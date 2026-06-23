/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_OPUSENC_H
#define SS_OPUSENC_H

#include "ot_opusenc.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

ot_opus_encoder ss_opusenc_create(const ot_opusenc_config *config);

td_void ss_opusenc_destroy(ot_opus_encoder encoder);

td_s32 ss_opusenc_process_frame(ot_opus_encoder encoder, td_s16 *in_buf, td_s32 samples, td_u8 *out_buf,
    td_u32 *out_valid_len);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
