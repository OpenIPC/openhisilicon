/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static GK_VOID
ExpanderStaticRegsInitialize(VI_PIPE ViPipe,
			     ISP_EXPANDER_STATIC_CFG_S *pstStaticRegCfg)
{
	GK_U32 _i, _v;
	GK_U32 X0, Y0, X1, Y1, X2, Y2, X3, Y3, X_max, Y_max;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);

	if (pstSnsDft->unKey.bit1Expander) {
		pstStaticRegCfg->u8BitDepthIn =
			pstSnsDft->pstExpander->u8BitDepthIn;
		pstStaticRegCfg->u8BitDepthOut =
			pstSnsDft->pstExpander->u8BitDepthOut;

		X0 = pstSnsDft->pstExpander->astExpanderPoint[0].u16X;
		Y0 = pstSnsDft->pstExpander->astExpanderPoint[0].u32Y;
		X1 = pstSnsDft->pstExpander->astExpanderPoint[1].u16X;
		Y1 = pstSnsDft->pstExpander->astExpanderPoint[1].u32Y;
		X2 = pstSnsDft->pstExpander->astExpanderPoint[2].u16X;
		Y2 = pstSnsDft->pstExpander->astExpanderPoint[2].u32Y;
		X3 = pstSnsDft->pstExpander->astExpanderPoint[3].u16X;
		Y3 = pstSnsDft->pstExpander->astExpanderPoint[3].u32Y;
		X_max = pstSnsDft->pstExpander->astExpanderPoint[4].u16X;
		Y_max = pstSnsDft->pstExpander->astExpanderPoint[4].u32Y;

		for (_i = 0; _i < X0; _i++) {
			_v = (((_i * Y0) / DIV_0_TO_1(X0)) >> 0);
			pstStaticRegCfg->au16Lut[_i] = _v;
		}

		for (; _i < X1; _i++) {
			_v = ((((_i - X0) * (Y1 - Y0)) / DIV_0_TO_1(X1 - X0) +
			       Y0) >>
			      0);
			pstStaticRegCfg->au16Lut[_i] = _v;
		}

		for (; _i < X2; _i++) {
			_v = ((((_i - X1) * (Y2 - Y1)) / DIV_0_TO_1(X2 - X1) +
			       Y1) >>
			      0);
			pstStaticRegCfg->au16Lut[_i] = _v;
		}

		for (; _i < X3; _i++) {
			_v = ((((_i - X2) * (Y3 - Y2)) / DIV_0_TO_1(X3 - X2) +
			       Y2) >>
			      0);
			pstStaticRegCfg->au16Lut[_i] = _v;
		}

		for (; _i < X_max; _i++) {
			_v = (Y_max >> 0);
			pstStaticRegCfg->au16Lut[_i] = _v;
		}
	} else {
		pstStaticRegCfg->u8BitDepthIn = 12;
		pstStaticRegCfg->u8BitDepthOut = 16;
		memset(pstStaticRegCfg->au16Lut, 0,
		       EXPANDER_NODE_NUM * sizeof(GK_U16));
	}

	pstStaticRegCfg->bResh = GK_TRUE;
}

static GK_S32 ExpanderCheckCmosParam(VI_PIPE ViPipe,
				     const ISP_CMOS_EXPANDER_S *pstExpander)
{
	GK_U8 i;

	ISP_CHECK_BOOL(pstExpander->bEnable);

	if ((pstExpander->u8BitDepthIn > 0x14) ||
	    (pstExpander->u8BitDepthIn < 0xC)) {
		ISP_TRACE(MODULE_DBG_ERR, "err u8BitDepthIn!\n");
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	if ((pstExpander->u8BitDepthOut > 0x14) ||
	    (pstExpander->u8BitDepthOut < 0xC)) {
		ISP_TRACE(MODULE_DBG_ERR, "err u8BitDepthOut!\n");
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	for (i = 0; i < ISP_EXPANDER_POINT_NUM; i++) {
		if (pstExpander->astExpanderPoint[i].u16X > 0x81) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err astExpanderPoint[%d].u16X!\n", i);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstExpander->astExpanderPoint[i].u32Y > 0x8000) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err astExpanderPoint[%d].u32Y!\n", i);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}
	}

	return GK_SUCCESS;
}

static GK_S32 ExpanderRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 i;
	GK_S32 s32Ret;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);
	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstSnsDft->unKey.bit1Expander) {
		ISP_CHECK_POINTER(pstSnsDft->pstExpander);

		s32Ret = ExpanderCheckCmosParam(ViPipe, pstSnsDft->pstExpander);
		if (s32Ret != GK_SUCCESS) {
			return s32Ret;
		}
	}

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		ExpanderStaticRegsInitialize(
			ViPipe,
			&pstRegCfg->stAlgRegCfg[i].stExpanderCfg.stStaticCfg);

		if (pstIspCtx->u8SnsWDRMode == WDR_MODE_BUILT_IN) {
			if (pstSnsDft->unKey.bit1Expander) {
				pstRegCfg->stAlgRegCfg[i].stExpanderCfg.bEnable =
					pstSnsDft->pstExpander->bEnable;
			} else {
				pstRegCfg->stAlgRegCfg[i].stExpanderCfg.bEnable =
					GK_FALSE;
			}
		} else {
			pstRegCfg->stAlgRegCfg[i].stExpanderCfg.bEnable =
				GK_FALSE;
		}
	}

	pstRegCfg->unKey.bit1ExpanderCfg = 1;

	return GK_SUCCESS;
}

GK_S32 ISP_ExpanderInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_S32 s32Ret;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	s32Ret = ExpanderRegsInitialize(ViPipe, pstRegCfg);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_ExpanderRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
		       GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
	return GK_SUCCESS;
}

GK_S32 ISP_ExpanderCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	switch (u32Cmd) {
	case ISP_WDR_MODE_SET:
		ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
		ISP_CHECK_POINTER(pRegCfg);
		ISP_ExpanderInit(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
		break;
	default:
		break;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_ExpanderExit(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

	for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
		pRegCfg->stRegCfg.stAlgRegCfg[i].stExpanderCfg.bEnable =
			GK_FALSE;
	}

	pRegCfg->stRegCfg.unKey.bit1ExpanderCfg = 1;

	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterExpander(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Expander);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_EXPANDER;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_ExpanderInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_ExpanderRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_ExpanderCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_ExpanderExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
