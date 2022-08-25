/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "sc500ai_cmos.h"
#include "sc500ai_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#define SC500AI_ID 500
#define ISO_LUT_NUM 8

ISP_SNS_STATE_S *g_pastSc500ai[ISP_MAX_PIPE_NUM] = { GK_NULL };

GK_VOID sc500ai_sensor_set_ctx(VI_PIPE ViPipe, ISP_SNS_STATE_S *pstCtx)
{
	g_pastSc500ai[ViPipe] = pstCtx;
}

GK_VOID sc500ai_sensor_reset_ctx(VI_PIPE ViPipe)
{
	g_pastSc500ai[ViPipe] = GK_NULL;
}

ISP_SNS_COMMBUS_U g_sc500ai_bus_info[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E g_fswdr_mode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

ISP_SNS_COMMBUS_U *sc500ai_get_bus_Info(VI_PIPE ViPipe)
{
	return &g_sc500ai_bus_info[ViPipe];
}

ISP_SNS_STATE_S *sc500ai_get_ctx(VI_PIPE ViPipe)
{
	return g_pastSc500ai[ViPipe];
}

static GK_U32 g_max_time_get_cnt[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_init_exposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_lines_per500ms[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_init_iso[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] =
						       100 }; /* init iso 100 */

static GK_U16 g_init_wb_Gain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_sample_rgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_sample_bgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_init_ccm[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = { { 0 } };
static GK_BOOL g_is_ir_mode[ISP_MAX_PIPE_NUM] = { GK_FALSE };
static const GK_U32 g_iso_lut[ISO_LUT_NUM] = { 100,  200,  400,	 800,
					       1600, 3200, 6400, 12800 };

/* Sc500ai Register Address */
#define SC500AI_EXP_H_ADDR 0x3e00
#define SC500AI_EXP_M_ADDR 0x3e01
#define SC500AI_EXP_L_ADDR 0x3e02
#define SC500AI_AGAIN_H_ADDR 0x3e08
#define SC500AI_AGAIN_L_ADDR 0x3e09
#define SC500AI_DGAIN_H_ADDR 0x3e06
#define SC500AI_DGAIN_L_ADDR 0x3e07
#define SC500AI_VMAX_H_ADDR 0x320e
#define SC500AI_VMAX_L_ADDR 0x320f
#define SC500AI_HMAX_H_ADDR 0x320c
#define SC500AI_HMAX_L_ADDR 0x320d

/* sensor fps mode */
#define SC500AI_FULL_LINES_MAX 0xFFFF
#define SC500AI_INCREASE_LINES \
	0 /* make real fps less than stand fps because NVR require */
#define SC500AI_VMAX_2880x1620_LINEAR (1650 + SC500AI_INCREASE_LINES)
#define SC500AI_FRAME_RATE_MIN 0x4D58
#define SC500AI_SENSOR_FPS_MAX 30
#define SC500AI_SENSOR_FPS_MIN 5

#define SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE 1
GK_BOOL SC500AI_RES_IS_5M(GK_U32 w, GK_U32 h)
{
	return (w <= 2880 && h <= 1620); /* width 2880 height 1620 */
}

static GK_VOID cmos_get_ae_comm_default(VI_PIPE ViPipe,
					AE_SENSOR_DEFAULT_S *pstAeSnsDft,
					ISP_SNS_STATE_S *pstSnsState)
{
	GK_U32 u32Fll = 0;

	if (pstSnsState->u8ImgMode ==
	    SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE) {
		u32Fll = SC500AI_VMAX_2880x1620_LINEAR;
	}

	pstSnsState->u32FLStd = u32Fll;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 0;
	pstAeSnsDft->u32FullLinesMax = SC500AI_FULL_LINES_MAX;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 0.015625; /* Accuracy 0.015625 */

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stDgainAccu.f32Accuracy =
		0.0078125; /* Accuracy 0.0078125 */

	if (g_lines_per500ms[ViPipe] == 0) {
		pstAeSnsDft->u32LinesPer500ms =
			((GK_U64)(u32Fll * SC500AI_SENSOR_FPS_MAX)) >> 1;
	} else {
		pstAeSnsDft->u32LinesPer500ms = g_lines_per500ms[ViPipe];
	}

	pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
	pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

	pstAeSnsDft->bAERouteExValid = GK_FALSE;
	pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
	pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

	return;
}

static GK_VOID cmos_get_ae_linear_default(VI_PIPE ViPipe,
					  AE_SENSOR_DEFAULT_S *pstAeSnsDft,
					  ISP_SNS_STATE_S *pstSnsState)
{
	pstAeSnsDft->au8HistThresh[0] = 0x0D; /* index 0 */
	pstAeSnsDft->au8HistThresh[1] = 0x28; /* index 1 */
	pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
	pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

	pstAeSnsDft->u8AeCompensation = 0x40;
	pstAeSnsDft->u32InitExposure = g_init_exposure[ViPipe] ?
					       g_init_exposure[ViPipe] :
					       148859; /* init 148859 */
	pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
	pstAeSnsDft->u32MinIntTime = 3; /* min 3 */
	pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
	pstAeSnsDft->u32MinIntTimeTarget = 3; /* min 3 */

	pstAeSnsDft->u32MaxAgain = 1556; /* 15.875x64xdcg1.53125 = 1556 */
	pstAeSnsDft->u32MinAgain = 64; /* min 64 */
	pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
	pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

	pstAeSnsDft->u32MaxDgain = 4080; /* 4080 */
	pstAeSnsDft->u32MinDgain = 128; /* min 128 */
	pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
	pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

	pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget =
		2 << pstAeSnsDft->u32ISPDgainShift; /* max 2 */

	return;
}

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0,
		 sizeof(ISP_AE_ROUTE_S));

	cmos_get_ae_comm_default(ViPipe, pstAeSnsDft, pstSnsState);

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		cmos_get_ae_linear_default(ViPipe, pstAeSnsDft, pstSnsState);
		break;
	}

	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32FullLines;
	GK_U32 u32VMAX = SC500AI_VMAX_2880x1620_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE:
		if ((f32Fps <= SC500AI_SENSOR_FPS_MAX) &&
		    (f32Fps >= SC500AI_SENSOR_FPS_MIN)) {
			u32VMAX = SC500AI_VMAX_2880x1620_LINEAR *
				  SC500AI_SENSOR_FPS_MAX /
				  DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		pstAeSnsDft->u32LinesPer500ms =
			SC500AI_VMAX_2880x1620_LINEAR * 2 * 15; /* 2 * 15 */
		break;
	default:
		break;
	}

	u32VMAX = (u32VMAX > SC500AI_FULL_LINES_MAX) ? SC500AI_FULL_LINES_MAX :
						       u32VMAX;

	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_H_ADDR].u32Data =
		(u32VMAX & 0xFF00) >> 8; /* shift 8 */
	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_L_ADDR].u32Data =
		u32VMAX & 0x00FF;

	u32FullLines = u32VMAX << 1;
	pstSnsState->u32FLStd = (u32FullLines > SC500AI_FULL_LINES_MAX) ?
					SC500AI_FULL_LINES_MAX :
					u32FullLines;
	pstAeSnsDft->f32Fps = f32Fps;

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* offset 8 */
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_U32 u32Value;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);
	u32FullLines = (u32FullLines > SC500AI_FRAME_RATE_MIN) ?
			       SC500AI_FRAME_RATE_MIN :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;
	u32Value = (((u32FullLines + 8) >> 1) & 0xFF00) >>
		   8; /* offset 8 shift 8 */
	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_H_ADDR].u32Data =
		u32Value;
	u32Value = ((u32FullLines + 8) >> 1) & 0x00FF; /* offset 8 */
	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_L_ADDR].u32Data =
		u32Value;

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8; /* offset 8 */

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val = (u32IntTime > SC500AI_FULL_LINES_MAX) ?
			 SC500AI_FULL_LINES_MAX :
			 u32IntTime;

	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_H_ADDR].u32Data =
		(u32Val >> 12) & 0x0F; /* shift 12 */
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_M_ADDR].u32Data =
		(u32Val >> 4) & 0xFF; /* shift 4 */
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_L_ADDR].u32Data =
		(u32Val << 4) & 0xF0; /* shift 4 */

	return;
}

#define AGAIN_INDEX_MAX 64
static const GK_U32 gau16AgainTab[AGAIN_INDEX_MAX] = {
	1024,  1088,  1152,  1216,  1280,  1344,  1408,	 1472,	1536,  1600,
	1664,  1728,  1792,  1856,  1920,  1984,  2048,	 2176,	2304,  2432,
	2560,  2688,  2816,  2944,  3072,  3200,  3328,	 3456,	3584,  3712,
	3840,  3968,  4096,  4352,  4608,  4864,  5120,	 5376,	5632,  5888,
	6144,  6400,  6656,  6912,  7168,  7424,  7680,	 7936,	8192,  8704,
	9216,  9728,  10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824,
	14336, 14848, 15360, 15872
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;
	static GK_U8 againmax = AGAIN_INDEX_MAX - 1;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= gau16AgainTab[againmax]) {
		*pu32AgainDb = againmax;
		return;
	}

	for (i = 1; i < AGAIN_INDEX_MAX; i++) {
		if (*pu32AgainLin < gau16AgainTab[i]) {
			*pu32AgainDb = i - 1;
			break;
		}
	}

	*pu32AgainLin = gau16AgainTab[*pu32AgainDb];

	return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin,
				     GK_U32 *pu32DgainDb)
{
	CMOS_CHECK_POINTER_VOID(pu32DgainLin);
	CMOS_CHECK_POINTER_VOID(pu32DgainDb);

	*pu32DgainDb = *pu32DgainLin / 1024; /* precison 1024 */

	if (*pu32DgainDb == 3) { /* 3 */
		*pu32DgainDb = 2; /* 2x */
	} else if (*pu32DgainDb >= 4 && *pu32DgainDb < 8) { /* 4 < gain < 8 */
		*pu32DgainDb = 4; /* 4x */
	} else if (*pu32DgainDb >= 8 && *pu32DgainDb < 16) { /* 8 < gain < 16 */
		*pu32DgainDb = 8; /* 8x */
	} else if (*pu32DgainDb >= 16) { /* gain > 16 */
		*pu32DgainDb = 16; /* 16x */
	}

	*pu32DgainLin = *pu32DgainDb * 1024; /* precison 1024 */

	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	GK_U8 u8Reg0x3e09;
	GK_U8 u8Reg0x3e08 = 0x03;
	GK_U8 u8Reg0x3e07;
	GK_U8 u8Reg0x3e06 = 0x00;
	GK_U8 aCoarseGain;
	GK_U8 aFineGain;
	GK_U8 dCoarseGain;
	GK_U8 dFineGain;

	if (u32Again < 98) { // a_gain < 1.75x64, 98
		for (aCoarseGain = 1; aCoarseGain <= 2;
		     aCoarseGain = aCoarseGain * 2) { // 1,2,4,8,16
			if (u32Again < (64 * 2 * aCoarseGain)) { // 64
				break;
			}
		}
		aFineGain = u32Again / aCoarseGain;
	} else {
		for (aCoarseGain = 1; aCoarseGain <= 8;
		     aCoarseGain = aCoarseGain * 2) { // 1,2,4,8
			if (u32Again <
			    (64 * 2 * aCoarseGain * 1.53)) { // 64 * 2 * 1.53
				break;
			}
		}
		aFineGain = 1.0 * u32Again / aCoarseGain / 1.53; // 1.53
	}

	for (; aCoarseGain >= 2; aCoarseGain = aCoarseGain / 2) { // 2
		u8Reg0x3e08 = (u8Reg0x3e08 << 1) | 0x01;
	}

	u8Reg0x3e09 = aFineGain & 0x7f;
	u8Reg0x3e08 = (u32Again >= 98) ? (u8Reg0x3e08 | 0x20) :
					 (u8Reg0x3e08 & 0x1f); // 98

	// dgain
	for (dCoarseGain = 1; dCoarseGain <= 16;
	     dCoarseGain = dCoarseGain * 2) { // 1,2,4,8,16
		if (u32Dgain < (128 * 2 * dCoarseGain)) { // 128 * 2
			break;
		}
	}
	dFineGain = u32Dgain / dCoarseGain;

	for (; dCoarseGain >= 2; dCoarseGain = dCoarseGain / 2) { // 2
		u8Reg0x3e06 = (u8Reg0x3e06 << 1) | 0x01;
	}

	u8Reg0x3e07 = dFineGain;

	pstSnsState->astRegsInfo[0].astI2cData[E_DGC_H_ADDR].u32Data =
		u8Reg0x3e06;
	pstSnsState->astRegsInfo[0].astI2cData[E_DGC_L_ADDR].u32Data =
		u8Reg0x3e07;
	pstSnsState->astRegsInfo[0].astI2cData[E_AGC_H_ADDR].u32Data =
		u8Reg0x3e08;
	pstSnsState->astRegsInfo[0].astI2cData[E_AGC_L_ADDR].u32Data =
		u8Reg0x3e09;

	return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,
				      AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
	CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

	g_fswdr_mode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
	g_max_time_get_cnt[ViPipe] = 0;

	return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset_s(pstExpFuncs, sizeof(AE_SENSOR_EXP_FUNC_S), 0,
		 sizeof(AE_SENSOR_EXP_FUNC_S));

	pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
	pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
	pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
	pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
	pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
	pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;
	pstExpFuncs->pfn_cmos_dgain_calc_table = cmos_dgain_calc_table;
	pstExpFuncs->pfn_cmos_get_inttime_max = GK_NULL;
	pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;
	return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_STATIC_TEMP 5000
#define CALIBRATE_AWB_STATIC_RGAIN 379
#define CALIBRATE_AWB_STATIC_GGAIN 256
#define CALIBRATE_AWB_STATIC_BGAIN 486
#define CALIBRATE_AWB_P1 193
#define CALIBRATE_AWB_P2 (-119)
#define CALIBRATE_AWB_Q1 (-182)
#define CALIBRATE_AWB_A1 247711
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-187838)

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0,
		 sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = CALIBRATE_AWB_STATIC_TEMP;

	pstAwbSnsDft->au16GainOffset[0] =
		CALIBRATE_AWB_STATIC_RGAIN; /* index 0  */
	pstAwbSnsDft->au16GainOffset[1] =
		CALIBRATE_AWB_STATIC_GGAIN; /* index 1  */
	pstAwbSnsDft->au16GainOffset[2] =
		CALIBRATE_AWB_STATIC_GGAIN; /* index 2  */
	pstAwbSnsDft->au16GainOffset[3] =
		CALIBRATE_AWB_STATIC_BGAIN; /* index 3  */

	pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1; /* index 0  */
	pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2; /* index 1  */
	pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1; /* index 2  */
	pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1; /* index 3  */
	pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1; /* index 4  */
	pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1; /* index 5  */
	pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
	pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm,
			 sizeof(AWB_CCM_S));
		if (g_is_ir_mode[ViPipe] == GK_TRUE) {
			memcpy_s(&pstAwbSnsDft->stAgcTbl,
				 sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTableIr,
				 sizeof(AWB_AGC_TABLE_S));
		} else {
			memcpy_s(&pstAwbSnsDft->stAgcTbl,
				 sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTable,
				 sizeof(AWB_AGC_TABLE_S));
		}
		break;
	}

	pstAwbSnsDft->u16InitRgain = g_init_wb_Gain[ViPipe][0]; /* rgain 0  */
	pstAwbSnsDft->u16InitGgain = g_init_wb_Gain[ViPipe][1]; /* ggain 1  */
	pstAwbSnsDft->u16InitBgain = g_init_wb_Gain[ViPipe][2]; /* bgain 2  */
	pstAwbSnsDft->u16SampleRgain = g_sample_rgain[ViPipe];
	pstAwbSnsDft->u16SampleBgain = g_sample_bgain[ViPipe];

	for (i = 0; i < CCM_MATRIX_SIZE; i++) {
		pstAwbSnsDft->au16InitCCM[i] = g_init_ccm[ViPipe][i];
	}

	return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset_s(pstExpFuncs, sizeof(AWB_SENSOR_EXP_FUNC_S), 0,
		 sizeof(AWB_SENSOR_EXP_FUNC_S));
	pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

	return GK_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = { { 378, 256, 430 },
						       { 439, 256, 439 } };

static GK_U32 Interpulate(GK_U32 u32Mid, GK_U32 u32Left, GK_U32 u32LValue,
			  GK_U32 u32Right, GK_U32 u32RValue)
{
	GK_U32 u32Value;
	GK_U32 k;

	if (u32Mid <= u32Left) {
		u32Value = u32LValue;
		return u32Value;
	}

	if (u32Mid >= u32Right) {
		u32Value = u32RValue;
		return u32Value;
	}

	k = (u32Right - u32Left);
	u32Value = (((u32Right - u32Mid) * u32LValue +
		     (u32Mid - u32Left) * u32RValue + (k >> 1)) /
		    k);

	return u32Value;
}

static GK_VOID cmos_set_drc_init_str(VI_PIPE ViPipe)
{
	GK_U32 u32Level;
	GK_U32 au32DrcInitStr0[ISO_LUT_NUM] = { DRC_INIT_STR_NA };
	GK_U32 au32DrcInitStr1[ISO_LUT_NUM] = { DRC_INIT_STR_IR };

	for (u32Level = 0; u32Level < ISO_LUT_NUM; u32Level++) {
		if (g_init_iso[ViPipe] <= g_iso_lut[u32Level]) {
			break;
		}
	}

	if (u32Level == ISO_LUT_NUM) {
		u32Level = ISO_LUT_NUM - 1;
	}

	if (g_is_ir_mode[ViPipe] == GK_TRUE) {
		if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
			g_stIspDRC.u16ManualStrength =
				au32DrcInitStr1[u32Level];
		} else {
			g_stIspDRC.u16ManualStrength = Interpulate(
				g_init_iso[ViPipe], g_iso_lut[u32Level - 1],
				au32DrcInitStr1[u32Level - 1],
				g_iso_lut[u32Level], au32DrcInitStr1[u32Level]);
		}
	} else {
		if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
			g_stIspDRC.u16ManualStrength =
				au32DrcInitStr0[u32Level];
		} else {
			g_stIspDRC.u16ManualStrength = Interpulate(
				g_init_iso[ViPipe], g_iso_lut[u32Level - 1],
				au32DrcInitStr0[u32Level - 1],
				g_iso_lut[u32Level], au32DrcInitStr0[u32Level]);
		}
	}
}

static GK_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *pstSnsState,
					ISP_CMOS_DEFAULT_S *pstDef)
{
	memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S),
		 &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample =
			10; /* 10 bit  */
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel =
			4095; /* max 4095  */
		break;
	}

	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH
		.u32Denominator = 1;
	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH
		.u32Numerator = 1;
	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV
		.u32Denominator = 1;
	pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV
		.u32Numerator = 1;
	pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
		.u16RepeatPatternDimRows = 2; /* DimRows 2  */
	pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
		.u16RepeatPatternDimCols = 2; /* DimCols 2  */
	pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows =
		2; /* Rows 2  */
	pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols =
		2; /* Cols 2  */
	pstDef->stSensorMode.stDngRawFormat.enCfaLayout =
		CFALAYOUT_TYPE_RECTANGULAR;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] =
		2; /* index2: CfaPlaneColor 2 */
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] =
		1; /* index2: CfaPattern 1 */
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] =
		2; /* index3: CfaPattern 2 */
	pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;
	return;
}

static void cmos_get_isp_linear_default(VI_PIPE ViPipe,
					ISP_CMOS_DEFAULT_S *pstDef)
{
	pstDef->unKey.bit1Demosaic = 1;
	pstDef->pstDemosaic = &g_stIspDemosaic;
	pstDef->unKey.bit1Sharpen = 1;
	pstDef->pstSharpen = &g_stIspYuvSharpen;
	pstDef->unKey.bit1Drc = 1;
	pstDef->pstDrc = &g_stIspDRC;
	pstDef->unKey.bit1BayerNr = 1;
	pstDef->pstBayerNr = &g_stIspBayerNr;
	pstDef->unKey.bit1AntiFalseColor = 1;
	pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
	pstDef->unKey.bit1Ldci = 1;
	if (g_is_ir_mode[ViPipe] == GK_TRUE) {
		pstDef->pstLdci = &g_stIspLdciIr;
	} else {
		pstDef->pstLdci = &g_stIspLdci;
	}
	pstDef->unKey.bit1Gamma = 1;
	if (g_is_ir_mode[ViPipe] == GK_TRUE) {
		pstDef->pstGamma = &g_stIspGammaIr;
	} else {
		pstDef->pstGamma = &g_stIspGamma;
	}
	pstDef->unKey.bit1Ge = 1;
	pstDef->pstGe = &g_stIspGe;
	pstDef->unKey.bit1Dehaze = 1;
	if (g_is_ir_mode[ViPipe] == GK_TRUE) {
		pstDef->pstDehaze = &g_stIspDehazeIr;
	} else {
		pstDef->pstDehaze = &g_stIspDehaze;
	}
	pstDef->unKey.bit1Lcac = 1;
	pstDef->pstLcac = &g_stIspLCac;
	pstDef->unKey.bit1Rgbir = 0;
	memcpy_s(&pstDef->stNoiseCalibration,
		 sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibratio,
		 sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
	return;
}

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstDef);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0,
		 sizeof(ISP_CMOS_DEFAULT_S));

	cmos_set_drc_init_str(ViPipe);

	pstDef->unKey.bit1Ca = 1;
	pstDef->pstCa = &g_stIspCA;
	pstDef->unKey.bit1Dpc = 1;
	pstDef->pstDpc = &g_stCmosDpc;
	pstDef->unKey.bit1Wdr = 1;
	pstDef->pstWdr = &g_stIspWDR;
	pstDef->unKey.bit1Lsc = 0;
	pstDef->pstLsc = &g_stCmosLsc;
	pstDef->unKey.bit1PreGamma = 0;
	pstDef->pstPreGamma = &g_stPreGamma;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		cmos_get_isp_linear_default(ViPipe, pstDef);
		break;
	}

	pstDef->stSensorMode.u32SensorID = SC500AI_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	cmos_get_isp_dng_default(pstSnsState, pstDef);

	return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,
				       ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstBlackLevel);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;

	for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
		pstBlackLevel->au16BlackLevel[i] = 0x100;
	}

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps;
	GK_U32 u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines_5Fps =
		(SC500AI_VMAX_2880x1620_LINEAR * SC500AI_SENSOR_FPS_MAX) /
		5; /* 5 fps */
	u32MaxIntTime_5Fps = u32FullLines_5Fps - 8; /* sub 8 */

	if (bEnable) { /* setup for ISP pixel calibration mode */
		sc500ai_write_register(ViPipe, SC500AI_VMAX_H_ADDR,
				       (u32FullLines_5Fps & 0xFF00) >>
					       8); /* shift 8 */
		sc500ai_write_register(ViPipe, SC500AI_VMAX_L_ADDR,
				       u32FullLines_5Fps & 0x00FF);
		sc500ai_write_register(ViPipe, SC500AI_EXP_M_ADDR,
				       (u32MaxIntTime_5Fps & 0x0FF0) >>
					       4); /* shift 4 */
		sc500ai_write_register(ViPipe, SC500AI_EXP_L_ADDR,
				       (u32MaxIntTime_5Fps & 0xF)
					       << 4); /* shift 4 */

		sc500ai_write_register(ViPipe, SC500AI_AGAIN_H_ADDR, 0x00);
		sc500ai_write_register(ViPipe, SC500AI_AGAIN_L_ADDR, 0x10);
	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd > SC500AI_FULL_LINES_MAX) ?
				(SC500AI_FULL_LINES_MAX) :
				pstSnsState->u32FLStd;
		sc500ai_write_register(ViPipe, SC500AI_VMAX_H_ADDR,
				       (pstSnsState->u32FLStd & 0xFF00) >>
					       8); /* shift 8 */
		sc500ai_write_register(ViPipe, SC500AI_VMAX_L_ADDR,
				       (pstSnsState->u32FLStd & 0xFF));

		pstSnsState->bSyncInit = GK_FALSE;
	}
	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		pstSnsState->u8ImgMode =
			SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE;
		printf("linear mode\n");
		break;
	default:
		printf("NOT support this mode!\n");
		return GK_FAILURE;
	}

	memset_s(pstSnsState->au32WDRIntTime,
		 sizeof(pstSnsState->au32WDRIntTime), 0,
		 sizeof(pstSnsState->au32WDRIntTime));

	return GK_SUCCESS;
}

static GK_VOID cmos_comm_sns_reg_info_init(VI_PIPE ViPipe,
					   ISP_SNS_STATE_S *pstSnsState)
{
	GK_U32 i;

	pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
	pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
		g_sc500ai_bus_info[ViPipe].s8I2cDev;
	pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* Max delay 2 */
	pstSnsState->astRegsInfo[0].u32RegNum = E_REG_MAX_IDX;

	for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
		pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
		pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
			SC500AI_I2C_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum =
			SC500AI_ADDR_BYTE;
		pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum =
			SC500AI_DATA_BYTE;
	}

	// Linear Mode Regs
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_H_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_H_ADDR].u32RegAddr =
		SC500AI_EXP_H_ADDR;
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_M_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_M_ADDR].u32RegAddr =
		SC500AI_EXP_M_ADDR;
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_L_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_EXP_L_ADDR].u32RegAddr =
		SC500AI_EXP_L_ADDR;

	pstSnsState->astRegsInfo[0].astI2cData[E_DGC_H_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_DGC_H_ADDR].u32RegAddr =
		SC500AI_DGAIN_H_ADDR;
	pstSnsState->astRegsInfo[0].astI2cData[E_DGC_L_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_DGC_L_ADDR].u32RegAddr =
		SC500AI_DGAIN_L_ADDR;

	pstSnsState->astRegsInfo[0].astI2cData[E_AGC_H_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_AGC_H_ADDR].u32RegAddr =
		SC500AI_AGAIN_H_ADDR;
	pstSnsState->astRegsInfo[0].astI2cData[E_AGC_L_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_AGC_L_ADDR].u32RegAddr =
		SC500AI_AGAIN_L_ADDR;

	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_H_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_H_ADDR].u32RegAddr =
		SC500AI_VMAX_H_ADDR;
	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_L_ADDR].u8DelayFrmNum = 0;
	pstSnsState->astRegsInfo[0].astI2cData[E_VMAX_L_ADDR].u32RegAddr =
		SC500AI_VMAX_L_ADDR;

	pstSnsState->bSyncInit = GK_TRUE;

	return;
}

static GK_VOID cmos_sns_reg_info_update(VI_PIPE ViPipe,
					ISP_SNS_STATE_S *pstSnsState)
{
	GK_U32 i;

	for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
		if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
		    pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_FALSE;
		} else {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
		}
	}

	return;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe,
				     ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSnsRegsInfo);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		cmos_comm_sns_reg_info_init(ViPipe, pstSnsState);
	} else {
		cmos_sns_reg_info_update(ViPipe, pstSnsState);
	}

	pstSnsRegsInfo->bConfig = GK_FALSE;

	memcpy_s(pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S),
		 &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
	memcpy_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S),
		 &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

	pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

	return GK_SUCCESS;
}

static GK_S32
cmos_set_image_mode(VI_PIPE ViPipe,
		    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
	GK_U8 u8SensorImageMode;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSensorImageMode);
	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= SC500AI_SENSOR_FPS_MAX) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (SC500AI_RES_IS_5M(pstSensorImageMode->u16Width,
					      pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd =
					SC500AI_VMAX_2880x1620_LINEAR *
					2; /* 2x */
			} else {
				ISP_TRACE(
					MODULE_DBG_ERR,
					"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
					pstSensorImageMode->u16Width,
					pstSensorImageMode->u16Height,
					pstSensorImageMode->f32Fps,
					pstSnsState->enWDRMode);
				return GK_FAILURE;
			}
		} else {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
				pstSensorImageMode->u16Width,
				pstSensorImageMode->u16Height,
				pstSensorImageMode->f32Fps,
				pstSnsState->enWDRMode);
			return GK_FAILURE;
		}
	} else {
	}

	if ((pstSnsState->bInit == GK_TRUE) &&
	    (u8SensorImageMode == pstSnsState->u8ImgMode)) {
		/* Don't need to switch SensorImageMode */
		return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
	}

	pstSnsState->u8ImgMode = u8SensorImageMode;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

	return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	sc500ai_sensor_get_ctx(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = SC500AI_SENSOR_2880x1620_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = SC500AI_VMAX_2880x1620_LINEAR * 2; /* 2x */
	pstSnsState->au32FL[0] = SC500AI_VMAX_2880x1620_LINEAR * 2; /* 2x */
	pstSnsState->au32FL[1] = SC500AI_VMAX_2880x1620_LINEAR * 2; /* 2x */

	memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0,
		 sizeof(ISP_SNS_REGS_INFO_S));
	memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0,
		 sizeof(ISP_SNS_REGS_INFO_S));
	return;
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0,
		 sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = sc500ai_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = sc500ai_exit;
	pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
	pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
	pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

	pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
	pstSensorExpFunc->pfn_cmos_get_isp_black_level =
		cmos_get_isp_black_level;
	pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
	pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

	return GK_SUCCESS;
}

static GK_S32 sc500ai_set_bus_info(VI_PIPE ViPipe,
				   ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_sc500ai_bus_info[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	sc500ai_sensor_get_ctx(ViPipe, pastSnsStateCtx);

	if (pastSnsStateCtx == GK_NULL) {
		pastSnsStateCtx =
			(ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
		if (pastSnsStateCtx == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] SnsCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0,
		 sizeof(ISP_SNS_STATE_S));

	sc500ai_sensor_set_ctx(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	sc500ai_sensor_get_ctx(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	sc500ai_sensor_reset_ctx(ViPipe);

	return;
}

static GK_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
				       ALG_LIB_S *pstAwbLib)
{
	GK_S32 s32Ret;
	ISP_SENSOR_REGISTER_S stIspRegister;
	AE_SENSOR_REGISTER_S stAeRegister;
	AWB_SENSOR_REGISTER_S stAwbRegister;
	ISP_SNS_ATTR_INFO_S stSnsAttrInfo;

	CMOS_CHECK_POINTER(pstAeLib);
	CMOS_CHECK_POINTER(pstAwbLib);

	s32Ret = sensor_ctx_init(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		return GK_FAILURE;
	}

	stSnsAttrInfo.eSensorId = SC500AI_ID;

	s32Ret = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
	s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo,
					       &stIspRegister);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor register callback function failed!\n");
		return s32Ret;
	}

	s32Ret = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
	s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo,
					      &stAeRegister);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor register callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
	s32Ret |= GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib,
					       &stSnsAttrInfo, &stAwbRegister);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor register callback function to awb lib failed!\n");
		return s32Ret;
	}

	return GK_SUCCESS;
}

static GK_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
					 ALG_LIB_S *pstAwbLib)
{
	GK_S32 s32Ret;

	CMOS_CHECK_POINTER(pstAeLib);
	CMOS_CHECK_POINTER(pstAwbLib);

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC500AI_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC500AI_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC500AI_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to awb lib failed!\n");
		return s32Ret;
	}

	sensor_ctx_exit(ViPipe);

	return GK_SUCCESS;
}

static GK_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
	GK_S32 i;
	CMOS_CHECK_POINTER(pstInitAttr);

	if (pstInitAttr->u32InitIso) {
		g_init_iso[ViPipe] = pstInitAttr->u32InitIso;
	}
	g_init_exposure[ViPipe] = pstInitAttr->u32Exposure;
	g_lines_per500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
	g_init_wb_Gain[ViPipe][0] = pstInitAttr->u16WBRgain; /* index0: Rgain */
	g_init_wb_Gain[ViPipe][1] = pstInitAttr->u16WBGgain; /* index1: Ggain */
	g_init_wb_Gain[ViPipe][2] = pstInitAttr->u16WBBgain; /* index2: Bgain */
	g_sample_rgain[ViPipe] = pstInitAttr->u16SampleRgain;
	g_sample_bgain[ViPipe] = pstInitAttr->u16SampleBgain;
	g_is_ir_mode[ViPipe] = pstInitAttr->bIsIrMode;

	for (i = 0; i < CCM_MATRIX_SIZE; i++) {
		g_init_ccm[ViPipe][i] = pstInitAttr->au16CCM[i];
	}

	return GK_SUCCESS;
}

ISP_SNS_OBJ_S g_sns_sc500ai_obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = sc500ai_standby,
	.pfnRestart = sc500ai_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = sc500ai_write_register,
	.pfnReadReg = sc500ai_read_register,
	.pfnSetBusInfo = sc500ai_set_bus_info,
	.pfnSetInit = sensor_set_init
};
