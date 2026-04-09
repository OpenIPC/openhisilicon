/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_vou_graphics.h"
#include "hi_osal.h"
#include "proc_ext.h"
#include "hi_errno.h"
#include "hi_debug.h"
#include "hifb_graphic_drv.h"

typedef struct {
    hi_bool vo_enable;        /* Device enable flag */

    VO_INTF_TYPE_E intf_type; /* Device interface type */

    hi_u32 max_width;         /* Device resolution maximum width */
    hi_u32 max_height;        /* Device resolution maximum height */
} vo_dev_info;

#define VO_IRQ_NR 90
vo_dev_info g_ast_vo_dev[VO_MAX_DEV_NUM];

vo_gfxlayer_context g_gfx_layer_ctx[VO_MAX_GRAPHICS_LAYER_NUM];
hi_u32 g_fb_dectect_zone = 0; /* Record the DDR detection area used */

vo_gfxlayer_context *get_gfx_layer_ctx(hi_void)
{
    return g_gfx_layer_ctx;
}

hi_void vou_get_gfx_dcmp_pixel(fb_vo_disp_pixel_format pixel_fmt, hal_disp_pixel_format *pen_pixel_fmt)
{
    switch (pixel_fmt) {
        case FB_VO_INPUTFMT_ARGB_4444:
            *pen_pixel_fmt = HAL_INPUTFMT_ARGB_4444;
            break;
        case FB_VO_INPUTFMT_ARGB_1555:
            *pen_pixel_fmt = HAL_INPUTFMT_ARGB_1555;
            break;
        case FB_VO_INPUTFMT_ARGB_8888:
            *pen_pixel_fmt = HAL_INPUTFMT_ARGB_8888;
            break;
        default:
            graphics_drv_trace(HI_DBG_ERR, "pixel format(%d) is invalid!\n", pixel_fmt);
            break;
    }
}

hi_void vou_graphics_init_mddr_detect(hi_void)
{
    return;
}

hi_bool vou_graphics_set_gfx_key_mode(GRAPHIC_LAYER layer, hi_u32 key_out)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_gfx_key_mode(disp_layer, key_out);
}

hi_bool vou_graphics_set_gfx_ext(GRAPHIC_LAYER layer, fb_vo_gfx_bitextend mode)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_gfx_ext(disp_layer, mode);
}

hi_bool vou_graphics_set_gfx_palpha(GRAPHIC_LAYER layer, hi_u32 alpha_en, hi_u32 arange, hi_u8 alpha0,
                                    hi_u8 alpha1)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_gfx_palpha(disp_layer, alpha_en, arange, alpha0, alpha1);
}

hi_bool vou_graphics_set_layer_galpha(GRAPHIC_LAYER layer, hi_u8 alpha0)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_layer_set_layer_galpha(disp_layer, alpha0);
}

hi_bool vou_graphics_set_csc_en(GRAPHIC_LAYER layer, hi_bool csc_en)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_layer_set_csc_en(disp_layer, csc_en);
}

hi_bool vou_graphics_set_gfx_addr(GRAPHIC_LAYER layer, hi_u64 l_addr)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_layer_addr(disp_layer, l_addr);
}

hi_bool vou_graphics_set_gfx_stride(GRAPHIC_LAYER layer, hi_u16 pitch)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_gfx_stride(disp_layer, pitch);
}

hi_bool vou_graphics_get_gfx_pre_mult(GRAPHIC_LAYER layer, hi_u32 *enable)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_get_gfx_pre_mult(disp_layer, enable);
}

hi_bool vou_graphics_set_gfx_pre_mult(GRAPHIC_LAYER layer, hi_u32 enable)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_gfx_pre_mult(disp_layer, enable);
}

hi_bool vou_graphics_set_layer_data_fmt(GRAPHIC_LAYER layer, fb_vo_disp_pixel_format data_fmt)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;
    hal_disp_pixel_format pix_fmt = HAL_INPUTFMT_ARGB_1555;

    vou_get_gfx_dcmp_pixel(data_fmt, &pix_fmt);

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_layer_data_fmt(disp_layer, pix_fmt);
}

hi_bool vou_graphics_set_layer_in_rect(GRAPHIC_LAYER layer, HIFB_RECT *rect)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_layer_in_rect(disp_layer, rect);
}

hi_bool vou_graphics_set_layer_src_image_reso(GRAPHIC_LAYER layer, HIFB_RECT *rect)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_src_image_resolution(disp_layer, rect);
}

hi_bool vou_graphics_set_layer_out_rect(GRAPHIC_LAYER layer, HIFB_RECT *rect)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_layer_out_rect(disp_layer, rect);
}

hi_bool vou_graphics_set_color_key_value(GRAPHIC_LAYER layer, fb_vo_gfx_key_max vo_key_max,
                                         fb_vo_gfx_key_min vo_key_min)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    hal_gfx_key_max key_max;
    hal_gfx_key_min key_min;
    key_max.key_max_r = vo_key_max.key_max_r;
    key_max.key_max_g = vo_key_max.key_max_g;
    key_max.key_max_b = vo_key_max.key_max_b;
    key_min.key_min_r = vo_key_min.key_min_r;
    key_min.key_min_g = vo_key_min.key_min_g;
    key_min.key_min_b = vo_key_min.key_min_b;
    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_color_key_value(disp_layer, key_max, key_min);
}

hi_bool vou_graphics_set_color_key_mask(GRAPHIC_LAYER layer, fb_vo_gfx_mask vo_msk)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    hal_gfx_mask msk;
    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    msk.mask_r = vo_msk.mask_r;
    msk.mask_g = vo_msk.mask_g;
    msk.mask_b = vo_msk.mask_b;
    return graphic_drv_set_color_key_mask(disp_layer, msk);
}

hi_bool vou_graphics_set_gfx_key_en(GRAPHIC_LAYER layer, hi_u32 key_enable)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_gfx_key_en(disp_layer, key_enable);
}

hi_bool vou_graphics_set_reg_up(GRAPHIC_LAYER layer)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_set_reg_up(disp_layer);
}

hi_s32 vou_graphics_init(hi_void)
{
    hi_s32 ret;
    hifb_vo_dev vo_dev = VO_DEV_DHD0;
    hal_disp_syncinfo sync_info = { 0 };
    hi_bool logic_ret = HI_FALSE;
    hi_bool vo_enable = HI_FALSE;
    VO_INTF_TYPE_E intf_type = HAL_DISP_INTF_BT1120;
    hi_s32 i;

    ret = fb_graphic_drv_init();

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        vo_dev = i;
        logic_ret = graphic_drv_get_dev_enable(vo_dev, &vo_enable);
        if (logic_ret == HI_FALSE) {
            continue;
        } else {
            g_ast_vo_dev[vo_dev].vo_enable = vo_enable;
        }

        logic_ret = graphic_drv_get_intf_sync(vo_dev, &sync_info);
        if (logic_ret == HI_FALSE) {
            continue;
        } else {
            g_ast_vo_dev[vo_dev].max_width = sync_info.hact;
            /* 2 alg data */
            g_ast_vo_dev[vo_dev].max_height = (sync_info.iop) ? sync_info.vact : sync_info.vact * 2;
        }

        logic_ret = graphic_drv_get_intf_mux_sel(vo_dev, &intf_type);
        if (logic_ret == HI_FALSE) {
            continue;
        } else {
            g_ast_vo_dev[vo_dev].intf_type = intf_type;
        }
    }

    return ret;
}

hi_s32 vou_graphics_deinit(hi_void)
{
    return fb_graphic_drv_exit();
}

hi_bool vou_graphics_get_layer_galpha(GRAPHIC_LAYER layer, hi_u8 *alpha0)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_get_layer_galpha(disp_layer, alpha0);
}

hi_bool vou_graphics_get_layer_data_fmt(GRAPHIC_LAYER layer, hi_u32 *fmt)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_get_layer_data_fmt(disp_layer, fmt);
}

hi_bool vou_graphics_get_gfx_addr(GRAPHIC_LAYER layer, hi_u64 *gfx_addr)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_get_gfx_addr(disp_layer, gfx_addr);
}

hi_bool vou_graphics_get_gfx_stride(GRAPHIC_LAYER layer, hi_u32 *gfx_stride)
{
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);
    return graphic_drv_get_gfx_stride(disp_layer, gfx_stride);
}

hi_s32 vou_graphics_get_dev_mode(hal_disp_layer layer, fb_vou_scan_mode *scan_mode, hi_bool *feild_update)
{
    hifb_vo_dev vo_dev;
    hi_u32 layer_index;
    unsigned long lock_flag;
    vo_gfxlayer_context *vo_gfx_layer_ctx = HI_NULL;
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);
    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }

    vo_gfx_layer_ctx = &g_gfx_layer_ctx[layer_index];

    gfx_spin_lock_irqsave(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
    if (!vo_gfx_layer_ctx->binded) {
        gfx_spin_unlock_irqrestore(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
        hifb_graphics_trace(HI_DBG_ERR, "graphics layer %u has not been binded!\n", layer_index);
        return HI_ERR_VO_GFX_NOT_BIND;
    }
    vo_dev = vo_gfx_layer_ctx->binded_dev;

    if (!g_ast_vo_dev[vo_dev].vo_enable) {
        gfx_spin_unlock_irqrestore(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
        hifb_graphics_trace(HI_DBG_ERR, "vodev %d for graphics layer %u has been disable!\n", vo_dev, layer_index);
        return HI_ERR_VO_DEV_NOT_ENABLE;
    }
    if (fb_graphic_drv_get_scan_mode(vo_dev, (hi_bool *)scan_mode) == HI_FALSE) {
        gfx_spin_unlock_irqrestore(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
        hifb_graphics_trace(HI_DBG_ERR, "get vodev:%d scan mode failed!\n", vo_dev);
        return HI_FAILURE;
    }

    if (graphic_drv_get_vt_thd_mode(vo_dev, feild_update) == HI_FALSE) {
        gfx_spin_unlock_irqrestore(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
        hifb_graphics_trace(HI_DBG_ERR, "get vodev:%d scan mode failed!\n", vo_dev);
        return HI_FAILURE;
    }
    gfx_spin_unlock_irqrestore(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
    return HI_SUCCESS;
}

hi_s32 fb_vo_graphics_init(hi_void)
{
    return fb_graphic_drv_init();
}

hi_s32 vo_graphics_deinit(hi_void)
{
    return fb_graphic_drv_exit();
}

hi_s32 vou_graphics_resource_init(hi_void)
{
    hi_s32 ret;
    ret = graphic_drv_resource_init();
    if (ret != HI_SUCCESS) {
        hifb_graphics_trace(HI_DBG_ERR, "graphic drv resource init failed.\n");
        return ret;
    }

    return ret;
}

hi_s32 vou_graphics_resource_deinit(hi_void)
{
    hi_s32 ret;
    ret = graphic_drv_resource_exit();
    return ret;
}

hi_s32 vou_graphics_enable_layer(GRAPHIC_LAYER layer, hi_bool enable)
{
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);
    if ((gfx_layer < LAYER_GFX_START) || (gfx_layer > LAYER_GFX_END)) {
        return HI_ERR_VO_GFX_INVALID_ID;
    }
    return graphic_drv_enable_layer(gfx_layer, enable);
}

hi_s32 vou_graphics_set_callback(GRAPHIC_LAYER layer, fb_vo_int_type type, vo_fb_intcallback call_back,
                                 hi_void *arg)
{
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    unsigned long lock_flag;
    hi_u32 layer_index;
    hi_s32 ret = HI_SUCCESS;
    vo_gfxlayer_context *vo_gfx_layer_ctx = HI_NULL;

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);
    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }

    vo_gfx_layer_ctx = &g_gfx_layer_ctx[layer_index];

    gfx_spin_lock_irqsave(&vo_gfx_layer_ctx->spin_lock, &lock_flag);
    switch (type) {
        case HIFB_INTTYPE_VO:
            vo_gfx_layer_ctx->vo_callback = (fb_intcallback)call_back;
            vo_gfx_layer_ctx->vo_callback_arg = arg;
            break;
        case HIFB_INTTYPE_WBC:
            vo_gfx_layer_ctx->wbc_callback = (fb_intcallback)call_back;
            vo_gfx_layer_ctx->wbc_callback_arg = arg;
            break;
        default:
            ret = HI_FAILURE;
            break;
    }
    gfx_spin_unlock_irqrestore(&vo_gfx_layer_ctx->spin_lock, &lock_flag);

    return ret;
}

hi_bool vou_graphics_enable_dcmp(GRAPHIC_LAYER layer, hi_bool enable)
{
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);
    return graphic_drv_enable_dcmp(gfx_layer, enable);
}

hi_bool vou_graphics_get_dcmp_enable_state(GRAPHIC_LAYER layer, hi_bool *enable)
{
    hi_s32 ret;
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    ret = fb_graphic_drv_get_layer_id(layer, &gfx_layer);
    if (ret != HI_SUCCESS) {
        hifb_graphics_trace(HI_DBG_ERR, "graphics layer %d is illegal!\n", layer);
        return HI_FAILURE;
    }
    return graphic_drv_get_dcmp_enable_state(gfx_layer, enable);
}

hi_bool vou_graphics_enable_ghdr(GRAPHIC_LAYER layer, hi_bool enable)
{
    return HI_FALSE;
}

hi_bool vou_graphics_enable_zme(GRAPHIC_LAYER layer, RECT_S *in_rect, RECT_S *out_rect, hi_bool enable)
{
    gf_g0_zme_cfg zme_cfg;
    hal_disp_layer disp_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &disp_layer);

    zme_cfg.in_width = in_rect->u32Width;
    zme_cfg.in_height = in_rect->u32Height;
    zme_cfg.out_width = out_rect->u32Width;
    zme_cfg.out_height = out_rect->u32Height;

    return graphic_drv_enable_zme(disp_layer, &zme_cfg, enable);
}

hi_s32 vou_graphics_get_intf_size(GRAPHIC_LAYER layer, hi_u32 *width, hi_u32 *height)
{
    hifb_vo_dev vo_dev;
    hi_u32 layer_index;
    vo_gfxlayer_context *vo_gfx_layer_ctx = HI_NULL;
    vo_dev_info *dev_info = HI_NULL;
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;
    hi_bool ret = HI_FALSE;
    hal_disp_syncinfo sync_info = { 0 };

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);
    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }

    vo_gfx_layer_ctx = &g_gfx_layer_ctx[layer_index];

    if (!vo_gfx_layer_ctx->binded) {
        hifb_graphics_trace(HI_DBG_ERR, "Graphics layer %u# has not been binded!\n", layer_index);
        return HI_FAILURE;
    }

    vo_dev = vo_gfx_layer_ctx->binded_dev;
    dev_info = &g_ast_vo_dev[vo_dev];

    if (!dev_info->vo_enable) {
        hifb_graphics_trace(HI_DBG_ERR, "The vo device (%d) for graphics layer %u has been disable!\n", vo_dev,
                            layer_index);
        return HI_FAILURE;
    }

    /*
     * Be sure to read from the register, otherwise the width and height of
     * the hifb context will not be updated when the vo timing changes.
     */
    ret = graphic_drv_get_intf_sync(vo_dev, &sync_info);
    if (ret == HI_FALSE) {
        /* keep the old value. */
    } else {
        g_ast_vo_dev[vo_dev].max_width = sync_info.hact;
        /* 2 alg data */
        g_ast_vo_dev[vo_dev].max_height = (sync_info.iop) ? sync_info.vact : sync_info.vact * 2;
    }

    *width = dev_info->max_width;
    *height = dev_info->max_height;

    return HI_SUCCESS;
}

hi_s32 vou_graphics_get_intf_type(GRAPHIC_LAYER layer, VO_INTF_TYPE_E *intf_type)
{
    hifb_vo_dev vo_dev;
    hi_u32 layer_index;
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }
    vo_dev = g_gfx_layer_ctx[layer_index].binded_dev;
    *intf_type = g_ast_vo_dev[vo_dev].intf_type;

    return HI_SUCCESS;
}

hi_s32 vou_graphics_set_csc_coef(GRAPHIC_LAYER layer)
{
    hi_u32 layer_index;
    hal_disp_layer gfx_layer = HAL_DISP_LAYER_BUTT;

    fb_graphic_drv_get_layer_id(layer, &gfx_layer);

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }

    return fb_graphic_drv_set_csc_coef(gfx_layer, &g_gfx_layer_ctx[layer_index].gfx_csc,
                                       &g_gfx_layer_ctx[layer_index].coef_param);
}

/* check the bind is exist or not when the first open */
hi_s32 vou_graphics_open_layer(hal_disp_layer gfx_layer)
{
    hifb_vo_dev vo_dev;
    unsigned long lock_flag = 0;
    hi_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }

    gfx_spin_lock_irqsave(&g_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
    if (!g_gfx_layer_ctx[layer_index].binded) {
        gfx_spin_unlock_irqrestore(&g_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
        hifb_graphics_trace(HI_DBG_ERR, "graphics layer %u has not been binded!\n", layer_index);
        return HI_ERR_VO_GFX_NOT_BIND;
    }

    vo_dev = g_gfx_layer_ctx[layer_index].binded_dev;

    if (!g_ast_vo_dev[vo_dev].vo_enable) {
        gfx_spin_unlock_irqrestore(&g_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
        hifb_graphics_trace(HI_DBG_ERR, "vodev %d for graphics layer %u has not been enable!\n", vo_dev, layer_index);
        return HI_ERR_VO_DEV_NOT_ENABLE;
    }
    g_gfx_layer_ctx[layer_index].opened = HI_TRUE;
    gfx_spin_unlock_irqrestore(&g_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
    return HI_SUCCESS;
}

hi_s32 vou_graphics_close_layer(hal_disp_layer gfx_layer)
{
    unsigned long lock_flag;
    hi_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != HI_SUCCESS) {
        graphics_drv_trace(HI_DBG_ERR, "gfx_layer(%u) is invalid!\n", (hi_u32)gfx_layer);
        return HI_ERR_VO_GFX_INVALID_ID;
    }

    gfx_spin_lock_irqsave(&g_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
    g_gfx_layer_ctx[layer_index].opened = HI_FALSE;
    gfx_spin_unlock_irqrestore(&g_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);

    return HI_SUCCESS;
}

hi_s32 fb_vou_graphics_show_proc(osal_proc_entry_t *s)
{
    fb_vo_csc csc;
    hi_s32 dev;
    hi_s32 i;

    osal_seq_printf(s, "\r\n");
    osal_seq_printf(s, "-----GRAPHIC LAYER CSC PARAM-----------------------------\n");
    /* 1 2 3 4  5 6 7 8  9 0 1 2  3 4 5 6 */
    osal_seq_printf(s, "%s%s%s%s%s%s\n", " LAYERID", "  Matrix", "    Luma", "    Cont",
                    "     Hue", "    Satu");
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; i++) {
        dev = g_gfx_layer_ctx[i].binded_dev;
        if (g_ast_vo_dev[dev].vo_enable) {
            csc = g_gfx_layer_ctx[i].gfx_csc;
            osal_seq_printf(s, "%8u%8u%8u%8u%8u%8u\n", i, csc.csc_matrix, csc.luma, csc.contrast,
                            csc.hue, csc.satuature);
        }
    }

    return HI_SUCCESS;
}

hi_s32 vou_graphics_enable_int(hi_u32 layer_index, hi_bool enable)
{
    vo_gfxlayer_context *vo_gfx_layer_ctx = HI_NULL;
    vo_dev_info *vo_dev = HI_NULL;
    hifb_vo_dev dev;
    vo_gfx_layer_ctx = &g_gfx_layer_ctx[layer_index];

    if (!vo_gfx_layer_ctx->binded) {
        hifb_graphics_trace(HI_DBG_ERR, "graphics layer %u has not been binded!\n", layer_index);
        return HI_ERR_VO_GFX_NOT_BIND;
    }
    dev = vo_gfx_layer_ctx->binded_dev;
    vo_dev = &g_ast_vo_dev[dev];

    if (vo_dev->vo_enable == HI_FALSE) {
        hifb_graphics_trace(HI_DBG_ERR, "Open Int Error:The vo device (%d) for graphics layer %u has been disable!\n",
                            dev, layer_index);
        return HI_FAILURE;
    }

    graphic_drv_dev_int_enable(dev, enable);
    return HI_SUCCESS;
}

hi_bool vou_graphics_clear_int(hi_u32 int_clear, hi_s32 irq)
{
    graphic_drv_int_clear(int_clear, irq);
    return HI_SUCCESS;
}

hi_bool vou_graphics_get_int(hi_u32 *int_staus)
{
    *int_staus = graphic_drv_int_get_status();
    return HI_SUCCESS;
}

hi_bool vou_graphics_clear_int_status(hi_u32 int_status)
{
    graphic_drv_clr_int_status(int_status);
    return HI_SUCCESS;
}

hi_s32 vou_graphics_get_interrupt_dev(hi_u32 int_status, hifb_vo_dev *vo_dev)
{
    return graphic_drv_get_interrupt_dev(int_status, vo_dev);
}
