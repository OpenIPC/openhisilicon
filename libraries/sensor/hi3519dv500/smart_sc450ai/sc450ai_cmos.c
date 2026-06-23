/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"

#include "sc450ai_cmos_ex.h"
#include "sc450ai_cmos.h"

#define SC450AI_ID             450
#define SC450AI_WIDTH          2688
#define SC450AI_HEIGHT         1520

#define high_8bits(x)   (((x) & 0xff00) >> 8)
#define low_8bits(x)    ((x) & 0x00ff)
#define lower_4bits(x)  (((x) & 0x000F) << 4)
#define higher_4bits(x) (((x) & 0xF000) >> 12)
#define higher_8bits(x) (((x) & 0x0FF0) >> 4)

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define sc450ai_sensor_set_ctx(dev, ctx)   ((g_sc450ai_sns_state[dev]) = (ctx))
#define sc450ai_sensor_reset_ctx(dev)      (g_sc450ai_sns_state[dev] = TD_NULL)

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

static td_u32 g_max_short_exp[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = SC450AI_SEXP_MAX_DEFAULT * 2
};

ot_isp_sns_commbus g_sc450ai_bus_info[OT_ISP_MAX_PIPE_NUM] = {
    [0] = { .i2c_dev = 0 },
    [1 ... OT_ISP_MAX_PIPE_NUM - 1] = { .i2c_dev = -1 }
};

static td_float g_dcg_ratio = 3.813;

ot_isp_sns_state *g_sc450ai_sns_state[OT_ISP_MAX_PIPE_NUM] = {TD_NULL};

static td_bool blc_clamp_info[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = TD_TRUE};
ot_isp_sns_commbus *sc450ai_get_bus_info(ot_vi_pipe vi_pipe)
{
    return &g_sc450ai_bus_info[vi_pipe];
}

ot_isp_sns_state *sc450ai_get_ctx(ot_vi_pipe vi_pipe)
{
    return g_sc450ai_sns_state[vi_pipe];
}

const sc450ai_video_mode_tbl g_sc450ai_mode_tbl[SC450AI_MODE_BUTT] = {
    {SC450AI_VMAX_4M_30FPS_12BIT_LINEAR, SC450AI_FULL_LINES_MAX, 30, 2.5,
     SC450AI_WIDTH, SC450AI_HEIGHT, 0, OT_WDR_MODE_NONE, "SC450AI_4M_30FPS_12BIT_LINEAR_MODE"},

    {SC450AI_VMAX_4M_30FPS_2TO1_WDR, SC450AI_FULL_LINES_MAX_2TO1_WDR, 30, 10,
     SC450AI_WIDTH, SC450AI_HEIGHT, 0, OT_WDR_MODE_2To1_LINE, "SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE"},
};

#define sc450ai_err_mode_print(sns_image_mode, sns_state) \
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
    td_float max_fps = 30; /* maxfps 30 */

    if (sns_state->img_mode == SC450AI_4M_30FPS_12BIT_LINEAR_MODE) {
        ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
        ae_sns_dft->int_time_accu.accuracy = 1;
        ae_sns_dft->int_time_accu.offset = 0.182; /* offset: 0.182 */
    } else if (sns_state->img_mode == SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE) {
        ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
        ae_sns_dft->int_time_accu.accuracy = 4; /* accuracy: 4 */
        ae_sns_dft->int_time_accu.offset = 0.216; /* offset: 0.216 */
    } else {
    }

    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = SC450AI_FULL_LINES_MAX * 2;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns, 30fps */

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 0.0625; /* accuracy: 0.0625 */

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 0.125; /* accuracy: 0.125 */

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 32 << ae_sns_dft->isp_dgain_shift; /* max 32 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = (td_u32)(sns_state->fl_std * max_fps / 2); /* 30fps, div 2 */
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
    ae_sns_dft->max_again = 61976; /* 15.875*3.813*1024 = 61976 */
    ae_sns_dft->min_again = 1024;  /* min 16 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 1024; /* max 1024 */
    ae_sns_dft->min_dgain = 1024;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 148859; /* init 148859 */

    ae_sns_dft->max_int_time = sns_state->fl_std - EXP_OFFSET_LINEAR;
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

static td_void cmos_get_ae_2to1_line_wdr_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_int_time = sns_state->fl_std - g_max_short_exp[vi_pipe] - EXP_OFFSET_WDR;
    ae_sns_dft->min_int_time = 4; /* min_int_time 4 */
    ae_sns_dft->int_time_accu.offset = 0.216; /* offset: 0.216 */
    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again = 61976; /* 15.875*3.4*1024 = 61976 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 16256; /* max 16256 */
    ae_sns_dft->min_dgain = 1024;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->max_isp_dgain_target = 2 << ae_sns_dft->isp_dgain_shift; /* max 2 << shift */
    ae_sns_dft->diff_gain_support = TD_TRUE;

    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 16462; /* init 16462 */

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
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
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
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = high_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = low_8bits(vmax);
    } else {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = high_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = low_8bits(vmax);
    }

    return;
}

/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines;
    td_u32 lines_max;
    td_u32 vmax;
    td_float max_fps;
    td_float min_fps;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_void_return(ae_sns_dft);
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_sc450ai_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_sc450ai_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_sc450ai_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_sc450ai_mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        isp_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
    vmax = (vmax > lines_max) ? lines_max : vmax;

    cmos_config_vmax(sns_state, vmax);
    sns_state->fl_std = vmax << 1;
    ae_sns_dft->lines_per500ms = (lines * 2) * 15; /* 2 * 15 */

    ae_sns_dft->fps = fps;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl_std - EXP_OFFSET_LINEAR;
    } else {
        g_max_short_exp[vi_pipe] = (td_u32)(SC450AI_SEXP_MAX_DEFAULT * max_fps / div_0_to_1_float(fps));
        ae_sns_dft->max_int_time = sns_state->fl_std - g_max_short_exp[vi_pipe] - EXP_OFFSET_WDR;
        sns_state->regs_info[0].i2c_data[SEXP_MAX_L_IDX].data = high_8bits(g_max_short_exp[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SEXP_MAX_H_IDX].data = low_8bits(g_max_short_exp[vi_pipe]);
    }

    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (td_u32)((1000000000) / (sns_state->fl_std * div_0_to_1_float(fps))); /* 1000000000ns */
    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 vmax;
    td_u32 lines_max;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_void_return(ae_sns_dft);
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines_max = g_sc450ai_mode_tbl[sns_state->img_mode].max_ver_lines;

    vmax = (full_lines + EXP_OFFSET_LINEAR) >> 1;
    vmax = (vmax > lines_max) ? lines_max : vmax;
    sns_state->fl[0] = vmax << 1;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = high_8bits(vmax);
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = low_8bits(vmax);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl[0] - EXP_OFFSET_LINEAR;
    } else {
        ae_sns_dft->max_int_time = sns_state->fl[0] - g_max_short_exp[vi_pipe] - EXP_OFFSET_WDR;
    }
    ae_sns_dft->full_lines = sns_state->fl[0];

    return;
}

static td_void cmos_inttime_update_linear(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = higher_4bits(int_time);
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].data = higher_8bits(int_time);
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = lower_4bits(int_time);

    return;
}

static td_void cmos_inttime_update_2to1_line(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    static td_bool is_first[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = 1};

    static td_u32 short_int_time[OT_ISP_MAX_PIPE_NUM] = {0};
    static td_u32 long_int_time[OT_ISP_MAX_PIPE_NUM] = {0};

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (is_first[vi_pipe]) { /* short exposure */
        sns_state->wdr_int_time[0] = int_time;
        short_int_time[vi_pipe] = int_time;
        is_first[vi_pipe] = TD_FALSE;
    } else { /* long exposure */
        sns_state->wdr_int_time[1] = int_time;
        long_int_time[vi_pipe] = int_time;

        sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = higher_4bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[EXPO_M_IDX].data = higher_8bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = lower_4bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SEXPO_L_IDX].data = higher_8bits(short_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SEXPO_H_IDX].data = lower_4bits(short_int_time[vi_pipe]);

        is_first[vi_pipe] = TD_TRUE;
    }
    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}

#define SC450AI_AGAIN_NODE_NUM               256
#define SC450AI_AGAIN_ADDR_INDEX_NODE_NUM    256
#define SC450AI_DGAIN_NODE_NUM               256
#define SC450AI_DGAIN_ADDR_INDEX_NODE_NUM    256

static td_u32 again_index[SC450AI_AGAIN_NODE_NUM] = {
    1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264,
    1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520,
    1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776,
    1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032,
    2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528,
    2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040,
    3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552,
    3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064,
    4096, 4160, 4224, 4288, 4352, 4416, 4480, 4544, 4608, 4672, 4736, 4800, 4864, 4928, 4992, 5056,
    5120, 5184, 5248, 5312, 5376, 5440, 5504, 5568, 5632, 5696, 5760, 5824, 5888, 5952, 6016, 6080,
    6144, 6208, 6272, 6336, 6400, 6464, 6528, 6592, 6656, 6720, 6784, 6848, 6912, 6976, 7040, 7104,
    7168, 7232, 7296, 7360, 7424, 7488, 7552, 7616, 7680, 7744, 7808, 7872, 7936, 8000, 8064, 8128,
    8192, 8320, 8448, 8576, 8704, 8832, 8960, 9088, 9216, 9344, 9472, 9600, 9728, 9856, 9984, 10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256
};

static td_u32 dgain_index[SC450AI_DGAIN_NODE_NUM] = {
    1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264,
    1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520,
    1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776,
    1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032,
    2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528,
    2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040,
    3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552,
    3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064,
    4096, 4160, 4224, 4288, 4352, 4416, 4480, 4544, 4608, 4672, 4736, 4800, 4864, 4928, 4992, 5056,
    5120, 5184, 5248, 5312, 5376, 5440, 5504, 5568, 5632, 5696, 5760, 5824, 5888, 5952, 6016, 6080,
    6144, 6208, 6272, 6336, 6400, 6464, 6528, 6592, 6656, 6720, 6784, 6848, 6912, 6976, 7040, 7104,
    7168, 7232, 7296, 7360, 7424, 7488, 7552, 7616, 7680, 7744, 7808, 7872, 7936, 8000, 8064, 8128,
    8192, 8320, 8448, 8576, 8704, 8832, 8960, 9088, 9216, 9344, 9472, 9600, 9728, 9856, 9984, 10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256
};

static td_u32 again_table[SC450AI_AGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347, 0x0348, 0x0349, 0x034A, 0x034B, 0x034C, 0x034D,
    0x034E, 0x034F, 0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357, 0x0358, 0x0359, 0x035A, 0x035B,
    0x035C, 0x035D, 0x035E, 0x035F, 0x0360, 0x0361, 0x0362, 0x0363, 0x0364, 0x0365, 0x0366, 0x0367, 0x0368, 0x0369,
    0x036A, 0x036B, 0x036C, 0x036D, 0x036E, 0x036F, 0x0370, 0x0371, 0x0372, 0x0373, 0x0374, 0x0375, 0x0376, 0x0377,
    0x0378, 0x0379, 0x037A, 0x037B, 0x037C, 0x037D, 0x037E, 0x037F, 0x0740, 0x0741, 0x0742, 0x0743, 0x0744, 0x0745,
    0x0746, 0x0747, 0x0748, 0x0749, 0x074A, 0x074B, 0x074C, 0x074D, 0x074E, 0x074F, 0x0750, 0x0751, 0x0752, 0x0753,
    0x0754, 0x0755, 0x0756, 0x0757, 0x0758, 0x0759, 0x075A, 0x075B, 0x075C, 0x075D, 0x075E, 0x075F, 0x0760, 0x0761,
    0x0762, 0x0763, 0x0764, 0x0765, 0x0766, 0x0767, 0x0768, 0x0769, 0x076A, 0x076B, 0x076C, 0x076D, 0x076E, 0x076F,
    0x0770, 0x0771, 0x0772, 0x0773, 0x0774, 0x0775, 0x0776, 0x0777, 0x0778, 0x0779, 0x077A, 0x077B, 0x077C, 0x077D,
    0x077E, 0x077F, 0x0F40, 0x0F41, 0x0F42, 0x0F43, 0x0F44, 0x0F45, 0x0F46, 0x0F47, 0x0F48, 0x0F49, 0x0F4A, 0x0F4B,
    0x0F4C, 0x0F4D, 0x0F4E, 0x0F4F, 0x0F50, 0x0F51, 0x0F52, 0x0F53, 0x0F54, 0x0F55, 0x0F56, 0x0F57, 0x0F58, 0x0F59,
    0x0F5A, 0x0F5B, 0x0F5C, 0x0F5D, 0x0F5E, 0x0F5F, 0x0F60, 0x0F61, 0x0F62, 0x0F63, 0x0F64, 0x0F65, 0x0F66, 0x0F67,
    0x0F68, 0x0F69, 0x0F6A, 0x0F6B, 0x0F6C, 0x0F6D, 0x0F6E, 0x0F6F, 0x0F70, 0x0F71, 0x0F72, 0x0F73, 0x0F74, 0x0F75,
    0x0F76, 0x0F77, 0x0F78, 0x0F79, 0x0F7A, 0x0F7B, 0x0F7C, 0x0F7D, 0x0F7E, 0x0F7F, 0x1F40, 0x1F41, 0x1F42, 0x1F43,
    0x1F44, 0x1F45, 0x1F46, 0x1F47, 0x1F48, 0x1F49, 0x1F4A, 0x1F4B, 0x1F4C, 0x1F4D, 0x1F4E, 0x1F4F, 0x1F50, 0x1F51,
    0x1F52, 0x1F53, 0x1F54, 0x1F55, 0x1F56, 0x1F57, 0x1F58, 0x1F59, 0x1F5A, 0x1F5B, 0x1F5C, 0x1F5D, 0x1F5E, 0x1F5F,
    0x1F60, 0x1F61, 0x1F62, 0x1F63, 0x1F64, 0x1F65, 0x1F66, 0x1F67, 0x1F68, 0x1F69, 0x1F6A, 0x1F6B, 0x1F6C, 0x1F6D,
    0x1F6E, 0x1F6F, 0x1F70, 0x1F71, 0x1F72, 0x1F73, 0x1F74, 0x1F75, 0x1F76, 0x1F77, 0x1F78, 0x1F79, 0x1F7A, 0x1F7B,
    0x1F7C, 0x1F7D, 0x1F7E, 0x1F7F
};

static td_u32 dgain_table[SC450AI_DGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0080, 0x0082, 0x0084, 0x0086, 0x0088, 0x008A, 0x008C, 0x008E, 0x0090, 0x0092, 0x0094, 0x0096, 0x0098, 0x009A,
    0x009C, 0x009E, 0x00A0, 0x00A2, 0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AC, 0x00AE, 0x00B0, 0x00B2, 0x00B4, 0x00B6,
    0x00B8, 0x00BA, 0x00BC, 0x00BE, 0x00C0, 0x00C2, 0x00C4, 0x00C6, 0x00C8, 0x00CA, 0x00CC, 0x00CE, 0x00D0, 0x00D2,
    0x00D4, 0x00D6, 0x00D8, 0x00DA, 0x00DC, 0x00DE, 0x00E0, 0x00E2, 0x00E4, 0x00E6, 0x00E8, 0x00EA, 0x00EC, 0x00EE,
    0x00F0, 0x00F2, 0x00F4, 0x00F6, 0x00F8, 0x00FA, 0x00FC, 0x00FE, 0x0180, 0x0182, 0x0184, 0x0186, 0x0188, 0x018A,
    0x018C, 0x018E, 0x0190, 0x0192, 0x0194, 0x0196, 0x0198, 0x019A, 0x019C, 0x019E, 0x01A0, 0x01A2, 0x01A4, 0x01A6,
    0x01A8, 0x01AA, 0x01AC, 0x01AE, 0x01B0, 0x01B2, 0x01B4, 0x01B6, 0x01B8, 0x01BA, 0x01BC, 0x01BE, 0x01C0, 0x01C2,
    0x01C4, 0x01C6, 0x01C8, 0x01CA, 0x01CC, 0x01CE, 0x01D0, 0x01D2, 0x01D4, 0x01D6, 0x01D8, 0x01DA, 0x01DC, 0x01DE,
    0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8, 0x01EA, 0x01EC, 0x01EE, 0x01F0, 0x01F2, 0x01F4, 0x01F6, 0x01F8, 0x01FA,
    0x01FC, 0x01FE, 0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038A, 0x038C, 0x038E, 0x0390, 0x0392, 0x0394, 0x0396,
    0x0398, 0x039A, 0x039C, 0x039E, 0x03A0, 0x03A2, 0x03A4, 0x03A6, 0x03A8, 0x03AA, 0x03AC, 0x03AE, 0x03B0, 0x03B2,
    0x03B4, 0x03B6, 0x03B8, 0x03BA, 0x03BC, 0x03BE, 0x03C0, 0x03C2, 0x03C4, 0x03C6, 0x03C8, 0x03CA, 0x03CC, 0x03CE,
    0x03D0, 0x03D2, 0x03D4, 0x03D6, 0x03D8, 0x03DA, 0x03DC, 0x03DE, 0x03E0, 0x03E2, 0x03E4, 0x03E6, 0x03E8, 0x03EA,
    0x03EC, 0x03EE, 0x03F0, 0x03F2, 0x03F4, 0x03F6, 0x03F8, 0x03FA, 0x03FC, 0x03FE, 0x0780, 0x0782, 0x0784, 0x0786,
    0x0788, 0x078A, 0x078C, 0x078E, 0x0790, 0x0792, 0x0794, 0x0796, 0x0798, 0x079A, 0x079C, 0x079E, 0x07A0, 0x07A2,
    0x07A4, 0x07A6, 0x07A8, 0x07AA, 0x07AC, 0x07AE, 0x07B0, 0x07B2, 0x07B4, 0x07B6, 0x07B8, 0x07BA, 0x07BC, 0x07BE,
    0x07C0, 0x07C2, 0x07C4, 0x07C6, 0x07C8, 0x07CA, 0x07CC, 0x07CE, 0x07D0, 0x07D2, 0x07D4, 0x07D6, 0x07D8, 0x07DA,
    0x07DC, 0x07DE, 0x07E0, 0x07E2, 0x07E4, 0x07E6, 0x07E8, 0x07EA, 0x07EC, 0x07EE, 0x07F0, 0x07F2, 0x07F4, 0x07F6,
    0x07F8, 0x07FA, 0x07FC, 0x07FE
};

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
    td_s32 i;
    td_u32 again_lin_tmp;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    if (*again_lin >= (td_u32)(again_index[SC450AI_AGAIN_ADDR_INDEX_NODE_NUM - 1] * g_dcg_ratio)) {
        *again_lin = (td_u32)(again_index[SC450AI_AGAIN_ADDR_INDEX_NODE_NUM - 1] * g_dcg_ratio);
        if (g_dcg_ratio > 1) {
            *again_db = again_table[SC450AI_AGAIN_NODE_NUM - 1] | 0x2000;
        } else {
            *again_db = again_table[SC450AI_AGAIN_NODE_NUM - 1];
        }
        return;
    }
    if (*again_lin > (1024 * g_dcg_ratio)) {  /* 1024 */
        again_lin_tmp = (td_u32)(*again_lin / g_dcg_ratio);
        for (i = 1; i < SC450AI_AGAIN_NODE_NUM; i++) {
            if (again_lin_tmp < again_index[i]) {
                *again_lin = (td_u32)(again_index[i - 1] * g_dcg_ratio);
                *again_db = again_table[i - 1] | 0x2000;
                break;
            }
        }
    } else {
        again_lin_tmp = *again_lin;
        for (i = 1; i < SC450AI_AGAIN_NODE_NUM; i++) {
            if (again_lin_tmp < again_index[i]) {
                *again_lin = again_index[i - 1];
                *again_db = again_table[i - 1];
                break;
            }
        }
    }
    return;
}

static td_void cmos_dgain_calc_table(ot_vi_pipe vi_pipe, td_u32 *dgain_lin, td_u32 *dgain_db)
{
    int i;

    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);

    if (*dgain_lin >= dgain_index[SC450AI_DGAIN_ADDR_INDEX_NODE_NUM - 1]) {
        *dgain_lin = dgain_index[SC450AI_DGAIN_ADDR_INDEX_NODE_NUM - 1];
        *dgain_db = dgain_table[SC450AI_DGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < SC450AI_DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < dgain_index[i]) {
            *dgain_lin = dgain_index[i - 1];
            *dgain_db = dgain_table[i - 1];
            break;
        }
    }
    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    td_u8 reg_0x3e09;
    td_u8 reg_0x3e08;
    td_u8 reg_0x3e07;
    td_u8 reg_0x3e06;

    reg_0x3e06 = high_8bits(dgain);
    reg_0x3e07 = low_8bits (dgain);
    reg_0x3e08 = high_8bits(again);
    reg_0x3e09 = low_8bits (again);

    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = reg_0x3e06;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = reg_0x3e07;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = reg_0x3e08;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].data = reg_0x3e09;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].i2c_data[SDGAIN_L_IDX].data = reg_0x3e06;
        sns_state->regs_info[0].i2c_data[SDGAIN_H_IDX].data = reg_0x3e07;
        sns_state->regs_info[0].i2c_data[SAGAIN_L_IDX].data = reg_0x3e08;
        sns_state->regs_info[0].i2c_data[SAGAIN_H_IDX].data = reg_0x3e09;
    } else {
    }
    return;
}

static td_void cmos_get_inttime_max_2to1_line(ot_vi_pipe vi_pipe, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    td_u32 short_max0;
    td_u32 short_max;
    td_u32 short_time_min_limit;
    ot_isp_sns_state *sns_state = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    short_time_min_limit = 4; /* short_time_min_limit 4 */

    if (g_fswdr_mode[vi_pipe] == OT_ISP_FSWDR_LONG_FRAME_MODE) {
        short_max0 = sns_state->fl[1] - 20 - sns_state->wdr_int_time[0]; /* sensor limit: sub 20 */
        short_max = sns_state->fl[0] - 20; /* sensor limit: sub 20 */
        short_max = short_max > (g_max_short_exp[vi_pipe] - SEXP_OFFSET_WDR) ?
                    (g_max_short_exp[vi_pipe] - SEXP_OFFSET_WDR) : short_max;
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        int_time->int_time_max[0] = short_time_min_limit;
        int_time->int_time_min[0] = short_time_min_limit;
        int_time->int_time_max[1] = short_max;
        int_time->int_time_min[1] = short_time_min_limit;
        return;
    } else {
        short_max0 = ((sns_state->fl[1] - 20 - g_max_short_exp[vi_pipe]) * 0x40) / div_0_to_1(ratio[0]); /* sub 20 */
        short_max = ((sns_state->fl[0] - 20 - g_max_short_exp[vi_pipe]) * 0x40) / div_0_to_1(ratio[0]); /* sub 20 */
        short_max = short_max > (g_max_short_exp[vi_pipe] - SEXP_OFFSET_WDR) ?
                    (g_max_short_exp[vi_pipe] - SEXP_OFFSET_WDR) : short_max;
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;
    }
    *lf_max_int_time = sns_state->fl[0] - 20; /* sub 20 */

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
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
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

static td_s32 cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    sns_check_pointer_return(exp_func);

    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));

    exp_func->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set            = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update     = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update       = cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    exp_func->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
    exp_func->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return TD_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP       4950
#define CALIBRATE_STATIC_WB_R_GAIN  478
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  497

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 (-102)
#define CALIBRATE_AWB_P2 358
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 183957
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-135938)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static td_s32 cmos_get_awb_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(awb_sns_dft);
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
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
        case SC450AI_4M_30FPS_12BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 10; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = 1023; /* max 1023 */
            break;
        case SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 10; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = 1023; /* max 1023 */
            break;
        default:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = 10; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = 1023; /* max 1023 */
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
    isp_def->key.bit1_clut           = 1;
    isp_def->clut                    = &g_cmos_clut_wdr;
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
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
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
        case OT_WDR_MODE_2To1_LINE:
            cmos_get_isp_wdr_default(isp_def);
            break;
        default:
            cmos_get_isp_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        isp_def->wdr_switch_attr.exp_ratio[0] = 0x400;
    }

    isp_def->sns_mode.sns_id = SC450AI_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_get_isp_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_get_isp_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32  i;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(black_level);
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
            black_level->auto_attr.black_level[0][i] = 0x410;
        }
    } else { /* black level of DOL mode */
        for (i = 0; i < OT_ISP_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = 0x410;
            black_level->auto_attr.black_level[i][1] = 0x410;
            black_level->auto_attr.black_level[i][2] = 0x410; /* index 2 */
            black_level->auto_attr.black_level[i][3] = 0x410; /* index 3 */
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

static td_s32 cmos_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear 4M30fps mode\n");
            break;

        case OT_WDR_MODE_2To1_LINE:
            sns_state->wdr_mode = OT_WDR_MODE_2To1_LINE;
            printf("2to1 line WDR 4M30fps mode!\n");
            break;

        default:
            isp_err_trace("Not support this mode!\n");
            return TD_FAILURE;
    }

    (td_void)memset_s(sns_state->wdr_int_time, sizeof(sns_state->wdr_int_time), 0, sizeof(sns_state->wdr_int_time));

    return TD_SUCCESS;
}

static td_void cmos_comm_sns_reg_info_init(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    sns_state->regs_info[0].sns_type = OT_ISP_SNS_TYPE_I2C;
    sns_state->regs_info[0].com_bus.i2c_dev = g_sc450ai_bus_info[vi_pipe].i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 9; /* reg num 9 */

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].reg_num += 8; /* regs +8 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = SC450AI_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = SC450AI_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = SC450AI_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = SC450AI_EXP_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].reg_addr = SC450AI_EXP_ADDR + 1;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = SC450AI_EXP_ADDR + 2; /* add 2 */

    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = SC450AI_DGAIN_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = SC450AI_DGAIN_ADDR + 1;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = SC450AI_AGAIN_ADDR;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].reg_addr = SC450AI_AGAIN_ADDR + 1;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = SC450AI_VMAX_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = SC450AI_VMAX_ADDR + 1;
    return;
}

static td_void cmos_2to1_line_wdr_sns_reg_info_init(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    sns_state->regs_info[0].i2c_data[SEXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SEXPO_L_IDX].reg_addr = SC450AI_SEXP_ADDR;
    sns_state->regs_info[0].i2c_data[SEXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SEXPO_H_IDX].reg_addr = SC450AI_SEXP_ADDR + 1;

    sns_state->regs_info[0].i2c_data[SDGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SDGAIN_L_IDX].reg_addr = SC450AI_SDGAIN_ADDR;
    sns_state->regs_info[0].i2c_data[SDGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SDGAIN_H_IDX].reg_addr = SC450AI_SDGAIN_ADDR + 1;

    sns_state->regs_info[0].i2c_data[SAGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SAGAIN_L_IDX].reg_addr = SC450AI_SAGAIN_ADDR;
    sns_state->regs_info[0].i2c_data[SAGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SAGAIN_H_IDX].reg_addr = SC450AI_SAGAIN_ADDR + 1;

    sns_state->regs_info[0].i2c_data[SEXP_MAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[SEXP_MAX_L_IDX].reg_addr = SC450AI_SEXP_MAX_ADDR;
    sns_state->regs_info[0].i2c_data[SEXP_MAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[SEXP_MAX_H_IDX].reg_addr = SC450AI_SEXP_MAX_ADDR + 1;

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

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        if ((sns_state->regs_info[0].i2c_data[SEXPO_L_IDX].update ||
             sns_state->regs_info[0].i2c_data[SEXPO_H_IDX].update ||
             sns_state->regs_info[0].i2c_data[SDGAIN_L_IDX].update ||
             sns_state->regs_info[0].i2c_data[SDGAIN_H_IDX].update ||
             sns_state->regs_info[0].i2c_data[SAGAIN_L_IDX].update ||
             sns_state->regs_info[0].i2c_data[SAGAIN_H_IDX].update) == TD_TRUE) {
            sns_state->regs_info[0].i2c_data[SEXPO_L_IDX].update = TD_TRUE;
            sns_state->regs_info[0].i2c_data[SEXPO_H_IDX].update = TD_TRUE;
            sns_state->regs_info[0].i2c_data[SDGAIN_L_IDX].update = TD_TRUE;
            sns_state->regs_info[0].i2c_data[SDGAIN_H_IDX].update = TD_TRUE;
            sns_state->regs_info[0].i2c_data[SAGAIN_L_IDX].update = TD_TRUE;
            sns_state->regs_info[0].i2c_data[SAGAIN_H_IDX].update = TD_TRUE;
            sns_state->regs_info[0].i2c_data[SEXP_MAX_L_IDX].update = TD_TRUE;
        } else {
            sns_state->regs_info[0].i2c_data[SEXPO_L_IDX].update = TD_FALSE;
            sns_state->regs_info[0].i2c_data[SEXPO_H_IDX].update = TD_FALSE;
            sns_state->regs_info[0].i2c_data[SDGAIN_L_IDX].update = TD_FALSE;
            sns_state->regs_info[0].i2c_data[SDGAIN_H_IDX].update = TD_FALSE;
            sns_state->regs_info[0].i2c_data[SAGAIN_L_IDX].update = TD_FALSE;
            sns_state->regs_info[0].i2c_data[SAGAIN_H_IDX].update = TD_FALSE;
            sns_state->regs_info[0].i2c_data[SEXP_MAX_L_IDX].update = TD_FALSE;
        }
    } else {
    }
    return;
}

static td_s32 cmos_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(sns_regs_info);
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
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

static td_void cmos_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sensor_image_mode,
    ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    switch (sensor_image_mode) {
        case SC450AI_4M_30FPS_12BIT_LINEAR_MODE:
            sns_state->fl_std = SC450AI_VMAX_4M_30FPS_12BIT_LINEAR << 1;
            break;
        case SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE:
            sns_state->fl_std = SC450AI_VMAX_4M_30FPS_2TO1_WDR << 1;
            break;
        default:
            sns_state->fl_std = SC450AI_VMAX_4M_30FPS_12BIT_LINEAR << 1;
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
    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < SC450AI_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_sc450ai_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_sc450ai_mode_tbl[i].width &&
            sns_image_mode->height <= g_sc450ai_mode_tbl[i].height &&
            sns_state->wdr_mode == g_sc450ai_mode_tbl[i].wdr_mode) {
            image_mode = (sc450ai_res_mode)i;
            break;
        }
    }

    if (i >= SC450AI_MODE_BUTT) {
        sc450ai_err_mode_print(sns_image_mode, sns_state);
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

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = SC450AI_4M_30FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = SC450AI_VMAX_4M_30FPS_12BIT_LINEAR << 1;
    sns_state->fl[0]     = SC450AI_VMAX_4M_30FPS_12BIT_LINEAR << 1;
    sns_state->fl[1]     = SC450AI_VMAX_4M_30FPS_12BIT_LINEAR << 1;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_void cmos_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    td_u32 full_lines_5fps;
    ot_isp_sns_state *sns_state = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == SC450AI_4M_30FPS_12BIT_LINEAR_MODE) {
            full_lines_5fps = (SC450AI_VMAX_4M_30FPS_12BIT_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    if (enable) { /* setup for ISP pixel calibration mode */
        sc450ai_write_register(vi_pipe, SC450AI_AGAIN_ADDR, 0x03);
        sc450ai_write_register(vi_pipe, SC450AI_AGAIN_ADDR + 1, 0x40);

        sc450ai_write_register(vi_pipe, SC450AI_DGAIN_ADDR, 0x00);
        sc450ai_write_register(vi_pipe, SC450AI_DGAIN_ADDR + 1, 0x80);

        sc450ai_write_register(vi_pipe, SC450AI_VMAX_ADDR, low_8bits(full_lines_5fps));
        sc450ai_write_register(vi_pipe, SC450AI_VMAX_ADDR + 1, high_8bits(full_lines_5fps));

        sc450ai_write_register(vi_pipe, SC450AI_EXP_ADDR, low_8bits(full_lines_5fps - 10));
        sc450ai_write_register(vi_pipe, SC450AI_EXP_ADDR + 1, high_8bits(full_lines_5fps - 10));
        sc450ai_write_register(vi_pipe, SC450AI_EXP_ADDR + 2, lower_4bits(full_lines_5fps - 10)); /* index 2 */
    } else { /* setup for ISP 'normal mode' */
        sns_state->fl_std = (sns_state->fl_std > SC450AI_FULL_LINES_MAX) ? SC450AI_FULL_LINES_MAX : sns_state->fl_std;
        sc450ai_write_register(vi_pipe, SC450AI_VMAX_ADDR, low_8bits(sns_state->fl_std));
        sc450ai_write_register(vi_pipe, SC450AI_VMAX_ADDR + 1, high_8bits(sns_state->fl_std));
        sns_state->sync_init = TD_FALSE;
    }
    return;
}


static td_s32 cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    sns_check_pointer_return(sensor_exp_func);

    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init         = sc450ai_init;
    sensor_exp_func->pfn_cmos_sns_exit         = sc450ai_exit;
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

static td_s32 sc450ai_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    g_sc450ai_bus_info[vi_pipe].i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 sensor_ctx_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == TD_NULL) {
        sns_state_ctx = (ot_isp_sns_state *)malloc(sizeof(ot_isp_sns_state));
        if (sns_state_ctx == TD_NULL) {
            isp_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_ERR_ISP_NOMEM;
        }
    }

    (td_void)memset_s(sns_state_ctx, sizeof(ot_isp_sns_state), 0, sizeof(ot_isp_sns_state));

    sc450ai_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return TD_SUCCESS;
}

static td_void sensor_ctx_exit(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    sc450ai_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    sc450ai_sensor_reset_ctx(vi_pipe);

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

    sns_attr_info.sns_id = SC450AI_ID;
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

    ret = ot_mpi_isp_sensor_unreg_callback(vi_pipe, SC450AI_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = ot_mpi_ae_sensor_unreg_callback(vi_pipe, ae_lib, SC450AI_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_mpi_awb_sensor_unreg_callback(vi_pipe, awb_lib, SC450AI_ID);
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

ot_isp_sns_obj g_sns_sc450ai_obj = {
    .pfn_register_callback     = sensor_register_callback,
    .pfn_un_register_callback  = sensor_unregister_callback,
    .pfn_standby               = sc450ai_standby,
    .pfn_restart               = sc450ai_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = sc450ai_write_register,
    .pfn_read_reg              = sc450ai_read_register,
    .pfn_set_bus_info          = sc450ai_set_bus_info,
    .pfn_set_init              = sensor_set_init
};
