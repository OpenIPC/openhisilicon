/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_lsc.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"
#include "isp_math_utils.h"

#define BNR_LSC_DEFAULT_GAIN 4096

static const hi_u16  g_mesh_gain_def[HI_ISP_LSC_MESHSCALE_NUM] = {
    HI_ISP_LSC_MESHSCALE0_DEF_GAIN, HI_ISP_LSC_MESHSCALE1_DEF_GAIN, HI_ISP_LSC_MESHSCALE2_DEF_GAIN,
    HI_ISP_LSC_MESHSCALE3_DEF_GAIN, 0, 0, 0, 0
};

typedef struct {
    hi_bool lsc_enable;
    hi_bool lsc_coef_updata;
    hi_bool lut_update;

    hi_u8  mesh_scale;
    hi_u16 mesh_strength;
    hi_u16 mesh_weight;
    hi_u16 first_point_pos_x[ISP_STRIPING_MAX_NUM];
    hi_u16 cur_width[ISP_STRIPING_MAX_NUM];
    hi_u16 blk_col_start;
    hi_u16 blk_col_end;

    hi_u16 delta_x[HI_ISP_LSC_GRID_COL - 1];
    hi_u16 delta_y[HI_ISP_MLSC_Y_HALF_GRID_NUM];
    hi_u16 inv_x[HI_ISP_LSC_GRID_COL - 1];
    hi_u16 inv_y[HI_ISP_MLSC_Y_HALF_GRID_NUM];
} isp_lsc;

isp_lsc *g_lsc_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define lsc_get_ctx(dev, ctx)   ((ctx) = g_lsc_ctx[dev])
#define lsc_set_ctx(dev, ctx)   (g_lsc_ctx[dev] = (ctx))
#define lsc_reset_ctx(dev)         (g_lsc_ctx[dev] = HI_NULL)

static hi_s32 lsc_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_lsc *lsc_ctx = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc_ctx);

    if (lsc_ctx == HI_NULL) {
        lsc_ctx = (isp_lsc *)isp_malloc(sizeof(isp_lsc));
        if (lsc_ctx == HI_NULL) {
            isp_err_trace("isp[%d] lsc_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(lsc_ctx, sizeof(isp_lsc), 0, sizeof(isp_lsc));

    lsc_set_ctx(vi_pipe, lsc_ctx);

    return HI_SUCCESS;
}

static hi_void lsc_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_lsc *lsc_ctx = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc_ctx);
    isp_free(lsc_ctx);
    lsc_reset_ctx(vi_pipe);
}

static hi_void lsc_geometric_inv_size(isp_lsc *lsc)
{
    hi_s32 i;

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1); i++) {
        if (lsc->delta_x[i] != 0) {
            lsc->inv_x[i] = (0x1000 * 0x400 / lsc->delta_x[i] + 0x200) >> HI_ISP_LSC_GAIN_BITWIDTH;
        } else {
            lsc->inv_x[i] = 0;
        }
    }

    for (i = 0; i < HI_ISP_MLSC_Y_HALF_GRID_NUM; i++) {
        if (lsc->delta_y[i] != 0) {
            lsc->inv_y[i] = (0x1000 * 0x400 / lsc->delta_y[i] + 0x200) >> HI_ISP_LSC_GAIN_BITWIDTH;
        } else {
            lsc->inv_y[i] = 0;
        }
    }

    return;
}

static hi_void lsc_get_lut_index(hi_u8 cur_blk, isp_lsc *lsc, isp_lsc_usr_cfg *usr_reg_cfg)
{
    hi_u16 width = lsc->cur_width[cur_blk] >> 1;
    hi_u16 first_point_pos_x = lsc->first_point_pos_x[cur_blk] >> 1;
    hi_u16 last_point_pos_x;
    hi_u16 width_sum_of_blk;
    hi_u16 dis2_right, dis2_left;
    hi_u16 blk_col_start;
    hi_u16 blk_col_end;

    blk_col_start      = 0;
    blk_col_end        = 0;
    width_sum_of_blk    = lsc->delta_x[0];

    while ((first_point_pos_x >= width_sum_of_blk) && (first_point_pos_x != 0)) {
        blk_col_start++;
        width_sum_of_blk += lsc->delta_x[blk_col_start];
    }

    dis2_right = width_sum_of_blk - first_point_pos_x;
    dis2_left  = lsc->delta_x[blk_col_start] - dis2_right;

    usr_reg_cfg->width_offset = dis2_left;

    last_point_pos_x = first_point_pos_x + width;
    blk_col_end     = blk_col_start;
    while (last_point_pos_x > width_sum_of_blk) {
        blk_col_end++;
        width_sum_of_blk += lsc->delta_x[blk_col_end];
    }
    blk_col_end += 1;

    lsc->blk_col_start  = blk_col_start;
    lsc->blk_col_end    = blk_col_end;

    return;
}

static hi_void lsc_get_gain_lut_initialize(hi_vi_pipe vi_pipe, isp_lsc *lsc, isp_lsc_usr_cfg *usr_reg_cfg)
{
    hi_u16 i, j;
    hi_u16 blk_col_end, blk_col_start, index_offset, src_index;
    hi_u32 def_gain;
    hi_isp_cmos_default   *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc *cmos_lsc = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    blk_col_end   = lsc->blk_col_end;
    blk_col_start = lsc->blk_col_start;

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_void_return(sns_dft->lsc);

        cmos_lsc = sns_dft->lsc;

        for (j = 0; j < HI_ISP_LSC_GRID_ROW; j++) {
            for (i = 0; i <= (blk_col_end - blk_col_start); i++) {
                index_offset = j * HI_ISP_LSC_GRID_COL;
                src_index    = index_offset  + blk_col_start + i;

                usr_reg_cfg->r_gain[index_offset + i] = ((hi_u32)cmos_lsc->lsc_calib_table[1].r_gain[src_index]
                    << HI_ISP_LSC_GAIN_BITWIDTH) + cmos_lsc->lsc_calib_table[0].r_gain[src_index];
                usr_reg_cfg->gr_gain[index_offset + i] = ((hi_u32)cmos_lsc->lsc_calib_table[1].gr_gain[src_index]
                    << HI_ISP_LSC_GAIN_BITWIDTH) + cmos_lsc->lsc_calib_table[0].gr_gain[src_index];
                usr_reg_cfg->gb_gain[index_offset + i] = ((hi_u32)cmos_lsc->lsc_calib_table[1].gb_gain[src_index]
                    << HI_ISP_LSC_GAIN_BITWIDTH) + cmos_lsc->lsc_calib_table[0].gb_gain[src_index];
                usr_reg_cfg->b_gain[index_offset + i] = ((hi_u32)cmos_lsc->lsc_calib_table[1].b_gain[src_index]
                    << HI_ISP_LSC_GAIN_BITWIDTH) + cmos_lsc->lsc_calib_table[0].b_gain[src_index];
            }
        }
    } else {
        def_gain = ((hi_u32)g_mesh_gain_def[lsc->mesh_scale] << HI_ISP_LSC_GAIN_BITWIDTH) +
            g_mesh_gain_def[lsc->mesh_scale];

        for (j = 0; j < HI_ISP_LSC_GRID_ROW; j++) {
            for (i = 0; i <= (blk_col_end - blk_col_start); i++) {
                index_offset = j * HI_ISP_LSC_GRID_COL;

                usr_reg_cfg->r_gain[index_offset + i]   = def_gain;
                usr_reg_cfg->gr_gain[index_offset + i]  = def_gain;
                usr_reg_cfg->gb_gain[index_offset + i]  = def_gain;
                usr_reg_cfg->b_gain[index_offset + i]   = def_gain;
            }
        }
    }

    for (i = 0; i < (blk_col_end - blk_col_start); i++) {
        usr_reg_cfg->delta_x[i] = lsc->delta_x[blk_col_start + i];
        usr_reg_cfg->inv_x[i]   = lsc->inv_x[blk_col_start   + i];
    }

    for (i = 0; i < HI_ISP_MLSC_Y_HALF_GRID_NUM; i++) {
        usr_reg_cfg->delta_y[i] = lsc->delta_y[i];
        usr_reg_cfg->inv_y[i]   = lsc->inv_y[i];
    }

    return;
}

static hi_void lsc_get_gain_lut(hi_vi_pipe vi_pipe, isp_lsc *lsc, isp_lsc_usr_cfg *usr_reg_cfg)
{
    hi_u16 i, j;
    hi_u16 blk_col_end;
    hi_u16 blk_col_start;
    hi_u16 index_offset;
    hi_u16 src_index;
    hi_u16 r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1, b_gain0, b_gain1;

    blk_col_end   = lsc->blk_col_end;
    blk_col_start = lsc->blk_col_start;

    for (j = 0; j < HI_ISP_LSC_GRID_ROW; j++) {
        for (i = 0; i <= (blk_col_end - blk_col_start); i++) {
            index_offset = j * HI_ISP_LSC_GRID_COL;
            src_index    = index_offset  + blk_col_start + i;

            r_gain0  = hi_ext_system_isp_mesh_shading_r_gain0_read(vi_pipe, src_index);
            r_gain1  = hi_ext_system_isp_mesh_shading_r_gain1_read(vi_pipe, src_index);

            gr_gain0 = hi_ext_system_isp_mesh_shading_gr_gain0_read(vi_pipe, src_index);
            gr_gain1 = hi_ext_system_isp_mesh_shading_gr_gain1_read(vi_pipe, src_index);

            gb_gain0 = hi_ext_system_isp_mesh_shading_gb_gain0_read(vi_pipe, src_index);
            gb_gain1 = hi_ext_system_isp_mesh_shading_gb_gain1_read(vi_pipe, src_index);

            b_gain0  = hi_ext_system_isp_mesh_shading_b_gain0_read(vi_pipe, src_index);
            b_gain1  = hi_ext_system_isp_mesh_shading_b_gain1_read(vi_pipe, src_index);

            usr_reg_cfg->r_gain[index_offset + i]  = ((hi_u32)r_gain1  << HI_ISP_LSC_GAIN_BITWIDTH) + r_gain0;
            usr_reg_cfg->gr_gain[index_offset + i] = ((hi_u32)gr_gain1 << HI_ISP_LSC_GAIN_BITWIDTH) + gr_gain0;
            usr_reg_cfg->gb_gain[index_offset + i] = ((hi_u32)gb_gain1 << HI_ISP_LSC_GAIN_BITWIDTH) + gb_gain0;
            usr_reg_cfg->b_gain[index_offset + i]  = ((hi_u32)b_gain1  << HI_ISP_LSC_GAIN_BITWIDTH) + b_gain0;
        }
    }

    for (i = 0; i < (blk_col_end - blk_col_start); i++) {
        usr_reg_cfg->delta_x[i] = lsc->delta_x[blk_col_start + i];
        usr_reg_cfg->inv_x[i]   = lsc->inv_x[blk_col_start   + i];
    }

    for (i = 0; i < HI_ISP_MLSC_Y_HALF_GRID_NUM; i++) {
        usr_reg_cfg->delta_y[i] = lsc->delta_y[i];
        usr_reg_cfg->inv_y[i]   = lsc->inv_y[i];
    }

    return;
}

static hi_void lsc_static_regs_initialize(hi_u8 i, hi_vi_pipe vi_pipe, isp_lsc_static_cfg *lsc_static_reg_cfg)
{
    lsc_static_reg_cfg->win_num_h   = HI_ISP_LSC_GRID_COL - 1;
    lsc_static_reg_cfg->win_num_v   = HI_ISP_LSC_GRID_ROW - 1;
    lsc_static_reg_cfg->static_resh = HI_TRUE;

    return;
}

static hi_void lsc_usr_regs_multi_initialize(hi_u8 cur_blk, hi_vi_pipe vi_pipe, isp_lsc_usr_cfg *usr_reg_cfg)
{
    isp_lsc *lsc = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc);
    isp_check_pointer_void_return(lsc);

    usr_reg_cfg->mesh_str  = lsc->mesh_strength;
    usr_reg_cfg->weight   = lsc->mesh_weight;
    usr_reg_cfg->mesh_scale = lsc->mesh_scale;

    lsc_get_lut_index(cur_blk, lsc, usr_reg_cfg);

    lsc_get_gain_lut_initialize(vi_pipe, lsc, usr_reg_cfg);

    usr_reg_cfg->update_index = 1;

    usr_reg_cfg->lut_update   = HI_TRUE;
    usr_reg_cfg->coef_update  = HI_TRUE;
    usr_reg_cfg->buf_id       = 0;
}

static hi_void lsc_usr_regs_single_initialize(hi_vi_pipe vi_pipe, isp_lsc_usr_cfg *usr_reg_cfg)
{
    hi_u32  i, def_gain;
    isp_lsc *lsc = HI_NULL;

    hi_isp_cmos_default   *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc *cmos_lsc = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    lsc_get_ctx(vi_pipe, lsc);
    isp_check_pointer_void_return(lsc);

    usr_reg_cfg->mesh_str     = lsc->mesh_strength;
    usr_reg_cfg->weight       = lsc->mesh_weight;
    usr_reg_cfg->mesh_scale   = lsc->mesh_scale;
    usr_reg_cfg->width_offset = 0;

    memcpy_s(usr_reg_cfg->delta_x, sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1), lsc->delta_x,
             sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy_s(usr_reg_cfg->inv_x, sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1), lsc->inv_x,
             sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy_s(usr_reg_cfg->delta_y, sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM, lsc->delta_y,
             sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM);
    memcpy_s(usr_reg_cfg->inv_y, sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM, lsc->inv_y,
             sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM);

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_void_return(sns_dft->lsc);

        cmos_lsc = sns_dft->lsc;

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            usr_reg_cfg->r_gain[i]  = ((hi_u32)cmos_lsc->lsc_calib_table[1].r_gain[i]  << HI_ISP_LSC_GAIN_BITWIDTH) +
                cmos_lsc->lsc_calib_table[0].r_gain[i];
            usr_reg_cfg->gr_gain[i] = ((hi_u32)cmos_lsc->lsc_calib_table[1].gr_gain[i] << HI_ISP_LSC_GAIN_BITWIDTH) +
                cmos_lsc->lsc_calib_table[0].gr_gain[i];
            usr_reg_cfg->gb_gain[i] = ((hi_u32)cmos_lsc->lsc_calib_table[1].gb_gain[i] << HI_ISP_LSC_GAIN_BITWIDTH) +
                cmos_lsc->lsc_calib_table[0].gb_gain[i];
            usr_reg_cfg->b_gain[i]  = ((hi_u32)cmos_lsc->lsc_calib_table[1].b_gain[i]  << HI_ISP_LSC_GAIN_BITWIDTH) +
                cmos_lsc->lsc_calib_table[0].b_gain[i];
        }
    } else {
        def_gain = ((hi_u32)g_mesh_gain_def[lsc->mesh_scale] << HI_ISP_LSC_GAIN_BITWIDTH) +
            g_mesh_gain_def[lsc->mesh_scale];

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            usr_reg_cfg->r_gain[i]  = def_gain;
            usr_reg_cfg->gr_gain[i] = def_gain;
            usr_reg_cfg->gb_gain[i] = def_gain;
            usr_reg_cfg->b_gain[i]  = def_gain;
        }
    }

    usr_reg_cfg->update_index = 1;

    usr_reg_cfg->lut_update           = HI_TRUE;
    usr_reg_cfg->coef_update          = HI_TRUE;
    usr_reg_cfg->buf_id               = 0;
    usr_reg_cfg->switch_lut2_stt_reg_new = HI_FALSE;
    usr_reg_cfg->switch_reg_new_cnt   = 0;
}

static hi_void lsc_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_bool offline_mode;
    hi_s32  i;
    isp_lsc *lsc = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_void_return(lsc);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg->cfg_num == 1) {
        lsc_static_regs_initialize(0, vi_pipe, &reg_cfg->alg_reg_cfg[0].lsc_reg_cfg.static_reg_cfg);
        lsc_usr_regs_single_initialize(vi_pipe, &reg_cfg->alg_reg_cfg[0].lsc_reg_cfg.usr_reg_cfg);

        /* for online lut2stt, frame 1 should disable */
        reg_cfg->alg_reg_cfg[0].lsc_reg_cfg.lsc_en             = offline_mode ? (lsc->lsc_enable) : (HI_FALSE);
        reg_cfg->alg_reg_cfg[0].lsc_reg_cfg.lut2_stt_en        = HI_TRUE;
        reg_cfg->alg_reg_cfg[0].be_lut_update_cfg.lsc_lut_update = HI_TRUE;
    } else {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            lsc_static_regs_initialize(i, vi_pipe, &reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.static_reg_cfg);
            lsc_usr_regs_multi_initialize(i, vi_pipe, &reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg);

            reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.lsc_en             = lsc->lsc_enable;
            reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.lut2_stt_en        = HI_TRUE;
            reg_cfg->alg_reg_cfg[i].be_lut_update_cfg.lsc_lut_update = HI_TRUE;
        }
    }

    reg_cfg->cfg_key.bit1_lsc_cfg    = 1;
}

static hi_void bnr_lsc_static_regs_initialize(hi_vi_pipe vi_pipe, isp_rlsc_static_cfg *static_reg_cfg)
{
    static_reg_cfg->node_num = HI_ISP_RLSC_POINTS;
    static_reg_cfg->static_resh = HI_TRUE;
    return;
}

static hi_void bnr_lsc_usr_regs_initialize(hi_u8 cur_blk, hi_vi_pipe vi_pipe, isp_rlsc_usr_cfg *usr_reg_cfg)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_rect block_rect;
    hi_isp_cmos_default    *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc  *cmos_lsc = HI_NULL;

    hi_u8  i, bayer;
    hi_u8  index_r, index_gr, index_gb, index_b;
    hi_u32 width, height;
    hi_u32 off_center;
    const hi_u32 temp = (1U << 31); /* left bitshift 31 for inverse calculation */

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_get_block_rect(&block_rect, &isp_ctx->block_attr, cur_blk);

    width  = isp_ctx->block_attr.frame_rect.width;
    height = isp_ctx->block_attr.frame_rect.height;

    usr_reg_cfg->width_offset = ABS(block_rect.x);
    usr_reg_cfg->gain_scale = HI_ISP_RLSC_DEFAULT_SCALE;

    usr_reg_cfg->center_rx  = width  >> 1;
    usr_reg_cfg->center_ry  = height >> 1;
    usr_reg_cfg->center_gr_x = width  >> 1;
    usr_reg_cfg->center_gr_y = height >> 1;
    usr_reg_cfg->center_gb_x = width  >> 1;
    usr_reg_cfg->center_gb_y = height >> 1;
    usr_reg_cfg->center_bx  = width  >> 1;
    usr_reg_cfg->center_by  = height >> 1;

    off_center = temp / div_0_to_1(width * width / 4 + height * height / 4); /* 4 is square of 2, half width/height */

    usr_reg_cfg->off_center_r  = off_center;
    usr_reg_cfg->off_center_gr = off_center;
    usr_reg_cfg->off_center_gb = off_center;
    usr_reg_cfg->off_center_b  = off_center;

    bayer = isp_ctx->bayer;
    index_r  = bayer ^ 0x0;
    index_gr = bayer ^ 0x1;
    index_gb = bayer ^ 0x2;
    index_b  = bayer ^ 0x3;

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_void_return(sns_dft->lsc);
        cmos_lsc = sns_dft->lsc;

        usr_reg_cfg->gain_str = cmos_lsc->mesh_strength;
        memcpy_s(usr_reg_cfg->gain_lut[index_r], HI_ISP_RLSC_POINTS * sizeof(hi_u16),
                 cmos_lsc->bnr_lsc_calib_table.r_gain, HI_ISP_RLSC_POINTS * sizeof(hi_u16));
        memcpy_s(usr_reg_cfg->gain_lut[index_gr], HI_ISP_RLSC_POINTS * sizeof(hi_u16),
                 cmos_lsc->bnr_lsc_calib_table.gr_gain, HI_ISP_RLSC_POINTS * sizeof(hi_u16));
        memcpy_s(usr_reg_cfg->gain_lut[index_gb], HI_ISP_RLSC_POINTS * sizeof(hi_u16),
                 cmos_lsc->bnr_lsc_calib_table.gb_gain, HI_ISP_RLSC_POINTS * sizeof(hi_u16));
        memcpy_s(usr_reg_cfg->gain_lut[index_b], HI_ISP_RLSC_POINTS * sizeof(hi_u16),
                 cmos_lsc->bnr_lsc_calib_table.b_gain, HI_ISP_RLSC_POINTS * sizeof(hi_u16));
    } else {
        usr_reg_cfg->gain_str  = HI_ISP_RLSC_DEFAULT_RADIAL_STR;
        for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
            usr_reg_cfg->gain_lut[index_r][i]  = BNR_LSC_DEFAULT_GAIN;
            usr_reg_cfg->gain_lut[index_gr][i] = BNR_LSC_DEFAULT_GAIN;
            usr_reg_cfg->gain_lut[index_gb][i] = BNR_LSC_DEFAULT_GAIN;
            usr_reg_cfg->gain_lut[index_b][i]  = BNR_LSC_DEFAULT_GAIN;
        }
    }

    usr_reg_cfg->gain_lut[index_r][HI_ISP_RLSC_POINTS]  = usr_reg_cfg->gain_lut[index_r][HI_ISP_RLSC_POINTS - 1];
    usr_reg_cfg->gain_lut[index_gr][HI_ISP_RLSC_POINTS] = usr_reg_cfg->gain_lut[index_gr][HI_ISP_RLSC_POINTS - 1];
    usr_reg_cfg->gain_lut[index_gb][HI_ISP_RLSC_POINTS] = usr_reg_cfg->gain_lut[index_gb][HI_ISP_RLSC_POINTS - 1];
    usr_reg_cfg->gain_lut[index_b][HI_ISP_RLSC_POINTS]  = usr_reg_cfg->gain_lut[index_b][HI_ISP_RLSC_POINTS - 1];

    usr_reg_cfg->update_index        = 1;
    usr_reg_cfg->lut_update          = HI_TRUE;
    usr_reg_cfg->coef_update         = HI_TRUE;
    usr_reg_cfg->usr_resh            = HI_TRUE;
    usr_reg_cfg->buf_id              = 0;
    usr_reg_cfg->switch_reg_new_cnt  = 0;
    usr_reg_cfg->switch_lut2_stt_reg_new  = HI_FALSE;

    return;
}

static hi_void bnr_lsc_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_s32 i;
    isp_lsc *lsc = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        bnr_lsc_static_regs_initialize(vi_pipe, &reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.static_reg_cfg);
        bnr_lsc_usr_regs_initialize(i, vi_pipe, &reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg);

        reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.rlsc_en = lsc->lsc_enable;
    }

    reg_cfg->cfg_key.bit1_rlsc_cfg = 1;
}

static hi_void bnr_lsc_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    hi_isp_cmos_default    *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc  *cmos_lsc = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_void_return(sns_dft->lsc);
        cmos_lsc = sns_dft->lsc;
        for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
            hi_ext_system_isp_bnr_shading_r_gain_write(vi_pipe,  i, cmos_lsc->bnr_lsc_calib_table.r_gain[i]);
            hi_ext_system_isp_bnr_shading_gr_gain_write(vi_pipe, i, cmos_lsc->bnr_lsc_calib_table.gr_gain[i]);
            hi_ext_system_isp_bnr_shading_gb_gain_write(vi_pipe, i, cmos_lsc->bnr_lsc_calib_table.gb_gain[i]);
            hi_ext_system_isp_bnr_shading_b_gain_write(vi_pipe,  i, cmos_lsc->bnr_lsc_calib_table.b_gain[i]);
        }
    } else {
        for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
            hi_ext_system_isp_bnr_shading_r_gain_write(vi_pipe, i,  BNR_LSC_DEFAULT_GAIN);
            hi_ext_system_isp_bnr_shading_gr_gain_write(vi_pipe, i, BNR_LSC_DEFAULT_GAIN);
            hi_ext_system_isp_bnr_shading_gb_gain_write(vi_pipe, i, BNR_LSC_DEFAULT_GAIN);
            hi_ext_system_isp_bnr_shading_b_gain_write(vi_pipe, i,  BNR_LSC_DEFAULT_GAIN);
        }
    }
    return;
}

static hi_void lsc_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    isp_lsc *lsc = HI_NULL;
    hi_isp_cmos_default   *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc *cmos_lsc = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    lsc_get_ctx(vi_pipe, lsc);
    isp_check_pointer_void_return(lsc);

    hi_ext_system_isp_mesh_shading_enable_write(vi_pipe, lsc->lsc_enable);
    hi_ext_system_isp_mesh_shading_attr_updata_write(vi_pipe, HI_FALSE);
    hi_ext_system_isp_mesh_shading_lut_attr_updata_write(vi_pipe, HI_FALSE);
    hi_ext_system_isp_mesh_shading_mesh_strength_write(vi_pipe, lsc->mesh_strength);
    hi_ext_system_isp_mesh_shading_blendratio_write(vi_pipe, lsc->mesh_weight);
    hi_ext_system_isp_mesh_shading_mesh_scale_write(vi_pipe, lsc->mesh_scale);

    for (i = 0; i < HI_ISP_MLSC_X_HALF_GRID_NUM; i++) {
        hi_ext_system_isp_mesh_shading_xgrid_write(vi_pipe, i, lsc->delta_x[i]);
    }

    for (i = 0; i < HI_ISP_MLSC_Y_HALF_GRID_NUM; i++) {
        hi_ext_system_isp_mesh_shading_ygrid_write(vi_pipe, i, lsc->delta_y[i]);
    }

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_void_return(sns_dft->lsc);

        cmos_lsc = sns_dft->lsc;

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            hi_ext_system_isp_mesh_shading_r_gain0_write(vi_pipe,  i, cmos_lsc->lsc_calib_table[0].r_gain[i]);
            hi_ext_system_isp_mesh_shading_gr_gain0_write(vi_pipe, i, cmos_lsc->lsc_calib_table[0].gr_gain[i]);
            hi_ext_system_isp_mesh_shading_gb_gain0_write(vi_pipe, i, cmos_lsc->lsc_calib_table[0].gb_gain[i]);
            hi_ext_system_isp_mesh_shading_b_gain0_write(vi_pipe,  i, cmos_lsc->lsc_calib_table[0].b_gain[i]);

            hi_ext_system_isp_mesh_shading_r_gain1_write(vi_pipe,  i, cmos_lsc->lsc_calib_table[1].r_gain[i]);
            hi_ext_system_isp_mesh_shading_gr_gain1_write(vi_pipe, i, cmos_lsc->lsc_calib_table[1].gr_gain[i]);
            hi_ext_system_isp_mesh_shading_gb_gain1_write(vi_pipe, i, cmos_lsc->lsc_calib_table[1].gb_gain[i]);
            hi_ext_system_isp_mesh_shading_b_gain1_write(vi_pipe,  i, cmos_lsc->lsc_calib_table[1].b_gain[i]);
        }
    } else {
        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            hi_ext_system_isp_mesh_shading_r_gain0_write(vi_pipe,  i, g_mesh_gain_def[lsc->mesh_scale]);
            hi_ext_system_isp_mesh_shading_gr_gain0_write(vi_pipe, i, g_mesh_gain_def[lsc->mesh_scale]);
            hi_ext_system_isp_mesh_shading_gb_gain0_write(vi_pipe, i, g_mesh_gain_def[lsc->mesh_scale]);
            hi_ext_system_isp_mesh_shading_b_gain0_write(vi_pipe,  i, g_mesh_gain_def[lsc->mesh_scale]);

            hi_ext_system_isp_mesh_shading_r_gain1_write(vi_pipe,  i, g_mesh_gain_def[lsc->mesh_scale]);
            hi_ext_system_isp_mesh_shading_gr_gain1_write(vi_pipe, i, g_mesh_gain_def[lsc->mesh_scale]);
            hi_ext_system_isp_mesh_shading_gb_gain1_write(vi_pipe, i, g_mesh_gain_def[lsc->mesh_scale]);
            hi_ext_system_isp_mesh_shading_b_gain1_write(vi_pipe,  i, g_mesh_gain_def[lsc->mesh_scale]);
        }
    }

    return;
}

static hi_void lsc_read_ext_regs(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    isp_lsc *lsc = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc);
    isp_check_pointer_void_return(lsc);

    lsc->lsc_coef_updata  = hi_ext_system_isp_mesh_shading_attr_updata_read(vi_pipe);
    hi_ext_system_isp_mesh_shading_attr_updata_write(vi_pipe, HI_FALSE);

    if (lsc->lsc_coef_updata) {
        lsc->mesh_strength = hi_ext_system_isp_mesh_shading_mesh_strength_read(vi_pipe);
        lsc->mesh_weight   = hi_ext_system_isp_mesh_shading_blendratio_read(vi_pipe);
    }

    lsc->lut_update      = hi_ext_system_isp_mesh_shading_lut_attr_updata_read(vi_pipe);
    hi_ext_system_isp_mesh_shading_lut_attr_updata_write(vi_pipe, HI_FALSE);

    if (lsc->lut_update) {
        lsc->mesh_scale = hi_ext_system_isp_mesh_shading_mesh_scale_read(vi_pipe);

        for (i = 0; i < HI_ISP_MLSC_X_HALF_GRID_NUM; i++) {
            lsc->delta_x[i] = hi_ext_system_isp_mesh_shading_xgrid_read(vi_pipe, i);
        }

        for (i = HI_ISP_MLSC_X_HALF_GRID_NUM; i < HI_ISP_LSC_GRID_COL - 1; i++) {
            lsc->delta_x[i] = lsc->delta_x[HI_ISP_LSC_GRID_COL - 2 - i]; /* 2 for other half of delta_x */
        }

        for (i = 0; i < HI_ISP_MLSC_Y_HALF_GRID_NUM; i++) {
            lsc->delta_y[i] = hi_ext_system_isp_mesh_shading_ygrid_read(vi_pipe, i);
        }

        lsc_geometric_inv_size(lsc);
    }

    return;
}

static hi_void lsc_geometric_grid_size(hi_u16 *delta, hi_u16 *inv, hi_u16 length, hi_u16 grid_size)
{
    hi_u16 i, sum;
    hi_u16 half_grid_size;
    hi_u16 diff;
    hi_u16 *tmp_step = HI_NULL;
    hi_u16 sum_r;

    half_grid_size = (grid_size - 1) >> 1;

    if (half_grid_size == 0) {
        return;
    }

    tmp_step = (hi_u16 *)isp_malloc(sizeof(hi_u16) * half_grid_size);
    if (tmp_step == HI_NULL) {
        return;
    }

    memset_s(tmp_step, sizeof(hi_u16) * half_grid_size, 0, sizeof(hi_u16) * half_grid_size);

    sum_r = (hi_u16)((((length >> 1) * 0x400 / div_0_to_1(half_grid_size)) + 0x200) >> HI_ISP_LSC_GAIN_BITWIDTH);

    for (i = 0; i < half_grid_size; i++) {
        tmp_step[i] = sum_r;
    }

    sum = 0;
    for (i = 0; i < half_grid_size; i++) {
        sum = sum + tmp_step[i];
    }

    if (sum != (length >> 1)) {
        if (sum > (length >> 1)) {
            diff = sum - (length >> 1);
            for (i = 0; i < diff; i++) {
                tmp_step[i] = tmp_step[i] - 1;
            }
        } else {
            diff = (length >> 1) - sum;
            for (i = 0; i < diff; i++) {
                tmp_step[half_grid_size - i - 1] = tmp_step[half_grid_size - i - 1] + 1;
            }
        }
    }

    for (i = 0; i < half_grid_size; i++) {
        delta[i] = tmp_step[i];
        inv[i] = (delta[i] == 0) ? (0) : ((0x1000 * 0x400 / div_0_to_1(delta[i]) + 0x200) >> HI_ISP_LSC_GAIN_BITWIDTH);
    }

    isp_free(tmp_step);
}

static hi_void lsc_image_size(hi_vi_pipe vi_pipe, hi_u8 blk_num, isp_lsc  *lsc)
{
    hi_u16 i;
    isp_rect block_rect;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    lsc_geometric_grid_size(lsc->delta_x, lsc->inv_x, isp_ctx->block_attr.frame_rect.width >> 1, HI_ISP_LSC_GRID_COL);
    lsc_geometric_grid_size(lsc->delta_y, lsc->inv_y, isp_ctx->block_attr.frame_rect.height >> 1, HI_ISP_LSC_GRID_ROW);

    for (i = 0; i < HI_ISP_MLSC_X_HALF_GRID_NUM; i++) {
        lsc->delta_x[HI_ISP_LSC_GRID_COL - 2 - i] = lsc->delta_x[i]; /* 2 for other half of delta_x */
        lsc->inv_x[HI_ISP_LSC_GRID_COL - 2 - i]   = lsc->inv_x[i]; /* 2 for other half of inv_x */
    }

    for (i = 0; i < blk_num; i++) {
        isp_get_block_rect(&block_rect, &isp_ctx->block_attr, i);

        lsc->cur_width[i]       = block_rect.width;
        lsc->first_point_pos_x[i] = block_rect.x;
    }
}

static hi_void bnr_lsc_image_size(hi_vi_pipe vi_pipe, hi_u8 blk_num, isp_rlsc_usr_cfg *usr_reg_cfg)
{
    hi_u32 width, height;
    hi_u32 off_center;
    const hi_u32 temp = (1U << 31); /* left bitshift 31 for inverse calculation */

    isp_usr_ctx  *isp_ctx = HI_NULL;
    isp_rect  block_rect;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_get_block_rect(&block_rect, &isp_ctx->block_attr, blk_num);

    width  = isp_ctx->block_attr.frame_rect.width;
    height = isp_ctx->block_attr.frame_rect.height;

    usr_reg_cfg->width_offset = ABS(block_rect.x);

    usr_reg_cfg->center_rx  = width  >> 1;
    usr_reg_cfg->center_ry  = height >> 1;
    usr_reg_cfg->center_gr_x = width  >> 1;
    usr_reg_cfg->center_gr_y = height >> 1;
    usr_reg_cfg->center_gb_x = width  >> 1;
    usr_reg_cfg->center_gb_y = height >> 1;
    usr_reg_cfg->center_bx  = width  >> 1;
    usr_reg_cfg->center_by  = height >> 1;

    off_center = temp / div_0_to_1(width * width / 4 + height * height / 4); /* 4 is square of 2, half width/height */

    usr_reg_cfg->off_center_r  = off_center;
    usr_reg_cfg->off_center_gr = off_center;
    usr_reg_cfg->off_center_gb = off_center;
    usr_reg_cfg->off_center_b  = off_center;

    usr_reg_cfg->update_index += 1;
    usr_reg_cfg->lut_update     = HI_TRUE;
    usr_reg_cfg->usr_resh       = HI_TRUE;
}

static hi_s32 lsc_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_lsc *cmos_lsc)
{
    hi_u16 i;

    if (cmos_lsc->mesh_scale > HI_ISP_LSC_MESHSCALE_NUM - 1) {
        isp_err_trace("Invalid mesh_scale %d!\n", cmos_lsc->mesh_scale);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        if ((cmos_lsc->lsc_calib_table[0].r_gain[i]  > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[0].gr_gain[i] > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[0].gb_gain[i] > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[0].b_gain[i]  > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[1].r_gain[i]  > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[1].gr_gain[i] > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[1].gb_gain[i] > HI_ISP_LSC_MAX_GAIN) ||
            (cmos_lsc->lsc_calib_table[1].b_gain[i]  > HI_ISP_LSC_MAX_GAIN)) {
            isp_err_trace("Invalid Gain!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 lsc_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_s32 ret;
    isp_lsc *lsc = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    lsc->mesh_weight  = HI_ISP_LSC_DEFAULT_WEIGHT;

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_return(sns_dft->lsc);

        ret = lsc_check_cmos_param(vi_pipe, sns_dft->lsc);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        lsc->mesh_strength = sns_dft->lsc->mesh_strength;
        lsc->mesh_scale     = sns_dft->lsc->mesh_scale;
    } else {
        lsc->mesh_strength = HI_ISP_LSC_DEFAULT_MESH_STRENGTH;
        lsc->mesh_scale     = HI_ISP_LSC_DEFAULT_MESH_SCALE;
    }

    lsc_image_size(vi_pipe, reg_cfg->cfg_num, lsc);

    lsc->lsc_enable = HI_TRUE;

    return HI_SUCCESS;
}

static hi_void lsc_usr_fw(hi_vi_pipe vi_pipe, hi_u8 cur_blk, isp_lsc *lsc, isp_lsc_usr_cfg *usr_reg_cfg)
{
    lsc_get_lut_index(cur_blk, lsc, usr_reg_cfg);

    lsc_get_gain_lut(vi_pipe, lsc, usr_reg_cfg);

    usr_reg_cfg->mesh_scale = lsc->mesh_scale;
    usr_reg_cfg->update_index += 1;
    usr_reg_cfg->lut_update = HI_TRUE;
}

static hi_void  lsc_usr_fw_normal(hi_vi_pipe vi_pipe, isp_lsc *lsc, isp_lsc_usr_cfg *usr_reg_cfg)
{
    hi_u16 i;
    hi_u16 r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1, b_gain0, b_gain1;

    usr_reg_cfg->width_offset  = 0;
    usr_reg_cfg->mesh_scale     = lsc->mesh_scale;
    usr_reg_cfg->update_index += 1;
    usr_reg_cfg->lut_update      = HI_TRUE;

    memcpy_s(usr_reg_cfg->delta_x, sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1),
             lsc->delta_x, sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy_s(usr_reg_cfg->inv_x, sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1),
             lsc->inv_x, sizeof(hi_u16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy_s(usr_reg_cfg->delta_y, sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM,
             lsc->delta_y, sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM);
    memcpy_s(usr_reg_cfg->inv_y, sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM,
             lsc->inv_y, sizeof(hi_u16) * HI_ISP_MLSC_Y_HALF_GRID_NUM);

    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        r_gain0  = hi_ext_system_isp_mesh_shading_r_gain0_read(vi_pipe, i);
        r_gain1  = hi_ext_system_isp_mesh_shading_r_gain1_read(vi_pipe, i);

        gr_gain0 = hi_ext_system_isp_mesh_shading_gr_gain0_read(vi_pipe, i);
        gr_gain1 = hi_ext_system_isp_mesh_shading_gr_gain1_read(vi_pipe, i);

        gb_gain0 = hi_ext_system_isp_mesh_shading_gb_gain0_read(vi_pipe, i);
        gb_gain1 = hi_ext_system_isp_mesh_shading_gb_gain1_read(vi_pipe, i);

        b_gain0  = hi_ext_system_isp_mesh_shading_b_gain0_read(vi_pipe, i);
        b_gain1  = hi_ext_system_isp_mesh_shading_b_gain1_read(vi_pipe, i);

        usr_reg_cfg->r_gain[i]  = ((hi_u32)r_gain1  << HI_ISP_LSC_GAIN_BITWIDTH) + r_gain0;
        usr_reg_cfg->gr_gain[i] = ((hi_u32)gr_gain1 << HI_ISP_LSC_GAIN_BITWIDTH) + gr_gain0;
        usr_reg_cfg->gb_gain[i] = ((hi_u32)gb_gain1 << HI_ISP_LSC_GAIN_BITWIDTH) + gb_gain0;
        usr_reg_cfg->b_gain[i]  = ((hi_u32)b_gain1  << HI_ISP_LSC_GAIN_BITWIDTH) + b_gain0;
    }

    return;
}

static hi_void lsc_usr_fw_bnr_lsc(hi_vi_pipe vi_pipe, isp_rlsc_usr_cfg *usr_reg_cfg)
{
    hi_u32 i;
    hi_u8  index_r, index_gr, index_gb, index_b;
    hi_u8  bayer;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    bayer = isp_ctx->bayer;
    index_r  = bayer ^ 0x0;
    index_gr = bayer ^ 0x1;
    index_gb = bayer ^ 0x2;
    index_b  = bayer ^ 0x3;

    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        usr_reg_cfg->gain_lut[index_r][i]  = hi_ext_system_isp_bnr_shading_r_gain_read(vi_pipe, i);
        usr_reg_cfg->gain_lut[index_gr][i] = hi_ext_system_isp_bnr_shading_gr_gain_read(vi_pipe, i);
        usr_reg_cfg->gain_lut[index_gb][i] = hi_ext_system_isp_bnr_shading_gb_gain_read(vi_pipe, i);
        usr_reg_cfg->gain_lut[index_b][i]  = hi_ext_system_isp_bnr_shading_b_gain_read(vi_pipe, i);
    }

    usr_reg_cfg->gain_lut[index_r][HI_ISP_RLSC_POINTS]  = usr_reg_cfg->gain_lut[index_r][HI_ISP_RLSC_POINTS - 1];
    usr_reg_cfg->gain_lut[index_gr][HI_ISP_RLSC_POINTS] = usr_reg_cfg->gain_lut[index_gr][HI_ISP_RLSC_POINTS - 1];
    usr_reg_cfg->gain_lut[index_gb][HI_ISP_RLSC_POINTS] = usr_reg_cfg->gain_lut[index_gb][HI_ISP_RLSC_POINTS - 1];
    usr_reg_cfg->gain_lut[index_b][HI_ISP_RLSC_POINTS]  = usr_reg_cfg->gain_lut[index_b][HI_ISP_RLSC_POINTS - 1];

    usr_reg_cfg->update_index += 1;
    usr_reg_cfg->lut_update      = HI_TRUE;
    usr_reg_cfg->usr_resh        = HI_TRUE;
}

static hi_bool __inline check_lsc_open(isp_lsc *lsc)
{
    return (lsc->lsc_enable == HI_TRUE);
}

static hi_s32 lsc_image_res_write(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i;
    hi_s32 ret;
    isp_lsc *lsc = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    lsc_get_ctx(vi_pipe, lsc);
    isp_check_pointer_return(lsc);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_lsc) {
        isp_check_pointer_return(sns_dft->lsc);

        ret = lsc_check_cmos_param(vi_pipe, sns_dft->lsc);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        lsc->mesh_strength = sns_dft->lsc->mesh_strength;
        lsc->mesh_scale     = sns_dft->lsc->mesh_scale;
    } else {
        lsc->mesh_strength = HI_ISP_LSC_DEFAULT_MESH_STRENGTH;
        lsc->mesh_scale     = HI_ISP_LSC_DEFAULT_MESH_SCALE;
    }

    lsc_image_size(vi_pipe, reg_cfg->cfg_num, lsc);

    lsc_ext_regs_initialize(vi_pipe);

    if (reg_cfg->cfg_num == 1) {
        lsc_usr_fw_normal(vi_pipe, lsc, &reg_cfg->alg_reg_cfg[0].lsc_reg_cfg.usr_reg_cfg);
    } else {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            lsc_usr_fw(vi_pipe, i, lsc, &reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg);
        }
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        bnr_lsc_image_size(vi_pipe, i, &reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg);
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_lut2_stt_reg_new    = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt         = 0;
        reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_lut2_stt_reg_new = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt        = 0;
        reg_cfg->alg_reg_cfg[i].be_lut_update_cfg.lsc_lut_update                     = HI_TRUE;
    }

    reg_cfg->cfg_key.bit1_lsc_cfg  = 1;
    reg_cfg->cfg_key.bit1_rlsc_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_lsc_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_in)
{
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_in;

    ret = lsc_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = lsc_initialize(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    lsc_regs_initialize(vi_pipe, reg_cfg);
    lsc_ext_regs_initialize(vi_pipe);

    bnr_lsc_regs_initialize(vi_pipe, reg_cfg);
    bnr_lsc_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_lsc_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                   hi_void *reg_cfg_in, hi_s32 rsv)
{
    hi_s32 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lsc *lsc = HI_NULL;
    isp_reg_cfg *reg_cfg  = (isp_reg_cfg *)reg_cfg_in;

    isp_get_ctx(vi_pipe, isp_ctx);
    lsc_get_ctx(vi_pipe, lsc);
    isp_check_pointer_return(lsc);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    lsc->lsc_enable = hi_ext_system_isp_mesh_shading_enable_read(vi_pipe);
    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.lsc_en = lsc->lsc_enable;
    }

    reg_cfg->cfg_key.bit1_lsc_cfg  = 1;
    reg_cfg->cfg_key.bit1_rlsc_cfg = 1;
    /* check hardware setting */
    if (!check_lsc_open(lsc)) {
        return HI_SUCCESS;
    }

    lsc_read_ext_regs(vi_pipe);

    if (lsc->lsc_coef_updata) {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.coef_update   = HI_TRUE;
            reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.mesh_str      = lsc->mesh_strength;
            reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.weight        = lsc->mesh_weight;
            reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.coef_update  = HI_TRUE;
            reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.gain_str     = lsc->mesh_strength;
        }
    }

    if (lsc->lut_update) {
        if (reg_cfg->cfg_num == 1) {
            lsc_usr_fw_normal(vi_pipe, lsc, &reg_cfg->alg_reg_cfg[0].lsc_reg_cfg.usr_reg_cfg);
        } else {
            for (i = 0; i < reg_cfg->cfg_num; i++) {
                lsc_usr_fw(vi_pipe, i, lsc, &reg_cfg->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg);
            }
        }

        for (i = 0; i < reg_cfg->cfg_num; i++) {
            lsc_usr_fw_bnr_lsc(vi_pipe, &reg_cfg->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_lsc_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    switch (cmd) {
        case ISP_CHANGE_IMAGE_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            lsc_image_res_write(vi_pipe, &reg_cfg->reg_cfg);
            break;
        case ISP_WDR_MODE_SET:
            isp_get_ctx(vi_pipe, isp_ctx);

            if (isp_ctx->block_attr.block_num != isp_ctx->block_attr.pre_block_num) {
                isp_regcfg_get_ctx(vi_pipe, reg_cfg);
                isp_check_pointer_return(reg_cfg);
                lsc_image_res_write(vi_pipe, &reg_cfg->reg_cfg);
            }
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_lsc_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].lsc_reg_cfg.lsc_en = HI_FALSE;
        reg_cfg->reg_cfg.alg_reg_cfg[i].rlsc_reg_cfg.rlsc_en = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_lsc_cfg = 1;
    reg_cfg->reg_cfg.cfg_key.bit1_rlsc_cfg = 1;

    lsc_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_lsc(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_lsc);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_LSC;
    algs->alg_func.pfn_alg_init = isp_lsc_init;
    algs->alg_func.pfn_alg_run  = isp_lsc_run;
    algs->alg_func.pfn_alg_ctrl = isp_lsc_ctrl;
    algs->alg_func.pfn_alg_exit = isp_lsc_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

