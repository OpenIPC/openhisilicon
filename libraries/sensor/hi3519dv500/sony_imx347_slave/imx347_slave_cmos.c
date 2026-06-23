/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "securec.h"
#include "imx347_slave_cmos_ex.h"
#include "imx347_slave_cmos.h"

#define IMX347_SLAVE_ID                    347
#define SENSOR_IMX347_SLAVE_WIDTH          2688
#define SENSOR_IMX347_SLAVE_HEIGHT         1520

#define higher_4bits(x) (((x) & 0xf0000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define imx347_slave_sensor_set_ctx(pipe, ctx)   ((g_imx347_slave_sns_state[pipe]) = (ctx))
#define imx347_slave_sensor_reset_ctx(pipe)      (g_imx347_slave_sns_state[pipe] = TD_NULL)

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

typedef struct {
    td_u8       hcg;
    td_u32      brl;
} imx347_slave_state;

imx347_slave_state g_imx347_slave_state[OT_ISP_MAX_PIPE_NUM] = {{0}};
ot_isp_sns_commbus g_imx347_slave_bus_info[OT_ISP_MAX_PIPE_NUM] = {
    [0] = { .i2c_dev = 0},
    [1 ... OT_ISP_MAX_PIPE_NUM - 1] = { .i2c_dev = -1}
};

td_s32 g_imx347_slave_bind_dev[OT_ISP_MAX_PIPE_NUM] = {0, 0, 1, 1};
static td_u32 g_imx347_slave_sensor_mode_time[OT_ISP_MAX_PIPE_NUM] = {0, 0, 0, 0};

ot_isp_sns_state *g_imx347_slave_sns_state[OT_ISP_MAX_PIPE_NUM] = {TD_NULL};

static td_bool blc_clamp_info[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = TD_TRUE};

ot_isp_slave_sns_sync g_imx347_slave_sync[OT_ISP_MAX_PIPE_NUM];


ot_isp_sns_commbus *imx347_slave_get_bus_info(ot_vi_pipe vi_pipe)
{
    return &g_imx347_slave_bus_info[vi_pipe];
}

ot_isp_sns_state *imx347_slave_get_ctx(ot_vi_pipe vi_pipe)
{
    return g_imx347_slave_sns_state[vi_pipe];
}

td_void imx347_slave_set_blc_clamp_value(ot_vi_pipe vi_pipe, td_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}


imx347_slave_video_mode_tbl g_imx347_slave_mode_tbl[IMX347_SLAVE_MODE_BUTT] = {
    {37125000,  750, 1237500, IMX347_SLAVE_VMAX_4M30_LINEAR,   IMX347_SLAVE_FULL_LINES_MAX,
     30, 0.05,  2688, 1520, 0, OT_WDR_MODE_NONE,       "IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* Imx347_slave Register Address */
#define IMX347_SLAVE_SHS1_ADDR                              0x3058
#define IMX347_SLAVE_GAIN_ADDR                              0x30e8
#define IMX347_SLAVE_HCG_ADDR                               0x3019
#define IMX347_SLAVE_VMAX_ADDR                              0x3030
#define IMX347_SLAVE_Y_OUT_SIZE_ADDR                        0x3056

#define imx347_slave_err_mode_print(sns_image_mode, sns_state) \
    do { \
        isp_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                      (sns_image_mode)->width,  \
                      (sns_image_mode)->height, \
                      (sns_image_mode)->fps,    \
                      (sns_state)->wdr_mode); \
    }while (0)

imx347_slave_video_mode_tbl *imx347_get_slave_mode_tbl(td_u8 img_mode)
{
    return &g_imx347_slave_mode_tbl[img_mode];
}

ot_isp_slave_sns_sync *imx347_get_slave_sync(ot_vi_pipe vi_pipe)
{
    return &g_imx347_slave_sync[vi_pipe];
}

td_s32 imx347_get_slave_bind_dev(ot_vi_pipe vi_pipe)
{
    return g_imx347_slave_bind_dev[vi_pipe];
}

td_u32 imx347_get_slave_sensor_mode_time(ot_vi_pipe vi_pipe)
{
    return g_imx347_slave_sensor_mode_time[vi_pipe];
}

ot_isp_sns_state *imx347_get_slave_sns_state(ot_vi_pipe vi_pipe)
{
    return g_imx347_slave_sns_state[vi_pipe];
}

static td_void cmos_get_ae_comm_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = IMX347_SLAVE_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns, 30fps */

    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 1;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 255 << ae_sns_dft->isp_dgain_shift; /* max 255 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = sns_state->fl_std * 30 / 2; /* 30fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }

    ae_sns_dft->max_iris_fno = OT_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_fno = OT_ISP_IRIS_F_NO_32_0;

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
    ae_sns_dft->max_again = 78572; /* max 78752 */
    ae_sns_dft->min_again = 1024; /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 1024; /* max 1024 */
    ae_sns_dft->min_dgain = 1024; /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */

    ae_sns_dft->max_int_time = sns_state->fl_std - 2; /* sub 2 */
    ae_sns_dft->min_int_time = 2; /* min 2 */
    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = 2; /* min 2 */
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
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
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
        sns_state->regs_info[0].i2c_data[6].data = low_8bits(vmax); /* array index 6 */
        sns_state->regs_info[0].i2c_data[7].data = high_8bits(vmax); /* array index 7 */
        sns_state->regs_info[0].i2c_data[8].data = higher_4bits(vmax); /* array index 8 */
    }
    return;
}

/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines, lines_max, vmax, vs_time;
    td_float max_fps, min_fps;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_void_return(ae_sns_dft);
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_imx347_slave_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_imx347_slave_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_imx347_slave_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_imx347_slave_mode_tbl[sns_state->img_mode].min_fps;
    vs_time = g_imx347_slave_mode_tbl[sns_state->img_mode].inck_per_vs;

    if ((fps > max_fps) || (fps < min_fps)) {
        isp_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
    vmax = (vmax > lines_max) ? lines_max : vmax;

    g_imx347_slave_sync[vi_pipe].vs_time = (td_u32)(vs_time * max_fps / div_0_to_1_float(fps));

    cmos_config_vmax(sns_state, vmax);

    sns_state->fl_std = vmax;
    sns_state->regs_info[0].slv_sync.slave_vs_time = g_imx347_slave_sync[vi_pipe].vs_time;
    ae_sns_dft->lines_per500ms = (td_u32)(lines * max_fps / 2); /* div 2 */
    ae_sns_dft->fps = fps;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - 2; /* sub 2 */
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (td_u32)((1000000000) / (sns_state->fl_std * div_0_to_1_float(fps))); /* 1000000000ns */
    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines_max;
    td_u32 vmax = IMX347_SLAVE_VMAX_4M30_LINEAR;
    td_u32 hs_time;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_void_return(ae_sns_dft);
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines_max = g_imx347_slave_mode_tbl[sns_state->img_mode].max_ver_lines;
    hs_time = g_imx347_slave_mode_tbl[sns_state->img_mode].inck_per_hs;

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        vmax = full_lines;
        vmax = (vmax > lines_max) ? lines_max : vmax;
        g_imx347_slave_sync[vi_pipe].vs_time = min((td_u64)hs_time * vmax, IMX347_SLAVE_VS_TIME_MAX);
        sns_state->regs_info[0].slv_sync.slave_vs_time = g_imx347_slave_sync[vi_pipe].vs_time;
        sns_state->fl[0] = vmax;
    }

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            sns_state->regs_info[0].i2c_data[6].data = vmax & 0xFF; /* index 6 */
            sns_state->regs_info[0].i2c_data[7].data = (vmax & 0xFF00) >> 8; /* index 7, shift 8 */
            sns_state->regs_info[0].i2c_data[8].data = (vmax & 0xF0000) >> 16; /* index 8, shift 16 */
            break;

        default:
            break;
    }

    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->max_int_time = sns_state->fl[0] - 2; /* max_int_time: Flstd - 2 */

    return;
}

static td_void cmos_inttime_update_linear(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 value;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    value = sns_state->fl[0] - int_time;
    value = min(value, IMX347_SLAVE_FULL_LINES_MAX);
    value = min(max(value, 3), sns_state->fl[0] - 2); /* max 3, sub 2 */

    sns_state->regs_info[0].i2c_data[0].data = (value & 0xFF);
    sns_state->regs_info[0].i2c_data[1].data = ((value & 0xFF00) >> 8); /* index 1, shift 8 */
    sns_state->regs_info[0].i2c_data[2].data = ((value & 0xF0000) >> 16); /* index 2, shift 16 */
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}
#define GAIN_NODE_NUM    262
#define AGAIN_NODE_NUM   127
#define DGAIN_NODE_NUM   140

static td_u32 g_gain_table[GAIN_NODE_NUM] = {
    1024,     1059,    1097,     1135,     1175,    1217,       1259,
    1304,     1349,    1397,     1446,     1497,    1549,       1604,
    1660,     1719,    1779,     1842,     1906,    1973,       2043,
    2048,     2119,    2194,     2271,     2351,    2434,       2519,
    2608,     2699,    2794,     2892,     2994,    3099,       3208,
    3321,     3438,    3559,     3684,     3813,    3947,       4086,
    4229,     4378,    4532,     4691,     4856,    5027,       5203,
    5386,     5576,    5772,     5974,     6184,    6402,       6627,
    6860,     7101,    7350,     7609,     7876,    8153,       8439,
    8736,     9043,    9361,     9690,    10030,   10383,      10748,
    11125,    11516,   11921,    12340,    12774,   13222,      13687,
    14168,    14666,   15182,    15715,    16267,   16839,      17431,
    18043,    18677,   19334,    20013,    20717,   21445,      22198,
    22978,    23786,   24622,    25487,    26383,   27310,      28270,
    29263,    30292,   31356,    32458,    33599,   34780,      36002,
    37267,    38577,   39932,    41336,    42788,   44292,      45849,
    47460,    49128,   50854,    52641,    54491,   56406,      58388,
    60440,    62564,   64763,    67039,    69395,   71833,      74358,
    76971,    79676,   82476,    85374,    88375,   91480,      94695,
    98023,   101468,  105034,    108725,   112545,  116501,    120595,
    124833,   129220,  133761,    138461,   143327,  148364,    153578,
    158975,   164562,  170345,    176331,   182528,  188942,    195582,
    202455,   209570,  216935,    224558,   232450,  240619,    249074,
    257827,   266888,  276267,    285976,   296026,  306429,    317197,
    328344,   339883,  351827,    364191,   376990,  390238,    403952,
    418147,   432842,  448053,    463799,   480098,  496969,    514434,
    532512,   551226,  570597,    590649,   611406,  632892,    655133,
    678156,   701988,  726657,    752194,   778627,  805990,    834314,
    863634,   893984,  925400,    957921,   991585,  1026431,  1062502,
    1099841, 1138491,  1178500,  1219916,   1262786, 1307163,  1353100,
    1400651, 1449872,  1500824,  1553566,   1608162, 1664676,  1723177,
    1783733, 1846417,  1911304,  1978472,   2048000, 2119971,  2194471,
    2271590, 2351418,  2434052,  2519590,   2608134, 2699789,  2794666,
    2892876, 2994538,  3099773,  3208706,   3321467, 3438190,  3559016,
    3684087, 3813554,  3947571,  4086297,   4229898, 4378546,  4532417,
    4691696, 4856573,  5027243,  5203912,   5386788, 5576092,  5772048,
    5974890, 6184861,  6402210,  6627198,   6860092, 7101170,  7350721,
    7609041, 7876439,  8153234
};

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
    int i;

    ot_unused(vi_pipe);
    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    if (*again_lin >= g_gain_table[AGAIN_NODE_NUM - 1]) {
        *again_lin = g_gain_table[AGAIN_NODE_NUM - 1];
        *again_db = AGAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (*again_lin < g_gain_table[i]) {
            *again_lin = g_gain_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }
    return;
}

static td_void cmos_dgain_calc_table(ot_vi_pipe vi_pipe, td_u32 *dgain_lin, td_u32 *dgain_db)
{
    int i;

    ot_unused(vi_pipe);
    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);

    if (*dgain_lin >= g_gain_table[DGAIN_NODE_NUM - 1]) {
        *dgain_lin = g_gain_table[DGAIN_NODE_NUM - 1];
        *dgain_db = DGAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < g_gain_table[i]) {
            *dgain_lin = g_gain_table[i - 1];
            *dgain_db = i - 1;
            break;
        }
    }
    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 hcg;
    td_u32 tmp;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (again >= 28) { /* HCG 28 */
        hcg = 0x1;
        again = again - 28; /* again sub 28 */
    } else {
        hcg = 0x0;
    }

    tmp = again + dgain;

    sns_state->regs_info[0].i2c_data[3].data = low_8bits(tmp); /* index 3 */
    sns_state->regs_info[0].i2c_data[4].data = higher_4bits(tmp); /* index 4 shift 8 */
    sns_state->regs_info[0].i2c_data[5].data = (hcg & 0xFF); /* index 5 */

    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_unused(man_ratio_enable);
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
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

    exp_func->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set           = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update    = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update      = cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    exp_func->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    exp_func->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return TD_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP                         4950
#define CALIBRATE_STATIC_WB_R_GAIN                    450
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    447

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              (-53)
#define CALIBRATE_AWB_P2                              309
#define CALIBRATE_AWB_Q1                              0
#define CALIBRATE_AWB_A1                              160279
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-110119)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static td_s32 cmos_get_awb_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(awb_sns_dft);
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
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
        case IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE:
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

static td_s32 cmos_get_isp_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(isp_def);
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
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
        default:
            cmos_get_isp_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    isp_def->sns_mode.sns_id = IMX347_SLAVE_ID;
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
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if (is_no_fe_pipe(vi_pipe)) {
        cmos_blc_def = &g_cmos_blc_virt_pipe;
    } else {
        cmos_blc_def = &g_cmos_blc;
    }

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), cmos_blc_def, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
            if (vi_pipe == 0) {
                black_level->auto_attr.black_level[0][i] = 800; /* blc 800 */
            } else if (vi_pipe == 1) {
                black_level->auto_attr.black_level[0][i] = 1200; /* blc 1200 */
            } else {
                black_level->auto_attr.black_level[0][i] = 800; /* blc 800 */
            }
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
    td_u32 full_lines_5fps, max_int_time_5fps;
    ot_isp_sns_state *sns_state = TD_NULL;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    check_ret(ot_mpi_isp_get_sns_slave_attr(vi_pipe, &g_imx347_slave_sync[vi_pipe]));

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE) {
            g_imx347_slave_sync[vi_pipe].vs_time = (td_u32)((g_imx347_slave_mode_tbl[sns_state->img_mode].inck_per_vs) *
                g_imx347_slave_mode_tbl[sns_state->img_mode].max_fps / 5); /* divide 5 */
            full_lines_5fps = (IMX347_SLAVE_VMAX_4M30_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    max_int_time_5fps = 4; /* max_int_time_5fps 4 */

    if (enable) { /* setup for ISP pixel calibration mode */
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_GAIN_ADDR, 0x00);

        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_VMAX_ADDR, low_8bits(full_lines_5fps));
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_VMAX_ADDR + 1, high_8bits(full_lines_5fps));
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_VMAX_ADDR + 2, higher_4bits(full_lines_5fps)); /* index 2 */

        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_SHS1_ADDR, low_8bits(max_int_time_5fps));
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_SHS1_ADDR + 1, high_8bits(max_int_time_5fps));
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_SHS1_ADDR + 2, higher_4bits(max_int_time_5fps)); /* index 2 */
    } else { /* setup for ISP 'normal mode' */
        sns_state->fl_std = (sns_state->fl_std > 0x1FFFF) ? 0x1FFFF : sns_state->fl_std;
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_VMAX_ADDR, low_8bits(sns_state->fl_std));
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_VMAX_ADDR + 1, high_8bits(sns_state->fl_std));
        imx347_slave_write_register(vi_pipe, IMX347_SLAVE_VMAX_ADDR + 2, higher_4bits(sns_state->fl_std)); /* index 2 */
        sns_state->sync_init = TD_FALSE;
    }

    check_ret(ot_mpi_isp_set_sns_slave_attr(vi_pipe, &g_imx347_slave_sync[vi_pipe]));

    return;
}

static td_s32 cmos_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init    = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear mode\n");
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
    sns_state->regs_info[0].com_bus.i2c_dev = g_imx347_slave_bus_info[vi_pipe].i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 9; /* reg_num 9 */

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = IMX347_SLAVE_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = IMX347_SLAVE_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = IMX347_SLAVE_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[0].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[0].reg_addr = IMX347_SLAVE_SHS1_ADDR;
    sns_state->regs_info[0].i2c_data[1].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[1].reg_addr = IMX347_SLAVE_SHS1_ADDR + 1;
    sns_state->regs_info[0].i2c_data[2].delay_frame_num = 0; /* index 2 */
    sns_state->regs_info[0].i2c_data[2].reg_addr = IMX347_SLAVE_SHS1_ADDR + 2; /* index 2, index 2 */

    sns_state->regs_info[0].i2c_data[3].delay_frame_num = 0; /* index 3 */
    sns_state->regs_info[0].i2c_data[3].reg_addr = IMX347_SLAVE_GAIN_ADDR; /* index 3 */

    sns_state->regs_info[0].i2c_data[4].delay_frame_num = 0; /* index 4 */
    sns_state->regs_info[0].i2c_data[4].reg_addr = IMX347_SLAVE_GAIN_ADDR + 1; /* index 4 */

    sns_state->regs_info[0].i2c_data[5].delay_frame_num = 0;  /* index 5 */
    sns_state->regs_info[0].i2c_data[5].reg_addr = IMX347_SLAVE_HCG_ADDR; /* index 5 */

    sns_state->regs_info[0].i2c_data[6].delay_frame_num = 1; /* index 6 */
    sns_state->regs_info[0].i2c_data[6].reg_addr = IMX347_SLAVE_VMAX_ADDR; /* index 6 */
    sns_state->regs_info[0].i2c_data[7].delay_frame_num = 1; /* index 7 */
    sns_state->regs_info[0].i2c_data[7].reg_addr = IMX347_SLAVE_VMAX_ADDR + 1; /* index 7 */
    sns_state->regs_info[0].i2c_data[8].delay_frame_num = 1; /* index 8 */
    sns_state->regs_info[0].i2c_data[8].reg_addr = IMX347_SLAVE_VMAX_ADDR + 2; /* index 8, index 2 */

    sns_state->regs_info[0].slv_sync.update = TD_TRUE;
    sns_state->regs_info[0].slv_sync.delay_frame_num = 1; /* delay_frame 1 */
    sns_state->regs_info[0].slv_sync.slave_bind_dev = g_imx347_slave_bind_dev[vi_pipe];
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
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == TD_FALSE) || (sns_regs_info->config == TD_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);

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
    switch (sns_image_mode) {
        case IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE:
            sns_state->fl_std         = IMX347_SLAVE_VMAX_4M30_LINEAR;
            g_imx347_slave_state[vi_pipe].hcg = 0x0;
            break;
        default:
            sns_state->fl_std         = IMX347_SLAVE_VMAX_4M30_LINEAR;
            g_imx347_slave_state[vi_pipe].hcg = 0x0;
            break;
    }
}

static td_s32 cmos_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{
    td_u32 i;
    td_u8 image_mode;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pointer_return(sns_image_mode);
    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < IMX347_SLAVE_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_imx347_slave_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_imx347_slave_mode_tbl[i].width &&
            sns_image_mode->height <= g_imx347_slave_mode_tbl[i].height &&
            sns_state->wdr_mode == g_imx347_slave_mode_tbl[i].wdr_mode) {
            image_mode = (imx347_slave_res_mode)i;
            break;
        }
    }

    if (i >= IMX347_SLAVE_MODE_BUTT) {
        imx347_slave_err_mode_print(sns_image_mode, sns_state);
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

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode = IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE;
    sns_state->wdr_mode = OT_WDR_MODE_NONE;
    sns_state->fl_std  = IMX347_SLAVE_VMAX_4M30_LINEAR;
    sns_state->fl[0] = IMX347_SLAVE_VMAX_4M30_LINEAR;
    sns_state->fl[1] = IMX347_SLAVE_VMAX_4M30_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    return;
}

static td_s32 cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    sns_check_pointer_return(sensor_exp_func);

    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init         = imx347_slave_init;
    sensor_exp_func->pfn_cmos_sns_exit         = imx347_slave_exit;
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

static td_s32 imx347_slave_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    g_imx347_slave_bus_info[vi_pipe].i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 sensor_ctx_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == TD_NULL) {
        sns_state_ctx = (ot_isp_sns_state *)malloc(sizeof(ot_isp_sns_state));
        if (sns_state_ctx == TD_NULL) {
            isp_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_ERR_ISP_NOMEM;
        }
    }

    (td_void)memset_s(sns_state_ctx, sizeof(ot_isp_sns_state), 0, sizeof(ot_isp_sns_state));

    imx347_slave_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return TD_SUCCESS;
}

static td_void sensor_ctx_exit(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    imx347_slave_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    imx347_slave_sensor_reset_ctx(vi_pipe);
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

    sns_attr_info.sns_id = IMX347_SLAVE_ID;
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

    ret = ot_mpi_isp_sensor_unreg_callback(vi_pipe, IMX347_SLAVE_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = ot_mpi_ae_sensor_unreg_callback(vi_pipe, ae_lib, IMX347_SLAVE_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_mpi_awb_sensor_unreg_callback(vi_pipe, awb_lib, IMX347_SLAVE_ID);
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
    g_ae_route_ex_valid[vi_pipe] = init_attr->ae_route_ex_valid;
    g_ae_stat_pos[vi_pipe]       = init_attr->ae_stat_pos;
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

ot_isp_sns_obj g_sns_imx347_slave_obj = {
    .pfn_register_callback     = sensor_register_callback,
    .pfn_un_register_callback  = sensor_unregister_callback,
    .pfn_standby               = imx347_slave_standby,
    .pfn_restart               = imx347_slave_restart,
    .pfn_mirror_flip           = imx347_mirror_flip,
    .pfn_set_blc_clamp         = imx347_blc_clamp,
    .pfn_write_reg             = imx347_slave_write_register,
    .pfn_read_reg              = imx347_slave_read_register,
    .pfn_set_bus_info          = imx347_slave_set_bus_info,
    .pfn_set_init              = sensor_set_init
};
