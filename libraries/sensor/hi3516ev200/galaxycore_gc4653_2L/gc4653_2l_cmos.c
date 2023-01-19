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
#include "gc4653_2l_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define GC4653_2L_ID 4653

/****************************************************************************
 * global variables                                                         *
 ****************************************************************************/
#define HIGH_8BITS(x) (((x)&0xFF00) >> 8)
#define LOW_8BITS(x) ((x)&0x00FF)
#define LOWER_4BITS(x) ((x)&0x000F)
#define HIGHER_4BITS(x) (((x)&0xF000) >> 12)
#define HIGHER_8BITS(x) (((x)&0x0FF0) >> 4)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastGc4653_2l[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define GC4653_2L_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastGc4653_2l[dev])
#define GC4653_2L_SENSOR_SET_CTX(dev, pstCtx) (g_pastGc4653_2l[dev] = pstCtx)
#define GC4653_2L_SENSOR_RESET_CTX(dev) (g_pastGc4653_2l[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunGc4653_2l_BusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 g_u32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };

static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern unsigned char gc4653_2l_i2c_addr;
extern unsigned int gc4653_2l_addr_byte;
extern unsigned int gc4653_2l_data_byte;

extern void gc4653_2l_init(VI_PIPE ViPipe);
extern void gc4653_2l_exit(VI_PIPE ViPipe);
extern void gc4653_2l_standby(VI_PIPE ViPipe);
extern void gc4653_2l_restart(VI_PIPE ViPipe);
extern int gc4653_2l_write_register(VI_PIPE ViPipe, int addr, int data);
extern int gc4653_2l_read_register(VI_PIPE ViPipe, int addr);
extern void gc4653_2l_mirror_flip(VI_PIPE ViPipe,
				  ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);

/****************************************************************************
 * local variables                                                          *
 ****************************************************************************/
#define GC4653_2L_FULL_LINES_MAX (0x3FFF)

#define GC4653_2L_EXPTIME_ADDR_H (0x0202) // Shutter-time[13:8]
#define GC4653_2L_EXPTIME_ADDR_L (0x0203) // Shutter-time[7:0]
#define GC4653_2L_AGAIN_ADDR_EXT0 (0x02b3) // Analog-gain[7:0]
#define GC4653_2L_AGAIN_ADDR_EXT1 (0x02b4)
#define GC4653_2L_AGAIN_ADDR_EXT2 (0x02b8)
#define GC4653_2L_AGAIN_ADDR_EXT3 (0x02b9)
#define GC4653_2L_AGAIN_ADDR_EXT4 (0x0515)
#define GC4653_2L_AGAIN_ADDR_EXT5 (0x0519)
#define GC4653_2L_AGAIN_ADDR_EXT6 (0x02D9)

#define GC4653_2L_DGAIN_ADDR_H (0x020e) // digital-gain[11:6]
#define GC4653_2L_DGAIN_ADDR_L (0x020f) // digital-gain[5:0]

#define GC4653_2L_VMAX_ADDR_H (0x0340) // Vmax[13:8]
#define GC4653_2L_VMAX_ADDR_L (0x0341) // Vmax[7:0]

#define GC4653_2L_INCREASE_LINES \
	(1) /* make real fps less than stand fps because NVR require*/
#define GC4653_2L_VMAX_4M30_LINEAR (1500 + GC4653_2L_INCREASE_LINES)

/* sensor fps mode */
#define GC4653_2L_SENSOR_4M_30FPS_LINEAR_MODE (0)

#define GC4653_2L_SHUTTER_MAX (0x3fdb)
#define GC4653_2L_AGAIN_MIN (1024)
#define GC4653_2L_AGAIN_MAX \
	(77660) // the max again is 77660 mean 75.84 recommend
#define GC4653_2L_DGAIN_MIN (1024)
#define GC4653_2L_DGAIN_MAX \
	(16368) // the max dgain is 16368 mean 15.98 recommend
#define GC4653_2L_AGAIN_MAX_INDEX 25

#define GC4653_2L_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                 \
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
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = GC4653_2L_FULL_LINES_MAX;
	pstAeSnsDft->u8AERunInterval = 1;
	pstAeSnsDft->u32InitExposure = 10;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 0.015625;

	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

	pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
	pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

	pstAeSnsDft->bAERouteExValid = GK_FALSE;
	pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
	pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

	if (g_au32LinesPer500ms[ViPipe] == 0) {
		pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
	} else {
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
	}
	switch (pstSnsState->enWDRMode) {
	case WDR_MODE_NONE:
		pstAeSnsDft->au8HistThresh[0] = 0x0D;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = GC4653_2L_AGAIN_MAX;
		pstAeSnsDft->u32MinAgain = GC4653_2L_AGAIN_MIN;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain =
			512; /* if Dgain enable,please set ispdgain bigger than 1 */
		pstAeSnsDft->u32MinDgain = 64;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x40;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
		pstAeSnsDft->u32MinIntTime = 1;
		pstAeSnsDft->u32MaxIntTimeTarget =
			GC4653_2L_SHUTTER_MAX; // 65535
		pstAeSnsDft->u32MinIntTimeTarget = 1;
		break;
	default:
		printf("cmos_get_ae_default_Sensor Mode is error!\n");
		break;
	}

	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32VMAX = GC4653_2L_VMAX_4M30_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case GC4653_2L_SENSOR_4M_30FPS_LINEAR_MODE:
		if ((f32Fps <= 30.0) && (f32Fps >= 2.75)) {
			u32VMAX = GC4653_2L_VMAX_4M30_LINEAR * 30 /
				  DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		break;
	default:
		break;
	}
	u32VMAX = (u32VMAX > GC4653_2L_FULL_LINES_MAX) ?
			  GC4653_2L_FULL_LINES_MAX :
			  u32VMAX;
	pstSnsState->u32FLStd = u32VMAX;
	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
		((u32VMAX & 0xFF00) >> 8);
	pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = (u32VMAX & 0xFF);

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = GC4653_2L_VMAX_4M30_LINEAR * 15;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);
	u32FullLines = (u32FullLines > GC4653_2L_FULL_LINES_MAX) ?
			       GC4653_2L_FULL_LINES_MAX :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;

	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
		((u32FullLines & 0xFF00) >> 8);
	pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
		(u32FullLines & 0xFF);

	pstSnsState->u32FLStd = u32FullLines;

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val = (u32IntTime > GC4653_2L_FULL_LINES_MAX) ?
			 GC4653_2L_SHUTTER_MAX :
			 u32IntTime;

	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
		((u32Val & 0xFF00) >> 8);
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32Val & 0xFF);
	return;
}

#define GAIN_TABLE_MAX 26
#define GAIN_TABLE_COL 7
static GK_U32 regValTable[GAIN_TABLE_MAX][GAIN_TABLE_COL] = {
	{ 0x00, 0x00, 0x01, 0x00, 0x30, 0x1e, 0x5C },
	{ 0x20, 0x00, 0x01, 0x0B, 0x30, 0x1e, 0x5C },
	{ 0x01, 0x00, 0x01, 0x19, 0x30, 0x1d, 0x5B },
	{ 0x21, 0x00, 0x01, 0x2A, 0x30, 0x1e, 0x5C },
	{ 0x02, 0x00, 0x02, 0x00, 0x30, 0x1e, 0x5C },
	{ 0x22, 0x00, 0x02, 0x17, 0x30, 0x1d, 0x5B },
	{ 0x03, 0x00, 0x02, 0x33, 0x20, 0x16, 0x54 },
	{ 0x23, 0x00, 0x03, 0x14, 0x20, 0x17, 0x55 },
	{ 0x04, 0x00, 0x04, 0x00, 0x20, 0x17, 0x55 },
	{ 0x24, 0x00, 0x04, 0x2F, 0x20, 0x19, 0x57 },
	{ 0x05, 0x00, 0x05, 0x26, 0x20, 0x19, 0x57 },
	{ 0x25, 0x00, 0x06, 0x28, 0x20, 0x1b, 0x59 },
	{ 0x0c, 0x00, 0x08, 0x00, 0x20, 0x1d, 0x5B },
	{ 0x2C, 0x00, 0x09, 0x1E, 0x20, 0x1f, 0x5D },
	{ 0x0D, 0x00, 0x0B, 0x0C, 0x20, 0x21, 0x5F },
	{ 0x2D, 0x00, 0x0D, 0x11, 0x20, 0x24, 0x62 },
	{ 0x1C, 0x00, 0x10, 0x00, 0x20, 0x26, 0x64 },
	{ 0x3C, 0x00, 0x12, 0x3D, 0x18, 0x2a, 0x68 },
	{ 0x5C, 0x00, 0x16, 0x19, 0x18, 0x2c, 0x6A },
	{ 0x7C, 0x00, 0x1A, 0x22, 0x18, 0x2e, 0x6C },
	{ 0x9C, 0x00, 0x20, 0x00, 0x18, 0x32, 0x70 },
	{ 0xBC, 0x00, 0x25, 0x3A, 0x18, 0x35, 0x73 },
	{ 0xDC, 0x00, 0x2C, 0x33, 0x10, 0x36, 0x74 },
	{ 0xFC, 0x00, 0x35, 0x05, 0x10, 0x38, 0x76 },
	{ 0x1C, 0x01, 0x40, 0x00, 0x10, 0x3c, 0x7A },
	{ 0x3C, 0x01, 0x4B, 0x35, 0x10, 0x42, 0x80 },
};

static GK_U32 analog_gain_table[GAIN_TABLE_MAX] = {
	1024,  1213,  1434,  1699,  2048,  2427,  2867,	 3398,	4096,
	4854,  5734,  6795,  8192,  9708,  11469, 13591, 16384, 19415,
	22938, 27181, 32768, 38830, 45875, 54362, 65536, 77660
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int again;
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	again = *pu32AgainLin;

	for (i = 0; i < GC4653_2L_AGAIN_MAX_INDEX; i++) {
		if (again < analog_gain_table[i + 1]) {
			break;
		}
	}

	*pu32AgainLin = analog_gain_table[i];
	*pu32AgainDb = i;

	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	GK_U8 u8DgainHigh, u8DgainLow;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u8DgainHigh = (u32Dgain >> 6) & 0x0f;
	u8DgainLow = (u32Dgain & 0x3f) << 2;

	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
		regValTable[u32Again][0];
	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
		regValTable[u32Again][1];
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
		regValTable[u32Again][2];
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
		regValTable[u32Again][3];
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
		regValTable[u32Again][4];
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
		regValTable[u32Again][5];
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
		regValTable[u32Again][6];

	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = u8DgainHigh;
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = u8DgainLow;

	return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable,
				    GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax,
				    GK_U32 *au32IntTimeMin,
				    GK_U32 *pu32LFMaxIntTime)
{
	return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,
				      AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
	CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

	genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
	g_u32MaxTimeGetCnt[ViPipe] = 0;

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
	pstExpFuncs->pfn_cmos_dgain_calc_table = GK_NULL;
	pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
	pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

	return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

/* AWB default parameter and function */
#define CALIBRATE_STATIC_WB_R_GAIN 404
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN 509

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 12
#define CALIBRATE_AWB_P2 213
#define CALIBRATE_AWB_Q1 -30
#define CALIBRATE_AWB_A1 186383
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 -131654

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 5082;

	pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;
	pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
	pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN;
	pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;

	pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;
	pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;
	pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1;
	pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1;
	pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1;
	pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1;
	pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
	pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;
	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable,
		       sizeof(AWB_AGC_TABLE_S));
		break;
	}
	pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
	pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];
	pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
	pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
	pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
	pstAwbSnsDft->u8AWBRunInterval = 4; /* need fix 4 */

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
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
		pstDef->unKey.bit1Lcac = 1;
		pstDef->pstLcac = &g_stIspLCac;
		pstDef->unKey.bit1Rgbir = 0;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = GC4653_2L_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case GC4653_2L_SENSOR_4M_30FPS_LINEAR_MODE:
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
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;
	pstBlackLevel->au16BlackLevel[0] = 256;
	pstBlackLevel->au16BlackLevel[1] = 256;
	pstBlackLevel->au16BlackLevel[2] = 256;
	pstBlackLevel->au16BlackLevel[3] = 256;

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);
	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		printf("linear mode\n");
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
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((GK_FALSE == pstSnsState->bSyncInit) ||
	    (GK_FALSE == pstSnsRegsInfo->bConfig)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunGc4653_2l_BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 13;
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				gc4653_2l_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = gc4653_2l_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = gc4653_2l_data_byte;
		}
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			GC4653_2L_EXPTIME_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			GC4653_2L_EXPTIME_ADDR_L;

		//gain
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT0;

		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT1;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT2;
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT3;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT4;
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT5;
		pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
			GC4653_2L_AGAIN_ADDR_EXT6;

		pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
			GC4653_2L_DGAIN_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr =
			GC4653_2L_DGAIN_ADDR_L;

		//vmax
		pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr =
			GC4653_2L_VMAX_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr =
			GC4653_2L_VMAX_ADDR_L;

		pstSnsState->bSyncInit = GK_TRUE;
	} else {
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum - 2;
		     i++) {
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

		if ((pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate ==
		     GK_TRUE)) {
			pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate =
				GK_TRUE;
		}

		if ((pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[1].astI2cData[6].bUpdate ==
		     GK_TRUE)) {
			pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[1].bUpdate =
				GK_TRUE;
		}

		if ((pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate ==
		     GK_TRUE)) {
			pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate =
				GK_TRUE;
		}

		if ((pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate ==
		     GK_TRUE)) {
			pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate =
				GK_TRUE;
		}

		if ((pstSnsState->astRegsInfo[0].astI2cData[2].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[8].bUpdate ==
		     GK_TRUE)) {
			pstSnsState->astRegsInfo[0].astI2cData[2].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[8].bUpdate =
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
	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if ((pstSensorImageMode->u16Width <= 2560) &&
	    (pstSensorImageMode->u16Height <= 1440)) {
		if (pstSensorImageMode->f32Fps <= 30) {
			u8SensorImageMode =
				GC4653_2L_SENSOR_4M_30FPS_LINEAR_MODE;
			pstSnsState->u32FLStd = GC4653_2L_VMAX_4M30_LINEAR;
		} else {
			GC4653_2L_ERR_MODE_PRINT(pstSensorImageMode,
						 pstSnsState);
			return GK_FAILURE;
		}
	} else {
		GC4653_2L_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
		return GK_FAILURE;
	}

	if ((GK_TRUE == pstSnsState->bInit) &&
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

	GC4653_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = GC4653_2L_SENSOR_4M_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = GC4653_2L_VMAX_4M30_LINEAR;
	pstSnsState->au32FL[0] = GC4653_2L_VMAX_4M30_LINEAR;
	pstSnsState->au32FL[1] = GC4653_2L_VMAX_4M30_LINEAR;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
	return;
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);
	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));
	pstSensorExpFunc->pfn_cmos_sensor_init = gc4653_2l_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = gc4653_2l_exit;
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
static GK_S32 gc4653_2l_set_bus_info(VI_PIPE ViPipe,
				     ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunGc4653_2l_BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	GC4653_2L_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

	if (GK_NULL == pastSnsStateCtx) {
		pastSnsStateCtx =
			(ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
		if (GK_NULL == pastSnsStateCtx) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] SnsCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

	GC4653_2L_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	GC4653_2L_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	GC4653_2L_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = GC4653_2L_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, GC4653_2L_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, GC4653_2L_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret =
		GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, GC4653_2L_ID);
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

ISP_SNS_OBJ_S stSnsgc4653_2lObj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = gc4653_2l_standby,
	.pfnRestart = gc4653_2l_restart,
	.pfnMirrorFlip = gc4653_2l_mirror_flip,
	.pfnWriteReg = gc4653_2l_write_register,
	.pfnReadReg = gc4653_2l_read_register,
	.pfnSetBusInfo = gc4653_2l_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
