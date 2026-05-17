// SPDX-License-Identifier: GPL-2.0
/*
 * OpenIPC frame-timestamp capture (issue #144).
 *
 * Per-channel ring of (pts_us, wall_ns) tuples sampled from per-SoC IRQ
 * handlers at sensor frame-start. Exposed via /dev/openipc-frame-ts.
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

#define OPENIPC_FT_NAME       "openipc-frame-ts"
#define OPENIPC_FT_MAX_CHN    8
#define OPENIPC_FT_DEPTH      64    /* power of 2 */

struct openipc_ft_chn {
	struct openipc_frame_ts_event ring[OPENIPC_FT_DEPTH];
	unsigned int head;
	unsigned int tail;
	u32 seq;
	u64 dropped;
	spinlock_t lock;
};

struct openipc_ft_file {
	u32 chn_mask;
};

static struct openipc_ft_chn g_chn[OPENIPC_FT_MAX_CHN];
static wait_queue_head_t g_wait;
static unsigned int g_default_chn_mask = (1U << OPENIPC_FT_MAX_CHN) - 1;

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

void openipc_frame_ts_push(unsigned int vi_chn)
{
	struct openipc_ft_chn *c;
	struct openipc_frame_ts_event *e;
	unsigned long flags;
	u64 now_ns;

	if (vi_chn >= OPENIPC_FT_MAX_CHN)
		return;

	now_ns = sched_clock();
	c = &g_chn[vi_chn];

	spin_lock_irqsave(&c->lock, flags);
	if (ring_full(c)) {
		/* drop oldest */
		c->tail = (c->tail + 1) & (OPENIPC_FT_DEPTH - 1);
		c->dropped++;
	}
	e = &c->ring[c->head];
	e->pts_us = div_u64(now_ns, 1000);
	e->wall_ns = ktime_get_real_ns();
	e->vi_chn = vi_chn;
	e->seq = c->seq++;
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
	file->private_data = priv;
	return 0;
}

static int ft_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

static bool any_pending(u32 mask)
{
	unsigned int i;

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		if ((mask & (1U << i)) && !ring_empty(&g_chn[i]))
			return true;
	}
	return false;
}

static bool pop_one(u32 mask, struct openipc_frame_ts_event *out)
{
	unsigned int i;
	unsigned long flags;
	bool got = false;

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		struct openipc_ft_chn *c = &g_chn[i];

		if (!(mask & (1U << i)))
			continue;

		spin_lock_irqsave(&c->lock, flags);
		if (!ring_empty(c)) {
			*out = c->ring[c->tail];
			c->tail = (c->tail + 1) & (OPENIPC_FT_DEPTH - 1);
			got = true;
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
		if (!pop_one(priv->chn_mask, &ev)) {
			if (copied)
				break;
			if (file->f_flags & O_NONBLOCK)
				return -EAGAIN;
			if (wait_event_interruptible(g_wait,
						     any_pending(priv->chn_mask)))
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
	unsigned int i;
	unsigned long flags;

	poll_wait(file, &g_wait, table);

	for (i = 0; i < OPENIPC_FT_MAX_CHN; i++) {
		struct openipc_ft_chn *c = &g_chn[i];

		if (!(priv->chn_mask & (1U << i)))
			continue;

		spin_lock_irqsave(&c->lock, flags);
		if (!ring_empty(c))
			mask |= POLLIN | POLLRDNORM;
		spin_unlock_irqrestore(&c->lock, flags);

		if (mask)
			break;
	}

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

	pr_info("openipc_frame_ts: /dev/%s minor=%d\n",
		OPENIPC_FT_NAME, g_miscdev.minor);
	return 0;
}

static void __exit openipc_ft_exit(void)
{
	misc_deregister(&g_miscdev);
}

module_init(openipc_ft_init);
module_exit(openipc_ft_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OpenIPC sensor frame-start (PTS, CLOCK_REALTIME) capture");
