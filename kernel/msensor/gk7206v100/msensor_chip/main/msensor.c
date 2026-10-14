/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor.h"
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>
#include "osal.h"
#include "mmz.h"
#include "msensor_ext.h"
#include "msensor_chip_ioctl.h"
#include "msensor.h"
#include "msensor_exe.h"
#include "icm42670.h"

#define UMAP_MSENSOR_CHIP_MINOR_BASE 153
#define UMAP_DEVNANME_MSENSOR_CHIP_BASE       "msensor_chip"

#if (defined CONFIG_PROC_SHOW_SUPPORT)
#include "msensor_chip_proc.h"
#endif

static osal_dev_t *g_msensor_chip_dev = XMEDIA_NULL;

static xmedia_s32 g_msensor_init[XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_FALSE};
static xmedia_bool g_msensor_start[XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_FALSE};
xmedia_msensor_param *g_msensor_param [XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_NULL};
static triger_config g_triger_data[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
static osal_atomic_t g_msensor_chip_user_ref[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

#define safe_kfree(memory)             \
    do {                               \
        if ((memory) != XMEDIA_NULL) { \
            osal_kfree(memory);        \
            memory = XMEDIA_NULL;      \
        }                              \
    } while (0)

xmedia_msensor_param *msensor_chip_get_param(xmedia_s32 dev)
{
    return g_msensor_param[dev];
}

xmedia_bool msensor_chip_get_status(xmedia_s32 dev)
{
    return g_msensor_start[dev];
}

#ifdef MNGBUFF_ENABLE
xmedia_s32 msensor_chip_int_callback(xmedia_s32 dev, xmedia_msensor_data *msensor_data)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (msensor_data == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d msensor_data is null\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    ret = msensor_mng_write_data_to_buf(dev, msensor_data);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d write data tobuf failed!(ret:0x%x)\n", dev, ret);
        return ret;
    }

    return ret;
}

static xmedia_s32 msensor_chip_init_mng_buf(xmedia_s32 dev, xmedia_msensor_attr msensor_attr,
    xmedia_msensor_buf_attr *msensor_buf_attr, xmedia_msensor_config *msensor_config)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((msensor_buf_attr == XMEDIA_NULL) || (msensor_config == XMEDIA_NULL)) {
        MSENSOR_ERR_TRACE("dev %d msensor_buf_attr or msensor_config is null!\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    ret = msensor_mng_buf_init(dev, &msensor_attr, msensor_buf_attr, msensor_config);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d init mng buff failed!(ret:0x%x)\n", dev, ret);
        return ret;
    }

    return ret;
}

static xmedia_s32 msensor_chip_exit_mng_buf(xmedia_s32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret = msensor_mng_buf_exit(dev);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d exit mng buff failed!(ret:0x%x)\n", dev, ret);
        return ret;
    }

    return ret;
}

 //todo，待确认，释放需要通过ioctl开放get cfg接口
 static xmedia_s32 msensor_chip_get_cfg(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
 {
     if (g_msensor_param[dev] == XMEDIA_NULL) {
         MSENSOR_ERR_TRACE("dev %d msensor_param is null\n", dev);
         return XMEDIA_ERRCODE_NULL_PTR;
     }

     if (msensor_param == XMEDIA_NULL) {
         MSENSOR_ERR_TRACE("dev %d input param is null\n", dev);
         return XMEDIA_ERRCODE_NULL_PTR;
     }

     osal_memcpy(msensor_param, g_msensor_param[dev], sizeof(xmedia_msensor_param));

     return XMEDIA_SUCCESS;
 }

 // todo，通过ioctl开放get data接口
 xmedia_s32 msensor_chip_get_data(xmedia_s32 dev)
 {
     xmedia_s32 ret;

     ret = chip_get_data_for_one_frm(dev);
     if (ret != XMEDIA_SUCCESS) {
         MSENSOR_ERR_TRACE("dev %d get data failed!\n", dev);
     }
     return ret;
 }

 static xmedia_s32 msensor_chip_register_mng_callback(xmedia_s32 dev)
{
    xmedia_s32 ret;

    msensor_mng_callback callback = { 0 };

    callback.pfn_get_config_from_chip = msensor_chip_get_cfg;
    callback.pfn_write_data_to_buf = msensor_chip_get_data;

    ret = msensor_mng_register_call_back(dev, &callback);
    if(ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d register msensor callback failed\n", dev);
        return XMEDIA_FAILURE;
    }

    return ret;
}

static xmedia_void msensor_chip_unregister_mng_callback(xmedia_s32 dev)
{
    msensor_mng_unregister_call_back(dev);

    return;
}

#endif

static xmedia_s32 msensor_chip_open(xmedia_void *private_data)
{
    xmedia_unused(private_data);
    MSENSOR_ERR_TRACE("msensor open\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_chip_release(xmedia_void *private_data)
{
    xmedia_unused(private_data);
    MSENSOR_ERR_TRACE("msensor close\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_chip_freeze(osal_dev_t *dev)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_chip_restore(osal_dev_t *dev)
{
    return XMEDIA_SUCCESS;
}

/* msensor chip ioctl functions */
static xmedia_s32 msensor_chip_drv_user_start(xmedia_s32 dev, xmedia_uintptr_t arg)
{
    if (g_msensor_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d msensor is not init!\n", dev);
        return XMEDIA_FAILURE;
    }

    if (g_msensor_start[dev] == XMEDIA_TRUE) {
        MSENSOR_ERR_TRACE("dev %d msensor is already start!\n", dev);
        return XMEDIA_SUCCESS;
    }

    xmedia_unused(arg);

    chip_fifo_data_reset(dev);

    osal_msleep(100); // 1.16 fix acc && gyro  pts not sync problem

    if (g_triger_data[dev].triger_mode == TRIGER_TIMER) {
        chip_timer_run(dev);
    } else if (g_triger_data[dev].triger_mode == TRIGER_EXTERN_INTERRUPT) {
        chip_interrupt_run(dev);
    } else {
        MSENSOR_ERR_TRACE("not triger_mode %d\n",g_triger_data[dev].triger_mode);
        return XMEDIA_FAILURE;
    }

    g_msensor_start[dev] = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_chip_drv_user_stop(xmedia_s32 dev, xmedia_uintptr_t arg)
{
    if (g_msensor_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d msensor is not init!\n", dev);
        return XMEDIA_FAILURE;
    }

    if (g_msensor_start[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d msensor is already stop!\n", dev);
        return XMEDIA_SUCCESS;
    }

    xmedia_unused(arg);

    if (g_triger_data[dev].triger_mode == TRIGER_TIMER) {
        chip_timer_stop(dev);
    } else if (g_triger_data[dev].triger_mode == TRIGER_EXTERN_INTERRUPT) {
        chip_interrupt_stop(dev);
    } else {
        MSENSOR_ERR_TRACE("not triger_mode %d\n",g_triger_data[dev].triger_mode);
        return XMEDIA_FAILURE;
    }

    g_msensor_start[dev] = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_chip_drv_user_init(xmedia_s32 dev, xmedia_uintptr_t arg)
{
    xmedia_s32 ret;
    xmedia_msensor_param *msensor_param;

    if (g_msensor_init[dev] == XMEDIA_TRUE) {
        MSENSOR_ERR_TRACE("dev %d msensor is already inited!\n", dev);
        return XMEDIA_FAILURE;
    }
    msensor_param = (xmedia_msensor_param *)(arg);
    ret = mmz_check_phyaddr(msensor_param->buf_attr.phys_addr, msensor_param->buf_attr.buf_len);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d check mem share failed, buf_attr.phys_addr is 0x%llx\n", dev,
                          msensor_param->buf_attr.phys_addr);
        return XMEDIA_FAILURE;
    }

    osal_memcpy(g_msensor_param[dev], (xmedia_void *)arg, sizeof(xmedia_msensor_param));

    if (((XMEDIA_MSENSOR_DEVICE_GYRO & g_msensor_param[dev]->attr.device_mask) &&
         (g_msensor_param[dev]->config.gyro_config.odr == 0)) ||
        ((XMEDIA_MSENSOR_DEVICE_ACC & g_msensor_param[dev]->attr.device_mask) &&
         (g_msensor_param[dev]->config.acc_config.odr == 0))) {
        MSENSOR_ERR_TRACE("dev %d msensor param err!\n", dev);
        return XMEDIA_FAILURE;
    }

    g_msensor_param[dev]->config.gyro_config.temperature_max = MOTIONSENSOR_MAX_TEMP;
    g_msensor_param[dev]->config.gyro_config.temperature_min = MOTIONSENSOR_MIN_TEMP;
    g_msensor_param[dev]->config.acc_config.temperature_max = MOTIONSENSOR_MAX_TEMP;
    g_msensor_param[dev]->config.acc_config.temperature_min = MOTIONSENSOR_MIN_TEMP;
    /* init senser */
    ret = chip_dev_init(dev, g_msensor_param[dev]);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d chip dev init failed! ret=%x\n", dev, ret);
        return ret;
    }

    ret = chip_get_triger_config(dev, &g_triger_data[dev]);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d imu get triger config failed! ret=%x\n", dev, ret);
        return ret;
    }

#ifdef MNGBUFF_ENABLE
    /* buff init, fix in this */
    msensor_chip_init_mng_buf(dev, g_msensor_param[dev]->attr, &g_msensor_param[dev]->buf_attr,
        &g_msensor_param[dev]->config);
#endif
    g_msensor_init[dev] = XMEDIA_TRUE;

    return ret;
}

static xmedia_s32 msensor_chip_drv_user_deinit(xmedia_s32 dev, xmedia_uintptr_t arg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (g_msensor_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d msensor has not inited!\n", dev);
        return XMEDIA_FAILURE;
    }

    if (g_msensor_start[dev] == XMEDIA_TRUE) {
        MSENSOR_ERR_TRACE("dev %d please stop msensor first!\n", dev);
        return XMEDIA_FAILURE;
    }

    xmedia_unused(arg);

    g_msensor_init[dev] = XMEDIA_FALSE;
    chip_dev_exit(dev, g_msensor_param[dev]);
#ifdef MNGBUFF_ENABLE
    ret = msensor_chip_exit_mng_buf(dev);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d exit mng buf failed! ret=%x\n", dev, ret);
        return ret;
    }
#endif
    return ret;
}

static xmedia_s32 msensor_chip_drv_user_get_param(xmedia_s32 dev, xmedia_uintptr_t arg)
{
    xmedia_s32 ret;

    ret = mmz_check_phyaddr(g_msensor_param[dev]->buf_attr.phys_addr, g_msensor_param[dev]->buf_attr.buf_len);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d check mem share failed, buf_attr.phys_addr is 0x%llx\n", dev,
            g_msensor_param[dev]->buf_attr.phys_addr);
        return ret;
    }

    osal_memcpy((xmedia_msensor_param *)arg, g_msensor_param[dev], sizeof(xmedia_msensor_param));

    return XMEDIA_SUCCESS;
}

/* only for debug: send and save yuv */
static xmedia_s32 msensor_chip_drv_user_mng_init(xmedia_s32 dev, xmedia_uintptr_t arg)
{
    xmedia_s32 ret;
    xmedia_msensor_param *msensor_param = XMEDIA_NULL;

    if (g_msensor_init[dev] == XMEDIA_TRUE) {
        MSENSOR_ERR_TRACE("dev %d msensor is already inited!\n", dev);
        return XMEDIA_FAILURE;
    }
    msensor_param = (xmedia_msensor_param *)(arg);
    ret = mmz_check_phyaddr(msensor_param->buf_attr.phys_addr, msensor_param->buf_attr.buf_len);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d check mem share failed, buf_attr.phys_addr is 0x%llx\n",
            dev, msensor_param->buf_attr.phys_addr);
        return ret;
    }

    osal_memcpy(g_msensor_param[dev], (xmedia_void *)arg, sizeof(xmedia_msensor_param));

    if (((XMEDIA_MSENSOR_DEVICE_GYRO & g_msensor_param[dev]->attr.device_mask) &&
         (g_msensor_param[dev]->config.gyro_config.odr == 0)) ||
        ((XMEDIA_MSENSOR_DEVICE_ACC & g_msensor_param[dev]->attr.device_mask) &&
         (g_msensor_param[dev]->config.acc_config.odr == 0))) {
        MSENSOR_ERR_TRACE("dev %d msensor param error\n", dev);
        return ret;
    }

    /* init senser */
    ret = chip_dev_mng_init(dev, g_msensor_param[dev]);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d mng init failed! ret=%x\n", dev, ret);
        return ret;
    }

#ifdef MNGBUFF_ENABLE
    /* buff init, fix in this */
    msensor_chip_init_mng_buf(dev, g_msensor_param[dev]->attr, &g_msensor_param[dev]->buf_attr,
                              &g_msensor_param[dev]->config);
#endif
    g_msensor_init[dev] = XMEDIA_TRUE;

    return ret;
}

static xmedia_s32 msensor_chip_drv_start(xmedia_s32 dev, xmedia_ulong arg)
{
    return msensor_chip_drv_user_start(dev, (xmedia_uintptr_t)arg);
}

static xmedia_s32 msensor_chip_drv_stop(xmedia_s32 dev, xmedia_ulong arg)
{
    return msensor_chip_drv_user_stop(dev, (xmedia_uintptr_t)arg);
}

static xmedia_s32 msensor_chip_drv_init(xmedia_s32 dev, xmedia_ulong arg)
{
    return msensor_chip_drv_user_init(dev, (xmedia_uintptr_t)arg);
}

static xmedia_s32 msensor_chip_drv_deinit(xmedia_s32 dev, xmedia_ulong arg)
{
    return msensor_chip_drv_user_deinit(dev, (xmedia_uintptr_t)arg);
}

static xmedia_s32 msensor_chip_drv_get_param(xmedia_s32 dev, xmedia_ulong arg)
{
    return msensor_chip_drv_user_get_param(dev, (xmedia_uintptr_t)arg);
}

/* only for debug: send and save yuv */
static xmedia_s32 msensor_chip_drv_mng_init(xmedia_s32 dev, xmedia_ulong arg)
{
    return msensor_chip_drv_user_mng_init(dev, (xmedia_uintptr_t)arg);
}

typedef xmedia_s32 (*msensor_ctl_ptr_func)(xmedia_s32 dev, xmedia_ulong arg);

typedef struct {
    xmedia_u32 cmd;
    msensor_ctl_ptr_func ptr_func;
} msensor_ioctl_func_item;

static msensor_ioctl_func_item g_msensor_chip_cmd_list[] = {
    { MSENSOR_CMD_START, msensor_chip_drv_start },
    { MSENSOR_CMD_STOP, msensor_chip_drv_stop },
    { MSENSOR_CMD_CREATE, msensor_chip_drv_init },
    { MSENSOR_CMD_DESTROY, msensor_chip_drv_deinit },
    { MSENSOR_CMD_GET_PARAM, msensor_chip_drv_get_param },
    { MSENSOR_CMD_MNG_INIT, msensor_chip_drv_mng_init }, /* only for debug: send and save yuv */
};

static xmedia_s32 msensor_chip_do_ioctl(xmedia_s32 dev, xmedia_u32 cmd, xmedia_ulong arg)
{
    xmedia_s32 i;

    for (i = 0; i < sizeof(g_msensor_chip_cmd_list) / sizeof(g_msensor_chip_cmd_list[0]); i++) {
        if (cmd == g_msensor_chip_cmd_list[i].cmd) {
            return g_msensor_chip_cmd_list[i].ptr_func(dev, arg);
        }
    }

    MSENSOR_ERR_TRACE("dev %d msensor chip ioctl cmd 0x%x not supported!\n", dev, cmd);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}
xmedia_slong msensor_chip_ioctl(xmedia_u32 cmd, xmedia_ulong arg, xmedia_void *private_data)
{
    xmedia_s32 ret;
    xmedia_s32 dev;

    if (cmd == MSENSOR_CMD_CHIP_BIND_FLAG2FD) {
        *(xmedia_u32 *)private_data = *(xmedia_u32 *)arg;
        return XMEDIA_SUCCESS;
    }

    dev = *(xmedia_u32 *)private_data;

    osal_atomic_inc_return(&g_msensor_chip_user_ref[dev]);

    ret = msensor_chip_do_ioctl(dev, cmd,  arg);

    osal_atomic_dec_return(&g_msensor_chip_user_ref[dev]);

    return ret;
}

static struct osal_fileops g_msensor_chip_fileop = {
    .open = msensor_chip_open,
    .unlocked_ioctl = msensor_chip_ioctl,
    .release = msensor_chip_release,
};

struct osal_pmops g_msensor_chip_drv_ops = {
    .pm_freeze = msensor_chip_freeze,
    .pm_restore = msensor_chip_restore,
};

static xmedia_s32 msensor_chip_param_init(xmedia_void)
{
    xmedia_s32 i ,j;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        g_msensor_param[i] = (xmedia_msensor_param *)osal_kmalloc(sizeof(xmedia_msensor_param), osal_gfp_kernel);
        if (g_msensor_param[i] == XMEDIA_NULL) {
            osal_printk("kmalloc memory failed\n");
            goto failed;
        }

        (xmedia_void) osal_memset(g_msensor_param[i], 0, sizeof(xmedia_msensor_param));
        g_msensor_start[i] = XMEDIA_FALSE;
    }
    return XMEDIA_SUCCESS;

failed:
    for (j = i - 1; j >= 0; j--) {
        osal_kfree(g_msensor_param[j]);
    }
    return -ENOMEM;
}

static xmedia_s32 msensor_chip_param_exit(xmedia_void)
{
    xmedia_s32 i;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        if(g_msensor_param[i] == XMEDIA_NULL) {
            osal_printk("msensor_param is null, nothing to exit \n");
        } else {
            osal_kfree(g_msensor_param[i]);
            g_msensor_param[i] = XMEDIA_NULL;
        }
        g_msensor_start[i] = XMEDIA_FALSE;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_chip_sensor_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_s32 i , j;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        ret = chip_sensor_init(i);
        if (ret != XMEDIA_SUCCESS) {
            osal_printk("init sensor! failed ret=%x\n", ret);
            goto failed;
        }
    }
    return XMEDIA_SUCCESS;
failed:
    for (j = i - 1; j >= 0; j--) {
        chip_sensor_exit(j);
    }
    return ret;

}

static xmedia_void msensor_chip_sensor_exit(xmedia_void)
{
    xmedia_s32 i;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        chip_sensor_exit(i);
    }
    return;
}

static xmedia_s32 msensor_chip_register_mng_callback_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_s32 i;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        ret = msensor_chip_register_mng_callback(i);
        if (ret != XMEDIA_SUCCESS) {
            osal_printk("register mng callback failed! ret=%x\n", ret);
            return XMEDIA_FAILURE;
        }
    }
    return XMEDIA_SUCCESS;
}

static xmedia_void msensor_chip_register_mng_callback_exit(xmedia_void)
{
    xmedia_s32 i;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        msensor_chip_unregister_mng_callback(i);

    }
    return;
}

static xmedia_s32 msensor_chip_atomic_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_s32 i ,j;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        ret = osal_atomic_init(&g_msensor_chip_user_ref[i]);
        if (ret != XMEDIA_SUCCESS) {
            osal_printk("atomic init failed. \n");
            goto  failed;
        }
        osal_atomic_set(&g_msensor_chip_user_ref[i], 0);
    }
    return XMEDIA_SUCCESS;
failed:
    for (j = i - 1; j >= 0; j--) {
        osal_atomic_destory(&g_msensor_chip_user_ref[j]);
    }
    return ret;
}

static xmedia_void msensor_chip_atomic_exit(xmedia_void)
{
    xmedia_s32 i;

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        osal_atomic_destroy(&g_msensor_chip_user_ref[i]);
    }
    return;
}

xmedia_s32 msensor_chip_module_init(xmedia_void)
{
    xmedia_s32 ret;

    ret = msensor_chip_param_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("init param failed! ret=%x\n", ret);
        goto FAIL0;
    }

    ret = msensor_chip_sensor_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("init sensor failed! ret=%x\n", ret);
        goto FAIL1;
    }

#ifdef MNGBUFF_ENABLE
    ret = msensor_chip_register_mng_callback_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("init sensor failed! ret=%x\n", ret);
        goto FAIL2;
    }
#endif

    ret = msensor_chip_atomic_init();
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("init sensor failed! ret=%x\n", ret);
        goto FAIL3;
    }

    g_msensor_chip_dev = osal_createdev(UMAP_DEVNANME_MSENSOR_CHIP_BASE);
    if (g_msensor_chip_dev == XMEDIA_NULL) {
        osal_printk("msensor: create device failed\n");
        goto FAIL4;
    }

    g_msensor_chip_dev->fops = &g_msensor_chip_fileop;
    g_msensor_chip_dev->minor = UMAP_MSENSOR_CHIP_MINOR_BASE;
    g_msensor_chip_dev->osal_pmops = &g_msensor_chip_drv_ops;
    ret = osal_registerdevice(g_msensor_chip_dev);
    if (ret != XMEDIA_SUCCESS) {
        osal_printk("register msensor device failed!\n");
        goto FAIL5;
    }

#if (defined CONFIG_PROC_SHOW_SUPPORT) && (defined ICM42670_PARAM_PROC)
    ret = mpu_proc_init();
    if (ret != XMEDIA_SUCCESS) {
        goto FAIL6;
    }
#endif
    osal_printk("load xm_msensor_chip.ko OK!\n");
    return XMEDIA_SUCCESS;

FAIL6:
    osal_deregisterdevice(g_msensor_chip_dev);
FAIL5:
    osal_destroydev(g_msensor_chip_dev);
FAIL4:
    msensor_chip_atomic_exit();
FAIL3:
    msensor_chip_register_mng_callback_exit();
FAIL2:
    msensor_chip_sensor_exit();
FAIL1:
    msensor_chip_param_exit();
FAIL0:
    osal_printk("load xm_msensor_chip.ko failed !\n");
    return XMEDIA_FAILURE;
}

xmedia_void msensor_chip_module_exit(xmedia_void)
{
    msensor_chip_atomic_exit();
#ifdef MNGBUFF_ENABLE
    msensor_chip_register_mng_callback_exit();
#endif
    msensor_chip_sensor_exit();
    msensor_chip_param_exit();
    osal_deregisterdevice(g_msensor_chip_dev);
    osal_destroydev(g_msensor_chip_dev);

#if (defined CONFIG_PROC_SHOW_SUPPORT)
    mpu_proc_exit();
#endif

    osal_printk("unload xm_msensor_chip.ko OK!\n");
}

#ifdef MODULE
module_init(msensor_chip_module_init);
module_exit(msensor_chip_module_exit);

MODULE_AUTHOR("otlicon");
MODULE_DESCRIPTION("motion_sensor driver");
MODULE_LICENSE("GPL");
#else
int __init msensor_chip_driver_init(void)
{
    return msensor_chip_module_init();
}
#endif

