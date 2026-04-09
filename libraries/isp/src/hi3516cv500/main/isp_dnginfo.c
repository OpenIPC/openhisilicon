/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_dnginfo.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_dnginfo.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_sensor.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_ext_config.h"

hi_s32 isp_dng_info_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_trans_info.dng_info.phy_addr;

    isp_ctx->dng_info_ctrl.isp_dng = HI_MPI_SYS_Mmap(phy_addr, sizeof(hi_isp_dng_image_static_info));

    if (isp_ctx->dng_info_ctrl.isp_dng == HI_NULL) {
        isp_err_trace("isp[%d] mmap Dng info buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

hi_void isp_dng_info_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->dng_info_ctrl.isp_dng != HI_NULL) {
        HI_MPI_SYS_Munmap(isp_ctx->dng_info_ctrl.isp_dng, sizeof(hi_isp_dng_image_static_info));
        isp_ctx->dng_info_ctrl.isp_dng = HI_NULL;
    }
}

static hi_bool isp_dng_color_param_check(isp_dng_ccm *dng_ccm, isp_dng_ccm *pre_dng_ccm)
{
    hi_bool changed = HI_FALSE;
    hi_u8 i;

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        if (dng_ccm->low_ccm[i] != pre_dng_ccm->low_ccm[i]) {
            changed = HI_TRUE;
            return changed;
        }
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        if (dng_ccm->high_ccm[i] != pre_dng_ccm->high_ccm[i]) {
            changed = HI_TRUE;
            return changed;
        }
    }

    if (dng_ccm->high_color_temp != pre_dng_ccm->high_color_temp) {
        changed = HI_TRUE;
        return changed;
    }

    if (dng_ccm->low_color_temp != pre_dng_ccm->low_color_temp) {
        changed = HI_TRUE;
        return changed;
    }

    return changed;
}

static void isp_ccm_data_format(hi_u16 *ccm_in, hi_double *pd_ccm_out)
{
    hi_u8 i;
    hi_s16 tmp;
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        tmp = (hi_s16)ccm_in[i];
        pd_ccm_out[i] = (hi_double)tmp / 256; /* const value 256 */
    }
}

const double g_k_near_zero = 1.0E-10;

static const double g_srgb_to_xyzd50[CCM_MATRIX_SIZE] = {
    0.4361, 0.3851, 0.1431, 0.2225, 0.7169, 0.0606, 0.0139, 0.0971, 0.7141
};
static const double g_srgb_to_xyza[CCM_MATRIX_SIZE] = {
    0.4969, 0.4388, 0.1630, 0.2225, 0.7169, 0.0606, 0.0060, 0.0419, 0.3080
};

static const double g_xyzd50_to_srgb[CCM_MATRIX_SIZE] = {
    3.1340, -1.6169, -0.4907, -0.9784, 1.9159, 0.0334, 0.0720, -0.2290, 1.4049
};

static const hi_double g_ad_noise_profile[DNG_NP_SIZE] = { 2.0E-5, 4.5E-7, 2.0E-5, 4.5E-7, 2.0E-5, 4.5E-7 };

static double abs_double(double x)
{
    return (x < 0.0 ? -x : x);
}

static void invert_3by3(const double *matrix_a, double *inv_matrix)
{
    double a00 = matrix_a[0];   /* array index 0 */
    double a01 = matrix_a[1];   /* array index 1 */
    double a02 = matrix_a[2];   /* array index 2 */
    double a10 = matrix_a[3];   /* array index 3 */
    double a11 = matrix_a[4];   /* array index 4 */
    double a12 = matrix_a[5];   /* array index 5 */
    double a20 = matrix_a[6];   /* array index 6 */
    double a21 = matrix_a[7];   /* array index 7 */
    double a22 = matrix_a[8];   /* array index 8 */
    double temp[CCM_MATRIX_SIZE];
    double det;
    int i;

    temp[0] = a11 * a22 - a21 * a12;    /* array index 0 */
    temp[1] = a21 * a02 - a01 * a22;    /* array index 1 */
    temp[2] = a01 * a12 - a11 * a02;    /* array index 2 */
    temp[3] = a20 * a12 - a10 * a22;    /* array index 3 */
    temp[4] = a00 * a22 - a20 * a02;    /* array index 4 */
    temp[5] = a10 * a02 - a00 * a12;    /* array index 5 */
    temp[6] = a10 * a21 - a20 * a11;    /* array index 6 */
    temp[7] = a20 * a01 - a00 * a21;    /* array index 7 */
    temp[8] = a00 * a11 - a10 * a01;    /* array index 8 */

    det = (a00 * temp[0] +      /* array index 0 */
           a01 * temp[3] +      /* array index 3 */
           a02 * temp[6]);      /* array index 6 */

    if (abs_double(det) < g_k_near_zero) {
        return;
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        inv_matrix[i] = temp[i] / det;
    }

    return;
}

static void multi_matrix_3x3(const double *matrix_a, const double *matrix_b, double *mut_matrix)
{
    int i, j, k;

    for (i = 0; i < 3; ++i) {       /* 3: row num */
        for (j = 0; j < 3; ++j) {   /* 3: column num */
            double temp = 0;

            for (k = 0; k < 3; ++k) {   /* 3: column num */
                temp += matrix_a[i * 3 + k] * matrix_b[k * 3 + j]; /* 3: column num */
            }
            mut_matrix[i * 3 + j] = temp;   /* 3: column num */
        }
    }
}

static void isp_dng_light_source_check(hi_u16 color_tmep, hi_u8 *light_source)
{
    /*  light source, actually this means white balance setting.
     *  '0' means unknown, '1' daylight, '2' fluorescent, '3' tungsten, '10' flash, '17' standard light A,
     *  '18' standard light B, '19' standard light C, '20' D55, '21' D65, '22' D75, '255' other
     */
    if (color_tmep >= (7500 - 500)) { /* D75 7500 */
        *light_source = 22; /* '22' D75 */
    } else if ((color_tmep < (6500 + 500)) && (color_tmep >= (6500 - 500))) { /* [6500 - 500, 6500 + 500) */
        *light_source = 21; /* '21' D65 */
    } else if ((color_tmep < (5500 + 500)) && (color_tmep >= (5500 - 250))) { /* [5500 - 250, 5500 + 500) */
        *light_source = 20; /* '20' D55 */
    } else if ((color_tmep < (5000 + 250)) && (color_tmep >= (5000 - 100))) { /* [5000 - 100, 5000 + 250) */
        *light_source = 23; /* '23' D50(daylight 5000) */
    } else if ((color_tmep < (4800 + 100)) && (color_tmep >= (4800 - 550))) { /* [4800 - 550, 4800 + 100)  */
        *light_source = 18; /* '18' standard light B(4800) */
    } else if ((color_tmep < (4000 + 250)) && (color_tmep >= (4000 - 800))) { /* [4000 - 800, 4000 + 250) */
        *light_source = 2;  /* '2' fluorescent 4000 */
    } else if (color_tmep < (2800 + 400)) { /* [0, 2800 + 400) */
        *light_source = 17; /* '17' standard light A 2800 */
    }
}

static hi_void isp_dng_ext_read(hi_vi_pipe vi_pipe, hi_isp_dng_color_param *dng_color_param)
{
    dng_color_param->wb_gain1.r_gain = hi_ext_system_dng_high_wb_gain_r_read(vi_pipe);
    dng_color_param->wb_gain1.g_gain = hi_ext_system_dng_high_wb_gain_g_read(vi_pipe);
    dng_color_param->wb_gain1.b_gain = hi_ext_system_dng_high_wb_gain_b_read(vi_pipe);

    dng_color_param->wb_gain2.r_gain = hi_ext_system_dng_low_wb_gain_r_read(vi_pipe);
    dng_color_param->wb_gain2.g_gain = hi_ext_system_dng_low_wb_gain_g_read(vi_pipe);
    dng_color_param->wb_gain2.b_gain = hi_ext_system_dng_low_wb_gain_b_read(vi_pipe);
}
static hi_void isp_awb_cmm_config_get(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, awb_ccm_config *awb_ccm_cfg)
{
    hi_vi_pipe AWB_DEV = (vi_pipe ? 0 : 1);

    switch (isp_ctx->linkage.snap_type) {
        default:

        case SNAP_TYPE_NORMAL:
        case SNAP_TYPE_PRO:
            if ((isp_ctx->linkage.snap_pipe_mode == ISP_SNAP_PICTURE) && (isp_ctx->linkage.load_ccm == HI_TRUE)) {
                isp_algs_ctrl(isp_ctx->algs, AWB_DEV, AWB_CCM_CONFIG_GET, (hi_void *)awb_ccm_cfg);
            } else {
                isp_algs_ctrl(isp_ctx->algs, vi_pipe, AWB_CCM_CONFIG_GET, (hi_void *)awb_ccm_cfg);
            }
            break;
    }
}

static hi_void isp_update_dng_color_param(hi_isp_dng_image_static_info *isp_dng,
                                          hi_isp_dng_color_param *dng_color_param, hi_u16 *hi_a_wbgain,
                                          hi_u16 *hi_d50_wbgain, isp_dng_ccm *dng_ccm)
{
    hi_u8 i;
    hi_double ad_hi_a_ccm[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_hi_d50_ccm[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_hi_a_wbgain[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_hi_d50_wbgain[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_a_mult_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_inv_a_color_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_a_color_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_d50_mult_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_inv_d50_color_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_d50_color_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_a_forward_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_double ad_d50_forward_matrix[CCM_MATRIX_SIZE] = { 0 };
    hi_a_wbgain[0] = dng_color_param->wb_gain1.r_gain;   /* array index 0 */
    hi_a_wbgain[4] = dng_color_param->wb_gain1.g_gain;   /* array index 4 */
    hi_a_wbgain[8] = dng_color_param->wb_gain1.b_gain;   /* array index 8 */
    hi_d50_wbgain[0] = dng_color_param->wb_gain2.r_gain; /* array index 0 */
    hi_d50_wbgain[4] = dng_color_param->wb_gain2.g_gain; /* array index 4 */
    hi_d50_wbgain[8] = dng_color_param->wb_gain2.b_gain; /* array index 8 */
    isp_ccm_data_format(dng_ccm->low_ccm, ad_hi_a_ccm);
    isp_ccm_data_format(dng_ccm->high_ccm, ad_hi_d50_ccm);
    isp_ccm_data_format(hi_a_wbgain, ad_hi_a_wbgain);
    isp_ccm_data_format(hi_d50_wbgain, ad_hi_d50_wbgain);

    /* calculate color_matrix1 */
    multi_matrix_3x3(ad_hi_a_ccm, ad_hi_a_wbgain, ad_a_mult_matrix);
    multi_matrix_3x3(g_srgb_to_xyza, ad_a_mult_matrix, ad_inv_a_color_matrix);
    invert_3by3(ad_inv_a_color_matrix, ad_a_color_matrix);

    /* calculate color_matrix2 */
    multi_matrix_3x3(ad_hi_d50_ccm, ad_hi_d50_wbgain, ad_d50_mult_matrix);
    multi_matrix_3x3(g_srgb_to_xyzd50, ad_d50_mult_matrix, ad_inv_d50_color_matrix);
    invert_3by3(ad_inv_d50_color_matrix, ad_d50_color_matrix);

    /* calculate forward_matrix1 */
    invert_3by3(g_xyzd50_to_srgb, ad_inv_a_color_matrix);
    multi_matrix_3x3(ad_inv_a_color_matrix, ad_hi_a_ccm, ad_a_forward_matrix);

    /* calculate forward_matrix2 */
    invert_3by3(g_xyzd50_to_srgb, ad_inv_d50_color_matrix);
    multi_matrix_3x3(ad_inv_d50_color_matrix, ad_hi_d50_ccm, ad_d50_forward_matrix);

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        isp_dng->color_matrix1[i].numerator = (hi_s32)(ad_a_color_matrix[i] * 1000000); /* const value 1000000 */
        isp_dng->color_matrix1[i].denominator = 1000000;        /* const value 1000000 */

        isp_dng->color_matrix2[i].numerator = (hi_s32)(ad_d50_color_matrix[i] * 1000000);   /* const value 1000000 */
        isp_dng->color_matrix2[i].denominator = 1000000;        /* const value 1000000 */

        isp_dng->forwad_matrix1[i].numerator = (hi_s32)(ad_a_forward_matrix[i] * 1000000);  /* const value 1000000 */
        isp_dng->forwad_matrix1[i].denominator = 1000000;       /* const value 1000000 */

        isp_dng->forwad_matrix2[i].numerator = (hi_s32)(ad_d50_forward_matrix[i] * 1000000);  /* const value 1000000 */
        isp_dng->forwad_matrix2[i].denominator = 1000000;       /* const value 1000000 */

        isp_dng->camera_calibration1[i].numerator = 0;
        isp_dng->camera_calibration1[i].denominator = 1000000;  /* const value 1000000 */

        isp_dng->camera_calibration2[i].numerator = 0;
        isp_dng->camera_calibration2[i].denominator = 1000000;  /* const value 1000000 */
    }
}

static hi_void isp_dng_color_param_update(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    awb_ccm_config awb_ccm_cfg = { 0 };
    isp_dng_ccm dng_ccm = { 0 };
    hi_u16 hi_d50_wbgain[CCM_MATRIX_SIZE] = { 0 };
    hi_u16 hi_a_wbgain[CCM_MATRIX_SIZE] = { 0 };
    hi_isp_dng_color_param dng_color_param = { 0 };
    hi_bool changed;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_dng_ext_read(vi_pipe, &dng_color_param);

    /* DNG color param have not set */
    if (dng_color_param.wb_gain1.g_gain == 0) {
        return;
    }

    memset_s(hi_a_wbgain, sizeof(hi_u16) * CCM_MATRIX_SIZE, 0, sizeof(hi_u16) * CCM_MATRIX_SIZE);
    memset_s(hi_d50_wbgain, sizeof(hi_u16) * CCM_MATRIX_SIZE, 0, sizeof(hi_u16) * CCM_MATRIX_SIZE);

    /* get CCM from AWB libiray */
    isp_awb_cmm_config_get(vi_pipe, isp_ctx, &awb_ccm_cfg);

    memcpy_s(&dng_ccm.low_ccm, sizeof(hi_u16) * CCM_MATRIX_SIZE,
             &awb_ccm_cfg.low_ccm, sizeof(hi_u16) * CCM_MATRIX_SIZE);
    memcpy_s(&dng_ccm.high_ccm, sizeof(hi_u16) * CCM_MATRIX_SIZE,
             &awb_ccm_cfg.high_ccm, sizeof(hi_u16) * CCM_MATRIX_SIZE);
    dng_ccm.high_color_temp = awb_ccm_cfg.high_color_temp;
    dng_ccm.low_color_temp  = awb_ccm_cfg.low_color_temp;

    /* if CCM or WB gain changed, recaculate color parameters */
    changed = isp_dng_color_param_check(&dng_ccm, &isp_ctx->pre_dng_ccm);
    if (!changed) {
        if ((isp_ctx->pre_dng_color_param.wb_gain1.r_gain != dng_color_param.wb_gain1.r_gain) ||
            (isp_ctx->pre_dng_color_param.wb_gain1.g_gain != dng_color_param.wb_gain1.g_gain) ||
            (isp_ctx->pre_dng_color_param.wb_gain1.b_gain != dng_color_param.wb_gain1.b_gain) ||
            (isp_ctx->pre_dng_color_param.wb_gain2.r_gain != dng_color_param.wb_gain2.r_gain) ||
            (isp_ctx->pre_dng_color_param.wb_gain2.g_gain != dng_color_param.wb_gain2.g_gain) ||
            (isp_ctx->pre_dng_color_param.wb_gain2.b_gain != dng_color_param.wb_gain2.b_gain)) {
            changed = HI_TRUE;
        }
    }
    /* save last CCM and WB gain */
    memcpy_s(&isp_ctx->pre_dng_ccm, sizeof(isp_dng_ccm), &dng_ccm, sizeof(isp_dng_ccm));
    memcpy_s(&isp_ctx->pre_dng_color_param, sizeof(hi_isp_dng_color_param),
             &dng_color_param, sizeof(hi_isp_dng_color_param));

    if (changed == HI_TRUE) {
        isp_update_dng_color_param(isp_ctx->dng_info_ctrl.isp_dng, &dng_color_param, hi_a_wbgain,
                                   hi_d50_wbgain, &dng_ccm);
        /* data format */
        isp_ctx->dng_info_ctrl.isp_dng->camera_calibration1[0].numerator = 1000000; /* index0, const value 1000000 */
        isp_ctx->dng_info_ctrl.isp_dng->camera_calibration1[4].numerator = 1000000; /* index4, const value 1000000 */
        isp_ctx->dng_info_ctrl.isp_dng->camera_calibration1[8].numerator = 1000000; /* index8, const value 1000000 */

        isp_ctx->dng_info_ctrl.isp_dng->camera_calibration2[0].numerator = 1000000; /* index0, const value 1000000 */
        isp_ctx->dng_info_ctrl.isp_dng->camera_calibration2[4].numerator = 1000000; /* index4, const value 1000000 */
        isp_ctx->dng_info_ctrl.isp_dng->camera_calibration2[8].numerator = 1000000; /* index8, const value 1000000 */

        isp_dng_light_source_check(dng_ccm.low_color_temp, &isp_ctx->dng_info_ctrl.isp_dng->calibration_illuminant1);
        isp_dng_light_source_check(dng_ccm.high_color_temp, &isp_ctx->dng_info_ctrl.isp_dng->calibration_illuminant2);
    }
}

hi_s32 isp_update_dng_image_dynamic_info(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    hi_dng_image_dynamic_info dng_dyna_info;
    hi_u8 i;
    hi_s32 ret;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    if (sns_black_level->update == HI_TRUE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            dng_dyna_info.black_level[i] = sns_black_level->black_level[i];
        }
    } else {
        dng_dyna_info.black_level[BAYER_CHN_R]  = hi_ext_system_black_level_query_00_read(vi_pipe);
        dng_dyna_info.black_level[BAYER_CHN_GR] = hi_ext_system_black_level_query_01_read(vi_pipe);
        dng_dyna_info.black_level[BAYER_CHN_GB] = hi_ext_system_black_level_query_10_read(vi_pipe);
        dng_dyna_info.black_level[BAYER_CHN_B]  = hi_ext_system_black_level_query_11_read(vi_pipe);
    }

    dng_dyna_info.as_shot_neutral[0].denominator = MAX2(isp_ctx->linkage.white_balance_gain[0], 1);
    dng_dyna_info.as_shot_neutral[1].denominator = MAX2(isp_ctx->linkage.white_balance_gain[1], 1);
    dng_dyna_info.as_shot_neutral[2].denominator = MAX2(isp_ctx->linkage.white_balance_gain[3], 1); /* index 2&3 */

    for (i = 0; i < CFACOLORPLANE; i++) {
        dng_dyna_info.as_shot_neutral[i].numerator = isp_ctx->linkage.white_balance_gain[BAYER_CHN_GR];
    }

    for (i = 0; i < DNG_NP_SIZE; i++) {
        dng_dyna_info.ad_noise_profile[i] = g_ad_noise_profile[i];
    }

    isp_dng_color_param_update(vi_pipe);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_DNG_INFO_SET, &dng_dyna_info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}
