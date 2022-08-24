/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <math.h>
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

#define RGBIR_CVTMATRIX_NUM 12

#ifndef IRMAX
#define IRMAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef IRMIN
#define IRMIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

typedef struct ISP_RGBIR_S {
    GK_BOOL bEnable;

    GK_U8   u8InPattern;
    GK_U8   u8OutPattern;
    GK_U8   u8IRStatus;

    GK_U8   u8Th;
    GK_U8   u8Tv;
    GK_U16  u16ExpCtrl1;  // u12.0
    GK_U16  u16ExpCtrl2;  // u12.0

    GK_U16  u16ReciExp1;  // u12.0
    GK_U16  u16ReciExp2;  // u12.0

    GK_U16  u16Gain1;  // u12.0
    GK_U16  u16Gain2;  // u12.0

    GK_S16  as16CvtMatrix[12];
} ISP_RGBIR_S;

ISP_RGBIR_S *g_astRgbirCtx[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define RGBIR_GET_CTX(dev, pstCtx)   (pstCtx = g_astRgbirCtx[dev])
#define RGBIR_SET_CTX(dev, pstCtx)   (g_astRgbirCtx[dev] = pstCtx)
#define RGBIR_RESET_CTX(dev)         (g_astRgbirCtx[dev] = GK_NULL)

GK_S16 as16DftCvtMatrix[12] = {1000, 0, 0, -1000, 0, 1000, 0, -1000, 0, 0, 1000, -1000};

GK_S32 RgbirCtxInit(VI_PIPE ViPipe)
{
    ISP_RGBIR_S *pstRgbir = GK_NULL;

    RGBIR_GET_CTX(ViPipe, pstRgbir);

    if (pstRgbir == GK_NULL) {
        pstRgbir = (ISP_RGBIR_S *)ISP_MALLOC(sizeof(ISP_RGBIR_S));
        if (pstRgbir == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] RgbirCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pstRgbir, 0, sizeof(ISP_RGBIR_S));

    RGBIR_SET_CTX(ViPipe, pstRgbir);

    return GK_SUCCESS;
}

GK_VOID RgbirCtxExit(VI_PIPE ViPipe)
{
    ISP_RGBIR_S *pstRgbir = GK_NULL;

    RGBIR_GET_CTX(ViPipe, pstRgbir);
    ISP_FREE(pstRgbir);
    RGBIR_RESET_CTX(ViPipe);
}

static GK_VOID RgbirExtRegsInitialize(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_CTX_S     *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ext_system_rgbir_inpattern_write(ViPipe, IRBAYER_GRGBI);
    ext_system_rgbir_outpattern_write(ViPipe, pstIspCtx->enBayer); // Output pattern should initialize as the system value
    ext_system_rgbir_irstatus_write(ViPipe, ISP_IR_CVTMAT_MODE_NORMAL); // Output pattern should initialize as the system value

    ext_system_rgbir_enable_write(ViPipe, GK_FALSE); // Init external regster as false

    ext_system_rgbir_expctrl_write(ViPipe, 1500, 0); // empirical value
    ext_system_rgbir_expctrl_write(ViPipe, 75, 1); // empirical value

    ext_system_rgbir_gain_write(ViPipe, 256, 0); // empirical value
    ext_system_rgbir_gain_write(ViPipe, 256, 1); // empirical value

    for (i = 0; i < ISP_RGBIR_CVTMATRIX_NUM; i++) {
        ext_system_rgbir_cvtmatrix_write(ViPipe, as16DftCvtMatrix[i], i); // 1000 as 1x
    }

    return;
}

static GK_S32 RgbirReadExtRegs(VI_PIPE ViPipe)
{
    GK_U16 i;
    GK_U16 u16ExpThCtl1, u16ExpThCtl2, u16ExtInput1, u16ExtInput2;
    GK_U16 u16BLC, u16BLC00, u16BLC01, u16BLC10, u16BLC11;

    ISP_RGBIR_S *pstRgbirCtx = GK_NULL;

    RGBIR_GET_CTX(ViPipe, pstRgbirCtx);
    ISP_CHECK_POINTER(pstRgbirCtx);

    u16BLC00 = ext_system_black_level_00_read(ViPipe);
    u16BLC01 = ext_system_black_level_01_read(ViPipe);
    u16BLC10 = ext_system_black_level_10_read(ViPipe);
    u16BLC11 = ext_system_black_level_11_read(ViPipe);

    u16ExtInput1 = ext_system_rgbir_expctrl_read(ViPipe, 0);
    u16ExtInput2 = ext_system_rgbir_expctrl_read(ViPipe, 1);

    u16BLC = (u16BLC00 + u16BLC01 + u16BLC10 + u16BLC11 + 2) >> 2;
    u16ExpThCtl1 = IRMAX(2050, (((1 << 12) - 1) - (IRMIN((u16BLC + u16ExtInput1), ((1 << 12) - 1))))); // default 12bit
    u16ExpThCtl2 = IRMAX(2050, (((1 << 12) - 1) - (IRMIN((u16BLC + u16ExtInput2), ((1 << 12) - 1))))); // default 12bit

    pstRgbirCtx->u8InPattern  = ext_system_rgbir_inpattern_read(ViPipe);
    pstRgbirCtx->u8OutPattern = ext_system_rgbir_outpattern_read(ViPipe);
    pstRgbirCtx->u8IRStatus   = ext_system_rgbir_irstatus_read(ViPipe);
    pstRgbirCtx->u16ExpCtrl1  = u16ExpThCtl1;  // u12.0
    pstRgbirCtx->u16ExpCtrl2  = u16ExpThCtl2;  // u12.0
    pstRgbirCtx->u16ReciExp1  = ((((1 << 12) - 1) << 8) + (u16ExpThCtl1 >> 1)) / DIV_0_TO_1(u16ExpThCtl1);  // u12.0
    pstRgbirCtx->u16ReciExp2  = ((((1 << 12) - 1) << 8) + (u16ExpThCtl2 >> 1)) / DIV_0_TO_1(u16ExpThCtl2);  // u12.0
    pstRgbirCtx->u16Gain1     = ext_system_rgbir_gain_read(ViPipe, 0);
    pstRgbirCtx->u16Gain2     = ext_system_rgbir_gain_read(ViPipe, 1);

    for (i = 0; i < ISP_RGBIR_CVTMATRIX_NUM; i++) {
        pstRgbirCtx->as16CvtMatrix[i] = ext_system_rgbir_cvtmatrix_read(ViPipe, i);
    }

    return GK_SUCCESS;
}

static GK_S32 RgbirInitialize(VI_PIPE ViPipe)
{
    GK_U8  u8WDRMode;
    GK_U16 u16ExpThCtl1, u16ExpThCtl2;
    GK_U16 u16BLC, u16BLC00, u16BLC01, u16BLC10, u16BLC11;
    ISP_CTX_S          *pstIspCtx   = GK_NULL;
    ISP_RGBIR_S        *pstRgbirCtx = GK_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft   = GK_NULL;

    ISP_SensorGetDefault(ViPipe, &pstSnsDft);
    RGBIR_GET_CTX(ViPipe, pstRgbirCtx);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstRgbirCtx);

    u8WDRMode  = pstIspCtx->u8SnsWDRMode;

    u16BLC00 = ext_system_black_level_00_read(ViPipe);
    u16BLC01 = ext_system_black_level_01_read(ViPipe);
    u16BLC10 = ext_system_black_level_10_read(ViPipe);
    u16BLC11 = ext_system_black_level_11_read(ViPipe);

    u16BLC = (u16BLC00 + u16BLC01 + u16BLC10 + u16BLC11 + 2) >> 2;
    u16ExpThCtl1 = IRMAX(2050, (((1 << 12) - 1) - (IRMIN((u16BLC + 1500), ((1 << 12) - 1))))); // default 12bit
    u16ExpThCtl2 = IRMAX(2050, (((1 << 12) - 1) - (IRMIN((u16BLC + 75), ((1 << 12) - 1))))); // default 12bit

    if (IS_LINEAR_MODE(u8WDRMode)) {
        /* Read from CMOS */
        if (pstSnsDft->unKey.bit1Rgbir) {
            ISP_CHECK_POINTER(pstSnsDft->pstRgbir);

            memcpy(&pstRgbirCtx->as16CvtMatrix, &pstSnsDft->pstRgbir->as16CvtMatrix, RGBIR_CVTMATRIX_NUMBER * sizeof(GK_S16));
            pstRgbirCtx->bEnable      = pstSnsDft->pstRgbir->bEnable;
        } else { /* Read from firmware */
            memcpy(&pstRgbirCtx->as16CvtMatrix, &as16DftCvtMatrix, RGBIR_CVTMATRIX_NUMBER * sizeof(GK_S16));
            pstRgbirCtx->bEnable      = GK_FALSE;
        }
    } else {
        memcpy(&pstRgbirCtx->as16CvtMatrix, &as16DftCvtMatrix, RGBIR_CVTMATRIX_NUMBER * sizeof(GK_S16));
        pstRgbirCtx->bEnable      = GK_FALSE;
    }

    pstRgbirCtx->u8InPattern  = IRBAYER_GRGBI;
    pstRgbirCtx->u8OutPattern = (GK_U8)pstIspCtx->enBayer;
    pstRgbirCtx->u8IRStatus   = ISP_IR_CVTMAT_MODE_NORMAL;
    pstRgbirCtx->u8Th         = 100;
    pstRgbirCtx->u8Tv         = 100;

    pstRgbirCtx->u16ExpCtrl1  = u16ExpThCtl1;
    pstRgbirCtx->u16ExpCtrl2  = u16ExpThCtl2;
    pstRgbirCtx->u16ReciExp1  = ((((1 << 12) - 1) << 8) + (u16ExpThCtl1 >> 1)) / DIV_0_TO_1(u16ExpThCtl1);
    pstRgbirCtx->u16ReciExp2  = ((((1 << 12) - 1) << 8) + (u16ExpThCtl2 >> 1)) / DIV_0_TO_1(u16ExpThCtl2);

    pstRgbirCtx->u16Gain1     = 256;
    pstRgbirCtx->u16Gain2     = 256;

    return GK_SUCCESS;
}

static GK_VOID RgbirUsrRegsInit(ISP_RGBIR_USR_CFG_S *pstUsrRegCfg, ISP_RGBIR_S *pstRgbir)
{
    GK_U32 j;
    pstUsrRegCfg->bResh          = GK_TRUE;
    pstUsrRegCfg->u32UpdateIndex = 1;

    pstUsrRegCfg->u8InPattern    = pstRgbir->u8InPattern;
    pstUsrRegCfg->u8OutPattern   = pstRgbir->u8OutPattern;
    pstUsrRegCfg->u8ThresV       = pstRgbir->u8Tv;
    pstUsrRegCfg->u8ThresH       = pstRgbir->u8Th;
    pstUsrRegCfg->u16ExpCtrl1    = pstRgbir->u16ExpCtrl1;
    pstUsrRegCfg->u16ExpCtrl2    = pstRgbir->u16ExpCtrl2;
    pstUsrRegCfg->u16ReciExp1    = pstRgbir->u16ReciExp1;
    pstUsrRegCfg->u16ReciExp2    = pstRgbir->u16ReciExp2;
    pstUsrRegCfg->u16Gain1       = pstRgbir->u16Gain1;
    pstUsrRegCfg->u16Gain2       = pstRgbir->u16Gain2;

    for (j = 0; j < 12; j++) {
        pstUsrRegCfg->s16Matrix[j]   = (pstRgbir->as16CvtMatrix[j]  * 64) / 1000;
    }
}

static GK_VOID RgbirRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U16 i;
    ISP_RGBIR_S *pstRgbirCtx = GK_NULL;

    RGBIR_GET_CTX(ViPipe, pstRgbirCtx);
    ISP_CHECK_POINTER_VOID(pstRgbirCtx);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        RgbirUsrRegsInit(&pstRegCfg->stAlgRegCfg[i].stRgbirCfg.stUsrRegCfg, pstRgbirCtx);
        pstRegCfg->stAlgRegCfg[i].stRgbirCfg.bEnable = pstRgbirCtx->bEnable;
    }

    pstRegCfg->unKey.bit1GammaCfg = 1;

    return;
}

GK_S32 ISP_RgbirInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret;

    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
    ISP_CTX_S     *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Rgbir);

    /* First initiate rgbir_ctx, using new template */
    s32Ret = RgbirCtxInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return GK_FAILURE;
    }

    /* Initiate external registers */
    RgbirExtRegsInitialize(ViPipe);

    /* Initiate struct used in this firmware */
    s32Ret = RgbirInitialize(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return GK_FAILURE;
    }

    /* Initiate logic registers */
    RgbirRegsInitialize(ViPipe, pstRegCfg);

    return GK_SUCCESS;

}

static GK_VOID ISP_RgbirWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_U8 i;
    GK_U32 au32UpdateIdx[ISP_STRIPING_MAX_NUM] = {0};
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        au32UpdateIdx[i] = pstRegCfg->stAlgRegCfg[i].stRgbirCfg.stUsrRegCfg.u32UpdateIndex;
    }

    ISP_RgbirInit(ViPipe, pRegCfg);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stRgbirCfg.stUsrRegCfg.u32UpdateIndex = au32UpdateIdx[i] + 1;
    }
}

static GK_BOOL __inline CheckRgbirOpen(ISP_RGBIR_S *pstRgbir)
{
    return (pstRgbir->bEnable == GK_TRUE);
}

GK_S32 ISP_RgbirRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                    GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_U8 i, j;
    ISP_REG_CFG_S        *pstReg       = (ISP_REG_CFG_S *)pRegCfg;
    ISP_RGBIR_S          *pstRgbirCtx  = GK_NULL;
    ISP_RGBIR_USR_CFG_S  *pstUsrRegCfg = GK_NULL;
    ISP_CTX_S            *pstIspCtx    = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Rgbir);
    RGBIR_GET_CTX(ViPipe, pstRgbirCtx);
    ISP_CHECK_POINTER(pstRgbirCtx);

    pstRgbirCtx->bEnable = ext_system_rgbir_enable_read(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstReg->stAlgRegCfg[i].stRgbirCfg.bEnable = pstRgbirCtx->bEnable;
    }

    pstReg->unKey.bit1RgbirCfg = 1;

    /* check hardware setting */
    if (!CheckRgbirOpen(pstRgbirCtx)) {
        return GK_SUCCESS;
    }

    // Update Control Regs;
    RgbirReadExtRegs(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstUsrRegCfg = &pstReg->stAlgRegCfg[i].stRgbirCfg.stUsrRegCfg;

        pstUsrRegCfg->u8InPattern  = pstRgbirCtx->u8InPattern;
        pstUsrRegCfg->u8OutPattern = pstRgbirCtx->u8OutPattern;
        pstUsrRegCfg->u16ExpCtrl1  = pstRgbirCtx->u16ExpCtrl1;
        pstUsrRegCfg->u16ExpCtrl2  = pstRgbirCtx->u16ExpCtrl2;
        pstUsrRegCfg->u16ReciExp1  = pstRgbirCtx->u16ReciExp1;
        pstUsrRegCfg->u16ReciExp2  = pstRgbirCtx->u16ReciExp2;
        pstUsrRegCfg->u16Gain1     = pstRgbirCtx->u16Gain1;
        pstUsrRegCfg->u16Gain2     = pstRgbirCtx->u16Gain2;

        for (j = 0; j < 12; j++) {
            pstUsrRegCfg->s16Matrix[j]   = (pstRgbirCtx->as16CvtMatrix[j]  * 64) / 1000;
        }

        pstUsrRegCfg->bResh = GK_TRUE;

        pstUsrRegCfg->u32UpdateIndex   += 1;
    }

    return GK_SUCCESS;
}

static GK_S32 ISP_RgbirCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S  *pRegCfg = GK_NULL;
    ISP_CTX_S     *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Rgbir);

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_RgbirWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;
        default :
            break;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_RgbirExit(VI_PIPE ViPipe)
{
    ISP_RGBIR_S *pstRgbirCtx = GK_NULL;
    ISP_CTX_S   *pstIspCtx  = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Rgbir);

    RGBIR_GET_CTX(ViPipe, pstRgbirCtx);
    ISP_CHECK_POINTER(pstRgbirCtx);

    RgbirCtxExit(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterRgbir(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_RGBIR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_RgbirInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_RgbirRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_RgbirCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_RgbirExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
