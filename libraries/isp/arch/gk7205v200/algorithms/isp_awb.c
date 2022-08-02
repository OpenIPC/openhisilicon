/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "math_fun.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Convert to direct presentation (used in the ISP) */
static GK_U16 AwbComplementToDirect(GK_S16 s16Value)
{
    GK_U16 u16Result;

    if (s16Value >= 0) {
        u16Result = s16Value;
    } else {
        u16Result = -s16Value;
        u16Result |= (1 << 15);
    }

    return u16Result;
}

/* Convert to complement presentation (used in the firmware for calculations) */
static GK_S16 AwbDirectToComplement(GK_U16 u16Value)
{
    GK_S16 s16Result;

    s16Result = u16Value & (~(1 << 15));

    if (u16Value & (1 << 15)) {
        s16Result = -s16Result;
    }

    return s16Result;
}

/* Multiply the two matrixes a1[dim1 x dim2] a2[dim2 x dim3] */
static GK_VOID AwbMatrixMultiply(GK_S16 *ps16Matrix1, GK_S16 *ps16Matrix2,
                                 GK_S16 *ps16Result, GK_S32 s32Dim1, GK_S32 s32Dim2, GK_S32 s32Dim3)
{
    GK_S32 i, j, k;
    GK_S32 s32Temp;

    for (i = 0; i < s32Dim1; ++i) {
        for (j = 0; j < s32Dim3; ++j) {
            s32Temp = 0;

            for (k = 0; k < s32Dim2; ++k) {
                s32Temp += (((GK_S32)ps16Matrix1[i * s32Dim2 + k] * ps16Matrix2[k * s32Dim3 + j]));
            }

            ps16Result[i * s32Dim3 + j] = (GK_S16)((s32Temp + 128) / 256);
        }
    }

    for (i = 0; i < s32Dim1; ++i) {
        s32Temp = 0;

        for (j = 0; j < s32Dim3; ++j) {
            s32Temp += (GK_S32)ps16Result[i * s32Dim3 + j];
        }

        if (0x0 != s32Temp) {
            for (j = 0; j < s32Dim3; ++j) {
                ps16Result[i * s32Dim3 + j] = (GK_S16)(ps16Result[i * s32Dim3 + j] * 0x100 / DIV_0_TO_1(s32Temp));
            }
        }
    }

    for (i = 0; i < s32Dim1; ++i) {
        s32Temp = 0;

        for (j = 0; j < s32Dim3; ++j) {
            s32Temp += (GK_S16)ps16Result[i * s32Dim3 + j];
        }

        if (0x100 != s32Temp) {
            ps16Result[i * s32Dim3 + i] += (0x100 - s32Temp);
        }
    }

    return;
}

static GK_VOID AwbResRegsDefault(VI_PIPE ViPipe, GK_U8 i, GK_U8 u8BlockNum, ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg)
{
    GK_U16 u16Overlap;
    ISP_RECT_S stBlockRect;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    // BEDYNAMIC
    ISP_GetBlockRect(&stBlockRect, &pstIspCtx->stBlockAttr, i);
    pstAwbRegDynCfg->u32BEWidth = stBlockRect.u32Width;
    pstAwbRegDynCfg->u32BEHeight = stBlockRect.u32Height;

    u16Overlap = pstIspCtx->stBlockAttr.u32OverLap;

    // awb overlap configs
    if (i == 0) {
        if (u8BlockNum > 1) {
            pstAwbRegDynCfg->u32BECropPosX = 0;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutWidth = pstAwbRegDynCfg->u32BEWidth - u16Overlap;
            pstAwbRegDynCfg->u32BECropOutHeight = pstAwbRegDynCfg->u32BEHeight;
        } else {
            pstAwbRegDynCfg->u32BECropPosX = 0;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.u32Height;
            pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.u32Width;
        }
    } else if (i == (u8BlockNum - 1)) {
        pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
        pstAwbRegDynCfg->u32BECropPosY = 0;
        pstAwbRegDynCfg->u32BECropOutWidth = pstAwbRegDynCfg->u32BEWidth - u16Overlap;
        pstAwbRegDynCfg->u32BECropOutHeight = pstAwbRegDynCfg->u32BEHeight;
    } else {
        pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
        pstAwbRegDynCfg->u32BECropPosY = 0;
        pstAwbRegDynCfg->u32BECropOutWidth = pstAwbRegDynCfg->u32BEWidth - (u16Overlap << 1);
        pstAwbRegDynCfg->u32BECropOutHeight = pstAwbRegDynCfg->u32BEHeight;
    }

    ext_system_awb_crop_en_write(ViPipe, ISP_EXT_SYSTEM_AWB_CROP_EN_DEFAULT);
    ext_system_awb_crop_x_write(ViPipe, 0);
    ext_system_awb_crop_y_write(ViPipe, 0);
    ext_system_awb_crop_height_write(ViPipe, pstIspCtx->stBlockAttr.stFrameRect.u32Height);
    ext_system_awb_crop_width_write(ViPipe, pstIspCtx->stBlockAttr.stFrameRect.u32Width);
}

static GK_VOID AwbResReadExtregs(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8 i, u8AWBZoneRow, u8AWBZoneCol;
    GK_U8 u8BlockNum = 0;
    GK_U8 u8CropEn;
    GK_U16 u16Overlap;
    GK_U32 u16CropX, u16CropY, u16CropHeight, u16CropWidth;
    ISP_RECT_S stBlockRect;

    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = GK_NULL;
    u8BlockNum = ((ISP_REG_CFG_S *)pstRegCfg)->u8CfgNum;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;
    u16Overlap = pstIspCtx->stBlockAttr.u32OverLap;

    u8CropEn = ext_system_awb_crop_en_read(ViPipe);
    u16CropX = (ext_system_awb_crop_x_read(ViPipe) >> 1) << 1;
    u16CropY = (ext_system_awb_crop_y_read(ViPipe) >> 1) << 1;
    u16CropHeight = (ext_system_awb_crop_height_read(ViPipe) >> 1) << 1;
    u16CropWidth = (ext_system_awb_crop_width_read(ViPipe) >> 1) << 1;

    u8AWBZoneRow = ext_system_awb_vnum_read(ViPipe);
    u8AWBZoneCol = ext_system_awb_hnum_read(ViPipe);

    u16CropHeight = CLIP3(u16CropHeight, u8AWBZoneRow * AWB_MIN_HEIGHT, pstIspCtx->stBlockAttr.stFrameRect.u32Height);
    u16CropWidth = CLIP3(u16CropWidth, u8AWBZoneCol * AWB_MIN_WIDTH, pstIspCtx->stBlockAttr.stFrameRect.u32Width);
    u16CropY = MIN2(u16CropY, (pstIspCtx->stBlockAttr.stFrameRect.u32Height - u16CropHeight));
    u16CropX = MIN2(u16CropX, (pstIspCtx->stBlockAttr.stFrameRect.u32Width - u16CropWidth));

    /* AWB BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AWB Configs */
        pstAwbRegDynCfg = &pstRegCfg->stAlgRegCfg[i].stAwbRegCfg.stAwbRegDynCfg;

        /* AWB Size Configs */
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->stBlockAttr, i);

        /* AWB Overlap Configs */
        if (i == 0) {
            if (u8BlockNum > 1) {
                pstAwbRegDynCfg->u32BECropPosX = 0;
                pstAwbRegDynCfg->u32BECropPosY = 0;
                pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.u32Height;
                pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.u32Width - u16Overlap;
            } else {
                if (u8CropEn) {
                    pstAwbRegDynCfg->u32BECropPosX = u16CropX;
                    pstAwbRegDynCfg->u32BECropPosY = u16CropY;
                    pstAwbRegDynCfg->u32BECropOutHeight = u16CropHeight;
                    pstAwbRegDynCfg->u32BECropOutWidth = u16CropWidth;
                } else {
                    pstAwbRegDynCfg->u32BECropPosX = 0;
                    pstAwbRegDynCfg->u32BECropPosY = 0;
                    pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.u32Height;
                    pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.u32Width;
                }
            }
        } else if (i == (u8BlockNum - 1)) {
            pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.u32Height;
            pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.u32Width - u16Overlap;
        } else {
            pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.u32Height;
            pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.u32Width - (u16Overlap << 1);
        }
    }
    pstRegCfg->unKey.bit1AwbDynCfg = 1;
    pstRegCfg->stKernelRegCfg.unKey.bit1AwbCfg = 1;
}

static GK_VOID AwbImageModeSet(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8 i;
    GK_U8 u8AWBZoneCol, u8AWBZoneRow;
    GK_U16 u16Height, u16Width;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u16Height = pstIspCtx->stBlockAttr.stFrameRect.u32Height;
    u16Width = pstIspCtx->stBlockAttr.stFrameRect.u32Width;
    u8AWBZoneCol = ext_system_awb_hnum_read(ViPipe);
    u8AWBZoneRow = ext_system_awb_vnum_read(ViPipe);

    if (u8AWBZoneCol * AWB_MIN_WIDTH > u16Width) {
        u8AWBZoneCol = u16Width / AWB_MIN_WIDTH;
    }

    if (u8AWBZoneRow * AWB_MIN_HEIGHT > u16Height) {
        u8AWBZoneRow = u16Height / AWB_MIN_HEIGHT;
    }

    u8AWBZoneCol = CLIP3(u8AWBZoneCol, pstIspCtx->stBlockAttr.u8BlockNum, AWB_ZONE_ORIG_COLUMN);
    u8AWBZoneRow = CLIP3(u8AWBZoneRow, 1, AWB_ZONE_ORIG_ROW);

    ext_system_awb_hnum_write(ViPipe, u8AWBZoneCol);
    ext_system_awb_vnum_write(ViPipe, u8AWBZoneRow);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstAwbRegUsrCfg = &pstRegCfg->stAlgRegCfg[i].stAwbRegCfg.stAwbRegUsrCfg;

        AwbResRegsDefault(ViPipe, i, pstRegCfg->u8CfgNum, &pstRegCfg->stAlgRegCfg[i].stAwbRegCfg.stAwbRegDynCfg);

        if (i < u8AWBZoneCol % DIV_0_TO_1(pstRegCfg->u8CfgNum)) {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(pstRegCfg->u8CfgNum) + 1;
        } else {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(pstRegCfg->u8CfgNum);
        }

        pstAwbRegUsrCfg->u16BEZoneRow = u8AWBZoneRow;

        pstAwbRegUsrCfg->bResh = GK_TRUE;
        pstAwbRegUsrCfg->u32UpdateIndex += 1;
    }

    pstRegCfg->unKey.bit1AwbDynCfg = 1;
}

static GK_VOID AwbRegsDefault(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg, GK_U8 u8AWBZoneRow, GK_U8 u8AWBZoneCol, GK_U8 u8AWBZoneBin)
{
    GK_U8 i = 0;
    GK_U8 u8BlockNum = 0;
    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = GK_NULL;
    ISP_AWB_REG_STA_CFG_S *pstAwbRegStaCfg = GK_NULL;
    ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;

    pstAwbRegDynCfg = &pstRegCfg->stAlgRegCfg[0].stAwbRegCfg.stAwbRegDynCfg;  // dynamic
    pstAwbRegStaCfg = &pstRegCfg->stAlgRegCfg[0].stAwbRegCfg.stAwbRegStaCfg;  // static

    // FE DYNAMIC
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[0] = 0x1ED;
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[1] = 0x100;
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[2] = 0x100;
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[3] = 0x1AB;
    pstAwbRegDynCfg->u8FEWbWorkEn = GK_TRUE;

    // FE STATIC
    pstAwbRegStaCfg->bFEAwbStaCfg = 1;

    pstAwbRegStaCfg->u32FEClipValue = 0xFFFFF;

    pstRegCfg->unKey.bit1AwbDynCfg = 1;

    for (i = 0; i < u8BlockNum; i++) {
        pstAwbRegStaCfg = &(pstRegCfg->stAlgRegCfg[i].stAwbRegCfg.stAwbRegStaCfg);  // static
        pstAwbRegDynCfg = &(pstRegCfg->stAlgRegCfg[i].stAwbRegCfg.stAwbRegDynCfg);  // dynamic
        pstAwbRegUsrCfg = &(pstRegCfg->stAlgRegCfg[i].stAwbRegCfg.stAwbRegUsrCfg);  // user

        // BE STATIC
        pstAwbRegStaCfg->bBEAwbStaCfg = 1;
        pstAwbRegStaCfg->u8BEAwbBitmove = 0x0;
        pstAwbRegStaCfg->u8BEWbWorkEn = GK_TRUE;
        pstAwbRegStaCfg->u8BEAwbWorkEn = GK_TRUE;

        pstAwbRegStaCfg->u32BEAwbStatRaddr = 0x000;
        pstAwbRegStaCfg->u8BECcColortoneEn = 0x0;

        pstAwbRegStaCfg->u32BETopK = 0x0;
        pstAwbRegStaCfg->u32BETopB = 0xFFFFF;
        pstAwbRegStaCfg->u32BEBotK = 0x0;
        pstAwbRegStaCfg->u32BEBotB = 0x0;

        pstAwbRegStaCfg->u32BECcInDc0 = 0x0;
        pstAwbRegStaCfg->u32BECcInDc1 = 0x0;
        pstAwbRegStaCfg->u32BECcInDc2 = 0x0;
        pstAwbRegStaCfg->u32BECcOutDc0 = 0x0;
        pstAwbRegStaCfg->u32BECcOutDc1 = 0x0;
        pstAwbRegStaCfg->u32BECcOutDc2 = 0x0;
        pstAwbRegStaCfg->u32BEWbClipValue = 0xFFFFF;
        pstAwbRegStaCfg->u16BEAwbOffsetComp = ISP_AWB_OFFSET_COMP_DEF;

        AwbResRegsDefault(ViPipe, i, u8BlockNum, pstAwbRegDynCfg);

        if (i < u8AWBZoneCol % DIV_0_TO_1(u8BlockNum)) {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlockNum) + 1;
        } else {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlockNum);
        }

        pstAwbRegDynCfg->au16BEColorMatrix[0] = 0x100;
        pstAwbRegDynCfg->au16BEColorMatrix[1] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[2] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[3] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[4] = 0x100;
        pstAwbRegDynCfg->au16BEColorMatrix[5] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[6] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[7] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[8] = 0x100;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[0] = 0x1ED;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[1] = 0x100;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[2] = 0x100;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[3] = 0x1AB;

        pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb = 0xffff;
        pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb = 0x0;
        pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb = 0x180;
        pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb = 0x40;
        pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb = 0x180;
        pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb = 0x40;

        pstAwbRegDynCfg->u8BEWbWorkEn = GK_TRUE;
        pstAwbRegDynCfg->u8BECcEn = GK_TRUE;

        pstAwbRegDynCfg->u16BECcColortoneEn = ISP_CCM_COLORTONE_EN_DEFAULT;
        pstAwbRegDynCfg->u16BECcRGain = ISP_CCM_COLORTONE_RGAIN_DEFAULT;
        pstAwbRegDynCfg->u16BECcGGain = ISP_CCM_COLORTONE_GGAIN_DEFAULT;
        pstAwbRegDynCfg->u16BECcBGain = ISP_CCM_COLORTONE_BGAIN_DEFAULT;

        pstAwbRegUsrCfg->u16BEZoneRow = u8AWBZoneRow;
        pstAwbRegUsrCfg->u16BEZoneBin = u8AWBZoneBin;
        pstAwbRegUsrCfg->u16BEMeteringBinHist0 = 0xffff;
        pstAwbRegUsrCfg->u16BEMeteringBinHist1 = 0xffff;
        pstAwbRegUsrCfg->u16BEMeteringBinHist2 = 0xffff;
        pstAwbRegUsrCfg->u16BEMeteringBinHist3 = 0xffff;
        pstAwbRegUsrCfg->enBEAWBSwitch = ISP_AWB_AFTER_DG;
        pstAwbRegUsrCfg->bResh = GK_TRUE;
        pstAwbRegUsrCfg->u32UpdateIndex = 1;

        pstRegCfg->stKernelRegCfg.unKey.bit1AwbCfg = 1;
        pstRegCfg->stKernelRegCfg.astAlgKernelCfg[i].stAWBZoneCfg.u32Row = u8AWBZoneRow;
        pstRegCfg->stKernelRegCfg.astAlgKernelCfg[i].stAWBZoneCfg.u32Colum = u8AWBZoneCol;
        pstRegCfg->stKernelRegCfg.astAlgKernelCfg[i].stAWBZoneCfg.u32ZoneBin = u8AWBZoneBin;

        ext_system_cc_enable_write(ViPipe, pstAwbRegDynCfg->u8BECcEn);
        ext_system_awb_gain_enable_write(ViPipe, pstAwbRegDynCfg->u8BEWbWorkEn);
        ext_system_awb_white_level_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb);
        ext_system_awb_black_level_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb);
        ext_system_awb_cr_ref_max_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb);
        ext_system_awb_cr_ref_min_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb);
        ext_system_awb_cb_ref_max_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb);
        ext_system_awb_cb_ref_min_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb);
        ext_system_cc_colortone_rgain_write(ViPipe, pstAwbRegDynCfg->u16BECcRGain);
        ext_system_cc_colortone_ggain_write(ViPipe, pstAwbRegDynCfg->u16BECcGGain);
        ext_system_cc_colortone_bgain_write(ViPipe, pstAwbRegDynCfg->u16BECcBGain);

        // User
        ext_system_awb_sta_enable_write(ViPipe, pstAwbRegStaCfg->u8BEAwbWorkEn);
        ext_system_wb_statistics_mpi_update_en_write(ViPipe, pstAwbRegUsrCfg->bResh);
        ext_system_awb_switch_write(ViPipe, pstAwbRegUsrCfg->enBEAWBSwitch);
        ext_system_awb_hnum_write(ViPipe, u8AWBZoneCol);  // the col num of the whole picture
        ext_system_awb_vnum_write(ViPipe, pstAwbRegUsrCfg->u16BEZoneRow);
        ext_system_awb_zone_bin_write(ViPipe, pstAwbRegUsrCfg->u16BEZoneBin);
        ext_system_awbst_bin_thresh0_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist0);
        ext_system_awbst_bin_thresh1_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist1);
        ext_system_awbst_bin_thresh2_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist2);
        ext_system_awbst_bin_thresh3_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist3);
    }

    return;
}

GK_S32 AwbCfgReg(VI_PIPE ViPipe, ISP_AWB_RESULT_S *pstAwbResult, GK_U8 u8WDRMode,
                 GK_U32 u32IspDgain, GK_U32 u32IspDgainShift, ISP_REG_CFG_S *pstRegCfg)
{
    GK_BOOL bUsrResh;
    GK_S32 i, k;
    GK_U32 au32WbGain[4];
    GK_U8 u8BlkNum;
    GK_U8 u8AWBZoneCol = AWB_ZONE_ORIG_COLUMN;
    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = GK_NULL;
    ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = GK_NULL;
    ISP_AWB_REG_STA_CFG_S *pstAwbRegStaCfg = GK_NULL;
    u8BlkNum = ((ISP_REG_CFG_S *)pstRegCfg)->u8CfgNum;

    bUsrResh = ext_system_wb_statistics_mpi_update_en_read(ViPipe);
    ext_system_wb_statistics_mpi_update_en_write(ViPipe, GK_FALSE);

    for (k = 0; k < u8BlkNum; k++) {
        pstAwbRegDynCfg = &(pstRegCfg->stAlgRegCfg[k].stAwbRegCfg.stAwbRegDynCfg);  // dynamic
        pstAwbRegUsrCfg = &(pstRegCfg->stAlgRegCfg[k].stAwbRegCfg.stAwbRegUsrCfg);  // user
        pstAwbRegStaCfg = &(pstRegCfg->stAlgRegCfg[k].stAwbRegCfg.stAwbRegStaCfg);  // sta

        if (pstAwbResult->stRawStatAttr.bStatCfgUpdate) { // CbCr
            ext_system_awb_cr_ref_max_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb, 0xFFF));
            ext_system_awb_cr_ref_min_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb, 0xFFF));
            ext_system_awb_cb_ref_max_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb, 0xFFF));
            ext_system_awb_cb_ref_min_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb, 0xFFF));
        }

        pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb = ext_system_awb_cr_ref_max_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb = ext_system_awb_cr_ref_min_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb = ext_system_awb_cb_ref_max_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb = ext_system_awb_cb_ref_min_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb = ext_system_awb_white_level_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb = ext_system_awb_black_level_read(ViPipe);

        for (i = 0; i < 9; i++) {
            pstAwbRegDynCfg->au16BEColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
        }

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
            au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
            au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];
            pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = (GK_U16)au32WbGain[i];
        }

        u8AWBZoneCol = ext_system_awb_hnum_read(ViPipe);

        if (bUsrResh == GK_TRUE) {
            if (k < u8AWBZoneCol % DIV_0_TO_1(u8BlkNum)) {
                pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlkNum) + 1;
            } else {
                pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlkNum);
            }

            pstAwbRegUsrCfg->u32UpdateIndex += 1;

            pstAwbRegUsrCfg->enBEAWBSwitch = ext_system_awb_switch_read(ViPipe);
            pstAwbRegUsrCfg->u16BEZoneRow = ext_system_awb_vnum_read(ViPipe);
            pstAwbRegUsrCfg->u16BEZoneBin = ext_system_awb_zone_bin_read(ViPipe);

            pstAwbRegUsrCfg->u16BEMeteringBinHist0 = ext_system_awbst_bin_thresh0_read(ViPipe);
            pstAwbRegUsrCfg->u16BEMeteringBinHist1 = ext_system_awbst_bin_thresh1_read(ViPipe);
            pstAwbRegUsrCfg->u16BEMeteringBinHist2 = ext_system_awbst_bin_thresh2_read(ViPipe);
            pstAwbRegUsrCfg->u16BEMeteringBinHist3 = ext_system_awbst_bin_thresh3_read(ViPipe);
            pstAwbRegUsrCfg->bResh = GK_TRUE;
        }

        pstAwbRegDynCfg->u8BECcEn = ext_system_cc_enable_read(ViPipe);
        pstAwbRegDynCfg->u16BECcBGain = ext_system_cc_colortone_bgain_read(ViPipe);
        pstAwbRegDynCfg->u16BECcGGain = ext_system_cc_colortone_ggain_read(ViPipe);
        pstAwbRegDynCfg->u16BECcRGain = ext_system_cc_colortone_rgain_read(ViPipe);

        pstAwbRegDynCfg->u8BEWbWorkEn = ext_system_awb_gain_enable_read(ViPipe);
        pstAwbRegDynCfg->u8FEWbWorkEn = ext_system_awb_gain_enable_read(ViPipe);
        pstAwbRegStaCfg->u8BEAwbWorkEn = ext_system_awb_sta_enable_read(ViPipe);

        pstRegCfg->stKernelRegCfg.astAlgKernelCfg[k].stAWBZoneCfg.u32Row = pstAwbRegUsrCfg->u16BEZoneRow;
        pstRegCfg->stKernelRegCfg.astAlgKernelCfg[k].stAWBZoneCfg.u32Colum = u8AWBZoneCol;
        pstRegCfg->stKernelRegCfg.astAlgKernelCfg[k].stAWBZoneCfg.u32ZoneBin = pstAwbRegUsrCfg->u16BEZoneBin;
    }

    // FE
    for (i = 0; i < 4; i++) {
        au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
        au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
        au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];
        pstRegCfg->stAlgRegCfg[0].stAwbRegCfg.stAwbRegDynCfg.au32FEWhiteBalanceGain[i] = (GK_U16)au32WbGain[i];
    }

    pstRegCfg->unKey.bit1AwbDynCfg = 1;
    pstRegCfg->stKernelRegCfg.unKey.bit1AwbCfg = 1;

    return GK_SUCCESS;
}

GK_S32 AwbCfgInitReg(VI_PIPE ViPipe, ISP_AWB_RESULT_S *pstAwbResult, ISP_REG_CFG_S *pstRegCfg)
{
    GK_S32 i, k;
    GK_U32 au32WbGain[4];
    GK_U8 u8BlkNum;
    ISP_AWB_REG_DYN_CFG_S  *pstAwbRegDynCfg = GK_NULL;
    u8BlkNum = ((ISP_REG_CFG_S *)pstRegCfg)->u8CfgNum;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
        au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
        au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];
        pstRegCfg->stAlgRegCfg[0].stAwbRegCfg.stAwbRegDynCfg.au32FEWhiteBalanceGain[i] = (GK_U16)au32WbGain[i];
    }

    for (k = 0; k < u8BlkNum; k++) {
        pstAwbRegDynCfg = &(pstRegCfg->stAlgRegCfg[k].stAwbRegCfg.stAwbRegDynCfg); // dynamic

        for (i = 0; i < 9; i++) {
            pstAwbRegDynCfg->au16BEColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
        }

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = (GK_U16)au32WbGain[i];
        }
    }

    pstRegCfg->unKey.bit1AwbDynCfg = 1;

    return GK_SUCCESS;
}

GK_S32 ISP_AwbCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue);
GK_S32 ISP_AwbInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_U8 u8ActualZoneCol, u8ActualZoneRow;
    GK_U16 u16Height, u16Width;
    GK_S32 i;
    ISP_AWB_PARAM_S stAwbParam;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib = GK_NULL;
    ISP_AWB_RESULT_S stAwbInitResult = {{0}};

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u16Height = pstIspCtx->stBlockAttr.stFrameRect.u32Height;
    u16Width = pstIspCtx->stBlockAttr.stFrameRect.u32Width;

    if (u16Width < AWB_ZONE_ORIG_COLUMN * AWB_MIN_WIDTH) {
        u8ActualZoneCol = u16Width / AWB_MIN_WIDTH;
    } else {
        u8ActualZoneCol = AWB_ZONE_ORIG_COLUMN;
    }

    if (u16Height < AWB_ZONE_ORIG_ROW * AWB_MIN_HEIGHT) {
        u8ActualZoneRow = u16Height / AWB_MIN_HEIGHT;
    } else {
        u8ActualZoneRow = AWB_ZONE_ORIG_ROW;
    }

    stAwbParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    stAwbParam.u8WDRMode = pstIspCtx->u8SnsWDRMode;
    stAwbParam.u32InitIso = pstIspCtx->stLinkage.u32Iso;
    memcpy(&stAwbParam.stStitchAttr, &pstIspCtx->stStitchAttr, sizeof(ISP_STITCH_ATTR_S));

    if (stAwbParam.stStitchAttr.bStitchEnable == 1) {
        GK_U32 u32BlockWidth = u8ActualZoneCol;

        AwbRegsDefault(ViPipe, (ISP_REG_CFG_S *)pRegCfg, u8ActualZoneRow, u32BlockWidth, 1);
        stAwbParam.u8AWBZoneRow = (GK_U8)(u8ActualZoneRow);
        stAwbParam.u8AWBZoneCol = MIN2(255, (u32BlockWidth * stAwbParam.stStitchAttr.u8StitchPipeNum));
        stAwbParam.u8AWBZoneBin = 1;
    } else {
        AwbRegsDefault(ViPipe, (ISP_REG_CFG_S *)pRegCfg, u8ActualZoneRow, u8ActualZoneCol, 1);

        stAwbParam.u8AWBZoneRow = u8ActualZoneRow;
        stAwbParam.u8AWBZoneCol = u8ActualZoneCol;
        stAwbParam.u8AWBZoneBin = 1;
    }

    stAwbParam.u16AWBWidth = pstIspCtx->stBlockAttr.stFrameRect.u32Width;
    stAwbParam.u16AWBHeight = pstIspCtx->stBlockAttr.stFrameRect.u32Height;

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed) {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];

            if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_init != GK_NULL) {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_init(pstLib->stAlgLib.s32Id, &stAwbParam, &stAwbInitResult);
            }
        }
    }

    AwbCfgInitReg(ViPipe, &stAwbInitResult,  (ISP_REG_CFG_S *)pRegCfg);
    return GK_SUCCESS;
}

GK_S32 ISP_AwbRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                  GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_S32 i, s32Ret = GK_FAILURE;
    ISP_AWB_PARAM_S stAwbParam;
    ISP_AWB_INFO_S stAwbInfo = { 0 };
    ISP_AWB_RESULT_S stAwbResult = { { 0 } };
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib = GK_NULL;
    GK_BOOL bLoadCCM;
    GK_U32 u32DiffGain;
    AWB_CCM_CONFIG_S stCCMConf, stCCMConfDef;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel) {
        return GK_SUCCESS;
    }

    bLoadCCM = pstIspCtx->stLinkage.bLoadCCM;

    if (pstIspCtx->stLinkage.bSnapState == GK_TRUE) {
        if (bLoadCCM == GK_TRUE) {
            return GK_SUCCESS;
        } else {
            if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl != GK_NULL) {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                AWB_CCM_CONFIG_GET, (GK_VOID *)&stCCMConfDef);

                stCCMConf.bAWBBypassEn = GK_TRUE;
                stCCMConf.bManualTempEn = GK_TRUE;
                stCCMConf.u32ManualTempValue = pstIspCtx->stLinkage.u32ColorTemp;
                stCCMConf.u16CCMSpeed = 0xfff;

                stCCMConf.bManualSatEn = stCCMConfDef.bManualSatEn;
                stCCMConf.u32ManualSatValue = stCCMConfDef.u32ManualSatValue;
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                AWB_CCM_CONFIG_SET, (GK_VOID *)&stCCMConf);
            }
        }
    } else {
        if (pstIspCtx->stLinkage.bStatReady == GK_FALSE) {
            return GK_SUCCESS;
        }
    }

    if ((IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)\
         || IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))\
        && (pstIspCtx->stLinkage.enSnapPipeMode == ISP_SNAP_PICTURE)) {
        return GK_SUCCESS;
    }
    AwbResReadExtregs(ViPipe, (ISP_REG_CFG_S *)pRegCfg);
    /* get statistics */
    stAwbInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;
    stAwbInfo.u8AwbGainSwitch = WDR_WBGAIN_IN_WB;
    stAwbInfo.u8AwbStatSwitch = ext_system_awb_switch_read(ViPipe);

    stAwbParam.u8AWBZoneRow = ext_system_awb_vnum_read(ViPipe);
    stAwbParam.u8AWBZoneCol = ext_system_awb_hnum_read(ViPipe);
    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        stAwbParam.u8AWBZoneCol = MIN2(255, (stAwbParam.u8AWBZoneCol * pstIspCtx->stStitchAttr.u8StitchPipeNum));
    }

    /* linkage with the iso of ae */
    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed) {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];

            if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl != GK_NULL) {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                ISP_AWB_ISO_SET, (GK_VOID *)&pstIspCtx->stLinkage.u32Iso);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                ISP_AWB_INTTIME_SET, (GK_VOID *)&pstIspCtx->stLinkage.u32Inttime);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                ISP_AWB_SNAP_MODE_SET, (GK_VOID *)&pstIspCtx->stLinkage.enSnapPipeMode);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                ISP_AWB_ZONE_ROW_SET, (GK_VOID *)&stAwbParam.u8AWBZoneRow);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                                ISP_AWB_ZONE_COL_SET, (GK_VOID *)&stAwbParam.u8AWBZoneCol);
            }
        }
    }

    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];
    stAwbInfo.pstAwbStat1 = &((ISP_STAT_S *)pStatInfo)->stAwbStat1;
    stAwbInfo.stAwbStat2.pau16ZoneAvgR = (((ISP_STAT_S *)pStatInfo)->stAwbStat2.au16MeteringMemArrayAvgR);
    stAwbInfo.stAwbStat2.pau16ZoneAvgG = (((ISP_STAT_S *)pStatInfo)->stAwbStat2.au16MeteringMemArrayAvgG);
    stAwbInfo.stAwbStat2.pau16ZoneAvgB = (((ISP_STAT_S *)pStatInfo)->stAwbStat2.au16MeteringMemArrayAvgB);
    stAwbInfo.stAwbStat2.pau16ZoneCount = (((ISP_STAT_S *)pStatInfo)->stAwbStat2.au16MeteringMemArrayCountAll);

    if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run != GK_NULL) {
        s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run(pstLib->stAlgLib.s32Id, &stAwbInfo, &stAwbResult, 0);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "WARNING!! ISP[%d] run awb lib err!\n", ViPipe);
        }
    }

    pstIspCtx->stLinkage.u32ColorTemp = stAwbResult.u32ColorTemp;

    pstIspCtx->stLinkage.au32WhiteBalanceGain[0] = stAwbResult.au32WhiteBalanceGain[0];
    pstIspCtx->stLinkage.au32WhiteBalanceGain[1] = stAwbResult.au32WhiteBalanceGain[1];
    pstIspCtx->stLinkage.au32WhiteBalanceGain[2] = stAwbResult.au32WhiteBalanceGain[2];
    pstIspCtx->stLinkage.au32WhiteBalanceGain[3] = stAwbResult.au32WhiteBalanceGain[3];

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pstIspCtx->stLinkage.au16CCM[i] = stAwbResult.au16ColorMatrix[i];
    }

    {
        pstIspCtx->stAttachInfoCtrl.pstAttachInfo->stIspHdr.u32ColorTemp = pstIspCtx->stLinkage.u32ColorTemp;
        memcpy(pstIspCtx->stAttachInfoCtrl.pstAttachInfo->stIspHdr.au16CCM, pstIspCtx->stLinkage.au16CCM, CCM_MATRIX_SIZE * sizeof(GK_U16));
        pstIspCtx->stAttachInfoCtrl.pstAttachInfo->u32ISO = pstIspCtx->stLinkage.u32Iso;
        pstIspCtx->stAttachInfoCtrl.pstAttachInfo->u8SnsWDRMode = pstIspCtx->u8SnsWDRMode;
    }

    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        GK_S16 as16CalcCCM[CCM_MATRIX_SIZE] = { 0 };
        GK_S16 as16DiffCCM[CCM_MATRIX_SIZE] = { 0 };
        GK_S16 as16ResuCCM[CCM_MATRIX_SIZE] = { 0 };

        /* TODO: Multi-Pipe different config WBgain/CCM */
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            u32DiffGain = ext_system_isp_pipe_diff_gain_read(ViPipe, i);
            stAwbResult.au32WhiteBalanceGain[i] = (stAwbResult.au32WhiteBalanceGain[i] * u32DiffGain) >> 8;
        }

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            as16CalcCCM[i] = AwbDirectToComplement(stAwbResult.au16ColorMatrix[i]);
            as16DiffCCM[i] = ext_system_isp_pipe_diff_ccm_read(ViPipe, i);
            as16DiffCCM[i] = AwbDirectToComplement(as16DiffCCM[i]);
        }

        AwbMatrixMultiply(as16CalcCCM, as16DiffCCM, as16ResuCCM, 3, 3, 3);

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            stAwbResult.au16ColorMatrix[i] = AwbComplementToDirect(as16ResuCCM[i]);
        }
    }

    AwbCfgReg(ViPipe, &stAwbResult, pstIspCtx->u8SnsWDRMode, pstIspCtx->stLinkage.u32IspDgain,
              pstIspCtx->stLinkage.u32IspDgainShift, (ISP_REG_CFG_S *)pRegCfg);

    if (pstIspCtx->stLinkage.bSnapState == GK_TRUE) {
        if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl != GK_NULL) {
            pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                                                            AWB_CCM_CONFIG_SET, (GK_VOID *)&stCCMConfDef);
        }
    }

    return s32Ret;
}

GK_S32 ISP_AwbCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    GK_S32 i, s32Ret = GK_FAILURE;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib = GK_NULL;
    ISP_REGCFG_S *pstRegCfg = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];

    if (u32Cmd == ISP_PROC_WRITE) {
        if (pstLib->bUsed) {
            if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl != GK_NULL) {
                s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    } else {
        for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
            if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed) {
                pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
                if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl != GK_NULL) {
                    s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id, u32Cmd, pValue);
                }
            }
        }
    }

    if (u32Cmd == ISP_CHANGE_IMAGE_MODE_SET) {
        AwbImageModeSet(ViPipe, &pstRegCfg->stRegCfg);
    }

    if ((u32Cmd == ISP_WDR_MODE_SET) && (pstIspCtx->stBlockAttr.u8BlockNum != pstIspCtx->stBlockAttr.u8PreBlockNum)) {
        AwbImageModeSet(ViPipe, &pstRegCfg->stRegCfg);
    }

    return s32Ret;
}

GK_S32 ISP_AwbExit(VI_PIPE ViPipe)
{
    GK_S32 i;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_LIB_NODE_S *pstLib = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed) {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];

            if (pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_exit != GK_NULL) {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_exit(pstLib->stAlgLib.s32Id);
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterAwb(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Awb);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AWB;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AwbInit;
    pstAlgs->stAlgFunc.pfn_alg_run = ISP_AwbRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AwbCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AwbExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
