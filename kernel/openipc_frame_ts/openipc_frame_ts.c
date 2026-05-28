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
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/device.h>

#include "openipc_frame_ts.h"

#define OPENIPC_FT_NAME           "openipc-frame-ts"
#define OPENIPC_FT_MAX_CHN        8
#define OPENIPC_FT_MAX_EVT_TYPE   2   /* MIPI_FS + ISP_FEND so far */
/*
 * Per-channel ring depth bounds. Must be powers of two — push uses
 * `head & (depth-1)` wrap math, which only works for power-of-2 depths.
 *
 * Default 256 ≈ 0.5 s buffer at 240 fps × 2 event types (8 KiB/chn). Fine
 * for the SR-anchor use case (reads cached state every ~5 s and tolerates
 * stale entries).
 *
 * For per-frame consumers — frame-by-frame latency histograms, evidence
 * chains, multi-sensor sync correlators — the default may be undersized
 * if the reader's dispatch latency is bursty (libevent under RTSP+RTCP
 * load on cv500 was observed to lose ~52 events/s during 30 s sessions,
 * see openhisilicon#200). Raise via `insmod open_openipc_frame_ts
 * ring_depth=4096` (16× default, ~8 s buffer; 128 KiB/chn × 8 chn = 1 MiB
 * total worst case).
 */
#define OPENIPC_FT_DEPTH_MIN      16
#define OPENIPC_FT_DEPTH_MAX      65536
#define OPENIPC_FT_DEPTH_DEFAULT  256

static unsigned int ring_depth = OPENIPC_FT_DEPTH_DEFAULT;
module_param(ring_depth, uint, 0444);
MODULE_PARM_DESC(ring_depth,
	"Per-channel ring depth (power of 2, 16..65536, default 256). "
	"Bump for per-frame consumers under bursty reader load.");

static unsigned int g_ring_depth;
static unsigned int g_ring_mask;
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
	struct openipc_frame_ts_event *ring;   /* kmalloc'd, g_ring_depth entries */
	unsigned int head;
	unsigned int tail;
	u32          seq[OPENIPC_FT_MAX_EVT_TYPE];
	u64          last_push_ns[OPENIPC_FT_MAX_EVT_TYPE];
	u64          dropped;          /* ring-overflow drops only (data loss) */
	u64          coalesced;        /* dedupe rejections (intentional) */
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

/*
 * Strobe-out: drive a GPIO synchronously with the sensor frame events,
 * for triggering external illumination / measurement equipment.
 *
 *   pulse — assert on MIPI_FS, deassert via hrtimer `strobe_pulse_us`
 *           microseconds later. Fixed-width pulse aligned with sensor
 *           row-0 start; suitable for flash / strobe lights.
 *   hold  — assert on MIPI_FS, deassert on ISP_FEND. High window equals
 *           the sensor readout duration (~10–30 ms depending on mode);
 *           suitable for a ring light that should be on only while
 *           pixels are being received.
 *
 * The GPIO must be SoC-direct (memory-mapped); GPIOs that can sleep
 * (I2C / SPI expanders) are rejected at configure time because the
 * firing path runs in hardirq context. One global strobe slot for now.
 */
enum strobe_mode {
	STROBE_OFF,
	STROBE_PULSE,
	STROBE_HOLD,
};

static int g_strobe_gpio = -1;
static enum strobe_mode g_strobe_mode = STROBE_OFF;
static unsigned int g_strobe_pulse_us = 1000;
static bool g_strobe_active_low;
static u64 g_strobe_events;
static struct hrtimer g_strobe_timer;
/* Serializes sysfs writes that re-request the GPIO. The firing path
 * doesn't take this — it only reads the scalar tunables. */
static DEFINE_MUTEX(g_strobe_lock);

static inline int strobe_active_level(void)
{
	return g_strobe_active_low ? 0 : 1;
}

static enum hrtimer_restart strobe_timer_cb(struct hrtimer *t)
{
	int gpio = READ_ONCE(g_strobe_gpio);

	if (gpio >= 0)
		gpio_set_value(gpio, !strobe_active_level());
	return HRTIMER_NORESTART;
}

/* Called from openipc_frame_ts_push after the ring push. Hardirq
 * context. */
static void strobe_on_event(unsigned int event_type)
{
	int gpio = READ_ONCE(g_strobe_gpio);
	enum strobe_mode mode = READ_ONCE(g_strobe_mode);

	if (gpio < 0 || mode == STROBE_OFF)
		return;

	if (event_type == OPENIPC_FT_EVT_MIPI_FS) {
		gpio_set_value(gpio, strobe_active_level());
		g_strobe_events++;
		if (mode == STROBE_PULSE) {
			u64 ns = (u64)READ_ONCE(g_strobe_pulse_us) * 1000ULL;

			hrtimer_start(&g_strobe_timer, ns_to_ktime(ns),
				      HRTIMER_MODE_REL);
		}
	} else if (event_type == OPENIPC_FT_EVT_ISP_FEND &&
		   mode == STROBE_HOLD) {
		gpio_set_value(gpio, !strobe_active_level());
	}
}

static inline unsigned int ring_count(const struct openipc_ft_chn *c)
{
	return (c->head - c->tail) & g_ring_mask;
}

static inline bool ring_full(const struct openipc_ft_chn *c)
{
	return ring_count(c) == g_ring_mask;
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
		/*
		 * Intentional rejection — the level-held vsync / FEND bits
		 * over-fire at high sensor IRQ rates and the dedupe window
		 * collapses them into one event per real frame. Not data
		 * loss; tracked separately from ring-overflow drops.
		 */
		c->coalesced++;
		spin_unlock_irqrestore(&c->lock, flags);
		return;
	}
	c->last_push_ns[event_type] = now_ns;

	if (ring_full(c)) {
		/* drop oldest */
		c->tail = (c->tail + 1) & g_ring_mask;
		c->dropped++;
	}
	e = &c->ring[c->head];
	e->pts_us     = div_u64(now_ns, 1000);
	e->wall_ns    = ktime_get_real_ns();
	e->vi_chn     = vi_chn;
	e->seq        = c->seq[event_type]++;
	e->event_type = event_type;
	e->reserved   = 0;
	c->head = (c->head + 1) & g_ring_mask;
	spin_unlock_irqrestore(&c->lock, flags);

	strobe_on_event(event_type);

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

			c->tail = (c->tail + 1) & g_ring_mask;
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
	case OPENIPC_FT_IOC_GET_COALESCED: {
		u64 total = 0;
		unsigned int i;
		unsigned long flags;

		for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
			spin_lock_irqsave(&g_chn[i].lock, flags);
			total += g_chn[i].coalesced;
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

/*
 * Strobe sysfs knobs, attached to the miscdev's underlying device so
 * they appear at /sys/class/misc/openipc-frame-ts/.
 */
static void strobe_release_gpio_locked(void)
{
	if (g_strobe_gpio < 0)
		return;
	gpio_set_value(g_strobe_gpio, !strobe_active_level());
	gpio_free(g_strobe_gpio);
	g_strobe_gpio = -1;
}

static ssize_t strobe_gpio_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", g_strobe_gpio);
}

static ssize_t strobe_gpio_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	int new_gpio;
	int ret;

	if (kstrtoint(buf, 0, &new_gpio))
		return -EINVAL;

	mutex_lock(&g_strobe_lock);

	hrtimer_cancel(&g_strobe_timer);
	strobe_release_gpio_locked();

	if (new_gpio < 0) {
		ret = count;
		goto out;
	}

	if (!gpio_is_valid(new_gpio)) {
		ret = -EINVAL;
		goto out;
	}

	ret = gpio_request(new_gpio, "openipc_frame_ts_strobe");
	if (ret)
		goto out;

	if (gpio_cansleep(new_gpio)) {
		gpio_free(new_gpio);
		ret = -EINVAL;
		goto out;
	}

	gpio_direction_output(new_gpio, g_strobe_active_low ? 1 : 0);
	g_strobe_gpio = new_gpio;
	ret = count;

out:
	mutex_unlock(&g_strobe_lock);
	return ret;
}
static DEVICE_ATTR(strobe_gpio, 0644, strobe_gpio_show, strobe_gpio_store);

static ssize_t strobe_mode_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	const char *s;

	switch (g_strobe_mode) {
	case STROBE_PULSE: s = "pulse"; break;
	case STROBE_HOLD:  s = "hold";  break;
	default:           s = "off";   break;
	}
	return scnprintf(buf, PAGE_SIZE, "%s\n", s);
}

static ssize_t strobe_mode_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	enum strobe_mode new_mode;

	if (sysfs_streq(buf, "off"))
		new_mode = STROBE_OFF;
	else if (sysfs_streq(buf, "pulse"))
		new_mode = STROBE_PULSE;
	else if (sysfs_streq(buf, "hold"))
		new_mode = STROBE_HOLD;
	else
		return -EINVAL;

	mutex_lock(&g_strobe_lock);
	if (new_mode == STROBE_OFF) {
		hrtimer_cancel(&g_strobe_timer);
		if (g_strobe_gpio >= 0)
			gpio_set_value(g_strobe_gpio, !strobe_active_level());
	}
	g_strobe_mode = new_mode;
	mutex_unlock(&g_strobe_lock);
	return count;
}
static DEVICE_ATTR(strobe_mode, 0644, strobe_mode_show, strobe_mode_store);

static ssize_t strobe_pulse_us_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_strobe_pulse_us);
}

static ssize_t strobe_pulse_us_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	unsigned int v;

	if (kstrtouint(buf, 0, &v))
		return -EINVAL;
	if (v < 1 || v > 100000)
		return -EINVAL;
	g_strobe_pulse_us = v;
	return count;
}
static DEVICE_ATTR(strobe_pulse_us, 0644,
		   strobe_pulse_us_show, strobe_pulse_us_store);

static ssize_t strobe_active_low_show(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_strobe_active_low ? 1 : 0);
}

static ssize_t strobe_active_low_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	bool v;

	if (kstrtobool(buf, &v))
		return -EINVAL;
	mutex_lock(&g_strobe_lock);
	g_strobe_active_low = v;
	if (g_strobe_gpio >= 0)
		gpio_direction_output(g_strobe_gpio, v ? 1 : 0);
	mutex_unlock(&g_strobe_lock);
	return count;
}
static DEVICE_ATTR(strobe_active_low, 0644,
		   strobe_active_low_show, strobe_active_low_store);

static ssize_t strobe_events_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%llu\n",
			 (unsigned long long)g_strobe_events);
}
static DEVICE_ATTR(strobe_events, 0444, strobe_events_show, NULL);

static struct attribute *strobe_attrs[] = {
	&dev_attr_strobe_gpio.attr,
	&dev_attr_strobe_mode.attr,
	&dev_attr_strobe_pulse_us.attr,
	&dev_attr_strobe_active_low.attr,
	&dev_attr_strobe_events.attr,
	NULL,
};
static const struct attribute_group strobe_attr_group = {
	.attrs = strobe_attrs,
};

static unsigned int sanitize_ring_depth(unsigned int req)
{
	unsigned int d = req;
	unsigned int p;

	if (d < OPENIPC_FT_DEPTH_MIN)
		d = OPENIPC_FT_DEPTH_MIN;
	if (d > OPENIPC_FT_DEPTH_MAX)
		d = OPENIPC_FT_DEPTH_MAX;

	/* round up to next power of 2 */
	p = 1;
	while (p < d)
		p <<= 1;
	return p;
}

static int __init openipc_ft_init(void)
{
	unsigned int i;
	int ret;

	g_ring_depth = sanitize_ring_depth(ring_depth);
	g_ring_mask  = g_ring_depth - 1;
	if (g_ring_depth != ring_depth)
		pr_info("openipc_frame_ts: ring_depth %u → %u (clamped + power-of-2)\n",
			ring_depth, g_ring_depth);

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		g_chn[i].ring = kmalloc_array(g_ring_depth,
			sizeof(struct openipc_frame_ts_event), GFP_KERNEL);
		if (!g_chn[i].ring) {
			ret = -ENOMEM;
			goto err_free_rings;
		}
		spin_lock_init(&g_chn[i].lock);
	}

	init_waitqueue_head(&g_wait);

	hrtimer_setup(&g_strobe_timer, strobe_timer_cb,
		      CLOCK_MONOTONIC, HRTIMER_MODE_REL);

	g_miscdev.minor = MISC_DYNAMIC_MINOR;
	g_miscdev.name  = OPENIPC_FT_NAME;
	g_miscdev.fops  = &g_fops;

	ret = misc_register(&g_miscdev);
	if (ret) {
		pr_err("openipc_frame_ts: misc_register failed: %d\n", ret);
		goto err_free_rings;
	}

	ret = sysfs_create_group(&g_miscdev.this_device->kobj,
				 &strobe_attr_group);
	if (ret)
		pr_warn("openipc_frame_ts: strobe sysfs attrs not created: %d\n",
			ret);

	pr_info("openipc_frame_ts: /dev/%s minor=%d, event types=%u, ring_depth=%u\n",
		OPENIPC_FT_NAME, g_miscdev.minor, OPENIPC_FT_MAX_EVT_TYPE,
		g_ring_depth);
	return 0;

err_free_rings:
	while (i--) {
		kfree(g_chn[i].ring);
		g_chn[i].ring = NULL;
	}
	return ret;
}

static void __exit openipc_ft_exit(void)
{
	unsigned int i;

	sysfs_remove_group(&g_miscdev.this_device->kobj, &strobe_attr_group);
	hrtimer_cancel(&g_strobe_timer);
	mutex_lock(&g_strobe_lock);
	g_strobe_mode = STROBE_OFF;
	strobe_release_gpio_locked();
	mutex_unlock(&g_strobe_lock);
	misc_deregister(&g_miscdev);

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		kfree(g_chn[i].ring);
		g_chn[i].ring = NULL;
	}
}

module_init(openipc_ft_init);
module_exit(openipc_ft_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OpenIPC sensor frame-event (PTS, CLOCK_REALTIME) capture");
