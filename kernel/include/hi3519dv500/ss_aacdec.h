/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_AACDEC_H
#define SS_AACDEC_H

#include "ot_aacdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 ss_aacdec_get_version(ot_aacdec_version *version);

ot_aac_decoder ss_aacdec_init_decoder(ot_aacdec_transport_type transport_type);

td_void ss_aacdec_free_decoder(ot_aac_decoder aac_decoder);

td_s32 ss_aacdec_set_raw_mode(ot_aac_decoder aac_decoder, td_s32 chn_num, td_s32 sample_rate);

td_s32 ss_aacdec_find_sync_header(ot_aac_decoder aac_decoder, td_u8 **in_buf, td_s32 *bytes_left);

td_s32 ss_aacdec_frame(ot_aac_decoder aac_decoder, td_u8 **in_buf, td_s32 *bytes_left, td_s16 *out_pcm);

td_s32 ss_aacdec_set_eos_flag(ot_aac_decoder aac_decoder, td_s32 eos_flag);

td_s32 ss_aacdec_get_last_frame_info(ot_aac_decoder aac_decoder, ot_aacdec_frame_info *frame_info);

td_s32 ss_aacdec_flush_codec(ot_aac_decoder aac_decoder);

td_s32 ss_aacdec_register_mod(const td_void *mod_handle);

td_void *ss_aac_sbrdec_get_handle(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
