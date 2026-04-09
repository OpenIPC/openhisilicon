/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_awb.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "hi_math.h"

#define ISP_AWB_GAIN_MAX_THR            0xFFF
#define ISP_AWB_STAT_CR_CB_MAX_THR      0xFFF
#define ISP_AWB_STAT_Y_MAX_THR          0xFFFF
#define ISP_CCM_NORM_VALUE              0x100
#define isp_awb_rshift8(x)    (((x) + 0x80) / 0x100)
#define MESH_SHADING_COR_ROW            2

static hi_u32 g_d50_wb_gain_recomm[ISP_BAYER_CHN_NUM] = {0x1ED, 0x100, 0x100, 0x1AB};
static hi_u16 g_identity_matrix[CCM_MATRIX_SIZE] = {0x100, 0x0, 0x0, 0x0, 0x100, 0x0, 0x0, 0x0, 0x100};

typedef struct {
    hi_u8 crop_en;
    hi_u16 overlap;
    hi_u32 crop_x;
    hi_u32 crop_y;
    hi_u32 crop_height;
    hi_u32 crop_width;
} isp_awb_crop_info;

typedef struct {
    hi_u16 blend_ratio;
    hi_u16 strength;
    hi_u16 scale;
    hi_u16 index;
    hi_u16 gain_shift;
    hi_u16 mesh_lut_r[MESH_SHADING_COR_ROW][HI_ISP_LSC_GRID_COL];
    hi_u16 mesh_lut_g[MESH_SHADING_COR_ROW][HI_ISP_LSC_GRID_COL];
    hi_u16 mesh_lut_b[MESH_SHADING_COR_ROW][HI_ISP_LSC_GRID_COL];
} isp_awb_lsc_lut;

typedef struct {
    hi_isp_awb_stat_1 awb_glob_stat;
    hi_isp_awb_stat_2 awb_zone_stat;
} isp_awb_stat_info;


isp_awb_stat_info *g_awb_stat_info[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define awb_get_stat_info(dev, stat)   ((stat) = g_awb_stat_info[dev])
#define awb_set_stat_info(dev, stat)   (g_awb_stat_info[dev] = (stat))
#define awb_reset_stat_info(dev)       (g_awb_stat_info[dev] = HI_NULL)

static hi_s32 awb_stat_info_init(hi_vi_pipe vi_pipe)
{
    isp_awb_stat_info *awb_stat = HI_NULL;

    awb_get_stat_info(vi_pipe, awb_stat);

    if (awb_stat == HI_NULL) {
        awb_stat = (isp_awb_stat_info *)isp_malloc(sizeof(isp_awb_stat_info));
        if (awb_stat == HI_NULL) {
            isp_err_trace("Isp[%d] awb stat malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }
    memset_s(awb_stat, sizeof(isp_awb_stat_info), 0, sizeof(isp_awb_stat_info));

    awb_set_stat_info(vi_pipe, awb_stat);

    return HI_SUCCESS;
}

static hi_void awb_stat_info_exit(hi_vi_pipe vi_pipe)
{
    isp_awb_stat_info *awb_stat = HI_NULL;

    awb_get_stat_info(vi_pipe, awb_stat);
    isp_free(awb_stat);
    awb_reset_stat_info(vi_pipe);
}

/* multiply the two matrixes a1[dim1 x dim2] a2[dim2 x dim3] */
static hi_void awb_matrix_multiply(hi_s16 *matrix1, hi_s16 *matrix2, hi_s16 *result, hi_s32 dim)
{
    hi_s32 i, j, k;
    hi_s32 temp;

    for (i = 0; i < dim; ++i) {
        for (j = 0; j < dim; ++j) {
            temp = 0;

            for (k = 0; k < dim; ++k) {
                temp += (((hi_s32)matrix1[i * dim + k] * matrix2[k * dim + j]));
            }

            result[i * dim + j] = (hi_s16)isp_awb_rshift8(temp);
        }
    }

    for (i = 0; i < dim; ++i) {
        temp = 0;

        for (j = 0; j < dim; ++j) {
            temp += (hi_s32)result[i * dim + j];
        }

        if (temp != 0x0) {
            for (j = 0; j < dim; ++j) {
                result[i * dim + j] = (hi_s16)(result[i * dim + j] * ISP_CCM_NORM_VALUE / div_0_to_1(temp));
            }
        }
    }

    for (i = 0; i < dim; ++i) {
        temp = 0;

        for (j = 0; j < dim; ++j) {
            temp += (hi_s16)result[i * dim + j];
        }

        if (temp != ISP_CCM_NORM_VALUE) {
            result[i * dim + i] += (ISP_CCM_NORM_VALUE - temp);
        }
    }
}

static hi_void awb_res_regs_default(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 block_num, isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg)
{
    hi_u16 overlap;
    isp_rect block_rect;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    /* BEDYNAMIC */
    isp_get_block_rect(&block_rect, &isp_ctx->block_attr, i);
    awb_reg_dyn_cfg->be_width  = block_rect.width;
    awb_reg_dyn_cfg->be_height = block_rect.height;

    overlap = isp_ctx->block_attr.over_lap;

    /* awb overlap configs */
    if (i == 0) {
        if (block_num > 1) {
            awb_reg_dyn_cfg->be_crop_pos_x      = 0;
            awb_reg_dyn_cfg->be_crop_pos_y      = 0;
            awb_reg_dyn_cfg->be_crop_out_width  = awb_reg_dyn_cfg->be_width - overlap;
            awb_reg_dyn_cfg->be_crop_out_height = awb_reg_dyn_cfg->be_height;
        } else {
            awb_reg_dyn_cfg->be_crop_pos_x      = 0;
            awb_reg_dyn_cfg->be_crop_pos_y      = 0;
            awb_reg_dyn_cfg->be_crop_out_height = block_rect.height;
            awb_reg_dyn_cfg->be_crop_out_width  = block_rect.width;
        }
    } else if (i == (block_num - 1)) {
        awb_reg_dyn_cfg->be_crop_pos_x      = overlap;
        awb_reg_dyn_cfg->be_crop_pos_y      = 0;
        awb_reg_dyn_cfg->be_crop_out_width  = awb_reg_dyn_cfg->be_width - overlap;
        awb_reg_dyn_cfg->be_crop_out_height = awb_reg_dyn_cfg->be_height;
    } else {
        awb_reg_dyn_cfg->be_crop_pos_x      = overlap;
        awb_reg_dyn_cfg->be_crop_pos_y      = 0;
        awb_reg_dyn_cfg->be_crop_out_width  = awb_reg_dyn_cfg->be_width - (overlap << 1);
        awb_reg_dyn_cfg->be_crop_out_height = awb_reg_dyn_cfg->be_height;
    }

    hi_ext_system_awb_crop_en_write(vi_pipe, HI_EXT_SYSTEM_AWB_CROP_EN_DEFAULT);
    hi_ext_system_awb_crop_x_write(vi_pipe, 0);
    hi_ext_system_awb_crop_y_write(vi_pipe, 0);
    hi_ext_system_awb_crop_height_write(vi_pipe, isp_ctx->block_attr.frame_rect.height);
    hi_ext_system_awb_crop_width_write(vi_pipe, isp_ctx->block_attr.frame_rect.width);
}

static hi_void awb_res_read_ext_crop_regs(hi_u8 i, hi_u8 block_num, isp_awb_crop_info *awb_crop, \
                                          isp_rect *block_rect, isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg)
{
    if (i == 0) {
        if (block_num > 1) {
            awb_reg_dyn_cfg->be_crop_pos_x = 0;
            awb_reg_dyn_cfg->be_crop_pos_y = 0;
            awb_reg_dyn_cfg->be_crop_out_height = block_rect->height;
            awb_reg_dyn_cfg->be_crop_out_width = block_rect->width - awb_crop->overlap;
        } else {
            if (awb_crop->crop_en) {
                awb_reg_dyn_cfg->be_crop_pos_x = awb_crop->crop_x;
                awb_reg_dyn_cfg->be_crop_pos_y = awb_crop->crop_y;
                awb_reg_dyn_cfg->be_crop_out_height = awb_crop->crop_height;
                awb_reg_dyn_cfg->be_crop_out_width = awb_crop->crop_width;
            } else {
                awb_reg_dyn_cfg->be_crop_pos_x = 0;
                awb_reg_dyn_cfg->be_crop_pos_y = 0;
                awb_reg_dyn_cfg->be_crop_out_height = block_rect->height;
                awb_reg_dyn_cfg->be_crop_out_width = block_rect->width;
            }
        }
    } else if (i == (block_num - 1)) {
        awb_reg_dyn_cfg->be_crop_pos_x = awb_crop->overlap;
        awb_reg_dyn_cfg->be_crop_pos_y = 0;
        awb_reg_dyn_cfg->be_crop_out_height = block_rect->height;
        awb_reg_dyn_cfg->be_crop_out_width = block_rect->width - awb_crop->overlap;
    } else {
        awb_reg_dyn_cfg->be_crop_pos_x = awb_crop->overlap;
        awb_reg_dyn_cfg->be_crop_pos_y = 0;
        awb_reg_dyn_cfg->be_crop_out_height = block_rect->height;
        awb_reg_dyn_cfg->be_crop_out_width = block_rect->width - (awb_crop->overlap << 1);
    }
}

static hi_void awb_res_read_ext_regs(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i, awb_zone_row, awb_zone_col, block_num;
    isp_rect block_rect;
    isp_awb_crop_info awb_crop;

    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg = HI_NULL;
    block_num = ((isp_reg_cfg *)reg_cfg)->cfg_num;
    isp_get_ctx(vi_pipe, isp_ctx);

    block_num = isp_ctx->block_attr.block_num;

    awb_crop.overlap = isp_ctx->block_attr.over_lap;
    awb_crop.crop_en = hi_ext_system_awb_crop_en_read(vi_pipe);
    awb_crop.crop_x  = (hi_ext_system_awb_crop_x_read(vi_pipe) >> 1) << 1;
    awb_crop.crop_y  = (hi_ext_system_awb_crop_y_read(vi_pipe) >> 1) << 1;
    awb_crop.crop_height = (hi_ext_system_awb_crop_height_read(vi_pipe) >> 1) << 1;
    awb_crop.crop_width = (hi_ext_system_awb_crop_width_read(vi_pipe) >> 1) << 1;

    awb_zone_row = hi_ext_system_awb_vnum_read(vi_pipe);
    awb_zone_col = hi_ext_system_awb_hnum_read(vi_pipe);

    awb_crop.crop_height = CLIP3(awb_crop.crop_height, awb_zone_row * AWB_MIN_HEIGHT, \
                                 isp_ctx->block_attr.frame_rect.height);
    awb_crop.crop_width = CLIP3(awb_crop.crop_width, awb_zone_col * AWB_MIN_WIDTH, \
                                isp_ctx->block_attr.frame_rect.width);
    awb_crop.crop_y = MIN2(awb_crop.crop_y, (isp_ctx->block_attr.frame_rect.height - awb_crop.crop_height));
    awb_crop.crop_x = MIN2(awb_crop.crop_x, (isp_ctx->block_attr.frame_rect.width - awb_crop.crop_width));

    /* AWB BE configs */
    for (i = 0; i < block_num; i++) {
        /* AWB configs */
        awb_reg_dyn_cfg = &reg_cfg->alg_reg_cfg[i].awb_reg_cfg.awb_reg_dyn_cfg;

        /* AWB size configs */
        isp_get_block_rect(&block_rect, &isp_ctx->block_attr, i);

        /* AWB overlap configs */
        awb_res_read_ext_crop_regs(i, block_num, &awb_crop, &block_rect, awb_reg_dyn_cfg);
    }
    reg_cfg->cfg_key.bit1_awb_dyn_cfg = 1;
    reg_cfg->kernel_reg_cfg.cfg_key.bit1_awb_cfg = 1;
}

static hi_void awb_image_mode_set(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i;
    hi_u8  awb_zone_col, awb_zone_row;
    hi_u16 height, width;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_awb_reg_usr_cfg *awb_reg_usr_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    height = isp_ctx->block_attr.frame_rect.height;
    width = isp_ctx->block_attr.frame_rect.width;
    awb_zone_col = hi_ext_system_awb_hnum_read(vi_pipe);
    awb_zone_row = hi_ext_system_awb_vnum_read(vi_pipe);

    if (awb_zone_col * AWB_MIN_WIDTH > width) {
        awb_zone_col = width / AWB_MIN_WIDTH;
    }

    if (awb_zone_row * AWB_MIN_HEIGHT > height) {
        awb_zone_row = height / AWB_MIN_HEIGHT;
    }

    awb_zone_col = CLIP3(awb_zone_col, isp_ctx->block_attr.block_num, AWB_ZONE_ORIG_COLUMN);
    awb_zone_row = CLIP3(awb_zone_row, 1, AWB_ZONE_ORIG_ROW);

    hi_ext_system_awb_hnum_write(vi_pipe, awb_zone_col);
    hi_ext_system_awb_vnum_write(vi_pipe, awb_zone_row);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        awb_reg_usr_cfg = &reg_cfg->alg_reg_cfg[i].awb_reg_cfg.awb_reg_usr_cfg;

        awb_res_regs_default(vi_pipe, i, reg_cfg->cfg_num, &reg_cfg->alg_reg_cfg[i].awb_reg_cfg.awb_reg_dyn_cfg);

        if (i < awb_zone_col % div_0_to_1(reg_cfg->cfg_num)) {
            awb_reg_usr_cfg->be_zone_col = awb_zone_col / div_0_to_1(reg_cfg->cfg_num) + 1;
        } else {
            awb_reg_usr_cfg->be_zone_col = awb_zone_col / div_0_to_1(reg_cfg->cfg_num);
        }

        awb_reg_usr_cfg->be_zone_row = awb_zone_row;

        awb_reg_usr_cfg->resh = HI_TRUE;
        awb_reg_usr_cfg->update_index += 1;
    }

    reg_cfg->cfg_key.bit1_awb_dyn_cfg = 1;
}

static hi_void awb_regs_default(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, hi_u8 awb_zone_row, hi_u8 awb_zone_col, \
                                hi_u8 awb_zone_bin)
{
    hi_u8 i, j;
    hi_u8 block_num;
    isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg = HI_NULL;
    isp_awb_reg_sta_cfg *awb_reg_sta_cfg = HI_NULL;
    isp_awb_reg_usr_cfg *awb_reg_usr_cfg = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    block_num = isp_ctx->block_attr.block_num;

    awb_reg_dyn_cfg = &reg_cfg->alg_reg_cfg[0].awb_reg_cfg.awb_reg_dyn_cfg; /* dynamic */
    awb_reg_sta_cfg = &reg_cfg->alg_reg_cfg[0].awb_reg_cfg.awb_reg_sta_cfg; /* static */

    /* FE DYNAMIC */
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        awb_reg_dyn_cfg->fe_white_balance_gain[i] = g_d50_wb_gain_recomm[i];
    }

    awb_reg_dyn_cfg->fe_wb_work_en = HI_TRUE;

    /* FE STATIC */
    awb_reg_sta_cfg->fe_awb_sta_cfg = 1;

    awb_reg_sta_cfg->fe_clip_value = 0xFFFFF;

    reg_cfg->cfg_key.bit1_awb_dyn_cfg = 1;

    for (i = 0; i < block_num; i++) {
        awb_reg_sta_cfg = &(reg_cfg->alg_reg_cfg[i].awb_reg_cfg.awb_reg_sta_cfg); /* static */
        awb_reg_dyn_cfg = &(reg_cfg->alg_reg_cfg[i].awb_reg_cfg.awb_reg_dyn_cfg); /* dynamic */
        awb_reg_usr_cfg = &(reg_cfg->alg_reg_cfg[i].awb_reg_cfg.awb_reg_usr_cfg); /* user */

        /* BE STATIC */
        awb_reg_sta_cfg->be_awb_sta_cfg = 1;
        awb_reg_sta_cfg->be_awb_bitmove = 0x0;
        awb_reg_sta_cfg->be_wb_work_en = HI_TRUE;
        awb_reg_sta_cfg->be_awb_work_en = HI_TRUE;

        awb_reg_sta_cfg->be_awb_stat_raddr = 0x000;
        awb_reg_sta_cfg->be_cc_colortone_en = 0x0;

        awb_reg_sta_cfg->be_top_k = 0x0;
        awb_reg_sta_cfg->be_top_b = 0xFFFFF;
        awb_reg_sta_cfg->be_bot_k = 0x0;
        awb_reg_sta_cfg->be_bot_b = 0x0;

        awb_reg_sta_cfg->be_cc_in_dc0 = 0x0;
        awb_reg_sta_cfg->be_cc_in_dc1 = 0x0;
        awb_reg_sta_cfg->be_cc_in_dc2 = 0x0;
        awb_reg_sta_cfg->be_cc_out_dc0 = 0x0;
        awb_reg_sta_cfg->be_cc_out_dc1 = 0x0;
        awb_reg_sta_cfg->be_cc_out_dc2 = 0x0;
        awb_reg_sta_cfg->be_wb_clip_value = 0xFFFFF;
        awb_reg_sta_cfg->be_awb_offset_comp = HI_ISP_AWB_OFFSET_COMP_DEF;

        awb_res_regs_default(vi_pipe, i, block_num, awb_reg_dyn_cfg);

        if (i < awb_zone_col % div_0_to_1(block_num)) {
            awb_reg_usr_cfg->be_zone_col = awb_zone_col / div_0_to_1(block_num) + 1;
        } else {
            awb_reg_usr_cfg->be_zone_col = awb_zone_col / div_0_to_1(block_num);
        }

        for (j = 0; j < CCM_MATRIX_SIZE; j++) {
            awb_reg_dyn_cfg->be_color_matrix[j] = g_identity_matrix[j];
        }

        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            awb_reg_dyn_cfg->be_white_balance_gain[j] = g_d50_wb_gain_recomm[j];
        }

        awb_reg_dyn_cfg->be_metering_white_level_awb = ISP_AWB_STAT_Y_MAX_THR;
        awb_reg_dyn_cfg->be_metering_black_level_awb = 0x0;
        awb_reg_dyn_cfg->be_metering_cr_ref_max_awb = 0x180;
        awb_reg_dyn_cfg->be_metering_cr_ref_min_awb = 0x40;
        awb_reg_dyn_cfg->be_metering_cb_ref_max_awb = 0x180;
        awb_reg_dyn_cfg->be_metering_cb_ref_min_awb = 0x40;

        awb_reg_dyn_cfg->be_wb_work_en = HI_TRUE;
        awb_reg_dyn_cfg->be_cc_en = HI_TRUE;

        awb_reg_dyn_cfg->be_cc_colortone_en = HI_ISP_CCM_COLORTONE_EN_DEFAULT;
        awb_reg_dyn_cfg->be_cc_r_gain = HI_ISP_CCM_COLORTONE_RGAIN_DEFAULT;
        awb_reg_dyn_cfg->be_cc_g_gain = HI_ISP_CCM_COLORTONE_GGAIN_DEFAULT;
        awb_reg_dyn_cfg->be_cc_b_gain = HI_ISP_CCM_COLORTONE_BGAIN_DEFAULT;

        awb_reg_usr_cfg->be_zone_row = awb_zone_row;
        awb_reg_usr_cfg->be_zone_bin = awb_zone_bin;
        awb_reg_usr_cfg->be_metering_bin_hist0 = ISP_AWB_STAT_Y_MAX_THR;
        awb_reg_usr_cfg->be_metering_bin_hist1 = ISP_AWB_STAT_Y_MAX_THR;
        awb_reg_usr_cfg->be_metering_bin_hist2 = ISP_AWB_STAT_Y_MAX_THR;
        awb_reg_usr_cfg->be_metering_bin_hist3 = ISP_AWB_STAT_Y_MAX_THR;
        awb_reg_usr_cfg->be_awb_switch = ISP_AWB_AFTER_DG;
        awb_reg_usr_cfg->resh = HI_TRUE;
        awb_reg_usr_cfg->update_index = 1;

        reg_cfg->kernel_reg_cfg.cfg_key.bit1_awb_cfg = 1;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_awb_zone_cfg.row = awb_zone_row;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_awb_zone_cfg.colum = awb_zone_col;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_awb_zone_cfg.zone_bin = awb_zone_bin;

        hi_ext_system_cc_enable_write(vi_pipe, awb_reg_dyn_cfg->be_cc_en);
        hi_ext_system_awb_gain_enable_write(vi_pipe, awb_reg_dyn_cfg->be_wb_work_en);
        hi_ext_system_awb_white_level_write(vi_pipe, awb_reg_dyn_cfg->be_metering_white_level_awb);
        hi_ext_system_awb_black_level_write(vi_pipe, awb_reg_dyn_cfg->be_metering_black_level_awb);
        hi_ext_system_awb_cr_ref_max_write(vi_pipe, awb_reg_dyn_cfg->be_metering_cr_ref_max_awb);
        hi_ext_system_awb_cr_ref_min_write(vi_pipe, awb_reg_dyn_cfg->be_metering_cr_ref_min_awb);
        hi_ext_system_awb_cb_ref_max_write(vi_pipe, awb_reg_dyn_cfg->be_metering_cb_ref_max_awb);
        hi_ext_system_awb_cb_ref_min_write(vi_pipe, awb_reg_dyn_cfg->be_metering_cb_ref_min_awb);
        hi_ext_system_cc_colortone_rgain_write(vi_pipe, awb_reg_dyn_cfg->be_cc_r_gain);
        hi_ext_system_cc_colortone_ggain_write(vi_pipe, awb_reg_dyn_cfg->be_cc_g_gain);
        hi_ext_system_cc_colortone_bgain_write(vi_pipe, awb_reg_dyn_cfg->be_cc_b_gain);

        /* user */
        hi_ext_system_awb_sta_enable_write(vi_pipe, awb_reg_sta_cfg->be_awb_work_en);
        hi_ext_system_wb_statistics_mpi_update_en_write(vi_pipe, awb_reg_usr_cfg->resh);
        hi_ext_system_awb_switch_write(vi_pipe, awb_reg_usr_cfg->be_awb_switch);
        hi_ext_system_awb_hnum_write(vi_pipe, awb_zone_col); /* the col num of the whole picture */
        hi_ext_system_awb_vnum_write(vi_pipe, awb_reg_usr_cfg->be_zone_row);
        hi_ext_system_awb_zone_bin_write(vi_pipe, awb_reg_usr_cfg->be_zone_bin);
        hi_ext_system_awb_hist_bin_thresh0_write(vi_pipe, awb_reg_usr_cfg->be_metering_bin_hist0);
        hi_ext_system_awb_hist_bin_thresh1_write(vi_pipe, awb_reg_usr_cfg->be_metering_bin_hist1);
        hi_ext_system_awb_hist_bin_thresh2_write(vi_pipe, awb_reg_usr_cfg->be_metering_bin_hist2);
        hi_ext_system_awb_hist_bin_thresh3_write(vi_pipe, awb_reg_usr_cfg->be_metering_bin_hist3);
    }
}

static hi_void awb_cfg_regs(hi_vi_pipe vi_pipe, hi_isp_awb_result *awb_result, isp_reg_cfg *reg_cfg)
{
    hi_bool usr_resh;
    hi_s32 i, k;
    hi_u32 wb_gain[ISP_BAYER_CHN_NUM];
    hi_u8 blk_num;
    hi_u8 awb_zone_col;
    isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg = HI_NULL;
    isp_awb_reg_usr_cfg *awb_reg_usr_cfg = HI_NULL;
    isp_awb_reg_sta_cfg *awb_reg_sta_cfg = HI_NULL;
    blk_num = ((isp_reg_cfg *)reg_cfg)->cfg_num;

    usr_resh = hi_ext_system_wb_statistics_mpi_update_en_read(vi_pipe);
    hi_ext_system_wb_statistics_mpi_update_en_write(vi_pipe, HI_FALSE);

    if (awb_result->raw_stat_attr.stat_cfg_update) { /* CbCr */
        hi_ext_system_awb_cr_ref_max_write(vi_pipe, \
            MIN2(awb_result->raw_stat_attr.metering_cr_ref_max_awb, ISP_AWB_STAT_CR_CB_MAX_THR));
        hi_ext_system_awb_cr_ref_min_write(vi_pipe, \
            MIN2(awb_result->raw_stat_attr.metering_cr_ref_min_awb, ISP_AWB_STAT_CR_CB_MAX_THR));
        hi_ext_system_awb_cb_ref_max_write(vi_pipe, \
            MIN2(awb_result->raw_stat_attr.metering_cb_ref_max_awb, ISP_AWB_STAT_CR_CB_MAX_THR));
        hi_ext_system_awb_cb_ref_min_write(vi_pipe, \
            MIN2(awb_result->raw_stat_attr.metering_cb_ref_min_awb, ISP_AWB_STAT_CR_CB_MAX_THR));
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        wb_gain[i] = awb_result->white_balance_gain[i];
        wb_gain[i] = isp_awb_rshift8(wb_gain[i]);
        wb_gain[i] = MIN2(wb_gain[i], ISP_AWB_GAIN_MAX_THR);
    }

    for (k = 0; k < blk_num; k++) {
        awb_reg_dyn_cfg = &(reg_cfg->alg_reg_cfg[k].awb_reg_cfg.awb_reg_dyn_cfg); /* dynamic */
        awb_reg_usr_cfg = &(reg_cfg->alg_reg_cfg[k].awb_reg_cfg.awb_reg_usr_cfg); /* user */
        awb_reg_sta_cfg = &(reg_cfg->alg_reg_cfg[k].awb_reg_cfg.awb_reg_sta_cfg); /* sta */

        awb_reg_dyn_cfg->be_metering_cr_ref_max_awb = hi_ext_system_awb_cr_ref_max_read(vi_pipe);
        awb_reg_dyn_cfg->be_metering_cr_ref_min_awb = hi_ext_system_awb_cr_ref_min_read(vi_pipe);
        awb_reg_dyn_cfg->be_metering_cb_ref_max_awb = hi_ext_system_awb_cb_ref_max_read(vi_pipe);
        awb_reg_dyn_cfg->be_metering_cb_ref_min_awb = hi_ext_system_awb_cb_ref_min_read(vi_pipe);
        awb_reg_dyn_cfg->be_metering_white_level_awb = hi_ext_system_awb_white_level_read(vi_pipe);
        awb_reg_dyn_cfg->be_metering_black_level_awb = hi_ext_system_awb_black_level_read(vi_pipe);

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            awb_reg_dyn_cfg->be_color_matrix[i] = awb_result->color_matrix[i];
        }

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            awb_reg_dyn_cfg->be_white_balance_gain[i] = wb_gain[i];
        }

        awb_zone_col = hi_ext_system_awb_hnum_read(vi_pipe);

        if (usr_resh == HI_TRUE) {
            if (k < awb_zone_col % div_0_to_1(blk_num)) {
                awb_reg_usr_cfg->be_zone_col = awb_zone_col / div_0_to_1(blk_num) + 1;
            } else {
                awb_reg_usr_cfg->be_zone_col = awb_zone_col / div_0_to_1(blk_num);
            }

            awb_reg_usr_cfg->update_index += 1;

            awb_reg_usr_cfg->be_awb_switch = hi_ext_system_awb_switch_read(vi_pipe);
            awb_reg_usr_cfg->be_zone_row = hi_ext_system_awb_vnum_read(vi_pipe);
            awb_reg_usr_cfg->be_zone_bin = hi_ext_system_awb_zone_bin_read(vi_pipe);

            awb_reg_usr_cfg->be_metering_bin_hist0 = hi_ext_system_awb_hist_bin_thresh0_read(vi_pipe);
            awb_reg_usr_cfg->be_metering_bin_hist1 = hi_ext_system_awb_hist_bin_thresh1_read(vi_pipe);
            awb_reg_usr_cfg->be_metering_bin_hist2 = hi_ext_system_awb_hist_bin_thresh2_read(vi_pipe);
            awb_reg_usr_cfg->be_metering_bin_hist3 = hi_ext_system_awb_hist_bin_thresh3_read(vi_pipe);
            awb_reg_usr_cfg->resh = HI_TRUE;
        }

        awb_reg_dyn_cfg->be_cc_en = hi_ext_system_cc_enable_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_b_gain = hi_ext_system_cc_colortone_bgain_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_g_gain = hi_ext_system_cc_colortone_ggain_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_r_gain = hi_ext_system_cc_colortone_rgain_read(vi_pipe);

        awb_reg_dyn_cfg->be_wb_work_en = hi_ext_system_awb_gain_enable_read(vi_pipe);
        awb_reg_dyn_cfg->fe_wb_work_en = hi_ext_system_awb_gain_enable_read(vi_pipe);
        awb_reg_sta_cfg->be_awb_work_en = hi_ext_system_awb_sta_enable_read(vi_pipe);

        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[k].be_awb_zone_cfg.row = awb_reg_usr_cfg->be_zone_row;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[k].be_awb_zone_cfg.colum = awb_zone_col;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[k].be_awb_zone_cfg.zone_bin = awb_reg_usr_cfg->be_zone_bin;
    }

    /* FE */
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        reg_cfg->alg_reg_cfg[0].awb_reg_cfg.awb_reg_dyn_cfg.fe_white_balance_gain[i] = wb_gain[i];
    }

    reg_cfg->cfg_key.bit1_awb_dyn_cfg = 1;
    reg_cfg->kernel_reg_cfg.cfg_key.bit1_awb_cfg = 1;
}

static hi_void awb_cfg_init_regs(hi_vi_pipe vi_pipe, hi_isp_awb_result *awb_result, isp_reg_cfg *reg_cfg)
{
    hi_s32 i, k;
    hi_u32 wb_gain[ISP_BAYER_CHN_NUM];
    hi_u8 blk_num;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg = HI_NULL;
    blk_num = ((isp_reg_cfg *)reg_cfg)->cfg_num;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        wb_gain[i] = awb_result->white_balance_gain[i];
        wb_gain[i] = isp_awb_rshift8(wb_gain[i]);
        wb_gain[i] = MIN2(wb_gain[i], ISP_AWB_GAIN_MAX_THR);
        reg_cfg->alg_reg_cfg[0].awb_reg_cfg.awb_reg_dyn_cfg.fe_white_balance_gain[i] = wb_gain[i];
    }

    for (k = 0; k < blk_num; k++) {
        awb_reg_dyn_cfg = &(reg_cfg->alg_reg_cfg[k].awb_reg_cfg.awb_reg_dyn_cfg); /* dynamic */

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            awb_reg_dyn_cfg->be_color_matrix[i] = awb_result->color_matrix[i];
        }

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            awb_reg_dyn_cfg->be_white_balance_gain[i] = wb_gain[i];
        }
    }

    /* init linkage */
    for (k = 0; k < ISP_SYNC_ISO_BUF_MAX; k++) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            isp_ctx->linkage.sync_wb_gain[k][i] = awb_result->white_balance_gain[i];
        }

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            isp_ctx->linkage.sync_ccm[k][i] = awb_result->color_matrix[i];
        }

        isp_ctx->linkage.sync_color_temp[k] = 5000; /* default color_tem: 5000 */
    }

    reg_cfg->cfg_key.bit1_awb_dyn_cfg = 1;
}

hi_s32 isp_awb_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    hi_u8  i, actual_zone_col, actual_zone_row;
    hi_u16 height, width;
    hi_isp_awb_param awb_param;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lib_node *lib = HI_NULL;
    hi_isp_awb_result awb_init_result = {{0}};

    ret = awb_stat_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_get_ctx(vi_pipe, isp_ctx);

    height = isp_ctx->block_attr.frame_rect.height;
    width = isp_ctx->block_attr.frame_rect.width;

    actual_zone_col = MIN2(width, AWB_ZONE_ORIG_COLUMN * AWB_MIN_WIDTH) / AWB_MIN_WIDTH;
    actual_zone_row = MIN2(height, AWB_ZONE_ORIG_ROW * AWB_MIN_HEIGHT) / AWB_MIN_HEIGHT;

    awb_param.sensor_id = isp_ctx->bind_attr.sensor_id;
    awb_param.wdr_mode = isp_ctx->sns_wdr_mode;
    awb_param.init_iso = isp_ctx->linkage.iso;
    awb_param.stitch_attr.main_pipe = isp_ctx->stitch_attr.main_pipe;
    awb_param.stitch_attr.stitch_enable = isp_ctx->stitch_attr.stitch_enable;
    awb_param.stitch_attr.stitch_pipe_num = isp_ctx->stitch_attr.stitch_pipe_num;

    memcpy_s(awb_param.stitch_attr.stitch_bind_id, sizeof(hi_s8) * VI_MAX_PIPE_NUM,
             isp_ctx->stitch_attr.stitch_bind_id, sizeof(hi_s8) * VI_MAX_PIPE_NUM);

    awb_regs_default(vi_pipe, (isp_reg_cfg *)reg_cfg, actual_zone_row, actual_zone_col, 1);
    awb_param.awb_zone_row = (hi_u8)(actual_zone_row);
    awb_param.awb_zone_bin = 1;

    if (awb_param.stitch_attr.stitch_enable == 1) {
        awb_param.awb_zone_col = MIN2(0xFF, (actual_zone_col * awb_param.stitch_attr.stitch_pipe_num));
    } else {
        awb_param.awb_zone_col = actual_zone_col;
    }

    awb_param.awb_width = isp_ctx->block_attr.frame_rect.width;
    awb_param.awb_height = isp_ctx->block_attr.frame_rect.height;

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (isp_ctx->awb_lib_info.libs[i].used) {
            lib = &isp_ctx->awb_lib_info.libs[i];

            if (lib->awb_regsiter.awb_exp_func.pfn_awb_init != HI_NULL) {
                lib->awb_regsiter.awb_exp_func.pfn_awb_init(lib->alg_lib.id, &awb_param, &awb_init_result);
            }
        }
    }

    awb_cfg_init_regs(vi_pipe, &awb_init_result, (isp_reg_cfg *)reg_cfg);
    return HI_SUCCESS;
}

static hi_void isp_awb_run_config_snap_ccm(isp_lib_node *lib, hi_u32 color_temp, awb_ccm_config *ccm_conf_def)
{
    awb_ccm_config ccm_conf;

    if (lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl != HI_NULL) {
        lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl(lib->alg_lib.id, AWB_CCM_CONFIG_GET, (hi_void *)ccm_conf_def);

        ccm_conf.awb_bypass_en = HI_TRUE;
        ccm_conf.manual_temp_en = HI_TRUE;
        ccm_conf.manual_temp_value = color_temp;
        ccm_conf.ccm_speed = 0xfff;

        ccm_conf.manual_sat_en = ccm_conf_def->manual_sat_en;
        ccm_conf.manual_sat_value = ccm_conf_def->manual_sat_value;
        lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl(lib->alg_lib.id, AWB_CCM_CONFIG_SET, (hi_void *)&ccm_conf);
    }
}

static hi_void isp_awb_run_config_exp_info(isp_lib_info *awb_lib_info, isp_linkage *linkage, \
                                           hi_isp_awb_param *awb_param)
{
    hi_u8 i;

    /* linkage with the iso of ae */
    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (awb_lib_info->libs[i].used) {
            isp_lib_node *lib = &awb_lib_info->libs[i];
            hi_isp_awb_exp_func *awb_exp_func = &lib->awb_regsiter.awb_exp_func;
            hi_s32 id = lib->alg_lib.id;

            if (awb_exp_func->pfn_awb_ctrl != HI_NULL) {
                awb_exp_func->pfn_awb_ctrl(id, ISP_AWB_ISO_SET, (hi_void *)&linkage->iso);
                awb_exp_func->pfn_awb_ctrl(id, ISP_AWB_INTTIME_SET, (hi_void *)&linkage->int_time);
                awb_exp_func->pfn_awb_ctrl(id, ISP_AWB_SNAP_MODE_SET, (hi_void *)&linkage->snap_pipe_mode);
                awb_exp_func->pfn_awb_ctrl(id, ISP_AWB_ZONE_ROW_SET, (hi_void *)&awb_param->awb_zone_row);
                awb_exp_func->pfn_awb_ctrl(id, ISP_AWB_ZONE_COL_SET, (hi_void *)&awb_param->awb_zone_col);
            }
        }
    }
}
hi_u16 stat_blc_dg(hi_u16 stat_value, hi_u32 offset, hi_s32 dgain, hi_s32 dgain_shift)
{
    if ((hi_u32)stat_value <= offset) {
        return 0;
    } else {
        return MIN2(((((hi_u32)stat_value - offset) * dgain) >> dgain_shift), 0xFFFF);
    }
}

static hi_void isp_awb_stat_blc_dg(hi_u8 vi_pipe, hi_isp_awb_info *awb_info, isp_reg_cfg *reg_cfg)
{
    hi_u32 i;
    hi_u8 dgain_en;
    hi_u32 blc_r, blc_g, blc_b;
    hi_s32 dgain, shft;
    isp_usr_ctx *isp_ctx = HI_NULL;

    dgain_en = hi_ext_system_isp_dgain_enable_read(vi_pipe);
    if (dgain_en == HI_FALSE) {
        return;
    }

    blc_r = (reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.lsc_blc.blc[BAYER_CHN_R] << 2); /* 2=(14bit to 16 bit) */
    blc_g = reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.lsc_blc.blc[BAYER_CHN_GR];
    blc_g += reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.lsc_blc.blc[BAYER_CHN_GB];
    blc_g = (blc_g << 1);
    blc_b = (reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.lsc_blc.blc[BAYER_CHN_B] << 2); /* 2=(14bit to 16 bit) */

    isp_get_ctx(vi_pipe, isp_ctx);

    dgain = isp_ctx->linkage.isp_dgain;
    shft = isp_ctx->linkage.isp_dgain_shift;

    awb_info->awb_stat1->metering_awb_avg_r	= stat_blc_dg(awb_info->awb_stat1->metering_awb_avg_r, blc_r, dgain, shft);
    awb_info->awb_stat1->metering_awb_avg_g	= stat_blc_dg(awb_info->awb_stat1->metering_awb_avg_g, blc_g, dgain, shft);
    awb_info->awb_stat1->metering_awb_avg_b	= stat_blc_dg(awb_info->awb_stat1->metering_awb_avg_b, blc_b, dgain, shft);

    for (i = 0; i < AWB_ZONE_NUM; i++) {
        awb_info->awb_stat2.zone_avg_r[i] = stat_blc_dg(awb_info->awb_stat2.zone_avg_r[i], blc_r, dgain, shft);
        awb_info->awb_stat2.zone_avg_g[i] = stat_blc_dg(awb_info->awb_stat2.zone_avg_g[i], blc_g, dgain, shft);
        awb_info->awb_stat2.zone_avg_b[i] = stat_blc_dg(awb_info->awb_stat2.zone_avg_b[i], blc_b, dgain, shft);
    }
}

static hi_void isp_awb_get_lsc_lut_line(hi_u8 vi_pipe, isp_awb_lsc_lut *lsc_lut, hi_u32 line_index)
{
    hi_u32 i;
    hi_u16 alpha = lsc_lut->blend_ratio;
    hi_u16 beta = 0x100 - alpha;
    hi_u16 blend_shift = 0x8;
    hi_u16 blend_round = 1 << (blend_shift - 1);
    hi_u16 index = lsc_lut->index;
    hi_u16 *mesh_gain_r = &lsc_lut->mesh_lut_r[line_index][0];
    hi_u16 *mesh_gain_g = &lsc_lut->mesh_lut_g[line_index][0];
    hi_u16 *mesh_gain_b = &lsc_lut->mesh_lut_b[line_index][0];

    for (i = 0; i < HI_ISP_LSC_GRID_COL; i++) {
        hi_u16 r_gain0, gr_gain0, gb_gain0, b_gain0, g_gain0;
        hi_u16 r_gain1, gr_gain1, gb_gain1, b_gain1, g_gain1;

        r_gain0 = hi_ext_system_isp_mesh_shading_r_gain0_read(vi_pipe, index);
        gr_gain0 = hi_ext_system_isp_mesh_shading_gr_gain0_read(vi_pipe, index);
        gb_gain0 = hi_ext_system_isp_mesh_shading_gb_gain0_read(vi_pipe, index);
        b_gain0 = hi_ext_system_isp_mesh_shading_b_gain0_read(vi_pipe, index);

        r_gain1 = hi_ext_system_isp_mesh_shading_r_gain1_read(vi_pipe, index);
        gr_gain1 = hi_ext_system_isp_mesh_shading_gr_gain1_read(vi_pipe, index);
        gb_gain1 = hi_ext_system_isp_mesh_shading_gb_gain1_read(vi_pipe, index);
        b_gain1 = hi_ext_system_isp_mesh_shading_b_gain1_read(vi_pipe, index);

        g_gain0 = (gr_gain0 + gb_gain0 + 0x1) >> 1; /* rshift 1 for average */
        g_gain1 = (gr_gain1 + gb_gain1 + 0x1) >> 1; /* rshift 1 for average */

        mesh_gain_r[i] = (alpha * r_gain0 + beta * r_gain1 + blend_round) >> blend_shift;
        mesh_gain_g[i] = (alpha * g_gain0 + beta * g_gain1 + blend_round) >> blend_shift;
        mesh_gain_b[i] = (alpha * b_gain0 + beta * b_gain1 + blend_round) >> blend_shift;
        index++;
    }
    lsc_lut->index += HI_ISP_LSC_GRID_COL;

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1); i++) {
        mesh_gain_r[i] = (mesh_gain_r[i] + mesh_gain_r[i + 1] + 0x1) >> 0x1; /* rshift 1 for average */
        mesh_gain_g[i] = (mesh_gain_g[i] + mesh_gain_g[i + 1] + 0x1) >> 0x1; /* rshift 1 for average */
        mesh_gain_b[i] = (mesh_gain_b[i] + mesh_gain_b[i + 1] + 0x1) >> 0x1; /* rshift 1 for average */
    }
}

#define AWB_STAT_LSC(a, b, shift) (hi_u16)(((a) * (b) + (1 << ((shift) - 1))) >> (shift))

static hi_void isp_awb_stat_lsc_process(hi_u8 vi_pipe, isp_awb_lsc_lut *lsc_lut, hi_isp_awb_info *awb_info)
{
    hi_u32 i, j;
    hi_u32 mul_shift, cur_line;
    hi_u16 gain_shift = lsc_lut->gain_shift;
    hi_s16 mesh_gain_unit = (1 << gain_shift);
    hi_u16 mesh_gain_max = (1 << (gain_shift + 0x4)); /* 0x4 for 16 */
    hi_s16 scale_gain_unit = 0;
    hi_isp_awb_stat_result *awb_stat_zone = &(awb_info->awb_stat2);
    hi_u64 sum_zone_r = 0;
    hi_u64 sum_zone_g = 0;
    hi_u64 sum_zone_b = 0;
    hi_u64 sum_zone_cnt = 0;

    if (lsc_lut->scale <= 3) { /* 0-3 mesh_scale */
        scale_gain_unit = (1 << (9 - lsc_lut->scale)); /* 9 for dec */
        mul_shift = (21 - gain_shift - lsc_lut->scale); /* 21 = 12 + 9 */
    } else {
        mul_shift = (26 - gain_shift - lsc_lut->scale); /* 26 = 12 + 9 + 5 */
    }

    lsc_lut->index = 0;
    cur_line = 0;
    isp_awb_get_lsc_lut_line(vi_pipe, lsc_lut, cur_line);

    for (i = 0; i < AWB_ZONE_ORIG_ROW; i++) {
        isp_awb_get_lsc_lut_line(vi_pipe, lsc_lut, !cur_line);

        for (j = 0; j < AWB_ZONE_ORIG_COLUMN; j++) {
            hi_s32 gain_r = (lsc_lut->mesh_lut_r[cur_line][j] + lsc_lut->mesh_lut_r[!cur_line][j] + 0x1) >> 1;
            hi_s32 gain_g = (lsc_lut->mesh_lut_g[cur_line][j] + lsc_lut->mesh_lut_g[!cur_line][j] + 0x1) >> 1;
            hi_s32 gain_b = (lsc_lut->mesh_lut_b[cur_line][j] + lsc_lut->mesh_lut_b[!cur_line][j] + 0x1) >> 1;
            hi_u32 k = i * AWB_ZONE_ORIG_COLUMN + j;

            gain_r -= scale_gain_unit;
            gain_g -= scale_gain_unit;
            gain_b -= scale_gain_unit;

            gain_r = mesh_gain_unit + ((gain_r * lsc_lut->strength) >> mul_shift);
            gain_g = mesh_gain_unit + ((gain_g * lsc_lut->strength) >> mul_shift);
            gain_b = mesh_gain_unit + ((gain_b * lsc_lut->strength) >> mul_shift);

            gain_r = MIN2(MAX2(gain_r, 0x0), mesh_gain_max);
            gain_g = MIN2(MAX2(gain_g, 0x0), mesh_gain_max);
            gain_b = MIN2(MAX2(gain_b, 0x0), mesh_gain_max);

            awb_stat_zone->zone_avg_r[k] = AWB_STAT_LSC(awb_stat_zone->zone_avg_r[k], (hi_u32)gain_r, gain_shift);
            awb_stat_zone->zone_avg_g[k] = AWB_STAT_LSC(awb_stat_zone->zone_avg_g[k], (hi_u32)gain_g, gain_shift);
            awb_stat_zone->zone_avg_b[k] = AWB_STAT_LSC(awb_stat_zone->zone_avg_b[k], (hi_u32)gain_b, gain_shift);

            sum_zone_r += ((hi_u64)awb_stat_zone->zone_avg_r[k]) * awb_stat_zone->zone_count[k];
            sum_zone_g += ((hi_u64)awb_stat_zone->zone_avg_g[k]) * awb_stat_zone->zone_count[k];
            sum_zone_b += ((hi_u64)awb_stat_zone->zone_avg_b[k]) * awb_stat_zone->zone_count[k];
            sum_zone_cnt += (hi_u64)awb_stat_zone->zone_count[k];
        }
        cur_line = !cur_line;
    }

    awb_info->awb_stat1->metering_awb_avg_r = (hi_u16)(sum_zone_r / div_0_to_1(sum_zone_cnt));
    awb_info->awb_stat1->metering_awb_avg_g = (hi_u16)(sum_zone_g / div_0_to_1(sum_zone_cnt));
    awb_info->awb_stat1->metering_awb_avg_b = (hi_u16)(sum_zone_b / div_0_to_1(sum_zone_cnt));

    return;
}

static hi_void isp_awb_run_config_stat_info(hi_vi_pipe vi_pipe, hi_u32 frame_cnt, isp_reg_cfg *reg_cfg, \
                                            isp_stat *stat_info, hi_isp_awb_info *awb_info)
{
    hi_u32 i;
    isp_awb_stat_info *awb_stat = HI_NULL;
    hi_u8 mesh_lsc_en;
    isp_awb_lsc_lut lsc_lut;

    awb_get_stat_info(vi_pipe, awb_stat);

    awb_info->awb_stat1 = &(awb_stat->awb_glob_stat);
    awb_info->awb_stat2.zone_avg_r = &(awb_stat->awb_zone_stat.metering_mem_array_avg_r[0]);
    awb_info->awb_stat2.zone_avg_g = &(awb_stat->awb_zone_stat.metering_mem_array_avg_g[0]);
    awb_info->awb_stat2.zone_avg_b = &(awb_stat->awb_zone_stat.metering_mem_array_avg_b[0]);
    awb_info->awb_stat2.zone_count = &(awb_stat->awb_zone_stat.metering_mem_array_count_all[0]);

    awb_info->frame_cnt = frame_cnt;
    awb_info->awb_gain_switch = WDR_WBGAIN_IN_WB;
    awb_info->awb_stat_switch = hi_ext_system_awb_switch_read(vi_pipe);
    awb_info->wb_gain_in_sensor = HI_FALSE;

    awb_info->awb_stat1->metering_awb_avg_r = stat_info->awb_stat1.metering_awb_avg_r;
    awb_info->awb_stat1->metering_awb_avg_g = stat_info->awb_stat1.metering_awb_avg_g;
    awb_info->awb_stat1->metering_awb_avg_b = stat_info->awb_stat1.metering_awb_avg_b;
    awb_info->awb_stat1->metering_awb_count_all = stat_info->awb_stat1.metering_awb_count_all;

    for (i = 0; i < AWB_ZONE_NUM; i++) {
        awb_info->awb_stat2.zone_avg_r[i] = stat_info->awb_stat2.metering_mem_array_avg_r[i];
        awb_info->awb_stat2.zone_avg_g[i] = stat_info->awb_stat2.metering_mem_array_avg_g[i];
        awb_info->awb_stat2.zone_avg_b[i] = stat_info->awb_stat2.metering_mem_array_avg_b[i];
        awb_info->awb_stat2.zone_count[i] = stat_info->awb_stat2.metering_mem_array_count_all[i];
    }

    if (awb_info->awb_stat_switch != HI_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER) {
        return;
    }

    isp_awb_stat_blc_dg(vi_pipe, awb_info, reg_cfg);

    mesh_lsc_en = hi_ext_system_isp_mesh_shading_enable_read(vi_pipe);
    lsc_lut.blend_ratio = hi_ext_system_isp_mesh_shading_blendratio_read(vi_pipe);
    lsc_lut.strength = hi_ext_system_isp_mesh_shading_mesh_strength_read(vi_pipe);
    lsc_lut.scale = hi_ext_system_isp_mesh_shading_mesh_scale_read(vi_pipe);

    if ((mesh_lsc_en == HI_FALSE) || (lsc_lut.strength == 0)) {
        return;
    }

    lsc_lut.gain_shift = 0x8; /* 8 bit dec */
    isp_awb_stat_lsc_process(vi_pipe, &lsc_lut, awb_info);

    return;
}

static hi_void isp_awb_run_res_2_linkage(hi_isp_awb_result *awb_result, isp_linkage *linkage,
                                         hi_isp_attach_info *attach_info, hi_u8 sns_wdr_mode)
{
    hi_u8 i, j;

    linkage->color_temp = awb_result->color_temp;
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        linkage->white_balance_gain[i] = awb_result->white_balance_gain[i];
    }
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        linkage->ccm[i] = awb_result->color_matrix[i];
    }

    for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
        linkage->sync_color_temp[i] = linkage->sync_color_temp[i - 1];
        for (j = 0; j < CCM_MATRIX_SIZE; j++) {
            linkage->sync_ccm[i][j] = linkage->sync_ccm[i - 1][j];
        }
        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            linkage->sync_wb_gain[i][j] = linkage->sync_wb_gain[i - 1][j];
        }
    }

    linkage->sync_color_temp[0] = awb_result->color_temp;
    for (j = 0; j < CCM_MATRIX_SIZE; j++) {
        linkage->sync_ccm[0][j] = awb_result->color_matrix[j];
    }
    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        linkage->sync_wb_gain[0][j] = awb_result->white_balance_gain[j];
    }

    attach_info->iso = linkage->iso;
    attach_info->sns_wdr_mode = sns_wdr_mode;
    attach_info->isp_hdr.color_temp = linkage->color_temp;
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        attach_info->isp_hdr.ccm[i] = linkage->ccm[i];
    }
}

static hi_void isp_awb_run_apply_diff(hi_vi_pipe vi_pipe, hi_isp_awb_result *awb_result)
{
    hi_u8 i;
    hi_u32 diff_gain;
    hi_s16 calc_ccm[CCM_MATRIX_SIZE] = {0};
    hi_s16 diff_ccm[CCM_MATRIX_SIZE] = {0};
    hi_s16 res_ccm[CCM_MATRIX_SIZE] = {0};

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        diff_gain = hi_ext_system_isp_pipe_diff_gain_read(vi_pipe, i);
        awb_result->white_balance_gain[i] = isp_awb_rshift8(awb_result->white_balance_gain[i] * diff_gain);
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        calc_ccm[i] = direct_to_complement_u16(awb_result->color_matrix[i]);
        diff_ccm[i] = hi_ext_system_isp_pipe_diff_ccm_read(vi_pipe, i);
        diff_ccm[i] = direct_to_complement_u16(diff_ccm[i]);
    }

    awb_matrix_multiply(calc_ccm, diff_ccm, res_ccm, 0x3);

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        awb_result->color_matrix[i] = complement_to_direct_u16(res_ccm[i]);
    }
}

static hi_void isp_awb_run_config_pq_ai_gain(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, isp_reg_cfg *reg_cfg)
{
    hi_u8 i, j;
    hi_u32 wb_gain[ISP_BAYER_CHN_NUM];
    isp_awb_reg_dyn_cfg *awb_reg_dyn_cfg = HI_NULL;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        wb_gain[i] = isp_ctx->linkage.white_balance_gain[i];
        wb_gain[i] = isp_awb_rshift8(wb_gain[i]);
        wb_gain[i] = MIN2(wb_gain[i], ISP_AWB_GAIN_MAX_THR);
    }

    for (j = 0; j < reg_cfg->cfg_num; j++) {
        awb_reg_dyn_cfg = &reg_cfg->alg_reg_cfg[j].awb_reg_cfg.awb_reg_dyn_cfg;
        awb_reg_dyn_cfg->be_wb_work_en = hi_ext_system_awb_gain_enable_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_en      = hi_ext_system_cc_enable_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_b_gain  = hi_ext_system_cc_colortone_bgain_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_g_gain  = hi_ext_system_cc_colortone_ggain_read(vi_pipe);
        awb_reg_dyn_cfg->be_cc_r_gain  = hi_ext_system_cc_colortone_rgain_read(vi_pipe);
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            awb_reg_dyn_cfg->be_white_balance_gain[i] = wb_gain[i];
        }
    }
}

hi_s32 isp_awb_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg, hi_s32 rsv)
{
    hi_s32 ret = HI_FAILURE;
    hi_isp_awb_param awb_param;
    hi_isp_awb_info awb_info = {0};
    hi_isp_awb_result awb_result = {{0}};
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lib_node *lib = HI_NULL;
    awb_ccm_config ccm_conf_def;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    if ((isp_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == isp_ctx->ai_info.ai_pipe_id)) {
        isp_awb_run_config_pq_ai_gain(vi_pipe, isp_ctx, (isp_reg_cfg *)reg_cfg);
        return HI_SUCCESS;
    }

    lib = &isp_ctx->awb_lib_info.libs[isp_ctx->awb_lib_info.active_lib];

    if ((isp_ctx->linkage.snap_state == HI_TRUE) && (isp_ctx->linkage.load_ccm == HI_TRUE)) {
        return HI_SUCCESS;
    }

    if ((isp_ctx->linkage.snap_state == HI_TRUE) && (isp_ctx->linkage.load_ccm != HI_TRUE)) {
        isp_awb_run_config_snap_ccm(lib, isp_ctx->linkage.color_temp, &ccm_conf_def);
    }

    if ((isp_ctx->linkage.snap_state != HI_TRUE) && (isp_ctx->linkage.stat_ready == HI_FALSE)) {
        return HI_SUCCESS;
    }

    if ((is_online_mode(isp_ctx->block_attr.running_mode) ||
         is_sidebyside_mode(isp_ctx->block_attr.running_mode)) &&
        (isp_ctx->linkage.snap_pipe_mode == ISP_SNAP_PICTURE)) {
        return HI_SUCCESS;
    }

    awb_res_read_ext_regs(vi_pipe, (isp_reg_cfg *)reg_cfg);

    awb_param.awb_zone_row = hi_ext_system_awb_vnum_read(vi_pipe);
    awb_param.awb_zone_col = hi_ext_system_awb_hnum_read(vi_pipe);
    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        awb_param.awb_zone_col = MIN2(0xFF, (awb_param.awb_zone_col * isp_ctx->stitch_attr.stitch_pipe_num));
    }

    /* linkage with the iso of ae */
    isp_awb_run_config_exp_info(&isp_ctx->awb_lib_info, &isp_ctx->linkage, &awb_param);

    /* get statistics */
    isp_awb_run_config_stat_info(vi_pipe, isp_ctx->frame_cnt, (isp_reg_cfg *)reg_cfg,
                                 (isp_stat *)stat_info, &awb_info);

    if (lib->awb_regsiter.awb_exp_func.pfn_awb_run != HI_NULL) {
        ret = lib->awb_regsiter.awb_exp_func.pfn_awb_run(lib->alg_lib.id, &awb_info, &awb_result, 0);
        if (ret != HI_SUCCESS) {
            isp_err_trace("WARNING!! ISP[%d] run awb lib err!\n", vi_pipe);
        }
    }

    isp_awb_run_res_2_linkage(&awb_result, &isp_ctx->linkage, isp_ctx->attach_info_ctrl.attach_info,
                              isp_ctx->sns_wdr_mode);

    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        isp_awb_run_apply_diff(vi_pipe, &awb_result);
    }

    awb_cfg_regs(vi_pipe, &awb_result, (isp_reg_cfg *)reg_cfg);

    if (isp_ctx->linkage.snap_state == HI_TRUE) {
        if (lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl != HI_NULL) {
            lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl(lib->alg_lib.id,
                                                        AWB_CCM_CONFIG_SET, (hi_void *)&ccm_conf_def);
        }
    }

    return ret;
}

hi_s32 isp_awb_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    hi_s32 i;
    hi_s32 ret = HI_FAILURE;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lib_node *lib = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    lib = &isp_ctx->awb_lib_info.libs[isp_ctx->awb_lib_info.active_lib];

    if (cmd == ISP_PROC_WRITE) {
        if (lib->used) {
            if (lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl != HI_NULL) {
                ret = lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl(lib->alg_lib.id, cmd, value);
            }
        }
        return ret;
    }

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (isp_ctx->awb_lib_info.libs[i].used) {
            lib = &isp_ctx->awb_lib_info.libs[i];
            if (lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl != HI_NULL) {
                ret = lib->awb_regsiter.awb_exp_func.pfn_awb_ctrl(lib->alg_lib.id, cmd, value);
            }
        }
    }

    if (cmd == ISP_CHANGE_IMAGE_MODE_SET) {
        awb_image_mode_set(vi_pipe, &reg_cfg->reg_cfg);
    }

    if ((cmd == ISP_WDR_MODE_SET) && (isp_ctx->block_attr.block_num != isp_ctx->block_attr.pre_block_num)) {
        awb_image_mode_set(vi_pipe, &reg_cfg->reg_cfg);
    }

    return ret;
}

hi_s32 isp_awb_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lib_node *lib = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (isp_ctx->awb_lib_info.libs[i].used) {
            lib = &isp_ctx->awb_lib_info.libs[i];

            if (lib->awb_regsiter.awb_exp_func.pfn_awb_exit != HI_NULL) {
                lib->awb_regsiter.awb_exp_func.pfn_awb_exit(lib->alg_lib.id);
            }
        }
    }

    awb_stat_info_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_awb(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_awb);

    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_AWB;
    algs->alg_func.pfn_alg_init = isp_awb_init;
    algs->alg_func.pfn_alg_run  = isp_awb_run;
    algs->alg_func.pfn_alg_ctrl = isp_awb_ctrl;
    algs->alg_func.pfn_alg_exit = isp_awb_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

