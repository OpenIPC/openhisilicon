/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "f37_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define F37_ID 37

enum {
	EXP_L_IDX = 0,
	EXP_H_IDX,
	GAIN_IDX,
	VMAX_L_IDX,
	VMAX_H_IDX,
	BSUN_EN_IDX,
	BSUN_STR0_IDX,
	BSUN_STR1_IDX,
	REG_MAX_IDX
} F37_REG_INDEX_E;

#define EXP_ADDR_L 0x01
#define EXP_ADDR_H 0x02
#define GAIN_ADDR 0x00
#define VMAX_ADDR_L 0x22
#define VMAX_ADDR_H 0x23
#define BSUN_EN_ADDR 0x0C
#define BSUN_STR0_ADDR 0x2F
#define BSUN_STR1_ADDR 0x82

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

#define HIGH_8BITS(x) (((x)&0xFF00) >> 8)
#define LOW_8BITS(x) ((x)&0x00FF)
#define LOWER_4BITS(x) ((x)&0x000F)
#define HIGHER_4BITS(x) (((x)&0xF000) >> 12)
#define HIGHER_8BITS(x) (((x)&0x0FF0) >> 4)
#define ISO_LUT_NUM 8

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastF37[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define F37_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastF37[dev])
#define F37_SENSOR_SET_CTX(dev, pstCtx) (g_pastF37[dev] = pstCtx)
#define F37_SENSOR_RESET_CTX(dev) (g_pastF37[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunF37BusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32InitIso[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM -
							 1)] = 100 };

static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16InitCCM[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = { { 0 } };

static GK_BOOL g_abIsIrMode[ISP_MAX_PIPE_NUM] = { GK_FALSE };
static const GK_U32 g_au32IsoLut[ISO_LUT_NUM] = { 100,	200,  400,  800,
						  1600, 3200, 6400, 12800 };

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int f37_i2c_addr;
extern unsigned int f37_addr_byte;
extern unsigned int f37_data_byte;

extern void f37_init(VI_PIPE ViPipe);
extern void f37_exit(VI_PIPE ViPipe);
extern void f37_standby(VI_PIPE ViPipe);
extern void f37_restart(VI_PIPE ViPipe);
extern int f37_write_register(VI_PIPE ViPipe, int addr, int data);
extern int f37_read_register(VI_PIPE ViPipe, int addr);

/* sensor fps mode */
#define F37_SENSOR_FPS_MAX 30
#define F37_SENSOR_FPS_MIN 1
#define F37_1080P_30FPS_LINEAR_MODE (1)
#define F37_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)
#define F37_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require */
#define F37_VMAX_1080P30_LINEAR (1125 + F37_INCREASE_LINES)
#define F37_FULL_LINES_MAX 0xFFFF

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_U32 u32Fll = 0;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	if (pstSnsState->u8ImgMode == F37_1080P_30FPS_LINEAR_MODE) {
		u32Fll = F37_VMAX_1080P30_LINEAR;
	}

	pstSnsState->u32FLStd = u32Fll;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 0;
	pstAeSnsDft->u32FullLinesMax = F37_FULL_LINES_MAX;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

	if (g_au32LinesPer500ms[ViPipe] == 0) {
		pstAeSnsDft->u32LinesPer500ms =
			((GK_U64)(u32Fll * F37_SENSOR_FPS_MAX)) >> 1;
	} else {
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
	}

	pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
	pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

	pstAeSnsDft->bAERouteExValid = GK_FALSE;
	pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
	pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		pstAeSnsDft->au8HistThresh[0] = 0x0D; /* index 0 */
		pstAeSnsDft->au8HistThresh[1] = 0x28; /* index 1 */
		pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
		pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

		pstAeSnsDft->u8AeCompensation = 0x40;
		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859; /* init 148859 */
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32MaxIntTime =
			pstSnsState->u32FLStd - 4; /* sub 4 */
		pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
		pstAeSnsDft->u32MaxIntTimeTarget = F37_FULL_LINES_MAX;
		pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

		pstAeSnsDft->u32MaxAgain = 15872; /* 15872: 15.5x1024 */
		pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 0x400; /* accuracy: 10 */
		pstAeSnsDft->u32MinDgain = 0x400;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
		pstAeSnsDft->u32MinISPDgainTarget =
			1 << pstAeSnsDft->u32ISPDgainShift;
		pstAeSnsDft->u32MaxISPDgainTarget =
			4 << pstAeSnsDft->u32ISPDgainShift; /* max 4 */
		break;
	}

	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32VMAX = F37_VMAX_1080P30_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case F37_1080P_30FPS_LINEAR_MODE:
		if ((f32Fps <= F37_SENSOR_FPS_MAX) &&
		    (f32Fps >= F37_SENSOR_FPS_MIN)) {
			u32VMAX = F37_VMAX_1080P30_LINEAR * F37_SENSOR_FPS_MAX /
				  f32Fps;
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		pstAeSnsDft->u32LinesPer500ms =
			(F37_VMAX_1080P30_LINEAR * F37_SENSOR_FPS_MAX) >> 1;
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR, "Not support this Mode\n");
		break;
	}

	pstSnsState->u32FLStd = u32VMAX;
	pstSnsState->u32FLStd = (pstSnsState->u32FLStd > F37_FULL_LINES_MAX) ?
					F37_FULL_LINES_MAX :
					pstSnsState->u32FLStd;

	pstSnsState->astRegsInfo[0].astI2cData[VMAX_H_IDX].u32Data =
		HIGH_8BITS(pstSnsState->u32FLStd);
	pstSnsState->astRegsInfo[0].astI2cData[VMAX_L_IDX].u32Data =
		LOW_8BITS(pstSnsState->u32FLStd);

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4; /* sub 4 */
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines = (u32FullLines > F37_FULL_LINES_MAX) ?
			       F37_FULL_LINES_MAX :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 4; /* sub 4 */

	pstSnsState->astRegsInfo[0].astI2cData[VMAX_H_IDX].u32Data =
		HIGH_8BITS(u32FullLines);
	pstSnsState->astRegsInfo[0].astI2cData[VMAX_L_IDX].u32Data =
		LOW_8BITS(u32FullLines);

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->astRegsInfo[0].astI2cData[EXP_L_IDX].u32Data =
		LOW_8BITS(u32IntTime);
	pstSnsState->astRegsInfo[0].astI2cData[EXP_H_IDX].u32Data =
		HIGH_8BITS(u32IntTime);
}

#define AGAIN_SHIFT0 31
#define AGAIN_SHIFT1 15
static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	GK_U32 u32Again;
	GK_U32 u32Tmp = 0;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	u32Again = *pu32AgainLin >> 6; /* shift 6 */

	while (u32Again > AGAIN_SHIFT0) {
		u32Again = u32Again >> 1;
		u32Tmp++;
	}

	if (u32Again > AGAIN_SHIFT1) {
		u32Again = u32Again - 16; /* sub 16 */
	}

	*pu32AgainDb = (u32Tmp << 4) | u32Again; /* shift 4 */
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin,
				     GK_U32 *pu32DgainDb)
{
	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (u32Again < 0x10) { /* Gain < 2x, for black sun cancellation */
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_EN_IDX].u32Data =
			0x40;
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_STR0_IDX].u32Data =
			0x64;
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_STR1_IDX].u32Data =
			0x23;
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_EN_IDX].u32Data =
			0x00;
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_STR0_IDX].u32Data =
			0x44;
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_STR1_IDX].u32Data =
			0x21;
	}

	pstSnsState->astRegsInfo[0].astI2cData[GAIN_IDX].u32Data =
		LOW_8BITS(u32Again);

	return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,
				      AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
	CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

	genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
	g_au32MaxTimeGetCnt[ViPipe] = 0;

	return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

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
#define CALIBRATE_AWB_STATIC_RGAIN 416
#define CALIBRATE_AWB_STATIC_GGAIN 256
#define CALIBRATE_AWB_STATIC_BGAIN 421
#define CALIBRATE_AWB_P1 83
#define CALIBRATE_AWB_P2 150
#define CALIBRATE_AWB_Q1 (-23)
#define CALIBRATE_AWB_A1 173966
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-124230)

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
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
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
		if (g_abIsIrMode[ViPipe] == GK_TRUE) {
			memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableIr,
			       sizeof(AWB_AGC_TABLE_S));
		} else {
			memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable,
			       sizeof(AWB_AGC_TABLE_S));
		}
		break;
	}

	pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0]; /* rgain 0  */
	pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1]; /* ggain 1  */
	pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2]; /* bgain 2  */
	pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
	pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

	for (i = 0; i < CCM_MATRIX_SIZE; i++) {
		pstAwbSnsDft->au16InitCCM[i] = g_au16InitCCM[ViPipe][i];
	}

	return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));
	pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

	return GK_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = { { 378, 256, 430 },
						       { 439, 256, 439 } };

static GK_U32 Interpulate(GK_U32 u32Mid, GK_U32 u32Left, GK_U32 u32LValue,
			  GK_U32 u32Right, GK_U32 u32RValue)
{
	GK_U32 u32Value = 0;
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
	GK_U32 u32Level = 0;
	GK_U32 au32DrcInitStr0[ISO_LUT_NUM] = { DRC_INIT_STR_NA };
	GK_U32 au32DrcInitStr1[ISO_LUT_NUM] = { DRC_INIT_STR_IR };

	for (u32Level = 0; u32Level < ISO_LUT_NUM; u32Level++) {
		if (g_au32InitIso[ViPipe] <= g_au32IsoLut[u32Level]) {
			break;
		}
	}

	if (u32Level == ISO_LUT_NUM) {
		u32Level = ISO_LUT_NUM - 1;
	}

	if (g_abIsIrMode[ViPipe] == GK_TRUE) {
		if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
			g_stIspDRC.u16ManualStrength =
				au32DrcInitStr1[u32Level];
		} else {
			g_stIspDRC.u16ManualStrength =
				Interpulate(g_au32InitIso[ViPipe],
					    g_au32IsoLut[u32Level - 1],
					    au32DrcInitStr1[u32Level - 1],
					    g_au32IsoLut[u32Level],
					    au32DrcInitStr1[u32Level]);
		}
	} else {
		if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
			g_stIspDRC.u16ManualStrength =
				au32DrcInitStr0[u32Level];
		} else {
			g_stIspDRC.u16ManualStrength =
				Interpulate(g_au32InitIso[ViPipe],
					    g_au32IsoLut[u32Level - 1],
					    au32DrcInitStr0[u32Level - 1],
					    g_au32IsoLut[u32Level],
					    au32DrcInitStr0[u32Level]);
		}
	}
}

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstDef);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
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
		if (g_abIsIrMode[ViPipe] == GK_TRUE) {
			pstDef->pstLdci = &g_stIspLdciIr;
		} else {
			pstDef->pstLdci = &g_stIspLdci;
		}
		pstDef->unKey.bit1Gamma = 1;
		if (g_abIsIrMode[ViPipe] == GK_TRUE) {
			pstDef->pstGamma = &g_stIspGammaIr;
		} else {
			pstDef->pstGamma = &g_stIspGamma;
		}
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspGe;
		pstDef->unKey.bit1Dehaze = 1;
		if (g_abIsIrMode[ViPipe] == GK_TRUE) {
			pstDef->pstDehaze = &g_stIspDehazeIr;
		} else {
			pstDef->pstDehaze = &g_stIspDehaze;
		}
		pstDef->unKey.bit1Lcac = 1;
		pstDef->pstLcac = &g_stIspLCac;
		pstDef->unKey.bit1Rgbir = 0;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = F37_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case F37_1080P_30FPS_LINEAR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample =
			10; /* 10 bit  */
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel =
			4095; /* whitelevel:4095 */
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

	return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,
				       ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstBlackLevel);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;

	for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
		pstBlackLevel->au16BlackLevel[i] = 0x40;
	}

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps;
	GK_U32 u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines_5Fps =
		(F37_VMAX_1080P30_LINEAR * F37_SENSOR_FPS_MAX) / 5; /* 5 fps */
	u32MaxIntTime_5Fps = u32FullLines_5Fps - 4; /* sub 4 */

	if (bEnable) { /* setup for ISP pixel calibration mode */
		f37_write_register(ViPipe, VMAX_ADDR_H,
				   HIGH_8BITS(u32FullLines_5Fps));
		f37_write_register(ViPipe, VMAX_ADDR_L,
				   LOW_8BITS(u32FullLines_5Fps));
		f37_write_register(ViPipe, EXP_ADDR_H,
				   HIGH_8BITS(u32MaxIntTime_5Fps));
		f37_write_register(ViPipe, EXP_ADDR_L,
				   LOW_8BITS(u32MaxIntTime_5Fps));
		f37_write_register(ViPipe, GAIN_ADDR, 0x00); /* min AG */
	} else { /* setup for ISP 'normal mode' */
		f37_write_register(ViPipe, VMAX_ADDR_H,
				   HIGH_8BITS(pstSnsState->u32FLStd));
		f37_write_register(ViPipe, VMAX_ADDR_L,
				   LOW_8BITS(pstSnsState->u32FLStd));

		pstSnsState->bSyncInit = GK_FALSE;
	}
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		pstSnsState->u8ImgMode = F37_1080P_30FPS_LINEAR_MODE;
		pstSnsState->u32FLStd = F37_VMAX_1080P30_LINEAR;
		ISP_TRACE(MODULE_DBG_ERR, "linear mode\n");
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
		return GK_FAILURE;
	}

	memset(pstSnsState->au32WDRIntTime, 0,
	       sizeof(pstSnsState->au32WDRIntTime));

	return GK_SUCCESS;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe,
				     ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSnsRegsInfo);
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunF37BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax =
			2; /* Max delay 2 */
		pstSnsState->astRegsInfo[0].u32RegNum = REG_MAX_IDX;

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				f37_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = f37_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = f37_data_byte;
		}

		pstSnsState->astRegsInfo[0].astI2cData[EXP_L_IDX].u8DelayFrmNum =
			0;
		pstSnsState->astRegsInfo[0].astI2cData[EXP_L_IDX].u32RegAddr =
			EXP_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[EXP_H_IDX].u8DelayFrmNum =
			0;
		pstSnsState->astRegsInfo[0].astI2cData[EXP_H_IDX].u32RegAddr =
			EXP_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[GAIN_IDX].u8DelayFrmNum =
			0;
		pstSnsState->astRegsInfo[0].astI2cData[GAIN_IDX].u32RegAddr =
			GAIN_ADDR;
		pstSnsState->astRegsInfo[0]
			.astI2cData[VMAX_L_IDX]
			.u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[VMAX_L_IDX].u32RegAddr =
			VMAX_ADDR_L;
		pstSnsState->astRegsInfo[0]
			.astI2cData[VMAX_H_IDX]
			.u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[VMAX_H_IDX].u32RegAddr =
			VMAX_ADDR_H;
		pstSnsState->astRegsInfo[0]
			.astI2cData[BSUN_EN_IDX]
			.u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[BSUN_EN_IDX].u32RegAddr =
			BSUN_EN_ADDR;
		pstSnsState->astRegsInfo[0]
			.astI2cData[BSUN_STR0_IDX]
			.u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0]
			.astI2cData[BSUN_STR0_IDX]
			.u32RegAddr = BSUN_STR0_ADDR;
		pstSnsState->astRegsInfo[0]
			.astI2cData[BSUN_STR1_IDX]
			.u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0]
			.astI2cData[BSUN_STR1_IDX]
			.u32RegAddr = BSUN_STR1_ADDR;

		pstSnsState->bSyncInit = GK_TRUE;
	} else {
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
			    pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
				pstSnsState->astRegsInfo[0]
					.astI2cData[i]
					.bUpdate = GK_FALSE;
			} else {
				pstSnsState->astRegsInfo[0]
					.astI2cData[i]
					.bUpdate = GK_TRUE;
			}
		}
	}

	pstSnsRegsInfo->bConfig = GK_FALSE;
	memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0],
	       sizeof(ISP_SNS_REGS_INFO_S));
	memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0],
	       sizeof(ISP_SNS_REGS_INFO_S));

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
	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (F37_RES_IS_1080P(pstSensorImageMode->u16Width,
			     pstSensorImageMode->u16Height)) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (pstSensorImageMode->f32Fps <= F37_SENSOR_FPS_MAX) {
				u8SensorImageMode = F37_1080P_30FPS_LINEAR_MODE;
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
		ISP_TRACE(
			MODULE_DBG_ERR,
			"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
			pstSensorImageMode->u16Width,
			pstSensorImageMode->u16Height,
			pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
		return GK_FAILURE;
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

	F37_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = F37_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = F37_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[0] = F37_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[1] = F37_VMAX_1080P30_LINEAR;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = f37_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = f37_exit;
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

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static GK_S32 f37_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunF37BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	F37_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

	F37_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	F37_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	F37_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = F37_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, F37_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, F37_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, F37_ID);
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
		g_au32InitIso[ViPipe] = pstInitAttr->u32InitIso;
	}
	g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
	g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
	g_au16InitWBGain[ViPipe][0] =
		pstInitAttr->u16WBRgain; /* index0: Rgain */
	g_au16InitWBGain[ViPipe][1] =
		pstInitAttr->u16WBGgain; /* index1: Ggain */
	g_au16InitWBGain[ViPipe][2] =
		pstInitAttr->u16WBBgain; /* index2: Bgain */
	g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
	g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;
	g_abIsIrMode[ViPipe] = pstInitAttr->bIsIrMode;

	for (i = 0; i < CCM_MATRIX_SIZE; i++) {
		g_au16InitCCM[ViPipe][i] = pstInitAttr->au16CCM[i];
	}

	return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSoiF37Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = f37_standby,
	.pfnRestart = f37_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = f37_write_register,
	.pfnReadReg = f37_read_register,
	.pfnSetBusInfo = f37_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
