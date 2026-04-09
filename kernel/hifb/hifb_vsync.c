/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

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


#include "hifb_vsync.h"

#define vsync_len 64
static unsigned int vsync_major = 288;
static unsigned int vsync_minor = 0;
static dev_t vsync_devno;
static struct class* vsync_cls;
static struct device* vsync_device;

static unsigned int pts_major = 289;
static unsigned int pts_minor = 0;
static dev_t pts_devno;
static struct class* pts_cls;
static struct device* pts_device;

static char vdata[vsync_len] = {0};
static char nowdata[vsync_len] = {0};

static spinlock_t vsync_lock;
static wait_queue_head_t wait;

static unsigned long long int g_timestamp;
static bool bnewdata = 0;

static int vsync_open (struct inode* inode, struct file* file)
{
    return 0;
}

static int vsync_release (struct inode* inode, struct file* file)
{
    return 0;
}

static int vsync_read (struct file* file, char __user* buf, size_t count, loff_t* offset)
{
    unsigned int len = 0;
    unsigned long  syncflags = 0;

    spin_lock_irqsave(&vsync_lock, syncflags);
    osal_memset(vdata, 0, vsync_len);
    osal_snprintf(vdata, vsync_len, "%llu\n", g_timestamp);
    spin_unlock_irqrestore(&vsync_lock, syncflags);

    len = strnlen(vdata, (vsync_len-1)) + 1;
    if ((buf == NULL) || (count < len)) {
        return 0;
    }

    if (copy_to_user(buf, vdata, len))
    {
        printk("copy to user err\n");
        len = 0;
    }

    spin_lock_irqsave(&vsync_lock, syncflags);
    bnewdata = 0;
    spin_unlock_irqrestore(&vsync_lock, syncflags);

    return len;
}

static unsigned int vsync_poll (struct file* file, struct poll_table_struct* table)
{
    unsigned int  mask = 0;
    unsigned long syncflags = 0;

    poll_wait(file, &wait, table);

    spin_lock_irqsave(&vsync_lock, syncflags);
    if (bnewdata)
    {
        mask |= POLLIN | POLLRDNORM;
    }
    spin_unlock_irqrestore(&vsync_lock, syncflags);

    return mask;
}

static struct file_operations vsync_ops =
{
    .owner = THIS_MODULE,
    .open = vsync_open,
    .read = (HI_VOID *)vsync_read,
    .release = vsync_release,
    .poll = vsync_poll,
};

static unsigned long long int inline get_cur_pts(void)
{
    unsigned long long int u64TimeNow;

    u64TimeNow = osal_sched_clock();
	do_div(u64TimeNow, 1000);

    return u64TimeNow;
}

int hifb_vsync_notify(void)
{
    unsigned long  syncflags = 0;

    spin_lock_irqsave(&vsync_lock, syncflags);
    g_timestamp = get_cur_pts();
    bnewdata    = 1;
    spin_unlock_irqrestore(&vsync_lock, syncflags);

    wake_up_interruptible(&wait);

    return 0;
}

int hifb_vsync_init(void)
{
    int ret;

    vsync_devno = MKDEV(vsync_major, vsync_minor);
    ret = register_chrdev(vsync_major, "vsync", &vsync_ops);
    if(ret < 0)
    {
        printk("Unable to register characterdevice!\n");
        return ret;
    }
    vsync_cls = class_create(THIS_MODULE, "vsync");

    if (IS_ERR(vsync_cls))
    {
        unregister_chrdev(vsync_major, "vsync");
        return -EBUSY;
    }

    vsync_device = device_create(vsync_cls, NULL, vsync_devno, NULL, "vsync"); //mknod /dev/vsync

    if (IS_ERR(vsync_device))
    {
        class_destroy(vsync_cls);
        unregister_chrdev(vsync_major, "vsync");
        return -EBUSY;
    }

    init_waitqueue_head(&wait);
    spin_lock_init(&vsync_lock);

    return 0;
}

void hifb_vsync_exit(void)
{
    device_destroy(vsync_cls, vsync_devno);
    class_destroy(vsync_cls);
    unregister_chrdev(vsync_major, "vsync");
    printk("hi_vsync_exit \n");
}

static int pts_read (struct file* file, char __user* buf, size_t count, loff_t* offset)
{
    unsigned long long int u64TimeNow = 0;
    unsigned int len = 0;
    u64TimeNow = get_cur_pts();

    osal_memset(nowdata, 0, vsync_len);
    osal_snprintf(nowdata, vsync_len, "%llu\n", u64TimeNow);
    len = strnlen(nowdata, (vsync_len-1)) + 1;
    if ((buf == NULL) || (count < len)) {
        return 0;
    }

    if (copy_to_user(buf, nowdata, len))
    {
        printk("copy to user err\n");
        len = 0;
    }

    return len;
}


static struct file_operations pts_ops =
{
    .owner = THIS_MODULE,
    .read = (HI_VOID *)pts_read,
};

int hifb_pts_init(void)
{
    int ret;

    pts_devno = MKDEV(pts_major, pts_minor);
    ret = register_chrdev(pts_major, "vsync_pts", &pts_ops);
    if(ret < 0)
    {
        printk("Unable to register characterdevice!\n");
        return ret;
    }

    pts_cls = class_create(THIS_MODULE, "vsync_pts");

    if (IS_ERR(pts_cls))
    {
        unregister_chrdev(pts_major, "vsync_pts");
        return -EBUSY;
    }

    pts_device = device_create(pts_cls, NULL, pts_devno, NULL, "vsync_pts"); //mknod /dev/vsync_pts

    if (IS_ERR(pts_device))
    {
        class_destroy(pts_cls);
        unregister_chrdev(pts_major, "vsync_pts");
        return -EBUSY;
    }

    return 0;
}

void hifb_pts_exit(void)
{
    device_destroy(pts_cls, pts_devno);
    class_destroy(pts_cls);
    unregister_chrdev(pts_major, "vsync_pts");
    printk("hi_pts_exit \n");
}
