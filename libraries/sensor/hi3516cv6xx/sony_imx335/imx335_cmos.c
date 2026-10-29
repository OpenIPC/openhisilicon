/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "imx335_cmos.h"
#include "imx335_cmos_param.h"
#include "imx335_sensor_ctrl.h"

/****************************************************************************
 * global variables                                                         *
 ****************************************************************************/
static cis_info g_imx335_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = IMX335_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        .bus_info = { .i2c_dev = 0 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
            /* 0: 2592x1944 12-bit linear */
            { IMX335_VMAX_5M_30FPS_12BIT_LINEAR, IMX335_FULL_LINES_MAX,
              IMX335_FPS_MAX_LINEAR, IMX335_FPS_MIN_LINEAR,
              IMX335_WIDTH_LINEAR, IMX335_HEIGHT_LINEAR, IMX335_5M_30FPS_12BIT_LINEAR_MODE,
              OT_WDR_MODE_NONE, "IMX335_5M_30FPS_12BIT_LINEAR_MODE" },
            /* 1: 1296x972 2x2 binning 10-bit */
            { IMX335_VMAX_BINNING, IMX335_FULL_LINES_MAX,
              IMX335_FPS_MAX_BINNING, IMX335_FPS_MIN_BINNING,
              IMX335_WIDTH_BINNING, IMX335_HEIGHT_BINNING, IMX335_60FPS_BINNING_MODE,
              OT_WDR_MODE_NONE, "IMX335_60FPS_BINNING_MODE" },
            /* 2: 1920x1080 10-bit crop */
            { IMX335_VMAX_1080P_CROP_10BIT, IMX335_FULL_LINES_MAX,
              IMX335_FPS_MAX_CROP, IMX335_FPS_MIN_CROP,
              IMX335_WIDTH_CROP, IMX335_HEIGHT_CROP, IMX335_60FPS_CROPPED_1080P_MODE,
              OT_WDR_MODE_NONE, "IMX335_60FPS_CROPPED_1080P_MODE" },
            /* 3: 2592x1944 10-bit full */
            { IMX335_VMAX_FULL_10BIT, IMX335_FULL_LINES_MAX,
              IMX335_FPS_MAX_FULL, IMX335_FPS_MIN_FULL,
              IMX335_WIDTH_FULL, IMX335_HEIGHT_FULL, IMX335_60FPS_FULL_1944P_MODE,
              OT_WDR_MODE_NONE, "IMX335_60FPS_FULL_1944P_MODE" },
            /* 4: 2592x1520 10-bit linear crop */
            { IMX335_VMAX_CROPPED_1520P, IMX335_FULL_LINES_MAX,
              IMX335_FPS_MAX_1520P, IMX335_FPS_MIN_1520P,
              IMX335_WIDTH_1520P, IMX335_HEIGHT_1520P, IMX335_1520P_10BIT_MODE,
              OT_WDR_MODE_NONE, "IMX335_1520P_10BIT_MODE" },
            /* 5: flexible window crop, arbitrary W×H (never auto-selected) */
            { IMX335_VMAX_5M_30FPS_12BIT_LINEAR, IMX335_FULL_LINES_MAX,
              IMX335_FPS_MAX_FLEX, IMX335_FPS_MIN_FLEX,
              IMX335_WIDTH, IMX335_HEIGHT, IMX335_CROP_FLEX_LINEAR_MODE,
              OT_WDR_MODE_NONE, "IMX335_CROP_FLEX_LINEAR_MODE" },
        },
        .i2c.fd = -1,
    }
};

/* Again and Dgain share one table, step 0.3 dB (from OpenIPC ev200 imx335). */
static td_u32 ad_gain_table[IMX335_AD_GAIN_TBL_RANGE] = {
    1024,    1059,    1097,    1135,    1175,    1217,    1259,
    1304,    1349,    1397,    1446,    1497,    1549,    1604, /* 3.9dB */
    1660,    1719,    1779,    1842,    1906,    1973,    2043,
    2114,    2189,    2266,    2345,    2428,    2513,    2601, /* 8.1dB */
    2693,    2788,    2886,    2987,    3092,    3201,    3313,
    3430,    3550,    3675,    3804,    3938,    4076,    4219, /* 12.3dB */
    4368,    4521,    4680,    4845,    5015,    5191,    5374,
    5562,    5758,    5960,    6170,    6387,    6611,    6843, /* 16.5dB */
    7084,    7333,    7591,    7857,    8133,    8419,    8715,
    9021,    9338,    9667,    10006,   10358,   10722,   11099, /* 20.7dB */
    11489,   11893,   12311,   12743,   13191,   13655,   14135,
    14631,   15146,   15678,   16229,   16799,   17390,   18001, /* 24.9dB */
    18633,   19288,   19966,   20668,   21394,   22146,   22924,
    23730,   24564,   25427,   26320,   27245,   28203,   29194, /* 29.1dB */
    30220,   31282,   32381,   33519,   34697,   35917,   37179,
    38485,   39838,   41238,   42687,   44187,   45740,   47347, /* 33.3dB */
    49011,   50734,   52517,   54362,   56273,   58250,   60297,
    62416,   64610,   66880,   69230,   71663,   74182,   76789, /* 37.5dB */
    79487,   82281,   85172,   88165,   91264,   94471,   97791,
    101228,  104785,  108468,  112279,  116225,  120310,  124537, /* 41.7dB */
    128914,  133444,  138134,  142988,  148013,  153215,  158599,
    164172,  169942,  175914,  182096,  188495,  195119,  201976, /* 45.9dB */
    209074,  216421,  224027,  231900,  240049,  248485,  257217,
    266256,  275613,  285299,  295325,  305703,  316446,  327567, /* 50.1dB */
    339078,  350994,  363329,  376097,  389314,  402995,  417157,
    431817,  446992,  462700,  478961,  495793,  513216,  531251, /* 54.3dB */
    549921,  569246,  589250,  609958,  631393,  653582,  676550,
    700326,  724936,  750412,  776783,  804081,  832338,  861589, /* 58.5dB */
    891867,  923209,  955652,  989236,  1024000, 1059985, 1097236,
    1135795, 1175709, 1217026, 1259795, 1304067, 1349895, 1397333, /* 62.7dB */
    1446438, 1497269, 1549887, 1604353, 1660734, 1719095, 1779508,
    1842044, 1906777, 1973786, 2043149, 2114949, 2189273, 2266209, /* 66.9dB */
    2345848, 2428287, 2513622, 2601956, 2693394, 2788046, 2886024,
    2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361, /* 71.1dB */
    3804521, 3938220, 4076617 /* 72.0dB */
};

static td_bool g_standby_cfg_init[OT_ISP_MAX_PIPE_NUM] = {0};

/* Runtime AE timing, kept separately from cis->lines_per500ms (which holds the
 * user's init_attr value and must not be overwritten by fps changes). Reset on
 * every image-mode switch. g_imx335_crop_hmax caches the HMAX that cmos_fps_set
 * programmed for a fixed crop mode so imx335_init() can replay it if the write
 * happened before the I2C bus was open. */
static td_u32 g_imx335_lines_per500ms[OT_ISP_MAX_PIPE_NUM] = {0};
static td_u32 g_imx335_crop_hmax[OT_ISP_MAX_PIPE_NUM] = {0};

static td_bool imx335_is_crop_mode(td_u8 img_mode);
static td_u32 imx335_crop_vmax(cis_info *cis, td_u8 img_mode);

/* Per-pipe crop window for IMX335_CROP_FLEX_LINEAR_MODE. Written by
 * cmos_isp_set_image_mode() from Isp_W/Isp_H and read by
 * imx335_flex_reg_init() through imx335_get_crop(). */
static td_u32 g_imx335_crop_w[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = IMX335_CROP_W_DEFAULT
};
static td_u32 g_imx335_crop_h[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = IMX335_CROP_H_DEFAULT
};

td_void imx335_set_crop(ot_vi_pipe vi_pipe, td_u32 width, td_u32 height)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx335_crop_w[vi_pipe] = (width != 0) ? width : IMX335_CROP_W_DEFAULT;
    g_imx335_crop_h[vi_pipe] = (height != 0) ? height : IMX335_CROP_H_DEFAULT;
}

td_void imx335_get_crop(ot_vi_pipe vi_pipe, td_u32 *width, td_u32 *height)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        if (width != TD_NULL) {
            *width = IMX335_CROP_W_DEFAULT;
        }
        if (height != TD_NULL) {
            *height = IMX335_CROP_H_DEFAULT;
        }
        return;
    }
    if (width != TD_NULL) {
        *width = g_imx335_crop_w[vi_pipe];
    }
    if (height != TD_NULL) {
        *height = g_imx335_crop_h[vi_pipe];
    }
}

static td_float g_imx335_flex_fps[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = 30.0f
};

td_void imx335_set_flex_fps(ot_vi_pipe vi_pipe, td_float fps)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx335_flex_fps[vi_pipe] = (fps >= IMX335_FPS_MIN_FLEX) ? fps : IMX335_FPS_MIN_FLEX;
}

td_float imx335_get_flex_fps(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return 30.0f;
    }
    return g_imx335_flex_fps[vi_pipe];
}

static td_void imx335_set_crop_hmax(ot_vi_pipe vi_pipe, td_u32 hmax)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx335_crop_hmax[vi_pipe] = hmax;
}

static td_u32 imx335_get_crop_hmax(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return 0;
    }
    return g_imx335_crop_hmax[vi_pipe];
}

static td_void imx335_reset_runtime_timing(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx335_crop_hmax[vi_pipe] = 0;
    g_imx335_lines_per500ms[vi_pipe] = 0;
}


/****************************************************************************
 * common functions                                                         *
 ****************************************************************************/
static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_imx335_info[vi_pipe];
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
    td_u32 lines;
    td_float max_fps;

    lines = cis->mode_tbl[sns_state->img_mode].ver_lines;
    if (imx335_is_crop_mode(sns_state->img_mode)) {
        /* Flex's table entry is the 5M frame length, not the crop's. */
        lines = imx335_crop_vmax(cis, sns_state->img_mode);
    }
    max_fps = cis->mode_tbl[sns_state->img_mode].max_fps;

    ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = INT_TIME_ACCURACY;
    ae_sns_dft->int_time_accu.offset = -0.198; /* -0.198 line */

    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = FLICKER_FREQ;
    ae_sns_dft->full_lines_max = cis->mode_tbl[sns_state->img_mode].max_ver_lines;
    ae_sns_dft->hmax_times = (td_u32)((1000000000) / ((td_float)lines * max_fps)); /* 1000000000ns */

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = AGAIN_ACCURACY;

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = DGAIN_ACCURACY;

    ae_sns_dft->isp_dgain_shift = ISP_DGAIN_SHIFT;
    ae_sns_dft->min_isp_dgain_target = ISP_DGAIN_TARGET_MIN << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = ISP_DGAIN_TARGET_MAX << ae_sns_dft->isp_dgain_shift;
    (td_void)memcpy_s(&ae_sns_dft->piris_attr, sizeof(ot_isp_piris_attr), &g_piris, sizeof(ot_isp_piris_attr));
    if (cis->lines_per500ms != 0) {
        /* User override from init_attr; not touched by runtime fps changes. */
        ae_sns_dft->lines_per500ms = cis->lines_per500ms;
    } else if (g_imx335_lines_per500ms[cis->pipe] != 0) {
        ae_sns_dft->lines_per500ms = g_imx335_lines_per500ms[cis->pipe];
    } else {
        ae_sns_dft->lines_per500ms = (td_u32)((td_float)lines * max_fps / 2); /* div 2 */
    }
    ae_sns_dft->max_iris_fno = OT_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid = TD_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = cis->quick_start_en;
    ae_sns_dft->quick_start.black_frame_num = 0;
    ae_sns_dft->ae_stat_pos = cis->ae_stat_pos;
    return;
}

static td_void cmos_get_ae_linear_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_again = IMX335_AGAIN_MAX;
    ae_sns_dft->min_again = IMX335_AGAIN_MIN;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = IMX335_DGAIN_MAX;
    ae_sns_dft->min_dgain = IMX335_DGAIN_MIN;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = AE_COMENSATION_DEFAULT;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure =
        (cis->init_exposure != 0) ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR;

    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
    ae_sns_dft->min_int_time = 2; /* min int 2 */
    ae_sns_dft->max_int_time_target = MAX_INT_TIME_TARGET;
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
        case OT_WDR_MODE_NONE:
        default:
            cmos_get_ae_linear_default(cis, ae_sns_dft, sns_state);
            break;
    }

    return TD_SUCCESS;
}

static td_void cmos_config_vmax(ot_isp_sns_state *sns_state, td_u32 vmax)
{
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[IMX335_VMAX_L_IDX].data = imx335_low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[IMX335_VMAX_M_IDX].data = imx335_mid_8bits(vmax);
        sns_state->regs_info[0].i2c_data[IMX335_VMAX_H_IDX].data = imx335_high_4bits(vmax);
    }
    return;
}

/* Crop modes (window crop / flex) must keep VMAX at the window's own frame
 * length: the T6 VI drops frames once it is scaled away from it. The rate is
 * carried by HMAX instead, so it is these modes that program HMAX at runtime. */
static td_bool imx335_is_crop_mode(td_u8 img_mode)
{
    return ((img_mode == IMX335_60FPS_CROPPED_1080P_MODE) ||
            (img_mode == IMX335_1520P_10BIT_MODE) ||
            (img_mode == IMX335_CROP_FLEX_LINEAR_MODE)) ? TD_TRUE : TD_FALSE;
}

/* Table HMAX of a fixed crop mode at its max_fps; 0 for the flex mode, whose
 * HMAX follows from the crop's VMAX. */
static td_u32 imx335_crop_hmax_base(td_u8 img_mode)
{
    switch (img_mode) {
        case IMX335_60FPS_CROPPED_1080P_MODE:
            return IMX335_HMAX_1080P_CROP;
        case IMX335_1520P_10BIT_MODE:
            return IMX335_HMAX_1520P;
        default:
            return 0;
    }
}

/* Sensor line rate (lines/s) implied by a crop mode's fixed VMAX: the table
 * HMAX times max_fps for fixed crops, FLEX_CLK / VMAX for flex. */
static td_float imx335_crop_line_rate(const cis_info *cis, td_u8 img_mode, td_u32 vmax)
{
    if (img_mode == IMX335_CROP_FLEX_LINEAR_MODE) {
        return (td_float)IMX335_FLEX_CLK / (td_float)vmax;
    }
    return (td_float)imx335_crop_hmax_base(img_mode) * cis->mode_tbl[img_mode].max_fps;
}

/* Frame length of a crop mode. For flex it follows the crop height; for the
 * fixed crops mode_tbl[].ver_lines already holds the window's frame length. */
static td_u32 imx335_crop_vmax(cis_info *cis, td_u8 img_mode)
{
    if (img_mode == IMX335_CROP_FLEX_LINEAR_MODE) {
        td_u32 crop_h;
        imx335_get_crop(cis->pipe, TD_NULL, &crop_h);
        return IMX335_FLEX_VMAX(crop_h);
    }
    return cis->mode_tbl[img_mode].ver_lines;
}

static td_void imx335_config_hmax(cis_info *cis, td_u32 hmax)
{
    (td_void)cis_write_reg(&cis->i2c, IMX335_HMAX_ADDR_L, imx335_low_8bits(hmax));
    (td_void)cis_write_reg(&cis->i2c, IMX335_HMAX_ADDR_H, (hmax >> 8) & 0x1F);
}

/* Replay the HMAX cached by cmos_fps_set() after the mode table has been
 * written, for the fixed crop modes whose rate is not fixed by the table. */
td_void imx335_apply_crop_hmax(cis_info *cis)
{
    td_u32 hmax;

    if (cis == TD_NULL) {
        return;
    }
    hmax = imx335_get_crop_hmax(cis->pipe);
    if (hmax == 0) {
        return;
    }
    imx335_config_hmax(cis, hmax);
}

/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines;
    td_u32 lines_max;
    td_u32 vmax;
    td_u32 hmax;
    td_u32 crop_h;
    td_float max_fps;
    td_float min_fps;
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
        isp_err_trace("ISP sensor imx335 Not support Fps: %f\n", fps);
        return;
    }

    if (imx335_is_crop_mode(sns_state->img_mode)) {
        /* VMAX stays at the crop's frame length; HMAX = line_rate / fps. */
        if (sns_state->img_mode == IMX335_CROP_FLEX_LINEAR_MODE) {
            imx335_get_crop(vi_pipe, TD_NULL, &crop_h);
            vmax = IMX335_FLEX_VMAX(crop_h);
            min_fps = IMX335_FLEX_MIN_FPS(vmax);
            if (fps < min_fps) {
                isp_err_trace("imx335 flex: fps %f below achievable min %f for crop_h %u\n",
                    fps, min_fps, crop_h);
                return;
            }
        } else {
            vmax = imx335_crop_vmax(cis, sns_state->img_mode);
        }
        vmax = (vmax > lines_max) ? lines_max : vmax;

        hmax = (td_u32)(imx335_crop_line_rate(cis, sns_state->img_mode, vmax) / fps + 0.5f);
        if (hmax > IMX335_FLEX_HMAX_MAX) {
            isp_err_trace("imx335 crop: fps %f needs HMAX %u > %u\n",
                fps, hmax, IMX335_FLEX_HMAX_MAX);
            return;
        }
        if (hmax < IMX335_FLEX_HMAX_MIN) {
            hmax = IMX335_FLEX_HMAX_MIN;
        }
        imx335_set_crop_hmax(vi_pipe, hmax);
        if (cis->i2c.fd >= 0) {
            imx335_config_hmax(cis, hmax);
        }
        printf("IMX335 CROP: mode=%u VMAX=%u HMAX=%u fps=%.2f\n",
            sns_state->img_mode, vmax, hmax, fps);

        cmos_config_vmax(sns_state, vmax);
        sns_state->fl_std = vmax;
        ae_sns_dft->lines_per500ms = (td_u32)((td_float)vmax * fps / 2); /* div 2 */
    } else {
        vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
        vmax = (vmax > lines_max) ? lines_max : vmax;

        cmos_config_vmax(sns_state, vmax);
        sns_state->fl_std = vmax;
        ae_sns_dft->lines_per500ms = (td_u32)(lines * max_fps / 2); /* div 2 */
    }
    g_imx335_lines_per500ms[vi_pipe] = ae_sns_dft->lines_per500ms;

    ae_sns_dft->fps = fps;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
    }

    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (td_u32)((1000000000) / ((td_float)sns_state->fl_std * fps)); /* 1000000000ns */

    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 vmax;
    td_u32 lines, lines_max;
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

    if (imx335_is_crop_mode(sns_state->img_mode)) {
        /* A crop-mode VMAX cannot be lengthened without the T6 VI dropping
         * frames, so the requested slow-shutter frame is not programmable:
         * keep the window's frame length and report the limits from it, so
         * AE does not compute exposure against a length the sensor is not
         * using. */
        vmax = imx335_crop_vmax(cis, sns_state->img_mode);
        vmax = (vmax > lines_max) ? lines_max : vmax;
        sns_state->fl[0] = vmax;
        cmos_config_vmax(sns_state, vmax);
        if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
            ae_sns_dft->max_int_time = sns_state->fl[0] - FL_OFFSET_LINEAR;
        }
        ae_sns_dft->full_lines = sns_state->fl[0];
        return;
    }

    vmax = full_lines + FL_OFFSET_LINEAR;
    vmax = clip3(vmax, lines, lines_max);
    sns_state->fl[0] = vmax;

    cmos_config_vmax(sns_state, vmax);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl[0] - FL_OFFSET_LINEAR;
    }
    ae_sns_dft->full_lines = full_lines;
    return;
}

static td_void cmos_inttime_update_linear(cis_info *cis, ot_isp_sns_state *sns_state, td_u32 int_time)
{
    td_u32 value;

    value = sns_state->fl[0] - int_time;
    value = (value > IMX335_FULL_LINES_MAX) ? IMX335_FULL_LINES_MAX : value;
    value = (value < 9) ? 9 : value;
    value = (value > (sns_state->fl[0] - 1)) ? (sns_state->fl[0] - 1) : value;

    if (cis->quick_start_en == TD_TRUE && cis->i2c.fd >= 0) {
        cis_write_reg(&cis->i2c, IMX335_SHR0_LOW, imx335_low_8bits(value));
        cis_write_reg(&cis->i2c, IMX335_SHR0_MIDDLE, imx335_mid_8bits(value));
        cis_write_reg(&cis->i2c, IMX335_SHR0_HIGH, imx335_high_4bits(value));
    } else {
        sns_state->regs_info[0].i2c_data[IMX335_SHR0_L_IDX].data = imx335_low_8bits(value);
        sns_state->regs_info[0].i2c_data[IMX335_SHR0_M_IDX].data = imx335_mid_8bits(value);
        sns_state->regs_info[0].i2c_data[IMX335_SHR0_H_IDX].data = imx335_high_4bits(value);
    }
    return;
}

static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        cmos_inttime_update_linear(cis, sns_state, int_time);
    }
    return;
}

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
    int i;
    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);
    ot_unused(vi_pipe);

    if (*again_lin >= ad_gain_table[IMX335_AGAIN_TBL_RANGE]) {
        *again_lin = ad_gain_table[IMX335_AGAIN_TBL_RANGE];
        *again_db = IMX335_AGAIN_TBL_RANGE;
        return;
    }

    for (i = 1; i < (IMX335_AGAIN_TBL_RANGE + 1); i++) {
        if (*again_lin < ad_gain_table[i]) {
            *again_lin = ad_gain_table[i - 1];
            *again_db = (td_u32)(i - 1);
            break;
        }
    }
    return;
}

static td_void cmos_dgain_calc_table(ot_vi_pipe vi_pipe, td_u32 *dgain_lin, td_u32 *dgain_db)
{
    int i;
    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);
    ot_unused(vi_pipe);

    if (*dgain_lin >= ad_gain_table[IMX335_DGAIN_TBL_RANGE]) {
        *dgain_lin = ad_gain_table[IMX335_DGAIN_TBL_RANGE];
        *dgain_db = IMX335_DGAIN_TBL_RANGE;
        return;
    }

    for (i = 1; i < (IMX335_DGAIN_TBL_RANGE + 1); i++) {
        if (*dgain_lin < ad_gain_table[i]) {
            *dgain_lin = ad_gain_table[i - 1];
            *dgain_db = (td_u32)(i - 1);
            break;
        }
    }
    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;
    td_u32 gain;

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    /* IMX335 has a single combined long gain register; again/dgain are the
       table indices handed over by the calc_table callbacks. */
    gain = again + dgain;

    if (cis->quick_start_en == TD_TRUE && cis->i2c.fd >= 0) {
        cis_write_reg(&cis->i2c, IMX335_GAIN_LONG_LOW, imx335_low_8bits(gain));
        cis_write_reg(&cis->i2c, IMX335_GAIN_LONG_HIGH, imx335_mid_8bits(gain));
    } else {
        sns_state->regs_info[0].i2c_data[IMX335_GAIN_L_IDX].data = imx335_low_8bits(gain);
        sns_state->regs_info[0].i2c_data[IMX335_GAIN_H_IDX].data = imx335_mid_8bits(gain);
    }
    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_unused(man_ratio_enable);
    ot_unused(ratio);
    ot_unused(int_time);
    ot_unused(lf_max_int_time);
    sns_check_pipe_void_return(vi_pipe);
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

static td_void cmos_ae_quick_start_status_set(ot_vi_pipe vi_pipe, td_bool quick_start_en)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    cis->quick_start_en = quick_start_en;
    sns_state->sync_init = TD_FALSE;
}

static td_void cmos_fast_ae_attr_get(ot_vi_pipe vi_pipe, ot_isp_ae_fast_ae_attr *fast_ae_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(fast_ae_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);
    fast_ae_attr->enable = cis->fast_ae_en;
    fast_ae_attr->sns_delay_frame = 1 + 2;
}

static td_void cmos_fast_ae_attr_set(ot_vi_pipe vi_pipe, const ot_isp_ae_fast_ae_attr *fast_ae_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(fast_ae_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);
    cis->fast_ae_en = fast_ae_attr->enable;
}

static td_void cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));
    exp_func->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set           = cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update    = cmos_inttime_update;
    exp_func->pfn_cmos_gains_update      = cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    exp_func->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
    exp_func->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;
    exp_func->pfn_cmos_ae_quick_start_status_set = cmos_ae_quick_start_status_set;
    exp_func->pfn_cmos_ae_fast_ae_attr_get = cmos_fast_ae_attr_get;
    exp_func->pfn_cmos_ae_fast_ae_attr_set = cmos_fast_ae_attr_set;
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
    ot_isp_cmos_dng_color_param dng_color_param = {{ 378, 256, 430 }, { 439, 256, 439 },
        { 2810, { 0x01AC, 0x8093, 0x8019, 0x8070, 0x01EA, 0x807A, 0x802A, 0x80F3, 0x021D }},
        { 4940, { 0x01D7, 0x8084, 0x8053, 0x8053, 0x01D9, 0x8086, 0x8010, 0x80B3, 0x01C3 }}};

    (td_void)memcpy_s(&isp_def->dng_color_param, sizeof(ot_isp_cmos_dng_color_param), &dng_color_param,
                      sizeof(ot_isp_cmos_dng_color_param));

    switch (sns_state->img_mode) {
        case IMX335_5M_30FPS_12BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR;
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR;
            break;
        case IMX335_60FPS_BINNING_MODE:
        case IMX335_60FPS_CROPPED_1080P_MODE:
        case IMX335_60FPS_FULL_1944P_MODE:
        case IMX335_1520P_10BIT_MODE:
        case IMX335_CROP_FLEX_LINEAR_MODE:
        default:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_CROP;
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_CROP;
            break;
    }
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
    isp_def->key.bit1_cac              = 1;
    isp_def->cac                       = &g_cmos_cac;
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
    isp_def->key.bit1_dpc     = 1;
    isp_def->dpc              = &g_cmos_dpc;

    isp_def->key.bit1_wdr     = 1;
    isp_def->wdr              = &g_cmos_wdr;

    isp_def->key.bit1_lsc      = 1;   /* vendor IMX335 LSC table (matches xrscam byte-for-byte) */
    isp_def->lsc               = &g_cmos_lsc;

    isp_def->key.bit1_acs      = 0;
    isp_def->acs               = &g_cmos_acs;

#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma = 0;
    isp_def->pregamma          = &g_cmos_pregamma;
#endif
    cmos_isp_get_linear_default(isp_def);

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    isp_def->sns_mode.sns_id = IMX335_ID;
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

    black_level->auto_attr.update = TD_FALSE;

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
            black_level->auto_attr.black_level[0][i] = BLACK_LEVEL_DEFAULT;
        }
    } else {
        for (i = 0; i < OT_ISP_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][1] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][2] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][3] = BLACK_LEVEL_DEFAULT;
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
    if (cis->quick_start_en) {
        sns_state->regs_info[0].cfg2_valid_delay_max = 1;
    } else {
        sns_state->regs_info[0].cfg2_valid_delay_max = 2;
    }
    sns_state->regs_info[0].reg_num = IMX335_REG_MAX_IDX;

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = IMX335_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = IMX335_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = IMX335_DATA_BYTE;
        sns_state->regs_info[0].i2c_data[i].delay_frame_num = 0;
    }

    /* SHR0 */
    sns_state->regs_info[0].i2c_data[IMX335_SHR0_L_IDX].reg_addr = IMX335_SHR0_LOW;
    sns_state->regs_info[0].i2c_data[IMX335_SHR0_M_IDX].reg_addr = IMX335_SHR0_MIDDLE;
    sns_state->regs_info[0].i2c_data[IMX335_SHR0_H_IDX].reg_addr = IMX335_SHR0_HIGH;
    /* Long gain */
    sns_state->regs_info[0].i2c_data[IMX335_GAIN_L_IDX].reg_addr = IMX335_GAIN_LONG_LOW;
    sns_state->regs_info[0].i2c_data[IMX335_GAIN_H_IDX].reg_addr = IMX335_GAIN_LONG_HIGH;
    /* VMAX */
    sns_state->regs_info[0].i2c_data[IMX335_VMAX_L_IDX].reg_addr = IMX335_VMAX_ADDR_L;
    sns_state->regs_info[0].i2c_data[IMX335_VMAX_M_IDX].reg_addr = IMX335_VMAX_ADDR_M;
    sns_state->regs_info[0].i2c_data[IMX335_VMAX_H_IDX].reg_addr = IMX335_VMAX_ADDR_H;

    return;
}

static td_void cmos_sns_config_logic_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    ot_unused(sns_state);
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

    cmos_sns_config_logic_update(vi_pipe, sns_state);
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

static td_s32 cmos_isp_get_standby_cfg(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *standby_cfg)
{

    td_s32 ret;
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(standby_cfg);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    standby_cfg->sns_type = OT_ISP_SNS_TYPE_I2C;
    standby_cfg->com_bus.i2c_dev = cis->bus_info.i2c_dev;
    ret = imx335_get_standby_cfg(standby_cfg);
    if (ret == TD_SUCCESS) {
        g_standby_cfg_init[vi_pipe] = TD_TRUE;
    }

    return TD_SUCCESS;
}

static td_void cmos_isp_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sns_image_mode,
    ot_isp_sns_state *sns_state)
{
    switch (sns_image_mode) {
        case IMX335_60FPS_BINNING_MODE:
            sns_state->fl_std = IMX335_VMAX_BINNING;
            break;
        case IMX335_60FPS_CROPPED_1080P_MODE:
            sns_state->fl_std = IMX335_VMAX_1080P_CROP_10BIT;
            break;
        case IMX335_1520P_10BIT_MODE:
            sns_state->fl_std = IMX335_VMAX_CROPPED_1520P;
            break;
        case IMX335_CROP_FLEX_LINEAR_MODE: {
            cis_info *cis = cmos_get_info(vi_pipe);
            sns_state->fl_std = (cis != TD_NULL) ?
                imx335_crop_vmax(cis, IMX335_CROP_FLEX_LINEAR_MODE) :
                IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
            break;
        }
        case IMX335_60FPS_FULL_1944P_MODE:
        case IMX335_5M_30FPS_12BIT_LINEAR_MODE:
        default:
            sns_state->fl_std = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
            break;
    }
    return;
}

static td_s32 cmos_isp_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{

    td_u32 i;
    td_u8 image_mode;
    td_bool force_switch = TD_FALSE;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(sns_image_mode);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    image_mode = IMX335_MODE_MAX;

    /* Flexible window crop has no fixed resolution, so it is selected only by
     * the explicit Isp_SnsMode hint (Isp_SnsMode==5); record the requested
     * W×H for imx335_flex_reg_init(). */
    if (sns_image_mode->sns_mode == IMX335_CROP_FLEX_LINEAR_MODE &&
        sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        td_u32 crop_w = sns_image_mode->width;
        td_u32 crop_h = sns_image_mode->height;
        td_u32 old_w = 0;
        td_u32 old_h = 0;
        td_float old_fps = 0.0f;

        /* imx335_flex_reg_init() snaps W to a multiple of 24 and H to a
         * multiple of 4. Refuse a window that would be changed, rather than
         * program a crop the capture pipeline does not know about. */
        if (crop_w == 0 || crop_h == 0 || (crop_w % 24) != 0 || (crop_h % 4) != 0 ||
            crop_w < 312 || crop_w > 2592 || crop_h < 372 || crop_h > 1944) {
            isp_err_trace("imx335 flex: unsupported crop %ux%u "
                "(W%%24==0, H%%4==0, 312..2592 x 372..1944)\n", crop_w, crop_h);
            return TD_FAILURE;
        }
        if (sns_image_mode->fps < cis->mode_tbl[IMX335_CROP_FLEX_LINEAR_MODE].min_fps ||
            sns_image_mode->fps > cis->mode_tbl[IMX335_CROP_FLEX_LINEAR_MODE].max_fps) {
            isp_err_trace("imx335 flex: fps %f outside [%f, %f]\n",
                sns_image_mode->fps,
                cis->mode_tbl[IMX335_CROP_FLEX_LINEAR_MODE].min_fps,
                cis->mode_tbl[IMX335_CROP_FLEX_LINEAR_MODE].max_fps);
            return TD_FAILURE;
        }
        {
            td_u32 vmax = IMX335_FLEX_VMAX(crop_h);
            td_float min_fps = IMX335_FLEX_MIN_FPS(vmax);

            /* A too-slow rate cannot be expressed by the 13-bit HMAX at this
             * crop's VMAX; refuse instead of silently running faster. */
            if (sns_image_mode->fps < min_fps) {
                isp_err_trace("imx335 flex: fps %f below achievable min %f for %ux%u\n",
                    sns_image_mode->fps, min_fps, crop_w, crop_h);
                return TD_FAILURE;
            }
        }
        imx335_get_crop(vi_pipe, &old_w, &old_h);
        old_fps = imx335_get_flex_fps(vi_pipe);
        imx335_set_crop(vi_pipe, crop_w, crop_h);
        imx335_set_flex_fps(vi_pipe, sns_image_mode->fps);
        /* A different window or rate must re-run imx335_flex_reg_init(); the
         * image-mode enum alone does not change, so flag a forced switch. */
        if (old_w != crop_w || old_h != crop_h || old_fps != sns_image_mode->fps) {
            force_switch = TD_TRUE;
        }
        image_mode = IMX335_CROP_FLEX_LINEAR_MODE;
    }

    /* 1) Explicit Isp_SnsMode hint from the sensor INI (0 = auto). Flex is
     * validated above and must not be re-selected here. */
    if (image_mode == IMX335_MODE_MAX &&
        sns_image_mode->sns_mode > 0 && sns_image_mode->sns_mode < IMX335_MODE_MAX &&
        sns_image_mode->sns_mode != IMX335_CROP_FLEX_LINEAR_MODE) {
        td_u8 hint = sns_image_mode->sns_mode;
        if (sns_state->wdr_mode == cis->mode_tbl[hint].wdr_mode &&
            sns_image_mode->fps <= cis->mode_tbl[hint].max_fps &&
            sns_image_mode->fps >= cis->mode_tbl[hint].min_fps) {
            image_mode = hint;
        }
    }
    /* 2) Exact resolution match (flex is hint-only). */
    for (i = 0; image_mode == IMX335_MODE_MAX && i < IMX335_MODE_MAX; i++) {
        if (i == IMX335_CROP_FLEX_LINEAR_MODE) {
            continue;
        }
        if (sns_image_mode->width == cis->mode_tbl[i].width &&
            sns_image_mode->height == cis->mode_tbl[i].height &&
            sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->fps >= cis->mode_tbl[i].min_fps &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (td_u8)i;
        }
    }
    /* 3) Widest mode that can still serve the requested size (flex excluded). */
    for (i = 0; image_mode == IMX335_MODE_MAX && i < IMX335_MODE_MAX; i++) {
        if (i == IMX335_CROP_FLEX_LINEAR_MODE) {
            continue;
        }
        if (sns_image_mode->width <= cis->mode_tbl[i].width &&
            sns_image_mode->height <= cis->mode_tbl[i].height &&
            sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->fps >= cis->mode_tbl[i].min_fps &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (td_u8)i;
        }
    }

    if (image_mode >= IMX335_MODE_MAX) {
        cmos_err_mode_print(sns_image_mode, sns_state);
        return TD_FAILURE;
    }

    cmos_isp_config_image_mode_param(vi_pipe, image_mode, sns_state);

    if ((sns_state->init == TD_TRUE) && (image_mode == sns_state->img_mode) &&
        (force_switch != TD_TRUE)) {
        return OT_ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    /* New mode (or new flex window): drop the previous mode's runtime timing
     * so AE defaults are computed for the mode actually programmed. */
    imx335_reset_runtime_timing(vi_pipe);
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];

    return TD_SUCCESS;
}

static td_bool cmos_isp_get_quick_start_flag(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    ot_isp_ctrl_param isp_ctrl_param = {};

    ret = ot_mpi_isp_get_ctrl_param(vi_pipe, &isp_ctrl_param);
    if (ret != TD_SUCCESS) {
        isp_err_trace("pipe[%d] call ot_mpi_isp_get_ctrl_param failed! ret = 0x%x, quick_start_flag force set to 0!\n",
            vi_pipe, ret);
        return TD_FALSE;
    }

    return isp_ctrl_param.quick_start_en;
}

static void cmos_isp_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    td_bool quick_start_flag = TD_FALSE;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sensor_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sensor_state = cis->sns_state;
    sns_check_pointer_void_return(sensor_state);

    cis->i2c.addr = IMX335_I2C_ADDR;
    cis->i2c.addr_byte_num = IMX335_ADDR_BYTE;
    cis->i2c.data_byte_num = IMX335_DATA_BYTE;


    ret = cis_i2c_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    quick_start_flag = cmos_isp_get_quick_start_flag(vi_pipe);
    if (quick_start_flag == TD_TRUE) {
        imx335_mark_programmed(cis);
        sensor_state->init = TD_TRUE;
        return;
    }

    ret = imx335_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("imx335_init failed! ret:0x%x\n", ret);
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
        isp_err_trace("IMX335 exit failed!\n");
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
    sns_state->img_mode  = IMX335_5M_30FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
    sns_state->fl[0]     = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
    sns_state->fl[1]     = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    return;
}

static td_void cmos_isp_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{

    td_u32 full_lines_5fps;
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_info *cis = TD_NULL;
    cis_i2c *i2c = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    i2c = &cis->i2c;
    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        return;
    } else if (sns_state->img_mode < IMX335_MODE_MAX &&
               cis->mode_tbl[sns_state->img_mode].wdr_mode == OT_WDR_MODE_NONE) {
        td_u32 lines = imx335_is_crop_mode(sns_state->img_mode) ?
            imx335_crop_vmax(cis, sns_state->img_mode) :
            cis->mode_tbl[sns_state->img_mode].ver_lines;
        full_lines_5fps = (td_u32)(lines *
                                   cis->mode_tbl[sns_state->img_mode].max_fps / 5);
    } else {
        return;
    }

    if (enable) {
        cis_write_reg(i2c, IMX335_GAIN_LONG_LOW, 0x00);
        cis_write_reg(i2c, IMX335_GAIN_LONG_HIGH, 0x00);

        cis_write_reg(i2c, IMX335_VMAX_ADDR_L, imx335_low_8bits(full_lines_5fps));
        cis_write_reg(i2c, IMX335_VMAX_ADDR_M, imx335_mid_8bits(full_lines_5fps));
        cis_write_reg(i2c, IMX335_VMAX_ADDR_H, imx335_high_4bits(full_lines_5fps));

        cis_write_reg(i2c, IMX335_SHR0_LOW, imx335_low_8bits(full_lines_5fps - FL_OFFSET_LINEAR));
        cis_write_reg(i2c, IMX335_SHR0_MIDDLE, imx335_mid_8bits(full_lines_5fps - FL_OFFSET_LINEAR));
        cis_write_reg(i2c, IMX335_SHR0_HIGH, imx335_high_4bits(full_lines_5fps - FL_OFFSET_LINEAR));
    } else {
        sns_state->fl_std = (sns_state->fl_std > cis->mode_tbl[sns_state->img_mode].max_ver_lines) ?
            cis->mode_tbl[sns_state->img_mode].max_ver_lines : sns_state->fl_std;
        cis_write_reg(i2c, IMX335_VMAX_ADDR_L, imx335_low_8bits(sns_state->fl_std));
        cis_write_reg(i2c, IMX335_VMAX_ADDR_M, imx335_mid_8bits(sns_state->fl_std));
        cis_write_reg(i2c, IMX335_VMAX_ADDR_H, imx335_high_4bits(sns_state->fl_std));
        sns_state->sync_init = TD_FALSE;
    }
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
    sensor_exp_func->pfn_cmos_get_standby_cfg       = cmos_isp_get_standby_cfg;
    return;
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
        isp_err_trace("cis_unregister_callback failed!\n");
        return ret;
    }

    /* Sensor session ends here: a new session starts from the sensor's own
     * upright readout unless its caller asks otherwise. */
    imx335_clear_orientation(vi_pipe);

    return TD_SUCCESS;
}

static void cmos_standby(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    if (g_standby_cfg_init[vi_pipe] == TD_TRUE) {
        return;
    }

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    ret += cis_write_reg(&cis->i2c, 0x3000, 0x01);
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

static void cmos_restart(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    ret += cis_write_reg(&cis->i2c, 0x3000, 0x00);
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
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

static td_s32 cmos_set_fast_ae(ot_vi_pipe vi_pipe, ot_isp_sns_fast_ae_attr *fast_ae_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(fast_ae_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis->fast_ae_en = fast_ae_attr->enable;
    return TD_SUCCESS;
}

/* Orientation: 0x304E bit 0 mirrors horizontally, 0x304F bit 0 reverses the
 * readout (vertical flip).  The vertical half needs more than that bit: the
 * readout window and the V-flip coefficient registers move with it, which is
 * why writing 0x304F alone was measured to deliver a flat grey frame
 * (1920x1080 JPEG, every pixel 232, std 0, VI reporting no error).
 * imx335_apply_orientation() in imx335_sensor_ctrl.c writes the mainline
 * proportional set (drivers/media/i2c/imx335.c, V4L2_CID_VFLIP) for the full
 * and binning modes, and clears the bit for crop/flex windows, which keep the
 * upright readout.
 *
 * The request is stored as asked and re-applied at every bring-up, so a mode
 * switch that cannot carry it (crop/flex) does not throw it away: switching
 * back to a full or binning mode restores the orientation the operator chose.
 * The warning for an unsupported mode is emitted by imx335_apply_orientation()
 * on either path, and that call takes the sensor mutex and programs the mode
 * the sensor is actually running, so the sequence cannot interleave with a
 * bring-up on the ISP thread, nor act on a mode selected but not yet brought up. */
static td_void imx335_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip)
{
    cis_info *cis = TD_NULL;
    td_bool mirror = TD_FALSE;
    td_bool flip = TD_FALSE;

    sns_check_pipe_void_return(vi_pipe);

    switch (sns_mirror_flip) {
        case ISP_SNS_MIRROR:      mirror = TD_TRUE;  flip = TD_FALSE; break;
        case ISP_SNS_FLIP:        mirror = TD_FALSE; flip = TD_TRUE;  break;
        case ISP_SNS_MIRROR_FLIP: mirror = TD_TRUE;  flip = TD_TRUE;  break;
        case ISP_SNS_NORMAL:
        default:                  mirror = TD_FALSE; flip = TD_FALSE; break;
    }

    cis = cmos_get_info(vi_pipe);
    if (cis == TD_NULL) {
        return;
    }

    /* Storing the request, the fd check and the image-mode read all happen
     * inside imx335_apply_orientation() under the sensor lock, so nothing here
     * samples state a concurrent bring-up could change underneath it. */
    if (imx335_apply_orientation(cis, mirror, flip) != TD_SUCCESS) {
        isp_err_trace("IMX335: failed to program the readout orientation\n");
    }
}

ot_isp_sns_obj g_sns_imx335_obj = {
    .pfn_register_callback     = cmos_register_callback,
    .pfn_un_register_callback  = cmos_unregister_callback,
    .pfn_standby               = cmos_standby,
    .pfn_restart               = cmos_restart,
    .pfn_mirror_flip           = imx335_mirror_flip,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = cmos_write_register,
    .pfn_read_reg              = cmos_read_register,
    .pfn_set_bus_info          = cmos_set_bus_info,
    .pfn_set_init              = cmos_set_init,
    .pfn_set_fast_ae           = cmos_set_fast_ae
};

/* Legacy symbol name that majestic / ev200-style sensor configs look up. */
extern ot_isp_sns_obj stSnsImx335Obj __attribute__((alias("g_sns_imx335_obj")));

ot_isp_sns_obj *imx335_get_obj(td_void)
{
    return &g_sns_imx335_obj;
}
