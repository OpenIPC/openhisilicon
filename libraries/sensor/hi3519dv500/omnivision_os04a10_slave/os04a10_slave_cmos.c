/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "securec.h"
#include "os04a10_slave_cmos_ex.h"
#include "os04a10_slave_cmos.h"

#define OS04A10_SLAVE_ID                    4
#define SENSOR_OS04A10_SLAVE_WIDTH          2688
#define SENSOR_OS04A10_SLAVE_HEIGHT         1520
#define INCK_ONCE_INCREASE_MAX              750000

#define higher_4bits(x) (((x) & 0xf0000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)
#define low_2bits(x)  ((x) & 0x0003)

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define os04a10_slave_sensor_set_ctx(dev, ctx)   ((g_os04a10_slave_sns_state[dev]) = (ctx))
#define os04a10_slave_sensor_reset_ctx(dev)      (g_os04a10_slave_sns_state[dev] = TD_NULL)

static ot_isp_fswdr_mode g_fswdr_mode[OT_ISP_MAX_PIPE_NUM] = {
    [0 ... OT_ISP_MAX_PIPE_NUM - 1] = OT_ISP_FSWDR_NORMAL_MODE
};
static td_u8 g_ae_stat_pos[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u32 g_max_time_get_cnt[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u32 g_init_exposure[OT_ISP_MAX_PIPE_NUM]  = {0};
static td_u32 g_lines_per500ms[OT_ISP_MAX_PIPE_NUM] = {0};

static td_u16 g_init_wb_gain[OT_ISP_MAX_PIPE_NUM][OT_ISP_RGB_CHN_NUM] = {{0}};
static td_u16 g_sample_r_gain[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u16 g_sample_b_gain[OT_ISP_MAX_PIPE_NUM] = {0};
static td_bool g_quick_start_en[OT_ISP_MAX_PIPE_NUM] = {TD_FALSE};

static td_bool g_ae_route_ex_valid[OT_ISP_MAX_PIPE_NUM] = {0};
static ot_isp_ae_route g_init_ae_route[OT_ISP_MAX_PIPE_NUM] = {{0}};
static ot_isp_ae_route_ex g_init_ae_route_ex[OT_ISP_MAX_PIPE_NUM] = {{0}};
static ot_isp_ae_route g_init_ae_route_sf[OT_ISP_MAX_PIPE_NUM] = {{0}};
static ot_isp_ae_route_ex g_init_ae_route_sf_ex[OT_ISP_MAX_PIPE_NUM] = {{0}};

ot_isp_sns_commbus g_os04a10_slave_bus_info[OT_ISP_MAX_PIPE_NUM] = {
    [0] = { .i2c_dev = 0 },
    [1 ... OT_ISP_MAX_PIPE_NUM - 1] = { .i2c_dev = -1 }
};

td_s32 g_os04a10_slave_bind_dev[OT_ISP_MAX_PIPE_NUM] = {0, 0, 1, 1, 2, 2, 3, 3};
static td_u32 g_os04a10_slave_sensor_mode_time[OT_ISP_MAX_PIPE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
ot_isp_sns_state *g_os04a10_slave_sns_state[OT_ISP_MAX_PIPE_NUM] = {TD_NULL};

static td_bool blc_clamp_info[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = TD_TRUE};
ot_isp_slave_sns_sync g_os04a10_slave_sync[OT_ISP_MAX_PIPE_NUM];
ot_isp_sns_commbus *os04a10_slave_get_bus_info(ot_vi_pipe vi_pipe)
{
    return &g_os04a10_slave_bus_info[vi_pipe];
}

ot_isp_sns_state *os04a10_slave_get_ctx(ot_vi_pipe vi_pipe)
{
    return g_os04a10_slave_sns_state[vi_pipe];
}

os04a10_slave_video_mode_tbl g_os04a10_slave_mode_tbl[OS04A10_SLAVE_MODE_BUTT] = {
    {
        24000000, 400, 800000, OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR, OS04A10_SLAVE_FULL_LINES_MAX, 30,
        5, 2688, 1520, 0, OT_WDR_MODE_NONE,    "OS04A10_SLAVE_4M_30FPS_12BIT_LINEAR_MODE"
    },
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* Os04a10 Register Address */
#define OS04A10_SLAVE_EXPO_L_ADDR    0x3502
#define OS04A10_SLAVE_EXPO_H_ADDR    0x3501
#define OS04A10_SLAVE_AGAIN_L_ADDR   0x3509
#define OS04A10_SLAVE_AGAIN_H_ADDR   0x3508
#define OS04A10_SLAVE_DGAIN_L_ADDR   0x350c
#define OS04A10_SLAVE_DGAIN_M_ADDR   0x350b
#define OS04A10_SLAVE_DGAIN_H_ADDR   0x350a
#define OS04A10_SLAVE_VMAX_L_ADDR    0x380f
#define OS04A10_SLAVE_VMAX_H_ADDR    0x380e
#define OS04A10_SLAVE_HCG_1_ADDR     0x376C
#define OS04A10_SLAVE_GROUP_1_ADDR   0x320d
#define OS04A10_SLAVE_GROUP_2_ADDR   0x3208
#define OS04A10_SLAVE_GROUP_3_ADDR   0x3208
#define OS04A10_SLAVE_GROUP_4_ADDR   0x320d
#define OS04A10_SLAVE_GROUP_5_ADDR   0x320a
#define OS04A10_SLAVE_GROUP_6_ADDR   0x320e


#define os04a10_slave_err_mode_print(sensor_image_mode, sns_state) \
do { \
    isp_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                  (sensor_image_mode)->width, \
                  (sensor_image_mode)->height, \
                  (sensor_image_mode)->fps, \
                  (sns_state)->wdr_mode); \
} while (0)

os04a10_slave_video_mode_tbl *os04a10_get_slave_mode_tbl(td_u8 img_mode)
{
    return &g_os04a10_slave_mode_tbl[img_mode];
}

ot_isp_slave_sns_sync *os04a10_get_slave_sync(ot_vi_pipe vi_pipe)
{
    return &g_os04a10_slave_sync[vi_pipe];
}

td_s32 os04a10_get_slave_bind_dev(ot_vi_pipe vi_pipe)
{
    return g_os04a10_slave_bind_dev[vi_pipe];
}

td_u32 os04a10_get_slave_sensor_mode_time(ot_vi_pipe vi_pipe)
{
    return g_os04a10_slave_sensor_mode_time[vi_pipe];
}

ot_isp_sns_state *os04a10_get_slave_sns_state(ot_vi_pipe vi_pipe)
{
    return g_os04a10_slave_sns_state[vi_pipe];
}

static td_void cmos_get_ae_comm_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = OS04A10_SLAVE_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns, 30fps */

    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0625; /* accuracy: 0.0625 */

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = 0.0009765625; /* accuracy: 0.0009765625 */

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 32 << ae_sns_dft->isp_dgain_shift; /* max 32 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = sns_state->fl_std * 30 / 2; /* 30fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }

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
    ae_sns_dft->max_again = 992; /* max 992 */
    ae_sns_dft->min_again = 16;  /* min 16 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 1024; /* max 1024 */
    ae_sns_dft->min_dgain = 1024;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */

    ae_sns_dft->max_int_time = sns_state->fl_std - 8; /* sub 8 */
    ae_sns_dft->min_int_time = 1; /* min int 1 */
    ae_sns_dft->max_int_time_target = 65535; /* max int 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe], sizeof(ot_isp_ae_route_ex));
    return;
}

static td_s32 cmos_get_ae_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(ae_sns_dft);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route), 0, sizeof(ot_isp_ae_route));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
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
    }

    return;
}

/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines, lines_max, vmax, inck, new_inck;
    td_float max_fps, min_fps;
    ot_isp_sns_state *sns_state = TD_NULL;
    static td_u32 last_inck[OT_ISP_MAX_PIPE_NUM] = {0};

    sns_check_pointer_void_return(ae_sns_dft);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_os04a10_slave_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_os04a10_slave_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_os04a10_slave_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_os04a10_slave_mode_tbl[sns_state->img_mode].min_fps;
    inck = g_os04a10_slave_mode_tbl[sns_state->img_mode].inck;

    if ((fps > max_fps) || (fps < min_fps)) {
        isp_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    new_inck = (td_u32)(inck / div_0_to_1_float(fps));
    if (last_inck[vi_pipe] == 0) {
        last_inck[vi_pipe] = new_inck;
    } else {
        if (new_inck > last_inck[vi_pipe] && (new_inck - last_inck[vi_pipe]) > INCK_ONCE_INCREASE_MAX) {
            new_inck = last_inck[vi_pipe] + INCK_ONCE_INCREASE_MAX;
            fps = (td_float)inck / (td_float)new_inck;
        } else if (new_inck < last_inck[vi_pipe] && (last_inck[vi_pipe] - new_inck) > INCK_ONCE_INCREASE_MAX) {
            new_inck = last_inck[vi_pipe] - INCK_ONCE_INCREASE_MAX;
            fps = (td_float)inck / (td_float)new_inck;
        }
        last_inck[vi_pipe] = new_inck;
    }
    g_os04a10_slave_sync[vi_pipe].vs_time = new_inck;

    vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
    vmax = (vmax > lines_max) ? lines_max : vmax;
    cmos_config_vmax(sns_state, vmax);
    sns_state->fl_std = vmax;
    sns_state->regs_info[0].slv_sync.slave_vs_time = g_os04a10_slave_sync[vi_pipe].vs_time;
    ae_sns_dft->lines_per500ms = lines * 15; /* *15 */

    ae_sns_dft->fps = fps;
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
    td_u32 lines, lines_max, inck, new_inck;
    td_u32 vmax;
    td_float fps, fps_max;
    ot_isp_sns_state *sns_state = TD_NULL;
    static td_u32 last_inck[OT_ISP_MAX_PIPE_NUM] = {0};
    sns_check_pointer_void_return(ae_sns_dft);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_os04a10_slave_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_os04a10_slave_mode_tbl[sns_state->img_mode].max_ver_lines;
    inck = g_os04a10_slave_mode_tbl[sns_state->img_mode].inck;
    fps_max = g_os04a10_slave_mode_tbl[sns_state->img_mode].max_fps;

    vmax = full_lines;
    vmax = (vmax > lines_max) ? lines_max : vmax;
    fps = lines * fps_max / (td_float)full_lines;
    new_inck = (td_u32)(inck / div_0_to_1_float(fps));
    if (last_inck[vi_pipe] == 0) {
        last_inck[vi_pipe] = g_os04a10_slave_sync[vi_pipe].vs_time;
    }
    if (new_inck > last_inck[vi_pipe] && (new_inck - last_inck[vi_pipe]) > INCK_ONCE_INCREASE_MAX) {
        new_inck = last_inck[vi_pipe] + INCK_ONCE_INCREASE_MAX;
        fps = (td_float)inck / (td_float)new_inck;
        vmax = (td_u32)(lines * fps_max / div_0_to_1_float(fps));
    }
    last_inck[vi_pipe] = new_inck;
    sns_state->fl[0] = vmax;
    g_os04a10_slave_sync[vi_pipe].vs_time = new_inck;
    sns_state->regs_info[0].slv_sync.slave_vs_time = g_os04a10_slave_sync[vi_pipe].vs_time;

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(sns_state->fl[0]);
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(sns_state->fl[0]);
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

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = low_8bits(int_time);
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = high_8bits(int_time);

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}

static td_void cmos_gains_regs_set(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state,
                                   td_u32 again_reg, td_u32 dgain_reg, td_u32 hcg_1_reg)
{
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = ((again_reg & 0xf) << 4); /* shift 4 */
        sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].data = ((again_reg & 0xf0) >> 4); /* shift 4 */
        sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = ((dgain_reg & 0x03) << 6); /* shift 6 */
        sns_state->regs_info[0].i2c_data[DGAIN_M_IDX].data = ((dgain_reg & 0x03FC) >> 2); /* shift 2 */
        sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = ((dgain_reg & 0x3C00) >> 10); /* shift 10 */
        sns_state->regs_info[0].i2c_data[HCG_1_IDX].data = (hcg_1_reg & 0xff);
    } else {
    }

    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 again_reg, dgain_reg, hcg_1_reg;

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (again < 0x44) {
        hcg_1_reg = 0x10;
    } else {
        hcg_1_reg = 0x00;
        again = (td_u32)((td_float)again / 4.25 + 0.5); /* HCG gain equal 4.25, 0.5 */
    }

    again_reg = clip3(again, 0x10, 0xF8);
    dgain_reg = clip3(dgain, 0x400, 0x3FFF);

    cmos_gains_regs_set(vi_pipe, sns_state, again_reg, dgain_reg, hcg_1_reg);

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
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

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

static td_s32 cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    sns_check_pointer_return(exp_func);

    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));

    exp_func->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set            = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update     = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update       = cmos_gains_update;
    exp_func->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return TD_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP                         5000
#define CALIBRATE_STATIC_WB_R_GAIN                    550
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    512

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              42
#define CALIBRATE_AWB_P2                              214
#define CALIBRATE_AWB_Q1                              0
#define CALIBRATE_AWB_A1                              165453
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-111831)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static td_s32 cmos_get_awb_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(awb_sns_dft);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
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
        case OS04A10_SLAVE_4M_30FPS_12BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 12; /* 12bit */
            isp_def->sns_mode.dng_raw_format.white_level = 4095; /* max 4095 */
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
    isp_def->key.bit1_clut             = 1;
    isp_def->clut                      = &g_cmos_clut;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    isp_def->key.bit1_ca = 1;
    isp_def->ca = &g_cmos_ca;
    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
    return;
}

static td_s32 cmos_get_isp_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(isp_def);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(isp_def, sizeof(ot_isp_cmos_default), 0, sizeof(ot_isp_cmos_default));
#ifdef CONFIG_OT_ISP_CA_SUPPORT
    isp_def->key.bit1_ca      = 1;
    isp_def->ca               = &g_cmos_ca;
#endif
    isp_def->key.bit1_dpc     = 1;
    isp_def->dpc              = &g_cmos_dpc;

    isp_def->key.bit1_wdr     = 1;
    isp_def->wdr              = &g_cmos_wdr;

    isp_def->key.bit1_lsc      = 0;
    isp_def->lsc               = &g_cmos_lsc;

    isp_def->key.bit1_acs      = 0;
    isp_def->acs               = &g_cmos_acs;

#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma = 0;
    isp_def->pregamma          = &g_cmos_pregamma;
#endif
    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            cmos_get_isp_linear_default(isp_def);
            break;
        default:
            cmos_get_isp_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        isp_def->wdr_switch_attr.exp_ratio[0] = 0x400;
    }

    isp_def->sns_mode.sns_id = OS04A10_SLAVE_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_get_isp_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_get_isp_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32  i;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(black_level);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
        black_level->auto_attr.black_level[0][i] = 0x400;
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

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    check_ret(ot_mpi_isp_get_sns_slave_attr(vi_pipe, &g_os04a10_slave_sync[vi_pipe]));

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == OS04A10_SLAVE_4M_30FPS_12BIT_LINEAR_MODE) {
            g_os04a10_slave_sync[vi_pipe].vs_time =
                (td_u32)((g_os04a10_slave_mode_tbl[sns_state->img_mode].inck_per_vs) *
                g_os04a10_slave_mode_tbl[sns_state->img_mode].max_fps / 20); /* divide 20 */
            full_lines_5fps = (OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR * 30) / 20; /* 30fps, 20fps */
        } else {
            return;
        }
    }

    if (enable) { /* setup for ISP pixel calibration mode */
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_AGAIN_L_ADDR, 0x00);
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_AGAIN_H_ADDR, 0x01);

        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_DGAIN_L_ADDR, 0x00);
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_DGAIN_M_ADDR, 0x00);
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_DGAIN_H_ADDR, 0x01);

        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_VMAX_L_ADDR, low_8bits(full_lines_5fps));
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_VMAX_H_ADDR, high_8bits(full_lines_5fps));

        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_EXPO_L_ADDR, low_8bits(full_lines_5fps - 10));
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_EXPO_H_ADDR, high_8bits(full_lines_5fps - 10));
    } else { /* setup for ISP 'normal mode' */
        sns_state->fl_std = (sns_state->fl_std > OS04A10_SLAVE_FULL_LINES_MAX) ? \
            OS04A10_SLAVE_FULL_LINES_MAX : sns_state->fl_std;
        g_os04a10_slave_sync[vi_pipe].vs_time = (td_u32)g_os04a10_slave_mode_tbl[sns_state->img_mode].inck_per_vs;
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_VMAX_L_ADDR, low_8bits(sns_state->fl_std));
        os04a10_slave_write_register(vi_pipe, OS04A10_SLAVE_VMAX_H_ADDR, high_8bits(sns_state->fl_std));
        sns_state->sync_init = TD_FALSE;
    }

    check_ret(ot_mpi_isp_set_sns_slave_attr(vi_pipe, &g_os04a10_slave_sync[vi_pipe]));
    return;
}

static td_s32 cmos_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case OT_WDR_MODE_BUILT_IN:
            sns_state->wdr_mode = OT_WDR_MODE_BUILT_IN;
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
    sns_state->regs_info[0].com_bus.i2c_dev = g_os04a10_slave_bus_info[vi_pipe].i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = REG_MAX_IDX;

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = OS04A10_SLAVE_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = OS04A10_SLAVE_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = OS04A10_SLAVE_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = OS04A10_SLAVE_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = OS04A10_SLAVE_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = OS04A10_SLAVE_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].reg_addr = OS04A10_SLAVE_AGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = OS04A10_SLAVE_DGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_M_IDX].reg_addr = OS04A10_SLAVE_DGAIN_M_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = OS04A10_SLAVE_DGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = OS04A10_SLAVE_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = OS04A10_SLAVE_VMAX_H_ADDR;

    sns_state->regs_info[0].i2c_data[GROUP1].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[GROUP1].reg_addr = OS04A10_SLAVE_GROUP_1_ADDR;

    sns_state->regs_info[0].i2c_data[GROUP2].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[GROUP2].reg_addr = OS04A10_SLAVE_GROUP_2_ADDR;

    sns_state->regs_info[0].i2c_data[HCG_1_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[HCG_1_IDX].interrupt_pos = 0;
    sns_state->regs_info[0].i2c_data[HCG_1_IDX].reg_addr = OS04A10_SLAVE_HCG_1_ADDR;

    sns_state->regs_info[0].i2c_data[GROUP3].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[GROUP3].reg_addr = OS04A10_SLAVE_GROUP_3_ADDR;

    sns_state->regs_info[0].i2c_data[GROUP4].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[GROUP4].reg_addr = OS04A10_SLAVE_GROUP_4_ADDR;

    sns_state->regs_info[0].i2c_data[GROUP5].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[GROUP5].reg_addr = OS04A10_SLAVE_GROUP_5_ADDR;

    sns_state->regs_info[0].i2c_data[GROUP6].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[GROUP6].reg_addr = OS04A10_SLAVE_GROUP_6_ADDR;

    sns_state->regs_info[0].slv_sync.update = TD_TRUE;
    sns_state->regs_info[0].slv_sync.delay_frame_num = 1; /* delay_frame 1 */
    sns_state->regs_info[0].slv_sync.slave_bind_dev = g_os04a10_slave_bind_dev[vi_pipe];

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
            if (i != HCG_1_IDX) {
                sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
            } else {
                sns_state->regs_info[0].i2c_data[GROUP1].update = TD_TRUE;
                sns_state->regs_info[0].i2c_data[GROUP2].update = TD_TRUE;
                sns_state->regs_info[0].i2c_data[HCG_1_IDX].update = TD_TRUE;
                sns_state->regs_info[0].i2c_data[GROUP3].update = TD_TRUE;
                sns_state->regs_info[0].i2c_data[GROUP4].update = TD_TRUE;
                sns_state->regs_info[0].i2c_data[GROUP5].update = TD_TRUE;
                sns_state->regs_info[0].i2c_data[GROUP6].update = TD_TRUE;
                break;
            }
        }
    }
    if (sns_state->regs_info[0].slv_sync.slave_vs_time == sns_state->regs_info[1].slv_sync.slave_vs_time) {
        sns_state->regs_info[0].slv_sync.update = TD_FALSE;
    } else {
        sns_state->regs_info[0].slv_sync.update = TD_TRUE;
    }

    return;
}

static td_s32 cmos_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(sns_regs_info);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == TD_FALSE) || (sns_regs_info->config == TD_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);
        sns_state->regs_info[0].i2c_data[GROUP1].data = 0x00;
        sns_state->regs_info[0].i2c_data[GROUP2].data = 0x01;
        sns_state->regs_info[0].i2c_data[GROUP3].data = 0x11;
        sns_state->regs_info[0].i2c_data[GROUP4].data = 0x05;
        sns_state->regs_info[0].i2c_data[GROUP5].data = 0x01;
        sns_state->regs_info[0].i2c_data[GROUP6].data = 0xa0;
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

static td_void cmos_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sensor_image_mode,
    ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    switch (sensor_image_mode) {
        case OS04A10_SLAVE_4M_30FPS_12BIT_LINEAR_MODE:
            sns_state->fl_std = OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR;
            break;
        default:
            sns_state->fl_std = OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR;
            break;
    }

    return;
}

static td_s32 cmos_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sensor_image_mode)
{
    td_u32 i;
    td_u8 image_mode;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_return(sensor_image_mode);
    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < OS04A10_SLAVE_MODE_BUTT; i++) {
        if (sensor_image_mode->fps <= g_os04a10_slave_mode_tbl[i].max_fps &&
            sensor_image_mode->width <= g_os04a10_slave_mode_tbl[i].width &&
            sensor_image_mode->height <= g_os04a10_slave_mode_tbl[i].height &&
            sns_state->wdr_mode == g_os04a10_slave_mode_tbl[i].wdr_mode) {
            image_mode = (os04a10_slave_res_mode)i;
            break;
        }
    }

    if (i >= OS04A10_SLAVE_MODE_BUTT) {
        os04a10_slave_err_mode_print(sensor_image_mode, sns_state);
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

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = OS04A10_SLAVE_4M_30FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR;
    sns_state->fl[0]     = OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR;
    sns_state->fl[1]     = OS04A10_SLAVE_VMAX_4M_30FPS_12BIT_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_s32 cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    sns_check_pointer_return(sensor_exp_func);

    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init            = os04a10_slave_init;
    sensor_exp_func->pfn_cmos_sns_exit            = os04a10_slave_exit;
    sensor_exp_func->pfn_cmos_sns_global_init     = sensor_global_init;
    sensor_exp_func->pfn_cmos_set_image_mode      = cmos_set_image_mode;
    sensor_exp_func->pfn_cmos_set_wdr_mode        = cmos_set_wdr_mode;
    sensor_exp_func->pfn_cmos_get_isp_default     = cmos_get_isp_default;
    sensor_exp_func->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    sensor_exp_func->pfn_cmos_get_blc_clamp_info  = cmos_get_isp_blc_clamp_info;
    sensor_exp_func->pfn_cmos_set_pixel_detect    = cmos_set_pixel_detect;
    sensor_exp_func->pfn_cmos_get_sns_reg_info    = cmos_get_sns_regs_info;

    return TD_SUCCESS;
}

static td_s32 os04a10_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    g_os04a10_slave_bus_info[vi_pipe].i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 sensor_ctx_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == TD_NULL) {
        sns_state_ctx = (ot_isp_sns_state *)malloc(sizeof(ot_isp_sns_state));
        if (sns_state_ctx == TD_NULL) {
            isp_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_ERR_ISP_NOMEM;
        }
    }

    (td_void)memset_s(sns_state_ctx, sizeof(ot_isp_sns_state), 0, sizeof(ot_isp_sns_state));

    os04a10_slave_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return TD_SUCCESS;
}

static td_void sensor_ctx_exit(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    os04a10_slave_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    os04a10_slave_sensor_reset_ctx(vi_pipe);

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

    sns_attr_info.sns_id = OS04A10_SLAVE_ID;
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

    ret = ot_mpi_isp_sensor_unreg_callback(vi_pipe, OS04A10_SLAVE_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = ot_mpi_ae_sensor_unreg_callback(vi_pipe, ae_lib, OS04A10_SLAVE_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_mpi_awb_sensor_unreg_callback(vi_pipe, awb_lib, OS04A10_SLAVE_ID);
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

ot_isp_sns_obj g_sns_os04a10_slave_obj = {
    .pfn_register_callback     = sensor_register_callback,
    .pfn_un_register_callback  = sensor_unregister_callback,
    .pfn_standby               = os04a10_slave_standby,
    .pfn_restart               = os04a10_slave_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = os04a10_slave_write_register,
    .pfn_read_reg              = os04a10_slave_read_register,
    .pfn_set_bus_info          = os04a10_set_bus_info,
    .pfn_set_init              = sensor_set_init
};
