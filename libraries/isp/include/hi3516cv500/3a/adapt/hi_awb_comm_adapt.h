/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of hi_awb_comm_adapt.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __HI_AWB_COMM_ADAPT_H__
#define __HI_AWB_COMM_ADAPT_H__

#include "hi_type.h"
#include "hi_debug_adapt.h"
#include "hi_comm_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

typedef struct {
    hi_u16 white_level;
    hi_u16 black_level;
    hi_u16 cr_max;
    hi_u16 cr_min;
    hi_u16 cb_max;
    hi_u16 cb_min;
    hi_u16 cr_high;
    hi_u16 cr_low;
    hi_u16 cb_high;
    hi_u16 cb_low;
    hi_u16 raw_white_level;
    hi_u16 raw_black_level;
    hi_u16 raw_cr_max;
    hi_u16 raw_cr_min;
    hi_u16 raw_cb_max;
    hi_u16 raw_cb_min;
    hi_u16 raw_cr_high;
    hi_u16 raw_cr_low;
    hi_u16 raw_cb_high;
    hi_u16 raw_cb_low;

    hi_u16 wdr_mode;
    hi_u16 enable;
    hi_u16 manual_enable;
    hi_u16 zone;
    hi_u16 high_temp;
    hi_u16 low_temp;
    hi_u16 ref_temp;
    hi_u16 rgain_base;
    hi_u16 ggain_base;
    hi_u16 bgain_base;
    hi_s32 p1;
    hi_s32 p2;
    hi_s32 q;
    hi_s32 a;
    hi_s32 c;

    hi_u16 man_sat_enable;
    hi_u16 sat_target;
} hi_isp_awb_dbg_attr;

typedef struct {
    hi_u16 sum;
    hi_u16 rg;
    hi_u16 bg;
    hi_u16 count_all;
    hi_u16 count_min;
    hi_u16 count_max;
    hi_u16 raw_r_avg;
    hi_u16 raw_g_avg;
    hi_u16 raw_b_avg;
    hi_u16 tk;
    hi_u16 weight;
    hi_s16 shift;
} hi_isp_awb_zone_dbg;

typedef struct {
    hi_u32 frm_num_bgn;
    hi_u32 global_sum;
    hi_u16 global_rg_sta;
    hi_u16 global_bg_sta;
    hi_u16 global_count_all;
    hi_u16 global_count_min;
    hi_u16 global_count_max;
    hi_u16 global_r_avg;
    hi_u16 global_g_avg;
    hi_u16 global_avg;
    hi_u16 tk;
    hi_u16 r_gain;
    hi_u16 g_gain;
    hi_u16 b_gain;
    hi_u16 ccm[CCM_MATRIX_SIZE];

    hi_u32 hist_info[256];    /* 256 is size of hist info */
    hi_isp_awb_zone_dbg zone_debug[AWB_ZONE_NUM];

    hi_u32 frm_num_end;
} hi_isp_awb_dbg_status;

/* sensor's interface to awb */
typedef struct {
    hi_u16 color_temp;              /* RW;  range:[2000,10000]; format:16.0; the current color temperature */
    hi_u16 ccm[CCM_MATRIX_SIZE];    /* RW;  range: [0x0, 0xFFFF]; format:8.8;
                                       CCM matrixes for different color temperature */
} hi_isp_awb_ccm_tab;

typedef struct {
    hi_u16  ccm_tab_num;                   /* RW;  range: [0x3, 0x7]; format:16.0; the number of CCM matrixes */
    hi_isp_awb_ccm_tab ccm_tab[CCM_MATRIX_NUM];
} hi_isp_awb_ccm;

typedef struct {
    hi_bool valid;

    hi_u8   saturation[ISP_AUTO_ISO_STRENGTH_NUM];   /* RW;adjust saturation, different iso with different saturation */
} hi_isp_awb_agc_table;

typedef struct {
    hi_u16  wb_ref_temp;       /* RW;reference color temperature for WB  */
    hi_u16  gain_offset[ISP_BAYER_CHN_NUM];  /* RW; gain offset for white balance */
    hi_s32  wb_para[AWB_CURVE_PARA_NUM];      /* RW; parameter for wb curve,p1,p2,q1,a1,b1,c1 */

    hi_u16  golden_rgain;      /* rgain for the golden sample */
    hi_u16  golden_bgain;      /* bgain for the golden sample */
    hi_u16  sample_rgain;      /* rgain for the current sample */
    hi_u16  sample_bgain;      /* bgain for the current sample */
    hi_isp_awb_agc_table agc_tbl;
    hi_isp_awb_ccm ccm;
    hi_u16    init_rgain;           /* init WB gain */
    hi_u16    init_ggain;
    hi_u16    init_bgain;
    hi_u8     awb_run_interval;       /* RW;AWB run interval */
    hi_u16    init_ccm[CCM_MATRIX_SIZE];
} hi_isp_awb_sensor_default;

typedef struct {
    hi_isp_spec_awb_attr  spec_awb_attrs;
    hi_isp_spec_awb_caa_control caa_control;
} hi_isp_awb_spec_sensor_default;

typedef struct {
    hi_s32 (*pfn_cmos_get_awb_default)(hi_vi_pipe vi_pipe, hi_isp_awb_sensor_default *awb_sns_dft);
    hi_s32 (*pfn_cmos_get_awb_spec_default)(hi_vi_pipe vi_pipe, hi_isp_awb_spec_sensor_default *awb_spec_sns_dft);
} hi_isp_awb_sensor_exp_func;

typedef struct {
    hi_isp_awb_sensor_exp_func sns_exp;
} hi_isp_awb_sensor_register;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
