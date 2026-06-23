/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef GFBG_DRV_H
#define GFBG_DRV_H
#ifdef __LITEOS__
#include <fb.h>
#endif
#include "gfbg.h"
#include "ot_debug.h"
#include "gfbg_graphics_drv.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __LITEOS__
typedef struct {
    struct gfbg_info *info;
    td_ulong layer_size;
    td_u32 curosr_buf_size; /* For soft cursor */
} gfbg_layer;

#else
typedef struct {
    struct fb_info *info;
    td_ulong layer_size;       /* layer_size = fb.smem_len, For display buf, KB */
    td_u32 curosr_buf_size; /* For soft cursor */
} gfbg_layer;
#endif

td_s32 gfbg_vou_get_dev_id(td_u32 layer_id);

typedef td_s32 (*int_callback)(const td_void *paraml, const td_void *paramr);
typedef td_s32 (*int_vo_callback)(const td_void *paraml, ot_vo_dev vo_dev, const td_void *paramr);

#define gfbg_debug(fmt, ...)                                                         \
    OT_DEBUG_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define gfbg_fatal(fmt, ...)  \
    OT_EMERG_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define gfbg_error(fmt, ...)  \
    OT_ERR_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define gfbg_warning(fmt, ...) \
    OT_WARN_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define gfbg_info(fmt, ...)   \
    OT_INFO_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

typedef td_u32 gfbg_layer_id; /* start from 0 */
hal_disp_layer gfbg_drv_gfbglayer_to_hwlayer(td_u32 layer_id);

td_bool is_4k_layer(td_u32 layer_id);
td_bool is_hd_layer(td_u32 layer_id);
td_bool is_sd_layer(td_u32 layer_id);
td_bool is_ad_layer(td_u32 layer_id);
td_bool is_cursor_layer(td_u32 layer_id);
td_bool is_layer_support_low_delay(td_u32 layer_id);

typedef struct {
    td_bool key_enable;  /* colorkey */
    td_bool mask_enable; /* key mask */
    td_u32 key;
    td_u8 red_mask;   /* red mask */
    td_u8 green_mask; /* green mask */
    td_u8 blue_mask;  /* blue mask */
    td_u8 reserved;
    td_u32 key_mode; /* 0:In region; 1:Out region */
    td_u8 red_max;    /* colorkey red max */
    td_u8 green_max;  /* colorkey green max */
    td_u8 blue_max;   /* colorkey blue max */
    td_u8 reserved1;
    td_u8 red_min;   /* colorkey red min */
    td_u8 green_min; /* colorkey green min */
    td_u8 blue_min;  /* colorkey blue min */
    td_u8 reserved2;
} gfbg_colorkeyex;

typedef struct {
    ot_fb_rect clip_rect;
    td_bool in_region_clip;
    td_bool clip;
} gfbg_clip;

typedef struct {
    ot_fb_alpha alpha;
    gfbg_colorkeyex ckey;
    gfbg_clip clip;
    ot_fb_layer_antiflicker_level antiflicker_level;
    td_bool scale;
    td_bool block;
    td_bool is_sync;
    td_bool call_back;
    td_void *param;
    td_phys_addr_t cmap_addr;
    td_bool soft_cursor_update;
    ot_fb_mirror_mode mirror_mode;
    td_bool compress;
}gfbg_blit_opt;

/* for graphic rotation. */
typedef struct {
    td_bool block;
    td_bool call_back;
    td_void *param;
    ot_fb_rotate_mode rotate_mode;
}gfbg_rotate_opt;

typedef enum {
    GFBG_SCANMODE_P,
    GFBG_SCANMODE_I,
    GFBG_SCANMODE_BUTT,
} gfbg_scan_mode;

/* layer state */
typedef enum {
    GFBG_LAYER_STATE_ENABLE = 0x0,

    GFBG_LAYER_STATE_DISABLE,

    GFBG_LAYER_STATE_INVALID,

    GFBG_LAYER_STATE_BUTT
} gfbg_layer_state;

typedef struct {
    gfbg_scan_mode scan_mode;
    td_u32 screen_width;
    td_u32 screen_height;
} disp_info;

typedef struct {
    td_phys_addr_t buffer_phy_addr;
    td_u32 stride;
    td_bool feild_update; /* field update mode or not */
    gfbg_scan_mode scan_mode;
    td_u32 screen_width;  /* current sync width */
    td_u32 screen_height; /* current sync height */
} gfbg_osd_data;

/* mask bit */
typedef enum {
    GFBG_LAYER_PARAMODIFY_FMT              = 0x1,   /* color format */
    GFBG_LAYER_PARAMODIFY_STRIDE           = 0x2,   /* stride, line spacing */
    GFBG_LAYER_PARAMODIFY_ALPHA            = 0x4,   /* alpha */
    GFBG_LAYER_PARAMODIFY_COLORKEY         = 0x8,   /* colorkey */
    GFBG_LAYER_PARAMODIFY_INRECT           = 0x10,  /* input rect */
    GFBG_LAYER_PARAMODIFY_OUTRECT          = 0x20,  /* output rect */
    GFBG_LAYER_PARAMODIFY_DISPLAYADDR      = 0x40,  /* display addr */
    GFBG_LAYER_PARAMODIFY_SHOW             = 0x80,  /* show or hide */
    GFBG_LAYER_PARAMODIFY_BMUL             = 0x100, /* be pre-multi data */
    GFBG_LAYER_PARAMODIFY_ANTIFLICKERLEVEL = 0x200, /* ANTIFLICKERLEVEL */
    GFBG_LAYER_PARAMODIFY_DYNAMICRANGE     = 0x400, /* DYNAMICRANGE */
    GFBG_LAYER_PARAMODIEY_SMART_RECT       = 0x800, /* SMART RECT */
    GFBG_LAYER_PARAMODIEY_CLUT_UP          = 0x1000, /* CLUT UP */
    GFBG_LAYER_PARAMODIEY_COMPRESS         = 0x2000, /* compress */
    GFBG_LAYER_PARAMODIFY_BUTT
} gfbg_layer_paramodify_maskbit;

typedef struct {
    td_u32 phy_addr;
    ot_fb_color_format color_format;
    ot_fb_size resolution;
    td_u32 stride;
} gfbg_vcmp_inbufinfo;

typedef struct {
    td_u32 bank_width;
    td_u32 stride;
    td_u32 alpha_addr;
    td_u32 red_addr;
    td_u32 green_addr;
    td_u32 blue_addr;
} gfbg_vcmp_outbufinfo;

typedef enum {
    GFBG_INPUTFMT_CLUT_2BPP = 0x10,
    GFBG_INPUTFMT_CLUT_4BPP = 0x20,
    GFBG_INPUTFMT_ARGB_4444 = 0x48,
    GFBG_INPUTFMT_ARGB_1555 = 0x49,
    GFBG_INPUTFMT_ARGB_8888 = 0x68,

    GFBG_DISP_PIXELFORMAT_BUTT
} gfbg_disp_pixel_format;

typedef struct {
    gfbg_disp_pixel_format pixel_fmt;
    td_phys_addr_t ar_phy_addr;
    td_phys_addr_t gb_phy_addr;
    td_u32 frame_size0;
    td_u32 frame_size1;
    td_u32 width;
    td_u32 height;
    td_bool is_lossless_a;
    td_bool is_lossless;
    td_u32 offset;
    td_u32 stride;
} gfbg_graphic_dcmp_info;

typedef enum {
    GFBG_INTTYPE_VO,      /* Vertical timing interrupt */
    GFBG_INTTYPE_VO_DISP, /* Timing switch notification */
    GFBG_INTTYPE_WBC,     /* Compression completion interrupt */
    GFBG_INTTYPE_BUTT,
} gfbg_int_type;

typedef struct {
    td_u32 safe_dist;
    td_bool is_hw_mute_clr_en;
    td_bool is_mute_en;
    td_bool is_sync;
} gfbg_bind_tde_cfg;

typedef struct {
    td_bool is_sync;
    td_u32  safe_dist;
} gfbg_sync_attr;

typedef struct {
    const char *chip_name;          /* chip name */
    td_u32 layer_count;             /* support how many layers */
    ot_fb_capability *capability;    /* capability */

    /* init DRV */
    td_s32 (*gfbg_drv_init)(td_void);

    /* De init DRV */
    td_s32 (*gfbg_drv_deinit)(td_void);
    td_s32 (*gfbg_drv_layer_default_setting)(td_u32 layer_id);

    /* enable/disable layer */
    td_s32 (*gfbg_drv_set_layer_enable)(td_u32 layer_id, td_bool enable);

    /* enable/disable layer */
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    td_s32 (*gfbg_drv_get_layer_enable)(td_u32 layer_id, td_bool *enable);
#endif

    /* set layer address */
    td_s32 (*gfbg_drv_set_layer_addr)(td_u32 layer_id, td_phys_addr_t addr);

    /* get layer address */
    td_s32 (*gfbg_drv_get_layer_addr)(td_u32 layer_id, td_phys_addr_t *phys_addr);

    /* set layer stride */
    td_s32 (*gfbg_drv_set_layer_stride)(td_u32 layer_id, td_u32 stride);

    /* set layer pixel format */
    td_s32 (*gfbg_drv_set_layer_data_fmt)(td_u32 layer_id, ot_fb_color_format data_fmt);

    /* set color registers */
    td_s32 (*gfbg_drv_set_color_reg)(td_u32 layer_id, td_u32 offset, td_u32 color, td_s32 up_flag);

    /* set color registers up */
    td_void (*gfbg_drv_set_color_reg_up)(td_u32 layer_id);

    /* wait until vblank, it's a block interface */
    td_s32 (*gfbg_drv_wait_v_blank)(td_u32 layer_id);

    /* set layer alpha */
    td_s32 (*gfbg_drv_set_layer_alpha)(td_u32 layer_id, ot_fb_alpha alpha);

    /* set layer start position and size */
    td_s32 (*gfbg_drv_set_layer_rect)(td_u32 layer_id, const ot_fb_rect *input_rect, const ot_fb_rect *output_rect);

    /* set layer src image resolution */
    td_s32 (*gfbg_drv_set_layer_src_image_reso)(td_u32 layer_id, const ot_fb_rect *rect);

    /* other color format convert to RGB888 format */
#ifdef __LITEOS__
    td_s32 (*gfbg_drv_color_convert)(const struct gfbg_info *info, gfbg_colorkeyex *ckey);
#else
    td_s32 (*gfbg_drv_color_convert)(const struct fb_var_screeninfo *var, gfbg_colorkeyex *ckey);
#endif
    /* set layer colorkey */
    td_s32 (*gfbg_drv_set_layer_key_mask)(td_u32 layer_id, const gfbg_colorkeyex *color_key);

    /* update layer register */
    td_s32 (*gfbg_drv_updata_layer_reg)(td_u32 layer_id);

    /* set premul data */
    td_s32 (*gfbg_drv_set_pre_mul)(td_u32 layer_id, td_bool is_premul);

    td_s32 (*gfbg_drv_set_clut_addr)(td_u32 layer_id, td_u32 phy_data);
    /* get osd data */
    td_s32 (*gfbg_drv_get_osd_data)(td_u32 layer_id, gfbg_osd_data *layer_data);
    /* register call back function */
    td_s32 (*gfbg_drv_set_int_callback)(gfbg_int_type eIntType, int_vo_callback call_back, td_u32 layer_id,
                                        td_void *call_back_arg);
    /* open */
    td_s32 (*gfbg_drv_open_display)(td_void);

    /* close */
    td_s32 (*gfbg_drv_close_display)(td_void);

    td_s32 (*gfbg_open_layer)(td_u32 layer_id);
    td_s32 (*gfbg_close_layer)(td_u32 layer_id);

    /* for compression */
    td_s32 (*gfbg_drv_enable_dcmp)(td_u32 layer_id, td_bool enable);
    td_s32 (*gfbg_drv_get_dcmp_enable_state)(td_u32 layer_id, td_bool *enable);
    td_s32 (*gfbg_drv_set_dcmp_info)(td_u32 layer_id, gfbg_graphic_dcmp_info *dcmp_info);

    /* for ZME */
    td_s32 (*gfbg_drv_enable_zme)(td_u32 layer_id, const ot_fb_rect *in_rect, const ot_fb_rect *out_rect,
        td_bool enable);
    td_bool (*gfbg_drv_is_layer_support_zoom_out)(td_u32 layer_id);
    /* for int */
    td_bool (*gfbg_drv_graphics_enable_int)(td_u32 layer_id, td_bool enable);
    td_bool (*gfbg_drv_graphics_clear_int)(td_u32 int_clear, td_s32 irq);
    td_s32 (*gfbg_drv_graphics_get_int)(td_u32 *int_status);
    td_bool (*gfbg_drv_graphics_clear_int_status)(td_u32 int_status);
    td_s32  (*gfbg_drv_graphics_get_int_dev)(td_u32 int_status, ot_vo_dev* dev);
    /* for LOW DELAY */
    td_void (*gfbg_drv_set_tde_sync)(td_u32 layer_id, const gfbg_sync_attr *sync_info);
    /* for smart rect */
    td_s32 (*gfbg_drv_set_smart_rect)(td_u32 layer_id, td_u32 max_width, td_u32 max_height,
        const ot_fb_smart_rect_param *param, const gfbg_mmz_buffer *osb_chn);
    td_void (*gfbg_drv_smart_rect_up_param)(td_u32 layer_id);
    td_void (*gfbg_close_smart_rect)(td_u32 layer_id);
} gfbg_drv_ops;

td_void gfbg_drv_get_ops(gfbg_drv_ops *ops);
gfbg_disp_pixel_format gfbg_drv_convert_pixel_format(ot_fb_color_format color_format);
td_phys_addr_t gfbg_drv_get_dcmp_offset_addr(td_phys_addr_t dcmp_addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif /* __cplusplus */

#endif  /* GFBG_DRV_H */
