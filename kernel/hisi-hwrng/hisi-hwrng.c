// SPDX-License-Identifier: GPL-2.0
/*
 * HiSilicon on-die TRNG driver.
 *
 * Targets Linux 3.19 .. 7.0 across all OpenIPC HiSilicon SoCs that ship a
 * documented TRNG block. Operates in one of two modes, picked at compile time:
 *
 *   1. Modern (CONFIG_HW_RANDOM=y/m, kernel >= 4.0):
 *        register a struct hwrng; the kernel hwrng-core thread (krngd) reads
 *        from .read() and feeds the entropy pool itself via .quality.
 *        Also exposes /dev/hwrng. This is the 7.0 idiomatic path.
 *
 *   2. Pump (CONFIG_HW_RANDOM disabled, or kernel < 4.0):
 *        run a small kthread that batches reads and pushes entropy via
 *        add_hwgenerator_randomness() (added in 3.17, so 3.19+ has it).
 *        On modern kernels with hwrng-core also available, mode 1 is taken
 *        instead so the upstream path wins.
 *
 * On kernels where hwrng-core *is* enabled but is pre-4.0 (no krngd, no
 * .quality field), both paths are taken: hwrng_register() to expose
 * /dev/hwrng, plus the pump kthread to fill the entropy pool.
 */
#include <linux/io.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/version.h>

#if IS_ENABLED(CONFIG_HW_RANDOM)
#  define HISI_HWRNG_USE_CORE 1
#else
#  define HISI_HWRNG_USE_CORE 0
#endif

#if !HISI_HWRNG_USE_CORE || LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
#  define HISI_HWRNG_USE_PUMP 1
#  include <linux/delay.h>
#  include <linux/kthread.h>
#  include <linux/random.h>
#else
#  define HISI_HWRNG_USE_PUMP 0
#endif

/*
 * hw_random.h carries struct hwrng + add_hwgenerator_randomness() across the
 * full 3.19..7.0 range (random.h didn't pick up the latter until 5.x).
 * Header is safe to include even when CONFIG_HW_RANDOM is disabled.
 */
#include <linux/hw_random.h>

#include "../compat/kernel_compat.h"

#if defined(hi3516ev200) || defined(gk7205v200)
#  define HWRNG_BASE   0x10080000
#  define HWRNG_DATA   0x204
#elif defined(hi3516cv500)
#  define HWRNG_BASE   0x10090000
#  define HWRNG_DATA   0x204
#elif defined(hi3516cv300)
#  define HWRNG_BASE   0x120C0000
#  define HWRNG_DATA   0x204
#elif defined(hi3519v101)
#  define HWRNG_BASE   0x120C0000
#  define HWRNG_DATA   0x204
#elif defined(hi3516cv200)
#  define HWRNG_BASE   0x20280000
#  define HWRNG_DATA   0x004    /* V2 RNG_GEN layout */
#else
#  error "hisi-hwrng: no TRNG base address known for this CHIPARCH"
#endif

#define HWRNG_REG_SIZE   0x1000
#define HWRNG_QUALITY    250    /* per-mille; ~25% entropy estimate */

struct hisi_hwrng {
	void __iomem       *base;
	void __iomem       *data;
#if HISI_HWRNG_USE_CORE
	struct hwrng        rng;
#endif
#if HISI_HWRNG_USE_PUMP
	struct task_struct *pump;
#endif
};

static struct hisi_hwrng *g_hwrng;

static int hisi_hwrng_fill(struct hisi_hwrng *h, void *buf, size_t max)
{
	u32 *out = buf;
	size_t n = 0;

	while (n + sizeof(u32) <= max) {
		out[n / sizeof(u32)] = readl(h->data);
		n += sizeof(u32);
	}
	return n;
}

#if HISI_HWRNG_USE_CORE
static int hisi_hwrng_read(struct hwrng *rng, void *buf, size_t max, bool wait)
{
	struct hisi_hwrng *h = container_of(rng, struct hisi_hwrng, rng);

	return hisi_hwrng_fill(h, buf, max);
}
#endif

#if HISI_HWRNG_USE_PUMP
#define HWRNG_BATCH_BYTES   1024
#define HWRNG_INTERVAL_MS   1000

static void hisi_hwrng_feed(const void *buf, size_t bytes)
{
	unsigned int entropy_bits = (bytes * 8 * HWRNG_QUALITY) / 1000;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 2, 0)
	add_hwgenerator_randomness(buf, bytes, entropy_bits, true);
#else
	add_hwgenerator_randomness(buf, bytes, entropy_bits);
#endif
}

static int hisi_hwrng_pump(void *arg)
{
	struct hisi_hwrng *h = arg;
	u32 *buf;

	buf = kmalloc(HWRNG_BATCH_BYTES, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	while (!kthread_should_stop()) {
		int n = hisi_hwrng_fill(h, buf, HWRNG_BATCH_BYTES);
		hisi_hwrng_feed(buf, n);
		msleep_interruptible(HWRNG_INTERVAL_MS);
	}
	kfree(buf);
	return 0;
}
#endif /* HISI_HWRNG_USE_PUMP */

static int __init hisi_hwrng_init(void)
{
	struct hisi_hwrng *h;
	int ret;

	h = kzalloc(sizeof(*h), GFP_KERNEL);
	if (!h)
		return -ENOMEM;

	h->base = ioremap_nocache(HWRNG_BASE, HWRNG_REG_SIZE);
	if (!h->base) {
		ret = -ENOMEM;
		goto fail_free;
	}
	h->data = h->base + HWRNG_DATA;

#if HISI_HWRNG_USE_CORE
	h->rng.name = "hisi-hwrng";
	h->rng.read = hisi_hwrng_read;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
	h->rng.quality = HWRNG_QUALITY;
#endif
	ret = hwrng_register(&h->rng);
	if (ret)
		goto fail_unmap;
#endif

#if HISI_HWRNG_USE_PUMP
	h->pump = kthread_run(hisi_hwrng_pump, h, "hisi-hwrng");
	if (IS_ERR(h->pump)) {
		ret = PTR_ERR(h->pump);
#if HISI_HWRNG_USE_CORE
		hwrng_unregister(&h->rng);
#endif
		goto fail_unmap;
	}
#endif

	g_hwrng = h;
	pr_info("hisi-hwrng: TRNG at 0x%08lx+0x%x registered (%s)\n",
		(unsigned long)HWRNG_BASE, HWRNG_DATA,
#if HISI_HWRNG_USE_CORE && HISI_HWRNG_USE_PUMP
		"hwrng-core + pump"
#elif HISI_HWRNG_USE_CORE
		"hwrng-core"
#else
		"pump"
#endif
		);
	return 0;

fail_unmap:
	iounmap(h->base);
fail_free:
	kfree(h);
	return ret;
}

static void __exit hisi_hwrng_exit(void)
{
	if (!g_hwrng)
		return;
#if HISI_HWRNG_USE_PUMP
	if (g_hwrng->pump)
		kthread_stop(g_hwrng->pump);
#endif
#if HISI_HWRNG_USE_CORE
	hwrng_unregister(&g_hwrng->rng);
#endif
	iounmap(g_hwrng->base);
	kfree(g_hwrng);
	g_hwrng = NULL;
}

module_init(hisi_hwrng_init);
module_exit(hisi_hwrng_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenIPC");
MODULE_DESCRIPTION("HiSilicon on-die TRNG driver");
