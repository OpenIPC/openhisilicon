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
// Lobal CAC,default value
#define ISP_LCAC_LUMA_THR_R_WDR (1500)
#define ISP_LCAC_LUMA_THR_G_WDR (1500)
#define ISP_LCAC_LUMA_THR_B_WDR (2150)

#define ISP_LCAC_LUMA_THR_R_LINEAR (1500)
#define ISP_LCAC_LUMA_THR_G_LINEAR (1500)
#define ISP_LCAC_LUMA_THR_B_LINEAR (3500)

#define ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR (3) /* [0,8] */
#define ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR (0) /* [0,8] */

#define ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR (7) /* [0,8] */
#define ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR (0) /* [0,8] */

#define ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR (292)
#define ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR (-50)

#define ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR (292)
#define ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR (-50)

#define ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_LINEAR (60)
#define ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_WDR (60)

#define ISP_LCAC_MAX_STRENGTH (8)
#define RANGE_MAX_VALUE (3)

static const GK_U32 g_au32ExpRatioLut[ISP_EXP_RATIO_STRENGTH_NUM] = {
	64,  128, 192, 256, 320, 384, 448, 512,
	576, 640, 704, 768, 832, 896, 960, 1023
};

static const GK_U8 g_au8WdrCbStrDefaultLut[ISP_EXP_RATIO_STRENGTH_NUM] = {
	0, 0, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};
static const GK_U8 g_au8WdrCrStrDefaultLut[ISP_EXP_RATIO_STRENGTH_NUM] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const GK_U8 g_au8LinearCbStrDefaultLut[ISP_EXP_RATIO_STRENGTH_NUM] = {
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};
static const GK_U8 g_au8LinearCrStrDefaultLut[ISP_EXP_RATIO_STRENGTH_NUM] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const GK_U8 g_au8DePurpStrLut[LCAC_STRENGTH_NUM] = { 0, 0, 0, 0, 0, 0,
							    0, 0, 0, 0, 0, 0,
							    0, 0, 0, 0 };

static const GK_U16 g_au16RLuma[RANGE_MAX_VALUE] = { 1500, 1500, 0 };
static const GK_U16 g_au16GLuma[RANGE_MAX_VALUE] = { 1500, 1500, 0 };
static const GK_U16 g_au16BLuma[RANGE_MAX_VALUE] = { 4095, 1500, 0 };
static const GK_U16 g_au16YLuma[RANGE_MAX_VALUE] = { 3200, 1500, 0 };
static const GK_S16 g_as16CBCRL[RANGE_MAX_VALUE] = { -50, -50, -50 };
static const GK_U16 g_au16PurpleDetRange[RANGE_MAX_VALUE] = { 0, 260, 410 };

typedef struct ISP_CAC_S {
	GK_BOOL bLocalCacEn;
	GK_BOOL bCacManualEn;
	GK_BOOL bCoefUpdateEn;
	GK_U8 au8DePurpleStr[LCAC_STRENGTH_NUM];
	GK_U8 au8DePurpleCbStr[ISP_EXP_RATIO_STRENGTH_NUM];
	GK_U8 au8DePurpleCrStr[ISP_EXP_RATIO_STRENGTH_NUM];
	GK_U8 u8DePurpleCbStr;
	GK_U8 u8DePurpleCrStr;
	GK_U8 u8LumaHighCntThr; // u8.0, [0,153]
	GK_U8 u8CbCntHighThr; // u7.0, [0,68]
	GK_U8 u8CbCntLowThr; // u7.0, [0,68]
	GK_U8 u8BldAvgCur; // u4.0, [0, 8]
	GK_U8 u8DeFColorCr;
	GK_U8 u8DeFColorCb;
	GK_U16 u16LumaThr; // u12.0, [0,4095]
	GK_U16 u16CbThr; // u12.0, [0,4095]
	GK_U16 u16PurpleDetRange;

	GK_U16 au16RLumaThd[ISP_LCAC_DET_NUM];
	GK_U16 au16GLumaThd[ISP_LCAC_DET_NUM];
	GK_U16 au16BLumaThd[ISP_LCAC_DET_NUM];
	GK_U16 au16YLumaThd[ISP_LCAC_DET_NUM];
	GK_S16 as16CbCrRatio[ISP_LCAC_DET_NUM];

	GK_U16 u16PurpleVarThr;
	GK_S32 ProcInfoDePurpleCtrCb; // [0, 8]
	GK_S32 ProcInfoDePurpleCtrCr; // [0, 8]

} ISP_LCAC_S;

ISP_LCAC_S *g_pastLocalCacCtx[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define LocalCAC_GET_CTX(dev, pstCtx) (pstCtx = g_pastLocalCacCtx[dev])
#define LocalCAC_SET_CTX(dev, pstCtx) (g_pastLocalCacCtx[dev] = pstCtx)
#define LocalCAC_RESET_CTX(dev) (g_pastLocalCacCtx[dev] = GK_NULL)

static GK_U32 LcacGetExpRatio(VI_PIPE ViPipe, GK_U8 u8CurBlk, GK_U8 u8WDRMode)
{
	GK_U32 u32ExpRatio = 0;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	u32ExpRatio = pstIspCtx->stLinkage.u32ExpRatio;

	if (IS_LINEAR_MODE(u8WDRMode)) {
		return 64;
	} else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)) {
		return 1023;
	} else if (IS_2to1_WDR_MODE(u8WDRMode)) {
		if ((pstIspCtx->stLinkage.enFSWDRMode ==
		     ISP_FSWDR_LONG_FRAME_MODE) ||
		    (pstIspCtx->stLinkage.enFSWDRMode ==
		     ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
			return 64;
		} else {
			return u32ExpRatio;
		}
	} else {
		return 64;
	}
}

GK_S32 LocalCacCtxInit(VI_PIPE ViPipe)
{
	ISP_LCAC_S *pastLocalCacCtx = GK_NULL;

	LocalCAC_GET_CTX(ViPipe, pastLocalCacCtx);

	if (pastLocalCacCtx == GK_NULL) {
		pastLocalCacCtx = (ISP_LCAC_S *)ISP_MALLOC(sizeof(ISP_LCAC_S));
		if (pastLocalCacCtx == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] LocalCacCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pastLocalCacCtx, 0, sizeof(ISP_LCAC_S));

	LocalCAC_SET_CTX(ViPipe, pastLocalCacCtx);

	return GK_SUCCESS;
}

GK_VOID LocalCacCtxExit(VI_PIPE ViPipe)
{
	ISP_LCAC_S *pastLocalCacCtx = GK_NULL;

	LocalCAC_GET_CTX(ViPipe, pastLocalCacCtx);
	ISP_FREE(pastLocalCacCtx);
	LocalCAC_RESET_CTX(ViPipe);
}

static GK_VOID LCacExtRegsInitialize(VI_PIPE ViPipe)
{
	GK_U8 i;
	GK_U8 u8WDRMode;
	ISP_CTX_S *pstIspCtx;
	ISP_GET_CTX(ViPipe, pstIspCtx);

	u8WDRMode = pstIspCtx->u8SnsWDRMode;
	ext_system_localCAC_manual_mode_enable_write(ViPipe, OP_TYPE_AUTO);
	ext_system_localCAC_enable_write(ViPipe, GK_TRUE);
	ext_system_LocalCAC_coef_update_en_write(ViPipe, GK_TRUE);
	ext_system_localCAC_purple_var_thr_write(
		ViPipe, ISP_LCAC_PURPLE_DET_THR_DEFAULT);
	ext_system_localCAC_lumagh_cnt_thr_write(
		ViPipe, ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR_DEFAULT);
	ext_system_localCAC_cb_cntgh_thr_write(
		ViPipe, ISP_DEMOSAIC_CAC_CB_CNT_HIGH_THR_DEFAULT);
	ext_system_localCAC_cb_cnt_low_thr_write(
		ViPipe, ISP_DEMOSAIC_CAC_CB_CNT_LOW_THR_DEFAULT);
	ext_system_localCAC_luma_thr_write(ViPipe,
					   ISP_DEMOSAIC_LUMA_THR_DEFAULT);
	ext_system_localCAC_cb_thr_write(ViPipe, ISP_DEMOSAIC_CB_THR_DEFAULT);
	ext_system_localCAC_bld_avg_cur_write(
		ViPipe, ISP_DEMOSAIC_CAC_BLD_AVG_CUR_DEFAULT);
	ext_system_localCAC_defcolor_cr_write(ViPipe,
					      ISP_DEMOSAIC_DEFCOLOR_CR_DEFAULT);
	ext_system_localCAC_defcolor_cb_write(ViPipe,
					      ISP_DEMOSAIC_DEFCOLOR_CB_DEFAULT);

	for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
		ext_system_localCAC_r_thd_table_write(ViPipe, i,
						      g_au16RLuma[i]);
		ext_system_localCAC_g_thd_table_write(ViPipe, i,
						      g_au16GLuma[i]);
		ext_system_localCAC_b_thd_table_write(ViPipe, i,
						      g_au16BLuma[i]);
		ext_system_localCAC_luma_thd_table_write(ViPipe, i,
							 g_au16YLuma[i]);
		ext_system_localCAC_cbcr_ratio_table_write(ViPipe, i,
							   g_as16CBCRL[i]);
	}

	/* linear mode */
	if (IS_LINEAR_MODE(u8WDRMode)) {
		for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
			ext_system_localCAC_auto_cb_str_table_write(
				ViPipe, i, g_au8LinearCbStrDefaultLut[i]);
			ext_system_localCAC_auto_cr_str_table_write(
				ViPipe, i, g_au8LinearCrStrDefaultLut[i]);
		}

		ext_system_localCAC_manual_cb_str_write(
			ViPipe, ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR);
		ext_system_localCAC_manual_cr_str_write(
			ViPipe, ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR);
		ext_system_localCAC_purple_det_range_write(
			ViPipe, ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_LINEAR);
	} else {
		/* WDR mode */
		for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
			ext_system_localCAC_auto_cb_str_table_write(
				ViPipe, i, g_au8WdrCbStrDefaultLut[i]);
			ext_system_localCAC_auto_cr_str_table_write(
				ViPipe, i, g_au8WdrCrStrDefaultLut[i]);
		}

		ext_system_localCAC_manual_cb_str_write(
			ViPipe, ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR);
		ext_system_localCAC_manual_cr_str_write(
			ViPipe, ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR);
		ext_system_localCAC_purple_det_range_write(
			ViPipe, ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_WDR);
	}
}

static GK_VOID
LCacStaticRegsInitialize(ISP_LOCAL_CAC_STATIC_CFG_S *pstLCacStaticRegCfg)
{
	GK_U8 i;

	pstLCacStaticRegCfg->bNddmCacBlendEn = GK_TRUE;
	pstLCacStaticRegCfg->u16NddmCacBlendRate =
		ISP_NDDM_CAC_BLEND_RATE_DEFAULT;
	pstLCacStaticRegCfg->u8RCounterThr = ISP_LCAC_COUNT_THR_R_DEFAULT;
	pstLCacStaticRegCfg->u8GCounterThr = ISP_LCAC_COUNT_THR_G_DEFAULT;
	pstLCacStaticRegCfg->u8BCounterThr = ISP_LCAC_COUNT_THR_B_DEFAULT;
	pstLCacStaticRegCfg->u16SatuThr = ISP_LCAC_SATU_THR_DEFAULT;
	// pstLCacStaticRegCfg->u16FakeCrVarThrHigh    = 300;
	// pstLCacStaticRegCfg->u16FakeCrVarThrLow     = 0;
	pstLCacStaticRegCfg->bCacCorMode = 0;
	for (i = 0; i < LCAC_STRENGTH_NUM; i++) {
		pstLCacStaticRegCfg->au8DePurpleStr[i] =
			ISP_LCAC_MAX_STRENGTH - g_au8DePurpStrLut[i];
	}

	pstLCacStaticRegCfg->bStaticResh = GK_TRUE;
}

static GK_VOID LCacUsrRegsInitialize(VI_PIPE ViPipe, GK_U8 u8WDRMode,
				     ISP_LOCAL_CAC_USR_CFG_S *pstLCacUsrRegCfg,
				     ISP_LCAC_S *pstLCacCtx)
{
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);

	pstLCacUsrRegCfg->bResh = GK_TRUE;
	pstLCacUsrRegCfg->u32UpdateIndex = 1;
	pstLCacUsrRegCfg->u8LumaHighCntThr =
		ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR_DEFAULT;
	pstLCacUsrRegCfg->u8CbCntHighThr =
		ISP_DEMOSAIC_CAC_CB_CNT_HIGH_THR_DEFAULT;
	pstLCacUsrRegCfg->u8CbCntLowThr =
		ISP_DEMOSAIC_CAC_CB_CNT_LOW_THR_DEFAULT;
	pstLCacUsrRegCfg->u8BldAvgCur = ISP_DEMOSAIC_CAC_BLD_AVG_CUR_DEFAULT;
	pstLCacUsrRegCfg->u16CbThr = ISP_DEMOSAIC_CB_THR_DEFAULT;
	//pstLCacUsrRegCfg->u8DeFColorCr     = ISP_DEMOSAIC_DEFCOLOR_CR_DEFAULT;
	//pstLCacUsrRegCfg->u8DeFColorCb     = ISP_DEMOSAIC_DEFCOLOR_CB_DEFAULT;

	if (IS_LINEAR_MODE(u8WDRMode)) {
		pstLCacUsrRegCfg->s16CbCrRatioLmtHigh =
			ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR;
	} else {
		pstLCacUsrRegCfg->s16CbCrRatioLmtHigh =
			ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR;
	}

	if (pstSnsDft->unKey.bit1Lcac) {
		pstLCacUsrRegCfg->u16VarThr = pstLCacCtx->u16PurpleVarThr;
	} else {
		pstLCacUsrRegCfg->u16VarThr = ISP_LCAC_PURPLE_DET_THR_DEFAULT;
	}
}

static GK_VOID
LCacDynaRegsInitialize(VI_PIPE ViPipe, GK_U8 u8WDRMode, GK_U8 u8CurBlk,
		       ISP_LOCAL_CAC_DYNA_CFG_S *pstLCacDynaRegCfg,
		       ISP_LCAC_S *pstLCacCtx)
{
	GK_U16 i;

	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);

	if (pstSnsDft->unKey.bit1Lcac) {
		// Loading from cmos has already distinguish WDR mode and non WDR mode
		// Converting cmos paramters to dynamic reg value
		GK_U16 u16PurpleDetRange = pstLCacCtx->u16PurpleDetRange;
		GK_S32 s32RangeLower, s32RangeUpper;
		GK_S32 s32RangeIdxUp, s32RangeIdxLow, s32RatioIdxUp,
			s32RatioIdxLow;
		GK_U8 DePurpleCtrCb, DePurpleCtrCr;

		s32RangeIdxUp = RANGE_MAX_VALUE - 1;
		for (i = 0; i < RANGE_MAX_VALUE; i++) {
			if (u16PurpleDetRange < g_au16PurpleDetRange[i]) {
				s32RangeIdxUp = i;
				break;
			}
		}

		s32RangeIdxLow = MAX2(s32RangeIdxUp - 1, 0);
		s32RangeUpper = g_au16PurpleDetRange[s32RangeIdxUp];
		s32RangeLower = g_au16PurpleDetRange[s32RangeIdxLow];

		GK_U16 u16RLumaThr = (GK_U16)LinearInter(
			u16PurpleDetRange, s32RangeLower,
			pstLCacCtx->au16RLumaThd[s32RangeIdxLow], s32RangeUpper,
			pstLCacCtx->au16RLumaThd[s32RangeIdxUp]);
		GK_U16 u16GLumaThr = (GK_U16)LinearInter(
			u16PurpleDetRange, s32RangeLower,
			pstLCacCtx->au16GLumaThd[s32RangeIdxLow], s32RangeUpper,
			pstLCacCtx->au16GLumaThd[s32RangeIdxUp]);
		GK_U16 u16BLumaThr = (GK_U16)LinearInter(
			u16PurpleDetRange, s32RangeLower,
			pstLCacCtx->au16BLumaThd[s32RangeIdxLow], s32RangeUpper,
			pstLCacCtx->au16BLumaThd[s32RangeIdxUp]);
		GK_U16 u16LumaThr = (GK_U16)LinearInter(
			u16PurpleDetRange, s32RangeLower,
			pstLCacCtx->au16YLumaThd[s32RangeIdxLow], s32RangeUpper,
			pstLCacCtx->au16YLumaThd[s32RangeIdxUp]);
		GK_S16 s16CbCrL =
			LinearInter(u16PurpleDetRange, s32RangeLower,
				    pstLCacCtx->as16CbCrRatio[s32RangeIdxLow],
				    s32RangeUpper,
				    pstLCacCtx->as16CbCrRatio[s32RangeIdxUp]);

		GK_U32 u32ExpRatio =
			LcacGetExpRatio(ViPipe, u8CurBlk, u8WDRMode);

		if (pstLCacCtx->bCacManualEn) {
			DePurpleCtrCb = pstLCacCtx->u8DePurpleCbStr;
			DePurpleCtrCr = pstLCacCtx->u8DePurpleCrStr;
		} else {
			s32RatioIdxUp = ISP_EXP_RATIO_STRENGTH_NUM - 1;
			for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
				if (u32ExpRatio <= g_au32ExpRatioLut[i]) {
					s32RatioIdxUp = i;
					break;
				}
			}

			s32RatioIdxLow = MAX2(s32RatioIdxUp - 1, 0);

			DePurpleCtrCb = LinearInter(
				u32ExpRatio, g_au32ExpRatioLut[s32RatioIdxLow],
				pstLCacCtx->au8DePurpleCbStr[s32RatioIdxLow],
				g_au32ExpRatioLut[s32RatioIdxUp],
				pstLCacCtx->au8DePurpleCbStr[s32RatioIdxUp]);
			DePurpleCtrCr = LinearInter(
				u32ExpRatio, g_au32ExpRatioLut[s32RatioIdxLow],
				pstLCacCtx->au8DePurpleCrStr[s32RatioIdxLow],
				g_au32ExpRatioLut[s32RatioIdxUp],
				pstLCacCtx->au8DePurpleCrStr[s32RatioIdxUp]);
		}

		pstLCacDynaRegCfg->u16RLumaThr = u16RLumaThr;
		pstLCacDynaRegCfg->u16GLumaThr = u16GLumaThr;
		pstLCacDynaRegCfg->u16BLumaThr = u16BLumaThr;
		pstLCacDynaRegCfg->u16LumaThr = u16LumaThr;
		pstLCacDynaRegCfg->s16CbCrRatioLmtLow = s16CbCrL;
		pstLCacDynaRegCfg->u8DePurpleCtrCb =
			ISP_LCAC_MAX_STRENGTH - DePurpleCtrCb;
		pstLCacDynaRegCfg->u8DePurpleCtrCr =
			ISP_LCAC_MAX_STRENGTH - DePurpleCtrCr;
	} else {
		if (IS_LINEAR_MODE(u8WDRMode)) {
			pstLCacDynaRegCfg->u16RLumaThr =
				ISP_LCAC_LUMA_THR_R_LINEAR;
			pstLCacDynaRegCfg->u16GLumaThr =
				ISP_LCAC_LUMA_THR_G_LINEAR;
			pstLCacDynaRegCfg->u16BLumaThr =
				ISP_LCAC_LUMA_THR_B_LINEAR;
			pstLCacDynaRegCfg->u8DePurpleCtrCb =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR;
			pstLCacDynaRegCfg->u8DePurpleCtrCr =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR;
			pstLCacDynaRegCfg->s16CbCrRatioLmtLow =
				ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR;
		} else {
			pstLCacDynaRegCfg->u16RLumaThr =
				ISP_LCAC_LUMA_THR_R_WDR;
			pstLCacDynaRegCfg->u16GLumaThr =
				ISP_LCAC_LUMA_THR_G_WDR;
			pstLCacDynaRegCfg->u16BLumaThr =
				ISP_LCAC_LUMA_THR_B_WDR;
			pstLCacDynaRegCfg->u8DePurpleCtrCb =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR;
			pstLCacDynaRegCfg->u8DePurpleCtrCr =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR;
			pstLCacDynaRegCfg->s16CbCrRatioLmtLow =
				ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR;
		}
	}

	pstLCacDynaRegCfg->u16LumaThr = ISP_DEMOSAIC_LUMA_THR_DEFAULT;
	pstLCacDynaRegCfg->bResh = GK_TRUE;
}

static GK_S32 LCac_SetLongFrameMode(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LOCAL_CAC_DYNA_CFG_S *pstLCacDynaRegCfg = GK_NULL;
	ISP_LOCAL_CAC_USR_CFG_S *pstLCacUsrRegCfg = GK_NULL;
	ISP_GET_CTX(ViPipe, pstIspCtx);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstLCacUsrRegCfg =
			&pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.stUsrRegCfg;
		pstLCacDynaRegCfg =
			&pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.stDynaRegCfg;

		if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode) ||
		    (pstIspCtx->stLinkage.enFSWDRMode ==
		     ISP_FSWDR_LONG_FRAME_MODE) ||
		    (pstIspCtx->stLinkage.enFSWDRMode ==
		     ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
			pstLCacDynaRegCfg->u16RLumaThr =
				ISP_LCAC_LUMA_THR_R_LINEAR;
			pstLCacDynaRegCfg->u16GLumaThr =
				ISP_LCAC_LUMA_THR_G_LINEAR;
			pstLCacDynaRegCfg->u16BLumaThr =
				ISP_LCAC_LUMA_THR_B_LINEAR;
			pstLCacDynaRegCfg->u8DePurpleCtrCb =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR;
			pstLCacDynaRegCfg->u8DePurpleCtrCr =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR;
			pstLCacDynaRegCfg->s16CbCrRatioLmtLow =
				ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR;

			pstLCacUsrRegCfg->s16CbCrRatioLmtHigh =
				ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR;
		} else {
			pstLCacDynaRegCfg->u16RLumaThr =
				ISP_LCAC_LUMA_THR_R_WDR;
			pstLCacDynaRegCfg->u16GLumaThr =
				ISP_LCAC_LUMA_THR_G_WDR;
			pstLCacDynaRegCfg->u16BLumaThr =
				ISP_LCAC_LUMA_THR_B_WDR;
			pstLCacDynaRegCfg->u8DePurpleCtrCb =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR;
			pstLCacDynaRegCfg->u8DePurpleCtrCr =
				ISP_LCAC_MAX_STRENGTH -
				ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR;
			pstLCacDynaRegCfg->s16CbCrRatioLmtLow =
				ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR;

			pstLCacUsrRegCfg->s16CbCrRatioLmtHigh =
				ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR;
		}

		pstLCacDynaRegCfg->bResh = GK_TRUE;
	}

	return GK_SUCCESS;
}

static GK_VOID LCacRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LCAC_S *pstLCacCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	LocalCAC_GET_CTX(ViPipe, pstLCacCtx);

	/* Local CAC */
	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		LCacStaticRegsInitialize(
			&pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.stStaticRegCfg);
		LCacUsrRegsInitialize(
			ViPipe, pstIspCtx->u8SnsWDRMode,
			&pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.stUsrRegCfg,
			pstLCacCtx);
		LCacDynaRegsInitialize(
			ViPipe, pstIspCtx->u8SnsWDRMode, i,
			&pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.stDynaRegCfg,
			pstLCacCtx);

		pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.bLocalCacEn = GK_TRUE;
	}

	pstRegCfg->unKey.bit1LocalCacCfg = 1;
}

static GK_VOID LocalCacReadExtregs(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_LCAC_S *pstLocalCAC = GK_NULL;

	LocalCAC_GET_CTX(ViPipe, pstLocalCAC);

	pstLocalCAC->bCoefUpdateEn =
		ext_system_LocalCAC_coef_update_en_read(ViPipe);
	ext_system_LocalCAC_coef_update_en_write(ViPipe, GK_FALSE);

	if (pstLocalCAC->bCoefUpdateEn) {
		for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
			pstLocalCAC->au8DePurpleCbStr[i] =
				ext_system_localCAC_auto_cb_str_table_read(
					ViPipe, i);
			pstLocalCAC->au8DePurpleCrStr[i] =
				ext_system_localCAC_auto_cr_str_table_read(
					ViPipe, i);
		}

		for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
			pstLocalCAC->au16RLumaThd[i] =
				ext_system_localCAC_r_thd_table_read(ViPipe, i);
			pstLocalCAC->au16GLumaThd[i] =
				ext_system_localCAC_g_thd_table_read(ViPipe, i);
			pstLocalCAC->au16BLumaThd[i] =
				ext_system_localCAC_b_thd_table_read(ViPipe, i);
			pstLocalCAC->au16YLumaThd[i] =
				ext_system_localCAC_luma_thd_table_read(ViPipe,
									i);
			pstLocalCAC->as16CbCrRatio[i] =
				ext_system_localCAC_cbcr_ratio_table_read(
					ViPipe, i);
		}

		pstLocalCAC->bCacManualEn =
			ext_system_localCAC_manual_mode_enable_read(ViPipe);
		pstLocalCAC->u8DePurpleCbStr =
			ext_system_localCAC_manual_cb_str_read(ViPipe);
		pstLocalCAC->u8DePurpleCrStr =
			ext_system_localCAC_manual_cr_str_read(ViPipe);
		pstLocalCAC->u16PurpleDetRange =
			ext_system_localCAC_purple_det_range_read(ViPipe);
		pstLocalCAC->u16PurpleVarThr =
			ext_system_localCAC_purple_var_thr_read(ViPipe);

		pstLocalCAC->u8LumaHighCntThr =
			ext_system_localCAC_lumagh_cnt_thr_read(ViPipe);
		pstLocalCAC->u8CbCntHighThr =
			ext_system_localCAC_cb_cntgh_thr_read(ViPipe);
		pstLocalCAC->u8CbCntLowThr =
			ext_system_localCAC_cb_cnt_low_thr_read(ViPipe);
		pstLocalCAC->u8BldAvgCur =
			ext_system_localCAC_bld_avg_cur_read(ViPipe);
		//pstLocalCAC->u8DeFColorCr       = ext_system_localCAC_defcolor_cr_read(ViPipe);
		//pstLocalCAC->u8DeFColorCb       = ext_system_localCAC_defcolor_cb_read(ViPipe);
		pstLocalCAC->u16LumaThr =
			ext_system_localCAC_luma_thr_read(ViPipe);
		pstLocalCAC->u16CbThr = ext_system_localCAC_cb_thr_read(ViPipe);
	}
}

static GK_U32 LcacCheckCmosParam(VI_PIPE ViPipe,
				 const ISP_CMOS_LCAC_S *pstCmosLcac)
{
	GK_U16 i;

	if (pstCmosLcac->u16PurpleDetRange > 410) {
		ISP_TRACE(MODULE_DBG_ERR, "err u16PurpleDetRange!\n");
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	if (pstCmosLcac->VarThr > 4095) {
		ISP_TRACE(MODULE_DBG_ERR, "err VarThr!\n");
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
		if (pstCmosLcac->stAuto.au8DePurpleCrStr[i] > 8) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err stAuto.au8DePurpleCrStr!\n");
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstCmosLcac->stAuto.au8DePurpleCbStr[i] > 8) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "err stAuto.au8DePurpleCbStr!\n");
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}
	}

	for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
		if (pstCmosLcac->au16RLumaThd[i] > 4095) {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"err au16RLumaThd[%d] Value!(%d exceed 4095)\n",
				i, pstCmosLcac->au16RLumaThd[i]);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstCmosLcac->au16GLumaThd[i] > 4095) {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"err au16GLumaThd[%d] Value!(%d exceed 4095)\n",
				i, pstCmosLcac->au16GLumaThd[i]);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstCmosLcac->au16BLumaThd[i] > 4095) {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"err au16BLumaThd[%d] Value!(%d exceed 4095)\n",
				i, pstCmosLcac->au16BLumaThd[i]);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if (pstCmosLcac->au16YLumaThd[i] > 4095) {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"err au16YLumaThd[%d] Value!(%d exceed 4095)\n",
				i, pstCmosLcac->au16YLumaThd[i]);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}

		if ((pstCmosLcac->as16CbCrRatio[i] < -2048) ||
		    (pstCmosLcac->as16CbCrRatio[i] > 2047)) {
			ISP_TRACE(MODULE_DBG_ERR, "err CbCrRatio[%d] Value!\n",
				  i);
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}
	}

	if (pstCmosLcac->stManual.u8DePurpleCrStr > 8) {
		ISP_TRACE(MODULE_DBG_ERR, "err stManual.u8DePurpleCrStr!\n");
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	if (pstCmosLcac->stManual.u8DePurpleCbStr > 8) {
		ISP_TRACE(MODULE_DBG_ERR, "err stManual.u8DePurpleCbStr!\n");
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	return GK_SUCCESS;
}

static GK_VOID LCacInitialize(VI_PIPE ViPipe)
{
	GK_U16 i;
	GK_S32 s32Ret;

	ISP_LCAC_S *pstLocalCAC = GK_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);
	LocalCAC_GET_CTX(ViPipe, pstLocalCAC);

	if (pstSnsDft->unKey.bit1Lcac) {
		if (pstSnsDft->pstLcac == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n");
			return;
		}

		s32Ret = LcacCheckCmosParam(ViPipe, pstSnsDft->pstLcac);
		if (s32Ret != GK_SUCCESS) {
			return;
		}

		// Write Cmos parameter to ext registers
		ext_system_localCAC_enable_write(ViPipe,
						 pstSnsDft->pstLcac->bEnable);
		ext_system_localCAC_manual_mode_enable_write(
			ViPipe, pstSnsDft->pstLcac->enOpMode);
		ext_system_localCAC_purple_det_range_write(
			ViPipe, pstSnsDft->pstLcac->u16PurpleDetRange);
		ext_system_localCAC_purple_var_thr_write(
			ViPipe, pstSnsDft->pstLcac->VarThr);

		ext_system_localCAC_manual_cb_str_write(
			ViPipe, pstSnsDft->pstLcac->stManual.u8DePurpleCbStr);
		ext_system_localCAC_manual_cr_str_write(
			ViPipe, pstSnsDft->pstLcac->stManual.u8DePurpleCrStr);

		/* linear mode */
		for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
			ext_system_localCAC_auto_cb_str_table_write(
				ViPipe, i,
				pstSnsDft->pstLcac->stAuto.au8DePurpleCbStr[i]);
			ext_system_localCAC_auto_cr_str_table_write(
				ViPipe, i,
				pstSnsDft->pstLcac->stAuto.au8DePurpleCrStr[i]);
			pstLocalCAC->au8DePurpleCbStr[i] =
				pstSnsDft->pstLcac->stAuto.au8DePurpleCbStr[i];
			pstLocalCAC->au8DePurpleCrStr[i] =
				pstSnsDft->pstLcac->stAuto.au8DePurpleCrStr[i];
		}

		for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
			ext_system_localCAC_r_thd_table_write(
				ViPipe, i, pstSnsDft->pstLcac->au16RLumaThd[i]);
			ext_system_localCAC_g_thd_table_write(
				ViPipe, i, pstSnsDft->pstLcac->au16GLumaThd[i]);
			ext_system_localCAC_b_thd_table_write(
				ViPipe, i, pstSnsDft->pstLcac->au16BLumaThd[i]);
			ext_system_localCAC_luma_thd_table_write(
				ViPipe, i, pstSnsDft->pstLcac->au16YLumaThd[i]);
			ext_system_localCAC_cbcr_ratio_table_write(
				ViPipe, i,
				pstSnsDft->pstLcac->as16CbCrRatio[i]);

			pstLocalCAC->au16RLumaThd[i] =
				pstSnsDft->pstLcac->au16RLumaThd[i];
			pstLocalCAC->au16GLumaThd[i] =
				pstSnsDft->pstLcac->au16GLumaThd[i];
			pstLocalCAC->au16BLumaThd[i] =
				pstSnsDft->pstLcac->au16BLumaThd[i];
			pstLocalCAC->au16YLumaThd[i] =
				pstSnsDft->pstLcac->au16YLumaThd[i];
			pstLocalCAC->as16CbCrRatio[i] =
				pstSnsDft->pstLcac->as16CbCrRatio[i];
		}

		// Update LocalCAC CTX
		pstLocalCAC->u16PurpleDetRange =
			pstSnsDft->pstLcac->u16PurpleDetRange;
		pstLocalCAC->u16PurpleVarThr = pstSnsDft->pstLcac->VarThr;
		pstLocalCAC->u8DePurpleCbStr =
			pstSnsDft->pstLcac->stManual.u8DePurpleCbStr;
		pstLocalCAC->u8DePurpleCrStr =
			pstSnsDft->pstLcac->stManual.u8DePurpleCrStr;
		pstLocalCAC->bCacManualEn = pstSnsDft->pstLcac->enOpMode;
		pstLocalCAC->bLocalCacEn = pstSnsDft->pstLcac->bEnable;
	} else {
		pstLocalCAC->bLocalCacEn = GK_TRUE;
	}
}

static GK_VOID local_cac_usr_fw(ISP_LOCAL_CAC_USR_CFG_S *pstLCacUsrRegCfg,
				ISP_LCAC_S *pstLocalCAC)
{
	pstLCacUsrRegCfg->bResh = GK_TRUE;
	pstLCacUsrRegCfg->u16VarThr = pstLocalCAC->u16PurpleVarThr;
	pstLCacUsrRegCfg->u8LumaHighCntThr = pstLocalCAC->u8LumaHighCntThr;
	pstLCacUsrRegCfg->u8BldAvgCur = pstLocalCAC->u8BldAvgCur;
	//pstLCacUsrRegCfg->u8DeFColorCr     = pstLocalCAC->u8DeFColorCr;
	//pstLCacUsrRegCfg->u8DeFColorCb     = pstLocalCAC->u8DeFColorCb;
	pstLCacUsrRegCfg->u16CbThr = pstLocalCAC->u16CbThr;

	pstLCacUsrRegCfg->u8CbCntHighThr = pstLocalCAC->u8CbCntHighThr;
	pstLCacUsrRegCfg->u8CbCntLowThr = pstLocalCAC->u8CbCntLowThr;

	if (pstLCacUsrRegCfg->u8CbCntLowThr ==
	    pstLCacUsrRegCfg->u8CbCntHighThr) {
		pstLCacUsrRegCfg->u16CbCntRatio = 0;
	} else {
		pstLCacUsrRegCfg->u16CbCntRatio =
			64 * 16 /
			DIV_0_TO_1(MAX2((pstLCacUsrRegCfg->u8CbCntHighThr -
					 pstLCacUsrRegCfg->u8CbCntLowThr),
					0));
	}

	pstLCacUsrRegCfg->u32UpdateIndex += 1;
}

static GK_S32 local_cac_dyna_fw(VI_PIPE ViPipe, GK_U8 u8CurBlk,
				ISP_LCAC_S *pstLocalCAC,
				ISP_LOCAL_CAC_DYNA_CFG_S *pstLCacDynaRegCfg,
				GK_U8 u8WDRMode)
{
	GK_S32 DePurpleCtrCb; // [0, 8]
	GK_S32 DePurpleCtrCr; // [0, 8]
	GK_U32 i, u32ExpRatio;
	GK_S32 s32RangeUpper, s32RangeLower;
	GK_S32 s32RangeIdxUp, s32RangeIdxLow;
	GK_U16 u16RLumaThr, u16GLumaThr, u16BLumaThr, u16LumaThr;
	GK_S16 s16CbCrL;
	GK_S32 s32ExpRatioIndexUpper, s32ExpRatioIndexLower;

	s32RangeIdxUp = RANGE_MAX_VALUE - 1;
	for (i = 0; i < RANGE_MAX_VALUE; i++) {
		if (pstLocalCAC->u16PurpleDetRange < g_au16PurpleDetRange[i]) {
			s32RangeIdxUp = i;
			break;
		}
	}

	s32RangeIdxLow = MAX2(s32RangeIdxUp - 1, 0);
	s32RangeUpper = g_au16PurpleDetRange[s32RangeIdxUp];
	s32RangeLower = g_au16PurpleDetRange[s32RangeIdxLow];

	u16RLumaThr = (GK_U16)LinearInter(
		pstLocalCAC->u16PurpleDetRange, s32RangeLower,
		pstLocalCAC->au16RLumaThd[s32RangeIdxLow], s32RangeUpper,
		pstLocalCAC->au16RLumaThd[s32RangeIdxUp]);
	u16GLumaThr = (GK_U16)LinearInter(
		pstLocalCAC->u16PurpleDetRange, s32RangeLower,
		pstLocalCAC->au16GLumaThd[s32RangeIdxLow], s32RangeUpper,
		pstLocalCAC->au16GLumaThd[s32RangeIdxUp]);
	u16BLumaThr = (GK_U16)LinearInter(
		pstLocalCAC->u16PurpleDetRange, s32RangeLower,
		pstLocalCAC->au16BLumaThd[s32RangeIdxLow], s32RangeUpper,
		pstLocalCAC->au16BLumaThd[s32RangeIdxUp]);
	u16LumaThr = (GK_U16)LinearInter(
		pstLocalCAC->u16PurpleDetRange, s32RangeLower,
		pstLocalCAC->au16YLumaThd[s32RangeIdxLow], s32RangeUpper,
		pstLocalCAC->au16YLumaThd[s32RangeIdxUp]);
	s16CbCrL = LinearInter(pstLocalCAC->u16PurpleDetRange, s32RangeLower,
			       pstLocalCAC->as16CbCrRatio[s32RangeIdxLow],
			       s32RangeUpper,
			       pstLocalCAC->as16CbCrRatio[s32RangeIdxUp]);

	u32ExpRatio = LcacGetExpRatio(ViPipe, u8CurBlk, u8WDRMode);

	if (pstLocalCAC->bCacManualEn) {
		DePurpleCtrCb = pstLocalCAC->u8DePurpleCbStr;
		DePurpleCtrCr = pstLocalCAC->u8DePurpleCrStr;
	} else {
		s32ExpRatioIndexUpper = ISP_EXP_RATIO_STRENGTH_NUM - 1;
		for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
			if (u32ExpRatio <= g_au32ExpRatioLut[i]) {
				s32ExpRatioIndexUpper = i;
				break;
			}
		}

		s32ExpRatioIndexLower = MAX2(s32ExpRatioIndexUpper - 1, 0);

		DePurpleCtrCb = LinearInter(
			u32ExpRatio, g_au32ExpRatioLut[s32ExpRatioIndexLower],
			pstLocalCAC->au8DePurpleCbStr[s32ExpRatioIndexLower],
			g_au32ExpRatioLut[s32ExpRatioIndexUpper],
			pstLocalCAC->au8DePurpleCbStr[s32ExpRatioIndexUpper]);
		DePurpleCtrCr = LinearInter(
			u32ExpRatio, g_au32ExpRatioLut[s32ExpRatioIndexLower],
			pstLocalCAC->au8DePurpleCrStr[s32ExpRatioIndexLower],
			g_au32ExpRatioLut[s32ExpRatioIndexUpper],
			pstLocalCAC->au8DePurpleCrStr[s32ExpRatioIndexUpper]);
	}
	pstLocalCAC->ProcInfoDePurpleCtrCr = DePurpleCtrCr;
	pstLocalCAC->ProcInfoDePurpleCtrCb = DePurpleCtrCb;

	pstLCacDynaRegCfg->bResh = GK_TRUE;
	pstLCacDynaRegCfg->u16RLumaThr = u16RLumaThr;
	pstLCacDynaRegCfg->u16GLumaThr = u16GLumaThr;
	pstLCacDynaRegCfg->u16BLumaThr = u16BLumaThr;
	pstLCacDynaRegCfg->u16LumaThr = u16LumaThr;
	pstLCacDynaRegCfg->s16CbCrRatioLmtLow = s16CbCrL;
	pstLCacDynaRegCfg->u8DePurpleCtrCb =
		ISP_LCAC_MAX_STRENGTH - DePurpleCtrCb;
	pstLCacDynaRegCfg->u8DePurpleCtrCr =
		ISP_LCAC_MAX_STRENGTH -
		DePurpleCtrCr; /* ISP_LCAC_MAX_STRENGTH means the max strength in user level,but means the min strength in fpga level */

	return GK_SUCCESS;
}

static GK_BOOL __inline CheckLCacOpen(ISP_LCAC_S *pstLocalCAC)
{
	return (pstLocalCAC->bLocalCacEn == GK_TRUE);
}

GK_S32 ISP_LCacInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_S32 s32Ret = GK_SUCCESS;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	s32Ret = LocalCacCtxInit(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	LCacExtRegsInitialize(ViPipe);
	LCacInitialize(ViPipe);
	LCacRegsInitialize(ViPipe, pstRegCfg);

	return GK_SUCCESS;
}

static GK_VOID ISP_LCacWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_U8 i;
	GK_U32 au32UpdateIdx[ISP_STRIPING_MAX_NUM] = { 0 };
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		au32UpdateIdx[i] =
			pstRegCfg->stAlgRegCfg[i]
				.stLCacRegCfg.stUsrRegCfg.u32UpdateIndex;
	}

	ISP_LCacInit(ViPipe, pRegCfg);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstRegCfg->stAlgRegCfg[i]
			.stLCacRegCfg.stUsrRegCfg.u32UpdateIndex =
			au32UpdateIdx[i] + 1;
	}
}

GK_S32 ISP_LCacRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
		   GK_S32 s32Rsv)
{
	GK_U8 i;
	ISP_LCAC_S *pstLocalCAC = GK_NULL;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	LocalCAC_GET_CTX(ViPipe, pstLocalCAC);
	ISP_CHECK_POINTER(pstLocalCAC);

	if (pstIspCtx->stLinkage.bDefectPixel) {
		return GK_SUCCESS;
	}

	pstLocalCAC->bLocalCacEn = ext_system_localCAC_enable_read(ViPipe);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.bLocalCacEn =
			pstLocalCAC->bLocalCacEn;
	}

	pstRegCfg->unKey.bit1LocalCacCfg = 1;

	/* check hardware setting */
	if (!CheckLCacOpen(pstLocalCAC)) {
		return GK_SUCCESS;
	}

	LocalCacReadExtregs(ViPipe);

	if (pstIspCtx->stLinkage.enFSWDRMode !=
	    pstIspCtx->stLinkage.enPreFSWDRMode) {
		LCac_SetLongFrameMode(ViPipe, pstRegCfg);
	}

	if (pstLocalCAC->bCoefUpdateEn) {
		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			local_cac_usr_fw(&pstRegCfg->stAlgRegCfg[i]
						  .stLCacRegCfg.stUsrRegCfg,
					 pstLocalCAC);
		}
	}

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		local_cac_dyna_fw(
			ViPipe, i, pstLocalCAC,
			&pstRegCfg->stAlgRegCfg[i].stLCacRegCfg.stDynaRegCfg,
			pstIspCtx->u8SnsWDRMode);
	}

	return GK_SUCCESS;
}
GK_S32 LcacProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	ISP_CTRL_PROC_WRITE_S stProcTmp;
	ISP_LCAC_S *pstLocalCAC = GK_NULL;
	LocalCAC_GET_CTX(ViPipe, pstLocalCAC);
	ISP_CHECK_POINTER(pstLocalCAC);

	if ((pstProc->pcProcBuff == GK_NULL) || (pstProc->u32BuffLen == 0)) {
		return GK_FAILURE;
	}

	stProcTmp.pcProcBuff = pstProc->pcProcBuff;
	stProcTmp.u32BuffLen = pstProc->u32BuffLen;

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"-----Lcac INFO-----------------------------------\n");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%12s"
			"%12s"
			"%12s\n",
			"Enable", "CrCtr", "CbCtr");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%12u"
			"%12u"
			"%12u\n",
			pstLocalCAC->bLocalCacEn,
			pstLocalCAC->ProcInfoDePurpleCtrCr,
			pstLocalCAC->ProcInfoDePurpleCtrCb);

	pstProc->u32WriteLen += 1;

	return GK_SUCCESS;
}

GK_S32 ISP_LCacCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	switch (u32Cmd) {
	case ISP_WDR_MODE_SET:
		ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
		ISP_CHECK_POINTER(pRegCfg);
		ISP_LCacWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
		break;
	case ISP_PROC_WRITE:
		LcacProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
		break;
	default:
		break;
	}
	return GK_SUCCESS;
}

GK_S32 ISP_LCacExit(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

	for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
		pRegCfg->stRegCfg.stAlgRegCfg[i].stLCacRegCfg.bLocalCacEn =
			GK_FALSE;
	}

	pRegCfg->stRegCfg.unKey.bit1LocalCacCfg = 1;

	LocalCacCtxExit(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterLCac(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1LCac);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_LCAC;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_LCacInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_LCacRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_LCacCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_LCacExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
