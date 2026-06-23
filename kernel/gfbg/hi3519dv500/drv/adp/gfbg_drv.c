/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include "gfbg_drv.h"
#include "mod_ext.h"
#include "securec.h"
#include "drv_tde.h"
#include "gfbg_graphics.h"
#include "gfbg_comm.h"

#ifdef CONFIG_GFBG_G0_FHD_SUPPORT
#define GFBG_MAX_LAYER_WIDTH_G0  1920
#ifdef CONFIG_GFBG_RESOLUTION_SUPPORT_SQUARE
#define GFBG_MAX_LAYER_HEIGHT_G0 1920
#else
#define GFBG_MAX_LAYER_HEIGHT_G0 1080
#endif
#else
#define GFBG_MAX_LAYER_WIDTH_G0  3840
#define GFBG_MAX_LAYER_HEIGHT_G0 2160
#endif
#define GFBG_MIN_LAYER_WIDTH_G0  32
#define GFBG_MIN_LAYER_HEIGH_G0  32

#ifdef CONFIG_GFBG_G1_4K_SUPPORT
#define GFBG_MAX_LAYER_WIDTH_G1  3840
#define GFBG_MAX_LAYER_HEIGTH_G1 2160
#else
#define GFBG_MAX_LAYER_WIDTH_G1  1920
#define GFBG_MAX_LAYER_HEIGTH_G1 1080
#endif
#define GFBG_MIN_LAYER_WIDTH_G1  32
#define GFBG_MIN_LAYER_HEIGTH_G1 32

#define GFBG_MAX_LAYER_WIDTH_G2  256
#define GFBG_MAX_LAYER_HEIGTH_G2 256
#define GFBG_MIN_LAYER_WIDTH_G2  2
#define GFBG_MIN_LAYER_HEIGTH_G2 2

#ifdef CONFIG_GFBG_G3_960_SUPPORT
#define GFBG_MAX_LAYER_WIDTH_G3  960
#define GFBG_MAX_LAYER_HEIGTH_G3 576
#else
#define GFBG_MAX_LAYER_WIDTH_G3  720
#define GFBG_MAX_LAYER_HEIGTH_G3 576
#endif
#define GFBG_MIN_LAYER_WIDTH_G3  2
#define GFBG_MIN_LAYER_HEIGTH_G3 2

#define GRAPHICS_LAYER_G0        0
#define GRAPHICS_LAYER_G1        1
#define GRAPHICS_LAYER_G2        2
#define GRAPHICS_LAYER_G3        3
#define GRAPHICS_LAYER_G4        4

ot_fb_capability g_gfbg_gfx_cap[GFBG_MAX_LAYER_NUM] = {
    /* G0 */
    {
        .is_key_rgb = 1,
        .is_key_alpha = 0,
        .is_global_alpha = 1,
        .is_cmap = 0,
        .is_color_format = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .max_width = GFBG_MAX_LAYER_WIDTH_G0,
        .max_height = GFBG_MAX_LAYER_HEIGHT_G0,
        .min_width = GFBG_MIN_LAYER_WIDTH_G0,
        .min_height = GFBG_MIN_LAYER_HEIGH_G0,
        .ver_deflicker_level = 0,
        .hor_deflicker_level = 0,
        .is_layer_support = TD_TRUE,
        .is_vo_scale = TD_TRUE,
        .has_cmap_reg = 0,
        .is_decompress = 1,
        .is_premul = 1,
        .is_ghdr = 0,
        .is_osb = 0,
    },
    /* G1 */
    {
        .is_key_rgb = 1,
        .is_key_alpha = 0,
        .is_global_alpha = 1,
        .is_cmap = 0,
        .is_color_format = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .max_width = GFBG_MAX_LAYER_WIDTH_G1,
        .max_height = GFBG_MAX_LAYER_HEIGTH_G1,
        .min_width = GFBG_MIN_LAYER_WIDTH_G1,
        .min_height = GFBG_MIN_LAYER_HEIGTH_G1,
        .ver_deflicker_level = 0,
        .hor_deflicker_level = 0,
        .is_layer_support = TD_TRUE,
        .is_vo_scale = TD_FALSE,
        .has_cmap_reg = 0,
        .is_decompress = 1,
        .is_premul = 1,
        .is_ghdr = 0,
        .is_osb = 0,
    },
    {
        .is_key_rgb = 1,
        .is_key_alpha = 0,
        .is_global_alpha = 1,
        .is_cmap = 0,
        .is_color_format = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .max_width = GFBG_MAX_LAYER_WIDTH_G2,
        .max_height = GFBG_MAX_LAYER_HEIGTH_G2,
        .min_width = GFBG_MIN_LAYER_WIDTH_G2,
        .min_height = GFBG_MIN_LAYER_HEIGTH_G2,
        .ver_deflicker_level = 0,
        .hor_deflicker_level = 0,
        .is_layer_support = TD_TRUE,
        .is_vo_scale = TD_FALSE,
        .has_cmap_reg = 0,
        .is_decompress = 0,
        .is_premul = 1,
        .is_ghdr = 0,
        .is_osb = 0,
    },
    /* G3 */
    {
        .is_key_rgb = 1,
        .is_key_alpha = 0,
        .is_global_alpha = 1,
        .is_cmap = 1,
        .is_color_format = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .max_width = GFBG_MAX_LAYER_WIDTH_G3,
        .max_height = GFBG_MAX_LAYER_HEIGTH_G3,
        .min_width = GFBG_MIN_LAYER_WIDTH_G3,
        .min_height = GFBG_MIN_LAYER_HEIGTH_G3,
        .ver_deflicker_level = 0,
        .hor_deflicker_level = 0,
        .is_layer_support = TD_TRUE,
        .is_vo_scale = TD_FALSE,
        .has_cmap_reg = 0,
        .is_decompress = 0,
        .is_premul = 1,
        .is_ghdr = 0,
        .is_osb = 1,
    },
    /* G4 */
    {
        .is_key_rgb = 1,
        .is_key_alpha = 0,
        .is_global_alpha = 1,
        .is_cmap = 1,
        .is_color_format = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .max_width = GFBG_MAX_LAYER_WIDTH_G3,
        .max_height = GFBG_MAX_LAYER_HEIGTH_G3,
        .min_width = GFBG_MIN_LAYER_WIDTH_G3,
        .min_height = GFBG_MIN_LAYER_HEIGTH_G3,
        .ver_deflicker_level = 0,
        .hor_deflicker_level = 0,
        .is_layer_support = TD_TRUE,
        .is_vo_scale = TD_FALSE,
        .has_cmap_reg = 0,
        .is_decompress = 0,
        .is_premul = 1,
        .is_ghdr = 0,
        .is_osb = 1,
    },
};

td_s32 *g_vo_gfbg_layer_index = TD_NULL;
td_s32 g_vo_gfbg_layer_index_array[2][5] = { /* 2 5 alg data */
    {
        4, /* OT_VO_LAYER_G0 */
        5, /* OT_VO_LAYER_G1 */
        6, /* OT_VO_LAYER_G2 */
        7, /* OT_VO_LAYER_G3 */
        8, /* OT_VO_LAYER_G4 */
    },
    {
        3, /* OT_VO_LAYER_G0 */
        4, /* OT_VO_LAYER_G1 */
        -1, /* OT_VO_LAYER_G2 */
        5, /* OT_VO_LAYER_G3 */
        -1, /* OT_VO_LAYER_G4 */
    },
};

td_s32 gfbg_vou_get_dev_id(td_u32 layer_id)
{
    if (layer_id >= GFBG_MAX_LAYER_NUM || g_vo_gfbg_layer_index == TD_NULL) {
        return -1;
    }
    return g_vo_gfbg_layer_index[layer_id] - g_vo_gfbg_layer_index[0];
}

/*
 * the index number of fb and the actual graphics layer
 * 0 - HAL_DISP_LAYER_GFX0
 * 1 - HAL_DISP_LAYER_GFX1
 * 2 - HAL_DISP_LAYER_GFX3
 */
hal_disp_layer g_gfbglayer_to_hwlayer[GFBG_MAX_LAYER_NUM] = {
    HAL_DISP_LAYER_GFX0,
    HAL_DISP_LAYER_GFX1,
    HAL_DISP_LAYER_GFX2,
    HAL_DISP_LAYER_GFX3,
    HAL_DISP_LAYER_GFX4,
};

hal_disp_layer gfbg_drv_gfbglayer_to_hwlayer(td_u32 layer_id)
{
    return g_gfbglayer_to_hwlayer[layer_id];
}

td_bool is_4k_layer(td_u32 layer_id)
{
#ifndef CONFIG_GFBG_G0_FHD_SUPPORT
    if (layer_id == GRAPHICS_LAYER_G0 || layer_id == GRAPHICS_LAYER_G1) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
#else
    ot_unused(layer_id);
    return TD_FALSE;
#endif
}

td_bool is_hd_layer(td_u32 layer_id)
{
#ifdef CONFIG_GFBG_G0_FHD_SUPPORT
    if (layer_id == GRAPHICS_LAYER_G0) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
#else
    ot_unused(layer_id);
    return TD_FALSE;
#endif
}

td_bool is_sd_layer(td_u32 layer_id)
{
    if ((layer_id == GRAPHICS_LAYER_G3) || (layer_id == GRAPHICS_LAYER_G4)) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

td_bool is_ad_layer(td_u32 layer_id)
{
    ot_unused(layer_id);
    return TD_FALSE;
}

td_bool is_cursor_layer(td_u32 layer_id)
{
    return (layer_id == GRAPHICS_LAYER_G2);
}

td_bool is_layer_support_low_delay(td_u32 layer_id)
{
    return (layer_id == GRAPHICS_LAYER_G0);
}

/* Frame decompression needs to be offset by 16 bytes;
line decompression does not need */
td_phys_addr_t gfbg_drv_get_dcmp_offset_addr(td_phys_addr_t dcmp_addr)
{
    return dcmp_addr;
}

td_s32 gfbg_drv_set_layer_enable(td_u32 layer_id, td_bool enable)
{
    return fb_graphics_set_layer_enable(g_gfbglayer_to_hwlayer[layer_id], enable);
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 gfbg_drv_get_layer_enable(td_u32 layer_id, td_bool *enable)
{
    return fb_graphics_get_layer_enable(g_gfbglayer_to_hwlayer[layer_id], enable);
}
#endif

/*
* Name : gfbg_drv_layer_default_setting
* Desc : The default settings for the layer, including the following:
            1.Color value mode
            2.Bit extension mode
            3.alpha, global alpha,CSC,CSC enable, get interface type
*/
td_s32 gfbg_drv_layer_default_setting(td_u32 layer_id)
{
    hal_disp_layer disp_layer = g_gfbglayer_to_hwlayer[layer_id];

    if (g_gfbg_gfx_cap[layer_id].is_key_rgb || g_gfbg_gfx_cap[layer_id].is_key_alpha) {
        fb_graphics_set_gfx_key_mode(disp_layer, FB_COLORKEY_IN);
    }

    fb_graphics_set_gfx_ext(disp_layer, FB_GFX_BITEXTEND_3RD);

    fb_graphics_set_gfx_palpha(disp_layer, TD_TRUE, TD_TRUE, GRAPHIC_ALPHA_OPACITY, GRAPHIC_ALPHA_OPACITY);

    fb_graphics_set_layer_galpha(disp_layer, GRAPHIC_ALPHA_OPACITY);

    fb_graphics_set_csc_coef(disp_layer);

    fb_graphics_set_csc_en(disp_layer, TD_TRUE);

    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_layer_addr(td_u32 layer_id, td_phys_addr_t addr)
{
    if (fb_graphics_set_gfx_addr(g_gfbglayer_to_hwlayer[layer_id], addr) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 gfbg_drv_get_layer_addr(td_u32 layer_id, td_phys_addr_t *phys_addr)
{
    if (fb_graphics_get_gfx_addr(g_gfbglayer_to_hwlayer[layer_id], phys_addr) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}


td_s32 gfbg_drv_set_layer_stride(td_u32 layer_id, td_u32 stride)
{
    if (fb_graphics_set_gfx_stride(g_gfbglayer_to_hwlayer[layer_id], stride) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_layer_alpha(td_u32 layer_id, ot_fb_alpha alpha)
{
    td_u32 pre_mult_mode = 0;
    hal_disp_layer disp_layer;

    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_FAILURE;
    }

    disp_layer = g_gfbglayer_to_hwlayer[layer_id];
    /* When the premultiply is enabled, the global alpha cannot be 1 */
    if (alpha.global_alpha == 1) {
        if (fb_graphics_get_gfx_pre_mult(disp_layer, &pre_mult_mode) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (pre_mult_mode == 1) {
            gfbg_error("the global alpha can not set to 1 when the pre-mult mode is enable\n");
            return TD_FAILURE;
        }
    }
    if (alpha.global_alpha_en) {
        fb_graphics_set_layer_galpha(g_gfbglayer_to_hwlayer[layer_id], alpha.global_alpha);
    } else {
        fb_graphics_set_layer_galpha(g_gfbglayer_to_hwlayer[layer_id], GRAPHIC_ALPHA_OPACITY);
    }

    fb_graphics_set_gfx_palpha(g_gfbglayer_to_hwlayer[layer_id], alpha.pixel_alpha, TD_TRUE,
                               alpha.alpha0, alpha.alpha1);

    return TD_SUCCESS;
}
td_s32 gfbg_drv_set_layer_fmt_clut_check(hal_disp_layer disp_layer)
{
#ifndef CONFIG_CHIP_GFBG_SUPPORT_G3
        if (disp_layer != HAL_DISP_LAYER_GFX0) {
            gfbg_error("GFBG does not support clut2/clut4 except gfx0\n");
            return TD_FAILURE;
        }
#else
        if (disp_layer != HAL_DISP_LAYER_GFX3 && disp_layer != HAL_DISP_LAYER_GFX4) {
            gfbg_error("GFBG does not support clut2/clut4 except gfx3 or gfx4\n");
            return TD_FAILURE;
        }
#endif

    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_layer_data_fmt(td_u32 layer_id, ot_fb_color_format data_format)
{
    gfbg_disp_pixel_format pixel_format_for_hal;
    hal_disp_layer disp_layer = g_gfbglayer_to_hwlayer[layer_id];

    switch (data_format) {
        case OT_FB_FORMAT_ARGB1555: {
            pixel_format_for_hal = GFBG_INPUTFMT_ARGB_1555;
            /* Pre-multiply enable is not supported when the data format is ARGB1555 */
            fb_graphics_set_gfx_pre_mult(disp_layer, TD_FALSE);
            break;
        }

        case OT_FB_FORMAT_ARGB8888: {
            pixel_format_for_hal = GFBG_INPUTFMT_ARGB_8888;
            break;
        }

        case OT_FB_FORMAT_ARGB4444: {
            pixel_format_for_hal = GFBG_INPUTFMT_ARGB_4444;
            break;
        }

        case OT_FB_FORMAT_4BPP: {
            if (gfbg_drv_set_layer_fmt_clut_check(disp_layer) != TD_SUCCESS) {
                return TD_FAILURE;
            }
            pixel_format_for_hal = GFBG_INPUTFMT_CLUT_4BPP;
            break;
        }

        case OT_FB_FORMAT_2BPP: {
            if (gfbg_drv_set_layer_fmt_clut_check(disp_layer) != TD_SUCCESS) {
                return TD_FAILURE;
            }
            pixel_format_for_hal = GFBG_INPUTFMT_CLUT_2BPP;
            break;
        }
        default: {
            gfbg_error("GFBG does not support this color format\n");
            return TD_FAILURE;
        }
    }

    if (fb_graphics_set_layer_data_fmt(disp_layer, pixel_format_for_hal) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_layer_src_image_reso(td_u32 disp_layer, const ot_fb_rect *rect)
{
    ot_fb_rect gfbg_rect = {0};

    if (rect == TD_NULL) {
        return TD_FAILURE;
    }
    gfbg_rect.x = rect->x;
    gfbg_rect.y = rect->y;
    gfbg_rect.width = rect->width;
    gfbg_rect.height = rect->height;
    if (fb_graphics_set_layer_src_image_reso(g_gfbglayer_to_hwlayer[disp_layer], &gfbg_rect) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_layer_rect(td_u32 layer_id, const ot_fb_rect *input_rect, const ot_fb_rect *output_rect)
{
    ot_fb_rect rect;
    fb_scan_mode scan_mode = 0;
    td_bool is_field_update;
    ot_unused(input_rect);

    if (output_rect == TD_NULL) {
        return TD_FAILURE;
    }

    rect.x = output_rect->x;
    rect.y = output_rect->y;
    rect.width = output_rect->width;
    rect.height = output_rect->height;
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer_id(%u) is invalid\n", layer_id);
        return TD_FAILURE;
    }
    if (fb_graphics_get_dev_mode(g_gfbglayer_to_hwlayer[layer_id], &scan_mode, &is_field_update) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (scan_mode == FB_SCAN_MODE_INTERLACE) {
        /* When interlaced display, the ordinate and height are required to be even */
        if ((((td_u32)rect.y & 0x1) == 1) || (((td_u32)rect.height & 0x1) == 1)) {
            gfbg_error("the height must be even for interlaced device\n");
            return TD_FAILURE;
        }
    }

    fb_graphics_set_layer_out_rect(g_gfbglayer_to_hwlayer[layer_id], &rect);

    fb_graphics_set_layer_in_rect(g_gfbglayer_to_hwlayer[layer_id], &rect);

    return TD_SUCCESS;
}

static td_s32 gfbg_drv_set_lay_key_mask(td_u32 layer_id, const gfbg_colorkeyex *colorkey)
{
    fb_gfx_key_max max_rgb;
    fb_gfx_key_min min_rgb;
    fb_gfx_mask mask_rgb;
    hal_disp_layer disp_layer = g_gfbglayer_to_hwlayer[layer_id];

    if (colorkey->key_enable) {
        if (fb_graphics_set_gfx_key_mode(disp_layer, colorkey->key_mode) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        max_rgb.key_max_r = colorkey->red_max;
        max_rgb.key_max_g = colorkey->green_max;
        max_rgb.key_max_b = colorkey->blue_max;

        min_rgb.key_min_r = colorkey->red_min;
        min_rgb.key_min_g = colorkey->green_min;
        min_rgb.key_min_b = colorkey->blue_min;

        fb_graphics_set_color_key_value(disp_layer, max_rgb, min_rgb);
        if (colorkey->mask_enable) {
            mask_rgb.mask_r = colorkey->red_mask;
            mask_rgb.mask_g = colorkey->green_mask;
            mask_rgb.mask_b = colorkey->blue_mask;
        } else {
            mask_rgb.mask_r = 0;
            mask_rgb.mask_g = 0;
            mask_rgb.mask_b = 0;
        }
        fb_graphics_set_color_key_mask(disp_layer, mask_rgb);
    }

    if (fb_graphics_set_gfx_key_en(disp_layer, colorkey->key_enable) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_FAILURE;
}

td_s32 gfbg_drv_color_convert(const struct fb_var_screeninfo *var, gfbg_colorkeyex *ck_ex)
{
    td_u8 r, g, b;
    td_u32 value;
    td_u8 red_mask, green_mask, blue_mask;

    if (var == TD_NULL || ck_ex == TD_NULL) {
        return TD_FAILURE;
    }
    value = ck_ex->key;

    r = (value >> var->red.offset) << (8 - var->red.length); /* 8 bit width */
    g = (value >> var->green.offset) << (8 - var->green.length); /* 8 bit width */
    b = (value >> var->blue.offset) << (8 - var->blue.length); /* 8 bit width */

    red_mask = (0xff >> var->red.length);
    green_mask = (0xff >> var->green.length);
    blue_mask = (0xff >> var->blue.length);

    ck_ex->blue_min = b;
    ck_ex->green_min = g;
    ck_ex->red_min = r;

    ck_ex->blue_max = b | blue_mask;
    ck_ex->green_max = g | blue_mask;
    ck_ex->red_max = r | blue_mask;

    /* low bits not using when do compare, after extend bits */
    ck_ex->red_mask = ~red_mask;
    ck_ex->green_mask = ~green_mask;
    ck_ex->blue_mask = ~blue_mask;

    ck_ex->key_mode = 0;

    ck_ex->mask_enable = TD_TRUE;

    return TD_SUCCESS;
}

td_s32 gfbg_drv_updata_layer_reg(td_u32 layer_id)
{
    if (fb_graphics_set_reg_up(g_gfbglayer_to_hwlayer[layer_id]) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_color_reg(td_u32 layer_id, td_u32 offset, td_u32 color, td_s32 up_flag)
{
    td_u32 *clut_table = TD_NULL;
    gfbg_mmz_buffer *clut_table_addr = TD_NULL;

#ifdef CONFIG_TDE_CLUT_RECT_SUPPORT_G0
    /* G0 support clut */
    if (layer_id != GRAPHICS_LAYER_G0) {
        return TD_FAILURE;
    }
#else
    /* only G3 or G4 support clut */
    if ((layer_id != GRAPHICS_LAYER_G3) && (layer_id != GRAPHICS_LAYER_G4)) {
        return TD_FAILURE;
    }
#endif
#ifdef CONFIG_TDE_CLUT_RECT_SUPPORT_G0
    if (layer_id == GRAPHICS_LAYER_G0) {
        clut_table_addr = graphic_drv_get_clut_table_g0();
        if (clut_table_addr == TD_NULL) {
            return TD_FAILURE;
        }
    }
#else
    if (layer_id == GRAPHICS_LAYER_G3) {
        clut_table_addr = graphic_drv_get_clut_table_g3();
        if (clut_table_addr == TD_NULL) {
            return TD_FAILURE;
        }
    }
#endif
#ifdef CONFIG_TDE_CLUT_RECT_SUPPORT_G4
    if (layer_id == GRAPHICS_LAYER_G4) {
        clut_table_addr = graphic_drv_get_clut_table_g4();
        if (clut_table_addr == TD_NULL) {
            return TD_FAILURE;
        }
    }
#endif
    if (clut_table_addr == TD_NULL || clut_table_addr->start_vir_addr == TD_NULL) {
        return TD_FAILURE;
    }
    clut_table = (td_u32 *)(clut_table_addr->start_vir_addr);
    if (clut_table == TD_NULL) {
        return TD_FAILURE;
    }

    if (offset < 256) { /* 256 for clut table max */
        clut_table[offset] = color;
    }

    if (up_flag != 0) {
        fb_graphics_set_color_reg(layer_id, clut_table_addr->start_phy_addr);
    }

    return TD_SUCCESS;
}

td_void gfbg_drv_set_color_reg_up(td_u32 layer_id)
{
    fb_graphics_set_color_reg_up(layer_id);
}

td_s32 gfbg_drv_init(td_void)
{
    td_s32 ret;
    ot_tde_export_func  *tde_export_func = TD_NULL;

    tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_open == TD_NULL) ||
        (tde_export_func->drv_tde_module_close == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    } else {
        ret = tde_export_func->drv_tde_module_open();
        if (ret != TD_SUCCESS) {
            gfbg_error("TDE open failed\n");
            return ret;
        }
    }

    ret = fb_graphics_resource_init();
    if (ret != TD_SUCCESS) {
        tde_export_func->drv_tde_module_close();
        gfbg_error("resource init failed\n");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 gfbg_drv_deinit(td_void)
{
    ot_tde_export_func  *tde_export_func = TD_NULL;
    fb_graphics_resource_deinit();

    tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_close == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    }
    tde_export_func->drv_tde_module_close();

    return TD_SUCCESS;
}

/* set premul data */
td_s32 gfbg_drv_set_pre_mul(td_u32 layer_id, td_bool is_premul)
{
    hal_disp_pixel_format format = HAL_INPUTFMT_ARGB_1555;
    hal_disp_layer layer = g_gfbglayer_to_hwlayer[layer_id];
    td_u8 global_alpha;

    if (is_premul == TD_TRUE) {
        /* When the global alpha configuration is 1, pre-multiplication cannot be enabled. */
        if (fb_graphics_get_layer_galpha(layer, &global_alpha) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (global_alpha == 1) {
            gfbg_error("not allow to set pre-mult mode when the GlobalAlpha is 1\n");
            return TD_FAILURE;
        }

        /* Pre-multiply enable is not supported when the data format is ARGB1555 */
        if (fb_graphics_get_layer_data_fmt(layer, &format) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if ((format == HAL_INPUTFMT_ARGB_1555) || (format == HAL_INPUTFMT_ARGB_4444)) {
            gfbg_error("not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
            return TD_FAILURE;
        }
    }

    if (fb_graphics_set_gfx_pre_mult(layer, is_premul) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_int_callback(gfbg_int_type int_type, int_vo_callback callback, td_u32 layer_id, td_void *arg)
{
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_FAILURE;
    }

    return fb_graphics_set_callback(g_gfbglayer_to_hwlayer[layer_id], int_type, (vo_fb_intcallback)callback, arg);
}

/* The premise is that the VO device is enabled and an error is returned when the VO device is not enabled.
For X5HD, the current configuration content of the graphics layer is obtained for the boot screen;
since the boot screen is not required, only the device resolution and scan information
can be obtained. */
td_s32 gfbg_drv_get_osd_data(td_u32 layer_id, gfbg_osd_data *layer_data)
{
    td_u32 width = 0;
    td_u32 height = 0;
    td_s32 ret;
    hal_disp_layer disp_layer = g_gfbglayer_to_hwlayer[layer_id];
    fb_scan_mode scan_mode = 0;

    if (layer_data == TD_NULL) {
        return TD_FAILURE;
    }

    ret = fb_graphics_get_intf_size(disp_layer, &width, &height);
    if (ret != TD_SUCCESS) {
        gfbg_error("Failed to get screen width and height!\n");
        return TD_FAILURE;
    }

    ret = fb_graphics_get_gfx_addr(disp_layer, &layer_data->buffer_phy_addr);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = fb_graphics_get_gfx_stride(disp_layer, &layer_data->stride);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = fb_graphics_get_dev_mode(disp_layer, &scan_mode, &layer_data->feild_update);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (scan_mode == FB_SCAN_MODE_INTERLACE) {
        layer_data->scan_mode = GFBG_SCANMODE_I;
    } else {
        layer_data->scan_mode = GFBG_SCANMODE_P;
    }

    layer_data->screen_height = height;
    layer_data->screen_width = width;

    return TD_SUCCESS;
}

td_s32 gfbg_open_layer(td_u32 layer_id)
{
    return fb_graphics_open_layer(g_gfbglayer_to_hwlayer[layer_id]);
}

td_s32 gfbg_close_layer(td_u32 layer_id)
{
    return fb_graphics_close_layer(g_gfbglayer_to_hwlayer[layer_id]);
}

td_s32 gfbg_drv_enable_dcmp(td_u32 layer_id, td_bool enable)
{
    return fb_graphics_enable_dcmp(g_gfbglayer_to_hwlayer[layer_id], enable);
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 gfbg_drv_get_dcmp_enable_state(td_u32 layer_id, td_bool *enable)
{
    return fb_graphics_get_dcmp_enable_state(g_gfbglayer_to_hwlayer[layer_id], enable);
}
#endif

#ifndef CONFIG_COMPRESS_ECONOMIZE_MEMERY
static td_u32 gfbg_drv_get_hdcmp_stride(const gfbg_graphic_dcmp_info *dcmp_info)
{
    td_u32 comp_ratio_int = 1000; /* 1000 for calculate cmp */
    td_u32 byte_per_pix;
    td_u32 extend_width;
    td_u32 exp_num_temp = 0;
    td_u32 stride = dcmp_info->stride;

    if (dcmp_info->pixel_fmt == GFBG_INPUTFMT_ARGB_8888) {
        if (dcmp_info->width <= 320) { /* 320 width */
            exp_num_temp = 2; /* 2 for calculate cmp */
            comp_ratio_int = 1000; /* 1000 for calculate cmp */
        } else if (dcmp_info->width <= 720) { /* 720 width */
            exp_num_temp = 10; /* 10 for calculate cmp */
            comp_ratio_int = 2000; /* 2000 for calculate cmp */
        } else if (dcmp_info->width <= 3840) { /* 3840 width */
            exp_num_temp = 10; /* 10 for calculate cmp */
            comp_ratio_int = 2000; /* 2000 for calculate cmp */
        }

        byte_per_pix = 4; /* 4 for calculate cmp */
        /* 8 bits, 1000 for calculate 127,128,16 for align */
        stride = ((dcmp_info->width * byte_per_pix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    } else if ((dcmp_info->pixel_fmt == GFBG_INPUTFMT_ARGB_1555) ||
               (dcmp_info->pixel_fmt == GFBG_INPUTFMT_ARGB_4444)) {
        if (dcmp_info->width <= 720) { /* 720 width */
            exp_num_temp = 2; /* 2 for calculate cmp */
            comp_ratio_int = 1000; /* 1000 for calculate cmp */
        } else if (dcmp_info->width <= 3840) { /* 3840 width */
            exp_num_temp = 0;
            comp_ratio_int = 1000; /* 1000 for calculate cmp */
        }
        byte_per_pix = 2; /* 2 for calculate cmp */

        extend_width = ((dcmp_info->width + 31) / 32) * 32; /* 31 32 for calculate cmp */
        /* 8 bits, 1000 for calculate 127,128,16 for align */
        stride = ((extend_width * byte_per_pix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    }

    stride = (stride + 0xf) & 0xfffffff0; /* 0xf 0xfffffff0 for calculate cmp */

    return stride;
}
#endif

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
static td_s32 gfbg_drv_set_dcmp_info(td_u32 layer_id, gfbg_graphic_dcmp_info *dcmp_info)
{
    /* line compress need to re calculate the stride */
#ifndef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    dcmp_info->stride = gfbg_drv_get_hdcmp_stride(dcmp_info);
#else
    gfbg_stride_attr attr = {0};
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = dcmp_info->width;

    if (dcmp_info->pixel_fmt == GFBG_INPUTFMT_ARGB_8888) {
        attr.format = OT_FB_FORMAT_ARGB8888;
    } else if (dcmp_info->pixel_fmt == GFBG_INPUTFMT_ARGB_1555) {
        attr.format = OT_FB_FORMAT_ARGB1555;
    } else if (dcmp_info->pixel_fmt == GFBG_INPUTFMT_ARGB_4444) {
        attr.format = OT_FB_FORMAT_ARGB4444;
    } else {
        attr.format = OT_FB_FORMAT_ARGB1555;
    }
    gfbg_recalculate_stride(&dcmp_info->stride, NULL, &attr);
#endif
    return fb_graphics_set_dcmp_info(g_gfbglayer_to_hwlayer[layer_id], dcmp_info);
}

td_bool gfbg_drv_is_layer_support_zoom_out(td_u32 layer_id)
{
    ot_unused(layer_id);
    return TD_FALSE;
}
#endif

td_s32 gfbg_drv_enable_zme(td_u32 layer_id, const ot_fb_rect *in_rect, const ot_fb_rect *out_rect, td_bool enable)
{
    ot_fb_rect inner_rect;
    ot_fb_rect outer_rect;

    if (in_rect == TD_NULL || out_rect == TD_NULL) {
        return TD_FAILURE;
    }
    inner_rect.width = in_rect->width;
    inner_rect.height = in_rect->height;
    outer_rect.width = out_rect->width;
    outer_rect.height = out_rect->height;

    if (g_gfbglayer_to_hwlayer[layer_id] == HAL_DISP_LAYER_GFX0) {
        return fb_graphics_enable_zme(g_gfbglayer_to_hwlayer[layer_id], &inner_rect, &outer_rect, enable);
    } else {
#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
        if (g_gfbglayer_to_hwlayer[layer_id] == HAL_DISP_LAYER_GFX1) {
            /* fb_graphics_enable_zme */
            return fb_graphics_enable_zme(g_gfbglayer_to_hwlayer[layer_id], &inner_rect, &outer_rect, enable);
        }
#endif
        if (enable) {
            gfbg_error("G%u does not support zme.\n", layer_id);
        }
        return TD_SUCCESS;
    }
}

td_s32 gfbg_drv_graphics_get_int(td_u32 *int_status)
{
    return fb_graphics_get_int(int_status);
}

static td_s32 gfbg_drv_graphics_get_interrupt_dev(td_u32 int_status, ot_vo_dev *vo_dev)
{
    return fb_graphics_get_interrupt_dev(int_status, vo_dev);
}

td_void gfbg_drv_set_tde_sync(td_u32 layer_id, const gfbg_sync_attr *sync_info)
{
#ifdef CONFIG_GFBG_LOW_DELAY_SUPPORT
    gfbg_bind_tde_cfg bind_tde_cfg = {0};

    if (sync_info == TD_NULL) {
        return;
    }

    if (g_gfbglayer_to_hwlayer[layer_id] != HAL_DISP_LAYER_GFX0) {
        return;
    }

    bind_tde_cfg.is_hw_mute_clr_en = 0;
    bind_tde_cfg.is_mute_en = 0;
    bind_tde_cfg.safe_dist = sync_info->safe_dist;
    bind_tde_cfg.is_sync = sync_info->is_sync;
    fb_graphics_set_tde_sync(&bind_tde_cfg);
    return;
#else
    ot_unused(layer_id);
    ot_unused(sync_info);
    return;
#endif
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
static td_s32 drv_gfbg_check_frame_param(const ot_fb_smart_rect_param *param, td_u32 i, td_u32 max_width,
    td_u32 max_height)
{
    if ((param->rect_start[i].rect.width <= 0) || (param->rect_start[i].rect.height <= 0) ||
        (param->rect_start[i].rect.x < 0) || (param->rect_start[i].rect.y < 0)) {
        gfbg_error("invalid i:%d, x:%d, y:%d, w:%d, h:%d\n", i, param->rect_start[i].rect.x,
            param->rect_start[i].rect.y, param->rect_start[i].rect.width, param->rect_start[i].rect.height);
        return TD_FAILURE;
    }
    if ((param->rect_start[i].rect.x > (td_s32)(max_width - 1)) ||
        (param->rect_start[i].rect.y > (td_s32)(max_height - 1)) ||
        ((param->rect_start[i].rect.x + param->rect_start[i].rect.width - 1) > (td_s32)(max_width - 1)) ||
        ((param->rect_start[i].rect.y + param->rect_start[i].rect.height - 1) > (td_s32)(max_height - 1))) {
        gfbg_error("invalid i:%d, x:%d, y:%d, w:%d, h:%d, max_w:%u, max_height:%u\n", i, param->rect_start[i].rect.x,
            param->rect_start[i].rect.y, param->rect_start[i].rect.width, param->rect_start[i].rect.height,
            max_width, max_height);
        return TD_FAILURE;
    }
    if (param->rect_start[i].mode == OT_FB_SMART_RECT_SOLID) {
        if ((param->rect_start[i].rect.width < (td_s32)(param->rect_start[i].thick * 2)) || /* 2 double */
            (param->rect_start[i].rect.height < (td_s32)(param->rect_start[i].thick * 2)) || /* 2 double */
            (param->rect_start[i].thick > 32)) { /* 32 max */
            gfbg_error("invalid i:%d, w:%d, h:%d, thick:%u, thick_max:32\n", i, param->rect_start[i].rect.width,
                param->rect_start[i].rect.height, param->rect_start[i].thick);
            return TD_FAILURE;
        }
    }
    if (param->rect_start[i].mode == OT_FB_SMART_RECT_CORNER) {
        if ((param->rect_start[i].rect.width < (td_s32)(param->rect_start[i].thick * 2)) || /* 2 double */
            (param->rect_start[i].rect.height < (td_s32)(param->rect_start[i].thick * 2)) || /* 2 double */
            (param->rect_start[i].rect.width < (td_s32)(param->rect_start[i].corner_length * 2)) || /* 2 double */
            (param->rect_start[i].rect.height < (td_s32)(param->rect_start[i].corner_length * 2)) ||
            (param->rect_start[i].thick > 32) || (param->rect_start[i].corner_length > 255)) { /* 32 255 max */
            gfbg_error("invalid i:%d, w:%d, h:%d; thick:%u, corner_length:%u, thick_max:32, corner_length_max:255\n", i,
                param->rect_start[i].rect.width, param->rect_start[i].rect.height, param->rect_start[i].thick,
                param->rect_start[i].corner_length);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_smart_rect(td_u32 layer_id, td_u32 max_width, td_u32 max_height,
    const ot_fb_smart_rect_param *param, const gfbg_mmz_buffer *osb_chn)
{
    td_u32 i, j;
    td_u32 *vir = TD_NULL;
    td_u32 alpha, red, green, blue, length, thick, mode;
    td_u32 hor_start, hor_end, ver_start, ver_end;
    /* only G3 or G4 support clut */
    if ((layer_id != GRAPHICS_LAYER_G3) && (layer_id != GRAPHICS_LAYER_G4)) {
        gfbg_error("only G3 or G4 support smart rect\n");
        return TD_FAILURE;
    }
    if (osb_chn == TD_NULL || param == TD_NULL || param->rect_start == TD_NULL) {
        return TD_FAILURE;
    }
    vir = (td_u32 *)(osb_chn->start_vir_addr);
    if (vir == TD_NULL) {
        return TD_FAILURE;
    }
    (td_void)memset_s(vir, osb_chn->size, 0, osb_chn->size);
    for (i = 0; i < param->num; i++) {
        if (drv_gfbg_check_frame_param(param, i, max_width, max_height)) {
            return TD_FAILURE;
        }
        j = 4 * i; /* 4 part */
        alpha = (param->rect_start[i].color_value & 0xf0000000) >> 28; /* 28 29~32bits */
        red = (param->rect_start[i].color_value & 0x00f00000) >> 20; /* 20 21~24bits */
        green = (param->rect_start[i].color_value & 0x0000f000) >> 12; /* 12 13~16bits */
        blue = (param->rect_start[i].color_value & 0x000000f0) >> 4; /* 4 5~8bits */
        length = (param->rect_start[i].corner_length & 0x000000ff); /* 0x000000ff low 8bits */
        thick = (param->rect_start[i].thick & 0x3f); /* 0x20 max 32 */
        mode = (param->rect_start[i].mode);
        /* 28,24,20,16,8 for alg data */
        vir[j] = (alpha << 28) | (red << 24) | (green << 20) | (blue << 16) | (length << 8) | (thick << 2) |
                 (mode); /* 2 for alg data */

        hor_start = (td_u32)(param->rect_start[i].rect.x) & 0xfff;
        hor_end = (td_u32)(param->rect_start[i].rect.x + param->rect_start[i].rect.width - 1) & 0xfff;
        ver_start = (td_u32)(param->rect_start[i].rect.y) & 0xfff;
        ver_end = (td_u32)(param->rect_start[i].rect.y + param->rect_start[i].rect.height - 1) & 0xfff;

        vir[j + 1] = (hor_end << 16) | (hor_start); /* 16 for alg data */
        vir[j + 2] = (ver_end << 16) | (ver_start); /* 2 16 for alg data */
    }
    fb_graphics_set_frame_reg(layer_id, osb_chn->start_phy_addr);

    return TD_SUCCESS;
}

td_void gfbg_drv_smart_rect_up_param(td_u32 layer_id)
{
    fb_graphics_up_frame_param(layer_id);
}
#endif

td_void gfbg_close_smart_rect(td_u32 layer_id)
{
    if ((layer_id != GRAPHICS_LAYER_G3) && (layer_id != GRAPHICS_LAYER_G4)) {
        return;
    }
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    fb_graphics_up_frame_param(layer_id);
    fb_graphics_close_smart_rect(layer_id);
    return;
#endif
}

static td_void get_implement_ops(gfbg_drv_ops *ops)
{
    ops->gfbg_drv_init = gfbg_drv_init;
    ops->gfbg_drv_deinit = gfbg_drv_deinit;
    ops->gfbg_drv_set_layer_enable = gfbg_drv_set_layer_enable;
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    ops->gfbg_drv_get_layer_enable = gfbg_drv_get_layer_enable;
#endif
    ops->gfbg_drv_layer_default_setting = gfbg_drv_layer_default_setting;
    ops->gfbg_drv_set_layer_addr = gfbg_drv_set_layer_addr;
    ops->gfbg_drv_get_layer_addr = gfbg_drv_get_layer_addr;
    ops->gfbg_drv_set_layer_stride = gfbg_drv_set_layer_stride;
    ops->gfbg_drv_set_layer_alpha = gfbg_drv_set_layer_alpha;
    ops->gfbg_drv_set_layer_data_fmt = gfbg_drv_set_layer_data_fmt;
    ops->gfbg_drv_set_layer_rect = gfbg_drv_set_layer_rect;
    ops->gfbg_drv_set_layer_src_image_reso = gfbg_drv_set_layer_src_image_reso;
    ops->gfbg_drv_set_layer_key_mask = gfbg_drv_set_lay_key_mask;
    ops->gfbg_drv_color_convert = gfbg_drv_color_convert;
    ops->gfbg_drv_updata_layer_reg = gfbg_drv_updata_layer_reg;
    ops->gfbg_drv_set_int_callback = gfbg_drv_set_int_callback;
    ops->gfbg_drv_set_color_reg = gfbg_drv_set_color_reg;
    ops->gfbg_drv_set_color_reg_up = gfbg_drv_set_color_reg_up;
    ops->gfbg_drv_set_pre_mul = gfbg_drv_set_pre_mul;
    ops->gfbg_drv_set_clut_addr = TD_NULL;
    ops->gfbg_drv_get_osd_data = gfbg_drv_get_osd_data;
    ops->gfbg_drv_open_display = TD_NULL;
    ops->gfbg_drv_close_display = TD_NULL;
    ops->gfbg_open_layer = gfbg_open_layer;
    ops->gfbg_close_layer = gfbg_close_layer;
    ops->gfbg_drv_enable_dcmp = gfbg_drv_enable_dcmp;
    ops->gfbg_close_smart_rect = gfbg_close_smart_rect;
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    ops->gfbg_drv_set_dcmp_info = gfbg_drv_set_dcmp_info;
    ops->gfbg_drv_get_dcmp_enable_state = gfbg_drv_get_dcmp_enable_state;
#endif
    return;
}

static td_void gfbg_init_gfx_cap(td_void)
{
    /* reset capability */

#if defined (CONFIG_CHIP_GFBG_SUPPORT_G3)
    g_gfbg_gfx_cap[1].is_decompress = TD_FALSE; /* 1 G1 */
    g_gfbg_gfx_cap[2].is_layer_support = TD_FALSE; /* 2 G2 */
    g_gfbg_gfx_cap[4].is_layer_support = TD_FALSE; /* 4 G4 */
    g_vo_gfbg_layer_index = g_vo_gfbg_layer_index_array[1];
#else
    g_gfbg_gfx_cap[0].is_vo_scale = TD_FALSE;      /* 0 G0 zme */
    g_gfbg_gfx_cap[0].is_decompress = TD_FALSE;    /* 0 G0 decompress */
    g_gfbg_gfx_cap[0].is_cmap = TD_TRUE;           /* 0 G0 color map */
    g_gfbg_gfx_cap[0].is_color_format[OT_FB_FORMAT_2BPP] = TD_TRUE;     /* 0 G0 clut2 */
    g_gfbg_gfx_cap[0].is_color_format[OT_FB_FORMAT_4BPP] = TD_TRUE;     /* 0 G0 clut4 */
    g_gfbg_gfx_cap[1].is_layer_support = TD_FALSE; /* 1 G1 */
    g_gfbg_gfx_cap[2].is_layer_support = TD_FALSE; /* 2 G2 */
    g_gfbg_gfx_cap[3].is_layer_support = TD_FALSE; /* 3 G3 */
    g_gfbg_gfx_cap[4].is_layer_support = TD_FALSE; /* 4 G4 */
    g_vo_gfbg_layer_index = g_vo_gfbg_layer_index_array[0];
#endif
    return;
}

td_void gfbg_drv_get_ops(gfbg_drv_ops *ops)
{
    ops->layer_count = GFBG_MAX_LAYER_NUM;
    /* check if support G1 */
    gfbg_init_gfx_cap();
    ops->capability = (ot_fb_capability *)g_gfbg_gfx_cap;

    /* implement */
    get_implement_ops(ops);
    /* For ZME */
    ops->gfbg_drv_enable_zme = gfbg_drv_enable_zme;
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    ops->gfbg_drv_is_layer_support_zoom_out = gfbg_drv_is_layer_support_zoom_out;
#endif
    /* for Int */
    ops->gfbg_drv_graphics_enable_int = TD_NULL;
    ops->gfbg_drv_graphics_clear_int = TD_NULL;
    ops->gfbg_drv_graphics_get_int = gfbg_drv_graphics_get_int;
    ops->gfbg_drv_graphics_clear_int_status = TD_NULL;
    ops->gfbg_drv_graphics_get_int_dev = gfbg_drv_graphics_get_interrupt_dev;
    /* for low delay */
    ops->gfbg_drv_set_tde_sync = gfbg_drv_set_tde_sync;
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    /* for smart rect */
    ops->gfbg_drv_set_smart_rect = gfbg_drv_set_smart_rect;
    ops->gfbg_drv_smart_rect_up_param = gfbg_drv_smart_rect_up_param;
#endif
}
