/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_RESAMPLE_H
#define SS_RESAMPLE_H

#include "ot_resample.h"
#include "ot_vqe_register.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_void *ss_resample_create(td_s32 in_rate, td_s32 out_rate, td_s32 chn_num);

td_s32 ss_resample_process(td_void *instance, const td_s16 *in_buf, td_s32 in_samples, td_s16 *out_buf);

td_void ss_resample_destroy(td_void *instance);

td_s32 ss_resample_get_max_output_num(td_void *instance, td_s32 in_samples);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
