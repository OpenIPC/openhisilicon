/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_REG_DEFINE_H
#define ISP_REG_DEFINE_H

#include "isp_lut_define.h"
#include "isp_viproc_reg_define.h"

/* vicap_ch  vicap_ch vicap_ch */
/* Define the union u_isp_ch_reg_newer */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    ch_reg_newer          : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_reg_newer;

/* Define the union u_isp_ch_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    ch_update             : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_update;

/* Define the union u_isp_ch_rch_para_addr_h */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int rch_para_addr_h        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_rch_para_addr_h;
/* Define the union u_isp_ch_rch_para_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int rch_para_addr_l        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_rch_para_addr_l;

/* Define the union u_isp_ch_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    ch_int_fstart         : 1; /* [0]  */
        unsigned int    ch_int_cc_int         : 1; /* [1]  */
        unsigned int    ch_int_buf_ovf        : 1; /* [2]  */
        unsigned int    ch_int_field_throw    : 1; /* [3]  */
        unsigned int    ch_int_update_cfg     : 1; /* [4]  */
        unsigned int    reserved_0            : 9; /* [13..5]  */
        unsigned int    ch_int_fstart_dly1    : 1; /* [14]  */
        unsigned int    ch_int_fstart_dly     : 1; /* [15]  */
        unsigned int    ch_stt_ae_finish      : 1; /* [16]  */
        unsigned int    ch_stt_af_finish      : 1; /* [17]  */
        unsigned int    ch_stt_em_finish      : 1; /* [18]  */
        unsigned int    ch_stt_int_buf_ovf    : 1; /* [19]  */
        unsigned int    ch_em_int_buf_ovf     : 1; /* [20]  */
        unsigned int    ch_tunl_line_int      : 1; /* [21]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_int;

/* Define the union u_isp_ch_int_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mask_fstart           : 1; /* [0]  */
        unsigned int    mask_cc_int           : 1; /* [1]  */
        unsigned int    mask_buf_ovf          : 1; /* [2]  */
        unsigned int    mask_field_throw      : 1; /* [3]  */
        unsigned int    mask_update_cfg       : 1; /* [4]  */
        unsigned int    reserved_0            : 9; /* [13..5]  */
        unsigned int    mask_fstart_dly1      : 1; /* [14]  */
        unsigned int    mask_fstart_dly       : 1; /* [15]  */
        unsigned int    mask_stt_ae_finish    : 1; /* [16]  */
        unsigned int    mask_stt_af_finish    : 1; /* [17]  */
        unsigned int    mask_stt_em_finish    : 1; /* [18]  */
        unsigned int    mask_stt_buf_ovf      : 1; /* [19]  */
        unsigned int    mask_em_buf_ovf       : 1; /* [20]  */
        unsigned int    mask_tunl_line_int    : 1; /* [21]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_int_mask;

/* Define the union u_isp_ch_wch_stt_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 17; /* [16..0]  */
        unsigned int    ch_wch_stt_finish_mode : 1; /* [17]  */
        unsigned int    reserved_1            : 12; /* [29..18]  */
        unsigned int    ch_wch_stt_32b_align  : 1; /* [30]  */
        unsigned int    reserved_2            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_cfg;

/* Define the union u_isp_ch_wch_stt_ae_hist_addr_h */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_hist_addr_h : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_ae_hist_addr_h;
/* Define the union u_isp_ch_wch_stt_ae_hist_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_hist_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_ae_hist_addr_l;
/* Define the union u_isp_ch_wch_stt_ae_aver_r_gr_addr_h */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_aver_r_gr_addr_h : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_ae_aver_r_gr_addr_h;
/* Define the union u_isp_ch_wch_stt_ae_aver_r_gr_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_aver_r_gr_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_ae_aver_r_gr_addr_l;
/* Define the union u_isp_ch_wch_stt_ae_aver_gb_b_addr_h */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_aver_gb_b_addr_h : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_ae_aver_gb_b_addr_h;
/* Define the union u_isp_ch_wch_stt_ae_aver_gb_b_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_aver_gb_b_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_ae_aver_gb_b_addr_l;
/* Define the union u_isp_ch_wch_stt_af_stat_y_addr_h */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_af_stat_y_addr_h : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_af_stat_y_addr_h;
/* Define the union u_isp_ch_wch_stt_af_stat_y_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_af_stat_y_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ch_wch_stt_af_stat_y_addr_l;

/* Define the global struct */
typedef struct {
    volatile unsigned int                         reserved_0;                      /* 0x10000 */
    volatile u_isp_ch_reg_newer                   ch_reg_newer;                    /* 0x10004 */
    volatile u_isp_ch_update                      ch_update;                       /* 0x10008 */
    volatile unsigned int                         reserved_1[51]; /* 0x1000c~0x100d4, reserved 51 * 4 bytes */
    volatile u_isp_ch_rch_para_addr_h             ch_rch_para_addr_h;              /* 0x100d8 */
    volatile u_isp_ch_rch_para_addr_l             ch_rch_para_addr_l;              /* 0x100dc */
    volatile unsigned int                         reserved_2[4]; /* 0x100e0~0x100ec, reserved 4 * 4 bytes */
    volatile u_isp_ch_int                         ch_int;                          /* 0x100f0 */
    volatile unsigned int                         reserved_3;                      /* 0x100f4 */
    volatile u_isp_ch_int_mask                    ch_int_mask;                     /* 0x100f8 */
    volatile unsigned int                         reserved_4[65]; /* 0x100fc ~ 0x101fc, reserved 65 * 4 bytes */
    volatile u_isp_ch_wch_stt_cfg                 ch_wch_stt_cfg;                  /* 0x10200 */
    volatile u_isp_ch_wch_stt_ae_hist_addr_h      ch_wch_stt_ae_hist_addr_h;       /* 0x10204 */
    volatile u_isp_ch_wch_stt_ae_hist_addr_l      ch_wch_stt_ae_hist_addr_l;       /* 0x10208 */
    volatile u_isp_ch_wch_stt_ae_aver_r_gr_addr_h ch_wch_stt_ae_aver_r_gr_addr_h;  /* 0x1020c */
    volatile u_isp_ch_wch_stt_ae_aver_r_gr_addr_l ch_wch_stt_ae_aver_r_gr_addr_l;  /* 0x10210 */
    volatile u_isp_ch_wch_stt_ae_aver_gb_b_addr_h ch_wch_stt_ae_aver_gb_b_addr_h;  /* 0x10214 */
    volatile u_isp_ch_wch_stt_ae_aver_gb_b_addr_l ch_wch_stt_ae_aver_gb_b_addr_l;  /* 0x10218 */
    volatile unsigned int                         reserved_5[8]; /* 0x1021c~0x10238, reserved 8 * 4 bytes */
    volatile u_isp_ch_wch_stt_af_stat_y_addr_h    ch_wch_stt_af_stat_y_addr_h;       /* 0x1023c */
    volatile u_isp_ch_wch_stt_af_stat_y_addr_l    ch_wch_stt_af_stat_y_addr_l;       /* 0x10240 */
    volatile unsigned int                         reserved_6[47];  /* 0x10244 ~ 0x102fc, reserved 47 * 4 bytes */
} isp_vicap_ch_reg_type;

/* FE  FE  FE FE  FE  FE */
/* Define the union u_isp_fe_version */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_patch          : 8; /* [7..0]  */
        unsigned int    isp_fe_build          : 8; /* [15..8]  */
        unsigned int    isp_fe_release        : 8; /* [23..16]  */
        unsigned int    isp_fe_version        : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_version;

/* Define the union u_isp_fe_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_date            : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_date;
/* Define the union u_isp_fe_fpga_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fpga_date       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_fpga_date;
/* Define the union u_isp_fe_module_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 16; /* [15..0]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_module_pos;

/* Define the union u_isp_fe_fstart */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_fstart;
/* Define the union u_isp_fe_user_define0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_user_define0    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_user_define0;
/* Define the union u_isp_fe_user_define1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_user_define1    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_user_define1;
/* Define the union u_isp_fe_startup */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fcnt            : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_startup;
/* Define the union u_isp_fe_format */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_format          : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_format;

/* Define the union u_isp_glb_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_glb_blc_mode : 2; /* [1..0]  */
        unsigned int isp_glb_tg_mode : 2;  /* [3..2]  */
        unsigned int reserved_0 : 28;      /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_glb_blc_cfg;

/* Define the union u_isp_blc_cfg00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_glb0_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_glb0_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_cfg00;

/* Define the union u_isp_blc_cfg01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_glb0_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_glb0_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_cfg01;

/* Define the union u_isp_blc_cfg10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_glb1_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_glb1_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_cfg10;

/* Define the union u_isp_blc_cfg11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_glb1_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_glb1_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_cfg11;

/* Define the union u_isp_blc_cfg20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_tg_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;    /* [15]  */
        unsigned int isp_tg_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_cfg20;

/* Define the union u_isp_blc_cfg21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_tg_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;    /* [15]  */
        unsigned int isp_tg_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_cfg21;

/* Define the union u_isp_fe_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_int_fstart     : 1; /* [0]  */
        unsigned int    isp_fe_int_update_cfg : 1; /* [1]  */
        unsigned int    isp_fe_int_cfg_loss   : 1; /* [2]  */
        unsigned int    isp_fe_int_int_delay  : 1; /* [3]  */
        unsigned int    isp_fe_int_ae1_int    : 1; /* [4]  */
        unsigned int    isp_fe_int_dynblc_int : 1; /* [5]  */
        unsigned int    isp_fe_int_af1_int    : 1; /* [6]  */
        unsigned int    isp_fe_int_dis1_int   : 1; /* [7]  */
        unsigned int    isp_fe_int_done_int   : 1; /* [8]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_int;

/* Define the union u_isp_fe_int_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_int_fstart_masked : 1; /* [0]  */
        unsigned int    isp_fe_int_update_cfg_masked : 1; /* [1]  */
        unsigned int    isp_fe_int_cfg_loss_masked : 1; /* [2]  */
        unsigned int    isp_fe_int_int_delay_masked : 1; /* [3]  */
        unsigned int    isp_fe_int_ae1_int_masked : 1; /* [4]  */
        unsigned int    isp_fe_int_dynblc_int_masked : 1; /* [5]  */
        unsigned int    isp_fe_int_af1_int_masked : 1; /* [6]  */
        unsigned int    isp_fe_int_dis1_int_masked : 1; /* [7]  */
        unsigned int    isp_fe_int_done_int_masked : 1; /* [8]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_int_state;

/* Define the union u_isp_fe_int_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_int_mask_fstart : 1; /* [0]  */
        unsigned int    isp_fe_int_mask_update_cfg : 1; /* [1]  */
        unsigned int    isp_fe_int_mask_cfg_loss : 1; /* [2]  */
        unsigned int    isp_fe_int_mask_int_delay : 1; /* [3]  */
        unsigned int    isp_fe_int_mask_ae1_int : 1; /* [4]  */
        unsigned int    isp_fe_int_mask_dynblc_int : 1; /* [5]  */
        unsigned int    isp_fe_int_mask_af1_int : 1; /* [6]  */
        unsigned int    isp_fe_int_mask_dis1_int : 1; /* [7]  */
        unsigned int    isp_fe_int_mask_done_int : 1; /* [8]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_int_mask;

/* Define the union u_isp_fe_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_line_dly_en       : 1; /* [1]  */
        unsigned int    reserved_1            : 1; /* [2]  */
        unsigned int    isp_crop_en           : 1; /* [3]  */
        unsigned int    isp_blc1_en           : 1; /* [4]  */
        unsigned int    isp_lblc_en           : 1; /* [5]  */
        unsigned int    reserved_2            : 1; /* [6]  */
        unsigned int    isp_dg1_en            : 1; /* [7]  */
        unsigned int    isp_dg2_en            : 1; /* [8]  */
        unsigned int    reserved_3            : 1; /* [9]  */
        unsigned int    isp_af1_en            : 1; /* [10]  */
        unsigned int    isp_wb1_en            : 1; /* [11]  */
        unsigned int    reserved_4            : 1; /* [12]  */
        unsigned int    isp_ae1_en            : 1; /* [13]  */
        unsigned int    reserved_5            : 1; /* [14]  */
        unsigned int    isp_blc_dyn_en        : 1; /* [15]  */
        unsigned int    isp_fpn_en            : 1; /* [16]  */
        unsigned int    reserved_6            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_ctrl;

/* Define the union u_isp_fe_manual_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_manual_update  : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_manual_update;

/* Define the union u_isp_fe_adapter_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    fe_hsync_mode         : 1; /* [0]  */
        unsigned int    fe_vsync_mode         : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_adapter_cfg;

/* Define the union u_isp_fe_out_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    fe_out_width          : 16; /* [15..0]  */
        unsigned int    fe_out_height         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_out_size;

/* Define the union u_isp_fe_fstart_delay */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_delay           : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_fstart_delay;
/* Define the union u_isp_fe_ctrl_f */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_rggb_cfg       : 2; /* [1..0]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_ctrl_f;

/* Define the union u_isp_fe_ctrl_i */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_update_mode    : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_ctrl_i;

/* Define the union u_isp_fe_timing_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_fe_fix_timing     : 13; /* [13..1]  */
        unsigned int    reserved_1            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_timing_cfg;

/* Define the union u_isp_fe_reg_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_update         : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_reg_update;

/* Define the union u_isp_fe_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_width          : 16; /* [15..0]  */
        unsigned int    isp_fe_height         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_size;

/* Define the union u_isp_fe_blk_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_blk_width      : 16; /* [15..0]  */
        unsigned int    isp_fe_blk_height     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_blk_size;

/* Define the union u_isp_fe_blk_hblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_blk_b_hblank   : 16; /* [15..0]  */
        unsigned int    isp_fe_blk_f_hblank   : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_blk_hblank;

/* Define the union u_isp_fe_blk_vblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_blk_b_vblank   : 16; /* [15..0]  */
        unsigned int    isp_fe_blk_f_vblank   : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_blk_vblank;

/* Define the union u_isp_blc1_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc1_version;
/* Define the union u_isp_blc1_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc1_ofsgr        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_blc1_ofsr         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc1_offset1;

/* Define the union u_isp_blc1_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc1_ofsgb        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_blc1_ofsb         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc1_offset2;

/* Define the union u_isp_blc1_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc1_roi_y : 13; /* [12..0]  */
        unsigned int isp_blc1_roi_x : 13; /* [25..13]  */
        unsigned int isp_blc1_roi_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;      /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc1_roi_cfg0;

/* Define the union u_isp_blc1_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc1_roi_height : 13; /* [12..0]  */
        unsigned int isp_blc1_roi_width : 13;  /* [25..13]  */
        unsigned int reserved_0 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc1_roi_cfg1;

/* Define the union u_isp_lblc_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_stt2lut_en   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_stt2lut_cfg;

/* Define the union u_isp_lblc_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_stt2lut_regnew;

/* Define the union u_isp_lblc_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_lblc_stt2lut_clr  : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_stt2lut_abn;

/* Define the union u_isp_lblc_winnum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_num_row      : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_lblc_num_col      : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winnum;

/* Define the union u_isp_lblc_winx0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax0      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx0        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx0;

/* Define the union u_isp_lblc_winx1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax1      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx1        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx1;

/* Define the union u_isp_lblc_winx2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax2      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx2        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx2;

/* Define the union u_isp_lblc_winx3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax3      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx3        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx3;

/* Define the union u_isp_lblc_winx4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax4      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx4        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx4;

/* Define the union u_isp_lblc_winx5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax5      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx5        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx5;

/* Define the union u_isp_lblc_winx6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax6      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx6        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx6;

/* Define the union u_isp_lblc_winx7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax7      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx7        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx7;

/* Define the union u_isp_lblc_winx8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax8      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx8        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx8;

/* Define the union u_isp_lblc_winx9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax9      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx9        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx9;

/* Define the union u_isp_lblc_winx10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax10     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx10       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx10;

/* Define the union u_isp_lblc_winx11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax11     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx11       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx11;

/* Define the union u_isp_lblc_winx12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax12     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx12       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx12;

/* Define the union u_isp_lblc_winx13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax13     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx13       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx13;

/* Define the union u_isp_lblc_winx14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax14     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx14       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx14;

/* Define the union u_isp_lblc_winx15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax15     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx15       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx15;

/* Define the union u_isp_lblc_winy0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay0      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy0        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy0;

/* Define the union u_isp_lblc_winy1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay1      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy1        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy1;

/* Define the union u_isp_lblc_winy2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay2      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy2        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy2;

/* Define the union u_isp_lblc_winy3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay3      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy3        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy3;

/* Define the union u_isp_lblc_winy4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay4      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy4        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy4;

/* Define the union u_isp_lblc_winy5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay5      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy5        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy5;

/* Define the union u_isp_lblc_winy6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay6      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy6        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy6;

/* Define the union u_isp_lblc_winy7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay7      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy7        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy7;

/* Define the union u_isp_lblc_width_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_width_offset : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_width_offset;

/* Define the union u_isp_lblc_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_strength     : 10; /* [9..0]  */
        unsigned int    isp_lblc_lut_shift_bit : 3; /* [12..10]  */
        unsigned int    isp_lblc_bitwidth_in  : 5; /* [17..13]  */
        unsigned int    isp_lblc_dither_en    : 1; /* [18]  */
        unsigned int    isp_lblc_roi_en       : 1; /* [19]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_ctrl0;

/* Define the union u_isp_lblc_rgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_r_waddr       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_rgain_waddr;
/* Define the union u_isp_lblc_rgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_r_wdata      : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_rgain_wdata;

/* Define the union u_isp_lblc_rgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_r_raddr       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_rgain_raddr;
/* Define the union u_isp_lblc_rgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_r_rdata      : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_rgain_rdata;

/* Define the union u_isp_lblc_grgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_gr_waddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_grgain_waddr;
/* Define the union u_isp_lblc_grgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_gr_wdata     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_grgain_wdata;

/* Define the union u_isp_lblc_grgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_gr_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_grgain_raddr;
/* Define the union u_isp_lblc_grgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_gr_rdata     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_grgain_rdata;

/* Define the union u_isp_lblc_bgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_b_waddr       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_bgain_waddr;
/* Define the union u_isp_lblc_bgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_b_wdata      : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_bgain_wdata;

/* Define the union u_isp_lblc_bgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_b_raddr       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_bgain_raddr;
/* Define the union u_isp_lblc_bgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_b_rdata      : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_bgain_rdata;

/* Define the union u_isp_lblc_gbgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_gb_waddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_gbgain_waddr;
/* Define the union u_isp_lblc_gbgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_gb_wdata     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_gbgain_wdata;

/* Define the union u_isp_lblc_gbgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lblc_gb_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_gbgain_raddr;
/* Define the union u_isp_lblc_gbgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_gb_rdata     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_gbgain_rdata;

/* Define the union u_isp_lblc_winx16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax16     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx16       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx16;

/* Define the union u_isp_lblc_winx17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax17     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx17       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx17;

/* Define the union u_isp_lblc_winx18 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax18     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx18       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx18;

/* Define the union u_isp_lblc_winx19 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax19     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx19       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx19;

/* Define the union u_isp_lblc_winx20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax20     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx20       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx20;

/* Define the union u_isp_lblc_winx21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax21     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx21       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx21;

/* Define the union u_isp_lblc_winx22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax22     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx22       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx22;

/* Define the union u_isp_lblc_winx23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax23     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx23       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx23;

/* Define the union u_isp_lblc_winx24 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax24     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx24       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx24;

/* Define the union u_isp_lblc_winx25 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax25     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx25       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx25;

/* Define the union u_isp_lblc_winx26 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax26     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx26       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx26;

/* Define the union u_isp_lblc_winx27 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax27     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx27       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx27;

/* Define the union u_isp_lblc_winx28 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax28     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx28       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx28;

/* Define the union u_isp_lblc_winx29 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax29     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx29       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx29;

/* Define the union u_isp_lblc_winx30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax30     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx30       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx30;

/* Define the union u_isp_lblc_winx31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltax31     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invx31       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winx31;

/* Define the union u_isp_lblc_winy8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay8      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy8        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy8;

/* Define the union u_isp_lblc_winy9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay9      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy9        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy9;

/* Define the union u_isp_lblc_winy10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay10     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy10       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy10;

/* Define the union u_isp_lblc_winy11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay11     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy11       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy11;

/* Define the union u_isp_lblc_winy12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay12     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy12       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy12;

/* Define the union u_isp_lblc_winy13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay13     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy13       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy13;

/* Define the union u_isp_lblc_winy14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay14     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy14       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy14;

/* Define the union u_isp_lblc_winy15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_deltay15     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_lblc_invy15       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_winy15;

/* Define the union u_isp_lblc_blc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_offset_r     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 2; /* [15..14]  */
        unsigned int    isp_lblc_offset_gr    : 14; /* [29..16]  */
        unsigned int    reserved_1            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_blc0;

/* Define the union u_isp_lblc_blc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lblc_offset_b     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 2; /* [15..14]  */
        unsigned int    isp_lblc_offset_gb    : 14; /* [29..16]  */
        unsigned int    reserved_1            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lblc_blc1;

/* Define the union u_isp_lblc_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_lblc_roi_y : 13; /* [12..0]  */
        unsigned int isp_lblc_roi_x : 13; /* [25..13]  */
        unsigned int reserved_0 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_lblc_roi_cfg0;

/* Define the union u_isp_lblc_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_lblc_roi_height : 13; /* [12..0]  */
        unsigned int isp_lblc_roi_width : 13;  /* [25..13]  */
        unsigned int reserved_0 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_lblc_roi_cfg1;

/* Define the union u_isp_dg1_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg1_en_in         : 1; /* [0]  */
        unsigned int    isp_dg1_en_out        : 1; /* [1]  */
        unsigned int    isp_dg1_en_comp       : 1; /* [2]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_blc_cfg;

/* Define the union u_isp_dg1_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_version;
/* Define the union u_isp_dg1_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg1_grgain        : 16; /* [15..0]  */
        unsigned int    isp_dg1_rgain         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_gain1;

/* Define the union u_isp_dg1_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg1_gbgain        : 16; /* [15..0]  */
        unsigned int    isp_dg1_bgain         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_gain2;

/* Define the union u_isp_dg1_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg1_ofsgr         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dg1_ofsr          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_blc_offset1;

/* Define the union u_isp_dg1_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg1_ofsgb         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dg1_ofsb          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_blc_offset2;

/* Define the union u_isp_dg1_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg1_clip_value    : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg1_clip_value;

/* Define the union u_isp_dg1_blc_offset_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg1_ofs_shift : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;       /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg1_blc_offset_shift;

/* Define the union u_isp_dg1_real_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg1_real_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_dg1_real_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg1_real_blc_offset1;

/* Define the union u_isp_dg1_real_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg1_real_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_dg1_real_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg1_real_blc_offset2;

/* Define the union u_isp_dg1_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg1_roi_y : 13; /* [12..0]  */
        unsigned int isp_dg1_roi_x : 13; /* [25..13]  */
        unsigned int isp_dg1_roi_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;     /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg1_roi_cfg0;

/* Define the union u_isp_dg1_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg1_roi_height : 13; /* [12..0]  */
        unsigned int isp_dg1_roi_width : 13;  /* [25..13]  */
        unsigned int reserved_0 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg1_roi_cfg1;

/* Define the union u_isp_dg2_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_en_in         : 1; /* [0]  */
        unsigned int    isp_dg2_en_out        : 1; /* [1]  */
        unsigned int    isp_dg2_en_comp       : 1; /* [2]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_blc_cfg;

/* Define the union u_isp_dg2_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_version;
/* Define the union u_isp_dg2_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_grgain        : 16; /* [15..0]  */
        unsigned int    isp_dg2_rgain         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_gain1;

/* Define the union u_isp_dg2_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_gbgain        : 16; /* [15..0]  */
        unsigned int    isp_dg2_bgain         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_gain2;

/* Define the union u_isp_dg2_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_ofsgr         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dg2_ofsr          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_blc_offset1;

/* Define the union u_isp_dg2_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_ofsgb         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dg2_ofsb          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_blc_offset2;

/* Define the union u_isp_dg2_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_clip_value    : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg2_clip_value;


/* Define the union u_isp_dg2_blc_offset_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_ofs_shift : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;       /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_blc_offset_shift;

/* Define the union u_isp_dg2_real_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_real_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_dg2_real_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_real_blc_offset1;

/* Define the union u_isp_dg2_real_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_real_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_dg2_real_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_real_blc_offset2;

/* Define the union u_isp_dg2_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_roi_y : 13; /* [12..0]  */
        unsigned int isp_dg2_roi_x : 13; /* [25..13]  */
        unsigned int isp_dg2_roi_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;     /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_roi_cfg0;

/* Define the union u_isp_dg2_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_roi_height : 13; /* [12..0]  */
        unsigned int isp_dg2_roi_width : 13;  /* [25..13]  */
        unsigned int reserved_0 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_roi_cfg1;

/* Define the union u_isp_wb1_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_en_in         : 1; /* [0]  */
        unsigned int    isp_wb1_en_out        : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_blc_cfg;

/* Define the union u_isp_wb1_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_version;
/* Define the union u_isp_wb1_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_grgain        : 16; /* [15..0]  */
        unsigned int    isp_wb1_rgain         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_gain1;

/* Define the union u_isp_wb1_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_gbgain        : 16; /* [15..0]  */
        unsigned int    isp_wb1_bgain         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_gain2;

/* Define the union u_isp_wb1_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_ofsgr         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_wb1_ofsr          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_blc_offset1;

/* Define the union u_isp_wb1_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_ofsgb         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_wb1_ofsb          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_blc_offset2;

/* Define the union u_isp_wb1_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_clip_value    : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb1_clip_value;

/* Define the union u_isp_ae_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_version         : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_version;
/* Define the union u_isp_ae_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_hnum           : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_ae_vnum           : 5; /* [12..8]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_zone;

/* Define the union u_isp_ae_skip_crg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_skip_x         : 3; /* [2..0]  */
        unsigned int    isp_ae_offset_x       : 1; /* [3]  */
        unsigned int    isp_ae_skip_y         : 3; /* [6..4]  */
        unsigned int    isp_ae_offset_y       : 1; /* [7]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_skip_crg;

/* Define the union u_isp_ae_total_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_total_pixels   : 27; /* [26..0]  */
        unsigned int    reserved_0            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_stat;

/* Define the union u_isp_ae_count_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_count_pixels   : 31; /* [30..0]  */
        unsigned int    reserved_0            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_count_stat;

/* Define the union u_isp_ae_total_r_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_r_aver    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_r_aver;
/* Define the union u_isp_ae_total_gr_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gr_aver   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_gr_aver;
/* Define the union u_isp_ae_total_gb_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gb_aver   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_gb_aver;
/* Define the union u_isp_ae_total_b_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_b_aver    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_b_aver;
/* Define the union u_isp_ae_hist_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_high       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist_high;
/* Define the union u_isp_ae_bitmove */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_bitmove        : 5; /* [4..0]  */
        unsigned int    isp_ae_hist_gamma_mode : 2; /* [6..5]  */
        unsigned int    isp_ae_aver_gamma_mode : 2; /* [8..7]  */
        unsigned int    isp_ae_blc_en         : 1; /* [9]  */
        unsigned int    isp_ae_gamma_limit    : 4; /* [13..10]  */
        unsigned int    isp_ae_fourplanemode  : 1; /* [14]  */
        unsigned int    reserved_0            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_bitmove;

/* Define the union u_isp_ae_offset_r_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_offset_r       : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_ae_offset_gr      : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_offset_r_gr;

/* Define the union u_isp_ae_offset_gb_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_offset_gb      : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_ae_offset_b       : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_offset_gb_b;

/* Define the union u_isp_ae_lut_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_lut_update     : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_lut_update;

/* Define the union u_isp_ae_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_stt_size       : 16; /* [15..0]  */
        unsigned int    isp_ae_stt_bst        : 4; /* [19..16]  */
        unsigned int    isp_ae_stt_en         : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_stt_bst;

/* Define the union u_isp_ae_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_crop_pos_x     : 16; /* [15..0]  */
        unsigned int    isp_ae_crop_pos_y     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_crop_pos;

/* Define the union u_isp_ae_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_crop_out_width : 16; /* [15..0]  */
        unsigned int    isp_ae_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_crop_outsize;

/* Define the union u_isp_ae_hist_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist_waddr;
/* Define the union u_isp_ae_hist_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist_wdata;
/* Define the union u_isp_ae_hist_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist_raddr;
/* Define the union u_isp_ae_hist_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_rdata      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist_rdata;
/* Define the union u_isp_ae_aver_r_gr_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_r_gr_waddr;
/* Define the union u_isp_ae_aver_r_gr_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_r_gr_wdata;
/* Define the union u_isp_ae_aver_r_gr_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_r_gr_raddr;
/* Define the union u_isp_ae_aver_r_gr_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_r_gr_rdata;
/* Define the union u_isp_ae_aver_gb_b_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_gb_b_waddr;
/* Define the union u_isp_ae_aver_gb_b_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_gb_b_wdata;
/* Define the union u_isp_ae_aver_gb_b_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_gb_b_raddr;
/* Define the union u_isp_ae_aver_gb_b_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_gb_b_rdata;
/* Define the union u_isp_ae_weight_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_wei_waddr       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_weight_waddr;
/* Define the union u_isp_ae_weight_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_wei_wdata       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_weight_wdata;
/* Define the union u_isp_ae_weight_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_weight_raddr;
/* Define the union u_isp_ae_weight_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_weight_rdata;
/* Define the union u_isp_af_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_af_iir0_en0       : 1; /* [1]  */
        unsigned int    isp_af_iir0_en1       : 1; /* [2]  */
        unsigned int    isp_af_iir0_en2       : 1; /* [3]  */
        unsigned int    isp_af_iir1_en0       : 1; /* [4]  */
        unsigned int    isp_af_iir1_en1       : 1; /* [5]  */
        unsigned int    isp_af_iir1_en2       : 1; /* [6]  */
        unsigned int    isp_af_peak_mode      : 1; /* [7]  */
        unsigned int    isp_af_squ_mode       : 1; /* [8]  */
        unsigned int    isp_af_offset_en      : 1; /* [9]  */
        unsigned int    isp_af_crop_en        : 1; /* [10]  */
        unsigned int    isp_af_lpf_en         : 1; /* [11]  */
        unsigned int    isp_af_mean_en        : 1; /* [12]  */
        unsigned int    reserved_1            : 1; /* [13]  */
        unsigned int    isp_af_raw_mode       : 1; /* [14]  */
        unsigned int    isp_af_bayer_mode     : 2; /* [16..15]  */
        unsigned int    isp_af_iir0_ds_en     : 1; /* [17]  */
        unsigned int    isp_af_iir1_ds_en     : 1; /* [18]  */
        unsigned int    isp_af_fir0_lpf_en    : 1; /* [19]  */
        unsigned int    isp_af_fir1_lpf_en    : 1; /* [20]  */
        unsigned int    isp_af_iir0_ldg_en    : 1; /* [21]  */
        unsigned int    isp_af_iir1_ldg_en    : 1; /* [22]  */
        unsigned int    isp_af_fir0_ldg_en    : 1; /* [23]  */
        unsigned int    isp_af_fir1_ldg_en    : 1; /* [24]  */
        unsigned int    isp_af_gamma          : 1; /* [25]  */
        unsigned int    reserved_2            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_cfg;

/* Define the union u_isp_af_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_version         : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_version;
/* Define the union u_isp_af_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_hnum           : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_af_vnum           : 5; /* [12..8]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_zone;

/* Define the union u_isp_af_crop_start */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_pos_x          : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_af_pos_y          : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_crop_start;

/* Define the union u_isp_af_crop_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_crop_hsize     : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_af_crop_vsize     : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_crop_size;

/* Define the union u_isp_af_mean_thres */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_mean_thres     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_mean_thres;

/* Define the union u_isp_af_iirg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg0_0        : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_af_iirg0_1        : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg0;

/* Define the union u_isp_af_iirg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg1_0        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_af_iirg1_1        : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg1;

/* Define the union u_isp_af_iirg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg2_0        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_af_iirg2_1        : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg2;

/* Define the union u_isp_af_iirg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg3_0        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_af_iirg3_1        : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg3;

/* Define the union u_isp_af_iirg4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg4_0        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_af_iirg4_1        : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg4;

/* Define the union u_isp_af_iirg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg5_0        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_af_iirg5_1        : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg5;

/* Define the union u_isp_af_iirg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg6_0        : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_af_iirg6_1        : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirg6;

/* Define the union u_isp_af_iirpl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirplg_0       : 8; /* [7..0]  */
        unsigned int    isp_af_iirpls_0       : 3; /* [10..8]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_af_iirplg_1       : 8; /* [23..16]  */
        unsigned int    isp_af_iirpls_1       : 3; /* [26..24]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirpl;

/* Define the union u_isp_af_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirshift0_0    : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_af_iirshift0_1    : 3; /* [6..4]  */
        unsigned int    reserved_1            : 1; /* [7]  */
        unsigned int    isp_af_iirshift0_2    : 3; /* [10..8]  */
        unsigned int    reserved_2            : 1; /* [11]  */
        unsigned int    isp_af_iirshift0_3    : 3; /* [14..12]  */
        unsigned int    reserved_3            : 1; /* [15]  */
        unsigned int    isp_af_iirshift1_0    : 3; /* [18..16]  */
        unsigned int    reserved_4            : 1; /* [19]  */
        unsigned int    isp_af_iirshift1_1    : 3; /* [22..20]  */
        unsigned int    reserved_5            : 1; /* [23]  */
        unsigned int    isp_af_iirshift1_2    : 3; /* [26..24]  */
        unsigned int    reserved_6            : 1; /* [27]  */
        unsigned int    isp_af_iirshift1_3    : 3; /* [30..28]  */
        unsigned int    reserved_7            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_shift;

/* Define the union u_isp_af_firh0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh0_0        : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_af_firh0_1        : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firh0;

/* Define the union u_isp_af_firh1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh1_0        : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_af_firh1_1        : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firh1;

/* Define the union u_isp_af_firh2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh2_0        : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_af_firh2_1        : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firh2;

/* Define the union u_isp_af_firh3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh3_0        : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_af_firh3_1        : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firh3;

/* Define the union u_isp_af_firh4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh4_0        : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_af_firh4_1        : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firh4;

/* Define the union u_isp_af_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_stt_size       : 16; /* [15..0]  */
        unsigned int    isp_af_stt_bst        : 4; /* [19..16]  */
        unsigned int    isp_af_stt_en         : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_stt_bst;

/* Define the union u_isp_af_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_stt_info       : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_af_stt_clr        : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_stt_abn;

/* Define the union u_isp_af_acc_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_acc_shift0_h   : 4; /* [3..0]  */
        unsigned int    isp_af_acc_shift1_h   : 4; /* [7..4]  */
        unsigned int    isp_af_acc_shift0_v   : 4; /* [11..8]  */
        unsigned int    isp_af_acc_shift1_v   : 4; /* [15..12]  */
        unsigned int    isp_af_acc_shift_y    : 4; /* [19..16]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_acc_shift;

/* Define the union u_isp_af_cnt_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_cnt_shift0_h   : 4; /* [3..0]  */
        unsigned int    isp_af_cnt_shift1_h   : 4; /* [7..4]  */
        unsigned int    isp_af_cnt_shift0_v   : 4; /* [11..8]  */
        unsigned int    isp_af_cnt_shift1_v   : 4; /* [15..12]  */
        unsigned int    isp_af_cnt_shift_y    : 4; /* [19..16]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_cnt_shift;

/* Define the union u_isp_af_stat_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_stat_raddr;
/* Define the union u_isp_af_stat_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_rdata      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_stat_rdata;
/* Define the union u_isp_af_iirthre */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_thre0_l    : 8; /* [7..0]  */
        unsigned int    isp_af_iir_thre0_h    : 8; /* [15..8]  */
        unsigned int    isp_af_iir_thre1_l    : 8; /* [23..16]  */
        unsigned int    isp_af_iir_thre1_h    : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirthre;

/* Define the union u_isp_af_iirgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_gain0_l    : 8; /* [7..0]  */
        unsigned int    isp_af_iir_gain0_h    : 8; /* [15..8]  */
        unsigned int    isp_af_iir_gain1_l    : 8; /* [23..16]  */
        unsigned int    isp_af_iir_gain1_h    : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirgain;

/* Define the union u_isp_af_iirslope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_slope0_l   : 4; /* [3..0]  */
        unsigned int    isp_af_iir_slope0_h   : 4; /* [7..4]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_af_iir_slope1_l   : 4; /* [19..16]  */
        unsigned int    isp_af_iir_slope1_h   : 4; /* [23..20]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirslope;

/* Define the union u_isp_af_iirdilate */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_dilate0    : 3; /* [2..0]  */
        unsigned int    reserved_0            : 5; /* [7..3]  */
        unsigned int    isp_af_iir_dilate1    : 3; /* [10..8]  */
        unsigned int    reserved_1            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirdilate;

/* Define the union u_isp_af_firthre */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_thre0_l    : 8; /* [7..0]  */
        unsigned int    isp_af_fir_thre0_h    : 8; /* [15..8]  */
        unsigned int    isp_af_fir_thre1_l    : 8; /* [23..16]  */
        unsigned int    isp_af_fir_thre1_h    : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firthre;

/* Define the union u_isp_af_firgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_gain0_l    : 8; /* [7..0]  */
        unsigned int    isp_af_fir_gain0_h    : 8; /* [15..8]  */
        unsigned int    isp_af_fir_gain1_l    : 8; /* [23..16]  */
        unsigned int    isp_af_fir_gain1_h    : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firgain;

/* Define the union u_isp_af_firslope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_slope0_l   : 4; /* [3..0]  */
        unsigned int    isp_af_fir_slope0_h   : 4; /* [7..4]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_af_fir_slope1_l   : 4; /* [19..16]  */
        unsigned int    isp_af_fir_slope1_h   : 4; /* [23..20]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firslope;

/* Define the union u_isp_af_iirthre_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_thre0_c    : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_af_iir_thre1_c    : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirthre_coring;

/* Define the union u_isp_af_iirpeak_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_peak0_c    : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_af_iir_peak1_c    : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirpeak_coring;

/* Define the union u_isp_af_iirslope_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_slope0_c   : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_af_iir_slope1_c   : 4; /* [11..8]  */
        unsigned int    reserved_1            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_iirslope_coring;

/* Define the union u_isp_af_firthre_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_thre0_c    : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_af_fir_thre1_c    : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firthre_coring;

/* Define the union u_isp_af_firpeak_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_peak0_c    : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_af_fir_peak1_c    : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firpeak_coring;

/* Define the union u_isp_af_firslope_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_slope0_c   : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_af_fir_slope1_c   : 4; /* [11..8]  */
        unsigned int    reserved_1            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_firslope_coring;

/* Define the union u_isp_af_highlight */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_highlight        : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_highlight;

/* Define the union u_isp_af_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_offset_gr      : 14; /* [13..0]  */
        unsigned int    reserved_0            : 2; /* [15..14]  */
        unsigned int    isp_af_offset_gb      : 14; /* [29..16]  */
        unsigned int    reserved_1            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_offset;

/* Define the union u_isp_fpn_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 10; /* [9..0]  */
        unsigned int    isp_fpn_calib_offline : 1; /* [10]  */
        unsigned int    reserved_1            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_cfg;

/* Define the union u_isp_fpn_calib_start */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_calib_start   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_calib_start;

/* Define the union u_isp_fpn_corr_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_correct0_en   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_corr_cfg;

/* Define the union u_isp_fpn_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_busy          : 1; /* [0]  */
        unsigned int    reserved_0            : 7; /* [7..1]  */
        unsigned int    isp_fpn_vcnt          : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_fpn_hcnt          : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_stat;

/* Define the union u_isp_fpn_white_level */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_white_level   : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_white_level;

/* Define the union u_isp_fpn_divcoef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_divcoef       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_divcoef;

/* Define the union u_isp_fpn_framelog2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_framelog2     : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_framelog2;

/* Define the union u_isp_fpn_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum0           : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_sum0;
/* Define the union u_isp_fpn_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum1           : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_sum1;
/* Define the union u_isp_fpn_corr0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_offset0       : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_fpn_strength0     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_corr0;

/* Define the union u_isp_fpn_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_shift         : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_fpn_in_shift      : 4; /* [11..8]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    isp_fpn_out_shift     : 4; /* [19..16]  */
        unsigned int    reserved_2            : 4; /* [23..20]  */
        unsigned int    isp_fpn_frame_calib_shift : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_shift;

/* Define the union u_isp_fpn_max_o */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_max_o         : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_max_o;

/* Define the union u_isp_fpn_overflowthr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_overflowthr   : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_overflowthr;

/* Define the union u_isp_fpn_black_frame_out_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_data0_out_sel : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_black_frame_out_sel;

/* Define the union u_isp_fpn_overlap_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_overlap_width_l : 16; /* [15..0]  */
        unsigned int    isp_fpn_overlap_width_r : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_overlap_cfg;

/* Define the union u_isp_fpn_calib_offline_set */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_calib_offline_set : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_calib_offline_set;

/* Define the union u_isp_fpn_calib_stat_clear */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_calib_stat_clear : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_calib_stat_clear;

/* Define the union u_isp_fpn_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_roi_y         : 13; /* [12..0]  */
        unsigned int    isp_fpn_roi_x         : 13; /* [25..13]  */
        unsigned int    isp_fpn_roi_en        : 1; /* [26]  */
        unsigned int    reserved_0            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_roi_cfg0;

/* Define the union u_isp_fpn_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_roi_height    : 13; /* [12..0]  */
        unsigned int    isp_fpn_roi_width     : 13; /* [25..13]  */
        unsigned int    reserved_0            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_roi_cfg1;

/* Define the union u_isp_crop_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_crop_version       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crop_version;
/* Define the union u_isp_crop_sizeout */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crop_width_out    : 16; /* [15..0]  */
        unsigned int    isp_crop_height_out   : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crop_sizeout;

/* Define the union u_isp_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crop_pos_x        : 16; /* [15..0]  */
        unsigned int    isp_crop_pos_y        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crop_pos;

/* Define the union u_isp_blc_dynamic_reg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_dynamic_bis2x2pattern : 1; /* [0]  */
        unsigned int    isp_blc_dynamic_raw_array : 4; /* [4..1]  */
        unsigned int    isp_blc_dynamic_dp_stat_en : 1; /* [5]  */
        unsigned int    isp_blc_dynamic_bfilter_en : 1; /* [6]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_dynamic_reg0;

/* Define the union u_isp_blc_dynamic_reg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc_dynamic_tar1 : 16; /* [15..0]  */
        unsigned int isp_blc_dynamic_tar0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_dynamic_reg1;
/* Define the union u_isp_blc_dynamic_reg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_dynamic_start_xpos : 16; /* [15..0]  */
        unsigned int    isp_blc_dynamic_start_ypos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_dynamic_reg2;

/* Define the union u_isp_blc_dynamic_reg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_dynamic_end_xpos : 16; /* [15..0]  */
        unsigned int    isp_blc_dynamic_end_ypos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_dynamic_reg3;

/* Define the union u_isp_blc_dynamic_reg4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_dynamic_hot_pxl_thd : 16; /* [15..0]  */
        unsigned int    isp_blc_dynamic_dead_pxl_thd : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_dynamic_reg4;

/* Define the union u_isp_blc_dynamic_reg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_dynamic_value0 : 16; /* [15..0]  */
        unsigned int    isp_blc_dynamic_value1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_dynamic_reg5;

/* Define the union u_isp_blc_dynamic_reg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_dynamic_value2 : 16; /* [15..0]  */
        unsigned int    isp_blc_dynamic_value3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_dynamic_reg6;

/* Define the global struct */
typedef struct {
    volatile unsigned int           reserved_0[32]; /* 0x0~0x7c, reserved 32 * 4 bytes */
    volatile u_isp_fe_version       isp_fe_version; /* 0x0080 */
    volatile u_isp_fe_date          isp_fe_date; /* 0x0084 */
    volatile u_isp_fe_fpga_date     isp_fe_fpga_date; /* 0x0088 */
    volatile unsigned int           reserved_1; /* 0x008c */
    volatile u_isp_fe_module_pos    isp_fe_module_pos; /* 0x0090 */
    volatile u_isp_fe_fstart        isp_fe_fstart; /* 0x0094 */
    volatile unsigned int           reserved_2[2]; /* 0x0098~0x009c, reserved 2 * 4 bytes */
    volatile u_isp_fe_user_define0   isp_fe_user_define0; /* 0x00a0 */
    volatile u_isp_fe_user_define1   isp_fe_user_define1; /* 0x00a4 */
    volatile unsigned int           reserved_3[2]; /* 0x00a8~0x00ac, reserved 2 * 4 bytes */
    volatile u_isp_fe_startup       isp_fe_startup; /* 0x00b0 */
    volatile u_isp_fe_format        isp_fe_format; /* 0x00b4 */
    volatile unsigned int           reserved_85[2]; /* 0x00b8~0x00bc */
    volatile u_isp_glb_blc_cfg      isp_glb_blc_cfg; /* 0x00c0 */
    volatile unsigned int           reserved_86[3]; /* 0x00c4~0x00cc */
    volatile u_isp_blc_cfg10        isp_blc_cfg10; /* 0x00d0 */
    volatile u_isp_blc_cfg11        isp_blc_cfg11; /* 0x00d4 */
    volatile u_isp_blc_cfg20        isp_blc_cfg20; /* 0x00d8 */
    volatile u_isp_blc_cfg21        isp_blc_cfg21; /* 0x00dc */
    volatile unsigned int           reserved_4[4]; /* 0x00e0~0x00ec, reserved 14 * 4 bytes */
    volatile u_isp_fe_int           isp_fe_int; /* 0x00f0 */
    volatile u_isp_fe_int_state     isp_fe_int_state; /* 0x00f4 */
    volatile u_isp_fe_int_mask      isp_fe_int_mask; /* 0x00f8 */
    volatile unsigned int           reserved_5; /* 0x00fc */
    volatile u_isp_fe_ctrl          isp_fe_ctrl; /* 0x0100 */
    volatile unsigned int           reserved_6[3]; /* 0x0104~0x010c, reserved 3 * 4 bytes */
    volatile u_isp_fe_manual_update isp_fe_manual_update; /* 0x0110 */
    volatile u_isp_fe_adapter_cfg   isp_fe_adapter_cfg; /* 0x0114 */
    volatile u_isp_fe_out_size      isp_fe_out_size; /* 0x0118 */
    volatile unsigned int           reserved_7[30]; /* 0x011c~0x0190, reserved 30 * 4 bytes */
    volatile u_isp_fe_fstart_delay   isp_fe_fstart_delay; /* 0x0194 */
    volatile unsigned int           reserved_8[18]; /* 0x0198~0x01dc, reserved 18 * 4 bytes */
    volatile u_isp_fe_ctrl_f        isp_fe_ctrl_f; /* 0x01e0 */
    volatile u_isp_fe_ctrl_i        isp_fe_ctrl_i; /* 0x01e4 */
    volatile u_isp_fe_timing_cfg    isp_fe_timing_cfg; /* 0x01e8 */
    volatile u_isp_fe_reg_update    isp_fe_reg_update; /* 0x01ec */
    volatile u_isp_fe_size          isp_fe_size; /* 0x01f0 */
    volatile unsigned int           reserved_9[416]; /* 0x01f4~0x081c, reserved 416 * 4 bytes */
    volatile u_isp_fe_blk_size      isp_fe_blk_size; /* 0x0874 */
    volatile u_isp_fe_blk_hblank    isp_fe_blk_hblank; /* 0x0878 */
    volatile u_isp_fe_blk_vblank    isp_fe_blk_vblank; /* 0x087c */
    volatile unsigned int           reserved_10[547]; /* 0x0880~0x1108, reserved 537 * 4 bytes */
    volatile u_isp_blc1_version     isp_blc1_version; /* 0x110c */
    volatile u_isp_blc1_offset1     isp_blc1_offset1; /* 0x1110 */
    volatile u_isp_blc1_offset2     isp_blc1_offset2; /* 0x1114 */
    volatile u_isp_blc1_roi_cfg0             isp_blc1_roi_cfg0; /* 0x1118 */
    volatile u_isp_blc1_roi_cfg1             isp_blc1_roi_cfg1; /* 0x111c */
    volatile unsigned int                    reserved_11[312]; /* 0x1120~0x15fc, reserved 312 * 4 bytes */
    volatile u_isp_lblc_stt2lut_cfg          isp_lblc_stt2lut_cfg; /* 0x1600 */
    volatile u_isp_lblc_stt2lut_regnew       isp_lblc_stt2lut_regnew; /* 0x1604 */
    volatile u_isp_lblc_stt2lut_abn          isp_lblc_stt2lut_abn; /* 0x1608 */
    volatile unsigned int                    reserved_12; /* 0x160c */
    volatile u_isp_lblc_winnum               isp_lblc_winnum; /* 0x1610 */
    volatile u_isp_lblc_winx0                isp_lblc_winx0; /* 0x1614 */
    volatile u_isp_lblc_winx1                isp_lblc_winx1; /* 0x1618 */
    volatile u_isp_lblc_winx2                isp_lblc_winx2; /* 0x161c */
    volatile u_isp_lblc_winx3                isp_lblc_winx3; /* 0x1620 */
    volatile u_isp_lblc_winx4                isp_lblc_winx4; /* 0x1624 */
    volatile u_isp_lblc_winx5                isp_lblc_winx5; /* 0x1628 */
    volatile u_isp_lblc_winx6                isp_lblc_winx6; /* 0x162c */
    volatile u_isp_lblc_winx7                isp_lblc_winx7; /* 0x1630 */
    volatile u_isp_lblc_winx8                isp_lblc_winx8; /* 0x1634 */
    volatile u_isp_lblc_winx9                isp_lblc_winx9; /* 0x1638 */
    volatile u_isp_lblc_winx10               isp_lblc_winx10; /* 0x163c */
    volatile u_isp_lblc_winx11               isp_lblc_winx11; /* 0x1640 */
    volatile u_isp_lblc_winx12               isp_lblc_winx12; /* 0x1644 */
    volatile u_isp_lblc_winx13               isp_lblc_winx13; /* 0x1648 */
    volatile u_isp_lblc_winx14               isp_lblc_winx14; /* 0x164c */
    volatile u_isp_lblc_winx15               isp_lblc_winx15; /* 0x1650 */
    volatile u_isp_lblc_winy0                isp_lblc_winy0; /* 0x1654 */
    volatile u_isp_lblc_winy1                isp_lblc_winy1; /* 0x1658 */
    volatile u_isp_lblc_winy2                isp_lblc_winy2; /* 0x165c */
    volatile u_isp_lblc_winy3                isp_lblc_winy3; /* 0x1660 */
    volatile u_isp_lblc_winy4                isp_lblc_winy4; /* 0x1664 */
    volatile u_isp_lblc_winy5                isp_lblc_winy5; /* 0x1668 */
    volatile u_isp_lblc_winy6                isp_lblc_winy6; /* 0x166c */
    volatile u_isp_lblc_winy7                isp_lblc_winy7; /* 0x1670 */
    volatile u_isp_lblc_width_offset         isp_lblc_width_offset; /* 0x1674 */
    volatile u_isp_lblc_ctrl0                isp_lblc_ctrl0; /* 0x1678 */
    volatile unsigned int                    reserved_13; /* 0x167c */
    volatile u_isp_lblc_rgain_waddr          isp_lblc_rgain_waddr; /* 0x1680 */
    volatile u_isp_lblc_rgain_wdata          isp_lblc_rgain_wdata; /* 0x1684 */
    volatile u_isp_lblc_rgain_raddr          isp_lblc_rgain_raddr; /* 0x1688 */
    volatile u_isp_lblc_rgain_rdata          isp_lblc_rgain_rdata; /* 0x168c */
    volatile u_isp_lblc_grgain_waddr         isp_lblc_grgain_waddr; /* 0x1690 */
    volatile u_isp_lblc_grgain_wdata         isp_lblc_grgain_wdata; /* 0x1694 */
    volatile u_isp_lblc_grgain_raddr         isp_lblc_grgain_raddr; /* 0x1698 */
    volatile u_isp_lblc_grgain_rdata         isp_lblc_grgain_rdata; /* 0x169c */
    volatile u_isp_lblc_bgain_waddr          isp_lblc_bgain_waddr; /* 0x16a0 */
    volatile u_isp_lblc_bgain_wdata          isp_lblc_bgain_wdata; /* 0x16a4 */
    volatile u_isp_lblc_bgain_raddr          isp_lblc_bgain_raddr; /* 0x16a8 */
    volatile u_isp_lblc_bgain_rdata          isp_lblc_bgain_rdata; /* 0x16ac */
    volatile u_isp_lblc_gbgain_waddr         isp_lblc_gbgain_waddr; /* 0x16b0 */
    volatile u_isp_lblc_gbgain_wdata         isp_lblc_gbgain_wdata; /* 0x16b4 */
    volatile u_isp_lblc_gbgain_raddr         isp_lblc_gbgain_raddr; /* 0x16b8 */
    volatile u_isp_lblc_gbgain_rdata         isp_lblc_gbgain_rdata; /* 0x16bc */
    volatile unsigned int                    reserved_15[16]; /* 0x16c0~0x16fc */
    volatile u_isp_lblc_winx16               isp_lblc_winx16; /* 0x1700 */
    volatile u_isp_lblc_winx17               isp_lblc_winx17; /* 0x1704 */
    volatile u_isp_lblc_winx18               isp_lblc_winx18; /* 0x1708 */
    volatile u_isp_lblc_winx19               isp_lblc_winx19; /* 0x170c */
    volatile u_isp_lblc_winx20               isp_lblc_winx20; /* 0x1710 */
    volatile u_isp_lblc_winx21               isp_lblc_winx21; /* 0x1714 */
    volatile u_isp_lblc_winx22               isp_lblc_winx22; /* 0x1718 */
    volatile u_isp_lblc_winx23               isp_lblc_winx23; /* 0x171c */
    volatile u_isp_lblc_winx24               isp_lblc_winx24; /* 0x1720 */
    volatile u_isp_lblc_winx25               isp_lblc_winx25; /* 0x1724 */
    volatile u_isp_lblc_winx26               isp_lblc_winx26; /* 0x1728 */
    volatile u_isp_lblc_winx27               isp_lblc_winx27; /* 0x172c */
    volatile u_isp_lblc_winx28               isp_lblc_winx28; /* 0x1730 */
    volatile u_isp_lblc_winx29               isp_lblc_winx29; /* 0x1734 */
    volatile u_isp_lblc_winx30               isp_lblc_winx30; /* 0x1738 */
    volatile u_isp_lblc_winx31               isp_lblc_winx31; /* 0x173c */
    volatile u_isp_lblc_winy8                isp_lblc_winy8; /* 0x1740 */
    volatile u_isp_lblc_winy9                isp_lblc_winy9; /* 0x1744 */
    volatile u_isp_lblc_winy10               isp_lblc_winy10; /* 0x1748 */
    volatile u_isp_lblc_winy11               isp_lblc_winy11; /* 0x174c */
    volatile u_isp_lblc_winy12               isp_lblc_winy12; /* 0x1750 */
    volatile u_isp_lblc_winy13               isp_lblc_winy13; /* 0x1754 */
    volatile u_isp_lblc_winy14               isp_lblc_winy14; /* 0x1758 */
    volatile u_isp_lblc_winy15               isp_lblc_winy15; /* 0x175c */
    volatile u_isp_lblc_blc0                 isp_lblc_blc0; /* 0x1760 */
    volatile u_isp_lblc_blc1                 isp_lblc_blc1; /* 0x1764 */
    volatile u_isp_lblc_roi_cfg0             isp_lblc_roi_cfg0; /* 0x1768 */
    volatile u_isp_lblc_roi_cfg1             isp_lblc_roi_cfg1; /* 0x176c */
    volatile unsigned int                    reserved_16[293]; /* 0x1770~0x1c00 */
    volatile u_isp_dg1_blc_cfg               isp_dg1_blc_cfg; /* 0x1c04 */
    volatile unsigned int                    reserved_17; /* 0x1c08 */
    volatile u_isp_dg1_version               isp_dg1_version; /* 0x1c0c */
    volatile u_isp_dg1_gain1                 isp_dg1_gain1; /* 0x1c10 */
    volatile u_isp_dg1_gain2                 isp_dg1_gain2; /* 0x1c14 */
    volatile u_isp_dg1_blc_offset1           isp_dg1_blc_offset1; /* 0x1c18 */
    volatile u_isp_dg1_blc_offset2           isp_dg1_blc_offset2; /* 0x1c1c */
    volatile u_isp_dg1_clip_value            isp_dg1_clip_value; /* 0x1c20 */
    volatile u_isp_dg1_blc_offset_shift      isp_dg1_blc_offset_shift; /* 0x1c24 */
    volatile u_isp_dg1_real_blc_offset1      isp_dg1_real_blc_offset1; /* 0x1c28 */
    volatile u_isp_dg1_real_blc_offset2      isp_dg1_real_blc_offset2; /* 0x1c2c */
    volatile u_isp_dg1_roi_cfg0              isp_dg1_roi_cfg0; /* 0x1c30 */
    volatile u_isp_dg1_roi_cfg1              isp_dg1_roi_cfg1; /* 0x1c34 */
    volatile unsigned int           reserved_18[51]; /* 0x1c38~0x1d00, reserved 51 * 4 bytes */
    volatile u_isp_dg2_blc_cfg      isp_dg2_blc_cfg; /* 0x1d04 */
    volatile unsigned int           reserved_19; /* 0x1d08 */
    volatile u_isp_dg2_version      isp_dg2_version; /* 0x1d0c */
    volatile u_isp_dg2_gain1        isp_dg2_gain1; /* 0x1d10 */
    volatile u_isp_dg2_gain2        isp_dg2_gain2; /* 0x1d14 */
    volatile u_isp_dg2_blc_offset1   isp_dg2_blc_offset1; /* 0x1d18 */
    volatile u_isp_dg2_blc_offset2   isp_dg2_blc_offset2; /* 0x1d1c */
    volatile u_isp_dg2_clip_value   isp_dg2_clip_value; /* 0x1d20 */
    volatile u_isp_dg2_blc_offset_shift      isp_dg2_blc_offset_shift; /* 0x1d24 */
    volatile u_isp_dg2_real_blc_offset1      isp_dg2_real_blc_offset1; /* 0x1d28 */
    volatile u_isp_dg2_real_blc_offset2      isp_dg2_real_blc_offset2; /* 0x1d2c */
    volatile u_isp_dg2_roi_cfg0              isp_dg2_roi_cfg0; /* 0x1d30 */
    volatile u_isp_dg2_roi_cfg1              isp_dg2_roi_cfg1; /* 0x1d34 */
    volatile unsigned int           reserved_20[115]; /* 0x1d38~0x1f00, reserved 115 * 4 bytes */
    volatile u_isp_wb1_blc_cfg      isp_wb1_blc_cfg; /* 0x1f04 */
    volatile unsigned int           reserved_21; /* 0x1f08 */
    volatile u_isp_wb1_version      isp_wb1_version; /* 0x1f0c */
    volatile u_isp_wb1_gain1        isp_wb1_gain1; /* 0x1f10 */
    volatile u_isp_wb1_gain2        isp_wb1_gain2; /* 0x1f14 */
    volatile u_isp_wb1_blc_offset1   isp_wb1_blc_offset1; /* 0x1f18 */
    volatile u_isp_wb1_blc_offset2   isp_wb1_blc_offset2; /* 0x1f1c */
    volatile u_isp_wb1_clip_value   isp_wb1_clip_value; /* 0x1f20 */
    volatile unsigned int           reserved_22[58]; /* 0x1f24~0x2008, reserved 58 * 4 bytes */
    volatile u_isp_ae_version       isp_ae_version; /* 0x200c */
    volatile u_isp_ae_zone          isp_ae_zone; /* 0x2010 */
    volatile u_isp_ae_skip_crg      isp_ae_skip_crg; /* 0x2014 */
    volatile u_isp_ae_total_stat    isp_ae_total_stat; /* 0x2018 */
    volatile u_isp_ae_count_stat    isp_ae_count_stat; /* 0x201c */
    volatile u_isp_ae_total_r_aver   isp_ae_total_r_aver; /* 0x2020 */
    volatile u_isp_ae_total_gr_aver   isp_ae_total_gr_aver; /* 0x2024 */
    volatile u_isp_ae_total_gb_aver   isp_ae_total_gb_aver; /* 0x2028 */
    volatile u_isp_ae_total_b_aver   isp_ae_total_b_aver; /* 0x202c */
    volatile u_isp_ae_hist_high     isp_ae_hist_high; /* 0x2030 */
    volatile unsigned int           reserved_23[3]; /* 0x2034~0x203c, reserved 3 * 4 bytes */
    volatile u_isp_ae_bitmove       isp_ae_bitmove; /* 0x2040 */
    volatile u_isp_ae_offset_r_gr   isp_ae_offset_r_gr; /* 0x2044 */
    volatile unsigned int                    reserved_24; /* 0x2048 */
    volatile u_isp_ae_offset_gb_b            isp_ae_offset_gb_b; /* 0x204c */
    volatile unsigned int           reserved_25[5]; /* 0x2050~0x2060, reserved 5 * 4 bytes */
    volatile u_isp_ae_lut_update    isp_ae_lut_update; /* 0x2064 */
    volatile u_isp_ae_stt_bst       isp_ae_stt_bst; /* 0x2068 */
    volatile unsigned int           reserved_26[2]; /* 0x206c~0x2070, reserved 2 * 4 bytes */
    volatile u_isp_ae_crop_pos      isp_ae_crop_pos; /* 0x2074 */
    volatile u_isp_ae_crop_outsize   isp_ae_crop_outsize; /* 0x2078 */
    volatile unsigned int           reserved_27; /* 0x207c */
    volatile u_isp_ae_hist_waddr    isp_ae_hist_waddr; /* 0x2080 */
    volatile u_isp_ae_hist_wdata    isp_ae_hist_wdata; /* 0x2084 */
    volatile u_isp_ae_hist_raddr    isp_ae_hist_raddr; /* 0x2088 */
    volatile u_isp_ae_hist_rdata    isp_ae_hist_rdata; /* 0x208c */
    volatile u_isp_ae_aver_r_gr_waddr   isp_ae_aver_r_gr_waddr; /* 0x2090 */
    volatile u_isp_ae_aver_r_gr_wdata   isp_ae_aver_r_gr_wdata; /* 0x2094 */
    volatile u_isp_ae_aver_r_gr_raddr   isp_ae_aver_r_gr_raddr; /* 0x2098 */
    volatile u_isp_ae_aver_r_gr_rdata   isp_ae_aver_r_gr_rdata; /* 0x209c */
    volatile u_isp_ae_aver_gb_b_waddr   isp_ae_aver_gb_b_waddr; /* 0x20a0 */
    volatile u_isp_ae_aver_gb_b_wdata   isp_ae_aver_gb_b_wdata; /* 0x20a4 */
    volatile u_isp_ae_aver_gb_b_raddr   isp_ae_aver_gb_b_raddr; /* 0x20a8 */
    volatile u_isp_ae_aver_gb_b_rdata   isp_ae_aver_gb_b_rdata; /* 0x20ac */
    volatile u_isp_ae_weight_waddr   isp_ae_weight_waddr; /* 0x20b0 */
    volatile u_isp_ae_weight_wdata   isp_ae_weight_wdata; /* 0x20b4 */
    volatile u_isp_ae_weight_raddr   isp_ae_weight_raddr; /* 0x20b8 */
    volatile u_isp_ae_weight_rdata   isp_ae_weight_rdata; /* 0x20bc */
    volatile unsigned int           reserved_28[80]; /* 0x20c0~0x21fc, reserved 80 * 4 bytes */
    volatile u_isp_af_cfg           isp_af_cfg; /* 0x2200 */
    volatile unsigned int           reserved_29[2]; /* 0x2204~0x2208, reserved 2 * 4 bytes */
    volatile u_isp_af_version       isp_af_version; /* 0x220c */
    volatile u_isp_af_zone          isp_af_zone; /* 0x2210 */
    volatile u_isp_af_crop_start    isp_af_crop_start; /* 0x2214 */
    volatile u_isp_af_crop_size     isp_af_crop_size; /* 0x2218 */
    volatile u_isp_af_mean_thres    isp_af_mean_thres; /* 0x221c */
    volatile u_isp_af_iirg0         isp_af_iirg0; /* 0x2220 */
    volatile u_isp_af_iirg1         isp_af_iirg1; /* 0x2224 */
    volatile u_isp_af_iirg2         isp_af_iirg2; /* 0x2228 */
    volatile u_isp_af_iirg3         isp_af_iirg3; /* 0x222c */
    volatile u_isp_af_iirg4         isp_af_iirg4; /* 0x2230 */
    volatile u_isp_af_iirg5         isp_af_iirg5; /* 0x2234 */
    volatile u_isp_af_iirg6         isp_af_iirg6; /* 0x2238 */
    volatile u_isp_af_iirpl         isp_af_iirpl; /* 0x223c */
    volatile u_isp_af_shift         isp_af_shift; /* 0x2240 */
    volatile unsigned int           reserved_30[3]; /* 0x2244~0x224c, reserved 3 * 4 bytes */
    volatile u_isp_af_firh0         isp_af_firh0; /* 0x2250 */
    volatile u_isp_af_firh1         isp_af_firh1; /* 0x2254 */
    volatile u_isp_af_firh2         isp_af_firh2; /* 0x2258 */
    volatile u_isp_af_firh3         isp_af_firh3; /* 0x225c */
    volatile u_isp_af_firh4         isp_af_firh4; /* 0x2260 */
    volatile u_isp_af_stt_bst       isp_af_stt_bst; /* 0x2264 */
    volatile u_isp_af_stt_abn       isp_af_stt_abn; /* 0x2268 */
    volatile unsigned int           reserved_31[3]; /* 0x226c~0x2274, reserved 3 * 4 bytes */
    volatile u_isp_af_acc_shift     isp_af_acc_shift; /* 0x2278 */
    volatile u_isp_af_cnt_shift     isp_af_cnt_shift; /* 0x227c */
    volatile unsigned int           reserved_32[2]; /* 0x2280~0x2284, reserved 2 * 4 bytes */
    volatile u_isp_af_stat_raddr    isp_af_stat_raddr; /* 0x2288 */
    volatile u_isp_af_stat_rdata    isp_af_stat_rdata; /* 0x228c */
    volatile unsigned int           reserved_33[28]; /* 0x2290~0x22fc, reserved 28 * 4 bytes */
    volatile u_isp_af_iirthre       isp_af_iirthre; /* 0x2300 */
    volatile u_isp_af_iirgain       isp_af_iirgain; /* 0x2304 */
    volatile u_isp_af_iirslope      isp_af_iirslope; /* 0x2308 */
    volatile u_isp_af_iirdilate     isp_af_iirdilate; /* 0x230c */
    volatile u_isp_af_firthre       isp_af_firthre; /* 0x2310 */
    volatile u_isp_af_firgain       isp_af_firgain; /* 0x2314 */
    volatile u_isp_af_firslope      isp_af_firslope; /* 0x2318 */
    volatile unsigned int           reserved_34; /* 0x231c */
    volatile u_isp_af_iirthre_coring   isp_af_iirthre_coring; /* 0x2320 */
    volatile u_isp_af_iirpeak_coring   isp_af_iirpeak_coring; /* 0x2324 */
    volatile u_isp_af_iirslope_coring   isp_af_iirslope_coring; /* 0x2328 */
    volatile unsigned int           reserved_35; /* 0x232c */
    volatile u_isp_af_firthre_coring   isp_af_firthre_coring; /* 0x2330 */
    volatile u_isp_af_firpeak_coring   isp_af_firpeak_coring; /* 0x2334 */
    volatile u_isp_af_firslope_coring   isp_af_firslope_coring; /* 0x2338 */
    volatile unsigned int           reserved_36; /* 0x233c */
    volatile u_isp_af_highlight       isp_af_highlight; /* 0x2340 */
    volatile u_isp_af_offset        isp_af_offset; /* 0x2344 */
    volatile unsigned int                    reserved_37[1454]; /* 0x2348~0x39fc */
    volatile u_isp_fpn_cfg                   isp_fpn_cfg; /* 0x3a00 */
    volatile u_isp_fpn_calib_start           isp_fpn_calib_start; /* 0x3a04 */
    volatile u_isp_fpn_corr_cfg              isp_fpn_corr_cfg; /* 0x3a08 */
    volatile u_isp_fpn_stat                  isp_fpn_stat; /* 0x3a0c */
    volatile u_isp_fpn_white_level           isp_fpn_white_level; /* 0x3a10 */
    volatile unsigned int                    reserved_38; /* 0x3a14 */
    volatile u_isp_fpn_divcoef               isp_fpn_divcoef; /* 0x3a18 */
    volatile u_isp_fpn_framelog2             isp_fpn_framelog2; /* 0x3a1c */
    volatile u_isp_fpn_sum0                  isp_fpn_sum0; /* 0x3a20 */
    volatile u_isp_fpn_sum1                  isp_fpn_sum1; /* 0x3a24 */
    volatile unsigned int                    reserved_39[2]; /* 0x3a28~0x3a2c */
    volatile u_isp_fpn_corr0                 isp_fpn_corr0; /* 0x3a30 */
    volatile unsigned int                    reserved_40[3]; /* 0x3a34~0x3a3c */
    volatile u_isp_fpn_shift                 isp_fpn_shift; /* 0x3a40 */
    volatile unsigned int                    reserved_41[3]; /* 0x3a44~0x3a4c */
    volatile u_isp_fpn_max_o                 isp_fpn_max_o; /* 0x3a50 */
    volatile u_isp_fpn_overflowthr           isp_fpn_overflowthr; /* 0x3a54 */
    volatile unsigned int                    reserved_42[2]; /* 0x3a58~0x3a5c */
    volatile u_isp_fpn_black_frame_out_sel   isp_fpn_black_frame_out_sel; /* 0x3a60 */
    volatile u_isp_fpn_overlap_cfg           isp_fpn_overlap_cfg; /* 0x3a64 */
    volatile u_isp_fpn_calib_offline_set     isp_fpn_calib_offline_set; /* 0x3a68 */
    volatile u_isp_fpn_calib_stat_clear      isp_fpn_calib_stat_clear; /* 0x3a6c */
    volatile u_isp_fpn_roi_cfg0              isp_fpn_roi_cfg0; /* 0x3a70 */
    volatile u_isp_fpn_roi_cfg1              isp_fpn_roi_cfg1; /* 0x3a74 */
    volatile unsigned int                    reserved_43[7589]; /* 0x3a78~0xb108 */
    volatile u_isp_crop_version     isp_crop_version; /* 0xb10c */
    volatile u_isp_crop_sizeout     isp_crop_sizeout; /* 0xb110 */
    volatile u_isp_crop_pos         isp_crop_pos; /* 0xb114 */
    volatile unsigned int           reserved_44[58]; /* 0xb118~0xb1fc, reserved 58 * 4 bytes */
    volatile u_isp_blc_dynamic_reg0   isp_blc_dynamic_reg0; /* 0xb200 */
    volatile u_isp_blc_dynamic_reg1   isp_blc_dynamic_reg1; /* 0xb204 */
    volatile u_isp_blc_dynamic_reg2   isp_blc_dynamic_reg2; /* 0xb208 */
    volatile u_isp_blc_dynamic_reg3   isp_blc_dynamic_reg3; /* 0xb20c */
    volatile u_isp_blc_dynamic_reg4   isp_blc_dynamic_reg4; /* 0xb210 */
    volatile u_isp_blc_dynamic_reg5   isp_blc_dynamic_reg5; /* 0xb214 */
    volatile u_isp_blc_dynamic_reg6   isp_blc_dynamic_reg6; /* 0xb218 */
} isp_fe_reg_type;

/* Define the union u_isp_be_version */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_patch             : 8; /* [7..0]  */
        unsigned int    isp_build             : 8; /* [15..8]  */
        unsigned int    isp_release           : 8; /* [23..16]  */
        unsigned int    isp_version           : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_version;

/* Define the union u_isp_be_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_date               : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_date;
/* Define the union u_isp_be_fpga_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpga_date          : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_fpga_date;
/* Define the union u_isp_be_module_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_sel            : 2; /* [1..0]  */
        unsigned int    isp_awb_sel           : 2; /* [3..2]  */
        unsigned int    isp_af_sel            : 2; /* [5..4]  */
        unsigned int    reserved_0            : 1; /* [6]  */
        unsigned int    isp_clut_sel          : 1; /* [7]  */
        unsigned int    reserved_1            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_module_pos;

/* Define the union u_isp_be_fstart_delay */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_delay              : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_fstart_delay;
/* Define the union u_isp_be_input_mux */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_input0_sel        : 2; /* [1..0]  */
        unsigned int    reserved_0            : 2; /* [3..2]  */
        unsigned int    isp_input1_sel        : 2; /* [5..4]  */
        unsigned int    reserved_1            : 2; /* [7..6]  */
        unsigned int    isp_input2_sel        : 2; /* [9..8]  */
        unsigned int    reserved_2            : 2; /* [11..10]  */
        unsigned int    isp_input3_sel        : 2; /* [13..12]  */
        unsigned int    reserved_3            : 2; /* [15..14]  */
        unsigned int    isp_input4_sel        : 1; /* [16]  */
        unsigned int    reserved_4            : 3; /* [19..17]  */
        unsigned int    isp_sw_sel            : 2; /* [21..20]  */
        unsigned int    reserved_5            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_input_mux;

/* Define the union u_isp_be_sys_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_stt_en            : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_sys_ctrl;

/* Define the union u_isp_be_user_define0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_user_define0       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_user_define0;
/* Define the union u_isp_be_user_define1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_user_define1       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_user_define1;
/* Define the union u_isp_be_startup */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fcnt               : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_startup;
/* Define the union u_isp_be_format */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_format            : 2; /* [1..0]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_be_format;

/* Define the union u_isp_clip_y_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clip_y_min        : 16; /* [15..0]  */
        unsigned int    isp_clip_y_max        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clip_y_cfg;

/* Define the union u_isp_clip_c_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clip_c_min        : 16; /* [15..0]  */
        unsigned int    isp_clip_c_max        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clip_c_cfg;

/* Define the union u_isp_dmnr_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_dither_en    : 1; /* [0]  */
        unsigned int    isp_dmnr_dither_round : 2; /* [2..1]  */
        unsigned int    isp_dmnr_dither_spatial_mode : 1; /* [3]  */
        unsigned int    isp_dmnr_dither_out_bits : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_dither;

/* Define the union u_isp_drc_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_dither_en     : 1; /* [0]  */
        unsigned int    isp_drc_dither_round  : 2; /* [2..1]  */
        unsigned int    isp_drc_dither_spatial_mode : 1; /* [3]  */
        unsigned int    isp_drc_dither_out_bits : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_drc_dither;

/* Define the union u_isp_sharpen_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dither_en : 1; /* [0]  */
        unsigned int    isp_sharpen_dither_round : 2; /* [2..1]  */
        unsigned int    isp_sharpen_dither_spatial_mode : 1; /* [3]  */
        unsigned int    isp_sharpen_dither_out_bits : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dither;

/* Define the union u_isp_blk_hblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blk_b_hblank      : 16; /* [15..0]  */
        unsigned int    isp_blk_f_hblank      : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blk_hblank;

/* Define the union u_isp_blk_vblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blk_b_vblank      : 16; /* [15..0]  */
        unsigned int    isp_blk_f_vblank      : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blk_vblank;

/* Define the union u_isp_y_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_y_sum0;
/* Define the union u_isp_y_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum1             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_y_sum1;
/* Define the union u_isp_blc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_version;
/* Define the union u_isp_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_ofsgr         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_blc_ofsr          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_offset1;

/* Define the union u_isp_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_ofsgb         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_blc_ofsb          : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_blc_offset2;

/* Define the union u_isp_blc_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc_roi_y : 13; /* [12..0]  */
        unsigned int isp_blc_roi_x : 13; /* [25..13]  */
        unsigned int isp_blc_roi_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;     /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_roi_cfg0;

/* Define the union u_isp_blc_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc_roi_height : 13; /* [12..0]  */
        unsigned int isp_blc_roi_width : 13;  /* [25..13]  */
        unsigned int reserved_0 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_roi_cfg1;

/* Define the union u_isp_wb_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_en_in : 1;   /* [0]  */
        unsigned int isp_wb_en_out : 1;  /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_blc_cfg;

/* Define the union u_isp_wb_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_version;
/* Define the union u_isp_wb_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_grgain         : 16; /* [15..0]  */
        unsigned int    isp_wb_rgain          : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_gain1;

/* Define the union u_isp_wb_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_gbgain         : 16; /* [15..0]  */
        unsigned int    isp_wb_bgain          : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_gain2;

/* Define the union u_isp_wb_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_ofsgr          : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_wb_ofsr           : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_blc_offset1;

/* Define the union u_isp_wb_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_ofsgb          : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_wb_ofsb           : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_blc_offset2;

/* Define the union u_isp_wb_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_clip_value     : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_clip_value;

/* Define the union u_isp_dg_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_en_in : 1;   /* [0]  */
        unsigned int isp_dg_en_out : 1;  /* [1]  */
        unsigned int isp_dg_en_comp : 1; /* [2]  */
        unsigned int reserved_0 : 29;    /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_blc_cfg;

/* Define the union u_isp_dg_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_version;
/* Define the union u_isp_dg_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_grgain         : 16; /* [15..0]  */
        unsigned int    isp_dg_rgain          : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_gain1;

/* Define the union u_isp_dg_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_gbgain         : 16; /* [15..0]  */
        unsigned int    isp_dg_bgain          : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_gain2;

/* Define the union u_isp_dg_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_ofsgr          : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dg_ofsr           : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_blc_offset1;

/* Define the union u_isp_dg_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_ofsgb          : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dg_ofsb           : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_blc_offset2;

/* Define the union u_isp_dg_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_clip_value     : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_clip_value;

/* Define the union u_isp_dg_blc_offset_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_ofs_shift : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;      /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_blc_offset_shift;

/* Define the union u_isp_dg_real_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_real_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;         /* [15]  */
        unsigned int isp_dg_real_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_real_blc_offset1;

/* Define the union u_isp_dg_real_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_real_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;         /* [15]  */
        unsigned int isp_dg_real_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_real_blc_offset2;

/* Define the union u_isp_dg_roi_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_roi_y : 13; /* [12..0]  */
        unsigned int isp_dg_roi_x : 13; /* [25..13]  */
        unsigned int isp_dg_roi_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;    /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_roi_cfg0;

/* Define the union u_isp_dg_roi_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_roi_height : 13; /* [12..0]  */
        unsigned int isp_dg_roi_width : 13;  /* [25..13]  */
        unsigned int reserved_0 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_roi_cfg1;

/* Define the union u_isp_cc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 2; /* [1..0]  */
        unsigned int    isp_cc_colortone_en   : 1; /* [2]  */
        unsigned int    isp_cc_recover_en     : 1; /* [3]  */
        unsigned int    reserved_1            : 1; /* [4]  */
        unsigned int    isp_cc_lutb_en        : 1; /* [5]  */
        unsigned int    reserved_2            : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_cfg;

/* Define the union u_isp_cc_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef00         : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef01         : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_coef0;

/* Define the union u_isp_cc_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef02         : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef10         : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_coef1;

/* Define the union u_isp_cc_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef11         : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef12         : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_coef2;

/* Define the union u_isp_cc_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef20         : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef21         : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_coef3;

/* Define the union u_isp_cc_coef4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef22         : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_coef4;

/* Define the union u_isp_cc_rcv_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_soft_clip0_step : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_cc_soft_clip1_step : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    reserved_2            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_rcv_ctrl0;

/* Define the union u_isp_cc_rcv_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_darkprev       : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_cc_peaksupp_sat   : 8; /* [15..8]  */
        unsigned int    isp_cc_peaksupp_max   : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_rcv_ctrl1;

/* Define the union u_isp_cc_lumafact */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_luma_coefr     : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_cc_luma_coefb     : 5; /* [12..8]  */
        unsigned int    reserved_1            : 3; /* [15..13]  */
        unsigned int    isp_cc_luma_coefr_up  : 5; /* [20..16]  */
        unsigned int    reserved_2            : 3; /* [23..21]  */
        unsigned int    isp_cc_luma_coefb_up  : 5; /* [28..24]  */
        unsigned int    reserved_3            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_lumafact;

/* Define the union u_isp_cc_colortone_rb_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_b_gain         : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_cc_r_gain         : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_colortone_rb_gain;

/* Define the union u_isp_cc_colortone_g_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_g_gain         : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_colortone_g_gain;

/* Define the union u_isp_cc_in_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_in_dc0         : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_in_dc0;

/* Define the union u_isp_cc_in_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_in_dc1         : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_in_dc1;

/* Define the union u_isp_cc_in_dc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_in_dc2         : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_in_dc2;

/* Define the union u_isp_cc_out_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_out_dc0        : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_out_dc0;

/* Define the union u_isp_cc_out_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_out_dc1        : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_out_dc1;

/* Define the union u_isp_cc_out_dc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_out_dc2        : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_out_dc2;

/* Define the union u_isp_csc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_csc_version        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_version;
/* Define the union u_isp_csc_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_csc_coef00        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_csc_coef01        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_coef0;

/* Define the union u_isp_csc_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_csc_coef02        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_csc_coef10        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_coef1;

/* Define the union u_isp_csc_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_csc_coef11        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_csc_coef12        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_coef2;

/* Define the union u_isp_csc_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_csc_coef20        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_csc_coef21        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_coef3;

/* Define the union u_isp_csc_coef4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_csc_coef22        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_coef4;

/* Define the union u_isp_csc_in_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5; /* [4..0]  */
        unsigned int    isp_csc_in_dc0        : 11; /* [15..5]  */
        unsigned int    reserved_1            : 5; /* [20..16]  */
        unsigned int    isp_csc_in_dc1        : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_in_dc0;

/* Define the union u_isp_csc_in_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5; /* [4..0]  */
        unsigned int    isp_csc_in_dc2        : 11; /* [15..5]  */
        unsigned int    reserved_1            : 5; /* [20..16]  */
        unsigned int    reserved_2            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_in_dc1;

/* Define the union u_isp_csc_out_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5; /* [4..0]  */
        unsigned int    isp_csc_out_dc0       : 11; /* [15..5]  */
        unsigned int    reserved_1            : 5; /* [20..16]  */
        unsigned int    isp_csc_out_dc1       : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_out_dc0;

/* Define the union u_isp_csc_out_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5; /* [4..0]  */
        unsigned int    isp_csc_out_dc2       : 11; /* [15..5]  */
        unsigned int    reserved_1            : 5; /* [20..16]  */
        unsigned int    reserved_2            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_csc_out_dc1;

/* Define the union u_isp_cc_dgain_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_pos_0    : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_cc_dgain_pos_1    : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_pos0;

/* Define the union u_isp_cc_dgain_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_pos_2    : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_cc_dgain_pos_3    : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_pos1;

/* Define the union u_isp_cc_dgain_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_pos_4    : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_cc_dgain_pos_5    : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_pos2;

/* Define the union u_isp_cc_dgain_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_pos_6    : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_cc_dgain_pos_7    : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_pos3;

/* Define the union u_isp_cc_dgain_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_inseg_0 : 7; /* [6..0]  */
        unsigned int    isp_cc_dgain_inseg_1 : 7; /* [13..7]  */
        unsigned int    isp_cc_dgain_inseg_2 : 7; /* [20..14]  */
        unsigned int    isp_cc_dgain_inseg_3 : 7; /* [27..21]  */
        unsigned int    reserved_0           : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_inseg0;

/* Define the union u_isp_cc_dgain_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_inseg_4  : 7; /* [6..0]  */
        unsigned int    isp_cc_dgain_inseg_5  : 7; /* [13..7]  */
        unsigned int    isp_cc_dgain_inseg_6  : 7; /* [20..14]  */
        unsigned int    isp_cc_dgain_inseg_7  : 7; /* [27..21]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_inseg1;

/* Define the union u_isp_cc_dgain_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_step0    : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_cc_dgain_step1    : 3; /* [6..4]  */
        unsigned int    reserved_1            : 1; /* [7]  */
        unsigned int    isp_cc_dgain_step2    : 3; /* [10..8]  */
        unsigned int    reserved_2            : 1; /* [11]  */
        unsigned int    isp_cc_dgain_step3    : 3; /* [14..12]  */
        unsigned int    reserved_3            : 1; /* [15]  */
        unsigned int    isp_cc_dgain_step4    : 3; /* [18..16]  */
        unsigned int    reserved_4            : 1; /* [19]  */
        unsigned int    isp_cc_dgain_step5    : 3; /* [22..20]  */
        unsigned int    reserved_5            : 1; /* [23]  */
        unsigned int    isp_cc_dgain_step6    : 3; /* [26..24]  */
        unsigned int    reserved_6            : 1; /* [27]  */
        unsigned int    isp_cc_dgain_step7    : 3; /* [30..28]  */
        unsigned int    reserved_7            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_step;

/* Define the union u_isp_cc_gamma_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_pos_0    : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_cc_gamma_pos_1    : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_pos0;

/* Define the union u_isp_cc_gamma_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_pos_2    : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_cc_gamma_pos_3    : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_pos1;

/* Define the union u_isp_cc_gamma_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_pos_4    : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_cc_gamma_pos_5    : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_pos2;

/* Define the union u_isp_cc_gamma_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_pos_6    : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_cc_gamma_pos_7    : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_pos3;

/* Define the union u_isp_cc_gamma_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_inseg_0  : 7; /* [6..0]  */
        unsigned int    isp_cc_gamma_inseg_1  : 7; /* [13..7]  */
        unsigned int    isp_cc_gamma_inseg_2  : 7; /* [20..14]  */
        unsigned int    isp_cc_gamma_inseg_3  : 7; /* [27..21]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_inseg0;

/* Define the union u_isp_cc_gamma_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_inseg_4  : 7; /* [6..0]  */
        unsigned int    isp_cc_gamma_inseg_5  : 7; /* [13..7]  */
        unsigned int    isp_cc_gamma_inseg_6  : 7; /* [20..14]  */
        unsigned int    isp_cc_gamma_inseg_7  : 7; /* [27..21]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_inseg1;

/* Define the union u_isp_cc_gamma_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_step0    : 4; /* [3..0]  */
        unsigned int    isp_cc_gamma_step1    : 4; /* [7..4]  */
        unsigned int    isp_cc_gamma_step2    : 4; /* [11..8]  */
        unsigned int    isp_cc_gamma_step3    : 4; /* [15..12]  */
        unsigned int    isp_cc_gamma_step4    : 4; /* [19..16]  */
        unsigned int    isp_cc_gamma_step5    : 4; /* [23..20]  */
        unsigned int    isp_cc_gamma_step6    : 4; /* [27..24]  */
        unsigned int    isp_cc_gamma_step7    : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_step;

/* Define the union u_isp_cc_luma_wdr_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_luma_wdr_coefr : 5; /* [4..0]  */
        unsigned int reserved_0 : 11;           /* [15..5]  */
        unsigned int isp_cc_luma_wdr_coefb : 5; /* [20..16]  */
        unsigned int reserved_1 : 11;           /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_luma_wdr_coef;

/* Define the union u_isp_cc_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_stt2lut_en     : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_stt2lut_cfg;

/* Define the union u_isp_cc_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_stt2lut_regnew;

/* Define the union u_isp_cc_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_stt2lut_info   : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_cc_stt2lut_clr    : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_stt2lut_abn;

/* Define the union u_isp_cc_dgain_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_lut_waddr : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_lut_waddr;

/* Define the union u_isp_cc_dgain_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_lut_wdata : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_lut_wdata;

/* Define the union u_isp_cc_dgain_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_lut_raddr : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_lut_raddr;

/* Define the union u_isp_cc_dgain_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_dgain_lut_rdata : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_dgain_lut_rdata;

/* Define the union u_isp_cc_gamma_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_lut_waddr : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_lut_waddr;

/* Define the union u_isp_cc_gamma_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_lut_wdata : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_lut_wdata;

/* Define the union u_isp_cc_gamma_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_lut_raddr : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_lut_raddr;

/* Define the union u_isp_cc_gamma_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_gamma_lut_rdata : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_cc_gamma_lut_rdata;

/* Define the union u_isp_hcds_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_hcds_version       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_hcds_version;
/* Define the union u_isp_hcds_coefh0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hcds_coefh1       : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_hcds_coefh0       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_hcds_coefh0;

/* Define the union u_isp_hcds_coefh1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hcds_coefh3       : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_hcds_coefh2       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_hcds_coefh1;

/* Define the union u_isp_hcds_coefh2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hcds_coefh5       : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_hcds_coefh4       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_hcds_coefh2;

/* Define the union u_isp_hcds_coefh3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hcds_coefh7       : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_hcds_coefh6       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_hcds_coefh3;

/* Define the union u_isp_4dg_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg_en_in         : 1; /* [0]  */
        unsigned int    isp_4dg_en_out        : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_blc_cfg;

/* Define the union u_isp_4dg_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_4dg_version        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_version;
/* Define the union u_isp_4dg_0_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_grgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg0_rgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_0_gain1;

/* Define the union u_isp_4dg_0_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_gbgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg0_bgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_0_gain2;

/* Define the union u_isp_4dg_0_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_ofsgr        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg0_ofsr         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_0_blc_offset1;

/* Define the union u_isp_4dg_0_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_ofsgb        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg0_ofsb         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_0_blc_offset2;

/* Define the union u_isp_4dg_1_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_grgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg1_rgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_1_gain1;

/* Define the union u_isp_4dg_1_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_gbgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg1_bgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_1_gain2;

/* Define the union u_isp_4dg_1_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_ofsgr        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg1_ofsr         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_1_blc_offset1;

/* Define the union u_isp_4dg_1_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_ofsgb        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg1_ofsb         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_1_blc_offset2;

/* Define the union u_isp_4dg_2_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_grgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg2_rgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_2_gain1;

/* Define the union u_isp_4dg_2_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_gbgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg2_bgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_2_gain2;

/* Define the union u_isp_4dg_2_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_ofsgr        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg2_ofsr         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_2_blc_offset1;

/* Define the union u_isp_4dg_2_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_ofsgb        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg2_ofsb         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_2_blc_offset2;

/* Define the union u_isp_4dg_3_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_grgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg3_rgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_3_gain1;

/* Define the union u_isp_4dg_3_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_gbgain       : 16; /* [15..0]  */
        unsigned int    isp_4dg3_bgain        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_3_gain2;

/* Define the union u_isp_4dg_3_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_ofsgr        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg3_ofsr         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_3_blc_offset1;

/* Define the union u_isp_4dg_3_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_ofsgb        : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_4dg3_ofsb         : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_3_blc_offset2;

/* Define the union u_isp_4dg_0_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_clip_value   : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_0_clip_value;

/* Define the union u_isp_4dg_1_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_clip_value   : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_1_clip_value;

/* Define the union u_isp_4dg_2_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_clip_value   : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_2_clip_value;

/* Define the union u_isp_4dg_3_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_clip_value   : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_4dg_3_clip_value;


/* Define the union u_isp_awb_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_version        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_version;
/* Define the union u_isp_awb_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_hnum          : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_awb_vnum          : 6; /* [13..8]  */
        unsigned int    reserved_1            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_zone;

/* Define the union u_isp_awb_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_bitmove       : 3; /* [2..0]  */
        unsigned int    isp_awb_stat_level_en : 1; /* [3]  */
        unsigned int    isp_awb_stat_level_status : 1; /* [4]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_ctrl;

/* Define the union u_isp_awb_thd_min */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_threshold_min : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_thd_min;

/* Define the union u_isp_awb_thd_max */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_threshold_max : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_thd_max;

/* Define the union u_isp_awb_cr_mm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_cr_ref_max    : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_awb_cr_ref_min    : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_cr_mm;

/* Define the union u_isp_awb_cb_mm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_cb_ref_max    : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_awb_cb_ref_min    : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_cb_mm;

/* Define the union u_isp_awb_offset_comp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_offset_comp   : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_offset_comp;

/* Define the union u_isp_awb_avg_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_r         : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_avg_r;

/* Define the union u_isp_awb_avg_g */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_g         : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_avg_g;

/* Define the union u_isp_awb_avg_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_b         : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_avg_b;

/* Define the union u_isp_awb_cnt_all */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_count_all     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 14; /* [29..16]  */
        unsigned int    isp_awb_debug_level_status : 1; /* [30]  */
        unsigned int    isp_awb_debug_level_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_cnt_all;

/* Define the union u_isp_awb_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_stt_size      : 16; /* [15..0]  */
        unsigned int    isp_awb_stt_bst       : 4; /* [19..16]  */
        unsigned int    isp_awb_stt_en        : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stt_bst;

/* Define the union u_isp_awb_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_stt_info      : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_awb_stt_clr       : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stt_abn;

/* Define the union u_isp_awb_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_crop_pos_x    : 16; /* [15..0]  */
        unsigned int    isp_awb_crop_pos_y    : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_crop_pos;

/* Define the union u_isp_awb_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_crop_out_width : 16; /* [15..0]  */
        unsigned int    isp_awb_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_crop_outsize;

/* Define the union u_isp_awb_stat_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stat_waddr;
/* Define the union u_isp_awb_stat_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stat_wdata;
/* Define the union u_isp_awb_stat_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_raddr     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stat_raddr;
/* Define the union u_isp_awb_stat_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_rdata     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stat_rdata;

/* Define the union u_isp_la_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_version         : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_version;
/* Define the union u_isp_la_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_hnum           : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_la_vnum           : 5; /* [12..8]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_zone;

/* Define the union u_isp_la_bitmove */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_bitmove        : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_la_gamma_en       : 1; /* [8]  */
        unsigned int    isp_la_blc_en         : 1; /* [9]  */
        unsigned int    reserved_1            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_bitmove;

/* Define the union u_isp_la_offset_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_r       : 15; /* [14..0]  */
        unsigned int    reserved_0            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_offset_r;

/* Define the union u_isp_la_offset_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_gr      : 15; /* [14..0]  */
        unsigned int    reserved_0            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_offset_gr;

/* Define the union u_isp_la_offset_gb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_gb      : 15; /* [14..0]  */
        unsigned int    reserved_0            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_offset_gb;

/* Define the union u_isp_la_offset_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_b       : 15; /* [14..0]  */
        unsigned int    reserved_0            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_offset_b;

/* Define the union u_isp_la_gamma_limit */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_gamma_limit    : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_gamma_limit;

/* Define the union u_isp_la_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_crop_pos_x     : 16; /* [15..0]  */
        unsigned int    isp_la_crop_pos_y     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_crop_pos;

/* Define the union u_isp_la_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_crop_out_width : 16; /* [15..0]  */
        unsigned int    isp_la_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_crop_outsize;

/* Define the union u_isp_la_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_stt_size       : 16; /* [15..0]  */
        unsigned int    isp_la_stt_bst        : 4; /* [19..16]  */
        unsigned int    isp_la_stt_en         : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_stt_bst;

/* Define the union u_isp_la_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_stt_info       : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_la_stt_clr        : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_stt_abn;

/* Define the union u_isp_la_aver_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_aver_waddr;
/* Define the union u_isp_la_aver_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_aver_wdata;
/* Define the union u_isp_la_aver_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_aver_raddr;
/* Define the union u_isp_la_aver_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_rdata      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_aver_rdata;
/* Define the union u_isp_flick_f0_inblc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f0_inblc_gr : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_flick_f0_inblc_r  : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_f0_inblc0;

/* Define the union u_isp_flick_f0_inblc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f0_inblc_b  : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_flick_f0_inblc_gb : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_f0_inblc1;

/* Define the union u_isp_flick_f1_inblc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f1_inblc_gr : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_flick_f1_inblc_r  : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_f1_inblc0;

/* Define the union u_isp_flick_f1_inblc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f1_inblc_b  : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_flick_f1_inblc_gb : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_f1_inblc1;

/* Define the union u_isp_flick_exporatio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_exporatio   : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_exporatio;

/* Define the union u_isp_flick_overth */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_overth      : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_overth;

/* Define the union u_isp_flick_gavg_pre */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gbavg_pre   : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_flick_gravg_pre   : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gavg_pre;

/* Define the union u_isp_flick_gr_diff_cur */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_diff_cur : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gr_diff_cur;

/* Define the union u_isp_flick_gb_diff_cur */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_diff_cur : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gb_diff_cur;

/* Define the union u_isp_flick_gr_abs_cur */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_abs_cur  : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gr_abs_cur;

/* Define the union u_isp_flick_gb_abs_cur */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_abs_cur  : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gb_abs_cur;

/* Define the union u_isp_flick_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_stt_size    : 16; /* [15..0]  */
        unsigned int    isp_flick_stt_bst     : 4; /* [19..16]  */
        unsigned int    isp_flick_stt_en      : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_stt_bst;

/* Define the union u_isp_flick_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_stt_info    : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_flick_stt_clr     : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_stt_abn;

/* Define the union u_isp_flick_gmean_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gmean_waddr;
/* Define the union u_isp_flick_gmean_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gmean_wdata;
/* Define the union u_isp_flick_gmean_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gmean_raddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gmean_raddr;

/* Define the union u_isp_flick_gmean_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gbmean_rdata : 11; /* [10..0]  */
        unsigned int    isp_flick_grmean_rdata : 11; /* [21..11]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gmean_rdata;

/* Define the union u_isp_awblsc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awblsc_version     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_version;
/* Define the union u_isp_awblsc_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_hnum       : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_awblsc_vnum       : 6; /* [13..8]  */
        unsigned int    reserved_1            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_zone;

/* Define the union u_isp_awblsc_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_bitmove    : 3; /* [2..0]  */
        unsigned int    isp_awblsc_stat_level_en : 1; /* [3]  */
        unsigned int    isp_awblsc_stat_level_status : 1; /* [4]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_ctrl;

/* Define the union u_isp_awblsc_thd_min */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_threshold_min : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_thd_min;

/* Define the union u_isp_awblsc_thd_max */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_threshold_max : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_thd_max;

/* Define the union u_isp_awblsc_cr_mm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_cr_ref_max : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_awblsc_cr_ref_min : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_cr_mm;

/* Define the union u_isp_awblsc_cb_mm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_cb_ref_max : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_awblsc_cb_ref_min : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_cb_mm;

/* Define the union u_isp_awblsc_offset_comp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_offset_comp : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_offset_comp;

/* Define the union u_isp_awblsc_avg_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_avg_r      : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_avg_r;

/* Define the union u_isp_awblsc_avg_g */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_avg_g      : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_avg_g;

/* Define the union u_isp_awblsc_avg_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_avg_b      : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_avg_b;

/* Define the union u_isp_awblsc_cnt_all */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_count_all  : 16; /* [15..0]  */
        unsigned int    reserved_0            : 14; /* [29..16]  */
        unsigned int    isp_awblsc_debug_level_status : 1; /* [30]  */
        unsigned int    isp_awblsc_debug_level_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_cnt_all;

/* Define the union u_isp_awblsc_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_stt_size   : 16; /* [15..0]  */
        unsigned int    isp_awblsc_stt_bst    : 4; /* [19..16]  */
        unsigned int    isp_awblsc_stt_en     : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_stt_bst;

/* Define the union u_isp_awblsc_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_stt_info   : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_awblsc_stt_clr    : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_stt_abn;

/* Define the union u_isp_awblsc_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_crop_pos_x : 16; /* [15..0]  */
        unsigned int    isp_awblsc_crop_pos_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_crop_pos;

/* Define the union u_isp_awblsc_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awblsc_crop_out_width : 16; /* [15..0]  */
        unsigned int    isp_awblsc_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_crop_outsize;

/* Define the union u_isp_awblsc_stat_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_stat_waddr;
/* Define the union u_isp_awblsc_stat_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_stat_wdata;
/* Define the union u_isp_awblsc_stat_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awblsc_stat_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_stat_raddr;
/* Define the union u_isp_awblsc_stat_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awblsc_stat_rdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awblsc_stat_rdata;

/* Define the union u_isp_lsc_mesh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_mesh_str      : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_lsc_mesh_scale    : 3; /* [18..16]  */
        unsigned int    reserved_1            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lsc_mesh;

/* Define the union u_isp_lsc_blcen */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_in_en     : 1; /* [0]  */
        unsigned int    reserved_0            : 7; /* [7..1]  */
        unsigned int    isp_lsc_blc_out_en    : 1; /* [8]  */
        unsigned int    reserved_1            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lsc_blcen;

/* Define the union u_isp_lsc_blc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_r         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_lsc_blc_gr        : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lsc_blc0;

/* Define the union u_isp_lsc_blc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_b         : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_lsc_blc_gb        : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lsc_blc1;

/* Define the union u_isp_lsc_right_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_right_shift : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lsc_right_shift;

/* Define the union u_isp_ge_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_ge_ge1_en         : 1; /* [1]  */
        unsigned int    reserved_1            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge_cfg;

/* Define the union u_isp_ge_blc_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge0_blc_offset_en : 1; /* [0]  */
        unsigned int    isp_ge1_blc_offset_en : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge_blc_en;

/* Define the union u_isp_ge_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_version         : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge_version;
/* Define the union u_isp_ge0_ct_th1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_th1     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge0_ct_th1;

/* Define the union u_isp_ge0_ct_th2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_th2     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge0_ct_th2;

/* Define the union u_isp_ge0_ct_th3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_th3     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge0_ct_th3;

/* Define the union u_isp_ge0_ct_slope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_slope1  : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_ge_ge0_ct_slope2  : 5; /* [12..8]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge0_ct_slope;

/* Define the union u_isp_ge1_ct_th1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_th1     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge1_ct_th1;

/* Define the union u_isp_ge1_ct_th2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_th2     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge1_ct_th2;

/* Define the union u_isp_ge1_ct_th3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_th3     : 14; /* [13..0]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge1_ct_th3;

/* Define the union u_isp_ge1_ct_slope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_slope1  : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_ge_ge1_ct_slope2  : 4; /* [11..8]  */
        unsigned int    reserved_1            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge1_ct_slope;

/* Define the union u_isp_ge0_blc_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge0_blc_offset_gb : 16; /* [15..0]  */
        unsigned int    isp_ge0_blc_offset_gr : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge0_blc_offset;

/* Define the union u_isp_ge1_blc_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge1_blc_offset_gb : 16; /* [15..0]  */
        unsigned int    isp_ge1_blc_offset_gr : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge1_blc_offset;

/* Define the union u_isp_ge_strength */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_strength       : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge_strength;

/* Define the union u_isp_ge_bit_depth_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_bit_depth_sel  : 2; /* [1..0]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ge_bit_depth_sel;

/* Define the union u_isp_dpc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_dpc_dpc_en1       : 1; /* [1]  */
        unsigned int    reserved_1            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_cfg;

/* Define the union u_isp_dpc_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_alpha1_g      : 8; /* [7..0]  */
        unsigned int    isp_dpc_alpha1_rb     : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_alpha;

/* Define the union u_isp_dpc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_version        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_version;
/* Define the union u_isp_dpc_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_det_en        : 1; /* [0]  */
        unsigned int    isp_dpc_cor_en        : 1; /* [1]  */
        unsigned int    isp_dpc_six_det_en    : 1; /* [2]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_dpc_grayscale_mode : 1; /* [4]  */
        unsigned int    isp_dpc_bpt_cor_en    : 1; /* [5]  */
        unsigned int    isp_dpc_dpt_det_sel   : 1; /* [6]  */
        unsigned int    isp_dpc_dp_hightlight_en : 1; /* [7]  */
        unsigned int    isp_dpc_ir_position   : 1; /* [8]  */
        unsigned int    isp_dpc_ir_channel    : 1; /* [9]  */
        unsigned int    reserved_1            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_mode;

/* Define the union u_isp_dpc_set_use */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_stage1_use_set1 : 1; /* [0]  */
        unsigned int    reserved_0            : 2; /* [2..1]  */
        unsigned int    isp_dpc_stage1_use_fix_set : 1; /* [3]  */
        unsigned int    reserved_1            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_set_use;

/* Define the union u_isp_dpc_methods_set_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_enable     : 1; /* [0]  */
        unsigned int    isp_dpc_lc_enable     : 1; /* [1]  */
        unsigned int    isp_dpc_ro_enable     : 1; /* [2]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_dpc_rg_enable     : 1; /* [4]  */
        unsigned int    reserved_1            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_methods_set_1;

/* Define the union u_isp_dpc_line_thresh_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_thr1_g   : 8; /* [7..0]  */
        unsigned int    isp_dpc_line_thr1_rb  : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_line_thresh_1;

/* Define the union u_isp_dpc_line_mad_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_mad_fac_1_g : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_dpc_line_mad_fac_1_rb : 6; /* [13..8]  */
        unsigned int    reserved_1            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_line_mad_fac_1;

/* Define the union u_isp_dpc_pg_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_fac_1_g    : 8; /* [7..0]  */
        unsigned int    isp_dpc_pg_fac_1_rb   : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_pg_fac_1;

/* Define the union u_isp_dpc_rg_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rg_fac_1_g    : 8; /* [7..0]  */
        unsigned int    isp_dpc_rg_fac_1_rb   : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_rg_fac_1;

/* Define the union u_isp_dpc_ro_limits */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ro_1_g        : 2; /* [1..0]  */
        unsigned int    isp_dpc_ro_1_rb       : 2; /* [3..2]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_ro_limits;

/* Define the union u_isp_dpc_bpt_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 4; /* [3..0]  */
        unsigned int    isp_dpc_bpt_use_set_1 : 1; /* [4]  */
        unsigned int    reserved_1            : 2; /* [6..5]  */
        unsigned int    isp_dpc_bpt_use_fix_set : 1; /* [7]  */
        unsigned int    reserved_2            : 8; /* [15..8]  */
        unsigned int    isp_dpc_bp_data       : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_ctrl;

/* Define the union u_isp_dpc_bpt_number */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_number    : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_number;

/* Define the union u_isp_dpc_bpt_calib_number */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_calib_number : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_calib_number;

/* Define the union u_isp_dpc_bpt_thrd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_dev_dead_thresh : 8; /* [7..0]  */
        unsigned int    isp_dpc_abs_dead_thresh : 8; /* [15..8]  */
        unsigned int    isp_dpc_dev_hot_thresh : 8; /* [23..16]  */
        unsigned int    isp_dpc_abs_hot_thresh : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_thrd;

/* Define the union u_isp_dpc_bpt_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_bpt_waddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_waddr;
/* Define the union u_isp_dpc_bpt_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_posx_wdata : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_dpc_bpt_posy_wdata : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_wdata;

/* Define the union u_isp_dpc_bpt_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_bpt_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_raddr;
/* Define the union u_isp_dpc_bpt_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_posx_rdata : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_dpc_bpt_posy_rdata : 13; /* [28..16]  */
        unsigned int    isp_dpc_bpt_rdata     : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_rdata;

/* Define the union u_isp_dpc1_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_alpha1_g     : 8; /* [7..0]  */
        unsigned int    isp_dpc1_alpha1_rb    : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_alpha;

/* Define the union u_isp_dpc1_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_det_en       : 1; /* [0]  */
        unsigned int    isp_dpc1_cor_en       : 1; /* [1]  */
        unsigned int    isp_dpc1_six_det_en   : 1; /* [2]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_dpc1_grayscale_mode : 1; /* [4]  */
        unsigned int    isp_dpc1_bpt_cor_en   : 1; /* [5]  */
        unsigned int    isp_dpc1_dpt_det_sel  : 1; /* [6]  */
        unsigned int    isp_dpc1_dp_hightlight_en : 1; /* [7]  */
        unsigned int    isp_dpc1_ir_position  : 1; /* [8]  */
        unsigned int    isp_dpc1_ir_channel   : 1; /* [9]  */
        unsigned int    reserved_1            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_mode;

/* Define the union u_isp_dpc1_set_use */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_stage1_use_set1 : 1; /* [0]  */
        unsigned int    reserved_0            : 2; /* [2..1]  */
        unsigned int    isp_dpc1_stage1_use_fix_set : 1; /* [3]  */
        unsigned int    reserved_1            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_set_use;

/* Define the union u_isp_dpc1_methods_set_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_pg_enable    : 1; /* [0]  */
        unsigned int    isp_dpc1_lc_enable    : 1; /* [1]  */
        unsigned int    isp_dpc1_ro_enable    : 1; /* [2]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_dpc1_rg_enable    : 1; /* [4]  */
        unsigned int    reserved_1            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_methods_set_1;

/* Define the union u_isp_dpc1_line_thresh_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_line_thr1_g  : 8; /* [7..0]  */
        unsigned int    isp_dpc1_line_thr1_rb : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_line_thresh_1;

/* Define the union u_isp_dpc1_line_mad_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_line_mad_fac_1_g : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_dpc1_line_mad_fac_1_rb : 6; /* [13..8]  */
        unsigned int    reserved_1            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_line_mad_fac_1;

/* Define the union u_isp_dpc1_pg_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_pg_fac_1_g   : 8; /* [7..0]  */
        unsigned int    isp_dpc1_pg_fac_1_rb  : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_pg_fac_1;

/* Define the union u_isp_dpc1_rg_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_rg_fac_1_g   : 8; /* [7..0]  */
        unsigned int    isp_dpc1_rg_fac_1_rb  : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_rg_fac_1;

/* Define the union u_isp_dpc1_ro_limits */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_ro_1_g       : 2; /* [1..0]  */
        unsigned int    isp_dpc1_ro_1_rb      : 2; /* [3..2]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_ro_limits;

/* Define the union u_isp_dpc_soft_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_soft_thr_min : 8; /* [7..0]  */
        unsigned int    isp_dpc_ex_soft_thr_max : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_soft_thr;

/* Define the union u_isp_dpc_bhardthr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_hard_thr_en : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bhardthr_en;

/* Define the union u_isp_dpc_rakeratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_rake_ratio : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_rakeratio;

/* Define the union u_isp_dpc_lch_thr_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_aver_fac_1 : 8; /* [7..0]  */
        unsigned int    isp_dpc_line_diff_thr_1 : 7; /* [14..8]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dpc_line_std_thr_1 : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_lch_thr_1;

/* Define the union u_isp_dpc_line_kerdiff_fac */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_kerdiff_fac : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_line_kerdiff_fac;

/* Define the union u_isp_dpc_bit_depth_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bit_depth_sel : 2; /* [1..0]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bit_depth_sel;

/* Define the union u_isp_dpc_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_stt2lut_en    : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_stt2lut_cfg;

/* Define the union u_isp_dpc_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_stt2lut_regnew;

/* Define the union u_isp_dpc_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_stt2lut_info  : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_dpc_stt2lut_clr   : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_stt2lut_abn;

/* Define the union u_isp_dpc_rg_fac_1_mtp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rg_fac_1_g_mtp : 10; /* [9..0]  */
        unsigned int    isp_dpc_rg_fac_1_rb_mtp : 10; /* [19..10]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_rg_fac_1_mtp;

/* Define the union u_isp_dpc_amp_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_amp_coef_k    : 8; /* [7..0]  */
        unsigned int    reserved_0            : 4; /* [11..8]  */
        unsigned int    isp_dpc_amp_coef_min  : 7; /* [18..12]  */
        unsigned int    reserved_1            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_amp_coef;

/* Define the union u_isp_dpc1_soft_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_ex_soft_thr_min : 8; /* [7..0]  */
        unsigned int    isp_dpc1_ex_soft_thr_max : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_soft_thr;

/* Define the union u_isp_dpc1_bhardthr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_ex_hard_thr_en : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_bhardthr_en;

/* Define the union u_isp_dpc1_rakeratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_ex_rake_ratio : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_rakeratio;

/* Define the union u_isp_dpc1_lch_thr_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_line_aver_fac_1 : 8; /* [7..0]  */
        unsigned int    isp_dpc1_line_diff_thr_1 : 7; /* [14..8]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_dpc1_line_std_thr_1 : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_lch_thr_1;

/* Define the union u_isp_dpc1_line_kerdiff_fac */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_line_kerdiff_fac : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_line_kerdiff_fac;

/* Define the union u_isp_dpc1_rg_fac_1_mtp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_rg_fac_1_g_mtp : 10; /* [9..0]  */
        unsigned int    isp_dpc1_rg_fac_1_rb_mtp : 10; /* [19..10]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_rg_fac_1_mtp;

/* Define the union u_isp_dpc1_amp_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc1_amp_coef_k   : 8; /* [7..0]  */
        unsigned int    reserved_0            : 4; /* [11..8]  */
        unsigned int    isp_dpc1_amp_coef_min : 7; /* [18..12]  */
        unsigned int    reserved_1            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc1_amp_coef;

/* Define the union u_isp_demosaic_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_ahd_en   : 1; /* [0]  */
        unsigned int    isp_demosaic_bshp_en  : 1; /* [1]  */
        unsigned int    isp_demosaic_gcac_en  : 1; /* [2]  */
        unsigned int    reserved_0            : 13; /* [15..3]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_demosaic_local_cac_en : 1; /* [17]  */
        unsigned int    reserved_2            : 1; /* [18]  */
        unsigned int    isp_demosaic_fcr_en   : 1; /* [19]  */
        unsigned int    reserved_3 : 1; /* [20]  */
        unsigned int    isp_demosaic_de_fake_en : 1; /* [21]  */
        unsigned int    reserved_4            : 2; /* [23..22]  */
        unsigned int    isp_demosaic_desat_enable : 1; /* [24]  */
        unsigned int    isp_demosaic_aidm_blend_enable : 1; /* [25]  */
        unsigned int    reserved_5            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_cfg1;

/* Define the union u_isp_demosaic_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_bld_limit2 : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_demosaic_bld_limit1 : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_coef0;

/* Define the union u_isp_demosaic_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_limit2 : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_demosaic_fcr_limit1 : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_coef2;

/* Define the union u_isp_demosaic_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_ahd_par2 : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_demosaic_ahd_par1 : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_coef3;

/* Define the union u_isp_demosaic_coef4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_gain : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_coef4;

/* Define the union u_isp_demosaic_coef5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hv_ratio : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_coef5;

/* Define the union u_isp_demosaic_coef6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_ratio : 6; /* [5..0]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_coef6;

/* Define the union u_isp_demosaic_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hv_sel   : 2; /* [1..0]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_sel;

/* Define the union u_isp_demosaic_lcac_cnt_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_r_counter_thr : 5; /* [4..0]  */
        unsigned int    isp_demosaic_b_counter_thr : 5; /* [9..5]  */
        unsigned int    isp_demosaic_g_counter_thr : 6; /* [15..10]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_lcac_cnt_thr;

/* Define the union u_isp_demosaic_lcac_luma_rb_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_r_luma_thr : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_demosaic_b_luma_thr : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_lcac_luma_rb_thr;

/* Define the union u_isp_demosaic_lcac_luma_g_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_g_luma_thr : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_lcac_luma_g_thr;

/* Define the union u_isp_demosaic_purple_var_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_purple_var_thr : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_purple_var_thr;

/* Define the union u_isp_demosaic_fake_cr_var_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fake_cr_var_thr_low : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_demosaic_fake_cr_var_thr_high : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_fake_cr_var_thr;

/* Define the union u_isp_demosaic_depurplectr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_depurplectrcb : 4; /* [3..0]  */
        unsigned int    reserved_0            : 12; /* [15..4]  */
        unsigned int    isp_demosaic_depurplectrcr : 4; /* [19..16]  */
        unsigned int    reserved_1            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_depurplectr;

/* Define the union u_isp_demosaic_lpf_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_lpf_f0   : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_demosaic_lpf_f1   : 5; /* [12..8]  */
        unsigned int    reserved_1            : 3; /* [15..13]  */
        unsigned int    isp_demosaic_lpf_f2   : 6; /* [21..16]  */
        unsigned int    reserved_2            : 2; /* [23..22]  */
        unsigned int    isp_demosaic_lpf_f3   : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_lpf_coef;

/* Define the union u_isp_demosaic_g_intp_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_g_clip_sft_bit : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_g_intp_ctrl;

/* Define the union u_isp_demosaic_cbcravgthld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cbcr_avg_thld : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_cbcravgthld;

/* Define the union u_isp_demosaic_cc_hf_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cc_hf_min_ratio : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_demosaic_cc_hf_max_ratio : 5; /* [12..8]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_cc_hf_ratio;

/* Define the union u_isp_demosaic_cbcr_ratio_limit */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cbcr_ratio_low_limit : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_demosaic_cbcr_ratio_high_limit : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_cbcr_ratio_limit;

/* Define the union u_isp_demosaic_fcr_gray_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_gray_ratio : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_fcr_gray_ratio;

/* Define the union u_isp_demosaic_fcr_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_detg_sel : 4; /* [3..0]  */
        unsigned int    isp_demosaic_fcr_cmax_sel : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_fcr_sel;

/* Define the union u_isp_demosaic_cc_var_thresh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cc_var_thresh : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_cc_var_thresh;

/* Define the union u_isp_demosaic_hf_intp_blur_th */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hf_intp_blur_th1 : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_demosaic_hf_intp_blur_th2 : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_hf_intp_blur_th;

/* Define the union u_isp_demosaic_hf_intp_blur_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hf_intp_blur_ratio : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_hf_intp_blur_ratio;

/* Define the union u_isp_demosaic_defcolor_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_defcolor_cr : 4; /* [3..0]  */
        unsigned int    reserved_0            : 12; /* [15..4]  */
        unsigned int    isp_demosaic_defcolor_cb : 4; /* [19..16]  */
        unsigned int    reserved_1            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_defcolor_coef;

/* Define the union u_isp_demosaic_fcr_hf_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_thresh1 : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_demosaic_fcr_thresh2 : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_fcr_hf_thr;

/* Define the union u_isp_demosaic_desat_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_desat_thresh1 : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_demosaic_desat_thresh2 : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_desat_thr;

/* Define the union u_isp_demosaic_desat_bldr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_desat_hig : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_demosaic_desat_low : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_desat_bldr;

/* Define the union u_isp_demosaic_desat_bldr_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_desat_ratio : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_desat_bldr_ratio;

/* Define the union u_isp_demosaic_desat_protect */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_desat_protect_sl : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_demosaic_desat_protect_th : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_desat_protect;

/* Define the union u_isp_demosaic_lut_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_depurplut_update : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_demosaic_lut_update;

/* Define the union u_isp_gcac_diffthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_out_diffthd  : 12; /* [11..0]  */
        unsigned int    reserved_0            : 8; /* [19..12]  */
        unsigned int    isp_gcac_desatuthr    : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_diffthd;

/* Define the union u_isp_gcac_lamda_reg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_edgemode     : 2; /* [1..0]  */
        unsigned int    isp_gcac_globalstr    : 10; /* [11..2]  */
        unsigned int    isp_gcac_normfactor   : 4; /* [15..12]  */
        unsigned int    isp_gcac_upperlmtsft  : 3; /* [18..16]  */
        unsigned int    isp_gcac_lowerlmtsft  : 3; /* [21..19]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_lamda_reg;

/* Define the union u_isp_gcac_lamdathd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_lamdathd1    : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_gcac_lamdathd2    : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_lamdathd;

/* Define the union u_isp_gcac_thrbthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_thrbthd1     : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_gcac_thrbthd2     : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_thrbthd;

/* Define the union u_isp_gcac_lamdamul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_thrbmul      : 16; /* [15..0]  */
        unsigned int    isp_gcac_lamdamul     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_lamdamul;

/* Define the union u_isp_gcac_distance_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_distance_thd1 : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_gcac_distance_thd2 : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_distance_thd;

/* Define the union u_isp_gcac_distance_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_distance_mul : 16; /* [15..0]  */
        unsigned int    isp_gcac_tao          : 5; /* [20..16]  */
        unsigned int    isp_gcac_purplealpha  : 6; /* [26..21]  */
        unsigned int    reserved_0            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_distance_mul;

/* Define the union u_isp_gcac_edge_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_edge_alpha   : 6; /* [5..0]  */
        unsigned int    isp_gcac_desatuthr_hig : 14; /* [19..6]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gcac_edge_alpha;

/* Define the union u_isp_dmnr_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_stt2lut_en   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_stt2lut_cfg;

/* Define the union u_isp_dmnr_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_stt2lut_regnew;

/* Define the union u_isp_dmnr_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_dmnr_stt2lut_clr  : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_stt2lut_abn;

/* Define the union u_isp_dmnr_aidm_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_aidm_blendrate : 9; /* [8..0]  */
        unsigned int    isp_dmnr_aidm_data_mask : 1; /* [9]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_aidm_cfg;

/* Define the union u_isp_dmnr_aidm_detail_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_aidm_detail_lut_waddr : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_aidm_detail_lut_waddr;

/* Define the union u_isp_dmnr_aidm_detail_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_aidm_detail_lut_wdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_aidm_detail_lut_wdata;

/* Define the union u_isp_dmnr_aidm_detail_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_aidm_detail_lut_raddr : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_aidm_detail_lut_raddr;

/* Define the union u_isp_dmnr_aidm_detail_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_aidm_detail_lut_rdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dmnr_aidm_detail_lut_rdata;

/* Define the union u_isp_sharpen_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_version    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_version;
/* Define the union u_isp_sharpen_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_benlumactrl : 1; /* [0]  */
        unsigned int    isp_sharpen_vcds_filterv : 1; /* [1]  */
        unsigned int    isp_sharpen_benshtvar_sel : 1; /* [2]  */
        unsigned int    isp_sharpen_benshtctrlbyvar : 1; /* [3]  */
        unsigned int    isp_sharpen_benskinctrl : 1; /* [4]  */
        unsigned int    isp_sharpen_weakdetailadj : 1; /* [5]  */
        unsigned int    isp_sharpen_benchrctrl : 1; /* [6]  */
        unsigned int    reserved_0            : 1; /* [7]  */
        unsigned int    reserved_1            : 1; /* [8]  */
        unsigned int    reserved_2            : 1; /* [9]  */
        unsigned int    isp_sharpen_detailthd_sel : 3; /* [12..10]  */
        unsigned int    isp_sharpen_bendetailctrl : 1; /* [13]  */
        unsigned int    reserved_3            : 1; /* [14]  */
        unsigned int    reserved_4            : 1; /* [15]  */
        unsigned int    isp_sharpen_ben8dir_sel : 1; /* [16]  */
        unsigned int    isp_sharpen_printfsel : 4; /* [20..17]  */
        unsigned int    reserved_5            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_ctrl;

/* Define the union u_isp_sharpen_mhfthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mhfthdsftd : 4; /* [3..0]  */
        unsigned int    isp_sharpen_mhfthdseld : 2; /* [5..4]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_sharpen_mhfthdsftud : 4; /* [19..16]  */
        unsigned int    isp_sharpen_mhfthdselud : 2; /* [21..20]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfthd;

/* Define the union u_isp_sharpen_weakdetail */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dirrlythrlow : 7; /* [6..0]  */
        unsigned int    reserved_0            : 1; /* [7]  */
        unsigned int    isp_sharpen_dirrlythrhih : 7; /* [14..8]  */
        unsigned int    reserved_1            : 1; /* [15]  */
        unsigned int    isp_sharpen_weakdetailgain : 7; /* [22..16]  */
        unsigned int    reserved_2            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_weakdetail;

/* Define the union u_isp_sharpen_dirvar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dirvarsft : 4; /* [3..0]  */
        unsigned int    isp_sharpen_dirvarscale : 4; /* [7..4]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dirrly0   : 7; /* [22..16]  */
        unsigned int    reserved_1            : 1; /* [23]  */
        unsigned int    isp_sharpen_dirrly1   : 7; /* [30..24]  */
        unsigned int    reserved_2            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dirvar;

/* Define the union u_isp_sharpen_dirdiff */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dirdiffsft : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_sharpen_dirrt0    : 5; /* [20..16]  */
        unsigned int    reserved_1            : 3; /* [23..21]  */
        unsigned int    isp_sharpen_dirrt1    : 5; /* [28..24]  */
        unsigned int    reserved_2            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dirdiff;

/* Define the union u_isp_sharpen_lumawgt0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt0  : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt1  : 6; /* [11..6]  */
        unsigned int    isp_sharpen_lumawgt2  : 6; /* [17..12]  */
        unsigned int    isp_sharpen_lumawgt3  : 6; /* [23..18]  */
        unsigned int    isp_sharpen_lumawgt4  : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt0;

/* Define the union u_isp_sharpen_lumawgt1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt5  : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt6  : 6; /* [11..6]  */
        unsigned int    isp_sharpen_lumawgt7  : 6; /* [17..12]  */
        unsigned int    isp_sharpen_lumawgt8  : 6; /* [23..18]  */
        unsigned int    isp_sharpen_lumawgt9  : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt1;

/* Define the union u_isp_sharpen_lumawgt2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt10 : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt11 : 6; /* [11..6]  */
        unsigned int    isp_sharpen_lumawgt12 : 6; /* [17..12]  */
        unsigned int    isp_sharpen_lumawgt13 : 6; /* [23..18]  */
        unsigned int    isp_sharpen_lumawgt14 : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt2;

/* Define the union u_isp_sharpen_lumawgt3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt15 : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt16 : 6; /* [11..6]  */
        unsigned int    isp_sharpen_lumawgt17 : 6; /* [17..12]  */
        unsigned int    isp_sharpen_lumawgt18 : 6; /* [23..18]  */
        unsigned int    isp_sharpen_lumawgt19 : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt3;

/* Define the union u_isp_sharpen_lumawgt4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt20 : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt21 : 6; /* [11..6]  */
        unsigned int    isp_sharpen_lumawgt22 : 6; /* [17..12]  */
        unsigned int    isp_sharpen_lumawgt23 : 6; /* [23..18]  */
        unsigned int    isp_sharpen_lumawgt24 : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt4;

/* Define the union u_isp_sharpen_lumawgt5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt25 : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt26 : 6; /* [11..6]  */
        unsigned int    isp_sharpen_lumawgt27 : 6; /* [17..12]  */
        unsigned int    isp_sharpen_lumawgt28 : 6; /* [23..18]  */
        unsigned int    isp_sharpen_lumawgt29 : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt5;

/* Define the union u_isp_sharpen_lumawgt6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt30 : 6; /* [5..0]  */
        unsigned int    isp_sharpen_lumawgt31 : 6; /* [11..6]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lumawgt6;

/* Define the union u_isp_sharpen_shoot_amt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_selpixwgt : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_sharpen_oshtamt   : 7; /* [14..8]  */
        unsigned int    reserved_1            : 1; /* [15]  */
        unsigned int    isp_sharpen_ushtamt   : 7; /* [22..16]  */
        unsigned int    reserved_2            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_shoot_amt;

/* Define the union u_isp_sharpen_shoot_maxchg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_omaxchg   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_sharpen_umaxchg   : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_shoot_maxchg;

/* Define the union u_isp_sharpen_shtvar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_shtvarsft : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_sharpen_shtvar5x5_sft : 4; /* [7..4]  */
        unsigned int    reserved_1            : 8; /* [15..8]  */
        unsigned int    isp_sharpen_shtbldrt  : 4; /* [19..16]  */
        unsigned int    reserved_2            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_shtvar;

/* Define the union u_isp_sharpen_oshtvar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_shtvarthd0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_shtvarthd1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_shtvarwgt0 : 7; /* [22..16]  */
        unsigned int    reserved_0            : 1; /* [23]  */
        unsigned int    isp_sharpen_shtvarwgt1 : 7; /* [30..24]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_oshtvar;

/* Define the union u_isp_sharpen_shtvar_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_shtvarmul : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_sharpen_shtvardiffmul : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_shtvar_mul;

/* Define the union u_isp_sharpen_oshtvardiff */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_shtvardiffthd0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_shtvardiffthd1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_shtvardiffwgt0 : 7; /* [22..16]  */
        unsigned int    reserved_0            : 1; /* [23]  */
        unsigned int    isp_sharpen_shtvardiffwgt1 : 7; /* [30..24]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_oshtvardiff;

/* Define the union u_isp_sharpen_lmtmf0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmtmf0    : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_sharpen_lmtmf1    : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_lmtmf2    : 6; /* [21..16]  */
        unsigned int    reserved_2            : 2; /* [23..22]  */
        unsigned int    isp_sharpen_lmtmf3    : 6; /* [29..24]  */
        unsigned int    reserved_3            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lmtmf0;

/* Define the union u_isp_sharpen_lmtmf1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmtmf4    : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_sharpen_lmtmf5    : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_lmtmf6    : 6; /* [21..16]  */
        unsigned int    reserved_2            : 2; /* [23..22]  */
        unsigned int    isp_sharpen_lmtmf7    : 6; /* [29..24]  */
        unsigned int    reserved_3            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lmtmf1;

/* Define the union u_isp_sharpen_lmthf0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmthf0    : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_sharpen_lmthf1    : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_lmthf2    : 6; /* [21..16]  */
        unsigned int    reserved_2            : 2; /* [23..22]  */
        unsigned int    isp_sharpen_lmthf3    : 6; /* [29..24]  */
        unsigned int    reserved_3            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lmthf0;

/* Define the union u_isp_sharpen_lmthf1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmthf4    : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_sharpen_lmthf5    : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_lmthf6    : 6; /* [21..16]  */
        unsigned int    reserved_2            : 2; /* [23..22]  */
        unsigned int    isp_sharpen_lmthf7    : 6; /* [29..24]  */
        unsigned int    reserved_3            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_lmthf1;

/* Define the union u_isp_sharpen_mhfgaind_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgaind_waddr;
/* Define the union u_isp_sharpen_mhfgaind_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgaind_wdata;
/* Define the union u_isp_sharpen_mhfgaind_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgaind_raddr;
/* Define the union u_isp_sharpen_mhfgaind_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgaind_rdata;
/* Define the union u_isp_sharpen_mhfgainud_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgainud_waddr;
/* Define the union u_isp_sharpen_mhfgainud_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgainud_wdata;
/* Define the union u_isp_sharpen_mhfgainud_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgainud_raddr;
/* Define the union u_isp_sharpen_mhfgainud_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_mhfgainud_rdata;
/* Define the union u_isp_sharpen_skin_u */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinminu  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_sharpen_skinmaxu  : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_skin_u;

/* Define the union u_isp_sharpen_skin_v */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinminv  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_sharpen_skinmaxv  : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_skin_v;

/* Define the union u_isp_sharpen_skin_cnt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skincntthd0 : 4; /* [3..0]  */
        unsigned int    isp_sharpen_skincntthd1 : 4; /* [7..4]  */
        unsigned int    isp_sharpen_skincntmul : 5; /* [12..8]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_skin_cnt;

/* Define the union u_isp_sharpen_skin_edge */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinedgesft : 4; /* [3..0]  */
        unsigned int    reserved_0            : 12; /* [15..4]  */
        unsigned int    isp_sharpen_skinedgemul : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_skin_edge;

/* Define the union u_isp_sharpen_skin_edgethd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinedgethd0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_skinedgethd1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_skinedgewgt0 : 5; /* [20..16]  */
        unsigned int    reserved_0            : 3; /* [23..21]  */
        unsigned int    isp_sharpen_skinedgewgt1 : 5; /* [28..24]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_skin_edgethd;

/* Define the union u_isp_sharpen_chrr_var */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrsft0  : 3; /* [2..0]  */
        unsigned int    isp_sharpen_chrrsft1  : 3; /* [5..3]  */
        unsigned int    isp_sharpen_chrrsft2  : 3; /* [8..6]  */
        unsigned int    isp_sharpen_chrrsft3  : 3; /* [11..9]  */
        unsigned int    isp_sharpen_chrrvarshift : 4; /* [15..12]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrr_var;

/* Define the union u_isp_sharpen_chrr_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrori0  : 8; /* [7..0]  */
        unsigned int    isp_sharpen_chrrori1  : 8; /* [15..8]  */
        unsigned int    isp_sharpen_chrrthd0  : 8; /* [23..16]  */
        unsigned int    isp_sharpen_chrrthd1  : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrr_thd;

/* Define the union u_isp_sharpen_chrr_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrgain  : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_sharpen_rmfscale  : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_chrrmul   : 10; /* [25..16]  */
        unsigned int    reserved_2            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrr_gain;

/* Define the union u_isp_sharpen_chrg_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgmul   : 14; /* [13..0]  */
        unsigned int    reserved_0            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_chrgmfmul : 14; /* [29..16]  */
        unsigned int    reserved_1            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrg_mul;

/* Define the union u_isp_sharpen_chrg_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgsft0  : 3; /* [2..0]  */
        unsigned int    isp_sharpen_chrgsft1  : 3; /* [5..3]  */
        unsigned int    isp_sharpen_chrgsft2  : 3; /* [8..6]  */
        unsigned int    isp_sharpen_chrgsft3  : 3; /* [11..9]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrg_sft;

/* Define the union u_isp_sharpen_chrg_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgori0  : 8; /* [7..0]  */
        unsigned int    isp_sharpen_chrgori1  : 8; /* [15..8]  */
        unsigned int    isp_sharpen_chrgthd0  : 8; /* [23..16]  */
        unsigned int    isp_sharpen_chrgthd1  : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrg_thd;

/* Define the union u_isp_sharpen_chrg_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrggain  : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_sharpen_chrgmfgain : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrg_gain;

/* Define the union u_isp_sharpen_chrb_var */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbsft0  : 3; /* [2..0]  */
        unsigned int    isp_sharpen_chrbsft1  : 3; /* [5..3]  */
        unsigned int    isp_sharpen_chrbsft2  : 3; /* [8..6]  */
        unsigned int    isp_sharpen_chrbsft3  : 3; /* [11..9]  */
        unsigned int    isp_sharpen_chrbvarshift : 4; /* [15..12]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrb_var;

/* Define the union u_isp_sharpen_chrb_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbori0  : 8; /* [7..0]  */
        unsigned int    isp_sharpen_chrbori1  : 8; /* [15..8]  */
        unsigned int    isp_sharpen_chrbthd0  : 8; /* [23..16]  */
        unsigned int    isp_sharpen_chrbthd1  : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrb_thd;

/* Define the union u_isp_sharpen_chrb_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbgain  : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_sharpen_bmfscale  : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_sharpen_chrbmul   : 10; /* [25..16]  */
        unsigned int    reserved_2            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_chrb_gain;

/* Define the union u_isp_sharpen_gain_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mfgain_sft : 3; /* [2..0]  */
        unsigned int    reserved_0            : 5; /* [7..3]  */
        unsigned int    isp_sharpen_lfgainwgt : 5; /* [12..8]  */
        unsigned int    reserved_1            : 3; /* [15..13]  */
        unsigned int    isp_sharpen_hfgain_sft : 3; /* [18..16]  */
        unsigned int    reserved_2            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_gain_sft;

/* Define the union u_isp_sharpen_shoot_maxgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_omaxgain  : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_sharpen_umaxgain  : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_shoot_maxgain;

/* Define the union u_isp_sharpen_detail_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_detl_oshtmul : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_sharpen_detl_ushtmul : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_detail_mul;

/* Define the union u_isp_sharpen_osht_detail */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_osht_dtl_thd0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_osht_dtl_thd1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_osht_dtl_wgt : 7; /* [22..16]  */
        unsigned int    reserved_0            : 1; /* [23]  */
        unsigned int    isp_sharpen_max_var_clip : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_osht_detail;

/* Define the union u_isp_sharpen_usht_detail */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_usht_dtl_thd0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_usht_dtl_thd1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_usht_dtl_wgt : 7; /* [22..16]  */
        unsigned int    reserved_0            : 1; /* [23]  */
        unsigned int    isp_sharpen_dtl_thdsft : 4; /* [27..24]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_usht_detail;

/* Define the union u_isp_sharpen_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_stt2lut_en : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_stt2lut_cfg;

/* Define the union u_isp_sharpen_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_stt2lut_regnew;

/* Define the union u_isp_sharpen_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_sharpen_stt2lut_clr : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_stt2lut_abn;

/* Define the union u_isp_sharpen_dlpf_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dlpfcoef0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dlpfcoef1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dlpfcoef2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dlpfsft   : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dlpf_coef;

/* Define the union u_isp_sharpen_udlpf_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_udlpfcoef0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_udlpfcoef1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_udlpfcoef2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_udlpfsft  : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_udlpf_coef;

/* Define the union u_isp_sharpen_udhsf_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_udhsfcoef0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_udhsfcoef1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_udhsfcoef2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_udhsfsft  : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_udhsf_coef;

/* Define the union u_isp_sharpen_dhsf_2dshift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 8; /* [7..0]  */
        unsigned int    reserved_1            : 8; /* [15..8]  */
        unsigned int    reserved_2            : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfsft   : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dshift;

/* Define the union u_isp_sharpen_dhsf_2dcoef0_03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef0_0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef0_1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef0_2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef0_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef0_03;

/* Define the union u_isp_sharpen_dhsf_2dcoef0_47 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef0_4 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef0_5 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef0_6 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef0_7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef0_47;

/* Define the union u_isp_sharpen_dhsf_2dcoef0_811 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef0_8 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef0_9 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef0_10 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef0_11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef0_811;

/* Define the union u_isp_sharpen_dhsf_2dcoef0_1215 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef0_12 : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef0_1215;

/* Define the union u_isp_sharpen_dhsf_2dcoef1_03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef1_0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef1_1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef1_2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef1_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef1_03;

/* Define the union u_isp_sharpen_dhsf_2dcoef1_47 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef1_4 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef1_5 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef1_6 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef1_7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef1_47;

/* Define the union u_isp_sharpen_dhsf_2dcoef1_811 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef1_8 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef1_9 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef1_10 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef1_11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef1_811;

/* Define the union u_isp_sharpen_dhsf_2dcoef1_1215 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef1_12 : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef1_1215;

/* Define the union u_isp_sharpen_dhsf_2dcoef2_03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef2_0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef2_1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef2_2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef2_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef2_03;

/* Define the union u_isp_sharpen_dhsf_2dcoef2_47 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef2_4 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef2_5 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef2_6 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef2_7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef2_47;

/* Define the union u_isp_sharpen_dhsf_2dcoef2_811 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef2_8 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef2_9 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef2_10 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef2_11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef2_811;

/* Define the union u_isp_sharpen_dhsf_2dcoef2_1215 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef2_12 : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef2_1215;

/* Define the union u_isp_sharpen_dhsf_2dcoef3_03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef3_0 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef3_1 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef3_2 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef3_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef3_03;

/* Define the union u_isp_sharpen_dhsf_2dcoef3_47 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef3_4 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef3_5 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef3_6 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef3_7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef3_47;

/* Define the union u_isp_sharpen_dhsf_2dcoef3_811 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef3_8 : 8; /* [7..0]  */
        unsigned int    isp_sharpen_dhsfcoef3_9 : 8; /* [15..8]  */
        unsigned int    isp_sharpen_dhsfcoef3_10 : 8; /* [23..16]  */
        unsigned int    isp_sharpen_dhsfcoef3_11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef3_811;

/* Define the union u_isp_sharpen_dhsf_2dcoef3_1215 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dhsfcoef3_12 : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_dhsf_2dcoef3_1215;

/* Define the union u_isp_sharpen_ldci_dither_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_ldci_dither_round : 2; /* [1..0]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_sharpen_ldci_dither_cfg;

/* Define the union u_isp_nddm_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_cac_blend_en : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_cfg;

/* Define the union u_isp_nddm_gf_th */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_th_low    : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_nddm_gf_th_high   : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_th;

/* Define the union u_isp_nddm_awb_gf_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_awb_gf_gn_low : 7; /* [6..0]  */
        unsigned int    reserved_0            : 1; /* [7]  */
        unsigned int    isp_nddm_awb_gf_gn_high : 7; /* [14..8]  */
        unsigned int    reserved_1            : 1; /* [15]  */
        unsigned int    isp_nddm_awb_gf_gn_max : 4; /* [19..16]  */
        unsigned int    reserved_2            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_awb_gf_cfg;

/* Define the union u_isp_nddm_fcr_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_fcr_gf_gain  : 5; /* [4..0]  */
        unsigned int    reserved_0            : 11; /* [15..5]  */
        unsigned int    isp_nddm_fcr_det_low  : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_fcr_gain;

/* Define the union u_isp_nddm_dm_bldrate */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_cac_blend_rate : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_dm_bldrate;

/* Define the union u_isp_nddm_ehc_gray */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_ehc_gray     : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_ehc_gray;

/* Define the union u_isp_nddm_dither_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_dith_mask    : 8; /* [7..0]  */
        unsigned int    isp_nddm_dith_ratio   : 8; /* [15..8]  */
        unsigned int    isp_nddm_dith_max     : 8; /* [23..16]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_dither_cfg;

/* Define the union u_isp_nddm_gf_lut_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_update : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_lut_update;

/* Define the union u_isp_nddm_gf_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_waddr : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_lut_waddr;

/* Define the union u_isp_nddm_gf_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_wdata : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_lut_wdata;

/* Define the union u_isp_nddm_gf_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_raddr : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_lut_raddr;

/* Define the union u_isp_nddm_gf_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_rdata : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_nddm_gf_lut_rdata;

/* Define the union u_isp_bnrshp_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_enshtctrl_byvar : 1; /* [0]  */
        unsigned int    isp_bnrshp_enshtctrl_shrink : 1; /* [1]  */
        unsigned int    isp_bnrshp_bitdepth   : 5; /* [6..2]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_cfg;

/* Define the union u_isp_bnrshp_lpfcoef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_lpfcoefud0 : 7; /* [6..0]  */
        unsigned int    isp_bnrshp_lpfcoefud1 : 7; /* [13..7]  */
        unsigned int    isp_bnrshp_lpfcoefud2 : 7; /* [20..14]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_lpfcoef;

/* Define the union u_isp_bnrshp_hsfcoef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_hsfcoefud0 : 7; /* [6..0]  */
        unsigned int    isp_bnrshp_hsfcoefud1 : 7; /* [13..7]  */
        unsigned int    isp_bnrshp_hsfcoefud2 : 7; /* [20..14]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_hsfcoef;

/* Define the union u_isp_bnrshp_mfgain_lut1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_mfgain4    : 6; /* [5..0]  */
        unsigned int    isp_bnrshp_mfgain5    : 6; /* [11..6]  */
        unsigned int    isp_bnrshp_mfgain6    : 6; /* [17..12]  */
        unsigned int    isp_bnrshp_mfgain7    : 6; /* [23..18]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_mfgain_lut1;

/* Define the union u_isp_bnrshp_mfgain_lut0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_mfgain0    : 6; /* [5..0]  */
        unsigned int    isp_bnrshp_mfgain1    : 6; /* [11..6]  */
        unsigned int    isp_bnrshp_mfgain2    : 6; /* [17..12]  */
        unsigned int    isp_bnrshp_mfgain3    : 6; /* [23..18]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_mfgain_lut0;

/* Define the union u_isp_bnrshp_hfgain_lut1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_hfgain4    : 6; /* [5..0]  */
        unsigned int    isp_bnrshp_hfgain5    : 6; /* [11..6]  */
        unsigned int    isp_bnrshp_hfgain6    : 6; /* [17..12]  */
        unsigned int    isp_bnrshp_hfgain7    : 6; /* [23..18]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_hfgain_lut1;

/* Define the union u_isp_bnrshp_hfgain_lut0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_hfgain0    : 6; /* [5..0]  */
        unsigned int    isp_bnrshp_hfgain1    : 6; /* [11..6]  */
        unsigned int    isp_bnrshp_hfgain2    : 6; /* [17..12]  */
        unsigned int    isp_bnrshp_hfgain3    : 6; /* [23..18]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_hfgain_lut0;

/* Define the union u_isp_bnrshp_darkgain_lut1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_darkgain4  : 6; /* [5..0]  */
        unsigned int    isp_bnrshp_darkgain5  : 6; /* [11..6]  */
        unsigned int    isp_bnrshp_darkgain6  : 6; /* [17..12]  */
        unsigned int    isp_bnrshp_darkgain7  : 6; /* [23..18]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_darkgain_lut1;

/* Define the union u_isp_bnrshp_darkgain_lut0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_darkgain0  : 6; /* [5..0]  */
        unsigned int    isp_bnrshp_darkgain1  : 6; /* [11..6]  */
        unsigned int    isp_bnrshp_darkgain2  : 6; /* [17..12]  */
        unsigned int    isp_bnrshp_darkgain3  : 6; /* [23..18]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_darkgain_lut0;

/* Define the union u_isp_bnrshp_shtcrtl_var_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_shtctrlvar_thd0 : 12; /* [11..0]  */
        unsigned int    isp_bnrshp_shtctrlvar_thd1 : 12; /* [23..12]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_shtcrtl_var_thd;

/* Define the union u_isp_bnrshp_shtcrtl_var_wgt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_shtctrlvar_wgt0 : 8; /* [7..0]  */
        unsigned int    isp_bnrshp_shtctrlvar_wgt1 : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_shtcrtl_var_wgt;

/* Define the union u_isp_bnrshp_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_texvarmap_thd : 12; /* [11..0]  */
        unsigned int    isp_bnrshp_darktex_gain : 4; /* [15..12]  */
        unsigned int    isp_bnrshp_hf_gain    : 4; /* [19..16]  */
        unsigned int    isp_bnrshp_mf_gain    : 4; /* [23..20]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_gain;

/* Define the union u_isp_bnrshp_dark_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_darkthd0   : 12; /* [11..0]  */
        unsigned int    isp_bnrshp_darkthd1   : 12; /* [23..12]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_dark_thd;

/* Define the union u_isp_bnrshp_shtamt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_usht_amt   : 8; /* [7..0]  */
        unsigned int    isp_bnrshp_osht_amt   : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_shtamt;

/* Define the union u_isp_bnrshp_maxchg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_umaxchg    : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_bnrshp_omaxchg    : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_maxchg;

/* Define the union u_isp_bnrshp_hvcoef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnrshp_dirrate_cd : 4; /* [3..0]  */
        unsigned int    isp_bnrshp_dirrate_hv : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnrshp_hvcoef;

/* Define the union u_isp_bnr_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_ensptnr       : 1; /* [0]  */
        unsigned int    isp_bnr_entmpnr       : 1; /* [1]  */
        unsigned int    isp_bnr_isinitial     : 1; /* [2]  */
        unsigned int    isp_bnr_frm_phase     : 2; /* [4..3]  */
        unsigned int    isp_bnr_rfr_mode      : 1; /* [5]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg;

/* Define the union u_isp_bnr_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_version        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_version;
/* Define the union u_isp_bnr_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_bltev500_win_size : 2;           /* [1..0]  */
        unsigned int isp_bnr_bltev500_n_limit_gain_sad_d : 8; /* [9..2]  */
        unsigned int isp_bnr_bltev500_pnr : 2;                /* [11..10]  */
        unsigned int isp_bnr_bltev500_en_imp_nr : 1;          /* [12]  */
        unsigned int isp_bnr_bltev500_imp_nr_str : 6;         /* [18..13]  */
        unsigned int isp_bnr_bltev500_wt_ccoef : 2;           /* [20..19]  */
        unsigned int isp_bnr_bltev500_wt_cmax : 6;            /* [26..21]  */
        unsigned int isp_bnr_blt_edge_mode : 1;               /* [27]  */
        unsigned int isp_bnr_jnlm_lum_sel : 2;                /* [29..28]  */
        unsigned int reserved_0 : 2;                          /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg0;

/* Define the union u_isp_bnr_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain0 : 17; /* [16..0]  */
        unsigned int    isp_bnr_dgain_value   : 6   ; /* [22..17]  */
        unsigned int    isp_bnr_dgain_slp     : 2   ; /* [24..23]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg1;

/* Define the union u_isp_bnr_cfg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain1 : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg2;

/* Define the union u_isp_bnr_cfg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain2 : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg3;

/* Define the union u_isp_bnr_cfg4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain3 : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg4;

/* Define the union u_isp_bnr_cfg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain4 : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg5;

/* Define the union u_isp_bnr_cfg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain5 : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg6;

/* Define the union u_isp_bnr_cfg7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain6 : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg7;

/* Define the union u_isp_bnr_cfg8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_lmt_gain7 : 17; /* [16..0]  */
        unsigned int    isp_bnr_en_area_alt   : 1; /* [17]  */
        unsigned int    isp_bnr_mdet_size     : 3; /* [20..18]  */
        unsigned int    isp_bnr_mdet_cor_level : 8; /* [28..21]  */
        unsigned int    isp_bnr_b_deform_md_wim : 1; /* [29]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg8;

/* Define the union u_isp_bnr_cfg9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_rnt_th        : 8; /* [7..0]  */
        unsigned int    isp_bnr_rnt_wid       : 8; /* [15..8]  */
        unsigned int    isp_bnr_nr_strength_st_int : 9; /* [24..16]  */
        unsigned int    isp_bnr_mdet_size_alpha : 6   ; /* [30..25]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg9;

/* Define the union u_isp_bnr_cfg10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_nr_strength_mv_int : 9; /* [8..0]  */
        unsigned int    isp_bnr_nr_strength_slope : 10; /* [18..9]  */
        unsigned int    isp_bnr_en_mixing     : 2; /* [20..19]  */
        unsigned int    isp_bnr_target_noise_ratio : 8; /* [28..21]  */
        unsigned int    reserved_0            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg10;

/* Define the union u_isp_bnr_cfg11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_mix_gain_0_r  : 8; /* [7..0]  */
        unsigned int    isp_bnr_mix_gain_0_b  : 8; /* [15..8]  */
        unsigned int    isp_bnr_wb_gain_r     : 10; /* [25..16]  */
        unsigned int    isp_bnr_rb_diff_en    : 1; /* [26]  */
        unsigned int    reserved_0            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg11;

/* Define the union u_isp_bnr_cfg12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_wb_gain_b     : 10; /* [9..0]  */
        unsigned int    isp_bnr_wb_gain_inv_r : 16; /* [25..10]  */
        unsigned int    reserved_0            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg12;

/* Define the union u_isp_bnr_cfg13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_wb_gain_inv_b : 16; /* [15..0]  */
        unsigned int    isp_bnr_bnrlscen      : 1; /* [16]  */
        unsigned int    isp_bnr_bnrlscratio   : 4; /* [20..17]  */
        unsigned int    isp_bnr_tnr1_ratio    : 7; /* [27..21]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg13;

/* Define the union u_isp_bnr_cfg14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_wdr_enable    : 2; /* [1..0]  */
        unsigned int    isp_bnr_wdr_en_fusion : 2; /* [3..2]  */
        unsigned int    isp_bnr_wdr_map_flt_mod : 1; /* [4]  */
        unsigned int    isp_bnr_wdr_map_gain  : 5; /* [9..5]  */
        unsigned int    isp_bnr_en_2nd_tmp_nr : 1; /* [10]  */
        unsigned int    reserved_0            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg14;

/* Define the union u_isp_bnr_cfg15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_inputraw_ratio : 7; /* [6..0]  */
        unsigned int    isp_bnr_mdet_mixratio : 7; /* [13..7]  */
        unsigned int    isp_bnr_en_irmd       : 1; /* [14]  */
        unsigned int    isp_bnr_irmd_mixratio_rgb : 7; /* [21..15]  */
        unsigned int    isp_bnr_alphamax_st   : 8; /* [29..22]  */
        unsigned int    isp_bnr_en_aimd       : 1   ; /* [30]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg15;

/* Define the union u_isp_bnr_cfg16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_coring_hig    : 16; /* [15..0]  */
        unsigned int    isp_bnr_coring_enable : 1; /* [16]  */
        unsigned int    isp_bnr_jnlm_gain     : 8; /* [24..17]  */
        unsigned int    isp_bnr_maxv_mode     : 1; /* [25]  */
        unsigned int    isp_bnr_mixgain_sprs_en : 1; /* [26]  */
        unsigned int    reserved_0            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg16;

/* Define the union u_isp_bnr_cfg17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_mixing_mot_ratio : 6; /* [5..0]  */
        unsigned int    isp_bnr_mixing_mot_thresh : 8; /* [13..6]  */
        unsigned int    isp_bnr_mixing_mot_gain : 5; /* [18..14]  */
        unsigned int    reserved_0            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg17;

/* Define the union u_isp_bnr_cfg18 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_ynet_alpha : 8;    /* [7..0]  */
        unsigned int isp_bnr_ynet_mot_bitw : 1; /* [8]  */
        unsigned int reserved_0 : 23;           /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg18;

/* Define the union u_isp_bnr_cfg19 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_blt_mix_r1_n0 : 16; /* [15..0]  */
        unsigned int isp_bnr_blt_mix_r2_n0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg19;

/* Define the union u_isp_bnr_cfg20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_blt_mix_m1_n0 : 7; /* [6..0]  */
        unsigned int isp_bnr_blt_mix_m2_n0 : 7; /* [13..7]  */
        unsigned int reserved_0 : 18;           /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg20;

/* Define the union u_isp_bnr_cfg21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_blt_mix_r1_n1 : 16; /* [15..0]  */
        unsigned int isp_bnr_blt_mix_r2_n1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg21;

/* Define the union u_isp_bnr_cfg22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_blt_mix_m1_n1 : 7; /* [6..0]  */
        unsigned int isp_bnr_blt_mix_m2_n1 : 7; /* [13..7]  */
        unsigned int reserved_0 : 18;           /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg22;

/* Define the union u_isp_bnr_cfg23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_blt_clipl : 16; /* [15..0]  */
        unsigned int isp_bnr_blt_cliph : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg23;

/* Define the union u_isp_bnr_cfg24 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_blt_weight_c1 : 5; /* [4..0]  */
        unsigned int isp_bnr_blt_weight_c : 5;  /* [9..5]  */
        unsigned int reserved_0 : 22;           /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg24;

/* Define the union u_isp_bnr_cfg25 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_std_mix_thr_3 : 8; /* [7..0]  */
        unsigned int isp_bnr_std_mix_thr_2 : 8; /* [15..8]  */
        unsigned int isp_bnr_std_mix_thr_1 : 8; /* [23..16]  */
        unsigned int isp_bnr_std_mix_thr_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg25;

/* Define the union u_isp_bnr_cfg26 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_std_mix_thr_7 : 8; /* [7..0]  */
        unsigned int isp_bnr_std_mix_thr_6 : 8; /* [15..8]  */
        unsigned int isp_bnr_std_mix_thr_5 : 8; /* [23..16]  */
        unsigned int isp_bnr_std_mix_thr_4 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg26;

/* Define the union u_isp_bnr_cfg_gflt_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_bnr_geq_edge_thresh : 16; /* [16..1]  */
        unsigned int    isp_bnr_geq_denoise_str : 5; /* [21..17]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg_gflt_1;

/* Define the union u_isp_bnr_cfg_gflt_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_geq_str_mv    : 6; /* [5..0]  */
        unsigned int    isp_bnr_geq_str_st    : 6; /* [11..6]  */
        unsigned int    isp_bnr_geq_th        : 8; /* [19..12]  */
        unsigned int    isp_bnr_geq_slope     : 8; /* [27..20]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_cfg_gflt_2;

/* Define the union u_isp_bnr_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_stt2lut_en    : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_stt2lut_cfg;

/* Define the union u_isp_bnr_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_stt2lut_regnew;

/* Define the union u_isp_bnr_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_stt2lut_info  : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_bnr_stt2lut_clr   : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_stt2lut_abn;

/* Define the union u_isp_bnr_noisesd_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_even_waddr;
/* Define the union u_isp_bnr_noisesd_even_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesd_even_wdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesd_s_even_wdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_even_wdata;

/* Define the union u_isp_bnr_noisesd_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_even_raddr;
/* Define the union u_isp_bnr_noisesd_even_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesd_even_rdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesd_s_even_rdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_even_rdata;

/* Define the union u_isp_bnr_noisesd_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_odd_waddr;
/* Define the union u_isp_bnr_noisesd_odd_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesd_odd_wdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesd_s_odd_wdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_odd_wdata;

/* Define the union u_isp_bnr_noisesd_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_odd_raddr;
/* Define the union u_isp_bnr_noisesd_odd_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesd_odd_rdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesd_s_odd_rdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesd_odd_rdata;

/* Define the union u_isp_bnr_noisesds_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_even_waddr;
/* Define the union u_isp_bnr_noisesds_even_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesds_even_wdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesds_s_even_wdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_even_wdata;

/* Define the union u_isp_bnr_noisesds_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_even_raddr;
/* Define the union u_isp_bnr_noisesds_even_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesds_even_rdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesds_s_even_rdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_even_rdata;

/* Define the union u_isp_bnr_noisesds_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_odd_waddr;
/* Define the union u_isp_bnr_noisesds_odd_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesds_odd_wdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesds_s_odd_wdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_odd_wdata;

/* Define the union u_isp_bnr_noisesds_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_odd_raddr;
/* Define the union u_isp_bnr_noisesds_odd_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_noisesds_odd_rdata : 15; /* [14..0]  */
        unsigned int    isp_bnr_noisesds_s_odd_rdata : 15; /* [29..15]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noisesds_odd_rdata;

/* Define the union u_isp_bnr_noiseinv_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_even_waddr;
/* Define the union u_isp_bnr_noiseinv_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_even_wdata;
/* Define the union u_isp_bnr_noiseinv_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_even_raddr;
/* Define the union u_isp_bnr_noiseinv_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_even_rdata;
/* Define the union u_isp_bnr_noiseinv_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_odd_waddr;
/* Define the union u_isp_bnr_noiseinv_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_odd_wdata;
/* Define the union u_isp_bnr_noiseinv_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_odd_raddr;
/* Define the union u_isp_bnr_noiseinv_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_odd_rdata;
/* Define the union u_isp_bnr_noiseinv_mag_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_even_waddr;
/* Define the union u_isp_bnr_noiseinv_mag_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_even_wdata;
/* Define the union u_isp_bnr_noiseinv_mag_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_even_raddr;
/* Define the union u_isp_bnr_noiseinv_mag_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_even_rdata;
/* Define the union u_isp_bnr_noiseinv_mag_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_odd_waddr;
/* Define the union u_isp_bnr_noiseinv_mag_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_odd_wdata;
/* Define the union u_isp_bnr_noiseinv_mag_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_odd_raddr;
/* Define the union u_isp_bnr_noiseinv_mag_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_mag_odd_rdata;
/* Define the union u_isp_bnr_fbratiotable_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_fbratiotable_waddr;
/* Define the union u_isp_bnr_fbratiotable_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_fbratiotable_wdata;
/* Define the union u_isp_bnr_fbratiotable_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_fbratiotable_raddr;
/* Define the union u_isp_bnr_fbratiotable_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_fbratiotable_rdata;
/* Define the union u_isp_bnr_coring_low_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_even_waddr;
/* Define the union u_isp_bnr_coring_low_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_even_wdata;
/* Define the union u_isp_bnr_coring_low_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_even_raddr;
/* Define the union u_isp_bnr_coring_low_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_even_rdata;
/* Define the union u_isp_bnr_coring_low_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_odd_waddr;
/* Define the union u_isp_bnr_coring_low_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_odd_wdata;
/* Define the union u_isp_bnr_coring_low_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_odd_raddr;
/* Define the union u_isp_bnr_coring_low_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_coring_low_odd_rdata;
/* Define the union u_isp_bnr_noiseinv_1_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_even_waddr;
/* Define the union u_isp_bnr_noiseinv_1_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_even_wdata;
/* Define the union u_isp_bnr_noiseinv_1_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_even_raddr;
/* Define the union u_isp_bnr_noiseinv_1_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_even_rdata;
/* Define the union u_isp_bnr_noiseinv_1_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_odd_waddr;
/* Define the union u_isp_bnr_noiseinv_1_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_odd_wdata;
/* Define the union u_isp_bnr_noiseinv_1_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_odd_raddr;
/* Define the union u_isp_bnr_noiseinv_1_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_1_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_noiseinv_1_odd_rdata;
/* Define the union u_isp_bnr_mixgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_mixgain_waddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_mixgain_waddr;
/* Define the union u_isp_bnr_mixgain_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_mixgain_wdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_mixgain_wdata;
/* Define the union u_isp_bnr_mixgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_mixgain_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_mixgain_raddr;
/* Define the union u_isp_bnr_mixgain_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_mixgain_rdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bnr_mixgain_rdata;

/* Define the union u_isp_bnr_ynet_fg_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_fg_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_fg_waddr;
/* Define the union u_isp_bnr_ynet_fg_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_fg_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_fg_wdata;
/* Define the union u_isp_bnr_ynet_fg_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_fg_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_fg_raddr;
/* Define the union u_isp_bnr_ynet_fg_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_fg_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_fg_rdata;
/* Define the union u_isp_bnr_ynet_bg_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_bg_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_bg_waddr;
/* Define the union u_isp_bnr_ynet_bg_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_bg_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_bg_wdata;
/* Define the union u_isp_bnr_ynet_bg_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_bg_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_bg_raddr;
/* Define the union u_isp_bnr_ynet_bg_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_bg_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_bg_rdata;
/* Define the union u_isp_bnr_ynet_mot_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_mot_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_mot_waddr;
/* Define the union u_isp_bnr_ynet_mot_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_mot_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_mot_wdata;
/* Define the union u_isp_bnr_ynet_mot_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_mot_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_mot_raddr;
/* Define the union u_isp_bnr_ynet_mot_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_ynet_mot_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_ynet_mot_rdata;
/* Define the union u_isp_mlsc_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_stt2lut_en   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_stt2lut_cfg;

/* Define the union u_isp_mlsc_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_stt2lut_regnew;

/* Define the union u_isp_mlsc_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_mlsc_stt2lut_clr  : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_stt2lut_abn;

/* Define the union u_isp_mlsc_winnum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_numh         : 6; /* [5..0]  */
        unsigned int    reserved_0            : 10; /* [15..6]  */
        unsigned int    isp_mlsc_numv         : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winnum;

/* Define the union u_isp_mlsc_winx0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax0      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx0        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx0;

/* Define the union u_isp_mlsc_winx1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax1      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx1        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx1;

/* Define the union u_isp_mlsc_winx2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax2      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx2        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx2;

/* Define the union u_isp_mlsc_winx3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax3      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx3        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx3;

/* Define the union u_isp_mlsc_winx4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax4      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx4        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx4;

/* Define the union u_isp_mlsc_winx5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax5      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx5        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx5;

/* Define the union u_isp_mlsc_winx6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax6      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx6        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx6;

/* Define the union u_isp_mlsc_winx7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax7      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx7        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx7;

/* Define the union u_isp_mlsc_winx8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax8      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx8        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx8;

/* Define the union u_isp_mlsc_winx9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax9      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx9        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx9;

/* Define the union u_isp_mlsc_winx10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax10     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx10       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx10;

/* Define the union u_isp_mlsc_winx11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax11     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx11       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx11;

/* Define the union u_isp_mlsc_winx12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax12     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx12       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx12;

/* Define the union u_isp_mlsc_winx13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax13     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx13       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx13;

/* Define the union u_isp_mlsc_winx14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax14     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx14       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx14;

/* Define the union u_isp_mlsc_winx15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax15     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx15       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx15;

/* Define the union u_isp_mlsc_winy0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay0      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy0        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy0;

/* Define the union u_isp_mlsc_winy1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay1      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy1        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy1;

/* Define the union u_isp_mlsc_winy2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay2      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy2        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy2;

/* Define the union u_isp_mlsc_winy3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay3      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy3        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy3;

/* Define the union u_isp_mlsc_winy4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay4      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy4        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy4;

/* Define the union u_isp_mlsc_winy5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay5      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy5        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy5;

/* Define the union u_isp_mlsc_winy6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay6      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy6        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy6;

/* Define the union u_isp_mlsc_winy7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay7      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy7        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy7;

/* Define the union u_isp_mlsc_width_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_width_offset : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_width_offset;

/* Define the union u_isp_mlsc_mesh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_mesh_str     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_mlsc_mesh_scale   : 3; /* [18..16]  */
        unsigned int    reserved_1            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_mesh;

/* Define the union u_isp_mlsc_rgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_rgain_waddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_rgain_waddr;
/* Define the union u_isp_mlsc_rgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_rgain_wdata  : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_rgain_wdata;

/* Define the union u_isp_mlsc_rgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_rgain_raddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_rgain_raddr;
/* Define the union u_isp_mlsc_rgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_rgain_rdata  : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_rgain_rdata;

/* Define the union u_isp_mlsc_grgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_grgain_waddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_grgain_waddr;
/* Define the union u_isp_mlsc_grgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_grgain_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_grgain_wdata;

/* Define the union u_isp_mlsc_grgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_grgain_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_grgain_raddr;
/* Define the union u_isp_mlsc_grgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_grgain_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_grgain_rdata;

/* Define the union u_isp_mlsc_bgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_bgain_waddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_bgain_waddr;
/* Define the union u_isp_mlsc_bgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_bgain_wdata  : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_bgain_wdata;

/* Define the union u_isp_mlsc_bgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_bgain_raddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_bgain_raddr;
/* Define the union u_isp_mlsc_bgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_bgain_rdata  : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_bgain_rdata;

/* Define the union u_isp_mlsc_gbgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_gbgain_waddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_gbgain_waddr;
/* Define the union u_isp_mlsc_gbgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_gbgain_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_gbgain_wdata;

/* Define the union u_isp_mlsc_gbgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_gbgain_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_gbgain_raddr;
/* Define the union u_isp_mlsc_gbgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_gbgain_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_gbgain_rdata;

/* Define the union u_isp_mlsc_winx16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax16     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx16       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx16;

/* Define the union u_isp_mlsc_winx17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax17     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx17       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx17;

/* Define the union u_isp_mlsc_winx18 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax18     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx18       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx18;

/* Define the union u_isp_mlsc_winx19 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax19     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx19       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx19;

/* Define the union u_isp_mlsc_winx20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax20     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx20       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx20;

/* Define the union u_isp_mlsc_winx21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax21     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx21       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx21;

/* Define the union u_isp_mlsc_winx22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax22     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx22       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx22;

/* Define the union u_isp_mlsc_winx23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax23     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx23       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx23;

/* Define the union u_isp_mlsc_winx24 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax24     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx24       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx24;

/* Define the union u_isp_mlsc_winx25 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax25     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx25       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx25;

/* Define the union u_isp_mlsc_winx26 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax26     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx26       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx26;

/* Define the union u_isp_mlsc_winx27 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax27     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx27       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx27;

/* Define the union u_isp_mlsc_winx28 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax28     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx28       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx28;

/* Define the union u_isp_mlsc_winx29 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax29     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx29       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx29;

/* Define the union u_isp_mlsc_winx30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax30     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx30       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx30;

/* Define the union u_isp_mlsc_winx31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltax31     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invx31       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winx31;

/* Define the union u_isp_mlsc_winy8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay8      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy8        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy8;

/* Define the union u_isp_mlsc_winy9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay9      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy9        : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy9;

/* Define the union u_isp_mlsc_winy10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay10     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy10       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy10;

/* Define the union u_isp_mlsc_winy11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay11     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy11       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy11;

/* Define the union u_isp_mlsc_winy12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay12     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy12       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy12;

/* Define the union u_isp_mlsc_winy13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay13     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy13       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy13;

/* Define the union u_isp_mlsc_winy14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay14     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy14       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy14;

/* Define the union u_isp_mlsc_winy15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mlsc_deltay15     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_mlsc_invy15       : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_mlsc_winy15;

/* Define the union u_isp_wdr_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_wdr_fusion_data_mode : 2; /* [2..1]  */
        unsigned int    reserved_1            : 1; /* [3]  */
        unsigned int    isp_wdr_fusionmode    : 1; /* [4]  */
        unsigned int    isp_wdr_revmode       : 1; /* [5]  */
        unsigned int    isp_wdr_sigma_mode_select : 1; /* [6]  */
        unsigned int    reserved_2            : 9; /* [15..7]  */
        unsigned int    isp_wdr_erosion_en    : 1; /* [16]  */
        unsigned int    reserved_3            : 3; /* [19..17]  */
        unsigned int    isp_wdr_mdt_en        : 1; /* [20]  */
        unsigned int    reserved_4            : 3; /* [23..21]  */
        unsigned int    isp_wdr_grayscale_mode : 1; /* [24]  */
        unsigned int    isp_wdr_pixelfuion_en : 1; /* [25]  */
        unsigned int    isp_wdr_fuionblend_en : 1; /* [26]  */
        unsigned int    reserved_5            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_ctrl;

/* Define the union u_isp_wdr_f0_inblc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f0_inblc_gr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;           /* [15]  */
        unsigned int isp_wdr_f0_inblc_r : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_f0_inblc0;

/* Define the union u_isp_wdr_f0_inblc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f0_inblc_b : 15;  /* [14..0]  */
        unsigned int reserved_0 : 1;           /* [15]  */
        unsigned int isp_wdr_f0_inblc_gb : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_f0_inblc1;

/* Define the union u_isp_wdr_f1_inblc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f1_inblc_gr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;           /* [15]  */
        unsigned int isp_wdr_f1_inblc_r : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_f1_inblc0;

/* Define the union u_isp_wdr_f1_inblc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f1_inblc_b : 15;  /* [14..0]  */
        unsigned int reserved_0 : 1;           /* [15]  */
        unsigned int isp_wdr_f1_inblc_gb : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_f1_inblc1;

/* Define the union u_isp_wdr_outblc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_bsaveblc      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_outblc;

/* Define the union u_isp_wdr_expovalue */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_expovalue1    : 14; /* [13..0]  */
        unsigned int    reserved_0            : 2; /* [15..14]  */
        unsigned int    isp_wdr_expovalue0    : 14; /* [29..16]  */
        unsigned int    reserved_1            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_expovalue;

/* Define the union u_isp_wdr_exporratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_exporratio0   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_exporratio;

/* Define the union u_isp_wdr_blc_comp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_blc_comp0     : 26; /* [25..0]  */
        unsigned int    reserved_0            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_blc_comp;

/* Define the union u_isp_wdr_maxratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_maxratio      : 17; /* [16..0]  */
        unsigned int    reserved_0            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_maxratio;

/* Define the union u_isp_wdr_saturate_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_saturate_thr  : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_saturate_thr;

/* Define the union u_isp_wdr_still_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f0_still_thr  : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_still_thr;

/* Define the union u_isp_wdr_mdtlbld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdtlbld       : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_mdtlbld;

/* Define the union u_isp_wdr_mdt_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdt_full_thr  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_wdr_mdt_still_thr : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_mdt_thr;

/* Define the union u_isp_wdr_forcelong_para */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_forcelong_high_thd : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_wdr_forcelong_low_thd : 12; /* [27..16]  */
        unsigned int    isp_wdr_forcelong_smooth_en : 1; /* [28]  */
        unsigned int    isp_wdr_forcelong_en  : 1; /* [29]  */
        unsigned int    reserved_1            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_forcelong_para;

/* Define the union u_isp_wdr_mdt_noisefloor */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdt_nosfloor  : 7; /* [6..0]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_mdt_noisefloor;

/* Define the union u_isp_wdr_mdthr_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdthr_hig_gain : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_wdr_mdthr_low_gain : 8; /* [23..16]  */
        unsigned int    reserved_1            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_mdthr_gain;

/* Define the union u_isp_wdr_shortexpo_chk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_shortchk_thd  : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_wdr_shortexpo_chk : 1; /* [16]  */
        unsigned int    reserved_1            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_shortexpo_chk;

/* Define the union u_isp_wdr_pix_avg_diff */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_pixel_avg_max_diff : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_pix_avg_diff;

/* Define the union u_isp_wdr_mask_similar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mask_similar_cnt : 4; /* [3..0]  */
        unsigned int    reserved_0            : 12; /* [15..4]  */
        unsigned int    isp_wdr_mask_similar_thr : 6; /* [21..16]  */
        unsigned int    reserved_1            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_mask_similar;

/* Define the union u_isp_wdr_wgtidx_blendratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_bldrlhfidx    : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_wgtidx_blendratio;

/* Define the union u_isp_wdr_wgtidx_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_long_thr      : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_wdr_short_thr     : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_wgtidx_thr;

/* Define the union u_isp_wdr_dftwgt_fl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_dftwgt_fl     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_dftwgt_fl;

/* Define the union u_isp_wdr_sensor_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_sqrt_dgain_g  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 13; /* [15..3]  */
        unsigned int    isp_wdr_sqrt_again_g  : 3; /* [18..16]  */
        unsigned int    reserved_1            : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_sensor_gain;

/* Define the union u_isp_wdr_fusion_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_f1_thr : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_wdr_fusion_f0_thr : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_fusion_thr;

/* Define the union u_isp_wdr_fusion_sat_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_saturate_thd : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_fusion_sat_thd;

/* Define the union u_isp_wdr_awb_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_rgain         : 10; /* [9..0]  */
        unsigned int    isp_wdr_ggain         : 10; /* [19..10]  */
        unsigned int    isp_wdr_bgain         : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_awb_gain;

/* Define the union u_isp_wdr_awb_rev_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_rev_rgain     : 10; /* [9..0]  */
        unsigned int    isp_wdr_rev_ggain     : 10; /* [19..10]  */
        unsigned int    isp_wdr_rev_bgain     : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_awb_rev_gain;

/* Define the union u_isp_wdr_fusion_blend */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_lhf_blend_wgt : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_fusion_blend;

/* Define the union u_isp_wdr_lut_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_snoise_lut_update : 1; /* [0]  */
        unsigned int    isp_wdr_lnoise_lut_update : 1; /* [1]  */
        unsigned int    isp_wdr_normalize_lut_update : 1; /* [2]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_update;

/* Define the union u_isp_wdr_lut_seg_idxbase0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_seg_idx_base2 : 10; /* [9..0]  */
        unsigned int    isp_wdr_seg_idx_base1 : 10; /* [19..10]  */
        unsigned int    isp_wdr_seg_idx_base0 : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_seg_idxbase0;

/* Define the union u_isp_wdr_lut_seg_idxbase1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_seg_idx_base5 : 10; /* [9..0]  */
        unsigned int    isp_wdr_seg_idx_base4 : 10; /* [19..10]  */
        unsigned int    isp_wdr_seg_idx_base3 : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_seg_idxbase1;

/* Define the union u_isp_wdr_lut_seg_idxbase2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_seg_idx_base7 : 10; /* [9..0]  */
        unsigned int    isp_wdr_seg_idx_base6 : 10; /* [19..10]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_seg_idxbase2;

/* Define the union u_isp_wdr_lut_seg_maxval0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_seg_maxval2   : 10; /* [9..0]  */
        unsigned int    isp_wdr_seg_maxval1   : 10; /* [19..10]  */
        unsigned int    isp_wdr_seg_maxval0   : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_seg_maxval0;

/* Define the union u_isp_wdr_lut_seg_maxval1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_seg_maxval5   : 10; /* [9..0]  */
        unsigned int    isp_wdr_seg_maxval4   : 10; /* [19..10]  */
        unsigned int    isp_wdr_seg_maxval3   : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_seg_maxval1;

/* Define the union u_isp_wdr_lut_seg_maxval2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_seg_maxval7   : 10; /* [9..0]  */
        unsigned int    isp_wdr_seg_maxval6   : 10; /* [19..10]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lut_seg_maxval2;

/* Define the union u_isp_wdr_fusion_data_select */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_data_select : 1   ; /* [0]  */
        unsigned int    isp_wdr_fusion_clamp_en : 1   ; /* [1]  */
        unsigned int    isp_wdr_fusion_aux_data_en : 1   ; /* [2]  */
        unsigned int    isp_wdr_fusion_sepr_thd_en : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_fusion_data_para;

/* Define the union u_isp_wdr_fusion_lf_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_lf_high_thd : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_wdr_fusion_lf_low_thd : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_fusion_lf_thd;

/* Define the union u_isp_wdr_fusion_sat_rb_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_saturate_thd_r : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;                     /* [15..14]  */
        unsigned int isp_wdr_fusion_saturate_thd_b : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;                     /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_sat_rb_thd;

/* Define the union u_isp_wdr_fusion_bld_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_bld_high_thd : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                   /* [15..12]  */
        unsigned int isp_wdr_fusion_bld_low_thd : 12;  /* [27..16]  */
        unsigned int reserved_1 : 4;                   /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_bld_thd;

/* Define the union u_isp_wdr_fusion_calc_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_f1_calc_thr : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                  /* [15..12]  */
        unsigned int isp_wdr_fusion_f0_calc_thr : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;                  /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_calc_thd;

/* Define the union u_isp_wdr_snoise_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_snoise_lut_waddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_snoise_lut_waddr;

/* Define the union u_isp_wdr_snoise_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_snoise_lut_wdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_snoise_lut_wdata;

/* Define the union u_isp_wdr_snoise_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_snoise_lut_raddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_snoise_lut_raddr;

/* Define the union u_isp_wdr_snoise_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_snoise_lut_rdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_snoise_lut_rdata;

/* Define the union u_isp_wdr_lnoise_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_lnoise_lut_waddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lnoise_lut_waddr;

/* Define the union u_isp_wdr_lnoise_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_lnoise_lut_wdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lnoise_lut_wdata;

/* Define the union u_isp_wdr_lnoise_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_lnoise_lut_raddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lnoise_lut_raddr;

/* Define the union u_isp_wdr_lnoise_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_lnoise_lut_rdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_lnoise_lut_rdata;

/* Define the union u_isp_wdr_normalize_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_normalize_lut_waddr : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_normalize_lut_waddr;

/* Define the union u_isp_wdr_normalize_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_normalize_lut_wdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_normalize_lut_wdata;

/* Define the union u_isp_wdr_normalize_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_normalize_lut_raddr : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_normalize_lut_raddr;

/* Define the union u_isp_wdr_normalize_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_normalize_lut_rdata : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wdr_normalize_lut_rdata;

/* Define the union u_isp_dehaze_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_version     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_version;
/* Define the union u_isp_dehaze_blk_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_block_sizev : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_dehaze_block_sizeh : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_blk_size;

/* Define the union u_isp_dehaze_blk_sum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_block_sum  : 11; /* [10..0]  */
        unsigned int    reserved_0            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_blk_sum;

/* Define the union u_isp_dehaze_dc_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_dc_numv    : 5; /* [4..0]  */
        unsigned int    isp_dehaze_dc_numh    : 5; /* [9..5]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_dc_size;

/* Define the union u_isp_dehaze_x */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_phase_x    : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_x;

/* Define the union u_isp_dehaze_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_phase_y    : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_y;

/* Define the union u_isp_dehaze_air */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_air_b      : 10; /* [9..0]  */
        unsigned int    isp_dehaze_air_g      : 10; /* [19..10]  */
        unsigned int    isp_dehaze_air_r      : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_air;

/* Define the union u_isp_dehaze_thld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_thld_tb    : 10; /* [9..0]  */
        unsigned int    isp_dehaze_thld_tg    : 10; /* [19..10]  */
        unsigned int    isp_dehaze_thld_tr    : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_thld;

/* Define the union u_isp_dehaze_gstrth */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_gstrth     : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_gstrth;

/* Define the union u_isp_dehaze_blthld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_blthld     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_blthld;

/* Define the union u_isp_dehaze_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_stt_size   : 16; /* [15..0]  */
        unsigned int    isp_dehaze_stt_bst    : 4; /* [19..16]  */
        unsigned int    isp_dehaze_stt_en     : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stt_bst;

/* Define the union u_isp_dehaze_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_stt_info   : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_dehaze_stt_clr    : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stt_abn;

/* Define the union u_isp_dehaze_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_stt2lut_en : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stt2lut_cfg;

/* Define the union u_isp_dehaze_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stt2lut_regnew;

/* Define the union u_isp_dehaze_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_dehaze_stt2lut_clr : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stt2lut_abn;

/* Define the union u_isp_dehaze_smlmapoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_smlmapoffset : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_smlmapoffset;

/* Define the union u_isp_dehaze_stat_point */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_statstartx : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_dehaze_statendx   : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stat_point;

/* Define the union u_isp_dehaze_stat_num */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_stat_numv  : 5; /* [4..0]  */
        unsigned int    isp_dehaze_stat_numh  : 5; /* [9..5]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_stat_num;

/* Define the union u_isp_dehaze_minstat_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_waddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_minstat_waddr;

/* Define the union u_isp_dehaze_minstat_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_wdata_l : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_dehaze_minstat_wdata_h : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_minstat_wdata;

/* Define the union u_isp_dehaze_minstat_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_raddr : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_minstat_raddr;

/* Define the union u_isp_dehaze_minstat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_rdata_l : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_dehaze_minstat_rdata_h : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_minstat_rdata;

/* Define the union u_isp_dehaze_maxstat_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_waddr : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_maxstat_waddr;

/* Define the union u_isp_dehaze_maxstat_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_wdata : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_maxstat_wdata;

/* Define the union u_isp_dehaze_maxstat_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_raddr : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_maxstat_raddr;

/* Define the union u_isp_dehaze_maxstat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_rdata : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_maxstat_rdata;

/* Define the union u_isp_dehaze_prestat_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_waddr : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_prestat_waddr;

/* Define the union u_isp_dehaze_prestat_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_prestat_wdata;

/* Define the union u_isp_dehaze_prestat_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_raddr : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_prestat_raddr;

/* Define the union u_isp_dehaze_prestat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_prestat_rdata;

/* Define the union u_isp_dehaze_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_waddr  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_lut_waddr;

/* Define the union u_isp_dehaze_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_wdata  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_lut_wdata;

/* Define the union u_isp_dehaze_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_raddr  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_lut_raddr;

/* Define the union u_isp_dehaze_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_rdata  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_lut_rdata;

/* Define the union u_isp_expander_bitw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_bitw_out : 5; /* [4..0]  */
        unsigned int    reserved_0            : 11; /* [15..5]  */
        unsigned int    isp_expander_bitw_in  : 5; /* [20..16]  */
        unsigned int    reserved_1            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_bitw;

/* Define the union u_isp_expander_offsetr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_offset_r : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_offsetr;

/* Define the union u_isp_expander_offsetgr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_offset_gr : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_offsetgr;

/* Define the union u_isp_expander_offsetgb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_offset_gb : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_offsetgb;

/* Define the union u_isp_expander_offsetb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_expander_offset_b : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_offsetb;

/* Define the union u_isp_expander_lut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_lut_waddr;
/* Define the union u_isp_expander_lut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_lut_wdata;
/* Define the union u_isp_expander_lut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_lut_raddr;
/* Define the union u_isp_expander_lut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_expander_lut_rdata;
/* Define the union u_isp_gamma_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    reserved_1            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_cfg;

/* Define the union u_isp_gamma_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_pos_0       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_gamma_pos_1       : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_pos0;

/* Define the union u_isp_gamma_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_pos_2       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_gamma_pos_3       : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_pos1;

/* Define the union u_isp_gamma_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_pos_4       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_gamma_pos_5       : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_pos2;

/* Define the union u_isp_gamma_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_pos_6       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_gamma_pos_7       : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_pos3;

/* Define the union u_isp_gamma_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_inseg_0     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_gamma_inseg_1     : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_inseg0;

/* Define the union u_isp_gamma_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_inseg_2     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_gamma_inseg_3     : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_inseg1;

/* Define the union u_isp_gamma_inseg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_inseg_4     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_gamma_inseg_5     : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_inseg2;

/* Define the union u_isp_gamma_inseg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_inseg_6     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_gamma_inseg_7     : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_inseg3;

/* Define the union u_isp_gamma_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_step0       : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    isp_gamma_step1       : 3; /* [6..4]  */
        unsigned int    reserved_1            : 1; /* [7]  */
        unsigned int    isp_gamma_step2       : 3; /* [10..8]  */
        unsigned int    reserved_2            : 1; /* [11]  */
        unsigned int    isp_gamma_step3       : 3; /* [14..12]  */
        unsigned int    reserved_3            : 1; /* [15]  */
        unsigned int    isp_gamma_step4       : 3; /* [18..16]  */
        unsigned int    reserved_4            : 1; /* [19]  */
        unsigned int    isp_gamma_step5       : 3; /* [22..20]  */
        unsigned int    reserved_5            : 1; /* [23]  */
        unsigned int    isp_gamma_step6       : 3; /* [26..24]  */
        unsigned int    reserved_6            : 1; /* [27]  */
        unsigned int    isp_gamma_step7       : 3; /* [30..28]  */
        unsigned int    reserved_7            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_step;

/* Define the union u_isp_gamma_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_stt2lut_en  : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_stt2lut_cfg;

/* Define the union u_isp_gamma_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_stt2lut_regnew;

/* Define the union u_isp_gamma_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_gamma_stt2lut_clr : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_stt2lut_abn;

/* Define the union u_isp_gamma_lut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_waddr    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_lut_waddr;
/* Define the union u_isp_gamma_lut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_wdata    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_lut_wdata;
/* Define the union u_isp_gamma_lut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_raddr    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_lut_raddr;
/* Define the union u_isp_gamma_lut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_rdata    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_lut_rdata;
/* Define the union u_isp_bcom_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    reserved_1            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bcom_cfg;

/* Define the union u_isp_bcom_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_version       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bcom_version;
/* Define the union u_isp_bcom_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_alpha        : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bcom_alpha;

/* Define the union u_isp_bdec_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    reserved_1            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_cfg;

/* Define the union u_isp_bdec_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bdec_version       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_version;
/* Define the union u_isp_bdec_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_alpha        : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_alpha;

/* Define the union u_isp_crb_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_mode          : 1; /* [0]  */
        unsigned int    isp_crb_rccmode       : 1; /* [1]  */
        unsigned int    isp_crb_stepfirstenable : 1; /* [2]  */
        unsigned int    isp_crb_stepsecondenable : 1; /* [3]  */
        unsigned int    isp_crb_stepthirdenable : 1; /* [4]  */
        unsigned int    isp_crb_wdr_fusion_en : 1; /* [5]  */
        unsigned int    isp_crb_wdr_enable    : 2; /* [7..6]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_cfg;

/* Define the union u_isp_crb_dc_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_dc_numv       : 5; /* [4..0]  */
        unsigned int    isp_crb_dc_numh       : 5; /* [9..5]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_dc_size;

/* Define the union u_isp_crb_kx */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_kx            : 22; /* [21..0]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_kx;

/* Define the union u_isp_crb_ky */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_ky            : 22; /* [21..0]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_ky;

/* Define the union u_isp_crb_smlmapoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_smlmapoffset  : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_smlmapoffset;

/* Define the union u_isp_crb_ccrcc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_ccrcc1        : 11; /* [10..0]  */
        unsigned int    isp_crb_ccrcc0        : 11; /* [21..11]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_ccrcc;

/* Define the union u_isp_crb_awbgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_awbgain1      : 12; /* [11..0]  */
        unsigned int    isp_crb_awbgain0      : 12; /* [23..12]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_awbgain;

/* Define the union u_isp_crb_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_gr1           : 12; /* [11..0]  */
        unsigned int    isp_crb_gr0           : 12; /* [23..12]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_gr;

/* Define the union u_isp_crb_cchigh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_cchigh        : 11; /* [10..0]  */
        unsigned int    reserved_0            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_cchigh;

/* Define the union u_isp_crb_mix0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_mix0          : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_mix0;

/* Define the union u_isp_crb_maxlevel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_maxlevel      : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_maxlevel;

/* Define the union u_isp_crb_highlevel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_highlevel     : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_highlevel;

/* Define the union u_isp_crb_cc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_cc1           : 11; /* [10..0]  */
        unsigned int    isp_crb_cc0           : 11; /* [21..11]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_cc;

/* Define the union u_isp_crb_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_stt2lut_en    : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_stt2lut_cfg;

/* Define the union u_isp_crb_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_stt2lut_regnew;

/* Define the union u_isp_crb_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_stt2lut_info  : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_crb_stt2lut_clr   : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_stt2lut_abn;

/* Define the union u_isp_crb_inblc_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_inblc_r       : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_inblc_r;

/* Define the union u_isp_crb_inblc_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_inblc_gr      : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_inblc_gr;

/* Define the union u_isp_crb_inblc_gb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_inblc_gb      : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_inblc_gb;

/* Define the union u_isp_crb_inblc_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_inblc_b       : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_inblc_b;

/* Define the union u_isp_crb_outblc_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_outblc_r      : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_outblc_r;

/* Define the union u_isp_crb_outblc_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_outblc_gr     : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_outblc_gr;

/* Define the union u_isp_crb_outblc_gb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_outblc_gb     : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_outblc_gb;

/* Define the union u_isp_crb_outblc_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_outblc_b      : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_outblc_b;

/* Define the union u_isp_crb_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_lut_waddr     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_lut_waddr;

/* Define the union u_isp_crb_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_lut_wdata     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_lut_wdata;

/* Define the union u_isp_crb_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_lut_raddr     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_lut_raddr;

/* Define the union u_isp_crb_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crb_lut_rdata     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_crb_lut_rdata;

/* Define the union u_isp_ca_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_satvssat_en    : 1; /* [0]  */
        unsigned int    reserved_0            : 3; /* [3..1]  */
        unsigned int    isp_ca_skinproc_en    : 1; /* [4]  */
        unsigned int    reserved_1            : 3; /* [7..5]  */
        unsigned int    isp_ca_llhcproc_en    : 1; /* [8]  */
        unsigned int    reserved_2            : 3; /* [11..9]  */
        unsigned int    isp_ca_cp_en          : 1; /* [12]  */
        unsigned int    reserved_3            : 3; /* [15..13]  */
        unsigned int    isp_ca_satadj_en      : 1; /* [16]  */
        unsigned int    reserved_4            : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ctrl;

/* Define the union u_isp_ca_des */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_des_mix        : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_ca_des_point      : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_des;

/* Define the union u_isp_ca_isoratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_isoratio       : 11; /* [10..0]  */
        unsigned int    reserved_0            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_isoratio;

/* Define the union u_isp_ca_sat_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_sat_c1         : 7; /* [6..0]  */
        unsigned int    reserved_0            : 9; /* [15..7]  */
        unsigned int    isp_ca_sat_c2         : 7; /* [22..16]  */
        unsigned int    reserved_1            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_sat_coef;

/* Define the union u_isp_ca_yratiolut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yratiolut_waddr : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yratiolut_waddr;

/* Define the union u_isp_ca_yratiolut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yratiolut_wdata : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yratiolut_wdata;

/* Define the union u_isp_ca_yratiolut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yratiolut_raddr : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yratiolut_raddr;

/* Define the union u_isp_ca_yratiolut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yratiolut_rdata : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yratiolut_rdata;

/* Define the union u_isp_ca_ysatlut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ysatlut_waddr  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ysatlut_waddr;

/* Define the union u_isp_ca_ysatlut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ysatlut_wdata  : 11; /* [10..0]  */
        unsigned int    reserved_0            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ysatlut_wdata;

/* Define the union u_isp_ca_ysatlut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ysatlut_raddr  : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ysatlut_raddr;

/* Define the union u_isp_ca_ysatlut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ysatlut_rdata  : 11; /* [10..0]  */
        unsigned int    reserved_0            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ysatlut_rdata;

/* Define the union u_isp_ca_ych1lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych1lut_waddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych1lut_waddr;

/* Define the union u_isp_ca_ych1lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych1lut_even_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_ych1lut_odd_wdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych1lut_wdata;

/* Define the union u_isp_ca_ych1lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych1lut_raddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych1lut_raddr;

/* Define the union u_isp_ca_ych1lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych1lut_even_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_ych1lut_odd_rdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych1lut_rdata;

/* Define the union u_isp_ca_ych2lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych2lut_waddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych2lut_waddr;

/* Define the union u_isp_ca_ych2lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych2lut_even_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_ych2lut_odd_wdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych2lut_wdata;

/* Define the union u_isp_ca_ych2lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych2lut_raddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych2lut_raddr;

/* Define the union u_isp_ca_ych2lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_ych2lut_even_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_ych2lut_odd_rdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_ych2lut_rdata;

/* Define the union u_isp_ca_yslulut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslulut_waddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslulut_waddr;

/* Define the union u_isp_ca_yslulut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslulut_even_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_yslulut_odd_wdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslulut_wdata;

/* Define the union u_isp_ca_yslulut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslulut_raddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslulut_raddr;

/* Define the union u_isp_ca_yslulut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslulut_even_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_yslulut_odd_rdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslulut_rdata;

/* Define the union u_isp_ca_yslvlut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslvlut_waddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslvlut_waddr;

/* Define the union u_isp_ca_yslvlut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslvlut_even_wdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_yslvlut_odd_wdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslvlut_wdata;

/* Define the union u_isp_ca_yslvlut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslvlut_raddr  : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslvlut_raddr;

/* Define the union u_isp_ca_yslvlut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yslvlut_even_rdata : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_ca_yslvlut_odd_rdata : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_yslvlut_rdata;

/* Define the union u_isp_ca_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_stt2lut_en     : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_stt2lut_cfg;

/* Define the union u_isp_ca_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_stt2lut_regnew;

/* Define the union u_isp_ca_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_stt2lut_info   : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_ca_stt2lut_clr    : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ca_stt2lut_abn;

/* Define the union u_isp_clut_adj0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_gain0        : 12; /* [11..0]  */
        unsigned int    reserved_0            : 4; /* [15..12]  */
        unsigned int    isp_clut_gain1        : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_adj0;

/* Define the union u_isp_clut_adj1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_gain2        : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_adj1;

/* Define the union u_isp_clut_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_stt2lut_en   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_stt2lut_cfg;

/* Define the union u_isp_clut_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_stt2lut_regnew;

/* Define the union u_isp_clut_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_clut_stt2lut_clr  : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_stt2lut_abn;

/* Define the union u_isp_clut_lut0_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut0_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut0_waddr;

/* Define the union u_isp_clut_lut0_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut0_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut0_wdata;

/* Define the union u_isp_clut_lut0_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut0_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut0_raddr;

/* Define the union u_isp_clut_lut0_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut0_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut0_rdata;

/* Define the union u_isp_clut_lut1_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut1_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut1_waddr;

/* Define the union u_isp_clut_lut1_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut1_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut1_wdata;

/* Define the union u_isp_clut_lut1_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut1_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut1_raddr;

/* Define the union u_isp_clut_lut1_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut1_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut1_rdata;

/* Define the union u_isp_clut_lut2_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut2_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut2_waddr;

/* Define the union u_isp_clut_lut2_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut2_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut2_wdata;

/* Define the union u_isp_clut_lut2_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut2_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut2_raddr;

/* Define the union u_isp_clut_lut2_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut2_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut2_rdata;

/* Define the union u_isp_clut_lut3_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut3_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut3_waddr;

/* Define the union u_isp_clut_lut3_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut3_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut3_wdata;

/* Define the union u_isp_clut_lut3_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut3_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut3_raddr;

/* Define the union u_isp_clut_lut3_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut3_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut3_rdata;

/* Define the union u_isp_clut_lut4_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut4_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut4_waddr;

/* Define the union u_isp_clut_lut4_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut4_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut4_wdata;

/* Define the union u_isp_clut_lut4_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut4_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut4_raddr;

/* Define the union u_isp_clut_lut4_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut4_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut4_rdata;

/* Define the union u_isp_clut_lut5_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut5_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut5_waddr;

/* Define the union u_isp_clut_lut5_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut5_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut5_wdata;

/* Define the union u_isp_clut_lut5_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut5_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut5_raddr;

/* Define the union u_isp_clut_lut5_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut5_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut5_rdata;

/* Define the union u_isp_clut_lut6_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut6_waddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut6_waddr;

/* Define the union u_isp_clut_lut6_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut6_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut6_wdata;

/* Define the union u_isp_clut_lut6_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut6_raddr   : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut6_raddr;

/* Define the union u_isp_clut_lut6_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut6_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut6_rdata;

/* Define the union u_isp_clut_lut7_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut7_waddr   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut7_waddr;

/* Define the union u_isp_clut_lut7_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut7_wdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut7_wdata;

/* Define the union u_isp_clut_lut7_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut7_raddr   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut7_raddr;

/* Define the union u_isp_clut_lut7_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut7_rdata   : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_clut_lut7_rdata;

/* Define the union u_isp_ldci_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_ldci_calc_en      : 1; /* [1]  */
        unsigned int    isp_ldci_wrstat_en    : 1; /* [2]  */
        unsigned int    isp_ldci_rdstat_en    : 1; /* [3]  */
        unsigned int    reserved_1            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_cfg;

/* Define the union u_isp_ldci_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_version       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_version;
/* Define the union u_isp_ldci_calc_small_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_calc_map_offsetx : 20; /* [19..0]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_calc_small_offset;

/* Define the union u_isp_ldci_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_smlmapstride : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_ldci_smlmapheight : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_ldci_total_zone   : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_zone;

/* Define the union u_isp_ldci_lpfstt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfstt_size  : 16; /* [15..0]  */
        unsigned int    isp_ldci_lpfstt_bst   : 4; /* [19..16]  */
        unsigned int    isp_ldci_lpfstt_en    : 1; /* [20]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpfstt_bst;

/* Define the union u_isp_ldci_lpfstt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfstt_info  : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_ldci_lpfstt_clr   : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpfstt_abn;

/* Define the union u_isp_ldci_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_scalex       : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_ldci_scaley       : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_scale;

/* Define the union u_isp_ldci_luma_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_luma_sel     : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_luma_sel;

/* Define the union u_isp_ldci_blc_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blc_ctrl     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_blc_ctrl;

/* Define the union u_isp_ldci_lpf_lpfcoef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef0     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_ldci_lpfcoef1     : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_lpfcoef0;

/* Define the union u_isp_ldci_lpf_lpfcoef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef2     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_ldci_lpfcoef3     : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_lpfcoef1;

/* Define the union u_isp_ldci_lpf_lpfcoef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef4     : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_lpfcoef2;

/* Define the union u_isp_ldci_lpf_lpfsft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfsft       : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_lpfsft;

/* Define the union u_isp_ldci_he_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_he_waddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_he_waddr;
/* Define the union u_isp_ldci_he_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hepos_wdata  : 9; /* [8..0]  */
        unsigned int    isp_ldci_heneg_wdata  : 9; /* [17..9]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_he_wdata;

/* Define the union u_isp_ldci_he_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_he_raddr      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_he_raddr;
/* Define the union u_isp_ldci_he_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hepos_rdata  : 9; /* [8..0]  */
        unsigned int    isp_ldci_heneg_rdata  : 9; /* [17..9]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_he_rdata;

/* Define the union u_isp_ldci_de_usm_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_de_usm_waddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_de_usm_waddr;
/* Define the union u_isp_ldci_de_usm_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_usmpos_wdata : 9; /* [8..0]  */
        unsigned int    isp_ldci_usmneg_wdata : 9; /* [17..9]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_de_usm_wdata;

/* Define the union u_isp_ldci_de_usm_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_de_usm_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_de_usm_raddr;
/* Define the union u_isp_ldci_de_usm_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_usmpos_rdata : 9; /* [8..0]  */
        unsigned int    isp_ldci_usmneg_rdata : 9; /* [17..9]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_de_usm_rdata;

/* Define the union u_isp_ldci_cgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_cgain_waddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_cgain_waddr;
/* Define the union u_isp_ldci_cgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_cgain_wdata  : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_cgain_wdata;

/* Define the union u_isp_ldci_cgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_cgain_raddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_cgain_raddr;
/* Define the union u_isp_ldci_cgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_cgain_rdata  : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_cgain_rdata;

/* Define the union u_isp_ldci_stat_hpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hstart       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_ldci_hend         : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stat_hpos;

/* Define the union u_isp_ldci_stat_vpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_vstart       : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_ldci_vend         : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stat_vpos;

/* Define the union u_isp_ldci_stat_evratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stat_evratio : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stat_evratio;

/* Define the union u_isp_ldci_stat_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stat_smlmapwidth : 6; /* [5..0]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    isp_ldci_stat_smlmapheight : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_ldci_stat_total_zone : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stat_zone;

/* Define the union u_isp_ldci_blk_smlmapwidth0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blk_smlmapwidth0 : 6; /* [5..0]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_blk_smlmapwidth0;

/* Define the union u_isp_ldci_blk_smlmapwidth1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blk_smlmapwidth1 : 6; /* [5..0]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_blk_smlmapwidth1;

/* Define the union u_isp_ldci_blk_smlmapwidth2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blk_smlmapwidth2 : 6; /* [5..0]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_blk_smlmapwidth2;

/* Define the union u_isp_ldci_chrdamp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_chrposdamp   : 8; /* [7..0]  */
        unsigned int    isp_ldci_chrnegdamp   : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_chrdamp;

/* Define the union u_isp_ldci_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stt2lut_en   : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stt2lut_cfg;

/* Define the union u_isp_ldci_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stt2lut_regnew : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stt2lut_regnew;

/* Define the union u_isp_ldci_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stt2lut_info : 16; /* [15..0]  */
        unsigned int    reserved_0            : 15; /* [30..16]  */
        unsigned int    isp_ldci_stt2lut_clr  : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_stt2lut_abn;

/* Define the union u_isp_ldci_lpf_map_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_map_waddr;
/* Define the union u_isp_ldci_lpf_map_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_map_wdata;
/* Define the union u_isp_ldci_lpf_map_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lpfmap_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_map_raddr;
/* Define the union u_isp_ldci_lpf_map_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpflitmap_rdata : 10; /* [9..0]  */
        unsigned int    isp_ldci_lpfplyp2map_rdata : 10; /* [19..10]  */
        unsigned int    isp_ldci_lpfplyp3map_rdata : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_map_rdata;

/* Define the union u_isp_adrc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_adrc_wrstat_en    : 1; /* [1]  */
        unsigned int    isp_adrc_rdstat_en    : 1; /* [2]  */
        unsigned int    isp_adrc_vbiflt_en    : 1; /* [3]  */
        unsigned int    isp_adrc_color_corr_en : 1; /* [4]  */
        unsigned int    isp_adrc_statfltrawluma : 1; /* [5]  */
        unsigned int    isp_adrc_statfltlinlog : 1; /* [6]  */
        unsigned int    isp_adrc_mono_chroma_en : 1; /* [7]  */
        unsigned int    isp_adrc_detail_rd_en : 1; /* [8]  */
        unsigned int    reserved_1            : 5; /* [13..9]  */
        unsigned int    isp_adrc_tmlutidx_sel : 1; /* [14]  */
        unsigned int    isp_adrc_statformat   : 1; /* [15]  */
        unsigned int    isp_adrc_statflttype  : 2; /* [17..16]  */
        unsigned int    isp_adrc_inadapt_mode : 2; /* [19..18]  */
        unsigned int    isp_adrc_dmsmooth_en  : 8; /* [27..20]  */
        unsigned int    reserved_2            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_cfg;

/* Define the union u_isp_adrc_lut_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_tmlut_update : 1; /* [0]  */
        unsigned int    isp_adrc_cclut_update : 1; /* [1]  */
        unsigned int    isp_adrc_mixlut_update : 1; /* [2]  */
        unsigned int    isp_adrc_declut_update : 1; /* [3]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_lut_update;

/* Define the union u_isp_adrc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_version       : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_version;
/* Define the union u_isp_adrc_strength */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_strength     : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_strength;

/* Define the union u_isp_adrc_rgb_wgt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_r_wgt        : 5; /* [4..0]  */
        unsigned int    reserved_0            : 3; /* [7..5]  */
        unsigned int    isp_adrc_g_wgt        : 5; /* [12..8]  */
        unsigned int    reserved_1            : 3; /* [15..13]  */
        unsigned int    isp_adrc_b_wgt        : 6; /* [21..16]  */
        unsigned int    reserved_2            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_rgb_wgt;

/* Define the union u_isp_adrc_shp_cfg_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_shp_log_raw  : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_adrc_shp_exp_raw  : 4; /* [11..8]  */
        unsigned int    reserved_1            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_shp_cfg_raw;

/* Define the union u_isp_adrc_div_denom_log_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_div_denom_log_raw : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_div_denom_log_raw;

/* Define the union u_isp_adrc_denom_exp_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_denom_exp_raw : 21; /* [20..0]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_denom_exp_raw;

/* Define the union u_isp_adrc_shp_cfg_luma */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_shp_log_luma : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_adrc_shp_exp_luma : 4; /* [11..8]  */
        unsigned int    reserved_1            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_shp_cfg_luma;

/* Define the union u_isp_adrc_div_denom_log_luma */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_div_denom_log_luma : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_div_denom_log_luma;

/* Define the union u_isp_adrc_denom_exp_luma */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_denom_exp_luma : 21; /* [20..0]  */
        unsigned int    reserved_0            : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_denom_exp_luma;

/* Define the union u_isp_adrc_idxbase0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_seg_idxbase0 : 8; /* [7..0]  */
        unsigned int    isp_adrc_seg_idxbase1 : 8; /* [15..8]  */
        unsigned int    isp_adrc_seg_idxbase2 : 8; /* [23..16]  */
        unsigned int    isp_adrc_seg_idxbase3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_idxbase0;

/* Define the union u_isp_adrc_idxbase1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_seg_idxbase4 : 8; /* [7..0]  */
        unsigned int    isp_adrc_seg_idxbase5 : 8; /* [15..8]  */
        unsigned int    isp_adrc_seg_idxbase6 : 8; /* [23..16]  */
        unsigned int    isp_adrc_seg_idxbase7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_idxbase1;

/* Define the union u_isp_adrc_maxval0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_seg_maxval0  : 8; /* [7..0]  */
        unsigned int    isp_adrc_seg_maxval1  : 8; /* [15..8]  */
        unsigned int    isp_adrc_seg_maxval2  : 8; /* [23..16]  */
        unsigned int    isp_adrc_seg_maxval3  : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_maxval0;

/* Define the union u_isp_adrc_maxval1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_seg_maxval4  : 8; /* [7..0]  */
        unsigned int    isp_adrc_seg_maxval5  : 8; /* [15..8]  */
        unsigned int    isp_adrc_seg_maxval6  : 8; /* [23..16]  */
        unsigned int    isp_adrc_seg_maxval7  : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_maxval1;

/* Define the union u_isp_adrc_gain_clip */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_gain_clip_step : 4; /* [3..0]  */
        unsigned int    isp_adrc_gain_clip_knee : 4; /* [7..4]  */
        unsigned int    isp_adrc_gain_clip_mode : 1; /* [8]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_gain_clip;

/* Define the union u_isp_adrc_dark_gain_lmt_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_val1_y       : 8; /* [7..0]  */
        unsigned int    isp_adrc_sft1_y       : 4; /* [11..8]  */
        unsigned int    isp_adrc_val2_y       : 4; /* [15..12]  */
        unsigned int    isp_adrc_sft2_y       : 4; /* [19..16]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_dark_gain_lmt_y;

/* Define the union u_isp_adrc_dark_gain_lmt_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_val1_c       : 8; /* [7..0]  */
        unsigned int    isp_adrc_sft1_c       : 4; /* [11..8]  */
        unsigned int    isp_adrc_val2_c       : 4; /* [15..12]  */
        unsigned int    isp_adrc_sft2_c       : 4; /* [19..16]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_dark_gain_lmt_c;

/* Define the union u_isp_adrc_bright_gain_lmt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_val          : 8; /* [7..0]  */
        unsigned int    isp_adrc_sft          : 4; /* [11..8]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bright_gain_lmt;

/* Define the union u_isp_adrc_lut_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_cc_lut_inout : 1; /* [0]  */
        unsigned int    isp_adrc_cc_lin_pow   : 1; /* [1]  */
        unsigned int    reserved_0            : 6; /* [7..2]  */
        unsigned int    isp_adrc_tm_lut_ctrl  : 1; /* [8]  */
        unsigned int    isp_adrc_tmgainskip   : 1; /* [9]  */
        unsigned int    isp_adrc_wgt_box_tri_sel : 1; /* [10]  */
        unsigned int    reserved_1            : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_lut_ctrl;

/* Define the union u_isp_adrc_sup_dark0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_darkmin0     : 8; /* [7..0]  */
        unsigned int    isp_adrc_darkmax0     : 8; /* [15..8]  */
        unsigned int    isp_adrc_darkthr0     : 8; /* [23..16]  */
        unsigned int    isp_adrc_darkslo0     : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_sup_dark0;

/* Define the union u_isp_adrc_sup_bright0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_brightmin0   : 8; /* [7..0]  */
        unsigned int    isp_adrc_brightmax0   : 8; /* [15..8]  */
        unsigned int    isp_adrc_brightthr0   : 8; /* [23..16]  */
        unsigned int    isp_adrc_brightslo0   : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_sup_bright0;

/* Define the union u_isp_adrc_sup1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_boostfact1   : 8; /* [7..0]  */
        unsigned int    isp_adrc_supfact1     : 8; /* [15..8]  */
        unsigned int    isp_adrc_dsubfact1    : 4; /* [19..16]  */
        unsigned int    isp_adrc_dsubfact0    : 5; /* [24..20]  */
        unsigned int    reserved_0            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_sup1;

/* Define the union u_isp_adrc_bld_darkl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_darkminl     : 8; /* [7..0]  */
        unsigned int    isp_adrc_darkmaxl     : 8; /* [15..8]  */
        unsigned int    isp_adrc_darkthrl     : 8; /* [23..16]  */
        unsigned int    isp_adrc_darkslol     : 4; /* [27..24]  */
        unsigned int    isp_adrc_bld_sftl     : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bld_darkl;

/* Define the union u_isp_adrc_bld_brightl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_brightminl   : 8; /* [7..0]  */
        unsigned int    isp_adrc_brightmaxl   : 8; /* [15..8]  */
        unsigned int    isp_adrc_brightthrl   : 8; /* [23..16]  */
        unsigned int    isp_adrc_brightslol   : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bld_brightl;

/* Define the union u_isp_adrc_bld_darkd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_darkmind     : 8; /* [7..0]  */
        unsigned int    isp_adrc_darkmaxd     : 8; /* [15..8]  */
        unsigned int    isp_adrc_darkthrd     : 8; /* [23..16]  */
        unsigned int    isp_adrc_darkslod     : 4; /* [27..24]  */
        unsigned int    isp_adrc_bld_sftd     : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bld_darkd;

/* Define the union u_isp_adrc_bld_brightd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_brightmind   : 8; /* [7..0]  */
        unsigned int    isp_adrc_brightmaxd   : 8; /* [15..8]  */
        unsigned int    isp_adrc_brightthrd   : 8; /* [23..16]  */
        unsigned int    isp_adrc_brightslod   : 4; /* [27..24]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bld_brightd;

/* Define the union u_isp_adrc_debug_info */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_debug_info    : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_debug_info;
/* Define the union u_isp_adrc_gain_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_ltm_gain_coef : 8; /* [7..0]  */
        unsigned int    isp_adrc_gtm_gain_coef : 8; /* [15..8]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_gain_coef;

/* Define the union u_isp_adrc_pd_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_pdrg_crt     : 8; /* [7..0]  */
        unsigned int    isp_adrc_pdrg_wid     : 8; /* [15..8]  */
        unsigned int    isp_adrc_pdrg_slo     : 3; /* [18..16]  */
        unsigned int    isp_adrc_pdbg_thr     : 8; /* [26..19]  */
        unsigned int    isp_adrc_pdbg_slo     : 3; /* [29..27]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_pd_cfg;

/* Define the union u_isp_adrc_pdw_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_pdwlowslo    : 5; /* [4..0]  */
        unsigned int    isp_adrc_pdwlowthr    : 8; /* [12..5]  */
        unsigned int    isp_adrc_pdwhighslo   : 5; /* [17..13]  */
        unsigned int    isp_adrc_pdwhighthr   : 8; /* [25..18]  */
        unsigned int    reserved_0            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_pdw_cfg;

/* Define the union u_isp_adrc_wgt_tm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgt_r_tm     : 8; /* [7..0]  */
        unsigned int    isp_adrc_wgt_g_tm     : 8; /* [15..8]  */
        unsigned int    isp_adrc_wgt_b_tm     : 8; /* [23..16]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgt_tm;

/* Define the union u_isp_adrc_tmlut0_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_tmlut0_waddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_tmlut0_waddr;
/* Define the union u_isp_adrc_tmlut0_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_tmlut0_wdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_tmlut0_wdata;
/* Define the union u_isp_adrc_tmlut0_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_tmlut0_raddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_tmlut0_raddr;
/* Define the union u_isp_adrc_tmlut0_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_tmlut0_rdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_tmlut0_rdata;
/* Define the union u_isp_adrc_cclut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_cclut_waddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_cclut_waddr;
/* Define the union u_isp_adrc_cclut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_cclut_wdata   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_cclut_wdata;
/* Define the union u_isp_adrc_cclut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_cclut_raddr   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_cclut_raddr;
/* Define the union u_isp_adrc_cclut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_cclut_rdata   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_cclut_rdata;
/* Define the union u_isp_adrc_localmix_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_localmix_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_localmix_waddr;
/* Define the union u_isp_adrc_localmix_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_localmix_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_localmix_wdata;
/* Define the union u_isp_adrc_localmix_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_localmix_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_localmix_raddr;
/* Define the union u_isp_adrc_localmix_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_localmix_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_localmix_rdata;
/* Define the union u_isp_adrc_detaildec_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_detaildec_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_detaildec_waddr;
/* Define the union u_isp_adrc_detaildec_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_detaildec_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_detaildec_wdata;
/* Define the union u_isp_adrc_detaildec_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_detaildecx_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_detaildec_raddr;
/* Define the union u_isp_adrc_detaildec_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_detaildec_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_detaildec_rdata;
/* Define the union u_isp_adrc_logcvt_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_waddr  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_logcvt_waddr;
/* Define the union u_isp_adrc_logcvt_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_wdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_logcvt_wdata;
/* Define the union u_isp_adrc_logcvt_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_logcvt_raddr;
/* Define the union u_isp_adrc_logcvt_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_rdata  : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_logcvt_rdata;

/* Define the union u_isp_adrc_logcvt_luma_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_luma_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_logcvt_luma_waddr;
/* Define the union u_isp_adrc_logcvt_luma_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_luma_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_logcvt_luma_wdata;
/* Define the union u_isp_adrc_logcvt_luma_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_luma_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_logcvt_luma_raddr;
/* Define the union u_isp_adrc_logcvt_luma_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_adrc_logcvt_luma_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_logcvt_luma_rdata;
/* Define the union u_isp_adrc_prev_luma_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_0  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_0;

/* Define the union u_isp_adrc_prev_luma_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_1  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_1;

/* Define the union u_isp_adrc_prev_luma_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_2  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_2;

/* Define the union u_isp_adrc_prev_luma_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_3  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_3;

/* Define the union u_isp_adrc_prev_luma_4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_4  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_4;

/* Define the union u_isp_adrc_prev_luma_5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_5  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_5;

/* Define the union u_isp_adrc_prev_luma_6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_6  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_6;

/* Define the union u_isp_adrc_prev_luma_7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_prev_luma_7  : 24; /* [23..0]  */
        unsigned int    reserved_0            : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_prev_luma_7;

/* Define the union u_isp_adrc_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_hnum         : 7; /* [6..0]  */
        unsigned int    reserved_0            : 1; /* [7]  */
        unsigned int    isp_adrc_vnum         : 7; /* [14..8]  */
        unsigned int    reserved_1            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_zone;

/* Define the union u_isp_adrc_zone_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_zone_hsize   : 9; /* [8..0]  */
        unsigned int    isp_adrc_chk_x        : 6; /* [14..9]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_adrc_zone_vsize   : 9; /* [24..16]  */
        unsigned int    isp_adrc_chk_y        : 6; /* [30..25]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_zone_size;

/* Define the union u_isp_adrc_zone_div0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_div_x0       : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_adrc_div_y0       : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_zone_div0;

/* Define the union u_isp_adrc_zone_div1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_div_x1       : 15; /* [14..0]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_adrc_div_y1       : 15; /* [30..16]  */
        unsigned int    reserved_1            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_zone_div1;

/* Define the union u_isp_adrc_bin */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_bin          : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bin;

/* Define the union u_isp_adrc_zone_init */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_acc_x_init   : 16; /* [15..0]  */
        unsigned int    isp_adrc_cnt_x_init   : 9; /* [24..16]  */
        unsigned int    isp_adrc_idx_x_init   : 6; /* [30..25]  */
        unsigned int    isp_adrc_big_x_init   : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_zone_init;

/* Define the union u_isp_adrc_bin_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_bin_scale    : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_bin_scale;

/* Define the union u_isp_adrc_stat_width */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_stat_width   : 16; /* [15..0]  */
        unsigned int    isp_adrc_total_width  : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_width;

/* Define the union u_isp_adrc_vbi_strength */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_var_rng_fine : 2; /* [1..0]  */
        unsigned int    reserved_0            : 10; /* [11..2]  */
        unsigned int    isp_adrc_var_spa_fine : 2; /* [13..12]  */
        unsigned int    reserved_1            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_vbi_strength;

/* Define the union u_isp_adrc_flt_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_flt_spa_fine : 4; /* [3..0]  */
        unsigned int    reserved_0            : 8; /* [11..4]  */
        unsigned int    isp_adrc_flt_rng_fine : 4; /* [15..12]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_flt_cfg;

/* Define the union u_isp_adrc_grad_rev */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_grad_rev_thres : 8; /* [7..0]  */
        unsigned int    isp_adrc_grad_rev_max : 7; /* [14..8]  */
        unsigned int    reserved_0            : 1; /* [15]  */
        unsigned int    isp_adrc_grad_rev_slope : 4; /* [19..16]  */
        unsigned int    reserved_1            : 4; /* [23..20]  */
        unsigned int    isp_adrc_grad_rev_shift : 3; /* [26..24]  */
        unsigned int    reserved_2            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_grad_rev;

/* Define the union u_isp_adrc_vbi_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_vbi_state    : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_vbi_state;

/* Define the union u_isp_adrc_blk_wgt_init */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_blk_wgt_init : 15; /* [14..0]  */
        unsigned int    reserved_0            : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_blk_wgt_init;

/* Define the union u_isp_adrc_wgtm_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgtm_en      : 1; /* [0]  */
        unsigned int    isp_adrc_wgtm_sft     : 4; /* [4..1]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgtm_cfg;

/* Define the union u_isp_adrc_wgtm_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgtm_0       : 8; /* [7..0]  */
        unsigned int    isp_adrc_wgtm_1       : 8; /* [15..8]  */
        unsigned int    isp_adrc_wgtm_2       : 8; /* [23..16]  */
        unsigned int    isp_adrc_wgtm_3       : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgtm_0;

/* Define the union u_isp_adrc_wgtm_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgtm_4       : 8; /* [7..0]  */
        unsigned int    isp_adrc_wgtm_5       : 8; /* [15..8]  */
        unsigned int    isp_adrc_wgtm_6       : 8; /* [23..16]  */
        unsigned int    isp_adrc_wgtm_7       : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgtm_1;

/* Define the union u_isp_adrc_wgtm_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgtm_8       : 8; /* [7..0]  */
        unsigned int    isp_adrc_wgtm_9       : 8; /* [15..8]  */
        unsigned int    isp_adrc_wgtm_10      : 8; /* [23..16]  */
        unsigned int    isp_adrc_wgtm_11      : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgtm_2;

/* Define the union u_isp_adrc_wgtm_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgtm_12      : 8; /* [7..0]  */
        unsigned int    isp_adrc_wgtm_13      : 8; /* [15..8]  */
        unsigned int    isp_adrc_wgtm_14      : 8; /* [23..16]  */
        unsigned int    isp_adrc_wgtm_15      : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgtm_3;

/* Define the union u_isp_adrc_wgtm_pd_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_wgtm_pd_slo  : 4; /* [3..0]  */
        unsigned int    isp_adrc_wgtm_pd_sft  : 4; /* [7..4]  */
        unsigned int    isp_adrc_wgtm_pd_thr  : 6; /* [13..8]  */
        unsigned int    reserved_0            : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_wgtm_pd_cfg;

/* Define the union u_isp_adrc_stat_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statfltminout : 16; /* [15..0]  */
        unsigned int    isp_adrc_lumaavegmanual : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg;

/* Define the union u_isp_adrc_luma_pw_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_adrc_lumapwthr0 : 8; /* [7..0]  */
        unsigned int isp_adrc_lumapwthr1 : 8; /* [15..8]  */
        unsigned int isp_adrc_lumapwthr2 : 8; /* [23..16]  */
        unsigned int isp_adrc_lumapwthr3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_luma_pw_thr;

/* Define the union u_isp_adrc_luma_pw_slo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_adrc_lumapwslo0 : 4; /* [3..0]  */
        unsigned int isp_adrc_lumapwslo1 : 4; /* [7..4]  */
        unsigned int isp_adrc_lumapwslo2 : 4; /* [11..8]  */
        unsigned int isp_adrc_lumapwslo3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;         /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_luma_pw_slo;

/* Define the union u_isp_adrc_luma_pw_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_adrc_lumapwsft0 : 4; /* [3..0]  */
        unsigned int isp_adrc_lumapwsft1 : 4; /* [7..4]  */
        unsigned int isp_adrc_lumapwsft2 : 4; /* [11..8]  */
        unsigned int isp_adrc_lumapwsft3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;         /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_luma_pw_sft;

/* Define the union u_isp_adrc_maskc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_adrc_maskc_min : 8; /* [7..0]  */
        unsigned int isp_adrc_maskc_max : 8; /* [15..8]  */
        unsigned int isp_adrc_maskc_thr : 8; /* [23..16]  */
        unsigned int isp_adrc_maskc_slo : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_adrc_maskc_cfg;


/* Define the union u_isp_adrc_stat_cfg00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale00 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset00 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg00;

/* Define the union u_isp_adrc_stat_cfg01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale01 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset01 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg01;

/* Define the union u_isp_adrc_stat_cfg02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale02 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset02 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg02;

/* Define the union u_isp_adrc_stat_cfg03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale03 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset03 : 12; /* [27..16]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg03;

/* Define the union u_isp_adrc_stat_cfg10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale10 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset10 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg10;

/* Define the union u_isp_adrc_stat_cfg11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale11 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset11 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg11;

/* Define the union u_isp_adrc_stat_cfg12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale12 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset12 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg12;

/* Define the union u_isp_adrc_stat_cfg13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale13 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset13 : 12; /* [27..16]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg13;

/* Define the union u_isp_adrc_stat_cfg20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale20 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset20 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg20;

/* Define the union u_isp_adrc_stat_cfg21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale21 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset21 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg21;

/* Define the union u_isp_adrc_stat_cfg22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale22 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset22 : 12; /* [27..16]  */
        unsigned int    reserved_0 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg22;

/* Define the union u_isp_adrc_stat_cfg23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefscale23 : 16; /* [15..0]  */
        unsigned int    isp_adrc_statcoefoffset23 : 12; /* [27..16]  */
        unsigned int    reserved_0            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_cfg23;

/* Define the union u_isp_adrc_stat_sft0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefsft00 : 5; /* [4..0]  */
        unsigned int    isp_adrc_statcoefsft01 : 5; /* [9..5]  */
        unsigned int    isp_adrc_statcoefsft02 : 5; /* [14..10]  */
        unsigned int    isp_adrc_statcoefsft03 : 5; /* [19..15]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_sft0;

/* Define the union u_isp_adrc_stat_sft1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefsft10 : 5; /* [4..0]  */
        unsigned int    isp_adrc_statcoefsft11 : 5; /* [9..5]  */
        unsigned int    isp_adrc_statcoefsft12 : 5; /* [14..10]  */
        unsigned int    isp_adrc_statcoefsft13 : 5; /* [19..15]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_sft1;

/* Define the union u_isp_adrc_stat_sft2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_adrc_statcoefsft20 : 5; /* [4..0]  */
        unsigned int    isp_adrc_statcoefsft21 : 5; /* [9..5]  */
        unsigned int    isp_adrc_statcoefsft22 : 5; /* [14..10]  */
        unsigned int    isp_adrc_statcoefsft23 : 5; /* [19..15]  */
        unsigned int    reserved_0            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_adrc_stat_sft2;

/* Define the union u_isp_drclog_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drclog_version     : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_drclog_version;
/* Define the union u_isp_drclog_shp_cfg_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drclog_shp_log_raw : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_drclog_shp_cfg_raw;

/* Define the union u_isp_drclog_div_denom_log_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drclog_div_denom_log_raw : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_drclog_div_denom_log_raw;

/* Define the global struct */
typedef struct {
    volatile unsigned int                    reserved_0[32]; /* 0x0~0x007c, reserved 32 * 4 bytes */
    volatile u_isp_be_version                isp_be_version; /* 0x0080 */
    volatile u_isp_be_date                   isp_be_date; /* 0x0084 */
    volatile u_isp_be_fpga_date              isp_be_fpga_date; /* 0x0088 */
    volatile unsigned int                    reserved_1; /* 0x008c */
    volatile u_isp_be_module_pos             isp_be_module_pos; /* 0x0090 */
    volatile u_isp_be_fstart_delay           isp_be_fstart_delay; /* 0x0094 */
    volatile u_isp_be_input_mux              isp_be_input_mux; /* 0x0098 */
    volatile u_isp_be_sys_ctrl               isp_be_sys_ctrl; /* 0x009c */
    volatile u_isp_be_user_define0           isp_be_user_define0; /* 0x00a0 */
    volatile u_isp_be_user_define1           isp_be_user_define1; /* 0x00a4 */
    volatile unsigned int                    reserved_2[2]; /* 0x00a8~0x00ac */
    volatile u_isp_be_startup                isp_be_startup; /* 0x00b0 */
    volatile u_isp_be_format                 isp_be_format; /* 0x00b4 */
    volatile unsigned int                   reserved_3[2];         /* 0x00b8~0x00bc */
    volatile u_isp_glb_blc_cfg              isp_glb_blc_cfg;   /* 0x00c0 */
    volatile unsigned int                   reserved_4;            /* 0x00c4 */
    volatile u_isp_blc_cfg00                isp_blc_cfg00;       /* 0x00c8 */
    volatile u_isp_blc_cfg01                isp_blc_cfg01;       /* 0x00cc */
    volatile u_isp_blc_cfg10                isp_blc_cfg10;       /* 0x00d0 */
    volatile u_isp_blc_cfg11                isp_blc_cfg11;       /* 0x00d4 */
    volatile u_isp_blc_cfg20                isp_blc_cfg20;       /* 0x00d8 */
    volatile u_isp_blc_cfg21                isp_blc_cfg21;       /* 0x00dc */
    volatile unsigned int                    reserved_5[456]; /* 0x00e0~0x07fc */
    volatile u_isp_clip_y_cfg                isp_clip_y_cfg; /* 0x0800 */
    volatile u_isp_clip_c_cfg                isp_clip_c_cfg; /* 0x0804 */
    volatile unsigned int                    reserved_6[14]; /* 0x0808~0x083c */
    volatile u_isp_dmnr_dither               isp_dmnr_dither; /* 0x0840 */
    volatile unsigned int                    reserved_7[7]; /* 0x0844~0x085c */
    volatile u_isp_drc_dither                isp_drc_dither; /* 0x0860 */
    volatile unsigned int                    reserved_8[4]; /* 0x0864~0x0870 */
    volatile u_isp_sharpen_dither            isp_sharpen_dither; /* 0x0874 */
    volatile u_isp_blk_hblank                isp_blk_hblank; /* 0x0878 */
    volatile u_isp_blk_vblank                isp_blk_vblank; /* 0x087c */
    volatile u_isp_y_sum0                    isp_y_sum0; /* 0x0880 */
    volatile u_isp_y_sum1                    isp_y_sum1; /* 0x0884 */
    volatile unsigned int                    reserved_9[481]; /* 0x0888~0x1008 */
    volatile u_isp_blc_version               isp_blc_version; /* 0x100c */
    volatile u_isp_blc_offset1               isp_blc_offset1; /* 0x1010 */
    volatile u_isp_blc_offset2               isp_blc_offset2; /* 0x1014 */
    volatile u_isp_blc_roi_cfg0             isp_blc_roi_cfg0; /* 0x1018 */
    volatile u_isp_blc_roi_cfg1             isp_blc_roi_cfg1; /* 0x101c */
    volatile unsigned int                    reserved_10[121]; /* 0x1020~0x1200 */
    volatile u_isp_wb_blc_cfg                isp_wb_blc_cfg; /* 0x1204 */
    volatile unsigned int                    reserved_11; /* 0x1208 */
    volatile u_isp_wb_version                isp_wb_version; /* 0x120c */
    volatile u_isp_wb_gain1                  isp_wb_gain1; /* 0x1210 */
    volatile u_isp_wb_gain2                  isp_wb_gain2; /* 0x1214 */
    volatile u_isp_wb_blc_offset1            isp_wb_blc_offset1; /* 0x1218 */
    volatile u_isp_wb_blc_offset2            isp_wb_blc_offset2; /* 0x121c */
    volatile u_isp_wb_clip_value             isp_wb_clip_value; /* 0x1220 */
    volatile unsigned int                    reserved_12[56]; /* 0x1224~0x1300 */
    volatile u_isp_dg_blc_cfg                isp_dg_blc_cfg; /* 0x1304 */
    volatile unsigned int                    reserved_13; /* 0x1308 */
    volatile u_isp_dg_version                isp_dg_version; /* 0x130c */
    volatile u_isp_dg_gain1                  isp_dg_gain1; /* 0x1310 */
    volatile u_isp_dg_gain2                  isp_dg_gain2; /* 0x1314 */
    volatile u_isp_dg_blc_offset1            isp_dg_blc_offset1; /* 0x1318 */
    volatile u_isp_dg_blc_offset2            isp_dg_blc_offset2; /* 0x131c */
    volatile u_isp_dg_clip_value             isp_dg_clip_value; /* 0x1320 */
    volatile u_isp_dg_blc_offset_shift      isp_dg_blc_offset_shift; /* 0x1324 */
    volatile u_isp_dg_real_blc_offset1      isp_dg_real_blc_offset1; /* 0x1328 */
    volatile u_isp_dg_real_blc_offset2      isp_dg_real_blc_offset2; /* 0x132c */
    volatile u_isp_dg_roi_cfg0              isp_dg_roi_cfg0; /* 0x1330 */
    volatile u_isp_dg_roi_cfg1              isp_dg_roi_cfg1; /* 0x1334 */
    volatile unsigned int                    reserved_14[50]; /* 0x1338~0x13fc */
    volatile u_isp_cc_cfg                    isp_cc_cfg; /* 0x1400 */
    volatile unsigned int                    reserved_15[3]; /* 0x1404~0x140c */
    volatile u_isp_cc_coef0                  isp_cc_coef0; /* 0x1410 */
    volatile u_isp_cc_coef1                  isp_cc_coef1; /* 0x1414 */
    volatile u_isp_cc_coef2                  isp_cc_coef2; /* 0x1418 */
    volatile u_isp_cc_coef3                  isp_cc_coef3; /* 0x141c */
    volatile u_isp_cc_coef4                  isp_cc_coef4; /* 0x1420 */
    volatile unsigned int                    reserved_16[3]; /* 0x1424~0x142c */
    volatile u_isp_cc_rcv_ctrl0              isp_cc_rcv_ctrl0; /* 0x1430 */
    volatile u_isp_cc_rcv_ctrl1              isp_cc_rcv_ctrl1; /* 0x1434 */
    volatile u_isp_cc_lumafact               isp_cc_lumafact; /* 0x1438 */
    volatile u_isp_cc_colortone_rb_gain      isp_cc_colortone_rb_gain; /* 0x143c */
    volatile u_isp_cc_colortone_g_gain       isp_cc_colortone_g_gain; /* 0x1440 */
    volatile unsigned int                    reserved_17[8]; /* 0x1444~0x1460 */
    volatile u_isp_cc_in_dc0                 isp_cc_in_dc0; /* 0x1464 */
    volatile u_isp_cc_in_dc1                 isp_cc_in_dc1; /* 0x1468 */
    volatile u_isp_cc_in_dc2                 isp_cc_in_dc2; /* 0x146c */
    volatile u_isp_cc_out_dc0                isp_cc_out_dc0; /* 0x1470 */
    volatile u_isp_cc_out_dc1                isp_cc_out_dc1; /* 0x1474 */
    volatile u_isp_cc_out_dc2                isp_cc_out_dc2; /* 0x1478 */
    volatile unsigned int reserved_18[36];                                            /* 0x147c~0x1508 */
    volatile u_isp_csc_version isp_csc_version;                                       /* 0x150c */
    volatile u_isp_csc_coef0 isp_csc_coef0;                                           /* 0x1510 */
    volatile u_isp_csc_coef1 isp_csc_coef1;                                           /* 0x1514 */
    volatile u_isp_csc_coef2 isp_csc_coef2;                                           /* 0x1518 */
    volatile u_isp_csc_coef3 isp_csc_coef3;                                           /* 0x151c */
    volatile u_isp_csc_coef4 isp_csc_coef4;                                           /* 0x1520 */
    volatile unsigned int reserved_19[3];                                             /* 0x1524~0x152c */
    volatile u_isp_csc_in_dc0 isp_csc_in_dc0;                                         /* 0x1530 */
    volatile u_isp_csc_in_dc1 isp_csc_in_dc1;                                         /* 0x1534 */
    volatile unsigned int reserved_20[2];                                             /* 0x1538~0x153c */
    volatile u_isp_csc_out_dc0 isp_csc_out_dc0;                                       /* 0x1540 */
    volatile u_isp_csc_out_dc1 isp_csc_out_dc1;                                       /* 0x1544 */
    volatile unsigned int reserved_21[46];                                            /* 0x1548~0x15fc */
    volatile u_isp_cc_dgain_pos0 isp_cc_dgain_pos0;                                   /* 0x1600 */
    volatile u_isp_cc_dgain_pos1 isp_cc_dgain_pos1;                                   /* 0x1604 */
    volatile u_isp_cc_dgain_pos2 isp_cc_dgain_pos2;                                   /* 0x1608 */
    volatile u_isp_cc_dgain_pos3 isp_cc_dgain_pos3;                                   /* 0x160c */
    volatile u_isp_cc_dgain_inseg0 isp_cc_dgain_inseg0;                               /* 0x1610 */
    volatile u_isp_cc_dgain_inseg1 isp_cc_dgain_inseg1;                               /* 0x1614 */
    volatile u_isp_cc_dgain_step isp_cc_dgain_step;                                   /* 0x1618 */
    volatile u_isp_cc_gamma_pos0 isp_cc_gamma_pos0;                                   /* 0x161c */
    volatile u_isp_cc_gamma_pos1 isp_cc_gamma_pos1;                                   /* 0x1620 */
    volatile u_isp_cc_gamma_pos2 isp_cc_gamma_pos2;                                   /* 0x1624 */
    volatile u_isp_cc_gamma_pos3 isp_cc_gamma_pos3;                                   /* 0x1628 */
    volatile u_isp_cc_gamma_inseg0 isp_cc_gamma_inseg0;                               /* 0x162c */
    volatile u_isp_cc_gamma_inseg1 isp_cc_gamma_inseg1;                               /* 0x1630 */
    volatile u_isp_cc_gamma_step isp_cc_gamma_step;                                   /* 0x1634 */
    volatile u_isp_cc_luma_wdr_coef isp_cc_luma_wdr_coef;                             /* 0x1638 */
    volatile u_isp_cc_stt2lut_cfg isp_cc_stt2lut_cfg;                                 /* 0x163c */
    volatile u_isp_cc_stt2lut_regnew isp_cc_stt2lut_regnew;                           /* 0x1640 */
    volatile u_isp_cc_stt2lut_abn isp_cc_stt2lut_abn;                                 /* 0x1644 */
    volatile unsigned int reserved_22[14];                                            /* 0x1648~0x167c */
    volatile u_isp_cc_dgain_lut_waddr isp_cc_dgain_lut_waddr;                         /* 0x1680 */
    volatile u_isp_cc_dgain_lut_wdata isp_cc_dgain_lut_wdata;                         /* 0x1684 */
    volatile u_isp_cc_dgain_lut_raddr isp_cc_dgain_lut_raddr;                         /* 0x1688 */
    volatile u_isp_cc_dgain_lut_rdata isp_cc_dgain_lut_rdata;                         /* 0x168c */
    volatile u_isp_cc_gamma_lut_waddr isp_cc_gamma_lut_waddr;                         /* 0x1690 */
    volatile u_isp_cc_gamma_lut_wdata isp_cc_gamma_lut_wdata;                         /* 0x1694 */
    volatile u_isp_cc_gamma_lut_raddr isp_cc_gamma_lut_raddr;                         /* 0x1698 */
    volatile u_isp_cc_gamma_lut_rdata isp_cc_gamma_lut_rdata;                         /* 0x169c */
    volatile unsigned int reserved_023[91];                                            /* 0x16a0~0x1808 */
    volatile u_isp_hcds_version              isp_hcds_version; /* 0x180c */
    volatile u_isp_hcds_coefh0               isp_hcds_coefh0; /* 0x1810 */
    volatile u_isp_hcds_coefh1               isp_hcds_coefh1; /* 0x1814 */
    volatile u_isp_hcds_coefh2               isp_hcds_coefh2; /* 0x1818 */
    volatile u_isp_hcds_coefh3               isp_hcds_coefh3; /* 0x181c */
    volatile unsigned int                    reserved_23[249]; /* 0x1820~0x1c00 */
    volatile u_isp_4dg_blc_cfg               isp_4dg_blc_cfg; /* 0x1c04 */
    volatile unsigned int                    reserved_24; /* 0x1c08 */
    volatile u_isp_4dg_version               isp_4dg_version; /* 0x1c0c */
    volatile u_isp_4dg_0_gain1               isp_4dg_0_gain1; /* 0x1c10 */
    volatile u_isp_4dg_0_gain2               isp_4dg_0_gain2; /* 0x1c14 */
    volatile u_isp_4dg_0_blc_offset1         isp_4dg_0_blc_offset1; /* 0x1c18 */
    volatile u_isp_4dg_0_blc_offset2         isp_4dg_0_blc_offset2; /* 0x1c1c */
    volatile u_isp_4dg_1_gain1               isp_4dg_1_gain1; /* 0x1c20 */
    volatile u_isp_4dg_1_gain2               isp_4dg_1_gain2; /* 0x1c24 */
    volatile u_isp_4dg_1_blc_offset1         isp_4dg_1_blc_offset1; /* 0x1c28 */
    volatile u_isp_4dg_1_blc_offset2         isp_4dg_1_blc_offset2; /* 0x1c2c */
    volatile u_isp_4dg_2_gain1               isp_4dg_2_gain1; /* 0x1c30 */
    volatile u_isp_4dg_2_gain2               isp_4dg_2_gain2; /* 0x1c34 */
    volatile u_isp_4dg_2_blc_offset1         isp_4dg_2_blc_offset1; /* 0x1c38 */
    volatile u_isp_4dg_2_blc_offset2         isp_4dg_2_blc_offset2; /* 0x1c3c */
    volatile u_isp_4dg_3_gain1               isp_4dg_3_gain1; /* 0x1c40 */
    volatile u_isp_4dg_3_gain2               isp_4dg_3_gain2; /* 0x1c44 */
    volatile u_isp_4dg_3_blc_offset1         isp_4dg_3_blc_offset1; /* 0x1c48 */
    volatile u_isp_4dg_3_blc_offset2         isp_4dg_3_blc_offset2; /* 0x1c4c */
    volatile u_isp_4dg_0_clip_value          isp_4dg_0_clip_value; /* 0x1c50 */
    volatile u_isp_4dg_1_clip_value          isp_4dg_1_clip_value; /* 0x1c54 */
    volatile u_isp_4dg_2_clip_value          isp_4dg_2_clip_value; /* 0x1c58 */
    volatile u_isp_4dg_3_clip_value          isp_4dg_3_clip_value; /* 0x1c5c */
    volatile unsigned int                    reserved_25[235]; /* 0x1c60~0x2008 */
    volatile u_isp_ae_version                isp_ae_version; /* 0x200c */
    volatile u_isp_ae_zone                   isp_ae_zone; /* 0x2010 */
    volatile u_isp_ae_skip_crg               isp_ae_skip_crg; /* 0x2014 */
    volatile u_isp_ae_total_stat             isp_ae_total_stat; /* 0x2018 */
    volatile u_isp_ae_count_stat             isp_ae_count_stat; /* 0x201c */
    volatile u_isp_ae_total_r_aver           isp_ae_total_r_aver; /* 0x2020 */
    volatile u_isp_ae_total_gr_aver          isp_ae_total_gr_aver; /* 0x2024 */
    volatile u_isp_ae_total_gb_aver          isp_ae_total_gb_aver; /* 0x2028 */
    volatile u_isp_ae_total_b_aver           isp_ae_total_b_aver; /* 0x202c */
    volatile u_isp_ae_hist_high              isp_ae_hist_high; /* 0x2030 */
    volatile unsigned int                    reserved_26[3]; /* 0x2034~0x203c */
    volatile u_isp_ae_bitmove                isp_ae_bitmove; /* 0x2040 */
    volatile u_isp_ae_offset_r_gr            isp_ae_offset_r_gr; /* 0x2044 */
    volatile unsigned int                    reserved_27; /* 0x2048 */
    volatile u_isp_ae_offset_gb_b            isp_ae_offset_gb_b; /* 0x204c */
    volatile unsigned int                    reserved_28[5]; /* 0x2050~0x2060 */
    volatile u_isp_ae_lut_update             isp_ae_lut_update; /* 0x2064 */
    volatile u_isp_ae_stt_bst                isp_ae_stt_bst; /* 0x2068 */
    volatile unsigned int                    reserved_29[2]; /* 0x206c~0x2070 */
    volatile u_isp_ae_crop_pos               isp_ae_crop_pos; /* 0x2074 */
    volatile u_isp_ae_crop_outsize           isp_ae_crop_outsize; /* 0x2078 */
    volatile unsigned int                    reserved_30; /* 0x207c */
    volatile u_isp_ae_hist_waddr             isp_ae_hist_waddr; /* 0x2080 */
    volatile u_isp_ae_hist_wdata             isp_ae_hist_wdata; /* 0x2084 */
    volatile u_isp_ae_hist_raddr             isp_ae_hist_raddr; /* 0x2088 */
    volatile u_isp_ae_hist_rdata             isp_ae_hist_rdata; /* 0x208c */
    volatile u_isp_ae_aver_r_gr_waddr        isp_ae_aver_r_gr_waddr; /* 0x2090 */
    volatile u_isp_ae_aver_r_gr_wdata        isp_ae_aver_r_gr_wdata; /* 0x2094 */
    volatile u_isp_ae_aver_r_gr_raddr        isp_ae_aver_r_gr_raddr; /* 0x2098 */
    volatile u_isp_ae_aver_r_gr_rdata        isp_ae_aver_r_gr_rdata; /* 0x209c */
    volatile u_isp_ae_aver_gb_b_waddr        isp_ae_aver_gb_b_waddr; /* 0x20a0 */
    volatile u_isp_ae_aver_gb_b_wdata        isp_ae_aver_gb_b_wdata; /* 0x20a4 */
    volatile u_isp_ae_aver_gb_b_raddr        isp_ae_aver_gb_b_raddr; /* 0x20a8 */
    volatile u_isp_ae_aver_gb_b_rdata        isp_ae_aver_gb_b_rdata; /* 0x20ac */
    volatile u_isp_ae_weight_waddr           isp_ae_weight_waddr; /* 0x20b0 */
    volatile u_isp_ae_weight_wdata           isp_ae_weight_wdata; /* 0x20b4 */
    volatile u_isp_ae_weight_raddr           isp_ae_weight_raddr; /* 0x20b8 */
    volatile u_isp_ae_weight_rdata           isp_ae_weight_rdata; /* 0x20bc */
    volatile unsigned int                    reserved_31[19]; /* 0x20c0~0x2108 */
    volatile u_isp_awb_version               isp_awb_version; /* 0x210c */
    volatile u_isp_awb_zone                  isp_awb_zone; /* 0x2110 */
    volatile u_isp_awb_ctrl                  isp_awb_ctrl; /* 0x2114 */
    volatile u_isp_awb_thd_min               isp_awb_thd_min; /* 0x2118 */
    volatile u_isp_awb_thd_max               isp_awb_thd_max; /* 0x211c */
    volatile u_isp_awb_cr_mm                 isp_awb_cr_mm; /* 0x2120 */
    volatile u_isp_awb_cb_mm                 isp_awb_cb_mm; /* 0x2124 */
    volatile u_isp_awb_offset_comp           isp_awb_offset_comp; /* 0x2128 */
    volatile unsigned int                    reserved_32[5]; /* 0x212c~0x213c, reserved 5 * 4 bytes */
    volatile u_isp_awb_avg_r                 isp_awb_avg_r; /* 0x2140 */
    volatile u_isp_awb_avg_g                 isp_awb_avg_g; /* 0x2144 */
    volatile u_isp_awb_avg_b                 isp_awb_avg_b; /* 0x2148 */
    volatile u_isp_awb_cnt_all               isp_awb_cnt_all; /* 0x214c */
    volatile unsigned int                    reserved_33[4]; /* 0x2150~0x215c, reserved 4 * 4 bytes */
    volatile u_isp_awb_stt_bst               isp_awb_stt_bst; /* 0x2160 */
    volatile u_isp_awb_stt_abn               isp_awb_stt_abn; /* 0x2164 */
    volatile unsigned int                    reserved_34[3]; /* 0x2168~0x2170, reserved 3 * 4 bytes */
    volatile u_isp_awb_crop_pos              isp_awb_crop_pos; /* 0x2174 */
    volatile u_isp_awb_crop_outsize          isp_awb_crop_outsize; /* 0x2178 */
    volatile unsigned int                    reserved_35; /* 0x217c */
    volatile u_isp_awb_stat_waddr            isp_awb_stat_waddr; /* 0x2180 */
    volatile u_isp_awb_stat_wdata            isp_awb_stat_wdata; /* 0x2184 */
    volatile u_isp_awb_stat_raddr            isp_awb_stat_raddr; /* 0x2188 */
    volatile u_isp_awb_stat_rdata            isp_awb_stat_rdata; /* 0x218c */
    volatile unsigned int                    reserved_36[28]; /* 0x2190~0x21fc, reserved 28 * 4 bytes */
    volatile u_isp_af_cfg                    isp_af_cfg; /* 0x2200 */
    volatile unsigned int                    reserved_37[2]; /* 0x2204~0x2208, reserved 2 * 4 bytes */
    volatile u_isp_af_version                isp_af_version; /* 0x220c */
    volatile u_isp_af_zone                   isp_af_zone; /* 0x2210 */
    volatile u_isp_af_crop_start             isp_af_crop_start; /* 0x2214 */
    volatile u_isp_af_crop_size              isp_af_crop_size; /* 0x2218 */
    volatile u_isp_af_mean_thres             isp_af_mean_thres; /* 0x221c */
    volatile u_isp_af_iirg0                  isp_af_iirg0; /* 0x2220 */
    volatile u_isp_af_iirg1                  isp_af_iirg1; /* 0x2224 */
    volatile u_isp_af_iirg2                  isp_af_iirg2; /* 0x2228 */
    volatile u_isp_af_iirg3                  isp_af_iirg3; /* 0x222c */
    volatile u_isp_af_iirg4                  isp_af_iirg4; /* 0x2230 */
    volatile u_isp_af_iirg5                  isp_af_iirg5; /* 0x2234 */
    volatile u_isp_af_iirg6                  isp_af_iirg6; /* 0x2238 */
    volatile u_isp_af_iirpl                  isp_af_iirpl; /* 0x223c */
    volatile u_isp_af_shift                  isp_af_shift; /* 0x2240 */
    volatile unsigned int                    reserved_38[3]; /* 0x2244~0x224c, reserved 3 * 4 bytes */
    volatile u_isp_af_firh0                  isp_af_firh0; /* 0x2250 */
    volatile u_isp_af_firh1                  isp_af_firh1; /* 0x2254 */
    volatile u_isp_af_firh2                  isp_af_firh2; /* 0x2258 */
    volatile u_isp_af_firh3                  isp_af_firh3; /* 0x225c */
    volatile u_isp_af_firh4                  isp_af_firh4; /* 0x2260 */
    volatile u_isp_af_stt_bst                isp_af_stt_bst; /* 0x2264 */
    volatile u_isp_af_stt_abn                isp_af_stt_abn; /* 0x2268 */
    volatile unsigned int                    reserved_39[3]; /* 0x226c~0x2274, reserved 3 * 4 bytes */
    volatile u_isp_af_acc_shift              isp_af_acc_shift; /* 0x2278 */
    volatile u_isp_af_cnt_shift              isp_af_cnt_shift; /* 0x227c */
    volatile unsigned int                    reserved_40[2]; /* 0x2280~0x2284, reserved 2 * 4 bytes */
    volatile u_isp_af_stat_raddr             isp_af_stat_raddr; /* 0x2288 */
    volatile u_isp_af_stat_rdata             isp_af_stat_rdata; /* 0x228c */
    volatile unsigned int                    reserved_41[28]; /* 0x2290~0x22fc, reserved 28 * 4 bytes */
    volatile u_isp_af_iirthre                isp_af_iirthre; /* 0x2300 */
    volatile u_isp_af_iirgain                isp_af_iirgain; /* 0x2304 */
    volatile u_isp_af_iirslope               isp_af_iirslope; /* 0x2308 */
    volatile u_isp_af_iirdilate              isp_af_iirdilate; /* 0x230c */
    volatile u_isp_af_firthre                isp_af_firthre; /* 0x2310 */
    volatile u_isp_af_firgain                isp_af_firgain; /* 0x2314 */
    volatile u_isp_af_firslope               isp_af_firslope; /* 0x2318 */
    volatile unsigned int                    reserved_42; /* 0x231c */
    volatile u_isp_af_iirthre_coring         isp_af_iirthre_coring; /* 0x2320 */
    volatile u_isp_af_iirpeak_coring         isp_af_iirpeak_coring; /* 0x2324 */
    volatile u_isp_af_iirslope_coring        isp_af_iirslope_coring; /* 0x2328 */
    volatile unsigned int                    reserved_43; /* 0x232c */
    volatile u_isp_af_firthre_coring         isp_af_firthre_coring; /* 0x2330 */
    volatile u_isp_af_firpeak_coring         isp_af_firpeak_coring; /* 0x2334 */
    volatile u_isp_af_firslope_coring        isp_af_firslope_coring; /* 0x2338 */
    volatile unsigned int                    reserved_44; /* 0x233c */
    volatile u_isp_af_highlight              isp_af_highlight; /* 0x2340 */
    volatile u_isp_af_offset                 isp_af_offset; /* 0x2344 */
    volatile unsigned int                    reserved_45[113]; /* 0x2348~0x2508, reserved 113 * 4 bytes */
    volatile u_isp_la_version                isp_la_version; /* 0x250c */
    volatile u_isp_la_zone                   isp_la_zone; /* 0x2510 */
    volatile unsigned int                    reserved_46[11]; /* 0x2514~0x253c, reserved 11 * 4 bytes */
    volatile u_isp_la_bitmove                isp_la_bitmove; /* 0x2540 */
    volatile u_isp_la_offset_r               isp_la_offset_r; /* 0x2544 */
    volatile u_isp_la_offset_gr              isp_la_offset_gr; /* 0x2548 */
    volatile u_isp_la_offset_gb              isp_la_offset_gb; /* 0x254c */
    volatile u_isp_la_offset_b               isp_la_offset_b; /* 0x2550 */
    volatile unsigned int                    reserved_47[3]; /* 0x2554~0x255c, reserved 3 * 4 bytes */
    volatile u_isp_la_gamma_limit            isp_la_gamma_limit; /* 0x2560 */
    volatile u_isp_la_crop_pos               isp_la_crop_pos; /* 0x2564 */
    volatile u_isp_la_crop_outsize           isp_la_crop_outsize; /* 0x2568 */
    volatile unsigned int                    reserved_48; /* 0x256c */
    volatile u_isp_la_stt_bst                isp_la_stt_bst; /* 0x2570 */
    volatile u_isp_la_stt_abn                isp_la_stt_abn; /* 0x2574 */
    volatile unsigned int                    reserved_49[6]; /* 0x2578~0x258c, reserved 6 * 4 bytes */
    volatile u_isp_la_aver_waddr             isp_la_aver_waddr; /* 0x2590 */
    volatile u_isp_la_aver_wdata             isp_la_aver_wdata; /* 0x2594 */
    volatile u_isp_la_aver_raddr             isp_la_aver_raddr; /* 0x2598 */
    volatile u_isp_la_aver_rdata             isp_la_aver_rdata; /* 0x259c */
    volatile unsigned int                    reserved_50[28]; /* 0x25a0~0x260c, reserved 28 * 4 bytes */
    volatile u_isp_flick_f0_inblc0           isp_flick_f0_inblc0; /* 0x2610 */
    volatile u_isp_flick_f0_inblc1           isp_flick_f0_inblc1; /* 0x2614 */
    volatile u_isp_flick_f1_inblc0           isp_flick_f1_inblc0; /* 0x2618 */
    volatile u_isp_flick_f1_inblc1           isp_flick_f1_inblc1; /* 0x261c */
    volatile u_isp_flick_exporatio           isp_flick_exporatio; /* 0x2620 */
    volatile u_isp_flick_overth              isp_flick_overth; /* 0x2624 */
    volatile u_isp_flick_gavg_pre            isp_flick_gavg_pre; /* 0x2628 */
    volatile u_isp_flick_gr_diff_cur         isp_flick_gr_diff_cur; /* 0x262c */
    volatile u_isp_flick_gb_diff_cur         isp_flick_gb_diff_cur; /* 0x2630 */
    volatile u_isp_flick_gr_abs_cur          isp_flick_gr_abs_cur; /* 0x2634 */
    volatile u_isp_flick_gb_abs_cur          isp_flick_gb_abs_cur; /* 0x2638 */
    volatile u_isp_flick_stt_bst             isp_flick_stt_bst; /* 0x263c */
    volatile u_isp_flick_stt_abn             isp_flick_stt_abn; /* 0x2640 */
    volatile unsigned int                    reserved_51[15]; /* 0x2644~0x267c, reserved 15 * 4 bytes */
    volatile u_isp_flick_gmean_waddr         isp_flick_gmean_waddr; /* 0x2680 */
    volatile u_isp_flick_gmean_wdata         isp_flick_gmean_wdata; /* 0x2684 */
    volatile u_isp_flick_gmean_raddr         isp_flick_gmean_raddr; /* 0x2688 */
    volatile u_isp_flick_gmean_rdata         isp_flick_gmean_rdata; /* 0x268c */
    volatile unsigned int                    reserved_52[31]; /* 0x2690~0x2708 */
    volatile u_isp_awblsc_version            isp_awblsc_version; /* 0x270c */
    volatile u_isp_awblsc_zone               isp_awblsc_zone; /* 0x2710 */
    volatile u_isp_awblsc_ctrl               isp_awblsc_ctrl; /* 0x2714 */
    volatile u_isp_awblsc_thd_min            isp_awblsc_thd_min; /* 0x2718 */
    volatile u_isp_awblsc_thd_max            isp_awblsc_thd_max; /* 0x271c */
    volatile u_isp_awblsc_cr_mm              isp_awblsc_cr_mm; /* 0x2720 */
    volatile u_isp_awblsc_cb_mm              isp_awblsc_cb_mm; /* 0x2724 */
    volatile u_isp_awblsc_offset_comp        isp_awblsc_offset_comp; /* 0x2728 */
    volatile unsigned int                    reserved_53[5]; /* 0x272c~0x273c */
    volatile u_isp_awblsc_avg_r              isp_awblsc_avg_r; /* 0x2740 */
    volatile u_isp_awblsc_avg_g              isp_awblsc_avg_g; /* 0x2744 */
    volatile u_isp_awblsc_avg_b              isp_awblsc_avg_b; /* 0x2748 */
    volatile u_isp_awblsc_cnt_all            isp_awblsc_cnt_all; /* 0x274c */
    volatile unsigned int                    reserved_54[4]; /* 0x2750~0x275c */
    volatile u_isp_awblsc_stt_bst            isp_awblsc_stt_bst; /* 0x2760 */
    volatile u_isp_awblsc_stt_abn            isp_awblsc_stt_abn; /* 0x2764 */
    volatile unsigned int                    reserved_55[3]; /* 0x2768~0x2770 */
    volatile u_isp_awblsc_crop_pos           isp_awblsc_crop_pos; /* 0x2774 */
    volatile u_isp_awblsc_crop_outsize       isp_awblsc_crop_outsize; /* 0x2778 */
    volatile unsigned int                    reserved_56; /* 0x277c */
    volatile u_isp_awblsc_stat_waddr         isp_awblsc_stat_waddr; /* 0x2780 */
    volatile u_isp_awblsc_stat_wdata         isp_awblsc_stat_wdata; /* 0x2784 */
    volatile u_isp_awblsc_stat_raddr         isp_awblsc_stat_raddr; /* 0x2788 */
    volatile u_isp_awblsc_stat_rdata         isp_awblsc_stat_rdata; /* 0x278c */
    volatile unsigned int                    reserved_57[540]; /* 0x2790~0x2ffc */
    volatile u_isp_lsc_mesh                  isp_lsc_mesh; /* 0x3000 */
    volatile u_isp_lsc_blcen                 isp_lsc_blcen; /* 0x3004 */
    volatile u_isp_lsc_blc0                  isp_lsc_blc0; /* 0x3008 */
    volatile u_isp_lsc_blc1                  isp_lsc_blc1; /* 0x300c */
    volatile u_isp_lsc_right_shift           isp_lsc_right_shift; /* 0x3010 */
    volatile unsigned int                    reserved_58[571]; /* 0x3014~0x38fc, reserved 571 * 4 bytes */
    volatile u_isp_ge_cfg                    isp_ge_cfg; /* 0x3900 */
    volatile u_isp_ge_blc_en                 isp_ge_blc_en; /* 0x3904 */
    volatile u_isp_ge_version                isp_ge_version; /* 0x3908 */
    volatile u_isp_ge0_ct_th1                isp_ge0_ct_th1; /* 0x390c */
    volatile u_isp_ge0_ct_th2                isp_ge0_ct_th2; /* 0x3910 */
    volatile u_isp_ge0_ct_th3                isp_ge0_ct_th3; /* 0x3914 */
    volatile u_isp_ge0_ct_slope              isp_ge0_ct_slope; /* 0x3918 */
    volatile u_isp_ge1_ct_th1                isp_ge1_ct_th1; /* 0x391c */
    volatile u_isp_ge1_ct_th2                isp_ge1_ct_th2; /* 0x3920 */
    volatile u_isp_ge1_ct_th3                isp_ge1_ct_th3; /* 0x3924 */
    volatile u_isp_ge1_ct_slope              isp_ge1_ct_slope; /* 0x3928 */
    volatile u_isp_ge0_blc_offset            isp_ge0_blc_offset; /* 0x392c */
    volatile u_isp_ge1_blc_offset            isp_ge1_blc_offset; /* 0x3930 */
    volatile u_isp_ge_strength               isp_ge_strength; /* 0x3934 */
    volatile u_isp_ge_bit_depth_sel          isp_ge_bit_depth_sel; /* 0x3938 */
    volatile unsigned int                    reserved_59[49]; /* 0x393c~0x39fc */
    volatile u_isp_fpn_cfg                   isp_fpn_cfg; /* 0x3a00 */
    volatile u_isp_fpn_calib_start           isp_fpn_calib_start; /* 0x3a04 */
    volatile u_isp_fpn_corr_cfg              isp_fpn_corr_cfg; /* 0x3a08 */
    volatile u_isp_fpn_stat                  isp_fpn_stat; /* 0x3a0c */
    volatile u_isp_fpn_white_level           isp_fpn_white_level; /* 0x3a10 */
    volatile unsigned int                    reserved_60; /* 0x3a14 */
    volatile u_isp_fpn_divcoef               isp_fpn_divcoef; /* 0x3a18 */
    volatile u_isp_fpn_framelog2             isp_fpn_framelog2; /* 0x3a1c */
    volatile u_isp_fpn_sum0                  isp_fpn_sum0; /* 0x3a20 */
    volatile u_isp_fpn_sum1                  isp_fpn_sum1; /* 0x3a24 */
    volatile unsigned int                    reserved_61[2]; /* 0x3a28~0x3a2c, reserved 2 * 4 bytes */
    volatile u_isp_fpn_corr0                 isp_fpn_corr0; /* 0x3a30 */
    volatile unsigned int                    reserved_62[3]; /* 0x3a34~0x3a3c, reserved 3 * 4 bytes */
    volatile u_isp_fpn_shift                 isp_fpn_shift; /* 0x3a40 */
    volatile unsigned int                    reserved_63[3]; /* 0x3a44~0x3a4c, reserved 3 * 4 bytes */
    volatile u_isp_fpn_max_o                 isp_fpn_max_o; /* 0x3a50 */
    volatile u_isp_fpn_overflowthr           isp_fpn_overflowthr; /* 0x3a54 */
    volatile unsigned int                    reserved_64[2]; /* 0x3a58~0x3a5c, reserved 2 * 4 bytes */
    volatile u_isp_fpn_black_frame_out_sel   isp_fpn_black_frame_out_sel; /* 0x3a60 */
    volatile u_isp_fpn_overlap_cfg           isp_fpn_overlap_cfg; /* 0x3a64 */
    volatile u_isp_fpn_calib_offline_set     isp_fpn_calib_offline_set; /* 0x3a68 */
    volatile u_isp_fpn_calib_stat_clear      isp_fpn_calib_stat_clear; /* 0x3a6c */
    volatile u_isp_fpn_roi_cfg0             isp_fpn_roi_cfg0; /* 0x3a70 */
    volatile u_isp_fpn_roi_cfg1             isp_fpn_roi_cfg1; /* 0x3a74 */
    volatile unsigned int                    reserved_65[98]; /* 0x3a78~0x3bfc, reserved 98 * 4 bytes */
    volatile u_isp_dpc_cfg                   isp_dpc_cfg; /* 0x3c00 */
    volatile u_isp_dpc_alpha                 isp_dpc_alpha; /* 0x3c04 */
    volatile u_isp_dpc_version               isp_dpc_version; /* 0x3c08 */
    volatile u_isp_dpc_mode                  isp_dpc_mode; /* 0x3c0c */
    volatile u_isp_dpc_set_use               isp_dpc_set_use; /* 0x3c10 */
    volatile u_isp_dpc_methods_set_1         isp_dpc_methods_set_1; /* 0x3c14 */
    volatile u_isp_dpc_line_thresh_1         isp_dpc_line_thresh_1; /* 0x3c18 */
    volatile u_isp_dpc_line_mad_fac_1        isp_dpc_line_mad_fac_1; /* 0x3c1c */
    volatile u_isp_dpc_pg_fac_1              isp_dpc_pg_fac_1; /* 0x3c20 */
    volatile u_isp_dpc_rg_fac_1              isp_dpc_rg_fac_1; /* 0x3c24 */
    volatile u_isp_dpc_ro_limits             isp_dpc_ro_limits; /* 0x3c28 */
    volatile u_isp_dpc_bpt_ctrl              isp_dpc_bpt_ctrl; /* 0x3c2c */
    volatile u_isp_dpc_bpt_number            isp_dpc_bpt_number; /* 0x3c30 */
    volatile u_isp_dpc_bpt_calib_number      isp_dpc_bpt_calib_number; /* 0x3c34 */
    volatile u_isp_dpc_bpt_thrd              isp_dpc_bpt_thrd; /* 0x3c38 */
    volatile unsigned int                    reserved_66; /* 0x3c3c */
    volatile u_isp_dpc_bpt_waddr             isp_dpc_bpt_waddr; /* 0x3c40 */
    volatile u_isp_dpc_bpt_wdata             isp_dpc_bpt_wdata; /* 0x3c44 */
    volatile u_isp_dpc_bpt_raddr             isp_dpc_bpt_raddr; /* 0x3c48 */
    volatile u_isp_dpc_bpt_rdata             isp_dpc_bpt_rdata; /* 0x3c4c */
    volatile u_isp_dpc1_alpha                isp_dpc1_alpha; /* 0x3c50 */
    volatile u_isp_dpc1_mode                 isp_dpc1_mode; /* 0x3c54 */
    volatile u_isp_dpc1_set_use              isp_dpc1_set_use; /* 0x3c58 */
    volatile u_isp_dpc1_methods_set_1        isp_dpc1_methods_set_1; /* 0x3c5c */
    volatile u_isp_dpc1_line_thresh_1        isp_dpc1_line_thresh_1; /* 0x3c60 */
    volatile u_isp_dpc1_line_mad_fac_1       isp_dpc1_line_mad_fac_1; /* 0x3c64 */
    volatile u_isp_dpc1_pg_fac_1             isp_dpc1_pg_fac_1; /* 0x3c68 */
    volatile u_isp_dpc1_rg_fac_1             isp_dpc1_rg_fac_1; /* 0x3c6c */
    volatile u_isp_dpc1_ro_limits            isp_dpc1_ro_limits; /* 0x3c70 */
    volatile unsigned int                    reserved_67[35]; /* 0x3c74~0x3cfc */
    volatile u_isp_dpc_soft_thr              isp_dpc_soft_thr; /* 0x3d00 */
    volatile u_isp_dpc_bhardthr_en           isp_dpc_bhardthr_en; /* 0x3d04 */
    volatile u_isp_dpc_rakeratio             isp_dpc_rakeratio; /* 0x3d08 */
    volatile u_isp_dpc_lch_thr_1             isp_dpc_lch_thr_1; /* 0x3d0c */
    volatile u_isp_dpc_line_kerdiff_fac      isp_dpc_line_kerdiff_fac; /* 0x3d10 */
    volatile u_isp_dpc_bit_depth_sel         isp_dpc_bit_depth_sel; /* 0x3d14 */
    volatile u_isp_dpc_stt2lut_cfg           isp_dpc_stt2lut_cfg; /* 0x3d18 */
    volatile u_isp_dpc_stt2lut_regnew        isp_dpc_stt2lut_regnew; /* 0x3d1c */
    volatile u_isp_dpc_stt2lut_abn           isp_dpc_stt2lut_abn; /* 0x3d20 */
    volatile u_isp_dpc_rg_fac_1_mtp          isp_dpc_rg_fac_1_mtp; /* 0x3d24 */
    volatile u_isp_dpc_amp_coef              isp_dpc_amp_coef; /* 0x3d28 */
    volatile unsigned int                    reserved_68; /* 0x3d2c */
    volatile u_isp_dpc1_soft_thr             isp_dpc1_soft_thr; /* 0x3d30 */
    volatile u_isp_dpc1_bhardthr_en          isp_dpc1_bhardthr_en; /* 0x3d34 */
    volatile u_isp_dpc1_rakeratio            isp_dpc1_rakeratio; /* 0x3d38 */
    volatile u_isp_dpc1_lch_thr_1            isp_dpc1_lch_thr_1; /* 0x3d3c */
    volatile u_isp_dpc1_line_kerdiff_fac     isp_dpc1_line_kerdiff_fac; /* 0x3d40 */
    volatile unsigned int                    reserved_69; /* 0x3d44 */
    volatile u_isp_dpc1_rg_fac_1_mtp         isp_dpc1_rg_fac_1_mtp; /* 0x3d48 */
    volatile u_isp_dpc1_amp_coef             isp_dpc1_amp_coef; /* 0x3d4c */
    volatile unsigned int                    reserved_70[172]; /* 0x3d50~0x3ffc */
    volatile u_isp_demosaic_cfg1             isp_demosaic_cfg1; /* 0x4000 */
    volatile u_isp_demosaic_coef0            isp_demosaic_coef0; /* 0x4004 */
    volatile u_isp_demosaic_coef2            isp_demosaic_coef2; /* 0x4008 */
    volatile u_isp_demosaic_coef3            isp_demosaic_coef3; /* 0x400c */
    volatile u_isp_demosaic_coef4            isp_demosaic_coef4; /* 0x4010 */
    volatile u_isp_demosaic_coef5            isp_demosaic_coef5; /* 0x4014 */
    volatile u_isp_demosaic_coef6            isp_demosaic_coef6; /* 0x4018 */
    volatile u_isp_demosaic_sel              isp_demosaic_sel; /* 0x401c */
    volatile u_isp_demosaic_lcac_cnt_thr     isp_demosaic_lcac_cnt_thr; /* 0x4020 */
    volatile u_isp_demosaic_lcac_luma_rb_thr   isp_demosaic_lcac_luma_rb_thr; /* 0x4024 */
    volatile u_isp_demosaic_lcac_luma_g_thr   isp_demosaic_lcac_luma_g_thr; /* 0x4028 */
    volatile u_isp_demosaic_purple_var_thr   isp_demosaic_purple_var_thr; /* 0x402c */
    volatile u_isp_demosaic_fake_cr_var_thr   isp_demosaic_fake_cr_var_thr; /* 0x4030 */
    volatile u_isp_demosaic_depurplectr      isp_demosaic_depurplectr; /* 0x4034 */
    volatile u_isp_demosaic_lpf_coef         isp_demosaic_lpf_coef; /* 0x4038 */
    volatile u_isp_demosaic_g_intp_ctrl      isp_demosaic_g_intp_ctrl; /* 0x403c */
    volatile u_isp_demosaic_cbcravgthld      isp_demosaic_cbcravgthld; /* 0x4040 */
    volatile u_isp_demosaic_cc_hf_ratio      isp_demosaic_cc_hf_ratio; /* 0x4044 */
    volatile unsigned int                    reserved_71[2]; /* 0x4048~0x404c */
    volatile u_isp_demosaic_cbcr_ratio_limit   isp_demosaic_cbcr_ratio_limit; /* 0x4050 */
    volatile u_isp_demosaic_fcr_gray_ratio   isp_demosaic_fcr_gray_ratio; /* 0x4054 */
    volatile u_isp_demosaic_fcr_sel          isp_demosaic_fcr_sel; /* 0x4058 */
    volatile u_isp_demosaic_cc_var_thresh    isp_demosaic_cc_var_thresh; /* 0x405c */
    volatile u_isp_demosaic_hf_intp_blur_th   isp_demosaic_hf_intp_blur_th; /* 0x4060 */
    volatile u_isp_demosaic_hf_intp_blur_ratio   isp_demosaic_hf_intp_blur_ratio; /* 0x4064 */
    volatile unsigned int                    reserved_72[3]; /* 0x4068~0x4070 */
    volatile u_isp_demosaic_defcolor_coef    isp_demosaic_defcolor_coef; /* 0x4074 */
    volatile unsigned int                    reserved;   /* 0x4078 */
    volatile u_isp_demosaic_fcr_hf_thr       isp_demosaic_fcr_hf_thr; /* 0x407c */
    volatile unsigned int                    reserved_73[32]; /* 0x4080~0x40fc */
    volatile u_isp_demosaic_desat_thr        isp_demosaic_desat_thr; /* 0x4100 */
    volatile u_isp_demosaic_desat_bldr       isp_demosaic_desat_bldr; /* 0x4104 */
    volatile u_isp_demosaic_desat_bldr_ratio   isp_demosaic_desat_bldr_ratio; /* 0x4108 */
    volatile u_isp_demosaic_desat_protect    isp_demosaic_desat_protect; /* 0x410c */
    volatile u_isp_demosaic_lut_update       isp_demosaic_lut_update; /* 0x4110 */
    volatile u_isp_gcac_diffthd              isp_gcac_diffthd; /* 0x4114 */
    volatile u_isp_gcac_lamda_reg            isp_gcac_lamda_reg; /* 0x4118 */
    volatile u_isp_gcac_lamdathd             isp_gcac_lamdathd; /* 0x411c */
    volatile u_isp_gcac_thrbthd              isp_gcac_thrbthd; /* 0x4120 */
    volatile u_isp_gcac_lamdamul             isp_gcac_lamdamul; /* 0x4124 */
    volatile u_isp_gcac_distance_thd         isp_gcac_distance_thd; /* 0x4128 */
    volatile u_isp_gcac_distance_mul         isp_gcac_distance_mul; /* 0x412c */
    volatile u_isp_gcac_edge_alpha           isp_gcac_edge_alpha; /* 0x4130 */
    volatile u_isp_dmnr_stt2lut_cfg          isp_dmnr_stt2lut_cfg; /* 0x4134 */
    volatile u_isp_dmnr_stt2lut_regnew       isp_dmnr_stt2lut_regnew; /* 0x4138 */
    volatile u_isp_dmnr_stt2lut_abn          isp_dmnr_stt2lut_abn; /* 0x413c */
    volatile u_isp_dmnr_aidm_cfg             isp_dmnr_aidm_cfg; /* 0x4140 */
    volatile unsigned int                    reserved_74[15]; /* 0x4144~0x417c */
    volatile u_isp_dmnr_aidm_detail_lut_waddr   isp_dmnr_aidm_detail_lut_waddr; /* 0x4180 */
    volatile u_isp_dmnr_aidm_detail_lut_wdata   isp_dmnr_aidm_detail_lut_wdata; /* 0x4184 */
    volatile u_isp_dmnr_aidm_detail_lut_raddr   isp_dmnr_aidm_detail_lut_raddr; /* 0x4188 */
    volatile u_isp_dmnr_aidm_detail_lut_rdata   isp_dmnr_aidm_detail_lut_rdata; /* 0x418c */
    volatile unsigned int                    reserved_75[1055]; /* 0x4190~0x5208 */
    volatile u_isp_sharpen_version           isp_sharpen_version; /* 0x520c */
    volatile u_isp_sharpen_ctrl              isp_sharpen_ctrl; /* 0x5210 */
    volatile u_isp_sharpen_mhfthd            isp_sharpen_mhfthd; /* 0x5214 */
    volatile u_isp_sharpen_weakdetail        isp_sharpen_weakdetail; /* 0x5218 */
    volatile u_isp_sharpen_dirvar            isp_sharpen_dirvar; /* 0x521c */
    volatile u_isp_sharpen_dirdiff           isp_sharpen_dirdiff; /* 0x5220 */
    volatile u_isp_sharpen_lumawgt0          isp_sharpen_lumawgt0; /* 0x5224 */
    volatile u_isp_sharpen_lumawgt1          isp_sharpen_lumawgt1; /* 0x5228 */
    volatile u_isp_sharpen_lumawgt2          isp_sharpen_lumawgt2; /* 0x522c */
    volatile u_isp_sharpen_lumawgt3          isp_sharpen_lumawgt3; /* 0x5230 */
    volatile u_isp_sharpen_lumawgt4          isp_sharpen_lumawgt4; /* 0x5234 */
    volatile u_isp_sharpen_lumawgt5          isp_sharpen_lumawgt5; /* 0x5238 */
    volatile u_isp_sharpen_lumawgt6          isp_sharpen_lumawgt6; /* 0x523c */
    volatile unsigned int                    reserved_76; /* 0x5240 */
    volatile u_isp_sharpen_shoot_amt         isp_sharpen_shoot_amt; /* 0x5244 */
    volatile u_isp_sharpen_shoot_maxchg      isp_sharpen_shoot_maxchg; /* 0x5248 */
    volatile u_isp_sharpen_shtvar            isp_sharpen_shtvar; /* 0x524c */
    volatile unsigned int                    reserved_77; /* 0x5250 */
    volatile u_isp_sharpen_oshtvar           isp_sharpen_oshtvar; /* 0x5254 */
    volatile unsigned int                    reserved_78; /* 0x5258 */
    volatile u_isp_sharpen_shtvar_mul        isp_sharpen_shtvar_mul; /* 0x525c */
    volatile u_isp_sharpen_oshtvardiff       isp_sharpen_oshtvardiff; /* 0x5260 */
    volatile unsigned int                    reserved_79[2]; /* 0x5264~0x5268, reserved 2 * 4 bytes */
    volatile u_isp_sharpen_lmtmf0            isp_sharpen_lmtmf0; /* 0x526c */
    volatile u_isp_sharpen_lmtmf1            isp_sharpen_lmtmf1; /* 0x5270 */
    volatile u_isp_sharpen_lmthf0            isp_sharpen_lmthf0; /* 0x5274 */
    volatile u_isp_sharpen_lmthf1            isp_sharpen_lmthf1; /* 0x5278 */
    volatile unsigned int                    reserved_80; /* 0x527c */
    volatile u_isp_sharpen_mhfgaind_waddr    isp_sharpen_mhfgaind_waddr; /* 0x5280 */
    volatile u_isp_sharpen_mhfgaind_wdata    isp_sharpen_mhfgaind_wdata; /* 0x5284 */
    volatile u_isp_sharpen_mhfgaind_raddr    isp_sharpen_mhfgaind_raddr; /* 0x5288 */
    volatile u_isp_sharpen_mhfgaind_rdata    isp_sharpen_mhfgaind_rdata; /* 0x528c */
    volatile u_isp_sharpen_mhfgainud_waddr   isp_sharpen_mhfgainud_waddr; /* 0x5290 */
    volatile u_isp_sharpen_mhfgainud_wdata   isp_sharpen_mhfgainud_wdata; /* 0x5294 */
    volatile u_isp_sharpen_mhfgainud_raddr   isp_sharpen_mhfgainud_raddr; /* 0x5298 */
    volatile u_isp_sharpen_mhfgainud_rdata   isp_sharpen_mhfgainud_rdata; /* 0x529c */
    volatile unsigned int                    reserved_81[25]; /* 0x52a0~0x5300, reserved 25 * 4 bytes */
    volatile u_isp_sharpen_skin_u            isp_sharpen_skin_u; /* 0x5304 */
    volatile u_isp_sharpen_skin_v            isp_sharpen_skin_v; /* 0x5308 */
    volatile u_isp_sharpen_skin_cnt          isp_sharpen_skin_cnt; /* 0x530c */
    volatile u_isp_sharpen_skin_edge         isp_sharpen_skin_edge; /* 0x5310 */
    volatile u_isp_sharpen_skin_edgethd      isp_sharpen_skin_edgethd; /* 0x5314 */
    volatile unsigned int                    reserved_82[2]; /* 0x5318~0x531c, reserved 2 * 4 bytes */
    volatile u_isp_sharpen_chrr_var          isp_sharpen_chrr_var; /* 0x5320 */
    volatile unsigned int                    reserved_83; /* 0x5324 */
    volatile u_isp_sharpen_chrr_thd          isp_sharpen_chrr_thd; /* 0x5328 */
    volatile u_isp_sharpen_chrr_gain         isp_sharpen_chrr_gain; /* 0x532c */
    volatile u_isp_sharpen_chrg_mul          isp_sharpen_chrg_mul; /* 0x5330 */
    volatile u_isp_sharpen_chrg_sft          isp_sharpen_chrg_sft; /* 0x5334 */
    volatile unsigned int                    reserved_84; /* 0x5338 */
    volatile u_isp_sharpen_chrg_thd          isp_sharpen_chrg_thd; /* 0x533c */
    volatile u_isp_sharpen_chrg_gain         isp_sharpen_chrg_gain; /* 0x5340 */
    volatile u_isp_sharpen_chrb_var          isp_sharpen_chrb_var; /* 0x5344 */
    volatile unsigned int                    reserved_85; /* 0x5348 */
    volatile u_isp_sharpen_chrb_thd          isp_sharpen_chrb_thd; /* 0x534c */
    volatile u_isp_sharpen_chrb_gain         isp_sharpen_chrb_gain; /* 0x5350 */
    volatile u_isp_sharpen_gain_sft          isp_sharpen_gain_sft; /* 0x5354 */
    volatile u_isp_sharpen_shoot_maxgain     isp_sharpen_shoot_maxgain; /* 0x5358 */
    volatile u_isp_sharpen_detail_mul        isp_sharpen_detail_mul; /* 0x535c */
    volatile u_isp_sharpen_osht_detail       isp_sharpen_osht_detail; /* 0x5360 */
    volatile u_isp_sharpen_usht_detail       isp_sharpen_usht_detail; /* 0x5364 */
    volatile unsigned int                    reserved_86[2]; /* 0x5368~0x536c, reserved 2 * 4 bytes */
    volatile u_isp_sharpen_stt2lut_cfg       isp_sharpen_stt2lut_cfg; /* 0x5370 */
    volatile u_isp_sharpen_stt2lut_regnew    isp_sharpen_stt2lut_regnew; /* 0x5374 */
    volatile u_isp_sharpen_stt2lut_abn       isp_sharpen_stt2lut_abn; /* 0x5378 */
    volatile unsigned int                    reserved_87[26]; /* 0x537c~0x53e0, reserved 26 * 4 bytes */
    volatile u_isp_sharpen_dlpf_coef         isp_sharpen_dlpf_coef; /* 0x53e4 */
    volatile u_isp_sharpen_udlpf_coef        isp_sharpen_udlpf_coef; /* 0x53e8 */
    volatile u_isp_sharpen_udhsf_coef        isp_sharpen_udhsf_coef; /* 0x53ec */
    volatile u_isp_sharpen_dhsf_2dshift      isp_sharpen_dhsf_2dshift; /* 0x53f0 */
    volatile unsigned int                    reserved_88[3]; /* 0x53f4~0x53fc, reserved 3 * 4 bytes */
    volatile u_isp_sharpen_dhsf_2dcoef0_03   isp_sharpen_dhsf_2dcoef0_03; /* 0x5400 */
    volatile u_isp_sharpen_dhsf_2dcoef0_47   isp_sharpen_dhsf_2dcoef0_47; /* 0x5404 */
    volatile u_isp_sharpen_dhsf_2dcoef0_811   isp_sharpen_dhsf_2dcoef0_811; /* 0x5408 */
    volatile u_isp_sharpen_dhsf_2dcoef0_1215   isp_sharpen_dhsf_2dcoef0_1215; /* 0x540c */
    volatile unsigned int                    reserved_89[2]; /* 0x5410~0x5414, reserved 2 * 4 bytes */
    volatile u_isp_sharpen_dhsf_2dcoef1_03   isp_sharpen_dhsf_2dcoef1_03; /* 0x5418 */
    volatile u_isp_sharpen_dhsf_2dcoef1_47   isp_sharpen_dhsf_2dcoef1_47; /* 0x541c */
    volatile u_isp_sharpen_dhsf_2dcoef1_811   isp_sharpen_dhsf_2dcoef1_811; /* 0x5420 */
    volatile u_isp_sharpen_dhsf_2dcoef1_1215   isp_sharpen_dhsf_2dcoef1_1215; /* 0x5424 */
    volatile unsigned int                    reserved_90[2]; /* 0x5428~0x542c, reserved 2 * 4 bytes */
    volatile u_isp_sharpen_dhsf_2dcoef2_03   isp_sharpen_dhsf_2dcoef2_03; /* 0x5430 */
    volatile u_isp_sharpen_dhsf_2dcoef2_47   isp_sharpen_dhsf_2dcoef2_47; /* 0x5434 */
    volatile u_isp_sharpen_dhsf_2dcoef2_811   isp_sharpen_dhsf_2dcoef2_811; /* 0x5438 */
    volatile u_isp_sharpen_dhsf_2dcoef2_1215   isp_sharpen_dhsf_2dcoef2_1215; /* 0x543c */
    volatile unsigned int                    reserved_91[2]; /* 0x5440~0x5444, reserved 2 * 4 bytes */
    volatile u_isp_sharpen_dhsf_2dcoef3_03   isp_sharpen_dhsf_2dcoef3_03; /* 0x5448 */
    volatile u_isp_sharpen_dhsf_2dcoef3_47   isp_sharpen_dhsf_2dcoef3_47; /* 0x544c */
    volatile u_isp_sharpen_dhsf_2dcoef3_811   isp_sharpen_dhsf_2dcoef3_811; /* 0x5450 */
    volatile u_isp_sharpen_dhsf_2dcoef3_1215   isp_sharpen_dhsf_2dcoef3_1215; /* 0x5454 */
    volatile unsigned int                    reserved_92[42]; /* 0x5458~0x54fc, reserved 42 * 4 bytes */
    volatile u_isp_sharpen_ldci_dither_cfg   isp_sharpen_ldci_dither_cfg; /* 0x5500 */
    volatile unsigned int                    reserved_93[63]; /* 0x5504~0x55fc, reserved 63 * 4 bytes */
    volatile u_isp_nddm_cfg                  isp_nddm_cfg; /* 0x5600 */
    volatile u_isp_nddm_gf_th                isp_nddm_gf_th; /* 0x5604 */
    volatile u_isp_nddm_awb_gf_cfg           isp_nddm_awb_gf_cfg; /* 0x5608 */
    volatile u_isp_nddm_fcr_gain             isp_nddm_fcr_gain; /* 0x560c */
    volatile u_isp_nddm_dm_bldrate           isp_nddm_dm_bldrate; /* 0x5610 */
    volatile u_isp_nddm_ehc_gray             isp_nddm_ehc_gray; /* 0x5614 */
    volatile u_isp_nddm_dither_cfg           isp_nddm_dither_cfg; /* 0x5618 */
    volatile u_isp_nddm_gf_lut_update        isp_nddm_gf_lut_update; /* 0x561c */
    volatile unsigned int                    reserved_94[24]; /* 0x5620~0x567c, reserved 24 * 4 bytes */
    volatile u_isp_nddm_gf_lut_waddr         isp_nddm_gf_lut_waddr; /* 0x5680 */
    volatile u_isp_nddm_gf_lut_wdata         isp_nddm_gf_lut_wdata; /* 0x5684 */
    volatile u_isp_nddm_gf_lut_raddr         isp_nddm_gf_lut_raddr; /* 0x5688 */
    volatile u_isp_nddm_gf_lut_rdata         isp_nddm_gf_lut_rdata; /* 0x568c */
    volatile unsigned int                    reserved_95[28]; /* 0x5690~0x56fc */
    volatile u_isp_bnrshp_cfg                isp_bnrshp_cfg; /* 0x5700 */
    volatile u_isp_bnrshp_lpfcoef            isp_bnrshp_lpfcoef; /* 0x5704 */
    volatile u_isp_bnrshp_hsfcoef            isp_bnrshp_hsfcoef; /* 0x5708 */
    volatile u_isp_bnrshp_mfgain_lut1        isp_bnrshp_mfgain_lut1; /* 0x570c */
    volatile u_isp_bnrshp_mfgain_lut0        isp_bnrshp_mfgain_lut0; /* 0x5710 */
    volatile u_isp_bnrshp_hfgain_lut1        isp_bnrshp_hfgain_lut1; /* 0x5714 */
    volatile u_isp_bnrshp_hfgain_lut0        isp_bnrshp_hfgain_lut0; /* 0x5718 */
    volatile u_isp_bnrshp_darkgain_lut1      isp_bnrshp_darkgain_lut1; /* 0x571c */
    volatile u_isp_bnrshp_darkgain_lut0      isp_bnrshp_darkgain_lut0; /* 0x5720 */
    volatile u_isp_bnrshp_shtcrtl_var_thd    isp_bnrshp_shtcrtl_var_thd; /* 0x5724 */
    volatile u_isp_bnrshp_shtcrtl_var_wgt    isp_bnrshp_shtcrtl_var_wgt; /* 0x5728 */
    volatile u_isp_bnrshp_gain               isp_bnrshp_gain; /* 0x572c */
    volatile u_isp_bnrshp_dark_thd           isp_bnrshp_dark_thd; /* 0x5730 */
    volatile u_isp_bnrshp_shtamt             isp_bnrshp_shtamt; /* 0x5734 */
    volatile u_isp_bnrshp_maxchg             isp_bnrshp_maxchg; /* 0x5738 */
    volatile u_isp_bnrshp_hvcoef             isp_bnrshp_hvcoef; /* 0x573c */
    volatile unsigned int                    reserved_96[48]; /* 0x5740~0x57fc, reserved 48 * 4 bytes */
    volatile u_isp_bnr_cfg                   isp_bnr_cfg; /* 0x5800 */
    volatile unsigned int                    reserved_97[2]; /* 0x5804~0x5808, reserved 2 * 4 bytes */
    volatile u_isp_bnr_version               isp_bnr_version; /* 0x580c */
    volatile u_isp_bnr_cfg0                  isp_bnr_cfg0; /* 0x5810 */
    volatile u_isp_bnr_cfg1                  isp_bnr_cfg1; /* 0x5814 */
    volatile u_isp_bnr_cfg2                  isp_bnr_cfg2; /* 0x5818 */
    volatile u_isp_bnr_cfg3                  isp_bnr_cfg3; /* 0x581c */
    volatile u_isp_bnr_cfg4                  isp_bnr_cfg4; /* 0x5820 */
    volatile u_isp_bnr_cfg5                  isp_bnr_cfg5; /* 0x5824 */
    volatile u_isp_bnr_cfg6                  isp_bnr_cfg6; /* 0x5828 */
    volatile u_isp_bnr_cfg7                  isp_bnr_cfg7; /* 0x582c */
    volatile u_isp_bnr_cfg8                  isp_bnr_cfg8; /* 0x5830 */
    volatile u_isp_bnr_cfg9                  isp_bnr_cfg9; /* 0x5834 */
    volatile u_isp_bnr_cfg10                 isp_bnr_cfg10; /* 0x5838 */
    volatile u_isp_bnr_cfg11                 isp_bnr_cfg11; /* 0x583c */
    volatile u_isp_bnr_cfg12                 isp_bnr_cfg12; /* 0x5840 */
    volatile u_isp_bnr_cfg13                 isp_bnr_cfg13; /* 0x5844 */
    volatile u_isp_bnr_cfg14                 isp_bnr_cfg14; /* 0x5848 */
    volatile u_isp_bnr_cfg15                 isp_bnr_cfg15; /* 0x584c */
    volatile u_isp_bnr_cfg16                 isp_bnr_cfg16; /* 0x5850 */
    volatile u_isp_bnr_cfg17                 isp_bnr_cfg17; /* 0x5854 */
    volatile u_isp_bnr_cfg18                 isp_bnr_cfg18;  /* 0x5858 */
    volatile unsigned int                    reserved_150[2]; /* 0x585c~0x5860 */
    volatile u_isp_bnr_cfg_gflt_1            isp_bnr_cfg_gflt_1; /* 0x5864 */
    volatile u_isp_bnr_cfg_gflt_2            isp_bnr_cfg_gflt_2; /* 0x5868 */
    volatile unsigned int                    reserved_98[2]; /* 0x586c~0x5870 */
    volatile u_isp_bnr_stt2lut_cfg           isp_bnr_stt2lut_cfg; /* 0x5874 */
    volatile u_isp_bnr_stt2lut_regnew        isp_bnr_stt2lut_regnew; /* 0x5878 */
    volatile u_isp_bnr_stt2lut_abn           isp_bnr_stt2lut_abn; /* 0x587c */
    volatile u_isp_bnr_noisesd_even_waddr    isp_bnr_noisesd_even_waddr; /* 0x5880 */
    volatile u_isp_bnr_noisesd_even_wdata    isp_bnr_noisesd_even_wdata; /* 0x5884 */
    volatile u_isp_bnr_noisesd_even_raddr    isp_bnr_noisesd_even_raddr; /* 0x5888 */
    volatile u_isp_bnr_noisesd_even_rdata    isp_bnr_noisesd_even_rdata; /* 0x588c */
    volatile u_isp_bnr_noisesd_odd_waddr     isp_bnr_noisesd_odd_waddr; /* 0x5890 */
    volatile u_isp_bnr_noisesd_odd_wdata     isp_bnr_noisesd_odd_wdata; /* 0x5894 */
    volatile u_isp_bnr_noisesd_odd_raddr     isp_bnr_noisesd_odd_raddr; /* 0x5898 */
    volatile u_isp_bnr_noisesd_odd_rdata     isp_bnr_noisesd_odd_rdata; /* 0x589c */
    volatile u_isp_bnr_noisesds_even_waddr   isp_bnr_noisesds_even_waddr; /* 0x58a0 */
    volatile u_isp_bnr_noisesds_even_wdata   isp_bnr_noisesds_even_wdata; /* 0x58a4 */
    volatile u_isp_bnr_noisesds_even_raddr   isp_bnr_noisesds_even_raddr; /* 0x58a8 */
    volatile u_isp_bnr_noisesds_even_rdata   isp_bnr_noisesds_even_rdata; /* 0x58ac */
    volatile u_isp_bnr_noisesds_odd_waddr    isp_bnr_noisesds_odd_waddr; /* 0x58b0 */
    volatile u_isp_bnr_noisesds_odd_wdata    isp_bnr_noisesds_odd_wdata; /* 0x58b4 */
    volatile u_isp_bnr_noisesds_odd_raddr    isp_bnr_noisesds_odd_raddr; /* 0x58b8 */
    volatile u_isp_bnr_noisesds_odd_rdata    isp_bnr_noisesds_odd_rdata; /* 0x58bc */
    volatile u_isp_bnr_noiseinv_even_waddr   isp_bnr_noiseinv_even_waddr; /* 0x58c0 */
    volatile u_isp_bnr_noiseinv_even_wdata   isp_bnr_noiseinv_even_wdata; /* 0x58c4 */
    volatile u_isp_bnr_noiseinv_even_raddr   isp_bnr_noiseinv_even_raddr; /* 0x58c8 */
    volatile u_isp_bnr_noiseinv_even_rdata   isp_bnr_noiseinv_even_rdata; /* 0x58cc */
    volatile u_isp_bnr_noiseinv_odd_waddr    isp_bnr_noiseinv_odd_waddr; /* 0x58d0 */
    volatile u_isp_bnr_noiseinv_odd_wdata    isp_bnr_noiseinv_odd_wdata; /* 0x58d4 */
    volatile u_isp_bnr_noiseinv_odd_raddr    isp_bnr_noiseinv_odd_raddr; /* 0x58d8 */
    volatile u_isp_bnr_noiseinv_odd_rdata    isp_bnr_noiseinv_odd_rdata; /* 0x58dc */
    volatile unsigned int                    reserved_99[12];            /* 0x58e0~0x590c */
    volatile u_isp_bnr_cfg19                 isp_bnr_cfg19;              /* 0x5910 */
    volatile u_isp_bnr_cfg20                 isp_bnr_cfg20;              /* 0x5914 */
    volatile u_isp_bnr_cfg21                 isp_bnr_cfg21;              /* 0x5918 */
    volatile u_isp_bnr_cfg22                 isp_bnr_cfg22;              /* 0x591c */
    volatile u_isp_bnr_cfg23                 isp_bnr_cfg23;              /* 0x5920 */
    volatile u_isp_bnr_cfg24                 isp_bnr_cfg24;              /* 0x5924 */
    volatile u_isp_bnr_cfg25                 isp_bnr_cfg25;              /* 0x5928 */
    volatile u_isp_bnr_cfg26                 isp_bnr_cfg26;              /* 0x592c */
    volatile unsigned int                    reserved_151[20];           /* 0x5930~0x597c */
    volatile u_isp_bnr_noiseinv_mag_even_waddr   isp_bnr_noiseinv_mag_even_waddr; /* 0x5980 */
    volatile u_isp_bnr_noiseinv_mag_even_wdata   isp_bnr_noiseinv_mag_even_wdata; /* 0x5984 */
    volatile u_isp_bnr_noiseinv_mag_even_raddr   isp_bnr_noiseinv_mag_even_raddr; /* 0x5988 */
    volatile u_isp_bnr_noiseinv_mag_even_rdata   isp_bnr_noiseinv_mag_even_rdata; /* 0x598c */
    volatile u_isp_bnr_noiseinv_mag_odd_waddr   isp_bnr_noiseinv_mag_odd_waddr; /* 0x5990 */
    volatile u_isp_bnr_noiseinv_mag_odd_wdata   isp_bnr_noiseinv_mag_odd_wdata; /* 0x5994 */
    volatile u_isp_bnr_noiseinv_mag_odd_raddr   isp_bnr_noiseinv_mag_odd_raddr; /* 0x5998 */
    volatile u_isp_bnr_noiseinv_mag_odd_rdata   isp_bnr_noiseinv_mag_odd_rdata; /* 0x599c */
    volatile u_isp_bnr_fbratiotable_waddr    isp_bnr_fbratiotable_waddr; /* 0x59a0 */
    volatile u_isp_bnr_fbratiotable_wdata    isp_bnr_fbratiotable_wdata; /* 0x59a4 */
    volatile u_isp_bnr_fbratiotable_raddr    isp_bnr_fbratiotable_raddr; /* 0x59a8 */
    volatile u_isp_bnr_fbratiotable_rdata    isp_bnr_fbratiotable_rdata; /* 0x59ac */
    volatile u_isp_bnr_coring_low_even_waddr   isp_bnr_coring_low_even_waddr; /* 0x59b0 */
    volatile u_isp_bnr_coring_low_even_wdata   isp_bnr_coring_low_even_wdata; /* 0x59b4 */
    volatile u_isp_bnr_coring_low_even_raddr   isp_bnr_coring_low_even_raddr; /* 0x59b8 */
    volatile u_isp_bnr_coring_low_even_rdata   isp_bnr_coring_low_even_rdata; /* 0x59bc */
    volatile u_isp_bnr_coring_low_odd_waddr   isp_bnr_coring_low_odd_waddr; /* 0x59c0 */
    volatile u_isp_bnr_coring_low_odd_wdata   isp_bnr_coring_low_odd_wdata; /* 0x59c4 */
    volatile u_isp_bnr_coring_low_odd_raddr   isp_bnr_coring_low_odd_raddr; /* 0x59c8 */
    volatile u_isp_bnr_coring_low_odd_rdata   isp_bnr_coring_low_odd_rdata; /* 0x59cc */
    volatile unsigned int                    reserved_100[44]; /* 0x59d0~0x5a7c */
    volatile u_isp_bnr_noiseinv_1_even_waddr   isp_bnr_noiseinv_1_even_waddr; /* 0x5a80 */
    volatile u_isp_bnr_noiseinv_1_even_wdata   isp_bnr_noiseinv_1_even_wdata; /* 0x5a84 */
    volatile u_isp_bnr_noiseinv_1_even_raddr   isp_bnr_noiseinv_1_even_raddr; /* 0x5a88 */
    volatile u_isp_bnr_noiseinv_1_even_rdata   isp_bnr_noiseinv_1_even_rdata; /* 0x5a8c */
    volatile u_isp_bnr_noiseinv_1_odd_waddr   isp_bnr_noiseinv_1_odd_waddr; /* 0x5a90 */
    volatile u_isp_bnr_noiseinv_1_odd_wdata   isp_bnr_noiseinv_1_odd_wdata; /* 0x5a94 */
    volatile u_isp_bnr_noiseinv_1_odd_raddr   isp_bnr_noiseinv_1_odd_raddr; /* 0x5a98 */
    volatile u_isp_bnr_noiseinv_1_odd_rdata   isp_bnr_noiseinv_1_odd_rdata; /* 0x5a9c */
    volatile u_isp_bnr_mixgain_waddr         isp_bnr_mixgain_waddr; /* 0x5aa0 */
    volatile u_isp_bnr_mixgain_wdata         isp_bnr_mixgain_wdata; /* 0x5aa4 */
    volatile u_isp_bnr_mixgain_raddr         isp_bnr_mixgain_raddr; /* 0x5aa8 */
    volatile u_isp_bnr_mixgain_rdata         isp_bnr_mixgain_rdata; /* 0x5aac */
    volatile u_isp_bnr_ynet_fg_waddr isp_bnr_ynet_fg_waddr;   /* 0x5ab0 */
    volatile u_isp_bnr_ynet_fg_wdata isp_bnr_ynet_fg_wdata;   /* 0x5ab4 */
    volatile u_isp_bnr_ynet_fg_raddr isp_bnr_ynet_fg_raddr;   /* 0x5ab8 */
    volatile u_isp_bnr_ynet_fg_rdata isp_bnr_ynet_fg_rdata;   /* 0x5abc */
    volatile u_isp_bnr_ynet_bg_waddr isp_bnr_ynet_bg_waddr;   /* 0x5ac0 */
    volatile u_isp_bnr_ynet_bg_wdata isp_bnr_ynet_bg_wdata;   /* 0x5ac4 */
    volatile u_isp_bnr_ynet_bg_raddr isp_bnr_ynet_bg_raddr;   /* 0x5ac8 */
    volatile u_isp_bnr_ynet_bg_rdata isp_bnr_ynet_bg_rdata;   /* 0x5acc */
    volatile u_isp_bnr_ynet_mot_waddr isp_bnr_ynet_mot_waddr; /* 0x5ad0 */
    volatile u_isp_bnr_ynet_mot_wdata isp_bnr_ynet_mot_wdata; /* 0x5ad4 */
    volatile u_isp_bnr_ynet_mot_raddr isp_bnr_ynet_mot_raddr; /* 0x5ad8 */
    volatile u_isp_bnr_ynet_mot_rdata isp_bnr_ynet_mot_rdata; /* 0x5adc */
    volatile unsigned int                    reserved_101[72]; /* 0x5ae0~0x5bfc */
    volatile u_isp_mlsc_stt2lut_cfg          isp_mlsc_stt2lut_cfg; /* 0x5c00 */
    volatile u_isp_mlsc_stt2lut_regnew       isp_mlsc_stt2lut_regnew; /* 0x5c04 */
    volatile u_isp_mlsc_stt2lut_abn          isp_mlsc_stt2lut_abn; /* 0x5c08 */
    volatile unsigned int                    reserved_102; /* 0x5c0c */
    volatile u_isp_mlsc_winnum               isp_mlsc_winnum; /* 0x5c10 */
    volatile u_isp_mlsc_winx0                isp_mlsc_winx0; /* 0x5c14 */
    volatile u_isp_mlsc_winx1                isp_mlsc_winx1; /* 0x5c18 */
    volatile u_isp_mlsc_winx2                isp_mlsc_winx2; /* 0x5c1c */
    volatile u_isp_mlsc_winx3                isp_mlsc_winx3; /* 0x5c20 */
    volatile u_isp_mlsc_winx4                isp_mlsc_winx4; /* 0x5c24 */
    volatile u_isp_mlsc_winx5                isp_mlsc_winx5; /* 0x5c28 */
    volatile u_isp_mlsc_winx6                isp_mlsc_winx6; /* 0x5c2c */
    volatile u_isp_mlsc_winx7                isp_mlsc_winx7; /* 0x5c30 */
    volatile u_isp_mlsc_winx8                isp_mlsc_winx8; /* 0x5c34 */
    volatile u_isp_mlsc_winx9                isp_mlsc_winx9; /* 0x5c38 */
    volatile u_isp_mlsc_winx10               isp_mlsc_winx10; /* 0x5c3c */
    volatile u_isp_mlsc_winx11               isp_mlsc_winx11; /* 0x5c40 */
    volatile u_isp_mlsc_winx12               isp_mlsc_winx12; /* 0x5c44 */
    volatile u_isp_mlsc_winx13               isp_mlsc_winx13; /* 0x5c48 */
    volatile u_isp_mlsc_winx14               isp_mlsc_winx14; /* 0x5c4c */
    volatile u_isp_mlsc_winx15               isp_mlsc_winx15; /* 0x5c50 */
    volatile u_isp_mlsc_winy0                isp_mlsc_winy0; /* 0x5c54 */
    volatile u_isp_mlsc_winy1                isp_mlsc_winy1; /* 0x5c58 */
    volatile u_isp_mlsc_winy2                isp_mlsc_winy2; /* 0x5c5c */
    volatile u_isp_mlsc_winy3                isp_mlsc_winy3; /* 0x5c60 */
    volatile u_isp_mlsc_winy4                isp_mlsc_winy4; /* 0x5c64 */
    volatile u_isp_mlsc_winy5                isp_mlsc_winy5; /* 0x5c68 */
    volatile u_isp_mlsc_winy6                isp_mlsc_winy6; /* 0x5c6c */
    volatile u_isp_mlsc_winy7                isp_mlsc_winy7; /* 0x5c70 */
    volatile u_isp_mlsc_width_offset         isp_mlsc_width_offset; /* 0x5c74 */
    volatile u_isp_mlsc_mesh                 isp_mlsc_mesh; /* 0x5c78 */
    volatile unsigned int                    reserved_103; /* 0x5c7c */
    volatile u_isp_mlsc_rgain_waddr          isp_mlsc_rgain_waddr; /* 0x5c80 */
    volatile u_isp_mlsc_rgain_wdata          isp_mlsc_rgain_wdata; /* 0x5c84 */
    volatile u_isp_mlsc_rgain_raddr          isp_mlsc_rgain_raddr; /* 0x5c88 */
    volatile u_isp_mlsc_rgain_rdata          isp_mlsc_rgain_rdata; /* 0x5c8c */
    volatile u_isp_mlsc_grgain_waddr         isp_mlsc_grgain_waddr; /* 0x5c90 */
    volatile u_isp_mlsc_grgain_wdata         isp_mlsc_grgain_wdata; /* 0x5c94 */
    volatile u_isp_mlsc_grgain_raddr         isp_mlsc_grgain_raddr; /* 0x5c98 */
    volatile u_isp_mlsc_grgain_rdata         isp_mlsc_grgain_rdata; /* 0x5c9c */
    volatile u_isp_mlsc_bgain_waddr          isp_mlsc_bgain_waddr; /* 0x5ca0 */
    volatile u_isp_mlsc_bgain_wdata          isp_mlsc_bgain_wdata; /* 0x5ca4 */
    volatile u_isp_mlsc_bgain_raddr          isp_mlsc_bgain_raddr; /* 0x5ca8 */
    volatile u_isp_mlsc_bgain_rdata          isp_mlsc_bgain_rdata; /* 0x5cac */
    volatile u_isp_mlsc_gbgain_waddr         isp_mlsc_gbgain_waddr; /* 0x5cb0 */
    volatile u_isp_mlsc_gbgain_wdata         isp_mlsc_gbgain_wdata; /* 0x5cb4 */
    volatile u_isp_mlsc_gbgain_raddr         isp_mlsc_gbgain_raddr; /* 0x5cb8 */
    volatile u_isp_mlsc_gbgain_rdata         isp_mlsc_gbgain_rdata; /* 0x5cbc */
    volatile unsigned int                    reserved_104[16]; /* 0x5cc0~0x5cfc */
    volatile u_isp_mlsc_winx16               isp_mlsc_winx16; /* 0x5d00 */
    volatile u_isp_mlsc_winx17               isp_mlsc_winx17; /* 0x5d04 */
    volatile u_isp_mlsc_winx18               isp_mlsc_winx18; /* 0x5d08 */
    volatile u_isp_mlsc_winx19               isp_mlsc_winx19; /* 0x5d0c */
    volatile u_isp_mlsc_winx20               isp_mlsc_winx20; /* 0x5d10 */
    volatile u_isp_mlsc_winx21               isp_mlsc_winx21; /* 0x5d14 */
    volatile u_isp_mlsc_winx22               isp_mlsc_winx22; /* 0x5d18 */
    volatile u_isp_mlsc_winx23               isp_mlsc_winx23; /* 0x5d1c */
    volatile u_isp_mlsc_winx24               isp_mlsc_winx24; /* 0x5d20 */
    volatile u_isp_mlsc_winx25               isp_mlsc_winx25; /* 0x5d24 */
    volatile u_isp_mlsc_winx26               isp_mlsc_winx26; /* 0x5d28 */
    volatile u_isp_mlsc_winx27               isp_mlsc_winx27; /* 0x5d2c */
    volatile u_isp_mlsc_winx28               isp_mlsc_winx28; /* 0x5d30 */
    volatile u_isp_mlsc_winx29               isp_mlsc_winx29; /* 0x5d34 */
    volatile u_isp_mlsc_winx30               isp_mlsc_winx30; /* 0x5d38 */
    volatile u_isp_mlsc_winx31               isp_mlsc_winx31; /* 0x5d3c */
    volatile u_isp_mlsc_winy8                isp_mlsc_winy8; /* 0x5d40 */
    volatile u_isp_mlsc_winy9                isp_mlsc_winy9; /* 0x5d44 */
    volatile u_isp_mlsc_winy10               isp_mlsc_winy10; /* 0x5d48 */
    volatile u_isp_mlsc_winy11               isp_mlsc_winy11; /* 0x5d4c */
    volatile u_isp_mlsc_winy12               isp_mlsc_winy12; /* 0x5d50 */
    volatile u_isp_mlsc_winy13               isp_mlsc_winy13; /* 0x5d54 */
    volatile u_isp_mlsc_winy14               isp_mlsc_winy14; /* 0x5d58 */
    volatile u_isp_mlsc_winy15               isp_mlsc_winy15; /* 0x5d5c */
    volatile unsigned int                    reserved_105[168]; /* 0x5d60~0x5ffc */
    volatile u_isp_wdr_ctrl                  isp_wdr_ctrl; /* 0x6000 */
    volatile unsigned int                    reserved_106[3]; /* 0x6004~0x600c, reserved 3 * 4 bytes */
    volatile u_isp_wdr_f0_inblc0             isp_wdr_f0_inblc0; /* 0x6010 */
    volatile u_isp_wdr_f0_inblc1             isp_wdr_f0_inblc1; /* 0x6014 */
    volatile u_isp_wdr_f1_inblc0             isp_wdr_f1_inblc0; /* 0x6018 */
    volatile u_isp_wdr_f1_inblc1             isp_wdr_f1_inblc1; /* 0x601c */
    volatile u_isp_wdr_outblc                isp_wdr_outblc; /* 0x6020 */
    volatile u_isp_wdr_expovalue             isp_wdr_expovalue; /* 0x6024 */
    volatile u_isp_wdr_exporratio            isp_wdr_exporratio; /* 0x6028 */
    volatile u_isp_wdr_blc_comp              isp_wdr_blc_comp; /* 0x602c */
    volatile u_isp_wdr_maxratio              isp_wdr_maxratio; /* 0x6030 */
    volatile u_isp_wdr_saturate_thr          isp_wdr_saturate_thr; /* 0x6034 */
    volatile u_isp_wdr_still_thr             isp_wdr_still_thr; /* 0x6038 */
    volatile u_isp_wdr_mdtlbld               isp_wdr_mdtlbld; /* 0x603c */
    volatile u_isp_wdr_mdt_thr               isp_wdr_mdt_thr; /* 0x6040 */
    volatile u_isp_wdr_forcelong_para        isp_wdr_forcelong_para; /* 0x6044 */
    volatile u_isp_wdr_mdt_noisefloor        isp_wdr_mdt_noisefloor; /* 0x6048 */
    volatile u_isp_wdr_mdthr_gain            isp_wdr_mdthr_gain; /* 0x604c */
    volatile u_isp_wdr_shortexpo_chk         isp_wdr_shortexpo_chk; /* 0x6050 */
    volatile u_isp_wdr_pix_avg_diff          isp_wdr_pix_avg_diff; /* 0x6054 */
    volatile u_isp_wdr_mask_similar          isp_wdr_mask_similar; /* 0x6058 */
    volatile u_isp_wdr_wgtidx_blendratio     isp_wdr_wgtidx_blendratio; /* 0x605c */
    volatile u_isp_wdr_wgtidx_thr            isp_wdr_wgtidx_thr; /* 0x6060 */
    volatile u_isp_wdr_dftwgt_fl             isp_wdr_dftwgt_fl; /* 0x6064 */
    volatile u_isp_wdr_sensor_gain           isp_wdr_sensor_gain; /* 0x6068 */
    volatile u_isp_wdr_fusion_thr            isp_wdr_fusion_thr; /* 0x606c */
    volatile u_isp_wdr_fusion_sat_thd        isp_wdr_fusion_sat_thd; /* 0x6070 */
    volatile u_isp_wdr_awb_gain              isp_wdr_awb_gain; /* 0x6074 */
    volatile u_isp_wdr_awb_rev_gain          isp_wdr_awb_rev_gain; /* 0x6078 */
    volatile u_isp_wdr_fusion_blend          isp_wdr_fusion_blend; /* 0x607c */
    volatile unsigned int                    reserved_107[32]; /* 0x6080~0x60fc */
    volatile u_isp_wdr_lut_update            isp_wdr_lut_update; /* 0x6100 */
    volatile u_isp_wdr_lut_seg_idxbase0      isp_wdr_lut_seg_idxbase0; /* 0x6104 */
    volatile u_isp_wdr_lut_seg_idxbase1      isp_wdr_lut_seg_idxbase1; /* 0x6108 */
    volatile u_isp_wdr_lut_seg_idxbase2      isp_wdr_lut_seg_idxbase2; /* 0x610c */
    volatile u_isp_wdr_lut_seg_maxval0       isp_wdr_lut_seg_maxval0; /* 0x6110 */
    volatile u_isp_wdr_lut_seg_maxval1       isp_wdr_lut_seg_maxval1; /* 0x6114 */
    volatile u_isp_wdr_lut_seg_maxval2       isp_wdr_lut_seg_maxval2; /* 0x6118 */
    volatile u_isp_wdr_fusion_data_para      isp_wdr_fusion_data_para; /* 0x611c */
    volatile u_isp_wdr_fusion_lf_thd         isp_wdr_fusion_lf_thd;    /* 0x6120 */
    volatile u_isp_wdr_fusion_sat_rb_thd     isp_wdr_fusion_sat_rb_thd; /* 0x6124 */
    volatile u_isp_wdr_fusion_bld_thd        isp_wdr_fusion_bld_thd;   /* 0x6128 */
    volatile u_isp_wdr_fusion_calc_thd       isp_wdr_fusion_calc_thd;  /* 0x612c */
    volatile unsigned int                    reserved_145[20];         /* 0x6130~0x617c */
    volatile u_isp_wdr_snoise_lut_waddr      isp_wdr_snoise_lut_waddr; /* 0x6180 */
    volatile u_isp_wdr_snoise_lut_wdata      isp_wdr_snoise_lut_wdata; /* 0x6184 */
    volatile u_isp_wdr_snoise_lut_raddr      isp_wdr_snoise_lut_raddr; /* 0x6188 */
    volatile u_isp_wdr_snoise_lut_rdata      isp_wdr_snoise_lut_rdata; /* 0x618c */
    volatile u_isp_wdr_lnoise_lut_waddr      isp_wdr_lnoise_lut_waddr; /* 0x6190 */
    volatile u_isp_wdr_lnoise_lut_wdata      isp_wdr_lnoise_lut_wdata; /* 0x6194 */
    volatile u_isp_wdr_lnoise_lut_raddr      isp_wdr_lnoise_lut_raddr; /* 0x6198 */
    volatile u_isp_wdr_lnoise_lut_rdata      isp_wdr_lnoise_lut_rdata; /* 0x619c */
    volatile u_isp_wdr_normalize_lut_waddr   isp_wdr_normalize_lut_waddr; /* 0x61a0 */
    volatile u_isp_wdr_normalize_lut_wdata   isp_wdr_normalize_lut_wdata; /* 0x61a4 */
    volatile u_isp_wdr_normalize_lut_raddr   isp_wdr_normalize_lut_raddr; /* 0x61a8 */
    volatile u_isp_wdr_normalize_lut_rdata   isp_wdr_normalize_lut_rdata; /* 0x61ac */
    volatile unsigned int                    reserved_109[343]; /* 0x61b0~0x6708 */
    volatile u_isp_dehaze_version            isp_dehaze_version; /* 0x670c */
    volatile u_isp_dehaze_blk_size           isp_dehaze_blk_size; /* 0x6710 */
    volatile u_isp_dehaze_blk_sum            isp_dehaze_blk_sum; /* 0x6714 */
    volatile u_isp_dehaze_dc_size            isp_dehaze_dc_size; /* 0x6718 */
    volatile u_isp_dehaze_x                  isp_dehaze_x; /* 0x671c */
    volatile u_isp_dehaze_y                  isp_dehaze_y; /* 0x6720 */
    volatile u_isp_dehaze_air                isp_dehaze_air; /* 0x6724 */
    volatile u_isp_dehaze_thld               isp_dehaze_thld; /* 0x6728 */
    volatile u_isp_dehaze_gstrth             isp_dehaze_gstrth; /* 0x672c */
    volatile u_isp_dehaze_blthld             isp_dehaze_blthld; /* 0x6730 */
    volatile u_isp_dehaze_stt_bst            isp_dehaze_stt_bst; /* 0x6734 */
    volatile u_isp_dehaze_stt_abn            isp_dehaze_stt_abn; /* 0x6738 */
    volatile u_isp_dehaze_stt2lut_cfg        isp_dehaze_stt2lut_cfg; /* 0x673c */
    volatile u_isp_dehaze_stt2lut_regnew     isp_dehaze_stt2lut_regnew; /* 0x6740 */
    volatile u_isp_dehaze_stt2lut_abn        isp_dehaze_stt2lut_abn; /* 0x6744 */
    volatile u_isp_dehaze_smlmapoffset       isp_dehaze_smlmapoffset; /* 0x6748 */
    volatile u_isp_dehaze_stat_point         isp_dehaze_stat_point; /* 0x674c */
    volatile u_isp_dehaze_stat_num           isp_dehaze_stat_num; /* 0x6750 */
    volatile unsigned int                    reserved_110[11]; /* 0x6754~0x677c, reserved 11 * 4 bytes */
    volatile u_isp_dehaze_minstat_waddr      isp_dehaze_minstat_waddr; /* 0x6780 */
    volatile u_isp_dehaze_minstat_wdata      isp_dehaze_minstat_wdata; /* 0x6784 */
    volatile u_isp_dehaze_minstat_raddr      isp_dehaze_minstat_raddr; /* 0x6788 */
    volatile u_isp_dehaze_minstat_rdata      isp_dehaze_minstat_rdata; /* 0x678c */
    volatile u_isp_dehaze_maxstat_waddr      isp_dehaze_maxstat_waddr; /* 0x6790 */
    volatile u_isp_dehaze_maxstat_wdata      isp_dehaze_maxstat_wdata; /* 0x6794 */
    volatile u_isp_dehaze_maxstat_raddr      isp_dehaze_maxstat_raddr; /* 0x6798 */
    volatile u_isp_dehaze_maxstat_rdata      isp_dehaze_maxstat_rdata; /* 0x679c */
    volatile u_isp_dehaze_prestat_waddr      isp_dehaze_prestat_waddr; /* 0x67a0 */
    volatile u_isp_dehaze_prestat_wdata      isp_dehaze_prestat_wdata; /* 0x67a4 */
    volatile u_isp_dehaze_prestat_raddr      isp_dehaze_prestat_raddr; /* 0x67a8 */
    volatile u_isp_dehaze_prestat_rdata      isp_dehaze_prestat_rdata; /* 0x67ac */
    volatile u_isp_dehaze_lut_waddr          isp_dehaze_lut_waddr; /* 0x67b0 */
    volatile u_isp_dehaze_lut_wdata          isp_dehaze_lut_wdata; /* 0x67b4 */
    volatile u_isp_dehaze_lut_raddr          isp_dehaze_lut_raddr; /* 0x67b8 */
    volatile u_isp_dehaze_lut_rdata          isp_dehaze_lut_rdata; /* 0x67bc */
    volatile unsigned int                    reserved_111[20]; /* 0x67c0~0x680c, reserved 20 * 4 bytes */
    volatile u_isp_expander_bitw             isp_expander_bitw; /* 0x6810 */
    volatile u_isp_expander_offsetr          isp_expander_offsetr; /* 0x6814 */
    volatile u_isp_expander_offsetgr         isp_expander_offsetgr; /* 0x6818 */
    volatile u_isp_expander_offsetgb         isp_expander_offsetgb; /* 0x681c */
    volatile u_isp_expander_offsetb          isp_expander_offsetb; /* 0x6820 */
    volatile unsigned int                    reserved_112[23]; /* 0x6824~0x687c, reserved 23 * 4 bytes */
    volatile u_isp_expander_lut_waddr        isp_expander_lut_waddr; /* 0x6880 */
    volatile u_isp_expander_lut_wdata        isp_expander_lut_wdata; /* 0x6884 */
    volatile u_isp_expander_lut_raddr        isp_expander_lut_raddr; /* 0x6888 */
    volatile u_isp_expander_lut_rdata        isp_expander_lut_rdata; /* 0x688c */
    volatile unsigned int                    reserved_113[92]; /* 0x6890~0x69fc, reserved 92 * 4 bytes */
    volatile u_isp_gamma_cfg                 isp_gamma_cfg; /* 0x6a00 */
    volatile unsigned int                    reserved_114[7]; /* 0x6a04~0x6a1c, reserved 7 * 4 bytes */
    volatile u_isp_gamma_pos0                isp_gamma_pos0; /* 0x6a20 */
    volatile u_isp_gamma_pos1                isp_gamma_pos1; /* 0x6a24 */
    volatile u_isp_gamma_pos2                isp_gamma_pos2; /* 0x6a28 */
    volatile u_isp_gamma_pos3                isp_gamma_pos3; /* 0x6a2c */
    volatile u_isp_gamma_inseg0              isp_gamma_inseg0; /* 0x6a30 */
    volatile u_isp_gamma_inseg1              isp_gamma_inseg1; /* 0x6a34 */
    volatile u_isp_gamma_inseg2              isp_gamma_inseg2; /* 0x6a38 */
    volatile u_isp_gamma_inseg3              isp_gamma_inseg3; /* 0x6a3c */
    volatile u_isp_gamma_step                isp_gamma_step; /* 0x6a40 */
    volatile unsigned int                    reserved_115[3]; /* 0x6a44~0x6a4c, reserved 3 * 4 bytes */
    volatile u_isp_gamma_stt2lut_cfg         isp_gamma_stt2lut_cfg; /* 0x6a50 */
    volatile u_isp_gamma_stt2lut_regnew      isp_gamma_stt2lut_regnew; /* 0x6a54 */
    volatile u_isp_gamma_stt2lut_abn         isp_gamma_stt2lut_abn; /* 0x6a58 */
    volatile unsigned int                    reserved_116[9]; /* 0x6a5c~0x6a7c, reserved 9 * 4 bytes */
    volatile u_isp_gamma_lut_waddr           isp_gamma_lut_waddr; /* 0x6a80 */
    volatile u_isp_gamma_lut_wdata           isp_gamma_lut_wdata; /* 0x6a84 */
    volatile u_isp_gamma_lut_raddr           isp_gamma_lut_raddr; /* 0x6a88 */
    volatile u_isp_gamma_lut_rdata           isp_gamma_lut_rdata; /* 0x6a8c */
    volatile unsigned int                    reserved_117[92]; /* 0x6a90~0x6bfc, reserved 92 * 4 bytes */
    volatile u_isp_bcom_cfg                  isp_bcom_cfg; /* 0x6c00 */
    volatile unsigned int                    reserved_118[2]; /* 0x6c04~0x6c08, reserved 2 * 4 bytes */
    volatile u_isp_bcom_version              isp_bcom_version; /* 0x6c0c */
    volatile u_isp_bcom_alpha                isp_bcom_alpha; /* 0x6c10 */
    volatile unsigned int                    reserved_119[59]; /* 0x6c14~0x6cfc, reserved 59 * 4 bytes */
    volatile u_isp_bdec_cfg                  isp_bdec_cfg; /* 0x6d00 */
    volatile unsigned int                    reserved_120[2]; /* 0x6d04~0x6d08, reserved 2 * 4 bytes */
    volatile u_isp_bdec_version              isp_bdec_version; /* 0x6d0c */
    volatile u_isp_bdec_alpha                isp_bdec_alpha; /* 0x6d10 */
    volatile unsigned int                    reserved_121[123]; /* 0x6d14~0x6efc, reserved 123 * 4 bytes */
    volatile u_isp_crb_cfg                   isp_crb_cfg; /* 0x6f00 */
    volatile u_isp_crb_dc_size               isp_crb_dc_size; /* 0x6f04 */
    volatile u_isp_crb_kx                    isp_crb_kx; /* 0x6f08 */
    volatile u_isp_crb_ky                    isp_crb_ky; /* 0x6f0c */
    volatile u_isp_crb_smlmapoffset          isp_crb_smlmapoffset; /* 0x6f10 */
    volatile u_isp_crb_ccrcc                 isp_crb_ccrcc; /* 0x6f14 */
    volatile u_isp_crb_awbgain               isp_crb_awbgain; /* 0x6f18 */
    volatile u_isp_crb_gr                    isp_crb_gr; /* 0x6f1c */
    volatile u_isp_crb_cchigh                isp_crb_cchigh; /* 0x6f20 */
    volatile u_isp_crb_mix0                  isp_crb_mix0; /* 0x6f24 */
    volatile u_isp_crb_maxlevel              isp_crb_maxlevel; /* 0x6f28 */
    volatile u_isp_crb_highlevel             isp_crb_highlevel; /* 0x6f2c */
    volatile u_isp_crb_cc                    isp_crb_cc; /* 0x6f30 */
    volatile unsigned int                    reserved_122[3]; /* 0x6f34~0x6f3c, reserved 3 * 4 bytes */
    volatile u_isp_crb_stt2lut_cfg           isp_crb_stt2lut_cfg; /* 0x6f40 */
    volatile u_isp_crb_stt2lut_regnew        isp_crb_stt2lut_regnew; /* 0x6f44 */
    volatile u_isp_crb_stt2lut_abn           isp_crb_stt2lut_abn; /* 0x6f48 */
    volatile u_isp_crb_inblc_r               isp_crb_inblc_r; /* 0x6f4c */
    volatile u_isp_crb_inblc_gr              isp_crb_inblc_gr; /* 0x6f50 */
    volatile u_isp_crb_inblc_gb              isp_crb_inblc_gb; /* 0x6f54 */
    volatile u_isp_crb_inblc_b               isp_crb_inblc_b; /* 0x6f58 */
    volatile u_isp_crb_outblc_r              isp_crb_outblc_r; /* 0x6f5c */
    volatile u_isp_crb_outblc_gr             isp_crb_outblc_gr; /* 0x6f60 */
    volatile u_isp_crb_outblc_gb             isp_crb_outblc_gb; /* 0x6f64 */
    volatile u_isp_crb_outblc_b              isp_crb_outblc_b; /* 0x6f68 */
    volatile unsigned int                    reserved_123[5]; /* 0x6f6c~0x6f7c, reserved 5 * 4 bytes */
    volatile u_isp_crb_lut_waddr             isp_crb_lut_waddr; /* 0x6f80 */
    volatile u_isp_crb_lut_wdata             isp_crb_lut_wdata; /* 0x6f84 */
    volatile u_isp_crb_lut_raddr             isp_crb_lut_raddr; /* 0x6f88 */
    volatile u_isp_crb_lut_rdata             isp_crb_lut_rdata; /* 0x6f8c */
    volatile unsigned int                    reserved_124[32]; /* 0x6f90~0x700c, reserved 32 * 4 bytes */
    volatile u_isp_ca_ctrl                   isp_ca_ctrl; /* 0x7010 */
    volatile u_isp_ca_des                    isp_ca_des; /* 0x7014 */
    volatile u_isp_ca_isoratio               isp_ca_isoratio; /* 0x7018 */
    volatile u_isp_ca_sat_coef               isp_ca_sat_coef; /* 0x701c */
    volatile unsigned int                    reserved_125[24]; /* 0x7020~0x707c */
    volatile u_isp_ca_yratiolut_waddr        isp_ca_yratiolut_waddr; /* 0x7080 */
    volatile u_isp_ca_yratiolut_wdata        isp_ca_yratiolut_wdata; /* 0x7084 */
    volatile u_isp_ca_yratiolut_raddr        isp_ca_yratiolut_raddr; /* 0x7088 */
    volatile u_isp_ca_yratiolut_rdata        isp_ca_yratiolut_rdata; /* 0x708c */
    volatile u_isp_ca_ysatlut_waddr          isp_ca_ysatlut_waddr; /* 0x7090 */
    volatile u_isp_ca_ysatlut_wdata          isp_ca_ysatlut_wdata; /* 0x7094 */
    volatile u_isp_ca_ysatlut_raddr          isp_ca_ysatlut_raddr; /* 0x7098 */
    volatile u_isp_ca_ysatlut_rdata          isp_ca_ysatlut_rdata; /* 0x709c */
    volatile u_isp_ca_ych1lut_waddr          isp_ca_ych1lut_waddr; /* 0x70a0 */
    volatile u_isp_ca_ych1lut_wdata          isp_ca_ych1lut_wdata; /* 0x70a4 */
    volatile u_isp_ca_ych1lut_raddr          isp_ca_ych1lut_raddr; /* 0x70a8 */
    volatile u_isp_ca_ych1lut_rdata          isp_ca_ych1lut_rdata; /* 0x70ac */
    volatile u_isp_ca_ych2lut_waddr          isp_ca_ych2lut_waddr; /* 0x70b0 */
    volatile u_isp_ca_ych2lut_wdata          isp_ca_ych2lut_wdata; /* 0x70b4 */
    volatile u_isp_ca_ych2lut_raddr          isp_ca_ych2lut_raddr; /* 0x70b8 */
    volatile u_isp_ca_ych2lut_rdata          isp_ca_ych2lut_rdata; /* 0x70bc */
    volatile u_isp_ca_yslulut_waddr          isp_ca_yslulut_waddr; /* 0x70c0 */
    volatile u_isp_ca_yslulut_wdata          isp_ca_yslulut_wdata; /* 0x70c4 */
    volatile u_isp_ca_yslulut_raddr          isp_ca_yslulut_raddr; /* 0x70c8 */
    volatile u_isp_ca_yslulut_rdata          isp_ca_yslulut_rdata; /* 0x70cc */
    volatile u_isp_ca_yslvlut_waddr          isp_ca_yslvlut_waddr; /* 0x70d0 */
    volatile u_isp_ca_yslvlut_wdata          isp_ca_yslvlut_wdata; /* 0x70d4 */
    volatile u_isp_ca_yslvlut_raddr          isp_ca_yslvlut_raddr; /* 0x70d8 */
    volatile u_isp_ca_yslvlut_rdata          isp_ca_yslvlut_rdata; /* 0x70dc */
    volatile unsigned int                    reserved_126[12]; /* 0x70e0~0x710c, reserved 12 * 4 bytes */
    volatile u_isp_ca_stt2lut_cfg            isp_ca_stt2lut_cfg; /* 0x7110 */
    volatile u_isp_ca_stt2lut_regnew         isp_ca_stt2lut_regnew; /* 0x7114 */
    volatile u_isp_ca_stt2lut_abn            isp_ca_stt2lut_abn; /* 0x7118 */
    volatile unsigned int                    reserved_127[187]; /* 0x711c~0x7404, reserved 187 * 4 bytes */
    volatile u_isp_clut_adj0                 isp_clut_adj0; /* 0x7408 */
    volatile u_isp_clut_adj1                 isp_clut_adj1; /* 0x740c */
    volatile unsigned int                    reserved_128; /* 0x7410 */
    volatile u_isp_clut_stt2lut_cfg          isp_clut_stt2lut_cfg; /* 0x7414 */
    volatile u_isp_clut_stt2lut_regnew       isp_clut_stt2lut_regnew; /* 0x7418 */
    volatile u_isp_clut_stt2lut_abn          isp_clut_stt2lut_abn; /* 0x741c */
    volatile unsigned int                    reserved_129[24]; /* 0x7420~0x747c, reserved 24 * 4 bytes */
    volatile u_isp_clut_lut0_waddr           isp_clut_lut0_waddr; /* 0x7480 */
    volatile u_isp_clut_lut0_wdata           isp_clut_lut0_wdata; /* 0x7484 */
    volatile u_isp_clut_lut0_raddr           isp_clut_lut0_raddr; /* 0x7488 */
    volatile u_isp_clut_lut0_rdata           isp_clut_lut0_rdata; /* 0x748c */
    volatile u_isp_clut_lut1_waddr           isp_clut_lut1_waddr; /* 0x7490 */
    volatile u_isp_clut_lut1_wdata           isp_clut_lut1_wdata; /* 0x7494 */
    volatile u_isp_clut_lut1_raddr           isp_clut_lut1_raddr; /* 0x7498 */
    volatile u_isp_clut_lut1_rdata           isp_clut_lut1_rdata; /* 0x749c */
    volatile u_isp_clut_lut2_waddr           isp_clut_lut2_waddr; /* 0x74a0 */
    volatile u_isp_clut_lut2_wdata           isp_clut_lut2_wdata; /* 0x74a4 */
    volatile u_isp_clut_lut2_raddr           isp_clut_lut2_raddr; /* 0x74a8 */
    volatile u_isp_clut_lut2_rdata           isp_clut_lut2_rdata; /* 0x74ac */
    volatile u_isp_clut_lut3_waddr           isp_clut_lut3_waddr; /* 0x74b0 */
    volatile u_isp_clut_lut3_wdata           isp_clut_lut3_wdata; /* 0x74b4 */
    volatile u_isp_clut_lut3_raddr           isp_clut_lut3_raddr; /* 0x74b8 */
    volatile u_isp_clut_lut3_rdata           isp_clut_lut3_rdata; /* 0x74bc */
    volatile unsigned int                    reserved_130[48]; /* 0x74c0~0x757c, reserved 48 * 4 bytes */
    volatile u_isp_clut_lut4_waddr           isp_clut_lut4_waddr; /* 0x7580 */
    volatile u_isp_clut_lut4_wdata           isp_clut_lut4_wdata; /* 0x7584 */
    volatile u_isp_clut_lut4_raddr           isp_clut_lut4_raddr; /* 0x7588 */
    volatile u_isp_clut_lut4_rdata           isp_clut_lut4_rdata; /* 0x758c */
    volatile u_isp_clut_lut5_waddr           isp_clut_lut5_waddr; /* 0x7590 */
    volatile u_isp_clut_lut5_wdata           isp_clut_lut5_wdata; /* 0x7594 */
    volatile u_isp_clut_lut5_raddr           isp_clut_lut5_raddr; /* 0x7598 */
    volatile u_isp_clut_lut5_rdata           isp_clut_lut5_rdata; /* 0x759c */
    volatile u_isp_clut_lut6_waddr           isp_clut_lut6_waddr; /* 0x75a0 */
    volatile u_isp_clut_lut6_wdata           isp_clut_lut6_wdata; /* 0x75a4 */
    volatile u_isp_clut_lut6_raddr           isp_clut_lut6_raddr; /* 0x75a8 */
    volatile u_isp_clut_lut6_rdata           isp_clut_lut6_rdata; /* 0x75ac */
    volatile u_isp_clut_lut7_waddr           isp_clut_lut7_waddr; /* 0x75b0 */
    volatile u_isp_clut_lut7_wdata           isp_clut_lut7_wdata; /* 0x75b4 */
    volatile u_isp_clut_lut7_raddr           isp_clut_lut7_raddr; /* 0x75b8 */
    volatile u_isp_clut_lut7_rdata           isp_clut_lut7_rdata; /* 0x75bc */
    volatile unsigned int                    reserved_131[144]; /* 0x75c0~0x77fc, reserved 144 * 4 bytes */
    volatile u_isp_ldci_cfg                  isp_ldci_cfg; /* 0x7800 */
    volatile unsigned int                    reserved_132[2]; /* 0x7804~0x7808, reserved 2 * 4 bytes */
    volatile u_isp_ldci_version              isp_ldci_version; /* 0x780c */
    volatile unsigned int                    reserved_133; /* 0x7810 */
    volatile u_isp_ldci_calc_small_offset    isp_ldci_calc_small_offset; /* 0x7814 */
    volatile u_isp_ldci_zone                 isp_ldci_zone; /* 0x7818 */
    volatile unsigned int                    reserved_134; /* 0x781c */
    volatile u_isp_ldci_lpfstt_bst           isp_ldci_lpfstt_bst; /* 0x7820 */
    volatile u_isp_ldci_lpfstt_abn           isp_ldci_lpfstt_abn; /* 0x7824 */
    volatile u_isp_ldci_scale                isp_ldci_scale; /* 0x7828 */
    volatile unsigned int                    reserved_135[2]; /* 0x782c~0x7830, reserved 2 * 4 bytes */
    volatile u_isp_ldci_luma_sel             isp_ldci_luma_sel; /* 0x7834 */
    volatile u_isp_ldci_blc_ctrl             isp_ldci_blc_ctrl; /* 0x7838 */
    volatile unsigned int                    reserved_136[10]; /* 0x783c~0x7860, reserved 10 * 4 bytes */
    volatile u_isp_ldci_lpf_lpfcoef0         isp_ldci_lpf_lpfcoef0; /* 0x7864 */
    volatile u_isp_ldci_lpf_lpfcoef1         isp_ldci_lpf_lpfcoef1; /* 0x7868 */
    volatile u_isp_ldci_lpf_lpfcoef2         isp_ldci_lpf_lpfcoef2; /* 0x786c */
    volatile unsigned int                    reserved_137[2]; /* 0x7870~0x7874, reserved 2 * 4 bytes */
    volatile u_isp_ldci_lpf_lpfsft           isp_ldci_lpf_lpfsft; /* 0x7878 */
    volatile unsigned int                    reserved_138[5]; /* 0x787c~0x788c, reserved 5 * 4 bytes */
    volatile u_isp_ldci_he_waddr             isp_ldci_he_waddr; /* 0x7890 */
    volatile u_isp_ldci_he_wdata             isp_ldci_he_wdata; /* 0x7894 */
    volatile u_isp_ldci_he_raddr             isp_ldci_he_raddr; /* 0x7898 */
    volatile u_isp_ldci_he_rdata             isp_ldci_he_rdata; /* 0x789c */
    volatile u_isp_ldci_de_usm_waddr         isp_ldci_de_usm_waddr; /* 0x78a0 */
    volatile u_isp_ldci_de_usm_wdata         isp_ldci_de_usm_wdata; /* 0x78a4 */
    volatile u_isp_ldci_de_usm_raddr         isp_ldci_de_usm_raddr; /* 0x78a8 */
    volatile u_isp_ldci_de_usm_rdata         isp_ldci_de_usm_rdata; /* 0x78ac */
    volatile u_isp_ldci_cgain_waddr          isp_ldci_cgain_waddr; /* 0x78b0 */
    volatile u_isp_ldci_cgain_wdata          isp_ldci_cgain_wdata; /* 0x78b4 */
    volatile u_isp_ldci_cgain_raddr          isp_ldci_cgain_raddr; /* 0x78b8 */
    volatile u_isp_ldci_cgain_rdata          isp_ldci_cgain_rdata; /* 0x78bc */
    volatile unsigned int                    reserved_139[24]; /* 0x78c0~0x791c, reserved 24 * 4 bytes */
    volatile u_isp_ldci_stat_hpos            isp_ldci_stat_hpos; /* 0x7920 */
    volatile u_isp_ldci_stat_vpos            isp_ldci_stat_vpos; /* 0x7924 */
    volatile u_isp_ldci_stat_evratio         isp_ldci_stat_evratio; /* 0x7928 */
    volatile unsigned int                    reserved_140; /* 0x792c */
    volatile u_isp_ldci_stat_zone            isp_ldci_stat_zone; /* 0x7930 */
    volatile unsigned int                    reserved_141[7]; /* 0x7934~0x794c, reserved 7 * 4 bytes */
    volatile u_isp_ldci_blk_smlmapwidth0     isp_ldci_blk_smlmapwidth0; /* 0x7950 */
    volatile u_isp_ldci_blk_smlmapwidth1     isp_ldci_blk_smlmapwidth1; /* 0x7954 */
    volatile u_isp_ldci_blk_smlmapwidth2     isp_ldci_blk_smlmapwidth2; /* 0x7958 */
    volatile unsigned int                    reserved_142[3]; /* 0x795c~0x7964, reserved 3 * 4 bytes */
    volatile u_isp_ldci_chrdamp              isp_ldci_chrdamp; /* 0x7968 */
    volatile unsigned int                    reserved_143; /* 0x796c */
    volatile u_isp_ldci_stt2lut_cfg          isp_ldci_stt2lut_cfg; /* 0x7970 */
    volatile u_isp_ldci_stt2lut_regnew       isp_ldci_stt2lut_regnew; /* 0x7974 */
    volatile u_isp_ldci_stt2lut_abn          isp_ldci_stt2lut_abn; /* 0x7978 */
    volatile unsigned int                    reserved_144[13]; /* 0x797c~0x79ac, reserved 13 * 4 bytes */
    volatile u_isp_ldci_lpf_map_waddr        isp_ldci_lpf_map_waddr; /* 0x79b0 */
    volatile u_isp_ldci_lpf_map_wdata        isp_ldci_lpf_map_wdata; /* 0x79b4 */
    volatile u_isp_ldci_lpf_map_raddr        isp_ldci_lpf_map_raddr; /* 0x79b8 */
    volatile u_isp_ldci_lpf_map_rdata        isp_ldci_lpf_map_rdata; /* 0x79bc */
    volatile unsigned int                    reserved_153[400]; /* 0x79c0~0x7ffc, reserved 400 * 4 bytes */
    volatile u_isp_adrc_cfg                  isp_adrc_cfg; /* 0x8000 */
    volatile u_isp_adrc_lut_update           isp_adrc_lut_update; /* 0x8004 */
    volatile u_isp_adrc_version              isp_adrc_version; /* 0x8008 */
    volatile u_isp_adrc_strength             isp_adrc_strength; /* 0x800c */
    volatile u_isp_adrc_rgb_wgt              isp_adrc_rgb_wgt; /* 0x8010 */
    volatile u_isp_adrc_shp_cfg_raw          isp_adrc_shp_cfg_raw; /* 0x8014 */
    volatile u_isp_adrc_div_denom_log_raw    isp_adrc_div_denom_log_raw; /* 0x8018 */
    volatile u_isp_adrc_denom_exp_raw        isp_adrc_denom_exp_raw; /* 0x801c */
    volatile u_isp_adrc_shp_cfg_luma         isp_adrc_shp_cfg_luma; /* 0x8020 */
    volatile u_isp_adrc_div_denom_log_luma   isp_adrc_div_denom_log_luma; /* 0x8024 */
    volatile u_isp_adrc_denom_exp_luma       isp_adrc_denom_exp_luma; /* 0x8028 */
    volatile u_isp_adrc_idxbase0             isp_adrc_idxbase0; /* 0x802c */
    volatile u_isp_adrc_idxbase1             isp_adrc_idxbase1; /* 0x8030 */
    volatile u_isp_adrc_maxval0              isp_adrc_maxval0; /* 0x8034 */
    volatile u_isp_adrc_maxval1              isp_adrc_maxval1; /* 0x8038 */
    volatile u_isp_adrc_gain_clip            isp_adrc_gain_clip; /* 0x803c */
    volatile u_isp_adrc_dark_gain_lmt_y      isp_adrc_dark_gain_lmt_y; /* 0x8040 */
    volatile u_isp_adrc_dark_gain_lmt_c      isp_adrc_dark_gain_lmt_c; /* 0x8044 */
    volatile u_isp_adrc_bright_gain_lmt      isp_adrc_bright_gain_lmt; /* 0x8048 */
    volatile u_isp_adrc_lut_ctrl             isp_adrc_lut_ctrl; /* 0x804c */
    volatile u_isp_adrc_sup_dark0            isp_adrc_sup_dark0; /* 0x8050 */
    volatile u_isp_adrc_sup_bright0          isp_adrc_sup_bright0; /* 0x8054 */
    volatile u_isp_adrc_sup1                 isp_adrc_sup1; /* 0x8058 */
    volatile u_isp_adrc_bld_darkl            isp_adrc_bld_darkl; /* 0x805c */
    volatile u_isp_adrc_bld_brightl          isp_adrc_bld_brightl; /* 0x8060 */
    volatile u_isp_adrc_bld_darkd            isp_adrc_bld_darkd; /* 0x8064 */
    volatile u_isp_adrc_bld_brightd          isp_adrc_bld_brightd; /* 0x8068 */
    volatile u_isp_adrc_debug_info           isp_adrc_debug_info; /* 0x806c */
    volatile u_isp_adrc_gain_coef            isp_adrc_gain_coef; /* 0x8070 */
    volatile u_isp_adrc_pd_cfg               isp_adrc_pd_cfg; /* 0x8074 */
    volatile u_isp_adrc_pdw_cfg              isp_adrc_pdw_cfg; /* 0x8078 */
    volatile u_isp_adrc_wgt_tm               isp_adrc_wgt_tm; /* 0x807c */
    volatile u_isp_adrc_tmlut0_waddr         isp_adrc_tmlut0_waddr; /* 0x8080 */
    volatile u_isp_adrc_tmlut0_wdata         isp_adrc_tmlut0_wdata; /* 0x8084 */
    volatile u_isp_adrc_tmlut0_raddr         isp_adrc_tmlut0_raddr; /* 0x8088 */
    volatile u_isp_adrc_tmlut0_rdata         isp_adrc_tmlut0_rdata; /* 0x808c */
    volatile u_isp_adrc_cclut_waddr          isp_adrc_cclut_waddr; /* 0x8090 */
    volatile u_isp_adrc_cclut_wdata          isp_adrc_cclut_wdata; /* 0x8094 */
    volatile u_isp_adrc_cclut_raddr          isp_adrc_cclut_raddr; /* 0x8098 */
    volatile u_isp_adrc_cclut_rdata          isp_adrc_cclut_rdata; /* 0x809c */
    volatile u_isp_adrc_localmix_waddr       isp_adrc_localmix_waddr; /* 0x80a0 */
    volatile u_isp_adrc_localmix_wdata       isp_adrc_localmix_wdata; /* 0x80a4 */
    volatile u_isp_adrc_localmix_raddr       isp_adrc_localmix_raddr; /* 0x80a8 */
    volatile u_isp_adrc_localmix_rdata       isp_adrc_localmix_rdata; /* 0x80ac */
    volatile u_isp_adrc_detaildec_waddr      isp_adrc_detaildec_waddr; /* 0x80b0 */
    volatile u_isp_adrc_detaildec_wdata      isp_adrc_detaildec_wdata; /* 0x80b4 */
    volatile u_isp_adrc_detaildec_raddr      isp_adrc_detaildec_raddr; /* 0x80b8 */
    volatile u_isp_adrc_detaildec_rdata      isp_adrc_detaildec_rdata; /* 0x80bc */
    volatile u_isp_adrc_logcvt_waddr         isp_adrc_logcvt_waddr; /* 0x80c0 */
    volatile u_isp_adrc_logcvt_wdata         isp_adrc_logcvt_wdata; /* 0x80c4 */
    volatile u_isp_adrc_logcvt_raddr         isp_adrc_logcvt_raddr; /* 0x80c8 */
    volatile u_isp_adrc_logcvt_rdata         isp_adrc_logcvt_rdata; /* 0x80cc */
    volatile u_isp_adrc_logcvt_luma_waddr isp_adrc_logcvt_luma_waddr; /* 0x80d0 */
    volatile u_isp_adrc_logcvt_luma_wdata isp_adrc_logcvt_luma_wdata; /* 0x80d4 */
    volatile u_isp_adrc_logcvt_luma_raddr isp_adrc_logcvt_luma_raddr; /* 0x80d8 */
    volatile u_isp_adrc_logcvt_luma_rdata isp_adrc_logcvt_luma_rdata; /* 0x80dc */
    volatile unsigned int                    reserved_146[8]; /* 0x80e0~0x80fc */
    volatile u_isp_adrc_prev_luma_0          isp_adrc_prev_luma_0; /* 0x8100 */
    volatile u_isp_adrc_prev_luma_1          isp_adrc_prev_luma_1; /* 0x8104 */
    volatile u_isp_adrc_prev_luma_2          isp_adrc_prev_luma_2; /* 0x8108 */
    volatile u_isp_adrc_prev_luma_3          isp_adrc_prev_luma_3; /* 0x810c */
    volatile u_isp_adrc_prev_luma_4          isp_adrc_prev_luma_4; /* 0x8110 */
    volatile u_isp_adrc_prev_luma_5          isp_adrc_prev_luma_5; /* 0x8114 */
    volatile u_isp_adrc_prev_luma_6          isp_adrc_prev_luma_6; /* 0x8118 */
    volatile u_isp_adrc_prev_luma_7          isp_adrc_prev_luma_7; /* 0x811c */
    volatile u_isp_adrc_zone                 isp_adrc_zone; /* 0x8120 */
    volatile u_isp_adrc_zone_size            isp_adrc_zone_size; /* 0x8124 */
    volatile u_isp_adrc_zone_div0            isp_adrc_zone_div0; /* 0x8128 */
    volatile u_isp_adrc_zone_div1            isp_adrc_zone_div1; /* 0x812c */
    volatile u_isp_adrc_bin                  isp_adrc_bin; /* 0x8130 */
    volatile u_isp_adrc_zone_init            isp_adrc_zone_init; /* 0x8134 */
    volatile u_isp_adrc_bin_scale            isp_adrc_bin_scale; /* 0x8138 */
    volatile u_isp_adrc_stat_width           isp_adrc_stat_width; /* 0x813c */
    volatile u_isp_adrc_vbi_strength         isp_adrc_vbi_strength; /* 0x8140 */
    volatile u_isp_adrc_flt_cfg              isp_adrc_flt_cfg; /* 0x8144 */
    volatile u_isp_adrc_grad_rev             isp_adrc_grad_rev; /* 0x8148 */
    volatile u_isp_adrc_vbi_state            isp_adrc_vbi_state; /* 0x814c */
    volatile u_isp_adrc_blk_wgt_init         isp_adrc_blk_wgt_init; /* 0x8150 */
    volatile u_isp_adrc_wgtm_cfg             isp_adrc_wgtm_cfg; /* 0x8154 */
    volatile u_isp_adrc_wgtm_0               isp_adrc_wgtm_0; /* 0x8158 */
    volatile u_isp_adrc_wgtm_1               isp_adrc_wgtm_1; /* 0x815c */
    volatile u_isp_adrc_wgtm_2               isp_adrc_wgtm_2; /* 0x8160 */
    volatile u_isp_adrc_wgtm_3               isp_adrc_wgtm_3; /* 0x8164 */
    volatile u_isp_adrc_wgtm_pd_cfg          isp_adrc_wgtm_pd_cfg; /* 0x8168 */
    volatile u_isp_adrc_stat_cfg             isp_adrc_stat_cfg; /* 0x816c */
    volatile u_isp_adrc_luma_pw_thr isp_adrc_luma_pw_thr;  /* 0x8170 */
    volatile u_isp_adrc_luma_pw_slo isp_adrc_luma_pw_slo;  /* 0x8174 */
    volatile u_isp_adrc_luma_pw_sft isp_adrc_luma_pw_sft;  /* 0x8178 */
    volatile u_isp_adrc_maskc_cfg isp_adrc_maskc_cfg;      /* 0x817c */
    volatile unsigned int                    reserved_147[32]; /* 0x8180~0x81fc */
    volatile u_isp_adrc_stat_cfg00           isp_adrc_stat_cfg00; /* 0x8200 */
    volatile u_isp_adrc_stat_cfg01           isp_adrc_stat_cfg01; /* 0x8204 */
    volatile u_isp_adrc_stat_cfg02           isp_adrc_stat_cfg02; /* 0x8208 */
    volatile u_isp_adrc_stat_cfg03           isp_adrc_stat_cfg03; /* 0x820c */
    volatile u_isp_adrc_stat_cfg10           isp_adrc_stat_cfg10; /* 0x8210 */
    volatile u_isp_adrc_stat_cfg11           isp_adrc_stat_cfg11; /* 0x8214 */
    volatile u_isp_adrc_stat_cfg12           isp_adrc_stat_cfg12; /* 0x8218 */
    volatile u_isp_adrc_stat_cfg13           isp_adrc_stat_cfg13; /* 0x821c */
    volatile u_isp_adrc_stat_cfg20           isp_adrc_stat_cfg20; /* 0x8220 */
    volatile u_isp_adrc_stat_cfg21           isp_adrc_stat_cfg21; /* 0x8224 */
    volatile u_isp_adrc_stat_cfg22           isp_adrc_stat_cfg22; /* 0x8228 */
    volatile u_isp_adrc_stat_cfg23           isp_adrc_stat_cfg23; /* 0x822c */
    volatile u_isp_adrc_stat_sft0            isp_adrc_stat_sft0; /* 0x8230 */
    volatile u_isp_adrc_stat_sft1            isp_adrc_stat_sft1; /* 0x8234 */
    volatile u_isp_adrc_stat_sft2            isp_adrc_stat_sft2; /* 0x8238 */
    volatile unsigned int                    reserved_148[627]; /* 0x823c~0x8c04 */
    volatile u_isp_drclog_version            isp_drclog_version; /* 0x8c08 */
    volatile unsigned int                    reserved_149; /* 0x8c0c */
    volatile u_isp_drclog_shp_cfg_raw        isp_drclog_shp_cfg_raw; /* 0x8c10 */
    volatile u_isp_drclog_div_denom_log_raw  isp_drclog_div_denom_log_raw; /* 0x8c14 */
    volatile unsigned int                    reserved_500[58]; /* 0x8c18~0x8cfc */

    isp_be_lut_type                          be_lut; /* 0x8d00 ~ 0x19dfc, size 0x11100 */
} isp_be_reg_type;

typedef struct {
    volatile unsigned int                    reserved[3]; /* 0x2000~0x2008 */
    volatile u_isp_ae_version                isp_ae_version; /* 0x200c */
    volatile u_isp_ae_zone                   isp_ae_zone; /* 0x2010 */
    volatile u_isp_ae_skip_crg               isp_ae_skip_crg; /* 0x2014 */
    volatile u_isp_ae_total_stat             isp_ae_total_stat; /* 0x2018 */
    volatile u_isp_ae_count_stat             isp_ae_count_stat; /* 0x201c */
    volatile u_isp_ae_total_r_aver           isp_ae_total_r_aver; /* 0x2020 */
    volatile u_isp_ae_total_gr_aver          isp_ae_total_gr_aver; /* 0x2024 */
    volatile u_isp_ae_total_gb_aver          isp_ae_total_gb_aver; /* 0x2028 */
    volatile u_isp_ae_total_b_aver           isp_ae_total_b_aver; /* 0x202c */
    volatile u_isp_ae_hist_high              isp_ae_hist_high; /* 0x2030 */
    volatile unsigned int                    reserved_26[3]; /* 0x2034~0x203c */
    volatile u_isp_ae_bitmove                isp_ae_bitmove; /* 0x2040 */
    volatile u_isp_ae_offset_r_gr            isp_ae_offset_r_gr; /* 0x2044 */
    volatile unsigned int                    reserved_27; /* 0x2048 */
    volatile u_isp_ae_offset_gb_b            isp_ae_offset_gb_b; /* 0x204c */
    volatile unsigned int                    reserved_28[5]; /* 0x2050~0x2060 */
    volatile u_isp_ae_lut_update             isp_ae_lut_update; /* 0x2064 */
    volatile u_isp_ae_stt_bst                isp_ae_stt_bst; /* 0x2068 */
    volatile unsigned int                    reserved_29[2]; /* 0x206c~0x2070 */
    volatile u_isp_ae_crop_pos               isp_ae_crop_pos; /* 0x2074 */
    volatile u_isp_ae_crop_outsize           isp_ae_crop_outsize; /* 0x2078 */
    volatile unsigned int                    reserved_30; /* 0x207c */
    volatile u_isp_ae_hist_waddr             isp_ae_hist_waddr; /* 0x2080 */
    volatile u_isp_ae_hist_wdata             isp_ae_hist_wdata; /* 0x2084 */
    volatile u_isp_ae_hist_raddr             isp_ae_hist_raddr; /* 0x2088 */
    volatile u_isp_ae_hist_rdata             isp_ae_hist_rdata; /* 0x208c */
    volatile u_isp_ae_aver_r_gr_waddr        isp_ae_aver_r_gr_waddr; /* 0x2090 */
    volatile u_isp_ae_aver_r_gr_wdata        isp_ae_aver_r_gr_wdata; /* 0x2094 */
    volatile u_isp_ae_aver_r_gr_raddr        isp_ae_aver_r_gr_raddr; /* 0x2098 */
    volatile u_isp_ae_aver_r_gr_rdata        isp_ae_aver_r_gr_rdata; /* 0x209c */
    volatile u_isp_ae_aver_gb_b_waddr        isp_ae_aver_gb_b_waddr; /* 0x20a0 */
    volatile u_isp_ae_aver_gb_b_wdata        isp_ae_aver_gb_b_wdata; /* 0x20a4 */
    volatile u_isp_ae_aver_gb_b_raddr        isp_ae_aver_gb_b_raddr; /* 0x20a8 */
    volatile u_isp_ae_aver_gb_b_rdata        isp_ae_aver_gb_b_rdata; /* 0x20ac */
    volatile u_isp_ae_weight_waddr           isp_ae_weight_waddr; /* 0x20b0 */
    volatile u_isp_ae_weight_wdata           isp_ae_weight_wdata; /* 0x20b4 */
    volatile u_isp_ae_weight_raddr           isp_ae_weight_raddr; /* 0x20b8 */
    volatile u_isp_ae_weight_rdata           isp_ae_weight_rdata; /* 0x20bc */
    volatile unsigned int                    reserved_31[16]; /* 0x20c0~0x2108 */
} isp_be_ae_reg_type;

typedef struct {
    volatile unsigned int                    reserved[3]; /* 0x2100~0x2108 */
    volatile u_isp_awb_version               isp_awb_version; /* 0x210c */
    volatile u_isp_awb_zone                  isp_awb_zone; /* 0x2110 */
    volatile u_isp_awb_ctrl                  isp_awb_ctrl; /* 0x2114 */
    volatile u_isp_awb_thd_min               isp_awb_thd_min; /* 0x2118 */
    volatile u_isp_awb_thd_max               isp_awb_thd_max; /* 0x211c */
    volatile u_isp_awb_cr_mm                 isp_awb_cr_mm; /* 0x2120 */
    volatile u_isp_awb_cb_mm                 isp_awb_cb_mm; /* 0x2124 */
    volatile u_isp_awb_offset_comp           isp_awb_offset_comp; /* 0x2128 */
    volatile unsigned int                    reserved_32[5]; /* 0x212c~0x213c, reserved 5 * 4 bytes */
    volatile u_isp_awb_avg_r                 isp_awb_avg_r; /* 0x2140 */
    volatile u_isp_awb_avg_g                 isp_awb_avg_g; /* 0x2144 */
    volatile u_isp_awb_avg_b                 isp_awb_avg_b; /* 0x2148 */
    volatile u_isp_awb_cnt_all               isp_awb_cnt_all; /* 0x214c */
    volatile unsigned int                    reserved_33[4]; /* 0x2150~0x215c, reserved 4 * 4 bytes */
    volatile u_isp_awb_stt_bst               isp_awb_stt_bst; /* 0x2160 */
    volatile u_isp_awb_stt_abn               isp_awb_stt_abn; /* 0x2164 */
    volatile unsigned int                    reserved_34[3]; /* 0x2168~0x2170, reserved 3 * 4 bytes */
    volatile u_isp_awb_crop_pos              isp_awb_crop_pos; /* 0x2174 */
    volatile u_isp_awb_crop_outsize          isp_awb_crop_outsize; /* 0x2178 */
    volatile unsigned int                    reserved_35; /* 0x217c */
    volatile u_isp_awb_stat_waddr            isp_awb_stat_waddr; /* 0x2180 */
    volatile u_isp_awb_stat_wdata            isp_awb_stat_wdata; /* 0x2184 */
    volatile u_isp_awb_stat_raddr            isp_awb_stat_raddr; /* 0x2188 */
    volatile u_isp_awb_stat_rdata            isp_awb_stat_rdata; /* 0x218c */
    volatile unsigned int                    reserved_36[28]; /* 0x2190~0x21fc, reserved 28 * 4 bytes */
} isp_be_awb_reg_type;

typedef isp_be_reg_type isp_pre_be_reg_type;
typedef isp_be_reg_type isp_post_be_reg_type;

typedef struct {
    isp_be_reg_type      be_reg;       /* 0x0 ~ 0x19dfc, size 0x19e00 = (0x8d00 + 0x11100) */
    isp_viproc_reg_type  viproc_reg;
} isp_be_all_reg_type;

#endif /* ISP_REG_DEFINE_H */
