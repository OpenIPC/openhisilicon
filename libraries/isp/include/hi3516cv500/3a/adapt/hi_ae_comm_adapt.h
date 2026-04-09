/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of hi_ae_comm_adapt.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __HI_AE_COMM_ADAPT_H__
#define __HI_AE_COMM_ADAPT_H__

#include "hi_type.h"
#include "hi_debug_adapt.h"
#include "hi_comm_isp_adapt.h"
#include "hi_ae_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef AE_CTRL_CMD_E hi_isp_ae_ctrl_cmd;

typedef struct {
    hi_u32  max_int_time;
    hi_u32  min_int_time;
    hi_u32  max_a_gain;
    hi_u32  min_a_gain;
    hi_u32  max_d_gain;
    hi_u32  min_d_gain;
    hi_u32  max_isp_d_gain;
    hi_u32  min_isp_d_gain;
    hi_u32  max_sys_gain;
    hi_u32  min_sys_gain;
    hi_u32  compensation;
    hi_u32  ev_bias;
    hi_bool manual_exposure_en;
    hi_bool manual_time_en;
    hi_bool manual_again_en;
    hi_bool manual_dgain_en;
    hi_bool manual_isp_dgain_en;
    hi_u32  manual_exposure_lines;
    hi_u32  manual_again;
    hi_u32  manual_dgain;
    hi_u32  manual_isp_dgain;
    hi_u32  ae_weights[(AE_ZONE_ROW) * (AE_ZONE_COLUMN)];
} hi_isp_ae_dbg_attr;

typedef struct {
    hi_u32  frm_num_bgn;
    hi_u32  full_lines;
    hi_u32  int_time;
    hi_u32  short_int_time;
    hi_u32  med_int_time;
    hi_u32  long_int_time;
    hi_u32  again;
    hi_u32  dgain;
    hi_u32  isp_dgain;
    hi_u32  iris_fno_lin;
    hi_u64  exposure;
    hi_u32  increment;
    hi_s32  hist_error;
    hi_s32  hist_ori_average;
    hi_s32  luma_offset;
    hi_u32  iso;
    hi_u32  exp_ratio;
    hi_u32  over_exp_ratio;
    hi_u32  over_exp_ratio_filter;
} hi_isp_ae_dbg_status;

typedef AE_ACCURACY_E hi_isp_ae_accuracy_type;

typedef struct {
    hi_isp_ae_accuracy_type accu_type;
    float   accuracy;
    float   offset;
} hi_isp_ae_accuracy;

typedef struct {
    hi_bool quick_start_enable;
    hi_u8 black_frame_num;
    hi_bool ir_mode_en;
    hi_u32 init_exposure_ir;
    hi_u32 iso_thr_ir;
    hi_u16 ir_cut_delay_time;
} hi_isp_quick_start_param;

typedef struct {
    hi_u8   hist_thresh[HIST_THRESH_NUM];
    hi_u8   ae_compensation;

    hi_u32  lines_per500ms;
    hi_u32  flicker_freq;
    hi_float fps;
    hi_u32  hmax_times; /* unit is ns */
    hi_u32  init_exposure;
    hi_u32  init_ae_speed;
    hi_u32  init_ae_tolerance;

    hi_u32  full_lines_std;
    hi_u32  full_lines_max;
    hi_u32  full_lines;
    hi_u32  binning_full_lines;
    hi_u32  max_int_time;     /* RW;unit is line */
    hi_u32  min_int_time;
    hi_u32  max_int_time_target;
    hi_u32  min_int_time_target;
    hi_isp_ae_accuracy int_time_accu;

    hi_u32  max_again;
    hi_u32  min_again;
    hi_u32  max_again_target;
    hi_u32  min_again_target;
    hi_isp_ae_accuracy again_accu;

    hi_u32  max_dgain;
    hi_u32  min_dgain;
    hi_u32  max_dgain_target;
    hi_u32  min_dgain_target;
    hi_isp_ae_accuracy dgain_accu;

    hi_u32  max_isp_dgain_target;
    hi_u32  min_isp_dgain_target;
    hi_u32  isp_dgain_shift;

    hi_u32  max_int_time_step;
    hi_bool max_time_step_enable;
    hi_u32  max_inc_time_step[WDR_MAX_FRAME_NUM];
    hi_u32  max_dec_time_step[WDR_MAX_FRAME_NUM];
    hi_u32  lf_max_short_time;
    hi_u32  lf_min_exposure;

    hi_isp_ae_route ae_route_attr;
    hi_bool ae_route_ex_valid;
    hi_isp_ae_route_ex ae_route_attr_ex;

    hi_isp_ae_route ae_route_sf_attr;
    hi_isp_ae_route_ex ae_route_sf_attr_ex;

    hi_u16 man_ratio_enable;
    hi_u32 arr_ratio[EXP_RATIO_NUM];

    hi_isp_iris_type  iris_type;
    hi_isp_piris_attr piris_attr;
    hi_isp_iris_f_no  max_iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                        Max F number of Piris's aperture, it's related to the specific iris */
    hi_isp_iris_f_no  min_iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                        Min F number of Piris's aperture, it's related to the specific iris */

    hi_isp_ae_strategy ae_exp_mode;

    hi_u16 iso_cal_coef;
    hi_u8  ae_run_interval;
    hi_u32 exp_ratio_max;
    hi_u32 exp_ratio_min;
    hi_bool diff_gain_support;
    hi_isp_quick_start_param quick_start;
    hi_isp_prior_frame prior_frame;
    hi_bool ae_gain_sep_cfg;
    hi_bool lhcg_support;
    hi_u32 sns_lhcg_exp_ratio;
} hi_isp_ae_sensor_default;

typedef struct {
    hi_isp_fswdr_mode fswdr_mode;
} hi_isp_ae_fswdr_attr;

typedef struct {
    hi_u32 reg_addr;
    hi_u32 reg_value;
} hi_isp_ae_param_reg;

typedef struct {
    hi_u32 tar_fps;
    hi_u32 exp_time;
    hi_u32 exp_again;
    hi_u32 exp_dgain;
    hi_u32 exp_isp_dgain;
    hi_isp_ae_param_reg time_reg[10]; /* 10 */
    hi_isp_ae_param_reg again_reg[10]; /* 10 */
    hi_isp_ae_param_reg dgain_reg[10]; /* 10 */
} hi_isp_ae_convert_param;

typedef struct {
    hi_s32 (*pfn_cmos_get_ae_default)(hi_vi_pipe vi_pipe, hi_isp_ae_sensor_default *ae_sns_dft);

    /* the function of sensor set fps */
    hi_void (*pfn_cmos_fps_set)(hi_vi_pipe vi_pipe, hi_float f32_fps, hi_isp_ae_sensor_default *ae_sns_dft);
    hi_void (*pfn_cmos_slow_framerate_set)(hi_vi_pipe vi_pipe, hi_u32 full_lines, hi_isp_ae_sensor_default *ae_sns_dft);

    /* while isp notify ae to update sensor regs, ae call these funcs. */
    hi_void (*pfn_cmos_inttime_update)(hi_vi_pipe vi_pipe, hi_u32 int_time);
    hi_void (*pfn_cmos_gains_update)(hi_vi_pipe vi_pipe, hi_u32 again, hi_u32 dgain);

    hi_void (*pfn_cmos_again_calc_table)(hi_vi_pipe vi_pipe, hi_u32 *again_lin, hi_u32 *again_db);
    hi_void (*pfn_cmos_dgain_calc_table)(hi_vi_pipe vi_pipe, hi_u32 *dgain_lin, hi_u32 *dgain_db);

    hi_void (*pfn_cmos_get_inttime_max)(hi_vi_pipe vi_pipe, hi_u16 man_ratio_enable,
        hi_u32 *ratio, hi_u32 *int_time_max, hi_u32 *int_time_min, hi_u32 *lf_max_int_time);

    /* long frame mode set */
    hi_void (*pfn_cmos_ae_fswdr_attr_set)(hi_vi_pipe vi_pipe, hi_isp_ae_fswdr_attr *ae_fswdr_attr);
    hi_void (*pfn_cmos_ae_quick_start_status_set)(hi_vi_pipe vi_pipe, hi_bool quick_start_status);
    hi_void (*pfn_cmos_exp_param_convert)(hi_vi_pipe vi_pipe, hi_isp_ae_convert_param *exp_param);
} hi_isp_ae_sensor_exp_func;

typedef struct {
    hi_isp_ae_sensor_exp_func sns_exp;
} hi_isp_ae_sensor_register;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
