/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_lut_define.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_LUT_DEFINE_H__
#define __ISP_LUT_DEFINE_H__

/* online lut stt define */
/* Define the union U_ISP_CLUT_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut_wstt     : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_CLUT_LUT_WSTT;

/* Define the union U_ISP_DEHAZE_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_wstt   : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_DEHAZE_LUT_WSTT;

/* Define the union U_ISP_BNR_LUT_WSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lut_wstt       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_LUT_WSTT;
/* Define the union U_ISP_GAMMA_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_lut_wstt    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_GAMMA_LUT_WSTT;

/* Define the union U_ISP_SHARPEN_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lut_wstt  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_SHARPEN_LUT_WSTT;

/* Define the union U_ISP_DPC_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_lut_wstt_x    : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_dpc_lut_wstt_y    : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_DPC_LUT_WSTT;

/* Define the union U_ISP_LSC_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_lut_wstt      : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_LSC_LUT_WSTT;

/* Define the union U_ISP_CA_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_wstt       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_CA_LUT_WSTT;

/* Define the union U_ISP_LDCI_LUT_WSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lut_wstt      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_LDCI_LUT_WSTT;
/* Define the union U_ISP_PREGAMMA_LUT_WSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_lut_wstt : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_PREGAMMA_LUT_WSTT;

/* Define the global struct */
typedef struct {
    volatile U_ISP_CLUT_LUT_WSTT     ISP_CLUT_LUT_WSTT[5508]    ; /* 0x0~0x560c, clut lut2stt size: 5508 */
    volatile unsigned int            reserved_0[60]             ; /* 0x5610~0x56fc, reserved 60 * 4 bytes */
    volatile U_ISP_DEHAZE_LUT_WSTT   ISP_DEHAZE_LUT_WSTT[1280]  ; /* 0x5700~0x6afc, dehaze lut2stt size: 1280 */
    volatile U_ISP_BNR_LUT_WSTT      ISP_BNR_LUT_WSTT[260]      ; /* 0x6b00~0x6f0c, bnr lut2stt size: 260 */
    volatile unsigned int            reserved_1[60]             ; /* 0x6f10~0x6ffc, reserved 60 * 4 bytes */
    volatile U_ISP_GAMMA_LUT_WSTT    ISP_GAMMA_LUT_WSTT[257]    ; /* 0x7000~0x7400, gamma lut2stt size: 257 */
    volatile unsigned int            reserved_2[63]             ; /* 0x7404~0x74fc, reserved 63 * 4 bytes */
    volatile U_ISP_SHARPEN_LUT_WSTT  ISP_SHARPEN_LUT_WSTT[256]  ; /* 0x7500~0x78fc, sharpen lut2stt size: 256 */
    volatile U_ISP_DPC_LUT_WSTT      ISP_DPC_LUT_WSTT[2048]     ; /* 0x7900~0x98fc, dpc lut2stt size: 2048 */
    volatile U_ISP_LSC_LUT_WSTT      ISP_LSC_LUT_WSTT[4356]     ; /* 0x9900~0xdd0c, lsc lut2stt size: 4356 */
    volatile unsigned int            reserved_3[60]             ; /* 0xdd10~0xddfc, reserved 60 * 4 bytes */
    volatile U_ISP_CA_LUT_WSTT       ISP_CA_LUT_WSTT[128]       ; /* 0xde00~0xdffc, ca lut2stt size: 128 */
    volatile U_ISP_LDCI_LUT_WSTT     ISP_LDCI_LUT_WSTT[260]     ; /* 0xe000~0xe40c, ldci lut2stt size: 260 */
    volatile unsigned int            reserved_4[60]             ; /* 0xe410~0xe4fc, reserved 60 * 4 bytes */
    volatile U_ISP_PREGAMMA_LUT_WSTT ISP_PREGAMMA_LUT_WSTT[257] ; /* 0xe500~0xe900, pregamma lut2stt size: 257 */
    volatile unsigned int            reserved_5[63]             ; /* 0xe904~0xe9fc, reserved 63 * 4 bytes */
} S_ISP_LUT_WSTT_TYPE;

/* offline lut reg define */
/* Define the union U_ISP_AE_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_weight0        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_ae_weight1        : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_ae_weight2        : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    isp_ae_weight3        : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_AE_WEIGHT;

/* Define the union U_ISP_DEMOSAIC_DEPURPLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_depurp_lut : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_DEMOSAIC_DEPURPLUT;

/* Define the union U_ISP_NDDM_GF_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gflut        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_NDDM_GF_LUT;

/* Define the union U_ISP_BNR_LMT_EVEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_even      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_LMT_EVEN;

/* Define the union U_ISP_BNR_LMT_ODD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_odd       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_LMT_ODD;

/* Define the union U_ISP_BNR_COR_EVEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_cor_even      : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_COR_EVEN;

/* Define the union U_ISP_BNR_COR_ODD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_cor_odd       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_COR_ODD;

/* Define the union U_ISP_BNR_LMT_S_EVEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_s_even    : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_LMT_S_EVEN;

/* Define the union U_ISP_BNR_LMT_S_ODD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_s_odd     : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_LMT_S_ODD;

/* Define the union U_ISP_BNR_GCOR_EVEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_gcor_even     : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_GCOR_EVEN;

/* Define the union U_ISP_BNR_GCOR_ODD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_gcor_odd      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_BNR_GCOR_ODD;

/* Define the union U_ISP_DE_LUMA_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_de_luma_gain      : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_DE_LUMA_GAIN;

/* Define the union U_ISP_EXPANDER_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_lut      : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_EXPANDER_LUT;

/* Define the union U_ISP_LDCI_POLYP_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_poply1_wlut  : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_poply2_wlut  : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_poply3_wlut  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_LDCI_POLYP_WLUT;

/* Define the union U_ISP_LDCI_POLYQ01_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq0_wlut   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq1_wlut   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_LDCI_POLYQ01_WLUT;

/* Define the union U_ISP_LDCI_POLYQ23_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq2_wlut   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq3_wlut   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_LDCI_POLYQ23_WLUT;

/* Define the union U_ISP_DRC_TMLUT0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_tmlut0_diff   : 14  ; /* [13..0]  */
        unsigned int    isp_drc_tmlut0_value  : 16  ; /* [29..14]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_DRC_TMLUT0;

/* Define the union U_ISP_DRC_CCLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cclut         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_ISP_DRC_CCLUT;

/* Define the global struct */
typedef struct {
    volatile U_ISP_AE_WEIGHT        ISP_AE_WEIGHT[64]                 ; /* 0x0~0xfc, ae weight lut size:64 */
    volatile U_ISP_DEMOSAIC_DEPURPLUT   ISP_DEMOSAIC_DEPURPLUT[16]    ; /* 0x100~0x13c, lcac lut size: 16 */
    volatile U_ISP_NDDM_GF_LUT      ISP_NDDM_GF_LUT[17]               ; /* 0x140~0x180, nddm gf lut size:17 */
    volatile unsigned int           reserved_0[3]                     ; /* 0x184~0x18c, reserved 3 * 4 bytes */
    volatile U_ISP_BNR_LMT_EVEN     ISP_BNR_LMT_EVEN[65]              ; /* 0x190~0x290, bnr lmt even lut size:65 */
    volatile unsigned int           reserved_1[3]                     ; /* 0x294~0x29c, reserved 3 * 4 bytes */
    volatile U_ISP_BNR_LMT_ODD      ISP_BNR_LMT_ODD[64]               ; /* 0x2a0~0x39c, bnr lmt odd lut size:64 */
    volatile U_ISP_BNR_COR_EVEN     ISP_BNR_COR_EVEN[17]              ; /* 0x3a0~0x3e0, bnr cor even lut size:17 */
    volatile unsigned int           reserved_2[3]                     ; /* 0x3e4~0x3ec, reserved 3 * 4 bytes */
    volatile U_ISP_BNR_COR_ODD      ISP_BNR_COR_ODD[16]               ; /* 0x3f0~0x42c, bnr cor odd lut size:16 */
    volatile U_ISP_BNR_LMT_S_EVEN   ISP_BNR_LMT_S_EVEN[65]            ; /* 0x430~0x530, bnr lmts even lut size:65 */
    volatile unsigned int           reserved_3[3]                     ; /* 0x534~0x53c, reserved 3 * 4 bytes */
    volatile U_ISP_BNR_LMT_S_ODD    ISP_BNR_LMT_S_ODD[64]             ; /* 0x540~0x63c, bnr lmts odd lut size:64 */
    volatile U_ISP_BNR_GCOR_EVEN    ISP_BNR_GCOR_EVEN[17]             ; /* 0x640~0x680, bnr gcor even lut size:17 */
    volatile unsigned int           reserved_4[3]                     ; /* 0x684~0x68c, reserved 3 * 4 bytes */
    volatile U_ISP_BNR_GCOR_ODD     ISP_BNR_GCOR_ODD[16]              ; /* 0x690~0x6cc, bnr gcor odd lut size:16 */
    volatile U_ISP_DE_LUMA_GAIN     ISP_DE_LUMA_GAIN[17]              ; /* 0x6d0~0x710, de luma lut size:17 */
    volatile unsigned int           reserved_5[3]                     ; /* 0x714~0x71c, reserved 3 * 4 bytes */
    volatile U_ISP_EXPANDER_LUT     ISP_EXPANDER_LUT[257]             ; /* 0x720~0xb20, expander lut size: 257 */
    volatile unsigned int           reserved_6[3]                     ; /* 0xb24~0xb2c, reserved 3 * 4 bytes */
    volatile U_ISP_LDCI_POLYP_WLUT   ISP_LDCI_POLYP_WLUT[65]          ; /* 0xb30~0xc30, ldci polyp lut size:65 */
    volatile unsigned int           reserved_7[3]                     ; /* 0xc34~0xc3c, reserved 3 * 4 bytes */
    volatile U_ISP_LDCI_POLYQ01_WLUT   ISP_LDCI_POLYQ01_WLUT[65]      ; /* 0xc40~0xd40, ldci polyq01 lut size:65 */
    volatile unsigned int           reserved_8[3]                     ; /* 0xd44~0xd4c, reserved 3 * 4 bytes */
    volatile U_ISP_LDCI_POLYQ23_WLUT   ISP_LDCI_POLYQ23_WLUT[65]      ; /* 0xd50~0xe50, ldci poly23 lut size:65 */
    volatile unsigned int           reserved_9[3]                     ; /* 0xe54~0xe5c, reserved 3 * 4 bytes */
    volatile U_ISP_DRC_TMLUT0       ISP_DRC_TMLUT0[200]               ; /* 0xe60~0x117c, drc tm lut0 size:200 */
    volatile U_ISP_DRC_CCLUT        ISP_DRC_CCLUT[33]                 ; /* 0x1180~0x1200, drc cclut size:33 */
    volatile unsigned int           reserved_12[63]                   ; /* 0x1204~0x12fc, reserved 63 * 4 bytes */
    S_ISP_LUT_WSTT_TYPE             stBeLut2Stt                       ; /* 0x1300~0xfcfc */
} S_ISP_LUT_REGS_TYPE;

#endif /* __ISP_LUT_DEFINE_H__ */
