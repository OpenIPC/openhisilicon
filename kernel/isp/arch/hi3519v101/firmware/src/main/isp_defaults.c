/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_defaults.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   :
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_defaults.h"
#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_VOID ISP_RegsDefault(ISP_DEV IspDev)
{
    /* Statistics after static WB */
    //#define HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB (0)
    //#define HI_ISP_TOP_AE_SWITCH_AFTER_TPG_CHANNEL1 (1)
    //#define HI_ISP_TOP_AE_SWITCH_AFTER_SHADING (2)
    //#define HI_ISP_TOP_AE_SWITCH_AFTER_WDR_STITCH (3)
    hi_isp_top_ae_switch_write(IspDev, HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB);
    hi_isp_top_aesum_switch_write(IspDev, HI_ISP_TOP_AESUM_SWITCH_AFTER_STATIC_WB);
    hi_isp_top_histogram_switch_write(IspDev, HI_ISP_TOP_HISTOGRAM_SWITCH_SAME_AS_AE);

    /* top bypass functions */
    hi_isp_top_bypass_green_equalize_write(IspDev, HI_ISP_TOP_BYPASS_GREEN_EQUALIZE_DEFAULT);
    hi_isp_top_bypass_color_matrix_write(IspDev, HI_ISP_TOP_BYPASS_COLOR_MATRIX_DEFAULT);
    hi_isp_top_bypass_debuger_write(IspDev, HI_ISP_TOP_BYPASS_DEBUGER_DEFAULT);
    hi_isp_top_bypass_logo_write(IspDev, HI_ISP_TOP_BYPASS_LOGO_DEFAULT);
    hi_isp_top_bypass_sharpen_write(IspDev, HI_ISP_TOP_BYPASS_SHARPEN_DEFAULT);

    /* Set low_power_consumption_config */
    hi_isp_low_power_consumption_config_write(IspDev, HI_ISP_LOW_POWER_CONSUMPTION_CONFIG_DEFAULT);

    /* Set interrupt */
    hi_isp_interrupts_interrupt0_source_write(IspDev, 0);
    hi_isp_interrupts_interrupt1_source_write(IspDev, 0);

    /* #define HI_ISP_TOP_CONFIG_BUFFER_MODE_GLOBAL_ALL_MODULE_CONFIG_UPDATED_DURING_ISP_VERTICAL_BLANKING (3) */
    hi_isp_top_config_buffer_mode_write(IspDev, 3);

    return;
}

HI_VOID ISP_RegsInitialize(ISP_DEV IspDev)
{
    return;
}

/* The ext regs is a block of memory, not real register, need a default value. */
HI_VOID ISP_ExtRegsDefault(ISP_DEV IspDev)
{
    HI_U8 i;

    hi_ext_system_set_1001_rate_divider_write(IspDev, HI_EXT_SYSTEM_SET_1001_RATE_DIVIDER_DEFAULT);
    hi_ext_system_half_pixel_clock_write(IspDev, HI_EXT_SYSTEM_HALF_PIXEL_CLOCK_DEFAULT);
    hi_ext_system_freeze_firmware_write(IspDev, HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT);
    hi_ext_system_low_light_enable_write(IspDev, HI_EXT_SYSTEM_LOW_LIGHT_ENABLE_DEFAULT);

    hi_ext_system_sys_debug_enable_write(IspDev, HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT);
    hi_ext_system_sys_debug_addr_write(IspDev, HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DEFAULT);
    hi_ext_system_sys_debug_size_write(IspDev, HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT);
    hi_ext_system_sys_debug_depth_write(IspDev, HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT);

    hi_ext_system_statistics_ctrl_write(IspDev, HI_EXT_SYSTEM_STATISTICS_DEFAULT);

    hi_ext_sns_delay_num_ctrl_write(IspDev, HI_EXT_SNS_DELAY_NUM_CTRL_USE_CMOS);
    for (i=0; i<HI_EXT_SNS_DELAY_NUM_SIZE; i++)
    {
        hi_ext_sns_delay_num_write(IspDev, i, HI_EXT_SNS_DELAY_NUM_DEFAULT);
    }

    for(i = 0; i < 4; i++)
    {
        hi_ext_system_isp_pipe_diff_offset_write(IspDev, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT);
        hi_ext_system_isp_pipe_diff_gain_write(IspDev, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT);
    }

    for(i = 0; i < 9; i++)
    {
        if(i%4 == 0)
        {
            hi_ext_system_isp_pipe_diff_ccm_write(IspDev, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT);
        }
        else
        {
            hi_ext_system_isp_pipe_diff_ccm_write(IspDev, i, 0);
        }
    }
    /*Snap and HDR */
    hi_ext_system_snap_type_write(IspDev, HI_EXT_SYSTEM_SNAP_TYPE_DEFAULT);
    hi_ext_system_snap_pipe_mode_write(IspDev, HI_EXT_SYSTEM_SNAP_PIPE_MODE_DEFAULT);
    hi_ext_system_pro_type_write(IspDev, HI_EXT_SYSTEM_PRO_TYPE_DEFAULT);
    hi_ext_system_pro_frame_num_write(IspDev, HI_EXT_SYSTEM_PRO_FRAME_NUM_DEFAULT);
    hi_ext_system_snap_use_info_ccm_enable_write(IspDev, HI_EXT_SYSTEM_SNAP_USE_INFO_CCM_ENABLE_DEFAULT);
    for (i=0; i<PRO_MAX_FRAME_NUM; i++)
    {
        hi_ext_system_pro_exp_step_table_write(IspDev, i, HI_EXT_SYSTEM_PRO_EXP_STEP_TABLE_DEFAULT);
        hi_ext_system_pro_man_exp_time_table_write(IspDev, i, HI_EXT_SYSTEM_PRO_MAN_EXP_TIME_TABLE_DEFAULT);
        hi_ext_system_pro_man_sysgain_table_write(IspDev, i, HI_EXT_SYSTEM_PRO_MAN_SYSGAIN_TABLE_DEFAULT);
    }

    return;
}

HI_VOID ISP_ExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_U32 u32IspBindAttr = 0;
    ISP_CMOS_SENSOR_MAX_RESOLUTION_S *pstSnsMaxResolution = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    ISP_SensorGetMaxResolution(IspDev,  &pstSnsMaxResolution);
    hi_ext_system_sensor_max_resolution_width_write(IspDev, pstSnsMaxResolution->u32MaxWidth);
    hi_ext_system_sensor_max_resolution_height_write(IspDev, pstSnsMaxResolution->u32MaxHeight);

    /* update Isp bind attr to ext regs */
    u32IspBindAttr = (pstIspCtx->stBindAttr.stAeLib.s32Id << 8) | pstIspCtx->stBindAttr.stAwbLib.s32Id;
    hi_ext_system_bind_attr_write(IspDev, u32IspBindAttr);

    return;
}

HI_VOID ISP_GlobalInitialize(ISP_DEV IspDev)
{
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    pstIspCtx->stSnsImageMode.u16Width = hi_ext_top_sensor_width_read(IspDev);
    pstIspCtx->stSnsImageMode.u16Height = hi_ext_top_sensor_height_read(IspDev);
    u32Value = hi_ext_system_fps_base_read(IspDev);
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;

    pstIspCtx->stPreSnsImageMode.u16Width = pstIspCtx->stSnsImageMode.u16Width;
    pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
    pstIspCtx->stPreSnsImageMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;

    pstIspCtx->u8SnsWDRMode = hi_ext_system_sensor_wdr_mode_read(IspDev);
    pstIspCtx->u8PreSnsWDRMode = pstIspCtx->u8SnsWDRMode;

    pstIspCtx->u32FrameCnt = 0;
    pstIspCtx->u8IspImageModeFlag = 0;

    pstIspCtx->stLinkage.bDefectPixel = HI_TRUE;
    pstIspCtx->bFreezeFw = HI_FALSE;

    pstIspCtx->stLinkage.bCapState = HI_FALSE;
    pstIspCtx->stLinkage.bProTrigger = HI_FALSE;
    pstIspCtx->stLinkage.enSnapType = SNAP_TYPE_NONE;
    pstIspCtx->stLinkage.enSnapPipeMode = ISP_SNAP_NONE;
    pstIspCtx->u8ProFrmCnt = 0;

    memset(&pstIspCtx->stPreDngCcm, 0, sizeof(ISP_DNG_CCM_S));
    memset(&pstIspCtx->stPreDngColorParam, 0, sizeof(ISP_DNG_COLORPARAM_S));

    return;
}

HI_S32 ISP_SnapDefaultSet(ISP_DEV IspDev)
{
    ISP_SNAP_ATTR_S stSnapAttr;
    HI_S32 s32Ret, i;
    ISP_CHECK_OPEN(IspDev);

    stSnapAttr.enSnapType = HI_EXT_SYSTEM_SNAP_TYPE_DEFAULT;
    stSnapAttr.enSnapPipeMode = HI_EXT_SYSTEM_SNAP_PIPE_MODE_DEFAULT;
    stSnapAttr.bUseInfoCCM = HI_EXT_SYSTEM_SNAP_USE_INFO_CCM_ENABLE_DEFAULT;
    stSnapAttr.stProParam.enOpType = HI_EXT_SYSTEM_PRO_TYPE_DEFAULT;
    stSnapAttr.stProParam.u8ProFrameNum = HI_EXT_SYSTEM_PRO_FRAME_NUM_DEFAULT;
    for (i=0; i<PRO_MAX_FRAME_NUM; i++)
    {
        stSnapAttr.stProParam.stAuto.au16ProExpStep[i] = HI_EXT_SYSTEM_PRO_EXP_STEP_TABLE_DEFAULT;
        stSnapAttr.stProParam.stManual.au32ManExpTime[i] = HI_EXT_SYSTEM_PRO_MAN_EXP_TIME_TABLE_DEFAULT;
        stSnapAttr.stProParam.stManual.au32ManSysgain[i] = HI_EXT_SYSTEM_PRO_MAN_SYSGAIN_TABLE_DEFAULT;
    }

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_PRO_EXP_ATTR_SET, &stSnapAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_ISP_SetSnapAttr Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_VOID ISP_DngExtRegsInitialize(ISP_DEV IspDev, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    if (pstDngColorParam->stWbGain1.u16Bgain > 0xFFF)
    {
        printf("stWbGain1.u16Bgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Bgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain1.u16Ggain > 0xFFF)
    {
        printf("stWbGain1.u16Ggain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Ggain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain1.u16Rgain > 0xFFF)
    {
        printf("stWbGain1.u16Rgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Rgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Bgain > 0xFFF)
    {
        printf("stWbGain2.u16Bgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Bgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Ggain > 0xFFF)
    {
        printf("stWbGain2.u16Ggain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Ggain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Rgain > 0xFFF)
    {
        printf("stWbGain2.u16Rgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Rgain = 0xFFF;
    }
    
    hi_ext_system_dng_high_wb_gain_r_write(IspDev, pstDngColorParam->stWbGain1.u16Rgain);
    hi_ext_system_dng_high_wb_gain_g_write(IspDev, pstDngColorParam->stWbGain1.u16Ggain);
    hi_ext_system_dng_high_wb_gain_b_write(IspDev, pstDngColorParam->stWbGain1.u16Bgain);
    hi_ext_system_dng_low_wb_gain_r_write(IspDev, pstDngColorParam->stWbGain2.u16Rgain);
    hi_ext_system_dng_low_wb_gain_g_write(IspDev, pstDngColorParam->stWbGain2.u16Ggain);
    hi_ext_system_dng_low_wb_gain_b_write(IspDev, pstDngColorParam->stWbGain2.u16Bgain);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

