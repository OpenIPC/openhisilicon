/*
 * cv200 V2 OSAL shim — re-exports kernel APIs that the binary blobs
 * in kernel/obj/hi3516cv200/ import directly. Targets Linux 6.x and 7.0.
 *
 * V2 (ARM926EJ-S) generation has no OSAL abstraction layer — the vendor
 * blobs were compiled against kernel 4.9.37 and embed the call sites
 * verbatim. Where the underlying kernel API was removed or renamed,
 * load-time linking would fail with "Unknown symbol" errors. This module
 * provides EXPORT_SYMBOL wrappers under the old names so depmod resolves
 * them.
 *
 * Symbols re-exported:
 *
 *   do_gettimeofday          (removed 5.0)
 *   register_sysctl_table    (removed 6.6 — best-effort flat path)
 *   init_timer_key           (callback signature changed; struct field
 *                             drift — runtime correctness BROKEN for
 *                             blobs that touch timer->data; see caveat
 *                             below)
 *   strlcpy                  (removed 6.8)
 *
 * Notably NOT re-exported here (must be handled separately):
 *
 *   printk → _printk         The kernel renamed the export 'printk' to
 *                            '_printk' in 6.0. We can't easily wrap it
 *                            because <linux/printk.h> defines 'printk'
 *                            as a macro globally, fouling any attempt
 *                            to define a function of the same name.
 *                            Use objcopy --redefine-sym on the blob
 *                            .o files at build time (see Kbuild).
 *
 *   unregister_sysctl_table  Still exported natively by the modern
 *                            kernel — no shim needed.
 *
 *   filp_open                Still exported natively — no shim needed.
 *
 * struct timer_list .data caveat
 * ------------------------------
 * 4.15 timer rework dropped the .data field; the callback signature
 * changed from `void(*)(unsigned long data)` to `void(*)(struct timer_list*)`.
 * Blobs that use init_timer_key (chnl, viu, vpss) embed assumptions
 * about both. The shim resolves the symbol but timers will never fire
 * with the correct argument — those modules need a kernel patch to
 * restore the .data field or a blob recompile to fix the workflow.
 *
 * Must be inserted before any cv200 blob module (load_hisilicon order:
 * open_mmz → open_himedia → open_v2_shim → open_base → ...).
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/timekeeping.h>
#include <linux/timer.h>
#include <linux/sysctl.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("cv200 V2 OSAL shim — re-exports removed-since-4.9 kernel APIs");

/* The entire shim body is only meaningful on kernels >= 5.0 where the
 * legacy symbols this module re-exports have actually been removed. On
 * older kernels (the cv200_lite production target, 4.9.37) every symbol
 * here is already exported by the kernel itself and re-defining them
 * causes "conflicting types" errors at module-compile time, plus the
 * runtime would refuse to load duplicate exports anyway. Make the
 * module a no-op shell on those kernels — the Kbuild dispatch stays
 * unchanged so the same Kbuild line works on both 4.9 and 7.0. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)

/* -------------------------------------------------------------------
 * do_gettimeofday() — removed in 5.0 (commit 192a82f9000b).
 * Define a shim with a unique internal name, then alias the legacy
 * symbol to it via asm. Avoids the do_gettimeofday() macro in
 * kernel_compat.h that would collide with a function definition.
 * ------------------------------------------------------------------- */

struct compat_v2_timeval {
	long tv_sec;
	long tv_usec;
};

void __v2_shim_do_gettimeofday(struct compat_v2_timeval *tv)
{
	struct timespec64 ts;
	ktime_get_real_ts64(&ts);
	tv->tv_sec = (long)ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}
/* kernel_compat.h #defines do_gettimeofday(tvp) → compat_do_gettimeofday(tvp).
 * Undef before declaring the alias, otherwise the extern declaration would
 * alias 'compat_do_gettimeofday' instead of the legacy 'do_gettimeofday'. */
#undef do_gettimeofday
extern void do_gettimeofday(struct compat_v2_timeval *tv)
	__attribute__((alias("__v2_shim_do_gettimeofday")));
EXPORT_SYMBOL(do_gettimeofday);

/* -------------------------------------------------------------------
 * register_sysctl_table() — removed in 6.6 (commit c1d967dd49a3) in
 * favour of register_sysctl() which takes a path string. Blob (base/sys)
 * walks the legacy .child-linked ctl_table tree. Register the leaf
 * flat under /proc/sys/hisi/ — good enough for symbol resolution; the
 * blob's expected paths may not materialize.
 * ------------------------------------------------------------------- */

struct ctl_table_header *register_sysctl_table(struct ctl_table *table)
{
	int count = 0;
	while (table[count].procname)
		count++;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
	return register_sysctl_sz("hisi", table, count);
#else
	return register_sysctl("hisi", table);
#endif
}
EXPORT_SYMBOL(register_sysctl_table);

/* -------------------------------------------------------------------
 * init_timer_key() — modern signature requires
 * void (*)(struct timer_list *). Blobs were built with the legacy
 * void (*)(unsigned long). Cast and call timer_setup — symbol resolves;
 * runtime correctness is broken (see caveat at top of file).
 * ------------------------------------------------------------------- */

void __v2_shim_init_timer_key(struct timer_list *timer,
			       void (*func)(struct timer_list *),
			       unsigned int flags,
			       const char *name,
			       struct lock_class_key *key)
{
	timer_setup(timer, func, flags);
	pr_warn_once("v2_shim: init_timer_key() called — blob timer with "
		     "legacy void(*)(unsigned long) callback resolved but "
		     "won't fire correctly\n");
}
extern void init_timer_key(struct timer_list *timer,
			   void (*func)(struct timer_list *),
			   unsigned int flags,
			   const char *name, struct lock_class_key *key)
	__attribute__((alias("__v2_shim_init_timer_key")));
EXPORT_SYMBOL(init_timer_key);

/* -------------------------------------------------------------------
 * strlcpy() — removed in 6.8. Map to strscpy and return size_t (blobs
 * don't check). Internal name to avoid the macro in <linux/string.h>.
 * ------------------------------------------------------------------- */

size_t __v2_shim_strlcpy(char *dst, const char *src, size_t size)
{
	ssize_t r;
	if (!size)
		return strlen(src);
	r = sized_strscpy(dst, src, size);
	return (r < 0) ? size : (size_t)r;
}
/* <linux/string.h> 6.x+ defines strlcpy as a macro to sized_strscpy with
 * __must_be_cstr() compile-time array checks — incompatible with a
 * function declaration. Undef so the alias attaches to the legacy symbol. */
#undef strlcpy
extern size_t strlcpy(char *dst, const char *src, size_t size)
	__attribute__((alias("__v2_shim_strlcpy")));
EXPORT_SYMBOL(strlcpy);

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

/* -------------------------------------------------------------------
 * Module init — empty; we exist purely to provide exports (or are a
 * no-op on pre-5.0 kernels where the kernel still ships every symbol
 * we'd otherwise re-export).
 * ------------------------------------------------------------------- */

static int __init v2_shim_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	pr_info("cv200 V2 OSAL shim: ready "
		"(do_gettimeofday, register_sysctl_table, "
		"init_timer_key, strlcpy)\n");
#else
	pr_info("cv200 V2 OSAL shim: no-op on this kernel (4.x); legacy "
		"symbols still natively exported\n");
#endif
	return 0;
}

static void __exit v2_shim_exit(void)
{
}

module_init(v2_shim_init);
module_exit(v2_shim_exit);
