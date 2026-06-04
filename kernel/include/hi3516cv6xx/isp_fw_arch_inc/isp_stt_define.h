/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_STT_DEFINE_H
#define ISP_STT_DEFINE_H

/* Define the union u_isp_ae_total_stat_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_total_pixels_stt : 27; /* [26..0]  */
        unsigned int    reserved_0              : 5;  /* [31..27]  */
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
        unsigned int isp_awb_count_all_stt : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_isp_awb_cnt_all_rstt;


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
/* Define the union u_isp_yuv422_y_sum0_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_y_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;
} u_isp_yuv422_y_sum0_rstt;
/* Define the union u_isp_yuv422_y_sum1_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_y_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;
} u_isp_yuv422_y_sum1_rstt;
/* Define the union u_isp_yuv422_c_sum0_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_c_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;
} u_isp_yuv422_c_sum0_rstt;
/* Define the union u_isp_yuv422_c_sum1_rstt */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_c_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;
} u_isp_yuv422_c_sum1_rstt;


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
/* Define the union u_isp_ldci_lpf_map_rstt */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpflitmap_rstt : 8   ; /* [7..0]  */
        unsigned int    isp_ldci_lpfplyp2map_rstt : 8   ; /* [15..8]  */
        unsigned int    isp_ldci_lpfplyp3map_rstt : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
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

typedef struct {
    volatile  u_isp_ae_total_stat_rstt        isp_ae_total_stat_rstt           ; /* 0x0 */
    volatile  u_isp_ae_count_stat_rstt        isp_ae_count_stat_rstt           ; /* 0x4 */
    volatile  u_isp_ae_total_r_aver_rstt      isp_ae_total_r_aver_rstt         ; /* 0x8 */
    volatile  u_isp_ae_total_gr_aver_rstt     isp_ae_total_gr_aver_rstt        ; /* 0xc */
    volatile  u_isp_ae_total_gb_aver_rstt     isp_ae_total_gb_aver_rstt        ; /* 0x10 */
    volatile  u_isp_ae_total_b_aver_rstt      isp_ae_total_b_aver_rstt         ; /* 0x14 */
    volatile  u_isp_ae_hist_high_rstt         isp_ae_hist_high_rstt            ; /* 0x18 */
    volatile  u_isp_awb_avg_r_rstt            isp_awb_avg_r_rstt               ; /* 0x1c */
    volatile  u_isp_awb_avg_g_rstt            isp_awb_avg_g_rstt               ; /* 0x20 */
    volatile  u_isp_awb_avg_b_rstt            isp_awb_avg_b_rstt               ; /* 0x24 */
    volatile  u_isp_awb_cnt_all_rstt          isp_awb_cnt_all_rstt             ; /* 0x28 */
    volatile  u_isp_fpn_stat_rstt             isp_fpn_stat_rstt                ; /* 0x2c */
    volatile  u_isp_fpn_sum0_rstt             isp_fpn_sum0_rstt                ; /* 0x30 */
    volatile  u_isp_fpn_sum1_rstt             isp_fpn_sum1_rstt                ; /* 0x34 */
    volatile  u_isp_y_sum0_rstt               isp_y_sum0_rstt                  ; /* 0x38 */
    volatile  u_isp_y_sum1_rstt               isp_y_sum1_rstt                  ; /* 0x3c */
    volatile  u_isp_yuv422_y_sum0_rstt        isp_yuv422_y_sum0_rstt           ; /* 0x40 */
    volatile  u_isp_yuv422_y_sum1_rstt        isp_yuv422_y_sum1_rstt           ; /* 0x44 */
    volatile  u_isp_yuv422_c_sum0_rstt        isp_yuv422_c_sum0_rstt           ; /* 0x48 */
    volatile  u_isp_yuv422_c_sum1_rstt        isp_yuv422_c_sum1_rstt           ; /* 0x4c */
    volatile unsigned int                    reserved[44];                        /* 0x50  ~ 0xfc */
    volatile u_isp_ae_hist                   isp_ae_hist[1024]                ; /* 0x0~0xffc */
    volatile u_isp_ae_aver_r_gr              isp_ae_aver_r_gr[1024]           ; /* 0x1000~0x1ffc */
    volatile u_isp_ae_aver_gb_b              isp_ae_aver_gb_b[1024]           ; /* 0x2000~0x2ffc */
    volatile  u_isp_awb_stat                  isp_awb_stat[2048]               ; /* 0x3000~0x4ffc */
    volatile  u_isp_af_stat                   isp_af_stat[1020]                ; /* 0x5000~0x */
    volatile unsigned int                    reserved_1[4];
    volatile  u_isp_ldci_lpf_map_rstt         isp_ldci_lpf_map_rstt[384]       ; /* 0x6000~0x65fc */
    volatile  u_isp_dehaze_minstat            isp_dehaze_minstat[192]          ; /* 0x6600~0x68fc */
    volatile  u_isp_dehaze_maxstat            isp_dehaze_maxstat[384]          ; /* 0x6900~0x6efc */
} isp_be_offline_stat;

/* Define the global struct */
typedef struct {
    isp_be_offline_stat  be_stat;   /* align 256 */
} isp_be_offline_stat_reg_type;

#define ISP_AWB_STAT_SIZE   864
/* Define the global struct */
typedef struct {
   // isp_be_offline_ae_stat_type          isp_ae_atat;             /* 0x0 ~ 0x1818 */
    volatile u_isp_awb_avg_r_rstt        isp_awb_avg_r_rstt;      /* 0x181c */
    volatile u_isp_awb_avg_g_rstt        isp_awb_avg_g_rstt;      /* 0x1820 */
    volatile u_isp_awb_avg_b_rstt        isp_awb_avg_b_rstt;      /* 0x1824 */
    volatile u_isp_awb_cnt_all_rstt      isp_awb_cnt_all_rstt;    /* 0x1828 */
    volatile u_isp_awb_stat              isp_awb_stat[ISP_AWB_STAT_SIZE];  /* 0x182c~, awb stat lut size: 864 */
} isp_stitch_stat_reg_type;

typedef struct {
    volatile u_isp_ae_hist                   isp_ae_hist[1024]                ; /* 0x0~0xffc */
    volatile u_isp_ae_aver_r_gr              isp_ae_aver_r_gr[1024]           ; /* 0x1000~0x1ffc */
    volatile u_isp_ae_aver_gb_b              isp_ae_aver_gb_b[1024]           ; /* 0x2000~0x2ffc */
    volatile  u_isp_awb_stat                  isp_awb_stat[2048]               ; /* 0x3000~0x4ffc */
    volatile  u_isp_af_stat                   isp_af_stat[1020]                ; /* 0x5000~0x */
    volatile unsigned int                    reserved_1[4];
    volatile  u_isp_ldci_lpf_map_rstt         isp_ldci_lpf_map_rstt[384]       ; /* 0x6000~0x65fc */
    volatile  u_isp_dehaze_minstat            isp_dehaze_minstat[192]          ; /* 0x6600~0x68fc */
    volatile  u_isp_dehaze_maxstat            isp_dehaze_maxstat[384]          ; /* 0x6900~0x6efc */
} isp_be_online_stat_reg_type;


typedef struct {
    volatile u_isp_ae_hist          isp_ae_hist[256];      /* 0x0 ~ 0xffc, ae hist size: 1024 */
} isp_fe_stat_reg_type;

#endif /* ISP_STT_DEFINE_H */
