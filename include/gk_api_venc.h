/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __GK_API_VENC_H__
#define __GK_API_VENC_H__

#include "common.h"
#include "comm_video.h"
#include "comm_venc.h"
#include "comm_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_VENC_CreateChn(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr);
GK_S32 GK_API_VENC_DestroyChn(VENC_CHN VeChn);

GK_S32 GK_API_VENC_ResetChn(VENC_CHN VeChn);

GK_S32 GK_API_VENC_StartRecvFrame(VENC_CHN VeChn, const VENC_RECV_PIC_PARAM_S *pstRecvParam);
GK_S32 GK_API_VENC_StopRecvFrame(VENC_CHN VeChn);

GK_S32 GK_API_VENC_QueryStatus(VENC_CHN VeChn, VENC_CHN_STATUS_S *pstStatus);

GK_S32 GK_API_VENC_SetChnAttr(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstChnAttr);
GK_S32 GK_API_VENC_GetChnAttr(VENC_CHN VeChn, VENC_CHN_ATTR_S *pstChnAttr);

GK_S32 GK_API_VENC_GetStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream, GK_S32 s32MilliSec);

GK_S32 GK_API_VENC_ReleaseStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream);

GK_S32 GK_API_VENC_InsertUserData(VENC_CHN VeChn, GK_U8 *pu8Data, GK_U32 u32Len);

GK_S32 GK_API_VENC_SendFrame(VENC_CHN VeChn, const VIDEO_FRAME_INFO_S *pstFrame, GK_S32 s32MilliSec);
GK_S32 GK_API_VENC_SendFrameEx(VENC_CHN VeChn, const USER_FRAME_INFO_S *pstFrame, GK_S32 s32MilliSec);

GK_S32 GK_API_VENC_RequestIDR(VENC_CHN VeChn, GK_BOOL bInstant);

GK_S32 GK_API_VENC_GetFd(VENC_CHN VeChn);
GK_S32 GK_API_VENC_CloseFd(VENC_CHN VeChn);

GK_S32 GK_API_VENC_SetRoiAttr(VENC_CHN VeChn, const VENC_ROI_ATTR_S *pstRoiAttr);
GK_S32 GK_API_VENC_GetRoiAttr(VENC_CHN VeChn, GK_U32 u32Index, VENC_ROI_ATTR_S *pstRoiAttr);

GK_S32 GK_API_VENC_GetRoiAttrEx(VENC_CHN VeChn, GK_U32 u32Index, VENC_ROI_ATTR_EX_S *pstRoiAttrEx);
GK_S32 GK_API_VENC_SetRoiAttrEx(VENC_CHN VeChn, const VENC_ROI_ATTR_EX_S *pstRoiAttrEx);

GK_S32 GK_API_VENC_SetRoiBgFrameRate(VENC_CHN VeChn, const VENC_ROIBG_FRAME_RATE_S *pstRoiBgFrmRate);
GK_S32 GK_API_VENC_GetRoiBgFrameRate(VENC_CHN VeChn, VENC_ROIBG_FRAME_RATE_S *pstRoiBgFrmRate);

GK_S32 GK_API_VENC_SetH264SliceSplit(VENC_CHN VeChn, const VENC_H264_SLICE_SPLIT_S *pstSliceSplit);
GK_S32 GK_API_VENC_GetH264SliceSplit(VENC_CHN VeChn, VENC_H264_SLICE_SPLIT_S *pstSliceSplit);

GK_S32 GK_API_VENC_SetH264IntraPred(VENC_CHN VeChn, const VENC_H264_INTRA_PRED_S *pstH264IntraPred);
GK_S32 GK_API_VENC_GetH264IntraPred(VENC_CHN VeChn, VENC_H264_INTRA_PRED_S *pstH264IntraPred);

GK_S32 GK_API_VENC_SetH264Trans(VENC_CHN VeChn, const VENC_H264_TRANS_S *pstH264Trans);
GK_S32 GK_API_VENC_GetH264Trans(VENC_CHN VeChn, VENC_H264_TRANS_S *pstH264Trans);

GK_S32 GK_API_VENC_SetH264Entropy(VENC_CHN VeChn, const VENC_H264_ENTROPY_S *pstH264EntropyEnc);
GK_S32 GK_API_VENC_GetH264Entropy(VENC_CHN VeChn, VENC_H264_ENTROPY_S *pstH264EntropyEnc);

GK_S32 GK_API_VENC_SetH264Dblk(VENC_CHN VeChn, const VENC_H264_DBLK_S *pstH264Dblk);
GK_S32 GK_API_VENC_GetH264Dblk(VENC_CHN VeChn, VENC_H264_DBLK_S *pstH264Dblk);

GK_S32 GK_API_VENC_SetH264Vui(VENC_CHN VeChn, const VENC_H264_VUI_S *pstH264Vui);
GK_S32 GK_API_VENC_GetH264Vui(VENC_CHN VeChn, VENC_H264_VUI_S *pstH264Vui);

GK_S32 GK_API_VENC_SetH265Vui(VENC_CHN VeChn, const VENC_H265_VUI_S *pstH265Vui);
GK_S32 GK_API_VENC_GetH265Vui(VENC_CHN VeChn, VENC_H265_VUI_S *pstH265Vui);

GK_S32 GK_API_VENC_SetJpegParam(VENC_CHN VeChn, const VENC_JPEG_PARAM_S *pstJpegParam);
GK_S32 GK_API_VENC_GetJpegParam(VENC_CHN VeChn, VENC_JPEG_PARAM_S *pstJpegParam);

GK_S32 GK_API_VENC_SetMjpegParam(VENC_CHN VeChn, const VENC_MJPEG_PARAM_S *pstMjpegParam);
GK_S32 GK_API_VENC_GetMjpegParam(VENC_CHN VeChn, VENC_MJPEG_PARAM_S *pstMjpegParam);

GK_S32 GK_API_VENC_GetRcParam(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcParam);
GK_S32 GK_API_VENC_SetRcParam(VENC_CHN VeChn, const VENC_RC_PARAM_S *pstRcParam);

GK_S32 GK_API_VENC_SetRefParam(VENC_CHN VeChn, const VENC_REF_PARAM_S *pstRefParam);
GK_S32 GK_API_VENC_GetRefParam(VENC_CHN VeChn, VENC_REF_PARAM_S *pstRefParam);

GK_S32 GK_API_VENC_SetJpegEncodeMode(VENC_CHN VeChn, const VENC_JPEG_ENCODE_MODE_E enJpegEncodeMode);
GK_S32 GK_API_VENC_GetJpegEncodeMode(VENC_CHN VeChn, VENC_JPEG_ENCODE_MODE_E *penJpegEncodeMode);

GK_S32 GK_API_VENC_EnableIDR(VENC_CHN VeChn, GK_BOOL bEnableIDR);

GK_S32 GK_API_VENC_GetStreamBufInfo(VENC_CHN VeChn, VENC_STREAM_BUF_INFO_S *pstStreamBufInfo);

GK_S32 GK_API_VENC_SetH265SliceSplit(VENC_CHN VeChn, const VENC_H265_SLICE_SPLIT_S *pstSliceSplit);
GK_S32 GK_API_VENC_GetH265SliceSplit(VENC_CHN VeChn, VENC_H265_SLICE_SPLIT_S *pstSliceSplit);

GK_S32 GK_API_VENC_SetH265PredUnit(VENC_CHN VeChn, const VENC_H265_PU_S *pstPredUnit);
GK_S32 GK_API_VENC_GetH265PredUnit(VENC_CHN VeChn, VENC_H265_PU_S *pstPredUnit);

GK_S32 GK_API_VENC_SetH265Trans(VENC_CHN VeChn, const VENC_H265_TRANS_S *pstH265Trans);
GK_S32 GK_API_VENC_GetH265Trans(VENC_CHN VeChn, VENC_H265_TRANS_S *pstH265Trans);

GK_S32 GK_API_VENC_SetH265Entropy(VENC_CHN VeChn, const VENC_H265_ENTROPY_S *pstH265Entropy);
GK_S32 GK_API_VENC_GetH265Entropy(VENC_CHN VeChn, VENC_H265_ENTROPY_S *pstH265Entropy);

GK_S32 GK_API_VENC_SetH265Dblk(VENC_CHN VeChn, const VENC_H265_DBLK_S *pstH265Dblk);
GK_S32 GK_API_VENC_GetH265Dblk(VENC_CHN VeChn, VENC_H265_DBLK_S *pstH265Dblk);

GK_S32 GK_API_VENC_SetH265Sao(VENC_CHN VeChn, const VENC_H265_SAO_S *pstH265Sao);
GK_S32 GK_API_VENC_GetH265Sao(VENC_CHN VeChn, VENC_H265_SAO_S *pstH265Sao);

GK_S32 GK_API_VENC_SetFrameLostStrategy(VENC_CHN VeChn, const VENC_FRAMELOST_S *pstFrmLostParam);
GK_S32 GK_API_VENC_GetFrameLostStrategy(VENC_CHN VeChn, VENC_FRAMELOST_S *pstFrmLostParam);

GK_S32 GK_API_VENC_SetSuperFrameStrategy(VENC_CHN VeChn, const VENC_SUPERFRAME_CFG_S *pstSuperFrmParam);
GK_S32 GK_API_VENC_GetSuperFrameStrategy(VENC_CHN VeChn, VENC_SUPERFRAME_CFG_S *pstSuperFrmParam);

GK_S32 GK_API_VENC_SetIntraRefresh(VENC_CHN VeChn, const VENC_INTRA_REFRESH_S *pstIntraRefresh);
GK_S32 GK_API_VENC_GetIntraRefresh(VENC_CHN VeChn, VENC_INTRA_REFRESH_S *pstIntraRefresh);

GK_S32 GK_API_VENC_GetSSERegion(VENC_CHN VeChn, GK_U32 u32Index, VENC_SSE_CFG_S *pstSSECfg);
GK_S32 GK_API_VENC_SetSSERegion(VENC_CHN VeChn, const VENC_SSE_CFG_S *pstSSECfg);

GK_S32 GK_API_VENC_SetChnParam(VENC_CHN VeChn, const VENC_CHN_PARAM_S *pstChnParam);
GK_S32 GK_API_VENC_GetChnParam(VENC_CHN VeChn, VENC_CHN_PARAM_S *pstChnParam);

GK_S32 GK_API_VENC_SetModParam(const VENC_PARAM_MOD_S *pstModParam);
GK_S32 GK_API_VENC_GetModParam(VENC_PARAM_MOD_S *pstModParam);

GK_S32 GK_API_VENC_GetForegroundProtect(VENC_CHN VeChn, VENC_FOREGROUND_PROTECT_S *pstForegroundProtect);
GK_S32 GK_API_VENC_SetForegroundProtect(VENC_CHN VeChn, const VENC_FOREGROUND_PROTECT_S *pstForegroundProtect);

GK_S32 GK_API_VENC_SetSceneMode(VENC_CHN VeChn, const VENC_SCENE_MODE_E enSceneMode);
GK_S32 GK_API_VENC_GetSceneMode(VENC_CHN VeChn, VENC_SCENE_MODE_E *penSceneMode);

GK_S32 GK_API_VENC_AttachVbPool(VENC_CHN VeChn, const VENC_CHN_POOL_S *pstPool);
GK_S32 GK_API_VENC_DetachVbPool(VENC_CHN VeChn);

GK_S32 GK_API_VENC_SetCuPrediction(VENC_CHN VeChn, const VENC_CU_PREDICTION_S *pstCuPrediction);
GK_S32 GK_API_VENC_GetCuPrediction(VENC_CHN VeChn, VENC_CU_PREDICTION_S *pstCuPrediction);

GK_S32 GK_API_VENC_SetSkipBias(VENC_CHN VeChn, const VENC_SKIP_BIAS_S *pstSkipBias);
GK_S32 GK_API_VENC_GetSkipBias(VENC_CHN VeChn, VENC_SKIP_BIAS_S *pstSkipBias);

GK_S32 GK_API_VENC_SetDeBreathEffect(VENC_CHN VeChn, const VENC_DEBREATHEFFECT_S *pstDeBreathEffect);
GK_S32 GK_API_VENC_GetDeBreathEffect(VENC_CHN VeChn, VENC_DEBREATHEFFECT_S *pstDeBreathEffect);

GK_S32 GK_API_VENC_SetHierarchicalQp(VENC_CHN VeChn, const VENC_HIERARCHICAL_QP_S *pstHierarchicalQp);
GK_S32 GK_API_VENC_GetHierarchicalQp(VENC_CHN VeChn, VENC_HIERARCHICAL_QP_S *pstHierarchicalQp);

GK_S32 GK_API_VENC_SetRcAdvParam(VENC_CHN VeChn, const VENC_RC_ADVPARAM_S *pstRcAdvParam);
GK_S32 GK_API_VENC_GetRcAdvParam(VENC_CHN VeChn, VENC_RC_ADVPARAM_S *pstRcAdvParam);

GK_S32 GK_API_VENC_EnableSvc(VENC_CHN VeChn, GK_BOOL bEnable);
GK_S32 GK_API_VENC_SetSvcParam(VENC_CHN VeChn, const VENC_SVC_PARAM_S *pstSvcParam);
GK_S32 GK_API_VENC_GetSvcParam(VENC_CHN VeChn, VENC_SVC_PARAM_S *pstSvcParam);
GK_S32 GK_API_VENC_GetSvcSceneComplexity(VENC_CHN VeChn, VENC_SVC_DETECT_RESULT_S *pstSvcDetectResult);
GK_S32 GK_API_VENC_SendSvcRegion(VENC_CHN VeChn, VENC_SVC_RECT_INFO_S *pstSvcRegion);

GK_S32 GK_API_VENC_SetSearchWindow(VENC_CHN VeChn, const VENC_SEARCH_WINDOW_S *pstSearchWindow);
GK_S32 GK_API_VENC_GetSearchWindow(VENC_CHN VeChn, VENC_SEARCH_WINDOW_S *pstSearchWindow);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

