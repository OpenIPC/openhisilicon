/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_VIPROC_REG_DEFINE_H
#define ISP_VIPROC_REG_DEFINE_H

/* Define the union u_isp_viproc_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out0_crop_en : 1;       /* [0]  */
        unsigned int reserved_0 : 5;         /* [5..1]  */
        unsigned int reserved_1 : 6;         /* [11..6]  */
        unsigned int isp_be_crop_en : 1;     /* [12]  */
        unsigned int isp_be_en : 1;          /* [13]  */
        unsigned int limit_range_en : 1;     /* [14]  */
        unsigned int reserved_2 : 1;         /* [15]  */
        unsigned int img_pro_mode : 2;       /* [17..16]  */
        unsigned int vcpin_sfifo_en : 1;     /* [18]  */
        unsigned int wdr_curc_mux : 1;         /* [19]  */
        unsigned int wdr_fpn_mux : 1;        /* [20]  */
        unsigned int dcg_fpn_sel : 1;        /* [21]  */
        unsigned int ch0_mux : 1;            /* [22]  */
        unsigned int reserved_4 : 3;         /* [25..23]  */
        unsigned int isp_be_bypass_mode : 2; /* [27..26]  */
        unsigned int wdr_mode : 1;           /* [28]  */
        unsigned int reserved_5 : 2;         /* [30..29]  */
        unsigned int video_en : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_ctrl0;

/* Define the union u_isp_viproc_regup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_up : 1;        /* [0]  */
        unsigned int reserved_0 : 1;    /* [1]  */
        unsigned int reserved_1 : 1;    /* [2]  */
        unsigned int reserved_2 : 5;    /* [7..3]  */
        unsigned int manual_reg_up : 1; /* [8]  */
        unsigned int reserved_3 : 23;   /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_regup;

/* Define the union u_isp_viproc_ispbe_regup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ispbe_reg_up : 1; /* [0]  */
        unsigned int reserved_0 : 31;  /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_ispbe_regup;

/* Define the union u_isp_viproc_ispbe_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdrsplit1_en : 1; /* [0]  */
        unsigned int isp_fpn_en : 1;       /* [1]  */
        unsigned int isp_fpn_mode : 1;     /* [2]  */
        unsigned int isp_dpc_en : 1;       /* [3]  */
        unsigned int isp_dpc_stat_en : 1;  /* [4]  */
        unsigned int isp_ge_en : 1;        /* [5]  */
        unsigned int isp_4dg_en : 1;       /* [6]  */
        unsigned int isp_flicker_en : 1;   /* [7]  */
        unsigned int isp_wdr_en : 1;       /* [8]  */
        unsigned int isp_expander_en : 1;  /* [9]  */
        unsigned int isp_bcom_en : 1;      /* [10]  */
        unsigned int isp_bnr_en : 1;       /* [11]  */
        unsigned int isp_bdec_en : 1;      /* [12]  */
        unsigned int isp_lsc_en : 1;       /* [13]  */
        unsigned int isp_dg_en : 1;        /* [14]  */
        unsigned int isp_af_en : 1;        /* [15]  */
        unsigned int isp_awb_en : 1;       /* [16]  */
        unsigned int isp_ae_en : 1;        /* [17]  */
        unsigned int isp_dis_en : 1;       /* [18]  */
        unsigned int isp_wb_en : 1;        /* [19]  */
        unsigned int isp_blc_en : 1;       /* [20]  */
        unsigned int isp_pregamma_en : 1;  /* [21]  */
        unsigned int isp_drc_en : 1;       /* [22]  */
        unsigned int reserved_0 : 1;       /* [23]  */
        unsigned int isp_la_en : 1;        /* [24]  */
        unsigned int isp_sqrt_en : 1;      /* [25]  */
        unsigned int isp_dmnr_vhdm_en : 1; /* [26]  */
        unsigned int isp_dmnr_nddm_en : 1; /* [27]  */
        unsigned int isp_loglut_en : 1;    /* [28]  */
        unsigned int isp_rlsc_en : 1;      /* [29]  */
        unsigned int isp_rgbir_en : 1;     /* [30]  */
        unsigned int reserved_1 : 1;       /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_ispbe_ctrl0;

/* Define the union u_isp_viproc_ispbe_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sq_en : 1;      /* [0]  */
        unsigned int isp_cc_en : 1;      /* [1]  */
        unsigned int isp_clut_en : 1;    /* [2]  */
        unsigned int isp_sqrt1_en : 1;   /* [3]  */
        unsigned int isp_gamma_en : 1;   /* [4]  */
        unsigned int isp_dehaze_en : 1;  /* [5]  */
        unsigned int isp_csc_en : 1;     /* [6]  */
        unsigned int isp_sumy_en : 1;    /* [7]  */
        unsigned int isp_ldci_en : 1;    /* [8]  */
        unsigned int reserved_0 : 1;     /* [9]  */
        unsigned int reserved_1 : 1;     /* [10]  */
        unsigned int isp_ca_en : 1;      /* [11]  */
        unsigned int isp_split_en : 1;   /* [12]  */
        unsigned int isp_sharpen_en : 1; /* [13]  */
        unsigned int isp_hcds_en : 1;    /* [14]  */
        unsigned int isp_hlc_en : 1;     /* [15]  */
        unsigned int isp_de_en : 1;      /* [16]  */
        unsigned int isp_vcds_en : 1;    /* [17]  */
        unsigned int isp_nry0_en : 1;    /* [18]  */
        unsigned int reserved_2 : 13;    /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_ispbe_ctrl1;

/* Define the union u_isp_viproc_ispbe_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_be_rggb_cfg : 2;    /* [1..0]  */
        unsigned int reserved_0 : 14;        /* [15..2]  */
        unsigned int isp_be_timing_cfg : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_ispbe_cfg;

/* Define the union u_isp_viproc_fstart_dly */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 16;     /* [15..0]  */
        unsigned int fstart_dly_cfg : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_fstart_dly;

/* Define the union u_isp_viproc_para_drc_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_drc_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_para_drc_addr_low;
/* Define the union u_isp_viproc_para_dci_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_dci_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_para_dci_addr_low;
/* Define the union u_isp_viproc_out_para_drc_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_drc_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_out_para_drc_addr_low;
/* Define the union u_isp_viproc_out_para_dci_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_dci_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_out_para_dci_addr_low;


/* Define the union u_isp_viproc_isp_ae_hist_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ae_hist_addr_low;
/* Define the union u_isp_viproc_isp_ae_aver_r_gr_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ae_aver_r_gr_addr_low;
/* Define the union u_isp_viproc_isp_ae_aver_gb_b_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ae_aver_gb_b_addr_low;
/* Define the union u_isp_viproc_isp_awb_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_awb_stat_addr_low;
/* Define the union u_isp_viproc_isp_af_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_af_stat_addr_low;
/* Define the union u_isp_viproc_isp_dis_h_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_h_stat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dis_h_stat_addr_low;
/* Define the union u_isp_viproc_isp_dis_v_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_v_stat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dis_v_stat_addr_low;
/* Define the union u_isp_viproc_isp_la_aver_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_la_aver_addr_low;
/* Define the union u_isp_viproc_isp_ge_aver0_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_aver0_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ge_aver0_addr_low;
/* Define the union u_isp_viproc_isp_ge_aver1_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_aver1_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ge_aver1_addr_low;
/* Define the union u_isp_viproc_isp_dehaze_min_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_min_stat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dehaze_min_stat_addr_low;
/* Define the union u_isp_viproc_isp_dehaze_max_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_max_stat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dehaze_max_stat_addr_low;
/* Define the union u_isp_viproc_isp_flick_gmean_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_flick_gmean_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_flick_gmean_addr_low;
/* Define the union u_isp_viproc_isp_ldci_lpf_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lpf_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ldci_lpf_addr_low;
/* Define the union u_isp_viproc_out1_finish_line */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out1_finish_line : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_out1_finish_line;

/* Define the union u_isp_viproc_isp_dcg_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dcg_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dcg_lut_addr_low;
/* Define the union u_isp_viproc_isp_dcg_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dcg_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dcg_lut_width_word;

/* Define the union u_isp_viproc_isp_bnr_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_bnr_lut_addr_low;
/* Define the union u_isp_viproc_isp_bnr_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_bnr_lut_width_word;

/* Define the union u_isp_viproc_isp_lsc_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_lsc_lut_addr_low;
/* Define the union u_isp_viproc_isp_lsc_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_lsc_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_lsc_lut_width_word;

/* Define the union u_isp_viproc_isp_clut_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_clut_lut_addr_low;
/* Define the union u_isp_viproc_isp_clut_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_clut_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;              /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_clut_lut_width_word;

/* Define the union u_isp_viproc_isp_gamma_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_gamma_lut_addr_low;
/* Define the union u_isp_viproc_isp_gamma_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;               /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_gamma_lut_width_word;

/* Define the union u_isp_viproc_isp_dehaze_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dehaze_lut_addr_low;
/* Define the union u_isp_viproc_isp_dehaze_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;                /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_dehaze_lut_width_word;

/* Define the union u_isp_viproc_isp_ldci_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ldci_lut_addr_low;
/* Define the union u_isp_viproc_isp_ldci_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;              /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ldci_lut_width_word;

/* Define the union u_isp_viproc_isp_ca_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ca_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ca_lut_addr_low;
/* Define the union u_isp_viproc_isp_ca_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_ca_lut_width_word;

/* Define the union u_isp_viproc_isp_sharpen_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_sharpen_lut_addr_low;
/* Define the union u_isp_viproc_isp_sharpen_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;                 /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_sharpen_lut_width_word;

/* Define the union u_isp_viproc_isp_pregamma_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_pregamma_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_pregamma_lut_addr_low;
/* Define the union u_isp_viproc_isp_pregamma_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_pregamma_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;                  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_pregamma_lut_width_word;

/* Define the union u_isp_viproc_isp_cc_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_cc_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_cc_lut_addr_low;

/* Define the union u_isp_viproc_isp_cc_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_lut_width_word : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_viproc_isp_cc_lut_width_word;

/* Define the global struct */
typedef struct {
    volatile u_isp_viproc_ctrl0 viproc_ctrl0;                                    /* 0x0 */
    volatile unsigned int reserved_0[3];                                         /* 0x4~0xc */
    volatile u_isp_viproc_regup viproc_regup;                                    /* 0x10 */
    volatile u_isp_viproc_ispbe_regup viproc_ispbe_regup;                        /* 0x14 */
    volatile unsigned int reserved_1[2];                                         /* 0x18~0x1c */
    volatile u_isp_viproc_ispbe_ctrl0 viproc_ispbe_ctrl0;                        /* 0x20 */
    volatile u_isp_viproc_ispbe_ctrl1 viproc_ispbe_ctrl1;                        /* 0x24 */
    volatile unsigned int reserved_2;                                            /* 0x28 */
    volatile u_isp_viproc_ispbe_cfg viproc_ispbe_cfg;                            /* 0x2c */
    volatile u_isp_viproc_fstart_dly viproc_fstart_dly;                          /* 0x30 */
    volatile unsigned int reserved_3[299];                                       /* 0x34~0x4dc */
    volatile u_isp_viproc_para_drc_addr_low viproc_para_drc_addr_low;            /* 0x4e0 */
    volatile u_isp_viproc_para_dci_addr_low viproc_para_dci_addr_low;            /* 0x4e4 */
    volatile u_isp_viproc_out_para_drc_addr_low viproc_out_para_drc_addr_low;    /* 0x4e8 */
    volatile u_isp_viproc_out_para_dci_addr_low viproc_out_para_dci_addr_low;    /* 0x4ec */
    volatile unsigned int reserved_33[28];                                       /* 0x4f0~0x55c */
    volatile u_isp_viproc_isp_ae_hist_addr_low ae_hist_addr_low;                 /* 0x560 */
    volatile u_isp_viproc_isp_ae_aver_r_gr_addr_low ae_aver_r_gr_addr_low;       /* 0x564 */
    volatile u_isp_viproc_isp_ae_aver_gb_b_addr_low ae_aver_gb_b_addr_low;       /* 0x568 */
    volatile u_isp_viproc_isp_awb_stat_addr_low awb_stat_addr_low;               /* 0x56c */
    volatile u_isp_viproc_isp_af_stat_addr_low af_stat_addr_low;                 /* 0x570 */
    volatile u_isp_viproc_isp_dis_h_stat_addr_low dis_h_stat_addr_low;           /* 0x574 */
    volatile u_isp_viproc_isp_dis_v_stat_addr_low dis_v_stat_addr_low;           /* 0x578 */
    volatile u_isp_viproc_isp_la_aver_addr_low la_aver_addr_low;                 /* 0x57c */
    volatile u_isp_viproc_isp_ge_aver0_addr_low ge_aver0_addr_low;               /* 0x580 */
    volatile u_isp_viproc_isp_ge_aver1_addr_low ge_aver1_addr_low;               /* 0x584 */
    volatile u_isp_viproc_isp_dehaze_min_stat_addr_low dehaze_min_stat_addr_low; /* 0x588 */
    volatile u_isp_viproc_isp_dehaze_max_stat_addr_low dehaze_max_stat_addr_low; /* 0x58c */
    volatile u_isp_viproc_isp_flick_gmean_addr_low flick_gmean_addr_low;         /* 0x590 */
    volatile u_isp_viproc_isp_ldci_lpf_addr_low ldci_lpf_addr_low;               /* 0x594 */
    volatile u_isp_viproc_out1_finish_line viproc_out1_finish_line;              /* 0x598 */
    volatile u_isp_viproc_isp_dcg_lut_addr_low dcg_lut_addr_low;                 /* 0x59c */
    volatile u_isp_viproc_isp_dcg_lut_width_word dcg_lut_width_word;             /* 0x5a0 */
    volatile u_isp_viproc_isp_bnr_lut_addr_low bnr_lut_addr_low;                 /* 0x5a4 */
    volatile u_isp_viproc_isp_bnr_lut_width_word bnr_lut_width_word;             /* 0x5a8 */
    volatile u_isp_viproc_isp_lsc_lut_addr_low lsc_lut_addr_low;                 /* 0x5ac */
    volatile u_isp_viproc_isp_lsc_lut_width_word lsc_lut_width_word;             /* 0x5b0 */
    volatile u_isp_viproc_isp_clut_lut_addr_low clut_lut_addr_low;               /* 0x5b4 */
    volatile u_isp_viproc_isp_clut_lut_width_word clut_lut_width_word;           /* 0x5b8 */
    volatile u_isp_viproc_isp_gamma_lut_addr_low gamma_lut_addr_low;             /* 0x5bc */
    volatile u_isp_viproc_isp_gamma_lut_width_word gamma_lut_width_word;         /* 0x5c0 */
    volatile u_isp_viproc_isp_dehaze_lut_addr_low dehaze_lut_addr_low;           /* 0x5c4 */
    volatile u_isp_viproc_isp_dehaze_lut_width_word dehaze_lut_width_word;       /* 0x5c8 */
    volatile u_isp_viproc_isp_ldci_lut_addr_low ldci_lut_addr_low;               /* 0x5cc */
    volatile u_isp_viproc_isp_ldci_lut_width_word ldci_lut_width_word;           /* 0x5d0 */
    volatile u_isp_viproc_isp_ca_lut_addr_low ca_lut_addr_low;                   /* 0x5d4 */
    volatile u_isp_viproc_isp_ca_lut_width_word ca_lut_width_word;               /* 0x5d8 */
    volatile u_isp_viproc_isp_sharpen_lut_addr_low sharpen_lut_addr_low;         /* 0x5dc */
    volatile u_isp_viproc_isp_sharpen_lut_width_word sharpen_lut_width_word;     /* 0x5e0 */
    volatile u_isp_viproc_isp_pregamma_lut_addr_low pregamma_lut_addr_low;       /* 0x5e4 */
    volatile u_isp_viproc_isp_pregamma_lut_width_word pregamma_lut_width_word;   /* 0x5e8 */
    volatile u_isp_viproc_isp_cc_lut_addr_low cc_lut_addr_low;                   /* 0x5ec */
    volatile u_isp_viproc_isp_cc_lut_width_word cc_lut_width_word;               /* 0x5f0 */
    volatile unsigned int reserved_60[3];                     /* 0x5f4 ~ 0x604, reserved 5* 4 bytes */
} isp_viproc_reg_type;

#endif /* ISP_VIPROC_REG_DEFINE_H */
