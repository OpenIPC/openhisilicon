/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mkp_isp.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __MKP_ISP_H__
#define __MKP_ISP_H__

#include "hi_type.h"
#include "hi_comm_3a_adapt.h"
#include "hi_common_adapt.h"
#include "hi_defines.h"
#include "isp_reg_define.h"
#include "isp_stt_define.h"
#include "hi_comm_snap_adapt.h"
#include "hi_comm_isp_adapt.h"
#include "isp_block.h"
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
#include "hi_comm_pq_ai.h"
#endif

#define IOC_TYPE_ISP                    'I'

#define MAX_ISP_STAT_BUF_NUM            2
#define ISP_LUT2STT_SYNC_BUF_MAX_NUM    2

#define ISP_VER_PRIX                    "_ISP_V"
#define ISP_VERSION                     CHIP_NAME ISP_VER_PRIX MK_VERSION(VER_X, VER_Y, VER_Z, VER_P, VER_B) VER_D

#define MAX_MMZ_NAME_LEN                16
#define ISP_INIT_HAND_SIGNAL            0x66778899
#define ISP_EXIT_HAND_SIGNAL            0x99887766
#define VIPROC_FRAME_START_INT          (1 << 10)
#define ISP_PROC_SIZE                   0x2000 /* 8KB */

typedef enum {
    IOC_NR_ISP_SET_FD = 0,
    IOC_NR_ISP_GET_FRAME_EDGE,
    IOC_NR_ISP_GET_VD_TIMEOUT,
    IOC_NR_ISP_GET_VD_END_TIMEOUT,
    IOC_NR_ISP_GET_VD_BEEND_TIMEOUT,
    IOC_NR_ISP_SET_INT_ENABLE,
    IOC_NR_ISP_GET_VERSION,

    IOC_NR_ISP_STAT_BUF_INIT,
    IOC_NR_ISP_STAT_BUF_EXIT,
    IOC_NR_ISP_STAT_BUF_GET,
    IOC_NR_ISP_STAT_BUF_PUT,
    IOC_NR_ISP_STAT_ACT_GET,
    IOC_NR_ISP_REG_CFG_INIT,
    IOC_NR_ISP_REG_CFG_SET,
    IOC_NR_ISP_BE_CFG_BUF_INIT,
    IOC_NR_ISP_GET_BE_BUF_FIRST,
    IOC_NR_ISP_BE_FREE_BUF_GET,
    IOC_NR_ISP_BE_LAST_BUF_GET,
    IOC_NR_ISP_BE_CFG_BUF_EXIT,
    IOC_NR_ISP_BE_CFG_BUF_CTL,
    IOC_NR_ISP_BE_CFG_BUF_RUNNING,
    IOC_NR_ISP_BE_All_BUF_INIT,
    IOC_NR_ISP_PROC_INIT,
    IOC_NR_ISP_PROC_WRITE_ING,
    IOC_NR_ISP_PROC_WRITE_OK,
    IOC_NR_ISP_PROC_EXIT,
    IOC_NR_ISP_PROC_PARAM_GET,
    IOC_NR_ISP_TRANS_BUF_INIT,
    IOC_NR_ISP_TRANS_BUF_EXIT,
    IOC_NR_ISP_PRO_BUF_INIT,
    IOC_NR_ISP_PRO_BUF_EXIT,
    IOC_NR_ISP_SYNC_CFG_SET,
    IOC_NR_ISP_SYNC_STITCH_PARAM_INIT,
    IOC_NR_WDR_SYNC_CFG_SET,
    IOC_NR_ISP_MEM_INFO_SET,
    IOC_NR_ISP_MEM_INFO_GET,
    IOC_NR_ISP_P2EN_INFO_GET,
    IOC_NR_ISP_INIT_INFO_SET,
    IOC_NR_ISP_SYNC_INIT_SET,
    IOC_NR_ISP_RUN_STATE_SET,
    IOC_NR_ISP_RESET_CTX,
    IOC_NR_ISP_CONFIG_INFO_SET,
    IOC_NR_ISP_SNAP_INFO_GET,
    IOC_NR_ISP_PRO_EN_GET,
    IOC_NR_ISP_UPDATE_POS_GET,
    IOC_NR_ISP_FRAME_CNT_GET,
    IOC_NR_ISP_SNAP_ATTR_GET,
    IOC_NR_ISP_PWM_NUM_GET,
    IOC_NR_ISP_RES_SWITCH_SET,
    IOC_NR_ISP_CHN_SELECT_CFG,
    IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET,

    IOC_NR_ISP_FRAME_INFO_SET,
    IOC_NR_ISP_FRAME_INFO_GET,
    IOC_NR_ISP_DNG_INFO_SET,
    IOC_NR_ISP_DNG_INFO_GET,
    IOC_NR_ISP_SET_MOD_PARAM,
    IOC_NR_ISP_GET_MOD_PARAM,
    IOC_NR_ISP_SET_CTRL_PARAM,
    IOC_NR_ISP_GET_CTRL_PARAM,

    IOC_NR_ISP_MODE_INFO,
    IOC_NR_ISP_WORK_MODE_INIT,
    IOC_NR_ISP_WORK_MODE_GET,
    IOC_NR_ISP_WORK_MODE_EXIT,
    IOC_NR_ISP_PRE_BLK_NUM_UPDATE,
    IOC_NR_ISP_GET_HDR_ATTR,
    IOC_NR_ISP_GET_WDR_ATTR,
    IOC_NR_ISP_GET_STITCH_ATTR,
    IOC_NR_ISP_GET_FUSION_ATTR,
    IOC_NR_ISP_GET_PIPE_SIZE,
    IOC_NR_ISP_PUB_ATTR_INFO,
    IOC_NR_ISP_CLUT_BUF_INIT,
    IOC_NR_ISP_CLUT_BUF_EXIT,
    IOC_NR_ISP_CLUT_BUF_GET,
    IOC_NR_ISP_STT_BUF_INIT,
    IOC_NR_ISP_STT_BUF_EXIT,
    IOC_NR_ISP_STT_ADDR_INIT,
    IOC_NR_ISP_BE_LUT_STT_BUF_GET,
    IOC_NR_ISP_KERNEL_RUNONCE,
    IOC_NR_ISP_YUV_RUNONCE_INFO,
    IOC_NR_ISP_KERNEL_YUV_RUNONCE,
    IOC_NR_ISP_OPT_RUNONCE_INFO,
    IOC_NR_ISP_SET_PROCALCDONE,
    IOC_NR_ISP_SPECAWB_BUF_INIT,
    IOC_NR_ISP_SPECAWB_BUF_EXIT,
    IOC_NR_ISP_SPECAWB_BUF_GET,
    IOC_NR_ISP_SET_RGBIR_FORMAT,
    IOC_NR_ISP_UPDATE_INFO_SET,
    IOC_NR_ISP_BE_SYNC_PARAM_INIT,
    IOC_NR_ISP_LDCI_BUF_INIT,
    IOC_NR_ISP_LDCI_BUF_EXIT,
    IOC_NR_ISP_LDCI_READ_STT_BUF_GET,
    IOC_NR_ISP_AWB_EN_SET,
    IOC_NR_ISP_BE_BUF_NUM_GET,
    IOC_NR_ISP_PQ_AI_GROUP_ATTR_SET,
    IOC_NR_ISP_PQ_AI_GROUP_ATTR_GET,
    IOC_NR_ISP_PQ_AI_POST_NR_ATTR_SET,
    IOC_NR_ISP_PQ_AI_POST_NR_ATTR_GET,
    IOC_NR_ISP_BUTT,
} ioc_nr_isp;

typedef struct {
    hi_u32  milli_sec;     /* RW, Range: [0x0, 0xFFFFFFFF], the number of the */
    hi_u32  int_status;    /* RO, when there is ISP interrupt, then the int_status is 1 */
} isp_vd_timeout;

#define ISP_VERSION_NUM_MAX 4
typedef struct {
    hi_u8 mpp_version[ISP_VERSION_NUM_MAX];
    hi_u32 magic;
} isp_version;

/* the statistics of isp */
typedef struct {
    hi_u16  defect_pixel_count[ISP_STRIPING_MAX_NUM];
    hi_u32  defect_pixel_lut[ISP_STRIPING_MAX_NUM][STATIC_DP_COUNT_NORMAL];
} isp_dp_stat;

#define ISP_FLICKER_G_MEAN_LENGTH       512
typedef struct {
    hi_u32 cur_diff_gr[ISP_STRIPING_MAX_NUM];                            /* s24.0, current frame gr sum */
    hi_u32 cur_diff_gb[ISP_STRIPING_MAX_NUM];                            /* s24.0, current frame gb sum */
    hi_u32 cur_abs_gr[ISP_STRIPING_MAX_NUM];                             /* u23.0 */
    hi_u32 cur_abs_gb[ISP_STRIPING_MAX_NUM];                             /* u23.0 */
    hi_u32 g_mean[ISP_STRIPING_MAX_NUM][ISP_FLICKER_G_MEAN_LENGTH];    /* u22.0 */
} isp_flicker_stat;

typedef struct {
    hi_u32  max_stat_dout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM];
    hi_u32  min_dout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM / DEFOG_REG_NUM];
} isp_dehaze_stat;

typedef struct {
    hi_u16  zone_avg[MG_ZONE_ROW][MG_ZONE_COLUMN][ISP_BAYER_CHN_NUM];
} isp_mg_stat;

typedef struct {
    hi_u32  global_map[LDCI_STAT_WND_Y][LDCI_STAT_WND_X];
} isp_ldci_stat;

typedef struct {
    hi_u16  white_balance_gain[ISP_BAYER_CHN_NUM];
} isp_comm_stat;

typedef struct {
    hi_isp_be_ae_stat_1  be_ae_stat1;
    hi_isp_be_ae_stat_2  be_ae_stat2;
    hi_isp_be_ae_stat_3  be_ae_stat3;
    hi_isp_awb_stat_1    awb_stat1;
    hi_isp_awb_stat_2    awb_stat2;
    hi_isp_be_af_stat    af_stat;
    isp_dp_stat         dp_stat;
    isp_comm_stat       comm_stat;
    isp_dehaze_stat   dehaze_stat;
    isp_mg_stat       mg_stat;
} isp_be_stat;

typedef struct {
    hi_isp_fe_ae_stat_1  fe_ae_stat1;
    hi_isp_fe_ae_stat_2  fe_ae_stat2;
    hi_isp_fe_ae_stat_3  fe_ae_stat3;
    hi_isp_be_ae_stat_1  be_ae_stat1;
    hi_isp_be_ae_stat_2  be_ae_stat2;
    hi_isp_be_ae_stat_3  be_ae_stat3;
    hi_isp_awb_stat_1    awb_stat1;
    hi_isp_awb_stat_2    awb_stat2;
    hi_isp_be_af_stat    be_af_stat;
    isp_dp_stat         dp_stat;
    isp_comm_stat       comm_stat;
    isp_dehaze_stat     dehaze_stat;
    isp_mg_stat       mg_stat;
    isp_flicker_stat    flicker_stat;
    isp_ldci_stat       ldci_stat;
    hi_bool             be_update;
} isp_stat;

#define DP_STAT_KEY_BIT                 14
#define FLICKER_STAT_KEY_BIT            18

typedef union {
    hi_u64  key;
    struct {
        hi_u64  bit1_fe_ae_glo_stat      : 1 ;   /* [0] */
        hi_u64  bit1_fe_ae_loc_stat      : 1 ;   /* [1] */
        hi_u64  bit1_fe_ae_sti_glo_stat  : 1 ;   /* [2] */
        hi_u64  bit1_fe_ae_sti_loc_stat  : 1 ;   /* [3] */
        hi_u64  bit1_be_ae_glo_stat      : 1 ;   /* [4] */
        hi_u64  bit1_be_ae_loc_stat      : 1 ;   /* [5] */
        hi_u64  bit1_be_ae_sti_glo_stat  : 1 ;   /* [6] */
        hi_u64  bit1_be_ae_sti_loc_stat  : 1 ;   /* [7] */
        hi_u64  bit1_awb_stat1           : 1 ;   /* [8] */
        hi_u64  bit1_awb_stat2           : 1 ;   /* [9] */
        hi_u64  bit1_awb_stat3           : 1 ;   /* [10] */
        hi_u64  bit1_awb_stat4           : 1 ;   /* [11] */
        hi_u64  bit1_fe_af_stat          : 1 ;   /* [12] */
        hi_u64  bit1_be_af_stat          : 1 ;   /* [13] */
        hi_u64  bit1_dp_stat             : 1 ;   /* [14] */
        hi_u64  bit1_comm_stat           : 1 ;   /* [15] */
        hi_u64  bit1_dehaze              : 1 ;   /* [16] */
        hi_u64  bit1_mg_stat             : 1 ;   /* [17] */
        hi_u64  bit1_flicker             : 1 ;   /* [18] */
        hi_u64  bit13_rsv                : 13;   /* [19:31] */
        hi_u64  bit32_isr_access         : 32;   /* [32:63] */
    };
} isp_stat_key;

typedef struct {
    isp_stat_key  stat_key;
    hi_u64   phy_addr;
    hi_void *virt_addr;                 /* (isp_stat *) */
} isp_stat_info;

typedef struct {
    hi_u64  phy_addr;
    hi_void *virt_addr;
    hi_u32 size;
    hi_bool usr_access;
} isp_stat_shadow_mem;

/* the register config of isp */
typedef struct {
    hi_u8   metering_hist_thresh[AE_PLANE_NUM]; /* 4 plane mode */
    hi_u8   weight_table[AE_ZONE_ROW][AE_ZONE_COLUMN];
} isp_ae_reg_cfg_1;

typedef struct {
    hi_u64  exposure;
    hi_u64  exposure_sf;
    hi_u32  int_time[WDR_CHN_MAX];
    hi_u32  isp_dgain;
    hi_u32  wdr_gain[WDR_CHN_MAX];

    hi_bool piris_valid;
    hi_s32  piris_pos;

    hi_isp_fswdr_mode fs_wdr_mode;
} isp_ae_reg_cfg_2;

typedef struct {
    hi_u8 fe_enable;
    hi_u8 be_enable;

    /* Overlap Configs */
    hi_u16 fe_crop_pos_x;
    hi_u16 fe_crop_pos_y;
    hi_u16 fe_crop_out_width;
    hi_u16 fe_crop_out_height;

    hi_u16 be_crop_pos_x;
    hi_u16 be_crop_pos_y;
    hi_u16 be_crop_out_width;
    hi_u16 be_crop_out_height;
} isp_ae_static_cfg;

typedef struct {
    /* ISP MPI Configs */
    hi_u8 fe_four_plane_mode;
    hi_u8 fe_hist_offset_x;
    hi_u8 fe_hist_offset_y;
    hi_u8 fe_hist_skip_x;
    hi_u8 fe_hist_skip_y;

    hi_u8 be_ae_sel;
    hi_u8 be_four_plane_mode;
    hi_u8 be_hist_offset_x;
    hi_u8 be_hist_offset_y;
    hi_u8 be_hist_skip_x;
    hi_u8 be_hist_skip_y;

    /* WDR Configs */
    hi_u8 fe_gamma_limit;
    hi_u8 fe_hist_gamma_mode;
    hi_u8 fe_aver_gamma_mode;
    hi_u8 fe_bit_move;

    hi_u8 be_gamma_limit;
    hi_u8 be_hist_gamma_mode;
    hi_u8 be_aver_gamma_mode;
    hi_u8 be_bit_move;

    /* Weight Table Configs */
    hi_u8 be_weight_table_update;
    hi_u8 be_weight_table_width;
    hi_u8 be_weight_table_height;
    hi_u8 be_weight_table[AE_ZONE_ROW][AE_ZONE_COLUMN];

    hi_u8 fe_weight_table_update;
    hi_u8 fe_weight_table_width;
    hi_u8 fe_weight_table_height;
    hi_u8 fe_weight_table[AE_ZONE_ROW][AE_ZONE_COLUMN];
} isp_ae_dyna_cfg;

typedef struct {
    isp_ae_static_cfg static_reg_cfg;
    isp_ae_dyna_cfg   dyna_reg_cfg;
} isp_ae_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   enable;
    hi_u8   rs_enable;
    hi_s16  hrs_filter_lut0[ISP_HRS_NUM];
    hi_s16  hrs_filter_lut1[ISP_HRS_NUM];
    hi_u16  width;
    hi_u16  height;
} isp_hrs_static_cfg;

typedef struct {
    hi_u32 reserved;
} isp_hrs_dyna_cfg;

typedef struct {
    isp_hrs_static_cfg static_reg_cfg;
    isp_hrs_dyna_cfg dyna_reg_cfg;
} isp_hrs_reg_cfg;

typedef struct {
    hi_u8 enable;

    /* Overlap Configs */
    hi_u16 crop_pos_x;
    hi_u16 crop_pos_y;
    hi_u16 crop_out_width;
    hi_u16 crop_out_height;
} isp_mg_static_cfg;

typedef struct {
    /* WDR Configs */
    hi_u8 gamma_limit;
    hi_u8 gamma_mode;
    hi_u8 bit_move;

    /* Zone Configs */
    hi_u8 zone_width;
    hi_u8 zone_height;
} isp_mg_dyna_cfg;

typedef struct {
    isp_mg_static_cfg static_reg_cfg;
    isp_mg_dyna_cfg dyna_reg_cfg;
} isp_mg_reg_cfg;

typedef struct {
    hi_bool resh;
} isp_dg_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  gain_r;
    hi_u16  gain_gr;
    hi_u16  gain_gb;
    hi_u16  gain_b;
    hi_u32  clip_value;
} isp_dg_dyna_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  gain_r[WDR_CHN_MAX];
    hi_u16  gain_gr[WDR_CHN_MAX];
    hi_u16  gain_gb[WDR_CHN_MAX];
    hi_u16  gain_b[WDR_CHN_MAX];
    hi_u32  clip_value;
} isp_fe_dg_dyna_cfg;

typedef struct {
    hi_bool dg_en;
    isp_dg_static_cfg static_reg_cfg;
    isp_dg_dyna_cfg   dyna_reg_cfg;
} isp_dg_reg_cfg;

typedef struct {
    hi_bool dg_en;
    isp_dg_static_cfg static_reg_cfg;
    isp_fe_dg_dyna_cfg   dyna_reg_cfg;
} isp_fe_dg_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_u16 gain_r0;
    hi_u16 gain_gr0;
    hi_u16 gain_gb0;
    hi_u16 gain_b0;
    hi_u16 gain_r1;
    hi_u16 gain_gr1;
    hi_u16 gain_gb1;
    hi_u16 gain_b1;
    hi_u16 gain_r2;
    hi_u16 gain_gr2;
    hi_u16 gain_gb2;
    hi_u16 gain_b2;
    hi_u16 gain_r3;
    hi_u16 gain_gr3;
    hi_u16 gain_gb3;
    hi_u16 gain_b3;
} isp_4dg_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u32 clip_value0;
    hi_u32 clip_value1;
    hi_u32 clip_value2;
    hi_u32 clip_value3;
} isp_4dg_dyna_cfg;

typedef struct {
    hi_bool enable;
    isp_4dg_static_cfg static_reg_cfg;
    isp_4dg_dyna_cfg   dyna_reg_cfg;
} isp_4dg_reg_cfg;

typedef struct {
    /* BE DYNAMIC */
    hi_u32  be_white_balance_gain[ISP_BAYER_CHN_NUM];
    hi_u16  be_color_matrix[CCM_MATRIX_SIZE];
    hi_u16  be_metering_white_level_awb;
    hi_u16  be_metering_black_level_awb;
    hi_u16  be_metering_cr_ref_max_awb;
    hi_u16  be_metering_cb_ref_max_awb;
    hi_u16  be_metering_cr_ref_min_awb;
    hi_u16  be_metering_cb_ref_min_awb;
    hi_u8   be_wb_work_en;
    hi_u8   be_cc_en;
    hi_bool be_cc_colortone_en;
    hi_u16  be_cc_r_gain;
    hi_u16  be_cc_g_gain;
    hi_u16  be_cc_b_gain;

    hi_u32 be_crop_pos_x;
    hi_u32 be_crop_pos_y;
    hi_u32 be_width;
    hi_u32 be_height;
    hi_u32 be_crop_out_height;
    hi_u32 be_crop_out_width;

    /* FE DYNAMIC */
    hi_u32 fe_white_balance_gain[ISP_BAYER_CHN_NUM];
    hi_u8 fe_wb_work_en;
    hi_u32 wdr_wb_gain[ISP_BAYER_CHN_NUM];
} isp_awb_reg_dyn_cfg;

typedef struct {
    /* BESTATIC */
    hi_bool be_awb_sta_cfg;
    hi_u8   be_awb_bitmove;
    hi_u8   be_awb_work_en;
    hi_u8   be_wb_work_en;

    hi_u32 be_awb_stat_raddr;
    hi_u8  be_cc_colortone_en;
    hi_u8  be_cc_en;

    hi_u32 be_top_k;
    hi_u32 be_top_b;
    hi_u32 be_bot_k;
    hi_u32 be_bot_b;

    hi_u32 be_cc_in_dc0;
    hi_u32 be_cc_in_dc1;
    hi_u32 be_cc_in_dc2;
    hi_u32 be_cc_out_dc0;
    hi_u32 be_cc_out_dc1;
    hi_u32 be_cc_out_dc2;
    hi_u32 be_wb_clip_value;

    hi_u16 be_awb_offset_comp;

    /* FESTATIC */
    hi_bool fe_awb_sta_cfg;
    hi_u8   fe_wb_work_en;

    hi_u32  fe_clip_value;
} isp_awb_reg_sta_cfg;

typedef struct {
    /* BE USER */
    hi_bool resh;
    hi_u32 update_index;
    hi_isp_awb_switch be_awb_switch;
    hi_u16 be_zone_row;
    hi_u16 be_zone_col;
    hi_u16 be_zone_bin;
    hi_u16 be_metering_bin_hist0;
    hi_u16 be_metering_bin_hist1;
    hi_u16 be_metering_bin_hist2;
    hi_u16 be_metering_bin_hist3;
} isp_awb_reg_usr_cfg;

typedef struct {
    isp_awb_reg_dyn_cfg        awb_reg_dyn_cfg;
    isp_awb_reg_sta_cfg        awb_reg_sta_cfg;
    isp_awb_reg_usr_cfg        awb_reg_usr_cfg;
} isp_awb_reg_cfg;

typedef struct {
    hi_u32  be_white_balance_gain[ISP_BAYER_CHN_NUM];
    hi_u32  wdr_wb_gain[ISP_BAYER_CHN_NUM];
    hi_u16  color_matrix[CCM_MATRIX_SIZE];
    hi_isp_awb_switch be_awb_switch;
} isp_awb_sync_cfg;

typedef isp_awb_sync_cfg isp_awb_reg_cfg_2;

typedef struct {
    hi_u32  wdr_gain[WDR_CHN_MAX];
    hi_u32  be_white_balance_gain[ISP_BAYER_CHN_NUM];
    hi_u32  wdr_wb_gain[ISP_BAYER_CHN_NUM];
} isp_sync_4dgain_cfg;

typedef struct {
    hi_u16 threshold_write;
    hi_u8 metrics_shift;
    hi_u8 np_offset;
    hi_bool af_enable;
    hi_bool iir0_enable0;
    hi_bool iir0_enable1;
    hi_bool iir0_enable2;
    hi_bool iir1_enable0;
    hi_bool iir1_enable1;
    hi_bool iir1_enable2;
    hi_isp_af_peak_mode peak_mode;
    hi_isp_af_squ_mode squ_mode;
    hi_bool offset_enable;
    hi_bool crop_enable;
    hi_bool lpf_enable;
    hi_bool mean_enable;
    hi_bool raw_mode;
    hi_bool iir0_ds_enable;
    hi_bool iir1_ds_enable;
    hi_bool fir0_lpf_enable;
    hi_bool fir1_lpf_enable;
    hi_bool iir0_ldg_enable;
    hi_bool iir1_ldg_enable;
    hi_bool fir0_ldg_enable;
    hi_bool fir1_ldg_enable;
    hi_bool update;

    hi_u8 bayer_mode;
    hi_u8 gamma;
    hi_u8 gain_limit;
    hi_u8 iir_plg_group0;
    hi_u8 iir_pls_group0;
    hi_u8 iir_plg_group1;
    hi_u8 iir_pls_group1;
    hi_u8 cnt_shift0_h;
    hi_u8 cnt_shift1_h;
    hi_u8 cnt_shift0_v;
    hi_u8 cnt_shift1_v;
    hi_u8 cnt_shift_y;
    hi_u8 iir_dilate0;
    hi_u8 iir_dilate1;
    hi_u8 hiligh_thre;
    hi_u8 af_pos_sel;

    hi_u16 mean_thres;
    hi_u16 window_vnum;
    hi_u16 window_hnum;
    hi_u16 iir_thre0_coring;
    hi_u16 iir_thre1_coring;
    hi_u16 iir_peak0_coring;
    hi_u16 iir_peak1_coring;
    hi_u16 fir_thre0_coring;
    hi_u16 fir_thre1_coring;
    hi_u16 fir_peak0_coring;
    hi_u16 fir_peak1_coring;
    hi_u16 iir_slope0_coring;
    hi_u16 iir_slope1_coring;
    hi_u16 fir_slope0_coring;
    hi_u16 fir_slope1_coring;
    hi_u16 offset_gr;
    hi_u16 offset_gb;
    hi_u16 iir0_shift_group0;
    hi_u16 iir1_shift_group0;
    hi_u16 iir2_shift_group0;
    hi_u16 iir3_shift_group0;
    hi_u16 iir0_shift_group1;
    hi_u16 iir1_shift_group1;
    hi_u16 iir2_shift_group1;
    hi_u16 iir3_shift_group1;
    hi_u16 iir_thre0_low;
    hi_u16 iir_thre0_high;
    hi_u16 iir_thre1_low;
    hi_u16 iir_thre1_high;
    hi_u16 iir_gain0_low;
    hi_u16 iir_gain0_high;
    hi_u16 iir_gain1_low;
    hi_u16 iir_gain1_high;
    hi_u16 iir_slope0_low;
    hi_u16 iir_slope0_high;
    hi_u16 iir_slope1_low;
    hi_u16 iir_slope1_high;
    hi_u16 fir_thre0_low;
    hi_u16 fir_thre0_high;
    hi_u16 fir_thre1_low;
    hi_u16 fir_thre1_high;
    hi_u16 fir_gain0_low;
    hi_u16 fir_gain0_high;
    hi_u16 fir_gain1_low;
    hi_u16 fir_gain1_high;
    hi_u16 fir_slope0_low;
    hi_u16 fir_slope0_high;
    hi_u16 fir_slope1_low;
    hi_u16 fir_slope1_high;
    hi_u16 acc_shift0_h;
    hi_u16 acc_shift1_h;
    hi_u16 acc_shift0_v;
    hi_u16 acc_shift1_v;
    hi_u16 acc_shift_y;
    hi_u16 crop_pos_y;
    hi_u16 crop_pos_x;
    hi_u16 shift_count_y;
    hi_u16 crop_v_size;
    hi_u16 crop_h_size;

    hi_s16 iir_gain0_group0;
    hi_s16 iir_gain0_group1;
    hi_s16 iir_gain1_group0;
    hi_s16 iir_gain1_group1;
    hi_s16 iir_gain2_group0;
    hi_s16 iir_gain2_group1;
    hi_s16 iir_gain3_group0;
    hi_s16 iir_gain3_group1;
    hi_s16 iir_gain4_group0;
    hi_s16 iir_gain4_group1;
    hi_s16 iir_gain5_group0;
    hi_s16 iir_gain5_group1;
    hi_s16 iir_gain6_group0;
    hi_s16 iir_gain6_group1;
    hi_s16 fir_h_gain0_group0;
    hi_s16 fir_h_gain0_group1;
    hi_s16 fir_h_gain1_group0;
    hi_s16 fir_h_gain1_group1;
    hi_s16 fir_h_gain2_group0;
    hi_s16 fir_h_gain2_group1;
    hi_s16 fir_h_gain3_group0;
    hi_s16 fir_h_gain3_group1;
    hi_s16 fir_h_gain4_group0;
    hi_s16 fir_h_gain4_group1;

    hi_u32 stat_read_addr;
    hi_u32 stat_read_data;
    hi_u32 update_index;
} isp_af_reg_cfg;

typedef struct {
    hi_u16  black_level_offset[ISP_BAYER_CHN_NUM];
} isp_offset_reg_cfg;

/* DRC register configure start */
typedef struct {
    hi_bool static_resh;

    hi_bool rch_en;
    hi_bool wch_en;

    hi_bool rdstat_en;
    hi_bool wrstat_en;

    hi_bool monochrome_mode;
    hi_bool detail_boost_enable;
    hi_bool pdw_sum_enable;
    hi_bool wgt_box_tri_sel;

    hi_u8   off_line_mode;

    hi_u8   bin_num_z;

    hi_u8   bit_width_out;
    hi_u8   local_edge_lmt;

    hi_u8   r_wgt;         /* u4.0 */
    hi_u8   g_wgt;         /* u5.0 */
    hi_u8   b_wgt;         /* u4.0 */

    hi_u8   color_control_mode;
    hi_u8   color_control_lut_ctrl;
    hi_u16  global_color_corr;

    hi_u8   pfrrg_slo;
    hi_u8   pfrrg_wid;
    hi_u8   pfrrg_ctr;

    hi_u8   pfrbg_slo;
    hi_u8   pfrbg_thr;

    hi_u8   seg_idx_base[HI_ISP_DRC_TM_SEG_NUM];
    hi_u8   seg_max_val[HI_ISP_DRC_TM_SEG_NUM];

    hi_u8   update_mode;
    hi_u16  timing_stat;
    hi_u8   drc_update;

    hi_bool first_frame;
    hi_u8   shp_log;
    hi_u8   shp_exp;
    hi_u32  div_denom_log;
    hi_u32  denom_exp;
    hi_u32  prev_luma[HI_ISP_DRC_EXP_COMP_SAMPLE_NUM];

    /* regs for DRC STAT */
    hi_bool drcs_en;
    hi_bool drcs_vbi_flt_en;
    hi_bool drcs_wrt_stat_en;
} isp_drc_static_cfg;

typedef struct {
    hi_bool dyna_resh;
    hi_bool img_size_changed;
    hi_bool vbiflt_en;
    hi_bool color_corr_enable;
    hi_bool lut_update;

    hi_u16  strength_bias;
    hi_u16  strength;
    hi_u16  lut_mix_ctrl;
    hi_u16  auto_strength_max;
    hi_u16  auto_strength_min;

    hi_u16  tone_mapping_value0[HI_ISP_DRC_TM_NODE_NUM];
    hi_u16  tone_mapping_diff0[HI_ISP_DRC_TM_NODE_NUM];

    hi_u8   pfr_high_thr;

    hi_u16  v_size;
    hi_u16  h_size;
    hi_u8   off_line_mode;

    hi_u8   block_v_num;
    hi_u8   block_h_num;
    hi_u8   block_chk_y;
    hi_u8   block_chk_x;
    hi_u16  block_v_size;
    hi_u16  block_h_size;

    hi_u16  div_y0;
    hi_u16  div_x0;
    hi_u16  div_y1;
    hi_u16  div_x1;
    hi_u8   bin_scale;

    hi_u8   big_x_init;
    hi_u8   idx_x_init;
    hi_u16  cnt_x_init;
    hi_u16  acc_x_init;
    hi_u16  wgt_x_init;
    hi_u16  total_width;
    hi_u16  stat_width;

    hi_u8   drcs_big_x_init;
    hi_u8   drcs_idx_x_init;
    hi_u16  drcs_cnt_x_init;
    hi_u16  drcs_acc_x_init;
    hi_u16  drcs_wgt_x_init;
    hi_u16  drcs_total_width;
    hi_u16  drcs_stat_width;

    hi_u16  drcs_v_size;
    hi_u16  drcs_h_size;
} isp_drc_dyna_cfg;

typedef struct {
    hi_bool usr_resh;
    hi_u16  exposure;

    hi_u8   ysft1;
    hi_u8   yval1;
    hi_u8   ysft2;
    hi_u8   yval2;

    hi_u8   csft1;
    hi_u8   cval1;
    hi_u8   csft2;
    hi_u8   cval2;

    hi_u8   sft;
    hi_u8   val;

    hi_u8   mixing_coring;
    hi_u8   mixing_dark_min;
    hi_u8   mixing_dark_max;
    hi_s8   mixing_dark_slo;
    hi_u8   mixing_dark_thr;

    hi_u8   mixing_bright_min;
    hi_u8   mixing_bright_max;
    hi_s8   mixing_bright_slo;
    hi_u8   mixing_bright_thr;

    hi_u8   gain_clip_knee;
    hi_u8   gain_clip_step;

    hi_u8   pfr_high_slo;
    hi_u8   pfr_low_slo;
    hi_u8   pfr_low_thr;

    hi_u8   grad_shift;
    hi_u8   grad_slope;
    hi_u8   grad_max;
    hi_u8   grad_thr;

    hi_u8   var_spa_coarse;
    hi_u8   var_spa_medium;
    hi_u8   var_spa_fine;

    hi_u8   var_rng_coarse;
    hi_u8   var_rng_medium;
    hi_u8   var_rng_fine;

    hi_u8   bin_mix_coarse[HI_ISP_DRC_BIN_MIX_NODE_NUM];
    hi_u8   bin_mix_medium[HI_ISP_DRC_BIN_MIX_NODE_NUM];

    hi_u8   flt_spa_coarse;
    hi_u8   flt_spa_medium;
    hi_u8   flt_spa_fine;
    hi_u8   flt_rng_coarse;
    hi_u8   flt_rng_medium;
    hi_u8   flt_rng_fine;

    hi_u8   dis_thr_coef_high;
    hi_u8   dis_thr_coef_low;
    hi_u8   dis_offset_coef;
    hi_u8   flt_rng_ada_max;

    hi_u8   flt_spa_ada_max;
    hi_u8   diff_thr_low;
    hi_u8   diff_thr_high;

    hi_s8   detail_sub_factor;

    hi_u16  pfr_sum;
    hi_u16  cclut[HI_ISP_DRC_CC_NODE_NUM];     /* color correction LUT */

    /* new add for HDR mode */
    hi_u8   detail_coring;
    hi_u8   detail_dark_step;
    hi_u8   detail_bright_step;

    hi_s8   detail_dark_slo;
    hi_u8   detail_dark_thr;
    hi_u8   detail_dark_max;
    hi_u8   detail_dark_min;

    hi_s8   detail_bright_slo;
    hi_u8   detail_bright_thr;
    hi_u8   detail_bright_max;
    hi_u8   detail_bright_min;

    hi_u8   detail_dark_curve[HI_ISP_DRC_DT_CURVE_NODE_NUM];
    hi_u8   detail_bright_curve[HI_ISP_DRC_DT_CURVE_NODE_NUM];

    hi_u8   suppress_bright_max;
    hi_u8   suppress_bright_min;
    hi_u8   suppress_bright_thr;
    hi_u8   suppress_bright_slo;
    hi_u8   suppress_dark_max;
    hi_u8   suppress_dark_min;
    hi_u8   suppress_dark_thr;
    hi_u8   suppress_dark_slo;

    hi_bool cubic_curve_sel;

    hi_u32  update_index;
} isp_drc_usr_cfg;

typedef struct {
    hi_u8   shp_log;
    hi_u8   shp_exp;
    hi_bool is_offline_repeat_mode;
    hi_s32  prev_luma_delta[HI_ISP_DRC_EXP_COMP_SAMPLE_NUM - 1];
} isp_drc_sync_cfg;

typedef isp_drc_sync_cfg ISP_DRC_REG_CFG_2_S;

typedef struct {
    hi_bool drc_en;      /* enable */
    isp_drc_static_cfg static_reg_cfg;
    isp_drc_dyna_cfg   dyna_reg_cfg;
    isp_drc_usr_cfg    usr_reg_cfg;
    isp_drc_sync_cfg   sync_reg_cfg;
} isp_drc_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_u8   bit_depth_in;
    hi_u8   bit_depth_out;
    hi_u8   seg_max_val[HI_ISP_PREGAMMA_SEG_NUM];
    hi_u8   seg_idx_base[HI_ISP_PREGAMMA_SEG_NUM];
} isp_pregamma_static_cfg;

typedef struct {
    hi_bool switch_mode;
    hi_bool pregamma_lut_update_en;
    hi_u8   buf_id;
    hi_u32  update_index;
    hi_u32  pregamma_lut[PREGAMMA_NODE_NUM];  /* u21.0 */
} isp_pregamma_dyna_cfg;

typedef struct {
    hi_bool pregamma_en;
    isp_pregamma_dyna_cfg   dyna_reg_cfg;
    isp_pregamma_static_cfg static_reg_cfg;
} isp_pregamma_reg_cfg;
/* pre_gamma register configure end */
typedef enum {
    ISP_DIS_DATA_TYPE_YUV = 0,
    ISP_DIS_DATA_TYPE_RAW = 1,
    ISP_DIS_DATA_TYPE_BUTT
} isp_dis_data_type;

#define DIS_STATS_NUM                   9
typedef struct {
    hi_s32 h_delta[DIS_STATS_NUM];
    hi_s32 h_sad[DIS_STATS_NUM];
    hi_s32 h_mv[DIS_STATS_NUM];
    hi_s32 v_delta[DIS_STATS_NUM];
    hi_s32 v_sad[DIS_STATS_NUM];
    hi_s32 v_mv[DIS_STATS_NUM];
    hi_u32 h_motion;
    hi_u32 v_motion;
    hi_u32 h_offset;
    hi_u32 v_offset;
} isp_dis_stats;

typedef struct {
    hi_bool resh;
    hi_u8  max_mode;
    hi_u16 dehaze_thld;

    hi_u16 dehaze_blthld;
    hi_u8  dehaze_neg_mode;

    hi_u16 block_sum;
    hi_u8   dchnum;
    hi_u8   dcvnum;

    hi_u8   offline_mode;
    hi_u8   update_mode;
    hi_u8   dehaze_update;
    hi_u16   f_hblank;
    hi_u16   b_hblank;
    hi_u16   f_vblank;
    hi_u16   b_vblank;
} isp_dehaze_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   buf_id;
    hi_u8   strength;
    hi_u16  air_r;
    hi_u16  air_g;
    hi_u16  air_b;
    hi_u8   lut[DEFOG_LUT_NUM];
    hi_u32  prestat[DEFOG_ZONE_NUM / DEFOG_REG_NUM];
    hi_u32  update;
    hi_u32  lut_update;

    hi_u32  width;
    hi_u32  height;
    hi_u16  blockhsize;
    hi_u16  blockvsize;
    hi_u32  phasex;
    hi_u32  phasey;

    hi_u32 sml_map_offset; /* 20bit */
    hi_u32 stat_start_x;
    hi_u32 stat_end_x;
    hi_u8  statnum_h;
    hi_u8  statnum_v;
} isp_dehaze_dyna_cfg;

typedef struct {
    hi_bool   dehaze_en;
    hi_bool   lut2_stt_en;
    isp_dehaze_dyna_cfg   dyna_reg_cfg;
    isp_dehaze_static_cfg static_reg_cfg;
} isp_dehaze_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_u8   dpcc_output_mode;      /* U4.0 */
    hi_u32  dpcc_bpt_ctrl;         /* U12.0 */
} isp_dpc_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   buf_id;
    hi_u16  dpcc_bpt_number;
    hi_u32  update_index;
    hi_u32  dpcc_bp_table[STATIC_DP_COUNT_NORMAL];
} isp_dpc_usr_static_cor_cfg;

typedef struct {
    hi_bool resh;
    hi_bool dpcc_hard_thr_en;
    hi_s8   dpcc_sup_twinkle_thr_max;
    hi_s8   dpcc_sup_twinkle_thr_min;
    hi_u16  dpcc_rake_ratio;
} isp_dpc_usr_dyna_cor_cfg;

typedef struct {
    isp_dpc_usr_dyna_cor_cfg   usr_dyna_cor_reg_cfg;
    isp_dpc_usr_static_cor_cfg usr_sta_cor_reg_cfg;
} isp_dpc_usr_cfg;

typedef struct {
    hi_bool resh;
    hi_bool dpc_stat_en;
    hi_u8   dpcc_set_use;                           /* U4.0 */
    hi_u8   dpcc_line_kerdiff_fac;
    hi_u8   dpcc_blend_mode;
    hi_u8   dpcc_bit_depth_sel;
    hi_u8   dpcc_line_std_thr[ISP_DPC_LCH_SET_NUM];
    hi_u8   dpcc_line_diff_thr[ISP_DPC_LCH_SET_NUM];
    hi_u8   dpcc_line_aver_fac[ISP_DPC_LCH_SET_NUM];
    hi_u16  dpcc_mode;                              /* U10.0 */
    hi_u16  dpcc_methods_set1;                      /* U13.0 */
    hi_u16  dpcc_methods_set2;                      /* U13.0 */
    hi_u16  dpcc_methods_set3;                      /* U13.0 */
    hi_u16  dpcc_line_thr[ISP_DPC_SET_NUM];         /* U16.0 */
    hi_u16  dpcc_line_mad_fac[ISP_DPC_SET_NUM];     /* U14.0 */
    hi_u16  dpcc_pg_fac[ISP_DPC_SET_NUM];           /* U14.0 */
    hi_u16  dpcc_rnd_thr[ISP_DPC_SET_NUM];          /* U16.0 */
    hi_u16  dpcc_rg_fac[ISP_DPC_SET_NUM];           /* U14.0 */
    hi_u16  dpcc_ro_limits;                         /* U12.0 */
    hi_u16  dpcc_rnd_offs;                          /* U12.0 */
    hi_u32  dpcc_bad_thresh;                        /* U32.0 */
    hi_u32  dpcc_alpha;                             /* U32.0 */
} isp_dpc_dyna_cfg;

typedef struct {
    hi_bool dpc_en[ISP_DPC_MAX_CHN_NUM];
    hi_bool lut2_stt_en;
    hi_u8   chn_num;
    isp_dpc_static_cfg    static_reg_cfg;
    isp_dpc_dyna_cfg      dyna_reg_cfg;
    isp_dpc_usr_cfg       usr_reg_cfg;
} isp_dpc_reg_cfg;

typedef struct {
    hi_bool  static_resh;
    hi_u8    mf_thd_sel_d;
    hi_u8    hf_thd_sel_d;
    hi_u8    dir_var_scale;
    hi_u8    dir_rly[2];       /* dir rly size 2 */
    hi_u8    range_thd1;       /* U8.0 */
    hi_u8    range_dir_wgt1;   /* U0.7 */

    hi_u8    en_hf_eps;        /* U1.0 */
    hi_u8    hf_eps_val0;      /* U0.7 */

    hi_u8    max_var_clip_min; /* 8 bit */
    hi_u16   o_max_chg;
    hi_u16   u_max_chg;
    hi_u8    osht_var_wgt1;
    hi_u8    usht_var_wgt1;
    hi_u8    osht_var_diff_wgt0;
    hi_u8    usht_var_diff_wgt0;
    hi_u8    osht_var_thd0;
    hi_u8    usht_var_thd0;
    hi_u8    sht_var_sft;
    hi_u8    lmt_mf[ISP_SHARPEN_FREQ_CORING_LENGTH];
    hi_u8    lmt_hf[ISP_SHARPEN_FREQ_CORING_LENGTH];

    hi_u8    skin_src_sel;
    hi_u8    skin_cnt_thd[2];   /* skin cnt thd size 2 */
    hi_u8    skin_edge_sft;
    hi_u8    skin_edge_thd[2];  /* skin edg thd size 2 */
    hi_u16   skin_accum_thd[2]; /* skin acc thd size 2 */
    hi_u8    skin_accum_wgt[2]; /* skin acc wgt size 2 */

    hi_u8    chr_r_var_sft;
    hi_u8    rc_bmin;
    hi_u8    rc_bmax;
    hi_u8    rc_rmin;
    hi_u8    rc_rmax;
    hi_u8    chr_r_ori_cb;    /* U8.0 */
    hi_u8    chr_r_ori_cr;    /* U8.0 */
    hi_u8    chr_r_sft[4];    /* chr r sft size 4 */
    hi_u8    chr_r_thd[2];    /* chr r thd size 2 */
    hi_u8    gc_bmin;
    hi_u8    gc_bmax;
    hi_u8    gc_rmin;
    hi_u8    gc_rmax;
    hi_u8    chr_g_ori_cb;    /* U8.0 */
    hi_u8    chr_g_ori_cr;    /* U8.0 */
    hi_u8    chr_g_sft[4];    /* chr g sft size 4 */
    hi_u8    chr_g_thd[2];    /* chr g thd size 2 */
    hi_u8    chr_b_var_sft;
    hi_u8    bc_bmin;
    hi_u8    bc_bmax;
    hi_u8    bc_rmin;
    hi_u8    bc_rmax;
    hi_u8    chr_b_ori_cb;    /* U8.0 */
    hi_u8    chr_b_ori_cr;    /* U8.0 */
    hi_u8    chr_b_sft[4];    /* chr b sft size 4 */
    hi_u8    chr_b_thd[2];    /* chr b thd size 2 */

    hi_u8    luma_src_sel;
    hi_u8    sht_noise_max;
    hi_u8    sht_noise_min;

    /* filter */
    hi_s8    lpf_coef_ud[3];  /* S8.0  lpf coef ud size  3  */
    hi_s8    lpf_coef_d[3];   /* S8.0  lpf coef  d size  3  */
    hi_s8    hsf_coef_ud[3];  /* S8.0  hsf coef ud size  3  */
    hi_s8    hsf_coef_d0[13]; /* S8.0  hsf coef d0 size 13  */
    hi_s8    hsf_coef_d1[13]; /* S8.0  hsf coef d1 size 13  */
    hi_s8    hsf_coef_d2[13]; /* S8.0  hsf coef d2 size 13  */
    hi_s8    hsf_coef_d3[13]; /* S8.0  hsf coef d3 size 13  */

    hi_u8    lpf_sft_ud;     /* U4.0--[3, 15] */
    hi_u8    lpf_sft_d;      /* U4.0--[3, 15] */
    hi_u8    hsf_sft_ud;     /* U4.0--[3, 15] */
    hi_u8    hsf_sft_d;      /* U4.0--[3, 15] */

    hi_u8    en_shp8_dir;

    /* shoot ctrl */
    hi_u8    hf_gain_sft;
    hi_u8    mf_gain_sft;
    hi_u8    sht_var_sel;
    hi_u8    sht_var5x5_sft;
    hi_u8    detail_thd_sel;
    hi_u8    detail_thd_sft;

    hi_u8    skin_cnt_mul;
    hi_s16   skin_accum_mul;
} isp_sharpen_static_reg_cfg;

typedef struct {
    hi_bool  resh;
    hi_u8    mf_thd_sft_d;
    hi_u8    mf_thd_sel_ud;
    hi_u8    mf_thd_sft_ud;
    hi_u8    hf_thd_sft_d;
    hi_u8    hf_thd_sel_ud;
    hi_u8    hf_thd_sft_ud;
    hi_u8    dir_var_sft;
    hi_u8    range_thd0;        /* U8.0 */
    hi_u8    range_dir_wgt0;    /* U0.7 */

    hi_u8    hf_eps_var_thr0;
    hi_u8    hf_eps_var_thr1;
    hi_u8    hf_eps_val1;

    hi_u8    sel_pix_wgt;
    hi_u8    osht_var_wgt0;
    hi_u8    usht_var_wgt0;
    hi_u8    osht_var_diff_thd[2]; /* osht var diff thd size 2 */
    hi_u8    osht_var_diff_wgt1;
    hi_u8    usht_var_diff_thd[2]; /* usht var diff thd size 2 */
    hi_u8    usht_var_diff_wgt1;

    hi_u8    rmf_gain_scale;
    hi_u8    bmf_gain_scale;
    hi_s16   range_dir_mul;       /* convert division to multiplication.            12-bit signed integer  S8.4 */
    hi_s16   hf_eps_mul;          /* convert division to multiplication.            12-bit signed integer  S8.4 */
    hi_s16   osht_var_diff_mul;   /* convert division to multiplication.            12-bit signed integer  S8.4 */
    hi_s16   usht_var_diff_mul;   /* convert division to multiplication.            12-bit signed integer  S8.4 */
} isp_sharpen_default_dyna_reg_cfg;

typedef struct {
    hi_bool  resh;
    hi_bool  pre_reg_new_en;
    hi_bool  switch_mode;
    hi_u8    buf_id;
    hi_u16   mf_gain_d[SHRP_GAIN_LUT_SIZE];
    hi_u16   mf_gain_ud[SHRP_GAIN_LUT_SIZE];
    hi_u16   hf_gain_d[SHRP_GAIN_LUT_SIZE];
    hi_u16   hf_gain_ud[SHRP_GAIN_LUT_SIZE];
    hi_u8    osht_amt;
    hi_u8    usht_amt;

    hi_u8    en_chr_ctrl;
    hi_u8    chr_r_gain;       /* U1.5  [0 32] */
    hi_u16   chr_g_gain;       /* U1.5  [0 32] */
    hi_u16   chr_gmf_gain;     /* U1.5  [0 32] */
    hi_u8    chr_b_gain;       /* U1.5  [0 32] */
    hi_u8    en_skin_ctrl;
    hi_u8    skin_edge_wgt[2]; /* U0.5  [0 31] skin edge wgt size 2  */

    hi_u8    en_sht_ctrl_by_var;
    hi_u8    sht_bld_rt;
    hi_u8    osht_var_thd1;
    hi_u8    usht_var_thd1;
    hi_u8    en_luma_ctrl;
    hi_bool  en_detail_ctrl;
    hi_u8    detail_osht_amt;
    hi_u8    detail_usht_amt;
    hi_u8    luma_wgt[ISP_SHARPEN_LUMA_NUM];
    hi_u16   osht_var_mul;        /* convert division to multiplication.            11-bit unsigned integer  U7.4 */
    hi_u16   usht_var_mul;        /* convert division to multiplication.            11-bit unsigned integer  U7.4 */
    hi_s16   detail_osht_mul;
    hi_s16   detail_usht_mul;
    hi_u8    dir_diff_sft;
    hi_u8    dir_rt[2] ;     /* dir rt size 2 */
    hi_s16   chr_r_mul;      /* 9-bit signed integer      S5.4 */
    hi_s16   chr_g_mul;      /* 9-bit signed integer      S5.4 */
    hi_s16   chr_gmf_mul;    /* 9-bit signed integer      S5.4 */
    hi_s16   chr_b_mul;      /* 9-bit signed integer      S5.4 */
    hi_s16   skin_edge_mul;
    hi_u8    skin_max_u;
    hi_u8    skin_min_u;
    hi_u8    skin_max_v;
    hi_u8    skin_min_v;
    hi_u16   o_max_gain;   /* 11bit */
    hi_u16   u_max_gain;   /* 11bit */
    hi_u8    detail_osht_thr[2]; /* detail osht thr size 2 */
    hi_u8    detail_usht_thr[2]; /* detail usht thr size 2 */
    hi_u32   update_index;
} isp_sharpen_mpi_dyna_reg_cfg;

typedef struct {
    isp_sharpen_mpi_dyna_reg_cfg          mpi_dyna_reg_cfg;
    isp_sharpen_default_dyna_reg_cfg      default_dyna_reg_cfg;
} isp_sharpen_dyna_reg_cfg;

typedef struct {
    hi_bool  enable;
    hi_bool  lut2_stt_en;
    isp_sharpen_dyna_reg_cfg          dyna_reg_cfg;
    isp_sharpen_static_reg_cfg        static_reg_cfg;
} isp_sharpen_reg_cfg;

typedef struct {
    hi_u8   enable;
    hi_u8   mark_edge_sft;
    hi_u8   mark_edge_thd;
    hi_u8   u_mark_value;
    hi_u8   v_mark_value;
    hi_u32  update_index;
} isp_edgemark_reg_cfg;

typedef struct {
    hi_u8   enable;
    hi_u8   y_max_value;
    hi_u8   y_max_load_value;
    hi_u8   y_min_value;
    hi_u8   y_min_load_value;
    hi_u8   c_max_value;
    hi_u8   c_max_load_value;
    hi_u8   c_min_value;
    hi_u8   c_min_load_value;
    hi_u32  update_index;
} isp_hlc_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_bool ge_gr_gb_en;          /* u1.0 */
    hi_bool ge_gr_en;            /* u1.0 */
    hi_bool ge_gb_en;            /* u1.0 */
    hi_u8   ge_num_v;
} isp_ge_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   ge_num_h;
    hi_u8   ge_ct_slope1[HI_ISP_GE_MAX_CHN_NUM];   /* u4.0    0x9 */
    hi_u8   ge_ct_slope2[HI_ISP_GE_MAX_CHN_NUM];   /* u4.0    0x9 */
    hi_u16  ge_ct_th2[HI_ISP_GE_MAX_CHN_NUM];     /* u14.0   0x200 */

    hi_u16  ge_crop_pos_x;      /* u16.0 */
    hi_u16  ge_crop_pos_y;      /* u16.0 */
    hi_u16  ge_crop_out_width;  /* u16.0 */
    hi_u16  ge_crop_out_height; /* u16.0 */
} isp_ge_usr_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  ge_ct_th1[HI_ISP_GE_MAX_CHN_NUM];     /* u14.0   0x400 */
    hi_u16  ge_ct_th3[HI_ISP_GE_MAX_CHN_NUM];     /* u14.0   0x200 */
    hi_u16  ge_strength;      /* u9.0  0x80 */
} isp_ge_dyna_cfg;

typedef struct {
    hi_bool ge_en[HI_ISP_GE_MAX_CHN_NUM];
    hi_u8   chn_num;
    isp_ge_static_cfg     static_reg_cfg;
    isp_ge_dyna_cfg       dyna_reg_cfg;
    isp_ge_usr_cfg        usr_reg_cfg;
} isp_ge_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   bit_depth_in;
    hi_u8   bit_depth_out;
    hi_u32  lut[EXPANDER_NODE_NUM];
} isp_expander_static_cfg;

typedef struct {
    hi_bool enable;
    isp_expander_static_cfg static_cfg;
} isp_expander_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_bool hcds_en;        /* UV Horizontal DS enable; 0: close; 1: open */
    hi_s16  h_coef[8];      /* h coef size 8 */
    hi_s8   v_coef[2];      /* v coef size 2 */
    hi_u16  coring_limit;
    hi_u8   midf_bldr;
} isp_mcds_static_reg_cfg;

typedef struct {
    hi_bool dyna_resh;
    hi_bool midf_en;        /* 0: Med Filter close; 1: Med Filter open */
    hi_bool vcds_en;        /* UV vertical DS enable; 0: close; 1: open */
} isp_mcds_dyna_reg_cfg;

typedef struct {
    hi_bool mcds_en;
    isp_mcds_static_reg_cfg static_reg_cfg;
    isp_mcds_dyna_reg_cfg   dyna_reg_cfg;
} isp_mcds_reg_cfg;

#define  NLUT_LENGTH       129
#define  WLUT_LENGTH       33
#define  WDR_LUT_LENGTH    128
#define  WDR_CHL_CHNLN     5

#define  WDR_FRAME0        0
#define  WDR_FRAME1        1
#define  WDR_FRAME2        2
#define  WDR_FRAME3        3

#define  WDR_CHL_Rg        0
#define  WDR_CHL_Gr        1
#define  WDR_CHL_Gb        2
#define  WDR_CHL_Bg        3
#define  WDR_CHL_MAX       2

typedef struct {
    hi_bool first_frame;
    hi_bool resh;
    hi_bool gray_scale_mode;

    hi_bool save_blc;
    hi_bool force_long_smooth_en;                    /* u1.0,[0,1] */

    hi_u8   bldr_lhf_idx;                           /* u5.0, [0,16] */
    hi_u8   mask_similar_thr;                       /* u6.0, [0,63], */
    hi_u8   mask_similar_cnt;                       /* u3.0, [0,7] */
    hi_u16  expo_value_lut[WDR_CHL_MAX];             /* u8.6, [0,16383] */
    hi_u16  flick_exp_ratio;
    hi_u16  max_ratio;                            /* u17.0, [0,131071] */
    hi_u16  dft_wgt_fl;                            /* u9.0, [0,256] */
    hi_u16  expo_r_ratio_lut[WDR_CHL_MAX - 1];        /* u10.0,[0,1023] */
    hi_u32  blc_comp_lut[WDR_CHL_MAX - 1];           /* s27.0,[0,67108863] */

    hi_u16  saturate_thr;             /* u14.0,[0,16383] */
    hi_u16  fusion_saturate_thd;       /* u12.0,[0,4095] */
} isp_fswdr_static_cfg;

typedef struct {
    hi_bool resh;
    hi_bool fusion_mode;                /* u1.0,  fusion mode */
    hi_bool short_expo_chk;              /* u1.0, [0,1] */
    hi_u8   mdt_l_bld;                  /* u8.0, [0,254],the amount of long-expo is added in motion mask */
    hi_u8   bnr_full_mdt_thr;            /* u8.0, [0,254] */

    hi_u8   mdt_still_thr;
    hi_u8   mdt_full_thr;
    hi_u32  update_index;

    hi_u8   short_sigma_gwgt;          /* U4.0  [0,15] */
    hi_u8   short_sigma_cwgt;          /* U4.0 [0,15] */

    hi_bool sfnr;                     /* u1.0 */

    hi_u16  pixel_avg_max_diff; /* u11.0,[0,4095] */

    hi_u8   short_sigma_l1_g_weight;           /* u4.4,[0.255] */
    hi_u8   short_sigma_l2_g_weight;           /* u4.4,[0.255] */
    hi_u8   short_sigma_l1_c_weight;           /* u4.4,[0.255] */
    hi_u8   short_sigma_l2_c_weight;           /* u4.4,[0.255] */

    hi_bool fusion_smooth;                 /* u1.0, [0,1] */
    hi_u32  fusion_smooth_diff_thd;        /* u16.0, [0,65535] */
} isp_fswdr_usr_cfg;

typedef struct {
    hi_bool resh;
    hi_bool wdr_mdt_en;                         /* u1.0,  motion detection switch */
    hi_u8   md_thr_low_gain;                    /* u4.2, [0,63] */
    hi_u8   md_thr_hig_gain;                    /* u4.2, [0,63] */
    hi_u8   sqrt_again_g;                       /* u3.0, [0,6] */
    hi_u8   sqrt_dgain_g;                       /* u3.0, [0,6] */
    hi_u8   mdt_nos_floor;                      /* u7.0, [0,127] */
    hi_u16  short_thr;                          /* u14.0,[0,16383],the threshold of beginning to adopt short exposure */
    hi_u16  long_thr;
    hi_u16  still_thr_lut[WDR_CHL_MAX - 1];     /* u14.0,[0,16383] */

    hi_u16  t_nos_floor;                        /* u12.0,[0,4095] */
    hi_bool erosion_en;
    hi_u8   texture_thd_wgt;                   /* u4.4,[0,255] */
    hi_u16  fusion_thr_r_lut[WDR_CHL_MAX];     /* u14.0,[0,16383] */
    hi_u16  fusion_thr_g_lut[WDR_CHL_MAX];     /* u14.0,[0,16383] */
    hi_u16  fusion_thr_b_lut[WDR_CHL_MAX];     /* u14.0,[0,16383] */

    hi_bool force_long;         /* u1.0,[0,1] */
    hi_u16  force_long_low_thr; /* u11.0,[0,4095] */
    hi_u16  force_long_hig_thr; /* u11.0,[0,4095] */
    hi_u16  short_check_thd; /* u11.0,[0,4095] */

    hi_bool bcom_en;
    hi_bool bdec_en;
    hi_u8   bcom_alpha;
    hi_u8   bdec_alpha;
    hi_u8   frm_merge;
    hi_bool fusion_data_mode;
} isp_fswdr_dyna_cfg;

typedef struct {
    hi_bool fusion_mode;
    hi_bool wdr_mdt_en;
    hi_u16  short_thr;
    hi_u16  long_thr;
    hi_u16  offset0;
} isp_fswdr_sync_cfg;

typedef struct {
    hi_bool wdr_en;
    isp_fswdr_static_cfg static_reg_cfg;
    isp_fswdr_dyna_cfg   dyna_reg_cfg;
    isp_fswdr_usr_cfg    usr_reg_cfg;
    isp_fswdr_sync_cfg   sync_reg_cfg;
} isp_wdr_reg_cfg;

typedef struct {
    hi_bool resh;
} isp_flicker_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  over_thr;         /* U10 */
    hi_u16  exp_ratio0;       /* U8.6      isp.c */
    hi_u16  exp_ratio1;       /* U8.6      isp.c */
    hi_u16  exp_ratio2;       /* U8.6      isp.c */
    hi_s16  gr_avg_pre;        /* S11       rw */
    hi_s16  gb_avg_pre;        /* S11       rw */
} isp_flicker_dyna_cfg;

typedef struct {
    hi_bool enable;
    isp_flicker_static_cfg static_reg_cfg;
    isp_flicker_dyna_cfg   dyna_reg_cfg;
} isp_flicker_reg_cfg;

typedef struct {
    hi_bool static_resh;
} isp_rc_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  center_ver_coor;
    hi_u16  center_hor_coor;
    hi_u32  square_radius;
} isp_rc_usr_cfg;

typedef struct {
    hi_bool rc_en;
    isp_rc_static_cfg static_reg_cfg;
    isp_rc_usr_cfg    usr_reg_cfg;
} isp_radial_crop_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_u8   win_num_h;
    hi_u8   win_num_v;
} isp_lsc_static_cfg;

typedef struct {
    hi_bool coef_update;
    hi_bool lut_update;
    hi_bool switch_lut2_stt_reg_new;
    hi_u8   switch_reg_new_cnt;
    hi_u8   buf_id;
    hi_u8   mesh_scale;
    hi_u16  weight;
    hi_u16  mesh_str;
    hi_u16  width_offset;
    hi_u16  delta_x[HI_ISP_LSC_GRID_COL - 1];
    hi_u16  delta_y[HI_ISP_MLSC_Y_HALF_GRID_NUM];
    hi_u16  inv_x[HI_ISP_LSC_GRID_COL - 1];
    hi_u16  inv_y[HI_ISP_MLSC_Y_HALF_GRID_NUM];
    hi_u32  r_gain[HI_ISP_LSC_GRID_POINTS];
    hi_u32  gr_gain[HI_ISP_LSC_GRID_POINTS];
    hi_u32  gb_gain[HI_ISP_LSC_GRID_POINTS];
    hi_u32  b_gain[HI_ISP_LSC_GRID_POINTS];
    hi_u32  update_index;
} isp_lsc_usr_cfg;

typedef struct {
    hi_bool lsc_en;
    hi_bool lut2_stt_en;
    isp_lsc_usr_cfg       usr_reg_cfg;
    isp_lsc_static_cfg    static_reg_cfg;
} isp_lsc_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_u16  node_num;
} isp_rlsc_static_cfg;

typedef struct {
    hi_bool usr_resh;
    hi_bool coef_update;
    hi_bool lut_update;
    hi_bool switch_lut2_stt_reg_new;
    hi_u8   switch_reg_new_cnt;
    hi_u8   buf_id;
    hi_u16  width_offset;
    hi_u16  center_rx;
    hi_u16  center_ry;
    hi_u16  center_gr_x;
    hi_u16  center_gr_y;
    hi_u16  center_gb_x;
    hi_u16  center_gb_y;
    hi_u16  center_bx;
    hi_u16  center_by;
    hi_u16  off_center_r;
    hi_u16  off_center_gr;
    hi_u16  off_center_gb;
    hi_u16  off_center_b;
    hi_u16  gain_str;
    hi_u8   gain_scale;
    hi_u16  gain_lut[ISP_BAYER_CHN_NUM][HI_ISP_RLSC_POINTS + 1];
    hi_u32  update_index;
} isp_rlsc_usr_cfg;

typedef struct {
    hi_bool rlsc_en;
    hi_bool lut2_stt_en;
    isp_rlsc_usr_cfg      usr_reg_cfg;
    isp_rlsc_static_cfg   static_reg_cfg;
} isp_rlsc_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_bool nddm_cac_blend_en;
    hi_u8   r_counter_thr;
    hi_u8   g_counter_thr;
    hi_u8   b_counter_thr;
    hi_u8   de_purple_str[LCAC_STRENGTH_NUM];
    hi_u16  satu_thr;
    hi_u16  nddm_cac_blend_rate;
} isp_local_cac_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   luma_high_cnt_thr;    /* u8.0, [0,153] */
    hi_u8   cb_cnt_high_thr;      /* u7.0, [0,68] */
    hi_u8   cb_cnt_low_thr;       /* u7.0, [0,68] */
    hi_u8   bld_avg_cur;         /* u4.0, [0, 8] */
    hi_u16  cb_thr;            /* u12.0, [0,4095] */
    hi_u16  var_thr;
    hi_s16  cb_cr_ratio_lmt_high;
    hi_u32  update_index;
} isp_local_cac_usr_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   de_purple_ctr_cr;
    hi_u8   de_purple_ctr_cb;
    hi_u16  luma_thr;          /* u12.0, [0,4095] */
    hi_u16  r_luma_thr;
    hi_u16  g_luma_thr;
    hi_u16  b_luma_thr;
    hi_s16  cb_cr_ratio_lmt_low;
} isp_local_cac_dyna_cfg;

typedef struct {
    hi_bool local_cac_en;
    isp_local_cac_usr_cfg    usr_reg_cfg;
    isp_local_cac_dyna_cfg   dyna_reg_cfg;
    isp_local_cac_static_cfg static_reg_cfg;
} isp_lcac_reg_cfg;

typedef struct {
    hi_bool static_resh;
    hi_bool gcac_ver_fil_en;
    hi_u8   gcac_blend_sel;
    hi_u8   gcac_chr_ver_mode;
    hi_u8   gcac_clip_mode_hor;
    hi_u8   gcac_clip_mode_ver;
} isp_global_cac_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   ver_norm_shift;
    hi_u8   ver_norm_factor;
    hi_u8   hor_norm_shift;
    hi_u8   hor_norm_factor;
    hi_u16  start_coor_hor;
    hi_u16  start_coor_ver;
    hi_s16  param_red_a;
    hi_s16  param_red_b;
    hi_s16  param_red_c;
    hi_s16  param_blue_a;
    hi_s16  param_blue_b;
    hi_s16  param_blue_c;
    hi_u16  center_coor_hor;
    hi_u16  center_coor_ver;
    hi_u16  cor_var_thr;
    hi_u32  update_index;
} isp_global_cac_usr_cfg;

typedef struct {
    hi_u32 reserved;
} isp_global_cac_dyna_cfg;

typedef struct {
    hi_bool global_cac_en;
    isp_global_cac_usr_cfg    usr_reg_cfg;
    isp_global_cac_dyna_cfg   dyna_reg_cfg;
    isp_global_cac_static_cfg static_reg_cfg;
} isp_gcac_reg_cfg;

typedef struct {
    hi_bool ccm_prot_en;
    hi_u32 data_cc_thd0;
    hi_u32 data_cc_thd1;
    hi_u16 cc_prot_ratio;
    hi_u32 data_rr_thd0;
    hi_u32 data_rr_thd1;
    hi_u32 data_gg_thd0;
    hi_u32 data_gg_thd1;
    hi_u32 data_bb_thd0;
    hi_u32 data_bb_thd1;
    hi_u32 max_rgb_thd;
    hi_u16 rgb_prot_ratio;
} isp_demosaic_darkregion_prot;

typedef struct {
    hi_bool resh;
    hi_bool de_sat_enable;       /* u1.0 */
    hi_bool ahd_enable;         /* u1.0 */
    hi_bool de_fake_enable;      /* u1.0 */
    hi_bool g_intp_ctrl_enable;   /* u1.0 */
    hi_u8   lpff3;            /* u8.0, */
    hi_u8   hv_blend_limit1;    /* u8.0, */
    hi_u8   hv_blend_limit2;    /* u8.0, */
    hi_u8   hv_color_ratio;     /* u8.0, */
    hi_u8   hv_selection;      /* u2.0 */
    hi_u8   g_clip_bit_sft;      /* u4.0, */
    hi_u8   dither_mask;       /* u8.0, */
    hi_u8   dither_ratio;      /* u8.0, */
    hi_u8   cx_var_max_rate;     /* u4.0, */
    hi_u8   cx_var_min_rate;     /* u4.0, */
    hi_u16  de_sat_thresh1;    /* u10.0, */
    hi_u16  de_sat_thresh2;    /* u10.0, */
    hi_u16  de_sat_hig;        /* u9.0, */
    hi_u16  de_sat_prot_sl;     /* u10.0 */
    hi_u16  ahd_part1;        /* u10.0, */
    hi_u16  ahd_part2;        /* u8.0, */
    hi_u16  gf_th_low;         /* u9.0,   gf low-fix strength */
    hi_u16  gf_th_hig;         /* u9.0,   gf high-fix strength */
    hi_u16  cb_cr_avg_thr;      /* u12.0, */
} isp_demosaic_static_cfg;

typedef struct {
    hi_bool resh;
    hi_bool update_gf;
    hi_u8   de_f_color_cr;
    hi_u8   de_f_color_cb;
    hi_u8   cc_hf_max_ratio;      /* u5.0, */
    hi_u8   cc_hf_min_ratio;      /* u5.0, */
    hi_u8   lpff0;             /* s4.0, */
    hi_u8   lpff1;             /* u5.0, */
    hi_u8   lpff2;             /* u5.0, */
    hi_u8   dither_max;         /* u8.0, */
    hi_u8   fcr_gf_gain;         /* s3.2,   fcr control */
    hi_u8   awb_gf_gain_low;      /* u3.4, */
    hi_u8   awb_gf_gain_hig;      /* u3.4, */
    hi_u8   awb_gf_gain_max;      /* u4.0, */
    hi_u8   ehc_gray;           /* u3.4, */
    hi_u16  fake_cr_var_thr_low;
    hi_u16  fake_cr_var_thr_high;
    hi_u16  de_sat_low;         /* u9.0, */
    hi_u16  de_sat_prot_th;      /* u10.0 */
    hi_u16  hf_intp_blur_th_low;  /* u10.0, */
    hi_u16  hf_intp_blur_th_hig;  /* u10.0, */
    hi_u16  fcr_det_low;        /* u12.0,  fcr det thresh */
    hi_u16  gf_blur_luts[HI_ISP_DEMOSAIC_LUT_LENGTH]; /* u12.0, */
    hi_u8   color_noise_thd_y;    /* u8.0 */
    hi_u8   color_noise_str_y;    /* u8.0 */
    isp_demosaic_darkregion_prot pro_dark_region;
} isp_demosaic_dyna_cfg;

typedef struct {
    hi_bool vhdm_enable;       /* u1.0, */
    hi_bool nddm_enable;       /* u1.0, */
    isp_demosaic_static_cfg static_reg_cfg;
    isp_demosaic_dyna_cfg   dyna_reg_cfg;
} isp_demosaic_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  fcr_limit1;      /* u12.0, */
    hi_u16  fcr_limit2;      /* u12.0, */
    hi_u16  fcr_thr;         /* u12.0, */
} isp_antifalsecolor_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   fcr_gain;         /* u5.0, */
    hi_u8   fcr_ratio;        /* u6.0, */
    hi_u8   fcr_gray_ratio;    /* u5.0, */
    hi_u8   fcr_cmax_sel;      /* u4.0, */
    hi_u8   fcr_detg_sel;      /* u4.0, */
    hi_u16  fcr_hf_thresh_low; /* u10.0,  fcr hf thresh low */
    hi_u16  fcr_hf_thresh_hig; /* u10.0,  fcr hf thresh high */
} isp_antifalsecolor_dyna_cfg;

typedef struct {
    hi_bool fcr_enable;        /* u1.0, */
    isp_antifalsecolor_static_cfg static_reg_cfg;
    isp_antifalsecolor_dyna_cfg   dyna_reg_cfg;
} isp_antifalsecolor_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_bool ca_lut_update_en; /* u1.0 */
    hi_u8   buf_id;
    hi_u16  ca_luma_thr_high; /* u10.0 */
    hi_u16  ca_luma_ratio_high; /* U1.10   high lightness thresh' ratio in Y-alpha */
    hi_u16  y_ratio_lut[HI_ISP_CA_YRATIO_LUT_LENGTH]; /* U1.10 */
    hi_u32  update_index;
} isp_ca_usr_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  ca_iso_ratio; /* 1.10bit  ISO ratio  for UV,max = 2047 FW limi */
} isp_ca_dyna_cfg;

typedef struct {
    hi_bool static_resh;
    hi_bool ca_llhc_proc_en;  /* u1.0 */
    hi_bool ca_skin_proc_en;  /* u1.0 */
    hi_bool ca_satu_adj_en;   /* u1.0 */
    hi_u16  ca_luma_thr_low;        /* u10.0 */
    hi_u16  ca_dark_chroma_thr_low;  /* u10.0 */
    hi_u16  ca_dark_chroma_thr_high; /* u10.0 */
    hi_u16  ca_s_dark_chroma_thr_low; /* u10.0 */
    hi_u16  ca_s_dark_chroma_thr_high; /* u10.0 */
    hi_u16  ca_luma_ratio_low;      /* U1.10 */

    hi_s16  ca_yuv2_rgb_coef[ISP_CSC_COEF_NUM];   /* s5.10 */
    hi_s16  ca_yuv2_rgb_in_dc[ISP_CSC_DC_NUM];   /* S11.0 */
    hi_s16  ca_yuv2_rgb_out_dc[ISP_CSC_DC_NUM];  /* S11.0 */

    hi_s16  ca_yuv2_rgb_in_dc0;      /* S11.0 */
    hi_s16  ca_yuv2_rgb_in_dc1;      /* S11.0 */
    hi_s16  ca_yuv2_rgb_in_dc2;      /* S11.0 */
    hi_s16  ca_yuv2_rgb_out_dc0;     /* S11.0 */
    hi_s16  ca_yuv2_rgb_out_dc1;     /* S11.0 */
    hi_s16  ca_yuv2_rgb_out_dc2;     /* S11.0 */
    hi_u16  ca_skin_low_luma_min_u;   /* u10 */
    hi_u16  ca_skin_low_luma_max_u;   /* u10 */
    hi_u16  ca_skin_low_luma_min_uy;  /* u10 */
    hi_u16  ca_skin_low_luma_max_uy;  /* u10 */
    hi_u16  ca_skin_high_luma_min_u;  /* u10 */
    hi_u16  ca_skin_high_luma_max_u;  /* u10 */
    hi_u16  ca_skin_high_luma_min_uy; /* u10 */
    hi_u16  ca_skin_high_luma_max_uy; /* u10 */
    hi_u16  ca_skin_low_luma_min_v;   /* u10 */
    hi_u16  ca_skin_low_luma_max_v;   /* u10 */
    hi_u16  ca_skin_low_luma_min_vy;  /* u10 */
    hi_u16  ca_skin_low_luma_max_vy;  /* u10 */
    hi_u16  ca_skin_high_luma_min_v;  /* u10 */
    hi_u16  ca_skin_high_luma_max_v;  /* u10 */
    hi_u16  ca_skin_high_luma_min_vy; /* u10 */
    hi_u16  ca_skin_high_luma_max_vy; /* u10 */
    hi_s16  ca_skin_uv_diff;        /* S11 */
    hi_u16  ca_skin_ratio_thr_low;   /* u12.0 */
    hi_u16  ca_skin_ratio_thr_mid;   /* u12.0 */
    hi_u16  ca_skin_ratio_thr_high;  /* u12.0 */
} isp_ca_static_cfg;

typedef struct {
    hi_bool ca_en;
    hi_bool lut2_stt_en;
    isp_ca_static_cfg static_reg_cfg;
    isp_ca_dyna_cfg   dyna_reg_cfg;
    isp_ca_usr_cfg    usr_reg_cfg;
} isp_ca_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   jnlm_sel;                          /* u2.0 */
    hi_u8   sad_win_size_sel;                    /* u2.0 */
    hi_u16  jnlm_max_wt_coef;                   /* u10.0 */
} isp_bayernr_static_cfg;

typedef struct {
    hi_bool resh;
    hi_bool medc_enable;                        /* u1.0 */
    hi_bool wdr_mode_en;                         /* u1.0 */
    hi_bool wdr_fusion_en;                       /* u1.0 */
    hi_bool center_wgt_en;                       /* u1.0 */
    hi_bool bnr_lut_update_en;                    /* u1.0 */
    hi_u8   wdr_map_flt_mode;                    /* u1.0 */
    hi_u8   c_ratio[ISP_BAYER_CHN_NUM];    /* u6.0 */
    hi_u8   amed_mode[ISP_BAYER_CHN_NUM];     /* u1.0 */
    hi_u8   amed_level[ISP_BAYER_CHN_NUM];    /* u2.0 */
    hi_u8   jnlm_sym_coef;                      /* u8.0 */
    hi_u8   jnlm_gain;                         /* u8.0 */
    hi_u8   wdr_map_gain;                       /* u5.0 */
    hi_u16  shot_ratio;                       /* u2.8 */
    hi_u16  jnlm_coring_hig;                   /* u16.0 */
    hi_u16  r_lmt_rgain;                       /* u10.0 */
    hi_u16  r_lmt_bgain;                       /* u10.0 */
    hi_u16  lmt_offset;                       /* u10.0 */
    hi_u32  edge_det_thr;                      /* u17.0 */
    hi_u8   g_coring_gain_lut[HI_ISP_BAYERNR_LUT_LENGTH];   /* u4.4 */
    hi_u8   jnlm_limit_lut[HI_ISP_BAYERNR_LMTLUTNUM];      /* u8.0 */
    hi_u8   jnlm_limit_s_lut[HI_ISP_BAYERNR_LMTLUTNUM];     /* u8.0 */
    hi_u16  jnlm_coring_low_lut[HI_ISP_BAYERNR_LUT_LENGTH]; /* u14.0 */
    hi_u32  jnlm_limit_mult_gain[ISP_BAYER_CHN_NUM];          /* u21.0 */
    hi_u32  jnlm_limit_mult_gain_s[ISP_BAYER_CHN_NUM];         /* u21.0 */
} isp_bayernr_dyna_cfg;

typedef struct {
    hi_bool resh;
    hi_u8 bnr_lsc_en;                          /* u8.0 */
    hi_bool bnr_mono_sensor_en;                   /* u1.0 */
    hi_u8   bnr_lsc_max_gain;                    /* u2.6 */
    hi_u16  bnr_lsc_cmp_strength;               /* u1.8 */
} isp_bayernr_usr_cfg;

typedef struct {
    hi_bool bnr_enable;                             /* u1.0 */
    isp_bayernr_static_cfg    static_reg_cfg;
    isp_bayernr_dyna_cfg      dyna_reg_cfg;
    isp_bayernr_usr_cfg       usr_reg_cfg;
} isp_bayernr_reg_cfg;

typedef struct {
    hi_bool  resh;
    hi_u8    de_pos_clip;
    hi_u8    de_neg_clip;
    hi_u16  clip_ratio;          /* u1.8  [0,256] */
    hi_u16  luma_scale_x0;        /* u1.8  [0,256] */
    hi_u16  luma_scale_x1;        /* u1.8  [0,256] */
    hi_u16  luma_scale_y0;        /* u1.8  [0,256] */
    hi_u16  luma_scale_y1;        /* u1.8  [0,256] */
    hi_u16  luma_scale_k;        /* s5.4  [0,511] */
    hi_u16  satu_gain_x0;       /* u1.8  [0,256] */
    hi_u16  satu_gain_x1;       /* u1.8  [0,256] */
    hi_u16  satu_gain_y0;       /* u1.8  [0,256] */
    hi_u16  satu_gain_y1;       /* u1.8  [0,256] */
    hi_u16  satu_gain_k;        /* s5.4  [0,511] */
    hi_u16  edge_scale_x0;        /* u1.8  [0,256] */
    hi_u16  edge_scale_x1;        /* u1.8  [0,256] */
    hi_u16  edge_scale_y0;        /* u1.8  [0,256] */
    hi_u16  edge_scale_y1;        /* u1.8  [0,256] */
    hi_u16  edge_scale_k;        /* s5.4  [0,511] */
    hi_u16  global_ratio;        /* u4.4  [0,255] */
    hi_bool sgm_mode;              /* U1.0  [0,1] */
    hi_bool edge_mode;             /* U1.0  [0,1] */
    hi_bool luma_gain_mode;         /* U1.0  [0,1] */
    hi_u8     luma_bit_mode;          /* U2.0  [0,2] */
} isp_detail_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   sgm_pos4_gain;        /* u3.2  [0,31] */
    hi_u8   sgm_pos3_gain;        /* u3.2  [0,31] */
    hi_u8   sgm_pos2_gain;        /* u3.2  [0,31] */
    hi_u8   sgm_neg4_gain;        /* u3.2  [0,31] */
    hi_u8   sgm_neg3_gain;        /* u3.2  [0,31] */
    hi_u8   sgm_neg2_gain;        /* u3.2  [0,31] */
    hi_u16  gain_lf;             /* u5.4  [0,256] */
    hi_u16  gain_hf_pos;          /* u5.4  [0,256] */
    hi_u16  gain_hf_neg;          /* u5.4  [0,256] */
} isp_detail_dyna_cfg;

typedef struct {
    hi_bool resh;
    hi_u16  luma_gain_lut[HI_ISP_DE_LUMA_GAIN_LUT_N]; /* U1.8  [0,256] */
} isp_detail_usr_cfg;

typedef struct {
    hi_bool de_enable;
    hi_bool bnr_enable;
} isp_de_reg_kernel;

typedef struct {
    hi_bool de_enable;                             /* u1.0 */
    isp_detail_static_cfg    static_reg_cfg;
    isp_detail_dyna_cfg      dyna_reg_cfg;
    isp_detail_usr_cfg       usr_reg_cfg;
    isp_de_reg_kernel        kernel_reg_cfg;
} isp_detail_reg_cfg;

typedef struct {
    hi_bool enable;
    hi_bool calc_drc_enable;
    hi_bool stat_drc_enable;
    hi_bool de_enable;
    hi_bool calc_de_ref;
    hi_bool calc_pf_lmt_enable;
    hi_bool pre_drc_lut_update;
    hi_bool static_resh;

    hi_u32  calc_luma_sel;
    hi_u32  deh_lpf_sft;
    hi_s16  deh_lpf_coef_h[LDCI_DE_LPF_H_COEF_NUM];
    hi_s16  deh_lpf_coef_v[LDCI_DE_LPF_V_COEF_NUM];
    hi_u32  calc_pfmul;
    hi_u32  calc_pf_ori[LDCI_PF_PARA_NUM];
    hi_s16  calc_pf_sft[LDCI_PF_SFT_NUM];
    hi_u32  calc_pf_thd[LDCI_PF_PARA_NUM];
    hi_u32  calc_pf_rly[LDCI_PF_PARA_NUM];
    hi_u32  global_he_wgt;
    hi_u32  lpf_sft;
    hi_u32  chr_pos_damp;
    hi_u32  chr_neg_damp;
    hi_s16  stat_poly_p2[LDCI_POLY_LUT_NUM];
    hi_s16  stat_poly_p3[LDCI_POLY_LUT_NUM];
    hi_s16  poly_p1[LDCI_POLY_LUT_NUM];
    hi_s16  poly_q0[LDCI_POLY_LUT_NUM];
    hi_s16  poly_q1[LDCI_POLY_LUT_NUM];
    hi_s16  poly_q2[LDCI_POLY_LUT_NUM];
    hi_s16  poly_q3[LDCI_POLY_LUT_NUM];
    hi_s16  stat_dyn_rng_cmp_lut[LDCI_POLY_LUT_NUM];
    hi_s16  calc_dyn_rng_cmp_lut[LDCI_POLY_LUT_NUM];
} isp_ldci_static_cfg;

typedef struct {
    hi_bool enable;
    hi_bool wrstat_en;
    hi_bool rdstat_en;
    hi_bool calc_enable;
    hi_bool pre_calc_lut_reg_new;
    hi_bool calc_lut_update;
    hi_bool lut2_stt_en;
    hi_u8   buf_id;

    hi_u32  calc_blc_ctrl;
    hi_u32  he_pos_lut[LDCI_HE_LUT_SIZE];
    hi_u32  he_neg_lut[LDCI_HE_LUT_SIZE];
    hi_u32  usm_pos_lut[LDCI_DE_USM_LUT_SIZE];
    hi_u32  usm_neg_lut[LDCI_DE_USM_LUT_SIZE];
    hi_u32  color_gain_lut[LDCI_COLOR_GAIN_LUT_SIZE];
    hi_u32  de_lut[LDCI_DE_USM_LUT_SIZE];
    hi_u32  lpf_coef[LDCI_LPF_LUT_SIZE];

    hi_u32  calc_map_offset_x;
    hi_u32  calc_sml_map_stride;
    hi_u32  calc_sml_map_height;
    hi_u32  calc_total_zone;
    hi_u32  calc_scale_x;
    hi_u32  calc_scale_y;
    hi_u32  blk_sml_map_width[LDCI_SM_WIDTH_NUM];
    hi_u32  stat_v_start;
    hi_u32  stat_v_end;
    hi_u32  stat_h_start;
    hi_u32  stat_h_end;
    hi_u32  stat_sml_map_width;
    hi_u32  stat_sml_map_height;
    hi_u32  stat_total_zone;
    isp_ldci_stat tpr_stat;
} isp_ldci_dyna_cfg;

typedef struct {
    isp_ldci_static_cfg static_reg_cfg;
    isp_ldci_dyna_cfg   dyna_reg_cfg;
} isp_ldci_reg_cfg;

typedef struct {
} isp_fpn_static_cfg;

typedef struct {
    hi_bool isp_fpn_enable;
    hi_bool isp_fpn_offline;

    hi_u32  isp_fpn_line_frame;
    hi_u32  isp_fpn_calib_corr;
    hi_u32  isp_fpn_frame_calib_shift;
    hi_u32  isp_fpn_out_shift;
    hi_u32  isp_fpn_in_shift;
    hi_u32  isp_fpn_shift;
    hi_u32  isp_fpn_update_mode;
    hi_u32  isp_fpn_max_o;
    hi_u32  isp_fpn_overflow_thr;
    hi_u32  isp_fpn_offset[HI_ISP_FPN_CHN_NUM];
    hi_u32  isp_fpn_strength[HI_ISP_FPN_CHN_NUM];
    hi_u32  isp_fpn_correct_en_id[HI_ISP_FPN_CHN_NUM];
    hi_u32  isp_fpn_height;
    hi_u32  isp_fpn_width;
    hi_u32  isp_fpn_update;
    hi_u32  isp_fpn_line_waddr;
    hi_u32  isp_fpn_line_raddr;
} isp_fpn_dyna_cfg;

typedef struct {
    isp_fpn_static_cfg static_reg_cfg;
    isp_fpn_dyna_cfg   dyna_reg_cfg;
} isp_fpn_reg_cfg;

typedef struct {
    hi_bool  resh;
    hi_u32   size;
} isp_clut_static_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   buf_id;
    hi_u32  update_index;
    hi_u32  lu_stt[HI_ISP_CLUT_LUT_LENGTH];
} isp_clut_usr_coef_cfg;

typedef struct {
    hi_bool resh;
    hi_bool enable;
    /* 0-debug closed; 1-debug open, the left screen is original video and the right screen is CLUT-processed video */
    hi_bool demo_enable;
    hi_bool demo_mode;
    hi_u32  demo_pos;
    hi_u32  gain_r; /* [0,512] */
    hi_u32  gain_g; /* [0,512] */
    hi_u32  gain_b; /* [0,512] */
} isp_clut_usr_ctrl_cfg;

typedef struct {
    isp_clut_usr_coef_cfg     clut_usr_coef_cfg;
    isp_clut_usr_ctrl_cfg     clut_usr_ctrl_cfg;
} isp_clut_usr_cfg;

typedef struct {
    hi_bool enable;
    isp_clut_static_cfg    static_reg_cfg;
    isp_clut_usr_cfg       usr_reg_cfg;
} isp_clut_reg_cfg;

typedef struct {
    hi_bool  static_resh;
} isp_gamma_static_cfg;

typedef struct {
    hi_bool resh;
    hi_bool gamma_lut_update_en;
    hi_bool switch_lut2_stt_reg_new;
    hi_bool switch_mode;
    hi_u8   switch_reg_new_cnt;
    hi_u8   buf_id;
    hi_u16  gamma_lut[GAMMA_REG_NODE_NUM];  /* u12.0 */
    hi_u8   gamma_step[GAMMA_OUTSEG_NUM];
    hi_u16  gamma_in_seg[GAMMA_OUTSEG_NUM];
    hi_u16  gamma_pos[GAMMA_OUTSEG_NUM];
    hi_u32  update_index;
} isp_gamma_usr_cfg;

typedef struct {
    hi_bool gamma_en;
    hi_bool lut2_stt_en;
    isp_gamma_static_cfg static_reg_cfg;
    isp_gamma_usr_cfg    usr_reg_cfg;
} isp_gamma_reg_cfg;

typedef struct {
    /* multiplier coefficients */
    hi_bool resh;
    hi_s16  csc_coef[ISP_CSC_COEF_NUM];  /* s5.10 */
    /* DC components */
    hi_s16  csc_in_dc[ISP_CSC_DC_NUM];  /* s11.0 */
    hi_s16  csc_out_dc[ISP_CSC_DC_NUM]; /* s11.0 */
} isp_csc_dyna_cfg;

typedef struct {
    hi_bool enable;       /* u1.0 */
    isp_csc_dyna_cfg    dyna_reg_cfg;
} isp_csc_reg_cfg;

typedef struct {
    hi_bool blc_in;
    hi_bool blc_out;
} isp_blc_static_cfg;

typedef struct {
    hi_u16 blc[ISP_BAYER_CHN_NUM];
    hi_u16 out_blc;
} isp_blc_dyna_cfg;

typedef struct {
    hi_bool resh_static;
    isp_blc_static_cfg lsc_blc;
    isp_blc_static_cfg dg_blc;
    isp_blc_static_cfg wdr_blc[ISP_WDR_CHN_MAX];
    isp_blc_static_cfg expander_blc;
    isp_blc_static_cfg wb_blc;
    isp_blc_static_cfg wdr_dg_blc[ISP_WDR_CHN_MAX];
    isp_blc_static_cfg ae_blc;
    isp_blc_static_cfg mg_blc;
    isp_blc_static_cfg flicker_blc[ISP_WDR_CHN_MAX];
    isp_blc_static_cfg de_blc;
    isp_blc_static_cfg bnr_blc;
} isp_be_blc_static_cfg;

typedef struct {
    hi_bool resh_dyna;
    isp_blc_dyna_cfg lsc_blc;
    isp_blc_dyna_cfg dg_blc;
    isp_blc_dyna_cfg wdr_blc[ISP_WDR_CHN_MAX];
    isp_blc_dyna_cfg expander_blc;
    isp_blc_dyna_cfg wb_blc;
    isp_blc_dyna_cfg wdr_dg_blc[ISP_WDR_CHN_MAX];
    isp_blc_dyna_cfg ae_blc;
    isp_blc_dyna_cfg mg_blc;
    isp_blc_dyna_cfg flicker_blc[ISP_WDR_CHN_MAX];
    isp_blc_dyna_cfg de_blc;
    isp_blc_dyna_cfg bnr_blc;
} isp_be_blc_dyna_cfg;

typedef struct {
    hi_bool resh_dyna_init;
    isp_be_blc_static_cfg static_blc;
    isp_be_blc_dyna_cfg   dyna_blc;
} isp_be_blc_cfg;

typedef struct {
    hi_bool resh_static;
    /* FE module */
    isp_blc_static_cfg fe_dg_blc;
    isp_blc_static_cfg fe_wb_blc;
    isp_blc_static_cfg fe_ae_blc;
    isp_blc_static_cfg fe_blc;
} isp_fe_blc_static_cfg;

typedef struct {
    hi_bool resh_dyna;
    /* FE module */
    isp_blc_dyna_cfg fe_dg_blc;
    isp_blc_dyna_cfg fe_wb_blc;
    isp_blc_dyna_cfg fe_ae_blc;
    isp_blc_dyna_cfg fe_blc;
} isp_fe_blc_dyna_cfg;

typedef struct {
    hi_bool resh_dyna_init;
    isp_fe_blc_static_cfg static_blc;
    isp_fe_blc_dyna_cfg   dyna_blc;
} isp_fe_blc_cfg;

typedef struct {
    hi_bool ae1_lut_update;
} isp_fe_lut_update_reg_cfg;

typedef struct {
    hi_bool ae_lut_update;
    hi_bool sharpen_lut_update;
    hi_bool drc_tm_lut_update;
    hi_bool lcac_lut_update;
    hi_bool nddm_gf_lut_update;
    hi_bool ldci_drc_lut_update;
    hi_bool ldci_calc_lut_update;
    hi_bool lsc_lut_update;
    hi_bool bnr_lut_update;
    hi_bool de_lut_update;
} isp_be_lut_update_reg_cfg;

typedef struct {
    hi_bool gamma_stt2lut_regnew;
    hi_bool ca_stt2lut_regnew;
    hi_bool clut_stt2lut_regnew;
    hi_bool bnr_lsc_stt2lut_regnew;
    hi_bool lsc_stt2lut_regnew;
    hi_bool dpc_stt2lut_regnew;
    hi_bool pre_gamma_stt2lut_regnew;

    /* pingpong */
    hi_bool dehaze_stt2lut_regnew;    /* every frame */
    hi_bool ldci_stt2lut_regnew;      /* every frame */
    hi_bool sharpen_stt2lut_regnew;   /* every frame */
} isp_be_stt2lut_regnew_reg_cfg;

typedef struct {
    hi_u16  air_r;
    hi_u16  air_g;
    hi_u16  air_b;
    hi_u8   strength;
} isp_dehaze_lut2_stt_sync_reg_cfg;

typedef struct {
    hi_bool resh;
    hi_u8   mesh_scale;
} isp_lsc_lut2stt_sync_reg_cfg;

typedef struct {
    hi_bool        resh;
    hi_u32          gain_r;
    hi_u32          gain_g;
    hi_u32          gain_b;
} isp_clut_lut2stt_sync_reg_cfg;

typedef struct {
    hi_u32    calc_blc_ctrl;
} isp_ldci_lut2stt_sync_reg_cfg;

typedef struct {
    isp_sharpen_mpi_dyna_reg_cfg          mpi_dyna_reg_cfg;
    isp_sharpen_default_dyna_reg_cfg      default_dyna_reg_cfg;
} isp_sharpen_lut2stt_sync_reg_cfg;

typedef struct {
    hi_u16  ca_luma_thr_high;
    hi_u16  ca_luma_ratio_high;
    hi_u16  ca_iso_ratio;
} isp_ca_lut2stt_sync_reg_cfg;

typedef struct {
    hi_bool nry0_en;
    hi_bool ae_en;
    hi_bool la_en;
    hi_bool awb_en;
    hi_bool wb_en;
    hi_bool ccm_en;
    hi_bool ccm_color_tone_en;
    hi_bool af_en;
    hi_bool sharpen_en;
    hi_bool edge_mark_en;
    hi_bool hlc_en;
    hi_bool vhdm_en;
    hi_bool nddm_en;
    hi_bool ldci_en;
    hi_bool lcac_en;
    hi_bool gcac_en;
    hi_bool fcr_en;
    hi_bool dpc_en[ISP_DPC_MAX_CHN_NUM];
    hi_bool ge_en[HI_ISP_GE_MAX_CHN_NUM];
    hi_bool lsc_en;
    hi_bool gamma_en;
    hi_bool csc_en;
    hi_bool ca_en;
    hi_bool wdr_en;
    hi_bool drc_en;
    hi_bool dehaze_en;
    hi_bool bnr_en;
    hi_u8   bnr_lsc_en;
    hi_bool dg_en;
    hi_bool four_dg_en;
    hi_bool pregamma_en;
    hi_bool clut_en;
    hi_bool wdr_split_en;
    hi_bool de_en;
    hi_bool expander_en;

    isp_dehaze_lut2_stt_sync_reg_cfg dehaze_sync_cfg;
    isp_lsc_lut2stt_sync_reg_cfg    lsc_sync_cfg;
    isp_clut_lut2stt_sync_reg_cfg   clut_sync_cfg;
    isp_ldci_lut2stt_sync_reg_cfg   ldci_sync_cfg;
    isp_sharpen_lut2stt_sync_reg_cfg  sharpen_sync_cfg;
    isp_ca_lut2stt_sync_reg_cfg ca_sync_cfg;
} isp_lut2stt_sync_reg_cfg;

typedef union {
    hi_u32  key;
    struct {
        hi_u32  bit1_awb_cfg     : 1 ;   /* [0] */
        hi_u32 bit1_be_af_cfg : 1; /* [1] */
        hi_u32 bit30_rsv : 30; /* [2:31] */
    };
} isp_reg_kernel_cfg_key;

typedef  struct {
    hi_u32 colum;
    hi_u32 row;
    hi_u32 zone_bin;
} awb_zone_cfg;

typedef struct {
    hi_u32 colum;
    hi_u32 row;
} af_zone_cfg;

typedef struct {
    awb_zone_cfg              be_awb_zone_cfg;
    af_zone_cfg               be_af_zone_cfg;
} isp_reg_kernel_cfg;

typedef struct {
    isp_reg_kernel_cfg_key    cfg_key;
    isp_reg_kernel_cfg        alg_kernel_cfg[ISP_STRIPING_MAX_NUM];
} isp_kernel_reg_cfg;

typedef union {
    hi_u64  key;
    struct {
        hi_u64  bit1_ae_cfg1      : 1 ;   /* [0] */
        hi_u64  bit1_ae_cfg2      : 1 ;   /* [1] */
        hi_u64  bit1_awb_dyn_cfg   : 1 ;   /* [2] */
        hi_u64  bit1_awb_cfg      : 1 ;   /* [3] */
        hi_u64  bit1_af_fe_cfg  : 1;    /* [4] */
        hi_u64  bit1_drc_cfg    : 1 ;   /* [5] */
        hi_u64  bit1_dp_cfg     : 1 ;   /* [6] */
        hi_u64  bit1_sharpen_cfg  : 1 ;   /* [7] */
        hi_u64  bit1_ge_cfg       : 1 ;   /* [8] */
        hi_u64  bit1_wdr_cfg      : 1 ;   /* [9] */
        hi_u64  bit1_dehaze_cfg   : 1 ;   /* [10] */
        hi_u64  bit1_awb_cfg3     : 1 ;   /* [11] */
        hi_u64  bit1_lsc_cfg      : 1 ;   /* [12] */
        hi_u64  bit1_dem_cfg      : 1 ;   /* [13] */
        hi_u64  bit1_fcr_cfg      : 1 ;   /* [14] */
        hi_u64  bit1_mcds_cfg     : 1 ;   /* [15] */
        hi_u64  bit1_rc_cfg       : 1 ;   /* [16] */
        hi_u64  bit1_fs_wdr_cfg : 1 ;   /* [17] */
        hi_u64  bit1_bayernr_cfg  : 1 ;   /* [18] */
        hi_u64  bit1_flicker_cfg  : 1 ;   /* [19] */
        hi_u64  bit1_ca_cfg     : 1 ;   /* [20] */
        hi_u64  bit1_nddm_cfg     : 1 ;   /* [21] */
        hi_u64  bit1_local_cac_cfg : 1 ;   /* [22] */
        hi_u64  bit1_global_cac_cfg: 1 ;   /* [23] */
        hi_u64  bit1_dci_cfg      : 1 ;   /* [24] */
        hi_u64  bit1_ldci_cfg   : 1 ;   /* [25] */
        hi_u64  bit1_csc_cfg    : 1 ;   /* [26] */
        hi_u64  bit1_gamma_cfg    : 1 ;   /* [27] */
        hi_u64  bit1_pregamma_cfg : 1 ;   /* [28] */
        hi_u64  bit1_split_cfg    : 1 ;   /* [29] */
        hi_u64  bit1_edge_mark_cfg : 1;    /* [30] */
        hi_u64  bit1_hlc_cfg      : 1;    /* [31] */
        hi_u64  bit1_hrs_cfg      : 1 ;   /* [32] */
        hi_u64  bit1_dg_cfg       : 1 ;   /* [33] */
        hi_u64  bit1_wdr_dg_cfg    : 1 ;   /* [34] */
        hi_u64  bit1_fe_dg_cfg     : 1 ;   /* [35] */
        hi_u64  bit1_af_be_cfg  : 1;    /* [36] */
        hi_u64  bit1_fe_blc_cfg : 1 ;   /* [37] */
        hi_u64  bit1_be_blc_cfg : 1 ;   /* [38] */
        hi_u64  bit1_clut_cfg   : 1 ;   /* [39] */
        hi_u64  bit1_rlsc_cfg     : 1 ;   /* [40] */
        hi_u64  bit1_fpn_cfg      : 1 ;   /* [41] */
        hi_u64  bit1_detail_cfg   : 1 ;   /* [42] */
        hi_u64  bit1_expander_cfg : 1 ;   /* [43] */
        hi_u64  bit20_rsv        : 20;   /* [44:63] */
    };
} isp_reg_cfg_key;

typedef struct {
    isp_ae_reg_cfg_2             ae_reg_cfg2;
    isp_ae_reg_cfg               ae_reg_cfg;
    isp_mg_reg_cfg               mg_reg_cfg;
    isp_dg_reg_cfg               dg_reg_cfg;
    isp_4dg_reg_cfg              four_dg_reg_cfg;
    isp_fe_dg_reg_cfg            fe_dg_reg_cfg;
    isp_awb_reg_cfg              awb_reg_cfg;

    isp_af_reg_cfg               be_af_reg_cfg;
    isp_dpc_reg_cfg              dp_reg_cfg;
    isp_drc_reg_cfg              drc_reg_cfg;
    isp_pregamma_reg_cfg         pregamma_reg_cfg;
    isp_ge_reg_cfg               ge_reg_cfg;
    isp_sharpen_reg_cfg          sharpen_reg_cfg;
    isp_edgemark_reg_cfg         edge_mark_reg_cfg;
    isp_hlc_reg_cfg              hlc_reg_cfg;
    isp_wdr_reg_cfg              wdr_reg_cfg;
    isp_dehaze_reg_cfg           dehaze_reg_cfg;
    isp_mcds_reg_cfg             mcds_reg_cfg;
    isp_lsc_reg_cfg              lsc_reg_cfg;
    isp_rlsc_reg_cfg             rlsc_reg_cfg;
    isp_demosaic_reg_cfg         dem_reg_cfg;
    isp_antifalsecolor_reg_cfg   anti_false_color_reg_cfg;
    isp_bayernr_reg_cfg          bnr_reg_cfg;
    isp_flicker_reg_cfg          flicker_reg_cfg;
    isp_expander_reg_cfg         expander_cfg;
    isp_ca_reg_cfg               ca_reg_cfg;
    isp_lcac_reg_cfg             lcac_reg_cfg;
    isp_gcac_reg_cfg             gcac_reg_cfg;
    isp_ldci_reg_cfg             ldci_reg_cfg;
    isp_fpn_reg_cfg              fpn_reg_cfg;
    isp_be_blc_cfg               be_blc_cfg;
    isp_fe_blc_cfg               fe_blc_cfg;
    isp_csc_reg_cfg              csc_cfg;
    isp_gamma_reg_cfg            gamma_cfg;
    isp_clut_reg_cfg             clut_cfg;
    isp_detail_reg_cfg           de_reg_cfg;
    isp_fe_lut_update_reg_cfg    fe_lut_update_cfg;
    isp_be_lut_update_reg_cfg    be_lut_update_cfg;
    isp_lut2stt_sync_reg_cfg     lut2stt_sync_cfg[ISP_LUT2STT_SYNC_BUF_MAX_NUM];
    isp_be_stt2lut_regnew_reg_cfg stt2lut_regnew_cfg;
} isp_alg_reg_cfg;

typedef struct {
    hi_u8 cfg_num;
    isp_reg_cfg_key       cfg_key;
    isp_alg_reg_cfg       alg_reg_cfg[ISP_STRIPING_MAX_NUM];
    isp_kernel_reg_cfg    kernel_reg_cfg;
} isp_reg_cfg;

typedef struct {
    S_ISPBE_REGS_TYPE       be_reg_cfg[ISP_STRIPING_MAX_NUM];
} isp_be_wo_reg_cfg;

typedef struct {
    S_ISP_ONLINE_STT_REGS_TYPE be_online_stt_reg[ISP_MAX_BE_NUM];
} isp_be_rw_online_stt_reg;

typedef struct {
    isp_dpc_reg_cfg       dp_reg_cfg;
} isp_ro_reg_cfg;

typedef struct {
    hi_u64  phy_addr;
    hi_u64  size;
    hi_void ATTRIBUTE *vir_addr;
} isp_be_wo_cfg_buf;

typedef struct {
    hi_bool             init;
    hi_s32              use_cnt;
    isp_be_wo_cfg_buf   be_buf_haddr;
} isp_be_cfg_buf_info;

/* offline mode isp sync para */
#define DRC_DENOM_LOG_EXP_NUM           16
#define SYNC_DRC_PRELUMA_NUM            8
#define SYNC_WDR_EXP_VAL_NUM            2
typedef struct {
    hi_u32  ldci_comp;
    hi_u8   drc_shp_log;
    hi_u32  drc_div_denom_log;
    hi_u32  drc_denom_exp;
    hi_u32  drc_prev_luma[SYNC_DRC_PRELUMA_NUM];
    hi_u32  isp_dgain[ISP_BAYER_CHN_NUM];
    hi_u32  wdr_gain[SYNC_WDR_EXP_VAL_NUM];
    hi_u32  wdr_exp_ratio;
    hi_u32  flick_exp_ratio;
    hi_u32  wdr_exp_val[SYNC_WDR_EXP_VAL_NUM];
    hi_u32  wdr_blc_comp;
    hi_bool wdr_mdt_en;
    hi_bool fusion_mode;
    hi_u16  short_thr;
    hi_u16  long_thr;
    hi_u32  wdr_max_ratio;
    hi_u32  wb_gain[ISP_BAYER_CHN_NUM];
    hi_u64  ldci_write_stt_addr[ISP_STRIPING_MAX_NUM];
    hi_u64  ldci_read_stt_addr[ISP_STRIPING_MAX_NUM];
    isp_be_blc_dyna_cfg be_blc;
} isp_be_sync_para;

typedef struct {
    hi_u8   block_num;
    hi_u8   block_dev;
} isp_be_regs_attr;

typedef struct {
    hi_u64  phy_addr;
    hi_u64  size;
    hi_void ATTRIBUTE *vir_addr;
} isp_mmz_buf_ex;

#define PING_PONG_NUM    2

typedef struct {
    isp_mmz_buf_ex lut_stt_buf[PING_PONG_NUM];   /* ping pong be lut stt info */
} isp_be_lut2stt_buf;

typedef struct {
    hi_bool            init;
    isp_be_lut2stt_buf be_lut_stt_buf[ISP_MAX_BE_NUM];
} isp_be_lut2stt_attr;

typedef struct {
    hi_bool init;
    hi_bool get_first_stat_info_flag;
    hi_u32  cur_write_flag;
    isp_mmz_buf_ex be_stt_buf[PING_PONG_NUM];   /* ping pong be online stt info */
} isp_be_online_stt_info;

typedef struct {
    hi_bool first_frame;
    hi_u32  cur_write_flag;
    isp_mmz_buf_ex fe_stt_buf[PING_PONG_NUM];   /* ping pong fe stt info */
} isp_fe_stt_info;

typedef struct {
    hi_bool         init;
    isp_fe_stt_info fe_stt[ISP_WDR_CHN_MAX];
} isp_fe_stt_attr;

typedef struct {
    hi_bool                init;
    S_ISP_FE_STT_REGS_TYPE *save_stt_stat[ISP_MAX_PIPE_NUM][ISP_WDR_CHN_MAX];
    isp_fe_stt_info        fe_stt[ISP_MAX_PIPE_NUM][ISP_WDR_CHN_MAX];
} isp_fe_stit_stt_attr;

typedef struct {
    hi_bool         init;
    isp_mmz_buf_ex  be_stt_buf[ISP_STRIPING_MAX_NUM];
} isp_be_off_stt_attr;

typedef struct {
    hi_bool         init;
    isp_mmz_buf_ex  be_stitch_buf[ISP_MAX_PIPE_NUM];
} isp_be_stit_stt_attr;

typedef struct {
    hi_bool         init;
    hi_u8           buf_idx;
    hi_u8           buf_num;
    isp_mmz_buf_ex  ldci_buf[ISP_BE_BUF_NUM_MAX];
} isp_ldci_buf;

typedef struct {
    hi_u64 cur_write_stt_addr[ISP_STRIPING_MAX_NUM];
    hi_u64 read_stt_addr[ISP_STRIPING_MAX_NUM];
    isp_ldci_stat ldci_stat;
} isp_ldci_stt_addr;

typedef struct {
    hi_u8  buf_num;
    hi_u8  buf_idx;
    hi_u64 head_phy_addr;
    isp_mmz_buf_ex read_buf[ISP_BE_BUF_NUM_MAX];
} isp_ldci_read_stt_buf;

typedef struct {
    hi_bool         init;
    isp_mmz_buf_ex  clut_buf;
} isp_clut_buf;

typedef struct {
    hi_bool         init;
    isp_mmz_buf_ex  spec_awb_buf;
} isp_spec_awb_buf;

#define ISP_SYNC_BUF_NODE_NUM           8
#define CFG2VLD_DLY_LIMIT               8
#define WDR_EXP_RATIO_SHIFT             6
#define DRC_COMP_SHIFT                  12

#define ISP_MAX_UPDATEINFO_BUF_NUM      (CFG2VLD_DLY_LIMIT)
#define ISP_MAX_FRAMEINFO_BUF_NUM       (CFG2VLD_DLY_LIMIT)
#define ISP_SAVEINFO_MAX                (CFG2VLD_DLY_LIMIT)
#define ISP_INTERRUPTS_SAVEINFO_MAX     (ISP_MAX_PHY_PIPE_NUM * 4)

typedef struct {
    hi_bool valid;

    hi_isp_sns_regs_info sns_regs_info;
    isp_ae_reg_cfg_2     ae_reg_cfg;
    isp_fswdr_sync_cfg   wdr_reg_cfg;
    isp_drc_sync_cfg     drc_reg_cfg;
    isp_de_reg_kernel    de_reg_cfg;
    isp_awb_reg_cfg_2    awb_reg_cfg;
    isp_fe_blc_dyna_cfg  fe_blc_reg_cfg;
    isp_be_blc_dyna_cfg  be_blc_reg_cfg;
} isp_sync_cfg_buf_node;

typedef struct {
    hi_bool init;
    isp_reg_cfg reg_cfg;
    isp_sync_cfg_buf_node sync_cfg_node;
} isp_reg_cfg_attr;

#define ISP_CHN_SWITCH_NUM 5
typedef struct {
    hi_u32  channel_sel;
    hi_u32  wdr_chn_sel[ISP_CHN_SWITCH_NUM];
} isp_chn_switch;

typedef struct {
    hi_u8 wdr_mode;
    hi_u32 exp_ratio[EXP_RATIO_NUM];
    isp_fswdr_sync_cfg wdr_reg_cfg;
} isp_wdr_cfg;

typedef struct {
    hi_u16   width;
    hi_u16   height;
    hi_float fps;
    hi_u8    sns_mode;
} isp_res_switch_mode;

typedef struct {
    hi_bool init;
    hi_u32  size;
    hi_u64  phy_addr;
    hi_void *virt_addr;
} isp_proc_mem;

typedef struct {
    hi_u64  phy_addr;
    hi_void *vir_addr;
} isp_mmz_buf;

typedef struct {
    hi_bool       init;
    isp_mmz_buf dng_info;
    isp_mmz_buf atta_info;
    isp_mmz_buf color_gammut_info;
    isp_mmz_buf frame_info;
    isp_mmz_buf update_info;
} isp_trans_info_buf;

typedef struct {
    hi_bool       init;
    isp_mmz_buf pro_nr_param_info;
    isp_mmz_buf pro_shp_param_info;
} isp_pro_info_buf;

typedef struct {
    hi_operation_mode operation_mode;
    hi_u8 pro_frame_num;
    hi_snap_pro_auto_param   auto_param;
    hi_snap_pro_manual_param manual_param;
} isp_pro_param;

/*
 * 0 = default mode:no snap
 * 1 = only preview
 * 2 = only picture
 * 3 = preview and picture is the same pipe
 */
typedef enum {
    ISP_SNAP_NONE            = 0,
    ISP_SNAP_PREVIEW,
    ISP_SNAP_PICTURE,
    ISP_SNAP_PREVIEW_PICTURE,
    ISP_SNAP_BUTT
} isp_snap_pipe_mode;

typedef struct {
    hi_snap_type snap_type;
    hi_s32 preview_pipe_id; /* -1,none snap */
    hi_s32 picture_pipe_id; /* -1,none snap */
    isp_pro_param pro_param;
    hi_bool load_ccm;    /* HI_TRUE:use CCM of snap_isp_info, HI_FALSE: algorithm calculate */
} isp_snap_attr;

typedef struct {
    hi_bool enable;
    hi_u32  param_num;
    hi_isp_nr_auto_attr nr_attr[PRO_MAX_FRAME_NUM];
} isp_pro_nr_param;

typedef struct {
    hi_bool enable;
    hi_u32  param_num;
    hi_isp_sharpen_auto_attr shp_attr[PRO_MAX_FRAME_NUM];
} isp_pro_shp_param;

typedef struct {
    hi_bool    pq_ai_en;
    hi_vi_pipe ai_pipe_id;
    hi_vi_pipe base_pipe_id;
} pq_ai_attr;

#define ISP_1ST_INT                     0x1
#define ISP_2ND_INT                     (1 << 4)
#define ISP_UNDEF_INT                   0xF4
#define ISP_BE_FSTART                   (1 << 16)

#define ISP_DEV_SET_FD                  _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, hi_s32)
#define ISP_GET_FRAME_EDGE              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, hi_u32)
#define ISP_GET_VD_TIMEOUT              _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, isp_vd_timeout)
#define ISP_GET_VD_END_TIMEOUT          _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_END_TIMEOUT, isp_vd_timeout)
#define ISP_GET_VD_BEEND_TIMEOUT        _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_BEEND_TIMEOUT, isp_vd_timeout)
#define ISP_SET_INT_ENABLE              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, hi_bool)
#define ISP_GET_VERSION                 _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, isp_version)
#define ISP_STAT_BUF_INIT               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, hi_u64)
#define ISP_STAT_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, isp_stat_info)
#define ISP_STAT_BUF_PUT                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, isp_stat_info)
#define ISP_STAT_ACT_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_ACT_GET, isp_stat_info)
#define ISP_REG_CFG_INIT                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, isp_reg_cfg)
#define ISP_REG_CFG_SET                 _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, isp_kernel_reg_cfg)

#define ISP_BE_CFG_BUF_INIT             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_INIT, hi_u64)
#define ISP_GET_BE_BUF_FIRST            _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_BE_BUF_FIRST, hi_u64)
#define ISP_BE_FREE_BUF_GET             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_FREE_BUF_GET, isp_be_wo_cfg_buf)
#define ISP_BE_LAST_BUF_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_LAST_BUF_GET, hi_u64)
#define ISP_BE_CFG_BUF_EXIT             _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_EXIT)
#define ISP_BE_CFG_BUF_CTL              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_CTL, isp_be_wo_cfg_buf)
#define ISP_BE_CFG_BUF_RUNNING          _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_RUNNING)
#define ISP_BE_All_BUF_INIT             _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_All_BUF_INIT)
#define ISP_BE_SWITCH_FINISH_STATE_SET  _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET)

#define ISP_MEM_INFO_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, hi_bool)
#define ISP_MEM_INFO_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, hi_bool)
#define ISP_P2EN_INFO_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_P2EN_INFO_GET, hi_bool)
#define ISP_BE_BUF_NUM_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_BUF_NUM_GET, hi_u8)
#define ISP_INIT_INFO_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_INIT_INFO_SET, hi_bool)
#define ISP_SYNC_INIT_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_INIT_SET, hi_bool)
#define ISP_RUN_STATE_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RUN_STATE_SET, hi_u64)
#define ISP_RESET_CTX                   _IO(IOC_TYPE_ISP, IOC_NR_ISP_RESET_CTX)
#define ISP_CONFIG_INFO_SET             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CONFIG_INFO_SET, hi_isp_config_info)
#define ISP_SNAP_INFO_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_INFO_GET, isp_snap_info)
#define ISP_PRO_TRIGGER_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_EN_GET, hi_bool)
#define ISP_UPDATE_POS_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_POS_GET, hi_u32)
#define ISP_FRAME_CNT_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_CNT_GET, hi_u32)
#define ISP_SNAP_ATTR_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_ATTR_GET, isp_snap_attr)

#define ISP_PROC_INIT                   _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, isp_proc_mem)
#define ISP_PROC_WRITE_ING              _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK               _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT                   _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, hi_u32)
#define ISP_TRANS_BUF_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_INIT, isp_trans_info_buf)
#define ISP_TRANS_BUF_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_EXIT)
#define ISP_PRO_BUF_INIT                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_BUF_INIT, isp_pro_info_buf)
#define ISP_PRO_BUF_EXIT                _IO(IOC_TYPE_ISP,  IOC_NR_ISP_PRO_BUF_EXIT)

#define ISP_UPDATE_INFO_SET             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_INFO_SET, hi_isp_dcf_update_info)
#define ISP_FRAME_INFO_SET              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_SET, hi_isp_frame_info)
#define ISP_FRAME_INFO_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_GET, hi_isp_frame_info)
#define ISP_DNG_INFO_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_SET, hi_dng_image_dynamic_info)
#define ISP_DNG_INFO_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_GET, hi_isp_dng_image_static_info)
#define ISP_SYNC_CFG_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, isp_sync_cfg_buf_node)
#define ISP_WDR_CFG_SET                 _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, isp_wdr_cfg)
#define ISP_BE_SYNC_PARAM_INIT          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_SYNC_PARAM_INIT, isp_be_sync_para)
#define ISP_RES_SWITCH_SET              _IO(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET)
#define ISP_CHN_SELECT_CFG              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CHN_SELECT_CFG, hi_u32)
#define ISP_AWB_EN_SET                  _IOW(IOC_TYPE_ISP, IOC_NR_ISP_AWB_EN_SET, hi_bool)

#define ISP_PWM_NUM_GET                 _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, hi_u32)
#define ISP_WORK_MODE_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_INIT, isp_block_attr)
#define ISP_WORK_MODE_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_GET, isp_working_mode)
#define ISP_WORK_MODE_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_EXIT)
#define ISP_SET_MOD_PARAM               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, hi_isp_mod_param)
#define ISP_GET_MOD_PARAM               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, hi_isp_mod_param)
#define ISP_SET_CTRL_PARAM              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_CTRL_PARAM, hi_isp_ctrl_param)
#define ISP_GET_CTRL_PARAM              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_CTRL_PARAM, hi_isp_ctrl_param)
#define ISP_PRE_BLK_NUM_UPDATE          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PRE_BLK_NUM_UPDATE, hi_u8)

#define ISP_GET_HDR_ATTR                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_HDR_ATTR, vi_pipe_hdr_attr)
#define ISP_GET_WDR_ATTR                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_WDR_ATTR, vi_pipe_wdr_attr)
#define ISP_GET_STITCH_ATTR             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_STITCH_ATTR, vi_stitch_attr)
#define ISP_GET_PIPE_SIZE               _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_PIPE_SIZE, hi_size)
#define ISP_PUB_ATTR_INFO               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PUB_ATTR_INFO, hi_isp_pub_attr)
#define ISP_KERNEL_RUNONCE              _IO(IOC_TYPE_ISP, IOC_NR_ISP_KERNEL_RUNONCE)
#define ISP_OPT_RUNONCE_INFO            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_OPT_RUNONCE_INFO, hi_bool)
#define ISP_SET_PROCALCDONE             _IO(IOC_TYPE_ISP, IOC_NR_ISP_SET_PROCALCDONE)
#define ISP_YUV_RUNONCE_INFO            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_YUV_RUNONCE_INFO, hi_bool)
#define ISP_KERNEL_YUV_RUNONCE          _IO(IOC_TYPE_ISP,  IOC_NR_ISP_KERNEL_YUV_RUNONCE)
#define ISP_SYNC_STITCH_PARAM_INIT      _IO(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_STITCH_PARAM_INIT)
#define ISP_CLUT_BUF_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_INIT)
#define ISP_CLUT_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_EXIT)
#define ISP_CLUT_BUF_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_GET, hi_u64)
#define ISP_LDCI_BUF_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_INIT)
#define ISP_LDCI_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_EXIT)
#define ISP_LDCI_READ_STT_BUF_GET       _IOR(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_READ_STT_BUF_GET, isp_ldci_read_stt_buf)

#define ISP_STT_BUF_INIT                _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_INIT)
#define ISP_STT_BUF_EXIT                _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_EXIT)
#define ISP_STT_ADDR_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_ADDR_INIT)
#define ISP_BE_LUT_STT_BUF_GET          _IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_LUT_STT_BUF_GET, hi_u64)

#define ISP_SPECAWB_BUF_INIT            _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_INIT)
#define ISP_SPECAWB_BUF_EXIT            _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_EXIT)
#define ISP_SPECAWB_BUF_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_GET, hi_u64)
#define ISP_SET_RGBIR_FORMAT            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_RGBIR_FORMAT, hi_isp_ir_bayer_format)

#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
#define ISP_PQ_AI_GROUP_ATTR_SET    _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PQ_AI_GROUP_ATTR_SET, pq_ai_attr)
#define ISP_PQ_AI_GROUP_ATTR_GET    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PQ_AI_GROUP_ATTR_GET, pq_ai_attr)
#define ISP_PQ_AI_POST_NR_ATTR_SET  _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PQ_AI_POST_NR_ATTR_SET, hi_pq_ai_noiseness_post_attr)
#define ISP_PQ_AI_POST_NR_ATTR_GET  _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PQ_AI_POST_NR_ATTR_GET, hi_pq_ai_noiseness_post_attr)
#endif

#define isp_get_dev(f)             (*((hi_u32*)(f)))

#define isp_check_pipe_return(pipe)                                   \
    do {                                                       \
        if (((pipe) < 0) || ((pipe) >= ISP_MAX_PIPE_NUM)) {    \
            isp_err_trace("Err isp pipe %d!\n", pipe); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                   \
        }                                                      \
    } while (0)

#define isp_check_vir_pipe_return(pipe)\
    do {\
        if (((pipe) < ISP_MAX_PIPE_NUM) && ((pipe) >= ISP_MAX_PHY_PIPE_NUM)) { \
            return HI_SUCCESS;                                                 \
        }                                                                      \
    } while (0)

#define isp_check_pointer_return(ptr)\
    do {\
        if ((ptr) == HI_NULL) {\
            isp_err_trace("Null Pointer!\n");\
            return HI_ERR_ISP_NULL_PTR;\
        }\
    } while (0)

#define isp_check_pointer_void_return(ptr)\
    do {\
        if ((ptr) == HI_NULL) {\
            isp_err_trace("Null Pointer!\n");\
            return;\
        }\
    } while (0)

#define isp_check_bool_return(bool)\
    do {\
        if (((bool) != HI_TRUE) && ((bool) != HI_FALSE)) {\
            isp_err_trace("Invalid ISP Bool Type %d!\n", bool);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    } while (0)

#define isp_check_buf_init_return(pipe, init)\
    do{\
        if ((init) != HI_FALSE) {\
            isp_err_trace("ISP[%d] buffer is initialized, Please exit ISP!!!\n", pipe);\
            return HI_FAILURE;\
        }\
    } while (0)

#define isp_check_buf_exit_return(pipe, init)\
    do{\
        if ((init) != HI_TRUE) {\
            return HI_SUCCESS;\
        }\
    } while (0)

#define isp_check_bebuf_init_return(pipe, init)\
    do{\
        if ((init) != HI_TRUE) {\
            isp_err_trace("ISP[%d] BeBuf doesn't initialized!\n", pipe);\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    } while (0)

#define isp_check_stabuf_init_return(pipe, init)\
    do{\
        if ((init) != HI_TRUE) {\
            isp_err_trace("ISP[%d] StaBuf doesn't initialized!\n", pipe);\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    } while (0)

#define isp_check_tranbuf_init_return(pipe, init)\
    do{\
        if ((init) != HI_TRUE) {\
            isp_warn_trace("ISP[%d] TransBuf doesn't initialized!\n", pipe);\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    } while (0)

#define isp_check_exit_state_return(pipe, isp_run_flag)\
    do{\
        if ((isp_run_flag) == HI_TRUE) {\
            isp_err_trace("Isp[%d] cann't exit!\n", pipe);\
            return HI_FAILURE;\
        }\
    } while (0)

#define isp_check_online_mode_return(pipe, running_mode)\
    do{\
        if ((is_online_mode(running_mode)) ||  \
            (is_sidebyside_mode(running_mode))) {\
            return HI_SUCCESS;\
        }\
    } while (0)

#define isp_dbg(en, fmt...)\
    do{\
        if (en) {\
            printf(fmt);\
        } \
    } while (0)

#define slave_check_dev_return(dev)\
    do {\
        if (((dev) < 0) || ((dev) >= CAP_SLAVE_MAX_NUM)) {\
            isp_err_trace("Err slave dev %d!\n", dev);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    } while (0)

#define isp_alg_check_return(alg_key)\
    do{\
        if ((alg_key) == 0) {\
            return HI_SUCCESS; \
        }\
    } while (0)

#define ISP_STATISTICS_KEY              0xFFFBBF77
#define ISP_PIPE_FEAF_MODULE_ENABLE     0, 0, 0, 0
#define ISP_PIPE_FELOGLUT_MODULE_ENABLE 0, 0, 0, 0

#define isp_check_feaf_module_return(dev)\
    do{\
        hi_u8 fe_have_af_mod[] = {ISP_PIPE_FEAF_MODULE_ENABLE};\
        if (!fe_have_af_mod[dev]) {\
            isp_err_trace("Invalid Af Fe Pipe %d!\n", dev);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    } while (0)

#define isp_check_ret(pipe, ret, fmt, ...)\
    do{\
        if (ret != HI_SUCCESS) {\
            isp_err_trace("Pipe[%d] " fmt " err 0x%x!\n", pipe, ret);\
            return ret;\
        }\
    } while (0)

#define isp_check_ret_success(pipe, ret, fmt, ...)\
    do{\
        if (ret != HI_SUCCESS) {\
            isp_warn_trace("Pipe[%d] " fmt " warn 0x%x!\n", pipe, ret);\
            return HI_SUCCESS;\
        }\
    } while (0)

#define isp_check_ret_continue(pipe, ret, fmt, ...)\
    do{\
        if (ret != HI_SUCCESS) {\
            isp_warn_trace("Pipe[%d] " fmt " warn 0x%x!\n", pipe, ret);\
        }\
    } while (0)

#define isp_check_pointer_success(ptr)\
    do {\
        if ((ptr) == HI_NULL) {\
            isp_warn_trace("Null Pointer!\n");\
            return HI_SUCCESS;\
        }\
    } while (0)

#define isp_check_ret_goto(ret, err, failed, fmt, ...)\
    do{                                                 \
        if (ret != HI_SUCCESS) {                        \
            isp_err_trace(fmt, ##__VA_ARGS__);          \
            ret = err;                                  \
            goto failed;                              \
        }                                               \
    } while (0)

#define isp_check_pointer_goto(ptr, ret, err, failed, fmt, ...)   \
    do {                                                            \
        if ((ptr) == HI_NULL) {                                     \
            isp_err_trace(fmt, ##__VA_ARGS__);                      \
            ret = err;                                              \
            goto failed;                                          \
        }                                                           \
    } while (0)

#define isp_check_max_param_return(x, max, fmt, ...)   \
    do {                                            \
        if ((x) > (max)) {                          \
            isp_err_trace(fmt, ##__VA_ARGS__);      \
            return HI_ERR_ISP_ILLEGAL_PARAM;        \
        }                                           \
    } while (0)

#endif
