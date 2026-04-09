/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#ifndef __HIFB_REG_H__
#define __HIFB_REG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
/* Define the union u_voctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 25;      /* [24..0] */
        unsigned int wbc_dhd_ck_gt_en : 1; /* [25] */
        unsigned int g1_ck_gt_en : 1;      /* [26] */
        unsigned int g0_ck_gt_en : 1;      /* [27] */
        unsigned int v1_ck_gt_en : 1;      /* [28] */
        unsigned int v0_ck_gt_en : 1;      /* [29] */
        unsigned int chk_sum_en : 1;       /* [30] */
        unsigned int vo_ck_gt_en : 1;      /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_voctrl;

/* Define the union u_vointsta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_int : 1;   /* [0] */
        unsigned int dhd0vtthd2_int : 1;   /* [1] */
        unsigned int dhd0vtthd3_int : 1;   /* [2] */
        unsigned int dhd0uf_int : 1;       /* [3] */
        unsigned int dhd1vtthd1_int : 1;   /* [4] */
        unsigned int dhd1vtthd2_int : 1;   /* [5] */
        unsigned int dhd1vtthd3_int : 1;   /* [6] */
        unsigned int dhd1uf_int : 1;       /* [7] */
        unsigned int dsdvtthd1_int : 1;    /* [8] */
        unsigned int dsdvtthd2_int : 1;    /* [9] */
        unsigned int dsdvtthd3_int : 1;    /* [10] */
        unsigned int dsduf_int : 1;        /* [11] */
        unsigned int b0_err_int : 1;       /* [12] */
        unsigned int b1_err_int : 1;       /* [13] */
        unsigned int b2_err_int : 1;       /* [14] */
        unsigned int wbc_dhd_over_int : 1; /* [15] */
        unsigned int vdac0_int : 1;        /* [16] */
        unsigned int vdac1_int : 1;        /* [17] */
        unsigned int vdac2_int : 1;        /* [18] */
        unsigned int vdac3_int : 1;        /* [19] */
        unsigned int reserved_0 : 12;      /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointsta;

/* Define the union u_vomskintsta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_clr : 1;   /* [0] */
        unsigned int dhd0vtthd2_clr : 1;   /* [1] */
        unsigned int dhd0vtthd3_clr : 1;   /* [2] */
        unsigned int dhd0uf_clr : 1;       /* [3] */
        unsigned int dhd1vtthd1_clr : 1;   /* [4] */
        unsigned int dhd1vtthd2_clr : 1;   /* [5] */
        unsigned int dhd1vtthd3_clr : 1;   /* [6] */
        unsigned int dhd1uf_clr : 1;       /* [7] */
        unsigned int dsdvtthd1_clr : 1;    /* [8] */
        unsigned int dsdvtthd2_clr : 1;    /* [9] */
        unsigned int dsdvtthd3_clr : 1;    /* [10] */
        unsigned int dsduf_clr : 1;        /* [11] */
        unsigned int b0_err_clr : 1;       /* [12] */
        unsigned int b1_err_clr : 1;       /* [13] */
        unsigned int b2_err_clr : 1;       /* [14] */
        unsigned int wbc_dhd_over_clr : 1; /* [15] */
        unsigned int vdac0_clr : 1;        /* [16] */
        unsigned int vdac1_clr : 1;        /* [17] */
        unsigned int vdac2_clr : 1;        /* [18] */
        unsigned int vdac3_clr : 1;        /* [19] */
        unsigned int reserved_0 : 12;      /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomskintsta;

/* Define the union u_vointmsk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_intmask : 1;   /* [0] */
        unsigned int dhd0vtthd2_intmask : 1;   /* [1] */
        unsigned int dhd0vtthd3_intmask : 1;   /* [2] */
        unsigned int dhd0uf_intmask : 1;       /* [3] */
        unsigned int dhd1vtthd1_intmask : 1;   /* [4] */
        unsigned int dhd1vtthd2_intmask : 1;   /* [5] */
        unsigned int dhd1vtthd3_intmask : 1;   /* [6] */
        unsigned int dhd1uf_intmask : 1;       /* [7] */
        unsigned int dsdvtthd1_intmask : 1;    /* [8] */
        unsigned int dsdvtthd2_intmask : 1;    /* [9] */
        unsigned int dsdvtthd3_intmask : 1;    /* [10] */
        unsigned int dsduf_intmask : 1;        /* [11] */
        unsigned int b0_err_intmask : 1;       /* [12] */
        unsigned int b1_err_intmask : 1;       /* [13] */
        unsigned int b2_err_intmask : 1;       /* [14] */
        unsigned int wbc_dhd_over_intmask : 1; /* [15] */
        unsigned int vdac0_intmask : 1;        /* [16] */
        unsigned int vdac1_intmask : 1;        /* [17] */
        unsigned int vdac2_intmask : 1;        /* [18] */
        unsigned int vdac3_intmask : 1;        /* [19] */
        unsigned int reserved_0 : 12;          /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointmsk;

/* Define the union u_vointsta1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_int : 1;   /* [0] */
        unsigned int dhd0vtthd2_int : 1;   /* [1] */
        unsigned int dhd0vtthd3_int : 1;   /* [2] */
        unsigned int dhd0uf_int : 1;       /* [3] */
        unsigned int dhd1vtthd1_int : 1;   /* [4] */
        unsigned int dhd1vtthd2_int : 1;   /* [5] */
        unsigned int dhd1vtthd3_int : 1;   /* [6] */
        unsigned int dhd1uf_int : 1;       /* [7] */
        unsigned int dsdvtthd1_int : 1;    /* [8] */
        unsigned int dsdvtthd2_int : 1;    /* [9] */
        unsigned int dsdvtthd3_int : 1;    /* [10] */
        unsigned int dsduf_int : 1;        /* [11] */
        unsigned int b0_err_int : 1;       /* [12] */
        unsigned int b1_err_int : 1;       /* [13] */
        unsigned int b2_err_int : 1;       /* [14] */
        unsigned int wbc_dhd_over_int : 1; /* [15] */
        unsigned int vdac0_int : 1;        /* [16] */
        unsigned int vdac1_int : 1;        /* [17] */
        unsigned int vdac2_int : 1;        /* [18] */
        unsigned int vdac3_int : 1;        /* [19] */
        unsigned int reserved_0 : 12;      /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointsta1;

/* Define the union u_vomskintsta1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_clr : 1;   /* [0] */
        unsigned int dhd0vtthd2_clr : 1;   /* [1] */
        unsigned int dhd0vtthd3_clr : 1;   /* [2] */
        unsigned int dhd0uf_clr : 1;       /* [3] */
        unsigned int dhd1vtthd1_clr : 1;   /* [4] */
        unsigned int dhd1vtthd2_clr : 1;   /* [5] */
        unsigned int dhd1vtthd3_clr : 1;   /* [6] */
        unsigned int dhd1uf_clr : 1;       /* [7] */
        unsigned int dsdvtthd1_clr : 1;    /* [8] */
        unsigned int dsdvtthd2_clr : 1;    /* [9] */
        unsigned int dsdvtthd3_clr : 1;    /* [10] */
        unsigned int dsduf_clr : 1;        /* [11] */
        unsigned int b0_err_clr : 1;       /* [12] */
        unsigned int b1_err_clr : 1;       /* [13] */
        unsigned int b2_err_clr : 1;       /* [14] */
        unsigned int wbc_dhd_over_clr : 1; /* [15] */
        unsigned int vdac0_clr : 1;        /* [16] */
        unsigned int vdac1_clr : 1;        /* [17] */
        unsigned int vdac2_clr : 1;        /* [18] */
        unsigned int vdac3_clr : 1;        /* [19] */
        unsigned int reserved_0 : 12;      /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomskintsta1;

/* Define the union u_vointmsk1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_intmask : 1;   /* [0] */
        unsigned int dhd0vtthd2_intmask : 1;   /* [1] */
        unsigned int dhd0vtthd3_intmask : 1;   /* [2] */
        unsigned int dhd0uf_intmask : 1;       /* [3] */
        unsigned int dhd1vtthd1_intmask : 1;   /* [4] */
        unsigned int dhd1vtthd2_intmask : 1;   /* [5] */
        unsigned int dhd1vtthd3_intmask : 1;   /* [6] */
        unsigned int dhd1uf_intmask : 1;       /* [7] */
        unsigned int dsdvtthd1_intmask : 1;    /* [8] */
        unsigned int dsdvtthd2_intmask : 1;    /* [9] */
        unsigned int dsdvtthd3_intmask : 1;    /* [10] */
        unsigned int dsduf_intmask : 1;        /* [11] */
        unsigned int b0_err_intmask : 1;       /* [12] */
        unsigned int b1_err_intmask : 1;       /* [13] */
        unsigned int b2_err_intmask : 1;       /* [14] */
        unsigned int wbc_dhd_over_intmask : 1; /* [15] */
        unsigned int vdac0_intmask : 1;        /* [16] */
        unsigned int vdac1_intmask : 1;        /* [17] */
        unsigned int vdac2_intmask : 1;        /* [18] */
        unsigned int vdac3_intmask : 1;        /* [19] */
        unsigned int reserved_0 : 12;          /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointmsk1;

/* Define the union u_mixg0_bkg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_bkgcr : 10; /* [9..0] */
        unsigned int mixer_bkgcb : 10; /* [19..10] */
        unsigned int mixer_bkgy : 10;  /* [29..20] */
        unsigned int reserved_0 : 2;   /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mixg0_bkg;

/* Define the union u_link_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int v1_link : 1;     /* [0] */
        unsigned int cbm1_sel : 1;    /* [1] */
        unsigned int hc_link : 1;     /* [2] */
        unsigned int reserved_0 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_link_ctrl;

/* Define the union u_para_haddr_vhd_chn06 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_haddr_vhd_chn06 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_para_haddr_vhd_chn06;

/* Define the union u_para_addr_vhd_chn06 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_addr_vhd_chn06 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_para_addr_vhd_chn06;

/* Define the union u_para_up_vhd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_up_vhd_chn00 : 1; /* [0] */
        unsigned int para_up_vhd_chn01 : 1; /* [1] */
        unsigned int para_up_vhd_chn02 : 1; /* [2] */
        unsigned int para_up_vhd_chn03 : 1; /* [3] */
        unsigned int para_up_vhd_chn04 : 1; /* [4] */
        unsigned int para_up_vhd_chn05 : 1; /* [5] */
        unsigned int para_up_vhd_chn06 : 1; /* [6] */
        unsigned int para_up_vhd_chn07 : 1; /* [7] */
        unsigned int para_up_vhd_chn08 : 1; /* [8] */
        unsigned int para_up_vhd_chn09 : 1; /* [9] */
        unsigned int para_up_vhd_chn10 : 1; /* [10] */
        unsigned int para_up_vhd_chn11 : 1; /* [11] */
        unsigned int para_up_vhd_chn12 : 1; /* [12] */
        unsigned int para_up_vhd_chn13 : 1; /* [13] */
        unsigned int para_up_vhd_chn14 : 1; /* [14] */
        unsigned int para_up_vhd_chn15 : 1; /* [15] */
        unsigned int para_up_vhd_chn16 : 1; /* [16] */
        unsigned int para_up_vhd_chn17 : 1; /* [17] */
        unsigned int para_up_vhd_chn18 : 1; /* [18] */
        unsigned int para_up_vhd_chn19 : 1; /* [19] */
        unsigned int para_up_vhd_chn20 : 1; /* [20] */
        unsigned int para_up_vhd_chn21 : 1; /* [21] */
        unsigned int para_up_vhd_chn22 : 1; /* [22] */
        unsigned int para_up_vhd_chn23 : 1; /* [23] */
        unsigned int para_up_vhd_chn24 : 1; /* [24] */
        unsigned int para_up_vhd_chn25 : 1; /* [25] */
        unsigned int para_up_vhd_chn26 : 1; /* [26] */
        unsigned int para_up_vhd_chn27 : 1; /* [27] */
        unsigned int para_up_vhd_chn28 : 1; /* [28] */
        unsigned int para_up_vhd_chn29 : 1; /* [29] */
        unsigned int para_up_vhd_chn30 : 1; /* [30] */
        unsigned int para_up_vhd_chn31 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_para_up_vhd;

/* Define the union u_v0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0] */
        unsigned int reserved_0 : 20; /* [27..8] */
        unsigned int rupd_field : 1;  /* [28] */
        unsigned int rgup_mode : 1;   /* [29] */
        unsigned int nosec_flag : 1;  /* [30] */
        unsigned int surface_en : 1;  /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_ctrl;

/* Define the union u_v0_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0] */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_upd;

/* Define the union u_v1_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0] */
        unsigned int reserved_0 : 20; /* [27..8] */
        unsigned int rupd_field : 1;  /* [28] */
        unsigned int rgup_mode : 1;   /* [29] */
        unsigned int nosec_flag : 1;  /* [30] */
        unsigned int surface_en : 1;  /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ctrl;

/* Define the union u_v1_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0] */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_upd;

/* Define the union u_v2_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0] */
        unsigned int reserved_0 : 20; /* [27..8] */
        unsigned int rupd_field : 1;  /* [28] */
        unsigned int rgup_mode : 1;   /* [29] */
        unsigned int nosec_flag : 1;  /* [30] */
        unsigned int surface_en : 1;  /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_ctrl;

/* Define the union u_v2_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0] */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_upd;

/* Define the union u_g0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;       /* [7..0] */
        unsigned int reserved_0 : 19;  /* [26..8] */
        unsigned int g0_depremult : 1; /* [27] */
        unsigned int rupd_field : 1;   /* [28] */
        unsigned int rgup_mode : 1;    /* [29] */
        unsigned int nosec_flag : 1;   /* [30] */
        unsigned int surface_en : 1;   /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ctrl;

/* Define the union u_g0_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0] */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_upd;

/* Define the union u_g0_galpha_sum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fk_alpha_sum : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_galpha_sum;

/* Define the union u_g0_0reso_read */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0] */
        unsigned int oh : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_0reso_read;

/* Define the union u_g0_dfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0] */
        unsigned int disp_yfpos : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dfpos;

/* Define the union u_g0_dlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0] */
        unsigned int disp_ylpos : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dlpos;

/* Define the union u_g0_vfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0] */
        unsigned int video_yfpos : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_vfpos;

/* Define the union u_g0_vlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0] */
        unsigned int video_ylpos : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_vlpos;

/* Define the union u_g0_hipp_csc_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_en : 1;       /* [0] */
        unsigned int hipp_csc_demo_en : 1;  /* [1] */
        unsigned int hipp_csc_ck_gt_en : 1; /* [2] */
        unsigned int reserved_0 : 29;       /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_ctrl;

/* Define the union u_g0_hipp_csc_coef00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef00 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef00;

/* Define the union u_g0_hipp_csc_coef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef01 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef01;

/* Define the union u_g0_hipp_csc_coef02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef02 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef02;

/* Define the union u_g0_hipp_csc_coef10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef10 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef10;

/* Define the union u_g0_hipp_csc_coef11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef11 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef11;

/* Define the union u_g0_hipp_csc_coef12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef12 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef12;

/* Define the union u_g0_hipp_csc_coef20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef20 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef20;

/* Define the union u_g0_hipp_csc_coef21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef21 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef21;

/* Define the union u_g0_hipp_csc_coef22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_coef22 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;      /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_coef22;

/* Define the union u_g0_hipp_csc_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_scale : 4; /* [3..0] */
        unsigned int reserved_0 : 28;    /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_scale;

/* Define the union u_g0_hipp_csc_idc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_idc0 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;    /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_idc0;

/* Define the union u_g0_hipp_csc_idc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_idc1 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;    /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_idc1;

/* Define the union u_g0_hipp_csc_idc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_idc2 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;    /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_idc2;

/* Define the union u_g0_hipp_csc_odc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_odc0 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;    /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_odc0;

/* Define the union u_g0_hipp_csc_odc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_odc1 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;    /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_odc1;

/* Define the union u_g0_hipp_csc_odc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_odc2 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;    /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_odc2;

/* Define the union u_g0_hipp_csc_min_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_min_y : 10; /* [9..0] */
        unsigned int reserved_0 : 22;     /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_min_y;

/* Define the union u_g0_hipp_csc_min_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_min_c : 10; /* [9..0] */
        unsigned int reserved_0 : 22;     /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_min_c;

/* Define the union u_g0_hipp_csc_max_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_max_y : 10; /* [9..0] */
        unsigned int reserved_0 : 22;     /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_max_y;

/* Define the union u_g0_hipp_csc_max_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc_max_c : 10; /* [9..0] */
        unsigned int reserved_0 : 22;     /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_max_c;

/* Define the union u_g0_hipp_csc2_coef00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef00 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef00;

/* Define the union u_g0_hipp_csc2_coef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef01 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef01;

/* Define the union u_g0_hipp_csc2_coef02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef02 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef02;

/* Define the union u_g0_hipp_csc2_coef10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef10 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef10;

/* Define the union u_g0_hipp_csc2_coef11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef11 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef11;

/* Define the union u_g0_hipp_csc2_coef12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef12 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef12;

/* Define the union u_g0_hipp_csc2_coef20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef20 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef20;

/* Define the union u_g0_hipp_csc2_coef21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef21 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef21;

/* Define the union u_g0_hipp_csc2_coef22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_coef22 : 16; /* [15..0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_coef22;

/* Define the union u_g0_hipp_csc2_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_scale : 4; /* [3..0] */
        unsigned int reserved_0 : 28;     /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_scale;

/* Define the union u_g0_hipp_csc2_idc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_idc0 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;     /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_idc0;

/* Define the union u_g0_hipp_csc2_idc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_idc1 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;     /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_idc1;

/* Define the union u_g0_hipp_csc2_idc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_idc2 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;     /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_idc2;

/* Define the union u_g0_hipp_csc2_odc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_odc0 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;     /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_odc0;

/* Define the union u_g0_hipp_csc2_odc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_odc1 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;     /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_odc1;

/* Define the union u_g0_hipp_csc2_odc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_odc2 : 11; /* [10..0] */
        unsigned int reserved_0 : 21;     /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_odc2;

/* Define the union u_g0_hipp_csc2_min_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_min_y : 10; /* [9..0] */
        unsigned int reserved_0 : 22;      /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_min_y;

/* Define the union u_g0_hipp_csc2_min_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_min_c : 10; /* [9..0] */
        unsigned int reserved_0 : 22;      /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_min_c;

/* Define the union u_g0_hipp_csc2_max_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_max_y : 10; /* [9..0] */
        unsigned int reserved_0 : 22;      /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_max_y;

/* Define the union u_g0_hipp_csc2_max_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hipp_csc2_max_c : 10; /* [9..0] */
        unsigned int reserved_0 : 22;      /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc2_max_c;

/* Define the union u_g0_hipp_csc_ink_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ink_en : 1;       /* [0] */
        unsigned int ink_sel : 1;      /* [1] */
        unsigned int data_fmt : 1;     /* [2] */
        unsigned int cross_enable : 1; /* [3] */
        unsigned int color_mode : 2;   /* [5..4] */
        unsigned int reserved_0 : 26;  /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_ink_ctrl;

/* Define the union u_g0_hipp_csc_ink_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int x_pos : 16; /* [15..0] */
        unsigned int y_pos : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_ink_pos;

/* Define the union u_g0_hipp_csc_ink_data */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ink_data : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_ink_data;
/* Define the union u_g0_hipp_csc_ink_data2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ink_data2 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_hipp_csc_ink_data2;
/* Define the union u_g0_dof_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0] */
        unsigned int dof_en : 1;      /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_ctrl;

/* Define the union u_g0_dof_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int left_step : 8;   /* [7..0] */
        unsigned int right_step : 8;  /* [15..8] */
        unsigned int reserved_0 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_step;

/* Define the union u_g0_dof_bkg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dof_bk_cr : 10; /* [9..0] */
        unsigned int dof_bk_cb : 10; /* [19..10] */
        unsigned int dof_bk_y : 10;  /* [29..20] */
        unsigned int reserved_0 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_bkg;

/* Define the union u_g0_dof_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dof_bk_alpha : 8; /* [7..0] */
        unsigned int reserved_0 : 24;  /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_alpha;

/* Define the union u_g0_zme_hinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_width : 16;  /* [15..0] */
        unsigned int ck_gt_en : 1;    /* [16] */
        unsigned int reserved_0 : 15; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hinfo;

/* Define the union u_g0_zme_hsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hratio : 24;      /* [23..0] */
        unsigned int hfir_order : 1;   /* [24] */
        unsigned int ahfir_mode : 1;   /* [25] */
        unsigned int lhfir_mode : 1;   /* [26] */
        unsigned int reserved_0 : 1;   /* [27] */
        unsigned int chfir_mid_en : 1; /* [28] */
        unsigned int lhfir_mid_en : 1; /* [29] */
        unsigned int ahfir_mid_en : 1; /* [30] */
        unsigned int hfir_en : 1;      /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hsp;

/* Define the union u_g0_zme_hloffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lhfir_offset : 24; /* [23..0] */
        unsigned int reserved_0 : 8;    /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hloffset;

/* Define the union u_g0_zme_hcoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chfir_offset : 24; /* [23..0] */
        unsigned int reserved_0 : 8;    /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hcoffset;

/* Define the union u_g0_zme_coef_ren */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;     /* [0] */
        unsigned int apb_g0_vf_lren : 1; /* [1] */
        unsigned int reserved_1 : 1;     /* [2] */
        unsigned int apb_g0_hf_lren : 1; /* [3] */
        unsigned int reserved_2 : 28;    /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_coef_ren;

/* Define the union u_g0_zme_coef_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_coef_raddr : 8; /* [7..0] */
        unsigned int reserved_0 : 24;        /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_coef_rdata;

/* Define the union u_g0_zme_vinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_height : 16; /* [15..0] */
        unsigned int reserved_0 : 2;  /* [17..16] */
        unsigned int out_pro : 1;     /* [18] */
        unsigned int reserved_1 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_vinfo;

/* Define the union u_g0_zme_vsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16;      /* [15..0] */
        unsigned int reserved_0 : 9;   /* [24..16] */
        unsigned int vafir_mode : 1;   /* [25] */
        unsigned int lvfir_mode : 1;   /* [26] */
        unsigned int reserved_1 : 1;   /* [27] */
        unsigned int cvfir_mid_en : 1; /* [28] */
        unsigned int lvfir_mid_en : 1; /* [29] */
        unsigned int avfir_mid_en : 1; /* [30] */
        unsigned int vfir_en : 1;      /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_vsp;

/* Define the union u_g0_zme_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbtm_offset : 16; /* [15..0] */
        unsigned int vtp_offset : 16;  /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_voffset;

/* Define the union u_dhd0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;          /* [0] */
        unsigned int disp_mode : 3;      /* [3..1] */
        unsigned int iop : 1;            /* [4] */
        unsigned int intf_ivs : 1;       /* [5] */
        unsigned int intf_ihs : 1;       /* [6] */
        unsigned int intf_idv : 1;       /* [7] */
        unsigned int reserved_0 : 1;     /* [8] */
        unsigned int hdmi420c_sel : 1;   /* [9] */
        unsigned int hdmi420_en : 1;     /* [10] */
        unsigned int uf_offline_en : 1;  /* [11] */
        unsigned int reserved_1 : 2;     /* [13..12] */
        unsigned int hdmi_mode : 1;      /* [14] */
        unsigned int twochn_debug : 1;   /* [15] */
        unsigned int twochn_en : 1;      /* [16] */
        unsigned int reserved_2 : 1;     /* [17] */
        unsigned int cbar_mode : 1;      /* [18] */
        unsigned int sin_en : 1;         /* [19] */
        unsigned int fpga_lmt_width : 7; /* [26..20] */
        unsigned int fpga_lmt_en : 1;    /* [27] */
        unsigned int p2i_en : 1;         /* [28] */
        unsigned int cbar_sel : 1;       /* [29] */
        unsigned int cbar_en : 1;        /* [30] */
        unsigned int intf_en : 1;        /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_ctrl;

/* Define the union u_dhd0_vsync1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vact : 16; /* [15..0] */
        unsigned int vbb : 16;  /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_vsync1;

/* Define the union u_dhd0_vsync2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vfb : 16;        /* [15..0] */
        unsigned int reserved_0 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_vsync2;

/* Define the union u_dhd0_hsync1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hact : 16; /* [15..0] */
        unsigned int hbb : 16;  /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_hsync1;

/* Define the union u_dhd0_hsync2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfb : 16;  /* [15..0] */
        unsigned int hmid : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_hsync2;

/* Define the union u_dhd0_vplus1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bvact : 16; /* [15..0] */
        unsigned int bvbb : 16;  /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_vplus1;

/* Define the union u_dhd0_vplus2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bvfb : 16;       /* [15..0] */
        unsigned int reserved_0 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_vplus2;

/* Define the union u_dhd0_pwr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hpw : 16;        /* [15..0] */
        unsigned int vpw : 8;         /* [23..16] */
        unsigned int reserved_0 : 3;  /* [26..24] */
        unsigned int multichn_en : 2; /* [28..27] */
        unsigned int reserved_1 : 3;  /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_pwr;

/* Define the union u_dhd0_vtthd3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd3 : 13;  /* [12..0] */
        unsigned int reserved_0 : 2; /* [14..13] */
        unsigned int thd3_mode : 1;  /* [15] */
        unsigned int vtmgthd4 : 13;  /* [28..16] */
        unsigned int reserved_1 : 2; /* [30..29] */
        unsigned int thd4_mode : 1;  /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_vtthd3;

/* Define the union u_dhd0_vtthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd1 : 13;  /* [12..0] */
        unsigned int reserved_0 : 2; /* [14..13] */
        unsigned int thd1_mode : 1;  /* [15] */
        unsigned int vtmgthd2 : 13;  /* [28..16] */
        unsigned int reserved_1 : 2; /* [30..29] */
        unsigned int thd2_mode : 1;  /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_vtthd;

/* Define the union u_dhd0_paraup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0] */
        unsigned int paraup_mode : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_paraup;

/* Define the union u_dhd0_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vback_blank : 1;  /* [0] */
        unsigned int vblank : 1;       /* [1] */
        unsigned int bottom_field : 1; /* [2] */
        unsigned int vcnt : 13;        /* [15..3] */
        unsigned int count_int : 8;    /* [23..16] */
        unsigned int dhd_even : 1;     /* [24] */
        unsigned int reserved_0 : 7;   /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_state;

/* Define the union u_vo_mux */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_sel : 4;    /* [3..0] */
        unsigned int lcd_sel : 4;     /* [7..4] */
        unsigned int bt_sel : 4;      /* [11..8] */
        unsigned int reserved_0 : 16; /* [27..12] */
        unsigned int digital_sel : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_mux;

/* Define the union u_gfx_read_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int read_mode : 2;    /* [1..0] */
        unsigned int reserved_0 : 2;   /* [3..2] */
        unsigned int draw_mode : 2;    /* [5..4] */
        unsigned int reserved_1 : 2;   /* [7..6] */
        unsigned int flip_en : 1;      /* [8] */
        unsigned int reserved_2 : 1;   /* [9] */
        unsigned int mute_en : 1;      /* [10] */
        unsigned int mute_req_en : 1;  /* [11] */
        unsigned int fdr_ck_gt_en : 1; /* [12] */
        unsigned int reserved_3 : 19;  /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_read_ctrl;

/* Define the union u_gfx_mac_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_ctrl : 2;    /* [1..0] */
        unsigned int req_len : 2;     /* [3..2] */
        unsigned int reserved_0 : 4;  /* [7..4] */
        unsigned int ofl_master : 1;  /* [8] */
        unsigned int reserved_1 : 23; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_mac_ctrl;

/* Define the union u_gfx_out_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int palpha_range : 1;   /* [0] */
        unsigned int palpha_en : 1;      /* [1] */
        unsigned int reserved_0 : 2;     /* [3..2] */
        unsigned int key_mode : 1;       /* [4] */
        unsigned int key_en : 1;         /* [5] */
        unsigned int reserved_1 : 2;     /* [7..6] */
        unsigned int bitext : 2;         /* [9..8] */
        unsigned int premulti_en : 1;    /* [10] */
        unsigned int testpattern_en : 1; /* [11] */
        unsigned int reserved_2 : 20;    /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_out_ctrl;

/* Define the union u_gfx_1555_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int alpha_0 : 8;     /* [7..0] */
        unsigned int alpha_1 : 8;     /* [15..8] */
        unsigned int reserved_0 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_1555_alpha;

/* Define the union u_gfx_src_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ifmt : 8;        /* [7..0] */
        unsigned int reserved_0 : 8;  /* [15..8] */
        unsigned int disp_mode : 4;   /* [19..16] */
        unsigned int dcmp_en : 1;     /* [20] */
        unsigned int reserved_1 : 11; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_src_info;

/* Define the union u_gfx_src_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_w : 16; /* [15..0] */
        unsigned int src_h : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_src_reso;

/* Define the union u_gfx_src_crop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_crop_x : 16; /* [15..0] */
        unsigned int src_crop_y : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_src_crop;

/* Define the union u_gfx_ireso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ireso_w : 16; /* [15..0] */
        unsigned int ireso_h : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ireso;

/* Define the union u_gfx_addr_h */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int gfx_addr_h : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_addr_h;
/* Define the union u_gfx_addr_l */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int gfx_addr_l : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_addr_l;
/* Define the union u_gfx_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int surface_stride : 16; /* [15..0] */
        unsigned int reserved_0 : 16;     /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_stride;

/* Define the union u_gfx_ckey_max */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_max : 8;  /* [7..0] */
        unsigned int key_g_max : 8;  /* [15..8] */
        unsigned int key_r_max : 8;  /* [23..16] */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ckey_max;

/* Define the union u_gfx_ckey_min */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_min : 8;  /* [7..0] */
        unsigned int key_g_min : 8;  /* [15..8] */
        unsigned int key_r_min : 8;  /* [23..16] */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ckey_min;

/* Define the union u_gfx_ckey_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_msk : 8;  /* [7..0] */
        unsigned int key_g_msk : 8;  /* [15..8] */
        unsigned int key_r_msk : 8;  /* [23..16] */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ckey_mask;

/* Define the global struct */
typedef struct {
    u_voctrl voctrl;                               /* 0x0 */
    u_vointsta vointsta;                           /* 0x4 */
    u_vomskintsta vomskintsta;                     /* 0x8 */
    u_vointmsk vointmsk;                           /* 0xc */
    unsigned int reserved_0;                       /* 0x10 */
    u_vointsta1 vointsta1;                         /* 0x14 */
    u_vomskintsta1 vomskintsta1;                   /* 0x18 */
    u_vointmsk1 vointmsk1;                         /* 0x1c */
    unsigned int reserved_1[568];                  /* 568 for 0x20~0x8fc */
    u_mixg0_bkg mixg0_bkg;                         /* 0x900 */
    unsigned int reserved_2[191];                  /* 191 for 0x904~0xbfc */
    u_link_ctrl link_ctrl;                         /* 0xc00 */
    unsigned int reserved_3[139];                  /* 139 for 0xc04~0xe2c */
    u_para_haddr_vhd_chn06 para_haddr_vhd_chn06;   /* 0xe30 */
    u_para_addr_vhd_chn06 para_addr_vhd_chn06;     /* 0xe34 */
    unsigned int reserved_4[50];                   /* 50 for 0xe38~0xefc */
    u_para_up_vhd para_up_vhd;                     /* 0xf00 */
    unsigned int reserved_5[63];                   /* 63 for 0xf04~0xffc */
    u_v0_ctrl v0_ctrl;                             /* 0x1000 */
    u_v0_upd v0_upd;                               /* 0x1004 */
    unsigned int reserved_6[1022];                 /* 1022 for 0x1008~0x1ffc */
    u_v1_ctrl v1_ctrl;                             /* 0x2000 */
    u_v1_upd v1_upd;                               /* 0x2004 */
    unsigned int reserved_7[1022];                 /* 1022 for 0x2008~0x2ffc */
    u_v2_ctrl v2_ctrl;                             /* 0x3000 */
    u_v2_upd v2_upd;                               /* 0x3004 */
    unsigned int reserved_8[4094];                 /* 4094 for 0x3008~0x6ffc */
    u_g0_ctrl g0_ctrl;                             /* 0x7000 */
    u_g0_upd g0_upd;                               /* 0x7004 */
    u_g0_galpha_sum g0_galpha_sum;                 /* 0x7008 */
    u_g0_0reso_read g0_0reso_read;                 /* 0x700c */
    unsigned int reserved_9[28];                   /* 28 for 0x7010~0x707c */
    u_g0_dfpos g0_dfpos;                           /* 0x7080 */
    u_g0_dlpos g0_dlpos;                           /* 0x7084 */
    u_g0_vfpos g0_vfpos;                           /* 0x7088 */
    u_g0_vlpos g0_vlpos;                           /* 0x708c */
    unsigned int reserved_10[28];                  /* 28 for 0x7090~0x70fc */
    u_g0_hipp_csc_ctrl g0_hipp_csc_ctrl;           /* 0x7100 */
    u_g0_hipp_csc_coef00 g0_hipp_csc_coef00;       /* 0x7104 */
    u_g0_hipp_csc_coef01 g0_hipp_csc_coef01;       /* 0x7108 */
    u_g0_hipp_csc_coef02 g0_hipp_csc_coef02;       /* 0x710c */
    u_g0_hipp_csc_coef10 g0_hipp_csc_coef10;       /* 0x7110 */
    u_g0_hipp_csc_coef11 g0_hipp_csc_coef11;       /* 0x7114 */
    u_g0_hipp_csc_coef12 g0_hipp_csc_coef12;       /* 0x7118 */
    u_g0_hipp_csc_coef20 g0_hipp_csc_coef20;       /* 0x711c */
    u_g0_hipp_csc_coef21 g0_hipp_csc_coef21;       /* 0x7120 */
    u_g0_hipp_csc_coef22 g0_hipp_csc_coef22;       /* 0x7124 */
    u_g0_hipp_csc_scale g0_hipp_csc_scale;         /* 0x7128 */
    u_g0_hipp_csc_idc0 g0_hipp_csc_idc0;           /* 0x712c */
    u_g0_hipp_csc_idc1 g0_hipp_csc_idc1;           /* 0x7130 */
    u_g0_hipp_csc_idc2 g0_hipp_csc_idc2;           /* 0x7134 */
    u_g0_hipp_csc_odc0 g0_hipp_csc_odc0;           /* 0x7138 */
    u_g0_hipp_csc_odc1 g0_hipp_csc_odc1;           /* 0x713c */
    u_g0_hipp_csc_odc2 g0_hipp_csc_odc2;           /* 0x7140 */
    u_g0_hipp_csc_min_y g0_hipp_csc_min_y;         /* 0x7144 */
    u_g0_hipp_csc_min_c g0_hipp_csc_min_c;         /* 0x7148 */
    u_g0_hipp_csc_max_y g0_hipp_csc_max_y;         /* 0x714c */
    u_g0_hipp_csc_max_c g0_hipp_csc_max_c;         /* 0x7150 */
    u_g0_hipp_csc2_coef00 g0_hipp_csc2_coef00;     /* 0x7154 */
    u_g0_hipp_csc2_coef01 g0_hipp_csc2_coef01;     /* 0x7158 */
    u_g0_hipp_csc2_coef02 g0_hipp_csc2_coef02;     /* 0x715c */
    u_g0_hipp_csc2_coef10 g0_hipp_csc2_coef10;     /* 0x7160 */
    u_g0_hipp_csc2_coef11 g0_hipp_csc2_coef11;     /* 0x7164 */
    u_g0_hipp_csc2_coef12 g0_hipp_csc2_coef12;     /* 0x7168 */
    u_g0_hipp_csc2_coef20 g0_hipp_csc2_coef20;     /* 0x716c */
    u_g0_hipp_csc2_coef21 g0_hipp_csc2_coef21;     /* 0x7170 */
    u_g0_hipp_csc2_coef22 g0_hipp_csc2_coef22;     /* 0x7174 */
    u_g0_hipp_csc2_scale g0_hipp_csc2_scale;       /* 0x7178 */
    u_g0_hipp_csc2_idc0 g0_hipp_csc2_idc0;         /* 0x717c */
    u_g0_hipp_csc2_idc1 g0_hipp_csc2_idc1;         /* 0x7180 */
    u_g0_hipp_csc2_idc2 g0_hipp_csc2_idc2;         /* 0x7184 */
    u_g0_hipp_csc2_odc0 g0_hipp_csc2_odc0;         /* 0x7188 */
    u_g0_hipp_csc2_odc1 g0_hipp_csc2_odc1;         /* 0x718c */
    u_g0_hipp_csc2_odc2 g0_hipp_csc2_odc2;         /* 0x7190 */
    u_g0_hipp_csc2_min_y g0_hipp_csc2_min_y;       /* 0x7194 */
    u_g0_hipp_csc2_min_c g0_hipp_csc2_min_c;       /* 0x7198 */
    u_g0_hipp_csc2_max_y g0_hipp_csc2_max_y;       /* 0x719c */
    u_g0_hipp_csc2_max_c g0_hipp_csc2_max_c;       /* 0x71a0 */
    unsigned int reserved_11[19];                  /* 19 for 0x71a4~0x71ec */
    u_g0_hipp_csc_ink_ctrl g0_hipp_csc_ink_ctrl;   /* 0x71f0 */
    u_g0_hipp_csc_ink_pos g0_hipp_csc_ink_pos;     /* 0x71f4 */
    u_g0_hipp_csc_ink_data g0_hipp_csc_ink_data;   /* 0x71f8 */
    u_g0_hipp_csc_ink_data2 g0_hipp_csc_ink_data2; /* 0x71fc */
    u_g0_dof_ctrl g0_dof_ctrl;                     /* 0x7200 */
    u_g0_dof_step g0_dof_step;                     /* 0x7204 */
    u_g0_dof_bkg g0_dof_bkg;                       /* 0x7208 */
    u_g0_dof_alpha g0_dof_alpha;                   /* 0x720c */
    unsigned int reserved_12[60];                  /* 60 for 0x7210~0x72fc */
    u_g0_zme_hinfo g0_zme_hinfo;                   /* 0x7300 */
    u_g0_zme_hsp g0_zme_hsp;                       /* 0x7304 */
    u_g0_zme_hloffset g0_zme_hloffset;             /* 0x7308 */
    u_g0_zme_hcoffset g0_zme_hcoffset;             /* 0x730c */
    unsigned int reserved_13[5];                   /* 5 for 0x7310~0x7320 */
    u_g0_zme_coef_ren g0_zme_coef_ren;             /* 0x7324 */
    u_g0_zme_coef_rdata g0_zme_coef_rdata;         /* 0x7328 */
    unsigned int reserved_14[21];                  /* 21 for 0x732c~0x737c */
    u_g0_zme_vinfo g0_zme_vinfo;                   /* 0x7380 */
    u_g0_zme_vsp g0_zme_vsp;                       /* 0x7384 */
    u_g0_zme_voffset g0_zme_voffset;               /* 0x7388 */
    unsigned int reserved_15[5917];                /* 5917 for 0x738c~0xcffc */
    u_dhd0_ctrl dhd0_ctrl;                         /* 0xd000 */
    u_dhd0_vsync1 dhd0_vsync1;                     /* 0xd004 */
    u_dhd0_vsync2 dhd0_vsync2;                     /* 0xd008 */
    u_dhd0_hsync1 dhd0_hsync1;                     /* 0xd00c */
    u_dhd0_hsync2 dhd0_hsync2;                     /* 0xd010 */
    u_dhd0_vplus1 dhd0_vplus1;                     /* 0xd014 */
    u_dhd0_vplus2 dhd0_vplus2;                     /* 0xd018 */
    u_dhd0_pwr dhd0_pwr;                           /* 0xd01c */
    u_dhd0_vtthd3 dhd0_vtthd3;                     /* 0xd020 */
    u_dhd0_vtthd dhd0_vtthd;                       /* 0xd024 */
    unsigned int reserved_16[4];                   /* 4 for 0xd028~0xd034 */
    u_dhd0_paraup dhd0_paraup;                     /* 0xd038 */
    unsigned int reserved_17[19];                  /* 19 for 0xd03c~0xd084 */
    u_dhd0_state dhd0_state;                       /* 0xd088 */
    unsigned int reserved_18;                      /* 0xd08c */
    u_vo_mux vo_mux;                               /* 0xd090 */
    unsigned int reserved_19[3931];                /* 3931 for 0xd094~0x10dfc */
    u_gfx_read_ctrl gfx_read_ctrl;                 /* 0x10e00 */
    u_gfx_mac_ctrl gfx_mac_ctrl;                   /* 0x10e04 */
    u_gfx_out_ctrl gfx_out_ctrl;                   /* 0x10e08 */
    unsigned int reserved_20[7];                   /* 7 for 0x10e0c~0x10e24 */
    u_gfx_1555_alpha gfx_1555_alpha;               /* 0x10e28 */
    unsigned int reserved_21[5];                   /* 5 for 0x10e2c~0x10e3c */
    u_gfx_src_info gfx_src_info;                   /* 0x10e40 */
    u_gfx_src_reso gfx_src_reso;                   /* 0x10e44 */
    u_gfx_src_crop gfx_src_crop;                   /* 0x10e48 */
    u_gfx_ireso gfx_ireso;                         /* 0x10e4c */
    u_gfx_addr_h gfx_addr_h;                       /* 0x10e50 */
    u_gfx_addr_l gfx_addr_l;                       /* 0x10e54 */
    unsigned int reserved_22[2];                   /* 2 for 0x10e58~0x10e5c */
    u_gfx_stride gfx_stride;                       /* 0x10e60 */
    unsigned int reserved_23[39];                  /* 39 for 0x10e64~0x10efc */
    u_gfx_ckey_max gfx_ckey_max;                   /* 0x10f00 */
    u_gfx_ckey_min gfx_ckey_min;                   /* 0x10f04 */
    u_gfx_ckey_mask gfx_ckey_mask;                 /* 0x10f08 */
} s_vdp_regs_type;

/* Declare the struct pointor of the module VDP */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __HIFB_REG_H__ */
