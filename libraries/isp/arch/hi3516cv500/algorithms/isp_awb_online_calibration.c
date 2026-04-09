/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_awb_online_calibration.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hi_comm_isp_adapt.h"
#include "isp_math_utils.h"
#include "mpi_isp_adapt.h"

#define AWB_CENTER_ROW_MIN  4
#define AWB_CENTER_COL_MIN  4

static hi_void isp_awb_calc_center_gain(hi_isp_statistics_cfg *stat_cfg, hi_isp_wb_statistics  *wb_stat, \
                                        hi_u16 *avg_r_gain, hi_u16 *avg_b_gain)
{
    hi_u32 i, j;
    hi_u32 rsum = 0;
    hi_u32 bsum = 0;
    hi_u32 gsum = 0;
    hi_u32 zone_col, zone_row;
    hi_u32 zone_col_start, zone_col_end, zone_row_start, zone_row_end;

    zone_col = stat_cfg->wb_cfg.zone_col;
    zone_row = stat_cfg->wb_cfg.zone_row;
    zone_col_start = (zone_col >> 1) - (AWB_CENTER_COL_MIN >> 1);
    zone_col_end   = (zone_col >> 1) + (AWB_CENTER_COL_MIN >> 1);
    zone_row_start = (zone_row >> 1) - (AWB_CENTER_ROW_MIN >> 1);
    zone_row_end   = (zone_row >> 1) + (AWB_CENTER_ROW_MIN >> 1);

    /* get_statistics */
    for (j = zone_row_start; j < zone_row_end; j++) {
        for (i = j * zone_col + zone_col_start; i < j * zone_col + zone_col_end; i++) {
            rsum += wb_stat->zone_avg_r[i];
            bsum += wb_stat->zone_avg_b[i];
            gsum += wb_stat->zone_avg_g[i];
        }
    }

    *avg_r_gain = (hi_u16)((gsum * 256) / div_0_to_1(rsum)); /* (G * 256 / R) */
    *avg_b_gain = (hi_u16)((gsum * 256) / div_0_to_1(bsum)); /* (G * 256 / B) */
}

hi_s32 isp_get_lightbox_gain(hi_vi_pipe vi_pipe, hi_isp_awb_calibration_gain *awb_calibration_gain)
{
    hi_s32 ret;
    hi_isp_wb_statistics  *wb_stat  = HI_NULL;
    hi_isp_statistics_cfg *stat_cfg = HI_NULL;

    wb_stat = (hi_isp_wb_statistics *)isp_malloc(sizeof(hi_isp_wb_statistics));
    if (wb_stat == HI_NULL) {
        isp_err_trace("wb_stat malloc failure !\n");
        return HI_ERR_ISP_NOMEM;
    }
    stat_cfg = (hi_isp_statistics_cfg *)isp_malloc(sizeof(hi_isp_statistics_cfg));
    if (stat_cfg == HI_NULL) {
        isp_err_trace("wb_stat malloc failure !\n");
        isp_free(wb_stat);

        return HI_ERR_ISP_NOMEM;
    }
    ret = hi_mpi_isp_get_statistics_config(vi_pipe, stat_cfg);
    if (ret != HI_SUCCESS) {
        isp_free(wb_stat);
        isp_free(stat_cfg);

        return ret;
    }

    if ((stat_cfg->wb_cfg.zone_col < AWB_CENTER_COL_MIN) || (stat_cfg->wb_cfg.zone_row < AWB_CENTER_ROW_MIN)) {
        isp_err_trace("Not support zone number less than 4 x 4 !\n");
        isp_free(wb_stat);
        isp_free(stat_cfg);

        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    ret = hi_mpi_isp_get_wb_statistics(vi_pipe, wb_stat);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Get WB statics failed!\n");
        isp_free(wb_stat);
        isp_free(stat_cfg);

        return ret;
    }

    isp_awb_calc_center_gain(stat_cfg, wb_stat, &awb_calibration_gain->avg_r_gain, &awb_calibration_gain->avg_b_gain);

    isp_free(wb_stat);
    isp_free(stat_cfg);

    return HI_SUCCESS;
}

