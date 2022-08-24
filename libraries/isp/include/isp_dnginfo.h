/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_DNGINFO_H__
#define __ISP_DNGINFO_H__

#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_DNG_INFO_CTRL_S {
	DNG_IMAGE_STATIC_INFO_S *pstIspDng;
} ISP_DNG_INFO_CTRL_S;

typedef struct DNG_MATRIX_S {
	GK_DOUBLE adAColorMatrix[9];
	GK_DOUBLE adD50ColorMatrix[9];
	GK_DOUBLE adAForwardMatrix[9];
	GK_DOUBLE adD50ForwardMatrix[9];
} DNG_MATRIX_S;

GK_S32 ISP_UpdateDngImageDynamicInfo(VI_PIPE ViPipe);

GK_S32 ISP_DngInfoInit(VI_PIPE ViPipe);
GK_S32 ISP_DngInfoExit(VI_PIPE ViPipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
