/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_frame_switch_wdr.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <math.h>
#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_proc.h"

#define HI_WDR_BITDEPTH                  12
#define HI_ISP_WDR_NOISE_CWEIGHT_DEFAULT 3
#define HI_ISP_WDR_NOISE_GWEIGHT_DEFAULT 3
#define HI_WDR_SQRT_GAIN_BITDEP          16
#define HI_WDR_EXPOSURE_BASE             0x40
#define HI_ISP_WDR_MAX_VALUE             ((1 << 22) - 1)
#define HI_ISP_WDR_SQRT_OUT_BITS         8
#define HI_ISP_WDR_BIT_DEPTH_PRC         14
#define HI_ISP_WDR_R_RATIO_BITS          10
#define HI_ISP_WDR_BCOM_ALPHA_DEFAULT    2
#define HI_ISP_WDR_BDEC_ALPHA_DEFAULT    4
#define HI_ISP_WDR_2M1_FRAME             2
#define HI_ISP_WDR_FUSION_RATIO_THR      576
#define HI_ISP_WDR_MERGE_FRAME_VS        0
#define HI_ISP_WDR_MERGE_FRAME_S         1
#define HI_ISP_WDR_MERGE_FRAME_M         2
#define HI_ISP_WDR_MERGE_FRAME_L         3
#define HI_ISP_WDR_AWB_FRACTION_BITS     8
#define HI_ISP_WDR_SENSOR_GAIN_SHIFT_BITS 16
#define HI_ISP_WDR_SENSOR_GAIN_BASE     1024
#define HI_ISP_WDR_SENSOR_GAIN_ROUND    512
#define HI_ISP_WDR_RATIO_MAX            16384
#define HI_WDR_RG_WGT                   3
#define HI_WDR_BG_WGT                   3
#define HI_FUSION_THR_BIT               12
#define HI_FUSION_THR_SHIFT_BIT         8
#define HI_WDR_THR_BIT                  12
#define HI_WDR_MD_LOW_THR               45
#define HI_WDR_MD_HIG_THR               45
#define HI_WDR_AWB_RGAIN_INDEX          0
#define HI_WDR_AWB_GGAIN_INDEX          1
#define HI_WDR_AWB_BGAIN_INDEX          3

#define wdr_clip3(low, high, x) (MAX2(MIN2((x), high), low))

static const  hi_s32 g_noise_again_set[NOISESET_ELENUM] = { 1, 2, 4, 8, 16, 32, 64 };
static const  hi_s32 g_noise_floor_set[NOISESET_ELENUM] = { 1, 2, 3, 6, 11, 17, 21 };
static const  hi_s32 g_fusion_thr[WDR_MAX_FRAME] = {3855, 3000};
static const  hi_u8  g_lut_mdt_low_thr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};                     /* u4.2 */
static const  hi_u8  g_lut_mdt_hig_thr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};                  /* u4.2 */

typedef struct {
    /* Public */
    /* fw input */
    hi_bool     coef_update_en;
    hi_bool     mdt_en;
    hi_bool     fusion_mode;
    hi_bool     wdr_en;
    hi_bool     erosion_en;
    hi_bool     short_expo_chk;
    hi_bool     md_ref_flicker;
    hi_bool     manual_mode;
    hi_u8       md_thr_low_gain;                         /* u4.2, [0,63] */
    hi_u8       md_thr_hig_gain;                         /* u4.2, [0,63] */
    hi_u8       actual_md_thr_low_gain;
    hi_u8       actual_md_thr_hig_gain;
    hi_u8       md_thr_low_gain_lut[ISP_AUTO_ISO_STRENGTH_NUM];
    hi_u8       md_thr_hig_gain_lut[ISP_AUTO_ISO_STRENGTH_NUM];
    hi_u8       bit_depth_prc;
    hi_u8       bit_depth_in_valid;
    hi_u8       frames_merge;
    hi_u8       nos_g_wgt_mod;
    hi_u8       nos_c_wgt_mod;
    hi_u8       mdt_l_bld;
    hi_u8       noise_model_coef;
    hi_u8       noise_ratio_rg;
    hi_u8       noise_ratio_bg;

    hi_u8       mdt_still_thr;
    hi_u8       mdt_full_thr;
    hi_u8       full_mot_sig_wgt;
    hi_u8       floor_set_lut[NOISESET_ELENUM];
    hi_u16      short_thr;
    hi_u16      long_thr;
    hi_u16      fusion_barrier0;      /* U14.0 */
    hi_u16      fusion_barrier1;      /* U14.0 */
    hi_u16      fusion_barrier2;      /* U14.0 */
    hi_u16      fusion_barrier3;      /* U14.0 */
    hi_u32      pre_iso129;
    hi_u32      pre_again;
    hi_s32      pre_mdt_noise;
    hi_u32      again_set_lut[NOISESET_ELENUM];
    hi_u8       texture_thd_wgt;

    hi_bool     force_long;         /* u1.0,[0,1] */
    hi_u16      force_long_low_thr; /* u11.0,[0,4095] */
    hi_u16      force_long_hig_thr; /* u11.0,[0,4095] */
    hi_u16      short_check_thd; /* u11.0,[0,4095] */

    hi_u16      fusion_saturate_thd;          /* u12.0,[0,4095] */

    hi_isp_bnr_mode bnr_mode;
} isp_fs_wdr;

isp_fs_wdr *g_fswdr_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define fs_wdr_get_ctx(dev, ctx)   ((ctx) = g_fswdr_ctx[dev])
#define fs_wdr_set_ctx(dev, ctx)   (g_fswdr_ctx[dev] = (ctx))
#define fs_wdr_reset_ctx(dev)         (g_fswdr_ctx[dev] = HI_NULL)

hi_s32 frame_wdr_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_fs_wdr *fswdr_ctx = HI_NULL;

    fs_wdr_get_ctx(vi_pipe, fswdr_ctx);

    if (fswdr_ctx == HI_NULL) {
        fswdr_ctx = (isp_fs_wdr *)isp_malloc(sizeof(isp_fs_wdr));
        if (fswdr_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] fs_wdr_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(fswdr_ctx, sizeof(isp_fs_wdr), 0, sizeof(isp_fs_wdr));

    fs_wdr_set_ctx(vi_pipe, fswdr_ctx);

    return HI_SUCCESS;
}

hi_void frame_wdr_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_fs_wdr *fswdr_ctx ;

    fs_wdr_get_ctx(vi_pipe, fswdr_ctx);
    isp_free(fswdr_ctx);
    fs_wdr_reset_ctx(vi_pipe);
}

static hi_u32 wdr_sqrt(hi_u32 val, hi_u32 dst_bit_dep)
{
    hi_u32 X;                                                       /* u10.0 */
    hi_u32 Y;                                                       /* u20.0 */
    hi_s8 j;
    hi_u8 shift_value = 2;
    hi_u8 jtemp;

    X = (1 << dst_bit_dep) - 1;
    Y = X * X;

    val = val << shift_value;

    for (j = dst_bit_dep; j >= 0; j--) {
        jtemp = j;
        if (val > Y) {
            Y = Y + ((hi_u64)1 << (jtemp + 1)) * X + ((hi_u64)1 << (shift_value * jtemp));
            X = X + ((hi_u64)1 << jtemp);                               /* u10.0 */
        } else {
            Y = Y - ((hi_u64)1 << (jtemp + 1)) * X + ((hi_u64)1 << (shift_value * jtemp));
            X = X - ((hi_u64)1 << jtemp);                               /* u10.0 */
        }
    }

    if (val > Y) {
        X = X + 1;
    } else if (val < Y) {
        X = X - 1;
    }

    X = X >> 1;

    return X;
}

static hi_void frame_wdr_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8 i, j;
    isp_fs_wdr *fswdr = HI_NULL;

    fs_wdr_get_ctx(vi_pipe, fswdr);

    hi_ext_system_wdr_en_write(vi_pipe, fswdr->wdr_en);
    hi_ext_system_wdr_coef_update_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_erosion_en_write(vi_pipe, HI_EXT_SYSTEM_EROSION_EN_DEFAULT);
    hi_ext_system_mdt_en_write(vi_pipe, fswdr->mdt_en);
    hi_ext_system_wdr_shortexpo_chk_write(vi_pipe, fswdr->short_expo_chk);
    hi_ext_system_wdr_mdref_flicker_write(vi_pipe, fswdr->md_ref_flicker);
    hi_ext_system_bnr_mode_write(vi_pipe, fswdr->bnr_mode);
    hi_ext_system_fusion_mode_write(vi_pipe, fswdr->fusion_mode);
    hi_ext_system_wdr_bnr_full_mdt_thr_write(vi_pipe, HI_EXT_SYSTEM_WDR_BNR_FULL_MDT_THR_DEFAULT);
    hi_ext_system_wdr_g_sigma_gain1_write(vi_pipe, HI_EXT_SYSTEM_WDR_G_SIGMA_GAIN1_DEFAULT);
    hi_ext_system_wdr_g_sigma_gain2_write(vi_pipe, HI_EXT_SYSTEM_WDR_G_SIGMA_GAIN2_DEFAULT);
    hi_ext_system_wdr_g_sigma_gain3_write(vi_pipe, HI_EXT_SYSTEM_WDR_G_SIGMA_GAIN3_DEFAULT);
    hi_ext_system_wdr_c_sigma_gain1_write(vi_pipe, HI_EXT_SYSTEM_WDR_C_SIGMA_GAIN1_DEFAULT);
    hi_ext_system_wdr_c_sigma_gain2_write(vi_pipe, HI_EXT_SYSTEM_WDR_C_SIGMA_GAIN2_DEFAULT);
    hi_ext_system_wdr_c_sigma_gain3_write(vi_pipe, HI_EXT_SYSTEM_WDR_C_SIGMA_GAIN3_DEFAULT);
    hi_ext_system_wdr_full_mot_sigma_weight_write(vi_pipe, HI_EXT_SYSTEM_WDR_FULL_MOT_SIGMA_WEIGHT_DEFAULT);
    hi_ext_system_wdr_mdt_full_thr_write(vi_pipe, HI_EXT_SYSTEM_WDR_MDT_FULL_THR_DEFAULT);
    hi_ext_system_wdr_mdt_long_blend_write(vi_pipe, fswdr->mdt_l_bld);
    hi_ext_system_wdr_mdt_still_thr_write(vi_pipe, fswdr->mdt_still_thr);
    hi_ext_system_wdr_longthr_write(vi_pipe, fswdr->long_thr);
    hi_ext_system_wdr_shortthr_write(vi_pipe, fswdr->short_thr);
    hi_ext_system_wdr_noise_c_weight_mode_write(vi_pipe, HI_EXT_SYSTEM_WDR_NOISE_C_WEIGHT_MODE_DEFAULT);
    hi_ext_system_wdr_noise_g_weight_mode_write(vi_pipe, HI_EXT_SYSTEM_WDR_NOISE_G_WEIGHT_MODE_DEFAULT);
    hi_ext_system_wdr_noise_model_coef_write(vi_pipe, fswdr->noise_model_coef);
    hi_ext_system_wdr_noise_ratio_rg_write(vi_pipe, HI_EXT_SYSTEM_WDR_NOISE_RATIO_RG_DEFAULT);
    hi_ext_system_wdr_noise_ratio_bg_write(vi_pipe, HI_EXT_SYSTEM_WDR_NOISE_RATIO_BG_DEFAULT);

    hi_ext_system_wdr_manual_mode_write(vi_pipe, OP_TYPE_AUTO);
    hi_ext_system_wdr_manual_mdthr_low_gain_write(vi_pipe, HI_ISP_WDR_MDTHR_LOW_GAIN_DEFAULT);
    hi_ext_system_wdr_manual_mdthr_hig_gain_write(vi_pipe, HI_ISP_WDR_MDTHR_HIG_GAIN_DEFAULT);

    for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) {
        hi_ext_system_wdr_auto_mdthr_low_gain_write(vi_pipe, j, fswdr->md_thr_low_gain_lut[j]);
        hi_ext_system_wdr_auto_mdthr_hig_gain_write(vi_pipe, j, fswdr->md_thr_hig_gain_lut[j]);
    }

    hi_ext_system_fusion_thr_write(vi_pipe, 0, fswdr->fusion_barrier0);
    hi_ext_system_fusion_thr_write(vi_pipe, 1, fswdr->fusion_barrier1);

    for (i = 0; i < NOISESET_ELENUM; i++) {
        hi_ext_system_wdr_floorset_write(vi_pipe, i, g_noise_floor_set[i]);
    }

    hi_ext_system_wdr_sfnr_en_write(vi_pipe, HI_EXT_SYSTEM_WDR_WDR_SFNR_EN_WGT);
    hi_ext_system_wdr_forcelong_en_write(vi_pipe, fswdr->force_long);
    hi_ext_system_wdr_forcelong_low_thd_write(vi_pipe, fswdr->force_long_low_thr);
    hi_ext_system_wdr_forcelong_high_thd_write(vi_pipe, fswdr->force_long_hig_thr);
    hi_ext_system_wdr_shortcheck_thd_write(vi_pipe, fswdr->short_check_thd);

    hi_ext_system_wdr_shortsigmal1_cwgt_write(vi_pipe, HI_EXT_SYSTEM_WDR_SHORTSIGMAL1_CWGT_WGT);
    hi_ext_system_wdr_shortsigmal2_cwgt_write(vi_pipe, HI_EXT_SYSTEM_WDR_SHORTSIGMAL2_CWGT_WGT);
    hi_ext_system_wdr_shortsigmal1_gwgt_write(vi_pipe, HI_EXT_SYSTEM_WDR_SHORTSIGMAL1_GWGT_WGT);
    hi_ext_system_wdr_shortsigmal2_gwgt_write(vi_pipe, HI_EXT_SYSTEM_WDR_SHORTSIGMAL2_GWGT_WGT);

    hi_ext_system_wdr_mot2sig_cwgt_high_write(vi_pipe, HI_EXT_SYSTEM_WDR_MOT2SIG_CWGT_WGT);
    hi_ext_system_wdr_mot2sig_gwgt_high_write(vi_pipe, HI_EXT_SYSTEM_WDR_MOT2SIG_GWGT_WGT);

    hi_ext_system_wdr_fusionsigma_cwgt0_write(vi_pipe, HI_EXT_SYSTEM_WDR_FUSIONSIGMA_CWGT0_WGT);
    hi_ext_system_wdr_fusionsigma_cwgt1_write(vi_pipe, HI_EXT_SYSTEM_WDR_FUSIONSIGMA_CWGT1_WGT);

    hi_ext_system_wdr_fusionsigma_gwgt0_write(vi_pipe, HI_EXT_SYSTEM_WDR_FUSIONSIGMA_GWGT0_WGT);
    hi_ext_system_wdr_fusionsigma_gwgt1_write(vi_pipe, HI_EXT_SYSTEM_WDR_FUSIONSIGMA_GWGT1_WGT);

    hi_ext_system_wdr_shortframe_nrstr_write(vi_pipe, HI_EXT_SYSTEM_WDR_SHORTFRAME_NR_STR_WGT);
    hi_ext_system_wdr_motionbnrstr_write(vi_pipe, HI_EXT_SYSTEM_WDR_MOTION_BNR_STR_WGT);
    hi_ext_system_wdr_fusionbnrstr_write(vi_pipe, HI_EXT_SYSTEM_WDR_FUSION_BNR_STR_WGT);
}

static hi_void frame_wdr_static_regs_initialize(hi_vi_pipe vi_pipe, hi_u8 wdr_mode,
                                                isp_fswdr_static_cfg *static_reg_cfg, isp_usr_ctx *isp_ctx)
{
    hi_u32 saturate_low, saturate_hig;
    hi_s32 blc_value;
    hi_s32 m_max_value_in = isp_bitmask(HI_WDR_BITDEPTH);
    hi_u32 expo_value_lut[EXP_RATIO_NUM] = { 0 };
    hi_isp_cmos_black_level  *sns_black_level = HI_NULL;
    hi_isp_cmos_default      *sns_dft         = HI_NULL;
    isp_fs_wdr               *fswdr           = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);
    fs_wdr_get_ctx(vi_pipe, fswdr);

    blc_value = (hi_s32)(sns_black_level->black_level[0]);

    if (is_2to1_wdr_mode(wdr_mode)) {
        expo_value_lut[0] = CLIP3(sns_dft->wdr_switch_attr.exp_ratio[0],
                                  HI_WDR_EXPOSURE_BASE, isp_bitmask(HI_ISP_WDR_BIT_DEPTH_PRC));
        expo_value_lut[1] = HI_WDR_EXPOSURE_BASE;

        static_reg_cfg->blc_comp_lut[0]   = expo_value_lut[0] - expo_value_lut[1];
        static_reg_cfg->expo_r_ratio_lut[0] = MIN2(isp_bitmask(HI_ISP_WDR_R_RATIO_BITS),
            (isp_bitmask(HI_ISP_WDR_R_RATIO_BITS) * HI_WDR_EXPOSURE_BASE / div_0_to_1(expo_value_lut[0])));
        static_reg_cfg->flick_exp_ratio = expo_value_lut[0];
    } else {
        static_reg_cfg->expo_value_lut[0]   = 0;
        static_reg_cfg->expo_value_lut[1]   = 0;
        static_reg_cfg->blc_comp_lut[0]     = 0;
        static_reg_cfg->expo_r_ratio_lut[0] = 0;
        static_reg_cfg->flick_exp_ratio     = 0;
    }

    if (fswdr->fusion_mode == HI_FALSE) {
        static_reg_cfg->expo_value_lut[0] = expo_value_lut[0];
        static_reg_cfg->expo_value_lut[1] = expo_value_lut[1];
        static_reg_cfg->max_ratio = HI_ISP_WDR_MAX_VALUE / div_0_to_1(expo_value_lut[0]);
    } else {
        static_reg_cfg->expo_value_lut[0] = expo_value_lut[0] + HI_WDR_EXPOSURE_BASE;
        static_reg_cfg->expo_value_lut[1] = expo_value_lut[1];
        static_reg_cfg->max_ratio = HI_ISP_WDR_MAX_VALUE / div_0_to_1(expo_value_lut[0] + HI_WDR_EXPOSURE_BASE);
    }

    static_reg_cfg->save_blc         = HI_ISP_WDR_SAVE_BLC_EN_DEFAULT;
    static_reg_cfg->gray_scale_mode  = HI_ISP_WDR_GRAYSCALE_DEFAULT;
    static_reg_cfg->mask_similar_thr = HI_ISP_WDR_MASK_SIMILAR_THR_DEFAULT;
    static_reg_cfg->mask_similar_cnt = HI_ISP_WDR_MASK_SIMILAR_CNT_DEFAULT;
    static_reg_cfg->dft_wgt_fl       = HI_ISP_WDR_DFTWGT_FL_DEFAULT;
    static_reg_cfg->bldr_lhf_idx     = HI_ISP_WDR_BLDRLHFIDX_DEFAULT;

    static_reg_cfg->saturate_thr   = HI_ISP_WDR_SATURATE_THR_DEFAULT;

    saturate_hig = ((hi_u32)(m_max_value_in - blc_value));
    saturate_low = wdr_sqrt(saturate_hig, HI_ISP_WDR_SQRT_OUT_BITS);
    static_reg_cfg->saturate_thr         = (hi_u16)(saturate_hig - saturate_low);
    static_reg_cfg->fusion_saturate_thd  = static_reg_cfg->saturate_thr;
    static_reg_cfg->force_long_smooth_en = HI_ISP_WDR_FORCELONG_SMOOTH_EN;
    static_reg_cfg->resh                 = HI_TRUE;
    static_reg_cfg->first_frame          = HI_TRUE;
}

static hi_void frame_wdr_usr_regs_initialize(isp_fswdr_usr_cfg *usr_reg_cfg, isp_fs_wdr *fswdr)
{
    usr_reg_cfg->fusion_mode        = fswdr->fusion_mode;
    usr_reg_cfg->short_expo_chk     = fswdr->short_expo_chk;
    usr_reg_cfg->mdt_l_bld          = fswdr->mdt_l_bld;
    usr_reg_cfg->mdt_still_thr      = fswdr->mdt_still_thr;
    usr_reg_cfg->mdt_full_thr       = fswdr->mdt_full_thr;
    usr_reg_cfg->pixel_avg_max_diff = HI_ISP_WDR_PIXEL_AVG_MAX_DIFF_DEFAULT;
    usr_reg_cfg->resh               = HI_TRUE;
    usr_reg_cfg->update_index       = 1;
}

static hi_void frame_wdr_sync_regs_initialize(isp_fswdr_sync_cfg *sync_reg_cfg, isp_fs_wdr *fswdr)
{
    sync_reg_cfg->fusion_mode     = fswdr->fusion_mode;
    sync_reg_cfg->wdr_mdt_en      = fswdr->mdt_en;
    sync_reg_cfg->short_thr       = fswdr->short_thr;
    sync_reg_cfg->long_thr        = fswdr->long_thr;
}

static hi_void frame_wdrs_dyna_regs_initialize(hi_vi_pipe vi_pipe, hi_u8 wdr_mode,
                                               isp_fswdr_dyna_cfg *dyna_reg_cfg, isp_fs_wdr *fswdr)
{
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (is_linear_mode(wdr_mode)) {
        dyna_reg_cfg->bcom_en      =   HI_FALSE;
        dyna_reg_cfg->bdec_en      =   HI_FALSE;
        dyna_reg_cfg->frm_merge    =   1;
        dyna_reg_cfg->bcom_alpha   =   0;
        dyna_reg_cfg->bdec_alpha   =   0;
    } else if (is_built_in_wdr_mode(wdr_mode)) {
        dyna_reg_cfg->bcom_en      =   HI_TRUE;
        dyna_reg_cfg->bdec_en      =   HI_TRUE;
        dyna_reg_cfg->frm_merge    =   1;
        dyna_reg_cfg->bcom_alpha   =   HI_ISP_WDR_BCOM_ALPHA_DEFAULT;
        dyna_reg_cfg->bdec_alpha   =   HI_ISP_WDR_BDEC_ALPHA_DEFAULT;
    } else if (is_2to1_wdr_mode(wdr_mode)) {
        dyna_reg_cfg->bcom_en      =   HI_TRUE;
        dyna_reg_cfg->bdec_en      =   HI_TRUE;
        dyna_reg_cfg->frm_merge    =   HI_ISP_WDR_2M1_FRAME;
        dyna_reg_cfg->bcom_alpha   =   HI_ISP_WDR_BCOM_ALPHA_DEFAULT;
        dyna_reg_cfg->bdec_alpha   =   HI_ISP_WDR_BDEC_ALPHA_DEFAULT;
    } else {
        dyna_reg_cfg->bcom_en      =   HI_FALSE;
        dyna_reg_cfg->bdec_en      =   HI_FALSE;
        dyna_reg_cfg->frm_merge    =   1;
        dyna_reg_cfg->bcom_alpha   =   0;
        dyna_reg_cfg->bdec_alpha   =   0;
    }

    dyna_reg_cfg->wdr_mdt_en        = fswdr->mdt_en;
    dyna_reg_cfg->sqrt_again_g      = HI_ISP_WDR_SQRT_AGAIN_G_DEFAULT;
    dyna_reg_cfg->sqrt_dgain_g      = HI_ISP_WDR_SQRT_DGAIN_G_DEFAULT;
    dyna_reg_cfg->mdt_nos_floor     = HI_ISP_WDR_MDT_NOS_FLOOR_DEFAULT;
    dyna_reg_cfg->short_thr         = fswdr->short_thr;
    dyna_reg_cfg->long_thr          = fswdr->long_thr;
    dyna_reg_cfg->still_thr_lut[0]  = HI_ISP_WDR_STILL_THR0_DEFAULT;

    dyna_reg_cfg->md_thr_low_gain    = HI_ISP_WDR_MDTHR_LOW_GAIN_DEFAULT;
    dyna_reg_cfg->md_thr_hig_gain    = HI_ISP_WDR_MDTHR_HIG_GAIN_DEFAULT;
    dyna_reg_cfg->erosion_en         = fswdr->erosion_en;

    dyna_reg_cfg->fusion_thr_r_lut[0]  = HI_ISP_WDR_FUSION_F0_THR_R;
    dyna_reg_cfg->fusion_thr_r_lut[1]  = HI_ISP_WDR_FUSION_F1_THR_R;

    dyna_reg_cfg->fusion_thr_g_lut[0]  = HI_ISP_WDR_FUSION_F0_THR_G;
    dyna_reg_cfg->fusion_thr_g_lut[1]  = HI_ISP_WDR_FUSION_F1_THR_G;

    dyna_reg_cfg->fusion_thr_b_lut[0]  = HI_ISP_WDR_FUSION_F0_THR_B;
    dyna_reg_cfg->fusion_thr_b_lut[1]  = HI_ISP_WDR_FUSION_F1_THR_B;

    dyna_reg_cfg->force_long         = fswdr->force_long;
    dyna_reg_cfg->force_long_low_thr = fswdr->force_long_low_thr;
    dyna_reg_cfg->force_long_hig_thr = fswdr->force_long_hig_thr;

    dyna_reg_cfg->short_check_thd   = fswdr->short_check_thd;

    if (sns_dft->wdr_switch_attr.exp_ratio[0] < HI_ISP_WDR_FUSION_RATIO_THR) {
        dyna_reg_cfg->fusion_data_mode = 0x1;
    } else {
        dyna_reg_cfg->fusion_data_mode = 0x0;
    }

    dyna_reg_cfg->resh          = HI_TRUE;
}

static hi_void frame_wdr_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 wdr_mode, i, block_num;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_fs_wdr  *fswdr   = HI_NULL;

    fs_wdr_get_ctx(vi_pipe, fswdr);
    isp_get_ctx(vi_pipe, isp_ctx);

    wdr_mode   = isp_ctx->sns_wdr_mode;
    block_num  = isp_ctx->block_attr.block_num;

    for (i = 0; i < block_num; i++) {
        frame_wdr_static_regs_initialize(vi_pipe, wdr_mode, &reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.static_reg_cfg,
                                         isp_ctx);
        frame_wdr_usr_regs_initialize(&reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.usr_reg_cfg, fswdr);
        frame_wdrs_dyna_regs_initialize(vi_pipe, wdr_mode, &reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.dyna_reg_cfg, fswdr);
        frame_wdr_sync_regs_initialize(&reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.sync_reg_cfg, fswdr);
        reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.wdr_en = fswdr->wdr_en;
    }

    reg_cfg->cfg_key.bit1_fs_wdr_cfg = 1;
}

static hi_s32 frame_wdr_read_ext_regs(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_fs_wdr *fs_wdr_ctx = HI_NULL;

    fs_wdr_get_ctx(vi_pipe, fs_wdr_ctx);

    fs_wdr_ctx->coef_update_en  =   hi_ext_system_wdr_coef_update_en_read(vi_pipe);
    hi_ext_system_wdr_coef_update_en_write(vi_pipe, HI_FALSE);

    if (fs_wdr_ctx->coef_update_en) {
        fs_wdr_ctx->fusion_mode        = hi_ext_system_fusion_mode_read(vi_pipe);
        fs_wdr_ctx->mdt_en             = hi_ext_system_mdt_en_read(vi_pipe);
        fs_wdr_ctx->wdr_en             = hi_ext_system_wdr_en_read(vi_pipe);
        fs_wdr_ctx->short_expo_chk      = hi_ext_system_wdr_shortexpo_chk_read(vi_pipe);
        fs_wdr_ctx->md_ref_flicker      = hi_ext_system_wdr_mdref_flicker_read(vi_pipe);
        fs_wdr_ctx->bnr_mode          = hi_ext_system_bnr_mode_read(vi_pipe);
        fs_wdr_ctx->long_thr         = hi_ext_system_wdr_longthr_read(vi_pipe);
        fs_wdr_ctx->short_thr        = hi_ext_system_wdr_shortthr_read(vi_pipe);
        fs_wdr_ctx->noise_model_coef   = hi_ext_system_wdr_noise_model_coef_read(vi_pipe);
        fs_wdr_ctx->fusion_barrier0  = hi_ext_system_fusion_thr_read(vi_pipe, HI_ISP_WDR_MERGE_FRAME_VS);
        fs_wdr_ctx->fusion_barrier1  = hi_ext_system_fusion_thr_read(vi_pipe, HI_ISP_WDR_MERGE_FRAME_S);
        fs_wdr_ctx->fusion_barrier2  = hi_ext_system_fusion_thr_read(vi_pipe, HI_ISP_WDR_MERGE_FRAME_M);
        fs_wdr_ctx->fusion_barrier3  = hi_ext_system_fusion_thr_read(vi_pipe, HI_ISP_WDR_MERGE_FRAME_L);
        fs_wdr_ctx->manual_mode        = hi_ext_system_wdr_manual_mode_read(vi_pipe);
        fs_wdr_ctx->md_thr_low_gain     = hi_ext_system_wdr_manual_mdthr_low_gain_read(vi_pipe);
        fs_wdr_ctx->md_thr_hig_gain     = hi_ext_system_wdr_manual_mdthr_hig_gain_read(vi_pipe);
        fs_wdr_ctx->noise_model_coef   = hi_ext_system_wdr_noise_model_coef_read(vi_pipe);
        fs_wdr_ctx->texture_thd_wgt    = hi_ext_system_wdr_texture_thd_wgt_read(vi_pipe);

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            fs_wdr_ctx->md_thr_low_gain_lut[i] = hi_ext_system_wdr_auto_mdthr_low_gain_read(vi_pipe, i);
            fs_wdr_ctx->md_thr_hig_gain_lut[i] = hi_ext_system_wdr_auto_mdthr_hig_gain_read(vi_pipe, i);
        }

        fs_wdr_ctx->force_long         = hi_ext_system_wdr_forcelong_en_read(vi_pipe);
        fs_wdr_ctx->force_long_hig_thr = hi_ext_system_wdr_forcelong_high_thd_read(vi_pipe);
        fs_wdr_ctx->force_long_low_thr = hi_ext_system_wdr_forcelong_low_thd_read(vi_pipe);
        fs_wdr_ctx->short_check_thd   = hi_ext_system_wdr_shortcheck_thd_read(vi_pipe);

        fs_wdr_ctx->mdt_still_thr      = hi_ext_system_wdr_mdt_still_thr_read(vi_pipe);
        fs_wdr_ctx->mdt_l_bld          = hi_ext_system_wdr_mdt_long_blend_read(vi_pipe);
    }

    return HI_SUCCESS;
}

static hi_s32 wdr_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_wdr *cmos_wdr)
{
    hi_u8 j;

    isp_check_bool_return(cmos_wdr->fusion_mode);
    isp_check_bool_return(cmos_wdr->motion_comp);
    isp_check_bool_return(cmos_wdr->short_expo_chk);
    isp_check_bool_return(cmos_wdr->md_ref_flicker);

    if (cmos_wdr->short_thr > 0xFFF) {
        isp_err_trace("Invalid u16ShortThr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (cmos_wdr->long_thr > 0xFFF) {
        isp_err_trace("Invalid u16LongThr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (cmos_wdr->long_thr > cmos_wdr->short_thr) {
        isp_err_trace("u16LongThresh should NOT be larger than u16ShortThresh!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) {
        if (cmos_wdr->md_thr_low_gain[j] > cmos_wdr->md_thr_hig_gain[j]) {
            isp_err_trace("au8MdThrLowGain[%d] should NOT be larger than au8MdThrHigGain[%d]\n", j, j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (cmos_wdr->bnr_mode >= BNR_BUTT) {
        isp_err_trace("Invalid enBnrMode!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (j = 0; j < WDR_MAX_FRAME; j++) {
        if (cmos_wdr->fusion_thr[j] > 0x3FFF) {
            isp_err_trace("Invalid au16FusionThr!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 frame_wdr_initialize(hi_vi_pipe vi_pipe, hi_isp_cmos_default *sns_dft)
{
    hi_u8  i;
    hi_u8  wdr_mode;
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_fs_wdr  *fswdr   = HI_NULL;
    fs_wdr_get_ctx(vi_pipe, fswdr);
    isp_get_ctx(vi_pipe, isp_ctx);

    wdr_mode   = isp_ctx->sns_wdr_mode;

    if (is_linear_mode(wdr_mode) || is_built_in_wdr_mode(wdr_mode)) {
        fswdr->wdr_en = HI_FALSE;
    } else {
        fswdr->wdr_en = HI_TRUE;
    }

    fswdr->bit_depth_prc   = HI_WDR_BITDEPTH;
    fswdr->pre_iso129      = 0;
    fswdr->pre_again       = 0;
    fswdr->manual_mode     = OP_TYPE_AUTO;
    fswdr->md_thr_low_gain = HI_ISP_WDR_MDTHR_LOW_GAIN_DEFAULT;
    fswdr->md_thr_hig_gain = HI_ISP_WDR_MDTHR_HIG_GAIN_DEFAULT;

    for (i = 0; i < NOISESET_ELENUM; i++) {
        fswdr->floor_set_lut[i] = g_noise_floor_set[i];
        fswdr->again_set_lut[i] = g_noise_again_set[i];
    }

    fswdr->erosion_en       = HI_EXT_SYSTEM_EROSION_EN_DEFAULT;
    fswdr->mdt_full_thr     = HI_EXT_SYSTEM_WDR_MDT_FULL_THR_DEFAULT;
    fswdr->nos_c_wgt_mod    = HI_EXT_SYSTEM_WDR_NOISE_C_WEIGHT_MODE_DEFAULT;
    fswdr->nos_g_wgt_mod    = HI_EXT_SYSTEM_WDR_NOISE_G_WEIGHT_MODE_DEFAULT;
    fswdr->noise_ratio_rg   = HI_EXT_SYSTEM_WDR_NOISE_RATIO_RG_DEFAULT;
    fswdr->noise_ratio_bg   = HI_EXT_SYSTEM_WDR_NOISE_RATIO_BG_DEFAULT;
    fswdr->noise_model_coef = HI_EXT_SYSTEM_WDR_NOISE_MODEL_COEF_DEFAULT;

    if (sns_dft->key.bit1_wdr) {
        isp_check_pointer_return(sns_dft->wdr);

        ret = wdr_check_cmos_param(vi_pipe, sns_dft->wdr);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        fswdr->fusion_mode = sns_dft->wdr->fusion_mode;
        fswdr->mdt_en      = sns_dft->wdr->motion_comp;
        fswdr->short_thr   = sns_dft->wdr->short_thr;
        fswdr->long_thr    = sns_dft->wdr->long_thr;
        fswdr->short_expo_chk = sns_dft->wdr->short_expo_chk;
        fswdr->md_ref_flicker = sns_dft->wdr->md_ref_flicker;

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            fswdr->md_thr_low_gain_lut[i] = sns_dft->wdr->md_thr_low_gain[i];
            fswdr->md_thr_hig_gain_lut[i] = sns_dft->wdr->md_thr_hig_gain[i];
        }

        fswdr->bnr_mode        = sns_dft->wdr->bnr_mode;
        fswdr->fusion_barrier0 = sns_dft->wdr->fusion_thr[0];
        fswdr->fusion_barrier1 = sns_dft->wdr->fusion_thr[1];

        fswdr->mdt_still_thr    = sns_dft->wdr->mdt_still_thd;
        fswdr->mdt_l_bld        = sns_dft->wdr->mdt_long_blend;
        fswdr->short_check_thd  = sns_dft->wdr->short_check_thd;

        fswdr->force_long         = sns_dft->wdr->force_long;
        fswdr->force_long_low_thr = sns_dft->wdr->force_long_low_thr;
        fswdr->force_long_hig_thr = sns_dft->wdr->force_long_hig_thr;
    } else {
        fswdr->fusion_mode        = HI_EXT_SYSTEM_FUSION_MODE_DEFAULT;
        fswdr->mdt_en             = HI_EXT_SYSTEM_MDT_EN_DEFAULT;
        fswdr->short_thr          = HI_EXT_SYSTEM_WDR_SHORTTHR_WRITE_DEFAULT;
        fswdr->long_thr           = HI_EXT_SYSTEM_WDR_LONGTHR_WRITE_DEFAULT;
        fswdr->short_expo_chk     = HI_ISP_WDR_SHORT_EXPO_CHK_DEFAULT;
        fswdr->md_ref_flicker     = HI_EXT_SYSTEM_WDR_MDREF_FLICKER_DEFAULT;
        fswdr->force_long         = HI_EXT_SYSTEM_WDR_FORCELONG_EN;
        fswdr->force_long_low_thr = HI_EXT_SYSTEM_WDR_FORCELONG_LOW_THD;
        fswdr->force_long_hig_thr = HI_EXT_SYSTEM_WDR_FORCELONG_HIGH_THD;

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            fswdr->md_thr_low_gain_lut[i] = g_lut_mdt_low_thr[i];
            fswdr->md_thr_hig_gain_lut[i] = g_lut_mdt_hig_thr[i];
        }

        fswdr->bnr_mode        = HI_EXT_SYSTEM_BNR_MODE_WRITE_DEFAULT;
        fswdr->fusion_barrier0 = g_fusion_thr[0];
        fswdr->fusion_barrier1 = g_fusion_thr[1];
        fswdr->mdt_l_bld       = HI_EXT_SYSTEM_WDR_MDT_LONG_BLEND_DEFAULT;
        fswdr->mdt_still_thr   = HI_EXT_SYSTEM_WDR_MDT_STILL_THR_DEFAULT;
        fswdr->short_check_thd = HI_EXT_SYSTEM_WDR_SHORTCHECK_THD;
    }

    return HI_SUCCESS;
}

hi_s32 isp_frame_wdr_init(hi_vi_pipe vi_pipe, hi_void *cfg)
{
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    ret = frame_wdr_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_sensor_get_default(vi_pipe, &sns_dft);

    ret = frame_wdr_initialize(vi_pipe, sns_dft);
    if (ret != HI_SUCCESS) {
        return ret;
    }
    frame_wdr_ext_regs_initialize(vi_pipe);
    frame_wdr_regs_initialize(vi_pipe, reg_cfg);

    return HI_SUCCESS;
}

static hi_s32 isp_frame_wdr_switch_mode(hi_vi_pipe vi_pipe, hi_void *cfg)
{
    hi_u8  i;
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;

    ret = isp_frame_wdr_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.static_reg_cfg.first_frame = HI_TRUE;
    }
    return HI_SUCCESS;
}

static hi_s32 isp_frame_res_switch_mode(hi_vi_pipe vi_pipe, hi_void *cfg)
{
    hi_u8 i;
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_linear_mode(isp_ctx->sns_wdr_mode) || is_built_in_wdr_mode(isp_ctx->sns_wdr_mode)) {
        return HI_SUCCESS;
    }

    ret = isp_frame_wdr_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.static_reg_cfg.first_frame = HI_TRUE;
    }
    return HI_SUCCESS;
}

static hi_bool __inline check_wdr_open(isp_fs_wdr *fs_wdr)
{
    return (fs_wdr->wdr_en == HI_TRUE);
}

hi_s32 check_wdr_mode(hi_vi_pipe vi_pipe, isp_fs_wdr *fs_wdr)
{
    hi_u8  wdr_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    if (is_linear_mode(wdr_mode) || is_built_in_wdr_mode(wdr_mode)) {
        fs_wdr->wdr_en = HI_FALSE;
    }

    return HI_SUCCESS;
}

static hi_u8 get_value_from_lut(hi_s32 x, const hi_u32 *lut_x, hi_u8 *lut_y, hi_s32 length)
{
    hi_s32 n = 0;
    hi_s32 y0, y1, x0, x1;

    if (x <= lut_x[0]) {
        return lut_y[0];
    }

    for (n = 1; n < length; n++) {
        if (x <= lut_x[n]) {
            y0 = lut_y[n - 1];
            y1 = lut_y[n];
            x0 = lut_x[n - 1];
            x1 = lut_x[n];

            return (hi_u8)((y1 - y0) * (x - x0) / div_0_to_1(x1 - x0) + y0);
        }
    }

    return lut_y[length - 1];
}

hi_s32 wdr_fw_blend(hi_s64 v, hi_s64 x0, hi_s64 x1, hi_s64 y0, hi_s64 y1)
{
    hi_s32 y;
    hi_u32 bitshift = 8;
    hi_s32 bitshift_value;
    hi_u32 temp;
    hi_u32 temp1;

    bitshift_value = (1 << (bitshift - 1));

    if (v <= x0) {
        return (hi_s32)y0;
    }

    if (v >= x1) {
        return (hi_s32)y1;
    }

    temp1 = ((y1 - y0) * (v - x0));

    temp = (((temp1 << bitshift) / (x1 - x0)) + bitshift_value);

    y = y0 + (temp >> bitshift);

    return (hi_s32)y;
}

static hi_void wdr_nr_param_cal(isp_fs_wdr *fs_wdr, isp_usr_ctx *isp_ctx, isp_fswdr_dyna_cfg *wdr_reg)
{
    hi_u32 m_f_sensor_again = ((isp_ctx->linkage.again << HI_ISP_WDR_SENSOR_GAIN_SHIFT_BITS) +
                               HI_ISP_WDR_SENSOR_GAIN_ROUND) / HI_ISP_WDR_SENSOR_GAIN_BASE;
    hi_u32 m_f_sensor_dgain = ((isp_ctx->linkage.dgain << HI_ISP_WDR_SENSOR_GAIN_SHIFT_BITS) +
           HI_ISP_WDR_SENSOR_GAIN_ROUND) / HI_ISP_WDR_SENSOR_GAIN_BASE + (1 << HI_ISP_WDR_SENSOR_GAIN_SHIFT_BITS);
    hi_s32 m_again_g;
    hi_s32 m_sqrt_again_g, m_sqrt_dgain_g;
    hi_u32 m_noise_floor = 0;
    hi_u32 f_gnoisefloor, f_rnoisefloor, f_bnoisefloor;
    hi_u32 m_noise_ratio_rg_wgt = HI_WDR_RG_WGT; /* 1.5 */
    hi_u32 m_noise_ratio_bg_wgt = HI_WDR_BG_WGT; /* 1.5 */
    hi_u16 nos_floor_g;
    hi_u8  sensor_sqrt_gain_shift = 8;
    hi_u8  sensor_again_shift = 16;
    hi_u8  noise_gain_base = 64;
    hi_u8  sqrt_gain_max = 6;
    hi_u8  ratio_sqrt_bits = 11;
    hi_u8  mdt_nos_floor_bitdep = 7;
    hi_u8  wdr_sqrt_out_bits = 8;
    hi_u8  wdr_sqrt_round = 32;
    hi_u8  noise_floor_g_shift = 16;
    hi_u8  noise_floor_fix_value = 90;
    hi_u8  noise_floor_shift_factor1 = 6;
    hi_u8  noise_floor_shift_factor2 = 7;
    hi_u8  noise_floor_round_value1 = 32;
    hi_u8  noise_floor_round_value2 = 64;
    hi_u32 i;
    hi_u8  nos_floor_g_biedep = 9;
    hi_u8  t_nos_floor_bitdep = 12;
    hi_u32 ratio = isp_ctx->linkage.exp_ratio;

    ratio = MIN2(MAX2(ratio, HI_WDR_EXPOSURE_BASE), HI_ISP_WDR_RATIO_MAX);
    m_again_g = (hi_s32)(m_f_sensor_again * noise_gain_base) >> sensor_again_shift;
    m_sqrt_again_g = (hi_s32)(wdr_sqrt(m_f_sensor_again, HI_WDR_SQRT_GAIN_BITDEP)) >> sensor_sqrt_gain_shift;
    m_sqrt_dgain_g = (hi_s32)(wdr_sqrt(m_f_sensor_dgain, HI_WDR_SQRT_GAIN_BITDEP)) >> sensor_sqrt_gain_shift;

    fs_wdr->pre_again = isp_ctx->linkage.again;

    for (i = 0; i < NOISESET_ELENUM; i++) {
        fs_wdr->again_set_lut[i] = g_noise_again_set[i] * noise_gain_base;
    }

    /* noise floor interpolation */
    for (i = 0; i < (NOISESET_ELENUM - 1); i++) {
        if (m_again_g >= fs_wdr->again_set_lut[i] && m_again_g <= fs_wdr->again_set_lut[i + 1]) {
            m_noise_floor = fs_wdr->floor_set_lut[i] + \
            ((fs_wdr->floor_set_lut[i + 1] - fs_wdr->floor_set_lut[i]) * (m_again_g - fs_wdr->again_set_lut[i]))\
             / div_0_to_1(fs_wdr->again_set_lut[i + 1] - fs_wdr->again_set_lut[i]);
        }
    }

    f_gnoisefloor =  m_noise_floor;
    f_rnoisefloor = ((((m_noise_floor * noise_floor_fix_value * m_noise_ratio_rg_wgt + 1) >> 1) + \
     noise_floor_round_value1) \
    >> noise_floor_shift_factor1) + \
    ((((m_noise_floor * m_noise_ratio_rg_wgt + 1) >> 1) + noise_floor_round_value2) >> noise_floor_shift_factor2);
    f_bnoisefloor = ((((m_noise_floor * noise_floor_fix_value * m_noise_ratio_bg_wgt + 1) >> 1) + \
    noise_floor_round_value1) \
    >> noise_floor_shift_factor1) + \
    ((((m_noise_floor * m_noise_ratio_bg_wgt + 1) >> 1) + noise_floor_round_value2) >> noise_floor_shift_factor2);

    wdr_reg->t_nos_floor    = MIN2(isp_bitmask(t_nos_floor_bitdep), \
        ((f_gnoisefloor + f_rnoisefloor + f_bnoisefloor) * \
        wdr_sqrt(m_f_sensor_dgain, HI_WDR_SQRT_GAIN_BITDEP) + wdr_sqrt_round) >> wdr_sqrt_out_bits);

    nos_floor_g             = MIN2(isp_bitmask(nos_floor_g_biedep), \
        (hi_s32)(m_noise_floor * m_f_sensor_dgain + (1 << (noise_floor_g_shift - 1))) >> noise_floor_g_shift);

    wdr_reg->mdt_nos_floor = MIN2(isp_bitmask(mdt_nos_floor_bitdep), \
        nos_floor_g * wdr_sqrt(wdr_sqrt(ratio, ratio_sqrt_bits), ratio_sqrt_bits));

    wdr_reg->sqrt_again_g  = MIN2(sqrt_gain_max, m_sqrt_again_g);
    wdr_reg->sqrt_dgain_g  = MIN2(sqrt_gain_max, m_sqrt_dgain_g);
}

static hi_void hiisp_wdr_func(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx,
                              isp_fs_wdr *fs_wdr, isp_fswdr_dyna_cfg *wdr_reg, isp_fswdr_static_cfg *static_reg_cfg)
{
    hi_s32 blc_value;
    hi_u32 i;
    hi_u32 awb_r_gain = isp_ctx->linkage.white_balance_gain[HI_WDR_AWB_RGAIN_INDEX] >> HI_ISP_WDR_AWB_FRACTION_BITS;
    hi_u32 awb_g_gain = isp_ctx->linkage.white_balance_gain[HI_WDR_AWB_GGAIN_INDEX] >> HI_ISP_WDR_AWB_FRACTION_BITS;
    hi_u32 awb_gain   = isp_ctx->linkage.white_balance_gain[HI_WDR_AWB_BGAIN_INDEX] >> HI_ISP_WDR_AWB_FRACTION_BITS;
    hi_u32 still_exp_s_low, still_exp_s_hig;
    hi_s32 m_max_value_in  = isp_bitmask(fs_wdr->bit_depth_prc);
    hi_u32 ratio = isp_ctx->linkage.exp_ratio;
    hi_u32 temp  = 0;
    hi_u8  still_exp_shift = 6;
    hi_isp_cmos_default     *sns_dft         = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    ratio = MIN2(MAX2(ratio, HI_WDR_EXPOSURE_BASE), HI_ISP_WDR_RATIO_MAX);
    blc_value = sns_black_level->black_level[0];

    wdr_reg->wdr_mdt_en     = fs_wdr->mdt_en;
    wdr_reg->long_thr       = fs_wdr->long_thr;
    wdr_reg->short_thr      = fs_wdr->short_thr;

    wdr_reg->texture_thd_wgt = fs_wdr->texture_thd_wgt;

    if (ratio < HI_ISP_WDR_FUSION_RATIO_THR) {
        wdr_reg->fusion_data_mode = 0x1;
    } else {
        wdr_reg->fusion_data_mode = 0x0;
    }

    if (awb_r_gain != 0 && awb_gain != 0 && awb_g_gain != 0) {
        temp = (fs_wdr->fusion_barrier0 << HI_FUSION_THR_SHIFT_BIT) / div_0_to_1(awb_r_gain);
        wdr_reg->fusion_thr_r_lut[0] = MIN2(isp_bitmask(HI_FUSION_THR_BIT), temp);

        temp = (fs_wdr->fusion_barrier1 << HI_FUSION_THR_SHIFT_BIT) / div_0_to_1(awb_r_gain);
        wdr_reg->fusion_thr_r_lut[1] = MIN2(isp_bitmask(HI_FUSION_THR_BIT), temp);

        temp = (fs_wdr->fusion_barrier0 << HI_FUSION_THR_SHIFT_BIT) / div_0_to_1(awb_g_gain);
        wdr_reg->fusion_thr_g_lut[0] = MIN2(isp_bitmask(HI_FUSION_THR_BIT), temp);

        temp = (fs_wdr->fusion_barrier1 << HI_FUSION_THR_SHIFT_BIT) / div_0_to_1(awb_g_gain);
        wdr_reg->fusion_thr_g_lut[1] = MIN2(isp_bitmask(HI_FUSION_THR_BIT), temp);

        temp = (fs_wdr->fusion_barrier0 << HI_FUSION_THR_SHIFT_BIT) / div_0_to_1(awb_gain);
        wdr_reg->fusion_thr_b_lut[0] = MIN2(isp_bitmask(HI_FUSION_THR_BIT), temp);

        temp = (fs_wdr->fusion_barrier1 << HI_FUSION_THR_SHIFT_BIT) / div_0_to_1(awb_gain);
        wdr_reg->fusion_thr_b_lut[1] = MIN2(isp_bitmask(HI_FUSION_THR_BIT), temp);
    }

    wdr_nr_param_cal(fs_wdr, isp_ctx, wdr_reg);

    if (fs_wdr->manual_mode) {
        wdr_reg->md_thr_low_gain = fs_wdr->md_thr_low_gain;
        wdr_reg->md_thr_hig_gain = fs_wdr->md_thr_hig_gain;
    } else {
        wdr_reg->md_thr_low_gain =  get_value_from_lut(isp_ctx->linkage.iso, get_iso_lut(), \
            fs_wdr->md_thr_low_gain_lut, ISP_AUTO_ISO_STRENGTH_NUM);
        wdr_reg->md_thr_hig_gain =  get_value_from_lut(isp_ctx->linkage.iso, get_iso_lut(), \
            fs_wdr->md_thr_hig_gain_lut, ISP_AUTO_ISO_STRENGTH_NUM);
    }

    fs_wdr->actual_md_thr_low_gain = wdr_reg->md_thr_low_gain;
    fs_wdr->actual_md_thr_hig_gain = wdr_reg->md_thr_hig_gain;

    wdr_reg->erosion_en = fs_wdr->erosion_en;

    if ((hi_ext_system_flicker_result_read(vi_pipe) == HI_TRUE) && (fs_wdr->md_ref_flicker == HI_TRUE)) {
        wdr_reg->md_thr_low_gain = HI_WDR_MD_LOW_THR;
        wdr_reg->md_thr_hig_gain = HI_WDR_MD_HIG_THR;
    }

    still_exp_s_hig = ((hi_u32)(m_max_value_in - blc_value)) << still_exp_shift;
    still_exp_s_low = ((hi_u32)wdr_sqrt((m_max_value_in - blc_value), HI_ISP_WDR_SQRT_OUT_BITS));

    for (i = 0; i < MIN2(WDR_CHL_MAX - 1, wdr_reg->frm_merge - 1); i++) {
        wdr_reg->still_thr_lut[i] = wdr_clip3(0, isp_bitmask(HI_WDR_THR_BIT), \
        ((hi_s32)(still_exp_s_hig / div_0_to_1(isp_ctx->linkage.exp_ratio_lut[i])) - (hi_s32)still_exp_s_low));
    }

    wdr_reg->force_long         = fs_wdr->force_long;
    wdr_reg->force_long_low_thr = MIN2(isp_bitmask(HI_WDR_THR_BIT), fs_wdr->force_long_low_thr);
    wdr_reg->force_long_hig_thr = MIN2(isp_bitmask(HI_WDR_THR_BIT), fs_wdr->force_long_hig_thr);
    wdr_reg->short_check_thd   = MIN2(isp_bitmask(HI_WDR_THR_BIT), fs_wdr->short_check_thd);

    wdr_reg->resh = HI_TRUE;
}

static hi_void hiisp_wdr_sync_fw(isp_fs_wdr *fswdr, isp_fswdr_sync_cfg *sync_reg_cfg)
{
    sync_reg_cfg->fusion_mode = fswdr->fusion_mode;
    sync_reg_cfg->wdr_mdt_en  = fswdr->mdt_en;
    sync_reg_cfg->short_thr = fswdr->short_thr;
    sync_reg_cfg->long_thr  = fswdr->long_thr;
}

static hi_void hiisp_wdr_usr_fw(isp_fs_wdr *fswdr, isp_fswdr_usr_cfg *usr_reg_cfg)
{
    usr_reg_cfg->fusion_mode     = fswdr->fusion_mode;
    usr_reg_cfg->short_expo_chk  = fswdr->short_expo_chk;
    usr_reg_cfg->mdt_full_thr    = fswdr->mdt_full_thr;
    usr_reg_cfg->mdt_l_bld       = fswdr->mdt_l_bld;
    usr_reg_cfg->mdt_still_thr   = fswdr->mdt_still_thr;
    usr_reg_cfg->resh            = HI_TRUE;
    usr_reg_cfg->update_index += 1;
}

static hi_void  wdr_set_long_frame_mode(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_fswdr_dyna_cfg *dyna_reg_cfg = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        dyna_reg_cfg = &reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.dyna_reg_cfg;
        if ((isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
            (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
            dyna_reg_cfg->bcom_en = HI_FALSE;
            dyna_reg_cfg->bdec_en = HI_FALSE;
        } else {
            dyna_reg_cfg->bcom_en = HI_TRUE;
            dyna_reg_cfg->bdec_en = HI_TRUE;
        }
    }
}

hi_s32 isp_frame_wdr_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                         hi_void *cfg, hi_s32 rsv)
{
    hi_u8 i = 0;
    isp_usr_ctx     *isp_ctx = HI_NULL;
    isp_fs_wdr  *fswdr  = HI_NULL;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;
    hi_s32 ret;

    isp_get_ctx(vi_pipe, isp_ctx);
    fs_wdr_get_ctx(vi_pipe, fswdr);

    isp_check_pointer_return(fswdr);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    fswdr->wdr_en  = hi_ext_system_wdr_en_read(vi_pipe);

    ret = check_wdr_mode(vi_pipe, fswdr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.wdr_en = fswdr->wdr_en;
    }

    reg_cfg->cfg_key.bit1_fs_wdr_cfg = 1;

    /* check hardware setting */
    ret = check_wdr_open(fswdr);
    if (!ret) {
        return HI_SUCCESS;
    }

    ret = frame_wdr_read_ext_regs(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    if (isp_ctx->linkage.fswdr_mode != isp_ctx->linkage.pre_fswdr_mode) {
        wdr_set_long_frame_mode(vi_pipe, reg_cfg);
    }

    if (fswdr->coef_update_en) {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            hiisp_wdr_usr_fw(fswdr, &reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.usr_reg_cfg);
        }
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        hiisp_wdr_sync_fw(fswdr, &reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.sync_reg_cfg);
    }

    hiisp_wdr_func(vi_pipe, isp_ctx, fswdr, &reg_cfg->alg_reg_cfg[0].wdr_reg_cfg.dyna_reg_cfg, \
        &reg_cfg->alg_reg_cfg[0].wdr_reg_cfg.static_reg_cfg);

    for (i = 1; i < reg_cfg->cfg_num; i++) {
        memcpy_s(&reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.dyna_reg_cfg, sizeof(isp_fswdr_dyna_cfg),
                 &reg_cfg->alg_reg_cfg[0].wdr_reg_cfg.dyna_reg_cfg, sizeof(isp_fswdr_dyna_cfg));
    }

    return HI_SUCCESS;
}

static hi_s32 frame_wdr_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_fs_wdr *fswdr = HI_NULL;

    fs_wdr_get_ctx(vi_pipe, fswdr);
    isp_check_pointer_return(fswdr);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----FrameWDR INFO------------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%10s" "%10s" "%10s" "%16s" "%16s\n",
                    "MdtEn", "LongThr", "ShortThr", "MdThrLowGain", "MdThrHighGain");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%10d" "%10d"  "%10d" "%16d" "%16d\n",
                    fswdr->mdt_en, fswdr->long_thr, fswdr->short_thr,
                    fswdr->actual_md_thr_low_gain, fswdr->actual_md_thr_hig_gain);

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_frame_wdr_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_frame_wdr_switch_mode(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;

        case ISP_CHANGE_IMAGE_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_frame_res_switch_mode(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;

        case ISP_PROC_WRITE:
            frame_wdr_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_frame_wdr_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].wdr_reg_cfg.dyna_reg_cfg.bcom_en = HI_FALSE;
        reg_cfg->reg_cfg.alg_reg_cfg[i].wdr_reg_cfg.dyna_reg_cfg.bdec_en = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_fs_wdr_cfg = 1;

    frame_wdr_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_frame_wdr(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_wdr);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_FrameWDR;
    algs->alg_func.pfn_alg_init = isp_frame_wdr_init;
    algs->alg_func.pfn_alg_run  = isp_frame_wdr_run;
    algs->alg_func.pfn_alg_ctrl = isp_frame_wdr_ctrl;
    algs->alg_func.pfn_alg_exit = isp_frame_wdr_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
