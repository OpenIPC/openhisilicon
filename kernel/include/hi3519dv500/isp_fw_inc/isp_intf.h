/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_INTF_H
#define ISP_INTF_H

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "ot_common_isp_inner.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define isp_param_check_return(x, min, max, fmt, ...)                      \
    do {                                                                   \
        if ((x) < (min) || (x) > (max)) {                                  \
            isp_err_trace(fmt, ##__VA_ARGS__);                   \
            return OT_ERR_ISP_ILLEGAL_PARAM;                               \
        }                                                                  \
    } while (0)

#define isp_max_param_check_return(x, max, fmt, ...)                       \
    do {                                                                   \
        if ((x) > (max)) {                                                 \
            isp_err_trace(fmt, ##__VA_ARGS__);                   \
            return OT_ERR_ISP_ILLEGAL_PARAM;                               \
        }                                                                  \
    } while (0)

#define isp_array_param_check_return(x, type, size, min, max, fmt, ...)                 \
    do {                                                                                \
        type *p = (type *)x;                                                            \
        for (int loops = 0; loops < size; loops++) {                                     \
            if ((p[loops]) < (min) || (p[loops]) > (max)) {                             \
                isp_err_trace(fmt, ##__VA_ARGS__);  \
                return OT_ERR_ISP_ILLEGAL_PARAM;                                        \
            }                                                                           \
        }                                                                               \
    } while (0)

#define isp_array_max_param_check_return(x, type, size, max, fmt, ...)                 \
    do {                                                                               \
        type *p = (type *)x;                                                           \
        for (int loops = 0; loops < size; loops++) {                                   \
            if ((p[loops]) > (max)) {                                                  \
                isp_err_trace(fmt, ##__VA_ARGS__); \
                return OT_ERR_ISP_ILLEGAL_PARAM;                                       \
            }                                                                          \
        }                                                                              \
    } while (0)

#define isp_check_sensor_register_return(pipe)                                              \
    do {                                                                                   \
        isp_usr_ctx *isp_ctx_tmp = TD_NULL;                                                \
        isp_get_ctx(pipe, isp_ctx_tmp);                                                     \
        if ((isp_ctx_tmp->sns_reg != TD_TRUE) && (isp_ctx_tmp->isp_yuv_mode != TD_TRUE)) { \
            isp_err_trace("Sensor doesn't register to ISP[%d]!\n", pipe);                   \
            return OT_ERR_ISP_SNS_UNREGISTER;                                              \
        }                                                                                  \
    } while (0)
#define isp_check_sensor_register_goto(pipe, ret, failed)                                              \
    do {                                                                                   \
        isp_usr_ctx *isp_ctx_tmp = TD_NULL;                                                \
        isp_get_ctx(pipe, isp_ctx_tmp);                                                     \
        if ((isp_ctx_tmp->sns_reg != TD_TRUE) && (isp_ctx_tmp->isp_yuv_mode != TD_TRUE)) { \
            isp_err_trace("Sensor doesn't register to ISP[%d]!\n", pipe);                   \
            (ret) = OT_ERR_ISP_SNS_UNREGISTER;                                              \
            goto failed;                                                                \
        }                                                                                  \
    } while (0)

td_s32 isp_set_calibrate_attr(ot_vi_pipe vi_pipe, ot_isp_fpn_calibrate_attr *calibrate);
td_s32 isp_set_correction_attr(ot_vi_pipe vi_pipe, const ot_isp_fpn_attr *correction);
td_s32 isp_get_correction_attr(ot_vi_pipe vi_pipe, ot_isp_fpn_attr *correction);

td_s32 isp_set_fe_correction_attr(ot_vi_pipe vi_pipe, const ot_isp_fpn_attr *correction);
td_s32 isp_get_fe_correction_attr(ot_vi_pipe vi_pipe, ot_isp_fpn_attr *correction);

td_s32 isp_get_fe_focus_stats(ot_vi_pipe vi_pipe, ot_isp_fe_focus_stats *fe_af_stat, isp_stat *isp_act_stat,
                              td_u8 wdr_chn);
td_s32 isp_get_mg_stats(ot_vi_pipe vi_pipe, ot_isp_mg_stats *mg_stat);
td_s32 isp_get_ae_stitch_stats(ot_vi_pipe vi_pipe, ot_isp_ae_stitch_stats *ae_stitch_stat);
td_s32 isp_get_wb_stitch_stats(ot_vi_pipe vi_pipe, ot_isp_wb_stitch_stats *stitch_wb_stat);
td_s32 isp_mesh_shading_calibration(const td_u16 *src_raw, ot_isp_mlsc_calibration_cfg *mlsc_cali_cfg,
                                    ot_isp_mesh_shading_table *mlsc_table);

td_s32 isp_get_lightbox_gain(ot_vi_pipe vi_pipe, ot_isp_awb_calibration_gain *awb_calibration_gain);
td_s32 isp_set_pipe_differ_attr(ot_vi_pipe vi_pipe, const ot_isp_pipe_diff_attr *pipe_differ);
td_s32 isp_get_pipe_differ_attr(ot_vi_pipe vi_pipe, ot_isp_pipe_diff_attr *pipe_differ);
td_s32 isp_set_rc_attr(ot_vi_pipe vi_pipe, const ot_isp_rc_attr *rc_attr);
td_s32 isp_get_rc_attr(ot_vi_pipe vi_pipe, ot_isp_rc_attr *rc_attr);
td_s32 isp_get_ae_grid_info(ot_vi_pipe vi_pipe, ot_isp_ae_grid_info *fe_grid_info, ot_isp_ae_grid_info *be_grid_info);
td_s32 isp_get_af_grid_info(ot_vi_pipe vi_pipe, ot_isp_focus_grid_info *fe_grid_info,
                            ot_isp_focus_grid_info *be_grid_info);
td_s32 isp_get_wb_grid_info(ot_vi_pipe vi_pipe, ot_isp_awb_grid_info *grid_info);
td_s32 isp_get_mg_grid_info(ot_vi_pipe vi_pipe, ot_isp_mg_grid_info *grid_info);

td_s32 isp_set_clut_coeff(ot_vi_pipe vi_pipe, const ot_isp_clut_lut *clut_lut);
td_s32 isp_get_clut_coeff(ot_vi_pipe vi_pipe, ot_isp_clut_lut *clut_lut);
td_s32 isp_set_clut_attr(ot_vi_pipe vi_pipe, const ot_isp_clut_attr *clut_attr);
td_s32 isp_get_clut_attr(ot_vi_pipe vi_pipe, ot_isp_clut_attr *clut_attr);
td_s32 calc_flicker_type(ot_isp_calc_flicker_input *input_param, ot_isp_calc_flicker_output *output_param,
                         ot_video_frame_info frame[], td_u32 array_size);
td_s32 isp_calc_flicker_type(ot_vi_pipe vi_pipe, ot_isp_calc_flicker_input *input_param,
                             ot_isp_calc_flicker_output *output_param, ot_video_frame_info frame[],
                             td_u32 array_size);
td_s32 isp_ir_auto_run_once(ot_vi_pipe vi_pipe, ot_isp_ir_auto_attr *ir_attr);
td_s32 isp_be_stats_estimate(ot_vi_pipe vi_pipe, td_u32 *estimate_be_hist, ot_isp_fe_ae_stat_1 *fe_ae_stat1,
    const isp_be_stats_calc_info *be_stats_calc_info);
td_s32 isp_get_check_sum_stats(ot_vi_pipe vi_pipe, isp_check_sum_stat *sum_stat);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
