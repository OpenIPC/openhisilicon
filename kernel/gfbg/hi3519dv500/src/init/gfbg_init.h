/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef GFBG_INIT_H
#define GFBG_INIT_H
#include <linux/dma-buf.h>
#include "ot_type.h"
#include "gfbg_graphics.h"
#include "gfbg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_void set_video_name(char *temp_video);

char *gfbg_get_layer_mmz_names(td_u32 layer_id);

td_void gfbg_cleanup(td_void);

td_s32 gfbg_init(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
