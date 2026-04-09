/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#ifndef __HIFB_VOU_GRAPHICS_H__
#define __HIFB_VOU_GRAPHICS_H__

#include "hi_type.h"
#include "hifb.h"
#include "hifb_vou_drv.h"
#include "hifb_graphic_drv.h"

#define MDDRC_ZONE_MAX_NUM 32
#define HIFB_LINE_BUF      1920
#define HIFB_MAX_LAYER_NUM 1
#define VO1_IRQ_NR         91

typedef enum {
    FB_VOU_BITEXT_LOW_ZERO = 0x0,
    FB_VOU_BITEXT_LOW_HIGHBIT = 0x2,
    FB_VOU_BITEXT_LOW_HIGHBITS = 0x3,
    FB_VOU_BITEXT_BUTT
} vou_bitext_mode;

typedef enum {
    FB_VOU_COLORKEY_IN = 0x0,
    FB_VOU_COLORKEY_OUT = 0x1,
    FB_VOU_COLORKEY_BUTT
} vou_colorkey_mode;

typedef struct {
    hi_u8 key_max_r;
    hi_u8 key_max_g;
    hi_u8 key_max_b;
} fb_vo_gfx_key_max;

typedef struct {
    hi_u8 key_min_r;
    hi_u8 key_min_g;
    hi_u8 key_min_b;
} fb_vo_gfx_key_min;

typedef struct {
    hi_u8 mask_r;
    hi_u8 mask_g;
    hi_u8 mask_b;
} fb_vo_gfx_mask;

typedef enum {
    FB_VOU_SCAN_MODE_INTERLACE = 0x0,
    FB_VOU_SCAN_MODE_PROGRESSIVE = 0x1,
    FB_VOU_SCAN_MODE_BUTT
} fb_vou_scan_mode;

typedef enum {
    FB_VO_GFX_BITEXTEND_1ST = 0,
    FB_VO_GFX_BITEXTEND_2ND = 0x2,
    FB_VO_GFX_BITEXTEND_3RD = 0x3,

    FB_VO_GFX_BITEXTEND_BUTT
} fb_vo_gfx_bitextend;
typedef hi_s32 (*vo_fb_intcallback)(const hi_void *, const hi_void *);

vo_gfxlayer_context *get_gfx_layer_ctx(hi_void);
hi_bool vou_graphics_enable_dcmp(GRAPHIC_LAYER gfx_layer, hi_bool enable);
hi_bool vou_graphics_get_dcmp_enable_state(GRAPHIC_LAYER gfx_layer, hi_bool *enable);
hi_void vou_graphics_enable_wbc(hal_disp_layer gfx_layer, hi_bool enable);
hi_void vou_graphics_set_mddr_detect_zone(hi_u32 start_id, hi_u32 cnt, fb_vo_mddrc_zone_addr *zone_addr);
hi_void vou_graphics_get_mddr_status(hi_u32 start_id, hi_u32 cnt, hi_u32 *status);
hi_void vou_graphics_clear_ddr_dectect_zone(hi_u32 start_id, hi_u32 cnt);
hi_s32 vou_graphics_check_ddr_dectect_zone(GRAPHIC_LAYER gfx_layer, hi_u32 new_start_id, hi_u32 new_cnt,
                                           hi_u32 pre_start_id, hi_u32 pre_cnt);
hi_void vou_graphics_get_ddr_zone_cfg(GRAPHIC_LAYER gfx_layer, hi_u32 *start_id, hi_u32 *cnt);
hi_void vou_graphics_init_mddr_detect(hi_void);

hi_bool vou_graphics_set_gfx_ext(GRAPHIC_LAYER gfx_layer, fb_vo_gfx_bitextend mode);
hi_bool vou_graphics_set_gfx_palpha(GRAPHIC_LAYER gfx_layer, hi_u32 alpha_en, hi_u32 arange, hi_u8 alpha0,
                                    hi_u8 alpha1);
hi_bool vou_graphics_set_layer_galpha(GRAPHIC_LAYER gfx_layer, hi_u8 alpha0);
hi_bool vou_graphics_set_csc_en(GRAPHIC_LAYER gfx_layer, hi_bool csc_en);
hi_bool vou_graphics_set_gfx_addr(GRAPHIC_LAYER gfx_layer, hi_u64 laddr);
hi_bool vou_graphics_set_gfx_stride(GRAPHIC_LAYER gfx_layer, hi_u16 pitch);

hi_bool vou_graphics_get_gfx_pre_mult(GRAPHIC_LAYER gfx_layer, hi_u32 *enable);
hi_bool vou_graphics_set_gfx_pre_mult(GRAPHIC_LAYER gfx_layer, hi_u32 enable);
hi_bool vou_graphics_set_layer_data_fmt(GRAPHIC_LAYER gfx_layer, fb_vo_disp_pixel_format data_fmt);
hi_bool vou_graphics_set_layer_in_rect(GRAPHIC_LAYER gfx_layer, HIFB_RECT *rect);
hi_bool vou_graphics_set_layer_src_image_reso(GRAPHIC_LAYER gfx_layer, HIFB_RECT *rect);
hi_bool vou_graphics_set_layer_out_rect(GRAPHIC_LAYER gfx_layer, HIFB_RECT *rect);
hi_bool vou_graphics_set_color_key_value(GRAPHIC_LAYER gfx_layer, fb_vo_gfx_key_max key_max,
                                         fb_vo_gfx_key_min key_min);
hi_bool vou_graphics_set_color_key_mask(GRAPHIC_LAYER gfx_layer, fb_vo_gfx_mask vo_msk);
hi_bool vou_graphics_set_gfx_key_en(GRAPHIC_LAYER gfx_layer, hi_u32 key_enable);
hi_bool vou_graphics_set_gfx_key_mode(GRAPHIC_LAYER gfx_layer, hi_u32 key_out);

hi_bool vou_graphics_set_reg_up(GRAPHIC_LAYER gfx_layer);
hi_bool vou_graphics_get_layer_galpha(GRAPHIC_LAYER gfx_layer, hi_u8 *alpha0);
hi_bool vou_graphics_get_layer_data_fmt(GRAPHIC_LAYER gfx_layer, hi_u32 *fmt);
hi_bool vou_graphics_get_gfx_stride(GRAPHIC_LAYER gfx_layer, hi_u32 *gfx_stride);
hi_bool vou_graphics_get_gfx_addr(GRAPHIC_LAYER gfx_layer, hi_u64 *gfx_addr);

hi_s32 vou_graphics_get_dev_mode(hal_disp_layer layer, fb_vou_scan_mode *scan_mode, hi_bool *feild_update);

hi_void vo_graphics_vtth_int_process(hifb_vo_dev vo_dev);
hi_void vo_graphics_wbc_int_process(hi_u32 int_status);

hi_s32 vou_graphics_bind_dev(GRAPHIC_LAYER gfx_layer, hifb_vo_dev vo_dev);
hi_s32 vou_graphic_sun_bind_dev(GRAPHIC_LAYER gfx_layer, hifb_vo_dev vo_dev);

hi_s32 vou_graphics_set_csc(hi_u32 layer, fb_vo_csc *csc);
hi_s32 vou_graphics_get_csc(hi_u32 layer, fb_vo_csc *csc);
hi_s32 fb_vo_graphics_init(hi_void);
hi_s32 vo_graphics_deinit(hi_void);
hi_s32 vou_graphics_resource_init(hi_void);
hi_s32 vou_graphics_resource_deinit(hi_void);

hi_s32 vou_graphics_open_layer(GRAPHIC_LAYER gfx_layer);
hi_s32 vou_graphics_close_layer(GRAPHIC_LAYER gfx_layer);
hi_s32 vou_graphics_enable_layer(GRAPHIC_LAYER gfx_layer, hi_bool enable);
hi_s32 vou_graphics_set_callback(GRAPHIC_LAYER gfx_layer, fb_vo_int_type int_type, vo_fb_intcallback callback,
                                 hi_void *arg);
hi_s32 vou_graphics_get_intf_size(GRAPHIC_LAYER gfx_layer, hi_u32 *width, hi_u32 *height);
hi_s32 vou_graphics_get_intf_type(GRAPHIC_LAYER gfx_layer, VO_INTF_TYPE_E *intf_type);

hi_s32 vou_graphics_set_csc_coef(GRAPHIC_LAYER gfx_layer);

hi_s32 fb_vou_graphics_show_proc(osal_proc_entry_t *s);
hi_s32 vou_graphics_enable_int(hi_u32 layer_index, hi_bool enable);
hi_bool vou_graphics_clear_int(hi_u32 int_clear, hi_s32 irq);
hi_bool vou_graphics_get_int(hi_u32 *int_staus);
hi_bool vou_graphics_clear_int_status(hi_u32 int_status);
hi_s32 vou_graphics_get_interrupt_dev(hi_u32 int_status, hifb_vo_dev *pvo_dev);
/* for GHDR */
hi_bool vou_graphics_enable_ghdr(GRAPHIC_LAYER gfx_layer, hi_bool enable);

/* for ZME */
hi_bool vou_graphics_enable_zme(GRAPHIC_LAYER gfx_layer, RECT_S *in_rect, RECT_S *out_rect, hi_bool enable);

hi_s32 vou_graphics_init(hi_void);
hi_s32 vou_graphics_deinit(hi_void);
#endif /* __HIFB_VOU_GRAPHICS_H__*/
