/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */
#ifndef __HIFB_DRV_H__
#define __HIFB_DRV_H__
#ifdef __HuaweiLite__
#include <fb.h>
#else
#include <linux/fb.h>
#endif
#include "hifb.h"
#include "hi_debug.h"
#include "hifb_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __HuaweiLite__
typedef struct {
    struct hifb_info *info;
    hi_ulong layer_size;
    hi_u32 curosr_buf_size;    /* For soft cursor */
} hifb_layer;

#else
typedef struct {
    struct fb_info *info;
    hi_ulong layer_size;       /* ulLayerSize = fb.smem_len, For display buf, KB */
    hi_u32 curosr_buf_size;    /* For soft cursor */
} hifb_layer;
#endif

char *hifb_get_layer_mmz_names(hi_u32 layer_id);

#if (HICHIP == HI3516E_V200)
typedef hi_s32 (*intvgscallback)(hi_u32 call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id, hi_void *job_data);
#endif

typedef hi_s32 (*int_callback)(const hi_void *paraml, const hi_void *paramr);
#define hifb_trace(level, fmt...)                                                         \
    do {                                                                                  \
        HI_TRACE(level, HI_ID_VO, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
        HI_TRACE(level, HI_ID_VO, ##fmt);                                                 \
    } while (0)

#define hi_trace_fb(level, fmt...)                                                        \
    do {                                                                                  \
        HI_TRACE(level, HI_ID_FB, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
        HI_TRACE(level, HI_ID_FB, ##fmt);                                                 \
    } while (0)

#define hifb_fatal(fmt...) hi_trace_fb(HI_DBG_EMERG, fmt)
#define hifb_error(fmt...) hi_trace_fb(HI_DBG_ERR, fmt)
#define hifb_warning(fmt...) hi_trace_fb(HI_DBG_WARN, fmt)
#define hifb_dbg_info(fmt...) hi_trace_fb(HI_DBG_INFO, fmt)

typedef hi_u32 hifb_layer_id; /* start from 0 */

hi_bool hifb_drv_support_rotate(void);
hi_bool is_hd_layer(hi_u32 layer_id);
hi_bool is_sd_layer(hi_u32 layer_id);
hi_bool is_ad_layer(hi_u32 layer_id);
hi_bool is_cursor_layer(hi_u32 layer_id);

typedef struct {
    hi_bool key_enable;  /* colorkey */
    hi_bool mask_enable; /* key mask */
    hi_u32 key;
    hi_u8 red_mask;      /* red mask */
    hi_u8 green_mask;    /* green mask */
    hi_u8 blue_mask;     /* blue mask */
    hi_u8 reserved;
    hi_u32 key_mode;     /* 0:In region; 1:Out region */
    hi_u8 red_max;       /* colorkey red max */
    hi_u8 green_max;     /* colorkey green max */
    hi_u8 blue_max;      /* colorkey blue max */
    hi_u8 reserved1;
    hi_u8 red_min;       /* colorkey red min */
    hi_u8 green_min;     /* colorkey green min */
    hi_u8 blue_min;      /* colorkey blue min */
    hi_u8 reserved2;
} hifb_colorkeyex;

typedef struct {
    HIFB_RECT clip_rect;
    hi_bool in_region_clip;
    hi_bool clip;
} hifb_clip;

typedef struct {
    HIFB_ALPHA_S alpha;
    hifb_colorkeyex ckey;
    hifb_clip clip;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E antiflicker_level;
    hi_bool scale;
    hi_bool block;
    hi_bool call_back;
    hi_bool region_deflicker;
    hi_void *param;
    hi_u64 cmap_addr;
    hi_bool soft_cursor_update;
    HIFB_MIRROR_MODE_E mirror_mode;
    hi_bool compress;
} hifb_blit_opt;

/* for graphic rotation. */
typedef struct {
    hi_bool block;
    hi_bool call_back;
    hi_void *param;
    HIFB_ROTATE_MODE_E rotate_mode;
} hifb_rotate_opt;

typedef enum {
    HIFB_SCANMODE_P,
    HIFB_SCANMODE_I,
    HIFB_SCANMODE_BUTT,
} hifb_scan_mode;

/* layer state */
typedef enum {
    HIFB_LAYER_STATE_ENABLE = 0x0,

    HIFB_LAYER_STATE_DISABLE,

    HIFB_LAYER_STATE_INVALID,

    HIFB_LAYER_STATE_BUTT
} hifb_layer_state;

typedef struct {
    hifb_scan_mode scan_mode;
    hi_u32 screen_width;
    hi_u32 screen_height;
} disp_info;

typedef struct {
    hi_u64 buffer_phy_addr;
    hi_u32 stride;
    hi_bool feild_update; /* field update mode or not */
    hifb_scan_mode scan_mode;
    hi_u32 screen_width;  /* current sync width */
    hi_u32 screen_height; /* current sync height */
} hifb_osd_data;

/* mask bit */
typedef enum {
    HIFB_LAYER_PARAMODIFY_FMT = 0x1,                /* color format */
    HIFB_LAYER_PARAMODIFY_STRIDE = 0x2,             /* stride, line spacing */
    HIFB_LAYER_PARAMODIFY_ALPHA = 0x4,              /* alpha */
    HIFB_LAYER_PARAMODIFY_COLORKEY = 0x8,           /* colorkey */
    HIFB_LAYER_PARAMODIFY_INRECT = 0x10,            /* input rect */
    HIFB_LAYER_PARAMODIFY_OUTRECT = 0x20,           /* output rect */
    HIFB_LAYER_PARAMODIFY_DISPLAYADDR = 0x40,       /* display addr */
    HIFB_LAYER_PARAMODIFY_SHOW = 0x80,              /* show or hide */
    HIFB_LAYER_PARAMODIFY_BMUL = 0x100,             /* be pre-multi data */
    HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL = 0x200, /* ANTIFLICKERLEVEL */
    HIFB_LAYER_PARAMODIFY_DYNAMICRANGE = 0x400,     /* DYNAMICRANGE */
    HIFB_LAYER_PARAMODIFY_BUTT
} hifb_layer_paramodify_maskbit;

typedef struct {
    hi_u32 phy_addr;
    HIFB_COLOR_FMT_E color_fmt;
    HIFB_SIZE_S resolution;
    hi_u32 stride;
} hifb_vcmp_inbufinfo;

typedef struct {
    hi_u32 bank_width;
    hi_u32 stride;
    hi_u32 alpha_addr;
    hi_u32 red_addr;
    hi_u32 green_addr;
    hi_u32 blue_addr;
} hifb_vcmp_outbufinfo;

typedef enum {
    FB_VO_INPUTFMT_ARGB_4444 = 0x48,
    FB_VO_INPUTFMT_ARGB_1555 = 0x49,
    FB_VO_INPUTFMT_ARGB_8888 = 0x68,

    FB_VO_DISP_PIXELFORMAT_BUTT
} fb_vo_disp_pixel_format;

typedef struct {
    fb_vo_disp_pixel_format pixel_fmt;
    hi_u64 ar_phy_addr;
    hi_u64 gb_phy_addr;
    hi_u32 frame_size0;
    hi_u32 frame_size1;
    hi_u32 width;
    hi_u32 height;
    hi_bool is_lossless_a;
    hi_bool is_lossless;
    hi_u32 offset;
    hi_u32 stride;
} fb_vo_graphic_dcmp_info;

typedef enum {
    HIFB_VO_INTTYPE_VO,
    HIFB_VO_INTTYPE_VO_DISP,
    HIFB_VO_INTTYPE_WBC,
    HIFB_VO_INTTYPE_BUTT,
} fb_vo_int_type;

typedef struct {
    hi_s32 x;
    hi_s32 y;
    hi_s32 w;
    hi_s32 h;
} fb_vo_rect;

typedef struct {
    hi_u64 ar_phy_addr;
    hi_u64 gb_phy_addr;
    hi_u32 offset;
    hi_u32 stride;
    fb_vo_rect in_rect;
    fb_vo_rect out_rect;
} fb_vo_graphic_wbc_info;

typedef struct {
    hi_u32 start_phy_addr;
    hi_u32 end_phy_addr;
} fb_vo_mddrc_zone_addr;

typedef enum {
    FB_VO_CSC_MATRIX_IDENTITY = 0, /* Identity CSC matrix. */

    FB_VO_CSC_MATRIX_BT601_TO_BT709, /* BT601 to BT709 */
    FB_VO_CSC_MATRIX_BT709_TO_BT601, /* BT709 to BT601 */

    FB_VO_CSC_MATRIX_BT601_TO_RGB_PC, /* BT601 to RGB */
    FB_VO_CSC_MATRIX_BT709_TO_RGB_PC, /* BT709 to RGB */

    FB_VO_CSC_MATRIX_RGB_TO_BT601_PC, /* RGB to BT601 FULL */
    FB_VO_CSC_MATRIX_RGB_TO_BT709_PC, /* RGB to BT709 FULL */

    FB_VO_CSC_MATRIX_RGB_TO_BT2020_PC, /* RGB to BT.2020 */
    FB_VO_CSC_MATRIX_BT2020_TO_RGB_PC, /* BT.2020 to RGB */

    FB_VO_CSC_MATRIX_RGB_TO_BT601_TV, /* RGB to BT601 LIMIT */
    FB_VO_CSC_MATRIX_RGB_TO_BT709_TV, /* RGB to BT709 LIMIT */

    FB_VO_CSC_MATRIX_BUTT
} fb_vo_csc_matrix;

typedef struct {
    fb_vo_csc_matrix csc_matrix; /* CSC matrix */
    hi_u32 luma; /* RW; range:    [0, 100]; luminance, default: 50 */
    hi_u32 contrast; /* RW; range:    [0, 100]; contrast, default: 50 */
    hi_u32 hue; /* RW; range:    [0, 100]; hue, default: 50 */
    hi_u32 satuature; /* RW; range:    [0, 100]; satuature, default: 50 */
} fb_vo_csc;

typedef enum {
    HIFB_INTTYPE_VO,      /* Vertical timing interrupt */
    HIFB_INTTYPE_VO_DISP, /* Timing switch notification */
    HIFB_INTTYPE_WBC,     /* Compression completion interrupt */
    HIFB_INTTYPE_BUTT,
} fb_int_type;

typedef struct {
    const char *chip_name;          /* chip name */
    hi_u32 layer_count;             /* support how many layers */
    HIFB_CAPABILITY_S *capability; /* capability */

    /* init DRV */
    hi_s32 (*hifb_drv_init)(hi_void);

    /* De init DRV */
    hi_s32 (*hifb_drv_deinit)(hi_void);
    hi_s32 (*hifb_drv_layer_default_setting)(hi_u32);

    /* enable/disable layer */
    hi_s32 (*hifb_drv_enable_layer)(hi_u32, hi_bool);

    /* set layer address */
    hi_s32 (*hifb_drv_set_layer_addr)(hi_u32, hi_u64);

    /* get layer address */
    hi_s32 (*hifb_drv_get_layer_addr)(hi_u32, hi_u64 *);

    /* set layer stride */
    hi_s32 (*hifb_drv_set_layer_stride)(hi_u32, hi_u32);

    /* set layer pixel format */
    hi_s32 (*hifb_drv_set_layer_data_fmt)(hi_u32, HIFB_COLOR_FMT_E);

    /* set color registers */
    hi_s32 (*hifb_drv_set_color_reg)(hi_u32, hi_u32, hi_u32, hi_s32);

    /* wait until vblank, it's a block interface */
    hi_s32 (*hifb_drv_wait_v_blank)(hi_u32);

    /* set layer deflicker */
    hi_s32 (*hifb_drv_set_layer_de_flicker)(hi_u32, HIFB_DEFLICKER_S);

    /* set layer alpha */
    hi_s32 (*hifb_drv_set_layer_alpha)(hi_u32, HIFB_ALPHA_S);

    /* set layer start position and size */
    hi_s32 (*hifb_drv_set_layer_rect)(hi_u32, const HIFB_RECT *, const HIFB_RECT *);

    /* set layer src image resolution */
    hi_s32 (*hifb_drv_set_layer_src_image_reso)(hi_u32, const HIFB_RECT *);

    /* other color format convert to RGB888 format */
#ifdef __HuaweiLite__
    hi_s32 (*hifb_drv_color_convert)(const struct hifb_info *, hifb_colorkeyex *);
#else
    hi_s32 (*hifb_drv_color_convert)(const struct fb_var_screeninfo *, hifb_colorkeyex *);
#endif
    /* set layer colorkey */
    hi_s32 (*hifb_drv_set_layer_key_mask)(hi_u32, const hifb_colorkeyex *);

    /* update layer register */
    hi_s32 (*hifb_drv_updata_layer_reg)(hi_u32);

    /* set premul data */
    hi_s32 (*hifb_drv_set_pre_mul)(hi_u32, hi_bool);

    hi_s32 (*hifb_drv_set_clut_addr)(hi_u32, hi_u32);
    /* get osd data */
    hi_s32 (*hifb_drv_get_osd_data)(hi_u32, hifb_osd_data *);
    /* register call back function */
    hi_s32 (*hifb_drv_set_int_callback)
    (fb_vo_int_type, int_callback, hi_u32, hi_void *);
    /* open vo */
    hi_s32 (*hifb_drv_open_display)(hi_void);

    /* close vou */
    hi_s32 (*hifb_drv_close_display)(hi_void);

    hi_s32 (*hifb_open_layer)(hi_u32);
    hi_s32 (*hifb_close_layer)(hi_u32);

    /* for compression */
    hi_void (*hifb_drv_get_wbc_en_state)(hi_u32, hi_bool *);
    hi_void (*hifb_drv_get_wbc_stop_state)(hi_u32, hi_bool *);
    hi_void (*hifb_drv_set_wbc_info)(hi_u32, fb_vo_graphic_wbc_info *);
    hi_void (*hifb_drv_enable_wbc_int)(hi_u32, hi_bool);
    hi_void (*hifb_drv_enable_wbc)(hi_u32, hi_bool);
    hi_bool (*hifb_drv_enable_dcmp)(hi_u32, hi_bool);
    hi_bool (*hifb_drv_get_dcmp_enable_state)(hi_u32, hi_bool *);
    hi_bool (*hifb_drv_set_dcmp_info)(hi_u32, fb_vo_graphic_dcmp_info *);
    hi_bool (*hifb_drv_is_layer_support_ddr_detect)(hi_u32);
    hi_void (*hifb_drv_get_mddr_status)(hi_u32, hi_u32, hi_u32 *);
    hi_void (*hifb_drv_set_mddr_detect_zone)(hi_u32, hi_u32, fb_vo_mddrc_zone_addr *);
    hi_void (*hifb_drv_clear_ddr_dectect_zone)(hi_u32, hi_u32);
    hi_s32 (*hifb_drv_check_ddr_dectect_zone)
    (hi_u32, hi_u32, hi_u32, hi_u32, hi_u32);
    hi_void (*hifb_drv_get_ddr_zone_cfg)(hi_u32, hi_u32 *, hi_u32 *);
    hi_void (*hifb_drv_init_mddr)(hi_void);

    /* for GHDR */
    hi_bool (*hifb_drv_enable_ghdr)(hi_u32, hi_bool);

    /* for ZME */
    hi_bool (*hifb_drv_enable_zme)(hi_u32, HIFB_RECT *, HIFB_RECT *, hi_bool);
    hi_bool (*hifb_drv_is_layer_support_zoom_out)(hi_u32);
    /* for int */
    hi_bool (*hifb_drv_graphics_enable_int)(hi_u32, hi_bool);
    hi_bool (*hifb_drv_graphics_clear_int)(hi_u32, hi_s32);
    hi_bool (*hifb_drv_graphics_get_int)(hi_u32 *);
    hi_bool (*hifb_drv_graphics_clear_int_status)(hi_u32);
    hi_s32 (*hifb_drv_graphics_get_int_dev)(hi_u32, hifb_vo_dev *);
} hifb_drv_ops;

hi_void hifb_drv_get_ops(hifb_drv_ops *ops);
fb_vo_disp_pixel_format hifb_drv_convert_pixel_format(HIFB_COLOR_FMT_E color_format);
hi_u64 hifb_drv_get_dcmp_offset_addr(hi_u64 dcmp_addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __HIFB_DRV_H__ */
#endif /* __cplusplus */

#endif /* __HIFB_DRV_H__ */
