/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_AACENC_H
#define SS_AACENC_H

#include "ot_aacenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 ss_aacenc_get_version(ot_aacenc_version *version);

td_s32 ss_aacenc_init_default_config(ot_aacenc_config *config);

td_s32 ss_aacenc_open(ot_aac_encoder **aac_encoder, const ot_aacenc_config *config);

td_s32 ss_aacenc_frame(ot_aac_encoder *aac_encoder, td_s16 *input_buf, td_u8 *output_buf, td_s32 *num_out_bytes);

td_void ss_aacenc_close(ot_aac_encoder *aac_encoder);

td_s32 ss_aacenc_register_mod(const td_void *mod_handle);

td_void *ss_aac_sbrenc_get_handle(td_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
