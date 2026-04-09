// ******************************************************************************
// Copyright     :  Copyright (C) 2017, Hisilicon Technologies Co., Ltd.
// File name     :   isp_lut_define.h
// Author        :
// Version       :  1.0
// Date          :  2017-02-23
// Description   :  Define all registers/tables
// History       :   2017-02-23 Create file
// ******************************************************************************

#ifndef __ISP_LUT_DEFINE_H__
#define __ISP_LUT_DEFINE_H__

/******************************************************************************
*******************************************************************************
*******************************************************************************
*************************** online lut stt define***********************************
*******************************************************************************
*******************************************************************************
******************************************************************************/
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
    volatile U_ISP_CLUT_LUT_WSTT     ISP_CLUT_LUT_WSTT[5508]          ; /* 0x0~0x560c */
    volatile unsigned int            reserved_0[60]                   ; /* 0x5610~0x56fc */
    volatile U_ISP_DEHAZE_LUT_WSTT   ISP_DEHAZE_LUT_WSTT[1280]        ; /* 0x5700~0x6afc */
    volatile U_ISP_BNR_LUT_WSTT      ISP_BNR_LUT_WSTT[260]            ; /* 0x6b00~0x6f0c */
    volatile unsigned int            reserved_1[60]                   ; /* 0x6f10~0x6ffc */
    volatile U_ISP_GAMMA_LUT_WSTT    ISP_GAMMA_LUT_WSTT[257]          ; /* 0x7000~0x7400 */
    volatile unsigned int            reserved_2[63]                   ; /* 0x7404~0x74fc */
    volatile U_ISP_SHARPEN_LUT_WSTT  ISP_SHARPEN_LUT_WSTT[256]        ; /* 0x7500~0x78fc */
    volatile U_ISP_DPC_LUT_WSTT      ISP_DPC_LUT_WSTT[2048]           ; /* 0x7900~0x98fc */
    volatile U_ISP_LSC_LUT_WSTT      ISP_LSC_LUT_WSTT[4356]           ; /* 0x9900~0xdd0c */
    volatile unsigned int            reserved_3[60]                   ; /* 0xdd10~0xddfc */
    volatile U_ISP_CA_LUT_WSTT       ISP_CA_LUT_WSTT[128]             ; /* 0xde00~0xdffc */
    volatile U_ISP_LDCI_LUT_WSTT     ISP_LDCI_LUT_WSTT[260]           ; /* 0xe000~0xe40c */
    volatile unsigned int            reserved_4[60]                   ; /* 0xe410~0xe4fc */
    volatile U_ISP_PREGAMMA_LUT_WSTT ISP_PREGAMMA_LUT_WSTT[257]       ; /* 0xe500~0xe900 */
    volatile unsigned int            reserved_5[63]                   ; /* 0xe904~0xe9fc */
} S_ISP_LUT_WSTT_TYPE;


/******************************************************************************
*******************************************************************************
*******************************************************************************
************************** offline lut reg define***********************************
*******************************************************************************
*******************************************************************************
******************************************************************************/

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
    volatile U_ISP_AE_WEIGHT        ISP_AE_WEIGHT[64]                 ; /* 0x0~0xfc */
    volatile U_ISP_DEMOSAIC_DEPURPLUT   ISP_DEMOSAIC_DEPURPLUT[16]    ; /* 0x100~0x13c */
    volatile U_ISP_NDDM_GF_LUT      ISP_NDDM_GF_LUT[17]               ; /* 0x140~0x180 */
    volatile unsigned int           reserved_0[3]                     ; /* 0x184~0x18c */
    volatile U_ISP_BNR_LMT_EVEN     ISP_BNR_LMT_EVEN[65]              ; /* 0x190~0x290 */
    volatile unsigned int           reserved_1[3]                     ; /* 0x294~0x29c */
    volatile U_ISP_BNR_LMT_ODD      ISP_BNR_LMT_ODD[64]               ; /* 0x2a0~0x39c */
    volatile U_ISP_BNR_COR_EVEN     ISP_BNR_COR_EVEN[17]              ; /* 0x3a0~0x3e0 */
    volatile unsigned int           reserved_2[3]                     ; /* 0x3e4~0x3ec */
    volatile U_ISP_BNR_COR_ODD      ISP_BNR_COR_ODD[16]               ; /* 0x3f0~0x42c */
    volatile U_ISP_BNR_LMT_S_EVEN   ISP_BNR_LMT_S_EVEN[65]            ; /* 0x430~0x530 */
    volatile unsigned int           reserved_3[3]                     ; /* 0x534~0x53c */
    volatile U_ISP_BNR_LMT_S_ODD    ISP_BNR_LMT_S_ODD[64]             ; /* 0x540~0x63c */
    volatile U_ISP_BNR_GCOR_EVEN    ISP_BNR_GCOR_EVEN[17]             ; /* 0x640~0x680 */
    volatile unsigned int           reserved_4[3]                     ; /* 0x684~0x68c */
    volatile U_ISP_BNR_GCOR_ODD     ISP_BNR_GCOR_ODD[16]              ; /* 0x690~0x6cc */
    volatile U_ISP_DE_LUMA_GAIN     ISP_DE_LUMA_GAIN[17]              ; /* 0x6d0~0x710 */
    volatile unsigned int           reserved_5[3]                     ; /* 0x714~0x71c */
    volatile U_ISP_EXPANDER_LUT     ISP_EXPANDER_LUT[257]             ; /* 0x720~0xb20 */
    volatile unsigned int           reserved_6[3]                     ; /* 0xb24~0xb2c */
    volatile U_ISP_LDCI_POLYP_WLUT   ISP_LDCI_POLYP_WLUT[65]          ; /* 0xb30~0xc30 */
    volatile unsigned int           reserved_7[3]                     ; /* 0xc34~0xc3c */
    volatile U_ISP_LDCI_POLYQ01_WLUT   ISP_LDCI_POLYQ01_WLUT[65]      ; /* 0xc40~0xd40 */
    volatile unsigned int           reserved_8[3]                     ; /* 0xd44~0xd4c */
    volatile U_ISP_LDCI_POLYQ23_WLUT   ISP_LDCI_POLYQ23_WLUT[65]      ; /* 0xd50~0xe50 */
    volatile unsigned int           reserved_9[3]                     ; /* 0xe54~0xe5c */
    volatile U_ISP_DRC_TMLUT0       ISP_DRC_TMLUT0[200]               ; /* 0xe60~0x117c */
    volatile U_ISP_DRC_CCLUT        ISP_DRC_CCLUT[33]                 ; /* 0x1180~0x1200 */
    volatile unsigned int           reserved_12[63]                   ; /* 0x1204~0x12fc */
    S_ISP_LUT_WSTT_TYPE             stBeLut2Stt;                      ; /* 0x1300~0xfcfc */
} S_ISP_LUT_REGS_TYPE;

#endif /* __ISP_LUT_DEFINE_H__ */
