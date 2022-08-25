/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__OV2718_CMOS_H_)
#define __OV2718_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#include "ov2718_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OV2718_ID 2718

ISP_SNS_STATE_S *g_pastOv2718[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define OV2718_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastOv2718[dev])
#define OV2718_SENSOR_SET_CTX(dev, pstCtx) (g_pastOv2718[dev] = pstCtx)
#define OV2718_SENSOR_RESET_CTX(dev) (g_pastOv2718[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunOv2718BusInfo[ISP_MAX_PIPE_NUM] = {
	[0] = { .s8I2cDev = 0 },
	[1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };

extern const unsigned int ov2718_i2c_addr;
extern unsigned int ov2718_addr_byte;
extern unsigned int ov2718_data_byte;

extern void ov2718_init(VI_PIPE ViPipe);
extern void ov2718_exit(VI_PIPE ViPipe);
extern void ov2718_standby(VI_PIPE ViPipe);
extern void ov2718_restart(VI_PIPE ViPipe);
extern int ov2718_write_register(VI_PIPE ViPipe, int addr, int data);
extern int ov2718_read_register(VI_PIPE ViPipe, int addr);

#define FULL_LINES_MAX (0xFFFF)

#define EXPOSURE_ADDR_HHH (0x0220)
#define EXPOSURE_ADDR_H (0x30B6)
#define EXPOSURE_ADDR_L (0x30B7)
#define SHORT_EXPOSURE_ADDR_H (0x312A)
#define SHORT_EXPOSURE_ADDR_L (0x312B)
#define AGC_ADDR (0x30BB)
#define DGC_ADDR_H (0x315A)
#define DGC_ADDR_L (0x315B)
#define LCG_ADDR_H (0x315C)
#define LCG_ADDR_L (0x315D)
#define AGC_ADDR_H (0x0204)
#define AGC_ADDR_L (0x0205)
#define VMAX_ADDR_H (0x30B2)
#define VMAX_ADDR_L (0x30B3)

#define INCREASE_LINES \
	(2) /* make real fps less than stand fps because NVR require */
#define VMAX_1080P30_LINEAR (1136 + INCREASE_LINES)
#define VMAX_1080P60_LINEAR (1125 + INCREASE_LINES)
#define VMAX_1080P60TO30_WDR (1136 + INCREASE_LINES)
#define VMAX_1080P50TO25_WDR (1500 + INCREASE_LINES)
#define VMAX_1080P120TO30_WDR (1125 + INCREASE_LINES)
#define VMAX_720P60TO30_WDR (750 + INCREASE_LINES)
#define VMAX_720P60TO20_WDR (750 + INCREASE_LINES)
#define VMAX_720P60TO15_WDR (750 + INCREASE_LINES)
#define VMAX_720P120_LINEAR (750 + INCREASE_LINES)
#define VMAX_VGA240_LINEAR (562 + INCREASE_LINES)

#define OV2718_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define OV2718_SENSOR_1080P_30FPS_WDR_2To1_MODE (2)
#define OV2718_SENSOR_1080P_30FPS_BUILT_IN_MODE (3)

static GK_U32 again_table[8] = { 1024,	2048,  4096,  8192,
				 11264, 22528, 45056, 90112 };

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
				  AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAeSnsDft);
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

	pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
	pstAeSnsDft->u32FlickerFreq = 0;
	pstAeSnsDft->u32FullLinesMax = FULL_LINES_MAX;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 6;

	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 0.00390625;

	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 6 << pstAeSnsDft->u32ISPDgainShift;

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

	pstAeSnsDft->u32InitExposure = 921600;

	if ((pstAeSnsDft->f32Fps == 50) || (pstAeSnsDft->f32Fps == 60)) {
		pstAeSnsDft->stIntTimeAccu.f32Offset = 0.6082;
	} else if ((pstAeSnsDft->f32Fps == 25) || (pstAeSnsDft->f32Fps == 30)) {
		pstAeSnsDft->stIntTimeAccu.f32Offset = 0.8045;
	} else {
	}

	switch (pstSnsState->enWDRMode) {
	default:
	case WDR_MODE_NONE: /* linear mode */
		pstAeSnsDft->au8HistThresh[0] = 0xd;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;
		pstAeSnsDft->u8AeCompensation = 0x30;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
		pstAeSnsDft->u32MinIntTime = 2;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 2;
		pstAeSnsDft->u32MaxAgain = 90112;
		pstAeSnsDft->u32MinAgain = 2048;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
		pstAeSnsDft->u32MaxDgain = 1536;
		pstAeSnsDft->u32MinDgain = 384;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		break;
	case WDR_MODE_BUILT_IN:
		pstAeSnsDft->au8HistThresh[0] = 0xd;
		pstAeSnsDft->au8HistThresh[1] = 0x28;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;
		pstAeSnsDft->u8AeCompensation = 0x38;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
		pstAeSnsDft->u32MinIntTime = 2;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 2;
		//pstAeSnsDft->u32MaxAgain = 23171;
		pstAeSnsDft->u32MaxAgain = 8192;
		pstAeSnsDft->u32MinAgain = 2048;
		pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
		pstAeSnsDft->u32MaxDgain = 16383;
		pstAeSnsDft->u32MinDgain = 384;
		pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

		pstAeSnsDft->u16ManRatioEnable = GK_TRUE;
		pstAeSnsDft->au32Ratio[0] = 16 * 64;
		break;

	case WDR_MODE_2To1_LINE:
		pstAeSnsDft->au8HistThresh[0] = 0xC;
		pstAeSnsDft->au8HistThresh[1] = 0x18;
		pstAeSnsDft->au8HistThresh[2] = 0x60;
		pstAeSnsDft->au8HistThresh[3] = 0x80;
		pstAeSnsDft->u8AeCompensation = 64;
		pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
		pstAeSnsDft->u32MinIntTime = 2;
		pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		pstAeSnsDft->u32MinIntTimeTarget = 140;
		pstAeSnsDft->u32MaxAgain = 8192;
		pstAeSnsDft->u32MinAgain = 2048;
		pstAeSnsDft->u32MaxAgainTarget = 8192;
		pstAeSnsDft->u32MinAgainTarget = 2048;
		pstAeSnsDft->u32MaxDgain = 16382;
		pstAeSnsDft->u32MinDgain = 384;
		pstAeSnsDft->u32MaxDgainTarget = 1024;

		pstAeSnsDft->u32LFMaxShortTime = 300;
		pstAeSnsDft->u32MaxIntTimeStep = 3000;
		pstAeSnsDft->u32LFMinExposure = 880000;

		pstAeSnsDft->u32MinDgainTarget = 384;
		pstAeSnsDft->u16ManRatioEnable = GK_TRUE;
		pstAeSnsDft->au32Ratio[0] = 16 * 64;
		if ((pstAeSnsDft->f32Fps == 50) ||
		    (pstAeSnsDft->f32Fps == 60)) {
			pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2155;
		} else if ((pstAeSnsDft->f32Fps == 25) ||
			   (pstAeSnsDft->f32Fps == 30)) {
			pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2818;
		} else {
		}
		break;
	}
	return GK_SUCCESS;
}

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
				   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstAwbSnsDft);
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
	pstAwbSnsDft->u16WbRefTemp = 5120;
	pstAwbSnsDft->au16GainOffset[0] = 0x25B;
	pstAwbSnsDft->au16GainOffset[1] = 0x100;
	pstAwbSnsDft->au16GainOffset[2] = 0x100;
	pstAwbSnsDft->au16GainOffset[3] = 0x19A;
	pstAwbSnsDft->as32WbPara[0] = -28;
	pstAwbSnsDft->as32WbPara[1] = 284;
	pstAwbSnsDft->as32WbPara[2] = 0;
	pstAwbSnsDft->as32WbPara[3] = 163812;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -113080;

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
	case WDR_MODE_BUILT_IN:
		memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmFsWdr,
		       sizeof(AWB_CCM_S));
		memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR,
		       sizeof(AWB_AGC_TABLE_S));
		break;
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

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstDef);
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
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
		pstDef->unKey.bit1Ge = 1;
		pstDef->pstGe = &g_stIspGe;
		pstDef->unKey.bit1AntiFalseColor = 1;
		pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
		pstDef->unKey.bit1Ldci = 1;
		pstDef->pstLdci = &g_stIspLdci;
		pstDef->unKey.bit1Dehaze = 1;
		pstDef->pstDehaze = &g_stIspDehaze;
		pstDef->unKey.bit1Lcac = 0;
		pstDef->pstLcac = &g_stIspLCac;
		pstDef->unKey.bit1Rgbir = 0;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	case WDR_MODE_BUILT_IN:
		pstDef->unKey.bit1Expander = 1;
		pstDef->pstExpander = &g_stCmosExpander;
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
		pstDef->unKey.bit1Rgbir = 0;
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
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
		pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
		memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
		       sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
		break;
	}

	pstDef->stSensorMode.u32SensorID = OV2718_ID;
	pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

	memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
	       sizeof(ISP_CMOS_DNG_COLORPARAM_S));

	pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
	pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;

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

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
			    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE)) {
		/* In WDR_MODE_2To1_LINE mode, 30fps means 60 frames(30 short and 30 long) combine to 30 frames per sencond */
		if ((f32Fps <= 30) && (f32Fps >= 0.51)) {
			if (pstSnsState->u8ImgMode ==
			    OV2718_SENSOR_1080P_30FPS_WDR_2To1_MODE) {
				pstSnsState->u32FLStd =
					VMAX_1080P60TO30_WDR * 30 /
					DIV_0_TO_1_FLOAT(f32Fps);
				pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2818;
			} else {
				ISP_TRACE(MODULE_DBG_ERR,
					  "Not support ImageMode&Fps: %d %f\n",
					  pstSnsState->u8ImgMode, f32Fps);
				return;
			}
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
	} else if (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN) {
		if ((f32Fps <= 30) && (f32Fps >= 0.51)) {
			if (pstSnsState->u8ImgMode ==
			    OV2718_SENSOR_1080P_30FPS_BUILT_IN_MODE) {
				pstSnsState->u32FLStd =
					VMAX_1080P60TO30_WDR * 30 /
					DIV_0_TO_1_FLOAT(f32Fps);
				pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2818;
			} else {
				ISP_TRACE(MODULE_DBG_ERR,
					  "Not support ImageMode&Fps: %d %f\n",
					  pstSnsState->u8ImgMode, f32Fps);
				return;
			}
		} else {
			ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n",
				  f32Fps);
			return;
		}
	} else {
		if (pstSnsState->u8ImgMode ==
		    OV2718_SENSOR_1080P_30FPS_LINEAR_MODE) {
			if ((f32Fps <= 30) && (f32Fps >= 0.51)) {
				/* In 1080P30fps mode, the VMAX(FullLines) is VMAX_1080P30_LINEAR,
                         and there are (VMAX_1080P30_LINEAR*30) lines in 1 second,
                         so in f32Fps mode, VMAX(FullLines) is (VMAX_1080P30_LINEAR*30)/f32Fps */
				pstSnsState->u32FLStd =
					(VMAX_1080P30_LINEAR * 30) /
					DIV_0_TO_1_FLOAT(f32Fps);
				pstAeSnsDft->stIntTimeAccu.f32Offset = 0.8045;
			} else {
				ISP_TRACE(MODULE_DBG_ERR,
					  "Not support Fps: %f\n", f32Fps);
				return;
			}
		} else {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"Not support! pstSnsState->u8ImgMode:%d, f32Fps:%f\n",
				pstSnsState->u8ImgMode, f32Fps);
			return;
		}
	}

	pstSnsState->u32FLStd = (pstSnsState->u32FLStd > FULL_LINES_MAX) ?
					FULL_LINES_MAX :
					pstSnsState->u32FLStd;
	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			((pstSnsState->u32FLStd & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			(pstSnsState->u32FLStd & 0xFF);

	} else if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) ||
		   (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE) ||
		   (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME)) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			((pstSnsState->u32FLStd & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			(pstSnsState->u32FLStd & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			((pstSnsState->u32FLStd & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			(pstSnsState->u32FLStd & 0xFF);
	} else if (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			((pstSnsState->u32FLStd & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			(pstSnsState->u32FLStd & 0xFF);

	} else {
	}
	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
	pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
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
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		u32FullLines = (u32FullLines > FULL_LINES_MAX) ?
				       FULL_LINES_MAX :
				       u32FullLines;
		pstSnsState->au32FL[0] = u32FullLines;
		pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			((u32FullLines & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			(u32FullLines & 0xFF);
	} else if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) ||
		   (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN)) {
		u32FullLines = (u32FullLines + 20) / 12;
		u32FullLines = (u32FullLines < pstAeSnsDft->u32FullLinesStd) ?
				       pstAeSnsDft->u32FullLinesStd :
				       u32FullLines;
		u32FullLines = (u32FullLines > FULL_LINES_MAX) ?
				       FULL_LINES_MAX :
				       u32FullLines;
		pstSnsState->au32FL[0] = u32FullLines;
		pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
		pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =
			((u32FullLines & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[6].u32Data =
			(u32FullLines & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[9].u32Data =
			((u32FullLines & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =
			(u32FullLines & 0xFF);
	} else {
		pstAeSnsDft->u32FullLines = pstSnsState->u32FLStd;
		return;
	}
	pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 4;
	return;
}
/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
	static GK_BOOL bFirst = GK_TRUE;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if ((pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE) ||
	    (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME)) {
		if (bFirst) { /* short exposure */
			pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
				((u32IntTime & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
				(u32IntTime & 0xFF);
			bFirst = GK_FALSE;
		} else { /* long exposure */
			bFirst = GK_TRUE;
		}
	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		if (bFirst) { /* short exposure */
			pstSnsState->au32WDRIntTime[0] = u32IntTime;
			pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
				((u32IntTime & 0xFF00) >> 8);
			pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
				(u32IntTime & 0xFF);
			bFirst = GK_FALSE;
		} else { /* long exposure */
			bFirst = GK_TRUE;
		}
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =
			((u32IntTime & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[1].u32Data =
			(u32IntTime & 0xFF);
		bFirst = GK_TRUE;
	}
	return;
}

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
				     GK_U32 *pu32AgainDb)
{
	int i;

	CMOS_CHECK_POINTER_VOID(pu32AgainLin);
	CMOS_CHECK_POINTER_VOID(pu32AgainDb);

	if (*pu32AgainLin >= again_table[7]) {
		*pu32AgainLin = again_table[7];
		*pu32AgainDb = 7;
		return;
	}

	for (i = 1; i < 8; i++) {
		if (*pu32AgainLin < again_table[i]) {
			*pu32AgainLin = again_table[i - 1];
			*pu32AgainDb = i - 1;
			break;
		}
	}

	return;
}
static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
				 GK_U32 u32Dgain)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
		if (u32Again < 1) {
			u32Again = 0x0; // LCG 1024
		} else if (u32Again < 2) {
			u32Again = 0x1; // LCG 2048
		} else if (u32Again < 3) {
			u32Again = 0x2; // LCG 4096
		} else if (u32Again < 4) {
			u32Again = 0x3; // LCG 8192
		} else if (u32Again < 5) {
			u32Again = 0x40; // HCG 1024
		} else if (u32Again < 6) {
			u32Again = 0x41; // HCG 2048
		} else if (u32Again < 7) {
			u32Again = 0x42; // HCG 4096
		} else {
			u32Again = 0x43; // HCG 8192
		}
		pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = u32Again &
								    0xFF;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
			((u32Dgain & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
			(u32Dgain & 0xFF);

	} else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		u32Again = (u32Again > 3) ? 3 : u32Again;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
			(u32Again << 2) | u32Again;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
			((u32Dgain & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
			(u32Dgain & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
			((u32Dgain & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			(u32Dgain & 0xFF);

	} else if (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN) {
		u32Again = (u32Again > 3) ? 3 : u32Again;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =
			(u32Again << 2) | u32Again;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32Data =
			((u32Dgain & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[4].u32Data =
			(u32Dgain & 0xFF);
		pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
			((u32Dgain & 0xFF00) >> 8);
		pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
			(u32Dgain & 0xFF);
	}
	return;
}

GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	CMOS_CHECK_POINTER(pstExpFuncs);

	memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

	pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
	pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
	pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
	pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
	pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
	pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;
	return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,
				       ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
	GK_S32 i;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstBlackLevel);
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = GK_FALSE;
	if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE)) {
		for (i = 0; i < 4; i++) {
			pstBlackLevel->au16BlackLevel[i] = 0x40;
		}
	} else if (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN) {
		for (i = 0; i < 4; i++) {
			pstBlackLevel->au16BlackLevel[i] = 0x40;
		}
	} else {
		for (i = 0; i < 4; i++) {
			pstBlackLevel->au16BlackLevel[i] = 0x40;
		}
	}
	return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
		return;
	} else {
		u32FullLines_5Fps = (VMAX_1080P30_LINEAR * 30) / 5;
	}

	u32MaxIntTime_5Fps = u32FullLines_5Fps - 2;

	if (bEnable) { /* setup for ISP pixel calibration mode */
		ov2718_write_register(ViPipe, VMAX_ADDR_H,
				      (u32FullLines_5Fps & 0xFF00) >>
					      8); /* 5fps */
		ov2718_write_register(ViPipe, VMAX_ADDR_L,
				      u32FullLines_5Fps & 0xFF); /* 5fps */
		ov2718_write_register(ViPipe, 0x0202,
				      (u32MaxIntTime_5Fps & 0xFF00) >>
					      8); /* max exposure lines */
		ov2718_write_register(ViPipe, 0x0203,
				      u32MaxIntTime_5Fps &
					      0xFF); /* max exposure lines */
		ov2718_write_register(ViPipe, 0x0204, 0x01); /* min AG */
		ov2718_write_register(ViPipe, 0x0205, 0x00); /* min AG */
		ov2718_write_register(ViPipe, 0x3108, 0x01); /* min DG */
		ov2718_write_register(ViPipe, 0x3109, 0x00); /* min DG */
	} else { /* setup for ISP 'normal mode' */
		ov2718_write_register(ViPipe, VMAX_ADDR_H,
				      (pstSnsState->u32FLStd & 0xFF00) >> 8);
		ov2718_write_register(ViPipe, VMAX_ADDR_L,
				      pstSnsState->u32FLStd & 0xFF);
		pstSnsState->bInit = GK_FALSE;
	}

	return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	pstSnsState->bSyncInit = GK_FALSE;

	switch (u8Mode & 0x3F) {
	case WDR_MODE_NONE:
		pstSnsState->enWDRMode = WDR_MODE_NONE;
		printf("linear mode\n");
		break;

	case WDR_MODE_2To1_LINE:
		pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
		printf("2to1 line WDR mode\n");
		break;

	case WDR_MODE_BUILT_IN:
		pstSnsState->enWDRMode = WDR_MODE_BUILT_IN;
		printf("sensor built in WDR mode\n");
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
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	if ((pstSnsState->bSyncInit == GK_FALSE) ||
	    (pstSnsRegsInfo->bConfig == GK_FALSE)) {
		pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
		pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
			g_aunOv2718BusInfo[ViPipe].s8I2cDev;
		pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 3;
		pstSnsState->astRegsInfo[0].u32RegNum = 9;

		for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
			pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate =
				GK_TRUE;
			pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr =
				ov2718_i2c_addr;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32AddrByteNum = ov2718_addr_byte;
			pstSnsState->astRegsInfo[0]
				.astI2cData[i]
				.u32DataByteNum = ov2718_data_byte;
		}
		pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr =
			EXPOSURE_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr =
			EXPOSURE_ADDR_L;

		/* AG */
		pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = AGC_ADDR;

		/* DG */
		pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr =
			DGC_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr =
			DGC_ADDR_L;

		/* VMAX */
		pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr =
			VMAX_ADDR_H;
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr =
			VMAX_ADDR_L;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1;

		if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) ||
		    (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN)) {
			pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr =
				LCG_ADDR_H;
			pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum =
				1;
			pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr =
				LCG_ADDR_L;
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
	}

	if (pstSnsState->au32FL[1] < pstSnsState->au32FL[0]) {
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
	} else {
		pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1;
		pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1;
	}

	pstSnsRegsInfo->bConfig = GK_FALSE;
	memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0],
	       sizeof(ISP_SNS_REGS_INFO_S));
	memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0],
	       sizeof(ISP_SNS_REGS_INFO_S));

	pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

	return GK_SUCCESS;
}

#define OV2718_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                    \
	do {                                                                      \
		ISP_TRACE(                                                        \
			MODULE_DBG_ERR,                                           \
			"Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", \
			pstSensorImageMode->u16Width,                             \
			pstSensorImageMode->u16Height,                            \
			pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);      \
	} while (0)

static GK_S32
cmos_set_image_mode(VI_PIPE ViPipe,
		    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
	GK_U8 u8SensorImageMode;
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	CMOS_CHECK_POINTER(pstSensorImageMode);
	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER(pstSnsState);

	u8SensorImageMode = pstSnsState->u8ImgMode;
	pstSnsState->bSyncInit = GK_FALSE;

	if (pstSensorImageMode->f32Fps <= 30) {
		if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
			if ((pstSensorImageMode->u16Width <= 1920) &&
			    (pstSensorImageMode->u16Height <= 1080)) {
				u8SensorImageMode =
					OV2718_SENSOR_1080P_30FPS_WDR_2To1_MODE;
				pstSnsState->u32FLStd = VMAX_1080P60TO30_WDR;
			} else {
				OV2718_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else if (pstSnsState->enWDRMode == WDR_MODE_BUILT_IN) {
			if ((pstSensorImageMode->u16Width <= 1920) &&
			    (pstSensorImageMode->u16Height <= 1080)) {
				u8SensorImageMode =
					OV2718_SENSOR_1080P_30FPS_BUILT_IN_MODE;
				pstSnsState->u32FLStd = VMAX_1080P30_LINEAR;
			} else {
				OV2718_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
			if ((pstSensorImageMode->u16Width <= 1920) &&
			    (pstSensorImageMode->u16Height <= 1080)) {
				u8SensorImageMode =
					OV2718_SENSOR_1080P_30FPS_LINEAR_MODE;
				pstSnsState->u32FLStd = VMAX_1080P30_LINEAR;
			} else {
				OV2718_ERR_MODE_PRINT(pstSensorImageMode,
						      pstSnsState);
				return GK_FAILURE;
			}
		} else {
			OV2718_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
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

GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pstSnsState = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pstSnsState);
	CMOS_CHECK_POINTER_VOID(pstSnsState);

	pstSnsState->bInit = GK_FALSE;
	pstSnsState->bSyncInit = GK_FALSE;
	pstSnsState->u8ImgMode = OV2718_SENSOR_1080P_30FPS_LINEAR_MODE;
	pstSnsState->enWDRMode = WDR_MODE_NONE;
	pstSnsState->u32FLStd = VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[0] = VMAX_1080P30_LINEAR;
	pstSnsState->au32FL[1] = VMAX_1080P30_LINEAR;

	memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	CMOS_CHECK_POINTER(pstSensorExpFunc);

	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = ov2718_init;
	pstSensorExpFunc->pfn_cmos_sensor_exit = ov2718_exit;
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

static GK_S32 ov2718_set_bus_info(VI_PIPE ViPipe,
				  ISP_SNS_COMMBUS_U unSNSBusInfo)
{
	g_aunOv2718BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

	return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

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

	OV2718_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

	return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
	ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

	OV2718_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
	SENSOR_FREE(pastSnsStateCtx);
	OV2718_SENSOR_RESET_CTX(ViPipe);
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

	stSnsAttrInfo.eSensorId = OV2718_ID;

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

	s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, OV2718_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sensor unregister callback function failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, OV2718_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, OV2718_ID);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(
			MODULE_DBG_ERR,
			"sensor unregister callback function to awb lib failed!\n");
		return s32Ret;
	}

	sensor_ctx_exit(ViPipe);

	return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsOV2718Obj = {
	.pfnRegisterCallback = sensor_register_callback,
	.pfnUnRegisterCallback = sensor_unregister_callback,
	.pfnStandby = ov2718_standby,
	.pfnRestart = ov2718_restart,
	.pfnMirrorFlip = GK_NULL,
	.pfnWriteReg = ov2718_write_register,
	.pfnReadReg = ov2718_read_register,
	.pfnSetBusInfo = ov2718_set_bus_info
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
