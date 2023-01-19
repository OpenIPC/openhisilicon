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
#include "comm_isp.h"
#include "sc3335_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SC3335_ID 3335

/****************************************************************************
 * global variables                                                         *
 ****************************************************************************/
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

ISP_SNS_STATE_S *g_pastSc3335[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define SC3335_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSc3335[dev])
#define SC3335_SENSOR_SET_CTX(dev, pstCtx) (g_pastSc3335[dev] = pstCtx)
#define SC3335_SENSOR_RESET_CTX(dev) (g_pastSc3335[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc3335BusInfo[ISP_MAX_PIPE_NUM] = {
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

extern const unsigned int sc3335_i2c_addr;
extern unsigned int sc3335_addr_byte;
extern unsigned int sc3335_data_byte;

extern void sc3335_init(VI_PIPE ViPipe);
extern void sc3335_exit(VI_PIPE ViPipe);
extern void sc3335_standby(VI_PIPE ViPipe);
extern void sc3335_restart(VI_PIPE ViPipe);
extern int sc3335_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sc3335_read_register(VI_PIPE ViPipe, int addr);
extern void sc3335_mirror_flip(VI_PIPE ViPipe,
			       ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);

/******** SC3335 Register Address ********/
#define SC3335_EXP_H_ADDR (0x3E00)
#define SC3335_EXP_M_ADDR (0x3E01)
#define SC3335_EXP_L_ADDR (0x3E02)
#define SC3335_AGAIN_H_ADDR (0x3E08)
#define SC3335_AGAIN_L_ADDR (0x3E09)
#define SC3335_DGAIN_H_ADDR (0x3E06)
#define SC3335_DGAIN_L_ADDR (0x3E07)
#define SC3335_VMAX_H_ADDR (0x320E)
#define SC3335_VMAX_L_ADDR (0x320F)

#define SC3335_FULL_LINES_MAX (0x7FFF)
#define SC3335_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require */
#define SC3335_VMAX_1296P30_LINEAR (1350 + SC3335_INCREASE_LINES)
#define SC3335_FRAME_RATE_MIN (0x34BC)
#define EXP_OFFSET_LINEAR (10)

// sensor fps mode
#define SC3335_SENSOR_1296P_30FPS_LINEAR_MODE (1)
#define SC3335_RES_IS_1296P(w, h) ((w) <= 2304 && (h) <= 1296)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = SC3335_FULL_LINES_MAX * 2;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;

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
	case WDR_MODE_NONE:
		pstAeSnsDft->au8HistThresh[0] = 0x0D;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 16256;
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 16256; //31744;
		pstAeSnsDft->u32MinDgain = 1024;
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
	GK_U32 u32VMAX = SC3335_VMAX_1296P30_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case SC3335_SENSOR_1296P_30FPS_LINEAR_MODE:
		if ((f32Fps <= 30) && (f32Fps >= 4)) {
			u32VMAX = SC3335_VMAX_1296P30_LINEAR * 30 /
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

	u32VMAX = (u32VMAX > SC3335_FULL_LINES_MAX) ? SC3335_FULL_LINES_MAX :
						      u32VMAX;

	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGH_8BITS(u32VMAX);
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(u32VMAX);

	u32FullLines = u32VMAX << 1;
	pstSnsState->u32FLStd = u32FullLines;
	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - EXP_OFFSET_LINEAR;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines = (u32FullLines + EXP_OFFSET_LINEAR) >> 1;
	u32FullLines = (u32FullLines > SC3335_FRAME_RATE_MIN) ?
			       SC3335_FRAME_RATE_MIN :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines << 1;
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
		HIGH_8BITS(u32FullLines);
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
		LOW_8BITS(u32FullLines);

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - EXP_OFFSET_LINEAR;
	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val =
		(u32IntTime > (SC3335_FULL_LINES_MAX * 2 - EXP_OFFSET_LINEAR)) ?
			(SC3335_FULL_LINES_MAX * 2 - EXP_OFFSET_LINEAR) :
			u32IntTime;

	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
		HIGHER_4BITS(u32Val);
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
		HIGHER_8BITS(u32Val);
	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOWER_4BITS(u32Val);
	return;
}

static GK_U32 gain_index[256] = {
	1024,  1040,  1056,  1072,  1088,  1104,  1120,	 1136,	1152,  1168,
	1184,  1200,  1216,  1232,  1248,  1264,  1280,	 1296,	1312,  1328,
	1344,  1360,  1376,  1392,  1408,  1424,  1440,	 1456,	1472,  1488,
	1504,  1520,  1536,  1552,  1568,  1584,  1600,	 1616,	1632,  1648,
	1664,  1680,  1696,  1712,  1728,  1744,  1760,	 1776,	1792,  1808,
	1824,  1840,  1856,  1872,  1888,  1904,  1920,	 1936,	1952,  1968,
	1984,  2000,  2016,  2032,  2048,  2080,  2112,	 2144,	2176,  2208,
	2240,  2272,  2304,  2336,  2368,  2400,  2432,	 2464,	2496,  2528,
	2560,  2592,  2624,  2656,  2688,  2720,  2752,	 2784,	2816,  2848,
	2880,  2912,  2944,  2976,  3008,  3040,  3072,	 3104,	3136,  3168,
	3200,  3232,  3264,  3296,  3328,  3360,  3392,	 3424,	3456,  3488,
	3520,  3552,  3584,  3616,  3648,  3680,  3712,	 3744,	3776,  3808,
	3840,  3872,  3904,  3936,  3968,  4000,  4032,	 4064,	4096,  4160,
	4224,  4288,  4352,  4416,  4480,  4544,  4608,	 4672,	4736,  4800,
	4864,  4928,  4992,  5056,  5120,  5184,  5248,	 5312,	5376,  5440,
	5504,  5568,  5632,  5696,  5760,  5824,  5888,	 5952,	6016,  6080,
	6144,  6208,  6272,  6336,  6400,  6464,  6528,	 6592,	6656,  6720,
	6784,  6848,  6912,  6976,  7040,  7104,  7168,	 7232,	7296,  7360,
	7424,  7488,  7552,  7616,  7680,  7744,  7808,	 7872,	7936,  8000,
	8064,  8128,  8192,  8320,  8448,  8576,  8704,	 8832,	8960,  9088,
	9216,  9344,  9472,  9600,  9728,  9856,  9984,	 10112, 10240, 10368,
	10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648,
	11776, 11904, 12032, 12160, 12288, 12416, 12544, 12672, 12800, 12928,
	13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
	14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488,
	15616, 15744, 15872, 16000, 16128, 16256
};

static GK_U32 again_table[256] = {
	0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347, 0x0348,
	0x0349, 0x034A, 0x034B, 0x034C, 0x034D, 0x034E, 0x034F, 0x0350, 0x0351,
	0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357, 0x0358, 0x0359, 0x035A,
	0x035B, 0x035C, 0x035D, 0x035E, 0x035F, 0x0360, 0x0361, 0x0362, 0x0363,
	0x0364, 0x0365, 0x0366, 0x0367, 0x0368, 0x0369, 0x036A, 0x036B, 0x036C,
	0x036D, 0x036E, 0x036F, 0x0370, 0x0371, 0x0372, 0x0373, 0x0374, 0x0375,
	0x0376, 0x0377, 0x0378, 0x0379, 0x037A, 0x037B, 0x037C, 0x037D, 0x037E,
	0x037F, 0x0740, 0x0741, 0x0742, 0x0743, 0x0744, 0x0745, 0x0746, 0x0747,
	0x0748, 0x0749, 0x074A, 0x074B, 0x074C, 0x074D, 0x074E, 0x074F, 0x0750,
	0x0751, 0x0752, 0x0753, 0x0754, 0x0755, 0x0756, 0x0757, 0x0758, 0x0759,
	0x075A, 0x075B, 0x075C, 0x075D, 0x075E, 0x075F, 0x0760, 0x0761, 0x0762,
	0x0763, 0x0764, 0x0765, 0x0766, 0x0767, 0x0768, 0x0769, 0x076A, 0x076B,
	0x076C, 0x076D, 0x076E, 0x076F, 0x0770, 0x0771, 0x0772, 0x0773, 0x0774,
	0x0775, 0x0776, 0x0777, 0x0778, 0x0779, 0x077A, 0x077B, 0x077C, 0x077D,
	0x077E, 0x077F, 0x0F40, 0x0F41, 0x0F42, 0x0F43, 0x0F44, 0x0F45, 0x0F46,
	0x0F47, 0x0F48, 0x0F49, 0x0F4A, 0x0F4B, 0x0F4C, 0x0F4D, 0x0F4E, 0x0F4F,
	0x0F50, 0x0F51, 0x0F52, 0x0F53, 0x0F54, 0x0F55, 0x0F56, 0x0F57, 0x0F58,
	0x0F59, 0x0F5A, 0x0F5B, 0x0F5C, 0x0F5D, 0x0F5E, 0x0F5F, 0x0F60, 0x0F61,
	0x0F62, 0x0F63, 0x0F64, 0x0F65, 0x0F66, 0x0F67, 0x0F68, 0x0F69, 0x0F6A,
	0x0F6B, 0x0F6C, 0x0F6D, 0x0F6E, 0x0F6F, 0x0F70, 0x0F71, 0x0F72, 0x0F73,
	0x0F74, 0x0F75, 0x0F76, 0x0F77, 0x0F78, 0x0F79, 0x0F7A, 0x0F7B, 0x0F7C,
	0x0F7D, 0x0F7E, 0x0F7F, 0x1F40, 0x1F41, 0x1F42, 0x1F43, 0x1F44, 0x1F45,
	0x1F46, 0x1F47, 0x1F48, 0x1F49, 0x1F4A, 0x1F4B, 0x1F4C, 0x1F4D, 0x1F4E,
	0x1F4F, 0x1F50, 0x1F51, 0x1F52, 0x1F53, 0x1F54, 0x1F55, 0x1F56, 0x1F57,
	0x1F58, 0x1F59, 0x1F5A, 0x1F5B, 0x1F5C, 0x1F5D, 0x1F5E, 0x1F5F, 0x1F60,
	0x1F61, 0x1F62, 0x1F63, 0x1F64, 0x1F65, 0x1F66, 0x1F67, 0x1F68, 0x1F69,
	0x1F6A, 0x1F6B, 0x1F6C, 0x1F6D, 0x1F6E, 0x1F6F, 0x1F70, 0x1F71, 0x1F72,
	0x1F73, 0x1F74, 0x1F75, 0x1F76, 0x1F77, 0x1F78, 0x1F79, 0x1F7A, 0x1F7B,
	0x1F7C, 0x1F7D, 0x1F7E, 0x1F7F
};

static GK_U32 dgain_table[256] = {
	0x0080, 0x0082, 0x0084, 0x0086, 0x0088, 0x008A, 0x008C, 0x008E, 0x0090,
	0x0092, 0x0094, 0x0096, 0x0098, 0x009A, 0x009C, 0x009E, 0x00A0, 0x00A2,
	0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AC, 0x00AE, 0x00B0, 0x00B2, 0x00B4,
	0x00B6, 0x00B8, 0x00BA, 0x00BC, 0x00BE, 0x00C0, 0x00C2, 0x00C4, 0x00C6,
	0x00C8, 0x00CA, 0x00CC, 0x00CE, 0x00D0, 0x00D2, 0x00D4, 0x00D6, 0x00D8,
	0x00DA, 0x00DC, 0x00DE, 0x00E0, 0x00E2, 0x00E4, 0x00E6, 0x00E8, 0x00EA,
	0x00EC, 0x00EE, 0x00F0, 0x00F2, 0x00F4, 0x00F6, 0x00F8, 0x00FA, 0x00FC,
	0x00FE, 0x0180, 0x0182, 0x0184, 0x0186, 0x0188, 0x018A, 0x018C, 0x018E,
	0x0190, 0x0192, 0x0194, 0x0196, 0x0198, 0x019A, 0x019C, 0x019E, 0x01A0,
	0x01A2, 0x01A4, 0x01A6, 0x01A8, 0x01AA, 0x01AC, 0x01AE, 0x01B0, 0x01B2,
	0x01B4, 0x01B6, 0x01B8, 0x01BA, 0x01BC, 0x01BE, 0x01C0, 0x01C2, 0x01C4,
	0x01C6, 0x01C8, 0x01CA, 0x01CC, 0x01CE, 0x01D0, 0x01D2, 0x01D4, 0x01D6,
	0x01D8, 0x01DA, 0x01DC, 0x01DE, 0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8,
	0x01EA, 0x01EC, 0x01EE, 0x01F0, 0x01F2, 0x01F4, 0x01F6, 0x01F8, 0x01FA,
	0x01FC, 0x01FE, 0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038A, 0x038C,
	0x038E, 0x0390, 0x0392, 0x0394, 0x0396, 0x0398, 0x039A, 0x039C, 0x039E,
	0x03A0, 0x03A2, 0x03A4, 0x03A6, 0x03A8, 0x03AA, 0x03AC, 0x03AE, 0x03B0,
	0x03B2, 0x03B4, 0x03B6, 0x03B8, 0x03BA, 0x03BC, 0x03BE, 0x03C0, 0x03C2,
	0x03C4, 0x03C6, 0x03C8, 0x03CA, 0x03CC, 0x03CE, 0x03D0, 0x03D2, 0x03D4,
	0x03D6, 0x03D8, 0x03DA, 0x03DC, 0x03DE, 0x03E0, 0x03E2, 0x03E4, 0x03E6,
	0x03E8, 0x03EA, 0x03EC, 0x03EE, 0x03F0, 0x03F2, 0x03F4, 0x03F6, 0x03F8,
	0x03FA, 0x03FC, 0x03FE, 0x0780, 0x0782, 0x0784, 0x0786, 0x0788, 0x078A,
	0x078C, 0x078E, 0x0790, 0x0792, 0x0794, 0x0796, 0x0798, 0x079A, 0x079C,
	0x079E, 0x07A0, 0x07A2, 0x07A4, 0x07A6, 0x07A8, 0x07AA, 0x07AC, 0x07AE,
	0x07B0, 0x07B2, 0x07B4, 0x07B6, 0x07B8, 0x07BA, 0x07BC, 0x07BE, 0x07C0,
	0x07C2, 0x07C4, 0x07C6, 0x07C8, 0x07CA, 0x07CC, 0x07CE, 0x07D0, 0x07D2,
	0x07D4, 0x07D6, 0x07D8, 0x07DA, 0x07DC, 0x07DE, 0x07E0, 0x07E2, 0x07E4,
	0x07E6, 0x07E8, 0x07EA, 0x07EC, 0x07EE, 0x07F0, 0x07F2, 0x07F4, 0x07F6,
	0x07F8, 0x07FA, 0x07FC, 0x07FE
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= gain_index[255]) {
		*pu32AgainLin = gain_index[255];
		*pu32AgainDb = again_table[255];
		return;
	}

	for (i = 1; i < 256; i++) {
		if (*pu32AgainLin < gain_index[i]) {
			*pu32AgainLin = gain_index[i - 1];
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

	if (*pu32DgainLin >= gain_index[255]) {
		*pu32DgainLin = gain_index[255];
		*pu32DgainDb = dgain_table[255];
		return;
	}

	for (i = 1; i < 256; i++) {
		if (*pu32DgainLin < gain_index[i]) {
			*pu32DgainLin = gain_index[i - 1];
			*pu32DgainDb = dgain_table[i - 1];
			break;
		}
	}
	return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	GK_U8 u8Reg0x3e09, u8Reg0x3e08, u8Reg0x3e07, u8Reg0x3e06, u8Reg0x3109;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
	u8Reg0x3109 = sc3335_read_register(ViPipe, 0x3109);
	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00; // R0x3812
	if (1 == u8Reg0x3109) {
		if (u8Reg0x3e08 < 0x07) { // gain<2x
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x5; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x29; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x00; // R0x5799
		} else if (u8Reg0x3e08 < 0x0f) { //  2x <= gain < 4x
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x7; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x25; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x00; //R0x5799
		} else if (u8Reg0x3e08 < 0x1f) { //  4x <= gain < 8x
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x7; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x25; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x07; // R0x5799
		} else { //  8x <= gain
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x7; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x18; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x07; // R0x5799
		}
	} else {
		if (u8Reg0x3e08 < 0x07) { // gain<2x
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x6; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x29; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x00; // R0x5799
		} else if (u8Reg0x3e08 < 0x0f) { //  2x <= gain < 4x
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x8; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x25; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x00; //R0x5799
		} else if (u8Reg0x3e08 < 0x1f) { //  4x <= gain < 8x
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x8; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x25; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x07; // R0x5799
		} else { //  8x <= gain
			pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
				0x8; // R0x363c
			pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
				0x18; // R0x330e
			pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =
				0x07; // R0x5799
		}
	}

	pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x30; // R0x3812
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
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 5000;

	pstAwbSnsDft->au16GainOffset[0] = 392;
	pstAwbSnsDft->au16GainOffset[1] = 0x100;
	pstAwbSnsDft->au16GainOffset[2] = 0x100;
	pstAwbSnsDft->au16GainOffset[3] = 449;

	pstAwbSnsDft->as32WbPara[0] = 163;
	pstAwbSnsDft->as32WbPara[1] = -51;
	pstAwbSnsDft->as32WbPara[2] = -144;
	pstAwbSnsDft->as32WbPara[3] = 211617;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -150060;
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
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
	}

	pstDef->stSensorMode.u32SensorID = SC3335_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case SC3335_SENSOR_1296P_30FPS_LINEAR_MODE:
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
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstBlackLevel);
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;
	pstBlackLevel->au16BlackLevel[0] = 0x104;
	pstBlackLevel->au16BlackLevel[1] = 0x104;
	pstBlackLevel->au16BlackLevel[2] = 0x104;
	pstBlackLevel->au16BlackLevel[3] = 0x104;
	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32FullLines_5Fps = (SC3335_VMAX_1296P30_LINEAR * 30) / 5;
	u32MaxIntTime_5Fps = u32FullLines_5Fps * 2 - EXP_OFFSET_LINEAR;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		sc3335_write_register(ViPipe, SC3335_VMAX_H_ADDR,
				      HIGH_8BITS(u32FullLines_5Fps)); /* 5fps */
		sc3335_write_register(ViPipe, SC3335_VMAX_L_ADDR,
				      LOW_8BITS(u32FullLines_5Fps)); /* 5fps */
		sc3335_write_register(
			ViPipe, SC3335_EXP_H_ADDR,
			HIGHER_4BITS(
				u32MaxIntTime_5Fps)); /* max exposure lines */
		sc3335_write_register(
			ViPipe, SC3335_EXP_M_ADDR,
			HIGHER_8BITS(
				u32MaxIntTime_5Fps)); /* max exposure lines */
		sc3335_write_register(
			ViPipe, SC3335_EXP_L_ADDR,
			LOWER_4BITS(
				u32MaxIntTime_5Fps)); /* max exposure lines */

		sc3335_write_register(ViPipe, SC3335_DGAIN_H_ADDR, 0x00);
		sc3335_write_register(ViPipe, SC3335_DGAIN_L_ADDR, 0x80);
		sc3335_write_register(ViPipe, SC3335_AGAIN_H_ADDR, 0x03);
		sc3335_write_register(ViPipe, SC3335_AGAIN_L_ADDR, 0x40);
	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd > (SC3335_FULL_LINES_MAX * 2)) ?
				(SC3335_FULL_LINES_MAX * 2) :
				pstSnsState->u32FLStd;
		pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
		sc3335_write_register(ViPipe, SC3335_VMAX_H_ADDR,
				      HIGH_8BITS(pstSnsState->u32FLStd >>
						 1)); /* Standard FPS */
		sc3335_write_register(ViPipe, SC3335_VMAX_L_ADDR,
				      LOW_8BITS(pstSnsState->u32FLStd >>
						1)); /* Standard FPS */
		pstSnsState->bSyncInit = GK_FALSE;
	}
	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunSc3335BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
		pstSnsState->astRegsInfo[0].u32RegNum = 14;

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				sc3335_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = sc3335_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = sc3335_data_byte;
		}
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			SC3335_EXP_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			SC3335_EXP_M_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			SC3335_EXP_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			SC3335_DGAIN_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			SC3335_DGAIN_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			SC3335_AGAIN_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			SC3335_AGAIN_L_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
			SC3335_VMAX_H_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
			SC3335_VMAX_L_ADDR;

		pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
			0x3812; /* Grouphold Logic */
		pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = 0x363c;
		pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = 0x330e;

		pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = 0x5799;

		pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = 0x3812;
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
		    pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate ||
		    pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate) {
			pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[13].bUpdate =
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
	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= 30) {
		if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if (SC3335_RES_IS_1296P(pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height)) {
				u8SensorImageMode =
					SC3335_SENSOR_1296P_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd =
					SC3335_VMAX_1296P30_LINEAR * 2;
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

	SC3335_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = SC3335_SENSOR_1296P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = SC3335_VMAX_1296P30_LINEAR * 2;
	pstSnsState->au32FL[0] = SC3335_VMAX_1296P30_LINEAR * 2;
	pstSnsState->au32FL[1] = SC3335_VMAX_1296P30_LINEAR * 2;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x00; // intt
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = 0xa8;
	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0x20;
	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = 0x00; // gain
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = 0x80;
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = 0x03;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x40;
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x05; // vts
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x46;

	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00; // group hold
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x05;
	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x29;
	pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x30;
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = sc3335_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = sc3335_exit;
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
static GK_S32 sensor_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunSc3335BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC3335_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

	if (pastSnsStateCtx == GK_NULL) {
		pastSnsStateCtx =
			(ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
		if (pastSnsStateCtx == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] SnsCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_ISP_NOMEM;
		}
	}

	memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

	SC3335_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC3335_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	SC3335_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = SC3335_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC3335_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC3335_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC3335_ID);
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

ISP_SNS_OBJ_S stSnsSc3335Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = sc3335_standby,
	.pfnRestart = sc3335_restart,
	.pfnMirrorFlip = sc3335_mirror_flip,
	.pfnWriteReg = sc3335_write_register,
	.pfnReadReg = sc3335_read_register,
	.pfnSetBusInfo = sensor_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif /* End of #ifdef __cplusplus */
