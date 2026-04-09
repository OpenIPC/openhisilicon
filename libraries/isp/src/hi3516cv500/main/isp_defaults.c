/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_defaults.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_defaults.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"

hi_void isp_regs_default(hi_vi_pipe vi_pipe)
{
    return;
}

hi_void isp_regs_initialize(hi_vi_pipe vi_pipe)
{
    return;
}

/* the ext regs is a block of memory, not real register, need a default value. */
hi_void isp_ext_regs_default(hi_vi_pipe vi_pipe)
{
    hi_u32 i;

    hi_ext_system_freeze_firmware_write(vi_pipe, HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT);

    hi_ext_system_sys_debug_enable_write(vi_pipe, HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT);
    hi_ext_system_sys_debug_high_addr_write(vi_pipe, HI_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DEFAULT);
    hi_ext_system_sys_debug_low_addr_write(vi_pipe, HI_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DEFAULT);
    hi_ext_system_sys_debug_size_write(vi_pipe, HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT);
    hi_ext_system_sys_debug_depth_write(vi_pipe, HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT);
    hi_ext_system_statistics_ctrl_lowbit_write(vi_pipe, HI_EXT_SYSTEM_STATISTICS_LOWBIT_DEFAULT);
    hi_ext_system_statistics_ctrl_highbit_write(vi_pipe, HI_EXT_SYSTEM_STATISTICS_HIGHBIT_DEFAULT);
    hi_ext_system_be_free_buffer_high_addr_write(vi_pipe, HI_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DEFAULT);
    hi_ext_system_be_free_buffer_low_addr_write(vi_pipe, HI_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DEFAULT);
    hi_ext_system_be_lut_stt_buffer_high_addr_write(vi_pipe, HI_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DEFAULT);
    hi_ext_system_be_lut_stt_buffer_low_addr_write(vi_pipe, HI_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DEFAULT);
    hi_ext_system_ldci_read_stt_buffer_low_addr_write(vi_pipe, HI_EXT_SYSTEM_LDCI_READ_STT_BUF_LOW_ADDR_DEFAULT);
    hi_ext_system_ldci_read_stt_buffer_high_addr_write(vi_pipe, HI_EXT_SYSTEM_LDCI_READ_STT_BUF_HIGH_ADDR_DEFAULT);
    hi_ext_system_top_channel_select_write(vi_pipe, HI_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DEFAULT);
    hi_ext_system_top_channel_select_pre_write(vi_pipe, HI_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DEFAULT);
    hi_ext_system_isp_raw_position_enable_write(vi_pipe, HI_EXT_SYSTEM_ISP_RAW_POSITION_ENABLE_WRITE_DEFAULT);

    /* isp pipe different configs for stitch mode */
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        hi_ext_system_isp_pipe_diff_offset_write(vi_pipe, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT);
        hi_ext_system_isp_pipe_diff_gain_write(vi_pipe, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT);
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        if (i % 4 == 0) {   /* const value 4 */
            hi_ext_system_isp_pipe_diff_ccm_write(vi_pipe, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT);
        } else {
            hi_ext_system_isp_pipe_diff_ccm_write(vi_pipe, i, 0);
        }
    }

    return;
}

hi_void isp_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_u32 isp_bind_attr;
    hi_isp_cmos_sensor_max_resolution *sns_max_resolution = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_sensor_get_max_resolution(vi_pipe, &sns_max_resolution);
    hi_ext_system_sensor_max_resolution_width_write(vi_pipe, sns_max_resolution->max_width);
    hi_ext_system_sensor_max_resolution_height_write(vi_pipe, sns_max_resolution->max_height);

    /* update isp bind attr to ext regs */
    isp_bind_attr = signed_left_shift(isp_ctx->bind_attr.ae_lib.id, 8) + isp_ctx->bind_attr.awb_lib.id; /* bit8~15:ae */

    hi_ext_system_bind_attr_write(vi_pipe, isp_bind_attr);

    hi_ext_system_block_num_write(vi_pipe, isp_ctx->block_attr.block_num);

    return;
}

hi_void isp_yuv_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_u32 isp_bind_attr;
    hi_u16 ae_lib_id, awb_lib_id;

    isp_get_ctx(vi_pipe, isp_ctx);

    /* update isp bind attr to ext regs */
    ae_lib_id  = isp_ctx->bind_attr.ae_lib.id;
    awb_lib_id = isp_ctx->bind_attr.awb_lib.id;
    isp_bind_attr = (ae_lib_id << 8) | awb_lib_id; /* bit8~15:ae_lib.id */
    hi_ext_system_bind_attr_write(vi_pipe, isp_bind_attr);

    hi_ext_system_block_num_write(vi_pipe, isp_ctx->block_attr.block_num);

    return;
}

static hi_void isp_pro_shp_param_initialize(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_pro_shp_param *pro_shp_param = HI_NULL;
    hi_u8 i, j, k;
    isp_get_ctx(vi_pipe, isp_ctx);

    pro_shp_param = isp_ctx->pro_shp_param_ctrl.pro_shp_param;
    pro_shp_param->enable = HI_FALSE;
    pro_shp_param->param_num = 3; /* 3:default shp param num */
    for (k = 0; k < PRO_MAX_FRAME_NUM; k++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                pro_shp_param->shp_attr[k].texture_str[j][i] = HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURESTR_DEF;
                pro_shp_param->shp_attr[k].edge_str[j][i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGESTR_DEF;
            }

            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                pro_shp_param->shp_attr[k].luma_wgt[j][i] = HI_EXT_SYSTEM_MANUAL_SHARPEN_LUMAWGT_DEF;
            }

            pro_shp_param->shp_attr[k].texture_freq[i]  = HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTUREFREQ_DEF;
            pro_shp_param->shp_attr[k].edge_freq[i]     = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFREQ_DEF;
            pro_shp_param->shp_attr[k].over_shoot[i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEF;
            pro_shp_param->shp_attr[k].under_shoot[i]   = HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEF;
            pro_shp_param->shp_attr[k].shoot_sup_str[i] = HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPSTR_DEF;
            pro_shp_param->shp_attr[k].detail_ctrl[i]   = HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEF;
            pro_shp_param->shp_attr[k].edge_filt_str[i] = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTSTR_DEF;
            pro_shp_param->shp_attr[k].r_gain[i]        = HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEF;
            pro_shp_param->shp_attr[k].b_gain[i]        = HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEF;
            pro_shp_param->shp_attr[k].skin_gain[i]     = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEF;
        }
    }
}

static hi_void isp_pro_nr_param_initialize(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_pro_nr_param *pro_nr_param = HI_NULL;
    hi_u8 i, j, k;
    isp_get_ctx(vi_pipe, isp_ctx);

    pro_nr_param = isp_ctx->pro_nr_param_ctrl.pro_nr_param;
    pro_nr_param->enable = HI_FALSE;
    pro_nr_param->param_num = 3;  /* 3:default nr param num */
    for (k = 0; k < PRO_MAX_FRAME_NUM; k++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pro_nr_param->nr_attr[k].fine_str[i]   = HI_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT;
            pro_nr_param->nr_attr[k].coring_wgt[i] = HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT;

            for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
                pro_nr_param->nr_attr[k].chroma_str[j][i] = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
                pro_nr_param->nr_attr[k].coarse_str[j][i] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
            }
        }
    }
}

hi_void isp_alg_key_init(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_ctx->alg_key.key = 0xFFFFFFFFFFFFFFFF;
}

hi_void isp_snap_param_initialize(isp_usr_ctx *isp_ctx)
{
    hi_u8 i;

    isp_ctx->linkage.snap_pipe_mode       = ISP_SNAP_NONE;
    isp_ctx->linkage.preview_pipe_id      = -1;
    isp_ctx->linkage.picture_pipe_id      = -1;
    isp_ctx->linkage.preview_running_mode = ISP_MODE_RUNNING_OFFLINE;
    isp_ctx->linkage.picture_running_mode = ISP_MODE_RUNNING_OFFLINE;
    isp_ctx->linkage.snap_type            = SNAP_TYPE_NORMAL;

    isp_ctx->linkage.snap_state       = HI_FALSE;
    isp_ctx->linkage.pro_trigger      = HI_FALSE;
    isp_ctx->pro_frm_cnt              = 0;
    isp_ctx->linkage.iso_done_frm_cnt = 0;
    isp_ctx->linkage.pro_index        = 0;

    for (i = 0; i < ISP_SYNC_ISO_BUF_MAX; i++) {
        isp_ctx->linkage.sync_iso_buf[i] = 100; /* 100: default iso */
        isp_ctx->linkage.pro_index_buf[i] = 0;
    }
}

hi_void isp_global_initialize(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_u8   i;
    hi_u32  value;
    hi_void *value_tmp = HI_NULL;

    isp_ctx->sns_image_mode.width  = hi_ext_top_sensor_width_read(vi_pipe);
    isp_ctx->sns_image_mode.height = hi_ext_top_sensor_height_read(vi_pipe);
    value = hi_ext_system_fps_base_read(vi_pipe);
    value_tmp = (hi_void *)&value;
    isp_ctx->sns_image_mode.fps = *(hi_float *)value_tmp;

    isp_ctx->pre_sns_image_mode.width  = isp_ctx->sns_image_mode.width;
    isp_ctx->pre_sns_image_mode.height = isp_ctx->sns_image_mode.height;
    isp_ctx->pre_sns_image_mode.fps    = isp_ctx->sns_image_mode.fps;

    isp_ctx->change_wdr_mode   = HI_FALSE;
    isp_ctx->change_image_mode = HI_FALSE;
    isp_ctx->change_isp_res    = HI_FALSE;
    isp_ctx->sns_wdr_mode      = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    isp_ctx->pre_sns_wdr_mode  = isp_ctx->sns_wdr_mode;

    isp_ctx->be_raw_info.enable_be_raw    = HI_FALSE;
    isp_ctx->be_raw_info.pre_enabe_be_raw = HI_FALSE;
    isp_ctx->frame_cnt           = 0;
    isp_ctx->isp_image_mode_flag = 0;

    isp_ctx->linkage.defect_pixel = HI_FALSE;
    isp_ctx->freeze_fw            = HI_FALSE;

    isp_snap_param_initialize(isp_ctx);

    isp_ctx->linkage.stat_ready  = HI_FALSE;
    isp_ctx->linkage.run_once    = HI_FALSE;
    isp_ctx->linkage.cfg2valid_delay_max = 2;  /* 2:default delay max */
    isp_ctx->linkage.fswdr_mode          = ISP_FSWDR_NORMAL_MODE;
    isp_ctx->linkage.pre_fswdr_mode      = ISP_FSWDR_NORMAL_MODE;

    for (i = 0; i < ISP_MAX_BE_NUM; i++) {
        isp_ctx->special_opt.be_on_stt_update[i] = HI_TRUE;
    }
    isp_ctx->special_opt.fe_stt_update = HI_TRUE;
    memset_s(&isp_ctx->pre_dng_ccm, sizeof(isp_dng_ccm), 0, sizeof(isp_dng_ccm));
    memset_s(&isp_ctx->pre_dng_color_param, sizeof(hi_isp_dng_color_param), 0, sizeof(hi_isp_dng_color_param));
    memset_s(&isp_ctx->dcf_update_info, sizeof(hi_isp_dcf_update_info), 0, sizeof(hi_isp_dcf_update_info));
    memset_s(&isp_ctx->frame_info, sizeof(hi_isp_frame_info), 0, sizeof(hi_isp_frame_info));

    isp_pro_shp_param_initialize(vi_pipe);
    isp_pro_nr_param_initialize(vi_pipe);
    isp_alg_key_init(vi_pipe);
}

hi_void isp_dng_ext_regs_initialize(hi_vi_pipe vi_pipe, hi_isp_dng_color_param *dng_color_param)
{
    if (dng_color_param->wb_gain1.b_gain > 0xFFF) {
        isp_err_trace("wb_gain1.b_gain can't bigger than 0xFFF in cmos!\n");
        dng_color_param->wb_gain1.b_gain = 0xFFF;
    }
    if (dng_color_param->wb_gain1.g_gain > 0xFFF) {
        isp_err_trace("wb_gain1.g_gain can't bigger than 0xFFF in cmos!\n");
        dng_color_param->wb_gain1.g_gain = 0xFFF;
    }
    if (dng_color_param->wb_gain1.r_gain > 0xFFF) {
        isp_err_trace("wb_gain1.g_gain can't bigger than 0xFFF in cmos!\n");
        dng_color_param->wb_gain1.r_gain = 0xFFF;
    }
    if (dng_color_param->wb_gain2.b_gain > 0xFFF) {
        isp_err_trace("wb_gain2.b_gain can't bigger than 0xFFF in cmos!\n");
        dng_color_param->wb_gain2.b_gain = 0xFFF;
    }
    if (dng_color_param->wb_gain2.g_gain > 0xFFF) {
        isp_err_trace("wb_gain2.g_gain can't bigger than 0xFFF in cmos!\n");
        dng_color_param->wb_gain2.g_gain = 0xFFF;
    }
    if (dng_color_param->wb_gain2.r_gain > 0xFFF) {
        isp_err_trace("wb_gain2.r_gain can't bigger than 0xFFF in cmos!\n");
        dng_color_param->wb_gain2.r_gain = 0xFFF;
    }

    hi_ext_system_dng_high_wb_gain_r_write(vi_pipe, dng_color_param->wb_gain1.r_gain);
    hi_ext_system_dng_high_wb_gain_g_write(vi_pipe, dng_color_param->wb_gain1.g_gain);
    hi_ext_system_dng_high_wb_gain_b_write(vi_pipe, dng_color_param->wb_gain1.b_gain);
    hi_ext_system_dng_low_wb_gain_r_write(vi_pipe, dng_color_param->wb_gain2.r_gain);
    hi_ext_system_dng_low_wb_gain_g_write(vi_pipe, dng_color_param->wb_gain2.g_gain);
    hi_ext_system_dng_low_wb_gain_b_write(vi_pipe, dng_color_param->wb_gain2.b_gain);
}

