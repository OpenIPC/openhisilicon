/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: hifb coef data structure
 */

#ifndef __HIFB_COEF_H__
#define __HIFB_COEF_H__

#include "hi_type.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define VOU_ZOOM_COEF_ITEM   18 /* 18 sets of scaling factors per group */
#define VOU_ZOOM_COEF_MAXTAP 8  /* The maximum order of each term is 8tap */

#define HIFB_COEF_BIT_RANGE 12
typedef enum {
    VOU_ZOOM_COEF_1 = 0,
    VOU_ZOOM_COEF_EQU_1,
    VOU_ZOOM_COEF_075,
    VOU_ZOOM_COEF_05,
    VOU_ZOOM_COEF_033,
    VOU_ZOOM_COEF_025,
    VOU_ZOOM_COEF_0,
    VOU_ZOOM_COEF_BUTT
} vou_zoom_coef;

typedef enum {
    VOU_ZOOM_TAP_8LH = 0,
    VOU_ZOOM_TAP_4CH,
    VOU_ZOOM_TAP_6LV,
    VOU_ZOOM_TAP_4LV,
    VOU_ZOOM_TAP_4CV,
    VOU_ZOOM_TAP_BUTT
} vou_zoom_tap;

typedef struct {
    hi_s32 bits_0 : 10;
    hi_s32 bits_1 : 10;
    hi_s32 bits_2 : 10;
    hi_s32 bits_32 : 2;
    hi_s32 bits_38 : 8;
    hi_s32 bits_4 : 10;
    hi_s32 bits_5 : 10;
    hi_s32 bits_64 : 4;
    hi_s32 bits_66 : 6;
    hi_s32 bits_7 : 10;
    hi_s32 bits_8 : 10;
    hi_s32 bits_96 : 6;
    hi_s32 bits_94 : 4;
    hi_s32 bits_10 : 10;
    hi_s32 bits_11 : 10;
    hi_s32 bits_12 : 8;
} vo_zoom_bit;

typedef struct {
    hi_u32 size;
    vo_zoom_bit bit[HIFB_COEF_BIT_RANGE];
} vo_zoom_bit_array;

/* COLOR SPACE CONVERT DEFINITION */
typedef struct {
    hi_s32 csc_coef00;
    hi_s32 csc_coef01;
    hi_s32 csc_coef02;

    hi_s32 csc_coef10;
    hi_s32 csc_coef11;
    hi_s32 csc_coef12;

    hi_s32 csc_coef20;
    hi_s32 csc_coef21;
    hi_s32 csc_coef22;
} vdp_csc_coef;

typedef struct {
    hi_s32 csc_in_dc0;
    hi_s32 csc_in_dc1;
    hi_s32 csc_in_dc2;

    hi_s32 csc_out_dc0;
    hi_s32 csc_out_dc1;
    hi_s32 csc_out_dc2;
} vdp_csc_dc_coef;

typedef struct {
    /* for old version csc */
    hi_s32 csc_coef00;
    hi_s32 csc_coef01;
    hi_s32 csc_coef02;

    hi_s32 csc_coef10;
    hi_s32 csc_coef11;
    hi_s32 csc_coef12;

    hi_s32 csc_coef20;
    hi_s32 csc_coef21;
    hi_s32 csc_coef22;

    hi_s32 csc_in_dc0;
    hi_s32 csc_in_dc1;
    hi_s32 csc_in_dc2;

    hi_s32 csc_out_dc0;
    hi_s32 csc_out_dc1;
    hi_s32 csc_out_dc2;

    hi_s32 new_csc_scale2p;
    hi_s32 new_csc_clip_min;
    hi_s32 new_csc_clip_max;
} csc_coef;

typedef struct {
    hi_s32 csc_scale2p;
    hi_s32 csc_clip_min;
    hi_s32 csc_clip_max;
} csc_coef_param;

/* Vga Sharpen HF Coefficient */
typedef struct {
    hi_s32 vga_hsp_tmp0;
    hi_s32 vga_hsp_tmp1;
    hi_s32 vga_hsp_tmp2;
    hi_s32 vga_hsp_tmp3;
    hi_u32 vga_hsp_coring;
    hi_s32 vga_hsp_gainneg;
    hi_s32 vga_hsp_gainpos;
    hi_s32 vga_hsp_adpshooten;
    hi_u32 vga_hsp_winsize;
    hi_u32 vga_hsp_mixratio;
    hi_u32 vga_hsp_underth;
    hi_u32 vga_hsp_overth;
} hsp_hf_coef;

typedef struct {
    hi_u32 vga_hsp_hf_shootdiv;
    hi_u32 vga_hsp_lti_ratio;
    hi_u32 vga_hsp_ldti_gain;
    hi_u32 vga_hsp_cdti_gain;
    hi_u32 vga_hsp_peak_ratio;
    hi_u32 vga_hsp_glb_overth;
    hi_u32 vga_hsp_glb_unferth;
} hsp_coef;

#define VO_CSC_DC_MAGIC 0x07c30180

/* CVFIR VCOEF */
typedef struct {
    hi_s32 vccoef00;
    hi_s32 vccoef01;
    hi_s32 vccoef02;
    hi_s32 vccoef03;
    hi_s32 vccoef10;
    hi_s32 vccoef11;
    hi_s32 vccoef12;
    hi_s32 vccoef13;
} cvfir_coef;

/* HFIR VCOEF */
typedef struct {
    hi_s32 coef0;
    hi_s32 coef1;
    hi_s32 coef2;
    hi_s32 coef3;
    hi_s32 coef4;
    hi_s32 coef5;
    hi_s32 coef6;
    hi_s32 coef7;
} hfir_coef;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __HIFB_COEF_H__ */
