/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_API_VPSS_H__
#define __GK_API_VPSS_H__

#include "common.h"
#include "comm_video.h"
#include "comm_vb.h"
#include "comm_vpss.h"
#include "comm_gdc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

GK_S32 GK_API_VPSS_CreateGrp(VPSS_GRP VpssGrp,
			     const VPSS_GRP_ATTR_S *pstGrpAttr);
GK_S32 GK_API_VPSS_DestroyGrp(VPSS_GRP VpssGrp);

GK_S32 GK_API_VPSS_StartGrp(VPSS_GRP VpssGrp);
GK_S32 GK_API_VPSS_StopGrp(VPSS_GRP VpssGrp);

GK_S32 GK_API_VPSS_ResetGrp(VPSS_GRP VpssGrp);

GK_S32 GK_API_VPSS_GetGrpAttr(VPSS_GRP VpssGrp, VPSS_GRP_ATTR_S *pstGrpAttr);
GK_S32 GK_API_VPSS_SetGrpAttr(VPSS_GRP VpssGrp,
			      const VPSS_GRP_ATTR_S *pstGrpAttr);

GK_S32 GK_API_VPSS_SetGrpCrop(VPSS_GRP VpssGrp,
			      const VPSS_CROP_INFO_S *pstCropInfo);
GK_S32 GK_API_VPSS_GetGrpCrop(VPSS_GRP VpssGrp, VPSS_CROP_INFO_S *pstCropInfo);

GK_S32 GK_API_VPSS_SendFrame(VPSS_GRP VpssGrp, VPSS_GRP_PIPE VpssGrpPipe,
			     const VIDEO_FRAME_INFO_S *pstVideoFrame,
			     GK_S32 s32MilliSec);

GK_S32 GK_API_VPSS_GetGrpFrame(VPSS_GRP VpssGrp, VPSS_GRP_PIPE VpssGrpPipe,
			       VIDEO_FRAME_INFO_S *pstVideoFrame);
GK_S32 GK_API_VPSS_ReleaseGrpFrame(VPSS_GRP VpssGrp, VPSS_GRP_PIPE VpssGrpPipe,
				   const VIDEO_FRAME_INFO_S *pstVideoFrame);

GK_S32 GK_API_VPSS_EnableBackupFrame(VPSS_GRP VpssGrp);
GK_S32 GK_API_VPSS_DisableBackupFrame(VPSS_GRP VpssGrp);

GK_S32
GK_API_VPSS_SetGrpSharpen(VPSS_GRP VpssGrp,
			  const VPSS_GRP_SHARPEN_ATTR_S *pstGrpSharpenAttr);
GK_S32 GK_API_VPSS_GetGrpSharpen(VPSS_GRP VpssGrp,
				 VPSS_GRP_SHARPEN_ATTR_S *pstGrpSharpenAttr);

GK_S32 GK_API_VPSS_SetGrpDelay(VPSS_GRP VpssGrp, GK_U32 u32Delay);
GK_S32 GK_API_VPSS_GetGrpDelay(VPSS_GRP VpssGrp, GK_U32 *pu32Delay);

GK_S32
GK_API_VPSS_SetGrpFisheyeConfig(VPSS_GRP VpssGrp,
				const FISHEYE_CONFIG_S *pstFisheyeConfig);
GK_S32 GK_API_VPSS_GetGrpFisheyeConfig(VPSS_GRP VpssGrp,
				       FISHEYE_CONFIG_S *pstFisheyeConfig);

GK_S32 GK_API_VPSS_EnableUserFrameRateCtrl(VPSS_GRP VpssGrp);
GK_S32 GK_API_VPSS_DisableUserFrameRateCtrl(VPSS_GRP VpssGrp);

GK_S32 GK_API_VPSS_SetGrpFrameInterruptAttr(
	VPSS_GRP VpssGrp, const FRAME_INTERRUPT_ATTR_S *pstFrameIntAttr);
GK_S32
GK_API_VPSS_GetGrpFrameInterruptAttr(VPSS_GRP VpssGrp,
				     FRAME_INTERRUPT_ATTR_S *pstFrameIntAttr);

GK_S32 GK_API_VPSS_SetChnAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			      const VPSS_CHN_ATTR_S *pstChnAttr);
GK_S32 GK_API_VPSS_GetChnAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			      VPSS_CHN_ATTR_S *pstChnAttr);

GK_S32 GK_API_VPSS_EnableChn(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
GK_S32 GK_API_VPSS_DisableChn(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);

GK_S32 GK_API_VPSS_SetChnCrop(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			      const VPSS_CROP_INFO_S *pstCropInfo);
GK_S32 GK_API_VPSS_GetChnCrop(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			      VPSS_CROP_INFO_S *pstCropInfo);

GK_S32 GK_API_VPSS_SetChnRotation(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				  ROTATION_E enRotation);
GK_S32 GK_API_VPSS_GetChnRotation(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				  ROTATION_E *penRotation);

GK_S32
GK_API_VPSS_SetChnRotationEx(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			     const VPSS_ROTATION_EX_ATTR_S *pstRotationExAttr);
GK_S32 GK_API_VPSS_GetChnRotationEx(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				    VPSS_ROTATION_EX_ATTR_S *pstRotationExAttr);

GK_S32 GK_API_VPSS_SetChnLDCAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				 const VPSS_LDC_ATTR_S *pstLDCAttr);
GK_S32 GK_API_VPSS_GetChnLDCAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				 VPSS_LDC_ATTR_S *pstLDCAttr);

GK_S32 GK_API_VPSS_SetChnLDCV3Attr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				   const VPSS_LDCV3_ATTR_S *pstLDCV3Attr);
GK_S32 GK_API_VPSS_GetChnLDCV3Attr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				   VPSS_LDCV3_ATTR_S *pstLDCV3Attr);

GK_S32 GK_API_VPSS_SetChnSpreadAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				    const SPREAD_ATTR_S *pstSpreadAttr);
GK_S32 GK_API_VPSS_GetChnSpreadAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				    SPREAD_ATTR_S *pstSpreadAttr);

GK_S32 GK_API_VPSS_GetChnFrame(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			       VIDEO_FRAME_INFO_S *pstVideoFrame,
			       GK_S32 s32MilliSec);
GK_S32 GK_API_VPSS_ReleaseChnFrame(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				   const VIDEO_FRAME_INFO_S *pstVideoFrame);

GK_S32 GK_API_VPSS_GetRegionLuma(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				 const VIDEO_REGION_INFO_S *pstRegionInfo,
				 GK_U64 *pu64LumaData, GK_S32 s32MilliSec);

GK_S32
GK_API_VPSS_SetLowDelayAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			    const VPSS_LOW_DELAY_INFO_S *pstLowDelayInfo);
GK_S32 GK_API_VPSS_GetLowDelayAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				   VPSS_LOW_DELAY_INFO_S *pstLowDelayInfo);

GK_S32
GK_API_VPSS_SetChnBufWrapAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			      const VPSS_CHN_BUF_WRAP_S *pstVpssChnBufWrap);
GK_S32 GK_API_VPSS_GetChnBufWrapAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				     VPSS_CHN_BUF_WRAP_S *pstVpssChnBufWrap);

GK_S32 GK_API_VPSS_TriggerSnapFrame(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				    GK_U32 u32FrameCnt);

GK_S32 GK_API_VPSS_AttachVbPool(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				VB_POOL hVbPool);
GK_S32 GK_API_VPSS_DetachVbPool(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);

GK_S32 GK_API_VPSS_EnableBufferShare(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
GK_S32 GK_API_VPSS_DisableBufferShare(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);

GK_S32 GK_API_VPSS_SetChnAlign(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			       GK_U32 u32Align);
GK_S32 GK_API_VPSS_GetChnAlign(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
			       GK_U32 *pu32Align);

GK_S32 GK_API_VPSS_SetChnProcMode(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				  VPSS_CHN_PROC_MODE_E enVpssChnProcMode);
GK_S32 GK_API_VPSS_GetChnProcMode(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				  VPSS_CHN_PROC_MODE_E *penVpssChnProcMode);

GK_S32 GK_API_VPSS_SetExtChnAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				 const VPSS_EXT_CHN_ATTR_S *pstExtChnAttr);
GK_S32 GK_API_VPSS_GetExtChnAttr(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				 VPSS_EXT_CHN_ATTR_S *pstExtChnAttr);

GK_S32 GK_API_VPSS_SetExtChnFisheye(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				    const FISHEYE_ATTR_S *pstFishEyeAttr);
GK_S32 GK_API_VPSS_GetExtChnFisheye(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
				    FISHEYE_ATTR_S *pstFishEyeAttr);

GK_S32 GK_API_VPSS_FisheyePosQueryDst2Src(VPSS_GRP VpssGrp, VPSS_CHN VpssChn,
					  GK_U32 u32RegionIndex,
					  const POINT_S *pstDstPointIn,
					  POINT_S *pstSrcPointOut);

GK_S32 GK_API_VPSS_SetGrpNRXParam(VPSS_GRP VpssGrp,
				  const VPSS_GRP_NRX_PARAM_S *pstNRXParam);
GK_S32 GK_API_VPSS_GetGrpNRXParam(VPSS_GRP VpssGrp,
				  VPSS_GRP_NRX_PARAM_S *pstNRXParam);

GK_S32 GK_API_VPSS_SetModParam(const VPSS_MOD_PARAM_S *pstModParam);
GK_S32 GK_API_VPSS_GetModParam(VPSS_MOD_PARAM_S *pstModParam);

GK_S32 GK_API_VPSS_GetChnFd(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
GK_S32 GK_API_VPSS_CloseFd(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
