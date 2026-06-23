/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_TX_REG_H
#define MIPI_TX_REG_H

/* define the union reg_ctrl_reset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ctrl_reset : 1;  /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ctrl_reset;

/* define the union reg_crg_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txescclk_div : 8; /* [7..0]  */
        unsigned int toclk_div : 8;    /* [15..8]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_crg_cfg;

/* define the union reg_video_vc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vcid : 2;        /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int vcid_sel : 1;    /* [4]  */
        unsigned int reserved_1 : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_video_vc;

/* define the union reg_data_type */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int data_type : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_data_type;

/* define the union reg_lp_cmd_byte */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int invact_lpcmd_byte : 8;  /* [7..0]  */
        unsigned int reserved_0 : 8;         /* [15..8]  */
        unsigned int outvact_lpcmd_byte : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;         /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lp_cmd_byte;

/* define the union reg_pck_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int eotp_tx_en : 1;   /* [0]  */
        unsigned int eotp_rx_en : 1;   /* [1]  */
        unsigned int crc_rx_en : 1;    /* [2]  */
        unsigned int ecc_rx_en : 1;    /* [3]  */
        unsigned int bta_en : 1;       /* [4]  */
        unsigned int tear_fx_en : 1;   /* [5]  */
        unsigned int ack_rqst_en : 1;  /* [6]  */
        unsigned int frame_ack_en : 1; /* [7]  */
        unsigned int reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_pck_en;

/* define the union reg_gen_vc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int gen_vcid : 2;    /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gen_vc;

/* define the union reg_mode_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cmd_video_mode : 1;     /* [0]  */
        unsigned int reserved_0 : 3;         /* [3..1]  */
        unsigned int cmd_mode_tran_type : 1; /* [4]  */
        unsigned int reserved_1 : 3;         /* [7..5]  */
        unsigned int video_mode_type : 2;    /* [9..8]  */
        unsigned int reserved_2 : 2;         /* [11..10]  */
        unsigned int dualpix_en : 1;         /* [12]  */
        unsigned int reserved_3 : 3;         /* [15..13]  */
        unsigned int scramb_en : 1;          /* [16]  */
        unsigned int reserved_4 : 3;         /* [19..17]  */
        unsigned int cphy_ppi_dw_sel : 1;    /* [20]  */
        unsigned int reserved_5 : 11;        /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mode_cfg;

/* define the union reg_video_lp_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_lp_en : 1;   /* [0]  */
        unsigned int vbp_lp_en : 1;   /* [1]  */
        unsigned int vfp_lp_en : 1;   /* [2]  */
        unsigned int vact_lp_en : 1;  /* [3]  */
        unsigned int hbp_lp_en : 1;   /* [4]  */
        unsigned int hfp_lp_en : 1;   /* [5]  */
        unsigned int reserved_0 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_video_lp_en;

/* define the union reg_videom_pkt_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_pkt_size : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;      /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_pkt_size;

/* define the union reg_videom_num_chunks */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_num_chunks : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;        /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_num_chunks;

/* define the union reg_videom_null_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_null_size : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;       /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_null_size;

/* define the union reg_videom_hsa_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_hsa_size : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_hsa_size;

/* define the union reg_videom_hbp_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int videom_hbp_size : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_hbp_size;

/* define the union reg_videom_hline_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_hline_size : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;       /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_hline_size;

/* define the union reg_videom_vsa_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vpw : 10;  /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vsa_lines;

/* define the union reg_videom_vbp_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbp : 10;  /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vbp_lines;

/* define the union reg_videom_vfp_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vfp : 10;  /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vfp_lines;

/* define the union reg_videom_vactive_lines */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v_active_lines : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;     /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_videom_vactive_lines;

/* define the union reg_command_pkt_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cmd_pkt_size : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_pkt_size;

/* define the union reg_command_tran_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_rd_pkt_size_tran : 1; /* [0]  */
        unsigned int gen_sw_0p : 1;            /* [1]  */
        unsigned int gen_sw_1p : 1;            /* [2]  */
        unsigned int gen_sw_2p : 1;            /* [3]  */
        unsigned int gen_sr_0p : 1;            /* [4]  */
        unsigned int gen_sr_1p : 1;            /* [5]  */
        unsigned int gen_sr_2p : 1;            /* [6]  */
        unsigned int gen_lw : 1;               /* [7]  */
        unsigned int dcs_sw_0p : 1;            /* [8]  */
        unsigned int dcs_sw_1p : 1;            /* [9]  */
        unsigned int dcs_sr_0p : 1;            /* [10]  */
        unsigned int dcs_lw : 1;               /* [11]  */
        unsigned int reserved_0 : 20;          /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_tran_mode;

/* define the union reg_command_header */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int command_datatype : 6;       /* [5..0]  */
        unsigned int command_virtualchannel : 2; /* [7..6]  */
        unsigned int command_wordcount_low : 8;  /* [15..8]  */
        unsigned int command_wordcount_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;             /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_header;

/* define the union reg_command_payload */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int command_pld_b1 : 8; /* [7..0]  */
        unsigned int command_pld_b2 : 8; /* [15..8]  */
        unsigned int command_pld_b3 : 8; /* [23..16]  */
        unsigned int command_pld_b4 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_payload;

/* define the union reg_command_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int command_empty : 1;   /* [0]  */
        unsigned int command_full : 1;    /* [1]  */
        unsigned int pld_write_empty : 1; /* [2]  */
        unsigned int pld_write_full : 1;  /* [3]  */
        unsigned int pld_read_empty : 1;  /* [4]  */
        unsigned int pld_read_full : 1;   /* [5]  */
        unsigned int rd_cmd_busy : 1;     /* [6]  */
        unsigned int command_idle : 1;    /* [7]  */
        unsigned int reserved_0 : 24;     /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_command_status;

/* define the union reg_hs_lp_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lprx_to_cnt : 16; /* [15..0]  */
        unsigned int hstx_to_cnt : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hs_lp_to_set;

/* define the union reg_hsrd_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsrd_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hsrd_to_set;

/* define the union reg_lprd_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lprd_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lprd_to_set;

/* define the union reg_hswr_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hswr_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 8;   /* [23..16]  */
        unsigned int pre_to_mode : 1;  /* [24]  */
        unsigned int reserved_1 : 7;   /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hswr_to_set;

/* define the union reg_lpwr_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lpwr_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lpwr_to_set;

/* define the union reg_bta_to_set */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bta_to_set : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_bta_to_set;

/* define the union reg_clklane_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txrequesthsclk : 1;       /* [0]  */
        unsigned int clklane_continue : 1;     /* [1]  */
        unsigned int cphy_clklane_sel_qst : 1; /* [2]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_clklane_ctrl;

/* define the union reg_clklane_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int clklane_lp2hs_time : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int clklane_hs2lp_time : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_clklane_time;

/* define the union reg_datalane_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int datalane_lp2hs_time : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int datalane_hs2lp_time : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_datalane_time;

/* define the union reg_phy_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int shutdownz : 1;   /* [0]  */
        unsigned int rstz : 1;        /* [1]  */
        unsigned int enableclk : 1;   /* [2]  */
        unsigned int forcepll : 1;    /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_ctrl;

/* define the union reg_lane_num */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lane_num : 2;       /* [1..0]  */
        unsigned int reserved_0 : 6;     /* [7..2]  */
        unsigned int stopstate_time : 8; /* [15..8]  */
        unsigned int reserved_1 : 16;    /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lane_num;

/* define the union reg_ulps_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txulpsclklane : 1;      /* [0]  */
        unsigned int txulpsexitclklane : 1;  /* [1]  */
        unsigned int txulpsdatalane : 1;     /* [2]  */
        unsigned int txulpsexitdatalane : 1; /* [3]  */
        unsigned int reserved_0 : 28;        /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ulps_ctrl;

/* define the union reg_tx_triggers */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tx_triggers : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_tx_triggers;

/* define the union reg_ppi_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phy_pll_lock : 1;       /* [0]  */
        unsigned int phy_tran_direction : 1; /* [1]  */
        unsigned int stopstateclklane : 1;   /* [2]  */
        unsigned int ulpsactivenotclk : 1;   /* [3]  */
        unsigned int stopstate0lane : 1;     /* [4]  */
        unsigned int ulpsactivenot0lane : 1; /* [5]  */
        unsigned int rxulpsesc0lane : 1;     /* [6]  */
        unsigned int stopstate1lane : 1;     /* [7]  */
        unsigned int ulpsactivenot1lane : 1; /* [8]  */
        unsigned int stopstate2lane : 1;     /* [9]  */
        unsigned int ulpsactivenot2lane : 1; /* [10]  */
        unsigned int stopstate3lane : 1;     /* [11]  */
        unsigned int ulpsactivenot3lane : 1; /* [12]  */
        unsigned int reserved_0 : 19;        /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_status;

/* define the union reg_phy_reg_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int testclr : 1;     /* [0]  */
        unsigned int testclk : 1;     /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_reg_cfg0;

/* define the union reg_phy_reg_cfg1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int testdin : 8;     /* [7..0]  */
        unsigned int testdout : 8;    /* [15..8]  */
        unsigned int testen : 1;      /* [16]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_reg_cfg1;

/* define the union reg_int0_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int error_report_0 : 1;   /* [0]  */
        unsigned int error_report_1 : 1;   /* [1]  */
        unsigned int error_report_2 : 1;   /* [2]  */
        unsigned int error_report_3 : 1;   /* [3]  */
        unsigned int error_report_4 : 1;   /* [4]  */
        unsigned int error_report_5 : 1;   /* [5]  */
        unsigned int error_report_6 : 1;   /* [6]  */
        unsigned int error_report_7 : 1;   /* [7]  */
        unsigned int error_report_8 : 1;   /* [8]  */
        unsigned int error_report_9 : 1;   /* [9]  */
        unsigned int error_report_10 : 1;  /* [10]  */
        unsigned int error_report_11 : 1;  /* [11]  */
        unsigned int error_report_12 : 1;  /* [12]  */
        unsigned int error_report_13 : 1;  /* [13]  */
        unsigned int error_report_14 : 1;  /* [14]  */
        unsigned int error_report_15 : 1;  /* [15]  */
        unsigned int errescentry : 1;      /* [16]  */
        unsigned int errsyncesc : 1;       /* [17]  */
        unsigned int errcontrol : 1;       /* [18]  */
        unsigned int errcontentionlp0 : 1; /* [19]  */
        unsigned int errcontentionlp1 : 1; /* [20]  */
        unsigned int reserved_0 : 11;      /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int0_status;

/* define the union reg_int1_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int eotp_rx_err : 1;            /* [0]  */
        unsigned int rxpkt_size_err : 1;         /* [1]  */
        unsigned int rxcrc_err : 1;              /* [2]  */
        unsigned int rxecc_single_err : 1;       /* [3]  */
        unsigned int rxecc_multi_err : 1;        /* [4]  */
        unsigned int pld_wr_err : 1;             /* [5]  */
        unsigned int pld_rd_err : 1;             /* [6]  */
        unsigned int cmd_wr_err : 1;             /* [7]  */
        unsigned int cmd_pld_wr_err : 1;         /* [8]  */
        unsigned int cmd_pld_rd_err : 1;         /* [9]  */
        unsigned int rxpld_rd_err : 1;           /* [10]  */
        unsigned int rxpld_wr_err : 1;           /* [11]  */
        unsigned int to_lp_rx : 1;               /* [12]  */
        unsigned int to_hs_tx : 1;               /* [13]  */
        unsigned int phy_pll_lock_err : 1;       /* [14]  */
        unsigned int cmd_tran_end : 1;           /* [15]  */
        unsigned int line_length_change : 1;     /* [16]  */
        unsigned int hss_abnormal : 1;           /* [17]  */
        unsigned int fifo_nempty_when_vsync : 1; /* [18]  */
        unsigned int reserved_0 : 1;             /* [19]  */
        unsigned int vss : 1;                    /* [20]  */
        unsigned int reserved_1 : 3;             /* [23..21]  */
        unsigned int rxtrigger0 : 1;             /* [24]  */
        unsigned int rxtrigger1 : 1;             /* [25]  */
        unsigned int rxtrigger2 : 1;             /* [26]  */
        unsigned int rxtrigger3 : 1;             /* [27]  */
        unsigned int reserved_2 : 4;             /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int1_status;

/* define the union reg_int0_mask */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mask_error_report_0 : 1;   /* [0]  */
        unsigned int mask_error_report_1 : 1;   /* [1]  */
        unsigned int mask_error_report_2 : 1;   /* [2]  */
        unsigned int mask_error_report_3 : 1;   /* [3]  */
        unsigned int mask_error_report_4 : 1;   /* [4]  */
        unsigned int mask_error_report_5 : 1;   /* [5]  */
        unsigned int mask_error_report_6 : 1;   /* [6]  */
        unsigned int mask_error_report_7 : 1;   /* [7]  */
        unsigned int mask_error_report_8 : 1;   /* [8]  */
        unsigned int mask_error_report_9 : 1;   /* [9]  */
        unsigned int mask_error_report_10 : 1;  /* [10]  */
        unsigned int mask_error_report_11 : 1;  /* [11]  */
        unsigned int mask_error_report_12 : 1;  /* [12]  */
        unsigned int mask_error_report_13 : 1;  /* [13]  */
        unsigned int mask_error_report_14 : 1;  /* [14]  */
        unsigned int mask_error_report_15 : 1;  /* [15]  */
        unsigned int mask_errescentry : 1;      /* [16]  */
        unsigned int mask_errsyncesc : 1;       /* [17]  */
        unsigned int mask_errcontrol : 1;       /* [18]  */
        unsigned int mask_errcontentionlp0 : 1; /* [19]  */
        unsigned int mask_errcontentionlp1 : 1; /* [20]  */
        unsigned int reserved_0 : 11;           /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int0_mask;

/* define the union reg_int1_mask */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mask_eotp_rx_err : 1;            /* [0]  */
        unsigned int mask_rxpkt_size_err : 1;         /* [1]  */
        unsigned int mask_rxcrc_err : 1;              /* [2]  */
        unsigned int mask_rxecc_single_err : 1;       /* [3]  */
        unsigned int mask_rxecc_multi_err : 1;        /* [4]  */
        unsigned int mask_pld_wr_err : 1;             /* [5]  */
        unsigned int mask_pld_rd_err : 1;             /* [6]  */
        unsigned int mask_cmd_wr_err : 1;             /* [7]  */
        unsigned int mask_cmd_pld_wr_err : 1;         /* [8]  */
        unsigned int mask_cmd_pld_rd_err : 1;         /* [9]  */
        unsigned int mask_rxpld_rd_err : 1;           /* [10]  */
        unsigned int mask_rxpld_wr_err : 1;           /* [11]  */
        unsigned int mask_to_lp_rx : 1;               /* [12]  */
        unsigned int mask_to_hs_tx : 1;               /* [13]  */
        unsigned int mask_phy_pll_lock_err : 1;       /* [14]  */
        unsigned int mask_cmd_tran_end : 1;           /* [15]  */
        unsigned int mask_line_length_change : 1;     /* [16]  */
        unsigned int mask_hss_abnormal : 1;           /* [17]  */
        unsigned int mask_fifo_nempty_when_vsync : 1; /* [18]  */
        unsigned int reserved_0 : 1;                  /* [19]  */
        unsigned int mask_vss : 1;                    /* [20]  */
        unsigned int reserved_1 : 3;                  /* [23..21]  */
        unsigned int mask_rxtrigger0 : 1;             /* [24]  */
        unsigned int mask_rxtrigger1 : 1;             /* [25]  */
        unsigned int mask_rxtrigger2 : 1;             /* [26]  */
        unsigned int mask_rxtrigger3 : 1;             /* [27]  */
        unsigned int reserved_2 : 4;                  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int1_mask;

/* define the union reg_phy_cal */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int txskewcalhs : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_cal;

/* define the union reg_auto_ulps_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int auto_ulps : 1;    /* [0]  */
        unsigned int reserved_0 : 15;  /* [15..1]  */
        unsigned int pll_off_ulps : 1; /* [16]  */
        unsigned int reserved_1 : 15;  /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_auto_ulps_mode;

/* define the union reg_auto_ulps_wakeup_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int twakeup_clk_div : 16; /* [15..0]  */
        unsigned int twakeup_cnt : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_auto_ulps_wakeup_time;

/* define the union reg_dsc_config */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int compression_mode : 1; /* [0]  */
        unsigned int reserved_0 : 7;       /* [7..1]  */
        unsigned int algorithm_mode : 2;   /* [9..8]  */
        unsigned int reserved_1 : 6;       /* [15..10]  */
        unsigned int pps : 2;              /* [17..16]  */
        unsigned int reserved_2 : 6;       /* [23..18]  */
        unsigned int video_lpcmd : 1;      /* [24]  */
        unsigned int reserved_3 : 7;       /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dsc_config;

/* define the union reg_read_cmd_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int read_cmd_time : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;    /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_read_cmd_time;

/* define the union reg_auto_ulps_min_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int auto_ulps_min_time : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;         /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_auto_ulps_min_time;

/* define the union reg_phy_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phy_mode : 1;    /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_mode;

/* define the union reg_vid_shadow_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vid_shadow_en : 1;  /* [0]  */
        unsigned int reserved_0 : 7;     /* [7..1]  */
        unsigned int vid_shadow_req : 1; /* [8]  */
        unsigned int reserved_1 : 23;    /* [31..9]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_shadow_ctrl;

/* define the union reg_dsi_dbg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vid_delay : 17; /* [16..0]  */
        unsigned int phyfsm_st : 3;  /* [19..17]  */
        unsigned int dbips_st : 4;   /* [23..20]  */
        unsigned int vidregion : 3;  /* [26..24]  */
        unsigned int dpips_st : 5;   /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dsi_dbg0;

/* define the union reg_phy_pll_start_time */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phy_pll_start_time_qst : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phy_pll_start_time;

/* define the union reg_dbg_crc_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dbg_crc_init : 1;     /* [0]  */
        unsigned int dbg_crc_lane_sel : 2; /* [2..1]  */
        unsigned int dbg_crc_en : 1;       /* [3]  */
        unsigned int reserved_0 : 28;      /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dbg_crc_ctrl;

/* define the union reg_secu_cfg_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int secu_cfg_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_secu_cfg_en;

/* define the union reg_polarity_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsync_plr : 1;   /* [0]  */
        unsigned int hsync_plr : 1;   /* [1]  */
        unsigned int reserved_0 : 1;  /* [2]  */
        unsigned int data_en_plr : 1; /* [3]  */
        unsigned int reserved_1 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_polarity_ctrl;

/* define the union reg_dual_pixelmode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 5;     /* [4..0]  */
        unsigned int dual_mode_en : 1;   /* [5]  */
        unsigned int reserved_1 : 7;     /* [12..6]  */
        unsigned int dual_mode_ctrl : 1; /* [13]  */
        unsigned int reserved_2 : 18;    /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dual_pixelmode;

/* define the union reg_hrz_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dsi_hsize : 13;  /* [12..0]  */
        unsigned int reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hrz_ctrl;

/* define the union reg_vrt_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dsi_vsize : 13;  /* [12..0]  */
        unsigned int reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vrt_ctrl;

/* define the union reg_int_pro_msk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int int_pro_msk : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;  /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_int_pro_msk;

/* define the union reg_vstate */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vstate : 11;     /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vstate;

/* define the union reg_hstate */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hstate : 6;      /* [5..0]  */
        unsigned int reserved_0 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hstate;

/* define the union reg_dphytx_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dphytx_ctrl : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dphytx_ctrl;

/* define the union reg_phytx_trstop_flag */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int phytx_trstop_flag : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_phytx_trstop_flag;

/* define the union reg_mipi_dsi_mem_control */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tselr : 2;       /* [1..0]  */
        unsigned int tselw : 2;       /* [3..2]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dsi_mem_control;

/* define the union reg_display_control */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dpishutdn : 1;    /* [0]  */
        unsigned int dpicolorm : 1;    /* [1]  */
        unsigned int vsync_f_sub : 1;  /* [2]  */
        unsigned int vsync_b_sub : 1;  /* [3]  */
        unsigned int v1_sel : 3;       /* [6..4]  */
        unsigned int reserved_0 : 1;   /* [7]  */
        unsigned int u1_sel : 3;       /* [10..8]  */
        unsigned int reserved_1 : 1;   /* [11]  */
        unsigned int y1_sel : 3;       /* [14..12]  */
        unsigned int reserved_2 : 1;   /* [15]  */
        unsigned int v2_sel : 3;       /* [18..16]  */
        unsigned int reserved_3 : 1;   /* [19]  */
        unsigned int u2_sel : 3;       /* [22..20]  */
        unsigned int reserved_4 : 1;   /* [23]  */
        unsigned int y2_sel : 3;       /* [26..24]  */
        unsigned int reserved_5 : 1;   /* [27]  */
        unsigned int dataen_f_sub : 1; /* [28]  */
        unsigned int dataen_b_sub : 1; /* [29]  */
        unsigned int hsync_f_sub : 1;  /* [30]  */
        unsigned int hsync_b_sub : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_display_control;

/* define the union reg_operation_mode */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 1;          /* [0]  */
        unsigned int mem_ck_en : 1;           /* [1]  */
        unsigned int reserved_1 : 3;          /* [4..2]  */
        unsigned int init_skew_en : 1;        /* [5]  */
        unsigned int period_skew_en : 1;      /* [6]  */
        unsigned int reserved_2 : 1;          /* [7]  */
        unsigned int colorbar_en : 1;         /* [8]  */
        unsigned int colorbar_orien : 1;      /* [9]  */
        unsigned int colorbar_test_en : 1;    /* [10]  */
        unsigned int reserved_3 : 1;          /* [11]  */
        unsigned int colorbar_mode : 4;       /* [15..12]  */
        unsigned int read_empty_vsync_en : 1; /* [16]  */
        unsigned int reserved_4 : 3;          /* [19..17]  */
        unsigned int hss_abnormal_rst : 1;    /* [20]  */
        unsigned int rxesc_buf_en : 1;        /* [21]  */
        unsigned int reserved_5 : 2;          /* [23..22]  */
        unsigned int turndisable : 1;         /* [24]  */
        unsigned int forcerxmode : 1;         /* [25]  */
        unsigned int forcetxstopmode : 1;     /* [26]  */
        unsigned int video_te_en : 1;         /* [27]  */
        unsigned int reserved_6 : 3;          /* [30..28]  */
        unsigned int input_en : 1;            /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_operation_mode;

/* define the union reg_hfp_hact_in */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hact_in : 16; /* [15..0]  */
        unsigned int hfp_in : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hfp_hact_in;

/* define the union reg_hbp_hsa_in */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsa_in : 16; /* [15..0]  */
        unsigned int hbp_in : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hbp_hsa_in;

/* define the union reg_vert_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vact_det : 16; /* [15..0]  */
        unsigned int vall_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vert_det;

/* define the union reg_hori0_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hact_det : 16;  /* [15..0]  */
        unsigned int hline_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hori0_det;

/* define the union reg_hori1_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsa_det : 16; /* [15..0]  */
        unsigned int hbp_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hori1_det;

/* define the union reg_vsa_det */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_det : 16;    /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vsa_det;

/* define the union reg_v_h_send */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int h_send : 16; /* [15..0]  */
        unsigned int v_send : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v_h_send;

/* define the union reg_lanereqhs_width */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lanerequesths_width : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;          /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lanereqhs_width;

/* define the union reg_hs_data */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l0hsdata : 8; /* [7..0]  */
        unsigned int l1hsdata : 8; /* [15..8]  */
        unsigned int l2hsdata : 8; /* [23..16]  */
        unsigned int l3hsdata : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hs_data;

/* define the union reg_datatype0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dt_pixel : 6;   /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int dt_hss : 6;     /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int dt_vse : 6;     /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int dt_vss : 6;     /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_datatype0;

/* define the union reg_datatype1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dt_null : 6;    /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int dt_blank : 6;   /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int dt_etop : 6;    /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int dt_hse : 6;     /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_datatype1;

/* define the union reg_csi_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int csi_en : 1;           /* [0]  */
        unsigned int reserved_0 : 3;       /* [3..1]  */
        unsigned int csi_vse_position : 1; /* [4]  */
        unsigned int reserved_1 : 3;       /* [7..5]  */
        unsigned int frame_num_inc : 1;    /* [8]  */
        unsigned int reserved_2 : 3;       /* [11..9]  */
        unsigned int dt_config : 1;        /* [12]  */
        unsigned int reserved_3 : 3;       /* [15..13]  */
        unsigned int csi_hss_en : 1;       /* [16]  */
        unsigned int reserved_4 : 15;      /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_csi_ctrl;

/* define the union reg_lane_id */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lane0_id : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int lane1_id : 2;    /* [5..4]  */
        unsigned int reserved_1 : 2;  /* [7..6]  */
        unsigned int lane2_id : 2;    /* [9..8]  */
        unsigned int reserved_2 : 2;  /* [11..10]  */
        unsigned int lane3_id : 2;    /* [13..12]  */
        unsigned int reserved_3 : 18; /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_lane_id;

/* define the union reg_read_memory_delay_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int delay_regin : 17;   /* [16..0]  */
        unsigned int reserved_0 : 3;     /* [19..17]  */
        unsigned int delay_abnormal : 1; /* [20]  */
        unsigned int delay_from_reg : 1; /* [21]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_read_memory_delay_ctrl;

/* define the union reg_ppi_v_send0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsa_length : 16;     /* [15..0]  */
        unsigned int vsa_vbp_length : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_v_send0;

/* define the union reg_ppi_v_send1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vactive_length : 16; /* [15..0]  */
        unsigned int vtotal_length : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_v_send1;

/* define the union reg_ppi_h_send0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hsa_length : 16;     /* [15..0]  */
        unsigned int hsa_hbp_length : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send0;

/* define the union reg_ppi_h_send1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hactive_length : 16; /* [15..0]  */
        unsigned int htotal_length : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send1;

/* define the union reg_ppi_h_send2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int pix_pkt_length : 16;   /* [15..0]  */
        unsigned int pix_pkt_distance : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send2;

/* define the union reg_ppi_h_send3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int line_length_send_diff : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_ppi_h_send3;

/* define the union reg_dft_control */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int code_length : 17;    /* [16..0]  */
        unsigned int reserved_0 : 3;      /* [19..17]  */
        unsigned int pattern_type : 2;    /* [21..20]  */
        unsigned int reserved_1 : 2;      /* [23..22]  */
        unsigned int hs_lp_mode : 2;      /* [25..24]  */
        unsigned int txskewcalhs_dft : 1; /* [26]  */
        unsigned int bist_mode : 1;       /* [27]  */
        unsigned int compare_done : 1;    /* [28]  */
        unsigned int compare : 1;         /* [29]  */
        unsigned int compare_result : 1;  /* [30]  */
        unsigned int reserved_2 : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dft_control;

/* define the union reg_dft_prbs_init */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int prbs_init : 7;   /* [6..0]  */
        unsigned int reserved_0 : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dft_prbs_init;

// ==============================================================================
/* define the global struct */
typedef struct {
    volatile reg_ctrl_reset ctrl_reset;                         /* 0x0 */
    volatile unsigned int reserved_0;                           /* 0x4 */
    volatile reg_crg_cfg crg_cfg;                               /* 0x8 */
    volatile reg_video_vc video_vc;                             /* 0xc */
    volatile reg_data_type data_type;                           /* 0x10 */
    volatile unsigned int reserved_1;                           /* 0x14 */
    volatile reg_lp_cmd_byte lp_cmd_byte;                       /* 0x18 */
    volatile unsigned int reserved_2[4];                        /* 0x1c~0x28 */
    volatile reg_pck_en pck_en;                                 /* 0x2c */
    volatile reg_gen_vc gen_vc;                                 /* 0x30 */
    volatile reg_mode_cfg mode_cfg;                             /* 0x34 */
    volatile reg_video_lp_en video_lp_en;                       /* 0x38 */
    volatile reg_videom_pkt_size videom_pkt_size;               /* 0x3c */
    volatile reg_videom_num_chunks videom_num_chunks;           /* 0x40 */
    volatile reg_videom_null_size videom_null_size;             /* 0x44 */
    volatile reg_videom_hsa_size videom_hsa_size;               /* 0x48 */
    volatile reg_videom_hbp_size videom_hbp_size;               /* 0x4c */
    volatile reg_videom_hline_size videom_hline_size;           /* 0x50 */
    volatile reg_videom_vsa_lines videom_vsa_lines;             /* 0x54 */
    volatile reg_videom_vbp_lines videom_vbp_lines;             /* 0x58 */
    volatile reg_videom_vfp_lines videom_vfp_lines;             /* 0x5c */
    volatile reg_videom_vactive_lines videom_vactive_lines;     /* 0x60 */
    volatile reg_command_pkt_size command_pkt_size;             /* 0x64 */
    volatile reg_command_tran_mode command_tran_mode;           /* 0x68 */
    volatile reg_command_header command_header;                 /* 0x6c */
    volatile reg_command_payload command_payload;               /* 0x70 */
    volatile reg_command_status command_status;                 /* 0x74 */
    volatile reg_hs_lp_to_set hs_lp_to_set;                     /* 0x78 */
    volatile reg_hsrd_to_set hsrd_to_set;                       /* 0x7c */
    volatile reg_lprd_to_set lprd_to_set;                       /* 0x80 */
    volatile reg_hswr_to_set hswr_to_set;                       /* 0x84 */
    volatile reg_lpwr_to_set lpwr_to_set;                       /* 0x88 */
    volatile reg_bta_to_set bta_to_set;                         /* 0x8c */
    volatile unsigned int reserved_3;                           /* 0x90 */
    volatile reg_clklane_ctrl clklane_ctrl;                     /* 0x94 */
    volatile reg_clklane_time clklane_time;                     /* 0x98 */
    volatile reg_datalane_time datalane_time;                   /* 0x9c */
    volatile reg_phy_ctrl phy_ctrl;                             /* 0xa0 */
    volatile reg_lane_num lane_num;                             /* 0xa4 */
    volatile reg_ulps_ctrl ulps_ctrl;                           /* 0xa8 */
    volatile reg_tx_triggers tx_triggers;                       /* 0xac */
    volatile reg_ppi_status ppi_status;                         /* 0xb0 */
    volatile reg_phy_reg_cfg0 phy_reg_cfg0;                     /* 0xb4 */
    volatile reg_phy_reg_cfg1 phy_reg_cfg1;                     /* 0xb8 */
    volatile reg_int0_status int0_status;                       /* 0xbc */
    volatile reg_int1_status int1_status;                       /* 0xc0 */
    volatile reg_int0_mask int0_mask;                           /* 0xc4 */
    volatile reg_int1_mask int1_mask;                           /* 0xc8 */
    volatile reg_phy_cal phy_cal;                               /* 0xcc */
    volatile unsigned int reserved_4[2];                        /* 0xd0~0xd4 */
    volatile unsigned int int_force0;                           /* 0xd8 */
    volatile unsigned int int_force1;                           /* 0xdc */
    volatile reg_auto_ulps_mode auto_ulps_mode;                 /* 0xe0 */
    volatile unsigned int auto_ulps_entry_delay;                /* 0xe4 */
    volatile reg_auto_ulps_wakeup_time auto_ulps_wakeup_time;   /* 0xe8 */
    volatile unsigned int reserved_5;                           /* 0xec */
    volatile reg_dsc_config dsc_config;                         /* 0xf0 */
    volatile reg_read_cmd_time read_cmd_time;                   /* 0xf4 */
    volatile reg_auto_ulps_min_time auto_ulps_min_time;         /* 0xf8 */
    volatile reg_phy_mode phy_mode;                             /* 0xfc */
    volatile reg_vid_shadow_ctrl vid_shadow_ctrl;               /* 0x100 */
    volatile unsigned int reserved_6[38];                       /* 0x104~0x198 */
    volatile reg_dsi_dbg0 dsi_dbg0;                             /* 0x19c */
    volatile reg_phy_pll_start_time phy_pll_start_time;         /* 0x1a0 */
    volatile reg_dbg_crc_ctrl dbg_crc_ctrl;                     /* 0x1a4 */
    volatile unsigned int dbg_crc_val;                          /* 0x1a8 */
    volatile reg_secu_cfg_en secu_cfg_en;                       /* 0x1ac */
    volatile reg_polarity_ctrl polarity_ctrl;                   /* 0x1b0 */
    volatile unsigned int reserved_7;                           /* 0x1b4 */
    volatile reg_dual_pixelmode dual_pixelmode;                 /* 0x1b8 */
    volatile unsigned int reserved_8[7];                        /* 0x1bc~0x1d4 */
    volatile unsigned int frm_valid_dbg;                        /* 0x1d8 */
    volatile unsigned int reserved_9[4];                        /* 0x1dc~0x1e8 */
    volatile reg_hrz_ctrl hrz_ctrl;                             /* 0x1ec */
    volatile reg_vrt_ctrl vrt_ctrl;                             /* 0x1f0 */
    volatile unsigned int reserved_10;                          /* 0x1f4 */
    volatile reg_int_pro_msk int_pro_msk;                       /* 0x1f8 */
    volatile reg_vstate vstate;                                 /* 0x1fc */
    volatile reg_hstate hstate;                                 /* 0x200 */
    volatile unsigned int reserved_11[8];                       /* 0x204~0x220 */
    volatile unsigned int phytx_stopsnt;                        /* 0x224 */
    volatile reg_dphytx_ctrl dphytx_ctrl;                       /* 0x228 */
    volatile reg_phytx_trstop_flag phytx_trstop_flag;           /* 0x22c */
    volatile unsigned int phytx_status;                         /* 0x230 */
    volatile unsigned int reserved_12[6];                       /* 0x234~0x248 */
    volatile unsigned int gint_msk;                             /* 0x24c */
    volatile unsigned int reserved_13[44];                      /* 0x250~0x2fc */
    volatile reg_mipi_dsi_mem_control mipi_dsi_mem_control;     /* 0x300 */
    volatile reg_display_control display_control;               /* 0x304 */
    volatile reg_operation_mode operation_mode;                 /* 0x308 */
    volatile reg_hfp_hact_in hfp_hact_in;                       /* 0x30c */
    volatile reg_hbp_hsa_in hbp_hsa_in;                         /* 0x310 */
    volatile reg_vert_det vert_det;                             /* 0x314 */
    volatile reg_hori0_det hori0_det;                           /* 0x318 */
    volatile reg_hori1_det hori1_det;                           /* 0x31c */
    volatile reg_vsa_det vsa_det;                               /* 0x320 */
    volatile reg_v_h_send v_h_send;                             /* 0x324 */
    volatile reg_lanereqhs_width lanereqhs_width;               /* 0x328 */
    volatile reg_hs_data hs_data;                               /* 0x32c */
    volatile reg_datatype0 datatype0;                           /* 0x330 */
    volatile reg_datatype1 datatype1;                           /* 0x334 */
    volatile reg_csi_ctrl csi_ctrl;                             /* 0x338 */
    volatile unsigned int skew_begin;                           /* 0x33c */
    volatile unsigned int skew_end;                             /* 0x340 */
    volatile unsigned int reserved_14[3];                       /* 0x344~0x34c */
    volatile reg_lane_id lane_id;                               /* 0x350 */
    volatile reg_read_memory_delay_ctrl read_memory_delay_ctrl; /* 0x354 */
    volatile reg_ppi_v_send0 ppi_v_send0;                       /* 0x358 */
    volatile reg_ppi_v_send1 ppi_v_send1;                       /* 0x35c */
    volatile reg_ppi_h_send0 ppi_h_send0;                       /* 0x360 */
    volatile reg_ppi_h_send1 ppi_h_send1;                       /* 0x364 */
    volatile reg_ppi_h_send2 ppi_h_send2;                       /* 0x368 */
    volatile reg_ppi_h_send3 ppi_h_send3;                       /* 0x36c */
    volatile unsigned int reserved_15[12];                      /* 0x370~0x39c */
    volatile unsigned int te_length;                            /* 0x3a0 */
    volatile unsigned int reserved_16[15];                      /* 0x3a4~0x3dc */
    volatile reg_dft_control dft_control;                       /* 0x3e0 */
    volatile unsigned int dft_skew_period;                      /* 0x3e4 */
    volatile reg_dft_prbs_init dft_prbs_init;                   /* 0x3e8 */
} mipi_tx_regs_type_t;

#endif /* MIPI_TX_REG_H */
