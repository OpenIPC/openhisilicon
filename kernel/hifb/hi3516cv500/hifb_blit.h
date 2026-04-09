/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: hifb blit
 */

#ifndef __HIFB_BLIT_H__
#define __HIFB_BLIT_H__

#include "hi_type.h"
#include "hifb_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_bool hifb_tde_is_support_fmt(HIFB_COLOR_FMT_E fmt);
hi_s32 hifb_drv_fill(HIFB_BUFFER_S *dst_img, hi_u32 fill_data);
hi_s32 hifb_drv_blit(HIFB_BUFFER_S *src_img, HIFB_BUFFER_S *dst_img, hifb_blit_opt *opt, hi_bool refresh_screen,
                     hi_s32 *refresh_handle);
hi_s32 hifb_drv_rotate(HIFB_BUFFER_S *src_img, HIFB_BUFFER_S *dst_img, hifb_rotate_opt *rot_opt,
                       hi_bool refresh_screen);
hi_s32 hifb_drv_set_tde_callback(int_callback tde_callback);
hi_s32 hifb_drv_set_tde_rotate_callback(int_callback tde_callback);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __HIFB_BLIT_H__ */
#endif /* __cplusplus */

#endif /* __HIFB_BLIT_H__ */
