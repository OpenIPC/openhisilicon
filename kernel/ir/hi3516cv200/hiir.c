/* interface/hiir/hiir.c
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * History:
 *      16-12-2006 Start of Hi3560 CPU support
 */


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/sched.h>
//#include <asm/hardware.h>
#include <linux/interrupt.h>

#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/errno.h>
#include <linux/fcntl.h>

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/version.h>
#include <linux/platform_device.h>


#include "hiir.h"
#include "hiir_codedef.h"

/* irq */

#define HIIR_DEVICE_IRQ_NO 19

static unsigned int ir_reg_base;
static int ir_irq = -1;

/* define IR IO */
#define IR_REG_BASE ir_reg_base
#define IR_ENABLE   (IR_REG_BASE + 0x00)
#define IR_CONFIG   (IR_REG_BASE + 0x04)
#define CNT_LEADS   (IR_REG_BASE + 0x08)
#define CNT_LEADE   (IR_REG_BASE + 0x0c)
#define CNT_SLEADE  (IR_REG_BASE + 0x10)
#define CNT0_B      (IR_REG_BASE + 0x14)
#define CNT1_B      (IR_REG_BASE + 0x18)
#define IR_BUSY     (IR_REG_BASE + 0x1c)
#define IR_DATAH    (IR_REG_BASE + 0x20)
#define IR_DATAL    (IR_REG_BASE + 0x24)
#define IR_INTM     (IR_REG_BASE + 0x28)
#define IR_INTS     (IR_REG_BASE + 0x2c)
#define IR_INTC     (IR_REG_BASE + 0x30)
#define IR_START    (IR_REG_BASE + 0x34)


/* interrupt mask */
#define INTMS_RCV      (1L << 16)
#define INTMS_FRAMERR  (1L << 17)
#define INTMS_OVERFLOW (1L << 18)
#define INTMS_RELEASE  (1L << 19)

/* define macro */
#define WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

/* debug */
static int g_dbg_flag = 0;
#define HIIR_PFX "hiir: "
#define hiir_dbg(params...) \
    do{ \
        if(g_dbg_flag) \
        { \
            printk(KERN_DEBUG HIIR_PFX params); \
        } \
    }while(0);
#define hiir_show_reg() \
    do{ \
        if(g_dbg_flag) \
        { \
            printk(KERN_DEBUG "HIIR11 REG:###################################################\n"); \
            printk(KERN_DEBUG "%.8x ", READ_REG(IR_ENABLE)); \
            printk("%.8x ", READ_REG(IR_CONFIG)); \
            printk("%.8x ", READ_REG(CNT_LEADS)); \
            printk("%.8x ", READ_REG(CNT_LEADE)); \
            printk("%.8x ", READ_REG(CNT_SLEADE)); \
            printk("%.8x ", READ_REG(CNT0_B)); \
            printk("%.8x ", READ_REG(CNT1_B)); \
            printk("\n"); \
            printk(KERN_DEBUG "%.8x ", READ_REG(IR_BUSY)); \
            printk("%.8x ", READ_REG(IR_DATAH)); \
            printk("%.8x ", READ_REG(IR_DATAL)); \
            printk("%.8x ", READ_REG(IR_INTM)); \
            printk("%.8x ", READ_REG(IR_INTS)); \
            printk("%.8x ", READ_REG(IR_INTC)); \
            printk("%.8x ", READ_REG(IR_START)); \
            printk("\n"); \
        } \
    }while(0);

#define DEFAULT_DELAYTIME 200

#define HI_IR_MAX_BUF 100
#define DEFAULT_BUF_LEN 8
#define BUF_HEAD hiir_dev.buf[hiir_dev.head]
#define BUF_TAIL hiir_dev.buf[hiir_dev.tail]
#define BUF_LAST hiir_dev.buf[(hiir_dev.head == 0)? (hiir_dev.buf_len - 1): (hiir_dev.head - 1)]
#define INC_BUF(x,len) 	({\
        unsigned int __ret = 0;\
        ++(x);\
        __ret=(x)%(len);\
        __ret;\
    })

typedef struct
{
    hiir_dev_param dev_parm;
    unsigned int head;
    unsigned int tail;
    irkey_info_s buf[HI_IR_MAX_BUF + 1];
    unsigned int buf_len;

    unsigned int enable_repkey;
    unsigned int repkey_checkflag;
    unsigned int repkey_delaytime;

    unsigned int enable_keyup;

    wait_queue_head_t irkey_wait;
}hiir_dev_struct;

static hiir_dev_struct hiir_dev;

static void repkey_timeout_handler(unsigned long data);
static DEFINE_TIMER(repkey_timeout_timer, repkey_timeout_handler, 0, 0);

static void hiir_config(void)
{
    int value = 0;

/* TODO: ASIC config pin share */
#if 0
    value = READ_REG(IOCONFIG);
    value = 0;
    WRITE_REG(IOCONFIG, value);
#endif

    WRITE_REG(IR_ENABLE, 0x01);
    while(READ_REG(IR_BUSY))
    {
        hiir_dbg("IR_BUSY. Wait...\n");
        //udelay(1);
    }

    value = (hiir_dev.dev_parm.codetype << 14);
    value |= (hiir_dev.dev_parm.code_len - 1) << 8;
    value |= (hiir_dev.dev_parm.frequence - 1);
    WRITE_REG(IR_CONFIG, value);

    value = hiir_dev.dev_parm.leads_min << 16;
    value |= hiir_dev.dev_parm.leads_max;
    WRITE_REG(CNT_LEADS, value);

    value = hiir_dev.dev_parm.leade_min << 16;
    value |= hiir_dev.dev_parm.leade_max;
    WRITE_REG(CNT_LEADE, value);

    value = hiir_dev.dev_parm.sleade_min << 16;
    value |= hiir_dev.dev_parm.sleade_max;
    WRITE_REG(CNT_SLEADE, value);

    value = hiir_dev.dev_parm.cnt0_b_min << 16;
    value |= hiir_dev.dev_parm.cnt0_b_max;
    WRITE_REG(CNT0_B, value);

    value = hiir_dev.dev_parm.cnt1_b_min << 16;
    value |= hiir_dev.dev_parm.cnt1_b_max;
    WRITE_REG(CNT1_B, value);

    WRITE_REG(IR_INTM, 0x00);
    WRITE_REG(IR_START, 0x00);

    //hiir_dbg("current config is...\n");
    //hiir_show_reg();
}

static void repkey_timeout_handler(unsigned long data)
{
    del_timer(&repkey_timeout_timer);
    hiir_dev.repkey_checkflag = 0;
}

static irqreturn_t hiir_interrupt(int irq, void *dev_id)
{
    hiir_dbg("Enter hiir_interrupt.\n");
    hiir_show_reg();

    if(READ_REG(IR_INTS) & INTMS_FRAMERR)
    {
        hiir_dbg("frame error.\n");
        WRITE_REG(IR_INTC, 0x01<<1);
    }
    else if(READ_REG(IR_INTS) & INTMS_OVERFLOW)
    {
        hiir_dbg("hardware overflow.\n");
        WRITE_REG(IR_INTC, 0x01<<2);
    }
    else if(READ_REG(IR_INTS) & INTMS_RELEASE)
    {
        hiir_dbg("release key interrupt.\n");
        WRITE_REG(IR_INTC, 0x01<<3);

        del_timer(&repkey_timeout_timer);
        hiir_dev.repkey_checkflag = 0;

        if( (hiir_dev.enable_keyup) && (HIIR_KEY_UP != BUF_LAST.irkey_state_code) )
        {
            BUF_HEAD = BUF_LAST;
            BUF_HEAD.irkey_state_code = HIIR_KEY_UP;
            hiir_dev.head = INC_BUF(hiir_dev.head, hiir_dev.buf_len);
            wake_up_interruptible(&(hiir_dev.irkey_wait));
        }
    }
    else if(READ_REG(IR_INTS) & INTMS_RCV)
    {
        hiir_dbg("receive data interrupt. 0x%.8x-0x%.8x\n", READ_REG(IR_DATAH), READ_REG(IR_DATAL));
        WRITE_REG(IR_INTC, 0x01<<0);

        if(hiir_dev.enable_repkey)
        {
            if( (hiir_dev.repkey_checkflag) &&
                (BUF_LAST.irkey_datah == READ_REG(IR_DATAH)) &&
                (BUF_LAST.irkey_datal == READ_REG(IR_DATAL)) )
            {
                hiir_dbg("repeart key [0x%.8x]-[0x%.8x] detective\n", READ_REG(IR_DATAH), READ_REG(IR_DATAL));
            }
            else
            {
                /* repeat key check */
                del_timer(&repkey_timeout_timer);
                repkey_timeout_timer.expires = jiffies + hiir_dev.repkey_delaytime * msecs_to_jiffies(1000) / 1000;
                add_timer(&repkey_timeout_timer);
                hiir_dev.repkey_checkflag = 1;
                BUF_HEAD.irkey_datah = READ_REG(IR_DATAH);
                BUF_HEAD.irkey_datal = READ_REG(IR_DATAL);
                BUF_HEAD.irkey_state_code = HIIR_KEY_DOWN;
                hiir_dev.head = INC_BUF(hiir_dev.head, hiir_dev.buf_len);
                wake_up_interruptible(&(hiir_dev.irkey_wait));
            }
        }
	else if( !((BUF_LAST.irkey_datah == READ_REG(IR_DATAH))
			&& (BUF_LAST.irkey_datal == READ_REG(IR_DATAL))
                    && (BUF_LAST.irkey_state_code == HIIR_KEY_DOWN) ) )
        {
            BUF_HEAD.irkey_datah = READ_REG(IR_DATAH);
            BUF_HEAD.irkey_datal = READ_REG(IR_DATAL);
            BUF_HEAD.irkey_state_code = HIIR_KEY_DOWN;
            hiir_dev.head = INC_BUF(hiir_dev.head, hiir_dev.buf_len);
            wake_up_interruptible(&(hiir_dev.irkey_wait));
        }
    }
    else
    {
        printk(KERN_DEBUG HIIR_PFX "logic Error: int_mask=0x%.8x, int_state=0x%.8x.\n", READ_REG(IR_INTM), READ_REG(IR_INTS));
    }

    hiir_dbg("Exit hiir_interrupt.\n");

    return IRQ_HANDLED;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int hiir_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
static long hiir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    int __user *p = (int __user *)arg;
    unsigned int min_val, max_val;

    switch(cmd)
    {
        case IR_IOC_SET_BUF:
            if((0<arg) && (arg<=HI_IR_MAX_BUF))
            {
                hiir_dev.buf_len = arg + 1;

                /* need reset buf pointrt */
                hiir_dev.head = 0;
                hiir_dev.tail = 0;
            }
            hiir_dbg("IR_IOC_SET_BUF -> buf_len=%d\n", hiir_dev.buf_len);
            break;
        case IR_IOC_SET_ENABLE_KEYUP:
            hiir_dev.enable_keyup = arg;
            hiir_dbg("IR_IOC_SET_ENABLE_KEYUP -> enable_keyup=%d\n", hiir_dev.enable_keyup);
            break;
        case IR_IOC_SET_ENABLE_REPKEY:
            hiir_dev.enable_repkey = arg;
            hiir_dbg("IR_IOC_SET_ENABLE_REPKEY -> enable_repkey=%d\n", hiir_dev.enable_repkey);
            break;
        case IR_IOC_SET_REPKEY_TIMEOUTVAL:
            if(arg > 0)
            {
                hiir_dev.repkey_delaytime = arg;
            }
            hiir_dbg("IR_IOC_SET_REPKEY_TIMEOUTVAL -> repkey_delaytime=%d\n", hiir_dev.repkey_delaytime);
            break;
        case IR_IOC_SET_FORMAT:
            if( (arg < 0) || (arg > 3) )
            {
                printk(KERN_DEBUG HIIR_PFX "Error: IR_IOC_SET_FORMAT -> invalid args=%lu\n", arg);
                return -EFAULT;
            }
            hiir_dev.dev_parm.codetype = arg;
            hiir_dbg("IR_IOC_SET_FORMAT -> codetype=%d\n", hiir_dev.dev_parm.codetype);
            break;
        case IR_IOC_SET_CODELEN:
            hiir_dev.dev_parm.code_len = arg;
            hiir_dbg("IR_IOC_SET_CODELEN -> code_len=%d\n", hiir_dev.dev_parm.code_len);
            break;
        case IR_IOC_SET_FREQ:
            if( (arg <= 0) || (arg > 128) )
            {
                printk(KERN_DEBUG HIIR_PFX "Error: IR_IOC_SET_FREQ -> invalid args=%lu\n", arg);
                return -EFAULT;
            }
            hiir_dev.dev_parm.frequence = arg;
            hiir_dbg("IR_IOC_SET_FREQ -> frequence=%d\n", hiir_dev.dev_parm.frequence);
            break;
        case IR_IOC_SET_LEADS:
            if( get_user(min_val, p) || get_user(max_val, p+1) )
            {
                return -EFAULT;
            }
            hiir_dev.dev_parm.leads_min = min_val;
            hiir_dev.dev_parm.leads_max = max_val;
            hiir_dbg("IR_IOC_SET_LEADS -> leads_min=%d, leads_max=%d\n", hiir_dev.dev_parm.leads_min, hiir_dev.dev_parm.leads_max);
            break;
        case IR_IOC_SET_LEADE:
            if( get_user(min_val, p) || get_user(max_val, p+1) )
            {
                return -EFAULT;
            }
            hiir_dev.dev_parm.leade_min = min_val;
            hiir_dev.dev_parm.leade_max = max_val;
            hiir_dbg("IR_IOC_SET_LEADE -> leade_min=%d, leade_max=%d\n", hiir_dev.dev_parm.leade_min, hiir_dev.dev_parm.leade_max);
            break;
        case IR_IOC_SET_SLEADE:
            if( get_user(min_val, p) || get_user(max_val, p+1) )
            {
                return -EFAULT;
            }
            hiir_dev.dev_parm.sleade_min = min_val;
            hiir_dev.dev_parm.sleade_max = max_val;
            hiir_dbg("IR_IOC_SET_SLEADE -> sleade_min=%d, sleade_max=%d\n", hiir_dev.dev_parm.sleade_min, hiir_dev.dev_parm.sleade_max);
            break;
        case IR_IOC_SET_CNT0_B:
            if( get_user(min_val, p) || get_user(max_val, p+1) )
            {
                return -EFAULT;
            }
            hiir_dev.dev_parm.cnt0_b_min = min_val;
            hiir_dev.dev_parm.cnt0_b_max = max_val;
            hiir_dbg("IR_IOC_SET_CNT0_B -> cnt0_b_min=%d, cnt0_b_max=%d\n", hiir_dev.dev_parm.cnt0_b_min, hiir_dev.dev_parm.cnt0_b_max);
            break;
        case IR_IOC_SET_CNT1_B:
            if( get_user(min_val, p) || get_user(max_val, p+1) )
            {
                return -EFAULT;
            }
            hiir_dev.dev_parm.cnt1_b_min = min_val;
            hiir_dev.dev_parm.cnt1_b_max = max_val;
            hiir_dbg("IR_IOC_SET_CNT1_B -> cnt1_b_min=%d, cnt1_b_max=%d\n", hiir_dev.dev_parm.cnt1_b_min, hiir_dev.dev_parm.cnt1_b_max);
            break;
        case IR_IOC_GET_CONFIG:
            if(copy_to_user((void __user *)arg, &(hiir_dev.dev_parm), sizeof(hiir_dev_param)))
            {
				printk("%s: copy_to_user fail!\n",__func__);
            }
            hiir_dbg("IR_IOC_GET_CONFIG\n");
            break;
        case IR_IOC_ENDBG:
            g_dbg_flag = 1;
            hiir_dbg("IR_IOC_ENDBG\n");
            break;
        case IR_IOC_DISDBG:
            g_dbg_flag = 0;
            hiir_dbg("IR_IOC_DISDBG\n");
            break;
        default:
            printk(KERN_DEBUG HIIR_PFX "Error: Inappropriate ioctl for device. cmd=%d\n", cmd);
            return -ENOTTY;
    }

    hiir_config();

    return 0;
}

static ssize_t hiir_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    irkey_info_s irkey_to_user;
    int res = 0;

retry :
    hiir_dbg("Enter hiir_read : head=%d, tail=%d, buf_len=%d\n", hiir_dev.head, hiir_dev.tail, hiir_dev.buf_len);

    if((hiir_dev.head)==(hiir_dev.tail))
    {
        if((filp->f_flags & O_NONBLOCK) == O_NONBLOCK)
        { return -EAGAIN; }
        wait_event_interruptible(hiir_dev.irkey_wait,(hiir_dev.head != hiir_dev.tail));
        if(signal_pending(current))
        { return -ERESTARTSYS; }
        goto retry;
    }
    else
    {
        while( ((hiir_dev.head)!=(hiir_dev.tail)) && ((res + sizeof(irkey_info_s)) <= count) )
        {
            irkey_to_user = BUF_TAIL;
            hiir_dev.tail = INC_BUF(hiir_dev.tail, hiir_dev.buf_len);
            if(copy_to_user((buf + res), &irkey_to_user, sizeof(irkey_info_s)))
            {
				printk("%s: copy_to_user fail!\n",__func__);
            }
            res += sizeof(irkey_info_s);
        }
    }
    return res;
}

static unsigned int hiir_select(struct file *filp, struct poll_table_struct *wait)
{
    hiir_dbg("Enter hiir_select.\n");
    if((hiir_dev.head)!=(hiir_dev.tail))
    {
        return 1;
    }
    poll_wait(filp, &(hiir_dev.irkey_wait), wait);
    return 0;
}

static atomic_t hiir_s_available = ATOMIC_INIT(1);

static int hiir_open(struct inode *inode, struct file *filp)
{
    hiir_dbg("Enter hiir_open.\n");

    if(! atomic_dec_and_test(&hiir_s_available) )
    {
        atomic_inc(&hiir_s_available);
        printk(KERN_DEBUG HIIR_PFX "Error: device already open.\n");
        return -EBUSY;
    }

    del_timer(&repkey_timeout_timer);

    g_dbg_flag = 0;

    /* init hiir_dev */
    hiir_dev.dev_parm = static_dev_param[0];/*nec format*/
    hiir_dev.head = 0;
    hiir_dev.tail = 0;
    hiir_dev.buf_len = DEFAULT_BUF_LEN;
    hiir_dev.enable_repkey = 1;
    hiir_dev.repkey_checkflag = 0;
    hiir_dev.repkey_delaytime = DEFAULT_DELAYTIME;
    hiir_dev.enable_keyup = 1;
    init_waitqueue_head(&hiir_dev.irkey_wait);

    hiir_config();

    return 0;
}

static int hiir_release(struct inode *inode, struct file *filp)
{
    hiir_dbg("Enter hiir_release.\n");

    del_timer(&repkey_timeout_timer);

    /* disable HIIR11 */
    WRITE_REG(IR_ENABLE, 0x00);

    atomic_inc(&hiir_s_available);

    return 0;
}

static int hiir_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    return 0;
}

static struct file_operations hiir_fops =
{
    owner   : THIS_MODULE,
    open    : hiir_open,
    unlocked_ioctl   : hiir_ioctl,
    poll    : hiir_select,
    read    : hiir_read,
    write   : hiir_write,
    release : hiir_release,
};

static struct miscdevice hiir_miscdev =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name  = HIIR_DEVICE_NAME,
    .fops  = &hiir_fops,
};

static int __init hiir_init(void)
{
    int res = 0;

    printk(KERN_INFO OSDRV_MODULE_VERSION_STRING "\n");

    /* disable HIIR */
    WRITE_REG(IR_ENABLE, 0x00);

	res = misc_register(&hiir_miscdev);
        if (0 != res)
        {
            printk(KERN_DEBUG HIIR_PFX "Error: can't register\n");
            return -EFAULT;
        }

    res = request_irq(ir_irq, hiir_interrupt, 0, HIIR_DEVICE_NAME, &hiir_interrupt);
    if(res)
    {
        printk(KERN_DEBUG HIIR_PFX "Error: request IRQ(%d) failed\n", HIIR_DEVICE_IRQ_NO);
        misc_deregister(&hiir_miscdev);
        goto hi_ir_init_failed;
    }

    printk(HIIR_PFX "init ok. ver=%s, %s.\n", __DATE__, __TIME__);

hi_ir_init_failed:
    return res;
}

static void __exit hiir_exit(void)
{
    free_irq(ir_irq, &hiir_interrupt);
    misc_deregister(&hiir_miscdev);
    printk(HIIR_PFX "unload hi_ir.ko ok.\n");
}

static int hi35xx_ir_probe(struct platform_device *pdev)
{
    struct resource *mem;

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    ir_reg_base = (unsigned int)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void *)ir_reg_base))
            return PTR_ERR((void *)ir_reg_base);

    ir_irq = platform_get_irq(pdev, 0);
    if (ir_irq <= 0) {
            dev_err(&pdev->dev, "cannot find ir IRQ\n");
    }

    if(0 != hiir_init())
    {
        return -1;
    }

    return 0;
}

static int hi35xx_ir_remove(struct platform_device *pdev)
{
    hiir_exit();
    return 0;
}

static const struct of_device_id hi35xx_ir_match[] = {
        { .compatible = "hisilicon,hi_ir" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_ir_match);

static struct platform_driver hi35xx_ir_driver = {
        .probe          = hi35xx_ir_probe,
        .remove         = hi35xx_ir_remove,
        .driver         = {
                .name   = "hi35xx_ir",
                .of_match_table = hi35xx_ir_match,
        },
};

module_platform_driver(hi35xx_ir_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hisilicon Infrared remoter(HIIR11) Device Driver");
MODULE_VERSION("HI_VERSION=" OSDRV_MODULE_VERSION_STRING);



