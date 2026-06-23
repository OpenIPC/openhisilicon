/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "piris.h"
#include <linux/miscdevice.h>
#ifdef __LITEOS__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ot_type.h"
#include <linux/fs.h>
#endif
#include "piris_hal.h"
#include "isp_ext.h"
#include "mm_ext.h"
#include "securec.h"
#include "ot_osal.h"
#include "piris_hal_intf.h"
#include "ot_debug.h"

#ifdef __LITEOS__
#ifndef IOMEM
#define IOMEM
#endif
#else
#ifndef IOMEM
#define IOMEM __iomem
#endif
#endif

td_void IOMEM *g_reg_piris_ibase[PIRIS_MAX_DEV_NUM];

#define MAX_MOTOR_PAHSE 4
#define MAX_STEPS       92
#define PIRIS_PPS       100
#define MAX_TIMEOUT     300

#define piris_err_trace(fmt, ...) \
    OT_ERR_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

static const td_uchar g_motor_phase_tbl[MAX_MOTOR_PAHSE] = { 0x0, 0x1, 0x2, 0x3};

typedef struct {
    td_s32 src_pos;
    td_s32 dest_pos;
    td_u32 pps;
    td_s32 phase;
    const td_uchar *phase_tbl;
    osal_spinlock lock;
    osal_semaphore sem;
    osal_timer timer;
    osal_completion piris_comp;
} piris_dev_s;

static piris_dev_s *g_piris_dev[PIRIS_MAX_DEV_NUM] = { TD_NULL };
static td_s32 g_dev;
td_bool g_piris_io_remap = TD_FALSE;

td_s32 piris_gpio_update(td_s32 dev, const td_s32 *piris_pos)
{
    td_ulong flags = 0;
    piris_dev_s *piris = TD_NULL;
    td_s32 dev_id;
    dev_id = MAX2(MIN2(dev, (PIRIS_MAX_DEV_NUM - 1)), 0);
    g_dev = dev_id;

    piris = g_piris_dev[dev_id];
    osal_spin_lock_irqsave(&piris->lock, &flags);
    piris->dest_pos = MAX2(MIN2(*piris_pos, 200), -200); /* range:[-200,200] */
    piris->pps = PIRIS_PPS;
    piris->pps = MAX2(MIN2(piris->pps, HZ), 1);
    osal_timer_mod(&piris->timer, 1);
    osal_spin_unlock_irqrestore(&piris->lock, &flags);
    return 0;
}

#define HYS_STEPS       100

/* first go to the full open iris step, set the full open as origin */
#ifdef __LITEOS__
static td_s32 piris_origin_set(td_s32 dev, piris_data *piris_data_info)
#else
td_s32 piris_origin_set(td_s32 dev, piris_data *piris_data_info)
#endif
{
    td_s32 piris_pos;
    td_ulong flags = 0;
    piris_dev_s *piris;
    td_s32 sign, hys_offset, hys_cnt;

    hys_cnt = 3;    /* should be even, default as 3 */
    piris = g_piris_dev[dev];
    piris_pos = piris_data_info->cur_pos;
    if ((piris_pos > 200) || (piris_pos < -200) || (piris_pos == g_piris_dev[dev]->src_pos)) { /* pos: [-200, 200] */
        piris_err_trace("Invalid cur_pos!\n");
        return -1;
    }

    if ((piris_data_info->total_step) > 400 || (piris_data_info->total_step == 0)) { /* total_step range (0, 400] */
        piris_err_trace("Invalid total_step!\n");
        return -1;
    }

    piris_gpio_update(dev, &piris_pos);
    osal_completion_reinit(&piris->piris_comp);
    /* wait for piris origin done */
    osal_wait_for_completion_timeout(&piris->piris_comp, MAX_TIMEOUT);
    /* consider hysteresis effection */
    sign = (piris_data_info->zero_is_max) ? 1 : -1;
    do {
        hys_offset = sign * HYS_STEPS;
        piris_pos += hys_offset;
        piris_gpio_update(dev, &piris_pos);
        osal_wait_for_completion_timeout(&piris->piris_comp, MAX_TIMEOUT);
        sign = -sign;
        hys_cnt--;
    } while (hys_cnt);

    osal_msleep(1000); /* system sleep for 1000 ms (1s) */

    piris_pos += piris_data_info->total_step * sign;
    piris_gpio_update(dev, &piris_pos);
    osal_wait_for_completion_timeout(&piris->piris_comp, MAX_TIMEOUT);
    /* then offset to full open position */
    osal_spin_lock_irqsave(&piris->lock, &flags);
    if (piris_data_info->zero_is_max == 1) {
        piris->src_pos  = 0;
        piris->dest_pos = 0;
    } else {
        piris->src_pos  = piris_data_info->total_step - 1;
        piris->dest_pos = piris_data_info->total_step - 1;
    }
    osal_spin_unlock_irqrestore(&piris->lock, &flags);

    return 0;
}

/* go to the full close iris step */
#ifdef __LITEOS__
static td_s32 piris_close_set(td_s32 dev, piris_data *piris_data_info)
#else
td_s32 piris_close_set(td_s32 dev, piris_data *piris_data_info)
#endif
{
    td_s32 piris_pos;
    td_ulong flags = 0;
    piris_dev_s *piris;

    piris = g_piris_dev[dev];
    piris_pos = piris_data_info->cur_pos;

    osal_completion_reinit(&piris->piris_comp);
    piris_gpio_update(dev, &piris_pos);
    /* wait for piris origin done */
    osal_wait_for_completion_timeout(&piris->piris_comp, MAX_TIMEOUT);
    osal_spin_lock_irqsave(&piris->lock, &flags);
    if (piris_data_info->zero_is_max == 1) {
        piris->src_pos  = piris_data_info->total_step - 1;
        piris->dest_pos = piris_data_info->total_step - 1;
    } else {
        piris->src_pos  = 0;
        piris->dest_pos = 0;
    }

    osal_spin_unlock_irqrestore(&piris->lock, &flags);
    return 0;
}

/* file operation */
#ifdef __LITEOS__
td_s32 piris_open(struct file *filep)
{
    ot_unused(filep);
    return 0;
}

td_s32 piris_close(struct file *filep)
{
    ot_unused(filep);
    return 0;
}
#else
td_s32 piris_open(struct inode *inode, struct file *file)
{
    ot_unused(inode);
    ot_unused(file);
    return 0;
}
td_s32  piris_close(struct inode *inode, struct file *file)
{
    ot_unused(inode);
    ot_unused(file);
    return 0;
}
#endif


#ifdef __LITEOS__
static td_s32 piris_ioctl(struct file *file, td_s32 cmd, td_ulong arg)
#else
static long piris_ioctl(struct file *file, td_u32 cmd, td_ulong arg)
#endif
{
    td_s32 ret;
    td_s32 piris_pos;
    piris_data    piris_data_info;
    piris_status  piris_status_info;
    piris_dev_s   *piris;

#ifdef __LITEOS__
    td_s32 dev;
    dev = (td_s32)(file->f_priv);
#else
    td_slong dev;
    dev = (td_slong)(td_uintptr_t)(file->private_data);
#endif

    dev = MAX2(MIN2(dev, 1), 0);
    piris = g_piris_dev[dev];

    if (_IOC_TYPE(cmd) != PIRIS_IOC_MAGIC) {
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > PIRIS_IOC_MAXNR) {
        return -ENOTTY;
    }
    if (arg == 0L) {
        return -1;
    }
    /* lock piris */
    if (osal_sem_down(&piris->sem)) {
        return -ERESTARTSYS;
    }

    switch (cmd) {
        case PIRIS_SET_FD:
#ifdef __LITEOS__
            if (read_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(td_s32)) == TD_FALSE) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }

            ret = copy_from_user(&dev, (td_s32 __user *)(td_uintptr_t)arg, sizeof(td_s32));
            if (ret) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            file->f_priv = (td_void *)(td_uintptr_t)(dev);
#else
            if (read_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(td_slong)) == TD_FALSE) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            ret = osal_copy_from_user(&dev, (td_slong __user *)(td_uintptr_t)arg, sizeof(td_slong));
            if (ret) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            file->private_data = (td_void *)(td_uintptr_t)(dev);
#endif
            break;

        case PIRIS_SET_ACT_ARGS:

            if (read_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(td_s32)) == TD_FALSE) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }

            ret = osal_copy_from_user(&piris_pos, (td_s32 __user *)(td_uintptr_t)arg, sizeof(td_s32));
            if (ret) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            piris_gpio_update(dev, &piris_pos);
            break;

        case PIRIS_SET_ORGIN:
            if (read_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(piris_data)) == TD_FALSE) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }

            ret = osal_copy_from_user(&piris_data_info, (piris_data __user *)(td_uintptr_t)arg, sizeof(piris_data));
            if (ret) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }

            piris_origin_set(dev, &piris_data_info);
            break;

        case PIRIS_SET_CLOSE:
            if (read_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(piris_data)) == TD_FALSE) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            ret = osal_copy_from_user(&piris_data_info, (piris_data __user *)(td_uintptr_t)arg, sizeof(piris_data));
            if (ret) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            piris_close_set(dev, &piris_data_info);
            break;

        case PIRIS_GET_STATUS:
            if (piris->dest_pos != piris->src_pos) {
                piris_status_info = PIRIS_BUSY;
            } else {
                piris_status_info = PIRIS_IDLE;
            }

            if (write_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(piris_status)) == TD_FALSE) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }

            ret = osal_copy_to_user((piris_status __user *)(td_uintptr_t)arg, &piris_status_info, sizeof(piris_status));
            if (ret) {
                osal_sem_up(&piris->sem);
                return -EFAULT;
            }
            break;

        default:  /* redundant, as cmd was checked against MAXNR */
            break;
    }

    /* unlock piris */
    osal_sem_up(&piris->sem);

    return 0;
}

#ifdef __LITEOS__
static struct file_operations_vfs g_piris_fops = {
    .ioctl = piris_ioctl,
    .open  = piris_open,
    .close = piris_close,
};

td_s32 ot_piris_device_register(td_void)
{
    return register_driver("/dev/piris", &g_piris_fops, 0666, 0); /* register addr, 0666 */
}

#else
static struct file_operations g_piris_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = piris_ioctl,
    .open           = piris_open,
    .release        = piris_close,
};
#endif

#ifndef __LITEOS__
static struct miscdevice g_st_piris_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "piris",
    .fops    = &g_piris_fops,
};
#endif

td_void piris_dev_unregister(td_void)
{
#ifdef __LITEOS__
    unregister_driver("/dev/piris");
#else
    misc_deregister(&g_st_piris_dev);
#endif
}

td_s32 piris_dev_register(td_void)
{
    td_s32 ret;
#ifdef __LITEOS__
    ret = ot_piris_device_register();
#else
    ret = misc_register(&g_st_piris_dev);
#endif
    return ret;
}

#ifdef __LITEOS__
td_void piris_timer_cb(UINT32 arg)
#else
td_void piris_timer_cb(td_ulong arg)
#endif
{
    td_s32 sign;
    td_uchar bits;
    td_ulong flags = 0;
    piris_dev_s *piris = TD_NULL;

    piris = g_piris_dev[g_dev];
    osal_spin_lock_irqsave(&piris->lock, &flags);

    if (piris->src_pos == piris->dest_pos) {
        osal_spin_unlock_irqrestore(&piris->lock, &flags);
        return;
    }

    sign = (piris->dest_pos - piris->src_pos < 0) ? -1 : 1;
    piris->src_pos += sign;

    /* close iris: 0->1->2->3->0;    open iris: 3->2->1->0->3 */
    piris->phase = (piris->phase + MAX_MOTOR_PAHSE + sign) % MAX_MOTOR_PAHSE;
    bits = piris->phase_tbl[piris->phase];
    piris_drv_write(g_reg_piris_ibase[g_dev], bits);
    if (piris->dest_pos == piris->src_pos) {
        osal_complete(&piris->piris_comp);
    }
    osal_spin_unlock_irqrestore(&piris->lock, &flags);
    osal_timer_mod(&piris->timer, 1);
    return;
}

static td_s32 ot_piris_isp_register(td_s32 dev)
{
    isp_piris_callback piris_cb = {0};

    piris_cb.pfn_piris_gpio_update = piris_gpio_update;
    if (ckfn_isp() && ckfn_isp_register_piris_callback()) {
        call_isp_register_piris_callback(dev, &piris_cb);
    } else {
        piris_err_trace("register piris_gpio_write_callback to isp failed, ot_piris init is failed!\n");
        return -1;
    }

    return 0;
}

static td_s32 ot_piris_isp_unregister(td_s32 dev)
{
    isp_piris_callback piris_cb = {0};

    piris_cb.pfn_piris_gpio_update = TD_NULL;
    if (ckfn_isp() && ckfn_isp_register_piris_callback()) {
        call_isp_register_piris_callback(dev, &piris_cb);
    } else {
        piris_err_trace("unregister piris_gpio_write_callback to isp failed!\n");
        return -1;
    }

    return 0;
}

#define PIRISI_SIZE_ALIGN 0x10000
static td_void piris_init_unmap(td_s32 num)
{
    td_s32 i;
    for (i = 0; i < num; i++) {
        if (g_reg_piris_ibase[i] != TD_NULL) {
            osal_iounmap(g_reg_piris_ibase[i], PIRISI_SIZE_ALIGN);
            g_reg_piris_ibase[i] = TD_NULL;
        }
    }

    g_piris_io_remap = TD_FALSE;
    return;
}

td_s32 piris_init_remap(td_s32 i)
{
    if (!g_reg_piris_ibase[i]) {
        g_reg_piris_ibase[i] = osal_ioremap(PIRISI_ADRESS_BASE, PIRISI_SIZE_ALIGN);
        if (!g_reg_piris_ibase[i]) {
            return -1;
        }
    }

    return 0;
}

static td_void piris_close_dev(td_s32 dev_num)
{
    td_s32 i;
    for (i = 0; i < dev_num; i++) {
        if (g_piris_dev[i] != TD_NULL) {
            osal_kfree(g_piris_dev[i]);
            g_piris_dev[i] = TD_NULL;
        }
    }

    return;
}

td_s32 piris_open_dev(td_s32 i, piris_dev_s *pdev)
{
    if (pdev == TD_NULL) {
        return -1;
    }
    (td_void)memset_s(pdev, sizeof(piris_dev_s), 0x0, sizeof(piris_dev_s));
    g_piris_dev[i] = pdev;

    return 0;
}

static td_void ot_piris_isp_register_fail(td_s32 dev)
{
    td_s32 i;
    piris_dev_s *pdev = TD_NULL;

    for (i = 0; i <= dev; i++) {
        pdev = g_piris_dev[i];
        osal_timer_destroy(&pdev->timer);
        osal_spin_lock_destroy(&g_piris_dev[i]->lock);
        osal_sem_destroy(&g_piris_dev[i]->sem);
        osal_complete_destory(&g_piris_dev[i]->piris_comp);
    }

    return;
}

static td_void piris_init_dev(td_s32 dev)
{
    osal_spin_lock_init(&g_piris_dev[dev]->lock);
    osal_sem_init(&g_piris_dev[dev]->sem, 1);
    osal_completion_init(&g_piris_dev[dev]->piris_comp);
#ifndef __LITEOS__
    g_piris_dev[dev]->timer.handler = piris_timer_cb;
#else
    g_piris_dev[dev]->timer.handler = (td_void *)piris_timer_cb;
    g_piris_dev[dev]->timer.data = (td_ulong)(td_uintptr_t)g_piris_dev[dev];
#endif
    g_piris_dev[dev]->timer.interval = 1;
    osal_timer_init(&g_piris_dev[dev]->timer);
    g_piris_dev[dev]->phase_tbl = g_motor_phase_tbl;
}

/* module init and exit */
td_s32 piris_mod_init(td_void)
{
    td_s32 ret, i;
    piris_dev_s *pdev = TD_NULL;

    ret = piris_dev_register();
    if (ret != 0) {
        osal_printk("register piris device failed with %#x!\n", ret);
        return -1;
    }
    /* initiate piris map */
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        ret = piris_init_remap(i);
        if (ret != 0) {
            osal_printk("initiate piris map failed at piris dev[%d]!\n", i);
            piris_init_unmap(i);
            goto OUT1;
        }
    }
    g_piris_io_remap = TD_TRUE;
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        pdev = osal_kmalloc(sizeof(piris_dev_s), OSAL_GFP_KERNEL);
        ret = piris_open_dev(i, pdev);
        if (ret != 0) {
            osal_printk("open piris dev failed at piris dev[%d]!\n", i);
            piris_close_dev(i);
            goto OUT2;
        }
    }
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        piris_init_dev(i);
        ret = ot_piris_isp_register(i);
        if (ret != TD_SUCCESS) {
            ot_piris_isp_register_fail(i);
            goto OUT3;
        }
    }
    osal_printk("load ot_piris.ko ....OK!\n");
    return 0;

OUT3:
    piris_close_dev(PIRIS_MAX_DEV_NUM);
OUT2:
    piris_init_unmap(PIRIS_MAX_DEV_NUM);
OUT1:
    piris_dev_unregister();
    return -1;
}

td_void piris_mod_exit(td_void)
{
    td_s32 i;
    piris_dev_s *pdev = TD_NULL;

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        pdev = g_piris_dev[i];
        osal_timer_destroy(&pdev->timer);
        osal_spin_lock_destroy(&g_piris_dev[i]->lock);
        osal_sem_destroy(&g_piris_dev[i]->sem);
        osal_complete_destory(&g_piris_dev[i]->piris_comp);
        if (g_piris_io_remap == TD_TRUE) {
            osal_iounmap(g_reg_piris_ibase[i], PIRISI_SIZE_ALIGN);
            g_reg_piris_ibase[i] = TD_NULL;
        }
        if (pdev != TD_NULL) {
            osal_kfree(pdev);
            pdev = TD_NULL;
        }
        g_piris_dev[i] = TD_NULL;
        ot_piris_isp_unregister(i);
    }

    g_piris_io_remap = TD_FALSE;

    piris_dev_unregister();
    osal_printk("unload ot_piris.ko ....OK!\n");
}
