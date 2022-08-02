/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_CONFIG_H__
#define __ISP_CONFIG_H__
#include "mpp_debug.h"
#include "isp_debug.h"
#include "isp_vreg.h"
#include "isp_reg_define.h"
#include "isp_main.h"
#include "isp_regcfg.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
#define ISP_FE_FIX_TIMING_STAT                     (0x40)
#define ISP_CLIP_Y_MIN_DEFAULT                        (0x0)
#define ISP_CLIP_Y_MAX_DEFAULT                        (0xFFFF)
#define ISP_CLIP_C_MIN_DEFAULT                        (0x0)
#define ISP_CLIP_C_MAX_DEFAULT                        (0xFFFF)
#define ISP_CSC_SUM_EN_DEFAULT                        (1)
#define ISP_YUV444_SUM_EN_DEFAULT                     (1)
#define ISP_YUV422_SUM_EN_DEFAULT                     (1)
#define ISP_WDR_SUM_EN_DEFAULT                        (1)
#define ISP_DEMOSAIC_SUM_EN_DEFAULT                   (1)
#define ISP_DMNR_DITHER_ROUND_DEFAULT              2
#define ISP_DMNR_DITHER_SPATIAL_MODE_DEFAULT       1
#define ISP_DMNR_DITHER_OUT_BITS_DEFAULT           (10)
#define ISP_ACM_DITHER_ROUND_DEFAULT               2
#define ISP_ACM_DITHER_SPATIAL_MODE_DEFAULT        1
#define ISP_ACM_DITHER_OUT_BITS_DEFAULT            (8)
#define ISP_DRC_DITHER_ROUND_DEFAULT               (2)
#define ISP_DRC_DITHER_SPATIAL_MODE_DEFAULT        (1)
#define ISP_DRC_DITHER_OUT_BITS_DEFAULT            (12)
#define ISP_SQRT1_DITHER_OUT_BITS_DEFAULT          (12)
#define ISP_BLK_B_HBLANK_DEFAULT                   0x20
#define ISP_BLK_F_HBLANK_DEFAULT                   0x18
#define ISP_BLK_B_VBLANK_DEFAULT                   0x0
#define ISP_BLK_F_VBLANK_DEFAULT                   0x0
#define ISP_AE_WEI_COMBIN_COUNT                    4
#define ISP_BE_AF_LPF_EN_DEFAULT                   (0)
#define ISP_BE_AF_LPF_EN_DATASIZE                  (1)
#define ISP_BE_AF_IIR0_DS_EN_DEFAULT               (0x1)
#define ISP_BE_AF_IIR0_DS_EN_DATASIZE              (1)
#define ISP_BE_AF_IIR1_DS_EN_DEFAULT               (0x1)
#define ISP_BE_AF_IIR1_DS_EN_DATASIZE              (1)
#define ISP_BE_AF_FIR0_LPF_EN_DEFAULT              (0x1)
#define ISP_BE_AF_FIR0_LPF_EN_DATASIZE             (1)
#define ISP_BE_AF_FIR1_LPF_EN_DEFAULT              (0x1)
#define ISP_BE_AF_FIR1_LPF_EN_DATASIZE             (1)
#define ISP_BE_AF_GAMMA_DEFAULT                    0x3
#define ISP_BE_AF_IIR0_PLG_DEFAULT                 (0x96)
#define ISP_BE_AF_IIR0_PLG_DATASIZE                (8)
#define ISP_BE_AF_IIR0_PLS_DEFAULT                 (0x3)
#define ISP_BE_AF_IIR0_PLS_DATASIZE                (3)
#define ISP_BE_AF_IIR1_PLG_DEFAULT                 (0x99)
#define ISP_BE_AF_IIR1_PLG_DATASIZE                (8)
#define ISP_BE_AF_IIR1_PLS_DEFAULT                 (0x3)
#define ISP_BE_AF_IIR1_PLS_DATASIZE                (3)
#define ISP_AF_CNT_SHIFT0_V_DEFAULT                   (0x2)
#define ISP_BE_AF_IIR0_DILATE_DEFAULT              (0x0)
#define ISP_BE_AF_IIR0_DILATE_DATASIZE             (3)
#define ISP_BE_AF_IIR1_DILATE_DEFAULT              (0x0)
#define ISP_BE_AF_IIR1_DILATE_DATASIZE             (3)
#define ISP_GCAC_PARAM_RED_A_DEFAULT               (3)
#define ISP_GCAC_PARAM_BLUE_A_DEFAULT              (3)
#define ISP_GCAC_PARAM_RED_B_DEFAULT               (-10)
#define ISP_GCAC_PARAM_BLUE_B_DEFAULT              (-16)
#define ISP_GCAC_PARAM_RED_C_DEFAULT               (2)
#define ISP_GCAC_PARAM_BLUE_C_DEFAULT              (7)
#define ISP_GCAC_Y_NORM_SHIFT_DEFAULT              (6)
#define ISP_GCAC_Y_NORM_FACTOR_DEFAULT             29
#define ISP_GCAC_X_NORM_SHIFT_DEFAULT              (6)
#define ISP_GCAC_X_NORM_FACTOR_DEFAULT             (29)
#define ISP_GCAC_VER_FILT_EN_DEFAULT               (0)
#define ISP_GCAC_CHR_VER_MODE_DEFAULT              (1)
#define ISP_GCAC_CLIP_MODE_VER_DEFAULT             (2)
#define ISP_GCAC_CLIP_MODE_HOR_DEFAULT             (1)
#define ISP_GE_NPOFFSET_DEFAULT                    (16383)
#define ISP_GE_SENSITHR_DEFAULT                    (4800)
#define ISP_GE_THRESHOLD_DEFAULT                   (4800)
#define ISP_GE_SENSISLOPE_DEFAULT                  (13)
#define ISP_GE_SLOPE_DEFAULT                       (13)
#define ISP_GE_STRENGTH_DEFAULT                    (128)
#define ISP_GE_BIT_DEPTH_DEFAULT                   (0)
#define ISP_DPC_DEFAULT_ALPHA                      (0)
#define ISP_DPC_DEFAULT_MODE                       (0x23)
#define ISP_DPC_DEFAULT_OUTPUT_MODE                (0x3)
#define ISP_DPC_DEFAULT_SET_USE                    (0x1)
#define ISP_DPC_DEFAULT_METHODS_SET_1              (0x1F1F)
#define ISP_DPC_DEFAULT_METHODS_SET_2              (0x707)
#define ISP_DPC_DEFAULT_METHODS_SET_3              (0x1F1F)
#define ISP_DPC_DEFAULT_LINE_THRESH_1              (0x5454)
#define ISP_DPC_DEFAULT_LINE_MAD_FAC_1             (0x1B1B)
#define ISP_DPC_DEFAULT_PG_FAC_1                   (0x0808)
#define ISP_DPC_DEFAULT_RND_THRESH_1               (0x0A0A)
#define ISP_DPC_DEFAULT_RG_FAC_1                   (0x2626)
#define ISP_DPC_DEFAULT_LINE_THRESH_2              (0x2121)
#define ISP_DPC_DEFAULT_LINE_MAD_FAC_2             (0x1810)
#define ISP_DPC_DEFAULT_PG_FAC_2                   (0x0B0B)
#define ISP_DPC_DEFAULT_RND_THRESH_2               (0x0808)
#define ISP_DPC_DEFAULT_RG_FAC_2                   (0x0808)
#define ISP_DPC_DEFAULT_LINE_THRESH_3              (0x2020)
#define ISP_DPC_DEFAULT_LINE_MAD_FAC_3             (0x404)
#define ISP_DPC_DEFAULT_PG_FAC_3                   (0x0a0a)
#define ISP_DPC_DEFAULT_RND_THRESH_3               (0x0806)
#define ISP_DPC_DEFAULT_RG_FAC_3                   (0x0404)
#define ISP_DPC_DEFAULT_RO_LIMITS                  (0x09a5)
#define ISP_DPC_DEFAULT_RND_OFFS                   (0x0aaa)
#define ISP_DPCC_BPDATA_DEFAULT                    (16383)
#define ISP_DPC_DEFAULT_BPT_CTRL                   (0x3FFF0330)
#define ISP_DPC_DEFAULT_BPT_NUMBER                 (0)
#define ISP_DPC_DEFAULT_BPT_THRESH                 (0xff800080)
#define ISP_DPC_DEFAULT_SOFT_THR_MIN               (-15)
#define ISP_DPC_DEFAULT_SOFT_THR_MAX               (6)
#define ISP_DPC_DEFAULT_HARD_THR_ENABLE            (1)
#define ISP_DPC_DEFAULT_SOFT_RAKE_RATIO            (24)
#define ISP_DPC_DEFAULT_LINE_AVER_FAC_1            (128)
#define ISP_DPC_DEFAULT_LINE_DIFF_THR_1            (36)
#define ISP_DPC_DEFAULT_LINE_STD_THR_1             (80)
#define ISP_DPC_DEFAULT_LINE_AVER_FAC_2            (128)
#define ISP_DPC_DEFAULT_LINE_DIFF_THR_2            (36)
#define ISP_DPC_DEFAULT_LINE_STD_THR_2             (80)
#define ISP_DPC_DEFAULT_LINE_AVER_FAC_3            (128)
#define ISP_DPC_DEFAULT_LINE_DIFF_THR_3            (36)
#define ISP_DPC_DEFAULT_LINE_STD_THR_3             (80)
#define ISP_DPC_DEFAULT_LINE_AVER_FAC_4            (128)
#define ISP_DPC_DEFAULT_LINE_DIFF_THR_4            (36)
#define ISP_DPC_DEFAULT_LINE_STD_THR_4             (80)
#define ISP_DPC_DEFAULT_LINE_AVER_FAC_5            (128)
#define ISP_DPC_DEFAULT_LINE_DIFF_THR_5            (36)
#define ISP_DPC_DEFAULT_LINE_STD_THR_5             (80)
#define ISP_DPC_DEFAULT_LINE_KERDIFF_FAC           (64)
#define ISP_DPC_DEFAULT_BLEND_MODE                 (0)
#define ISP_DPC_DEFAULT_BIT_DEPTH_SEL              (0)
#define ISP_DPC_DEFAULT_POS_LINEAR                 (1)
#define ISP_DPC_DEFAULT_POS_WDR                    (0)
#define ISP_DEMOSAIC_AND_EN_DEFAULT                1
#define ISP_GCAC_BLEND_SELECTION_DEFAULT           0
#define ISP_DEMOSAIC_FCR_EN_DEFAULT                1
#define ISP_DEMOSAIC_DE_FAKE_EN_DEFAULT            1
#define ISP_DEMOSAIC_DESAT_ENABLE_DEFAULT          (1)
#define ISP_DEMOSAIC_BLENDLIMIT2_DEFAULT           (40)
#define ISP_DEMOSAIC_BLENDLIMIT1_DEFAULT           (24)
#define ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT            0xA0
#define ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT            0x10
#define ISP_DEMOSAIC_AHDPART2_DEFAULT              (8)
#define ISP_DEMOSAIC_AHDPART1_DEFAULT              (400)
#define ISP_DEMOSAIC_FCR_GAIN_DEFAULT              8
#define ISP_DEMOSAIC_FCR_THR_DEFAULT               0x1
#define ISP_DEMOSAIC_HV_RATIO_DEFAULT              16
#define ISP_DEMOSAIC_FCR_RATIO_DEFAULT             8
#define ISP_DEMOSAIC_HV_SEL_DEFAULT                3
#define ISP_LCAC_COUNT_THR_R_DEFAULT               1
#define ISP_LCAC_COUNT_THR_B_DEFAULT               1
#define ISP_LCAC_COUNT_THR_G_DEFAULT               1
#define ISP_LCAC_PURPLE_DET_THR_DEFAULT            0
#define ISP_DEMOSAIC_LPF_F0_DEFAULT                0xF
#define ISP_DEMOSAIC_LPF_F1_DEFAULT                0x7
#define ISP_DEMOSAIC_LPF_F2_DEFAULT                0x14
#define ISP_DEMOSAIC_LPF_F3_DEFAULT                0x0
#define ISP_DEMOSAIC_G_CLIP_SFT_BIT_DEFAULT        1
#define ISP_DEMOSAIC_G_INTP_CONTROL_DEFAULT        0x1
#define ISP_DEMOSAIC_CBCR_AVG_THLD_DEFAULT         0x96
#define ISP_DEMOSAIC_CC_HF_MIN_RATIO_DEFAULT       0x0
#define ISP_DEMOSAIC_CC_HF_MAX_RATIO_DEFAULT       0x8
#define ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT        130
#define ISP_LCAC_SATU_THR_DEFAULT                  128
#define ISP_DEMOSAIC_FCR_GRAY_RATIO_DEFAULT        0x4
#define ISP_DEMOSAIC_FCR_DETG_SEL_DEFAULT          0x4
#define ISP_DEMOSAIC_FCR_CMAX_SEL_DEFAULT          0x4
#define ISP_DEMOSAIC_CC_VAR_THRESH_DEFAULT         (-1)
#define ISP_DEMOSAIC_HF_INPT_BLUR_TH_LOW_DEFAULT   0X0
#define ISP_DEMOSAIC_HF_INPT_BLUR_TH_HIGH_DEFAULT  0X40
#define ISP_DEMOSAIC_HF_INPT_BLUR_RATIO_DEFAULT    (0)
#define ISP_DEMOSAIC_FAKECR_VAR_THR_LOW_DEFAULT    (0)
#define ISP_DEMOSAIC_FAKECR_VAR_THR_HIGH_DEFAULT   (300)
#define ISP_DEMOSAIC_CB_THR_DEFAULT                (400)
#define ISP_DEMOSAIC_LUMA_THR_DEFAULT              (1500)
#define ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR_DEFAULT (1)
#define ISP_DEMOSAIC_CAC_CB_CNT_LOW_THR_DEFAULT    (55)
#define ISP_DEMOSAIC_CAC_CB_CNT_HIGH_THR_DEFAULT   (68)
#define ISP_DEMOSAIC_DEFCOLOR_CR_DEFAULT           (8)
#define ISP_DEMOSAIC_DEFCOLOR_CB_DEFAULT           (8)
#define ISP_DEMOSAIC_CAC_BLD_AVG_CUR_DEFAULT       (8)
#define ISP_DEMOSAIC_FCR_HF_THRESH_LOW_DEFAULT     (48)
#define ISP_DEMOSAIC_FCR_HF_THRESH_HIGH_DEFAULT    (128)
#define ISP_DEMOSAIC_DESAT_THRESH1_DEFAULT         (96)
#define ISP_DEMOSAIC_DESAT_THRESH2_DEFAULT         (192)
#define ISP_DEMOSAIC_DESAT_HIG_DEFAULT             (256)
#define ISP_DEMOSAIC_DESAT_LOW_DEFAULT             (166)
#define ISP_DEMOSAIC_DESAT_RATIO_DEFAULT           (-15)
#define ISP_DEMOSAIC_DESAT_PROTECT_SL_DEFAULT      (180)
#define ISP_DEMOSAIC_DESAT_PROTECT_TH_DEFAULT      (128)
#define ISP_DEMOSAIC_COLORNOISE_STR_MAX            0x8
#define ISP_NDDM_GF_TH_LOW_DEFAULT                 0
#define ISP_NDDM_GF_TH_HIGH_DEFAULT                272
#define ISP_NDDM_AWB_GF_GN_LOW_DEFAULT             0x0
#define ISP_NDDM_AWB_GF_GN_HIGH_DEFAULT            0x0
#define ISP_NDDM_AWB_GF_GN_MAX_DEFAULT             0x0
#define ISP_NDDM_FCR_GF_GAIN_DEFAULT               0x4
#define ISP_NDDM_FCR_DET_LOW_DEFAULT               150
#define ISP_NDDM_CAC_BLEND_RATE_DEFAULT            256
#define ISP_NDDM_EHC_GRAY_DEFAULT                  0x10
#define ISP_DITHER_MASK_DEFAULT                    0x1f
#define ISP_DITHER_RATIO_DEFAULT                   0x80
#define ISP_DITH_MAX_DEFAULT                       0x0
#define ISP_NDDM_COEF_K_DEFAULT                    (0x80)
#define ISP_NDDM_COEF_B_DEFAULT                    (0x10)
#define ISP_BNR_DEFAULT_C_RATIO_R                  0
#define ISP_BNR_DEFAULT_C_RATIO_GR                 0
#define ISP_BNR_DEFAULT_C_RATIO_GB                 0
#define ISP_BNR_DEFAULT_C_RATIO_B                  0
#define ISP_BNR_DEFAULT_AMED_MODE_R                0
#define ISP_BNR_DEFAULT_AMED_MODE_GR               0
#define ISP_BNR_DEFAULT_AMED_MODE_GB               0
#define ISP_BNR_DEFAULT_AMED_MODE_B                0
#define ISP_BNR_DEFAULT_AMED_LEVEL_R               1
#define ISP_BNR_DEFAULT_AMED_LEVEL_GR              0
#define ISP_BNR_DEFAULT_AMED_LEVEL_GB              0
#define ISP_BNR_DEFAULT_AMED_LEVEL_B               1
#define ISP_BNR_DEFAULT_JNLM_CORING_HIGH           0x300
#define ISP_BNR_DEFAULT_JNLM_GAIN                  0x52
#define ISP_BNR_DEFAULT_JNLM_SEL                   0x3
#define ISP_BNR_DEFAULT_JNLM_MAX_WT_COEF           0x80
#define ISP_BNR_DEFAULT_LUMA_SEL                   0x1
#define ISP_BNR_DEFAULT_JNLM_SYMCOEF_LINEAR        64
#define ISP_BNR_DEFAULT_JNLM_SYMCOEF_WDR           64
#define ISP_BNR_DEFAULT_RLMT_BLC                   0x10
#define ISP_BNR_DEFAULT_RLMT_RGAIN                 0x100
#define ISP_BNR_DEFAULT_RLMT_BGAIN                 0x100
#define ISP_BNR_DE_NEGCLIP_DEFAULT                 (0x7F)
#define ISP_BNR_DE_POSCLIP_DEFAULT                 (0x7F)
#define ISP_BNR_DEFAULT_LINEAR_WDRMAP_FLTMODE      (0x0)
#define ISP_BNR_DEFAULT_WDR_WDRMAP_FLTMODE         (0x1)
#define ISP_BNR_DEFAULT_LINEAR_WDRMAP_GAIN         (0x0)
#define ISP_BNR_DEFAULT_WDR_WDRMAP_GAIN            (0x3)
#define ISP_BNR_DEFAULT_SAD_WINSIZE_SEL            0x0
#define ISP_BNR_DEFAULT_SHOT_RATIO                 0x146
#define ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR  0x0
#define ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_WDR     0x800
#define ISP_BNR_DEFAULT_ENABLE_POST_PROC           (3)
#define ISP_BNR_DEFAULT_SYMMSAD                    (1)
#define ISP_BNR_DEFAULT_IMPLS_NR_STRENGTH          (0x40)
#define ISP_BNR_DEFAULT_IMPLS_NR_ENABLE3           (1)
#define ISP_BNR_DEFAULT_IMPLS_NR_ENABLE2           (1)
#define ISP_BNR_DEFAULT_IMPLS_NR_ENABLE1           (1)
#define ISP_BNR_DEFAULT_GAIN_SAD                   (0x40)
#define ISP_BNR_DEFAULT_PATTERN_NOISE_REDUCTION    (1)
#define ISP_BNR_DEFAULT_WINDOW_SIZE_SEL            (3)
#define ISP_BNR_DEFAULT_NLIMIT_GAIN_BLT            (200)
#define ISP_BNR_DEFAULT_BILATERAL_ENABLE           (2)
#define ISP_WDR_FUSION_DATA_MODE                   (0x2)
#define ISP_FUSION_MODE_DEFAULT                    0
#define ISP_WDR_EROSION_EN_DEFAULT                 (1)
#define ISP_WDR_MDT_EN_DEFAULT                     (1)
#define ISP_WDR_GRAYSCALE_DEFAULT                  0
#define ISP_WDR_SAVE_BLC_EN_DEFAULT                1
#define ISP_WDR_SATURATE_THR_DEFAULT               3777
#define ISP_WDR_STILL_THR0_DEFAULT                 0
#define ISP_WDR_MDTL_BLD_DEFAULT                   (0)
#define ISP_WDR_MDT_FULL_THR_DEFAULT               250
#define ISP_WDR_MDT_STILL_THR_DEFAULT              (20)
#define ISP_WDR_FORCELONG_HIGH_THD_DEFAULT         (1600)
#define ISP_WDR_FORCELONG_LOW_THD_DEFAULT          (1024)
#define ISP_WDR_FORCELONG_SMOOTH_EN                (0x1)
#define ISP_WDR_FORCELONG_EN_DEFAULT               (0)
#define ISP_WDR_FORCELONG_SLOPE_DEFAULT            (0x51)
#define ISP_WDR_MDT_NOSF_HIG_THR_DEFAULT           (0xbd)
#define ISP_WDR_MDT_NOSF_LOW_THR_DEFAULT           (0x7e)
#define ISP_WDR_GAIN_SUM_HIG_THR_DEFAULT           (0x3)
#define ISP_WDR_GAIN_SUM_LOW_THR_DEFAULT           (0x2)
#define ISP_WDR_SHORTCHK_THD_DEFAULT                  (12)
#define ISP_WDR_SHORT_EXPO_CHK_DEFAULT             (1)
#define ISP_WDR_PIXEL_AVG_MAX_DIFF_DEFAULT         (70)
#define ISP_WDR_MASK_SIMILAR_CNT_DEFAULT           (3)
#define ISP_WDR_MASK_SIMILAR_THR_DEFAULT           (20)
#define ISP_WDR_BLDRLHFIDX_DEFAULT                 (4)
#define ISP_WDR_LONG_THR_DEFAULT                   (3008)
#define ISP_WDR_SHORT_THR_DEFAULT                  (4032)
#define ISP_WDR_DFTWGT_FL_DEFAULT                  (256)
#define ISP_WDR_WGT_SLOPE_DEFAULT                  (0x10)
#define ISP_WDR_FUSION_F0_THR_R                    (3500)
#define ISP_WDR_FUSION_F1_THR_B                    (3000)
#define ISP_WDR_FUSION_F0_THR_B                    (3500)
#define ISP_WDR_FUSION_F1_THR_G                    (3000)
#define ISP_WDR_FUSION_F0_THR_G                    (3500)
#define ISP_WDR_FUSION_SATURATE_THD                (0xed1)
#define ISP_CA_LUMA_THD_LOW_DEFAULT                (250)
#define ISP_CA_DARKCHROMA_THD_HIGH_DEFAULT         (138*4)
#define ISP_CA_DARKCHROMA_THD_LOW_DEFAULT          (135*4)
#define ISP_CA_SDARKCHROMA_THD_HIGH_DEFAULT        (140*4)
#define ISP_CA_SDARKCHROMA_THD_LOW_DEFAULT         (135*4)
#define ISP_CA_LUMA_RATIO_LOW_DEFAULT              (800)
#define ISP_CA_Y_LUMA_RATIO_DEFAULT                (10)
#define ISP_CA_SKIN_BETA_RATIO_DEFAULT             (128)
#define ISP_CA_SKINLOWLUAM_UMAX_DEFAULT            (128*4)
#define ISP_CA_SKINLOWLUAM_UMIN_DEFAULT            (115*4)
#define ISP_CA_SKINLOWLUAM_UYMAX_DEFAULT           (400)
#define ISP_CA_SKINLOWLUAM_UYMIN_DEFAULT           (64)
#define ISP_CA_SKINHIGHLUAM_UMAX_DEFAULT           (128*4)
#define ISP_CA_SKINHIGHLUAM_UMIN_DEFAULT           (96*4)
#define ISP_CA_SKINHIGHLUAM_UYMAX_DEFAULT          (960)
#define ISP_CA_SKINHIGHLUAM_UYMIN_DEFAULT          (400)
#define ISP_CA_SKINLOWLUAM_VMAX_DEFAULT            (156*4+2)
#define ISP_CA_SKINLOWLUAM_VMIN_DEFAULT            (130*4)
#define ISP_CA_SKINLOWLUAM_VYMAX_DEFAULT           (400)
#define ISP_CA_SKINLOWLUAM_VYMIN_DEFAULT           (64)
#define ISP_CA_SKINHIGHLUAM_VMAX_DEFAULT           (156*4+2)
#define ISP_CA_SKINHIGHLUAM_VMIN_DEFAULT           (130*4)
#define ISP_CA_SKINHIGHLUAM_VYMAX_DEFAULT          (831)
#define ISP_CA_SKINHIGHLUAM_VYMIN_DEFAULT          (400)
#define ISP_CA_SKINUVDIFF_DEFAULT                  (10*4)
#define ISP_CA_SKINRATIOTHD_MID_DEFAULT            (1300)
#define ISP_CA_SKINRATIOTHD_LOW_DEFAULT            (900)
#define ISP_CA_SKINRATIOTHD_HIGH_DEFAULT           (1500)
#define ISP_DEMOSAIC_CFG_EN_DEFAULT                1
#define ISP_NDDM_CFG_EN_DEFAULT                    1
#define ISP_DEHAZE_LUT_WIDTH_WORD_DEFAULT          (160) // 640/4
#define ISP_GAMMA_LUT_WIDTH_WORD_DEFAULT           (65)   // 257/4
#define ISP_SHARPEN_LUT_WIDTH_WORD_DEFAULT         (32) // 128/4
#define ISP_MLSC_LUT_WIDTH_WORD_DEFAULT            (289) // 1156/4
#define ISP_LDCI_LUT_WIDTH_WORD_DEFAULT            (65)
#define ISP_CA_LUT_WIDTH_WORD_DEFAULT              (32)
#define ISP_BNR_LUT_WIDTH_WORD_DEFAULT             (65)

#define PFND(b) printf("%s = %d \n",#b,b);

/******************************************************************************
*******************************************************************************
*******************************************************************************
******************** FE FE FE FE FE FE FE FE FE************************************
*******************************************************************************
*******************************************************************************
******************************************************************************/
static __inline GK_VOID isp_fe_user_define0_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_user_define0)
{
    pstFeReg->ISP_FE_USER_DEFINE0.u32 = uisp_fe_user_define0;
}
static __inline  GK_VOID isp_fe_user_define1_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_user_define1)
{
    pstFeReg->ISP_FE_USER_DEFINE1.u32 = uisp_fe_user_define1;
}
static __inline  GK_VOID isp_fe_format_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_format)
{
    pstFeReg->ISP_FE_FORMAT.u32 = uisp_fe_format;
}
static __inline  GK_VOID isp_fe_int_mask_fstart_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_fstart)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_fstart = uisp_fe_int_mask_fstart;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_int_mask_update_cfg_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_update_cfg)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_update_cfg = uisp_fe_int_mask_update_cfg;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_int_mask_cfg_loss_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_cfg_loss)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_cfg_loss = uisp_fe_int_mask_cfg_loss;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_int_mask_int_delay_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_int_delay)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_int_delay = uisp_fe_int_mask_int_delay;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_int_mask_ae1_int_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_ae1_int)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_ae1_int = uisp_fe_int_mask_ae1_int;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_int_mask_af1_int_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_af1_int)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_af1_int = uisp_fe_int_mask_af1_int;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_int_mask_dis1_int_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_int_mask_dis1_int)
{
    U_ISP_FE_INT_MASK o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = pstFeReg->ISP_FE_INT_MASK.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_dis1_int = uisp_fe_int_mask_dis1_int;
    pstFeReg->ISP_FE_INT_MASK.u32 = o_isp_fe_int_mask.u32;
}
static __inline  GK_VOID isp_fe_colorbar_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_colorbar_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_colorbar_en = uisp_colorbar_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_crop_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_crop_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_crop_en = uisp_crop_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_blc1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_blc1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_blc1_en = uisp_blc1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_rc_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_rc_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_rc_en = uisp_rc_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_hrs_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_hrs_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_hrs_en = uisp_hrs_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_rlsc1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_rlsc1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_rlsc1_en = uisp_rlsc1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_dg2_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_dg2_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_dg2_en = uisp_dg2_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_dis1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_dis1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_dis1_en = uisp_dis1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_af1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_af1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_af1_en = uisp_af1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_wb1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_wb1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_wb1_en = uisp_wb1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_drcs_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_drcs_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_drcs_en = uisp_drcs_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_ae1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_ae1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_ae1_en = uisp_ae1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_loglut1_en_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_loglut1_en)
{
    U_ISP_FE_CTRL o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    o_isp_fe_ctrl.bits.isp_loglut1_en = uisp_loglut1_en;
    pstFeReg->ISP_FE_CTRL.u32 = o_isp_fe_ctrl.u32;
}
static __inline  GK_VOID isp_fe_hsync_mode_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_hsync_mode)
{
    U_ISP_FE_ADAPTER_CFG o_isp_fe_adapter_cfg;
    if (!(uisp_fe_hsync_mode == 0)) {
        return;
    }
    o_isp_fe_adapter_cfg.u32 = pstFeReg->ISP_FE_ADAPTER_CFG.u32;
    o_isp_fe_adapter_cfg.bits.fe_hsync_mode = uisp_fe_hsync_mode;
    pstFeReg->ISP_FE_ADAPTER_CFG.u32 = o_isp_fe_adapter_cfg.u32;
}
static __inline  GK_VOID isp_fe_vsync_mode_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_vsync_mode)
{
    U_ISP_FE_ADAPTER_CFG o_isp_fe_adapter_cfg;
    if (!(uisp_fe_vsync_mode == 0)) {
        return;
    }
    o_isp_fe_adapter_cfg.u32 = pstFeReg->ISP_FE_ADAPTER_CFG.u32;
    o_isp_fe_adapter_cfg.bits.fe_vsync_mode = uisp_fe_vsync_mode;
    pstFeReg->ISP_FE_ADAPTER_CFG.u32 = o_isp_fe_adapter_cfg.u32;
}
static __inline  GK_VOID isp_fe_delay_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_delay)
{
    pstFeReg->ISP_FE_FSTART_DELAY.u32 = uisp_fe_delay;
}
static __inline  GK_VOID isp_fe_rggb_cfg_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_rggb_cfg)
{
    U_ISP_FE_CTRL_F o_isp_fe_ctrl_f;
    o_isp_fe_ctrl_f.bits.isp_fe_rggb_cfg = uisp_fe_rggb_cfg;
    o_isp_fe_ctrl_f.bits.reserved_0 = 0;
    pstFeReg->ISP_FE_CTRL_F.u32 = o_isp_fe_ctrl_f.u32;
}
static __inline  GK_VOID isp_fe_update_mode_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_update_mode)
{
    U_ISP_FE_CTRL_I o_isp_fe_ctrl_i;
    o_isp_fe_ctrl_i.bits.isp_fe_update_mode = uisp_fe_update_mode;
    o_isp_fe_ctrl_i.bits.reserved_0 = 0;
    pstFeReg->ISP_FE_CTRL_I.u32 = o_isp_fe_ctrl_i.u32;
}
#define ISP_FE_FIX_TIMING_STAT                     (0x40)
static __inline  GK_VOID isp_fe_fix_timing_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_fix_timing)
{
    U_ISP_FE_TIMING_CFG o_isp_fe_timing_cfg;
    o_isp_fe_timing_cfg.bits.isp_fe_fix_timing = uisp_fe_fix_timing;
    o_isp_fe_timing_cfg.bits.reserved_0 = 0;
    pstFeReg->ISP_FE_TIMING_CFG.u32 = o_isp_fe_timing_cfg.u32;
}
static __inline  GK_VOID isp_fe_update_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_update)
{
    U_ISP_FE_REG_UPDATE o_isp_fe_reg_update;
    o_isp_fe_reg_update.bits.isp_fe_update = uisp_fe_update;
    o_isp_fe_reg_update.bits.reserved_0 = 0;
    pstFeReg->ISP_FE_REG_UPDATE.u32 = o_isp_fe_reg_update.u32;
}
static __inline  GK_VOID isp_fe_width_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_width)
{
    U_ISP_FE_SIZE o_isp_fe_size;
    o_isp_fe_size.u32 = pstFeReg->ISP_FE_SIZE.u32;
    o_isp_fe_size.bits.isp_fe_width = uisp_fe_width;
    pstFeReg->ISP_FE_SIZE.u32 = o_isp_fe_size.u32;
}
static __inline  GK_VOID isp_fe_height_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_height)
{
    U_ISP_FE_SIZE o_isp_fe_size;
    o_isp_fe_size.u32 = pstFeReg->ISP_FE_SIZE.u32;
    o_isp_fe_size.bits.isp_fe_height = uisp_fe_height;
    pstFeReg->ISP_FE_SIZE.u32 = o_isp_fe_size.u32;
}
static __inline  GK_VOID isp_fe_blk_f_hblank_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uisp_fe_blk_f_hblank)
{
    U_ISP_FE_BLK_HBLANK o_isp_fe_blk_hblank;
    o_isp_fe_blk_hblank.u32 = pstFeReg->ISP_FE_BLK_HBLANK.u32;
    o_isp_fe_blk_hblank.bits.isp_fe_blk_f_hblank = uisp_fe_blk_f_hblank;
    pstFeReg->ISP_FE_BLK_HBLANK.u32 = o_isp_fe_blk_hblank.u32;
}


static __inline  GK_VOID isp_blc1_ofset1_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_blc1_ofset1)
{
    pstFeReg->ISP_BLC1_OFFSET1.u32 = uisp_blc1_ofset1;
}

static __inline  GK_VOID isp_blc1_ofset2_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_blc1_ofset2)
{
    pstFeReg->ISP_BLC1_OFFSET2.u32 = uisp_blc1_ofset2;
}

static __inline  GK_VOID isp_dg2_en_in_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_en_in)
{
    U_ISP_DG2_BLC_CFG o_isp_dg2_blc_cfg;
    o_isp_dg2_blc_cfg.u32 = pstFeReg->ISP_DG2_BLC_CFG.u32;
    o_isp_dg2_blc_cfg.bits.isp_dg2_en_in = uisp_dg2_en_in;
    pstFeReg->ISP_DG2_BLC_CFG.u32 = o_isp_dg2_blc_cfg.u32;
}
static __inline  GK_VOID isp_dg2_en_out_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_en_out)
{
    U_ISP_DG2_BLC_CFG o_isp_dg2_blc_cfg;
    o_isp_dg2_blc_cfg.u32 = pstFeReg->ISP_DG2_BLC_CFG.u32;
    o_isp_dg2_blc_cfg.bits.isp_dg2_en_out = uisp_dg2_en_out;
    pstFeReg->ISP_DG2_BLC_CFG.u32 = o_isp_dg2_blc_cfg.u32;
}
static __inline  GK_VOID isp_dg2_grgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_grgain)
{
    U_ISP_DG2_GAIN1 o_isp_dg2_gain1;
    o_isp_dg2_gain1.u32 = pstFeReg->ISP_DG2_GAIN1.u32;
    o_isp_dg2_gain1.bits.isp_dg2_grgain = uisp_dg2_grgain;
    pstFeReg->ISP_DG2_GAIN1.u32 = o_isp_dg2_gain1.u32;
}
static __inline  GK_VOID isp_dg2_rgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_rgain)
{
    U_ISP_DG2_GAIN1 o_isp_dg2_gain1;
    o_isp_dg2_gain1.u32 = pstFeReg->ISP_DG2_GAIN1.u32;
    o_isp_dg2_gain1.bits.isp_dg2_rgain = uisp_dg2_rgain;
    pstFeReg->ISP_DG2_GAIN1.u32 = o_isp_dg2_gain1.u32;
}
static __inline  GK_VOID isp_dg2_gbgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_gbgain)
{
    U_ISP_DG2_GAIN2 o_isp_dg2_gain2;
    o_isp_dg2_gain2.u32 = pstFeReg->ISP_DG2_GAIN2.u32;
    o_isp_dg2_gain2.bits.isp_dg2_gbgain = uisp_dg2_gbgain;
    pstFeReg->ISP_DG2_GAIN2.u32 = o_isp_dg2_gain2.u32;
}
static __inline  GK_VOID isp_dg2_bgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_bgain)
{
    U_ISP_DG2_GAIN2 o_isp_dg2_gain2;
    o_isp_dg2_gain2.u32 = pstFeReg->ISP_DG2_GAIN2.u32;
    o_isp_dg2_gain2.bits.isp_dg2_bgain = uisp_dg2_bgain;
    pstFeReg->ISP_DG2_GAIN2.u32 = o_isp_dg2_gain2.u32;
}
static __inline  GK_VOID isp_dg2_ofsgr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_ofsgr)
{
    U_ISP_DG2_BLC_OFFSET1 o_isp_dg2_blc_offset1;
    o_isp_dg2_blc_offset1.u32 = pstFeReg->ISP_DG2_BLC_OFFSET1.u32;
    o_isp_dg2_blc_offset1.bits.isp_dg2_ofsgr = uisp_dg2_ofsgr;
    pstFeReg->ISP_DG2_BLC_OFFSET1.u32 = o_isp_dg2_blc_offset1.u32;
}
static __inline  GK_VOID isp_dg2_ofsr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_ofsr)
{
    U_ISP_DG2_BLC_OFFSET1 o_isp_dg2_blc_offset1;
    o_isp_dg2_blc_offset1.u32 = pstFeReg->ISP_DG2_BLC_OFFSET1.u32;
    o_isp_dg2_blc_offset1.bits.isp_dg2_ofsr = uisp_dg2_ofsr;
    pstFeReg->ISP_DG2_BLC_OFFSET1.u32 = o_isp_dg2_blc_offset1.u32;
}
static __inline  GK_VOID isp_dg2_ofsgb_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_ofsgb)
{
    U_ISP_DG2_BLC_OFFSET2 o_isp_dg2_blc_offset2;
    o_isp_dg2_blc_offset2.u32 = pstFeReg->ISP_DG2_BLC_OFFSET2.u32;
    o_isp_dg2_blc_offset2.bits.isp_dg2_ofsgb = uisp_dg2_ofsgb;
    pstFeReg->ISP_DG2_BLC_OFFSET2.u32 = o_isp_dg2_blc_offset2.u32;
}
static __inline  GK_VOID isp_dg2_ofsb_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_ofsb)
{
    U_ISP_DG2_BLC_OFFSET2 o_isp_dg2_blc_offset2;
    o_isp_dg2_blc_offset2.u32 = pstFeReg->ISP_DG2_BLC_OFFSET2.u32;
    o_isp_dg2_blc_offset2.bits.isp_dg2_ofsb = uisp_dg2_ofsb;
    pstFeReg->ISP_DG2_BLC_OFFSET2.u32 = o_isp_dg2_blc_offset2.u32;
}
static __inline  GK_VOID isp_dg2_clip_value_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_dg2_clip_value)
{
    U_ISP_DG2_CLIP_VALUE o_isp_dg2_clip_value;
    o_isp_dg2_clip_value.bits.isp_dg2_clip_value = uisp_dg2_clip_value;
    o_isp_dg2_clip_value.bits.reserved_0 = 0;
    pstFeReg->ISP_DG2_CLIP_VALUE.u32 = o_isp_dg2_clip_value.u32;
}
static __inline  GK_VOID isp_wb1_en_in_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_en_in)
{
    U_ISP_WB1_BLC_CFG o_isp_wb1_blc_cfg;
    o_isp_wb1_blc_cfg.u32 = pstFeReg->ISP_WB1_BLC_CFG.u32;
    o_isp_wb1_blc_cfg.bits.isp_wb1_en_in = uisp_wb1_en_in;
    pstFeReg->ISP_WB1_BLC_CFG.u32 = o_isp_wb1_blc_cfg.u32;
}
static __inline  GK_VOID isp_wb1_en_out_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_en_out)
{
    U_ISP_WB1_BLC_CFG o_isp_wb1_blc_cfg;
    o_isp_wb1_blc_cfg.u32 = pstFeReg->ISP_WB1_BLC_CFG.u32;
    o_isp_wb1_blc_cfg.bits.isp_wb1_en_out = uisp_wb1_en_out;
    pstFeReg->ISP_WB1_BLC_CFG.u32 = o_isp_wb1_blc_cfg.u32;
}
static __inline  GK_VOID isp_wb1_grgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_grgain)
{
    U_ISP_WB1_GAIN1 o_isp_wb1_gain1;
    o_isp_wb1_gain1.u32 = pstFeReg->ISP_WB1_GAIN1.u32;
    o_isp_wb1_gain1.bits.isp_wb1_grgain = uisp_wb1_grgain;
    pstFeReg->ISP_WB1_GAIN1.u32 = o_isp_wb1_gain1.u32;
}
static __inline  GK_VOID isp_wb1_rgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_rgain)
{
    U_ISP_WB1_GAIN1 o_isp_wb1_gain1;
    o_isp_wb1_gain1.u32 = pstFeReg->ISP_WB1_GAIN1.u32;
    o_isp_wb1_gain1.bits.isp_wb1_rgain = uisp_wb1_rgain;
    pstFeReg->ISP_WB1_GAIN1.u32 = o_isp_wb1_gain1.u32;
}
static __inline  GK_VOID isp_wb1_gbgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_gbgain)
{
    U_ISP_WB1_GAIN2 o_isp_wb1_gain2;
    o_isp_wb1_gain2.u32 = pstFeReg->ISP_WB1_GAIN2.u32;
    o_isp_wb1_gain2.bits.isp_wb1_gbgain = uisp_wb1_gbgain;
    pstFeReg->ISP_WB1_GAIN2.u32 = o_isp_wb1_gain2.u32;
}
static __inline  GK_VOID isp_wb1_bgain_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_bgain)
{
    U_ISP_WB1_GAIN2 o_isp_wb1_gain2;
    o_isp_wb1_gain2.u32 = pstFeReg->ISP_WB1_GAIN2.u32;
    o_isp_wb1_gain2.bits.isp_wb1_bgain = uisp_wb1_bgain;
    pstFeReg->ISP_WB1_GAIN2.u32 = o_isp_wb1_gain2.u32;
}
static __inline  GK_VOID isp_wb1_ofsgr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_ofsgr)
{
    U_ISP_WB1_BLC_OFFSET1 o_isp_wb1_blc_offset1;
    o_isp_wb1_blc_offset1.u32 = pstFeReg->ISP_WB1_BLC_OFFSET1.u32;
    o_isp_wb1_blc_offset1.bits.isp_wb1_ofsgr = uisp_wb1_ofsgr;
    pstFeReg->ISP_WB1_BLC_OFFSET1.u32 = o_isp_wb1_blc_offset1.u32;
}
static __inline  GK_VOID isp_wb1_ofsr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_ofsr)
{
    U_ISP_WB1_BLC_OFFSET1 o_isp_wb1_blc_offset1;
    o_isp_wb1_blc_offset1.u32 = pstFeReg->ISP_WB1_BLC_OFFSET1.u32;
    o_isp_wb1_blc_offset1.bits.isp_wb1_ofsr = uisp_wb1_ofsr;
    pstFeReg->ISP_WB1_BLC_OFFSET1.u32 = o_isp_wb1_blc_offset1.u32;
}
static __inline  GK_VOID isp_wb1_ofsgb_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_ofsgb)
{
    U_ISP_WB1_BLC_OFFSET2 o_isp_wb1_blc_offset2;
    o_isp_wb1_blc_offset2.u32 = pstFeReg->ISP_WB1_BLC_OFFSET2.u32;
    o_isp_wb1_blc_offset2.bits.isp_wb1_ofsgb = uisp_wb1_ofsgb;
    pstFeReg->ISP_WB1_BLC_OFFSET2.u32 = o_isp_wb1_blc_offset2.u32;
}
static __inline  GK_VOID isp_wb1_ofsb_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_ofsb)
{
    U_ISP_WB1_BLC_OFFSET2 o_isp_wb1_blc_offset2;
    o_isp_wb1_blc_offset2.u32 = pstFeReg->ISP_WB1_BLC_OFFSET2.u32;
    o_isp_wb1_blc_offset2.bits.isp_wb1_ofsb = uisp_wb1_ofsb;
    pstFeReg->ISP_WB1_BLC_OFFSET2.u32 = o_isp_wb1_blc_offset2.u32;
}
static __inline  GK_VOID isp_wb1_clip_value_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_wb1_clip_value)
{
    U_ISP_WB1_CLIP_VALUE o_isp_wb1_clip_value;
    o_isp_wb1_clip_value.bits.isp_wb1_clip_value = uisp_wb1_clip_value;
    o_isp_wb1_clip_value.bits.reserved_0 = 0;
    pstFeReg->ISP_WB1_CLIP_VALUE.u32 = o_isp_wb1_clip_value.u32;
}
static __inline  GK_VOID isp_ae1_hnum_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_hnum)
{
    U_ISP_AE1_ZONE o_isp_ae1_zone;
    o_isp_ae1_zone.u32 = pstFeReg->ISP_AE1_ZONE.u32;
    o_isp_ae1_zone.bits.isp_ae_hnum = uisp_ae1_hnum;
    pstFeReg->ISP_AE1_ZONE.u32 = o_isp_ae1_zone.u32;
}
static __inline  GK_VOID isp_ae1_vnum_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_vnum)
{
    U_ISP_AE1_ZONE o_isp_ae1_zone;
    o_isp_ae1_zone.u32 = pstFeReg->ISP_AE1_ZONE.u32;
    o_isp_ae1_zone.bits.isp_ae_vnum = uisp_ae1_vnum;
    pstFeReg->ISP_AE1_ZONE.u32 = o_isp_ae1_zone.u32;
}
static __inline  GK_VOID isp_ae1_skip_x_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_skip_x)
{
    U_ISP_AE1_SKIP_CRG o_isp_ae1_skip_crg;
    o_isp_ae1_skip_crg.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    o_isp_ae1_skip_crg.bits.isp_ae_skip_x = uisp_ae1_skip_x;
    pstFeReg->ISP_AE1_SKIP_CRG.u32 = o_isp_ae1_skip_crg.u32;
}
static __inline  GK_VOID isp_ae1_offset_x_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_offset_x)
{
    U_ISP_AE1_SKIP_CRG o_isp_ae1_skip_crg;
    o_isp_ae1_skip_crg.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    o_isp_ae1_skip_crg.bits.isp_ae_offset_x = uisp_ae1_offset_x;
    pstFeReg->ISP_AE1_SKIP_CRG.u32 = o_isp_ae1_skip_crg.u32;
}
static __inline  GK_VOID isp_ae1_skip_y_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_skip_y)
{
    U_ISP_AE1_SKIP_CRG o_isp_ae1_skip_crg;
    o_isp_ae1_skip_crg.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    o_isp_ae1_skip_crg.bits.isp_ae_skip_y = uisp_ae1_skip_y;
    pstFeReg->ISP_AE1_SKIP_CRG.u32 = o_isp_ae1_skip_crg.u32;
}
static __inline  GK_VOID isp_ae1_offset_y_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_offset_y)
{
    U_ISP_AE1_SKIP_CRG o_isp_ae1_skip_crg;
    o_isp_ae1_skip_crg.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    o_isp_ae1_skip_crg.bits.isp_ae_offset_y = uisp_ae1_offset_y;
    pstFeReg->ISP_AE1_SKIP_CRG.u32 = o_isp_ae1_skip_crg.u32;
}
static __inline  GK_VOID isp_ae1_bitmove_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_bitmove)
{
    U_ISP_AE1_BITMOVE o_isp_ae1_bitmove;
    o_isp_ae1_bitmove.u32 = pstFeReg->ISP_AE1_BITMOVE.u32;
    o_isp_ae1_bitmove.bits.isp_ae_bitmove = uisp_ae1_bitmove;
    pstFeReg->ISP_AE1_BITMOVE.u32 = o_isp_ae1_bitmove.u32;
}
static __inline  GK_VOID isp_ae1_blc_en_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_blc_en)
{
    U_ISP_AE1_BITMOVE o_isp_ae1_bitmove;
    o_isp_ae1_bitmove.u32 = pstFeReg->ISP_AE1_BITMOVE.u32;
    o_isp_ae1_bitmove.bits.isp_ae_blc_en = uisp_ae1_blc_en;
    pstFeReg->ISP_AE1_BITMOVE.u32 = o_isp_ae1_bitmove.u32;
}
static __inline  GK_VOID isp_ae1_offset_r_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_offset_r)
{
    U_ISP_AE1_OFFSET_R_GR o_isp_ae1_offset_r_gr;
    o_isp_ae1_offset_r_gr.u32 = pstFeReg->ISP_AE1_OFFSET_R_GR.u32;
    o_isp_ae1_offset_r_gr.bits.isp_ae_offset_r = uisp_ae1_offset_r;
    pstFeReg->ISP_AE1_OFFSET_R_GR.u32 = o_isp_ae1_offset_r_gr.u32;
}
static __inline  GK_VOID isp_ae1_offset_gr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_offset_gr)
{
    U_ISP_AE1_OFFSET_R_GR o_isp_ae1_offset_r_gr;
    o_isp_ae1_offset_r_gr.u32 = pstFeReg->ISP_AE1_OFFSET_R_GR.u32;
    o_isp_ae1_offset_r_gr.bits.isp_ae_offset_gr = uisp_ae1_offset_gr;
    pstFeReg->ISP_AE1_OFFSET_R_GR.u32 = o_isp_ae1_offset_r_gr.u32;
}
static __inline  GK_VOID isp_ae1_offset_gb_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_offset_gb)
{
    U_ISP_AE1_OFFSET_GB_B o_isp_ae1_offset_gb_b;
    o_isp_ae1_offset_gb_b.u32 = pstFeReg->ISP_AE1_OFFSET_GB_B.u32;
    o_isp_ae1_offset_gb_b.bits.isp_ae_offset_gb = uisp_ae1_offset_gb;
    pstFeReg->ISP_AE1_OFFSET_GB_B.u32 = o_isp_ae1_offset_gb_b.u32;
}
static __inline  GK_VOID isp_ae1_offset_b_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_offset_b)
{
    U_ISP_AE1_OFFSET_GB_B o_isp_ae1_offset_gb_b;
    o_isp_ae1_offset_gb_b.u32 = pstFeReg->ISP_AE1_OFFSET_GB_B.u32;
    o_isp_ae1_offset_gb_b.bits.isp_ae_offset_b = uisp_ae1_offset_b;
    pstFeReg->ISP_AE1_OFFSET_GB_B.u32 = o_isp_ae1_offset_gb_b.u32;
}
static __inline  GK_VOID isp_ae1_lut_update_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_lut_update)
{
    U_ISP_AE1_LUT_UPDATE o_isp_ae1_lut_update;
    o_isp_ae1_lut_update.bits.isp_ae_lut_update = uisp_ae1_lut_update;
    o_isp_ae1_lut_update.bits.reserved_0 = 0;
    pstFeReg->ISP_AE1_LUT_UPDATE.u32 = o_isp_ae1_lut_update.u32;
}
static __inline  GK_VOID isp_ae1_stt_size_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_stt_size)
{
    U_ISP_AE1_STT_BST o_isp_ae1_stt_bst;
    o_isp_ae1_stt_bst.u32 = pstFeReg->ISP_AE1_STT_BST.u32;
    o_isp_ae1_stt_bst.bits.isp_ae_stt_size = uisp_ae1_stt_size;
    pstFeReg->ISP_AE1_STT_BST.u32 = o_isp_ae1_stt_bst.u32;
}
static __inline  GK_VOID isp_ae1_stt_bst_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_stt_bst)
{
    U_ISP_AE1_STT_BST o_isp_ae1_stt_bst;
    o_isp_ae1_stt_bst.u32 = pstFeReg->ISP_AE1_STT_BST.u32;
    o_isp_ae1_stt_bst.bits.isp_ae_stt_bst = uisp_ae1_stt_bst;
    pstFeReg->ISP_AE1_STT_BST.u32 = o_isp_ae1_stt_bst.u32;
}
static __inline  GK_VOID isp_ae1_stt_en_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_stt_en)
{
    U_ISP_AE1_STT_BST o_isp_ae1_stt_bst;
    o_isp_ae1_stt_bst.u32 = pstFeReg->ISP_AE1_STT_BST.u32;
    o_isp_ae1_stt_bst.bits.isp_ae_stt_en = uisp_ae1_stt_en;
    pstFeReg->ISP_AE1_STT_BST.u32 = o_isp_ae1_stt_bst.u32;
}
static __inline  GK_VOID isp_ae1_crop_pos_x_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_crop_pos_x)
{
    U_ISP_AE1_CROP_POS o_isp_ae1_crop_pos;
    o_isp_ae1_crop_pos.u32 = pstFeReg->ISP_AE1_CROP_POS.u32;
    o_isp_ae1_crop_pos.bits.isp_ae_crop_pos_x = uisp_ae1_crop_pos_x;
    pstFeReg->ISP_AE1_CROP_POS.u32 = o_isp_ae1_crop_pos.u32;
}
static __inline  GK_VOID isp_ae1_crop_pos_y_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_crop_pos_y)
{
    U_ISP_AE1_CROP_POS o_isp_ae1_crop_pos;
    o_isp_ae1_crop_pos.u32 = pstFeReg->ISP_AE1_CROP_POS.u32;
    o_isp_ae1_crop_pos.bits.isp_ae_crop_pos_y = uisp_ae1_crop_pos_y;
    pstFeReg->ISP_AE1_CROP_POS.u32 = o_isp_ae1_crop_pos.u32;
}
static __inline  GK_VOID isp_ae1_crop_out_width_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_crop_out_width)
{
    U_ISP_AE1_CROP_OUTSIZE o_isp_ae1_crop_outsize;
    o_isp_ae1_crop_outsize.u32 = pstFeReg->ISP_AE1_CROP_OUTSIZE.u32;
    o_isp_ae1_crop_outsize.bits.isp_ae_crop_out_width = uisp_ae1_crop_out_width;
    pstFeReg->ISP_AE1_CROP_OUTSIZE.u32 = o_isp_ae1_crop_outsize.u32;
}
static __inline  GK_VOID isp_ae1_crop_out_height_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_crop_out_height)
{
    U_ISP_AE1_CROP_OUTSIZE o_isp_ae1_crop_outsize;
    o_isp_ae1_crop_outsize.u32 = pstFeReg->ISP_AE1_CROP_OUTSIZE.u32;
    o_isp_ae1_crop_outsize.bits.isp_ae_crop_out_height = uisp_ae1_crop_out_height;
    pstFeReg->ISP_AE1_CROP_OUTSIZE.u32 = o_isp_ae1_crop_outsize.u32;
}
static __inline  GK_VOID isp_ae1st_raddr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1st_raddr)
{
    pstFeReg->ISP_AE1_HIST_RADDR.u32 = uisp_ae1st_raddr;
}
static __inline  GK_VOID isp_ae1_wei_waddr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_wei_waddr)
{
    pstFeReg->ISP_AE1_WEIGHT_WADDR.u32 = uisp_ae1_wei_waddr;
}
static __inline  GK_VOID isp_ae1_wei_wdata_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_ae1_wei_wdata)
{
    pstFeReg->ISP_AE1_WEIGHT_WDATA.u32 = uisp_ae1_wei_wdata;
}
static __inline  GK_VOID isp_rc_blc_in_en_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_blc_in_en)
{
    U_ISP_RC_BLCEN o_isp_rc_blcen;
    o_isp_rc_blcen.u32 = pstFeReg->ISP_RC_BLCEN.u32;
    o_isp_rc_blcen.bits.isp_rc_blc_in_en = uisp_rc_blc_in_en;
    pstFeReg->ISP_RC_BLCEN.u32 = o_isp_rc_blcen.u32;
}
static __inline  GK_VOID isp_rc_blc_out_en_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_blc_out_en)
{
    U_ISP_RC_BLCEN o_isp_rc_blcen;
    o_isp_rc_blcen.u32 = pstFeReg->ISP_RC_BLCEN.u32;
    o_isp_rc_blcen.bits.isp_rc_blc_out_en = uisp_rc_blc_out_en;
    pstFeReg->ISP_RC_BLCEN.u32 = o_isp_rc_blcen.u32;
}
static __inline  GK_VOID isp_rc_blc_r_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_blc_r)
{
    U_ISP_RC_BLC0 o_isp_rc_blc0;
    o_isp_rc_blc0.u32 = pstFeReg->ISP_RC_BLC0.u32;
    o_isp_rc_blc0.bits.isp_rc_blc_r = uisp_rc_blc_r;
    pstFeReg->ISP_RC_BLC0.u32 = o_isp_rc_blc0.u32;
}
static __inline  GK_VOID isp_rc_blc_gr_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_blc_gr)
{
    U_ISP_RC_BLC0 o_isp_rc_blc0;
    o_isp_rc_blc0.u32 = pstFeReg->ISP_RC_BLC0.u32;
    o_isp_rc_blc0.bits.isp_rc_blc_gr = uisp_rc_blc_gr;
    pstFeReg->ISP_RC_BLC0.u32 = o_isp_rc_blc0.u32;
}
static __inline  GK_VOID isp_rc_blc_b_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_blc_b)
{
    U_ISP_RC_BLC1 o_isp_rc_blc1;
    o_isp_rc_blc1.u32 = pstFeReg->ISP_RC_BLC1.u32;
    o_isp_rc_blc1.bits.isp_rc_blc_b = uisp_rc_blc_b;
    pstFeReg->ISP_RC_BLC1.u32 = o_isp_rc_blc1.u32;
}
static __inline  GK_VOID isp_rc_blc_gb_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_blc_gb)
{
    U_ISP_RC_BLC1 o_isp_rc_blc1;
    o_isp_rc_blc1.u32 = pstFeReg->ISP_RC_BLC1.u32;
    o_isp_rc_blc1.bits.isp_rc_blc_gb = uisp_rc_blc_gb;
    pstFeReg->ISP_RC_BLC1.u32 = o_isp_rc_blc1.u32;
}
static __inline  GK_VOID isp_rc_sqradius_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_sqradius)
{
    pstFeReg->ISP_RC_SQRADIUS.u32 = uisp_rc_sqradius;
}
static __inline  GK_VOID isp_rc_cenhor_coor_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_cenhor_coor)
{
    U_ISP_RC_CENTER_COOR o_isp_rc_center_coor;
    o_isp_rc_center_coor.u32 = pstFeReg->ISP_RC_CENTER_COOR.u32;
    o_isp_rc_center_coor.bits.isp_rc_cenhor_coor = uisp_rc_cenhor_coor;
    pstFeReg->ISP_RC_CENTER_COOR.u32 = o_isp_rc_center_coor.u32;
}
static __inline  GK_VOID isp_rc_cenver_coor_write(S_ISPFE_REGS_TYPE *pstFeReg,  GK_U32 uisp_rc_cenver_coor)
{
    U_ISP_RC_CENTER_COOR o_isp_rc_center_coor;
    o_isp_rc_center_coor.u32 = pstFeReg->ISP_RC_CENTER_COOR.u32;
    o_isp_rc_center_coor.bits.isp_rc_cenver_coor = uisp_rc_cenver_coor;
    pstFeReg->ISP_RC_CENTER_COOR.u32 = o_isp_rc_center_coor.u32;
}

/******************************************************************************
******************************************************************************
******************************************************************************
********************BE BE BE BE BE BE BE BE BE  ******************************
******************************************************************************
******************************************************************************
******************************************************************************/
static __inline GK_U32 Isp_GetBeFcnt(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->ISP_BE_STARTUP.u32;
}

static __inline  GK_VOID isp_ae_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_sel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = pstBeReg->ISP_BE_MODULE_POS.u32;
    o_isp_be_module_pos.bits.isp_ae_sel = uisp_ae_sel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}
static __inline  GK_VOID isp_awb_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_awb_sel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = pstBeReg->ISP_BE_MODULE_POS.u32;
    o_isp_be_module_pos.bits.isp_awb_sel = uisp_awb_sel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}
static __inline  GK_VOID isp_af_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_af_sel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = pstBeReg->ISP_BE_MODULE_POS.u32;
    o_isp_be_module_pos.bits.isp_af_sel = uisp_af_sel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}

static __inline  GK_VOID isp_dis_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dis_sel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = pstBeReg->ISP_BE_MODULE_POS.u32;
    o_isp_be_module_pos.bits.isp_dis_sel = uisp_dis_sel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}
static __inline  GK_VOID isp_clut_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_clut_sel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = pstBeReg->ISP_BE_MODULE_POS.u32;
    o_isp_be_module_pos.bits.isp_clut_sel = uisp_clut_sel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}
static __inline  GK_VOID isp_dcg_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dcg_sel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = pstBeReg->ISP_BE_MODULE_POS.u32;
    o_isp_be_module_pos.bits.isp_dcg_sel = uisp_dcg_sel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}
static __inline  GK_VOID isp_delay_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_delay)
{
    pstBeReg->ISP_BE_FSTART_DELAY.u32 = uisp_delay;
}
static __inline  GK_VOID isp_input0_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_input0_sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    o_isp_be_input_mux.u32 = pstBeReg->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input0_sel = uisp_input0_sel;
    pstBeReg->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}
static __inline  GK_VOID isp_input1_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_input1_sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    o_isp_be_input_mux.u32 = pstBeReg->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input1_sel = uisp_input1_sel;
    pstBeReg->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}
static __inline  GK_VOID isp_input2_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_input2_sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    o_isp_be_input_mux.u32 = pstBeReg->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input2_sel = uisp_input2_sel;
    pstBeReg->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}
static __inline  GK_VOID isp_input3_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_input3_sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    o_isp_be_input_mux.u32 = pstBeReg->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input3_sel = uisp_input3_sel;
    pstBeReg->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}
static __inline  GK_VOID isp_input4_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_input4_sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    o_isp_be_input_mux.u32 = pstBeReg->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input4_sel = uisp_input4_sel;
    pstBeReg->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}
static __inline  GK_VOID isp_stt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_stt_en)
{
    U_ISP_BE_SYS_CTRL o_isp_be_sys_ctrl;
    o_isp_be_sys_ctrl.bits.isp_stt_en = uisp_stt_en;
    o_isp_be_sys_ctrl.bits.reserved_0 = 0;
    pstBeReg->ISP_BE_SYS_CTRL.u32 = o_isp_be_sys_ctrl.u32;
}

static __inline  GK_VOID isp_user_define0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_user_define0)
{
    pstBeReg->ISP_BE_USER_DEFINE0.u32 = uisp_user_define0;
}
static __inline  GK_VOID isp_user_define1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_user_define1)
{
    pstBeReg->ISP_BE_USER_DEFINE1.u32 = uisp_user_define1;
}
static __inline  GK_VOID isp_format_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_format)
{
    U_ISP_BE_FORMAT o_isp_be_format;
    o_isp_be_format.bits.isp_format = uisp_format;
    o_isp_be_format.bits.reserved_0 = 0;
    pstBeReg->ISP_BE_FORMAT.u32 = o_isp_be_format.u32;
}

static __inline  GK_VOID isp_clip_y_min_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_clip_y_min)
{
    U_ISP_CLIP_Y_CFG o_isp_clip_y_cfg;
    o_isp_clip_y_cfg.u32 = pstBeReg->ISP_CLIP_Y_CFG.u32;
    o_isp_clip_y_cfg.bits.isp_clip_y_min = uisp_clip_y_min;
    pstBeReg->ISP_CLIP_Y_CFG.u32 = o_isp_clip_y_cfg.u32;
}
static __inline  GK_VOID isp_clip_y_max_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_clip_y_max)
{
    U_ISP_CLIP_Y_CFG o_isp_clip_y_cfg;
    o_isp_clip_y_cfg.u32 = pstBeReg->ISP_CLIP_Y_CFG.u32;
    o_isp_clip_y_cfg.bits.isp_clip_y_max = uisp_clip_y_max;
    pstBeReg->ISP_CLIP_Y_CFG.u32 = o_isp_clip_y_cfg.u32;
}
static __inline  GK_VOID isp_clip_c_min_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_clip_c_min)
{
    U_ISP_CLIP_C_CFG o_isp_clip_c_cfg;
    o_isp_clip_c_cfg.u32 = pstBeReg->ISP_CLIP_C_CFG.u32;
    o_isp_clip_c_cfg.bits.isp_clip_c_min = uisp_clip_c_min;
    pstBeReg->ISP_CLIP_C_CFG.u32 = o_isp_clip_c_cfg.u32;
}
static __inline  GK_VOID isp_clip_c_max_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_clip_c_max)
{
    U_ISP_CLIP_C_CFG o_isp_clip_c_cfg;
    o_isp_clip_c_cfg.u32 = pstBeReg->ISP_CLIP_C_CFG.u32;
    o_isp_clip_c_cfg.bits.isp_clip_c_max = uisp_clip_c_max;
    pstBeReg->ISP_CLIP_C_CFG.u32 = o_isp_clip_c_cfg.u32;
}
static __inline  GK_VOID isp_csc_sum_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_csc_sum_en)
{
    U_ISP_CSC_SUM_CFG o_isp_csc_sum_cfg;
    o_isp_csc_sum_cfg.bits.isp_csc_sum_en = uisp_csc_sum_en;
    o_isp_csc_sum_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_CSC_SUM_CFG.u32 = o_isp_csc_sum_cfg.u32;
}
static __inline  GK_VOID isp_yuv444_sum_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_yuv444_sum_en)
{
    U_ISP_YUV444_SUM_CFG o_isp_yuv444_sum_cfg;
    o_isp_yuv444_sum_cfg.bits.isp_yuv444_sum_en = uisp_yuv444_sum_en;
    o_isp_yuv444_sum_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_YUV444_SUM_CFG.u32 = o_isp_yuv444_sum_cfg.u32;
}
static __inline  GK_VOID isp_yuv422_sum_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_yuv422_sum_en)
{
    U_ISP_YUV422_SUM_CFG o_isp_yuv422_sum_cfg;
    o_isp_yuv422_sum_cfg.bits.isp_yuv422_sum_en = uisp_yuv422_sum_en;
    o_isp_yuv422_sum_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_YUV422_SUM_CFG.u32 = o_isp_yuv422_sum_cfg.u32;
}
static __inline  GK_VOID isp_wdr_sum_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_sum_en)
{
    U_ISP_WDR_SUM_CFG o_isp_wdr_sum_cfg;
    o_isp_wdr_sum_cfg.bits.isp_wdr_sum_en = uisp_wdr_sum_en;
    o_isp_wdr_sum_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_WDR_SUM_CFG.u32 = o_isp_wdr_sum_cfg.u32;
}
static __inline  GK_VOID isp_demosaic_sum_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_sum_en)
{
    U_ISP_DEMOSAIC_SUM_CFG o_isp_demosaic_sum_cfg;
    o_isp_demosaic_sum_cfg.bits.isp_demosaic_sum_en = uisp_demosaic_sum_en;
    o_isp_demosaic_sum_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_DEMOSAIC_SUM_CFG.u32 = o_isp_demosaic_sum_cfg.u32;
}
static __inline  GK_VOID isp_dmnr_dither_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dmnr_dither_en)
{
    U_ISP_DMNR_DITHER o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = pstBeReg->ISP_DMNR_DITHER.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_en = uisp_dmnr_dither_en;
    pstBeReg->ISP_DMNR_DITHER.u32 = o_isp_dmnr_dither.u32;
}
static __inline  GK_VOID isp_dmnr_dither_round_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dmnr_dither_round)
{
    U_ISP_DMNR_DITHER o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = pstBeReg->ISP_DMNR_DITHER.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_round = uisp_dmnr_dither_round;
    pstBeReg->ISP_DMNR_DITHER.u32 = o_isp_dmnr_dither.u32;
}
static __inline  GK_VOID isp_dmnr_dither_spatial_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dmnr_dither_spatial_mode)
{
    U_ISP_DMNR_DITHER o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = pstBeReg->ISP_DMNR_DITHER.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_spatial_mode = uisp_dmnr_dither_spatial_mode;
    pstBeReg->ISP_DMNR_DITHER.u32 = o_isp_dmnr_dither.u32;
}
static __inline  GK_VOID isp_dmnr_dither_out_bits_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dmnr_dither_out_bits)
{
    U_ISP_DMNR_DITHER o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = pstBeReg->ISP_DMNR_DITHER.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_out_bits = uisp_dmnr_dither_out_bits;
    pstBeReg->ISP_DMNR_DITHER.u32 = o_isp_dmnr_dither.u32;
}
static __inline  GK_VOID isp_acm_dither_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_acm_dither_en)
{
    U_ISP_ACM_DITHER o_isp_acm_dither;
    o_isp_acm_dither.u32 = pstBeReg->ISP_ACM_DITHER.u32;
    o_isp_acm_dither.bits.isp_acm_dither_en = uisp_acm_dither_en;
    pstBeReg->ISP_ACM_DITHER.u32 = o_isp_acm_dither.u32;
}
static __inline  GK_VOID isp_acm_dither_round_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_acm_dither_round)
{
    U_ISP_ACM_DITHER o_isp_acm_dither;
    o_isp_acm_dither.u32 = pstBeReg->ISP_ACM_DITHER.u32;
    o_isp_acm_dither.bits.isp_acm_dither_round = uisp_acm_dither_round;
    pstBeReg->ISP_ACM_DITHER.u32 = o_isp_acm_dither.u32;
}
static __inline  GK_VOID isp_acm_dither_spatial_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_acm_dither_spatial_mode)
{
    U_ISP_ACM_DITHER o_isp_acm_dither;
    o_isp_acm_dither.u32 = pstBeReg->ISP_ACM_DITHER.u32;
    o_isp_acm_dither.bits.isp_acm_dither_spatial_mode = uisp_acm_dither_spatial_mode;
    pstBeReg->ISP_ACM_DITHER.u32 = o_isp_acm_dither.u32;
}
static __inline  GK_VOID isp_acm_dither_out_bits_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_acm_dither_out_bits)
{
    U_ISP_ACM_DITHER o_isp_acm_dither;
    o_isp_acm_dither.u32 = pstBeReg->ISP_ACM_DITHER.u32;
    o_isp_acm_dither.bits.isp_acm_dither_out_bits = uisp_acm_dither_out_bits;
    pstBeReg->ISP_ACM_DITHER.u32 = o_isp_acm_dither.u32;
}
static __inline  GK_VOID isp_drc_dither_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_dither_en)
{
    U_ISP_DRC_DITHER o_isp_drc_dither;
    o_isp_drc_dither.u32 = pstBeReg->ISP_DRC_DITHER.u32;
    o_isp_drc_dither.bits.isp_drc_dither_en = uisp_drc_dither_en;
    pstBeReg->ISP_DRC_DITHER.u32 = o_isp_drc_dither.u32;
}
static __inline  GK_VOID isp_drc_dither_round_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_dither_round)
{
    U_ISP_DRC_DITHER o_isp_drc_dither;
    o_isp_drc_dither.u32 = pstBeReg->ISP_DRC_DITHER.u32;
    o_isp_drc_dither.bits.isp_drc_dither_round = uisp_drc_dither_round;
    pstBeReg->ISP_DRC_DITHER.u32 = o_isp_drc_dither.u32;
}
static __inline  GK_VOID isp_drc_dither_spatial_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_dither_spatial_mode)
{
    U_ISP_DRC_DITHER o_isp_drc_dither;
    o_isp_drc_dither.u32 = pstBeReg->ISP_DRC_DITHER.u32;
    o_isp_drc_dither.bits.isp_drc_dither_spatial_mode = uisp_drc_dither_spatial_mode;
    pstBeReg->ISP_DRC_DITHER.u32 = o_isp_drc_dither.u32;
}
static __inline  GK_VOID isp_drc_dither_out_bits_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_dither_out_bits)
{
    U_ISP_DRC_DITHER o_isp_drc_dither;
    o_isp_drc_dither.u32 = pstBeReg->ISP_DRC_DITHER.u32;
    o_isp_drc_dither.bits.isp_drc_dither_out_bits = uisp_drc_dither_out_bits;
    pstBeReg->ISP_DRC_DITHER.u32 = o_isp_drc_dither.u32;
}
static __inline  GK_VOID isp_sqrt1_dither_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sqrt1_dither_en)
{
    U_ISP_SQRT1_DITHER o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = pstBeReg->ISP_SQRT1_DITHER.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_en = uisp_sqrt1_dither_en;
    pstBeReg->ISP_SQRT1_DITHER.u32 = o_isp_sqrt1_dither.u32;
}
static __inline  GK_VOID isp_sqrt1_dither_round_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sqrt1_dither_round)
{
    U_ISP_SQRT1_DITHER o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = pstBeReg->ISP_SQRT1_DITHER.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_round = uisp_sqrt1_dither_round;
    pstBeReg->ISP_SQRT1_DITHER.u32 = o_isp_sqrt1_dither.u32;
}
static __inline  GK_VOID isp_sqrt1_dither_spatial_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sqrt1_dither_spatial_mode)
{
    U_ISP_SQRT1_DITHER o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = pstBeReg->ISP_SQRT1_DITHER.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_spatial_mode = uisp_sqrt1_dither_spatial_mode;
    pstBeReg->ISP_SQRT1_DITHER.u32 = o_isp_sqrt1_dither.u32;
}
static __inline  GK_VOID isp_sqrt1_dither_out_bits_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sqrt1_dither_out_bits)
{
    U_ISP_SQRT1_DITHER o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = pstBeReg->ISP_SQRT1_DITHER.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_out_bits = uisp_sqrt1_dither_out_bits;
    pstBeReg->ISP_SQRT1_DITHER.u32 = o_isp_sqrt1_dither.u32;
}
static __inline  GK_VOID isp_blk_b_hblank_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_blk_b_hblank)
{
    U_ISP_BLK_HBLANK o_isp_blk_hblank;
    o_isp_blk_hblank.u32 = pstBeReg->ISP_BLK_HBLANK.u32;
    o_isp_blk_hblank.bits.isp_blk_b_hblank = uisp_blk_b_hblank;
    pstBeReg->ISP_BLK_HBLANK.u32 = o_isp_blk_hblank.u32;
}
static __inline  GK_VOID isp_blk_f_hblank_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_blk_f_hblank)
{
    U_ISP_BLK_HBLANK o_isp_blk_hblank;
    o_isp_blk_hblank.u32 = pstBeReg->ISP_BLK_HBLANK.u32;
    o_isp_blk_hblank.bits.isp_blk_f_hblank = uisp_blk_f_hblank;
    pstBeReg->ISP_BLK_HBLANK.u32 = o_isp_blk_hblank.u32;
}
static __inline  GK_VOID isp_blk_b_vblank_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_blk_b_vblank)
{
    U_ISP_BLK_VBLANK o_isp_blk_vblank;
    o_isp_blk_vblank.u32 = pstBeReg->ISP_BLK_VBLANK.u32;
    o_isp_blk_vblank.bits.isp_blk_b_vblank = uisp_blk_b_vblank;
    pstBeReg->ISP_BLK_VBLANK.u32 = o_isp_blk_vblank.u32;
}
static __inline  GK_VOID isp_blk_f_vblank_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_blk_f_vblank)
{
    U_ISP_BLK_VBLANK o_isp_blk_vblank;
    o_isp_blk_vblank.u32 = pstBeReg->ISP_BLK_VBLANK.u32;
    o_isp_blk_vblank.bits.isp_blk_f_vblank = uisp_blk_f_vblank;
    pstBeReg->ISP_BLK_VBLANK.u32 = o_isp_blk_vblank.u32;
}

static __inline  GK_VOID isp_dg_clip_value_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dg_clip_value)
{
    U_ISP_DG_CLIP_VALUE o_isp_dg_clip_value;
    o_isp_dg_clip_value.bits.isp_dg_clip_value = uisp_dg_clip_value;
    o_isp_dg_clip_value.bits.reserved_0 = 0;
    pstBeReg->ISP_DG_CLIP_VALUE.u32 = o_isp_dg_clip_value.u32;
}
static __inline  GK_VOID isp_cc_colortone_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_cc_colortone_en)
{
    U_ISP_CC_CFG o_isp_cc_cfg;
    o_isp_cc_cfg.u32 = pstBeReg->ISP_CC_CFG.u32;
    o_isp_cc_cfg.bits.isp_cc_colortone_en = uisp_cc_colortone_en;
    pstBeReg->ISP_CC_CFG.u32 = o_isp_cc_cfg.u32;
}
static __inline  GK_VOID isp_cc_recover_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_cc_recover_en)
{
    U_ISP_CC_CFG o_isp_cc_cfg;
    o_isp_cc_cfg.u32 = pstBeReg->ISP_CC_CFG.u32;
    o_isp_cc_cfg.bits.isp_cc_recover_en = uisp_cc_recover_en;
    pstBeReg->ISP_CC_CFG.u32 = o_isp_cc_cfg.u32;
}

static __inline  GK_VOID isp_4dg0_clip_value_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_4dg0_clip_value)
{
    U_ISP_4DG_0_CLIP_VALUE o_isp_4dg_0_clip_value;
    o_isp_4dg_0_clip_value.bits.isp_4dg0_clip_value = uisp_4dg0_clip_value;
    o_isp_4dg_0_clip_value.bits.reserved_0 = 0;
    pstBeReg->ISP_4DG_0_CLIP_VALUE.u32 = o_isp_4dg_0_clip_value.u32;
}
static __inline  GK_VOID isp_4dg1_clip_value_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_4dg1_clip_value)
{
    U_ISP_4DG_1_CLIP_VALUE o_isp_4dg_1_clip_value;
    o_isp_4dg_1_clip_value.bits.isp_4dg1_clip_value = uisp_4dg1_clip_value;
    o_isp_4dg_1_clip_value.bits.reserved_0 = 0;
    pstBeReg->ISP_4DG_1_CLIP_VALUE.u32 = o_isp_4dg_1_clip_value.u32;
}
static __inline  GK_VOID isp_ae_hnum_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_hnum)
{
    U_ISP_AE_ZONE o_isp_ae_zone;
    o_isp_ae_zone.u32 = pstBeReg->ISP_AE_ZONE.u32;
    o_isp_ae_zone.bits.isp_ae_hnum = uisp_ae_hnum;
    pstBeReg->ISP_AE_ZONE.u32 = o_isp_ae_zone.u32;
}
static __inline  GK_VOID isp_ae_vnum_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_vnum)
{
    U_ISP_AE_ZONE o_isp_ae_zone;
    o_isp_ae_zone.u32 = pstBeReg->ISP_AE_ZONE.u32;
    o_isp_ae_zone.bits.isp_ae_vnum = uisp_ae_vnum;
    pstBeReg->ISP_AE_ZONE.u32 = o_isp_ae_zone.u32;
}
static __inline  GK_VOID isp_ae_skip_x_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_skip_x)
{
    U_ISP_AE_SKIP_CRG o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    o_isp_ae_skip_crg.bits.isp_ae_skip_x = uisp_ae_skip_x;
    pstBeReg->ISP_AE_SKIP_CRG.u32 = o_isp_ae_skip_crg.u32;
}
static __inline  GK_VOID isp_ae_offset_x_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_offset_x)
{
    U_ISP_AE_SKIP_CRG o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    o_isp_ae_skip_crg.bits.isp_ae_offset_x = uisp_ae_offset_x;
    pstBeReg->ISP_AE_SKIP_CRG.u32 = o_isp_ae_skip_crg.u32;
}
static __inline  GK_VOID isp_ae_skip_y_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_skip_y)
{
    U_ISP_AE_SKIP_CRG o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    o_isp_ae_skip_crg.bits.isp_ae_skip_y = uisp_ae_skip_y;
    pstBeReg->ISP_AE_SKIP_CRG.u32 = o_isp_ae_skip_crg.u32;
}
static __inline  GK_VOID isp_ae_offset_y_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_offset_y)
{
    U_ISP_AE_SKIP_CRG o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    o_isp_ae_skip_crg.bits.isp_ae_offset_y = uisp_ae_offset_y;
    pstBeReg->ISP_AE_SKIP_CRG.u32 = o_isp_ae_skip_crg.u32;
}
static __inline  GK_VOID isp_ae_bitmove_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_bitmove)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    o_isp_ae_bitmove.bits.isp_ae_bitmove = uisp_ae_bitmove;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}
static __inline  GK_VOID isp_aest_gamma_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_aest_gamma_mode)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    o_isp_ae_bitmove.bits.isp_aest_gamma_mode = uisp_aest_gamma_mode;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}
static __inline  GK_VOID isp_ae_aver_gamma_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_aver_gamma_mode)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    o_isp_ae_bitmove.bits.isp_ae_aver_gamma_mode = uisp_ae_aver_gamma_mode;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}
static __inline  GK_VOID isp_ae_blc_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_blc_en)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    o_isp_ae_bitmove.bits.isp_ae_blc_en = uisp_ae_blc_en;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}
static __inline  GK_VOID isp_ae_gamma_limit_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_gamma_limit)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    o_isp_ae_bitmove.bits.isp_ae_gamma_limit = uisp_ae_gamma_limit;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}
static __inline  GK_VOID isp_ae_four_plane_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_fourplanemode)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    o_isp_ae_bitmove.bits.isp_ae_fourplanemode = uisp_ae_fourplanemode;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}

static __inline  GK_VOID isp_ae_lut_update_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_lut_update)
{
    U_ISP_AE_LUT_UPDATE o_isp_ae_lut_update;
    o_isp_ae_lut_update.bits.isp_ae_lut_update = uisp_ae_lut_update;
    o_isp_ae_lut_update.bits.reserved_0 = 0;
    pstBeReg->ISP_AE_LUT_UPDATE.u32 = o_isp_ae_lut_update.u32;
}
static __inline  GK_VOID isp_ae_stt_size_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_stt_size)
{
    U_ISP_AE_STT_BST o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = pstBeReg->ISP_AE_STT_BST.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_size = uisp_ae_stt_size;
    pstBeReg->ISP_AE_STT_BST.u32 = o_isp_ae_stt_bst.u32;
}
static __inline  GK_VOID isp_ae_stt_bst_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_stt_bst)
{
    U_ISP_AE_STT_BST o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = pstBeReg->ISP_AE_STT_BST.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_bst = uisp_ae_stt_bst;
    pstBeReg->ISP_AE_STT_BST.u32 = o_isp_ae_stt_bst.u32;
}
static __inline  GK_VOID isp_ae_stt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_stt_en)
{
    U_ISP_AE_STT_BST o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = pstBeReg->ISP_AE_STT_BST.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_en = uisp_ae_stt_en;
    pstBeReg->ISP_AE_STT_BST.u32 = o_isp_ae_stt_bst.u32;
}
static __inline  GK_VOID isp_ae_crop_pos_x_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_crop_pos_x)
{
    U_ISP_AE_CROP_POS o_isp_ae_crop_pos;
    o_isp_ae_crop_pos.u32 = pstBeReg->ISP_AE_CROP_POS.u32;
    o_isp_ae_crop_pos.bits.isp_ae_crop_pos_x = uisp_ae_crop_pos_x;
    pstBeReg->ISP_AE_CROP_POS.u32 = o_isp_ae_crop_pos.u32;
}
static __inline  GK_VOID isp_ae_crop_pos_y_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_crop_pos_y)
{
    U_ISP_AE_CROP_POS o_isp_ae_crop_pos;
    o_isp_ae_crop_pos.u32 = pstBeReg->ISP_AE_CROP_POS.u32;
    o_isp_ae_crop_pos.bits.isp_ae_crop_pos_y = uisp_ae_crop_pos_y;
    pstBeReg->ISP_AE_CROP_POS.u32 = o_isp_ae_crop_pos.u32;
}
static __inline  GK_VOID isp_ae_crop_out_width_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_crop_out_width)
{
    U_ISP_AE_CROP_OUTSIZE o_isp_ae_crop_outsize;
    o_isp_ae_crop_outsize.u32 = pstBeReg->ISP_AE_CROP_OUTSIZE.u32;
    o_isp_ae_crop_outsize.bits.isp_ae_crop_out_width = uisp_ae_crop_out_width;
    pstBeReg->ISP_AE_CROP_OUTSIZE.u32 = o_isp_ae_crop_outsize.u32;
}
static __inline  GK_VOID isp_ae_crop_out_height_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ae_crop_out_height)
{
    U_ISP_AE_CROP_OUTSIZE o_isp_ae_crop_outsize;
    o_isp_ae_crop_outsize.u32 = pstBeReg->ISP_AE_CROP_OUTSIZE.u32;
    o_isp_ae_crop_outsize.bits.isp_ae_crop_out_height = uisp_ae_crop_out_height;
    pstBeReg->ISP_AE_CROP_OUTSIZE.u32 = o_isp_ae_crop_outsize.u32;
}

static __inline  GK_VOID isp_awb_stt_size_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_awb_stt_size)
{
    U_ISP_AWB_STT_BST o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.u32 = pstBeReg->ISP_AWB_STT_BST.u32;
    o_isp_awb_stt_bst.bits.isp_awb_stt_size = uisp_awb_stt_size;
    pstBeReg->ISP_AWB_STT_BST.u32 = o_isp_awb_stt_bst.u32;
}
static __inline  GK_VOID isp_awb_stt_bst_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_awb_stt_bst)
{
    U_ISP_AWB_STT_BST o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.u32 = pstBeReg->ISP_AWB_STT_BST.u32;
    o_isp_awb_stt_bst.bits.isp_awb_stt_bst = uisp_awb_stt_bst;
    pstBeReg->ISP_AWB_STT_BST.u32 = o_isp_awb_stt_bst.u32;
}
static __inline  GK_VOID isp_awb_stt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_awb_stt_en)
{
    U_ISP_AWB_STT_BST o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.u32 = pstBeReg->ISP_AWB_STT_BST.u32;
    o_isp_awb_stt_bst.bits.isp_awb_stt_en = uisp_awb_stt_en;
    pstBeReg->ISP_AWB_STT_BST.u32 = o_isp_awb_stt_bst.u32;
}

static __inline  GK_VOID isp_af_stt_size_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_af_stt_size)
{
    U_ISP_AF_STT_BST o_isp_af_stt_bst;
    o_isp_af_stt_bst.u32 = pstBeReg->ISP_AF_STT_BST.u32;
    o_isp_af_stt_bst.bits.isp_af_stt_size = uisp_af_stt_size;
    pstBeReg->ISP_AF_STT_BST.u32 = o_isp_af_stt_bst.u32;
}
static __inline  GK_VOID isp_af_stt_bst_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_af_stt_bst)
{
    U_ISP_AF_STT_BST o_isp_af_stt_bst;
    o_isp_af_stt_bst.u32 = pstBeReg->ISP_AF_STT_BST.u32;
    o_isp_af_stt_bst.bits.isp_af_stt_bst = uisp_af_stt_bst;
    pstBeReg->ISP_AF_STT_BST.u32 = o_isp_af_stt_bst.u32;
}
static __inline  GK_VOID isp_af_stt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_af_stt_en)
{
    U_ISP_AF_STT_BST o_isp_af_stt_bst;
    o_isp_af_stt_bst.u32 = pstBeReg->ISP_AF_STT_BST.u32;
    o_isp_af_stt_bst.bits.isp_af_stt_en = uisp_af_stt_en;
    pstBeReg->ISP_AF_STT_BST.u32 = o_isp_af_stt_bst.u32;
}


static __inline  GK_VOID isp_la_hnum_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_hnum)
{
    U_ISP_LA_ZONE o_isp_la_zone;
    o_isp_la_zone.u32 = pstBeReg->ISP_LA_ZONE.u32;
    o_isp_la_zone.bits.isp_la_hnum = uisp_la_hnum;
    pstBeReg->ISP_LA_ZONE.u32 = o_isp_la_zone.u32;
}
static __inline  GK_VOID isp_la_vnum_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_vnum)
{
    U_ISP_LA_ZONE o_isp_la_zone;
    o_isp_la_zone.u32 = pstBeReg->ISP_LA_ZONE.u32;
    o_isp_la_zone.bits.isp_la_vnum = uisp_la_vnum;
    pstBeReg->ISP_LA_ZONE.u32 = o_isp_la_zone.u32;
}
static __inline  GK_VOID isp_la_bitmove_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_bitmove)
{
    U_ISP_LA_BITMOVE o_isp_la_bitmove;
    o_isp_la_bitmove.u32 = pstBeReg->ISP_LA_BITMOVE.u32;
    o_isp_la_bitmove.bits.isp_la_bitmove = uisp_la_bitmove;
    pstBeReg->ISP_LA_BITMOVE.u32 = o_isp_la_bitmove.u32;
}
static __inline  GK_VOID isp_la_gamma_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_gamma_en)
{
    U_ISP_LA_BITMOVE o_isp_la_bitmove;
    o_isp_la_bitmove.u32 = pstBeReg->ISP_LA_BITMOVE.u32;
    o_isp_la_bitmove.bits.isp_la_gamma_en = uisp_la_gamma_en;
    pstBeReg->ISP_LA_BITMOVE.u32 = o_isp_la_bitmove.u32;
}
static __inline  GK_VOID isp_la_blc_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_blc_en)
{
    U_ISP_LA_BITMOVE o_isp_la_bitmove;
    o_isp_la_bitmove.u32 = pstBeReg->ISP_LA_BITMOVE.u32;
    o_isp_la_bitmove.bits.isp_la_blc_en = uisp_la_blc_en;
    pstBeReg->ISP_LA_BITMOVE.u32 = o_isp_la_bitmove.u32;
}

static __inline  GK_VOID isp_la_gamma_limit_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_gamma_limit)
{
    U_ISP_LA_GAMMA_LIMIT o_isp_la_gamma_limit;
    o_isp_la_gamma_limit.bits.isp_la_gamma_limit = uisp_la_gamma_limit;
    o_isp_la_gamma_limit.bits.reserved_0 = 0;
    pstBeReg->ISP_LA_GAMMA_LIMIT.u32 = o_isp_la_gamma_limit.u32;
}
static __inline  GK_VOID isp_la_crop_pos_x_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_crop_pos_x)
{
    U_ISP_LA_CROP_POS o_isp_la_crop_pos;
    o_isp_la_crop_pos.u32 = pstBeReg->ISP_LA_CROP_POS.u32;
    o_isp_la_crop_pos.bits.isp_la_crop_pos_x = uisp_la_crop_pos_x;
    pstBeReg->ISP_LA_CROP_POS.u32 = o_isp_la_crop_pos.u32;
}
static __inline  GK_VOID isp_la_crop_pos_y_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_crop_pos_y)
{
    U_ISP_LA_CROP_POS o_isp_la_crop_pos;
    o_isp_la_crop_pos.u32 = pstBeReg->ISP_LA_CROP_POS.u32;
    o_isp_la_crop_pos.bits.isp_la_crop_pos_y = uisp_la_crop_pos_y;
    pstBeReg->ISP_LA_CROP_POS.u32 = o_isp_la_crop_pos.u32;
}
static __inline  GK_VOID isp_la_crop_out_width_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_crop_out_width)
{
    U_ISP_LA_CROP_OUTSIZE o_isp_la_crop_outsize;
    o_isp_la_crop_outsize.u32 = pstBeReg->ISP_LA_CROP_OUTSIZE.u32;
    o_isp_la_crop_outsize.bits.isp_la_crop_out_width = uisp_la_crop_out_width;
    pstBeReg->ISP_LA_CROP_OUTSIZE.u32 = o_isp_la_crop_outsize.u32;
}
static __inline  GK_VOID isp_la_crop_out_height_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_crop_out_height)
{
    U_ISP_LA_CROP_OUTSIZE o_isp_la_crop_outsize;
    o_isp_la_crop_outsize.u32 = pstBeReg->ISP_LA_CROP_OUTSIZE.u32;
    o_isp_la_crop_outsize.bits.isp_la_crop_out_height = uisp_la_crop_out_height;
    pstBeReg->ISP_LA_CROP_OUTSIZE.u32 = o_isp_la_crop_outsize.u32;
}
static __inline  GK_VOID isp_la_stt_size_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_stt_size)
{
    U_ISP_LA_STT_BST o_isp_la_stt_bst;
    o_isp_la_stt_bst.u32 = pstBeReg->ISP_LA_STT_BST.u32;
    o_isp_la_stt_bst.bits.isp_la_stt_size = uisp_la_stt_size;
    pstBeReg->ISP_LA_STT_BST.u32 = o_isp_la_stt_bst.u32;
}
static __inline  GK_VOID isp_la_stt_bst_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_stt_bst)
{
    U_ISP_LA_STT_BST o_isp_la_stt_bst;
    o_isp_la_stt_bst.u32 = pstBeReg->ISP_LA_STT_BST.u32;
    o_isp_la_stt_bst.bits.isp_la_stt_bst = uisp_la_stt_bst;
    pstBeReg->ISP_LA_STT_BST.u32 = o_isp_la_stt_bst.u32;
}
static __inline  GK_VOID isp_la_stt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_la_stt_en)
{
    U_ISP_LA_STT_BST o_isp_la_stt_bst;
    o_isp_la_stt_bst.u32 = pstBeReg->ISP_LA_STT_BST.u32;
    o_isp_la_stt_bst.bits.isp_la_stt_en = uisp_la_stt_en;
    pstBeReg->ISP_LA_STT_BST.u32 = o_isp_la_stt_bst.u32;
}

static __inline  GK_VOID isp_lsc_mesh_str_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_lsc_mesh_str)
{
    U_ISP_LSC_MESH o_isp_lsc_mesh;
    o_isp_lsc_mesh.u32 = pstBeReg->ISP_LSC_MESH.u32;
    o_isp_lsc_mesh.bits.isp_lsc_mesh_str = uisp_lsc_mesh_str;
    pstBeReg->ISP_LSC_MESH.u32 = o_isp_lsc_mesh.u32;
}
static __inline  GK_VOID isp_lsc_mesh_scale_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_lsc_mesh_scale)
{
    U_ISP_LSC_MESH o_isp_lsc_mesh;
    o_isp_lsc_mesh.u32 = pstBeReg->ISP_LSC_MESH.u32;
    o_isp_lsc_mesh.bits.isp_lsc_mesh_scale = uisp_lsc_mesh_scale;
    pstBeReg->ISP_LSC_MESH.u32 = o_isp_lsc_mesh.u32;
}
static __inline  GK_VOID isp_mlsc_mesh_str_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 isp_mlsc_mesh_str)
{
    U_ISP_MLSC_MESH o_isp_mlsc_mesh;
    o_isp_mlsc_mesh.u32 = pstBeReg->ISP_MLSC_MESH.u32;
    o_isp_mlsc_mesh.bits.isp_mlsc_mesh_str = isp_mlsc_mesh_str;
    pstBeReg->ISP_MLSC_MESH.u32 = o_isp_mlsc_mesh.u32;
}

static __inline  GK_VOID isp_fpn_strength0_write(VI_PIPE ViPipe, BLK_DEV BlkDev, GK_U32 uisp_fpn_strength0)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_FPN_CORR0 o_isp_fpn_corr0;
    ISP_CHECK_POINTER_VOID(pstBeReg);
    o_isp_fpn_corr0.u32 = pstBeReg->ISP_FPN_CORR0.u32;
    o_isp_fpn_corr0.bits.isp_fpn_strength0 = uisp_fpn_strength0;
    pstBeReg->ISP_FPN_CORR0.u32 = o_isp_fpn_corr0.u32;
}
static __inline  GK_VOID isp_fpn_overflowthr_write(VI_PIPE ViPipe, BLK_DEV BlkDev, GK_U32 uisp_fpn_overflowthr)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_FPN_OVERFLOWTHR o_isp_fpn_overflowthr;
    ISP_CHECK_POINTER_VOID(pstBeReg);
    o_isp_fpn_overflowthr.u32 = pstBeReg->ISP_FPN_OVERFLOWTHR.u32;
    o_isp_fpn_overflowthr.bits.isp_fpn_overflowthr = uisp_fpn_overflowthr;
    pstBeReg->ISP_FPN_OVERFLOWTHR.u32 = o_isp_fpn_overflowthr.u32;
}
static __inline  GK_VOID isp_dpc_line_aver_fac_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_aver_fac_1)
{
    U_ISP_DPC_LCH_THR_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.u32 = pstBeReg->ISP_DPC_LCH_THR_1.u32;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_aver_fac_1 = uisp_dpc_line_aver_fac_1;
    pstBeReg->ISP_DPC_LCH_THR_1.u32 = o_isp_dpc_lch_thr_1.u32;
}
static __inline  GK_VOID isp_dpc_line_diff_thr_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_diff_thr_1)
{
    U_ISP_DPC_LCH_THR_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.u32 = pstBeReg->ISP_DPC_LCH_THR_1.u32;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_diff_thr_1 = uisp_dpc_line_diff_thr_1;
    pstBeReg->ISP_DPC_LCH_THR_1.u32 = o_isp_dpc_lch_thr_1.u32;
}
static __inline  GK_VOID isp_dpc_line_std_thr_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_std_thr_1)
{
    U_ISP_DPC_LCH_THR_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.u32 = pstBeReg->ISP_DPC_LCH_THR_1.u32;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_std_thr_1 = uisp_dpc_line_std_thr_1;
    pstBeReg->ISP_DPC_LCH_THR_1.u32 = o_isp_dpc_lch_thr_1.u32;
}
static __inline  GK_VOID isp_dpc_line_aver_fac_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_aver_fac_2)
{
    U_ISP_DPC_LCH_THR_2 o_isp_dpc_lch_thr_2;
    o_isp_dpc_lch_thr_2.u32 = pstBeReg->ISP_DPC_LCH_THR_2.u32;
    o_isp_dpc_lch_thr_2.bits.isp_dpc_line_aver_fac_2 = uisp_dpc_line_aver_fac_2;
    pstBeReg->ISP_DPC_LCH_THR_2.u32 = o_isp_dpc_lch_thr_2.u32;
}
static __inline  GK_VOID isp_dpc_line_diff_thr_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_diff_thr_2)
{
    U_ISP_DPC_LCH_THR_2 o_isp_dpc_lch_thr_2;
    o_isp_dpc_lch_thr_2.u32 = pstBeReg->ISP_DPC_LCH_THR_2.u32;
    o_isp_dpc_lch_thr_2.bits.isp_dpc_line_diff_thr_2 = uisp_dpc_line_diff_thr_2;
    pstBeReg->ISP_DPC_LCH_THR_2.u32 = o_isp_dpc_lch_thr_2.u32;
}
static __inline  GK_VOID isp_dpc_line_std_thr_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_std_thr_2)
{
    U_ISP_DPC_LCH_THR_2 o_isp_dpc_lch_thr_2;
    o_isp_dpc_lch_thr_2.u32 = pstBeReg->ISP_DPC_LCH_THR_2.u32;
    o_isp_dpc_lch_thr_2.bits.isp_dpc_line_std_thr_2 = uisp_dpc_line_std_thr_2;
    pstBeReg->ISP_DPC_LCH_THR_2.u32 = o_isp_dpc_lch_thr_2.u32;
}
static __inline  GK_VOID isp_dpc_line_aver_fac_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_aver_fac_3)
{
    U_ISP_DPC_LCH_THR_3 o_isp_dpc_lch_thr_3;
    o_isp_dpc_lch_thr_3.u32 = pstBeReg->ISP_DPC_LCH_THR_3.u32;
    o_isp_dpc_lch_thr_3.bits.isp_dpc_line_aver_fac_3 = uisp_dpc_line_aver_fac_3;
    pstBeReg->ISP_DPC_LCH_THR_3.u32 = o_isp_dpc_lch_thr_3.u32;
}
static __inline  GK_VOID isp_dpc_line_diff_thr_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_diff_thr_3)
{
    U_ISP_DPC_LCH_THR_3 o_isp_dpc_lch_thr_3;
    o_isp_dpc_lch_thr_3.u32 = pstBeReg->ISP_DPC_LCH_THR_3.u32;
    o_isp_dpc_lch_thr_3.bits.isp_dpc_line_diff_thr_3 = uisp_dpc_line_diff_thr_3;
    pstBeReg->ISP_DPC_LCH_THR_3.u32 = o_isp_dpc_lch_thr_3.u32;
}
static __inline  GK_VOID isp_dpc_line_std_thr_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_std_thr_3)
{
    U_ISP_DPC_LCH_THR_3 o_isp_dpc_lch_thr_3;
    o_isp_dpc_lch_thr_3.u32 = pstBeReg->ISP_DPC_LCH_THR_3.u32;
    o_isp_dpc_lch_thr_3.bits.isp_dpc_line_std_thr_3 = uisp_dpc_line_std_thr_3;
    pstBeReg->ISP_DPC_LCH_THR_3.u32 = o_isp_dpc_lch_thr_3.u32;
}
static __inline  GK_VOID isp_dpc_line_aver_fac_4_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_aver_fac_4)
{
    U_ISP_DPC_LCH_THR_4 o_isp_dpc_lch_thr_4;
    o_isp_dpc_lch_thr_4.u32 = pstBeReg->ISP_DPC_LCH_THR_4.u32;
    o_isp_dpc_lch_thr_4.bits.isp_dpc_line_aver_fac_4 = uisp_dpc_line_aver_fac_4;
    pstBeReg->ISP_DPC_LCH_THR_4.u32 = o_isp_dpc_lch_thr_4.u32;
}
static __inline  GK_VOID isp_dpc_line_diff_thr_4_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_diff_thr_4)
{
    U_ISP_DPC_LCH_THR_4 o_isp_dpc_lch_thr_4;
    o_isp_dpc_lch_thr_4.u32 = pstBeReg->ISP_DPC_LCH_THR_4.u32;
    o_isp_dpc_lch_thr_4.bits.isp_dpc_line_diff_thr_4 = uisp_dpc_line_diff_thr_4;
    pstBeReg->ISP_DPC_LCH_THR_4.u32 = o_isp_dpc_lch_thr_4.u32;
}
static __inline  GK_VOID isp_dpc_line_std_thr_4_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_std_thr_4)
{
    U_ISP_DPC_LCH_THR_4 o_isp_dpc_lch_thr_4;
    o_isp_dpc_lch_thr_4.u32 = pstBeReg->ISP_DPC_LCH_THR_4.u32;
    o_isp_dpc_lch_thr_4.bits.isp_dpc_line_std_thr_4 = uisp_dpc_line_std_thr_4;
    pstBeReg->ISP_DPC_LCH_THR_4.u32 = o_isp_dpc_lch_thr_4.u32;
}
static __inline  GK_VOID isp_dpc_line_aver_fac_5_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_aver_fac_5)
{
    U_ISP_DPC_LCH_THR_5 o_isp_dpc_lch_thr_5;
    o_isp_dpc_lch_thr_5.u32 = pstBeReg->ISP_DPC_LCH_THR_5.u32;
    o_isp_dpc_lch_thr_5.bits.isp_dpc_line_aver_fac_5 = uisp_dpc_line_aver_fac_5;
    pstBeReg->ISP_DPC_LCH_THR_5.u32 = o_isp_dpc_lch_thr_5.u32;
}
static __inline  GK_VOID isp_dpc_line_diff_thr_5_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_diff_thr_5)
{
    U_ISP_DPC_LCH_THR_5 o_isp_dpc_lch_thr_5;
    o_isp_dpc_lch_thr_5.u32 = pstBeReg->ISP_DPC_LCH_THR_5.u32;
    o_isp_dpc_lch_thr_5.bits.isp_dpc_line_diff_thr_5 = uisp_dpc_line_diff_thr_5;
    pstBeReg->ISP_DPC_LCH_THR_5.u32 = o_isp_dpc_lch_thr_5.u32;
}
static __inline  GK_VOID isp_dpc_line_std_thr_5_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_std_thr_5)
{
    U_ISP_DPC_LCH_THR_5 o_isp_dpc_lch_thr_5;
    o_isp_dpc_lch_thr_5.u32 = pstBeReg->ISP_DPC_LCH_THR_5.u32;
    o_isp_dpc_lch_thr_5.bits.isp_dpc_line_std_thr_5 = uisp_dpc_line_std_thr_5;
    pstBeReg->ISP_DPC_LCH_THR_5.u32 = o_isp_dpc_lch_thr_5.u32;
}
static __inline  GK_VOID isp_dpc_line_kerdiff_fac_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_line_kerdiff_fac)
{
    U_ISP_DPC_LINE_KERDIFF_FAC o_isp_dpc_line_kerdiff_fac;
    o_isp_dpc_line_kerdiff_fac.bits.isp_dpc_line_kerdiff_fac = uisp_dpc_line_kerdiff_fac;
    o_isp_dpc_line_kerdiff_fac.bits.reserved_0 = 0;
    pstBeReg->ISP_DPC_LINE_KERDIFF_FAC.u32 = o_isp_dpc_line_kerdiff_fac.u32;
}
static __inline  GK_VOID isp_dpc_blend_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_blend_mode)
{
    U_ISP_DPC_BLEND_MODE o_isp_dpc_blend_mode;
    o_isp_dpc_blend_mode.bits.isp_dpc_blend_mode = uisp_dpc_blend_mode;
    o_isp_dpc_blend_mode.bits.reserved_0 = 0;
    pstBeReg->ISP_DPC_BLEND_MODE.u32 = o_isp_dpc_blend_mode.u32;
}
static __inline  GK_VOID isp_dpc_bit_depth_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_bit_depth_sel)
{
    U_ISP_DPC_BIT_DEPTH_SEL o_isp_dpc_bit_depth_sel;
    o_isp_dpc_bit_depth_sel.bits.isp_dpc_bit_depth_sel = uisp_dpc_bit_depth_sel;
    o_isp_dpc_bit_depth_sel.bits.reserved_0 = 0;
    pstBeReg->ISP_DPC_BIT_DEPTH_SEL.u32 = o_isp_dpc_bit_depth_sel.u32;
}
static __inline  GK_VOID isp_dpc_output_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U8 uisp_dpc_output_mode)
{
    pstBeReg->ISP_DPC_OUTPUT_MODE.u32 = uisp_dpc_output_mode & 0xF;
}
static __inline  GK_VOID isp_dpc_set_use_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U8 uisp_dpc_set_use)
{
    pstBeReg->ISP_DPC_SET_USE.u32 = uisp_dpc_set_use & 0xF;
}
static __inline  GK_VOID isp_dpc_bpt_ctrl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_bpt_ctrl)
{
    pstBeReg->ISP_DPC_BPT_CTRL.u32 = uisp_dpc_bpt_ctrl;
}
static __inline  GK_VOID isp_dpc_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16  uisp_dpc_mode)
{
    pstBeReg->ISP_DPC_MODE.u32 = uisp_dpc_mode & 0x3FF;
}
static __inline  GK_VOID isp_dpc_methods_set_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_method_set_1)
{
    pstBeReg->ISP_DPC_METHODS_SET_1.u32 = uisp_dpc_method_set_1 & 0x1F1F;
}
static __inline  GK_VOID isp_dpc_methods_set_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_method_set_2)
{
    pstBeReg->ISP_DPC_METHODS_SET_2.u32 = uisp_dpc_method_set_2 & 0x1F1F;
}
static __inline  GK_VOID isp_dpc_methods_set_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_method_set_3)
{
    pstBeReg->ISP_DPC_METHODS_SET_3.u32 = uisp_dpc_method_set_3 & 0x1F1F;
}
static __inline  GK_VOID isp_dpc_line_thresh_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_line_thresh_1)
{
    pstBeReg->ISP_DPC_LINE_THRESH_1.u32 = uisp_dpc_line_thresh_1 & 0xFFFF;
}
static __inline  GK_VOID isp_dpc_line_mad_fac_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_line_mad_fac_1)
{
    U_ISP_DPC_LINE_MAD_FAC_1 ISP_DPC_LINE_MAD_FAC_1;
    ISP_DPC_LINE_MAD_FAC_1.u32 = pstBeReg->ISP_DPC_LINE_MAD_FAC_1.u32;
    ISP_DPC_LINE_MAD_FAC_1.bits.isp_dpc_line_mad_fac_1_g  = uisp_dpc_line_mad_fac_1 & 0x3F;
    ISP_DPC_LINE_MAD_FAC_1.bits.isp_dpc_line_mad_fac_1_rb = (uisp_dpc_line_mad_fac_1 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_LINE_MAD_FAC_1.u32 = ISP_DPC_LINE_MAD_FAC_1.u32;
}
static __inline  GK_VOID isp_dpc_pg_fac_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_pg_fac_1)
{
    U_ISP_DPC_PG_FAC_1 ISP_DPC_PG_FAC_1;
    ISP_DPC_PG_FAC_1.u32 = pstBeReg->ISP_DPC_PG_FAC_1.u32;
    ISP_DPC_PG_FAC_1.bits.isp_dpc_pg_fac_1_g  = uisp_dpc_pg_fac_1 & 0x3F;
    ISP_DPC_PG_FAC_1.bits.isp_dpc_pg_fac_1_rb = (uisp_dpc_pg_fac_1 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_PG_FAC_1.u32 = ISP_DPC_PG_FAC_1.u32;
}
static __inline  GK_VOID isp_dpc_rnd_thresh_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_rnd_thresh_1)
{
    pstBeReg->ISP_DPC_RND_THRESH_1.u32 = uisp_dpc_rnd_thresh_1 & 0xFFFF;
}
static __inline  GK_VOID isp_dpc_rg_fac_1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rg_fac_1)
{
    U_ISP_DPC_RG_FAC_1 ISP_DPC_RG_FAC_1;
    ISP_DPC_RG_FAC_1.u32 = pstBeReg->ISP_DPC_RG_FAC_1.u32;
    ISP_DPC_RG_FAC_1.bits.isp_dpc_rg_fac_1_g  = uisp_dpc_rg_fac_1 & 0x3F;
    ISP_DPC_RG_FAC_1.bits.isp_dpc_rg_fac_1_rb = (uisp_dpc_rg_fac_1 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_RG_FAC_1.u32 = ISP_DPC_RG_FAC_1.u32;
}
static __inline  GK_VOID isp_dpc_line_thresh_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_line_thresh_2)
{
    U_ISP_DPC_LINE_THRESH_2 ISP_DPC_LINE_THRESH_2;
    ISP_DPC_LINE_THRESH_2.u32 = pstBeReg->ISP_DPC_LINE_THRESH_2.u32;
    ISP_DPC_LINE_THRESH_2.bits.isp_dpc_line_thr2_g  = uisp_dpc_line_thresh_2 & 0xFF;
    ISP_DPC_LINE_THRESH_2.bits.isp_dpc_line_thr2_rb = (uisp_dpc_line_thresh_2 >> 8) & 0xFF;
    pstBeReg->ISP_DPC_LINE_THRESH_2.u32 = ISP_DPC_LINE_THRESH_2.u32;
}
static __inline  GK_VOID isp_dpc_line_mad_fac_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_line_mad_fac_2)
{
    U_ISP_DPC_LINE_MAD_FAC_2 ISP_DPC_LINE_MAD_FAC_2;
    ISP_DPC_LINE_MAD_FAC_2.u32 = pstBeReg->ISP_DPC_LINE_MAD_FAC_2.u32;
    ISP_DPC_LINE_MAD_FAC_2.bits.isp_dpc_line_mad_fac_2_g  = uisp_dpc_line_mad_fac_2 & 0x3F;
    ISP_DPC_LINE_MAD_FAC_2.bits.isp_dpc_line_mad_fac_2_rb = (uisp_dpc_line_mad_fac_2 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_LINE_MAD_FAC_2.u32 = ISP_DPC_LINE_MAD_FAC_2.u32;
}
static __inline  GK_VOID isp_dpc_pg_fac_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_pg_fac_2)
{
    U_ISP_DPC_PG_FAC_2 ISP_DPC_PG_FAC_2;
    ISP_DPC_PG_FAC_2.u32 = pstBeReg->ISP_DPC_PG_FAC_2.u32;
    ISP_DPC_PG_FAC_2.bits.isp_dpc_pg_fac_2_g  = uisp_dpc_pg_fac_2 & 0x3F;
    ISP_DPC_PG_FAC_2.bits.isp_dpc_pg_fac_2_rb = (uisp_dpc_pg_fac_2 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_PG_FAC_2.u32 = ISP_DPC_PG_FAC_2.u32;
}
static __inline  GK_VOID isp_dpc_rnd_thresh_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_rnd_thresh_2)
{
    U_ISP_DPC_RND_THRESH_2 ISP_DPC_RND_THRESH_2;
    ISP_DPC_RND_THRESH_2.u32 = pstBeReg->ISP_DPC_RND_THRESH_2.u32;
    ISP_DPC_RND_THRESH_2.bits.isp_dpc_rnd_thr2_g  = uisp_dpc_rnd_thresh_2 & 0xFF;
    ISP_DPC_RND_THRESH_2.bits.isp_dpc_rnd_thr2_rb = (uisp_dpc_rnd_thresh_2 >> 8) & 0xFF;
    pstBeReg->ISP_DPC_RND_THRESH_2.u32 = ISP_DPC_RND_THRESH_2.u32;
}
static __inline  GK_VOID isp_dpc_rg_fac_2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rg_fac_2)
{
    U_ISP_DPC_RG_FAC_2 ISP_DPC_RG_FAC_2;
    ISP_DPC_RG_FAC_2.u32 = pstBeReg->ISP_DPC_RG_FAC_2.u32;
    ISP_DPC_RG_FAC_2.bits.isp_dpc_rg_fac_2_g  = uisp_dpc_rg_fac_2 & 0x3F;
    ISP_DPC_RG_FAC_2.bits.isp_dpc_rg_fac_2_rb = (uisp_dpc_rg_fac_2 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_RG_FAC_2.u32 = ISP_DPC_RG_FAC_2.u32;
}
static __inline  GK_VOID isp_dpc_line_thresh_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_line_thresh_3)
{
    U_ISP_DPC_LINE_THRESH_3 ISP_DPC_LINE_THRESH_3;
    ISP_DPC_LINE_THRESH_3.u32 = pstBeReg->ISP_DPC_LINE_THRESH_3.u32;
    ISP_DPC_LINE_THRESH_3.bits.isp_dpc_line_thr3_g  = uisp_dpc_line_thresh_3 & 0xFF;
    ISP_DPC_LINE_THRESH_3.bits.isp_dpc_line_thr3_rb = (uisp_dpc_line_thresh_3 >> 8) & 0xFF;
    pstBeReg->ISP_DPC_LINE_THRESH_3.u32 = ISP_DPC_LINE_THRESH_3.u32;
}
/*****************************************************
ISP_DPC_LINE_MAD_FAC_3:
0x205a3C54:                 31:14   reserved
                    13:08   line_mad_fac_3_rb
                    07:06   reserved
                    05:00   line_mad_fac_3_g
*****************************************************/
#define ISP_DPC_DEFAULT_LINE_MAD_FAC_3             (0x404)
static __inline  GK_VOID isp_dpc_line_mad_fac_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_line_mad_fac_3)
{
    U_ISP_DPC_LINE_MAD_FAC_3 ISP_DPC_LINE_MAD_FAC_3;
    ISP_DPC_LINE_MAD_FAC_3.u32 = pstBeReg->ISP_DPC_LINE_MAD_FAC_3.u32;
    ISP_DPC_LINE_MAD_FAC_3.bits.isp_dpc_line_mad_fac_3_g  = uisp_dpc_line_mad_fac_3 & 0x3F;
    ISP_DPC_LINE_MAD_FAC_3.bits.isp_dpc_line_mad_fac_3_rb = (uisp_dpc_line_mad_fac_3 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_LINE_MAD_FAC_3.u32 = ISP_DPC_LINE_MAD_FAC_3.u32;
}
static __inline  GK_VOID isp_dpc_pg_fac_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_pg_fac_3)
{
    U_ISP_DPC_PG_FAC_3 ISP_DPC_PG_FAC_3;
    ISP_DPC_PG_FAC_3.u32 = pstBeReg->ISP_DPC_PG_FAC_3.u32;
    ISP_DPC_PG_FAC_3.bits.isp_dpc_pg_fac_3_g  = uisp_dpc_pg_fac_3 & 0x3F;
    ISP_DPC_PG_FAC_3.bits.isp_dpc_pg_fac_3_rb = (uisp_dpc_pg_fac_3 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_PG_FAC_3.u32 = ISP_DPC_PG_FAC_3.u32;
}
static __inline  GK_VOID isp_dpc_rnd_thresh_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 uisp_dpc_rnd_thresh_3)
{
    U_ISP_DPC_RND_THRESH_3 ISP_DPC_RND_THRESH_3;
    ISP_DPC_RND_THRESH_3.u32 = pstBeReg->ISP_DPC_RND_THRESH_3.u32;
    ISP_DPC_RND_THRESH_3.bits.isp_dpc_rnd_thr3_g  = uisp_dpc_rnd_thresh_3 & 0xFF;
    ISP_DPC_RND_THRESH_3.bits.isp_dpc_rnd_thr3_rb = (uisp_dpc_rnd_thresh_3 >> 8) & 0xFF;
    pstBeReg->ISP_DPC_RND_THRESH_3.u32 = ISP_DPC_RND_THRESH_3.u32;
}
static __inline  GK_VOID isp_dpc_rg_fac_3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rg_fac_3)
{
    U_ISP_DPC_RG_FAC_3 ISP_DPC_RG_FAC_3;
    ISP_DPC_RG_FAC_3.u32 = pstBeReg->ISP_DPC_RG_FAC_3.u32;
    ISP_DPC_RG_FAC_3.bits.isp_dpc_rg_fac_3_g  = uisp_dpc_rg_fac_3 & 0x3F;
    ISP_DPC_RG_FAC_3.bits.isp_dpc_rg_fac_3_rb = (uisp_dpc_rg_fac_3 >> 8) & 0x3F;
    pstBeReg->ISP_DPC_RG_FAC_3.u32 = ISP_DPC_RG_FAC_3.u32;
}
static __inline  GK_VOID isp_dpc_rnd_thresh_1_mtp_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rnd_thresh_1_mtp)
{
    U_ISP_DPC_RND_THRESH_1_MTP o_isp_dpc_rnd_thresh_1_mtp;
    o_isp_dpc_rnd_thresh_1_mtp.u32 = pstBeReg->ISP_DPC_RND_THRESH_1_MTP.u32;
    o_isp_dpc_rnd_thresh_1_mtp.bits.isp_dpc_rnd_thr1_g_mtp = uisp_dpc_rnd_thresh_1_mtp & 0x3ff;
    o_isp_dpc_rnd_thresh_1_mtp.bits.isp_dpc_rnd_thr1_rb_mtp = (uisp_dpc_rnd_thresh_1_mtp >> 10) & 0x3ff;
    pstBeReg->ISP_DPC_RND_THRESH_1_MTP.u32 = o_isp_dpc_rnd_thresh_1_mtp.u32;
}
static __inline  GK_VOID isp_dpc_rnd_thresh_2_mtp_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rnd_thresh_2_mtp)
{
    U_ISP_DPC_RND_THRESH_2_MTP o_isp_dpc_rnd_thresh_2_mtp;
    o_isp_dpc_rnd_thresh_2_mtp.u32 = pstBeReg->ISP_DPC_RND_THRESH_2_MTP.u32;
    o_isp_dpc_rnd_thresh_2_mtp.bits.isp_dpc_rnd_thr2_g_mtp = uisp_dpc_rnd_thresh_2_mtp & 0x3ff;
    o_isp_dpc_rnd_thresh_2_mtp.bits.isp_dpc_rnd_thr2_rb_mtp = (uisp_dpc_rnd_thresh_2_mtp >> 10) & 0x3ff;
    pstBeReg->ISP_DPC_RND_THRESH_2_MTP.u32 = o_isp_dpc_rnd_thresh_2_mtp.u32;
}
static __inline  GK_VOID isp_dpc_rnd_thresh_3_mtp_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rnd_thresh_3_mtp)
{
    U_ISP_DPC_RND_THRESH_3_MTP o_isp_dpc_rnd_thresh_3_mtp;
    o_isp_dpc_rnd_thresh_3_mtp.u32 = pstBeReg->ISP_DPC_RND_THRESH_3_MTP.u32;
    o_isp_dpc_rnd_thresh_3_mtp.bits.isp_dpc_rnd_thr3_g_mtp = uisp_dpc_rnd_thresh_3_mtp & 0x3ff;
    o_isp_dpc_rnd_thresh_3_mtp.bits.isp_dpc_rnd_thr3_rb_mtp = (uisp_dpc_rnd_thresh_3_mtp >> 10) & 0x3ff;
    pstBeReg->ISP_DPC_RND_THRESH_3_MTP.u32 = o_isp_dpc_rnd_thresh_3_mtp.u32;
}
static __inline  GK_VOID isp_dpc_rg_fac_1_mtp_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rg_fac_1_mtp)
{
    U_ISP_DPC_RG_FAC_1_MTP o_isp_dpc_rg_fac_1_mtp;
    o_isp_dpc_rg_fac_1_mtp.u32 = pstBeReg->ISP_DPC_RG_FAC_1_MTP.u32;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_g_mtp = uisp_dpc_rg_fac_1_mtp & 0x3ff;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_rb_mtp = (uisp_dpc_rg_fac_1_mtp >> 10) & 0x3ff;
    pstBeReg->ISP_DPC_RG_FAC_1_MTP.u32 = o_isp_dpc_rg_fac_1_mtp.u32;
}
static __inline  GK_VOID isp_dpc_rg_fac_2_mtp_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rg_fac_2_mtp)
{
    U_ISP_DPC_RG_FAC_2_MTP o_isp_dpc_rg_fac_2_mtp;
    o_isp_dpc_rg_fac_2_mtp.u32 = pstBeReg->ISP_DPC_RG_FAC_2_MTP.u32;
    o_isp_dpc_rg_fac_2_mtp.bits.isp_dpc_rg_fac_2_g_mtp = uisp_dpc_rg_fac_2_mtp & 0x3ff;
    o_isp_dpc_rg_fac_2_mtp.bits.isp_dpc_rg_fac_2_rb_mtp = (uisp_dpc_rg_fac_2_mtp >> 10) & 0x3ff;
    pstBeReg->ISP_DPC_RG_FAC_2_MTP.u32 = o_isp_dpc_rg_fac_2_mtp.u32;
}
static __inline  GK_VOID isp_dpc_rg_fac_3_mtp_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rg_fac_3_mtp)
{
    U_ISP_DPC_RG_FAC_3_MTP o_isp_dpc_rg_fac_3_mtp;
    o_isp_dpc_rg_fac_3_mtp.u32 = pstBeReg->ISP_DPC_RG_FAC_3_MTP.u32;
    o_isp_dpc_rg_fac_3_mtp.bits.isp_dpc_rg_fac_3_g_mtp = uisp_dpc_rg_fac_3_mtp & 0x3ff;
    o_isp_dpc_rg_fac_3_mtp.bits.isp_dpc_rg_fac_3_rb_mtp = (uisp_dpc_rg_fac_3_mtp >> 10) & 0x3ff;
    pstBeReg->ISP_DPC_RG_FAC_3_MTP.u32 = o_isp_dpc_rg_fac_3_mtp.u32;
}
static __inline  GK_VOID isp_dpc_ro_limits_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_ro_limits)
{
    pstBeReg->ISP_DPC_RO_LIMITS.u32 = uisp_dpc_ro_limits & 0xFFF;
}
static __inline  GK_VOID isp_dpc_rnd_offs_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_rnd_offs)
{
    pstBeReg->ISP_DPC_RND_OFFS.u32 = uisp_dpc_rnd_offs & 0xFFF;
}
static __inline  GK_VOID isp_dpc_blend_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dpc_alpha)
{
    pstBeReg->ISP_DPC_ALPHA.u32 = uisp_dpc_alpha;
}
static __inline  GK_VOID isp_demosaic_ahd_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_ahd_en)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = pstBeReg->ISP_DEMOSAIC_CFG1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_ahd_en = uisp_demosaic_ahd_en;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}
static __inline  GK_VOID isp_demosaic_local_cac_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_local_cac_en)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = pstBeReg->ISP_DEMOSAIC_CFG1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_local_cac_en = uisp_demosaic_local_cac_en;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}
static __inline  GK_VOID isp_demosaic_fcr_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_fcr_en)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = pstBeReg->ISP_DEMOSAIC_CFG1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_fcr_en = uisp_demosaic_fcr_en;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}
static __inline  GK_VOID isp_demosaic_cac_cor_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_cac_cor_mode)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = pstBeReg->ISP_DEMOSAIC_CFG1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_cac_cor_mode = uisp_demosaic_cac_cor_mode;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}
static __inline  GK_VOID isp_demosaic_de_fake_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_de_fake_en)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = pstBeReg->ISP_DEMOSAIC_CFG1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_de_fake_en = uisp_demosaic_de_fake_en;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}
static __inline  GK_VOID isp_demosaic_desat_enable_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_desat_enable)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = pstBeReg->ISP_DEMOSAIC_CFG1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_desat_enable = uisp_demosaic_desat_enable;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}

static __inline  GK_VOID isp_demosaic_lpf_f0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_lpf_f0)
{
    U_ISP_DEMOSAIC_LPF_COEF o_isp_demosaic_lpf_coef;
    o_isp_demosaic_lpf_coef.u32 = pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f0 = uisp_demosaic_lpf_f0;
    pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32 = o_isp_demosaic_lpf_coef.u32;
}
static __inline  GK_VOID isp_demosaic_lpf_f1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_lpf_f1)
{
    U_ISP_DEMOSAIC_LPF_COEF o_isp_demosaic_lpf_coef;
    o_isp_demosaic_lpf_coef.u32 = pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f1 = uisp_demosaic_lpf_f1;
    pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32 = o_isp_demosaic_lpf_coef.u32;
}
static __inline  GK_VOID isp_demosaic_lpf_f2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_lpf_f2)
{
    U_ISP_DEMOSAIC_LPF_COEF o_isp_demosaic_lpf_coef;
    o_isp_demosaic_lpf_coef.u32 = pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f2 = uisp_demosaic_lpf_f2;
    pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32 = o_isp_demosaic_lpf_coef.u32;
}
static __inline  GK_VOID isp_demosaic_lpf_f3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_lpf_f3)
{
    U_ISP_DEMOSAIC_LPF_COEF o_isp_demosaic_lpf_coef;
    o_isp_demosaic_lpf_coef.u32 = pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f3 = uisp_demosaic_lpf_f3;
    pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32 = o_isp_demosaic_lpf_coef.u32;
}

static __inline  GK_VOID isp_demosaic_cbcr_ratio_low_limit_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_cbcr_ratio_low_limit)
{
    U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT o_isp_demosaic_cbcr_ratio_limit;
    o_isp_demosaic_cbcr_ratio_limit.u32 = pstBeReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
    o_isp_demosaic_cbcr_ratio_limit.bits.isp_demosaic_cbcr_ratio_low_limit = uisp_demosaic_cbcr_ratio_low_limit;
    pstBeReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = o_isp_demosaic_cbcr_ratio_limit.u32;
}
static __inline  GK_VOID isp_demosaic_cbcr_ratiogh_limit_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_cbcr_ratiogh_limit)
{
    U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT o_isp_demosaic_cbcr_ratio_limit;
    o_isp_demosaic_cbcr_ratio_limit.u32 = pstBeReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
    o_isp_demosaic_cbcr_ratio_limit.bits.isp_demosaic_cbcr_ratiogh_limit = uisp_demosaic_cbcr_ratiogh_limit;
    pstBeReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = o_isp_demosaic_cbcr_ratio_limit.u32;
}

static __inline  GK_VOID isp_demosaic_cb_thr_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_cb_thr)
{
    U_ISP_DEMOSAIC_CAC_CBCR_THR o_isp_demosaic_cac_cbcr_thr;
    o_isp_demosaic_cac_cbcr_thr.u32 = pstBeReg->ISP_DEMOSAIC_CAC_CBCR_THR.u32;
    o_isp_demosaic_cac_cbcr_thr.bits.isp_demosaic_cb_thr = uisp_demosaic_cb_thr;
    pstBeReg->ISP_DEMOSAIC_CAC_CBCR_THR.u32 = o_isp_demosaic_cac_cbcr_thr.u32;
}
static __inline  GK_VOID isp_demosaic_luma_thr_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_luma_thr)
{
    U_ISP_DEMOSAIC_CAC_CBCR_THR o_isp_demosaic_cac_cbcr_thr;
    o_isp_demosaic_cac_cbcr_thr.u32 = pstBeReg->ISP_DEMOSAIC_CAC_CBCR_THR.u32;
    o_isp_demosaic_cac_cbcr_thr.bits.isp_demosaic_luma_thr = uisp_demosaic_luma_thr;
    pstBeReg->ISP_DEMOSAIC_CAC_CBCR_THR.u32 = o_isp_demosaic_cac_cbcr_thr.u32;
}
static __inline  GK_VOID isp_demosaic_desatg_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_desatg)
{
    U_ISP_DEMOSAIC_DESAT_BLDR o_isp_demosaic_desat_bldr;
    o_isp_demosaic_desat_bldr.u32 = pstBeReg->ISP_DEMOSAIC_DESAT_BLDR.u32;
    o_isp_demosaic_desat_bldr.bits.isp_demosaic_desatg = uisp_demosaic_desatg;
    pstBeReg->ISP_DEMOSAIC_DESAT_BLDR.u32 = o_isp_demosaic_desat_bldr.u32;
}
static __inline  GK_VOID isp_demosaic_desat_low_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_desat_low)
{
    U_ISP_DEMOSAIC_DESAT_BLDR o_isp_demosaic_desat_bldr;
    o_isp_demosaic_desat_bldr.u32 = pstBeReg->ISP_DEMOSAIC_DESAT_BLDR.u32;
    o_isp_demosaic_desat_bldr.bits.isp_demosaic_desat_low = uisp_demosaic_desat_low;
    pstBeReg->ISP_DEMOSAIC_DESAT_BLDR.u32 = o_isp_demosaic_desat_bldr.u32;
}
static __inline  GK_VOID isp_demosaic_desat_protect_sl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_desat_protect_sl)
{
    U_ISP_DEMOSAIC_DESAT_PROTECT o_isp_demosaic_desat_protect;
    o_isp_demosaic_desat_protect.u32 = pstBeReg->ISP_DEMOSAIC_DESAT_PROTECT.u32;
    o_isp_demosaic_desat_protect.bits.isp_demosaic_desat_protect_sl = uisp_demosaic_desat_protect_sl;
    pstBeReg->ISP_DEMOSAIC_DESAT_PROTECT.u32 = o_isp_demosaic_desat_protect.u32;
}
static __inline  GK_VOID isp_demosaic_desat_protect_th_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_desat_protect_th)
{
    U_ISP_DEMOSAIC_DESAT_PROTECT o_isp_demosaic_desat_protect;
    o_isp_demosaic_desat_protect.u32 = pstBeReg->ISP_DEMOSAIC_DESAT_PROTECT.u32;
    o_isp_demosaic_desat_protect.bits.isp_demosaic_desat_protect_th = uisp_demosaic_desat_protect_th;
    pstBeReg->ISP_DEMOSAIC_DESAT_PROTECT.u32 = o_isp_demosaic_desat_protect.u32;
}
static __inline  GK_VOID isp_demosaic_depurplut_update_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_demosaic_depurplut_update)
{
    U_ISP_DEMOSAIC_LUT_UPDATE o_isp_demosaic_lut_update;
    o_isp_demosaic_lut_update.bits.isp_demosaic_depurplut_update = uisp_demosaic_depurplut_update;
    o_isp_demosaic_lut_update.bits.reserved_0 = 0;
    pstBeReg->ISP_DEMOSAIC_LUT_UPDATE.u32 = o_isp_demosaic_lut_update.u32;
}
static __inline  GK_VOID isp_rgbir_blc_in_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_rgbir_blc_in_en)
{
    U_ISP_RGBIR_CFG o_isp_rgbir_cfg;
    o_isp_rgbir_cfg.u32 = pstBeReg->ISP_RGBIR_CFG.u32;
    o_isp_rgbir_cfg.bits.isp_rgbir_blc_in_en = uisp_rgbir_blc_in_en;
    pstBeReg->ISP_RGBIR_CFG.u32 = o_isp_rgbir_cfg.u32;
}
static __inline  GK_VOID isp_rgbir_blc_out_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_rgbir_blc_out_en)
{
    U_ISP_RGBIR_CFG o_isp_rgbir_cfg;
    o_isp_rgbir_cfg.u32 = pstBeReg->ISP_RGBIR_CFG.u32;
    o_isp_rgbir_cfg.bits.isp_rgbir_blc_out_en = uisp_rgbir_blc_out_en;
    pstBeReg->ISP_RGBIR_CFG.u32 = o_isp_rgbir_cfg.u32;
}
static __inline  GK_VOID isp_rgbir_ir_pattern_in_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_rgbir_ir_pattern_in)
{
    U_ISP_RGBIR_CFG o_isp_rgbir_cfg;
    o_isp_rgbir_cfg.u32 = pstBeReg->ISP_RGBIR_CFG.u32;
    o_isp_rgbir_cfg.bits.isp_rgbir_ir_pattern_in = uisp_rgbir_ir_pattern_in;
    pstBeReg->ISP_RGBIR_CFG.u32 = o_isp_rgbir_cfg.u32;
}
static __inline  GK_VOID isp_rgbir_pattern_out_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_rgbir_pattern_out)
{
    U_ISP_RGBIR_CFG o_isp_rgbir_cfg;
    o_isp_rgbir_cfg.u32 = pstBeReg->ISP_RGBIR_CFG.u32;
    o_isp_rgbir_cfg.bits.isp_rgbir_pattern_out = uisp_rgbir_pattern_out;
    pstBeReg->ISP_RGBIR_CFG.u32 = o_isp_rgbir_cfg.u32;
}

static __inline  GK_VOID isp_sharpen_benlumactrl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_benlumactrl)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benlumactrl = uisp_sharpen_benlumactrl;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_vcds_filterv_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_vcds_filterv)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_vcds_filterv = uisp_sharpen_vcds_filterv;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_benshtvar_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_benshtvar_sel)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benshtvar_sel = uisp_sharpen_benshtvar_sel;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_benshtctrlbyvar_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_benshtctrlbyvar)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benshtctrlbyvar = uisp_sharpen_benshtctrlbyvar;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_benskinctrl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_benskinctrl)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benskinctrl = uisp_sharpen_benskinctrl;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_weakdetailadj_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_weakdetailadj)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_weakdetailadj = uisp_sharpen_weakdetailadj;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_benchrctrl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_benchrctrl)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benchrctrl = uisp_sharpen_benchrctrl;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_detailthd_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_detailthd_sel)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_detailthd_sel = uisp_sharpen_detailthd_sel;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_bendetailctrl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_bendetailctrl)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_bendetailctrl = uisp_sharpen_bendetailctrl;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_ben8dir_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_ben8dir_sel)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = pstBeReg->ISP_SHARPEN_CTRL.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_ben8dir_sel = uisp_sharpen_ben8dir_sel;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}
static __inline  GK_VOID isp_sharpen_mhfthdsftd_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_mhfthdsftd)
{
    U_ISP_SHARPEN_MHFTHD o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = pstBeReg->ISP_SHARPEN_MHFTHD.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdsftd = uisp_sharpen_mhfthdsftd;
    pstBeReg->ISP_SHARPEN_MHFTHD.u32 = o_isp_sharpen_mhfthd.u32;
}
static __inline  GK_VOID isp_sharpen_mhfthdseld_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_mhfthdseld)
{
    U_ISP_SHARPEN_MHFTHD o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = pstBeReg->ISP_SHARPEN_MHFTHD.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdseld = uisp_sharpen_mhfthdseld;
    pstBeReg->ISP_SHARPEN_MHFTHD.u32 = o_isp_sharpen_mhfthd.u32;
}
static __inline  GK_VOID isp_sharpen_mhfthdsftud_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_mhfthdsftud)
{
    U_ISP_SHARPEN_MHFTHD o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = pstBeReg->ISP_SHARPEN_MHFTHD.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdsftud = uisp_sharpen_mhfthdsftud;
    pstBeReg->ISP_SHARPEN_MHFTHD.u32 = o_isp_sharpen_mhfthd.u32;
}
static __inline  GK_VOID isp_sharpen_mhfthdselud_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_mhfthdselud)
{
    U_ISP_SHARPEN_MHFTHD o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = pstBeReg->ISP_SHARPEN_MHFTHD.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdselud = uisp_sharpen_mhfthdselud;
    pstBeReg->ISP_SHARPEN_MHFTHD.u32 = o_isp_sharpen_mhfthd.u32;
}
static __inline  GK_VOID isp_sharpen_dirrlythrlow_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirrlythrlow)
{
    U_ISP_SHARPEN_WEAKDETAIL o_isp_sharpen_weakdetail;
    o_isp_sharpen_weakdetail.u32 = pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32;
    o_isp_sharpen_weakdetail.bits.isp_sharpen_dirrlythrlow = uisp_sharpen_dirrlythrlow;
    pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32 = o_isp_sharpen_weakdetail.u32;
}
static __inline  GK_VOID isp_sharpen_dirrlythrhih_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirrlythrhih)
{
    U_ISP_SHARPEN_WEAKDETAIL o_isp_sharpen_weakdetail;
    o_isp_sharpen_weakdetail.u32 = pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32;
    o_isp_sharpen_weakdetail.bits.isp_sharpen_dirrlythrhih = uisp_sharpen_dirrlythrhih;
    pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32 = o_isp_sharpen_weakdetail.u32;
}
static __inline  GK_VOID isp_sharpen_weakdetailgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_weakdetailgain)
{
    U_ISP_SHARPEN_WEAKDETAIL o_isp_sharpen_weakdetail;
    o_isp_sharpen_weakdetail.u32 = pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32;
    o_isp_sharpen_weakdetail.bits.isp_sharpen_weakdetailgain = uisp_sharpen_weakdetailgain;
    pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32 = o_isp_sharpen_weakdetail.u32;
}
static __inline  GK_VOID isp_sharpen_dirvarsft_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirvarsft)
{
    U_ISP_SHARPEN_DIRVAR o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = pstBeReg->ISP_SHARPEN_DIRVAR.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirvarsft = uisp_sharpen_dirvarsft;
    pstBeReg->ISP_SHARPEN_DIRVAR.u32 = o_isp_sharpen_dirvar.u32;
}
static __inline  GK_VOID isp_sharpen_dirvarscale_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirvarscale)
{
    U_ISP_SHARPEN_DIRVAR o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = pstBeReg->ISP_SHARPEN_DIRVAR.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirvarscale = uisp_sharpen_dirvarscale;
    pstBeReg->ISP_SHARPEN_DIRVAR.u32 = o_isp_sharpen_dirvar.u32;
}
static __inline  GK_VOID isp_sharpen_dirrly0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirrly0)
{
    U_ISP_SHARPEN_DIRVAR o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = pstBeReg->ISP_SHARPEN_DIRVAR.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirrly0 = uisp_sharpen_dirrly0;
    pstBeReg->ISP_SHARPEN_DIRVAR.u32 = o_isp_sharpen_dirvar.u32;
}
static __inline  GK_VOID isp_sharpen_dirrly1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirrly1)
{
    U_ISP_SHARPEN_DIRVAR o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = pstBeReg->ISP_SHARPEN_DIRVAR.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirrly1 = uisp_sharpen_dirrly1;
    pstBeReg->ISP_SHARPEN_DIRVAR.u32 = o_isp_sharpen_dirvar.u32;
}
static __inline  GK_VOID isp_sharpen_dirdiffsft_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirdiffsft)
{
    U_ISP_SHARPEN_DIRDIFF o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.u32 = pstBeReg->ISP_SHARPEN_DIRDIFF.u32;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirdiffsft = uisp_sharpen_dirdiffsft;
    pstBeReg->ISP_SHARPEN_DIRDIFF.u32 = o_isp_sharpen_dirdiff.u32;
}
static __inline  GK_VOID isp_sharpen_dirrt0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirrt0)
{
    U_ISP_SHARPEN_DIRDIFF o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.u32 = pstBeReg->ISP_SHARPEN_DIRDIFF.u32;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirrt0 = uisp_sharpen_dirrt0;
    pstBeReg->ISP_SHARPEN_DIRDIFF.u32 = o_isp_sharpen_dirdiff.u32;
}
static __inline  GK_VOID isp_sharpen_dirrt1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dirrt1)
{
    U_ISP_SHARPEN_DIRDIFF o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.u32 = pstBeReg->ISP_SHARPEN_DIRDIFF.u32;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirrt1 = uisp_sharpen_dirrt1;
    pstBeReg->ISP_SHARPEN_DIRDIFF.u32 = o_isp_sharpen_dirdiff.u32;
}

static __inline  GK_VOID isp_sharpen_selpixwgt_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_selpixwgt)
{
    U_ISP_SHARPEN_SHOOT_AMT o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.u32 = pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_selpixwgt = uisp_sharpen_selpixwgt;
    pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32 = o_isp_sharpen_shoot_amt.u32;
}
static __inline  GK_VOID isp_sharpen_oshtamt_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_oshtamt)
{
    U_ISP_SHARPEN_SHOOT_AMT o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.u32 = pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_oshtamt = uisp_sharpen_oshtamt;
    pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32 = o_isp_sharpen_shoot_amt.u32;
}
static __inline  GK_VOID isp_sharpen_ushtamt_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_ushtamt)
{
    U_ISP_SHARPEN_SHOOT_AMT o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.u32 = pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_ushtamt = uisp_sharpen_ushtamt;
    pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32 = o_isp_sharpen_shoot_amt.u32;
}

static __inline  GK_VOID isp_sharpen_shtvarsft_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvarsft)
{
    U_ISP_SHARPEN_SHTVAR o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = pstBeReg->ISP_SHARPEN_SHTVAR.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtvarsft = uisp_sharpen_shtvarsft;
    pstBeReg->ISP_SHARPEN_SHTVAR.u32 = o_isp_sharpen_shtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtvar5x5_sft_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvar5x5_sft)
{
    U_ISP_SHARPEN_SHTVAR o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = pstBeReg->ISP_SHARPEN_SHTVAR.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtvar5x5_sft = uisp_sharpen_shtvar5x5_sft;
    pstBeReg->ISP_SHARPEN_SHTVAR.u32 = o_isp_sharpen_shtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtbldrt_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtbldrt)
{
    U_ISP_SHARPEN_SHTVAR o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = pstBeReg->ISP_SHARPEN_SHTVAR.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtbldrt = uisp_sharpen_shtbldrt;
    pstBeReg->ISP_SHARPEN_SHTVAR.u32 = o_isp_sharpen_shtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtvarthd0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvarthd0)
{
    U_ISP_SHARPEN_OSHTVAR o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = pstBeReg->ISP_SHARPEN_OSHTVAR.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarthd0 = uisp_sharpen_shtvarthd0;
    pstBeReg->ISP_SHARPEN_OSHTVAR.u32 = o_isp_sharpen_oshtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtvarthd1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvarthd1)
{
    U_ISP_SHARPEN_OSHTVAR o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = pstBeReg->ISP_SHARPEN_OSHTVAR.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarthd1 = uisp_sharpen_shtvarthd1;
    pstBeReg->ISP_SHARPEN_OSHTVAR.u32 = o_isp_sharpen_oshtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtvarwgt0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvarwgt0)
{
    U_ISP_SHARPEN_OSHTVAR o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = pstBeReg->ISP_SHARPEN_OSHTVAR.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarwgt0 = uisp_sharpen_shtvarwgt0;
    pstBeReg->ISP_SHARPEN_OSHTVAR.u32 = o_isp_sharpen_oshtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtvarwgt1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvarwgt1)
{
    U_ISP_SHARPEN_OSHTVAR o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = pstBeReg->ISP_SHARPEN_OSHTVAR.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarwgt1 = uisp_sharpen_shtvarwgt1;
    pstBeReg->ISP_SHARPEN_OSHTVAR.u32 = o_isp_sharpen_oshtvar.u32;
}
static __inline  GK_VOID isp_sharpen_shtvarmul_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvarmul)
{
    U_ISP_SHARPEN_SHTVAR_MUL o_isp_sharpen_shtvar_mul;
    o_isp_sharpen_shtvar_mul.u32 = pstBeReg->ISP_SHARPEN_SHTVAR_MUL.u32;
    o_isp_sharpen_shtvar_mul.bits.isp_sharpen_shtvarmul = uisp_sharpen_shtvarmul;
    pstBeReg->ISP_SHARPEN_SHTVAR_MUL.u32 = o_isp_sharpen_shtvar_mul.u32;
}
static __inline  GK_VOID isp_sharpen_shtvardiffmul_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvardiffmul)
{
    U_ISP_SHARPEN_SHTVAR_MUL o_isp_sharpen_shtvar_mul;
    o_isp_sharpen_shtvar_mul.u32 = pstBeReg->ISP_SHARPEN_SHTVAR_MUL.u32;
    o_isp_sharpen_shtvar_mul.bits.isp_sharpen_shtvardiffmul = uisp_sharpen_shtvardiffmul;
    pstBeReg->ISP_SHARPEN_SHTVAR_MUL.u32 = o_isp_sharpen_shtvar_mul.u32;
}
static __inline  GK_VOID isp_sharpen_shtvardiffthd0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvardiffthd0)
{
    U_ISP_SHARPEN_OSHTVARDIFF o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffthd0 = uisp_sharpen_shtvardiffthd0;
    pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32 = o_isp_sharpen_oshtvardiff.u32;
}
static __inline  GK_VOID isp_sharpen_shtvardiffthd1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvardiffthd1)
{
    U_ISP_SHARPEN_OSHTVARDIFF o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffthd1 = uisp_sharpen_shtvardiffthd1;
    pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32 = o_isp_sharpen_oshtvardiff.u32;
}
static __inline  GK_VOID isp_sharpen_shtvardiffwgt0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvardiffwgt0)
{
    U_ISP_SHARPEN_OSHTVARDIFF o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffwgt0 = uisp_sharpen_shtvardiffwgt0;
    pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32 = o_isp_sharpen_oshtvardiff.u32;
}
static __inline  GK_VOID isp_sharpen_shtvardiffwgt1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_shtvardiffwgt1)
{
    U_ISP_SHARPEN_OSHTVARDIFF o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffwgt1 = uisp_sharpen_shtvardiffwgt1;
    pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32 = o_isp_sharpen_oshtvardiff.u32;
}

static __inline  GK_VOID isp_sharpen_skinedgesft_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_skinedgesft)
{
    U_ISP_SHARPEN_SKIN_EDGE o_isp_sharpen_skin_edge;
    o_isp_sharpen_skin_edge.u32 = pstBeReg->ISP_SHARPEN_SKIN_EDGE.u32;
    o_isp_sharpen_skin_edge.bits.isp_sharpen_skinedgesft = uisp_sharpen_skinedgesft;
    pstBeReg->ISP_SHARPEN_SKIN_EDGE.u32 = o_isp_sharpen_skin_edge.u32;
}
static __inline  GK_VOID isp_sharpen_skinedgemul_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_skinedgemul)
{
    U_ISP_SHARPEN_SKIN_EDGE o_isp_sharpen_skin_edge;
    o_isp_sharpen_skin_edge.u32 = pstBeReg->ISP_SHARPEN_SKIN_EDGE.u32;
    o_isp_sharpen_skin_edge.bits.isp_sharpen_skinedgemul = uisp_sharpen_skinedgemul;
    pstBeReg->ISP_SHARPEN_SKIN_EDGE.u32 = o_isp_sharpen_skin_edge.u32;
}
static __inline  GK_VOID isp_sharpen_skinedgethd0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_skinedgethd0)
{
    U_ISP_SHARPEN_SKIN_EDGETHD o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgethd0 = uisp_sharpen_skinedgethd0;
    pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32 = o_isp_sharpen_skin_edgethd.u32;
}
static __inline  GK_VOID isp_sharpen_skinedgethd1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_skinedgethd1)
{
    U_ISP_SHARPEN_SKIN_EDGETHD o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgethd1 = uisp_sharpen_skinedgethd1;
    pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32 = o_isp_sharpen_skin_edgethd.u32;
}
static __inline  GK_VOID isp_sharpen_skinedgewgt0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_skinedgewgt0)
{
    U_ISP_SHARPEN_SKIN_EDGETHD o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgewgt0 = uisp_sharpen_skinedgewgt0;
    pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32 = o_isp_sharpen_skin_edgethd.u32;
}
static __inline  GK_VOID isp_sharpen_skinedgewgt1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_skinedgewgt1)
{
    U_ISP_SHARPEN_SKIN_EDGETHD o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgewgt1 = uisp_sharpen_skinedgewgt1;
    pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32 = o_isp_sharpen_skin_edgethd.u32;
}
static __inline  GK_VOID isp_sharpen_chrrgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_chrrgain)
{
    U_ISP_SHARPEN_CHRR_GAIN o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.u32 = pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_chrrgain = uisp_sharpen_chrrgain;
    pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32 = o_isp_sharpen_chrr_gain.u32;
}
static __inline  GK_VOID isp_sharpen_rmfscale_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_rmfscale)
{
    U_ISP_SHARPEN_CHRR_GAIN o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.u32 = pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_rmfscale = uisp_sharpen_rmfscale;
    pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32 = o_isp_sharpen_chrr_gain.u32;
}
static __inline  GK_VOID isp_sharpen_chrrmul_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_chrrmul)
{
    U_ISP_SHARPEN_CHRR_GAIN o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.u32 = pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_chrrmul = uisp_sharpen_chrrmul;
    pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32 = o_isp_sharpen_chrr_gain.u32;
}

static __inline  GK_VOID isp_sharpen_chrbgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_chrbgain)
{
    U_ISP_SHARPEN_CHRB_GAIN o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.u32 = pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_chrbgain = uisp_sharpen_chrbgain;
    pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32 = o_isp_sharpen_chrb_gain.u32;
}
static __inline  GK_VOID isp_sharpen_bmfscale_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_bmfscale)
{
    U_ISP_SHARPEN_CHRB_GAIN o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.u32 = pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_bmfscale = uisp_sharpen_bmfscale;
    pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32 = o_isp_sharpen_chrb_gain.u32;
}
static __inline  GK_VOID isp_sharpen_chrbmul_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_chrbmul)
{
    U_ISP_SHARPEN_CHRB_GAIN o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.u32 = pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_chrbmul = uisp_sharpen_chrbmul;
    pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32 = o_isp_sharpen_chrb_gain.u32;
}
static __inline  GK_VOID isp_sharpen_osht_dtl_thd0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_osht_dtl_thd0)
{
    U_ISP_SHARPEN_OSHT_DETAIL o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_thd0 = uisp_sharpen_osht_dtl_thd0;
    pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32 = o_isp_sharpen_osht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_osht_dtl_thd1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_osht_dtl_thd1)
{
    U_ISP_SHARPEN_OSHT_DETAIL o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_thd1 = uisp_sharpen_osht_dtl_thd1;
    pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32 = o_isp_sharpen_osht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_osht_dtl_wgt_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_osht_dtl_wgt)
{
    U_ISP_SHARPEN_OSHT_DETAIL o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_wgt = uisp_sharpen_osht_dtl_wgt;
    pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32 = o_isp_sharpen_osht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_max_var_clip_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_max_var_clip)
{
    U_ISP_SHARPEN_OSHT_DETAIL o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_max_var_clip = uisp_sharpen_max_var_clip;
    pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32 = o_isp_sharpen_osht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_usht_dtl_thd0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_usht_dtl_thd0)
{
    U_ISP_SHARPEN_USHT_DETAIL o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_thd0 = uisp_sharpen_usht_dtl_thd0;
    pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32 = o_isp_sharpen_usht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_usht_dtl_thd1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_usht_dtl_thd1)
{
    U_ISP_SHARPEN_USHT_DETAIL o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_thd1 = uisp_sharpen_usht_dtl_thd1;
    pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32 = o_isp_sharpen_usht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_usht_dtl_wgt_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_usht_dtl_wgt)
{
    U_ISP_SHARPEN_USHT_DETAIL o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_wgt = uisp_sharpen_usht_dtl_wgt;
    pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32 = o_isp_sharpen_usht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_dtl_thdsft_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_dtl_thdsft)
{
    U_ISP_SHARPEN_USHT_DETAIL o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_dtl_thdsft = uisp_sharpen_dtl_thdsft;
    pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32 = o_isp_sharpen_usht_detail.u32;
}
static __inline  GK_VOID isp_sharpen_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_stt2lut_en)
{
    U_ISP_SHARPEN_STT2LUT_CFG o_isp_sharpen_stt2lut_cfg;
    o_isp_sharpen_stt2lut_cfg.bits.isp_sharpen_stt2lut_en = uisp_sharpen_stt2lut_en;
    o_isp_sharpen_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_SHARPEN_STT2LUT_CFG.u32 = o_isp_sharpen_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_sharpen_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_sharpen_stt2lut_regnew)
{
    U_ISP_SHARPEN_STT2LUT_REGNEW o_isp_sharpen_stt2lut_regnew;
    o_isp_sharpen_stt2lut_regnew.bits.isp_sharpen_stt2lut_regnew = uisp_sharpen_stt2lut_regnew;
    o_isp_sharpen_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_SHARPEN_STT2LUT_REGNEW.u32 = o_isp_sharpen_stt2lut_regnew.u32;
}


static __inline  GK_VOID isp_nddm_dith_mask_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_nddm_dith_mask)
{
    U_ISP_NDDM_DITHER_CFG o_isp_nddm_dither_cfg;
    o_isp_nddm_dither_cfg.u32 = pstBeReg->ISP_NDDM_DITHER_CFG.u32;
    o_isp_nddm_dither_cfg.bits.isp_nddm_dith_mask = uisp_nddm_dith_mask;
    pstBeReg->ISP_NDDM_DITHER_CFG.u32 = o_isp_nddm_dither_cfg.u32;
}
static __inline  GK_VOID isp_nddm_dith_ratio_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_nddm_dith_ratio)
{
    U_ISP_NDDM_DITHER_CFG o_isp_nddm_dither_cfg;
    o_isp_nddm_dither_cfg.u32 = pstBeReg->ISP_NDDM_DITHER_CFG.u32;
    o_isp_nddm_dither_cfg.bits.isp_nddm_dith_ratio = uisp_nddm_dith_ratio;
    pstBeReg->ISP_NDDM_DITHER_CFG.u32 = o_isp_nddm_dither_cfg.u32;
}
static __inline  GK_VOID isp_nddm_dith_max_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_nddm_dith_max)
{
    U_ISP_NDDM_DITHER_CFG o_isp_nddm_dither_cfg;
    o_isp_nddm_dither_cfg.u32 = pstBeReg->ISP_NDDM_DITHER_CFG.u32;
    o_isp_nddm_dither_cfg.bits.isp_nddm_dith_max = uisp_nddm_dith_max;
    pstBeReg->ISP_NDDM_DITHER_CFG.u32 = o_isp_nddm_dither_cfg.u32;
}
static __inline  GK_VOID isp_nddm_gf_lut_update_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_nddm_gf_lut_update)
{
    U_ISP_NDDM_GF_LUT_UPDATE o_isp_nddm_gf_lut_update;
    o_isp_nddm_gf_lut_update.bits.isp_nddm_gf_lut_update = uisp_nddm_gf_lut_update;
    o_isp_nddm_gf_lut_update.bits.reserved_0 = 0;
    pstBeReg->ISP_NDDM_GF_LUT_UPDATE.u32 = o_isp_nddm_gf_lut_update.u32;
}


static __inline  GK_VOID isp_bnr_jnlm_coringhig_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_jnlm_coringhig)
{
    U_ISP_BNR_JNLM0 o_isp_bnr_jnlm0;
    o_isp_bnr_jnlm0.u32 = pstBeReg->ISP_BNR_JNLM0.u32;
    o_isp_bnr_jnlm0.bits.isp_bnr_jnlm_coringhig = uisp_bnr_jnlm_coringhig;
    pstBeReg->ISP_BNR_JNLM0.u32 = o_isp_bnr_jnlm0.u32;
}
static __inline  GK_VOID isp_bnr_jnlm_gain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_jnlm_gain)
{
    U_ISP_BNR_JNLM0 o_isp_bnr_jnlm0;
    o_isp_bnr_jnlm0.u32 = pstBeReg->ISP_BNR_JNLM0.u32;
    o_isp_bnr_jnlm0.bits.isp_bnr_jnlm_gain = uisp_bnr_jnlm_gain;
    pstBeReg->ISP_BNR_JNLM0.u32 = o_isp_bnr_jnlm0.u32;
}
static __inline  GK_VOID isp_bnr_jnlm_sel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_jnlm_sel)
{
    U_ISP_BNR_JNLM0 o_isp_bnr_jnlm0;
    o_isp_bnr_jnlm0.u32 = pstBeReg->ISP_BNR_JNLM0.u32;
    o_isp_bnr_jnlm0.bits.isp_bnr_jnlm_sel = uisp_bnr_jnlm_sel;
    pstBeReg->ISP_BNR_JNLM0.u32 = o_isp_bnr_jnlm0.u32;
}
static __inline  GK_VOID isp_bnr_lscbnrenable_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_lscbnrenable)
{
    U_ISP_BNR_LSC_CTRL o_isp_bnr_lsc_ctrl;
    o_isp_bnr_lsc_ctrl.u32 = pstBeReg->ISP_BNR_LSC_CTRL.u32;
    o_isp_bnr_lsc_ctrl.bits.isp_bnr_lscbnrenable = uisp_bnr_lscbnrenable;
    pstBeReg->ISP_BNR_LSC_CTRL.u32 = o_isp_bnr_lsc_ctrl.u32;
}
static __inline  GK_VOID isp_bnr_lscmaxgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_lscmaxgain)
{
    U_ISP_BNR_LSC_CTRL o_isp_bnr_lsc_ctrl;
    o_isp_bnr_lsc_ctrl.u32 = pstBeReg->ISP_BNR_LSC_CTRL.u32;
    o_isp_bnr_lsc_ctrl.bits.isp_bnr_lscmaxgain = uisp_bnr_lscmaxgain;
    pstBeReg->ISP_BNR_LSC_CTRL.u32 = o_isp_bnr_lsc_ctrl.u32;
}
static __inline  GK_VOID isp_bnr_lsccmpstrength_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_lsccmpstrength)
{
    U_ISP_BNR_LSC_CTRL o_isp_bnr_lsc_ctrl;
    o_isp_bnr_lsc_ctrl.u32 = pstBeReg->ISP_BNR_LSC_CTRL.u32;
    o_isp_bnr_lsc_ctrl.bits.isp_bnr_lsccmpstrength = uisp_bnr_lsccmpstrength;
    pstBeReg->ISP_BNR_LSC_CTRL.u32 = o_isp_bnr_lsc_ctrl.u32;
}

static __inline  GK_VOID isp_bnr_rlmt_blc_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_rlmt_blc)
{
    U_ISP_BNR_CHROMA o_isp_bnr_chroma;
    o_isp_bnr_chroma.u32 = pstBeReg->ISP_BNR_CHROMA.u32;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_blc = uisp_bnr_rlmt_blc;
    pstBeReg->ISP_BNR_CHROMA.u32 = o_isp_bnr_chroma.u32;
}
static __inline  GK_VOID isp_bnr_rlmt_rgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_rlmt_rgain)
{
    U_ISP_BNR_CHROMA o_isp_bnr_chroma;
    o_isp_bnr_chroma.u32 = pstBeReg->ISP_BNR_CHROMA.u32;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_rgain = uisp_bnr_rlmt_rgain;
    pstBeReg->ISP_BNR_CHROMA.u32 = o_isp_bnr_chroma.u32;
}
static __inline  GK_VOID isp_bnr_rlmt_bgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_rlmt_bgain)
{
    U_ISP_BNR_CHROMA o_isp_bnr_chroma;
    o_isp_bnr_chroma.u32 = pstBeReg->ISP_BNR_CHROMA.u32;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_bgain = uisp_bnr_rlmt_bgain;
    pstBeReg->ISP_BNR_CHROMA.u32 = o_isp_bnr_chroma.u32;
}
static __inline  GK_VOID isp_bnr_windowsizesel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_windowsizesel)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_windowsizesel = uisp_bnr_windowsizesel;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}

static __inline  GK_VOID isp_bnr_pattern_noise_reduction_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_pattern_noise_reduction)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_pattern_noise_reduction = uisp_bnr_pattern_noise_reduction;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_gainsad_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_gainsad)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_gainsad = uisp_bnr_gainsad;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_implsnrenable1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_implsnrenable1)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrenable1 = uisp_bnr_implsnrenable1;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_implsnrenable2_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_implsnrenable2)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrenable2 = uisp_bnr_implsnrenable2;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_implsnrenable3_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_implsnrenable3)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrenable3 = uisp_bnr_implsnrenable3;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_implsnrstrength_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_implsnrstrength)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrstrength = uisp_bnr_implsnrstrength;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_enablesymmsad_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_enablesymmsad)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_enablesymmsad = uisp_bnr_enablesymmsad;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_enablepostproc_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_enablepostproc)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_enablepostproc = uisp_bnr_enablepostproc;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_bilateral_enable_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_bilateral_enable)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_bilateral_enable = uisp_bnr_bilateral_enable;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_lumasel_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_lumasel)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.u32 = pstBeReg->ISP_BNR_LPFCTRL.u32;
    o_isp_bnr_lpfctrl.bits.isp_bnr_lumasel = uisp_bnr_lumasel;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}
static __inline  GK_VOID isp_bnr_wdr_enable_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_wdr_enable)
{
    U_ISP_BNR_WDRCTRL o_isp_bnr_wdrctrl;
    o_isp_bnr_wdrctrl.u32 = pstBeReg->ISP_BNR_WDRCTRL.u32;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_enable = uisp_bnr_wdr_enable;
    pstBeReg->ISP_BNR_WDRCTRL.u32 = o_isp_bnr_wdrctrl.u32;
}
static __inline  GK_VOID isp_bnr_wdr_enfusion_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_wdr_enfusion)
{
    U_ISP_BNR_WDRCTRL o_isp_bnr_wdrctrl;
    o_isp_bnr_wdrctrl.u32 = pstBeReg->ISP_BNR_WDRCTRL.u32;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_enfusion = uisp_bnr_wdr_enfusion;
    pstBeReg->ISP_BNR_WDRCTRL.u32 = o_isp_bnr_wdrctrl.u32;
}
static __inline  GK_VOID isp_bnr_wdr_mapfltmod_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_wdr_mapfltmod)
{
    U_ISP_BNR_WDRCTRL o_isp_bnr_wdrctrl;
    o_isp_bnr_wdrctrl.u32 = pstBeReg->ISP_BNR_WDRCTRL.u32;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_mapfltmod = uisp_bnr_wdr_mapfltmod;
    pstBeReg->ISP_BNR_WDRCTRL.u32 = o_isp_bnr_wdrctrl.u32;
}
static __inline  GK_VOID isp_bnr_wdr_mapgain_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_wdr_mapgain)
{
    U_ISP_BNR_WDRCTRL o_isp_bnr_wdrctrl;
    o_isp_bnr_wdrctrl.u32 = pstBeReg->ISP_BNR_WDRCTRL.u32;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_mapgain = uisp_bnr_wdr_mapgain;
    pstBeReg->ISP_BNR_WDRCTRL.u32 = o_isp_bnr_wdrctrl.u32;
}

static __inline  GK_VOID isp_bnr_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_stt2lut_en)
{
    U_ISP_BNR_STT2LUT_CFG o_isp_bnr_stt2lut_cfg;
    o_isp_bnr_stt2lut_cfg.bits.isp_bnr_stt2lut_en = uisp_bnr_stt2lut_en;
    o_isp_bnr_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_BNR_STT2LUT_CFG.u32 = o_isp_bnr_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_bnr_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_bnr_stt2lut_regnew)
{
    U_ISP_BNR_STT2LUT_REGNEW o_isp_bnr_stt2lut_regnew;
    o_isp_bnr_stt2lut_regnew.bits.isp_bnr_stt2lut_regnew = uisp_bnr_stt2lut_regnew;
    o_isp_bnr_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_BNR_STT2LUT_REGNEW.u32 = o_isp_bnr_stt2lut_regnew.u32;
}


static __inline  GK_VOID isp_mlsc_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_mlsc_stt2lut_en)
{
    U_ISP_MLSC_STT2LUT_CFG o_isp_mlsc_stt2lut_cfg;
    o_isp_mlsc_stt2lut_cfg.bits.isp_mlsc_stt2lut_en = uisp_mlsc_stt2lut_en;
    o_isp_mlsc_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_MLSC_STT2LUT_CFG.u32 = o_isp_mlsc_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_mlsc_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_mlsc_stt2lut_regnew)
{
    U_ISP_MLSC_STT2LUT_REGNEW o_isp_mlsc_stt2lut_regnew;
    o_isp_mlsc_stt2lut_regnew.bits.isp_mlsc_stt2lut_regnew = uisp_mlsc_stt2lut_regnew;
    o_isp_mlsc_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_MLSC_STT2LUT_REGNEW.u32 = o_isp_mlsc_stt2lut_regnew.u32;
}

static __inline  GK_VOID isp_mlsc_winx_info_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U8 index, GK_U16 uisp_mlsc_deltax, GK_U16 uisp_mlsc_invx)
{
    U_ISP_MLSC_WINX o_isp_mlsc_winx;
    o_isp_mlsc_winx.bits.isp_mlsc_deltax = uisp_mlsc_deltax;
    o_isp_mlsc_winx.bits.isp_mlsc_invx   = uisp_mlsc_invx;
    pstBeReg->ISP_MLSC_WINX[index].u32 = o_isp_mlsc_winx.u32;
}
static __inline  GK_VOID isp_mlsc_winy_info_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U8 index, GK_U16 uisp_mlsc_deltay, GK_U16 uisp_mlsc_invy)
{
    U_ISP_MLSC_WINY o_isp_mlsc_winy;
    o_isp_mlsc_winy.bits.isp_mlsc_deltay = uisp_mlsc_deltay;
    o_isp_mlsc_winy.bits.isp_mlsc_invy = uisp_mlsc_invy;
    pstBeReg->ISP_MLSC_WINY[index].u32 = o_isp_mlsc_winy.u32;
}
static __inline  GK_VOID isp_mlsc_width_offset_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_mlsc_width_offset)
{
    U_ISP_MLSC_WIDTH_OFFSET o_isp_mlsc_width_offset;
    o_isp_mlsc_width_offset.bits.isp_mlsc_width_offset = uisp_mlsc_width_offset;
    o_isp_mlsc_width_offset.bits.reserved_0 = 0;
    pstBeReg->ISP_MLSC_WIDTH_OFFSET.u32 = o_isp_mlsc_width_offset.u32;
}

static __inline  GK_VOID isp_mlsc_mesh_scale_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_mlsc_mesh_scale)
{
    U_ISP_MLSC_MESH o_isp_mlsc_mesh;
    o_isp_mlsc_mesh.u32 = pstBeReg->ISP_MLSC_MESH.u32;
    o_isp_mlsc_mesh.bits.isp_mlsc_mesh_scale = uisp_mlsc_mesh_scale;
    pstBeReg->ISP_MLSC_MESH.u32 = o_isp_mlsc_mesh.u32;
}

static __inline  GK_VOID isp_wdr_fusion_data_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_fusion_data_mode)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = pstBeReg->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_fusion_data_mode = uisp_wdr_fusion_data_mode;
    pstBeReg->ISP_WDR_CTRL.u32 = o_isp_wdr_ctrl.u32;
}

static __inline  GK_VOID isp_wdr_erosion_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_erosion_en)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = pstBeReg->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_erosion_en = uisp_wdr_erosion_en;
    pstBeReg->ISP_WDR_CTRL.u32 = o_isp_wdr_ctrl.u32;
}
static __inline  GK_VOID isp_wdr_grayscale_mode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_grayscale_mode)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = pstBeReg->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_grayscale_mode = uisp_wdr_grayscale_mode;
    pstBeReg->ISP_WDR_CTRL.u32 = o_isp_wdr_ctrl.u32;
}


static __inline  GK_VOID isp_wdr_outblc_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_outblc)
{
    U_ISP_WDR_OUTBLC o_isp_wdr_outblc;
    o_isp_wdr_outblc.u32 = pstBeReg->ISP_WDR_OUTBLC.u32;
    o_isp_wdr_outblc.bits.isp_wdr_outblc = uisp_wdr_outblc;
    pstBeReg->ISP_WDR_OUTBLC.u32 = o_isp_wdr_outblc.u32;
}
static __inline  GK_VOID isp_wdr_bsaveblc_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_bsaveblc)
{
    U_ISP_WDR_OUTBLC o_isp_wdr_outblc;
    o_isp_wdr_outblc.u32 = pstBeReg->ISP_WDR_OUTBLC.u32;
    o_isp_wdr_outblc.bits.isp_wdr_bsaveblc = uisp_wdr_bsaveblc;
    pstBeReg->ISP_WDR_OUTBLC.u32 = o_isp_wdr_outblc.u32;
}

static __inline  GK_VOID isp_wdr_forcelonggh_thd_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_forcelonggh_thd)
{
    U_ISP_WDR_FORCELONG_PARA o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = pstBeReg->ISP_WDR_FORCELONG_PARA.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelonggh_thd = uisp_wdr_forcelonggh_thd;
    pstBeReg->ISP_WDR_FORCELONG_PARA.u32 = o_isp_wdr_forcelong_para.u32;
}
static __inline  GK_VOID isp_wdr_forcelong_low_thd_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_forcelong_low_thd)
{
    U_ISP_WDR_FORCELONG_PARA o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = pstBeReg->ISP_WDR_FORCELONG_PARA.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_low_thd = uisp_wdr_forcelong_low_thd;
    pstBeReg->ISP_WDR_FORCELONG_PARA.u32 = o_isp_wdr_forcelong_para.u32;
}
static __inline  GK_VOID isp_wdr_forcelong_smooth_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_forcelong_smooth_en)
{
    U_ISP_WDR_FORCELONG_PARA o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = pstBeReg->ISP_WDR_FORCELONG_PARA.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_smooth_en = uisp_wdr_forcelong_smooth_en;
    pstBeReg->ISP_WDR_FORCELONG_PARA.u32 = o_isp_wdr_forcelong_para.u32;
}
static __inline  GK_VOID isp_wdr_forcelong_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_forcelong_en)
{
    U_ISP_WDR_FORCELONG_PARA o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = pstBeReg->ISP_WDR_FORCELONG_PARA.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_en = uisp_wdr_forcelong_en;
    pstBeReg->ISP_WDR_FORCELONG_PARA.u32 = o_isp_wdr_forcelong_para.u32;
}
static __inline  GK_VOID isp_wdr_shortchk_thd_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_shortchk_thd)
{
    U_ISP_WDR_SHORTEXPO_CHK o_isp_wdr_shortexpo_chk;
    o_isp_wdr_shortexpo_chk.u32 = pstBeReg->ISP_WDR_SHORTEXPO_CHK.u32;
    o_isp_wdr_shortexpo_chk.bits.isp_wdr_shortchk_thd = uisp_wdr_shortchk_thd;
    pstBeReg->ISP_WDR_SHORTEXPO_CHK.u32 = o_isp_wdr_shortexpo_chk.u32;
}
static __inline  GK_VOID isp_wdr_shortexpo_chk_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_shortexpo_chk)
{
    U_ISP_WDR_SHORTEXPO_CHK o_isp_wdr_shortexpo_chk;
    o_isp_wdr_shortexpo_chk.u32 = pstBeReg->ISP_WDR_SHORTEXPO_CHK.u32;
    o_isp_wdr_shortexpo_chk.bits.isp_wdr_shortexpo_chk = uisp_wdr_shortexpo_chk;
    pstBeReg->ISP_WDR_SHORTEXPO_CHK.u32 = o_isp_wdr_shortexpo_chk.u32;
}

static __inline GK_VOID isp_wdr_mdt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_mdt_en)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = pstBeReg->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_mdt_en = uisp_wdr_mdt_en;
    pstBeReg->ISP_WDR_CTRL.u32 = o_isp_wdr_ctrl.u32;
}

static __inline GK_VOID isp_wdr_fusionmode_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_wdr_fusionmode)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = pstBeReg->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_fusionmode = uisp_wdr_fusionmode;
    pstBeReg->ISP_WDR_CTRL.u32 = o_isp_wdr_ctrl.u32;
}


static __inline  GK_VOID isp_dehaze_stt_size_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dehaze_stt_size)
{
    U_ISP_DEHAZE_STT_BST o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.u32 = pstBeReg->ISP_DEHAZE_STT_BST.u32;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_size = uisp_dehaze_stt_size;
    pstBeReg->ISP_DEHAZE_STT_BST.u32 = o_isp_dehaze_stt_bst.u32;
}
static __inline  GK_VOID isp_dehaze_stt_bst_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dehaze_stt_bst)
{
    U_ISP_DEHAZE_STT_BST o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.u32 = pstBeReg->ISP_DEHAZE_STT_BST.u32;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_bst = uisp_dehaze_stt_bst;
    pstBeReg->ISP_DEHAZE_STT_BST.u32 = o_isp_dehaze_stt_bst.u32;
}
static __inline  GK_VOID isp_dehaze_stt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dehaze_stt_en)
{
    U_ISP_DEHAZE_STT_BST o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.u32 = pstBeReg->ISP_DEHAZE_STT_BST.u32;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_en = uisp_dehaze_stt_en;
    pstBeReg->ISP_DEHAZE_STT_BST.u32 = o_isp_dehaze_stt_bst.u32;
}

static __inline  GK_VOID isp_dehaze_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dehaze_stt2lut_en)
{
    U_ISP_DEHAZE_STT2LUT_CFG o_isp_dehaze_stt2lut_cfg;
    o_isp_dehaze_stt2lut_cfg.bits.isp_dehaze_stt2lut_en = uisp_dehaze_stt2lut_en;
    o_isp_dehaze_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_DEHAZE_STT2LUT_CFG.u32 = o_isp_dehaze_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_dehaze_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_dehaze_stt2lut_regnew)
{
    U_ISP_DEHAZE_STT2LUT_REGNEW o_isp_dehaze_stt2lut_regnew;
    o_isp_dehaze_stt2lut_regnew.bits.isp_dehaze_stt2lut_regnew = uisp_dehaze_stt2lut_regnew;
    o_isp_dehaze_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_DEHAZE_STT2LUT_REGNEW.u32 = o_isp_dehaze_stt2lut_regnew.u32;
}



static __inline  GK_VOID isp_gamma_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_gamma_stt2lut_en)
{
    U_ISP_GAMMA_STT2LUT_CFG o_isp_gamma_stt2lut_cfg;
    o_isp_gamma_stt2lut_cfg.bits.isp_gamma_stt2lut_en = uisp_gamma_stt2lut_en;
    o_isp_gamma_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_GAMMA_STT2LUT_CFG.u32 = o_isp_gamma_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_gamma_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_gamma_stt2lut_regnew)
{
    U_ISP_GAMMA_STT2LUT_REGNEW o_isp_gamma_stt2lut_regnew;
    o_isp_gamma_stt2lut_regnew.bits.isp_gamma_stt2lut_regnew = uisp_gamma_stt2lut_regnew;
    o_isp_gamma_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_GAMMA_STT2LUT_REGNEW.u32 = o_isp_gamma_stt2lut_regnew.u32;
}

static __inline  GK_VOID isp_ca_lumathgh_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_lumathgh)
{
    U_ISP_CA_LUMATH o_isp_ca_lumath;
    o_isp_ca_lumath.u32 = pstBeReg->ISP_CA_LUMATH.u32;
    o_isp_ca_lumath.bits.isp_ca_lumathgh = uisp_ca_lumathgh;
    pstBeReg->ISP_CA_LUMATH.u32 = o_isp_ca_lumath.u32;
}
static __inline  GK_VOID isp_ca_lumath_low_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_lumath_low)
{
    U_ISP_CA_LUMATH o_isp_ca_lumath;
    o_isp_ca_lumath.u32 = pstBeReg->ISP_CA_LUMATH.u32;
    o_isp_ca_lumath.bits.isp_ca_lumath_low = uisp_ca_lumath_low;
    pstBeReg->ISP_CA_LUMATH.u32 = o_isp_ca_lumath.u32;
}
static __inline  GK_VOID isp_ca_lumaratiogh_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_lumaratiogh)
{
    U_ISP_CA_LLHC_RATIO o_isp_ca_llhc_ratio;
    o_isp_ca_llhc_ratio.u32 = pstBeReg->ISP_CA_LLHC_RATIO.u32;
    o_isp_ca_llhc_ratio.bits.isp_ca_lumaratiogh = uisp_ca_lumaratiogh;
    pstBeReg->ISP_CA_LLHC_RATIO.u32 = o_isp_ca_llhc_ratio.u32;
}
static __inline  GK_VOID isp_ca_lumaratio_low_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_lumaratio_low)
{
    U_ISP_CA_LLHC_RATIO o_isp_ca_llhc_ratio;
    o_isp_ca_llhc_ratio.u32 = pstBeReg->ISP_CA_LLHC_RATIO.u32;
    o_isp_ca_llhc_ratio.bits.isp_ca_lumaratio_low = uisp_ca_lumaratio_low;
    pstBeReg->ISP_CA_LLHC_RATIO.u32 = o_isp_ca_llhc_ratio.u32;
}
static __inline  GK_VOID isp_ca_isoratio_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_isoratio)
{
    U_ISP_CA_ISORATIO o_isp_ca_isoratio;
    o_isp_ca_isoratio.bits.isp_ca_isoratio = uisp_ca_isoratio;
    o_isp_ca_isoratio.bits.reserved_0 = 0;
    pstBeReg->ISP_CA_ISORATIO.u32 = o_isp_ca_isoratio.u32;
}
static __inline  GK_VOID isp_ca_yraratio_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_yraratio)
{
    U_ISP_CA_YRARATIO o_isp_ca_yraratio;
    o_isp_ca_yraratio.bits.isp_ca_yraratio = uisp_ca_yraratio;
    o_isp_ca_yraratio.bits.reserved_0 = 0;
    pstBeReg->ISP_CA_YRARATIO.u32 = o_isp_ca_yraratio.u32;
}

static __inline  GK_VOID isp_ca_skin_betaratio_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_skin_betaratio)
{
    U_ISP_CA_SKIN_BETARATIO o_isp_ca_skin_betaratio;
    o_isp_ca_skin_betaratio.bits.isp_ca_skin_betaratio = uisp_ca_skin_betaratio;
    o_isp_ca_skin_betaratio.bits.reserved_0 = 0;
    pstBeReg->ISP_CA_SKIN_BETARATIO.u32 = o_isp_ca_skin_betaratio.u32;
}

static __inline  GK_VOID isp_ca_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_stt2lut_en)
{
    U_ISP_CA_STT2LUT_CFG o_isp_ca_stt2lut_cfg;
    o_isp_ca_stt2lut_cfg.bits.isp_ca_stt2lut_en = uisp_ca_stt2lut_en;
    o_isp_ca_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_CA_STT2LUT_CFG.u32 = o_isp_ca_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_ca_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ca_stt2lut_regnew)
{
    U_ISP_CA_STT2LUT_REGNEW o_isp_ca_stt2lut_regnew;
    o_isp_ca_stt2lut_regnew.bits.isp_ca_stt2lut_regnew = uisp_ca_stt2lut_regnew;
    o_isp_ca_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_CA_STT2LUT_REGNEW.u32 = o_isp_ca_stt2lut_regnew.u32;
}

static __inline GK_VOID isp_ldci_wrstat_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_wrstat_en)
{
    U_ISP_LDCI_CFG o_isp_ldci_cfg;
    o_isp_ldci_cfg.u32 = pstBeReg->ISP_LDCI_CFG.u32;
    o_isp_ldci_cfg.bits.isp_ldci_wrstat_en = uisp_ldci_wrstat_en;
    pstBeReg->ISP_LDCI_CFG.u32 = o_isp_ldci_cfg.u32;
}

static __inline GK_VOID isp_ldci_rdstat_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_rdstat_en)
{
    U_ISP_LDCI_CFG o_isp_ldci_cfg;
    o_isp_ldci_cfg.u32 = pstBeReg->ISP_LDCI_CFG.u32;
    o_isp_ldci_cfg.bits.isp_ldci_rdstat_en = uisp_ldci_rdstat_en;
    pstBeReg->ISP_LDCI_CFG.u32 = o_isp_ldci_cfg.u32;
}

static __inline  GK_VOID isp_ldci_lpfstt_size_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_lpfstt_size)
{
    U_ISP_LDCI_LPFSTT_BST o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.u32 = pstBeReg->ISP_LDCI_LPFSTT_BST.u32;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_size = uisp_ldci_lpfstt_size;
    pstBeReg->ISP_LDCI_LPFSTT_BST.u32 = o_isp_ldci_lpfstt_bst.u32;
}
static __inline  GK_VOID isp_ldci_lpfstt_bst_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_lpfstt_bst)
{
    U_ISP_LDCI_LPFSTT_BST o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.u32 = pstBeReg->ISP_LDCI_LPFSTT_BST.u32;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_bst = uisp_ldci_lpfstt_bst;
    pstBeReg->ISP_LDCI_LPFSTT_BST.u32 = o_isp_ldci_lpfstt_bst.u32;
}
static __inline  GK_VOID isp_ldci_lpfstt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_lpfstt_en)
{
    U_ISP_LDCI_LPFSTT_BST o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.u32 = pstBeReg->ISP_LDCI_LPFSTT_BST.u32;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_en = uisp_ldci_lpfstt_en;
    pstBeReg->ISP_LDCI_LPFSTT_BST.u32 = o_isp_ldci_lpfstt_bst.u32;
}
static __inline  GK_VOID isp_ldci_blc_ctrl_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_blc_ctrl)
{
    U_ISP_LDCI_BLC_CTRL o_isp_ldci_blc_ctrl;
    o_isp_ldci_blc_ctrl.bits.isp_ldci_blc_ctrl = uisp_ldci_blc_ctrl;
    o_isp_ldci_blc_ctrl.bits.reserved_0 = 0;
    pstBeReg->ISP_LDCI_BLC_CTRL.u32 = o_isp_ldci_blc_ctrl.u32;
}

static __inline  GK_VOID isp_ldci_stt2lut_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_stt2lut_en)
{
    U_ISP_LDCI_STT2LUT_CFG o_isp_ldci_stt2lut_cfg;
    o_isp_ldci_stt2lut_cfg.bits.isp_ldci_stt2lut_en = uisp_ldci_stt2lut_en;
    o_isp_ldci_stt2lut_cfg.bits.reserved_0 = 0;
    pstBeReg->ISP_LDCI_STT2LUT_CFG.u32 = o_isp_ldci_stt2lut_cfg.u32;
}
static __inline  GK_VOID isp_ldci_stt2lut_regnew_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_ldci_stt2lut_regnew)
{
    U_ISP_LDCI_STT2LUT_REGNEW o_isp_ldci_stt2lut_regnew;
    o_isp_ldci_stt2lut_regnew.bits.isp_ldci_stt2lut_regnew = uisp_ldci_stt2lut_regnew;
    o_isp_ldci_stt2lut_regnew.bits.reserved_0 = 0;
    pstBeReg->ISP_LDCI_STT2LUT_REGNEW.u32 = o_isp_ldci_stt2lut_regnew.u32;
}
static __inline  GK_VOID isp_drc_wrstat_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_wrstat_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_wrstat_en = uisp_drc_wrstat_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_rdstat_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_rdstat_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_rdstat_en = uisp_drc_rdstat_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_vbiflt_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_vbiflt_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_vbiflt_en = uisp_drc_vbiflt_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_color_corr_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_color_corr_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_color_corr_en = uisp_drc_color_corr_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_mono_chroma_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_mono_chroma_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_mono_chroma_en = uisp_drc_mono_chroma_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_pregamma_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_pregamma_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_pregamma_en = uisp_drc_pregamma_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_dp_det_en_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_dp_det_en)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = pstBeReg->ISP_DRC_CFG.u32;
    o_isp_drc_cfg.bits.isp_drc_dp_det_en = uisp_drc_dp_det_en;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}
static __inline  GK_VOID isp_drc_lut_update0_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_lut_update0)
{
    U_ISP_DRC_LUT_UPDATE0 o_isp_drc_lut_update0;
    o_isp_drc_lut_update0.bits.isp_drc_lut_update0 = uisp_drc_lut_update0;
    o_isp_drc_lut_update0.bits.reserved_0 = 0;
    pstBeReg->ISP_DRC_LUT_UPDATE0.u32 = o_isp_drc_lut_update0.u32;
}
static __inline  GK_VOID isp_drc_lut_update1_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_lut_update1)
{
    U_ISP_DRC_LUT_UPDATE1 o_isp_drc_lut_update1;
    o_isp_drc_lut_update1.bits.isp_drc_lut_update1 = uisp_drc_lut_update1;
    o_isp_drc_lut_update1.bits.reserved_0 = 0;
    pstBeReg->ISP_DRC_LUT_UPDATE1.u32 = o_isp_drc_lut_update1.u32;
}
static __inline  GK_VOID isp_drc_strength_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uisp_drc_strength)
{
    U_ISP_DRC_STRENGTH o_isp_drc_strength;
    o_isp_drc_strength.bits.isp_drc_strength = uisp_drc_strength;
    o_isp_drc_strength.bits.reserved_0 = 0;
    pstBeReg->ISP_DRC_STRENGTH.u32 = o_isp_drc_strength.u32;
}

/******************************************************************************
******************************************************************************
******************************************************************************
********************VIPROC VIPROC VIPROC VIPROC VIPROC VIPROC VIPROC VIPROC VIPROC  ******************************
******************************************************************************
******************************************************************************
******************************************************************************/

static __inline  GK_VOID out0_crop_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uout0_crop_en)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.out0_crop_en = uout0_crop_en;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID isp_be_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_be_en)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.isp_be_en = uisp_be_en;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID img_pro_mode_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uimg_pro_mode)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.img_pro_mode = uimg_pro_mode;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID vcpin_sfifo_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uvcpin_sfifo_en)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.vcpin_sfifo_en = uvcpin_sfifo_en;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID wdr_fpn_mux_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uwdr_fpn_mux)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.wdr_fpn_mux = uwdr_fpn_mux;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID isp_dcg_fpn_sel_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 udcg_fpn_sel)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.dcg_fpn_sel = udcg_fpn_sel;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID ch0_mux_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uch0_mux)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.ch0_mux = uch0_mux;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID wdr_mode_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uwdr_mode)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.wdr_mode = uwdr_mode;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID video_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uvideo_en)
{
    U_ISP_VIPROC_CTRL0 o_viproc_ctrl0;
    o_viproc_ctrl0.u32 = pstViprocReg->VIPROC_CTRL0.u32;
    o_viproc_ctrl0.bits.video_en = uvideo_en;
    pstViprocReg->VIPROC_CTRL0.u32 = o_viproc_ctrl0.u32;
}
static __inline  GK_VOID isp_be_reg_up_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ureg_up)
{
    U_ISP_VIPROC_ISP_BE_REGUP o_isp_viproc_ispbe_regup;
    o_isp_viproc_ispbe_regup.bits.ispbe_reg_up = ureg_up;
    o_isp_viproc_ispbe_regup.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISPBE_REGUP.u32 = o_isp_viproc_ispbe_regup.u32;
}
static __inline  GK_VOID manual_reg_up_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 umanual_reg_up)
{
    U_ISP_VIPROC_REGUP o_viproc_regup;
    o_viproc_regup.u32 = pstViprocReg->VIPROC_REGUP.u32;
    o_viproc_regup.bits.manual_reg_up = umanual_reg_up;
    pstViprocReg->VIPROC_REGUP.u32 = o_viproc_regup.u32;
}
static __inline  GK_U32 viproc_reg_up_read(S_VIPROC_REGS_TYPE *pstViprocReg)
{
    return  pstViprocReg->VIPROC_REGUP.u32;
}

static __inline  GK_VOID isp_dpc_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dpc_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dpc_en = uisp_dpc_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_dpc_stat_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dpc_stat_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dpc_stat_en = uisp_dpc_stat_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_ge_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ge_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_ge_en = uisp_ge_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_4dg_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_4dg_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_4dg_en = uisp_4dg_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_wdr_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_wdr_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_wdr_en = uisp_wdr_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_expander_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_expander_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_expander_en = uisp_expander_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_bcom_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_bcom_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_bcom_en = uisp_bcom_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_bnr_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_bnr_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_bnr_en = uisp_bnr_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_bdec_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_bdec_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_bdec_en = uisp_bdec_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_lsc_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_lsc_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_lsc_en = uisp_lsc_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_dg_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dg_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dg_en = uisp_dg_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_af_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_af_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_af_en = uisp_af_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_awb_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_awb_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_awb_en = uisp_awb_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_ae_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ae_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_ae_en = uisp_ae_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_wb_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_wb_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_wb_en = uisp_wb_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_blc_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_blc_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_blc_en = uisp_blc_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_pregamma_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_pregamma_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_pregamma_en = uisp_pregamma_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_drc_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_drc_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_drc_en = uisp_drc_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_la_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_la_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_la_en = uisp_la_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_sqrt_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sqrt_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_sqrt_en = uisp_sqrt_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_dmnr_vhdm_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dmnr_vhdm_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dmnr_vhdm_en = uisp_dmnr_vhdm_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_dmnr_nddm_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dmnr_nddm_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dmnr_nddm_en = uisp_dmnr_nddm_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_rgbir_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_rgbir_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_rgbir_en = uisp_rgbir_en;
    pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}
static __inline  GK_VOID isp_sq_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sq_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sq_en = uisp_sq_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_cc_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_cc_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_cc_en = uisp_cc_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_sqrt1_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sqrt1_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sqrt1_en = uisp_sqrt1_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_gamma_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_gamma_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_gamma_en = uisp_gamma_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_dehaze_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dehaze_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_dehaze_en = uisp_dehaze_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_csc_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_csc_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_csc_en = uisp_csc_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_sumy_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sumy_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sumy_en = uisp_sumy_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_ldci_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ldci_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_ldci_en = uisp_ldci_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_ca_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ca_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_ca_en = uisp_ca_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_split_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_split_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_split_en = uisp_split_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_sharpen_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sharpen_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sharpen_en = uisp_sharpen_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_hcds_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_hcds_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_hcds_en = uisp_hcds_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_vcds_en_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_vcds_en)
{
    U_VIPROC_ISP_BE_CTRL1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_vcds_en = uisp_vcds_en;
    pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = o_viproc_ispbe_ctrl1.u32;
}
static __inline  GK_VOID isp_be_rggb_cfg_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_be_rggb_cfg)
{
    U_VIPROC_ISP_BE_CFG o_viproc_ispbe_cfg;
    o_viproc_ispbe_cfg.u32 = pstViprocReg->VIPROC_ISPBE_CFG.u32;
    o_viproc_ispbe_cfg.bits.isp_be_rggb_cfg = uisp_be_rggb_cfg;
    pstViprocReg->VIPROC_ISPBE_CFG.u32 = o_viproc_ispbe_cfg.u32;
}
static __inline  GK_VOID isp_be_timing_cfg_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_be_timing_cfg)
{
    U_VIPROC_ISP_BE_CFG o_viproc_ispbe_cfg;
    o_viproc_ispbe_cfg.u32 = pstViprocReg->VIPROC_ISPBE_CFG.u32;
    o_viproc_ispbe_cfg.bits.isp_be_timing_cfg = uisp_be_timing_cfg;
    pstViprocReg->VIPROC_ISPBE_CFG.u32 = o_viproc_ispbe_cfg.u32;
}

static __inline  GK_VOID eof_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ueof_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.eof_mask = ueof_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID eof_end_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ueof_end_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.eof_end_mask = ueof_end_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID timeout_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 utimeout_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.timeout_mask = utimeout_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID wbus_err_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uwbus_err_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.wbus_err_mask = uwbus_err_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID rbus_err_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 urbus_err_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.rbus_err_mask = urbus_err_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID wrap_overflow_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uwrap_overflow_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.wrap_overflow_mask = uwrap_overflow_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID apb_err_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uapb_err_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.apb_err_mask = uapb_err_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID dcmp_err_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 udcmp_err_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.dcmp_err_mask = udcmp_err_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID flow_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uflow_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.flow_mask = uflow_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID frm_err_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ufrm_err_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.frm_err_mask = ufrm_err_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID frm_start_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ufrm_start_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.frm_start_mask = ufrm_start_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID tunl_finish_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 utunl_finish_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.tunl_finish_mask = utunl_finish_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID cfg_loss_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ucfg_loss_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.cfg_loss_mask = ucfg_loss_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID load_lut_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uload_lut_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.load_lut_mask = uload_lut_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_fstart_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_fstart_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_fstart_mask = uispbe_fstart_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_update_cfg_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_update_cfg_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_update_cfg_mask = uispbe_update_cfg_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_cfg_loss_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_cfg_loss_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_cfg_loss_mask = uispbe_cfg_loss_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_ae_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_ae_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_ae_mask = uispbe_ae_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_awb_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_awb_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_awb_mask = uispbe_awb_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_af_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_af_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_af_mask = uispbe_af_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_dis_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_dis_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_dis_mask = uispbe_dis_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_ldci_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_ldci_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_ldci_mask = uispbe_ldci_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_dehaze_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_dehaze_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_dehaze_mask = uispbe_dehaze_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_flick_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_flick_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_flick_mask = uispbe_flick_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_mg_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_mg_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_mg_mask = uispbe_mg_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_drc_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_drc_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_drc_mask = uispbe_drc_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_fpn_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_fpn_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_fpn_mask = uispbe_fpn_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID ispbe_dcg_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uispbe_dcg_mask)
{
    U_ISP_VIPROC_INTMASK o_viproc_intmask;
    o_viproc_intmask.u32 = pstViprocReg->VIPROC_INTMASK.u32;
    o_viproc_intmask.bits.ispbe_dcg_mask = uispbe_dcg_mask;
    pstViprocReg->VIPROC_INTMASK.u32 = o_viproc_intmask.u32;
}
static __inline  GK_VOID af_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uaf_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.af_int_mask = uaf_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID ae_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uae_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.ae_int_mask = uae_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID awb_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uawb_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.awb_int_mask = uawb_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID dis_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 udis_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.dis_int_mask = udis_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID la_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 ula_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.la_int_mask = ula_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID ge_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uge_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.ge_int_mask = uge_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID dehaze_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 udehaze_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.dehaze_int_mask = udehaze_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID flick_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uflick_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.flick_int_mask = uflick_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}
static __inline  GK_VOID ldci_int_mask_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uldci_int_mask)
{
    U_ISP_VIPROC_INT1MASK o_viproc_int1mask;
    o_viproc_int1mask.u32 = pstViprocReg->VIPROC_INT1MASK.u32;
    o_viproc_int1mask.bits.ldci_int_mask = uldci_int_mask;
    pstViprocReg->VIPROC_INT1MASK.u32 = o_viproc_int1mask.u32;
}

static __inline GK_VOID  viproc_para_drc_addr_low_write(S_VIPROC_REGS_TYPE *pstViProcReg, GK_U32 upara_drc_addr_low)
{
    pstViProcReg->PARA_DRC_ADDR_LOW.u32 = upara_drc_addr_low;
}

static __inline GK_VOID  viproc_para_dci_addr_low_write(S_VIPROC_REGS_TYPE *pstViProcReg, GK_U32 upara_dci_addr_low)
{
    pstViProcReg->PARA_DCI_ADDR_LOW.u32 = upara_dci_addr_low;
}

static __inline GK_VOID  viproc_out_para_drc_addr_low_write(S_VIPROC_REGS_TYPE *pstViProcReg, GK_U32 uout_para_drc_addr_low)
{
    pstViProcReg->OUT_PARA_DRC_ADDR_LOW.u32 = uout_para_drc_addr_low;
}

static __inline GK_VOID  viproc_out_para_dci_addr_low_write(S_VIPROC_REGS_TYPE *pstViProcReg, GK_U32 uout_para_dci_addr_low)
{
    pstViProcReg->OUT_PARA_DCI_ADDR_LOW.u32 = uout_para_dci_addr_low;
}

static __inline  GK_VOID isp_bnr_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_bnr_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_BNR_LUT_ADDR_LOW.u32 = uisp_bnr_lut_addr_low;
}
static __inline  GK_VOID isp_bnr_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_bnr_lut_width_word)
{
    U_VIPROC_ISP_BE_BNR_LUT_WIDTH_WORD o_viproc_isp_bnr_lut_width_word;
    o_viproc_isp_bnr_lut_width_word.bits.isp_bnr_lut_width_word = uisp_bnr_lut_width_word;
    o_viproc_isp_bnr_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_BNR_LUT_WIDTH_WORD.u32 = o_viproc_isp_bnr_lut_width_word.u32;
}
static __inline  GK_VOID isp_lsc_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_lsc_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_LSC_LUT_ADDR_LOW.u32 = uisp_lsc_lut_addr_low;
}
static __inline  GK_VOID isp_lsc_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_lsc_lut_width_word)
{
    U_VIPROC_ISP_BE_LSC_LUT_WIDTH_WORD o_viproc_isp_lsc_lut_width_word;
    o_viproc_isp_lsc_lut_width_word.bits.isp_lsc_lut_width_word = uisp_lsc_lut_width_word;
    o_viproc_isp_lsc_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_LSC_LUT_WIDTH_WORD.u32 = o_viproc_isp_lsc_lut_width_word.u32;
}

static __inline  GK_VOID isp_gamma_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_gamma_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_GAMMA_LUT_ADDR_LOW.u32 = uisp_gamma_lut_addr_low;
}
static __inline  GK_VOID isp_gamma_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_gamma_lut_width_word)
{
    U_VIPROC_ISP_BE_GAMMA_LUT_WIDTH_WORD o_viproc_isp_gamma_lut_width_word;
    o_viproc_isp_gamma_lut_width_word.bits.isp_gamma_lut_width_word = uisp_gamma_lut_width_word;
    o_viproc_isp_gamma_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_GAMMA_LUT_WIDTH_WORD.u32 = o_viproc_isp_gamma_lut_width_word.u32;
}
static __inline  GK_VOID isp_dehaze_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dehaze_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_DEHAZE_LUT_ADDR_LOW.u32 = uisp_dehaze_lut_addr_low;
}
static __inline  GK_VOID isp_dehaze_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_dehaze_lut_width_word)
{
    U_VIPROC_ISP_BE_DEHAZE_LUT_WIDTH_WORD o_viproc_isp_dehaze_lut_width_word;
    o_viproc_isp_dehaze_lut_width_word.bits.isp_dehaze_lut_width_word = uisp_dehaze_lut_width_word;
    o_viproc_isp_dehaze_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_DEHAZE_LUT_WIDTH_WORD.u32 = o_viproc_isp_dehaze_lut_width_word.u32;
}
static __inline  GK_VOID isp_ldci_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ldci_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_LDCI_LUT_ADDR_LOW.u32 = uisp_ldci_lut_addr_low;
}
static __inline  GK_VOID isp_ldci_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ldci_lut_width_word)
{
    U_VIPROC_ISP_BE_LDCI_LUT_WIDTH_WORD o_viproc_isp_ldci_lut_width_word;
    o_viproc_isp_ldci_lut_width_word.bits.isp_ldci_lut_width_word = uisp_ldci_lut_width_word;
    o_viproc_isp_ldci_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_LDCI_LUT_WIDTH_WORD.u32 = o_viproc_isp_ldci_lut_width_word.u32;
}
static __inline  GK_VOID isp_ca_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ca_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_CA_LUT_ADDR_LOW.u32 = uisp_ca_lut_addr_low;
}
static __inline  GK_VOID isp_ca_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_ca_lut_width_word)
{
    U_VIPROC_ISP_BE_CA_LUT_WIDTH_WORD o_viproc_isp_ca_lut_width_word;
    o_viproc_isp_ca_lut_width_word.bits.isp_ca_lut_width_word = uisp_ca_lut_width_word;
    o_viproc_isp_ca_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_CA_LUT_WIDTH_WORD.u32 = o_viproc_isp_ca_lut_width_word.u32;
}
static __inline  GK_VOID isp_sharpen_lut_addr_low_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sharpen_lut_addr_low)
{
    pstViprocReg->VIPROC_ISP_SHARPEN_LUT_ADDR_LOW.u32 = uisp_sharpen_lut_addr_low;
}
static __inline  GK_VOID isp_sharpen_lut_width_word_write(S_VIPROC_REGS_TYPE *pstViprocReg, GK_U32 uisp_sharpen_lut_width_word)
{
    U_VIPROC_ISP_BE_SHARPEN_LUT_WIDTH_WORD o_viproc_isp_sharpen_lut_width_word;
    o_viproc_isp_sharpen_lut_width_word.bits.isp_sharpen_lut_width_word = uisp_sharpen_lut_width_word;
    o_viproc_isp_sharpen_lut_width_word.bits.reserved_0 = 0;
    pstViprocReg->VIPROC_ISP_SHARPEN_LUT_WIDTH_WORD.u32 = o_viproc_isp_sharpen_lut_width_word.u32;
}

static __inline  GK_U8 isp_fe_colorbar_en_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_FE_CTRL o_vicap_ispfe_ctrl;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_vicap_ispfe_ctrl.u32 = pstFeReg->ISP_FE_CTRL.u32;
    return o_vicap_ispfe_ctrl.bits.isp_colorbar_en;
}
static __inline  GK_U8 isp_fe_vsync_mode_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_FE_ADAPTER_CFG o_vicap_fe_adapter_cfg;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_vicap_fe_adapter_cfg.u32 = pstFeReg->ISP_FE_ADAPTER_CFG.u32;
    return o_vicap_fe_adapter_cfg.bits.fe_vsync_mode;
}
static __inline  GK_U8 isp_fe_rggb_cfg_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_FE_CTRL_F o_isp_fe_ctrl_f;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_isp_fe_ctrl_f.u32 = pstFeReg->ISP_FE_CTRL_F.u32;
    return o_isp_fe_ctrl_f.bits.isp_fe_rggb_cfg;
}
static __inline  GK_U8 isp_fe_ae1_skip_x_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_SKIP_CRG ISP_AE1_SKIP_CRG;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_SKIP_CRG.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    return ISP_AE1_SKIP_CRG.bits.isp_ae_skip_x ;
}
static __inline  GK_U8 isp_fe_ae1_offset_x_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_SKIP_CRG ISP_AE1_SKIP_CRG;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_SKIP_CRG.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    return ISP_AE1_SKIP_CRG.bits.isp_ae_offset_x;
}
static __inline  GK_U8 isp_fe_ae1_skip_y_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_SKIP_CRG ISP_AE1_SKIP_CRG;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_SKIP_CRG.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    return ISP_AE1_SKIP_CRG.bits.isp_ae_skip_y;
}
static __inline  GK_U8 isp_fe_ae1_offset_y_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_SKIP_CRG ISP_AE1_SKIP_CRG;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_SKIP_CRG.u32 = pstFeReg->ISP_AE1_SKIP_CRG.u32;
    return ISP_AE1_SKIP_CRG.bits.isp_ae_offset_y;
}
static __inline  GK_U32 isp_fe_ae1_pixel_selected_count_read(GK_U8 ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_TOTAL_STAT ISP_AE1_TOTAL_STAT;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_TOTAL_STAT.u32 = pstFeReg->ISP_AE1_TOTAL_STAT.u32;
    return (ISP_AE1_TOTAL_STAT.bits.isp_ae_total_pixels);
}
static __inline  GK_U32 isp_fe_ae1_pixel_selected_weight_read(GK_U8 ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_COUNT_STAT ISP_AE1_COUNT_STAT;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_COUNT_STAT.u32 = pstFeReg->ISP_AE1_COUNT_STAT.u32;
    return (ISP_AE1_COUNT_STAT.bits.isp_ae_count_pixels);
}
static __inline  GK_U32 isp_fe_ae1st_rdata_read(GK_U8 ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_HIST_RDATA ISP_AE1_HIST_RDATA;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_HIST_RDATA.u32 = pstFeReg->ISP_AE1_HIST_RDATA.u32;
    return ISP_AE1_HIST_RDATA.u32;
}
static __inline  GK_U32 isp_fe_ae1_aver_r_gr_rdata_read(GK_U8 ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_AVER_R_GR_RDATA ISP_AE1_AVER_R_GR_RDATA;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_AVER_R_GR_RDATA.u32 = pstFeReg->ISP_AE1_AVER_R_GR_RDATA.u32;
    return ISP_AE1_AVER_R_GR_RDATA.u32;
}
static __inline  GK_U32 isp_fe_ae1_aver_gb_b_rdata_read(GK_U8 ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_AE1_AVER_GB_B_RDATA ISP_AE1_AVER_GB_B_RDATA;
    ISP_CHECK_NULLPTR(pstFeReg);
    ISP_AE1_AVER_GB_B_RDATA.u32 = pstFeReg->ISP_AE1_AVER_GB_B_RDATA.u32;
    return ISP_AE1_AVER_GB_B_RDATA.u32;
}
static __inline  GK_U32 isp_fe_ae1_wei_wdata_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    ISP_CHECK_NULLPTR(pstFeReg);
    return pstFeReg->ISP_AE1_WEIGHT_WDATA.u32;
}
static __inline  GK_U8 isp_fe_ae1_wei_raddr_write(VI_PIPE ViPipe, GK_U32 uisp_ae1_wei_raddr)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    ISP_CHECK_NULLPTR(pstFeReg);
    pstFeReg->ISP_AE1_WEIGHT_RADDR.u32 = uisp_ae1_wei_raddr;
    return 1;
}
static __inline  GK_U32 isp_fe_ae1_wei_rdata_read(GK_U8 ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    ISP_CHECK_NULLPTR(pstFeReg);
    return pstFeReg->ISP_AE1_WEIGHT_RDATA.u32;
}
static __inline  GK_U16 isp_crop_width_out_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_CROP_SIZEOUT o_isp_crop_sizeout;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_isp_crop_sizeout.u32 = pstFeReg->ISP_CROP_SIZEOUT.u32;
    return o_isp_crop_sizeout.bits.isp_crop_width_out;
}
static __inline  GK_U16 isp_crop_height_out_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_CROP_SIZEOUT o_isp_crop_sizeout;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_isp_crop_sizeout.u32 = pstFeReg->ISP_CROP_SIZEOUT.u32;
    return o_isp_crop_sizeout.bits.isp_crop_height_out;
}
static __inline  GK_U16 isp_crop_pos_x_read(VI_PIPE ViPipe)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_CROP_POS o_isp_crop_pos;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_isp_crop_pos.u32 = pstFeReg->ISP_CROP_POS.u32;
    return o_isp_crop_pos.bits.isp_crop_pos_x;
}
static __inline  GK_U16 isp_crop_pos_y_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPFE_REGS_TYPE *pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipe);
    U_ISP_CROP_POS o_isp_crop_pos;
    ISP_CHECK_NULLPTR(pstFeReg);
    o_isp_crop_pos.u32 = pstFeReg->ISP_CROP_POS.u32;
    return o_isp_crop_pos.bits.isp_crop_pos_y;
}
static __inline  GK_U16 isp_wb_grgain_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_WB_GAIN1 o_isp_wb_gain1;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_wb_gain1.u32 = pstBeReg->ISP_WB_GAIN1.u32;
    return o_isp_wb_gain1.bits.isp_wb_grgain;
}
static __inline  GK_U16 isp_wb_rgain_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_WB_GAIN1 o_isp_wb_gain1;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_wb_gain1.u32 = pstBeReg->ISP_WB_GAIN1.u32;
    return o_isp_wb_gain1.bits.isp_wb_rgain;
}
static __inline  GK_U16 isp_wb_gbgain_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_WB_GAIN2 o_isp_wb_gain2;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_wb_gain2.u32 = pstBeReg->ISP_WB_GAIN2.u32;
    return o_isp_wb_gain2.bits.isp_wb_gbgain;
}
static __inline  GK_U16 isp_wb_bgain_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_WB_GAIN2 o_isp_wb_gain2;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_wb_gain2.u32 = pstBeReg->ISP_WB_GAIN2.u32;
    return o_isp_wb_gain2.bits.isp_wb_bgain;
}
static __inline  GK_U16 isp_dg_ofsr_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_DG_BLC_OFFSET1 ISP_DG_BLC_OFFSET1;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_DG_BLC_OFFSET1.u32 = pstBeReg->ISP_DG_BLC_OFFSET1.u32;
    return ISP_DG_BLC_OFFSET1.bits.isp_dg_ofsr;
}
static __inline  GK_U16 isp_dg_ofsb_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_DG_BLC_OFFSET2 ISP_DG_BLC_OFFSET2;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_DG_BLC_OFFSET2.u32 = pstBeReg->ISP_DG_BLC_OFFSET2.u32;
    return ISP_DG_BLC_OFFSET2.bits.isp_dg_ofsb;
}
static __inline  GK_U8 isp_ae_skip_x_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CFG;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_SKIP_CFG.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    return ISP_AE_SKIP_CFG.bits.isp_ae_skip_x;
}
static __inline  GK_U8 isp_ae_offset_x_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CFG;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_SKIP_CFG.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    return ISP_AE_SKIP_CFG.bits.isp_ae_offset_x;
}
static __inline  GK_U8 isp_ae_skip_y_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CFG;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_SKIP_CFG.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    return ISP_AE_SKIP_CFG.bits.isp_ae_skip_y;
}
static __inline  GK_U8 isp_ae_offset_y_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CFG;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_SKIP_CFG.u32 = pstBeReg->ISP_AE_SKIP_CRG.u32;
    return ISP_AE_SKIP_CFG.bits.isp_ae_offset_y;
}
static __inline  GK_U32 isp_ae_pixel_selected_count_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_TOTAL_STAT ISP_AE_TOTAL_STAT;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_TOTAL_STAT.u32 = pstBeReg->ISP_AE_TOTAL_STAT.u32;
    return ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
}
static __inline  GK_U32 isp_ae_pixel_selected_weight_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_COUNT_STAT ISP_AE_COUNT_STAT;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_COUNT_STAT.u32 = pstBeReg->ISP_AE_COUNT_STAT.u32;
    return ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
}
static __inline  GK_U32 isp_ae_total_r_aver_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_TOTAL_R_AVER ISP_AE_TOTAL_R_AVER;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_TOTAL_R_AVER.u32 = pstBeReg->ISP_AE_TOTAL_R_AVER.u32;
    return ISP_AE_TOTAL_R_AVER.bits.isp_ae_total_r_aver;
}
static __inline  GK_U32 isp_ae_total_gr_aver_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_TOTAL_GR_AVER ISP_AE_TOTAL_GR_AVER;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_TOTAL_GR_AVER.u32 = pstBeReg->ISP_AE_TOTAL_GR_AVER.u32;
    return ISP_AE_TOTAL_GR_AVER.bits.isp_ae_total_gr_aver;
}
static __inline  GK_U32 isp_ae_total_gb_aver_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_TOTAL_GB_AVER ISP_AE_TOTAL_GB_AVER;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_TOTAL_GB_AVER.u32 = pstBeReg->ISP_AE_TOTAL_GB_AVER.u32;
    return ISP_AE_TOTAL_GB_AVER.bits.isp_ae_total_gb_aver;
}
static __inline  GK_U32 isp_ae_total_b_aver_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_TOTAL_B_AVER ISP_AE_TOTAL_B_AVER;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_TOTAL_B_AVER.u32 = pstBeReg->ISP_AE_TOTAL_B_AVER.u32;
    return ISP_AE_TOTAL_B_AVER.bits.isp_ae_total_b_aver;
}
static __inline  GK_U8 isp_ae_four_plane_mode_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_BITMOVE ISP_AE_BITMOVE;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_BITMOVE.u32 = pstBeReg->ISP_AE_BITMOVE.u32;
    return ISP_AE_BITMOVE.bits.isp_ae_fourplanemode;
}
static __inline  GK_U32 isp_aest_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_HIST_RDATA ISP_AE_HIST_RDATA;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_HIST_RDATA.u32 = pstBeReg->ISP_AE_HIST_RDATA.u32;
    return ISP_AE_HIST_RDATA.u32;
}
static __inline  GK_U32 isp_ae_aver_r_gr_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_AVER_R_GR_RDATA ISP_AE_AVER_R_GR_RDATA;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_AVER_R_GR_RDATA.u32 = pstBeReg->ISP_AE_AVER_R_GR_RDATA.u32;
    return ISP_AE_AVER_R_GR_RDATA.u32;
}
static __inline  GK_U32 isp_ae_aver_gb_b_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_AVER_GB_B_RDATA ISP_AE_AVER_GB_B_RDATA;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_AVER_GB_B_RDATA.u32 = pstBeReg->ISP_AE_AVER_GB_B_RDATA.u32;
    return ISP_AE_AVER_GB_B_RDATA.u32;
}
static __inline  GK_U32 isp_ae_wei_wdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_WEIGHT_WDATA ISP_AE_WEIGHT_WDATA;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_WEIGHT_WDATA.u32 = pstBeReg->ISP_AE_WEIGHT_WDATA.u32;
    return ISP_AE_WEIGHT_WDATA.u32;
}
static __inline  GK_U32 isp_ae_wei_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AE_WEIGHT_RDATA ISP_AE_WEIGHT_RDATA;
    ISP_CHECK_NULLPTR(pstBeReg);
    ISP_AE_WEIGHT_RDATA.u32 = pstBeReg->ISP_AE_WEIGHT_RDATA.u32;
    return ISP_AE_WEIGHT_RDATA.u32;
}
static __inline  GK_U16 isp_awb_threshold_min_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_THD_MIN o_isp_awb_thd_min;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_thd_min.u32 = pstBeReg->ISP_AWB_THD_MIN.u32;
    return o_isp_awb_thd_min.bits.isp_awb_threshold_min;
}
static __inline  GK_U16 isp_awb_threshold_max_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_THD_MAX o_isp_awb_thd_max;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_thd_max.u32 = pstBeReg->ISP_AWB_THD_MAX.u32;
    return o_isp_awb_thd_max.bits.isp_awb_threshold_max;
}
static __inline  GK_U16 isp_awb_cr_ref_max_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_CR_MM o_isp_awb_cr_mm;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_cr_mm.u32 = pstBeReg->ISP_AWB_CR_MM.u32;
    return o_isp_awb_cr_mm.bits.isp_awb_cr_ref_max ;
}
static __inline  GK_U16 isp_awb_cr_ref_min_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_CR_MM o_isp_awb_cr_mm;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_cr_mm.u32 = pstBeReg->ISP_AWB_CR_MM.u32;
    return o_isp_awb_cr_mm.bits.isp_awb_cr_ref_min;
}
static __inline  GK_U16 isp_awb_cb_ref_max_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_CB_MM o_isp_awb_cb_mm;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_cb_mm.u32 = pstBeReg->ISP_AWB_CB_MM.u32;
    return o_isp_awb_cb_mm.bits.isp_awb_cb_ref_max ;
}
static __inline  GK_U16 isp_awb_cb_ref_min_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_CB_MM o_isp_awb_cb_mm;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_cb_mm.u32 = pstBeReg->ISP_AWB_CB_MM.u32;
    return o_isp_awb_cb_mm.bits.isp_awb_cb_ref_min;
}
static __inline  GK_U32 isp_awb_stat_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_AWB_STAT_RDATA o_isp_awb_stat_rdata;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_awb_stat_rdata.u32 = pstBeReg->ISP_AWB_STAT_RDATA.u32;
    return o_isp_awb_stat_rdata.bits.isp_awb_stat_rdata;
}
static __inline  GK_U16 isp_dpc_line_thresh_1_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    ISP_CHECK_NULLPTR(pstBeReg);
    return (pstBeReg->ISP_DPC_LINE_THRESH_1.u32 & 0xFFFF);
}
static __inline  GK_U16 isp_dpc_line_mad_fac_1_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    ISP_CHECK_NULLPTR(pstBeReg);
    return (pstBeReg->ISP_DPC_LINE_MAD_FAC_1.u32 & 0x3FFF);
}
static __inline  GK_U32 isp_dehaze_minstat_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_DEHAZE_MINSTAT_RDATA o_isp_dehaze_minstat_rdata;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_dehaze_minstat_rdata.u32 = pstBeReg->ISP_DEHAZE_MINSTAT_RDATA.u32;
    return o_isp_dehaze_minstat_rdata.u32;
}
static __inline  GK_U32 isp_dehaze_maxstat_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_DEHAZE_MAXSTAT_RDATA o_isp_dehaze_maxstat_rdata;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_dehaze_maxstat_rdata.u32 = pstBeReg->ISP_DEHAZE_MAXSTAT_RDATA.u32;
    o_isp_dehaze_maxstat_rdata.bits.isp_dehaze_maxstat_rdata =  pstBeReg->ISP_DEHAZE_MAXSTAT_RDATA.bits.isp_dehaze_maxstat_rdata;
    return o_isp_dehaze_maxstat_rdata.bits.isp_dehaze_maxstat_rdata;
}
static __inline  GK_U32 isp_dehaze_prestat_rdata_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    ISP_CHECK_NULLPTR(pstBeReg);
    return pstBeReg->ISP_DEHAZE_PRESTAT_RDATA.u32;
}
static __inline  GK_U8 isp_drc_strength_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    U_ISP_DRC_STRENGTH o_isp_drc_strength;
    ISP_CHECK_NULLPTR(pstBeReg);
    o_isp_drc_strength.u32 = pstBeReg->ISP_DRC_STRENGTH.u32;
    return o_isp_drc_strength.bits.isp_drc_strength;
}
static __inline  GK_U8 isp_fpn_en_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_VIPROC_REGS_TYPE *pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, BlkDev);
    ISP_CHECK_NULLPTR(pstViProcReg);
    return pstViProcReg->VIPROC_ISPBE_CTRL0.bits.isp_fpn_en;
}
static __inline  GK_U8 isp_dg_en_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_VIPROC_REGS_TYPE *pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, BlkDev);
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    ISP_CHECK_NULLPTR(pstViProcReg);
    o_viproc_ispbe_ctrl0.u32 = pstViProcReg->VIPROC_ISPBE_CTRL0.u32;
    return o_viproc_ispbe_ctrl0.bits.isp_dg_en;
}
static __inline  GK_U8 isp_drc_en_read(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    S_VIPROC_REGS_TYPE *pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, BlkDev);
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    ISP_CHECK_NULLPTR(pstViProcReg);
    o_viproc_ispbe_ctrl0.u32 = pstViProcReg->VIPROC_ISPBE_CTRL0.u32;
    return o_viproc_ispbe_ctrl0.bits.isp_drc_en;
}
static __inline  GK_U32 isp_fpn_corr_gainoffset_read(VI_PIPE ViPipe, BLK_DEV BlkDev, GK_U8 index)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    ISP_CHECK_NULLPTR(pstBeReg);
    switch (index) {
        case 0:
            return pstBeReg->ISP_FPN_CORR0.u32;
        default:
            return 0;
    }
}



static __inline  GK_VOID isp_ae_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_ae_update_index)
{
    pstBeReg->reserved_0[0] = uisp_ae_update_index;
}
static __inline  GK_U32 isp_ae_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[0];
}
static __inline  GK_VOID isp_awb_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_awb_update_index)
{
    pstBeReg->reserved_0[1] = uisp_awb_update_index;
}
static __inline  GK_U32 isp_awb_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[1];
}
static __inline  GK_VOID isp_af_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_af_update_index)
{
    pstBeReg->reserved_0[2] = uisp_af_update_index;
}
static __inline  GK_U32 isp_af_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[2];
}
static __inline  GK_VOID isp_dpc_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_dpc_update_index)
{
    pstBeReg->reserved_0[3] = uisp_dpc_update_index;
}
static __inline  GK_U32 isp_dpc_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[3];
}
static __inline  GK_VOID isp_ge_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_ge_update_index)
{
    pstBeReg->reserved_0[4] = uisp_ge_update_index;
}
static __inline  GK_U32 isp_ge_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[4];
}
static __inline  GK_VOID isp_expander_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_expander_update_index)
{
    pstBeReg->reserved_0[5] = uisp_expander_update_index;
}
static __inline  GK_U32 isp_expander_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[5];
}
static __inline  GK_VOID isp_wdr_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_wdr_update_index)
{
    pstBeReg->reserved_0[6] = uisp_wdr_update_index;
}
static __inline  GK_U32 isp_wdr_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[6];
}
static __inline  GK_VOID isp_bayernr_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_bayernr_update_index)
{
    pstBeReg->reserved_0[7] = uisp_bayernr_update_index;
}
static __inline  GK_U32 isp_bayernr_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[7];
}
static __inline  GK_VOID isp_split_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_Split_update_index)
{
    pstBeReg->reserved_0[8] = uisp_Split_update_index;
}
static __inline  GK_U32 isp_split_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[8];
}
static __inline  GK_VOID isp_lsc_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_lsc_update_index)
{
    pstBeReg->reserved_0[9] = uisp_lsc_update_index;
}
static __inline  GK_U32 isp_lsc_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[9];
}
static __inline  GK_VOID isp_drc_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_drc_update_index)
{
    pstBeReg->reserved_0[10] = uisp_drc_update_index;
}
static __inline  GK_U32 isp_drc_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[10];
}
static __inline  GK_VOID isp_dehaze_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_dehaze_update_index)
{
    pstBeReg->reserved_0[11] = uisp_dehaze_update_index;
}
static __inline  GK_U32 isp_dehaze_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[11];
}
static __inline  GK_VOID isp_lcac_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_lcac_update_index)
{
    pstBeReg->reserved_0[12] = uisp_lcac_update_index;
}
static __inline  GK_U32 isp_lcac_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[12];
}
static __inline  GK_VOID isp_gcac_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_gcac_update_index)
{
    pstBeReg->reserved_0[13] = uisp_gcac_update_index;
}
static __inline  GK_U32 isp_gcac_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[13];
}
static __inline  GK_VOID isp_demosaic_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_demosaic_update_index)
{
    pstBeReg->reserved_0[14] = uisp_demosaic_update_index;
}
static __inline  GK_U32 isp_demosaic_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[14];
}
static __inline  GK_VOID isp_gamma_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_gamma_update_index)
{
    pstBeReg->reserved_0[15] = uisp_gamma_update_index;
}
static __inline  GK_U32 isp_gamma_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[15];
}
static __inline  GK_VOID isp_ca_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_ca_update_index)
{
    pstBeReg->reserved_0[16] = uisp_ca_update_index;
}
static __inline  GK_U32 isp_ca_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[16];
}
static __inline  GK_VOID isp_sharpen_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_sharpen_update_index)
{
    pstBeReg->reserved_0[17] = uisp_sharpen_update_index;
}
static __inline  GK_U32 isp_sharpen_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[17];
}
static __inline  GK_VOID isp_ldci_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_ldci_update_index)
{
    pstBeReg->reserved_0[18] = uisp_ldci_update_index;
}
static __inline  GK_U32 isp_ldci_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[18];
}
static __inline  GK_VOID isp_pregamma_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_pregamma_update_index)
{
    pstBeReg->reserved_0[19] = uisp_pregamma_update_index;
}
static __inline  GK_U32 isp_pregamma_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[19];
}
static __inline  GK_VOID isp_clut_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_clut_update_index)
{
    pstBeReg->reserved_0[20] = uisp_clut_update_index;
}
static __inline  GK_U32 isp_clut_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[20];
}
static __inline  GK_VOID isp_rlsc_update_index_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32  uisp_rlsc_update_index)
{
    pstBeReg->reserved_0[21] = uisp_rlsc_update_index;
}
static __inline  GK_U32 isp_rlsc_update_index_read(S_ISPBE_REGS_TYPE *pstBeReg)
{
    return pstBeReg->reserved_0[21];
}
#define BITS_MASK(msb, lsb) (GK_U32)(((1ULL << (msb+1)) - 1) & ~((1ULL << lsb) - 1))
#define BITS_SHIFT(data, msb, lsb) (GK_U32)((data & (BITS_MASK(msb, lsb) >> lsb)) << lsb)
#define REG_BITS_MODIFY(wrfunc, rdfunc, offset, msb, lsb, data) \
    do {                                                        \
        GK_U32 u32Cfg = rdfunc(offset);                         \
        wrfunc(offset, ((BITS_SHIFT(data, msb, lsb)) | (u32Cfg & ~ BITS_MASK(msb, lsb)))); \
    } while (0);
#define REG_BITS_READ(wrfunc, rdfunc, offset, msb, lsb) \
    ((rdfunc(offset) & BITS_MASK(msb, lsb)) >> lsb)
#define COMPLEMENT_ALIGN32(data, signbit) \
    data & (1ULL << signbit) ? (data | BITS_MASK(31, (signbit+1))) : data;
static __inline  GK_VOID isp_slave_mode_configs_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x0, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_configs_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x0, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_vstime_low_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x8, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_vstime_low_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x8, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_vstimegh_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x4, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_vstimegh_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x4, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_hstime_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0xc, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_hstime_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0xc, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_vscyc_low_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x1c, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_vscyc_low_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x1c, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_vscycgh_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x18, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_vscycgh_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x18, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_hscyc_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x20, 31, 0, data);
}
static __inline  GK_U32 isp_slave_mode_hscyc_read(SLAVE_DEV SlaveDev)
{
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, CAP_SLAVE_REG_BASE(SlaveDev) + 0x20, 31, 0);
}
static __inline  GK_VOID isp_slave_mode_time_cfg_select_write(SLAVE_DEV SlaveDev, GK_U32 data)
{
    switch (SlaveDev) {
        case 0:
            REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, SLAVE_MODE_REG_BASE, 1, 0, data);
            break;
        case 1:
            REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, SLAVE_MODE_REG_BASE, 3, 2, data);
            break;
        default:
            break;
    }
}
static __inline  GK_U32 isp_slave_mode_time_cfg_select_read(SLAVE_DEV SlaveDev)
{
    switch (SlaveDev) {
        case 0:
            return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, SLAVE_MODE_REG_BASE, 1, 0);
        case 1:
            return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, SLAVE_MODE_REG_BASE, 3, 2);
        default:
            return 0;
    }
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif
