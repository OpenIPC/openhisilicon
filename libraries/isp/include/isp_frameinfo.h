/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_FRAMEINFO_H__
#define __ISP_FRAMEINFO_H__

#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_FRAME_INFO_CTRL_S {
    ISP_FRAME_INFO_S *pstIspFrame;
} ISP_FRAME_INFO_CTRL_S;

typedef struct ISP_ATTACH_INFO_CTRL_S {
    ISP_ATTACH_INFO_S *pstAttachInfo;
} ISP_ATTACH_INFO_CTRL_S;

typedef struct ISP_GAMMUT_INFO_CTRL_S {
    ISP_COLORGAMMUT_INFO_S *pstColorGamutInfo;
} ISP_GAMMUT_INFO_CTRL_S;

typedef struct ISP_PRO_NR_PARAM_CTRL_S {
    ISP_PRO_NR_PARAM_S *pstProNrParam;
} ISP_PRO_NR_PARAM_CTRL_S;

typedef struct ISP_PRO_SHP_PARAM_CTRL_S {
    ISP_PRO_SHP_PARAM_S *pstProShpParam;
} ISP_PRO_SHP_PARAM_CTRL_S;

GK_S32 ISP_TransInfoInit(VI_PIPE ViPipe, ISP_TRANS_INFO_BUF_S *pstTransInfo);
GK_S32 ISP_TransInfoExit(VI_PIPE ViPipe);

GK_S32 ISP_FrameInfoInit(VI_PIPE ViPipe);
GK_S32 ISP_FrameInfoExit(VI_PIPE ViPipe);

GK_S32 ISP_AttachInfoInit(VI_PIPE ViPipe);
GK_S32 ISP_AttachInfoExit(VI_PIPE ViPipe);

GK_S32 ISP_ColorGamutInfoInit(VI_PIPE ViPipe);
GK_S32 ISP_ColorGamutInfoExit(VI_PIPE ViPipe);

GK_S32 ISP_ProInfoInit(VI_PIPE ViPipe, ISP_PRO_INFO_BUF_S *pstProInfo);
GK_S32 ISP_ProInfoExit(VI_PIPE ViPipe);

GK_S32 ISP_ProNrParamInit(VI_PIPE ViPipe);
GK_S32 ISP_ProNrParamExit(VI_PIPE ViPipe);

GK_S32 ISP_ProShpParamInit(VI_PIPE ViPipe);
GK_S32 ISP_ProShpParamExit(VI_PIPE ViPipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
