/*
 * IMX664 (Sony Starvis 2, 4M) - CV610 ISP sensor driver.
 * Register map, exposure/gain model, limits, tuning and AWB all EXTRACTED from
 * the firmware's own IMX664 driver (xipc, type 284). Linear 4M30 12-bit;
 * SHR0 exposure (0x3050), shared 0.3dB gain ladder with HCG split (FDG 0x3030).
 */
#include <stdio.h>
#include <pthread.h>
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"

#include "imx664_cmos.h"
#include "imx664_cmos_param.h"
#include "imx664_sensor_ctrl.h"

static cis_info g_imx664_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = IMX664_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        .bus_info = { .i2c_dev = -1 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
            { IMX664_VMAX_4M_30FPS, IMX664_FULL_LINES_MAX, IMX664_FPS_30, IMX664_FPS_MIN_LINEAR,
              IMX664_WIDTH, IMX664_HEIGHT, 0, OT_WDR_MODE_NONE, "IMX664_4M_30FPS_12BIT_LINEAR" },
        },
        .i2c.fd = -1,
        .pre_ratio = 0x40
    }
};

/* IMX664 is single-mode (4M30); per-mode geometry/timing kept here. */

typedef struct {
    td_u32  vmax;
    td_float max_fps;
    td_u32  width;
    td_u32  height;
} imx664_mode_info;

static const imx664_mode_info g_imx664_modes[IMX664_MODE_MAX] = {
    { IMX664_VMAX_4M_30FPS, IMX664_FPS_30, IMX664_WIDTH, IMX664_HEIGHT },
};

/* Again & Dgain share one table, step 0.3dB (gain x1024). Ported from EV200. */
static td_u32 ad_gain_table[IMX664_AD_GAIN_TBL_RANGE] = {
    1024,    1059,    1097,    1135,    1175,    1217,    1259,
    1304,    1349,    1397,    1446,    1497,    1549,    1604,
    1660,    1719,    1779,    1842,    1906,    1973,    2043,
    2114,    2189,    2266,    2345,    2428,    2513,    2601,
    2693,    2788,    2886,    2987,    3092,    3201,    3313,
    3430,    3550,    3675,    3804,    3938,    4076,    4219,
    4368,    4521,    4680,    4845,    5015,    5191,    5374,
    5562,    5758,    5960,    6170,    6387,    6611,    6843,
    7084,    7333,    7591,    7857,    8133,    8419,    8715,
    9021,    9338,    9667,    10006,   10358,   10722,   11099,
    11489,   11893,   12311,   12743,   13191,   13655,   14135,
    14631,   15146,   15678,   16229,   16799,   17390,   18001,
    18633,   19288,   19966,   20668,   21394,   22146,   22924,
    23730,   24564,   25427,   26320,   27245,   28203,   29194,
    30220,   31282,   32381,   33519,   34697,   35917,   37179,
    38485,   39838,   41238,   42687,   44187,   45740,   47347,
    49011,   50734,   52517,   54362,   56273,   58250,   60297,
    62416,   64610,   66880,   69230,   71663,   74182,   76789,
    79487,   82281,   85172,   88165,   91264,   94471,   97791,
    101228,  104785,  108468,  112279,  116225,  120310,  124537,
    128914,  133444,  138134,  142988,  148013,  153215,  158599,
    164172,  169942,  175914,  182096,  188495,  195119,  201976,
    209074,  216421,  224027,  231900,  240049,  248485,  257217,
    266256,  275613,  285299,  295325,  305703,  316446,  327567,
    339078,  350994,  363329,  376097,  389314,  402995,  417157,
    431817,  446992,  462700,  478961,  495793,  513216,  531251,
    549921,  569246,  589250,  609958,  631393,  653582,  676550,
    700326,  724936,  750412,  776783,  804081,  832338,  861589,
    891867,  923209,  955652,  989236,  1024000, 1059985, 1097236,
    1135795, 1175709, 1217026, 1259795, 1304067, 1349895, 1397333,
    1446438, 1497269, 1549887, 1604353, 1660734, 1719095, 1779508,
    1842044, 1906777, 1973786, 2043149, 2114949, 2189273, 2266209,
    2345848, 2428287, 2513622, 2601956, 2693394, 2788046, 2886024,
    2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361,
    3804521, 3938220, 4076617
};

static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_imx664_info[vi_pipe];
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

/****************************************************************************
 * AE                                                                        *
 ****************************************************************************/
static td_s32 cmos_get_ae_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 fll;
    td_float max_fps;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_sns_dft);
    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);
    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    fll = g_imx664_modes[sns_state->img_mode].vmax;    /* 1651 */
    max_fps = g_imx664_modes[sns_state->img_mode].max_fps;  /* 30 */

    (td_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route), 0, sizeof(ot_isp_ae_route));

    sns_state->fl_std = fll;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 0;
    ae_sns_dft->hmax_times = (1000000000) / (fll * max_fps);

    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->int_time_accu.offset = -0.198;

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy = 1;
    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 1;

    ae_sns_dft->isp_dgain_shift = ISP_DGAIN_SHIFT;
    ae_sns_dft->min_isp_dgain_target = ISP_DGAIN_TARGET_MIN << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = ISP_DGAIN_TARGET_MAX << ae_sns_dft->isp_dgain_shift;

    if (cis->lines_per500ms == 0) {
        ae_sns_dft->lines_per500ms = (td_u32)(fll * max_fps / 2);
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
    ae_sns_dft->ae_stat_pos = cis->ae_stat_pos;

    ae_sns_dft->ae_compensation = AE_COMPENSATION_DEFAULT;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = cis->init_exposure ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR;

    ae_sns_dft->max_again = IMX664_AGAIN_MAX;
    ae_sns_dft->min_again = IMX664_AGAIN_MIN;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = IMX664_DGAIN_MAX;
    ae_sns_dft->min_dgain = IMX664_DGAIN_MIN;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->full_lines_max = IMX664_FULL_LINES_MAX;
    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
    ae_sns_dft->min_int_time = MIN_INT_TIME;
    ae_sns_dft->max_int_time_target = MAX_INT_TIME_TARGET;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return TD_SUCCESS;
}

static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines, vmax;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_sns_dft);
    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);
    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    td_float native_fps = g_imx664_modes[sns_state->img_mode].max_fps;
    if ((fps > native_fps) || (fps < IMX664_FPS_MIN_LINEAR)) {
        isp_err_trace("ISP sensor imx664 Not support Fps: %f\n", fps);
        return;
    }
    lines = (td_u32)(g_imx664_modes[sns_state->img_mode].vmax * native_fps / div_0_to_1_float(fps));
    vmax = (lines > IMX664_FULL_LINES_MAX) ? IMX664_FULL_LINES_MAX : lines;

    sns_state->fl_std = vmax;
    /* IMX664 VMAX is a 20-bit value across 0x3028/29/2A */
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
    sns_state->regs_info[0].i2c_data[VMAX_M_IDX].data = high_8bits(vmax);
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = (vmax >> 16) & 0xf;

    ae_sns_dft->fps = fps;
    ae_sns_dft->lines_per500ms = (td_u32)(sns_state->fl_std * fps / 2);
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (td_u32)((1000000000) / (sns_state->fl_std * div_0_to_1_float(fps)));
    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_sns_dft);
    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);
    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    full_lines = (full_lines > IMX664_FULL_LINES_MAX) ? IMX664_FULL_LINES_MAX : full_lines;
    sns_state->fl[0] = full_lines;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(full_lines);
    sns_state->regs_info[0].i2c_data[VMAX_M_IDX].data = high_8bits(full_lines);
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = (full_lines >> 16) & 0xf;

    ae_sns_dft->full_lines = full_lines;
    ae_sns_dft->max_int_time = sns_state->fl[0] - FL_OFFSET_LINEAR;
    return;
}

/* IMX664 exposure is inverted: SHR0 = frame_length - int_time, clamp [9, FL-1] */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 shr0;

    sns_check_pipe_void_return(vi_pipe);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_void_return(sns_state);

    /* An exposure as long as the frame or longer (AE can ask for one while the
     * frame length is changing) is the longest the frame allows, not a
     * wrapped subtraction that lands on the shortest. */
    if (int_time >= sns_state->fl[0] - SHR0_MIN) {
        shr0 = SHR0_MIN;
    } else {
        shr0 = sns_state->fl[0] - int_time;
    }
    shr0 = MIN2(shr0, IMX664_FULL_LINES_MAX);
    shr0 = clip3(shr0, SHR0_MIN, sns_state->fl[0] - SHR0_MAX_OFFSET);

    sns_state->regs_info[0].i2c_data[SHR0_L_IDX].data = low_8bits(shr0);
    sns_state->regs_info[0].i2c_data[SHR0_M_IDX].data = high_8bits(shr0);
    sns_state->regs_info[0].i2c_data[SHR0_H_IDX].data = (shr0 >> 16) & 0xf;
    return;
}

/* again/dgain are already quantised to table indexes; the IMX664 GAIN register
 * is the sum of the two 0.3dB indexes. */
static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 gain;
    td_u8 fdg = 0;

    sns_check_pipe_void_return(vi_pipe);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_void_return(sns_state);

    /* IMX664: again idx > 99 selects high conversion gain and drops 51 steps */
    if (again > IMX664_HCG_THRESHOLD) {
        fdg = 1;
        again -= IMX664_HCG_OFFSET;
    }
    gain = again + dgain;
    sns_state->regs_info[0].i2c_data[GAIN_L_IDX].data = low_8bits(gain);
    sns_state->regs_info[0].i2c_data[GAIN_H_IDX].data = high_8bits(gain);
    sns_state->regs_info[0].i2c_data[FDG_IDX].data = fdg;
    return;
}

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
    td_s32 i;
    ot_unused(vi_pipe);
    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    if (*again_lin >= ad_gain_table[IMX664_AGAIN_TBL_RANGE]) {
        *again_lin = ad_gain_table[IMX664_AGAIN_TBL_RANGE];
        *again_db = IMX664_AGAIN_TBL_RANGE;
        return;
    }
    for (i = 1; i < IMX664_AGAIN_TBL_RANGE + 1; i++) {
        if (*again_lin < ad_gain_table[i]) {
            *again_lin = ad_gain_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }
    return;
}

static td_void cmos_dgain_calc_table(ot_vi_pipe vi_pipe, td_u32 *dgain_lin, td_u32 *dgain_db)
{
    td_s32 i;
    ot_unused(vi_pipe);
    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);

    if (*dgain_lin >= ad_gain_table[IMX664_DGAIN_TBL_RANGE]) {
        *dgain_lin = ad_gain_table[IMX664_DGAIN_TBL_RANGE];
        *dgain_db = IMX664_DGAIN_TBL_RANGE;
        return;
    }
    for (i = 1; i < IMX664_DGAIN_TBL_RANGE + 1; i++) {
        if (*dgain_lin < ad_gain_table[i]) {
            *dgain_lin = ad_gain_table[i - 1];
            *dgain_db = i - 1;
            break;
        }
    }
    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_unused(vi_pipe); ot_unused(man_ratio_enable); ot_unused(ratio);
    ot_unused(int_time); ot_unused(lf_max_int_time);
    return; /* linear-only */
}

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
    exp_func->pfn_cmos_get_ae_default      = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set             = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set  = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update      = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update        = cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table    = cmos_again_calc_table;
    exp_func->pfn_cmos_dgain_calc_table    = cmos_dgain_calc_table;
    exp_func->pfn_cmos_get_inttime_max     = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set   = cmos_ae_fswdr_attr_set;
    return;
}

/****************************************************************************
 * AWB                                                                       *
 ****************************************************************************/
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
    awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP;
    awb_sns_dft->gain_offset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[2] = CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[3] = CALIBRATE_STATIC_WB_B_GAIN;
    awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

    (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm, sizeof(ot_isp_awb_ccm));
    (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table), &g_awb_agc_table,
                      sizeof(ot_isp_awb_agc_table));
    /* xipc's IMX664 awb copies only ccm + agc_table (no colour sector) */

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

/****************************************************************************
 * ISP                                                                       *
 ****************************************************************************/
static td_void cmos_isp_get_dng_default(const ot_isp_sns_state *sns_state, ot_isp_cmos_default *isp_def)
{
    ot_isp_cmos_dng_color_param dng_color_param = {{ 335, 256, 580 }, { 440, 256, 400 },
        { 2810, { 0x01AC, 0x8093, 0x8019, 0x8070, 0x01EA, 0x807A, 0x802A, 0x80F3, 0x021D }},
        { 4940, { 0x01D7, 0x8084, 0x8053, 0x8053, 0x01D9, 0x8086, 0x8010, 0x80B3, 0x01C3 }}};

    ot_unused(sns_state); /* single mode: always 12-bit */
    (td_void)memcpy_s(&isp_def->dng_color_param, sizeof(ot_isp_cmos_dng_color_param), &dng_color_param,
                      sizeof(ot_isp_cmos_dng_color_param));
    isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 12-bit */
    isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_hor.denominator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_hor.numerator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_ver.denominator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_ver.numerator = 1;
    isp_def->sns_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_row = 2;
    isp_def->sns_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_col = 2;
    isp_def->sns_mode.dng_raw_format.black_level_repeat_dim.repeat_row = 2;
    isp_def->sns_mode.dng_raw_format.black_level_repeat_dim.repeat_col = 2;
    isp_def->sns_mode.dng_raw_format.cfa_layout = OT_ISP_CFALAYOUT_TYPE_RECTANGULAR;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[2] = 2;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[2] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[3] = 2;
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
    /* xipc's IMX664 does not enable CAC */
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
    isp_def->key.bit1_clut             = 1;
    isp_def->clut                      = &g_cmos_clut;
    isp_def->key.bit1_dehaze           = 1;
    isp_def->dehaze                    = &g_cmos_dehaze;
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
    isp_def->key.bit1_dpc = 1;
    isp_def->dpc          = &g_cmos_dpc;
    isp_def->key.bit1_wdr = 1;
    isp_def->wdr          = &g_cmos_wdr;
    isp_def->key.bit1_lsc = 0;
    isp_def->lsc          = &g_cmos_lsc;
    isp_def->key.bit1_acs = 0;
    isp_def->acs          = &g_cmos_acs;

    cmos_isp_get_linear_default(isp_def);

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;
    isp_def->sns_mode.sns_id = IMX664_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_isp_get_dng_default(sns_state, isp_def);
    return TD_SUCCESS;
}

static td_s32 cmos_isp_get_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32 i;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(black_level);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));
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
    if ((mode & 0x3F) == OT_WDR_MODE_NONE) {
        sns_state->wdr_mode = OT_WDR_MODE_NONE;
    } else {
        isp_err_trace("imx664 does not support WDR mode %d!\n", mode);
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
    sns_state->regs_info[0].cfg2_valid_delay_max = 2;
    sns_state->regs_info[0].reg_num = 9; /* SHR0x3 + GAINx2 + VMAXx3 + FDG */

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = IMX664_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = IMX664_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = IMX664_DATA_BYTE;
        sns_state->regs_info[0].i2c_data[i].delay_frame_num = 0;
    }

    sns_state->regs_info[0].i2c_data[SHR0_L_IDX].reg_addr = IMX664_SHR0_ADDR;
    sns_state->regs_info[0].i2c_data[SHR0_M_IDX].reg_addr = IMX664_SHR0_ADDR + 1;
    sns_state->regs_info[0].i2c_data[SHR0_H_IDX].reg_addr = IMX664_SHR0_ADDR + 2; /* +2 */
    sns_state->regs_info[0].i2c_data[GAIN_L_IDX].reg_addr = IMX664_GAIN_ADDR;
    sns_state->regs_info[0].i2c_data[GAIN_H_IDX].reg_addr = IMX664_GAIN_ADDR + 1;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = IMX664_VMAX_ADDR_L;
    sns_state->regs_info[0].i2c_data[VMAX_M_IDX].reg_addr = IMX664_VMAX_ADDR_M;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = IMX664_VMAX_ADDR_H;
    sns_state->regs_info[0].i2c_data[FDG_IDX].reg_addr = IMX664_FDG_ADDR; /* conversion gain */

    /* The bring-up replays these slots before AE has filled them, and global
     * init left them zero: a VMAX of 0 and an SHR0 below its minimum. Seed
     * what is unset -- the mode's frame length and the shortest exposure --
     * and leave alone what AE already wrote, so a re-init keeps the exposure
     * in force. */
    if ((sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data |
         sns_state->regs_info[0].i2c_data[VMAX_M_IDX].data |
         sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data) == 0) {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(sns_state->fl[0]);
        sns_state->regs_info[0].i2c_data[VMAX_M_IDX].data = high_8bits(sns_state->fl[0]);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = (sns_state->fl[0] >> 16) & 0xf;
    }
    if ((sns_state->regs_info[0].i2c_data[SHR0_L_IDX].data |
         sns_state->regs_info[0].i2c_data[SHR0_M_IDX].data |
         sns_state->regs_info[0].i2c_data[SHR0_H_IDX].data) == 0) {
        td_u32 shr0 = sns_state->fl[0] - FL_OFFSET_LINEAR;
        sns_state->regs_info[0].i2c_data[SHR0_L_IDX].data = low_8bits(shr0);
        sns_state->regs_info[0].i2c_data[SHR0_M_IDX].data = high_8bits(shr0);
        sns_state->regs_info[0].i2c_data[SHR0_H_IDX].data = (shr0 >> 16) & 0xf;
    }
    return;
}

static td_void cmos_sns_reg_info_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    ot_unused(vi_pipe);
    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        if (sns_state->regs_info[0].i2c_data[i].data == sns_state->regs_info[1].i2c_data[i].data) {
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
    (td_void)memcpy_s(sns_regs_info, sizeof(ot_isp_sns_regs_info), &sns_state->regs_info[0],
                      sizeof(ot_isp_sns_regs_info));
    (td_void)memcpy_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), &sns_state->regs_info[0],
                      sizeof(ot_isp_sns_regs_info));
    sns_state->fl[1] = sns_state->fl[0];
    return TD_SUCCESS;
}

static td_s32 cmos_isp_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(sns_image_mode);
    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);
    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    td_u32 i;
    td_u8 image_mode = sns_state->img_mode;

    for (i = 0; i < IMX664_MODE_MAX; i++) {
        if (sns_image_mode->fps <= g_imx664_modes[i].max_fps &&
            sns_image_mode->width <= g_imx664_modes[i].width &&
            sns_image_mode->height <= g_imx664_modes[i].height &&
            sns_state->wdr_mode == OT_WDR_MODE_NONE) {
            image_mode = (td_u8)i;
            break;
        }
    }
    if (i >= IMX664_MODE_MAX) {
        cmos_err_mode_print(sns_image_mode, sns_state);
        return TD_FAILURE;
    }

    sns_state->fl_std = g_imx664_modes[image_mode].vmax;
    if ((sns_state->init == TD_TRUE) && (image_mode == sns_state->img_mode)) {
        return OT_ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];
    return TD_SUCCESS;
}

static void cmos_isp_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sensor_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);
    sensor_state = cis->sns_state;
    sns_check_pointer_void_return(sensor_state);

    cis->i2c.addr = IMX664_I2C_ADDR;
    cis->i2c.addr_byte_num = IMX664_ADDR_BYTE;
    cis->i2c.data_byte_num = IMX664_DATA_BYTE;

    /* Under the sensor lock: a live orientation callback checks and uses the
     * descriptor under it. */
    (td_void)pthread_mutex_lock(&cis->mutex);
    ret = cis_i2c_init(cis);
    (td_void)pthread_mutex_unlock(&cis->mutex);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }
    ret = imx664_linear_4m30_12bit_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("imx664: bring-up of image mode %u failed!\n", sensor_state->img_mode);
        return;
    }
    sensor_state->init = TD_TRUE;
    return;
}

static void cmos_isp_exit(ot_vi_pipe vi_pipe)
{
    cis_info *cis = TD_NULL;
    sns_check_pipe_void_return(vi_pipe);
    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);
    (td_void)pthread_mutex_lock(&cis->mutex);
    (td_void)cis_i2c_exit(cis);
    (td_void)pthread_mutex_unlock(&cis->mutex);
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
    sns_state->img_mode  = IMX664_4M_30FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = IMX664_VMAX_4M_30FPS;
    sns_state->fl[0]     = IMX664_VMAX_4M_30FPS;
    sns_state->fl[1]     = IMX664_VMAX_4M_30FPS;
    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    return;
}

static td_void cmos_isp_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    ot_unused(vi_pipe); ot_unused(enable);
    return;
}

static td_void cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));
    sensor_exp_func->pfn_cmos_sns_init            = cmos_isp_init;
    sensor_exp_func->pfn_cmos_sns_exit            = cmos_isp_exit;
    sensor_exp_func->pfn_cmos_sns_global_init     = cmos_isp_global_init;
    sensor_exp_func->pfn_cmos_set_image_mode      = cmos_isp_set_image_mode;
    sensor_exp_func->pfn_cmos_set_wdr_mode        = cmos_isp_set_wdr_mode;
    sensor_exp_func->pfn_cmos_get_isp_default     = cmos_isp_get_default;
    sensor_exp_func->pfn_cmos_get_isp_black_level = cmos_isp_get_black_level;
    sensor_exp_func->pfn_cmos_get_blc_clamp_info  = cmos_isp_get_blc_clamp_info;
    sensor_exp_func->pfn_cmos_set_pixel_detect    = cmos_isp_set_pixel_detect;
    sensor_exp_func->pfn_cmos_get_sns_reg_info    = cmos_isp_get_sns_regs_info;
}

/****************************************************************************
 * registration                                                              *
 ****************************************************************************/
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
        isp_err_trace("cis_unregister_callback failed!\n");
        return ret;
    }
    imx664_clear_orientation(vi_pipe);
    return TD_SUCCESS;
}

static td_void cmos_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip)
{
    cis_info *cis = cmos_get_info(vi_pipe);
    td_bool mirror = TD_FALSE;
    td_bool flip = TD_FALSE;

    if (cis == TD_NULL) {
        return;
    }
    switch (sns_mirror_flip) {
        case ISP_SNS_MIRROR:      mirror = TD_TRUE;  break;
        case ISP_SNS_FLIP:        flip = TD_TRUE;    break;
        case ISP_SNS_MIRROR_FLIP: mirror = TD_TRUE;  flip = TD_TRUE; break;
        case ISP_SNS_NORMAL:
        default:                  break;
    }
    if (imx664_set_orientation(cis, mirror, flip) != TD_SUCCESS) {
        isp_err_trace("imx664: failed to program the readout orientation\n");
    }
}

static void cmos_standby(ot_vi_pipe vi_pipe) { ot_unused(vi_pipe); return; }
static void cmos_restart(ot_vi_pipe vi_pipe) { ot_unused(vi_pipe); return; }

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
    ot_unused(vi_pipe); ot_unused(addr);
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

ot_isp_sns_obj g_sns_imx664_obj = {
    .pfn_register_callback     = cmos_register_callback,
    .pfn_un_register_callback  = cmos_unregister_callback,
    .pfn_standby               = cmos_standby,
    .pfn_restart               = cmos_restart,
    .pfn_mirror_flip           = cmos_mirror_flip,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = cmos_write_register,
    .pfn_read_reg              = cmos_read_register,
    .pfn_set_bus_info          = cmos_set_bus_info,
    .pfn_set_init              = cmos_set_init
};

ot_isp_sns_obj *imx664_get_obj(td_void)
{
    return &g_sns_imx664_obj;
}
