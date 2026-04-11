/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_defect_pixel.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include "isp_sensor.h"
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ----------         GREEN EQUALIZE ------------------------------------------------------
#define GREEN_EQUALIZE_GE_STRENGTH 0x10
#define GREEN_EQUALIZE_DP_THRESHOLD 0xBC
#define GREEN_EQUALIZE_GE_THRESHOLD 0x10E
#define GREEN_EQUALIZE_DP_SLOPE 0x0             /* 0x0 in normal scene, 0x200 in lowlight scene. */
#define GREEN_EQUALIZE_DP_SLOPE_LOWLIGHT 0x200       /* 0x0 in normal scene, 0x200 in lowlight scene. */
#define GREEN_EQUALIZE_DP_DETECT_TYPE 0x0
#define GREEN_EQUALIZE_GE_SLOPE 0x478
#define GREEN_EQUALIZE_GE_SENS 0xDB

#define HPDEV_THRESHOLD 0x90A

typedef struct hiISP_DEFECT_PIXEL_S
{
    /* Public */
    HI_U16  u16StaticDPThresh;
    HI_U8  u8PixelDetectType;

    /* Private */
    HI_U8  u8FrameCnt;
    HI_U8  u8ShowStaticPixel;
    HI_S8  s8Finished;
    HI_S8  s8Started;
    HI_U8  u8TrialCount;
    HI_U8  u8TrialCntLimit;
    HI_U16 u16BadPixelsCount;
    HI_U16 u16DynamicDpSlope;
    HI_U16 u16DynamicDpThresh;
    HI_BOOL bBadPixelsEn;
    HI_U32 u32StructSize;
} ISP_DEFECT_PIXEL_S;

ISP_DEFECT_PIXEL_S g_astDpCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DP_GET_CTX(dev, pstCtx)   pstCtx = &g_astDpCtx[dev]

static HI_VOID DpExtRegsDefault(HI_VOID)
{
    /* initial register of defect pixel calibration */
    hi_ext_system_calibrate_bad_pixels_write(HI_EXT_SYSTEM_CALIBRATE_BAD_PIXELS_DEFAULT);
    hi_ext_system_bad_pixel_count_max_write(HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MAX_DEFAULT);
    hi_ext_system_bad_pixel_count_min_write(HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MIN_DEFAULT);
    hi_ext_system_bad_pixel_start_thresh_write(HI_EXT_SYSTEM_BAD_PIXEL_START_THRESH_DEFAULT);
    hi_ext_system_bad_pixel_finish_thresh_write(HI_EXT_SYSTEM_BAD_PIXEL_FINISH_THRESH_DEFAULT);
    hi_ext_system_bad_pixel_trigger_status_write(HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_STATUS_DEFAULT);
    hi_ext_system_bad_pixel_trigger_time_write(HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_TIME_DEFAULT);
    hi_ext_system_bad_pixel_detect_type_write(HI_EXT_SYSTEM_BAD_PIXEL_DETECT_TYPE_DEFAULT);
    hi_ext_system_dynamic_defect_pixel_slope_write(HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_SLOPE_DEFAULT);
    hi_ext_system_dynamic_defect_pixel_thresh_write(HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_THRESH_DEFAULT);
    
    return;
}

static HI_VOID DpRegsDefault(HI_VOID) 
{
    return;
}

static HI_VOID DpExtRegsInitialize(ISP_DEV IspDev)
{
    hi_ext_system_dynamic_defect_pixel_thresh_write(GREEN_EQUALIZE_DP_THRESHOLD);
    hi_ext_system_dynamic_defect_pixel_slope_write(GREEN_EQUALIZE_DP_SLOPE);
    hi_ext_system_bad_pixel_detect_type_write(GREEN_EQUALIZE_DP_DETECT_TYPE);
    return;
}

static HI_VOID DpRegsInitialize(ISP_DEV IspDev)
{
    hi_isp_defect_pixel_enable_write(1);

    /* enable correction */
    hi_isp_green_equalize_ge_enable_write(1);
    hi_isp_green_equalize_dp_enable_write(1);
    //  Strength for counting static hot pixels
    hi_isp_green_equalize_dp_threshold_write(GREEN_EQUALIZE_DP_THRESHOLD);
    hi_isp_green_equalize_dp_slope_write(GREEN_EQUALIZE_DP_SLOPE);
    hi_isp_green_equalize_ge_strength_write(GREEN_EQUALIZE_GE_STRENGTH);
    hi_isp_green_equalize_ge_threshold_write(GREEN_EQUALIZE_GE_THRESHOLD);
    hi_isp_green_equalize_ge_slope_write(GREEN_EQUALIZE_GE_SLOPE);
    hi_isp_green_equalize_ge_sens_write(GREEN_EQUALIZE_GE_SENS);
    hi_isp_raw_frontend_hpdev_threshold_write(HPDEV_THRESHOLD);


    return;
}

static HI_S32 DpReadExtregs(ISP_DEV IspDev)
{
    ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;
    
    DP_GET_CTX(IspDev, pstDp);
    
    pstDp->u8TrialCntLimit = (HI_U8)(hi_ext_system_bad_pixel_trigger_time_read() >> 3);
    pstDp->bBadPixelsEn = hi_ext_system_calibrate_bad_pixels_read();
    
    pstDp->u16DynamicDpSlope = hi_ext_system_dynamic_defect_pixel_slope_read();
    pstDp->u16DynamicDpThresh = hi_ext_system_dynamic_defect_pixel_thresh_read();
    pstDp->u8PixelDetectType = hi_ext_system_bad_pixel_detect_type_read();
    return 0;
}

static __inline HI_U8 DpIsValid(ISP_DEFECT_PIXEL_S * pstDp)
{
    return (pstDp->u32StructSize != sizeof(ISP_DEFECT_PIXEL_S));
}

static HI_S32 DpInit(ISP_DEFECT_PIXEL_S *pstDp)
{
    pstDp->u32StructSize = sizeof(ISP_DEFECT_PIXEL_S);

    pstDp->u8FrameCnt = 0;
    pstDp->u16BadPixelsCount = 0;
    /* read the start thresh from user setting, not the default value of ISP */
    //pstDp->bad_pixel_threshold = hi_ext_system_bad_pixel_start_thresh_read();
    pstDp->u8TrialCount = 0;

    return HI_SUCCESS;
}

static HI_S32 DpEnter(ISP_DEV IspDev, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg)
{
    if (0 == pstDp->u8PixelDetectType)
    {
        /* slow framerate to 5fps */
        ISP_SensorSetPixelDetect(IspDev, HI_TRUE);

        /*Should bypass digital gain when enter defect pixel calibration*/
        hi_isp_top_bypass_digital_gain_write(1);
    }
    else
    {
        /* do nothing */
    }
    
    /* read the start thresh from user setting, not the default value of ISP */
    pstDp->u16StaticDPThresh = hi_ext_system_bad_pixel_start_thresh_read();

    pstDp->u8ShowStaticPixel = hi_isp_defect_pixel_show_static_defect_pixels_read();
    
    pstDp->s8Started = 1;

    return 0;
}

static HI_S32 DpExit(ISP_DEV IspDev, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg)
{    
    DpInit(pstDp);

    if (0 == pstDp->u8PixelDetectType)
    {
        ISP_SensorSetPixelDetect(IspDev, HI_FALSE);

        /*Not bypass digital gain when finish defect pixel calibration*/
        hi_isp_top_bypass_digital_gain_write(0);
    }
    else
    {
        /* do nothing */
    }

    pstRegCfg->stDpRegCfg.u8ShowStaticDefectPixels = pstDp->u8ShowStaticPixel;
    hi_isp_defect_pixel_show_static_defect_pixels_write(pstDp->u8ShowStaticPixel);

    pstDp->s8Started = 0;

    return 0;
}

static HI_S32 DpReset(ISP_DEV IspDev, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg)
{
    if (pstDp->s8Started)   /* for Static DPC */
    {
        DpExit(IspDev, pstDp, pstRegCfg);
        
        pstRegCfg->unKey.bit1DpCfg = 1;
        pstRegCfg->stGeRegCfg.u8GeStrength = 0; /* the default value is 0, added by d00181140, 2012.04.21, green equalize */
        pstRegCfg->unKey.bit1GeCfg = 1;
    }
    else    /* for Dynamic DPC */
    {
        pstRegCfg->stDpRegCfg.u16DpSlope = pstDp->u16DynamicDpSlope;
        pstRegCfg->stDpRegCfg.u16DPThresh = pstDp->u16DynamicDpThresh;
        
        hi_isp_green_equalize_dp_slope_write(pstDp->u16DynamicDpSlope);
        hi_isp_green_equalize_dp_threshold_write(pstDp->u16DynamicDpThresh);
    }

    return 0;
}

HI_S32 ISP_DpInit(ISP_DEV IspDev)
{
    ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;
    
    DP_GET_CTX(IspDev, pstDp);

    DpRegsDefault();
    DpExtRegsDefault();
    DpRegsInitialize(IspDev);
    DpExtRegsInitialize(IspDev);

    DpInit(pstDp);

    return HI_SUCCESS;
}

#define WAIT_FRM_NUM_MAX 1 /* wait for AE stop update sensor in the kernel */

HI_S32 ISP_DpRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_CTX_S           *pstIspCtx  = HI_NULL;
    ISP_DEFECT_PIXEL_S  *pstDp      = HI_NULL;
    //ISP_STAT_S          *pstStat    = (ISP_STAT_S *)pStatInfo;
    ISP_REG_CFG_S       *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;
    
    DP_GET_CTX(IspDev, pstDp);    
    ISP_GET_CTX(IspDev, pstIspCtx);
    DpReadExtregs(IspDev);

    if (!pstDp->bBadPixelsEn)
    {
        DpReset(IspDev, pstDp, pstRegCfg);
        pstIspCtx->stLinkage.bDefectPixel = HI_FALSE;
        return HI_SUCCESS;
    }
    else
    {
        pstIspCtx->stLinkage.bDefectPixel = HI_TRUE;
        pstRegCfg->unKey.u32Key = 0;        /* clear the other algorithms's reg config */
    }
    
    if (0 != DpIsValid(pstDp))
    {
        return HI_FAILURE;
    }

    pstDp->s8Finished = 0;

    if(0 == pstDp->u8PixelDetectType)
    {
        hi_isp_green_equalize_bright_disable_write(0);
        hi_isp_green_equalize_dark_disable_write(1);
            
        if(pstDp->u8FrameCnt < 9 + WAIT_FRM_NUM_MAX)
        {
            if(pstDp->u8FrameCnt == 1 + WAIT_FRM_NUM_MAX)
            {            
                DpEnter(IspDev, pstDp, pstRegCfg);
                pstRegCfg->stGeRegCfg.u8GeStrength = 0x20;  
                pstRegCfg->unKey.bit1GeCfg = 1;             
                pstRegCfg->stDpRegCfg.u16DpSlope = HI_ISP_GREEN_EQUALIZE_DP_SLOPE_DEFAULT;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }
            pstDp->u8FrameCnt++;
    
            if(pstDp->u8FrameCnt == 3 + WAIT_FRM_NUM_MAX)
            {
                pstRegCfg->stDpRegCfg.u16DPThresh = pstDp->u16StaticDPThresh;
                pstRegCfg->stDpRegCfg.u16DpCountIn = 0;
                pstRegCfg->stDpRegCfg.u8ShowStaticDefectPixels = 1;
                pstRegCfg->stDpRegCfg.u8DpEnable = 1;
                pstRegCfg->stDpRegCfg.u8DpPointerReset = 1;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }    
    
            if(pstDp->u8FrameCnt == 4 + WAIT_FRM_NUM_MAX)
            {
                pstRegCfg->stDpRegCfg.u8DpPointerReset = 0;
                pstRegCfg->stDpRegCfg.u8DpDetectionTrigger = 1;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }
                
            if(pstDp->u8FrameCnt == 6 + WAIT_FRM_NUM_MAX)
            {
                pstRegCfg->stDpRegCfg.u8DpDetectionTrigger = 0;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }

            if (pstDp->u8FrameCnt == 8 + WAIT_FRM_NUM_MAX)
            {
                pstDp->u16BadPixelsCount = hi_isp_defect_pixel_defect_pixel_count_read();
                
                pstRegCfg->stDpRegCfg.u8ShowStaticDefectPixels = 0;
                pstRegCfg->unKey.bit1DpCfg = 1;
            
                if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit)
                {
                    printf("BAD PIXEL CALIBRATION TIME OUT  %x\n", pstDp->u8TrialCntLimit);
                    DpExit(IspDev, pstDp, pstRegCfg); 
                    pstRegCfg->stGeRegCfg.u8GeStrength = 0; 
                    pstRegCfg->unKey.bit1GeCfg = 1;
                    
                    pstDp->s8Finished = -1;
                    hi_ext_system_bad_pixel_trigger_status_write(0x2);
                }
                else if (pstDp->u16BadPixelsCount > hi_ext_system_bad_pixel_count_max_read())
                {
                    printf("BAD_PIXEL_COUNT_UPPER_LIMIT %x, %x\n", pstDp->u16StaticDPThresh, pstDp->u16BadPixelsCount );
                    pstDp->u8FrameCnt = 2 + WAIT_FRM_NUM_MAX;
                    pstDp->u16StaticDPThresh++;
                    pstDp->u8TrialCount ++;
                }
                else if (pstDp->u16BadPixelsCount < hi_ext_system_bad_pixel_count_min_read())
                {
                    printf("BAD_PIXEL_COUNT_LOWER_LIMIT %x, %x\n", pstDp->u16StaticDPThresh, pstDp->u16BadPixelsCount);
                    pstDp->u8FrameCnt = 2 + WAIT_FRM_NUM_MAX;

                    if(pstDp->u16StaticDPThresh == 1 )
                    {
                        pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                    }
                    pstDp->u16StaticDPThresh--;                    
                    pstDp->u8TrialCount ++;
                }
                else
                {
                    printf("trial: %x, findshed: %x\n", pstDp->u8TrialCount, pstDp->u16BadPixelsCount);
                    DpExit(IspDev, pstDp, pstRegCfg);
                    pstRegCfg->stGeRegCfg.u8GeStrength = 0; 
                    pstRegCfg->unKey.bit1GeCfg = 1;

                    pstDp->s8Finished = 1;                                        
                    hi_ext_system_bad_pixel_trigger_status_write(0x1);
                }

                hi_ext_system_bad_pixel_finish_thresh_write(pstDp->u16StaticDPThresh);
            } 
        }

    }
    else if(1 == pstDp->u8PixelDetectType)
    {
        hi_isp_green_equalize_bright_disable_write(1);
        hi_isp_green_equalize_dark_disable_write(0);
        
        if(pstDp->u8FrameCnt < 9 + WAIT_FRM_NUM_MAX)
        {
            if(pstDp->u8FrameCnt == 1 + WAIT_FRM_NUM_MAX)
            {            
                DpEnter(IspDev, pstDp, pstRegCfg);
                pstRegCfg->stGeRegCfg.u8GeStrength = 0x20;  
                pstRegCfg->unKey.bit1GeCfg = 1;             
                pstRegCfg->unKey.bit1DpCfg = 1;
                pstRegCfg->stDpRegCfg.u16DpSlope = 0x200;
                pstRegCfg->stDpRegCfg.u16DPThresh = 0x40;
            }
            pstDp->u8FrameCnt++;

            if(pstDp->u8FrameCnt == 3 + WAIT_FRM_NUM_MAX)
            {
                pstRegCfg->stDpRegCfg.u16DPThresh = pstDp->u16StaticDPThresh;
                pstRegCfg->stDpRegCfg.u16DpCountIn = 0;
                pstRegCfg->stDpRegCfg.u8ShowStaticDefectPixels = 1;
                pstRegCfg->stDpRegCfg.u8DpEnable = 1;
                pstRegCfg->stDpRegCfg.u8DpPointerReset = 1;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }    

            if(pstDp->u8FrameCnt == 4 + WAIT_FRM_NUM_MAX)
            {
                pstRegCfg->stDpRegCfg.u8DpPointerReset = 0;
                pstRegCfg->stDpRegCfg.u8DpDetectionTrigger = 1;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }

            if(pstDp->u8FrameCnt == 6 + WAIT_FRM_NUM_MAX)
            {
                pstRegCfg->stDpRegCfg.u8DpDetectionTrigger = 0;
                pstRegCfg->unKey.bit1DpCfg = 1;
            }

            if (pstDp->u8FrameCnt == 8 + WAIT_FRM_NUM_MAX)
            {
                pstDp->u16BadPixelsCount = hi_isp_defect_pixel_defect_pixel_count_read();

                pstRegCfg->stDpRegCfg.u8ShowStaticDefectPixels = 0;
                pstRegCfg->unKey.bit1DpCfg = 1;

                if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit)
                {
                    printf("BAD PIXEL CALIBRATION TIME OUT  %x\n", pstDp->u8TrialCntLimit);
                    DpExit(IspDev, pstDp, pstRegCfg);
                    pstRegCfg->stGeRegCfg.u8GeStrength = 0; 
                    pstRegCfg->unKey.bit1GeCfg = 1;

                    pstDp->s8Finished = -1;
                    hi_ext_system_bad_pixel_trigger_status_write(0x2);
                }
                else if (pstDp->u16BadPixelsCount > hi_ext_system_bad_pixel_count_max_read())
                {
                    printf("BAD_PIXEL_COUNT_UPPER_LIMIT %x, %x\n",pstDp->u16StaticDPThresh, pstDp->u16BadPixelsCount );
                    pstDp->u8FrameCnt = 2 + WAIT_FRM_NUM_MAX;
                    pstDp->u16StaticDPThresh++;
                    pstDp->u8TrialCount ++;
                }
                else if (pstDp->u16BadPixelsCount < hi_ext_system_bad_pixel_count_min_read())
                {
                    printf("BAD_PIXEL_COUNT_LOWER_LIMIT %x, %x\n", pstDp->u16StaticDPThresh, pstDp->u16BadPixelsCount);
                    pstDp->u8FrameCnt = 2 + WAIT_FRM_NUM_MAX;
                    
                    if(pstDp->u16StaticDPThresh == 1 )
                    {
                        pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                    }
                    pstDp->u16StaticDPThresh--;                    
                    pstDp->u8TrialCount ++;
                }
                else
                {
                    printf("trial: %x, findshed: %x\n", pstDp->u8TrialCount, pstDp->u16BadPixelsCount);
                    DpExit(IspDev, pstDp, pstRegCfg);
                    pstRegCfg->stGeRegCfg.u8GeStrength = 0; 
                    pstRegCfg->unKey.bit1GeCfg = 1;
                    
                    pstDp->s8Finished = 1;
                    hi_ext_system_bad_pixel_trigger_status_write(0x1);
                }
                
                hi_ext_system_bad_pixel_finish_thresh_write(pstDp->u16StaticDPThresh);
            }
        }
    }
    else
    {
        printf("invalid static defect pixel detect type!\n");
    }

    if (0 != pstDp->s8Finished)
    {
        hi_ext_system_calibrate_bad_pixels_write(0);
        hi_isp_green_equalize_bright_disable_write(0);
        hi_isp_green_equalize_dark_disable_write(0);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DpCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    return HI_SUCCESS;
}

HI_S32 ISP_DpExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDp(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DP;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DpInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DpRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DpCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DpExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

