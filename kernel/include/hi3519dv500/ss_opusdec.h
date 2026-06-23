/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_OPUSDEC_H
#define SS_OPUSDEC_H

#include "ot_opusdec.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

ot_opus_decoder ss_opusdec_create(const ot_opusdec_config *config);

td_void ss_opusdec_destroy(ot_opus_decoder decoder);

td_s32 ss_opusdec_process_frame(ot_opus_decoder decoder, td_u8 **in_buf, td_s32 *in_left_byte, td_s16 *out_buf,
    td_u32 *out_bytes_per_chn);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
