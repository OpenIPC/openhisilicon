/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_DGain {
	GK_BOOL bEnable;
} ISP_DG_S;

ISP_DG_S g_astDgCtx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define DG_GET_CTX(dev, pstCtx) pstCtx = &g_astDgCtx[dev]

static GK_VOID BeDgStaticInit(VI_PIPE ViPipe,
			      ISP_DG_STATIC_CFG_S *pstStaticRegCfg)
{
	pstStaticRegCfg->bResh = GK_TRUE;
}

static GK_VOID BeDgDynaInit(VI_PIPE ViPipe, ISP_DG_DYNA_CFG_S *pstDynaRegCfg,
			    GK_U16 u16Gain)
{
	pstDynaRegCfg->u32ClipValue = 0xFFFFF;
	pstDynaRegCfg->u16GainR = u16Gain;
	pstDynaRegCfg->u16GainGR = u16Gain;
	pstDynaRegCfg->u16GainGB = u16Gain;
	pstDynaRegCfg->u16GainB = u16Gain;
	pstDynaRegCfg->bResh = GK_TRUE;
}

static GK_VOID WDRDgStaticInit(VI_PIPE ViPipe, GK_U8 i,
			       ISP_4DG_STATIC_CFG_S *pstStaticRegCfg)
{
	pstStaticRegCfg->u16GainR0 = 0x100;
	pstStaticRegCfg->u16GainGR0 = 0x100;
	pstStaticRegCfg->u16GainGB0 = 0x100;
	pstStaticRegCfg->u16GainB0 = 0x100;
	pstStaticRegCfg->u16GainR1 = 0x100;
	pstStaticRegCfg->u16GainGR1 = 0x100;
	pstStaticRegCfg->u16GainGB1 = 0x100;
	pstStaticRegCfg->u16GainB1 = 0x100;
	pstStaticRegCfg->u16GainR2 = 0x100;
	pstStaticRegCfg->u16GainGR2 = 0x100;
	pstStaticRegCfg->u16GainGB2 = 0x100;
	pstStaticRegCfg->u16GainB2 = 0x100;
	pstStaticRegCfg->u16GainR3 = 0x100;
	pstStaticRegCfg->u16GainGR3 = 0x100;
	pstStaticRegCfg->u16GainGB3 = 0x100;
	pstStaticRegCfg->u16GainB3 = 0x100;
	pstStaticRegCfg->bResh = GK_TRUE;
}

static GK_VOID WDRDgDynaInit(VI_PIPE ViPipe, ISP_4DG_DYNA_CFG_S *pstDynaRegCfg)
{
	pstDynaRegCfg->u32ClipValue0 = 0xFFFFF;
	pstDynaRegCfg->u32ClipValue1 = 0xFFFFF;
	pstDynaRegCfg->u32ClipValue2 = 0xFFFFF;
	pstDynaRegCfg->u32ClipValue3 = 0xFFFFF;
	pstDynaRegCfg->bResh = GK_TRUE;
}

static GK_VOID FeDgDynaInit(VI_PIPE ViPipe, ISP_FE_DG_DYNA_CFG_S *pstDynaRegCfg,
			    GK_U16 u16Gain)
{
	GK_U8 i = 0;

	for (i = 0; i < 4; i++) {
		pstDynaRegCfg->au16GainR[i] = u16Gain;
		pstDynaRegCfg->au16GainGR[i] = u16Gain;
		pstDynaRegCfg->au16GainGB[i] = u16Gain;
		pstDynaRegCfg->au16GainB[i] = u16Gain;
	}
	pstDynaRegCfg->u32ClipValue = 0xFFFFF;
	pstDynaRegCfg->bResh = GK_TRUE;
}

static GK_VOID WDRDgEnInit(VI_PIPE ViPipe, GK_U8 i, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 u8WDRMode = 0;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	u8WDRMode = pstIspCtx->u8SnsWDRMode;

	if (IS_LINEAR_MODE(u8WDRMode)) {
		pstRegCfg->stAlgRegCfg[i].st4DgRegCfg.bEnable = GK_FALSE;
	} else if (IS_2to1_WDR_MODE(u8WDRMode)) {
		pstRegCfg->stAlgRegCfg[i].st4DgRegCfg.bEnable = GK_TRUE;
	} else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)) {
		pstRegCfg->stAlgRegCfg[i].st4DgRegCfg.bEnable = GK_FALSE;
	}
}

static GK_VOID DgRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 i;
	GK_U16 u16Gain;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	u16Gain = pstIspCtx->stLinkage.u32IspDgain;

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		BeDgStaticInit(
			ViPipe,
			&pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stStaticRegCfg);
		BeDgDynaInit(ViPipe,
			     &pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stDynaRegCfg,
			     u16Gain);

		WDRDgStaticInit(
			ViPipe, i,
			&pstRegCfg->stAlgRegCfg[i].st4DgRegCfg.stStaticRegCfg);
		WDRDgDynaInit(
			ViPipe,
			&pstRegCfg->stAlgRegCfg[i].st4DgRegCfg.stDynaRegCfg);

		WDRDgEnInit(ViPipe, i, pstRegCfg);

		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.bDgEn = GK_TRUE;
		pstRegCfg->unKey.bit1DgCfg = 1;
		pstRegCfg->unKey.bit1WDRDgCfg = 1;
	}

	FeDgDynaInit(ViPipe,
		     &pstRegCfg->stAlgRegCfg[0].stFeDgRegCfg.stDynaRegCfg,
		     u16Gain);
	pstRegCfg->stAlgRegCfg[0].stFeDgRegCfg.bDgEn = GK_TRUE;
	pstRegCfg->unKey.bit1FeDgCfg = 1;
}

static GK_VOID DgExtRegsInitialize(VI_PIPE ViPipe)
{
	ext_system_isp_dgain_enable_write(ViPipe, GK_TRUE);
}

static GK_VOID DgInitialize(VI_PIPE ViPipe)
{
	ISP_DG_S *pstDg = GK_NULL;

	DG_GET_CTX(ViPipe, pstDg);

	pstDg->bEnable = GK_TRUE;
}

static GK_VOID ISP_DgWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_U8 i, j;
	GK_U16 u16Gain;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
	ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;

	ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

	u16Gain = CLIP3(
		4095 * 256 /
				DIV_0_TO_1(
					4095 -
					pstSnsBlackLevel->au16BlackLevel[1]) +
			1,
		0x100, 0x200);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		WDRDgEnInit(ViPipe, i, pstRegCfg);
		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stStaticRegCfg.bResh =
			GK_TRUE;
		pstRegCfg->stAlgRegCfg[i].st4DgRegCfg.stStaticRegCfg.bResh =
			GK_TRUE;

		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stDynaRegCfg.u16GainR =
			u16Gain;
		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stDynaRegCfg.u16GainGR =
			u16Gain;
		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stDynaRegCfg.u16GainGB =
			u16Gain;
		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.stDynaRegCfg.u16GainB =
			u16Gain;
	}

	pstRegCfg->unKey.bit1WDRDgCfg = 1;
	pstRegCfg->unKey.bit1FeDgCfg = 1;
	pstRegCfg->unKey.bit1DgCfg = 1;

	for (j = 0; j < 4; j++) {
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainR[j] = u16Gain;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainGR[j] = u16Gain;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainGB[j] = u16Gain;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainB[j] = u16Gain;
	}
}

GK_S32 ISP_DgInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	DgRegsInitialize(ViPipe, pstRegCfg);
	DgExtRegsInitialize(ViPipe);
	DgInitialize(ViPipe);

	return GK_SUCCESS;
}

static GK_BOOL __inline CheckDgOpen(ISP_DG_S *pstDg)
{
	return (pstDg->bEnable == GK_TRUE);
}

GK_S32 ISP_DgRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
		 GK_S32 s32Rsv)
{
	GK_S32 i;
	GK_U32 u32IspDgain;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_DG_S *pstDg = GK_NULL;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
	GK_U32 u32WDRGain;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	DG_GET_CTX(ViPipe, pstDg);

	if (pstIspCtx->stLinkage.bDefectPixel &&
	    (ext_system_dpc_static_defect_type_read(ViPipe) == 0)) {
		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			pstRegCfg->stAlgRegCfg[i].stDgRegCfg.bDgEn = GK_FALSE;
		}

		pstRegCfg->unKey.bit1DgCfg = 1;

		return GK_SUCCESS;
	}

	pstDg->bEnable = ext_system_isp_dgain_enable_read(ViPipe);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstRegCfg->stAlgRegCfg[i].stDgRegCfg.bDgEn = pstDg->bEnable;
	}

	pstRegCfg->stAlgRegCfg[0].stFeDgRegCfg.bDgEn = pstDg->bEnable;

	pstRegCfg->unKey.bit1FeDgCfg = 1;
	pstRegCfg->unKey.bit1DgCfg = 1;

	/* check hardware setting */
	if (!CheckDgOpen(pstDg)) {
		return GK_SUCCESS;
	}

	u32IspDgain = pstIspCtx->stLinkage.u32IspDgain;

	for (i = 0; i < 4; i++) {
		u32WDRGain = ((GK_U64)u32IspDgain *
			      pstIspCtx->stLinkage.au32WDRGain[i]) >>
			     8;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainR[i] = u32WDRGain;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainGR[i] = u32WDRGain;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainGB[i] = u32WDRGain;
		pstRegCfg->stAlgRegCfg[0]
			.stFeDgRegCfg.stDynaRegCfg.au16GainB[i] = u32WDRGain;
	}

	pstRegCfg->stAlgRegCfg[0].stFeDgRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	return GK_SUCCESS;
}

GK_S32 ISP_DgCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	switch (u32Cmd) {
	case ISP_CHANGE_IMAGE_MODE_SET:
	case ISP_WDR_MODE_SET:
		ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
		ISP_CHECK_POINTER(pRegCfg);
		ISP_DgWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
		break;
	default:
		break;
	}
	return GK_SUCCESS;
}

GK_S32 ISP_DgExit(VI_PIPE ViPipe)
{
	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterDg(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Dg);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_DG;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_DgInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_DgRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DgCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DgExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
