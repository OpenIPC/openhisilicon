/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_STT_DEFINE_H
#define ISP_STT_DEFINE_H

/* Define the union u_isp_ae_total_stat_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_total_pixels_stt : 27; /* [26..0]  */
        unsigned int    reserved_0            : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_stat_rstt;

/* Define the union u_isp_ae_count_stat_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_count_pixels_stt : 31; /* [30..0]  */
        unsigned int    reserved_0            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_count_stat_rstt;

/* Define the union u_isp_ae_total_r_aver_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_r_aver_stt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_r_aver_rstt;
/* Define the union u_isp_ae_total_gr_aver_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gr_aver_stt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_gr_aver_rstt;
/* Define the union u_isp_ae_total_gb_aver_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gb_aver_stt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_gb_aver_rstt;
/* Define the union u_isp_ae_total_b_aver_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_b_aver_stt : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_total_b_aver_rstt;
/* Define the union u_isp_ae_hist_high_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_high_stt   : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist_high_rstt;
/* Define the union u_isp_ae_hist */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_hist           : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_hist;

/* Define the union u_isp_ae_aver_r_gr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_aver_gr        : 16; /* [15..0]  */
        unsigned int    isp_ae_aver_r         : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_r_gr;

/* Define the union u_isp_ae_aver_gb_b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_aver_b         : 16; /* [15..0]  */
        unsigned int    isp_ae_aver_gb        : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ae_aver_gb_b;

/* Define the union u_isp_awb_avg_r_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_r_stt     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_avg_r_rstt;

/* Define the union u_isp_awb_avg_g_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_g_stt     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_avg_g_rstt;

/* Define the union u_isp_awb_avg_b_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_b_stt     : 16; /* [15..0]  */
        unsigned int    reserved_0            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_avg_b_rstt;

/* Define the union u_isp_awb_cnt_all_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_count_all_stt : 16; /* [15..0]  */
        unsigned int    reserved_0            : 14  ; /* [29..16]  */
        unsigned int    isp_awb_debug_level_status_stt : 1; /* [30]  */
        unsigned int    isp_awb_debug_level_en_stt : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_cnt_all_rstt;

/* Define the union u_isp_awb_stat */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat           : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_stat;
/* Define the union u_isp_af_stat */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_out        : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_af_stat;
/* Define the union u_isp_la_aver */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_aver_b         : 8; /* [7..0]  */
        unsigned int    isp_la_aver_gb        : 8; /* [15..8]  */
        unsigned int    isp_la_aver_gr        : 8; /* [23..16]  */
        unsigned int    isp_la_aver_r         : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_la_aver;

/* Define the union u_isp_flick_gr_diff_cur_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_diff_cur_rstt : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gr_diff_cur_rstt;

/* Define the union u_isp_flick_gb_diff_cur_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_diff_cur_rstt : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gb_diff_cur_rstt;

/* Define the union u_isp_flick_gr_abs_cur_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_abs_cur_rstt : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gr_abs_cur_rstt;

/* Define the union u_isp_flick_gb_abs_cur_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_abs_cur_rstt : 23; /* [22..0]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gb_abs_cur_rstt;

/* Define the union u_isp_flick_gmean */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gbmean      : 11; /* [10..0]  */
        unsigned int    isp_flick_grmean      : 11; /* [21..11]  */
        unsigned int    reserved_0            : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_flick_gmean;

/* Define the union u_isp_fpn_stat_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_busy_rstt     : 1; /* [0]  */
        unsigned int    reserved_0            : 7; /* [7..1]  */
        unsigned int    isp_fpn_vcnt_rstt     : 6; /* [13..8]  */
        unsigned int    reserved_1            : 2; /* [15..14]  */
        unsigned int    isp_fpn_hcnt_rstt     : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_stat_rstt;

/* Define the union u_isp_fpn_sum0_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum0_rstt      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_sum0_rstt;
/* Define the union u_isp_fpn_sum1_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum1_rstt      : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_fpn_sum1_rstt;

/* Define the union u_isp_dpc_bpt_calib_number_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_calib_number_rstt : 12; /* [11..0]  */
        unsigned int    reserved_0            : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_calib_number_rstt;

/* Define the union u_isp_dpc_bpt_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_posx_rstt     : 13; /* [12..0]  */
        unsigned int    reserved_0            : 3; /* [15..13]  */
        unsigned int    isp_dpc_posy_rstt     : 13; /* [28..16]  */
        unsigned int    reserved_1            : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dpc_bpt_rstt;

/* Define the union u_isp_ldci_lpf_map_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpflitmap_rstt : 10; /* [9..0]  */
        unsigned int    isp_ldci_lpfplyp2map_rstt : 10; /* [19..10]  */
        unsigned int    isp_ldci_lpfplyp3map_rstt : 10; /* [29..20]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_ldci_lpf_map_rstt;

/* Define the union u_isp_dehaze_minstat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_l  : 10; /* [9..0]  */
        unsigned int    reserved_0            : 6; /* [15..10]  */
        unsigned int    isp_dehaze_minstat_h  : 10; /* [25..16]  */
        unsigned int    reserved_1            : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_minstat;

/* Define the union u_isp_dehaze_maxstat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat    : 30; /* [29..0]  */
        unsigned int    reserved_0            : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_dehaze_maxstat;

/* Define the union u_isp_y_sum0_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum0             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_y_sum0_rstt;
/* Define the union u_isp_y_sum1_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum1             : 32; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_y_sum1_rstt;

typedef struct {
    volatile u_isp_ae_total_stat_rstt        isp_ae_total_stat_rstt; /* 0x0 */
    volatile u_isp_ae_count_stat_rstt        isp_ae_count_stat_rstt; /* 0x4 */
    volatile u_isp_ae_total_r_aver_rstt      isp_ae_total_r_aver_rstt; /* 0x8 */
    volatile u_isp_ae_total_gr_aver_rstt     isp_ae_total_gr_aver_rstt; /* 0xc */
    volatile u_isp_ae_total_gb_aver_rstt     isp_ae_total_gb_aver_rstt; /* 0x10 */
    volatile u_isp_ae_total_b_aver_rstt      isp_ae_total_b_aver_rstt; /* 0x14 */
    volatile u_isp_ae_hist_high_rstt         isp_ae_hist_high_rstt; /* 0x18 */
    volatile u_isp_ae_hist                   isp_ae_hist[1024]; /* 0x1c~0x1018 */
    volatile u_isp_ae_aver_r_gr              isp_ae_aver_r_gr[256]; /* 0x101c~0x1418 */
    volatile u_isp_ae_aver_gb_b              isp_ae_aver_gb_b[256]; /* 0x141c~0x1818 */
} isp_be_offline_ae_stat_type;

typedef struct {
    isp_be_offline_ae_stat_type              isp_ae_atat; /* 0x0 ~ 0x1818 */
    volatile u_isp_awb_avg_r_rstt            isp_awb_avg_r_rstt; /* 0x181c */
    volatile u_isp_awb_avg_g_rstt            isp_awb_avg_g_rstt; /* 0x1820 */
    volatile u_isp_awb_avg_b_rstt            isp_awb_avg_b_rstt; /* 0x1824 */
    volatile u_isp_awb_cnt_all_rstt          isp_awb_cnt_all_rstt; /* 0x1828 */
    volatile u_isp_awb_stat                  isp_awb_stat[2048]; /* 0x182c~0x3828 */
    volatile u_isp_af_stat                   isp_af_stat[1020]; /* 0x382c~0x4818 */
    volatile u_isp_la_aver                   isp_la_aver[256]; /* 0x481c~0x4c18 */
    volatile u_isp_flick_gr_diff_cur_rstt    isp_flick_gr_diff_cur_rstt; /* 0x4c1c */
    volatile u_isp_flick_gb_diff_cur_rstt    isp_flick_gb_diff_cur_rstt; /* 0x4c20 */
    volatile u_isp_flick_gr_abs_cur_rstt     isp_flick_gr_abs_cur_rstt; /* 0x4c24 */
    volatile u_isp_flick_gb_abs_cur_rstt     isp_flick_gb_abs_cur_rstt; /* 0x4c28 */
    volatile u_isp_flick_gmean               isp_flick_gmean[512]; /* 0x4c2c~0x5428 */
    volatile u_isp_fpn_stat_rstt             isp_fpn_stat_rstt; /* 0x542c */
    volatile u_isp_fpn_sum0_rstt             isp_fpn_sum0_rstt; /* 0x5430 */
    volatile u_isp_fpn_sum1_rstt             isp_fpn_sum1_rstt; /* 0x5434 */
    volatile u_isp_dpc_bpt_calib_number_rstt isp_dpc_bpt_calib_number_rstt; /* 0x5438 */
    volatile u_isp_dpc_bpt_rstt              isp_dpc_bpt_rstt[2048]; /* 0x543c~0x7438 */
    volatile u_isp_ldci_lpf_map_rstt         isp_ldci_lpf_map_rstt[384]; /* 0x743c~0x7a38 */
    volatile u_isp_dehaze_minstat            isp_dehaze_minstat[192]; /* 0x7a3c~0x7d38 */
    volatile u_isp_dehaze_maxstat            isp_dehaze_maxstat[384]; /* 0x7d3c~0x8338 */
    volatile u_isp_y_sum0_rstt               isp_y_sum0_rstt; /* 0x833c */
    volatile u_isp_y_sum1_rstt               isp_y_sum1_rstt; /* 0x8340 */
    volatile unsigned int                    reserved_3[47];  /* 0x8344 ~ 0x83fc, reserved 47 * 4 bytes */
} isp_be_offline_stat;

/* Define the global struct */
typedef struct {
    isp_be_offline_stat  be_stat;   /* align 256 */
} isp_be_offline_stat_reg_type;

/* Define the global struct */
typedef struct {
    isp_be_offline_ae_stat_type          isp_ae_atat;             /* 0x0 ~ 0x1818 */
    volatile u_isp_awb_avg_r_rstt        isp_awb_avg_r_rstt;      /* 0x181c */
    volatile u_isp_awb_avg_g_rstt        isp_awb_avg_g_rstt;      /* 0x1820 */
    volatile u_isp_awb_avg_b_rstt        isp_awb_avg_b_rstt;      /* 0x1824 */
    volatile u_isp_awb_cnt_all_rstt      isp_awb_cnt_all_rstt;    /* 0x1828 */
    volatile u_isp_awb_stat              isp_awb_stat[2048];      /* 0x182c~0x3828, awb stat lut size: 2048 */
} isp_stitch_stat_reg_type;

typedef struct {
    volatile u_isp_ae_hist           isp_ae_hist[1024];           /* 0x0~0xffc, ae hist size: 1024 */
    volatile u_isp_ae_aver_r_gr      isp_ae_aver_r_gr[256];       /* 0x1000~0x13fc, ae aver size: 256  */
    volatile u_isp_ae_aver_gb_b      isp_ae_aver_gb_b[256];       /* 0x1400~0x17fc, ae aver size: 256  */
    volatile u_isp_af_stat           isp_af_stat[1020];           /* 0x1800 ~ 0x27ec, af stat size:1020 */
    volatile unsigned int            reserved_1[4];               /* 0x27ef0 ~ 0x27fc, reserved 4 * 4 bytes */
    volatile u_isp_awb_stat          isp_awb_stat[2048];          /* 0x2800~0x47fc, awb stat lut size: 2048  */
    volatile u_isp_la_aver           isp_la_aver[256];            /* 0x4800~0x4bfc, la aver lut size: 256  */
    volatile u_isp_dehaze_minstat    isp_dehaze_minstat[192];     /* 0x4c00~0x4efc, dehaze min stat size: 192 */
    volatile u_isp_dehaze_maxstat    isp_dehaze_maxstat[384];    /* 0x4f00~0x54fc, dehaze max stat size: 384 */
    volatile u_isp_ldci_lpf_map_rstt isp_ldci_lpf_map_rstt[384]; /* 0x5500~0x5afc, ldci lpf map size:384  */
    volatile u_isp_flick_gmean       isp_flick_gmean[512];       /* 0x5b00~0x62fc, flicker gmean lut size: 512 */
} isp_be_online_stat_reg_type;

typedef struct {
    volatile u_isp_ae_hist          isp_ae_hist[1024];      /* 0x0 ~ 0xffc, ae hist size: 1024 */
    volatile u_isp_ae_aver_r_gr     isp_ae_aver_r_gr[256];  /* 0x1000 ~ 0x13fc, ae aver size: 256 */
    volatile u_isp_ae_aver_gb_b     isp_ae_aver_gb_b[256];  /* 0x1400 ~ 0x17fc, ae aver size: 256 */
    volatile u_isp_af_stat          isp_af_stat[1020];      /* 0x1800 ~ 0x27ec, af stat size:1020 */
    volatile unsigned int           reserved_1[4];          /* 0x27f0 ~ 0x27fc, reserved 4 * 4 bytes */
} isp_fe_stat_reg_type;
#ifdef CONFIG_OT_ISP_DETAIL_STATS_SUPPORT
typedef struct {
    volatile u_isp_awb_avg_r_rstt            isp_awb_avg_r_rstt; /* 0x181c */
    volatile u_isp_awb_avg_g_rstt            isp_awb_avg_g_rstt; /* 0x1820 */
    volatile u_isp_awb_avg_b_rstt            isp_awb_avg_b_rstt; /* 0x1824 */
    volatile u_isp_awb_cnt_all_rstt          isp_awb_cnt_all_rstt; /* 0x1828 */
    volatile u_isp_awb_stat                  isp_awb_stat[2048]; /* 0x182c~0x3828 */
} isp_be_offline_detail_awb_stat;

typedef struct {
    volatile isp_be_offline_ae_stat_type     isp_detail_ae_stat; /* 0x0 ~ 0x1818 */
    volatile isp_be_offline_detail_awb_stat  isp_detail_awb_stat; /* 0x181c ~ 0x3828 */
    volatile unsigned int                    reserved[5];  /* 0x382c ~ 0x383c, reserved 20 bytes, 32byte align */
} isp_be_offline_detail_ae_awb_stat;

typedef struct {
    isp_be_offline_detail_ae_awb_stat   isp_ae_awb_stat[OT_ISP_DETAIL_STATS_BLK_NUM_MAX]; /* 0 ~ 0x545FC=0x3840 * 24 */
    volatile unsigned int               isp_other_stat[4853]; /* 0x54600 ~ 0x591D0 other stats 4853 * 4 bytes */
} isp_be_offline_detail_stat;
#endif
#endif /* ISP_STT_DEFINE_H */
