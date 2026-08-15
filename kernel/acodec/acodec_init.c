#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "type.h"
#include "osal.h"

extern int acodec_init(void);
extern void acodec_exit(void);
extern unsigned int init_delay_time;
module_param(init_delay_time, uint, S_IRUGO);

#if defined(gk7205v200) || defined(hi3516ev200)
#define ACODEC_RCTUNE_WATCHDOG
#endif

#ifdef ACODEC_RCTUNE_WATCHDOG

/*
 * The inner codec tunes its ADC RC network whenever ACODEC_SET_I2S1_FS pulses
 * en_rctune, and the code it latches is only correct for the codec clock that
 * was running at the time. Register 0x18 reports that live: bit 3 stays set
 * for as long as the tuning still matches the clock.
 *
 * Enabling an AI device reprograms the AIAO TX0 clock, which invalidates a
 * tuning that was correct a moment earlier, and nothing recalibrates
 * afterwards. Traced on gk7205v200 inside a single 70 ms window: 0x18 read
 * 0x3F with the codec fully configured, then 0x25 the instant
 * AIAO_CLK_TX0_CRG/CFG moved. From there the ADC modulator runs mistuned and
 * settles into a limit cycle -- an audible tone at fs*39/1024 and its
 * harmonics, up to 20 dB over the noise floor, lasting as long as the device
 * stays up. Confirmed on gk7205v200 and hi3516ev200. See OpenIPC/majestic#285.
 *
 * The codec cannot observe the clock moving, but it can observe the result.
 * Watch the validity bit and retune when it goes stale: that is the same pulse
 * the SET_I2S1_FS ioctl issues, so the tuning follows the clock no matter
 * which consumer moves it, or when.
 */

#define ACODEC_REGS_BASE 0x100f0000
#define ACODEC_REGS_SIZE 0x1000

#define ACODEC_ANAREG1 0x04
#define ACODEC_EN_RCTUNE (1u << 1)
#define ACODEC_CAL_STATUS 0x18
#define ACODEC_CAL_VALID (1u << 3)

/* One retune settles it, so a second is a long time to sound wrong. Once it is
 * clear the tuning will not take, stop pulsing every second over a codec
 * somebody may be reading from. */
#define ACODEC_POLL_MS 1000
#define ACODEC_POLL_STUCK_MS 10000
#define ACODEC_RETUNE_LIMIT 10

static int rctune_watchdog = 1;
module_param(rctune_watchdog, int, S_IRUGO);
MODULE_PARM_DESC(rctune_watchdog,
		 "retune the ADC when a clock change invalidates it (default 1)");

static void *acodec_regs;
static osal_timer_t rctune_timer;
static unsigned int rctune_failures;

#define acodec_readl(off) osal_readl((uintptr_t)acodec_regs + (off))
#define acodec_writel(v, off) osal_writel(v, (uintptr_t)acodec_regs + (off))

static void acodec_rctune(void)
{
	unsigned int anareg1 = acodec_readl(ACODEC_ANAREG1);

	acodec_writel(anareg1 & ~ACODEC_EN_RCTUNE, ACODEC_ANAREG1);
	osal_udelay(30);
	acodec_writel(anareg1 | ACODEC_EN_RCTUNE, ACODEC_ANAREG1);
}

static void acodec_rctune_check(unsigned long data)
{
	unsigned long period = ACODEC_POLL_MS;

	if (acodec_readl(ACODEC_CAL_STATUS) & ACODEC_CAL_VALID) {
		rctune_failures = 0;
	} else {
		acodec_rctune();

		rctune_failures++;
		if (rctune_failures == 1)
			osal_printk("acodec: ADC tuning stale after a clock "
				    "change, retuning\n");
		else if (rctune_failures == ACODEC_RETUNE_LIMIT)
			osal_printk("acodec: ADC tuning will not latch, "
				    "backing off\n");

		if (rctune_failures >= ACODEC_RETUNE_LIMIT)
			period = ACODEC_POLL_STUCK_MS;
	}

	osal_set_timer(&rctune_timer, period);
}

static void acodec_rctune_watchdog_init(void)
{
	if (!rctune_watchdog)
		return;

	acodec_regs = osal_ioremap(ACODEC_REGS_BASE, ACODEC_REGS_SIZE);
	if (acodec_regs == NULL) {
		osal_printk("acodec: cannot map codec registers, ADC tuning "
			    "will not be maintained\n");
		return;
	}

	if (osal_timer_init(&rctune_timer)) {
		osal_iounmap(acodec_regs);
		acodec_regs = NULL;
		return;
	}

	rctune_timer.function = acodec_rctune_check;
	rctune_timer.data = 0;
	osal_set_timer(&rctune_timer, ACODEC_POLL_MS);
}

static void acodec_rctune_watchdog_exit(void)
{
	if (acodec_regs == NULL)
		return;

	osal_del_timer(&rctune_timer);
	osal_timer_destory(&rctune_timer);
	osal_iounmap(acodec_regs);
	acodec_regs = NULL;
}

#else /* no inner codec we know the tuning-status register of */

static void acodec_rctune_watchdog_init(void)
{
}
static void acodec_rctune_watchdog_exit(void)
{
}

#endif

static __init int acodec_mod_init(void)
{
	int ret = acodec_init();

	if (ret)
		return ret;

	acodec_rctune_watchdog_init();
	return 0;
}
static __exit void acodec_mod_exit(void)
{
	acodec_rctune_watchdog_exit();
	acodec_exit();
}

module_init(acodec_mod_init);
module_exit(acodec_mod_exit);

MODULE_LICENSE("GPL");
