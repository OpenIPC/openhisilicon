/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_inner.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_INNER_H__
#define __ISP_INNER_H__

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define isp_param_check_return(x, min, max, fmt, ...)                  \
    do {                                                                   \
        if ((x) < (min) || (x) > (max)) {                                  \
            isp_err_trace("%#x " fmt, x);                                  \
            return HI_ERR_ISP_ILLEGAL_PARAM;                               \
        }                                                                  \
    } while (0)

#define isp_max_param_check_return(x, max, fmt, ...)                   \
    do {                                                                   \
        if ((x) > (max)) {                                                 \
            isp_err_trace("%#x " fmt,x);                                   \
            return HI_ERR_ISP_ILLEGAL_PARAM;                               \
        }                                                                  \
    } while (0)

#define isp_array_param_check_return(x, type, size, min, max, fmt, ...)     \
    do {                                                                        \
        type *p = (type *)x;                                                    \
        for(int loops = 0; loops < size; loops++) {                             \
            if ((p[loops]) < (min) || (p[loops]) > (max)) {                     \
                isp_err_trace("array[%d] = %#x " fmt, loops, p[loops]);         \
                return HI_ERR_ISP_ILLEGAL_PARAM;                                \
            }                                                                   \
        }                                                                       \
    } while (0)

#define isp_array_max_param_check_return(x, type, size, max, fmt, ...)      \
    do {                                                                        \
        type *p = (type *)x;                                                    \
        for (int loops = 0; loops < size; loops++) {                            \
            if ((p[loops]) > (max)) {                                           \
                isp_err_trace("array[%d] = %#x " fmt, loops, p[loops]);         \
                return HI_ERR_ISP_ILLEGAL_PARAM;                                \
            }                                                                   \
        }                                                                       \
    } while (0)

#define isp_check_sensor_register_return(dev)                                             \
    do {                                                                           \
        isp_usr_ctx *isp_ctx_tmp = HI_NULL;                                            \
        isp_get_ctx(dev, isp_ctx_tmp);                                                 \
        if ((isp_ctx_tmp->sns_reg != HI_TRUE) && (isp_ctx_tmp->isp_yuv_mode != HI_TRUE)) { \
            isp_err_trace("Sensor doesn't register to ISP[%d]!\n", dev);           \
            return HI_ERR_ISP_SNS_UNREGISTER;                                      \
        }                                                                          \
    } while (0)

#ifdef _MSC_VER
#define mutex_init_lock(mutex) InitializeCriticalSection(&(mutex))
#define mutex_lock(mutex) EnterCriticalSection(&(mutex))
#define mutex_unlock(mutex) LeaveCriticalSection(&(mutex))
#define mutex_destroy(mutex) DeleteCriticalSection(&(mutex))
#else
#define mutex_init_lock(mutex)                  \
    do {                                        \
        (void)pthread_mutex_init(&(mutex), HI_NULL); \
    } while (0)
#define mutex_lock(mutex)                 \
    do {                                  \
        (void)pthread_mutex_lock(&(mutex)); \
    } while (0)
#define mutex_unlock(mutex)                 \
    do {                                    \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)
#define mutex_destroy(mutex)                 \
    do {                                     \
        (void)pthread_mutex_destroy(&(mutex)); \
    } while (0)
#endif

hi_s32 isp_set_calibrate_attr(hi_vi_pipe vi_pipe, hi_isp_fpn_calibrate_attr *calibrate);
hi_s32 isp_set_correction_attr(hi_vi_pipe vi_pipe, const hi_isp_fpn_attr *correction);
hi_s32 isp_get_correction_attr(hi_vi_pipe vi_pipe, hi_isp_fpn_attr *correction);
hi_s32 isp_get_fe_focus_statistics(hi_vi_pipe vi_pipe, hi_isp_fe_focus_statistics *fe_af_stat, isp_stat *isp_act_stat,
                                   hi_u8 wdr_chn);

hi_s32 isp_get_ae_stitch_statistics(hi_vi_pipe vi_pipe, hi_isp_ae_stitch_statistics *ae_stitch_stat);
hi_s32 isp_get_wb_stitch_statistics(hi_vi_pipe vi_pipe, hi_isp_wb_stitch_statistics *stitch_wb_stat);
hi_s32 isp_mesh_shading_calibration(hi_u16 *src_raw, hi_isp_mlsc_calibration_cfg *mlsc_cali_cfg,
                                    hi_isp_mesh_shading_table *mlsc_table);

hi_s32 isp_get_lightbox_gain(hi_vi_pipe vi_pipe, hi_isp_awb_calibration_gain *awb_calibration_gain);
hi_s32 isp_set_radial_shading_attr(hi_vi_pipe vi_pipe, const hi_isp_radial_shading_attr *radial_shading_attr);
hi_s32 isp_get_radial_shading_attr(hi_vi_pipe vi_pipe, hi_isp_radial_shading_attr *radial_shading_attr);
hi_s32 isp_set_radial_shading_lut(hi_vi_pipe vi_pipe, const hi_isp_radial_shading_lut_attr *radial_shading_lut_attr);
hi_s32 isp_get_radial_shading_lut(hi_vi_pipe vi_pipe, hi_isp_radial_shading_lut_attr *radial_shading_lut_attr);
hi_s32 isp_set_pipe_differ_attr(hi_vi_pipe vi_pipe, const hi_isp_pipe_diff_attr *pipe_differ);
hi_s32 isp_get_pipe_differ_attr(hi_vi_pipe vi_pipe, hi_isp_pipe_diff_attr *pipe_differ);
hi_s32 isp_set_rc_attr(hi_vi_pipe vi_pipe, const hi_isp_rc_attr *rc_attr);
hi_s32 isp_get_rc_attr(hi_vi_pipe vi_pipe, hi_isp_rc_attr *rc_attr);
hi_s32 isp_set_rgbir_attr(hi_vi_pipe vi_pipe, const hi_isp_rgbir_attr *rgbir_attr);
hi_s32 isp_get_rgbir_attr(hi_vi_pipe vi_pipe, hi_isp_rgbir_attr *rgbir_attr);
hi_s32 isp_set_pre_log_lut_attr(hi_vi_pipe vi_pipe, const hi_isp_preloglut_attr *pre_log_lut_attr);
hi_s32 isp_get_pre_log_lut_attr(hi_vi_pipe vi_pipe, hi_isp_preloglut_attr *pre_log_lut_attr);
hi_s32 isp_set_log_lut_attr(hi_vi_pipe vi_pipe, const hi_isp_loglut_attr *log_lut_attr);
hi_s32 isp_get_log_lut_attr(hi_vi_pipe vi_pipe, hi_isp_loglut_attr *log_lut_attr);
hi_s32 isp_get_ae_grid_info(hi_vi_pipe vi_pipe, hi_isp_ae_grid_info *fe_grid_info, hi_isp_ae_grid_info *be_grid_info);
hi_s32 isp_get_af_grid_info(hi_vi_pipe vi_pipe, hi_isp_focus_grid_info *fe_grid_info,
                            hi_isp_focus_grid_info *be_grid_info);
hi_s32 isp_get_wb_grid_info(hi_vi_pipe vi_pipe, hi_isp_awb_grid_info *grid_info);
hi_s32 isp_get_mg_grid_info(hi_vi_pipe vi_pipe, hi_isp_mg_grid_info *grid_info);

hi_s32 isp_set_clut_coeff(hi_vi_pipe vi_pipe, const hi_isp_clut_lut *clut_lut);
hi_s32 isp_get_clut_coeff(hi_vi_pipe vi_pipe, hi_isp_clut_lut *clut_lut);
hi_s32 isp_set_clut_attr(hi_vi_pipe vi_pipe, const hi_isp_clut_attr *clut_attr);
hi_s32 isp_get_clut_attr(hi_vi_pipe vi_pipe, hi_isp_clut_attr *clut_attr);
hi_s32 calc_flicker_type(hi_isp_calc_flicker_input *input_param, hi_isp_calc_flicker_output *output_param,
                         hi_video_frame_info frame[], hi_u32 array_size);
hi_s32 isp_set_raw_pos(hi_vi_pipe vi_pipe, const hi_isp_raw_pos_attr *raw_pos_attr);
hi_s32 isp_get_raw_pos(hi_vi_pipe vi_pipe, hi_isp_raw_pos_attr *raw_pos_attr);
hi_s32 isp_calc_flicker_type(hi_vi_pipe vi_pipe, hi_isp_calc_flicker_input *input_param,
                             hi_isp_calc_flicker_output *output_param, hi_video_frame_info frame[],
                             hi_u32 array_size);
hi_s32 isp_ir_auto_run_once(hi_vi_pipe vi_pipe, hi_isp_ir_auto_attr *ir_attr);
#ifdef CONFIG_HI_ISP_RLSC_ONLINE_CALIB_SUPPORT
hi_s32 isp_radial_shading_calibration(hi_u16 *data, hi_isp_rlsc_calibration_cfg *rlsc_cali_cfg,
                                      hi_isp_radial_shading_table *rlsc_table);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
