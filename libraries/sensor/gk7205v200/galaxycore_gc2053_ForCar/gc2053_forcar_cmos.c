/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__GC2053_FORCAR_CMOS_H_)
#define __GC2053_FORCAR_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "gc2053_forcar_cmos_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define GC2053_ID 2053

/****************************************************************************
 * global variables                                                            *
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

ISP_SNS_STATE_S *g_pastGc2053_forcar[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define GC2053_FORCAR_SENSOR_GET_CTX(dev, pstCtx) \
	(pstCtx = g_pastGc2053_forcar[dev])
#define GC2053_FORCAR_SENSOR_SET_CTX(dev, pstCtx) \
	(g_pastGc2053_forcar[dev] = pstCtx)
#define GC2053_FORCAR_SENSOR_RESET_CTX(dev) (g_pastGc2053_forcar[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunGc2053_forcar_BusInfo[ISP_MAX_PIPE_NUM] = {
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

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int gc2053_forcar_i2c_addr;
extern unsigned int gc2053_forcar_addr_byte;
extern unsigned int gc2053_forcar_data_byte;

extern void gc2053_forcar_init(VI_PIPE ViPipe);
extern void gc2053_forcar_exit(VI_PIPE ViPipe);
extern void gc2053_forcar_standby(VI_PIPE ViPipe);
extern void gc2053_forcar_restart(VI_PIPE ViPipe);
extern int gc2053_forcar_write_register(VI_PIPE ViPipe, int addr, int data);
extern int gc2053_forcar_read_register(VI_PIPE ViPipe, int addr);
extern void gc2053_forcar_mirror_flip(VI_PIPE ViPipe,
				      ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

#define GC2053_FULL_LINES_MAX (0x3FFF)

/*****SC2235 Register Address*****/
#define GC2053_EXPTIME_ADDR_H (0x03) // Shutter-time[13:8]
#define GC2053_EXPTIME_ADDR_L (0x04) // Shutter-time[7:0]

#define GC2053_AGAIN_ADDR_H (0xb4) // Analog-gain[9:8]
#define GC2053_AGAIN_ADDR_L (0xb3) // Analog-gain[7:0]

#define GC2053_DGAIN_ADDR_H (0xb9) // digital-gain[11:6]
#define GC2053_DGAIN_ADDR_L (0xb8) // digital-gain[5:0]

#define GC2053_AUTO_PREGAIN_ADDR_H (0xb1) // auto-pregain-gain[9:6]
#define GC2053_AUTO_PREGAIN_ADDR_L (0xb2) // auto-pregain-gain[5:0]

#define GC2053_VMAX_ADDR_H (0x41) // Vmax[13:8]
#define GC2053_VMAX_ADDR_L (0x42) // Vmax[7:0]
#define GC2053_STATUS_ADDR (0x90) // slow shutter via framerate or exptime

#define GC2053_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require */
#define GC2053_VMAX_1080P30_LINEAR (1125 + GC2053_INCREASE_LINES)

/* sensor fps mode */
#define GC2053_SENSOR_1080P_30FPS_LINEAR_MODE (0)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = GC2053_FULL_LINES_MAX;
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

		pstAeSnsDft->u32MaxAgain = 113168;
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 512;
		pstAeSnsDft->u32MinDgain = 64;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x38;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
		pstAeSnsDft->u32MinIntTime = 3;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 3;
		break;
	default:
		printf("cmos_get_ae_default_Sensor Mode is error!\n");
		break;
	}

	return 0;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32VMAX = GC2053_VMAX_1080P30_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case GC2053_SENSOR_1080P_30FPS_LINEAR_MODE:
		if ((f32Fps <= 30) && (f32Fps > 2.06)) {
			u32VMAX = GC2053_VMAX_1080P30_LINEAR * 30 /
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
	u32VMAX = (u32VMAX > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX :
						      u32VMAX;
	pstSnsState->u32FLStd = u32VMAX;
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x0;
	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (u32VMAX & 0xFF);
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
		((u32VMAX & 0xFF00) >> 8);

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = GC2053_VMAX_1080P30_LINEAR * 15;
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
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);
	u32FullLines = (u32FullLines > GC2053_FULL_LINES_MAX) ?
			       GC2053_FULL_LINES_MAX :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;

	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x0;
	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
		(u32FullLines & 0xFF);
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
		((u32FullLines & 0xFF00) >> 8);

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val = (u32IntTime > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX :
							u32IntTime;

	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (u32Val & 0xFF);
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
		((u32Val & 0xFF00) >> 8);
	return;
}

static GK_U32 regValTable[29][4] = {
	{ 0x00, 0x00, 0x01, 0x00 }, // 0xb4 0xb3 0xb8 0xb9
	{ 0x00, 0x10, 0x01, 0x0c }, { 0x00, 0x20, 0x01, 0x1b },
	{ 0x00, 0x30, 0x01, 0x2c }, { 0x00, 0x40, 0x01, 0x3f },
	{ 0x00, 0x50, 0x02, 0x16 }, { 0x00, 0x60, 0x02, 0x35 },
	{ 0x00, 0x70, 0x03, 0x16 }, { 0x00, 0x80, 0x04, 0x02 },
	{ 0x00, 0x90, 0x04, 0x31 }, { 0x00, 0xa0, 0x05, 0x32 },
	{ 0x00, 0xb0, 0x06, 0x35 }, { 0x00, 0xc0, 0x08, 0x04 },
	{ 0x00, 0x5a, 0x09, 0x19 }, { 0x00, 0x83, 0x0b, 0x0f },
	{ 0x00, 0x93, 0x0d, 0x12 }, { 0x00, 0x84, 0x10, 0x00 },
	{ 0x00, 0x94, 0x12, 0x3a }, { 0x01, 0x2c, 0x1a, 0x02 },
	{ 0x01, 0x3c, 0x1b, 0x20 }, { 0x00, 0x8c, 0x20, 0x0f },
	{ 0x00, 0x9c, 0x26, 0x07 }, { 0x02, 0x64, 0x36, 0x21 },
	{ 0x02, 0x74, 0x37, 0x3a }, { 0x00, 0xc6, 0x3d, 0x02 },
	{ 0x00, 0xdc, 0x3f, 0x3f }, { 0x02, 0x85, 0x3f, 0x3f },
	{ 0x02, 0x95, 0x3f, 0x3f }, { 0x00, 0xce, 0x3f, 0x3f },
};

static GK_U32 analog_gain_table[29] = {
	1024,  1230,  1440,  1730,  2032,  2380,  2880,	 3460,	 4080,	 4800,
	5776,  6760,  8064,  9500,  11552, 13600, 16132, 18912,	 22528,	 27036,
	32340, 38256, 45600, 53912, 63768, 76880, 92300, 108904, 123568,
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int again;
	int i;
	static GK_U8 againmax = 28;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	again = *pu32AgainLin;

	if (again >= analog_gain_table[againmax]) {
		*pu32AgainLin = analog_gain_table[28];
		*pu32AgainDb = againmax;
	} else {
		for (i = 1; i < 29; i++) {
			if (again < analog_gain_table[i]) {
				*pu32AgainLin = analog_gain_table[i - 1];
				*pu32AgainDb = i - 1;
				break;
			}
		}
	}

	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	GK_U8 u8DgainHigh, u8DgainLow;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
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

	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = u8DgainLow;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8DgainHigh;

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
	pstExpFuncs->pfn_cmos_dgain_calc_table = GK_NULL;
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
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 4908;

	pstAwbSnsDft->au16GainOffset[0] = 388; // R
	pstAwbSnsDft->au16GainOffset[1] = 256; // G
	pstAwbSnsDft->au16GainOffset[2] = 256; // G
	pstAwbSnsDft->au16GainOffset[3] = 519; // B

	pstAwbSnsDft->as32WbPara[0] = 39;
	pstAwbSnsDft->as32WbPara[1] = 152;
	pstAwbSnsDft->as32WbPara[2] = -65;
	pstAwbSnsDft->as32WbPara[3] = 201612;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -147924;
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
	pstAwbSnsDft->u8AWBRunInterval = 4;

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
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

	pstDef->unKey.bit1Ca = 1;
	pstDef->pstCa = &g_stIspCA;
	pstDef->unKey.bit1Dpc = 1;
	pstDef->pstDpc = &g_stCmosDpc;
	pstDef->unKey.bit1Wdr = 1;
	pstDef->pstWdr = &g_stIspWDR;
	pstDef->unKey.bit1Lsc = 1;
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
	}

	pstDef->stSensorMode.u32SensorID = GC2053_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case GC2053_SENSOR_1080P_30FPS_LINEAR_MODE:
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
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
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

	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);
	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		printf("GC2053_SENSOR_1080P_30FPS_LINEAR_MODE\n");
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR,
			  "cmos_set_wdr_mode_NOT support this mode!\n");
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
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunGc2053_forcar_BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 3;
		pstSnsState->astRegsInfo[0].u32RegNum = 11;
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				gc2053_forcar_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = gc2053_forcar_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = gc2053_forcar_data_byte;
		}
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			GC2053_EXPTIME_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			GC2053_EXPTIME_ADDR_H;

		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			GC2053_AGAIN_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			GC2053_AGAIN_ADDR_L;

		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			GC2053_DGAIN_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			GC2053_DGAIN_ADDR_H;

		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			GC2053_AUTO_PREGAIN_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			GC2053_AUTO_PREGAIN_ADDR_H;

		pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
			GC2053_STATUS_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
			GC2053_VMAX_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr =
			GC2053_VMAX_ADDR_H;

		pstSnsState->bSyncInit = GK_TRUE;
	} else {
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum - 2;
		     i++) { // AEC/AGC_Update
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

		if ((pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate ==
		     GK_TRUE) ||
		    (pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate ==
		     GK_TRUE)) {
			pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate =
				GK_TRUE;
		}

		pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate = GK_TRUE;
		pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate = GK_TRUE;
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
	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if ((pstSensorImageMode->u16Width <= 1920) &&
	    (pstSensorImageMode->u16Height <= 1080)) {
		if (pstSensorImageMode->f32Fps <= 30) {
			u8SensorImageMode =
				GC2053_SENSOR_1080P_30FPS_LINEAR_MODE;
			pstSnsState->u32FLStd = GC2053_VMAX_1080P30_LINEAR;
		} else {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Not support! Width:%d, Height:%d, Fps:%f\n",
				  pstSensorImageMode->u16Width,
				  pstSensorImageMode->u16Height,
				  pstSensorImageMode->f32Fps);
			return GK_FAILURE;
		}
	} else {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Not support! Width:%d, Height:%d, Fps:%f\n",
			  pstSensorImageMode->u16Width,
			  pstSensorImageMode->u16Height,
			  pstSensorImageMode->f32Fps);
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

	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = GC2053_SENSOR_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = GC2053_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[0] = GC2053_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[1] = GC2053_VMAX_1080P30_LINEAR;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);
	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));
	pstSensorExpFunc->pfn_cmos_sensor_init = gc2053_forcar_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = gc2053_forcar_exit;
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

static GK_S32 gc2053_forcar_set_bus_info(VI_PIPE ViPipe,
					 ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunGc2053_forcar_BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	GC2053_FORCAR_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	GC2053_FORCAR_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	GC2053_FORCAR_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = GC2053_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, GC2053_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, GC2053_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, GC2053_ID);
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

ISP_SNS_OBJ_S stSnsGc2053ForCarObj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = gc2053_forcar_standby,
	.pfnRestart = gc2053_forcar_restart,
	.pfnMirrorFlip = gc2053_forcar_mirror_flip,
	.pfnWriteReg = gc2053_forcar_write_register,
	.pfnReadReg = gc2053_forcar_read_register,
	.pfnSetBusInfo = gc2053_forcar_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
#endif /* __GC2053_FORCAR_CMOS_H_ */
