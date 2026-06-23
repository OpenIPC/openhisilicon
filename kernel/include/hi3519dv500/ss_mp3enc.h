/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MP3ENC_H
#define SS_MP3ENC_H

#include "ot_mp3enc.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

ot_mp3enc_handle ss_mp3enc_create(const ot_mp3enc_attr *attr);

td_s32 ss_mp3enc_process_frame(ot_mp3enc_handle mp3_encoder, td_s16 *pcm_buf, td_u8 *out_buf, td_s32 *num_out_bytes);

td_void ss_mp3enc_destroy(ot_mp3enc_handle mp3_encoder);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
