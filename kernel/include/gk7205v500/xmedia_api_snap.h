/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_API_SNAP_H__
#define __XMEDIA_API_SNAP_H__

#include "comm_video.h"
#include "comm_snap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

XMEDIA_S32 XMEDIA_API_SNAP_SetPipeAttr(VI_PIPE ViPipe, const SNAP_ATTR_S *pstSnapAttr);
XMEDIA_S32 XMEDIA_API_SNAP_GetPipeAttr(VI_PIPE ViPipe, SNAP_ATTR_S *pstSnapAttr);
XMEDIA_S32 XMEDIA_API_SNAP_EnablePipe(VI_PIPE ViPipe);
XMEDIA_S32 XMEDIA_API_SNAP_DisablePipe(VI_PIPE ViPipe);
XMEDIA_S32 XMEDIA_API_SNAP_TriggerPipe(VI_PIPE ViPipe);

XMEDIA_S32 XMEDIA_API_SNAP_MultiTrigger(VI_STITCH_GRP StitchGrp);

XMEDIA_S32 XMEDIA_API_SNAP_SetProSharpenParam(VI_PIPE ViPipe, const ISP_PRO_SHARPEN_PARAM_S *pstIspShpParam);
XMEDIA_S32 XMEDIA_API_SNAP_GetProSharpenParam(VI_PIPE ViPipe, ISP_PRO_SHARPEN_PARAM_S *pstIspShpParam);
XMEDIA_S32 XMEDIA_API_SNAP_SetProBNRParam(VI_PIPE ViPipe, const ISP_PRO_BNR_PARAM_S *pstNrParma);
XMEDIA_S32 XMEDIA_API_SNAP_GetProBNRParam(VI_PIPE ViPipe, ISP_PRO_BNR_PARAM_S *pstNrParma);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


