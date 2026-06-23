/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_ROTATE_H
#define GFBG_ROTATE_H

#include "ot_type.h"
#include "ot_common.h"
#include "drv_tde.h"
#include "gfbg_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef td_s32 (*gfbg_rotate)(const ot_fb_buf *, const ot_fb_buf *, const gfbg_rotate_opt *, td_bool);

typedef struct {
    td_bool is_support;
    gfbg_rotate gfbg_drv_rotate;
} gfbg_rotate_ops;

td_void gfbg_rotation_register(td_void);
td_bool gfbg_get_rotation_support(td_void);
gfbg_rotate gfbg_get_rotation(td_void);
td_s32 gfbg_drv_rotate_process(const ot_fb_buf *src_img, const ot_fb_buf *dst_img, const gfbg_rotate_opt *rotate_opt,
    td_bool is_refresh_screen);
td_s32 gfbg_drv_set_tde_rotate_callback(drv_tde_func_callback tde_rot_callback);
drv_tde_func_callback gfbg_drv_get_tde_rotate_callback(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_ROTATE_H */