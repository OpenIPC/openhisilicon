// SPDX-License-Identifier: GPL-2.0
/*
 * OpenIPC frame-timestamp capture (issue #144).
 *
 * Per-channel ring of (pts_us, wall_ns, event_type) tuples sampled
 * from per-SoC IRQ handlers at well-defined pipeline points. Exposed
 * via /dev/openipc-frame-ts.
 *
 * Today the chrdev emits two event types per frame:
 *   OPENIPC_FT_EVT_MIPI_FS   — MIPI RX driver, on FS short packet
 *                              (≈ sensor begins streaming row 0)
 *   OPENIPC_FT_EVT_ISP_FEND  — ISP front-end IRQ, on FEND bit set
 *                              (≈ ISP FE finished receiving last row)
 *
 * Their delta on the same physical frame is dominated by sensor
 * readout duration (HMAX × VMAX / pixel_clock), so consumers can
 * decompose latency budgets without an external probe.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/sched.h>

#include "openipc_frame_ts.h"

#define OPENIPC_FT_NAME           "openipc-frame-ts"
#define OPENIPC_FT_MAX_CHN        8
#define OPENIPC_FT_MAX_EVT_TYPE   2   /* MIPI_FS + ISP_FEND so far */
/*
 * Per-channel ring depth (power of 2). 256 events ≈ 0.5 s of buffer at
 * 240 fps × 2 event types — covers high-fps modes (imx335 800x480 240 fps)
 * with margin for reader-side jitter (scheduler latency, printf-to-disk).
 * At 32 B/event the per-channel cost is 8 KiB.
 */
#define OPENIPC_FT_DEPTH          256
/*
 * Drop double-pushes of the SAME event type within these intervals.
 *
 * MIPI_FS (1 ms): MIPI RX vsync IRQ on cv500 fires twice ~30–80 µs apart
 *   on ~4 % of frames. 1 ms is safely below any plausible sensor frame
 *   interval (1000 fps) yet catches the duplicate.
 *
 * ISP_FEND (25 ms): the ISP_INT_FE.FEND bit is a level signal — hardware
 *   holds it asserted across the entire inter-frame gap and re-sets it
 *   after the ISR's W1C clear while the underlying condition still
 *   holds. The ISR fires far more often than the sensor frame rate
 *   (other ISP bits), so without a long dedup we'd emit ~IRQ-rate worth
 *   of FEND events. 25 ms caps at 40 Hz — adequate for every supported
 *   sensor (max 30 fps single-stream on these SoCs) and still leaves
 *   margin for jitter.
 *
 * NB: dedup is per (chn, event_type). MIPI_FS and ISP_FEND for the
 * same frame can be tens of ms apart (= readout duration) — that's
 * the whole point — so dedup deliberately does NOT cross event types.
 */
#define OPENIPC_FT_MIN_INTERVAL_NS_FS    1000000
#define OPENIPC_FT_MIN_INTERVAL_NS_FEND  25000000

static const u64 openipc_ft_min_interval_ns[OPENIPC_FT_MAX_EVT_TYPE] = {
	[OPENIPC_FT_EVT_MIPI_FS]  = OPENIPC_FT_MIN_INTERVAL_NS_FS,
	[OPENIPC_FT_EVT_ISP_FEND] = OPENIPC_FT_MIN_INTERVAL_NS_FEND,
};

struct openipc_ft_chn {
	struct openipc_frame_ts_event ring[OPENIPC_FT_DEPTH];
	unsigned int head;
	unsigned int tail;
	u32          seq[OPENIPC_FT_MAX_EVT_TYPE];
	u64          last_push_ns[OPENIPC_FT_MAX_EVT_TYPE];
	u64          dropped;
	spinlock_t   lock;
};

struct openipc_ft_file {
	u32 chn_mask;
	u32 evt_mask;
};

static struct openipc_ft_chn g_chn[OPENIPC_FT_MAX_CHN];
static wait_queue_head_t g_wait;
static const unsigned int g_default_chn_mask =
	(1U << OPENIPC_FT_MAX_CHN) - 1;
static const unsigned int g_default_evt_mask = ~0U;

static struct miscdevice g_miscdev;

static inline unsigned int ring_count(const struct openipc_ft_chn *c)
{
	return (c->head - c->tail) & (OPENIPC_FT_DEPTH - 1);
}

static inline bool ring_full(const struct openipc_ft_chn *c)
{
	return ring_count(c) == (OPENIPC_FT_DEPTH - 1);
}

static inline bool ring_empty(const struct openipc_ft_chn *c)
{
	return c->head == c->tail;
}

void openipc_frame_ts_push(unsigned int vi_chn, unsigned int event_type)
{
	struct openipc_ft_chn *c;
	struct openipc_frame_ts_event *e;
	unsigned long flags;
	u64 now_ns;

	if (vi_chn >= OPENIPC_FT_MAX_CHN)
		return;
	if (event_type >= OPENIPC_FT_MAX_EVT_TYPE)
		return;

	now_ns = sched_clock();
	c = &g_chn[vi_chn];

	spin_lock_irqsave(&c->lock, flags);
	if (c->last_push_ns[event_type] &&
	    now_ns - c->last_push_ns[event_type] <
		    openipc_ft_min_interval_ns[event_type]) {
		c->dropped++;
		spin_unlock_irqrestore(&c->lock, flags);
		return;
	}
	c->last_push_ns[event_type] = now_ns;

	if (ring_full(c)) {
		/* drop oldest */
		c->tail = (c->tail + 1) & (OPENIPC_FT_DEPTH - 1);
		c->dropped++;
	}
	e = &c->ring[c->head];
	e->pts_us     = div_u64(now_ns, 1000);
	e->wall_ns    = ktime_get_real_ns();
	e->vi_chn     = vi_chn;
	e->seq        = c->seq[event_type]++;
	e->event_type = event_type;
	e->reserved   = 0;
	c->head = (c->head + 1) & (OPENIPC_FT_DEPTH - 1);
	spin_unlock_irqrestore(&c->lock, flags);

	wake_up_interruptible(&g_wait);
}
EXPORT_SYMBOL(openipc_frame_ts_push);

static int ft_open(struct inode *inode, struct file *file)
{
	struct openipc_ft_file *priv;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->chn_mask = g_default_chn_mask;
	priv->evt_mask = g_default_evt_mask;
	file->private_data = priv;
	return 0;
}

static int ft_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

static bool event_passes(const struct openipc_frame_ts_event *e,
			 u32 chn_mask, u32 evt_mask)
{
	if (!(chn_mask & (1U << e->vi_chn)))
		return false;
	if (e->event_type < 32 && !(evt_mask & (1U << e->event_type)))
		return false;
	return true;
}

static bool any_pending(u32 chn_mask, u32 evt_mask)
{
	unsigned int i;
	unsigned long flags;

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		struct openipc_ft_chn *c = &g_chn[i];
		bool hit = false;

		if (!(chn_mask & (1U << i)))
			continue;

		spin_lock_irqsave(&c->lock, flags);
		if (!ring_empty(c)) {
			/* Cheap heuristic: any unread events on a permitted
			 * channel ⇒ pending. The fd may later discard some
			 * after a per-event evt_mask check in pop_one; that's
			 * fine — read() handles the empty-after-filter case
			 * by sleeping again. */
			hit = true;
		}
		spin_unlock_irqrestore(&c->lock, flags);

		if (hit)
			return true;
	}
	return false;
}

static bool pop_one(u32 chn_mask, u32 evt_mask,
		    struct openipc_frame_ts_event *out)
{
	unsigned int i;
	unsigned long flags;
	bool got = false;

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		struct openipc_ft_chn *c = &g_chn[i];

		if (!(chn_mask & (1U << i)))
			continue;

		spin_lock_irqsave(&c->lock, flags);
		while (!ring_empty(c)) {
			struct openipc_frame_ts_event *cand =
				&c->ring[c->tail];

			c->tail = (c->tail + 1) & (OPENIPC_FT_DEPTH - 1);
			if (event_passes(cand, chn_mask, evt_mask)) {
				*out = *cand;
				got = true;
				break;
			}
			/* Drop events that don't match this fd's evt_mask;
			 * they were consumed from the ring already. */
		}
		spin_unlock_irqrestore(&c->lock, flags);

		if (got)
			break;
	}
	return got;
}

static ssize_t ft_read(struct file *file, char __user *buf, size_t count,
		       loff_t *ppos)
{
	struct openipc_ft_file *priv = file->private_data;
	struct openipc_frame_ts_event ev;
	size_t copied = 0;

	if (count < sizeof(ev))
		return -EINVAL;

	while (copied + sizeof(ev) <= count) {
		if (!pop_one(priv->chn_mask, priv->evt_mask, &ev)) {
			if (copied)
				break;
			if (file->f_flags & O_NONBLOCK)
				return -EAGAIN;
			if (wait_event_interruptible(
				    g_wait, any_pending(priv->chn_mask,
							priv->evt_mask)))
				return -ERESTARTSYS;
			continue;
		}

		if (copy_to_user(buf + copied, &ev, sizeof(ev)))
			return -EFAULT;
		copied += sizeof(ev);
	}

	return copied;
}

static unsigned int ft_poll(struct file *file, poll_table *table)
{
	struct openipc_ft_file *priv = file->private_data;
	unsigned int mask = 0;

	poll_wait(file, &g_wait, table);

	if (any_pending(priv->chn_mask, priv->evt_mask))
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static long ft_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct openipc_ft_file *priv = file->private_data;
	void __user *uarg = (void __user *)arg;

	switch (cmd) {
	case OPENIPC_FT_IOC_SET_CHANNEL_MASK: {
		u32 m;

		if (copy_from_user(&m, uarg, sizeof(m)))
			return -EFAULT;
		priv->chn_mask = m;
		return 0;
	}
	case OPENIPC_FT_IOC_SET_EVENT_MASK: {
		u32 m;

		if (copy_from_user(&m, uarg, sizeof(m)))
			return -EFAULT;
		priv->evt_mask = m;
		return 0;
	}
	case OPENIPC_FT_IOC_GET_DROPPED: {
		u64 total = 0;
		unsigned int i;
		unsigned long flags;

		for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
			spin_lock_irqsave(&g_chn[i].lock, flags);
			total += g_chn[i].dropped;
			spin_unlock_irqrestore(&g_chn[i].lock, flags);
		}
		if (copy_to_user(uarg, &total, sizeof(total)))
			return -EFAULT;
		return 0;
	}
	default:
		return -ENOTTY;
	}
}

static const struct file_operations g_fops = {
	.owner          = THIS_MODULE,
	.open           = ft_open,
	.release        = ft_release,
	.read           = ft_read,
	.poll           = ft_poll,
	.unlocked_ioctl = ft_ioctl,
};

static int __init openipc_ft_init(void)
{
	unsigned int i;
	int ret;

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++)
		spin_lock_init(&g_chn[i].lock);

	init_waitqueue_head(&g_wait);

	g_miscdev.minor = MISC_DYNAMIC_MINOR;
	g_miscdev.name  = OPENIPC_FT_NAME;
	g_miscdev.fops  = &g_fops;

	ret = misc_register(&g_miscdev);
	if (ret) {
		pr_err("openipc_frame_ts: misc_register failed: %d\n", ret);
		return ret;
	}

	pr_info("openipc_frame_ts: /dev/%s minor=%d, event types=%u\n",
		OPENIPC_FT_NAME, g_miscdev.minor, OPENIPC_FT_MAX_EVT_TYPE);
	return 0;
}

static void __exit openipc_ft_exit(void)
{
	misc_deregister(&g_miscdev);
}

module_init(openipc_ft_init);
module_exit(openipc_ft_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OpenIPC sensor frame-event (PTS, CLOCK_REALTIME) capture");
