/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_AUDIO_DL_ADP_H
#define SS_AUDIO_DL_ADP_H

#include "ot_audio_dl_adp.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

td_s32 ss_audio_dlpath(td_char *lib_path);

td_s32 ss_audio_dlopen(td_void **lib_handle, td_char *lib_name);

td_s32 ss_audio_dlsym(td_void **func_handle, td_void *lib_handle, td_char *func_name);

td_s32 ss_audio_dlclose(td_void *lib_handle);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif
