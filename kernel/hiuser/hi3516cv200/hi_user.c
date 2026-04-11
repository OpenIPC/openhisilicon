/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>


int hi_sched_setscheduler(struct task_struct *p)
{
#if 0
    struct sched_param param;
    param.sched_priority = 99;

    return sched_setscheduler(p, SCHED_RR, &param);
#else

	return 0;
#endif
}
EXPORT_SYMBOL(hi_sched_setscheduler);



static int __init hi_user_init(void)
{
    printk(KERN_INFO "load hiuser.ko ...OK!\n");
	return 0;
}

static void __exit hi_user_exit(void)
{
    printk(KERN_INFO "unload hiuser.ko ...OK!\n");
	return;
}

module_init(hi_user_init);
module_exit(hi_user_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");

