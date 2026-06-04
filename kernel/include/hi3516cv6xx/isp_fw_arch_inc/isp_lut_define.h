/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_LUT_DEFINE_H
#define ISP_LUT_DEFINE_H


/* Define the union u_isp_ae_weight */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_weight0 : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;     /* [7..4]  */
        unsigned int isp_ae_weight1 : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;     /* [15..12]  */
        unsigned int isp_ae_weight2 : 4; /* [19..16]  */
        unsigned int reserved_2 : 4;     /* [23..20]  */
        unsigned int isp_ae_weight3 : 4; /* [27..24]  */
        unsigned int reserved_3 : 4;     /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_weight;

/* Define the union u_isp_dis_refinfo_wlut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_refinfo_wlut0 : 9; /* [8..0]  */
        unsigned int isp_dis_refinfo_wlut1 : 9; /* [17..9]  */
        unsigned int reserved_0 : 14;           /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_refinfo_wlut;

/* Define the union u_isp_expander_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_lut      : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_lut;

/* Define the union u_isp_drc_tmlut0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    isp_drc_tmlut0_diff   : 12  ; /* [13..2]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_drc_tmlut0_value  : 14  ; /* [29..16]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_tmlut0;


/* Define the union u_isp_drc_cclut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 2;    /* [1..0]  */
        unsigned int isp_drc_cclut : 9; /* [10..2]  */
        unsigned int reserved_1 : 21;   /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_cclut;


/* Define the union u_isp_drc_coefalut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_coefalut      : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_drc_coefalut;

/* Define the union u_isp_drc_delut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_delut         : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_drc_delut;

/* Define the union u_isp_bcom_gamma_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_gamma_lut    : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bcom_gamma_lut;

/* Define the union u_isp_bdec_gamma_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_gamma_lut    : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_gamma_lut;


typedef struct {
    volatile u_isp_ae_weight                 isp_ae_weight[256]; /* 0x0~0x3fc, ae weight lut size 64 */
    volatile u_isp_dis_refinfo_wlut          isp_dis_refinfo_wlut[576];  /* 0x400~0xcfc, dis ref info lut size:576 */
    volatile u_isp_expander_lut              isp_expander_lut[257];   /* 0xd00~0x1100, isp_expander_lut size:257 */
    volatile unsigned int                    reserved_0[3]; /* 0x1104~0x110c, reserved 3 * 4 bytes */
    volatile u_isp_drc_tmlut0               isp_drc_tmlut0[200]; /* 0x1110~0x142c */
    volatile u_isp_drc_cclut                isp_drc_cclut[33];   /* 0x1430~0x14b0 */
    volatile unsigned int                    reserved_1[3]; /* 0x14b4~0x14bc */
    volatile u_isp_drc_coefalut              isp_drc_coefalut[19]; /* 0x14c0~0x1508 */
    volatile unsigned int                    reserved_2; /* 0x150c */
    volatile u_isp_drc_delut                 isp_drc_delut[16]; /* 0x1510~0x154c */
    volatile u_isp_bcom_gamma_lut            isp_bcom_gamma_lut[257]; /* 0x1550~0x1950 */
    volatile unsigned int                    reserved_3[3]                ; /* 0x1954~0x195c */
    volatile u_isp_bdec_gamma_lut            isp_bdec_gamma_lut[257]  ; /* 0x1960~0x1d60 */
    volatile unsigned int                    reserved_4[39]                ; /* 0x1d64~0x1dfc */
} isp_be_lut_apb_type;


/* Define the union u_isp_cc_dgain_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_lut : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;       /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_lut;

/* Define the union u_isp_cc_gamma_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_lut : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_lut;

/* Define the union u_isp_sharpen_mhfgaind */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mfgaind : 12; /* [11..0]  */
        unsigned int isp_sharpen_hfgaind : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgaind;

/* Define the union u_isp_sharpen_mhfgainud */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mfgainud : 12; /* [11..0]  */
        unsigned int isp_sharpen_hfgainud : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;            /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgainud;

/* Define the union u_isp_dehaze_prestat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_prestat : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;         /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_prestat;

/* Define the union u_isp_dehaze_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;    /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_lut;

/* Define the union u_isp_gamma_lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_lut : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;    /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_lut;

/* Define the union u_isp_bnr_noisesd_even */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesd_even  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_even;

/* Define the union u_isp_bnr_noisesd_odd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesd_odd   : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_odd;

/* Define the union u_isp_bnr_noisesds_even */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesds_even : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_even;

/* Define the union u_isp_bnr_noisesds_odd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesds_odd  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_odd;

/* Define the union u_isp_bnr_noiseinv_even */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noiseinv_even : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_even;

/* Define the union u_isp_bnr_noiseinv_odd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noiseinv_odd  : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_odd;

/* Define the union u_isp_bnr_noiseinv_mag_even */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noiseinv_mag_even : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_even;

/* Define the union u_isp_bnr_noiseinv_mag_odd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noiseinv_mag_odd : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_odd;

/* Define the union u_isp_bnr_fbratiotable */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_fbratiotable  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_fbratiotable;

/* Define the union u_isp_bnr_coring_low_even */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_coring_low_even : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_even;

/* Define the union u_isp_bnr_coring_low_odd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_coring_low_odd : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_odd;

/* Define the union u_isp_bnr_mixgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_fbra_mot2yuv       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_fbra_mot2yuv;


/* Define the union u_isp_mlsc_rgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_rgain : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;     /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_rgain;

/* Define the union u_isp_mlsc_grgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_grgain : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_grgain;

/* Define the union u_isp_mlsc_bgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_bgain : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;     /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_bgain;

/* Define the union u_isp_mlsc_gbgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_gbgain : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_gbgain;

/* Define the union u_isp_ca_yratiolut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yratiolut : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;        /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yratiolut;

/* Define the union u_isp_ca_ysatlut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ysatlut : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ysatlut;

/* Define the union u_isp_ca_ych1lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych1lut_even : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int isp_ca_ych1lut_odd : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych1lut;

/* Define the union u_isp_ca_ych2lut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych2lut_even : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int isp_ca_ych2lut_odd : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych2lut;

/* Define the union u_isp_ca_yslulut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslulut_even : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int isp_ca_yslulut_odd : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslulut;

/* Define the union u_isp_ca_yslvlut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslvlut_even : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int isp_ca_yslvlut_odd : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslvlut;

/* Define the union u_isp_ldci_he_wlut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_hepos_wlut : 9; /* [8..0]  */
        unsigned int isp_ldci_heneg_wlut : 9; /* [17..9]  */
        unsigned int reserved_0 : 14;         /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_he_wlut;

/* Define the union u_isp_ldci_cgain_wlut */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_cgain_wlut : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;          /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_cgain_wlut;

/* Define the union u_isp_dehaze_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut_wstt : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;          /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_lut_wstt;

/* Define the union u_isp_gamma_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_lut_wstt : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;         /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_lut_wstt;

/* Define the union u_isp_sharpen_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lut_wstt : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;            /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lut_wstt;

/* Define the union u_isp_mlsc_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_lut_wstt : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;        /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_lut_wstt;

/* Define the union u_isp_bnr_lut_wstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lut_wstt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_lut_wstt;
/* Define the union u_isp_ca_lut_wstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ca_lut_wstt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_lut_wstt;
/* Define the union u_isp_ldci_lut_wstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lut_wstt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lut_wstt;
/* Define the union u_isp_cc_lut_wstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_lut_wstt : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;      /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_lut_wstt;

typedef struct {
    u_isp_dehaze_lut_wstt isp_dehaze_lut_wstt[640];    /* 0x0~0x9fc */
    u_isp_gamma_lut_wstt isp_gamma_lut_wstt[257];      /* 0xa00~0xe00 */
    unsigned int reserved_15[3];                       /* 0xe04~0xe0c */
    u_isp_sharpen_lut_wstt isp_sharpen_lut_wstt[128];  /* 0xe10~0x100c */
    u_isp_mlsc_lut_wstt isp_mlsc_lut_wstt[2508];       /* 0x1010~0x373c */
    u_isp_bnr_lut_wstt isp_bnr_lut_wstt[776];          /* 0x3740~0x435c */
    u_isp_ca_lut_wstt isp_ca_lut_wstt[288];           /* 0x4360~0x47dc */
    u_isp_ldci_lut_wstt isp_ldci_lut_wstt[260];       /* 0x47e0~0x4bec */
    u_isp_cc_lut_wstt isp_cc_lut_wstt[136];            /* 0x4bf0~0x4e0c */
    unsigned int reserved_16[60];                      /* 0x4e10~0x4efc */
} isp_be_lut_wstt_type;


typedef struct {
    isp_be_lut_apb_type   be_apb_lut;   /* 0x0 ~ 0x1dfc, size  0x1f00 */
    isp_be_lut_wstt_type  be_lut2stt;   /* 0x1f00 ~ 0x  size 0x */
} isp_be_lut_type;

#endif /* ISP_LUT_DEFINE_H */
