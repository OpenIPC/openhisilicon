/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_sensor.h"
#include "yuv_cmos_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_SENSOR_S {
	ISP_SNS_ATTR_INFO_S stSnsAttrInfo;
	ISP_SENSOR_REGISTER_S stRegister;
	ISP_CMOS_DEFAULT_S stSnsDft;
	ISP_CMOS_BLACK_LEVEL_S
		stSnsBlackLevel; /* some sensors's black level will be changed with iso */
	ISP_SNS_REGS_INFO_S stSnsRegInfo;
	ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
} ISP_SENSOR_S;

ISP_SENSOR_S *g_pastSensorCtx[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSensorCtx[dev])
#define SENSOR_SET_CTX(dev, pstCtx) (g_pastSensorCtx[dev] = pstCtx)
#define SENSOR_RESET_CTX(dev) (g_pastSensorCtx[dev] = GK_NULL)

GK_S32 ISP_SensorCtxInit(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensorCtx = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensorCtx);

	if (pstSensorCtx == GK_NULL) {
		pstSensorCtx = (ISP_SENSOR_S *)ISP_MALLOC(sizeof(ISP_SENSOR_S));
		if (pstSensorCtx == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] SensorCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pstSensorCtx, 0, sizeof(ISP_SENSOR_S));

	SENSOR_SET_CTX(ViPipe, pstSensorCtx);

	return GK_SUCCESS;
}

GK_S32 ISP_SensorRegCallBack(VI_PIPE ViPipe,
			     ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
			     ISP_SENSOR_REGISTER_S *pstRegister)
{
	GK_S32 s32Ret = GK_SUCCESS;
	ISP_SENSOR_S *pstSensor = GK_NULL;

	s32Ret = ISP_SensorCtxInit(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	memcpy(&pstSensor->stSnsAttrInfo, pstSnsAttrInfo,
	       sizeof(ISP_SNS_ATTR_INFO_S));
	memcpy(&pstSensor->stRegister, pstRegister,
	       sizeof(ISP_SENSOR_REGISTER_S));

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_global_init !=
	    GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_global_init(
			ViPipe);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorUnRegCallBack(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_FREE(pstSensor);
	SENSOR_RESET_CTX(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_SensorUpdateAll(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default !=
	    GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default(
			ViPipe, &pstSensor->stSnsDft);
	} else {
		ISP_TRACE(MODULE_DBG_ERR, "Get isp[%d] default value error!\n",
			  ViPipe);
		return GK_FAILURE;
	}

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level !=
	    GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level(
			ViPipe, &pstSensor->stSnsBlackLevel);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorUpdateAll_YUV(VI_PIPE ViPipe)
{
	GK_S32 s32Ret = GK_SUCCESS;
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	s32Ret = ISP_GetYUVDefault(&pstSensor->stSnsDft);

	pstSensor->stSnsBlackLevel.au16BlackLevel[0] = 257;
	pstSensor->stSnsBlackLevel.au16BlackLevel[1] = 257;
	pstSensor->stSnsBlackLevel.au16BlackLevel[2] = 257;
	pstSensor->stSnsBlackLevel.au16BlackLevel[3] = 257;
	pstSensor->stSnsBlackLevel.bUpdate = GK_TRUE;

	return s32Ret;
}

GK_S32 ISP_SensorGetId(VI_PIPE ViPipe, SENSOR_ID *pSensorId)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	*pSensorId = pstSensor->stSnsAttrInfo.eSensorId;

	return GK_SUCCESS;
}

GK_S32 ISP_SensorGetBlc(VI_PIPE ViPipe,
			ISP_CMOS_BLACK_LEVEL_S **ppstSnsBlackLevel)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	ISP_CHECK_PIPE(ViPipe);
	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	*ppstSnsBlackLevel = &pstSensor->stSnsBlackLevel;

	return GK_SUCCESS;
}

GK_S32 ISP_SensorGetDefault(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S **ppstSnsDft)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	ISP_CHECK_PIPE(ViPipe);
	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	*ppstSnsDft = &pstSensor->stSnsDft;

	return GK_SUCCESS;
}

GK_S32 ISP_SensorGetMaxResolution(
	VI_PIPE ViPipe, ISP_CMOS_SENSOR_MAX_RESOLUTION_S **ppstSnsMaxResolution)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	*ppstSnsMaxResolution = &pstSensor->stSnsDft.stSensorMaxResolution;

	return GK_SUCCESS;
}

GK_S32 ISP_SensorGetSnsReg(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S **ppstSnsRegInfo)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	*ppstSnsRegInfo = &pstSensor->stSnsRegInfo;

	return GK_SUCCESS;
}

GK_S32 ISP_SensorInit(VI_PIPE ViPipe)
{
	GK_S8 s8SspDev;
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	/* if I2C or SSP Dev is -1, don't init sensor */
	{
		ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

		ISP_SensorUpdateSnsReg(ViPipe);
		ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
		s8SspDev = pstSnsRegsInfo->unComBus.s8SspDev.bit4SspDev;

		if ((pstSnsRegsInfo->enSnsType == ISP_SNS_I2C_TYPE) &&
		    (pstSnsRegsInfo->unComBus.s8I2cDev == -1)) {
			return GK_SUCCESS;
		}

		if ((pstSnsRegsInfo->enSnsType == ISP_SNS_SSP_TYPE) &&
		    (s8SspDev == -1)) {
			return GK_SUCCESS;
		}
	}

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_init != GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_init(ViPipe);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorSwitch(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;
	GK_S8 s8SspDev;
	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);
	/* if I2C or SSP Dev is -1, don't init sensor */
	{
		ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

		ISP_SensorUpdateSnsReg(ViPipe);
		ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
		s8SspDev = pstSnsRegsInfo->unComBus.s8SspDev.bit4SspDev;

		if ((pstSnsRegsInfo->enSnsType == ISP_SNS_I2C_TYPE) &&
		    (pstSnsRegsInfo->unComBus.s8I2cDev == -1)) {
			return GK_SUCCESS;
		}

		if ((pstSnsRegsInfo->enSnsType == ISP_SNS_SSP_TYPE) &&
		    (s8SspDev == -1)) {
			return GK_SUCCESS;
		}
	}

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_init != GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_init(ViPipe);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorExit(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_exit != GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_exit(ViPipe);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorUpdateBlc(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level !=
	    GK_NULL) {
		/* sensor should record the present iso, and calculate new black level. */
		pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level(
			ViPipe, &pstSensor->stSnsBlackLevel);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorUpdateDefault(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default !=
	    GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default(
			ViPipe, &pstSensor->stSnsDft);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorSetWDRMode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_set_wdr_mode != GK_NULL) {
		if (pstSensor->stRegister.stSnsExp.pfn_cmos_set_wdr_mode(
			    ViPipe, u8Mode) != GK_SUCCESS) {
			return GK_FAILURE;
		}
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorSetImageMode(VI_PIPE ViPipe,
			      ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSnsImageMode)
{
	ISP_SENSOR_S *pstSensor = NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_set_image_mode != GK_NULL) {
		return pstSensor->stRegister.stSnsExp.pfn_cmos_set_image_mode(
			ViPipe, pstSnsImageMode);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorSetPixelDetect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_set_pixel_detect !=
	    GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_set_pixel_detect(
			ViPipe, bEnable);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SensorUpdateSnsReg(VI_PIPE ViPipe)
{
	ISP_SENSOR_S *pstSensor = GK_NULL;

	SENSOR_GET_CTX(ViPipe, pstSensor);
	ISP_CHECK_POINTER(pstSensor);

	if (pstSensor->stRegister.stSnsExp.pfn_cmos_get_sns_reg_info !=
	    GK_NULL) {
		pstSensor->stRegister.stSnsExp.pfn_cmos_get_sns_reg_info(
			ViPipe, &pstSensor->stSnsRegInfo);
	} else {
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
