/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of yuv_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_sensor.h"
#include "yuv_cmos_ex.h"

hi_s32 isp_get_yuv_default(hi_isp_cmos_default *sns_dft)
{
    isp_check_pointer_return(sns_dft);

    sns_dft->key.bit1_ca        = 0;
    sns_dft->ca                 = &g_yuv_isp_ca;
    sns_dft->key.bit1_clut      = 0;
    sns_dft->clut               = HI_NULL;
    sns_dft->key.bit1_wdr       = 0;
    sns_dft->wdr                = HI_NULL;
    sns_dft->key.bit1_dpc       = 0;
    sns_dft->dpc                = HI_NULL;

    sns_dft->key.bit1_lsc       = 0;
    sns_dft->lsc                = HI_NULL;

    sns_dft->key.bit1_demosaic  = 0;
    sns_dft->demosaic           = HI_NULL;
    sns_dft->key.bit1_drc       = 0;
    sns_dft->drc                = HI_NULL;
    sns_dft->key.bit1_gamma     = 0;
    sns_dft->gamma              = HI_NULL;
    sns_dft->key.bit1_bayer_nr  = 0;
    sns_dft->bayer_nr           = HI_NULL;
    sns_dft->key.bit1_sharpen   = 0;
    sns_dft->sharpen            = &g_yuv_isp_yuv_sharpen;
    sns_dft->key.bit1_edge_mark = 0;
    sns_dft->edge_mark          = &g_yuv_isp_edge_mark;
    sns_dft->key.bit1_ge        = 0;
    sns_dft->ge                 = HI_NULL;
    sns_dft->key.bit1_anti_false_color = 0;
    sns_dft->anti_false_color      = HI_NULL;
    sns_dft->key.bit1_ldci         = 0;
    sns_dft->ldci                  = &g_yuv_isp_ldci;

    sns_dft->sensor_max_resolution.max_width  = 4056; /* max width 4056 */
    sns_dft->sensor_max_resolution.max_height = 3040; /* max width 3040 */
    sns_dft->sensor_mode.sensor_id = 477; /* sensor id 477 */
    sns_dft->sensor_mode.sensor_mode = WDR_MODE_NONE;

    memcpy_s(&sns_dft->dng_color_param, sizeof(hi_isp_cmos_dng_color_param),
             &g_yuv_dng_color_param, sizeof(hi_isp_cmos_dng_color_param));

    sns_dft->sensor_mode.dng_raw_format.bits_per_sample = 12; /* bit depth 12 */
    sns_dft->sensor_mode.dng_raw_format.white_level = 4095; /* white levle 4095 */

    sns_dft->sensor_mode.dng_raw_format.default_scale.default_scale_h.denominator = 1;
    sns_dft->sensor_mode.dng_raw_format.default_scale.default_scale_h.numerator   = 1;
    sns_dft->sensor_mode.dng_raw_format.default_scale.default_scale_v.denominator = 1;
    sns_dft->sensor_mode.dng_raw_format.default_scale.default_scale_v.numerator   = 1;
    sns_dft->sensor_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_rows = 2; /* dim row 2 */
    sns_dft->sensor_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_cols = 2; /* dim colum 2 */
    sns_dft->sensor_mode.dng_raw_format.blc_repeat_dim.blc_repeat_rows = 2; /* row 2 */
    sns_dft->sensor_mode.dng_raw_format.blc_repeat_dim.blc_repeat_cols = 2; /* colum 2 */
    sns_dft->sensor_mode.dng_raw_format.cfa_layout = CFALAYOUT_TYPE_RECTANGULAR;
    sns_dft->sensor_mode.dng_raw_format.cfa_plane_color[0] = 0; /* array index 0 assignment 0 */
    sns_dft->sensor_mode.dng_raw_format.cfa_plane_color[1] = 1; /* array index 1 assignment 1 */
    sns_dft->sensor_mode.dng_raw_format.cfa_plane_color[2] = 2; /* array index 2 assignment 2 */
    sns_dft->sensor_mode.dng_raw_format.cfa_pattern[0] = 0; /* array index 0 assignment 0 */
    sns_dft->sensor_mode.dng_raw_format.cfa_pattern[1] = 1; /* array index 1 assignment 1 */
    sns_dft->sensor_mode.dng_raw_format.cfa_pattern[2] = 1; /* array index 2 assignment 1 */
    sns_dft->sensor_mode.dng_raw_format.cfa_pattern[3] = 2; /* array index 3 assignment 2 */
    sns_dft->sensor_mode.valid_dng_raw_format = HI_TRUE;

    return HI_SUCCESS;
}

