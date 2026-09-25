/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_RESAMPLER_API_H__
#define __XMEDIA_RESAMPLER_API_H__

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

XMEDIA_VOID *XMEDIA_AUDIO_Resampler_Create(XMEDIA_S32 s32Inrate, XMEDIA_S32 s32Outrate, XMEDIA_S32 s32Chans);

XMEDIA_S32 XMEDIA_AUDIO_Resampler_Process(XMEDIA_VOID *inst, XMEDIA_S16 *s16Inbuf, XMEDIA_S32 s32Insamps, XMEDIA_S16 *s16Outbuf);

XMEDIA_VOID XMEDIA_AUDIO_Resampler_Destroy(XMEDIA_VOID *inst);

XMEDIA_S32 XMEDIA_AUDIO_Resampler_GetMaxOutputNum(XMEDIA_VOID *inst, XMEDIA_S32 s32Insamps);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

