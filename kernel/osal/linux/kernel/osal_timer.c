/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/rtc.h>
#include "osal.h"

#include "../../../../compat/kernel_compat.h"

int osal_hrtimer_create(osal_hrtimer_t *phrtimer)
{
	return -1;
}
int osal_hrtimer_start(osal_hrtimer_t *phrtimer)
{
	return -1;
}
int osal_hrtimer_destory(osal_hrtimer_t *phrtimer)
{
	return -1;
}

/*
 * del_timer() returns as soon as the timer is off the list; it does not wait
 * for a callback already running on another CPU. Deleting and then freeing --
 * which osal_timer_destory() does -- therefore leaves that callback writing
 * into memory that has just gone away, and a callback that re-arms itself goes
 * on to schedule a freed timer. OpenIPC/openhisilicon#210.
 *
 * del_timer_sync() waits, but it cannot be used from interrupt context: it
 * spins for the handler, which deadlocks when the handler is on this CPU.
 * Destroying a timer from an interrupt cannot be made safe at all -- there is
 * no way to know the handler is not running -- so keep the old behaviour there
 * and name it, rather than trading silent corruption for a hang.
 *
 * del_timer_sync() on its own is still not enough for a callback that re-arms
 * itself: it waits for that callback, which has already put the timer back on
 * the list by the time it returns. The rtc temperature poll does exactly this
 * and ships as a blob, so it cannot be given a stop flag. Repeat until a pass
 * finds nothing pending -- at that point no handler is running and nothing is
 * queued -- and bound it, because a callback re-arming with no delay would
 * otherwise spin here forever.
 */
#define OSAL_TIMER_STOP_TRIES 5

/* 0 when the timer is off the list with no callback running, -1 when that could
 * not be established -- the caller must not free it in that case. */
static int osal_timer_stop(struct timer_list *t)
{
	int i;

	if (in_interrupt()) {
		osal_printk(
			"%s - called from interrupt, cannot wait for the timer callback\n",
			__FUNCTION__);
		del_timer(t);
		return -1;
	}

	for (i = 0; i < OSAL_TIMER_STOP_TRIES; i++) {
		if (!del_timer_sync(t))
			return 0;
	}

	osal_printk("%s - timer keeps re-arming itself, gave up after %d tries\n",
		    __FUNCTION__, OSAL_TIMER_STOP_TRIES);
	return -1;
}

/*
 * On kernels 4.15+, timer_setup() replaces init_timer() and the callback
 * signature changes from void(*)(unsigned long) to void(*)(struct timer_list*).
 *
 * We store the OSAL timer in a wrapper struct so the shim callback can
 * recover the original function pointer and data argument.
 */
#ifdef COMPAT_TIMER_SETUP

struct osal_timer_compat {
	struct timer_list tl;
	void (*function)(unsigned long);
	unsigned long data;
};

static void osal_timer_shim_callback(struct timer_list *t)
{
	struct osal_timer_compat *ot =
		container_of(t, struct osal_timer_compat, tl);
	if (ot->function)
		ot->function(ot->data);
}

int osal_timer_init(osal_timer_t *timer)
{
	struct osal_timer_compat *ot = NULL;

	if (timer == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}

	ot = kmalloc(sizeof(struct osal_timer_compat), GFP_KERNEL);
	if (ot == NULL) {
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}

	memset(ot, 0, sizeof(*ot));
	timer_setup(&ot->tl, osal_timer_shim_callback, 0);
	timer->timer = ot;
	return 0;
}
EXPORT_SYMBOL(osal_timer_init);

int osal_set_timer(osal_timer_t *timer, unsigned long interval)
{
	struct osal_timer_compat *ot = NULL;
	if ((timer == NULL) || (timer->timer == NULL) ||
	    (timer->function == NULL) || (interval == 0)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	ot = timer->timer;
	ot->function = timer->function;
	ot->data = timer->data;
	return mod_timer(&ot->tl, jiffies + msecs_to_jiffies(interval) - 1);
}
EXPORT_SYMBOL(osal_set_timer);

int osal_del_timer(osal_timer_t *timer)
{
	struct osal_timer_compat *ot = NULL;
	if ((timer == NULL) || (timer->timer == NULL) ||
	    (timer->function == NULL)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	ot = timer->timer;
	return del_timer(&ot->tl);
}
EXPORT_SYMBOL(osal_del_timer);

/* Callers legitimately use osal_del_timer() from an interrupt, and from inside
 * the timer's own callback, to mean "stop re-arming"; neither can wait. This is
 * the variant for callers in process context that need the callback finished
 * before they tear down what it touches. */
int osal_del_timer_sync(osal_timer_t *timer)
{
	struct osal_timer_compat *ot = NULL;
	if ((timer == NULL) || (timer->timer == NULL) ||
	    (timer->function == NULL)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	ot = timer->timer;
	return osal_timer_stop(&ot->tl);
}
EXPORT_SYMBOL(osal_del_timer_sync);

int osal_timer_destory(osal_timer_t *timer)
{
	struct osal_timer_compat *ot = timer->timer;

	if (osal_timer_stop(&ot->tl)) {
		/* Leaking the wrapper is the lesser fault: freeing one that is
		 * still queued, or whose callback may still be running, is the
		 * use-after-free this exists to prevent. */
		osal_printk("%s - timer still live, leaked rather than freed\n",
			    __FUNCTION__);
		return -1;
	}

	kfree(ot);
	timer->timer = NULL;
	return 0;
}
EXPORT_SYMBOL(osal_timer_destory);

#else /* pre-4.15: original init_timer API */

int osal_timer_init(osal_timer_t *timer)
{
	struct timer_list *t = NULL;

	if (timer == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}

	t = (struct timer_list *)kmalloc(sizeof(struct timer_list), GFP_KERNEL);
	if (t == NULL) {
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}

	init_timer(t);
	timer->timer = t;
	return 0;
}
EXPORT_SYMBOL(osal_timer_init);

int osal_set_timer(osal_timer_t *timer, unsigned long interval)
{
	struct timer_list *t = NULL;
	if ((timer == NULL) || (timer->timer == NULL) ||
	    (timer->function == NULL) || (interval == 0)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	t = timer->timer;
	t->function = timer->function;
	t->data = timer->data;
	return mod_timer(t, jiffies + msecs_to_jiffies(interval) - 1);
}
EXPORT_SYMBOL(osal_set_timer);

int osal_del_timer(osal_timer_t *timer)
{
	struct timer_list *t = NULL;
	if ((timer == NULL) || (timer->timer == NULL) ||
	    (timer->function == NULL)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	t = timer->timer;
	return del_timer(t);
}
EXPORT_SYMBOL(osal_del_timer);

/* See the COMPAT_TIMER_SETUP branch above. */
int osal_del_timer_sync(osal_timer_t *timer)
{
	struct timer_list *t = NULL;
	if ((timer == NULL) || (timer->timer == NULL) ||
	    (timer->function == NULL)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	t = timer->timer;
	return osal_timer_stop(t);
}
EXPORT_SYMBOL(osal_del_timer_sync);

int osal_timer_destory(osal_timer_t *timer)
{
	struct timer_list *t = timer->timer;

	if (osal_timer_stop(t)) {
		/* See the COMPAT_TIMER_SETUP branch above. */
		osal_printk("%s - timer still live, leaked rather than freed\n",
			    __FUNCTION__);
		return -1;
	}

	kfree(t);
	timer->timer = NULL;
	return 0;
}
EXPORT_SYMBOL(osal_timer_destory);

#endif /* COMPAT_TIMER_SETUP */

unsigned long osal_msleep(unsigned int msecs)
{
	return msleep_interruptible(msecs);
}
EXPORT_SYMBOL(osal_msleep);

void osal_udelay(unsigned int usecs)
{
	udelay(usecs);
}
EXPORT_SYMBOL(osal_udelay);

void osal_mdelay(unsigned int msecs)
{
	mdelay(msecs);
}
EXPORT_SYMBOL(osal_mdelay);

unsigned int osal_get_tickcount()
{
	return jiffies_to_msecs(jiffies);
}
EXPORT_SYMBOL(osal_get_tickcount);

unsigned long long osal_sched_clock()
{
	return sched_clock();
}
EXPORT_SYMBOL(osal_sched_clock);

void osal_gettimeofday(osal_timeval_t *tv)
{
	if (tv == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
#ifdef COMPAT_NO_DO_GETTIMEOFDAY
	{
		struct timespec64 ts;
		ktime_get_real_ts64(&ts);
		tv->tv_sec = ts.tv_sec;
		tv->tv_usec = ts.tv_nsec / 1000;
	}
#else
	{
		struct timeval t;
		do_gettimeofday(&t);
		tv->tv_sec = t.tv_sec;
		tv->tv_usec = t.tv_usec;
	}
#endif
}
EXPORT_SYMBOL(osal_gettimeofday);

void osal_rtc_time_to_tm(unsigned long time, osal_rtc_time_t *tm)
{
	struct rtc_time _tm = { 0 };

	compat_rtc_time_to_tm(time, &_tm);

	tm->tm_sec = _tm.tm_sec;
	tm->tm_min = _tm.tm_min;
	tm->tm_hour = _tm.tm_hour;
	tm->tm_mday = _tm.tm_mday;
	tm->tm_mon = _tm.tm_mon;
	tm->tm_year = _tm.tm_year;
	tm->tm_wday = _tm.tm_wday;
	tm->tm_yday = _tm.tm_yday;
	tm->tm_isdst = _tm.tm_isdst;
}
EXPORT_SYMBOL(osal_rtc_time_to_tm);

void osal_rtc_tm_to_time(osal_rtc_time_t *tm, unsigned long *time)
{
	struct rtc_time _tm;
	_tm.tm_sec = tm->tm_sec;
	_tm.tm_min = tm->tm_min;
	_tm.tm_hour = tm->tm_hour;
	_tm.tm_mday = tm->tm_mday;
	_tm.tm_mon = tm->tm_mon;
	_tm.tm_year = tm->tm_year;
	_tm.tm_wday = tm->tm_wday;
	_tm.tm_yday = tm->tm_yday;
	_tm.tm_isdst = tm->tm_isdst;

	compat_rtc_tm_to_time(&_tm, time);
}
EXPORT_SYMBOL(osal_rtc_tm_to_time);

void osal_getjiffies(unsigned long long *pjiffies)
{
	*pjiffies = jiffies;
}
EXPORT_SYMBOL(osal_getjiffies);

int osal_rtc_valid_tm(struct osal_rtc_time *tm)
{
	struct rtc_time _tm;
	_tm.tm_sec = tm->tm_sec;
	_tm.tm_min = tm->tm_min;
	_tm.tm_hour = tm->tm_hour;
	_tm.tm_mday = tm->tm_mday;
	_tm.tm_mon = tm->tm_mon;
	_tm.tm_year = tm->tm_year;
	_tm.tm_wday = tm->tm_wday;
	_tm.tm_yday = tm->tm_yday;
	_tm.tm_isdst = tm->tm_isdst;

	return rtc_valid_tm(&_tm);
}
EXPORT_SYMBOL(osal_rtc_valid_tm);
