/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_csc.c
 * Author: ISP SW
 * Create: 2012/06/28
 */
#include "isp_csc.h"
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"

#define ISP_CSC_DFT_LUMA 50
#define ISP_CSC_DFT_CON  50
#define ISP_CSC_DFT_HUE  50
#define ISP_CSC_DFT_SAT  50
#define ISP_CSC_SIN_TABLE_NUM 61

/* BT.709 -> RGB coefficient matrix */
static hi_isp_csc_matrx g_csc_attr_601 = {
    /* csc input DC component (IDC) */
    {0, 0, 0},
    /* csc output DC component (ODC) */
    {0, 128, 128},
    /* csc multiplier coefficient */
    {299, 587, 114, -172, -339, 511, 511, -428, -83},
};

/* BT.709 -> RGB coefficient matrix */
static hi_isp_csc_matrx g_csc_attr_601_limited = {
    /* csc input DC component (IDC) */
    {0, 0, 0},
    /* csc output DC component (ODC) */
    {16, 128, 128},
    /* csc multiplier coefficient */
    {258, 504, 98, -148, -291, 439, 439, -368, -71},
};

/* RGB -> YUV BT.709 coefficient matrix */
static hi_isp_csc_matrx g_csc_attr_709 = {
    /* csc input DC component (IDC) */
    {0, 0, 0},
    /* csc output DC component (ODC) */
    {0, 128, 128},
    /* csc multiplier coefficient */
    {213, 715, 72, -117, -394, 511, 511, -464, -47},
};

/* BT.709 -> RGB coefficient matrix */
static hi_isp_csc_matrx g_csc_attr_709_limited = {
    /* csc input DC component (IDC) */
    {0, 0, 0},
    /* csc output DC component (ODC) */
    {16, 128, 128},
    /* csc multiplier coefficient */
    {184, 614, 62, -101, -338, 439, 439, -399, -40},
};

static const int g_csc_sin_table[ISP_CSC_SIN_TABLE_NUM] = {
    -500,  -485,  -469,  -454,  -438,  -422,  -407,  -391,  -374,  -358,
    -342,  -325,  -309,  -292,  -276,  -259,  -242,  -225,  -208,  -191,
    -174,  -156,  -139,  -122,  -104,   -87,   -70,   -52,   -35,   -17,
    0,    17,    35,    52,    70,    87,   104,   122,   139,   156,
    174,   191,   208,   225,   242,   259,   276,   292,   309,   325,
    342,   358,   374,   391,   407,   422,   438,   454,   469,   485,
    500
};

static const int g_csc_cos_table[ISP_CSC_SIN_TABLE_NUM] = {
    866,   875,   883,   891,   899,   906,   914,   921,   927,   934,
    940,   946,   951,   956,   961,   966,   970,   974,   978,   982,
    985,   988,   990,   993,   995,   996,   998,   999,   999,  1000,
    1000,  1000,  999,   999,   998,   996,   995,   993,   990,   988,
    985,   982,   978,   974,   970,   966,   961,   956,   951,   946,
    940,   934,   927,   921,   914,   906,   899,   891,   883,   875,
    866
};

static isp_csc g_csc_ctx[ISP_MAX_PIPE_NUM];

#define csc_get_ctx(dev, ctx)            ctx = &g_csc_ctx[dev]

/* CSC reg write interface */
static hi_void csc_dyna_regs_config(hi_isp_csc_matrx *csc_attr, isp_csc_dyna_cfg *dyna_reg_cfg)
{
    hi_u8 i;
    hi_u16 norm_coeff1 = 1024;
    hi_u16 norm_coeff2 = 1000;

    for (i = 0; i < ISP_CSC_COEF_NUM; i++) {
        dyna_reg_cfg->csc_coef[i] = csc_attr->csc_coef[i] * norm_coeff1 / norm_coeff2;
    }

    for (i = 0; i < ISP_CSC_DC_NUM; i++) {
        dyna_reg_cfg->csc_in_dc[i]  = csc_attr->csc_in_dc[i] * 4; /* multiply by 4 bits to fit register */
        dyna_reg_cfg->csc_out_dc[i] = csc_attr->csc_out_dc[i] * 4; /* multiply by 4 bits to fit register */
    }

    dyna_reg_cfg->resh = HI_TRUE;
}

static hi_void csc_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    /* csc regs initialize */
    hi_u8 i;
    hi_isp_csc_matrx csc_attr;

    /* initilize color gamut information */
    memcpy_s(&csc_attr, sizeof(hi_isp_csc_matrx), &g_csc_attr_709, sizeof(hi_isp_csc_matrx));

    /* config all four csc attributes. */
    for (i = 0; i < reg_cfg->cfg_num; i++) {
        /* coefficiets config */
        csc_dyna_regs_config(&csc_attr, &reg_cfg->alg_reg_cfg[i].csc_cfg.dyna_reg_cfg);

        reg_cfg->alg_reg_cfg[i].csc_cfg.enable = HI_TRUE;
    }

    /* refresh flags are forced on */
    reg_cfg->cfg_key.bit1_csc_cfg = HI_TRUE;
}

static hi_void csc_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8 i;

    hi_isp_csc_matrx csc_attr;
    hi_ext_system_csc_gamuttype_write(vi_pipe, COLOR_GAMUT_BT709);
    memcpy_s(&csc_attr, sizeof(hi_isp_csc_matrx), &g_csc_attr_709, sizeof(hi_isp_csc_matrx));

    /* write csc related parameters */
    hi_ext_system_csc_hue_write(vi_pipe, ISP_CSC_DFT_HUE);
    hi_ext_system_csc_sat_write(vi_pipe, ISP_CSC_DFT_SAT);
    hi_ext_system_csc_contrast_write(vi_pipe, ISP_CSC_DFT_CON);
    hi_ext_system_csc_luma_write(vi_pipe, ISP_CSC_DFT_LUMA);
    hi_ext_system_csc_limitrange_en_write(vi_pipe, HI_FALSE);
    hi_ext_system_csc_ext_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_csc_ctmode_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_csc_attr_update_write(vi_pipe, HI_TRUE);

    /* write DC components */
    for (i = 0; i < ISP_CSC_DC_NUM; i++) {
        hi_ext_system_csc_dcin_write(vi_pipe, i,  csc_attr.csc_in_dc[i]);
        hi_ext_system_csc_dcout_write(vi_pipe, i, csc_attr.csc_out_dc[i]);
    }

    /* write coefficients */
    for (i = 0; i < ISP_CSC_COEF_NUM; i++) {
        hi_ext_system_csc_coef_write(vi_pipe, i, csc_attr.csc_coef[i]);
    }

    hi_ext_system_csc_enable_write(vi_pipe, HI_TRUE);
}

hi_void isp_csc_read_ext_regs(hi_vi_pipe vi_pipe, isp_csc *csc)
{
    hi_u8 i;
    hi_u8 gamut;

    csc->update = hi_ext_system_csc_attr_update_read(vi_pipe);
    hi_ext_system_csc_attr_update_write(vi_pipe, HI_FALSE);

    if (csc->update) {
        csc->mpi_cfg.limited_range_en = hi_ext_system_csc_limitrange_en_read(vi_pipe);
        csc->mpi_cfg.ext_csc_en = hi_ext_system_csc_ext_en_read(vi_pipe);
        csc->mpi_cfg.ct_mode_en = hi_ext_system_csc_ctmode_en_read(vi_pipe);

        gamut = hi_ext_system_csc_gamuttype_read(vi_pipe);
        if (gamut == COLOR_GAMUT_BT2020) {
            isp_warn_trace("invalid color gamut type, set back to previous gamut type!\n");
        } else {
            csc->mpi_cfg.color_gamut = gamut;
        }

        csc->mpi_cfg.contr = hi_ext_system_csc_contrast_read(vi_pipe);
        csc->mpi_cfg.hue   = hi_ext_system_csc_hue_read(vi_pipe);
        csc->mpi_cfg.satu  = hi_ext_system_csc_sat_read(vi_pipe);
        csc->mpi_cfg.luma = hi_ext_system_csc_luma_read(vi_pipe);

        for (i = 0; i < ISP_CSC_COEF_NUM; i++) {
            csc->mpi_cfg.csc_magtrx.csc_coef[i] = hi_ext_system_csc_coef_read(vi_pipe, i);
        }

        /* DC components read */
        for (i = 0; i < ISP_CSC_DC_NUM; i++) {
            csc->mpi_cfg.csc_magtrx.csc_in_dc[i] = hi_ext_system_csc_dcin_read(vi_pipe, i);
            csc->mpi_cfg.csc_magtrx.csc_out_dc[i] = hi_ext_system_csc_dcout_read(vi_pipe, i);
        }
    }
}

static hi_s32 csc_cal_matrix(hi_vi_pipe vi_pipe, isp_csc *csc, hi_isp_csc_matrx *csc_coef)
{
    hi_s32 luma = 0;
    hi_s32 contrast, satu, hue;
    hi_s32 i = 0;
    hi_s16 black_in = 0;
    hi_s16 black_out = 0;
    hi_s16 norm_factor2 = 100;
    hi_s16 norm_factor3 = 1000;
    hi_s16 norm_factor4 = 10000;
    hi_isp_csc_matrx *csc_coef_tmp = HI_NULL;

    if (csc->mpi_cfg.ext_csc_en) {
        luma = (hi_s32)csc->mpi_cfg.luma - 50;  /* -50 ~ 50 */
        luma = luma * 0x80 / 50;  /* -128 ~ 128 */ /* convert by 50 */
        luma = (luma == 128) ? 127 : luma; /* max value shold be 127(7bits) , not 128(8bits) */
    } else {
        luma = (hi_s32)csc->mpi_cfg.luma * 64 / 100 - 32; /* convert from [0, 100] to [-32, 32], use 64 as norm */
        luma = CLIP3(luma, -128, 128); /* limit the result of luma, ,[-128, 128] */
    }

    contrast = ((hi_s32)csc->mpi_cfg.contr - 50) * 2 + 100; /*  contrast from [0, 100] to [0, 200], use 50 to convert */
    hue  = (hi_s32)csc->mpi_cfg.hue * 60 / 100; /* convert hue from [0, 100] to [0, 60] */
    hue  = CLIP3(hue, 0, 60); /* hue range in [0, 60] */
    satu = ((hi_s32)csc->mpi_cfg.satu - 50) * 2 + 100; /* convert satu from [0, 100] to [0, 200], use 50 to convert */

    switch (csc->mpi_cfg.color_gamut) {
        case COLOR_GAMUT_BT601:
            csc_coef_tmp  = (csc->mpi_cfg.limited_range_en) ? &g_csc_attr_601_limited : &g_csc_attr_601;
            break;
        case COLOR_GAMUT_BT709:
            csc_coef_tmp  = (csc->mpi_cfg.limited_range_en) ? &g_csc_attr_709_limited : &g_csc_attr_709;
            break;
        case COLOR_GAMUT_USER:
            csc_coef_tmp  = &(csc->mpi_cfg.csc_magtrx);
            csc_coef_tmp->csc_out_dc[0] = (csc->mpi_cfg.limited_range_en) ? 16 : 0; /* 16 as DC value */
            break;
        default:
            isp_err_trace("invalid color gamut type!\n");
            return HI_FAILURE;
    }

    if ((csc->mpi_cfg.ct_mode_en) && (csc->mpi_cfg.color_gamut <= COLOR_GAMUT_USER)) {
        black_in  = -128; /* -128 as default input dc value */
        /* 110 = (128 * 219) / 256, full range to limited range conversion */
        black_out = (csc->mpi_cfg.limited_range_en) ? 110 : 127; /* 110 equal (129*219)/256, 127 is default dc value */
    } else {
        black_in  = 0;
        black_out = 0;
    }

    csc_coef->csc_coef[0] = (hi_s16)((contrast * ((hi_s32)csc_coef_tmp->csc_coef[0])) / norm_factor2);
    csc_coef->csc_coef[1] = (hi_s16)((contrast * ((hi_s32)csc_coef_tmp->csc_coef[1])) / norm_factor2);
    csc_coef->csc_coef[2] = (hi_s16)((contrast * ((hi_s32)csc_coef_tmp->csc_coef[2])) / norm_factor2); /* [2] */
    csc_coef->csc_coef[3] = (hi_s16)((contrast * satu * ((hi_s32)(csc_coef_tmp->csc_coef[3] * /* 3 3 */
                                                        g_csc_cos_table[hue] + csc_coef_tmp->csc_coef[6] * /* 6 */
                                                        g_csc_sin_table[hue]) / norm_factor3)) / norm_factor4);
    csc_coef->csc_coef[4] = (hi_s16)((contrast * satu * ((hi_s32)(csc_coef_tmp->csc_coef[4] * /* 4 4 */
                                                        g_csc_cos_table[hue] + csc_coef_tmp->csc_coef[7] * /* 7 */
                                                        g_csc_sin_table[hue]) / norm_factor3)) / norm_factor4);
    csc_coef->csc_coef[5] = (hi_s16)((contrast * satu * ((hi_s32)(csc_coef_tmp->csc_coef[5] * /* 5 5 */
                                                        g_csc_cos_table[hue] + csc_coef_tmp->csc_coef[8] * /* 8 */
                                                        g_csc_sin_table[hue]) / norm_factor3)) / norm_factor4);
    csc_coef->csc_coef[6] = (hi_s16)((contrast * satu * ((hi_s32)(csc_coef_tmp->csc_coef[6] * /* 6 6 */
                                                        g_csc_cos_table[hue] - csc_coef_tmp->csc_coef[3] * /* 3 */
                                                        g_csc_sin_table[hue]) / norm_factor3)) / norm_factor4);
    csc_coef->csc_coef[7] = (hi_s16)((contrast * satu * ((hi_s32)(csc_coef_tmp->csc_coef[7] * /* 7 7 */
                                                        g_csc_cos_table[hue] - csc_coef_tmp->csc_coef[4] * /* 4 */
                                                        g_csc_sin_table[hue]) / norm_factor3)) / norm_factor4);
    csc_coef->csc_coef[8] = (hi_s16)((contrast * satu * ((hi_s32)(csc_coef_tmp->csc_coef[8] * /* 8 8 */
                                                        g_csc_cos_table[hue] - csc_coef_tmp->csc_coef[5] *  /* 5 */
                                                        g_csc_sin_table[hue]) / norm_factor3)) / norm_factor4);

    for (i = 0; i < ISP_CSC_DC_NUM; i++) {
        if (csc->mpi_cfg.color_gamut == COLOR_GAMUT_USER) {
            csc_coef_tmp->csc_in_dc[i] = 0;
        }
        csc_coef->csc_in_dc[i] = csc_coef_tmp->csc_in_dc[i] + black_in;
        csc_coef->csc_out_dc[i] = csc_coef_tmp->csc_out_dc[i];
    }

    /* add luma */
    csc_coef->csc_out_dc[0] += ((hi_s16)luma + black_out);

    /* update ext regs: */
    for (i = 0; i < ISP_CSC_DC_NUM; i++) {
        hi_ext_system_csc_dcin_write(vi_pipe, i,  csc_coef->csc_in_dc[i]);
        hi_ext_system_csc_dcout_write(vi_pipe, i, csc_coef->csc_out_dc[i]);
    }

    if (csc->mpi_cfg.color_gamut != COLOR_GAMUT_USER) {
        for (i = 0; i < ISP_CSC_COEF_NUM; i++) {
            hi_ext_system_csc_coef_write(vi_pipe, i, csc_coef->csc_coef[i]);
        }
    }

    return HI_SUCCESS;
}

static hi_bool __inline check_csc_open(isp_csc *csc)
{
    return (csc->mpi_cfg.enable == HI_TRUE);
}

static hi_s32 csc_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    return HI_SUCCESS;
}

static hi_s32 csc_color_gamutinfo_get(hi_vi_pipe vi_pipe,  hi_isp_colorgammut_info *color_gamut_info)
{
    isp_csc *csc = HI_NULL;

    csc_get_ctx(vi_pipe, csc);

    color_gamut_info->color_gamut = csc->mpi_cfg.color_gamut;

    return HI_SUCCESS;
}

hi_s32 isp_csc_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_csc *csc)
{
    hi_u8  i;
    hi_s32 ret;
    hi_isp_csc_matrx csc_attr;

    /* calculate actual csc coefs */
    ret = csc_cal_matrix(vi_pipe, csc, &csc_attr);
    if (ret == HI_FAILURE) {
        return HI_FAILURE;
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        /* coefficiets config */
        csc_dyna_regs_config(&csc_attr, &reg_cfg->alg_reg_cfg[i].csc_cfg.dyna_reg_cfg);
    }

    /* refresh flags are forced on */
    reg_cfg->cfg_key.bit1_csc_cfg = HI_TRUE;

    return HI_SUCCESS;
}

hi_void isp_csc_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    csc_regs_initialize(vi_pipe, reg_cfg);
    csc_ext_regs_initialize(vi_pipe);
}

static hi_s32 isp_csc_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    isp_csc_param_init(vi_pipe, (isp_reg_cfg *)reg_cfg);

    return HI_SUCCESS;
}

static hi_s32 isp_csc_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_u16 i;
    isp_csc      *csc     = HI_NULL;
    isp_usr_ctx  *isp_ctx = HI_NULL;
    isp_reg_cfg  *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    isp_get_ctx(vi_pipe, isp_ctx);
    csc_get_ctx(vi_pipe, csc);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    csc->mpi_cfg.enable = hi_ext_system_csc_enable_read(vi_pipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].csc_cfg.enable = csc->mpi_cfg.enable;
    }

    reg_cfg->cfg_key.bit1_csc_cfg = 1;

    /* check hardware setting */
    if (!check_csc_open(csc)) {
        return HI_SUCCESS;
    }

    /* read ext regs */
    isp_csc_read_ext_regs(vi_pipe, csc);

    return isp_csc_reg_update(vi_pipe, reg_cfg, csc);
}

static hi_s32 isp_csc_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    switch (cmd) {
        case ISP_PROC_WRITE:
            csc_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        case ISP_COLORGAMUTINFO_GET:
            csc_color_gamutinfo_get(vi_pipe, (hi_isp_colorgammut_info *)value);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_csc_exit(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_csc(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_csc);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_CSC;
    algs->alg_func.pfn_alg_init = isp_csc_init;
    algs->alg_func.pfn_alg_run  = isp_csc_run;
    algs->alg_func.pfn_alg_ctrl = isp_csc_ctrl;
    algs->alg_func.pfn_alg_exit = isp_csc_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

