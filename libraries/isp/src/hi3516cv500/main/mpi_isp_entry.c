/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mpi_isp_entry.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <stdio.h>
#include <string.h>

#include <sys/ioctl.h>
#include <signal.h>

#include "mpi_sys.h"
#include "mkp_isp.h"
#include "hi_isp_debug.h"
#include "isp_debug.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_regcfg.h"
#include "isp_vreg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "hi_osal.h"
#include "isp_inner.h"
#include "mpi_isp_adapt.h"

/* GLOBAL VARIABLES */
isp_usr_ctx g_isp_ctx[ISP_MAX_PIPE_NUM] = {{0}};
hi_s32      g_isp_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

hi_u16 g_res_w_max[ISP_MAX_PIPE_NUM] = {ISP_RES_WIDTH_MAX};
hi_u16 g_res_h_max[ISP_MAX_PIPE_NUM] = {ISP_RES_HEIGHT_MAX};

#define isp_check_isp_wdr_cfg(dev)                                            \
    do {                                                                      \
        if (g_isp_ctx[dev].para_rec.wdr_cfg != HI_TRUE) {                     \
            isp_err_trace("Isp[%d] WDR mode doesn't config!\n", dev);         \
            return HI_ERR_ISP_ATTR_NOT_CFG;                                   \
        }                                                                     \
    } while (0)

#define isp_check_isp_pub_attr_cfg(dev)                                       \
    do {                                                                      \
        if (g_isp_ctx[dev].para_rec.pub_cfg != HI_TRUE) {                     \
            isp_err_trace("Isp[%d] pub attr doesn't config!\n", dev);         \
            return HI_ERR_ISP_ATTR_NOT_CFG;                                   \
        }                                                                     \
    } while (0)

#define isp_check_isp_init(dev)                                             \
    do {                                                                    \
        if (g_isp_ctx[dev].para_rec.init != HI_TRUE) {                      \
            isp_err_trace("Isp[%d] doesn't initialized!\n", dev);           \
            return HI_ERR_ISP_NOT_INIT;                                     \
        }                                                                   \
    } while (0)

/* MACRO DEFINITION */
#define ISP_VERSION_MAGIC 20130305

hi_s32 isp_check_dev_open(hi_isp_dev dev)
{
    if (g_isp_fd[dev] <= 0) {
        hi_u32 arg = (dev);
        g_isp_fd[dev] = open("/dev/isp_dev", O_RDONLY, S_IRUSR);
        if (g_isp_fd[dev] < 0) {
            perror("open isp device error!\n");
            return HI_ERR_ISP_NOT_INIT;
        }
        if (ioctl(g_isp_fd[dev], ISP_DEV_SET_FD, &arg)) {
            close(g_isp_fd[dev]);
            g_isp_fd[dev] = -1;
            return HI_ERR_ISP_NOT_INIT;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_check_mem_init_func(hi_isp_dev dev)
{
    if (g_isp_ctx[dev].mem_init != HI_TRUE) {
        if (ioctl(g_isp_fd[dev], ISP_MEM_INFO_GET, &g_isp_ctx[dev].mem_init)) {
            isp_err_trace("ISP[%d] get mem info failed!\n", dev);
            return HI_ERR_ISP_MEM_NOT_INIT;
        }
        if (g_isp_ctx[dev].mem_init != HI_TRUE) {
            isp_err_trace("ISP[%d] mem NOT init %d!\n", dev, g_isp_ctx[dev].mem_init);
            return HI_ERR_ISP_MEM_NOT_INIT;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_fd(hi_vi_pipe vi_pipe)
{
    return g_isp_fd[vi_pipe];
}

isp_usr_ctx *isp_get_usr_ctx(hi_vi_pipe vi_pipe)
{
    return &g_isp_ctx[vi_pipe];
}

hi_bool isp_check_hrs_on(hi_vi_pipe vi_pipe)
{
    return (g_isp_ctx[vi_pipe].isp0_p2_en == HI_TRUE);
}

hi_u16 isp_get_max_width(hi_vi_pipe vi_pipe)
{
    return g_res_w_max[vi_pipe];
}

hi_u16 isp_get_max_height(hi_vi_pipe vi_pipe)
{
    return g_res_h_max[vi_pipe];
}

/*
 * prototype   : HI_MPI_ISP_SetModParam
 * description : isp module parameter
 */
HI_S32 HI_MPI_ISP_SetModParam(const ISP_MOD_PARAM_S *pstModParam)
{
    MPI_ASSERT(sizeof(ISP_MOD_PARAM_S) == sizeof(hi_isp_mod_param));
    return hi_mpi_isp_set_mod_param((hi_isp_mod_param *)pstModParam);
}

HI_S32 HI_MPI_ISP_GetModParam(ISP_MOD_PARAM_S *pstModParam)
{
    MPI_ASSERT(sizeof(ISP_MOD_PARAM_S) == sizeof(hi_isp_mod_param));
    return hi_mpi_isp_get_mod_param((hi_isp_mod_param *)pstModParam);
}

hi_s32 hi_mpi_isp_mem_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;

    /* check status */
    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);
    isp_check_open_return(vi_pipe);

    if (ioctl(g_isp_fd[vi_pipe], ISP_MEM_INFO_GET, &isp_ctx->mem_init)) {
        isp_err_trace("ISP[%d] get mem info failed!\n", vi_pipe);
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    if (isp_ctx->mem_init == HI_TRUE) {
        ret = isp_exit(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] exit failed!\n", vi_pipe);
            return ret;
        }
    }

    /* WDR attribute */
    ret = ioctl(g_isp_fd[vi_pipe], ISP_GET_WDR_ATTR, &isp_ctx->wdr_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get WDR attr failed\n", vi_pipe);
        return ret;
    }

    isp_check_wdr_main_pipe_return(isp_ctx->wdr_attr.wdr_mode, isp_ctx->wdr_attr.mast_pipe); /* wdr mode abnormal */

    /* creat extern registers */
    ret = vreg_init(vi_pipe, isp_vir_reg_base(vi_pipe), ISP_VREG_SIZE);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init mem failed\n", vi_pipe);
        return ret;
    }

    ret = isp_mem_init_update_ctx(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail0;
    }

    ret = isp_check_sns_register(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail1;
    }

    ret = isp_mem_info_set(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail1;
    }

    return HI_SUCCESS;
fail1:
    isp_sensor_unreg_callback(vi_pipe);
fail0:
    vreg_exit(vi_pipe, isp_vir_reg_base(vi_pipe), ISP_VREG_SIZE);
    return ret;
}

/*
 * prototype       : HI_MPI_ISP_MemInit
 * description     : isp initial extent memory
 */
HI_S32 HI_MPI_ISP_MemInit(VI_PIPE ViPipe)
{
    return hi_mpi_isp_mem_init(ViPipe);
}

hi_s32 hi_mpi_isp_init(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    /* check status */
    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);
    isp_check_open_return(vi_pipe);
    if (isp_ctx->isp_yuv_mode == HI_FALSE) {
        isp_check_sensor_register_return(vi_pipe);
    }
    isp_check_mem_init_return(vi_pipe);
    isp_check_wdr_main_pipe_return(isp_ctx->wdr_attr.wdr_mode, isp_ctx->wdr_attr.mast_pipe); /* wdr mode abnormal */

    isp_ctx->para_rec.wdr_cfg = hi_ext_top_wdr_cfg_read(vi_pipe);
    isp_ctx->para_rec.pub_cfg = hi_ext_top_pub_attr_cfg_read(vi_pipe);
    isp_check_isp_wdr_cfg(vi_pipe);
    isp_check_isp_pub_attr_cfg(vi_pipe);

    if (isp_ctx->para_rec.init == HI_TRUE) {
        isp_err_trace("ISP[%d] init failed!\n", vi_pipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return isp_init(vi_pipe);
}

/*
 * prototype       : HI_MPI_ISP_Init
 * description     : isp initial process, include extent memory, top structure,
 *                 default value, etc.
 */
HI_S32 HI_MPI_ISP_Init(VI_PIPE ViPipe)
{
    return hi_mpi_isp_init(ViPipe);
}

hi_s32 hi_isp_yuv_run_once(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);
    mutex_lock(isp_ctx->lock);

    if (isp_ctx->para_rec.run == HI_TRUE) {
        isp_err_trace("ISP[%d] is already running !\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* sometimes HI_MPI_ISP_Run thread is not scheduled to run before calling HI_MPI_ISP_Exit. */
    if (isp_ctx->para_rec.run_en == HI_FALSE) {
        mutex_unlock(isp_ctx->lock);
        return HI_SUCCESS;
    }

    isp_ctx->para_rec.run = HI_TRUE;
    isp_ctx->sns_wdr_mode = hi_ext_system_sensor_wdr_mode_read(vi_pipe);

    isp_ctx->linkage.run_once = HI_TRUE;
    ret = ioctl(g_isp_fd[vi_pipe], ISP_YUV_RUNONCE_INFO, &isp_ctx->linkage.run_once);
    if (ret != HI_SUCCESS) {
        isp_ctx->linkage.run_once = HI_FALSE;
        isp_ctx->para_rec.run  = HI_FALSE;
        isp_err_trace("ISP[%d] set runonce info failed!\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return ret;
    }

    isp_run(vi_pipe);

    ret = ioctl(g_isp_fd[vi_pipe], ISP_KERNEL_YUV_RUNONCE);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] kernel runonce  failed!\n", vi_pipe);
        isp_ctx->para_rec.run  = HI_FALSE;
        isp_ctx->linkage.run_once = HI_FALSE;
        ioctl(g_isp_fd[vi_pipe], ISP_YUV_RUNONCE_INFO, &isp_ctx->linkage.run_once);
        mutex_unlock(isp_ctx->lock);
        return ret;
    }

    isp_ctx->para_rec.run = HI_FALSE;

    mutex_unlock(isp_ctx->lock);

    return HI_SUCCESS;
}

/* when offline mode user send raw to BE, firstly need call this function to insure paramters ready */
hi_s32 hi_isp_run_once(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_running_mode isp_run_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    /* online mode not support */
    isp_run_mode = isp_ctx->block_attr.running_mode;
    if ((is_online_mode(isp_run_mode)) || (is_sidebyside_mode(isp_run_mode))) {
        isp_err_trace("ISP[%d] run_once not support for online!\n", vi_pipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    mutex_lock(isp_ctx->lock);

    if (isp_ctx->para_rec.run == HI_TRUE) {
        isp_err_trace("ISP[%d] run failed!\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* sometimes HI_MPI_ISP_Run thread is not scheduled to run before calling HI_MPI_ISP_Exit. */
    if (isp_ctx->para_rec.run_en == HI_FALSE) {
        mutex_unlock(isp_ctx->lock);
        return HI_SUCCESS;
    }

    isp_ctx->para_rec.run = HI_TRUE;

    /* change image mode (WDR mode or resolution) */
    ret = isp_switch_mode(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] switch mode failed!\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return ret;
    }

    isp_ctx->linkage.run_once = HI_TRUE;
    ret = ioctl(g_isp_fd[vi_pipe], ISP_OPT_RUNONCE_INFO, &isp_ctx->linkage.run_once);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set runonce info failed!\n", vi_pipe);
        goto fail0;
    }

    isp_run(vi_pipe);

    ret = ioctl(g_isp_fd[vi_pipe], ISP_KERNEL_RUNONCE);
    if (ret != HI_SUCCESS) {
        goto fail0;
    }

    isp_ctx->para_rec.run = HI_FALSE;
    mutex_unlock(isp_ctx->lock);

    return HI_SUCCESS;

fail0:
    isp_ctx->linkage.run_once = HI_FALSE;
    if (ioctl(g_isp_fd[vi_pipe], ISP_OPT_RUNONCE_INFO, &isp_ctx->linkage.run_once)) {
        isp_err_trace("ISP[%d] set runonce info failed!\n", vi_pipe);
    }
    isp_ctx->para_rec.run = HI_FALSE;
    mutex_unlock(isp_ctx->lock);

    return ret;
}

hi_s32 hi_mpi_isp_run_once(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_s32 ret;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);

    /* check status */
    isp_check_open_return(vi_pipe);
    if (isp_ctx->isp_yuv_mode == HI_FALSE) {
        isp_check_sensor_register_return(vi_pipe);
    }
    isp_check_mem_init_return(vi_pipe);
    isp_check_isp_init(vi_pipe);
    isp_check_wdr_main_pipe_return(isp_ctx->wdr_attr.wdr_mode, isp_ctx->wdr_attr.mast_pipe); /* wdr mode abnormal */

    if (isp_ctx->isp_yuv_mode == HI_FALSE) {
        ret = hi_isp_run_once(vi_pipe);
    } else {
        ret = hi_isp_yuv_run_once(vi_pipe);
    }

    return ret;
}

HI_S32 HI_MPI_ISP_RunOnce(VI_PIPE ViPipe)
{
    return hi_mpi_isp_run_once(ViPipe);
}

hi_s32 hi_mpi_isp_run(hi_vi_pipe vi_pipe)
{
    hi_bool en;
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;

    /* check status */
    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);
    isp_check_open_return(vi_pipe);
    isp_check_sensor_register_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);
    isp_check_isp_init(vi_pipe);
    isp_check_wdr_main_pipe_return(isp_ctx->wdr_attr.wdr_mode, isp_ctx->wdr_attr.mast_pipe); /* wdr mode abnormal */

    if (isp_ctx->para_rec.run == HI_TRUE) {
        isp_err_trace("ISP[%d] run failed!\n", vi_pipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    mutex_lock(isp_ctx->lock);

    /* sometimes ISP run thread is not scheduled to run before calling ISP exit. */
    if (isp_ctx->para_rec.run_en == HI_FALSE) {
        mutex_unlock(isp_ctx->lock);
        return HI_SUCCESS;
    }

    /* enable interrupt */
    en = HI_TRUE;
    if (ioctl(g_isp_fd[vi_pipe], ISP_SET_INT_ENABLE, &en) < 0) {
        isp_err_trace("Enable ISP[%d] interrupt failed!\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return HI_FAILURE;
    }

    isp_ctx->para_rec.run = HI_TRUE;
    mutex_unlock(isp_ctx->lock);

    while (1) {
        ret = isp_run_thread_proc(vi_pipe);
        if (ret != HI_SUCCESS) {
            break;
        }
    }

    /* disable interrupt */
    en = HI_FALSE;
    if (ioctl(g_isp_fd[vi_pipe], ISP_SET_INT_ENABLE, &en) < 0) {
        isp_err_trace("Disable ISP[%d] interrupt failed!\n", vi_pipe);
    }

    return HI_SUCCESS;
}

/*
 * prototype       : HI_MPI_ISP_Run
 * description     : isp firmware recurrent task, always run in a single thread.
 */
HI_S32 HI_MPI_ISP_Run(VI_PIPE ViPipe)
{
    return hi_mpi_isp_run(ViPipe);
}

hi_s32 hi_mpi_isp_exit(hi_vi_pipe vi_pipe)
{
    hi_s32  ret;
    hi_bool enable = HI_FALSE;
    isp_usr_ctx *isp_ctx = HI_NULL;

    /* check status */
    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);
    isp_check_wdr_main_pipe_return(isp_ctx->wdr_attr.wdr_mode, isp_ctx->wdr_attr.mast_pipe); /* wdr mode abnormal */

    isp_stitch_sync_exit(vi_pipe);
    mutex_lock(isp_ctx->lock);
    isp_alg_en_exit(vi_pipe);
    if (ioctl(g_isp_fd[vi_pipe], ISP_SET_INT_ENABLE, &enable) < 0) {
        isp_err_trace("Disable ISP[%d] interrupt failed!\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    ret = isp_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] exit failed!\n", vi_pipe);
    }

    isp_libs_unregister(vi_pipe);
    mutex_unlock(isp_ctx->lock);
    mutex_destroy(isp_ctx->lock);

    return ret;
}

/*
 * prototype       : HI_MPI_ISP_Exit
 * description     : control isp to exit recurrent task, always run in main process.
 */
HI_S32 HI_MPI_ISP_Exit(VI_PIPE ViPipe)
{
    return hi_mpi_isp_exit(ViPipe);
}

/*
 * prototype       : HI_MPI_ISP_SetRegister
 * description     : set isp register, include extent memory.
 */
HI_S32 HI_MPI_ISP_SetRegister(VI_PIPE ViPipe, HI_U32 u32Addr, HI_U32 u32Value)
{
    return hi_mpi_isp_set_register(ViPipe, u32Addr, u32Value);
}

/*
 * prototype       : HI_MPI_ISP_GetRegister
 * description     : get isp register, include extent memory.
 */
HI_S32 HI_MPI_ISP_GetRegister(VI_PIPE ViPipe, HI_U32 u32Addr, HI_U32 *pu32Value)
{
    return hi_mpi_isp_get_register(ViPipe, u32Addr, pu32Value);
}

HI_S32 HI_MPI_ISP_SetSnsSlaveAttr(SLAVE_DEV SlaveDev, const ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    MPI_ASSERT(sizeof(ISP_SLAVE_SNS_SYNC_S) == sizeof(hi_isp_slave_sns_sync));
    return hi_mpi_isp_set_sns_slave_attr(SlaveDev, (hi_isp_slave_sns_sync *)pstSnsSync);
}

HI_S32 HI_MPI_ISP_GetSnsSlaveAttr(SLAVE_DEV SlaveDev, ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    MPI_ASSERT(sizeof(ISP_SLAVE_SNS_SYNC_S) == sizeof(hi_isp_slave_sns_sync));
    return hi_mpi_isp_get_sns_slave_attr(SlaveDev, (hi_isp_slave_sns_sync *)pstSnsSync);
}

HI_S32 HI_MPI_ISP_SensorRegCallBack(VI_PIPE ViPipe, ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
                                    ISP_SENSOR_REGISTER_S *pstRegister)
{
    MPI_ASSERT(sizeof(ISP_SNS_ATTR_INFO_S) == sizeof(hi_isp_sns_attr_info));
    MPI_ASSERT(sizeof(ISP_SENSOR_REGISTER_S) == sizeof(hi_isp_sensor_register));
    return hi_mpi_isp_sensor_reg_callback(ViPipe, (hi_isp_sns_attr_info *)pstSnsAttrInfo,
                                          (hi_isp_sensor_register *)pstRegister);
}

HI_S32 HI_MPI_ISP_AELibRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                   ISP_AE_REGISTER_S *pstRegister)
{
    MPI_ASSERT(sizeof(ALG_LIB_S) == sizeof(hi_isp_alg_lib));
    MPI_ASSERT(sizeof(ISP_AE_REGISTER_S) == sizeof(hi_isp_ae_register));
    return hi_mpi_isp_ae_lib_reg_callback(ViPipe, (hi_isp_alg_lib *)pstAeLib, (hi_isp_ae_register *)pstRegister);
}

HI_S32 HI_MPI_ISP_AWBLibRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
                                    ISP_AWB_REGISTER_S *pstRegister)
{
    MPI_ASSERT(sizeof(ALG_LIB_S) == sizeof(hi_isp_alg_lib));
    MPI_ASSERT(sizeof(ISP_AWB_REGISTER_S) == sizeof(hi_isp_awb_register));
    return hi_mpi_isp_awb_lib_reg_callback(ViPipe, (hi_isp_alg_lib *)pstAwbLib, (hi_isp_awb_register *)pstRegister);
}

HI_S32 HI_MPI_ISP_SensorUnRegCallBack(VI_PIPE ViPipe, SENSOR_ID SensorId)
{
    return hi_mpi_isp_sensor_unreg_callback(ViPipe, SensorId);
}

HI_S32 HI_MPI_ISP_AELibUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib)
{
    MPI_ASSERT(sizeof(ALG_LIB_S) == sizeof(hi_isp_alg_lib));
    return hi_mpi_isp_ae_lib_unreg_callback(ViPipe, (hi_isp_alg_lib *)pstAeLib);
}

HI_S32 HI_MPI_ISP_AWBLibUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib)
{
    MPI_ASSERT(sizeof(ALG_LIB_S) == sizeof(hi_isp_alg_lib));
    return hi_mpi_isp_awb_lib_unreg_callback(ViPipe, (hi_isp_alg_lib *)pstAwbLib);
}

HI_S32 HI_MPI_ISP_SetBindAttr(VI_PIPE ViPipe, const ISP_BIND_ATTR_S *pstBindAttr)
{
    MPI_ASSERT(sizeof(ISP_BIND_ATTR_S) == sizeof(hi_isp_bind_attr));
    return hi_mpi_isp_set_bind_attr(ViPipe, (hi_isp_bind_attr *)pstBindAttr);
}

HI_S32 HI_MPI_ISP_GetBindAttr(VI_PIPE ViPipe, ISP_BIND_ATTR_S *pstBindAttr)
{
    MPI_ASSERT(sizeof(ISP_BIND_ATTR_S) == sizeof(hi_isp_bind_attr));
    return hi_mpi_isp_get_bind_attr(ViPipe, (hi_isp_bind_attr *)pstBindAttr);
}

HI_S32 HI_MPI_ISP_GetVDTimeOut(VI_PIPE ViPipe, ISP_VD_TYPE_E enIspVDType, HI_U32 u32MilliSec)
{
    MPI_ASSERT(sizeof(ISP_VD_TYPE_E) == sizeof(hi_isp_vd_type));
    return hi_mpi_isp_get_vd_time_out(ViPipe, (hi_isp_vd_type)enIspVDType, u32MilliSec);
}

/*
 * prototype       : HI_MPI_ISP_SetDCFInfo
 * description     : set dcf info to isp firmware
 */
HI_S32 HI_MPI_ISP_SetDCFInfo(VI_PIPE ViPipe, const ISP_DCF_INFO_S *pstIspDCF)
{
    MPI_ASSERT(sizeof(ISP_DCF_INFO_S) == sizeof(hi_isp_dcf_info));
    return hi_mpi_isp_set_dcf_info(ViPipe, (hi_isp_dcf_info *)pstIspDCF);
}

/*
 * prototype       : HI_MPI_ISP_GetDCFInfo
 * description     : get dcf info from isp firmware
 */
HI_S32 HI_MPI_ISP_GetDCFInfo(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF)
{
    MPI_ASSERT(sizeof(ISP_DCF_INFO_S) == sizeof(hi_isp_dcf_info));
    return hi_mpi_isp_get_dcf_info(ViPipe, (hi_isp_dcf_info *)pstIspDCF);
}

HI_S32 HI_MPI_ISP_SetFrameInfo(VI_PIPE ViPipe, const ISP_FRAME_INFO_S *pstIspFrame)
{
    MPI_ASSERT(sizeof(ISP_FRAME_INFO_S) == sizeof(hi_isp_frame_info));
    return hi_mpi_isp_set_frame_info(ViPipe, (hi_isp_frame_info *)pstIspFrame);
}

HI_S32 HI_MPI_ISP_GetFrameInfo(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspFrame)
{
    MPI_ASSERT(sizeof(ISP_FRAME_INFO_S) == sizeof(hi_isp_frame_info));
    return hi_mpi_isp_get_frame_info(ViPipe, (hi_isp_frame_info *)pstIspFrame);
}

