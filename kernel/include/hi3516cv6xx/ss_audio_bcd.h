/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_AUDIO_BCD_H
#define SS_AUDIO_BCD_H

#include "ot_audio_bcd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 ss_baby_crying_detection_init(ot_bcd_handle *bcd, td_s32 sample_rate, const ot_bcd_config *bcd_config);

td_s32 ss_baby_crying_detection_process(ot_bcd_handle bcd, ot_bcd_process_data *input_data,
    ot_bcd_process_data *output_data);

td_s32 ss_baby_crying_detection_deinit(ot_bcd_handle bcd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
