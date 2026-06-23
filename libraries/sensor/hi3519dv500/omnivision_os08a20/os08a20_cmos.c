/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "os08a20_cmos_ex.h"
#include "os08a20_cmos.h"

#define OS08A20_ID                    8
#define SENSOR_OS08A20_WIDTH          3840
#define SENSOR_OS08A20_HEIGHT         2160

#define higher_4bits(x) (((x) & 0xf0000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define os08a20_sensor_set_ctx(pipe, ctx)   ((g_os08a20_sns_state[pipe]) = (ctx))
#define os08a20_sensor_reset_ctx(pipe)      (g_os08a20_sns_state[pipe] = TD_NULL)

static ot_isp_fswdr_mode g_fswdr_mode[OT_ISP_MAX_PIPE_NUM] = {
    [0 ... OT_ISP_MAX_PIPE_NUM - 1] = OT_ISP_FSWDR_NORMAL_MODE
};

static td_u32 g_max_time_get_cnt[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u32 g_init_exposure[OT_ISP_MAX_PIPE_NUM]  = {0};
static td_u32 g_init_int_time[OT_ISP_MAX_PIPE_NUM]  = {0};
static td_u32 g_init_again[OT_ISP_MAX_PIPE_NUM]  = {0};
static td_u32 g_init_dgain[OT_ISP_MAX_PIPE_NUM]  = {0};
static td_u32 g_init_isp_dgain[OT_ISP_MAX_PIPE_NUM]  = {0};
static td_u32 g_lines_per500ms[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u8 g_ae_stat_pos[OT_ISP_MAX_PIPE_NUM] = {0};

static td_u16 g_init_wb_gain[OT_ISP_MAX_PIPE_NUM][OT_ISP_RGB_CHN_NUM] = {{0}};
static td_u16 g_sample_r_gain[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u16 g_sample_b_gain[OT_ISP_MAX_PIPE_NUM] = {0};
static td_bool g_quick_start_en[OT_ISP_MAX_PIPE_NUM] = {TD_FALSE};

static td_bool g_ae_route_ex_valid[OT_ISP_MAX_PIPE_NUM] = {0};
static ot_isp_ae_route g_init_ae_route[OT_ISP_MAX_PIPE_NUM] = {{0}};
static ot_isp_ae_route_ex g_init_ae_route_ex[OT_ISP_MAX_PIPE_NUM] = {{0}};
static ot_isp_ae_route g_init_ae_route_sf[OT_ISP_MAX_PIPE_NUM] = {{0}};
static ot_isp_ae_route_ex g_init_ae_route_sf_ex[OT_ISP_MAX_PIPE_NUM] = {{0}};

ot_isp_sns_commbus g_os08a20_bus_info[OT_ISP_MAX_PIPE_NUM] = {
    [0] = { .i2c_dev = 0},
    [1 ... OT_ISP_MAX_PIPE_NUM - 1] = { .i2c_dev = -1}
};

ot_isp_sns_state *g_os08a20_sns_state[OT_ISP_MAX_PIPE_NUM] = {TD_NULL};

static td_bool blc_clamp_info[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = TD_TRUE};

ot_isp_sns_commbus *os08a20_get_bus_info(ot_vi_pipe vi_pipe)
{
    return &g_os08a20_bus_info[vi_pipe];
}

ot_isp_sns_state *os08a20_get_ctx(ot_vi_pipe vi_pipe)
{
    return g_os08a20_sns_state[vi_pipe];
}

td_void os08a20_set_blc_clamp_value(ot_vi_pipe vi_pipe, td_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}

const os08a20_video_mode_tbl g_os08a20_mode_tbl[OS08A20_MODE_BUTT] = {
    {OS08A20_VMAX_8M_30FPS_12BIT_LINEAR,   OS08A20_FULL_LINES_MAX,          30, 1.06,
     3840, 2180, 0, OT_WDR_MODE_NONE,       "OS08A20_8M_30FPS_12BIT_LINEAR_MODE"},
    {OS08A20_VMAX_8M_30FPS_10BIT_2TO1_WDR,  OS08A20_FULL_LINES_MAX_2TO1_WDR, 30, 5.0,
     3840, 2180, 0, OT_WDR_MODE_2To1_LINE,  "OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* Os08a20 Register Address */
#define OS08A20_EXPO_H_ADDR          0x3501
#define OS08A20_EXPO_L_ADDR          0x3502
#define OS08A20_AGAIN_H_ADDR         0x3508
#define OS08A20_AGAIN_L_ADDR         0x3509
#define OS08A20_DGAIN_H_ADDR         0x350a
#define OS08A20_DGAIN_L_ADDR         0x350b
#define OS08A20_SHORT_EXPO_H_ADDR    0x3511
#define OS08A20_SHORT_EXPO_L_ADDR    0x3512
#define OS08A20_SHORT_AGAIN_H_ADDR   0x350c
#define OS08A20_SHORT_AGAIN_L_ADDR   0x350d
#define OS08A20_SHORT_DGAIN_H_ADDR   0x350e
#define OS08A20_SHORT_DGAIN_L_ADDR   0x350f
#define OS08A20_VMAX_H_ADDR          0x380e
#define OS08A20_VMAX_L_ADDR          0x380f
#define OS08A20_MARGIN               4


#define os08a20_err_mode_print(sns_image_mode, sns_state) \
do { \
    isp_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                  (sns_image_mode)->width, \
                  (sns_image_mode)->height, \
                  (sns_image_mode)->fps, \
                  (sns_state)->wdr_mode); \
} while (0)

static td_void cmos_get_ae_comm_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = OS08A20_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns, 30fps */

    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = 0.0078125; /* accuracy: 0.0078125 */

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 128 << ae_sns_dft->isp_dgain_shift; /* max 128 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = sns_state->fl_std * 30 / 2; /* 30fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }
    (td_void)memcpy_s(&ae_sns_dft->piris_attr, sizeof(ot_isp_piris_attr), &g_piris, sizeof(ot_isp_piris_attr));
    ae_sns_dft->max_iris_fno = OT_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid = TD_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = g_quick_start_en[vi_pipe];
    ae_sns_dft->quick_start.black_frame_num = 0;
    ae_sns_dft->ae_stat_pos = g_ae_stat_pos[vi_pipe]; /* 1 use be stat to AE */
    return;
}

static td_void cmos_get_ae_linear_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_again = 15872; /* max 15872 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 128; /* max 128 */
    ae_sns_dft->min_dgain = 128;  /* min 128 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */
    ae_sns_dft->init_int_time = g_init_int_time[vi_pipe];
    ae_sns_dft->init_again = g_init_again[vi_pipe];
    ae_sns_dft->init_dgain = g_init_dgain[vi_pipe];
    ae_sns_dft->init_isp_dgain = g_init_isp_dgain[vi_pipe];

    ae_sns_dft->max_int_time = sns_state->fl_std - 10; /* sub 10 */
    ae_sns_dft->min_int_time = 8; /* min int 8 */
    ae_sns_dft->max_int_time_target = 65535; /* max int 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe], sizeof(ot_isp_ae_route_ex));

    return;
}

static td_void cmos_get_ae_2to1_line_wdr_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_int_time = sns_state->fl_std - 10; /* sub 10 */
    ae_sns_dft->min_int_time = 2; /* min_int_time 2 */
    ae_sns_dft->int_time_accu.offset = -0.115; /* -0.115 line for stagger */

    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again = 15872; /* max 15872 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 2047; /* max 2047 */
    ae_sns_dft->min_dgain = 128;  /* min 128 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->max_isp_dgain_target = 4 << ae_sns_dft->isp_dgain_shift; /* max 4 << shift */
    ae_sns_dft->diff_gain_support = TD_TRUE;

    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 52000; /* init 52000 */

    if (g_fswdr_mode[vi_pipe] == OT_ISP_FSWDR_LONG_FRAME_MODE) {
        ae_sns_dft->ae_compensation = 56; /* ae_compensation 56 */
        ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation = 32; /* ae_compensation 32 */
        ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = TD_TRUE;
        ae_sns_dft->arr_ratio[0] = 0x400;
        ae_sns_dft->arr_ratio[1] = 0x40;
        ae_sns_dft->arr_ratio[2] = 0x40; /* array index 2 */
    }
    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe],  sizeof(ot_isp_ae_route_ex));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_sf_attr, sizeof(ot_isp_ae_route),
                      &g_init_ae_route_sf[vi_pipe], sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_sf_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &g_init_ae_route_sf_ex[vi_pipe],  sizeof(ot_isp_ae_route_ex));
    return;
}

static td_s32 cmos_get_ae_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(ae_sns_dft);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route), 0, sizeof(ot_isp_ae_route));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        case OT_WDR_MODE_2To1_LINE:
            cmos_get_ae_2to1_line_wdr_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        default:
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;
    }

    return TD_SUCCESS;
}

static td_void cmos_config_vmax(ot_isp_sns_state *sns_state, td_u32 vmax)
{
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);
    } else {
        sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].data = high_8bits(vmax);
    }

    return;
}

/* the function of sensor set fps */
static td_bool cmos_2to1_vmax_limit(td_u32 *vmax, td_u32 full_line, td_u32 step, td_bool fps_up)
{
    if (fps_up) {
        if ((*vmax) + step < full_line) {
            (*vmax) = (full_line - step);
            return TD_FALSE;
        }
    } else {
        if ((*vmax) > full_line + step) {
            (*vmax) = (full_line + step);
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines, lines_max, vmax;
    td_float max_fps, min_fps;
    td_bool achieve_fps_ok;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_void_return(ae_sns_dft);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_os08a20_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_os08a20_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_os08a20_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_os08a20_mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        isp_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    achieve_fps_ok = TD_TRUE;
    vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        achieve_fps_ok = cmos_2to1_vmax_limit(&vmax, sns_state->fl[0], 154, fps > ae_sns_dft->fps); /* step 154 */
    }
    vmax = (vmax > lines_max) ? lines_max : vmax;

    cmos_config_vmax(sns_state, vmax);

    sns_state->fl_std = vmax;
    ae_sns_dft->lines_per500ms = lines * 15; /* *15 */
    g_lines_per500ms[vi_pipe] = ae_sns_dft->lines_per500ms;

    ae_sns_dft->fps = (achieve_fps_ok == TD_TRUE) ? fps : (lines * max_fps / vmax);
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - 10; /* sub 10 */
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (td_u32)((1000000000) / (sns_state->fl_std * div_0_to_1_float(fps))); /* 1000000000ns */

    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines_max;
    td_u32 vmax;
    td_bool achieve_fps_ok;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_void_return(ae_sns_dft);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines_max = g_os08a20_mode_tbl[sns_state->img_mode].max_ver_lines;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        vmax = full_lines;
        achieve_fps_ok =
            cmos_2to1_vmax_limit(&vmax, sns_state->fl[0], 154, full_lines < sns_state->fl[0]); /* step 154 */
    } else {
        vmax = full_lines;
    }

    vmax = (vmax > lines_max) ? lines_max : vmax;
    sns_state->fl[0] = vmax;

    ot_unused(achieve_fps_ok);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(sns_state->fl[0]);
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(sns_state->fl[0]);
            break;
        case OT_WDR_MODE_2To1_LINE:
            sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].data = low_8bits(sns_state->fl[0]);
            sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].data = high_8bits(sns_state->fl[0]);
            break;
        default:
            break;
    }

    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->max_int_time = sns_state->fl[0] - 10; /* max_int_time: Flstd - 10 */

    return;
}

static td_void cmos_inttime_update_linear(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 value;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    value = int_time;

    if (g_quick_start_en[vi_pipe] == TD_TRUE) {
        os08a20_write_register(vi_pipe, OS08A20_EXPO_L_ADDR, low_8bits(value));
        os08a20_write_register(vi_pipe, OS08A20_EXPO_H_ADDR, high_8bits(value));
    } else {
        sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = low_8bits(value);
        sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = high_8bits(value);
    }

    return;
}

static td_void cmos_inttime_update_2to1_line(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    static td_bool is_first[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = 1};

    static td_u32 short_int_time[OT_ISP_MAX_PIPE_NUM] = {0};
    static td_u32 long_int_time[OT_ISP_MAX_PIPE_NUM] = {0};

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (is_first[vi_pipe]) { /* short exposure */
        sns_state->wdr_int_time[0] = int_time;
        short_int_time[vi_pipe] = int_time;
        is_first[vi_pipe] = TD_FALSE;
    } else { /* long exposure */
        sns_state->wdr_int_time[1] = int_time;
        long_int_time[vi_pipe] = int_time;

        sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = low_8bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = high_8bits(long_int_time[vi_pipe]);

        sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_L_IDX].data = low_8bits(short_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_H_IDX].data = high_8bits(short_int_time[vi_pipe]);
        is_first[vi_pipe] = TD_TRUE;
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}

#define GAIN_NODE_NUM 64
static td_u32 g_gain_table[GAIN_NODE_NUM] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856, 1920, 1984, 2048, 2176,
    2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096, 4352, 4608, 4864,
    5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936, 8192, 8704, 9216, 9728, 10240, 10752,
    11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
    int i;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    ot_unused(vi_pipe);

    if (*again_lin >= g_gain_table[GAIN_NODE_NUM - 1]) {
        *again_lin = g_gain_table[GAIN_NODE_NUM - 1];
        *again_db = GAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < GAIN_NODE_NUM; i++) {
        if (*again_lin < g_gain_table[i]) {
            *again_lin = g_gain_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }
    return;
}

static td_void cmos_gains_regs_set(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state, td_u32 again_reg, td_u32 dgain_reg)
{
    static td_bool first_gain[OT_ISP_MAX_PIPE_NUM] = { [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = 1 };

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        if (g_quick_start_en[vi_pipe] == TD_TRUE) {
            os08a20_write_register(vi_pipe, OS08A20_AGAIN_L_ADDR, (again_reg & 0xff));
            os08a20_write_register(vi_pipe, OS08A20_AGAIN_H_ADDR, ((again_reg & 0x3f00) >> 8)); /* shift 8 */
            os08a20_write_register(vi_pipe, OS08A20_DGAIN_L_ADDR, (dgain_reg & 0xff));
            os08a20_write_register(vi_pipe, OS08A20_DGAIN_H_ADDR, ((dgain_reg & 0x3f00) >> 8)); /* shift 8 */
        } else {
            sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = (again_reg & 0xff);
            sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8); /* shift 8 */
            sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = (dgain_reg & 0xff);
            sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8); /* shift 8 */
            }
    } else if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        if (first_gain[vi_pipe] == TD_TRUE) { /* long frame */
            sns_state->regs_info[0].i2c_data[WDR_AGAIN_L_IDX].data = (again_reg & 0xff);
            sns_state->regs_info[0].i2c_data[WDR_AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8); /* shift 8 */
            sns_state->regs_info[0].i2c_data[WDR_DGAIN_L_IDX].data = (dgain_reg & 0xff);
            sns_state->regs_info[0].i2c_data[WDR_DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8); /* shift 8 */

            first_gain[vi_pipe] = TD_FALSE;
        } else {                              /* short frame */
            sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_L_IDX].data = (again_reg & 0xff);
            sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8); /* shift 8 */
            sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_L_IDX].data = (dgain_reg & 0xff);
            sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8); /* shift 8 */

            first_gain[vi_pipe] = TD_TRUE;
        }
    }

    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 again_reg, dgain_reg;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    again_reg = g_gain_table[again];
    again_reg = clip3((again_reg >> 3), 0x80, 0x7C0); /* shift 3 */

    if (dgain < 0x80) {
        dgain_reg = 0x400;
    } else if (dgain < 0x800) {
        dgain_reg = dgain * 0x8;
    } else {
        dgain_reg = 0x3FFF;
    }

    cmos_gains_regs_set(vi_pipe, sns_state, again_reg, dgain_reg);

    return;
}

static td_void cmos_clip_ratio_range(td_u32 *a_exp_ratio, const ot_isp_ae_int_time_range *int_time,
    td_u32 wdr_frame_num)
{
    td_u32 i, ratio_min, ratio_max;
    for (i = 0; i < wdr_frame_num - 1; ++i) {
        ratio_max = int_time->int_time_max[i + 1] * 0x40 / int_time->int_time_min[i];
        ratio_min = int_time->int_time_min[i + 1] * 0x40 / int_time->int_time_max[i];
        a_exp_ratio[i] = MIN2(MAX2(a_exp_ratio[i], ratio_min), ratio_max);
        a_exp_ratio[i] = MIN2(MAX2(a_exp_ratio[i], 0x40), 0x4000);
    }
}

static td_void cmos_step_limit(ot_isp_ae_int_time_range *int_time,
    const time_step *step, td_u32 full_lines, td_u32 wdr_frame_num)
{
    td_u32 i;
    td_u32 max_int_time = 0;
    for (i = 0; i < wdr_frame_num; ++i) {
        if (int_time->pre_int_time[i] == 0) {
            return ;
        }
    }
    for (i = 0; i < wdr_frame_num; ++i) {
        if (step->inc[i] > 0) {
            int_time->int_time_max[i] = MIN2(int_time->pre_int_time[i] + step->inc[i], int_time->int_time_max[i]);
        }
        if (step->dec[i] > 0) {
            if (int_time->pre_int_time[i] > step->dec[i]) {
                int_time->int_time_min[i] = MAX2(int_time->pre_int_time[i] - step->dec[i], int_time->int_time_min[i]);
            } else {
                int_time->int_time_min[i] = MAX2(int_time->int_time_min[i], 0);
            }
        }
        if (int_time->int_time_min[i] > int_time->int_time_max[i]) {
            int_time->int_time_max[i] = int_time->int_time_min[i];
        }
        /* make sure LEF > SEF1 and SEF1 > SEF2 */
        if (i > 0) {
            int_time->int_time_max[i] = MAX2(int_time->int_time_max[i], int_time->int_time_max[i - 1]);
            int_time->int_time_min[i] = MAX2(int_time->int_time_min[i], int_time->int_time_min[i - 1]);
        }
    }
    /* make sure max_int_time < full_lines */
    for (i = 0; i < wdr_frame_num; ++i) {
        max_int_time += int_time->int_time_max[i];
    }
    if (max_int_time > full_lines) {
        max_int_time = max_int_time - full_lines;
        for (i = 0; i < wdr_frame_num; ++i) {
            if (int_time->int_time_max[i] - int_time->int_time_min[i] > max_int_time) {
                int_time->int_time_max[i] = int_time->int_time_max[i] - max_int_time;
                return;
            }
        }
    }
    return;
}
static td_void cmos_set_2to1_long_frame_range(ot_isp_ae_int_time_range *int_time,
                                              td_s32 time_min, td_s32 time_max, td_s32 step)
{
    int_time->int_time_max[0] = MAX2((td_s32)int_time->pre_int_time[0] - step, time_min);    /* index 0 */
    int_time->int_time_min[0] = MAX2((td_s32)int_time->pre_int_time[0] - step, time_min);    /* index 0 */
    int_time->int_time_max[1] = time_max;                                                       /* index 2 */
    int_time->int_time_min[1] = time_min;                                                       /* index 2 */
}


static td_void cmos_get_inttime_max_2to1_line(ot_vi_pipe vi_pipe, td_u32 *ratio,
                                              ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    td_u32 short_max0, i, short_max, short_time_min_limit;
    ot_isp_sns_state *sns_state = TD_NULL;
    time_step step = {{0}};
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
    static td_u32 pre_ratio[OT_ISP_MAX_PIPE_NUM] = {[0 ... OT_ISP_MAX_PIPE_NUM - 1] = 0X40};
    short_time_min_limit = 2; /* short_time_min_limit 2 */

    for (i = 0; i < 2; ++i) { /* frame number is 2 */
        step.inc[i] = (sns_state->fl[1] - g_os08a20_mode_tbl[OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE].height) >> 1;
        step.dec[i] = (sns_state->fl[1] - g_os08a20_mode_tbl[OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE].height) >> 1;
    }

    if (g_fswdr_mode[vi_pipe] == OT_ISP_FSWDR_LONG_FRAME_MODE) {
        short_max0 = sns_state->fl[1] - OS08A20_MARGIN - step.dec[0] - sns_state->wdr_int_time[0];
        short_max = sns_state->fl[0] - OS08A20_MARGIN - step.dec[0];
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        int_time->int_time_max[0] = short_time_min_limit;
        int_time->int_time_min[0] = short_time_min_limit;
        int_time->int_time_max[1] = short_max;
        int_time->int_time_min[1] = short_time_min_limit;
        cmos_set_2to1_long_frame_range(int_time, short_time_min_limit, short_max, 90); /* step 90 */
    } else {
        short_max0 = ((sns_state->fl[1] - OS08A20_MARGIN - step.dec[0] - sns_state->wdr_int_time[0]) * 0x40) /
            div_0_to_1(ratio[0]);
        short_max = ((sns_state->fl[0] - OS08A20_MARGIN - step.inc[0]) * 0x40) / (ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        *lf_max_int_time = sns_state->fl[0] - OS08A20_MARGIN - step.inc[0];

        if (short_max >= short_time_min_limit) {
            int_time->int_time_max[0] = short_max;
            int_time->int_time_max[1] = (int_time->int_time_max[0] * ratio[0]) >> 6; /* shift 6 */
            int_time->int_time_min[0] = short_time_min_limit;
            int_time->int_time_min[1] = (int_time->int_time_min[0] * ratio[0]) >> 6; /* shift 6 */
        } else {
            short_max = short_time_min_limit;
            int_time->int_time_max[0] = short_max;
            int_time->int_time_max[1] = (int_time->int_time_max[0] * 0xFFF) >> 6; /* shift 6 */
            int_time->int_time_min[0] = int_time->int_time_max[0];
            int_time->int_time_min[1] = int_time->int_time_max[1];
        }

        if (ratio[0] != pre_ratio[vi_pipe]) {
            cmos_step_limit(int_time, &step, sns_state->fl[1], 2); /* frame number is 2 */
            cmos_clip_ratio_range(ratio, (const ot_isp_ae_int_time_range *)int_time, 2); /* frame number is 2 */
        }
    }
    pre_ratio[vi_pipe] = ratio[0];
    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    ot_unused(man_ratio_enable);
    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_2To1_LINE:
            cmos_get_inttime_max_2to1_line(vi_pipe, ratio, int_time, lf_max_int_time);
            break;
        default:
            break;
    }

    return;
}

/* Only used in LINE_WDR mode */
static td_void cmos_ae_fswdr_attr_set(ot_vi_pipe vi_pipe, ot_isp_ae_fswdr_attr *ae_fswdr_attr)
{
    sns_check_pointer_void_return(ae_fswdr_attr);

    g_fswdr_mode[vi_pipe] = ae_fswdr_attr->fswdr_mode;
    g_max_time_get_cnt[vi_pipe] = 0;

    return;
}

static td_void cmos_ae_quick_start_status_set(ot_vi_pipe vi_pipe, td_bool quick_start_en)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    g_quick_start_en[vi_pipe] = quick_start_en;
    sns_state->sync_init = TD_FALSE;
}

static td_s32 cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    sns_check_pointer_return(exp_func);

    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));

    exp_func->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set           = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update    = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update      = cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    exp_func->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;
    exp_func->pfn_cmos_ae_quick_start_status_set = cmos_ae_quick_start_status_set;

    return TD_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP                         5000
#define CALIBRATE_STATIC_WB_R_GAIN                    492
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    443

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              16
#define CALIBRATE_AWB_P2                              240
#define CALIBRATE_AWB_Q1                              0
#define CALIBRATE_AWB_A1                              188580
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-139106)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static td_s32 cmos_get_awb_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(awb_sns_dft);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(awb_sns_dft, sizeof(ot_isp_awb_sensor_default), 0, sizeof(ot_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP; /* wb_ref_temp 4950 */

    awb_sns_dft->gain_offset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
    awb_sns_dft->gain_offset[3] = CALIBRATE_STATIC_WB_B_GAIN; /* index 3 */

    awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1; /* index 2 */
    awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1; /* index 3 */
    awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1; /* index 4 */
    awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1; /* index 5 */

    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm, sizeof(ot_isp_awb_ccm));
            (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table),
                              &g_awb_agc_table, sizeof(ot_isp_awb_agc_table));
            break;

        case OT_WDR_MODE_2To1_FRAME:
        case OT_WDR_MODE_2To1_LINE:
            (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm_wdr, sizeof(ot_isp_awb_ccm));
            (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table),
                              &g_awb_agc_table_wdr, sizeof(ot_isp_awb_agc_table));

            break;

        default:
            (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm, sizeof(ot_isp_awb_ccm));
            (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table),
                              &g_awb_agc_table, sizeof(ot_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
    awb_sns_dft->init_ggain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
    awb_sns_dft->init_bgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
    awb_sns_dft->sample_rgain = g_sample_r_gain[vi_pipe];
    awb_sns_dft->sample_bgain = g_sample_b_gain[vi_pipe];

    return TD_SUCCESS;
}

static td_s32 cmos_init_awb_exp_function(ot_isp_awb_sensor_exp_func *exp_func)
{
    sns_check_pointer_return(exp_func);

    (td_void)memset_s(exp_func, sizeof(ot_isp_awb_sensor_exp_func), 0, sizeof(ot_isp_awb_sensor_exp_func));

    exp_func->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return TD_SUCCESS;
}

static ot_isp_cmos_dng_color_param g_dng_color_param = {{ 286, 256, 608 }, { 415, 256, 429 },
    { 2810, { 0x01AC, 0x8093, 0x8019, 0x8070, 0x01EA, 0x807A, 0x802A, 0x80F3, 0x021D }},
    { 4940, { 0x01D7, 0x8084, 0x8053, 0x8053, 0x01D9, 0x8086, 0x8010, 0x80B3, 0x01C3 }}};

static td_void cmos_get_isp_dng_default(const ot_isp_sns_state *sns_state, ot_isp_cmos_default *isp_def)
{
    (td_void)memcpy_s(&isp_def->dng_color_param, sizeof(ot_isp_cmos_dng_color_param), &g_dng_color_param,
                      sizeof(ot_isp_cmos_dng_color_param));

    switch (sns_state->img_mode) {
        case OS08A20_8M_30FPS_12BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 12; /* 12bit */
            isp_def->sns_mode.dng_raw_format.white_level = 4095; /* max 4095 */
            break;

        case OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 10; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = 1023; /* max 1023 */
            break;

        default:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 12; /* 12bit */
            isp_def->sns_mode.dng_raw_format.white_level = 4095; /* max 4095 */
            break;
    }

    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_hor.denominator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_hor.numerator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_ver.denominator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_ver.numerator = 1;
    isp_def->sns_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_row = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_col = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.black_level_repeat_dim.repeat_row = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.black_level_repeat_dim.repeat_col = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.cfa_layout = OT_ISP_CFALAYOUT_TYPE_RECTANGULAR;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[2] = 2; /* index 2, cfa_plane_color 2 */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[2] = 1; /* index 2, cfa_pattern 1 */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[3] = 2; /* index 3, cfa_pattern 2 */
    isp_def->sns_mode.valid_dng_raw_format = TD_TRUE;

    return;
}

static void cmos_get_isp_linear_default(ot_isp_cmos_default *isp_def)
{
    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen;
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc;
    isp_def->key.bit1_bayer_nr         = 1;
    isp_def->bayer_nr                  = &g_cmos_bayer_nr;
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_cac              = 1;
    isp_def->cac                       = &g_cmos_cac;
    isp_def->key.bit1_bshp             = 1;
    isp_def->bshp                      = &g_cmos_bayershp;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    isp_def->key.bit1_ca = 0;
    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
    return;
}

static void cmos_get_isp_wdr_default(ot_isp_cmos_default *isp_def)
{
    isp_def->key.bit1_dpc            = 1;
    isp_def->dpc                     = &g_cmos_dpc_wdr;
    isp_def->key.bit1_demosaic       = 1;
    isp_def->demosaic                = &g_cmos_demosaic_wdr;
    isp_def->key.bit1_sharpen        = 1;
    isp_def->sharpen                 = &g_cmos_yuv_sharpen_wdr;
    isp_def->key.bit1_drc            = 1;
    isp_def->drc                     = &g_cmos_drc_wdr;
    isp_def->key.bit1_gamma          = 1;
    isp_def->gamma                   = &g_cmos_gamma_wdr;
#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma       = 1;
    isp_def->pregamma                = &g_cmos_pregamma;
#endif
    isp_def->key.bit1_bayer_nr       = 1;
    isp_def->bayer_nr                = &g_cmos_bayer_nr_wdr;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge             = 1;
    isp_def->ge                      = &g_cmos_ge_wdr;
#endif
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color = &g_cmos_anti_false_color_wdr;
    isp_def->key.bit1_cac = 1;
    isp_def->cac = &g_cmos_cac_wdr;
    isp_def->key.bit1_bshp= 1;
    isp_def->bshp = &g_cmos_bayershp_wdr;
    isp_def->key.bit1_ldci = 1;
    isp_def->ldci = &g_cmos_ldci_wdr;
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze_wdr;
    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
    return;
}

static td_s32 cmos_get_isp_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(isp_def);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(isp_def, sizeof(ot_isp_cmos_default), 0, sizeof(ot_isp_cmos_default));
#ifdef CONFIG_OT_ISP_CA_SUPPORT
    isp_def->key.bit1_ca      = 0;
#endif
    isp_def->key.bit1_clut    = 1;
    isp_def->clut             = &g_cmos_clut;

    isp_def->key.bit1_dpc     = 1;
    isp_def->dpc              = &g_cmos_dpc;

    isp_def->key.bit1_wdr     = 1;
    isp_def->wdr              = &g_cmos_wdr;

    isp_def->key.bit1_lsc      = 0;
    isp_def->lsc               = &g_cmos_lsc;

    isp_def->key.bit1_acs      = 0;
    isp_def->acs               = &g_cmos_acs;

#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma = 1;
    isp_def->pregamma          = &g_cmos_pregamma;
#endif
    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            cmos_get_isp_linear_default(isp_def);
            break;
        case OT_WDR_MODE_2To1_LINE:
            cmos_get_isp_wdr_default(isp_def);
            break;
        default:
            cmos_get_isp_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x400;

    isp_def->sns_mode.sns_id = OS08A20_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_get_isp_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_get_isp_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32  i;
    ot_isp_sns_state *sns_state = TD_NULL;
    const ot_isp_cmos_black_level *cmos_blc_def = TD_NULL;

    sns_check_pointer_return(black_level);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        cmos_blc_def = &g_cmos_blc;
    } else {
        cmos_blc_def = &g_cmos_blc_wdr;
    }

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), cmos_blc_def, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
            black_level->auto_attr.black_level[0][i] = 0x400;
        }
    } else { /* black level of DOL mode */
        for (i = 0; i < OT_ISP_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = 0x400;
            black_level->auto_attr.black_level[i][1] = 0x400;
            black_level->auto_attr.black_level[i][2] = 0x400; /* index 2 */
            black_level->auto_attr.black_level[i][3] = 0x400; /* index 3 */
        }
    }

    return TD_SUCCESS;
}

static td_s32 cmos_get_isp_blc_clamp_info(ot_vi_pipe vi_pipe, td_bool *blc_clamp_en)
{
    sns_check_pointer_return(blc_clamp_en);

    *blc_clamp_en = blc_clamp_info[vi_pipe];

    return TD_SUCCESS;
}

static td_void cmos_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    td_u32 full_lines_5fps;
    ot_isp_sns_state *sns_state = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == OS08A20_8M_30FPS_12BIT_LINEAR_MODE) {
            full_lines_5fps = (OS08A20_VMAX_8M_30FPS_12BIT_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    if (enable) { /* setup for ISP pixel calibration mode */
        os08a20_write_register(vi_pipe, OS08A20_AGAIN_L_ADDR, 0x80);
        os08a20_write_register(vi_pipe, OS08A20_AGAIN_H_ADDR, 0x00);

        os08a20_write_register(vi_pipe, OS08A20_DGAIN_L_ADDR, 0x00);
        os08a20_write_register(vi_pipe, OS08A20_DGAIN_H_ADDR, 0x04);

        os08a20_write_register(vi_pipe, OS08A20_VMAX_L_ADDR, low_8bits(full_lines_5fps));
        os08a20_write_register(vi_pipe, OS08A20_VMAX_H_ADDR, high_8bits(full_lines_5fps));

        os08a20_write_register(vi_pipe, OS08A20_EXPO_L_ADDR, low_8bits(full_lines_5fps - 10));
        os08a20_write_register(vi_pipe, OS08A20_EXPO_H_ADDR, high_8bits(full_lines_5fps - 10));
    } else { /* setup for ISP 'normal mode' */
        sns_state->fl_std = (sns_state->fl_std > OS08A20_FULL_LINES_MAX) ? OS08A20_FULL_LINES_MAX : sns_state->fl_std;
        os08a20_write_register(vi_pipe, OS08A20_VMAX_L_ADDR, low_8bits(sns_state->fl_std));
        os08a20_write_register(vi_pipe, OS08A20_VMAX_H_ADDR, high_8bits(sns_state->fl_std));
        sns_state->sync_init = TD_FALSE;
    }

    return;
}

static td_s32 cmos_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case OT_WDR_MODE_2To1_LINE:
            sns_state->wdr_mode = OT_WDR_MODE_2To1_LINE;
            printf("2to1 line WDR 4k mode(60fps->30fps)\n");
            break;

        default:
            isp_err_trace("NOT support this mode!\n");
            return TD_FAILURE;
    }

    (td_void)memset_s(sns_state->wdr_int_time, sizeof(sns_state->wdr_int_time), 0, sizeof(sns_state->wdr_int_time));

    return TD_SUCCESS;
}

static td_void cmos_comm_sns_reg_info_init(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    sns_state->regs_info[0].sns_type = OT_ISP_SNS_TYPE_I2C;
    sns_state->regs_info[0].com_bus.i2c_dev = g_os08a20_bus_info[vi_pipe].i2c_dev;
    if (g_quick_start_en[vi_pipe]) {
        sns_state->regs_info[0].cfg2_valid_delay_max = 1; /* delay_max 1 */
    } else {
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }
    sns_state->regs_info[0].reg_num = REG_MAX_IDX;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].reg_num = WDR_REG_MAX_IDX;
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = OS08A20_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = OS08A20_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = OS08A20_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = OS08A20_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = OS08A20_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = OS08A20_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].reg_addr = OS08A20_AGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = OS08A20_DGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = OS08A20_DGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = OS08A20_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = OS08A20_VMAX_H_ADDR;

    return;
}

static td_void cmos_2to1_line_wdr_sns_reg_info_init(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_L_IDX].reg_addr = OS08A20_SHORT_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_H_IDX].reg_addr = OS08A20_SHORT_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_L_IDX].reg_addr = OS08A20_SHORT_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_H_IDX].reg_addr = OS08A20_SHORT_AGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_L_IDX].reg_addr = OS08A20_SHORT_DGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_H_IDX].reg_addr = OS08A20_SHORT_DGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].reg_addr = OS08A20_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].reg_addr = OS08A20_VMAX_H_ADDR;

    return;
}

static td_void cmos_sns_reg_info_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    ot_unused(vi_pipe);

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        if (sns_state->regs_info[0].i2c_data[i].data ==
            sns_state->regs_info[1].i2c_data[i].data) {
            sns_state->regs_info[0].i2c_data[i].update = TD_FALSE;
        } else {
            sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        }
    }

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        if (sns_state->fl[0] > sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].delay_frame_num = 1;
            sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].delay_frame_num = 1;
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].delay_frame_num = 2; /* delay 2 */
            sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].delay_frame_num = 2; /* delay 2 */
        } else {
        }
    } else {
        if (sns_state->fl[0] > sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0;
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0;
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 1; /* delay 1 */
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 1; /* delay 1 */
        } else {
        }
    }

    return;
}

static td_s32 cmos_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(sns_regs_info);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == TD_FALSE) || (sns_regs_info->config == TD_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);

        if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
            /* DOL 2t1 Mode Regs */
            cmos_2to1_line_wdr_sns_reg_info_init(vi_pipe, sns_state);
        }
        sns_state->sync_init = TD_TRUE;
    } else {
        cmos_sns_reg_info_update(vi_pipe, sns_state);
    }

    sns_regs_info->config = TD_FALSE;
    (td_void)memcpy_s(sns_regs_info, sizeof(ot_isp_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info));
    (td_void)memcpy_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info));
    sns_state->fl[1] = sns_state->fl[0];

    return TD_SUCCESS;
}

static td_void cmos_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sns_image_mode,
    ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    switch (sns_image_mode) {
        case OS08A20_8M_30FPS_12BIT_LINEAR_MODE:
            sns_state->fl_std = OS08A20_VMAX_8M_30FPS_12BIT_LINEAR;
            break;
        case OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE:
            sns_state->fl_std = OS08A20_VMAX_8M_30FPS_10BIT_2TO1_WDR;
            break;
        default:
            sns_state->fl_std = OS08A20_VMAX_8M_30FPS_12BIT_LINEAR;
            break;
    }

    return;
}

static td_s32 cmos_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{
    td_u32 i;
    td_u8 image_mode;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_return(sns_image_mode);
    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < OS08A20_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_os08a20_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_os08a20_mode_tbl[i].width &&
            sns_image_mode->height <= g_os08a20_mode_tbl[i].height &&
            sns_state->wdr_mode == g_os08a20_mode_tbl[i].wdr_mode) {
            image_mode = (os08a20_res_mode)i;
            break;
        }
    }

    if (i >= OS08A20_MODE_BUTT) {
        os08a20_err_mode_print(sns_image_mode, sns_state);
        return TD_FAILURE;
    }

    cmos_config_image_mode_param(vi_pipe, image_mode, sns_state);

    if ((sns_state->init == TD_TRUE) && (image_mode == sns_state->img_mode)) {
        return OT_ISP_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch image_mode */
    }

    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];

    return TD_SUCCESS;
}

static td_void sensor_global_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = OS08A20_8M_30FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = OS08A20_VMAX_8M_30FPS_12BIT_LINEAR;
    sns_state->fl[0]     = OS08A20_VMAX_8M_30FPS_12BIT_LINEAR;
    sns_state->fl[1]     = OS08A20_VMAX_8M_30FPS_12BIT_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_s32 cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    sns_check_pointer_return(sensor_exp_func);

    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init         = os08a20_init;
    sensor_exp_func->pfn_cmos_sns_exit         = os08a20_exit;
    sensor_exp_func->pfn_cmos_sns_global_init  = sensor_global_init;
    sensor_exp_func->pfn_cmos_set_image_mode      = cmos_set_image_mode;
    sensor_exp_func->pfn_cmos_set_wdr_mode        = cmos_set_wdr_mode;
    sensor_exp_func->pfn_cmos_get_isp_default     = cmos_get_isp_default;
    sensor_exp_func->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    sensor_exp_func->pfn_cmos_get_blc_clamp_info  = cmos_get_isp_blc_clamp_info;
    sensor_exp_func->pfn_cmos_set_pixel_detect    = cmos_set_pixel_detect;
    sensor_exp_func->pfn_cmos_get_sns_reg_info    = cmos_get_sns_regs_info;

    return TD_SUCCESS;
}

static td_s32 os08a20_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    g_os08a20_bus_info[vi_pipe].i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 sensor_ctx_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == TD_NULL) {
        sns_state_ctx = (ot_isp_sns_state *)malloc(sizeof(ot_isp_sns_state));
        if (sns_state_ctx == TD_NULL) {
            isp_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_ERR_ISP_NOMEM;
        }
    }

    (td_void)memset_s(sns_state_ctx, sizeof(ot_isp_sns_state), 0, sizeof(ot_isp_sns_state));

    os08a20_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return TD_SUCCESS;
}

static td_void sensor_ctx_exit(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    os08a20_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    os08a20_sensor_reset_ctx(vi_pipe);

    return;
}

static td_s32 sensor_register_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib, ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;
    ot_isp_sns_register isp_register;
    ot_isp_ae_sensor_register ae_register;
    ot_isp_awb_sensor_register awb_register;
    ot_isp_sns_attr_info sns_attr_info;

    sns_check_pointer_return(ae_lib);
    sns_check_pointer_return(awb_lib);

    ret = sensor_ctx_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sns_attr_info.sns_id = OS08A20_ID;
    ret = cmos_init_sensor_exp_function(&isp_register.sns_exp);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cmos init exp function failed!\n");
        return TD_FAILURE;
    }
    ret = ot_mpi_isp_sensor_reg_callback(vi_pipe, &sns_attr_info, &isp_register);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor register callback function failed!\n");
        return ret;
    }

    ret = cmos_init_ae_exp_function(&ae_register.sns_exp);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cmos init ae exp function failed!\n");
        return TD_FAILURE;
    }
    ret = ot_mpi_ae_sensor_reg_callback(vi_pipe, ae_lib, &sns_attr_info, &ae_register);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor register callback function to ae lib failed!\n");
        return ret;
    }

    ret = cmos_init_awb_exp_function(&awb_register.sns_exp);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cmos init awb exp function failed!\n");
        return TD_FAILURE;
    }
    ret = ot_mpi_awb_sensor_reg_callback(vi_pipe, awb_lib, &sns_attr_info, &awb_register);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor register callback function to awb lib failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sensor_unregister_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib, ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;

    sns_check_pointer_return(ae_lib);
    sns_check_pointer_return(awb_lib);

    ret = ot_mpi_isp_sensor_unreg_callback(vi_pipe, OS08A20_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = ot_mpi_ae_sensor_unreg_callback(vi_pipe, ae_lib, OS08A20_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_mpi_awb_sensor_unreg_callback(vi_pipe, awb_lib, OS08A20_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(vi_pipe);
    return TD_SUCCESS;
}

static td_s32 sensor_set_init(ot_vi_pipe vi_pipe, ot_isp_init_attr *init_attr)
{
    sns_check_pointer_return(init_attr);

    g_init_exposure[vi_pipe]  = init_attr->exposure;
    g_init_int_time[vi_pipe]  = init_attr->exp_time;
    g_init_again[vi_pipe]     = init_attr->a_gain;
    g_init_dgain[vi_pipe]     = init_attr->d_gain;
    g_init_isp_dgain[vi_pipe] = init_attr->ispd_gain;
    g_lines_per500ms[vi_pipe] = init_attr->lines_per500ms;
    g_init_wb_gain[vi_pipe][0] = init_attr->wb_r_gain; /* 0: rgain */
    g_init_wb_gain[vi_pipe][1] = init_attr->wb_g_gain; /* 1: ggain */
    g_init_wb_gain[vi_pipe][2] = init_attr->wb_b_gain; /* 2: bgain */
    g_sample_r_gain[vi_pipe] = init_attr->sample_r_gain;
    g_sample_b_gain[vi_pipe] = init_attr->sample_b_gain;
    g_quick_start_en[vi_pipe] = init_attr->quick_start_en;
    g_ae_stat_pos[vi_pipe]       = init_attr->ae_stat_pos;
    g_ae_route_ex_valid[vi_pipe] = init_attr->ae_route_ex_valid;
    (td_void)memcpy_s(&g_init_ae_route[vi_pipe], sizeof(ot_isp_ae_route),
                      &init_attr->ae_route, sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&g_init_ae_route_ex[vi_pipe], sizeof(ot_isp_ae_route_ex),
                      &init_attr->ae_route_ex, sizeof(ot_isp_ae_route_ex));
    (td_void)memcpy_s(&g_init_ae_route_sf[vi_pipe], sizeof(ot_isp_ae_route),
                      &init_attr->ae_route_sf, sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&g_init_ae_route_sf_ex[vi_pipe], sizeof(ot_isp_ae_route_ex),
                      &init_attr->ae_route_sf_ex, sizeof(ot_isp_ae_route_ex));

    return TD_SUCCESS;
}

ot_isp_sns_obj g_sns_os08a20_obj = {
    .pfn_register_callback     = sensor_register_callback,
    .pfn_un_register_callback  = sensor_unregister_callback,
    .pfn_standby               = os08a20_standby,
    .pfn_restart               = os08a20_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = os08a20_blc_clamp,
    .pfn_write_reg             = os08a20_write_register,
    .pfn_read_reg              = os08a20_read_register,
    .pfn_set_bus_info          = os08a20_set_bus_info,
    .pfn_set_init              = sensor_set_init
};
