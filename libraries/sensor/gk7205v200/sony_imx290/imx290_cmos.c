/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__IMX290_CMOS_H_)
#define __IMX290_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#include "imx290_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IMX290_ID 290

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

ISP_SNS_STATE_S *g_pastImx290[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define IMX290_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastImx290[dev])
#define IMX290_SENSOR_SET_CTX(dev, pstCtx) (g_pastImx290[dev] = pstCtx)
#define IMX290_SENSOR_RESET_CTX(dev) (g_pastImx290[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunImx290BusInfo[ISP_MAX_PIPE_NUM] = {
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

extern const unsigned int imx290_i2c_addr;
extern unsigned int imx290_addr_byte;
extern unsigned int imx290_data_byte;

typedef struct IMX290_STATE_S {
	GK_U8 u8Hcg;
	GK_U32 u32BRL;
	GK_U32 u32RHS1_MAX;
	GK_U32 u32RHS2_MAX;
} IMX290_STATE_S;

IMX290_STATE_S g_astimx290State[ISP_MAX_PIPE_NUM] = { { 0 } };

extern void imx290_init(VI_PIPE ViPipe);
extern void imx290_exit(VI_PIPE ViPipe);
extern void imx290_standby(VI_PIPE ViPipe);
extern void imx290_restart(VI_PIPE ViPipe);
extern int imx290_write_register(VI_PIPE ViPipe, int addr, int data);
extern int imx290_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX290_FULL_LINES_MAX (0x3FFFF)
#define IMX290_FULL_LINES_MAX_2TO1_WDR \
	(0x8AA) // considering the YOUT_SIZE and bad frame
#define IMX290_FULL_LINES_MAX_3TO1_WDR (0x7FC)

/*****Imx290 Register Address*****/
#define IMX290_SHS1_ADDR (0x3020)
#define IMX290_SHS2_ADDR (0x3024)
#define IMX290_SHS3_ADDR (0x3028)
#define IMX290_GAIN_ADDR (0x3014)
#define IMX290_HCG_ADDR (0x3009)
#define IMX290_VMAX_ADDR (0x3018)
#define IMX290_HMAX_ADDR (0x301c)
#define IMX290_RHS1_ADDR (0x3030)
#define IMX290_RHS2_ADDR (0x3034)
#define IMX290_Y_OUT_SIZE_ADDR (0x3418)

#define IMX290_INCREASE_LINES \
	(1) /* make real fps less than stand fps because NVR require */

#define IMX290_VMAX_1080P30_LINEAR (1125 + IMX290_INCREASE_LINES)
#define IMX290_VMAX_720P60TO30_WDR (750 + IMX290_INCREASE_LINES)
#define IMX290_VMAX_1080P60TO30_WDR (1220 + IMX290_INCREASE_LINES)
#define IMX290_VMAX_1080P120TO30_WDR (1125 + IMX290_INCREASE_LINES)

// sensor fps mode
#define IMX290_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define IMX290_SENSOR_1080P_30FPS_3t1_WDR_MODE (2)
#define IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE (3)
#define IMX290_SENSOR_720P_30FPS_2t1_WDR_MODE (4)
#define IMX290_SENSOR_1080P_30FPS_2t1_FRAME_WDR_MODE (5)

#define IMX290_RES_IS_720P(w, h) ((w) <= 1280 && (h) <= 720)
#define IMX290_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

#define IMX290_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                    \
	do {                                                                      \
		ISP_TRACE(                                                        \
			MODULE_DBG_ERR,                                           \
			"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", \
			pstSensorImageMode->u16Width,                             \
			pstSensorImageMode->u16Height,                            \
			pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);      \
	} while (0)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = IMX290_FULL_LINES_MAX;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

	if (g_au32LinesPer500ms[ViPipe] == 0) {
		pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
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
	case WDR_MODE_NONE: /* linear mode */
		pstAeSnsDft->au8HistThresh[0] = 0xd;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 62564;
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 38577;
		pstAeSnsDft->u32MinDgain = 1024;
		pstAeSnsDft->u32MaxDgainTarget = 20013;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x38;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;

		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
		pstAeSnsDft->u32MinIntTime = 1;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 1;
		break;

	case WDR_MODE_2To1_FRAME:
		pstAeSnsDft->au8HistThresh[0] = 0xC;
		pstAeSnsDft->au8HistThresh[1] = 0x18;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u8AeCompensation = 0x18;
		pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
		pstAeSnsDft->u32MinIntTime = 1;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

		pstAeSnsDft->u32MaxAgain = 62564;
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 38577;
		pstAeSnsDft->u32MinDgain = 1024;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u16ManRatioEnable = GK_TRUE;
		pstAeSnsDft->au32Ratio[0] = 0x400;

		break;
	case WDR_MODE_2To1_LINE:
		pstAeSnsDft->au8HistThresh[0] = 0xC;
		pstAeSnsDft->au8HistThresh[1] = 0x18;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
		pstAeSnsDft->u32MinIntTime = 2;

		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

		pstAeSnsDft->u32MaxAgain = 62564;
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = 62564;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 38577;
		pstAeSnsDft->u32MinDgain = 1024;
		pstAeSnsDft->u32MaxDgainTarget = 20013;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				16462;

		if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
			pstAeSnsDft->u8AeCompensation = 56;
			pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
		} else {
			pstAeSnsDft->u32MaxDgainTarget = 8153;
			pstAeSnsDft->u32MaxISPDgainTarget = 512;
			pstAeSnsDft->u8AeCompensation = 24;
			pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
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
	GK_U32 u32VMAX = IMX290_VMAX_1080P30_LINEAR;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE:
		if ((f32Fps <= 30) && (f32Fps >= 16.5)) {
			u32VMAX = IMX290_VMAX_1080P60TO30_WDR * 30 /
				  DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		u32VMAX = (u32VMAX > IMX290_FULL_LINES_MAX_2TO1_WDR) ?
				  IMX290_FULL_LINES_MAX_2TO1_WDR :
				  u32VMAX;
		break;

	case IMX290_SENSOR_1080P_30FPS_LINEAR_MODE:
	case IMX290_SENSOR_1080P_30FPS_2t1_FRAME_WDR_MODE:
		if ((f32Fps <= 30) && (f32Fps >= 0.5)) {
			u32VMAX = IMX290_VMAX_1080P30_LINEAR * 30 /
				  DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		u32VMAX = (u32VMAX > IMX290_FULL_LINES_MAX) ?
				  IMX290_FULL_LINES_MAX :
				  u32VMAX;
		break;

	case IMX290_SENSOR_720P_30FPS_2t1_WDR_MODE:
		if ((f32Fps <= 30) && (f32Fps >= 0.5)) {
			u32VMAX = IMX290_VMAX_720P60TO30_WDR * 30 /
				  DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		u32VMAX = (u32VMAX > IMX290_FULL_LINES_MAX_2TO1_WDR) ?
				  IMX290_FULL_LINES_MAX_2TO1_WDR :
				  u32VMAX;
		break;

	default:
		return;
	}

	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			(u32VMAX & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			((u32VMAX & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
			((u32VMAX & 0xF0000) >> 16);
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			(u32VMAX & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			((u32VMAX & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			((u32VMAX & 0xF0000) >> 16);
	}

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstSnsState->u32FLStd = u32VMAX * 2;

		/*
            RHS1 limitation:
            2n + 5
            RHS1 <= FSC - BRL*2 -21
            (2 * VMAX_IMX290_1080P30_WDR - 2 * gu32BRL - 21) - (((2 * VMAX_IMX290_1080P30_WDR - 2 * 1109 - 21) - 5) %2)
        */
		g_astimx290State[ViPipe].u32RHS1_MAX =
			(u32VMAX - g_astimx290State[ViPipe].u32BRL) * 2 - 21;

	} else {
		pstSnsState->u32FLStd = u32VMAX;
	}

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		u32FullLines =
			(u32FullLines > 2 * IMX290_FULL_LINES_MAX_2TO1_WDR) ?
				2 * IMX290_FULL_LINES_MAX_2TO1_WDR :
				u32FullLines;
		pstSnsState->au32FL[0] = (u32FullLines >> 1) << 1;
		g_astimx290State[ViPipe].u32RHS1_MAX =
			pstSnsState->au32FL[0] -
			g_astimx290State[ViPipe].u32BRL * 2 - 21;
	}

	else if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
		u32FullLines = (u32FullLines > IMX290_FULL_LINES_MAX) ?
				       IMX290_FULL_LINES_MAX :
				       u32FullLines;
		pstSnsState->au32FL[0] = u32FullLines;
	} else {
		u32FullLines = (u32FullLines > IMX290_FULL_LINES_MAX) ?
				       IMX290_FULL_LINES_MAX :
				       u32FullLines;
		pstSnsState->au32FL[0] = u32FullLines;
	}

	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			(pstSnsState->au32FL[0] & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			((pstSnsState->au32FL[0] & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
			((pstSnsState->au32FL[0] & 0xF0000) >> 16);
	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			((pstSnsState->au32FL[0] >> 1) & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			(((pstSnsState->au32FL[0] >> 1) & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			(((pstSnsState->au32FL[0] >> 1) & 0xF0000) >> 16);
	}

	else if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			(pstSnsState->au32FL[0] & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			((pstSnsState->au32FL[0] & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			(((pstSnsState->au32FL[0] & 0xF0000)) >> 16);
	} else {
	}

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	static GK_BOOL bFirst[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM -
							   1)] = 1 };
	GK_U32 u32Value = 0;

	static GK_U32 u32ShortIntTime[ISP_MAX_PIPE_NUM] = { 0 };
	static GK_U32 u32LongIntTime[ISP_MAX_PIPE_NUM] = { 0 };

	static GK_U32 u32RHS1[ISP_MAX_PIPE_NUM] = { 0 };

	static GK_U32 u32SHS1[ISP_MAX_PIPE_NUM] = { 0 };
	static GK_U32 u32SHS2[ISP_MAX_PIPE_NUM] = { 0 };

	GK_U32 u32YOUTSIZE;

	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		if (bFirst[ViPipe]) { /* short exposure */
			pstSnsState->au32WDRIntTime[0] = u32IntTime;
			u32ShortIntTime[ViPipe] = u32IntTime;
			bFirst[ViPipe] = GK_FALSE;
		}

		else { /* long exposure */
			pstSnsState->au32WDRIntTime[1] = u32IntTime;
			u32LongIntTime[ViPipe] = u32IntTime;

			u32SHS2[ViPipe] = pstSnsState->au32FL[1] -
					  u32LongIntTime[ViPipe] - 1;

			// allocate the RHS1
			u32SHS1[ViPipe] = (u32ShortIntTime[ViPipe] % 2) + 2;
			u32RHS1[ViPipe] =
				u32ShortIntTime[ViPipe] + u32SHS1[ViPipe] + 1;

			if (pstSnsState->u8ImgMode ==
			    IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE) {
				u32YOUTSIZE =
					(1097 + (u32RHS1[ViPipe] - 1) / 2 + 7) *
					2;
				u32YOUTSIZE = (u32YOUTSIZE >= 0x1FFF) ?
						      0x1FFF :
						      u32YOUTSIZE;
			} else {
				u32YOUTSIZE =
					(720 + 9) * 2 + (u32RHS1[ViPipe] - 1);
			}

			pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
				(u32SHS1[ViPipe] & 0xFF);
			pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
				((u32SHS1[ViPipe] & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
				((u32SHS1[ViPipe] & 0xF0000) >> 16);

			pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
				(u32SHS2[ViPipe] & 0xFF);
			pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
				((u32SHS2[ViPipe] & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
				((u32SHS2[ViPipe] & 0xF0000) >> 16);

			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				(u32RHS1[ViPipe] & 0xFF);
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				((u32RHS1[ViPipe] & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[13].u32Data =
				((u32RHS1[ViPipe] & 0xF0000) >> 16);

			pstSnsState->astRegsInfo[0].astI2cData[14].u32Data =
				(u32YOUTSIZE & 0xFF);
			pstSnsState->astRegsInfo[0].astI2cData[15].u32Data =
				((u32YOUTSIZE & 0x1F00) >> 8);

			bFirst[ViPipe] = GK_TRUE;
		}

	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
		if (bFirst[ViPipe]) { /* short exposure */
			u32Value = pstSnsState->au32FL[0] - u32IntTime - 1;
			pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
				(u32Value & 0xFF);
			pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
				((u32Value & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
				((u32Value & 0x30000) >> 16);
			bFirst[ViPipe] = GK_FALSE;
		} else { /* long exposure */
			u32Value = pstSnsState->au32FL[0] - u32IntTime - 1;
			pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
				(u32Value & 0xFF);
			pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
				((u32Value & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
				((u32Value & 0x30000) >> 16);
			bFirst[ViPipe] = GK_TRUE;
		}
	}

	else {
		u32Value = pstSnsState->au32FL[0] - u32IntTime - 1;

		pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
			(u32Value & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
			((u32Value & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
			((u32Value & 0x30000) >> 16);

		bFirst[ViPipe] = GK_TRUE;
	}

	return;
}

static GK_U32 gain_table[262] = {
	1024,	 1059,	  1097,	   1135,    1175,    1217,    1259,    1304,
	1349,	 1397,	  1446,	   1497,    1549,    1604,    1660,    1719,
	1779,	 1842,	  1906,	   1973,    2043,    2048,    2119,    2194,
	2271,	 2351,	  2434,	   2519,    2608,    2699,    2794,    2892,
	2994,	 3099,	  3208,	   3321,    3438,    3559,    3684,    3813,
	3947,	 4086,	  4229,	   4378,    4532,    4691,    4856,    5027,
	5203,	 5386,	  5576,	   5772,    5974,    6184,    6402,    6627,
	6860,	 7101,	  7350,	   7609,    7876,    8153,    8439,    8736,
	9043,	 9361,	  9690,	   10030,   10383,   10748,   11125,   11516,
	11921,	 12340,	  12774,   13222,   13687,   14168,   14666,   15182,
	15715,	 16267,	  16839,   17431,   18043,   18677,   19334,   20013,
	20717,	 21445,	  22198,   22978,   23786,   24622,   25487,   26383,
	27310,	 28270,	  29263,   30292,   31356,   32458,   33599,   34780,
	36002,	 37267,	  38577,   39932,   41336,   42788,   44292,   45849,
	47460,	 49128,	  50854,   52641,   54491,   56406,   58388,   60440,
	62564,	 64763,	  67039,   69395,   71833,   74358,   76971,   79676,
	82476,	 85374,	  88375,   91480,   94695,   98023,   101468,  105034,
	108725,	 112545,  116501,  120595,  124833,  129220,  133761,  138461,
	143327,	 148364,  153578,  158975,  164562,  170345,  176331,  182528,
	188942,	 195582,  202455,  209570,  216935,  224558,  232450,  240619,
	249074,	 257827,  266888,  276267,  285976,  296026,  306429,  317197,
	328344,	 339883,  351827,  364191,  376990,  390238,  403952,  418147,
	432842,	 448053,  463799,  480098,  496969,  514434,  532512,  551226,
	570597,	 590649,  611406,  632892,  655133,  678156,  701988,  726657,
	752194,	 778627,  805990,  834314,  863634,  893984,  925400,  957921,
	991585,	 1026431, 1062502, 1099841, 1138491, 1178500, 1219916, 1262786,
	1307163, 1353100, 1400651, 1449872, 1500824, 1553566, 1608162, 1664676,
	1723177, 1783733, 1846417, 1911304, 1978472, 2048000, 2119971, 2194471,
	2271590, 2351418, 2434052, 2519590, 2608134, 2699789, 2794666, 2892876,
	2994538, 3099773, 3208706, 3321467, 3438190, 3559016, 3684087, 3813554,
	3947571, 4086297, 4229898, 4378546, 4532417, 4691696, 4856573, 5027243,
	5203912, 5386788, 5576092, 5772048, 5974890, 6184861, 6402210, 6627198,
	6860092, 7101170, 7350721, 7609041, 7876439, 8153234
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= gain_table[120]) {
		*pu32AgainLin = gain_table[120];
		*pu32AgainDb = 120;
		return;
	}

	for (i = 1; i < 121; i++) {
		if (*pu32AgainLin < gain_table[i]) {
			*pu32AgainLin = gain_table[i - 1];
			*pu32AgainDb = i - 1;
			break;
		}
	}
	return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin,
				     GK_U32 *pu32DgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32DgainLin);
	CMOS_CHECK_POINTER_VOID(pu32DgainDb);

	if (*pu32DgainLin >= gain_table[106]) {
		*pu32DgainLin = gain_table[106];
		*pu32DgainDb = 106;
		return;
	}

	for (i = 1; i < 106; i++) {
		if (*pu32DgainLin < gain_table[i]) {
			*pu32DgainLin = gain_table[i - 1];
			*pu32DgainDb = i - 1;
			break;
		}
	}

	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GK_U32 u32HCG = g_astimx290State[ViPipe].u8Hcg;
	GK_U32 u32Tmp;

	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (u32Again >= 21) {
		u32HCG = u32HCG | 0x10; // bit[4] HCG  .Reg0x3009[7:0]
		u32Again = u32Again - 21;
	}

	u32Tmp = u32Again + u32Dgain;

	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = (u32Tmp & 0xFF);
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = (u32HCG & 0xFF);

	return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable,
				    GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax,
				    GK_U32 *au32IntTimeMin,
				    GK_U32 *pu32LFMaxIntTime)
{
	GK_U32 i = 0;
	GK_U32 u32IntTimeMaxTmp0 = 0;
	GK_U32 u32IntTimeMaxTmp = 0;
	GK_U32 u32RatioTmp = 0x40;
	GK_U32 u32ShortTimeMinLimit = 0;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(au32Ratio);
	CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
	CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
	CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32ShortTimeMinLimit = 2;

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
		u32IntTimeMaxTmp = ((pstSnsState->au32FL[0] - 2) << 6) /
				   DIV_0_TO_1(au32Ratio[0]);
	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		/*  limitation for line base WDR

            SHS1 limitation:
            2 or more
            RHS1 - 2 or less

            SHS2 limitation:
            RHS1 + 2 or more
            FSC - 2 or less

            RHS1 Limitation
            2n + 5 (n = 0,1,2...)
            RHS1 <= FSC - BRL * 2 - 21

            short exposure time = RHS1 - (SHS1 + 1) <= RHS1 - 3
            long exposure time = FSC - (SHS2 + 1) <= FSC - (RHS1 + 3)
            ExposureShort + ExposureLong <= FSC - 6
            short exposure time <= (FSC - 6) / (ratio + 1)
        */
		if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
			u32IntTimeMaxTmp0 = pstSnsState->au32FL[1] - 6 -
					    pstSnsState->au32WDRIntTime[0];
			u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 10;
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ?
					u32IntTimeMaxTmp0 :
					u32IntTimeMaxTmp;
			au32IntTimeMax[0] = u32IntTimeMaxTmp;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			return;
		} else {
			u32IntTimeMaxTmp0 = ((pstSnsState->au32FL[1] - 6 -
					      pstSnsState->au32WDRIntTime[0]) *
					     0x40) /
					    DIV_0_TO_1(au32Ratio[0]);
			u32IntTimeMaxTmp =
				((pstSnsState->au32FL[0] - 6) * 0x40) /
				DIV_0_TO_1(au32Ratio[0] + 0x40);
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ?
					u32IntTimeMaxTmp0 :
					u32IntTimeMaxTmp;
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp >
				 (g_astimx290State[ViPipe].u32RHS1_MAX - 3)) ?
					(g_astimx290State[ViPipe].u32RHS1_MAX -
					 3) :
					u32IntTimeMaxTmp;
			u32IntTimeMaxTmp =
				(u32IntTimeMaxTmp == 0) ? 1 : u32IntTimeMaxTmp;
		}

	}

	else {
	}

	if (u32IntTimeMaxTmp >= u32ShortTimeMinLimit) {
		if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
			au32IntTimeMax[0] = u32IntTimeMaxTmp;
			au32IntTimeMax[1] = (pstSnsState->au32FL[0] - 2);
			au32IntTimeMin[0] = 2;
			au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >>
					    6;
		}
		if (IS_LINE_WDR_MODE(pstSnsState->enWDRMode)) {
			au32IntTimeMax[0] = u32IntTimeMaxTmp;
			au32IntTimeMax[1] = au32IntTimeMax[0] * au32Ratio[0] >>
					    6;
			au32IntTimeMax[2] = au32IntTimeMax[1] * au32Ratio[1] >>
					    6;
			au32IntTimeMax[3] = au32IntTimeMax[2] * au32Ratio[2] >>
					    6;
			au32IntTimeMin[0] = u32ShortTimeMinLimit;
			au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >>
					    6;
			au32IntTimeMin[2] = au32IntTimeMin[1] * au32Ratio[1] >>
					    6;
			au32IntTimeMin[3] = au32IntTimeMin[2] * au32Ratio[2] >>
					    6;
		} else {
		}
	} else {
		if (u16ManRatioEnable == 1) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Manaul ExpRatio is too large!\n");
			return;
		} else {
			u32IntTimeMaxTmp = u32ShortTimeMinLimit;

			if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
				for (i = 0x40; i <= 0xFFF; i++) {
					if ((u32IntTimeMaxTmp * i >> 6) >
					    (pstSnsState->au32FL[0] - 2)) {
						break;
					}
				}

				au32IntTimeMax[0] = u32IntTimeMaxTmp;
				au32IntTimeMax[1] =
					(pstSnsState->au32FL[0] - 2);
			} else if (pstSnsState->enWDRMode ==
				   WDR_MODE_2To1_LINE) {
				u32RatioTmp = 0xFFF;
				au32IntTimeMax[0] = u32IntTimeMaxTmp;
				au32IntTimeMax[1] =
					au32IntTimeMax[0] * u32RatioTmp >> 6;
			} else if (pstSnsState->enWDRMode ==
				   WDR_MODE_3To1_LINE) {
				for (i = 0x40; i <= 0xFFF; i++) {
					if ((u32IntTimeMaxTmp +
					     (u32IntTimeMaxTmp * i >> 6) +
					     (u32IntTimeMaxTmp * i * i >> 12)) >
					    (pstSnsState->au32FL[0] - 12)) {
						u32RatioTmp = i - 1;
						break;
					}
				}
				au32IntTimeMax[0] = u32IntTimeMaxTmp;
				au32IntTimeMax[1] =
					au32IntTimeMax[0] * u32RatioTmp >> 6;
				au32IntTimeMax[2] =
					au32IntTimeMax[1] * u32RatioTmp >> 6;
			} else {
			}
			au32IntTimeMin[0] = au32IntTimeMax[0];
			au32IntTimeMin[1] = au32IntTimeMax[1];
			au32IntTimeMin[2] = au32IntTimeMax[2];
			au32IntTimeMin[3] = au32IntTimeMax[3];
		}
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
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 4900;

	pstAwbSnsDft->au16GainOffset[0] = 0x1C3;
	pstAwbSnsDft->au16GainOffset[1] = 0x100;
	pstAwbSnsDft->au16GainOffset[2] = 0x100;
	pstAwbSnsDft->au16GainOffset[3] = 0x1D4;

	pstAwbSnsDft->as32WbPara[0] = -37;
	pstAwbSnsDft->as32WbPara[1] = 293;
	pstAwbSnsDft->as32WbPara[2] = 0;
	pstAwbSnsDft->as32WbPara[3] = 179537;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -123691;
	pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
	pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable,
		       sizeof(AWB_AGC_TABLE_S));
		break;

	case WDR_MODE_2To1_FRAME:
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
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
		pstDef->pstLdci = &g_stIspLdci;
		pstDef->unKey.bit1Gamma = 1;
		pstDef->pstGamma = &g_stIspGamma;
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspGe;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehaze;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCac;
		pstDef->unKey.bit1Rgbir = 0;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	case WDR_MODE_2To1_FRAME:
	case WDR_MODE_2To1_LINE:
		pstDef->unKey.bit1Demosaic = 1;
		pstDef->pstDemosaic = &g_stIspDemosaicWdr;
		pstDef->unKey.bit1Sharpen = 1;
		pstDef->pstSharpen = &g_stIspYuvSharpenWdr;
		pstDef->unKey.bit1Drc = 1;
		pstDef->pstDrc = &g_stIspDRCWDR;
		pstDef->unKey.bit1Gamma = 1;
		pstDef->pstGamma = &g_stIspGammaFSWDR;
		pstDef->unKey.bit1PreGamma = 0;
		pstDef->pstPreGamma = &g_stPreGamma;
		pstDef->unKey.bit1BayerNr = 1;
		pstDef->pstBayerNr = &g_stIspBayerNrWdr2To1;
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspWdrGe;
		pstDef->unKey.bit1AntiFalseColor = 1;
		pstDef->pstAntiFalseColor = &g_stIspWdrAntiFalseColor;
		pstDef->unKey.bit1Ldci = 1;
		pstDef->pstLdci = &g_stIspWdrLdci;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehazeWDR;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCacWdr;
		pstDef->unKey.bit1Rgbir = 0;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x40;
		pstDef->stWdrSwitchAttr.au32ExpRatio[1] = 0x40;
		pstDef->stWdrSwitchAttr.au32ExpRatio[2] = 0x40;
	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
		pstDef->stWdrSwitchAttr.au32ExpRatio[1] = 0x40;
		pstDef->stWdrSwitchAttr.au32ExpRatio[2] = 0x40;
	}

	pstDef->stSensorMode.u32SensorID = IMX290_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case IMX290_SENSOR_1080P_30FPS_LINEAR_MODE:
	case IMX290_SENSOR_1080P_30FPS_2t1_FRAME_WDR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
		break;

	case IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE:
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
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstBlackLevel);
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;

	/* black level of linear mode */
	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		for (i = 0; i < 4; i++) {
			pstBlackLevel->au16BlackLevel[i] = 0xF0; // 240
		}
	}

	/* black level of DOL mode */
	else {
		pstBlackLevel->au16BlackLevel[0] = 0xF0;
		pstBlackLevel->au16BlackLevel[1] = 0xF0;
		pstBlackLevel->au16BlackLevel[2] = 0xF0;
		pstBlackLevel->au16BlackLevel[3] = 0xF0;
	}

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		return;
	}

	else if (pstSnsState->enWDRMode == WDR_MODE_3To1_LINE) {
		return;
	}

	else {
		if ((pstSnsState->u8ImgMode ==
		     IMX290_SENSOR_1080P_30FPS_LINEAR_MODE) ||
		    (pstSnsState->u8ImgMode ==
		     IMX290_SENSOR_1080P_30FPS_2t1_FRAME_WDR_MODE)) {
			u32FullLines_5Fps =
				(IMX290_VMAX_1080P30_LINEAR * 30) / 5;
		}

		else {
			return;
		}
	}

	u32MaxIntTime_5Fps = 4;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		imx290_write_register(ViPipe, IMX290_GAIN_ADDR, 0x00);

		imx290_write_register(ViPipe, IMX290_VMAX_ADDR,
				      u32FullLines_5Fps & 0xFF);
		imx290_write_register(ViPipe, IMX290_VMAX_ADDR + 1,
				      (u32FullLines_5Fps & 0xFF00) >> 8);
		imx290_write_register(ViPipe, IMX290_VMAX_ADDR + 2,
				      (u32FullLines_5Fps & 0xF0000) >> 16);

		imx290_write_register(ViPipe, IMX290_SHS1_ADDR,
				      u32MaxIntTime_5Fps & 0xFF);
		imx290_write_register(ViPipe, IMX290_SHS1_ADDR + 1,
				      (u32MaxIntTime_5Fps & 0xFF00) >> 8);
		imx290_write_register(ViPipe, IMX290_SHS1_ADDR + 2,
				      (u32MaxIntTime_5Fps & 0xF0000) >> 16);

	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd = (pstSnsState->u32FLStd > 0x1FFFF) ?
						0x1FFFF :
						pstSnsState->u32FLStd;
		imx290_write_register(ViPipe, IMX290_VMAX_ADDR,
				      pstSnsState->u32FLStd & 0xFF);
		imx290_write_register(ViPipe, IMX290_VMAX_ADDR + 1,
				      (pstSnsState->u32FLStd & 0xFF00) >> 8);
		imx290_write_register(ViPipe, IMX290_VMAX_ADDR + 2,
				      (pstSnsState->u32FLStd & 0xF0000) >> 16);
		pstSnsState->bSyncInit = GK_FALSE;
	}

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode & 0x3F) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		printf("linear mode\n");
		break;

	case WDR_MODE_2To1_FRAME:
		pstSnsState->enWDRMode = WDR_MODE_2To1_FRAME;
		printf("2to1 half-rate frame WDR mode\n");
		break;

	case WDR_MODE_2To1_LINE:
		pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
		printf("2to1 line WDR mode\n");
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
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunImx290BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 8;

		if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
			pstSnsState->astRegsInfo[0].u32RegNum += 3;
			pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		}

		else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			pstSnsState->astRegsInfo[0].u32RegNum += 8;
			pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		}

		else if (pstSnsState->enWDRMode == WDR_MODE_3To1_LINE) {
			pstSnsState->astRegsInfo[0].u32RegNum += 14;
			pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		}

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				imx290_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = imx290_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = imx290_data_byte;
		}

		// Linear Mode Regs
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			IMX290_SHS1_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			IMX290_SHS1_ADDR + 1;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			IMX290_SHS1_ADDR + 2;

		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum =
			0; // make shutter and gain effective at the same time
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			IMX290_GAIN_ADDR; // gain
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			IMX290_HCG_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			IMX290_VMAX_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			IMX290_VMAX_ADDR + 1;
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			IMX290_VMAX_ADDR + 2;

		if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
			pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
				IMX290_SHS1_ADDR;
			pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
				IMX290_SHS1_ADDR + 1;
			pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
				IMX290_SHS1_ADDR + 2;

			pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
				IMX290_VMAX_ADDR;
			pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
				IMX290_VMAX_ADDR + 1;
			pstSnsState->astRegsInfo[0]
				.astI2cData[10]
				.u8DelayFrmNum = 0;
			pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr =
				IMX290_VMAX_ADDR + 2;

		}
		// DOL 2t1 Mode Regs
		else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum =
				0;

			pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
				IMX290_SHS2_ADDR;
			pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
				IMX290_SHS2_ADDR + 1;
			pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum =
				0;
			pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
				IMX290_SHS2_ADDR + 2;

			pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
				IMX290_VMAX_ADDR;
			pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
				IMX290_VMAX_ADDR + 1;
			pstSnsState->astRegsInfo[0]
				.astI2cData[10]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr =
				IMX290_VMAX_ADDR + 2;

			pstSnsState->astRegsInfo[0]
				.astI2cData[11]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr =
				IMX290_RHS1_ADDR;
			pstSnsState->astRegsInfo[0]
				.astI2cData[12]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr =
				IMX290_RHS1_ADDR + 1;
			pstSnsState->astRegsInfo[0]
				.astI2cData[13]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr =
				IMX290_RHS1_ADDR + 2;

			pstSnsState->astRegsInfo[0]
				.astI2cData[14]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr =
				IMX290_Y_OUT_SIZE_ADDR;
			pstSnsState->astRegsInfo[0]
				.astI2cData[15]
				.u8DelayFrmNum = 1;
			pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr =
				IMX290_Y_OUT_SIZE_ADDR + 1;
		}

		pstSnsState->bSyncInit = GK_TRUE;
	} else {
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
			    pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
				pstSnsState->astRegsInfo[0]
					.astI2cData[i]
					.bUpdate = GK_FALSE;
			}

			else {
				pstSnsState->astRegsInfo[0]
					.astI2cData[i]
					.bUpdate = GK_TRUE;
			}
		}

		if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
			pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[1].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[2].bUpdate =
				GK_TRUE;

			pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate =
				GK_TRUE;
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
	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= 30) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (IMX290_RES_IS_1080P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					IMX290_SENSOR_1080P_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd =
					IMX290_VMAX_1080P30_LINEAR;
				g_astimx290State[ViPipe].u8Hcg = 0x2;
			} else {
				IMX290_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
			if (IMX290_RES_IS_1080P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					IMX290_SENSOR_1080P_30FPS_2t1_FRAME_WDR_MODE;
				pstSnsState->u32FLStd =
					IMX290_VMAX_1080P30_LINEAR;
				g_astimx290State[ViPipe].u8Hcg = 0x2;
			} else {
				IMX290_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			if (IMX290_RES_IS_720P(pstSensorImageMode->u16Width,
					       pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					IMX290_SENSOR_720P_30FPS_2t1_WDR_MODE;
				pstSnsState->u32FLStd =
					IMX290_VMAX_720P60TO30_WDR * 2;
				g_astimx290State[ViPipe].u32BRL = 735;
			} else if (IMX290_RES_IS_1080P(
					   pstSensorImageMode->u16Width,
					   pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE;
				pstSnsState->u32FLStd =
					IMX290_VMAX_1080P60TO30_WDR * 2;
				g_astimx290State[ViPipe].u32BRL = 1109;
			} else {
				IMX290_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}

			g_astimx290State[ViPipe].u8Hcg = 0x1;
		} else if (pstSnsState->enWDRMode == WDR_MODE_3To1_LINE) {
			if (IMX290_RES_IS_1080P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					IMX290_SENSOR_1080P_30FPS_3t1_WDR_MODE;
				pstSnsState->u32FLStd =
					IMX290_VMAX_1080P120TO30_WDR * 4;
				g_astimx290State[ViPipe].u32BRL = 1109;
				g_astimx290State[ViPipe].u8Hcg = 0x0;
			} else {
				IMX290_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else {
			IMX290_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
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

	IMX290_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = IMX290_SENSOR_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = IMX290_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[0] = IMX290_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[1] = IMX290_VMAX_1080P30_LINEAR;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = imx290_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = imx290_exit;
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

static GK_S32 imx290_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunImx290BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	IMX290_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	IMX290_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	IMX290_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	IMX290_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = IMX290_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, IMX290_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, IMX290_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, IMX290_ID);
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

ISP_SNS_OBJ_S stSnsImx290Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = imx290_standby,
	.pfnRestart = imx290_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = imx290_write_register,
	.pfnReadReg = imx290_read_register,
	.pfnSetBusInfo = imx290_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __IMX290_CMOS_H_ */
