/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_vou_drv.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/fb.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include "hifb_vou_graphics.h"
#include "hi_tde_ext.h"
#include "mod_ext.h"

#define HIFB_MAX_LAYER_WIDTH_G0  1920
#define HIFB_MAX_LAYER_HEIGHT_G0 1080
#define HIFB_MIN_LAYER_WIDTH_G0  32
#define HIFB_MIN_LAYER_HEIGH_G0  32

#define GRAPHIC_ALPHA_OPACITY 0xff

char *apszLayerMmzNames[HIFB_MAX_LAYER_NUM] = { [0 ... HIFB_MAX_LAYER_NUM - 1] = HI_NULL };

module_param_array(apszLayerMmzNames, charp, HI_NULL, S_IRUGO);
MODULE_PARM_DESC(apszLayerMmzNames, "The mmz names for the graphics layers.");

/*
 * Prototype       : Variable Definition
 * Description     : hifb: a definition of the graphics layer capability set.
 */
const HIFB_CAPABILITY_S g_hifb_gfx_cap[HIFB_MAX_LAYER_NUM] = {
    /* G0 */
    {
        .bKeyRgb = 1,
        .bKeyAlpha = 0,
        .bGlobalAlpha = 1,
        .bCmap = 0,
        .bColFmt = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }, /* 25 */
        .u32MaxWidth = HIFB_MAX_LAYER_WIDTH_G0,
        .u32MaxHeight = HIFB_MAX_LAYER_HEIGHT_G0,
        .u32MinWidth = HIFB_MIN_LAYER_WIDTH_G0,
        .u32MinHeight = HIFB_MIN_LAYER_HEIGH_G0,
        .u32VDefLevel = 0,
        .u32HDefLevel = 0,
        .bLayerSupported = HI_TRUE,
        .bVoScale = HI_TRUE,
        .bHasCmapReg = 0,
        .bDcmp = 0,
        .bPreMul = 1,
        .bGHDR = 0,
    },
};

char *hifb_get_layer_mmz_names(hi_u32 layer_id)
{
    if (layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_NULL;
    }
    return apszLayerMmzNames[layer_id];
}

/*
 * the index of array means the number of framebuffer layer, and the value of the array means the maping
 * harware graphic layer
 * the index number of fb and the actual graphics layer
 * 0 - HAL_DISP_LAYER_GFX0
 * 1 - HAL_DISP_LAYER_GFX1
 * 2 - HAL_DISP_LAYER_GFX3
 */
static hal_disp_layer g_hifblayer_to_hwlayer[HIFB_MAX_LAYER_NUM] = {
    HAL_DISP_LAYER_GFX0,
};

hi_bool hifb_drv_support_rotate(hi_void)
{
    return HI_TRUE;
}

hi_bool is_hd_layer(hi_u32 layer_id)
{
    return (layer_id == 0);
}

hi_bool is_sd_layer(hi_u32 layer_id)
{
    return HI_FALSE;
}

hi_bool is_ad_layer(hi_u32 layer_id)
{
    return HI_FALSE;
}

hi_bool is_cursor_layer(hi_u32 layer_id)
{
    return (layer_id == 2); /* 2 is layer */
}

fb_vo_disp_pixel_format hifb_drv_convert_pixel_format(HIFB_COLOR_FMT_E color_format)
{
    switch (color_format) {
        case HIFB_FMT_ARGB8888:
            return HAL_INPUTFMT_ARGB_8888;
        case HIFB_FMT_ARGB1555:
            return HAL_INPUTFMT_ARGB_1555;
        default:
            return HAL_DISP_PIXELFORMAT_BUTT;
    }
}

/*
 * Frame decompression needs to be offset by 16 bytes;
 * line decompression does not need
 */
hi_u64 hifb_drv_get_dcmp_offset_addr(hi_u64 dcmp_addr)
{
    return dcmp_addr;
}

hi_s32 hifb_drv_enable_layer(hi_u32 layer_id, hi_bool enable)
{
    return vou_graphics_enable_layer(g_hifblayer_to_hwlayer[layer_id], enable);
}

/*
 * Name : hifb_drv_layer_default_setting
 * Desc : The default settings for the layer, including the following:
            1.Color key mode
            2.Bit extension mode
            3.alpha, global alpha,CSC,CSC enable, get interface type
 */
hi_s32 hifb_drv_layer_default_setting(hi_u32 layer_id)
{
    hal_disp_layer disp_layer = g_hifblayer_to_hwlayer[layer_id];

    if (g_hifb_gfx_cap[layer_id].bKeyRgb || g_hifb_gfx_cap[layer_id].bKeyAlpha) {
        vou_graphics_set_gfx_key_mode(disp_layer, FB_VOU_COLORKEY_IN);
    }

    vou_graphics_set_gfx_ext(disp_layer, FB_VOU_BITEXT_LOW_HIGHBITS);

    vou_graphics_set_gfx_palpha(disp_layer, HI_TRUE, HI_TRUE, GRAPHIC_ALPHA_OPACITY, GRAPHIC_ALPHA_OPACITY);

    vou_graphics_set_layer_galpha(disp_layer, GRAPHIC_ALPHA_OPACITY);

    vou_graphics_set_csc_coef(disp_layer);

    vou_graphics_set_csc_en(disp_layer, HI_TRUE);

    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_layer_addr(hi_u32 layer_id, hi_u64 addr)
{
    hi_bool status = vou_graphics_set_gfx_addr(g_hifblayer_to_hwlayer[layer_id], addr);
    if (status == HI_FALSE) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 hifb_drv_get_layer_addr(hi_u32 layer_id, hi_u64 *phys_addr)
{
    hi_bool status = vou_graphics_get_gfx_addr(g_hifblayer_to_hwlayer[layer_id], phys_addr);
    if (status == HI_FALSE) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_layer_stride(hi_u32 layer_id, hi_u32 stride)
{
    hi_bool status = vou_graphics_set_gfx_stride(g_hifblayer_to_hwlayer[layer_id], stride);
    if (status == HI_FALSE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_get_layer_stride_align(hi_u32 layer_id, hi_u32 *stride_align)
{
    if (stride_align != HI_NULL) {
        *stride_align = 16; /* 16 is stride align */
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_layer_alpha(hi_u32 layer_id, HIFB_ALPHA_S alpha)
{
    hi_u32 pre_mult_mode = 0;
    hal_disp_layer disp_layer;

    if (layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    disp_layer = g_hifblayer_to_hwlayer[layer_id];
    /* When the premultiply is enabled, the global alpha cannot be 1 */
    if (alpha.u8GlobalAlpha == 1) {
        if (vou_graphics_get_gfx_pre_mult(disp_layer, &pre_mult_mode) == HI_FALSE) {
            return HI_FAILURE;
        }
        if (pre_mult_mode == 1) {
            hi_trace_fb(HI_DBG_ERR, "the global alpha can not set to 1 when the pre-mult mode is enable\n");
            return HI_FAILURE;
        }
    }
    if (alpha.bAlphaChannel) {
        vou_graphics_set_layer_galpha(g_hifblayer_to_hwlayer[layer_id], alpha.u8GlobalAlpha);
    } else {
        vou_graphics_set_layer_galpha(g_hifblayer_to_hwlayer[layer_id], GRAPHIC_ALPHA_OPACITY);
    }

    vou_graphics_set_gfx_palpha(g_hifblayer_to_hwlayer[layer_id], alpha.bAlphaEnable, HI_TRUE, alpha.u8Alpha0,
                                alpha.u8Alpha1);

    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_layer_data_fmt(hi_u32 layer_id, HIFB_COLOR_FMT_E data_fmt)
{
    fb_vo_disp_pixel_format pixel_format_for_hal;
    hal_disp_layer disp_layer = g_hifblayer_to_hwlayer[layer_id];

    switch (data_fmt) {
        case HIFB_FMT_ARGB1555:
            pixel_format_for_hal = HAL_INPUTFMT_ARGB_1555;
            /* Pre-multiply enable is not supported when the data format is ARGB1555 */
            vou_graphics_set_gfx_pre_mult(disp_layer, HI_FALSE);
            break;
        case HIFB_FMT_ARGB8888:
            pixel_format_for_hal = HAL_INPUTFMT_ARGB_8888;
            break;
        case HIFB_FMT_ARGB4444:
            pixel_format_for_hal = HAL_INPUTFMT_ARGB_4444;
            break;
        default:
            hi_trace_fb(HI_DBG_ERR, "HiFB does not support this color format");
            return HI_FAILURE;
    }

    if (vou_graphics_set_layer_data_fmt(disp_layer, pixel_format_for_hal) == HI_FALSE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_layer_src_image_reso(hi_u32 disp_layer, const HIFB_RECT *hifb_rect)
{
    HIFB_RECT rect = { 0 };
    rect.x = hifb_rect->x;
    rect.y = hifb_rect->y;
    rect.w = hifb_rect->w;
    rect.h = hifb_rect->h;
    if (vou_graphics_set_layer_src_image_reso(g_hifblayer_to_hwlayer[disp_layer], &rect) == HI_FALSE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_layer_rect(hi_u32 layer_id, const HIFB_RECT *input_rect, const HIFB_RECT *output_rect)
{
    HIFB_RECT rect;
    fb_vou_scan_mode scan_mode = 0;
    hi_bool field_update = HI_FALSE;

    rect.x = output_rect->x;
    rect.y = output_rect->y;
    rect.w = output_rect->w;
    rect.h = output_rect->h;

    if (vou_graphics_get_dev_mode(g_hifblayer_to_hwlayer[layer_id], &scan_mode, &field_update) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    if (scan_mode == FB_VOU_SCAN_MODE_INTERLACE) {
        /* When interlaced display, the ordinate and height are required to be even */
        if ((((hi_u32)rect.y & 0x1) == 1) || (((hi_u32)rect.h & 0x1) == 1)) {
            hi_trace_fb(HI_DBG_ERR, "the height must be even for interlaced device\n");
            return HI_FAILURE;
        }
    }

    vou_graphics_set_layer_out_rect(g_hifblayer_to_hwlayer[layer_id], &rect);

    vou_graphics_set_layer_in_rect(g_hifblayer_to_hwlayer[layer_id], &rect);

    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_lay_key_mask(hi_u32 layer_id, const hifb_colorkeyex *colorkey)
{
    fb_vo_gfx_key_max key_max_rgb;
    fb_vo_gfx_key_min key_min_rgb;
    fb_vo_gfx_mask key_mask_rgb;
    hal_disp_layer disp_layer = g_hifblayer_to_hwlayer[layer_id];

    if (colorkey->key_enable) {
        if (vou_graphics_set_gfx_key_mode(disp_layer, colorkey->key_mode) == HI_FALSE) {
            return HI_FAILURE;
        }

        key_max_rgb.key_max_r = colorkey->red_max;
        key_max_rgb.key_max_g = colorkey->green_max;
        key_max_rgb.key_max_b = colorkey->blue_max;

        key_min_rgb.key_min_r = colorkey->red_min;
        key_min_rgb.key_min_g = colorkey->green_min;
        key_min_rgb.key_min_b = colorkey->blue_min;

        vou_graphics_set_color_key_value(disp_layer, key_max_rgb, key_min_rgb);
        if (colorkey->mask_enable) {
            key_mask_rgb.mask_r = colorkey->red_mask;
            key_mask_rgb.mask_g = colorkey->green_mask;
            key_mask_rgb.mask_b = colorkey->blue_mask;
        } else {
            key_mask_rgb.mask_r = 0;
            key_mask_rgb.mask_g = 0;
            key_mask_rgb.mask_b = 0;
        }
        vou_graphics_set_color_key_mask(disp_layer, key_mask_rgb);
    }

    if (vou_graphics_set_gfx_key_en(disp_layer, colorkey->key_enable) == HI_FALSE) {
        return HI_FAILURE;
    }

    return HI_FAILURE;
}

hi_s32 hifb_drv_color_convert(const struct fb_var_screeninfo *var, hifb_colorkeyex *ckey)
{
    hi_u8 r, g, b;
    hi_u32 key = ckey->key;
    hi_u8 r_mask, g_mask, b_mask;

    r = (key >> var->red.offset) << (8 - var->red.length);     /* 8 flip data */
    g = (key >> var->green.offset) << (8 - var->green.length); /* 8 flip data */
    b = (key >> var->blue.offset) << (8 - var->blue.length);   /* 8 flip data */

    r_mask = (0xff >> var->red.length);   /* 0xff bit */
    g_mask = (0xff >> var->green.length); /* 0xff bit */
    b_mask = (0xff >> var->blue.length);  /* 0xff bit */

    ckey->blue_min = b;
    ckey->green_min = g;
    ckey->red_min = r;

    ckey->blue_max = b | b_mask;
    ckey->green_max = g | b_mask;
    ckey->red_max = r | b_mask;

    /* low bits not using when do compare, after extend bits */
    ckey->red_mask = ~r_mask;
    ckey->green_mask = ~g_mask;
    ckey->blue_mask = ~b_mask;

    ckey->key_mode = 0;

    ckey->mask_enable = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 hifb_drv_updata_layer_reg(hi_u32 layer_id)
{
    hi_bool status = vou_graphics_set_reg_up(g_hifblayer_to_hwlayer[layer_id]);
    if (status != HI_TRUE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_gamma_enable(hi_u32 layer_id, hi_bool enable)
{
    return HI_FAILURE;
}

hi_s32 hifb_drv_set_layer_de_flicker(hi_u32 layer_id, HIFB_DEFLICKER_S de_flicker)
{
    return HI_FAILURE;
}

hi_s32 hifb_drv_set_color_reg(hi_u32 layer_id, hi_u32 off_set, hi_u32 color, hi_s32 up_flag)
{
    return HI_FAILURE;
}

hi_s32 hifb_drv_init(hi_void)
{
    hi_s32 ret;
    hi_tde_export_func *tde_export_func = HI_NULL;

    tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
    if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_open == HI_NULL)) {
        hi_trace_fb(HI_DBG_ERR, "TDE pTdeExportFunc is HI_NULL!\n");
    } else {
        ret = tde_export_func->drv_tde_module_open();
        if (ret) {
            hi_trace_fb(HI_DBG_ERR, "TDE open failed\n");
            return ret;
        }
    }

    ret = vou_graphics_resource_init();
    if (ret) {
        hi_trace_fb(HI_DBG_ERR, "resource init failed\n");
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 hifb_drv_deinit(hi_void)
{
    hi_tde_export_func *tde_export_func = HI_NULL;

    vou_graphics_resource_deinit();

    tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
    if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_close == HI_NULL)) {
        hi_trace_fb(HI_DBG_ERR, "TDE pTdeExportFunc is HI_NULL!\n");
        return HI_FAILURE;
    }
    tde_export_func->drv_tde_module_close();

    return HI_SUCCESS;
}

/* set premul data */
hi_s32 hifb_drv_set_pre_mul(hi_u32 layer_id, hi_bool premul)
{
    hal_disp_pixel_format pixel_format = HAL_INPUTFMT_ARGB_1555;
    hal_disp_layer layer = g_hifblayer_to_hwlayer[layer_id];
    hi_u8 global_alpha;

    if (premul == HI_TRUE) {
        /* When the global alpha configuration is 1, pre-multiplication cannot be enabled. */
        if (vou_graphics_get_layer_galpha(layer, &global_alpha) != HI_TRUE) {
            return HI_FAILURE;
        }
        if (global_alpha == 1) {
            hi_trace_fb(HI_DBG_ERR, "not allow to set pre-mult mode when the GlobalAlpha is 1\n");
            return HI_FAILURE;
        }

        /* Pre-multiply enable is not supported when the data format is ARGB1555 */
        if (vou_graphics_get_layer_data_fmt(layer, &pixel_format) != HI_TRUE) {
            return HI_FAILURE;
        }
        if ((pixel_format == HAL_INPUTFMT_ARGB_1555) || (HAL_INPUTFMT_ARGB_4444 == pixel_format)) {
            hi_trace_fb(HI_DBG_ERR, "not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
            return HI_FAILURE;
        }
    }

    if (vou_graphics_set_gfx_pre_mult(layer, premul) != HI_TRUE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_clut_addr(hi_u32 layer_id, hi_u32 phy_addr)
{
    return HI_FAILURE;
}

hi_s32 hifb_drv_set_int_callback(fb_vo_int_type int_type, int_callback call_back, hi_u32 layer_id,
                                 hi_void *call_back_arg)
{
    if (layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    return vou_graphics_set_callback(g_hifblayer_to_hwlayer[layer_id], int_type, call_back, call_back_arg);
}

/*
 * The premise is that the VO device is enabled and an error is returned when the VO device is not enabled.
 * For X5HD, the current configuration content of the graphics layer is obtained for the boot screen;
 * for the 3531, since the boot screen is not required, only the device resolution and scan information
 * can be obtained.
 */
hi_s32 hifb_drv_get_osd_data(hi_u32 layer_id, hifb_osd_data *layer_data)
{
    hi_u32 width = 0;
    hi_u32 height = 0;
    hi_s32 ret;
    hal_disp_layer layer = g_hifblayer_to_hwlayer[layer_id];
    fb_vou_scan_mode scan_mode = 0;

    ret = vou_graphics_get_intf_size(layer, &width, &height);
    if (ret != HI_SUCCESS) {
        hi_trace_fb(HI_DBG_ERR, "Failed to get screen width and height!\n");
        return HI_FAILURE;
    }

    ret = vou_graphics_get_gfx_addr(layer, &layer_data->buffer_phy_addr);
    if (ret != HI_TRUE) {
        return HI_FAILURE;
    }

    ret = vou_graphics_get_gfx_stride(layer, &layer_data->stride);
    if (ret != HI_TRUE) {
        return HI_FAILURE;
    }

    ret = vou_graphics_get_dev_mode(layer, &scan_mode, &layer_data->feild_update);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }
    if (scan_mode == FB_VOU_SCAN_MODE_INTERLACE) {
        layer_data->scan_mode = HIFB_SCANMODE_I;
    } else {
        layer_data->scan_mode = HIFB_SCANMODE_P;
    }

    layer_data->screen_height = height;
    layer_data->screen_width = width;

    return HI_SUCCESS;
}

hi_s32 hifb_open_layer(hi_u32 layer_id)
{
    return vou_graphics_open_layer(g_hifblayer_to_hwlayer[layer_id]);
}

hi_s32 hifb_close_layer(hi_u32 layer_id)
{
    return vou_graphics_close_layer(g_hifblayer_to_hwlayer[layer_id]);
}

/*
 * open vo & close vo -- For internal debugging only
 * (for the API that does not adjust VO, the device can display the graphics layer)
 */
hi_s32 hifb_drv_open_display(hi_void)
{
    return HI_SUCCESS;
}

/* close vou */
hi_s32 hifb_drv_close_display(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 tde_cal_scale_rect_hifb(const hi_tde_rect *src_rect, const hi_tde_rect *dst_rect, hi_tde_rect *rect_in_src,
                               hi_tde_rect *rect_in_dst)
{
    rect_in_dst->pos_x = rect_in_src->pos_x * dst_rect->width / src_rect->width;
    rect_in_dst->pos_y = rect_in_src->pos_y * dst_rect->height / src_rect->height;
    rect_in_dst->height = dst_rect->height;
    rect_in_dst->width = dst_rect->width;

    return HI_SUCCESS;
}

hi_bool hifb_drv_enable_dcmp(hi_u32 layer_id, hi_bool enable)
{
    return HI_TRUE;
}

hi_bool hifb_drv_get_dcmp_enable_state(hi_u32 layer_id, hi_bool *enable)
{
    return HI_TRUE;
}

hi_bool hifb_drv_set_dcmp_info(hi_u32 layer_id, fb_vo_graphic_dcmp_info *dcmp_info)
{
    return HI_TRUE;
}

hi_bool hifb_drv_enable_ghdr(hi_u32 layer_id, hi_bool enable)
{
    return HI_SUCCESS;
}

hi_bool hifb_drv_enable_zme(hi_u32 layer_id, HIFB_RECT *in_rect, HIFB_RECT *out_rect, hi_bool enable)
{
    RECT_S in;
    RECT_S out;

    in.u32Width = in_rect->w;
    in.u32Height = in_rect->h;
    out.u32Width = out_rect->w;
    out.u32Height = out_rect->h;

    if (g_hifblayer_to_hwlayer[layer_id] == HAL_DISP_LAYER_GFX0) {
        return vou_graphics_enable_zme(g_hifblayer_to_hwlayer[layer_id], &in, &out, enable);
    } else {
        if (enable) {
            hi_trace_fb(HI_DBG_ERR, "G%u does not support zme.\n", layer_id);
        }
        return HI_TRUE;
    }
}

hi_bool hifb_drv_is_layer_support_zoom_out(hi_u32 layer_id)
{
    return HI_FALSE;
}

hi_void hifb_drv_enable_wbc_int(hi_u32 layer_id, hi_bool enable)
{
    return;
}

hi_void hifb_drv_set_wbc_info(hi_u32 layer_id, fb_vo_graphic_wbc_info *wbc_info)
{
    return;
}

hi_void hifb_drv_enable_wbc(hi_u32 layer_id, hi_bool enable)
{
    return;
}

hi_void hifb_drv_get_wbc_en_state(hi_u32 layer_id, hi_bool *wbc_en)
{
    return;
}

hi_void hifb_drv_get_wbc_stop_state(hi_u32 layer_id, hi_bool *wbc_stop)
{
    return;
}

#ifdef MDDRDETECT
static hi_bool hifb_drv_is_layer_support_ddrdetect(hi_u32 layer_id)
{
    return HI_FALSE;
}

static hi_void hifb_drv_set_ddr_detect_zone(hi_u32 start_id, hi_u32 cnt, fb_vo_mddrc_zone_addr *zone_addr)
{
    return;
}

static hi_void hifb_drv_get_mddr_status(hi_u32 start_id, hi_u32 cnt, hi_u32 *status)
{
    return;
}

hi_void hifb_drv_clear_ddrdectect_zone(hi_u32 start_id, hi_u32 cnt)
{
    return;
}

hi_s32 hifb_drv_check_ddr_dectect_zone(hi_u32 layer_id, hi_u32 new_start_id, hi_u32 new_cnt, hi_u32 pre_start_id,
                                       hi_u32 pre_cnt)
{
    return HI_FAILURE;
}

hi_void hifb_drv_get_ddr_zone_cfg(hi_u32 layer_id, hi_u32 *start_id, hi_u32 *cnt)
{
    return;
}
#endif

hi_bool hifb_drv_graphics_enable_int(hi_u32 layer_id, hi_bool enable)
{
    return (vou_graphics_enable_int(layer_id, enable) == HI_SUCCESS ? HI_TRUE : HI_FALSE);
}

hi_bool hifb_drv_graphics_clear_int(hi_u32 int_clear, hi_s32 irq)
{
    return vou_graphics_clear_int(int_clear, irq);
}

hi_bool hifb_drv_graphics_get_int(hi_u32 *int_status)
{
    return vou_graphics_get_int(int_status);
}

hi_bool hifb_drv_graphics_clear_int_status(hi_u32 int_status)
{
    return vou_graphics_clear_int_status(int_status);
}

hi_s32 hifb_drv_graphics_get_interrupt_dev(hi_u32 int_status, hifb_vo_dev *vo_dev)
{
    return vou_graphics_get_interrupt_dev(int_status, vo_dev);
}

#ifdef MDDRDETECT
static hi_void get_ddr_detect_ops(hifb_drv_ops *ops)
{
    ops->hifb_drv_is_layer_support_ddr_detect = hifb_drv_is_layer_support_ddrdetect;
    ops->hifb_drv_get_mddr_status = hifb_drv_get_mddr_status;
    ops->hifb_drv_set_mddr_detect_zone = hifb_drv_set_ddr_detect_zone;
    ops->hifb_drv_clear_ddr_dectect_zone = hifb_drv_clear_ddrdectect_zone;
    ops->hifb_drv_check_ddr_dectect_zone = hifb_drv_check_ddr_dectect_zone;
    ops->hifb_drv_get_ddr_zone_cfg = hifb_drv_get_ddr_zone_cfg;
    ops->hifb_drv_init_mddr = vou_graphics_init_mddr_detect;
    return;
}
#endif

static hi_void get_implement_ops(hifb_drv_ops *ops)
{
    ops->hifb_drv_init = hifb_drv_init;
    ops->hifb_drv_deinit = hifb_drv_deinit;
    ops->hifb_drv_enable_layer = hifb_drv_enable_layer;
    ops->hifb_drv_layer_default_setting = hifb_drv_layer_default_setting;
    ops->hifb_drv_set_layer_addr = hifb_drv_set_layer_addr;
    ops->hifb_drv_get_layer_addr = hifb_drv_get_layer_addr;
    ops->hifb_drv_set_layer_stride = hifb_drv_set_layer_stride;
    ops->hifb_drv_set_layer_alpha = hifb_drv_set_layer_alpha;
    ops->hifb_drv_set_layer_data_fmt = hifb_drv_set_layer_data_fmt;
    ops->hifb_drv_set_layer_rect = hifb_drv_set_layer_rect;
    ops->hifb_drv_set_layer_src_image_reso = hifb_drv_set_layer_src_image_reso;
    ops->hifb_drv_set_layer_key_mask = hifb_drv_set_lay_key_mask;
    ops->hifb_drv_color_convert = hifb_drv_color_convert;
    ops->hifb_drv_updata_layer_reg = hifb_drv_updata_layer_reg;
    ops->hifb_drv_set_int_callback = hifb_drv_set_int_callback;
    ops->hifb_drv_set_layer_de_flicker = hifb_drv_set_layer_de_flicker;
    ops->hifb_drv_set_color_reg = hifb_drv_set_color_reg;
    ops->hifb_drv_set_pre_mul = hifb_drv_set_pre_mul;
    ops->hifb_drv_set_clut_addr = hifb_drv_set_clut_addr;
    ops->hifb_drv_get_osd_data = hifb_drv_get_osd_data;
    ops->hifb_drv_open_display = hifb_drv_open_display;
    ops->hifb_drv_close_display = hifb_drv_close_display;
    ops->hifb_open_layer = hifb_open_layer;
    ops->hifb_close_layer = hifb_close_layer;

    ops->hifb_drv_enable_wbc_int = hifb_drv_enable_wbc_int;
    ops->hifb_drv_get_wbc_stop_state = hifb_drv_get_wbc_stop_state;
    ops->hifb_drv_enable_wbc = hifb_drv_enable_wbc;
    ops->hifb_drv_set_wbc_info = hifb_drv_set_wbc_info;
    ops->hifb_drv_get_wbc_en_state = hifb_drv_get_wbc_en_state;
    ops->hifb_drv_set_dcmp_info = hifb_drv_set_dcmp_info;
    ops->hifb_drv_enable_dcmp = hifb_drv_enable_dcmp;
    ops->hifb_drv_get_dcmp_enable_state = hifb_drv_get_dcmp_enable_state;
    return;
}

hi_void hifb_drv_get_ops(hifb_drv_ops *ops)
{
    ops->layer_count = HIFB_MAX_LAYER_NUM;
    ops->capability = (HIFB_CAPABILITY_S *)g_hifb_gfx_cap;

    /* implement */
    get_implement_ops(ops);
#ifdef MDDRDETECT
    get_ddr_detect_ops(ops);
#endif
    /* For GHDR */
    ops->hifb_drv_enable_ghdr = hifb_drv_enable_ghdr;
    /* for zme */
    ops->hifb_drv_enable_zme = hifb_drv_enable_zme;
    ops->hifb_drv_is_layer_support_zoom_out = hifb_drv_is_layer_support_zoom_out;
    /* for int */
    ops->hifb_drv_graphics_enable_int = hifb_drv_graphics_enable_int;
    ops->hifb_drv_graphics_clear_int = hifb_drv_graphics_clear_int;
    ops->hifb_drv_graphics_get_int = hifb_drv_graphics_get_int;
    ops->hifb_drv_graphics_clear_int_status = hifb_drv_graphics_clear_int_status;
    ops->hifb_drv_graphics_get_int_dev = hifb_drv_graphics_get_interrupt_dev;
}
