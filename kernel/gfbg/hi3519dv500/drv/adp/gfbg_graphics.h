/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_GRAPHICS_H
#define GFBG_GRAPHICS_H

#include "ot_type.h"
#include "gfbg.h"
#include "gfbg_drv.h"
#include "gfbg_graphics_drv.h"

#define MDDRC_ZONE_MAX_NUM 32
#define GFBG_LINE_BUF      1920
#define GFBG_IRQ_NR        193

#define FB_CSC_LUMA_MAX     100
#define FB_CSC_CONT_MAX     100
#define FB_CSC_HUE_MAX      100
#define FB_CSC_SAT_MAX      100

typedef enum {
    FB_BITEXT_LOW_ZERO = 0x0,
    FB_BITEXT_LOW_HIGHBIT = 0x2,
    FB_BITEXT_LOW_HIGHBITS = 0x3,
    FB_BITEXT_BUTT
} fb_bitext_mode;

typedef enum {
    FB_COLORKEY_IN = 0x0,
    FB_COLORKEY_OUT = 0x1,
    FB_COLORKEY_BUTT
} fb_colorkey_mode;

typedef struct {
    td_u8 key_max_r;
    td_u8 key_max_g;
    td_u8 key_max_b;
} fb_gfx_key_max;

typedef struct {
    td_u8 key_min_r;
    td_u8 key_min_g;
    td_u8 key_min_b;
} fb_gfx_key_min;

typedef struct {
    td_u8 mask_r;
    td_u8 mask_g;
    td_u8 mask_b;
} fb_gfx_mask;

typedef enum {
    FB_SCAN_MODE_INTERLACE = 0x0,
    FB_SCAN_MODE_PROGRESSIVE = 0x1,
    FB_SCAN_MODE_BUTT
} fb_scan_mode;

typedef enum {
    FB_GFX_BITEXTEND_1ST = 0,
    FB_GFX_BITEXTEND_2ND = 0x2,
    FB_GFX_BITEXTEND_3RD = 0x3,

    FB_GFX_BITEXTEND_BUTT
} fb_gfx_bitextend;
typedef td_s32 (*vo_fb_intcallback)(const td_void *paraml, ot_vo_dev vo_dev, const td_void *paramr);

unsigned int *fb_get_gfbg_irq(td_void);
graphic_layer_context *fb_graphics_get_gfx_layer_ctx(td_void);
td_s32 fb_graphics_enable_dcmp(ot_gfx_layer gfx_layer, td_bool enable);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_graphics_get_dcmp_enable_state(ot_gfx_layer gfx_layer, td_bool *enable);
td_s32 fb_graphics_set_dcmp_info(ot_gfx_layer gfx_layer, const gfbg_graphic_dcmp_info *dcmp_info);
#endif

td_s32 fb_graphics_set_gfx_ext(ot_gfx_layer gfx_layer, fb_gfx_bitextend mode);
td_s32 fb_graphics_set_gfx_palpha(ot_gfx_layer gfx_layer, td_u32 alpha_en, td_u32 arange, td_u8 alpha0, td_u8 alpha1);
td_s32 fb_graphics_set_layer_galpha(ot_gfx_layer gfx_layer, td_u8 alpha0);
td_s32 fb_graphics_set_csc_en(ot_gfx_layer gfx_layer, td_bool csc_en);
td_s32 fb_graphics_set_gfx_addr(ot_gfx_layer gfx_layer, td_phys_addr_t laddr);
td_s32 fb_graphics_set_gfx_stride(ot_gfx_layer gfx_layer, td_u16 pitch);

td_s32 fb_graphics_get_gfx_pre_mult(ot_gfx_layer gfx_layer, td_u32 *enable);
td_s32 fb_graphics_set_gfx_pre_mult(ot_gfx_layer gfx_layer, td_u32 enable);
td_s32 fb_graphics_set_layer_data_fmt(ot_gfx_layer gfx_layer, gfbg_disp_pixel_format enDataFmt);
td_s32 fb_graphics_set_layer_in_rect(ot_gfx_layer gfx_layer, const ot_fb_rect *rect);
td_s32 fb_graphics_set_layer_src_image_reso(ot_gfx_layer gfx_layer, const ot_fb_rect *rect);
td_s32 fb_graphics_set_layer_out_rect(ot_gfx_layer gfx_layer, const ot_fb_rect *rect);
td_s32 fb_graphics_set_color_key_value(ot_gfx_layer gfx_layer, fb_gfx_key_max key_max, fb_gfx_key_min key_min);
td_s32 fb_graphics_set_color_key_mask(ot_gfx_layer gfx_layer, fb_gfx_mask msk);
td_s32 graphic_drv_get_dev_enable(ot_vo_dev vo_dev, td_bool *intf_en);
td_s32 graphic_drv_get_intf_sync(ot_vo_dev vo_dev, hal_disp_syncinfo *sync_info);
td_s32 graphic_drv_get_intf_mux_sel(ot_vo_dev vo_dev, vo_intf_type *intf_type);
td_s32 fb_graphics_set_gfx_key_en(ot_gfx_layer gfx_layer, td_u32 key_enable);
td_s32 fb_graphics_set_gfx_key_mode(ot_gfx_layer gfx_layer, td_u32 key_out);

td_s32 fb_graphics_set_reg_up(ot_gfx_layer gfx_layer);
td_void fb_graphics_set_color_reg(td_u32 layer_id, td_phys_addr_t clut_phy_addr);
td_void fb_graphics_set_color_reg_up(td_u32 layer_id);
td_s32 fb_graphics_get_layer_galpha(ot_gfx_layer gfx_layer, td_u8 *alpha0);
td_s32 fb_graphics_get_layer_data_fmt(ot_gfx_layer gfx_layer, td_u32 *fmt);
td_s32 fb_graphics_get_gfx_stride(ot_gfx_layer gfx_layer, td_u32 *gfx_stride);
td_s32 fb_graphics_get_gfx_addr(ot_gfx_layer gfx_layer, td_phys_addr_t *gfx_addr);

td_s32 fb_graphics_get_dev_mode(hal_disp_layer layer, fb_scan_mode *scan_mode, td_bool *feild_update);

td_s32 fb_graphics_resource_init(td_void);
td_s32 fb_graphics_resource_deinit(td_void);

td_s32 fb_graphics_open_layer(ot_gfx_layer gfx_layer);
td_s32 fb_graphics_close_layer(ot_gfx_layer gfx_layer);
td_s32 fb_graphics_set_layer_enable(ot_gfx_layer gfx_layer, td_bool enable);
td_s32 fb_graphics_get_layer_enable(ot_gfx_layer gfx_layer, td_bool *enable);
td_s32 fb_graphics_set_callback(ot_gfx_layer gfx_layer, gfbg_int_type int_type, vo_fb_intcallback callback,
                                td_void *arg);
td_s32 fb_graphics_get_intf_size(ot_gfx_layer gfx_layer, td_u32 *width, td_u32 *height);

td_s32 fb_graphics_set_csc_coef(ot_gfx_layer gfx_layer);

td_s32 fb_graphics_get_int(td_u32 *int_staus);
td_s32 fb_graphics_get_interrupt_dev(td_u32 int_status, ot_vo_dev *vo_dev);

// for ZME
td_s32 fb_graphics_enable_zme(ot_gfx_layer gfx_layer, const ot_fb_rect *in_rect, const ot_fb_rect *out_rect,
                              td_bool enable);

/* for low delay */
td_void fb_graphics_set_tde_sync(const gfbg_bind_tde_cfg *bind_tde_cfg);
/* for smart rect */
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_graphics_set_frame_reg(td_u32 layer_id, td_phys_addr_t phys_addr);
td_void fb_graphics_up_frame_param(td_u32 layer_id);
td_void fb_graphics_close_smart_rect(td_s32 layer_id);
#endif

#endif /* GFBG_GRAPHICS_H */

