/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_lut_config.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_LUT_CONFIG_H__
#define __ISP_LUT_CONFIG_H__

#include "hi_debug.h"
#include "hi_isp_debug.h"
#include "isp_vreg.h"
#include "isp_main.h"
#include "isp_regcfg.h"
#include "isp_lut_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Function    : iSetISP_AE_WEIGHTisp_ae_weight2 */
/* Description : Set the value of the member ISP_AE_WEIGHT.isp_ae_weight2 */
/* Input       : hi_u32 *upisp_ae_weight2: 4 bits */
static __inline hi_void isp_ae_weight_write(S_ISPBE_REGS_TYPE *be_reg, hi_u32 *upisp_ae_weight)
{
    hi_s32 i;

    for (i = 0; i < 64; i++) { /* ae weight table number 64 */
        be_reg->stIspBeLut.ISP_AE_WEIGHT[i].u32 = upisp_ae_weight[i];
    }
}

/* Function    : iSetISP_DEMOSAIC_DEPURPLUTisp_demosaic_depurp_lut */
/* Description : Set the value of the member ISP_DEMOSAIC_DEPURPLUT.isp_demosaic_depurp_lut */
/* Input       : hi_u32 *upisp_demosaic_depurp_lut: 4 bits */
static __inline hi_void isp_demosaic_depurp_lut_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_demosaic_depurp_lut)
{
    hi_s32 i;

    for (i = 0; i < LCAC_STRENGTH_NUM; i++) {
        be_reg->stIspBeLut.ISP_DEMOSAIC_DEPURPLUT[i].u32 = upisp_demosaic_depurp_lut[i] & 0xF;
    }
}

/* Function    : iSetISP_NDDM_GF_LUTisp_nddm_gflut */
/* Description : Set the value of the member ISP_NDDM_GF_LUT.isp_nddm_gflut */
/* Input       : hi_u32 *upisp_nddm_gflut: 12 bits */
static __inline hi_void isp_nddm_gflut_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_nddm_gflut)
{
    hi_s32 i;

    for (i = 0; i < HI_ISP_DEMOSAIC_LUT_LENGTH; i++) {
        be_reg->stIspBeLut.ISP_NDDM_GF_LUT[i].u32 = upisp_nddm_gflut[i] & 0xFFF;
    }
}

/* Function    : iSetISP_BNR_LMT_EVENisp_bnr_lmt_even */
/* Description : Set the value of the member ISP_BNR_LMT_EVEN.isp_bnr_lmt_even */
/* Input       : hi_u32 *upisp_bnr_lmt_even: 8 bits */
static __inline hi_void isp_bnr_lmt_even_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_bnr_lmt_even)
{
    hi_s32 i;

    for (i = 0; i < 65; i++) { /* 65:bnr_lmt_even lut size */
        be_reg->stIspBeLut.ISP_BNR_LMT_EVEN[i].u32 = upisp_bnr_lmt_even[2 * i]; /* 2 * i: even */
    }
}

/* Function    : iSetISP_BNR_LMT_ODDisp_bnr_lmt_odd */
/* Description : Set the value of the member ISP_BNR_LMT_ODD.isp_bnr_lmt_odd */
/* Input       : hi_u32 *upisp_bnr_lmt_odd: 8 bits */
static __inline hi_void isp_bnr_lmt_odd_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_bnr_lmt_odd)
{
    hi_s32 i;

    for (i = 0; i < 64; i++) { /* 64:bnr_lmt_odd lut size */
        be_reg->stIspBeLut.ISP_BNR_LMT_ODD[i].u32 = upisp_bnr_lmt_odd[2 * i + 1]; /* 2 * i + 1: odd */
    }
}

/* Function    : iSetISP_BNR_COR_EVENisp_bnr_cor_even */
/* Description : Set the value of the member ISP_BNR_COR_EVEN.isp_bnr_cor_even */
/* Input       : hi_u32 *upisp_bnr_cor_even: 14 bits */
static __inline hi_void isp_bnr_cor_even_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_bnr_cor_even)
{
    hi_s32 i;

    for (i = 0; i < 17; i++) { /* 17:bnr_cor_even lut size */
        be_reg->stIspBeLut.ISP_BNR_COR_EVEN[i].u32 = upisp_bnr_cor_even[2 * i] & 0x3FFF; /* 2 * i: even */
    }
}

/* Function    : iSetISP_BNR_COR_ODDisp_bnr_cor_odd */
/* Description : Set the value of the member ISP_BNR_COR_ODD.isp_bnr_cor_odd */
/* Input       : hi_u32 *upisp_bnr_cor_odd: 14 bits */
static __inline hi_void isp_bnr_cor_odd_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_bnr_cor_odd)
{
    hi_s32 i;

    for (i = 0; i < 16; i++) { /* 16:bnr_cor_odd lut size */
        be_reg->stIspBeLut.ISP_BNR_COR_ODD[i].u32 = upisp_bnr_cor_odd[2 * i + 1] & 0x3FFF; /* 2 * i + 1: odd */
    }
}

/* Function    : iSetISP_BNR_LMT_S_EVENisp_bnr_lmt_s_even */
/* Description : Set the value of the member ISP_BNR_LMT_S_EVEN.isp_bnr_lmt_s_even */
/* Input       : hi_u32 *upisp_bnr_lmt_s_even: 8 bits */
static __inline hi_void isp_bnr_lmt_s_even_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_bnr_lmt_s_even)
{
    hi_s32 i;

    for (i = 0; i < 65; i++) { /* 65:bnr_lmt_s_even lut size */
        be_reg->stIspBeLut.ISP_BNR_LMT_S_EVEN[i].u32 = upisp_bnr_lmt_s_even[2 * i]; /* 2 * i: even */
    }
}

/* Function    : iSetISP_BNR_LMT_S_ODDisp_bnr_lmt_s_odd */
/* Description : Set the value of the member ISP_BNR_LMT_S_ODD.isp_bnr_lmt_s_odd */
/* Input       : hi_u32 *upisp_bnr_lmt_s_odd: 8 bits */
static __inline hi_void isp_bnr_lmt_s_odd_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_bnr_lmt_s_odd)
{
    hi_s32 i;

    for (i = 0; i < 64; i++) { /* 64:bnr_lmt_s_odd lut size */
        be_reg->stIspBeLut.ISP_BNR_LMT_S_ODD[i].u32 = upisp_bnr_lmt_s_odd[2 * i + 1]; /* 2 * i + 1: odd */
    }
}

/* Function    : iSetISP_BNR_GCOR_EVENisp_bnr_gcor_even */
/* Description : Set the value of the member ISP_BNR_GCOR_EVEN.isp_bnr_gcor_even */
/* Input       : hi_u32 *upisp_bnr_gcor_even: 8 bits */
static __inline hi_void isp_bnr_gcor_even_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_bnr_gcor_even)
{
    hi_s32 i;

    for (i = 0; i < 17; i++) { /* 17:bnr_gcor_even lut size */
        be_reg->stIspBeLut.ISP_BNR_GCOR_EVEN[i].u32 = upisp_bnr_gcor_even[2 * i]; /* 2 * i: even */
    }
}

/* Function    : iSetISP_BNR_GCOR_ODDisp_bnr_gcor_odd */
/* Description : Set the value of the member ISP_BNR_GCOR_ODD.isp_bnr_gcor_odd */
/* Input       : hi_u32 *upisp_bnr_gcor_odd: 8 bits */
static __inline hi_void isp_bnr_gcor_odd_write(S_ISPBE_REGS_TYPE *be_reg, hi_u8 *upisp_bnr_gcor_odd)
{
    hi_s32 i;

    for (i = 0; i < 16; i++) { /* 16:bnr_gcor_odd lut size */
        be_reg->stIspBeLut.ISP_BNR_GCOR_ODD[i].u32 = upisp_bnr_gcor_odd[2 * i + 1]; /* 2 * i + 1: odd */
    }
}

/* Function    : iSetISP_DE_LUMA_GAINisp_de_luma_gain */
/* Description : Set the value of the member ISP_DE_LUMA_GAIN.isp_de_luma_gain */
/* Input       : hi_u32 *upisp_de_luma_gain: 9 bits */
static __inline hi_void isp_de_luma_gain_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_de_luma_gain)
{
    hi_s32 i;

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        be_reg->stIspBeLut.ISP_DE_LUMA_GAIN[i].u32 = upisp_de_luma_gain[i] & 0x1FF;
    }
}

/* Function    : iSetISP_EXPANDER_LUTisp_expander_lut */
/* Description : Set the value of the member ISP_EXPANDER_LUT.isp_expander_lut */
/* Input       : hi_u32 *upisp_expander_lut: 21 bits */
static __inline hi_void isp_expander_lut_write(S_ISPBE_REGS_TYPE *be_reg, hi_u32 *upisp_expander_lut)
{
    hi_s32 i;

    for (i = 0; i < EXPANDER_NODE_NUM; i++) {
        be_reg->stIspBeLut.ISP_EXPANDER_LUT[i].u32 = upisp_expander_lut[i] & 0x1FFFFF;
    }
}

/* Function    : iSetISP_LDCI_POLYP_WLUTisp_ldci_poply1_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYP_WLUT.isp_ldci_poply1_wlut */
/* Input       : hi_u32 *upisp_ldci_poply1_wlut: 10 bits */
static __inline hi_void isp_ldci_poply1_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_poply1_wlut)
{
    U_ISP_LDCI_POLYP_WLUT o_isp_ldci_polyp_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyp_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32;
        o_isp_ldci_polyp_wlut.bits.isp_ldci_poply1_wlut = upisp_ldci_poply1_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32 = o_isp_ldci_polyp_wlut.u32;
    }
}

/* Function    : iSetISP_LDCI_POLYP_WLUTisp_ldci_poply2_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYP_WLUT.isp_ldci_poply2_wlut */
/* Input       : hi_u32 *upisp_ldci_poply2_wlut: 10 bits */
static __inline hi_void isp_ldci_poply2_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_poply2_wlut)
{
    U_ISP_LDCI_POLYP_WLUT o_isp_ldci_polyp_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyp_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32;
        o_isp_ldci_polyp_wlut.bits.isp_ldci_poply2_wlut = upisp_ldci_poply2_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32 = o_isp_ldci_polyp_wlut.u32;
    }
}

/* Function    : iSetISP_LDCI_POLYP_WLUTisp_ldci_poply3_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYP_WLUT.isp_ldci_poply3_wlut */
/* Input       : hi_u32 *upisp_ldci_poply3_wlut: 10 bits */
static __inline hi_void isp_ldci_poply3_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_poply3_wlut)
{
    U_ISP_LDCI_POLYP_WLUT o_isp_ldci_polyp_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyp_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32;
        o_isp_ldci_polyp_wlut.bits.isp_ldci_poply3_wlut = upisp_ldci_poply3_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32 = o_isp_ldci_polyp_wlut.u32;
    }
}

/* Function    : iSetISP_LDCI_POLYQ01_WLUTisp_ldci_plyq0_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYQ01_WLUT.isp_ldci_plyq0_wlut */
/* Input       : hi_u32 *upisp_ldci_plyq0_wlut: 12 bits */
static __inline hi_void isp_ldci_plyq0_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_plyq0_wlut)
{
    U_ISP_LDCI_POLYQ01_WLUT o_isp_ldci_polyq01_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyq01_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32;
        o_isp_ldci_polyq01_wlut.bits.isp_ldci_plyq0_wlut = upisp_ldci_plyq0_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32 = o_isp_ldci_polyq01_wlut.u32;
    }
}

/* Function    : iSetISP_LDCI_POLYQ01_WLUTisp_ldci_plyq1_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYQ01_WLUT.isp_ldci_plyq1_wlut */
/* Input       : hi_u32 *upisp_ldci_plyq1_wlut: 12 bits */
static __inline hi_void isp_ldci_plyq1_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_plyq1_wlut)
{
    U_ISP_LDCI_POLYQ01_WLUT o_isp_ldci_polyq01_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyq01_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32;
        o_isp_ldci_polyq01_wlut.bits.isp_ldci_plyq1_wlut = upisp_ldci_plyq1_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32 = o_isp_ldci_polyq01_wlut.u32;
    }
}

/* Function    : iSetISP_LDCI_POLYQ23_WLUTisp_ldci_plyq2_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYQ23_WLUT.isp_ldci_plyq2_wlut */
/* Input       : hi_u32 *upisp_ldci_plyq2_wlut: 12 bits */
static __inline hi_void isp_ldci_plyq2_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_plyq2_wlut)
{
    U_ISP_LDCI_POLYQ23_WLUT o_isp_ldci_polyq23_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyq23_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32;
        o_isp_ldci_polyq23_wlut.bits.isp_ldci_plyq2_wlut = upisp_ldci_plyq2_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32 = o_isp_ldci_polyq23_wlut.u32;
    }
}

/* Function    : iSetISP_LDCI_POLYQ23_WLUTisp_ldci_plyq3_wlut */
/* Description : Set the value of the member ISP_LDCI_POLYQ23_WLUT.isp_ldci_plyq3_wlut */
/* Input       : hi_u32 *upisp_ldci_plyq3_wlut: 12 bits */
static __inline hi_void isp_ldci_plyq3_wlut_write(S_ISPBE_REGS_TYPE *be_reg, hi_s16 *upisp_ldci_plyq3_wlut)
{
    U_ISP_LDCI_POLYQ23_WLUT o_isp_ldci_polyq23_wlut;
    hi_s32 i;

    for (i = 0; i < LDCI_POLY_LUT_NUM; i++) {
        o_isp_ldci_polyq23_wlut.u32 = be_reg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32;
        o_isp_ldci_polyq23_wlut.bits.isp_ldci_plyq3_wlut = upisp_ldci_plyq3_wlut[i];
        be_reg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32 = o_isp_ldci_polyq23_wlut.u32;
    }
}

/* Function    : iSetISP_DRC_TMLUT0isp_drc_tmlut0_diff */
/* Description : Set the value of the member ISP_DRC_TMLUT0.isp_drc_tmlut0_diff */
/* Input       : hi_u32 *upisp_drc_tmlut0_diff: 14 bits */
static __inline hi_void isp_drc_tmlut0_diff_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_drc_tmlut0_diff)
{
    U_ISP_DRC_TMLUT0 o_isp_drc_tmlut0;
    hi_s32 i;

    for (i = 0; i < HI_ISP_DRC_TM_NODE_NUM; i++) {
        o_isp_drc_tmlut0.u32 = be_reg->stIspBeLut.ISP_DRC_TMLUT0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_diff = upisp_drc_tmlut0_diff[i];
        be_reg->stIspBeLut.ISP_DRC_TMLUT0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

/* Function    : iSetISP_DRC_TMLUT0isp_drc_tmlut0_value */
/* Description : Set the value of the member ISP_DRC_TMLUT0.isp_drc_tmlut0_value */
/* Input       : hi_u32 *upisp_drc_tmlut0_value: 16 bits */
static __inline hi_void isp_drc_tmlut0_value_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_drc_tmlut0_value)
{
    U_ISP_DRC_TMLUT0 o_isp_drc_tmlut0;
    hi_s32 i;

    for (i = 0; i < HI_ISP_DRC_TM_NODE_NUM; i++) {
        o_isp_drc_tmlut0.u32 = be_reg->stIspBeLut.ISP_DRC_TMLUT0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_value = upisp_drc_tmlut0_value[i];
        be_reg->stIspBeLut.ISP_DRC_TMLUT0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

/* Function    : iSetISP_DRC_CCLUTisp_drc_cclut */
/* Description : Set the value of the member ISP_DRC_CCLUT.isp_drc_cclut */
/* Input       : hi_u32 *upisp_drc_cclut: 12 bits */
static __inline hi_void isp_drc_cclut_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *upisp_drc_cclut)
{
    hi_s32 i;

    for (i = 0; i < HI_ISP_DRC_CC_NODE_NUM; i++) {
        be_reg->stIspBeLut.ISP_DRC_CCLUT[i].u32 = upisp_drc_cclut[i] & 0xFFF;
    }
}

/* Function    : iSetISP_CLUT_LUT_WSTTisp_clut_lut_wstt */
/* Description : Set the value of the member ISP_CLUT_LUT_WSTT.isp_clut_lut_wstt */
/* Input       : hi_u32 *upisp_clut_lut_wstt: 30 bits */
/* Return      : hi_s32 : 0-Error, 1-Success */
static __inline hi_void isp_clut_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u32 *upisp_clut_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < HI_ISP_CLUT_LUT_LENGTH; i++) {
        be_lut_stt_reg->ISP_CLUT_LUT_WSTT[i].u32 = upisp_clut_lut_wstt[i];
    }
}

/* dehaze_lut_wstt size:1280
   0~1024:dehaze prestat lut
        odd index: (prestat_lut >> 16) & 0x3ff
        even index:prestat_lut & 0x3ff
   1024~1280: dehaze strength lut */
static __inline hi_void isp_dehaze_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u16 blk_num,
                                                  hi_u32 *prestat_wstt, hi_u8 *lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < blk_num; i++) {
        be_lut_stt_reg->ISP_DEHAZE_LUT_WSTT[2 * i].bits.isp_dehaze_lut_wstt = prestat_wstt[i] & 0x3ff; /* addr:2 * i */
        be_lut_stt_reg->ISP_DEHAZE_LUT_WSTT[2 * i + 1].bits.isp_dehaze_lut_wstt = /* addr:2 * i + 1 */
            (prestat_wstt[i] >> 16) & 0x3ff; /* odd index: (prestat_lut >> 16) & 0x3ff */
    }

    for (i = 2 * blk_num; i < 1024; i++) { /* [2 * blk_num, 1024] */
        be_lut_stt_reg->ISP_DEHAZE_LUT_WSTT[i].bits.isp_dehaze_lut_wstt = 0;
    }

    for (i = 1024; i < 1280; i++) { /* [1024, 1280] */
        be_lut_stt_reg->ISP_DEHAZE_LUT_WSTT[i].bits.isp_dehaze_lut_wstt = lut_wstt[i - 1024]; /* sub 1024 */
    }
}

/* bnr rlsc lut size: 65 * 4;
   lut0_depth:65(addr:4 * i, 0<=i<65), lut0_value = (gr[2 * i] << 16) + r[2 * i]
   lut1_depth:65(addr:4 * i + 1, 0<=i<65), lut1_value = (gr[2 * i + 1] <<16) + r[2 * i + 1]
   lut2_depth:65(addr:4 * i + 2, 0<=i<65), lut2_value = (b[2 * i] << 16) + gb[2 * i]
   lut3_depth:65(addr:4 * i + 3, 0<=i<65), lut3_value = (b[2 * i + 1] << 16) + gb[2 * i + 1] */
static __inline hi_void isp_bnr_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u16 *bnr_lut0, hi_u16 *bnr_lut1,
                                               hi_u16 *bnr_lut2, hi_u16 *bnr_lut3)
{
    hi_s32 i;

    for (i = 0; i < 65; i++) { /* [0, 65) */
        be_lut_stt_reg->ISP_BNR_LUT_WSTT[4 * i].bits.isp_bnr_lut_wstt     = /* addr:4 * i */
            (((hi_u32)bnr_lut1[2 * i]) << 16) + bnr_lut0[2 * i]; /* lut0:(gr[2 * i] << 16) + r[2 * i] */
        be_lut_stt_reg->ISP_BNR_LUT_WSTT[4 * i + 1].bits.isp_bnr_lut_wstt = /* addr:4 * i + 1 */
            (((hi_u32)bnr_lut1[2 * i + 1]) << 16) + bnr_lut0[2 * i + 1]; /* lut1:(gr[2 * i + 1] << 16) + r[2 * i + 1] */
        be_lut_stt_reg->ISP_BNR_LUT_WSTT[4 * i + 2].bits.isp_bnr_lut_wstt = /* addr:4 * i + 2 */
            (((hi_u32)bnr_lut3[2 * i]) << 16) + bnr_lut2[2 * i]; /* lut2:(gr[2 * i + 2] << 16) + r[2 * i + 2] */
        be_lut_stt_reg->ISP_BNR_LUT_WSTT[4 * i + 3].bits.isp_bnr_lut_wstt = /* addr:4 * i + 3 */
            (((hi_u32)bnr_lut3[2 * i + 1]) << 16) + bnr_lut2[2 * i + 1]; /* lut3:(gr[2 * i + 3] << 16) + r[2 * i + 3] */
    }
}

/* Function    : iSetISP_GAMMA_LUT_WSTTisp_gamma_lut_wstt */
/* Description : Set the value of the member ISP_GAMMA_LUT_WSTT.isp_gamma_lut_wstt */
/* Input       : hi_u32 *upisp_gamma_lut_wstt: 12 bits */
static __inline hi_void isp_gamma_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u16 *upisp_gamma_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < GAMMA_REG_NODE_NUM; i++) {
        be_lut_stt_reg->ISP_GAMMA_LUT_WSTT[i].u32 = upisp_gamma_lut_wstt[i] & 0xFFF;
    }
}

/* Function    : iSetISP_SHARPEN_LUT_WSTTisp_sharpen_lut_wstt */
/* Description : Set the value of the member ISP_SHARPEN_LUT_WSTT.isp_sharpen_lut_wstt */
/* Input       : hi_u32 *upisp_sharpen_lut_wstt: 30 bits */
static __inline hi_void isp_sharpen_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u16 *mfgaind_wstt,
                                                   hi_u16 *mfgainud_wstt, hi_u16 *hfgaind_wstt, hi_u16 *hfgainud_wstt)
{
    hi_s32 i;

    for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
        be_lut_stt_reg->ISP_SHARPEN_LUT_WSTT[i * 4].bits.isp_sharpen_lut_wstt     = mfgaind_wstt[i];  /* 4 * i */
        be_lut_stt_reg->ISP_SHARPEN_LUT_WSTT[i * 4 + 1].bits.isp_sharpen_lut_wstt = mfgainud_wstt[i]; /* 4 * i + 1 */
        be_lut_stt_reg->ISP_SHARPEN_LUT_WSTT[i * 4 + 2].bits.isp_sharpen_lut_wstt = hfgaind_wstt[i];  /* 4 * i + 2 */
        be_lut_stt_reg->ISP_SHARPEN_LUT_WSTT[i * 4 + 3].bits.isp_sharpen_lut_wstt = hfgainud_wstt[i]; /* 4 * i + 3 */
    }
}

static __inline hi_void isp_dpc_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u32 *upisp_dpc_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < STATIC_DP_COUNT_NORMAL; i++) {
        be_lut_stt_reg->ISP_DPC_LUT_WSTT[i].u32 = upisp_dpc_lut_wstt[i];
    }
}

/* Function    : iSetISP_LSC_LUT_WSTTisp_lsc_lut_wstt */
/* Description : Set the value of the member ISP_LSC_LUT_WSTT.isp_lsc_lut_wstt */
/* Input       : hi_u32 *upisp_lsc_lut_wstt: 20 bits */
static __inline hi_void isp_lsc_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u32 *upisp_lsc_rgain_wstt, \
                                               hi_u32 *upisp_lsc_grgain_wstt, hi_u32 *upisp_lsc_gbgain_wstt,
                                               hi_u32 *upisp_lsc_bgain_wstt)
{
    hi_s32 i;

    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        be_lut_stt_reg->ISP_LSC_LUT_WSTT[4 * i + 0].u32 = upisp_lsc_rgain_wstt[i];  /* 4 * i: r_gain */
        be_lut_stt_reg->ISP_LSC_LUT_WSTT[4 * i + 1].u32 = upisp_lsc_grgain_wstt[i]; /* 4 * i + 1: gr_gain */
        be_lut_stt_reg->ISP_LSC_LUT_WSTT[4 * i + 2].u32 = upisp_lsc_bgain_wstt[i];  /* 4 * i + 2: b_gain */
        be_lut_stt_reg->ISP_LSC_LUT_WSTT[4 * i + 3].u32 = upisp_lsc_gbgain_wstt[i]; /* 4 * i + 3: gb_gain */
    }
}

/* Function    : iSetISP_CA_LUT_WSTTisp_ca_lut_wstt */
/* Description : Set the value of the member ISP_CA_LUT_WSTT.isp_ca_lut_wstt */
/* Input       : hi_u32 *upisp_ca_lut_wstt: 11 bits */
static __inline hi_void isp_ca_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u16 *upisp_ca_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        be_lut_stt_reg->ISP_CA_LUT_WSTT[i].u32 = upisp_ca_lut_wstt[i] & 0x7FF;
    }
}

/* ldci lut_wstt size: 65 * 4; order:
   4 * i:                 (cgain_lut << 20) + (statdrc_lut << 10) + calcdrc_lut
   4 * i + 1(for i < 33): (heneg_lut << 18) + (hepos_lut << 9) + delut_lut
   4 * i + 2(for i < 33): (ldci_usmneg_lut << 9) + ldci_usmpos_lut
   4 * i + 3            : 0 */
static __inline hi_void isp_ldci_drc_cgain_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_s16 *calcdrc_lut,
                                                          hi_s16 *statdrc_lut, hi_u32 *cgain_lut)
{
    hi_u32 ldci_statdrc;
    hi_s32 i;

    for (i = 0; i < 65; i++) { /* [0, 65) */
        ldci_statdrc = statdrc_lut[i];
        be_lut_stt_reg->ISP_LDCI_LUT_WSTT[4 * i].u32 =  /* addr:4 * i */
            (cgain_lut[i] << 20) + (ldci_statdrc << 10) + calcdrc_lut[i]; /* 20~29:cgain, 10~19:statdrc */
        be_lut_stt_reg->ISP_LDCI_LUT_WSTT[4 * i + 3].u32 = 0;  /* addr:4 * i + 3 */
    }
}

static __inline hi_void isp_ldci_he_delut_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg, hi_u32 *delut_lut_wstt,
                                                         hi_u32 *hepos_lut_wstt, hi_u32 *heneg_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < 33; i++) { /* [0, 33) */
        be_lut_stt_reg->ISP_LDCI_LUT_WSTT[4 * i + 1].u32 =   /* addr:4 * i + 1 */
            (heneg_lut_wstt[i] << 18) + (hepos_lut_wstt[i] << 9) + delut_lut_wstt[i]; /* 18~26:heneg, 9~17:hepos */
    }

    for (i = 33; i < 65; i++) { /* [33, 65) */
        be_lut_stt_reg->ISP_LDCI_LUT_WSTT[4 * i + 1].u32 = 0;  /* addr:4 * i + 1 */
    }
}

static __inline hi_void isp_ldci_usm_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg,
                                                    hi_u32 *upisp_ldci_usmpos_lut_wstt,
                                                    hi_u32 *upisp_ldci_usmneg_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < 33; i++) { /* [0, 33) */
        be_lut_stt_reg->ISP_LDCI_LUT_WSTT[4 * i + 2].u32 = /* addr:4 * i + 2 */
            (upisp_ldci_usmneg_lut_wstt[i] << 9) + upisp_ldci_usmpos_lut_wstt[i]; /* 9~17:usmneg_lut */
    }

    for (i = 33; i < 65; i++) { /* [33, 65) */
        be_lut_stt_reg->ISP_LDCI_LUT_WSTT[4 * i + 2].u32 = 0; /* addr:4 * i + 2 */
    }
}

/* Function    : iSetISP_PREGAMMA_LUT_WSTTisp_pregamma_lut_wstt */
/* Description : Set the value of the member ISP_PREGAMMA_LUT_WSTT.isp_pregamma_lut_wstt */
/* Input       : hi_u32 *upisp_pregamma_lut_wstt: 20 bits */
static __inline hi_void isp_pregamma_lut_wstt_write(S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg,
                                                    hi_u32 *upisp_pregamma_lut_wstt)
{
    hi_s32 i;

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) { /* pregamma lut size is 257 */
        be_lut_stt_reg->ISP_PREGAMMA_LUT_WSTT[i].u32 = upisp_pregamma_lut_wstt[i] & 0x1FFFFF;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
