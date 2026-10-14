/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor_exe.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include "osal.h"
#include "mmz.h"
#include "common.h"
#include "msensor_ext.h"
#include "msensor_buf.h"
#ifdef CONFIG_PROC_SHOW_SUPPORT
#include "msensor_mng_proc.h"
#endif
#include "msensor_mng_ioctl.h"

#define UMAP_MSENSOR_MNG_MINOR_BASE  154
#define UMAP_DEVNANME_MSENSOR_MNG_BASE        "msensor_mng"

static osal_dev_t *g_msensor_mng_dev = XMEDIA_NULL;
msensor_mng_proc_info g_mng_proc_info[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
static msensor_mng_callback g_msensor_mng_callback[XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_NULL };
static osal_atomic_t g_msensor_mng_user_ref[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

#define USER_SEND_DATA 1

#define GYRO_NAME      "ICM42670"

msensor_mng_proc_info *msensor_mng_get_proc_info(xmedia_s32 dev)
{
    return &g_mng_proc_info[dev];
}

xmedia_s32 msensor_mng_proc_info_init(xmedia_s32 dev)
{
    msensor_mng_proc_info *proc_info = XMEDIA_NULL;
    proc_info = msensor_mng_get_proc_info(dev);

    osal_memcpy(proc_info->gyro_name, "ICM42670", sizeof("ICM42670"));

    osal_memcpy(proc_info->accel_name, "ICM42670", sizeof("ICM42670"));

    return XMEDIA_SUCCESS;
}

xmedia_s32 msensor_mng_write_data_to_buf(xmedia_s32 dev, xmedia_msensor_data *msensor_data)
{
    xmedia_u32 i;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    osal_spinlock_t *mng_lock = msensor_buf_get_lock(dev);
    xmedia_ulong flags;

    if (msensor_data == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d msensor data is null\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    if ((msensor_data->gyro_buf.data_num > XMEDIA_MSENSOR_MAX_DATA_NUM) ||
        (msensor_data->acc_buf.data_num > XMEDIA_MSENSOR_MAX_DATA_NUM)) {
        return XMEDIA_FAILURE;
    }

    osal_spin_lock_irqsave(mng_lock, &flags);

    if ((msensor_data->attr.device_mask & XMEDIA_MSENSOR_DEVICE_GYRO) == XMEDIA_MSENSOR_DEVICE_GYRO) {
        for (i = 0; i < msensor_data->gyro_buf.data_num; i++) {
            if ((msensor_data->attr.temperature_mask & XMEDIA_MSENSOR_TEMP_GYRO) != XMEDIA_MSENSOR_TEMP_GYRO) {
                msensor_data->gyro_buf.gyro_data[i].temperature = 0xffffffff;
            }

            ret = msensor_buf_write_data(dev, XMEDIA_MSENSOR_DATA_TYPE_GYRO, &(msensor_data->gyro_buf.gyro_data[i]));
        }
    }

    if ((msensor_data->attr.device_mask & XMEDIA_MSENSOR_DEVICE_ACC) == XMEDIA_MSENSOR_DEVICE_ACC) {
        for (i = 0; i < msensor_data->acc_buf.data_num; i++) {
            if ((msensor_data->attr.temperature_mask & XMEDIA_MSENSOR_TEMP_ACC) != XMEDIA_MSENSOR_TEMP_ACC) {
                msensor_data->acc_buf.acc_data[i].temperature = 0xffffffff;
            }
            ret = msensor_buf_write_data(dev, XMEDIA_MSENSOR_DATA_TYPE_ACC, &(msensor_data->acc_buf.acc_data[i]));
        }
    }

    osal_spin_unlock_irqrestore(mng_lock, &flags);
    return ret;
}

static xmedia_s32 msensor_buf_check_phys_addr(xmedia_s32 dev, xmedia_msensor_data_info *data_info)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    xmedia_unused(dev);

    if (data_info->data[0].num > 0) {
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[0].x_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[0].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[0].y_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[0].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[0].z_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[0].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[0].temperature_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[0].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[0].pts_phys_addr,
                                 sizeof(xmedia_u64) * data_info->data[0].num);
    }

    if (data_info->data[1].num > 0) {
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[1].x_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[1].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[1].y_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[1].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[1].z_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[1].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[1].temperature_phys_addr,
                                 sizeof(xmedia_s32) * data_info->data[1].num);
        ret |= mmz_check_phyaddr((xmedia_u64)(xmedia_uintptr_t)data_info->data[1].pts_phys_addr,
                                 sizeof(xmedia_u64) * data_info->data[1].num);
    }

    return ret;
}

/* msensor mng ioctl functions */
static xmedia_s32 msensor_mng_drv_user_get_data(xmedia_s32 dev, xmedia_ulong arg)
{
    xmedia_s32 ret;

    ret = msensor_buf_get_data(dev, (xmedia_msensor_data_info *)arg);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d get data failed! ret=%x\n", dev, ret);
        return ret;
    }

    ret = msensor_buf_check_phys_addr(dev, (xmedia_msensor_data_info *)arg);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d check msensor data addr share failed\n", dev);
        return ret;
    }
    return ret;
}

static xmedia_s32 msensor_mng_drv_user_release_buf(xmedia_s32 dev, xmedia_ulong arg)
{
    xmedia_s32 ret;

    ret = msensor_buf_check_phys_addr(dev, (xmedia_msensor_data_info *)arg);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d check msensor_data_addr share failed\n", dev);
        return XMEDIA_FAILURE;
    }

    ret = msensor_buf_release_data(dev, (xmedia_msensor_data_info *)arg);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d  buf release failed! ret=%x\n", dev, ret);
        return XMEDIA_FAILURE;
    }
    return ret;
}

static xmedia_s32 msensor_mng_drv_user_add_user(xmedia_s32 dev, xmedia_ulong arg)
{
    xmedia_s32 ret;

    ret = msensor_buf_add_user(dev, (xmedia_s32 *)arg);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d msensor buf add_user failed! ret=%x\n", dev, ret);
        return ret;
    }
    return ret;
}

static xmedia_s32 msensor_mng_drv_user_delete_user(xmedia_s32 dev, xmedia_ulong arg)
{
    xmedia_s32 ret;

    ret = msensor_buf_delete_user(dev, (xmedia_s32 *)arg);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d msensor buf delete user failed! ret=%x\n", dev, ret);
        return ret;
    }
    return ret;
}

static xmedia_s32 msensor_mng_drv_user_send_data(xmedia_s32 dev, xmedia_ulong arg)
{
#ifdef USER_SEND_DATA
    xmedia_s32 ret;
    xmedia_msensor_data *msensor_data;

    msensor_data = (xmedia_msensor_data *)arg;

    ret = msensor_mng_write_data_to_buf(dev, msensor_data);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d write data to buf failed! ret=%x\n", dev, ret);
        return ret;
    }
    return ret;
#else
    MSENSOR_ERR_TRACE("dev %d msensor send data not support\n", dev);
    return XMEDIA_FAILURE;
#endif
}
xmedia_s32 msensor_mng_drv_get_data(xmedia_s32 dev, xmedia_ulong arg)
{

    return msensor_mng_drv_user_get_data(dev, arg);
}

xmedia_s32 msensor_mng_drv_release_buf(xmedia_s32 dev, xmedia_ulong arg)
{

    return msensor_mng_drv_user_release_buf(dev, arg);
}

xmedia_s32 msensor_mng_drv_add_user(xmedia_s32 dev, xmedia_ulong arg)
{

    return msensor_mng_drv_user_add_user(dev, arg);
}

xmedia_s32 msensor_mng_drv_delete_user(xmedia_s32 dev, xmedia_ulong arg)
{

    return msensor_mng_drv_user_delete_user(dev, arg);
}

xmedia_s32 msensor_mng_drv_send_data(xmedia_s32 dev, xmedia_ulong arg)
{

    return msensor_mng_drv_user_send_data(dev, arg);
}

typedef xmedia_s32 (*msensor_ctl_ptr_func)(xmedia_s32 dev_id, xmedia_ulong arg);

typedef struct {
    xmedia_u32 cmd;
    msensor_ctl_ptr_func ptr_func;
} msensor_ioctl_func_item;

static msensor_ioctl_func_item g_msensor_mng_cmd_list[] = {
    { MSENSOR_CMD_GET_DATA, msensor_mng_drv_get_data },   { MSENSOR_CMD_RELEASE_BUF, msensor_mng_drv_release_buf },
    { MSENSOR_CMD_ADD_USER, msensor_mng_drv_add_user },   { MSENSOR_CMD_DELETE_USER, msensor_mng_drv_delete_user },
    { MSENSOR_CMD_SEND_DATA, msensor_mng_drv_send_data },
};

static xmedia_s32 msensor_mng_do_ioctl(xmedia_s32 dev, xmedia_u32 cmd, xmedia_ulong arg)
{
    xmedia_s32 i;

    for (i = 0; i < sizeof(g_msensor_mng_cmd_list) / sizeof(g_msensor_mng_cmd_list[0]); i++) {
        if (cmd == g_msensor_mng_cmd_list[i].cmd) {
            return g_msensor_mng_cmd_list[i].ptr_func(dev, arg);
        }
    }

    MSENSOR_ERR_TRACE("dev %d  msensor chip ioctl cmd 0x%x not supported!\n", dev, cmd);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

xmedia_slong msensor_mng_ioctl(xmedia_u32 cmd, xmedia_ulong arg, xmedia_void *private_data)
{
    xmedia_s32 ret;
    xmedia_s32 dev;

    if (cmd == MSENSOR_CMD_MNG_BIND_FLAG2FD) {
        *(xmedia_u32 *)private_data = *(xmedia_u32 *)arg;
        return XMEDIA_SUCCESS;
    }

    dev = *(xmedia_u32 *)private_data;

    osal_atomic_inc_return(&g_msensor_mng_user_ref[dev]);

    ret = msensor_mng_do_ioctl(dev, cmd, arg);

    osal_atomic_dec_return(&g_msensor_mng_user_ref[dev]);

    return ret;
}

static xmedia_s32 msensor_mng_open(xmedia_void *private_data)
{
    xmedia_unused(private_data);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_mng_release(xmedia_void *private_data)
{
    xmedia_unused(private_data);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_mng_freeze(osal_dev_t *dev)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_mng_restore(osal_dev_t *dev)
{
    return XMEDIA_SUCCESS;
}

// todo，待实现 init 函数
// static xmedia_s32 msensor_mng_fn_init(xmedia_void *args)
// {
//     xmedia_unused(args);
//     return XMEDIA_SUCCESS;
// }

xmedia_s32 msensor_mng_buf_init(xmedia_s32 dev, const xmedia_msensor_attr *motion_attr,
                                const xmedia_msensor_buf_attr *msensor_buf_attr,
                                const xmedia_msensor_config *msensor_config)
{
    xmedia_u32 gyro_odr;
    xmedia_u32 acc_odr;
    xmedia_u32 magn_odr;
    xmedia_s32 ret;

    if (motion_attr == XMEDIA_NULL || msensor_buf_attr == XMEDIA_NULL || msensor_config == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("input NULL\n");
        return XMEDIA_FAILURE;
    }

    switch (motion_attr->device_mask) {
        case XMEDIA_MSENSOR_DEVICE_GYRO | XMEDIA_MSENSOR_DEVICE_ACC: {
            /* only for american present */
            gyro_odr = msensor_config->gyro_config.odr;
            acc_odr = msensor_config->acc_config.odr;
            magn_odr = 0;

            msensor_debug_trace("dev %d odr:gyro_odr:%d acc_odr:%d magn_odr:%d\n", dev, gyro_odr, acc_odr, magn_odr);

            ret = msensor_buf_init(dev, msensor_buf_attr, gyro_odr, acc_odr, magn_odr);
            break;
        }

        case XMEDIA_MSENSOR_DEVICE_GYRO: {
            /* only for american present */
            gyro_odr = msensor_config->gyro_config.odr;
            acc_odr = 0;
            magn_odr = 0;

            msensor_debug_trace("dev %d gyro_odr:%d acc_odr:%d magn_odr:%d\n", dev, gyro_odr, acc_odr, magn_odr);

            ret = msensor_buf_init(dev, msensor_buf_attr, gyro_odr, acc_odr, magn_odr);
            break;
        }
        #if 0
        case XMEDIA_MSENSOR_DEVICE_ALL: {
            /* only for american present */
            gyro_odr = msensor_config->gyro_config.odr;
            acc_odr = msensor_config->acc_config.odr;
            magn_odr = msensor_config->acc_config.odr;

            msensor_debug_trace("dev %d gyro_odr:%d acc_odr:%d magn_odr:%d\n", dev, gyro_odr, acc_odr, magn_odr);

            ret = msensor_buf_init(dev, msensor_buf_attr, gyro_odr, acc_odr, magn_odr);
            break;
        }
        #endif
        default: {
            gyro_odr = 0;
            acc_odr = 0;
            magn_odr = 0;
            MSENSOR_ERR_TRACE("dev %d msensor mng init buf(gyro_odr:%d acc_odr:%d magn_odr:%d) err!\n", dev, gyro_odr,
                              acc_odr, magn_odr);
            ret = XMEDIA_FAILURE;
            break;
        }
    }
    return ret;
}

xmedia_s32 msensor_mng_buf_exit(xmedia_s32 dev)
{
    return msensor_buf_exit(dev);
}

xmedia_s32 msensor_mng_get_chip_cfg(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    if (g_msensor_mng_callback[dev].pfn_get_config_from_chip == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d null point!\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }
    return g_msensor_mng_callback[dev].pfn_get_config_from_chip(dev, msensor_param);
}

xmedia_s32 msensor_mng_write_data_2_buf(xmedia_s32 dev)
{
    if (g_msensor_mng_callback[dev].pfn_write_data_to_buf == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d nul point!\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }
    return g_msensor_mng_callback[dev].pfn_write_data_to_buf(dev);
}

xmedia_s32 msensor_mng_register_call_back(xmedia_s32 dev, const msensor_mng_callback *callback)
{
    msensor_return_if_null_ptr(callback);

    g_msensor_mng_callback[dev].pfn_get_config_from_chip = callback->pfn_get_config_from_chip;
    g_msensor_mng_callback[dev].pfn_write_data_to_buf = callback->pfn_write_data_to_buf;
    return XMEDIA_SUCCESS;
}

xmedia_void msensor_mng_unregister_call_back(xmedia_s32 dev)
{
    g_msensor_mng_callback[dev].pfn_get_config_from_chip = XMEDIA_NULL;
    g_msensor_mng_callback[dev].pfn_write_data_to_buf = XMEDIA_NULL;
    return;
}

static struct osal_fileops g_msensor_mng_fops = {
    .open = msensor_mng_open,
    .unlocked_ioctl = msensor_mng_ioctl,
    .release = msensor_mng_release,
};

struct osal_pmops g_msensor_mng_drv_ops = {
    .pm_freeze = msensor_mng_freeze,
    .pm_restore = msensor_mng_restore,
};

static xmedia_s32 msensor_mng_atomic_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_s32 i, j;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        ret = osal_atomic_init(&g_msensor_mng_user_ref[i]);
        if (ret != XMEDIA_SUCCESS) {
            osal_printk("atomic init failed. \n");
            goto failed;
        }
        osal_atomic_set(&g_msensor_mng_user_ref[i], 0);
    }
    return XMEDIA_SUCCESS;
failed:
    for (j = i - 1; j >= 0; j--) {
        osal_atomic_destory(&g_msensor_mng_user_ref[j]);
    }
    return ret;
}

static xmedia_void msensor_mng_atomic_exit(xmedia_void)
{
    xmedia_s32 i;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        osal_atomic_destroy(&g_msensor_mng_user_ref[i]);
    }
    return;
}

xmedia_s32 msensor_mng_module_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_s32 i;

#ifdef CONFIG_PROC_SHOW_SUPPORT
    ret = msensor_proc_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("msensor proc init failed\n");
        goto FAIL0;
    }
#endif

    g_msensor_mng_dev = osal_createdev(UMAP_DEVNANME_MSENSOR_MNG_BASE);
    if (g_msensor_mng_dev == XMEDIA_NULL) {
        osal_printk("msensor: create device failed\n");
        goto FAIL1;
    }

    g_msensor_mng_dev->fops = &g_msensor_mng_fops;
    g_msensor_mng_dev->minor = UMAP_MSENSOR_MNG_MINOR_BASE;
    g_msensor_mng_dev->osal_pmops = &g_msensor_mng_drv_ops;
    ret = osal_registerdevice(g_msensor_mng_dev);
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("register msensor device failed!\n");
        goto FAIL2;
    }

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        (xmedia_void) osal_memset(&g_mng_proc_info[i], 0, sizeof(g_mng_proc_info[i]));
    }

    ret = msensor_buf_lock_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("register msensor buf lock init failed!\n");
        goto FAIL3;
    }

    ret = msensor_mng_atomic_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("register msensor device failed!\n");
        goto FAIL4;
    }

    osal_printk("load xm_msensor_mng.ko OK!\n");
    return XMEDIA_SUCCESS;
FAIL4:
    msensor_buf_lock_exit();
FAIL3:
    osal_deregisterdevice(g_msensor_mng_dev);
FAIL2:
    osal_destroydev(g_msensor_mng_dev);
FAIL1:
#ifdef CONFIG_PROC_SHOW_SUPPORT
    msensor_proc_exit();
FAIL0:
#endif

    return XMEDIA_FAILURE;
}

/* if static, liteos warning */
xmedia_void msensor_mng_module_exit(xmedia_void)
{
    xmedia_s32 i;

    msensor_mng_atomic_exit();

    msensor_buf_lock_exit();

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        (xmedia_void) osal_memset(&g_mng_proc_info[i], 0, sizeof(g_mng_proc_info[i]));
    }

    osal_deregisterdevice(g_msensor_mng_dev);

    osal_destroydev(g_msensor_mng_dev);

#ifdef CONFIG_PROC_SHOW_SUPPORT
    msensor_proc_exit();
#endif
    osal_printk("unload msensor_mng.ko OK!\n");
}

#ifdef MODULE
EXPORT_SYMBOL(msensor_mng_buf_init);
EXPORT_SYMBOL(msensor_mng_buf_exit);
EXPORT_SYMBOL(msensor_mng_write_data_to_buf);
EXPORT_SYMBOL(msensor_mng_register_call_back);
EXPORT_SYMBOL(msensor_mng_unregister_call_back);

module_init(msensor_mng_module_init);
module_exit(msensor_mng_module_exit);

MODULE_DESCRIPTION("msensor driver");
MODULE_LICENSE("GPL");
#else

int __init msensor_mng_driver_init(void)
{
    return msensor_mng_module_init();
}
#endif
