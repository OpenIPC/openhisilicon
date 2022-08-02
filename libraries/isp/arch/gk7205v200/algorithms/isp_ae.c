/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define WEIGHT_TABLE_WIDTH  17
#define WEIGHT_TABLE_HEIGHT 15

static GK_U32 AePirisLinToFNO(GK_U32 u32Value)
{
    GK_U32 i = 0;
    GK_U32 u32Tmp = u32Value;

    if (u32Value <= 1) {
        i = 0;
    } else {
        while (u32Tmp > 1) {
            u32Tmp /= 2;
            i++;
        }
    }

    return i;
}

static GK_S32 AeGetDCFInfo(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspUpdateInfo = &pstIspCtx->stDcfUpdateInfo;

    if (pstIspUpdateInfo == GK_NULL) {
        return GK_FAILURE;
    }

    pstIspUpdateInfo->u32ExposureBiasValue = pstAeResult->stUpdateInfo.u32ExposureBiasValue;
    pstIspUpdateInfo->u32ISOSpeedRatings   = pstAeResult->stUpdateInfo.u32ISOSpeedRatings;
    pstIspUpdateInfo->u8ExposureProgram    = pstAeResult->stUpdateInfo.u8ExposureProgram;
    pstIspUpdateInfo->u8ExposureMode       = pstAeResult->stUpdateInfo.u8ExposureMode;
    pstIspUpdateInfo->u32ExposureTime      = pstAeResult->stUpdateInfo.u32ExposureTime;
    pstIspUpdateInfo->u32MaxApertureValue  = pstAeResult->stUpdateInfo.u32MaxApertureValue;
    pstIspUpdateInfo->u32FNumber           = pstAeResult->stUpdateInfo.u32FNumber;

    return GK_SUCCESS;
}

static GK_S32 AeGetFrameInfo(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_FRAME_INFO_S *pstIspFrame = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspFrame = &pstIspCtx->stFrameInfo;

    if (pstIspFrame == GK_NULL) {
        return GK_FAILURE;
    }

    pstIspFrame->u32ExposureTime = pstAeResult->u32IntTime[0] >> 4;
    pstIspFrame->u32ISO = pstAeResult->u32Iso;
    pstIspFrame->u32IspDgain = pstAeResult->u32IspDgain << 2;
    pstIspFrame->u32Again = pstAeResult->u32Again;
    pstIspFrame->u32Dgain = pstAeResult->u32Dgain;

    if (IS_2to1_WDR_MODE(pstIspCtx->u8SnsWDRMode)) {
        pstIspFrame->au32Ratio[0] = ((GK_U64)(pstAeResult->u32IntTime[1]) * (pstAeResult->u32Iso << 2) + ((pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF) >> 4 ) / 2) / DIV_0_TO_1((pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF) >> 4);
        pstIspFrame->au32Ratio[1] = 64;
        pstIspFrame->au32Ratio[2] = 64;
    } else {
        pstIspFrame->au32Ratio[0] = 64;
        pstIspFrame->au32Ratio[1] = 64;
        pstIspFrame->au32Ratio[2] = 64;
    }

    pstIspFrame->u32FNumber = AePirisLinToFNO(pstAeResult->u32PirisGain);
    pstIspFrame->u32HmaxTimes = pstAeResult->u32HmaxTimes;
    pstIspFrame->u32Vmax      = pstAeResult->u32Vmax;

    return GK_SUCCESS;
}


static GK_VOID AeRegsRangeCheck(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    GK_U32 i, j;

    pstAeResult->stStatAttr.u8AEBESel       = MIN2(pstAeResult->stStatAttr.u8AEBESel, 2);
    pstAeResult->stStatAttr.u8FourPlaneMode = MIN2(pstAeResult->stStatAttr.u8FourPlaneMode, 1);
    if (pstAeResult->stStatAttr.u8FourPlaneMode) {
        pstAeResult->stStatAttr.u8HistSkipX = MIN2(pstAeResult->stStatAttr.u8HistSkipX, 6);
    } else {
        pstAeResult->stStatAttr.u8HistSkipX = MIN2(pstAeResult->stStatAttr.u8HistSkipX, 6);
        pstAeResult->stStatAttr.u8HistSkipX = MAX2(pstAeResult->stStatAttr.u8HistSkipX, 1);
    }
    pstAeResult->stStatAttr.u8HistSkipY   = MIN2(pstAeResult->stStatAttr.u8HistSkipY, 6);
    pstAeResult->stStatAttr.u8HistOffsetX = MIN2(pstAeResult->stStatAttr.u8HistOffsetX, 1);
    pstAeResult->stStatAttr.u8HistOffsetY = MIN2(pstAeResult->stStatAttr.u8HistOffsetY, 1);

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j] = MIN2(pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j], 15);
        }
    }

    pstAeResult->stStatAttr.u8HistMode    = MIN2(pstAeResult->stStatAttr.u8HistMode, 1);
    pstAeResult->stStatAttr.u8AverMode    = MIN2(pstAeResult->stStatAttr.u8AverMode, 1);
    pstAeResult->stStatAttr.u8MaxGainMode = MIN2(pstAeResult->stStatAttr.u8MaxGainMode, 1);
}

static GK_VOID AeResRegsDefault(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8  i;
    GK_U8  u8BlockNum = 0;
    GK_U16 u16Overlap;

    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_RECT_S stBlockRect;
    ISP_MG_STATIC_CFG_S    *pstMgStaticRegCfg  = GK_NULL;
    ISP_AE_STATIC_CFG_S    *pstAeStaticRegCfg  = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;
    u16Overlap = pstIspCtx->stBlockAttr.u32OverLap;

    /* AE BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AE&MG&DG BLC Configs */
        pstAeStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stAeRegCfg.stStaticRegCfg;
        pstMgStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stMgRegCfg.stStaticRegCfg;

        /* AE&MG&DG Size Configs */
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->stBlockAttr, i);

        pstAeStaticRegCfg->u16BECropPosY      = 0;
        pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.u32Height;
        pstMgStaticRegCfg->u16CropPosY        = 0;
        pstMgStaticRegCfg->u16CropOutHeight   = stBlockRect.u32Height;


        /* AE&MG Overlap Configs */
        if (i == 0) {
            if (u8BlockNum > 1) {
                pstAeStaticRegCfg->u16BECropPosX      = 0;
                pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width - u16Overlap;

                pstMgStaticRegCfg->u16CropPosX        = 0;
                pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.u32Width - u16Overlap;
            } else {
                pstAeStaticRegCfg->u16BECropPosX      = 0;
                pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width;

                pstMgStaticRegCfg->u16CropPosX        = 0;
                pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.u32Width;
            }
        } else if (i == (u8BlockNum - 1)) {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width - u16Overlap;

            pstMgStaticRegCfg->u16CropPosX        = u16Overlap;
            pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.u32Width - u16Overlap;
        } else {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width - (u16Overlap << 1);

            pstMgStaticRegCfg->u16CropPosX        = u16Overlap;
            pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.u32Width - (u16Overlap << 1);
        }
    }

    /* AE FE Configs */
    pstAeStaticRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stStaticRegCfg;

    /* Crop Configs */
    pstAeStaticRegCfg->u16FECropPosX = 0;
    pstAeStaticRegCfg->u16FECropPosY = 0;
    pstAeStaticRegCfg->u16FECropOutHeight = pstIspCtx->stSysRect.u32Height;
    pstAeStaticRegCfg->u16FECropOutWidth  = pstIspCtx->stSysRect.u32Width;

    ext_system_ae_crop_en_write(ViPipe, ISP_EXT_SYSTEM_CROP_EN_DEFAULT);
    ext_system_ae_crop_x_write(ViPipe, 0);
    ext_system_ae_crop_y_write(ViPipe, 0);
    ext_system_ae_crop_height_write(ViPipe, pstIspCtx->stBlockAttr.stFrameRect.u32Height);
    ext_system_ae_crop_width_write(ViPipe, pstIspCtx->stBlockAttr.stFrameRect.u32Width);

    pstRegCfg->unKey.bit1AeCfg1 = GK_TRUE;
    pstRegCfg->unKey.bit1AeCfg2 = GK_TRUE;
}

static GK_VOID AeResReadExtregs(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8  i;
    GK_U8  u8BlockNum = 0;
    GK_U8  u8CropEn;
    GK_U16 u16Overlap;
    GK_U32 u16CropX, u16CropY, u16CropHeight, u16CropWidth;

    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_RECT_S stBlockRect;
    ISP_MG_STATIC_CFG_S    *pstMgStaticRegCfg = GK_NULL;
    ISP_AE_STATIC_CFG_S    *pstAeStaticRegCfg  = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;
    u16Overlap = pstIspCtx->stBlockAttr.u32OverLap;

    u8CropEn = ext_system_ae_crop_en_read(ViPipe);
    u16CropX = (ext_system_ae_crop_x_read(ViPipe) >> 2) << 2;
    u16CropY = (ext_system_ae_crop_y_read(ViPipe) >> 2) << 2;
    u16CropHeight = (ext_system_ae_crop_height_read(ViPipe) >> 2) << 2;
    u16CropWidth = (ext_system_ae_crop_width_read(ViPipe) >> 2) << 2;

    u16CropWidth  = MAX2(u16CropWidth,  AE_MIN_WIDTH);
    u16CropHeight = MAX2(u16CropHeight, AE_MIN_HEIGHT);
    u16CropWidth  = MIN2(u16CropWidth, pstIspCtx->stBlockAttr.stFrameRect.u32Width);
    u16CropHeight = MIN2(u16CropHeight, pstIspCtx->stBlockAttr.stFrameRect.u32Height);
    u16CropX      = MIN2(u16CropX, (pstIspCtx->stBlockAttr.stFrameRect.u32Width - u16CropWidth));
    u16CropY      = MIN2(u16CropY, (pstIspCtx->stBlockAttr.stFrameRect.u32Height - u16CropHeight));

    /* AE BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AE&MG&DG BLC Configs */
        pstAeStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stAeRegCfg.stStaticRegCfg;
        pstMgStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stMgRegCfg.stStaticRegCfg;

        /* AE&MG&DG Size Configs */
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->stBlockAttr, i);

        /* AE&MG Overlap Configs */
        if (i == 0) {
            if (u8BlockNum > 1) {
                pstAeStaticRegCfg->u16BECropPosX      = 0;
                pstAeStaticRegCfg->u16BECropPosY      = 0;
                pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.u32Height;
                pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width - u16Overlap;

                pstMgStaticRegCfg->u16CropPosX      = 0;
                pstMgStaticRegCfg->u16CropPosY      = 0;
                pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.u32Height;
                pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.u32Width - u16Overlap;
            } else {
                if (u8CropEn) {
                    pstAeStaticRegCfg->u16BECropPosX      = u16CropX;
                    pstAeStaticRegCfg->u16BECropPosY      = u16CropY;
                    pstAeStaticRegCfg->u16BECropOutHeight = u16CropHeight;
                    pstAeStaticRegCfg->u16BECropOutWidth  = u16CropWidth;

                    pstMgStaticRegCfg->u16CropPosX      = u16CropX;
                    pstMgStaticRegCfg->u16CropPosY      = u16CropY;
                    pstMgStaticRegCfg->u16CropOutHeight = u16CropHeight;
                    pstMgStaticRegCfg->u16CropOutWidth  = u16CropWidth;
                } else {
                    pstAeStaticRegCfg->u16BECropPosX      = 0;
                    pstAeStaticRegCfg->u16BECropPosY      = 0;
                    pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.u32Height;
                    pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width;

                    pstMgStaticRegCfg->u16CropPosX      = 0;
                    pstMgStaticRegCfg->u16CropPosY      = 0;
                    pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.u32Height;
                    pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.u32Width;
                }
            }
        } else if (i == (u8BlockNum - 1)) {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropPosY      = 0;
            pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.u32Height;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width - u16Overlap;

            pstMgStaticRegCfg->u16CropPosX      = u16Overlap;
            pstMgStaticRegCfg->u16CropPosY      = 0;
            pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.u32Height;
            pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.u32Width - u16Overlap;
        } else {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropPosY      = 0;
            pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.u32Height;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.u32Width - (u16Overlap << 1);

            pstMgStaticRegCfg->u16CropPosX      = u16Overlap;
            pstMgStaticRegCfg->u16CropPosY      = 0;
            pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.u32Height;
            pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.u32Width - (u16Overlap << 1);
        }
    }

    /* AE FE Configs */
    pstAeStaticRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stStaticRegCfg;

    /* Crop Configs */
    if (u8CropEn) {
        pstAeStaticRegCfg->u16FECropPosX = u16CropX;
        pstAeStaticRegCfg->u16FECropPosY = u16CropY;
        pstAeStaticRegCfg->u16FECropOutHeight = u16CropHeight;
        pstAeStaticRegCfg->u16FECropOutWidth  = u16CropWidth;
    } else {
        pstAeStaticRegCfg->u16FECropPosX = 0;
        pstAeStaticRegCfg->u16FECropPosY = 0;
        pstAeStaticRegCfg->u16FECropOutHeight = pstIspCtx->stSysRect.u32Height;
        pstAeStaticRegCfg->u16FECropOutWidth  = pstIspCtx->stSysRect.u32Width;
    }

    pstRegCfg->unKey.bit1AeCfg1 = GK_TRUE;
    pstRegCfg->unKey.bit1AeCfg2 = GK_TRUE;

}


static GK_VOID AeRegsDefault(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8 i, j, k;
    GK_U8 u8WDRMode = 0;
    GK_U8 u8BlockNum = 0;
    GK_U8 u8HistSkipX = 0;
    GK_U8 u8HistSkipY = 0;
    GK_U8 u8HistOffsetX = 0;
    GK_U8 u8HistOffsetY = 0;
    GK_U8 u8BlockOffsetX = 0;
    GK_U8 u8SensorPatternType = 0;

    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel   = GK_NULL;
    ISP_MG_DYNA_CFG_S      *pstMgDynaRegCfg    = GK_NULL;
    ISP_MG_STATIC_CFG_S    *pstMgStaticRegCfg  = GK_NULL;
    ISP_AE_STATIC_CFG_S    *pstAeStaticRegCfg  = GK_NULL;
    ISP_AE_DYNA_CFG_S      *pstAeDynaRegCfg    = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;
    u8WDRMode  = pstIspCtx->u8SnsWDRMode;
    u8SensorPatternType = ext_system_rggb_cfg_read(ViPipe);

    GK_U8 au8WeightTable[WEIGHT_TABLE_HEIGHT][WEIGHT_TABLE_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    if ((u8SensorPatternType == ISP_TOP_RGGB_START_R_GR_GB_B) || (u8SensorPatternType == ISP_TOP_RGGB_START_B_GB_GR_R)) {
        u8HistSkipX = 1;
        u8HistSkipY = 1;
        u8HistOffsetX = 1;
        u8HistOffsetY = 0;
    } else {
        u8HistSkipX = 1;
        u8HistSkipY = 1;
        u8HistOffsetX = 0;
        u8HistOffsetY = 0;
    }

    /* AE BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AE&MG&DG BLC Configs */
        pstAeDynaRegCfg   = &pstRegCfg->stAlgRegCfg[i].stAeRegCfg.stDynaRegCfg;
        pstAeStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stAeRegCfg.stStaticRegCfg;

        pstMgDynaRegCfg   = &pstRegCfg->stAlgRegCfg[i].stMgRegCfg.stDynaRegCfg;
        pstMgStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stMgRegCfg.stStaticRegCfg;

        pstAeStaticRegCfg->u8BEEnable  = GK_TRUE;
        pstMgStaticRegCfg->u8Enable    = GK_TRUE;

        /* AE&MG WDR Configs */
        pstAeDynaRegCfg->u8BEBitMove    = 0;
        pstAeDynaRegCfg->u8BEGammaLimit = 6;
        pstMgDynaRegCfg->u8BitMove      = 0;
        pstMgDynaRegCfg->u8GammaLimit   = 3;
        if (IS_LINEAR_MODE(u8WDRMode)) {
            pstAeDynaRegCfg->u8BEHistGammaMode = 0;
            pstAeDynaRegCfg->u8BEAverGammaMode = 0;
            pstMgDynaRegCfg->u8GammaMode       = 0;
            pstMgStaticRegCfg->u8Enable        = GK_FALSE;
        } else {
            pstAeDynaRegCfg->u8BEHistGammaMode = 1;
            pstAeDynaRegCfg->u8BEAverGammaMode = 1;
            pstMgDynaRegCfg->u8GammaMode       = 1;
        }

        /* MPI Configs */
        pstAeDynaRegCfg->u8BEAESel         = 1;
        pstAeDynaRegCfg->u8BEFourPlaneMode = 0;
        pstAeDynaRegCfg->u8BEHistSkipX     = u8HistSkipX;
        pstAeDynaRegCfg->u8BEHistSkipY     = u8HistSkipY;
        pstAeDynaRegCfg->u8BEHistOffsetX   = u8HistOffsetX;
        pstAeDynaRegCfg->u8BEHistOffsetY   = u8HistOffsetY;

        /* Weight Table Configs */
        pstAeDynaRegCfg->u8BEWightTableUpdate = GK_TRUE;
        if (i < WEIGHT_TABLE_WIDTH % DIV_0_TO_1(u8BlockNum)) {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum) + 1;
            pstMgDynaRegCfg->u8ZoneWidth          = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum) + 1;
        } else {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum);
            pstMgDynaRegCfg->u8ZoneWidth          = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum);
        }
        pstAeDynaRegCfg->u8BEWeightTableHeight = WEIGHT_TABLE_HEIGHT;
        pstMgDynaRegCfg->u8ZoneHeight          = WEIGHT_TABLE_HEIGHT;
        for (j = 0; j < pstAeDynaRegCfg->u8BEWeightTableHeight; j++) {
            for (k = 0; k < pstAeDynaRegCfg->u8BEWeightTableWidth; k++) {
                pstAeDynaRegCfg->au8BEWeightTable[j][k] = au8WeightTable[j][k + u8BlockOffsetX];
            }
        }
        u8BlockOffsetX += pstAeDynaRegCfg->u8BEWeightTableWidth;
    }

    /* AE FE Configs */
    pstAeDynaRegCfg   = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stDynaRegCfg;
    pstAeStaticRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stStaticRegCfg;
    pstMgStaticRegCfg = &pstRegCfg->stAlgRegCfg[0].stMgRegCfg.stStaticRegCfg;

    /* BLC Configs */
    pstAeStaticRegCfg->u8FEEnable = GK_TRUE;

    /* WDR Configs */
    pstAeDynaRegCfg->u8FEBitMove       = 0;
    pstAeDynaRegCfg->u8FEGammaLimit    = 6;
    pstAeDynaRegCfg->u8FEHistGammaMode = 0;
    pstAeDynaRegCfg->u8FEAverGammaMode = 0;

    /* MPI Configs */
    pstAeDynaRegCfg->u8FEFourPlaneMode = 0;
    pstAeDynaRegCfg->u8FEHistSkipX     = u8HistSkipX;
    pstAeDynaRegCfg->u8FEHistSkipY     = u8HistSkipY;
    pstAeDynaRegCfg->u8FEHistOffsetX   = u8HistOffsetX;
    pstAeDynaRegCfg->u8FEHistOffsetY   = u8HistOffsetY;

    /* Weight Tbale Configs */
    pstAeDynaRegCfg->u8FEWightTableUpdate  = GK_TRUE;
    pstAeDynaRegCfg->u8FEWeightTableWidth  = WEIGHT_TABLE_WIDTH;
    pstAeDynaRegCfg->u8FEWeightTableHeight = WEIGHT_TABLE_HEIGHT;

    memcpy(pstAeDynaRegCfg->au8FEWeightTable, au8WeightTable, WEIGHT_TABLE_HEIGHT * WEIGHT_TABLE_WIDTH * sizeof(GK_U8));

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            ext_system_ae_weight_table_write(ViPipe, (i * AE_ZONE_COLUMN + j), au8WeightTable[i][j]);
        }
    }

    ext_system_ae_be_sel_write(ViPipe, pstAeDynaRegCfg->u8BEAESel);
    ext_system_ae_fourplanemode_write(ViPipe, pstAeDynaRegCfg->u8BEFourPlaneMode);

    ext_system_aest_skip_x_write(ViPipe, u8HistSkipX);
    ext_system_aest_skip_y_write(ViPipe, u8HistSkipY);
    ext_system_aest_offset_x_write(ViPipe, u8HistOffsetX);
    ext_system_aest_offset_y_write(ViPipe, u8HistOffsetY);

    ext_system_ae_fe_en_write(ViPipe, pstAeStaticRegCfg->u8FEEnable);
    ext_system_ae_be_en_write(ViPipe, pstAeStaticRegCfg->u8BEEnable);
    ext_system_mg_en_write(ViPipe, pstMgStaticRegCfg->u8Enable);

    if (IS_LINEAR_MODE(u8WDRMode)) {
        ext_system_aestmode_write(ViPipe, 0);
        ext_system_ae_avermode_write(ViPipe, 0);
        ext_system_ae_maxgainmode_write(ViPipe, 0);
    } else {
        ext_system_aestmode_write(ViPipe, 1);
        ext_system_ae_avermode_write(ViPipe, 1);
        ext_system_ae_maxgainmode_write(ViPipe, 1);
    }

    AeResRegsDefault(ViPipe, pstRegCfg);

    ext_system_smart_update_write(ViPipe, ISP_EXT_SYSTEM_SMART_UPDATE_DEFAULT);
    for (i = 0; i < SMART_CLASS_MAX; i++) {
        ext_system_smart_enable_write(ViPipe, i, ISP_EXT_SYSTEM_SMART_ENABLE_DEFAULT);
        ext_system_smart_available_write(ViPipe, i, ISP_EXT_SYSTEM_SMART_AVAILABLE_DEFAULT);
        ext_system_smart_luma_write(ViPipe, i, ISP_EXT_SYSTEM_SMART_LUMA_DEFAULT);
    }

}

static GK_VOID AeReadExtregs(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    GK_U32 i, j;

    if (!pstAeResult->stStatAttr.bHistAdjust) {
        if (pstAeResult->stStatAttr.u8AEBESel != ext_system_ae_be_sel_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8AEBESel = ext_system_ae_be_sel_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8FourPlaneMode != ext_system_ae_fourplanemode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8FourPlaneMode = ext_system_ae_fourplanemode_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistOffsetX != ext_system_aest_offset_x_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8HistOffsetX = ext_system_aest_offset_x_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistOffsetY != ext_system_aest_offset_y_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8HistOffsetY = ext_system_aest_offset_y_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistSkipX != ext_system_aest_skip_x_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8HistSkipX = ext_system_aest_skip_x_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistSkipY != ext_system_aest_skip_y_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8HistSkipY = ext_system_aest_skip_y_read(ViPipe);
        }
    } else {
        ext_system_ae_be_sel_write(ViPipe, pstAeResult->stStatAttr.u8AEBESel);
        ext_system_ae_fourplanemode_write(ViPipe, pstAeResult->stStatAttr.u8FourPlaneMode);
        ext_system_aest_skip_x_write(ViPipe, pstAeResult->stStatAttr.u8HistSkipX);
        ext_system_aest_skip_y_write(ViPipe, pstAeResult->stStatAttr.u8HistSkipY);
        ext_system_aest_offset_x_write(ViPipe, pstAeResult->stStatAttr.u8HistOffsetX);
        ext_system_aest_offset_y_write(ViPipe, pstAeResult->stStatAttr.u8HistOffsetY);
    }

    if (!pstAeResult->stStatAttr.bWightTableUpdate) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                if (pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j] != ext_system_ae_weight_table_read(ViPipe, (i * AE_ZONE_COLUMN + j))) {
                    pstAeResult->stStatAttr.bChange = GK_TRUE;
                    pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j] = ext_system_ae_weight_table_read(ViPipe, (i * AE_ZONE_COLUMN + j));
                }
            }
        }
    } else {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                ext_system_ae_weight_table_write(ViPipe, (i * AE_ZONE_COLUMN + j), pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j]);
            }
        }
    }

    if (!pstAeResult->stStatAttr.bModeUpdate) {
        if (pstAeResult->stStatAttr.u8HistMode != ext_system_aestmode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8HistMode = ext_system_aestmode_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8AverMode != ext_system_ae_avermode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8AverMode = ext_system_ae_avermode_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8MaxGainMode != ext_system_ae_maxgainmode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = GK_TRUE;
            pstAeResult->stStatAttr.u8MaxGainMode = ext_system_ae_maxgainmode_read(ViPipe);
        }
    } else {
        ext_system_aestmode_write(ViPipe, pstAeResult->stStatAttr.u8HistMode);
        ext_system_ae_avermode_write(ViPipe, pstAeResult->stStatAttr.u8AverMode);
        ext_system_ae_maxgainmode_write(ViPipe, pstAeResult->stStatAttr.u8MaxGainMode);
    }
}

static GK_VOID AeUpdateConfig(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8  u8BlockNum = 0;
    GK_U8  u8BlockOffsetX = 0;
    GK_S32 i, j, k = 0;

    ISP_AE_STAT_ATTR_S *pstStatAttr      = GK_NULL;
    ISP_CTX_S          *pstIspCtx        = GK_NULL;
    ISP_AE_DYNA_CFG_S  *pstAeDynaRegCfg  = GK_NULL;
    ISP_AE_STATIC_CFG_S *pstAeStatRegCfg = GK_NULL;
    ISP_AE_REG_CFG_2_S *pstAeRegCfg2     = GK_NULL;
    ISP_MG_DYNA_CFG_S  *pstMgDynaRegCfg  = GK_NULL;
    ISP_MG_STATIC_CFG_S *pstMgStatRegCfg = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum   = pstIspCtx->stBlockAttr.u8BlockNum;
    pstStatAttr  = &pstAeResult->stStatAttr;
    pstAeRegCfg2 = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg2;

    for (i = 0; i < u8BlockNum; i++) {
        pstAeStatRegCfg = &pstRegCfg->stAlgRegCfg[i].stAeRegCfg.stStaticRegCfg;
        pstMgStatRegCfg = &pstRegCfg->stAlgRegCfg[i].stMgRegCfg.stStaticRegCfg;

        pstAeStatRegCfg->u8FEEnable = ext_system_ae_fe_en_read(ViPipe);
        pstAeStatRegCfg->u8BEEnable = ext_system_ae_be_en_read(ViPipe);
        pstMgStatRegCfg->u8Enable   = ext_system_mg_en_read(ViPipe);
    }

    /* BE Configs Update */
    if (pstStatAttr->bHistAdjust) {
        pstAeDynaRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stDynaRegCfg;

        if ((pstAeDynaRegCfg->u8BEAESel != pstStatAttr->u8AEBESel)
            || (pstAeDynaRegCfg->u8BEFourPlaneMode != pstStatAttr->u8FourPlaneMode)
            || (pstAeDynaRegCfg->u8BEHistSkipX != pstStatAttr->u8HistSkipX)
            || (pstAeDynaRegCfg->u8BEHistSkipY != pstStatAttr->u8HistSkipY)
            || (pstAeDynaRegCfg->u8BEHistOffsetX != pstStatAttr->u8HistOffsetX)
            || (pstAeDynaRegCfg->u8BEHistOffsetY != pstStatAttr->u8HistOffsetY)) {
            pstStatAttr->bChange = GK_TRUE;
        }
    }

    if (pstStatAttr->bModeUpdate) {
        pstAeDynaRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stDynaRegCfg;
        pstMgDynaRegCfg = &pstRegCfg->stAlgRegCfg[0].stMgRegCfg.stDynaRegCfg;

        if ((pstAeDynaRegCfg->u8BEHistGammaMode != pstStatAttr->u8HistMode)
            || (pstAeDynaRegCfg->u8BEAverGammaMode != pstStatAttr->u8AverMode)
            || (pstMgDynaRegCfg->u8GammaMode != pstStatAttr->u8MaxGainMode)) {
            pstStatAttr->bChange = GK_TRUE;
        }
    }

    if (pstStatAttr->bWightTableUpdate) {
        pstAeDynaRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stDynaRegCfg;

        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                if (pstAeDynaRegCfg->au8BEWeightTable[i][j] != pstStatAttr->au8WeightTable[ViPipe][i][j]) {
                    pstStatAttr->bChange = GK_TRUE;
                }
            }
        }
    }

    for (i = 0; i < u8BlockNum; i++) {
        pstAeDynaRegCfg = &pstRegCfg->stAlgRegCfg[i].stAeRegCfg.stDynaRegCfg;
        pstMgDynaRegCfg = &pstRegCfg->stAlgRegCfg[i].stMgRegCfg.stDynaRegCfg;

        pstAeDynaRegCfg->u8BEAESel         = pstStatAttr->u8AEBESel;
        pstAeDynaRegCfg->u8BEFourPlaneMode = pstStatAttr->u8FourPlaneMode;
        pstAeDynaRegCfg->u8BEHistSkipX     = pstStatAttr->u8HistSkipX;
        pstAeDynaRegCfg->u8BEHistSkipY     = pstStatAttr->u8HistSkipY;
        pstAeDynaRegCfg->u8BEHistOffsetX   = pstStatAttr->u8HistOffsetX;
        pstAeDynaRegCfg->u8BEHistOffsetY   = pstStatAttr->u8HistOffsetY;

        pstAeDynaRegCfg->u8BEHistGammaMode = pstStatAttr->u8HistMode;
        pstAeDynaRegCfg->u8BEAverGammaMode = pstStatAttr->u8AverMode;
        pstMgDynaRegCfg->u8GammaMode       = pstStatAttr->u8MaxGainMode;

        pstAeDynaRegCfg->u8BEWightTableUpdate = GK_TRUE;

        if (i < WEIGHT_TABLE_WIDTH % DIV_0_TO_1(u8BlockNum)) {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum) + 1;
        } else {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum);
        }

        pstAeDynaRegCfg->u8BEWeightTableHeight = WEIGHT_TABLE_HEIGHT;
        for (j = 0; j < pstAeDynaRegCfg->u8BEWeightTableHeight; j++) {
            for (k = 0; k < pstAeDynaRegCfg->u8BEWeightTableWidth; k++) {
                pstAeDynaRegCfg->au8BEWeightTable[j][k] = pstStatAttr->au8WeightTable[ViPipe][j][k + u8BlockOffsetX];
            }
        }

        u8BlockOffsetX += pstAeDynaRegCfg->u8BEWeightTableWidth;

        pstRegCfg->unKey.bit1AeCfg1 = GK_TRUE;
        pstRegCfg->unKey.bit1AeCfg2 = GK_TRUE;
    }

    /* FE Configs Update */
    pstAeDynaRegCfg = &pstRegCfg->stAlgRegCfg[0].stAeRegCfg.stDynaRegCfg;

    pstAeDynaRegCfg->u8FEFourPlaneMode = pstStatAttr->u8FourPlaneMode;
    pstAeDynaRegCfg->u8FEHistSkipX     = pstStatAttr->u8HistSkipX;
    pstAeDynaRegCfg->u8FEHistSkipY     = pstStatAttr->u8HistSkipY;
    pstAeDynaRegCfg->u8FEHistOffsetX   = pstStatAttr->u8HistOffsetX;
    pstAeDynaRegCfg->u8FEHistOffsetY   = pstStatAttr->u8HistOffsetY;

    pstAeDynaRegCfg->u8FEWightTableUpdate  = GK_TRUE;
    pstAeDynaRegCfg->u8FEWeightTableWidth  = WEIGHT_TABLE_WIDTH;
    pstAeDynaRegCfg->u8FEWeightTableHeight = WEIGHT_TABLE_HEIGHT;

    for (j = 0; j < WEIGHT_TABLE_HEIGHT; j++) {
        for (k = 0; k < WEIGHT_TABLE_WIDTH; k++) {
            pstAeDynaRegCfg->au8FEWeightTable[j][k] = pstStatAttr->au8WeightTable[ViPipe][j][k];
        }
    }

    pstStatAttr->bChange = GK_FALSE;

    pstAeRegCfg2->u32IntTime[0] = pstAeResult->u32IntTime[0];
    pstAeRegCfg2->u32IntTime[1] = pstAeResult->u32IntTime[1];
    pstAeRegCfg2->u32IntTime[2] = pstAeResult->u32IntTime[2];
    pstAeRegCfg2->u32IntTime[3] = pstAeResult->u32IntTime[3];
    pstAeRegCfg2->u32IspDgain   = pstAeResult->u32IspDgain;
    pstAeRegCfg2->bPirisValid   = pstAeResult->bPirisValid;
    pstAeRegCfg2->s32PirisPos   = pstAeResult->s32PirisPos;
    pstAeRegCfg2->enFSWDRMode   = pstAeResult->enFSWDRMode;
    for (i = 0; i < 4; i++) {
        pstAeResult->au32WDRGain[i] = MAX2(pstAeResult->au32WDRGain[i], 0x100);
        pstAeRegCfg2->au32WDRGain[i] = pstAeResult->au32WDRGain[i];
    }
    /* be careful avoid overflow */
    if (IS_2to1_WDR_MODE(pstIspCtx->u8SnsWDRMode)) {
        if (pstAeResult->bPirisValid == GK_TRUE) {
            pstAeRegCfg2->u64Exposure = (GK_U64)pstAeResult->u32IntTime[1] * pstAeResult->u32Iso * pstAeResult->u32PirisGain;
            pstAeRegCfg2->u64ExposureSF = (GK_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF * pstAeResult->u32PirisGain;
        } else {
            pstAeRegCfg2->u64Exposure = (GK_U64)pstAeResult->u32IntTime[1] * pstAeResult->u32Iso;
            pstAeRegCfg2->u64ExposureSF = (GK_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF;
        }
    } else {
        if (pstAeResult->bPirisValid == GK_TRUE) {
            pstAeRegCfg2->u64Exposure = (GK_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32Iso * pstAeResult->u32PirisGain;
        } else {
            pstAeRegCfg2->u64Exposure = (GK_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32Iso;
        }
    }
}

static GK_VOID AeUpdateLinkage(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult, ISP_LINKAGE_S *pstLinkage)
{
    GK_S32 i = 0;
    GK_U8 u8DelayMax;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_U32 u32ExpRatioTmp;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstLinkage->u32IspDgain      = pstAeResult->u32IspDgain;
    pstLinkage->u32Dgain         = pstAeResult->u32Dgain;
    pstLinkage->u32Again         = pstAeResult->u32Again;
    pstLinkage->u32IspDgainShift = 8;

    for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
        pstLinkage->au32SyncIsoBuf[i] = pstLinkage->au32SyncIsoBuf[i - 1];
    }

    pstLinkage->au32SyncIsoBuf[0] = pstAeResult->u32Iso;

    u8DelayMax = pstIspCtx->stLinkage.u8Cfg2ValidDelayMax;
    u8DelayMax = CLIP3(u8DelayMax, 1, ISP_SYNC_ISO_BUF_MAX - 1);

    if (pstLinkage->u32UpdatePos == 0) {
        pstLinkage->u32Iso = MAX2(pstLinkage->au32SyncIsoBuf[u8DelayMax], 100);
    } else {
        pstLinkage->u32Iso = MAX2(pstLinkage->au32SyncIsoBuf[u8DelayMax - 1], 100);
    }

    if (pstIspCtx->stLinkage.bRunOnce == GK_TRUE) {
        pstLinkage->u32Iso = MAX2(pstLinkage->au32SyncIsoBuf[0], 100);
    }

    pstIspCtx->stLinkage.u32IsoDoneFrmcnt = ISP_FrameCntGet(ViPipe);

    pstLinkage->u32SensorIso = ((GK_U64)pstAeResult->u32Iso << 8) / DIV_0_TO_1(pstAeResult->u32IspDgain);
    pstLinkage->u32SensorIso = (pstLinkage->u32SensorIso < 100) ? 100 : pstLinkage->u32SensorIso;
    pstLinkage->u32Inttime = pstAeResult->u32IntTime[0] >> 4;
    pstLinkage->u8AERunInterval = pstAeResult->u8AERunInterval;

    for (i = 0; i < 4; i++) {
        pstLinkage->au32WDRGain[i] = 256;
    }

    if (IS_2to1_WDR_MODE(pstIspCtx->u8SnsWDRMode)) {
        /* WDR exposure ratio is 6bit precision */
        pstLinkage->u32Inttime = pstAeResult->u32IntTime[1] >> 4;
        u32ExpRatioTmp = (((GK_U64)pstAeResult->u32IntTime[1] * pstAeResult->u32Iso << 6) + (((GK_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF) / 2)) / DIV_0_TO_1((GK_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF);

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            pstLinkage->au32SyncAllExpRatioBuf[i] = pstLinkage->au32SyncAllExpRatioBuf[i - 1];
            pstLinkage->au32SyncExpRatioBuf[0][i] = pstLinkage->au32SyncExpRatioBuf[0][i - 1];
        }

        pstLinkage->au32SyncAllExpRatioBuf[0] = u32ExpRatioTmp;
        pstLinkage->au32SyncExpRatioBuf[0][0] = u32ExpRatioTmp;

        if (pstLinkage->u32UpdatePos == 0) {
            pstLinkage->u32ExpRatio = pstLinkage->au32SyncAllExpRatioBuf[u8DelayMax];
            pstLinkage->au32ExpRatio[0] = pstLinkage->au32SyncExpRatioBuf[0][u8DelayMax];
        } else {
            pstLinkage->u32ExpRatio = pstLinkage->au32SyncAllExpRatioBuf[u8DelayMax - 1];
            pstLinkage->au32ExpRatio[0] = pstLinkage->au32SyncExpRatioBuf[0][u8DelayMax - 1];
        }

        if (pstIspCtx->stLinkage.bRunOnce == GK_TRUE) {
            pstLinkage->u32ExpRatio = pstLinkage->au32SyncAllExpRatioBuf[0];
            pstLinkage->au32ExpRatio[0] = pstLinkage->au32SyncExpRatioBuf[0][0];
        }

        ext_system_actual_wdr_exposure_ratio_write(ViPipe, 0, pstLinkage->au32ExpRatio[0]);
        ext_system_actual_wdr_exposure_ratio_write(ViPipe, 1, 64);
        ext_system_actual_wdr_exposure_ratio_write(ViPipe, 2, 64);

        pstLinkage->au32WDRGain[0] = pstAeResult->au32WDRGain[0];
        pstLinkage->au32WDRGain[1] = pstAeResult->au32WDRGain[1];
    } else {

    }

    if (pstAeResult->bPirisValid == GK_TRUE) {
        pstLinkage->u32PirisGain = pstAeResult->u32PirisGain;
    } else {
        pstLinkage->u32PirisGain = 0;
    }
    pstLinkage->enPreFSWDRMode = pstLinkage->enFSWDRMode;
    pstLinkage->enFSWDRMode    = pstAeResult->enFSWDRMode;
}

GK_S32 ISP_AeCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue);
GK_S32 ISP_AeInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 i;
    ISP_AE_PARAM_S stAeParam;
    ISP_AE_INIT_INFO_S stAeInitInfo;
    ISP_CTX_S      *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib    = GK_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    AeRegsDefault(ViPipe, (ISP_REG_CFG_S *)pRegCfg);
    ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

    stAeParam.SensorId      = pstIspCtx->stBindAttr.SensorId;
    stAeParam.u8WDRMode     = pstIspCtx->u8SnsWDRMode;
    stAeParam.u8HDRMode     = pstIspCtx->stHdrAttr.enDynamicRange;
    stAeParam.enBayer       = ext_system_rggb_cfg_read(ViPipe);
    stAeParam.f32Fps        = pstIspCtx->stSnsImageMode.f32Fps;
    stAeParam.u16BlackLevel = pstSnsBlackLevel->au16BlackLevel[1];

    memcpy(&stAeParam.stStitchAttr, &pstIspCtx->stStitchAttr, sizeof(ISP_STITCH_ATTR_S));

    /* init all registered ae libs */
    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed) {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_init != GK_NULL) {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_init(pstLib->stAlgLib.s32Id, &stAeParam);
            }

            if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl != GK_NULL) {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->stAlgLib.s32Id,
                                                             ISP_AE_INIT_INFO_GET, (GK_VOID *)&stAeInitInfo);
            }
        }
    }
    pstIspCtx->stLinkage.u32IspDgain = MAX2(stAeInitInfo.u32IspDgain, 0x100);
    pstIspCtx->stLinkage.u32Iso      = MAX2(stAeInitInfo.u32Iso, 100);
    pstIspCtx->stAttachInfoCtrl.pstAttachInfo->u32InitISO = pstIspCtx->stLinkage.u32Iso;

    return GK_SUCCESS;
}

GK_S32 ISP_AeRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                 GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_S32 i;
    GK_U8  u8BayerFormat = 0;
    GK_S32 s32Ret = GK_FAILURE;
    GK_U16 u16BlackOffset = 0;
    GK_U8  u8RCEnable = 0;
    static GK_U32 u32UpdateCnt = 0;
    static GK_U32 u32WaitCnt = 0;
    static GK_BOOL bInTime = GK_FALSE;
    ISP_AE_INFO_S       stAeInfo    = {0};
    ISP_AE_RESULT_S     stAeResult  = {{0}};

    ISP_LINKAGE_S           *pstLinkage   = GK_NULL;
    ISP_CTX_S               *pstIspCtx    = GK_NULL;
    ISP_LIB_NODE_S          *pstLib       = GK_NULL;
    ISP_STAT_S              *pIspStatInfo = GK_NULL;
    ISP_REG_CFG_S           *pstRegCfg    = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstLib       = &pstIspCtx->stAeLibInfo.astLibs[pstIspCtx->stAeLibInfo.u32ActiveLib];
    pstRegCfg    = (ISP_REG_CFG_S *)pRegCfg;
    pIspStatInfo = (ISP_STAT_S *)pStatInfo;
    pstLinkage   = &pstIspCtx->stLinkage;

    if (pstLinkage->bDefectPixel) {
        return GK_SUCCESS;
    }

    if (pstLinkage->bSnapState) {
        return GK_SUCCESS;
    }
    if ((IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)\
         || IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))\
        && (pstIspCtx->stLinkage.enSnapPipeMode == ISP_SNAP_PICTURE)) {
        ISP_CHECK_PIPE(pstIspCtx->stLinkage.s32PreviewPipeId);
        pstIspCtx->stLinkage.u32IsoDoneFrmcnt = ISP_FrameCntGet(pstIspCtx->stLinkage.s32PreviewPipeId);
        return GK_SUCCESS;
    }

    if (pstIspCtx->stLinkage.bStatReady == GK_FALSE) {
        return GK_SUCCESS;
    }

    stAeInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAeInfo.pstFEAeStat1 = &pIspStatInfo->stFEAeStat1;
    stAeInfo.pstFEAeStat2 = &pIspStatInfo->stFEAeStat2;
    stAeInfo.pstFEAeStat3 = &pIspStatInfo->stFEAeStat3;
    stAeInfo.pstBEAeStat1 = &pIspStatInfo->stBEAeStat1;
    stAeInfo.pstBEAeStat2 = &pIspStatInfo->stBEAeStat2;
    stAeInfo.pstBEAeStat3 = &pIspStatInfo->stBEAeStat3;

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        stAeInfo.stSmartInfo.stROI[i].bEnable     = ext_system_smart_enable_read(ViPipe, i);
        stAeInfo.stSmartInfo.stROI[i].u8Luma     = ext_system_smart_luma_read(ViPipe, i);
        stAeInfo.stSmartInfo.stROI[i].bAvailable = ext_system_smart_available_read(ViPipe, i);
    }

    if (ext_system_smart_update_read(ViPipe)) {
        u32UpdateCnt++;
        ext_system_smart_update_write(ViPipe, GK_FALSE);
    } else {
        u32WaitCnt++;
    }

    if (u32UpdateCnt) {
        if (u32WaitCnt < 20) {
            bInTime = GK_TRUE;
        }
        u32UpdateCnt = 0;
        u32WaitCnt   = 0;
	}
	if(u32WaitCnt >= 20)		
    {
        bInTime = GK_FALSE;
    }

    if (!bInTime) {
        for (i = 0; i < SMART_CLASS_MAX; i++) {
            stAeInfo.stSmartInfo.stROI[i].bAvailable = GK_FALSE;
        }
    }

    if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl != GK_NULL) {
        u8BayerFormat = ext_system_rggb_cfg_read(ViPipe);
        pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->stAlgLib.s32Id,
                                                     ISP_AE_BAYER_FORMAT_SET, (GK_VOID *)&u8BayerFormat);
        u16BlackOffset = ext_system_black_level_query_00_read(ViPipe);
        pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->stAlgLib.s32Id,
                                                     ISP_AE_BLC_SET, (GK_VOID *)&u16BlackOffset);
        u8RCEnable = ext_system_rc_en_read(ViPipe);
        pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->stAlgLib.s32Id,
                                                     ISP_AE_RC_SET, (GK_VOID *)&u8RCEnable);
    }
    if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_run != GK_NULL) {
        s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_run(pstLib->stAlgLib.s32Id, &stAeInfo, &stAeResult, 0);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "WARNING!! ISP[%d] run ae lib err!\n", ViPipe);
        }
    }
    if (stAeResult.u32AgainSF == 0 || stAeResult.u32DgainSF == 0 || stAeResult.u32IspDgainSF == 0 || stAeResult.u32IsoSF == 0) {
        stAeResult.u32AgainSF = stAeResult.u32Again;
        stAeResult.u32DgainSF = stAeResult.u32Dgain;
        stAeResult.u32IspDgainSF = stAeResult.u32IspDgain;
        stAeResult.u32IsoSF = stAeResult.u32Iso;
    }

    AeReadExtregs(ViPipe, &stAeResult);
    AeResReadExtregs(ViPipe, pstRegCfg);
    AeRegsRangeCheck(ViPipe, &stAeResult);
    AeUpdateConfig(ViPipe, &stAeResult, pstRegCfg);
    AeUpdateLinkage(ViPipe, &stAeResult, pstLinkage);
    if (pstIspCtx->stLinkage.enSnapPipeMode != ISP_SNAP_PICTURE) {
        AeGetFrameInfo(ViPipe, &stAeResult);
        AeGetDCFInfo(ViPipe, &stAeResult);
    }
    ext_system_sys_iso_write(ViPipe, stAeResult.u32Iso);
    return s32Ret;
}

GK_S32 ISP_AeCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    GK_S32  i, s32Ret = GK_FAILURE;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib = GK_NULL;
    ISP_REGCFG_S  *pstRegCfg = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

    pstLib = &pstIspCtx->stAeLibInfo.astLibs[pstIspCtx->stAeLibInfo.u32ActiveLib];

    if (u32Cmd == ISP_PROC_WRITE) {
        if (pstLib->bUsed) {
            if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl != GK_NULL) {
                s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    } else {
        for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
            if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed) {
                pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
                if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl != GK_NULL) {
                    s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->stAlgLib.s32Id, u32Cmd, pValue);
                }
            }
        }
    }

    if (u32Cmd == ISP_WDR_MODE_SET) {
        AeRegsDefault(ViPipe, &pstRegCfg->stRegCfg);
    }

    if (u32Cmd == ISP_CHANGE_IMAGE_MODE_SET) {
        AeRegsDefault(ViPipe, &pstRegCfg->stRegCfg);
    }

    return s32Ret;
}

GK_S32 ISP_AeExit(VI_PIPE ViPipe)
{
    GK_S32 i;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed) {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_exit != GK_NULL) {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_exit(pstLib->stAlgLib.s32Id);
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterAe(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Ae);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AeExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
