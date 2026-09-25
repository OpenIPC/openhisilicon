/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __AUDIO_BCD_H__
#define __AUDIO_BCD_H__

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_CODE_BCD_INVALID_HANDLE (xmedia_u32)(0xA0148100)
#define ERR_CODE_BCD_MEMORY_ERROR   (xmedia_u32)(0xA0148101)
#define ERR_CODE_BCD_ILLEGAL_PARAM  (xmedia_u32)(0xA0148102)
#define ERR_CODE_BCD_NULL_PTR       (xmedia_u32)(0xA0148103)
#define ERR_CODE_BCD_NOT_PERM       (xmedia_u32)(0xA0148104)

typedef xmedia_void *bcd_handle;
typedef xmedia_s32 (*fn_bcd_callback)(xmedia_void *);

typedef struct {
    xmedia_bool usr_mode;
    xmedia_bool bypass;
    xmedia_u32 alarm_threshold;
    xmedia_u32 time_limit;
    xmedia_u32 time_limit_threshold_count;
    xmedia_u32 interval_time;
    fn_bcd_callback callback;
} audio_bcd_config;

typedef struct {
    xmedia_s16 *data;
    xmedia_s32 data_size;
} audio_bcd_process_data;

xmedia_s32 audio_baby_crying_detection_init(bcd_handle *bcd, xmedia_s32 sample_rate, const audio_bcd_config *bcd_config);

xmedia_s32 audio_baby_crying_detection_process(bcd_handle bcd, audio_bcd_process_data *input_data,
    audio_bcd_process_data *output_data);

xmedia_s32 audio_baby_crying_detection_deinit(bcd_handle bcd);

#ifdef __cplusplus
}
#endif
#endif