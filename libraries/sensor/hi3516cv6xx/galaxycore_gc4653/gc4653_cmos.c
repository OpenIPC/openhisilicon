/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "gc4653_cmos.h"
#include "gc4653_cmos_param.h"
#include "gc4653_sensor_ctrl.h"

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
static cis_info g_gc4653_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = GC4653_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        /*
         * Bus 0 until pfn_set_bus_info says otherwise: isp_sensor_init()
         * skips pfn_cmos_sns_init for an I2C sensor whose com_bus.i2c_dev
         * is -1, so the sensor would never be programmed.
         */
        .bus_info = { .i2c_dev = 0 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
            {
                GC4653_VMAX_LINEAR,
                GC4653_FULL_LINES_MAX_LINEAR,
                GC4653_FPS_MAX_LINEAR,
                GC4653_FPS_MIN_LINEAR,
                GC4653_WIDTH_LINEAR,
                GC4653_HEIGHT_LINEAR,
                GC4653_MODE_LINEAR,
                OT_WDR_MODE_NONE,
                "GC4653_4M30_10BIT_LINEAR_MODE"
             },
        },
        .i2c.fd = -1,
    }
};

/*
 * Analog gain: 26 steps of 1x .. 75.84x, each one a row of seven register
 * values, as the hi3516ev200 driver has them. Digital gain is AE's, in 1/64
 * steps: 0x020e takes bits 11:6, 0x020f bits 5:0 shifted up by two.
 */
#define GC4653_SNS_REGS 13

static const td_u32 g_gc4653_again[GC4653_AGAIN_ROWS] = {
    1024,  1213,  1434,  1699,  2048,  2427,  2867,  3398,  4096,
    4854,  5734,  6795,  8192,  9708,  11469, 13591, 16384, 19415,
    22938, 27181, 32768, 38830, 45875, 54362, 65536, 77660
};

static const td_u8 g_gc4653_again_regs[GC4653_AGAIN_ROWS][7] = {
    { 0x00, 0x00, 0x01, 0x00, 0x30, 0x1e, 0x5c },
    { 0x20, 0x00, 0x01, 0x0b, 0x30, 0x1e, 0x5c },
    { 0x01, 0x00, 0x01, 0x19, 0x30, 0x1d, 0x5b },
    { 0x21, 0x00, 0x01, 0x2a, 0x30, 0x1e, 0x5c },
    { 0x02, 0x00, 0x02, 0x00, 0x30, 0x1e, 0x5c },
    { 0x22, 0x00, 0x02, 0x17, 0x30, 0x1d, 0x5b },
    { 0x03, 0x00, 0x02, 0x33, 0x20, 0x16, 0x54 },
    { 0x23, 0x00, 0x03, 0x14, 0x20, 0x17, 0x55 },
    { 0x04, 0x00, 0x04, 0x00, 0x20, 0x17, 0x55 },
    { 0x24, 0x00, 0x04, 0x2f, 0x20, 0x19, 0x57 },
    { 0x05, 0x00, 0x05, 0x26, 0x20, 0x19, 0x57 },
    { 0x25, 0x00, 0x06, 0x28, 0x20, 0x1b, 0x59 },
    { 0x0c, 0x00, 0x08, 0x00, 0x20, 0x1d, 0x5b },
    { 0x2c, 0x00, 0x09, 0x1e, 0x20, 0x1f, 0x5d },
    { 0x0d, 0x00, 0x0b, 0x0c, 0x20, 0x21, 0x5f },
    { 0x2d, 0x00, 0x0d, 0x11, 0x20, 0x24, 0x62 },
    { 0x1c, 0x00, 0x10, 0x00, 0x20, 0x26, 0x64 },
    { 0x3c, 0x00, 0x12, 0x3d, 0x18, 0x2a, 0x68 },
    { 0x5c, 0x00, 0x16, 0x19, 0x18, 0x2c, 0x6a },
    { 0x7c, 0x00, 0x1a, 0x22, 0x18, 0x2e, 0x6c },
    { 0x9c, 0x00, 0x20, 0x00, 0x18, 0x32, 0x70 },
    { 0xbc, 0x00, 0x25, 0x3a, 0x18, 0x35, 0x73 },
    { 0xdc, 0x00, 0x2c, 0x33, 0x10, 0x36, 0x74 },
    { 0xfc, 0x00, 0x35, 0x05, 0x10, 0x38, 0x76 },
    { 0x1c, 0x01, 0x40, 0x00, 0x10, 0x3c, 0x7a },
    { 0x3c, 0x01, 0x4b, 0x35, 0x10, 0x42, 0x80 },
};

/* The sync payload, in the vendor driver's order. */
enum {
    GC4653_REG_AGAIN_0 = 0,  /* .. GC4653_REG_AGAIN_0 + 6 */
    GC4653_REG_DGAIN_H = 7,
    GC4653_REG_DGAIN_L,
    GC4653_REG_EXPO_H,
    GC4653_REG_EXPO_L,
    GC4653_REG_VMAX_H,
    GC4653_REG_VMAX_L,
};

/****************************************************************************
 * common functions                                                         *
 ****************************************************************************/
static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_gc4653_info[vi_pipe];
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

static td_void cmos_get_ae_comm_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft, const ot_isp_sns_state *sns_state)
{
    ot_unused(cis);
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->full_lines_max = GC4653_FULL_LINES_MAX_LINEAR;
    ae_sns_dft->flicker_freq = FLICKER_FREQ;
    ae_sns_dft->hmax_times = 1000000000U / (GC4653_VMAX_LINEAR * STANDARD_FPS);
    ae_sns_dft->lines_per500ms = cis->lines_per500ms ? cis->lines_per500ms :
        GC4653_VMAX_LINEAR * STANDARD_FPS / 2;
    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy = 1;
    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = DGAIN_ACCURACY;
    ae_sns_dft->isp_dgain_shift = 8;
    ae_sns_dft->min_isp_dgain_target = 256;
    ae_sns_dft->max_isp_dgain_target = 2 << 8; /* 2x, as the hi3516ev200 driver */
}


static td_void cmos_get_ae_linear_default(cis_info *cis, ot_isp_ae_sensor_default *d, const ot_isp_sns_state *st)
{
    d->min_again = d->min_again_target = GC4653_AGAIN_MIN;
    d->max_again = d->max_again_target = GC4653_AGAIN_MAX;
    d->min_dgain = d->min_dgain_target = GC4653_DGAIN_MIN;
    d->max_dgain = d->max_dgain_target = GC4653_DGAIN_MAX;
    d->ae_compensation = AE_COMENSATION_DEFAULT;
    d->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    d->init_exposure = cis->init_exposure ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR;
    d->min_int_time = GC4653_INT_TIME_MIN;
    d->full_lines = st->fl[0];
    d->max_int_time = st->fl[0] - FL_OFFSET_LINEAR;
    if (d->max_int_time > GC4653_INT_TIME_MAX) {
        d->max_int_time = GC4653_INT_TIME_MAX;
    }
    d->min_int_time_target = d->min_int_time;
    d->max_int_time_target = d->max_int_time;
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

static td_void cmos_config_vmax(ot_isp_sns_state *st, td_u32 vmax)
{
    st->regs_info[0].i2c_data[GC4653_REG_VMAX_H].data = high_8bits(vmax);
    st->regs_info[0].i2c_data[GC4653_REG_VMAX_L].data = low_8bits(vmax);
}


/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe pipe, td_float fps, ot_isp_ae_sensor_default *d)
{
    cis_info *cis;
    ot_isp_sns_state *st;
    td_u32 vmax;
    sns_check_pipe_void_return(pipe);
    sns_check_pointer_void_return(d);
    cis = cmos_get_info(pipe);
    sns_check_pointer_void_return(cis);
    st = cis->sns_state;
    sns_check_pointer_void_return(st);
    if (fps < GC4653_FPS_MIN_LINEAR || fps > GC4653_FPS_MAX_LINEAR) {
        isp_err_trace("GC4653 unsupported fps: %f\n", fps);
        return;
    }
    vmax = (td_u32)((td_float)(GC4653_VMAX_LINEAR * GC4653_FPS_MAX_LINEAR) / fps);
    if (vmax > GC4653_FULL_LINES_MAX_LINEAR) {
        vmax = GC4653_FULL_LINES_MAX_LINEAR;
    }
    cmos_config_vmax(st, vmax);
    st->fl_std = vmax;
    st->fl[0] = vmax;
    d->fps = fps;
    d->full_lines_std = vmax;
    d->full_lines = vmax;
    d->lines_per500ms = cis->lines_per500ms ? cis->lines_per500ms :
        GC4653_VMAX_LINEAR * GC4653_FPS_MAX_LINEAR / 2;
    d->max_int_time = vmax - FL_OFFSET_LINEAR;
    if (d->max_int_time > GC4653_INT_TIME_MAX) {
        d->max_int_time = GC4653_INT_TIME_MAX;
    }
    d->max_int_time_target = d->max_int_time;
    d->hmax_times = (td_u32)(1000000000.0f / (vmax * fps));
}


static td_void cmos_slow_framerate_set(ot_vi_pipe pipe, td_u32 lines, ot_isp_ae_sensor_default *d)
{
    ot_isp_sns_state *st;
    td_u32 vmax;
    sns_check_pipe_void_return(pipe);
    sns_check_pointer_void_return(d);
    st = cmos_get_state(pipe);
    sns_check_pointer_void_return(st);
    /* AE passes an exposure limit; VMAX includes the margin. */
    vmax = lines > GC4653_FULL_LINES_MAX_LINEAR - FL_OFFSET_LINEAR ?
        GC4653_FULL_LINES_MAX_LINEAR : lines + FL_OFFSET_LINEAR;
    if (vmax < GC4653_VMAX_LINEAR) {
        vmax = GC4653_VMAX_LINEAR;
    }
    st->fl[0] = vmax;
    cmos_config_vmax(st, vmax);
    d->full_lines = lines;
    d->max_int_time = vmax - FL_OFFSET_LINEAR;
    if (d->max_int_time > GC4653_INT_TIME_MAX) {
        d->max_int_time = GC4653_INT_TIME_MAX;
    }
    d->max_int_time_target = d->max_int_time;
}


static td_void cmos_inttime_update_linear(ot_isp_sns_state *st, td_u32 t)
{
    if (t > st->fl[0] - FL_OFFSET_LINEAR) {
        t = st->fl[0] - FL_OFFSET_LINEAR;
    }
    if (t < GC4653_INT_TIME_MIN) {
        t = GC4653_INT_TIME_MIN;
    }
    if (t > GC4653_INT_TIME_MAX) {
        t = GC4653_INT_TIME_MAX;
    }
    st->regs_info[0].i2c_data[GC4653_REG_EXPO_H].data = high_8bits(t);
    st->regs_info[0].i2c_data[GC4653_REG_EXPO_L].data = low_8bits(t);
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

static td_void cmos_again_calc_table(ot_vi_pipe pipe, td_u32 *lin, td_u32 *db)
{
    td_u32 i;
    ot_unused(pipe);
    sns_check_pointer_void_return(lin);
    sns_check_pointer_void_return(db);
    for (i = 1; i < GC4653_AGAIN_ROWS && *lin >= g_gc4653_again[i]; ++i) {
    }
    *lin = g_gc4653_again[i - 1];
    *db = i - 1;
}

/*
 * One combined transfer (address write, repeated start, read): two separate
 * write()/read() calls let an ISP sync write land in between and move the
 * sensor's register pointer.
 */
static td_s32 gc4653_read_reg(const cis_i2c *i2c, td_u32 addr, td_u8 *value)
{
#ifdef OT_GPIO_I2C
    /* The descriptor is /dev/gpioi2c_ex, which takes no I2C_RDWR. */
    ot_unused(i2c);
    ot_unused(addr);
    ot_unused(value);
    isp_err_trace("gc4653: register reads are not supported over GPIO-I2C\n");
    return TD_FAILURE;
#else
    td_u8 reg[GC4653_ADDR_BYTE] = { high_8bits(addr), low_8bits(addr) };
    struct i2c_msg msg[2] = {
        { .addr = i2c->addr >> 1, .flags = 0, .len = sizeof(reg), .buf = reg },
        { .addr = i2c->addr >> 1, .flags = I2C_M_RD, .len = 1, .buf = value },
    };
    struct i2c_rdwr_ioctl_data rdwr = { .msgs = msg, .nmsgs = 2 };

    if (i2c->fd < 0 || ioctl(i2c->fd, I2C_RDWR, &rdwr) != 2) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
#endif
}

static td_void cmos_gains_update(ot_vi_pipe pipe, td_u32 again, td_u32 dgain)
{
    cis_info *cis;
    ot_isp_sns_state *st;
    sns_check_pipe_void_return(pipe);
    cis = cmos_get_info(pipe);
    sns_check_pointer_void_return(cis);
    st = cis->sns_state;
    sns_check_pointer_void_return(st);
    td_u32 i;
    if (again >= GC4653_AGAIN_ROWS) {
        again = GC4653_AGAIN_ROWS - 1;
    }
    if (dgain > GC4653_DGAIN_MAX) {
        dgain = GC4653_DGAIN_MAX;
    }
    for (i = 0; i < 7; ++i) {
        st->regs_info[0].i2c_data[GC4653_REG_AGAIN_0 + i].data = g_gc4653_again_regs[again][i];
    }
    st->regs_info[0].i2c_data[GC4653_REG_DGAIN_H].data = (dgain >> 6) & 0x0f;
    st->regs_info[0].i2c_data[GC4653_REG_DGAIN_L].data = (dgain & 0x3f) << 2;
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

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(awb_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    (td_void)memset_s(awb_sns_dft, sizeof(ot_isp_awb_sensor_default), 0, sizeof(ot_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP;

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

    (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm, sizeof(ot_isp_awb_ccm));
    (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table),
                      &g_awb_agc_table, sizeof(ot_isp_awb_agc_table));

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
    ot_unused(sns_state);

    isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR;
    isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR;
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
    /*
     * GRBG, as the hi3516ev200 profile runs this sensor. The vendor's CV610
     * table moves the window start by one pixel on one axis (0x0352/0x0354),
     * which may turn that into RGGB or BGGR; unverified on hardware.
     */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[0] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[1] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[2] = 2; /* index 2, cfa_pattern 2 */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[3] = 1; /* index 3, cfa_pattern 1 */
    isp_def->sns_mode.valid_dng_raw_format = TD_TRUE;
}

/* superpix_sp2308's module set; no CAC, LSC or ACS. */
static td_void cmos_isp_get_linear_default(ot_isp_cmos_default *isp_def)
{
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc;
    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci;
    isp_def->key.bit1_dpc              = 1;
    isp_def->dpc                       = &g_cmos_dpc;
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_bayer_nr         = 1;
    isp_def->bayer_nr                  = &g_cmos_bayer_nr;
    isp_def->key.bit1_ca               = 1;
    isp_def->ca                        = &g_cmos_ca;
    isp_def->key.bit1_clut             = 1;
    isp_def->clut                      = &g_cmos_clut;
    isp_def->key.bit1_wdr              = 1;
    isp_def->wdr                       = &g_cmos_wdr;
    isp_def->key.bit1_dehaze           = 1;
    isp_def->dehaze                    = &g_cmos_dehaze;
    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
}

static td_s32 cmos_isp_get_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(isp_def);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(isp_def, sizeof(ot_isp_cmos_default), 0, sizeof(ot_isp_cmos_default));
    cmos_isp_get_linear_default(isp_def);

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    isp_def->sns_mode.sns_id = GC4653_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_isp_get_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_isp_get_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32 i;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(black_level);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;
    for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
        black_level->auto_attr.black_level[0][i] = BLACK_LEVEL_DEFAULT;
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

static td_void cmos_comm_sns_reg_info_init(cis_info *cis, ot_isp_sns_state *st)
{
    static const td_u16 reg_addr[GC4653_SNS_REGS] = {
        GC4653_AGAIN_ADDR_0, GC4653_AGAIN_ADDR_1, GC4653_AGAIN_ADDR_2, GC4653_AGAIN_ADDR_3,
        GC4653_AGAIN_ADDR_4, GC4653_AGAIN_ADDR_5, GC4653_AGAIN_ADDR_6,
        GC4653_DGAIN_ADDR_H, GC4653_DGAIN_ADDR_L, GC4653_EXPO_ADDR_H, GC4653_EXPO_ADDR_L,
        GC4653_VMAX_ADDR_H, GC4653_VMAX_ADDR_L
    };
    td_u32 i;
    st->regs_info[0].sns_type = OT_ISP_SNS_TYPE_I2C;
    st->regs_info[0].com_bus.i2c_dev = cis->bus_info.i2c_dev;
    st->regs_info[0].cfg2_valid_delay_max = 2;
    st->regs_info[0].reg_num = GC4653_SNS_REGS;
    for (i = 0; i < GC4653_SNS_REGS; ++i) {
        st->regs_info[0].i2c_data[i].update = TD_TRUE;
        st->regs_info[0].i2c_data[i].dev_addr = GC4653_I2C_ADDR;
        st->regs_info[0].i2c_data[i].addr_byte_num = GC4653_ADDR_BYTE;
        st->regs_info[0].i2c_data[i].data_byte_num = GC4653_DATA_BYTE;
        st->regs_info[0].i2c_data[i].reg_addr = reg_addr[i];
        /* as the vendor driver: gain and shutter a frame after VMAX */
        st->regs_info[0].i2c_data[i].delay_frame_num = i < GC4653_REG_VMAX_H ? 1 : 0;
    }
}


static td_void cmos_sns_reg_info_update(ot_vi_pipe pipe, ot_isp_sns_state *st)
{
    td_u32 i;
    ot_unused(pipe);
    for (i = 0; i < GC4653_SNS_REGS; ++i) {
        st->regs_info[0].i2c_data[i].update = TD_TRUE;
    }
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
        case GC4653_4M30_10BIT_LINEAR_MODE:
            sns_state->fl_std = GC4653_VMAX_LINEAR;
            break;
        default:
            sns_state->fl_std = GC4653_VMAX_LINEAR;
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

    for (i = 0; i < GC4653_MODE_MAX; i++) {
        if (sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->fps >= cis->mode_tbl[i].min_fps &&
            sns_image_mode->width <= cis->mode_tbl[i].width &&
            sns_image_mode->height <= cis->mode_tbl[i].height &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (gc4653_res_mode)i;
            break;
        }
    }

    if (i >= GC4653_MODE_MAX) {
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

    cis->i2c.addr = GC4653_I2C_ADDR;
    cis->i2c.addr_byte_num = GC4653_ADDR_BYTE;
    cis->i2c.data_byte_num = GC4653_DATA_BYTE;

    ret = cis_i2c_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    ret = gc4653_linear_4m30_10bit_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("GC4653 init failed\n");
        /* Let go of the bus, so a later attempt opens it afresh. */
        (td_void)cis_i2c_exit(cis);
        return;
    }
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
        isp_err_trace("GC4653 exit failed!\n");
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
    sns_state->img_mode  = GC4653_4M30_10BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = GC4653_VMAX_LINEAR;
    sns_state->fl[0]     = GC4653_VMAX_LINEAR;
    sns_state->fl[1]     = GC4653_VMAX_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    /* The first sync payload: half a frame of exposure, 1x gain. */
    sns_state->fl[0] = GC4653_VMAX_LINEAR;
    cmos_inttime_update_linear(sns_state, GC4653_VMAX_LINEAR / 2);
    {
        td_u32 i;
        for (i = 0; i < 7; ++i) {
            sns_state->regs_info[0].i2c_data[GC4653_REG_AGAIN_0 + i].data = g_gc4653_again_regs[0][i];
        }
    }
    sns_state->regs_info[0].i2c_data[GC4653_REG_DGAIN_H].data = (GC4653_DGAIN_MIN >> 6) & 0x0f;
    sns_state->regs_info[0].i2c_data[GC4653_REG_DGAIN_L].data = (GC4653_DGAIN_MIN & 0x3f) << 2;
    cmos_config_vmax(sns_state, GC4653_VMAX_LINEAR);

    return;
}

static td_void cmos_isp_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    ot_unused(enable);
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
    cis_info *cis = TD_NULL;
    td_u8 value;

    sns_check_pipe_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    if (gc4653_read_reg(&cis->i2c, addr, &value) != TD_SUCCESS) {
        isp_err_trace("gc4653 read register 0x%x failed!\n", addr);
        return TD_FAILURE;
    }

    return value;
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

ot_isp_sns_obj g_sns_gc4653_obj = {
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

ot_isp_sns_obj *gc4653_get_obj(td_void)
{
    return &g_sns_gc4653_obj;
}
