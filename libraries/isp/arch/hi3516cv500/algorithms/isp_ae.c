/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_ae.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <stdio.h>
#include <string.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"

#define HI_AE_WEIGHT_TABLE_WIDTH  17
#define HI_AE_WEIGHT_TABLE_HEIGHT 15
#define HI_AE_STAT_WEIGHT_MAX 15
#define HI_AE_WDR_GAIN_MIN       0x100  /* min wdrgain 1X */
#define HI_AE_WDR_EXP_RATIO_MIN  0x40   /* min expratio 1X */
#define HI_AE_ISP_DGAIN_MIN      0x100  /* min ispdgain 1X */
#define HI_AE_ISO_MIN  100
#define HI_AE_WDR_EXP_RATIO_SHIFT    6
#define HI_AE_HIST_SKIP_MAX 6
#define HI_AE_HIST_SKIP_MIN 1

static hi_u32 ae_piris_lin_to_fno(hi_u32 value)
{
    hi_u32 i = 0;
    hi_u32 tmp = value;

    if (value <= 1) {
        i = 0;
    } else {
        while (tmp > 1) {
            tmp = tmp >> 1;
            i++;
        }
    }

    return i;
}

static hi_s32 ae_get_dcf_info(hi_vi_pipe vi_pipe, hi_isp_ae_result *ae_result)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_dcf_update_info *isp_update_info = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_update_info = &isp_ctx->dcf_update_info;

    if (isp_update_info == HI_NULL) {
        return HI_FAILURE;
    }

    isp_update_info->exposure_bias_value = ae_result->update_info.exposure_bias_value;
    isp_update_info->iso_speed_ratings   = ae_result->update_info.iso_speed_ratings;
    isp_update_info->exposure_program    = ae_result->update_info.exposure_program;
    isp_update_info->exposure_mode       = ae_result->update_info.exposure_mode;
    isp_update_info->exposure_time       = ae_result->update_info.exposure_time;
    isp_update_info->max_aperture_value  = ae_result->update_info.max_aperture_value;
    isp_update_info->f_number            = ae_result->update_info.f_number;

    return HI_SUCCESS;
}

static hi_s32 ae_get_frame_info(hi_vi_pipe vi_pipe, hi_isp_ae_result *ae_result)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_frame_info *isp_frame = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_frame = &isp_ctx->frame_info;

    if (isp_frame == HI_NULL) {
        return HI_FAILURE;
    }

    isp_frame->exposure_time = ae_result->int_time[0] >> 4;  /* accuracy diff 4 bit */
    isp_frame->iso = ae_result->iso;
    isp_frame->isp_dgain = ae_result->isp_dgain << 2; /* left shift 2 bit, from 8 bit to 10 bit */
    isp_frame->again = ae_result->again;
    isp_frame->dgain = ae_result->dgain;
    if (is_2to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        isp_frame->ratio[0] = ((((hi_u64)ae_result->int_time[1] * ae_result->iso) << HI_AE_WDR_EXP_RATIO_SHIFT) +
                               ((hi_u64)ae_result->int_time[0] * ae_result->iso_sf) / 2) /     /* 2 */
                              div_0_to_1((hi_u64)ae_result->int_time[0] * ae_result->iso_sf);
        isp_frame->ratio[1] = HI_AE_WDR_EXP_RATIO_MIN;
        isp_frame->ratio[2] = HI_AE_WDR_EXP_RATIO_MIN;     /* 2 */
    } else {
        isp_frame->ratio[0] = HI_AE_WDR_EXP_RATIO_MIN;
        isp_frame->ratio[1] = HI_AE_WDR_EXP_RATIO_MIN;
        isp_frame->ratio[2] = HI_AE_WDR_EXP_RATIO_MIN;     /* 2 */
    }

    isp_frame->f_number   = ae_piris_lin_to_fno(ae_result->piris_gain);
    isp_frame->hmax_times = ae_result->hmax_times;
    isp_frame->vmax       = ae_result->vmax;

    return HI_SUCCESS;
}

static hi_void ae_regs_range_check(hi_vi_pipe vi_pipe, hi_isp_ae_result *ae_result)
{
    hi_u32 i, j;

    ae_result->stat_attr.ae_be_sel       = MIN2(ae_result->stat_attr.ae_be_sel, 2); /* 2 mean be after DRC */
    ae_result->stat_attr.four_plane_mode = MIN2(ae_result->stat_attr.four_plane_mode, 1);
    if (ae_result->stat_attr.four_plane_mode) {
        ae_result->stat_attr.hist_skip_x = MIN2(ae_result->stat_attr.hist_skip_x, HI_AE_HIST_SKIP_MAX);
    } else {
        ae_result->stat_attr.hist_skip_x = MIN2(ae_result->stat_attr.hist_skip_x, HI_AE_HIST_SKIP_MAX);
        ae_result->stat_attr.hist_skip_x = MAX2(ae_result->stat_attr.hist_skip_x, HI_AE_HIST_SKIP_MIN);
    }
    ae_result->stat_attr.hist_skip_y   = MIN2(ae_result->stat_attr.hist_skip_y, HI_AE_HIST_SKIP_MAX);
    ae_result->stat_attr.hist_offset_x = MIN2(ae_result->stat_attr.hist_offset_x, 1);
    ae_result->stat_attr.hist_offset_y = MIN2(ae_result->stat_attr.hist_offset_y, 1);

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            ae_result->stat_attr.weight_table[vi_pipe][i][j] =
                MIN2(ae_result->stat_attr.weight_table[vi_pipe][i][j], HI_AE_STAT_WEIGHT_MAX);
        }
    }

    ae_result->stat_attr.hist_mode    = MIN2(ae_result->stat_attr.hist_mode, 1);
    ae_result->stat_attr.aver_mode    = MIN2(ae_result->stat_attr.aver_mode, 1);
    ae_result->stat_attr.max_gain_mode = MIN2(ae_result->stat_attr.max_gain_mode, 1);
}

static hi_void ae_res_regs_default(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_rect block_rect;
    isp_mg_static_cfg  *mg_static_reg_cfg  = HI_NULL;
    isp_ae_static_cfg  *ae_static_reg_cfg  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    /* AE BE configs */
    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        /* AE&MG&DG BLC configs */
        ae_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].ae_reg_cfg.static_reg_cfg;
        mg_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].mg_reg_cfg.static_reg_cfg;

        /* AE&MG&DG size configs */
        isp_get_block_rect(&block_rect, &isp_ctx->block_attr, i);

        ae_static_reg_cfg->be_crop_pos_y      = 0;
        ae_static_reg_cfg->be_crop_out_height = block_rect.height;
        ae_static_reg_cfg->be_crop_pos_x      = 0;
        ae_static_reg_cfg->be_crop_out_width  = block_rect.width;
        mg_static_reg_cfg->crop_pos_y         = 0;
        mg_static_reg_cfg->crop_out_height    = block_rect.height;
        mg_static_reg_cfg->crop_pos_x         = 0;
        mg_static_reg_cfg->crop_out_width     = block_rect.width;

        /* AE&MG overlap configs */
        if (i == 0) {
            if (isp_ctx->block_attr.block_num > 1) {
                ae_static_reg_cfg->be_crop_out_width  = block_rect.width - isp_ctx->block_attr.over_lap;
                mg_static_reg_cfg->crop_out_width    = block_rect.width - isp_ctx->block_attr.over_lap;
            }
        } else if (i == (isp_ctx->block_attr.block_num - 1)) {
            ae_static_reg_cfg->be_crop_pos_x     = isp_ctx->block_attr.over_lap;
            ae_static_reg_cfg->be_crop_out_width = block_rect.width - isp_ctx->block_attr.over_lap;
            mg_static_reg_cfg->crop_pos_x        = isp_ctx->block_attr.over_lap;
            mg_static_reg_cfg->crop_out_width    = block_rect.width - isp_ctx->block_attr.over_lap;
        } else {
            ae_static_reg_cfg->be_crop_pos_x     = isp_ctx->block_attr.over_lap;
            ae_static_reg_cfg->be_crop_out_width = block_rect.width - (isp_ctx->block_attr.over_lap << 1);
            mg_static_reg_cfg->crop_pos_x        = isp_ctx->block_attr.over_lap;
            mg_static_reg_cfg->crop_out_width    = block_rect.width - (isp_ctx->block_attr.over_lap << 1);
        }
    }

    /* AE FE configs */
    ae_static_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.static_reg_cfg;

    /* crop configs */
    ae_static_reg_cfg->fe_crop_pos_x = 0;
    ae_static_reg_cfg->fe_crop_pos_y = 0;
    ae_static_reg_cfg->fe_crop_out_height = isp_ctx->sys_rect.height;
    ae_static_reg_cfg->fe_crop_out_width  = isp_ctx->sys_rect.width;

    hi_ext_system_ae_crop_en_write(vi_pipe, HI_EXT_SYSTEM_CROP_EN_DEFAULT);
    hi_ext_system_ae_crop_x_write(vi_pipe, 0);
    hi_ext_system_ae_crop_y_write(vi_pipe, 0);
    hi_ext_system_ae_crop_height_write(vi_pipe, isp_ctx->block_attr.frame_rect.height);
    hi_ext_system_ae_crop_width_write(vi_pipe, isp_ctx->block_attr.frame_rect.width);

    reg_cfg->cfg_key.bit1_ae_cfg1 = HI_TRUE;
    reg_cfg->cfg_key.bit1_ae_cfg2 = HI_TRUE;
}

static hi_void ae_res_read_extregs(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i;
    hi_u8  block_num;
    hi_u8  crop_en;
    hi_u16 overlap;
    hi_u32 crop_x, crop_y, crop_height, crop_width;

    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_rect block_rect;
    isp_mg_static_cfg *mg_static_reg_cfg = HI_NULL;
    isp_ae_static_cfg *ae_static_reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    block_num = isp_ctx->block_attr.block_num;
    overlap = isp_ctx->block_attr.over_lap;

    crop_en = hi_ext_system_ae_crop_en_read(vi_pipe);
    crop_x = (hi_ext_system_ae_crop_x_read(vi_pipe) >> 2) << 2; /* shift 2 bit to avoid error */
    crop_y = (hi_ext_system_ae_crop_y_read(vi_pipe) >> 2) << 2; /* shift 2 bit to avoid error */
    crop_height = (hi_ext_system_ae_crop_height_read(vi_pipe) >> 2) << 2; /* shift 2 bit to avoid error */
    crop_width = (hi_ext_system_ae_crop_width_read(vi_pipe) >> 2) << 2; /* shift 2 bit to avoid error */

    crop_width  = MAX2(crop_width,  AE_MIN_WIDTH);
    crop_height = MAX2(crop_height, AE_MIN_HEIGHT);
    crop_width  = MIN2(crop_width, isp_ctx->block_attr.frame_rect.width);
    crop_height = MIN2(crop_height, isp_ctx->block_attr.frame_rect.height);
    crop_x      = MIN2(crop_x, (isp_ctx->block_attr.frame_rect.width - crop_width));
    crop_y      = MIN2(crop_y, (isp_ctx->block_attr.frame_rect.height - crop_height));

    /* AE BE configs */
    for (i = 0; i < block_num; i++) {
        /* AE&MG&DG BLC configs */
        ae_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].ae_reg_cfg.static_reg_cfg;
        mg_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].mg_reg_cfg.static_reg_cfg;

        /* AE&MG&DG size configs */
        isp_get_block_rect(&block_rect, &isp_ctx->block_attr, i);

        /* AE&MG overlap configs */
        if (i == 0) {
            if (block_num > 1) {
                ae_static_reg_cfg->be_crop_pos_x      = 0;
                ae_static_reg_cfg->be_crop_pos_y      = 0;
                ae_static_reg_cfg->be_crop_out_height = block_rect.height;
                ae_static_reg_cfg->be_crop_out_width  = block_rect.width - overlap;

                mg_static_reg_cfg->crop_pos_x      = 0;
                mg_static_reg_cfg->crop_pos_y      = 0;
                mg_static_reg_cfg->crop_out_height = block_rect.height;
                mg_static_reg_cfg->crop_out_width  = block_rect.width - overlap;
            } else if (crop_en) {
                ae_static_reg_cfg->be_crop_pos_x      = crop_x;
                ae_static_reg_cfg->be_crop_pos_y      = crop_y;
                ae_static_reg_cfg->be_crop_out_height = crop_height;
                ae_static_reg_cfg->be_crop_out_width  = crop_width;

                mg_static_reg_cfg->crop_pos_x      = crop_x;
                mg_static_reg_cfg->crop_pos_y      = crop_y;
                mg_static_reg_cfg->crop_out_height = crop_height;
                mg_static_reg_cfg->crop_out_width  = crop_width;
            } else {
                ae_static_reg_cfg->be_crop_pos_x      = 0;
                ae_static_reg_cfg->be_crop_pos_y      = 0;
                ae_static_reg_cfg->be_crop_out_height = block_rect.height;
                ae_static_reg_cfg->be_crop_out_width  = block_rect.width;

                mg_static_reg_cfg->crop_pos_x      = 0;
                mg_static_reg_cfg->crop_pos_y      = 0;
                mg_static_reg_cfg->crop_out_height = block_rect.height;
                mg_static_reg_cfg->crop_out_width  = block_rect.width;
            }
        } else if (i == (block_num - 1)) {
            ae_static_reg_cfg->be_crop_pos_x      = overlap;
            ae_static_reg_cfg->be_crop_pos_y      = 0;
            ae_static_reg_cfg->be_crop_out_height = block_rect.height;
            ae_static_reg_cfg->be_crop_out_width  = block_rect.width - overlap;

            mg_static_reg_cfg->crop_pos_x      = overlap;
            mg_static_reg_cfg->crop_pos_y      = 0;
            mg_static_reg_cfg->crop_out_height = block_rect.height;
            mg_static_reg_cfg->crop_out_width  = block_rect.width - overlap;
        } else {
            ae_static_reg_cfg->be_crop_pos_x      = overlap;
            ae_static_reg_cfg->be_crop_pos_y      = 0;
            ae_static_reg_cfg->be_crop_out_height = block_rect.height;
            ae_static_reg_cfg->be_crop_out_width  = block_rect.width - (overlap << 1);

            mg_static_reg_cfg->crop_pos_x      = overlap;
            mg_static_reg_cfg->crop_pos_y      = 0;
            mg_static_reg_cfg->crop_out_height = block_rect.height;
            mg_static_reg_cfg->crop_out_width  = block_rect.width - (overlap << 1);
        }
    }

    /* AE FE configs */
    ae_static_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.static_reg_cfg;

    /* crop configs */
    if (crop_en) {
        ae_static_reg_cfg->fe_crop_pos_x = crop_x;
        ae_static_reg_cfg->fe_crop_pos_y = crop_y;
        ae_static_reg_cfg->fe_crop_out_height = crop_height;
        ae_static_reg_cfg->fe_crop_out_width  = crop_width;
    } else {
        ae_static_reg_cfg->fe_crop_pos_x = 0;
        ae_static_reg_cfg->fe_crop_pos_y = 0;
        ae_static_reg_cfg->fe_crop_out_height = isp_ctx->sys_rect.height;
        ae_static_reg_cfg->fe_crop_out_width  = isp_ctx->sys_rect.width;
    }

    reg_cfg->cfg_key.bit1_ae_cfg1 = HI_TRUE;
    reg_cfg->cfg_key.bit1_ae_cfg2 = HI_TRUE;
}

static hi_void ae_regs_default(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i, j, k;
    hi_u8 wdr_mode;
    hi_u8 block_num;
    hi_u8 hist_skip_x = 0;
    hi_u8 hist_skip_y = 0;
    hi_u8 hist_offset_x = 0;
    hi_u8 hist_offset_y = 0;
    hi_u8 block_offset_x = 0;
    hi_u8 sensor_pattern_type;

    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level   = HI_NULL;
    isp_mg_dyna_cfg      *mg_dyna_reg_cfg      = HI_NULL;
    isp_mg_static_cfg    *mg_static_reg_cfg    = HI_NULL;
    isp_ae_static_cfg    *ae_static_reg_cfg    = HI_NULL;
    isp_ae_dyna_cfg      *ae_dyna_reg_cfg      = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    block_num = isp_ctx->block_attr.block_num;
    wdr_mode  = isp_ctx->sns_wdr_mode;
    sensor_pattern_type = hi_ext_system_rggb_cfg_read(vi_pipe);

    hi_u8 weight_table[HI_AE_WEIGHT_TABLE_HEIGHT][HI_AE_WEIGHT_TABLE_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},     /* 2 */
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    if ((sensor_pattern_type == HI_ISP_TOP_RGGB_START_R_GR_GB_B) ||
        (sensor_pattern_type == HI_ISP_TOP_RGGB_START_B_GB_GR_R)) {
        hist_skip_x = 1;
        hist_skip_y = 1;
        hist_offset_x = 1;
        hist_offset_y = 0;
    } else {
        hist_skip_x = 1;
        hist_skip_y = 1;
        hist_offset_x = 0;
        hist_offset_y = 0;
    }

    /* AE BE configs */
    for (i = 0; i < block_num; i++) {
        /* AE&MG&DG BLC configs */
        ae_dyna_reg_cfg   = &reg_cfg->alg_reg_cfg[i].ae_reg_cfg.dyna_reg_cfg;
        ae_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].ae_reg_cfg.static_reg_cfg;

        mg_dyna_reg_cfg   = &reg_cfg->alg_reg_cfg[i].mg_reg_cfg.dyna_reg_cfg;
        mg_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].mg_reg_cfg.static_reg_cfg;

        ae_static_reg_cfg->be_enable  = HI_TRUE;
        mg_static_reg_cfg->enable    = HI_TRUE;

        /* AE&MG WDR configs */
        ae_dyna_reg_cfg->be_bit_move    = 0;
        ae_dyna_reg_cfg->be_gamma_limit = 6;  /* experience value 6 */
        mg_dyna_reg_cfg->bit_move       = 0;
        mg_dyna_reg_cfg->gamma_limit    = 3;  /* experience value 3 */
        if (is_linear_mode(wdr_mode)) {
            ae_dyna_reg_cfg->be_hist_gamma_mode = 0;
            ae_dyna_reg_cfg->be_aver_gamma_mode = 0;
            mg_dyna_reg_cfg->gamma_mode         = 0;
        } else {
            ae_dyna_reg_cfg->be_hist_gamma_mode = 1;
            ae_dyna_reg_cfg->be_aver_gamma_mode = 1;
            mg_dyna_reg_cfg->gamma_mode       = 1;
        }

        /* MPI configs */
        ae_dyna_reg_cfg->be_ae_sel         = 1;
        ae_dyna_reg_cfg->be_four_plane_mode = 0;
        ae_dyna_reg_cfg->be_hist_skip_x     = hist_skip_x;
        ae_dyna_reg_cfg->be_hist_skip_y     = hist_skip_y;
        ae_dyna_reg_cfg->be_hist_offset_x   = hist_offset_x;
        ae_dyna_reg_cfg->be_hist_offset_y   = hist_offset_y;

        /* weight table configs */
        ae_dyna_reg_cfg->be_weight_table_update = HI_TRUE;
        if (i < HI_AE_WEIGHT_TABLE_WIDTH % div_0_to_1(block_num)) {
            ae_dyna_reg_cfg->be_weight_table_width = HI_AE_WEIGHT_TABLE_WIDTH / div_0_to_1(block_num) + 1;
            mg_dyna_reg_cfg->zone_width          = HI_AE_WEIGHT_TABLE_WIDTH / div_0_to_1(block_num) + 1;
        } else {
            ae_dyna_reg_cfg->be_weight_table_width = HI_AE_WEIGHT_TABLE_WIDTH / div_0_to_1(block_num);
            mg_dyna_reg_cfg->zone_width          = HI_AE_WEIGHT_TABLE_WIDTH / div_0_to_1(block_num);
        }
        ae_dyna_reg_cfg->be_weight_table_height = HI_AE_WEIGHT_TABLE_HEIGHT;
        mg_dyna_reg_cfg->zone_height          = HI_AE_WEIGHT_TABLE_HEIGHT;
        for (j = 0; j < ae_dyna_reg_cfg->be_weight_table_height; j++) {
            for (k = 0; k < ae_dyna_reg_cfg->be_weight_table_width; k++) {
                ae_dyna_reg_cfg->be_weight_table[j][k] = weight_table[j][k + block_offset_x];
            }
        }
        block_offset_x += ae_dyna_reg_cfg->be_weight_table_width;
    }

    /* AE FE configs */
    ae_dyna_reg_cfg   = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg;
    ae_static_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.static_reg_cfg;
    mg_static_reg_cfg = &reg_cfg->alg_reg_cfg[0].mg_reg_cfg.static_reg_cfg;

    /* BLC configs */
    ae_static_reg_cfg->fe_enable = HI_TRUE;

    /* WDR configs */
    ae_dyna_reg_cfg->fe_bit_move       = 0;
    ae_dyna_reg_cfg->fe_gamma_limit    = 6;  /* experience value 6 */
    ae_dyna_reg_cfg->fe_hist_gamma_mode = 0;
    ae_dyna_reg_cfg->fe_aver_gamma_mode = 0;

    /* MPI configs */
    ae_dyna_reg_cfg->fe_four_plane_mode = 0;
    ae_dyna_reg_cfg->fe_hist_skip_x     = hist_skip_x;
    ae_dyna_reg_cfg->fe_hist_skip_y     = hist_skip_y;
    ae_dyna_reg_cfg->fe_hist_offset_x   = hist_offset_x;
    ae_dyna_reg_cfg->fe_hist_offset_y   = hist_offset_y;

    /* weight tbale configs */
    ae_dyna_reg_cfg->fe_weight_table_update  = HI_TRUE;
    ae_dyna_reg_cfg->fe_weight_table_width  = HI_AE_WEIGHT_TABLE_WIDTH;
    ae_dyna_reg_cfg->fe_weight_table_height = HI_AE_WEIGHT_TABLE_HEIGHT;

    memcpy_s(ae_dyna_reg_cfg->fe_weight_table, HI_AE_WEIGHT_TABLE_HEIGHT * HI_AE_WEIGHT_TABLE_WIDTH * sizeof(hi_u8),
             weight_table, HI_AE_WEIGHT_TABLE_HEIGHT * HI_AE_WEIGHT_TABLE_WIDTH * sizeof(hi_u8));

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            hi_ext_system_ae_weight_table_write(vi_pipe, (i * AE_ZONE_COLUMN + j), weight_table[i][j]);
        }
    }

    hi_ext_system_ae_be_sel_write(vi_pipe, ae_dyna_reg_cfg->be_ae_sel);
    hi_ext_system_ae_fourplanemode_write(vi_pipe, ae_dyna_reg_cfg->be_four_plane_mode);

    hi_ext_system_ae_hist_skip_x_write(vi_pipe, hist_skip_x);
    hi_ext_system_ae_hist_skip_y_write(vi_pipe, hist_skip_y);
    hi_ext_system_ae_hist_offset_x_write(vi_pipe, hist_offset_x);
    hi_ext_system_ae_hist_offset_y_write(vi_pipe, hist_offset_y);

    hi_ext_system_ae_fe_en_write(vi_pipe, ae_static_reg_cfg->fe_enable);
    hi_ext_system_ae_be_en_write(vi_pipe, ae_static_reg_cfg->be_enable);
    hi_ext_system_mg_en_write(vi_pipe, mg_static_reg_cfg->enable);

    if (is_linear_mode(wdr_mode)) {
        hi_ext_system_ae_histmode_write(vi_pipe, 0);
        hi_ext_system_ae_avermode_write(vi_pipe, 0);
        hi_ext_system_ae_maxgainmode_write(vi_pipe, 0);
    } else {
        hi_ext_system_ae_histmode_write(vi_pipe, 1);
        hi_ext_system_ae_avermode_write(vi_pipe, 1);
        hi_ext_system_ae_maxgainmode_write(vi_pipe, 1);
    }

    ae_res_regs_default(vi_pipe, reg_cfg);

    hi_ext_system_smart_update_write(vi_pipe, HI_EXT_SYSTEM_SMART_UPDATE_DEFAULT);
    for (i = 0; i < SMART_CLASS_MAX; i++) {
        hi_ext_system_smart_enable_write(vi_pipe, i, HI_EXT_SYSTEM_SMART_ENABLE_DEFAULT);
        hi_ext_system_smart_available_write(vi_pipe, i, HI_EXT_SYSTEM_SMART_AVAILABLE_DEFAULT);
        hi_ext_system_smart_luma_write(vi_pipe, i, HI_EXT_SYSTEM_SMART_LUMA_DEFAULT);
    }
}

static hi_void ae_read_extregs(hi_vi_pipe vi_pipe, hi_isp_ae_result *ae_result)
{
    hi_u32 i, j;

    if (!ae_result->stat_attr.hist_adjust) {
        if (ae_result->stat_attr.ae_be_sel != hi_ext_system_ae_be_sel_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.ae_be_sel = hi_ext_system_ae_be_sel_read(vi_pipe);
        }

        if (ae_result->stat_attr.four_plane_mode != hi_ext_system_ae_fourplanemode_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.four_plane_mode = hi_ext_system_ae_fourplanemode_read(vi_pipe);
        }

        if (ae_result->stat_attr.hist_offset_x != hi_ext_system_ae_hist_offset_x_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.hist_offset_x = hi_ext_system_ae_hist_offset_x_read(vi_pipe);
        }

        if (ae_result->stat_attr.hist_offset_y != hi_ext_system_ae_hist_offset_y_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.hist_offset_y = hi_ext_system_ae_hist_offset_y_read(vi_pipe);
        }

        if (ae_result->stat_attr.hist_skip_x != hi_ext_system_ae_hist_skip_x_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.hist_skip_x = hi_ext_system_ae_hist_skip_x_read(vi_pipe);
        }

        if (ae_result->stat_attr.hist_skip_y != hi_ext_system_ae_hist_skip_y_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.hist_skip_y = hi_ext_system_ae_hist_skip_y_read(vi_pipe);
        }
    } else {
        hi_ext_system_ae_be_sel_write(vi_pipe, ae_result->stat_attr.ae_be_sel);
        hi_ext_system_ae_fourplanemode_write(vi_pipe, ae_result->stat_attr.four_plane_mode);
        hi_ext_system_ae_hist_skip_x_write(vi_pipe, ae_result->stat_attr.hist_skip_x);
        hi_ext_system_ae_hist_skip_y_write(vi_pipe, ae_result->stat_attr.hist_skip_y);
        hi_ext_system_ae_hist_offset_x_write(vi_pipe, ae_result->stat_attr.hist_offset_x);
        hi_ext_system_ae_hist_offset_y_write(vi_pipe, ae_result->stat_attr.hist_offset_y);
    }

    if (!ae_result->stat_attr.wight_table_update) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                ae_result->stat_attr.change = (ae_result->stat_attr.weight_table[vi_pipe][i][j] !=
                    hi_ext_system_ae_weight_table_read(vi_pipe, (i * AE_ZONE_COLUMN + j)) ?
                    HI_TRUE : ae_result->stat_attr.change);
                ae_result->stat_attr.weight_table[vi_pipe][i][j] =
                    hi_ext_system_ae_weight_table_read(vi_pipe, (i * AE_ZONE_COLUMN + j));
            }
        }
    } else {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                hi_ext_system_ae_weight_table_write(vi_pipe,
                    (i * AE_ZONE_COLUMN + j), ae_result->stat_attr.weight_table[vi_pipe][i][j]);
            }
        }
    }

    if (!ae_result->stat_attr.mode_update) {
        if (ae_result->stat_attr.hist_mode != hi_ext_system_ae_histmode_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.hist_mode = hi_ext_system_ae_histmode_read(vi_pipe);
        }

        if (ae_result->stat_attr.aver_mode != hi_ext_system_ae_avermode_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.aver_mode = hi_ext_system_ae_avermode_read(vi_pipe);
        }

        if (ae_result->stat_attr.max_gain_mode != hi_ext_system_ae_maxgainmode_read(vi_pipe)) {
            ae_result->stat_attr.change = HI_TRUE;
            ae_result->stat_attr.max_gain_mode = hi_ext_system_ae_maxgainmode_read(vi_pipe);
        }
    } else {
        hi_ext_system_ae_histmode_write(vi_pipe, ae_result->stat_attr.hist_mode);
        hi_ext_system_ae_avermode_write(vi_pipe, ae_result->stat_attr.aver_mode);
        hi_ext_system_ae_maxgainmode_write(vi_pipe, ae_result->stat_attr.max_gain_mode);
    }
}

static hi_void ae_update_config(hi_vi_pipe vi_pipe, hi_isp_ae_result *ae_result, isp_reg_cfg *reg_cfg)
{
    hi_u8  block_num;
    hi_u8  block_offset_x = 0;
    hi_s32 i, j, k;
    hi_u32 sns_gain_sf, sns_gain;
    hi_u32 isp_gain_sf, isp_gain;

    hi_isp_ae_stat_attr *stat_attr     = HI_NULL;
    isp_usr_ctx          *isp_ctx      = HI_NULL;
    isp_ae_dyna_cfg  *ae_dyna_reg_cfg  = HI_NULL;
    isp_ae_static_cfg *ae_stat_reg_cfg = HI_NULL;
    isp_ae_reg_cfg_2 *ae_reg_cfg2      = HI_NULL;
    isp_mg_dyna_cfg  *mg_dyna_reg_cfg  = HI_NULL;
    isp_mg_static_cfg *mg_stat_reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    block_num   = isp_ctx->block_attr.block_num;
    stat_attr   = &ae_result->stat_attr;
    ae_reg_cfg2 = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg2;

    for (i = 0; i < block_num; i++) {
        ae_stat_reg_cfg = &reg_cfg->alg_reg_cfg[i].ae_reg_cfg.static_reg_cfg;
        mg_stat_reg_cfg = &reg_cfg->alg_reg_cfg[i].mg_reg_cfg.static_reg_cfg;

        ae_stat_reg_cfg->fe_enable = hi_ext_system_ae_fe_en_read(vi_pipe);
        ae_stat_reg_cfg->be_enable = hi_ext_system_ae_be_en_read(vi_pipe);
        mg_stat_reg_cfg->enable   = hi_ext_system_mg_en_read(vi_pipe);
    }

    /* BE configs update */
    if (stat_attr->hist_adjust) {
        ae_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg;

        if ((ae_dyna_reg_cfg->be_ae_sel != stat_attr->ae_be_sel) ||
            (ae_dyna_reg_cfg->be_four_plane_mode != stat_attr->four_plane_mode) ||
            (ae_dyna_reg_cfg->be_hist_skip_x != stat_attr->hist_skip_x) ||
            (ae_dyna_reg_cfg->be_hist_skip_y != stat_attr->hist_skip_y) ||
            (ae_dyna_reg_cfg->be_hist_offset_x != stat_attr->hist_offset_x) ||
            (ae_dyna_reg_cfg->be_hist_offset_y != stat_attr->hist_offset_y)) {
            stat_attr->change = HI_TRUE;
        }
    }

    if (stat_attr->mode_update) {
        ae_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg;
        mg_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[0].mg_reg_cfg.dyna_reg_cfg;

        if ((ae_dyna_reg_cfg->be_hist_gamma_mode != stat_attr->hist_mode) ||
            (ae_dyna_reg_cfg->be_aver_gamma_mode != stat_attr->aver_mode) ||
            (mg_dyna_reg_cfg->gamma_mode != stat_attr->max_gain_mode)) {
            stat_attr->change = HI_TRUE;
        }
    }

    if (stat_attr->wight_table_update) {
        ae_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg;

        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                stat_attr->change = (ae_dyna_reg_cfg->be_weight_table[i][j] != stat_attr->weight_table[vi_pipe][i][j] ?
                    HI_TRUE : stat_attr->change);
            }
        }
    }

    for (i = 0; i < block_num; i++) {
        ae_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[i].ae_reg_cfg.dyna_reg_cfg;
        mg_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[i].mg_reg_cfg.dyna_reg_cfg;

        ae_dyna_reg_cfg->be_ae_sel          = stat_attr->ae_be_sel;
        ae_dyna_reg_cfg->be_four_plane_mode = stat_attr->four_plane_mode;
        ae_dyna_reg_cfg->be_hist_skip_x     = stat_attr->hist_skip_x;
        ae_dyna_reg_cfg->be_hist_skip_y     = stat_attr->hist_skip_y;
        ae_dyna_reg_cfg->be_hist_offset_x   = stat_attr->hist_offset_x;
        ae_dyna_reg_cfg->be_hist_offset_y   = stat_attr->hist_offset_y;

        ae_dyna_reg_cfg->be_hist_gamma_mode = stat_attr->hist_mode;
        ae_dyna_reg_cfg->be_aver_gamma_mode = stat_attr->aver_mode;
        mg_dyna_reg_cfg->gamma_mode       = stat_attr->max_gain_mode;

        ae_dyna_reg_cfg->be_weight_table_update = HI_TRUE;

        if (i < HI_AE_WEIGHT_TABLE_WIDTH % div_0_to_1(block_num)) {
            ae_dyna_reg_cfg->be_weight_table_width = HI_AE_WEIGHT_TABLE_WIDTH / div_0_to_1(block_num) + 1;
        } else {
            ae_dyna_reg_cfg->be_weight_table_width = HI_AE_WEIGHT_TABLE_WIDTH / div_0_to_1(block_num);
        }

        ae_dyna_reg_cfg->be_weight_table_height = HI_AE_WEIGHT_TABLE_HEIGHT;
        for (j = 0; j < ae_dyna_reg_cfg->be_weight_table_height; j++) {
            for (k = 0; k < ae_dyna_reg_cfg->be_weight_table_width; k++) {
                ae_dyna_reg_cfg->be_weight_table[j][k] = stat_attr->weight_table[vi_pipe][j][k + block_offset_x];
            }
        }

        block_offset_x += ae_dyna_reg_cfg->be_weight_table_width;

        reg_cfg->cfg_key.bit1_ae_cfg1 = HI_TRUE;
        reg_cfg->cfg_key.bit1_ae_cfg2 = HI_TRUE;
    }

    /* FE configs update */
    ae_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg;

    ae_dyna_reg_cfg->fe_four_plane_mode = stat_attr->four_plane_mode;
    ae_dyna_reg_cfg->fe_hist_skip_x     = stat_attr->hist_skip_x;
    ae_dyna_reg_cfg->fe_hist_skip_y     = stat_attr->hist_skip_y;
    ae_dyna_reg_cfg->fe_hist_offset_x   = stat_attr->hist_offset_x;
    ae_dyna_reg_cfg->fe_hist_offset_y   = stat_attr->hist_offset_y;

    ae_dyna_reg_cfg->fe_weight_table_update = HI_TRUE;
    ae_dyna_reg_cfg->fe_weight_table_width  = HI_AE_WEIGHT_TABLE_WIDTH;
    ae_dyna_reg_cfg->fe_weight_table_height = HI_AE_WEIGHT_TABLE_HEIGHT;

    for (j = 0; j < HI_AE_WEIGHT_TABLE_HEIGHT; j++) {
        for (k = 0; k < HI_AE_WEIGHT_TABLE_WIDTH; k++) {
            ae_dyna_reg_cfg->fe_weight_table[j][k] = stat_attr->weight_table[vi_pipe][j][k];
        }
    }
    stat_attr->change = HI_FALSE;

    ae_reg_cfg2->int_time[0] = ae_result->int_time[0];
    ae_reg_cfg2->int_time[1] = ae_result->int_time[1];
    ae_reg_cfg2->int_time[2] = ae_result->int_time[2];     /* 2 */
    ae_reg_cfg2->int_time[3] = ae_result->int_time[3];     /* 3 */
    ae_reg_cfg2->isp_dgain   = ae_result->isp_dgain;
    ae_reg_cfg2->piris_valid = ae_result->piris_valid;
    ae_reg_cfg2->piris_pos   = ae_result->piris_pos;
    ae_reg_cfg2->fs_wdr_mode = ae_result->fswdr_mode;
    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        ae_result->wdr_gain[i] = MAX2(ae_result->wdr_gain[i], HI_AE_WDR_GAIN_MIN);
        ae_reg_cfg2->wdr_gain[i] = ae_result->wdr_gain[i];
    }
    /* be careful avoid overflow */
    if (is_2to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        sns_gain_sf = (ae_result->again_sf * ae_result->dgain_sf) >> 10; /* 10 */
        sns_gain = (ae_result->again * ae_result->dgain) >> 10; /* 10 */
        isp_gain_sf = (ae_result->isp_dgain * ae_result->wdr_gain[0]) >> 8; /* 8 */
        isp_gain = (ae_result->isp_dgain * ae_result->wdr_gain[1]) >> 8; /* 8 */

        if (ae_result->piris_valid == HI_TRUE) {
            ae_reg_cfg2->exposure = ((hi_u64)ae_result->sns_lhcg_exp_ratio * 
                ae_result->int_time[1] * sns_gain * isp_gain * ae_result->piris_gain) >> HI_AE_WDR_EXP_RATIO_SHIFT;
            ae_reg_cfg2->exposure_sf = (hi_u64)ae_result->int_time[0] * sns_gain_sf * isp_gain_sf *
                ae_result->piris_gain;
        } else {
            ae_reg_cfg2->exposure = ((hi_u64)ae_result->sns_lhcg_exp_ratio * 
                ae_result->int_time[1] * sns_gain * isp_gain) >> HI_AE_WDR_EXP_RATIO_SHIFT;
            ae_reg_cfg2->exposure_sf = (hi_u64)ae_result->int_time[0] * sns_gain_sf * isp_gain_sf;
        }
    } else {
        if (ae_result->piris_valid == HI_TRUE) {
            ae_reg_cfg2->exposure = (hi_u64)ae_result->int_time[0] * ae_result->iso * ae_result->piris_gain;
        } else {
            ae_reg_cfg2->exposure = (hi_u64)ae_result->int_time[0] * ae_result->iso;
        }
    }
}

static hi_u8 isp_ae_get_delay_max(isp_usr_ctx *isp_ctx)
{
    hi_u8 delay_max;

    if (is_online_mode(isp_ctx->block_attr.running_mode) || is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        delay_max = isp_ctx->linkage.cfg2valid_delay_max;
    } else {
        delay_max = isp_ctx->linkage.cfg2valid_delay_max + 1;
    }
    delay_max = CLIP3(delay_max, 1, ISP_SYNC_ISO_BUF_MAX - 1);

    return delay_max;
}

static hi_void ae_update_linkage(hi_vi_pipe vi_pipe, hi_isp_ae_result *ae_result, isp_linkage *linkage)
{
    hi_s32 i = 0;
    hi_u8 delay_max;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_u32 exp_ratio_tmp;

    isp_get_ctx(vi_pipe, isp_ctx);

    linkage->dgain           = ae_result->dgain;
    linkage->again           = ae_result->again;
    linkage->isp_dgain_shift = 8;  /* isp dgain shift 8 */

    for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
        linkage->sync_iso_buf[i] = linkage->sync_iso_buf[i - 1];
        linkage->sync_isp_dgain_buf[i] = linkage->sync_isp_dgain_buf[i - 1];
    }

    linkage->sync_iso_buf[0] = ae_result->iso;
    linkage->sync_isp_dgain_buf[0] = ae_result->isp_dgain;

    delay_max = isp_ae_get_delay_max(isp_ctx);

    if (linkage->update_pos == 0) {
        linkage->iso = MAX2(linkage->sync_iso_buf[delay_max], HI_AE_ISO_MIN);
        linkage->isp_dgain = MAX2(linkage->sync_isp_dgain_buf[delay_max], HI_AE_ISP_DGAIN_MIN);
    } else {
        linkage->iso = MAX2(linkage->sync_iso_buf[delay_max - 1], HI_AE_ISO_MIN);
        linkage->isp_dgain = MAX2(linkage->sync_isp_dgain_buf[delay_max - 1], HI_AE_ISP_DGAIN_MIN);
    }

    if (isp_ctx->linkage.run_once == HI_TRUE) {
        linkage->iso = MAX2(linkage->sync_iso_buf[0], HI_AE_ISO_MIN);
        linkage->isp_dgain = MAX2(linkage->sync_isp_dgain_buf[0], HI_AE_ISP_DGAIN_MIN);
    }

    isp_ctx->linkage.iso_done_frm_cnt = isp_frame_cnt_get(vi_pipe);

    linkage->sensor_iso = ((hi_u64)linkage->iso << 8) / div_0_to_1(linkage->isp_dgain); /* left shift 8 bit */
    linkage->sensor_iso = (linkage->sensor_iso < HI_AE_ISO_MIN) ? HI_AE_ISO_MIN : linkage->sensor_iso;
    linkage->int_time = ae_result->int_time[0] >> 4; /* right shift 4 bit */
    linkage->ae_run_interval = ae_result->ae_run_interval;

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        linkage->wdr_gain[i] = HI_AE_WDR_GAIN_MIN;
    }

    if (is_2to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        /* WDR exposure ratio is 6bit precision */
        linkage->int_time = ae_result->int_time[1] >> 4; /* right shift 4 bit */
        exp_ratio_tmp = (((hi_u64)ae_result->int_time[1] * ae_result->iso * ae_result->sns_lhcg_exp_ratio) +
            ((hi_u64)ae_result->int_time[0] * ae_result->iso_sf) / 2) /     /* div 2 */
            div_0_to_1((hi_u64)ae_result->int_time[0] * ae_result->iso_sf);

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            linkage->sync_all_exp_ratio_buf[i] = linkage->sync_all_exp_ratio_buf[i - 1];
            linkage->sync_exp_ratio_buf[0][i] = linkage->sync_exp_ratio_buf[0][i - 1];
        }

        linkage->sync_all_exp_ratio_buf[0] = exp_ratio_tmp;
        linkage->sync_exp_ratio_buf[0][0] = exp_ratio_tmp;

        if (linkage->update_pos == 0) {
            linkage->exp_ratio = linkage->sync_all_exp_ratio_buf[delay_max];
            linkage->exp_ratio_lut[0] = linkage->sync_exp_ratio_buf[0][delay_max];
        } else {
            linkage->exp_ratio = linkage->sync_all_exp_ratio_buf[delay_max - 1];
            linkage->exp_ratio_lut[0] = linkage->sync_exp_ratio_buf[0][delay_max - 1];
        }

        if (isp_ctx->linkage.run_once == HI_TRUE) {
            linkage->exp_ratio = linkage->sync_all_exp_ratio_buf[0];
            linkage->exp_ratio_lut[0] = linkage->sync_exp_ratio_buf[0][0];
        }

        hi_ext_system_actual_wdr_exposure_ratio_write(vi_pipe, 0, linkage->exp_ratio_lut[0]);
        hi_ext_system_actual_wdr_exposure_ratio_write(vi_pipe, 1, HI_AE_WDR_EXP_RATIO_MIN);
        hi_ext_system_actual_wdr_exposure_ratio_write(vi_pipe, 2, HI_AE_WDR_EXP_RATIO_MIN);     /* index 2 */

        linkage->wdr_gain[0] = ae_result->wdr_gain[0];
        linkage->wdr_gain[1] = ae_result->wdr_gain[1];
    } else {
        hi_ext_system_actual_wdr_exposure_ratio_write(vi_pipe, 0, HI_AE_WDR_EXP_RATIO_MIN);
        hi_ext_system_actual_wdr_exposure_ratio_write(vi_pipe, 1, HI_AE_WDR_EXP_RATIO_MIN);
        hi_ext_system_actual_wdr_exposure_ratio_write(vi_pipe, 2, HI_AE_WDR_EXP_RATIO_MIN);      /* index 2 */
    }

    if (ae_result->piris_valid == HI_TRUE) {
        linkage->piris_gain = ae_result->piris_gain;
    } else {
        linkage->piris_gain = 0;
    }

    linkage->pre_fswdr_mode = linkage->fswdr_mode;
    linkage->fswdr_mode     = ae_result->fswdr_mode;
}

static hi_void ae_update_tunnel_info(hi_vi_pipe vi_pipe, hi_isp_ae_info *ae_info)
{
    hi_s32 i;
    static hi_u32  tunnel_update_cnt = 0;
    static hi_u32  tunnel_wait_cnt = 0;
    static hi_bool tunnel_in_time = HI_FALSE;
    for (i = 0; i < TUNNEL_CLASS_MAX; i++) {
        ae_info->tunnel_info.roi[i].enable       = hi_ext_system_tunnel_enable_read(vi_pipe, i);
        ae_info->tunnel_info.roi[i].available    = hi_ext_system_tunnel_available_read(vi_pipe, i);
        ae_info->tunnel_info.roi[i].tunnel_area_ratio = hi_ext_system_tunnel_area_ratio_read(vi_pipe, i);
        ae_info->tunnel_info.roi[i].tunnel_exp_perf = hi_ext_system_tunnel_exp_perf_read(vi_pipe, i);
    }
    if (hi_ext_system_tunnel_update_read(vi_pipe)) {
        tunnel_update_cnt++;
        hi_ext_system_tunnel_update_write(vi_pipe, HI_FALSE);
    } else {
        tunnel_wait_cnt++;
    }
    if (tunnel_update_cnt) {
        if (tunnel_wait_cnt < 20) {  /* experience value 20 */
            tunnel_in_time = HI_TRUE;
        } else {
            tunnel_in_time = HI_FALSE;
        }
        tunnel_update_cnt = 0;
        tunnel_wait_cnt   = 0;
    } else if (tunnel_wait_cnt >= 20) {  /* experience value 20 */
        tunnel_in_time = HI_FALSE;
    }
    if (!tunnel_in_time) {
        for (i = 0; i < TUNNEL_CLASS_MAX; i++) {
            ae_info->tunnel_info.roi[i].available = HI_FALSE;
        }
    }
}
hi_s32 isp_ae_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value);
hi_s32 isp_ae_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 i;
    hi_isp_ae_param ae_param;
    hi_isp_ae_init_info ae_init_info = { 0 };
    isp_usr_ctx  *isp_ctx = HI_NULL;
    isp_lib_node *lib     = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    ae_regs_default(vi_pipe, (isp_reg_cfg *)reg_cfg);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    ae_param.sensor_id   = isp_ctx->bind_attr.sensor_id;
    ae_param.wdr_mode    = isp_ctx->sns_wdr_mode;
    ae_param.hdr_mode    = isp_ctx->hdr_attr.dynamic_range;
    ae_param.bayer       = hi_ext_system_rggb_cfg_read(vi_pipe);
    ae_param.fps         = isp_ctx->sns_image_mode.fps;
    ae_param.black_level = sns_black_level->black_level[1];

    ae_param.stitch_attr.main_pipe       = isp_ctx->stitch_attr.main_pipe;
    ae_param.stitch_attr.stitch_enable   = isp_ctx->stitch_attr.stitch_enable;
    ae_param.stitch_attr.stitch_pipe_num = isp_ctx->stitch_attr.stitch_pipe_num;
    memcpy_s(ae_param.stitch_attr.stitch_bind_id, sizeof(hi_s8) *
             VI_MAX_PIPE_NUM, isp_ctx->stitch_attr.stitch_bind_id, sizeof(hi_s8) * VI_MAX_PIPE_NUM);

    /* init all registered ae libs */
    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (isp_ctx->ae_lib_info.libs[i].used) {
            lib = &isp_ctx->ae_lib_info.libs[i];
            if (lib->ae_regsiter.ae_exp_func.pfn_ae_init != HI_NULL) {
                lib->ae_regsiter.ae_exp_func.pfn_ae_init(lib->alg_lib.id, &ae_param);
            }
            if (lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl != HI_NULL) {
                lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl(lib->alg_lib.id,
                    ISP_AE_INIT_INFO_GET, (hi_void *)&ae_init_info);
            }
        }
    }

    isp_ctx->linkage.isp_dgain = MAX2(ae_init_info.isp_dgain, 0x100);
    isp_ctx->linkage.iso = MAX2(ae_init_info.iso, HI_AE_ISO_MIN);
    isp_ctx->attach_info_ctrl.attach_info->init_iso = isp_ctx->linkage.iso;

    for (i = 0; i < ISP_SYNC_ISO_BUF_MAX; i++) {
        isp_ctx->linkage.sync_iso_buf[i]       = isp_ctx->linkage.iso;
        isp_ctx->linkage.sync_isp_dgain_buf[i] = isp_ctx->linkage.isp_dgain;
    }
    isp_ctx->linkage.sensor_iso = ((hi_u64)isp_ctx->linkage.iso << 8) / div_0_to_1(isp_ctx->linkage.isp_dgain); /* 8 */
    isp_ctx->linkage.sensor_iso = MAX2(isp_ctx->linkage.sensor_iso, HI_AE_ISO_MIN);
    return HI_SUCCESS;
}

hi_s32 isp_ae_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_input, hi_s32 rsv)
{
    hi_s32 i;
    hi_u8  bayer_format = 0;
    hi_s32 ret = HI_FAILURE;
    hi_u16 black_offset = 0;
    hi_u8  rc_enable = 0;
    static hi_u32 update_cnt = 0;
    static hi_u32 wait_cnt = 0;
    static hi_bool in_time = HI_FALSE;
    hi_isp_ae_info   ae_info    = {0};
    hi_isp_ae_result ae_result  = {{0}};

    isp_linkage      *linkage   = HI_NULL;
    isp_usr_ctx      *isp_ctx   = HI_NULL;
    isp_lib_node     *lib       = HI_NULL;
    isp_stat         *isp_stat_info = HI_NULL;
    isp_reg_cfg      *reg_cfg   = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    lib       = &isp_ctx->ae_lib_info.libs[isp_ctx->ae_lib_info.active_lib];
    reg_cfg    = (isp_reg_cfg *)reg_cfg_input;
    isp_stat_info = (isp_stat *)stat_info;
    linkage   = &isp_ctx->linkage;

    if (linkage->defect_pixel || linkage->snap_state) {
        return HI_SUCCESS;
    }

    if ((is_online_mode(isp_ctx->block_attr.running_mode) || is_sidebyside_mode(isp_ctx->block_attr.running_mode)) &&
        (isp_ctx->linkage.snap_pipe_mode == ISP_SNAP_PICTURE)) {
        isp_check_pipe_return(isp_ctx->linkage.preview_pipe_id);
        isp_ctx->linkage.iso_done_frm_cnt = isp_frame_cnt_get(isp_ctx->linkage.preview_pipe_id);
        return HI_SUCCESS;
    }

    if ((isp_ctx->ai_info.pq_ai_en == HI_TRUE) && (isp_ctx->ai_info.ai_pipe_id == vi_pipe)) {
        isp_check_pipe_return(isp_ctx->ai_info.ai_pipe_id);
        isp_ctx->linkage.iso_done_frm_cnt = isp_frame_cnt_get(isp_ctx->ai_info.ai_pipe_id);

        return HI_SUCCESS;
    }

    if (isp_ctx->linkage.stat_ready == HI_FALSE) {
        return HI_SUCCESS;
    }

    ae_info.frame_cnt = isp_ctx->frame_cnt;

    ae_info.fe_ae_stat1 = &isp_stat_info->fe_ae_stat1;
    ae_info.fe_ae_stat2 = &isp_stat_info->fe_ae_stat2;
    ae_info.fe_ae_stat3 = &isp_stat_info->fe_ae_stat3;
    ae_info.be_ae_stat1 = &isp_stat_info->be_ae_stat1;
    ae_info.be_ae_stat2 = &isp_stat_info->be_ae_stat2;
    ae_info.be_ae_stat3 = &isp_stat_info->be_ae_stat3;

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        ae_info.smart_info.roi[i].enable     = hi_ext_system_smart_enable_read(vi_pipe, i);
        ae_info.smart_info.roi[i].luma       = hi_ext_system_smart_luma_read(vi_pipe, i);
        ae_info.smart_info.roi[i].available  = hi_ext_system_smart_available_read(vi_pipe, i);
    }

    if (hi_ext_system_smart_update_read(vi_pipe)) {
        update_cnt++;
        hi_ext_system_smart_update_write(vi_pipe, HI_FALSE);
    } else {
        wait_cnt++;
    }

    if (update_cnt) {
        if (wait_cnt < 20) {  /* experience value 20 */
            in_time = HI_TRUE;
        } else {
            in_time = HI_FALSE;
        }
        update_cnt = 0;
        wait_cnt   = 0;
    } else if (wait_cnt >= 20) {  /* experience value 20 */
        in_time = HI_FALSE;
    }

    if (!in_time) {
        for (i = 0; i < SMART_CLASS_MAX; i++) {
            ae_info.smart_info.roi[i].available = HI_FALSE;
        }
    }

    ae_update_tunnel_info(vi_pipe, &ae_info);
    if (lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl != HI_NULL) {
        bayer_format = hi_ext_system_rggb_cfg_read(vi_pipe);
        lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl(lib->alg_lib.id,
                                                 ISP_AE_BAYER_FORMAT_SET, (hi_void *)&bayer_format);
        black_offset = hi_ext_system_black_level_query_00_read(vi_pipe);
        lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl(lib->alg_lib.id,
                                                 ISP_AE_BLC_SET, (hi_void *)&black_offset);
        rc_enable = hi_ext_system_rc_en_read(vi_pipe);
        lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl(lib->alg_lib.id,
                                                 ISP_AE_RC_SET, (hi_void *)&rc_enable);
    }
    if (lib->ae_regsiter.ae_exp_func.pfn_ae_run != HI_NULL) {
        ret = lib->ae_regsiter.ae_exp_func.pfn_ae_run(lib->alg_lib.id, &ae_info, &ae_result, 0);
        if (ret != HI_SUCCESS) {
            isp_err_trace("WARNING!! ISP[%d] run ae lib err!\n", vi_pipe);
        }
    }

    if (ae_result.again_sf == 0 || ae_result.dgain_sf == 0 || ae_result.isp_dgain_sf == 0 || ae_result.iso_sf == 0) {
        ae_result.again_sf = ae_result.again;
        ae_result.dgain_sf = ae_result.dgain;
        ae_result.isp_dgain_sf = ae_result.isp_dgain;
        ae_result.iso_sf = ae_result.iso;
    }
    ae_result.sns_lhcg_exp_ratio = (ae_result.sns_lhcg_exp_ratio == 0) ? 
                                   HI_AE_WDR_EXP_RATIO_MIN : ae_result.sns_lhcg_exp_ratio;

    hi_ext_system_sys_iso_write(vi_pipe, ae_result.iso);

    ae_read_extregs(vi_pipe, &ae_result);
    ae_res_read_extregs(vi_pipe, reg_cfg);
    ae_regs_range_check(vi_pipe, &ae_result);
    ae_update_config(vi_pipe, &ae_result, reg_cfg);
    ae_update_linkage(vi_pipe, &ae_result, linkage);
    if (isp_ctx->linkage.snap_pipe_mode != ISP_SNAP_PICTURE) {
        ae_get_frame_info(vi_pipe, &ae_result);
        ae_get_dcf_info(vi_pipe, &ae_result);
    }

    return ret;
}

hi_s32 isp_ae_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    hi_s32 i;
    hi_s32 ret = HI_FAILURE;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lib_node *lib = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    lib = &isp_ctx->ae_lib_info.libs[isp_ctx->ae_lib_info.active_lib];

    if (cmd == ISP_PROC_WRITE) {
        if (lib->used) {
            if (lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl != HI_NULL) {
                ret = lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl(lib->alg_lib.id, cmd, value);
            }
        }
    } else {
        for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
            if (isp_ctx->ae_lib_info.libs[i].used) {
                lib = &isp_ctx->ae_lib_info.libs[i];
                ret = (lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl != HI_NULL ?
                       lib->ae_regsiter.ae_exp_func.pfn_ae_ctrl(lib->alg_lib.id, cmd, value) : HI_FAILURE);
            }
        }
    }

    if (cmd == ISP_WDR_MODE_SET) {
        ae_regs_default(vi_pipe, &reg_cfg->reg_cfg);
    }

    if (cmd == ISP_CHANGE_IMAGE_MODE_SET) {
        ae_regs_default(vi_pipe, &reg_cfg->reg_cfg);
    }

    return ret;
}

hi_s32 isp_ae_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lib_node *lib = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (isp_ctx->ae_lib_info.libs[i].used) {
            lib = &isp_ctx->ae_lib_info.libs[i];
            if (lib->ae_regsiter.ae_exp_func.pfn_ae_exit != HI_NULL) {
                lib->ae_regsiter.ae_exp_func.pfn_ae_exit(lib->alg_lib.id);
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_ae(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_alg_check_return(isp_ctx->alg_key.bit1_ae);

    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_AE;
    algs->alg_func.pfn_alg_init = isp_ae_init;
    algs->alg_func.pfn_alg_run  = isp_ae_run;
    algs->alg_func.pfn_alg_ctrl = isp_ae_ctrl;
    algs->alg_func.pfn_alg_exit = isp_ae_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

