/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_LUT_CONFIG_H
#define ISP_LUT_CONFIG_H

#include "ot_debug.h"
#include "ot_isp_debug.h"
#include "isp_vreg.h"
#include "isp_main.h"
#include "isp_regcfg.h"
#include "isp_lut_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* be apb lut */
static __inline td_void isp_ae_weight_write(isp_be_lut_apb_type *be_apb_lut,
    const td_u32 *upisp_ae_weight, td_u32 ae_weight_len)
{
    td_u32 i;

    for (i = 0; i < ae_weight_len; i++) {
        be_apb_lut->isp_ae_weight[i].u32 = upisp_ae_weight[i];
    }
}

static __inline td_void isp_ae_weight_read(isp_be_lut_apb_type *be_apb_lut,
    td_u32 *upisp_ae_weight, td_u32 ae_weight_len)
{
    td_u32 i;

    for (i = 0; i < ae_weight_len; i++) {
        upisp_ae_weight[i] = be_apb_lut->isp_ae_weight[i].u32;
    }
}

/* upisp_adgamma_lut  17 bits */
static __inline td_void isp_adgamma_lut_write(isp_be_lut_apb_type *be_apb_lut,
    const td_u32 *upisp_adgamma_lut)
{
    td_s32 i;

    for (i = 0; i < ISP_AGAMMA_NODE_NUM; i++) {
        be_apb_lut->isp_expander_lut[i].u32 = upisp_adgamma_lut[i] & 0x1FFFF; /* 17bitw */
    }
}

/* upisp_expander_lut  16 bits */
static __inline td_void isp_expander_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u32 *upisp_expander_lut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_EXPANDER_REG_NODE_NUM; i++) {
        be_apb_lut->isp_expander_lut[i].u32 = upisp_expander_lut[i] & 0x3FFFF;
    }
}


/* Input         td_u32 *upisp_adrc_tmlut0_diff  14 bits */
static __inline td_void isp_drc_tmlut0_diff_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *drc_tmlut0_diff)
{
    u_isp_drc_tmlut0 o_isp_drc_tmlut0;
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_TM_NODE_NUM; i++) {
        o_isp_drc_tmlut0.u32 = be_apb_lut->isp_drc_tmlut0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_diff = drc_tmlut0_diff[i];
        be_apb_lut->isp_drc_tmlut0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

/* Input         td_u32 *upisp_adrc_tmlut0_value  16 bits */
static __inline td_void isp_drc_tmlut0_value_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *drc_tmlut0_value)
{
    u_isp_drc_tmlut0 o_isp_drc_tmlut0;
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_TM_NODE_NUM; i++) {
        o_isp_drc_tmlut0.u32 = be_apb_lut->isp_drc_tmlut0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_value = drc_tmlut0_value[i];
        be_apb_lut->isp_drc_tmlut0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

/* Input         td_u32 *upisp_drc_cclut  8 bits */
static __inline td_void isp_drc_cclut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *upisp_drc_cclut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_CC_NODE_NUM; i++) {
        ot_unused(upisp_drc_cclut);
        be_apb_lut->isp_drc_cclut[i].bits.isp_drc_cclut = upisp_drc_cclut[i] & 0x1FF;
    }
}

/* Input         td_u32 *upisp_delut  8 bits */
static __inline td_void isp_drc_delut_write(isp_be_lut_apb_type *be_apb_lut, const td_u8 *upisp_drc_delut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_BCNR_NODE_NUM; i++) {
        be_apb_lut->isp_drc_delut[i].u32 = upisp_drc_delut[i] & 0xFF;
    }
}

/* Input         td_u32 *upisp_coefa  14 bits */
static __inline td_void isp_drc_coefalut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *u_isp_drc_coefalut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_BCNR_STRENGTH_NODE_NUM; i++) {
        be_apb_lut->isp_drc_coefalut[i].u32 = u_isp_drc_coefalut[i] & 0x3FFF;
    }
}

/* Input         td_u32 *uisp_bcom_gamma_lut  13 bits */
static __inline td_void isp_bcom_gamma_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u32 *uisp_bcom_gamma_lut)
{
    td_s32 i;

    for (i = 0; i < BCOM_BDEC_NODE_NUMBER; i++) {
        be_apb_lut->isp_bcom_gamma_lut[i].u32 = uisp_bcom_gamma_lut[i] & 0x1FFF;
    }
}

/* Input         td_u32 *uisp_bdec_gamma_lut  18 bits */
static __inline td_void isp_bdec_gamma_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u32 *uisp_bdec_gamma_lut)
{
    td_s32 i;

    for (i = 0; i < BCOM_BDEC_NODE_NUMBER; i++) {
        be_apb_lut->isp_bdec_gamma_lut[i].u32 = uisp_bdec_gamma_lut[i] & 0x3FFFF;
    }
}

/* be lut2stt */
static __inline td_void isp_mlsc_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *lsc_rgain_wstt,
                                                const td_u16 *lsc_grgain_wstt, const td_u16 *lsc_gbgain_wstt,
                                                const td_u16 *lsc_bgain_wstt)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_LSC_GRID_POINTS; i++) {
        be_lut2stt->isp_mlsc_lut_wstt[4 * i + 0].u32 = lsc_rgain_wstt[i] & 0x3FF;  /* 4 * i: r_gain */
        be_lut2stt->isp_mlsc_lut_wstt[4 * i + 1].u32 = lsc_grgain_wstt[i] & 0x3FF; /* 4 * i + 1: gr_gain */
        be_lut2stt->isp_mlsc_lut_wstt[4 * i + 2].u32 = lsc_bgain_wstt[i] & 0x3FF;  /* 4 * i + 2: b_gain */
        be_lut2stt->isp_mlsc_lut_wstt[4 * i + 3].u32 = lsc_gbgain_wstt[i] & 0x3FF; /* 4 * i + 3: gb_gain */
    }
}

static __inline td_void isp_bnr_noised_lut_wstt_write(isp_be_lut_wstt_type *lut2stt, const td_u16 *u16noise_sd_lut)
{
    td_s32 i;
    for (i = 0; i < 65; i++) { /* [0, 65) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = u16noise_sd_lut[2 * i]; /* 4/2 */
    }

    for (i = 0; i < 64; i++) { /* [0, 64) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = u16noise_sd_lut[2 * i + 1]; /* 4/2 */
    }
}

static __inline td_void isp_bnr_noisedinv_lut_wstt_write(isp_be_lut_wstt_type *lut2stt, const td_u32 *u32noise_inv_lut)
{
    td_s32 i;
    for (i = 0; i < 65; i++) { /* [0, 65) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = u32noise_inv_lut[2 * i]; /* 4/2 */
    }

    for (i = 0; i < 64; i++) { /* [0, 64) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = u32noise_inv_lut[2 * i + 1]; /* 4/2/3 */
    }
}

static __inline td_void isp_bnr_noiseds_lut_wstt_write(isp_be_lut_wstt_type *lut2stt, const td_u16 *u16noise_sd_lut_s)
{
    td_s32 i;
    for (i = 65; i < 130; i++) { /* [65, 130) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = u16noise_sd_lut_s[2 * (i - 65)]; /* 4/2/65 */
    }

    for (i = 65; i < 129; i++) { /* [65, 129) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = u16noise_sd_lut_s[2 * (i - 65) + 1]; /* 4/2/65 */
    }
}

static __inline td_void isp_bnr_noisedinv_mag_lut_wstt_write(isp_be_lut_wstt_type *lut2stt,
    const td_u8 *u8noise_inv_magidx)
{
    td_s32 i;
    for (i = 65; i < 130; i++) { /* [65, 130) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = u8noise_inv_magidx[2 * (i - 65)]; /* 4/2/65 */
    }

    for (i = 65; i < 129; i++) { /* [65, 129) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = u8noise_inv_magidx[2 * (i - 65) + 1]; /* 4/2/3/65 */
    }
}

static __inline td_void isp_bnr_fbratiotable_lut_wstt_write(isp_be_lut_wstt_type *lut2stt,
    const td_u8 *u8fb_ratio_table)
{
    td_s32 i;
    for (i = 130; i < 194; i++) { /* [130, 194) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = u8fb_ratio_table[i - 130]; /* 4/130 */
    }
}

static __inline td_void isp_bnr_fbratio_mot2yuv_table_lut_wstt_write(isp_be_lut_wstt_type *lut2stt,
    const td_u8 *u8fb_ratio_mot2yuv_table)
{
    td_s32 i;
    for (i = 130; i < 194; i++) { /* [130, 194) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = u8fb_ratio_mot2yuv_table[i - 130]; /* 4/130 */
    }
}

static __inline td_void isp_bnr_coring_low_lut_wstt_write(isp_be_lut_wstt_type *lut2stt,
    const td_u16 *coring_low_lut)
{
    td_s32 i;
    for (i = 130; i < 147; i++) { /* [130, 147) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = coring_low_lut[2 * (i - 130)]; /* 4/2/130 */
    }

    for (i = 130; i < 146; i++) { /* [130, 146) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = coring_low_lut[2 * (i - 130) + 1]; /* 4/2/3/130 */
    }
}

/* Input         td_u32 *upisp_gamma_lut_wstt  12 bits */
static __inline td_void isp_gamma_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *gamma_lut_wstt)
{
    td_s32 i;

    for (i = 0; i < GAMMA_REG_NODE_NUM; i++) {
        be_lut2stt->isp_gamma_lut_wstt[i].u32 = gamma_lut_wstt[i] & 0xFFF;
    }
}
/* dehaze_lut_wstt size:1280
   0~383:dehaze prestat lut
        odd index: (prestat_lut >> 16) & 0x3ff
        even index:prestat_lut & 0x3ff
   384~640: dehaze strength lut */
static __inline td_void isp_dehaze_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 blk_num,
                                                  const td_u32 *prestat_wstt, const td_u8 *lut_wstt)
{
    td_s32 i;

    for (i = 0; i < blk_num; i++) {
        be_lut2stt->isp_dehaze_lut_wstt[2 * i].bits.isp_dehaze_lut_wstt = prestat_wstt[i] & 0x3ff; /* addr 2 * i */
        be_lut2stt->isp_dehaze_lut_wstt[2 * i + 1].bits.isp_dehaze_lut_wstt = /* addr 2 * i + 1 */
            (prestat_wstt[i] >> 16) & 0x3ff; /* odd index  (prestat_lut >> 16) & 0x3ff */
    }

    for (i = 2 * blk_num; i < 384; i++) { /* [2 * blk_num, 384) */
        be_lut2stt->isp_dehaze_lut_wstt[i].bits.isp_dehaze_lut_wstt = 0;
    }

    for (i = 384; i < 640; i++) { /* [384, 640) */
        be_lut2stt->isp_dehaze_lut_wstt[i].bits.isp_dehaze_lut_wstt = lut_wstt[i - 384]; /* sub 384 */
    }
}

/* Input         td_u32 *upisp_sharpen_lut_wstt  30 bits */
static __inline td_void isp_sharpen_lut_wstt_write(isp_be_lut_wstt_type *lut2stt, const td_u16 *mf_gain_d,
    const td_u16 *mf_gain_ud, const td_u16 *hf_gain_d, const td_u16 *hf_gain_ud)
{
    td_s32 i;

    for (i = 0; i < 32; i++) {  /* sharpen lut size is 32 */
        lut2stt->isp_sharpen_lut_wstt[i * 4].bits.isp_sharpen_lut_wstt = /* addr:4 * i */
            mf_gain_d[2 * i] + (hf_gain_d[2 * i] << 12); /* index 2 2, left shift 12 bit */

        lut2stt->isp_sharpen_lut_wstt[i * 4 + 1].bits.isp_sharpen_lut_wstt = /* addr:4 * i + 1 */
            mf_gain_ud[2 * i] + (hf_gain_ud[2 * i] << 12); /* index 2 2, left shift 12 bit */

        lut2stt->isp_sharpen_lut_wstt[i * 4 + 2].bits.isp_sharpen_lut_wstt = /* addr: 4 * i + 2 */
            mf_gain_d[2 * i + 1] + (hf_gain_d[2 * i + 1] << 12); /* index 2 2, left shift 12 bit */

        lut2stt->isp_sharpen_lut_wstt[i * 4 + 3].bits.isp_sharpen_lut_wstt = /* addr:4 * i + 3 */
            mf_gain_ud[2 * i + 1] + (hf_gain_ud[2 * i + 1] << 12); /* index 2 2, left shift 12 bit */
    }
}

/* ldci lut_wstt size: 65 * 4; order:
   4 * i:                 (cgain_lut << 20)
   4 * i + 1(for i < 33): (heneg_lut << 18) + (hepos_lut << 9)
   4 * i + 2(for i < 33): he_adj_lut
   4 * i + 3            : 0 */
static __inline td_void isp_ldci_drc_cgain_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt,
    const td_u32 *cgain_lut)
{
    td_s32 i;

    for (i = 0; i < 65; i++) { /* [0, 65) */
        be_lut2stt->isp_ldci_lut_wstt[4 * i].u32 = (cgain_lut[i] << 20); /* addr:4 * i, shift 20 bit */
        be_lut2stt->isp_ldci_lut_wstt[4 * i + 3].u32 = 0;  /* addr:4 * i + 3 */
    }
}

static __inline td_void isp_ldci_he_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt,
    const td_u32 *hepos_lut_wstt, const td_u32 *heneg_lut_wstt)
{
    td_s32 i;

    for (i = 0; i < 33; i++) { /* [0, 33) */
        be_lut2stt->isp_ldci_lut_wstt[4 * i + 1].u32 =   /* addr:4 * i + 1 */
            (heneg_lut_wstt[i] << 18) + (hepos_lut_wstt[i] << 9); /* 18~26:heneg, 9~17:hepos */
    }

    for (i = 33; i < 65; i++) { /* [33, 65) */
        be_lut2stt->isp_ldci_lut_wstt[4 * i + 1].u32 = 0;  /* addr:4 * i + 1 */
    }
}

static __inline td_void isp_ldci_usm_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt,
    const td_u32 *upisp_ldci_usmpos_lut_wstt, const td_u32 *upisp_ldci_usmneg_lut_wstt)
{
    td_s32 i;

    for (i = 0; i < 33; i++) { /* [0, 33) */
        be_lut2stt->isp_ldci_lut_wstt[4 * i + 2].u32 = /* addr:4 * i + 2 */
            (upisp_ldci_usmneg_lut_wstt[i] << 9) + upisp_ldci_usmpos_lut_wstt[i]; /* 9~17:usmneg_lut */
    }

    for (i = 33; i < 65; i++) { /* [33, 65) */
        be_lut2stt->isp_ldci_lut_wstt[4 * i + 2].u32 = 0; /* addr:4 * i + 2 */
    }
}

static __inline td_void isp_ca_y_ratio_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u32 *upisp_ca_lut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        be_lut2stt->isp_ca_lut_wstt[i].u32 = upisp_ca_lut[i] & 0xFFFFFF;
    }
}

static __inline td_void isp_ca_y_sat_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u32 *y_sat_lut)
{
    td_s32 i;

    for (i = 128; i < 256; i++) { /* [128, 256) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = y_sat_lut[i - 128]; /* index: i - 256 */
    }
}

static __inline td_void isp_ca_y_ch1_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *y_ch1_lut)
{
    td_s32 i, idx;

    ot_unused(be_lut2stt);
    ot_unused(y_ch1_lut);
    ot_unused(idx);

    for (i = 256; i < 264; i++) { /* [256, 264) */
        idx = 2 * (i - 256); /* even index: 2 * (i - 512) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = (y_ch1_lut[idx + 1] << 16) + y_ch1_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_ca_y_ch2_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *y_ch2_lut)
{
    td_s32 i, idx;
    ot_unused(be_lut2stt);
    ot_unused(y_ch2_lut);
    ot_unused(idx);
    for (i = 264; i < 272; i++) { /* [264, 272) */
        idx = 2 * (i - 264); /* even index: 2 * (i - 520) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = (y_ch2_lut[idx + 1] << 16) + y_ch2_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_ca_y_slu_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *y_slu_lut)
{
    td_s32 i, idx;
    ot_unused(be_lut2stt);
    ot_unused(y_slu_lut);
    ot_unused(idx);

    for (i = 272; i < 280; i++) { /* [272, 280) */
        idx = 2 * (i - 272); /* even index: 2 * (i - 528) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = (y_slu_lut[idx + 1] << 16) + y_slu_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_ca_y_slv_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *y_slv_lut)
{
    td_s32 i, idx;
    ot_unused(be_lut2stt);
    ot_unused(y_slv_lut);
    ot_unused(idx);

    for (i = 280; i < 288; i++) { /* [280, 288) */
        idx = 2 * (i - 280); /* even index: 2 * (i - 536) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = (y_slv_lut[idx + 1] << 16) + y_slv_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_cc_lut_wstt_write(isp_be_lut_wstt_type *be_lut2stt, const td_u16 *upisp_cc_dgain_lut,
    const td_u16 *upisp_cc_gamma_lut)
{
    td_s32 i;

    for (i = 0; i < 65; i++) {  /* cc lut size is 65 */
        be_lut2stt->isp_cc_lut_wstt[i].u32 = upisp_cc_dgain_lut[i] & 0x3FFF;      /* [0, 64] */
        be_lut2stt->isp_cc_lut_wstt[i + 68].u32 = upisp_cc_gamma_lut[i] & 0x3FFF; /* [68, 132] */
    }
    be_lut2stt->isp_cc_lut_wstt[65].u32 = upisp_cc_dgain_lut[1] & 0x3FFF;      /* lut index 65 1 */
    be_lut2stt->isp_cc_lut_wstt[65 + 68].u32 = upisp_cc_gamma_lut[1] & 0x3FFF; /* lut index 65 68 1 */
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
