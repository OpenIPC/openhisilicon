// SPDX-License-Identifier: GPL-2.0
/*
 * XMedia NPU driver for the GK7205V500 family (V500/V510/V530, GK7202V330).
 *
 * The NPU executes a compiled command stream (.xmm) on its own sequencer;
 * the CPU only points it at the stream, starts it and fields the interrupt.
 * Userspace (libxmedia_npu.so) allocates every buffer from MMZ itself and
 * talks to /dev/npu.0 through the ioctls in npu_ioctl.h, plus an mmap of
 * the register page. Jobs are serialised in a pending queue and collected
 * from a completion queue by id.
 */

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

#include "npu_ioctl.h"

/* sys_config/gk7205v500: MOD_ID_NPU = 64, 450 MHz is the SDK's setting */
#define NPU_MOD_ID		64
#define NPU_CLK_MHZ		450
extern void sysconfig_module_set_clk(int mod_id, unsigned int clk_freq,
				     void *ext_param);

/* NPU clock gate and soft reset in the CRG */
#define CRG_BASE		0x12010000
#define CRG_NPU_CLK		0xb4
#define CRG_NPU_SRST		BIT(0)
#define CRG_NPU_CLK_EN		BIT(1)

/* Registers */
#define NPU_VERSION		0x000
#define NPU_DDR_BASE		0x004
#define NPU_CMD_ADDR		0x008
#define NPU_CMD_SIZE		0x00c
#define NPU_INT_EN		0x010
#define NPU_INT_CLR		0x014
#define NPU_INT_STATUS		0x018
#define NPU_START		0x020
#define NPU_DEBUG		0x024
#define NPU_DEBUG_BRKPC		0x028
#define NPU_RUN_CYCLES		0x02c
#define NPU_STCT_ADDR		0x034
#define NPU_STCT_SIZE		0x038
#define NPU_DFX_RPT		0x03c
#define NPU_END_ADDR		0x040

/* NPU_INT_STATUS / NPU_INT_EN */
#define NPU_INT_DONE		BIT(0)
#define NPU_INT_SI		BIT(1)
#define NPU_INT_DFX		BIT(2)
#define NPU_INT_PC		BIT(3)
#define NPU_INT_ALL		(NPU_INT_DONE | NPU_INT_SI | \
				 NPU_INT_DFX | NPU_INT_PC)

/* NPU_INT_CLR: same events, but WFI sits at bit 2 and shifts DFX/PC up */
#define NPU_CLR_DONE		BIT(0)
#define NPU_CLR_SI		BIT(1)
#define NPU_CLR_WFI		BIT(2)
#define NPU_CLR_DFX		BIT(3)
#define NPU_CLR_PC		BIT(4)

/* NPU_DEBUG */
#define NPU_DEBUG_MODE		BIT(0)
#define NPU_DEBUG_MAILBOX_WR	BIT(1)
#define NPU_DEBUG_STCT_RPT	BIT(8)

static const u32 npu_mailbox[4] = { 0x1a8, 0x1ac, 0x1c8, 0x1cc };

/* Device state; the first three as the SDK numbers them */
enum {
	NPU_IDLE = 1,
	NPU_BUSY = 2,
	NPU_ABORTED = 3,	/* DFX error; cleared by NPU_IOC_RESET */
	NPU_DRAINING = 4,	/* an abandoned job still owns the NPU */
};

#define NPU_RESET_POLLS		10000	/* x 10-20 us */

#define NPU_DESTROY_TIMEOUT	1000	/* jiffies, as the SDK driver */

static unsigned int memsize = 0x400000;
module_param(memsize, uint, 0444);
MODULE_PARM_DESC(memsize, "memsize=0x400000");

struct npu_job {
	struct list_head node;
	struct npu_start_params params;
	struct npu_job_status status;
	bool orphan;		/* its queue is gone; free it once it ends */
};

struct npu_dev {
	void __iomem *regs;
	phys_addr_t regs_phys;
	void __iomem *clk_gate;		/* CRG_NPU_CLK */
	int irq;			/* for remove */
	struct device *dev;
	struct miscdevice misc;

	struct mutex ref_lock;		/* file opens, which hold the clock */
	unsigned int refs;
	struct mutex open_lock;		/* NPU_IOC_OPEN/CLOSE */
	unsigned int opened;
	struct mutex queue_lock;	/* NPU_IOC_CREATE/DESTROY_QUEUE */
	unsigned int queues;

	/*
	 * state, cur, last_id and both lists. The hard IRQ handler only
	 * writes cur->status and state; everything that moves a job runs in
	 * process or IRQ-thread context under this mutex. cur is only ever
	 * freed by the IRQ thread or after the IRQ is gone, never under the
	 * hard handler. Lock order: open_lock, queue_lock, state_lock.
	 */
	struct mutex state_lock;
	int state;
	struct npu_job *cur;
	u32 last_id;
	struct list_head pending;
	struct list_head done;

	wait_queue_head_t wq;
	unsigned int events;		/* bumped on every wake-up */
};

static inline u32 npu_read(struct npu_dev *npu, u32 reg)
{
	return readl(npu->regs + reg);
}

static inline void npu_write(struct npu_dev *npu, u32 reg, u32 val)
{
	writel(val, npu->regs + reg);
}

static void npu_update(struct npu_dev *npu, u32 reg, u32 mask, u32 val)
{
	npu_write(npu, reg, (npu_read(npu, reg) & ~mask) | (val & mask));
}

static void npu_clock(struct npu_dev *npu, bool on)
{
	u32 v = readl(npu->clk_gate);

	if (on)
		v |= CRG_NPU_CLK_EN;
	else
		v &= ~CRG_NPU_CLK_EN;
	writel(v, npu->clk_gate);
}

/*
 * Holds the NPU in reset until its running cycle counter clears, which
 * stops whatever it was executing, as the SDK's library does from
 * userspace.
 */
static int npu_soft_reset(struct npu_dev *npu)
{
	int polls = 0;

	writel(readl(npu->clk_gate) | CRG_NPU_SRST, npu->clk_gate);
	while (npu_read(npu, NPU_RUN_CYCLES) && ++polls < NPU_RESET_POLLS)
		usleep_range(10, 20);
	writel(readl(npu->clk_gate) & ~CRG_NPU_SRST, npu->clk_gate);
	if (polls == NPU_RESET_POLLS) {
		dev_err(npu->dev, "soft reset: the NPU did not stop\n");
		return -EIO;
	}
	/* write-one-to-clear what is left of the DFX report */
	npu_write(npu, NPU_DFX_RPT, npu_read(npu, NPU_DFX_RPT));
	return 0;
}

static void npu_clear_int(struct npu_dev *npu, u32 status)
{
	u32 clr = npu_read(npu, NPU_INT_CLR);

	if (status & NPU_INT_DONE)
		clr |= NPU_CLR_DONE;
	if (status & NPU_INT_SI)
		clr |= NPU_CLR_SI;
	if (status & NPU_INT_DFX)
		clr |= NPU_CLR_DFX;
	if (status & NPU_INT_PC)
		clr |= NPU_CLR_PC;
	npu_write(npu, NPU_INT_CLR, clr);
}

static void npu_hw_open(struct npu_dev *npu, u32 base, u32 end)
{
	npu_clear_int(npu, npu_read(npu, NPU_INT_STATUS));
	npu_update(npu, NPU_INT_EN, NPU_INT_ALL, NPU_INT_ALL);
	npu_write(npu, NPU_DDR_BASE, base);
	npu_write(npu, NPU_END_ADDR, end);
	npu_write(npu, NPU_DEBUG_BRKPC, 0xffffffff);
}

static void npu_hw_close(struct npu_dev *npu)
{
	npu_update(npu, NPU_DEBUG, NPU_DEBUG_STCT_RPT, 0);
	mutex_lock(&npu->state_lock);
	/* a draining job still has to raise its interrupt to be let go */
	if (npu->state != NPU_DRAINING) {
		npu_clear_int(npu, npu_read(npu, NPU_INT_STATUS));
		npu_update(npu, NPU_INT_EN, NPU_INT_ALL, 0);
	}
	mutex_unlock(&npu->state_lock);
}

static void npu_hw_start(struct npu_dev *npu, const struct npu_start_params *p)
{
	int i;

	npu_write(npu, NPU_CMD_ADDR, p->cmd_addr);
	npu_write(npu, NPU_CMD_SIZE, p->cmd_size);

	/* the mailboxes only take writes with the debug port opened */
	npu_update(npu, NPU_DEBUG, NPU_DEBUG_MODE | NPU_DEBUG_MAILBOX_WR,
		   NPU_DEBUG_MAILBOX_WR);
	for (i = 0; i < ARRAY_SIZE(npu_mailbox); i++)
		if (p->dynamic_input[i] != 0xffffffff)
			npu_write(npu, npu_mailbox[i], p->dynamic_input[i]);
	npu_update(npu, NPU_DEBUG, NPU_DEBUG_MODE | NPU_DEBUG_MAILBOX_WR, 0);

	if (p->base_addr)
		npu_write(npu, NPU_DDR_BASE, p->base_addr);
	if (p->end_addr)
		npu_write(npu, NPU_END_ADDR, p->end_addr);
	npu_update(npu, NPU_START, BIT(0), BIT(0));
}

static void npu_wake(struct npu_dev *npu)
{
	npu->events++;
	wake_up_all(&npu->wq);
}

/* Caller holds state_lock. Starts the next pending job, or goes idle. */
static void npu_start_next(struct npu_dev *npu)
{
	struct npu_job *job;

	job = list_first_entry_or_null(&npu->pending, struct npu_job, node);
	if (!job) {
		npu->cur = NULL;
		npu->state = NPU_IDLE;
		return;
	}
	list_del(&job->node);
	job->status.state = NPU_JOB_PROCESS;
	npu->cur = job;
	npu_hw_start(npu, &job->params);
}

/* Caller holds state_lock. Hands a job that has ended to its collector. */
static void npu_retire(struct npu_dev *npu, struct npu_job *job)
{
	if (job->orphan)
		kfree(job);
	else
		list_add_tail(&job->node, &npu->done);
}

static irqreturn_t npu_irq(int irq, void *data)
{
	struct npu_dev *npu = data;
	struct npu_job *job = READ_ONCE(npu->cur);
	u32 status = npu_read(npu, NPU_INT_STATUS);

	if (job) {
		if (status & NPU_INT_SI)
			job->status.state = NPU_JOB_SYNC;
		if (status & NPU_INT_PC)
			job->status.state = NPU_JOB_TEMP;
		if (status & NPU_INT_DONE)
			job->status.state = NPU_JOB_SUCCESS;
		if (status & NPU_INT_DFX) {
			/*
			 * A hardware error: keep the report, mask everything
			 * and leave the status latched for whoever looks next.
			 * The device stays aborted until NPU_IOC_RESET.
			 */
			job->status.state = NPU_JOB_ABORT;
			job->status.abort_value = npu_read(npu, NPU_DFX_RPT);
			npu_update(npu, NPU_INT_EN, NPU_INT_ALL, 0);
			npu->state = NPU_ABORTED;
			return IRQ_WAKE_THREAD;
		}
	}
	npu_clear_int(npu, status);
	return IRQ_WAKE_THREAD;
}

static irqreturn_t npu_irq_thread(int irq, void *data)
{
	struct npu_dev *npu = data;
	struct npu_job *job, *n;

	mutex_lock(&npu->state_lock);
	job = npu->cur;
	if (job) {
		switch (job->status.state) {
		case NPU_JOB_SUCCESS:
			npu_retire(npu, job);
			npu_start_next(npu);
			break;
		case NPU_JOB_ABORT:
			npu_retire(npu, job);
			npu->cur = NULL;
			/* nothing starts until reset: fail what was queued */
			list_for_each_entry_safe(job, n, &npu->pending, node) {
				list_del(&job->node);
				job->status.state = NPU_JOB_ABORT;
				list_add_tail(&job->node, &npu->done);
			}
			break;
		default:
			/* SI/PC stop: the job keeps the NPU, waiters see it */
			break;
		}
		npu_wake(npu);
	}
	mutex_unlock(&npu->state_lock);
	return IRQ_HANDLED;
}

/* Caller holds state_lock. Collects a finished job from the done list. */
static void npu_collect(struct npu_dev *npu, u32 id, struct npu_job_status *st)
{
	struct npu_job *job;

	list_for_each_entry(job, &npu->done, node) {
		if (job->status.job_id == id) {
			*st = job->status;
			list_del(&job->node);
			kfree(job);
			return;
		}
	}
	st->job_id = id;
	st->state = NPU_JOB_UNKNOWN;
}

/*
 * Caller holds state_lock. Fills *st and returns true if job @id has
 * something to report: finished (collected), stopped at SI/PC, or gone
 * (NPU_JOB_UNKNOWN: collected already, or dropped with its queue).
 * Returns false while it is still pending or running; st->state then
 * says which.
 */
static bool npu_job_status(struct npu_dev *npu, u32 id,
			   struct npu_job_status *st)
{
	struct npu_job *cur = npu->cur, *job;

	if (cur && !cur->orphan && cur->status.job_id == id) {
		if (cur->status.state == NPU_JOB_SYNC ||
		    cur->status.state == NPU_JOB_TEMP) {
			*st = cur->status;
			return true;
		}
		/* running, or finished with the IRQ thread still to move it */
		st->state = NPU_JOB_PROCESS;
		return false;
	}
	list_for_each_entry(job, &npu->pending, node) {
		if (job->status.job_id == id) {
			st->state = NPU_JOB_PEND;
			return false;
		}
	}
	npu_collect(npu, id, st);
	return true;
}

/*
 * Drops every job. One still running gets NPU_DESTROY_TIMEOUT to finish
 * first. Teardown does not stop the NPU, so a job that outlives that, or
 * sits at an SI/PC stop, keeps it: the device drains, taking no new work,
 * until the job ends or NPU_IOC_RESET resets the NPU. Either case, or an
 * abort while waiting, is reported as -EPERM. Waiters on dropped jobs see
 * NPU_JOB_UNKNOWN.
 */
static int npu_destroy_queue(struct npu_dev *npu)
{
	struct npu_job *job, *tmp;
	bool running;
	int ret = 0;

	mutex_lock(&npu->state_lock);
	list_for_each_entry_safe(job, tmp, &npu->pending, node) {
		list_del(&job->node);
		kfree(job);
	}
	running = npu->state == NPU_BUSY && npu->cur &&
		  npu->cur->status.state != NPU_JOB_SYNC &&
		  npu->cur->status.state != NPU_JOB_TEMP;
	mutex_unlock(&npu->state_lock);

	if (running) {
		wait_event_timeout(npu->wq, READ_ONCE(npu->state) != NPU_BUSY,
				   NPU_DESTROY_TIMEOUT);
		if (READ_ONCE(npu->state) == NPU_ABORTED)
			ret = -EPERM;
	}

	mutex_lock(&npu->state_lock);
	job = npu->cur;
	if (job && !job->orphan) {
		dev_warn(npu->dev, "job %u still owns the NPU, draining\n",
			 job->status.job_id);
		job->orphan = true;
		if (npu->state == NPU_BUSY)
			npu->state = NPU_DRAINING;
		ret = -EPERM;
	}
	list_for_each_entry_safe(job, tmp, &npu->done, node) {
		list_del(&job->node);
		kfree(job);
	}
	npu_wake(npu);
	mutex_unlock(&npu->state_lock);
	return ret;
}

static int npu_open_queue(struct npu_dev *npu)
{
	mutex_lock(&npu->queue_lock);
	if (npu->queues++ == 0) {
		mutex_lock(&npu->state_lock);
		npu->last_id = 0;
		mutex_unlock(&npu->state_lock);
	}
	mutex_unlock(&npu->queue_lock);
	return 0;
}

/* Caller holds queue_lock */
static int npu_drop_queues(struct npu_dev *npu)
{
	if (!npu->queues)
		return 0;
	npu->queues = 0;
	return npu_destroy_queue(npu);
}

static long npu_submit(struct npu_dev *npu, void __user *arg)
{
	struct npu_job *job;
	long ret;

	job = kzalloc(sizeof(*job), GFP_KERNEL);
	if (!job)
		return -EFAULT;
	if (copy_from_user(&job->params, arg, sizeof(job->params))) {
		kfree(job);
		return -EFAULT;
	}

	/* queue_lock keeps a destroy or reset from slipping in between */
	mutex_lock(&npu->queue_lock);
	mutex_lock(&npu->state_lock);
	if (!npu->queues || npu->state == NPU_ABORTED) {
		ret = -EFAULT;
	} else if (npu->state == NPU_DRAINING) {
		ret = -EBUSY;
	} else {
		/* the library takes the job id from the return value */
		ret = job->status.job_id = ++npu->last_id;
		if (npu->state == NPU_IDLE) {
			npu->state = NPU_BUSY;
			job->status.state = NPU_JOB_PROCESS;
			npu->cur = job;
			npu_hw_start(npu, &job->params);
		} else {
			job->status.state = NPU_JOB_PEND;
			list_add_tail(&job->node, &npu->pending);
		}
		job = NULL;
	}
	mutex_unlock(&npu->state_lock);
	mutex_unlock(&npu->queue_lock);

	kfree(job);
	return ret;
}

static long npu_query(struct npu_dev *npu, void __user *arg, bool wait)
{
	struct npu_job_status st;
	long timeout = MAX_SCHEDULE_TIMEOUT;
	unsigned int seen;
	bool ready;

	if (!npu->queues)
		return -EFAULT;
	if (copy_from_user(&st, arg, sizeof(st)))
		return -EFAULT;
	if (st.job_id > npu->last_id)
		return -EFAULT;
	if (wait && st.delay)
		timeout = msecs_to_jiffies(st.delay) ?: 1;

	for (;;) {
		mutex_lock(&npu->state_lock);
		ready = npu_job_status(npu, st.job_id, &st);
		seen = npu->events;
		mutex_unlock(&npu->state_lock);
		if (ready || !wait)
			break;

		timeout = wait_event_interruptible_timeout(npu->wq,
				READ_ONCE(npu->events) != seen, timeout);
		if (timeout <= 0)
			return -EAGAIN;
	}

	if (copy_to_user(arg, &st, sizeof(st)))
		return -EFAULT;
	return 0;
}

static long npu_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct npu_dev *npu = container_of(file->private_data,
					   struct npu_dev, misc);
	void __user *uarg = (void __user *)arg;
	struct npu_open_params open;
	struct npu_profiling prof;
	struct npu_job *job;
	long ret = 0;
	u32 val;

	switch (cmd) {
	case NPU_IOC_OPEN:
		mutex_lock(&npu->open_lock);
		if (npu->opened++ == 0) {
			if (copy_from_user(&open, uarg, sizeof(open))) {
				npu->opened--;
				ret = -EFAULT;
			} else {
				npu_hw_open(npu, open.base_addr, open.end_addr);
			}
		}
		mutex_unlock(&npu->open_lock);
		return ret;

	case NPU_IOC_CLOSE:
		mutex_lock(&npu->open_lock);
		if (npu->opened)
			npu->opened--;
		if (!npu->opened) {
			mutex_lock(&npu->queue_lock);
			npu_drop_queues(npu);
			mutex_unlock(&npu->queue_lock);
			npu_hw_close(npu);
		}
		mutex_unlock(&npu->open_lock);
		return 0;

	case NPU_IOC_CREATE_QUEUE:
		return npu_open_queue(npu);

	case NPU_IOC_DESTROY_QUEUE:
		mutex_lock(&npu->queue_lock);
		if (npu->queues)
			npu->queues--;
		if (!npu->queues)
			ret = npu_destroy_queue(npu);
		mutex_unlock(&npu->queue_lock);
		return ret;

	case NPU_IOC_RESET:
		mutex_lock(&npu->open_lock);
		mutex_lock(&npu->queue_lock);
		npu_drop_queues(npu);
		mutex_lock(&npu->state_lock);
		/*
		 * A job still here is draining, or aborted with the IRQ
		 * thread yet to see it; after a DFX abort the sequencer keeps
		 * running too. Either way, stop the NPU before reusing it,
		 * and if it will not stop, keep it out of service.
		 */
		job = npu->cur;
		if (job || npu->state == NPU_ABORTED) {
			ret = npu_soft_reset(npu);
			if (ret) {
				job = NULL;
			} else {
				if (job)
					dev_warn(npu->dev,
						 "reset stopped job %u\n",
						 job->status.job_id);
				npu->cur = NULL;
				/* clear the latched status, unmask what the
				 * IRQ masked */
				npu_clear_int(npu,
					      npu_read(npu, NPU_INT_STATUS));
				if (npu->opened)
					npu_update(npu, NPU_INT_EN,
						   NPU_INT_ALL, NPU_INT_ALL);
				npu->state = NPU_IDLE;
			}
		}
		mutex_unlock(&npu->state_lock);
		if (job) {
			/* the hard handler reads cur unlocked; the thread
			 * takes state_lock, so this waits outside it */
			synchronize_irq(npu->irq);
			kfree(job);
		}
		mutex_unlock(&npu->queue_lock);
		mutex_unlock(&npu->open_lock);
		return ret;

	case NPU_IOC_SUBMIT_JOB:
		return npu_submit(npu, uarg);

	case NPU_IOC_WAIT_JOB:
		return npu_query(npu, uarg, true);

	case NPU_IOC_QUERY_JOB:
		return npu_query(npu, uarg, false);

	case NPU_IOC_CLR_WFI:
		npu_update(npu, NPU_INT_CLR, NPU_CLR_WFI, NPU_CLR_WFI);
		return 0;

	case NPU_IOC_SET_PROFILING:
		if (copy_from_user(&prof, uarg, sizeof(prof)))
			return -EFAULT;
		npu_write(npu, NPU_STCT_ADDR, prof.stct_addr);
		npu_write(npu, NPU_STCT_SIZE, prof.stct_size);
		npu_update(npu, NPU_DEBUG, NPU_DEBUG_STCT_RPT,
			   NPU_DEBUG_STCT_RPT);
		return 0;

	case NPU_IOC_GET_CAPABILITY:
		/* NPU_VERSION_PERF/PIX/PLATFORM/SUBVERSION: one byte each */
		if (get_user(val, (u32 __user *)uarg))
			return -EFAULT;
		if (val > 3)
			return -EINVAL;
		return (npu_read(npu, NPU_VERSION) >> (24 - 8 * val)) & 0xff;

	case NPU_IOC_GET_MEMSIZE:
		if (!memsize)
			memsize = 0x400000;
		return put_user(memsize, (u32 __user *)uarg);

	case NPU_IOC_SET_MEMSIZE:
		return get_user(memsize, (u32 __user *)uarg);

	default:
		pr_err("%s: Unknown ioctl (cmd=%d)\n", __func__, cmd);
		return -EINVAL;
	}
}

static int npu_fop_open(struct inode *inode, struct file *file)
{
	struct npu_dev *npu = container_of(file->private_data,
					   struct npu_dev, misc);

	mutex_lock(&npu->ref_lock);
	if (npu->refs++ == 0)
		npu_clock(npu, true);
	mutex_unlock(&npu->ref_lock);
	return 0;
}

static int npu_fop_release(struct inode *inode, struct file *file)
{
	struct npu_dev *npu = container_of(file->private_data,
					   struct npu_dev, misc);

	mutex_lock(&npu->ref_lock);
	if (npu->refs)
		npu->refs--;
	if (!npu->refs) {
		/* last user gone: drop whatever it left behind */
		mutex_lock(&npu->queue_lock);
		npu_drop_queues(npu);
		mutex_unlock(&npu->queue_lock);
		mutex_lock(&npu->open_lock);
		if (npu->opened) {
			npu->opened = 0;
			npu_hw_close(npu);
		}
		mutex_unlock(&npu->open_lock);
		npu_clock(npu, false);
	}
	mutex_unlock(&npu->ref_lock);
	return 0;
}

/* The library maps the register page, uncached, and nothing else. */
static int npu_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct npu_dev *npu = container_of(file->private_data,
					   struct npu_dev, misc);
	unsigned long size = vma->vm_end - vma->vm_start;

	if (size != PAGE_SIZE)
		return -EINVAL;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	if (remap_pfn_range(vma, vma->vm_start, npu->regs_phys >> PAGE_SHIFT,
			    size, vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}

static const struct file_operations npu_fops = {
	.owner = THIS_MODULE,
	.open = npu_fop_open,
	.release = npu_fop_release,
	.unlocked_ioctl = npu_ioctl,
	.mmap = npu_mmap,
};

static int npu_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct npu_dev *npu;
	struct resource *res;
	int irq, ret;

	BUILD_BUG_ON(sizeof(struct npu_open_params) != 32);
	BUILD_BUG_ON(sizeof(struct npu_start_params) != 32);
	BUILD_BUG_ON(sizeof(struct npu_job_status) != 24);

	npu = devm_kzalloc(dev, sizeof(*npu), GFP_KERNEL);
	if (!npu)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	npu->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(npu->regs))
		return PTR_ERR(npu->regs);
	npu->regs_phys = res->start;
	npu->dev = dev;

	npu->clk_gate = devm_ioremap(dev, CRG_BASE + CRG_NPU_CLK, 4);
	if (!npu->clk_gate)
		return -ENOMEM;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;
	npu->irq = irq;

	mutex_init(&npu->ref_lock);
	mutex_init(&npu->open_lock);
	mutex_init(&npu->queue_lock);
	mutex_init(&npu->state_lock);
	INIT_LIST_HEAD(&npu->pending);
	INIT_LIST_HEAD(&npu->done);
	init_waitqueue_head(&npu->wq);
	npu->state = NPU_IDLE;

	ret = devm_request_threaded_irq(dev, irq, npu_irq, npu_irq_thread, 0,
					dev_name(dev), npu);
	if (ret) {
		dev_err(dev, "IRQ %d already claimed\n", irq);
		return ret;
	}

	npu->misc.minor = MISC_DYNAMIC_MINOR;
	npu->misc.name = "npu.0";
	npu->misc.fops = &npu_fops;
	npu->misc.parent = dev;
	ret = misc_register(&npu->misc);
	if (ret) {
		dev_err(dev, "misc_register failed\n");
		return ret;
	}
	platform_set_drvdata(pdev, npu);

	sysconfig_module_set_clk(NPU_MOD_ID, NPU_CLK_MHZ, NULL);
	dev_info(dev, "registers %pa, irq %d\n", &res->start, irq);
	return 0;
}

/*
 * Only reached on module unload (bind/unbind via sysfs is off), so no file
 * is open: every open one holds a module reference through npu_fops.owner.
 */
static int npu_remove(struct platform_device *pdev)
{
	struct npu_dev *npu = platform_get_drvdata(pdev);
	struct npu_job *job, *tmp;

	misc_deregister(&npu->misc);
	disable_irq(npu->irq);
	kfree(npu->cur);
	list_for_each_entry_safe(job, tmp, &npu->pending, node)
		kfree(job);
	list_for_each_entry_safe(job, tmp, &npu->done, node)
		kfree(job);
	return 0;
}

static const struct of_device_id npu_match[] = {
	{ .compatible = "xmedia,npu" },
	{ }
};
MODULE_DEVICE_TABLE(of, npu_match);

static struct platform_driver npu_driver = {
	.probe = npu_probe,
	.remove = npu_remove,
	.driver = {
		.name = "xmedia npu driver",
		.of_match_table = npu_match,
		.suppress_bind_attrs = true,
	},
};
module_platform_driver(npu_driver);

MODULE_DESCRIPTION("XMedia NPU driver");
MODULE_LICENSE("GPL v2");
