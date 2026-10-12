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
#include "gc2083_cmos.h"
#include "gc2083_cmos_param.h"
#include "gc2083_sensor_ctrl.h"

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
static cis_info g_gc2083_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = GC2083_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        .bus_info = { .i2c_dev = -1 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
            {
                GC2083_VMAX_LINEAR,
                GC2083_FULL_LINES_MAX_LINEAR,
                GC2083_FPS_MAX_LINEAR,
                GC2083_FPS_MIN_LINEAR,
                GC2083_WIDTH_LINEAR,
                GC2083_HEIGHT_LINEAR,
                GC2083_MODE_LINEAR,
                OT_WDR_MODE_NONE,
                "GC2083_1080P30_10BIT_LINEAR_MODE"
             },
        },
        .i2c.fd = -1,
    }
};

/* Exact VENDOR GC2083 linear gain table and its 13-register payload. */
#define GC2083_AGAIN_ROWS 29
#define GC2083_AGAIN_REGS 13
#define GC2083_SNS_REGS 21
#define GC2083_INT_TIME_MAX 0x3fff
static const td_u32 g_gc2083_again[GC2083_AGAIN_ROWS] = {
    1024, 1232, 1472, 1760, 2048, 2464, 2976, 3568, 4304, 5168,
    6096, 7312, 8704, 10448, 12192, 14624, 17248, 20688, 24656,
    29584, 34832, 41792, 50176, 60224, 75360, 90432, 106496,
    127808, 151584
};
/* Columns: 00d0, 0dc1, 00b8, 00b9, 0155, 0410..0417. */
static const td_u8 g_gc2083_again_regs[GC2083_AGAIN_ROWS][GC2083_AGAIN_REGS] = {
    {0x00,0x00,0x01,0x00,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x10,0x00,0x01,0x0c,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x01,0x00,0x01,0x1a,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x11,0x00,0x01,0x2b,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x02,0x00,0x02,0x00,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x12,0x00,0x02,0x18,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x03,0x00,0x02,0x33,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x13,0x00,0x03,0x15,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x04,0x00,0x04,0x00,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x14,0x00,0x04,0xe0,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x05,0x00,0x05,0x26,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x15,0x00,0x06,0x2b,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x44,0x00,0x08,0x00,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x54,0x00,0x09,0x22,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x45,0x00,0x0b,0x0d,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x55,0x00,0x0d,0x16,0x03,0x11,0x11,0x11,0x11,0x6f,0x6f,0x6f,0x6f},
    {0x04,0x01,0x10,0x00,0x19,0x16,0x16,0x16,0x16,0x6f,0x6f,0x6f,0x6f},
    {0x14,0x01,0x13,0x04,0x19,0x16,0x16,0x16,0x16,0x6f,0x6f,0x6f,0x6f},
    {0x24,0x01,0x16,0x1a,0x19,0x16,0x16,0x16,0x16,0x6f,0x6f,0x6f,0x6f},
    {0x34,0x01,0x1a,0x2b,0x19,0x16,0x16,0x16,0x16,0x6f,0x6f,0x6f,0x6f},
    {0x44,0x01,0x20,0x00,0x36,0x18,0x18,0x18,0x18,0x6f,0x6f,0x6f,0x6f},
    {0x54,0x01,0x26,0x07,0x36,0x18,0x18,0x18,0x18,0x6f,0x6f,0x6f,0x6f},
    {0x64,0x01,0x2c,0x33,0x36,0x18,0x18,0x18,0x18,0x6f,0x6f,0x6f,0x6f},
    {0x74,0x01,0x35,0x17,0x36,0x18,0x18,0x18,0x18,0x6f,0x6f,0x6f,0x6f},
    {0x84,0x01,0x35,0x17,0x64,0x16,0x16,0x16,0x16,0x72,0x72,0x72,0x72},
    {0x94,0x01,0x35,0x17,0x64,0x16,0x16,0x16,0x16,0x72,0x72,0x72,0x72},
    {0x85,0x01,0x35,0x17,0x64,0x16,0x16,0x16,0x16,0x72,0x72,0x72,0x72},
    {0x95,0x01,0x35,0x17,0x64,0x16,0x16,0x16,0x16,0x72,0x72,0x72,0x72},
    {0xa5,0x01,0x35,0x17,0x64,0x16,0x16,0x16,0x16,0x72,0x72,0x72,0x72}
};
static td_u32 g_gc2083_gain_update_count[OT_ISP_MAX_PIPE_NUM];

/****************************************************************************
 * common functions                                                         *
 ****************************************************************************/
static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_gc2083_info[vi_pipe];
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
    ae_sns_dft->full_lines_max = GC2083_FULL_LINES_MAX_LINEAR;
    ae_sns_dft->flicker_freq = FLICKER_FREQ;
    ae_sns_dft->hmax_times = 1000000000U / (GC2083_VMAX_LINEAR * STANDARD_FPS);
    ae_sns_dft->lines_per500ms = cis->lines_per500ms ? cis->lines_per500ms :
        GC2083_VMAX_LINEAR * STANDARD_FPS / 2;
    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy = 1;
    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = DGAIN_ACCURACY;
    ae_sns_dft->isp_dgain_shift = 8;
    ae_sns_dft->min_isp_dgain_target = 256;
    ae_sns_dft->max_isp_dgain_target = 512;
}


static td_void cmos_get_ae_linear_default(cis_info *cis, ot_isp_ae_sensor_default *d, const ot_isp_sns_state *st)
{
    d->min_again = d->min_again_target = GC2083_AGAIN_MIN;
    d->max_again = d->max_again_target = GC2083_AGAIN_MAX;
    d->min_dgain = d->max_dgain = d->min_dgain_target = d->max_dgain_target = GC2083_DGAIN_MIN;
    d->ae_compensation = 0x40;
    d->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    d->init_exposure = cis->init_exposure ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR;
    d->min_int_time = 2;
    d->full_lines = st->fl[0];
    d->max_int_time = st->fl[0] - FL_OFFSET_LINEAR;
    if (d->max_int_time > GC2083_INT_TIME_MAX) {
        d->max_int_time = GC2083_INT_TIME_MAX;
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
    st->regs_info[0].i2c_data[19].data = high_8bits(vmax);
    st->regs_info[0].i2c_data[20].data = low_8bits(vmax);
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
    if (fps < GC2083_FPS_MIN_LINEAR || fps > GC2083_FPS_MAX_LINEAR) {
        isp_err_trace("GC2083 unsupported fps: %f\n", fps);
        return;
    }
    vmax = (td_u32)((td_float)(GC2083_VMAX_LINEAR * GC2083_FPS_MAX_LINEAR) / fps);
    if (vmax > GC2083_FULL_LINES_MAX_LINEAR) {
        vmax = GC2083_FULL_LINES_MAX_LINEAR;
    }
    cmos_config_vmax(st, vmax);
    st->fl_std = vmax;
    st->fl[0] = vmax;
    d->fps = fps;
    d->full_lines_std = vmax;
    d->full_lines = vmax;
    d->lines_per500ms = cis->lines_per500ms ? cis->lines_per500ms :
        GC2083_VMAX_LINEAR * GC2083_FPS_MAX_LINEAR / 2;
    d->max_int_time = vmax - FL_OFFSET_LINEAR;
    if (d->max_int_time > GC2083_INT_TIME_MAX) {
        d->max_int_time = GC2083_INT_TIME_MAX;
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
    /* Same B051 contract as GC4023: AE passes an exposure limit; VMAX includes the margin. */
    vmax = lines > GC2083_FULL_LINES_MAX_LINEAR - FL_OFFSET_LINEAR ?
        GC2083_FULL_LINES_MAX_LINEAR : lines + FL_OFFSET_LINEAR;
    if (vmax < GC2083_VMAX_LINEAR) {
        vmax = GC2083_VMAX_LINEAR;
    }
    st->fl[0] = vmax;
    cmos_config_vmax(st, vmax);
    d->full_lines = lines;
    d->max_int_time = vmax - FL_OFFSET_LINEAR;
    if (d->max_int_time > GC2083_INT_TIME_MAX) {
        d->max_int_time = GC2083_INT_TIME_MAX;
    }
    d->max_int_time_target = d->max_int_time;
}


static td_void cmos_inttime_update_linear(ot_isp_sns_state *st, td_u32 t)
{
    if (t > GC2083_INT_TIME_MAX) {
        t = GC2083_INT_TIME_MAX;
    }
    st->regs_info[0].i2c_data[0].data = high_8bits(t);
    st->regs_info[0].i2c_data[1].data = low_8bits(t);
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
    for (i = 1; i < GC2083_AGAIN_ROWS && *lin >= g_gc2083_again[i]; ++i) {
    }
    *lin = g_gc2083_again[i - 1];
    *db = i - 1;
}

/*
 * One combined transfer (address write, repeated start, read): two separate
 * write()/read() calls, as the vendor driver does, let an ISP sync write
 * land in between and move the sensor's register pointer.
 */
static td_s32 gc2083_read_reg(const cis_i2c *i2c, td_u32 addr, td_u8 *value)
{
    td_u8 reg[GC2083_ADDR_BYTE] = { high_8bits(addr), low_8bits(addr) };
    struct i2c_msg msg[2] = {
        { .addr = i2c->addr >> 1, .flags = 0, .len = sizeof(reg), .buf = reg },
        { .addr = i2c->addr >> 1, .flags = I2C_M_RD, .len = 1, .buf = value },
    };
    struct i2c_rdwr_ioctl_data rdwr = { .msgs = msg, .nmsgs = 2 };

    if (i2c->fd < 0 || ioctl(i2c->fd, I2C_RDWR, &rdwr) != 2) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* VENDOR reads 040c and sets both trims only on every fifth gain update. */
static td_void cmos_update_gain_trim(cis_info *cis)
{
    td_u8 value;
    td_u8 trim;
    td_s32 ret_171;
    td_s32 ret_172;
    if (gc2083_read_reg(&cis->i2c, 0x040c, &value) != TD_SUCCESS) {
        isp_err_trace("GC2083 read 0x040c failed; retain previous gain trim\n");
        return;
    }
    if (value < 0x18) trim = 0x40;
    else if (value < 0x28) trim = 0x3e;
    else if (value < 0x30) trim = 0x3d;
    else if (value < 0x38) trim = 0x3c;
    else if (value < 0x40) trim = 0x3b;
    else if (value < 0x48) trim = 0x3a;
    else if (value < 0x50) trim = 0x39;
    else if (value < 0x58) trim = 0x38;
    else trim = 0x37;
    ret_171 = cis_write_reg(&cis->i2c, 0x0171, trim);
    ret_172 = cis_write_reg(&cis->i2c, 0x0172, trim);
    if (ret_171 != TD_SUCCESS || ret_172 != TD_SUCCESS) {
        isp_err_trace("GC2083 gain trim write failed\n");
    }
}

static td_void cmos_gains_update(ot_vi_pipe pipe, td_u32 again, td_u32 dgain)
{
    cis_info *cis;
    ot_isp_sns_state *st;
    td_u32 i;
    const td_u8 *row;
    sns_check_pipe_void_return(pipe);
    cis = cmos_get_info(pipe);
    sns_check_pointer_void_return(cis);
    st = cis->sns_state;
    sns_check_pointer_void_return(st);
    if (again >= GC2083_AGAIN_ROWS) {
        again = GC2083_AGAIN_ROWS - 1;
    }
    row = g_gc2083_again_regs[again];
    st->regs_info[0].i2c_data[2].data = row[0];
    st->regs_info[0].i2c_data[3].data = 0x2e;
    st->regs_info[0].i2c_data[4].data = row[1];
    st->regs_info[0].i2c_data[5].data = 0x28;
    for (i = 0; i < 11; ++i) {
        st->regs_info[0].i2c_data[i + 6].data = row[i + 2];
    }
    st->regs_info[0].i2c_data[17].data = (dgain >> 6) & 0x0f;
    st->regs_info[0].i2c_data[18].data = (dgain << 2) & 0xff;
    if (++g_gc2083_gain_update_count[pipe] == 5) {
        g_gc2083_gain_update_count[pipe] = 0;
        cmos_update_gain_trim(cis);
    }
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
    isp_def->sns_mode.dng_raw_format.cfa_pattern[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[2] = 1; /* index 2, cfa_pattern 1 */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[3] = 2; /* index 3, cfa_pattern 2 */
    isp_def->sns_mode.valid_dng_raw_format = TD_TRUE;
}

/* The module set the vendor driver enables (key 0xef7b); no CAC, LSC or ACS. */
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

    isp_def->sns_mode.sns_id = GC2083_ID;
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
    static const td_u16 reg_addr[GC2083_SNS_REGS] = {
        0x0d03, 0x0d04, 0x00d0, 0x031d, 0x0dc1, 0x031d,
        0x00b8, 0x00b9, 0x0155, 0x0410, 0x0411, 0x0412,
        0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x00b1,
        0x00b2, 0x0d41, 0x0d42
    };
    td_u32 i;
    st->regs_info[0].sns_type = OT_ISP_SNS_TYPE_I2C;
    st->regs_info[0].com_bus.i2c_dev = cis->bus_info.i2c_dev;
    st->regs_info[0].cfg2_valid_delay_max = 1;
    st->regs_info[0].reg_num = GC2083_SNS_REGS;
    for (i = 0; i < GC2083_SNS_REGS; ++i) {
        st->regs_info[0].i2c_data[i].update = TD_TRUE;
        st->regs_info[0].i2c_data[i].dev_addr = GC2083_I2C_ADDR;
        st->regs_info[0].i2c_data[i].addr_byte_num = GC2083_ADDR_BYTE;
        st->regs_info[0].i2c_data[i].data_byte_num = GC2083_DATA_BYTE;
        st->regs_info[0].i2c_data[i].reg_addr = reg_addr[i];
        st->regs_info[0].i2c_data[i].delay_frame_num =
            (i == 3 || i == 5 || i == 19 || i == 20) ? 0 : 1;
    }
}


static td_void cmos_sns_reg_info_update(ot_vi_pipe pipe, ot_isp_sns_state *st)
{
    td_u32 i;
    ot_unused(pipe);
    for (i = 0; i < GC2083_SNS_REGS; ++i) {
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
        case GC2083_1080P30_10BIT_LINEAR_MODE:
            sns_state->fl_std = GC2083_VMAX_LINEAR;
            break;
        default:
            sns_state->fl_std = GC2083_VMAX_LINEAR;
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

    for (i = 0; i < GC2083_MODE_MAX; i++) {
        if (sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->width <= cis->mode_tbl[i].width &&
            sns_image_mode->height <= cis->mode_tbl[i].height &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (gc2083_res_mode)i;
            break;
        }
    }

    if (i >= GC2083_MODE_MAX) {
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

    cis->i2c.addr = GC2083_I2C_ADDR;
    cis->i2c.addr_byte_num = GC2083_ADDR_BYTE;
    cis->i2c.data_byte_num = GC2083_DATA_BYTE;

    ret = cis_i2c_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    ret = gc2083_linear_1080p30_10bit_init(cis);
    if (ret != TD_SUCCESS) { isp_err_trace("GC2083 init failed\n"); return; }
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
        isp_err_trace("GC2083 exit failed!\n");
    }

    return;
}

static td_void cmos_isp_global_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_info *cis = TD_NULL;
    td_u32 i;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = GC2083_1080P30_10BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = GC2083_VMAX_LINEAR;
    sns_state->fl[0]     = GC2083_VMAX_LINEAR;
    sns_state->fl[1]     = GC2083_VMAX_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    /* Prime the first sync payload with the VENDOR row-0 AE state. */
    sns_state->regs_info[0].i2c_data[0].data = 0x02;
    sns_state->regs_info[0].i2c_data[1].data = 0x02;
    sns_state->regs_info[0].i2c_data[2].data = g_gc2083_again_regs[0][0];
    sns_state->regs_info[0].i2c_data[3].data = 0x2e;
    sns_state->regs_info[0].i2c_data[4].data = g_gc2083_again_regs[0][1];
    sns_state->regs_info[0].i2c_data[5].data = 0x28;
    for (i = 0; i < 11; ++i) {
        sns_state->regs_info[0].i2c_data[i + 6].data = g_gc2083_again_regs[0][i + 2];
    }
    sns_state->regs_info[0].i2c_data[17].data = 0x01;
    sns_state->regs_info[0].i2c_data[18].data = 0x00;
    cmos_config_vmax(sns_state, GC2083_VMAX_LINEAR);
    g_gc2083_gain_update_count[vi_pipe] = 0;

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
    cis_info *cis = TD_NULL;
    td_u8 value;

    sns_check_pipe_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    if (gc2083_read_reg(&cis->i2c, addr, &value) != TD_SUCCESS) {
        isp_err_trace("gc2083 read register 0x%x failed!\n", addr);
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

ot_isp_sns_obj g_sns_gc2083_obj = {
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

ot_isp_sns_obj *gc2083_get_obj(td_void)
{
    return &g_sns_gc2083_obj;
}
