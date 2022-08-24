/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__SC2231_CMOS_H_)
#define __SC2231_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#include "sc2231_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SC2231_ID 2231

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

#define HIGH_8BITS(x) ((x & 0xFF00) >> 8)
#define LOW_8BITS(x) (x & 0x00FF)
#define LOWER_4BITS(x) (x & 0x000F)
#define HIGHER_4BITS(x) ((x & 0xF000) >> 12)
#define HIGHER_8BITS(x) ((x & 0x0FF0) >> 4)
#define ISO_LUT_NUM 8

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSc2231[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define SC2231_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSc2231[dev])
#define SC2231_SENSOR_SET_CTX(dev, pstCtx) (g_pastSc2231[dev] = pstCtx)
#define SC2231_SENSOR_RESET_CTX(dev) (g_pastSc2231[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc2231BusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };
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
extern const unsigned int sc2231_i2c_addr;
extern unsigned int sc2231_addr_byte;
extern unsigned int sc2231_data_byte;

extern void sc2231_init(VI_PIPE ViPipe);
extern void sc2231_exit(VI_PIPE ViPipe);
extern void sc2231_standby(VI_PIPE ViPipe);
extern void sc2231_restart(VI_PIPE ViPipe);
extern int sc2231_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sc2231_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

/******** Sc2231 Register Address ********/
#define SC2231_EXP_H_ADDR (0x3e00)
#define SC2231_EXP_M_ADDR (0x3e01)
#define SC2231_EXP_L_ADDR (0x3e02)
#define SC2231_AGAIN_H_ADDR (0x3e08)
#define SC2231_AGAIN_L_ADDR (0x3e09)
#define SC2231_DGAIN_H_ADDR (0x3e06)
#define SC2231_DGAIN_L_ADDR (0x3e07)
#define SC2231_VMAX_H_ADDR (0x320e)
#define SC2231_VMAX_L_ADDR (0x320f)
#define SC2231_HMAX_H_ADDR (0x320c)
#define SC2231_HMAX_L_ADDR (0x320d)

#define SC2231_FULL_LINES_MAX (0x7FFF)
#define SC2231_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require */
#define SC2231_VMAX_1080P30_LINEAR (1125 + SC2231_INCREASE_LINES)
#define SC4236_FRAME_RATE_MIN (0x34BC)

// sensor fps mode
#define SC2231_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define SC2231_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = SC2231_FULL_LINES_MAX;

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
	default:
	case WDR_MODE_NONE: /* linear mode */
		pstAeSnsDft->au8HistThresh[0] = 0x0D;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 16128; // 15.75x32
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 16128;
		pstAeSnsDft->u32MinDgain = 1024;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x40;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;

		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
		pstAeSnsDft->u32MinIntTime = 3;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 3;
		break;
	}

	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32FullLines;
	GK_U32 u32VMAX = SC2231_VMAX_1080P30_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case SC2231_SENSOR_1080P_30FPS_LINEAR_MODE:
		if ((f32Fps <= 30) && (f32Fps >= 5)) { /* Minimum 5fps */
			u32VMAX = SC2231_VMAX_1080P30_LINEAR * 30 /
				  DIV_0_TO_1_FLOAT(f32Fps);
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
		pstAeSnsDft->u32LinesPer500ms =
			SC2231_VMAX_1080P30_LINEAR * 2 * 15;
		break;
	default:
		break;
	}

	u32VMAX = (u32VMAX > SC2231_FULL_LINES_MAX) ? SC2231_FULL_LINES_MAX :
						      u32VMAX;

	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGH_8BITS(u32VMAX);
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(u32VMAX);

	u32FullLines = u32VMAX << 1;
	pstSnsState->u32FLStd = (u32FullLines > SC2231_FULL_LINES_MAX) ?
					SC2231_FULL_LINES_MAX :
					u32FullLines;
	pstAeSnsDft->f32Fps = f32Fps;

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
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines = (u32FullLines > SC4236_FRAME_RATE_MIN) ?
			       SC4236_FRAME_RATE_MIN :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
		HIGH_8BITS((pstSnsState->au32FL[0] + 8) >> 1);
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
		LOW_8BITS((pstSnsState->au32FL[0] + 8) >> 1);

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val = (u32IntTime > SC2231_FULL_LINES_MAX) ? SC2231_FULL_LINES_MAX :
							u32IntTime;

	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (u32Val >> 12) &
							    0x0F;
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32Val >> 4) &
							    0xFF;
	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = (u32Val << 4) &
							    0xF0;

	return;
}

static const GK_U16 gau16GainIndex[128] = {
	1024,  1056,  1089,  1120,  1152,  1184,  1217,	 1248,	1280,  1312,
	1345,  1376,  1408,  1440,  1473,  1504,  1536,	 1568,	1601,  1632,
	1664,  1696,  1729,  1760,  1792,  1824,  1857,	 1888,	1920,  1952,
	1985,  2016,  2048,  2113,  2176,  2241,  2304,	 2369,	2432,  2497,
	2560,  2640,  2688,  2753,  2816,  2877,  2944,	 3011,	3072,  3133,
	3200,  3267,  3328,  3389,  3456,  3523,  3584,	 3645,	3712,  3779,
	3840,  3901,  3968,  4035,  4096,  4229,  4352,	 4485,	4608,  4741,
	4864,  4997,  5120,  5253,  5376,  5509,  5632,	 5765,	5888,  6021,
	6144,  6277,  6400,  6533,  6656,  6789,  6912,	 7045,	7168,  7301,
	7424,  7557,  7680,  7813,  7936,  8069,  8192,	 8448,	8704,  8960,
	9216,  9472,  9728,  9984,  10240, 10496, 10752, 11008, 11264, 11520,
	11776, 12032, 12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080,
	14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128
};

static const GK_U16 gau16AGainTable[128] = {
	0x0320, 0x0321, 0x0322, 0x0323, 0x0324, 0x0325, 0x0326, 0x0327, 0x0328,
	0x0329, 0x032a, 0x032b, 0x032c, 0x032d, 0x032e, 0x032f, 0x0330, 0x0331,
	0x0332, 0x0333, 0x0334, 0x0335, 0x0336, 0x0337, 0x0338, 0x0339, 0x033a,
	0x033b, 0x033c, 0x033d, 0x033e, 0x033f, 0x0720, 0x0721, 0x0722, 0x0723,
	0x0724, 0x0725, 0x0726, 0x0727, 0x0728, 0x0729, 0x072a, 0x072b, 0x072c,
	0x072d, 0x072e, 0x072f, 0x0730, 0x0731, 0x0732, 0x0733, 0x0734, 0x0735,
	0x0736, 0x0737, 0x0738, 0x0739, 0x073a, 0x073b, 0x073c, 0x073d, 0x073e,
	0x073f, 0x0f20, 0x0f21, 0x0f22, 0x0f23, 0x0f24, 0x0f25, 0x0f26, 0x0f27,
	0x0f28, 0x0f29, 0x0f2a, 0x0f2b, 0x0f2c, 0x0f2d, 0x0f2e, 0x0f2f, 0x0f30,
	0x0f31, 0x0f32, 0x0f33, 0x0f34, 0x0f35, 0x0f36, 0x0f37, 0x0f38, 0x0f39,
	0x0f3a, 0x0f3b, 0x0f3c, 0x0f3d, 0x0f3e, 0x0f3f, 0x1f20, 0x1f21, 0x1f22,
	0x1f23, 0x1f24, 0x1f25, 0x1f26, 0x1f27, 0x1f28, 0x1f29, 0x1f2a, 0x1f2b,
	0x1f2c, 0x1f2d, 0x1f2e, 0x1f2f, 0x1f30, 0x1f31, 0x1f32, 0x1f33, 0x1f34,
	0x1f35, 0x1f36, 0x1f37, 0x1f38, 0x1f39, 0x1f3a, 0x1f3b, 0x1f3c, 0x1f3d,
	0x1f3e, 0x1f3f
};

static const GK_U16 gau16DGainTable[160] = {
	0x0080, 0x0084, 0x0088, 0x008C, 0x0090, 0x0094, 0x0098, 0x009C, 0x00A0,
	0x00A4, 0x00A8, 0x00AC, 0x00B0, 0x00B4, 0x00B8, 0x00BC, 0x00C0, 0x00C4,
	0x00C8, 0x00CC, 0x00D0, 0x00D4, 0x00D8, 0x00DC, 0x00E0, 0x00E4, 0x00E8,
	0x00EC, 0x00F0, 0x00F4, 0x00F8, 0x00FC, 0x0180, 0x0184, 0x0188, 0x018C,
	0x0190, 0x0194, 0x0198, 0x019C, 0x01A0, 0x01A4, 0x01A8, 0x01AC, 0x01B0,
	0x01B4, 0x01B8, 0x01BC, 0x01C0, 0x01C4, 0x01C8, 0x01CC, 0x01D0, 0x01D4,
	0x01D8, 0x01DC, 0x01E0, 0x01E4, 0x01E8, 0x01EC, 0x01F0, 0x01F4, 0x01F8,
	0x01FC, 0x0380, 0x0384, 0x0388, 0x038C, 0x0390, 0x0394, 0x0398, 0x039C,
	0x03A0, 0x03A4, 0x03A8, 0x03AC, 0x03B0, 0x03B4, 0x03B8, 0x03BC, 0x03C0,
	0x03C4, 0x03C8, 0x03CC, 0x03D0, 0x03D4, 0x03D8, 0x03DC, 0x03E0, 0x03E4,
	0x03E8, 0x03EC, 0x03F0, 0x03F4, 0x03F8, 0x03FC, 0x0780, 0x0784, 0x0788,
	0x078C, 0x0790, 0x0794, 0x0798, 0x079C, 0x07A0, 0x07A4, 0x07A8, 0x07AC,
	0x07B0, 0x07B4, 0x07B8, 0x07BC, 0x07C0, 0x07C4, 0x07C8, 0x07CC, 0x07D0,
	0x07D4, 0x07D8, 0x07DC, 0x07E0, 0x07E4, 0x07E8, 0x07EC, 0x07F0, 0x07F4,
	0x07F8, 0x07FC, 0x0F80, 0x0F84, 0x0F88, 0x0F8C, 0x0F90, 0x0F94, 0x0F98,
	0x0F9C, 0x0FA0, 0x0FA4, 0x0FA8, 0x0FAC, 0x0FB0, 0x0FB4, 0x0FB8, 0x0FBC,
	0x0FC0, 0x0FC4, 0x0FC8, 0x0FCC, 0x0FD0, 0x0FD4, 0x0FD8, 0x0FDC, 0x0FE0,
	0x0FE4, 0x0FE8, 0x0FEC, 0x0FF0, 0x0FF4, 0x0FF8, 0x0FFC
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= gau16GainIndex[127]) {
		*pu32AgainLin = gau16GainIndex[127];
		*pu32AgainDb = gau16AGainTable[127];
		return;
	}

	for (i = 1; i < 128; i++) {
		if (*pu32AgainLin < gau16GainIndex[i]) {
			*pu32AgainLin = gau16GainIndex[i - 1];
			*pu32AgainDb = gau16AGainTable[i - 1];
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

	if (*pu32DgainLin >= gau16GainIndex[127]) {
		*pu32DgainLin = gau16GainIndex[127];
		*pu32DgainDb = gau16DGainTable[127];
		return;
	}

	for (i = 1; i < 128; i++) {
		if (*pu32DgainLin < gau16GainIndex[i]) {
			*pu32DgainLin = gau16GainIndex[i - 1];
			*pu32DgainDb = gau16DGainTable[i - 1];
			break;
		}
	}

	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	GK_U8 u8Reg0x3e09, u8Reg0x3e08, u8Reg0x3e07, u8Reg0x3e06;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u8Reg0x3e06 = HIGH_8BITS(u32Dgain);
	u8Reg0x3e07 = LOW_8BITS(u32Dgain);
	u8Reg0x3e08 = HIGH_8BITS(u32Again);
	u8Reg0x3e09 = LOW_8BITS(u32Again);

	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = u8Reg0x3e06;
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u8Reg0x3e07;
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = u8Reg0x3e08;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8Reg0x3e09;

	// logic
	if (u8Reg0x3e08 < 0x07) { /* 0x0310~0x031f gain<2x */
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x40;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
	} else if (u8Reg0x3e08 >= 0x07 &&
		   u8Reg0x3e08 < 0x0f) { /* 0x0710~0x071f 4x>gain >= 2x */
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x40;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
	} else if (u8Reg0x3e08 >= 0x0f &&
		   u8Reg0x3e08 < 0x1f) { /* 0x0f10~0x0f1f 8x>gain >= 4x */
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x40;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
	} else if ((0x1f == u8Reg0x3e08) && (0x00 == u8Reg0x3e06) &&
		   (0x80 == u8Reg0x3e07)) { /* 0x1f10~0x1f1f 8x<=gain<=15.5x */
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x40;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
	} else { /* gain > 15.5x */
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x40;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
	}

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
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 5000;

	pstAwbSnsDft->au16GainOffset[0] = 0x19F;
	pstAwbSnsDft->au16GainOffset[1] = 0x100;
	pstAwbSnsDft->au16GainOffset[2] = 0x100;
	pstAwbSnsDft->au16GainOffset[3] = 0x1AE;

	pstAwbSnsDft->as32WbPara[0] = 18;
	pstAwbSnsDft->as32WbPara[1] = 238;
	pstAwbSnsDft->as32WbPara[2] = 0;
	pstAwbSnsDft->as32WbPara[3] = 257539;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -206277;
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

	pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
	pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
	pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
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
	GK_U32 k = 0;

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
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCac;
		pstDef->unKey.bit1Rgbir = 0;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = SC2231_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case SC2231_SENSOR_1080P_30FPS_LINEAR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
		pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
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
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;

	for (i = 0; i < 4; i++) {
		pstBlackLevel->au16BlackLevel[i] = 0x100;
	}

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines_5Fps = (SC2231_VMAX_1080P30_LINEAR * 30) / 5;
	u32MaxIntTime_5Fps = pstSnsState->u32FLStd - 8;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		sc2231_write_register(ViPipe, SC2231_VMAX_H_ADDR,
				      (u32FullLines_5Fps & 0xFF00) >>
					      8); /* 5fps */
		sc2231_write_register(ViPipe, SC2231_VMAX_L_ADDR,
				      (u32FullLines_5Fps & 0xFF)); /* 5fps */
		sc2231_write_register(ViPipe, SC2231_EXP_M_ADDR,
				      (u32MaxIntTime_5Fps & 0x0FF0) >>
					      4); /* max exposure lines */
		sc2231_write_register(ViPipe, SC2231_EXP_L_ADDR,
				      (u32MaxIntTime_5Fps & 0xF)
					      << 4); /* max exposure lines */

		sc2231_write_register(ViPipe, SC2231_AGAIN_H_ADDR,
				      0x00); /* AG, Context A */
		sc2231_write_register(ViPipe, SC2231_AGAIN_L_ADDR,
				      0x10); /* DG, Context A */
	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd > SC2231_FULL_LINES_MAX * 2) ?
				(SC2231_FULL_LINES_MAX * 2) :
				pstSnsState->u32FLStd;
		sc2231_write_register(ViPipe, SC2231_VMAX_H_ADDR,
				      (pstSnsState->u32FLStd & 0xFF00) >>
					      8); /* 30fps */
		sc2231_write_register(ViPipe, SC2231_VMAX_L_ADDR,
				      (pstSnsState->u32FLStd & 0xFF));

		pstSnsState->bSyncInit = GK_FALSE;
	}
	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunSc2231BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 13;

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				sc2231_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = sc2231_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = sc2231_data_byte;
		}

		// Linear Mode Regs
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			SC2231_EXP_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			SC2231_EXP_M_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			SC2231_EXP_L_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			SC2231_DGAIN_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			SC2231_DGAIN_L_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			SC2231_AGAIN_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			SC2231_AGAIN_L_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			SC2231_VMAX_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
			SC2231_VMAX_L_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 2;
		pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
			0x3812; // Grouphold Logic
		pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 2;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = 0x3630;
		pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 2;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = 0x3632;
		pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 2;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = 0x3812;

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

		if (pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate) {
			pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate =
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
	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= 30) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (SC2231_RES_IS_1080P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					SC2231_SENSOR_1080P_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd =
					SC2231_VMAX_1080P30_LINEAR * 2;
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

	SC2231_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = SC2231_SENSOR_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = SC2231_VMAX_1080P30_LINEAR * 2;
	pstSnsState->au32FL[0] = SC2231_VMAX_1080P30_LINEAR * 2;
	pstSnsState->au32FL[1] = SC2231_VMAX_1080P30_LINEAR * 2;

	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x04;
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x65;
	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x40;
	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x88;
	pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = sc2231_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = sc2231_exit;
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

static GK_S32 sc2231_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunSc2231BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC2231_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	SC2231_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC2231_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	SC2231_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = SC2231_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC2231_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC2231_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC2231_ID);
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
	g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
	g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
	g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
	g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
	g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;
	g_abIsIrMode[ViPipe] = pstInitAttr->bIsIrMode;

	for (i = 0; i < CCM_MATRIX_SIZE; i++) {
		g_au16InitCCM[ViPipe][i] = pstInitAttr->au16CCM[i];
	}

	return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSc2231Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = sc2231_standby,
	.pfnRestart = sc2231_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = sc2231_write_register,
	.pfnReadReg = sc2231_read_register,
	.pfnSetBusInfo = sc2231_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SC2231_CMOS_H_ */
