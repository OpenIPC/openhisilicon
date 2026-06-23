/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_LUT_DEFINE_H
#define ISP_LUT_DEFINE_H

/* Define the union u_isp_lblc_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_lut_wstt     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_lut_wstt;

/* Define the union u_isp_ae_weight */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_weight0        : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_ae_weight1        : 4; /* [11..8]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    isp_ae_weight2        : 4; /* [19..16]  */
        unsigned int    reserved_2            : 4; /* [23..20]  */
        unsigned int    isp_ae_weight3        : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_weight;

/* Define the union u_isp_nddm_gf_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gflut        : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_lut;

/* Define the union u_isp_expander_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_lut      : 21; /* [20..0]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_lut;

/* Define the union u_isp_wdr_snoise_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_snoise_lut    : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_snoise_lut;

/* Define the union u_isp_wdr_lnoise_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_lnoise_lut    : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lnoise_lut;

/* Define the union u_isp_wdr_normalize_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_normalize_lut : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_normalize_lut;

/* Define the union u_isp_adrc_tmlut0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_tmlut0_diff  : 14; /* [13..0]  */
        unsigned int    isp_adrc_tmlut0_value : 16; /* [29..14]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_tmlut0;

/* Define the union u_isp_adrc_cclut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_cclut        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_cclut;

/* Define the union u_isp_adrc_localmix */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_localmix_dark0 : 8; /* [7..0]  */
        unsigned int    isp_adrc_localmix_bright0 : 8; /* [15..8]  */
        unsigned int    isp_adrc_localmix_dark1 : 8; /* [23..16]  */
        unsigned int    isp_adrc_localmix_bright1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_localmix;

/* Define the union u_isp_adrc_detaildec */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_detaildec    : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_detaildec;

/* Define the union u_isp_adrc_logcvt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_logcvt       : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_logcvt;

/* Define the union u_isp_adrc_logcvt_luma */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_logcvt_luma  : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_logcvt_luma;

/* 0x0 ~ 0xfc para0 AE */
/* 0x100 ~ 0x140 para1 dmnr */
/* 0x150 ~ 0x1580 para2 expander/wdr */
/* 0x1590 ~ 0x1fcc para3 drc */
typedef struct {
    volatile u_isp_ae_weight                 isp_ae_weight[64]; /* 0x0~0xfc, ae weight lut size 64 */
    volatile u_isp_nddm_gf_lut               isp_nddm_gf_lut[17]; /* 0x100~0x140, nddm gf lut size 17 */
    volatile unsigned int                    reserved_0[3]; /* 0x144~0x14c, reserved 3 * 4 bytes */
    volatile u_isp_expander_lut              isp_expander_lut[257]; /* 0x150~0x550, expander lut size  257 */
    volatile unsigned int                    reserved_1[3]; /* 0x554~0x55c, reserved 3 * 4 bytes */
    volatile u_isp_wdr_snoise_lut            isp_wdr_snoise_lut[257]; /* 0x560~0x960, wdr snoise lut size  257 */
    volatile unsigned int                    reserved_2[3]; /* 0x964~0x96c, reserved 3 * 4 bytes */
    volatile u_isp_wdr_lnoise_lut            isp_wdr_lnoise_lut[257]; /* 0x970~0xd70, wdr lnoise lut size  257 */
    volatile unsigned int                    reserved_3[3]; /* 0xd74~0xd7c, reserved 3 * 4 bytes */
    volatile u_isp_wdr_normalize_lut         isp_wdr_normalize_lut[513]; /* 0xd80~0x1580, wdr normalize lut size  513 */
    volatile unsigned int                    reserved_4[3]; /* 0x1584~0x158c, reserved 3 * 4 bytes */
    volatile u_isp_adrc_tmlut0               isp_adrc_tmlut0[200]; /* 0x1590~0x18ac, drc tm lut0 size 200 */
    volatile u_isp_adrc_cclut                isp_adrc_cclut[33]; /* 0x18b0~0x1930, drc cclut size 33 */
    volatile unsigned int                    reserved_5[3]; /* 0x1934~0x193c, reserved 3 * 4 bytes */
    volatile u_isp_adrc_localmix             isp_adrc_localmix[33]; /* 0x1940~0x19c0, drc localmix size 33 */
    volatile unsigned int                    reserved_6[3]; /* 0x19c4~0x19cc, reserved 3 * 4 bytes */
    volatile u_isp_adrc_detaildec            isp_adrc_detaildec[128]; /* 0x19d0~0x1bcc, drc detaildec size 128 */
    volatile u_isp_adrc_logcvt               isp_adrc_logcvt[128]; /* 0x1bd0~0x1dcc, drc logcvt size 128 */
    volatile u_isp_adrc_logcvt_luma          isp_adrc_logcvt_luma[128]; /* 0x1dd0~0x1fcc, drc logcvt_luma size 128 */
    volatile unsigned int                    reserved_7[12]; /* 0x1fd0~0x1ffc, reserved 12 * 4 bytes */
} isp_be_lut_apb_type;

/* Define the union u_isp_clut_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut_wstt     : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut_wstt;
/* Define the union u_isp_gamma_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_lut_wstt    : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_lut_wstt;

/* Define the union u_isp_dehaze_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_wstt   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_lut_wstt;

/* Define the union u_isp_sharpen_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lut_wstt  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lut_wstt;

/* Define the union u_isp_ldci_lut_wstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lut_wstt      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lut_wstt;
/* Define the union u_isp_ca_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_lut_wstt        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_lut_wstt;

/* Define the union u_isp_cc_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_lut_wstt       : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_lut_wstt;

/* Define the union u_isp_dpc_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_lut_wstt_x    : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_dpc_lut_wstt_y    : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_lut_wstt;

/* Define the union u_isp_mlsc_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_lut_wstt     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_lut_wstt;

/* Define the union u_isp_bnr_lut_wstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lut_wstt       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_lut_wstt;
/* Define the union u_isp_dmnr_aidm_detail_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_aidm_detail_lut : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_aidm_detail_lut;

typedef struct {
    volatile u_isp_dpc_lut_wstt       isp_dpc_lut_wstt[2048];  /* 0 ~ 0x1FFC, dpc lut size 2048 */
    volatile u_isp_mlsc_lut_wstt      isp_mlsc_lut_wstt[4356]; /* 0x2000 ~ 0x640c, lsc lut2stt size 4356 */
    volatile unsigned int             reserved_5[60];          /* 0x6410 ~ 0x64fc, reserved 60 * 4 bytes */
    volatile u_isp_bnr_lut_wstt       isp_bnr_lut_wstt[1036];  /* 0x6500 ~ 0x752c, bnr lut2stt size 1036 */
    volatile unsigned int             reserved_6[52];          /* 0x7530 ~ 0x75fc, reserved 52 * 4 bytes */
} isp_pre_be_lut_wstt_type;

typedef struct {
    volatile u_isp_clut_lut_wstt      isp_clut_lut_wstt[5508];     /* 0x0 ~ 0x560c, clut lut2stt size  5508 */
    volatile unsigned int             reserved_0[60];              /* 0x5610 ~ 0x56fc, reserved 60 * 4 bytes */
    volatile u_isp_gamma_lut_wstt     isp_gamma_lut_wstt[257];     /* 0x5700 ~ 0x5b00, gamma lut2stt size  257 */
    volatile unsigned int             reserved_2[63];              /* 0x5b04 ~ 0x5bfc, reserved 63 * 4 bytes */
    volatile u_isp_dehaze_lut_wstt    isp_dehaze_lut_wstt[640];    /* 0x5c00 ~ 0x65fc, dehaze lut2stt size 640 */
    volatile u_isp_sharpen_lut_wstt   isp_sharpen_lut_wstt[128];   /* 0x6600 ~ 0x67fc, sharpen lut2stt size  128 */
    volatile u_isp_ldci_lut_wstt      isp_ldci_lut_wstt[260];      /* 0x6800 ~ 0x6c0c, ldci lut2stt size  128 */
    volatile unsigned int             reserved_3[60];              /* 0x6c10 ~ 0x6cfc, reserved 60 * 4 bytes */
    volatile u_isp_ca_lut_wstt        isp_ca_lut_wstt[544];        /* 0x6d00 ~ 0x70fc, sharpen lut2stt size 544 */
    volatile unsigned int             reserved_4[32];              /* 0x7580 ~ 0x75fc, reserved 32 * 4 bytes */
    volatile u_isp_cc_lut_wstt        isp_cc_lut_wstt[136];        /* 0x7600 ~ 0x731c, cc lut2stt size 136 */
    volatile unsigned int             reserved_5[56];              /* 0x7820 ~ 0x78fc, reserved 56 * 4 bytes */
    volatile u_isp_dmnr_aidm_detail_lut isp_dmnr_lut_wstt[128]; /* 0x7900~0x7afc, aidm lut2stt size 128 */
} isp_post_be_lut_wstt_type;

typedef struct {
    volatile u_isp_lblc_lut_wstt isp_lblc_lut_wstt[4356]; /* 0x0 ~ 0x440f, lblc lut2stt size 4356 */
    volatile unsigned int        reserved_5[60];          /* 0x4410 ~ 0x44fc, reserved 60 * 4 bytes */
} isp_fe_lut_wstt_type;

typedef struct {
    isp_pre_be_lut_wstt_type  pre_be_lut2stt;     /* 0x0 ~ 0x75fc, size 0x7600 */
    isp_post_be_lut_wstt_type post_be_lut2stt;    /* 0x7600 ~ 0xf0fc, size 0x7B00 */
} isp_be_lut_wstt_type;

typedef struct {
    isp_be_lut_apb_type   be_apb_lut;   /* 0x0 ~ 0x1ffc, size  0x2000 */
    isp_be_lut_wstt_type  be_lut2stt;   /* 0x2000 ~ 0x110fc, size 0xf100 */
} isp_be_lut_type;

#endif /* ISP_LUT_DEFINE_H */
