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
#include "sc2235_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SC2235_ID 2235

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

ISP_SNS_STATE_S *g_pastSc2235[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define SC2235_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSc2235[dev])
#define SC2235_SENSOR_SET_CTX(dev, pstCtx) (g_pastSc2235[dev] = pstCtx)
#define SC2235_SENSOR_RESET_CTX(dev) (g_pastSc2235[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc2235BusInfo[ISP_MAX_PIPE_NUM] = {
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
extern const unsigned int sc2235_i2c_addr;
extern unsigned int sc2235_addr_byte;
extern unsigned int sc2235_data_byte;

extern void sc2235_init(VI_PIPE ViPipe);
extern void sc2235_exit(VI_PIPE ViPipe);
extern void sc2235_standby(VI_PIPE ViPipe);
extern void sc2235_restart(VI_PIPE ViPipe);
extern int sc2235_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sc2235_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

#define SC2235_FULL_LINES_MAX (0xFFF)

/*****SC2235 Register Address*****/
#define SC2235_EXPTIME0_ADDR (0x3E01)
#define SC2235_EXPTIME1_ADDR (0x3E02)

#define SC2235_GAIN0_ADDR (0x3E07)
#define SC2235_GAIN1_ADDR (0x3E08)
#define SC2235_GAIN2_ADDR (0x3E09)

#define SC2235_VMAX0_ADDR (0x320E)
#define SC2235_VMAX1_ADDR (0x320F)
#define SC2235_HMAX0_ADDR (0x320C)
#define SC2235_HMAX1_ADDR (0x320D)

#define SC2235_INCREASE_LINES \
	(0) /* make real fps less than stand fps because NVR require */
#define SC2235_VMAX_1080P30_LINEAR (1125 + SC2235_INCREASE_LINES)

/* sensor fps mode */
#define SC2235_SENSOR_1080P_30FPS_LINEAR_MODE (0)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 50 * 256;
	pstAeSnsDft->u32FullLinesMax = SC2235_FULL_LINES_MAX;
	pstAeSnsDft->u8AERunInterval = 1;
	pstAeSnsDft->u32InitExposure = 10;

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
	case WDR_MODE_NONE:
		pstAeSnsDft->au8HistThresh[0] = 0x0D;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;

		pstAeSnsDft->u32MaxAgain = 15872; // 15.5*1024
		pstAeSnsDft->u32MinAgain = 1024;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

		pstAeSnsDft->u32MaxDgain = 4096; // 4
		pstAeSnsDft->u32MinDgain = 1024;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u8AeCompensation = 0x38;
		pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

		pstAeSnsDft->u32InitExposure =
			g_au32InitExposure[ViPipe] ?
				g_au32InitExposure[ViPipe] :
				148859;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
		pstAeSnsDft->u32MinIntTime = 1;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 1;
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
	GK_U32 u32VMAX = SC2235_VMAX_1080P30_LINEAR;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	switch (pstSnsState->u8ImgMode) {
	case SC2235_SENSOR_1080P_30FPS_LINEAR_MODE:
		if ((f32Fps <= 30) && (f32Fps > 8.229)) {
			u32VMAX = SC2235_VMAX_1080P30_LINEAR * 30 /
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
	u32VMAX = (u32VMAX > SC2235_FULL_LINES_MAX) ? SC2235_FULL_LINES_MAX :
						      u32VMAX;
	pstSnsState->u32FLStd = u32VMAX;

	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
		((u32VMAX & 0xFF00) >> 8);
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (u32VMAX & 0xFF);

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = SC2235_VMAX_1080P30_LINEAR * 15;
	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

	return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
				       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);
	u32FullLines = (u32FullLines > SC2235_FULL_LINES_MAX) ?
			       SC2235_FULL_LINES_MAX :
			       u32FullLines;
	pstSnsState->au32FL[0] = u32FullLines;

	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
		((pstSnsState->au32FL[0] & 0xFF00) >> 8);
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
		(pstSnsState->au32FL[0] & 0xFF);

	pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 4;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	GK_U32 u32Val;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	u32Val = (u32IntTime > SC2235_FULL_LINES_MAX) ? SC2235_FULL_LINES_MAX :
							u32IntTime;
	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (u32Val >> 4) &
							    0xFF;
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32Val << 4) &
							    0xF0;

	if (u32Val < 0x23B) { // SC2235_logic_with inttime   old 0x50
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x12;
	} else if (u32Val > 0x24B) { // old a0
		pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x02;
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

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	GK_U8 u8Reg0x3e09, u8Reg0x3e08;
	GK_U8 i, leftnum;
	int gain_all = u16AgainTab[u32Again] * u32Dgain;

	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	// Again
	u8Reg0x3e09 = 0x10 | (u32Again & 0x0f);
	u8Reg0x3e08 = 0x03;
	leftnum = u32Again / 16; // Again_index >>>> Left_shift_num
	for (i = 0; i < leftnum; i++) {
		u8Reg0x3e08 = (u8Reg0x3e08 << 1) | 0x01;
	}
	// Dgain
	if (u32Dgain == 2) {
		u8Reg0x3e08 |= 0x20;
	} else if (u32Dgain == 4) {
		u8Reg0x3e08 |= 0x60;
	} else if (u32Dgain == 8) {
		u8Reg0x3e08 |= 0xe0;
	}
	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = u8Reg0x3e08;
	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = u8Reg0x3e09;

	// Sensor_Logic
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x84; // 0x3903
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x04; // 0x3903
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x00; // 0x3812
	if (u16AgainTab[u32Again] < 2 * 1024) {
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			0x05; // 0x3301
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			0x84; // 0x3631
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =
			0x2f; // 0x366f
	} else if (u16AgainTab[u32Again] < 8 * 1024) {
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x15;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x2f;
	} else if (u16AgainTab[u32Again] < 15.5 * 1024) {
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x19;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x2f;
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0xff;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x88;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x3c;
	}

	// Sensor_Internal_DPC
	if (gain_all < 10 * 1024) {
		pstSnsState->astRegsInfo[0].astI2cData[13].u32Data =
			0x04; // 0x5781
		pstSnsState->astRegsInfo[0].astI2cData[14].u32Data =
			0x18; // 0x5785
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x02;
		pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = 0x08;
	}
	pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = 0x30; // 0x3812

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
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 5082;

	pstAwbSnsDft->au16GainOffset[0] = 0x15E; // R
	pstAwbSnsDft->au16GainOffset[1] = 0x100; // G
	pstAwbSnsDft->au16GainOffset[2] = 0x100; // G
	pstAwbSnsDft->au16GainOffset[3] = 0x195; // B

	pstAwbSnsDft->as32WbPara[0] = 207;
	pstAwbSnsDft->as32WbPara[1] = -141;
	pstAwbSnsDft->as32WbPara[2] = -190;
	pstAwbSnsDft->as32WbPara[3] = 186726;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -136275;
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
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
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

	pstDef->stSensorMode.u32SensorID = SC2235_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	switch (pstSnsState->u8ImgMode) {
	default:
	case SC2235_SENSOR_1080P_30FPS_LINEAR_MODE:
		pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
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
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;
	pstBlackLevel->au16BlackLevel[0] = 0x47;
	pstBlackLevel->au16BlackLevel[1] = 0x47;
	pstBlackLevel->au16BlackLevel[2] = 0x47;
	pstBlackLevel->au16BlackLevel[3] = 0x47;

	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	// min fps is 8.23,so u32FullLines_5Fps = SC2235_FULL_LINES_MAX
	GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->u8ImgMode == SC2235_SENSOR_1080P_30FPS_LINEAR_MODE) {
		u32FullLines_5Fps = SC2235_FULL_LINES_MAX;
	} else {
		return;
	}
	u32MaxIntTime_5Fps = u32FullLines_5Fps;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		sc2235_write_register(ViPipe, SC2235_VMAX0_ADDR,
				      (u32FullLines_5Fps & 0xFF00) >>
					      8); /* 5fps */
		sc2235_write_register(ViPipe, SC2235_VMAX1_ADDR,
				      u32FullLines_5Fps & 0xFF); /* 5fps */
		sc2235_write_register(ViPipe, SC2235_EXPTIME0_ADDR,
				      ((u32MaxIntTime_5Fps >> 4) &
				       0xFF)); /* max exposure lines */
		sc2235_write_register(ViPipe, SC2235_EXPTIME1_ADDR,
				      ((u32MaxIntTime_5Fps << 4) &
				       0xF0)); /* max exposure lines */
		sc2235_write_register(ViPipe, SC2235_GAIN0_ADDR,
				      0x00); /* min Gain = 1 */
		sc2235_write_register(ViPipe, SC2235_GAIN1_ADDR, 0x03);
		sc2235_write_register(ViPipe, SC2235_GAIN2_ADDR, 0x10);
	} else { /* setup for ISP 'normal mode' */
		pstSnsState->u32FLStd =
			(pstSnsState->u32FLStd > SC2235_FULL_LINES_MAX) ?
				SC2235_FULL_LINES_MAX :
				pstSnsState->u32FLStd;
		pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
		sc2235_write_register(ViPipe, SC2235_VMAX0_ADDR,
				      ((pstSnsState->u32FLStd & 0xFF00) >>
				       8)); /* Standard FPS */
		sc2235_write_register(ViPipe, SC2235_VMAX1_ADDR,
				      (pstSnsState->u32FLStd &
				       0xFF)); /* Standard FPS */
		pstSnsState->bSyncInit = GK_FALSE;
	}

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		if (pstSnsState->u8ImgMode ==
		    SC2235_SENSOR_1080P_30FPS_LINEAR_MODE) {
			pstSnsState->u32FLStd = SC2235_VMAX_1080P30_LINEAR;
			ISP_TRACE(MODULE_DBG_ERR,
				  "SC2235_SENSOR_1080P_30FPS_LINEAR_MODE\n");
		} else {
			ISP_TRACE(MODULE_DBG_ERR,
				  "cmos_set_wdr_mode_Mode's name is wrong\n");
			return GK_FAILURE;
		}
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR,
			  "cmos_set_wdr_mode_NOT support this mode!\n");
		return GK_FAILURE;
		break;
	}

	pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
	pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
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
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunSc2235BusInfo[ViPipe].s8I2cDev;

		// if isp is online mode, configured to 3, if it is offline mode, configured to 2
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 3;

		pstSnsState->astRegsInfo[0].u32RegNum = 16;
		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				sc2235_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = sc2235_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = sc2235_data_byte;
		}
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			SC2235_EXPTIME0_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			SC2235_EXPTIME1_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr =
			SC2235_GAIN1_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			SC2235_GAIN2_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			SC2235_VMAX0_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			SC2235_VMAX1_ADDR;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = 0x3903;
		pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = 0x3903;
		pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = 0x3812;
		pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = 0x3301;
		pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = 0x3631;
		pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = 0x366f;
		pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = 0x3314;
		pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 2;
		pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = 0x5781;
		pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 2;
		pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = 0x5785;
		pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr = 0x3812;
		pstSnsState->bSyncInit = GK_TRUE;
	} else {
		for (i = 0; i < 6; i++) { // AEC/AGC_Update
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
		if (GK_TRUE ==
		    (pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate ||
		     pstSnsState->astRegsInfo[0].astI2cData[1].bUpdate ||
		     pstSnsState->astRegsInfo[0].astI2cData[2].bUpdate ||
		     pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate)) {
			pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate =
				GK_TRUE; // Grouphold_AEC/AGC_Logic
			pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[8].bUpdate =
				GK_TRUE;
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
			pstSnsState->astRegsInfo[0].astI2cData[14].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[15].bUpdate =
				GK_TRUE;
		} else {
			pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate =
				GK_FALSE; // Grouphold_AEC/AGC_Logic
			pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[8].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[13].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[14].bUpdate =
				GK_FALSE;
			pstSnsState->astRegsInfo[0].astI2cData[15].bUpdate =
				GK_FALSE;
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
	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if ((pstSensorImageMode->u16Width <= 1920) &&
	    (pstSensorImageMode->u16Height <= 1080)) {
		if (pstSensorImageMode->f32Fps <= 30) {
			u8SensorImageMode =
				SC2235_SENSOR_1080P_30FPS_LINEAR_MODE;

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

	return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	SC2235_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = SC2235_SENSOR_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = SC2235_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[0] = SC2235_VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[1] = SC2235_VMAX_1080P30_LINEAR;

	pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = 0x10;
	pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0x03;
	pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = 0x10;
	pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = 0x04;
	pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = 0x65;
	pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x84;
	pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x04;
	pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x00;
	pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x05;
	pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x84;
	pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x2F;
	pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x02;
	pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x04;
	pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = 0x18;
	pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = 0x30;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);
	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));
	pstSensorExpFunc->pfn_cmos_sensor_init = sc2235_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = sc2235_exit;
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

static GK_S32 sc2235_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunSc2235BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC2235_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	SC2235_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	SC2235_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	SC2235_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = SC2235_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC2235_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC2235_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC2235_ID);
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

ISP_SNS_OBJ_S stSnsSc2235Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = sc2235_standby,
	.pfnRestart = sc2235_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = sc2235_write_register,
	.pfnReadReg = sc2235_read_register,
	.pfnSetBusInfo = sc2235_set_bus_info,
	.pfnSetInit = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
