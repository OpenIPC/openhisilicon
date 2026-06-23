/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_AUDIO_AAC_ADP_H
#define SS_AUDIO_AAC_ADP_H

#include "ot_audio_aac_adp.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

td_s32 ss_mpi_aenc_aac_init(td_void);
td_s32 ss_mpi_aenc_aac_deinit(td_void);

td_s32 ss_mpi_adec_aac_init(td_void);
td_s32 ss_mpi_adec_aac_deinit(td_void);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
