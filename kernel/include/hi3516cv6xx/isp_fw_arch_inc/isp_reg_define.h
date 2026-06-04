/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_REG_DEFINE_H
#define ISP_REG_DEFINE_H

#include "isp_lut_define.h"
#include "isp_viproc_reg_define.h"

/* Define the union u_ch_reg_newer */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ch_reg_newer : 1; /* [0]  */
        unsigned int reserved_0 : 31;  /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_reg_newer;

/* Define the union u_ch_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ch_update : 1;   /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_update;

/* Define the union u_ch_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ch_int_fstart : 1;      /* [0]  */
        unsigned int ch_int_cc_int : 1;      /* [1]  */
        unsigned int ch_int_buf_ovf : 1;     /* [2]  */
        unsigned int ch_int_field_throw : 1; /* [3]  */
        unsigned int ch_int_update_cfg : 1;  /* [4]  */
        unsigned int reserved_0 : 10;        /* [14..5]  */
        unsigned int ch_int_fstart_dly : 1;  /* [15]  */
        unsigned int ch_stt_ae_finish : 1;   /* [16]  */
        unsigned int reserved_1 : 1;         /* [17]  */
        unsigned int ch_stt_em_finish : 1;   /* [18]  */
        unsigned int ch_stt_int_buf_ovf : 1; /* [19]  */
        unsigned int ch_em_int_buf_ovf : 1;  /* [20]  */
        unsigned int    ch_tunl_line_int      : 1   ; /* [21]  */
        unsigned int reserved_2 : 10;        /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_int;

/* Define the union u_ch_int_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mask_fstart : 1;        /* [0]  */
        unsigned int mask_cc_int : 1;        /* [1]  */
        unsigned int mask_buf_ovf : 1;       /* [2]  */
        unsigned int mask_field_throw : 1;   /* [3]  */
        unsigned int mask_update_cfg : 1;    /* [4]  */
        unsigned int reserved_0 : 10;        /* [14..5]  */
        unsigned int mask_fstart_dly : 1;    /* [15]  */
        unsigned int mask_stt_ae_finish : 1; /* [16]  */
        unsigned int reserved_1 : 1;         /* [17]  */
        unsigned int mask_stt_em_finish : 1; /* [18]  */
        unsigned int mask_stt_buf_ovf : 1;   /* [19]  */
        unsigned int mask_em_buf_ovf : 1;    /* [20]  */
        unsigned int    mask_tunl_line_int    : 1   ; /* [21]  */
        unsigned int reserved_2 : 10;        /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_int_mask;

/* Define the union u_ch_wch_stt_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 17;            /* [16..0]  */
        unsigned int ch_wch_stt_finish_mode : 1; /* [17]  */
        unsigned int reserved_1 : 12;            /* [29..18]  */
        unsigned int ch_wch_stt_32b_align : 1;   /* [30]  */
        unsigned int reserved_2 : 1;             /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_wch_stt_cfg;

/* Define the union u_ch_wch_stt_ae_hist_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_hist_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_wch_stt_ae_hist_addr_l;
/* Define the union u_ch_wch_stt_ae_aver_r_gr_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_aver_r_gr_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_wch_stt_ae_aver_r_gr_addr_l;

/* Define the union u_ch_wch_stt_ae_aver_gb_b_addr_l */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ch_wch_stt_ae_aver_gb_b_addr_l : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ch_wch_stt_ae_aver_gb_b_addr_l;

typedef struct {
    volatile unsigned int                       reserved_0;                         /* 0x10000 */
    volatile u_isp_ch_reg_newer                     ch_reg_newer;                       /* 0x10004 */
    volatile u_isp_ch_update                        ch_update;                          /* 0x10008 */
    volatile unsigned int                       reserved_1[57]; /* 0x1000c~0x100ec, reserved 57 * 4 bytes */
    volatile u_isp_ch_int                           ch_int;                             /* 0x100f0 */
    volatile unsigned int                       reserved_2;                         /* 0x100f4 */
    volatile u_isp_ch_int_mask                      ch_int_mask;                        /* 0x100f8 */
    volatile unsigned int                       reserved_3[65]; /* 0x100fc~0x101fc, reserved 65 * 4 bytes */
    volatile u_isp_ch_wch_stt_cfg                   ch_wch_stt_cfg;                     /* 0x10200 */
    volatile unsigned int                       reserved_4;                         /* 0x10204 */
    volatile u_isp_ch_wch_stt_ae_hist_addr_l        ch_wch_stt_ae_hist_addr_l;          /* 0x10208 */
    volatile unsigned int                       reserved_5;                         /* 0x1020c */
    volatile u_isp_ch_wch_stt_ae_aver_r_gr_addr_l   ch_wch_stt_ae_aver_r_gr_addr_l;     /* 0x10210 */
    volatile unsigned int                       reserved_6;                         /* 0x10214 */
    volatile u_isp_ch_wch_stt_ae_aver_gb_b_addr_l   ch_wch_stt_ae_aver_gb_b_addr_l;     /* 0x10218 */
    volatile unsigned int                       reserved_7[25]; /* 0x1021c~0x1027c, reserved 25 * 4 bytes */
} isp_vicap_ch_reg_type;

/* Define the union u_isp_fe_version */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_patch : 8;   /* [7..0]  */
        unsigned int isp_fe_build : 8;   /* [15..8]  */
        unsigned int isp_fe_release : 8; /* [23..16]  */
        unsigned int isp_fe_version : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_version;

/* Define the union u_isp_fe_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_date : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_date;
/* Define the union u_isp_fe_fpga_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fpga_date : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_fpga_date;
/* Define the union u_isp_fe_module_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_module_posl : 16; /* [15..0]  */
        unsigned int isp_fe_module_posh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_module_pos;

/* Define the union u_isp_fe_fstart */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fstart : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_fstart;
/* Define the union u_isp_fe_user_define0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_user_define0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_user_define0;
/* Define the union u_isp_fe_user_define1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_user_define1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_user_define1;
/* Define the union u_isp_fe_startup */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fcnt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_startup;
/* Define the union u_isp_fe_format */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_format : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_format;
/* Define the union u_isp_fe_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_int_fstart : 1;     /* [0]  */
        unsigned int isp_fe_int_update_cfg : 1; /* [1]  */
        unsigned int isp_fe_int_cfg_loss : 1;   /* [2]  */
        unsigned int isp_fe_int_int_delay : 1;  /* [3]  */
        unsigned int isp_fe_int_ae1_int : 1;    /* [4]  */
        unsigned int reserved_0 : 1;            /* [5]  */
        unsigned int isp_fe_int_af1_int : 1;    /* [6]  */
        unsigned int isp_fe_int_dis1_int : 1;   /* [7]  */
        unsigned int isp_fe_int_done_int   : 1; /* [8]  */
        unsigned int reserved_1            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_int;

/* Define the union u_isp_fe_int_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_int_mask_fstart : 1;     /* [0]  */
        unsigned int isp_fe_int_mask_update_cfg : 1; /* [1]  */
        unsigned int isp_fe_int_mask_cfg_loss : 1;   /* [2]  */
        unsigned int isp_fe_int_mask_int_delay : 1;  /* [3]  */
        unsigned int isp_fe_int_mask_ae1_int : 1;    /* [4]  */
        unsigned int reserved_0 : 1;                 /* [5]  */
        unsigned int isp_fe_int_mask_af1_int : 1;    /* [6]  */
        unsigned int isp_fe_int_mask_dis1_int : 1;   /* [7]  */
        unsigned int isp_fe_int_mask_int   : 1   ; /* [8]  */
        unsigned int reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_int_mask;

/* Define the union u_isp_fe_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 2;      /* [1..0]  */
        unsigned int isp_colorbar_en : 1; /* [2]  */
        unsigned int isp_crop_en : 1;     /* [3]  */
        unsigned int isp_blc1_en : 1;     /* [4]  */
        unsigned int isp_rc_en : 1;       /* [5]  */
        unsigned int isp_hrs_en : 1;      /* [6]  */
        unsigned int isp_rlsc1_en : 1;    /* [7]  */
        unsigned int isp_dg2_en : 1;      /* [8]  */
        unsigned int isp_dis1_en : 1;     /* [9]  */
        unsigned int isp_af1_en : 1;      /* [10]  */
        unsigned int isp_wb1_en : 1;      /* [11]  */
        unsigned int isp_drcs_en : 1;     /* [12]  */
        unsigned int isp_ae1_en : 1;      /* [13]  */
        unsigned int isp_loglut1_en : 1;  /* [14]  */
        unsigned int isp_dg_en : 1;       /* [15]  */
        unsigned int isp_gamma_en : 1;    /* [16]  */
        unsigned int reserved_1 : 15;     /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_ctrl;

/* Define the union u_isp_fe_adapter_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fe_hsync_mode : 1; /* [0]  */
        unsigned int fe_vsync_mode : 1; /* [1]  */
        unsigned int reserved_0 : 30;   /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_adapter_cfg;

/* Define the union u_isp_fe_out_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fe_out_width : 16;  /* [15..0]  */
        unsigned int fe_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_out_size;

/* Define the union u_isp_fe_fstart_delay */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_delay : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_fstart_delay;
/* Define the union u_isp_fe_ctrl_f */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_rggb_cfg : 2; /* [1..0]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_ctrl_f;

/* Define the union u_isp_fe_ctrl_i */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_update_mode : 1; /* [0]  */
        unsigned int reserved_0 : 31;        /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_ctrl_i;

/* Define the union u_isp_fe_timing_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;         /* [0]  */
        unsigned int isp_fe_fix_timing : 13; /* [13..1]  */
        unsigned int reserved_1 : 18;        /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_timing_cfg;

/* Define the union u_isp_fe_reg_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_update : 1; /* [0]  */
        unsigned int reserved_0 : 31;   /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_reg_update;

/* Define the union u_isp_fe_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_width : 16;  /* [15..0]  */
        unsigned int isp_fe_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_size;

/* Define the union u_isp_fe_in_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_csc_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_in_sum_cfg;

/* Define the union u_isp_fe_out_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_yuv444_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;          /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_out_sum_cfg;

/* Define the union u_isp_fe_blk_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_blk_width : 16;  /* [15..0]  */
        unsigned int isp_fe_blk_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_blk_size;

/* Define the union u_isp_fe_blk_hblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_blk_b_hblank : 16; /* [15..0]  */
        unsigned int isp_fe_blk_f_hblank : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_blk_hblank;

/* Define the union u_isp_fe_blk_vblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fe_blk_b_vblank : 16; /* [15..0]  */
        unsigned int isp_fe_blk_f_vblank : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_blk_vblank;

/* Define the union u_isp_fe_in_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_y_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_in_sum0;
/* Define the union u_isp_fe_in_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_y_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_in_sum1;
/* Define the union u_isp_fe_out_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_yuv444_y_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_out_sum0;
/* Define the union u_isp_fe_out_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_yuv444_y_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fe_out_sum1;
/* Define the union u_isp_blc1_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc1_version;
/* Define the union u_isp_blc1_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc1_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_blc1_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc1_offset1;

/* Define the union u_isp_blc1_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc1_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_blc1_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc1_offset2;

/* Define the union u_isp_dg2_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_en_in : 1;  /* [0]  */
        unsigned int isp_dg2_en_out : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_blc_cfg;

/* Define the union u_isp_dg2_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_version;
/* Define the union u_isp_dg2_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_grgain : 16; /* [15..0]  */
        unsigned int isp_dg2_rgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_gain1;

/* Define the union u_isp_dg2_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_gbgain : 16; /* [15..0]  */
        unsigned int isp_dg2_bgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_gain2;

/* Define the union u_isp_dg2_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_dg2_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_blc_offset1;

/* Define the union u_isp_dg2_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_dg2_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_blc_offset2;

/* Define the union u_isp_dg2_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg2_clip_value : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;         /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg2_clip_value;

/* Define the union u_isp_wb1_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb1_en_in : 1;  /* [0]  */
        unsigned int isp_wb1_en_out : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_blc_cfg;

/* Define the union u_isp_wb1_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_version;
/* Define the union u_isp_wb1_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb1_grgain : 16; /* [15..0]  */
        unsigned int isp_wb1_rgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_gain1;

/* Define the union u_isp_wb1_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb1_gbgain : 16; /* [15..0]  */
        unsigned int isp_wb1_bgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_gain2;

/* Define the union u_isp_wb1_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb1_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_wb1_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_blc_offset1;

/* Define the union u_isp_wb1_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb1_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_wb1_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_blc_offset2;

/* Define the union u_isp_wb1_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb1_clip_value : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;         /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb1_clip_value;

/* Define the union u_isp_ae_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_version;
/* Define the union u_isp_ae_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_hnum : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;  /* [7..6]  */
        unsigned int isp_ae_vnum : 6; /* [13..8]  */
        unsigned int reserved_1 : 18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_zone;

/* Define the union u_isp_ae_skip_crg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_skip_x : 3;   /* [2..0]  */
        unsigned int isp_ae_offset_x : 1; /* [3]  */
        unsigned int isp_ae_skip_y : 3;   /* [6..4]  */
        unsigned int isp_ae_offset_y : 1; /* [7]  */
        unsigned int reserved_0 : 24;     /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_skip_crg;

/* Define the union u_isp_ae_total_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_total_pixels : 27; /* [26..0]  */
        unsigned int reserved_0 : 5;           /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_total_stat;

/* Define the union u_isp_ae_count_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_count_pixels : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_count_stat;

/* Define the union u_isp_ae_total_r_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_total_r_aver;
/* Define the union u_isp_ae_total_gr_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_total_gr_aver;
/* Define the union u_isp_ae_total_gb_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_total_gb_aver;
/* Define the union u_isp_ae_total_b_aver */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_total_b_aver;
/* Define the union u_isp_ae_hist_high */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_high : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_hist_high;

/* Define the union u_isp_ae_bitmove */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_bitmove        : 5   ; /* [4..0]  */
        unsigned int    isp_ae_hist_gamma_mode : 2   ; /* [6..5]  */
        unsigned int    isp_ae_aver_gamma_mode : 2   ; /* [8..7]  */
        unsigned int    isp_ae_blc_en         : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fe_ae_bitmove;

/* Define the union u_isp_ae_bitmove */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_bitmove : 5;         /* [4..0]  */
        unsigned int isp_ae_hist_gamma_mode : 2; /* [6..5]  */
        unsigned int isp_ae_aver_gamma_mode : 2; /* [8..7]  */
        unsigned int isp_ae_blc_en : 1;          /* [9]  */
        unsigned int isp_ae_gamma_limit : 4;     /* [13..10]  */
        unsigned int isp_ae_fourplanemode : 1;   /* [14]  */
        unsigned int reserved_0 : 17;            /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_bitmove;


/* Define the union u_isp_ae_offset_r_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_offset_r : 15;  /* [14..0]  */
        unsigned int reserved_0 : 1;        /* [15]  */
        unsigned int isp_ae_offset_gr : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_offset_r_gr;

/* Define the union u_isp_ae_offset_gb_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_offset_gb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;        /* [15]  */
        unsigned int isp_ae_offset_b : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_offset_gb_b;

/* Define the union u_isp_ae_lut_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_lut_update : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_lut_update;

/* Define the union u_isp_ae_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_stt_size : 16; /* [15..0]  */
        unsigned int isp_ae_stt_bst : 4;   /* [19..16]  */
        unsigned int isp_ae_stt_en : 1;    /* [20]  */
        unsigned int reserved_0 : 11;      /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_stt_bst;

/* Define the union u_isp_ae_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_crop_pos_x : 16; /* [15..0]  */
        unsigned int isp_ae_crop_pos_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_crop_pos;

/* Define the union u_isp_ae_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_crop_out_width : 16;  /* [15..0]  */
        unsigned int isp_ae_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_crop_outsize;

/* Define the union u_isp_ae_hist_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_hist_waddr;
/* Define the union u_isp_ae_hist_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_hist_wdata;
/* Define the union u_isp_ae_hist_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_hist_raddr;
/* Define the union u_isp_ae_hist_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_hist_rdata;
/* Define the union u_isp_ae_aver_r_gr_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_r_gr_waddr;
/* Define the union u_isp_ae_aver_r_gr_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_r_gr_wdata;
/* Define the union u_isp_ae_aver_r_gr_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_r_gr_raddr;
/* Define the union u_isp_ae_aver_r_gr_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_r_gr_rdata;
/* Define the union u_isp_ae_aver_gb_b_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_gb_b_waddr;
/* Define the union u_isp_ae_aver_gb_b_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_gb_b_wdata;
/* Define the union u_isp_ae_aver_gb_b_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_gb_b_raddr;
/* Define the union u_isp_ae_aver_gb_b_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_aver_gb_b_rdata;
/* Define the union u_isp_ae_weight_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_wei_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_weight_waddr;
/* Define the union u_isp_ae_weight_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_wei_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_weight_wdata;
/* Define the union u_isp_ae_weight_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_weight_raddr;
/* Define the union u_isp_ae_weight_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ae_weight_rdata;

/* Define the union u_isp_gamma_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_version      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;
} u_isp_gamma_version;
/* Define the union u_isp_gamma_bitw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_bitw_out    : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_gamma_bitw_in     : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_gamma_bitw;

/* Define the union u_isp_rc_blcen */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rc_blc_in_en : 1;  /* [0]  */
        unsigned int reserved_0 : 7;        /* [7..1]  */
        unsigned int isp_rc_blc_out_en : 1; /* [8]  */
        unsigned int reserved_1 : 23;       /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rc_blcen;

/* Define the union u_isp_rc_blc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rc_blc_r : 15;  /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_rc_blc_gr : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rc_blc0;

/* Define the union u_isp_rc_blc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rc_blc_b : 15;  /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_rc_blc_gb : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rc_blc1;

/* Define the union u_isp_rc_sqradius */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rc_sqradius : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rc_sqradius;
/* Define the union u_isp_rc_center_coor */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rc_cenhor_coor : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;          /* [15..14]  */
        unsigned int isp_rc_cenver_coor : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;          /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rc_center_coor;

/* Define the union u_isp_crop_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_crop_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_crop_version;
/* Define the union u_isp_crop_sizeout */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_crop_width_out : 16;  /* [15..0]  */
        unsigned int isp_crop_height_out : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_crop_sizeout;

/* Define the union u_isp_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_crop_pos_x : 16; /* [15..0]  */
        unsigned int isp_crop_pos_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_crop_pos;

/* be reg */

/* Define the union u_isp_be_version */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_patch : 8;   /* [7..0]  */
        unsigned int isp_build : 8;   /* [15..8]  */
        unsigned int isp_release : 8; /* [23..16]  */
        unsigned int isp_version : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_version;

/* Define the union u_isp_be_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_date : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_date;
/* Define the union u_isp_be_fpga_date */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpga_date : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_fpga_date;
/* Define the union u_isp_be_module_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ae_sel : 2;   /* [1..0]  */
        unsigned int isp_awb_sel : 2;  /* [3..2]  */
        unsigned int isp_af_sel : 2;   /* [5..4]  */
        unsigned int isp_dis_sel : 1;  /* [6]  */
        unsigned int isp_clut_sel : 1; /* [7]  */
        unsigned int isp_dcg_sel : 1;  /* [8]  */
        unsigned int isp_mot_sel : 1   ; /* [9]  */
        unsigned int    isp_wrnt_sel          : 1   ; /* [10]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_module_pos;

/* Define the union u_isp_be_fstart_delay */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_delay : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_fstart_delay;
/* Define the union u_isp_be_input_mux */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_input0_sel : 2; /* [1..0]  */
        unsigned int reserved_0 : 2;     /* [3..2]  */
        unsigned int isp_input1_sel : 2; /* [5..4]  */
        unsigned int reserved_1 : 2;     /* [7..6]  */
        unsigned int isp_input2_sel : 2; /* [9..8]  */
        unsigned int reserved_2 : 2;     /* [11..10]  */
        unsigned int isp_input3_sel : 2; /* [13..12]  */
        unsigned int reserved_3 : 2;     /* [15..14]  */
        unsigned int isp_input4_sel : 1; /* [16]  */
        unsigned int reserved_4 : 15;    /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_input_mux;

/* Define the union u_isp_be_sys_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_stt_en : 1;  /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_sys_ctrl;

/* Define the union u_isp_be_user_define0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_user_define0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_user_define0;
/* Define the union u_isp_be_user_define1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_user_define1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_user_define1;
/* Define the union u_isp_be_startup */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fcnt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_startup;
/* Define the union u_isp_be_format */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_format : 2;  /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_format;

/* Define the union u_isp_be_mem_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mem_cfg           : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_mem_cfg;

/* Define the union u_isp_be_mem_err */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mem_err       : 1; /* [0]  */
        unsigned int    isp_dcg_mem_err       : 1; /* [1]  */
        unsigned int    isp_bnr_mem_err       : 1; /* [2]  */
        unsigned int    isp_drc_mem_err       : 1; /* [3]  */
        unsigned int    isp_dmnr_mem_err      : 1; /* [4]  */
        unsigned int    isp_sharpen_mem_err   : 1; /* [5]  */
        unsigned int    isp_npu_mem_err       : 1; /* [6]  */
        unsigned int    isp_npu_mem_range_err : 1; /* [7]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_be_mem_err;

/* Define the union u_isp_clip_y_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_clip_y_min : 16; /* [15..0]  */
        unsigned int isp_clip_y_max : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_clip_y_cfg;

/* Define the union u_isp_clip_c_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_clip_c_min : 16; /* [15..0]  */
        unsigned int isp_clip_c_max : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_clip_c_cfg;

/* Define the union u_isp_csc_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_csc_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;    /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_sum_cfg;

/* Define the union u_isp_yuv444_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_yuv444_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_sum_cfg;

/* Define the union u_isp_yuv422_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_yuv422_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv422_sum_cfg;

/* Define the union u_isp_wdr_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;    /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_sum_cfg;

/* Define the union u_isp_demosaic_sum_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_sum_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_sum_cfg;

/* Define the union u_isp_dmnr_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dmnr_dither_en : 1;           /* [0]  */
        unsigned int isp_dmnr_dither_round : 2;        /* [2..1]  */
        unsigned int isp_dmnr_dither_spatial_mode : 1; /* [3]  */
        unsigned int isp_dmnr_dither_out_bits : 4;     /* [7..4]  */
        unsigned int reserved_0 : 24;                  /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dmnr_dither;

/* Define the union u_isp_acm_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_acm_dither_en : 1;           /* [0]  */
        unsigned int isp_acm_dither_round : 2;        /* [2..1]  */
        unsigned int isp_acm_dither_spatial_mode : 1; /* [3]  */
        unsigned int isp_acm_dither_out_bits : 4;     /* [7..4]  */
        unsigned int reserved_0 : 24;                 /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_acm_dither;

/* Define the union u_isp_drc_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_dither_en : 1;           /* [0]  */
        unsigned int isp_drc_dither_round : 2;        /* [2..1]  */
        unsigned int isp_drc_dither_spatial_mode : 1; /* [3]  */
        unsigned int isp_drc_dither_out_bits : 4;     /* [7..4]  */
        unsigned int reserved_0 : 24;                 /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_dither;

/* Define the union u_isp_sqrt1_dither */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sqrt1_dither_en : 1;           /* [0]  */
        unsigned int isp_sqrt1_dither_round : 2;        /* [2..1]  */
        unsigned int isp_sqrt1_dither_spatial_mode : 1; /* [3]  */
        unsigned int isp_sqrt1_dither_out_bits : 4;     /* [7..4]  */
        unsigned int reserved_0 : 24;                   /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sqrt1_dither;

/* Define the union u_isp_blk_hblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blk_b_hblank : 16; /* [15..0]  */
        unsigned int isp_blk_f_hblank : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blk_hblank;

/* Define the union u_isp_blk_vblank */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blk_b_vblank : 16; /* [15..0]  */
        unsigned int isp_blk_f_vblank : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blk_vblank;

/* Define the union u_isp_y_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_y_sum0;
/* Define the union u_isp_y_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_y_sum1;
/* Define the union u_isp_yuv444_y_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_y_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_y_sum0;
/* Define the union u_isp_yuv444_y_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_y_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_y_sum1;
/* Define the union u_isp_yuv444_u_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_u_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_u_sum0;
/* Define the union u_isp_yuv444_u_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_u_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_u_sum1;
/* Define the union u_isp_yuv444_v_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_v_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_v_sum0;
/* Define the union u_isp_yuv444_v_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_v_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv444_v_sum1;
/* Define the union u_isp_yuv422_y_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_y_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv422_y_sum0;
/* Define the union u_isp_yuv422_y_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_y_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv422_y_sum1;
/* Define the union u_isp_yuv422_c_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_c_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv422_c_sum0;
/* Define the union u_isp_yuv422_c_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_c_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_yuv422_c_sum1;
/* Define the union u_isp_wdr0_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr0_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr0_sum0;
/* Define the union u_isp_wdr0_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr0_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr0_sum1;
/* Define the union u_isp_wdr1_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr1_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr1_sum0;
/* Define the union u_isp_wdr1_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr1_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr1_sum1;
/* Define the union u_isp_wdr2_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr2_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr2_sum0;
/* Define the union u_isp_wdr2_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr2_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr2_sum1;
/* Define the union u_isp_wdr3_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr3_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr3_sum0;
/* Define the union u_isp_wdr3_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr3_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr3_sum1;
/* Define the union u_isp_demosaic_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_demosaic_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_sum0;
/* Define the union u_isp_demosaic_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_demosaic_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_sum1;
/* Define the union u_isp_blc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_version;
/* Define the union u_isp_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_blc_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_offset1;

/* Define the union u_isp_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_blc_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;     /* [15]  */
        unsigned int isp_blc_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_blc_offset2;


/* Define the union u_isp_wb_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_en_in : 1;  /* [0]  */
        unsigned int isp_wb_en_out : 1; /* [1]  */
        unsigned int reserved_0 : 30;   /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_blc_cfg;

/* Define the union u_isp_wb_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_version;
/* Define the union u_isp_wb_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_grgain : 16; /* [15..0]  */
        unsigned int isp_wb_rgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_gain1;

/* Define the union u_isp_wb_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_gbgain : 16; /* [15..0]  */
        unsigned int isp_wb_bgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_gain2;

/* Define the union u_isp_wb_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;    /* [15]  */
        unsigned int isp_wb_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_blc_offset1;

/* Define the union u_isp_wb_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;    /* [15]  */
        unsigned int isp_wb_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_blc_offset2;

/* Define the union u_isp_wb_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wb_clip_value : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wb_clip_value;

/* Define the union u_isp_wb_blc_offset_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_blc_offset_shift : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_wb_blc_offset_shift;

/* Define the union u_isp_dg_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_en_in : 1;  /* [0]  */
        unsigned int isp_dg_en_out : 1; /* [1]  */
        unsigned int reserved_0 : 30;   /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_blc_cfg;

/* Define the union u_isp_dg_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_version;
/* Define the union u_isp_dg_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_grgain : 16; /* [15..0]  */
        unsigned int isp_dg_rgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_gain1;

/* Define the union u_isp_dg_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_gbgain : 16; /* [15..0]  */
        unsigned int isp_dg_bgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_gain2;

/* Define the union u_isp_dg_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;    /* [15]  */
        unsigned int isp_dg_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_blc_offset1;

/* Define the union u_isp_dg_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;    /* [15]  */
        unsigned int isp_dg_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_blc_offset2;

/* Define the union u_isp_dg_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dg_clip_value : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dg_clip_value;

/* Define the union u_isp_dg_blc_offset_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_blc_offset_shift : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dg_blc_offset_shift;

/* Define the union u_isp_cc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 2;          /* [1..0]  */
        unsigned int isp_cc_colortone_en : 1; /* [2]  */
        unsigned int isp_cc_recover_en : 1;   /* [3]  */
        unsigned int reserved_1 : 1;          /* [4]  */
        unsigned int isp_cc_lutb_en : 1;      /* [5]  */
        unsigned int isp_cc_hue_en : 1;       /* [6]  */
        unsigned int reserved_2 : 25;         /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_cfg;
/* Define the union u_isp_cc_coef00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef000        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef001        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef00;

/* Define the union u_isp_cc_coef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef002        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef010        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef01;

/* Define the union u_isp_cc_coef02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef011        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef012        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef02;

/* Define the union u_isp_cc_coef03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef020        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef021        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef03;

/* Define the union u_isp_cc_coef04 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef022        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef04;

/* Define the union u_isp_cc_coef10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef100        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef101        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef10;

/* Define the union u_isp_cc_coef11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef102        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef110        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef11;

/* Define the union u_isp_cc_coef12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef111        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef112        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef12;

/* Define the union u_isp_cc_coef13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef120        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef121        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef13;

/* Define the union u_isp_cc_coef14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef122        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef14;

/* Define the union u_isp_cc_coef20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef200        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef201        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef20;

/* Define the union u_isp_cc_coef21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef202        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef210        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef21;

/* Define the union u_isp_cc_coef22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef211        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef212        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef22;

/* Define the union u_isp_cc_coef23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef220        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef221        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef23;

/* Define the union u_isp_cc_coef24 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef222        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef24;

/* Define the union u_isp_cc_coef30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef300        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef301        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef30;

/* Define the union u_isp_cc_coef31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef302        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef310        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef31;

/* Define the union u_isp_cc_coef32 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef311        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef312        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef32;

/* Define the union u_isp_cc_coef33 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef320        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef321        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef33;

/* Define the union u_isp_cc_coef34 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef322        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef34;

/* Define the union u_isp_cc_coef40 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef400        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef401        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef40;

/* Define the union u_isp_cc_coef41 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef402        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef410        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef41;

/* Define the union u_isp_cc_coef42 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef411        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef412        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef42;

/* Define the union u_isp_cc_coef43 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef420        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef421        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef43;

/* Define the union u_isp_cc_coef44 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef422        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef44;

/* Define the union u_isp_cc_coef50 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef500        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef501        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef50;

/* Define the union u_isp_cc_coef51 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef502        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef510        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef51;

/* Define the union u_isp_cc_coef52 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef511        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef512        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef52;

/* Define the union u_isp_cc_coef53 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef520        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef521        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef53;

/* Define the union u_isp_cc_coef54 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef522        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef54;

/* Define the union u_isp_cc_coef60 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef600        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef601        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef60;

/* Define the union u_isp_cc_coef61 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef602        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef610        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef61;

/* Define the union u_isp_cc_coef62 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef611        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef612        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef62;

/* Define the union u_isp_cc_coef63 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef620        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef621        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef63;

/* Define the union u_isp_cc_coef64 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef622        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef64;

/* Define the union u_isp_cc_coef70 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef700        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef701        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef70;

/* Define the union u_isp_cc_coef71 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef702        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef710        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef71;

/* Define the union u_isp_cc_coef72 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef711        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef712        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef72;

/* Define the union u_isp_cc_coef73 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef720        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 1; /* [16]  */
        unsigned int    isp_cc_coef721        : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef73;

/* Define the union u_isp_cc_coef74 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1; /* [0]  */
        unsigned int    isp_cc_coef722        : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_coef74;

/* Define the union u_isp_cc_hue_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_hue0           : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_cc_hue1           : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_hue_coef0;

/* Define the union u_isp_cc_hue_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_hue2           : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_cc_hue3           : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_hue_coef1;

/* Define the union u_isp_cc_hue_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_hue4           : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_cc_hue5           : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_hue_coef2;

/* Define the union u_isp_cc_hue_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_hue6           : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_cc_hue7           : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_hue_coef3;

/* Define the union u_isp_cc_hue_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_matrix_gray_en : 1; /* [0]  */
        unsigned int    reserved_0            : 15; /* [15..1]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_hue_cfg;

/* Define the union u_isp_cc_rcv_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_soft_clip0_step : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;             /* [7..6]  */
        unsigned int isp_cc_soft_clip1_step : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;             /* [15..14]  */
        unsigned int reserved_2 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_rcv_ctrl0;

/* Define the union u_isp_cc_rcv_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_darkprev : 5;     /* [4..0]  */
        unsigned int reserved_0 : 3;          /* [7..5]  */
        unsigned int isp_cc_peaksupp_sat : 8; /* [15..8]  */
        unsigned int isp_cc_peaksupp_max : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;          /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_rcv_ctrl1;

/* Define the union u_isp_cc_lumafact */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_luma_coefr : 5;    /* [4..0]  */
        unsigned int reserved_0 : 3;           /* [7..5]  */
        unsigned int isp_cc_luma_coefb : 5;    /* [12..8]  */
        unsigned int reserved_1 : 3;           /* [15..13]  */
        unsigned int isp_cc_luma_coefr_up : 5; /* [20..16]  */
        unsigned int reserved_2 : 3;           /* [23..21]  */
        unsigned int isp_cc_luma_coefb_up : 5; /* [28..24]  */
        unsigned int reserved_3 : 3;           /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_lumafact;

/* Define the union u_isp_cc_colortone_rb_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_b_gain : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;     /* [15..12]  */
        unsigned int isp_cc_r_gain : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;     /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_colortone_rb_gain;

/* Define the union u_isp_cc_colortone_g_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_g_gain : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;    /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_colortone_g_gain;

/* Define the union u_isp_cc_in_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_in_dc0 : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;    /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_in_dc0;

/* Define the union u_isp_cc_in_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_in_dc1 : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;    /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_in_dc1;

/* Define the union u_isp_cc_in_dc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_in_dc2 : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;    /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_in_dc2;

/* Define the union u_isp_cc_out_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_out_dc0 : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;     /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_out_dc0;

/* Define the union u_isp_cc_out_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_out_dc1 : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;     /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_out_dc1;

/* Define the union u_isp_cc_out_dc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_out_dc2 : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;     /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_out_dc2;

/* Define the union u_isp_cc_dgain_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_pos_0 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;          /* [15..13]  */
        unsigned int isp_cc_dgain_pos_1 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_pos0;

/* Define the union u_isp_cc_dgain_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_pos_2 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;          /* [15..13]  */
        unsigned int isp_cc_dgain_pos_3 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_pos1;

/* Define the union u_isp_cc_dgain_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_pos_4 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;          /* [15..13]  */
        unsigned int isp_cc_dgain_pos_5 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_pos2;

/* Define the union u_isp_cc_dgain_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_pos_6 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;          /* [15..13]  */
        unsigned int isp_cc_dgain_pos_7 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;          /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_pos3;

/* Define the union u_isp_cc_dgain_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_inseg_0 : 7; /* [6..0]  */
        unsigned int isp_cc_dgain_inseg_1 : 7; /* [13..7]  */
        unsigned int isp_cc_dgain_inseg_2 : 7; /* [20..14]  */
        unsigned int isp_cc_dgain_inseg_3 : 7; /* [27..21]  */
        unsigned int reserved_0 : 4;           /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_inseg0;

/* Define the union u_isp_cc_dgain_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_inseg_4 : 7; /* [6..0]  */
        unsigned int isp_cc_dgain_inseg_5 : 7; /* [13..7]  */
        unsigned int isp_cc_dgain_inseg_6 : 7; /* [20..14]  */
        unsigned int isp_cc_dgain_inseg_7 : 7; /* [27..21]  */
        unsigned int reserved_0 : 4;           /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_inseg1;

/* Define the union u_isp_cc_dgain_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_step0 : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;         /* [3]  */
        unsigned int isp_cc_dgain_step1 : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;         /* [7]  */
        unsigned int isp_cc_dgain_step2 : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;         /* [11]  */
        unsigned int isp_cc_dgain_step3 : 3; /* [14..12]  */
        unsigned int reserved_3 : 1;         /* [15]  */
        unsigned int isp_cc_dgain_step4 : 3; /* [18..16]  */
        unsigned int reserved_4 : 1;         /* [19]  */
        unsigned int isp_cc_dgain_step5 : 3; /* [22..20]  */
        unsigned int reserved_5 : 1;         /* [23]  */
        unsigned int isp_cc_dgain_step6 : 3; /* [26..24]  */
        unsigned int reserved_6 : 1;         /* [27]  */
        unsigned int isp_cc_dgain_step7 : 3; /* [30..28]  */
        unsigned int reserved_7 : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_step;

/* Define the union u_isp_cc_gamma_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_pos_0 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_cc_gamma_pos_1 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_pos0;

/* Define the union u_isp_cc_gamma_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_pos_2 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_cc_gamma_pos_3 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_pos1;

/* Define the union u_isp_cc_gamma_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_pos_4 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_cc_gamma_pos_5 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_pos2;

/* Define the union u_isp_cc_gamma_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_pos_6 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;          /* [15]  */
        unsigned int isp_cc_gamma_pos_7 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_pos3;

/* Define the union u_isp_cc_gamma_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_inseg_0 : 7; /* [6..0]  */
        unsigned int isp_cc_gamma_inseg_1 : 7; /* [13..7]  */
        unsigned int isp_cc_gamma_inseg_2 : 7; /* [20..14]  */
        unsigned int isp_cc_gamma_inseg_3 : 7; /* [27..21]  */
        unsigned int reserved_0 : 4;           /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_inseg0;

/* Define the union u_isp_cc_gamma_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_inseg_4 : 7; /* [6..0]  */
        unsigned int isp_cc_gamma_inseg_5 : 7; /* [13..7]  */
        unsigned int isp_cc_gamma_inseg_6 : 7; /* [20..14]  */
        unsigned int isp_cc_gamma_inseg_7 : 7; /* [27..21]  */
        unsigned int reserved_0 : 4;           /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_inseg1;

/* Define the union u_isp_cc_gamma_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_step0 : 4; /* [3..0]  */
        unsigned int isp_cc_gamma_step1 : 4; /* [7..4]  */
        unsigned int isp_cc_gamma_step2 : 4; /* [11..8]  */
        unsigned int isp_cc_gamma_step3 : 4; /* [15..12]  */
        unsigned int isp_cc_gamma_step4 : 4; /* [19..16]  */
        unsigned int isp_cc_gamma_step5 : 4; /* [23..20]  */
        unsigned int isp_cc_gamma_step6 : 4; /* [27..24]  */
        unsigned int isp_cc_gamma_step7 : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
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
        unsigned int isp_cc_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_stt2lut_cfg;

/* Define the union u_isp_cc_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;           /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_stt2lut_regnew;

/* Define the union u_isp_cc_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;          /* [30..16]  */
        unsigned int isp_cc_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_stt2lut_abn;

/* Define the union u_isp_cc_dgain_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_lut_waddr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_lut_waddr;

/* Define the union u_isp_cc_dgain_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_lut_wdata : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;             /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_lut_wdata;

/* Define the union u_isp_cc_dgain_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_lut_raddr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_lut_raddr;

/* Define the union u_isp_cc_dgain_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_dgain_lut_rdata : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;             /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_dgain_lut_rdata;

/* Define the union u_isp_cc_gamma_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_lut_waddr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_lut_waddr;

/* Define the union u_isp_cc_gamma_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_lut_wdata : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;             /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_lut_wdata;

/* Define the union u_isp_cc_gamma_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_lut_raddr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_lut_raddr;

/* Define the union u_isp_cc_gamma_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_cc_gamma_lut_rdata : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;             /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_cc_gamma_lut_rdata;

/* Define the union u_isp_csc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_csc_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_version;
/* Define the union u_isp_csc_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;      /* [0]  */
        unsigned int isp_csc_coef00 : 15; /* [15..1]  */
        unsigned int reserved_1 : 1;      /* [16]  */
        unsigned int isp_csc_coef01 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_coef0;

/* Define the union u_isp_csc_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;      /* [0]  */
        unsigned int isp_csc_coef02 : 15; /* [15..1]  */
        unsigned int reserved_1 : 1;      /* [16]  */
        unsigned int isp_csc_coef10 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_coef1;

/* Define the union u_isp_csc_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;      /* [0]  */
        unsigned int isp_csc_coef11 : 15; /* [15..1]  */
        unsigned int reserved_1 : 1;      /* [16]  */
        unsigned int isp_csc_coef12 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_coef2;

/* Define the union u_isp_csc_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;      /* [0]  */
        unsigned int isp_csc_coef20 : 15; /* [15..1]  */
        unsigned int reserved_1 : 1;      /* [16]  */
        unsigned int isp_csc_coef21 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_coef3;

/* Define the union u_isp_csc_coef4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;      /* [0]  */
        unsigned int isp_csc_coef22 : 15; /* [15..1]  */
        unsigned int reserved_1 : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_coef4;

/* Define the union u_isp_csc_in_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5;      /* [4..0]  */
        unsigned int isp_csc_in_dc0 : 11; /* [15..5]  */
        unsigned int reserved_1 : 5;      /* [20..16]  */
        unsigned int isp_csc_in_dc1 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_in_dc0;

/* Define the union u_isp_csc_in_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5;      /* [4..0]  */
        unsigned int isp_csc_in_dc2 : 11; /* [15..5]  */
        unsigned int reserved_1 : 5;      /* [20..16]  */
        unsigned int reserved_2 : 11;     /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_in_dc1;

/* Define the union u_isp_csc_out_dc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5;       /* [4..0]  */
        unsigned int isp_csc_out_dc0 : 11; /* [15..5]  */
        unsigned int reserved_1 : 5;       /* [20..16]  */
        unsigned int isp_csc_out_dc1 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_out_dc0;

/* Define the union u_isp_csc_out_dc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5;       /* [4..0]  */
        unsigned int isp_csc_out_dc2 : 11; /* [15..5]  */
        unsigned int reserved_1 : 5;       /* [20..16]  */
        unsigned int reserved_2 : 11;      /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_csc_out_dc1;

/* Define the union u_isp_hcds_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_hcds_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_hcds_version;
/* Define the union u_isp_hcds_coefh0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_hcds_coefh1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;       /* [15..11]  */
        unsigned int isp_hcds_coefh0 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_hcds_coefh0;

/* Define the union u_isp_hcds_coefh1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_hcds_coefh3 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;       /* [15..11]  */
        unsigned int isp_hcds_coefh2 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_hcds_coefh1;

/* Define the union u_isp_hcds_coefh2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_hcds_coefh5 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;       /* [15..11]  */
        unsigned int isp_hcds_coefh4 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_hcds_coefh2;

/* Define the union u_isp_hcds_coefh3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_hcds_coefh7 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;       /* [15..11]  */
        unsigned int isp_hcds_coefh6 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_hcds_coefh3;

/* Define the union u_isp_4dg_blc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg_en_in : 1;  /* [0]  */
        unsigned int isp_4dg_en_out : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_blc_cfg;

/* Define the union u_isp_4dg_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_4dg_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_version;
/* Define the union u_isp_4dg_0_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg0_grgain : 16; /* [15..0]  */
        unsigned int isp_4dg0_rgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_0_gain1;

/* Define the union u_isp_4dg_0_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg0_gbgain : 16; /* [15..0]  */
        unsigned int isp_4dg0_bgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_0_gain2;

/* Define the union u_isp_4dg_0_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg0_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_4dg0_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_0_blc_offset1;

/* Define the union u_isp_4dg_0_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg0_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_4dg0_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_0_blc_offset2;

/* Define the union u_isp_4dg_1_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg1_grgain : 16; /* [15..0]  */
        unsigned int isp_4dg1_rgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_1_gain1;

/* Define the union u_isp_4dg_1_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg1_gbgain : 16; /* [15..0]  */
        unsigned int isp_4dg1_bgain : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_1_gain2;

/* Define the union u_isp_4dg_1_blc_offset1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg1_ofsgr : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_4dg1_ofsr : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_1_blc_offset1;

/* Define the union u_isp_4dg_1_blc_offset2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg1_ofsgb : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_4dg1_ofsb : 15;  /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_1_blc_offset2;

/* Define the union u_isp_4dg_0_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg0_clip_value : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;          /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_0_clip_value;

/* Define the union u_isp_4dg_1_clip_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_4dg1_clip_value : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;          /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_4dg_1_clip_value;
/* Define the union u_isp_awb_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_version;
/* Define the union u_isp_awb_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_hnum : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;   /* [7..6]  */
        unsigned int isp_awb_vnum : 6; /* [13..8]  */
        unsigned int reserved_1 : 18;  /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_zone;

/* Define the union u_isp_awb_bitmove */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_bitmove : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;     /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_bitmove;

/* Define the union u_isp_awb_thd_min */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_threshold_min : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_thd_min;

/* Define the union u_isp_awb_thd_max */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_threshold_max : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_thd_max;

/* Define the union u_isp_awb_cr_mm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_cr_ref_max : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;          /* [15..12]  */
        unsigned int isp_awb_cr_ref_min : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;          /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_cr_mm;

/* Define the union u_isp_awb_cb_mm */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_cb_ref_max : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;          /* [15..12]  */
        unsigned int isp_awb_cb_ref_min : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;          /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_cb_mm;

/* Define the union u_isp_awb_offset_comp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_offset_comp : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;          /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_offset_comp;

/* Define the union u_isp_awb_avg_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_avg_r : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_avg_r;

/* Define the union u_isp_awb_avg_g */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_avg_g : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_avg_g;

/* Define the union u_isp_awb_avg_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_avg_b : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_avg_b;

/* Define the union u_isp_awb_cnt_all */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_count_all : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_cnt_all;

/* Define the union u_isp_awb_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_stt_size : 16; /* [15..0]  */
        unsigned int isp_awb_stt_bst : 4;   /* [19..16]  */
        unsigned int isp_awb_stt_en : 1;    /* [20]  */
        unsigned int reserved_0 : 11;       /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_stt_bst;

/* Define the union u_isp_awb_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_stt_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;       /* [30..16]  */
        unsigned int isp_awb_stt_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_stt_abn;

/* Define the union u_isp_awb_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_crop_pos_x : 16; /* [15..0]  */
        unsigned int isp_awb_crop_pos_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_crop_pos;

/* Define the union u_isp_awb_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_awb_crop_out_width : 16;  /* [15..0]  */
        unsigned int isp_awb_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_crop_outsize;

/* Define the union u_isp_awb_stat_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_stat_waddr;
/* Define the union u_isp_awb_stat_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_stat_wdata;
/* Define the union u_isp_awb_stat_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_stat_raddr;
/* Define the union u_isp_awb_stat_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_awb_stat_rdata;
/* Define the union u_isp_af_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;         /* [0]  */
        unsigned int isp_af_iir0_en0 : 1;    /* [1]  */
        unsigned int isp_af_iir0_en1 : 1;    /* [2]  */
        unsigned int isp_af_iir0_en2 : 1;    /* [3]  */
        unsigned int isp_af_iir1_en0 : 1;    /* [4]  */
        unsigned int isp_af_iir1_en1 : 1;    /* [5]  */
        unsigned int isp_af_iir1_en2 : 1;    /* [6]  */
        unsigned int isp_af_peak_mode : 1;   /* [7]  */
        unsigned int isp_af_squ_mode : 1;    /* [8]  */
        unsigned int isp_af_offset_en : 1;   /* [9]  */
        unsigned int isp_af_crop_en : 1;     /* [10]  */
        unsigned int isp_af_lpf_en : 1;      /* [11]  */
        unsigned int isp_af_mean_en : 1;     /* [12]  */
        unsigned int reserved_1 : 1;         /* [13]  */
        unsigned int isp_af_raw_mode : 1;    /* [14]  */
        unsigned int isp_af_bayer_mode : 2;  /* [16..15]  */
        unsigned int isp_af_iir0_ds_en : 1;  /* [17]  */
        unsigned int isp_af_iir1_ds_en : 1;  /* [18]  */
        unsigned int isp_af_fir0_lpf_en : 1; /* [19]  */
        unsigned int isp_af_fir1_lpf_en : 1; /* [20]  */
        unsigned int isp_af_iir0_ldg_en : 1; /* [21]  */
        unsigned int isp_af_iir1_ldg_en : 1; /* [22]  */
        unsigned int isp_af_fir0_ldg_en : 1; /* [23]  */
        unsigned int isp_af_fir1_ldg_en : 1; /* [24]  */
        unsigned int isp_af_gamma : 1;       /* [25]  */
        unsigned int reserved_2 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_cfg;

/* Define the union u_isp_af_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_version;
/* Define the union u_isp_af_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_hnum : 5; /* [4..0]  */
        unsigned int reserved_0 : 3;  /* [7..5]  */
        unsigned int isp_af_vnum : 5; /* [12..8]  */
        unsigned int reserved_1 : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_zone;

/* Define the union u_isp_af_crop_start */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_pos_x : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int isp_af_pos_y : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;    /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_crop_start;

/* Define the union u_isp_af_crop_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_crop_hsize : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;         /* [15..13]  */
        unsigned int isp_af_crop_vsize : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;         /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_crop_size;

/* Define the union u_isp_af_mean_thres */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_mean_thres : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_mean_thres;

/* Define the union u_isp_af_iirg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg0_0 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;     /* [15..8]  */
        unsigned int isp_af_iirg0_1 : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;     /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg0;

/* Define the union u_isp_af_iirg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg1_0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int isp_af_iirg1_1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg1;

/* Define the union u_isp_af_iirg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg2_0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int isp_af_iirg2_1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg2;

/* Define the union u_isp_af_iirg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg3_0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int isp_af_iirg3_1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg3;

/* Define the union u_isp_af_iirg4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg4_0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int isp_af_iirg4_1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg4;

/* Define the union u_isp_af_iirg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg5_0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int isp_af_iirg5_1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg5;

/* Define the union u_isp_af_iirg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirg6_0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int isp_af_iirg6_1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirg6;

/* Define the union u_isp_af_iirpl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirplg_0 : 8; /* [7..0]  */
        unsigned int isp_af_iirpls_0 : 3; /* [10..8]  */
        unsigned int reserved_0 : 5;      /* [15..11]  */
        unsigned int isp_af_iirplg_1 : 8; /* [23..16]  */
        unsigned int isp_af_iirpls_1 : 3; /* [26..24]  */
        unsigned int reserved_1 : 5;      /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirpl;

/* Define the union u_isp_af_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iirshift0_0 : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;         /* [3]  */
        unsigned int isp_af_iirshift0_1 : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;         /* [7]  */
        unsigned int isp_af_iirshift0_2 : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;         /* [11]  */
        unsigned int isp_af_iirshift0_3 : 3; /* [14..12]  */
        unsigned int reserved_3 : 1;         /* [15]  */
        unsigned int isp_af_iirshift1_0 : 3; /* [18..16]  */
        unsigned int reserved_4 : 1;         /* [19]  */
        unsigned int isp_af_iirshift1_1 : 3; /* [22..20]  */
        unsigned int reserved_5 : 1;         /* [23]  */
        unsigned int isp_af_iirshift1_2 : 3; /* [26..24]  */
        unsigned int reserved_6 : 1;         /* [27]  */
        unsigned int isp_af_iirshift1_3 : 3; /* [30..28]  */
        unsigned int reserved_7 : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_shift;

/* Define the union u_isp_af_firh0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_firh0_0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;    /* [15..6]  */
        unsigned int isp_af_firh0_1 : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firh0;

/* Define the union u_isp_af_firh1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_firh1_0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;    /* [15..6]  */
        unsigned int isp_af_firh1_1 : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firh1;

/* Define the union u_isp_af_firh2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_firh2_0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;    /* [15..6]  */
        unsigned int isp_af_firh2_1 : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firh2;

/* Define the union u_isp_af_firh3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_firh3_0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;    /* [15..6]  */
        unsigned int isp_af_firh3_1 : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firh3;

/* Define the union u_isp_af_firh4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_firh4_0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;    /* [15..6]  */
        unsigned int isp_af_firh4_1 : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firh4;

/* Define the union u_isp_af_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_stt_size : 16; /* [15..0]  */
        unsigned int isp_af_stt_bst : 4;   /* [19..16]  */
        unsigned int isp_af_stt_en : 1;    /* [20]  */
        unsigned int reserved_0 : 11;      /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_stt_bst;

/* Define the union u_isp_af_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_stt_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;      /* [30..16]  */
        unsigned int isp_af_stt_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_stt_abn;

/* Define the union u_isp_af_acc_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_acc_shift0_h : 4; /* [3..0]  */
        unsigned int isp_af_acc_shift1_h : 4; /* [7..4]  */
        unsigned int isp_af_acc_shift0_v : 4; /* [11..8]  */
        unsigned int isp_af_acc_shift1_v : 4; /* [15..12]  */
        unsigned int isp_af_acc_shift_y : 4;  /* [19..16]  */
        unsigned int reserved_0 : 12;         /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_acc_shift;

/* Define the union u_isp_af_cnt_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_cnt_shift0_h : 4; /* [3..0]  */
        unsigned int isp_af_cnt_shift1_h : 4; /* [7..4]  */
        unsigned int isp_af_cnt_shift0_v : 4; /* [11..8]  */
        unsigned int isp_af_cnt_shift1_v : 4; /* [15..12]  */
        unsigned int isp_af_cnt_shift_y : 4;  /* [19..16]  */
        unsigned int reserved_0 : 12;         /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_cnt_shift;

/* Define the union u_isp_af_stat_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_stat_raddr;
/* Define the union u_isp_af_stat_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_stat_rdata;
/* Define the union u_isp_af_iirthre */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_thre0_l : 8; /* [7..0]  */
        unsigned int isp_af_iir_thre0_h : 8; /* [15..8]  */
        unsigned int isp_af_iir_thre1_l : 8; /* [23..16]  */
        unsigned int isp_af_iir_thre1_h : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirthre;

/* Define the union u_isp_af_iirgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_gain0_l : 8; /* [7..0]  */
        unsigned int isp_af_iir_gain0_h : 8; /* [15..8]  */
        unsigned int isp_af_iir_gain1_l : 8; /* [23..16]  */
        unsigned int isp_af_iir_gain1_h : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirgain;

/* Define the union u_isp_af_iirslope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_slope0_l : 4; /* [3..0]  */
        unsigned int isp_af_iir_slope0_h : 4; /* [7..4]  */
        unsigned int reserved_0 : 8;          /* [15..8]  */
        unsigned int isp_af_iir_slope1_l : 4; /* [19..16]  */
        unsigned int isp_af_iir_slope1_h : 4; /* [23..20]  */
        unsigned int reserved_1 : 8;          /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirslope;

/* Define the union u_isp_af_iirdilate */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_dilate0 : 3; /* [2..0]  */
        unsigned int reserved_0 : 5;         /* [7..3]  */
        unsigned int isp_af_iir_dilate1 : 3; /* [10..8]  */
        unsigned int reserved_1 : 21;        /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirdilate;

/* Define the union u_isp_af_firthre */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_fir_thre0_l : 8; /* [7..0]  */
        unsigned int isp_af_fir_thre0_h : 8; /* [15..8]  */
        unsigned int isp_af_fir_thre1_l : 8; /* [23..16]  */
        unsigned int isp_af_fir_thre1_h : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firthre;

/* Define the union u_isp_af_firgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_fir_gain0_l : 8; /* [7..0]  */
        unsigned int isp_af_fir_gain0_h : 8; /* [15..8]  */
        unsigned int isp_af_fir_gain1_l : 8; /* [23..16]  */
        unsigned int isp_af_fir_gain1_h : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firgain;

/* Define the union u_isp_af_firslope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_fir_slope0_l : 4; /* [3..0]  */
        unsigned int isp_af_fir_slope0_h : 4; /* [7..4]  */
        unsigned int reserved_0 : 8;          /* [15..8]  */
        unsigned int isp_af_fir_slope1_l : 4; /* [19..16]  */
        unsigned int isp_af_fir_slope1_h : 4; /* [23..20]  */
        unsigned int reserved_1 : 8;          /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firslope;

/* Define the union u_isp_af_iirthre_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_thre0_c : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;          /* [15..11]  */
        unsigned int isp_af_iir_thre1_c : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;          /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirthre_coring;

/* Define the union u_isp_af_iirpeak_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_peak0_c : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;          /* [15..11]  */
        unsigned int isp_af_iir_peak1_c : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;          /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirpeak_coring;

/* Define the union u_isp_af_iirslope_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_iir_slope0_c : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;          /* [7..4]  */
        unsigned int isp_af_iir_slope1_c : 4; /* [11..8]  */
        unsigned int reserved_1 : 20;         /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_iirslope_coring;

/* Define the union u_isp_af_firthre_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_fir_thre0_c : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;          /* [15..11]  */
        unsigned int isp_af_fir_thre1_c : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;          /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firthre_coring;

/* Define the union u_isp_af_firpeak_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_fir_peak0_c : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;          /* [15..11]  */
        unsigned int isp_af_fir_peak1_c : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;          /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firpeak_coring;

/* Define the union u_isp_af_firslope_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_fir_slope0_c : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;          /* [7..4]  */
        unsigned int isp_af_fir_slope1_c : 4; /* [11..8]  */
        unsigned int reserved_1 : 20;         /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_firslope_coring;

/* Define the union u_isp_af_hilight */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_hilight : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;    /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_hilight;

/* Define the union u_isp_af_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_af_offset_gr : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;        /* [15..14]  */
        unsigned int isp_af_offset_gb : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;        /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_af_offset;

/* Define the union u_isp_dis_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;     /* [0]  */
        unsigned int isp_dis_blc_en : 1; /* [1]  */
        unsigned int reserved_1 : 30;    /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_cfg;

/* Define the union u_isp_dis_blk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_blk_size_0 : 1;   /* [0]  */
        unsigned int isp_dis_blk_size : 8;     /* [8..1]  */
        unsigned int reserved_0 : 7;           /* [15..9]  */
        unsigned int isp_dis_srch_range_0 : 1; /* [16]  */
        unsigned int isp_dis_srch_range : 7;   /* [23..17]  */
        unsigned int reserved_1 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_blk;

/* Define the union u_isp_dis_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_version;
/* Define the union u_isp_dis_v0pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_v0pos_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_dis_v0pos_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_v0pos;

/* Define the union u_isp_dis_v4pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_v4pos_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_dis_v4pos_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_v4pos;

/* Define the union u_isp_dis_v8pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_v8pos_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_dis_v8pos_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_v8pos;

/* Define the union u_isp_dis_v0pose */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_v0pose_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_dis_v0pose_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_v0pose;

/* Define the union u_isp_dis_v4pose */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_v4pose_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_dis_v4pose_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_v4pose;

/* Define the union u_isp_dis_v8pose */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_v8pose_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_dis_v8pose_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_v8pose;

/* Define the union u_isp_dis_h0pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h0pos_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_dis_h0pos_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_h0pos;

/* Define the union u_isp_dis_h4pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h4pos_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_dis_h4pos_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_h4pos;

/* Define the union u_isp_dis_h8pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h8pos_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_dis_h8pos_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_h8pos;

/* Define the union u_isp_dis_h0pose */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h0pose_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_dis_h0pose_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_h0pose;

/* Define the union u_isp_dis_h4pose */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h4pose_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_dis_h4pose_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_h4pose;

/* Define the union u_isp_dis_h8pose */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h8pose_hor : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_dis_h8pose_ver : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_h8pose;

/* Define the union u_isp_dis_raw_luma */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_raw_luma : 1; /* [0]  */
        unsigned int reserved_0 : 31;      /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_raw_luma;

/* Define the union u_isp_dis_gamma_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_gamma_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;      /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_gamma_en;

/* Define the union u_isp_dis_offset_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_offset_gr : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_offset_gr;

/* Define the union u_isp_dis_offset_gb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_offset_gb : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_offset_gb;

/* Define the union u_isp_dis_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_hor_scale : 2; /* [1..0]  */
        unsigned int reserved_0 : 6;        /* [7..2]  */
        unsigned int isp_dis_ver_scale : 2; /* [9..8]  */
        unsigned int reserved_1 : 22;       /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_scale;

/* Define the union u_isp_dis_refinfo_update */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_refinfo_update : 1; /* [0]  */
        unsigned int reserved_0 : 31;            /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_refinfo_update;

/* Define the union u_isp_dis_prj_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h_prj_shift_bits : 4; /* [3..0]  */
        unsigned int isp_dis_v_prj_shift_bits : 4; /* [7..4]  */
        unsigned int reserved_0 : 24;              /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_prj_shift;

/* Define the union u_isp_dis_stat_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_stat_waddr;
/* Define the union u_isp_dis_stat_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_stat_wdata;
/* Define the union u_isp_dis_stat_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_stat_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_stat_raddr;
/* Define the union u_isp_dis_stat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dis_h_stat_rdata : 16; /* [15..0]  */
        unsigned int isp_dis_v_stat_rdata : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_stat_rdata;

/* Define the union u_isp_dis_refinfo_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_refinfo_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_refinfo_waddr;
/* Define the union u_isp_dis_refinfo_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_refinfo_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_refinfo_wdata;
/* Define the union u_isp_dis_refinfo_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_refinfo_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_refinfo_raddr;
/* Define the union u_isp_dis_refinfo_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_refinfo_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dis_refinfo_rdata;
/* Define the union u_isp_la_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_version;
/* Define the union u_isp_la_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_hnum : 5; /* [4..0]  */
        unsigned int reserved_0 : 3;  /* [7..5]  */
        unsigned int isp_la_vnum : 5; /* [12..8]  */
        unsigned int reserved_1 : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_zone;

/* Define the union u_isp_la_bitmove */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_bitmove : 5;  /* [4..0]  */
        unsigned int reserved_0 : 3;      /* [7..5]  */
        unsigned int isp_la_gamma_en : 1; /* [8]  */
        unsigned int isp_la_blc_en : 1;   /* [9]  */
        unsigned int reserved_1 : 22;     /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_bitmove;

/* Define the union u_isp_la_offset_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_offset_r : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;      /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_offset_r;

/* Define the union u_isp_la_offset_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_offset_gr : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;       /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_offset_gr;

/* Define the union u_isp_la_offset_gb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_offset_gb : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;       /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_offset_gb;

/* Define the union u_isp_la_offset_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_offset_b : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;      /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_offset_b;

/* Define the union u_isp_la_gamma_limit */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_gamma_limit : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;        /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_gamma_limit;

/* Define the union u_isp_la_crop_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_crop_pos_x : 16; /* [15..0]  */
        unsigned int isp_la_crop_pos_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_crop_pos;

/* Define the union u_isp_la_crop_outsize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_crop_out_width : 16;  /* [15..0]  */
        unsigned int isp_la_crop_out_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_crop_outsize;

/* Define the union u_isp_la_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_stt_size : 16; /* [15..0]  */
        unsigned int isp_la_stt_bst : 4;   /* [19..16]  */
        unsigned int isp_la_stt_en : 1;    /* [20]  */
        unsigned int reserved_0 : 11;      /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_stt_bst;

/* Define the union u_isp_la_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_la_stt_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;      /* [30..16]  */
        unsigned int isp_la_stt_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_stt_abn;

/* Define the union u_isp_la_aver_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_aver_waddr;
/* Define the union u_isp_la_aver_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_aver_wdata;
/* Define the union u_isp_la_aver_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_aver_raddr;
/* Define the union u_isp_la_aver_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_la_aver_rdata;
/* Define the union u_isp_lsc_mesh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_lsc_mesh_str : 10;  /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int isp_lsc_mesh_scale : 3; /* [18..16]  */
        unsigned int reserved_1 : 13;        /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_lsc_mesh;

/* Define the union u_isp_lsc_blcen */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_lsc_blc_in_en : 1;  /* [0]  */
        unsigned int reserved_0 : 7;         /* [7..1]  */
        unsigned int isp_lsc_blc_out_en : 1; /* [8]  */
        unsigned int reserved_1 : 23;        /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
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
    unsigned int u32;
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
    unsigned int u32;
} u_isp_lsc_blc1;
/* Define the union u_isp_lsc_right_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_right_shift : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_lsc_right_shift;

/* Define the union u_isp_ge_blc_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge0_blc_offset_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;           /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge_blc_en;

/* Define the union u_isp_ge_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge_version;
/* Define the union u_isp_ge0_ct_th1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge_ge0_ct_th1 : 12; /* [15..0]  */
        unsigned int reserved_0 : 20;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge0_ct_th1;

/* Define the union u_isp_ge0_ct_th2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge_ge0_ct_th2 : 12; /* [15..0]  */
        unsigned int reserved_0 : 20;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge0_ct_th2;

/* Define the union u_isp_ge0_ct_th3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge_ge0_ct_th3 : 12; /* [15..0]  */
        unsigned int reserved_0 : 20;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge0_ct_th3;

/* Define the union u_isp_ge0_ct_slope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge_ge0_ct_slope1 : 5; /* [4..0]  */
        unsigned int reserved_0 : 3;           /* [7..5]  */
        unsigned int isp_ge_ge0_ct_slope2 : 5; /* [12..8]  */
        unsigned int reserved_1 : 19;          /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge0_ct_slope;

/* Define the union u_isp_ge0_blc_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge0_blc_offset_gb : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;             /* [15..12]  */
        unsigned int isp_ge0_blc_offset_gr : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;             /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge0_blc_offset;

/* Define the union u_isp_ge_strength */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ge_strength : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;     /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ge_strength;

/* Define the union u_isp_fpn_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10;           /* [9..0]  */
        unsigned int isp_fpn_calib_offline : 1; /* [10]  */
        unsigned int reserved_1 : 21;           /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_cfg;

/* Define the union u_isp_fpn_calib_start */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_calib_start : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_calib_start;

/* Define the union u_isp_fpn_corr_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_correct0_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_corr_cfg;

/* Define the union u_isp_fpn_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_busy : 1;  /* [0]  */
        unsigned int reserved_0 : 7;    /* [7..1]  */
        unsigned int isp_fpn_vcnt : 6;  /* [13..8]  */
        unsigned int reserved_1 : 2;    /* [15..14]  */
        unsigned int isp_fpn_hcnt : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_stat;

/* Define the union u_isp_fpn_white_level */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_white_level : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;          /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_white_level;

/* Define the union u_isp_fpn_divcoef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_divcoef : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;      /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_divcoef;

/* Define the union u_isp_fpn_framelog2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_framelog2 : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;       /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_framelog2;

/* Define the union u_isp_fpn_sum0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_sum0;
/* Define the union u_isp_fpn_sum1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum1 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_sum1;
/* Define the union u_isp_fpn_corr0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_offset0 : 12;   /* [11..0]  */
        unsigned int reserved_0 : 4;         /* [15..12]  */
        unsigned int isp_fpn_strength0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_corr0;

/* Define the union u_isp_fpn_shift */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_shift : 4;             /* [3..0]  */
        unsigned int reserved_0 : 4;                /* [7..4]  */
        unsigned int isp_fpn_in_shift : 4;          /* [11..8]  */
        unsigned int reserved_1 : 4;                /* [15..12]  */
        unsigned int isp_fpn_out_shift : 4;         /* [19..16]  */
        unsigned int reserved_2 : 4;                /* [23..20]  */
        unsigned int isp_fpn_frame_calib_shift : 4; /* [27..24]  */
        unsigned int reserved_3 : 4;                /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_shift;

/* Define the union u_isp_fpn_max_o */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_max_o : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_max_o;

/* Define the union u_isp_fpn_overflowthr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_overflowthr : 14; /* [13..0]  */
        unsigned int reserved_0 : 18;          /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_overflowthr;

/* Define the union u_isp_fpn_black_frame_out_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_data0_out_sel : 1; /* [0]  */
        unsigned int reserved_0 : 31;           /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_black_frame_out_sel;

/* Define the union u_isp_fpn_overlap_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_overlap_width_l : 16; /* [15..0]  */
        unsigned int isp_fpn_overlap_width_r : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_overlap_cfg;

/* Define the union u_isp_fpn_calib_offline_set */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_calib_offline_set : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;               /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_calib_offline_set;

/* Define the union u_isp_fpn_calib_stat_clear */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_fpn_calib_stat_clear : 1; /* [0]  */
        unsigned int reserved_0 : 31;              /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_fpn_calib_stat_clear;

/* Define the union u_isp_dpc_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_alpha1_g : 8;  /* [7..0]  */
        unsigned int isp_dpc_alpha1_rb : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_alpha;

/* Define the union u_isp_dpc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_version;
/* Define the union u_isp_dpc_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_det_en : 1;         /* [0]  */
        unsigned int isp_dpc_cor_en : 1;         /* [1]  */
        unsigned int reserved_0 : 2;             /* [3..2]  */
        unsigned int isp_dpc_grayscale_mode : 1; /* [4]  */
        unsigned int reserved_1 : 3;             /* [7..5]  */
        unsigned int isp_dpc_ir_position : 1;    /* [8]  */
        unsigned int isp_dpc_ir_channel : 1;     /* [9]  */
        unsigned int reserved_2 : 1;             /* [10]  */
        unsigned int isp_dpc_quadref_en : 1;     /* [11]  */
        unsigned int reserved_3 : 20;            /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_mode;

/* Define the union u_isp_dpc_set_use */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_stage1_use_set1 : 1;    /* [0]  */
        unsigned int reserved_0 : 2;                 /* [2..1]  */
        unsigned int isp_dpc_stage1_use_fix_set : 1; /* [3]  */
        unsigned int reserved_1 : 28;                /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_set_use;

/* Define the union u_isp_dpc_methods_set_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_pg_enable : 1; /* [0]  */
        unsigned int isp_dpc_lc_enable : 1; /* [1]  */
        unsigned int isp_dpc_ro_enable : 1; /* [2]  */
        unsigned int isp_dpc_mid_en : 1;    /* [3]  */
        unsigned int isp_dpc_rg_enable : 1; /* [4]  */
        unsigned int reserved_0 : 27;       /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_methods_set_1;

/* Define the union u_isp_dpc_line_thresh_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_line_thr1_g : 8;  /* [7..0]  */
        unsigned int isp_dpc_line_thr1_rb : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;          /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_line_thresh_1;

/* Define the union u_isp_dpc_line_mad_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_line_mad_fac_1_g : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2;                /* [7..6]  */
        unsigned int isp_dpc_line_mad_fac_1_rb : 6; /* [13..8]  */
        unsigned int reserved_1 : 18;               /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_line_mad_fac_1;

/* Define the union u_isp_dpc_pg_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_pg_fac_1_g : 8;  /* [7..0]  */
        unsigned int isp_dpc_pg_fac_1_rb : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;         /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_pg_fac_1;

/* Define the union u_isp_dpc_rg_fac_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_rg_fac_1_g : 8;  /* [7..0]  */
        unsigned int isp_dpc_rg_fac_1_rb : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;         /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_rg_fac_1;

/* Define the union u_isp_dpc_ro_limits */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_ro_1_g : 2;  /* [1..0]  */
        unsigned int isp_dpc_ro_1_rb : 2; /* [3..2]  */
        unsigned int reserved_0 : 28;     /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_ro_limits;

/* Define the union u_isp_dpc_bhardthr_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_ex_hard_thr_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;            /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_bhardthr_en;

/* Define the union u_isp_dpc_lch_thr_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_line_aver_fac_1 : 8; /* [7..0]  */
        unsigned int isp_dpc_line_diff_thr_1 : 7; /* [14..8]  */
        unsigned int reserved_0 : 1;              /* [15]  */
        unsigned int isp_dpc_line_std_thr_1 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;              /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_lch_thr_1;

/* Define the union u_isp_dpc_rg_fac_1_mtp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_rg_fac_1_g_mtp : 10;  /* [9..0]  */
        unsigned int isp_dpc_rg_fac_1_rb_mtp : 10; /* [19..10]  */
        unsigned int reserved_0 : 12;              /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_rg_fac_1_mtp;

/* Define the union u_isp_dpc_amp_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_amp_coef_k : 8;   /* [7..0]  */
        unsigned int reserved_0 : 4;           /* [11..8]  */
        unsigned int isp_dpc_amp_coef_min : 7; /* [18..12]  */
        unsigned int reserved_1 : 13;          /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_amp_coef;

/* Define the union u_isp_dpc_soft_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_soft_thr_min : 8; /* [7..0]  */
        unsigned int isp_dpc_soft_thr_max : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;          /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_soft_thr;

/* Define the union u_isp_dpc_rakeratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_ex_rake_ratio : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_rakeratio;

/* Define the union u_isp_dpc_dark_enhance */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_dark_enhance : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;          /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_dark_enhance;

/* Define the union u_isp_dpc_line_kerdiff_fac */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dpc_line_kerdiff_fac : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;              /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dpc_line_kerdiff_fac;


/* Define the union u_isp_demosaic_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_inner_bypass_en : 1; /* [0]  */
        unsigned int    isp_demosaic_ahd_en   : 1;        /* [1]  */
        unsigned int    isp_demosaic_nddm_mode : 1;       /* [2]  */
        unsigned int    reserved_0            : 13;        /* [15..3]  */
        unsigned int    isp_demosaic_gcac_en  : 1;        /* [16]  */
        unsigned int    isp_demosaic_lcac_en  : 1;        /* [17]  */
        unsigned int    reserved_1            : 1;        /* [18]  */
        unsigned int    isp_demosaic_fcr_en   : 1;        /* [19]  */
        unsigned int    reserved_2            : 1;        /* [20]  */
        unsigned int    isp_demosaic_desat_en : 1;        /* [21]  */
        unsigned int    reserved_3            : 2;        /* [23..22]  */
        unsigned int    reserved_4            : 1;        /* [24]  */
        unsigned int    reserved_5            : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_cfg1;

/* Define the union u_isp_demosaic_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_bld_limit2 : 8; /* [7..0]  */
        unsigned int    isp_demosaic_bld_limit1 : 8; /* [15..8]  */
        unsigned int    isp_demosaic_grad_limit2 : 8; /* [23..16]  */
        unsigned int    isp_demosaic_grad_limit1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef0;

/* Define the union u_isp_demosaic_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_fcr_limit2 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;               /* [15..12]  */
        unsigned int isp_demosaic_fcr_limit1 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;               /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef2;

/* Define the union u_isp_demosaic_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_ahd_par2 : 8;  /* [7..0]  */
        unsigned int reserved_0 : 8;             /* [15..8]  */
        unsigned int isp_demosaic_ahd_par1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;             /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef3;

/* Define the union u_isp_demosaic_coef4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_gain : 5;        /* [4..0]  */
        unsigned int    reserved_0            : 3;        /* [7..5]  */
        unsigned int    isp_demosaic_var_thr_for_cac : 10; /* [17..8]  */
        unsigned int    reserved_1            : 2;        /* [19..18]  */
        unsigned int    isp_demosaic_var_thr_for_ahd : 10; /* [29..20]  */
        unsigned int    reserved_2            : 2;        /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef4;

/* Define the union u_isp_demosaic_coef5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hv_detg_ratio : 8; /* [7..0]  */
        unsigned int    isp_demosaic_hv_color_ratio : 8; /* [15..8]  */
        unsigned int    isp_demosaic_wgd_limit2 : 8; /* [23..16]  */
        unsigned int    isp_demosaic_wgd_limit1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef5;

/* Define the union u_isp_demosaic_coef6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_fcr_ratio : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;            /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef6;

/* Define the union u_isp_demosaic_coef7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_fcr_remove : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;              /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_coef7;

/* Define the union u_isp_demosaic_lcac_cnt_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_r_counter_thr : 5; /* [4..0]  */
        unsigned int isp_demosaic_b_counter_thr : 5; /* [9..5]  */
        unsigned int isp_demosaic_g_counter_thr : 6; /* [15..10]  */
        unsigned int reserved_0 : 16;                /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_cnt_thr;

/* Define the union u_isp_demosaic_lcac_luma_rb_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_r_luma_thr : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;               /* [15..12]  */
        unsigned int isp_demosaic_b_luma_thr : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;               /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_luma_rb_thr;

/* Define the union u_isp_demosaic_lcac_luma_g_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_g_luma_thr : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;              /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_luma_g_thr;

/* Define the union u_isp_demosaic_lcac_ratio_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_lcac_ratio_thd : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;                  /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_ratio_thd;

/* Define the union u_isp_demosaic_lcac_ratio_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_lcac_ratio_sft : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;                 /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_ratio_sft;

/* Define the union u_isp_demosaic_lcac_g_avg_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_lcac_g_avg_thd : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;                  /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_g_avg_thd;

/* Define the union u_isp_demosaic_lcac_g_avg_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_lcac_g_avg_sft : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;                 /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_lcac_g_avg_sft;

/* Define the union u_isp_demosaic_plat_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_plat_thd : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;            /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_plat_thd;

/* Define the union u_isp_demosaic_over_exp_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_over_exp_thd : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;               /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_over_exp_thd;

/* Define the union u_isp_demosaic_b_diff_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_b_diff_sft : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;             /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_b_diff_sft;

/* Define the union u_isp_demosaic_mix_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_mix_ratio : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;            /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_mix_ratio;

/* Define the union u_isp_demosaic_depurplectr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_depurplectrcb : 4; /* [3..0]  */
        unsigned int reserved_0 : 12;                /* [15..4]  */
        unsigned int isp_demosaic_depurplectrcr : 4; /* [19..16]  */
        unsigned int reserved_1 : 12;                /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_depurplectr;

/* Define the union u_isp_demosaic_g_intp_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_g_clip_sft_bit : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;                 /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_g_intp_ctrl;

/* Define the union u_isp_demosaic_cc_hf_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_cc_hf_min_ratio : 5; /* [4..0]  */
        unsigned int reserved_0 : 3;                   /* [7..5]  */
        unsigned int isp_demosaic_cc_hf_max_ratio : 5; /* [12..8]  */
        unsigned int reserved_1 : 19;                  /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_cc_hf_ratio;

/* Define the union u_isp_demosaic_satu_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_satu_thr : 8; /* [7..0]  */
        unsigned int    reserved_0            : 8; /* [15..8]  */
        unsigned int    isp_demosaic_g_var_thr_for_purple : 12; /* [27..16]  */
        unsigned int    reserved_1            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_satu_thr;

/* Define the union u_isp_demosaic_cbcr_ratio_limit */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_cbcr_ratio_low_limit : 10;  /* [9..0]  */
        unsigned int reserved_0 : 6;                          /* [15..11]  */
        unsigned int isp_demosaic_cbcr_ratio_high_limit : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;                          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_cbcr_ratio_limit;

/* Define the union u_isp_demosaic_fcr_gray_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_fcr_gray_ratio : 5; /* [4..0]  */
        unsigned int reserved_0 : 27;                 /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_fcr_gray_ratio;

/* Define the union u_isp_demosaic_cx_var_rate */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_cx_var_min_rate : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;                   /* [7..4]  */
        unsigned int isp_demosaic_cx_var_max_rate : 4; /* [11..8]  */
        unsigned int reserved_1 : 20;                  /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_cx_var_rate;

/* Define the union u_isp_demosaic_gcac_diffthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_out_diffthd : 12; /* [11..0]  */
        unsigned int reserved_0 : 8;                     /* [19..12]  */
        unsigned int isp_demosaic_gcac_desatuthr : 12;   /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_diffthd;

/* Define the union u_isp_demosaic_gcac_lamda_reg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_edgemode : 2;    /* [1..0]  */
        unsigned int isp_demosaic_gcac_globalstr : 10;  /* [11..2]  */
        unsigned int isp_demosaic_gcac_normfactor : 4;  /* [15..12]  */
        unsigned int isp_demosaic_gcac_upperlmtsft : 3; /* [18..16]  */
        unsigned int isp_demosaic_gcac_lowerlmtsft : 3; /* [21..19]  */
        unsigned int reserved_0 : 10;                   /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_lamda_reg;

/* Define the union u_isp_demosaic_gcac_lamdathd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_lamdathd1 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                   /* [15..12]  */
        unsigned int isp_demosaic_gcac_lamdathd2 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;                   /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_lamdathd;

/* Define the union u_isp_demosaic_gcac_thrbthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_thrbthd1 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                  /* [15..12]  */
        unsigned int isp_demosaic_gcac_thrbthd2 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;                  /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_thrbthd;

/* Define the union u_isp_demosaic_gcac_lamdamul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_thrbmul : 16;  /* [15..0]  */
        unsigned int isp_demosaic_gcac_lamdamul : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_lamdamul;

/* Define the union u_isp_demosaic_gcac_distance_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_distance_thd1 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                       /* [15..12]  */
        unsigned int isp_demosaic_gcac_distance_thd2 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;                       /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_distance_thd;

/* Define the union u_isp_demosaic_gcac_distance_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_distance_mul : 16; /* [15..0]  */
        unsigned int isp_demosaic_gcac_tao : 5;           /* [20..16]  */
        unsigned int isp_demosaic_gcac_purplealpha : 6;   /* [26..21]  */
        unsigned int reserved_0 : 5;                      /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_distance_mul;

/* Define the union u_isp_demosaic_gcac_edge_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_gcac_edge_alpha : 6;     /* [5..0]  */
        unsigned int isp_demosaic_gcac_desatuthr_hig : 14; /* [19..6]  */
        unsigned int isp_demosaic_gcac_rb_diff_thd : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_gcac_edge_alpha;

/* Define the union u_isp_demosaic_filter_blur_str_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_filter_f3 : 8; /* [7..0]  */
        unsigned int reserved_0 : 2;             /* [9..8]  */
        unsigned int isp_demosaic_filter_f2 : 6; /* [15..10]  */
        unsigned int reserved_1 : 5;             /* [20..16]  */
        unsigned int isp_demosaic_filter_f1 : 5; /* [25..21]  */
        unsigned int reserved_2 : 3;             /* [28..26]  */
        unsigned int isp_demosaic_filter_f0 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_filter_blur_str_1;


/* Define the union u_isp_demosaic_filter_blur_str_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_filter_blur1 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;                      /* [15..9]  */
        unsigned int isp_demosaic_filter_blur2 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;                      /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_filter_blur_str_2;

/* Define the union u_isp_demosaic_fcr_hf_thresh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_fcr_hfthresh1 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                  /* [15..10]  */
        unsigned int isp_demosaic_fcr_hfthresh2 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;                  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_fcr_hf_thresh;

/* Define the union u_isp_demosaic_nddm_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_nddm_str : 11;      /* [10..0]  */
        unsigned int isp_demosaic_nddm_eps_sft : 3;  /* [13..11]  */
        unsigned int reserved_0            : 2;      /* [15..14]  */
        unsigned int isp_demosaic_nddm_ehcgray : 7;     /* [22..16]  */
        unsigned int reserved_1 : 9;                    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_nddm_coef;

/* Define the union u_isp_demosaic_desat_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_desat_thresh1 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                  /* [15..10]  */
        unsigned int isp_demosaic_desat_thresh2 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;                  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_desat_thr;

/* Define the union u_isp_demosaic_desat_bldr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_desat_hig : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;             /* [15..9]  */
        unsigned int isp_demosaic_desat_low : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;             /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_desat_bldr;

/* Define the union u_isp_demosaic_desat_bldr_ratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_desat_ratio : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;               /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_desat_bldr_ratio;

/* Define the union u_isp_demosaic_desat_protect */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_demosaic_desat_protect_sl : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                     /* [15..10]  */
        unsigned int isp_demosaic_desat_protect_th : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;                     /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_demosaic_desat_protect;

/* Define the union u_isp_rgbir_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_blc_in_en : 1;     /* [0]  */
        unsigned int isp_rgbir_blc_out_en : 1;    /* [1]  */
        unsigned int isp_rgbir_ir_pattern_in : 3; /* [4..2]  */
        unsigned int isp_rgbir_pattern_out : 2;   /* [6..5]  */
        unsigned int reserved_0 : 25;             /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cfg;

/* Define the union u_isp_rgbir_thre */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_th : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;   /* [15..8]  */
        unsigned int isp_rgbir_tv : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;   /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_thre;

/* Define the union u_isp_rgbir_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rgbir_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_version;
/* Define the union u_isp_rgbir_exp_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_exp_thr1 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;          /* [15..12]  */
        unsigned int isp_rgbir_exp_thr2 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;          /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_exp_thr;

/* Define the union u_isp_rgbir_reci_ctl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_reci_ctl1 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;          /* [15..9]  */
        unsigned int isp_rgbir_reci_ctl2 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;          /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_reci_ctl;

/* Define the union u_isp_rgbir_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_gain_r : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;        /* [15..12]  */
        unsigned int isp_rgbir_gain_b : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;        /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_gain;

/* Define the union u_isp_rgbir_cvt01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_matrix0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;         /* [15..11]  */
        unsigned int isp_rgbir_matrix1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;         /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cvt01;

/* Define the union u_isp_rgbir_cvt23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_matrix2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;         /* [15..11]  */
        unsigned int isp_rgbir_matrix3 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;         /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cvt23;

/* Define the union u_isp_rgbir_cvt45 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_matrix4 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;         /* [15..11]  */
        unsigned int isp_rgbir_matrix5 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;         /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cvt45;

/* Define the union u_isp_rgbir_cvt67 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_matrix6 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;         /* [15..11]  */
        unsigned int isp_rgbir_matrix7 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;         /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cvt67;

/* Define the union u_isp_rgbir_cvt89 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_matrix8 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;         /* [15..11]  */
        unsigned int isp_rgbir_matrix9 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;         /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cvt89;

/* Define the union u_isp_rgbir_cvt1011 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_matrix10 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;          /* [15..11]  */
        unsigned int isp_rgbir_matrix11 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;          /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_cvt1011;

/* Define the union u_isp_rgbir_blc_offset_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_blc_offset_r : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;             /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_blc_offset_r;

/* Define the union u_isp_rgbir_blc_offset_g */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_blc_offset_g : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;             /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_blc_offset_g;

/* Define the union u_isp_rgbir_blc_offset_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_blc_offset_b : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;             /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_blc_offset_b;

/* Define the union u_isp_rgbir_blc_offset_ir */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_rgbir_blc_offset_ir : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;              /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_blc_offset_ir;

/* Define the union u_isp_rgbir_ir_sum */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rgbir_ir_sum : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_rgbir_ir_sum;
/* Define the union u_isp_sharpen_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_version;
/* Define the union u_isp_sharpen_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_benlumactrl : 1;       /* [0]  */
        unsigned int isp_sharpen_vcds_filterv : 1;      /* [1]  */
        unsigned int isp_sharpen_benshtvar_sel : 1;     /* [2]  */
        unsigned int isp_sharpen_benshtctrlbyvar : 1;   /* [3]  */
        unsigned int isp_sharpen_benskinctrl : 1;       /* [4]  */
        unsigned int reserved_0 : 1;                    /* [5]  */
        unsigned int isp_sharpen_benchrctrl : 1;        /* [6]  */
        unsigned int reserved_1 : 1;                    /* [7]  */
        unsigned int reserved_2 : 1;                    /* [8]  */
        unsigned int reserved_3 : 1;                    /* [9]  */
        unsigned int isp_sharpen_detailthd_sel : 3;     /* [12..10]  */
        unsigned int isp_sharpen_bendetailctrl : 1;     /* [13]  */
        unsigned int reserved_4 : 1;                    /* [14]  */
        unsigned int reserved_5 : 1;                    /* [15]  */
        unsigned int isp_sharpen_ben8dir_sel : 1;       /* [16]  */
        unsigned int reserved_6 : 15;                   /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_ctrl;

/* Define the union u_isp_sharpen_mhfthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mhfthdsftd : 4;  /* [3..0]  */
        unsigned int isp_sharpen_mhfthdseld : 2;  /* [5..4]  */
        unsigned int reserved_0 : 10;             /* [15..6]  */
        unsigned int isp_sharpen_mhfthdsftud : 4; /* [19..16]  */
        unsigned int isp_sharpen_mhfthdselud : 2; /* [21..20]  */
        unsigned int reserved_1 : 10;             /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfthd;

/* Define the union u_isp_sharpen_dirvar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_dirvarsft : 4;   /* [3..0]  */
        unsigned int isp_sharpen_dirvarscale : 4; /* [7..4]  */
        unsigned int reserved_0 : 8;              /* [15..8]  */
        unsigned int isp_sharpen_dirrly0 : 7;     /* [22..16]  */
        unsigned int reserved_1 : 1;              /* [23]  */
        unsigned int isp_sharpen_dirrly1 : 7;     /* [30..24]  */
        unsigned int reserved_2 : 1;              /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_dirvar;

/* Define the union u_isp_sharpen_dirdiff */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_dirdiffsft : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;            /* [15..6]  */
        unsigned int isp_sharpen_dirrt0 : 5;     /* [20..16]  */
        unsigned int reserved_1 : 3;             /* [23..21]  */
        unsigned int isp_sharpen_dirrt1 : 5;     /* [28..24]  */
        unsigned int reserved_2 : 3;             /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_dirdiff;

/* Define the union u_isp_sharpen_lumawgt0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt0 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt1 : 6; /* [11..6]  */
        unsigned int isp_sharpen_lumawgt2 : 6; /* [17..12]  */
        unsigned int isp_sharpen_lumawgt3 : 6; /* [23..18]  */
        unsigned int isp_sharpen_lumawgt4 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;           /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt0;

/* Define the union u_isp_sharpen_lumawgt1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt5 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt6 : 6; /* [11..6]  */
        unsigned int isp_sharpen_lumawgt7 : 6; /* [17..12]  */
        unsigned int isp_sharpen_lumawgt8 : 6; /* [23..18]  */
        unsigned int isp_sharpen_lumawgt9 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;           /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt1;

/* Define the union u_isp_sharpen_lumawgt2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt10 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt11 : 6; /* [11..6]  */
        unsigned int isp_sharpen_lumawgt12 : 6; /* [17..12]  */
        unsigned int isp_sharpen_lumawgt13 : 6; /* [23..18]  */
        unsigned int isp_sharpen_lumawgt14 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;            /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt2;

/* Define the union u_isp_sharpen_lumawgt3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt15 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt16 : 6; /* [11..6]  */
        unsigned int isp_sharpen_lumawgt17 : 6; /* [17..12]  */
        unsigned int isp_sharpen_lumawgt18 : 6; /* [23..18]  */
        unsigned int isp_sharpen_lumawgt19 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;            /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt3;

/* Define the union u_isp_sharpen_lumawgt4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt20 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt21 : 6; /* [11..6]  */
        unsigned int isp_sharpen_lumawgt22 : 6; /* [17..12]  */
        unsigned int isp_sharpen_lumawgt23 : 6; /* [23..18]  */
        unsigned int isp_sharpen_lumawgt24 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;            /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt4;

/* Define the union u_isp_sharpen_lumawgt5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt25 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt26 : 6; /* [11..6]  */
        unsigned int isp_sharpen_lumawgt27 : 6; /* [17..12]  */
        unsigned int isp_sharpen_lumawgt28 : 6; /* [23..18]  */
        unsigned int isp_sharpen_lumawgt29 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;            /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt5;

/* Define the union u_isp_sharpen_lumawgt6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lumawgt30 : 6; /* [5..0]  */
        unsigned int isp_sharpen_lumawgt31 : 6; /* [11..6]  */
        unsigned int reserved_0 : 20;           /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lumawgt6;
/* Define the union u_isp_sharpen_luma_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_exluma_thd : 9; /* [8..0]  */
        unsigned int    reserved_0            : 7; /* [15..9]  */
        unsigned int    isp_sharpen_exluma_out_thd : 9; /* [24..16]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_luma_thd;

/* Define the union u_isp_sharpen_shoot_amt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_selpixwgt : 5; /* [4..0]  */
        unsigned int reserved_0 : 3;            /* [7..5]  */
        unsigned int isp_sharpen_oshtamt : 7;   /* [14..8]  */
        unsigned int reserved_1 : 1;            /* [15]  */
        unsigned int isp_sharpen_ushtamt : 7;   /* [22..16]  */
        unsigned int reserved_2 : 9;            /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_shoot_amt;

/* Define the union u_isp_sharpen_shoot_maxchg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_omaxchg : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;           /* [15..10]  */
        unsigned int isp_sharpen_umaxchg : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_shoot_maxchg;

/* Define the union u_isp_sharpen_shtvar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_shtvarsft : 3;     /* [2..0]  */
        unsigned int reserved_0 : 1;                /* [3]  */
        unsigned int isp_sharpen_shtvar5x5_sft : 4; /* [7..4]  */
        unsigned int reserved_1 : 8;                /* [15..8]  */
        unsigned int isp_sharpen_shtbldrt : 4;      /* [19..16]  */
        unsigned int    isp_sharpen_dirvar_thd : 10  ; /* [29..20]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_shtvar;
/* Define the union u_isp_sharpen_hard_luma_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_hard_luma_thd : 9; /* [8..0]  */
        unsigned int    reserved_0            : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_hard_luma_thd;

/* Define the union u_isp_sharpen_oshtvar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_shtvarthd0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_shtvarthd1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_shtvarwgt0 : 7; /* [22..16]  */
        unsigned int reserved_0 : 1;             /* [23]  */
        unsigned int isp_sharpen_shtvarwgt1 : 7; /* [30..24]  */
        unsigned int reserved_1 : 1;             /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_oshtvar;

/* Define the union u_isp_sharpen_exluma_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_exluma_mul : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_exluma_mul;

/* Define the union u_isp_sharpen_shtvar_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_shtvarmul : 12;     /* [11..0]  */
        unsigned int reserved_0 : 4;                 /* [15..12]  */
        unsigned int isp_sharpen_shtvardiffmul : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;                 /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_shtvar_mul;

/* Define the union u_isp_sharpen_oshtvardiff */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_shtvardiffthd0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_shtvardiffthd1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_shtvardiffwgt0 : 7; /* [22..16]  */
        unsigned int reserved_0 : 1;                 /* [23]  */
        unsigned int isp_sharpen_shtvardiffwgt1 : 7; /* [30..24]  */
        unsigned int reserved_1 : 1;                 /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_oshtvardiff;

/* Define the union u_isp_sharpen_lmtmf0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmtmf0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;         /* [7..6]  */
        unsigned int isp_sharpen_lmtmf1 : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;         /* [15..14]  */
        unsigned int isp_sharpen_lmtmf2 : 6; /* [21..16]  */
        unsigned int reserved_2 : 2;         /* [23..22]  */
        unsigned int isp_sharpen_lmtmf3 : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;         /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmtmf0;

/* Define the union u_isp_sharpen_lmtmf1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmtmf4 : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;         /* [7..6]  */
        unsigned int isp_sharpen_lmtmf5 : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;         /* [15..14]  */
        unsigned int isp_sharpen_lmtmf6 : 6; /* [21..16]  */
        unsigned int reserved_2 : 2;         /* [23..22]  */
        unsigned int isp_sharpen_lmtmf7 : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;         /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmtmf1;

/* Define the union u_isp_sharpen_lmthf0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmthf0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;         /* [7..6]  */
        unsigned int isp_sharpen_lmthf1 : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;         /* [15..14]  */
        unsigned int isp_sharpen_lmthf2 : 6; /* [21..16]  */
        unsigned int reserved_2 : 2;         /* [23..22]  */
        unsigned int isp_sharpen_lmthf3 : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;         /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmthf0;

/* Define the union u_isp_sharpen_lmthf1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmthf4 : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;         /* [7..6]  */
        unsigned int isp_sharpen_lmthf5 : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;         /* [15..14]  */
        unsigned int isp_sharpen_lmthf6 : 6; /* [21..16]  */
        unsigned int reserved_2 : 2;         /* [23..22]  */
        unsigned int isp_sharpen_lmthf7 : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;         /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmthf1;

/* Define the union u_isp_sharpen_mhfgaind_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgaind_waddr;
/* Define the union u_isp_sharpen_mhfgaind_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgaind_wdata;
/* Define the union u_isp_sharpen_mhfgaind_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgaind_raddr;
/* Define the union u_isp_sharpen_mhfgaind_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgaind_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgaind_rdata;
/* Define the union u_isp_sharpen_mhfgainud_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgainud_waddr;
/* Define the union u_isp_sharpen_mhfgainud_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgainud_wdata;
/* Define the union u_isp_sharpen_mhfgainud_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgainud_raddr;
/* Define the union u_isp_sharpen_mhfgainud_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mhfgainud_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mhfgainud_rdata;
/* Define the union u_isp_sharpen_skin_u */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_skinminu : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;           /* [15..8]  */
        unsigned int isp_sharpen_skinmaxu : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_skin_u;

/* Define the union u_isp_sharpen_skin_v */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_skinminv : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;           /* [15..8]  */
        unsigned int isp_sharpen_skinmaxv : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_skin_v;

/* Define the union u_isp_sharpen_skin_cnt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_skincntthd0 : 4; /* [3..0]  */
        unsigned int isp_sharpen_skincntthd1 : 4; /* [7..4]  */
        unsigned int isp_sharpen_skincntmul : 5;  /* [12..8]  */
        unsigned int reserved_0 : 19;             /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_skin_cnt;

/* Define the union u_isp_sharpen_skin_edge */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_skinedgesft : 4;  /* [3..0]  */
        unsigned int reserved_0 : 12;              /* [15..4]  */
        unsigned int isp_sharpen_skinedgemul : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;               /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_skin_edge;

/* Define the union u_isp_sharpen_skin_edgethd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_skinedgethd0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_skinedgethd1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_skinedgewgt0 : 5; /* [20..16]  */
        unsigned int reserved_0 : 3;               /* [23..21]  */
        unsigned int isp_sharpen_skinedgewgt1 : 5; /* [28..24]  */
        unsigned int reserved_1 : 3;               /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_skin_edgethd;

/* Define the union u_isp_sharpen_chrr_var */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrrsft0 : 3;     /* [2..0]  */
        unsigned int isp_sharpen_chrrsft1 : 3;     /* [5..3]  */
        unsigned int isp_sharpen_chrrsft2 : 3;     /* [8..6]  */
        unsigned int isp_sharpen_chrrsft3 : 3;     /* [11..9]  */
        unsigned int isp_sharpen_chrrvarshift : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;              /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrr_var;

/* Define the union u_isp_sharpen_chrr_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrrori0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_chrrori1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_chrrthd0 : 8; /* [23..16]  */
        unsigned int isp_sharpen_chrrthd1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrr_thd;

/* Define the union u_isp_sharpen_chrr_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrrgain : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;           /* [7..6]  */
        unsigned int isp_sharpen_rmfscale : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;           /* [15..14]  */
        unsigned int isp_sharpen_chrrmul : 10; /* [25..16]  */
        unsigned int reserved_2 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrr_gain;

/* Define the union u_isp_sharpen_chrg_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrgmul : 14;   /* [13..0]  */
        unsigned int reserved_0 : 2;             /* [15..14]  */
        unsigned int isp_sharpen_chrgmfmul : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;             /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrg_mul;

/* Define the union u_isp_sharpen_chrg_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrgsft0 : 3; /* [2..0]  */
        unsigned int isp_sharpen_chrgsft1 : 3; /* [5..3]  */
        unsigned int isp_sharpen_chrgsft2 : 3; /* [8..6]  */
        unsigned int isp_sharpen_chrgsft3 : 3; /* [11..9]  */
        unsigned int reserved_0 : 20;          /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrg_sft;

/* Define the union u_isp_sharpen_chrg_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrgori0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_chrgori1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_chrgthd0 : 8; /* [23..16]  */
        unsigned int isp_sharpen_chrgthd1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrg_thd;

/* Define the union u_isp_sharpen_chrg_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrggain : 9;   /* [8..0]  */
        unsigned int reserved_0 : 7;             /* [15..9]  */
        unsigned int isp_sharpen_chrgmfgain : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;             /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrg_gain;

/* Define the union u_isp_sharpen_chrb_var */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrbsft0 : 3;     /* [2..0]  */
        unsigned int isp_sharpen_chrbsft1 : 3;     /* [5..3]  */
        unsigned int isp_sharpen_chrbsft2 : 3;     /* [8..6]  */
        unsigned int isp_sharpen_chrbsft3 : 3;     /* [11..9]  */
        unsigned int isp_sharpen_chrbvarshift : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;              /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrb_var;

/* Define the union u_isp_sharpen_chrb_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrbori0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_chrbori1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_chrbthd0 : 8; /* [23..16]  */
        unsigned int isp_sharpen_chrbthd1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrb_thd;

/* Define the union u_isp_sharpen_chrb_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_chrbgain : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;           /* [7..6]  */
        unsigned int isp_sharpen_bmfscale : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;           /* [15..14]  */
        unsigned int isp_sharpen_chrbmul : 10; /* [25..16]  */
        unsigned int reserved_2 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_chrb_gain;

/* Define the union u_isp_sharpen_gain_sft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mfgain_sft : 3; /* [2..0]  */
        unsigned int reserved_0 : 5;             /* [7..3]  */
        unsigned int isp_sharpen_lfgainwgt : 5;  /* [12..8]  */
        unsigned int reserved_1 : 3;             /* [15..13]  */
        unsigned int isp_sharpen_hfgain_sft : 3; /* [18..16]  */
        unsigned int reserved_2 : 13;            /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_gain_sft;

/* Define the union u_isp_sharpen_shoot_maxgain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_omaxgain : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;            /* [15..11]  */
        unsigned int isp_sharpen_umaxgain : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;            /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_shoot_maxgain;

/* Define the union u_isp_sharpen_detail_mul */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_detl_oshtmul : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                /* [15..12]  */
        unsigned int isp_sharpen_detl_ushtmul : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;                /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_detail_mul;

/* Define the union u_isp_sharpen_osht_detail */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_osht_dtl_thd0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_osht_dtl_thd1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_osht_dtl_wgt : 7;  /* [22..16]  */
        unsigned int reserved_0 : 1;                /* [23]  */
        unsigned int isp_sharpen_max_var_clip : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_osht_detail;

/* Define the union u_isp_sharpen_usht_detail */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_usht_dtl_thd0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_usht_dtl_thd1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_usht_dtl_wgt : 7;  /* [22..16]  */
        unsigned int reserved_0 : 1;                /* [23]  */
        unsigned int isp_sharpen_dtl_thdsft : 4;    /* [27..24]  */
        unsigned int reserved_1 : 4;                /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_usht_detail;

/* Define the union u_isp_sharpen_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;            /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_stt2lut_cfg;

/* Define the union u_isp_sharpen_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;                /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_stt2lut_regnew;

/* Define the union u_isp_sharpen_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;               /* [30..16]  */
        unsigned int isp_sharpen_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_stt2lut_abn;

/* Define the union u_isp_sharpen_std_gain_bymot0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_bymot0 : 6; /* [5..0]  */
        unsigned int isp_sharpen_std_gain_bymot1 : 6; /* [11..6]  */
        unsigned int isp_sharpen_std_gain_bymot2 : 6; /* [17..12]  */
        unsigned int isp_sharpen_std_gain_bymot3 : 6; /* [23..18]  */
        unsigned int isp_sharpen_std_gain_bymot4 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;                  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_bymot0;

/* Define the union u_isp_sharpen_std_gain_bymot1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_bymot5 : 6; /* [5..0]  */
        unsigned int isp_sharpen_std_gain_bymot6 : 6; /* [11..6]  */
        unsigned int isp_sharpen_std_gain_bymot7 : 6; /* [17..12]  */
        unsigned int isp_sharpen_std_gain_bymot8 : 6; /* [23..18]  */
        unsigned int isp_sharpen_std_gain_bymot9 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;                  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_bymot1;

/* Define the union u_isp_sharpen_std_gain_bymot2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_bymot10 : 6; /* [5..0]  */
        unsigned int isp_sharpen_std_gain_bymot11 : 6; /* [11..6]  */
        unsigned int isp_sharpen_std_gain_bymot12 : 6; /* [17..12]  */
        unsigned int isp_sharpen_std_gain_bymot13 : 6; /* [23..18]  */
        unsigned int isp_sharpen_std_gain_bymot14 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;                   /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_bymot2;

/* Define the union u_isp_sharpen_std_gain_bymot3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_bymot15 : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;                  /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_bymot3;

/* Define the union u_isp_sharpen_std_gain_byy0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_byy0 : 6; /* [5..0]  */
        unsigned int isp_sharpen_std_gain_byy1 : 6; /* [11..6]  */
        unsigned int isp_sharpen_std_gain_byy2 : 6; /* [17..12]  */
        unsigned int isp_sharpen_std_gain_byy3 : 6; /* [23..18]  */
        unsigned int isp_sharpen_std_gain_byy4 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;                /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_byy0;

/* Define the union u_isp_sharpen_std_gain_byy1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_byy5 : 6; /* [5..0]  */
        unsigned int isp_sharpen_std_gain_byy6 : 6; /* [11..6]  */
        unsigned int isp_sharpen_std_gain_byy7 : 6; /* [17..12]  */
        unsigned int isp_sharpen_std_gain_byy8 : 6; /* [23..18]  */
        unsigned int isp_sharpen_std_gain_byy9 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;                /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_byy1;

/* Define the union u_isp_sharpen_std_gain_byy2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_byy10 : 6; /* [5..0]  */
        unsigned int isp_sharpen_std_gain_byy11 : 6; /* [11..6]  */
        unsigned int isp_sharpen_std_gain_byy12 : 6; /* [17..12]  */
        unsigned int isp_sharpen_std_gain_byy13 : 6; /* [23..18]  */
        unsigned int isp_sharpen_std_gain_byy14 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;                 /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_byy2;

/* Define the union u_isp_sharpen_std_gain_byy3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_gain_byy15 : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;                /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_gain_byy3;

/* Define the union u_isp_sharpen_std_offset_bymot0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_bymot0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_std_offset_bymot1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_std_offset_bymot2 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_bymot3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_bymot0;

/* Define the union u_isp_sharpen_std_offset_bymot1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_bymot4 : 8; /* [7..0]  */
        unsigned int isp_sharpen_std_offset_bymot5 : 8; /* [15..8]  */
        unsigned int isp_sharpen_std_offset_bymot6 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_bymot7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_bymot1;

/* Define the union u_isp_sharpen_std_offset_bymot2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_bymot8 : 8;  /* [7..0]  */
        unsigned int isp_sharpen_std_offset_bymot9 : 8;  /* [15..8]  */
        unsigned int isp_sharpen_std_offset_bymot10 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_bymot11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_bymot2;

/* Define the union u_isp_sharpen_std_offset_bymot3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_bymot12 : 8; /* [7..0]  */
        unsigned int isp_sharpen_std_offset_bymot13 : 8; /* [15..8]  */
        unsigned int isp_sharpen_std_offset_bymot14 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_bymot15 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_bymot3;

/* Define the union u_isp_sharpen_std_offset_byy0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_byy0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_std_offset_byy1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_std_offset_byy2 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_byy3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_byy0;

/* Define the union u_isp_sharpen_std_offset_byy1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_byy4 : 8; /* [7..0]  */
        unsigned int isp_sharpen_std_offset_byy5 : 8; /* [15..8]  */
        unsigned int isp_sharpen_std_offset_byy6 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_byy7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_byy1;

/* Define the union u_isp_sharpen_std_offset_byy2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_byy8 : 8;  /* [7..0]  */
        unsigned int isp_sharpen_std_offset_byy9 : 8;  /* [15..8]  */
        unsigned int isp_sharpen_std_offset_byy10 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_byy11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_byy2;

/* Define the union u_isp_sharpen_std_offset_byy3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_std_offset_byy12 : 8; /* [7..0]  */
        unsigned int isp_sharpen_std_offset_byy13 : 8; /* [15..8]  */
        unsigned int isp_sharpen_std_offset_byy14 : 8; /* [23..16]  */
        unsigned int isp_sharpen_std_offset_byy15 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_std_offset_byy3;

/* Define the union u_isp_sharpen_lmf_mot_gain0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmf_mot_gain0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_lmf_mot_gain1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_lmf_mot_gain2 : 8; /* [23..16]  */
        unsigned int isp_sharpen_lmf_mot_gain3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmf_mot_gain0;

/* Define the union u_isp_sharpen_lmf_mot_gain1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmf_mot_gain4 : 8; /* [7..0]  */
        unsigned int isp_sharpen_lmf_mot_gain5 : 8; /* [15..8]  */
        unsigned int isp_sharpen_lmf_mot_gain6 : 8; /* [23..16]  */
        unsigned int isp_sharpen_lmf_mot_gain7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmf_mot_gain1;

/* Define the union u_isp_sharpen_lmf_mot_gain2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmf_mot_gain8 : 8;  /* [7..0]  */
        unsigned int isp_sharpen_lmf_mot_gain9 : 8;  /* [15..8]  */
        unsigned int isp_sharpen_lmf_mot_gain10 : 8; /* [23..16]  */
        unsigned int isp_sharpen_lmf_mot_gain11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmf_mot_gain2;

/* Define the union u_isp_sharpen_lmf_mot_gain3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_lmf_mot_gain12 : 8; /* [7..0]  */
        unsigned int isp_sharpen_lmf_mot_gain13 : 8; /* [15..8]  */
        unsigned int isp_sharpen_lmf_mot_gain14 : 8; /* [23..16]  */
        unsigned int isp_sharpen_lmf_mot_gain15 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_lmf_mot_gain3;

/* Define the union u_isp_sharpen_mf_mot_dec0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mf_mot_dec0 : 6; /* [5..0]  */
        unsigned int isp_sharpen_mf_mot_dec1 : 6; /* [11..6]  */
        unsigned int isp_sharpen_mf_mot_dec2 : 6; /* [17..12]  */
        unsigned int isp_sharpen_mf_mot_dec3 : 6; /* [23..18]  */
        unsigned int isp_sharpen_mf_mot_dec4 : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mf_mot_dec0;

/* Define the union u_isp_sharpen_mf_mot_dec1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mf_mot_dec5 : 6; /* [5..0]  */
        unsigned int isp_sharpen_mf_mot_dec6 : 6; /* [11..6]  */
        unsigned int isp_sharpen_mf_mot_dec7 : 6; /* [17..12]  */
        unsigned int isp_sharpen_mf_mot_dec8 : 6; /* [23..18]  */
        unsigned int isp_sharpen_mf_mot_dec9 : 6; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mf_mot_dec1;

/* Define the union u_isp_sharpen_mf_mot_dec2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mf_mot_dec10 : 6; /* [5..0]  */
        unsigned int isp_sharpen_mf_mot_dec11 : 6; /* [11..6]  */
        unsigned int isp_sharpen_mf_mot_dec12 : 6; /* [17..12]  */
        unsigned int isp_sharpen_mf_mot_dec13 : 6; /* [23..18]  */
        unsigned int isp_sharpen_mf_mot_dec14 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;               /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mf_mot_dec2;

/* Define the union u_isp_sharpen_mf_mot_dec3 */
typedef union {
  /* Define the struct bits */
  struct {
    unsigned int isp_sharpen_mf_mot_dec15 : 6; /* [5..0]  */
    unsigned int reserved_0 : 26;              /* [31..6]  */
  } bits;

  /* Define an unsigned member */
  unsigned int u32;
} u_isp_sharpen_mf_mot_dec3;
/* Define the union u_isp_sharpen_hf_mot_dec0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_hf_mot_dec0 : 6; /* [5..0]  */
        unsigned int isp_sharpen_hf_mot_dec1 : 6; /* [11..6]  */
        unsigned int isp_sharpen_hf_mot_dec2 : 6; /* [17..12]  */
        unsigned int isp_sharpen_hf_mot_dec3 : 6; /* [23..18]  */
        unsigned int isp_sharpen_hf_mot_dec4 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;              /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_hf_mot_dec0;

/* Define the union u_isp_sharpen_hf_mot_dec1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_hf_mot_dec5 : 6; /* [5..0]  */
        unsigned int isp_sharpen_hf_mot_dec6 : 6; /* [11..6]  */
        unsigned int isp_sharpen_hf_mot_dec7 : 6; /* [17..12]  */
        unsigned int isp_sharpen_hf_mot_dec8 : 6; /* [23..18]  */
        unsigned int isp_sharpen_hf_mot_dec9 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;              /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_hf_mot_dec1;

/* Define the union u_isp_sharpen_hf_mot_dec2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_hf_mot_dec10 : 6; /* [5..0]  */
        unsigned int isp_sharpen_hf_mot_dec11 : 6; /* [11..6]  */
        unsigned int isp_sharpen_hf_mot_dec12 : 6; /* [17..12]  */
        unsigned int isp_sharpen_hf_mot_dec13 : 6; /* [23..18]  */
        unsigned int isp_sharpen_hf_mot_dec14 : 6; /* [29..24]  */
        unsigned int reserved_0 : 2;               /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_hf_mot_dec2;

/* Define the union u_isp_sharpen_hf_mot_dec3 */
typedef union {
  /* Define the struct bits */
  struct {
    unsigned int isp_sharpen_hf_mot_dec15 : 6; /* [5..0]  */
    unsigned int reserved_0 : 26;              /* [31..6]  */
  } bits;

  /* Define an unsigned member */
  unsigned int u32;
} u_isp_sharpen_hf_mot_dec3;
/* Define the union u_isp_sharpen_rly_wgt0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_rly_wgt0 : 8; /* [7..0]  */
        unsigned int isp_sharpen_rly_wgt1 : 8; /* [15..8]  */
        unsigned int isp_sharpen_rly_wgt2 : 8; /* [23..16]  */
        unsigned int isp_sharpen_rly_wgt3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_rly_wgt0;

/* Define the union u_isp_sharpen_rly_wgt1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_rly_wgt4 : 8; /* [7..0]  */
        unsigned int isp_sharpen_rly_wgt5 : 8; /* [15..8]  */
        unsigned int isp_sharpen_rly_wgt6 : 8; /* [23..16]  */
        unsigned int isp_sharpen_rly_wgt7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_rly_wgt1;

/* Define the union u_isp_sharpen_rly_wgt2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_rly_wgt8 : 8;  /* [7..0]  */
        unsigned int isp_sharpen_rly_wgt9 : 8;  /* [15..8]  */
        unsigned int isp_sharpen_rly_wgt10 : 8; /* [23..16]  */
        unsigned int isp_sharpen_rly_wgt11 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_rly_wgt2;

/* Define the union u_isp_sharpen_rly_wgt3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_rly_wgt12 : 8; /* [7..0]  */
        unsigned int isp_sharpen_rly_wgt13 : 8; /* [15..8]  */
        unsigned int isp_sharpen_rly_wgt14 : 8; /* [23..16]  */
        unsigned int isp_sharpen_rly_wgt15 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_rly_wgt3;

/* Define the union u_isp_sharpen_mot_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_mot_ctrl_en : 1;       /* [0]  */
        unsigned int isp_sharpen_mot_interp_mode : 1;   /* [1]  */
        unsigned int isp_sharpen_std_adj_bymot_en : 1;  /* [2]  */
        unsigned int isp_sharpen_std_adj_byy_en : 1;    /* [3]  */
        unsigned int isp_sharpen_std_comb_mode : 2;     /* [5..4]  */
        unsigned int isp_sharpen_std_comb_alpha : 5;    /* [10..6]  */
        unsigned int isp_sharpen_mot_thd : 4;           /* [14..11]  */
        unsigned int isp_sharpen_mot_en : 1;            /* [15]  */
        unsigned int isp_sharpen_var7x9_calc_en : 1;   /* [16]  */
        unsigned int reserved_0 : 15;                  /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_mot_cfg;

/* Define the union u_isp_sharpen_var5_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_var5_low_thd : 10;  /* [9..0]  */
        unsigned int isp_sharpen_var5_mid_thd : 10;  /* [19..10]  */
        unsigned int isp_sharpen_var5_high_thd : 10; /* [29..20]  */
        unsigned int reserved_0 : 2;                 /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_var5_thd;

/* Define the union u_isp_sharpen_var7_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_var7_low_thd : 10;  /* [9..0]  */
        unsigned int isp_sharpen_var7_high_thd : 10; /* [19..10]  */
        unsigned int reserved_0 : 12;                /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_var7_thd;

/* Define the union u_isp_sharpen_shoot_edge_amt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_edge_usht_amt : 7; /* [6..0]  */
        unsigned int isp_sharpen_edge_osht_amt : 7; /* [13..7]  */
        unsigned int reserved_0 : 18;               /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_shoot_edge_amt;


/* Define the union u_isp_sharpen_ldci_dither_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_sharpen_ldci_dither_round : 2; /* [1..0]  */
        unsigned int reserved_0 : 30;                   /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_sharpen_ldci_dither_cfg;

/* Define the union u_isp_bnr_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_ensptnr : 1;   /* [0]  */
        unsigned int isp_bnr_entmpnr : 1;   /* [1]  */
        unsigned int isp_bnr_isinitial : 1; /* [2]  */
        unsigned int isp_bnr_frm_phase : 2; /* [4..3]  */
        unsigned int isp_bnr2d_en : 1;      /* [5]  */
        unsigned int isp_bnr_freqreconst_en : 1; /* [6]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg;

/* Define the union u_isp_bnr_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
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
        unsigned int isp_bnr_bltev500_wt_cmax : 5;            /* [25..21]  */
        unsigned int isp_bnr_sad_bit_reduction : 3;           /* [28..26]  */
        unsigned int isp_bnr_blt_enable_symm_sad : 1;         /* [29]  */
        unsigned int isp_bnr_jnlm_lum_sel : 2;                /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg0;

/* Define the union u_isp_bnr_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain0 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg1;

/* Define the union u_isp_bnr_cfg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain1 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg2;

/* Define the union u_isp_bnr_cfg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain2 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg3;

/* Define the union u_isp_bnr_cfg4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain3 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg4;

/* Define the union u_isp_bnr_cfg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain4 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg5;

/* Define the union u_isp_bnr_cfg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain5 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg6;

/* Define the union u_isp_bnr_cfg7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain6 : 17; /* [16..0]  */
        unsigned int reserved_0 : 15;             /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg7;

/* Define the union u_isp_bnr_cfg8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_jnlm_lmt_gain7 : 17; /* [16..0]  */
        unsigned int isp_bnr_en_area_alt : 1;     /* [17]  */
        unsigned int isp_bnr_mdet_size : 3;       /* [20..18]  */
        unsigned int isp_bnr_mdet_cor_level : 8;  /* [28..21]  */
        unsigned int isp_bnr_b_deform_md_wim : 1; /* [29]  */
        unsigned int reserved_0 : 2;              /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg8;

/* Define the union u_isp_bnr_cfg9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_rnt_th : 8;             /* [7..0]  */
        unsigned int isp_bnr_rnt_wid : 8;            /* [15..8]  */
        unsigned int isp_bnr_nr_strength_st_int : 9; /* [24..16]  */
        unsigned int isp_bnr_inputraw_ratio : 7;     /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg9;

/* Define the union u_isp_bnr_cfg10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_nr_strength_mv_int : 9; /* [8..0]  */
        unsigned int isp_bnr_nr_strength_slope : 10; /* [18..9]  */
        unsigned int isp_bnr_en_mixing : 2;          /* [20..19]  */
        unsigned int isp_bnr_target_noise_ratio : 8; /* [28..21]  */
        unsigned int reserved_0 : 3;                 /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg10;

/* Define the union u_isp_bnr_cfg11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_mix_gain_0_r : 8; /* [7..0]  */
        unsigned int isp_bnr_mix_gain_0_b : 8; /* [15..8]  */
        unsigned int isp_bnr_wb_gain_r : 10;   /* [25..16]  */
        unsigned int isp_bnr_rb_diff_en : 1;   /* [26]  */
        unsigned int reserved_0 : 5;           /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg11;

/* Define the union u_isp_bnr_cfg12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_wb_gain_b : 10;     /* [9..0]  */
        unsigned int isp_bnr_wb_gain_inv_r : 16; /* [25..10]  */
        unsigned int reserved_0 : 6;             /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg12;

/* Define the union u_isp_bnr_cfg13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_wb_gain_inv_b : 16; /* [15..0]  */
        unsigned int isp_bnr_bnrlscen : 1;       /* [16]  */
        unsigned int isp_bnr_bnrlscratio : 4;    /* [20..17]  */
        unsigned int isp_bnr_alphamax_st : 8;    /* [28..21]  */
        unsigned int reserved_0 : 3;             /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg13;

/* Define the union u_isp_bnr_cfg14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_wdr_enable : 2;      /* [1..0]  */
        unsigned int isp_bnr_wdr_en_fusion : 2;   /* [3..2]  */
        unsigned int isp_bnr_wdr_map_flt_mod : 1; /* [4]  */
        unsigned int isp_bnr_wdr_map_gain : 5;    /* [9..5]  */
        unsigned int isp_bnr_en_2nd_tmp_nr : 1;   /* [10]  */
        unsigned int isp_bnr_ynet_alpha : 8;      /* [18..11]  */
        unsigned int isp_bnr_mdet_mixratio : 7;   /* [25..19]  */
        unsigned int reserved_0 : 6;              /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg14;

/* Define the union u_isp_bnr_cfg15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_coring_hig : 16;     /* [15..0]  */
        unsigned int isp_bnr_coring_enable : 1;   /* [16]  */
        unsigned int isp_bnr_jnlm_gain : 8;       /* [24..17]  */
        unsigned int isp_bnr_maxv_mode : 1;       /* [25]  */
        unsigned int isp_bnr_mixgain_sprs_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;              /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg15;

/* Define the union u_isp_bnr_cfg16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_nr_strength_mv_int_mot2yuv : 9; /* [8..0]  */
        unsigned int isp_bnr_nr_strength_slope_mot2yuv : 10; /* [18..9]  */
        unsigned int isp_bnr_nr_strength_st_int_mot2yuv : 9; /* [27..19]  */
        unsigned int reserved_0 : 4;                         /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg16;

/* Define the union u_isp_bnr_cfg17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_rnt_th_mot2yuv : 8;         /* [7..0]  */
        unsigned int isp_bnr_mdet_cor_level_mot2yuv : 8; /* [15..8]  */
        unsigned int isp_bnr_mdet_size_mot2yuv : 3;      /* [18..16]  */
        unsigned int isp_bnr_tss : 8;                    /* [26..19]  */
        unsigned int reserved_0 : 5;                     /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_cfg17;

/* Define the union u_isp_bnr_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;        /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_stt2lut_cfg;

/* Define the union u_isp_bnr_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;            /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_stt2lut_regnew;

/* Define the union u_isp_bnr_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;           /* [30..16]  */
        unsigned int isp_bnr_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_stt2lut_abn;

/* Define the union u_isp_bnr_noisesd_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_even_waddr;
/* Define the union u_isp_bnr_noisesd_even_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesd_even_wdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesd_s_even_wdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_even_wdata;

/* Define the union u_isp_bnr_noisesd_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_even_raddr;
/* Define the union u_isp_bnr_noisesd_even_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesd_even_rdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesd_s_even_rdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_even_rdata;

/* Define the union u_isp_bnr_noisesd_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_odd_waddr;
/* Define the union u_isp_bnr_noisesd_odd_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesd_odd_wdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesd_s_odd_wdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                   /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_odd_wdata;

/* Define the union u_isp_bnr_noisesd_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesd_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_odd_raddr;
/* Define the union u_isp_bnr_noisesd_odd_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesd_odd_rdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesd_s_odd_rdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                   /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesd_odd_rdata;

/* Define the union u_isp_bnr_noiseinv_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_even_waddr;
/* Define the union u_isp_bnr_noiseinv_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_even_wdata;
/* Define the union u_isp_bnr_noiseinv_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_even_raddr;
/* Define the union u_isp_bnr_noiseinv_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_even_rdata;
/* Define the union u_isp_bnr_noiseinv_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_odd_waddr;
/* Define the union u_isp_bnr_noiseinv_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_odd_wdata;
/* Define the union u_isp_bnr_noiseinv_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_odd_raddr;
/* Define the union u_isp_bnr_noiseinv_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_odd_rdata;
/* Define the union u_isp_bnr_fbratiotable_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbratiotable_waddr;
/* Define the union u_isp_bnr_fbratiotable_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbratiotable_wdata;
/* Define the union u_isp_bnr_fbratiotable_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbratiotable_raddr;
/* Define the union u_isp_bnr_fbratiotable_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbratiotable_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbratiotable_rdata;
/* Define the union u_isp_bnr_noisesds_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_even_waddr;
/* Define the union u_isp_bnr_noisesds_even_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesds_even_wdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesds_s_even_wdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                     /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_even_wdata;

/* Define the union u_isp_bnr_noisesds_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_even_raddr;
/* Define the union u_isp_bnr_noisesds_even_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesds_even_rdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesds_s_even_rdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                     /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_even_rdata;

/* Define the union u_isp_bnr_noisesds_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_odd_waddr;
/* Define the union u_isp_bnr_noisesds_odd_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesds_odd_wdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesds_s_odd_wdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_odd_wdata;

/* Define the union u_isp_bnr_noisesds_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noisesds_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_odd_raddr;
/* Define the union u_isp_bnr_noisesds_odd_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_bnr_noisesds_odd_rdata : 15;   /* [14..0]  */
        unsigned int isp_bnr_noisesds_s_odd_rdata : 15; /* [29..15]  */
        unsigned int reserved_0 : 2;                    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noisesds_odd_rdata;

/* Define the union u_isp_bnr_noiseinv_mag_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_even_waddr;
/* Define the union u_isp_bnr_noiseinv_mag_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_even_wdata;
/* Define the union u_isp_bnr_noiseinv_mag_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_even_raddr;
/* Define the union u_isp_bnr_noiseinv_mag_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_even_rdata;
/* Define the union u_isp_bnr_noiseinv_mag_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_odd_waddr;
/* Define the union u_isp_bnr_noiseinv_mag_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_odd_wdata;
/* Define the union u_isp_bnr_noiseinv_mag_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_odd_raddr;
/* Define the union u_isp_bnr_noiseinv_mag_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_noiseinv_mag_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_noiseinv_mag_odd_rdata;
/* Define the union u_isp_bnr_coring_low_even_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_even_waddr;
/* Define the union u_isp_bnr_coring_low_even_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_even_wdata;
/* Define the union u_isp_bnr_coring_low_even_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_even_raddr;
/* Define the union u_isp_bnr_coring_low_even_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_even_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_even_rdata;
/* Define the union u_isp_bnr_coring_low_odd_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_odd_waddr;
/* Define the union u_isp_bnr_coring_low_odd_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_odd_wdata;
/* Define the union u_isp_bnr_coring_low_odd_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_odd_raddr;
/* Define the union u_isp_bnr_coring_low_odd_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_coring_low_odd_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_coring_low_odd_rdata;

/* Define the union u_isp_bnr_fbra_mot2yuv_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbra_mot2yuv_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbra_mot2yuv_waddr;
/* Define the union u_isp_bnr_fbra_mot2yuv_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbra_mot2yuv_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbra_mot2yuv_wdata;
/* Define the union u_isp_bnr_fbra_mot2yuv_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbra_mot2yuv_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbra_mot2yuv_raddr;
/* Define the union u_isp_bnr_fbra_mot2yuv_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_fbra_mot2yuv_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bnr_fbra_mot2yuv_rdata;


/* Define the union u_isp_mlsc_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_stt2lut_cfg;

/* Define the union u_isp_mlsc_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;             /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_stt2lut_regnew;

/* Define the union u_isp_mlsc_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;            /* [30..16]  */
        unsigned int isp_mlsc_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_stt2lut_abn;

/* Define the union u_isp_mlsc_winnum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_numh : 6; /* [5..0]  */
        unsigned int reserved_0 : 10;   /* [15..6]  */
        unsigned int isp_mlsc_numv : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;   /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winnum;

/* Define the union u_isp_mlsc_winx0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax0 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx0 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx0;

/* Define the union u_isp_mlsc_winx1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax1 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx1 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx1;

/* Define the union u_isp_mlsc_winx2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax2 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx2 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx2;

/* Define the union u_isp_mlsc_winx3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax3 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx3 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx3;

/* Define the union u_isp_mlsc_winx4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax4 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx4 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx4;

/* Define the union u_isp_mlsc_winx5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax5 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx5 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx5;

/* Define the union u_isp_mlsc_winx6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax6 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx6 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx6;

/* Define the union u_isp_mlsc_winx7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax7 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx7 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx7;

/* Define the union u_isp_mlsc_winx8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax8 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx8 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx8;

/* Define the union u_isp_mlsc_winx9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax9 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invx9 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx9;

/* Define the union u_isp_mlsc_winx10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax10 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx10 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx10;

/* Define the union u_isp_mlsc_winx11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax11 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx11 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx11;

/* Define the union u_isp_mlsc_winx12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax12 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx12 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx12;

/* Define the union u_isp_mlsc_winx13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax13 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx13 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx13;

/* Define the union u_isp_mlsc_winx14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax14 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx14 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx14;

/* Define the union u_isp_mlsc_winx15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax15 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx15 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx15;

/* Define the union u_isp_mlsc_winy0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay0 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy0 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy0;

/* Define the union u_isp_mlsc_winy1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay1 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy1 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy1;

/* Define the union u_isp_mlsc_winy2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay2 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy2 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy2;

/* Define the union u_isp_mlsc_winy3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay3 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy3 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy3;

/* Define the union u_isp_mlsc_winy4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay4 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy4 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy4;

/* Define the union u_isp_mlsc_winy5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay5 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy5 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy5;

/* Define the union u_isp_mlsc_winy6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay6 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy6 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy6;

/* Define the union u_isp_mlsc_winy7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay7 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy7 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy7;

/* Define the union u_isp_mlsc_width_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_width_offset : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;            /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_width_offset;

/* Define the union u_isp_mlsc_mesh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_mesh_str : 10;  /* [9..0]  */
        unsigned int reserved_0 : 6;          /* [15..10]  */
        unsigned int isp_mlsc_mesh_scale : 3; /* [18..16]  */
        unsigned int reserved_1 : 13;         /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_mesh;

/* Define the union u_isp_mlsc_rgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_rgain_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_rgain_waddr;
/* Define the union u_isp_mlsc_rgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_rgain_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;           /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_rgain_wdata;

/* Define the union u_isp_mlsc_rgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_rgain_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_rgain_raddr;
/* Define the union u_isp_mlsc_rgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_rgain_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;           /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_rgain_rdata;

/* Define the union u_isp_mlsc_grgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_grgain_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_grgain_waddr;
/* Define the union u_isp_mlsc_grgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_grgain_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;            /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_grgain_wdata;

/* Define the union u_isp_mlsc_grgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_grgain_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_grgain_raddr;
/* Define the union u_isp_mlsc_grgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_grgain_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;            /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_grgain_rdata;

/* Define the union u_isp_mlsc_bgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_bgain_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_bgain_waddr;
/* Define the union u_isp_mlsc_bgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_bgain_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;           /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_bgain_wdata;

/* Define the union u_isp_mlsc_bgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_bgain_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_bgain_raddr;
/* Define the union u_isp_mlsc_bgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_bgain_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;           /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_bgain_rdata;

/* Define the union u_isp_mlsc_gbgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_gbgain_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_gbgain_waddr;
/* Define the union u_isp_mlsc_gbgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_gbgain_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;            /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_gbgain_wdata;

/* Define the union u_isp_mlsc_gbgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mlsc_gbgain_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_gbgain_raddr;
/* Define the union u_isp_mlsc_gbgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_gbgain_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;            /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_gbgain_rdata;

/* Define the union u_isp_mlsc_winx16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax16 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx16 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx16;

/* Define the union u_isp_mlsc_winx17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax17 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx17 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx17;

/* Define the union u_isp_mlsc_winx18 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax18 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx18 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx18;

/* Define the union u_isp_mlsc_winx19 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax19 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx19 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx19;

/* Define the union u_isp_mlsc_winx20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax20 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx20 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx20;

/* Define the union u_isp_mlsc_winx21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax21 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx21 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx21;

/* Define the union u_isp_mlsc_winx22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax22 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx22 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx22;

/* Define the union u_isp_mlsc_winx23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax23 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx23 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx23;

/* Define the union u_isp_mlsc_winx24 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax24 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx24 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx24;

/* Define the union u_isp_mlsc_winx25 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax25 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx25 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx25;

/* Define the union u_isp_mlsc_winx26 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax26 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx26 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx26;

/* Define the union u_isp_mlsc_winx27 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax27 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx27 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx27;

/* Define the union u_isp_mlsc_winx28 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax28 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx28 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx28;

/* Define the union u_isp_mlsc_winx29 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax29 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx29 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx29;

/* Define the union u_isp_mlsc_winx30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax30 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx30 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx30;

/* Define the union u_isp_mlsc_winx31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltax31 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_mlsc_invx31 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winx31;

/* Define the union u_isp_mlsc_winy8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_mlsc_deltay8 : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;       /* [15..8]  */
        unsigned int isp_mlsc_invy8 : 11;  /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_mlsc_winy8;

/* Define the union u_isp_wdr_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;               /* [0]  */
        unsigned int isp_wdr_fusion_data_mode : 1; /* [1]  */
        unsigned int reserved_1 : 2;               /* [3..2]  */
        unsigned int isp_wdr_fusionmode : 1;       /* [4]  */
        unsigned int reserved_2 : 11;              /* [15..5]  */
        unsigned int isp_wdr_erosion_en : 1;       /* [16]  */
        unsigned int reserved_3 : 3;               /* [19..17]  */
        unsigned int isp_wdr_mdt_en : 1;           /* [20]  */
        unsigned int reserved_4 : 3;               /* [23..21]  */
        unsigned int isp_wdr_grayscale_mode : 1;   /* [24]  */
        unsigned int reserved_5 : 7;               /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_ctrl;

/* Define the union u_isp_wdr_f0_inblc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f0_inblc_gr : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;           /* [15..13]  */
        unsigned int isp_wdr_f0_inblc_r : 13;  /* [28..16]  */
        unsigned int reserved_1 : 3;           /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_f0_inblc0;

/* Define the union u_isp_wdr_f0_inblc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f0_inblc_b : 13;  /* [12..0]  */
        unsigned int reserved_0 : 3;           /* [15..13]  */
        unsigned int isp_wdr_f0_inblc_gb : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;           /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_f0_inblc1;

/* Define the union u_isp_wdr_f1_inblc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f1_inblc_gr : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;           /* [15..13]  */
        unsigned int isp_wdr_f1_inblc_r : 13;  /* [28..16]  */
        unsigned int reserved_1 : 3;           /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_f1_inblc0;

/* Define the union u_isp_wdr_f1_inblc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f1_inblc_b : 13;  /* [12..0]  */
        unsigned int reserved_0 : 3;           /* [15..13]  */
        unsigned int isp_wdr_f1_inblc_gb : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;           /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_f1_inblc1;

/* Define the union u_isp_wdr_outblc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_outblc : 18;  /* [17..0]  */
        unsigned int isp_wdr_bsaveblc : 1; /* [18]  */
        unsigned int reserved_0 : 13;      /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_outblc;

/* Define the union u_isp_wdr_expovalue */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_expovalue1    : 11; /* [10..0]  */
        unsigned int    reserved_0            : 5; /* [15..11]  */
        unsigned int    isp_wdr_expovalue0    : 11; /* [26..16]  */
        unsigned int    reserved_1            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_expovalue;

/* Define the union u_isp_wdr_exporratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_exporratio0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;          /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_exporratio;

/* Define the union u_isp_wdr_blc_comp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_blc_comp0 : 26; /* [25..0]  */
        unsigned int reserved_0 : 6;         /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_blc_comp;

/* Define the union u_isp_wdr_maxratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_maxratio      : 13; /* [12..0]  */
        unsigned int    reserved_0            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_maxratio;

/* Define the union u_isp_wdr_saturate_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_saturate_thr : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;           /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_saturate_thr;

/* Define the union u_isp_wdr_still_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_f0_still_thr : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;           /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_still_thr;

/* Define the union u_isp_wdr_mdtlbld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_mdtlbld : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;     /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_mdtlbld;

/* Define the union u_isp_wdr_mdt_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_mdt_full_thr : 8;  /* [7..0]  */
        unsigned int reserved_0 : 8;            /* [15..8]  */
        unsigned int isp_wdr_mdt_still_thr : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;            /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_mdt_thr;

/* Define the union u_isp_wdr_forcelong_para */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_forcelong_high_thd : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;                  /* [15..12]  */
        unsigned int isp_wdr_forcelong_low_thd : 12;  /* [27..16]  */
        unsigned int isp_wdr_forcelong_smooth_en : 1; /* [28]  */
        unsigned int isp_wdr_forcelong_en : 1;        /* [29]  */
        unsigned int reserved_1 : 2;                  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_forcelong_para;

/* Define the union u_isp_wdr_forcelong_slope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_forcelong_slope : 10;   /* [9..0]  */
        unsigned int reserved_0 : 22;                /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_forcelong_slope;

/* Define the union u_isp_wdr_mdt_nosf_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_mdt_nosf_hig_thr : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;                /* [15..11]  */
        unsigned int isp_wdr_mdt_nosf_low_thr : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;                /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_mdt_nosf_thr;

/* Define the union u_isp_wdr_gain_sum_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_gain_sum_hig_thr : 8; /* [7..0]  */
        unsigned int reserved_0 : 8;               /* [15..8]  */
        unsigned int isp_wdr_gain_sum_low_thr : 8; /* [23..16]  */
        unsigned int reserved_1 : 8;               /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_gain_sum_thr;

/* Define the union u_isp_wdr_shortexpo_chk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_shortchk_thd : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;            /* [15..12]  */
        unsigned int isp_wdr_shortexpo_chk : 1; /* [16]  */
        unsigned int reserved_1 : 15;           /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_shortexpo_chk;

/* Define the union u_isp_wdr_pix_avg_diff */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_pixel_avg_max_diff : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;                 /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_pix_avg_diff;

/* Define the union u_isp_wdr_mask_similar */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_mask_similar_cnt : 4; /* [3..0]  */
        unsigned int reserved_0 : 12;              /* [15..4]  */
        unsigned int isp_wdr_mask_similar_thr : 6; /* [21..16]  */
        unsigned int reserved_1 : 10;              /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_mask_similar;

/* Define the union u_isp_wdr_wgtidx_blendratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_bldrlhfidx : 5; /* [4..0]  */
        unsigned int reserved_0 : 27;        /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_wgtidx_blendratio;

/* Define the union u_isp_wdr_wgtidx_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_long_thr : 12;  /* [11..0]  */
        unsigned int reserved_0 : 4;         /* [15..12]  */
        unsigned int isp_wdr_short_thr : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;         /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_wgtidx_thr;

/* Define the union u_isp_wdr_dftwgt_fl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_dftwgt_fl : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;       /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_dftwgt_fl;

/* Define the union u_isp_wdr_wgt_slope */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_wgt_slope : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;        /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_wgt_slope;

/* Define the union u_isp_wdr_fusion_thr_r */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_f1_thr_r : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;               /* [15..12]  */
        unsigned int isp_wdr_fusion_f0_thr_r : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;               /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_thr_r;

/* Define the union u_isp_wdr_fusion_thr_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_f1_thr_b : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;               /* [15..12]  */
        unsigned int isp_wdr_fusion_f0_thr_b : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;               /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_thr_b;

/* Define the union u_isp_wdr_fusion_thr_g */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_f1_thr_g : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;               /* [15..12]  */
        unsigned int isp_wdr_fusion_f0_thr_g : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;               /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_thr_g;

/* Define the union u_isp_wdr_fusion_sat_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_wdr_fusion_saturate_thd : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;                  /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_wdr_fusion_sat_thd;

/* Define the union u_isp_dehaze_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_version;
/* Define the union u_isp_dehaze_blk_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_block_sizev : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;             /* [15..9]  */
        unsigned int isp_dehaze_block_sizeh : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;             /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_blk_size;

/* Define the union u_isp_dehaze_blk_sum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_block_sum : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;           /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_blk_sum;

/* Define the union u_isp_dehaze_dc_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_dc_numv : 5; /* [4..0]  */
        unsigned int isp_dehaze_dc_numh : 5; /* [9..5]  */
        unsigned int reserved_0 : 22;        /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_dc_size;

/* Define the union u_isp_dehaze_x */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_phase_x : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;          /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_x;

/* Define the union u_isp_dehaze_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_phase_y : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;          /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_y;

/* Define the union u_isp_dehaze_air */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_air_b : 10; /* [9..0]  */
        unsigned int isp_dehaze_air_g : 10; /* [19..10]  */
        unsigned int isp_dehaze_air_r : 10; /* [29..20]  */
        unsigned int reserved_0 : 2;        /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_air;

/* Define the union u_isp_dehaze_thld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_thld_tb : 10; /* [9..0]  */
        unsigned int isp_dehaze_thld_tg : 10; /* [19..10]  */
        unsigned int isp_dehaze_thld_tr : 10; /* [29..20]  */
        unsigned int reserved_0 : 2;          /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_thld;

/* Define the union u_isp_dehaze_gstrth */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_gstrth : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;       /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_gstrth;

/* Define the union u_isp_dehaze_blthld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_blthld : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;        /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_blthld;

/* Define the union u_isp_dehaze_stt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_stt_size : 16; /* [15..0]  */
        unsigned int isp_dehaze_stt_bst : 4;   /* [19..16]  */
        unsigned int isp_dehaze_stt_en : 1;    /* [20]  */
        unsigned int reserved_0 : 11;          /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stt_bst;

/* Define the union u_isp_dehaze_stt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_stt_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;          /* [30..16]  */
        unsigned int isp_dehaze_stt_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stt_abn;

/* Define the union u_isp_dehaze_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;           /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stt2lut_cfg;

/* Define the union u_isp_dehaze_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;               /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stt2lut_regnew;

/* Define the union u_isp_dehaze_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;              /* [30..16]  */
        unsigned int isp_dehaze_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stt2lut_abn;

/* Define the union u_isp_dehaze_smlmapoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_smlmapoffset : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;              /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_smlmapoffset;

/* Define the union u_isp_dehaze_stat_point */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_statstartx : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;             /* [15..13]  */
        unsigned int isp_dehaze_statendx : 13;   /* [28..16]  */
        unsigned int reserved_1 : 3;             /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stat_point;

/* Define the union u_isp_dehaze_stat_num */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_stat_numv : 5; /* [4..0]  */
        unsigned int isp_dehaze_stat_numh : 5; /* [9..5]  */
        unsigned int reserved_0 : 22;          /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_stat_num;

/* Define the union u_isp_dehaze_minstat_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_minstat_waddr : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;              /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_minstat_waddr;

/* Define the union u_isp_dehaze_minstat_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_minstat_wdata_l : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                  /* [15..10]  */
        unsigned int isp_dehaze_minstat_wdata_h : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;                  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_minstat_wdata;

/* Define the union u_isp_dehaze_minstat_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_minstat_raddr : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;              /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_minstat_raddr;

/* Define the union u_isp_dehaze_minstat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_minstat_rdata_l : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                  /* [15..10]  */
        unsigned int isp_dehaze_minstat_rdata_h : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;                  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_minstat_rdata;

/* Define the union u_isp_dehaze_maxstat_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_maxstat_waddr : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;               /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_maxstat_waddr;

/* Define the union u_isp_dehaze_maxstat_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_maxstat_wdata : 30; /* [29..0]  */
        unsigned int reserved_0 : 2;                /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_maxstat_wdata;

/* Define the union u_isp_dehaze_maxstat_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_maxstat_raddr : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;               /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_maxstat_raddr;

/* Define the union u_isp_dehaze_maxstat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_maxstat_rdata : 30; /* [29..0]  */
        unsigned int reserved_0 : 2;                /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_maxstat_rdata;

/* Define the union u_isp_dehaze_prestat_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_prestat_waddr : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;               /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_prestat_waddr;

/* Define the union u_isp_dehaze_prestat_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_prestat_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;               /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_prestat_wdata;

/* Define the union u_isp_dehaze_prestat_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_prestat_raddr : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;               /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_prestat_raddr;

/* Define the union u_isp_dehaze_prestat_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_prestat_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;               /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_prestat_rdata;

/* Define the union u_isp_dehaze_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut_waddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_lut_waddr;

/* Define the union u_isp_dehaze_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut_wdata : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_lut_wdata;

/* Define the union u_isp_dehaze_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_lut_raddr;

/* Define the union u_isp_dehaze_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_dehaze_lut_rdata : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_dehaze_lut_rdata;

/* Define the union u_isp_expander_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_version;
/* Define the union u_isp_expander_bitw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_expander_bitw_out : 5; /* [4..0]  */
        unsigned int reserved_0 : 11;           /* [15..5]  */
        unsigned int isp_expander_bitw_in : 5;  /* [20..16]  */
        unsigned int reserved_1 : 11;           /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_bitw;

/* Define the union u_isp_expander_lut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_waddr : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;            /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_lut_waddr;
/* Define the union u_isp_expander_lut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_wdata : 18; /* [17..0]  */
        unsigned int reserved_0 : 14;            /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_lut_wdata;
/* Define the union u_isp_expander_lut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_raddr : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;            /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_lut_raddr;
/* Define the union u_isp_expander_lut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_expander_lut_rdata : 18; /* [17..0]  */
        unsigned int reserved_0 : 14;            /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_expander_lut_rdata;
/* Define the union u_isp_gamma_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;  /* [0]  */
        unsigned int reserved_1 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_cfg;

/* Define the union u_isp_gamma_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_pos_0 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int isp_gamma_pos_1 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_pos0;

/* Define the union u_isp_gamma_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_pos_2 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int isp_gamma_pos_3 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_pos1;

/* Define the union u_isp_gamma_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_pos_4 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int isp_gamma_pos_5 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_pos2;

/* Define the union u_isp_gamma_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_pos_6 : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int isp_gamma_pos_7 : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_pos3;

/* Define the union u_isp_gamma_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_inseg_0 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;        /* [15..9]  */
        unsigned int isp_gamma_inseg_1 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_inseg0;

/* Define the union u_isp_gamma_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_inseg_2 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;        /* [15..9]  */
        unsigned int isp_gamma_inseg_3 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_inseg1;

/* Define the union u_isp_gamma_inseg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_inseg_4 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;        /* [15..9]  */
        unsigned int isp_gamma_inseg_5 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_inseg2;

/* Define the union u_isp_gamma_inseg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_inseg_6 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;        /* [15..9]  */
        unsigned int isp_gamma_inseg_7 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_inseg3;

/* Define the union u_isp_gamma_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_step0 : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;      /* [3]  */
        unsigned int isp_gamma_step1 : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;      /* [7]  */
        unsigned int isp_gamma_step2 : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;      /* [11]  */
        unsigned int isp_gamma_step3 : 3; /* [14..12]  */
        unsigned int reserved_3 : 1;      /* [15]  */
        unsigned int isp_gamma_step4 : 3; /* [18..16]  */
        unsigned int reserved_4 : 1;      /* [19]  */
        unsigned int isp_gamma_step5 : 3; /* [22..20]  */
        unsigned int reserved_5 : 1;      /* [23]  */
        unsigned int isp_gamma_step6 : 3; /* [26..24]  */
        unsigned int reserved_6 : 1;      /* [27]  */
        unsigned int isp_gamma_step7 : 3; /* [30..28]  */
        unsigned int reserved_7 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_step;

/* Define the union u_isp_gamma_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;          /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_stt2lut_cfg;

/* Define the union u_isp_gamma_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;              /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_stt2lut_regnew;

/* Define the union u_isp_gamma_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_gamma_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;             /* [30..16]  */
        unsigned int isp_gamma_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_stt2lut_abn;

/* Define the union u_isp_gamma_lut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_lut_waddr;
/* Define the union u_isp_gamma_lut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_lut_wdata;
/* Define the union u_isp_gamma_lut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_lut_raddr;
/* Define the union u_isp_gamma_lut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_gamma_lut_rdata;


/* Define the union u_isp_bcom_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_version;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_step0    : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_bcom_lut_step1    : 4; /* [11..8]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    isp_bcom_lut_step2    : 4; /* [19..16]  */
        unsigned int    reserved_2            : 4; /* [23..20]  */
        unsigned int    isp_bcom_lut_step3    : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_step0;

/* Define the union u_isp_bcom_lut_step1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_step4    : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_bcom_lut_step5    : 4; /* [11..8]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    isp_bcom_lut_step6    : 4; /* [19..16]  */
        unsigned int    reserved_2            : 4; /* [23..20]  */
        unsigned int    isp_bcom_lut_step7    : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_step1;

/* Define the union u_isp_bcom_lut_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_inseg0   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 2; /* [10..9]  */
        unsigned int    isp_bcom_lut_inseg1   : 9; /* [19..11]  */
        unsigned int    reserved_1            : 2; /* [21..20]  */
        unsigned int    isp_bcom_lut_inseg2   : 9; /* [30..22]  */
        unsigned int    reserved_2            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_inseg0;

/* Define the union u_isp_bcom_lut_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_inseg3   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 2; /* [10..9]  */
        unsigned int    isp_bcom_lut_inseg4   : 9; /* [19..11]  */
        unsigned int    reserved_1            : 2; /* [21..20]  */
        unsigned int    isp_bcom_lut_inseg5   : 9; /* [30..22]  */
        unsigned int    reserved_2            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_inseg1;

/* Define the union u_isp_bcom_lut_inseg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_inseg6   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 2; /* [10..9]  */
        unsigned int    isp_bcom_lut_inseg7   : 9; /* [19..11]  */
        unsigned int    reserved_1            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_inseg2;

/* Define the union u_isp_bcom_lut_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos0     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos0;

/* Define the union u_isp_bcom_lut_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos1     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos1;

/* Define the union u_isp_bcom_lut_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos2     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos2;

/* Define the union u_isp_bcom_lut_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos3     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos3;

/* Define the union u_isp_bcom_lut_pos4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos4     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos4;

/* Define the union u_isp_bcom_lut_pos5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos5     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos5;

/* Define the union u_isp_bcom_lut_pos6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos6     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos6;

/* Define the union u_isp_bcom_lut_pos7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_lut_pos7     : 18; /* [17..0]  */
        unsigned int    reserved_0            : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bcom_lut_pos7;

/* Define the union u_isp_bcom_gamma_lut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_gamma_lut_waddr : 9;  /* [8..0]  */
        unsigned int reserved_0 : 23;               /* [31..9]  */
    } bits;

    /* Define an unsigned member */
        unsigned int u32;
} u_isp_bcom_gamma_lut_waddr;
/* Define the union u_isp_bcom_gamma_lut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_gamma_lut_wdata : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;               /* [31..13]  */
    } bits;

    /* Define an unsigned member */
        unsigned int u32;
} u_isp_bcom_gamma_lut_wdata;
/* Define the union u_isp_bcom_gamma_lut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_gamma_lut_raddr : 9;  /* [8..0]  */
        unsigned int reserved_0 : 23;               /* [31..9]  */
    } bits;

    /* Define an unsigned member */
        unsigned int u32;
} u_isp_bcom_gamma_lut_raddr;
/* Define the union u_isp_bcom_gamma_lut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_gamma_lut_rdata : 13; /* [12..0]  */
        unsigned int reserved_0 : 19;               /* [31..13]  */
    } bits;

    /* Define an unsigned member */
        unsigned int u32;
} u_isp_bcom_gamma_lut_rdata;

/* Define the union u_isp_bdec_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bdec_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_version;

/* Define the union u_isp_bdec_lut_step0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_step0    : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_bdec_lut_step1    : 4; /* [11..8]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    isp_bdec_lut_step2    : 4; /* [19..16]  */
        unsigned int    reserved_2            : 4; /* [23..20]  */
        unsigned int    isp_bdec_lut_step3    : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_step0;

/* Define the union u_isp_bdec_lut_step1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_step4    : 4; /* [3..0]  */
        unsigned int    reserved_0            : 4; /* [7..4]  */
        unsigned int    isp_bdec_lut_step5    : 4; /* [11..8]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    isp_bdec_lut_step6    : 4; /* [19..16]  */
        unsigned int    reserved_2            : 4; /* [23..20]  */
        unsigned int    isp_bdec_lut_step7    : 4; /* [27..24]  */
        unsigned int    reserved_3            : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_step1;

/* Define the union u_isp_bdec_lut_inseg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_inseg0   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 2; /* [10..9]  */
        unsigned int    isp_bdec_lut_inseg1   : 9; /* [19..11]  */
        unsigned int    reserved_1            : 2; /* [21..20]  */
        unsigned int    isp_bdec_lut_inseg2   : 9; /* [30..22]  */
        unsigned int    reserved_2            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_inseg0;

/* Define the union u_isp_bdec_lut_inseg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_inseg3   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 2; /* [10..9]  */
        unsigned int    isp_bdec_lut_inseg4   : 9; /* [19..11]  */
        unsigned int    reserved_1            : 2; /* [21..20]  */
        unsigned int    isp_bdec_lut_inseg5   : 9; /* [30..22]  */
        unsigned int    reserved_2            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_inseg1;

/* Define the union u_isp_bdec_lut_inseg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_inseg6   : 9; /* [8..0]  */
        unsigned int    reserved_0            : 2; /* [10..9]  */
        unsigned int    isp_bdec_lut_inseg7   : 9; /* [19..11]  */
        unsigned int    reserved_1            : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_inseg2;

/* Define the union u_isp_bdec_lut_pos0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_pos0     : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_bdec_lut_pos1     : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_pos0;

/* Define the union u_isp_bdec_lut_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_pos2     : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_bdec_lut_pos3     : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_pos1;

/* Define the union u_isp_bdec_lut_pos2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_pos4     : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_bdec_lut_pos5     : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_pos2;

/* Define the union u_isp_bdec_lut_pos3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_lut_pos6     : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_bdec_lut_pos7     : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_bdec_lut_pos3;

/* Define the union u_isp_bdec_gamma_lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_gamma_lut_waddr : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_gamma_lut_waddr;

/* Define the union u_isp_bdec_gamma_lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_gamma_lut_wdata : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_gamma_lut_wdata;

/* Define the union u_isp_bdec_gamma_lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_gamma_lut_raddr : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_gamma_lut_raddr;

/* Define the union u_isp_bdec_gamma_lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_gamma_lut_rdata : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_bdec_gamma_lut_rdata;


/* Define the union u_isp_ca_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_satvssat_en : 1; /* [0]  */
        unsigned int reserved_0 : 3;         /* [3..1]  */
        unsigned int isp_ca_skinproc_en : 1; /* [4]  */
        unsigned int reserved_1 : 3;         /* [7..5]  */
        unsigned int isp_ca_llhcproc_en : 1; /* [8]  */
        unsigned int reserved_2 : 7;         /* [15..9]  */
        unsigned int isp_ca_satadj_en : 1;   /* [16]  */
        unsigned int reserved_3 : 15;        /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ctrl;

/* Define the union u_isp_ca_des */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_des_mix : 8;    /* [7..0]  */
        unsigned int reserved_0 : 8;        /* [15..8]  */
        unsigned int isp_ca_des_point : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;        /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_des;

/* Define the union u_isp_ca_isoratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_isoratio : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;      /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_isoratio;

/* Define the union u_isp_ca_sat_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_sat_c1 : 7; /* [6..0]  */
        unsigned int reserved_0 : 9;    /* [15..7]  */
        unsigned int isp_ca_sat_c2 : 7; /* [22..16]  */
        unsigned int reserved_1 : 9;    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_sat_coef;

/* Define the union u_isp_ca_yratiolut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yratiolut_waddr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yratiolut_waddr;

/* Define the union u_isp_ca_yratiolut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yratiolut_wdata : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;              /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yratiolut_wdata;

/* Define the union u_isp_ca_yratiolut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yratiolut_raddr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yratiolut_raddr;

/* Define the union u_isp_ca_yratiolut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yratiolut_rdata : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;              /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yratiolut_rdata;

/* Define the union u_isp_ca_ysatlut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ysatlut_waddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ysatlut_waddr;

/* Define the union u_isp_ca_ysatlut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ysatlut_wdata : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;           /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ysatlut_wdata;

/* Define the union u_isp_ca_ysatlut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ysatlut_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ysatlut_raddr;

/* Define the union u_isp_ca_ysatlut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ysatlut_rdata : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;           /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ysatlut_rdata;

/* Define the union u_isp_ca_ych1lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych1lut_waddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych1lut_waddr;

/* Define the union u_isp_ca_ych1lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych1lut_even_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_ych1lut_odd_wdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych1lut_wdata;

/* Define the union u_isp_ca_ych1lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych1lut_raddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych1lut_raddr;

/* Define the union u_isp_ca_ych1lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych1lut_even_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_ych1lut_odd_rdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych1lut_rdata;

/* Define the union u_isp_ca_ych2lut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych2lut_waddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych2lut_waddr;

/* Define the union u_isp_ca_ych2lut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych2lut_even_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_ych2lut_odd_wdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych2lut_wdata;

/* Define the union u_isp_ca_ych2lut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych2lut_raddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych2lut_raddr;

/* Define the union u_isp_ca_ych2lut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_ych2lut_even_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_ych2lut_odd_rdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_ych2lut_rdata;

/* Define the union u_isp_ca_yslulut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslulut_waddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslulut_waddr;

/* Define the union u_isp_ca_yslulut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslulut_even_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_yslulut_odd_wdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslulut_wdata;

/* Define the union u_isp_ca_yslulut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslulut_raddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslulut_raddr;

/* Define the union u_isp_ca_yslulut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslulut_even_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_yslulut_odd_rdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslulut_rdata;

/* Define the union u_isp_ca_yslvlut_waddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslvlut_waddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslvlut_waddr;

/* Define the union u_isp_ca_yslvlut_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslvlut_even_wdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_yslvlut_odd_wdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslvlut_wdata;

/* Define the union u_isp_ca_yslvlut_raddr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslvlut_raddr : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslvlut_raddr;

/* Define the union u_isp_ca_yslvlut_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_yslvlut_even_rdata : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;                 /* [15..10]  */
        unsigned int isp_ca_yslvlut_odd_rdata : 10;  /* [25..16]  */
        unsigned int reserved_1 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_yslvlut_rdata;

/* Define the union u_isp_ca_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_stt2lut_cfg;

/* Define the union u_isp_ca_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;           /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_stt2lut_regnew;

/* Define the union u_isp_ca_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ca_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;          /* [30..16]  */
        unsigned int isp_ca_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ca_stt2lut_abn;

/* Define the union u_isp_ldci_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;         /* [0]  */
        unsigned int isp_ldci_calc_en : 1;   /* [1]  */
        unsigned int isp_ldci_wrstat_en : 1; /* [2]  */
        unsigned int isp_ldci_rdstat_en : 1; /* [3]  */
        unsigned int isp_ldci_lpf_en : 1;    /* [4]  */
        unsigned int reserved_1 : 27;        /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_cfg;

/* Define the union u_isp_ldci_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_version;

/* Define the union u_isp_ldci_stat_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_stat_offset : 5; /* [4..0]  */
        unsigned int reserved_0 : 27;          /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stat_offset;

/* Define the union u_isp_ldci_calc_small_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_calc_map_offsetx : 20; /* [19..0]  */
        unsigned int reserved_0 : 12;                /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_calc_small_offset;

/* Define the union u_isp_ldci_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_smlmapstride : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;            /* [7..6]  */
        unsigned int isp_ldci_smlmapheight : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;            /* [15..14]  */
        unsigned int isp_ldci_total_zone : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_zone;

/* Define the union u_isp_ldci_lpfstt_bst */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpfstt_size : 16; /* [15..0]  */
        unsigned int isp_ldci_lpfstt_bst : 4;   /* [19..16]  */
        unsigned int isp_ldci_lpfstt_en : 1;    /* [20]  */
        unsigned int reserved_0 : 11;           /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpfstt_bst;

/* Define the union u_isp_ldci_lpfstt_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpfstt_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;           /* [30..16]  */
        unsigned int isp_ldci_lpfstt_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpfstt_abn;

/* Define the union u_isp_ldci_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_scalex : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;       /* [15]  */
        unsigned int isp_ldci_scaley : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;       /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_scale;

/* Define the union u_isp_ldci_luma_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_luma_sel : 1; /* [0]  */
        unsigned int reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_luma_sel;

/* Define the union u_isp_ldci_blc_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_blc_ctrl : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;       /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_blc_ctrl;

/* Define the union u_isp_ldci_lpf_lpfcoef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpfcoef0 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;        /* [15..9]  */
        unsigned int isp_ldci_lpfcoef1 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_lpfcoef0;

/* Define the union u_isp_ldci_lpf_lpfcoef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpfcoef2 : 9; /* [8..0]  */
        unsigned int reserved_0 : 7;        /* [15..9]  */
        unsigned int isp_ldci_lpfcoef3 : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_lpfcoef1;

/* Define the union u_isp_ldci_lpf_lpfcoef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpfcoef4 : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;       /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_lpfcoef2;

/* Define the union u_isp_ldci_lpf_lpfsft */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpfsft : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;     /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_lpfsft;

/* Define the union u_isp_ldci_he_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_he_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_he_waddr;
/* Define the union u_isp_ldci_he_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_hepos_wdata : 9; /* [8..0]  */
        unsigned int isp_ldci_heneg_wdata : 9; /* [17..9]  */
        unsigned int reserved_0 : 14;          /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_he_wdata;

/* Define the union u_isp_ldci_he_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_he_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_he_raddr;
/* Define the union u_isp_ldci_he_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_hepos_rdata : 9; /* [8..0]  */
        unsigned int isp_ldci_heneg_rdata : 9; /* [17..9]  */
        unsigned int reserved_0 : 14;          /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_he_rdata;

/* Define the union u_isp_ldci_cgain_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_cgain_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_cgain_waddr;
/* Define the union u_isp_ldci_cgain_wdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_cgain_wdata : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;           /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_cgain_wdata;

/* Define the union u_isp_ldci_cgain_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_cgain_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_cgain_raddr;
/* Define the union u_isp_ldci_cgain_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_cgain_rdata : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;           /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_cgain_rdata;

/* Define the union u_isp_ldci_stat_hpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_hstart : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int isp_ldci_hend : 13;   /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stat_hpos;

/* Define the union u_isp_ldci_stat_vpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_vstart : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int isp_ldci_vend : 13;   /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stat_vpos;

/* Define the union u_isp_ldci_stat_evratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_stat_evratio : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stat_evratio;

/* Define the union u_isp_ldci_stat_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_stat_smlmapwidth : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2;                 /* [7..6]  */
        unsigned int isp_ldci_stat_smlmapheight : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;                 /* [15..14]  */
        unsigned int isp_ldci_stat_total_zone : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stat_zone;

/* Define the union u_isp_ldci_blk_smlmapwidth0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_blk_smlmapwidth0 : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;               /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_blk_smlmapwidth0;

/* Define the union u_isp_ldci_blk_smlmapwidth1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_blk_smlmapwidth1 : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;               /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_blk_smlmapwidth1;

/* Define the union u_isp_ldci_blk_smlmapwidth2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_blk_smlmapwidth2 : 6; /* [5..0]  */
        unsigned int reserved_0 : 26;               /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_blk_smlmapwidth2;

/* Define the union u_isp_ldci_chrdamp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_chrposdamp : 8; /* [7..0]  */
        unsigned int isp_ldci_chrnegdamp : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;         /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_chrdamp;

/* Define the union u_isp_ldci_stt2lut_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_stt2lut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stt2lut_cfg;

/* Define the union u_isp_ldci_stt2lut_regnew */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_stt2lut_regnew : 1; /* [0]  */
        unsigned int reserved_0 : 31;             /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stt2lut_regnew;

/* Define the union u_isp_ldci_stt2lut_abn */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_stt2lut_info : 16; /* [15..0]  */
        unsigned int reserved_0 : 15;            /* [30..16]  */
        unsigned int isp_ldci_stt2lut_clr : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_stt2lut_abn;

/* Define the union u_isp_ldci_lpf_map_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_map_waddr;
/* Define the union u_isp_ldci_lpf_map_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0 : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_map_wdata;
/* Define the union u_isp_ldci_lpf_map_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lpfmap_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_map_raddr;
/* Define the union u_isp_ldci_lpf_map_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_ldci_lpflitmap_rdata : 10;   /* [9..0]  */
        unsigned int isp_ldci_lpfplyp2map_rdata : 10; /* [19..10]  */
        unsigned int isp_ldci_lpfplyp3map_rdata : 10; /* [29..20]  */
        unsigned int reserved_0 : 2;                  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_ldci_lpf_map_rdata;

/* Define the union u_isp_drc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_drc_wrstat_en     : 1   ; /* [1]  */
        unsigned int    isp_drc_rdstat_en     : 1   ; /* [2]  */
        unsigned int    isp_drc_vbiflt_en     : 1   ; /* [3]  */
        unsigned int    isp_drc_color_corr_en : 1   ; /* [4]  */
        unsigned int    reserved_1            : 1   ; /* [5]  */
        unsigned int    reserved_2            : 1   ; /* [6]  */
        unsigned int    isp_drc_mono_chroma_en : 1   ; /* [7]  */
        unsigned int    isp_drc_flt_en        : 1   ; /* [8]  */
        unsigned int    isp_drc_dp_det_en     : 1   ; /* [9]  */
        unsigned int    isp_drc_inadpt_en     : 1   ; /* [10]  */
        unsigned int    isp_drc_line_detect_en : 1   ; /* [11]  */
        unsigned int    isp_drc_nr_en         : 1   ; /* [12]  */
        unsigned int    isp_drc_inner_bypass_en : 1   ; /* [13]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    isp_drc_strp_en       : 1   ; /* [16]  */
        unsigned int    reserved_4            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_cfg;

/* Define the union u_isp_drc_lut_update0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_lut_update0 : 1; /* [0]  */
        unsigned int isp_drc_lut_update1 : 1; /* [1]  */
        unsigned int isp_drc_lut_update2 : 1; /* [2]  */
        unsigned int reserved_0 : 29;         /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_lut_update0;

/* Define the union u_isp_drc_lut_update1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;          /* [0]  */
        unsigned int reserved_1 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_lut_update1;

/* Define the union u_isp_drc_version */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_version : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_version;
/* Define the union u_isp_drc_zone */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_hnum : 6; /* [5..0]  */
        unsigned int reserved_0 : 2;   /* [7..6]  */
        unsigned int isp_drc_vnum : 6; /* [13..8]  */
        unsigned int reserved_1 : 18;  /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_zone;

/* Define the union u_isp_drc_zone_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_zone_hsize : 9; /* [8..0]  */
        unsigned int isp_drc_chk_x : 6;      /* [14..9]  */
        unsigned int reserved_0 : 1;         /* [15]  */
        unsigned int isp_drc_zone_vsize : 9; /* [24..16]  */
        unsigned int isp_drc_chk_y : 6;      /* [30..25]  */
        unsigned int reserved_1 : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_zone_size;

/* Define the union u_isp_drc_zone_div0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_div_x0 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_drc_div_y0 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_zone_div0;

/* Define the union u_isp_drc_zone_div1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_div_x1 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1;      /* [15]  */
        unsigned int isp_drc_div_y1 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_zone_div1;

/* Define the union u_isp_drc_bin */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_bin : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bin;

/* Define the union u_isp_drc_zone_init */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_acc_x_init : 16; /* [15..0]  */
        unsigned int isp_drc_cnt_x_init : 9;  /* [24..16]  */
        unsigned int isp_drc_idx_x_init : 6;  /* [30..25]  */
        unsigned int isp_drc_big_x_init : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_zone_init;

/* Define the union u_isp_drc_bin_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_bin_scale : 3; /* [2..0]  */
        unsigned int reserved_0 : 29;       /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bin_scale;

/* Define the union u_isp_drc_gain_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_colorgain_thr : 8;     /* [7..0]  */
        unsigned int isp_drc_colorgain_slope : 8;   /* [15..8]  */
        unsigned int isp_drc_detailgain_slope : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8;                /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_gain_cfg;

/* Define the union u_isp_drc_strength */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_strength : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;       /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_strength;

/* Define the union u_isp_drc_stat_width */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_stat_width : 16;  /* [15..0]  */
        unsigned int isp_drc_total_width : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_stat_width;

/* Define the union u_isp_drc_dark_gain_lmt_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_val1_y : 8; /* [7..0]  */
        unsigned int isp_drc_sft1_y : 4; /* [11..8]  */
        unsigned int isp_drc_val2_y : 4; /* [15..12]  */
        unsigned int isp_drc_sft2_y : 4; /* [19..16]  */
        unsigned int reserved_0 : 12;    /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_dark_gain_lmt_y;

/* Define the union u_isp_drc_dark_gain_lmt_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_val1_c : 8; /* [7..0]  */
        unsigned int isp_drc_sft1_c : 4; /* [11..8]  */
        unsigned int isp_drc_val2_c : 4; /* [15..12]  */
        unsigned int isp_drc_sft2_c : 4; /* [19..16]  */
        unsigned int reserved_0 : 12;    /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_dark_gain_lmt_c;

/* Define the union u_isp_drc_bright_gain_lmt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_val : 8; /* [7..0]  */
        unsigned int isp_drc_sft : 4; /* [11..8]  */
        unsigned int reserved_0 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bright_gain_lmt;

/* Define the union u_isp_drc_rgb_wgt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_r_wgt : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;    /* [7..4]  */
        unsigned int isp_drc_g_wgt : 5; /* [12..8]  */
        unsigned int reserved_1 : 3;    /* [15..13]  */
        unsigned int isp_drc_b_wgt : 4; /* [19..16]  */
        unsigned int reserved_2 : 12;   /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_rgb_wgt;

/* Define the union u_isp_drc_dp_dtc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_dp_det_thrmin : 16; /* [15..0]  */
        unsigned int isp_drc_dp_det_thrslo : 5;  /* [20..16]  */
        unsigned int isp_drc_dp_det_g2rb : 1;    /* [21]  */
        unsigned int isp_drc_dp_det_rb2rb : 1;   /* [22]  */
        unsigned int isp_drc_dp_det_replctr : 1; /* [23]  */
        unsigned int isp_drc_dp_det_rngrto : 5;  /* [28..24]  */
        unsigned int reserved_0 : 3;             /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_dp_dtc;

/* Define the union u_isp_drc_gain_clip */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_gain_clip_step : 4; /* [3..0]  */
        unsigned int isp_drc_gain_clip_knee : 4; /* [7..4]  */
        unsigned int reserved_0 : 24;            /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_gain_clip;

/* Define the union u_isp_drc_color_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_cc_lut_ctrl : 1; /* [0]  */
        unsigned int reserved_0 : 31;         /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_color_ctrl;

/* Define the union u_isp_drc_global_corr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_cc_global_corr : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;             /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_global_corr;

/* Define the union u_isp_drc_mixing_coring */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_mixing_coring : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;           /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_mixing_coring;

/* Define the union u_isp_drc_mixing_dark */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_dark_min : 8; /* [7..0]  */
        unsigned int isp_drc_dark_max : 8; /* [15..8]  */
        unsigned int isp_drc_dark_thr : 8; /* [23..16]  */
        unsigned int isp_drc_dark_slo : 4; /* [27..24]  */
        unsigned int reserved_0 : 4;       /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_mixing_dark;

/* Define the union u_isp_drc_mixing_bright */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_bright_min : 8; /* [7..0]  */
        unsigned int isp_drc_bright_max : 8; /* [15..8]  */
        unsigned int isp_drc_bright_thr : 8; /* [23..16]  */
        unsigned int isp_drc_bright_slo : 4; /* [27..24]  */
        unsigned int reserved_0 : 4;         /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_mixing_bright;

/* Define the union u_isp_drc_tmlut0_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_tmlut0_waddr;
/* Define the union u_isp_drc_tmlut0_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_tmlut0_wdata;
/* Define the union u_isp_drc_tmlut0_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_tmlut0_raddr;
/* Define the union u_isp_drc_tmlut0_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_tmlut0_rdata;
/* Define the union u_isp_drc_cclut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_cclut_waddr;
/* Define the union u_isp_drc_cclut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_cclut_wdata;
/* Define the union u_isp_drc_cclut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_cclut_raddr;
/* Define the union u_isp_drc_cclut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_cclut_rdata;

/* Define the union u_isp_drc_coefalut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_coefalut_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_coefalut_waddr;
/* Define the union u_isp_drc_coefalut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_coefalut_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_coefalut_wdata;
/* Define the union u_isp_drc_coefalut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_coefalut_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_coefalut_raddr;
/* Define the union u_isp_drc_coefalut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_coefalut_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_coefalut_rdata;
/* Define the union u_isp_drc_delut_waddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_delut_waddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_delut_waddr;
/* Define the union u_isp_drc_delut_wdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_delut_wdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_delut_wdata;
/* Define the union u_isp_drc_delut_raddr */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_delut_raddr : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_delut_raddr;
/* Define the union u_isp_drc_delut_rdata */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_delut_rdata : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_delut_rdata;

/* Define the union u_isp_drc_grad_rev */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_grad_rev_thres : 8; /* [7..0]  */
        unsigned int isp_drc_grad_rev_max : 7;   /* [14..8]  */
        unsigned int reserved_0 : 1;             /* [15]  */
        unsigned int isp_drc_grad_rev_slope : 4; /* [19..16]  */
        unsigned int reserved_1 : 4;             /* [23..20]  */
        unsigned int isp_drc_grad_rev_shift : 3; /* [26..24]  */
        unsigned int reserved_2 : 5;             /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_grad_rev;

/* Define the union u_isp_drc_vbi_strength */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_var_rng_fine : 2; /* [1..0]  */
        unsigned int reserved_0 : 2;           /* [3..2]  */
        unsigned int reserved_1 : 3;           /* [6..4]  */
        unsigned int reserved_2 : 1;           /* [7]  */
        unsigned int reserved_3 : 4;           /* [11..8]  */
        unsigned int reserved_4 : 2;           /* [13..12]  */
        unsigned int reserved_5 : 2;           /* [15..14]  */
        unsigned int reserved_6 : 3;           /* [18..16]  */
        unsigned int reserved_7 : 1;           /* [19]  */
        unsigned int reserved_8 : 4;           /* [23..20]  */
        unsigned int reserved_9 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_vbi_strength;

/* Define the union u_isp_drc_rng_ada_coef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 4;  /* [3..0]  */
        unsigned int reserved_1 : 4;  /* [7..4]  */
        unsigned int reserved_2 : 3;  /* [10..8]  */
        unsigned int reserved_3 : 1;  /* [11]  */
        unsigned int reserved_4 : 3;  /* [14..12]  */
        unsigned int reserved_5 : 1;  /* [15]  */
        unsigned int reserved_6 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_rng_ada_coef;

/* Define the union u_isp_drc_vbi_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_vbi_state : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_vbi_state;

/* Define the union u_isp_drc_prev_luma_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_0 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_0;

/* Define the union u_isp_drc_prev_luma_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_1 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_1;

/* Define the union u_isp_drc_prev_luma_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_2 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_2;

/* Define the union u_isp_drc_prev_luma_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_3 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_3;

/* Define the union u_isp_drc_prev_luma_4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_4 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_4;

/* Define the union u_isp_drc_prev_luma_5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_5 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_5;

/* Define the union u_isp_drc_prev_luma_6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_6 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_6;

/* Define the union u_isp_drc_prev_luma_7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_prev_luma_7 : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;           /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_prev_luma_7;

/* Define the union u_isp_drc_suppress_bright */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_suppress_bright_max : 8; /* [7..0]  */
        unsigned int isp_drc_suppress_bright_min : 8; /* [15..8]  */
        unsigned int isp_drc_suppress_bright_thr : 8; /* [23..16]  */
        unsigned int isp_drc_suppress_bright_slo : 4; /* [27..24]  */
        unsigned int reserved_0 : 4;                  /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_suppress_bright;

/* Define the union u_isp_drc_suppress_dark */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_suppress_dark_max : 8; /* [7..0]  */
        unsigned int isp_drc_suppress_dark_min : 8; /* [15..8]  */
        unsigned int isp_drc_suppress_dark_thr : 8; /* [23..16]  */
        unsigned int isp_drc_suppress_dark_slo : 4; /* [27..24]  */
        unsigned int reserved_0 : 4;                /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_suppress_dark;

/* Define the union u_isp_drc_idxbase0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_idxbase0 : 8; /* [7..0]  */
        unsigned int isp_drc_idxbase1 : 8; /* [15..8]  */
        unsigned int isp_drc_idxbase2 : 8; /* [23..16]  */
        unsigned int isp_drc_idxbase3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_idxbase0;

/* Define the union u_isp_drc_idxbase1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_idxbase4 : 8; /* [7..0]  */
        unsigned int isp_drc_idxbase5 : 8; /* [15..8]  */
        unsigned int isp_drc_idxbase6 : 8; /* [23..16]  */
        unsigned int isp_drc_idxbase7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_idxbase1;

/* Define the union u_isp_drc_maxval0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_maxval0 : 8; /* [7..0]  */
        unsigned int isp_drc_maxval1 : 8; /* [15..8]  */
        unsigned int isp_drc_maxval2 : 8; /* [23..16]  */
        unsigned int isp_drc_maxval3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_maxval0;

/* Define the union u_isp_drc_maxval1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_maxval4 : 8; /* [7..0]  */
        unsigned int isp_drc_maxval5 : 8; /* [15..8]  */
        unsigned int isp_drc_maxval6 : 8; /* [23..16]  */
        unsigned int isp_drc_maxval7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_maxval1;

/* Define the union u_isp_drc_pregamma_idxbase0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 8; /* [7..0]  */
        unsigned int reserved_1 : 8; /* [15..8]  */
        unsigned int reserved_2 : 8; /* [23..16]  */
        unsigned int reserved_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_idxbase0;

/* Define the union u_isp_drc_pregamma_idxbase1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 8; /* [7..0]  */
        unsigned int reserved_1 : 8; /* [15..8]  */
        unsigned int reserved_2 : 8; /* [23..16]  */
        unsigned int reserved_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_idxbase1;

/* Define the union u_isp_drc_pregamma_idxbase2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 8; /* [7..0]  */
        unsigned int reserved_1 : 8; /* [15..8]  */
        unsigned int reserved_2 : 8; /* [23..16]  */
        unsigned int reserved_3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_idxbase2;

/* Define the union u_isp_drc_pregamma_maxval0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 14; /* [13..0]  */
        unsigned int reserved_1 : 2;  /* [15..14]  */
        unsigned int reserved_2 : 14; /* [29..16]  */
        unsigned int reserved_3 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_maxval0;

/* Define the union u_isp_drc_pregamma_maxval1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 14; /* [13..0]  */
        unsigned int reserved_1 : 2;  /* [15..14]  */
        unsigned int reserved_2 : 14; /* [29..16]  */
        unsigned int reserved_3 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_maxval1;

/* Define the union u_isp_drc_pregamma_maxval2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 14; /* [13..0]  */
        unsigned int reserved_1 : 2;  /* [15..14]  */
        unsigned int reserved_2 : 14; /* [29..16]  */
        unsigned int reserved_3 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_maxval2;

/* Define the union u_isp_drc_pregamma_maxval3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 14; /* [13..0]  */
        unsigned int reserved_1 : 2;  /* [15..14]  */
        unsigned int reserved_2 : 14; /* [29..16]  */
        unsigned int reserved_3 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_maxval3;

/* Define the union u_isp_drc_pregamma_maxval4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 14; /* [13..0]  */
        unsigned int reserved_1 : 2;  /* [15..14]  */
        unsigned int reserved_2 : 14; /* [29..16]  */
        unsigned int reserved_3 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_maxval4;

/* Define the union u_isp_drc_pregamma_maxval5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 14; /* [13..0]  */
        unsigned int reserved_1 : 2;  /* [15..14]  */
        unsigned int reserved_2 : 14; /* [29..16]  */
        unsigned int reserved_3 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_pregamma_maxval5;

/* Define the union u_isp_drc_flt_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_flt_spa_fine : 4;   /* [3..0]  */
        unsigned int reserved_0 : 4;             /* [7..4]  */
        unsigned int reserved_1 : 4;             /* [11..8]  */
        unsigned int isp_drc_flt_rng_fine : 4;   /* [15..12]  */
        unsigned int reserved_2 : 4;             /* [19..16]  */
        unsigned int reserved_3 : 4;             /* [23..20]  */
        unsigned int reserved_4 : 8;             /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_flt_cfg;

/* Define the union u_isp_drc_bin_factor_coarse_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5; /* [4..0]  */
        unsigned int reserved_1 : 3; /* [7..5]  */
        unsigned int reserved_2 : 5; /* [12..8]  */
        unsigned int reserved_3 : 3; /* [15..13]  */
        unsigned int reserved_4 : 5; /* [20..16]  */
        unsigned int reserved_5 : 3; /* [23..21]  */
        unsigned int reserved_6 : 5; /* [28..24]  */
        unsigned int reserved_7 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bin_factor_coarse_0;

/* Define the union u_isp_drc_bin_factor_coarse_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5; /* [4..0]  */
        unsigned int reserved_1 : 3; /* [7..5]  */
        unsigned int reserved_2 : 5; /* [12..8]  */
        unsigned int reserved_3 : 3; /* [15..13]  */
        unsigned int reserved_4 : 5; /* [20..16]  */
        unsigned int reserved_5 : 3; /* [23..21]  */
        unsigned int reserved_6 : 5; /* [28..24]  */
        unsigned int reserved_7 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bin_factor_coarse_1;

/* Define the union u_isp_drc_bin_factor_medium_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5; /* [4..0]  */
        unsigned int reserved_1 : 3; /* [7..5]  */
        unsigned int reserved_2 : 5; /* [12..8]  */
        unsigned int reserved_3 : 3; /* [15..13]  */
        unsigned int reserved_4 : 5; /* [20..16]  */
        unsigned int reserved_5 : 3; /* [23..21]  */
        unsigned int reserved_6 : 5; /* [28..24]  */
        unsigned int reserved_7 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bin_factor_medium_0;

/* Define the union u_isp_drc_bin_factor_medium_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 5; /* [4..0]  */
        unsigned int reserved_1 : 3; /* [7..5]  */
        unsigned int reserved_2 : 5; /* [12..8]  */
        unsigned int reserved_3 : 3; /* [15..13]  */
        unsigned int reserved_4 : 5; /* [20..16]  */
        unsigned int reserved_5 : 3; /* [23..21]  */
        unsigned int reserved_6 : 5; /* [28..24]  */
        unsigned int reserved_7 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_bin_factor_medium_1;

/* Define the union u_isp_drc_detail_sub_factor */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_detail_sub_factor : 5; /* [4..0]  */
        unsigned int reserved_0 : 27;               /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_detail_sub_factor;

/* Define the union u_isp_drc_wgt_box_tri_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_wgt_box_tri_sel : 1; /* [0]  */
        unsigned int reserved_0 : 31;             /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_wgt_box_tri_sel;

/* Define the union u_isp_drc_blk_wgt_init */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_blk_wgt_init : 15; /* [14..0]  */
        unsigned int reserved_0 : 17;           /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_blk_wgt_init;

/* Define the union u_isp_drc_shp_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_shp_log : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;      /* [7..4]  */
        unsigned int isp_drc_shp_exp : 4; /* [11..8]  */
        unsigned int reserved_1 : 20;     /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_shp_cfg;

/* Define the union u_isp_drc_div_denom_log */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_div_denom_log : 18; /* [17..0]  */
        unsigned int reserved_0 : 14;            /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_div_denom_log;

/* Define the union u_isp_drc_denom_exp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int isp_drc_denom_exp : 21; /* [20..0]  */
        unsigned int reserved_0 : 11;        /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_denom_exp;

/* Define the union u_isp_drc_debug_info */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_debug_info : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_isp_drc_debug_info;


typedef struct {
    volatile unsigned int reserved_0[32];                               /* 0x0~0x7c, reserved 32 * 4 bytes */
    volatile u_isp_fe_version isp_fe_version;                                 /* 0x0080 */
    volatile u_isp_fe_date isp_fe_date;                                       /* 0x0084 */
    volatile u_isp_fe_fpga_date isp_fe_fpga_date;                             /* 0x0088 */
    volatile unsigned int reserved_1;                                         /* 0x008c */
    volatile u_isp_fe_module_pos isp_fe_module_pos;                           /* 0x0090 */
    volatile u_isp_fe_fstart isp_fe_fstart;                                   /* 0x0094 */
    volatile unsigned int reserved_2[2];                                /* 0x0098~0x009c, reserved 2 * 4 bytes */
    volatile u_isp_fe_user_define0 isp_fe_user_define0;                       /* 0x00a0 */
    volatile u_isp_fe_user_define1 isp_fe_user_define1;                       /* 0x00a4 */
    volatile unsigned int reserved_3[2];                                /* 0x00a8~0x00ac, reserved 2 * 4 bytes */
    volatile u_isp_fe_startup isp_fe_startup;                                 /* 0x00b0 */
    volatile u_isp_fe_format isp_fe_format;                                   /* 0x00b4 */
    volatile unsigned int reserved_4[14];                               /* 0x00b8~0x00ec, reserved 14 * 4 bytes */
    volatile u_isp_fe_int isp_fe_int;                                         /* 0x00f0 */
    volatile unsigned int reserved_5;                                         /* 0x00f4 */
    volatile u_isp_fe_int_mask isp_fe_int_mask;                               /* 0x00f8 */
    volatile unsigned int reserved_6;                                         /* 0x00fc */
    volatile u_isp_fe_ctrl isp_fe_ctrl;                                       /* 0x0100 */
    volatile unsigned int reserved_7[4];                                /* 0x0104~0x0110, reserved 4 * 4 bytes */
    volatile u_isp_fe_adapter_cfg isp_fe_adapter_cfg;                         /* 0x0114 */
    volatile u_isp_fe_out_size isp_fe_out_size;                               /* 0x0118 */
    volatile unsigned int reserved_8[30];                               /* 0x011c~0x0190, reserved 30 * 4 bytes */
    volatile u_isp_fe_fstart_delay isp_fe_fstart_delay;                       /* 0x0194 */
    volatile unsigned int reserved_9[18];                               /* 0x0198~0x01dc, reserved 18 * 4 bytes */
    volatile u_isp_fe_ctrl_f isp_fe_ctrl_f;                                   /* 0x01e0 */
    volatile u_isp_fe_ctrl_i isp_fe_ctrl_i;                                   /* 0x01e4 */
    volatile u_isp_fe_timing_cfg isp_fe_timing_cfg;                           /* 0x01e8 */
    volatile u_isp_fe_reg_update isp_fe_reg_update;                           /* 0x01ec */
    volatile u_isp_fe_size isp_fe_size;                                       /* 0x01f0 */
    volatile unsigned int reserved_10[395];                             /* 0x01f4~0x081c, reserved 395 * 4 bytes */
    volatile u_isp_fe_in_sum_cfg isp_fe_in_sum_cfg;                           /* 0x0820 */
    volatile u_isp_fe_out_sum_cfg isp_fe_out_sum_cfg;                         /* 0x0824 */
    volatile unsigned int reserved_11[19];                              /* 0x0828~0x0870, reserved 19 * 4 bytes */
    volatile u_isp_fe_blk_size isp_fe_blk_size;                               /* 0x0874 */
    volatile u_isp_fe_blk_hblank isp_fe_blk_hblank;                           /* 0x0878 */
    volatile u_isp_fe_blk_vblank isp_fe_blk_vblank;                           /* 0x087c */
    volatile unsigned int reserved_12[4];                               /* 0x0880~0x088c, reserved 4 * 4 bytes */
    volatile u_isp_fe_in_sum0 isp_fe_in_sum0;                                 /* 0x0890 */
    volatile u_isp_fe_in_sum1 isp_fe_in_sum1;                                 /* 0x0894 */
    volatile unsigned int reserved_13[2];                               /* 0x0898~0x089c, reserved 2 * 4 bytes */
    volatile u_isp_fe_out_sum0 isp_fe_out_sum0;                               /* 0x08a0 */
    volatile u_isp_fe_out_sum1 isp_fe_out_sum1;                               /* 0x08a4 */
    volatile unsigned int reserved_14[537];                             /* 0x08a8~0x1108, reserved 537 * 4 bytes */
    volatile u_isp_blc1_version isp_blc1_version;                             /* 0x110c */
    volatile u_isp_blc1_offset1 isp_blc1_offset1;                             /* 0x1110 */
    volatile u_isp_blc1_offset2 isp_blc1_offset2;                             /* 0x1114 */
    volatile unsigned int reserved_15[763];                             /* 0x1118~0x1d00, reserved 763 * 4 bytes */
    volatile u_isp_dg2_blc_cfg isp_dg2_blc_cfg;                               /* 0x1d04 */
    volatile unsigned int reserved_16;                                        /* 0x1d08 */
    volatile u_isp_dg2_version isp_dg2_version;                               /* 0x1d0c */
    volatile u_isp_dg2_gain1 isp_dg2_gain1;                                   /* 0x1d10 */
    volatile u_isp_dg2_gain2 isp_dg2_gain2;                                   /* 0x1d14 */
    volatile u_isp_dg2_blc_offset1 isp_dg2_blc_offset1;                       /* 0x1d18 */
    volatile u_isp_dg2_blc_offset2 isp_dg2_blc_offset2;                       /* 0x1d1c */
    volatile u_isp_dg2_clip_value isp_dg2_clip_value;                         /* 0x1d20 */
    volatile unsigned int reserved_17[120];                             /* 0x1d24~0x1f00, reserved 120 * 4 bytes */
    volatile u_isp_wb1_blc_cfg isp_wb1_blc_cfg;                               /* 0x1f04 */
    volatile unsigned int reserved_18;                                        /* 0x1f08 */
    volatile u_isp_wb1_version isp_wb1_version;                               /* 0x1f0c */
    volatile u_isp_wb1_gain1 isp_wb1_gain1;                                   /* 0x1f10 */
    volatile u_isp_wb1_gain2 isp_wb1_gain2;                                   /* 0x1f14 */
    volatile u_isp_wb1_blc_offset1 isp_wb1_blc_offset1;                       /* 0x1f18 */
    volatile u_isp_wb1_blc_offset2 isp_wb1_blc_offset2;                       /* 0x1f1c */
    volatile u_isp_wb1_clip_value isp_wb1_clip_value;                         /* 0x1f20 */
    volatile unsigned int reserved_19[570];                             /* 0x1f24~0x2808, reserved 570 * 4 bytes */
    volatile u_isp_ae_version isp_ae_version;                                 /* 0x280c */
    volatile u_isp_ae_zone isp_ae_zone;                                       /* 0x2810 */
    volatile u_isp_ae_skip_crg isp_ae_skip_crg;                               /* 0x2814 */
    volatile u_isp_ae_total_stat isp_ae_total_stat;                           /* 0x2818 */
    volatile u_isp_ae_count_stat isp_ae_count_stat;                           /* 0x281c */
    volatile u_isp_ae_total_r_aver isp_ae_total_r_aver;                       /* 0x2820 */
    volatile u_isp_ae_total_gr_aver isp_ae_total_gr_aver;                     /* 0x2824 */
    volatile u_isp_ae_total_gb_aver isp_ae_total_gb_aver;                     /* 0x2828 */
    volatile u_isp_ae_total_b_aver isp_ae_total_b_aver;                       /* 0x282c */
    volatile u_isp_ae_hist_high isp_ae_hist_high;                             /* 0x2830 */
    volatile unsigned int reserved_20[3];                               /* 0x2834~0x283c, reserved 3 * 4 bytes */
    volatile u_isp_fe_ae_bitmove isp_ae_bitmove;                                 /* 0x2840 */
    volatile u_isp_ae_offset_r_gr isp_ae_offset_r_gr;                         /* 0x2844 */
    volatile unsigned int reserved_21;                                        /* 0x2848 */
    volatile u_isp_ae_offset_gb_b isp_ae_offset_gb_b;                         /* 0x284c */
    volatile unsigned int reserved_22[5];                               /* 0x2850~0x2860, reserved 5 * 4 bytes */
    volatile u_isp_ae_lut_update isp_ae_lut_update;                           /* 0x2864 */
    volatile u_isp_ae_stt_bst isp_ae_stt_bst;                                 /* 0x2868 */
    volatile unsigned int reserved_23[2];                               /* 0x286c~0x2870, reserved 2 * 4 bytes */
    volatile u_isp_ae_crop_pos isp_ae_crop_pos;                               /* 0x2874 */
    volatile u_isp_ae_crop_outsize isp_ae_crop_outsize;                       /* 0x2878 */
    volatile unsigned int reserved_24;                                        /* 0x287c */
    volatile u_isp_ae_hist_waddr isp_ae_hist_waddr;                           /* 0x2880 */
    volatile u_isp_ae_hist_wdata isp_ae_hist_wdata;                           /* 0x2884 */
    volatile u_isp_ae_hist_raddr isp_ae_hist_raddr;                           /* 0x2888 */
    volatile u_isp_ae_hist_rdata isp_ae_hist_rdata;                           /* 0x288c */
    volatile u_isp_ae_aver_r_gr_waddr isp_ae_aver_r_gr_waddr;                 /* 0x2890 */
    volatile u_isp_ae_aver_r_gr_wdata isp_ae_aver_r_gr_wdata;                 /* 0x2894 */
    volatile u_isp_ae_aver_r_gr_raddr isp_ae_aver_r_gr_raddr;                 /* 0x2898 */
    volatile u_isp_ae_aver_r_gr_rdata isp_ae_aver_r_gr_rdata;                 /* 0x289c */
    volatile u_isp_ae_aver_gb_b_waddr isp_ae_aver_gb_b_waddr;                 /* 0x28a0 */
    volatile u_isp_ae_aver_gb_b_wdata isp_ae_aver_gb_b_wdata;                 /* 0x28a4 */
    volatile u_isp_ae_aver_gb_b_raddr isp_ae_aver_gb_b_raddr;                 /* 0x28a8 */
    volatile u_isp_ae_aver_gb_b_rdata isp_ae_aver_gb_b_rdata;                 /* 0x28ac */
    volatile u_isp_ae_weight_waddr isp_ae_weight_waddr;                       /* 0x28b0 */
    volatile u_isp_ae_weight_wdata isp_ae_weight_wdata;                       /* 0x28b4 */
    volatile u_isp_ae_weight_raddr isp_ae_weight_raddr;                       /* 0x28b8 */
    volatile u_isp_ae_weight_rdata isp_ae_weight_rdata;                       /* 0x28bc */
    volatile unsigned int reserved_25[465];                             /* 0x28c0~0x3000, reserved 465 * 4 bytes */
    volatile u_isp_dg_blc_cfg isp_dg_blc_cfg ;                                /* 0x3004 */
    volatile unsigned int reserved_26;                                        /* 0x3008 */
    volatile u_isp_dg_version isp_dg_version;                                 /* 0x300c */
    volatile u_isp_dg_gain1 isp_dg_gain1;                                     /* 0x3010 */
    volatile u_isp_dg_gain2 isp_dg_gain2;                                     /* 0x3014 */
    volatile u_isp_dg_blc_offset1 isp_dg_blc_offset1;                         /* 0x3018 */
    volatile u_isp_dg_blc_offset2 isp_dg_blc_offset2;                         /* 0x301c */
    volatile u_isp_dg_clip_value  isp_dg_clip_value;                          /* 0x3020 */
    volatile unsigned int reserved_27[58];                                    /* 0x3024~0x3108 */
    volatile u_isp_gamma_version isp_gamma_version;                           /* 0x310c */
    volatile u_isp_gamma_bitw isp_gamma_bitw;                                 /* 0x3110 */
    volatile unsigned int reserved_28[27];                                    /* 0x3114~0x317c */
    volatile u_isp_gamma_lut_waddr isp_gamma_lut_waddr;                       /* 0x3180 */
    volatile u_isp_gamma_lut_wdata isp_gamma_lut_wdata;                       /* 0x3184 */
    volatile u_isp_gamma_lut_raddr isp_gamma_lut_raddr;                       /* 0x3188 */
    volatile u_isp_gamma_lut_rdata isp_gamma_lut_rdata;                       /* 0x318c */
    volatile unsigned int reserved_29[288];                                   /* 0x3190~0x360c */
    volatile u_isp_rc_blcen isp_rc_blcen;                                     /* 0x3610 */
    volatile u_isp_rc_blc0 isp_rc_blc0;                                       /* 0x3614 */
    volatile u_isp_rc_blc1 isp_rc_blc1;                                       /* 0x3618 */
    volatile unsigned int reserved_30;                                        /* 0x361c */
    volatile u_isp_rc_sqradius isp_rc_sqradius;                               /* 0x3620 */
    volatile u_isp_rc_center_coor isp_rc_center_coor;                         /* 0x3624 */
    volatile unsigned int reserved_31[7865];                            /* 0x3628~0xb108, reserved 7865 * 4 bytes */
    volatile u_isp_crop_version isp_crop_version;                             /* 0xb10c */
    volatile u_isp_crop_sizeout isp_crop_sizeout;                             /* 0xb110 */
    volatile u_isp_crop_pos isp_crop_pos;                                     /* 0xb114 */
} isp_fe_reg_type;


/* Define the global struct */
typedef struct {
    volatile unsigned int reserved_0[32];                                     /* 0x0~0x007c, reserved 32 * 4 bytes */
    volatile u_isp_be_version isp_be_version;                                 /* 0x20080 */
    volatile u_isp_be_date isp_be_date;                                       /* 0x20084 */
    volatile u_isp_be_fpga_date isp_be_fpga_date;                             /* 0x20088 */
    volatile unsigned int reserved_3;                                         /* 0x2008c */
    volatile u_isp_be_module_pos isp_be_module_pos;                           /* 0x20090 */
    volatile u_isp_be_fstart_delay isp_be_fstart_delay;                       /* 0x20094 */
    volatile u_isp_be_input_mux isp_be_input_mux;                             /* 0x20098 */
    volatile u_isp_be_sys_ctrl isp_be_sys_ctrl;                               /* 0x2009c */
    volatile u_isp_be_user_define0 isp_be_user_define0;                       /* 0x200a0 */
    volatile u_isp_be_user_define1 isp_be_user_define1;                       /* 0x200a4 */
    volatile unsigned int reserved_4[2];                                      /* 0x200a8~0x200ac */
    volatile u_isp_be_startup isp_be_startup;                                 /* 0x200b0 */
    volatile u_isp_be_format isp_be_format;                                   /* 0x200b4 */
    volatile u_isp_be_mem_cfg isp_be_mem_cfg;                                 /* 0x200b8 */
    volatile u_isp_be_mem_err isp_be_mem_err;                                 /* 0x200bc */
    volatile unsigned int reserved_5[464];                                    /* 0x200c0~0x207fc */
    volatile u_isp_clip_y_cfg isp_clip_y_cfg;                                 /* 0x20800 */
    volatile u_isp_clip_c_cfg isp_clip_c_cfg;                                 /* 0x20804 */
    volatile unsigned int reserved_6[6];                                      /* 0x20808~0x2081c */
    volatile u_isp_csc_sum_cfg isp_csc_sum_cfg;                               /* 0x20820 */
    volatile u_isp_yuv444_sum_cfg isp_yuv444_sum_cfg;                         /* 0x20824 */
    volatile u_isp_yuv422_sum_cfg isp_yuv422_sum_cfg;                         /* 0x20828 */
    volatile unsigned int reserved_7;                                         /* 0x2082c */
    volatile u_isp_wdr_sum_cfg isp_wdr_sum_cfg;                               /* 0x20830 */
    volatile u_isp_demosaic_sum_cfg isp_demosaic_sum_cfg;                     /* 0x20834 */
    volatile unsigned int reserved_8[2];                                      /* 0x20838~0x2083c */
    volatile u_isp_dmnr_dither isp_dmnr_dither;                               /* 0x20840 */
    volatile unsigned int reserved_9[3];                                      /* 0x20844~0x2084c */
    volatile u_isp_acm_dither isp_acm_dither;                                 /* 0x20850 */
    volatile unsigned int reserved_10[3];                                     /* 0x20854~0x2085c */
    volatile u_isp_drc_dither isp_drc_dither;                                 /* 0x20860 */
    volatile unsigned int reserved_11[3];                                     /* 0x20864~0x2086c */
    volatile u_isp_sqrt1_dither isp_sqrt1_dither;                             /* 0x20870 */
    volatile unsigned int reserved_12;                                        /* 0x20874 */
    volatile u_isp_blk_hblank isp_blk_hblank;                                 /* 0x20878 */
    volatile u_isp_blk_vblank isp_blk_vblank;                                 /* 0x2087c */
    volatile u_isp_y_sum0 isp_y_sum0;                                         /* 0x20880 */
    volatile u_isp_y_sum1 isp_y_sum1;                                         /* 0x20884 */
    volatile unsigned int reserved_13[2];                                     /* 0x20888~0x2088c */
    volatile u_isp_yuv444_y_sum0 isp_yuv444_y_sum0;                           /* 0x20890 */
    volatile u_isp_yuv444_y_sum1 isp_yuv444_y_sum1;                           /* 0x20894 */
    volatile u_isp_yuv444_u_sum0 isp_yuv444_u_sum0;                           /* 0x20898 */
    volatile u_isp_yuv444_u_sum1 isp_yuv444_u_sum1;                           /* 0x2089c */
    volatile u_isp_yuv444_v_sum0 isp_yuv444_v_sum0;                           /* 0x208a0 */
    volatile u_isp_yuv444_v_sum1 isp_yuv444_v_sum1;                           /* 0x208a4 */
    volatile unsigned int reserved_14[2];                                     /* 0x208a8~0x208ac */
    volatile u_isp_yuv422_y_sum0 isp_yuv422_y_sum0;                           /* 0x208b0 */
    volatile u_isp_yuv422_y_sum1 isp_yuv422_y_sum1;                           /* 0x208b4 */
    volatile u_isp_yuv422_c_sum0 isp_yuv422_c_sum0;                           /* 0x208b8 */
    volatile u_isp_yuv422_c_sum1 isp_yuv422_c_sum1;                           /* 0x208bc */
    volatile u_isp_wdr0_sum0 isp_wdr0_sum0;                                   /* 0x208c0 */
    volatile u_isp_wdr0_sum1 isp_wdr0_sum1;                                   /* 0x208c4 */
    volatile u_isp_wdr1_sum0 isp_wdr1_sum0;                                   /* 0x208c8 */
    volatile u_isp_wdr1_sum1 isp_wdr1_sum1;                                   /* 0x208cc */
    volatile u_isp_wdr2_sum0 isp_wdr2_sum0;                                   /* 0x208d0 */
    volatile u_isp_wdr2_sum1 isp_wdr2_sum1;                                   /* 0x208d4 */
    volatile u_isp_wdr3_sum0 isp_wdr3_sum0;                                   /* 0x208d8 */
    volatile u_isp_wdr3_sum1 isp_wdr3_sum1;                                   /* 0x208dc */
    volatile u_isp_demosaic_sum0 isp_demosaic_sum0;                           /* 0x208e0 */
    volatile u_isp_demosaic_sum1 isp_demosaic_sum1;                           /* 0x208e4 */
    volatile unsigned int reserved_15[457];                                   /* 0x208e8~0x21008 */
    volatile u_isp_blc_version isp_blc_version;                               /* 0x2100c */
    volatile u_isp_blc_offset1 isp_blc_offset1;                               /* 0x21010 */
    volatile u_isp_blc_offset2 isp_blc_offset2;                               /* 0x21014 */
    volatile unsigned int reserved_16[123];                                   /* 0x21018~0x21200 */
    volatile u_isp_wb_blc_cfg isp_wb_blc_cfg;                                 /* 0x21204 */
    volatile unsigned int reserved_17;                                        /* 0x21208 */
    volatile u_isp_wb_version isp_wb_version;                                 /* 0x2120c */
    volatile u_isp_wb_gain1 isp_wb_gain1;                                     /* 0x21210 */
    volatile u_isp_wb_gain2 isp_wb_gain2;                                     /* 0x21214 */
    volatile u_isp_wb_blc_offset1 isp_wb_blc_offset1;                         /* 0x21218 */
    volatile u_isp_wb_blc_offset2 isp_wb_blc_offset2;                         /* 0x2121c */
    volatile u_isp_wb_clip_value isp_wb_clip_value;                           /* 0x21220 */
    volatile u_isp_wb_blc_offset_shift       isp_wb_blc_offset_shift        ; /* 0x21224 */
    volatile unsigned int reserved_18[55];                                    /* 0x21228~0x21300 */
    volatile u_isp_dg_blc_cfg isp_dg_blc_cfg;                                 /* 0x21304 */
    volatile unsigned int reserved_19;                                        /* 0x21308 */
    volatile u_isp_dg_version isp_dg_version;                                 /* 0x2130c */
    volatile u_isp_dg_gain1 isp_dg_gain1;                                     /* 0x21310 */
    volatile u_isp_dg_gain2 isp_dg_gain2;                                     /* 0x21314 */
    volatile u_isp_dg_blc_offset1 isp_dg_blc_offset1;                         /* 0x21318 */
    volatile u_isp_dg_blc_offset2 isp_dg_blc_offset2;                         /* 0x2131c */
    volatile u_isp_dg_clip_value isp_dg_clip_value;                           /* 0x21320 */
    volatile  u_isp_dg_blc_offset_shift       isp_dg_blc_offset_shift       ; /* 0x21324 */
    volatile unsigned int reserved_20[54];                                    /* 0x21328~0x213fc */
    volatile u_isp_cc_cfg isp_cc_cfg;                                         /* 0x21400 */
    volatile unsigned int reserved_21[3];                                     /* 0x21404~0x2140c */
    volatile u_isp_cc_coef00 isp_cc_coef00;                         /* 0x21410 */
    volatile u_isp_cc_coef01 isp_cc_coef01;                         /* 0x21414 */
    volatile u_isp_cc_coef02 isp_cc_coef02;                         /* 0x21418 */
    volatile u_isp_cc_coef03 isp_cc_coef03;                         /* 0x2141c */
    volatile u_isp_cc_coef04 isp_cc_coef04;                         /* 0x21420 */
    volatile u_isp_cc_coef10 isp_cc_coef10;                         /* 0x21424 */
    volatile u_isp_cc_coef11 isp_cc_coef11;                         /* 0x21428 */
    volatile u_isp_cc_coef12 isp_cc_coef12;                         /* 0x2142c */
    volatile u_isp_cc_coef13 isp_cc_coef13;                         /* 0x21430 */
    volatile u_isp_cc_coef14 isp_cc_coef14;                         /* 0x21434 */
    volatile u_isp_cc_coef20 isp_cc_coef20;                         /* 0x21438 */
    volatile u_isp_cc_coef21 isp_cc_coef21;                         /* 0x2143c */
    volatile u_isp_cc_coef22 isp_cc_coef22;                         /* 0x21440 */
    volatile u_isp_cc_coef23 isp_cc_coef23;                         /* 0x21444 */
    volatile u_isp_cc_coef24 isp_cc_coef24;                         /* 0x21448 */
    volatile u_isp_cc_coef30 isp_cc_coef30;                         /* 0x2144c */
    volatile u_isp_cc_coef31 isp_cc_coef31;                         /* 0x21450 */
    volatile u_isp_cc_coef32 isp_cc_coef32;                         /* 0x21454 */
    volatile u_isp_cc_coef33 isp_cc_coef33;                         /* 0x21458 */
    volatile u_isp_cc_coef34 isp_cc_coef34;                         /* 0x2145c */
    volatile u_isp_cc_coef40 isp_cc_coef40;                         /* 0x21460 */
    volatile u_isp_cc_coef41 isp_cc_coef41;                         /* 0x21464 */
    volatile u_isp_cc_coef42 isp_cc_coef42;                         /* 0x21468 */
    volatile u_isp_cc_coef43 isp_cc_coef43;                         /* 0x2146c */
    volatile u_isp_cc_coef44 isp_cc_coef44;                         /* 0x21470 */
    volatile u_isp_cc_coef50 isp_cc_coef50;                         /* 0x21474 */
    volatile u_isp_cc_coef51 isp_cc_coef51;                         /* 0x21478 */
    volatile u_isp_cc_coef52 isp_cc_coef52;                         /* 0x2147c */
    volatile unsigned int reserved_1031[32];                        /* 0x21480~0x214fc */
    volatile u_isp_cc_coef53 isp_cc_coef53;                           /* 0x21500 */
    volatile u_isp_cc_coef54 isp_cc_coef54;                           /* 0x21504 */
    volatile u_isp_cc_coef60 isp_cc_coef60;                           /* 0x21508 */
    volatile u_isp_cc_coef61 isp_cc_coef61;                           /* 0x2150c */
    volatile u_isp_cc_coef62 isp_cc_coef62;                           /* 0x21510 */
    volatile u_isp_cc_coef63 isp_cc_coef63;                           /* 0x21514 */
    volatile u_isp_cc_coef64 isp_cc_coef64;                           /* 0x21518 */
    volatile u_isp_cc_coef70 isp_cc_coef70;                           /* 0x2151c */
    volatile u_isp_cc_coef71 isp_cc_coef71;                           /* 0x21520 */
    volatile u_isp_cc_coef72 isp_cc_coef72;                           /* 0x21524 */
    volatile u_isp_cc_coef73 isp_cc_coef73;                           /* 0x21528 */
    volatile u_isp_cc_coef74 isp_cc_coef74;                           /* 0x2152c */
    volatile u_isp_cc_hue_coef0 isp_cc_hue_coef0;                     /* 0x21530 */
    volatile u_isp_cc_hue_coef1 isp_cc_hue_coef1;                     /* 0x21534 */
    volatile u_isp_cc_hue_coef2 isp_cc_hue_coef2;                     /* 0x21538 */
    volatile u_isp_cc_hue_coef3 isp_cc_hue_coef3;                     /* 0x2153c */
    volatile u_isp_cc_hue_cfg isp_cc_hue_cfg;                         /* 0x21540 */
    volatile u_isp_cc_rcv_ctrl0 isp_cc_rcv_ctrl0;                     /* 0x21544 */
    volatile u_isp_cc_rcv_ctrl1 isp_cc_rcv_ctrl1;                     /* 0x21548 */
    volatile u_isp_cc_lumafact isp_cc_lumafact;                       /* 0x2154c */
    volatile u_isp_cc_colortone_rb_gain isp_cc_colortone_rb_gain;     /* 0x21550 */
    volatile u_isp_cc_colortone_g_gain isp_cc_colortone_g_gain;       /* 0x21554 */
    volatile u_isp_cc_in_dc0 isp_cc_in_dc0;                           /* 0x21558 */
    volatile u_isp_cc_in_dc1 isp_cc_in_dc1;                           /* 0x2155c */
    volatile u_isp_cc_in_dc2 isp_cc_in_dc2;                           /* 0x21560 */
    volatile u_isp_cc_out_dc0 isp_cc_out_dc0;                         /* 0x21564 */
    volatile u_isp_cc_out_dc1 isp_cc_out_dc1;                         /* 0x21568 */
    volatile u_isp_cc_out_dc2 isp_cc_out_dc2;                         /* 0x2156c */
    volatile unsigned int reserved_103[36];                           /* 0x21570~0x215fc */
    volatile u_isp_cc_dgain_pos0 isp_cc_dgain_pos0;                           /* 0x21600 */
    volatile u_isp_cc_dgain_pos1 isp_cc_dgain_pos1;                           /* 0x21604 */
    volatile u_isp_cc_dgain_pos2 isp_cc_dgain_pos2;                           /* 0x21608 */
    volatile u_isp_cc_dgain_pos3 isp_cc_dgain_pos3;                           /* 0x2160c */
    volatile u_isp_cc_dgain_inseg0 isp_cc_dgain_inseg0;                       /* 0x21610 */
    volatile u_isp_cc_dgain_inseg1 isp_cc_dgain_inseg1;                       /* 0x21614 */
    volatile u_isp_cc_dgain_step isp_cc_dgain_step;                           /* 0x21618 */
    volatile u_isp_cc_gamma_pos0 isp_cc_gamma_pos0;                           /* 0x2161c */
    volatile u_isp_cc_gamma_pos1 isp_cc_gamma_pos1;                           /* 0x21620 */
    volatile u_isp_cc_gamma_pos2 isp_cc_gamma_pos2;                           /* 0x21624 */
    volatile u_isp_cc_gamma_pos3 isp_cc_gamma_pos3;                           /* 0x21628 */
    volatile u_isp_cc_gamma_inseg0 isp_cc_gamma_inseg0;                       /* 0x2162c */
    volatile u_isp_cc_gamma_inseg1 isp_cc_gamma_inseg1;                       /* 0x21630 */
    volatile u_isp_cc_gamma_step isp_cc_gamma_step;                           /* 0x21634 */
    volatile u_isp_cc_luma_wdr_coef isp_cc_luma_wdr_coef;                     /* 0x21638 */
    volatile u_isp_cc_stt2lut_cfg isp_cc_stt2lut_cfg;                         /* 0x2163c */
    volatile u_isp_cc_stt2lut_regnew isp_cc_stt2lut_regnew;                   /* 0x21640 */
    volatile u_isp_cc_stt2lut_abn isp_cc_stt2lut_abn;                         /* 0x21644 */
    volatile unsigned int reserved_25[14];                                    /* 0x21648~0x2167c */
    volatile u_isp_cc_dgain_lut_waddr isp_cc_dgain_lut_waddr;                 /* 0x21680 */
    volatile u_isp_cc_dgain_lut_wdata isp_cc_dgain_lut_wdata;                 /* 0x21684 */
    volatile u_isp_cc_dgain_lut_raddr isp_cc_dgain_lut_raddr;                 /* 0x21688 */
    volatile u_isp_cc_dgain_lut_rdata isp_cc_dgain_lut_rdata;                 /* 0x2168c */
    volatile u_isp_cc_gamma_lut_waddr isp_cc_gamma_lut_waddr;                 /* 0x21690 */
    volatile u_isp_cc_gamma_lut_wdata isp_cc_gamma_lut_wdata;                 /* 0x21694 */
    volatile u_isp_cc_gamma_lut_raddr isp_cc_gamma_lut_raddr;                 /* 0x21698 */
    volatile u_isp_cc_gamma_lut_rdata isp_cc_gamma_lut_rdata;                 /* 0x2169c */
    volatile unsigned int reserved_26[27];                                    /* 0x216a0~0x21708 */
    volatile u_isp_csc_version isp_csc_version;                               /* 0x2170c */
    volatile u_isp_csc_coef0 isp_csc_coef0;                                   /* 0x21710 */
    volatile u_isp_csc_coef1 isp_csc_coef1;                                   /* 0x21714 */
    volatile u_isp_csc_coef2 isp_csc_coef2;                                   /* 0x21718 */
    volatile u_isp_csc_coef3 isp_csc_coef3;                                   /* 0x2171c */
    volatile u_isp_csc_coef4 isp_csc_coef4;                                   /* 0x21720 */
    volatile unsigned int reserved_27[3];                                     /* 0x21724~0x2172c */
    volatile u_isp_csc_in_dc0 isp_csc_in_dc0;                                 /* 0x21730 */
    volatile u_isp_csc_in_dc1 isp_csc_in_dc1;                                 /* 0x21734 */
    volatile unsigned int reserved_28[2];                                     /* 0x21738~0x2173c */
    volatile u_isp_csc_out_dc0 isp_csc_out_dc0;                               /* 0x21740 */
    volatile u_isp_csc_out_dc1 isp_csc_out_dc1;                               /* 0x21744 */
    volatile unsigned int reserved_29[49];                                    /* 0x21748~0x21808 */
    volatile u_isp_hcds_version isp_hcds_version;                             /* 0x2180c */
    volatile u_isp_hcds_coefh0 isp_hcds_coefh0;                               /* 0x21810 */
    volatile u_isp_hcds_coefh1 isp_hcds_coefh1;                               /* 0x21814 */
    volatile u_isp_hcds_coefh2 isp_hcds_coefh2;                               /* 0x21818 */
    volatile u_isp_hcds_coefh3 isp_hcds_coefh3;                               /* 0x2181c */
    volatile unsigned int reserved_30[249];                                   /* 0x21820~0x21c00 */
    volatile u_isp_4dg_blc_cfg isp_4dg_blc_cfg;                               /* 0x21c04 */
    volatile unsigned int reserved_31;                                        /* 0x21c08 */
    volatile u_isp_4dg_version isp_4dg_version;                               /* 0x21c0c */
    volatile u_isp_4dg_0_gain1 isp_4dg_0_gain1;                               /* 0x21c10 */
    volatile u_isp_4dg_0_gain2 isp_4dg_0_gain2;                               /* 0x21c14 */
    volatile u_isp_4dg_0_blc_offset1 isp_4dg_0_blc_offset1;                   /* 0x21c18 */
    volatile u_isp_4dg_0_blc_offset2 isp_4dg_0_blc_offset2;                   /* 0x21c1c */
    volatile u_isp_4dg_1_gain1 isp_4dg_1_gain1;                               /* 0x21c20 */
    volatile u_isp_4dg_1_gain2 isp_4dg_1_gain2;                               /* 0x21c24 */
    volatile u_isp_4dg_1_blc_offset1 isp_4dg_1_blc_offset1;                   /* 0x21c28 */
    volatile u_isp_4dg_1_blc_offset2 isp_4dg_1_blc_offset2;                   /* 0x21c2c */
    volatile unsigned int reserved_32[8];                                     /* 0x21c30~0x21c4c */
    volatile u_isp_4dg_0_clip_value isp_4dg_0_clip_value;                     /* 0x21c50 */
    volatile u_isp_4dg_1_clip_value isp_4dg_1_clip_value;                     /* 0x21c54 */
    volatile unsigned int reserved_33[237];                                   /* 0x21c58~0x22008 */
    volatile u_isp_ae_version isp_ae_version;                                 /* 0x2200c */
    volatile u_isp_ae_zone isp_ae_zone;                                       /* 0x22010 */
    volatile u_isp_ae_skip_crg isp_ae_skip_crg;                               /* 0x22014 */
    volatile u_isp_ae_total_stat isp_ae_total_stat;                           /* 0x22018 */
    volatile u_isp_ae_count_stat isp_ae_count_stat;                           /* 0x2201c */
    volatile u_isp_ae_total_r_aver isp_ae_total_r_aver;                       /* 0x22020 */
    volatile u_isp_ae_total_gr_aver isp_ae_total_gr_aver;                     /* 0x22024 */
    volatile u_isp_ae_total_gb_aver isp_ae_total_gb_aver;                     /* 0x22028 */
    volatile u_isp_ae_total_b_aver isp_ae_total_b_aver;                       /* 0x2202c */
    volatile u_isp_ae_hist_high isp_ae_hist_high;                             /* 0x22030 */
    volatile unsigned int reserved_34[3];                                     /* 0x22034~0x2203c */
    volatile u_isp_ae_bitmove isp_ae_bitmove;                                 /* 0x22040 */
    volatile u_isp_ae_offset_r_gr isp_ae_offset_r_gr;                         /* 0x22044 */
    volatile unsigned int reserved_35;                                        /* 0x22048 */
    volatile u_isp_ae_offset_gb_b isp_ae_offset_gb_b;                         /* 0x2204c */
    volatile unsigned int reserved_36[5];                                     /* 0x22050~0x22060 */
    volatile u_isp_ae_lut_update isp_ae_lut_update;                           /* 0x22064 */
    volatile u_isp_ae_stt_bst isp_ae_stt_bst;                                 /* 0x22068 */
    volatile unsigned int reserved_37[2];                                     /* 0x2206c~0x22070 */
    volatile u_isp_ae_crop_pos isp_ae_crop_pos;                               /* 0x22074 */
    volatile u_isp_ae_crop_outsize isp_ae_crop_outsize;                       /* 0x22078 */
    volatile unsigned int reserved_38;                                        /* 0x2207c */
    volatile u_isp_ae_hist_waddr isp_ae_hist_waddr;                           /* 0x22080 */
    volatile u_isp_ae_hist_wdata isp_ae_hist_wdata;                           /* 0x22084 */
    volatile u_isp_ae_hist_raddr isp_ae_hist_raddr;                           /* 0x22088 */
    volatile u_isp_ae_hist_rdata isp_ae_hist_rdata;                           /* 0x2208c */
    volatile u_isp_ae_aver_r_gr_waddr isp_ae_aver_r_gr_waddr;                 /* 0x22090 */
    volatile u_isp_ae_aver_r_gr_wdata isp_ae_aver_r_gr_wdata;                 /* 0x22094 */
    volatile u_isp_ae_aver_r_gr_raddr isp_ae_aver_r_gr_raddr;                 /* 0x22098 */
    volatile u_isp_ae_aver_r_gr_rdata isp_ae_aver_r_gr_rdata;                 /* 0x2209c */
    volatile u_isp_ae_aver_gb_b_waddr isp_ae_aver_gb_b_waddr;                 /* 0x220a0 */
    volatile u_isp_ae_aver_gb_b_wdata isp_ae_aver_gb_b_wdata;                 /* 0x220a4 */
    volatile u_isp_ae_aver_gb_b_raddr isp_ae_aver_gb_b_raddr;                 /* 0x220a8 */
    volatile u_isp_ae_aver_gb_b_rdata isp_ae_aver_gb_b_rdata;                 /* 0x220ac */
    volatile u_isp_ae_weight_waddr isp_ae_weight_waddr;                       /* 0x220b0 */
    volatile u_isp_ae_weight_wdata isp_ae_weight_wdata;                       /* 0x220b4 */
    volatile u_isp_ae_weight_raddr isp_ae_weight_raddr;                       /* 0x220b8 */
    volatile u_isp_ae_weight_rdata isp_ae_weight_rdata;                       /* 0x220bc */
    volatile unsigned int reserved_39[19];                                    /* 0x220c0~0x22108 */
    volatile u_isp_awb_version isp_awb_version;                               /* 0x2210c */
    volatile u_isp_awb_zone isp_awb_zone;                                     /* 0x22110 */
    volatile u_isp_awb_bitmove isp_awb_bitmove;                               /* 0x22114 */
    volatile u_isp_awb_thd_min isp_awb_thd_min;                               /* 0x22118 */
    volatile u_isp_awb_thd_max isp_awb_thd_max;                               /* 0x2211c */
    volatile u_isp_awb_cr_mm isp_awb_cr_mm;                                   /* 0x22120 */
    volatile u_isp_awb_cb_mm isp_awb_cb_mm;                                   /* 0x22124 */
    volatile u_isp_awb_offset_comp isp_awb_offset_comp;                       /* 0x22128 */
    volatile unsigned int reserved_40[5];                                     /* 0x2212c~0x2213c */
    volatile u_isp_awb_avg_r isp_awb_avg_r;                                   /* 0x22140 */
    volatile u_isp_awb_avg_g isp_awb_avg_g;                                   /* 0x22144 */
    volatile u_isp_awb_avg_b isp_awb_avg_b;                                   /* 0x22148 */
    volatile u_isp_awb_cnt_all isp_awb_cnt_all;                               /* 0x2214c */
    volatile unsigned int reserved_41[4];                                     /* 0x22150~0x2215c */
    volatile u_isp_awb_stt_bst isp_awb_stt_bst;                               /* 0x22160 */
    volatile u_isp_awb_stt_abn isp_awb_stt_abn;                               /* 0x22164 */
    volatile unsigned int reserved_42[3];                                     /* 0x22168~0x22170 */
    volatile u_isp_awb_crop_pos isp_awb_crop_pos;                             /* 0x22174 */
    volatile u_isp_awb_crop_outsize isp_awb_crop_outsize;                     /* 0x22178 */
    volatile unsigned int reserved_43;                                        /* 0x2217c */
    volatile u_isp_awb_stat_waddr isp_awb_stat_waddr;                         /* 0x22180 */
    volatile u_isp_awb_stat_wdata isp_awb_stat_wdata;                         /* 0x22184 */
    volatile u_isp_awb_stat_raddr isp_awb_stat_raddr;                         /* 0x22188 */
    volatile u_isp_awb_stat_rdata isp_awb_stat_rdata;                         /* 0x2218c */
    volatile unsigned int reserved_44[28];                                    /* 0x22190~0x221fc */
    volatile u_isp_af_cfg isp_af_cfg;                                         /* 0x22200 */
    volatile unsigned int reserved_45[2];                                     /* 0x22204~0x22208 */
    volatile u_isp_af_version isp_af_version;                                 /* 0x2220c */
    volatile u_isp_af_zone isp_af_zone;                                       /* 0x22210 */
    volatile u_isp_af_crop_start isp_af_crop_start;                           /* 0x22214 */
    volatile u_isp_af_crop_size isp_af_crop_size;                             /* 0x22218 */
    volatile u_isp_af_mean_thres isp_af_mean_thres;                           /* 0x2221c */
    volatile u_isp_af_iirg0 isp_af_iirg0;                                     /* 0x22220 */
    volatile u_isp_af_iirg1 isp_af_iirg1;                                     /* 0x22224 */
    volatile u_isp_af_iirg2 isp_af_iirg2;                                     /* 0x22228 */
    volatile u_isp_af_iirg3 isp_af_iirg3;                                     /* 0x2222c */
    volatile u_isp_af_iirg4 isp_af_iirg4;                                     /* 0x22230 */
    volatile u_isp_af_iirg5 isp_af_iirg5;                                     /* 0x22234 */
    volatile u_isp_af_iirg6 isp_af_iirg6;                                     /* 0x22238 */
    volatile u_isp_af_iirpl isp_af_iirpl;                                     /* 0x2223c */
    volatile u_isp_af_shift isp_af_shift;                                     /* 0x22240 */
    volatile unsigned int reserved_46[3];                                     /* 0x22244~0x2224c */
    volatile u_isp_af_firh0 isp_af_firh0;                                     /* 0x22250 */
    volatile u_isp_af_firh1 isp_af_firh1;                                     /* 0x22254 */
    volatile u_isp_af_firh2 isp_af_firh2;                                     /* 0x22258 */
    volatile u_isp_af_firh3 isp_af_firh3;                                     /* 0x2225c */
    volatile u_isp_af_firh4 isp_af_firh4;                                     /* 0x22260 */
    volatile u_isp_af_stt_bst isp_af_stt_bst;                                 /* 0x22264 */
    volatile u_isp_af_stt_abn isp_af_stt_abn;                                 /* 0x22268 */
    volatile unsigned int reserved_47[3];                                     /* 0x2226c~0x22274 */
    volatile u_isp_af_acc_shift isp_af_acc_shift;                             /* 0x22278 */
    volatile u_isp_af_cnt_shift isp_af_cnt_shift;                             /* 0x2227c */
    volatile unsigned int reserved_48[2];                                     /* 0x22280~0x22284 */
    volatile u_isp_af_stat_raddr isp_af_stat_raddr;                           /* 0x22288 */
    volatile u_isp_af_stat_rdata isp_af_stat_rdata;                           /* 0x2228c */
    volatile unsigned int reserved_49[28];                                    /* 0x22290~0x222fc */
    volatile u_isp_af_iirthre isp_af_iirthre;                                 /* 0x22300 */
    volatile u_isp_af_iirgain isp_af_iirgain;                                 /* 0x22304 */
    volatile u_isp_af_iirslope isp_af_iirslope;                               /* 0x22308 */
    volatile u_isp_af_iirdilate isp_af_iirdilate;                             /* 0x2230c */
    volatile u_isp_af_firthre isp_af_firthre;                                 /* 0x22310 */
    volatile u_isp_af_firgain isp_af_firgain;                                 /* 0x22314 */
    volatile u_isp_af_firslope isp_af_firslope;                               /* 0x22318 */
    volatile unsigned int reserved_50;                                        /* 0x2231c */
    volatile u_isp_af_iirthre_coring isp_af_iirthre_coring;                   /* 0x22320 */
    volatile u_isp_af_iirpeak_coring isp_af_iirpeak_coring;                   /* 0x22324 */
    volatile u_isp_af_iirslope_coring isp_af_iirslope_coring;                 /* 0x22328 */
    volatile unsigned int reserved_51;                                        /* 0x2232c */
    volatile u_isp_af_firthre_coring isp_af_firthre_coring;                   /* 0x22330 */
    volatile u_isp_af_firpeak_coring isp_af_firpeak_coring;                   /* 0x22334 */
    volatile u_isp_af_firslope_coring isp_af_firslope_coring;                 /* 0x22338 */
    volatile unsigned int reserved_52;                                        /* 0x2233c */
    volatile u_isp_af_hilight isp_af_hilight;                                 /* 0x22340 */
    volatile u_isp_af_offset isp_af_offset;                                   /* 0x22344 */
    volatile unsigned int reserved_53[46];                                    /* 0x22348~0x223fc */
    volatile u_isp_dis_cfg isp_dis_cfg;                                       /* 0x22400 */
    volatile u_isp_dis_blk isp_dis_blk;                                       /* 0x22404 */
    volatile unsigned int reserved_54;                                        /* 0x22408 */
    volatile u_isp_dis_version isp_dis_version;                               /* 0x2240c */
    volatile u_isp_dis_v0pos isp_dis_v0pos;                                   /* 0x22410 */
    volatile u_isp_dis_v4pos isp_dis_v4pos;                                   /* 0x22414 */
    volatile u_isp_dis_v8pos isp_dis_v8pos;                                   /* 0x22418 */
    volatile unsigned int reserved_55;                                        /* 0x2241c */
    volatile u_isp_dis_v0pose isp_dis_v0pose;                                 /* 0x22420 */
    volatile u_isp_dis_v4pose isp_dis_v4pose;                                 /* 0x22424 */
    volatile u_isp_dis_v8pose isp_dis_v8pose;                                 /* 0x22428 */
    volatile unsigned int reserved_56;                                        /* 0x2242c */
    volatile u_isp_dis_h0pos isp_dis_h0pos;                                   /* 0x22430 */
    volatile u_isp_dis_h4pos isp_dis_h4pos;                                   /* 0x22434 */
    volatile u_isp_dis_h8pos isp_dis_h8pos;                                   /* 0x22438 */
    volatile unsigned int reserved_57;                                        /* 0x2243c */
    volatile u_isp_dis_h0pose isp_dis_h0pose;                                 /* 0x22440 */
    volatile u_isp_dis_h4pose isp_dis_h4pose;                                 /* 0x22444 */
    volatile u_isp_dis_h8pose isp_dis_h8pose;                                 /* 0x22448 */
    volatile unsigned int reserved_58;                                        /* 0x2244c */
    volatile u_isp_dis_raw_luma isp_dis_raw_luma;                             /* 0x22450 */
    volatile u_isp_dis_gamma_en isp_dis_gamma_en;                             /* 0x22454 */
    volatile unsigned int reserved_59;                                        /* 0x22458 */
    volatile u_isp_dis_offset_gr isp_dis_offset_gr;                           /* 0x2245c */
    volatile u_isp_dis_offset_gb isp_dis_offset_gb;                           /* 0x22460 */
    volatile u_isp_dis_scale isp_dis_scale;                                   /* 0x22464 */
    volatile unsigned int reserved_60;                                        /* 0x22468 */
    volatile u_isp_dis_refinfo_update isp_dis_refinfo_update;                 /* 0x2246c */
    volatile u_isp_dis_prj_shift isp_dis_prj_shift;                           /* 0x22470 */
    volatile unsigned int reserved_61[3];                                     /* 0x22474~0x2247c */
    volatile u_isp_dis_stat_waddr isp_dis_stat_waddr;                         /* 0x22480 */
    volatile u_isp_dis_stat_wdata isp_dis_stat_wdata;                         /* 0x22484 */
    volatile u_isp_dis_stat_raddr isp_dis_stat_raddr;                         /* 0x22488 */
    volatile u_isp_dis_stat_rdata isp_dis_stat_rdata;                         /* 0x2248c */
    volatile unsigned int reserved_62[4];                                     /* 0x22490~0x2249c */
    volatile u_isp_dis_refinfo_waddr isp_dis_refinfo_waddr;                   /* 0x224a0 */
    volatile u_isp_dis_refinfo_wdata isp_dis_refinfo_wdata;                   /* 0x224a4 */
    volatile u_isp_dis_refinfo_raddr isp_dis_refinfo_raddr;                   /* 0x224a8 */
    volatile u_isp_dis_refinfo_rdata isp_dis_refinfo_rdata;                   /* 0x224ac */
    volatile unsigned int reserved_63[23];                                    /* 0x224b0~0x22508 */
    volatile u_isp_la_version isp_la_version;                                 /* 0x2250c */
    volatile u_isp_la_zone isp_la_zone;                                       /* 0x22510 */
    volatile unsigned int reserved_64[11];                                    /* 0x22514~0x2253c */
    volatile u_isp_la_bitmove isp_la_bitmove;                                 /* 0x22540 */
    volatile u_isp_la_offset_r isp_la_offset_r;                               /* 0x22544 */
    volatile u_isp_la_offset_gr isp_la_offset_gr;                             /* 0x22548 */
    volatile u_isp_la_offset_gb isp_la_offset_gb;                             /* 0x2254c */
    volatile u_isp_la_offset_b isp_la_offset_b;                               /* 0x22550 */
    volatile unsigned int reserved_65[3];                                     /* 0x22554~0x2255c */
    volatile u_isp_la_gamma_limit isp_la_gamma_limit;                         /* 0x22560 */
    volatile u_isp_la_crop_pos isp_la_crop_pos;                               /* 0x22564 */
    volatile u_isp_la_crop_outsize isp_la_crop_outsize;                       /* 0x22568 */
    volatile unsigned int reserved_66;                                        /* 0x2256c */
    volatile u_isp_la_stt_bst isp_la_stt_bst;                                 /* 0x22570 */
    volatile u_isp_la_stt_abn isp_la_stt_abn;                                 /* 0x22574 */
    volatile unsigned int reserved_67[6];                                     /* 0x22578~0x2258c */
    volatile u_isp_la_aver_waddr isp_la_aver_waddr;                           /* 0x22590 */
    volatile u_isp_la_aver_wdata isp_la_aver_wdata;                           /* 0x22594 */
    volatile u_isp_la_aver_raddr isp_la_aver_raddr;                           /* 0x22598 */
    volatile u_isp_la_aver_rdata isp_la_aver_rdata;                           /* 0x2259c */
    volatile unsigned int reserved_68[664];                                   /* 0x225a0~0x22ffc */
    volatile u_isp_lsc_mesh isp_lsc_mesh;                                     /* 0x23000 */
    volatile u_isp_lsc_blcen isp_lsc_blcen;                                   /* 0x23004 */
    volatile u_isp_lsc_blc0 isp_lsc_blc0;                                     /* 0x23008 */
    volatile u_isp_lsc_blc1 isp_lsc_blc1;                                     /* 0x2300c */
    volatile u_isp_lsc_right_shift           isp_lsc_right_shift            ; /* 0x23010 */
    volatile unsigned int reserved_69[572];                                   /* 0x23014~0x23900 */
    volatile u_isp_ge_blc_en isp_ge_blc_en;                                   /* 0x23904 */
    volatile unsigned int reserved_70;                                        /* 0x23908 */
    volatile u_isp_ge_version isp_ge_version;                                 /* 0x2390c */
    volatile u_isp_ge0_ct_th1 isp_ge0_ct_th1;                                 /* 0x23910 */
    volatile u_isp_ge0_ct_th2 isp_ge0_ct_th2;                                 /* 0x23914 */
    volatile u_isp_ge0_ct_th3 isp_ge0_ct_th3;                                 /* 0x23918 */
    volatile u_isp_ge0_ct_slope isp_ge0_ct_slope;                             /* 0x2391c */
    volatile unsigned int reserved_71[4];                                     /* 0x23920~0x2392c */
    volatile u_isp_ge0_blc_offset isp_ge0_blc_offset;                         /* 0x23930 */
    volatile unsigned int reserved_72[9];                                     /* 0x23934~0x23954 */
    volatile u_isp_ge_strength isp_ge_strength;                               /* 0x23958 */
    volatile unsigned int reserved_74[41];                                   /* 0x2395c~0x239fc */
    volatile u_isp_fpn_cfg isp_fpn_cfg;                                       /* 0x23a00 */
    volatile u_isp_fpn_calib_start isp_fpn_calib_start;                       /* 0x23a04 */
    volatile u_isp_fpn_corr_cfg isp_fpn_corr_cfg;                             /* 0x23a08 */
    volatile u_isp_fpn_stat isp_fpn_stat;                                     /* 0x23a0c */
    volatile u_isp_fpn_white_level isp_fpn_white_level;                       /* 0x23a10 */
    volatile unsigned int reserved_85;                                        /* 0x23a14 */
    volatile u_isp_fpn_divcoef isp_fpn_divcoef;                               /* 0x23a18 */
    volatile u_isp_fpn_framelog2 isp_fpn_framelog2;                           /* 0x23a1c */
    volatile u_isp_fpn_sum0 isp_fpn_sum0;                                     /* 0x23a20 */
    volatile u_isp_fpn_sum1 isp_fpn_sum1;                                     /* 0x23a24 */
    volatile unsigned int reserved_86[2];                                     /* 0x23a28~0x23a2c */
    volatile u_isp_fpn_corr0 isp_fpn_corr0;                                   /* 0x23a30 */
    volatile unsigned int reserved_87[3];                                     /* 0x23a34~0x23a3c */
    volatile u_isp_fpn_shift isp_fpn_shift;                                   /* 0x23a40 */
    volatile unsigned int reserved_88[3];                                     /* 0x23a44~0x23a4c */
    volatile u_isp_fpn_max_o isp_fpn_max_o;                                   /* 0x23a50 */
    volatile u_isp_fpn_overflowthr isp_fpn_overflowthr;                       /* 0x23a54 */
    volatile unsigned int reserved_89[2];                                     /* 0x23a58~0x23a5c */
    volatile u_isp_fpn_black_frame_out_sel isp_fpn_black_frame_out_sel;       /* 0x23a60 */
    volatile u_isp_fpn_overlap_cfg isp_fpn_overlap_cfg;                       /* 0x23a64 */
    volatile u_isp_fpn_calib_offline_set isp_fpn_calib_offline_set;           /* 0x23a68 */
    volatile u_isp_fpn_calib_stat_clear isp_fpn_calib_stat_clear;             /* 0x23a6c */
    volatile unsigned int reserved_222[102];                                  /* 0x23a70~0x23c04 */
    volatile u_isp_dpc_alpha isp_dpc_alpha;                                   /* 0x23c08 */
    volatile u_isp_dpc_version isp_dpc_version;                               /* 0x23c0c */
    volatile u_isp_dpc_mode isp_dpc_mode;                                     /* 0x23c10 */
    volatile unsigned int reserved_75;                                        /* 0x23c14 */
    volatile u_isp_dpc_set_use isp_dpc_set_use;                               /* 0x23c18 */
    volatile u_isp_dpc_methods_set_1 isp_dpc_methods_set_1;                   /* 0x23c1c */
    volatile unsigned int reserved_76[2];                                     /* 0x23c20~0x23c24 */
    volatile u_isp_dpc_line_thresh_1 isp_dpc_line_thresh_1;                   /* 0x23c28 */
    volatile u_isp_dpc_line_mad_fac_1 isp_dpc_line_mad_fac_1;                 /* 0x23c2c */
    volatile u_isp_dpc_pg_fac_1 isp_dpc_pg_fac_1;                             /* 0x23c30 */
    volatile unsigned int reserved_77;                                        /* 0x23c34 */
    volatile u_isp_dpc_rg_fac_1 isp_dpc_rg_fac_1;                             /* 0x23c38 */
    volatile unsigned int reserved_78[10];                                    /* 0x23c3c~0x23c60 */
    volatile u_isp_dpc_ro_limits isp_dpc_ro_limits;                           /* 0x23c64 */
    volatile unsigned int reserved_79[43];                                    /* 0x23c68~0x23d10 */
    volatile u_isp_dpc_bhardthr_en isp_dpc_bhardthr_en;                       /* 0x23d14 */
    volatile unsigned int reserved_80[2];                                     /* 0x23d18~0x23d1c */
    volatile u_isp_dpc_lch_thr_1 isp_dpc_lch_thr_1;                           /* 0x23d20 */
    volatile unsigned int reserved_81[13];                                     /* 0x23d24~0x23d54 */
    volatile u_isp_dpc_rg_fac_1_mtp isp_dpc_rg_fac_1_mtp;                     /* 0x23d58 */
    volatile unsigned int reserved_83[2];                                     /* 0x23d5c~0x23d60 */
    volatile u_isp_dpc_amp_coef isp_dpc_amp_coef;                             /* 0x23d64 */
    volatile unsigned int reserved_84[2];                                     /* 0x23d68~0x23d6c */
    volatile u_isp_dpc_soft_thr isp_dpc_soft_thr;                             /* 0x23d70 */
    volatile u_isp_dpc_rakeratio isp_dpc_rakeratio;                           /* 0x23d74 */
    volatile u_isp_dpc_dark_enhance isp_dpc_dark_enhance;                     /* 0x23d78 */
    volatile u_isp_dpc_line_kerdiff_fac isp_dpc_line_kerdiff_fac;             /* 0x23d7c */
    volatile unsigned int reserved_90[160];                                   /* 0x23a80~0x23ffc */
    volatile u_isp_demosaic_cfg1 isp_demosaic_cfg1;                           /* 0x24000 */
    volatile u_isp_demosaic_coef0 isp_demosaic_coef0;                         /* 0x24004 */
    volatile u_isp_demosaic_coef2 isp_demosaic_coef2;                         /* 0x24008 */
    volatile u_isp_demosaic_coef3 isp_demosaic_coef3;                         /* 0x2400c */
    volatile u_isp_demosaic_coef4 isp_demosaic_coef4;                         /* 0x24010 */
    volatile u_isp_demosaic_coef5 isp_demosaic_coef5;                         /* 0x24014 */
    volatile u_isp_demosaic_coef6 isp_demosaic_coef6;                         /* 0x24018 */
    volatile u_isp_demosaic_coef7 isp_demosaic_coef7;                         /* 0x2401c */
    volatile u_isp_demosaic_lcac_cnt_thr isp_demosaic_lcac_cnt_thr;           /* 0x24020 */
    volatile u_isp_demosaic_lcac_luma_rb_thr isp_demosaic_lcac_luma_rb_thr;   /* 0x24024 */
    volatile u_isp_demosaic_lcac_luma_g_thr isp_demosaic_lcac_luma_g_thr;     /* 0x24028 */
    volatile u_isp_demosaic_lcac_ratio_thd isp_demosaic_lcac_ratio_thd;       /* 0x2402c */
    volatile u_isp_demosaic_lcac_ratio_sft isp_demosaic_lcac_ratio_sft;       /* 0x24030 */
    volatile u_isp_demosaic_lcac_g_avg_thd isp_demosaic_lcac_g_avg_thd;       /* 0x24034 */
    volatile u_isp_demosaic_lcac_g_avg_sft isp_demosaic_lcac_g_avg_sft;       /* 0x24038 */
    volatile u_isp_demosaic_plat_thd isp_demosaic_plat_thd;                   /* 0x2403c */
    volatile u_isp_demosaic_over_exp_thd isp_demosaic_over_exp_thd;           /* 0x24040 */
    volatile u_isp_demosaic_b_diff_sft isp_demosaic_b_diff_sft;               /* 0x24044 */
    volatile u_isp_demosaic_mix_ratio isp_demosaic_mix_ratio;                 /* 0x24048 */
    volatile u_isp_demosaic_depurplectr isp_demosaic_depurplectr;             /* 0x2404c */
    volatile u_isp_demosaic_g_intp_ctrl isp_demosaic_g_intp_ctrl;             /* 0x24050 */
    volatile u_isp_demosaic_cc_hf_ratio isp_demosaic_cc_hf_ratio;             /* 0x24054 */
    volatile unsigned int reserved_91;                                        /* 0x24058 */
    volatile u_isp_demosaic_satu_thr isp_demosaic_satu_thr;                   /* 0x2405c */
    volatile u_isp_demosaic_cbcr_ratio_limit isp_demosaic_cbcr_ratio_limit;   /* 0x24060 */
    volatile u_isp_demosaic_fcr_gray_ratio isp_demosaic_fcr_gray_ratio;       /* 0x24064 */
    volatile u_isp_demosaic_cx_var_rate isp_demosaic_cx_var_rate;             /* 0x24068 */
    volatile unsigned int reserved_92[42];                                    /* 0x2406c~0x24110 */
    volatile u_isp_demosaic_gcac_diffthd isp_demosaic_gcac_diffthd;           /* 0x24114 */
    volatile u_isp_demosaic_gcac_lamda_reg isp_demosaic_gcac_lamda_reg;       /* 0x24118 */
    volatile u_isp_demosaic_gcac_lamdathd isp_demosaic_gcac_lamdathd;         /* 0x2411c */
    volatile u_isp_demosaic_gcac_thrbthd isp_demosaic_gcac_thrbthd;           /* 0x24120 */
    volatile u_isp_demosaic_gcac_lamdamul isp_demosaic_gcac_lamdamul;         /* 0x24124 */
    volatile u_isp_demosaic_gcac_distance_thd isp_demosaic_gcac_distance_thd; /* 0x24128 */
    volatile u_isp_demosaic_gcac_distance_mul isp_demosaic_gcac_distance_mul; /* 0x2412c */
    volatile u_isp_demosaic_gcac_edge_alpha isp_demosaic_gcac_edge_alpha;     /* 0x24130 */
    volatile unsigned int reserved_93[7];                                     /* 0x24134~0x2414c */
    volatile u_isp_demosaic_filter_blur_str_1 isp_demosaic_filter_blur_str_1; /* 0x24150 */
    volatile u_isp_demosaic_filter_blur_str_2 isp_demosaic_filter_blur_str_2; /* 0x24154 */
    volatile u_isp_demosaic_fcr_hf_thresh isp_demosaic_fcr_hf_thresh;         /* 0x24158 */
    volatile u_isp_demosaic_nddm_coef isp_demosaic_nddm_coef;                 /* 0x2415c */
    volatile u_isp_demosaic_desat_thr isp_demosaic_desat_thr;                 /* 0x24160 */
    volatile u_isp_demosaic_desat_bldr isp_demosaic_desat_bldr;               /* 0x24164 */
    volatile u_isp_demosaic_desat_bldr_ratio isp_demosaic_desat_bldr_ratio;   /* 0x24168 */
    volatile u_isp_demosaic_desat_protect isp_demosaic_desat_protect;         /* 0x2416c */
    volatile unsigned int reserved_94[420];                                   /* 0x24170~0x247fc */
    volatile u_isp_rgbir_cfg isp_rgbir_cfg;                                   /* 0x24800 */
    volatile u_isp_rgbir_thre isp_rgbir_thre;                                 /* 0x24804 */
    volatile unsigned int reserved_95;                                        /* 0x24808 */
    volatile u_isp_rgbir_version isp_rgbir_version;                           /* 0x2480c */
    volatile u_isp_rgbir_exp_thr isp_rgbir_exp_thr;                           /* 0x24810 */
    volatile u_isp_rgbir_reci_ctl isp_rgbir_reci_ctl;                         /* 0x24814 */
    volatile u_isp_rgbir_gain isp_rgbir_gain;                                 /* 0x24818 */
    volatile unsigned int reserved_96;                                        /* 0x2481c */
    volatile u_isp_rgbir_cvt01 isp_rgbir_cvt01;                               /* 0x24820 */
    volatile u_isp_rgbir_cvt23 isp_rgbir_cvt23;                               /* 0x24824 */
    volatile u_isp_rgbir_cvt45 isp_rgbir_cvt45;                               /* 0x24828 */
    volatile u_isp_rgbir_cvt67 isp_rgbir_cvt67;                               /* 0x2482c */
    volatile u_isp_rgbir_cvt89 isp_rgbir_cvt89;                               /* 0x24830 */
    volatile u_isp_rgbir_cvt1011 isp_rgbir_cvt1011;                           /* 0x24834 */
    volatile unsigned int reserved_97[2];                                     /* 0x24838~0x2483c */
    volatile u_isp_rgbir_blc_offset_r isp_rgbir_blc_offset_r;                 /* 0x24840 */
    volatile u_isp_rgbir_blc_offset_g isp_rgbir_blc_offset_g;                 /* 0x24844 */
    volatile u_isp_rgbir_blc_offset_b isp_rgbir_blc_offset_b;                 /* 0x24848 */
    volatile u_isp_rgbir_blc_offset_ir isp_rgbir_blc_offset_ir;               /* 0x2484c */
    volatile u_isp_rgbir_ir_sum isp_rgbir_ir_sum;                             /* 0x24850 */
    volatile unsigned int reserved_98[622];                                   /* 0x24854~0x25208 */
    volatile u_isp_sharpen_version isp_sharpen_version;                       /* 0x2520c */
    volatile u_isp_sharpen_ctrl isp_sharpen_ctrl;                             /* 0x25210 */
    volatile u_isp_sharpen_mhfthd isp_sharpen_mhfthd;                         /* 0x25214 */
    volatile unsigned int reserved_99;                                        /* 0x25218 */
    volatile u_isp_sharpen_dirvar isp_sharpen_dirvar;                         /* 0x2521c */
    volatile u_isp_sharpen_dirdiff isp_sharpen_dirdiff;                       /* 0x25220 */
    volatile u_isp_sharpen_lumawgt0 isp_sharpen_lumawgt0;                     /* 0x25224 */
    volatile u_isp_sharpen_lumawgt1 isp_sharpen_lumawgt1;                     /* 0x25228 */
    volatile u_isp_sharpen_lumawgt2 isp_sharpen_lumawgt2;                     /* 0x2522c */
    volatile u_isp_sharpen_lumawgt3 isp_sharpen_lumawgt3;                     /* 0x25230 */
    volatile u_isp_sharpen_lumawgt4 isp_sharpen_lumawgt4;                     /* 0x25234 */
    volatile u_isp_sharpen_lumawgt5 isp_sharpen_lumawgt5;                     /* 0x25238 */
    volatile u_isp_sharpen_lumawgt6 isp_sharpen_lumawgt6;                     /* 0x2523c */
    volatile u_isp_sharpen_luma_thd isp_sharpen_luma_thd;                     /* 0x25240 */
    volatile u_isp_sharpen_shoot_amt isp_sharpen_shoot_amt;                   /* 0x25244 */
    volatile u_isp_sharpen_shoot_maxchg isp_sharpen_shoot_maxchg;             /* 0x25248 */
    volatile u_isp_sharpen_shtvar isp_sharpen_shtvar;                         /* 0x2524c */
    volatile u_isp_sharpen_hard_luma_thd isp_sharpen_hard_luma_thd;           /* 0x25250 */
    volatile u_isp_sharpen_oshtvar isp_sharpen_oshtvar;                       /* 0x25254 */
    volatile u_isp_sharpen_exluma_mul isp_sharpen_exluma_mul;                 /* 0x25258 */
    volatile u_isp_sharpen_shtvar_mul isp_sharpen_shtvar_mul;                 /* 0x2525c */
    volatile u_isp_sharpen_oshtvardiff isp_sharpen_oshtvardiff;               /* 0x25260 */
    volatile unsigned int reserved_100[2];                                    /* 0x25264~0x25268 */
    volatile u_isp_sharpen_lmtmf0 isp_sharpen_lmtmf0;                         /* 0x2526c */
    volatile u_isp_sharpen_lmtmf1 isp_sharpen_lmtmf1;                         /* 0x25270 */
    volatile u_isp_sharpen_lmthf0 isp_sharpen_lmthf0;                         /* 0x25274 */
    volatile u_isp_sharpen_lmthf1 isp_sharpen_lmthf1;                         /* 0x25278 */
    volatile unsigned int reserved_104;                                       /* 0x2527c */
    volatile u_isp_sharpen_mhfgaind_waddr isp_sharpen_mhfgaind_waddr;         /* 0x25280 */
    volatile u_isp_sharpen_mhfgaind_wdata isp_sharpen_mhfgaind_wdata;         /* 0x25284 */
    volatile u_isp_sharpen_mhfgaind_raddr isp_sharpen_mhfgaind_raddr;         /* 0x25288 */
    volatile u_isp_sharpen_mhfgaind_rdata isp_sharpen_mhfgaind_rdata;         /* 0x2528c */
    volatile u_isp_sharpen_mhfgainud_waddr isp_sharpen_mhfgainud_waddr;       /* 0x25290 */
    volatile u_isp_sharpen_mhfgainud_wdata isp_sharpen_mhfgainud_wdata;       /* 0x25294 */
    volatile u_isp_sharpen_mhfgainud_raddr isp_sharpen_mhfgainud_raddr;       /* 0x25298 */
    volatile u_isp_sharpen_mhfgainud_rdata isp_sharpen_mhfgainud_rdata;       /* 0x2529c */
    volatile unsigned int reserved_105[25];                                   /* 0x252a0~0x25300 */
    volatile u_isp_sharpen_skin_u isp_sharpen_skin_u;                         /* 0x25304 */
    volatile u_isp_sharpen_skin_v isp_sharpen_skin_v;                         /* 0x25308 */
    volatile u_isp_sharpen_skin_cnt isp_sharpen_skin_cnt;                     /* 0x2530c */
    volatile u_isp_sharpen_skin_edge isp_sharpen_skin_edge;                   /* 0x25310 */
    volatile u_isp_sharpen_skin_edgethd isp_sharpen_skin_edgethd;             /* 0x25314 */
    volatile unsigned int reserved_105_1[2];                                    /* 0x25318~0x2531c */
    volatile u_isp_sharpen_chrr_var isp_sharpen_chrr_var;                     /* 0x25320 */
    volatile unsigned int reserved_105_2;                                     /* 0x25324 */
    volatile u_isp_sharpen_chrr_thd isp_sharpen_chrr_thd;                     /* 0x25328 */
    volatile u_isp_sharpen_chrr_gain isp_sharpen_chrr_gain;                   /* 0x2532c */
    volatile u_isp_sharpen_chrg_mul isp_sharpen_chrg_mul;                     /* 0x25330 */
    volatile u_isp_sharpen_chrg_sft isp_sharpen_chrg_sft;                     /* 0x25334 */
    volatile unsigned int reserved_105_3;                                     /* 0x25338 */
    volatile u_isp_sharpen_chrg_thd isp_sharpen_chrg_thd;                     /* 0x2533c */
    volatile u_isp_sharpen_chrg_gain isp_sharpen_chrg_gain;                   /* 0x25340 */
    volatile u_isp_sharpen_chrb_var isp_sharpen_chrb_var;                     /* 0x25344 */
    volatile unsigned int reserved_105_4;                                     /* 0x25348 */
    volatile u_isp_sharpen_chrb_thd isp_sharpen_chrb_thd;                     /* 0x2534c */
    volatile u_isp_sharpen_chrb_gain isp_sharpen_chrb_gain;                   /* 0x25350 */
    volatile u_isp_sharpen_gain_sft isp_sharpen_gain_sft;                     /* 0x25354 */
    volatile u_isp_sharpen_shoot_maxgain isp_sharpen_shoot_maxgain;           /* 0x25358 */
    volatile u_isp_sharpen_detail_mul isp_sharpen_detail_mul;                 /* 0x2535c */
    volatile u_isp_sharpen_osht_detail isp_sharpen_osht_detail;               /* 0x25360 */
    volatile u_isp_sharpen_usht_detail isp_sharpen_usht_detail;               /* 0x25364 */
    volatile unsigned int reserved_106[2];                                    /* 0x25368~0x2536c */
    volatile u_isp_sharpen_stt2lut_cfg isp_sharpen_stt2lut_cfg;               /* 0x25370 */
    volatile u_isp_sharpen_stt2lut_regnew isp_sharpen_stt2lut_regnew;         /* 0x25374 */
    volatile u_isp_sharpen_stt2lut_abn isp_sharpen_stt2lut_abn;               /* 0x25378 */
    volatile unsigned int reserved_107[33];                                   /* 0x2537c~0x253fc */
    volatile u_isp_sharpen_std_gain_bymot0   isp_sharpen_std_gain_bymot0      ; /* 0x25400 */
    volatile u_isp_sharpen_std_gain_bymot1   isp_sharpen_std_gain_bymot1      ; /* 0x25404 */
    volatile u_isp_sharpen_std_gain_bymot2   isp_sharpen_std_gain_bymot2      ; /* 0x25408 */
    volatile u_isp_sharpen_std_gain_bymot3   isp_sharpen_std_gain_bymot3      ; /* 0x2540c */
    volatile u_isp_sharpen_std_gain_byy0     isp_sharpen_std_gain_byy0        ; /* 0x25410 */
    volatile u_isp_sharpen_std_gain_byy1     isp_sharpen_std_gain_byy1        ; /* 0x25414 */
    volatile u_isp_sharpen_std_gain_byy2     isp_sharpen_std_gain_byy2        ; /* 0x25418 */
    volatile u_isp_sharpen_std_gain_byy3     isp_sharpen_std_gain_byy3        ; /* 0x2541c */
    volatile u_isp_sharpen_std_offset_bymot0   isp_sharpen_std_offset_bymot0    ; /* 0x25420 */
    volatile u_isp_sharpen_std_offset_bymot1   isp_sharpen_std_offset_bymot1    ; /* 0x25424 */
    volatile u_isp_sharpen_std_offset_bymot2   isp_sharpen_std_offset_bymot2    ; /* 0x25428 */
    volatile u_isp_sharpen_std_offset_bymot3   isp_sharpen_std_offset_bymot3    ; /* 0x2542c */
    volatile u_isp_sharpen_std_offset_byy0   isp_sharpen_std_offset_byy0      ; /* 0x25430 */
    volatile u_isp_sharpen_std_offset_byy1   isp_sharpen_std_offset_byy1      ; /* 0x25434 */
    volatile u_isp_sharpen_std_offset_byy2   isp_sharpen_std_offset_byy2      ; /* 0x25438 */
    volatile u_isp_sharpen_std_offset_byy3   isp_sharpen_std_offset_byy3      ; /* 0x2543c */
    volatile u_isp_sharpen_lmf_mot_gain0     isp_sharpen_lmf_mot_gain0        ; /* 0x25440 */
    volatile u_isp_sharpen_lmf_mot_gain1     isp_sharpen_lmf_mot_gain1        ; /* 0x25444 */
    volatile u_isp_sharpen_lmf_mot_gain2     isp_sharpen_lmf_mot_gain2        ; /* 0x25448 */
    volatile u_isp_sharpen_lmf_mot_gain3     isp_sharpen_lmf_mot_gain3        ; /* 0x2544c */
    volatile u_isp_sharpen_mf_mot_dec0       isp_sharpen_mf_mot_dec0          ; /* 0x25450 */
    volatile u_isp_sharpen_mf_mot_dec1       isp_sharpen_mf_mot_dec1          ; /* 0x25454 */
    volatile u_isp_sharpen_mf_mot_dec2       isp_sharpen_mf_mot_dec2          ; /* 0x25458 */
    volatile u_isp_sharpen_mf_mot_dec3       isp_sharpen_mf_mot_dec3          ; /* 0x2545c */
    volatile u_isp_sharpen_hf_mot_dec0       isp_sharpen_hf_mot_dec0          ; /* 0x25460 */
    volatile u_isp_sharpen_hf_mot_dec1 isp_sharpen_hf_mot_dec1                ; /* 0x25464 */
    volatile u_isp_sharpen_hf_mot_dec2 isp_sharpen_hf_mot_dec2;               ; /* 0x25468 */
    volatile u_isp_sharpen_hf_mot_dec3 isp_sharpen_hf_mot_dec3                ; /* 0x2546c */
    volatile u_isp_sharpen_rly_wgt0 isp_sharpen_rly_wgt0                      ; /* 0x25470 */
    volatile u_isp_sharpen_rly_wgt1 isp_sharpen_rly_wgt1                      ; /* 0x25474 */
    volatile u_isp_sharpen_rly_wgt2 isp_sharpen_rly_wgt2                      ; /* 0x25478 */
    volatile u_isp_sharpen_rly_wgt3 isp_sharpen_rly_wgt3                      ; /* 0x2547c */
    unsigned int                             reserved_130[2];        /* 0x25480~0x25484 */
    volatile u_isp_sharpen_mot_cfg isp_sharpen_mot_cfg;               /* 0x25488 */
    volatile u_isp_sharpen_var5_thd isp_sharpen_var5_thd;             /* 0x2548c */
    volatile unsigned int reserved_131[2];                            /* 0x25490~0x25494 */
    volatile u_isp_sharpen_var7_thd          isp_sharpen_var7_thd             ; /* 0x25498 */
    volatile u_isp_sharpen_shoot_edge_amt    isp_sharpen_shoot_edge_amt       ; /* 0x2549c */
    volatile unsigned int                    reserved_107_1[24]                 ; /* 0x254a0~0x254fc */
    volatile u_isp_sharpen_ldci_dither_cfg   isp_sharpen_ldci_dither_cfg      ; /* 0x25500 */
    volatile unsigned int                    reserved_110[191]                ; /* 0x25504~0x257fc */
    volatile u_isp_bnr_cfg isp_bnr_cfg;                                       /* 0x25800 */
    volatile unsigned int reserved_111[2];                                    /* 0x25804~0x25808 */
    volatile u_isp_bnr_version isp_bnr_version;                               /* 0x2580c */
    volatile u_isp_bnr_cfg0 isp_bnr_cfg0;                                        /* 0x25810 */
    volatile u_isp_bnr_cfg1 isp_bnr_cfg1;                                        /* 0x25814 */
    volatile u_isp_bnr_cfg2 isp_bnr_cfg2;                                        /* 0x25818 */
    volatile u_isp_bnr_cfg3 isp_bnr_cfg3;                                        /* 0x2581c */
    volatile u_isp_bnr_cfg4 isp_bnr_cfg4;                                        /* 0x25820 */
    volatile u_isp_bnr_cfg5 isp_bnr_cfg5;                                        /* 0x25824 */
    volatile u_isp_bnr_cfg6 isp_bnr_cfg6;                                        /* 0x25828 */
    volatile u_isp_bnr_cfg7 isp_bnr_cfg7;                                        /* 0x2582c */
    volatile u_isp_bnr_cfg8 isp_bnr_cfg8;                                        /* 0x25830 */
    volatile u_isp_bnr_cfg9 isp_bnr_cfg9;                                        /* 0x25834 */
    volatile u_isp_bnr_cfg10 isp_bnr_cfg10;                                      /* 0x25838 */
    volatile u_isp_bnr_cfg11 isp_bnr_cfg11;                                      /* 0x2583c */
    volatile u_isp_bnr_cfg12 isp_bnr_cfg12;                                      /* 0x25840 */
    volatile u_isp_bnr_cfg13 isp_bnr_cfg13;                                      /* 0x25844 */
    volatile u_isp_bnr_cfg14 isp_bnr_cfg14;                                      /* 0x25848 */
    volatile u_isp_bnr_cfg15 isp_bnr_cfg15;                                      /* 0x2584c */
    volatile u_isp_bnr_cfg16                 isp_bnr_cfg16                    ; /* 0x25850 */
    volatile u_isp_bnr_cfg17                 isp_bnr_cfg17                    ; /* 0x25854 */
    volatile unsigned int reserved_190[7];                                       /* 0x25858~0x25870 */
    volatile u_isp_bnr_stt2lut_cfg isp_bnr_stt2lut_cfg;                          /* 0x25874 */
    volatile u_isp_bnr_stt2lut_regnew isp_bnr_stt2lut_regnew;                    /* 0x25878 */
    volatile u_isp_bnr_stt2lut_abn isp_bnr_stt2lut_abn;                          /* 0x2587c */
    volatile u_isp_bnr_noisesd_even_waddr isp_bnr_noisesd_even_waddr;            /* 0x25880 */
    volatile u_isp_bnr_noisesd_even_wdata isp_bnr_noisesd_even_wdata;            /* 0x25884 */
    volatile u_isp_bnr_noisesd_even_raddr isp_bnr_noisesd_even_raddr;            /* 0x25888 */
    volatile u_isp_bnr_noisesd_even_rdata isp_bnr_noisesd_even_rdata;            /* 0x2588c */
    volatile u_isp_bnr_noisesd_odd_waddr isp_bnr_noisesd_odd_waddr;              /* 0x25890 */
    volatile u_isp_bnr_noisesd_odd_wdata isp_bnr_noisesd_odd_wdata;              /* 0x25894 */
    volatile u_isp_bnr_noisesd_odd_raddr isp_bnr_noisesd_odd_raddr;              /* 0x25898 */
    volatile u_isp_bnr_noisesd_odd_rdata isp_bnr_noisesd_odd_rdata;              /* 0x2589c */
    volatile u_isp_bnr_noiseinv_even_waddr isp_bnr_noiseinv_even_waddr;          /* 0x258a0 */
    volatile u_isp_bnr_noiseinv_even_wdata isp_bnr_noiseinv_even_wdata;          /* 0x258a4 */
    volatile u_isp_bnr_noiseinv_even_raddr isp_bnr_noiseinv_even_raddr;          /* 0x258a8 */
    volatile u_isp_bnr_noiseinv_even_rdata isp_bnr_noiseinv_even_rdata;          /* 0x258ac */
    volatile u_isp_bnr_noiseinv_odd_waddr isp_bnr_noiseinv_odd_waddr;            /* 0x258b0 */
    volatile u_isp_bnr_noiseinv_odd_wdata isp_bnr_noiseinv_odd_wdata;            /* 0x258b4 */
    volatile u_isp_bnr_noiseinv_odd_raddr isp_bnr_noiseinv_odd_raddr;            /* 0x258b8 */
    volatile u_isp_bnr_noiseinv_odd_rdata isp_bnr_noiseinv_odd_rdata;            /* 0x258bc */
    volatile u_isp_bnr_fbratiotable_waddr isp_bnr_fbratiotable_waddr;            /* 0x258c0 */
    volatile u_isp_bnr_fbratiotable_wdata isp_bnr_fbratiotable_wdata;            /* 0x258c4 */
    volatile u_isp_bnr_fbratiotable_raddr isp_bnr_fbratiotable_raddr;            /* 0x258c8 */
    volatile u_isp_bnr_fbratiotable_rdata isp_bnr_fbratiotable_rdata;            /* 0x258cc */
    volatile u_isp_bnr_noisesds_even_waddr isp_bnr_noisesds_even_waddr;          /* 0x258d0 */
    volatile u_isp_bnr_noisesds_even_wdata isp_bnr_noisesds_even_wdata;          /* 0x258d4 */
    volatile u_isp_bnr_noisesds_even_raddr isp_bnr_noisesds_even_raddr;          /* 0x258d8 */
    volatile u_isp_bnr_noisesds_even_rdata isp_bnr_noisesds_even_rdata;          /* 0x258dc */
    volatile unsigned int reserved_191[40];                                      /* 0x258e0~0x2597c */
    volatile u_isp_bnr_noisesds_odd_waddr isp_bnr_noisesds_odd_waddr;            /* 0x25980 */
    volatile u_isp_bnr_noisesds_odd_wdata isp_bnr_noisesds_odd_wdata;            /* 0x25984 */
    volatile u_isp_bnr_noisesds_odd_raddr isp_bnr_noisesds_odd_raddr;            /* 0x25988 */
    volatile u_isp_bnr_noisesds_odd_rdata isp_bnr_noisesds_odd_rdata;            /* 0x2598c */
    volatile u_isp_bnr_noiseinv_mag_even_waddr isp_bnr_noiseinv_mag_even_waddr;  /* 0x25990 */
    volatile u_isp_bnr_noiseinv_mag_even_wdata isp_bnr_noiseinv_mag_even_wdata;  /* 0x25994 */
    volatile u_isp_bnr_noiseinv_mag_even_raddr isp_bnr_noiseinv_mag_even_raddr;  /* 0x25998 */
    volatile u_isp_bnr_noiseinv_mag_even_rdata isp_bnr_noiseinv_mag_even_rdata;  /* 0x2599c */
    volatile u_isp_bnr_noiseinv_mag_odd_waddr isp_bnr_noiseinv_mag_odd_waddr;    /* 0x259a0 */
    volatile u_isp_bnr_noiseinv_mag_odd_wdata isp_bnr_noiseinv_mag_odd_wdata;    /* 0x259a4 */
    volatile u_isp_bnr_noiseinv_mag_odd_raddr isp_bnr_noiseinv_mag_odd_raddr;    /* 0x259a8 */
    volatile u_isp_bnr_noiseinv_mag_odd_rdata isp_bnr_noiseinv_mag_odd_rdata;    /* 0x259ac */
    volatile u_isp_bnr_coring_low_even_waddr isp_bnr_coring_low_even_waddr;      /* 0x259b0 */
    volatile u_isp_bnr_coring_low_even_wdata isp_bnr_coring_low_even_wdata;      /* 0x259b4 */
    volatile u_isp_bnr_coring_low_even_raddr isp_bnr_coring_low_even_raddr;      /* 0x259b8 */
    volatile u_isp_bnr_coring_low_even_rdata isp_bnr_coring_low_even_rdata;      /* 0x259bc */
    volatile u_isp_bnr_coring_low_odd_waddr isp_bnr_coring_low_odd_waddr;        /* 0x259c0 */
    volatile u_isp_bnr_coring_low_odd_wdata isp_bnr_coring_low_odd_wdata;        /* 0x259c4 */
    volatile u_isp_bnr_coring_low_odd_raddr isp_bnr_coring_low_odd_raddr;        /* 0x259c8 */
    volatile u_isp_bnr_coring_low_odd_rdata isp_bnr_coring_low_odd_rdata;        /* 0x259cc */
    volatile u_isp_bnr_fbra_mot2yuv_waddr    isp_bnr_fbra_mot2yuv_waddr       ; /* 0x259d0 */
    volatile u_isp_bnr_fbra_mot2yuv_wdata    isp_bnr_fbra_mot2yuv_wdata       ; /* 0x259d4 */
    volatile u_isp_bnr_fbra_mot2yuv_raddr    isp_bnr_fbra_mot2yuv_raddr       ; /* 0x259d8 */
    volatile u_isp_bnr_fbra_mot2yuv_rdata    isp_bnr_fbra_mot2yuv_rdata       ; /* 0x259dc */
    volatile unsigned int reserved_192[8];                                       /* 0x259e0~0x259fc */
    volatile u_isp_mlsc_stt2lut_cfg isp_mlsc_stt2lut_cfg;                     /* 0x25a00 */
    volatile u_isp_mlsc_stt2lut_regnew isp_mlsc_stt2lut_regnew;               /* 0x25a04 */
    volatile u_isp_mlsc_stt2lut_abn isp_mlsc_stt2lut_abn;                     /* 0x25a08 */
    volatile unsigned int reserved_116;                                       /* 0x25a0c */
    volatile u_isp_mlsc_winnum isp_mlsc_winnum;                               /* 0x25a10 */
    volatile u_isp_mlsc_winx0 isp_mlsc_winx0;                                 /* 0x25a14 */
    volatile u_isp_mlsc_winx1 isp_mlsc_winx1;                                 /* 0x25a18 */
    volatile u_isp_mlsc_winx2 isp_mlsc_winx2;                                 /* 0x25a1c */
    volatile u_isp_mlsc_winx3 isp_mlsc_winx3;                                 /* 0x25a20 */
    volatile u_isp_mlsc_winx4 isp_mlsc_winx4;                                 /* 0x25a24 */
    volatile u_isp_mlsc_winx5 isp_mlsc_winx5;                                 /* 0x25a28 */
    volatile u_isp_mlsc_winx6 isp_mlsc_winx6;                                 /* 0x25a2c */
    volatile u_isp_mlsc_winx7 isp_mlsc_winx7;                                 /* 0x25a30 */
    volatile u_isp_mlsc_winx8 isp_mlsc_winx8;                                 /* 0x25a34 */
    volatile u_isp_mlsc_winx9 isp_mlsc_winx9;                                 /* 0x25a38 */
    volatile u_isp_mlsc_winx10 isp_mlsc_winx10;                               /* 0x25a3c */
    volatile u_isp_mlsc_winx11 isp_mlsc_winx11;                               /* 0x25a40 */
    volatile u_isp_mlsc_winx12 isp_mlsc_winx12;                               /* 0x25a44 */
    volatile u_isp_mlsc_winx13 isp_mlsc_winx13;                               /* 0x25a48 */
    volatile u_isp_mlsc_winx14 isp_mlsc_winx14;                               /* 0x25a4c */
    volatile u_isp_mlsc_winx15 isp_mlsc_winx15;                               /* 0x25a50 */
    volatile u_isp_mlsc_winy0 isp_mlsc_winy0;                                 /* 0x25a54 */
    volatile u_isp_mlsc_winy1 isp_mlsc_winy1;                                 /* 0x25a58 */
    volatile u_isp_mlsc_winy2 isp_mlsc_winy2;                                 /* 0x25a5c */
    volatile u_isp_mlsc_winy3 isp_mlsc_winy3;                                 /* 0x25a60 */
    volatile u_isp_mlsc_winy4 isp_mlsc_winy4;                                 /* 0x25a64 */
    volatile u_isp_mlsc_winy5 isp_mlsc_winy5;                                 /* 0x25a68 */
    volatile u_isp_mlsc_winy6 isp_mlsc_winy6;                                 /* 0x25a6c */
    volatile u_isp_mlsc_winy7 isp_mlsc_winy7;                                 /* 0x25a70 */
    volatile u_isp_mlsc_width_offset isp_mlsc_width_offset;                   /* 0x25a74 */
    volatile u_isp_mlsc_mesh isp_mlsc_mesh;                                   /* 0x25a78 */
    volatile unsigned int reserved_117;                                       /* 0x25a7c */
    volatile u_isp_mlsc_rgain_waddr isp_mlsc_rgain_waddr;                     /* 0x25a80 */
    volatile u_isp_mlsc_rgain_wdata isp_mlsc_rgain_wdata;                     /* 0x25a84 */
    volatile u_isp_mlsc_rgain_raddr isp_mlsc_rgain_raddr;                     /* 0x25a88 */
    volatile u_isp_mlsc_rgain_rdata isp_mlsc_rgain_rdata;                     /* 0x25a8c */
    volatile u_isp_mlsc_grgain_waddr isp_mlsc_grgain_waddr;                   /* 0x25a90 */
    volatile u_isp_mlsc_grgain_wdata isp_mlsc_grgain_wdata;                   /* 0x25a94 */
    volatile u_isp_mlsc_grgain_raddr isp_mlsc_grgain_raddr;                   /* 0x25a98 */
    volatile u_isp_mlsc_grgain_rdata isp_mlsc_grgain_rdata;                   /* 0x25a9c */
    volatile u_isp_mlsc_bgain_waddr isp_mlsc_bgain_waddr;                     /* 0x25aa0 */
    volatile u_isp_mlsc_bgain_wdata isp_mlsc_bgain_wdata;                     /* 0x25aa4 */
    volatile u_isp_mlsc_bgain_raddr isp_mlsc_bgain_raddr;                     /* 0x25aa8 */
    volatile u_isp_mlsc_bgain_rdata isp_mlsc_bgain_rdata;                     /* 0x25aac */
    volatile u_isp_mlsc_gbgain_waddr isp_mlsc_gbgain_waddr;                   /* 0x25ab0 */
    volatile u_isp_mlsc_gbgain_wdata isp_mlsc_gbgain_wdata;                   /* 0x25ab4 */
    volatile u_isp_mlsc_gbgain_raddr isp_mlsc_gbgain_raddr;                   /* 0x25ab8 */
    volatile u_isp_mlsc_gbgain_rdata isp_mlsc_gbgain_rdata;                   /* 0x25abc */
    volatile unsigned int reserved_195[16];                               /* 0x25ac0~0x25afc */
    volatile u_isp_mlsc_winx16 isp_mlsc_winx16;                           /* 0x25b00 */
    volatile u_isp_mlsc_winx17 isp_mlsc_winx17;                           /* 0x25b04 */
    volatile u_isp_mlsc_winx18 isp_mlsc_winx18;                           /* 0x25b08 */
    volatile u_isp_mlsc_winx19 isp_mlsc_winx19;                           /* 0x25b0c */
    volatile u_isp_mlsc_winx20 isp_mlsc_winx20;                           /* 0x25b10 */
    volatile u_isp_mlsc_winx21 isp_mlsc_winx21;                           /* 0x25b14 */
    volatile u_isp_mlsc_winx22 isp_mlsc_winx22;                           /* 0x25b18 */
    volatile u_isp_mlsc_winx23 isp_mlsc_winx23;                           /* 0x25b1c */
    volatile u_isp_mlsc_winx24               isp_mlsc_winx24                  ; /* 0x25b20 */
    volatile u_isp_mlsc_winx25               isp_mlsc_winx25                  ; /* 0x25b24 */
    volatile u_isp_mlsc_winx26               isp_mlsc_winx26                  ; /* 0x25b28 */
    volatile u_isp_mlsc_winx27               isp_mlsc_winx27                  ; /* 0x25b2c */
    volatile u_isp_mlsc_winx28               isp_mlsc_winx28                  ; /* 0x25b30 */
    volatile u_isp_mlsc_winx29               isp_mlsc_winx29                  ; /* 0x25b34 */
    volatile u_isp_mlsc_winx30               isp_mlsc_winx30                  ; /* 0x25b38 */
    volatile u_isp_mlsc_winx31               isp_mlsc_winx31                  ; /* 0x25b3c */
    volatile u_isp_mlsc_winy8 isp_mlsc_winy8;                             /* 0x25b40 */
    volatile unsigned int reserved_197[307];                              /* 0x25b44~0x2600c */
    volatile u_isp_wdr_ctrl isp_wdr_ctrl;                                     /* 0x26010 */
    volatile unsigned int reserved_121;                                       /* 0x26014 */
    volatile u_isp_wdr_f0_inblc0 isp_wdr_f0_inblc0;                           /* 0x26018 */
    volatile u_isp_wdr_f0_inblc1 isp_wdr_f0_inblc1;                           /* 0x2601c */
    volatile u_isp_wdr_f1_inblc0 isp_wdr_f1_inblc0;                           /* 0x26020 */
    volatile u_isp_wdr_f1_inblc1 isp_wdr_f1_inblc1;                           /* 0x26024 */
    volatile u_isp_wdr_outblc isp_wdr_outblc;                                 /* 0x26028 */
    volatile u_isp_wdr_expovalue isp_wdr_expovalue;                           /* 0x2602c */
    volatile u_isp_wdr_exporratio isp_wdr_exporratio;                         /* 0x26030 */
    volatile u_isp_wdr_blc_comp isp_wdr_blc_comp;                             /* 0x26034 */
    volatile u_isp_wdr_maxratio isp_wdr_maxratio;                             /* 0x26038 */
    volatile u_isp_wdr_saturate_thr isp_wdr_saturate_thr;                     /* 0x2603c */
    volatile u_isp_wdr_still_thr isp_wdr_still_thr;                           /* 0x26040 */
    volatile u_isp_wdr_mdtlbld isp_wdr_mdtlbld;                               /* 0x26044 */
    volatile u_isp_wdr_mdt_thr isp_wdr_mdt_thr;                               /* 0x26048 */
    volatile u_isp_wdr_forcelong_para isp_wdr_forcelong_para;                 /* 0x2604c */
    volatile u_isp_wdr_forcelong_slope isp_wdr_forcelong_slope;               /* 0x26050 */
    volatile u_isp_wdr_mdt_nosf_thr isp_wdr_mdt_nosf_thr;                     /* 0x26054 */
    volatile u_isp_wdr_gain_sum_thr isp_wdr_gain_sum_thr;                     /* 0x26058 */
    volatile u_isp_wdr_shortexpo_chk isp_wdr_shortexpo_chk;                   /* 0x2605c */
    volatile u_isp_wdr_pix_avg_diff isp_wdr_pix_avg_diff;                     /* 0x26060 */
    volatile u_isp_wdr_mask_similar isp_wdr_mask_similar;                     /* 0x26064 */
    volatile u_isp_wdr_wgtidx_blendratio isp_wdr_wgtidx_blendratio;           /* 0x26068 */
    volatile u_isp_wdr_wgtidx_thr isp_wdr_wgtidx_thr;                         /* 0x2606c */
    volatile u_isp_wdr_dftwgt_fl isp_wdr_dftwgt_fl;                           /* 0x26070 */
    volatile u_isp_wdr_wgt_slope isp_wdr_wgt_slope;                           /* 0x26074 */
    volatile unsigned int reserved_122[49];                                   /* 0x26078~0x26138 */
    volatile u_isp_wdr_fusion_thr_r isp_wdr_fusion_thr_r;                     /* 0x2613c */
    volatile u_isp_wdr_fusion_thr_b isp_wdr_fusion_thr_b;                     /* 0x26140 */
    volatile u_isp_wdr_fusion_thr_g isp_wdr_fusion_thr_g;                     /* 0x26144 */
    volatile u_isp_wdr_fusion_sat_thd isp_wdr_fusion_sat_thd;                 /* 0x26148 */
    volatile unsigned int reserved_123[368];                                  /* 0x2614c~0x26708 */
    volatile u_isp_dehaze_version isp_dehaze_version;                         /* 0x2670c */
    volatile u_isp_dehaze_blk_size isp_dehaze_blk_size;                       /* 0x26710 */
    volatile u_isp_dehaze_blk_sum isp_dehaze_blk_sum;                         /* 0x26714 */
    volatile u_isp_dehaze_dc_size isp_dehaze_dc_size;                         /* 0x26718 */
    volatile u_isp_dehaze_x isp_dehaze_x;                                     /* 0x2671c */
    volatile u_isp_dehaze_y isp_dehaze_y;                                     /* 0x26720 */
    volatile u_isp_dehaze_air isp_dehaze_air;                                 /* 0x26724 */
    volatile u_isp_dehaze_thld isp_dehaze_thld;                               /* 0x26728 */
    volatile u_isp_dehaze_gstrth isp_dehaze_gstrth;                           /* 0x2672c */
    volatile u_isp_dehaze_blthld isp_dehaze_blthld;                           /* 0x26730 */
    volatile u_isp_dehaze_stt_bst isp_dehaze_stt_bst;                         /* 0x26734 */
    volatile u_isp_dehaze_stt_abn isp_dehaze_stt_abn;                         /* 0x26738 */
    volatile u_isp_dehaze_stt2lut_cfg isp_dehaze_stt2lut_cfg;                 /* 0x2673c */
    volatile u_isp_dehaze_stt2lut_regnew isp_dehaze_stt2lut_regnew;           /* 0x26740 */
    volatile u_isp_dehaze_stt2lut_abn isp_dehaze_stt2lut_abn;                 /* 0x26744 */
    volatile u_isp_dehaze_smlmapoffset isp_dehaze_smlmapoffset;               /* 0x26748 */
    volatile u_isp_dehaze_stat_point isp_dehaze_stat_point;                   /* 0x2674c */
    volatile u_isp_dehaze_stat_num isp_dehaze_stat_num;                       /* 0x26750 */
    volatile unsigned int reserved_124[11];                                   /* 0x26754~0x2677c */
    volatile u_isp_dehaze_minstat_waddr isp_dehaze_minstat_waddr;             /* 0x26780 */
    volatile u_isp_dehaze_minstat_wdata isp_dehaze_minstat_wdata;             /* 0x26784 */
    volatile u_isp_dehaze_minstat_raddr isp_dehaze_minstat_raddr;             /* 0x26788 */
    volatile u_isp_dehaze_minstat_rdata isp_dehaze_minstat_rdata;             /* 0x2678c */
    volatile u_isp_dehaze_maxstat_waddr isp_dehaze_maxstat_waddr;             /* 0x26790 */
    volatile u_isp_dehaze_maxstat_wdata isp_dehaze_maxstat_wdata;             /* 0x26794 */
    volatile u_isp_dehaze_maxstat_raddr isp_dehaze_maxstat_raddr;             /* 0x26798 */
    volatile u_isp_dehaze_maxstat_rdata isp_dehaze_maxstat_rdata;             /* 0x2679c */
    volatile u_isp_dehaze_prestat_waddr isp_dehaze_prestat_waddr;             /* 0x267a0 */
    volatile u_isp_dehaze_prestat_wdata isp_dehaze_prestat_wdata;             /* 0x267a4 */
    volatile u_isp_dehaze_prestat_raddr isp_dehaze_prestat_raddr;             /* 0x267a8 */
    volatile u_isp_dehaze_prestat_rdata isp_dehaze_prestat_rdata;             /* 0x267ac */
    volatile u_isp_dehaze_lut_waddr isp_dehaze_lut_waddr;                     /* 0x267b0 */
    volatile u_isp_dehaze_lut_wdata isp_dehaze_lut_wdata;                     /* 0x267b4 */
    volatile u_isp_dehaze_lut_raddr isp_dehaze_lut_raddr;                     /* 0x267b8 */
    volatile u_isp_dehaze_lut_rdata isp_dehaze_lut_rdata;                     /* 0x267bc */
    volatile unsigned int reserved_125[19];                                   /* 0x267c0~0x26808 */
    volatile u_isp_expander_version isp_expander_version;                     /* 0x2680c */
    volatile u_isp_expander_bitw isp_expander_bitw;                           /* 0x26810 */
    volatile unsigned int reserved_126[27];                                   /* 0x26814~0x2687c */
    volatile u_isp_expander_lut_waddr isp_expander_lut_waddr;                 /* 0x26880 */
    volatile u_isp_expander_lut_wdata isp_expander_lut_wdata;                 /* 0x26884 */
    volatile u_isp_expander_lut_raddr isp_expander_lut_raddr;                 /* 0x26888 */
    volatile u_isp_expander_lut_rdata isp_expander_lut_rdata;                 /* 0x2688c */
    volatile unsigned int reserved_127[92];                                   /* 0x26890~0x269fc */
    volatile u_isp_gamma_cfg isp_gamma_cfg;                                   /* 0x26a00 */
    volatile unsigned int reserved_128[7];                                    /* 0x26a04~0x26a1c */
    volatile u_isp_gamma_pos0 isp_gamma_pos0;                                 /* 0x26a20 */
    volatile u_isp_gamma_pos1 isp_gamma_pos1;                                 /* 0x26a24 */
    volatile u_isp_gamma_pos2 isp_gamma_pos2;                                 /* 0x26a28 */
    volatile u_isp_gamma_pos3 isp_gamma_pos3;                                 /* 0x26a2c */
    volatile u_isp_gamma_inseg0 isp_gamma_inseg0;                             /* 0x26a30 */
    volatile u_isp_gamma_inseg1 isp_gamma_inseg1;                             /* 0x26a34 */
    volatile u_isp_gamma_inseg2 isp_gamma_inseg2;                             /* 0x26a38 */
    volatile u_isp_gamma_inseg3 isp_gamma_inseg3;                             /* 0x26a3c */
    volatile u_isp_gamma_step isp_gamma_step;                                 /* 0x26a40 */
    volatile unsigned int reserved_129[3];                                    /* 0x26a44~0x26a4c */
    volatile u_isp_gamma_stt2lut_cfg isp_gamma_stt2lut_cfg;                   /* 0x26a50 */
    volatile u_isp_gamma_stt2lut_regnew isp_gamma_stt2lut_regnew;             /* 0x26a54 */
    volatile u_isp_gamma_stt2lut_abn isp_gamma_stt2lut_abn;                   /* 0x26a58 */
    volatile unsigned int reserved_130_1[9];                                    /* 0x26a5c~0x26a7c */
    volatile u_isp_gamma_lut_waddr isp_gamma_lut_waddr;                       /* 0x26a80 */
    volatile u_isp_gamma_lut_wdata isp_gamma_lut_wdata;                       /* 0x26a84 */
    volatile u_isp_gamma_lut_raddr isp_gamma_lut_raddr;                       /* 0x26a88 */
    volatile u_isp_gamma_lut_rdata isp_gamma_lut_rdata;                       /* 0x26a8c */
    volatile unsigned int reserved_130_2[92];                                   /* 0x26a90~0x26bfc */
    volatile u_isp_bcom_version isp_bcom_version;                             /* 0x26c00 */
    volatile u_isp_bcom_lut_step0 isp_bcom_lut_step0;                         /* 0x26c04 */
    volatile u_isp_bcom_lut_step1 isp_bcom_lut_step1;                         /* 0x26c08 */
    volatile u_isp_bcom_lut_inseg0 isp_bcom_lut_inseg0;                       /* 0x26c0c */
    volatile u_isp_bcom_lut_inseg1 isp_bcom_lut_inseg1;                       /* 0x26c10 */
    volatile u_isp_bcom_lut_inseg2 isp_bcom_lut_inseg2;                       /* 0x26c14 */
    volatile u_isp_bcom_lut_pos0 isp_bcom_lut_pos0;                           /* 0x26c18 */
    volatile u_isp_bcom_lut_pos1 isp_bcom_lut_pos1;                           /* 0x26c1c */
    volatile u_isp_bcom_lut_pos2 isp_bcom_lut_pos2;                           /* 0x26c20 */
    volatile u_isp_bcom_lut_pos3 isp_bcom_lut_pos3;                           /* 0x26c24 */
    volatile u_isp_bcom_lut_pos4 isp_bcom_lut_pos4;                           /* 0x26c28 */
    volatile u_isp_bcom_lut_pos5 isp_bcom_lut_pos5;                           /* 0x26c2c */
    volatile u_isp_bcom_lut_pos6 isp_bcom_lut_pos6;                           /* 0x26c30 */
    volatile u_isp_bcom_lut_pos7 isp_bcom_lut_pos7;                           /* 0x26c34 */
    volatile unsigned int reserved_132[18];                                   /* 0x26c38~0x26c7c */
    volatile u_isp_bcom_gamma_lut_waddr isp_bcom_gamma_lut_waddr;             /* 0x26c80 */
    volatile u_isp_bcom_gamma_lut_wdata isp_bcom_gamma_lut_wdata;             /* 0x26c84 */
    volatile u_isp_bcom_gamma_lut_raddr isp_bcom_gamma_lut_raddr;             /* 0x26c88 */
    volatile u_isp_bcom_gamma_lut_rdata isp_bcom_gamma_lut_rdata;             /* 0x26c8c */
    volatile unsigned int reserved_133[28];                                   /* 0x26c90~0x26cfc */
    volatile u_isp_bdec_version isp_bdec_version;                             /* 0x26d00 */
    volatile u_isp_bdec_lut_step0 isp_bdec_lut_step0;                         /* 0x26d04 */
    volatile u_isp_bdec_lut_step1 isp_bdec_lut_step1;                         /* 0x26d08 */
    volatile u_isp_bdec_lut_inseg0 isp_bdec_lut_inseg0;                       /* 0x26d0c */
    volatile u_isp_bdec_lut_inseg1 isp_bdec_lut_inseg1;                       /* 0x26d10 */
    volatile u_isp_bdec_lut_inseg2 isp_bdec_lut_inseg2;                       /* 0x26d14 */
    volatile u_isp_bdec_lut_pos0 isp_bdec_lut_pos0;                           /* 0x26d18 */
    volatile u_isp_bdec_lut_pos1 isp_bdec_lut_pos1;                           /* 0x26d1c */
    volatile u_isp_bdec_lut_pos2 isp_bdec_lut_pos2;                           /* 0x26d20 */
    volatile u_isp_bdec_lut_pos3 isp_bdec_lut_pos3;                           /* 0x26d24 */
    volatile unsigned int reserved_134[22];                                   /* 0x26d28~0x26d7c */
    volatile u_isp_bdec_gamma_lut_waddr isp_bdec_gamma_lut_waddr;             /* 0x26d80 */
    volatile u_isp_bdec_gamma_lut_wdata isp_bdec_gamma_lut_wdata;             /* 0x26d84 */
    volatile u_isp_bdec_gamma_lut_raddr isp_bdec_gamma_lut_raddr;             /* 0x26d88 */
    volatile u_isp_bdec_gamma_lut_rdata isp_bdec_gamma_lut_rdata;             /* 0x26d8c */
    volatile unsigned int reserved_135[160];                                  /* 0x26d90~0x2700c */
    volatile u_isp_ca_ctrl isp_ca_ctrl;                                       /* 0x27010 */
    volatile u_isp_ca_des isp_ca_des;                                         /* 0x27014 */
    volatile u_isp_ca_isoratio isp_ca_isoratio;                               /* 0x27018 */
    volatile u_isp_ca_sat_coef isp_ca_sat_coef;                               /* 0x2701c */
    volatile unsigned int reserved_136[24];                                   /* 0x27020~0x2707c */
    volatile u_isp_ca_yratiolut_waddr isp_ca_yratiolut_waddr;                 /* 0x27080 */
    volatile u_isp_ca_yratiolut_wdata isp_ca_yratiolut_wdata;                 /* 0x27084 */
    volatile u_isp_ca_yratiolut_raddr isp_ca_yratiolut_raddr;                 /* 0x27088 */
    volatile u_isp_ca_yratiolut_rdata isp_ca_yratiolut_rdata;                 /* 0x2708c */
    volatile u_isp_ca_ysatlut_waddr isp_ca_ysatlut_waddr;                     /* 0x27090 */
    volatile u_isp_ca_ysatlut_wdata isp_ca_ysatlut_wdata;                     /* 0x27094 */
    volatile u_isp_ca_ysatlut_raddr isp_ca_ysatlut_raddr;                     /* 0x27098 */
    volatile u_isp_ca_ysatlut_rdata isp_ca_ysatlut_rdata;                     /* 0x2709c */
    volatile u_isp_ca_ych1lut_waddr isp_ca_ych1lut_waddr;                     /* 0x270a0 */
    volatile u_isp_ca_ych1lut_wdata isp_ca_ych1lut_wdata;                     /* 0x270a4 */
    volatile u_isp_ca_ych1lut_raddr isp_ca_ych1lut_raddr;                     /* 0x270a8 */
    volatile u_isp_ca_ych1lut_rdata isp_ca_ych1lut_rdata;                     /* 0x270ac */
    volatile u_isp_ca_ych2lut_waddr isp_ca_ych2lut_waddr;                     /* 0x270b0 */
    volatile u_isp_ca_ych2lut_wdata isp_ca_ych2lut_wdata;                     /* 0x270b4 */
    volatile u_isp_ca_ych2lut_raddr isp_ca_ych2lut_raddr;                     /* 0x270b8 */
    volatile u_isp_ca_ych2lut_rdata isp_ca_ych2lut_rdata;                     /* 0x270bc */
    volatile u_isp_ca_yslulut_waddr isp_ca_yslulut_waddr;                     /* 0x270c0 */
    volatile u_isp_ca_yslulut_wdata isp_ca_yslulut_wdata;                     /* 0x270c4 */
    volatile u_isp_ca_yslulut_raddr isp_ca_yslulut_raddr;                     /* 0x270c8 */
    volatile u_isp_ca_yslulut_rdata isp_ca_yslulut_rdata;                     /* 0x270cc */
    volatile u_isp_ca_yslvlut_waddr isp_ca_yslvlut_waddr;                     /* 0x270d0 */
    volatile u_isp_ca_yslvlut_wdata isp_ca_yslvlut_wdata;                     /* 0x270d4 */
    volatile u_isp_ca_yslvlut_raddr isp_ca_yslvlut_raddr;                     /* 0x270d8 */
    volatile u_isp_ca_yslvlut_rdata isp_ca_yslvlut_rdata;                     /* 0x270dc */
    volatile unsigned int reserved_137[12];                                   /* 0x270e0~0x2710c */
    volatile u_isp_ca_stt2lut_cfg isp_ca_stt2lut_cfg;                         /* 0x27110 */
    volatile u_isp_ca_stt2lut_regnew isp_ca_stt2lut_regnew;                   /* 0x27114 */
    volatile u_isp_ca_stt2lut_abn isp_ca_stt2lut_abn;                         /* 0x27118 */
    volatile unsigned int reserved_138[441];                                  /* 0x2711c~0x277fc */
    volatile u_isp_ldci_cfg isp_ldci_cfg;                                     /* 0x27800 */
    volatile unsigned int reserved_139[2];                                    /* 0x27804~0x27808 */
    volatile u_isp_ldci_version isp_ldci_version;                             /* 0x2780c */
    volatile u_isp_ldci_stat_offset isp_ldci_stat_offset;                     /* 0x27810 */
    volatile u_isp_ldci_calc_small_offset isp_ldci_calc_small_offset;         /* 0x27814 */
    volatile u_isp_ldci_zone isp_ldci_zone;                                   /* 0x27818 */
    volatile unsigned int reserved_141;                                       /* 0x2781c */
    volatile u_isp_ldci_lpfstt_bst isp_ldci_lpfstt_bst;                       /* 0x27820 */
    volatile u_isp_ldci_lpfstt_abn isp_ldci_lpfstt_abn;                       /* 0x27824 */
    volatile u_isp_ldci_scale isp_ldci_scale;                                 /* 0x27828 */
    volatile unsigned int reserved_142[2];                                    /* 0x2782c~0x27830 */
    volatile u_isp_ldci_luma_sel isp_ldci_luma_sel;                           /* 0x27834 */
    volatile u_isp_ldci_blc_ctrl isp_ldci_blc_ctrl;                           /* 0x27838 */
    volatile unsigned int reserved_143[10];                                   /* 0x2783c~0x27860 */
    volatile u_isp_ldci_lpf_lpfcoef0 isp_ldci_lpf_lpfcoef0;                   /* 0x27864 */
    volatile u_isp_ldci_lpf_lpfcoef1 isp_ldci_lpf_lpfcoef1;                   /* 0x27868 */
    volatile u_isp_ldci_lpf_lpfcoef2 isp_ldci_lpf_lpfcoef2;                   /* 0x2786c */
    volatile unsigned int reserved_144[2];                                    /* 0x27870~0x27874 */
    volatile u_isp_ldci_lpf_lpfsft isp_ldci_lpf_lpfsft;                       /* 0x27878 */
    volatile unsigned int reserved_145[5];                                    /* 0x2787c~0x2788c */
    volatile u_isp_ldci_he_waddr isp_ldci_he_waddr;                           /* 0x27890 */
    volatile u_isp_ldci_he_wdata isp_ldci_he_wdata;                           /* 0x27894 */
    volatile u_isp_ldci_he_raddr isp_ldci_he_raddr;                           /* 0x27898 */
    volatile u_isp_ldci_he_rdata isp_ldci_he_rdata;                           /* 0x2789c */
    volatile unsigned int reserved_146[4];                                    /* 0x278a0~0x278ac */
    volatile u_isp_ldci_cgain_waddr isp_ldci_cgain_waddr;                     /* 0x278b0 */
    volatile u_isp_ldci_cgain_wdata isp_ldci_cgain_wdata;                     /* 0x278b4 */
    volatile u_isp_ldci_cgain_raddr isp_ldci_cgain_raddr;                     /* 0x278b8 */
    volatile u_isp_ldci_cgain_rdata isp_ldci_cgain_rdata;                     /* 0x278bc */
    volatile unsigned int reserved_147[24];                                   /* 0x278c0~0x2791c */
    volatile u_isp_ldci_stat_hpos isp_ldci_stat_hpos;                         /* 0x27920 */
    volatile u_isp_ldci_stat_vpos isp_ldci_stat_vpos;                         /* 0x27924 */
    volatile u_isp_ldci_stat_evratio isp_ldci_stat_evratio;                   /* 0x27928 */
    volatile unsigned int reserved_148;                                       /* 0x2792c */
    volatile u_isp_ldci_stat_zone isp_ldci_stat_zone;                         /* 0x27930 */
    volatile unsigned int reserved_149[7];                                    /* 0x27934~0x2794c */
    volatile u_isp_ldci_blk_smlmapwidth0 isp_ldci_blk_smlmapwidth0;           /* 0x27950 */
    volatile u_isp_ldci_blk_smlmapwidth1 isp_ldci_blk_smlmapwidth1;           /* 0x27954 */
    volatile u_isp_ldci_blk_smlmapwidth2 isp_ldci_blk_smlmapwidth2;           /* 0x27958 */
    volatile unsigned int reserved_150[3];                                    /* 0x2795c~0x27964 */
    volatile u_isp_ldci_chrdamp isp_ldci_chrdamp;                             /* 0x27968 */
    volatile unsigned int reserved_151;                                       /* 0x2796c */
    volatile u_isp_ldci_stt2lut_cfg isp_ldci_stt2lut_cfg;                     /* 0x27970 */
    volatile u_isp_ldci_stt2lut_regnew isp_ldci_stt2lut_regnew;               /* 0x27974 */
    volatile u_isp_ldci_stt2lut_abn isp_ldci_stt2lut_abn;                     /* 0x27978 */
    volatile unsigned int reserved_152[13];                                   /* 0x2797c~0x279ac */
    volatile u_isp_ldci_lpf_map_waddr isp_ldci_lpf_map_waddr;                 /* 0x279b0 */
    volatile u_isp_ldci_lpf_map_wdata isp_ldci_lpf_map_wdata;                 /* 0x279b4 */
    volatile u_isp_ldci_lpf_map_raddr isp_ldci_lpf_map_raddr;                 /* 0x279b8 */
    volatile u_isp_ldci_lpf_map_rdata isp_ldci_lpf_map_rdata;                 /* 0x279bc */
    volatile unsigned int reserved_153[144];                                  /* 0x279c0~0x27bfc */
    volatile u_isp_drc_cfg isp_drc_cfg;                                       /* 0x27c00 */
    volatile u_isp_drc_lut_update0 isp_drc_lut_update0;                       /* 0x27c04 */
    volatile u_isp_drc_lut_update1 isp_drc_lut_update1;                         /* 0x27c08 */
    volatile u_isp_drc_version isp_drc_version;                                 /* 0x27c0c */
    volatile u_isp_drc_zone isp_drc_zone;                                       /* 0x27c10 */
    volatile u_isp_drc_zone_size isp_drc_zone_size;                             /* 0x27c14 */
    volatile u_isp_drc_zone_div0 isp_drc_zone_div0;                             /* 0x27c18 */
    volatile u_isp_drc_zone_div1 isp_drc_zone_div1;                             /* 0x27c1c */
    volatile u_isp_drc_bin isp_drc_bin;                                         /* 0x27c20 */
    volatile u_isp_drc_zone_init isp_drc_zone_init;                             /* 0x27c24 */
    volatile u_isp_drc_bin_scale isp_drc_bin_scale;                             /* 0x27c28 */
    volatile u_isp_drc_gain_cfg isp_drc_gain_cfg;                               /* 0x27c2c */
    volatile unsigned int reserved_231[2];                                      /* 0x27c30~0x27c34 */
    volatile u_isp_drc_strength isp_drc_strength;                               /* 0x27c38 */
    volatile u_isp_drc_stat_width isp_drc_stat_width;                           /* 0x27c3c */
    volatile u_isp_drc_dark_gain_lmt_y isp_drc_dark_gain_lmt_y;                 /* 0x27c40 */
    volatile u_isp_drc_dark_gain_lmt_c isp_drc_dark_gain_lmt_c;                 /* 0x27c44 */
    volatile u_isp_drc_bright_gain_lmt isp_drc_bright_gain_lmt;                 /* 0x27c48 */
    volatile u_isp_drc_rgb_wgt isp_drc_rgb_wgt;                                 /* 0x27c4c */
    volatile u_isp_drc_dp_dtc isp_drc_dp_dtc;                                   /* 0x27c50 */
    volatile unsigned int reserved_232;                                         /* 0x27c54 */
    volatile u_isp_drc_gain_clip isp_drc_gain_clip;                             /* 0x27c58 */
    volatile u_isp_drc_color_ctrl isp_drc_color_ctrl;                           /* 0x27c5c */
    volatile u_isp_drc_global_corr isp_drc_global_corr;                         /* 0x27c60 */
    volatile u_isp_drc_mixing_coring isp_drc_mixing_coring;                     /* 0x27c64 */
    volatile u_isp_drc_mixing_dark isp_drc_mixing_dark;                         /* 0x27c68 */
    volatile u_isp_drc_mixing_bright isp_drc_mixing_bright;                     /* 0x27c6c */
    volatile unsigned int reserved_233[4];                                      /* 0x27c70~0x27c7c */
    volatile u_isp_drc_tmlut0_waddr isp_drc_tmlut0_waddr;                       /* 0x27c80 */
    volatile u_isp_drc_tmlut0_wdata isp_drc_tmlut0_wdata;                       /* 0x27c84 */
    volatile u_isp_drc_tmlut0_raddr isp_drc_tmlut0_raddr;                       /* 0x27c88 */
    volatile u_isp_drc_tmlut0_rdata isp_drc_tmlut0_rdata;                       /* 0x27c8c */
    volatile unsigned int reserved_234[4];                                      /* 0x27c90~0x27c9c */
    volatile u_isp_drc_cclut_waddr isp_drc_cclut_waddr;                         /* 0x27ca0 */
    volatile u_isp_drc_cclut_wdata isp_drc_cclut_wdata;                         /* 0x27ca4 */
    volatile u_isp_drc_cclut_raddr isp_drc_cclut_raddr;                         /* 0x27ca8 */
    volatile u_isp_drc_cclut_rdata isp_drc_cclut_rdata;                         /* 0x27cac */
    volatile u_isp_drc_coefalut_waddr isp_drc_coefalut_waddr;                   /* 0x27cb0 */
    volatile u_isp_drc_coefalut_wdata isp_drc_coefalut_wdata;                   /* 0x27cb4 */
    volatile u_isp_drc_coefalut_raddr isp_drc_coefalut_raddr;                   /* 0x27cb8 */
    volatile u_isp_drc_coefalut_rdata isp_drc_coefalut_rdata;                   /* 0x27cbc */
    volatile u_isp_drc_delut_waddr isp_drc_delut_waddr;                         /* 0x27cc0 */
    volatile u_isp_drc_delut_wdata isp_drc_delut_wdata;                         /* 0x27cc4 */
    volatile u_isp_drc_delut_raddr isp_drc_delut_raddr;                         /* 0x27cc8 */
    volatile u_isp_drc_delut_rdata isp_drc_delut_rdata;                         /* 0x27ccc */
    volatile unsigned int reserved_235[12];                                     /* 0x27cd0~0x27cfc */
    volatile u_isp_drc_grad_rev isp_drc_grad_rev;                               /* 0x27d00 */
    volatile u_isp_drc_vbi_strength isp_drc_vbi_strength;                       /* 0x27d04 */
    volatile u_isp_drc_rng_ada_coef isp_drc_rng_ada_coef;                       /* 0x27d08 */
    volatile unsigned int reserved_236;                                         /* 0x27d0c */
    volatile u_isp_drc_vbi_state isp_drc_vbi_state;                             /* 0x27d10 */
    volatile unsigned int reserved_237[10];                                     /* 0x27d14~0x27d38 */
    volatile u_isp_drc_prev_luma_0 isp_drc_prev_luma_0;                         /* 0x27d3c */
    volatile u_isp_drc_prev_luma_1 isp_drc_prev_luma_1;                         /* 0x27d40 */
    volatile u_isp_drc_prev_luma_2 isp_drc_prev_luma_2;                         /* 0x27d44 */
    volatile u_isp_drc_prev_luma_3 isp_drc_prev_luma_3;                         /* 0x27d48 */
    volatile u_isp_drc_prev_luma_4 isp_drc_prev_luma_4;                         /* 0x27d4c */
    volatile u_isp_drc_prev_luma_5 isp_drc_prev_luma_5;                         /* 0x27d50 */
    volatile u_isp_drc_prev_luma_6 isp_drc_prev_luma_6;                         /* 0x27d54 */
    volatile u_isp_drc_prev_luma_7 isp_drc_prev_luma_7;                         /* 0x27d58 */
    volatile unsigned int reserved_238;                                         /* 0x27d5c */
    volatile u_isp_drc_suppress_bright isp_drc_suppress_bright;                 /* 0x27d60 */
    volatile u_isp_drc_suppress_dark isp_drc_suppress_dark;                     /* 0x27d64 */
    volatile u_isp_drc_idxbase0 isp_drc_idxbase0;                               /* 0x27d68 */
    volatile u_isp_drc_idxbase1 isp_drc_idxbase1;                               /* 0x27d6c */
    volatile u_isp_drc_maxval0 isp_drc_maxval0;                                 /* 0x27d70 */
    volatile u_isp_drc_maxval1 isp_drc_maxval1;                                 /* 0x27d74 */
    volatile unsigned int reserved_239[34];                                     /* 0x27d78~0x27dfc */
    volatile u_isp_drc_pregamma_idxbase0 isp_drc_pregamma_idxbase0;             /* 0x27e00 */
    volatile u_isp_drc_pregamma_idxbase1 isp_drc_pregamma_idxbase1;             /* 0x27e04 */
    volatile u_isp_drc_pregamma_idxbase2 isp_drc_pregamma_idxbase2;             /* 0x27e08 */
    volatile u_isp_drc_pregamma_maxval0 isp_drc_pregamma_maxval0;               /* 0x27e0c */
    volatile u_isp_drc_pregamma_maxval1 isp_drc_pregamma_maxval1;               /* 0x27e10 */
    volatile u_isp_drc_pregamma_maxval2 isp_drc_pregamma_maxval2;               /* 0x27e14 */
    volatile u_isp_drc_pregamma_maxval3 isp_drc_pregamma_maxval3;               /* 0x27e18 */
    volatile u_isp_drc_pregamma_maxval4 isp_drc_pregamma_maxval4;               /* 0x27e1c */
    volatile u_isp_drc_pregamma_maxval5 isp_drc_pregamma_maxval5;               /* 0x27e20 */
    volatile unsigned int reserved_240[11];                                     /* 0x27e24~0x27e4c */
    volatile u_isp_drc_flt_cfg isp_drc_flt_cfg;                                 /* 0x27e50 */
    volatile u_isp_drc_bin_factor_coarse_0 isp_drc_bin_factor_coarse_0;         /* 0x27e54 */
    volatile u_isp_drc_bin_factor_coarse_1 isp_drc_bin_factor_coarse_1;         /* 0x27e58 */
    volatile u_isp_drc_bin_factor_medium_0 isp_drc_bin_factor_medium_0;         /* 0x27e5c */
    volatile u_isp_drc_bin_factor_medium_1 isp_drc_bin_factor_medium_1;         /* 0x27e60 */
    volatile u_isp_drc_detail_sub_factor isp_drc_detail_sub_factor;             /* 0x27e64 */
    volatile u_isp_drc_wgt_box_tri_sel isp_drc_wgt_box_tri_sel;                 /* 0x27e68 */
    volatile u_isp_drc_blk_wgt_init isp_drc_blk_wgt_init;                       /* 0x27e6c */
    volatile u_isp_drc_shp_cfg isp_drc_shp_cfg;                                 /* 0x27e70 */
    volatile u_isp_drc_div_denom_log isp_drc_div_denom_log;                     /* 0x27e74 */
    volatile u_isp_drc_denom_exp isp_drc_denom_exp;                             /* 0x27e78 */
    volatile u_isp_drc_debug_info isp_drc_debug_info;                           /* 0x27e7c */
    volatile unsigned int reserved_164[32];                                     /* 0x0x27e80 ~ 0x27efc */

    isp_be_lut_type                          be_lut; /* size 5e00 */
}  isp_be_reg_type;


typedef isp_be_reg_type isp_pre_be_reg_type;
typedef isp_be_reg_type isp_post_be_reg_type;

typedef struct {
    isp_be_reg_type      be_reg;       /* 0x0 ~ 0xbbfc, size 0xbc00 = (0x7e00 + 0x3e00) */
    isp_viproc_reg_type  viproc_reg; /* size :0x800 */
    // all size: 0xda00 + 0x800 = 0xe200
} isp_be_all_reg_type;

#endif /* ISP_REG_DEFINE_H */
