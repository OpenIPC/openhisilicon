/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_RX_REG_H
#define MIPI_RX_REG_H

/* Define the union u_phy_mode_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_en_d : 4;                /* [3..0]  */
        unsigned int phy_rg_en_clk : 1;              /* [4]  */
        unsigned int phy_rg_en_clk2 : 1;             /* [5]  */
        unsigned int reserved_0 : 2;                 /* [7..6]  */
        unsigned int phy_rg_en_cmos : 1;             /* [8]  */
        unsigned int phy_rg_en_mipi2lcmos : 1;       /* [9]  */
        unsigned int phy_rg_en_mipi2lcmos1_swap : 1; /* [10]  */
        unsigned int phy_rg_da_en_cmos_tx : 1;       /* [11]  */
        unsigned int phy_rg_int_en : 1;              /* [12]  */
        unsigned int phy_rg_ext2_en : 1;             /* [13]  */
        unsigned int phy_rg_ext_en : 1;              /* [14]  */
        unsigned int phy_rg_extbus_en : 1;           /* [15]  */
        unsigned int phy_rg_drveclk_enz : 1;         /* [16]  */
        unsigned int phy_rg_drveclk2_enz : 1;        /* [17]  */
        unsigned int reserved_1 : 6;                 /* [23..18]  */
        unsigned int phy_rg_mipi_mode : 1;           /* [24]  */
        unsigned int phy_rg_mipi_mode2 : 1;          /* [25]  */
        unsigned int reserved_2 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_mode_link;

/* Define the union u_phy_skew_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_en_deskew : 4; /* [3..0]  */
        unsigned int phy_rg_deskew : 4;    /* [7..4]  */
        unsigned int phy_rg_dirbi : 4;     /* [11..8]  */
        unsigned int phy_rg_aadh_ds : 4;   /* [15..12]  */
        unsigned int phy_rg_adv12 : 4;     /* [19..16]  */
        unsigned int phy_rg_ah_ds : 4;     /* [23..20]  */
        unsigned int reserved_0 : 8;       /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_skew_link;

/* Define the union u_phy_lane_si_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_lane0_si : 8; /* [7..0]  */
        unsigned int phy_rg_lane1_si : 8; /* [15..8]  */
        unsigned int phy_rg_lane2_si : 8; /* [23..16]  */
        unsigned int phy_rg_lane3_si : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_lane_si_link;

/* Define the union u_phy_en_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_d0_valid : 1;    /* [0]  */
        unsigned int phy_da_d1_valid : 1;    /* [1]  */
        unsigned int phy_da_d2_valid : 1;    /* [2]  */
        unsigned int phy_da_d3_valid : 1;    /* [3]  */
        unsigned int phy_da_d_valid_inv : 4; /* [7..4]  */
        unsigned int phy_d0_term_en : 1;     /* [8]  */
        unsigned int phy_d1_term_en : 1;     /* [9]  */
        unsigned int phy_d2_term_en : 1;     /* [10]  */
        unsigned int phy_d3_term_en : 1;     /* [11]  */
        unsigned int phy_clk_term_en : 1;    /* [12]  */
        unsigned int phy_clk2_term_en : 1;   /* [13]  */
        unsigned int reserved_0 : 18;        /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_en_link;

/* Define the union u_phy_eq_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_eq_clk : 4;   /* [3..0]  */
        unsigned int phy_rg_eq_clk2 : 4;  /* [7..4]  */
        unsigned int phy_rg_eq_lane0 : 4; /* [11..8]  */
        unsigned int phy_rg_eq_lane1 : 4; /* [15..12]  */
        unsigned int phy_rg_eq_lane2 : 4; /* [19..16]  */
        unsigned int phy_rg_eq_lane3 : 4; /* [23..20]  */
        unsigned int reserved_0 : 8;      /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_eq_link;

/* Define the union u_phy_cfg_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_ulpb_ck0 : 1;  /* [0]  */
        unsigned int phy_da_ulpb_ck1 : 1;  /* [1]  */
        unsigned int phy_da_idlb_clk0 : 1; /* [2]  */
        unsigned int phy_da_idlb_clk1 : 1; /* [3]  */
        unsigned int phy_da_ulpb : 4;      /* [7..4]  */
        unsigned int phy_da_idlb : 4;      /* [11..8]  */
        unsigned int phy_rg_test : 4;      /* [15..12]  */
        unsigned int phy_rg_sa_en : 1;     /* [16]  */
        unsigned int reserved_0 : 15;      /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cfg_link;

/* Define the union u_phy_data_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_data0_mipi : 8; /* [7..0]  */
        unsigned int phy_data1_mipi : 8; /* [15..8]  */
        unsigned int phy_data2_mipi : 8; /* [23..16]  */
        unsigned int phy_data3_mipi : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_data_link;

/* Define the union u_phy_ph_mipi_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_ph0_mipi : 8; /* [7..0]  */
        unsigned int phy_ph1_mipi : 8; /* [15..8]  */
        unsigned int phy_ph2_mipi : 8; /* [23..16]  */
        unsigned int phy_ph3_mipi : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_ph_mipi_link;

/* Define the union u_phy_data_mipi_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_data0_mipi_hs : 8; /* [7..0]  */
        unsigned int phy_data1_mipi_hs : 8; /* [15..8]  */
        unsigned int phy_data2_mipi_hs : 8; /* [23..16]  */
        unsigned int phy_data3_mipi_hs : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_data_mipi_link;

/* Define the union u_phy_sync_dct_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_raw_type0 : 3;        /* [2..0]  */
        unsigned int reserved_0 : 1;           /* [3]  */
        unsigned int cil_split_mode0 : 3;      /* [6..4]  */
        unsigned int reserved_1 : 1;           /* [7]  */
        unsigned int cil_code_big_endian0 : 1; /* [8]  */
        unsigned int reserved_2 : 3;           /* [11..9]  */
        unsigned int cil_raw_type1 : 3;        /* [14..12]  */
        unsigned int reserved_3 : 1;           /* [15]  */
        unsigned int cil_split_mode1 : 3;      /* [18..16]  */
        unsigned int reserved_4 : 1;           /* [19]  */
        unsigned int cil_code_big_endian1 : 1; /* [20]  */
        unsigned int reserved_5 : 3;           /* [23..21]  */
        unsigned int cil_split_sel : 1;        /* [24]  */
        unsigned int cil_sync_det_mode : 1;    /* [25]  */
        unsigned int reserved_6 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_dct_link;

/* Define the union u_phy_cmos_dir */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int pad_datap0_cmos_rx_en : 1; /* [0]  */
        unsigned int pad_datan0_cmos_rx_en : 1; /* [1]  */
        unsigned int pad_datap1_cmos_rx_en : 1; /* [2]  */
        unsigned int pad_datan1_cmos_rx_en : 1; /* [3]  */
        unsigned int pad_datap2_cmos_rx_en : 1; /* [4]  */
        unsigned int pad_datan2_cmos_rx_en : 1; /* [5]  */
        unsigned int pad_datap3_cmos_rx_en : 1; /* [6]  */
        unsigned int pad_datan3_cmos_rx_en : 1; /* [7]  */
        unsigned int pad_clkp_cmos_rx_en : 1;   /* [8]  */
        unsigned int pad_clkn_cmos_rx_en : 1;   /* [9]  */
        unsigned int pad_clkp2_cmos_rx_en : 1;  /* [10]  */
        unsigned int pad_clkn2_cmos_rx_en : 1;  /* [11]  */
        unsigned int pad_datap0_cmos_tx_en : 1; /* [12]  */
        unsigned int pad_datan0_cmos_tx_en : 1; /* [13]  */
        unsigned int pad_datap1_cmos_tx_en : 1; /* [14]  */
        unsigned int pad_datan1_cmos_tx_en : 1; /* [15]  */
        unsigned int pad_datap2_cmos_tx_en : 1; /* [16]  */
        unsigned int pad_datan2_cmos_tx_en : 1; /* [17]  */
        unsigned int pad_datap3_cmos_tx_en : 1; /* [18]  */
        unsigned int pad_datan3_cmos_tx_en : 1; /* [19]  */
        unsigned int pad_clkp_cmos_tx_en : 1;   /* [20]  */
        unsigned int pad_clkn_cmos_tx_en : 1;   /* [21]  */
        unsigned int pad_clkp2_cmos_tx_en : 1;  /* [22]  */
        unsigned int pad_clkn2_cmos_tx_en : 1;  /* [23]  */
        unsigned int reserved_0 : 8;            /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cmos_dir;

/* Define the union u_phy_sync_sof0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_0 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof0_link;

/* Define the union u_phy_sync_sof1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_1 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof1_link;

/* Define the union u_phy_sync_sof2_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_2 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof2_link;

/* Define the union u_phy_sync_sof3_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_3 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof3_link;

/* Define the union u_phy_out_valid_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_lvds_out_valid : 4; /* [3..0]  */
        unsigned int cil_mipi_out_valid : 4; /* [7..4]  */
        unsigned int reserved_0 : 24;        /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_out_valid_link;

/* Define the union u_phy_data_lvds_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_data0_lvds_hs : 8; /* [7..0]  */
        unsigned int phy_data1_lvds_hs : 8; /* [15..8]  */
        unsigned int phy_data2_lvds_hs : 8; /* [23..16]  */
        unsigned int phy_data3_lvds_hs : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_data_lvds_link;

/* Define the union u_phy_deskew_cal_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int deskew_cal_cycle : 16;     /* [15..0]  */
        unsigned int deskew_cal_en : 4;         /* [19..16]  */
        unsigned int deskew_cal_rstn_en : 4;    /* [23..20]  */
        unsigned int deskew_cal_sot_bypass : 4; /* [27..24]  */
        unsigned int deskew_cal_8b1 : 4;        /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_deskew_cal_link;

/* Define the union u_phy_pix_pum_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_lane02_pix_num : 16; /* [15..0]  */
        unsigned int phy_lane13_pix_num : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_pix_pum_link;

/* Define the union u_phy_sync_sol0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_0 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol0_link;

/* Define the union u_phy_sync_sol1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_1 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol1_link;

/* Define the union u_phy_sync_sol2_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_2 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol2_link;

/* Define the union u_phy_sync_sol3_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_3 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol3_link;

/* Define the union u_cil_timeout_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_timeout_cyc : 24;  /* [23..0]  */
        unsigned int reserved_0 : 6;        /* [29..24]  */
        unsigned int cil_timeout_en_ck : 1; /* [30]  */
        unsigned int cil_timeout_en_d : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_timeout_link;

/* Define the union u_cil_fsm0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cyc_clk_lp00 : 6;     /* [5..0]  */
        unsigned int reserved_0 : 2;           /* [7..6]  */
        unsigned int cil_cyc_clk_hs0 : 6;      /* [13..8]  */
        unsigned int reserved_1 : 2;           /* [15..14]  */
        unsigned int cil_cyc_clk_hs_exit : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2;           /* [23..22]  */
        unsigned int cil_cyc_clk_hs_trail : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;           /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm0_link;

/* Define the union u_cil_fsm_st0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cur_st_d0 : 4; /* [3..0]  */
        unsigned int cil_nxt_st_d0 : 4; /* [7..4]  */
        unsigned int cil_cur_st_d1 : 4; /* [11..8]  */
        unsigned int cil_nxt_st_d1 : 4; /* [15..12]  */
        unsigned int cil_cur_st_d2 : 4; /* [19..16]  */
        unsigned int cil_nxt_st_d2 : 4; /* [23..20]  */
        unsigned int cil_cur_st_d3 : 4; /* [27..24]  */
        unsigned int cil_nxt_st_d3 : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm_st0_link;

/* Define the union u_cil_fsm_st1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cur_st_ck : 4;  /* [3..0]  */
        unsigned int cil_nxt_st_ck : 4;  /* [7..4]  */
        unsigned int cil_cur_st_ck2 : 4; /* [11..8]  */
        unsigned int cil_nxt_st_ck2 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm_st1_link;

/* Define the union u_phy_st0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_rg_en_d : 4;         /* [3..0]  */
        unsigned int physt_rg_en_clk : 1;       /* [4]  */
        unsigned int physt_rg_en_clk2 : 1;      /* [5]  */
        unsigned int reserved_0 : 2;            /* [7..6]  */
        unsigned int physt_rg_en_cmos : 1;      /* [8]  */
        unsigned int reserved_1 : 3;            /* [11..9]  */
        unsigned int physt_rg_int_en : 1;       /* [12]  */
        unsigned int physt_rg_ext2_en : 1;      /* [13]  */
        unsigned int physt_rg_ext_en : 1;       /* [14]  */
        unsigned int physt_rg_extbus_en : 1;    /* [15]  */
        unsigned int physt_rg_drveclk_enz : 1;  /* [16]  */
        unsigned int physt_rg_drveclk2_enz : 1; /* [17]  */
        unsigned int reserved_2 : 6;            /* [23..18]  */
        unsigned int physt_rg_mipi_mode : 1;    /* [24]  */
        unsigned int physt_rg_mipi_mode2 : 1;   /* [25]  */
        unsigned int reserved_3 : 6;            /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st0_link;

/* Define the union u_phy_st1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_da_en_deskew : 4; /* [3..0]  */
        unsigned int physt_rg_deskew : 4;    /* [7..4]  */
        unsigned int physt_rg_dirbi : 4;     /* [11..8]  */
        unsigned int physt_rg_aadh_ds : 4;   /* [15..12]  */
        unsigned int physt_rg_adv12 : 4;     /* [19..16]  */
        unsigned int physt_rg_ah_ds : 4;     /* [23..20]  */
        unsigned int physt_ad_dirb_d : 4;    /* [27..24]  */
        unsigned int reserved_0 : 4;         /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st1_link;

/* Define the union u_phy_st2_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_rg_lane0_si : 4; /* [3..0]  */
        unsigned int physt_rg_lane1_si : 4; /* [7..4]  */
        unsigned int physt_rg_lane2_si : 4; /* [11..8]  */
        unsigned int physt_rg_lane3_si : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st2_link;

/* Define the union u_phy_st3_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_da_d0_valid : 1;    /* [0]  */
        unsigned int physt_da_d1_valid : 1;    /* [1]  */
        unsigned int physt_da_d2_valid : 1;    /* [2]  */
        unsigned int physt_da_d3_valid : 1;    /* [3]  */
        unsigned int physt_da_d_valid_inv : 4; /* [7..4]  */
        unsigned int physt_d0_term_en : 1;     /* [8]  */
        unsigned int physt_d1_term_en : 1;     /* [9]  */
        unsigned int physt_d2_term_en : 1;     /* [10]  */
        unsigned int physt_d3_term_en : 1;     /* [11]  */
        unsigned int physt_clk_term_en : 1;    /* [12]  */
        unsigned int physt_clk2_term_en : 1;   /* [13]  */
        unsigned int reserved_0 : 18;          /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st3_link;

/* Define the union u_phy_st4_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_rg_eq_clk : 4;   /* [3..0]  */
        unsigned int physt_rg_eq_clk2 : 4;  /* [7..4]  */
        unsigned int physt_rg_eq_lane0 : 4; /* [11..8]  */
        unsigned int physt_rg_eq_lane1 : 4; /* [15..12]  */
        unsigned int physt_rg_eq_lane2 : 4; /* [19..16]  */
        unsigned int physt_rg_eq_lane3 : 4; /* [23..20]  */
        unsigned int reserved_0 : 8;        /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st4_link;

/* Define the union u_phy_st5_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_da_ulpb_ck : 1;   /* [0]  */
        unsigned int physt_da_ulpb_ck2 : 1;  /* [1]  */
        unsigned int physt_da_idlb_clk : 1;  /* [2]  */
        unsigned int physt_da_idlb_clk2 : 1; /* [3]  */
        unsigned int physt_da_ulpb : 4;      /* [7..4]  */
        unsigned int physt_da_idlb : 4;      /* [11..8]  */
        unsigned int physt_rg_test : 4;      /* [15..12]  */
        unsigned int physt_rg_sa_en : 1;     /* [16]  */
        unsigned int reserved_0 : 15;        /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st5_link;

/* Define the union u_phy_st_so_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_ad_so_d0 : 8; /* [7..0]  */
        unsigned int phy_ad_so_d1 : 8; /* [15..8]  */
        unsigned int phy_ad_so_d2 : 8; /* [23..16]  */
        unsigned int phy_ad_so_d3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st_so_link;

/* Define the union u_cil_fsm1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cyc_data_lp00 : 6;     /* [5..0]  */
        unsigned int reserved_0 : 2;            /* [7..6]  */
        unsigned int cil_cyc_data_hs0 : 6;      /* [13..8]  */
        unsigned int reserved_1 : 2;            /* [15..14]  */
        unsigned int cil_cyc_data_hs_exit : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2;            /* [23..22]  */
        unsigned int cil_cyc_data_hs_trail : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;            /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm1_link;

/* Define the union u_skew_cal_lepgth0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int skew_cal_length0 : 16; /* [15..0]  */
        unsigned int skew_cal_length1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_skew_cal_lepgth0_link;

/* Define the union u_skew_cal_lepgth1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int skew_cal_length0 : 16; /* [15..0]  */
        unsigned int skew_cal_length1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_skew_cal_lepgth1_link;

/* Define the union u_phy_ext_reg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rg_en_idac_cal_ov : 1;   /* [0]  */
        unsigned int da_kofs_dphy_en : 1;     /* [1]  */
        unsigned int rg_cmp_mod : 1;          /* [2]  */
        unsigned int rg_clk_modeck : 1;       /* [3]  */
        unsigned int phy_rg_lane0_si : 2;     /* [5..4]  */
        unsigned int phy_rg_lane1_si : 2;     /* [7..6]  */
        unsigned int phy_rg_lane2_si : 2;     /* [9..8]  */
        unsigned int phy_rg_lane3_si : 2;     /* [11..10]  */
        unsigned int phy_rg_eq_lane0 : 3;     /* [14..12]  */
        unsigned int phy_rg_eq_lane1 : 3;     /* [17..15]  */
        unsigned int phy_rg_eq_lane2 : 3;     /* [20..18]  */
        unsigned int phy_rg_eq_lane3 : 3;     /* [23..21]  */
        unsigned int phy_rg_eq_clk : 3;       /* [26..24]  */
        unsigned int phy_rg_eq_clk2 : 3;      /* [29..27]  */
        unsigned int reserved_0 : 2;          /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_ext_reg0;

/* Define the union u_phy_ext_reg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rg_clk_mode : 4;         /* [0..3]  */
        unsigned int rg_idac_cal_in : 6;      /* [9..4]  */
        unsigned int rg_eq_test : 8;          /* [17..10]  */
        unsigned int rg_ad12 : 4;             /* [21..18]  */
        unsigned int rg_bg_test_mipi : 8;     /* [29..22]  */
        unsigned int reserved_0 : 2;          /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_ext_reg1;

/* Define the union u_phy_test */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_test_top : 16;   /* [0..15]  */
        unsigned int phy_rg_test : 12;       /* [27..16]  */
        unsigned int reserved_0 : 4;         /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_test;

/* Define the union u_phy_dsw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_lane0_dsw : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2;        /* [7..6]  */
        unsigned int phy_rg_lane1_dsw : 6;  /* [13..8]  */
        unsigned int reserved_1 : 2;        /* [15..14]  */
        unsigned int phy_rg_lane2_dsw : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2;        /* [23..22]  */
        unsigned int phy_rg_lane3_dsw : 6;  /* [29..24]  */
        unsigned int reserved_3 : 2;        /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_dsw;

/* Define the union u_phy_offnn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_lane0_offnn : 4; /* [3..0]  */
        unsigned int phy_da_lane1_offnn : 4; /* [7..4]  */
        unsigned int phy_da_lane2_offnn : 4; /* [11..8]  */
        unsigned int phy_da_lane3_offnn : 4; /* [15..12]  */
        unsigned int phy_da_clk_offnn : 4;   /* [19..16]  */
        unsigned int phy_da_clk2_offnn : 4;  /* [23..20]  */
        unsigned int reserved_0 : 8;         /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_offnn;

/* Define the union u_phy_temrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_terml_lane0 : 4; /* [3..0]  */
        unsigned int phy_rg_terml_lane1 : 4; /* [7..4]  */
        unsigned int phy_rg_terml_lane2 : 4; /* [11..8]  */
        unsigned int phy_rg_terml_lane3 : 4; /* [15..12]  */
        unsigned int phy_rg_terml_clk : 4;   /* [19..16]  */
        unsigned int phy_rg_terml_clk2 : 4;  /* [23..20]  */
        unsigned int reserved_0 : 8;         /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_temrl;

/* Define the union u_freq_measure */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cnt : 16; /* [15..0]  */
        unsigned int hs_cnt : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_freq_measure;

/* Define the union u_mipi_cil_int_raw_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_escape_d0_raw : 1;  /* [0]  */
        unsigned int err_escape_d1_raw : 1;  /* [1]  */
        unsigned int err_escape_d2_raw : 1;  /* [2]  */
        unsigned int err_escape_d3_raw : 1;  /* [3]  */
        unsigned int err_escape_ck_raw : 1;  /* [4]  */
        unsigned int reserved_0 : 3;         /* [7..5]  */
        unsigned int err_timeout_d0_raw : 1; /* [8]  */
        unsigned int err_timeout_d1_raw : 1; /* [9]  */
        unsigned int err_timeout_d2_raw : 1; /* [10]  */
        unsigned int err_timeout_d3_raw : 1; /* [11]  */
        unsigned int err_timeout_ck_raw : 1; /* [12]  */
        unsigned int reserved_1 : 3;         /* [15..13]  */
        unsigned int sot_detc_d0_raw : 1;    /* [16]  */
        unsigned int sot_detc_d1_raw : 1;    /* [17]  */
        unsigned int sot_detc_d2_raw : 1;    /* [18]  */
        unsigned int sot_detc_d3_raw : 1;    /* [19]  */
        unsigned int reserved_2 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_cil_int_raw_link;

/* Define the union u_mipi_cil_int_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_escape_d0_st : 1;  /* [0]  */
        unsigned int err_escape_d1_st : 1;  /* [1]  */
        unsigned int err_escape_d2_st : 1;  /* [2]  */
        unsigned int err_escape_d3_st : 1;  /* [3]  */
        unsigned int err_escape_ck_st : 1;  /* [4]  */
        unsigned int reserved_0 : 3;        /* [7..5]  */
        unsigned int err_timeout_d0_st : 1; /* [8]  */
        unsigned int err_timeout_d1_st : 1; /* [9]  */
        unsigned int err_timeout_d2_st : 1; /* [10]  */
        unsigned int err_timeout_d3_st : 1; /* [11]  */
        unsigned int err_timeout_ck_st : 1; /* [12]  */
        unsigned int reserved_1 : 3;        /* [15..13]  */
        unsigned int sot_detc_d0_st : 1;    /* [16]  */
        unsigned int sot_detc_d1_st : 1;    /* [17]  */
        unsigned int sot_detc_d2_st : 1;    /* [18]  */
        unsigned int sot_detc_d3_st : 1;    /* [19]  */
        unsigned int reserved_2 : 12;       /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_cil_int_link;

/* Define the union u_mipi_cil_int_msk_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_escape_d0_msk : 1;  /* [0]  */
        unsigned int err_escape_d1_msk : 1;  /* [1]  */
        unsigned int err_escape_d2_msk : 1;  /* [2]  */
        unsigned int err_escape_d3_msk : 1;  /* [3]  */
        unsigned int err_escape_ck_msk : 1;  /* [4]  */
        unsigned int reserved_0 : 3;         /* [7..5]  */
        unsigned int err_timeout_d0_msk : 1; /* [8]  */
        unsigned int err_timeout_d1_msk : 1; /* [9]  */
        unsigned int err_timeout_d2_msk : 1; /* [10]  */
        unsigned int err_timeout_d3_msk : 1; /* [11]  */
        unsigned int err_timeout_ck_msk : 1; /* [12]  */
        unsigned int reserved_1 : 3;         /* [15..13]  */
        unsigned int sot_detc_d0_msk : 1;    /* [16]  */
        unsigned int sot_detc_d1_msk : 1;    /* [17]  */
        unsigned int sot_detc_d2_msk : 1;    /* [18]  */
        unsigned int sot_detc_d3_msk : 1;    /* [19]  */
        unsigned int reserved_2 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_cil_int_msk_link;

/* Define the union u_hs_mode_select */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hs_mode : 4;     /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hs_mode_select;

/* Define the union u_d_p_swap */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy0_d0_swap : 1;  /* [0]  */
        unsigned int phy0_d1_swap : 1;  /* [1]  */
        unsigned int phy0_d2_swap : 1;  /* [2]  */
        unsigned int phy0_d3_swap : 1;  /* [3]  */
        unsigned int phy0_ck_swap : 1;  /* [4]  */
        unsigned int phy0_ck2_swap : 1; /* [5]  */
        unsigned int reserved_0 : 2;    /* [7..6]  */
        unsigned int phy1_d0_swap : 1;  /* [8]  */
        unsigned int phy1_d1_swap : 1;  /* [9]  */
        unsigned int phy1_d2_swap : 1;  /* [10]  */
        unsigned int phy1_d3_swap : 1;  /* [11]  */
        unsigned int phy1_ck_swap : 1;  /* [12]  */
        unsigned int phy1_ck2_swap : 1; /* [13]  */
        unsigned int reserved_1 : 2;    /* [15..14]  */
        unsigned int phy2_d0_swap : 1;  /* [16]  */
        unsigned int phy2_d1_swap : 1;  /* [17]  */
        unsigned int phy2_d2_swap : 1;  /* [18]  */
        unsigned int phy2_d3_swap : 1;  /* [19]  */
        unsigned int phy2_ck_swap : 1;  /* [20]  */
        unsigned int phy2_ck2_swap : 1; /* [21]  */
        unsigned int reserved_2 : 2;    /* [23..22]  */
        unsigned int phy3_d0_swap : 1;  /* [24]  */
        unsigned int phy3_d1_swap : 1;  /* [25]  */
        unsigned int phy3_d2_swap : 1;  /* [26]  */
        unsigned int phy3_d3_swap : 1;  /* [27]  */
        unsigned int phy3_ck_swap : 1;  /* [28]  */
        unsigned int phy3_ck2_swap : 1; /* [29]  */
        unsigned int reserved_3 : 2;    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_d_p_swap;

/* Define the union u_phy_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy0_en : 1;     /* [0]  */
        unsigned int phy1_en : 1;     /* [1]  */
        unsigned int phy2_en : 1;     /* [2]  */
        unsigned int phy3_en : 1;     /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_en;

/* Define the union u_lane_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_en : 1;    /* [0]  */
        unsigned int lane1_en : 1;    /* [1]  */
        unsigned int lane2_en : 1;    /* [2]  */
        unsigned int lane3_en : 1;    /* [3]  */
        unsigned int lane4_en : 1;    /* [4]  */
        unsigned int lane5_en : 1;    /* [5]  */
        unsigned int lane6_en : 1;    /* [6]  */
        unsigned int lane7_en : 1;    /* [7]  */
        unsigned int lane8_en : 1;    /* [8]  */
        unsigned int lane9_en : 1;    /* [9]  */
        unsigned int lane10_en : 1;   /* [10]  */
        unsigned int lane11_en : 1;   /* [11]  */
        unsigned int lane12_en : 1;   /* [12]  */
        unsigned int lane13_en : 1;   /* [13]  */
        unsigned int lane14_en : 1;   /* [14]  */
        unsigned int lane15_en : 1;   /* [15]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_en;

/* Define the union u_phy_cil_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phycil0_cken : 1; /* [0]  */
        unsigned int phycil1_cken : 1; /* [1]  */
        unsigned int phycil2_cken : 1; /* [2]  */
        unsigned int phycil3_cken : 1; /* [3]  */
        unsigned int reserved_0 : 4;   /* [7..4]  */
        unsigned int cil0_rst_req : 1; /* [8]  */
        unsigned int cil1_rst_req : 1; /* [9]  */
        unsigned int cil2_rst_req : 1; /* [10]  */
        unsigned int cil3_rst_req : 1; /* [11]  */
        unsigned int cil4_rst_req : 1; /* [12]  */
        unsigned int cil5_rst_req : 1; /* [13]  */
        unsigned int cil6_rst_req : 1; /* [14]  */
        unsigned int cil7_rst_req : 1; /* [15]  */
        unsigned int reserved_1 : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cil_ctrl;

/* Define the union u_phy_lp_select */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy1_lp_ck_sel : 1; /* [0]  */
        unsigned int phy3_lp_ck_sel : 1; /* [1]  */
        unsigned int reserved_0 : 2;     /* [3..2]  */
        unsigned int pix2_ck_sel : 1;    /* [4]  */
        unsigned int pix6_ck_sel : 1;    /* [5]  */
        unsigned int reserved_1 : 26;    /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_lp_select;

/* Define the union u_phycfg_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phycil0_0_cfg_mode : 3;   /* [2..0]  */
        unsigned int phycil0_cfg_mode_sel : 1; /* [3]  */
        unsigned int phycil0_1_cfg_mode : 3;   /* [6..4]  */
        unsigned int reserved_0 : 1;           /* [7]  */
        unsigned int phycil1_0_cfg_mode : 3;   /* [10..8]  */
        unsigned int phycil1_cfg_mode_sel : 1; /* [11]  */
        unsigned int phycil1_1_cfg_mode : 3;   /* [14..12]  */
        unsigned int reserved_1 : 1;           /* [15]  */
        unsigned int phycil2_0_cfg_mode : 3;   /* [18..16]  */
        unsigned int phycil2_cfg_mode_sel : 1; /* [19]  */
        unsigned int phycil2_1_cfg_mode : 3;   /* [22..20]  */
        unsigned int reserved_2 : 1;           /* [23]  */
        unsigned int phycil3_0_cfg_mode : 3;   /* [26..24]  */
        unsigned int phycil3_cfg_mode_sel : 1; /* [27]  */
        unsigned int phycil3_1_cfg_mode : 3;   /* [30..28]  */
        unsigned int reserved_3 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phycfg_mode;

/* Define the union u_phycfg_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phycil0_cfg_en : 1; /* [0]  */
        unsigned int phycil1_cfg_en : 1; /* [1]  */
        unsigned int phycil2_cfg_en : 1; /* [2]  */
        unsigned int phycil3_cfg_en : 1; /* [3]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phycfg_en;

/* Define the union u_chn0_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn0_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn0_mem_rma : 4;    /* [7..4]  */
        unsigned int chn0_mem_test1a : 1; /* [8]  */
        unsigned int chn0_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn0_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn0_mem_test1b : 1; /* [16]  */
        unsigned int chn0_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn0_mem_ctrl;

/* Define the union u_chn0_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn0_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn0_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn0_clr_en;

/* Define the union u_chn1_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn1_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn1_mem_rma : 4;    /* [7..4]  */
        unsigned int chn1_mem_test1a : 1; /* [8]  */
        unsigned int chn1_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn1_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn1_mem_test1b : 1; /* [16]  */
        unsigned int chn1_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn1_mem_ctrl;

/* Define the union u_chn1_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn1_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn1_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn1_clr_en;

/* Define the union u_chn2_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn2_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn2_mem_rma : 4;    /* [7..4]  */
        unsigned int chn2_mem_test1a : 1; /* [8]  */
        unsigned int chn2_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn2_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn2_mem_test1b : 1; /* [16]  */
        unsigned int chn2_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn2_mem_ctrl;

/* Define the union u_chn2_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn2_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn2_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn2_clr_en;

/* Define the union u_chn3_mem_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn3_mem_ck_gt : 1;  /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int chn3_mem_rma : 4;    /* [7..4]  */
        unsigned int chn3_mem_test1a : 1; /* [8]  */
        unsigned int chn3_mem_rmea : 1;   /* [9]  */
        unsigned int reserved_1 : 2;      /* [11..10]  */
        unsigned int chn3_mem_rmb : 4;    /* [15..12]  */
        unsigned int chn3_mem_test1b : 1; /* [16]  */
        unsigned int chn3_mem_rmeb : 1;   /* [17]  */
        unsigned int reserved_2 : 14;     /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn3_mem_ctrl;

/* Define the union u_chn3_clr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chn3_clr_en_lvds : 1;  /* [0]  */
        unsigned int chn3_clr_en_align : 1; /* [1]  */
        unsigned int reserved_0 : 30;       /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn3_clr_en;

/* Define the union u_mipi_test_phy */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy_en : 1;       /* [0]  */
        unsigned int reserved_0 : 3;        /* [3..1]  */
        unsigned int test_phy_srst_req : 1; /* [4]  */
        unsigned int reserved_1 : 27;       /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_test_phy;

/* Define the union u_colorbar_ctrl_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_en : 1;        /* [0]  */
        unsigned int colorbar_mode : 1;      /* [1]  */
        unsigned int colorbar_pattern : 1;   /* [2]  */
        unsigned int colorbar_line_init : 1; /* [3]  */
        unsigned int colorbar_width : 10;    /* [13..4]  */
        unsigned int reserved_0 : 18;        /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_ctrl_link0;

/* Define the union u_colorbar_init_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_data_init : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;          /* [15..12]  */
        unsigned int colorbar_data_inc : 12;  /* [27..16]  */
        unsigned int reserved_1 : 4;          /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_init_link0;

/* Define the union u_colorbar_v_blk_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_vertical_blk : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;            /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_v_blk_link0;

/* Define the union u_colorbar_h_blk_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_line_blk : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_h_blk_link0;

/* Define the union u_colorbar_img_size_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_imgwidth : 16;  /* [15..0]  */
        unsigned int colorbar_imgheight : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_img_size_link0;

/* Define the union u_colorbar_sync_code0_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_sof : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;    /* [15..12]  */
        unsigned int colorbar_eof : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_sync_code0_link0;

/* Define the union u_colorbar_sync_code1_link0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int colorbar_sol : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;    /* [15..12]  */
        unsigned int colorbar_eol : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_colorbar_sync_code1_link0;

/* Define the union u_phy_cal_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_cal_init0 : 1; /* [0]  */
        unsigned int phy_cal_init1 : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cal_int;

/* Define the union u_phy0_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy0_resault : 4; /* [3..0]  */
        unsigned int test_phy0_compare : 4; /* [7..4]  */
        unsigned int test_phy0_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy0_test_out;

/* Define the union u_phy1_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy1_resault : 4; /* [3..0]  */
        unsigned int test_phy1_compare : 4; /* [7..4]  */
        unsigned int test_phy1_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy1_test_out;

/* Define the union u_phy2_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy2_resault : 4; /* [3..0]  */
        unsigned int test_phy2_compare : 4; /* [7..4]  */
        unsigned int test_phy2_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy2_test_out;

/* Define the union u_phy3_test_out */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_phy3_resault : 4; /* [3..0]  */
        unsigned int test_phy3_compare : 4; /* [7..4]  */
        unsigned int test_phy3_done : 4;    /* [11..8]  */
        unsigned int reserved_0 : 20;       /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy3_test_out;

/* Define the union u_lpdt_ph_addr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int lpdt_packet_header : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lpdt_ph_addr;
/* Define the union u_lpdt_check_res_addr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lpdt_crc_err : 1; /* [0]  */
        unsigned int lpdt_ecc_err : 1; /* [1]  */
        unsigned int reserved_0 : 30;  /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lpdt_check_res_addr;

/* Define the union u_mipi_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_phycil0_raw : 1; /* [0]  */
        unsigned int int_phycil1_raw : 1; /* [1]  */
        unsigned int int_phycil2_raw : 1; /* [2]  */
        unsigned int int_phycil3_raw : 1; /* [3]  */
        unsigned int int_chn0_raw : 1;    /* [4]  */
        unsigned int int_chn1_raw : 1;    /* [5]  */
        unsigned int int_chn2_raw : 1;    /* [6]  */
        unsigned int int_chn3_raw : 1;    /* [7]  */
        unsigned int int_chn4_raw : 1;    /* [8]  */
        unsigned int int_chn5_raw : 1;    /* [9]  */
        unsigned int int_chn6_raw : 1;    /* [10]  */
        unsigned int int_chn7_raw : 1;    /* [11]  */
        unsigned int reserved_0 : 20;     /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_int_raw;

/* Define the union u_mipi_int_st */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_phycil0_st : 1; /* [0]  */
        unsigned int int_phycil1_st : 1; /* [1]  */
        unsigned int int_phycil2_st : 1; /* [2]  */
        unsigned int int_phycil3_st : 1; /* [3]  */
        unsigned int int_chn0_st : 1;    /* [4]  */
        unsigned int int_chn1_st : 1;    /* [5]  */
        unsigned int int_chn2_st : 1;    /* [6]  */
        unsigned int int_chn3_st : 1;    /* [7]  */
        unsigned int int_chn4_st : 1;    /* [8]  */
        unsigned int int_chn5_st : 1;    /* [9]  */
        unsigned int int_chn6_st : 1;    /* [10]  */
        unsigned int int_chn7_st : 1;    /* [11]  */
        unsigned int reserved_0 : 20;    /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_int_st;

/* Define the union u_mipi_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_phycil0_mask : 1; /* [0]  */
        unsigned int int_phycil1_mask : 1; /* [1]  */
        unsigned int int_phycil2_mask : 1; /* [2]  */
        unsigned int int_phycil3_mask : 1; /* [3]  */
        unsigned int int_chn0_mask : 1;    /* [4]  */
        unsigned int int_chn1_mask : 1;    /* [5]  */
        unsigned int int_chn2_mask : 1;    /* [6]  */
        unsigned int int_chn3_mask : 1;    /* [7]  */
        unsigned int int_chn4_mask : 1;    /* [8]  */
        unsigned int int_chn5_mask : 1;    /* [9]  */
        unsigned int int_chn6_mask : 1;    /* [10]  */
        unsigned int int_chn7_mask : 1;    /* [11]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_int_msk;

/* Define the union u_mipi_interlaced */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_csi_interlaced : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_interlaced;

/* Define the union u_mipi_frame_num0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_frame_num_vc0 : 16; /* [15..0]  */
        unsigned int mipi_frame_num_vc1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_frame_num0;

/* Define the union u_mipi_frame_num1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_frame_num_vc2 : 16; /* [15..0]  */
        unsigned int mipi_frame_num_vc3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_frame_num1;

/* Define the union u_mipi_crc_intr_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crc_err_vc0_raw : 1;                /* [0]  */
        unsigned int crc_err_vc1_raw : 1;                /* [1]  */
        unsigned int crc_err_vc2_raw : 1;                /* [2]  */
        unsigned int crc_err_vc3_raw : 1;                /* [3]  */
        unsigned int ecc_err_vc0_raw : 1;                /* [4]  */
        unsigned int ecc_err_vc1_raw : 1;                /* [5]  */
        unsigned int ecc_err_vc2_raw : 1;                /* [6]  */
        unsigned int ecc_err_vc3_raw : 1;                /* [7]  */
        unsigned int ecc_err_mult_raw : 1;               /* [8]  */
        unsigned int reserved_0 : 7;                     /* [15..9]  */
        unsigned int frame_num_err_vc0_raw : 1;          /* [16]  */
        unsigned int frame_num_err_vc1_raw : 1;          /* [17]  */
        unsigned int frame_num_err_vc2_raw : 1;          /* [18]  */
        unsigned int frame_num_err_vc3_raw : 1;          /* [19]  */
        unsigned int frame_s_e_num_mismatch_vc0_raw : 1; /* [20]  */
        unsigned int frame_s_e_num_mismatch_vc1_raw : 1; /* [21]  */
        unsigned int frame_s_e_num_mismatch_vc2_raw : 1; /* [22]  */
        unsigned int frame_s_e_num_mismatch_vc3_raw : 1; /* [23]  */
        unsigned int reserved_1 : 8;                     /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crc_intr_raw;

/* Define the union u_mipi_crc_intr_st */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crc_err_vc0_st : 1;                /* [0]  */
        unsigned int crc_err_vc1_st : 1;                /* [1]  */
        unsigned int crc_err_vc2_st : 1;                /* [2]  */
        unsigned int crc_err_vc3_st : 1;                /* [3]  */
        unsigned int ecc_err_vc0_st : 1;                /* [4]  */
        unsigned int ecc_err_vc1_st : 1;                /* [5]  */
        unsigned int ecc_err_vc2_st : 1;                /* [6]  */
        unsigned int ecc_err_vc3_st : 1;                /* [7]  */
        unsigned int ecc_err_mult_st : 1;               /* [8]  */
        unsigned int reserved_0 : 7;                    /* [15..9]  */
        unsigned int frame_num_err_vc0_st : 1;          /* [16]  */
        unsigned int frame_num_err_vc1_st : 1;          /* [17]  */
        unsigned int frame_num_err_vc2_st : 1;          /* [18]  */
        unsigned int frame_num_err_vc3_st : 1;          /* [19]  */
        unsigned int frame_s_e_num_mismatch_vc0_st : 1; /* [20]  */
        unsigned int frame_s_e_num_mismatch_vc1_st : 1; /* [21]  */
        unsigned int frame_s_e_num_mismatch_vc2_st : 1; /* [22]  */
        unsigned int frame_s_e_num_mismatch_vc3_st : 1; /* [23]  */
        unsigned int reserved_1 : 8;                    /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crc_intr_st;

/* Define the union u_mipi_crc_intr_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crc_err_vc0_msk : 1;                /* [0]  */
        unsigned int crc_err_vc1_msk : 1;                /* [1]  */
        unsigned int crc_err_vc2_msk : 1;                /* [2]  */
        unsigned int crc_err_vc3_msk : 1;                /* [3]  */
        unsigned int ecc_err_vc0_msk : 1;                /* [4]  */
        unsigned int ecc_err_vc1_msk : 1;                /* [5]  */
        unsigned int ecc_err_vc2_msk : 1;                /* [6]  */
        unsigned int ecc_err_vc3_msk : 1;                /* [7]  */
        unsigned int ecc_err_mult_msk : 1;               /* [8]  */
        unsigned int reserved_0 : 7;                     /* [15..9]  */
        unsigned int frame_num_err_vc0_msk : 1;          /* [16]  */
        unsigned int frame_num_err_vc1_msk : 1;          /* [17]  */
        unsigned int frame_num_err_vc2_msk : 1;          /* [18]  */
        unsigned int frame_num_err_vc3_msk : 1;          /* [19]  */
        unsigned int frame_s_e_num_mismatch_vc0_msk : 1; /* [20]  */
        unsigned int frame_s_e_num_mismatch_vc1_msk : 1; /* [21]  */
        unsigned int frame_s_e_num_mismatch_vc2_msk : 1; /* [22]  */
        unsigned int frame_s_e_num_mismatch_vc3_msk : 1; /* [23]  */
        unsigned int reserved_1 : 8;                     /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crc_intr_msk;

/* Define the union u_mipi_userdef_dt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int user_def0_dt : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;   /* [3]  */
        unsigned int user_def1_dt : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;   /* [7]  */
        unsigned int user_def2_dt : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;   /* [11]  */
        unsigned int user_def3_dt : 3; /* [14..12]  */
        unsigned int reserved_3 : 17;  /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_userdef_dt;

/* Define the union u_mipi_user_def */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int user_def0 : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int user_def1 : 6;  /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int user_def2 : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int user_def3 : 6;  /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_user_def;

/* Define the union u_mipi_ctrl_mode_hs */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hdr_mode : 1;       /* [0]  */
        unsigned int reserved_0 : 3;     /* [3..1]  */
        unsigned int vc_mode : 1;        /* [4]  */
        unsigned int reserved_1 : 3;     /* [7..5]  */
        unsigned int user_def_en : 1;    /* [8]  */
        unsigned int reserved_2 : 3;     /* [11..9]  */
        unsigned int lane0_location : 3; /* [14..12]  */
        unsigned int reserved_3 : 17;    /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_mode_hs;

/* Define the union u_mipi_vhend_delay */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vend_delay : 16; /* [15..0]  */
        unsigned int hend_delay : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_vhend_delay;

/* Define the union u_mipi_dol_id_code0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int id_code_reg0 : 16; /* [15..0]  */
        unsigned int id_code_reg1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dol_id_code0;

/* Define the union u_mipi_dol_id_code1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int id_code_reg2 : 16; /* [15..0]  */
        unsigned int id_code_reg3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dol_id_code1;

/* Define the union u_mipi_dol_id_code2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int id_code_reg4 : 16; /* [15..0]  */
        unsigned int id_code_reg5 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dol_id_code2;

/* Define the union u_mipi_crop_start_chn0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn0 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn0;

/* Define the union u_mipi_crop_start_chn1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn1 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn1;

/* Define the union u_mipi_crop_start_chn2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn2 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn2;

/* Define the union u_mipi_crop_start_chn3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_start_x_chn3 : 16; /* [15..0]  */
        unsigned int mipi_start_y_chn3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_crop_start_chn3;

/* Define the union u_mipi_imgsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_imgwidth : 16;  /* [15..0]  */
        unsigned int mipi_imgheight : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize;

/* Define the union u_mipi_ctrl_mode_pixel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int crop_en : 1;                  /* [0]  */
        unsigned int mipi_wdr_num : 2;             /* [2..1]  */
        unsigned int memory_hold_en : 1;           /* [3]  */
        unsigned int mipi_dol_mode : 1;            /* [4]  */
        unsigned int reserved_0 : 3;               /* [7..5]  */
        unsigned int rx2mode : 1;                  /* [8]  */
        unsigned int data_merge_mode : 2;          /* [10..9]  */
        unsigned int sync_clear_en : 1;            /* [11]  */
        unsigned int stagger_hdr_mode : 1;         /* [12]  */
        unsigned int stagger_frm_num : 2;          /* [14..13]  */
        unsigned int dummy_line_detect : 1;        /* [15]  */
        unsigned int mipi_double_pix_en : 1;       /* [16]  */
        unsigned int mipi_double_yuv_en : 1;       /* [17]  */
        unsigned int mipi_yuv_422_en : 1;          /* [18]  */
        unsigned int mipi_yuv_420_nolegacy_en : 1; /* [19]  */
        unsigned int mipi_yuv_420_legacy_en : 1;   /* [20]  */
        unsigned int reserved_1 : 11;              /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_mode_pixel;

/* Define the union u_mipi_dummy_pix_reg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dummy_pix_reg : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_dummy_pix_reg;

/* Define the union u_mipi_imgsize0_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc0 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize0_statis;

/* Define the union u_mipi_imgsize1_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc1 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize1_statis;

/* Define the union u_mipi_imgsize2_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc2 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize2_statis;

/* Define the union u_mipi_imgsize3_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int imgwidth_statis_vc3 : 16;  /* [15..0]  */
        unsigned int imgheight_statis_vc3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_imgsize3_statis;

/* Define the union u_mipi_ctrl_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_cfifo_wrerr_raw : 1;      /* [0]  */
        unsigned int int_dfifo_wrerr_raw : 1;      /* [1]  */
        unsigned int reserved_0 : 2;               /* [3..2]  */
        unsigned int int_vsync_raw : 1;            /* [4]  */
        unsigned int reserved_1 : 3;               /* [7..5]  */
        unsigned int memory_noempty_write_raw : 1; /* [8]  */
        unsigned int cmdfifo_noempty_raw : 1;      /* [9]  */
        unsigned int reserved_2 : 6;               /* [15..10]  */
        unsigned int int_cfifo_rderr_raw : 1;      /* [16]  */
        unsigned int int_dfifo_rderr_raw : 1;      /* [17]  */
        unsigned int reserved_3 : 14;              /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_int_raw;

/* Define the union u_mipi_ctrl_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_cfifo_wrerr_st : 1;      /* [0]  */
        unsigned int int_dfifo_wrerr_st : 1;      /* [1]  */
        unsigned int reserved_0 : 2;              /* [3..2]  */
        unsigned int int_vsync_st : 1;            /* [4]  */
        unsigned int reserved_1 : 3;              /* [7..5]  */
        unsigned int memory_noempty_write_st : 1; /* [8]  */
        unsigned int cmdfifo_noempty_st : 1;      /* [9]  */
        unsigned int reserved_2 : 6;              /* [15..10]  */
        unsigned int int_cfifo_rderr_st : 1;      /* [16]  */
        unsigned int int_dfifo_rderr_st : 1;      /* [17]  */
        unsigned int reserved_3 : 14;             /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_int;

/* Define the union u_mipi_ctrl_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_cfifo_wrerr_msk : 1;      /* [0]  */
        unsigned int int_dfifo_wrerr_msk : 1;      /* [1]  */
        unsigned int reserved_0 : 2;               /* [3..2]  */
        unsigned int int_vsync_msk : 1;            /* [4]  */
        unsigned int reserved_1 : 3;               /* [7..5]  */
        unsigned int memory_noempty_write_msk : 1; /* [8]  */
        unsigned int cmdfifo_noempty_msk : 1;      /* [9]  */
        unsigned int reserved_2 : 6;               /* [15..10]  */
        unsigned int int_cfifo_rderr_msk : 1;      /* [16]  */
        unsigned int int_dfifo_rderr_msk : 1;      /* [17]  */
        unsigned int reserved_3 : 14;              /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_ctrl_int_msk;

/* Define the union u_lvds_wdr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_wdr_en : 1;       /* [0]  */
        unsigned int reserved_0 : 3;        /* [3..1]  */
        unsigned int lvds_wdr_num : 2;      /* [5..4]  */
        unsigned int reserved_1 : 2;        /* [7..6]  */
        unsigned int lvds_wdr_mode : 4;     /* [11..8]  */
        unsigned int lvds_wdr_id_shift : 4; /* [15..12]  */
        unsigned int reserved_2 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_wdr;

/* Define the union u_lvds_dolscd_hblk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dol_hblank1 : 16; /* [15..0]  */
        unsigned int dol_hblank2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_dolscd_hblk;

/* Define the union u_lvds_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_sync_mode : 1;       /* [0]  */
        unsigned int reserved_0 : 3;           /* [3..1]  */
        unsigned int lvds_raw_type : 3;        /* [6..4]  */
        unsigned int reserved_1 : 1;           /* [7]  */
        unsigned int lvds_pix_big_endian : 1;  /* [8]  */
        unsigned int lvds_code_big_endian : 1; /* [9]  */
        unsigned int reserved_2 : 2;           /* [11..10]  */
        unsigned int lvds_crop_en : 1;         /* [12]  */
        unsigned int reserved_3 : 3;           /* [15..13]  */
        unsigned int lvds_split_mode : 3;      /* [18..16]  */
        unsigned int reserved_4 : 13;          /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl;

/* Define the union u_lvds_imgsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth_lane : 16; /* [15..0]  */
        unsigned int lvds_imgheight : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize;

/* Define the union u_lvds_crop_start0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x0_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start0;

/* Define the union u_lvds_crop_start1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x1_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y1 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start1;

/* Define the union u_lvds_crop_start2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x2_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y2 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start2;

/* Define the union u_lvds_crop_start3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_start_x3_lane : 16; /* [15..0]  */
        unsigned int lvds_start_y3 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_crop_start3;

/* Define the union u_lvds_lane_sof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sof_0 : 16; /* [15..0]  */
        unsigned int lane_sof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sof_01;

/* Define the union u_lvds_lane_sof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sof_2 : 16; /* [15..0]  */
        unsigned int lane_sof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sof_23;

/* Define the union u_lvds_lane_eof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eof_0 : 16; /* [15..0]  */
        unsigned int lane_eof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eof_01;

/* Define the union u_lvds_lane_eof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eof_2 : 16; /* [15..0]  */
        unsigned int lane_eof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eof_23;

/* Define the union u_lvds_lane_sol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sol_0 : 16; /* [15..0]  */
        unsigned int lane_sol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sol_01;

/* Define the union u_lvds_lane_sol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_sol_2 : 16; /* [15..0]  */
        unsigned int lane_sol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_sol_23;

/* Define the union u_lvds_lane_eol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eol_0 : 16; /* [15..0]  */
        unsigned int lane_eol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eol_01;

/* Define the union u_lvds_lane_eol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_eol_2 : 16; /* [15..0]  */
        unsigned int lane_eol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_eol_23;

/* Define the union u_lvds_lane_nxt_sof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sof_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_sof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sof_01;

/* Define the union u_lvds_lane_nxt_sof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sof_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_sof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sof_23;

/* Define the union u_lvds_lane_nxt_eof_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eof_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_eof_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eof_01;

/* Define the union u_lvds_lane_nxt_eof_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eof_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_eof_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eof_23;

/* Define the union u_lvds_lane_nxt_sol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sol_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_sol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sol_01;

/* Define the union u_lvds_lane_nxt_sol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_sol_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_sol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_sol_23;

/* Define the union u_lvds_lane_nxt_eol_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eol_0 : 16; /* [15..0]  */
        unsigned int lane_nxt_eol_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eol_01;

/* Define the union u_lvds_lane_nxt_eol_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_nxt_eol_2 : 16; /* [15..0]  */
        unsigned int lane_nxt_eol_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_nxt_eol_23;

/* Define the union u_lvds_li_word0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word0_0 : 16; /* [15..0]  */
        unsigned int li_word0_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word0;

/* Define the union u_lvds_li_word1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word1_0 : 16; /* [15..0]  */
        unsigned int li_word1_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word1;

/* Define the union u_lvds_li_word2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word2_0 : 16; /* [15..0]  */
        unsigned int li_word2_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word2;

/* Define the union u_lvds_li_word3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int li_word3_0 : 16; /* [15..0]  */
        unsigned int li_word3_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_li_word3;

/* Define the union u_lvds_sync_bord0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord0_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord0_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord0_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord0_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord0;

/* Define the union u_lvds_sync_bord1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord1_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord1_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord1_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord1_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord1;

/* Define the union u_lvds_sync_bord2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord2_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord2_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord2_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord2_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord2;

/* Define the union u_lvds_sync_bord3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_bord3_lane0 : 4; /* [3..0]  */
        unsigned int sync_bord3_lane1 : 4; /* [7..4]  */
        unsigned int sync_bord3_lane2 : 4; /* [11..8]  */
        unsigned int sync_bord3_lane3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_sync_bord3;

/* Define the union u_lvds_lane_imgsize_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane_imgwidth : 16;  /* [15..0]  */
        unsigned int lane_imgheight : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_imgsize_statis;

/* Define the union u_lvds_imgsize0_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth0 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize0_statis;

/* Define the union u_lvds_imgsize1_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth1 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize1_statis;

/* Define the union u_lvds_imgsize2_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth2 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize2_statis;

/* Define the union u_lvds_imgsize3_statis */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_imgwidth3 : 16;  /* [15..0]  */
        unsigned int lvds_imgheight3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_imgsize3_statis;

/* Define the union u_lvds_lane_orders0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane0_id : 4; /* [3..0]  */
        unsigned int lvds_lane1_id : 4; /* [7..4]  */
        unsigned int lvds_lane2_id : 4; /* [11..8]  */
        unsigned int lvds_lane3_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders0;

/* Define the union u_lvds_lane_orders1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane4_id : 4; /* [3..0]  */
        unsigned int lvds_lane5_id : 4; /* [7..4]  */
        unsigned int lvds_lane6_id : 4; /* [11..8]  */
        unsigned int lvds_lane7_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders1;

/* Define the union u_lvds_lane_orders2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane8_id : 4;  /* [3..0]  */
        unsigned int lvds_lane9_id : 4;  /* [7..4]  */
        unsigned int lvds_lane10_id : 4; /* [11..8]  */
        unsigned int lvds_lane11_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders2;

/* Define the union u_lvds_lane_orders3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_lane12_id : 4; /* [3..0]  */
        unsigned int lvds_lane13_id : 4; /* [7..4]  */
        unsigned int lvds_lane14_id : 4; /* [11..8]  */
        unsigned int lvds_lane15_id : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_lane_orders3;

/* Define the union u_lvds_output_pix_num */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_double_pix_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;        /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_output_pix_num;

/* Define the union u_lvds_f1_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lvds_f1_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;    /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_f1_en;

/* Define the union u_lvds_ctrl_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_sync_err_raw : 1;  /* [0]  */
        unsigned int lane1_sync_err_raw : 1;  /* [1]  */
        unsigned int lane2_sync_err_raw : 1;  /* [2]  */
        unsigned int lane3_sync_err_raw : 1;  /* [3]  */
        unsigned int lane4_sync_err_raw : 1;  /* [4]  */
        unsigned int lane5_sync_err_raw : 1;  /* [5]  */
        unsigned int lane6_sync_err_raw : 1;  /* [6]  */
        unsigned int lane7_sync_err_raw : 1;  /* [7]  */
        unsigned int lane8_sync_err_raw : 1;  /* [8]  */
        unsigned int lane9_sync_err_raw : 1;  /* [9]  */
        unsigned int lane10_sync_err_raw : 1; /* [10]  */
        unsigned int lane11_sync_err_raw : 1; /* [11]  */
        unsigned int lane12_sync_err_raw : 1; /* [12]  */
        unsigned int lane13_sync_err_raw : 1; /* [13]  */
        unsigned int lane14_sync_err_raw : 1; /* [14]  */
        unsigned int lane15_sync_err_raw : 1; /* [15]  */
        unsigned int link0_wr_err_raw : 1;    /* [16]  */
        unsigned int link1_wr_err_raw : 1;    /* [17]  */
        unsigned int link2_wr_err_raw : 1;    /* [18]  */
        unsigned int link3_wr_err_raw : 1;    /* [19]  */
        unsigned int link0_rd_err_raw : 1;    /* [20]  */
        unsigned int link1_rd_err_raw : 1;    /* [21]  */
        unsigned int link2_rd_err_raw : 1;    /* [22]  */
        unsigned int link3_rd_err_raw : 1;    /* [23]  */
        unsigned int lvds_state_err_raw : 1;  /* [24]  */
        unsigned int reserved_0 : 3;          /* [27..25]  */
        unsigned int lvds_vsync_raw : 1;      /* [28]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl_int_raw;

/* Define the union u_lvds_ctrl_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_sync_err_st : 1;  /* [0]  */
        unsigned int lane1_sync_err_st : 1;  /* [1]  */
        unsigned int lane2_sync_err_st : 1;  /* [2]  */
        unsigned int lane3_sync_err_st : 1;  /* [3]  */
        unsigned int lane4_sync_err_st : 1;  /* [4]  */
        unsigned int lane5_sync_err_st : 1;  /* [5]  */
        unsigned int lane6_sync_err_st : 1;  /* [6]  */
        unsigned int lane7_sync_err_st : 1;  /* [7]  */
        unsigned int lane8_sync_err_st : 1;  /* [8]  */
        unsigned int lane9_sync_err_st : 1;  /* [9]  */
        unsigned int lane10_sync_err_st : 1; /* [10]  */
        unsigned int lane11_sync_err_st : 1; /* [11]  */
        unsigned int lane12_sync_err_st : 1; /* [12]  */
        unsigned int lane13_sync_err_st : 1; /* [13]  */
        unsigned int lane14_sync_err_st : 1; /* [14]  */
        unsigned int lane15_sync_err_st : 1; /* [15]  */
        unsigned int link0_wr_err_st : 1;    /* [16]  */
        unsigned int link1_wr_err_st : 1;    /* [17]  */
        unsigned int link2_wr_err_st : 1;    /* [18]  */
        unsigned int link3_wr_err_st : 1;    /* [19]  */
        unsigned int link0_rd_err_st : 1;    /* [20]  */
        unsigned int link1_rd_err_st : 1;    /* [21]  */
        unsigned int link2_rd_err_st : 1;    /* [22]  */
        unsigned int link3_rd_err_st : 1;    /* [23]  */
        unsigned int lvds_state_err_st : 1;  /* [24]  */
        unsigned int reserved_0 : 3;         /* [27..25]  */
        unsigned int lvds_vsync_st : 1;      /* [28]  */
        unsigned int reserved_1 : 3;         /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl_int;

/* Define the union u_lvds_ctrl_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_sync_err_msk : 1;  /* [0]  */
        unsigned int lane1_sync_err_msk : 1;  /* [1]  */
        unsigned int lane2_sync_err_msk : 1;  /* [2]  */
        unsigned int lane3_sync_err_msk : 1;  /* [3]  */
        unsigned int lane4_sync_err_msk : 1;  /* [4]  */
        unsigned int lane5_sync_err_msk : 1;  /* [5]  */
        unsigned int lane6_sync_err_msk : 1;  /* [6]  */
        unsigned int lane7_sync_err_msk : 1;  /* [7]  */
        unsigned int lane8_sync_err_msk : 1;  /* [8]  */
        unsigned int lane9_sync_err_msk : 1;  /* [9]  */
        unsigned int lane10_sync_err_msk : 1; /* [10]  */
        unsigned int lane11_sync_err_msk : 1; /* [11]  */
        unsigned int lane12_sync_err_msk : 1; /* [12]  */
        unsigned int lane13_sync_err_msk : 1; /* [13]  */
        unsigned int lane14_sync_err_msk : 1; /* [14]  */
        unsigned int lane15_sync_err_msk : 1; /* [15]  */
        unsigned int link0_wr_err_msk : 1;    /* [16]  */
        unsigned int link1_wr_err_msk : 1;    /* [17]  */
        unsigned int link2_wr_err_msk : 1;    /* [18]  */
        unsigned int link3_wr_err_msk : 1;    /* [19]  */
        unsigned int link0_rd_err_msk : 1;    /* [20]  */
        unsigned int link1_rd_err_msk : 1;    /* [21]  */
        unsigned int link2_rd_err_msk : 1;    /* [22]  */
        unsigned int link3_rd_err_msk : 1;    /* [23]  */
        unsigned int lvds_state_err_msk : 1;  /* [24]  */
        unsigned int reserved_0 : 3;          /* [27..25]  */
        unsigned int lvds_vsync_msk : 1;      /* [28]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lvds_ctrl_int_msk;

/* Define the union u_lane_id0_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_id : 4;    /* [3..0]  */
        unsigned int lane1_id : 4;    /* [7..4]  */
        unsigned int lane2_id : 4;    /* [11..8]  */
        unsigned int lane3_id : 4;    /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id0_chn;

/* Define the union u_lane_id1_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane4_id : 4;    /* [3..0]  */
        unsigned int lane5_id : 4;    /* [7..4]  */
        unsigned int lane6_id : 4;    /* [11..8]  */
        unsigned int lane7_id : 4;    /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id1_chn;

/* Define the union u_lane_id2_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane8_id : 4;    /* [3..0]  */
        unsigned int lane9_id : 4;    /* [7..4]  */
        unsigned int lane10_id : 4;   /* [11..8]  */
        unsigned int lane11_id : 4;   /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id2_chn;

/* Define the union u_lane_id3_chn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane12_id : 4;   /* [3..0]  */
        unsigned int lane13_id : 4;   /* [7..4]  */
        unsigned int lane14_id : 4;   /* [11..8]  */
        unsigned int lane15_id : 4;   /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_id3_chn;

/* Define the union u_align_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_full_raw : 1;   /* [0]  */
        unsigned int err_lane0_raw : 1;  /* [1]  */
        unsigned int err_lane1_raw : 1;  /* [2]  */
        unsigned int err_lane2_raw : 1;  /* [3]  */
        unsigned int err_lane3_raw : 1;  /* [4]  */
        unsigned int err_lane4_raw : 1;  /* [5]  */
        unsigned int err_lane5_raw : 1;  /* [6]  */
        unsigned int err_lane6_raw : 1;  /* [7]  */
        unsigned int err_lane7_raw : 1;  /* [8]  */
        unsigned int err_lane8_raw : 1;  /* [9]  */
        unsigned int err_lane9_raw : 1;  /* [10]  */
        unsigned int err_lane10_raw : 1; /* [11]  */
        unsigned int err_lane11_raw : 1; /* [12]  */
        unsigned int err_lane12_raw : 1; /* [13]  */
        unsigned int err_lane13_raw : 1; /* [14]  */
        unsigned int err_lane14_raw : 1; /* [15]  */
        unsigned int err_lane15_raw : 1; /* [16]  */
        unsigned int reserved_0 : 15;    /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_align_int_raw;

/* Define the union u_align_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_full_st : 1;   /* [0]  */
        unsigned int err_lane0_st : 1;  /* [1]  */
        unsigned int err_lane1_st : 1;  /* [2]  */
        unsigned int err_lane2_st : 1;  /* [3]  */
        unsigned int err_lane3_st : 1;  /* [4]  */
        unsigned int err_lane4_st : 1;  /* [5]  */
        unsigned int err_lane5_st : 1;  /* [6]  */
        unsigned int err_lane6_st : 1;  /* [7]  */
        unsigned int err_lane7_st : 1;  /* [8]  */
        unsigned int err_lane8_st : 1;  /* [9]  */
        unsigned int err_lane9_st : 1;  /* [10]  */
        unsigned int err_lane10_st : 1; /* [11]  */
        unsigned int err_lane11_st : 1; /* [12]  */
        unsigned int err_lane12_st : 1; /* [13]  */
        unsigned int err_lane13_st : 1; /* [14]  */
        unsigned int err_lane14_st : 1; /* [15]  */
        unsigned int err_lane15_st : 1; /* [16]  */
        unsigned int reserved_0 : 15;   /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_align_int;

/* Define the union u_align_int_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_full_mask : 1;   /* [0]  */
        unsigned int err_lane0_mask : 1;  /* [1]  */
        unsigned int err_lane1_mask : 1;  /* [2]  */
        unsigned int err_lane2_mask : 1;  /* [3]  */
        unsigned int err_lane3_mask : 1;  /* [4]  */
        unsigned int err_lane4_mask : 1;  /* [5]  */
        unsigned int err_lane5_mask : 1;  /* [6]  */
        unsigned int err_lane6_mask : 1;  /* [7]  */
        unsigned int err_lane7_mask : 1;  /* [8]  */
        unsigned int err_lane8_mask : 1;  /* [9]  */
        unsigned int err_lane9_mask : 1;  /* [10]  */
        unsigned int err_lane10_mask : 1; /* [11]  */
        unsigned int err_lane11_mask : 1; /* [12]  */
        unsigned int err_lane12_mask : 1; /* [13]  */
        unsigned int err_lane13_mask : 1; /* [14]  */
        unsigned int err_lane14_mask : 1; /* [15]  */
        unsigned int err_lane15_mask : 1; /* [16]  */
        unsigned int reserved_0 : 15;     /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_align_int_msk;

/* Define the union u_chn_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_lvds_ctrl_raw : 1;  /* [0]  */
        unsigned int int_mipi_csi_raw : 1;   /* [1]  */
        unsigned int int_mipi_ctrl_raw : 1;  /* [2]  */
        unsigned int int_data_align_raw : 1; /* [3]  */
        unsigned int reserved_0 : 28;        /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn_int_raw;

/* Define the union u_chn_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_lvds_ctrl_st : 1;  /* [0]  */
        unsigned int int_mipi_csi_st : 1;   /* [1]  */
        unsigned int int_mipi_ctrl_st : 1;  /* [2]  */
        unsigned int int_data_align_st : 1; /* [3]  */
        unsigned int reserved_0 : 28;       /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn_int;

/* Define the union u_chn_int_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_lvds_ctrl_mask : 1;  /* [0]  */
        unsigned int int_mipi_csi_mask : 1;   /* [1]  */
        unsigned int int_mipi_ctrl_mask : 1;  /* [2]  */
        unsigned int int_data_align_mask : 1; /* [3]  */
        unsigned int reserved_0 : 28;         /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_chn_int_mask;

// ==============================================================================
/* Define the global struct */
typedef struct {
    u_phy_mode_link phy_mode_link;                        /* 0x0 */
    u_phy_skew_link phy_skew_link;                        /* 0x4 */
    u_phy_lane_si_link phy_lane_si_link;                  /* 0x8 */
    u_phy_en_link phy_en_link;                            /* 0xc */
    u_phy_eq_link phy_eq_link;                            /* 0x10 */
    u_phy_cfg_link phy_cfg_link;                          /* 0x14 */
    u_phy_data_link phy_data_link;                        /* 0x18 */
    u_phy_ph_mipi_link phy_ph_mipi_link;                  /* 0x1c */
    u_phy_data_mipi_link phy_data_mipi_link;              /* 0x20 */
    u_phy_sync_dct_link phy_sync_dct_link;                /* 0x24 */
    u_phy_cmos_dir phy_cmos_dir;                          /* 0x28 */
    unsigned int reserved_0;                              /* 0x2c */
    u_phy_sync_sof0_link phy_sync_sof0_link;              /* 0x30 */
    u_phy_sync_sof1_link phy_sync_sof1_link;              /* 0x34 */
    u_phy_sync_sof2_link phy_sync_sof2_link;              /* 0x38 */
    u_phy_sync_sof3_link phy_sync_sof3_link;              /* 0x3c */
    u_phy_out_valid_link phy_out_valid_link;              /* 0x40 */
    u_phy_data_lvds_link phy_data_lvds_link;              /* 0x44 */
    unsigned int reserved_1[2];                           /* 0x48~0x4c */
    u_phy_deskew_cal_link phy_deskew_cal_link;            /* 0x50 */
    u_phy_pix_pum_link phy_pix_pum_link;                  /* 0x54 */
    unsigned int reserved_2[2];                           /* 0x58~0x5c */
    u_phy_sync_sol0_link phy_sync_sol0_link;              /* 0x60 */
    u_phy_sync_sol1_link phy_sync_sol1_link;              /* 0x64 */
    u_phy_sync_sol2_link phy_sync_sol2_link;              /* 0x68 */
    u_phy_sync_sol3_link phy_sync_sol3_link;              /* 0x6c */
    unsigned int reserved_3[36];                          /* 0x70~0xfc */
    u_cil_timeout_link cil_timeout_link;                  /* 0x100 */
    u_cil_fsm0_link cil_fsm0_link;                        /* 0x104 */
    u_cil_fsm_st0_link cil_fsm_st0_link;                  /* 0x108 */
    u_cil_fsm_st1_link cil_fsm_st1_link;                  /* 0x10c */
    u_phy_st0_link phy_st0_link;                          /* 0x110 */
    u_phy_st1_link phy_st1_link;                          /* 0x114 */
    u_phy_st2_link phy_st2_link;                          /* 0x118 */
    u_phy_st3_link phy_st3_link;                          /* 0x11c */
    u_phy_st4_link phy_st4_link;                          /* 0x120 */
    u_phy_st5_link phy_st5_link;                          /* 0x124 */
    u_phy_st_so_link phy_st_so_link;                      /* 0x128 */
    u_cil_fsm1_link cil_fsm1_link;                        /* 0x12c */
    u_skew_cal_lepgth0_link skew_cal_lepgth0_link;        /* 0x130 */
    u_skew_cal_lepgth1_link skew_cal_lepgth1_link;        /* 0x134 */
    unsigned int reserved_4[6];                           /* 0x138~0x14c */
    u_freq_measure freq_measure;                          /* 0x150 */
    unsigned int reserved_5[3];                           /* 0x154~0x15c */
    u_phy_ext_reg0 phy_ext_reg0;                          /* 0x160 */
    u_phy_ext_reg1 phy_ext_reg1;                          /* 0x164 */
    u_phy_test phy_test;                                  /* 0x168 */
    u_phy_dsw phy_dsw;                                    /* 0x16c */
    u_phy_offnn phy_offnn;                                /* 0x170 */
    unsigned int reserved_6[5];                           /* 0x174~0x184 */
    u_phy_temrl phy_temrl;                                /* 0x188 */
    unsigned int reserved_7[25];                          /* 0x18c~0x1ec */
    u_mipi_cil_int_raw_link mipi_cil_int_raw_link;        /* 0x1f0 */
    u_mipi_cil_int_link mipi_cil_int_link;                /* 0x1f4 */
    u_mipi_cil_int_msk_link mipi_cil_int_msk_link;        /* 0x1f8 */
    unsigned int reserved_8;                              /* 0x1fc */
} mipi_rx_phy_cfg_t;

typedef struct {
    u_hs_mode_select hs_mode_select;                           /* 0x800 */
    u_d_p_swap d_p_swap;                                       /* 0x804 */
    u_phy_en phy_en;                                           /* 0x808 */
    u_lane_en lane_en;                                         /* 0x80c */
    u_phy_cil_ctrl phy_cil_ctrl;                               /* 0x810 */
    u_phy_lp_select phy_lp_select;                             /* 0x814 */
    u_phycfg_mode phycfg_mode;                                 /* 0x818 */
    u_phycfg_en phycfg_en;                                     /* 0x81c */
    u_chn0_mem_ctrl chn0_mem_ctrl;                             /* 0x820 */
    u_chn0_clr_en chn0_clr_en;                                 /* 0x824 */
    u_chn1_mem_ctrl chn1_mem_ctrl;                             /* 0x828 */
    u_chn1_clr_en chn1_clr_en;                                 /* 0x82c */
    u_chn2_mem_ctrl chn2_mem_ctrl;                             /* 0x830 */
    u_chn2_clr_en chn2_clr_en;                                 /* 0x834 */
    u_chn3_mem_ctrl chn3_mem_ctrl;                             /* 0x838 */
    u_chn3_clr_en chn3_clr_en;                                 /* 0x83c */
    unsigned int reserved_0[16];                               /* 0x840~0x87c */
    u_mipi_test_phy mipi_test_phy;                             /* 0x880 */
    unsigned int reserved_1[31];                               /* 0x884~0x8fc */
    u_colorbar_ctrl_link0 colorbar_ctrl_link0;                 /* 0x900 */
    u_colorbar_init_link0 colorbar_init_link0;                 /* 0x904 */
    u_colorbar_v_blk_link0 colorbar_v_blk_link0;               /* 0x908 */
    u_colorbar_h_blk_link0 colorbar_h_blk_link0;               /* 0x90c */
    u_colorbar_img_size_link0 colorbar_img_size_link0;         /* 0x910 */
    u_colorbar_sync_code0_link0 colorbar_sync_code0_link0;     /* 0x914 */
    u_colorbar_sync_code1_link0 colorbar_sync_code1_link0;     /* 0x918 */
    unsigned int reserved_2;                                   /* 0x91c */
    u_phy_cal_int phy_cal_int;                                 /* 0x920 */
    unsigned int reserved_3[55];                               /* 0x924~0x9fc */
    u_phy0_test_out phy0_test_out;                             /* 0xa00 */
    u_phy1_test_out phy1_test_out;                             /* 0xa04 */
    u_phy2_test_out phy2_test_out;                             /* 0xa08 */
    u_phy3_test_out phy3_test_out;                             /* 0xa0c */
    unsigned int reserved_4[60];                               /* 0xa10~0xafc */
    u_lpdt_ph_addr lpdt_ph_addr;                               /* 0xb00 */
    u_lpdt_check_res_addr lpdt_check_res_addr;                 /* 0xb04 */
    unsigned int reserved_5[314];                              /* 0xb08~0xfec */
    u_mipi_int_raw mipi_int_raw;                               /* 0xff0 */
    u_mipi_int_st mipi_int_st;                                 /* 0xff4 */
    u_mipi_int_msk mipi_int_msk;                               /* 0xff8 */
    unsigned int reserved_6;                                   /* 0xffc */
} mipi_rx_sys_regs_t;

typedef struct {
    u_mipi_interlaced mipi_interlaced;                   /* 0x1000 */
    unsigned int reserved_1[3];                          /* 0x1004~0x100c */
    u_mipi_frame_num0 mipi_frame_num0;                   /* 0x1010 */
    u_mipi_frame_num1 mipi_frame_num1;                   /* 0x1014 */
    unsigned int reserved_2[2];                          /* 0x1018~0x101c */
    u_mipi_crc_intr_raw mipi_crc_intr_raw;               /* 0x1020 */
    u_mipi_crc_intr_st mipi_crc_intr_st;                 /* 0x1024 */
    u_mipi_crc_intr_msk mipi_crc_intr_msk;               /* 0x1028 */
    unsigned int reserved_3[53];                         /* 0x102c~0x10fc */
    u_mipi_userdef_dt mipi_userdef_dt;                   /* 0x1100 */
    u_mipi_user_def mipi_user_def;                       /* 0x1104 */
    u_mipi_ctrl_mode_hs mipi_ctrl_mode_hs;               /* 0x1108 */
    u_mipi_vhend_delay mipi_vhend_delay;                 /* 0x110c */
    unsigned int reserved_4[60];                         /* 0x1110~0x11fc */
    u_mipi_dol_id_code0 mipi_dol_id_code0;               /* 0x1200 */
    u_mipi_dol_id_code1 mipi_dol_id_code1;               /* 0x1204 */
    u_mipi_dol_id_code2 mipi_dol_id_code2;               /* 0x1208 */
    unsigned int reserved_5;                             /* 0x120c */
    u_mipi_crop_start_chn0 mipi_crop_start_chn0;         /* 0x1210 */
    u_mipi_crop_start_chn1 mipi_crop_start_chn1;         /* 0x1214 */
    u_mipi_crop_start_chn2 mipi_crop_start_chn2;         /* 0x1218 */
    u_mipi_crop_start_chn3 mipi_crop_start_chn3;         /* 0x121c */
    unsigned int reserved_6;                             /* 0x1220 */
    u_mipi_imgsize mipi_imgsize;                         /* 0x1224 */
    unsigned int reserved_7[2];                          /* 0x1228~0x122c */
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;         /* 0x1230 */
    unsigned int reserved_8[3];                          /* 0x1234~0x123c */
    u_mipi_dummy_pix_reg mipi_dummy_pix_reg;             /* 0x1240 */
    unsigned int reserved_9[3];                          /* 0x1244~0x124c */
    u_mipi_imgsize0_statis mipi_imgsize0_statis;         /* 0x1250 */
    u_mipi_imgsize1_statis mipi_imgsize1_statis;         /* 0x1254 */
    u_mipi_imgsize2_statis mipi_imgsize2_statis;         /* 0x1258 */
    u_mipi_imgsize3_statis mipi_imgsize3_statis;         /* 0x125c */
    unsigned int reserved_10[36];                        /* 0x1260~0x12ec */
    u_mipi_ctrl_int_raw mipi_ctrl_int_raw;               /* 0x12f0 */
    u_mipi_ctrl_int mipi_ctrl_int;                       /* 0x12f4 */
    u_mipi_ctrl_int_msk mipi_ctrl_int_msk;               /* 0x12f8 */
    unsigned int reserved_11;                            /* 0x12fc */
} mipi_ctrl_regs_t;

typedef struct {
    u_lvds_lane_sof_01 lvds_lane_sof_01;                 /* 0x1320 */
    u_lvds_lane_sof_23 lvds_lane_sof_23;                 /* 0x1324 */
    u_lvds_lane_eof_01 lvds_lane_eof_01;                 /* 0x1328 */
    u_lvds_lane_eof_23 lvds_lane_eof_23;                 /* 0x132c */
    u_lvds_lane_sol_01 lvds_lane_sol_01;                 /* 0x1330 */
    u_lvds_lane_sol_23 lvds_lane_sol_23;                 /* 0x1334 */
    u_lvds_lane_eol_01 lvds_lane_eol_01;                 /* 0x1338 */
    u_lvds_lane_eol_23 lvds_lane_eol_23;                 /* 0x133c */
} lvds_sync_code_cfg_t;

typedef struct {
    u_lvds_wdr lvds_wdr;                                          /* 0x1300 */
    u_lvds_dolscd_hblk lvds_dolscd_hblk;                          /* 0x1304 */
    u_lvds_ctrl lvds_ctrl;                                        /* 0x1308 */
    u_lvds_imgsize lvds_imgsize;                                  /* 0x130c */
    u_lvds_crop_start0 lvds_crop_start0;                          /* 0x1310 */
    u_lvds_crop_start1 lvds_crop_start1;                          /* 0x1314 */
    u_lvds_crop_start2 lvds_crop_start2;                          /* 0x1318 */
    u_lvds_crop_start3 lvds_crop_start3;                          /* 0x131c */
    lvds_sync_code_cfg_t lvds_this_frame_sync_code[16];           /* 0x1320~0x151c */
    lvds_sync_code_cfg_t lvds_next_frame_sync_code[16];           /* 0x1520~0x171c */
    u_lvds_li_word0 lvds_li_word0;                                /* 0x1720 */
    u_lvds_li_word1 lvds_li_word1;                                /* 0x1724 */
    u_lvds_li_word2 lvds_li_word2;                                /* 0x1728 */
    u_lvds_li_word3 lvds_li_word3;                                /* 0x172c */
    u_lvds_sync_bord0 lvds_sync_bord0;                            /* 0x1730 */
    u_lvds_sync_bord1 lvds_sync_bord1;                            /* 0x1734 */
    u_lvds_sync_bord2 lvds_sync_bord2;                            /* 0x1738 */
    u_lvds_sync_bord3 lvds_sync_bord3;                            /* 0x173c */
    u_lvds_lane_imgsize_statis lvds_lane_imgsize_statis[16];      /* 0x1740~0x177c */
    u_lvds_imgsize0_statis lvds_imgsize0_statis;                  /* 0x1780 */
    u_lvds_imgsize1_statis lvds_imgsize1_statis;                  /* 0x1784 */
    u_lvds_imgsize2_statis lvds_imgsize2_statis;                  /* 0x1788 */
    u_lvds_imgsize3_statis lvds_imgsize3_statis;                  /* 0x178c */
    u_lvds_lane_orders0 lvds_lane_orders0;                        /* 0x1790 */
    u_lvds_lane_orders1 lvds_lane_orders1;                        /* 0x1794 */
    u_lvds_lane_orders2 lvds_lane_orders2;                        /* 0x1798 */
    u_lvds_lane_orders3 lvds_lane_orders3;                        /* 0x179c */
    u_lvds_output_pix_num lvds_output_pix_num;                    /* 0x17a0 */
    u_lvds_f1_en lvds_f1_en;                              /* 0x17a4 */
    unsigned int reserved_4[18];                                  /* 0x17a8~0x17ec */
    u_lvds_ctrl_int_raw lvds_ctrl_int_raw;                        /* 0x17f0 */
    u_lvds_ctrl_int lvds_ctrl_int;                                /* 0x17f4 */
    u_lvds_ctrl_int_msk lvds_ctrl_int_msk;                        /* 0x17f8 */
    unsigned int reserved_5;                                      /* 0x17fc */
} lvds_ctrl_regs_t;

typedef struct {
    u_lane_id0_chn lane_id0_chn;                         /* 0x1800 */
    u_lane_id1_chn lane_id1_chn;                         /* 0x1804 */
    u_lane_id2_chn lane_id2_chn;                         /* 0x1808 */
    u_lane_id3_chn lane_id3_chn;                         /* 0x180c */
    unsigned int reserved_1[56];                         /* 0x1810~0x18ec */
    u_align_int_raw align_int_raw;                       /* 0x18f0 */
    u_align_int align_int;                               /* 0x18f4 */
    u_align_int_msk align_int_msk;                       /* 0x18f8 */
    unsigned int reserved_2[445];                        /* 0x18fc~0x1fec */
    u_chn_int_raw chn_int_raw;                           /* 0x1ff0 */
    u_chn_int chn_int;                                   /* 0x1ff4 */
    u_chn_int_mask chn_int_mask;                         /* 0x1ff8 */
    unsigned int reserved_3;                             /* 0x1ffc */
} global_ctrl_regs_t;

typedef struct {
    mipi_ctrl_regs_t       mipi_ctrl_regs;   /* 0x1000 ~ 0x12fc */
    lvds_ctrl_regs_t       lvds_ctrl_regs;   /* 0x1300 ~ 0x17fc */
    global_ctrl_regs_t     global_ctrl_regs; /* 0x1800 ~ 0x1ffc */
} mipi_rx_ctrl_regs_t;

/* Define the global struct */
typedef struct {
    mipi_rx_phy_cfg_t      mipi_rx_phy_cfg[2];      /* 0x0 ~ 0x1fc, 0x200 ~ 0x3fc */
    volatile unsigned int  reserved_0[256];         /* 0x400 ~ 0x7fc */
    mipi_rx_sys_regs_t     mipi_rx_sys_regs;        /* 0x800 ~ 0xffc */
    mipi_rx_ctrl_regs_t    mipi_rx_ctrl_regs[4];    /* 0x1000 ~ 0x1ffc */
} mipi_rx_regs_type_t;

#endif /* MIPI_RX_REG_H */
