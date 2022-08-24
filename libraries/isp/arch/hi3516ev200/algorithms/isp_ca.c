/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_CA_CSC_DC_LEN       (3)
#define ISP_CA_CSC_COEF_LEN     (9)
#define ISP_CA_CSC_TYPE_DEFAULT (0)
#define ISP_CA_RATIO_MAX        (2047)

static const  GK_U16 g_au16YRatioLut[ISP_CA_YRATIO_LUT_LENGTH] = {
    516, 525, 534, 544, 554, 563, 573, 583, 594, 604, 614, 624, 634, 644, 654, 664, 674, 684, 694,
    704, 713, 723, 732, 741, 750, 758, 766, 775, 782, 790, 797, 804, 811, 817, 823, 828, 834,
    839, 844, 848, 853, 857, 861, 865, 868, 872, 875, 878, 881, 884, 887, 890, 892, 895, 898,
    900, 903, 905, 908, 910, 913, 915, 918, 921, 924, 926, 929, 932, 935, 937, 940, 943, 945,
    948, 950, 952, 955, 957, 959, 961, 964, 966, 968, 970, 972, 974, 976, 978, 979, 981, 983,
    985, 987, 988, 990, 992, 993, 994, 995, 996, 997, 998, 998, 999, 1000, 1001, 1002, 1002,
    1003, 1004, 1005, 1006, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1018, 1019, 1020, 1022, 1024
};
static const  GK_S16 g_as16ISORatio[ISP_AUTO_ISO_STRENGTH_NUM] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};

typedef struct ISP_CA_S {
    GK_BOOL bCaEn;      // u1.0
    GK_BOOL bCaCoefUpdateEn;
    GK_U16 u16LumaThdHigh;
    GK_U16 u16CaLumaThrLow;
    GK_S16 s16CaLumaRatioLow;
    GK_U16 u16CaSkinRatioThrHigh;
    GK_U16 u16CaSkinRatioThrMid;
    GK_S16 s16SaturationRatio;
    GK_U16 au16YRatioLut[ISP_CA_YRATIO_LUT_LENGTH];
    GK_S16 as16CaIsoRatio[ISP_AUTO_ISO_STRENGTH_NUM]; // 16
    GK_S16 s16ProcInfoIsoRatio;
} ISP_CA_S;

ISP_CA_S *g_pastCaCtx[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define CA_GET_CTX(dev, pstCtx)   (pstCtx = g_pastCaCtx[dev])
#define CA_SET_CTX(dev, pstCtx)   (g_pastCaCtx[dev] = pstCtx)
#define CA_RESET_CTX(dev)         (g_pastCaCtx[dev] = GK_NULL)

GK_S32 CaCtxInit(VI_PIPE ViPipe)
{
    ISP_CA_S *pastCaCtx = GK_NULL;

    CA_GET_CTX(ViPipe, pastCaCtx);

    if (pastCaCtx == GK_NULL) {
        pastCaCtx = (ISP_CA_S *)ISP_MALLOC(sizeof(ISP_CA_S));
        if (pastCaCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] CaCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastCaCtx, 0, sizeof(ISP_CA_S));

    CA_SET_CTX(ViPipe, pastCaCtx);

    return GK_SUCCESS;
}

GK_VOID CaCtxExit(VI_PIPE ViPipe)
{
    ISP_CA_S *pastCaCtx = GK_NULL;

    CA_GET_CTX(ViPipe, pastCaCtx);
    ISP_FREE(pastCaCtx);
    CA_RESET_CTX(ViPipe);
}

static GK_VOID CaExtRegsInitialize(VI_PIPE ViPipe)
{
    GK_U16 i;
    ISP_CA_S  *pstCA     = GK_NULL;

    CA_GET_CTX(ViPipe, pstCA);

    ext_system_ca_en_write(ViPipe, pstCA->bCaEn);

    ext_system_ca_luma_thdgh_write(ViPipe, ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT);
    ext_system_ca_saturation_ratio_write(ViPipe, ISP_EXT_CA_SATURATION_RATIO_DEFAULT);
    ext_system_ca_coef_update_en_write(ViPipe, GK_TRUE);

    for (i = 0; i < ISP_CA_YRATIO_LUT_LENGTH; i++) {
        ext_system_ca_y_ratio_lut_write(ViPipe, i, pstCA->au16YRatioLut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        ext_system_ca_iso_ratio_lut_write(ViPipe, i, pstCA->as16CaIsoRatio[i]);
    }

    return;
}

static GK_VOID CaStaticRegsInitialize(GK_U8 i, ISP_CA_STATIC_CFG_S *pstStaticRegCfg)
{
    pstStaticRegCfg->bCaLlhcProcEn  = GK_TRUE;
    pstStaticRegCfg->bCaSkinProcEn  = GK_TRUE;
    pstStaticRegCfg->bCaSatuAdjEn   = GK_TRUE;

    pstStaticRegCfg->u16CaLumaThrLow         = ISP_CA_LUMA_THD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaDarkChromaThrLow   = ISP_CA_DARKCHROMA_THD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaDarkChromaThrHigh  = ISP_CA_DARKCHROMA_THD_HIGH_DEFAULT;
    pstStaticRegCfg->u16CaSDarkChromaThrLow  = ISP_CA_SDARKCHROMA_THD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaSDarkChromaThrHigh = ISP_CA_SDARKCHROMA_THD_HIGH_DEFAULT;
    pstStaticRegCfg->s16CaLumaRatioLow       = ISP_CA_LUMA_RATIO_LOW_DEFAULT;

    pstStaticRegCfg->u16CaSkinLowLumaMinU    = ISP_CA_SKINLOWLUAM_UMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxU    = ISP_CA_SKINLOWLUAM_UMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMinUy   = ISP_CA_SKINLOWLUAM_UYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxUy   = ISP_CA_SKINLOWLUAM_UYMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinU   = ISP_CA_SKINHIGHLUAM_UMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxU   = ISP_CA_SKINHIGHLUAM_UMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinUy  = ISP_CA_SKINHIGHLUAM_UYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxUy  = ISP_CA_SKINHIGHLUAM_UYMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMinV    = ISP_CA_SKINLOWLUAM_VMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxV    = ISP_CA_SKINLOWLUAM_VMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMinVy   = ISP_CA_SKINLOWLUAM_VYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxVy   = ISP_CA_SKINLOWLUAM_VYMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinV   = ISP_CA_SKINHIGHLUAM_VMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxV   = ISP_CA_SKINHIGHLUAM_VMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinVy  = ISP_CA_SKINHIGHLUAM_VYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxVy  = ISP_CA_SKINHIGHLUAM_VYMAX_DEFAULT;
    pstStaticRegCfg->s16CaSkinUvDiff         = ISP_CA_SKINUVDIFF_DEFAULT;
    pstStaticRegCfg->u16CaSkinRatioThrLow    = ISP_CA_SKINRATIOTHD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaSkinRatioThrMid    = ISP_CA_SKINRATIOTHD_MID_DEFAULT;
    pstStaticRegCfg->u16CaSkinRatioThrHigh   = ISP_CA_SKINRATIOTHD_HIGH_DEFAULT;

    pstStaticRegCfg->bStaticResh = GK_TRUE;
}

static GK_VOID CaUsrRegsInitialize(ISP_CA_USR_CFG_S *pstUsrRegCfg, ISP_CA_S  *pstCA)
{
    GK_U16 u16Index;

    pstUsrRegCfg->u16CaLumaThrHigh  = ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT;
    u16Index = (pstUsrRegCfg->u16CaLumaThrHigh >> 3);
    u16Index = (u16Index >= ISP_CA_YRATIO_LUT_LENGTH) ? (ISP_CA_YRATIO_LUT_LENGTH - 1) : u16Index;
    pstUsrRegCfg->s16CaLumaRatioHigh = pstCA->au16YRatioLut[u16Index];
    pstUsrRegCfg->s16CaLumaRatioLow  = ISP_CA_LUMA_RATIO_LOW_DEFAULT;
    pstUsrRegCfg->s16CaYLumaRatio    = ISP_CA_Y_LUMA_RATIO_DEFAULT;
    pstUsrRegCfg->s16CaSkinBetaRatio = ISP_CA_SKIN_BETA_RATIO_DEFAULT;
    memcpy(pstUsrRegCfg->au16YRatioLUT, pstCA->au16YRatioLut, ISP_CA_YRATIO_LUT_LENGTH * sizeof(GK_U16));

    pstUsrRegCfg->bCaLutUpdateEn = GK_TRUE;
    pstUsrRegCfg->bResh          = GK_TRUE;
    pstUsrRegCfg->u32UpdateIndex = 1;
    pstUsrRegCfg->u8BufId        = 0;
}

static GK_VOID CaDynaRegsInitialize(ISP_CA_DYNA_CFG_S *pstDynaRegCfg)
{
    pstDynaRegCfg->u16CaISORatio  = 1024;
    pstDynaRegCfg->bResh          = GK_TRUE;

    return;
}

static GK_VOID CaRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8 i;
    ISP_CA_S  *pstCA     = GK_NULL;

    CA_GET_CTX(ViPipe, pstCA);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stCaRegCfg.bCaEn      = pstCA->bCaEn;
        pstRegCfg->stAlgRegCfg[i].stCaRegCfg.bLut2SttEn = GK_TRUE;

        CaStaticRegsInitialize(i, &pstRegCfg->stAlgRegCfg[i].stCaRegCfg.stStaticRegCfg);
        CaUsrRegsInitialize(&pstRegCfg->stAlgRegCfg[i].stCaRegCfg.stUsrRegCfg, pstCA);
        CaDynaRegsInitialize(&pstRegCfg->stAlgRegCfg[i].stCaRegCfg.stDynaRegCfg);
    }
    pstCA->u16CaLumaThrLow = pstRegCfg->stAlgRegCfg[0].stCaRegCfg.stStaticRegCfg.u16CaLumaThrLow;
    pstCA->s16CaLumaRatioLow = pstRegCfg->stAlgRegCfg[0].stCaRegCfg.stStaticRegCfg.s16CaLumaRatioLow;
    pstCA->u16CaSkinRatioThrHigh = pstRegCfg->stAlgRegCfg[0].stCaRegCfg.stStaticRegCfg.u16CaSkinRatioThrHigh;
    pstCA->u16CaSkinRatioThrMid = pstRegCfg->stAlgRegCfg[0].stCaRegCfg.stStaticRegCfg.u16CaSkinRatioThrMid;

    pstRegCfg->unKey.bit1CaCfg = 1;
}

static GK_S32 CaCheckCmosParam(VI_PIPE ViPipe, const ISP_CMOS_CA_S *pstCmosCa)
{
    GK_U16 i;

    ISP_CHECK_BOOL(pstCmosCa->bEnable);

    for (i = 0; i < ISP_CA_YRATIO_LUT_LENGTH; i++) {
        if (pstCmosCa->au16YRatioLut[i] > ISP_CA_RATIO_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Err au16YRatioLut[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCmosCa->as16ISORatio[i] > ISP_CA_RATIO_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Err as16ISORatio[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

static GK_S32 CaInInitialize(VI_PIPE ViPipe)
{
    GK_S32             s32Ret;
    ISP_CA_S           *pstCA     = GK_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

    CA_GET_CTX(ViPipe, pstCA);
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);

    if (pstSnsDft->unKey.bit1Ca) {
        ISP_CHECK_POINTER(pstSnsDft->pstCa);

        s32Ret = CaCheckCmosParam(ViPipe, pstSnsDft->pstCa);
        if (s32Ret != GK_SUCCESS) {
            return s32Ret;
        }

        pstCA->bCaEn  = pstSnsDft->pstCa->bEnable;
        memcpy(pstCA->au16YRatioLut, pstSnsDft->pstCa->au16YRatioLut, ISP_CA_YRATIO_LUT_LENGTH * sizeof(GK_U16));
        memcpy(pstCA->as16CaIsoRatio, pstSnsDft->pstCa->as16ISORatio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_S16));
    } else {
        pstCA->bCaEn  = GK_TRUE;
        memcpy(pstCA->au16YRatioLut,  g_au16YRatioLut, ISP_CA_YRATIO_LUT_LENGTH * sizeof(GK_U16));
        memcpy(pstCA->as16CaIsoRatio, g_as16ISORatio,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_S16));
    }

    return GK_SUCCESS;
}

static GK_S32 CaReadExtregs(VI_PIPE ViPipe)
{
    GK_U16 i;
    ISP_CA_S *pstCA = GK_NULL;

    CA_GET_CTX(ViPipe, pstCA);

    pstCA->bCaCoefUpdateEn = ext_system_ca_coef_update_en_read(ViPipe);
    ext_system_ca_coef_update_en_write(ViPipe, GK_FALSE);

    if (pstCA->bCaCoefUpdateEn) {

        for (i = 0; i < ISP_CA_YRATIO_LUT_LENGTH; i++) {
            pstCA->au16YRatioLut[i] = ext_system_ca_y_ratio_lut_read(ViPipe, i);
        }


        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstCA->as16CaIsoRatio[i] = ext_system_ca_iso_ratio_lut_read(ViPipe, i);
        }

        pstCA->u16LumaThdHigh     = ext_system_ca_luma_thdgh_read(ViPipe);
        pstCA->s16SaturationRatio = (GK_S16)ext_system_ca_saturation_ratio_read(ViPipe);
    }

    return GK_SUCCESS;
}

static GK_S32 CaGetValueFromLut(GK_S32 x, GK_U32 const *pLutX, GK_S16 *pLutY, GK_S32 length)
{
    GK_S32 n = 0;

    if (x <= pLutX[0]) {
        return pLutY[0];
    }

    for (n = 1; n < length; n++) {
        if (x <= pLutX[n]) {
            return (pLutY[n - 1] + (GK_S64)(pLutY[n] - pLutY[n - 1]) * (GK_S64)(x - (GK_S32)pLutX[n - 1]) / DIV_0_TO_1((GK_S32)pLutX[n] - (GK_S32)pLutX[n - 1]));
        }
    }

    return pLutY[length - 1];
}

static GK_BOOL __inline CheckCaOpen(ISP_CA_S *pstCA)
{
    return (pstCA->bCaEn == GK_TRUE);
}

static GK_VOID Isp_Ca_Usr_Fw(ISP_CA_S *pstCA, ISP_CA_USR_CFG_S *pstUsrRegCfg)
{
    GK_U16 j, i;
    GK_U16 u16lum_min = 0;
    GK_U16 u16lum_max = 0;
    GK_U16 u16lmin = 0;
    GK_U16 u16lmax = 0;
    GK_U16 u16lum_a = 0;
    GK_U16 u16lum_b = 0;
    GK_U16 u16weight = 200;

    for (j = 0; j < ISP_CA_YRATIO_LUT_LENGTH; j++) {
        pstUsrRegCfg->au16YRatioLUT[j] = MIN2((GK_S32)pstCA->au16YRatioLut[j] * pstCA->s16SaturationRatio / 1000, 2047);
    }

    u16lum_min = pstUsrRegCfg->au16YRatioLUT[0];
    u16lum_max = pstUsrRegCfg->au16YRatioLUT[0];
    u16lmin = pstUsrRegCfg->au16YRatioLUT[0];
    u16lmax = pstUsrRegCfg->au16YRatioLUT[0];
    u16lum_a = pstUsrRegCfg->au16YRatioLUT[0];
    u16lum_b = pstUsrRegCfg->au16YRatioLUT[0];

    for (i = 0; i < pstCA->u16CaLumaThrLow / 8; i = i + 2) {
        if (pstUsrRegCfg->au16YRatioLUT[i] >= pstUsrRegCfg->au16YRatioLUT[i + 1]) {
            u16lmin = pstUsrRegCfg->au16YRatioLUT[i + 1];
            u16lmax = pstUsrRegCfg->au16YRatioLUT[i];
        } else {
            u16lmax = pstUsrRegCfg->au16YRatioLUT[i + 1];
            u16lmin = pstUsrRegCfg->au16YRatioLUT[i];
        }

        if (u16lmax >= u16lum_max) {
            u16lum_max = u16lmax;
        }

        if (u16lmin <= u16lum_min) {
            u16lum_min = u16lmin;
        }
    }

    if (ABS((GK_S16)u16lum_max - 1024) >= ABS((GK_S16)u16lum_min - 1024)) {
        u16lum_a = u16lum_min;
        u16lum_b = u16lum_max;
    } else {
        u16lum_a = u16lum_max;
        u16lum_b = u16lum_min;
    }

    pstUsrRegCfg->s16CaLumaRatioLow = (u16weight * (u16lum_a - u16lum_b) + 1024 * u16lum_b) >> 10;

    u16lum_min = pstUsrRegCfg->au16YRatioLUT[pstCA->u16CaLumaThrLow / 8];
    u16lum_max = pstUsrRegCfg->au16YRatioLUT[pstCA->u16CaLumaThrLow / 8];

    u16lmin = pstUsrRegCfg->au16YRatioLUT[pstCA->u16CaLumaThrLow / 8];
    u16lmax = pstUsrRegCfg->au16YRatioLUT[pstCA->u16CaLumaThrLow / 8];

    for (i = pstCA->u16CaLumaThrLow / 8; i < pstCA->u16LumaThdHigh / 8; i = i + 2) {
        if (pstUsrRegCfg->au16YRatioLUT[i] >= pstUsrRegCfg->au16YRatioLUT[i + 1]) {
            u16lmin = pstUsrRegCfg->au16YRatioLUT[i + 1];
            u16lmax = pstUsrRegCfg->au16YRatioLUT[i];
        } else {
            u16lmax = pstUsrRegCfg->au16YRatioLUT[i + 1];
            u16lmin = pstUsrRegCfg->au16YRatioLUT[i];
        }

        if (u16lmax >= u16lum_max) {
            u16lum_max = u16lmax;
        }

        if (u16lmin <= u16lum_min) {
            u16lum_min = u16lmin;
        }
    }

    if (ABS((GK_S16)u16lum_max - 1024) >= ABS((GK_S16)u16lum_min - 1024)) {
        u16lum_a = u16lum_min;
        u16lum_b = u16lum_max;
    } else {
        u16lum_a = u16lum_max;
        u16lum_b = u16lum_min;
    }
    pstUsrRegCfg->s16CaLumaRatioHigh = (u16weight * (u16lum_a - u16lum_b) + 1024 * u16lum_b) >> 10;

    if ((pstCA->u16LumaThdHigh - pstCA->u16CaLumaThrLow) < 4) {
        pstUsrRegCfg->s16CaYLumaRatio = (pstUsrRegCfg->s16CaLumaRatioHigh - pstUsrRegCfg->s16CaLumaRatioLow) * 32 / 4;
    } else {
        pstUsrRegCfg->s16CaYLumaRatio = ((pstUsrRegCfg->s16CaLumaRatioHigh - pstUsrRegCfg->s16CaLumaRatioLow) * 32 / DIV_0_TO_1(pstCA->u16LumaThdHigh - pstCA->u16CaLumaThrLow));
    }

    if ((pstCA->u16CaSkinRatioThrHigh - pstCA->u16CaSkinRatioThrMid) < 4) {
        pstUsrRegCfg->s16CaSkinBetaRatio = (1024) * 32 / 4;
    } else {
        pstUsrRegCfg->s16CaSkinBetaRatio = ((1024) * 32 / DIV_0_TO_1(pstCA->u16CaSkinRatioThrHigh - pstCA->u16CaSkinRatioThrMid));
    }

    pstUsrRegCfg->bCaLutUpdateEn     = GK_TRUE;

    pstUsrRegCfg->bResh              = GK_TRUE;
    pstUsrRegCfg->u32UpdateIndex    += 1;

    return;
}

static GK_VOID Isp_Ca_Dyna_Fw(GK_S32 s32Iso, ISP_CA_DYNA_CFG_S *pstDynaRegCfg, ISP_CA_S *pstCA)
{
    GK_S32 s32IsoRatio;

    s32IsoRatio = CaGetValueFromLut(s32Iso, g_au32IsoLut, pstCA->as16CaIsoRatio, ISP_AUTO_ISO_STRENGTH_NUM);
    pstDynaRegCfg->u16CaISORatio = CLIP3(s32IsoRatio, 0, ISP_CA_RATIO_MAX);
    pstCA->s16ProcInfoIsoRatio = pstDynaRegCfg->u16CaISORatio;
    pstDynaRegCfg->bResh         = GK_TRUE;
}

static GK_S32 ISP_CaInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    s32Ret = CaCtxInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = CaInInitialize(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    CaRegsInitialize(ViPipe, pstRegCfg);
    CaExtRegsInitialize(ViPipe);

    return GK_SUCCESS;
}

static GK_S32 ISP_CaRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                        GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_U8  i;
    ISP_CA_S  *pstCA = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_REG_CFG_S *pstReg = (ISP_REG_CFG_S *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    CA_GET_CTX(ViPipe, pstCA);
    ISP_CHECK_POINTER(pstCA);

    /* calculate every two interrupts */
    if ((pstIspCtx->u32FrameCnt % 2 != 0) && (pstIspCtx->stLinkage.bSnapState != GK_TRUE)) {
        return GK_SUCCESS;
    }

    pstCA->bCaEn = ext_system_ca_en_read(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstReg->stAlgRegCfg[i].stCaRegCfg.bCaEn = pstCA->bCaEn;
    }

    pstReg->unKey.bit1CaCfg = 1;

    /* check hardware setting */
    if (!CheckCaOpen(pstCA)) {
        return GK_SUCCESS;
    }

    CaReadExtregs(ViPipe);

    if (pstCA->bCaCoefUpdateEn) {
        for (i = 0; i < pstReg->u8CfgNum; i++) {
            Isp_Ca_Usr_Fw(pstCA, &pstReg->stAlgRegCfg[i].stCaRegCfg.stUsrRegCfg);
        }
    }

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        Isp_Ca_Dyna_Fw((GK_S32)pstIspCtx->stLinkage.u32Iso, &pstReg->stAlgRegCfg[i].stCaRegCfg.stDynaRegCfg, pstCA);
    }

    return GK_SUCCESS;
}

GK_S32 CaProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_CA_S  *pstCa = GK_NULL;
    CA_GET_CTX(ViPipe, pstCa);
    ISP_CHECK_POINTER(pstCa);

    if ((pstProc->pcProcBuff == GK_NULL)
        || (pstProc->u32BuffLen == 0)) {
        return GK_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "-----CA INFO-----------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12s" "%12s\n",
                    "Enable", "isoRatio");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12u" "%12u\n",
                    pstCa->bCaEn, pstCa->s16ProcInfoIsoRatio);


    pstProc->u32WriteLen += 1;

    return GK_SUCCESS;
}


static GK_S32 ISP_CaCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    switch (u32Cmd) {
        case ISP_PROC_WRITE:
            CaProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return GK_SUCCESS;
}

static GK_S32 ISP_CaExit(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_REGCFG_S  *pRegCfg = GK_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stCaRegCfg.bCaEn = GK_FALSE;
    }

    pRegCfg->stRegCfg.unKey.bit1CaCfg = 1;

    CaCtxExit(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterCa(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Ca);
    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_CA;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CaInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CaRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CaCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CaExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
