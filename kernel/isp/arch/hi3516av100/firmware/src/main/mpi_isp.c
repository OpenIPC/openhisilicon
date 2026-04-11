/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   :
  History       :
  1.Date        : 2011/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "hi_comm_isp.h"
#include "mpi_sys.h"
#include "mpi_isp.h"
#include "mkp_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_shading_mem_config.h"
#include "isp_metering_mem_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_gamma_fe0_mem_config.h"
#include "isp_gamma_fe1_mem_config.h"
#include "isp_debug.h"
#include "isp_main.h"
#include "isp_proc.h"

#include "hi_vreg.h"

#include "hi_comm_vi.h"
#include "mkp_vi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/


/*****************************************************************************
 Prototype       : isp pub configure
 Description     : need I/F cowork:
                    OB area, need to configure window, and I/F font edge;
                    others configure I/F as back edge;
 Input           : None
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/14
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetPubAttr(ISP_DEV IspDev, const ISP_PUB_ATTR_S *pstPubAttr)
{
    HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if ((pstPubAttr->stWndRect.u32Width < 480) || (pstPubAttr->stWndRect.u32Width > 2592))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Width:%d!\n", pstPubAttr->stWndRect.u32Width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.u32Height < 240) || (pstPubAttr->stWndRect.u32Height > 2200))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Height:%d!\n", pstPubAttr->stWndRect.u32Height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.s32X < 0) || (pstPubAttr->stWndRect.s32X > 2592 - 480))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image X:%d!\n", pstPubAttr->stWndRect.s32X);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.s32Y < 0) || (pstPubAttr->stWndRect.s32Y > 2200 - 240))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Y:%d!\n", pstPubAttr->stWndRect.s32Y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->f32FrameRate <= 0.0) || (pstPubAttr->f32FrameRate > 65535.0))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid FrameRate:%f!\n", pstPubAttr->f32FrameRate);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstPubAttr->enBayer >= BAYER_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Bayer Pattern:%d!\n", pstPubAttr->enBayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_res_switch_write(HI_FALSE);

    hi_isp_top_active_width_write(pstPubAttr->stWndRect.u32Width);
    hi_isp_top_active_height_write(pstPubAttr->stWndRect.u32Height);
    hi_isp_top_rggb_start_write((HI_U8)pstPubAttr->enBayer);

    pValue = (HI_VOID *)(&pstPubAttr->f32FrameRate);
    hi_ext_system_fps_base_write(*(HI_U32 *)pValue);

    hi_isp_input_port_signal_flag_write(0x0a000500);

    hi_isp_input_port_hc_start0_write(pstPubAttr->stWndRect.s32X);
    hi_isp_input_port_hc_size0_write(pstPubAttr->stWndRect.u32Width);
    hi_isp_input_port_vc_start_write(pstPubAttr->stWndRect.s32Y);
    hi_isp_input_port_vc_size_write(pstPubAttr->stWndRect.u32Height);

    //himm 0x20660204 0x0010
    hi_isp_input_port_hs_polarity_vc_write(1);

    //himm 0x20660200 0x0e0f    // preset mode
    hi_isp_input_port_preset_write(0xf);
    hi_isp_input_port_vs_toggle_write(1);
    hi_isp_input_port_vs_polarity_write(1);
    hi_isp_input_port_vs_polarity_acl_write(1);

    //himm 0x20660240 0x1    // safe start
    hi_isp_input_port_mode_request_write(1);

    hi_isp_top_delay_line_jitter_correction_write(0x120d);

    hi_isp_top_delay_line_regen_hblank_write(0x120);

    hi_isp_input_port_field_mode_write(0);

    //hi_ext_system_window_mode_write(pstPubAttr->enWndMode);

    pstIspCtx->stIspParaRec.bPubCfg = HI_TRUE;
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetPubAttr(ISP_DEV IspDev, ISP_PUB_ATTR_S *pstPubAttr)
{
    HI_U8 u8Bayer;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8Bayer = hi_isp_top_rggb_start_read();
    if (u8Bayer >= BAYER_BUTT)
    {
        pstPubAttr->enBayer = BAYER_BUTT;
    }
    else
    {
        pstPubAttr->enBayer = u8Bayer;
    }

    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    pstPubAttr->f32FrameRate = *(HI_FLOAT *)pValue;

    pstPubAttr->stWndRect.s32X     = hi_isp_input_port_hc_start0_read();
    pstPubAttr->stWndRect.u32Width = hi_isp_input_port_hc_size0_read();
    pstPubAttr->stWndRect.s32Y     = hi_isp_input_port_vc_start_read();
    pstPubAttr->stWndRect.u32Height = hi_isp_input_port_vc_size_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFMWState(ISP_DEV IspDev, const ISP_FMW_STATE_E enState)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (enState >= ISP_FMW_STATE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid firmware state %d!\n", enState);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_freeze_firmware_write((HI_U8)enState);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFMWState(ISP_DEV IspDev, ISP_FMW_STATE_E *penState)
{
    HI_U8 u8FMWState;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(penState);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8FMWState = hi_ext_system_freeze_firmware_read();

    if (u8FMWState >=  ISP_FMW_STATE_BUTT)
    {
        *penState = ISP_FMW_STATE_BUTT;
    }
    else
    {
        *penState = u8FMWState;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetWDRMode(ISP_DEV IspDev, const ISP_WDR_MODE_S *pstWDRMode)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstWDRMode->enWDRMode >= WDR_MODE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid WDR mode %d!\n", pstWDRMode->enWDRMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_wdr_switch_write(HI_FALSE);

    pstIspCtx->stIspParaRec.bWDRCfg = HI_TRUE;
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);

    if ((HI_U8)pstWDRMode->enWDRMode == hi_ext_system_sensor_wdr_mode_read())
    {
        hi_ext_top_wdr_switch_write(HI_TRUE);
        return HI_SUCCESS;
    }

    hi_ext_system_sensor_wdr_mode_write((HI_U8)pstWDRMode->enWDRMode);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetWDRMode(ISP_DEV IspDev, ISP_WDR_MODE_S *pstWDRMode)
{
    HI_U8 u8WDRmode;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8WDRmode = hi_ext_system_sensor_wdr_mode_read();
    if (u8WDRmode >= WDR_MODE_BUTT)
    {
        pstWDRMode->enWDRMode = WDR_MODE_BUTT;
    }
    else
    {
        pstWDRMode->enWDRMode = u8WDRmode;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetModuleControl(ISP_DEV IspDev, const ISP_MODULE_CTRL_U *punModCtrl)
{
    HI_U32 u32Key = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u32Key = punModCtrl->u32Key;

    u32Key &= 0x04C1FB7E;
    u32Key |= 0x00040000;

    hi_isp_top_bypass_flag_write(u32Key);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetModuleControl(ISP_DEV IspDev, ISP_MODULE_CTRL_U *punModCtrl)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    punModCtrl->u32Key = hi_isp_top_bypass_flag_read();

    return HI_SUCCESS;
}

/* General Function Settings */
HI_S32 HI_MPI_ISP_SetDRCAttr(ISP_DEV IspDev, const ISP_DRC_ATTR_S *pstDRC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_BOOL(pstDRC->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstDRC->u32SlopeMax > 0xFF)
    {
        printf("Invalid u32SlopeMax Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32SlopeMin > 0xFF)
    {
        printf("Invalid u32SlopeMin Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32WhiteLevel > 0xFFF)
    {
        printf("Invalid u32WhiteLevel Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32BlackLevel > 0xFFF)
    {
        printf("Invalid u32BlackLevel Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32VarianceSpace > 0xF)
    {
        printf("Invalid u32VarianceSpace Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32VarianceIntensity > 0xF)
    {
        printf("Invalid u32VarianceIntensity Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32Asymmetry > 0xFF)
    {
        printf("Invalid u32Asymmetry Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32BrightEnhance > 0xFF)
    {
        printf("Invalid u32BrightEnhance Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stAuto.u32Strength > 0xFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u32StrengthTarget %d!\n", pstDRC->stAuto.u32Strength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stManual.u32Strength > 0xFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u32Strength:%d!\n", pstDRC->stManual.u32Strength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid DRC Op mode:%d!\n", pstDRC->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_drc_enable_write(pstDRC->bEnable);
    hi_isp_drc_slope_max_write(pstDRC->u32SlopeMax);
    hi_isp_drc_slope_min_write(pstDRC->u32SlopeMin);
    hi_isp_drc_black_level_write(pstDRC->u32BlackLevel);
    hi_isp_drc_white_level_write(pstDRC->u32WhiteLevel);
    hi_isp_drc_variance_space_write(pstDRC->u32VarianceSpace);
    hi_isp_drc_variance_intensity_write(pstDRC->u32VarianceIntensity);
    hi_ext_system_drc_asymmetry_write(pstDRC->u32Asymmetry);
    hi_ext_system_drc_bright_enhance_write(pstDRC->u32BrightEnhance);

    hi_ext_system_manual_drc_write(pstDRC->enOpType);
    hi_ext_system_drc_manual_strength_write(pstDRC->stManual.u32Strength);
    hi_ext_system_drc_auto_strength_write(pstDRC->stAuto.u32Strength);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDRCAttr(ISP_DEV IspDev, ISP_DRC_ATTR_S *pstDRC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDRC->bEnable = hi_isp_drc_enable_read();
    pstDRC->u32SlopeMax = hi_isp_drc_slope_max_read();
    pstDRC->u32SlopeMin = hi_isp_drc_slope_min_read();
    pstDRC->u32BlackLevel = hi_isp_drc_black_level_read();
    pstDRC->u32WhiteLevel = hi_isp_drc_white_level_read();
    pstDRC->u32VarianceSpace = hi_isp_drc_variance_space_read();
    pstDRC->u32VarianceIntensity = hi_isp_drc_variance_intensity_read();
    pstDRC->u32Asymmetry = hi_ext_system_drc_asymmetry_read();
    pstDRC->u32BrightEnhance = hi_ext_system_drc_bright_enhance_read();

    pstDRC->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_drc_read();
    pstDRC->stAuto.u32Strength = hi_ext_system_drc_auto_strength_read();
    pstDRC->stManual.u32Strength = hi_ext_system_drc_manual_strength_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFSWDRAttr(ISP_DEV IspDev, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_BOOL(pstFSWDRAttr->bMotionComp);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstFSWDRAttr->u16ShortThresh > 0xFFF)
    {
        printf("Invalid u16ShortThresh %d!\n", pstFSWDRAttr->u16ShortThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16LongThresh > 0xFFF)
    {
        printf("Invalid u16LongThresh %d!\n", pstFSWDRAttr->u16LongThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16LongThresh > pstFSWDRAttr->u16ShortThresh)
    {
        printf("u16LongThresh should NOT be larger than u16ShortThresh!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->enFSWDRComMode >= FS_WDR_COMBINE_BUTT)
    {
        printf("Invalid enFSWDRComMode %d!\n", pstFSWDRAttr->enFSWDRComMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_wdr_use_stitching_error_write(pstFSWDRAttr->bMotionComp);
    hi_isp_wdr_short_thresh_write(pstFSWDRAttr->u16ShortThresh);
    hi_isp_wdr_long_thresh_write(pstFSWDRAttr->u16LongThresh);
    hi_isp_wdr_use_long_override_write(pstFSWDRAttr->enFSWDRComMode);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFSWDRAttr(ISP_DEV IspDev, ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstFSWDRAttr->bMotionComp = hi_isp_wdr_use_stitching_error_read();
    pstFSWDRAttr->u16ShortThresh = hi_isp_wdr_short_thresh_read();
    pstFSWDRAttr->u16LongThresh = hi_isp_wdr_long_thresh_read();
    pstFSWDRAttr->enFSWDRComMode = hi_isp_wdr_use_long_override_read();

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetDPCalibrate(ISP_DEV IspDev, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnable);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnableDetect);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstDPCalibrate->u16CountMax > 0xFFF)
    {
        printf("Invalid u16CountMax Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16CountMin > pstDPCalibrate->u16CountMax)
    {
        printf("Invalid u16CountMin Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16TimeLimit > 0x640)
    {
        printf("Invalid u16TimeLimit Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstDPCalibrate->enStaticDPType >= ISP_STATIC_DP_BUTT)
    {
        printf("Invalid  enStaticDPType Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstDPCalibrate->enStatus >= ISP_STATE_BUTT)
    {
        printf("Invalid  enStatus Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(0 == pstDPCalibrate->u8StartThresh)
    {
        printf("u8StartThresh  should be bigger than 0!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_defect_pixel_enable_write(pstDPCalibrate->bEnable);
    hi_ext_system_calibrate_bad_pixels_write(pstDPCalibrate->bEnableDetect);

    hi_ext_system_bad_pixel_detect_type_write(pstDPCalibrate->enStaticDPType);
    hi_ext_system_bad_pixel_start_thresh_write(pstDPCalibrate->u8StartThresh);
    hi_ext_system_bad_pixel_count_max_write(pstDPCalibrate->u16CountMax);
    hi_ext_system_bad_pixel_count_min_write(pstDPCalibrate->u16CountMin);
    hi_ext_system_bad_pixel_trigger_time_write(pstDPCalibrate->u16TimeLimit);
    hi_ext_system_bad_pixel_trigger_status_write(pstDPCalibrate->enStatus);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDPCalibrate(ISP_DEV IspDev, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    HI_U16 i;
    HI_U16 u16TableAddr;
    //HI_U16 u16DefectPixelCount;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDPCalibrate->bEnable = hi_isp_defect_pixel_enable_read();
    pstDPCalibrate->bEnableDetect = hi_ext_system_calibrate_bad_pixels_read();

    pstDPCalibrate->enStaticDPType = hi_ext_system_bad_pixel_detect_type_read();
    pstDPCalibrate->u8StartThresh = hi_ext_system_bad_pixel_start_thresh_read();
    pstDPCalibrate->u16CountMax = hi_ext_system_bad_pixel_count_max_read();
    pstDPCalibrate->u16CountMin = hi_ext_system_bad_pixel_count_min_read();
    pstDPCalibrate->u16TimeLimit = hi_ext_system_bad_pixel_trigger_time_read();

    pstDPCalibrate->u8FinishThresh = hi_ext_system_bad_pixel_finish_thresh_read();
    pstDPCalibrate->u16Count = hi_isp_defect_pixel_defect_pixel_count_read();
    pstDPCalibrate->enStatus = hi_ext_system_bad_pixel_trigger_status_read();

    u16TableAddr = hi_isp_defect_pixel_table_start_read();

    for(i=0; i<STATIC_DP_COUNT_MAX; i++)
    {
        if(i < pstDPCalibrate->u16Count)
        {
            pstDPCalibrate->u32Table[i] = hi_isp_defect_pixel_table_lut_read((i+u16TableAddr)&0xFFF, 2);
        }
        else
        {
            pstDPCalibrate->u32Table[i] = 0;
        }
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetDPAttr(ISP_DEV IspDev, const ISP_DP_ATTR_S *pstDPAttr)
{
    HI_U16 i = 0, j = 0, m = 0, u16CountIn = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPAttr);
    static HI_U32 au32DefectPixelTable[STATIC_DP_COUNT_MAX];
    ISP_CHECK_BOOL(pstDPAttr->stDynamicAttr.bEnable);
    ISP_CHECK_BOOL(pstDPAttr->stStaticAttr.bEnable);
    ISP_CHECK_BOOL(pstDPAttr->stStaticAttr.bShow);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstDPAttr->stDynamicAttr.u16Slope > 0xFFF)
    {
        printf("DynamicAttr u16Slope should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u16Thresh > 0xFFF)
    {
        printf("DynamicAttr u16Thresh should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u16LineThresh > 0xFFF)
    {
        printf("DynamicAttr u16LineThresh should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u16HpThresh > 0xFFF)
    {
        printf("DynamicAttr u16HpThresh should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u8BlendRatio > 0xFF)
    {
        printf("DynamicAttr u8BlendRatio should be less  than 0xFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stStaticAttr.u16BrightCount > STATIC_DP_COUNT_MAX)
    {
        printf("StaticAttr u16BrightCount should be less  than STATIC_DP_COUNT_MAX!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stStaticAttr.u16DarkCount > STATIC_DP_COUNT_MAX)
    {
        printf("StaticAttr u16DarkCount should be less  than STATIC_DP_COUNT_MAX!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for ( i = 0 ; i < STATIC_DP_COUNT_MAX ; i++ )
    {
        if (pstDPAttr->stStaticAttr.au32BrightTable[i] > 0xFFFFFF || pstDPAttr->stStaticAttr.au32DarkTable[i] > 0xFFFFFF)
        {
            printf("au32DarkTable and au32BrightTable should be less than 0x1000000!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    /* merging dark talbe and bright table */
    i = 0;
    while ((i <  pstDPAttr->stStaticAttr.u16BrightCount) && (j < pstDPAttr->stStaticAttr.u16DarkCount))
    {
        if (m >= STATIC_DP_COUNT_MAX)
        {
            printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstDPAttr->stStaticAttr.au32BrightTable[i] > pstDPAttr->stStaticAttr.au32DarkTable[j])
        {
            au32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32DarkTable[j++];
        }
        else if (pstDPAttr->stStaticAttr.au32BrightTable[i] < pstDPAttr->stStaticAttr.au32DarkTable[j])
        {
            au32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32BrightTable[i++];
        }
        else
        {
            au32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32BrightTable[i];
            i++;
            j++;
        }
    }

    if (i >=  pstDPAttr->stStaticAttr.u16BrightCount)
    {
        while (j < pstDPAttr->stStaticAttr.u16DarkCount)
        {
            if (m >= STATIC_DP_COUNT_MAX)
            {
                printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            au32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32DarkTable[j++];
        }
    }
    if (j >=  pstDPAttr->stStaticAttr.u16DarkCount)
    {
        while (i < pstDPAttr->stStaticAttr.u16BrightCount)
        {
            if (m >= STATIC_DP_COUNT_MAX)
            {
                printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            au32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32BrightTable[i++];
        }
    }

    u16CountIn = m;

    hi_isp_green_equalize_dp_enable_write(pstDPAttr->stDynamicAttr.bEnable);
    hi_ext_system_dynamic_defect_pixel_slope_write(pstDPAttr->stDynamicAttr.u16Slope);
    hi_ext_system_dynamic_defect_pixel_thresh_write(pstDPAttr->stDynamicAttr.u16Thresh);

    hi_isp_raw_frontend_line_thresh_write(pstDPAttr->stDynamicAttr.u16LineThresh);
    hi_isp_raw_frontend_hpdev_threshold_write(pstDPAttr->stDynamicAttr.u16HpThresh);
    hi_isp_raw_frontend_hp_blend_write(pstDPAttr->stDynamicAttr.u8BlendRatio);

    hi_isp_defect_pixel_enable_write(pstDPAttr->stStaticAttr.bEnable);
    hi_isp_defect_pixel_show_static_defect_pixels_write(pstDPAttr->stStaticAttr.bShow);

    for(i = 0; i < u16CountIn; i++)
    {
        hi_isp_defect_pixel_table_lut_write(i, au32DefectPixelTable[i]);
    }

    hi_isp_defect_pixel_defect_pixel_count_in_write(u16CountIn);
    hi_isp_defect_pixel_pointer_reset_write(1);
    hi_isp_defect_pixel_pointer_reset_write(0);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDPAttr(ISP_DEV IspDev, ISP_DP_ATTR_S *pstDPAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDPAttr->stDynamicAttr.bEnable = hi_isp_green_equalize_dp_enable_read();
    pstDPAttr->stDynamicAttr.u16Slope = hi_ext_system_dynamic_defect_pixel_slope_read();
    pstDPAttr->stDynamicAttr.u16Thresh = hi_ext_system_dynamic_defect_pixel_thresh_read();
    pstDPAttr->stDynamicAttr.u16LineThresh = hi_isp_raw_frontend_line_thresh_read();
    pstDPAttr->stDynamicAttr.u16HpThresh = hi_isp_raw_frontend_hpdev_threshold_read();
    pstDPAttr->stDynamicAttr.u8BlendRatio = hi_isp_raw_frontend_hp_blend_read();

    pstDPAttr->stStaticAttr.bEnable = hi_isp_defect_pixel_enable_read();
    pstDPAttr->stStaticAttr.bShow = hi_isp_defect_pixel_show_static_defect_pixels_read();
    pstDPAttr->stStaticAttr.u16BrightCount = hi_isp_defect_pixel_defect_pixel_count_in_read();
    pstDPAttr->stStaticAttr.u16DarkCount = 0;


    for(i = 0; i < STATIC_DP_COUNT_MAX; i++)
    {
        if(i < pstDPAttr->stStaticAttr.u16BrightCount)
        {
            pstDPAttr->stStaticAttr.au32BrightTable[i] = (hi_isp_defect_pixel_table_lut_read(i, 2) & 0xFFFFFF);
        }
        else
        {
            pstDPAttr->stStaticAttr.au32BrightTable[i] = 0;
        }
    }

    memset(pstDPAttr->stStaticAttr.au32DarkTable, 0, sizeof(pstDPAttr->stStaticAttr.au32DarkTable));

    return HI_SUCCESS;
}

extern HI_S32 VI_SetDISAttr(VI_DEV ViDev, const VI_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_SetDISAttr(ISP_DEV IspDev, const ISP_DIS_ATTR_S *pstDISAttr)
{
    VI_DIS_ATTR_S stDisAttr = {0};

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDISAttr);
    ISP_CHECK_BOOL(pstDISAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    //hi_isp_dis_manual_control_write(!pstDISAttr->bEnable);

    stDisAttr.bEnable = pstDISAttr->bEnable;
    return VI_SetDISAttr(0, &stDisAttr);
}

extern HI_S32 VI_GetDISAttr(VI_DEV ViDev, VI_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_GetDISAttr(ISP_DEV IspDev, ISP_DIS_ATTR_S *pstDISAttr)
{
    VI_DIS_ATTR_S stDisAttr = {0};
    HI_S32 s32Ret;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDISAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    //pstDISAttr->bEnable = !hi_isp_dis_manual_control_read();
    s32Ret = VI_GetDISAttr(0, &stDisAttr);
    pstDISAttr->bEnable = stDisAttr.bEnable;
    return s32Ret;
}

HI_S32 HI_MPI_ISP_SetShadingAttr(ISP_DEV IspDev, const ISP_SHADING_ATTR_S *pstShadingAttr)
{
    HI_U32 i = 0, j = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_BOOL(pstShadingAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstShadingAttr->u16TableNodeNum > SHADING_NODE_NUM_MAX)
    {
        printf("Invalid Shading Table Node Number %d!\n", pstShadingAttr->u16TableNodeNum);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (j=0; j<3; j++)
    {
        if ((pstShadingAttr->astRadialShading[j].stCenter.s32X < 0) || (pstShadingAttr->astRadialShading[j].stCenter.s32X > 0xFFFF))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Shading[%d] s32X %d!\n", j, pstShadingAttr->astRadialShading[j].stCenter.s32X);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if ((pstShadingAttr->astRadialShading[j].stCenter.s32Y < 0) || (pstShadingAttr->astRadialShading[j].stCenter.s32Y > 0xFFFF))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Shading[%d] s32Y %d!\n", j, pstShadingAttr->astRadialShading[j].stCenter.s32Y);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    for (i=0; i<pstShadingAttr->u16TableNodeNum; i++)
    {
        for (j=0; j<3; j++)
        {
            if (pstShadingAttr->astRadialShading[j].u32Table[i] > 0xFFFF)
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid Shading[%d] Table[%d] %d!\n", j, i, pstShadingAttr->astRadialShading[j].u32Table[i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
    }

    hi_isp_radial_shading_enable_write(pstShadingAttr->bEnable);

    hi_isp_radial_shading_off_center_multr_write(pstShadingAttr->astRadialShading[0].u16OffCenter);
    hi_isp_radial_shading_off_center_multg_write(pstShadingAttr->astRadialShading[1].u16OffCenter);
    hi_isp_radial_shading_off_center_multb_write(pstShadingAttr->astRadialShading[2].u16OffCenter);
    hi_isp_radial_shading_centerr_x_write(pstShadingAttr->astRadialShading[0].stCenter.s32X);
    hi_isp_radial_shading_centerr_y_write(pstShadingAttr->astRadialShading[0].stCenter.s32Y);
    hi_isp_radial_shading_centerg_x_write(pstShadingAttr->astRadialShading[1].stCenter.s32X);
    hi_isp_radial_shading_centerg_y_write(pstShadingAttr->astRadialShading[1].stCenter.s32Y);
    hi_isp_radial_shading_centerb_x_write(pstShadingAttr->astRadialShading[2].stCenter.s32X);
    hi_isp_radial_shading_centerb_y_write(pstShadingAttr->astRadialShading[2].stCenter.s32Y);

    hi_isp_radial_shading_mcu_priority_write(1);
    for (i=0; i<pstShadingAttr->u16TableNodeNum; i++)
    {
        hi_radial_shading_mem_luts_shading_lutr_write(i, pstShadingAttr->astRadialShading[0].u32Table[i]);
        hi_radial_shading_mem_luts_shading_lutg_write(i, pstShadingAttr->astRadialShading[1].u32Table[i]);
        hi_radial_shading_mem_luts_shading_lutb_write(i, pstShadingAttr->astRadialShading[2].u32Table[i]);
    }
    hi_isp_radial_shading_mcu_priority_write(0);

    hi_ext_system_shading_table_node_number_write(pstShadingAttr->u16TableNodeNum);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetShadingAttr(ISP_DEV IspDev, ISP_SHADING_ATTR_S *pstShadingAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstShadingAttr->bEnable = hi_isp_radial_shading_enable_read();
    pstShadingAttr->astRadialShading[0].u16OffCenter = hi_isp_radial_shading_off_center_multr_read();
    pstShadingAttr->astRadialShading[1].u16OffCenter = hi_isp_radial_shading_off_center_multg_read();
    pstShadingAttr->astRadialShading[2].u16OffCenter = hi_isp_radial_shading_off_center_multb_read();

    pstShadingAttr->astRadialShading[0].stCenter.s32X = hi_isp_radial_shading_centerr_x_read();
    pstShadingAttr->astRadialShading[0].stCenter.s32Y = hi_isp_radial_shading_centerr_y_read();
    pstShadingAttr->astRadialShading[1].stCenter.s32X = hi_isp_radial_shading_centerg_x_read();
    pstShadingAttr->astRadialShading[1].stCenter.s32Y = hi_isp_radial_shading_centerg_y_read();
    pstShadingAttr->astRadialShading[2].stCenter.s32X = hi_isp_radial_shading_centerb_x_read();
    pstShadingAttr->astRadialShading[2].stCenter.s32Y = hi_isp_radial_shading_centerb_y_read();

    pstShadingAttr->u16TableNodeNum = hi_ext_system_shading_table_node_number_read();

    hi_isp_radial_shading_mcu_priority_write(1);
    for (i=0; i<SHADING_NODE_NUM_MAX; i++)
    {
        pstShadingAttr->astRadialShading[0].u32Table[i] = hi_radial_shading_mem_luts_shading_lutr_read(i);
        pstShadingAttr->astRadialShading[1].u32Table[i] = hi_radial_shading_mem_luts_shading_lutg_read(i);
        pstShadingAttr->astRadialShading[2].u32Table[i] = hi_radial_shading_mem_luts_shading_lutb_read(i);
    }
    hi_isp_radial_shading_mcu_priority_write(0);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetNRAttr(ISP_DEV IspDev, const ISP_NR_ATTR_S *pstNRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_BOOL(pstNRAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstNRAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid NR type %d!\n", pstNRAttr->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_nr_enable_write(pstNRAttr->bEnable);
    hi_ext_system_manual_nr_write(pstNRAttr->enOpType);

    for(i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_snr_thresh_write(i, pstNRAttr->stAuto.au8Thresh[i]);
    }

    hi_ext_system_nr_threshold_target_write(pstNRAttr->stManual.u8Thresh);
    hi_ext_system_nr_threshold_long_target_write(pstNRAttr->stManual.u8ThreshLong);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetNRAttr(ISP_DEV IspDev, ISP_NR_ATTR_S *pstNRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstNRAttr->bEnable = hi_isp_nr_enable_read();
    pstNRAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_nr_read();

    for(i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstNRAttr->stAuto.au8Thresh[i] = hi_ext_system_agc_table_snr_thresh_read(i);
    }
    pstNRAttr->stManual.u8Thresh = hi_ext_system_nr_threshold_target_read();
    pstNRAttr->stManual.u8ThreshLong = hi_ext_system_nr_threshold_long_target_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetNPTable(ISP_DEV IspDev, const ISP_NP_TABLE_S *pstNPTable)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNPTable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for(i = 0; i < NP_NODE_NUM_MAX; i++)
    {
        hi_ext_nr_np_lut_write(i, pstNPTable->au8NPTable[i]);
    }

    hi_ext_nr_np_lut_update_write(HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetNPTable(ISP_DEV IspDev, ISP_NP_TABLE_S *pstNPTable)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNPTable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for(i = 0; i < NP_NODE_NUM_MAX; i++)
    {
        pstNPTable->au8NPTable[i] = hi_ext_nr_np_lut_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetColorToneAttr(ISP_DEV IspDev, const ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCTAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    hi_isp_matrix_coefft_wb_r_write(pstCTAttr->u16RedCastGain);
    hi_isp_matrix_coefft_wb_g_write(pstCTAttr->u16GreenCastGain);
    hi_isp_matrix_coefft_wb_b_write(pstCTAttr->u16BlueCastGain);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetColorToneAttr(ISP_DEV IspDev, ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCTAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstCTAttr->u16RedCastGain = hi_isp_matrix_coefft_wb_r_read();
    pstCTAttr->u16GreenCastGain = hi_isp_matrix_coefft_wb_g_read();
    pstCTAttr->u16BlueCastGain = hi_isp_matrix_coefft_wb_b_read();

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetGammaAttr(ISP_DEV IspDev, const ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;
    HI_U32 u32WDRMode;
    const HI_U16 *pu16GammaLut;
    static HI_U16 au16GammaDef[GAMMA_NODE_NUM] = {
         0  ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,
        1096,1138,1178,1218,1254,1280,1314,1346,1378,1408,1438,1467,1493,1519,1543,1568,
        1592,1615,1638,1661,1683,1705,1726,1748,1769,1789,1810,1830,1849,1869,1888,1907,
        1926,1945,1963,1981,1999,2017,2034,2052,2069,2086,2102,2119,2136,2152,2168,2184,
        2200,2216,2231,2247,2262,2277,2292,2307,2322,2337,2351,2366,2380,2394,2408,2422,
        2436,2450,2464,2477,2491,2504,2518,2531,2544,2557,2570,2583,2596,2609,2621,2634,
        2646,2659,2671,2683,2696,2708,2720,2732,2744,2756,2767,2779,2791,2802,2814,2825,
        2837,2848,2859,2871,2882,2893,2904,2915,2926,2937,2948,2959,2969,2980,2991,3001,
        3012,3023,3033,3043,3054,3064,3074,3085,3095,3105,3115,3125,3135,3145,3155,3165,
        3175,3185,3194,3204,3214,3224,3233,3243,3252,3262,3271,3281,3290,3300,3309,3318,
        3327,3337,3346,3355,3364,3373,3382,3391,3400,3409,3418,3427,3436,3445,3454,3463,
        3471,3480,3489,3498,3506,3515,3523,3532,3540,3549,3557,3566,3574,3583,3591,3600,
        3608,3616,3624,3633,3641,3649,3657,3665,3674,3682,3690,3698,3706,3714,3722,3730,
        3738,3746,3754,3762,3769,3777,3785,3793,3801,3808,3816,3824,3832,3839,3847,3855,
        3862,3870,3877,3885,3892,3900,3907,3915,3922,3930,3937,3945,3952,3959,3967,3974,
        3981,3989,3996,4003,4010,4018,4025,4032,4039,4046,4054,4061,4068,4075,4082,4089,4095
    };
    static HI_U16 au16GammasRGB[GAMMA_NODE_NUM] = {
        0,    203,  347,  452,  539,  613,  679,  739,  794,  846,  894,  939,  982,  1023,  1062,  1100,
        1136, 1171, 1204, 1237, 1268, 1299, 1329, 1358, 1386, 1414, 1441, 1467, 1493, 1519,  1543,  1568,
        1592, 1615, 1638, 1661, 1683, 1705, 1726, 1748, 1769, 1789, 1810, 1830, 1849, 1869,  1888,  1907,
        1926, 1945, 1963, 1981, 1999, 2017, 2034, 2052, 2069, 2086, 2102, 2119, 2136, 2152,  2168,  2184,
        2200, 2216, 2231, 2247, 2262, 2277, 2292, 2307, 2322, 2337, 2351, 2366, 2380, 2394,  2408,  2422,
        2436, 2450, 2464, 2477, 2491, 2504, 2518, 2531, 2544, 2557, 2570, 2583, 2596, 2609,  2621,  2634,
        2646, 2659, 2671, 2683, 2696, 2708, 2720, 2732, 2744, 2756, 2767, 2779, 2791, 2802,  2814,  2825,
        2837, 2848, 2859, 2871, 2882, 2893, 2904, 2915, 2926, 2937, 2948, 2959, 2969, 2980,  2991,  3001,
        3012, 3023, 3033, 3043, 3054, 3064, 3074, 3085, 3095, 3105, 3115, 3125, 3135, 3145,  3155,  3165,
        3175, 3185, 3194, 3204, 3214, 3224, 3233, 3243, 3252, 3262, 3271, 3281, 3290, 3300,  3309,  3318,
        3327, 3337, 3346, 3355, 3364, 3373, 3382, 3391, 3400, 3409, 3418, 3427, 3436, 3445,  3454,  3463,
        3471, 3480, 3489, 3498, 3506, 3515, 3523, 3532, 3540, 3549, 3557, 3566, 3574, 3583,  3591,  3600,
        3608, 3616, 3624, 3633, 3641, 3649, 3657, 3665, 3674, 3682, 3690, 3698, 3706, 3714,  3722,  3730,
        3738, 3746, 3754, 3762, 3769, 3777, 3785, 3793, 3801, 3808, 3816, 3824, 3832, 3839,  3847,  3855,
        3862, 3870, 3877, 3885, 3892, 3900, 3907, 3915, 3922, 3930, 3937, 3945, 3952, 3959,  3967,  3974,
        3981, 3989, 3996, 4003, 4010, 4018, 4025, 4032, 4039, 4046, 4054, 4061, 4068, 4075,  4082,  4089, 4095
    };

    static HI_U16 au16GammaDefWDR[GAMMA_NODE_NUM]={
        0,1,2,4,8,12,17,23,30,38,47,57,68,79,92,105,120,133,147,161,176,192,209,226,243,260,278,296,317,340,365,390,416,440,466,491,517,538,561,584,607,631,656,680,705,730,756,784,812,835,858,882,908,934,958,982,1008,1036,1064,1092,1119,1143,1167,1192,1218,1243,1269,1296,1323,1351,1379,1408,1434,1457,1481,1507,1531,1554,1579,1603,1628,1656,1683,1708,1732,1756,1780,1804,1829,1854,1877,1901,1926,1952,1979,2003,2024,2042,2062,2084,2106,2128,2147,2168,2191,2214,2233,2256,2278,2296,2314,2335,2352,2373,2391,2412,2431,2451,2472,2492,2513,2531,2547,2566,2581,2601,2616,2632,2652,2668,2688,2705,2721,2742,2759,2779,2796,2812,2826,2842,2857,2872,2888,2903,2920,2934,2951,2967,2983,3000,3015,3033,3048,3065,3080,3091,3105,3118,3130,3145,3156,3171,3184,3197,3213,3224,3240,3252,3267,3281,3295,3310,3323,3335,3347,3361,3372,3383,3397,3409,3421,3432,3447,3459,3470,3482,3497,3509,3521,3534,3548,3560,3572,3580,3592,3602,3613,3625,3633,3646,3657,3667,3679,3688,3701,3709,3719,3727,3736,3745,3754,3764,3773,3781,3791,3798,3806,3816,3823,3833,3840,3847,3858,3865,3872,3879,3888,3897,3904,3911,3919,3926,3933,3940,3948,3955,3962,3970,3973,3981,3988,3996,4003,4011,4018,4026,4032,4037,4045,4053,4057,4064,4072,4076,4084,4088,4095
        };
    static HI_U16 au16GammasRGBWDR[GAMMA_NODE_NUM]={
           0,   1,   3,   7,  13,  20,  29,  40,  52,  65,  81,  98, 116, 136, 158, 181,
         203, 226, 248, 269, 291, 312, 334, 355, 376, 396, 417, 438, 458, 478, 499, 519,
         538, 558, 578, 598, 617, 637, 656, 675, 695, 714, 733, 752, 771, 789, 808, 827,
         845, 864, 883, 901, 919, 938, 956, 974, 992,1010,1028,1046,1064,1082,1100,1118,
        1136,1153,1171,1189,1206,1224,1241,1259,1276,1293,1311,1328,1345,1362,1379,1397,
        1414,1431,1448,1465,1482,1499,1515,1532,1549,1566,1583,1599,1616,1633,1649,1666,
        1683,1699,1716,1732,1749,1765,1781,1798,1814,1830,1847,1863,1879,1896,1912,1928,
        1944,1960,1976,1992,2008,2024,2041,2056,2072,2088,2104,2120,2136,2152,2168,2184,
        2199,2215,2231,2247,2262,2278,2294,2309,2325,2341,2356,2372,2387,2403,2418,2434,
        2449,2465,2480,2496,2511,2527,2542,2557,2573,2588,2603,2619,2634,2649,2664,2680,
        2695,2710,2725,2740,2756,2771,2786,2801,2816,2831,2846,2861,2876,2891,2906,2921,
        2936,2951,2966,2981,2996,3011,3026,3041,3056,3070,3085,3100,3115,3130,3145,3159,
        3174,3189,3204,3218,3233,3248,3262,3277,3292,3306,3321,3336,3350,3365,3379,3394,
        3409,3423,3438,3452,3467,3481,3496,3510,3525,3539,3554,3568,3582,3597,3611,3626,
        3640,3654,3669,3683,3697,3712,3726,3740,3755,3769,3783,3798,3812,3826,3840,3855,
        3869,3883,3897,3911,3926,3940,3954,3968,3982,3996,4010,4025,4039,4053,4067,4081,4095
        };

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_BOOL(pstGammaAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u32WDRMode = hi_ext_system_sensor_wdr_mode_read();

    switch (pstGammaAttr->enCurveType)
    {
        case ISP_GAMMA_CURVE_DEFAULT:
            if (0 == u32WDRMode)
            {
                pu16GammaLut = au16GammaDef;
            }
            else
            {
                pu16GammaLut = au16GammaDefWDR;
            }
            break;
        case ISP_GAMMA_CURVE_SRGB:
            if (0 == u32WDRMode)
            {
                pu16GammaLut = au16GammasRGB;
            }
            else
            {
                pu16GammaLut = au16GammasRGBWDR;
            }
            break;
        case ISP_GAMMA_CURVE_USER_DEFINE:
            pu16GammaLut = pstGammaAttr->u16Table;
            break;
        default:
            printf("Invalid ISP Gamma Curve Type %d!\n", pstGammaAttr->enCurveType);
            return HI_ERR_ISP_ILLEGAL_PARAM;
            break;
    }

    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
        if (pu16GammaLut[i] > 0xFFF)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Gamma Table[%d] %d!\n", i, pu16GammaLut[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_isp_gamma_enable_write(pstGammaAttr->bEnable);
#if 0
    hi_isp_gamma_mcu_priority_write(1);
    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
        hi_gamma_rgb_mem_array_data_write(i, pu16GammaLut[i]);
    }
    hi_isp_gamma_mcu_priority_write(0);
#endif

    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
        hi_isp_ext_gamma_rgb_mem_array_data_write((HI_U16)i, pu16GammaLut[i]);
    }

    hi_isp_ext_system_gamma_change_write(1);

    hi_ext_system_gamma_curve_type_write(pstGammaAttr->enCurveType);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetGammaAttr(ISP_DEV IspDev, ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstGammaAttr->bEnable = hi_isp_gamma_enable_read();

    pstGammaAttr->enCurveType = hi_ext_system_gamma_curve_type_read();

    hi_isp_gamma_mcu_priority_write(1);
    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
        pstGammaAttr->u16Table[i] = hi_gamma_rgb_mem_array_data_read(i);
    }
    hi_isp_gamma_mcu_priority_write(0);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetGammaFEAttr(ISP_DEV IspDev, const ISP_GAMMAFE_ATTR_S *pstGammaFEAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaFEAttr);
    ISP_CHECK_BOOL(pstGammaFEAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    hi_isp_gammafe_enable_write(pstGammaFEAttr->bEnable);
    hi_isp_gammafe_enable1_write(pstGammaFEAttr->bEnable);

    hi_isp_gammafe_mcu_priority_write(1);
    for(i = 0; i < GAMMA_FE0_NODE_NUM; i++)
    {
        hi_gamma_fe0_mem_array_data_write(i, pstGammaFEAttr->u16Table[i]);
    }
    for(i = 0; i< GAMMA_FE1_NODE_NUM; i++)
    {
        hi_gamma_fe1_mem_array_data_write(i, pstGammaFEAttr->u16Table[GAMMA_FE0_NODE_NUM+i]);
    }
    hi_isp_gammafe_mcu_priority_write(0);

    return HI_SUCCESS;

}

HI_S32 HI_MPI_ISP_GetGammaFEAttr(ISP_DEV IspDev, ISP_GAMMAFE_ATTR_S *pstGammaFEAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaFEAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstGammaFEAttr->bEnable = hi_isp_gammafe_enable1_read();

    hi_isp_gammafe_mcu_priority_write(1);
    for(i = 0; i<GAMMA_FE0_NODE_NUM; i++)
    {
        pstGammaFEAttr->u16Table[i] = hi_gamma_fe0_mem_array_data_read(i);
    }
    for(i = 0; i<GAMMA_FE1_NODE_NUM; i++)
    {
        pstGammaFEAttr->u16Table[GAMMA_FE0_NODE_NUM+i] = hi_gamma_fe1_mem_array_data_read(i);
    }
    hi_isp_gammafe_mcu_priority_write(0);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetSharpenAttr(ISP_DEV IspDev, const ISP_SHARPEN_ATTR_S *pstSharpenAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSharpenAttr);
    ISP_CHECK_BOOL(pstSharpenAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstSharpenAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen Type %d!\n", pstSharpenAttr->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstSharpenAttr->stRGBSharpenAttr.u8LutCore > 255)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid RGBSharpen Core %d!\n", pstSharpenAttr->stRGBSharpenAttr.u8LutCore);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpenAttr->stRGBSharpenAttr.u8LutStrength > 127)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid RGBSharpen Strength %d!\n", pstSharpenAttr->stRGBSharpenAttr.u8LutStrength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpenAttr->stRGBSharpenAttr.u8LutMagnitude > 31)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid RGBSharpen Magnitude %d!\n", pstSharpenAttr->stRGBSharpenAttr.u8LutMagnitude);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_sharpening_enable_write(pstSharpenAttr->bEnable);
    hi_ext_system_manual_sharpening_write(pstSharpenAttr->enOpType);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_sharp_alt_d_write(i, pstSharpenAttr->stAuto.au8SharpenD[i]);
        hi_ext_system_agc_table_sharp_alt_ud_write(i, pstSharpenAttr->stAuto.au8SharpenUd[i]);
        hi_ext_system_agc_table_RGBsharp_strength_write(i, pstSharpenAttr->stAuto.au8SharpenRGB[i]);
    }

    hi_ext_system_sharpening_target_write(pstSharpenAttr->stManual.u8SharpenD);
    hi_ext_system_sharpening_ud_target_write(pstSharpenAttr->stManual.u8SharpenUd);
    hi_ext_system_RGBsharpening_target_write(pstSharpenAttr->stManual.u8SharpenRGB);

    hi_ext_system_sharpening_lut_coring_write(pstSharpenAttr->stRGBSharpenAttr.u8LutCore);
    hi_ext_system_sharpening_lut_strength_write(pstSharpenAttr->stRGBSharpenAttr.u8LutStrength);
    hi_ext_system_sharpening_lut_magnitude_write(pstSharpenAttr->stRGBSharpenAttr.u8LutMagnitude);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetSharpenAttr(ISP_DEV IspDev, ISP_SHARPEN_ATTR_S *pstSharpenAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSharpenAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstSharpenAttr->bEnable = hi_ext_system_sharpening_enable_read();
    pstSharpenAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_sharpening_read();

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstSharpenAttr->stAuto.au8SharpenD[i] = hi_ext_system_agc_table_sharp_alt_d_read(i);
        pstSharpenAttr->stAuto.au8SharpenUd[i] = hi_ext_system_agc_table_sharp_alt_ud_read(i);
        pstSharpenAttr->stAuto.au8SharpenRGB[i] = hi_ext_system_agc_table_RGBsharp_strength_read(i);
    }

    pstSharpenAttr->stManual.u8SharpenD = hi_ext_system_sharpening_target_read();
    pstSharpenAttr->stManual.u8SharpenUd = hi_ext_system_sharpening_ud_target_read();
    pstSharpenAttr->stManual.u8SharpenRGB = hi_ext_system_RGBsharpening_target_read();

    pstSharpenAttr->stRGBSharpenAttr.u8LutCore = hi_ext_system_sharpening_lut_coring_read();
    pstSharpenAttr->stRGBSharpenAttr.u8LutStrength = hi_ext_system_sharpening_lut_strength_read();
    pstSharpenAttr->stRGBSharpenAttr.u8LutMagnitude = hi_ext_system_sharpening_lut_magnitude_read();

    return HI_SUCCESS;
}

/* Crosstalk Removal Strength */
HI_S32 HI_MPI_ISP_SetCrosstalkAttr(ISP_DEV IspDev, const ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_BOOL(pstCRAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstCRAttr->u16Threshold > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid CrosstalkAttr u16Threshold:%d!\n", pstCRAttr->u16Threshold);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstCRAttr->u16Slope > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid CrosstalkAttr u16Slope:%d!\n", pstCRAttr->u16Slope);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_green_equalize_ge_enable_write(pstCRAttr->bEnable);
    hi_isp_green_equalize_ge_threshold_write(pstCRAttr->u16Threshold);
    hi_isp_green_equalize_ge_slope_write(pstCRAttr->u16Slope);
    hi_isp_green_equalize_ge_sens_write(pstCRAttr->u8Sensitivity);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_ge_strength_write(i, pstCRAttr->au8Strength[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetCrosstalkAttr(ISP_DEV IspDev, ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstCRAttr->bEnable = hi_isp_green_equalize_ge_enable_read();
    pstCRAttr->u16Threshold = hi_isp_green_equalize_ge_threshold_read();
    pstCRAttr->u16Slope = hi_isp_green_equalize_ge_slope_read();
    pstCRAttr->u8Sensitivity = hi_isp_green_equalize_ge_sens_read();

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstCRAttr->au8Strength[i] = hi_ext_system_agc_table_ge_strength_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAntiFalseColorAttr(ISP_DEV IspDev, const ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    hi_isp_demosaic_fc_slope_write(pstAntiFC->u8Strength);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetAntiFalseColorAttr(ISP_DEV IspDev, ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstAntiFC->u8Strength = hi_isp_demosaic_fc_slope_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDemosaicAttr(ISP_DEV IspDev, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstDemosaicAttr->u16VhThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic VhThresh %d!\n", pstDemosaicAttr->u16VhThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16AaThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic AaThresh %d!\n", pstDemosaicAttr->u16AaThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16VaThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic VaThresh %d!\n", pstDemosaicAttr->u16VaThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16UuThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic UuThresh %d!\n", pstDemosaicAttr->u16UuThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstDemosaicAttr->enCfgType >= ISP_DEMOSAIC_CFG_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic Config %d!\n", pstDemosaicAttr->enCfgType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_isp_demosaic_vh_slope_write(pstDemosaicAttr->u8VhSlope);
    hi_isp_demosaic_aa_slope_write(pstDemosaicAttr->u8AaSlope);
    hi_isp_demosaic_va_slope_write(pstDemosaicAttr->u8VaSlope);
    hi_isp_demosaic_uu_slope_write(pstDemosaicAttr->u8UuSlope);

    hi_isp_demosaic_vh_thresh_write(pstDemosaicAttr->u16VhThresh);
    hi_isp_demosaic_aa_thresh_write(pstDemosaicAttr->u16AaThresh);
    hi_isp_demosaic_va_thresh_write(pstDemosaicAttr->u16VaThresh);
    hi_isp_demosaic_uu_thresh_write(pstDemosaicAttr->u16UuThresh);

    switch(pstDemosaicAttr->enCfgType)
    {
        case ISP_DEMOSAIC_CFG_VH:
            hi_isp_demosaic_dmsc_config_write(0x13);
            break;
        case ISP_DEMOSAIC_CFG_AA:
            hi_isp_demosaic_dmsc_config_write(0x11);
            break;
        case ISP_DEMOSAIC_CFG_VA:
            hi_isp_demosaic_dmsc_config_write(0x12);
            break;
        case ISP_DEMOSAIC_CFG_UU:
            hi_isp_demosaic_dmsc_config_write(0x4);
            break;
        default:
            hi_isp_demosaic_dmsc_config_write(0x0);
    }
    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_demosaic_lum_thresh_write(i, pstDemosaicAttr->au8LumThresh[i]);
        hi_ext_system_agc_table_demosaic_np_offset_write(i, pstDemosaicAttr->au8NpOffset[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDemosaicAttr(ISP_DEV IspDev, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDemosaicAttr->u8VhSlope = hi_isp_demosaic_vh_slope_read();
    pstDemosaicAttr->u8AaSlope = hi_isp_demosaic_aa_slope_read();
    pstDemosaicAttr->u8VaSlope = hi_isp_demosaic_va_slope_read();
    pstDemosaicAttr->u8UuSlope = hi_isp_demosaic_uu_slope_read();

    pstDemosaicAttr->u16VhThresh = hi_isp_demosaic_vh_thresh_read();
    pstDemosaicAttr->u16AaThresh = hi_isp_demosaic_aa_thresh_read();
    pstDemosaicAttr->u16VaThresh = hi_isp_demosaic_va_thresh_read();
    pstDemosaicAttr->u16UuThresh = hi_isp_demosaic_uu_thresh_read();

    switch(hi_isp_demosaic_dmsc_config_read())
    {
        case 0x13:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_VH;
            break;
        case 0x11:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_AA;
            break;
        case 0x12:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_VA;
            break;
        case 0x4:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_UU;
            break;
        case 0x0:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_DEFAULT;
            break;
        default:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_BUTT;
    }

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstDemosaicAttr->au8LumThresh[i] = hi_ext_system_agc_table_demosaic_lum_thresh_read(i);
        pstDemosaicAttr->au8NpOffset[i] = hi_ext_system_agc_table_demosaic_np_offset_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBlackLevelAttr(ISP_DEV IspDev, const ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for (i=0; i<4; i++)
    {
        if (pstBlackLevel->au16BlackLevel[i] > 0xFFF)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid BlackLevel[%d]:%d!\n", i, pstBlackLevel->au16BlackLevel[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_ext_system_black_level_00_write(pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_01_write(pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_10_write(pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_11_write(pstBlackLevel->au16BlackLevel[3]);
    hi_ext_system_black_level_change_write((HI_U8)HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBlackLevelAttr(ISP_DEV IspDev, ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstBlackLevel->au16BlackLevel[0] = hi_ext_system_black_level_00_read();
    pstBlackLevel->au16BlackLevel[1] = hi_ext_system_black_level_01_read();
    pstBlackLevel->au16BlackLevel[2] = hi_ext_system_black_level_10_read();
    pstBlackLevel->au16BlackLevel[3] = hi_ext_system_black_level_11_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_FPNCalibrate(ISP_DEV IspDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCalibrateAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_SetCalibrateAttr(0, pstCalibrateAttr);
}

HI_S32 HI_MPI_ISP_SetFPNAttr(ISP_DEV IspDev, const ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_SetCorrectionAttr(0, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_GetFPNAttr(ISP_DEV IspDev, ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_GetCorrectionAttr(0, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_SetDeFogAttr(ISP_DEV IspDev, const ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_BOOL(pstDefogAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstDefogAttr->enOpType >= OP_TYPE_BUTT)
    {
        printf("Invalid Defog OpType!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->bEnable != HI_FALSE)&&(pstDefogAttr->bEnable != HI_TRUE))
    {
       printf("Invalid Defog Enable Input\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->u8HorizontalBlock > 16)||(pstDefogAttr->u8HorizontalBlock <= 0))
    {
        printf("HorizonBlock should between 1 and 16!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->u8VerticalBlock > 15)||(pstDefogAttr->u8VerticalBlock <= 0))
    {
        printf("VerticalBlock should between 1 and 15 !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_vi_dehaze_enable_write(pstDefogAttr->bEnable);
    isp_ext_system_manual_dehaze_hblk_write(pstDefogAttr->u8HorizontalBlock);
    isp_ext_system_manual_dehaze_vblk_write(pstDefogAttr->u8VerticalBlock);
    isp_ext_system_manual_dehaze_enable_write(pstDefogAttr->enOpType);
    isp_ext_system_manual_dehaze_strength_write(pstDefogAttr->stManual.u8strength);
    isp_ext_system_manual_dehaze_autostrength_write(pstDefogAttr->stAuto.u8strength);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDeFogAttr(ISP_DEV IspDev, ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDefogAttr->bEnable = hi_vi_dehaze_enable_read();
    pstDefogAttr->u8HorizontalBlock = isp_ext_system_manual_dehaze_hblk_read();
    pstDefogAttr->u8VerticalBlock = isp_ext_system_manual_dehaze_vblk_read();
    pstDefogAttr->enOpType = (ISP_OP_TYPE_E)isp_ext_system_manual_dehaze_enable_read();
    pstDefogAttr->stManual.u8strength = isp_ext_system_manual_dehaze_strength_read();
    pstDefogAttr->stAuto.u8strength = isp_ext_system_manual_dehaze_autostrength_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_ACM_SetAttr(pstAcmAttr);
}
HI_S32 HI_MPI_ISP_GetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_ACM_GetAttr(pstAcmAttr);
}

HI_S32 HI_MPI_ISP_SetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT, ISP_ACM_MODE_E enMode)
{
    HI_S32 s32Ret;
    HI_S32 i = 0, j = 0, k = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (enMode >= ISP_ACM_MODE_BUTT)
    {
        printf("Invalid ACM Mode %d!\n", enMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ACM_Y_NUM; i++)
    {
        for (j = 0; j < ACM_S_NUM; j++)
        {
            for (k = 0; k < ACM_H_NUM; k++)
            {
                if ((pstAcmLUT->as16Y[i][j][k] < -256) || (pstAcmLUT->as16Y[i][j][k] > 256))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16Y[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16Y[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
                if ((pstAcmLUT->as16H[i][j][k] < -64) || (pstAcmLUT->as16H[i][j][k] > 64))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16H[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16H[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
                if ((pstAcmLUT->as16S[i][j][k] < -256) || (pstAcmLUT->as16S[i][j][k] > 256))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16S[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16S[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
            }
        }
    }


    s32Ret = ISP_ACM_SetCoeff(pstAcmLUT,enMode);
    return s32Ret;
}
HI_S32 HI_MPI_ISP_GetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT, ISP_ACM_MODE_E enMode)
{
    HI_S32 s32Ret;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    s32Ret =  ISP_ACM_GetCoeff(pstAcmLUT,enMode);
    return s32Ret;
}

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(ISP_DEV IspDev, ISP_INNER_STATE_INFO_S *pstInnerStateInfo)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstInnerStateInfo);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstInnerStateInfo->u32DRCStrengthActual = hi_isp_drc_strength_read();
    pstInnerStateInfo->u32NRStrengthActual = hi_isp_nr_thresh_short_read();
    pstInnerStateInfo->u32SharpenStrengthDActual = hi_isp_demosaic_sharp_alt_d_read();
    pstInnerStateInfo->u32SharpenStrengthUdActual = hi_isp_demosaic_sharp_alt_ud_read();
    pstInnerStateInfo->u32SharpenStrengthRGBActual = hi_isp_sharpen_strength_read();
    pstInnerStateInfo->u32DefogStrengthActual = hi_vi_dehaze_gstrth_read();
    pstInnerStateInfo->u32WDRExpRatioActual = hi_isp_wdr_exposure_ratio_read();
    pstInnerStateInfo->au32BLActual[0] = hi_ext_system_black_level_query_00_read();
    pstInnerStateInfo->au32BLActual[1] = hi_ext_system_black_level_query_01_read();
    pstInnerStateInfo->au32BLActual[2] = hi_ext_system_black_level_query_10_read();
    pstInnerStateInfo->au32BLActual[3] = hi_ext_system_black_level_query_11_read();

    pstInnerStateInfo->bWDRSwitchFinish = hi_ext_top_wdr_switch_read();
    pstInnerStateInfo->bResSwitchFinish = hi_ext_top_res_switch_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetStatisticsConfig(ISP_DEV IspDev, const ISP_STATISTICS_CFG_S *pstStatCfg)
{
    HI_U8 i;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstStatCfg->stAECfg.au8HistThresh[1] < pstStatCfg->stAECfg.au8HistThresh[0])
    {
        printf("AE Thresh[1] %d should not less than Thresh[0] %d!\n",
			pstStatCfg->stAECfg.au8HistThresh[1], pstStatCfg->stAECfg.au8HistThresh[0]);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstStatCfg->stAECfg.au8HistThresh[2] < pstStatCfg->stAECfg.au8HistThresh[1])
    {
        printf("AE Thresh[2] %d should not less than Thresh[1] %d!\n",
			pstStatCfg->stAECfg.au8HistThresh[2], pstStatCfg->stAECfg.au8HistThresh[1]);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstStatCfg->stAECfg.au8HistThresh[3] < pstStatCfg->stAECfg.au8HistThresh[2])
    {
        printf("AE Thresh[3] %d should not less than Thresh[2] %d!\n",
			pstStatCfg->stAECfg.au8HistThresh[3], pstStatCfg->stAECfg.au8HistThresh[2]);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enAESwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Switch %d in %s!\n", pstStatCfg->stAECfg.enAESwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enAESumSwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Sum Switch %d in %s!\n", pstStatCfg->stAECfg.enAESumSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enHistSwitch >= ISP_AE_HIST_SWITCH_BUTT)
    {
        printf("Invalid AE 256 Histogram Switch %d in %s!\n", pstStatCfg->stAECfg.enHistSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel > 0x3FF)
    {
        printf("Max value of RGB Config u16WhiteLevel is 0x3FF! %d\n", pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CrMax > 0xFFF)
    {
        printf("Max value of RGB Config u16CrMax is 0xFFF!\n %d", pstStatCfg->stWBCfg.stRGBCfg.u16CrMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CbMax > 0xFFF)
    {
        printf("Max value of RGB Config u16CbMax is 0xFFF! %d\n", pstStatCfg->stWBCfg.stRGBCfg.u16CbMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel > pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel)
    {
        printf("RGB Config BlackLevel %d should not larger than WhiteLevel! %d\n",
			pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel, pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CrMin > pstStatCfg->stWBCfg.stRGBCfg.u16CrMax)
    {
        printf("RGB Config CrMin %d should not larger than CrMax! %d\n",
			pstStatCfg->stWBCfg.stRGBCfg.u16CrMin, pstStatCfg->stWBCfg.stRGBCfg.u16CrMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CbMin > pstStatCfg->stWBCfg.stRGBCfg.u16CbMax)
    {
        printf("RGB Config CbMin %d should not larger than CbMax! %d\n",
			pstStatCfg->stWBCfg.stRGBCfg.u16CbMin, pstStatCfg->stWBCfg.stRGBCfg.u16CbMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

#if 0
    /*confilict with the logic default setting, delete*/
    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh > pstStatCfg->stWBCfg.stRGBCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh < pstStatCfg->stWBCfg.stRGBCfg.u16CrMin) )
    {
        printf("RGB Config u16CrHigh should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CrLow> pstStatCfg->stWBCfg.stRGBCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CrLow < pstStatCfg->stWBCfg.stRGBCfg.u16CrMin) )
    {
        printf("RGB Config u16CrLow should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh > pstStatCfg->stWBCfg.stRGBCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh < pstStatCfg->stWBCfg.stRGBCfg.u16CbMin) )
    {
        printf("RGB Config u16CbHigh should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CbLow> pstStatCfg->stWBCfg.stRGBCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CbLow < pstStatCfg->stWBCfg.stRGBCfg.u16CbMin) )
    {
        printf("RGB Config u16CbLow should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
#endif

    if (pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel > 0xFFF)
    {
        printf("Max value of Bayer Config u16WhiteLevel is 0xFFF! %d\n", pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrMax > 0xFFF)
    {
        printf("Max value of Bayer Config u16CrMax is 0xFFF! %d\n", pstStatCfg->stWBCfg.stBayerCfg.u16CrMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbMax > 0xFFF)
    {
        printf("Max value of Bayer Config u16CbMax is 0xFFF! %d\n", pstStatCfg->stWBCfg.stBayerCfg.u16CbMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel > pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel)
    {
        printf("Bayer Config BlackLevel %d should not larger than WhiteLevel %d!\n",
			pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel, pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrMin > pstStatCfg->stWBCfg.stBayerCfg.u16CrMax)
    {
        printf("Bayer CrMin %d should not larger than CrMax %d!\n",
			pstStatCfg->stWBCfg.stBayerCfg.u16CrMin, pstStatCfg->stWBCfg.stBayerCfg.u16CrMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbMin > pstStatCfg->stWBCfg.stBayerCfg.u16CbMax)
    {
        printf("Bayer CbMin %d should not larger than CbMax %d!\n",
			pstStatCfg->stWBCfg.stBayerCfg.u16CbMin, pstStatCfg->stWBCfg.stBayerCfg.u16CbMax);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

#if 0
    /*confilict with the logic default setting, delete*/
    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh > pstStatCfg->stWBCfg.stBayerCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh < pstStatCfg->stWBCfg.stBayerCfg.u16CrMin) )
    {
        printf("Bayer u16CrHigh should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CrLow> pstStatCfg->stWBCfg.stBayerCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CrLow < pstStatCfg->stWBCfg.stBayerCfg.u16CrMin) )
    {
        printf("Bayer u16CrLow should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh > pstStatCfg->stWBCfg.stBayerCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh < pstStatCfg->stWBCfg.stBayerCfg.u16CbMin) )
    {
        printf("Bayer u16CbHigh should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CbLow> pstStatCfg->stWBCfg.stBayerCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CbLow < pstStatCfg->stWBCfg.stBayerCfg.u16CbMin) )
    {
        printf("Bayer u16CbLow should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
#endif

    if ((pstStatCfg->stFocusCfg.stConfig.bEnable != HI_FALSE) && (pstStatCfg->stFocusCfg.stConfig.bEnable != HI_TRUE))
    {
        printf("Invalid AF bEnable input! %d\n", pstStatCfg->stFocusCfg.stConfig.bEnable);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=0;i<3;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[i] != HI_FALSE) && (pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[i] != HI_TRUE))
        {
            printf("Invalid AF stHParam_IIR0.abIIREn[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i=0;i<3;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[i] != HI_FALSE) && (pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[i] != HI_TRUE))
        {
            printf("Invalid AF stHParam_IIR1.abIIREn[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    //if ((pstStatCfg->stFocusCfg.stConfig.enPeakMode >= ISP_AF_STA_BUTT) || (pstStatCfg->stFocusCfg.stConfig.enPeakMode < 0))
    if (pstStatCfg->stFocusCfg.stConfig.enPeakMode >= ISP_AF_STA_BUTT)
    {
        printf("Invalid AF stConfig.enPeakMode input! %d\n", pstStatCfg->stFocusCfg.stConfig.enPeakMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    //if ((pstStatCfg->stFocusCfg.stConfig.enSquMode >= ISP_AF_STA_SUM_BUTT) || (pstStatCfg->stFocusCfg.stConfig.enSquMode < 0))
    if (pstStatCfg->stFocusCfg.stConfig.enSquMode >= ISP_AF_STA_SUM_BUTT)
    {
        printf("Invalid AF stConfig.enSquMode input! %d\n", pstStatCfg->stFocusCfg.stConfig.enSquMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.u16Hwnd > AF_ZONE_COLUMN) || (pstStatCfg->stFocusCfg.stConfig.u16Hwnd <= 0))
    {
        printf("Invalid AF stConfig.u16Hwnd input! %d\n", pstStatCfg->stFocusCfg.stConfig.u16Hwnd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.u16Vwnd > AF_ZONE_ROW) || (pstStatCfg->stFocusCfg.stConfig.u16Vwnd <= 0))
    {
        printf("Invalid AF stConfig.u16Vwnd input! %d\n", pstStatCfg->stFocusCfg.stConfig.u16Vwnd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] > 0xFF) || (pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] < 0))
    {
        printf("Invalid AF stHParam_IIR0.as16IIRGain[0] input! %d\n", pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=1;i<7;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i] > 511) || (pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i] < -511))
        {
            printf("Invalid AF stHParam_IIR0.as16IIRGain[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if ((pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] > 0xFF) || (pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] < 0))
    {
        printf("Invalid AF stHParam_IIR0.as16IIRGain[0] input! %d\n", pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=1;i<7;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i] > 511) || (pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i] < -511))
        {
            printf("Invalid AF stHParam_IIR0.as16IIRGain[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i=0;i<4;i++)
    {
        if (pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[i] > 0xF)
        {
            printf("Invalid AF stHParam_IIR0.au16IIRShift[%d] input! %d\n",i,pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[i] > 0xF)
        {
            printf("Invalid AF stHParam_IIR1.au16IIRShift[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i=0;i<5;i++)
    {
        if ((pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i] > 31) || (pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i] < -31))
        {
            printf("Invalid AF stVParam_FIR0.as16FIRH[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i] > 31) || (pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i] < -31))
        {
            printf("Invalid AF stVParam_FIR0.as16FIRH[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd > 0x7FF)
    {
        printf("Invalid AF stHParam_IIR0.u16IIRThd input! %d\n", pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd > 0x7FF)
    {
        printf("Invalid AF stHParam_IIR1.u16IIRThd input! %d\n", pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd > 0x7FF)
    {
        printf("Invalid AF stVParam_FIR0.u16FIRThd input! %d\n", pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd > 0x7FF)
    {
        printf("Invalid AF stVParam_FIR1.u16FIRThd input! %d\n", pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=0;i<2;i++)
    {
        if (pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[i] > 0xF)
        {
            printf("Invalid AF stFVParam.au16AccShiftH[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[i] > 0xF)
        {
            printf("Invalid AF stFVParam.au16AccShiftV[%d] input! %d\n",i, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY > 0xF)
    {
        printf("Invalid AF stFVParam.u16AccShiftY input! %d\n", pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.u16Hsize < 1) || (pstStatCfg->stFocusCfg.stConfig.u16Hsize > 0xFFF))
    {
        printf("Invalid AF stConfig.u16Hsize input! %d\n", pstStatCfg->stFocusCfg.stConfig.u16Hsize);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.u16Vsize < 1) || (pstStatCfg->stFocusCfg.stConfig.u16Vsize > 0xFFF))
    {
        printf("Invalid AF stConfig.u16Vsize input! %d\n", pstStatCfg->stFocusCfg.stConfig.u16Vsize);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_statistics_ctrl_write(pstStatCfg->unKey.u32Key);

    hi_isp_metering_hist_thresh_0_1_write(pstStatCfg->stAECfg.au8HistThresh[0]);
    hi_isp_metering_hist_thresh_1_2_write(pstStatCfg->stAECfg.au8HistThresh[1]);
    hi_isp_metering_hist_thresh_3_4_write(pstStatCfg->stAECfg.au8HistThresh[2]);
    hi_isp_metering_hist_thresh_4_5_write(pstStatCfg->stAECfg.au8HistThresh[3]);

    hi_isp_top_ae_switch_write(pstStatCfg->stAECfg.enAESwitch);
    hi_isp_top_aesum_switch_write(pstStatCfg->stAECfg.enAESumSwitch);
    hi_isp_top_histogram_switch_write(pstStatCfg->stAECfg.enHistSwitch);

    hi_isp_metering_white_level_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel);
    hi_isp_metering_black_level_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel);
    hi_isp_metering_cr_ref_max_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CrMax);
    hi_isp_metering_cr_ref_min_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CrMin);
    hi_isp_metering_cb_ref_max_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CbMax);
    hi_isp_metering_cb_ref_min_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CbMin);
    hi_isp_metering_cr_ref_high_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh);
    hi_isp_metering_cr_ref_low_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CrLow);
    hi_isp_metering_cb_ref_high_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh);
    hi_isp_metering_cb_ref_low_awb_write(pstStatCfg->stWBCfg.stRGBCfg.u16CbLow);

    hi_isp_metering_max_threshold_write(pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel);
    hi_isp_metering_min_threshold_write(pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel);
    hi_isp_metering_cr_ref_max_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrMax);
    hi_isp_metering_cr_ref_min_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrMin);
    hi_isp_metering_cb_ref_max_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbMax);
    hi_isp_metering_cb_ref_min_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbMin);
    hi_isp_metering_cr_ref_high_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh);
    hi_isp_metering_cr_ref_low_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrLow);
    hi_isp_metering_cb_ref_high_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh);
    hi_isp_metering_cb_ref_low_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbLow);

    hi_vi_af_enable_write(pstStatCfg->stFocusCfg.stConfig.bEnable);
    hi_vi_af_iir0_en0_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0]);
    hi_vi_af_iir0_en1_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1]);
    hi_vi_af_iir0_en2_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2]);
    hi_vi_af_iir1_en0_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0]);
    hi_vi_af_iir1_en1_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1]);
    hi_vi_af_iir1_en2_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2]);
    hi_vi_af_fvmode_peak_write(pstStatCfg->stFocusCfg.stConfig.enPeakMode);
    hi_vi_af_fvmode_squ_write(pstStatCfg->stFocusCfg.stConfig.enSquMode);
    hi_vi_af_hwnd_write(pstStatCfg->stFocusCfg.stConfig.u16Hwnd);
    hi_vi_af_vwnd_write(pstStatCfg->stFocusCfg.stConfig.u16Vwnd);
    hi_vi_af_iir0_gain0_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
    hi_vi_af_iir1_gain0_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
    hi_vi_af_iir0_gain1_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1]);
    hi_vi_af_iir1_gain1_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1]);
    hi_vi_af_iir0_gain2_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2]);
    hi_vi_af_iir1_gain2_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2]);
    hi_vi_af_iir0_gain3_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3]);
    hi_vi_af_iir1_gain3_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3]);
    hi_vi_af_iir0_gain4_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4]);
    hi_vi_af_iir1_gain4_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4]);
    hi_vi_af_iir0_gain5_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5]);
    hi_vi_af_iir1_gain5_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5]);
    hi_vi_af_iir0_gain6_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6]);
    hi_vi_af_iir1_gain6_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6]);
    hi_vi_af_iir0_shift0_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0]);
    hi_vi_af_iir0_shift1_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1]);
    hi_vi_af_iir0_shift2_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2]);
    hi_vi_af_iir0_shift3_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3]);
    hi_vi_af_iir1_shift0_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0]);
    hi_vi_af_iir1_shift1_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1]);
    hi_vi_af_iir1_shift2_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2]);
    hi_vi_af_iir1_shift3_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3]);
    hi_vi_af_fir0_h0_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0]);
    hi_vi_af_fir1_h0_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0]);
    hi_vi_af_fir0_h1_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1]);
    hi_vi_af_fir1_h1_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1]);
    hi_vi_af_fir0_h2_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2]);
    hi_vi_af_fir1_h2_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2]);
    hi_vi_af_fir0_h3_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3]);
    hi_vi_af_fir1_h3_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3]);
    hi_vi_af_fir0_h4_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4]);
    hi_vi_af_fir1_h4_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4]);
    hi_vi_af_iir0_thd_x_write(pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd);
    hi_vi_af_iir1_thd_x_write(pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd);
    hi_vi_af_fir0_thd_y_write(pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd);
    hi_vi_af_fir1_thd_y_write(pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd);
    hi_vi_af_acc_shift_h0_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0]);
    hi_vi_af_acc_shift_h1_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1]);
    hi_vi_af_acc_shift_v0_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0]);
    hi_vi_af_acc_shift_v1_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1]);
    hi_vi_af_acc_shift_y_write(pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY);
    hi_vi_af_hsize_write(pstStatCfg->stFocusCfg.stConfig.u16Hsize - 1);
    hi_vi_af_vsize_write(pstStatCfg->stFocusCfg.stConfig.u16Vsize - 1);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetStatisticsConfig(ISP_DEV IspDev, ISP_STATISTICS_CFG_S *pstStatCfg)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    HI_U8  u8Tmp;

    pstStatCfg->unKey.u32Key = hi_ext_system_statistics_ctrl_read();
    pstStatCfg->stAECfg.au8HistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
    pstStatCfg->stAECfg.au8HistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
    pstStatCfg->stAECfg.au8HistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
    pstStatCfg->stAECfg.au8HistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();

    u8Tmp = hi_isp_top_ae_switch_read();
    pstStatCfg->stAECfg.enAESwitch = (u8Tmp > 7) ? ISP_AE_SWITCH_BUTT : u8Tmp;
    u8Tmp = hi_isp_top_aesum_switch_read();
    pstStatCfg->stAECfg.enAESumSwitch = (u8Tmp > 7) ? ISP_AE_SWITCH_BUTT : u8Tmp;
    u8Tmp = hi_isp_top_histogram_switch_read();
    pstStatCfg->stAECfg.enHistSwitch = (u8Tmp > 7) ? ISP_AE_HIST_SWITCH_BUTT : u8Tmp;

    pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel = hi_isp_metering_white_level_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel = hi_isp_metering_black_level_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CrMax = hi_isp_metering_cr_ref_max_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CrMin = hi_isp_metering_cr_ref_min_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CbMax = hi_isp_metering_cb_ref_max_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CbMin = hi_isp_metering_cb_ref_min_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh = hi_isp_metering_cr_ref_high_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CrLow = hi_isp_metering_cr_ref_low_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh = hi_isp_metering_cb_ref_high_awb_read();
    pstStatCfg->stWBCfg.stRGBCfg.u16CbLow = hi_isp_metering_cb_ref_low_awb_read();

    pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel = hi_isp_metering_max_threshold_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel = hi_isp_metering_min_threshold_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMax = hi_isp_metering_cr_ref_max_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMin = hi_isp_metering_cr_ref_min_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMax = hi_isp_metering_cb_ref_max_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMin = hi_isp_metering_cb_ref_min_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh = hi_isp_metering_cr_ref_high_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrLow = hi_isp_metering_cr_ref_low_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh = hi_isp_metering_cb_ref_high_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbLow = hi_isp_metering_cb_ref_low_awb_sum_read();

    pstStatCfg->stFocusCfg.stConfig.bEnable = hi_vi_af_enable_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0] = hi_vi_af_iir0_en0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1] = hi_vi_af_iir0_en1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2] = hi_vi_af_iir0_en2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0] = hi_vi_af_iir1_en0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1] = hi_vi_af_iir1_en1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2] = hi_vi_af_iir1_en2_read();
    pstStatCfg->stFocusCfg.stConfig.enPeakMode = hi_vi_af_fvmode_peak_read();
    pstStatCfg->stFocusCfg.stConfig.enSquMode = hi_vi_af_fvmode_squ_read();
    pstStatCfg->stFocusCfg.stConfig.u16Hwnd = hi_vi_af_hwnd_read();
    pstStatCfg->stFocusCfg.stConfig.u16Vwnd = hi_vi_af_vwnd_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] = (HI_S16)hi_vi_af_iir0_gain0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] = (HI_S16)hi_vi_af_iir1_gain0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1] = (HI_S16)hi_vi_af_iir0_gain1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1] = (HI_S16)hi_vi_af_iir1_gain1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2] = (HI_S16)hi_vi_af_iir0_gain2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2] = (HI_S16)hi_vi_af_iir1_gain2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3] = (HI_S16)hi_vi_af_iir0_gain3_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3] = (HI_S16)hi_vi_af_iir1_gain3_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4] = (HI_S16)hi_vi_af_iir0_gain4_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4] = (HI_S16)hi_vi_af_iir1_gain4_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5] = (HI_S16)hi_vi_af_iir0_gain5_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5] = (HI_S16)hi_vi_af_iir1_gain5_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6] = (HI_S16)hi_vi_af_iir0_gain6_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6] = (HI_S16)hi_vi_af_iir1_gain6_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0] = hi_vi_af_iir0_shift0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1] = hi_vi_af_iir0_shift1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2] = hi_vi_af_iir0_shift2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3] = hi_vi_af_iir0_shift3_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0] = hi_vi_af_iir1_shift0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1] = hi_vi_af_iir1_shift1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2] = hi_vi_af_iir1_shift2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3] = hi_vi_af_iir1_shift3_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0] = (HI_S16)hi_vi_af_fir0_h0_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0] = (HI_S16)hi_vi_af_fir1_h0_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1] = (HI_S16)hi_vi_af_fir0_h1_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1] = (HI_S16)hi_vi_af_fir1_h1_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2] = (HI_S16)hi_vi_af_fir0_h2_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2] = (HI_S16)hi_vi_af_fir1_h2_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3] = (HI_S16)hi_vi_af_fir0_h3_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3] = (HI_S16)hi_vi_af_fir1_h3_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4] = (HI_S16)hi_vi_af_fir0_h4_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4] = (HI_S16)hi_vi_af_fir1_h4_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd = hi_vi_af_iir0_thd_x_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd = hi_vi_af_iir1_thd_x_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd = hi_vi_af_fir0_thd_y_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd = hi_vi_af_fir1_thd_y_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0] = hi_vi_af_acc_shift_h0_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1] = hi_vi_af_acc_shift_h1_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0] = hi_vi_af_acc_shift_v0_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1] = hi_vi_af_acc_shift_v1_read();
    pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY = hi_vi_af_acc_shift_y_read();
    pstStatCfg->stFocusCfg.stConfig.u16Hsize = hi_vi_af_hsize_read() + 1;
    pstStatCfg->stFocusCfg.stConfig.u16Vsize = hi_vi_af_vsize_read() + 1;
#if 0
printf("%s :\n %d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d\n",
	__func__,
	pstStatCfg->unKey.u32Key,
    pstStatCfg->stAECfg.au8HistThresh[0],
    pstStatCfg->stAECfg.au8HistThresh[1],
    pstStatCfg->stAECfg.au8HistThresh[2],
    pstStatCfg->stAECfg.au8HistThresh[3],
    pstStatCfg->stAECfg.enAESwitch,
    pstStatCfg->stAECfg.enAESumSwitch,
    pstStatCfg->stAECfg.enHistSwitch,
    pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel,
    pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel,
    pstStatCfg->stWBCfg.stRGBCfg.u16CrMax,
    pstStatCfg->stWBCfg.stRGBCfg.u16CrMin,
    pstStatCfg->stWBCfg.stRGBCfg.u16CbMax,
    pstStatCfg->stWBCfg.stRGBCfg.u16CbMin,
    pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh,
    pstStatCfg->stWBCfg.stRGBCfg.u16CrLow,
    pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh,
    pstStatCfg->stWBCfg.stRGBCfg.u16CbLow,
    pstStatCfg->stFocusCfg.stConfig.bEnable,
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0],
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1],
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2],
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0],
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1],
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2]
);

printf("%d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d\n",
	pstStatCfg->stFocusCfg.stConfig.enPeakMode,
    pstStatCfg->stFocusCfg.stConfig.enSquMode,
    pstStatCfg->stFocusCfg.stConfig.u16Hwnd,
    pstStatCfg->stFocusCfg.stConfig.u16Vwnd,
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0],
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1],
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2],
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3],
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4],
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5],
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6],
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6],
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0],
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1],
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2],
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3],
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0],
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1],
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2],
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3],
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0],
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0],
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1],
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1]
	);
	printf("%d %d %d %d %d, %d %d %d %d %d, %d %d %d %d %d, %d %d\n",
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2],
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2],
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3],
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3],
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4],
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4],
    pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd,
    pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd,
    pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd,
    pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd,
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0],
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1],
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0],
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1],
    pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY,
    pstStatCfg->stFocusCfg.stConfig.u16Hsize,
    pstStatCfg->stFocusCfg.stConfig.u16Vsize);
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetStatistics(ISP_DEV IspDev, ISP_STATISTICS_S *pstStat)
{
    HI_S32 i, j;
    HI_U32 u32Sum;
    HI_U32 u32Hist10, u32Hist43;
    HI_U32 u32HighBit = 0, u32LowBit = 0;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    //if (pstStat->unKey.bit1AeStat1)
    {
        pstStat->stAEStat.au16Hist5Value[0] = hi_isp_metering_hist_0_read();
        pstStat->stAEStat.au16Hist5Value[1] = hi_isp_metering_hist_1_read();
        pstStat->stAEStat.au16Hist5Value[3] = hi_isp_metering_hist_3_read();
        pstStat->stAEStat.au16Hist5Value[4] = hi_isp_metering_hist_4_read();

        u32Sum = pstStat->stAEStat.au16Hist5Value[0]
            + pstStat->stAEStat.au16Hist5Value[1]
            + pstStat->stAEStat.au16Hist5Value[3]
            + pstStat->stAEStat.au16Hist5Value[4];
        if(u32Sum > 0xFFFF)
        {
            u32Sum = 0xFFFF;
        }
        pstStat->stAEStat.au16Hist5Value[2] = 0xFFFF - u32Sum;
    }

    //if (pstStat->unKey.bit1AeStat2)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j < AE_ZONE_COLUMN ;j++)
            {
                u32Hist10 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 0);
                u32Hist43 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 1);

                pstStat->stAEStat.au16ZoneHist5Value[i][j][0] = (HI_U16)(u32Hist10 & 0xffff);
                pstStat->stAEStat.au16ZoneHist5Value[i][j][1] = (HI_U16)((u32Hist10 & 0xffff0000) >> 16);
                pstStat->stAEStat.au16ZoneHist5Value[i][j][3] = (HI_U16)(u32Hist43 & 0xffff);
                pstStat->stAEStat.au16ZoneHist5Value[i][j][4] = (HI_U16)((u32Hist43 & 0xffff0000) >> 16);

                u32Sum = pstStat->stAEStat.au16ZoneHist5Value[i][j][0]
                    + pstStat->stAEStat.au16ZoneHist5Value[i][j][1]
                    + pstStat->stAEStat.au16ZoneHist5Value[i][j][3]
                    + pstStat->stAEStat.au16ZoneHist5Value[i][j][4];
                if(u32Sum > 0xFFFF)
                {
                    u32Sum = 0xFFFF;
                }

                pstStat->stAEStat.au16ZoneHist5Value[i][j][2] = 0xFFFF - u32Sum;
            }
        }
    }

    //if (pstStat->unKey.bit1AeStat3)
    {
        for(i = 0; i < 256; i++)
        {
            pstStat->stAEStat.au32Hist256Value[i] = (HI_U16)(hi_histogram_mem_array_data_read(i) & 0xffff);
            u32HighBit = pstStat->stAEStat.au32Hist256Value[i] >> 12;
            u32LowBit  = pstStat->stAEStat.au32Hist256Value[i] & 0xfff;
            if(u32HighBit == 0)
            {
                pstStat->stAEStat.au32Hist256Value[i] = u32LowBit;
            }
            else
            {
                pstStat->stAEStat.au32Hist256Value[i] = (4096 + u32LowBit) << (u32HighBit - 1);
            }
        }
    }

    //if (pstStat->unKey.bit1AeStat4)
    {
        pstStat->stAEStat.au16GlobalAvg[0]  = hi_isp_metering_sum_r_read();
        pstStat->stAEStat.au16GlobalAvg[1] = hi_isp_metering_sum_rg_read();
        pstStat->stAEStat.au16GlobalAvg[2] = hi_isp_metering_sum_bg_read();
        pstStat->stAEStat.au16GlobalAvg[3]  = hi_isp_metering_sum_b_read();
    }

    //if (pstStat->unKey.bit1AeStat5)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                pstStat->stAEStat.au16ZoneAvg[i][j][0] = hi_ae_sum_array_data_read_r(i*AE_ZONE_COLUMN+j);   /*R */
                pstStat->stAEStat.au16ZoneAvg[i][j][1] = hi_ae_sum_array_data_read_gr(i*AE_ZONE_COLUMN+j);  /*Gr*/
                pstStat->stAEStat.au16ZoneAvg[i][j][2] = hi_ae_sum_array_data_read_gb(i*AE_ZONE_COLUMN+j);  /*Gb*/
                pstStat->stAEStat.au16ZoneAvg[i][j][3] = hi_ae_sum_array_data_read_b(i*AE_ZONE_COLUMN+j);   /*B */
            }
        }
    }

    //if (pstStat->unKey.bit1AwbStat1)
    {
        pstStat->stWBStat.stRGBStatistics.u16GlobalGR = hi_isp_metering_awb_rg_read();
        pstStat->stWBStat.stRGBStatistics.u16GlobalGB = hi_isp_metering_awb_bg_read();
        pstStat->stWBStat.stRGBStatistics.u32GlobalSum = hi_isp_metering_awb_sum_read();
    }

    //if (pstStat->unKey.bit1AwbStat2)
    {
        for(i = 0; i < AWB_ZONE_ROW; i++)
        {
            for(j = 0; j < AWB_ZONE_COLUMN; j++)
            {
                HI_U32 u32BgRg = hi_metering_mem_array_data_read_bg_rg(i*AWB_ZONE_COLUMN+j);
                HI_U32 u32Sum = hi_metering_mem_array_data_read_sum(i*AWB_ZONE_COLUMN+j);

                pstStat->stWBStat.stRGBStatistics.au16ZoneGR[i][j] = u32BgRg & 0xfff;
                pstStat->stWBStat.stRGBStatistics.au16ZoneGB[i][j] = (u32BgRg >> 16) & 0xfff;
                pstStat->stWBStat.stRGBStatistics.au32ZoneSum[i][j] = u32Sum;
            }
        }
    }

    //if (pstStat->unKey.bit1AwbStat3)
    {
        HI_U16 u16BlackLevelR = ((HI_U16)hi_isp_offset_black_00_read() << 4);
        HI_U16 u16BlackLevelG = ((HI_U16)hi_isp_offset_black_01_read() << 4);
        HI_U16 u16BlackLevelB = ((HI_U16)hi_isp_offset_black_11_read() << 4);

        pstStat->stWBStat.stBayerStatistics.u16GlobalR = hi_isp_metering_avg_r_read();
        pstStat->stWBStat.stBayerStatistics.u16GlobalG = hi_isp_metering_avg_g_read();
        pstStat->stWBStat.stBayerStatistics.u16GlobalB = hi_isp_metering_avg_b_read();
        pstStat->stWBStat.stBayerStatistics.u16CountAll = hi_isp_metering_count_all_read();
        pstStat->stWBStat.stBayerStatistics.u16CountMin = hi_isp_metering_count_min_read();
        pstStat->stWBStat.stBayerStatistics.u16CountMax= hi_isp_metering_count_max_read();

        if (0 != pstStat->stWBStat.stBayerStatistics.u16CountAll)
        {
            if ((pstStat->stWBStat.stBayerStatistics.u16GlobalR >= u16BlackLevelR) &&
                (pstStat->stWBStat.stBayerStatistics.u16GlobalG >= u16BlackLevelG) &&
                (pstStat->stWBStat.stBayerStatistics.u16GlobalB >= u16BlackLevelB))
            {
                pstStat->stWBStat.stBayerStatistics.u16GlobalR -= u16BlackLevelR;
                pstStat->stWBStat.stBayerStatistics.u16GlobalG -= u16BlackLevelG;
                pstStat->stWBStat.stBayerStatistics.u16GlobalB -= u16BlackLevelB;
            }
            else
            {
                pstStat->stWBStat.stBayerStatistics.u16GlobalR = 0;
                pstStat->stWBStat.stBayerStatistics.u16GlobalG = 0;
                pstStat->stWBStat.stBayerStatistics.u16GlobalB = 0;
                pstStat->stWBStat.stBayerStatistics.u16CountAll = 0x0;
                pstStat->stWBStat.stBayerStatistics.u16CountMin = 0xFFFF;
                pstStat->stWBStat.stBayerStatistics.u16CountMax= 0x0;
            }
        }
    }

    //if (pstStat->unKey.bit1AwbStat4)
    {
        HI_U16 u16BlackLevelR = ((HI_U16)hi_isp_offset_black_00_read() << 4);
        HI_U16 u16BlackLevelG = ((HI_U16)hi_isp_offset_black_01_read() << 4);
        HI_U16 u16BlackLevelB = ((HI_U16)hi_isp_offset_black_11_read() << 4);

        for(i = 0; i < AWB_ZONE_ROW; i++)
        {
            for(j = 0; j < AWB_ZONE_COLUMN; j++)
            {
                HI_U32 u32GR = hi_awb_sum_array_data_read_avg_g_r(i*AWB_ZONE_COLUMN+j);
                HI_U32 u32ALLB = hi_awb_sum_array_data_read_all_avgb(i*AWB_ZONE_COLUMN+j);
                HI_U32 u32MINMAX = hi_awb_sum_array_data_read_min_max(i*AWB_ZONE_COLUMN+j);

                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] = u32GR & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] = (u32GR >> 16) & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] = u32ALLB & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i][j] = (u32ALLB >> 16) & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i][j] = u32MINMAX & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i][j] = (u32MINMAX >> 16) & 0xffff;

                if (0 != pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i][j])
                {
                    if ((pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] >= u16BlackLevelR) &&
                        (pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] >= u16BlackLevelG) &&
                        (pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] >= u16BlackLevelB))
                    {
                        pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] -= u16BlackLevelR;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] -= u16BlackLevelG;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] -= u16BlackLevelB;
                    }
                    else
                    {
                        pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] = 0;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] = 0;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] = 0;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i][j] = 0;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i][j] = 0;
                        pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i][j] = 0xFFFF;
                    }
                }
            }
        }
    }


    HI_U32 u32Zone = hi_area_block_distribute_data_read_af();
    HI_U8 u8Col = ((u32Zone & 0x1F) > AF_ZONE_COLUMN) ? AF_ZONE_COLUMN : (u32Zone & 0x1F);
    HI_U8 u8Row = (((u32Zone & 0x1F00) >> 8) > AF_ZONE_ROW) ? AF_ZONE_ROW : ((u32Zone & 0x1F00) >> 8);
    for (i = 0; i < u8Row; i++)
    {
        for (j = 0; j < u8Col; j++)
        {
            pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & hi_area_stat_mem_array_data_read_af_v1h1(i * u8Col + j)) >> 16);
            pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & hi_area_stat_mem_array_data_read_af_v1h1(i * u8Col + j));
            pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & hi_area_stat_mem_array_data_read_af_v2h2(i * u8Col + j)) >> 16);
            pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & hi_area_stat_mem_array_data_read_af_v2h2(i * u8Col + j));
            pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & hi_area_stat_mem_array_data_read_af_y(i * u8Col + j));
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetISPRegAttr(ISP_DEV IspDev, ISP_REG_ATTR_S *pstIspRegAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspRegAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    (void)pthread_mutex_lock(&pstIspCtx->stLock);
    VReg_Munmap();

    pstIspRegAttr->u32IspRegAddr    = VReg_GetVirtAddr(ISP_REG_BASE);
    pstIspRegAttr->u32IspRegSize    = ISP_REG_SIZE_BIN;
    pstIspRegAttr->u32IspExtRegAddr = VReg_GetVirtAddr(ISP_VREG_BASE);
    pstIspRegAttr->u32IspExtRegSize = ISP_VREG_SIZE_BIN;
    pstIspRegAttr->u32AeExtRegAddr  = VReg_GetVirtAddr(AE_LIB_VREG_BASE(0));
    pstIspRegAttr->u32AeExtRegSize  = AE_LIB_VREG_SIZE_BIN;
    pstIspRegAttr->u32AwbExtRegAddr = VReg_GetVirtAddr(AWB_LIB_VREG_BASE(0));
    pstIspRegAttr->u32AwbExtRegSize = AWB_LIB_VREG_SIZE_BIN;

    (void)pthread_mutex_unlock(&pstIspCtx->stLock);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDebug(ISP_DEV IspDev, const ISP_DEBUG_INFO_S *pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_BOOL(pstIspDebug->bDebugEn);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_DbgSet(pstIspDebug);
}

HI_S32 HI_MPI_ISP_GetDebug(ISP_DEV IspDev, ISP_DEBUG_INFO_S * pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_DbgGet(pstIspDebug);
}

HI_S32 HI_MPI_ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(0);
    ISP_CHECK_MEM_INIT(0);

	return 	ioctl(g_as32IspFd[0], ISP_SET_MOD_PARAM, pstIspModParam);
}

HI_S32 HI_MPI_ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(0);
    ISP_CHECK_MEM_INIT(0);

    return ioctl(g_as32IspFd[0], ISP_GET_MOD_PARAM, pstIspModParam);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

