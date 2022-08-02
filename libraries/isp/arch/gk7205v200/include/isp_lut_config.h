/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_LUT_CONFIG_H__
#define __ISP_LUT_CONFIG_H__

#include "mpp_debug.h"
#include "isp_debug.h"
#include "isp_vreg.h"
#include "isp_main.h"
#include "isp_regcfg.h"
#include "isp_lut_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


static __inline GK_VOID isp_ae_weight_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 *upisp_ae_weight)
{
    GK_S32 i;

    for (i = 0; i < 64; i++) {
        pstBeReg->stIspBeLut.ISP_AE_WEIGHT[i].u32 = upisp_ae_weight[i];
    }
}

// ******************************************************************************
//  Function    : iSetISP_DEMOSAIC_DEPURPLUTisp_demosaic_depurp_lut
//  Description : Set the value of the member ISP_DEMOSAIC_DEPURPLUT.isp_demosaic_depurp_lut
//  Input       : GK_U32 *upisp_demosaic_depurp_lut: 4 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_demosaic_depurp_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U8 *upisp_demosaic_depurp_lut)
{
    U_ISP_DEMOSAIC_DEPURPLUT o_isp_demosaic_depurplut;
    GK_S32 i;
    for (i = 0; i < 16; i++) {
        o_isp_demosaic_depurplut.bits.isp_demosaic_depurp_lut = upisp_demosaic_depurp_lut[i];
        o_isp_demosaic_depurplut.bits.reserved_0 = 0;
        pstBeReg->stIspBeLut.ISP_DEMOSAIC_DEPURPLUT[i].u32 = o_isp_demosaic_depurplut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_NDDM_GF_LUTisp_nddm_gflut
//  Description : Set the value of the member ISP_NDDM_GF_LUT.isp_nddm_gflut
//  Input       : GK_U32 *upisp_nddm_gflut: 12 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_U8 isp_nddm_gflut_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *upisp_nddm_gflut)
{
    U_ISP_NDDM_GF_LUT o_isp_nddm_gf_lut;
    GK_S32 i;
    ISP_CHECK_NULLPTR(pstBeReg);

    for (i = 0; i < 17; i++) {
        o_isp_nddm_gf_lut.bits.isp_nddm_gflut = upisp_nddm_gflut[i];
        o_isp_nddm_gf_lut.bits.reserved_0 = 0;
        pstBeReg->stIspBeLut.ISP_NDDM_GF_LUT[i].u32 = o_isp_nddm_gf_lut.u32;
    }

    return 1;
}

// ******************************************************************************
//  Function    : iSetISP_EXPANDER_LUTisp_expander_lut
//  Description : Set the value of the member ISP_EXPANDER_LUT.isp_expander_lut
//  Input       : GK_U32 *upisp_expander_lut: 21 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_expander_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *upisp_expander_lut)
{
    U_ISP_EXPANDER_LUT o_isp_expander_lut;
    GK_S32 i;

    for (i = 0; i < 129; i++) {
        o_isp_expander_lut.bits.isp_expander_lut = upisp_expander_lut[i];
        o_isp_expander_lut.bits.reserved_0 = 0;
        pstBeReg->stIspBeLut.ISP_EXPANDER_LUT[i].u32 = o_isp_expander_lut.u32;
    }

}

// ******************************************************************************
//  Function    : iSetISP_DRC_TMLUT0isp_drc_tmlut0_diff
//  Description : Set the value of the member ISP_DRC_TMLUT0.isp_drc_tmlut0_diff
//  Input       : GK_U32 *upisp_drc_tmlut0_diff: 14 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_drc_tmlut0_diff_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *upisp_drc_tmlut0_diff)
{
    U_ISP_DRC_TMLUT0 o_isp_drc_tmlut0;
    GK_S32 i;

    for (i = 0; i < 200; i++) {
        o_isp_drc_tmlut0.u32 = pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_diff = upisp_drc_tmlut0_diff[i];
        pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_TMLUT0isp_drc_tmlut0_value
//  Description : Set the value of the member ISP_DRC_TMLUT0.isp_drc_tmlut0_value
//  Input       : GK_U32 *upisp_drc_tmlut0_value: 16 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_drc_tmlut0_value_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *upisp_drc_tmlut0_value)
{
    U_ISP_DRC_TMLUT0 o_isp_drc_tmlut0;
    GK_S32 i;

    for (i = 0; i < 200; i++) {
        o_isp_drc_tmlut0.u32 = pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_value = upisp_drc_tmlut0_value[i];
        pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_CCLUTisp_drc_cclut
//  Description : Set the value of the member ISP_DRC_CCLUT.isp_drc_cclut
//  Input       : GK_U32 *upisp_drc_cclut: 12 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_drc_cclut_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *upisp_drc_cclut)
{
    U_ISP_DRC_CCLUT o_isp_drc_cclut;
    GK_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_drc_cclut.u32 = pstBeReg->stIspBeLut.ISP_DRC_CCLUT[i].u32;
        o_isp_drc_cclut.bits.isp_drc_cclut = upisp_drc_cclut[i];
        pstBeReg->stIspBeLut.ISP_DRC_CCLUT[i].u32 = o_isp_drc_cclut.u32;
    }
}


static __inline GK_VOID isp_drc_pregamma_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *uisp_drc_pregamma)
{
    GK_S32 i;
    for (i = 0; i < 257; i++) {
        pstBeReg->stIspBeLut.ISP_DRC_PREGAMMA[i].u32  = uisp_drc_pregamma[i];
    }
}

// ******************************************************************************
//  Function    : iSetISP_DEHAZE_LUT_WSTTisp_dehaze_lut_wstt
//  Description : Set the value of the member ISP_DEHAZE_LUT_WSTT.isp_dehaze_lut_wstt
//  Input       : GK_U32 *upisp_dehaze_lut_wstt: 10 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_dehaze_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U16 u16BlkNum,
                                                  GK_U32 *upisp_dehaze_prestat_wstt, GK_U8 *upisp_dehaze_lut_wstt)
{
    GK_S32 i;

    for (i = 0; i < u16BlkNum; i++) {
        pstBeLutSttReg->ISP_DEHAZE_LUT_WSTT[2 * i].bits.isp_dehaze_lut_wstt     = upisp_dehaze_prestat_wstt[i] & 0x3ff;
        pstBeLutSttReg->ISP_DEHAZE_LUT_WSTT[2 * i + 1].bits.isp_dehaze_lut_wstt = (upisp_dehaze_prestat_wstt[i] >> 16) & 0x3ff;
    }
    for (i = 2 * u16BlkNum; i < 384; i++) {
        pstBeLutSttReg->ISP_DEHAZE_LUT_WSTT[i].bits.isp_dehaze_lut_wstt = 0;
    }
    for (i = 384; i < 640; i++) {
        pstBeLutSttReg->ISP_DEHAZE_LUT_WSTT[i].bits.isp_dehaze_lut_wstt = upisp_dehaze_lut_wstt[i - 384];
    }
}

// ******************************************************************************
//  Function    : iSetISP_GAMMA_LUT_WSTTisp_gamma_lut_wstt
//  Description : Set the value of the member ISP_GAMMA_LUT_WSTT.isp_gamma_lut_wstt
//  Input       : GK_U32 *upisp_gamma_lut_wstt: 12 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_gamma_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U16 *upisp_gamma_lut_wstt)
{
    U_ISP_GAMMA_LUT_WSTT o_isp_gamma_lut_wstt;
    GK_S32 i;

    for (i = 0; i < 257; i++) {
        o_isp_gamma_lut_wstt.u32 = pstBeLutSttReg->ISP_GAMMA_LUT_WSTT[i].u32;
        o_isp_gamma_lut_wstt.bits.isp_gamma_lut_wstt = upisp_gamma_lut_wstt[i];
        pstBeLutSttReg->ISP_GAMMA_LUT_WSTT[i].u32 = o_isp_gamma_lut_wstt.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_SHARPEN_LUT_WSTTisp_sharpen_lut_wstt
//  Description : Set the value of the member ISP_SHARPEN_LUT_WSTT.isp_sharpen_lut_wstt
//  Input       : GK_U32 *upisp_sharpen_lut_wstt: 30 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_sharpen_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg,
                                                   GK_U16 *upisp_sharpen_mfgaind_wstt,
                                                   GK_U16 *upisp_sharpen_mfgainud_wstt,
                                                   GK_U16 *upisp_sharpen_hfgaind_wstt,
                                                   GK_U16 *upisp_sharpen_hfgainud_wstt)
{
    GK_S32 i;
    for (i = 0; i < ISP_SHARPEN_LUT_WIDTH_WORD_DEFAULT; i++) {
        pstBeLutSttReg->ISP_SHARPEN_LUT_WSTT[i * 4].bits.isp_sharpen_lut_wstt     = upisp_sharpen_mfgaind_wstt [2 * i]   + (upisp_sharpen_hfgaind_wstt[2 * i]   << 12);
        pstBeLutSttReg->ISP_SHARPEN_LUT_WSTT[i * 4 + 1].bits.isp_sharpen_lut_wstt = upisp_sharpen_mfgainud_wstt[2 * i]   + (upisp_sharpen_hfgainud_wstt[2 * i]  << 12);
        pstBeLutSttReg->ISP_SHARPEN_LUT_WSTT[i * 4 + 2].bits.isp_sharpen_lut_wstt = upisp_sharpen_mfgaind_wstt [2 * i + 1] + (upisp_sharpen_hfgaind_wstt[2 * i + 1] << 12);
        pstBeLutSttReg->ISP_SHARPEN_LUT_WSTT[i * 4 + 3].bits.isp_sharpen_lut_wstt = upisp_sharpen_mfgainud_wstt[2 * i + 1] + (upisp_sharpen_hfgainud_wstt[2 * i + 1] << 12);
    }
}

// ******************************************************************************
//  Function    : iSetISP_LSC_LUT_WSTTisp_lsc_lut_wstt
//  Description : Set the value of the member ISP_LSC_LUT_WSTT.isp_lsc_lut_wstt
//  Input       : GK_U32 *upisp_lsc_lut_wstt: 20 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_mlsc_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U32 *upisp_lsc_rgain_wstt,
                                                GK_U32 *upisp_lsc_grgain_wstt, GK_U32 *upisp_lsc_gbgain_wstt,
                                                GK_U32 *upisp_lsc_bgain_wstt)
{
    GK_S32 i;
    for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
        pstBeLutSttReg->ISP_MLSC_LUT_WSTT[4 * i + 0].u32 = upisp_lsc_rgain_wstt[i];
        pstBeLutSttReg->ISP_MLSC_LUT_WSTT[4 * i + 1].u32 = upisp_lsc_grgain_wstt[i];
        pstBeLutSttReg->ISP_MLSC_LUT_WSTT[4 * i + 2].u32 = upisp_lsc_bgain_wstt[i];
        pstBeLutSttReg->ISP_MLSC_LUT_WSTT[4 * i + 3].u32 = upisp_lsc_gbgain_wstt[i];
    }
}

// ******************************************************************************
//  Function    : iSetISP_CA_LUT_WSTTisp_ca_lut_wstt
//  Description : Set the value of the member ISP_CA_LUT_WSTT.isp_ca_lut_wstt
//  Input       : GK_U32 *upisp_ca_lut_wstt: 11 bits
//  Return      : GK_S32: 0-Error, 1-Success
// ******************************************************************************
static __inline GK_VOID isp_ca_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U16 *upisp_ca_lut_wstt)
{
    U_ISP_CA_LUT_WSTT o_isp_ca_lut_wstt;
    GK_S32 i;

    for (i = 0; i < 128; i++) {
        o_isp_ca_lut_wstt.u32 = pstBeLutSttReg->ISP_CA_LUT_WSTT[i].u32;
        o_isp_ca_lut_wstt.bits.isp_ca_lut_wstt = upisp_ca_lut_wstt[i];
        pstBeLutSttReg->ISP_CA_LUT_WSTT[i].u32 = o_isp_ca_lut_wstt.u32;
    }
}

static __inline GK_VOID isp_ldci_cgain_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg,
                                                      GK_U32 *upisp_ldci_cgain_lut_wstt)
{
    GK_S32 i;

    for (i = 0; i < 65; i++) {
        pstBeLutSttReg->ISP_LDCI_LUT_WSTT[4 * i].u32     = (upisp_ldci_cgain_lut_wstt[i] << 20);

        pstBeLutSttReg->ISP_LDCI_LUT_WSTT[4 * i + 3].u32 = 0;
    }

}

static __inline GK_VOID isp_ldci_he_lut_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg,
                                                       GK_U32 *upisp_ldci_hepos_lut_wstt,
                                                       GK_U32 *upisp_ldci_heneg_lut_wstt)
{
    GK_S32 i;

    for (i = 0; i < 33; i++) {
        pstBeLutSttReg->ISP_LDCI_LUT_WSTT[4 * i + 1].u32 = (upisp_ldci_heneg_lut_wstt[i] << 18) + (upisp_ldci_hepos_lut_wstt[i] << 9);
    }

    for (i = 33; i < 65; i++) {
        pstBeLutSttReg->ISP_LDCI_LUT_WSTT[4 * i + 1].u32 = 0;
    }

}

static __inline GK_VOID isp_ldci_usm_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg,
                                                    GK_U32 *upisp_ldci_usmpos_lut_wstt,
                                                    GK_U32 *upisp_ldci_usmneg_lut_wstt)
{
    GK_S32 i;

    for (i = 0; i < 33; i++) {
        pstBeLutSttReg->ISP_LDCI_LUT_WSTT[4 * i + 2].u32 = (upisp_ldci_usmneg_lut_wstt[i] << 9) + upisp_ldci_usmpos_lut_wstt[i];
    }

    for (i = 33; i < 65; i++) {
        pstBeLutSttReg->ISP_LDCI_LUT_WSTT[4 * i + 2].u32 = 0;
    }

}

static __inline GK_VOID isp_bnr_lmt_even_wlut_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U8 *isp_bnr_lmt_s_even,
                                                    GK_U8 *isp_bnr_lmt_even)
{
    GK_S32 i;
    for (i = 0; i < 65; i++) {
        pstBeLutSttReg->ISP_BNR_LUT_WSTT[4 * i].u32 = (isp_bnr_lmt_s_even[i] << 8) + (isp_bnr_lmt_even[i]);
    }
}

static __inline GK_VOID isp_bnr_lmt_odd_wlut_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U8 *isp_bnr_lmt_s_odd,
                                                   GK_U8 *isp_bnr_lmt_odd)
{
    GK_S32 i;
    for (i = 0; i < 64; i++) {
        pstBeLutSttReg->ISP_BNR_LUT_WSTT[4 * i + 1].u32 = (isp_bnr_lmt_s_odd[i] << 8) + (isp_bnr_lmt_odd[i]);
    }
}

static __inline GK_VOID isp_bnr_cor_even_wlut_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U8 *isp_bnr_gcor_even,
                                                    GK_U16 *isp_bnr_cor_even)
{
    GK_S32 i;
    for (i = 0; i < 17; i++) {
        pstBeLutSttReg->ISP_BNR_LUT_WSTT[4 * i + 2].u32 = (isp_bnr_gcor_even[i] << 16) + (isp_bnr_cor_even[i]);
    }
}

static __inline GK_VOID isp_bnr_cor_odd_wlut_write(S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg, GK_U8 *isp_bnr_gcor_odd,
                                                   GK_U16 *isp_bnr_cor_odd)
{
    GK_S32 i;
    for (i = 0; i < 16; i++) {
        pstBeLutSttReg->ISP_BNR_LUT_WSTT[4 * i + 3].u32 = (isp_bnr_gcor_odd[i] << 16) + (isp_bnr_cor_odd[i]);
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
