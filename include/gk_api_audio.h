/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_API_AUDIO_H__
#define __GK_API_AUDIO_H__

#include "type.h"
#include "common.h"
#include "comm_aio.h"
#include "comm_aenc.h"
#include "comm_adec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

#define AENC_ADAPT_MAGIC 0Xfcfcfcfc

GK_S32 GK_API_AUDIO_SetModParam(const AUDIO_MOD_PARAM_S *pstModParam);
GK_S32 GK_API_AUDIO_GetModParam(AUDIO_MOD_PARAM_S *pstModParam);
GK_S32 GK_API_AUDIO_RegisterVQEModule(const AUDIO_VQE_REGISTER_S *pstVqeRegister);

GK_S32 GK_API_AI_SetPubAttr(AUDIO_DEV AiDevId, const AIO_ATTR_S *pstAttr);
GK_S32 GK_API_AI_GetPubAttr(AUDIO_DEV AiDevId, AIO_ATTR_S *pstAttr);

GK_S32 GK_API_AI_Enable(AUDIO_DEV AiDevId);
GK_S32 GK_API_AI_Disable(AUDIO_DEV AiDevId);

GK_S32 GK_API_AI_EnableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);
GK_S32 GK_API_AI_DisableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);

GK_S32 GK_API_AI_GetFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm, AEC_FRAME_S *pstAecFrm, GK_S32 s32MilliSec);
GK_S32 GK_API_AI_ReleaseFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, const AUDIO_FRAME_S *pstFrm, const AEC_FRAME_S *pstAecFrm);

GK_S32 GK_API_AI_SetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn, const AI_CHN_PARAM_S *pstChnParam);
GK_S32 GK_API_AI_GetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_CHN_PARAM_S *pstChnParam);

GK_S32 GK_API_AI_SetRecordVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, const AI_RECORDVQE_CONFIG_S *pstVqeConfig);
GK_S32 GK_API_AI_GetRecordVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_RECORDVQE_CONFIG_S *pstVqeConfig);

GK_S32 GK_API_AI_EnableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn);
GK_S32 GK_API_AI_DisableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn);

GK_S32 GK_API_AI_EnableReSmp(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_SAMPLE_RATE_E enOutSampleRate);
GK_S32 GK_API_AI_DisableReSmp(AUDIO_DEV AiDevId, AI_CHN AiChn);

GK_S32 GK_API_AI_SetTrackMode(AUDIO_DEV AiDevId, AUDIO_TRACK_MODE_E enTrackMode);
GK_S32 GK_API_AI_GetTrackMode(AUDIO_DEV AiDevId, AUDIO_TRACK_MODE_E *penTrackMode);
GK_S32 GK_API_AI_SaveFile(AUDIO_DEV AiDevId, AI_CHN AiChn, const AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo);
GK_S32 GK_API_AI_QueryFileStatus(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FILE_STATUS_S* pstFileStatus);

GK_S32 GK_API_AI_ClrPubAttr(AUDIO_DEV AiDevId);

GK_S32 GK_API_AI_GetFd(AUDIO_DEV AiDevId, AI_CHN AiChn);

GK_S32 GK_API_AI_EnableAecRefFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn);
GK_S32 GK_API_AI_DisableAecRefFrame(AUDIO_DEV AiDevId, AI_CHN AiChn);

GK_S32 GK_API_AI_SetTalkVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn, const AI_TALKVQE_CONFIG_S *pstVqeConfig);
GK_S32 GK_API_AI_GetTalkVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_TALKVQE_CONFIG_S *pstVqeConfig);

GK_S32 GK_API_AI_SetTalkVqeV2Attr(AUDIO_DEV AiDevId, AI_CHN AiChn,AUDIO_DEV AoDevId, AO_CHN AoChn, const AI_TALKVQEV2_CONFIG_S *pstVqeConfig);
GK_S32 GK_API_AI_GetTalkVqeV2Attr(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_TALKVQEV2_CONFIG_S *pstVqeConfig);

GK_S32 GK_API_AO_SetPubAttr(AUDIO_DEV AoDevId, const AIO_ATTR_S *pstAttr);
GK_S32 GK_API_AO_GetPubAttr(AUDIO_DEV AoDevId, AIO_ATTR_S *pstAttr);

GK_S32 GK_API_AO_Enable(AUDIO_DEV AoDevId);
GK_S32 GK_API_AO_Disable(AUDIO_DEV AoDevId);

GK_S32 GK_API_AO_EnableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
GK_S32 GK_API_AO_DisableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

GK_S32 GK_API_AO_SendFrame(AUDIO_DEV AoDevId, AO_CHN AoChn, const AUDIO_FRAME_S *pstData, GK_S32 s32MilliSec);

GK_S32 GK_API_AO_EnableReSmp(AUDIO_DEV AoDevId, AO_CHN AoChn, AUDIO_SAMPLE_RATE_E enInSampleRate);
GK_S32 GK_API_AO_DisableReSmp(AUDIO_DEV AoDevId, AO_CHN AoChn);

GK_S32 GK_API_AO_ClearChnBuf(AUDIO_DEV AoDevId, AO_CHN AoChn);
GK_S32 GK_API_AO_QueryChnStat(AUDIO_DEV AoDevId, AO_CHN AoChn, AO_CHN_STATE_S *pstStatus);

GK_S32 GK_API_AO_PauseChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
GK_S32 GK_API_AO_ResumeChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

GK_S32 GK_API_AO_SetVolume(AUDIO_DEV AoDevId, GK_S32 s32VolumeDb);
GK_S32 GK_API_AO_GetVolume(AUDIO_DEV AoDevId, GK_S32 *ps32VolumeDb);

GK_S32 GK_API_AO_SetMute(AUDIO_DEV AoDevId, GK_BOOL bEnable, const AUDIO_FADE_S *pstFade);
GK_S32 GK_API_AO_GetMute(AUDIO_DEV AoDevId, GK_BOOL *pbEnable, AUDIO_FADE_S *pstFade);

GK_S32 GK_API_AO_SetTrackMode(AUDIO_DEV AoDevId, AUDIO_TRACK_MODE_E enTrackMode);
GK_S32 GK_API_AO_GetTrackMode(AUDIO_DEV AoDevId, AUDIO_TRACK_MODE_E *penTrackMode);

GK_S32 GK_API_AO_GetFd(AUDIO_DEV AoDevId, AO_CHN AoChn);

GK_S32 GK_API_AO_ClrPubAttr(AUDIO_DEV AoDevId);

GK_S32 GK_API_AO_SetVqeAttr(AUDIO_DEV AoDevId, AO_CHN AoChn, const AO_VQE_CONFIG_S *pstVqeConfig);
GK_S32 GK_API_AO_GetVqeAttr(AUDIO_DEV AoDevId, AO_CHN AoChn, AO_VQE_CONFIG_S *pstVqeConfig);

GK_S32 GK_API_AO_EnableVqe(AUDIO_DEV AoDevId, AO_CHN AoChn);
GK_S32 GK_API_AO_DisableVqe(AUDIO_DEV AoDevId, AO_CHN AoChn);

GK_S32 GK_API_AENC_CreateChn(AENC_CHN AeChn, const AENC_CHN_ATTR_S *pstAttr);
GK_S32 GK_API_AENC_DestroyChn(AENC_CHN AeChn);

GK_S32 GK_API_AENC_SendFrame(AENC_CHN AeChn, const AUDIO_FRAME_S *pstFrm, const AEC_FRAME_S *pstAecFrm);

GK_S32 GK_API_AENC_GetStream(AENC_CHN AeChn, AUDIO_STREAM_S *pstStream, GK_S32 s32MilliSec);
GK_S32 GK_API_AENC_ReleaseStream(AENC_CHN AeChn, const AUDIO_STREAM_S *pstStream);

GK_S32 GK_API_AENC_GetFd(AENC_CHN AeChn);

GK_S32 GK_API_AENC_RegisterEncoder(GK_S32 *ps32Handle, const AENC_ENCODER_S *pstEncoder);
GK_S32 GK_API_AENC_UnRegisterEncoder(GK_S32 s32Handle);

GK_S32 GK_API_AENC_GetStreamBufInfo(AENC_CHN AeChn, GK_U64* pu64PhysAddr, GK_U32* pu32Size);

GK_S32 GK_API_ADEC_CreateChn(ADEC_CHN AdChn, const ADEC_CHN_ATTR_S *pstAttr);
GK_S32 GK_API_ADEC_DestroyChn(ADEC_CHN AdChn);

GK_S32 GK_API_ADEC_SendStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstStream, GK_BOOL bBlock);

GK_S32 GK_API_ADEC_ClearChnBuf(ADEC_CHN AdChn);

GK_S32 GK_API_ADEC_RegisterDecoder(GK_S32 *ps32Handle, const ADEC_DECODER_S *pstDecoder);
GK_S32 GK_API_ADEC_UnRegisterDecoder(GK_S32 s32Handle);

GK_S32 GK_API_ADEC_GetFrame(ADEC_CHN AdChn, AUDIO_FRAME_INFO_S *pstFrmInfo, GK_BOOL bBlock);
GK_S32 GK_API_ADEC_ReleaseFrame(ADEC_CHN AdChn, const AUDIO_FRAME_INFO_S *pstFrmInfo);
GK_S32 GK_API_ADEC_SendEndOfStream(ADEC_CHN AdChn, GK_BOOL bInstant);
GK_S32 GK_API_ADEC_QueryChnStat(ADEC_CHN AdChn, ADEC_CHN_STATE_S *pstBufferStatus);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

