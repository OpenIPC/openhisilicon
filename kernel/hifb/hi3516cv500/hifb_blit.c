/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_blit.h"
#include <linux/hardirq.h>
#include "hi_tde_ext.h"
#include "mod_ext.h"

#ifdef __HuaweiLite__
#define in_atomic() (HI_FALSE)
#endif

#ifdef HI_FPGA
#define HIFB_TIME_OUT_FROM_TDE 432
#else
#define HIFB_TIME_OUT_FROM_TDE 100
#endif

static int_callback g_tde_call_back;
static int_callback g_tde_rotate_call_back;

#define fb_drv_tde_end_job_val(end_job, thandle, tis_block, ttime_out, tis_sync) \
    do {                                                                         \
        (end_job).handle = thandle;                                              \
        (end_job).is_block = tis_block;                                          \
        (end_job).is_sync = tis_sync;                                            \
        (end_job).time_out = ttime_out;                                          \
    } while (0)

/* tde support which color format */
static hi_bool g_tde_col_fmt[HIFB_FMT_BUTT] = {
    HI_FALSE, /* HIFB_FMT_RGB565 */
    HI_FALSE, /* HIFB_FMT_RGB888 */
    HI_FALSE, /* HIFB_FMT_KRGB444 */
    HI_FALSE, /* HIFB_FMT_KRGB555 */
    HI_FALSE, /* HIFB_FMT_KRGB888 */
    HI_TRUE,  /* HIFB_FMT_ARGB4444 */
    HI_TRUE,  /* HIFB_FMT_ARGB1555 */
    HI_TRUE,  /* HIFB_FMT_ARGB8888 */
    HI_FALSE, /* HIFB_FMT_ARGB8565 */
    HI_FALSE, /* HIFB_FMT_RGBA4444 */
    HI_FALSE, /* HIFB_FMT_RGBA5551 */
    HI_FALSE, /* HIFB_FMT_RGBA5658 */
    HI_FALSE, /* HIFB_FMT_RGBA8888 */

    HI_FALSE, /* BGR565 */
    HI_FALSE, /* BGR888 */
    HI_FALSE, /* ABGR4444 */
    HI_FALSE, /* ABGR1555 */
    HI_FALSE, /* ABGR8888 */
    HI_FALSE, /* ABGR8565 */
    HI_FALSE, /* BGR444 16bpp */
    HI_FALSE, /* BGR555 16bpp */
    HI_FALSE, /* BGR888 32bpp */

    HI_FALSE, /* HIFB_FMT_1BPP */
    HI_FALSE, /* HIFB_FMT_2BPP */
    HI_FALSE, /* HIFB_FMT_4BPP */
    HI_FALSE, /* HIFB_FMT_8BPP */
    HI_FALSE, /* HIFB_FMT_ACLUT44 */
    HI_FALSE, /* HIFB_FMT_ACLUT88 */
    HI_FALSE, /* HIFB_FMT_PUYVY */
    HI_FALSE, /* HIFB_FMT_PYUYV */
    HI_FALSE, /* HIFB_FMT_PYVYU */
    HI_FALSE, /* HIFB_FMT_YUV888 */
    HI_FALSE, /* HIFB_FMT_AYUV8888 */
    HI_FALSE, /* HIFB_FMT_YUVA8888 */
};

static hi_tde_color_fmt g_hifb_fmt[HIFB_FMT_BUTT] = {
    HI_TDE_COLOR_FMT_RGB565,
    HI_TDE_COLOR_FMT_RGB888,
    HI_TDE_COLOR_FMT_RGB444,
    HI_TDE_COLOR_FMT_RGB555,
    HI_TDE_COLOR_FMT_ARGB8888,
    HI_TDE_COLOR_FMT_ARGB4444,
    HI_TDE_COLOR_FMT_ARGB1555,
    HI_TDE_COLOR_FMT_ARGB8888,
    HI_TDE_COLOR_FMT_ARGB8565,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_BGR565,
    HI_TDE_COLOR_FMT_BGR888,
    HI_TDE_COLOR_FMT_ABGR4444,
    HI_TDE_COLOR_FMT_ABGR1555,
    HI_TDE_COLOR_FMT_ABGR8888,
    HI_TDE_COLOR_FMT_ABGR8565,
    HI_TDE_COLOR_FMT_BGR444,
    HI_TDE_COLOR_FMT_BGR555,
    HI_TDE_COLOR_FMT_ABGR8888,
    HI_TDE_COLOR_FMT_CLUT1,
    HI_TDE_COLOR_FMT_CLUT2,
    HI_TDE_COLOR_FMT_CLUT4,
    HI_TDE_COLOR_FMT_CLUT8,
    HI_TDE_COLOR_FMT_ACLUT44,
    HI_TDE_COLOR_FMT_ACLUT88,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
    HI_TDE_COLOR_FMT_MAX,
};

hi_bool hifb_tde_is_support_fmt(HIFB_COLOR_FMT_E fmt)
{
    if (fmt >= HIFB_FMT_BUTT) {
        return HI_FALSE;
    }
    return g_tde_col_fmt[fmt];
}

hi_tde_color_fmt hifb_drv_conv_fmt(HIFB_COLOR_FMT_E fmt)
{
    if (fmt >= HIFB_FMT_BUTT) {
        return HI_TDE_COLOR_FMT_MAX;
    }
    return g_hifb_fmt[fmt];
}

hi_s32 hifb_drv_fill(HIFB_BUFFER_S *dst_img, hi_u32 fill_data)
{
    hi_s32 ret;
    hi_tde_surface dst_sur = { 0 };
    hi_tde_rect dst_rect = { 0 };
    hi_s32 handle = -1;
    drv_tde_end_job_cmd end_job;
    hi_tde_export_func *tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
    if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_begin_job == HI_NULL) ||
        (tde_export_func->drv_tde_module_end_job == HI_NULL) ||
        (tde_export_func->drv_tde_module_cancel_job == HI_NULL) ||
        (tde_export_func->drv_tde_module_quick_fill == HI_NULL)) {
        hi_trace_fb(HI_DBG_ERR, "TDE tde_export_func is HI_NULL!\n");
        return HI_FAILURE;
    }

    if (dst_img == HI_NULL) {
        hi_trace_fb(HI_DBG_ERR, "invalid parameter is HI_NULL!\n");
        return HI_FAILURE;
    }

    /* config dst */
    dst_sur.phy_addr = dst_img->stCanvas.u64PhyAddr;
    dst_sur.width = dst_img->stCanvas.u32Width;
    dst_sur.height = dst_img->stCanvas.u32Height;
    dst_sur.stride = dst_img->stCanvas.u32Pitch;
    dst_sur.alpha_max_is_255 = HI_TRUE;
    dst_sur.is_ycbcr_clut = HI_FALSE;
    dst_sur.color_fmt = hifb_drv_conv_fmt(dst_img->stCanvas.enFmt);

    dst_rect.pos_x = 0;
    dst_rect.pos_y = 0;
    dst_rect.height = dst_sur.height;
    dst_rect.width = dst_sur.width;

    ret = tde_export_func->drv_tde_module_begin_job(&handle);
    if (ret != HI_SUCCESS) {
        osal_printk("HiFB Warning: TDE begin job failed\n");
        return ret;
    }

    ret = tde_export_func->drv_tde_module_quick_fill(handle, &dst_sur, &dst_rect, fill_data);
    if (ret != HI_SUCCESS) {
        osal_printk("HiFB Warning: TDE blit failed. ret = 0x%x handle %d \n", ret, handle);
        tde_export_func->drv_tde_module_cancel_job(handle);
        return ret;
    }

    /* 100 is timeout */
    fb_drv_tde_end_job_val(end_job, handle, HI_TRUE, 100, HI_FALSE);
    ret = tde_export_func->drv_tde_module_end_job(&end_job, HI_NULL, HI_NULL);
    if (ret != HI_SUCCESS) {
        osal_printk("HiFB Warning: FILL job submitted to TDE failed!!! Ret = 0x%x\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_u32 hifb_drv_get_dcmp_stride(HIFB_SURFACE_S *canvas)
{
    hi_u32 comp_ratio_int = 1000; /* 1000 for default compress ratio */
    hi_u32 extend_width, bytes_per_pix;
    hi_u32 exp_num_temp = 0;
    hi_u32 stride = 0;
    if (canvas == HI_NULL) {
        return stride;
    }
    stride = canvas->u32Pitch;

    if (canvas->enFmt == HIFB_FMT_ARGB8888) {
        if (canvas->u32Width <= 320) {         /* 320 max width */
            exp_num_temp = 2;                  /* 2 alg data */
            comp_ratio_int = 1000;             /* 1000 alg data */
        } else if (canvas->u32Width <= 720) {  /* 720 max width */
            exp_num_temp = 10;                 /* 10 alg data */
            comp_ratio_int = 2000;             /* 2000 alg data */
        } else if (canvas->u32Width <= 3840) { /* 3840 max width */
            exp_num_temp = 10;                 /* 10 alg data */
            comp_ratio_int = 2000;             /* 2000 alg data */
        }
        /* 4 is PerPix byte */
        bytes_per_pix = 4;
        /* 8 1000 127 128 16 alg data */
        stride = ((canvas->u32Width * bytes_per_pix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    } else if ((canvas->enFmt == HIFB_FMT_ARGB1555) || (canvas->enFmt == HIFB_FMT_ARGB4444)) {
        if (canvas->u32Width <= 720) {         /* 720 max width */
            exp_num_temp = 2;                  /* 2 alg data */
            comp_ratio_int = 1000;             /* 1000 alg data */
        } else if (canvas->u32Width <= 3840) { /* 3840 alg data */
            exp_num_temp = 0;
            comp_ratio_int = 1000; /* 1000 alg data */
        }
        bytes_per_pix = 2; /* 2 is PerPix byte */

        extend_width = ((canvas->u32Width + 31) / 32) * 32; /* 31 32 alg data */
        /* 8 1000 127 128 16 alg data */
        stride = ((extend_width * bytes_per_pix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    }

    stride = (stride + 0xf) & 0xfffffff0; /* 0xf 0xfffffff0 for 16 bytes align */

    return stride;
}

static hi_void drv_blit_init(hi_tde_surface *surface, hi_tde_rect *rect,
                             HIFB_BUFFER_S *img, hifb_blit_opt *opt)
{
    surface->phy_addr = img->stCanvas.u64PhyAddr;
    surface->width = img->stCanvas.u32Width;
    surface->height = img->stCanvas.u32Height;
    surface->stride = img->stCanvas.u32Pitch;
    surface->alpha_max_is_255 = HI_TRUE;
    surface->is_ycbcr_clut = HI_FALSE;
    surface->color_fmt = hifb_drv_conv_fmt(img->stCanvas.enFmt);
    surface->alpha0 = opt->alpha.u8Alpha0;
    surface->alpha1 = opt->alpha.u8Alpha1;

    rect->pos_x = img->UpdateRect.x;
    rect->pos_y = img->UpdateRect.y;
    rect->width = img->UpdateRect.w;
    rect->height = img->UpdateRect.h;
    return;
}

static hi_void drv_blit_compress(hi_tde_surface *dst_surface, HIFB_BUFFER_S *dst_img, hifb_blit_opt *opt)
{
    if (opt->compress == HI_TRUE) {
        dst_surface->cbcr_phy_addr = dst_img->stCanvas.u64GBPhyAddr;
        dst_surface->cbcr_stride = dst_img->stCanvas.u32Pitch;
        dst_surface->stride = hifb_drv_get_dcmp_stride(&dst_img->stCanvas);
    }
    return;
}

static hi_void drv_blit_refresh_screen(hi_tde_surface *src_surface, hi_tde_rect *src_rect,
                                       hi_tde_surface *dst_surface, hi_tde_rect *dst_rect,
                                       hi_bool is_refresh_screen)
{
    if (is_refresh_screen == HI_TRUE) {
        if ((src_surface->width == 0) || (src_surface->height == 0)) {
            return;
        }
        dst_rect->pos_x = src_rect->pos_x * dst_surface->width / src_surface->width;
        dst_rect->pos_y = src_rect->pos_y * dst_surface->height / src_surface->height;
        dst_rect->width = src_rect->width * dst_surface->width / src_surface->width;
        dst_rect->height = src_rect->height * dst_surface->height / src_surface->height;

        if (hifb_update_rotate_rect()) {
            dst_rect->pos_x = dst_surface->width - dst_rect->width - dst_rect->pos_x;
            dst_rect->pos_y = dst_surface->height - dst_rect->height - dst_rect->pos_y;
        }
    }
    return;
}

static hi_void blit_opt_get_filter_mode(hi_tde_opt *option, hifb_blit_opt *opt)
{
    switch (opt->antiflicker_level) {
        case HIFB_LAYER_ANTIFLICKER_NONE:
            option->filter_mode = HI_TDE_FILTER_MODE_COLOR;
            option->deflicker_mode = HI_TDE_DEFLICKER_LEVEL_MODE_NONE;
            break;
        case HIFB_LAYER_ANTIFLICKER_LOW:
            option->filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        case HIFB_LAYER_ANTIFLICKER_MIDDLE:
            option->filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        case HIFB_LAYER_ANTIFLICKER_HIGH:
            option->filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        case HIFB_LAYER_ANTIFLICKER_AUTO:
            option->filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        default:
            option->deflicker_mode = HI_TDE_DEFLICKER_LEVEL_MODE_NONE;
            break;
    }
    return;
}

static hi_void blit_opt_get_mirror_mode(hi_tde_opt *option, hifb_blit_opt *opt)
{
    switch (opt->mirror_mode) {
        case HIFB_MIRROR_NONE:
            option->mirror = HI_TDE_MIRROR_NONE;
            break;
        case HIFB_MIRROR_HORIZONTAL:
            option->mirror = HI_TDE_MIRROR_HORIZONTAL;
            break;
        case HIFB_MIRROR_VERTICAL:
            option->mirror = HI_TDE_MIRROR_VERTICAL;
            break;
        case HIFB_MIRROR_BOTH:
            option->mirror = HI_TDE_MIRROR_BOTH;
            break;
        default:
            option->mirror = HI_TDE_MIRROR_NONE;
            break;
    }
    return;
}

static hi_void blit_opt_get_alpha(hi_tde_opt *option, hifb_blit_opt *opt)
{
    if (opt->alpha.bAlphaEnable) {
        option->alpha_blending_cmd = HI_TDE_ALPHA_BLENDING_BLEND;
        option->global_alpha = opt->alpha.u8GlobalAlpha;
        option->out_alpha_from = HI_TDE_OUT_ALPHA_FROM_NORM;
    } else {
        option->out_alpha_from = HI_TDE_OUT_ALPHA_FROM_FOREGROUND;
    }
    return;
}

static hi_void drv_blit_option_init(hi_tde_surface *src_surface, hi_tde_surface *dst_surface,
                                    hi_tde_opt *option, hifb_blit_opt *opt)
{
    option->resize = opt->scale;
    if ((src_surface->color_fmt >= hifb_drv_conv_fmt(HIFB_FMT_1BPP)) &&
        (src_surface->color_fmt <= hifb_drv_conv_fmt(HIFB_FMT_ACLUT88))) {
        option->clut_reload = HI_TRUE;
        src_surface->clut_phy_addr = opt->cmap_addr;
        dst_surface->clut_phy_addr = opt->cmap_addr;
    }

    /* if opt is antiflicker level */
    blit_opt_get_filter_mode(option, opt);

    if (opt->ckey.key_enable) {
        if ((src_surface->color_fmt >= hifb_drv_conv_fmt(HIFB_FMT_1BPP)) &&
            (src_surface->color_fmt <= hifb_drv_conv_fmt(HIFB_FMT_ACLUT88))) {
            option->color_key_mode = HI_TDE_COLOR_KEY_MODE_FOREGROUND;
            option->color_key_value.clut_color_key.alpha.is_component_ignore = HI_TRUE;
            option->color_key_value.clut_color_key.clut.is_component_out = opt->ckey.key_mode;
            option->color_key_value.clut_color_key.clut.component_max = opt->ckey.blue_max;
            option->color_key_value.clut_color_key.clut.component_min = opt->ckey.blue_min;
        } else {
            option->color_key_mode = HI_TDE_COLOR_KEY_MODE_FOREGROUND;
            option->color_key_value.argb_color_key.alpha.is_component_ignore = HI_TRUE;
            option->color_key_value.argb_color_key.red.component_max = opt->ckey.red_max;
            option->color_key_value.argb_color_key.red.component_min = opt->ckey.red_min;
            option->color_key_value.argb_color_key.red.is_component_out = opt->ckey.key_mode;
            option->color_key_value.argb_color_key.red.component_mask = 0xff; /* 0xff mask */
            option->color_key_value.argb_color_key.green.component_max = opt->ckey.green_max;
            option->color_key_value.argb_color_key.green.component_min = opt->ckey.green_min;
            option->color_key_value.argb_color_key.green.is_component_out = opt->ckey.key_mode;
            option->color_key_value.argb_color_key.green.component_mask = 0xff; /* 0xff mask */
            option->color_key_value.argb_color_key.blue.component_max = opt->ckey.blue_max;
            option->color_key_value.argb_color_key.blue.component_min = opt->ckey.blue_min;
            option->color_key_value.argb_color_key.blue.is_component_out = opt->ckey.key_mode;
            option->color_key_value.argb_color_key.blue.component_mask = 0xff; /* 0xff mask */
        }
    }

    /* if opt is alpha enable */
    blit_opt_get_alpha(option, opt);

    /* if opt is mirror mode */
    blit_opt_get_mirror_mode(option, opt);

    return;
}

static hi_s32 drv_blit_start(hi_s32 handle, hifb_blit_opt *opt, hi_tde_export_func *tde_export_func,
                             hi_s32 *refresh_handle)
{
    hifb_tde_callback_param *param = HI_NULL;
    hi_s32 ret;
    drv_tde_end_job_cmd end_job;

    if (opt->call_back) {
        if (in_atomic()) {
            param = kmalloc(sizeof(hifb_tde_callback_param), GFP_ATOMIC);
        } else {
            param = kmalloc(sizeof(hifb_tde_callback_param), GFP_KERNEL);
        }
        if (param == HI_NULL) {
            tde_export_func->drv_tde_module_cancel_job(handle);
            return HI_FAILURE;
        }

        param->layer_id = *(hi_u32 *)opt->param;
        param->soft_cursor_update = opt->soft_cursor_update;
        param->compress = opt->compress;
        fb_drv_tde_end_job_val(end_job, handle, opt->block, HIFB_TIME_OUT_FROM_TDE, HI_FALSE);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, (tde_func_cb)g_tde_call_back, param);
    } else {
        fb_drv_tde_end_job_val(end_job, handle, opt->block, HIFB_TIME_OUT_FROM_TDE, HI_FALSE);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, HI_NULL, HI_NULL);
    }

    if (ret != HI_SUCCESS) {
        ret = tde_export_func->drv_tde_module_cancel_job(handle);
        if (ret != HI_SUCCESS) {
            hi_trace_fb(HI_DBG_ERR, "cancel job failed!ret = %x\n", ret);
        } else {
            if (param != HI_NULL) {
                kfree(param);
                param = HI_NULL;
            }
        }
        return ret;
    }

    /* if region deflicker close */
    if (opt->region_deflicker) {
        ret = tde_export_func->drv_tde_module_enable_region_deflicker(HI_FALSE);
        if (ret != HI_SUCCESS) {
            hi_trace_fb(HI_DBG_ERR, "disable region deflicker failed!\n");
        }
    }

    return ret;
}

static hi_s32 hifb_blit_check(hi_tde_export_func **tde_export_func, HIFB_BUFFER_S *src_img, HIFB_BUFFER_S *dst_img,
    hifb_blit_opt *opt)
{
    if ((src_img == HI_NULL) || (dst_img == HI_NULL) || (opt == HI_NULL)) {
        return HI_FAILURE;
    }
    *tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
    if ((*tde_export_func == HI_NULL) || ((*tde_export_func)->drv_tde_module_begin_job == HI_NULL) ||
        ((*tde_export_func)->drv_tde_module_end_job == HI_NULL) ||
        ((*tde_export_func)->drv_tde_module_cancel_job == HI_NULL) ||
        ((*tde_export_func)->drv_tde_module_enable_region_deflicker == HI_NULL) ||
        ((*tde_export_func)->drv_tde_module_blit == HI_NULL)) {
        hi_trace_fb(HI_DBG_ERR, "TDE tde_export_func is HI_NULL!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hifb_drv_blit(HIFB_BUFFER_S *src_img, HIFB_BUFFER_S *dst_img, hifb_blit_opt *opt,
                     hi_bool is_refresh_screen, hi_s32 *refresh_handle)
{
    hi_s32 ret, handle;
    hi_tde_surface src_sur = { 0 };
    hi_tde_surface dst_sur = { 0 };
    hi_tde_rect src_rect, dst_rect;
    hi_tde_opt option = { 0 };
    hi_tde_export_func *tde_export_func = HI_NULL;
    hi_tde_double_src double_src;
    if (hifb_blit_check((hi_tde_export_func **)&tde_export_func, src_img, dst_img, opt) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    /* src init */
    drv_blit_init(&src_sur, &src_rect, src_img, opt);
    /* dst_init */
    drv_blit_init(&dst_sur, &dst_rect, dst_img, opt);
    /* if compress */
    drv_blit_compress(&dst_sur, dst_img, opt);

    /* if is_refresh_screen */
    drv_blit_refresh_screen(&src_sur, &src_rect, &dst_sur, &dst_rect, is_refresh_screen);

    /* get option */
    drv_blit_option_init(&src_sur, &dst_sur, &option, opt);

    /* if region deflicker open */
    if (opt->region_deflicker) {
        ret = tde_export_func->drv_tde_module_enable_region_deflicker(HI_TRUE);
        if (ret != HI_SUCCESS) {
            hi_trace_fb(HI_DBG_ERR, "enable region deflicker failed!\n");
            return ret;
        }
    }
    ret = tde_export_func->drv_tde_module_begin_job(&handle);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    if (refresh_handle != HI_NULL) {
        *refresh_handle = handle;
    }

    option.is_compress = opt->compress;
    double_src.bg_rect = &dst_rect;
    double_src.bg_surface = &dst_sur;
    double_src.dst_rect = &dst_rect;
    double_src.dst_surface = &dst_sur;
    double_src.fg_rect = &src_rect;
    double_src.fg_surface = &src_sur;
    ret = tde_export_func->drv_tde_module_blit(handle, &double_src, &option);
    if (ret != HI_SUCCESS) {
        tde_export_func->drv_tde_module_cancel_job(handle);
        return ret;
    }

    /* blit start */
    ret = drv_blit_start(handle, opt, tde_export_func, refresh_handle);
    if (ret != HI_SUCCESS) {
        return ret;
    }
    return HI_SUCCESS;
}

static hi_void drv_rotate_init(hi_tde_surface *surface, hi_tde_rect *rect, HIFB_BUFFER_S *src_img,
                               HIFB_BUFFER_S *dst_img)
{
    if (dst_img == HI_NULL) {
        surface->phy_addr = src_img->stCanvas.u64PhyAddr;
        surface->width = src_img->stCanvas.u32Width;
        surface->height = src_img->stCanvas.u32Height;
        surface->stride = src_img->stCanvas.u32Pitch;
        surface->alpha_max_is_255 = HI_TRUE;
        surface->is_ycbcr_clut = HI_FALSE;
        surface->color_fmt = hifb_drv_conv_fmt(src_img->stCanvas.enFmt);

        rect->pos_x = src_img->UpdateRect.x;
        rect->pos_y = src_img->UpdateRect.y;
        rect->width = src_img->UpdateRect.w;
        rect->height = src_img->UpdateRect.h;
    } else {
        surface->phy_addr = dst_img->stCanvas.u64PhyAddr;
        surface->width = dst_img->stCanvas.u32Width;
        surface->height = dst_img->stCanvas.u32Height;
        surface->stride = dst_img->stCanvas.u32Pitch;
        surface->alpha_max_is_255 = HI_TRUE;
        surface->is_ycbcr_clut = HI_FALSE;
        surface->color_fmt = hifb_drv_conv_fmt(dst_img->stCanvas.enFmt);

        rect->pos_x = dst_img->UpdateRect.x;
        rect->pos_y = dst_img->UpdateRect.y;
        rect->width = src_img->UpdateRect.h;
        rect->height = src_img->UpdateRect.w;
    }

    return;
}

static hi_void drv_get_rotate_mode(hifb_rotate_opt *rotate_opt, hi_tde_rotate_angle *tde_rotate_angle)
{
    switch (rotate_opt->rotate_mode) {
        case HIFB_ROTATE_90:
            *tde_rotate_angle = HI_TDE_ROTATE_CLOCKWISE_90;
            break;
        case HIFB_ROTATE_180:
            *tde_rotate_angle = HI_TDE_ROTATE_CLOCKWISE_180;
            break;
        case HIFB_ROTATE_270:
            *tde_rotate_angle = HI_TDE_ROTATE_CLOCKWISE_270;
            break;
        default:
            break;
    }
    return;
}

static hi_s32 drv_rotate_start(hi_s32 handle, hifb_rotate_opt *rotate_opt, hi_tde_export_func *tde_export_func,
    hi_s32 *cancel_ret)
{
    hi_s32 ret;
    hifb_tde_callback_param *param = HI_NULL;
    drv_tde_end_job_cmd end_job;

    if (rotate_opt->call_back) {
        if (in_atomic()) {
            param = kmalloc(sizeof(hifb_tde_callback_param), GFP_ATOMIC);
        } else {
            param = kmalloc(sizeof(hifb_tde_callback_param), GFP_KERNEL);
        }
        if (param == HI_NULL) {
            tde_export_func->drv_tde_module_cancel_job(handle);
            return HI_FAILURE;
        }
        param->layer_id = *(hi_u32 *)rotate_opt->param;
        fb_drv_tde_end_job_val(end_job, handle, rotate_opt->block, HIFB_TIME_OUT_FROM_TDE, HI_FALSE);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, (tde_func_cb)g_tde_rotate_call_back, param);
    } else {
        fb_drv_tde_end_job_val(end_job, handle, rotate_opt->block, HIFB_TIME_OUT_FROM_TDE, HI_FALSE);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, HI_NULL, HI_NULL);
    }
    if (ret != HI_SUCCESS) {
        *cancel_ret = tde_export_func->drv_tde_module_cancel_job(handle);
        if (*cancel_ret != HI_SUCCESS) {
            hi_trace_fb(HI_DBG_ERR, "cancel job failed!ret = %x\n", *cancel_ret);
        } else {
            if (param != HI_NULL) {
                kfree(param);
                param = HI_NULL;
            }
        }
    }
    return ret;
}

/* for graphic rotation. */
hi_s32 hifb_drv_rotate(HIFB_BUFFER_S *src_img, HIFB_BUFFER_S *dst_img, hifb_rotate_opt *rotate_opt,
                       hi_bool is_refresh_screen)
{
    hi_s32 ret;
    hi_s32 handle = -1;
    hi_s32 cancel_ret = 0;
    hi_tde_surface src_surface = { 0 };
    hi_tde_surface dst_surface = { 0 };
    hi_tde_rect src_rect = { 0 };
    hi_tde_rect dst_rect = { 0 };
    hi_tde_rotate_angle tde_rotate_angle = HI_TDE_ROTATE_MAX;
    hi_tde_single_src single_src = { 0 };
    hi_tde_export_func *tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
    if ((src_img == HI_NULL) || (dst_img == HI_NULL) || (rotate_opt == HI_NULL)) {
        return HI_FAILURE;
    }

    if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_begin_job == HI_NULL) ||
        (tde_export_func->drv_tde_module_rotate == HI_NULL) ||
        (tde_export_func->drv_tde_module_cancel_job == HI_NULL) ||
        (tde_export_func->drv_tde_module_end_job == HI_NULL)) {
        hi_trace_fb(HI_DBG_ERR, "TDE tde_export_func is HI_NULL!\n");
        return HI_FAILURE;
    }

    /* src init */
    drv_rotate_init(&src_surface, &src_rect, src_img, HI_NULL);
    /* dst_init */
    drv_rotate_init(&dst_surface, &dst_rect, src_img, dst_img);

    /* get rotate mode */
    drv_get_rotate_mode(rotate_opt, &tde_rotate_angle);

    ret = tde_export_func->drv_tde_module_begin_job(&handle);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    single_src.src_surface = &src_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_surface = &dst_surface;
    single_src.dst_rect = &dst_rect;
    ret = tde_export_func->drv_tde_module_rotate(handle, &single_src, tde_rotate_angle);
    if (ret != HI_SUCCESS) {
        hi_trace_fb(HI_DBG_ERR, "tde rotate failed ret = 0x%x handle %d \n", ret, handle);
        tde_export_func->drv_tde_module_cancel_job(handle);
        return ret;
    }

    /* rotate start */
    ret = drv_rotate_start(handle, rotate_opt, tde_export_func, &cancel_ret);
    if (ret != HI_SUCCESS) {
        hi_trace_fb(HI_DBG_ERR, "drv_rotate_start failed!\n");
        return cancel_ret;
    }

    return handle;
}

hi_s32 hifb_drv_set_tde_callback(int_callback tde_callback)
{
    g_tde_call_back = tde_callback;
    return HI_SUCCESS;
}

hi_s32 hifb_drv_set_tde_rotate_callback(int_callback tde_rot_callback)
{
    g_tde_rotate_call_back = tde_rot_callback;
    return HI_SUCCESS;
}
