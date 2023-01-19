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
#include "sc223a_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SC223A_ID 0xcb3e
#define HIGH_8BITS(x) (((x)&0xFF00) >> 8)
#define LOW_8BITS(x) ((x)&0x00FF)
#define LOWER_4BITS(x) (((x)&0x000F) << 4)
#define HIGHER_4BITS(x) (((x)&0xF000) >> 12)
#define HIGHER_8BITS(x) (((x)&0x0FF0) >> 4)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSc223a[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define SC223A_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSc223a[dev])
#define SC223A_SENSOR_SET_CTX(dev, pstCtx) (g_pastSc223a[dev] = pstCtx)
#define SC223A_SENSOR_RESET_CTX(dev) (g_pastSc223a[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc223aBusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 au32WDRIntTime[4] = { 0 };

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int sc223a_i2c_addr;
extern unsigned int sc223a_addr_byte;
extern unsigned int sc223a_data_byte;

extern void sc223a_init(VI_PIPE ViPipe);
extern void sc223a_exit(VI_PIPE ViPipe);
extern void sc223a_standby(VI_PIPE ViPipe);
extern void sc223a_restart(VI_PIPE ViPipe);
extern int sc223a_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sc223a_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define SC223A_FULL_LINES_MAX_LINEAR (0x3FFFF)
#define SC223A_FULL_LINES_MAX_2TO1_WDR (0x1194)

/******** SC223a Register Address ********/
#define SC223A_EXP_ADDR (0x3E00)
#define SC223A_SEXP_ADDR (0x3E04)
#define SC223A_AGAIN_ADDR (0x3E09)
#define SC223A_SAGAIN_ADDR (0x3E12)
#define SC223A_DGAIN_ADDR (0x3E06)
#define SC223A_SDGAIN_ADDR (0x3E10)
#define SC223A_VMAX_ADDR (0x320E)
#define SC223A_SEXP_MAX_ADDR (0x3E23)

#define SC223A_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require*/

#define SC223A_VMAX_1080P30_LINEAR (1125 + SC223A_INCREASE_LINES)
#define SC223A_VMAX_1080P60TO30_WDR (2250 + SC223A_INCREASE_LINES)
#define SC223A_FRAME_RATE_MIN (0x34BC)
#define EXP_OFFSET_LINEAR (8)
#define EXP_OFFSET_WDR (13)
#define SC223A_SEXP_MAX_DEFAULT (0x88)

//sensor fps mode
#define SC223A_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define SC223A_SENSOR_1080P_30FPS_2t1_WDR_MODE (2)

#define SC223A_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

#define SC223A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                    \
	do {                                                                      \
		ISP_TRACE(                                                        \
			MODULE_DBG_ERR,                                           \
			"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", \
			pstSensorImageMode->u16Width,                             \
			pstSensorImageMode->u16Height,                            \
			pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);      \
	} while (0)

static GK_U32 gu32MaxShortEXP = SC223A_SEXP_MAX_DEFAULT * 2;

GK_U16 Dgain_mapping_realgain_to_sensorgain(GK_FLOAT gain_real);

GK_U16 Dgain_mapping_realgain_to_sensorgain(GK_FLOAT gain_real)
{
	GK_U16 u16sensorgain;
	GK_U32 u32Dgain = gain_real * 128;
	GK_U8 dCoarseGain = 0;
	GK_U8 dFineGain = 0;
	GK_U8 u8Reg0x3e06 = 0;
	for (dCoarseGain = 1; dCoarseGain <= 16;
	     dCoarseGain = dCoarseGain * 2) //1,2,4,8,16
	{
		if (u32Dgain < (128 * 2 * dCoarseGain)) {
			break;
		}
	}
	dFineGain = u32Dgain / dCoarseGain;

	for (; dCoarseGain >= 2; dCoarseGain = dCoarseGain / 2) {
		u8Reg0x3e06 = (u8Reg0x3e06 << 1) | 0x01;
	}
	u16sensorgain = ((GK_U16)u8Reg0x3e06) << 8;
	u16sensorgain += dFineGain;

	return u16sensorgain;
}

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_U32 u32Fll = 0;
	GK_FLOAT f32maxFps = 0;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	if (pstSnsState->u8ImgMode == SC223A_SENSOR_1080P_30FPS_LINEAR_MODE) {
		u32Fll = SC223A_VMAX_1080P30_LINEAR * 2;
		f32maxFps = 30;
		pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
		pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
		pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
	} else if (pstSnsState->u8ImgMode ==
		   SC223A_SENSOR_1080P_30FPS_2t1_WDR_MODE) {
		u32Fll = SC223A_VMAX_1080P60TO30_WDR * 2;
		f32maxFps = 30;
		pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
		pstAeSnsDft->stIntTimeAccu.f32Accuracy = 2;
		pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
	}

	pstSnsState->u32FLStd = u32Fll;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = SC223A_FULL_LINES_MAX_LINEAR * 2;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125; //1/128

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0078125; //1/128

	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

	pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
	pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

	pstAeSnsDft->bAERouteExValid = GK_FALSE;
	pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
	pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

	if (g_au32LinesPer500ms[ViPipe] == 0) {
		pstAeSnsDft->u32LinesPer500ms =
			pstSnsState->u32FLStd * f32maxFps / 2;
	} else {
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
	}

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		pstAeSnsDft->au8HistThresh[0] = 0x0D;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 7414; //  1.81*128*32
		pstAeSnsDft->u32MinAgain = 0x80;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 512; //4*128
		pstAeSnsDft->u32MinDgain = 0x80;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x40;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;

		pstAeSnsDft->u32MaxIntTime =
			pstSnsState->u32FLStd - EXP_OFFSET_LINEAR;
		pstAeSnsDft->u32MinIntTime = 2;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 2;
		break;

	case WDR_MODE_2To1_LINE:
		pstAeSnsDft->au8HistThresh[0] = 0x0C;
		pstAeSnsDft->au8HistThresh[1] = 0x18;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 7414; //  1.81*128*32
		pstAeSnsDft->u32MinAgain = 128;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 512; //4*128
		pstAeSnsDft->u32MinDgain = 128;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x40;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;

		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd -
					     gu32MaxShortEXP - EXP_OFFSET_WDR;
		pstAeSnsDft->u32MinIntTime = 1;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 1;
		if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
			pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
		} else {
			pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
			pstAeSnsDft->u32ExpRatioMax = 0x400;
			pstAeSnsDft->u32ExpRatioMin = 0x400;
			pstAeSnsDft->u16ManRatioEnable = GK_FALSE;
			pstAeSnsDft->au32Ratio[0] = 0x400;
			pstAeSnsDft->au32Ratio[1] = 0x40;
			pstAeSnsDft->au32Ratio[2] = 0x40;
		}
		break;
	}

	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_FLOAT f32maxFps;
	GK_U32 u32FullLines;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case SC223A_SENSOR_1080P_30FPS_LINEAR_MODE:
		f32maxFps = 30;
		if ((f32Fps <= 30) && (f32Fps >= 2.5)) { /* Minimum 2.5fps */
			u32FullLines = SC223A_VMAX_1080P30_LINEAR * f32maxFps /
				       DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		u32FullLines = (u32FullLines > SC223A_FULL_LINES_MAX_LINEAR) ?
				       SC223A_FULL_LINES_MAX_LINEAR :
				       u32FullLines;
		break;
	case SC223A_SENSOR_1080P_30FPS_2t1_WDR_MODE:
		f32maxFps = 30;
		if ((f32Fps <= f32maxFps) && (f32Fps >= 15)) {
			u32FullLines = SC223A_VMAX_1080P60TO30_WDR * f32maxFps /
				       DIV_0_TO_1_FLOAT(f32Fps);
			gu32MaxShortEXP = (SC223A_SEXP_MAX_DEFAULT * f32maxFps /
					   DIV_0_TO_1_FLOAT(f32Fps));
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		u32FullLines = (u32FullLines > SC223A_FULL_LINES_MAX_2TO1_WDR) ?
				       SC223A_FULL_LINES_MAX_2TO1_WDR :
				       u32FullLines;
		break;

	default:
		printf("Not support this Mode\n");
		return;
		break;
	}

	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
		HIGH_8BITS(u32FullLines);
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
		LOW_8BITS(u32FullLines);

	pstSnsState->u32FLStd = u32FullLines << 1;
	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstAeSnsDft->u32MaxIntTime =
			pstSnsState->u32FLStd - EXP_OFFSET_LINEAR;
	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstSnsState->astRegsInfo[0].astI2cData[13].u32Data =
			HIGH_8BITS(gu32MaxShortEXP);
		pstSnsState->astRegsInfo[0].astI2cData[14].u32Data =
			LOW_8BITS(gu32MaxShortEXP);
		gu32MaxShortEXP = gu32MaxShortEXP << 1;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd -
					     gu32MaxShortEXP - EXP_OFFSET_WDR;
	}

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines = (u32FullLines + EXP_OFFSET_LINEAR) >> 1;
	u32FullLines = (u32FullLines > SC223A_FRAME_RATE_MIN) ?
			       SC223A_FRAME_RATE_MIN :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines << 1;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
		HIGH_8BITS(u32FullLines);
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
		LOW_8BITS(u32FullLines);

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstAeSnsDft->u32MaxIntTime =
			pstSnsState->au32FL[0] - EXP_OFFSET_LINEAR;
	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] -
					     gu32MaxShortEXP - EXP_OFFSET_WDR;
	}
	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	static GK_BOOL bFirst = GK_TRUE;
	static GK_U32 u32ShortIntTime = 0;
	static GK_U32 u32LongIntTime = 0;
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		if (bFirst) { /* short exposure */
			au32WDRIntTime[0] = u32IntTime;
			u32ShortIntTime = u32IntTime;
			pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
				HIGHER_8BITS(u32ShortIntTime);
			pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
				LOWER_4BITS(u32ShortIntTime);
			bFirst = GK_FALSE;
		} else { /* long exposure */
			au32WDRIntTime[1] = u32IntTime;
			u32LongIntTime = u32IntTime;
			pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
				HIGHER_4BITS(u32LongIntTime);
			pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
				HIGHER_8BITS(u32LongIntTime);
			pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
				LOWER_4BITS(u32LongIntTime);
			bFirst = GK_TRUE;
		}
	} else if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
			HIGHER_4BITS(u32IntTime);
		pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
			HIGHER_8BITS(u32IntTime);
		pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
			LOWER_4BITS(u32IntTime);
	}

	return;
}

static const GK_U16 u16AgainTab[64] = {
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
	int again;
	int i;
	static GK_U8 againmax = 63;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	again = *pu32AgainLin;

	if (again >= u16AgainTab[againmax]) {
		*pu32AgainDb = againmax;
	} else {
		for (i = 1; i < 64; i++) {
			if (again < u16AgainTab[i]) {
				*pu32AgainDb = i - 1;
				break;
			}
		}
	}
	*pu32AgainLin = u16AgainTab[*pu32AgainDb];

	return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin,
				     GK_U32 *pu32DgainDb)
{
	CMOS_CHECK_POINTER_VOID(pu32DgainLin);
	CMOS_CHECK_POINTER_VOID(pu32DgainDb);

	*pu32DgainDb = *pu32DgainLin / 1024;
	if (*pu32DgainDb == 3) {
		*pu32DgainDb = 2;
	} else if (*pu32DgainDb >= 4 && *pu32DgainDb < 8) {
		*pu32DgainDb = 4;
	} else if (*pu32DgainDb >= 8) {
		*pu32DgainDb = 8;
	}

	*pu32DgainLin = *pu32DgainDb * 1024;

	return;
}

static GK_FLOAT Again_real_cal[] = { 128,	  1,	       231.68,
				     1.802,	  231.68 * 2,  1.802 * 2,
				     231.68 * 4,  1.802 * 4,   231.68 * 8,
				     1.802 * 8,	  231.68 * 16, 1.802 * 16,
				     231.68 * 32, 1.802 * 32 };

static GK_FLOAT Again_reg_table[] = { 128,	   00,	 231.68,      0x40,
				      231.68 * 2,  0x41, 231.68 * 4,  0x43,
				      231.68 * 8,  0x47, 231.68 * 16, 0x4f,
				      231.68 * 32, 0x5f };

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	GK_U16 u16Again, u16Dgain;
	GK_FLOAT f_Dgain; //f_Again

	//GK_U8 Again_temp;
	GK_U8 Again_index;
	for (Again_index = 12; Again_index >= 0; Again_index -= 2) {
		if (u32Again >= Again_real_cal[Again_index])
			break;
	}

	u16Again = Again_reg_table[Again_index + 1];
	f_Dgain = (u32Again / Again_real_cal[Again_index]) * u32Dgain / 128.0;
	u16Dgain = Dgain_mapping_realgain_to_sensorgain(f_Dgain);
	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
		(u16Dgain & 0xFF00) >> 8;
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u16Dgain & 0x00FF;
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = u16Again & 0x00FF;

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			(u16Dgain & 0xFF00) >> 8;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = u16Dgain &
								     0x00FF;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = u16Again &
								     0x00FF;

	} else {
	}

	return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable,
				    GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax,
				    GK_U32 *au32IntTimeMin,
				    GK_U32 *pu32LFMaxIntTime)
{
	GK_U32 u32ShortTimeMinLimit = 0;
	GK_U32 u32ShortTimeMaxlimit = 0;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(au32Ratio);
	CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
	CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
	CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);
	u32ShortTimeMinLimit = 1;

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
			au32IntTimeMax[0] = u32ShortTimeMinLimit;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			return;
		} else if (genFSWDRMode[ViPipe] ==
			   ISP_FSWDR_AUTO_LONG_FRAME_MODE) {
			u32ShortTimeMaxlimit =
				((((pstSnsState->au32FL[0]) * 0x40) /
					  DIV_0_TO_1(au32Ratio[0] + 0x40) +
				  1) *
				 2) /
				2;
			u32ShortTimeMaxlimit = (u32ShortTimeMaxlimit >
						(gu32MaxShortEXP - 10)) ?
						       (gu32MaxShortEXP - 10) :
						       u32ShortTimeMaxlimit;
			u32ShortTimeMaxlimit =
				((u32ShortTimeMaxlimit *
				  DIV_0_TO_1(au32Ratio[0]) / 0x40) >
				 (pstSnsState->au32FL[0] - gu32MaxShortEXP -
				  EXP_OFFSET_WDR)) ?
					((pstSnsState->au32FL[0] -
					  gu32MaxShortEXP - EXP_OFFSET_WDR) *
					 0x40 / DIV_0_TO_1(au32Ratio[0])) :
					u32ShortTimeMaxlimit;
			*pu32LFMaxIntTime =
				pstSnsState->au32FL[0] - gu32MaxShortEXP - 10;
			au32IntTimeMax[0] = u32ShortTimeMaxlimit;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			return;
		} else {
			u32ShortTimeMaxlimit =
				((((pstSnsState->au32FL[0]) * 0x40) /
					  DIV_0_TO_1(au32Ratio[0] + 0x40) +
				  1) *
				 2) /
				2;
			u32ShortTimeMaxlimit = (u32ShortTimeMaxlimit >
						(gu32MaxShortEXP - 10)) ?
						       (gu32MaxShortEXP - 10) :
						       u32ShortTimeMaxlimit;
			u32ShortTimeMaxlimit =
				((u32ShortTimeMaxlimit *
				  DIV_0_TO_1(au32Ratio[0]) / 0x40) >
				 (pstSnsState->au32FL[0] - gu32MaxShortEXP -
				  EXP_OFFSET_WDR)) ?
					((pstSnsState->au32FL[0] -
					  gu32MaxShortEXP - EXP_OFFSET_WDR) *
					 0x40 / DIV_0_TO_1(au32Ratio[0])) :
					u32ShortTimeMaxlimit;
			au32IntTimeMax[0] = u32ShortTimeMaxlimit;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			return;
		}
	} else {
	}

	return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,
				      AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
	CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

	genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
	gu32MaxTimeGetCnt[ViPipe] = 0;

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
	pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
	pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

	return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 5000;

	pstAwbSnsDft->au16GainOffset[0] = 478;
	pstAwbSnsDft->au16GainOffset[1] = 256;
	pstAwbSnsDft->au16GainOffset[2] = 256;
	pstAwbSnsDft->au16GainOffset[3] = 497;

	pstAwbSnsDft->as32WbPara[0] = -102;
	pstAwbSnsDft->as32WbPara[1] = 358;
	pstAwbSnsDft->as32WbPara[2] = 0;
	pstAwbSnsDft->as32WbPara[3] = 183957;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -135938;
	pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
	pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable,
		       sizeof(AWB_AGC_TABLE_S));
		break;
	case WDR_MODE_2To1_LINE:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmFsWdr,
		       sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR,
		       sizeof(AWB_AGC_TABLE_S));
		break;
	}

	pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
	pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
	pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
	pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
	pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

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

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstDef);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

	pstDef->unKey.bit1Ca = 1;
	pstDef->pstCa = &g_stIspCA;
	pstDef->unKey.bit1Dpc = 1;
	pstDef->pstDpc = &g_stCmosDpc;
	pstDef->unKey.bit1Wdr = 1;
	pstDef->pstWdr = &g_stIspWDR;
	pstDef->unKey.bit1Lsc = 0;
	pstDef->pstLsc = &g_stCmosLsc;
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
		pstDef->unKey.bit1Gamma = 1;
		pstDef->pstGamma = &g_stIspGamma;
#ifdef CONFIG_ISP_CR_SUPPORT
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspGe;
#endif
		pstDef->unKey.bit1AntiFalseColor = 1;
		pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
		pstDef->unKey.bit1Ldci = 1;
		pstDef->pstLdci = &g_stIspLdci;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehaze;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCac;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	case WDR_MODE_2To1_LINE:
		pstDef->unKey.bit1Demosaic = 1;
		pstDef->pstDemosaic = &g_stIspDemosaicWdr;
		pstDef->unKey.bit1Sharpen = 1;
		pstDef->pstSharpen = &g_stIspYuvSharpenWdr;
		pstDef->unKey.bit1Drc = 1;
		pstDef->pstDrc = &g_stIspDRCWDR;
		pstDef->unKey.bit1Gamma = 1;
		pstDef->pstGamma = &g_stIspGammaFSWDR;
#ifdef CONFIG_ISP_PREGAMMA_SUPPORT
		pstDef->unKey.bit1PreGamma = 0;
		pstDef->pstPreGamma = &g_stPreGamma;
#endif
		pstDef->unKey.bit1BayerNr = 1;
		pstDef->pstBayerNr = &g_stIspBayerNrWdr2To1;
#ifdef CONFIG_ISP_CR_SUPPORT
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspWdrGe;
#endif
		pstDef->unKey.bit1AntiFalseColor = 1;
		pstDef->pstAntiFalseColor = &g_stIspWdrAntiFalseColor;
		pstDef->unKey.bit1Ldci = 1;
		pstDef->pstLdci = &g_stIspWdrLdci;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehazeWDR;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCacWdr;
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x100;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = SC223A_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x100;
	}

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case SC223A_SENSOR_1080P_30FPS_LINEAR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023;
		break;
	case SC223A_SENSOR_1080P_30FPS_2t1_WDR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023;
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
		.u16RepeatPatternDimRows = 2;
	pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
		.u16RepeatPatternDimCols = 2;
	pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2;
	pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2;
	pstDef->stSensorMode.stDngRawFormat.enCfaLayout =
		CFALAYOUT_TYPE_RECTANGULAR;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
	pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
	pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;

	return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,
				       ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstBlackLevel);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;

	/* black level of linear mode */
	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstBlackLevel->au16BlackLevel[0] = 0x103;
		pstBlackLevel->au16BlackLevel[1] = 0x103;
		pstBlackLevel->au16BlackLevel[2] = 0x103;
		pstBlackLevel->au16BlackLevel[3] = 0x103;
	} else { /* black level of HDR mode */
		pstBlackLevel->au16BlackLevel[0] = 0x103;
		pstBlackLevel->au16BlackLevel[1] = 0x103;
		pstBlackLevel->au16BlackLevel[2] = 0x103;
		pstBlackLevel->au16BlackLevel[3] = 0x103;
	}

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		return;
	} else {
		if (SC223A_SENSOR_1080P_30FPS_LINEAR_MODE ==
		    pstSnsState->u8ImgMode) {
			u32FullLines_5Fps = SC223A_VMAX_1080P30_LINEAR * 30 / 5;
		} else {
			return;
		}
	}

	u32MaxIntTime_5Fps = u32FullLines_5Fps * 2 - EXP_OFFSET_LINEAR;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		sc223a_write_register(ViPipe, SC223A_VMAX_ADDR,
				      HIGH_8BITS(u32FullLines_5Fps)); /* 5fps */
		sc223a_write_register(ViPipe, SC223A_VMAX_ADDR + 1,
				      LOW_8BITS(u32FullLines_5Fps)); /* 5fps */
		sc223a_write_register(
			ViPipe, SC223A_EXP_ADDR,
			HIGHER_4BITS(
				u32MaxIntTime_5Fps)); /* max exposure lines */
		sc223a_write_register(
			ViPipe, SC223A_EXP_ADDR + 1,
			HIGHER_8BITS(
				u32MaxIntTime_5Fps)); /* max exposure lines */
		sc223a_write_register(
			ViPipe, SC223A_EXP_ADDR + 2,
			LOWER_4BITS(
				u32MaxIntTime_5Fps)); /* max exposure lines */

		sc223a_write_register(ViPipe, SC223A_DGAIN_ADDR, 0x00);
		sc223a_write_register(ViPipe, SC223A_DGAIN_ADDR + 1, 0x80);
		sc223a_write_register(ViPipe, SC223A_AGAIN_ADDR, 0x00);
		// sc223a_write_register(ViPipe, SC223A_AGAIN_ADDR+1, 0x40);
	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd >
			 SC223A_FULL_LINES_MAX_LINEAR * 2) ?
				(SC223A_FULL_LINES_MAX_LINEAR * 2) :
				pstSnsState->u32FLStd;
		pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
		sc223a_write_register(ViPipe, SC223A_VMAX_ADDR,
				      HIGH_8BITS(pstSnsState->u32FLStd >> 1));
		sc223a_write_register(ViPipe, SC223A_VMAX_ADDR + 1,
				      LOW_8BITS(pstSnsState->u32FLStd >> 1));
		pstSnsState->bSyncInit = GK_FALSE;
	}

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode & 0x3F) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		printf("linear mode\n");
		break;
	case WDR_MODE_2To1_LINE:
		pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
		printf("2to1 line WDR 1080p mode(60fps->30fps)\n");
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
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunSc223aBusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 8;

		if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			pstSnsState->astRegsInfo[0].u32RegNum += 7;
		}

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				sc223a_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = sc223a_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = sc223a_data_byte;
		}

		//Linear Mode Regs
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			SC223A_EXP_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			SC223A_EXP_ADDR + 1;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			SC223A_EXP_ADDR + 2;

		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			SC223A_DGAIN_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			SC223A_DGAIN_ADDR + 1;

		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			SC223A_AGAIN_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			SC223A_VMAX_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			SC223A_VMAX_ADDR + 1;

		if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
				SC223A_SEXP_ADDR; // Short shutter
			pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
				SC223A_SEXP_ADDR + 1; // Short shutter
			pstSnsState->astRegsInfo[0]
				.astI2cData[10]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr =
				SC223A_SDGAIN_ADDR; // Short Dgain
			pstSnsState->astRegsInfo[0]
				.astI2cData[11]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr =
				SC223A_SDGAIN_ADDR + 1; // Short Dgain
			pstSnsState->astRegsInfo[0]
				.astI2cData[12]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr =
				SC223A_SAGAIN_ADDR; // Short Again

			pstSnsState->astRegsInfo[0]
				.astI2cData[13]
				.u8DelayFrmNum = 2;
			pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr =
				SC223A_SEXP_MAX_ADDR; // Short EXP MAX
			pstSnsState->astRegsInfo[0]
				.astI2cData[14]
				.u8DelayFrmNum = 2;
			pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr =
				SC223A_SEXP_MAX_ADDR + 1; // Short EXP MAX
		}
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
	GK_U8 u8SensorImageMode = 0;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSensorImageMode);
	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= 30) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (SC223A_RES_IS_1080P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					SC223A_SENSOR_1080P_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd =
					SC223A_VMAX_1080P30_LINEAR * 2;
			} else {
				SC223A_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			if (SC223A_RES_IS_1080P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					SC223A_SENSOR_1080P_30FPS_2t1_WDR_MODE;
				pstSnsState->u32FLStd =
					SC223A_VMAX_1080P60TO30_WDR * 2;
			} else {
				SC223A_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else {
			SC223A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
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
	memset(pstSnsState->au32WDRIntTime, 0,
	       sizeof(pstSnsState->au32WDRIntTime));

	return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC223A_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = SC223A_SENSOR_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = SC223A_VMAX_1080P30_LINEAR * 2;
	pstSnsState->au32FL[0] = SC223A_VMAX_1080P30_LINEAR * 2;
	pstSnsState->au32FL[1] = SC223A_VMAX_1080P30_LINEAR * 2;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = 0x8c;
	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0x20;
	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = 0x80;
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x04;
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x65;

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x08; //vts
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0xca;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x80;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
			0x00; //gain
		pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = 0x88;
	}
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = sc223a_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = sc223a_exit;
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

static GK_S32 sensor_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunSc223aBusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC223A_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	SC223A_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC223A_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	SC223A_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = SC223A_ID;
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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC223A_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC223A_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC223A_ID);
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
	CMOS_CHECK_POINTER(pstInitAttr);

	g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
	g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
	g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
	g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
	g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
	g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
	g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;

	return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSc223aObj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = sc223a_standby,
	.pfnRestart = sc223a_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = sc223a_write_register,
	.pfnReadReg = sc223a_read_register,
	.pfnSetBusInfo = sensor_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif /* End of #ifdef __cplusplus */
