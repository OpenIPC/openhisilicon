/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_AUDIO_OPUS_ADP_H
#define SS_AUDIO_OPUS_ADP_H

#include "ot_audio_opus_adp.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_aenc_opus_init(td_void);

td_s32 ss_mpi_aenc_opus_deinit(td_void);

td_s32 ss_mpi_adec_opus_init(td_void);

td_s32 ss_mpi_adec_opus_deinit(td_void);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif