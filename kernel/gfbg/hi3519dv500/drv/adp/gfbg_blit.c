/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_blit.h"
#include "mod_ext.h"
#include "gfbg_comm.h"
#include "gfbg_rotate.h"

#ifdef __LITEOS__
#define in_atomic() (TD_FALSE)
#endif

static drv_tde_func_callback g_tde_call_back;

static td_s32 drv_blit_submit_job(const ot_tde_export_func *, const gfbg_blit_opt *, const td_s32 *);

/* tde support which color format */
static td_bool g_tde_col_fmt[OT_FB_FORMAT_BUTT] = {
    TD_FALSE, /* OT_FB_FORMAT_RGB565 */
    TD_FALSE, /* OT_FB_FORMAT_RGB888 */
    TD_FALSE, /* OT_FB_FORMAT_KRGB444 */
    TD_FALSE, /* OT_FB_FORMAT_KRGB555 */
    TD_FALSE, /* OT_FB_FORMAT_KRGB888 */
    TD_TRUE, /* OT_FB_FORMAT_ARGB4444 */
    TD_TRUE, /* OT_FB_FORMAT_ARGB1555 */
    TD_TRUE, /* OT_FB_FORMAT_ARGB8888 */
    TD_FALSE, /* OT_FB_FORMAT_ARGB8565 */
    TD_FALSE, /* OT_FB_FORMAT_RGBA4444 */
    TD_FALSE, /* OT_FB_FORMAT_RGBA5551 */
    TD_FALSE, /* OT_FB_FORMAT_RGBA5658 */
    TD_FALSE, /* OT_FB_FORMAT_RGBA8888 */

    TD_FALSE, /* BGR565 */
    TD_FALSE, /* BGR888 */
    TD_FALSE, /* ABGR4444 */
    TD_FALSE, /* ABGR1555 */
    TD_FALSE, /* ABGR8888 */
    TD_FALSE, /* ABGR8565 */
    TD_FALSE, /* BGR444 16bpp */
    TD_FALSE, /* BGR555 16bpp */
    TD_FALSE, /* BGR888 32bpp */

    TD_FALSE, /* OT_FB_FORMAT_1BPP */
    TD_TRUE, /* OT_FB_FORMAT_2BPP */ /* just for quickcopy(dma) */
    TD_TRUE, /* OT_FB_FORMAT_4BPP */ /* just for quickcopy(dma) */
    TD_FALSE, /* OT_FB_FORMAT_8BPP */
    TD_FALSE, /* OT_FB_FORMAT_ACLUT44 */
    TD_FALSE, /* OT_FB_FORMAT_ACLUT88 */
    TD_FALSE, /* OT_FB_FORMAT_PUYVY */
    TD_FALSE, /* OT_FB_FORMAT_PYUYV */
    TD_FALSE, /* OT_FB_FORMAT_PYVYU */
    TD_FALSE, /* OT_FB_FORMAT_YUV888 */
    TD_FALSE, /* OT_FB_FORMAT_AYUV8888 */
    TD_FALSE, /* OT_FB_FORMAT_YUVA8888 */
};

static drv_tde_color_fmt g_gfbg_fmt[OT_FB_FORMAT_BUTT] =  {
    DRV_TDE_COLOR_FMT_RGB565,
    DRV_TDE_COLOR_FMT_RGB888,
    DRV_TDE_COLOR_FMT_RGB444,
    DRV_TDE_COLOR_FMT_RGB555,
    DRV_TDE_COLOR_FMT_ARGB8888,
    DRV_TDE_COLOR_FMT_ARGB4444,
    DRV_TDE_COLOR_FMT_ARGB1555,
    DRV_TDE_COLOR_FMT_ARGB8888,
    DRV_TDE_COLOR_FMT_ARGB8565,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_BGR565,
    DRV_TDE_COLOR_FMT_BGR888,
    DRV_TDE_COLOR_FMT_ABGR4444,
    DRV_TDE_COLOR_FMT_ABGR1555,
    DRV_TDE_COLOR_FMT_ABGR8888,
    DRV_TDE_COLOR_FMT_ABGR8565,
    DRV_TDE_COLOR_FMT_BGR444,
    DRV_TDE_COLOR_FMT_BGR555,
    DRV_TDE_COLOR_FMT_ABGR8888,
    DRV_TDE_COLOR_FMT_CLUT1,
    DRV_TDE_COLOR_FMT_CLUT2,
    DRV_TDE_COLOR_FMT_CLUT4,
    DRV_TDE_COLOR_FMT_CLUT8,
    DRV_TDE_COLOR_FMT_ACLUT44,
    DRV_TDE_COLOR_FMT_ACLUT88,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
    DRV_TDE_COLOR_FMT_MAX,
};

td_bool gfbg_tde_is_support_fmt(ot_fb_color_format format)
{
    if (format >= OT_FB_FORMAT_BUTT) {
        return TD_FALSE;
    }
    return g_tde_col_fmt[format];
}

drv_tde_color_fmt gfbg_drv_conv_fmt(ot_fb_color_format format)
{
    if (format >= OT_FB_FORMAT_BUTT) {
        return DRV_TDE_COLOR_FMT_MAX;
    }
    return g_gfbg_fmt[format];
}

td_s32 gfbg_drv_fill(const ot_fb_buf *dst_img, td_u32 fill_data)
{
    td_s32 ret;
    drv_tde_surface dst_surface = {0};
    drv_tde_rect dst_rect = {0};
    td_s32 handle = -1;
    drv_tde_end_job_cmd end_job;
    ot_tde_export_func *tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_begin_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_end_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_cancel_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_quick_fill == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    }

    /* config dst */
    dst_surface.phys_addr = dst_img->canvas.phys_addr;
    dst_surface.width  = dst_img->canvas.width;
    dst_surface.height = dst_img->canvas.height;
    dst_surface.stride = dst_img->canvas.pitch;
    dst_surface.alpha_max_is_255 = TD_TRUE;
    dst_surface.is_ycbcr_clut = TD_FALSE;
    dst_surface.color_format = gfbg_drv_conv_fmt(dst_img->canvas.format);

    dst_rect.pos_x     = 0;
    dst_rect.pos_y     = 0;
    dst_rect.height   = dst_surface.height;
    dst_rect.width    = dst_surface.width;

    ret = tde_export_func->drv_tde_module_begin_job(&handle);
    if (ret != TD_SUCCESS) {
        osal_printk("GFBG Warning: TDE begin job failed\n");
        return ret;
    }

    ret = tde_export_func->drv_tde_module_quick_fill(handle, &dst_surface, &dst_rect, fill_data);
    if (ret != TD_SUCCESS) {
        osal_printk("GFBG Warning: TDE blit failed. ret = 0x%x handle %d \n", ret, handle);
        tde_export_func->drv_tde_module_cancel_job(handle);
        return ret;
    }

    /* 1000 is timeout */
    fb_drv_tde_end_job_val(end_job, handle, TD_TRUE, 1000, TD_FALSE);
    ret = tde_export_func->drv_tde_module_end_job(&end_job, TD_NULL, TD_NULL);
    if (ret != TD_SUCCESS) {
        osal_printk("GFBG Warning: FILL job submitted to TDE failed!!! Ret = 0x%x\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

#ifndef CONFIG_COMPRESS_ECONOMIZE_MEMERY
static td_u32 gfbg_drv_get_dcmp_stride(const ot_fb_surface *canvas)
{
    td_u32 comp_ratio_int = 1000; /* 1000 for default compress ratio */
    td_u32 extend_width, bytes_per_pix;
    td_u32 exp_num_temp = 0;
    td_u32 stride = 0;
    if (canvas == TD_NULL) {
        return stride;
    }
    stride = canvas->pitch;

    if (canvas->format == OT_FB_FORMAT_ARGB8888) {
        if (canvas->width <= 320) { /* 320 max width */
            exp_num_temp = 2; /* 2 alg data */
            comp_ratio_int = 1000; /* 1000 alg data */
        } else if (canvas->width <= 720) { /* 720 max width */
            exp_num_temp = 10; /* 10 alg data */
            comp_ratio_int = 2000; /* 2000 alg data */
        } else if (canvas->width <= 3840) { /* 3840 max width */
            exp_num_temp = 10; /* 10 alg data */
            comp_ratio_int = 2000; /* 2000 alg data */
        }
        /* 4 is PerPix byte */
        bytes_per_pix = 4;
        /* 8 1000 127 128 16 alg data */
        stride = ((canvas->width * bytes_per_pix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    } else if (OT_FB_FORMAT_ARGB1555 == canvas->format || OT_FB_FORMAT_ARGB4444 == canvas->format) {
        if (canvas->width <= 720) { /* 720 max width */
            exp_num_temp = 2; /* 2 alg data */
            comp_ratio_int = 1000; /* 1000 alg data */
        } else if (canvas->width <= 3840) { /* 3840 alg data */
            exp_num_temp = 0;
            comp_ratio_int = 1000; /* 1000 alg data */
        }
        bytes_per_pix = 2; /* 2 is PerPix byte */

        extend_width = ((canvas->width + 31) / 32) * 32; /* 31 32 alg data */
        /* 8 1000 127 128 16 alg data */
        stride = ((extend_width * bytes_per_pix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    }

    stride = (stride + 0xf) & 0xfffffff0; /* 0xf 0xfffffff0 for 16 bytes align */

    return stride;
}
#endif

static td_void drv_blit_init(drv_tde_surface *surface, drv_tde_rect *rect, const ot_fb_buf *img,
    const gfbg_blit_opt *opt)
{
    surface->phys_addr   = img->canvas.phys_addr;
    surface->width  = img->canvas.width;
    surface->height = img->canvas.height;
    surface->stride = img->canvas.pitch;
    surface->alpha_max_is_255 = TD_TRUE;
    surface->is_ycbcr_clut = TD_FALSE;
    surface->color_format = gfbg_drv_conv_fmt(img->canvas.format);
    surface->alpha0 = opt->alpha.alpha0;
    surface->alpha1 = opt->alpha.alpha1;

    rect->pos_x = img->update_rect.x;
    rect->pos_y = img->update_rect.y;
    rect->width = img->update_rect.width;
    rect->height = img->update_rect.height;
    return;
}

static td_void drv_blit_compress(drv_tde_surface *dst_surface, const ot_fb_buf *dst_img, const gfbg_blit_opt *opt)
{
#ifndef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    if (opt->compress == TD_TRUE) {
        dst_surface->cbcr_phys_addr = dst_img->canvas.phys_addr;
        dst_surface->cbcr_stride = dst_img->canvas.pitch;
        dst_surface->stride = gfbg_drv_get_dcmp_stride(&dst_img->canvas);
    }
#else
    td_u32 cmp_stride = 0;
    td_u32 uncmp_stride = 0;
    gfbg_stride_attr attr = {0};

    dst_surface->cbcr_phys_addr = dst_img->canvas.phys_addr;
    dst_surface->cbcr_stride = dst_img->canvas.pitch;

    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = dst_img->canvas.width;
    attr.format = dst_img->canvas.format;
    gfbg_recalculate_stride(&cmp_stride, &uncmp_stride, &attr);
    if (opt->compress != TD_TRUE && uncmp_stride != 0) {
        dst_surface->stride = uncmp_stride;
    } else if (cmp_stride != 0) {
        dst_surface->stride = cmp_stride;
    }
#endif
    return;
}

static td_void drv_blit_refresh_screen(const drv_tde_surface *src_surface, const drv_tde_rect *src_rect,
                                       const drv_tde_surface *dst_surface, drv_tde_rect *dst_rect,
                                       td_bool is_refresh_screen)
{
    if (is_refresh_screen == TD_TRUE) {
        if ((src_surface->width == 0) || (src_surface->height == 0)) {
            return;
        }
        dst_rect->pos_x   =  src_rect->pos_x * dst_surface->width / src_surface->width;
        dst_rect->pos_y   =  src_rect->pos_y * dst_surface->height / src_surface->height;
        dst_rect->width  =  src_rect->width * dst_surface->width / src_surface->width;
        dst_rect->height =  src_rect->height * dst_surface->height / src_surface->height;
    }
    return;
}

static td_void blit_opt_get_filter_mode(drv_tde_opt *option, const gfbg_blit_opt *opt)
{
    switch (opt->antiflicker_level) {
        case OT_FB_LAYER_ANTIFLICKER_NONE:
            option->filter_mode = DRV_TDE_FILTER_MODE_COLOR;
            break;
        case OT_FB_LAYER_ANTIFLICKER_LOW:
            option->filter_mode = DRV_TDE_FILTER_MODE_COLOR;
            break;
        case OT_FB_LAYER_ANTIFLICKER_MID:
            option->filter_mode = DRV_TDE_FILTER_MODE_COLOR;
            break;
        case OT_FB_LAYER_ANTIFLICKER_HIGH:
            option->filter_mode = DRV_TDE_FILTER_MODE_COLOR;
            break;
        case OT_FB_LAYER_ANTIFLICKER_AUTO:
            option->filter_mode = DRV_TDE_FILTER_MODE_COLOR;
            break;
        default:
            break;
    }
    return;
}

static td_void blit_opt_get_mirror_mode(drv_tde_opt *option, const gfbg_blit_opt *opt)
{
    switch (opt->mirror_mode) {
        case OT_FB_MIRROR_NONE:
            option->mirror = DRV_TDE_MIRROR_NONE;
            break;
        case OT_FB_MIRROR_HOR:
            option->mirror = DRV_TDE_MIRROR_HORIZONTAL;
            break;
        case OT_FB_MIRROR_VER:
            option->mirror = DRV_TDE_MIRROR_VERTICAL;
            break;
        case OT_FB_MIRROR_BOTH:
            option->mirror = DRV_TDE_MIRROR_BOTH;
            break;
        default:
            option->mirror = DRV_TDE_MIRROR_NONE;
            break;
    }
    return;
}

static td_void blit_opt_get_alpha(drv_tde_opt *option, const gfbg_blit_opt *opt)
{
    if (opt->alpha.pixel_alpha) {
        option->alpha_blending_cmd = DRV_TDE_ALPHA_BLENDING_BLEND;
        option->global_alpha = opt->alpha.global_alpha;
        option->out_alpha_from = DRV_TDE_OUT_ALPHA_FROM_NORM;
    } else {
        option->out_alpha_from = DRV_TDE_OUT_ALPHA_FROM_FOREGROUND;
    }
    return;
}

static td_void drv_blit_option_init(drv_tde_surface *src_surface, drv_tde_surface *dst_surface, drv_tde_opt *option,
                                    const gfbg_blit_opt *opt)
{
    option->resize = opt->scale;
    if ((src_surface->color_format >= gfbg_drv_conv_fmt(OT_FB_FORMAT_1BPP)) &&
        (src_surface->color_format <= gfbg_drv_conv_fmt(OT_FB_FORMAT_ACLUT88))) {
        option->clut_reload = TD_TRUE;
        src_surface->clut_phys_addr = opt->cmap_addr;
        dst_surface->clut_phys_addr = opt->cmap_addr;
    }

    /* if opt is antiflicker level */
    blit_opt_get_filter_mode(option, opt);

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    if (opt->ckey.key_enable) {
        if ((src_surface->color_format >= gfbg_drv_conv_fmt(OT_FB_FORMAT_1BPP)) &&
            (src_surface->color_format <= gfbg_drv_conv_fmt(OT_FB_FORMAT_ACLUT88))) {
            option->colorkey_mode = DRV_TDE_COLOR_KEY_MODE_FOREGROUND;
            option->colorkey_value.clut_colorkey.alpha.is_component_ignore = TD_TRUE;
            option->colorkey_value.clut_colorkey.clut.is_component_out = opt->ckey.key_mode;
            option->colorkey_value.clut_colorkey.clut.component_max = opt->ckey.blue_max;
            option->colorkey_value.clut_colorkey.clut.component_min = opt->ckey.blue_min;
        } else {
            option->colorkey_mode = DRV_TDE_COLOR_KEY_MODE_FOREGROUND;
            option->colorkey_value.argb_colorkey.alpha.is_component_ignore = TD_TRUE;
            option->colorkey_value.argb_colorkey.red.component_max = opt->ckey.red_max;
            option->colorkey_value.argb_colorkey.red.component_min = opt->ckey.red_min;
            option->colorkey_value.argb_colorkey.red.is_component_out = opt->ckey.key_mode;
            option->colorkey_value.argb_colorkey.red.component_mask = 0xff;
            option->colorkey_value.argb_colorkey.green.component_max = opt->ckey.green_max;
            option->colorkey_value.argb_colorkey.green.component_min = opt->ckey.green_min;
            option->colorkey_value.argb_colorkey.green.is_component_out = opt->ckey.key_mode;
            option->colorkey_value.argb_colorkey.green.component_mask = 0xff;
            option->colorkey_value.argb_colorkey.blue.component_max = opt->ckey.blue_max;
            option->colorkey_value.argb_colorkey.blue.component_min = opt->ckey.blue_min;
            option->colorkey_value.argb_colorkey.blue.is_component_out = opt->ckey.key_mode;
            option->colorkey_value.argb_colorkey.blue.component_mask = 0xff;
        }
    }
#endif
    /* if opt is alpha enable */
    blit_opt_get_alpha(option, opt);

    /* if opt is mirror mode */
    blit_opt_get_mirror_mode(option, opt);

    return;
}


static td_s32 gfbg_check_is_intelligence_rect(const drv_tde_surface *src_surface, const drv_tde_surface *dst_surface,
                                              const drv_tde_opt *option)
{
    if (src_surface->color_format != dst_surface->color_format) {
        gfbg_error("clut only support src and dst the same color format!\n");
        return TD_FAILURE;
    }
    /* not support scale */
    if (option->resize == TD_TRUE) {
        gfbg_error("clut not support resize!\n");
        return TD_FAILURE;
    }
    /* not support compress */
    if ((option->is_compress == TD_TRUE) || (option->is_decompress == TD_TRUE)) {
        gfbg_error("clut not support compress!\n");
        return TD_FAILURE;
    }
    /* not support alpha blending */
    if (option->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE) {
        gfbg_error("clut not support alpha blending!\n");
        return TD_FAILURE;
    }
    /* not support colorkey */
    if (option->colorkey_mode != DRV_TDE_COLOR_KEY_MODE_NONE) {
        gfbg_error("clut not support colorkey!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_blit_start(td_s32 *handle, const drv_tde_single_src *single_src, drv_tde_opt *option,
                             const gfbg_blit_opt *opt, const ot_tde_export_func *tde_export_func)
{
    td_s32 ret;
    drv_tde_double_src double_src;

    ret = tde_export_func->drv_tde_module_begin_job(handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    option->is_compress = opt->compress;
   /*
    * for 1 and 2 buff refresh intelligence rect
    * when both src surface's dst surface's color format are clut2 or clut4
    * only quick copy without any operation
    */
    if ((single_src->dst_surface->color_format == DRV_TDE_COLOR_FMT_CLUT2) ||
        (single_src->dst_surface->color_format == DRV_TDE_COLOR_FMT_CLUT4)) {
        ret = gfbg_check_is_intelligence_rect(single_src->src_surface, single_src->dst_surface, option);
        if (ret != TD_SUCCESS) {
            tde_export_func->drv_tde_module_cancel_job(*handle);
            return ret;
        }
        ret = tde_export_func->drv_tde_module_quick_copy(*handle, single_src);
    } else {
        double_src.bg_surface = single_src->dst_surface;
        double_src.bg_rect = single_src->dst_rect;
        double_src.fg_surface = single_src->src_surface;
        double_src.fg_rect = single_src->src_rect;
        double_src.dst_surface = single_src->dst_surface;
        double_src.dst_rect = single_src->dst_rect;
        ret = tde_export_func->drv_tde_module_blit(*handle, &double_src, option);
    }

    if (ret != TD_SUCCESS) {
        tde_export_func->drv_tde_module_cancel_job(*handle);
        return ret;
    }

    /* submit tde the job */
    ret = drv_blit_submit_job(tde_export_func, opt, handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return ret;
}

static td_s32 drv_blit_submit_job(const ot_tde_export_func *tde_export_func, const gfbg_blit_opt *opt,
                                  const td_s32 *handle)
{
    gfbg_tde_callback_param *param = TD_NULL;
    const td_u32 time_out = 1000;
    td_s32 ret;
    drv_tde_end_job_cmd end_job;

    if (opt->call_back) {
        if (in_atomic()) {
            param = osal_kmalloc(sizeof(gfbg_tde_callback_param), GFP_ATOMIC);
        } else {
            param = osal_kmalloc(sizeof(gfbg_tde_callback_param), GFP_KERNEL);
        }
        if (param == TD_NULL) {
            tde_export_func->drv_tde_module_cancel_job(*handle);
            return TD_FAILURE;
        }

        param->layer_id = *(td_u32 *)opt->param;
        param->soft_cursor_update = opt->soft_cursor_update;
        param->compress = opt->compress;
        fb_drv_tde_end_job_val(end_job, *handle, opt->block, time_out, opt->is_sync);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, g_tde_call_back, param);
    } else {
        fb_drv_tde_end_job_val(end_job, *handle, opt->block, time_out, opt->is_sync);
        ret = tde_export_func->drv_tde_module_end_job(&end_job, TD_NULL, TD_NULL);
    }

    if (ret != TD_SUCCESS) {
        ret = tde_export_func->drv_tde_module_cancel_job(*handle);
        if (ret != TD_SUCCESS) {
           /*
            * job has been submitted to the logic for processing
            * memory will be released in the callback function
            */
            gfbg_error("cancel job failed!ret = %x\n", ret);
        } else {
            /*
            * job is not submitted to the logic for processing
            * callback function is not invoked
            * memory needs to be released here
            */
            if (param != TD_NULL) {
                osal_kfree(param);
                param = TD_NULL;
            }
        }
        return TD_FAILURE;
    }
   /*
    * memory has been released in the callback function
    * or after cancel_job executed successfully
    */
    return TD_SUCCESS;
}

static td_s32 gfbg_blit_check(ot_tde_export_func **tde_export_func, const ot_fb_buf *src_img, const ot_fb_buf *dst_img,
                              const gfbg_blit_opt *opt)
{
    if ((src_img == TD_NULL) || (dst_img == TD_NULL) || (opt == TD_NULL)) {
        return TD_FAILURE;
    }
    *tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((*tde_export_func == TD_NULL) || ((*tde_export_func)->drv_tde_module_begin_job == TD_NULL) ||
        ((*tde_export_func)->drv_tde_module_end_job == TD_NULL) ||
        ((*tde_export_func)->drv_tde_module_cancel_job == TD_NULL) ||
        ((*tde_export_func)->drv_tde_module_blit == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 gfbg_drv_blit(const ot_fb_buf *src_img, const ot_fb_buf *dst_img, const gfbg_blit_opt *opt,
                     td_bool is_refresh_screen)
{
    td_s32 ret, handle;
    drv_tde_surface src_surface = {0};
    drv_tde_surface dst_surface = {0};
    drv_tde_rect src_rect, dst_rect;
    drv_tde_opt option = {0};
    ot_tde_export_func *tde_export_func = TD_NULL;
    drv_tde_single_src single_src = {0};
    if (gfbg_blit_check((ot_tde_export_func **)&tde_export_func, src_img, dst_img, opt) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* src init */
    drv_blit_init(&src_surface, &src_rect, src_img, opt);
    /* dst_init */
    drv_blit_init(&dst_surface, &dst_rect, dst_img, opt);
    /* if compress */
    drv_blit_compress(&dst_surface, dst_img, opt);
    /* if is_refresh_screen */
    drv_blit_refresh_screen(&src_surface, &src_rect, &dst_surface, &dst_rect, is_refresh_screen);
    /* get option */
    drv_blit_option_init(&src_surface, &dst_surface, &option, opt);

    /* blit start */
    single_src.src_surface = &src_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_surface = &dst_surface;
    single_src.dst_rect = &dst_rect;
    ret = drv_blit_start(&handle, &single_src, &option, opt, tde_export_func);
    if (ret != TD_SUCCESS) {
        gfbg_error("drv_blit_start failed!\n");
        return ret;
    }

    return handle;
}

td_s32 gfbg_drv_set_tde_callback(drv_tde_func_callback tde_callback)
{
    g_tde_call_back = tde_callback;
    return TD_SUCCESS;
}

