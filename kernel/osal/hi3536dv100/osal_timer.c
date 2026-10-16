#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/rtc.h>
#include "hi_osal.h"


int osal_timer_init(osal_timer_t *timer)
{
    struct timer_list *t = NULL;

    if(timer == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    t = (struct timer_list *)kmalloc(sizeof(struct timer_list), GFP_KERNEL);
    if(t == NULL){
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
    if(timer == NULL || timer->timer == NULL || timer->function == NULL || interval == 0){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    t = timer->timer;
    t->function = timer->function;
    t->data = timer->data;
    return mod_timer(t, jiffies + msecs_to_jiffies(interval));
}
EXPORT_SYMBOL(osal_set_timer);

/*
 * del_timer() returns as soon as the timer is off the list; it does not wait
 * for a callback already running on another CPU, so deleting and then freeing
 * leaves that callback writing into freed memory, and a callback that re-arms
 * itself schedules a freed timer. Same fix as the V4 OSAL (#211): wait with
 * del_timer_sync(), repeated for callbacks that re-arm themselves, and never
 * from interrupt context, where waiting for the handler can deadlock.
 */
#define OSAL_TIMER_STOP_TRIES 5

/* 0 when the timer is off the list with no callback running, -1 when that
 * could not be established -- the caller must not free it in that case. */
static int osal_timer_stop(struct timer_list *t)
{
    int i;

    if (in_interrupt()) {
        osal_printk("%s - called from interrupt, cannot wait for the timer callback\n",
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

int osal_del_timer(osal_timer_t *timer)
{
    struct timer_list *t = NULL;
    if(timer == NULL || timer->timer == NULL || timer->function == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    t = timer->timer;
    return del_timer(t);
}
EXPORT_SYMBOL(osal_del_timer);

int osal_timer_destory(osal_timer_t *timer)
{
    struct timer_list *t = NULL;
    if(timer == NULL || timer->timer == NULL){
        osal_printk("%s - timer->timer is NULL!\n", __FUNCTION__);
        return -1;
    }
    t = timer->timer;
    if (osal_timer_stop(t)) {
        /* Leaking the timer is the lesser fault: freeing one that is still
         * queued, or whose callback may still be running, is the
         * use-after-free this exists to prevent. */
        osal_printk("%s - timer still live, leaked rather than freed\n", __FUNCTION__);
        return -1;
    }
    kfree(t);
    timer->timer=NULL;
    return 0;
}
EXPORT_SYMBOL(osal_timer_destory);

unsigned long osal_msleep(unsigned int msecs)
{
    return  msleep_interruptible(msecs);
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
    struct timeval t;
    if(tv == NULL){
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    do_gettimeofday(&t);

    tv->tv_sec = t.tv_sec;
    tv->tv_usec = t.tv_usec;
}
EXPORT_SYMBOL(osal_gettimeofday);

void osal_rtc_time_to_tm(unsigned long time, osal_rtc_time_t *tm)
{
    struct rtc_time _tm;

    rtc_time_to_tm(time, &_tm);

    tm->tm_sec   = _tm.tm_sec;
    tm->tm_min   = _tm.tm_min;
    tm->tm_hour  = _tm.tm_hour;
    tm->tm_mday  = _tm.tm_mday;
    tm->tm_mon   = _tm.tm_mon;
    tm->tm_year  = _tm.tm_year;
    tm->tm_wday  = _tm.tm_wday;
    tm->tm_yday  = _tm.tm_yday;
    tm->tm_isdst = _tm.tm_isdst;
}
EXPORT_SYMBOL(osal_rtc_time_to_tm);

void osal_rtc_tm_to_time(osal_rtc_time_t *tm ,unsigned long *time)
{
    struct rtc_time _tm;
    _tm.tm_sec  = tm->tm_sec;
    _tm.tm_min  = tm->tm_min;
    _tm.tm_hour = tm->tm_hour;
    _tm.tm_mday = tm->tm_mday;
    _tm.tm_mon  = tm->tm_mon;
    _tm.tm_year = tm->tm_year;
    _tm.tm_wday = tm->tm_wday;
    _tm.tm_yday = tm->tm_yday;
    _tm.tm_isdst = tm->tm_isdst;

    rtc_tm_to_time(&_tm, time);
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
    _tm.tm_sec  = tm->tm_sec;
    _tm.tm_min  = tm->tm_min;
    _tm.tm_hour = tm->tm_hour;
    _tm.tm_mday = tm->tm_mday;
    _tm.tm_mon  = tm->tm_mon;
    _tm.tm_year = tm->tm_year;
    _tm.tm_wday = tm->tm_wday;
    _tm.tm_yday = tm->tm_yday;
    _tm.tm_isdst = tm->tm_isdst;

    return rtc_valid_tm(&_tm);
}
EXPORT_SYMBOL(osal_rtc_valid_tm);

int osal_is_enable_hrtimer(void)
{
    int is_enable_hrtimer;

#ifdef CONFIG_HIGH_RES_TIMERS
    is_enable_hrtimer = 1;
#else
    is_enable_hrtimer = 0;
#endif
    return is_enable_hrtimer;
}
EXPORT_SYMBOL(osal_is_enable_hrtimer);
