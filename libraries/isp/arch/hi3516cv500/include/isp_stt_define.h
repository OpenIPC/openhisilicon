// ******************************************************************************
// Copyright     :  Copyright (C) 2017, Hisilicon Technologies Co., Ltd.
// File name     :  isp_stt_define.h
// Author        :
// Version       :  1.0
// Date          :  2017-02-23
// Description   :  Define all registers/tables
// History       :  2017-02-23 Create file
// ******************************************************************************

#ifndef __ISP_STT_DEFINE_H__
#define __ISP_STT_DEFINE_H__

/* Define the union U_ISP_AE_TOTAL_STAT_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_total_pixels_stt : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_STAT_RSTT;

/* Define the union U_ISP_AE_COUNT_STAT_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_count_pixels_stt : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_COUNT_STAT_RSTT;

/* Define the union U_ISP_AE_TOTAL_R_AVER_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_r_aver_stt : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_R_AVER_RSTT;
/* Define the union U_ISP_AE_TOTAL_GR_AVER_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gr_aver_stt : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_GR_AVER_RSTT;
/* Define the union U_ISP_AE_TOTAL_GB_AVER_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gb_aver_stt : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_GB_AVER_RSTT;
/* Define the union U_ISP_AE_TOTAL_B_AVER_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_b_aver_stt : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_B_AVER_RSTT;
/* Define the union U_ISP_AE_HIST_HIGH_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_high_stt   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST_HIGH_RSTT;
/* Define the union U_ISP_AE_HIST */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_hist           : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST;

/* Define the union U_ISP_AE_AVER_R_GR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_aver_gr        : 16  ; /* [15..0]  */
        unsigned int    isp_ae_aver_r         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_R_GR;

/* Define the union U_ISP_AE_AVER_GB_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_aver_b         : 16  ; /* [15..0]  */
        unsigned int    isp_ae_aver_gb        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_GB_B;

/* Define the union U_ISP_AWB_AVG_R_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_r_stt     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_R_RSTT;

/* Define the union U_ISP_AWB_AVG_G_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_g_stt     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_G_RSTT;

/* Define the union U_ISP_AWB_AVG_B_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_b_stt     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_B_RSTT;

/* Define the union U_ISP_AWB_CNT_ALL_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_count_all_stt : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CNT_ALL_RSTT;

/* Define the union U_ISP_AWB_STAT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_STAT;
/* Define the union U_ISP_AF_STAT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_out        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT;
/* Define the union U_ISP_LA_AVER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_aver_b         : 8   ; /* [7..0]  */
        unsigned int    isp_la_aver_gb        : 8   ; /* [15..8]  */
        unsigned int    isp_la_aver_gr        : 8   ; /* [23..16]  */
        unsigned int    isp_la_aver_r         : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_AVER;

/* Define the union U_ISP_FLICK_GR_DIFF_CUR_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_diff_cur_rstt : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GR_DIFF_CUR_RSTT;

/* Define the union U_ISP_FLICK_GB_DIFF_CUR_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_diff_cur_rstt : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GB_DIFF_CUR_RSTT;

/* Define the union U_ISP_FLICK_GR_ABS_CUR_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_abs_cur_rstt : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GR_ABS_CUR_RSTT;

/* Define the union U_ISP_FLICK_GB_ABS_CUR_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_abs_cur_rstt : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GB_ABS_CUR_RSTT;

/* Define the union U_ISP_FLICK_GMEAN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gbmean      : 11  ; /* [10..0]  */
        unsigned int    isp_flick_grmean      : 11  ; /* [21..11]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GMEAN;

/* Define the union U_ISP_GE_MEM_AVER0_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_mem_aver0_gb_rstt : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ge_mem_aver0_gr_rstt : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER0_RSTT;

/* Define the union U_ISP_GE_MEM_AVER1_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_mem_aver1_gb_rstt : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ge_mem_aver1_gr_rstt : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER1_RSTT;

/* Define the union U_ISP_FPN_STAT_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_busy_rstt     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    isp_fpn_vcnt_rstt     : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_fpn_hcnt_rstt     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_STAT_RSTT;

/* Define the union U_ISP_FPN_SUM0_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum0_rstt      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SUM0_RSTT;
/* Define the union U_ISP_FPN_SUM1_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum1_rstt      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SUM1_RSTT;
/* Define the union U_ISP_DPC_BPT_CALIB_NUMBER_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_calib_number_rstt : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_CALIB_NUMBER_RSTT;

/* Define the union U_ISP_DPC_BPT_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_posx_rstt     : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_dpc_posy_rstt     : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_RSTT;

/* Define the union U_ISP_LDCI_GROBAL_MAP_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_grobal_luma_rstt : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_grobal_poply2_rstt : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_grobal_poply3_rstt : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_GROBAL_MAP_RSTT;

/* Define the union U_ISP_LDCI_LPF_MAP_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpflitmap_rstt : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_lpfplyp2map_rstt : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_lpfplyp3map_rstt : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_MAP_RSTT;

/* Define the union U_ISP_DRC_PPDTC_SUM_RSTT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_ppdtc_sum_rstt : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PPDTC_SUM_RSTT;

/* Define the union U_ISP_DEHAZE_MINSTAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_l  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_dehaze_minstat_h  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT;

/* Define the union U_ISP_DEHAZE_MAXSTAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat    : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT;

/* Define the union U_ISP_Y_SUM0_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_Y_SUM0_RSTT;
/* Define the union U_ISP_Y_SUM1_RSTT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum1             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_Y_SUM1_RSTT;

/* Define the global struct */
typedef struct {
    volatile U_ISP_AE_TOTAL_STAT_RSTT   ISP_AE_TOTAL_STAT_RSTT        ; /* 0x0 */
    volatile U_ISP_AE_COUNT_STAT_RSTT   ISP_AE_COUNT_STAT_RSTT        ; /* 0x4 */
    volatile U_ISP_AE_TOTAL_R_AVER_RSTT   ISP_AE_TOTAL_R_AVER_RSTT    ; /* 0x8 */
    volatile U_ISP_AE_TOTAL_GR_AVER_RSTT   ISP_AE_TOTAL_GR_AVER_RSTT  ; /* 0xc */
    volatile U_ISP_AE_TOTAL_GB_AVER_RSTT   ISP_AE_TOTAL_GB_AVER_RSTT  ; /* 0x10 */
    volatile U_ISP_AE_TOTAL_B_AVER_RSTT   ISP_AE_TOTAL_B_AVER_RSTT    ; /* 0x14 */
    volatile U_ISP_AE_HIST_HIGH_RSTT   ISP_AE_HIST_HIGH_RSTT          ; /* 0x18 */
    volatile U_ISP_AE_HIST          ISP_AE_HIST[1024]                 ; /* 0x1c~0x1018 */
    volatile U_ISP_AE_AVER_R_GR     ISP_AE_AVER_R_GR[256]             ; /* 0x101c~0x1418 */
    volatile U_ISP_AE_AVER_GB_B     ISP_AE_AVER_GB_B[256]             ; /* 0x141c~0x1818 */
    volatile U_ISP_AWB_AVG_R_RSTT   ISP_AWB_AVG_R_RSTT                ; /* 0x181c */
    volatile U_ISP_AWB_AVG_G_RSTT   ISP_AWB_AVG_G_RSTT                ; /* 0x1820 */
    volatile U_ISP_AWB_AVG_B_RSTT   ISP_AWB_AVG_B_RSTT                ; /* 0x1824 */
    volatile U_ISP_AWB_CNT_ALL_RSTT   ISP_AWB_CNT_ALL_RSTT            ; /* 0x1828 */
    volatile U_ISP_AWB_STAT         ISP_AWB_STAT[2048]                ; /* 0x182c~0x3828 */
    volatile U_ISP_AF_STAT          ISP_AF_STAT[1020]                 ; /* 0x382c~0x4818 */
    volatile U_ISP_LA_AVER          ISP_LA_AVER[256]                  ; /* 0x481c~0x4c18 */
    volatile U_ISP_FLICK_GR_DIFF_CUR_RSTT   ISP_FLICK_GR_DIFF_CUR_RSTT ; /* 0x4c1c */
    volatile U_ISP_FLICK_GB_DIFF_CUR_RSTT   ISP_FLICK_GB_DIFF_CUR_RSTT ; /* 0x4c20 */
    volatile U_ISP_FLICK_GR_ABS_CUR_RSTT   ISP_FLICK_GR_ABS_CUR_RSTT  ; /* 0x4c24 */
    volatile U_ISP_FLICK_GB_ABS_CUR_RSTT   ISP_FLICK_GB_ABS_CUR_RSTT  ; /* 0x4c28 */
    volatile U_ISP_FLICK_GMEAN      ISP_FLICK_GMEAN[512]              ; /* 0x4c2c~0x5428 */
    volatile U_ISP_GE_MEM_AVER0_RSTT   ISP_GE_MEM_AVER0_RSTT[256]     ; /* 0x542c~0x5828 */
    volatile U_ISP_GE_MEM_AVER1_RSTT   ISP_GE_MEM_AVER1_RSTT[256]     ; /* 0x582c~0x5c28 */
    volatile U_ISP_FPN_STAT_RSTT    ISP_FPN_STAT_RSTT                 ; /* 0x5c2c */
    volatile U_ISP_FPN_SUM0_RSTT    ISP_FPN_SUM0_RSTT                 ; /* 0x5c30 */
    volatile U_ISP_FPN_SUM1_RSTT    ISP_FPN_SUM1_RSTT                 ; /* 0x5c34 */
    volatile U_ISP_DPC_BPT_CALIB_NUMBER_RSTT   ISP_DPC_BPT_CALIB_NUMBER_RSTT ; /* 0x5c38 */
    volatile U_ISP_DPC_BPT_RSTT     ISP_DPC_BPT_RSTT[2048]            ; /* 0x5c3c~0x7c38 */
    volatile U_ISP_LDCI_GROBAL_MAP_RSTT   ISP_LDCI_GROBAL_MAP_RSTT    ; /* 0x7c3c */
    volatile U_ISP_LDCI_LPF_MAP_RSTT   ISP_LDCI_LPF_MAP_RSTT[1024]    ; /* 0x7c40~0x8c3c */
    volatile U_ISP_DRC_PPDTC_SUM_RSTT   ISP_DRC_PPDTC_SUM_RSTT        ; /* 0x8c40 */
    volatile U_ISP_DEHAZE_MINSTAT   ISP_DEHAZE_MINSTAT[512]           ; /* 0x8c44~0x9440 */
    volatile U_ISP_DEHAZE_MAXSTAT   ISP_DEHAZE_MAXSTAT[1024]          ; /* 0x9444~0xa440 */
    volatile U_ISP_Y_SUM0_RSTT      ISP_Y_SUM0_RSTT                   ; /* 0xa444 */
    volatile U_ISP_Y_SUM1_RSTT      ISP_Y_SUM1_RSTT                   ; /* 0xa448 */
} S_ISP_STT_REGS_TYPE;

/* Define the global struct */
typedef struct {
    volatile U_ISP_AE_TOTAL_STAT_RSTT   ISP_AE_TOTAL_STAT_RSTT        ; /* 0x0 */
    volatile U_ISP_AE_COUNT_STAT_RSTT   ISP_AE_COUNT_STAT_RSTT        ; /* 0x4 */
    volatile U_ISP_AE_TOTAL_R_AVER_RSTT   ISP_AE_TOTAL_R_AVER_RSTT    ; /* 0x8 */
    volatile U_ISP_AE_TOTAL_GR_AVER_RSTT   ISP_AE_TOTAL_GR_AVER_RSTT  ; /* 0xc */
    volatile U_ISP_AE_TOTAL_GB_AVER_RSTT   ISP_AE_TOTAL_GB_AVER_RSTT  ; /* 0x10 */
    volatile U_ISP_AE_TOTAL_B_AVER_RSTT   ISP_AE_TOTAL_B_AVER_RSTT    ; /* 0x14 */
    volatile U_ISP_AE_HIST_HIGH_RSTT   ISP_AE_HIST_HIGH_RSTT          ; /* 0x18 */
    volatile U_ISP_AE_HIST          ISP_AE_HIST[1024]                 ; /* 0x1c~0x1018 */
    volatile U_ISP_AE_AVER_R_GR     ISP_AE_AVER_R_GR[256]             ; /* 0x101c~0x1418 */
    volatile U_ISP_AE_AVER_GB_B     ISP_AE_AVER_GB_B[256]             ; /* 0x141c~0x1818 */
    volatile U_ISP_AWB_AVG_R_RSTT   ISP_AWB_AVG_R_RSTT                ; /* 0x181c */
    volatile U_ISP_AWB_AVG_G_RSTT   ISP_AWB_AVG_G_RSTT                ; /* 0x1820 */
    volatile U_ISP_AWB_AVG_B_RSTT   ISP_AWB_AVG_B_RSTT                ; /* 0x1824 */
    volatile U_ISP_AWB_CNT_ALL_RSTT   ISP_AWB_CNT_ALL_RSTT            ; /* 0x1828 */
    volatile U_ISP_AWB_STAT         ISP_AWB_STAT[2048]                ; /* 0x182c~0x3828 */
    volatile U_ISP_AF_STAT          ISP_AF_STAT[1020]                 ; /* 0x382c~0x4818 */
} S_ISP_STITCH_STT_REGS_TYPE;

typedef struct {
    volatile U_ISP_AE_HIST           ISP_AE_HIST[1024]                 ; /* 0x0~0xffc         */
    volatile U_ISP_AE_AVER_R_GR      ISP_AE_AVER_R_GR[256]             ; /* 0x1000~0x13fc  */
    volatile U_ISP_AE_AVER_GB_B      ISP_AE_AVER_GB_B[256]             ; /* 0x1400~0x17fc  */
    volatile U_ISP_AF_STAT           ISP_AF_STAT[1020]                 ; /* 0x1800~0x27ec */
    volatile unsigned int            reserved_4[4]                     ; /* 0x27f0~27fc      */
    volatile U_ISP_AWB_STAT          ISP_AWB_STAT[2048]                ; /* 0x2800~0x47fc  */
    volatile U_ISP_LA_AVER           ISP_LA_AVER[256]                  ; /* 0x4800~0x4bfc  */
    volatile U_ISP_DEHAZE_MINSTAT    ISP_DEHAZE_MINSTAT[512]           ; /* 0x4c00~0x53fc  */
    volatile U_ISP_DEHAZE_MAXSTAT    ISP_DEHAZE_MAXSTAT[1024]          ; /* 0x5400~0x63fc  */
    volatile U_ISP_LDCI_LPF_MAP_RSTT ISP_LDCI_LPF_MAP_RSTT[1024]       ; /* 0x6400~0x73fc  */
    volatile U_ISP_FLICK_GMEAN       ISP_FLICK_GMEAN[512]              ; /* 0x7400~0x7bfc  */
} S_ISP_ONLINE_STT_REGS_TYPE;

typedef struct {
    volatile U_ISP_AE_HIST          ISP_AE_HIST[1024]                 ; /* 0x0~0xffc      */
    volatile U_ISP_AE_AVER_R_GR     ISP_AE_AVER_R_GR[256]             ; /* 0x1000~0x13fc  */
    volatile U_ISP_AE_AVER_GB_B     ISP_AE_AVER_GB_B[256]             ; /* 0x1400~0x17fc  */
} S_ISP_FE_STT_REGS_TYPE;

#endif /* __ISP_STT_DEFINE_H__ */
