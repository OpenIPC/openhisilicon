/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_VIPROC_REG_DEFINE_H
#define ISP_VIPROC_REG_DEFINE_H

/* Define the union u_isp_viproc_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    out0_crop_en          : 1; /* [0]  */
        unsigned int    out1_crop_en          : 1; /* [1]  */
        unsigned int    wdrmap_crop_en        : 1; /* [2]  */
        unsigned int    bas_out_crop_en       : 1; /* [3]  */
        unsigned int    bnrrnt_crop_en        : 1; /* [4]  */
        unsigned int    ldcigain_crop_en      : 1; /* [5]  */
        unsigned int    ratio_crop_en         : 1; /* [6]  */
        unsigned int    isp_rgb3w_sel         : 1; /* [7]  */
        unsigned int    stitch0_en            : 1; /* [8]  */
        unsigned int    stitch0_mode          : 1; /* [9]  */
        unsigned int    stitch1_en            : 1; /* [10]  */
        unsigned int    stitch1_mode          : 1; /* [11]  */
        unsigned int    isp_rgb3w_en          : 1; /* [12]  */
        unsigned int    isp_be_en             : 1; /* [13]  */
        unsigned int    limit_range_en        : 1; /* [14]  */
        unsigned int    sigma_mot_sel         : 1; /* [15]  */
        unsigned int    img_pro_mode          : 2; /* [17..16]  */
        unsigned int    vcpin_sfifo_en        : 1; /* [18]  */
        unsigned int    dsf_en                : 1; /* [19]  */
        unsigned int    wdr_fpn_mux           : 1; /* [20]  */
        unsigned int    dcg_fpn_sel           : 1; /* [21]  */
        unsigned int    ch0_mux               : 1; /* [22]  */
        unsigned int    vnr_mode              : 1; /* [23]  */
        unsigned int    vnr_en                : 1; /* [24]  */
        unsigned int    fpn_out_crop_en       : 1; /* [25]  */
        unsigned int    isp_be_bypass_mode    : 3; /* [28..26]  */
        unsigned int    wdr_mode              : 1; /* [29]  */
        unsigned int    reserved_2            : 1; /* [30]  */
        unsigned int    video_en              : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ctrl0;

/* Define the union u_isp_viproc_regup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reg_up                : 1; /* [0]  */
        unsigned int    reserved_0            : 1; /* [1]  */
        unsigned int    reserved_1            : 1; /* [2]  */
        unsigned int    reserved_2            : 5; /* [7..3]  */
        unsigned int    manual_reg_up         : 1; /* [8]  */
        unsigned int    reserved_3            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_regup;

/* Define the union u_isp_viproc_ispbe_regup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    ispbe_reg_up          : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ispbe_regup;

/* Define the union u_isp_viproc_ispbe_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_fpn_en            : 1; /* [1]  */
        unsigned int    isp_fpn_mode          : 1; /* [2]  */
        unsigned int    reserved_1            : 1; /* [3]  */
        unsigned int    isp_dpc_en            : 1; /* [4]  */
        unsigned int    isp_dpc_stat_en       : 1; /* [5]  */
        unsigned int    isp_ge_en             : 1; /* [6]  */
        unsigned int    reserved_2            : 1; /* [7]  */
        unsigned int    reserved_3            : 1; /* [8]  */
        unsigned int    reserved_4            : 1; /* [9]  */
        unsigned int    isp_4dg_en            : 1; /* [10]  */
        unsigned int    isp_flicker_en        : 1; /* [11]  */
        unsigned int    isp_wdr_en            : 1; /* [12]  */
        unsigned int    isp_expander_en       : 1; /* [13]  */
        unsigned int    isp_crb_en            : 1; /* [14]  */
        unsigned int    isp_bcom_en           : 1; /* [15]  */
        unsigned int    reserved_5            : 1; /* [16]  */
        unsigned int    reserved_6            : 1; /* [17]  */
        unsigned int    reserved_7            : 1; /* [18]  */
        unsigned int    isp_geq_en            : 1; /* [19]  */
        unsigned int    isp_bnr_en            : 1; /* [20]  */
        unsigned int    isp_bdec_en           : 1; /* [21]  */
        unsigned int    reserved_8            : 1; /* [22]  */
        unsigned int    isp_lsc_en            : 1; /* [23]  */
        unsigned int    isp_dg_en             : 1; /* [24]  */
        unsigned int    isp_af_en             : 1; /* [25]  */
        unsigned int    isp_awb_en            : 1; /* [26]  */
        unsigned int    isp_ae_en             : 1; /* [27]  */
        unsigned int    isp_drclog_en         : 1; /* [28]  */
        unsigned int    isp_wb_en             : 1; /* [29]  */
        unsigned int    isp_blc_en            : 1; /* [30]  */
        unsigned int    reserved_9            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ispbe_ctrl0;

/* Define the union u_isp_viproc_ispbe_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_drc_en            : 1; /* [1]  */
        unsigned int    isp_la_en             : 1; /* [2]  */
        unsigned int    reserved_1            : 1; /* [3]  */
        unsigned int    isp_dmnr_nddm_en      : 1; /* [4]  */
        unsigned int    isp_dmnr_vhdm_en      : 1; /* [5]  */
        unsigned int    reserved_2            : 1; /* [6]  */
        unsigned int    reserved_3            : 1; /* [7]  */
        unsigned int    isp_cc_en             : 1; /* [8]  */
        unsigned int    isp_clut_en           : 1; /* [9]  */
        unsigned int    reserved_4            : 1; /* [10]  */
        unsigned int    isp_gamma_en          : 1; /* [11]  */
        unsigned int    isp_dehaze_en         : 1; /* [12]  */
        unsigned int    isp_csc_en            : 1; /* [13]  */
        unsigned int    isp_sumy_en           : 1; /* [14]  */
        unsigned int    isp_ldci_en           : 1; /* [15]  */
        unsigned int    isp_ca_en             : 1; /* [16]  */
        unsigned int    isp_split_en          : 1; /* [17]  */
        unsigned int    isp_sharpen_en        : 1; /* [18]  */
        unsigned int    isp_hcds_en           : 1; /* [19]  */
        unsigned int    reserved_5            : 1; /* [20]  */
        unsigned int    reserved_6            : 1; /* [21]  */
        unsigned int    isp_vcds_en           : 1; /* [22]  */
        unsigned int    reserved_7            : 1; /* [23]  */
        unsigned int    reserved_8            : 1; /* [24]  */
        unsigned int    reserved_9            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ispbe_ctrl1;

/* Define the union u_isp_viproc_ispbe_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_be_rggb_cfg       : 2; /* [1..0]  */
        unsigned int    reserved_0            : 14; /* [15..2]  */
        unsigned int    isp_be_timing_cfg     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ispbe_cfg;

/* Define the union u_isp_viproc_ratio_crop0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    ratio_center_hor      : 16; /* [15..0]  */
        unsigned int    ratio_center_ver      : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ratio_crop0;

/* Define the union u_isp_viproc_ratio_crop1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ratio_square_of_radius : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ratio_crop1;
/* Define the union u_isp_viproc_flick_gmean_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_flick_gmean_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_flick_gmean_addr_low;
/* Define the union u_isp_viproc_flick_gmean_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_flick_gmean_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_flick_gmean_addr_high;
/* Define the union u_isp_viproc_dpc_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_lut_addr_low   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dpc_lut_addr_low;
/* Define the union u_isp_viproc_dpc_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_lut_addr_high  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dpc_lut_addr_high;
/* Define the union u_isp_viproc_dpc_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dpc_lut_width_word;

/* Define the union u_isp_viproc_crb_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_crb_lut_addr_low   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_crb_lut_addr_low;

/* Define the union u_isp_viproc_crb_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_crb_lut_addr_high  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_crb_lut_addr_high;
/* Define the union u_isp_viproc_crb_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_crb_lut_width_word;

/* Define the union u_isp_viproc_bnr_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lut_addr_low   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_bnr_lut_addr_low;
/* Define the union u_isp_viproc_bnr_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lut_addr_high  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_bnr_lut_addr_high;
/* Define the union u_isp_viproc_bnr_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_bnr_lut_width_word;

/* Define the union u_isp_viproc_mlsc_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_lut_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_mlsc_lut_addr_low;
/* Define the union u_isp_viproc_mlsc_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_mlsc_lut_addr_high;
/* Define the union u_isp_viproc_mlsc_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_mlsc_lut_width_word;

/* Define the union u_isp_viproc_para_dci_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_dci_addr_low      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_para_dci_addr_low;
/* Define the union u_isp_viproc_para_dci_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_dci_addr_high     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_para_dci_addr_high;

/* Define the union u_isp_viproc_out_para_dci_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_dci_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_out_para_dci_addr_low;
/* Define the union u_isp_viproc_out_para_dci_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_dci_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_out_para_dci_addr_high;
/* Define the union u_isp_viproc_dehaze_minstat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_minstat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_minstat_addr_low;
/* Define the union u_isp_viproc_dehaze_minstat_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_minstat_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_minstat_addr_high;
/* Define the union u_isp_viproc_dehaze_maxstat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_maxstat_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_maxstat_addr_low;
/* Define the union u_isp_viproc_dehaze_maxstat_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_maxstat_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_maxstat_addr_high;

/* Define the union u_isp_viproc_ldci_lpf_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lpf_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ldci_lpf_addr_low;
/* Define the union u_isp_viproc_ldci_lpf_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lpf_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ldci_lpf_addr_high;
/* Define the union u_isp_viproc_dmnr_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dmnr_lut_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dmnr_lut_addr_low;
/* Define the union u_isp_viproc_dmnr_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dmnr_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dmnr_lut_addr_high;
/* Define the union u_isp_viproc_dmnr_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dmnr_lut_width_word;
/* Define the union u_isp_viproc_cc_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_cc_lut_addr_low    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_cc_lut_addr_low;
/* Define the union u_isp_viproc_cc_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_cc_lut_addr_high   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_cc_lut_addr_high;
/* Define the union u_isp_viproc_cc_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_cc_lut_width_word;

/* Define the union u_isp_viproc_clut_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_clut_lut_addr_low;
/* Define the union u_isp_viproc_clut_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_clut_lut_addr_high;
/* Define the union u_isp_viproc_clut_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_clut_lut_width_word;

/* Define the union u_isp_viproc_gamma_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_gamma_lut_addr_low;
/* Define the union u_isp_viproc_gamma_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_gamma_lut_addr_high;
/* Define the union u_isp_viproc_gamma_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_gamma_lut_width_word;

/* Define the union u_isp_viproc_dehaze_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_lut_addr_low;
/* Define the union u_isp_viproc_dehaze_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_lut_addr_high;
/* Define the union u_isp_viproc_dehaze_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_dehaze_lut_width_word;

/* Define the union u_isp_viproc_ca_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ca_lut_addr_low    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ca_lut_addr_low;
/* Define the union u_isp_viproc_ca_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ca_lut_addr_high   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ca_lut_addr_high;
/* Define the union u_isp_viproc_ca_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ca_lut_width_word;

/* Define the union u_isp_viproc_sharpen_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_lut_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_sharpen_lut_addr_low;
/* Define the union u_isp_viproc_sharpen_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_sharpen_lut_addr_high;
/* Define the union u_isp_viproc_sharpen_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_sharpen_lut_width_word;
/* Define the union u_isp_viproc_ldci_lut_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lut_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ldci_lut_addr_low;
/* Define the union u_isp_viproc_ldci_lut_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lut_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ldci_lut_addr_high;
/* Define the union u_isp_viproc_ldci_lut_width_word */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lut_width_word : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ldci_lut_width_word;


/* Define the union u_isp_viproc_ae_hist_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_addr_low   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ae_hist_addr_low;
/* Define the union u_isp_viproc_ae_hist_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_addr_high  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ae_hist_addr_high;
/* Define the union u_isp_viproc_ae_aver_r_gr_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ae_aver_r_gr_addr_low;
/* Define the union u_isp_viproc_ae_aver_r_gr_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ae_aver_r_gr_addr_high;
/* Define the union u_isp_viproc_ae_aver_gb_b_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_addr_low : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ae_aver_gb_b_addr_low;
/* Define the union u_isp_viproc_ae_aver_gb_b_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_ae_aver_gb_b_addr_high;
/* Define the union u_isp_viproc_awb_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_awb_stat_addr_low;
/* Define the union u_isp_viproc_awb_stat_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_awb_stat_addr_high;
/* Define the union u_isp_viproc_af_stat_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_addr_low   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_af_stat_addr_low;
/* Define the union u_isp_viproc_af_stat_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_addr_high  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_af_stat_addr_high;
/* Define the union u_isp_viproc_la_aver_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_addr_low   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_la_aver_addr_low;
/* Define the union u_isp_viproc_la_aver_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_addr_high  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_la_aver_addr_high;
/* Define the union u_isp_viproc_para_drc_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_drc_addr_low      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_para_drc_addr_low;
/* Define the union u_isp_viproc_para_drc_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_drc_addr_high     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_para_drc_addr_high;
/* Define the union u_isp_viproc_out_para_drc_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_drc_addr_low  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_out_para_drc_addr_low;
/* Define the union u_isp_viproc_out_para_drc_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_drc_addr_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_out_para_drc_addr_high;
/* Define the union u_isp_viproc_drcdetail_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    drcdetail_rdcmp_en    : 1; /* [0]  */
        unsigned int    reserved_0            : 7; /* [7..1]  */
        unsigned int    drcdetail_bitw        : 5; /* [12..8]  */
        unsigned int    reserved_1            : 3; /* [15..13]  */
        unsigned int    drcdetail_format      : 6; /* [21..16]  */
        unsigned int    reserved_2            : 9; /* [30..22]  */
        unsigned int    drcdetail_en          : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_drcdetail_ctrl;

/* Define the union u_isp_viproc_drcdetail_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    drcdetail_width       : 16; /* [15..0]  */
        unsigned int    drcdetail_height      : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_drcdetail_size;

/* Define the union u_isp_viproc_drcdetail_addr_low */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int drcdetail_addr_low     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_drcdetail_addr_low;
/* Define the union u_isp_viproc_drcdetail_addr_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int drcdetail_addr_high    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_drcdetail_addr_high;
/* Define the union u_isp_viproc_drcdetail_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    drcdetail_stride      : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_viproc_drcdetail_stride;

/* Define the global struct */
typedef struct {
    volatile u_isp_viproc_ctrl0                  viproc_ctrl0; /* 0x0 */
    volatile unsigned int                    reserved_0[3]; /* 0x4~0xc, reserved 3 * 4 bytes */
    volatile u_isp_viproc_regup                  viproc_regup; /* 0x10 */
    volatile u_isp_viproc_ispbe_regup            viproc_ispbe_regup; /* 0x14 */
    volatile unsigned int                    reserved_1[2]; /* 0x18~0x1c, reserved 2 * 4 bytes */
    volatile u_isp_viproc_ispbe_ctrl0            viproc_ispbe_ctrl0; /* 0x20 */
    volatile u_isp_viproc_ispbe_ctrl1            viproc_ispbe_ctrl1; /* 0x24 */
    volatile unsigned int                    reserved_2; /* 0x28 */
    volatile u_isp_viproc_ispbe_cfg              viproc_ispbe_cfg; /* 0x2c */
    volatile unsigned int                    reserved_3[20]; /* 0x30~0x7c, reserved 20 * 4 bytes */
    volatile u_isp_viproc_ratio_crop0            viproc_ratio_crop0; /* 0x80 */
    volatile u_isp_viproc_ratio_crop1            viproc_ratio_crop1; /* 0x84 */
    volatile unsigned int                    reserved_4[261]; /* 0x88~0x498, reserved 261 * 4 bytes */
    volatile u_isp_viproc_flick_gmean_addr_low   flick_gmean_addr_low; /* 0x49c */
    volatile u_isp_viproc_flick_gmean_addr_high   flick_gmean_addr_high; /* 0x4a0 */
    volatile u_isp_viproc_dpc_lut_addr_low   dpc_lut_addr_low; /* 0x4a4 */
    volatile u_isp_viproc_dpc_lut_addr_high   dpc_lut_addr_high; /* 0x4a8 */
    volatile u_isp_viproc_dpc_lut_width_word   dpc_lut_width_word; /* 0x4ac */
    volatile u_isp_viproc_crb_lut_addr_low   crb_lut_addr_low; /* 0x4b0 */
    volatile u_isp_viproc_crb_lut_addr_high   crb_lut_addr_high; /* 0x4b4 */
    volatile u_isp_viproc_crb_lut_width_word   crb_lut_width_word; /* 0x4b8 */
    volatile u_isp_viproc_bnr_lut_addr_low   bnr_lut_addr_low; /* 0x4bc */
    volatile u_isp_viproc_bnr_lut_addr_high   bnr_lut_addr_high; /* 0x4c0 */
    volatile u_isp_viproc_bnr_lut_width_word   bnr_lut_width_word; /* 0x4c4 */
    volatile u_isp_viproc_mlsc_lut_addr_low   mlsc_lut_addr_low; /* 0x4c8 */
    volatile u_isp_viproc_mlsc_lut_addr_high   mlsc_lut_addr_high; /* 0x4cc */
    volatile u_isp_viproc_mlsc_lut_width_word   mlsc_lut_width_word; /* 0x4d0 */

    volatile unsigned int                    reserved_5[114]; /* 0x4d4~0x698, reserved 114 * 4 bytes */
    volatile u_isp_viproc_para_dci_addr_low      viproc_para_dci_addr_low; /* 0x69c */
    volatile u_isp_viproc_para_dci_addr_high     viproc_para_dci_addr_high; /* 0x6a0 */
    volatile u_isp_viproc_out_para_dci_addr_low   viproc_out_para_dci_addr_low; /* 0x6a4 */
    volatile u_isp_viproc_out_para_dci_addr_high   viproc_out_para_dci_addr_high; /* 0x6a8 */
    volatile u_isp_viproc_dehaze_minstat_addr_low   dehaze_minstat_addr_low; /* 0x6ac */
    volatile u_isp_viproc_dehaze_minstat_addr_high   dehaze_minstat_addr_high; /* 0x6b0 */
    volatile u_isp_viproc_dehaze_maxstat_addr_low  dehaze_maxstat_addr_low; /* 0x6b4 */
    volatile u_isp_viproc_dehaze_maxstat_addr_high   dehaze_maxstat_addr_high; /* 0x6b8 */
    volatile u_isp_viproc_ldci_lpf_addr_low   ldci_lpf_addr_low; /* 0x6bc */
    volatile u_isp_viproc_ldci_lpf_addr_high   ldci_lpf_addr_high; /* 0x6c0 */
    volatile u_isp_viproc_dmnr_lut_addr_low   dmnr_lut_addr_low; /* 0x6c4 */
    volatile u_isp_viproc_dmnr_lut_addr_high   dmnr_lut_addr_high; /* 0x6c8 */
    volatile u_isp_viproc_dmnr_lut_width_word   dmnr_lut_width_word; /* 0x6cc */
    volatile u_isp_viproc_cc_lut_addr_low    cc_lut_addr_low; /* 0x6d0 */
    volatile u_isp_viproc_cc_lut_addr_high   cc_lut_addr_high; /* 0x6d4 */
    volatile u_isp_viproc_cc_lut_width_word   cc_lut_width_word; /* 0x6d8 */
    volatile u_isp_viproc_clut_lut_addr_low   clut_lut_addr_low; /* 0x6dc */
    volatile u_isp_viproc_clut_lut_addr_high   clut_lut_addr_high; /* 0x6e0 */
    volatile u_isp_viproc_clut_lut_width_word   clut_lut_width_word; /* 0x6e4 */
    volatile u_isp_viproc_gamma_lut_addr_low   gamma_lut_addr_low; /* 0x6e8 */
    volatile u_isp_viproc_gamma_lut_addr_high   gamma_lut_addr_high; /* 0x6ec */
    volatile u_isp_viproc_gamma_lut_width_word   gamma_lut_width_word; /* 0x6f0 */
    volatile u_isp_viproc_dehaze_lut_addr_low   dehaze_lut_addr_low; /* 0x6f4 */
    volatile u_isp_viproc_dehaze_lut_addr_high   dehaze_lut_addr_high; /* 0x6f8 */
    volatile u_isp_viproc_dehaze_lut_width_word   dehaze_lut_width_word; /* 0x6fc */
    volatile u_isp_viproc_ca_lut_addr_low    ca_lut_addr_low; /* 0x700 */
    volatile u_isp_viproc_ca_lut_addr_high   ca_lut_addr_high; /* 0x704 */
    volatile u_isp_viproc_ca_lut_width_word   ca_lut_width_word; /* 0x708 */
    volatile u_isp_viproc_sharpen_lut_addr_low   sharpen_lut_addr_low; /* 0x70c */
    volatile u_isp_viproc_sharpen_lut_addr_high   sharpen_lut_addr_high; /* 0x710 */
    volatile u_isp_viproc_sharpen_lut_width_word   sharpen_lut_width_word; /* 0x714 */
    volatile u_isp_viproc_ldci_lut_addr_low   ldci_lut_addr_low; /* 0x718 */
    volatile u_isp_viproc_ldci_lut_addr_high   ldci_lut_addr_high; /* 0x71c */
    volatile u_isp_viproc_ldci_lut_width_word   ldci_lut_width_word; /* 0x720 */
    volatile unsigned int                    reserved_16[23]; /* 0x724~0x77c */
    volatile u_isp_viproc_ae_hist_addr_low   ae_hist_addr_low; /* 0x780 */
    volatile u_isp_viproc_ae_hist_addr_high   ae_hist_addr_high; /* 0x784 */
    volatile u_isp_viproc_ae_aver_r_gr_addr_low   ae_aver_r_gr_addr_low; /* 0x788 */
    volatile u_isp_viproc_ae_aver_r_gr_addr_high   ae_aver_r_gr_addr_high; /* 0x78c */
    volatile u_isp_viproc_ae_aver_gb_b_addr_low   ae_aver_gb_b_addr_low; /* 0x790 */
    volatile u_isp_viproc_ae_aver_gb_b_addr_high   ae_aver_gb_b_addr_high; /* 0x794 */
    volatile u_isp_viproc_awb_stat_addr_low   awb_stat_addr_low; /* 0x798 */
    volatile u_isp_viproc_awb_stat_addr_high   awb_stat_addr_high; /* 0x79c */
    volatile u_isp_viproc_af_stat_addr_low   af_stat_addr_low; /* 0x7a0 */
    volatile u_isp_viproc_af_stat_addr_high   af_stat_addr_high; /* 0x7a4 */
    volatile u_isp_viproc_la_aver_addr_low   la_aver_addr_low; /* 0x7a8 */
    volatile u_isp_viproc_la_aver_addr_high   la_aver_addr_high; /* 0x7ac */
    volatile u_isp_viproc_para_drc_addr_low      viproc_para_drc_addr_low; /* 0x7b0 */
    volatile u_isp_viproc_para_drc_addr_high     viproc_para_drc_addr_high; /* 0x7b4 */
    volatile u_isp_viproc_out_para_drc_addr_low   viproc_out_para_drc_addr_low; /* 0x7b8 */
    volatile u_isp_viproc_out_para_drc_addr_high   viproc_out_para_drc_addr_high; /* 0x7bc */
    volatile u_isp_viproc_drcdetail_ctrl         viproc_drcdetail_ctrl; /* 0x7c0 */
    volatile u_isp_viproc_drcdetail_size         viproc_drcdetail_size; /* 0x7c4 */
    volatile u_isp_viproc_drcdetail_addr_low     viproc_drcdetail_addr_low; /* 0x7c8 */
    volatile u_isp_viproc_drcdetail_addr_high    viproc_drcdetail_addr_high; /* 0x7cc */
    volatile u_isp_viproc_drcdetail_stride       viproc_drcdetail_stride; /* 0x7d0 */
    volatile unsigned int                       reserved_9[11]; /* 0x7d4 ~ 0x7fc, reserved 45 * 4 bytes */
} isp_viproc_reg_type;

#endif /* ISP_VIPROC_REG_DEFINE_H */
