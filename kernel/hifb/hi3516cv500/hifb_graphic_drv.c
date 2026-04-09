/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_graphic_drv.h"
#include "proc_ext.h"
#include "hi_errno.h"
#include "hi_debug.h"
#include "hifb_vou_graphics.h"
#include "mm_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"

#define VOU_INTMSK_G0WBC 0x400
#define VOU_INTMSK_G4WBC 0x800

#define VOU1_IRQ_NR 91

#define GFX_CSC_SCALE    0xd
#define GFX_CSC_CLIP_MIN 0x0
#define GFX_CSC_CLIP_MAX 0x3ff

#define HIFB_DATEARR_RANGE 4

#define HIFB_CSC_COEF_ALG_DATA 100
#define HIFB_CSC_STATU_ALG_DATA 1000
#define HIFB_CSC_HUE_ALG_DATA 10000
#define HIFB_COEF_CSC_DEX 8
#define HIFB_COEF_CSC_DC_ALG_DATA 4
#define HIFB_COEF_CSC_GFX 1024

static hifb_coef_addr g_hifb_coef_buf_addr;

/* vou interrupt mask type */
typedef enum {
    HIFB_INTMSK_NONE = 0,
    HIFB_INTMSK_DHD0_VTTHD1 = 0x1,
    HIFB_INTMSK_DHD0_VTTHD2 = 0x2,
    HIFB_INTMSK_DHD0_VTTHD3 = 0x4,
    HIFB_INTMSK_DHD0_UFINT = 0x8,

    HIFB_INTMSK_DHD1_VTTHD1 = 0x10,
    HIFB_INTMSK_DHD1_VTTHD2 = 0x20,
    HIFB_INTMSK_DHD1_VTTHD3 = 0x40,
    HIFB_INTMSK_DHD1_UFINT = 0x80,

    HIFB_INTMSK_DSD_VTTHD1 = 0x100,
    HIFB_INTMSK_DSD_VTTHD2 = 0x200,
    HIFB_INTMSK_DSD_VTTHD3 = 0x400,
    HIFB_INTMSK_DSD_UFINT = 0x800,

    HIFB_INTMSK_B0_ERR = 0x1000,
    HIFB_INTMSK_B1_ERR = 0x2000,
    HIFB_INTMSK_B2_ERR = 0x4000,

    HIFB_INTMSK_WBC_DHDOVER = 0x8000,
    HIFB_INTREPORT_ALL = 0xffffffff
} hifb_int_mask;

/* RGB->YUV601 Constant coefficient matrix */
const csc_coef g_fb_csc_rgb2yuv601_tv = {
    /* csc coef */
    258,
    504,
    98,
    -148,
    -291,
    439,
    439,
    -368,
    -71,
    /* csc Input DC(IDC) */
    0,
    0,
    0,
    /* csc Output DC(ODC) */
    16,
    128,
    128,
};

/* RGB->YUV601 Constant coefficient matrix */
const csc_coef g_fb_csc_rgb2yuv601_pc = {
    /* csc coef */
    299, 587, 114, -172, -339, 511, 511, -428, -83,
    /* csc Input DC(IDC) */
    0, 0, 0,
    /* csc Output DC(ODC) */
    0, 128, 128, /* 64, 512, 512, */
};

/* RGB->YUV709 Constant coefficient matrix */
const csc_coef g_fb_csc_rgb2yuv709_tv = {
    /* csc coef */
    184,
    614,
    62,
    -101,
    -338,
    439,
    439,
    -399,
    -40,
    /* csc Input DC(IDC) */
    0,
    0,
    0,
    /* csc Output DC(ODC) */
    16,
    128,
    128,
};

/* RGB->YUV709 Constant coefficient matrix, output full[0,255] */
const csc_coef g_fb_csc_rgb2yuv709_pc = {
    /* csc coef */
    213,
    715,
    72,
    -117,
    -394,
    511,
    511,
    -464,
    -47,
    /* csc Input DC(IDC) */
    0,
    0,
    0,
    /* csc Output DC(ODC) */
    0,
    128,
    128,
};

/* YUV601->RGB Constant coefficient matrix */
const csc_coef g_csc_yuv6012rgb_pc = {
    /* csc coef */
    1164,
    0,
    1596,
    1164,
    -391,
    -813,
    1164,
    2018,
    0,
    /* csc Input DC(IDC) */
    -16,
    -128,
    -128,
    /* csc Output DC(ODC) */
    0,
    0,
    0,
};

/* YUV709->RGB Constant coefficient matrix */
const csc_coef g_csc_yuv7092rgb_pc = {
    /* csc coef */
    1164,
    0,
    1793,
    1164,
    -213,
    -534,
    1164,
    2115,
    0,
    /* csc Input DC(IDC) */
    -16,
    -128,
    -128,
    /* csc Output DC(ODC) */
    0,
    0,
    0,
};

/* YUV601->YUV709 Constant coefficient matrix, output full[0,255] */
const csc_coef g_fb_csc_yuv2yuv_601_709 = {
    /* csc coef */
    1000,
    -116,
    208,
    0,
    1017,
    114,
    0,
    75,
    1025,
    /* csc Input DC(IDC) */
    -16,
    -128,
    -128,
    /* csc Output DC(ODC) */
    16,
    128,
    128,
};

/* YUV709->YUV601 Constant coefficient matrix,output full[0,255] */
const csc_coef g_fb_csc_yuv2yuv_709_601 = {
    /* csc coef */
    1000,
    99,
    192,
    0,
    990,
    -111,
    0,
    -72,
    983,
    /* csc Input DC(IDC) */
    -16,
    -128,
    -128,
    /* csc Output DC(ODC) */
    16,
    128,
    128,
};

/* YUV601->YUV709 Constant coefficient matrix */
const csc_coef g_fb_csc_init = {
    /* csc coef */
    1000,
    0,
    0,
    0,
    1000,
    0,
    0,
    0,
    1000,
    /* csc Input DC(IDC) */
    -16,
    -128,
    -128,
    /* csc Output DC(ODC) */
    16,
    128,
    128,
};

#define HIFB_SIN_TAB_RANGE 61
#define HIFB_COS_TAB_RANGE 61

const int g_hifb_sin_table[HIFB_SIN_TAB_RANGE] = {
    -500, -485, -469, -454, -438, -422, -407, -391, -374,
    -358, -342, -325, -309, -292, -276, -259, -242, -225, -208,
    -191, -174, -156, -139, -122, -104, -87, -70, -52, -35, -17,
    0, 17, 35, 52, 70, 87, 104, 122, 139, 156, 174, 191, 208, 225,
    242, 259, 276, 292, 309, 325, 342, 358, 374, 391, 407, 422, 438,
    454, 469, 485, 500
};

const int g_hifb_cos_table[HIFB_COS_TAB_RANGE] = {
    866, 875, 883, 891, 899, 906, 914, 921, 927, 934, 940,
    946, 951, 956, 961, 966, 970, 974, 978, 982, 985, 988, 990,
    993, 995, 996, 998, 999, 999, 1000, 1000, 1000, 999, 999, 998,
    996, 995, 993, 990, 988, 985, 982, 978, 974, 970, 966, 961,
    956, 951, 946, 940, 934, 927, 921, 914, 906, 899, 891, 883, 875, 866
};

hi_s32 fb_graphic_drv_get_layer_index(hal_disp_layer disp_layer, hi_u32 *layer)
{
    switch (disp_layer) {
        case HAL_DISP_LAYER_GFX0:
            *layer = 0;
            break;
        case HAL_DISP_LAYER_GFX1:
            *layer = 1;
            break;
        case HAL_DISP_LAYER_GFX3:
            *layer = 2; /* 2 is 2 Layer */
            break;
        default:
            return HI_ERR_VO_GFX_INVALID_ID;
    }

    return HI_SUCCESS;
}

hi_s32 fb_graphic_drv_get_layer_id(hi_u32 layer, hal_disp_layer *disp_layer)
{
    switch (layer) {
        case VO_LAYER_G0:
            *disp_layer = HAL_DISP_LAYER_GFX0;
            break;
        case VO_LAYER_G1:
            *disp_layer = HAL_DISP_LAYER_GFX1;
            break;
        case VO_LAYER_G3:
            *disp_layer = HAL_DISP_LAYER_GFX3;
            break;
        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_bool graphic_drv_set_gfx_key_mode(hal_disp_layer layer, hi_u32 key_out)
{
    return hal_graphic_set_gfx_key_mode(layer, key_out);
}

hi_bool graphic_drv_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode)
{
    return hal_graphic_set_gfx_ext(layer, mode);
}

hi_bool graphic_drv_set_gfx_palpha(hal_disp_layer layer, hi_u32 alpha_en, hi_u32 arange, hi_u8 alpha0,
                                   hi_u8 alpha1)
{
    return hal_graphic_set_gfx_palpha(layer, alpha_en, HI_TRUE, alpha0, alpha1);
}

hi_bool graphic_drv_layer_set_layer_galpha(hal_disp_layer layer, hi_u8 alpha0)
{
    return hal_layer_set_layer_galpha(layer, alpha0);
}

hi_bool graphic_drv_layer_set_csc_en(hal_disp_layer layer, hi_bool csc_en)
{
    if (fb_hal_layer_set_csc_en(layer, csc_en) == HI_FALSE) {
        return HI_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0) {
        return HI_TRUE;
    }
    return HI_TRUE;
}

hi_bool graphic_drv_set_layer_addr(hal_disp_layer layer_id, hi_u64 addr)
{
    hi_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(layer_id, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)layer_id);
        return HI_FALSE;
    }

    if (hal_graphic_set_gfx_addr(layer_id, addr) == HI_FALSE) {
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_bool graphic_drv_set_gfx_stride(hal_disp_layer layer, hi_u16 pitch)
{
    hi_u32 layer_index = 0;
    hi_u16 depth;

    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)layer);
        return HI_FALSE;
    }
    /* Stride need be divided by 16(2^4) before setting the register */
    depth = pitch >> 4;
    if (hal_graphic_set_gfx_stride(layer, depth) == HI_FALSE) {
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool graphic_drv_get_gfx_pre_mult(hal_disp_layer layer, hi_u32 *enable)
{
    return hal_graphic_get_gfx_premult(layer, enable);
}

hi_bool graphic_drv_set_gfx_pre_mult(hal_disp_layer layer, hi_u32 enable)
{
    return hal_graphic_set_gfx_pre_mult(layer, enable);
}

hi_bool graphic_drv_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    hi_u32 layer_index = 0;
    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)layer);
        return HI_FALSE;
    }

    if (fb_hal_layer_set_layer_data_fmt(layer, data_fmt) == HI_FALSE) {
        return HI_FALSE;
    }
    graphics_drv_trace(HI_DBG_INFO, "set layer(%u) DataFmt: %d!\n", (hi_u32)layer, data_fmt);
    return HI_TRUE;
}

hi_bool graphic_drv_set_layer_in_rect(hal_disp_layer layer, HIFB_RECT *rect)
{
    hi_u32 layer_index = 0;

    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)layer);
        return HI_FALSE;
    }

    if (fb_hal_layer_set_layer_in_rect(layer, rect) == HI_FALSE) {
        return HI_FALSE;
    }

    if (fb_hal_video_set_layer_disp_rect(layer, rect) == HI_FALSE) {
        return HI_FALSE;
    }
    if (fb_hal_video_set_layer_video_rect(layer, rect) == HI_FALSE) {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*
 * Name : graphic_drv_set_src_image_resolution
 * Desc : set the original resolution of the frame.
 */
hi_bool graphic_drv_set_src_image_resolution(hal_disp_layer layer, HIFB_RECT *rect)
{
    if (fb_hal_layer_set_src_resolution(layer, rect) == HI_FALSE) {
        return HI_FALSE;
    }

    if (fb_hal_layer_set_layer_in_rect(layer, rect) == HI_FALSE) {
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool graphic_drv_set_layer_out_rect(hal_disp_layer layer, HIFB_RECT *rect)
{
    return fb_hal_layer_set_layer_out_rect(layer, rect);
}

hi_bool graphic_drv_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min)
{
    return hal_graphic_set_color_key_value(layer, key_max, key_min);
}

hi_bool graphic_drv_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk)
{
    return hal_graphic_set_color_key_mask(layer, msk);
}

hi_bool graphic_drv_set_gfx_key_en(hal_disp_layer layer, hi_u32 key_enable)
{
    return hal_graphic_set_gfx_key_en(layer, key_enable);
}

hi_bool graphic_drv_set_reg_up(hal_disp_layer layer)
{
    return fb_hal_layer_set_reg_up(layer);
}

hi_bool graphic_drv_get_layer_galpha(hal_disp_layer layer, hi_u8 *alpha0)
{
    return hal_layer_get_layer_galpha(layer, alpha0);
}

hi_bool graphic_drv_get_layer_data_fmt(hal_disp_layer layer, hi_u32 *fmt)
{
    return fb_hal_layer_get_layer_data_fmt(layer, fmt);
}

hi_bool graphic_drv_get_gfx_addr(hal_disp_layer layer, hi_u64 *gfx_addr)
{
    return hal_graphic_get_gfx_addr(layer, gfx_addr);
}

hi_bool graphic_drv_get_gfx_stride(hal_disp_layer layer, hi_u32 *gfx_stride)
{
    return hal_graphic_get_gfx_stride(layer, gfx_stride);
}

hi_bool fb_graphic_drv_get_scan_mode(hifb_vo_dev vo_dev, hi_bool *iop)
{
    return fb_hal_disp_get_disp_iop(vo_dev, iop);
}

hi_bool graphic_drv_enable_dcmp(hal_disp_layer layer, hi_bool enable)
{
    hi_u32 layer_index = 0;
    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)layer);
        return HI_FALSE;
    }
    if (hal_graphic_set_gfx_dcmp_enable(layer, enable) == HI_FALSE) {
        return HI_FALSE;
    }
    hal_fdr_gfx_set_dcmp_en(layer_index, enable);
    return HI_TRUE;
}

hi_bool graphic_drv_get_dcmp_enable_state(hal_disp_layer layer, hi_bool *enable)
{
    return hal_graphic_get_gfx_dcmp_enable_state(layer, enable);
}

hi_bool graphic_drv_get_vt_thd_mode(hifb_vo_dev vo_dev, hi_bool *feild_update)
{
    return hal_disp_get_vt_thd_mode(vo_dev, feild_update);
}

hi_bool fb_graphic_drv_get_int_state(hal_disp_layer gfx_layer, hi_bool *bottom)
{
    return fb_hal_disp_get_int_state((hal_disp_outputchannel)gfx_layer, bottom);
}

static hi_s32 vo_drv_get_csc_matrix(hal_csc_mode csc_mode, const csc_coef **csc_tmp)
{
    hi_u32 loop;
    hi_u32 len;

    vo_drv_csc_mode_coef csc_mode_coef[] = {
        { HAL_CSC_MODE_BT601_TO_BT601,  &g_fb_csc_init },
        { HAL_CSC_MODE_BT709_TO_BT709,  &g_fb_csc_init },
        { HAL_CSC_MODE_RGB_TO_RGB,      &g_fb_csc_init },
        { HAL_CSC_MODE_BT709_TO_BT601,  &g_fb_csc_yuv2yuv_709_601 },
        { HAL_CSC_MODE_BT601_TO_BT709,  &g_fb_csc_yuv2yuv_601_709 },
        { HAL_CSC_MODE_BT601_TO_RGB_PC, &g_csc_yuv6012rgb_pc },
        { HAL_CSC_MODE_BT709_TO_RGB_PC, &g_csc_yuv7092rgb_pc },
        { HAL_CSC_MODE_RGB_TO_BT601_PC, &g_fb_csc_rgb2yuv601_pc },
        { HAL_CSC_MODE_RGB_TO_BT709_PC, &g_fb_csc_rgb2yuv709_pc },
        { HAL_CSC_MODE_RGB_TO_BT601_TV, &g_fb_csc_rgb2yuv601_tv },
        { HAL_CSC_MODE_RGB_TO_BT709_TV, &g_fb_csc_rgb2yuv709_tv },
    };

    len = sizeof(csc_mode_coef) / sizeof(vo_drv_csc_mode_coef);
    for (loop = 0; loop < len; loop++) {
        if (csc_mode_coef[loop].csc_mode == csc_mode) {
            *csc_tmp = csc_mode_coef[loop].coef;
            return HI_SUCCESS;
        }
    }
    return HI_FAILURE;
}

static hi_void hifb_drv_yuvtorgb(hi_s32 hue, hi_s32 contrast, hi_s32 satu, csc_coef *cstcoef,
                                 const csc_coef *csctmp)
{
    cstcoef->csc_coef00 = (contrast * csctmp->csc_coef00) / HIFB_CSC_COEF_ALG_DATA;

    cstcoef->csc_coef01 = (contrast * satu * ((csctmp->csc_coef01 * g_hifb_cos_table[hue] - csctmp->csc_coef02 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef02 = (contrast * satu * ((csctmp->csc_coef01 * g_hifb_sin_table[hue] + csctmp->csc_coef02 *
        g_hifb_cos_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef10 = (contrast * csctmp->csc_coef10) / HIFB_CSC_COEF_ALG_DATA;

    cstcoef->csc_coef11 = (contrast * satu * ((csctmp->csc_coef11 * g_hifb_cos_table[hue] - csctmp->csc_coef12 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef12 = (contrast * satu * ((csctmp->csc_coef11 * g_hifb_sin_table[hue] + csctmp->csc_coef12 *
        g_hifb_cos_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef20 = (contrast * csctmp->csc_coef20) / HIFB_CSC_COEF_ALG_DATA;

    cstcoef->csc_coef21 = (contrast * satu * ((csctmp->csc_coef21 * g_hifb_cos_table[hue] - csctmp->csc_coef22 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef22 = (contrast * satu * ((csctmp->csc_coef21 * g_hifb_sin_table[hue] + csctmp->csc_coef22 *
        g_hifb_cos_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    return;
}

static hi_void hifb_drv_rgbtoyuv(hi_s32 hue, hi_s32 contrast, hi_s32 satu, csc_coef *cstcoef,
                                 const csc_coef *csctmp)
{
    cstcoef->csc_coef00 = (contrast * csctmp->csc_coef00) / HIFB_CSC_COEF_ALG_DATA;

    cstcoef->csc_coef01 = (contrast * csctmp->csc_coef01) / HIFB_CSC_COEF_ALG_DATA;

    cstcoef->csc_coef02 = (contrast * csctmp->csc_coef02) / HIFB_CSC_COEF_ALG_DATA;

    cstcoef->csc_coef10 = (contrast * satu * ((csctmp->csc_coef10 * g_hifb_cos_table[hue] + csctmp->csc_coef20 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef11 = (contrast * satu * ((csctmp->csc_coef11 * g_hifb_cos_table[hue] + csctmp->csc_coef21 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef12 = (contrast * satu * ((csctmp->csc_coef12 * g_hifb_cos_table[hue] + csctmp->csc_coef22 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef20 = (contrast * satu * ((csctmp->csc_coef20 * g_hifb_cos_table[hue] - csctmp->csc_coef10 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef21 = (contrast * satu * ((csctmp->csc_coef21 * g_hifb_cos_table[hue] - csctmp->csc_coef11 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    cstcoef->csc_coef22 = (contrast * satu * ((csctmp->csc_coef22 * g_hifb_cos_table[hue] - csctmp->csc_coef12 *
        g_hifb_sin_table[hue]) / HIFB_CSC_STATU_ALG_DATA)) / HIFB_CSC_HUE_ALG_DATA;

    return;
}

/*
 * Name : fb_vou_drv_calc_csc_matrix
 * Desc : Calculate csc matrix.
 */
hi_s32 fb_vou_drv_calc_csc_matrix(fb_vo_csc *gfx_csc, hal_csc_mode csd_mode, csc_coef *coef)
{
    hi_s32 luma, contrast, hue, satu;

    const csc_coef *csc_tmp = HI_NULL;
    luma = (hi_s32)gfx_csc->luma * 64 / 100 - 32;          /* 100 64 32 alg data */
    contrast = ((hi_s32)gfx_csc->contrast - 50) * 2 + 100; /* 100 50 2 alg data */
    hue = (hi_s32)gfx_csc->hue * 60 / 100;                 /* 100 60 alg data */
    satu = ((hi_s32)gfx_csc->satuature - 50) * 2 + 100;    /* 100 50 2 alg data */

    if (vo_drv_get_csc_matrix(csd_mode, (const csc_coef **)&csc_tmp) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    coef->csc_in_dc0 = csc_tmp->csc_in_dc0;
    coef->csc_in_dc1 = csc_tmp->csc_in_dc1;
    coef->csc_in_dc2 = csc_tmp->csc_in_dc2;
    coef->csc_out_dc0 = csc_tmp->csc_out_dc0;
    coef->csc_out_dc1 = csc_tmp->csc_out_dc1;
    coef->csc_out_dc2 = csc_tmp->csc_out_dc2;
    /*
     *  C_ratio normally is 0~1.99, C_ratio=contrast/100
     *  S normally is 0~1.99,S=satu/100
     *  Hue -30~30, using the lut to get COS and SIN and then /1000
     */
    if ((csd_mode == HAL_CSC_MODE_BT601_TO_RGB_PC) || (csd_mode == HAL_CSC_MODE_BT709_TO_RGB_PC) ||
        (csd_mode == HAL_CSC_MODE_BT601_TO_RGB_TV) || (csd_mode == HAL_CSC_MODE_BT709_TO_RGB_TV)) {
        /* YUV->RGB convert, RGB->YUV convert */
        hifb_drv_yuvtorgb(hue, contrast, satu, coef, csc_tmp);
        coef->csc_in_dc0 += (contrast != 0) ? (luma * 100 / contrast) : luma * 100; /* 100 contrast data */
    } else {
        /* RGB->YUV convert, YUV->RGB convert, YUV->YUV */
        hifb_drv_rgbtoyuv(hue, contrast, satu, coef, csc_tmp);
        coef->csc_out_dc0 += luma;
    }

    return HI_SUCCESS;
}

hi_bool graphic_drv_get_dev_enable(hifb_vo_dev vo_dev, hi_bool *intf_en)
{
    return fb_hal_disp_get_intf_enable(vo_dev, intf_en);
}

hi_bool graphic_drv_get_intf_sync(hifb_vo_dev vo_dev, hal_disp_syncinfo *sync_info)
{
    return hal_disp_get_intf_sync(vo_dev, sync_info);
}

hi_bool graphic_drv_get_intf_mux_sel(hifb_vo_dev vo_dev, VO_INTF_TYPE_E *ben_intf_type)
{
    return hal_disp_get_intf_mux_sel(vo_dev, ben_intf_type);
}

hi_s32 graphic_drv_allocate_mem(hi_u32 size, hifb_mmz_buffer *vdp_mmz_buffer)
{
    hi_s32 ret;

    hi_void *mmz_name = HI_NULL;
    hi_mpp_chn chn;

    chn.mod_id = HI_ID_FB;
    chn.dev_id = 0;
    chn.chn_id = 0;

    if (func_entry(sys_export_func, HI_ID_SYS)->pfn_get_mmz_name(&chn, &mmz_name)) {
        hifb_graphics_trace(HI_DBG_ERR, "get mmz name fail!\n");
        return HI_FAILURE;
    }

    ret = cmpi_mmz_malloc_nocache(mmz_name, "HIFB COEF", &vdp_mmz_buffer->start_phy_addr,
                                  &vdp_mmz_buffer->start_vir_addr, size);
    if (ret != 0) {
        hifb_graphics_trace(HI_DBG_ERR, "HIFB DDR CFG  failed\n");
        return HI_FAILURE;
    }
    osal_memset(vdp_mmz_buffer->start_vir_addr, 0, size);
    return HI_SUCCESS;
}

hi_void graphic_drv_delete_mem(hifb_mmz_buffer *vdp_mmz_buffer)
{
    if (vdp_mmz_buffer != HI_NULL) {
        cmpi_mmz_free(vdp_mmz_buffer->start_phy_addr, vdp_mmz_buffer->start_vir_addr);
        vdp_mmz_buffer->start_phy_addr = 0;
        vdp_mmz_buffer->start_vir_addr = HI_NULL;
    }
}

hi_s32 graphic_drv_vhd_coef_buf_addr_distribute(hifb_coef_addr *vdp_coef_buf_addr)
{
    vdp_coef_buf_addr->coef_vir_addr[HIFB_COEF_BUF_G0ZME] = vdp_coef_buf_addr->buf_base_addr.start_vir_addr;
    vdp_coef_buf_addr->coef_phy_addr[HIFB_COEF_BUF_G0ZME] = vdp_coef_buf_addr->buf_base_addr.start_phy_addr;

    hal_para_set_para_addr_vhd_chn06(vdp_coef_buf_addr->coef_phy_addr[HIFB_COEF_BUF_G0ZME]);

    return HI_SUCCESS;
}

hi_s32 graphic_zme_coef_init(hi_void)
{
    /* hifb only need zme coef size */
    if (graphic_drv_allocate_mem(COEF_SIZE_G0ZME, &g_hifb_coef_buf_addr.buf_base_addr) != HI_SUCCESS ||
        graphic_drv_vhd_coef_buf_addr_distribute(&g_hifb_coef_buf_addr) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 fb_graphic_drv_init(hi_void)
{
    hi_s32 i;
    vo_gfxlayer_context *vo_gfx_ctx = HI_NULL;
    vo_gfxlayer_context *gfx_layer = get_gfx_layer_ctx();

    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        vo_gfx_ctx = &gfx_layer[i];

        vo_gfx_ctx->layer_id = HAL_DISP_LAYER_GFX0 + i; /* HAL_DISP_LAYER_GFX0+1 */
        vo_gfx_ctx->binded_dev = i;                     /* 0 */
        vo_gfx_ctx->binded = HI_TRUE;
    }

    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        vo_gfx_ctx = &gfx_layer[i];
        vo_gfx_ctx->gfx_csc.csc_matrix = FB_VO_CSC_MATRIX_RGB_TO_BT601_TV;
        vo_gfx_ctx->gfx_csc.luma = 50;      /* 50 is gfx csc date */
        vo_gfx_ctx->gfx_csc.contrast = 50;  /* 50 is gfx csc date */
        vo_gfx_ctx->gfx_csc.hue = 50;       /* 50 is gfx csc date */
        vo_gfx_ctx->gfx_csc.satuature = 50; /* 50 is gfx csc date */

        /* CSC extra coef */
        vo_gfx_ctx->coef_param.csc_scale2p = GFX_CSC_SCALE;
        vo_gfx_ctx->coef_param.csc_clip_min = GFX_CSC_CLIP_MIN;
        vo_gfx_ctx->coef_param.csc_clip_max = GFX_CSC_CLIP_MAX;
    }

    /* DDR detect coef */
    gfx_layer[0].start_section = 0;
    gfx_layer[0].zone_nums = 16; /* 16 is alg date */
    return HI_SUCCESS;
}

hi_s32 graphic_drv_get_bind_dev(hi_s32 layer_id)
{
    vo_gfxlayer_context *gfx_layer = get_gfx_layer_ctx();
    return gfx_layer[layer_id].binded_dev;
}

hi_s32 fb_graphic_drv_exit(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 graphic_drv_resource_init(hi_void)
{
    hi_s32 i;
    vo_gfxlayer_context *gfx_layer = get_gfx_layer_ctx();

    if (fb_hal_vou_init() != HI_SUCCESS) {
        return HI_FAILURE;
    }
    /* lock init */
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        osal_memset(&gfx_layer[i], 0, sizeof(vo_gfxlayer_context));
        gfx_spin_lock_init(&gfx_layer[i].spin_lock);
    }

    /* mem alloc */
    if (graphic_zme_coef_init() != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 graphic_drv_resource_exit(hi_void)
{
    hi_s32 i;
    vo_gfxlayer_context *vo_gfx_ctx = HI_NULL;
    vo_gfxlayer_context *gfx_layer = get_gfx_layer_ctx();
    /* lock deinit */
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        vo_gfx_ctx = &gfx_layer[i];
        gfx_spin_lock_deinit(&vo_gfx_ctx->spin_lock);
    }
    /* mem delete */
    graphic_drv_delete_mem(&g_hifb_coef_buf_addr.buf_base_addr);
    fb_hal_vou_exit();

    return HI_SUCCESS;
}

hi_s32 graphic_drv_enable_layer(hal_disp_layer gfx_layer, hi_bool enabled)
{
    if (fb_hal_layer_enable_layer(gfx_layer, enabled) == HI_FALSE) {
        graphics_drv_trace(HI_DBG_ERR, "graphics layer %d Enable failed!\n", gfx_layer);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 fb_graphic_drv_set_csc_coef(hal_disp_layer gfx_layer, fb_vo_csc *gfx_csc, csc_coef_param *coef_param)
{
    csc_coef coef;
    hal_csc_mode csc_mode;

    hi_u32 layer_index;
    vo_gfxlayer_context *gfx_layer_ctx = get_gfx_layer_ctx();

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_FAILURE;
    }
    if (gfx_csc->csc_matrix == FB_VO_CSC_MATRIX_RGB_TO_BT601_PC) {
        csc_mode = HAL_CSC_MODE_RGB_TO_BT601_PC;
    } else if (gfx_csc->csc_matrix == FB_VO_CSC_MATRIX_RGB_TO_BT709_PC) {
        csc_mode = HAL_CSC_MODE_RGB_TO_BT709_PC;
    } else if (gfx_csc->csc_matrix == FB_VO_CSC_MATRIX_RGB_TO_BT601_TV) {
        csc_mode = HAL_CSC_MODE_RGB_TO_BT601_TV;
    } else if (gfx_csc->csc_matrix == FB_VO_CSC_MATRIX_RGB_TO_BT709_TV) {
        csc_mode = HAL_CSC_MODE_RGB_TO_BT709_TV;
    } else {
        graphics_drv_trace(HI_DBG_ERR, "csc_matrix %d err, should only be RGB to YUV matrix\n", gfx_csc->csc_matrix);
        return HI_ERR_VO_ILLEGAL_PARAM;
    }
    /* cal CSC coef and CSC dc coef */
    if (fb_vou_drv_calc_csc_matrix(gfx_csc, csc_mode, &coef) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    coef.new_csc_clip_max = GFX_CSC_CLIP_MAX;
    coef.new_csc_clip_min = GFX_CSC_CLIP_MIN;
    coef.new_csc_scale2p = GFX_CSC_SCALE;

    coef.csc_coef00 = HIFB_COEF_CSC_DEX * coef.csc_coef00 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef01 = HIFB_COEF_CSC_DEX * coef.csc_coef01 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef02 = HIFB_COEF_CSC_DEX * coef.csc_coef02 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef10 = HIFB_COEF_CSC_DEX * coef.csc_coef10 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef11 = HIFB_COEF_CSC_DEX * coef.csc_coef11 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef12 = HIFB_COEF_CSC_DEX * coef.csc_coef12 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef20 = HIFB_COEF_CSC_DEX * coef.csc_coef20 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef21 = HIFB_COEF_CSC_DEX * coef.csc_coef21 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;
    coef.csc_coef22 = HIFB_COEF_CSC_DEX * coef.csc_coef22 * HIFB_COEF_CSC_GFX / HIFB_CSC_STATU_ALG_DATA;

    coef.csc_in_dc0 = HIFB_COEF_CSC_DC_ALG_DATA * coef.csc_in_dc0;
    coef.csc_in_dc1 = HIFB_COEF_CSC_DC_ALG_DATA * coef.csc_in_dc1;
    coef.csc_in_dc2 = HIFB_COEF_CSC_DC_ALG_DATA * coef.csc_in_dc2;

    coef.csc_out_dc0 = HIFB_COEF_CSC_DC_ALG_DATA * coef.csc_out_dc0;
    coef.csc_out_dc1 = HIFB_COEF_CSC_DC_ALG_DATA * coef.csc_out_dc1;
    coef.csc_out_dc2 = HIFB_COEF_CSC_DC_ALG_DATA * coef.csc_out_dc2;

    /* set CSC coef and CSC dc coef */
    fb_hal_layer_set_csc_coef(gfx_layer, &coef);

    /* copy to drv */
    osal_memcpy(&gfx_layer_ctx[layer_index].gfx_csc, gfx_csc, sizeof(fb_vo_csc));
    osal_memcpy(&gfx_layer_ctx[layer_index].coef_param, coef_param, sizeof(csc_coef_param));

    return HI_SUCCESS;
}

hi_void gf_func_set_g0zme_mode(hi_u32 layer, gf_g0_zme_mode g0zme_mode, gf_g0_zme_cfg *cfg)
{
    /* filed declare */
    hi_u32 hfir_order = 1;
    hi_s32 lhfir_offset = 0;
    hi_s32 chfir_offset = 0;
    hi_s32 vtp_offset = 0;
    hi_s32 vbtm_offset = 0;
    hi_u32 hratio, vratio;
    hi_ulong zme_hprec = (1 << 20); /* 20 bit data */
    hi_ulong zme_vprec = (1 << 12); /* 12 bit data */

    if ((cfg->out_width == 0) || (cfg->out_height == 0)) {
        return;
    }
    hratio = (cfg->in_width * zme_hprec) / cfg->out_width;
    vratio = (cfg->in_height * zme_vprec) / cfg->out_height;

    if (g0zme_mode == VDP_G0_ZME_TYP) {
        lhfir_offset = 0;
        chfir_offset = 0;
        vtp_offset = 0;
        vbtm_offset = (-1) * (hi_s64)zme_vprec / 2; /* 2 offset data */
    }

    /* drv transfer */
    hal_g0_zme_set_ck_gt_en(cfg->ck_gt_en);
    hal_g0_zme_set_out_width(cfg->out_width);
    hal_g0_zme_set_hfir_en(cfg->hfir_en);
    hal_g0_zme_set_ahfir_mid_en(cfg->ahmid_en);
    hal_g0_zme_set_lhfir_mid_en(cfg->lhmid_en);
    hal_g0_zme_set_chfir_mid_en(cfg->lhmid_en);
    hal_g0_zme_set_lhfir_mode(cfg->lhfir_mode);
    hal_g0_zme_set_ahfir_mode(cfg->ahfir_mode);
    hal_g0_zme_set_hfir_order(hfir_order);
    hal_g0_zme_set_hratio(hratio);
    hal_g0_zme_set_lhfir_offset(lhfir_offset);
    hal_g0_zme_set_chfir_offset(chfir_offset);
    hal_g0_zme_set_out_pro(cfg->out_pro);
    hal_g0_zme_set_out_height(cfg->out_height);
    hal_g0_zme_set_vfir_en(cfg->vfir_en);
    hal_g0_zme_set_avfir_mid_en(cfg->avmid_en);
    hal_g0_zme_set_lvfir_mid_en(cfg->lvmid_en);
    hal_g0_zme_set_cvfir_mid_en(cfg->lvmid_en);
    hal_g0_zme_set_lvfir_mode(cfg->lvfir_mode);
    hal_g0_zme_set_vafir_mode(cfg->avfir_mode);
    hal_g0_zme_set_vratio(vratio);
    hal_g0_zme_set_vtp_offset(vtp_offset);
    hal_g0_zme_set_vbtm_offset(vbtm_offset);
}

hi_u32 vo_drv_write_ddr(hi_u8 *addr, hifb_drv_u128 *data128)
{
    hi_u32 ii;
    hi_u32 data_arr[HIFB_DATEARR_RANGE] = {
        data128->data0, data128->data1,
        data128->data2, data128->data3
    };
    hi_u8 *addr_tmp = HI_NULL;

    hi_u32 data_tmp = 0;

    for (ii = 0; ii < HIFB_DATEARR_RANGE; ii++) {
        addr_tmp = addr + ii * HIFB_DATEARR_RANGE;
        data_tmp = data_arr[ii];
        *(hi_u32 *)addr_tmp = data_tmp;
    }

    return 0;
}

hi_void vo_drv_push128(hifb_drv_u128 *data128, hi_u32 coef_data, hi_u32 bit_len)
{
    coef_data = coef_data & (0xFFFFFFFF >> (32 - bit_len)); /* 0xFFFFFFFF 32 bit len */

    if (data128->depth < 32) {                  /* 32 max depth */
        if ((data128->depth + bit_len) <= 32) { /* 32 bit len */
            data128->data0 = (coef_data << data128->depth) | data128->data0;
        } else {
            data128->data0 = (coef_data << data128->depth) | data128->data0;
            data128->data1 = coef_data >> (32 - data128->depth % 32); /* 32 depth data */
        }
    } else if ((data128->depth >= 32) && (data128->depth < 64)) { /* 32 64 depth data */
        if ((data128->depth + bit_len) <= 64) {
            data128->data1 = (coef_data << (data128->depth % 32)) | data128->data1; /* 32 depth data */
        } else {
            data128->data1 = (coef_data << (data128->depth % 32)) | data128->data1; /* 32 depth data */
            data128->data2 = coef_data >> (32 - data128->depth % 32);               /* 32 depth data */
        }
    } else if ((data128->depth >= 64) && (data128->depth < 96)) {                   /* 96 64 depth data */
        if ((data128->depth + bit_len) <= 96) {                                     /* 96 depth data */
            data128->data2 = (coef_data << (data128->depth % 32)) | data128->data2; /* 32 depth data */
        } else {
            data128->data2 = (coef_data << (data128->depth % 32)) | data128->data2; /* 32 depth data */
            data128->data3 = coef_data >> (32 - data128->depth % 32);               /* 32 depth data */
        }
    } else if (data128->depth >= 96) {                                              /* 96 depth data */
        if ((data128->depth + bit_len) <= 128) {                                    /* 128 depth data */
            data128->data3 = (coef_data << (data128->depth % 32)) | data128->data3; /* 32 depth data */
        }
    }

    data128->depth = data128->depth + bit_len;
    return;
}

hi_u32 vo_drv_find_max(hi_u32 *array, hi_u32 num)
{
    hi_u32 ii;
    hi_u32 tmp_data = array[0];

    for (ii = 1; ii < num; ii++) {
        if (tmp_data < array[ii]) {
            tmp_data = array[ii];
        }
    }

    return tmp_data;
}

hi_void send_coef_coef_array(hifb_drv_coef_send_cfg *cfg, hifb_drv_u128 *data128, hi_u32 coef_cnt)
{
    hi_s32 tmp_data = 0;
    hi_u32 nn;
    for (nn = 0; nn < cfg->lut_num; nn++) {
        switch (cfg->data_type) {
            case DRV_COEF_DATA_TYPE_S16:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((hi_s16 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_U16:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((hi_u16 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_U32:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((hi_u32 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_S32:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((hi_s32 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_S8:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((hi_s8 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_U8:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((hi_u8 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            default:
                break;
        }

        vo_drv_push128(data128, tmp_data, cfg->coef_bit_length[nn]);
    }
    return;
}

hi_u8 *vo_drv_send_coef(hifb_drv_coef_send_cfg *cfg)
{
    hi_u32 ii, kk, mm;
    hi_u8 *addr_base = cfg->coef_addr;
    hi_u32 addr_offset = 0;
    hi_u8 *addr = HI_NULL;
    hi_u32 cycle_num, coef_cnt, total_burst_num, max_len;
    hi_u32 total_bit_len = 0;
    hifb_drv_u128 data128 = { 0 };

    addr_base = cfg->coef_addr;

    /* data type convertion */
    addr = addr_base;

    cycle_num = cfg->cycle_num;

    for (ii = 0; ii < cfg->lut_num; ii++) {
        total_bit_len = total_bit_len + cfg->coef_bit_length[ii];
    }

    /* send data */
    max_len = vo_drv_find_max(cfg->lut_length, cfg->lut_num);
    if ((cfg->burst_num == 1) && ((cfg->data_type < DRV_COEF_DATA_TYPE_BUTT) && (cfg->data_type >= 0))) {
        if (cycle_num == 0) {
            return HI_NULL;
        }
        total_burst_num = (max_len + cycle_num - 1) / cycle_num;
        for (kk = 0; kk < total_burst_num; kk++) {
            osal_memset((hi_void *)&data128, 0, sizeof(data128));

            for (mm = 0; mm < cycle_num; mm++) {
                coef_cnt = kk * cycle_num + mm;
                send_coef_coef_array(cfg, &data128, coef_cnt);
            }
            addr = addr_base + addr_offset;
            addr_offset = addr_offset + 16; /* 16 alg data */
            vo_drv_write_ddr(addr, &data128);
        }
    }

    return (addr_base + addr_offset);
}

hi_void gf_drv_set_g0zme_coef(hi_s16 *coef_h, hi_s16 *coef_v)
{
    hifb_drv_coef_send_cfg coef_send;
    hi_u8 *addr = HI_NULL;

    hi_void *coef_array[1] = { coef_h };
    hi_u32 lut_length[1] = { 64 }; /* 64 is max lut length */
    hi_u32 coef_bit_length[1] = { 16 }; /* 16 is max bit length */

    addr = g_hifb_coef_buf_addr.coef_vir_addr[HIFB_COEF_BUF_G0ZME];

    coef_send.coef_addr = addr;
    coef_send.lut_num = 1;
    coef_send.burst_num = 1;
    coef_send.cycle_num = 8; /* 8 is cycle NUM */
    coef_send.coef_array = coef_array;
    coef_send.lut_length = lut_length;
    coef_send.coef_bit_length = coef_bit_length;
    coef_send.data_type = DRV_COEF_DATA_TYPE_S16;

    addr = vo_drv_send_coef(&coef_send);
    coef_array[0] = coef_v;
    lut_length[0] = 128;     /* 128 is length */
    coef_bit_length[0] = 16; /* 16 is length */

    coef_send.coef_addr = addr;
    coef_send.cycle_num = 8; /* 8 is cycle NUM */
    coef_send.coef_array = coef_array;
    coef_send.lut_length = lut_length;
    coef_send.coef_bit_length = coef_bit_length;
    coef_send.data_type = DRV_COEF_DATA_TYPE_S16;

    addr = vo_drv_send_coef(&coef_send);
    if (addr == HI_NULL) {
        hifb_error(" vo_drv_send_coef HI_NULL\r\n");
    }
}

#define HIFB_COEF_V_RANGE     16
#define HIFB_COEF_H_RANGE     8
#define HIFB_COEF_RANGE       8
#define HIFB_COEF_V_NEW_RANGE 128
#define HIFB_COEF_H_NEW_RANGE 64

hi_s16 g_coef_v[HIFB_COEF_V_RANGE][HIFB_COEF_RANGE] = { { 0, 0, 0, 0, 0, 63, 0, 0 }, { 0, 0, 0, 0, 6, 51, 13, -6 },
    { 0, 0, 0, 0, 4, 51, 16, -7 }, { 0, 0, 0, 0, 1, 50, 20, -7 },
    { 0, 0, 0, 0, -1, 49, 24, -8 }, { 0, 0, 0, 0, -3, 47, 28, -8 },
    { 0, 0, 0, 0, -4, 45, 31, -8 }, { 0, 0, 0, 0, -6, 42, 35, -7 },
    { 0, 0, 0, 0, -7, 39, 39, -7 }, { 0, 0, 0, 0, -7, 35, 42, -6 },
    { 0, 0, 0, 0, -8, 31, 45, -4 }, { 0, 0, 0, 0, -8, 28, 47, -3 },
    { 0, 0, 0, 0, -8, 24, 49, -1 }, { 0, 0, 0, 0, -7, 20, 50, 1 },
    { 0, 0, 0, 0, -7, 16, 51, 4 }, { 0, 0, 0, 0, -6, 13, 51, 6 }
};

hi_s16 g_coef_h[HIFB_COEF_H_RANGE][HIFB_COEF_RANGE] = { { 0, 0, 0, 0, 63, 0, 0, 0 }, { 0, 0, -4, 4, 52, 16, -6, 2 },
    { 0, 0, -2, -1, 48, 24, -7, 2 }, { 0, 0, -1, -4, 44, 31, -7, 1 },
    { 0, 0, 1, -7, 38, 38, -7, 1 }, { 0, 0, 1, -7, 31, 44, -4, -1 },
    { 0, 0, 2, -7, 24, 48, -1, -2 }, { 0, 0, 2, -6, 16, 52, 4, -4 }
};

hi_s16 g_coef_h_new[HIFB_COEF_H_NEW_RANGE];
hi_s16 g_coef_v_new[HIFB_COEF_V_NEW_RANGE];

hi_void gf_vset_g0zme_coef(gf_rm_coef_mode coef_mode)
{
    hi_u32 ii;

    if (coef_mode == GF_RM_COEF_MODE_TYP) {
        for (ii = 0; ii < 8; ii++) {                    /* 8 order buffer */
            g_coef_h_new[ii * 8 + 0] = g_coef_h[ii][7]; /* calc 0index coef for 8 order coef from 7index old */
            g_coef_h_new[ii * 8 + 1] = g_coef_h[ii][6]; /* calc 1index coef for 8 order coef from 6index old */
            g_coef_h_new[ii * 8 + 2] = g_coef_h[ii][5]; /* calc 2index coef for 8 order coef from 5index old */
            g_coef_h_new[ii * 8 + 3] = g_coef_h[ii][4]; /* calc 3index coef for 8 order coef from 4index old */
            g_coef_h_new[ii * 8 + 4] = g_coef_h[ii][3]; /* calc 4index coef for 8 order coef from 3index old */
            g_coef_h_new[ii * 8 + 5] = g_coef_h[ii][2]; /* calc 5index coef for 8 order coef from 2index old */
            g_coef_h_new[ii * 8 + 6] = g_coef_h[ii][1]; /* calc 6index coef for 8 order coef from 1index old */
            g_coef_h_new[ii * 8 + 7] = g_coef_h[ii][0]; /* calc 7index coef for 8 order coef from 0index old */
        }

        for (ii = 0; ii < 16; ii++) {                   /* 16 order buffer */
            g_coef_v_new[ii * 8 + 0] = g_coef_v[ii][7]; /* calc 0index coef for 8 order coef from 7index old */
            g_coef_v_new[ii * 8 + 1] = g_coef_v[ii][6]; /* calc 1index coef for 8 order coef from 6index old */
            g_coef_v_new[ii * 8 + 2] = g_coef_v[ii][5]; /* calc 2index coef for 8 order coef from 5index old */
            g_coef_v_new[ii * 8 + 3] = g_coef_v[ii][4]; /* calc 3index coef for 8 order coef from 4index old */
            g_coef_v_new[ii * 8 + 4] = g_coef_v[ii][3]; /* calc 4index coef for 8 order coef from 3index old */
            g_coef_v_new[ii * 8 + 5] = g_coef_v[ii][2]; /* calc 5index coef for 8 order coef from 2index old */
            g_coef_v_new[ii * 8 + 6] = g_coef_v[ii][1]; /* calc 6index coef for 8 order coef from 1index old */
            g_coef_v_new[ii * 8 + 7] = g_coef_v[ii][0]; /* calc 7index coef for 8 order coef from 0index old */
        }
    }

    /* send coef to DDR */
    gf_drv_set_g0zme_coef(g_coef_h_new, g_coef_v_new);
}

hi_bool graphic_drv_enable_zme(hi_u32 layer, gf_g0_zme_cfg *zme_cfg, hi_bool enable_zme)
{
    gf_g0_zme_cfg cfg;

    if ((zme_cfg->in_width == 0) || (zme_cfg->in_height == 0) || (zme_cfg->out_width == 0) ||
        (zme_cfg->out_height == 0)) {
        return HI_FALSE;
    }

    cfg.ck_gt_en = 1;
    cfg.out_pro = VDP_RMODE_PROGRESSIVE;

    cfg.in_width = zme_cfg->in_width;
    cfg.in_height = zme_cfg->in_height;
    cfg.out_width = zme_cfg->out_width;
    cfg.out_height = zme_cfg->out_height;
    cfg.lhmid_en = 1;
    cfg.ahmid_en = 1;
    cfg.lhfir_mode = 1;
    cfg.ahfir_mode = 1;
    cfg.lvmid_en = 1;
    cfg.avmid_en = 1;
    cfg.lvfir_mode = 1;
    cfg.avfir_mode = 1;

    if (enable_zme) {
        cfg.hfir_en = 1;
        cfg.vfir_en = 1;

        gf_func_set_g0zme_mode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &cfg);

        /* It will be reset when VO exit, so config again */
        graphic_drv_vhd_coef_buf_addr_distribute(&g_hifb_coef_buf_addr);
        gf_vset_g0zme_coef(GF_RM_COEF_MODE_TYP);
        hal_para_set_para_up_vhd_chn(HIFB_COEF_BUF_G0ZME);
    } else {
        cfg.hfir_en = 0;
        cfg.vfir_en = 0;

        gf_func_set_g0zme_mode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &cfg);
    }

    return HI_TRUE;
}

hi_void graphic_drv_dev_int_enable(hifb_vo_dev vo_dev, hi_bool enable)
{
    hifb_int_mask int_type;

    switch (vo_dev) {
        case VO_DEV_DHD0:
            int_type = HIFB_INTMSK_DHD0_VTTHD2;
            break;

        case VO_DEV_DHD1:
            int_type = HIFB_INTMSK_DHD1_VTTHD2;
            break;

        default:
            return;
    }

    if (enable == HI_TRUE) {
        fb_hal_disp_set_int_mask(int_type);
    } else {
        fb_hal_disp_clr_int_mask(int_type);
    }

    return;
}

hi_void graphic_drv_int_clear(hi_u32 int_clear, hi_s32 irq)
{
    /* vo double interrupt, read and mask use interrupt 1, but clear use interrupt 0 as video */
    fb_hal_disp_clear_int_status(int_clear);
    return;
}

hi_u32 graphic_drv_int_get_status(hi_void)
{
    return fb_hal_disp_get_int_status(HIFB_INTREPORT_ALL);
}

hi_void graphic_drv_clr_int_status(hi_u32 int_status)
{
    if (int_status & HIFB_INTMSK_DHD0_VTTHD2) {
        graphic_drv_int_clear(HIFB_INTMSK_DHD0_VTTHD2, VOU1_IRQ_NR);
    }

    if (int_status & HIFB_INTMSK_DHD0_VTTHD3) {
        graphic_drv_int_clear(HIFB_INTMSK_DHD0_VTTHD3, VOU1_IRQ_NR);
    }

    if (int_status & HIFB_INTMSK_DHD1_VTTHD2) {
        graphic_drv_int_clear(HIFB_INTMSK_DHD1_VTTHD2, VOU1_IRQ_NR);
    }

    if (int_status & HIFB_INTMSK_DHD1_VTTHD3) {
        graphic_drv_int_clear(HIFB_INTMSK_DHD1_VTTHD3, VOU1_IRQ_NR);
    }

    return;
}

hi_s32 graphic_drv_get_interrupt_dev(hi_u32 int_status, hifb_vo_dev *vo_dev)
{
    if (int_status & HIFB_INTMSK_DHD0_VTTHD2) {
        graphics_drv_trace(HI_DBG_DEBUG, "Graphic: DHD0 INTTERRUPT\n");
        graphic_drv_int_clear(HIFB_INTMSK_DHD0_VTTHD2, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD0;
    } else if (int_status & HIFB_INTMSK_DHD0_VTTHD3) {
        graphics_drv_trace(HI_DBG_DEBUG, "Graphic: DHD0 INTTERRUPT\n");
        graphic_drv_int_clear(HIFB_INTMSK_DHD0_VTTHD3, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD0;
    } else if (int_status & HIFB_INTMSK_DHD1_VTTHD2) {
        graphics_drv_trace(HI_DBG_DEBUG, "Graphic: DHD1 INTTERRUPT\n");
        graphic_drv_int_clear(HIFB_INTMSK_DHD1_VTTHD2, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD1;
    } else if (int_status & HIFB_INTMSK_DHD1_VTTHD3) {
        graphics_drv_trace(HI_DBG_DEBUG, "Graphic: DHD1 INTTERRUPT\n");
        graphic_drv_int_clear(HIFB_INTMSK_DHD1_VTTHD3, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD1;
    } else {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
