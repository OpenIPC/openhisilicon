/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "gc4023_cmos.h"
#include "gc4023_cmos_param.h"
#include "gc4023_sensor_ctrl.h"

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
static cis_info g_gc4023_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = GC4023_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        .bus_info = { .i2c_dev = -1 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
            {
                GC4023_VMAX_LINEAR,
                GC4023_FULL_LINES_MAX_LINEAR,
                GC4023_FPS_MAX_LINEAR,
                GC4023_FPS_MIN_LINEAR,
                GC4023_WIDTH_LINEAR,
                GC4023_HEIGHT_LINEAR,
                GC4023_MODE_LINEAR,
                OT_WDR_MODE_NONE,
                "GC4023_4M_30FPS_10BIT_LINEAR_MODE"
             },
        },
        .i2c.fd = -1,
    }
};

#define GC4023_REG_VAL_TAB_ROW    25
#define GC4023_REG_VAL_TAB_COL    9

static td_u32 g_reg_val_table[GC4023_REG_VAL_TAB_ROW][GC4023_REG_VAL_TAB_COL] = {
    /* 02cd 614  0615  0218  1467  1468  00b8  00b9  1466 */
    {0x92, 0x00, 0x00, 0x00, 0x2f, 0x2f, 0x01, 0x00, 0x1e},
    {0x92, 0x80, 0x02, 0x00, 0x30, 0x30, 0x01, 0x0B, 0x1e},
    {0x92, 0x01, 0x00, 0x00, 0x29, 0x29, 0x01, 0x19, 0x18},
    {0x92, 0x81, 0x02, 0x00, 0x2b, 0x2b, 0x01, 0x2A, 0x18},
    {0x92, 0x02, 0x00, 0x00, 0x27, 0x27, 0x02, 0x00, 0x14},
    {0x92, 0x82, 0x02, 0x00, 0x28, 0x28, 0x02, 0x17, 0x14},
    {0x92, 0x03, 0x00, 0x00, 0x29, 0x29, 0x02, 0x33, 0x14},
    {0x92, 0x83, 0x02, 0x00, 0x2b, 0x2b, 0x03, 0x14, 0x14},
    {0x92, 0x04, 0x00, 0x00, 0x2c, 0x2c, 0x04, 0x00, 0x14},
    {0x99, 0x80, 0x02, 0x20, 0x23, 0x23, 0x04, 0x2F, 0x16},
    {0x99, 0x01, 0x00, 0x20, 0x24, 0x24, 0x05, 0x26, 0x18},
    {0x99, 0x81, 0x02, 0x20, 0x27, 0x27, 0x06, 0x28, 0x1a},
    {0x99, 0x02, 0x00, 0x20, 0x29, 0x29, 0x08, 0x00, 0x1c},
    {0x99, 0x82, 0x02, 0x20, 0x2c, 0x2c, 0x09, 0x1E, 0x1f},
    {0x99, 0x03, 0x00, 0x20, 0x31, 0x31, 0x0B, 0x0C, 0x24},
    {0x99, 0x83, 0x02, 0x20, 0x35, 0x35, 0x0D, 0x11, 0x29},
    {0x99, 0x04, 0x00, 0x20, 0x3a, 0x3a, 0x10, 0x00, 0x2E},
    {0x99, 0x84, 0x02, 0x20, 0x3c, 0x3c, 0x12, 0x3D, 0x30},
    {0x99, 0x05, 0x00, 0x20, 0x3d, 0x3d, 0x16, 0x19, 0x32},
    {0x99, 0x85, 0x02, 0x20, 0x3f, 0x3f, 0x1A, 0x22, 0x34},
    {0x99, 0xb5, 0x04, 0x20, 0x3d, 0x3d, 0x20, 0x00, 0x32},
    {0x99, 0x85, 0x05, 0x20, 0x3b, 0x3b, 0x25, 0x3A, 0x30},
    {0x99, 0x05, 0x08, 0x20, 0x34, 0x34, 0x2C, 0x33, 0x29},
    {0x99, 0x45, 0x09, 0x20, 0x2b, 0x2b, 0x35, 0x05, 0x20},
    {0x99, 0x55, 0x0a, 0x20, 0x26, 0x26, 0x40, 0x00, 0x1a},
};

/****************************************************************************
  Again & Dgain table for TABLE Mode                                        *
 ****************************************************************************/
static td_u32 g_analog_gain_table[GC4023_REG_VAL_TAB_ROW] = {
    1024,
    1213,
    1448,
    1715,
    2006,
    2385,
    2833,
    3368,
    3935,
    4646,
    5493,
    6511,
    7608,
    9039,
    10737,
    12733,
    14861,
    17837,
    21317,
    25286,
    30053,
    32386,
    42098,
    49882,
    60263,
};

/****************************************************************************
 * common functions                                                         *
 ****************************************************************************/
static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_gc4023_info[vi_pipe];
}

static ot_isp_sns_state *cmos_get_state(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return cmos_get_info(vi_pipe)->sns_state;
}

static void cmos_err_mode_print(const ot_isp_cmos_sns_image_mode *sns_image_mode, const ot_isp_sns_state *sns_state)
{
    isp_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n",
        (sns_image_mode)->width, (sns_image_mode)->height, (sns_image_mode)->fps, (sns_state)->wdr_mode);
}

static td_void cmos_get_ae_comm_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    td_float max_fps = STANDARD_FPS; /* maxfps 30 */

    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = FLICKER_FREQ; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = GC4023_FULL_LINES_MAX_LINEAR;
    ae_sns_dft->hmax_times = (1000000000) / (GC4023_VMAX_LINEAR * STANDARD_FPS); /* 1000000000ns, 30fps */

    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = INT_TIME_ACCURACY; /* accuracy: 1 */
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = AGAIN_ACCURACY; /* accuracy: 1 */

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = DGAIN_ACCURACY; /* accuracy: 0.015625 */

    ae_sns_dft->isp_dgain_shift = ISP_DGAIN_SHIFT; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = ISP_DGAIN_TARGET_MIN << ae_sns_dft->isp_dgain_shift; /* min 1 */
    ae_sns_dft->max_isp_dgain_target = ISP_DGAIN_TARGET_MAX << ae_sns_dft->isp_dgain_shift; /* max 16 */
    if (cis->lines_per500ms == 0) {
        ae_sns_dft->lines_per500ms = GC4023_VMAX_LINEAR * max_fps / 2; /* 30fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = cis->lines_per500ms;
    }
    (td_void)memcpy_s(&ae_sns_dft->piris_attr, sizeof(ot_isp_piris_attr), &g_piris, sizeof(ot_isp_piris_attr));
    ae_sns_dft->max_iris_fno = OT_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid = TD_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = cis->quick_start_en;
    ae_sns_dft->quick_start.black_frame_num = 0;

    return;
}

static td_void cmos_get_ae_linear_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_again = GC4023_AGAIN_MAX; /* 64*1024 = 65536 */
    ae_sns_dft->min_again = GC4023_AGAIN_MIN;  /* min 16 = 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = GC4023_DGAIN_MAX; /* max:16X, 512 */
    ae_sns_dft->min_dgain = GC4023_DGAIN_MIN;  /* min:1X, 64 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = AE_COMENSATION_DEFAULT;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = cis->init_exposure ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR;

    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
    ae_sns_dft->min_int_time = 2; /* min int 2 */
    ae_sns_dft->max_int_time_target = MAX_INT_TIME_TARGET; /* max int 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->ae_route_ex_valid = cis->ae_route_ex_valid;
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route),
                      &cis->init_ae_route,  sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &cis->init_ae_route_ex, sizeof(ot_isp_ae_route_ex));
    return;
}

static td_s32 cmos_get_ae_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;

    sns_check_pointer_return(sns_state);

    (td_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route), 0, sizeof(ot_isp_ae_route));

    cmos_get_ae_comm_default(cis, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(cis, ae_sns_dft, sns_state);
            break;
        default:
            cmos_get_ae_linear_default(cis, ae_sns_dft, sns_state);
            break;
    }

    return TD_SUCCESS;
}

static td_void cmos_config_vmax(ot_isp_sns_state *sns_state, td_u32 vmax)
{
    sns_state->regs_info[0].i2c_data[14].data = high_8bits(vmax); /* index 14 */
    sns_state->regs_info[0].i2c_data[15].data = low_8bits(vmax);  /* index 15 */

    return;
}

/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines, lines_max, vmax;
    td_float max_fps, min_fps;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    lines = cis->mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = cis->mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = cis->mode_tbl[sns_state->img_mode].max_fps;
    min_fps = cis->mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        isp_err_trace("ISP sensor gc4023 Not support Fps: %f\n", fps);
        return;
    }

    vmax = lines * max_fps / div_0_to_1_float(fps);
    vmax = (vmax > lines_max) ? lines_max : vmax;

    cmos_config_vmax(sns_state, vmax);

    sns_state->fl_std = vmax;
    ae_sns_dft->lines_per500ms = lines * STANDARD_FPS / 2; /* div 2 */

    ae_sns_dft->fps = fps;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (1000000000) / (sns_state->fl_std * div_0_to_1_float(fps)); /* 1000000000ns */

    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 vmax;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;
    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    vmax = full_lines + FL_OFFSET_LINEAR;
    vmax = clip3(vmax, GC4023_VMAX_LINEAR, GC4023_FRAME_RATE_MIN);
    sns_state->fl[0] = vmax;

    sns_state->regs_info[0].i2c_data[14].data = high_8bits(vmax); /* index 14 */
    sns_state->regs_info[0].i2c_data[15].data = low_8bits(vmax);  /* index 15 */

    ae_sns_dft->full_lines = full_lines;
    ae_sns_dft->max_int_time = sns_state->fl[0] - FL_OFFSET_LINEAR;

    return;
}

static td_void cmos_inttime_update_linear(ot_isp_sns_state *sns_state, td_u32 int_time)
{
    td_u32 val;

    val = (int_time > GC4023_FULL_LINES_MAX_LINEAR) ? GC4023_FULL_LINES_MAX_LINEAR : int_time;

    sns_state->regs_info[0].i2c_data[0].data = high_8bits(val);
    sns_state->regs_info[0].i2c_data[1].data = low_8bits(val);

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    cmos_inttime_update_linear(sns_state, int_time);

    return;
}

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *pu32_again_lin, td_u32 *pu32_again_db)
{
    td_u32 again;
    int i;
    static td_u8 againmax = GC4023_REG_VAL_TAB_ROW - 1; /* adgain max 25 */
    ot_unused(vi_pipe);
    sns_check_pointer_void_return(pu32_again_lin);
    sns_check_pointer_void_return(pu32_again_db);

    again = *pu32_again_lin;

    if (again >= g_analog_gain_table[againmax]) {
        *pu32_again_lin = g_analog_gain_table[againmax];
        *pu32_again_db = againmax;
    } else {
        for (i = 1; i < GC4023_REG_VAL_TAB_ROW; i++) {
            if (again < g_analog_gain_table[i]) {
                *pu32_again_lin = g_analog_gain_table[i - 1];
                *pu32_again_db = i - 1;
                break;
            }
        }
    }

    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 pu32_again, td_u32 pu32_dgain)
{
    td_u8 dgain_high, dgain_low;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    dgain_high = (pu32_dgain >> 6) & 0x0F; /* shift 6 */
    dgain_low  = (pu32_dgain & 0x3F) << 2; /* shift 2 */

    sns_state->regs_info[0].i2c_data[2].data = g_reg_val_table[pu32_again][0]; /* index 2 (0x02cd) */
    sns_state->regs_info[0].i2c_data[3].data = 0x2d; /* index 3, group hold */
    sns_state->regs_info[0].i2c_data[4].data = g_reg_val_table[pu32_again][1]; /* index 4, index 1 (0x0614) */
    sns_state->regs_info[0].i2c_data[5].data = g_reg_val_table[pu32_again][2]; /* index 5, index 2 (0x0615) */
    sns_state->regs_info[0].i2c_data[6].data = 0x28; /* index 6, group hold */
    sns_state->regs_info[0].i2c_data[7].data  = g_reg_val_table[pu32_again][3]; /* index 7, index 3 (0x0218) */
    sns_state->regs_info[0].i2c_data[16].data = g_reg_val_table[pu32_again][8]; /* index 16, index 8 (0x1466) */
    sns_state->regs_info[0].i2c_data[8].data  = g_reg_val_table[pu32_again][4]; /* index 8, index 4 (0x1467) */
    sns_state->regs_info[0].i2c_data[9].data  = g_reg_val_table[pu32_again][5]; /* index 9, index 5 (0x1468) */
    sns_state->regs_info[0].i2c_data[10].data = g_reg_val_table[pu32_again][6]; /* index 10, index 6 (0x00b8) */
    sns_state->regs_info[0].i2c_data[11].data = g_reg_val_table[pu32_again][7]; /* index 11, index 7 (0x00b9) */

    /* dgain */
    sns_state->regs_info[0].i2c_data[12].data = dgain_high; /* index 12 (0x0064) */
    sns_state->regs_info[0].i2c_data[13].data = dgain_low;  /* index 13 (0x0065) */

    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_unused(vi_pipe);
    ot_unused(man_ratio_enable);
    ot_unused(ratio);
    ot_unused(int_time);
    ot_unused(lf_max_int_time);

    return;
}

/* Only used in LINE_WDR mode */
static td_void cmos_ae_fswdr_attr_set(ot_vi_pipe vi_pipe, ot_isp_ae_fswdr_attr *ae_fswdr_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_fswdr_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    cis->fswdr_mode = ae_fswdr_attr->fswdr_mode;
    cis->max_time_get_cnt = 0;

    return;
}

static td_void cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));

    exp_func->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set            = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update     = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update       = cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    exp_func->pfn_cmos_dgain_calc_table   = TD_NULL;
    exp_func->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return;
}

static td_s32 cmos_awb_get_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(awb_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(awb_sns_dft, sizeof(ot_isp_awb_sensor_default), 0, sizeof(ot_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP; /* wb_ref_temp 4950 */

    awb_sns_dft->gain_offset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[1] = CALIBRATE_STATIC_WB_GR_GAIN; /* index 1 */
    awb_sns_dft->gain_offset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
    awb_sns_dft->gain_offset[3] = CALIBRATE_STATIC_WB_B_GAIN;  /* index 3 */

    awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2; /* index 1 */
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

    awb_sns_dft->init_rgain = cis->init_wb_r_gain;
    awb_sns_dft->init_ggain = cis->init_wb_g_gain;
    awb_sns_dft->init_bgain = cis->init_wb_b_gain;
    awb_sns_dft->sample_rgain = cis->sample_r_gain;
    awb_sns_dft->sample_bgain = cis->sample_b_gain;

    return TD_SUCCESS;
}

static td_void cmos_init_awb_exp_function(ot_isp_awb_sensor_exp_func *exp_func)
{
    (td_void)memset_s(exp_func, sizeof(ot_isp_awb_sensor_exp_func), 0, sizeof(ot_isp_awb_sensor_exp_func));

    exp_func->pfn_cmos_get_awb_default = cmos_awb_get_default;

    return;
}

static td_void cmos_isp_get_dng_default(const ot_isp_sns_state *sns_state, ot_isp_cmos_default *isp_def)
{
    ot_isp_cmos_dng_color_param dng_color_param = {{ 286, 256, 608 }, { 415, 256, 429 },
        { 2810, { 0x01AC, 0x8093, 0x8019, 0x8070, 0x01EA, 0x807A, 0x802A, 0x80F3, 0x021D }},
        { 4940, { 0x01D7, 0x8084, 0x8053, 0x8053, 0x01D9, 0x8086, 0x8010, 0x80B3, 0x01C3 }}};

    (td_void)memcpy_s(&isp_def->dng_color_param, sizeof(ot_isp_cmos_dng_color_param), &dng_color_param,
                      sizeof(ot_isp_cmos_dng_color_param));

    switch (sns_state->img_mode) {
        case GC4023_4M_30FPS_10BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 12bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR; /* max 4095 */
            break;
        default:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 12bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR; /* max 4095 */
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

static void cmos_isp_get_linear_default(ot_isp_cmos_default *isp_def)
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
    isp_def->key.bit1_ca = 1;
    isp_def->ca = &g_cmos_ca;
    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
    return;
}

static td_s32 cmos_isp_get_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(isp_def);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(isp_def, sizeof(ot_isp_cmos_default), 0, sizeof(ot_isp_cmos_default));
#ifdef CONFIG_OT_ISP_CA_SUPPORT
    isp_def->key.bit1_ca      = 1;
    isp_def->ca               = &g_cmos_ca;
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
    isp_def->key.bit1_pregamma = 0;
    isp_def->pregamma          = &g_cmos_pregamma;
#endif
    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            cmos_isp_get_linear_default(isp_def);
            break;
        default:
            cmos_isp_get_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    isp_def->sns_mode.sns_id = GC4023_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_isp_get_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_isp_get_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32  i;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(black_level);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
            black_level->auto_attr.black_level[0][i] = BLACK_LEVEL_DEFAULT;
        }
    } else { /* black level of DOL mode */
        for (i = 0; i < OT_ISP_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][1] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][2] = BLACK_LEVEL_DEFAULT; /* index 2 */
            black_level->auto_attr.black_level[i][3] = BLACK_LEVEL_DEFAULT; /* index 3 */
        }
    }

    return TD_SUCCESS;
}

static td_s32 cmos_isp_get_blc_clamp_info(ot_vi_pipe vi_pipe, td_bool *blc_clamp_en)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(blc_clamp_en);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    *blc_clamp_en = cis->blc_clamp_info;

    return TD_SUCCESS;
}

static td_s32 cmos_isp_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);

    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear mode\n");
            break;
        default:
            isp_err_trace("Not support this mode!\n");
            return TD_FAILURE;
    }

    (td_void)memset_s(sns_state->wdr_int_time, sizeof(sns_state->wdr_int_time), 0, sizeof(sns_state->wdr_int_time));

    return TD_SUCCESS;
}

static td_void cmos_comm_sns_reg_info_init(cis_info *cis, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    sns_state->regs_info[0].sns_type = OT_ISP_SNS_TYPE_I2C;
    sns_state->regs_info[0].com_bus.i2c_dev = cis->bus_info.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 17; /* reg num 17 */

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = GC4023_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = GC4023_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = GC4023_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[0].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[0].reg_addr = GC4023_EXPO_H_ADDR; // 0x0202
    sns_state->regs_info[0].i2c_data[1].delay_frame_num = 0; /* index 1 */
    sns_state->regs_info[0].i2c_data[1].reg_addr = GC4023_EXPO_L_ADDR; // 0x0203 /* index 1 */

    /* again */
    sns_state->regs_info[0].i2c_data[2].delay_frame_num = 0; /* index 2 */
    sns_state->regs_info[0].i2c_data[2].reg_addr = 0x02cd; /* index 2 */
    sns_state->regs_info[0].i2c_data[3].delay_frame_num = 0; /* index 3 */
    sns_state->regs_info[0].i2c_data[3].reg_addr = 0x031d; /* group hold */ /* index 3 */
    sns_state->regs_info[0].i2c_data[4].delay_frame_num = 0; /* index 4 */
    sns_state->regs_info[0].i2c_data[4].reg_addr = GC4023_AGAIN_L_ADDR; // 0x0614 /* index 4 */
    sns_state->regs_info[0].i2c_data[5].delay_frame_num = 0; /* index 5 */
    sns_state->regs_info[0].i2c_data[5].reg_addr = GC4023_AGAIN_M_ADDR; // 0x0615 /* index 5 */
    sns_state->regs_info[0].i2c_data[6].delay_frame_num = 0; /* index 6 */
    sns_state->regs_info[0].i2c_data[6].reg_addr = 0x031d; /* group hold */ /* index 6 */

    sns_state->regs_info[0].i2c_data[7].delay_frame_num = 0; /* index 7 */
    sns_state->regs_info[0].i2c_data[7].reg_addr = GC4023_AGAIN_H_ADDR; // 0x0218 /* index 7 */
    sns_state->regs_info[0].i2c_data[16].delay_frame_num = 0; /* index 16 */
    sns_state->regs_info[0].i2c_data[16].reg_addr = 0x1466; /* index 16 */
    sns_state->regs_info[0].i2c_data[8].delay_frame_num = 0; /* index 8 */
    sns_state->regs_info[0].i2c_data[8].reg_addr = 0x1467; /* index 8 */
    sns_state->regs_info[0].i2c_data[9].delay_frame_num = 0; /* index 9 */
    sns_state->regs_info[0].i2c_data[9].reg_addr = 0x1468;  /* index 9 */

    sns_state->regs_info[0].i2c_data[10].delay_frame_num = 0;  /* index 10 */
    sns_state->regs_info[0].i2c_data[10].reg_addr = GC4023_COL_GAIN_H_ADDR; // 0x00b8 /* index 10 */
    sns_state->regs_info[0].i2c_data[11].delay_frame_num = 0; /* index 11 */
    sns_state->regs_info[0].i2c_data[11].reg_addr = GC4023_COL_GAIN_L_ADDR; // 0x00b9 /* index 11 */

    /* auto-pregain */
    sns_state->regs_info[0].i2c_data[12].delay_frame_num = 0; /* index 12 */
    sns_state->regs_info[0].i2c_data[12].reg_addr  = GC2053_AUTO_PREGAIN_ADDR_H; // 0x0064  /* index 12 */
    sns_state->regs_info[0].i2c_data[13].delay_frame_num = 0; /* index 13 */
    sns_state->regs_info[0].i2c_data[13].reg_addr = GC2053_AUTO_PREGAIN_ADDR_L; // 0x0065 /* index 13 */

    /* vmax */
    sns_state->regs_info[0].i2c_data[14].delay_frame_num = 0; /* index 14 */
    sns_state->regs_info[0].i2c_data[14].reg_addr = GC4023_VMAX_H_ADDR; // 0x0340 /* index 14 */
    sns_state->regs_info[0].i2c_data[15].delay_frame_num = 0; /* index 15 */
    sns_state->regs_info[0].i2c_data[15].reg_addr = GC4023_VMAX_L_ADDR; // 0x0341 /* index 15 */

    return;
}

static td_void cmos_sns_reg_info_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    ot_unused(vi_pipe);

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        if (sns_state->regs_info[0].i2c_data[i].reg_addr == 0x031d) {
            sns_state->regs_info[0].i2c_data[i].update = TD_FALSE;
        } else {
            sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        }
    }

    return;
}

static td_s32 cmos_isp_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(sns_regs_info);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == TD_FALSE) || (sns_regs_info->config == TD_FALSE)) {
        cmos_comm_sns_reg_info_init(cis, sns_state);
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

static td_void cmos_isp_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sns_image_mode, ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    switch (sns_image_mode) {
        case GC4023_4M_30FPS_10BIT_LINEAR_MODE:
            sns_state->fl_std = GC4023_VMAX_LINEAR;
            break;
        default:
            sns_state->fl_std = GC4023_VMAX_LINEAR;
            break;
    }

    return;
}

static td_s32 cmos_isp_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{
    td_u32 i;
    td_u8 image_mode;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(sns_image_mode);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < GC4023_MODE_MAX; i++) {
        if (sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->width <= cis->mode_tbl[i].width &&
            sns_image_mode->height <= cis->mode_tbl[i].height &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (gc4023_res_mode)i;
            break;
        }
    }

    if (i >= GC4023_MODE_MAX) {
        cmos_err_mode_print(sns_image_mode, sns_state);
        return TD_FAILURE;
    }

    cmos_isp_config_image_mode_param(vi_pipe, image_mode, sns_state);

    if ((sns_state->init == TD_TRUE) && (image_mode == sns_state->img_mode)) {
        return OT_ISP_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch image_mode */
    }

    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];

    return TD_SUCCESS;
}

static void cmos_isp_init(ot_vi_pipe vi_pipe)
{
    td_s32           ret;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sensor_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sensor_state = cis->sns_state;
    sns_check_pointer_void_return(sensor_state);

    cis->i2c.addr = GC4023_I2C_ADDR;
    cis->i2c.addr_byte_num = GC4023_ADDR_BYTE;
    cis->i2c.data_byte_num = GC4023_DATA_BYTE;

    ret = cis_i2c_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    gc4023_linear_4m30_10bit_init(cis);

    sensor_state->init = TD_TRUE;

    return;
}

static void cmos_isp_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    ret = cis_i2c_exit(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("SC500AI exit failed!\n");
    }

    return;
}

static td_void cmos_isp_global_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = GC4023_4M_30FPS_10BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = GC4023_VMAX_LINEAR;
    sns_state->fl[0]     = GC4023_VMAX_LINEAR;
    sns_state->fl[1]     = GC4023_VMAX_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_void cmos_isp_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    sns_check_pipe_void_return(vi_pipe);

    return;
}


static td_void cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init              = cmos_isp_init;
    sensor_exp_func->pfn_cmos_sns_exit              = cmos_isp_exit;
    sensor_exp_func->pfn_cmos_sns_global_init       = cmos_isp_global_init;
    sensor_exp_func->pfn_cmos_set_image_mode        = cmos_isp_set_image_mode;
    sensor_exp_func->pfn_cmos_set_wdr_mode          = cmos_isp_set_wdr_mode;
    sensor_exp_func->pfn_cmos_get_isp_default       = cmos_isp_get_default;
    sensor_exp_func->pfn_cmos_get_isp_black_level   = cmos_isp_get_black_level;
    sensor_exp_func->pfn_cmos_get_blc_clamp_info    = cmos_isp_get_blc_clamp_info;
    sensor_exp_func->pfn_cmos_set_pixel_detect      = cmos_isp_set_pixel_detect;
    sensor_exp_func->pfn_cmos_get_sns_reg_info      = cmos_isp_get_sns_regs_info;
}

static td_s32 cmos_register_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib, ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;
    cis_register reg = {0};
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_lib);
    sns_check_pointer_return(awb_lib);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis->pipe = vi_pipe;

    reg.ae_lib = ae_lib;
    reg.awb_lib = awb_lib;

    cmos_init_sensor_exp_function(&reg.isp_register.sns_exp);
    cmos_init_ae_exp_function(&reg.ae_register.sns_exp);
    cmos_init_awb_exp_function(&reg.awb_register.sns_exp);

    ret = cis_register_callback(cis, &reg);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cis_register_callback failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 cmos_unregister_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib, ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;
    cis_register reg = {0};
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_lib);
    sns_check_pointer_return(awb_lib);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    reg.ae_lib = ae_lib;
    reg.awb_lib = awb_lib;
    ret = cis_unregister_callback(cis, &reg);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cis_register_callback failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static void cmos_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static void cmos_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static td_s32 cmos_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_check_return(cis_write_reg(&cis->i2c, addr, data));

    return TD_SUCCESS;
}

static td_s32 cmos_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

static td_s32 cmos_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis->bus_info.i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 cmos_set_init(ot_vi_pipe vi_pipe, ot_isp_init_attr *init_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(init_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis_init_attr(cis, init_attr);

    return TD_SUCCESS;
}

ot_isp_sns_obj g_sns_gc4023_obj = {
    .pfn_register_callback     = cmos_register_callback,
    .pfn_un_register_callback  = cmos_unregister_callback,
    .pfn_standby               = cmos_standby,
    .pfn_restart               = cmos_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = cmos_write_register,
    .pfn_read_reg              = cmos_read_register,
    .pfn_set_bus_info          = cmos_set_bus_info,
    .pfn_set_init              = cmos_set_init
};

ot_isp_sns_obj *gc4023_get_obj(td_void)
{
    return &g_sns_gc4023_obj;
}
