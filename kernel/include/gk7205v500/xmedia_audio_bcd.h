/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_BCD_H__
#define __XMEDIA_AUDIO_BCD_H__

#include "type.h"
#include "common.h"

#include "audio_bcd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

xmedia_s32 xmedia_audio_baby_crying_detection_init(bcd_handle *bcd, xmedia_s32 sample_rate, const audio_bcd_config *bcd_config);

xmedia_s32 xmedia_audio_baby_crying_detection_process(bcd_handle bcd, audio_bcd_process_data *input_data,
    audio_bcd_process_data *output_data);

xmedia_s32 xmedia_audio_baby_crying_detection_deinit(bcd_handle bcd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

