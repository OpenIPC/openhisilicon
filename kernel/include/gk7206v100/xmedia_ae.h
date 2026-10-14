/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AE_H__
#define __XMEDIA_AE_H__

#include "xmedia_type.h"
#include "xmedia_isp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    xmedia_u32 max;
    xmedia_u32 min;
} xmedia_ae_range;

typedef enum {
    XMEDIA_AE_MODE_SLOW_SHUTTER = 0,
    XMEDIA_AE_MODE_FIX_FRAME_RATE,
    XMEDIA_AE_MODE_MAX
} xmedia_ae_mode;

typedef enum {
    XMEDIA_AE_ANTIFLICKER_NORMAL_MODE = 0,
    XMEDIA_AE_ANTIFLICKER_AUTO_MODE,
    XMEDIA_AE_ANTIFLICKER_MODE_MAX
} xmedia_ae_anti_flicker_mode;

typedef struct ISP_ANTIFLICKER_S {
    xmedia_bool                 enable;
    xmedia_u8                   frequency;
    xmedia_ae_anti_flicker_mode mode;
} xmedia_ae_anti_flicker;

typedef struct {
    xmedia_bool enable;
    xmedia_u8   luma_diff;
} xmedia_ae_sub_flicker;

typedef struct {
    xmedia_u16 black_delay_frame;
    xmedia_u16 white_delay_frame;
} xmedia_ae_delay;

typedef struct {
    // base parameter
    xmedia_ae_range        exp_time_range;
    xmedia_ae_range        again_range;
    xmedia_ae_range        dgain_range;
    xmedia_ae_range        isp_dgain_range;
    xmedia_ae_range        sys_gain_range;
    xmedia_u32             gain_thres_hold;
    xmedia_u8              speed;
    xmedia_u16             black_speed_bias;
    xmedia_u8              tolerance;
    xmedia_u8              iso_num;
    xmedia_u32             iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8              compensation[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16             ev_bias[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_isp_ae_strategy ae_strategy_mode;
    xmedia_u16             hist_ratio_slope[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8              max_hist_offset[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_ae_mode         ae_mode;
    xmedia_ae_anti_flicker anti_flicker;
    xmedia_ae_sub_flicker  sub_flicker;
    xmedia_ae_delay        ae_delay_attr;
    xmedia_bool            manual_exp_value;
    xmedia_u32             exp_value;
    xmedia_isp_wdr_mode    wdr_mode;
    xmedia_bool            wdr_quick;
    xmedia_u16             iso_cal_coef;
} xmedia_ae_auto_attr;

typedef struct {
    xmedia_video_operation_mode exp_time_op_type;
    xmedia_video_operation_mode again_op_type;
    xmedia_video_operation_mode dgain_op_type;
    xmedia_video_operation_mode isp_dgain_op_type;
    xmedia_u32                  exp_time;
    xmedia_u32                  again;
    xmedia_u32                  dgain;
    xmedia_u32                  isp_dgain;
} xmedia_ae_manual_attr;

typedef enum {
    XMEDIA_LONG_FRAME = 0,
    XMEDIA_SHORT_FRAME,
    XMEDIA_PRIOR_FRAME_MAX
} xmedia_ae_prior_frame;

typedef struct {
    xmedia_bool                 bypass;
    xmedia_video_operation_mode op_type;
    xmedia_u8                   ae_run_interval;
    xmedia_bool                 hist_stat_adjust;
    xmedia_bool                 ae_route_ex_valid;
    xmedia_ae_prior_frame       prior_frame;
    xmedia_bool                 ae_gain_sep_cfg;
    xmedia_ae_manual_attr       manual_attr;
    xmedia_ae_auto_attr         auto_attr;
} xmedia_ae_exposure_attr;

typedef struct {
    xmedia_video_operation_mode exp_ratio_type;
    xmedia_u32                  exp_ratio[XMEDIA_ISP_WDR_EXP_RATIO_MAX_NUM];
    xmedia_u32                  exp_ratio_max;
    xmedia_u32                  exp_ratio_min;
    xmedia_u16                  tolerance;
    xmedia_u16                  speed;
    xmedia_u16                  ratio_bias;
} xmedia_ae_wdr_exposure_attr;

typedef struct {
    xmedia_s32 kp;
    xmedia_s32 ki;
    xmedia_s32 kd;
    xmedia_u32 min_pwm_duty;
    xmedia_u32 max_pwm_duty;
    xmedia_u32 open_pwm_duty;
} xmedia_ae_dciris_attr;

typedef struct {
    xmedia_u32           hold_value;
    xmedia_isp_iris_f_no iris_f_no;
} xmedia_ae_mi_attr;

typedef enum {
    XMEDIA_AE_IRIS_KEEP = 0,
    XMEDIA_AE_IRIS_OPEN,
    XMEDIA_AE_IRIS_CLOSE,
    XMEDIA_AE_IRIS_MAX
} xmedia_ae_iris_status;

typedef struct {
    xmedia_bool                 enable;
    xmedia_video_operation_mode op_type;
    xmedia_isp_iris_type        iris_type;
    xmedia_ae_iris_status       iris_status;
    xmedia_ae_mi_attr           mi_attr;
} xmedia_ae_iris_attr;

typedef struct {
    xmedia_u32              exp_time;
    xmedia_u32              short_exp_time;
    xmedia_u32              median_exp_time;
    xmedia_u32              long_exp_time;
    xmedia_u32              again;
    xmedia_u32              dgain;
    xmedia_u32              again_sf;
    xmedia_u32              dgain_sf;
    xmedia_u32              isp_dgain;
    xmedia_u32              exposure;
    xmedia_bool             exposure_is_max;
    xmedia_s16              hist_error;
    xmedia_u32              ae_hist[XMEDIA_ISP_AE_HIST_NUM];
    xmedia_u8               avg_luma;
    xmedia_u32              lines_per_500ms;
    xmedia_u32              piris_f_no;
    xmedia_u32              fps;
    xmedia_u32              iso;
    xmedia_u32              iso_sf;
    xmedia_u32              iso_calibrate;
    xmedia_u32              ref_exp_ratio;
    xmedia_u32              first_stable_time;
    xmedia_isp_ae_route     ae_route;
    xmedia_isp_ae_ext_route ae_route_ex;
    xmedia_isp_ae_route     ae_route_sf;
    xmedia_isp_ae_ext_route ae_route_sf_ex;
} xmedia_ae_exp_info;

typedef struct {
    xmedia_bool                 enable;
    xmedia_bool                 ir_mode;
    xmedia_video_operation_mode smart_exp_type;
    xmedia_u8                   iso_num;
    xmedia_u32                  iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                   luma_target[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                  exp_coef[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                  exp_coef_max;
    xmedia_u16                  exp_coef_min;
    xmedia_u8                   smart_interval;
    xmedia_u8                   smart_speed;
    xmedia_u16                  smart_delay_num;
} xmedia_ae_smart_exposure_attr;

typedef struct {
    xmedia_u32                  max_inttime;
    xmedia_u32                  min_inttime;
    xmedia_u32                  max_again;
    xmedia_u32                  min_again;
    xmedia_u32                  max_dgain;
    xmedia_u32                  min_dgain;
    xmedia_u32                  max_isp_dgain;
    xmedia_u32                  min_isp_dgain;
    xmedia_u32                  max_sys_gain;
    xmedia_u32                  min_sys_gain;
    xmedia_u8                   compensation[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                  ev_bias[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_bool                 manual_exposure_en;
    xmedia_video_operation_mode exp_time_op_type;
    xmedia_video_operation_mode again_op_type;
    xmedia_video_operation_mode dgain_op_type;
    xmedia_video_operation_mode isp_dgain_op_type;
    xmedia_u32                  manual_exp_time;
    xmedia_u32                  manual_again;
    xmedia_u32                  manual_dgain;
    xmedia_u32                  manual_ispdgain;
} xmedia_ae_debug_attr;

typedef struct {
    xmedia_u32 frm_num;
    xmedia_u32 full_lines;
    xmedia_u32 exp_time;
    xmedia_u32 short_exp_time;
    xmedia_u32 median_exp_time;
    xmedia_u32 long_exp_time;
    xmedia_u32 again;
    xmedia_u32 dgain;
    xmedia_u32 isp_dgain;
    xmedia_u64 exposure_value;
    xmedia_u32 increment;
    xmedia_s32 hist_error;
    xmedia_s32 hist_ori_avg;
    xmedia_s32 luma_offset;
    xmedia_u32 iso;
    xmedia_u32 exp_ratio;
    xmedia_u32 over_exp_ratio;
    xmedia_u32 over_exp_ratio_filter;
} xmedia_ae_debug_status_attr;

/*
 * 函数功能: 注册ae库到vi
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_EXIST - ae重复注册
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 无可用内存
 */
xmedia_s32 xmedia_ae_register(xmedia_u32 pipe);

/*
 * 函数功能: 注销ae库
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 */
xmedia_s32 xmedia_ae_unregister(xmedia_u32 pipe);

/*
 * 函数功能: 设置ae库debug功能, 打印信息输出到内存
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      debug_info - 用户分配的debug信息输出buffer信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 */
xmedia_s32 xmedia_ae_set_debug_info(xmedia_u32 pipe, xmedia_isp_debug_info *debug_info);

/*
 * 函数功能: 设置ae库debug打印等级, 打印信息输出到串口
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      debug_level - AE库debug打印等级 (1: ERROR  2: WARNING  3: NOTICE  4: INFO  5: DEBUG)
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 */
xmedia_s32 xmedia_ae_set_debug_level(xmedia_u32 pipe, xmedia_u32 debug_level);

/*
 * 函数功能: 设置ae参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_exposure_attr(xmedia_u32 pipe, const xmedia_ae_exposure_attr *exposure_attr);

/*
 * 函数功能: 获取ae参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_exposure_attr(xmedia_u32 pipe, xmedia_ae_exposure_attr *exposure_attr);

/*
 * 函数功能: 设置WDR模式ae参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_wdr_exposure_attr(xmedia_u32 pipe, const xmedia_ae_wdr_exposure_attr *wdr_exp_attr);

/*
 * 函数功能: 获取WDR模式ae参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_wdr_exposure_attr(xmedia_u32 pipe, xmedia_ae_wdr_exposure_attr *wdr_exp_attr);

/*
 * 函数功能: 设置ae曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_route_attr(xmedia_u32 pipe, const xmedia_isp_ae_route *ae_route_attr);

/*
 * 函数功能: 获取ae曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_route_attr(xmedia_u32 pipe, xmedia_isp_ae_route *ae_route_attr);

/*
 * 函数功能: 设置短帧ae曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_route_sf_attr(xmedia_u32 pipe, const xmedia_isp_ae_route *ae_route_sf_attr);

/*
 * 函数功能: 获取短帧ae曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_route_sf_attr(xmedia_u32 pipe, xmedia_isp_ae_route *ae_route_sf_attr);

/*
 * 函数功能: 获取ae内部运行状态信息
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_query_exposure_info(xmedia_u32 pipe, xmedia_ae_exp_info *exp_info);

/*
 * 函数功能: 设置扩展模式曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_ext_route_attr(xmedia_u32 pipe, const xmedia_isp_ae_ext_route *ae_ext_route_attr);

/*
 * 函数功能: 获取扩展模式曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_ext_route_attr(xmedia_u32 pipe, xmedia_isp_ae_ext_route *ae_ext_route_attr);

/*
 * 函数功能: 设置短帧扩展模式曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_ext_route_sf_attr(xmedia_u32 pipe, const xmedia_isp_ae_ext_route *ae_ext_route_attr);

/*
 * 函数功能: 获取短帧扩展模式曝光表参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_ext_route_sf_attr(xmedia_u32 pipe, xmedia_isp_ae_ext_route *ae_ext_route_attr);

/*
 * 函数功能: 设置智能曝光参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_set_smart_exposure_attr(xmedia_u32 pipe, const xmedia_ae_smart_exposure_attr *smart_exp_attr);

/*
 * 函数功能: 获取智能曝光参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      exposure_attr - ae参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_ae_get_smart_exposure_attr(xmedia_u32 pipe, xmedia_ae_smart_exposure_attr *smart_exp_attr);

#ifdef __cplusplus
}
#endif

#endif // __XMEDIA_API_AE_H__
