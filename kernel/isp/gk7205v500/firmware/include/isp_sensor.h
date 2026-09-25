/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __ISP_SENSOR_H__
#define __ISP_SENSOR_H__

#include "mkp_isp.h"
#include "comm_sns.h"
#include "comm_isp.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

XMEDIA_S32 ISP_SensorRegCallBack(VI_PIPE ViPipe, ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
                             ISP_SENSOR_REGISTER_S *pstRegister);
XMEDIA_S32 ISP_SensorUnRegCallBack(VI_PIPE ViPipe);

XMEDIA_S32 ISP_SensorUpdateAll(VI_PIPE ViPipe);

XMEDIA_S32 ISP_SensorGetId(VI_PIPE ViPipe, SENSOR_ID *pSensorId);
XMEDIA_S32 ISP_SensorGetBlc(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S **ppstSnsBlackLevel);
XMEDIA_S32 ISP_SensorGetDefault(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S **ppstSnsDft);
XMEDIA_S32 ISP_SensorGetSnsReg(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S **ppstSnsRegInfo);
XMEDIA_S32 ISP_SensorGetMaxResolution(VI_PIPE ViPipe, ISP_CMOS_SENSOR_MAX_RESOLUTION_S **ppstSnsMaxResolution);

XMEDIA_S32 ISP_SensorCtxInit(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorUpdateAll_YUV(VI_PIPE ViPipe);
XMEDIA_S32 ISP_GetYUVDefault(ISP_CMOS_DEFAULT_S *pstSnsDft);
XMEDIA_S32 ISP_SensorInit(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorSwitch(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorExit(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorUpdateBlc(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorUpdateDefault(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorSetWDRMode(VI_PIPE ViPipe, XMEDIA_U8 u8Mode);
XMEDIA_S32 ISP_SensorSetResolutionMode(VI_PIPE ViPipe, XMEDIA_U32 u32Mode);
XMEDIA_S32 ISP_SensorSetPixelDetect(VI_PIPE ViPipe, XMEDIA_BOOL bEnable);
XMEDIA_S32 ISP_SensorUpdateSnsReg(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SensorSetImageMode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSnsImageMode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
