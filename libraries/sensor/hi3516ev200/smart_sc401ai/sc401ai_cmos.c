/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#if !defined(__SC401AI_CMOS_H_)
#define __SC401AI_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "hicompat.h"
#include "sc401ai_cmos_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SC401AI_ID 401
#define HIGH_8BITS(x) (((x) & 0xFF00) >> 8)
#define LOW_8BITS(x) ((x) & 0x00FF)
#define LOWER_4BITS(x) (((x) & 0x000F) << 4)
#define HIGHER_4BITS(x) (((x) & 0xF000) >> 12)
#define HIGHER_8BITS(x) (((x) & 0x0FF0) >> 4)
#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSc401ai[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define SC401AI_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSc401ai[dev])
#define SC401AI_SENSOR_SET_CTX(dev, pstCtx) (g_pastSc401ai[dev] = pstCtx)
#define SC401AI_SENSOR_RESET_CTX(dev) (g_pastSc401ai[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc401aiBusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
	[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };

static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };

extern const unsigned int SC401AI_I2C_ADDR;
extern unsigned int SC401AI_ADDR_BYTE;
extern unsigned int SC401AI_DATA_BYTE;

extern void sc401ai_init(VI_PIPE ViPipe);
extern void sc401ai_exit(VI_PIPE ViPipe);
extern void sc401ai_standby(VI_PIPE ViPipe);
extern void sc401ai_mirror_flip(VI_PIPE ViPipe,
				ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
extern void sc401ai_restart(VI_PIPE ViPipe);
extern int sc401ai_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sc401ai_read_register(VI_PIPE ViPipe, int addr);

/******** SC401AI Register Address ********/
#define SC401AI_EXP_H_ADDR 0x3e00
#define SC401AI_EXP_M_ADDR 0x3e01
#define SC401AI_EXP_L_ADDR 0x3e02
#define SC401AI_AGAIN_H_ADDR 0x3e08
#define SC401AI_AGAIN_L_ADDR 0x3e09
#define SC401AI_DGAIN_H_ADDR 0x3e06
#define SC401AI_DGAIN_L_ADDR 0x3e07
#define SC401AI_VMAX_H_ADDR 0x320e
#define SC401AI_VMAX_L_ADDR 0x320f

/* frame length in half-lines; VMAX (0x320e/0x320f, in lines) is half of it */
#define SC401AI_FULL_LINES_MAX 0xFFFF
#define SC401AI_INCREASE_LINES \
	0 /* make real fps less than stand fps because NVR require */
#define SC401AI_VMAX_2560x1440_LINEAR (1500 + SC401AI_INCREASE_LINES)

// sensor fps mode
#define SC401AI_SENSOR_2560x1440_30FPS_LINEAR_MODE (1)
#define SC401AI_RES_IS_2560x1440(w, h) ((w) <= 2560 && (h) <= 1440)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0,
		 sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = SC401AI_FULL_LINES_MAX;

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
		pstAeSnsDft->au8HistThresh[0] = 0xd;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 23880;
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 32384;
		pstAeSnsDft->u32MinDgain = 1024;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x38;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				76151;

		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
		pstAeSnsDft->u32MinIntTime = 1;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 1;
		break;
	}
	return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 u32FullLines;
	GK_U32 u32VMAX = SC401AI_VMAX_2560x1440_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case SC401AI_SENSOR_2560x1440_30FPS_LINEAR_MODE:
		if ((f32Fps <= 30) &&
		    (f32Fps >= SC401AI_VMAX_2560x1440_LINEAR * 30.0 /
				       (SC401AI_FULL_LINES_MAX >> 1))) {
			u32VMAX = SC401AI_VMAX_2560x1440_LINEAR * 30 /
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

	u32VMAX = (u32VMAX > (SC401AI_FULL_LINES_MAX >> 1)) ?
			  (SC401AI_FULL_LINES_MAX >> 1) :
			  u32VMAX;

	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32VMAX);
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS(u32VMAX);

	u32FullLines = u32VMAX << 1;
	pstSnsState->u32FLStd = (u32FullLines > SC401AI_FULL_LINES_MAX) ?
					SC401AI_FULL_LINES_MAX :
					u32FullLines;
	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
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
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines = (u32FullLines > SC401AI_FULL_LINES_MAX) ?
			       SC401AI_FULL_LINES_MAX :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
		LOW_8BITS(pstSnsState->au32FL[0] >> 1);
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
		HIGH_8BITS(pstSnsState->au32FL[0] >> 1);

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val = (u32IntTime > (SC401AI_FULL_LINES_MAX - 8)) ?
			 (SC401AI_FULL_LINES_MAX - 8) :
			 u32IntTime;

	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
		HIGHER_4BITS(u32Val); // 3e00
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
		HIGHER_8BITS(u32Val); // 3e01
	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
		LOWER_4BITS(u32Val); // 3e02
	return;
}

static GK_U32 again_index[286] = {
	// 1 2 3 6 12
	1024,  1040,  1056,  1072,  1088,  1104,  1120,	 1136,	1152,  1168,
	1184,  1200,  1216,  1232,  1248,  1264,  1280,	 1296,	1312,  1328,
	1344,  1360,  1376,  1392,  1408,  1424,  1440,	 1456,	1472,  1488,

	1504,  1528,  1551,  1575,  1598,  1622,  1646,	 1669,	1692,  1716,
	1740,  1762,  1786,  1809,  1833,  1856,  1880,	 1903,	1927,  1951,
	1974,  1998,  2021,  2044,  2068,  2092,  2115,	 2140,	2163,  2186,
	2210,  2233,  2256,  2279,  2303,  2327,  2350,	 2374,	2397,  2420,
	2444,  2468,  2491,  2515,  2538,  2562,  2586,	 2609,	2632,  2656,
	2680,  2703,  2726,  2750,  2774,  2798,  2823,	 2829,	2850,  2874,
	2898,  2926,  2952,  2985,

	3008,  3058,  3108,  3158,  3208,  3258,  3308,	 3358,	3408,  3458,
	3508,  3558,  3608,  3658,  3667,  3714,  3724,	 3777,	3832,  3878,
	3934,  3981,  4032,  4078,  4128,  4177,  4226,	 4279,	4328,  4375,
	4422,  4466,  4512,  4560,  4606,  4654,  4700,	 4748,	4794,  4840,
	4888,  4936,  4983,  5029,  5076,  5124,  5170,	 5218,	5265,  5311,
	5360,  5410,  5460,  5510,  5560,  5610,  5660,	 5710,	5755,  5798,
	5844,  5886,  5922,  5970,

	6017,  6111,  6205,  6299,  6393,  6487,  6581,	 6675,	6656,  6770,
	6862,  7051,  7145,  7239,  7332,  7427,  7521,	 7615,	7710,  7803,
	7897,  7791,  8086,  8180,  8274,  8367,  8461,	 8556,	8650,  8744,
	8837,  8931,  9025,  9120,  9214,  9307,  9401,	 9496,	9590,  9684,
	9778,  9871,  9966,  10059, 10153, 10248, 10341, 10436, 10529, 10624,
	10718, 10811, 10999, 11094, 11008, 11188, 11282, 11976, 11470, 11564,
	11658, 11752, 11846, 11940,

	12034, 12222, 12410, 12598, 12787, 12974, 13162, 13350, 13538, 13727,
	13914, 14103, 14291, 14478, 14667, 14854, 15042, 15231, 15418, 15606,
	15796, 15983, 16171, 16358, 16547, 16735, 16923, 17111, 17299, 17487,
	17675, 17862, 18239, 18239, 18427, 18615, 18804, 18991, 19180, 19367,
	19555, 19743, 19931, 20120, 20308, 20495, 20684, 20871, 21059, 21248,
	21435, 21623, 21812, 22000, 22188, 22375, 22563, 22752, 22940, 23128,
	23316, 23504, 23692, 23880,
};

static GK_U32 dgain_index[320] = {
	// 1 2 4 8 16
	1024,  1040,  1056,  1072,  1088,  1104,  1120,	 1136,	1152,  1168,
	1184,  1200,  1216,  1232,  1248,  1264,  1280,	 1296,	1312,  1328,
	1344,  1360,  1376,  1392,  1408,  1424,  1440,	 1456,	1472,  1488,
	1504,  1520,  1536,  1552,  1568,  1584,  1600,	 1616,	1632,  1648,
	1664,  1680,  1696,  1712,  1728,  1744,  1760,	 1776,	1792,  1808,
	1824,  1840,  1856,  1872,  1888,  1904,  1920,	 1936,	1952,  1968,
	1984,  2000,  2016,  2032,

	2048,  2080,  2112,  2144,  2176,  2208,  2240,	 2272,	2304,  2336,
	2368,  2400,  2432,  2464,  2496,  2528,  2560,	 2592,	2624,  2656,
	2688,  2720,  2752,  2784,  2816,  2848,  2880,	 2912,	2944,  2976,
	3008,  3040,  3072,  3104,  3136,  3168,  3200,	 3232,	3264,  3296,
	3328,  3360,  3392,  3424,  3456,  3488,  3520,	 3552,	3584,  3616,
	3648,  3680,  3712,  3744,  3776,  3808,  3840,	 3872,	3904,  3936,
	3968,  4000,  4032,  4064,

	4096,  4160,  4224,  4288,  4352,  4416,  4480,	 4544,	4608,  4672,
	4736,  4800,  4864,  4928,  4992,  5056,  5120,	 5184,	5248,  5312,
	5376,  5440,  5504,  5568,  5632,  5696,  5760,	 5824,	5888,  5952,
	6016,  6080,  6144,  6208,  6272,  6336,  6400,	 6464,	6528,  6592,
	6656,  6720,  6784,  6848,  6912,  6976,  7040,	 7104,	7168,  7232,
	7296,  7360,  7424,  7488,  7552,  7616,  7680,	 7744,	7808,  7872,
	7936,  8000,  8064,  8128,

	8192,  8320,  8448,  8576,  8704,  8832,  8960,	 9088,	9216,  9344,
	9472,  9600,  9728,  9856,  9984,  10112, 10240, 10368, 10496, 10624,
	10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904,
	12032, 12160, 12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184,
	13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208, 14336, 14464,
	14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744,
	15872, 16000, 16128, 16256,

	16512, 16768, 17024, 17280, 17536, 17024, 18048, 18304, 18560, 18816,
	19072, 19328, 19584, 19840, 20096, 20352, 20608, 20864, 18688, 21120,
	21376, 21632, 21888, 22144, 22400, 22656, 22919, 23168, 23424, 23680,
	23936, 24192, 24448, 24704, 24960, 25216, 25472, 25728, 25984, 26240,
	26496, 26752, 27008, 27264, 27250, 27776, 28032, 28288, 28544, 28800,
	29056, 29312, 29568, 29824, 30080, 30336, 30592, 30848, 31104, 31360,
	31616, 31872, 32128, 32384,

};

static GK_U32 again_table[286] = { // 1 2 3 6 12
	0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347,
	0x0348, 0x0349, 0x034A, 0x034B, 0x034C, 0x034D, 0x034E, 0x034F,
	0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357,
	0x0358, 0x0359, 0x035A, 0x035B, 0x035C, 0x035D,

	0x2340, 0x2341, 0x2342, 0x2343, 0x2344, 0x2345, 0x2346, 0x2347,
	0x2348, 0x2349, 0x234A, 0x234B, 0x234C, 0x234D, 0x234E, 0x234F,
	0x2350, 0x2351, 0x2352, 0x2353, 0x2354, 0x2355, 0x2356, 0x2357,
	0x2358, 0x2359, 0x235A, 0x235B, 0x235C, 0x235D, 0x235E, 0x235F,
	0x2360, 0x2361, 0x2362, 0x2363, 0x2364, 0x2365, 0x2366, 0x2367,
	0x2368, 0x2369, 0x236A, 0x236B, 0x236C, 0x236D, 0x236E, 0x236F,
	0x2370, 0x2371, 0x2372, 0x2373, 0x2374, 0x2375, 0x2376, 0x2377,
	0x2378, 0x2379, 0x237A, 0x237B, 0x237C, 0x237D, 0x237E, 0x237F,

	0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747,
	0x2748, 0x2749, 0x274A, 0x274B, 0x274C, 0x274D, 0x274E, 0x274F,
	0x2750, 0x2751, 0x2752, 0x2753, 0x2754, 0x2755, 0x2756, 0x2757,
	0x2758, 0x2759, 0x275A, 0x275B, 0x275C, 0x275D, 0x275E, 0x275F,
	0x2760, 0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767,
	0x2768, 0x2769, 0x276A, 0x276B, 0x276C, 0x276D, 0x276E, 0x276F,
	0x2770, 0x2771, 0x2772, 0x2773, 0x2774, 0x2775, 0x2776, 0x2777,
	0x2778, 0x2779, 0x277A, 0x277B, 0x277C, 0x277D, 0x277E, 0x277F,

	0x2F40, 0x2F41, 0x2F42, 0x2F43, 0x2F44, 0x2F45, 0x2F46, 0x2F47,
	0x2F48, 0x2F49, 0x2F4A, 0x2F4B, 0x2F4C, 0x2F4D, 0x2F4E, 0x2F4F,
	0x2F50, 0x2F51, 0x2F52, 0x2F53, 0x2F54, 0x2F55, 0x2F56, 0x2F57,
	0x2F58, 0x2F59, 0x2F5A, 0x2F5B, 0x2F5C, 0x2F5D, 0x2F5E, 0x2F5F,
	0x2F60, 0x2F61, 0x2F62, 0x2F63, 0x2F64, 0x2F65, 0x2F66, 0x2F67,
	0x2F68, 0x2F69, 0x2F6A, 0x2F6B, 0x2F6C, 0x2F6D, 0x2F6E, 0x2F6F,
	0x2F70, 0x2F71, 0x2F72, 0x2F73, 0x2F74, 0x2F75, 0x2F76, 0x2F77,
	0x2F78, 0x2F79, 0x2F7A, 0x2F7B, 0x2F7C, 0x2F7D, 0x2F7E, 0x2F7F,

	0x3F40, 0x3F41, 0x3F42, 0x3F43, 0x3F44, 0x3F45, 0x3F46, 0x3F47,
	0x3F48, 0x3F49, 0x3F4A, 0x3F4B, 0x3F4C, 0x3F4D, 0x3F4E, 0x3F4F,
	0x3F50, 0x3F51, 0x3F52, 0x3F53, 0x3F54, 0x3F55, 0x3F56, 0x3F57,
	0x3F58, 0x3F59, 0x3F5A, 0x3F5B, 0x3F5C, 0x3F5D, 0x3F5E, 0x3F5F,
	0x3F60, 0x3F61, 0x3F62, 0x3F63, 0x3F64, 0x3F65, 0x3F66, 0x3F67,
	0x3F68, 0x3F69, 0x3F6A, 0x3F6B, 0x3F6C, 0x3F6D, 0x3F6E, 0x3F6F,
	0x3F70, 0x3F71, 0x3F72, 0x3F73, 0x3F74, 0x3F75, 0x3F76, 0x3F77,
	0x3F78, 0x3F79, 0x3F7A, 0x3F7B, 0x3F7C, 0x3F7D, 0x3F7E, 0x3F7F

};

static GK_U32 dgain_table[320] = { // 1 2 4 8 16
	0x0080, 0x0082, 0x0084, 0x0086, 0x0088, 0x008A, 0x008C, 0x008E,
	0x0090, 0x0092, 0x0094, 0x0096, 0x0098, 0x009A, 0x009C, 0x009E,
	0x00A0, 0x00A2, 0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AC, 0x00AE,
	0x00B0, 0x00B2, 0x00B4, 0x00B6, 0x00B8, 0x00BA, 0x00BC, 0x00BE,
	0x00C0, 0x00C2, 0x00C4, 0x00C6, 0x00C8, 0x00CA, 0x00CC, 0x00CE,
	0x00D0, 0x00D2, 0x00D4, 0x00D6, 0x00D8, 0x00DA, 0x00DC, 0x00DE,
	0x00E0, 0x00E2, 0x00E4, 0x00E6, 0x00E8, 0x00EA, 0x00EC, 0x00EE,
	0x00F0, 0x00F2, 0x00F4, 0x00F6, 0x00F8, 0x00FA, 0x00FC, 0x00FE,

	0x0180, 0x0182, 0x0184, 0x0186, 0x0188, 0x018A, 0x018C, 0x018E,
	0x0190, 0x0192, 0x0194, 0x0196, 0x0198, 0x019A, 0x019C, 0x019E,
	0x01A0, 0x01A2, 0x01A4, 0x01A6, 0x01A8, 0x01AA, 0x01AC, 0x01AE,
	0x01B0, 0x01B2, 0x01B4, 0x01B6, 0x01B8, 0x01BA, 0x01BC, 0x01BE,
	0x01C0, 0x01C2, 0x01C4, 0x01C6, 0x01C8, 0x01CA, 0x01CC, 0x01CE,
	0x01D0, 0x01D2, 0x01D4, 0x01D6, 0x01D8, 0x01DA, 0x01DC, 0x01DE,
	0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8, 0x01EA, 0x01EC, 0x01EE,
	0x01F0, 0x01F2, 0x01F4, 0x01F6, 0x01F8, 0x01FA, 0x01FC, 0x01FE,

	0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038A, 0x038C, 0x038E,
	0x0390, 0x0392, 0x0394, 0x0396, 0x0398, 0x039A, 0x039C, 0x039E,
	0x03A0, 0x03A2, 0x03A4, 0x03A6, 0x03A8, 0x03AA, 0x03AC, 0x03AE,
	0x03B0, 0x03B2, 0x03B4, 0x03B6, 0x03B8, 0x03BA, 0x03BC, 0x03BE,
	0x03C0, 0x03C2, 0x03C4, 0x03C6, 0x03C8, 0x03CA, 0x03CC, 0x03CE,
	0x03D0, 0x03D2, 0x03D4, 0x03D6, 0x03D8, 0x03DA, 0x03DC, 0x03DE,
	0x03E0, 0x03E2, 0x03E4, 0x03E6, 0x03E8, 0x03EA, 0x03EC, 0x03EE,
	0x03F0, 0x03F2, 0x03F4, 0x03F6, 0x03F8, 0x03FA, 0x03FC, 0x03FE,

	0x0780, 0x0782, 0x0784, 0x0786, 0x0788, 0x078A, 0x078C, 0x078E,
	0x0790, 0x0792, 0x0794, 0x0796, 0x0798, 0x079A, 0x079C, 0x079E,
	0x07A0, 0x07A2, 0x07A4, 0x07A6, 0x07A8, 0x07AA, 0x07AC, 0x07AE,
	0x07B0, 0x07B2, 0x07B4, 0x07B6, 0x07B8, 0x07BA, 0x07BC, 0x07BE,
	0x07C0, 0x07C2, 0x07C4, 0x07C6, 0x07C8, 0x07CA, 0x07CC, 0x07CE,
	0x07D0, 0x07D2, 0x07D4, 0x07D6, 0x07D8, 0x07DA, 0x07DC, 0x07DE,
	0x07E0, 0x07E2, 0x07E4, 0x07E6, 0x07E8, 0x07EA, 0x07EC, 0x07EE,
	0x07F0, 0x07F2, 0x07F4, 0x07F6, 0x07F8, 0x07FA, 0x07FC, 0x07FE,

	0x0F80, 0x0F82, 0x0F84, 0x0F86, 0x0F88, 0x0F8A, 0x0F8C, 0x0F8E,
	0x0F90, 0x0F92, 0x0F94, 0x0F96, 0x0F98, 0x0F9A, 0x0F9C, 0x0F9E,
	0x0FA0, 0x0FA2, 0x0FA4, 0x0FA6, 0x0FA8, 0x0FAA, 0x0FAC, 0x0FAE,
	0x0FB0, 0x0FB2, 0x0FB4, 0x0FB6, 0x0FB8, 0x0FBA, 0x0FBC, 0x0FBE,
	0x0FC0, 0x0FC2, 0x0FC4, 0x0FC6, 0x0FC8, 0x0FCA, 0x0FCC, 0x0FCE,
	0x0FD0, 0x0FD2, 0x0FD4, 0x0FD6, 0x0FD8, 0x0FDA, 0x0FDC, 0x0FDE,
	0x0FE0, 0x0FE2, 0x0FE4, 0x0FE6, 0x0FE8, 0x0FEA, 0x0FEC, 0x0FEE,
	0x0FF0, 0x0FF2, 0x0FF4, 0x0FF6, 0x0FF8, 0x0FFA, 0x0FFC, 0x0FFE
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= again_index[285]) {
		*pu32AgainLin = again_index[285];
		*pu32AgainDb = again_table[285];
		return;
	}

	for (i = 1; i < 285; i++) {
		if (*pu32AgainLin < again_index[i]) {
			*pu32AgainLin = again_index[i - 1];
			*pu32AgainDb = again_table[i - 1];
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

	if (*pu32DgainLin >= dgain_index[319]) {
		*pu32DgainLin = dgain_index[319];
		*pu32DgainDb = dgain_table[319];
		return;
	}

	for (i = 1; i < 320; i++) {
		if (*pu32DgainLin < dgain_index[i]) {
			*pu32DgainLin = dgain_index[i - 1];
			*pu32DgainDb = dgain_table[i - 1];
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

	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u8Reg0x3e06 = HIGH_8BITS(u32Dgain);
	u8Reg0x3e07 = LOW_8BITS(u32Dgain);
	u8Reg0x3e08 = HIGH_8BITS(u32Again);
	u8Reg0x3e09 = LOW_8BITS(u32Again);

	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = u8Reg0x3e09;
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u8Reg0x3e08;
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = u8Reg0x3e07;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8Reg0x3e06;

	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00; // R0x3812
	if (u8Reg0x3e08 < 0x27) {
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			0x0b; // R0x363a
	} else if (u8Reg0x3e08 >= 0x27) {
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			0x17; // R0x3812
	}
	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x30; // R0x3812
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

	g_genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
	g_au32MaxTimeGetCnt[ViPipe] = 0;

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
	pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
	pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

	return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_STATIC_TEMP 5000
#define CALIBRATE_AWB_STATIC_RGAIN 374
#define CALIBRATE_AWB_STATIC_GGAIN 256
#define CALIBRATE_AWB_STATIC_BGAIN 660
#define CALIBRATE_AWB_P1 137
#define CALIBRATE_AWB_P2 (-5)
#define CALIBRATE_AWB_Q1 (-124)
#define CALIBRATE_AWB_A1 169437
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-103151)

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
		memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S),
			 &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
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

	memset_s(pstExpFuncs, sizeof(AWB_SENSOR_EXP_FUNC_S), 0,
		 sizeof(AWB_SENSOR_EXP_FUNC_S));
	pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

	return GK_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = { { 378, 256, 430 },
						       { 439, 256, 439 } };

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstDef);
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0,
		 sizeof(ISP_CMOS_DEFAULT_S));

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
		memcpy_s(&pstDef->stNoiseCalibration,
			 sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
			 &g_stIspNoiseCalibratio,
			 sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = SC401AI_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S),
		 &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case SC401AI_SENSOR_2560x1440_30FPS_LINEAR_MODE:
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
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
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

	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines_5Fps = (SC401AI_VMAX_2560x1440_LINEAR * 30) / 5;
	/* VMAX counts lines, the shutter half-lines */
	u32MaxIntTime_5Fps = u32FullLines_5Fps * 2 - 8; /* sub 8 */

	if (bEnable) { /* setup for ISP pixel calibration mode */
		sc401ai_write_register(ViPipe, SC401AI_AGAIN_L_ADDR, 0x10);
		sc401ai_write_register(ViPipe, SC401AI_AGAIN_H_ADDR, 0x00);
		sc401ai_write_register(ViPipe, SC401AI_EXP_L_ADDR,
				       LOWER_4BITS(u32MaxIntTime_5Fps));
		sc401ai_write_register(ViPipe, SC401AI_EXP_M_ADDR,
				       HIGHER_8BITS(u32MaxIntTime_5Fps));
		sc401ai_write_register(ViPipe, SC401AI_EXP_H_ADDR,
				       HIGHER_4BITS(u32MaxIntTime_5Fps));
		sc401ai_write_register(ViPipe, SC401AI_VMAX_L_ADDR,
				       LOW_8BITS(u32FullLines_5Fps));
		sc401ai_write_register(ViPipe, SC401AI_VMAX_H_ADDR,
				       HIGH_8BITS(u32FullLines_5Fps));

	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd > SC401AI_FULL_LINES_MAX) ?
				(SC401AI_FULL_LINES_MAX) :
				pstSnsState->u32FLStd;

		sc401ai_write_register(ViPipe, SC401AI_VMAX_L_ADDR,
				       LOW_8BITS(pstSnsState->u32FLStd >> 1));
		sc401ai_write_register(ViPipe, SC401AI_VMAX_H_ADDR,
				       HIGH_8BITS(pstSnsState->u32FLStd >> 1));
		pstSnsState->bSyncInit = GK_FALSE;
	}

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;

		pstSnsState->u8ImgMode =
			SC401AI_SENSOR_2560x1440_30FPS_LINEAR_MODE;
		pstSnsState->u32FLStd = SC401AI_VMAX_2560x1440_LINEAR;
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

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe,
				     ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSnsRegsInfo);
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunSc401aiBusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 12;

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				SC401AI_I2C_ADDR;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = SC401AI_ADDR_BYTE;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = SC401AI_DATA_BYTE;
		}

		// Linear Mode Regs
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			SC401AI_EXP_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			SC401AI_EXP_M_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			SC401AI_EXP_H_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			SC401AI_AGAIN_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			SC401AI_AGAIN_H_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			SC401AI_DGAIN_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			SC401AI_DGAIN_H_ADDR;

		/* a frame later than the shutter, so a longer frame and the
		 * longer exposure that needs it reach the sensor together */
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			SC401AI_VMAX_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
			SC401AI_VMAX_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
			0x3812; /* Grouphold Logic */
		pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = 0x363a;
		pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = 0x3812;
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
		/* every gain change goes inside group hold with its 0x363a,
		 * not only the ones that move 0x363a */
		if (pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate) {
			pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate =
				GK_TRUE;
		}
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
	GK_U8 u8SensorImageMode = 0;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSensorImageMode);
	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= 30) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (SC401AI_RES_IS_2560x1440(
				    pstSensorImageMode->u16Width,
				    pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					SC401AI_SENSOR_2560x1440_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd =
					SC401AI_VMAX_2560x1440_LINEAR * 2;
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

	SC401AI_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = SC401AI_SENSOR_2560x1440_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = SC401AI_VMAX_2560x1440_LINEAR * 2;
	pstSnsState->au32FL[0] = SC401AI_VMAX_2560x1440_LINEAR * 2;
	pstSnsState->au32FL[1] = SC401AI_VMAX_2560x1440_LINEAR * 2;

	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x12;
	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x30;

	memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0,
		 sizeof(ISP_SNS_REGS_INFO_S));
	memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0,
		 sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0,
		 sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = sc401ai_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = sc401ai_exit;
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
static GK_S32 sc401ai_set_bus_info(VI_PIPE ViPipe,
				   ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunSc401aiBusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC401AI_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	SC401AI_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC401AI_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	SC401AI_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = SC401AI_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC401AI_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC401AI_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC401AI_ID);
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

ISP_SNS_OBJ_S stSnsSc401aiObj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = sc401ai_standby,
	.pfnRestart = sc401ai_restart,
	.pfnMirrorFlip = sc401ai_mirror_flip,
	.pfnWriteReg = sc401ai_write_register,
	.pfnReadReg = sc401ai_read_register,
	.pfnSetBusInfo = sc401ai_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SC401AI_CMOS_H_ */
