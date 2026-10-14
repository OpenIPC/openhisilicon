/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef _XMEDIA_AUDIO_RESAMPLER_H_
#define _XMEDIA_AUDIO_RESAMPLER_H_

#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

xmedia_void *xmedia_audio_resampler_create(xmedia_s32 in_rate, xmedia_s32 out_rate, xmedia_s32 chans);
xmedia_s32 xmedia_audio_resampler_process(xmedia_void *inst, xmedia_s16 *in_buf, xmedia_s32 in_samps, xmedia_s16 *out_buf);
xmedia_void xmedia_audio_resampler_destroy(xmedia_void *inst);
xmedia_s32 xmedia_audio_resampler_getmaxoutputnum(xmedia_void *inst, xmedia_s32 in_samps);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

