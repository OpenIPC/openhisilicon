/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "isp_config.h"
#include "isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define     ISP_SHARPEN_FREQ_CORING_LENGTH      (8)
#define     SHRP_GAIN_LUT_SFT                   (2)
#define     SHRP_GAIN_LUT_SIZE                  (64)
#define     SHRP_DIR_RANGE_MUL_PRECS            (4)
#define     SHRP_HF_EPS_MUL_PRECS               (4)
#define     SHRP_SHT_VAR_MUL_PRECS              (4)
#define     SHRP_SKIN_EDGE_MUL_PRECS            (4)
#define     SHRP_SKIN_ACCUM_MUL_PRECS           (3)
#define     SHRP_CHR_MUL_SFT                    (4)
#define     SHARPEN_EN                          (1)
#define     SHRP_DETAIL_SHT_MUL_PRECS           (4)
#define     SHRP_DETAIL_CTRL_THR_DELTA          (16)

#define SHARPEN_CLIP3(min,max,x) ( (x)<= (min) ? (min) : ((x)>(max)?(max):(x)) )

typedef struct ISP_SHARPEN_S {
    GK_BOOL bSharpenEn;
    GK_BOOL bSharpenMpiUpdateEn;
    GK_U32  u32IsoLast;
    /* Sharpening Yuv */
    // tmp registers
    GK_U8   u8ManualSharpenYuvEnabled;
    GK_U8   u8gainThdSftD;
    GK_U8   u8dirVarSft;

    GK_U8   u8selPixWgt;
    GK_U8   u8RmfGainScale;
    GK_U8   u8BmfGainScale;

    GK_U8   u8gainThdSelUD;
    GK_U8   u8gainThdSftUD;

    GK_U8    u8dirRlyThrLow;
    GK_U8    u8dirRlyThrhigh;

    GK_U8   u8shtVarWgt0;
    GK_U8   u8shtVarDiffThd0;
    GK_U8   u8shtVarDiffThd1;
    GK_U8   u8shtVarDiffWgt1;

    // MPI
    GK_U8   u8SkinUmin;
    GK_U8   u8SkinVmin;
    GK_U8   u8SkinUmax;
    GK_U8   u8SkinVmax;
    GK_U16  au16TextureStr[ISP_SHARPEN_GAIN_NUM];
    GK_U16  au16EdgeStr[ISP_SHARPEN_GAIN_NUM];
    GK_U8   au8LumaWgt[ISP_SHARPEN_LUMA_NUM];
    GK_U16  u16TextureFreq;
    GK_U16  u16EdgeFreq;
    GK_U8   u8OverShoot;
    GK_U8   u8UnderShoot;
    GK_U8   u8ShootSupStr;
    GK_U8   u8ShootSupAdj;
    GK_U8   u8DetailCtrl;
    GK_U8   u8DetailCtrlThr;
    GK_U8   u8EdgeFiltStr;                               //u8EdgeFiltStr          U6.0  [0, 63]
    GK_U8   u8EdgeFiltMaxCap;                            //u8EdgeFiltMaxCap    U6.0  [0, 47]
    GK_U8   u8RGain;
    GK_U8   u8GGain;
    GK_U8   u8BGain;
    GK_U8   u8SkinGain;
    GK_U16  u16MaxSharpGain;
    GK_U8   u8WeakDetailGain;

    GK_U16 au16AutoTextureStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16AutoEdgeStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8AutoLumaWgt[ISP_SHARPEN_LUMA_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16TextureFreq[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16EdgeFreq[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8ShootSupStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8ShootSupAdj[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8DetailCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];                               //u8EdgeFiltStr    U6.0  [0, 63]
    GK_U8  au8EdgeFiltMaxCap[ISP_AUTO_ISO_STRENGTH_NUM];                            //u8EdgeFiltStr    U6.0  [0, 47]
    GK_U8  au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8GGain[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8WeakDetailGain[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_SHARPEN_S;

ISP_SHARPEN_S *g_pastSharpenCtx[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define SHARPEN_GET_CTX(dev, pstCtx)   (pstCtx = g_pastSharpenCtx[dev])
#define SHARPEN_SET_CTX(dev, pstCtx)   (g_pastSharpenCtx[dev] = pstCtx)
#define SHARPEN_RESET_CTX(dev)         (g_pastSharpenCtx[dev] = GK_NULL)

GK_S32 shrp_blend(GK_U8 sft, GK_S32 wgt1, GK_S32 v1, GK_S32 wgt2, GK_S32 v2)
{
    GK_S32 res;
    res = SignedRightShift(((GK_S64)v1 * wgt1) + ((GK_S64)v2 * wgt2), sft);
    return res;
}


GK_S32 SharpenCtxInit(VI_PIPE ViPipe)
{
    ISP_SHARPEN_S *pastSharpenCtx = GK_NULL;

    SHARPEN_GET_CTX(ViPipe, pastSharpenCtx);

    if (pastSharpenCtx == GK_NULL) {
        pastSharpenCtx = (ISP_SHARPEN_S *)ISP_MALLOC(sizeof(ISP_SHARPEN_S));
        if (pastSharpenCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SharpenCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSharpenCtx, 0, sizeof(ISP_SHARPEN_S));

    SHARPEN_SET_CTX(ViPipe, pastSharpenCtx);

    return GK_SUCCESS;
}

GK_VOID SharpenCtxExit(VI_PIPE ViPipe)
{
    ISP_SHARPEN_S *pastSharpenCtx = GK_NULL;

    SHARPEN_GET_CTX(ViPipe, pastSharpenCtx);
    ISP_FREE(pastSharpenCtx);
    SHARPEN_RESET_CTX(ViPipe);
}

static GK_S32 SharpenCheckCmosParam(VI_PIPE ViPipe, const ISP_CMOS_SHARPEN_S *pstSharpen)
{
    GK_U8 i, j;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (pstSharpen->stAuto.au16TextureStr[j][i] > 4095) {
                ISP_TRACE(MODULE_DBG_ERR, "err au16TextureStr[%d][%d]! Value range:[0, 4095]\n", j, i);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
            if (pstSharpen->stAuto.au16EdgeStr[j][i] > 4095) {
                ISP_TRACE(MODULE_DBG_ERR, "err au16EdgeStr[%d][%d]! Value range:[0, 4095]\n", j, i);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            if (pstSharpen->stAuto.au8LumaWgt[j][i] > ISP_SHARPEN_LUMAWGT) {
                ISP_TRACE(MODULE_DBG_ERR, "err au8LumaWgt[%d][%d]! [0, %d]\n", j, i, ISP_SHARPEN_LUMAWGT);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }
        if (pstSharpen->stAuto.au16TextureFreq[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16TextureFreq[%d]! Value range:[0, 4095]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstSharpen->stAuto.au16EdgeFreq[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16EdgeFreq[%d]! Value range:[0, 4095]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstSharpen->stAuto.au8OverShoot[i] > 127) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8OverShoot[%d]! Value range:[0, 127]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstSharpen->stAuto.au8UnderShoot[i] > 127) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8UnderShoot[%d]! Value range:[0, 127]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8ShootSupAdj[i] > 15) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8ShootSupAdj[%d]! Value range:[0, 15]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8EdgeFiltStr[i] > 63) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8EdgeFiltStr[%d]! Value range:[0, 63]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8EdgeFiltMaxCap[i] > ISP_SHARPEN_EdgeFiltMaxCap_MAX ) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8EdgeFiltMaxCap[%d]! Value range:[0, %d]\n", i, ISP_SHARPEN_EdgeFiltMaxCap_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8RGain[i] > ISP_SHARPEN_RGAIN_MAX ) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8RGain[%d]! Value range:[0, %d]\n", i, ISP_SHARPEN_RGAIN_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8GGain[i] > ISP_SHARPEN_GGAIN_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8GGain[%d]! Value range: [0, %d]\n", i, ISP_SHARPEN_GGAIN_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8BGain[i] > ISP_SHARPEN_BGAIN_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8BGain[%d]! Value range:[0, %d]\n", i, ISP_SHARPEN_BGAIN_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8SkinGain[i] > 31) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8SkinGain[%d]! Value range:[0, 31]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au16MaxSharpGain[i] > 0x7FF) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16MaxSharpGain[%d]! Value range:[0, 0x7FF]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstSharpen->stAuto.au8WeakDetailGain[i] > 0x7F) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8WeakDetailGain[%d]! Value range:[0, 0x7F]\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

    }
    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        if (pstSharpen->stManual.au16TextureStr[j] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16TextureStr[%d]! Value range:[0, 4095]\n", j);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstSharpen->stManual.au16EdgeStr[j] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16EdgeStr[%d]! Value range:[0, 4095]\n", j);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        if (pstSharpen->stManual.au8LumaWgt[j] > ISP_SHARPEN_LUMAWGT) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8LumaWgt[%d]! Value range:[0, %d]\n", j, ISP_SHARPEN_LUMAWGT);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }
    if (pstSharpen->stManual.u16TextureFreq > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "err u16TextureFreq! Value range:[0, 4095]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u16EdgeFreq > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "err u16EdgeFreq! Value range:[0, 4095]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8OverShoot > 127) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8OverShoot! Value range:[0, 127]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8UnderShoot > 127) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8UnderShoot! Value range:[0, 127]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8ShootSupAdj > 15) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8ShootSupAdj! Value range:[0, 15]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8EdgeFiltStr > 63) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8EdgeFiltStr! Value range:[0, 63]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8EdgeFiltMaxCap > ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8EdgeFiltMaxCap! Value range:[0, %d]\n", ISP_SHARPEN_EdgeFiltMaxCap_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8RGain > ISP_SHARPEN_RGAIN_MAX ) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8RGain! Value range:[0, %d]\n", ISP_SHARPEN_RGAIN_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8GGain > ISP_SHARPEN_GGAIN_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid  u8GGain is [0, %d]\n", ISP_SHARPEN_GGAIN_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8BGain > ISP_SHARPEN_BGAIN_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8BGain! Value range:[0, %d]\n", ISP_SHARPEN_BGAIN_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8SkinGain > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8SkinGain! Value range:[0, 31]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u16MaxSharpGain > 0x7FF) {
        ISP_TRACE(MODULE_DBG_ERR, "err u16MaxSharpGain! Value range:[0, 0x7FF]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstSharpen->stManual.u8WeakDetailGain > 0x7F) {
        ISP_TRACE(MODULE_DBG_ERR, "stManual.u8WeakDetailGain ! Value range:[0, 0x7F]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    return GK_SUCCESS;
}

static GK_S32 SharpenExtRegsInitialize(VI_PIPE ViPipe)
{

    GK_S32 s32Ret;
    ISP_CMOS_DEFAULT_S       *pstSnsDft      = GK_NULL;
    const ISP_CMOS_SHARPEN_S *pstCmosSharpen = GK_NULL;
#ifdef VREG_OLD
    GK_U32 u32Addr;
    GK_U8 *pVirtAddr = GK_NULL;
    GK_U8 *pVirtAddrTmp = GK_NULL;
#endif
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);

    ext_system_isp_sharpen_manu_mode_write(ViPipe, OP_TYPE_AUTO);
    ext_system_manual_isp_sharpen_en_write(ViPipe, GK_TRUE);
    ext_system_sharpen_mpi_update_en_write(ViPipe, GK_TRUE);

    // auto ExtRegs initial
    if (pstSnsDft->unKey.bit1Sharpen) {
        ISP_CHECK_POINTER(pstSnsDft->pstSharpen);

        s32Ret = SharpenCheckCmosParam(ViPipe, pstSnsDft->pstSharpen);
        if (s32Ret != GK_SUCCESS) {
            return s32Ret;
        }

        pstCmosSharpen = pstSnsDft->pstSharpen;
#ifndef VREG_OLD
        GK_U16 i, j;
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                ext_system_Isp_sharpen_TextureStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, pstCmosSharpen->stAuto.au16TextureStr[j][i]);
                ext_system_Isp_sharpen_EdgeStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, pstCmosSharpen->stAuto.au16EdgeStr[j][i]);
            }
            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                ext_system_Isp_sharpen_LumaWgt_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, pstCmosSharpen->stAuto.au8LumaWgt[j][i]);
            }
            ext_system_Isp_sharpen_TextureFreq_write    (ViPipe, i, pstCmosSharpen->stAuto.au16TextureFreq[i]);
            ext_system_Isp_sharpen_EdgeFreq_write       (ViPipe, i, pstCmosSharpen->stAuto.au16EdgeFreq[i]);
            ext_system_Isp_sharpen_OverShoot_write      (ViPipe, i, pstCmosSharpen->stAuto.au8OverShoot[i]);
            ext_system_Isp_sharpen_UnderShoot_write     (ViPipe, i, pstCmosSharpen->stAuto.au8UnderShoot[i]);
            ext_system_Isp_sharpen_shootSupStr_write    (ViPipe, i, pstCmosSharpen->stAuto.au8ShootSupStr[i]);
            ext_system_Isp_sharpen_detailctrl_write     (ViPipe, i, pstCmosSharpen->stAuto.au8DetailCtrl[i]);
            ext_system_Isp_sharpen_EdgeFiltStr_write    (ViPipe, i, pstCmosSharpen->stAuto.au8EdgeFiltStr[i]);
            ext_system_Isp_sharpen_EdgeFiltMaxCap_write    (ViPipe, i, pstCmosSharpen->stAuto.au8EdgeFiltMaxCap[i]);
            ext_system_Isp_sharpen_RGain_write          (ViPipe, i, pstCmosSharpen->stAuto.au8RGain[i]);
            ext_system_Isp_sharpen_GGain_write          (ViPipe, i, pstCmosSharpen->stAuto.au8GGain[i]);
            ext_system_Isp_sharpen_BGain_write          (ViPipe, i, pstCmosSharpen->stAuto.au8BGain[i]);
            ext_system_Isp_sharpen_SkinGain_write       (ViPipe, i, pstCmosSharpen->stAuto.au8SkinGain[i]);
            ext_system_Isp_sharpen_ShootSupAdj_write(ViPipe, i, pstCmosSharpen->stAuto.au8ShootSupAdj[i]);
            ext_system_Isp_sharpen_MaxSharpGain_write(ViPipe, i, pstCmosSharpen->stAuto.au16MaxSharpGain[i]);
            ext_system_Isp_sharpen_detailctrlThr_write(ViPipe, i, pstCmosSharpen->stAuto.au8DetailCtrlThr[i]);
            ext_system_Isp_sharpen_WeakDetailGain_write(ViPipe, i, pstCmosSharpen->stAuto.au8WeakDetailGain[i]);
        }

        // manual ExtRegs initial
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            ext_system_manual_Isp_sharpen_TextureStr_write(ViPipe, i, pstCmosSharpen->stManual.au16TextureStr[i]);
            ext_system_manual_Isp_sharpen_EdgeStr_write(ViPipe, i, pstCmosSharpen->stManual.au16EdgeStr[i]);
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            ext_system_manual_Isp_sharpen_LumaWgt_write(ViPipe, i, pstCmosSharpen->stManual.au8LumaWgt[i]);
        }
#else
        u32Addr   = VREG_SHARPEN_BASE(ViPipe);
        pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);
        pVirtAddr    = pVirtAddrTmp + 0xb2;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stAuto.au16TextureStr, (ISP_AUTO_ISO_STRENGTH_NUM * ISP_SHARPEN_GAIN_NUM)*sizeof(GK_U16));

        pVirtAddr    = pVirtAddrTmp + 0x4b2;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stAuto.au16EdgeStr, (ISP_AUTO_ISO_STRENGTH_NUM * ISP_SHARPEN_GAIN_NUM)*sizeof(GK_U16));

        pVirtAddr    = pVirtAddrTmp + 0x972;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8LumaWgt, (ISP_AUTO_ISO_STRENGTH_NUM * ISP_SHARPEN_LUMA_NUM)*sizeof(GK_U8));

        pVirtAddr    = pVirtAddrTmp + 0x8b2;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stAuto.au16TextureFreq, sizeof(GK_U16) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x8d2;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stAuto.au16EdgeFreq, sizeof(GK_U16) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x8f2;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8OverShoot, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x902;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8UnderShoot, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x912;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8ShootSupStr, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x922;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8DetailCtrl, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x962;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8EdgeFiltStr, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x932;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8RGain, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x8ec;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8GGain, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x942;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8BGain, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));


        pVirtAddr    = pVirtAddrTmp + 0x952;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8SkinGain, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0xb81;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8ShootSupAdj, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0xb82;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stAuto.au16MaxSharpGain, sizeof(GK_U16) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0xb80;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8DetailCtrlThr, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));
#endif

        ext_system_manual_Isp_sharpen_TextureFreq_write   (ViPipe, pstCmosSharpen->stManual.u16TextureFreq);
        ext_system_manual_Isp_sharpen_EdgeFreq_write      (ViPipe, pstCmosSharpen->stManual.u16EdgeFreq);
        ext_system_manual_Isp_sharpen_OverShoot_write     (ViPipe, pstCmosSharpen->stManual.u8OverShoot);
        ext_system_manual_Isp_sharpen_UnderShoot_write    (ViPipe, pstCmosSharpen->stManual.u8UnderShoot);
        ext_system_manual_Isp_sharpen_shootSupStr_write   (ViPipe, pstCmosSharpen->stManual.u8ShootSupStr);
        ext_system_manual_Isp_sharpen_detailctrl_write    (ViPipe, pstCmosSharpen->stManual.u8DetailCtrl);
        ext_system_manual_Isp_sharpen_EdgeFiltStr_write   (ViPipe, pstCmosSharpen->stManual.u8EdgeFiltStr);
        ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(ViPipe, pstCmosSharpen->stManual.u8EdgeFiltMaxCap);
        ext_system_manual_Isp_sharpen_RGain_write         (ViPipe, pstCmosSharpen->stManual.u8RGain);
        ext_system_manual_Isp_sharpen_GGain_write         (ViPipe, pstCmosSharpen->stManual.u8GGain);
        ext_system_manual_Isp_sharpen_BGain_write         (ViPipe, pstCmosSharpen->stManual.u8BGain);
        ext_system_manual_Isp_sharpen_SkinGain_write      (ViPipe, pstCmosSharpen->stManual.u8SkinGain);
        ext_system_manual_Isp_sharpen_ShootSupAdj_write      (ViPipe, pstCmosSharpen->stManual.u8ShootSupAdj);
        ext_system_manual_Isp_sharpen_MaxSharpGain_write      (ViPipe, pstCmosSharpen->stManual.u16MaxSharpGain);
        ext_system_manual_Isp_sharpen_detailctrlThr_write      (ViPipe, pstCmosSharpen->stManual.u8DetailCtrlThr);
        ext_system_manual_Isp_sharpen_WeakDetailGain_write(ViPipe, pstCmosSharpen->stManual.u8WeakDetailGain);
        ext_system_manual_Isp_sharpen_SkinUmax_write(ViPipe, pstCmosSharpen->u8SkinUmax);
        ext_system_manual_Isp_sharpen_SkinUmin_write(ViPipe, pstCmosSharpen->u8SkinUmin);
        ext_system_manual_Isp_sharpen_SkinVmax_write(ViPipe, pstCmosSharpen->u8SkinVmax);
        ext_system_manual_Isp_sharpen_SkinVmin_write(ViPipe, pstCmosSharpen->u8SkinVmin);

        ext_system_actual_sharpen_overshootAmt_write     (ViPipe, pstCmosSharpen->stManual.u8OverShoot);
        ext_system_actual_sharpen_undershootAmt_write    (ViPipe, pstCmosSharpen->stManual.u8UnderShoot);
        ext_system_actual_sharpen_shootSupSt_write       (ViPipe, pstCmosSharpen->stManual.u8ShootSupStr);
        ext_system_actual_sharpen_edge_frequence_write   (ViPipe, pstCmosSharpen->stManual.u16EdgeFreq);
        ext_system_actual_sharpen_texture_frequence_write(ViPipe, pstCmosSharpen->stManual.u16TextureFreq);
#ifndef VREG_OLD
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            ext_system_actual_sharpen_edge_str_write     (ViPipe, i, pstCmosSharpen->stManual.au16EdgeStr[i]);
            ext_system_actual_sharpen_texture_str_write  (ViPipe, i, pstCmosSharpen->stManual.au16TextureStr[i]);
        }
#else
        u32Addr  =   VREG_TOP_BASE(ViPipe);
        pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);

        pVirtAddr    = pVirtAddrTmp + 0x120;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stManual.au16EdgeStr, sizeof(GK_U16) * (ISP_SHARPEN_GAIN_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x160;
        memcpy((GK_U16 *)pVirtAddr, pstCmosSharpen->stManual.au16TextureStr, sizeof(GK_U16) * (ISP_SHARPEN_GAIN_NUM));
#endif
    } else {
        // auto ExtRegs initial
#ifndef VREG_OLD
        GK_U16 i, j;
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                ext_system_Isp_sharpen_TextureStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT);
                ext_system_Isp_sharpen_EdgeStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT);
            }
            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                ext_system_Isp_sharpen_LumaWgt_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT);
            }
            ext_system_Isp_sharpen_TextureFreq_write    (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT);
            ext_system_Isp_sharpen_EdgeFreq_write       (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT);
            ext_system_Isp_sharpen_OverShoot_write      (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT);
            ext_system_Isp_sharpen_UnderShoot_write     (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT);
            ext_system_Isp_sharpen_shootSupStr_write    (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT);
            ext_system_Isp_sharpen_detailctrl_write     (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT);
            ext_system_Isp_sharpen_EdgeFiltStr_write    (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT);
            ext_system_Isp_sharpen_EdgeFiltMaxCap_write (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT);
            ext_system_Isp_sharpen_RGain_write          (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT);
            ext_system_Isp_sharpen_GGain_write          (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT);
            ext_system_Isp_sharpen_BGain_write          (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT);
            ext_system_Isp_sharpen_SkinGain_write       (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT);
            ext_system_Isp_sharpen_ShootSupAdj_write       (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT);
            ext_system_Isp_sharpen_MaxSharpGain_write       (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT);
            ext_system_Isp_sharpen_detailctrlThr_write       (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT);
            ext_system_Isp_sharpen_WeakDetailGain_write       (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_WEAKDETAILGAIN_DEFAULT);
        }
        // manual ExtRegs initial
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            ext_system_manual_Isp_sharpen_TextureStr_write(ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT);
            ext_system_manual_Isp_sharpen_EdgeStr_write(ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT);
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            ext_system_manual_Isp_sharpen_LumaWgt_write(ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT);
        }
#else
        u32Addr   = VREG_SHARPEN_BASE(ViPipe);
        pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);
        pVirtAddr    = pVirtAddrTmp + 0xb2;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT, ISP_AUTO_ISO_STRENGTH_NUM * ISP_SHARPEN_GAIN_NUM);

        pVirtAddr    = pVirtAddrTmp + 0x4b2;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT, ISP_AUTO_ISO_STRENGTH_NUM * ISP_SHARPEN_GAIN_NUM);

        pVirtAddr    = pVirtAddrTmp + 0x972;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT, ISP_AUTO_ISO_STRENGTH_NUM * ISP_SHARPEN_LUMA_NUM);

        pVirtAddr    = pVirtAddrTmp + 0x8b2;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT, ISP_AUTO_ISO_STRENGTH_NUM);

        pVirtAddr    = pVirtAddrTmp + 0x8d2;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT, ISP_AUTO_ISO_STRENGTH_NUM);

        pVirtAddr    = pVirtAddrTmp + 0x8f2;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x902;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x912;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x922;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x962;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x932;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x8ec;
        memcpy((GK_U8 *)pVirtAddr, pstCmosSharpen->stAuto.au8GGain, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x942;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0x952;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0xb81;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));

        pVirtAddr    = pVirtAddrTmp + 0xb82;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT, ISP_AUTO_ISO_STRENGTH_NUM);

        pVirtAddr    = pVirtAddrTmp + 0xb80;
        memset((GK_U8 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT, sizeof(GK_U8) * (ISP_AUTO_ISO_STRENGTH_NUM));
#endif

        ext_system_manual_Isp_sharpen_TextureFreq_write (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT);
        ext_system_manual_Isp_sharpen_EdgeFreq_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT);
        ext_system_manual_Isp_sharpen_OverShoot_write   (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT);
        ext_system_manual_Isp_sharpen_UnderShoot_write  (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT);
        ext_system_manual_Isp_sharpen_shootSupStr_write (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT);
        ext_system_manual_Isp_sharpen_detailctrl_write  (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT);
        ext_system_manual_Isp_sharpen_EdgeFiltStr_write (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT);
        ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT);
        ext_system_manual_Isp_sharpen_RGain_write       (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT);
        ext_system_manual_Isp_sharpen_GGain_write       (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT);
        ext_system_manual_Isp_sharpen_BGain_write       (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT);
        ext_system_manual_Isp_sharpen_SkinGain_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT);
        ext_system_manual_Isp_sharpen_ShootSupAdj_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT);
        ext_system_manual_Isp_sharpen_MaxSharpGain_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT);
        ext_system_manual_Isp_sharpen_detailctrlThr_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT);
        ext_system_manual_Isp_sharpen_WeakDetailGain_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_WEAKDETAILGAIN_DEFAULT);
        ext_system_manual_Isp_sharpen_SkinUmax_write(ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMAX_DEFAULT);
        ext_system_manual_Isp_sharpen_SkinUmin_write(ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMIN_DEFAULT);
        ext_system_manual_Isp_sharpen_SkinVmax_write(ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMAX_DEFAULT);
        ext_system_manual_Isp_sharpen_SkinVmin_write(ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMIN_DEFAULT);

        ext_system_actual_sharpen_overshootAmt_write      (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT);
        ext_system_actual_sharpen_undershootAmt_write     (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT);
        ext_system_actual_sharpen_shootSupSt_write        (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT);
        ext_system_actual_sharpen_edge_frequence_write    (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT);
        ext_system_actual_sharpen_texture_frequence_write (ViPipe, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT);
#ifndef VREG_OLD
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            ext_system_actual_sharpen_edge_str_write      (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT);
            ext_system_actual_sharpen_texture_str_write   (ViPipe, i, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT);
        }
#else
        u32Addr  = VREG_TOP_BASE(ViPipe);
        pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);

        pVirtAddr    = pVirtAddrTmp + 0x120;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT, ISP_SHARPEN_GAIN_NUM);

        pVirtAddr    = pVirtAddrTmp + 0x160;
        MemsetU16((GK_U16 *)pVirtAddr, ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT, ISP_SHARPEN_GAIN_NUM);
#endif
    }

    return GK_SUCCESS;
}

//****Sharpen hardware Regs that will not change****//
static GK_VOID SharpenStaticRegInit(VI_PIPE ViPipe, ISP_SHARPEN_STATIC_REG_CFG_S *pstStaticRegCfg)
{
    GK_U8 i;

    pstStaticRegCfg->u8ditherMode    = 2;

    pstStaticRegCfg->bStaticResh     = GK_TRUE;
    pstStaticRegCfg->u8gainThdSelD   = 1;
    pstStaticRegCfg->u8dirVarScale   = 0;
    pstStaticRegCfg->u8dirRly[0]     = 127;
    pstStaticRegCfg->u8dirRly[1]     = 0;
    pstStaticRegCfg->u8MaxVarClipMin = 3;
    pstStaticRegCfg->u16oMaxChg      = 1000;
    pstStaticRegCfg->u16uMaxChg      = 1000;
    pstStaticRegCfg->u8shtVarSft     = 0;

    for (i = 0; i < ISP_SHARPEN_FREQ_CORING_LENGTH; i++) {
        pstStaticRegCfg->u8lmtMF[i]     = 0;
        pstStaticRegCfg->u8lmtHF[i]     = 0;
    }

    pstStaticRegCfg->u8skinSrcSel       = 0;
    pstStaticRegCfg->u8skinCntThd[0]    = 5;
    pstStaticRegCfg->u8skinEdgeThd[0]   = 10;
    pstStaticRegCfg->u8skinCntThd[1]    = 8;
    pstStaticRegCfg->u8skinEdgeThd[1]   = 30;
    pstStaticRegCfg->u8skinEdgeSft      = 1;

    // Chr
    pstStaticRegCfg->u8chrRVarSft    = 7;
    pstStaticRegCfg->u8chrROriCb     = 120;
    pstStaticRegCfg->u8chrROriCr     = 220;
    pstStaticRegCfg->u8chrRSft[0]    = 7;
    pstStaticRegCfg->u8chrRSft[1]    = 7;
    pstStaticRegCfg->u8chrRSft[2]    = 7;
    pstStaticRegCfg->u8chrRSft[3]    = 6;
    pstStaticRegCfg->u8chrRThd[0]    = 40;
    pstStaticRegCfg->u8chrRThd[1]    = 60;

    pstStaticRegCfg->u8chrBVarSft    = 2;
    pstStaticRegCfg->u8chrBOriCb     = 200;
    pstStaticRegCfg->u8chrBOriCr     = 64;
    pstStaticRegCfg->u8chrBSft[0]    = 7;
    pstStaticRegCfg->u8chrBSft[1]    = 7;
    pstStaticRegCfg->u8chrBSft[2]    = 7;
    pstStaticRegCfg->u8chrBSft[3]    = 7;
    pstStaticRegCfg->u8chrBThd[0]    = 50;
    pstStaticRegCfg->u8chrBThd[1]    = 100;

    pstStaticRegCfg->u8chrGOriCb     = 90;
    pstStaticRegCfg->u8chrGOriCr     = 110;
    pstStaticRegCfg->u8chrGSft[0]    = 4;
    pstStaticRegCfg->u8chrGSft[1]    = 7;
    pstStaticRegCfg->u8chrGSft[2]    = 4;
    pstStaticRegCfg->u8chrGSft[3]    = 7;
    pstStaticRegCfg->u8chrGThd[0]    = 20;
    pstStaticRegCfg->u8chrGThd[1]    = 40;

    pstStaticRegCfg->u8shtVarWgt1      = 127;
    pstStaticRegCfg->u8shtVarDiffWgt0  = 127;
    pstStaticRegCfg->u8shtVarThd0      = 0;
    //pstStaticRegCfg->u8dirRt[0]        = 6;
    //pstStaticRegCfg->u8dirRt[1]        = 18;

    // filter
    pstStaticRegCfg->s8lpfCoefUD[0]   = 5;
    pstStaticRegCfg->s8lpfCoefUD[1]   = 7;
    pstStaticRegCfg->s8lpfCoefUD[2]   = 8;

    pstStaticRegCfg->s8lpfCoefD[0]    = 4;
    pstStaticRegCfg->s8lpfCoefD[1]    = 7;
    pstStaticRegCfg->s8lpfCoefD[2]    = 10;

    pstStaticRegCfg->s8hsfCoefUD[0]   = -2;
    pstStaticRegCfg->s8hsfCoefUD[1]   = 9;
    pstStaticRegCfg->s8hsfCoefUD[2]   = 18;

    pstStaticRegCfg->s8hsfCoefD0[0]   = -1;
    pstStaticRegCfg->s8hsfCoefD0[1]   = -2;
    pstStaticRegCfg->s8hsfCoefD0[2]   = -3;
    pstStaticRegCfg->s8hsfCoefD0[3]   = -2;
    pstStaticRegCfg->s8hsfCoefD0[4]   = -1;
    pstStaticRegCfg->s8hsfCoefD0[5]   = 12;
    pstStaticRegCfg->s8hsfCoefD0[6]   = 27;
    pstStaticRegCfg->s8hsfCoefD0[7]   = 36;
    pstStaticRegCfg->s8hsfCoefD0[8]   = 27;
    pstStaticRegCfg->s8hsfCoefD0[9]   = 12;
    pstStaticRegCfg->s8hsfCoefD0[10]  = 31;
    pstStaticRegCfg->s8hsfCoefD0[11]  = 72;
    pstStaticRegCfg->s8hsfCoefD0[12]  = 96;

    pstStaticRegCfg->s8hsfCoefD1[0]   = -2;
    pstStaticRegCfg->s8hsfCoefD1[1]   = -3;
    pstStaticRegCfg->s8hsfCoefD1[2]   = -1;
    pstStaticRegCfg->s8hsfCoefD1[3]   = 6;
    pstStaticRegCfg->s8hsfCoefD1[4]   = 6;
    pstStaticRegCfg->s8hsfCoefD1[5]   = 0;
    pstStaticRegCfg->s8hsfCoefD1[6]   = 13;
    pstStaticRegCfg->s8hsfCoefD1[7]   = 41;
    pstStaticRegCfg->s8hsfCoefD1[8]   = 47;
    pstStaticRegCfg->s8hsfCoefD1[9]   = 23;
    pstStaticRegCfg->s8hsfCoefD1[10]  = 18;
    pstStaticRegCfg->s8hsfCoefD1[11]  = 62;
    pstStaticRegCfg->s8hsfCoefD1[12]  = 92;

    pstStaticRegCfg->s8hsfCoefD2[0]   = -1;
    pstStaticRegCfg->s8hsfCoefD2[1]   = -3;
    pstStaticRegCfg->s8hsfCoefD2[2]   = 6;
    pstStaticRegCfg->s8hsfCoefD2[3]   = 16;
    pstStaticRegCfg->s8hsfCoefD2[4]   = 10;
    pstStaticRegCfg->s8hsfCoefD2[5]   = -3;
    pstStaticRegCfg->s8hsfCoefD2[6]   = 10;
    pstStaticRegCfg->s8hsfCoefD2[7]   = 50;
    pstStaticRegCfg->s8hsfCoefD2[8]   = 53;
    pstStaticRegCfg->s8hsfCoefD2[9]   = 16;
    pstStaticRegCfg->s8hsfCoefD2[10]  = 6;
    pstStaticRegCfg->s8hsfCoefD2[11]  = 50;
    pstStaticRegCfg->s8hsfCoefD2[12]  = 92;

    pstStaticRegCfg->s8hsfCoefD3[0]   = -2;
    pstStaticRegCfg->s8hsfCoefD3[1]   = 0;
    pstStaticRegCfg->s8hsfCoefD3[2]   = 18;
    pstStaticRegCfg->s8hsfCoefD3[3]   = 23;
    pstStaticRegCfg->s8hsfCoefD3[4]   = 6;
    pstStaticRegCfg->s8hsfCoefD3[5]   = -3;
    pstStaticRegCfg->s8hsfCoefD3[6]   = 13;
    pstStaticRegCfg->s8hsfCoefD3[7]   = 62;
    pstStaticRegCfg->s8hsfCoefD3[8]   = 47;
    pstStaticRegCfg->s8hsfCoefD3[9]   = 6;
    pstStaticRegCfg->s8hsfCoefD3[10]  = -1;
    pstStaticRegCfg->s8hsfCoefD3[11]  = 41;
    pstStaticRegCfg->s8hsfCoefD3[12]  = 92;

    pstStaticRegCfg->u8lpfSftUD        = 5;
    pstStaticRegCfg->u8lpfSftD         = 5;
    pstStaticRegCfg->u8hsfSftUD        = 5;
    pstStaticRegCfg->u8hsfSftD         = 9;

    pstStaticRegCfg->bEnShp8Dir        = 1;

    pstStaticRegCfg->u8lfGainWgt       = 14;
    pstStaticRegCfg->u8hfGainSft       = 5;
    pstStaticRegCfg->u8mfGainSft       = 5;
    pstStaticRegCfg->u8shtVarSel       = 1;
    pstStaticRegCfg->u8shtVar5x5Sft    = 1;
    pstStaticRegCfg->u8detailThdSel    = 0;
    pstStaticRegCfg->u8detailThdSft    = 1;

    // Skin detection
    pstStaticRegCfg->u8skinCntMul    = CalcMulCoef(pstStaticRegCfg->u8skinCntThd[0], 0, pstStaticRegCfg->u8skinCntThd[1], 31, 0);
}

// ****Sharpen hardware Regs that will change with MPI and ISO****//
static GK_VOID SharpenMpiDynaRegInit(ISP_SHARPEN_MPI_DYNA_REG_CFG_S *pstMpiDynaRegCfg)
{

    GK_U8 i;
    for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
        pstMpiDynaRegCfg->u16mfGainD[i]  = 300;
        pstMpiDynaRegCfg->u16mfGainUD[i] = 200;
        pstMpiDynaRegCfg->u16hfGainD[i]  = 450;
        pstMpiDynaRegCfg->u16hfGainUD[i] = 400;
    }

    pstMpiDynaRegCfg->u8oshtAmt          = 100;
    pstMpiDynaRegCfg->u8ushtAmt          = 127;
    pstMpiDynaRegCfg->u8bEnShtCtrlByVar  = 1;
    pstMpiDynaRegCfg->u8shtBldRt         = 9;
    pstMpiDynaRegCfg->u8shtVarThd1       = 5;

    pstMpiDynaRegCfg->bEnWeakDetailAdj   = 0;
    pstMpiDynaRegCfg->u8WeakDetailGain   = 2;

    pstMpiDynaRegCfg->u8bEnChrCtrl       = 1;
    pstMpiDynaRegCfg->u8chrRGain         = 6;
    pstMpiDynaRegCfg->u16chrGGain        = 32;
    pstMpiDynaRegCfg->u16chrGmfGain      = 32;
    pstMpiDynaRegCfg->u8chrBGain         = 14;
    pstMpiDynaRegCfg->u8bEnSkinCtrl      = 0;
    pstMpiDynaRegCfg->u8skinEdgeWgt[1]   = 31;
    pstMpiDynaRegCfg->u8skinEdgeWgt[0]   = 31;

    pstMpiDynaRegCfg->u8bEnLumaCtrl      = 0;
    pstMpiDynaRegCfg->bEnDetailCtrl      = 0;
    pstMpiDynaRegCfg->u8detailOshtAmt    = 100;
    pstMpiDynaRegCfg->u8detailUshtAmt    = 127;
    pstMpiDynaRegCfg->u8dirDiffSft       = 10;
    pstMpiDynaRegCfg->u8dirRt[0]         = 6;
    pstMpiDynaRegCfg->u8dirRt[1]         = 18;
    pstMpiDynaRegCfg->u8skinMaxU         = 127;
    pstMpiDynaRegCfg->u8skinMinU         = 95;
    pstMpiDynaRegCfg->u8skinMaxV         = 155;
    pstMpiDynaRegCfg->u8skinMinV         = 135;
    pstMpiDynaRegCfg->u16oMaxGain        = 160;
    pstMpiDynaRegCfg->u16uMaxGain        = 160;
    pstMpiDynaRegCfg->u8detailOshtThr[0] = 65;
    pstMpiDynaRegCfg->u8detailOshtThr[1] = 90;
    pstMpiDynaRegCfg->u8detailUshtThr[0] = 65;
    pstMpiDynaRegCfg->u8detailUshtThr[1] = 90;
#ifndef VREG_OLD
    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        pstMpiDynaRegCfg->au8LumaWgt[i]  = 31;
    }
#else
    memset(pstMpiDynaRegCfg->au8LumaWgt, 31, sizeof(GK_U8)*SHRP_GAIN_LUT_SIZE);
#endif
    pstMpiDynaRegCfg->u32UpdateIndex     = 1;
    pstMpiDynaRegCfg->u8BufId            = 0;
    pstMpiDynaRegCfg->bResh              = GK_TRUE;
    pstMpiDynaRegCfg->bPreRegNewEn       = GK_FALSE;
}

// ****Sharpen hardware Regs that will change only with ISO****//
static GK_VOID SharpenDefaultDynaRegInit(ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstDefaultDynaRegCfg)
{
    pstDefaultDynaRegCfg->bResh               = GK_TRUE;
    pstDefaultDynaRegCfg->u8gainThdSftD       = 0;
    pstDefaultDynaRegCfg->u8gainThdSelUD      = 2;
    pstDefaultDynaRegCfg->u8gainThdSftUD        = 0;
    pstDefaultDynaRegCfg->u8dirVarSft         = 10;
    pstDefaultDynaRegCfg->u8selPixWgt         = 31;
    pstDefaultDynaRegCfg->u8shtVarDiffThd[0]  = 27;
    pstDefaultDynaRegCfg->u8shtVarWgt0        = 0;
    pstDefaultDynaRegCfg->u8shtVarDiffThd[1]  = 50;
    pstDefaultDynaRegCfg->u8shtVarDiffWgt1    = 5;
    pstDefaultDynaRegCfg->u8RmfGainScale      = 24;
    pstDefaultDynaRegCfg->u8BmfGainScale      = 10;
    pstDefaultDynaRegCfg->u8dirRlyThrLow      = 2;
    pstDefaultDynaRegCfg->u8dirRlyThrhigh     = 120;
}

static GK_VOID SharpenDynaRegInit(ISP_SHARPEN_REG_CFG_S *pstSharpenRegCfg)
{
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstDefaultDynaRegCfg = GK_NULL;
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S     *pstMpiDynaRegCfg = GK_NULL;
    ISP_SHARPEN_STATIC_REG_CFG_S       *pstStaticRegCfg = GK_NULL;

    pstDefaultDynaRegCfg = &(pstSharpenRegCfg->stDynaRegCfg.stDefaultDynaRegCfg);
    pstMpiDynaRegCfg     = &pstSharpenRegCfg->stDynaRegCfg.stMpiDynaRegCfg;
    pstStaticRegCfg      = &pstSharpenRegCfg->stStaticRegCfg;

    SharpenDefaultDynaRegInit(pstDefaultDynaRegCfg);
    SharpenMpiDynaRegInit(pstMpiDynaRegCfg);

    /* Calc all MulCoef */
    // Mpi
    pstMpiDynaRegCfg->u16shtVarMul  = CalcMulCoef(pstStaticRegCfg->u8shtVarThd0,  pstDefaultDynaRegCfg->u8shtVarWgt0,
                                                  pstMpiDynaRegCfg->u8shtVarThd1, pstStaticRegCfg->u8shtVarWgt1,
                                                  SHRP_SHT_VAR_MUL_PRECS);

    pstMpiDynaRegCfg->s16chrRMul     = CalcMulCoef(pstStaticRegCfg->u8chrRThd[0], pstMpiDynaRegCfg->u8chrRGain,
                                                   pstStaticRegCfg->u8chrRThd[1], 32,
                                                   SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16chrGMul     = CalcMulCoef(pstStaticRegCfg->u8chrGThd[0], pstMpiDynaRegCfg->u16chrGGain,
                                                   pstStaticRegCfg->u8chrGThd[1], 32,
                                                   SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16chrGmfMul   = CalcMulCoef(pstStaticRegCfg->u8chrGThd[0], pstMpiDynaRegCfg->u16chrGmfGain,
                                                   pstStaticRegCfg->u8chrGThd[1], 32,
                                                   SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16chrBMul     = CalcMulCoef(pstStaticRegCfg->u8chrBThd[0], pstMpiDynaRegCfg->u8chrBGain,
                                                   pstStaticRegCfg->u8chrBThd[1], 32,
                                                   SHRP_CHR_MUL_SFT);

    pstMpiDynaRegCfg->s16skinEdgeMul = CalcMulCoef(pstStaticRegCfg->u8skinEdgeThd[0], pstMpiDynaRegCfg->u8skinEdgeWgt[0],
                                                   pstStaticRegCfg->u8skinEdgeThd[1], pstMpiDynaRegCfg->u8skinEdgeWgt[1],
                                                   SHRP_SKIN_EDGE_MUL_PRECS);

    pstMpiDynaRegCfg->s16detailOshtMul = CalcMulCoef(pstMpiDynaRegCfg->u8detailOshtThr[0], pstMpiDynaRegCfg->u8detailOshtAmt,
                                                     pstMpiDynaRegCfg->u8detailOshtThr[1], pstMpiDynaRegCfg->u8oshtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);

    pstMpiDynaRegCfg->s16detailUshtMul = CalcMulCoef(pstMpiDynaRegCfg->u8detailUshtThr[0], pstMpiDynaRegCfg->u8detailUshtAmt,
                                                     pstMpiDynaRegCfg->u8detailUshtThr[1], pstMpiDynaRegCfg->u8ushtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);
    // Defalut
    pstDefaultDynaRegCfg->s16shtVarDiffMul = CalcMulCoef(pstDefaultDynaRegCfg->u8shtVarDiffThd[0], pstStaticRegCfg->u8shtVarDiffWgt0,
                                                         pstDefaultDynaRegCfg->u8shtVarDiffThd[1], pstDefaultDynaRegCfg->u8shtVarDiffWgt1,
                                                         SHRP_SHT_VAR_MUL_PRECS);
}

static GK_VOID SharpenRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pRegCfg)
{
    GK_U32 i;

    for (i = 0; i < pRegCfg->u8CfgNum; i++) {
        pRegCfg->stAlgRegCfg[i].stSharpenRegCfg.bEnable    = GK_TRUE;
        pRegCfg->stAlgRegCfg[i].stSharpenRegCfg.bLut2SttEn = GK_TRUE;
        SharpenStaticRegInit(ViPipe, &(pRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stStaticRegCfg));
        SharpenDynaRegInit(&(pRegCfg->stAlgRegCfg[i].stSharpenRegCfg));
    }

    pRegCfg->unKey.bit1SharpenCfg = 1;
}


static GK_VOID SharpenReadExtregs(VI_PIPE ViPipe)
{
    GK_U8 i, j;
    ISP_SHARPEN_S *pstSharpen = GK_NULL;
    SHARPEN_GET_CTX(ViPipe, pstSharpen);

    pstSharpen->bSharpenMpiUpdateEn = ext_system_sharpen_mpi_update_en_read(ViPipe);
    pstSharpen->u8SkinUmax   = ext_system_manual_Isp_sharpen_SkinUmax_read(ViPipe);
    pstSharpen->u8SkinUmin   = ext_system_manual_Isp_sharpen_SkinUmin_read(ViPipe);
    pstSharpen->u8SkinVmax   = ext_system_manual_Isp_sharpen_SkinVmax_read(ViPipe);
    pstSharpen->u8SkinVmin   = ext_system_manual_Isp_sharpen_SkinVmin_read(ViPipe);

    ext_system_sharpen_mpi_update_en_write(ViPipe, GK_FALSE);

    if (pstSharpen->bSharpenMpiUpdateEn) {
        pstSharpen->u8ManualSharpenYuvEnabled = ext_system_isp_sharpen_manu_mode_read(ViPipe);

        if (pstSharpen->u8ManualSharpenYuvEnabled == OP_TYPE_MANUAL) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                pstSharpen->au16TextureStr[j] = ext_system_manual_Isp_sharpen_TextureStr_read(ViPipe, j);
                pstSharpen->au16EdgeStr[j]    = ext_system_manual_Isp_sharpen_EdgeStr_read(ViPipe, j);
            }
            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                pstSharpen->au8LumaWgt[j]     = ext_system_manual_Isp_sharpen_LumaWgt_read(ViPipe, j);
            }
            pstSharpen->u16TextureFreq    = ext_system_manual_Isp_sharpen_TextureFreq_read(ViPipe);
            pstSharpen->u16EdgeFreq       = ext_system_manual_Isp_sharpen_EdgeFreq_read(ViPipe);
            pstSharpen->u8OverShoot       = ext_system_manual_Isp_sharpen_OverShoot_read(ViPipe);
            pstSharpen->u8UnderShoot      = ext_system_manual_Isp_sharpen_UnderShoot_read(ViPipe);
            pstSharpen->u8ShootSupStr     = ext_system_manual_Isp_sharpen_shootSupStr_read(ViPipe);
            pstSharpen->u8DetailCtrl      = ext_system_manual_Isp_sharpen_detailctrl_read(ViPipe);
            pstSharpen->u8EdgeFiltStr     = ext_system_manual_Isp_sharpen_EdgeFiltStr_read(ViPipe);
            pstSharpen->u8EdgeFiltMaxCap  = ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_read(ViPipe);
            pstSharpen->u8RGain           = ext_system_manual_Isp_sharpen_RGain_read(ViPipe);
            pstSharpen->u8GGain           = ext_system_manual_Isp_sharpen_GGain_read(ViPipe);
            pstSharpen->u8BGain           = ext_system_manual_Isp_sharpen_BGain_read(ViPipe);
            pstSharpen->u8SkinGain        = ext_system_manual_Isp_sharpen_SkinGain_read(ViPipe);
            pstSharpen->u8ShootSupAdj     = ext_system_manual_Isp_sharpen_ShootSupAdj_read(ViPipe);
            pstSharpen->u8DetailCtrlThr   = ext_system_manual_Isp_sharpen_detailctrlThr_read(ViPipe);
            pstSharpen->u16MaxSharpGain   = ext_system_manual_Isp_sharpen_MaxSharpGain_read(ViPipe);
            pstSharpen->u8WeakDetailGain  = ext_system_manual_Isp_sharpen_WeakDetailGain_read(ViPipe);
        } else {
            for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
                for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                    pstSharpen->au16AutoTextureStr[j][i] = ext_system_Isp_sharpen_TextureStr_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
                    pstSharpen->au16AutoEdgeStr[j][i]    = ext_system_Isp_sharpen_EdgeStr_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
                }
                for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                    pstSharpen->au8AutoLumaWgt[j][i]     = ext_system_Isp_sharpen_LumaWgt_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
                }
                pstSharpen->au16TextureFreq[i]   = ext_system_Isp_sharpen_TextureFreq_read(ViPipe, i);
                pstSharpen->au16EdgeFreq[i]      = ext_system_Isp_sharpen_EdgeFreq_read(ViPipe, i);
                pstSharpen->au8OverShoot[i]      = ext_system_Isp_sharpen_OverShoot_read(ViPipe, i);
                pstSharpen->au8UnderShoot[i]     = ext_system_Isp_sharpen_UnderShoot_read(ViPipe, i);
                pstSharpen->au8ShootSupStr[i]    = ext_system_Isp_sharpen_shootSupStr_read(ViPipe, i);
                pstSharpen->au8DetailCtrl[i]     = ext_system_Isp_sharpen_detailctrl_read(ViPipe, i);
                pstSharpen->au8EdgeFiltStr[i]    = ext_system_Isp_sharpen_EdgeFiltStr_read(ViPipe, i);
                pstSharpen->au8EdgeFiltMaxCap[i] = ext_system_Isp_sharpen_EdgeFiltMaxCap_read(ViPipe, i);
                pstSharpen->au8RGain[i]          = ext_system_Isp_sharpen_RGain_read(ViPipe, i);
                pstSharpen->au8GGain[i]          = ext_system_Isp_sharpen_GGain_read(ViPipe, i);
                pstSharpen->au8BGain[i]          = ext_system_Isp_sharpen_BGain_read(ViPipe, i);
                pstSharpen->au8SkinGain[i]       = ext_system_Isp_sharpen_SkinGain_read(ViPipe, i);
                pstSharpen->au8ShootSupAdj[i]    = ext_system_Isp_sharpen_ShootSupAdj_read(ViPipe, i);
                pstSharpen->au16MaxSharpGain[i]  = ext_system_Isp_sharpen_MaxSharpGain_read(ViPipe, i);
                pstSharpen->au8DetailCtrlThr[i]  = ext_system_Isp_sharpen_detailctrlThr_read(ViPipe, i);
                pstSharpen->au8WeakDetailGain[i] = ext_system_Isp_sharpen_WeakDetailGain_read(ViPipe, i);
            }
        }
    }
}

static GK_S32 SharpenReadProMode(VI_PIPE ViPipe)
{
    GK_U8 i, j;
    ISP_CTX_S     *pstIspCtx  = GK_NULL;
    ISP_SHARPEN_S *pstSharpen = GK_NULL;
    GK_U8 u8Index = 0;
    GK_U8 u8IndexMaxValue = 0;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    if (pstIspCtx->stProShpParamCtrl.pstProShpParam->bEnable == GK_TRUE) {
        u8Index = pstIspCtx->stLinkage.u8ProIndex;
        u8IndexMaxValue = MIN2(pstIspCtx->stProShpParamCtrl.pstProShpParam->u32ParamNum, PRO_MAX_FRAME_NUM);
        if (u8Index > u8IndexMaxValue) {
            u8Index =  u8IndexMaxValue;
        }
        if (u8Index < 1) {
            return GK_SUCCESS;
        }
        u8Index -= 1;
    } else {
        return GK_SUCCESS;
    }
    pstSharpen->u8ManualSharpenYuvEnabled = OP_TYPE_AUTO;
    pstSharpen->bSharpenMpiUpdateEn = GK_TRUE;
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            pstSharpen->au16AutoTextureStr[j][i] = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au16TextureStr[j][i];
            pstSharpen->au16AutoEdgeStr[j][i]    = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au16EdgeStr[j][i];
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            pstSharpen->au8AutoLumaWgt[j][i]     = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8LumaWgt[j][i];
        }
        pstSharpen->au16TextureFreq[i]  = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au16TextureFreq[i];
        pstSharpen->au16EdgeFreq[i]     = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au16EdgeFreq[i];
        pstSharpen->au8OverShoot[i]     = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8OverShoot[i];
        pstSharpen->au8UnderShoot[i]    = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8UnderShoot[i];
        pstSharpen->au8ShootSupStr[i]   = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8ShootSupStr[i];
        pstSharpen->au8DetailCtrl[i]    = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8DetailCtrl[i];
        pstSharpen->au8EdgeFiltStr[i]   = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8EdgeFiltStr[i];
        pstSharpen->au8EdgeFiltMaxCap[i] = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8EdgeFiltMaxCap[i];
        pstSharpen->au8RGain[i]         = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8RGain[i];
        pstSharpen->au8GGain[i]         = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8GGain[i];
        pstSharpen->au8BGain[i]         = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8BGain[i];
        pstSharpen->au8SkinGain[i]      = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8SkinGain[i];
        pstSharpen->au8ShootSupAdj[i]      = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8ShootSupAdj[i];
        pstSharpen->au16MaxSharpGain[i]      = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au16MaxSharpGain[i];
        pstSharpen->au8DetailCtrlThr[i]      = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8DetailCtrlThr[i];
        pstSharpen->au8WeakDetailGain[i] = pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[u8Index].au8WeakDetailGain[i];
    }
    return GK_SUCCESS;
}
static GK_S32 ISP_SharpenCtxInit(ISP_SHARPEN_S *pstSharpen)
{
    GK_U32 i, j;
    pstSharpen->bSharpenEn          = 1;
    pstSharpen->bSharpenMpiUpdateEn = 1;
    pstSharpen->u32IsoLast          = 0;
    /* Sharpening Yuv */
    // tmp registers
    pstSharpen->u8ManualSharpenYuvEnabled = 1;
    pstSharpen->u8gainThdSftD             = 0;
    pstSharpen->u8dirVarSft               = 10;
    pstSharpen->u8selPixWgt               = 31;

    pstSharpen->u8dirRlyThrhigh  = 120;
    pstSharpen->u8dirRlyThrLow = 2;
    pstSharpen->u8WeakDetailGain = 2;

    pstSharpen->u8RmfGainScale    = 2;
    pstSharpen->u8BmfGainScale    = 4;

    pstSharpen->u8gainThdSelUD    = 2;
    pstSharpen->u8gainThdSftUD    = 0;

    pstSharpen->u8shtVarWgt0     = 10;
    pstSharpen->u8shtVarDiffThd0 = 20;
    pstSharpen->u8shtVarDiffThd1 = 35;
    pstSharpen->u8shtVarDiffWgt1 = 27;
    // MPI
    pstSharpen->u8SkinUmax      = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMAX_DEFAULT;
    pstSharpen->u8SkinUmin      = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMIN_DEFAULT;
    pstSharpen->u8SkinVmin      = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMIN_DEFAULT;
    pstSharpen->u8SkinVmax      = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMAX_DEFAULT;


    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        pstSharpen->au16TextureStr[i] = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT;
        pstSharpen->au16EdgeStr[i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT;
    }
    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        pstSharpen->au8LumaWgt[i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT;
    }

    pstSharpen->u16TextureFreq  = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT;
    pstSharpen->u16EdgeFreq     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT;
    pstSharpen->u8OverShoot     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT;
    pstSharpen->u8UnderShoot    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT;
    pstSharpen->u8ShootSupStr   = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT;
    pstSharpen->u8DetailCtrl    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT;
    pstSharpen->u8EdgeFiltStr   = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT;
    pstSharpen->u8EdgeFiltMaxCap = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT;
    pstSharpen->u8RGain         = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT;
    pstSharpen->u8GGain         = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT;
    pstSharpen->u8BGain         = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT;
    pstSharpen->u8SkinGain      = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT;
    pstSharpen->u8ShootSupAdj     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT;
    pstSharpen->u16MaxSharpGain    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT;
    pstSharpen->u8DetailCtrlThr    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            pstSharpen->au16AutoTextureStr[j][i] = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT;
            pstSharpen->au16AutoEdgeStr[j][i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT;
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            pstSharpen->au8AutoLumaWgt[j][i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT;
        }
        pstSharpen->au16TextureFreq[i] = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT;
        pstSharpen->au16EdgeFreq[i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT;
        pstSharpen->au8OverShoot[i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT;
        pstSharpen->au8UnderShoot[i]   = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT;
        pstSharpen->au8ShootSupStr[i]  = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT;
        pstSharpen->au8DetailCtrl[i]   = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT;
        pstSharpen->au8EdgeFiltStr[i]  = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT;
        pstSharpen->au8EdgeFiltMaxCap[i]  = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT;
        pstSharpen->au8RGain[i]        = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT;
        pstSharpen->au8GGain[i]        = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT;
        pstSharpen->au8BGain[i]        = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT;
        pstSharpen->au8SkinGain[i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT;
        pstSharpen->au8ShootSupAdj[i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT;
        pstSharpen->au16MaxSharpGain[i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT;
        pstSharpen->au8DetailCtrlThr[i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT;
    }
    return GK_SUCCESS;
}

GK_S32 ISP_SharpenInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_SHARPEN_S *pstSharpen = GK_NULL;
    ISP_CTX_S     *pstIspCtx  = GK_NULL;
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Sharpen);

    s32Ret = SharpenCtxInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    pstSharpen->u8ManualSharpenYuvEnabled = 1;
    pstSharpen->bSharpenEn                = GK_TRUE;
    pstSharpen->u32IsoLast                = 0;

    ISP_SharpenCtxInit(pstSharpen);
    SharpenRegsInitialize(ViPipe, (ISP_REG_CFG_S *)pRegCfg);
    s32Ret = SharpenExtRegsInitialize(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    return GK_SUCCESS;
}

static GK_VOID ISP_SharpenWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_U8  i;
    GK_U32 au32UpdateIdx[ISP_STRIPING_MAX_NUM] = {0};
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        au32UpdateIdx[i] = pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex;
    }

    ISP_SharpenInit(ViPipe, pRegCfg);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex = au32UpdateIdx[i] + 1;
        pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bSwitchMode    = GK_TRUE;
    }
}

static GK_S32 ISP_Sharpen_GetLinearDefaultRegCfg(ISP_SHARPEN_S *pstSharpenPara, GK_U32 u32ISO, GK_U32  idxCur, GK_U32 idxPre, GK_U32  isoLvlCur, GK_U32 isoLvlPre)
{
    // Linear mode defalt regs
    const GK_U8  u8gainThdSelUDLinear[ISP_AUTO_ISO_STRENGTH_NUM]    = {1,    1,    1,     1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1};
    const GK_U8  u8gainThdSftUDLinear[ISP_AUTO_ISO_STRENGTH_NUM]    = {0,     0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

    const GK_U8  u8shtVarWgt0Linear[ISP_AUTO_ISO_STRENGTH_NUM]      = {20,   20,   20,    20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20};
    const GK_U8  u8shtVarDiffThd0Linear[ISP_AUTO_ISO_STRENGTH_NUM]  = {27,   29,   31,    33,   37,   40,   42,   48,   49,   50,   53,   53,   53,   53,   53,   53};
    const GK_U8  u8shtVarDiffThd1Linear[ISP_AUTO_ISO_STRENGTH_NUM]  = {49,   50,   51,    52,   54,   56,   56,   61,   63,   64,   66,   67,   67,   67,   67,   67};
    const GK_U8  u8shtVarDiffWgt1Linear[ISP_AUTO_ISO_STRENGTH_NUM]  = {10,   10,   10,    10,   15,   15,   15,   18,   20,   20,   20,   20,   20,   20,   20,   20};

    GK_U16 WgtPre = 0;   // for linerinter
    GK_U16 WgtCur = 0;   // for linerinter
    GK_U8  sft = 8;      // for linerinter

    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8gainThdSelUD    =  u8gainThdSelUDLinear[idxPre];
        pstSharpenPara->u8gainThdSftUD    =  u8gainThdSftUDLinear[idxPre];
        pstSharpenPara->u8shtVarWgt0      =  u8shtVarWgt0Linear[idxPre];
        pstSharpenPara->u8shtVarDiffThd0  =  u8shtVarDiffThd0Linear[idxPre];
        pstSharpenPara->u8shtVarDiffThd1  =  u8shtVarDiffThd1Linear[idxPre];
        pstSharpenPara->u8shtVarDiffWgt1  =  u8shtVarDiffWgt1Linear[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8gainThdSelUD    =  u8gainThdSelUDLinear[idxCur];
        pstSharpenPara->u8gainThdSftUD    =  u8gainThdSftUDLinear[idxCur];
        pstSharpenPara->u8shtVarWgt0      =  u8shtVarWgt0Linear[idxCur];
        pstSharpenPara->u8shtVarDiffThd0  =  u8shtVarDiffThd0Linear[idxCur];
        pstSharpenPara->u8shtVarDiffThd1  =  u8shtVarDiffThd1Linear[idxCur];
        pstSharpenPara->u8shtVarDiffWgt1  =  u8shtVarDiffWgt1Linear[idxCur];
    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8gainThdSelUD    =  shrp_blend(sft, WgtPre, u8gainThdSelUDLinear[idxPre], WgtCur, u8gainThdSelUDLinear[idxCur]);
        pstSharpenPara->u8gainThdSftUD    =  shrp_blend(sft, WgtPre, u8gainThdSftUDLinear[idxPre], WgtCur, u8gainThdSftUDLinear[idxCur]);
        pstSharpenPara->u8shtVarWgt0      =  shrp_blend(sft, WgtPre, u8shtVarWgt0Linear[idxPre], WgtCur, u8shtVarWgt0Linear[idxCur]);
        pstSharpenPara->u8shtVarDiffThd0  =  shrp_blend(sft, WgtPre, u8shtVarDiffThd0Linear[idxPre], WgtCur, u8shtVarDiffThd0Linear[idxCur]);
        pstSharpenPara->u8shtVarDiffThd1  =  shrp_blend(sft, WgtPre, u8shtVarDiffThd1Linear[idxPre], WgtCur, u8shtVarDiffThd1Linear[idxCur]);
        pstSharpenPara->u8shtVarDiffWgt1  =  shrp_blend(sft, WgtPre, u8shtVarDiffWgt1Linear[idxPre], WgtCur, u8shtVarDiffWgt1Linear[idxCur]);
    }

    return GK_SUCCESS;
}

static GK_S32 ISP_Sharpen_GetWdrDefaultRegCfg(ISP_SHARPEN_S *pstSharpenPara, GK_U32 u32ISO, GK_U32  idxCur, GK_U32 idxPre, GK_U32  isoLvlCur, GK_U32 isoLvlPre)
{
    // WDR mode defalt regs
    const GK_U8  u8gainThdSelUDWdr[ISP_AUTO_ISO_STRENGTH_NUM]      = { 2,    2,    2,     2,    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1};
    const GK_U8  u8gainThdSftUDWdr[ISP_AUTO_ISO_STRENGTH_NUM]      = { 0,    0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

    const GK_U8  u8shtVarWgt0Wdr[ISP_AUTO_ISO_STRENGTH_NUM]      = {65,   65,   65,    65,   65,   55,   25,   25,   25,   25,   25,   20,   20,   20,   20,   20};
    const GK_U8  u8shtVarDiffThd0Wdr[ISP_AUTO_ISO_STRENGTH_NUM]  = {27,   29,   31,    33,   37,   40,   42,   48,   49,   50,   53,   53,   53,   53,   53,   53};
    const GK_U8  u8shtVarDiffThd1Wdr[ISP_AUTO_ISO_STRENGTH_NUM]  = {49,   50,   51,    52,   54,   56,   56,   61,   63,   64,   66,   67,   67,   67,   67,   67};
    const GK_U8  u8shtVarDiffWgt1Wdr[ISP_AUTO_ISO_STRENGTH_NUM]  = { 5,    5,    5,     5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,    5};


    GK_U16 WgtPre = 0;   // for linerinter
    GK_U16 WgtCur = 0;   // for linerinter
    GK_U8  sft = 8;      // for linerinter

    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8gainThdSelUD    =  u8gainThdSelUDWdr[idxPre];
        pstSharpenPara->u8gainThdSftUD    =  u8gainThdSftUDWdr[idxPre];
        pstSharpenPara->u8shtVarWgt0      =  u8shtVarWgt0Wdr[idxPre];
        pstSharpenPara->u8shtVarDiffThd0  =  u8shtVarDiffThd0Wdr[idxPre];
        pstSharpenPara->u8shtVarDiffThd1  =  u8shtVarDiffThd1Wdr[idxPre];
        pstSharpenPara->u8shtVarDiffWgt1  =  u8shtVarDiffWgt1Wdr[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8gainThdSelUD    =  u8gainThdSelUDWdr[idxCur];
        pstSharpenPara->u8gainThdSftUD    =  u8gainThdSftUDWdr[idxCur];
        pstSharpenPara->u8shtVarWgt0      =  u8shtVarWgt0Wdr[idxCur];
        pstSharpenPara->u8shtVarDiffThd0  =  u8shtVarDiffThd0Wdr[idxCur];
        pstSharpenPara->u8shtVarDiffThd1  =  u8shtVarDiffThd1Wdr[idxCur];
        pstSharpenPara->u8shtVarDiffWgt1  =  u8shtVarDiffWgt1Wdr[idxCur];
    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8gainThdSelUD    =  shrp_blend(sft, WgtPre, u8gainThdSelUDWdr[idxPre], WgtCur, u8gainThdSelUDWdr[idxCur]);
        pstSharpenPara->u8gainThdSftUD    =  shrp_blend(sft, WgtPre, u8gainThdSftUDWdr[idxPre], WgtCur, u8gainThdSftUDWdr[idxCur]);
        pstSharpenPara->u8shtVarWgt0      =  shrp_blend(sft, WgtPre, u8shtVarWgt0Wdr[idxPre], WgtCur, u8shtVarWgt0Wdr[idxCur]);
        pstSharpenPara->u8shtVarDiffThd0  =  shrp_blend(sft, WgtPre, u8shtVarDiffThd0Wdr[idxPre], WgtCur, u8shtVarDiffThd0Wdr[idxCur]);
        pstSharpenPara->u8shtVarDiffThd1  =  shrp_blend(sft, WgtPre, u8shtVarDiffThd1Wdr[idxPre], WgtCur, u8shtVarDiffThd1Wdr[idxCur]);
        pstSharpenPara->u8shtVarDiffWgt1  =  shrp_blend(sft, WgtPre, u8shtVarDiffWgt1Wdr[idxPre], WgtCur, u8shtVarDiffWgt1Wdr[idxCur]);
    }

    return GK_SUCCESS;
}

static GK_VOID ISP_Sharpen_GetDefaultRegCfg(VI_PIPE ViPipe, GK_U32 u32ISO)
{
    // Common Regs
    const GK_U8  u8gainThdSftD[ISP_AUTO_ISO_STRENGTH_NUM]  = {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};
    const GK_U8  u8dirVarSft[ISP_AUTO_ISO_STRENGTH_NUM]    = { 10,  10,  10,  10,  10,  10,  10,   8,   7,   6,   5,   4,   3,   3,   3,   3};

    const GK_U8  u8selPixWgt[ISP_AUTO_ISO_STRENGTH_NUM]    = { 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31};
    const GK_U16 u8RmfGainScale[ISP_AUTO_ISO_STRENGTH_NUM] = { 24,  24,  24,  24,  24,  20,  14,  10,   6,   4,   2,   2,   2,   2,   2,   2};
    const GK_U16 u8BmfGainScale[ISP_AUTO_ISO_STRENGTH_NUM] = { 16,  12,  10,   6,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4};
    const GK_U8  u8dirRlyThrLow[ISP_AUTO_ISO_STRENGTH_NUM]  = { 2,      2,     2,     2,     2,     3,       4,    4,   5,    5,      6,     6,     7,      7,    7,     7};
    const GK_U8  u8dirRlyThrhigh[ISP_AUTO_ISO_STRENGTH_NUM]  = { 120,   120,  120,   120,   120,   121,    122,   123,    123,  124,   125,    126,  126,  126,     126,   126};


    GK_U32 idxCur, idxPre;
    GK_U32 isoLvlCur, isoLvlPre;
    GK_U8  u8WDRMode;

    GK_U16 WgtPre = 0;   // for linerinter
    GK_U16 WgtCur = 0;   // for linerinter
    GK_U8  sft = 8;      // for linerinter

    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_SHARPEN_S *pstSharpenPara = GK_NULL;

    SHARPEN_GET_CTX(ViPipe, pstSharpenPara);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    // Get ISO category index
    // idxCur : current index
    // idxPre : previous level index
    idxCur = GetIsoIndex(u32ISO);
    idxPre = (idxCur == 0) ? 0 : MAX2(idxCur - 1, 0);

    isoLvlCur   =  g_au32IsoLut[idxCur];
    isoLvlPre   =  g_au32IsoLut[idxPre];
    /* Common default regs */
    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8gainThdSftD   =  u8gainThdSftD[idxPre];
        pstSharpenPara->u8dirVarSft     =  u8dirVarSft[idxPre];
        pstSharpenPara->u8selPixWgt     =  u8selPixWgt[idxPre];
        pstSharpenPara->u8RmfGainScale  =  u8RmfGainScale[idxPre];
        pstSharpenPara->u8BmfGainScale  =  u8BmfGainScale[idxPre];
        pstSharpenPara->u8dirRlyThrLow  =  u8dirRlyThrLow[idxPre];
        pstSharpenPara->u8dirRlyThrhigh =  u8dirRlyThrhigh[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8gainThdSftD   =  u8gainThdSftD[idxCur];
        pstSharpenPara->u8dirVarSft     =  u8dirVarSft[idxCur];
        pstSharpenPara->u8selPixWgt     =  u8selPixWgt[idxCur];
        pstSharpenPara->u8RmfGainScale  =  u8RmfGainScale[idxCur];
        pstSharpenPara->u8BmfGainScale  =  u8BmfGainScale[idxCur];
        pstSharpenPara->u8dirRlyThrLow  =  u8dirRlyThrLow[idxCur];
        pstSharpenPara->u8dirRlyThrhigh =  u8dirRlyThrhigh[idxCur];
    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8gainThdSftD   =  shrp_blend(sft, WgtPre, u8gainThdSftD[idxPre], WgtCur, u8gainThdSftD[idxCur]);
        pstSharpenPara->u8dirVarSft     =  shrp_blend(sft, WgtPre, u8dirVarSft[idxPre], WgtCur, u8dirVarSft[idxCur]);
        pstSharpenPara->u8selPixWgt     =  shrp_blend(sft, WgtPre, u8selPixWgt[idxPre], WgtCur, u8selPixWgt[idxCur]);
        pstSharpenPara->u8RmfGainScale  =  shrp_blend(sft, WgtPre, u8RmfGainScale[idxPre], WgtCur, u8RmfGainScale[idxCur]);
        pstSharpenPara->u8BmfGainScale  =  shrp_blend(sft, WgtPre, u8BmfGainScale[idxPre], WgtCur, u8BmfGainScale[idxCur]);
        pstSharpenPara->u8dirRlyThrLow  =  shrp_blend(sft, WgtPre, u8dirRlyThrLow[idxPre], WgtCur, u8dirRlyThrLow[idxCur]);
        pstSharpenPara->u8dirRlyThrhigh =  shrp_blend(sft, WgtPre, u8dirRlyThrhigh[idxPre], WgtCur, u8dirRlyThrhigh[idxCur]);
    }

    /* Linear mode default regs */
    if (IS_LINEAR_MODE(u8WDRMode) || (pstIspCtx->stLinkage.enFSWDRMode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (pstIspCtx->stLinkage.enFSWDRMode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        ISP_Sharpen_GetLinearDefaultRegCfg(pstSharpenPara, u32ISO, idxCur, idxPre, isoLvlCur, isoLvlPre);
    }
    /* WDR mode default regs */
    else {
        ISP_Sharpen_GetWdrDefaultRegCfg(pstSharpenPara, u32ISO, idxCur, idxPre, isoLvlCur, isoLvlPre);
    }

}

static GK_S32 ISP_Sharpen_GetMpiRegCfg(VI_PIPE ViPipe, GK_U32 u32ISO)
{
    GK_U32  i;
    GK_S32  idxCur, idxPre;
    GK_S32  isoLvlCur, isoLvlPre;
    ISP_SHARPEN_S *pstSharpenPara = GK_NULL;

    GK_U16 WgtPre = 0;   // for linerinter
    GK_U16 WgtCur = 0;   // for linerinter
    GK_U8  sft = 8;      // for linerinter

    SHARPEN_GET_CTX(ViPipe, pstSharpenPara);
    ISP_CHECK_POINTER(pstSharpenPara);

    // Get ISO category index
    // idxCur : current index
    // idxPre : previous level index
    idxCur    = GetIsoIndex(u32ISO);
    idxPre    = MAX2(idxCur - 1, 0);
    isoLvlCur = g_au32IsoLut[idxCur];
    isoLvlPre = g_au32IsoLut[idxPre];

    // linerinter begain
    if (u32ISO <= isoLvlPre) {
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            pstSharpenPara->au16TextureStr[i] =  pstSharpenPara->au16AutoTextureStr[i][idxPre];
            pstSharpenPara->au16EdgeStr[i]    =  pstSharpenPara->au16AutoEdgeStr[i][idxPre];
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenPara->au8LumaWgt[i]     =  pstSharpenPara->au8AutoLumaWgt[i][idxPre];
        }
        pstSharpenPara->u16TextureFreq        =  pstSharpenPara->au16TextureFreq[idxPre];
        pstSharpenPara->u16EdgeFreq           =  pstSharpenPara->au16EdgeFreq[idxPre];
        pstSharpenPara->u8OverShoot           =  pstSharpenPara->au8OverShoot[idxPre];
        pstSharpenPara->u8UnderShoot          =  pstSharpenPara->au8UnderShoot[idxPre];
        pstSharpenPara->u8ShootSupStr         =  pstSharpenPara->au8ShootSupStr[idxPre];
        pstSharpenPara->u8DetailCtrl          =  pstSharpenPara->au8DetailCtrl[idxPre];
        pstSharpenPara->u8EdgeFiltStr         =  pstSharpenPara->au8EdgeFiltStr[idxPre];
        pstSharpenPara->u8EdgeFiltMaxCap      =  pstSharpenPara->au8EdgeFiltMaxCap[idxPre];
        pstSharpenPara->u8RGain               =  pstSharpenPara->au8RGain[idxPre];
        pstSharpenPara->u8GGain               =  pstSharpenPara->au8GGain[idxPre];
        pstSharpenPara->u8BGain               =  pstSharpenPara->au8BGain[idxPre];
        pstSharpenPara->u8SkinGain            =  pstSharpenPara->au8SkinGain[idxPre];
        pstSharpenPara->u8ShootSupAdj         =  pstSharpenPara->au8ShootSupAdj[idxPre];
        pstSharpenPara->u8DetailCtrlThr       =  pstSharpenPara->au8DetailCtrlThr[idxPre];
        pstSharpenPara->u16MaxSharpGain       =  pstSharpenPara->au16MaxSharpGain[idxPre];
        pstSharpenPara->u8WeakDetailGain      =  pstSharpenPara->au8WeakDetailGain[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            pstSharpenPara->au16TextureStr[i] =  pstSharpenPara->au16AutoTextureStr[i][idxCur];
            pstSharpenPara->au16EdgeStr[i]    =  pstSharpenPara->au16AutoEdgeStr[i][idxCur];
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenPara->au8LumaWgt[i]     =  pstSharpenPara->au8AutoLumaWgt[i][idxCur];
        }
        pstSharpenPara->u16TextureFreq        =  pstSharpenPara->au16TextureFreq[idxCur];
        pstSharpenPara->u16EdgeFreq           =  pstSharpenPara->au16EdgeFreq[idxCur];
        pstSharpenPara->u8OverShoot           =  pstSharpenPara->au8OverShoot[idxCur];
        pstSharpenPara->u8UnderShoot          =  pstSharpenPara->au8UnderShoot[idxCur];
        pstSharpenPara->u8ShootSupStr         =  pstSharpenPara->au8ShootSupStr[idxCur];
        pstSharpenPara->u8DetailCtrl          =  pstSharpenPara->au8DetailCtrl[idxCur];
        pstSharpenPara->u8EdgeFiltStr         =  pstSharpenPara->au8EdgeFiltStr[idxCur];
        pstSharpenPara->u8EdgeFiltMaxCap      =  pstSharpenPara->au8EdgeFiltMaxCap[idxCur];
        pstSharpenPara->u8RGain               =  pstSharpenPara->au8RGain[idxCur];
        pstSharpenPara->u8GGain               =  pstSharpenPara->au8GGain[idxCur];
        pstSharpenPara->u8BGain               =  pstSharpenPara->au8BGain[idxCur];
        pstSharpenPara->u8SkinGain            =  pstSharpenPara->au8SkinGain[idxCur];
        pstSharpenPara->u8ShootSupAdj         =  pstSharpenPara->au8ShootSupAdj[idxCur];
        pstSharpenPara->u8DetailCtrlThr       =  pstSharpenPara->au8DetailCtrlThr[idxCur];
        pstSharpenPara->u16MaxSharpGain       =  pstSharpenPara->au16MaxSharpGain[idxCur];
        pstSharpenPara->u8WeakDetailGain      =  pstSharpenPara->au8WeakDetailGain[idxCur];

    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            pstSharpenPara->au16TextureStr[i] =  shrp_blend(sft, WgtPre, pstSharpenPara->au16AutoTextureStr[i][idxPre], WgtCur, pstSharpenPara->au16AutoTextureStr[i][idxCur]);
            pstSharpenPara->au16EdgeStr[i]    =  shrp_blend(sft, WgtPre, pstSharpenPara->au16AutoEdgeStr[i][idxPre], WgtCur, pstSharpenPara->au16AutoEdgeStr[i][idxCur]);
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenPara->au8LumaWgt[i]     =  shrp_blend(sft, WgtPre, pstSharpenPara->au8AutoLumaWgt[i][idxPre], WgtCur, pstSharpenPara->au8AutoLumaWgt[i][idxCur]);
        }
        pstSharpenPara->u16TextureFreq        =  shrp_blend(sft, WgtPre, pstSharpenPara->au16TextureFreq[idxPre], WgtCur, pstSharpenPara->au16TextureFreq[idxCur]);
        pstSharpenPara->u16EdgeFreq           =  shrp_blend(sft, WgtPre, pstSharpenPara->au16EdgeFreq[idxPre], WgtCur, pstSharpenPara->au16EdgeFreq[idxCur]);
        pstSharpenPara->u8OverShoot           =  shrp_blend(sft, WgtPre, pstSharpenPara->au8OverShoot[idxPre], WgtCur, pstSharpenPara->au8OverShoot[idxCur]);
        pstSharpenPara->u8UnderShoot          =  shrp_blend(sft, WgtPre, pstSharpenPara->au8UnderShoot[idxPre], WgtCur, pstSharpenPara->au8UnderShoot[idxCur]);
        pstSharpenPara->u8ShootSupStr         =  shrp_blend(sft, WgtPre, pstSharpenPara->au8ShootSupStr[idxPre], WgtCur, pstSharpenPara->au8ShootSupStr[idxCur]);
        pstSharpenPara->u8DetailCtrl          =  shrp_blend(sft, WgtPre, pstSharpenPara->au8DetailCtrl[idxPre], WgtCur, pstSharpenPara->au8DetailCtrl[idxCur]);
        pstSharpenPara->u8EdgeFiltStr         =  shrp_blend(sft, WgtPre, pstSharpenPara->au8EdgeFiltStr[idxPre], WgtCur, pstSharpenPara->au8EdgeFiltStr[idxCur]);
        pstSharpenPara->u8EdgeFiltMaxCap      =  shrp_blend(sft, WgtPre, pstSharpenPara->au8EdgeFiltMaxCap[idxPre], WgtCur, pstSharpenPara->au8EdgeFiltMaxCap[idxCur]);
        pstSharpenPara->u8RGain               =  shrp_blend(sft, WgtPre, pstSharpenPara->au8RGain[idxPre], WgtCur, pstSharpenPara->au8RGain[idxCur]);
        pstSharpenPara->u8GGain               =  shrp_blend(sft, WgtPre, pstSharpenPara->au8GGain[idxPre], WgtCur, pstSharpenPara->au8GGain[idxCur]);
        pstSharpenPara->u8BGain               =  shrp_blend(sft, WgtPre, pstSharpenPara->au8BGain[idxPre], WgtCur, pstSharpenPara->au8BGain[idxCur]);
        pstSharpenPara->u8SkinGain            =  shrp_blend(sft, WgtPre, pstSharpenPara->au8SkinGain[idxPre], WgtCur, pstSharpenPara->au8SkinGain[idxCur]);
        pstSharpenPara->u8ShootSupAdj         =  shrp_blend(sft, WgtPre, pstSharpenPara->au8ShootSupAdj[idxPre], WgtCur, pstSharpenPara->au8ShootSupAdj[idxCur]);
        pstSharpenPara->u8DetailCtrlThr       =  shrp_blend(sft, WgtPre, pstSharpenPara->au8DetailCtrlThr[idxPre], WgtCur, pstSharpenPara->au8DetailCtrlThr[idxCur]);
        pstSharpenPara->u16MaxSharpGain       =  shrp_blend(sft, WgtPre, pstSharpenPara->au16MaxSharpGain[idxPre], WgtCur, pstSharpenPara->au16MaxSharpGain[idxCur]);
        pstSharpenPara->u8WeakDetailGain      =  shrp_blend(sft, WgtPre, pstSharpenPara->au8WeakDetailGain[idxPre], WgtCur, pstSharpenPara->au8WeakDetailGain[idxCur]);
    }

    return GK_SUCCESS;
}

static void SharpenMPI2Reg(ISP_SHARPEN_REG_CFG_S *pstSharpenRegCfg, ISP_SHARPEN_S *pstSharpen, GK_U32 u32ISO)
{
    GK_U8    i, j;
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstDefaultDynaRegCfg = GK_NULL;
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S     *pstMpiDynaRegCfg = GK_NULL;
    ISP_SHARPEN_STATIC_REG_CFG_S       *pstStaticRegCfg = GK_NULL;
    ISP_SHARPEN_DYNA_REG_CFG_S         *pstDynaRegCfg = GK_NULL;

    pstDynaRegCfg        = &(pstSharpenRegCfg->stDynaRegCfg);
    pstDefaultDynaRegCfg = &(pstDynaRegCfg->stDefaultDynaRegCfg);
    pstMpiDynaRegCfg     = &(pstDynaRegCfg->stMpiDynaRegCfg);
    pstStaticRegCfg      = &(pstSharpenRegCfg->stStaticRegCfg);

    if (pstDefaultDynaRegCfg->bResh) {
        pstDefaultDynaRegCfg->u8gainThdSftD       = pstSharpen->u8gainThdSftD;
        pstDefaultDynaRegCfg->u8gainThdSelUD      = pstSharpen->u8gainThdSelUD;
        pstDefaultDynaRegCfg->u8gainThdSftUD      = pstSharpen->u8gainThdSftUD;
        pstDefaultDynaRegCfg->u8dirVarSft         = pstSharpen->u8dirVarSft;
        pstDefaultDynaRegCfg->u8selPixWgt         = pstSharpen->u8selPixWgt;
        pstDefaultDynaRegCfg->u8shtVarDiffThd[0]  = pstSharpen->u8shtVarDiffThd0;
        pstDefaultDynaRegCfg->u8shtVarWgt0        = pstSharpen->u8shtVarWgt0;
        pstDefaultDynaRegCfg->u8shtVarDiffThd[1]  = pstSharpen->u8shtVarDiffThd1;
        pstDefaultDynaRegCfg->u8shtVarDiffWgt1    = pstSharpen->u8shtVarDiffWgt1;
        pstDefaultDynaRegCfg->u8RmfGainScale      = pstSharpen->u8RmfGainScale;
        pstDefaultDynaRegCfg->u8BmfGainScale      = pstSharpen->u8BmfGainScale;
        pstDefaultDynaRegCfg->u8dirRlyThrLow      = pstSharpen->u8dirRlyThrLow;
        pstDefaultDynaRegCfg->u8dirRlyThrhigh     = pstSharpen->u8dirRlyThrhigh;
    }

    if (pstMpiDynaRegCfg->bResh) {
        // GainD
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            j = i << 1;
            if (i < ISP_SHARPEN_GAIN_NUM - 1) {
                pstMpiDynaRegCfg->u16mfGainD[j]      = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16EdgeStr[i]));
                pstMpiDynaRegCfg->u16mfGainD[j + 1]  = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16EdgeStr[i] + pstSharpen->au16EdgeStr[i + 1]) >> 1)));
            } else { // 31
                pstMpiDynaRegCfg->u16mfGainD[j]  = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16EdgeStr[i]));
                pstMpiDynaRegCfg->u16mfGainD[j + 1]  = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16EdgeStr[i]));
            }
            pstMpiDynaRegCfg->u16hfGainD[j]  = (GK_U16)SHARPEN_CLIP3(0, 0xFFF, ((((GK_U32)(pstMpiDynaRegCfg->u16mfGainD[j])) * (pstSharpen->u16EdgeFreq)) >> 6));
            pstMpiDynaRegCfg->u16hfGainD[j + 1]  = (GK_U16)SHARPEN_CLIP3(0, 0xFFF, ((((GK_U32)(pstMpiDynaRegCfg->u16mfGainD[j + 1])) * (pstSharpen->u16EdgeFreq)) >> 6));
        }
        // GainUD
        if (pstDefaultDynaRegCfg->u8gainThdSelUD == 1) {
            j = 0;
            for (i = 0; i < 12; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            }
            for (i = 12; i < 20; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i] + pstSharpen->au16TextureStr[i + 1]) >> 1)));
            }
            for (i = 20; i < 31; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i]) * 2 + (pstSharpen->au16TextureStr[i + 1])) / 3));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i])   + (pstSharpen->au16TextureStr[i + 1]) * 2) / 3));
            }
            i = 31;
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
        } else {
            j = 0;
            for (i = 0; i < 16; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            }
            for (i = 16; i < 24; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i] + pstSharpen->au16TextureStr[i + 1]) >> 1)));
            }
            for (i = 24; i < 28; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i]) * 2 + (pstSharpen->au16TextureStr[i + 1])) / 3));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i])   + (pstSharpen->au16TextureStr[i + 1]) * 2) / 3));
            }
            for (i = 28; i < 31; i++) {
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i]) * 4 + (pstSharpen->au16TextureStr[i + 1])) / 5));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i]) * 3 + (pstSharpen->au16TextureStr[i + 1]) * 2) / 5));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i]) * 2 + (pstSharpen->au16TextureStr[i + 1]) * 3) / 5));
                pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i])   + (pstSharpen->au16TextureStr[i + 1]) * 4) / 5));
            }
            i = 31;
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            pstMpiDynaRegCfg->u16mfGainUD[j++] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
        }
        for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
            pstMpiDynaRegCfg->u16hfGainUD[i]  = (GK_U16)SHARPEN_CLIP3(0, 0xFFF, ((((GK_U32)(pstMpiDynaRegCfg->u16mfGainUD[i])) * (pstSharpen->u16TextureFreq)) >> 6));
        }

        // shoot ctrl
        pstMpiDynaRegCfg->u8oshtAmt              = pstSharpen->u8OverShoot;
        pstMpiDynaRegCfg->u8ushtAmt              = pstSharpen->u8UnderShoot;
        // skin Ctrl
        if (pstSharpen->u8SkinGain == 31) {
            pstMpiDynaRegCfg->u8bEnSkinCtrl      = 0;
        } else {
            pstMpiDynaRegCfg->u8bEnSkinCtrl      = 1;
            pstMpiDynaRegCfg->u8skinEdgeWgt[1]   = SHARPEN_CLIP3(0, 0x1F, (31 - pstSharpen->u8SkinGain));
            pstMpiDynaRegCfg->u8skinEdgeWgt[0]   = SHARPEN_CLIP3(0, 0x1F, (pstMpiDynaRegCfg->u8skinEdgeWgt[1] << 1));

        }
        // Chr Ctrl
        if ((pstSharpen->u8GGain == 32) & (pstSharpen->u8RGain == 31) & (pstSharpen->u8BGain == 31)) {
            pstMpiDynaRegCfg->u8bEnChrCtrl       = 0;
        } else {
            pstMpiDynaRegCfg->u8bEnChrCtrl       = 1;
            pstMpiDynaRegCfg->u8chrRGain         = pstSharpen->u8RGain;
            pstMpiDynaRegCfg->u16chrGGain        = pstSharpen->u8GGain;
            pstMpiDynaRegCfg->u16chrGmfGain      = pstSharpen->u8GGain;
            pstMpiDynaRegCfg->u8chrBGain         = pstSharpen->u8BGain;
        }

        if (pstSharpen->u8DetailCtrl == 128) {
            pstMpiDynaRegCfg->bEnDetailCtrl = 0;
        } else {
            pstMpiDynaRegCfg->bEnDetailCtrl = 1;
        }

        pstMpiDynaRegCfg->u8detailOshtAmt = SHARPEN_CLIP3(0, 127, (pstMpiDynaRegCfg->u8oshtAmt) + (pstSharpen->u8DetailCtrl) - 128);
        pstMpiDynaRegCfg->u8detailUshtAmt = SHARPEN_CLIP3(0, 127, (pstMpiDynaRegCfg->u8ushtAmt) + (pstSharpen->u8DetailCtrl) - 128);

        //dir
        pstMpiDynaRegCfg->u8dirDiffSft    = 63 - pstSharpen->u8EdgeFiltStr;

        if (pstSharpen->u8EdgeFiltMaxCap <= 12) {
            pstMpiDynaRegCfg->u8dirRt[1] = pstSharpen->u8EdgeFiltMaxCap;
            pstMpiDynaRegCfg->u8dirRt[0] = (pstSharpen->u8EdgeFiltMaxCap) >> 1;
        } else if (pstSharpen->u8EdgeFiltMaxCap <= 30) {
            pstMpiDynaRegCfg->u8dirRt[1] = pstSharpen->u8EdgeFiltMaxCap;
            pstMpiDynaRegCfg->u8dirRt[0] = 6;
        } else {
            pstMpiDynaRegCfg->u8dirRt[1] = 30;
            pstMpiDynaRegCfg->u8dirRt[0] = pstSharpen->u8EdgeFiltMaxCap - 24;
        }


        //Weak Detail Gain
        if (pstSharpen->u8WeakDetailGain == 0)
        { pstMpiDynaRegCfg->bEnWeakDetailAdj = 0; }
        else
        { pstMpiDynaRegCfg->bEnWeakDetailAdj = 1; }

        pstMpiDynaRegCfg->u8WeakDetailGain = pstSharpen->u8WeakDetailGain;


        pstMpiDynaRegCfg->u8bEnShtCtrlByVar = 1;
        pstMpiDynaRegCfg->u8shtBldRt        = pstSharpen->u8ShootSupAdj;
        pstMpiDynaRegCfg->u8shtVarThd1     = pstSharpen->u8ShootSupStr;


        pstMpiDynaRegCfg->u16oMaxGain = pstSharpen->u16MaxSharpGain;
        pstMpiDynaRegCfg->u16uMaxGain = pstSharpen->u16MaxSharpGain;
        pstMpiDynaRegCfg->u8skinMaxU = pstSharpen->u8SkinUmax;
        pstMpiDynaRegCfg->u8skinMinU = pstSharpen->u8SkinUmin;
        pstMpiDynaRegCfg->u8skinMaxV = pstSharpen->u8SkinVmax;
        pstMpiDynaRegCfg->u8skinMinV = pstSharpen->u8SkinVmin;
        pstMpiDynaRegCfg->u8detailOshtThr[0]  = pstSharpen->u8DetailCtrlThr;
        pstMpiDynaRegCfg->u8detailOshtThr[1]  = SHARPEN_CLIP3(0, 255, (pstSharpen->u8DetailCtrlThr + SHRP_DETAIL_CTRL_THR_DELTA));
        pstMpiDynaRegCfg->u8detailUshtThr[0]  = pstSharpen->u8DetailCtrlThr;
        pstMpiDynaRegCfg->u8detailUshtThr[1]  = SHARPEN_CLIP3(0, 255, (pstSharpen->u8DetailCtrlThr + SHRP_DETAIL_CTRL_THR_DELTA));
        pstMpiDynaRegCfg->u8bEnLumaCtrl = 0;
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstMpiDynaRegCfg->au8LumaWgt[i] = pstSharpen->au8LumaWgt[i];

            if (pstMpiDynaRegCfg->au8LumaWgt[i] < 31) {
                pstMpiDynaRegCfg->u8bEnLumaCtrl = 1;
            }
        }
    }

    /* Calc all MulCoef */
    // Defalut

    pstDefaultDynaRegCfg->s16shtVarDiffMul = CalcMulCoef(pstDefaultDynaRegCfg->u8shtVarDiffThd[0], pstStaticRegCfg->u8shtVarDiffWgt0,
                                                         pstDefaultDynaRegCfg->u8shtVarDiffThd[1], pstDefaultDynaRegCfg->u8shtVarDiffWgt1,
                                                         SHRP_SHT_VAR_MUL_PRECS);

    // Mpi
    pstMpiDynaRegCfg->u16shtVarMul  = CalcMulCoef(pstStaticRegCfg->u8shtVarThd0,  pstDefaultDynaRegCfg->u8shtVarWgt0,
                                                  pstMpiDynaRegCfg->u8shtVarThd1, pstStaticRegCfg->u8shtVarWgt1,
                                                  SHRP_SHT_VAR_MUL_PRECS);

    pstMpiDynaRegCfg->s16detailOshtMul = CalcMulCoef(pstMpiDynaRegCfg->u8detailOshtThr[0], pstMpiDynaRegCfg->u8detailOshtAmt,
                                                     pstMpiDynaRegCfg->u8detailOshtThr[1], pstMpiDynaRegCfg->u8oshtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);

    pstMpiDynaRegCfg->s16detailUshtMul = CalcMulCoef(pstMpiDynaRegCfg->u8detailUshtThr[0], pstMpiDynaRegCfg->u8detailUshtAmt,
                                                     pstMpiDynaRegCfg->u8detailUshtThr[1], pstMpiDynaRegCfg->u8ushtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);
    pstMpiDynaRegCfg->s16chrRMul      = CalcMulCoef(pstStaticRegCfg->u8chrRThd[0], pstMpiDynaRegCfg->u8chrRGain,
                                                    pstStaticRegCfg->u8chrRThd[1], 32,
                                                    SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16chrGMul      = CalcMulCoef(pstStaticRegCfg->u8chrGThd[0], pstMpiDynaRegCfg->u16chrGGain,
                                                    pstStaticRegCfg->u8chrGThd[1], 32,
                                                    SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16chrGmfMul    = CalcMulCoef(pstStaticRegCfg->u8chrGThd[0], pstMpiDynaRegCfg->u16chrGmfGain,
                                                    pstStaticRegCfg->u8chrGThd[1], 32,
                                                    SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16chrBMul      = CalcMulCoef(pstStaticRegCfg->u8chrBThd[0], pstMpiDynaRegCfg->u8chrBGain,
                                                    pstStaticRegCfg->u8chrBThd[1], 32,
                                                    SHRP_CHR_MUL_SFT);
    pstMpiDynaRegCfg->s16skinEdgeMul =  CalcMulCoef(pstStaticRegCfg->u8skinEdgeThd[0], pstMpiDynaRegCfg->u8skinEdgeWgt[0],
                                                    pstStaticRegCfg->u8skinEdgeThd[1], pstMpiDynaRegCfg->u8skinEdgeWgt[1],
                                                    SHRP_SKIN_EDGE_MUL_PRECS);

}

static GK_BOOL __inline CheckSharpenOpen(ISP_SHARPEN_S *pstSharpen)
{
    return (pstSharpen->bSharpenEn == GK_TRUE);
}

static GK_VOID SharpenActualUpdate(VI_PIPE ViPipe, ISP_SHARPEN_S *pstSharpen)
{
    GK_U8 i;

    ext_system_actual_sharpen_overshootAmt_write(ViPipe, pstSharpen->u8OverShoot);
    ext_system_actual_sharpen_undershootAmt_write(ViPipe, pstSharpen->u8UnderShoot);
    ext_system_actual_sharpen_shootSupSt_write(ViPipe, pstSharpen->u8ShootSupStr);
    ext_system_actual_sharpen_edge_frequence_write(ViPipe, pstSharpen->u16EdgeFreq);
    ext_system_actual_sharpen_texture_frequence_write(ViPipe, pstSharpen->u16TextureFreq);

    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        ext_system_actual_sharpen_edge_str_write(ViPipe, i, pstSharpen->au16EdgeStr[i]);
        ext_system_actual_sharpen_texture_str_write(ViPipe, i, pstSharpen->au16TextureStr[i]);
    }
}


GK_S32 SharpenProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    GK_U8 i, u8Index;
    ISP_CTRL_PROC_WRITE_S stProcTmp;

    ISP_SHARPEN_S *pstSharpen = GK_NULL;

    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    if ((pstProc->pcProcBuff == GK_NULL) || (pstProc->u32BuffLen == 0)) {
        return GK_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "-----SHARPEN INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%16s\n",
                    "bSharpenEn");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%16u\n",
                    (GK_U16)pstSharpen->bSharpenEn);

    for (i = 0; i < 4; i++) {
        u8Index = i * 8;

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                        "%s"  "%d"  "--"  "%d:\n",
                        "LumaWgt ", u8Index, u8Index + 7);

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                        "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 0],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 1],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 2],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 3],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 4],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 5],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 6],
                        (GK_U16)pstSharpen->au8LumaWgt[u8Index + 7]);
    }

    for (i = 0; i < 4; i++) {
        u8Index = i * 8;

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                        "%s"  "%d"  "--"  "%d:\n",
                        "TextureStr ", u8Index, u8Index + 7);

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                        "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 0],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 1],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 2],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 3],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 4],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 5],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 6],
                        (GK_U16)pstSharpen->au16TextureStr[u8Index + 7]);
    }

    for (i = 0; i < 4; i++) {
        u8Index = i * 8;
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                        "%s"  "%d"  "--"  "%d:\n",
                        "EdgeStr ", u8Index, u8Index + 7);

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                        "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 0],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 1],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 2],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 3],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 4],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 5],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 6],
                        (GK_U16)pstSharpen->au16EdgeStr[u8Index + 7]);
    }

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s \n",
                    "TextureFreq", "EdgeFreq", "OverShoot", "UnderShoot", "ShootSupStr", "DetailCtrl", "EdgeFiltStr", "EdgeFiltMaxCap", "RGain", "GGain", "BGain", "SkinGain");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"   "%12u"   "%12u\n\n",
                    (GK_U16)pstSharpen->u16TextureFreq,
                    (GK_U16)pstSharpen->u16EdgeFreq,
                    (GK_U16)pstSharpen->u8OverShoot,
                    (GK_U16)pstSharpen->u8UnderShoot,
                    (GK_U16)pstSharpen->u8ShootSupStr,
                    (GK_U16)pstSharpen->u8DetailCtrl,
                    (GK_U16)pstSharpen->u8EdgeFiltStr,
                    (GK_U16)pstSharpen->u8EdgeFiltMaxCap,
                    (GK_U16)pstSharpen->u8RGain,
                    (GK_U16)pstSharpen->u8GGain,
                    (GK_U16)pstSharpen->u8BGain,
                    (GK_U16)pstSharpen->u8SkinGain);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12s" "%14s" "%14s" "%12s" "%12s" "%12s"  "%12s" "%16s\n",
                    "ShootSupAdj", "DetailCtrlThr", "MaxSharpGain", "SkinUmax", "SkinUmin", "SkinVmax", "SkinVmin", "WeakDetailGain");


    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12u"  "%14u"  "%14u"  "%12u"  "%12u"  "%12u"   "%12u"  "%16u\n\n",
                    (GK_U16)pstSharpen->u8ShootSupAdj,
                    (GK_U16)pstSharpen->u8DetailCtrlThr,
                    (GK_U16)pstSharpen->u16MaxSharpGain,
                    (GK_U16)pstSharpen->u8SkinUmax,
                    (GK_U16)pstSharpen->u8SkinUmin,
                    (GK_U16)pstSharpen->u8SkinVmax,
                    (GK_U16)pstSharpen->u8SkinVmin,
                    (GK_U16)pstSharpen->u8WeakDetailGain);

    pstProc->u32WriteLen += 1;

    return GK_SUCCESS;
}

GK_S32 ISP_SharpenRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg, GK_S32 s32Rsv)
{

    GK_U8  i;
    GK_U32 u32Iso = 0;

    ISP_CTX_S     *pstIspCtx  = GK_NULL;
    ISP_SHARPEN_S *pstSharpen = GK_NULL;
    ISP_REG_CFG_S *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Sharpen);
    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    if (pstIspCtx->stLinkage.bDefectPixel) {
        return GK_SUCCESS;
    }

    pstSharpen->bSharpenEn = ext_system_manual_isp_sharpen_en_read(ViPipe);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.bEnable = pstSharpen->bSharpenEn;
    }

    pstRegCfg->unKey.bit1SharpenCfg = 1;

    /* check hardware setting */
    if (!CheckSharpenOpen(pstSharpen)) {
        return GK_SUCCESS;
    }

    /* sharpen strength linkage with the u32ISO calculated by ae */
    u32Iso = pstIspCtx->stLinkage.u32Iso;

    SharpenReadExtregs(ViPipe);
    SharpenReadProMode(ViPipe);
    if ((u32Iso != pstSharpen->u32IsoLast) ||
        (pstIspCtx->stLinkage.enFSWDRMode != pstIspCtx->stLinkage.enPreFSWDRMode)) { // will not work if ISO is the same
        ISP_Sharpen_GetDefaultRegCfg(ViPipe, u32Iso);
        for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
            pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg.bResh = GK_TRUE;
        }
    }

    if (pstSharpen->bSharpenMpiUpdateEn) {
        if (pstSharpen->u8ManualSharpenYuvEnabled == OP_TYPE_AUTO) { // auto mode
            ISP_Sharpen_GetMpiRegCfg(ViPipe, u32Iso);
        }
        for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
            pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bResh            = GK_TRUE;
            pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex  += 1;
            SharpenMPI2Reg(&(pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg), pstSharpen, u32Iso);
        }
    } else {
        if (u32Iso != pstSharpen->u32IsoLast) {
            if (pstSharpen->u8ManualSharpenYuvEnabled == OP_TYPE_AUTO) { // auto mode
                ISP_Sharpen_GetMpiRegCfg(ViPipe, u32Iso);
                for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
                    pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bResh           = GK_TRUE;
                    pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex += 1;
                    SharpenMPI2Reg(&(pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg), pstSharpen, u32Iso);
                }
            } else {
                for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
                    SharpenMPI2Reg(&(pstRegCfg->stAlgRegCfg[i].stSharpenRegCfg), pstSharpen, u32Iso);
                }
            }
        } else {
        }
    }

    SharpenActualUpdate(ViPipe, pstSharpen);

    pstSharpen->u32IsoLast = u32Iso;    // will not work if ISO is the same

    return GK_SUCCESS;
}

GK_S32 ISP_SharpenCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S  *pRegCfg = GK_NULL;
    ISP_CTX_S     *pstIspCtx = GK_NULL;
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Sharpen);

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_SharpenWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;
        case ISP_PROC_WRITE:
            SharpenProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SharpenExit(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_REGCFG_S *pRegCfg   = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Sharpen);

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stSharpenRegCfg.bEnable = GK_FALSE;
    }
    pRegCfg->stRegCfg.unKey.bit1SharpenCfg = 1;

    ext_system_isp_sharpen_manu_mode_write(ViPipe, GK_FALSE);

    SharpenCtxExit(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterSharpen(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SHARPEN;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_SharpenInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_SharpenRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_SharpenCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_SharpenExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
