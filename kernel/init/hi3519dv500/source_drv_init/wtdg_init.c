/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include "ot_type.h"
#include "ot_osal.h"
#include "ot_wtdg.h"

#define OSDRV_MODULE_VERSION_STRING "OT_WTDG @OT_MPP"
static DEFINE_RWLOCK(feed_dog_lock);

#define CPU_MAX_COUNT        32
static int default_margin = OT_DOG_TIMER_MARGIN;
static int nodeamon = 0;
static int fiq_support = 0;
static struct hrtimer g_hrtimer;
static int g_feed_count = 0;
static int g_feed_max = 10;
static unsigned int g_interval_nsec;
static volatile unsigned char g_status = 0;
static struct task_struct *dog_threads[CPU_MAX_COUNT] = {0};
#define INTERVAL 100050000

#ifndef MODULE
osal_setup_num_param(fiq_support, fiq_support);
osal_setup_num_param(default_margin, default_margin);
osal_setup_num_param(nodeamon, nodeamon);
#else
module_param(fiq_support, int, S_IRUGO);

module_param(default_margin, int, 0);
MODULE_PARM_DESC(default_margin,
    "Watchdog default_margin in seconds. (0<default_margin<80, default=" __MODULE_STRING(OT_DOG_TIMER_MARGIN) ")");

module_param(nodeamon, int, 0);
MODULE_PARM_DESC(nodeamon,
    "By default, a kernel daemon feed watchdog when idle, set 'nodeamon=1' to disable this. (default=0)");
#endif


void set_hrtimer_interval(int dog_time)
{
    g_interval_nsec = dog_time * INTERVAL;
}

enum hrtimer_restart hrtimer_handler(struct hrtimer *ht)
{
    if (g_feed_count < g_feed_max) {
        write_lock(&feed_dog_lock);
        g_feed_count++;
        ot_dog_feed();

        write_unlock(&feed_dog_lock);
    }

    hrtimer_forward(ht, hrtimer_cb_get_time(ht), ktime_set(0, g_interval_nsec));

    return HRTIMER_RESTART;
}

void stop_hrtimer(void)
{
    int ret = hrtimer_cancel(&g_hrtimer);
    if (ret < 0) {
        printk(KERN_ERR "Failed to cancel hrtimer\n");
    }
}

void stop_dog_thread(void)
{
    int i;
    for (i = 0; i < nr_cpu_ids; i++) {
        kthread_stop(dog_threads[i]);
    }
}

void creat_dog_thread(int cpu_num)
{
    struct task_struct *dog = NULL;
    dog = kthread_create(ot_dog_deamon, NULL, "ot_dog_%d", cpu_num);
    if (dog != NULL) {
        dog_threads[cpu_num] = dog;
        kthread_bind(dog, cpu_num);
        wake_up_process(dog);
    } else {
        printk("create ot_dog_deamon on CPU %d failed!\n", cpu_num);
    }
}

void clear_feed_count_status(void)
{
    g_feed_count = 0;
    g_status = 0;
}

#define STATUS_OK 0x3
void fiq_dog_feed(void)
{
    uint64_t cpu;
    cpu = smp_processor_id();

    write_lock(&feed_dog_lock);
    g_status |= (1 << cpu);
    if (g_status == STATUS_OK) {
        clear_feed_count_status();
    }
    write_unlock(&feed_dog_lock);
    return;
}

static int ot_wdg_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;
    volatile void **ot_wtdg_reg_base = NULL;

    wtdg_set_module_param(default_margin, nodeamon, fiq_support);
    ot_wtdg_reg_base = get_wtdg_reg_base();
    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    *ot_wtdg_reg_base = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void*)*ot_wtdg_reg_base)) {
        return PTR_ERR((void*)*ot_wtdg_reg_base);
    }

    if (fiq_support) {
        set_hrtimer_interval(default_margin);
        hrtimer_init(&g_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        g_hrtimer.function = hrtimer_handler;
        hrtimer_start(&g_hrtimer, ktime_set(0, g_interval_nsec), HRTIMER_MODE_REL);
    }
    return watchdog_init();
}

static int ot_wdg_remove(struct platform_device *pdev)
{
    wtdg_unused(pdev);
    watchdog_exit();
    return 0;
}

static const struct of_device_id g_ot_wdg_match[] = {
    { .compatible = "vendor,wdg" },
    { },
};

static struct platform_driver g_ot_wdg_driver = {
    .probe  = ot_wdg_probe,
    .remove = ot_wdg_remove,
    .driver =  { .name = "ot_wdg",
                 .of_match_table = g_ot_wdg_match,
               },
};

module_platform_driver(g_ot_wdg_driver);
MODULE_LICENSE("GPL");

/*
 * Export symbol
 */
MODULE_DESCRIPTION("Mipi Driver");
MODULE_VERSION("OT_VERSION=" OSDRV_MODULE_VERSION_STRING);
