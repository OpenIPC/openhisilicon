/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
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

/* fe lut2stt */
static __inline td_void isp_lblc_lut_wstt_write(isp_fe_lut_wstt_type *fe_lut2stt, const td_u16 *upisp_lblc_r,
    const td_u16 *upisp_lblc_gr, const td_u16 *upisp_lblc_gb, const td_u16 *upisp_lblc_b)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_LBLC_GRID_POINTS; i++) {
        fe_lut2stt->isp_lblc_lut_wstt[4 * i + 0].u32 = upisp_lblc_r[i] & 0x3FFF;  /* 4 * i: r_gain */
        fe_lut2stt->isp_lblc_lut_wstt[4 * i + 1].u32 = upisp_lblc_gr[i] & 0x3FFF; /* 4 * i + 1: gr_gain */
        fe_lut2stt->isp_lblc_lut_wstt[4 * i + 2].u32 = upisp_lblc_b[i] & 0x3FFF;  /* 4 * i + 2: b_gain */
        fe_lut2stt->isp_lblc_lut_wstt[4 * i + 3].u32 = upisp_lblc_gb[i] & 0x3FFF; /* 4 * i + 3: gb_gain */
    }
}

/* be apb lut */
static __inline td_void isp_ae_weight_write(isp_be_lut_apb_type *be_apb_lut, const td_u32 *upisp_ae_weight)
{
    td_s32 i;

    for (i = 0; i < 64; i++) { /* ae weight table number 64 */
        be_apb_lut->isp_ae_weight[i].u32 = upisp_ae_weight[i];
    }
}

/* upisp_nddm_gflut  12 bits */
static __inline td_void isp_nddm_gflut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *upisp_nddm_gflut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_DEMOSAIC_LUT_LENGTH; i++) {
        be_apb_lut->isp_nddm_gf_lut[i].u32 = upisp_nddm_gflut[i] & 0xFFF;
    }
}

/* upisp_expander_lut  21 bits */
static __inline td_void isp_expander_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u32 *upisp_expander_lut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_EXPANDER_NODE_NUM; i++) {
        be_apb_lut->isp_expander_lut[i].u32 = upisp_expander_lut[i] & 0x1FFFFF;
    }
}

/* isp_wdr_snoise_lut  16 bits */
static __inline td_void isp_wdr_snoise_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *wdr_snoise_lut)
{
    td_s32 i;

    for (i = 0; i < 257; i++) {  /* wdr_snoise lut size is 257 */
        be_apb_lut->isp_wdr_snoise_lut[i].u32 = wdr_snoise_lut[i];
    }
}

/* isp_wdr_lnoise_lut  16 bits */
static __inline td_void isp_wdr_lnoise_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *wdr_lnoise_lut)
{
    td_s32 i;

    for (i = 0; i < 257; i++) { /* wdr_lnoise lut size is 257 */
        be_apb_lut->isp_wdr_lnoise_lut[i].u32 = wdr_lnoise_lut[i];
    }
}
/* isp_wdr_normalize_lut  16 bits */
static __inline td_void isp_wdr_normalize_lut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *wdr_normalize_lut)
{
    td_s32 i;

    for (i = 0; i < 513; i++) { /* wdr_normalize lut size is 513 */
        be_apb_lut->isp_wdr_normalize_lut[i].u32 = wdr_normalize_lut[i];
    }
}

/* Input         td_u32 *upisp_adrc_tmlut0_diff  14 bits */
static __inline td_void isp_drc_tmlut0_diff_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *drc_tmlut0_diff)
{
    u_isp_adrc_tmlut0 o_isp_drc_tmlut0;
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_TM_NODE_NUM; i++) {
        o_isp_drc_tmlut0.u32 = be_apb_lut->isp_adrc_tmlut0[i].u32;
        o_isp_drc_tmlut0.bits.isp_adrc_tmlut0_diff = drc_tmlut0_diff[i];
        be_apb_lut->isp_adrc_tmlut0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

/* Input         td_u32 *upisp_adrc_tmlut0_value  16 bits */
static __inline td_void isp_drc_tmlut0_value_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *drc_tmlut0_value)
{
    u_isp_adrc_tmlut0 o_isp_drc_tmlut0;
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_TM_NODE_NUM; i++) {
        o_isp_drc_tmlut0.u32 = be_apb_lut->isp_adrc_tmlut0[i].u32;
        o_isp_drc_tmlut0.bits.isp_adrc_tmlut0_value = drc_tmlut0_value[i];
        be_apb_lut->isp_adrc_tmlut0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

/* Input         td_u32 *upisp_drc_cclut  12 bits */
static __inline td_void isp_drc_cclut_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *upisp_drc_cclut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_DRC_CC_NODE_NUM; i++) {
        be_apb_lut->isp_adrc_cclut[i].u32 = upisp_drc_cclut[i] & 0xFFF;
    }
}

static __inline td_void isp_drc_local_mix_write(isp_be_lut_apb_type *be_apb_lut, const td_u8 *localmix_dark0,
    const td_u8 *localmix_bright0, const td_u8 *localmix_dark1, const td_u8 *localmix_bright1)
{
    td_s32 i;
    u_isp_adrc_localmix o_isp_adrc_localmix;

    for (i = 0; i < OT_ISP_DRC_LMIX_NODE_NUM; i++) { /* drc local mix lut size is 33 */
        o_isp_adrc_localmix.u32 = be_apb_lut->isp_adrc_localmix[i].u32;
        o_isp_adrc_localmix.bits.isp_adrc_localmix_dark0   = localmix_dark0[i];
        o_isp_adrc_localmix.bits.isp_adrc_localmix_bright0 = localmix_bright0[i];
        o_isp_adrc_localmix.bits.isp_adrc_localmix_dark1   = localmix_dark1[i];
        o_isp_adrc_localmix.bits.isp_adrc_localmix_bright1 = localmix_bright1[i];
        be_apb_lut->isp_adrc_localmix[i].u32 = o_isp_adrc_localmix.u32;
    }
}

static __inline td_void isp_drc_detail_dec_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *upisp_adrc_detaildec)
{
    td_s32 i;

    for (i = 0; i < DRC_DEC_LUT_NODE_NUM; i++) { /* drc detail_dec lut size is 128 */
        be_apb_lut->isp_adrc_detaildec[i].u32 = upisp_adrc_detaildec[i];
    }
}

static __inline td_void isp_drc_logcvt_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *upisp_adrc_logcvt)
{
    td_s32 i;

    for (i = 0; i < DRC_LOG_CVT_LUT_NODE_NUM; i++) { /* drc logcvt lut size is 128 */
        be_apb_lut->isp_adrc_logcvt[i].u32 = upisp_adrc_logcvt[i];
    }
}

static __inline td_void isp_drc_logcvt_luma_write(isp_be_lut_apb_type *be_apb_lut, const td_u16 *adrc_logcvt_luma)
{
    td_s32 i;

    for (i = 0; i < DRC_LOG_CVT_LUT_NODE_NUM; i++) { /* drc logcvt lut size is 128 */
        be_apb_lut->isp_adrc_logcvt_luma[i].u32 = adrc_logcvt_luma[i];
    }
}

/* be lut2stt */
static __inline td_void isp_dpc_lut_wstt_write(isp_pre_be_lut_wstt_type *pre_be_lut2stt, const td_u32 *dpc_lut_wstt)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_STATIC_DP_COUNT_NORMAL; i++) {
        pre_be_lut2stt->isp_dpc_lut_wstt[i].u32 = dpc_lut_wstt[i];
    }
}

static __inline td_void isp_mlsc_lut_wstt_write(isp_pre_be_lut_wstt_type *be_lut2stt, const td_u16 *lsc_rgain_wstt,
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
static __inline td_void isp_bnr_noised_lut_wstt_write(isp_pre_be_lut_wstt_type *lut2stt, const td_u16 *isp_bnr_noised)
{
    td_s32 i;
    for (i = 0; i < 65; i++) { /* [0, 65) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = isp_bnr_noised[2 * i]; /* 4/2 */
    }

    for (i = 0; i < 64; i++) { /* [0, 64) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = isp_bnr_noised[2 * i + 1]; /* 4/2 */
    }
}

static __inline td_void isp_bnr_noiseds_lut_wstt_write(isp_pre_be_lut_wstt_type *lut2stt, const td_u16 *isp_bnr_noiseds)
{
    td_s32 i;
    for (i = 65; i < 130; i++) { /* [65, 130) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = isp_bnr_noiseds[2 * (i - 65)]; /* 4/2/65 */
    }

    for (i = 65; i < 129; i++) { /* [65, 129) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = isp_bnr_noiseds[2 * (i - 65) + 1]; /* 4/2/65 */
    }
}

static __inline td_void isp_bnr_noisedinv_lut_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
                                                         const td_u32 *isp_bnr_noisedinv)
{
    td_s32 i;
    for (i = 0; i < 65; i++) { /* [0, 65) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = isp_bnr_noisedinv[2 * i]; /* addr 4 * i + 2 */
    }

    for (i = 0; i < 64; i++) { /* [0, 64) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = isp_bnr_noisedinv[2 * i + 1]; /* addr 4 * i + 3, 2 */
    }
}

static __inline td_void isp_bnr_noisedinv_mag_lut_wstt_write(isp_pre_be_lut_wstt_type *lut2stt,
    const td_u8 *noisedinv_mag)
{
    td_s32 i;
    for (i = 65; i < 130; i++) { /* [65, 130) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = noisedinv_mag[2 * (i - 65)]; /* 4/2/65 */
    }

    for (i = 65; i < 129; i++) { /* [65, 129) */
        lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = noisedinv_mag[2 * (i - 65) + 1]; /* 4/2/3/65 */
    }
}

static __inline td_void isp_bnr_noise_inv_l_lut_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
                                                           const td_u32 *isp_bnr_noise_inv_l)
{
    td_s32 i;
    for (i = 130; i < 195; i++) { /* [130, 195) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = isp_bnr_noise_inv_l[2 * (i - 130)]; /* addr 4 * i + 2 */
    }

    for (i = 130; i < 194; i++) { /* [130, 194) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = isp_bnr_noise_inv_l[2 * (i - 130) + 1]; /* 4 * i + 3, 2 */
    }
}

static __inline td_void isp_bnr_coring_low_lut_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
                                                          const td_u32 *isp_bnr_coring_low)
{
    td_s32 i;
    for (i = 130; i < 147; i++) { /* [130, 147) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = isp_bnr_coring_low[2 * (i - 130)]; /* addr 4 * i + 2 */
    }

    for (i = 130; i < 146; i++) { /* [130, 146) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = isp_bnr_coring_low[2 * (i - 130) + 1]; /* 4 * i + 3, 2 */
    }
}

static __inline td_void isp_bnr_fbratiotable_lut_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
    const td_u8 *isp_bnr_fbratiotable)
{
    td_s32 i;
    for (i = 195; i < 259; i++) { /* [195, 259) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 0].u32 = isp_bnr_fbratiotable[i - 195]; /* addr 4 * i + 0, sub 195 */
    }
}

static __inline td_void isp_bnr_mix_gain_lut_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
    const td_u8 *isp_bnr_mix_gain)
{
    td_s32 i;
    for (i = 195; i < 227; i++) { /* [195, 227) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 1].u32 = isp_bnr_mix_gain[i - 195]; /* addr 4 * i + 1, sub 195 */
    }
}

static __inline td_void isp_bnr_ynet_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
    const td_u8 *ynet_fg, const td_u8 *ynet_bg)
{
    td_s32 i, idx;
    for (i = 195; i < 211; i++) { /* [195, 211) */
        idx = i - 195; /* sub 195 */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 2].u32 = ((td_u32)ynet_fg[idx] << 16) + ynet_bg[idx]; /* num 4 2 16 */
    }
}

static __inline td_void isp_bnr_ynet_mot_wstt_write(isp_pre_be_lut_wstt_type *post_be_lut2stt,
    const td_u8 *ynet_mot)
{
    td_s32 i;
    for (i = 195; i < 211; i++) { /* [195, 211) */
        post_be_lut2stt->isp_bnr_lut_wstt[4 * i + 3].u32 = ynet_mot[i - 195]; /* addr 4 * i + 3, sub 195 */
    }
}

/* Input         td_u32 *upisp_clut_lut_wstt  30 bits */
static __inline td_void isp_clut_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u32 *clut_lut_wstt)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_CLUT_LUT_LENGTH; i++) {
        be_lut2stt->isp_clut_lut_wstt[i].u32 = clut_lut_wstt[i];
    }
}

/* Input         td_u32 *upisp_gamma_lut_wstt  12 bits */
static __inline td_void isp_gamma_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 *gamma_lut_wstt)
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
static __inline td_void isp_dehaze_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 blk_num,
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
static __inline td_void isp_sharpen_lut_wstt_write(isp_post_be_lut_wstt_type *lut2stt, const td_u16 *mf_gain_d,
    const td_u16 *mf_gain_ud, const td_u16 *hf_gain_d, const td_u16 *hf_gain_ud)
{
    for (td_s32 i = 0; i < 32; i++) {  /* sharpen lut size is 32 */
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
static __inline td_void isp_ldci_drc_cgain_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt,
    const td_u32 *cgain_lut)
{
    td_s32 i;

    for (i = 0; i < 65; i++) { /* [0, 65) */
        be_lut2stt->isp_ldci_lut_wstt[4 * i].u32 = (cgain_lut[i] << 20); /* addr:4 * i, shift 20 bit */
        be_lut2stt->isp_ldci_lut_wstt[4 * i + 3].u32 = 0;  /* addr:4 * i + 3 */
    }
}

static __inline td_void isp_ldci_he_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt,
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

static __inline td_void isp_ldci_usm_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt,
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

static __inline td_void isp_ca_y_ratio_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u32 *upisp_ca_lut)
{
    td_s32 i;

    for (i = 0; i < OT_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        be_lut2stt->isp_ca_lut_wstt[i].u32 = upisp_ca_lut[i] & 0xFFFFFF;
    }
}

static __inline td_void isp_ca_y_sat_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u32 *y_sat_lut)
{
    td_s32 i;

    for (i = 256; i < 512; i++) { /* [256, 512) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = y_sat_lut[i - 256]; /* index: i - 256 */
    }
}

static __inline td_void isp_ca_y_ch1_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 *y_ch1_lut)
{
    td_s32 i, idx;

    for (i = 512; i < 520; i++) { /* [512, 520) */
        idx = 2 * (i - 512); /* even index: 2 * (i - 512) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = ((td_u32)y_ch1_lut[idx + 1] << 16) + y_ch1_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_ca_y_ch2_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 *y_ch2_lut)
{
    td_s32 i, idx;

    for (i = 520; i < 528; i++) { /* [520, 528) */
        idx = 2 * (i - 520); /* even index: 2 * (i - 520) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = ((td_u32)y_ch2_lut[idx + 1] << 16) + y_ch2_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_ca_y_slu_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 *y_slu_lut)
{
    td_s32 i, idx;
    for (i = 528; i < 536; i++) { /* [528, 536) */
        idx = 2 * (i - 528); /* even index: 2 * (i - 528) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = ((td_u32)y_slu_lut[idx + 1] << 16) + y_slu_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_ca_y_slv_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 *y_slv_lut)
{
    td_s32 i, idx;

    for (i = 536; i < 544; i++) { /* [536, 544) */
        idx = 2 * (i - 536); /* even index: 2 * (i - 536) */
        be_lut2stt->isp_ca_lut_wstt[i].u32 = ((td_u32)y_slv_lut[idx + 1] << 16) + y_slv_lut[idx]; /* shift 16 */
    }
}

static __inline td_void isp_cc_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt, const td_u16 *upisp_cc_dgain_lut,
    const td_u16 *upisp_cc_gamma_lut)
{
    td_s32 i;

    for (i = 0; i < 65; i++) {  /* cc lut size is 65 */
        be_lut2stt->isp_cc_lut_wstt[i].u32 = upisp_cc_dgain_lut[i] & 0x3FFF;      /* [0, 64] */
        be_lut2stt->isp_cc_lut_wstt[i + 68].u32 = upisp_cc_gamma_lut[i] & 0xFFF; /* [68, 132] */
    }
}

static __inline td_void isp_dmnr_lut_wstt_write(isp_post_be_lut_wstt_type *be_lut2stt,
    const td_u16 *dmnr_lut)
{
    td_s32 i;

    for (i = 0; i < 128; i++) { /* [0, 128) */
        be_lut2stt->isp_dmnr_lut_wstt[i].u32 = dmnr_lut[i];
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
