/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_vsync.h"
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/fb.h>
#include <asm/uaccess.h>
#include <asm/types.h>
#include <asm/stat.h>
#include <asm/fcntl.h>
#include <linux/interrupt.h>
#include <linux/spinlock_types.h>
#include <linux/seq_file.h>
#include "sys_ext.h"

#define VSYNC_LEN 64
static unsigned int g_vsync_major = 288;
static unsigned int g_vsync_minor = 0;
static dev_t g_vsync_devno;
static struct class *g_vsync_cls;
static struct device *g_vsync_device;

static unsigned int g_pts_major = 289;
static unsigned int g_pts_minor = 0;
static dev_t g_pts_devno;
static struct class *g_pts_cls;
static struct device *g_pts_device;

static char g_vdata[VSYNC_LEN] = {0};
static char g_nowdata[VSYNC_LEN] = {0};

static spinlock_t g_vsync_lock;
static wait_queue_head_t g_wait;

static unsigned long long int g_timestamp;
static bool g_bnewdata = 0;

static int vsync_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int vsync_release(struct inode *inode, struct file *file)
{
    return 0;
}

static int vsync_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    unsigned int len;
    unsigned long syncflags = 0;

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    osal_memset(g_vdata, 0, VSYNC_LEN);
    osal_snprintf(g_vdata, VSYNC_LEN, "%llu\n", g_timestamp);
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    len = strnlen(g_vdata, (VSYNC_LEN - 1)) + 1;
    if ((buf == HI_NULL) || (count < len)) {
        return 0;
    }

    if (copy_to_user(buf, g_vdata, len)) {
        printk("copy to user err\n");
        len = 0;
    }

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    g_bnewdata = 0;
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    return len;
}

static unsigned int vsync_poll(struct file *file, struct poll_table_struct *table)
{
    unsigned int mask = 0;
    unsigned long syncflags = 0;

    poll_wait(file, &g_wait, table);

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    if (g_bnewdata) {
        mask |= POLLIN | POLLRDNORM;
    }
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    return mask;
}

static struct file_operations g_vsync_ops = {
    .owner = THIS_MODULE,
    .open = vsync_open,
    .read = (hi_void *)vsync_read,
    .release = vsync_release,
    .poll = vsync_poll,
};

static unsigned long long int inline get_cur_pts(void)
{
    unsigned long long int time_now;
    time_now = osal_sched_clock();
    do_div(time_now, 1000); /* 1000 alg data */

    return time_now;
}

int hifb_vsync_notify(void)
{
    unsigned long syncflags = 0;

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    g_timestamp = get_cur_pts();
    g_bnewdata = 1;
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    wake_up_interruptible(&g_wait);

    return 0;
}

int hifb_vsync_init(void)
{
    int ret;
    g_vsync_devno = MKDEV(g_vsync_major, g_vsync_minor);
    ret = register_chrdev(g_vsync_major, "vsync", &g_vsync_ops);
    if (ret < 0) {
        printk("Unable to register characterdevice!\n");
        return ret;
    }
    g_vsync_cls = class_create(THIS_MODULE, "vsync");
    if (IS_ERR(g_vsync_cls)) {
        unregister_chrdev(g_vsync_major, "vsync");
        return -EBUSY;
    }

    g_vsync_device = device_create(g_vsync_cls, HI_NULL, g_vsync_devno, HI_NULL, "vsync"); /* mknod /dev/vsync */
    if (IS_ERR(g_vsync_device)) {
        class_destroy(g_vsync_cls);
        unregister_chrdev(g_vsync_major, "vsync");
        return -EBUSY;
    }

    init_waitqueue_head(&g_wait);
    spin_lock_init(&g_vsync_lock);

    return 0;
}

void hifb_vsync_exit(void)
{
    device_destroy(g_vsync_cls, g_vsync_devno);
    class_destroy(g_vsync_cls);
    unregister_chrdev(g_vsync_major, "vsync");
    printk("hi_vsync_exit \n");
}

static int pts_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    unsigned long long int time_now;
    unsigned int len;
    time_now = get_cur_pts();

    osal_memset(g_nowdata, 0, VSYNC_LEN);
    osal_snprintf(g_nowdata, VSYNC_LEN, "%llu\n", time_now);
    len = strnlen(g_nowdata, (VSYNC_LEN - 1)) + 1;
    if ((buf == HI_NULL) || (count < len)) {
        return 0;
    }

    if (copy_to_user(buf, g_nowdata, len)) {
        printk("copy to user err\n");
        len = 0;
    }

    return len;
}

static struct file_operations g_pts_ops = {
    .owner = THIS_MODULE,
    .read = (hi_void *)pts_read,
};

int hifb_pts_init(void)
{
    int ret;
    g_pts_devno = MKDEV(g_pts_major, g_pts_minor);
    ret = register_chrdev(g_pts_major, "vsync_pts", &g_pts_ops);
    if (ret < 0) {
        printk("Unable to register characterdevice!\n");
        return ret;
    }

    g_pts_cls = class_create(THIS_MODULE, "vsync_pts");
    if (IS_ERR(g_pts_cls)) {
        unregister_chrdev(g_pts_major, "vsync_pts");
        return -EBUSY;
    }

    g_pts_device = device_create(g_pts_cls, HI_NULL, g_pts_devno, HI_NULL, "vsync_pts"); /* mknod /dev/vsync_pts */
    if (IS_ERR(g_pts_device)) {
        class_destroy(g_pts_cls);
        unregister_chrdev(g_pts_major, "vsync_pts");
        return -EBUSY;
    }

    return 0;
}

void hifb_pts_exit(void)
{
    device_destroy(g_pts_cls, g_pts_devno);
    class_destroy(g_pts_cls);
    unregister_chrdev(g_pts_major, "vsync_pts");
    printk("hi_pts_exit \n");
}
