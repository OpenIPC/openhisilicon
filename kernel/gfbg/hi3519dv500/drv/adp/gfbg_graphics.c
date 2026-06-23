/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_graphics.h"
#include "ot_osal.h"
#include "proc_ext.h"
#include "ot_debug.h"

typedef struct {
    td_bool vo_enable; /* Device enable flag */
    vo_intf_type intf_type; /* Device interface type */
    td_u32 max_width;  /* Device resolution maximum width */
    td_u32 max_height; /* Device resolution maximum height */
} fb_dev_info;

unsigned int g_fifb_irq = GFBG_IRQ_NR;

fb_dev_info g_ast_vo_dev[OT_VO_MAX_PHYS_DEV_NUM];

graphic_layer_context g_gfbg_gfx_layer_ctx[GFBG_MAX_LAYER_NUM];
td_u32 g_fb_dectect_zone = 0; /* Record the DDR detection area used */

unsigned int *fb_get_gfbg_irq(td_void)
{
    return &g_fifb_irq;
}

graphic_layer_context* fb_graphics_get_gfx_layer_ctx(td_void)
{
    return g_gfbg_gfx_layer_ctx;
}

static td_void fb_graphics_get_gfx_dcmp_pixel(gfbg_disp_pixel_format pixel_fmt, hal_disp_pixel_format *pen_pixel_fmt)
{
    switch (pixel_fmt) {
        case GFBG_INPUTFMT_ARGB_4444:
            *pen_pixel_fmt = HAL_INPUTFMT_ARGB_4444;
            break;
        case GFBG_INPUTFMT_ARGB_1555:
            *pen_pixel_fmt = HAL_INPUTFMT_ARGB_1555;
            break;
        case GFBG_INPUTFMT_ARGB_8888:
            *pen_pixel_fmt = HAL_INPUTFMT_ARGB_8888;
            break;
        case GFBG_INPUTFMT_CLUT_4BPP:
            *pen_pixel_fmt = HAL_INPUTFMT_CLUT_4BPP;
            break;
        case GFBG_INPUTFMT_CLUT_2BPP:
            *pen_pixel_fmt = HAL_INPUTFMT_CLUT_2BPP;
            break;
        default:
            graphics_drv_error("pixel format(%d) is invalid!\n", pixel_fmt);
            break;
    }
}

td_s32 fb_graphics_set_gfx_key_mode(ot_gfx_layer disp_layer, td_u32 key_out)
{
    return graphic_drv_set_gfx_key_mode(disp_layer, key_out);
}

td_s32 fb_graphics_set_gfx_ext(ot_gfx_layer disp_layer, fb_gfx_bitextend mode)
{
    return graphic_drv_set_gfx_ext(disp_layer, (hal_gfx_bitextend)mode);
}

td_s32 fb_graphics_set_gfx_palpha(ot_gfx_layer disp_layer, td_u32 alpha_en, td_u32 arange, td_u8 alpha0, td_u8 alpha1)
{
    return graphic_drv_set_gfx_palpha(disp_layer, alpha_en, arange, alpha0, alpha1);
}

td_s32 fb_graphics_set_layer_galpha(ot_gfx_layer disp_layer, td_u8 alpha0)
{
    return graphic_drv_layer_set_layer_galpha(disp_layer, alpha0);
}

td_s32 fb_graphics_set_csc_en(ot_gfx_layer disp_layer, td_bool csc_en)
{
    return graphic_drv_layer_set_csc_en(disp_layer, csc_en);
}

td_s32 fb_graphics_set_gfx_addr(ot_gfx_layer disp_layer, td_phys_addr_t l_addr)
{
    return graphic_drv_set_layer_addr(disp_layer, l_addr);
}

td_s32 fb_graphics_set_gfx_stride(ot_gfx_layer disp_layer, td_u16 pitch)
{
    return graphic_drv_set_gfx_stride(disp_layer, pitch);
}

td_s32 fb_graphics_get_gfx_pre_mult(ot_gfx_layer disp_layer, td_u32 *enable)
{
    return graphic_drv_get_gfx_pre_mult(disp_layer, enable);
}

td_s32 fb_graphics_set_gfx_pre_mult(ot_gfx_layer disp_layer, td_u32 enable)
{
    return graphic_drv_set_gfx_pre_mult(disp_layer, enable);
}

td_s32 fb_graphics_set_layer_data_fmt(ot_gfx_layer disp_layer, gfbg_disp_pixel_format data_fmt)
{
    hal_disp_pixel_format pix_fmt = HAL_INPUTFMT_ARGB_1555;
    fb_graphics_get_gfx_dcmp_pixel(data_fmt, &pix_fmt);
    return graphic_drv_set_layer_data_fmt(disp_layer, pix_fmt);
}

td_s32 fb_graphics_set_layer_in_rect(ot_gfx_layer disp_layer, const ot_fb_rect *rect)
{
    return graphic_drv_set_layer_in_rect(disp_layer, rect);
}

td_s32 fb_graphics_set_layer_src_image_reso(ot_gfx_layer disp_layer, const ot_fb_rect *rect)
{
    return graphic_drv_set_src_image_resolution(disp_layer, rect);
}

td_s32 fb_graphics_set_layer_out_rect(ot_gfx_layer disp_layer, const ot_fb_rect *rect)
{
    return graphic_drv_set_layer_out_rect(disp_layer, rect);
}

td_s32 fb_graphics_set_color_key_value(ot_gfx_layer disp_layer, fb_gfx_key_max vo_key_max,
    fb_gfx_key_min vo_key_min)
{
    hal_gfx_key_max key_max;
    hal_gfx_key_min key_min;
    key_max.key_max_r = vo_key_max.key_max_r;
    key_max.key_max_g = vo_key_max.key_max_g;
    key_max.key_max_b = vo_key_max.key_max_b;
    key_min.key_min_r = vo_key_min.key_min_r;
    key_min.key_min_g = vo_key_min.key_min_g;
    key_min.key_min_b = vo_key_min.key_min_b;
    return graphic_drv_set_color_key_value(disp_layer, key_max, key_min);
}

td_s32 fb_graphics_set_color_key_mask(ot_gfx_layer disp_layer, fb_gfx_mask vo_msk)
{
    hal_gfx_mask msk;
    msk.mask_r = vo_msk.mask_r;
    msk.mask_g = vo_msk.mask_g;
    msk.mask_b = vo_msk.mask_b;
    return graphic_drv_set_color_key_mask(disp_layer, msk);
}

td_s32 fb_graphics_set_gfx_key_en(ot_gfx_layer disp_layer, td_u32 key_enable)
{
    return graphic_drv_set_gfx_key_en(disp_layer, key_enable);
}

td_s32 fb_graphics_set_reg_up(ot_gfx_layer disp_layer)
{
    return graphic_drv_set_reg_up(disp_layer);
}

td_void fb_graphics_set_color_reg(td_u32 layer_id, td_phys_addr_t clut_phy_addr)
{
    graphic_drv_set_clut_reg(layer_id, clut_phy_addr);
    return;
}

td_void fb_graphics_set_color_reg_up(td_u32 layer_id)
{
    graphic_drv_set_clut_reg_up(layer_id);
    return;
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_graphics_set_frame_reg(td_u32 layer_id, td_phys_addr_t frame_phy_addr)
{
    return graphic_drv_set_smart_rect_reg(layer_id, frame_phy_addr);
}

td_void fb_graphics_up_frame_param(td_u32 layer_id)
{
    return graphic_drv_smart_rect_up_param(layer_id);
}

td_void fb_graphics_close_smart_rect(td_s32 layer_id)
{
    return graphic_drv_smart_rect_disable(layer_id);
}
#endif

td_s32 fb_graphics_init(td_void)
{
    td_s32 ret;
    ot_vo_dev vo_dev;
    hal_disp_syncinfo sync_info = {0};
    td_bool logic_ret;
    td_bool vo_enable = TD_FALSE;
    vo_intf_type intf_type = HAL_DISP_INTF_BT1120;
    td_s32 i;

    ret = fb_graphic_drv_init();

    for (i = 0; i < OT_VO_MAX_PHYS_DEV_NUM; i++) {
        vo_dev = i;
        logic_ret = graphic_drv_get_dev_enable(vo_dev, &vo_enable);
        if (logic_ret != TD_SUCCESS) {
            continue;
        } else {
            g_ast_vo_dev[vo_dev].vo_enable = vo_enable;
        }

        logic_ret = graphic_drv_get_intf_sync(vo_dev, &sync_info);
        if (logic_ret != TD_SUCCESS) {
            continue;
        } else {
            g_ast_vo_dev[vo_dev].max_width = sync_info.hact;
            /* 2 alg data */
            g_ast_vo_dev[vo_dev].max_height = (sync_info.iop) ? sync_info.vact : (sync_info.vact * 2);
        }

        logic_ret = graphic_drv_get_intf_mux_sel(vo_dev, &intf_type);
        if (logic_ret != TD_SUCCESS) {
            continue;
        } else {
            g_ast_vo_dev[vo_dev].intf_type = intf_type;
        }
    }

    return ret;
}

td_s32 fb_graphics_deinit(td_void)
{
    return fb_graphic_drv_exit();
}

td_s32 fb_graphics_get_layer_galpha(ot_gfx_layer disp_layer, td_u8 *alpha0)
{
    return graphic_drv_get_layer_galpha(disp_layer, alpha0);
}

td_s32 fb_graphics_get_layer_data_fmt(ot_gfx_layer disp_layer, td_u32 *fmt)
{
    return graphic_drv_get_layer_data_fmt(disp_layer, fmt);
}

td_s32 fb_graphics_get_gfx_addr(ot_gfx_layer disp_layer, td_phys_addr_t *gfx_addr)
{
    return graphic_drv_get_gfx_addr(disp_layer, gfx_addr);
}

td_s32 fb_graphics_get_gfx_stride(ot_gfx_layer disp_layer, td_u32 *gfx_stride)
{
    return graphic_drv_get_gfx_stride(disp_layer, gfx_stride);
}

td_s32 fb_graphics_get_dev_mode(hal_disp_layer layer, fb_scan_mode *scan_mode, td_bool *feild_update)
{
    ot_vo_dev vo_dev;
    td_u32 layer_index;
    unsigned long lock_flag;
    graphic_layer_context *gfx_layer_ctx = TD_NULL;
    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer);
        return TD_FAILURE;
    }
    gfx_layer_ctx = &g_gfbg_gfx_layer_ctx[layer_index];
    gfx_spin_lock_irqsave(&gfx_layer_ctx->spin_lock, &lock_flag);
    if (!gfx_layer_ctx->binded) {
        gfx_spin_unlock_irqrestore(&gfx_layer_ctx->spin_lock, &lock_flag);
        gfbg_graphics_error("graphics layer %d has not been binded!\n", layer_index);
        return TD_FAILURE;
    }
    vo_dev = gfx_layer_ctx->binded_dev;
    if (!g_ast_vo_dev[vo_dev].vo_enable) {
        gfx_spin_unlock_irqrestore(&gfx_layer_ctx->spin_lock, &lock_flag);
        gfbg_graphics_error("vodev %d for graphics layer %d has been disable!\n", vo_dev, layer_index);
        return TD_FAILURE;
    }
    if (fb_graphic_drv_get_scan_mode(vo_dev, (td_bool *)scan_mode) != TD_SUCCESS) {
        gfx_spin_unlock_irqrestore(&gfx_layer_ctx->spin_lock, &lock_flag);
        gfbg_graphics_error("get vodev:%d scan mode failed!\n", vo_dev);
        return TD_FAILURE;
    }
    if (graphic_drv_get_vt_thd_mode(vo_dev, feild_update) != TD_SUCCESS) {
        gfx_spin_unlock_irqrestore(&gfx_layer_ctx->spin_lock, &lock_flag);
        gfbg_graphics_error("get vodev:%d scan mode failed!\n", vo_dev);
        return TD_FAILURE;
    }
    gfx_spin_unlock_irqrestore(&gfx_layer_ctx->spin_lock, &lock_flag);
    return TD_SUCCESS;
}

td_s32 fb_graphics_resource_init(td_void)
{
    td_s32 ret;

    ret = graphic_drv_resource_init();
    if (ret != TD_SUCCESS) {
        gfbg_graphics_error("graphic drv resource init failed.\n");
        return ret;
    }

    return ret;
}

td_s32 fb_graphics_resource_deinit(td_void)
{
    td_s32 ret;
    ret = graphic_drv_resource_exit();
    return ret;
}

td_s32 fb_graphics_set_layer_enable(ot_gfx_layer gfx_layer, td_bool enable)
{
    if ((gfx_layer < LAYER_GFX_START) || (gfx_layer > LAYER_GFX_END)) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }
    return graphic_drv_set_layer_enable(gfx_layer, enable);
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_graphics_get_layer_enable(ot_gfx_layer gfx_layer, td_bool *enable)
{
    if ((gfx_layer < LAYER_GFX_START) || (gfx_layer > LAYER_GFX_END)) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }
    return graphic_drv_get_layer_enable(gfx_layer, enable);
}
#endif

td_s32 fb_graphics_set_callback(ot_gfx_layer gfx_layer, gfbg_int_type type, vo_fb_intcallback call_back, td_void *arg)
{
    unsigned long lock_flag;
    td_u32 layer_index;
    td_s32 ret = TD_SUCCESS;
    graphic_layer_context *gfx_layer_ctx = TD_NULL;

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }

    gfx_layer_ctx = &g_gfbg_gfx_layer_ctx[layer_index];

    gfx_spin_lock_irqsave(&gfx_layer_ctx->spin_lock, &lock_flag);
    switch (type) {
        case GFBG_INTTYPE_VO:
            gfx_layer_ctx->vo_callback = (fb_intcallback)call_back;
            gfx_layer_ctx->vo_callback_arg = arg;
            break;
        default:
            ret = TD_FAILURE;
            break;
    }
    gfx_spin_unlock_irqrestore(&gfx_layer_ctx->spin_lock, &lock_flag);

    return ret;
}

td_s32 fb_graphics_enable_dcmp(ot_gfx_layer gfx_layer, td_bool enable)
{
    return graphic_drv_enable_dcmp(gfx_layer, enable);
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_graphics_get_dcmp_enable_state(ot_gfx_layer gfx_layer, td_bool *enable)
{
    return graphic_drv_get_dcmp_enable_state(gfx_layer, enable);
}

td_s32 fb_graphics_set_dcmp_info(ot_gfx_layer disp_layer, const gfbg_graphic_dcmp_info *dcmp_info)
{
    graphic_dcmp_info dcmp_information = {0};

    if (dcmp_info == TD_NULL) {
        graphics_drv_error("null pointer!\n");
        return TD_FAILURE;
    }
    fb_graphics_get_gfx_dcmp_pixel(dcmp_info->pixel_fmt, &dcmp_information.pixel_fmt);
    dcmp_information.is_lossless = dcmp_info->is_lossless;
    dcmp_information.is_lossless_a = dcmp_info->is_lossless_a;
    dcmp_information.ar_phy_addr = dcmp_info->ar_phy_addr;
    dcmp_information.frame_size0 = dcmp_info->frame_size0;
    dcmp_information.frame_size1 = dcmp_info->frame_size1;
    dcmp_information.gb_phy_addr = dcmp_info->gb_phy_addr;
    dcmp_information.height = dcmp_info->height;
    dcmp_information.offset = dcmp_info->offset;
    dcmp_information.stride = dcmp_info->stride;
    dcmp_information.width = dcmp_info->width;
    return graphic_drv_set_dcmp_info(disp_layer, &dcmp_information);
}
#endif

td_s32 fb_graphics_enable_zme(ot_gfx_layer disp_layer, const ot_fb_rect *in_rect, const ot_fb_rect *out_rect,
    td_bool enable)
{
    gf_zme_cfg zme_cfg;

    if (in_rect == TD_NULL || out_rect == TD_NULL) {
        return TD_FAILURE;
    }
    zme_cfg.in_width = in_rect->width;
    zme_cfg.in_height = in_rect->height;
    zme_cfg.out_width = out_rect->width;
    zme_cfg.out_height = out_rect->height;

    return graphic_drv_enable_zme(disp_layer, &zme_cfg, enable);
}

td_s32 fb_graphics_get_intf_size(ot_gfx_layer gfx_layer, td_u32 *width, td_u32 *height)
{
    ot_vo_dev vo_dev;
    td_u32 layer_index;
    graphic_layer_context *gfx_layer_ctx = TD_NULL;
    fb_dev_info *dev_info = TD_NULL;
    td_bool ret;
    hal_disp_syncinfo sync_info = { 0 };

    if (width == TD_NULL || height == TD_NULL) {
        return TD_FAILURE;
    }

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }

    gfx_layer_ctx = &g_gfbg_gfx_layer_ctx[layer_index];

    if (!gfx_layer_ctx->binded) {
        gfbg_graphics_error("Graphics layer %d# has not been binded!\n", layer_index);
        return TD_FAILURE;
    }

    vo_dev = gfx_layer_ctx->binded_dev;
    dev_info = &g_ast_vo_dev[vo_dev];

    if (!dev_info->vo_enable) {
        gfbg_graphics_error("The vo device (%d) for graphics layer %d has been disable!\n", vo_dev,
                            layer_index);
        return TD_FAILURE;
    }

    /* Be sure to read from the register, otherwise the width and height of
    the gfbg context will not be updated when the vo timing changes. */
    ret = graphic_drv_get_intf_sync(vo_dev, &sync_info);
    if (ret != TD_SUCCESS) {
        /* keep the old value. */
    } else {
        g_ast_vo_dev[vo_dev].max_width = sync_info.hact;
        /* 2 alg data */
        g_ast_vo_dev[vo_dev].max_height = (sync_info.iop) ? sync_info.vact : (sync_info.vact * 2);
    }

    *width = dev_info->max_width;
    *height = dev_info->max_height;

    return TD_SUCCESS;
}

td_s32 fb_graphics_set_csc_coef(ot_gfx_layer gfx_layer)
{
    td_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }
    return fb_graphic_drv_set_csc_coef(gfx_layer, &g_gfbg_gfx_layer_ctx[layer_index].gfx_csc,
                                       &g_gfbg_gfx_layer_ctx[layer_index].coef_param);
}

/* check the bind is exist or not when the first open */
td_s32 fb_graphics_open_layer(ot_gfx_layer gfx_layer)
{
    ot_vo_dev vo_dev;
    unsigned long lock_flag = 0;
    td_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }

    gfx_spin_lock_irqsave(&g_gfbg_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
    if (!g_gfbg_gfx_layer_ctx[layer_index].binded) {
        gfx_spin_unlock_irqrestore(&g_gfbg_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
        gfbg_graphics_error("graphics layer %d has not been binded!\n", layer_index);
        return TD_FAILURE;
    }

    vo_dev = g_gfbg_gfx_layer_ctx[layer_index].binded_dev;

    if (!g_ast_vo_dev[vo_dev].vo_enable) {
        gfx_spin_unlock_irqrestore(&g_gfbg_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
        gfbg_graphics_error("vodev %d for graphics layer %d has not been enable!\n", vo_dev, layer_index);
        return TD_FAILURE;
    }
    g_gfbg_gfx_layer_ctx[layer_index].opened = TD_TRUE;
    gfx_spin_unlock_irqrestore(&g_gfbg_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
    return TD_SUCCESS;
}

td_s32 fb_graphics_close_layer(ot_gfx_layer gfx_layer)
{
    unsigned long lock_flag = 0;
    td_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FAILURE;
    }

    gfx_spin_lock_irqsave(&g_gfbg_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);
    g_gfbg_gfx_layer_ctx[layer_index].opened = TD_FALSE;
    gfx_spin_unlock_irqrestore(&g_gfbg_gfx_layer_ctx[layer_index].spin_lock, &lock_flag);

    return TD_SUCCESS;
}

td_s32 fb_graphics_get_int(td_u32 *int_staus)
{
    if (int_staus == TD_NULL) {
        return TD_FAILURE;
    }
    *int_staus = graphic_drv_int_get_status();
    return TD_SUCCESS;
}

td_s32 fb_graphics_get_interrupt_dev(td_u32 int_status, ot_vo_dev *vo_dev)
{
    return graphic_drv_get_interrupt_dev(int_status, vo_dev);
}

td_void fb_graphics_set_tde_sync(const gfbg_bind_tde_cfg *bind_tde_cfg)
{
    return graphics_drv_set_tde_sync(bind_tde_cfg->is_sync, bind_tde_cfg->is_hw_mute_clr_en, bind_tde_cfg->is_mute_en,
                                     bind_tde_cfg->safe_dist);
}
