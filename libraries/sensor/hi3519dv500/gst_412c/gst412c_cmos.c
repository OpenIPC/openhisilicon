/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_thermo.h"
#include "ot_mpi_awb.h"

#include "gst412c_cmos_ex.h"
#include "gst412c_cmos.h"

#define GST412C_ID                    412
#define SENSOR_GST412C_WIDTH          2688
#define SENSOR_GST412C_HEIGHT         1520


/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define gst412c_sensor_set_ctx(dev, ctx)   ((g_gst412c_sns_state[dev]) = (ctx))
#define gst412c_sensor_reset_ctx(dev)      (g_gst412c_sns_state[dev] = TD_NULL)

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

ot_isp_sns_commbus g_gst412c_bus_info[OT_ISP_MAX_PIPE_NUM] = {
    [0] = { .i2c_dev = 0 },
    [1 ... OT_ISP_MAX_PIPE_NUM - 1] = { .i2c_dev = -1 }
};

ot_isp_sns_state *g_gst412c_sns_state[OT_ISP_MAX_PIPE_NUM] = {TD_NULL};

static td_bool blc_clamp_info[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = TD_TRUE};
ot_isp_sns_commbus *gst412c_get_bus_info(ot_vi_pipe vi_pipe)
{
    return &g_gst412c_bus_info[vi_pipe];
}

ot_isp_sns_state *gst412c_get_ctx(ot_vi_pipe vi_pipe)
{
    return g_gst412c_sns_state[vi_pipe];
}

const gst412c_video_mode_tbl g_gst412c_mode_tbl[GST412C_MODE_BUTT] = {
    {GST412C_VMAX_4M_30FPS_12BIT_LINEAR, GST412C_FULL_LINES_MAX,      30,  5,
     2692, 1524, 0, OT_WDR_MODE_NONE,    "GST412C_4M_30FPS_12BIT_LINEAR_MODE"},
};

#define gst412c_err_mode_print(sns_image_mode, sns_state) \
do { \
    isp_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                  (sns_image_mode)->width, \
                  (sns_image_mode)->height, \
                  (sns_image_mode)->fps, \
                  (sns_state)->wdr_mode); \
} while (0)

static td_void cmos_get_thermo_comm_default(ot_vi_pipe vi_pipe, ot_isp_thermo_sensor_default *themro_sns_dft)
{
    themro_sns_dft->bright_compensation = 4095; /* compensation 4095 */
    themro_sns_dft->bright_throw_ratio = 50;    /* throw_ratio 50 */
    themro_sns_dft->dark_throw_ratio = 50;      /* throw_ratio 50 */
    return;
}

static td_s32 cmos_get_thermo_default(ot_vi_pipe vi_pipe, ot_isp_thermo_sensor_default *thermo_sns_dft)
{
    sns_check_pointer_return(thermo_sns_dft);
    cmos_get_thermo_comm_default(vi_pipe, thermo_sns_dft);

    return TD_SUCCESS;
}

static td_s32 cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    sns_check_pointer_return(exp_func);

    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));

    exp_func->pfn_cmos_get_thermo_default = cmos_get_thermo_default;

    return TD_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP                         5000
#define CALIBRATE_STATIC_WB_R_GAIN                    527
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    526

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              1
#define CALIBRATE_AWB_P2                              241
#define CALIBRATE_AWB_Q1                              (-15)
#define CALIBRATE_AWB_A1                              159211
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-104902)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static td_s32 cmos_get_awb_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(awb_sns_dft);
    gst412c_sensor_get_ctx(vi_pipe, sns_state);
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
        case GST412C_4M_30FPS_12BIT_LINEAR_MODE:
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
    gst412c_sensor_get_ctx(vi_pipe, sns_state);
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

    cmos_get_isp_linear_default(isp_def);

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        isp_def->wdr_switch_attr.exp_ratio[0] = 0x400;
    }

    isp_def->sns_mode.sns_id = GST412C_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_get_isp_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_get_isp_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32  i;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(black_level);
    gst412c_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
        black_level->auto_attr.black_level[0][i] = 0xBB8;
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
    return;
}

static td_s32 cmos_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    gst412c_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case OT_WDR_MODE_2To1_LINE:
            sns_state->wdr_mode = OT_WDR_MODE_2To1_LINE;
            printf("vc wdr 2t1 mode\n");
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
    sns_state->regs_info[0].com_bus.i2c_dev = g_gst412c_bus_info[vi_pipe].i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 0;

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = GST412C_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = GST412C_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = GST412C_DATA_BYTE;
    }

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

    return;
}

static td_s32 cmos_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pointer_return(sns_regs_info);
    gst412c_sensor_get_ctx(vi_pipe, sns_state);
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
    ot_unused(vi_pipe);
    switch (sns_image_mode) {
        case GST412C_4M_30FPS_12BIT_LINEAR_MODE:
            sns_state->fl_std = GST412C_VMAX_4M_30FPS_12BIT_LINEAR;
            break;
        default:
            sns_state->fl_std = GST412C_VMAX_4M_30FPS_12BIT_LINEAR;
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
    gst412c_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < GST412C_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_gst412c_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_gst412c_mode_tbl[i].width &&
            sns_image_mode->height <= g_gst412c_mode_tbl[i].height &&
            sns_state->wdr_mode == g_gst412c_mode_tbl[i].wdr_mode) {
            image_mode = (gst412c_res_mode)i;
            break;
        }
    }

    if (i >= GST412C_MODE_BUTT) {
        gst412c_err_mode_print(sns_image_mode, sns_state);
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

    gst412c_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = GST412C_4M_30FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = GST412C_VMAX_4M_30FPS_12BIT_LINEAR;
    sns_state->fl[0]     = GST412C_VMAX_4M_30FPS_12BIT_LINEAR;
    sns_state->fl[1]     = GST412C_VMAX_4M_30FPS_12BIT_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_s32 cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    sns_check_pointer_return(sensor_exp_func);

    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init         = gst412c_init;
    sensor_exp_func->pfn_cmos_sns_exit         = gst412c_exit;
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

static td_s32 gst412c_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    g_gst412c_bus_info[vi_pipe].i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 sensor_ctx_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    gst412c_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == TD_NULL) {
        sns_state_ctx = (ot_isp_sns_state *)malloc(sizeof(ot_isp_sns_state));
        if (sns_state_ctx == TD_NULL) {
            isp_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_ERR_ISP_NOMEM;
        }
    }

    (td_void)memset_s(sns_state_ctx, sizeof(ot_isp_sns_state), 0, sizeof(ot_isp_sns_state));

    gst412c_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return TD_SUCCESS;
}

static td_void sensor_ctx_exit(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state_ctx = TD_NULL;

    gst412c_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    gst412c_sensor_reset_ctx(vi_pipe);

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

    sns_attr_info.sns_id = GST412C_ID;
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
    ret = ot_mpi_thermo_sensor_reg_callback(vi_pipe, ae_lib, &sns_attr_info, &ae_register);
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

    ret = ot_mpi_isp_sensor_unreg_callback(vi_pipe, GST412C_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = ot_mpi_thermo_sensor_unreg_callback(vi_pipe, ae_lib, GST412C_ID);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to thermo lib failed!\n");
        return ret;
    }

    ret = ot_mpi_awb_sensor_unreg_callback(vi_pipe, awb_lib, GST412C_ID);
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

ot_isp_sns_obj g_sns_gst412c_obj = {
    .pfn_register_callback     = sensor_register_callback,
    .pfn_un_register_callback  = sensor_unregister_callback,
    .pfn_standby               = gst412c_standby,
    .pfn_restart               = gst412c_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = gst412c_write_register,
    .pfn_read_reg              = gst412c_read_register,
    .pfn_set_bus_info          = gst412c_set_bus_info,
    .pfn_set_init              = sensor_set_init
};
