/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_DCFINFO_H__
#define __ISP_DCFINFO_H__

#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_UPDATE_INFO_CTRL_S {
	ISP_DCF_CONST_INFO_S *pstIspDCFConstInfo;
	ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo;
} ISP_UPDATE_INFO_CTRL_S;

GK_S32 ISP_UpdateInfoInit(VI_PIPE ViPipe);
GK_S32 ISP_UpdateInfoExit(VI_PIPE ViPipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
