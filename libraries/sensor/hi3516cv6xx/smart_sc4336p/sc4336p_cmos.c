/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "sc4336p_cmos.h"
#include "sc4336p_cmos_param.h"
#include "sc4336p_sensor_ctrl.h"

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
static cis_info g_sc4336p_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = SC4336P_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        .bus_info = { .i2c_dev = -1 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
            {
                SC4336P_VMAX_2M_LINEAR,
                SC4336P_FULL_LINES_MAX_2M_LINEAR,
                SC4336P_FPS_MAX_LINEAR,
                SC4336P_FPS_MIN_LINEAR,
                SC4336P_WIDTH_2M_LINEAR,
                SC4336P_HEIGHT_2M_LINEAR,
                SC4336P_MODE_LINEAR,
                OT_WDR_MODE_NONE,
                "SC4336P_2M_30FPS_10BIT_LINEAR_MODE"
            },
            {
                SC4336P_VMAX_LINEAR,
                SC4336P_FULL_LINES_MAX_LINEAR,
                SC4336P_FPS_MAX_LINEAR,
                SC4336P_FPS_MIN_LINEAR,
                SC4336P_WIDTH_LINEAR,
                SC4336P_HEIGHT_LINEAR,
                SC4336P_MODE_LINEAR,
                OT_WDR_MODE_NONE,
                "SC4336P_4M_30FPS_10BIT_LINEAR_MODE"
            },
        },
        .i2c.fd = -1,
    }
};

/****************************************************************************
  Again & Dgain table for TABLE Mode                                        *
 ****************************************************************************/
#define SC4336P_AGAIN_NODE_NUM               6
#define SC4336P_AGAIN_ADDR_INDEX_NODE_NUM    6
#define SC4336P_DGAIN_NODE_NUM               128
#define SC4336P_DGAIN_ADDR_INDEX_NODE_NUM    128

static td_u32 again_index[SC4336P_AGAIN_NODE_NUM] = {
    1024,  2048,  4096,  8192, 16384, 32768
};

static td_u32 dgain_index[SC4336P_DGAIN_NODE_NUM] = {
    1024, 1056, 1088, 1120,  1152, 1184, 1216, 1248,  1280, 1312, 1344, 1376,
    1408, 1440, 1472, 1504,  1536, 1568, 1600, 1632,  1664, 1696, 1728, 1760,
    1792, 1824, 1856, 1888,  1920, 1952, 1984, 2016,  2048, 2112, 2176, 2240,
    2304, 2368, 2432, 2496,  2560, 2624, 2688, 2752,  2816, 2880, 2944, 3008,
    3072, 3136, 3200, 3264,  3328, 3392, 3456, 3520,  3584, 3648, 3712, 3776,
    3840, 3904, 3968, 4032,  4096, 4224, 4352, 4480,  4608, 4736, 4864, 4992,
    5120, 5248, 5376, 5504,  5632, 5760, 5888, 6016,  6144, 6272, 6400, 6528,
    6656, 6784, 6912, 7040,  7168, 7296, 7424, 7552,  7680, 7808, 7936, 8064,
    8192, 8448, 8704, 8960,  9216, 9472, 9728, 9984, 10240, 10496, 10752, 11008,
    11264, 11520, 11776, 12032, 12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080,
    14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128
};


static td_u32 again_table[SC4336P_AGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0, 0x8, 0x9, 0xb, 0x0f, 0x1f
};

static td_u32 dgain_table[SC4336P_DGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0080, 0x0084, 0x0088, 0x008C, 0x0090, 0x0094, 0x0098, 0x009C, 0x00A0, 0x00A4, 0x00A8, 0x00AC,
    0x00B0, 0x00B4, 0x00B8, 0x00BC, 0x00C0, 0x00C4, 0x00C8, 0x00CC, 0x00D0, 0x00D4, 0x00D8, 0x00DC,
    0x00E0, 0x00E4, 0x00E8, 0x00EC, 0x00F0, 0x00F4, 0x00F8, 0x00FC, 0x0180, 0x0184, 0x0188, 0x018C,
    0x0190, 0x0194, 0x0198, 0x019C, 0x01A0, 0x01A4, 0x01A8, 0x01AC, 0x01B0, 0x01B4, 0x01B8, 0x01BC,
    0x01C0, 0x01C4, 0x01C8, 0x01CC, 0x01D0, 0x01D4, 0x01D8, 0x01DC, 0x01E0, 0x01E4, 0x01E8, 0x01EC,
    0x01F0, 0x01F4, 0x01F8, 0x01FC, 0x0380, 0x0384, 0x0388, 0x038C, 0x0390, 0x0394, 0x0398, 0x039C,
    0x03A0, 0x03A4, 0x03A8, 0x03AC, 0x03B0, 0x03B4, 0x03B8, 0x03BC, 0x03C0, 0x03C4, 0x03C8, 0x03CC,
    0x03D0, 0x03D4, 0x03D8, 0x03DC, 0x03E0, 0x03E4, 0x03E8, 0x03EC, 0x03F0, 0x03F4, 0x03F8, 0x03FC,
    0x0780, 0x0784, 0x0788, 0x078C, 0x0790, 0x0794, 0x0798, 0x079C, 0x07A0, 0x07A4, 0x07A8, 0x07AC,
    0x07B0, 0x07B4, 0x07B8, 0x07BC, 0x07C0, 0x07C4, 0x07C8, 0x07CC, 0x07D0, 0x07D4, 0x07D8, 0x07DC,
    0x07E0, 0x07E4, 0x07E8, 0x07EC, 0x07F0, 0x07F4, 0x07F8, 0x07FC,
};

static td_bool g_standby_cfg_init[OT_ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * common functions                                                         *
 ****************************************************************************/
static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_sc4336p_info[vi_pipe];
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
    td_float max_fps = STANDARD_FPS; /* maxfps 30 */

    lines = cis->mode_tbl[sns_state->img_mode].ver_lines;
    max_fps = cis->mode_tbl[sns_state->img_mode].max_fps;
    if (sns_state->img_mode == SC4336P_4M_30FPS_10BIT_LINEAR_MODE) {
        ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
        ae_sns_dft->int_time_accu.accuracy = INT_TIME_ACCURACY;
        ae_sns_dft->int_time_accu.offset = 0.386; /* 0.386 line */
    } else if (sns_state->img_mode == SC4336P_2M_30FPS_10BIT_LINEAR_MODE) {
        ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
        ae_sns_dft->int_time_accu.accuracy = INT_TIME_ACCURACY;
        ae_sns_dft->int_time_accu.offset = 0.386; /* 0.386 line */
    }

    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = FLICKER_FREQ; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = cis->mode_tbl[sns_state->img_mode].max_ver_lines;
    ae_sns_dft->hmax_times = (td_u32)((1000000000) / ((td_float)lines * max_fps)); /* 1000000000ns, 30fps */

    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = AGAIN_ACCURACY;

    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = DGAIN_ACCURACY;

    ae_sns_dft->isp_dgain_shift = ISP_DGAIN_SHIFT;
    ae_sns_dft->min_isp_dgain_target = ISP_DGAIN_TARGET_MIN << ae_sns_dft->isp_dgain_shift; /* min 1 */
    ae_sns_dft->max_isp_dgain_target = ISP_DGAIN_TARGET_MAX << ae_sns_dft->isp_dgain_shift; /* max 32 */
    (td_void)memcpy_s(&ae_sns_dft->piris_attr, sizeof(ot_isp_piris_attr), &g_piris, sizeof(ot_isp_piris_attr));
    if (cis->lines_per500ms == 0) {
        ae_sns_dft->lines_per500ms = (td_u32)((td_float)lines * max_fps / 2); /* 30fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = cis->lines_per500ms;
    }

    ae_sns_dft->max_iris_fno = OT_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid = TD_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = cis->quick_start_en;
    ae_sns_dft->quick_start.black_frame_num = 0;
    ae_sns_dft->ae_stat_pos = cis->ae_stat_pos; /* 1 use be stat to AE */
    return;
}

static td_void cmos_get_ae_linear_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_again = SC4336P_AGAIN_MAX; /* max 32768 */
    ae_sns_dft->min_again = SC4336P_AGAIN_MIN;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = SC4336P_DGAIN_MAX; /* max 15.75*1024 = 16128 */
    ae_sns_dft->min_dgain = SC4336P_DGAIN_MIN;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = AE_COMENSATION_DEFAULT;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure =
        (cis->init_exposure != 0) ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR; /* init 148859 */

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
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);
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
        isp_err_trace("ISP sensor sc4336p Not support Fps: %f\n", fps);
        return;
    }

    vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
    vmax = (vmax > lines_max) ? lines_max : vmax;

    cmos_config_vmax(sns_state, vmax);
    sns_state->fl_std = vmax;
    ae_sns_dft->lines_per500ms = (td_u32)(lines * max_fps / 2); /* 30/2 */
    cis->lines_per500ms = ae_sns_dft->lines_per500ms;

    ae_sns_dft->fps = fps;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR;
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

    vmax = full_lines + FL_OFFSET_LINEAR;

    vmax = clip3(vmax, lines, lines_max);
    sns_state->fl[0] = vmax;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl[0] - FL_OFFSET_LINEAR;
    }
    ae_sns_dft->full_lines = full_lines;
    return;
}

static td_void cmos_inttime_update_linear(cis_info *cis, ot_isp_sns_state *sns_state, td_u32 int_time)
{
    if (cis->quick_start_en == TD_TRUE && cis->i2c.fd >= 0) {
        cis_write_reg(&cis->i2c, SC4336P_EXPO_L_ADDR, lower_4bits(int_time));
        cis_write_reg(&cis->i2c, SC4336P_EXPO_M_ADDR, higher_8bits(int_time));
        cis_write_reg(&cis->i2c, SC4336P_EXPO_H_ADDR, higher_4bits(int_time));
    } else {
        sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = lower_4bits(int_time);
        sns_state->regs_info[0].i2c_data[EXPO_M_IDX].data = higher_8bits(int_time);
        sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = higher_4bits(int_time);
    }

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

    if (*again_lin >= again_index[SC4336P_AGAIN_ADDR_INDEX_NODE_NUM - 1]) {
        *again_lin = again_index[SC4336P_AGAIN_ADDR_INDEX_NODE_NUM - 1];
        *again_db = again_table[SC4336P_AGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < SC4336P_AGAIN_NODE_NUM; i++) {
        if (*again_lin < again_index[i]) {
            *again_lin = again_index[i - 1];
            *again_db = again_table[i - 1];
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

    if (*dgain_lin >= dgain_index[SC4336P_DGAIN_ADDR_INDEX_NODE_NUM - 1]) {
        *dgain_lin = dgain_index[SC4336P_DGAIN_ADDR_INDEX_NODE_NUM - 1];
        *dgain_db = dgain_table[SC4336P_DGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < SC4336P_DGAIN_NODE_NUM; i++) {
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
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_i2c *i2c = TD_NULL;
    td_u8 reg_0x3e09;
    td_u8 reg_0x3e07;
    td_u8 reg_0x3e06;

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    i2c = &cis->i2c;
    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    reg_0x3e06 = high_8bits(dgain);
    reg_0x3e07 = low_8bits (dgain);
    reg_0x3e09 = low_8bits (again);

    if (cis->quick_start_en == TD_TRUE && cis->i2c.fd >= 0) {
        cis_write_reg(i2c, SC4336P_DGAIN_H_ADDR, reg_0x3e06);
        cis_write_reg(i2c, SC4336P_DGAIN_L_ADDR, reg_0x3e07);
        cis_write_reg(i2c, SC4336P_AGAIN_L_ADDR, reg_0x3e09);
    } else {
        sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = reg_0x3e06;
        sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = reg_0x3e07;
        sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = reg_0x3e09;
    }

    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    ot_unused(man_ratio_enable);
    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_void_return(sns_state);

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
    fast_ae_attr->sns_delay_frame = 1 + 2; /* delay 1 + 2 frame */
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
        case SC4336P_4M_30FPS_10BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR; /* max 1023 */
            break;
        case SC4336P_2M_30FPS_10BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR; /* max 1023 */
            break;
        default:
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

    isp_def->sns_mode.sns_id = SC4336P_ID;
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
    if (cis->quick_start_en) {
        sns_state->regs_info[0].cfg2_valid_delay_max = 1; /* delay_max 1 */
    } else {
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }
    sns_state->regs_info[0].reg_num = REG_MAX_IDX;

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = SC4336P_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = SC4336P_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = SC4336P_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = SC4336P_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].reg_addr = SC4336P_EXPO_M_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = SC4336P_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = SC4336P_AGAIN_L_ADDR;

    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = SC4336P_DGAIN_H_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = SC4336P_DGAIN_L_ADDR;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = SC4336P_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = SC4336P_VMAX_H_ADDR;
    return;
}

static td_void cmos_sns_config_logic_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
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
    ret = sc4336p_get_standby_cfg(standby_cfg);
    if (ret == TD_SUCCESS) {
        g_standby_cfg_init[vi_pipe] = TD_TRUE;
    }

    return TD_SUCCESS;
}


static td_void cmos_isp_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sns_image_mode,
    ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    switch (sns_image_mode) {
        case SC4336P_4M_30FPS_10BIT_LINEAR_MODE:
            sns_state->fl_std = SC4336P_VMAX_LINEAR;
            break;
        case SC4336P_2M_30FPS_10BIT_LINEAR_MODE:
            sns_state->fl_std = SC4336P_VMAX_2M_LINEAR;
            break;
        default:
            sns_state->fl_std = SC4336P_VMAX_LINEAR;
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

    for (i = 0; i < SC4336P_MODE_MAX; i++) {
        if (sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->width <= cis->mode_tbl[i].width &&
            sns_image_mode->height <= cis->mode_tbl[i].height &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (sc4336p_res_mode)i;
            break;
        }
    }

    if (i >= SC4336P_MODE_MAX) {
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
    td_s32           ret;
    td_bool          quick_start_flag = TD_FALSE;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sensor_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sensor_state = cis->sns_state;
    sns_check_pointer_void_return(sensor_state);

    cis->i2c.addr = SC4336P_I2C_ADDR;
    cis->i2c.addr_byte_num = SC4336P_ADDR_BYTE;
    cis->i2c.data_byte_num = SC4336P_DATA_BYTE;

    ret = cis_i2c_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    quick_start_flag = cmos_isp_get_quick_start_flag(vi_pipe);
    if (quick_start_flag == TD_TRUE) {
        sensor_state->init = TD_TRUE;
        return;
    }

    /* When sensor first init, config all registers */
    sc4336p_linear_4m30_10bit_init(cis);

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
    sns_state->img_mode  = SC4336P_4M_30FPS_10BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = SC4336P_VMAX_LINEAR;
    sns_state->fl[0]     = SC4336P_VMAX_LINEAR;
    sns_state->fl[1]     = SC4336P_VMAX_LINEAR;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_void cmos_isp_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    td_u32 full_lines_5fps, lines_max;
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
    } else {
        if (sns_state->img_mode == SC4336P_4M_30FPS_10BIT_LINEAR_MODE) {
            full_lines_5fps = (SC4336P_VMAX_LINEAR * STANDARD_FPS) / 5; /* 30fps, 5fps */
        } else if (sns_state->img_mode == SC4336P_2M_30FPS_10BIT_LINEAR_MODE) {
            full_lines_5fps = (SC4336P_VMAX_2M_LINEAR * STANDARD_FPS) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    if (enable) { /* setup for ISP pixel calibration mode */
        cis_write_reg(i2c, SC4336P_AGAIN_L_ADDR, 0x00);

        cis_write_reg(i2c, SC4336P_DGAIN_H_ADDR, 0x00);
        cis_write_reg(i2c, SC4336P_DGAIN_L_ADDR, 0x80);

        cis_write_reg(i2c, SC4336P_VMAX_L_ADDR, low_8bits(full_lines_5fps));
        cis_write_reg(i2c, SC4336P_VMAX_H_ADDR, high_8bits(full_lines_5fps));

        cis_write_reg(i2c, SC4336P_EXPO_L_ADDR, lower_4bits(full_lines_5fps - FL_OFFSET_LINEAR));
        cis_write_reg(i2c, SC4336P_EXPO_M_ADDR, higher_8bits(full_lines_5fps - FL_OFFSET_LINEAR));
        cis_write_reg(i2c, SC4336P_EXPO_H_ADDR, higher_4bits(full_lines_5fps - FL_OFFSET_LINEAR));
    } else { /* setup for ISP 'normal mode' */
        lines_max = cis->mode_tbl[sns_state->img_mode].max_ver_lines;
        sns_state->fl_std = (sns_state->fl_std > lines_max) ? lines_max : sns_state->fl_std;
        cis_write_reg(i2c, SC4336P_VMAX_L_ADDR, low_8bits(sns_state->fl_std));
        cis_write_reg(i2c, SC4336P_VMAX_H_ADDR, high_8bits(sns_state->fl_std));
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
    td_s32 ret = TD_SUCCESS;
    cis_info *cis = TD_NULL;

    if (g_standby_cfg_init[vi_pipe] == TD_TRUE) {
        return;
    }

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    ret += cis_write_reg(&cis->i2c, 0x0100, 0x00);
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

    ret += cis_write_reg(&cis->i2c, 0x0100, 0x01);
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

ot_isp_sns_obj g_sns_sc4336p_obj = {
    .pfn_register_callback     = cmos_register_callback,
    .pfn_un_register_callback  = cmos_unregister_callback,
    .pfn_standby               = cmos_standby,
    .pfn_restart               = cmos_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = cmos_write_register,
    .pfn_read_reg              = cmos_read_register,
    .pfn_set_bus_info          = cmos_set_bus_info,
    .pfn_set_init              = cmos_set_init,
    .pfn_set_fast_ae           = cmos_set_fast_ae
};

ot_isp_sns_obj *sc4336p_get_obj(td_void)
{
    return &g_sns_sc4336p_obj;
}
