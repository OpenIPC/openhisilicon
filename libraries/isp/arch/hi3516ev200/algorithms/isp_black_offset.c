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

#define LUT_FACTOR (7)

typedef struct ISP_BLACKLEVEL_S {
	GK_BOOL bPreDefectPixel;
	GK_U8 u8BlackLevelChange;
	GK_U8 u8WDRModeState;
	GK_U16 au16BlackLevel[ISP_BAYER_CHN_NUM];
	GK_U16 au16RmDiffBlackLevel[ISP_BAYER_CHN_NUM];
	GK_U16 au16ActualBlackLevel[ISP_BAYER_CHN_NUM];
	ISP_OP_TYPE_E enOpType;
} ISP_BLACKLEVEL_S;

ISP_BLACKLEVEL_S g_astBlackLevelCtx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define BLACKLEVEL_GET_CTX(dev, pstBlackLevelCtx) \
	pstBlackLevelCtx = &g_astBlackLevelCtx[dev]

static GK_VOID BlcInitialize(VI_PIPE ViPipe)
{
	GK_U8 i;
	GK_U8 u8WDRMode;
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;
	ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);
	ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);
	ISP_GET_CTX(ViPipe, pstIspCtx);

	u8WDRMode = pstIspCtx->u8SnsWDRMode;

	for (i = 0; i < 4; i++) {
		pstBlackLevel->au16BlackLevel[i] =
			pstSnsBlackLevel->au16BlackLevel[i];
	}

	if (IS_LINEAR_MODE(u8WDRMode) || IS_BUILT_IN_WDR_MODE(u8WDRMode)) {
		pstBlackLevel->u8WDRModeState = GK_FALSE;
	} else {
		pstBlackLevel->u8WDRModeState = GK_TRUE;
	}

	pstBlackLevel->bPreDefectPixel = GK_FALSE;
	pstBlackLevel->enOpType = OP_TYPE_AUTO;
}

static GK_VOID BlcExtRegsInitialize(VI_PIPE ViPipe)
{
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;

	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);

	ext_system_black_level_manual_mode_write(
		ViPipe, ISP_EXT_SYSTEM_BLACK_LEVEL_MANUAL_MODE_DEFAULT);
	ext_system_black_level_change_write(
		ViPipe, ISP_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT);
	ext_system_black_level_00_write(ViPipe,
					pstBlackLevel->au16BlackLevel[0]);
	ext_system_black_level_01_write(ViPipe,
					pstBlackLevel->au16BlackLevel[1]);
	ext_system_black_level_10_write(ViPipe,
					pstBlackLevel->au16BlackLevel[2]);
	ext_system_black_level_11_write(ViPipe,
					pstBlackLevel->au16BlackLevel[3]);

	ext_system_black_level_query_00_write(
		ViPipe, pstBlackLevel->au16ActualBlackLevel[0]);
	ext_system_black_level_query_01_write(
		ViPipe, pstBlackLevel->au16ActualBlackLevel[1]);
	ext_system_black_level_query_10_write(
		ViPipe, pstBlackLevel->au16ActualBlackLevel[2]);
	ext_system_black_level_query_11_write(
		ViPipe, pstBlackLevel->au16ActualBlackLevel[3]);
}

static GK_VOID BalanceBlackLevel(ISP_BLACKLEVEL_S *pstBlackLevel)
{
	GK_U8 i;
	GK_U16 u16OffsetMin;

	/* calulate Min blacklevel of RGrGbG channel */
	u16OffsetMin = pstBlackLevel->au16BlackLevel[0];
	for (i = 1; i < ISP_BAYER_CHN_NUM; i++) {
		if (u16OffsetMin > pstBlackLevel->au16BlackLevel[i]) {
			u16OffsetMin = pstBlackLevel->au16BlackLevel[i];
		}
	}

	for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
		pstBlackLevel->au16RmDiffBlackLevel[i] = u16OffsetMin;
	}
}

static GK_S32 LinearInterpol(GK_U16 xm, GK_U16 x0, GK_S32 y0, GK_U16 x1,
			     GK_S32 y1, GK_U8 u8Shift)
{
	GK_S32 ym;

	if (xm <= (x0 << u8Shift)) {
		return y0;
	}
	if (xm >= (x1 << u8Shift)) {
		return y1;
	}

	ym = (y1 - y0) * (xm - (x0 << u8Shift)) / DIV_0_TO_1(x1 - x0) + y0;

	return ym;
}

static GK_VOID GetBuiltInExpanderBlc(const ISP_CMOS_EXPANDER_S *pstSnsExpander,
				     GK_U16 *pu16SensorBlc,
				     GK_U16 *pu16ExpanderBlc)
{
	GK_U8 i, j;
	GK_U8 u8Shift = 12 - LUT_FACTOR;
	GK_U8 u8IndexUp = ISP_EXPANDER_POINT_NUM - 1;
	GK_U8 u8IndexLow = 0;
	GK_U16 au16X[ISP_EXPANDER_POINT_NUM + 1] = { 0 };
	GK_U32 au32Y[ISP_EXPANDER_POINT_NUM + 1] = { 0 };

	for (i = 1; i < ISP_EXPANDER_POINT_NUM + 1; i++) {
		au16X[i] = pstSnsExpander->astExpanderPoint[i - 1].u16X;
		au32Y[i] = pstSnsExpander->astExpanderPoint[i - 1].u32Y;
	}

	for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
		u8IndexUp = ISP_EXPANDER_POINT_NUM - 1;

		for (i = 0; i < ISP_EXPANDER_POINT_NUM; i++) {
			if ((pu16SensorBlc[j] >> u8Shift) < au16X[i]) {
				u8IndexUp = i;
				break;
			}
		}

		u8IndexLow = (GK_U8)MAX2((GK_S8)u8IndexUp - 1, 0);

		pu16ExpanderBlc[j] =
			((GK_U32)LinearInterpol(
				pu16SensorBlc[j], au16X[u8IndexLow],
				au32Y[u8IndexLow], au16X[u8IndexUp],
				au32Y[u8IndexUp], u8Shift)) >>
			(1 + u8Shift);
	}
}

static GK_VOID BE_BlcDynaRegs_Linear(VI_PIPE ViPipe,
				     isp_be_blc_dyna_cfg *dyna_blc,
				     ISP_BLACKLEVEL_S *pstBlackLevel)
{
	GK_U8 i, j;

	for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
		for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
			dyna_blc->st4DgBlc[i].blc[j] =
				pstBlackLevel->au16RmDiffBlackLevel[j]
				<< 2; /* 4DG */
			dyna_blc->stWdrBlc[i].blc[j] =
				pstBlackLevel->au16RmDiffBlackLevel[j]; /* WDR */
			dyna_blc->stWdrBlc[i].out_blc = 0; /* WDR */
		}

		dyna_blc->stLscBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* lsc */
		dyna_blc->stDgBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* Dg */
		dyna_blc->stAeBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* AE */
		dyna_blc->stMgBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* MG */
		dyna_blc->stWbBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* WB */
		dyna_blc->stRgbirBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j]; // rgbir
	}
	/* bnr */
	dyna_blc->stBnrBlc.blc[0] =
		pstBlackLevel->au16RmDiffBlackLevel[0]; // 12bits

	for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
		pstBlackLevel->au16ActualBlackLevel[i] =
			pstBlackLevel->au16BlackLevel[i];
	}
}

static GK_VOID BE_BlcDynaRegs_Wdr(VI_PIPE ViPipe, isp_be_blc_dyna_cfg *dyna_blc,
				  ISP_BLACKLEVEL_S *pstBlackLevel)
{
	GK_U8 i, j;
	GK_U8 u8WDRModeState = pstBlackLevel->u8WDRModeState;
	GK_U16 WDROutBlc = 0;

	if (u8WDRModeState == GK_FALSE) { // reg value same as linear mode
		BE_BlcDynaRegs_Linear(ViPipe, dyna_blc, pstBlackLevel);
	} else if (u8WDRModeState == GK_TRUE) {
		WDROutBlc = ((pstBlackLevel->au16RmDiffBlackLevel[0] >> 4)
			     << 6); /* WDR outblc */
		for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
			for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
				dyna_blc->st4DgBlc[i].blc[j] =
					pstBlackLevel->au16RmDiffBlackLevel[j]
					<< 2; /* 4DG */
				dyna_blc->stWdrBlc[i].blc[j] =
					pstBlackLevel->au16RmDiffBlackLevel
						[j]; /* WDR */
				dyna_blc->stWdrBlc[i].out_blc = WDROutBlc >>
								2; /* WDR */
			}

			dyna_blc->stLscBlc.blc[j] = WDROutBlc >> 6; /* lsc */
			dyna_blc->stDgBlc.blc[j] = WDROutBlc >> 6; /* Dg */
			dyna_blc->stAeBlc.blc[j] = WDROutBlc >> 6; /* AE */
			dyna_blc->stMgBlc.blc[j] = WDROutBlc >> 6; /* MG */
			dyna_blc->stWbBlc.blc[j] = WDROutBlc >> 6; /* WB */
			dyna_blc->stRgbirBlc.blc[j] = 0;
		}

		/* bnr */
		dyna_blc->stBnrBlc.blc[0] = WDROutBlc >> 8; // 12bits

		for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
			pstBlackLevel->au16ActualBlackLevel[i] =
				pstBlackLevel->au16BlackLevel[i];
		}
	}
}

static GK_S32 ExpanderCheckCmosParam(VI_PIPE ViPipe,
				     const ISP_CMOS_EXPANDER_S *pstExpander)
{
	GK_U8 i;

	for (i = 0; i < ISP_EXPANDER_POINT_NUM; i++) {
		if (pstExpander->astExpanderPoint[i].u16X > 0x101) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err astExpanderPoint[%d].u16X:%d!\n", i,
				  pstExpander->astExpanderPoint[i].u16X);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstExpander->astExpanderPoint[i].u32Y > 0x100000) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err astExpanderPoint[%d].u16Y:%d!\n", i,
				  pstExpander->astExpanderPoint[i].u32Y);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}
	}

	return GK_SUCCESS;
}

static GK_S32 BE_BlcDynaRegs_BuiltIn(VI_PIPE ViPipe,
				     isp_be_blc_dyna_cfg *dyna_blc,
				     ISP_BLACKLEVEL_S *pstBlackLevel)
{
	GK_U8 i, j;
	GK_U16 au16BlackLevel[ISP_BAYER_CHN_NUM] = { 0 };
	GK_U16 au16ActualBlackLevel[ISP_BAYER_CHN_NUM] = { 0 };
	GK_S32 s32Ret;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);

	if (pstSnsDft->unKey.bit1Expander) {
		ISP_CHECK_POINTER(pstSnsDft->pstExpander);
		s32Ret = ExpanderCheckCmosParam(ViPipe, pstSnsDft->pstExpander);
		if (s32Ret != GK_SUCCESS) {
			return s32Ret;
		}

		GetBuiltInExpanderBlc(pstSnsDft->pstExpander,
				      pstBlackLevel->au16RmDiffBlackLevel,
				      au16BlackLevel);
		GetBuiltInExpanderBlc(pstSnsDft->pstExpander,
				      pstBlackLevel->au16BlackLevel,
				      au16ActualBlackLevel);
	} else {
		for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
			au16BlackLevel[i] =
				pstBlackLevel->au16RmDiffBlackLevel[i]
				<< 2; // 14bits
			au16ActualBlackLevel[i] =
				pstBlackLevel->au16BlackLevel[i] << 2; // 14bits
		}
	}

	for (j = 0; j < 4; j++) {
		for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
			dyna_blc->st4DgBlc[i].blc[j] = 0; /* 4DG */
			dyna_blc->stWdrBlc[i].blc[j] = 0; /* WDR */
			dyna_blc->stWdrBlc[i].out_blc = 0; /* WDR */
		}

		dyna_blc->stLscBlc.blc[j] = au16BlackLevel[j]; /* lsc */
		dyna_blc->stDgBlc.blc[j] = au16BlackLevel[j]; /* Dg */
		dyna_blc->stAeBlc.blc[j] = au16BlackLevel[j]; /* AE */
		dyna_blc->stMgBlc.blc[j] = au16BlackLevel[j]; /* MG */
		dyna_blc->stWbBlc.blc[j] = au16BlackLevel[j]; /* WB */
		dyna_blc->stRgbirBlc.blc[j] = 0;
	}

	/* bnr */
	dyna_blc->stBnrBlc.blc[0] = au16BlackLevel[0] >> 2; // 12bits

	for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
		pstBlackLevel->au16ActualBlackLevel[i] =
			au16ActualBlackLevel[i]; /* Notice: Actual Blc is 14bits */
	}

	return GK_SUCCESS;
}

static GK_VOID BE_BlcDynaRegs(VI_PIPE ViPipe, GK_U8 u8WdrMode,
			      isp_be_blc_dyna_cfg *dyna_blc,
			      ISP_BLACKLEVEL_S *pstBlackLevel)
{
	pstBlackLevel->u8WDRModeState = ext_system_wdr_en_read(ViPipe);

	if (IS_LINEAR_MODE(u8WdrMode)) {
		BE_BlcDynaRegs_Linear(ViPipe, dyna_blc, pstBlackLevel);
	} else if (IS_2to1_WDR_MODE(u8WdrMode)) {
		BE_BlcDynaRegs_Wdr(ViPipe, dyna_blc, pstBlackLevel);
	} else if (IS_BUILT_IN_WDR_MODE(u8WdrMode)) {
		BE_BlcDynaRegs_BuiltIn(ViPipe, dyna_blc, pstBlackLevel);
	}

	dyna_blc->bReshDyna = GK_TRUE;
}

static GK_VOID BE_BlcStaticRegs(GK_U8 u8WdrMode,
				isp_be_blc_static_cfg *static_blc)
{
	GK_U8 i;

	/* 4DG */
	for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
		static_blc->st4DgBlc[i].blc_in = GK_TRUE;
		static_blc->st4DgBlc[i].blc_out = GK_TRUE;
	}

	/* WDR */
	static_blc->stWdrBlc[0].blc_out = GK_TRUE;
	/* Rgbir */
	static_blc->stRgbirBlc.blc_in = GK_TRUE;
	static_blc->stRgbirBlc.blc_out = GK_TRUE;
	/* lsc */
	static_blc->stLscBlc.blc_in = GK_TRUE;
	static_blc->stLscBlc.blc_out = GK_TRUE;
	/* Dg */
	static_blc->stDgBlc.blc_in = GK_TRUE;
	static_blc->stDgBlc.blc_out = GK_FALSE;
	/* AE */
	static_blc->stAeBlc.blc_in = GK_FALSE;
	/* MG */
	static_blc->stMgBlc.blc_in = GK_FALSE;
	/* WB */
	static_blc->stWbBlc.blc_in = GK_FALSE;
	static_blc->stWbBlc.blc_out = GK_FALSE;

	static_blc->bReshStatic = GK_TRUE;
}

static GK_VOID FE_BlcDynaRegs(isp_fe_blc_dyna_cfg *dyna_blc,
			      ISP_BLACKLEVEL_S *pstBlackLevel)
{
	GK_U8 j;
	for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
		dyna_blc->stFeBlc.blc[j] =
			(pstBlackLevel->au16BlackLevel[j] -
			 pstBlackLevel->au16RmDiffBlackLevel[j])
			<< 2; /* Fe Blc */
		dyna_blc->stFeDgBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* Fe Dg */
		dyna_blc->stFeWbBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* Fe WB */
		dyna_blc->stFeAeBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* Fe AE */
		dyna_blc->stRcBlc.blc[j] =
			pstBlackLevel->au16RmDiffBlackLevel[j] << 2; /* Fe RC */
	}

	dyna_blc->bReshDyna = GK_TRUE;
}

static GK_VOID FE_BlcStaticRegs(isp_fe_blc_static_cfg *static_blc)
{
	/* Fe Dg */
	static_blc->stFeDgBlc.blc_in = GK_TRUE;
	static_blc->stFeDgBlc.blc_out = GK_TRUE;
	/* Fe WB */
	static_blc->stFeWbBlc.blc_in = GK_TRUE;
	static_blc->stFeWbBlc.blc_out = GK_TRUE;
	/* Fe AE */
	static_blc->stFeAeBlc.blc_in = GK_FALSE;
	/* RC */
	static_blc->stRcBlc.blc_in = GK_FALSE;
	static_blc->stRcBlc.blc_out = GK_FALSE;
	/* FE BLC */
	static_blc->stFeBlc.blc_in = GK_TRUE;

	static_blc->bReshStatic = GK_TRUE;
}

static GK_VOID BlcRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 i, u8WdrMode;
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);

	u8WdrMode = pstIspCtx->u8SnsWDRMode;

	BalanceBlackLevel(pstBlackLevel);

	/* BE */
	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		BE_BlcDynaRegs(ViPipe, u8WdrMode,
			       &pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.dyna_blc,
			       pstBlackLevel);
		BE_BlcStaticRegs(
			u8WdrMode,
			&pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.static_blc);
		pstRegCfg->stAlgRegCfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 =
			pstRegCfg->stAlgRegCfg[i]
				.stBeBlcCfg.dyna_blc.stWdrBlc[0]
				.blc[1];
		pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.resh_dyna_init = GK_TRUE;
	}
	pstRegCfg->unKey.bit1BeBlcCfg = 1;

	/* FE */
	FE_BlcDynaRegs(&pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.dyna_blc,
		       pstBlackLevel);
	FE_BlcStaticRegs(&pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.static_blc);
	pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.resh_dyna_init = GK_TRUE;
	pstRegCfg->unKey.bit1FeBlcCfg = 1;
}

static GK_S32 BlcReadExtregs(VI_PIPE ViPipe)
{
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;
	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);

	pstBlackLevel->u8BlackLevelChange =
		ext_system_black_level_change_read(ViPipe);
	ext_system_black_level_change_write(ViPipe, GK_FALSE);

	if (pstBlackLevel->u8BlackLevelChange) {
		pstBlackLevel->enOpType =
			ext_system_black_level_manual_mode_read(ViPipe);
		pstBlackLevel->au16BlackLevel[0] =
			ext_system_black_level_00_read(ViPipe);
		pstBlackLevel->au16BlackLevel[1] =
			ext_system_black_level_01_read(ViPipe);
		pstBlackLevel->au16BlackLevel[2] =
			ext_system_black_level_10_read(ViPipe);
		pstBlackLevel->au16BlackLevel[3] =
			ext_system_black_level_11_read(ViPipe);
	}

	return GK_SUCCESS;
}

static GK_VOID BlcWriteActualValue(VI_PIPE ViPipe, GK_U16 *pu16BlackLevel)
{
	ext_system_black_level_query_00_write(ViPipe, pu16BlackLevel[0]);
	ext_system_black_level_query_01_write(ViPipe, pu16BlackLevel[1]);
	ext_system_black_level_query_10_write(ViPipe, pu16BlackLevel[2]);
	ext_system_black_level_query_11_write(ViPipe, pu16BlackLevel[3]);
}

static GK_S32 ISP_BlcProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	ISP_CTRL_PROC_WRITE_S stProcTmp;
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;

	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);

	if ((pstProc->pcProcBuff == GK_NULL) || (pstProc->u32BuffLen == 0)) {
		return GK_FAILURE;
	}

	stProcTmp.pcProcBuff = pstProc->pcProcBuff;
	stProcTmp.u32BuffLen = pstProc->u32BuffLen;

	ISP_PROC_PRINTF(
		&stProcTmp, pstProc->u32WriteLen,
		"-----Black Level Actual INFO--------------------------------------------------------------\n");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%16s"
			"%16s"
			"%16s"
			"%16s\n",
			"BlcR", "BlcGr", "BlcGb", "BlcB");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%16u"
			"%16u"
			"%16u"
			"%16u\n",
			pstBlackLevel->au16ActualBlackLevel[0],
			pstBlackLevel->au16ActualBlackLevel[1],
			pstBlackLevel->au16ActualBlackLevel[2],
			pstBlackLevel->au16ActualBlackLevel[3]);

	pstProc->u32WriteLen += 1;

	return GK_SUCCESS;
}

GK_BOOL CheckWDRState(VI_PIPE ViPipe, ISP_CTX_S *pstIspCtx,
		      ISP_BLACKLEVEL_S *pstBlackLevel)
{
	GK_U8 bWDREn;
	GK_BOOL bWDRStateChange;

	bWDREn = ext_system_wdr_en_read(ViPipe);

	if (IS_2to1_WDR_MODE(pstIspCtx->u8SnsWDRMode)) {
		bWDRStateChange = (pstBlackLevel->u8WDRModeState == bWDREn) ?
					  GK_FALSE :
					  GK_TRUE;
	} else {
		bWDRStateChange = GK_FALSE;
	}

	pstBlackLevel->u8WDRModeState = bWDREn;

	return bWDRStateChange;
}

static GK_S32 ISP_BlcWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_U8 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;
	ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);
	ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

	for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
		pstBlackLevel->au16BlackLevel[i] =
			pstSnsBlackLevel->au16BlackLevel[i];
	}

	BalanceBlackLevel(pstBlackLevel);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		BE_BlcDynaRegs(ViPipe, pstIspCtx->u8SnsWDRMode,
			       &pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.dyna_blc,
			       pstBlackLevel);
		pstRegCfg->stAlgRegCfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 =
			pstRegCfg->stAlgRegCfg[i]
				.stBeBlcCfg.dyna_blc.stWdrBlc[0]
				.blc[1];
		pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.resh_dyna_init = GK_TRUE;
	}

	FE_BlcDynaRegs(&pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.dyna_blc,
		       pstBlackLevel);
	BlcExtRegsInitialize(ViPipe);

	pstRegCfg->unKey.bit1BeBlcCfg = 1;
	pstRegCfg->unKey.bit1FeBlcCfg = 1;
	pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.resh_dyna_init = GK_TRUE;
	pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.dyna_blc.bReshDyna = GK_TRUE;
	pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.static_blc.bReshStatic = GK_TRUE;

	return GK_SUCCESS;
}

GK_S32 ISP_BlcInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	BlcInitialize(ViPipe);
	BlcRegsInitialize(ViPipe, pstRegCfg);
	BlcExtRegsInitialize(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_BlcRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
		  GK_S32 s32Rsv)
{
	GK_U8 i;
	GK_BOOL bWDRStateChange;
	VI_PIPE MainPipe = 0;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;
	ISP_BLACKLEVEL_S *pstBlackLevel = GK_NULL;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	BLACKLEVEL_GET_CTX(ViPipe, pstBlackLevel);

	bWDRStateChange = CheckWDRState(ViPipe, pstIspCtx, pstBlackLevel);

	if (ext_system_dpc_static_defect_type_read(ViPipe) == 0) { // hot pixel
		if (pstIspCtx->stLinkage.bDefectPixel) {
			if (pstBlackLevel->bPreDefectPixel == GK_FALSE) {
				for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
					pstRegCfg->stAlgRegCfg[i]
						.stBeBlcCfg.static_blc.stWbBlc
						.blc_in = GK_TRUE;
					pstRegCfg->stAlgRegCfg[i]
						.stBeBlcCfg.static_blc
						.bReshStatic = GK_TRUE;
				}
			}
		} else if (pstBlackLevel->bPreDefectPixel) {
			for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
				pstRegCfg->stAlgRegCfg[i]
					.stBeBlcCfg.static_blc.stWbBlc.blc_in =
					GK_FALSE;
				pstRegCfg->stAlgRegCfg[i]
					.stBeBlcCfg.static_blc.bReshStatic =
					GK_TRUE;
			}
		}

		pstBlackLevel->bPreDefectPixel =
			pstIspCtx->stLinkage.bDefectPixel;
	}

	BlcReadExtregs(ViPipe);

	pstRegCfg->unKey.bit1FeBlcCfg = 1;
	pstRegCfg->unKey.bit1BeBlcCfg = 1;

	if (pstBlackLevel->enOpType == OP_TYPE_MANUAL) {
		BalanceBlackLevel(pstBlackLevel);
		FE_BlcDynaRegs(&pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.dyna_blc,
			       pstBlackLevel);

		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			BE_BlcDynaRegs(
				ViPipe, pstIspCtx->u8SnsWDRMode,
				&pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.dyna_blc,
				pstBlackLevel);
			pstRegCfg->stAlgRegCfg[i]
				.stWdrRegCfg.stSyncRegCfg.u16Offset0 =
				pstRegCfg->stAlgRegCfg[i]
					.stBeBlcCfg.dyna_blc.stWdrBlc[0]
					.blc[1];
		}

		BlcWriteActualValue(ViPipe,
				    pstBlackLevel->au16ActualBlackLevel);

		return GK_SUCCESS;
	}

	/* some sensors's blacklevel is changed with iso. */
	ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

	/* sensors's blacklevel is changed by cmos. */
	if (pstSnsBlackLevel->bUpdate == GK_TRUE) {
		if (pstIspCtx->stLinkage.enSnapPipeMode != ISP_SNAP_NONE) {
			if (ViPipe == pstIspCtx->stLinkage.s32PicturePipeId) {
				MainPipe =
					pstIspCtx->stLinkage.s32PreviewPipeId;
				ISP_CHECK_PIPE(MainPipe);
				ISP_SensorGetBlc(MainPipe, &pstSnsBlackLevel);
			} else {
				ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);
			}

			memcpy(&pstBlackLevel->au16BlackLevel[0],
			       &pstSnsBlackLevel->au16BlackLevel[0],
			       ISP_BAYER_CHN_NUM * sizeof(GK_U16));
		}

		ISP_SensorUpdateBlc(ViPipe);
		memcpy(&pstBlackLevel->au16BlackLevel[0],
		       &pstSnsBlackLevel->au16BlackLevel[0],
		       ISP_BAYER_CHN_NUM * sizeof(GK_U16));

		BalanceBlackLevel(pstBlackLevel);

		FE_BlcDynaRegs(&pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.dyna_blc,
			       pstBlackLevel);

		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			BE_BlcDynaRegs(
				ViPipe, pstIspCtx->u8SnsWDRMode,
				&pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.dyna_blc,
				pstBlackLevel);
			pstRegCfg->stAlgRegCfg[i]
				.stWdrRegCfg.stSyncRegCfg.u16Offset0 =
				pstRegCfg->stAlgRegCfg[i]
					.stBeBlcCfg.dyna_blc.stWdrBlc[0]
					.blc[1];
		}

		BlcWriteActualValue(ViPipe,
				    pstBlackLevel->au16ActualBlackLevel);

		return GK_SUCCESS;
	}

	if (((pstBlackLevel->u8BlackLevelChange == GK_TRUE) &&
	     (pstBlackLevel->enOpType == OP_TYPE_AUTO)) ||
	    (bWDRStateChange == GK_TRUE)) {
		ISP_SensorUpdateBlc(ViPipe);
		memcpy(&pstBlackLevel->au16BlackLevel[0],
		       &pstSnsBlackLevel->au16BlackLevel[0],
		       ISP_BAYER_CHN_NUM * sizeof(GK_U16));

		BalanceBlackLevel(pstBlackLevel);

		FE_BlcDynaRegs(&pstRegCfg->stAlgRegCfg[0].stFeBlcCfg.dyna_blc,
			       pstBlackLevel);

		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			BE_BlcDynaRegs(
				ViPipe, pstIspCtx->u8SnsWDRMode,
				&pstRegCfg->stAlgRegCfg[i].stBeBlcCfg.dyna_blc,
				pstBlackLevel);
			pstRegCfg->stAlgRegCfg[i]
				.stWdrRegCfg.stSyncRegCfg.u16Offset0 =
				pstRegCfg->stAlgRegCfg[i]
					.stBeBlcCfg.dyna_blc.stWdrBlc[0]
					.blc[1];
		}

		BlcWriteActualValue(ViPipe,
				    pstBlackLevel->au16ActualBlackLevel);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_BlcCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	switch (u32Cmd) {
	case ISP_CHANGE_IMAGE_MODE_SET:
	case ISP_WDR_MODE_SET:
		ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
		ISP_CHECK_POINTER(pRegCfg);
		ISP_BlcWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
		break;
	case ISP_PROC_WRITE:
		ISP_BlcProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
		break;
	default:
		break;
	}
	return GK_SUCCESS;
}

GK_S32 ISP_BlcExit(VI_PIPE ViPipe)
{
	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterBlc(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Blc);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_BLC;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_BlcInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_BlcRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_BlcCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_BlcExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
