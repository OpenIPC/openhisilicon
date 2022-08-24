/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include <math.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static const GK_U8 g_au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM] = {
	8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0
};
static const GK_U8 g_au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM] = {
	8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0
};

typedef struct ISP_ANTIFALSECOLOR_S {
	GK_BOOL bEnable; /* RW;Range:[0x0,0x1];Format:1.0; AntiFalseColor Enable */
	GK_U8 u8WdrMode;
	GK_U8 u8ProcThreshold;
	GK_U8 u8ProcStrength;
	GK_U8 au8LutAntiFalseColorGrayRatio[ISP_AUTO_ISO_STRENGTH_NUM]; // u5.0,
	GK_U8 au8LutAntiFalseColorCmaxSel[ISP_AUTO_ISO_STRENGTH_NUM]; // u4.0,
	GK_U8 au8LutAntiFalseColorDetgSel[ISP_AUTO_ISO_STRENGTH_NUM]; // u4.0,
	GK_U16 au16AntiFalseColorHfThreshLow[ISP_AUTO_ISO_STRENGTH_NUM]; // 10.0,
	GK_U16 au16AntiFalseColorHfThreshHig[ISP_AUTO_ISO_STRENGTH_NUM]; // 10.0,

	ISP_OP_TYPE_E enOpType;
	ISP_ANTIFALSECOLOR_AUTO_ATTR_S stAuto;
	ISP_ANTIFALSECOLOR_MANUAL_ATTR_S stManual;
} ISP_ANTIFALSECOLOR_S;

ISP_ANTIFALSECOLOR_S *g_pastAntiFalseColorCtx[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define ANTIFALSECOLOR_GET_CTX(dev, pstCtx) \
	(pstCtx = g_pastAntiFalseColorCtx[dev])
#define ANTIFALSECOLOR_SET_CTX(dev, pstCtx) \
	(g_pastAntiFalseColorCtx[dev] = pstCtx)
#define ANTIFALSECOLOR_RESET_CTX(dev) (g_pastAntiFalseColorCtx[dev] = GK_NULL)

GK_S32 AntiFalseColorCtxInit(VI_PIPE ViPipe)
{
	ISP_ANTIFALSECOLOR_S *pastAntiFalseColorCtx = GK_NULL;

	ANTIFALSECOLOR_GET_CTX(ViPipe, pastAntiFalseColorCtx);

	if (pastAntiFalseColorCtx == GK_NULL) {
		pastAntiFalseColorCtx = (ISP_ANTIFALSECOLOR_S *)ISP_MALLOC(
			sizeof(ISP_ANTIFALSECOLOR_S));
		if (pastAntiFalseColorCtx == GK_NULL) {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"Isp[%d] AntiFalseColorCtx malloc memory failed!\n",
				ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pastAntiFalseColorCtx, 0, sizeof(ISP_ANTIFALSECOLOR_S));

	ANTIFALSECOLOR_SET_CTX(ViPipe, pastAntiFalseColorCtx);

	return GK_SUCCESS;
}

GK_VOID AntiFalseColorCtxExit(VI_PIPE ViPipe)
{
	ISP_ANTIFALSECOLOR_S *pastAntiFalseColorCtx = GK_NULL;

	ANTIFALSECOLOR_GET_CTX(ViPipe, pastAntiFalseColorCtx);
	ISP_FREE(pastAntiFalseColorCtx);
	ANTIFALSECOLOR_RESET_CTX(ViPipe);
}

static GK_VOID AntiFalseColorInitFwWdr(VI_PIPE ViPipe)
{
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;

	GK_U8 au8LutAntiFalseColorGrayRatio[ISP_AUTO_ISO_STRENGTH_NUM] = {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
	};
	GK_U8 au8LutAntiFalseColorCmaxSel[ISP_AUTO_ISO_STRENGTH_NUM] = {
		4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
	};
	GK_U8 au8LutAntiFalseColorDetgSel[ISP_AUTO_ISO_STRENGTH_NUM] = {
		4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	GK_U16 au16AntiFalseColorHfThreshLow[ISP_AUTO_ISO_STRENGTH_NUM] = {
		96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96
	};
	GK_U16 au16AntiFalseColorHfThreshHig[ISP_AUTO_ISO_STRENGTH_NUM] = {
		256, 256, 256, 256, 256, 256, 256, 256,
		256, 256, 256, 256, 256, 256, 256, 256
	};

	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

	memcpy(pstAntiFalseColor->au8LutAntiFalseColorGrayRatio,
	       au8LutAntiFalseColorGrayRatio,
	       sizeof(GK_U8) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au8LutAntiFalseColorCmaxSel,
	       au8LutAntiFalseColorCmaxSel,
	       sizeof(GK_U8) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au8LutAntiFalseColorDetgSel,
	       au8LutAntiFalseColorDetgSel,
	       sizeof(GK_U8) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshLow,
	       au16AntiFalseColorHfThreshLow,
	       sizeof(GK_U16) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshHig,
	       au16AntiFalseColorHfThreshHig,
	       sizeof(GK_U16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static GK_VOID AntiFalseColorInitFwLinear(VI_PIPE ViPipe)
{
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;

	GK_U8 au8LutAntiFalseColorGrayRatio[ISP_AUTO_ISO_STRENGTH_NUM] = {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
	};
	GK_U8 au8LutAntiFalseColorCmaxSel[ISP_AUTO_ISO_STRENGTH_NUM] = {
		4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
	};
	GK_U8 au8LutAntiFalseColorDetgSel[ISP_AUTO_ISO_STRENGTH_NUM] = {
		4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	GK_U16 au16AntiFalseColorHfThreshLow[ISP_AUTO_ISO_STRENGTH_NUM] = {
		30, 30, 35, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48
	};
	GK_U16 au16AntiFalseColorHfThreshHig[ISP_AUTO_ISO_STRENGTH_NUM] = {
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128
	};

	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

	memcpy(pstAntiFalseColor->au8LutAntiFalseColorGrayRatio,
	       au8LutAntiFalseColorGrayRatio,
	       sizeof(GK_U8) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au8LutAntiFalseColorCmaxSel,
	       au8LutAntiFalseColorCmaxSel,
	       sizeof(GK_U8) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au8LutAntiFalseColorDetgSel,
	       au8LutAntiFalseColorDetgSel,
	       sizeof(GK_U8) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshLow,
	       au16AntiFalseColorHfThreshLow,
	       sizeof(GK_U16) * ISP_AUTO_ISO_STRENGTH_NUM);
	memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshHig,
	       au16AntiFalseColorHfThreshHig,
	       sizeof(GK_U16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static GK_S32 AntiFalseColor_SetLongFrameMode(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode) ||
	    (pstIspCtx->stLinkage.enFSWDRMode == ISP_FSWDR_LONG_FRAME_MODE) ||
	    (pstIspCtx->stLinkage.enFSWDRMode ==
	     ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
		AntiFalseColorInitFwLinear(ViPipe);
	} else {
		AntiFalseColorInitFwWdr(ViPipe);
	}

	return GK_SUCCESS;
}

static GK_S32
AntiFalseColorCheckCmosParam(VI_PIPE ViPipe,
			     const ISP_CMOS_ANTIFALSECOLOR_S *pstAntiFalseColor)
{
	GK_U8 i;

	ISP_CHECK_BOOL(pstAntiFalseColor->bEnable);

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
		if (pstAntiFalseColor->au8AntiFalseColorThreshold[i] > 0x20) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err au8AntiFalseColorThreshold[%d]!\n", i);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstAntiFalseColor->au8AntiFalseColorStrength[i] > 0x1f) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err au8AntiFalseColorStrength[%d]!\n", i);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}
	}

	return GK_SUCCESS;
}

static GK_S32 AntiFalseColorExtRegsInitialize(VI_PIPE ViPipe)
{
	GK_U32 i;
	GK_S32 s32Ret;
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_SensorGetDefault(ViPipe, &pstSnsDft);
	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

	pstAntiFalseColor->u8WdrMode = pstIspCtx->u8SnsWDRMode;

	if (pstAntiFalseColor->u8WdrMode != 0) {
		AntiFalseColorInitFwWdr(ViPipe);
	} else {
		AntiFalseColorInitFwLinear(ViPipe);
	}

	ext_system_antifalsecolor_manual_mode_write(
		ViPipe, ISP_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_MODE_DEFAULT);

	pstAntiFalseColor->stManual.u8AntiFalseColorThreshold =
		ISP_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_THRESHOLD_DEFAULT;
	pstAntiFalseColor->stManual.u8AntiFalseColorStrength =
		ISP_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_STRENGTH_DEFAULT;

	ext_system_antifalsecolor_manual_threshold_write(
		ViPipe, pstAntiFalseColor->stManual.u8AntiFalseColorThreshold);
	ext_system_antifalsecolor_manual_strenght_write(
		ViPipe, pstAntiFalseColor->stManual.u8AntiFalseColorStrength);

	if (pstSnsDft->unKey.bit1AntiFalseColor) {
		ISP_CHECK_POINTER(pstSnsDft->pstAntiFalseColor);

		s32Ret = AntiFalseColorCheckCmosParam(
			ViPipe, pstSnsDft->pstAntiFalseColor);
		if (s32Ret != GK_SUCCESS) {
			return s32Ret;
		}

		ext_system_antifalsecolor_enable_write(
			ViPipe, pstSnsDft->pstAntiFalseColor->bEnable);

		memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold,
		       pstSnsDft->pstAntiFalseColor->au8AntiFalseColorThreshold,
		       ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U8));
		memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorStrength,
		       pstSnsDft->pstAntiFalseColor->au8AntiFalseColorStrength,
		       ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U8));
	} else {
		ext_system_antifalsecolor_enable_write(
			ViPipe, ISP_EXT_SYSTEM_ANTIFALSECOLOR_ENABLE_DEFAULT);
		memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold,
		       g_au8AntiFalseColorThreshold,
		       ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U8));
		memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorStrength,
		       g_au8AntiFalseColorStrength,
		       ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U8));
	}

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
		ext_system_antifalsecolor_auto_threshold_write(
			ViPipe, i,
			pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i]);
		ext_system_antifalsecolor_auto_strenght_write(
			ViPipe, i,
			pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i]);
	}

	return GK_SUCCESS;
}

static GK_VOID AntiFalseColorRegsInitialize(VI_PIPE ViPipe,
					    ISP_REG_CFG_S *pstRegCfg)
{
	GK_U32 i;
	ISP_ANTIFALSECOLOR_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_ANTIFALSECOLOR_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstRegCfg->stAlgRegCfg[i].stAntiFalseColorRegCfg.bFcrEnable =
			ISP_DEMOSAIC_FCR_EN_DEFAULT;
		pstStaticRegCfg =
			&pstRegCfg->stAlgRegCfg[i]
				 .stAntiFalseColorRegCfg.stStaticRegCfg;
		pstDynaRegCfg = &pstRegCfg->stAlgRegCfg[i]
					 .stAntiFalseColorRegCfg.stDynaRegCfg;

		// static
		pstStaticRegCfg->bResh = GK_TRUE;
		pstStaticRegCfg->u16FcrLimit1 = ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT;
		pstStaticRegCfg->u16FcrLimit2 = ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT;
		pstStaticRegCfg->u16FcrThr = ISP_DEMOSAIC_FCR_THR_DEFAULT;

		/* dynamic */
		pstDynaRegCfg->bResh = GK_TRUE;
		pstDynaRegCfg->u8FcrGain = ISP_DEMOSAIC_FCR_GAIN_DEFAULT;
		pstDynaRegCfg->u8FcrRatio = ISP_DEMOSAIC_FCR_RATIO_DEFAULT;
		pstDynaRegCfg->u8FcrGrayRatio =
			ISP_DEMOSAIC_FCR_GRAY_RATIO_DEFAULT;
		pstDynaRegCfg->u8FcrCmaxSel = ISP_DEMOSAIC_FCR_CMAX_SEL_DEFAULT;
		pstDynaRegCfg->u8FcrDetgSel = ISP_DEMOSAIC_FCR_DETG_SEL_DEFAULT;
		pstDynaRegCfg->u16FcrHfThreshLow =
			ISP_DEMOSAIC_FCR_HF_THRESH_LOW_DEFAULT;
		pstDynaRegCfg->u16FcrHfThreshHig =
			ISP_DEMOSAIC_FCR_HF_THRESH_HIGH_DEFAULT;
	}

	pstRegCfg->unKey.bit1FcrCfg = 1;
}

static GK_S32 AntiFalseColorReadExtregs(VI_PIPE ViPipe)
{
	GK_U32 i;
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;

	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

	pstAntiFalseColor->enOpType =
		ext_system_antifalsecolor_manual_mode_read(ViPipe);

	if (pstAntiFalseColor->enOpType == OP_TYPE_AUTO) {
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
			pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i] =
				ext_system_antifalsecolor_auto_threshold_read(
					ViPipe, i);
			pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i] =
				ext_system_antifalsecolor_auto_strenght_read(
					ViPipe, i);
		}
	} else if (pstAntiFalseColor->enOpType == OP_TYPE_MANUAL) {
		pstAntiFalseColor->stManual.u8AntiFalseColorThreshold =
			ext_system_antifalsecolor_manual_threshold_read(ViPipe);
		pstAntiFalseColor->stManual.u8AntiFalseColorStrength =
			ext_system_antifalsecolor_manual_strenght_read(ViPipe);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_AntiFalseColorInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_S32 s32Ret = GK_SUCCESS;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	s32Ret = AntiFalseColorCtxInit(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	AntiFalseColorRegsInitialize(ViPipe, pstRegCfg);
	s32Ret = AntiFalseColorExtRegsInitialize(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

static GK_BOOL __inline CheckAntiFalseColorOpen(
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor)
{
	return (pstAntiFalseColor->bEnable == GK_TRUE);
}

static GK_VOID
AntiFalseColorCfg(ISP_ANTIFALSECOLOR_DYNA_CFG_S *pstAntiFalseColorDynaCfg,
		  ISP_ANTIFALSECOLOR_S *pstAntiFalseColor, GK_U32 u32Iso,
		  GK_U32 u32ISO2, GK_U32 u32ISO1, GK_U8 u8IsoIndexUpper,
		  GK_U8 u8IsoIndexLower)
{
	pstAntiFalseColorDynaCfg->u8FcrGrayRatio = (GK_U8)LinearInter(
		u32Iso, u32ISO1,
		pstAntiFalseColor
			->au8LutAntiFalseColorGrayRatio[u8IsoIndexLower],
		u32ISO2,
		pstAntiFalseColor
			->au8LutAntiFalseColorGrayRatio[u8IsoIndexUpper]);
	pstAntiFalseColorDynaCfg->u8FcrCmaxSel = (GK_U8)LinearInter(
		u32Iso, u32ISO1,
		pstAntiFalseColor->au8LutAntiFalseColorCmaxSel[u8IsoIndexLower],
		u32ISO2,
		pstAntiFalseColor->au8LutAntiFalseColorCmaxSel[u8IsoIndexUpper]);
	pstAntiFalseColorDynaCfg->u8FcrDetgSel = (GK_U8)LinearInter(
		u32Iso, u32ISO1,
		pstAntiFalseColor->au8LutAntiFalseColorDetgSel[u8IsoIndexLower],
		u32ISO2,
		pstAntiFalseColor->au8LutAntiFalseColorDetgSel[u8IsoIndexUpper]);
	pstAntiFalseColorDynaCfg->u16FcrHfThreshLow = (GK_U16)LinearInter(
		u32Iso, u32ISO1,
		pstAntiFalseColor
			->au16AntiFalseColorHfThreshLow[u8IsoIndexLower],
		u32ISO2,
		pstAntiFalseColor
			->au16AntiFalseColorHfThreshLow[u8IsoIndexUpper]);
	pstAntiFalseColorDynaCfg->u16FcrHfThreshHig = (GK_U16)LinearInter(
		u32Iso, u32ISO1,
		pstAntiFalseColor
			->au16AntiFalseColorHfThreshHig[u8IsoIndexLower],
		u32ISO2,
		pstAntiFalseColor
			->au16AntiFalseColorHfThreshHig[u8IsoIndexUpper]);
	pstAntiFalseColorDynaCfg->bResh = GK_TRUE;
}

static GK_S32 ISP_AntiFalseColor_Fw(GK_U32 u32Iso, VI_PIPE ViPipe,
				    GK_U8 u8CurBlk, ISP_REG_CFG_S *pstReg)
{
	GK_U8 u8IsoIndexUpper, u8IsoIndexLower;
	GK_U32 u32ISO1 = 0;
	GK_U32 u32ISO2 = 0;
	ISP_ANTIFALSECOLOR_DYNA_CFG_S *pstAntiFalseColorDynaCfg =
		&pstReg->stAlgRegCfg[u8CurBlk]
			 .stAntiFalseColorRegCfg.stDynaRegCfg;
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;

	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

	u8IsoIndexUpper = GetIsoIndex(u32Iso);
	u8IsoIndexLower = MAX2((GK_S8)u8IsoIndexUpper - 1, 0);
	u32ISO1 = g_au32IsoLut[u8IsoIndexLower];
	u32ISO2 = g_au32IsoLut[u8IsoIndexUpper];

	AntiFalseColorCfg(pstAntiFalseColorDynaCfg, pstAntiFalseColor, u32Iso,
			  u32ISO2, u32ISO1, u8IsoIndexUpper, u8IsoIndexLower);

	if (pstAntiFalseColor->enOpType == OP_TYPE_AUTO) {
		pstAntiFalseColorDynaCfg->u8FcrRatio = (GK_U8)LinearInter(
			u32Iso, u32ISO1,
			pstAntiFalseColor->stAuto
				.au8AntiFalseColorThreshold[u8IsoIndexLower],
			u32ISO2,
			pstAntiFalseColor->stAuto
				.au8AntiFalseColorThreshold[u8IsoIndexUpper]);
		pstAntiFalseColorDynaCfg->u8FcrGain = (GK_U8)LinearInter(
			u32Iso, u32ISO1,
			pstAntiFalseColor->stAuto
				.au8AntiFalseColorStrength[u8IsoIndexLower],
			u32ISO2,
			pstAntiFalseColor->stAuto
				.au8AntiFalseColorStrength[u8IsoIndexUpper]);
	} else if (pstAntiFalseColor->enOpType == OP_TYPE_MANUAL) {
		pstAntiFalseColorDynaCfg->u8FcrRatio =
			pstAntiFalseColor->stManual.u8AntiFalseColorThreshold;
		pstAntiFalseColorDynaCfg->u8FcrGain =
			pstAntiFalseColor->stManual.u8AntiFalseColorStrength;
	}
	pstAntiFalseColor->u8ProcThreshold =
		pstAntiFalseColorDynaCfg->u8FcrRatio;
	pstAntiFalseColor->u8ProcStrength = pstAntiFalseColorDynaCfg->u8FcrGain;
	pstAntiFalseColorDynaCfg->bResh = GK_TRUE;

	return GK_SUCCESS;
}

GK_S32 ISP_AntiFalseColorRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
			     GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
	GK_U8 i;
	ISP_REG_CFG_S *pstReg = (ISP_REG_CFG_S *)pRegCfg;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);
	ISP_CHECK_POINTER(pstAntiFalseColor);

	if (pstIspCtx->stLinkage.enFSWDRMode !=
	    pstIspCtx->stLinkage.enPreFSWDRMode) {
		AntiFalseColor_SetLongFrameMode(ViPipe);
	}

	/* calculate every two interrupts */
	if ((pstIspCtx->u32FrameCnt % 2 != 0) &&
	    (pstIspCtx->stLinkage.bSnapState != GK_TRUE)) {
		return GK_SUCCESS;
	}

	pstAntiFalseColor->bEnable =
		ext_system_antifalsecolor_enable_read(ViPipe);

	for (i = 0; i < pstReg->u8CfgNum; i++) {
		pstReg->stAlgRegCfg[i].stAntiFalseColorRegCfg.bFcrEnable =
			pstAntiFalseColor->bEnable;
	}

	pstReg->unKey.bit1FcrCfg = 1;

	/* check hardware setting */
	if (!CheckAntiFalseColorOpen(pstAntiFalseColor)) {
		return GK_SUCCESS;
	}

	AntiFalseColorReadExtregs(ViPipe);

	for (i = 0; i < pstReg->u8CfgNum; i++) {
		ISP_AntiFalseColor_Fw(pstIspCtx->stLinkage.u32Iso, ViPipe, i,
				      pstReg);
	}

	return GK_SUCCESS;
}

GK_S32 AntiFalseColorProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	ISP_CTRL_PROC_WRITE_S stProcTmp;
	ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = GK_NULL;

	ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);
	ISP_CHECK_POINTER(pstAntiFalseColor);

	if ((pstProc->pcProcBuff == GK_NULL) || (pstProc->u32BuffLen == 0)) {
		return GK_FAILURE;
	}

	stProcTmp.pcProcBuff = pstProc->pcProcBuff;
	stProcTmp.u32BuffLen = pstProc->u32BuffLen;

	ISP_PROC_PRINTF(
		&stProcTmp, pstProc->u32WriteLen,
		"-----AntiFalseColor INFO-------------------------------------------------------------\n");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%12s"
			"%12s"
			"%12s\n",
			"Enable", "Threshold", "Strength");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%12u"
			"%12u"
			"%12u\n",
			pstAntiFalseColor->bEnable,
			pstAntiFalseColor->u8ProcThreshold,
			pstAntiFalseColor->u8ProcStrength);

	pstProc->u32WriteLen += 1;

	return GK_SUCCESS;
}

GK_S32 ISP_AntiFalseColorCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	switch (u32Cmd) {
	case ISP_PROC_WRITE:
		AntiFalseColorProcWrite(ViPipe,
					(ISP_CTRL_PROC_WRITE_S *)pValue);
		break;

	default:
		break;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_AntiFalseColorExit(VI_PIPE ViPipe)
{
	GK_U16 i;
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

	for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
		pRegCfg->stRegCfg.stAlgRegCfg[i]
			.stAntiFalseColorRegCfg.bFcrEnable = GK_FALSE;
	}

	pRegCfg->stRegCfg.unKey.bit1FcrCfg = 1;

	AntiFalseColorCtxExit(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterFcr(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Fcr);

	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_ANTIFALSECOLOR;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_AntiFalseColorInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_AntiFalseColorRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AntiFalseColorCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AntiFalseColorExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
