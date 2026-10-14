/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_ISP_H__
#define __XMEDIA_ISP_H__

#include "xmedia_video_common.h"
#include "xmedia_intf_common.h"
#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_ISP_ISO_MAX_COUNT             16
#define XMEDIA_ISP_EXP_RATIO_MAX_COUNT       9

#define XMEDIA_ISP_PIPE_MAX_NUM          VI_MAX_PIPE_NUM
#define XMEDIA_ISP_WDR_CHN_MAX_NUM       VI_MAX_WDR_NUM
#define XMEDIA_ISP_WDR_EXP_RATIO_MAX_NUM (XMEDIA_ISP_WDR_CHN_MAX_NUM - 1)
#define XMEDIA_ISP_AI_MODEL_PER_ISO      ISP_ISO_MODEL_MAX_NUM
#define XMEDIA_ISP_AI_MODEL_MAX_NUM      (XMEDIA_ISP_ISO_MAX_COUNT * XMEDIA_ISP_AI_MODEL_PER_ISO)
#define XMEDIA_ISP_AINR_MODEL_MAX_NUM    XMEDIA_ISP_AI_MODEL_MAX_NUM
#define XMEDIA_ISP_WDR_MAX_EXP_RATIO     ISP_WDR_MAX_EXP_RATIO
#define XMEDIA_ISP_TRIG_DEV_MAX_NUM      ISP_TRIG_DEV_MAX_NUM

#define XMEDIA_ISP_BAYER_PATTERN_NUM    4
#define XMEDIA_ISP_AE_HIST_NUM          1024
#define XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM  16
#define XMEDIA_ISP_AE_ZONE_COL_MAX_NUM  24
#define XMEDIA_ISP_AWB_ZONE_ROW_MAX_NUM 32
#define XMEDIA_ISP_AWB_ZONE_COL_MAX_NUM 32
#define XMEDIA_ISP_AWB_ZONE_BIN         1
#define XMEDIA_ISP_AWB_ZONE_NUM                                                                                        \
    (XMEDIA_ISP_AWB_ZONE_ROW_MAX_NUM * XMEDIA_ISP_AWB_ZONE_COL_MAX_NUM * XMEDIA_ISP_AWB_ZONE_BIN)
#define XMEDIA_ISP_AF_ZONE_ROW_MAX_NUM 15
#define XMEDIA_ISP_AF_ZONE_COL_MAX_NUM 18

#define XMEDIA_ISP_PD_MAX_WIDTH  128
#define XMEDIA_ISP_PD_MAX_HEIGHT 128

#define XMEDIA_ISP_BAYERNR_LUT_LENGTH       33
#define XMEDIA_ISP_EXP_RATIO_STRENGTH_NUM   16
#define XMEDIA_ISP_LCAC_DET_NUM             3
#define XMEDIA_ISP_PREGAMMA_NODE_NUM        257
#define XMEDIA_ISP_BAYER_CALIBTAION_MAX_NUM 50
#define XMEDIA_ISP_EXP_RATIO_NUM            3
#define XMEDIA_ISP_SLAVE_PIPE_MAX_NUM       3
#define XMEDIA_ISP_EXP_MAX_COUNT            16

typedef struct {
    xmedia_u16 x;
    xmedia_u16 y;
} xmedia_isp_point;

typedef enum {
    XMEDIA_ISP_AAA_CMD_WDR_MODE_SET = 8000,
    XMEDIA_ISP_AAA_CMD_PROC_WRITE,

    XMEDIA_ISP_AAA_CMD_AE_FPS_BASE_SET,
    XMEDIA_ISP_AAA_CMD_AE_BLC_SET,
    XMEDIA_ISP_AAA_CMD_AE_RC_SET,
    XMEDIA_ISP_AAA_CMD_AE_BAYER_FORMAT_SET,
    XMEDIA_ISP_AAA_CMD_AE_INIT_INFO_GET,

    XMEDIA_ISP_AAA_CMD_AWB_ISO_SET, // set iso, change saturation when iso change
    XMEDIA_ISP_AAA_CMD_AWB_BLC_SET,
    XMEDIA_ISP_AAA_CMD_CHANGE_IMAGE_MODE_SET,
    XMEDIA_ISP_AAA_CMD_UPDATE_INFO_GET,
    XMEDIA_ISP_AAA_CMD_FRAMEINFO_GET,
    XMEDIA_ISP_AAA_CMD_ATTACHINFO_GET,
    XMEDIA_ISP_AAA_CMD_COLORGAMUTINFO_GET,
    XMEDIA_ISP_AAA_CMD_AWB_INTTIME_SET,
    XMEDIA_ISP_AAA_CMD_BAS_MODE_SET,
    XMEDIA_ISP_AAA_CMD_PROTRIGGER_SET,
    XMEDIA_ISP_AAA_CMD_AWB_PIRIS_SET,
    XMEDIA_ISP_AAA_CMD_AWB_SNAP_MODE_SET,
    XMEDIA_ISP_AAA_CMD_AWB_ZONE_ROW_SET,
    XMEDIA_ISP_AAA_CMD_AWB_ZONE_COL_SET,
    XMEDIA_ISP_AAA_CMD_AWB_ZONE_BIN_SET,
    XMEDIA_ISP_AAA_CMD_AWB_GAIN_GET,
    XMEDIA_ISP_AAA_CMD_AWB_RESULT_GET,

    XMEDIA_ISP_AAA_CMD_CTRL_CMD_MAX,
} xmedia_isp_aaa_cmd;

typedef enum {
    XMEDIA_ISP_MODULE_ID_MIN = 0x00584D00,
    XMEDIA_ISP_MODULE_ID_ISP = XMEDIA_ISP_MODULE_ID_MIN, // 0x00584D00
    XMEDIA_ISP_MODULE_ID_AWB,                            // 0x00584D01
    XMEDIA_ISP_MODULE_ID_AE,                             // 0x00584D02
    XMEDIA_ISP_MODULE_ID_AF,                             // 0x00584D03
    XMEDIA_ISP_MODULE_ID_MAX
} xmedia_isp_module_id;

#define XMEDIA_ISP_MODULE_MAX_NUM     (XMEDIA_ISP_MODULE_ID_MAX - XMEDIA_ISP_MODULE_ID_MIN)
#define XMEDIA_ISP_VREG_NAME_MAX_SIZE 16

typedef struct {
    // 模块id，以区分ISP/AE/AWB/AF
    xmedia_u32 module;
    // 算法vreg版本，当算法参数有变动时必须改变版本；
    // 参数变动指：增删参数、移动参数位置、修改参数类型等任何会改变vreg参数排布的变动
    xmedia_u32 version;
    // vreg长度，单位：字节
    xmedia_u32 length;
    // 模块名字，比如：‘ISP’‘AE’‘AWB’‘AF’
    xmedia_char name[XMEDIA_ISP_VREG_NAME_MAX_SIZE];
    // 导出时是vreg首地址，导入时是buffer地址
    xmedia_void *addr;
} xmedia_isp_module_info;

typedef enum {
    XMEDIA_ISP_BAYER_RAWBIT_8BIT  = 8,
    XMEDIA_ISP_BAYER_RAWBIT_10BIT = 10,
    XMEDIA_ISP_BAYER_RAWBIT_12BIT = 12,
    XMEDIA_ISP_BAYER_RAWBIT_14BIT = 14,
    XMEDIA_ISP_BAYER_RAWBIT_16BIT = 16,
    XMEDIA_ISP_BAYER_RAWBIT_MAX
} xmedia_isp_bayer_rawbit;

typedef enum {
    XMEDIA_ISP_WDR_MODE_NORMAL = 0,
    XMEDIA_ISP_WDR_MODE_LONG_FRAME,
    XMEDIA_ISP_WDR_MODE_AUTO_LONG_FRAME,
    XMEDIA_ISP_WDR_MODE_MAX
} xmedia_isp_wdr_mode;

typedef struct {
    xmedia_video_wdr_mode   mode;
    xmedia_video_wdr_format fmt;
} xmedia_isp_wdr_info;

typedef enum {
    XMEDIA_ISP_WORK_MODE_MASTER = 0,
    XMEDIA_ISP_WORK_MODE_SLAVE,
    XMEDIA_ISP_WORK_MODE_MAX
} xmedia_isp_work_mode;

#define XMEDIA_ISP_FOLLOW_AAC       (1 << 0)
#define XMEDIA_ISP_FOLLOW_AE        (1 << 1)
#define XMEDIA_ISP_FOLLOW_AF        (1 << 2)
#define XMEDIA_ISP_FOLLOW_AWB       (1 << 3)
#define XMEDIA_ISP_FOLLOW_BLC       (1 << 4)
#define XMEDIA_ISP_FOLLOW_BNR       (1 << 5)
#define XMEDIA_ISP_FOLLOW_CCM       (1 << 6)
#define XMEDIA_ISP_FOLLOW_CDS       (1 << 7)
#define XMEDIA_ISP_FOLLOW_CLUT      (1 << 8)
#define XMEDIA_ISP_FOLLOW_COMPRESS  (1 << 9)
#define XMEDIA_ISP_FOLLOW_CROSSTALK (1 << 10)
#define XMEDIA_ISP_FOLLOW_CSC       (1 << 11)
#define XMEDIA_ISP_FOLLOW_CUS       (1 << 12)
#define XMEDIA_ISP_FOLLOW_DEHAZE    (1 << 13)
#define XMEDIA_ISP_FOLLOW_DGN       (1 << 14)
#define XMEDIA_ISP_FOLLOW_DMS       (1 << 15)
#define XMEDIA_ISP_FOLLOW_DPC       (1 << 16)
#define XMEDIA_ISP_FOLLOW_DRC       (1 << 17)
#define XMEDIA_ISP_FOLLOW_EXPANDER  (1 << 18)
#define XMEDIA_ISP_FOLLOW_FPN       (1 << 19)
#define XMEDIA_ISP_FOLLOW_GAMMA     (1 << 20)
#define XMEDIA_ISP_FOLLOW_GCAC      (1 << 21)
#define XMEDIA_ISP_FOLLOW_HLC       (1 << 22)
#define XMEDIA_ISP_FOLLOW_LCE       (1 << 23)
#define XMEDIA_ISP_FOLLOW_MLSC      (1 << 24)
#define XMEDIA_ISP_FOLLOW_RC        (1 << 25)
#define XMEDIA_ISP_FOLLOW_RGBIR     (1 << 26)
#define XMEDIA_ISP_FOLLOW_RLSC      (1 << 27)
#define XMEDIA_ISP_FOLLOW_SHARPEN   (1 << 28)
#define XMEDIA_ISP_FOLLOW_WDR       (1 << 29)
#define XMEDIA_ISP_FOLLOW_CA        (1 << 30)
#define XMEDIA_ISP_FOLLOW_SENSOR    (1 << 31)
#define XMEDIA_ISP_FOLLOW_ALL       0xFFFFFFFF

typedef struct {
    xmedia_isp_work_mode work_mode;

    union {
        struct {
            xmedia_u32  slave_num;
            xmedia_u32  slave_pipe[XMEDIA_ISP_SLAVE_PIPE_MAX_NUM];
            xmedia_bool blend_stat_enable;
            xmedia_bool blend_luma_enable;
        } master_mode;

        struct {
            xmedia_u32 master_pipe; // pipe id depends on master pipe
            xmedia_u64 follow_info; // parameters depends on master pipe 1bit represent 1 alg
        } slave_mode;
    };
} xmedia_isp_mode_config;

typedef struct {
    xmedia_video_size         size;
    xmedia_float              fps;
    xmedia_video_wdr_mode     wdr_mode;
    xmedia_video_pixel_format pixel_fmt;
    xmedia_video_bayer_format bayer_fmt;
    xmedia_isp_mode_config    mode_config;
} xmedia_isp_config;

typedef struct {
    xmedia_video_size         size;
    xmedia_float              fps;
    xmedia_video_wdr_mode     wdr_mode;
    xmedia_video_bayer_format bayer_fmt;
} xmedia_isp_attr;

typedef struct {
    xmedia_u32 proc_interval; // RW;format:32.0; indicate the update frequency of isp proc information
    xmedia_u32 stat_interval; // Rw;range:(0,0xffffffff];format:32.0; indicate the time interval of isp stat info
    xmedia_u32 int_timeout;   // RW;Format:32.0; Indicate the time(unit:ms) of interrupt timeout
} xmedia_isp_ctrl_param;

typedef struct {
    xmedia_bool enable;
    xmedia_bool available;
    xmedia_u8   luma;
} xmedia_isp_smart_roi;

#define XMEDIA_ISP_SMART_CLASS_MAX 2

typedef struct {
    xmedia_isp_smart_roi roi[XMEDIA_ISP_SMART_CLASS_MAX];
} xmedia_isp_smart_info;

#define XMEDIA_ISP_BNR_POINT_MAX_COUNT       61
#define XMEDIA_ISP_BNR_CALIB_POINT_MAX_COUNT 5
#define XMEDIA_ISP_BNR_RESI_NOISE_MAX_COUNT  49
#define XMEDIA_ISP_BNR_SQRT_LUT_MAX_COUNT    1025
#define XMEDIA_ISP_BNR_COLOR_NUM_MAX_COUNT   3
#define XMEDIA_ISP_BNR_THD_MAX_COUNT         4

typedef enum {
    XMEDIA_ISP_BNR_CENTER_MODE_MIN_VAL = 0,
    XMEDIA_ISP_BNR_CENTER_MODE_SMALL_WIN,
    XMEDIA_ISP_BNR_CENTER_MODE_BIG_WIN,
    XMEDIA_ISP_BNR_CENTER_MODE_MAX_VAL,
    XMEDIA_ISP_BNR_CENTER_MODE_MAX
} xmedia_isp_bnr_center_mode;

typedef enum {
    XMEDIA_ISP_BNR_CORING_POS_PRE = 0,
    XMEDIA_ISP_BNR_CORING_POS_POST,
    XMEDIA_ISP_BNR_CORING_POS_MAX
} xmedia_isp_bnr_coring_pos;

typedef enum {
    XMEDIA_ISP_BNR_AINR_POS_OFF = 0, // ainr_pre_enable=0 & tnr_ainr_mode=0
    XMEDIA_ISP_BNR_AINR_POS_PRE,     // ainr_pre_enable=1 & tnr_ainr_mode=0
    XMEDIA_ISP_BNR_AINR_POS_POST,    // ainr_pre_enable=0 & tnr_ainr_mode=1
    XMEDIA_ISP_BNR_AINR_POS_MAX
} xmedia_isp_bnr_ainr_pos;

typedef enum {
    XMEDIA_ISP_BNR_TNR_DP_MODE_ZERO = 0,
    XMEDIA_ISP_BNR_TNR_DP_MODE_ONE,
    XMEDIA_ISP_BNR_TNR_DP_MODE_MAX
} xmedia_isp_bnr_tnr_dp_mode;

typedef struct {
    xmedia_u8                  iso_num;
    xmedia_u32                 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 snr_coarse_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u8                  snr_coring_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 snr_luma_nr_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_BNR_POINT_MAX_COUNT];
    xmedia_isp_bnr_center_mode snr_center_luma_mode[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_isp_bnr_center_mode snr_center_tnr_luma_mode[XMEDIA_ISP_ISO_MAX_COUNT]; // 7605V2
    xmedia_u16                 snr_wdr_comp_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    // Bayer3D param
    xmedia_u16                 tnr_noise_min[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_bayer_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_md_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_md_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_wgt_min[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_min_ratio[XMEDIA_ISP_ISO_MAX_COUNT]; // 7605V2
    xmedia_u16                 tnr_mot_bld_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_bld_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_edge_md_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_edge_md_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_flat_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_edge_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                  tnr_md_mode[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_luma_md_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_luma_md_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dark_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_bright_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_scl[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_BNR_COLOR_NUM_MAX_COUNT];
    xmedia_u16                 tnr_var_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_dark_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_bright_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_dpc_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_convg_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_BNR_THD_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_scl_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_bool                tnr_dpc_en[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_isp_bnr_tnr_dp_mode tnr_dp_mode[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dp_thd_rb[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dp_thd_g[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dp_luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dp_luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dp_thd_low_dark_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_dp_thd_high_dark_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                 tnr_luma_ratio[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_BNR_POINT_MAX_COUNT];
    xmedia_u16                 tnr_wdr_comp_scl[XMEDIA_ISP_ISO_MAX_COUNT]; // 7605V2
} xmedia_isp_bnr_auto_attr;

typedef struct {
    xmedia_u16                 snr_coarse_str[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u8                  snr_coring_wgt;
    xmedia_u16                 snr_luma_nr_str[XMEDIA_ISP_BNR_POINT_MAX_COUNT];
    xmedia_isp_bnr_center_mode snr_center_luma_mode;
    xmedia_isp_bnr_center_mode snr_center_tnr_luma_mode;
    xmedia_u16                 snr_wdr_comp_scl;
    // Bayer3D param
    xmedia_u16                 tnr_noise_min;
    xmedia_u16                 tnr_bayer_str;
    xmedia_u16                 tnr_md_thd_low;
    xmedia_u16                 tnr_md_thd_high;
    xmedia_u16                 tnr_wgt_min;
    xmedia_u16                 tnr_min_ratio; // 7605V2
    xmedia_u16                 tnr_mot_bld_thd_low;
    xmedia_u16                 tnr_mot_bld_thd_high;
    xmedia_u16                 tnr_edge_md_thd_low;
    xmedia_u16                 tnr_edge_md_thd_high;
    xmedia_u16                 tnr_flat_md_scl;
    xmedia_u16                 tnr_edge_md_scl;
    xmedia_u8                  tnr_md_mode;
    xmedia_u16                 tnr_luma_md_thd_low;
    xmedia_u16                 tnr_luma_md_thd_high;
    xmedia_u16                 tnr_dark_md_scl;
    xmedia_u16                 tnr_bright_md_scl;
    xmedia_u16                 tnr_mot_scl[XMEDIA_ISP_BNR_COLOR_NUM_MAX_COUNT];
    xmedia_u16                 tnr_var_scl;
    xmedia_u16                 tnr_mot_noise_luma_thd_low;
    xmedia_u16                 tnr_mot_noise_luma_thd_high;
    xmedia_u16                 tnr_mot_noise_dark_scl;
    xmedia_u16                 tnr_mot_noise_bright_scl;
    xmedia_u16                 tnr_mot_dpc_str;
    xmedia_u16                 tnr_mot_convg_thd[XMEDIA_ISP_BNR_THD_MAX_COUNT];
    xmedia_u16                 tnr_mot_noise_scl;
    xmedia_u16                 tnr_mot_noise_scl_high;
    xmedia_bool                tnr_dpc_en;
    xmedia_isp_bnr_tnr_dp_mode tnr_dp_mode;
    xmedia_u16                 tnr_dp_thd_rb;
    xmedia_u16                 tnr_dp_thd_g;
    xmedia_u16                 tnr_dp_luma_thd_low;
    xmedia_u16                 tnr_dp_luma_thd_high;
    xmedia_u16                 tnr_dp_thd_low_dark_scl;
    xmedia_u16                 tnr_dp_thd_high_dark_scl;
    xmedia_u16                 tnr_luma_ratio[XMEDIA_ISP_BNR_POINT_MAX_COUNT];
    xmedia_u16                 tnr_wdr_comp_scl;
} xmedia_isp_bnr_manual_attr;

typedef struct {
    xmedia_u8 wdr_frame_str[XMEDIA_ISP_WDR_CHN_MAX_NUM];
} xmedia_isp_bnr_wdr_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_isp_bnr_ainr_pos     ainr_pos_switch;
    xmedia_isp_bnr_coring_pos   coring_pos_switch;
    xmedia_bool                 noise_reserve_enable;
    xmedia_u16                  noise_reserve_lut[XMEDIA_ISP_BNR_POINT_MAX_COUNT];
    xmedia_u32                  noise_profile_thd;
    xmedia_double               noise_profile_coef0[XMEDIA_ISP_BNR_CALIB_POINT_MAX_COUNT]; // low_iso_noise_coef
    xmedia_double               noise_profile_coef1[XMEDIA_ISP_BNR_CALIB_POINT_MAX_COUNT]; // high_iso_noise_coef
    xmedia_bool                 snr_wdr_comp_en;
    xmedia_bool                 tnr_enable;
    xmedia_bool                 tnr_luma_comp_enable;
    xmedia_bool                 tnr_wdr_comp_enable; // 7605V2
    xmedia_video_operation_mode op_type;
    xmedia_isp_bnr_auto_attr    auto_attr;
    xmedia_isp_bnr_manual_attr  manual_attr;
    xmedia_isp_bnr_wdr_attr     wdr;
} xmedia_isp_bnr_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_u16  luma_thr;
    xmedia_u16  luma_target;
} xmedia_isp_hlc_attr;

typedef enum {
    XMEDIA_ISP_GAMMA_CURVE_TYPE_DEFAULT = 0,
    XMEDIA_ISP_GAMMA_CURVE_TYPE_SRGB,
    XMEDIA_ISP_GAMMA_CURVE_TYPE_HDR,
    XMEDIA_ISP_GAMMA_CURVE_TYPE_USER_DEFINE,
    XMEDIA_ISP_GAMMA_CURVE_TYPE_MAX
} xmedia_isp_gamma_curve_type;

#define XMEDIA_ISP_GAMMA_NODE_NUM 1025

typedef struct {
    xmedia_u16 table[XMEDIA_ISP_GAMMA_NODE_NUM];
} xmedia_isp_gamma_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 table[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_GAMMA_NODE_NUM];
} xmedia_isp_gamma_auto_attr;

typedef struct {
    xmedia_bool                  enable;
    xmedia_isp_gamma_curve_type  curve_type;
    xmedia_video_operation_mode  op_type;
    xmedia_isp_gamma_manual_attr manual_attr;
    xmedia_isp_gamma_auto_attr   auto_attr;
} xmedia_isp_gamma_attr;

#define XMEDIA_ISP_DRC_TM_VALUE_NUM 1024
#define XMEDIA_ISP_DRC_TM_POS_NUM   3

typedef struct {
    xmedia_u8 asymmetry;
    xmedia_u8 second_pole;
    xmedia_u8 stretch;
    xmedia_u8 compress;
} xmedia_isp_asymmetry_curve_attr;

typedef struct {
    xmedia_u16                        range_fil_coef;
    xmedia_u16                        spatial_fil_coef;
    xmedia_u8                         dering_thd;
    xmedia_u8                         dering_coarse_str;
    xmedia_u16                        dering_fine_str;
    xmedia_u8                         mixing_bright_min;
    xmedia_u8                         mixing_bright_max;
    xmedia_u16                        mixing_bright_thd_low;
    xmedia_u16                        mixing_bright_thd_high;
    xmedia_u8                         mixing_dark_min;
    xmedia_u8                         mixing_dark_max;
    xmedia_u16                        mixing_dark_thd_low;
    xmedia_u16                        mixing_dark_thd_high;
    xmedia_u8                         ltmc_str;
    xmedia_u16                        drc_str;
    xmedia_isp_asymmetry_curve_attr   asymmetry_curve;
    xmedia_isp_asymmetry_curve_attr   asymmetry_max_curve;
    xmedia_u16                        tm_value_user[XMEDIA_ISP_DRC_TM_VALUE_NUM];
    xmedia_u8                         tm_value_max_user[XMEDIA_ISP_DRC_TM_VALUE_NUM];
} xmedia_isp_drc_manual_attr;

typedef struct {
    xmedia_u8                         iso_num;
    xmedia_u32                        iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        range_fil_coef[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        spatial_fil_coef[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         dering_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         dering_coarse_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        dering_fine_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         mixing_bright_min[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         mixing_bright_max[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        mixing_bright_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        mixing_bright_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         mixing_dark_min[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         mixing_dark_max[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        mixing_dark_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        mixing_dark_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         ltmc_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16                        drc_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8                         ratio_num;
    xmedia_u16                        ratio_level[XMEDIA_ISP_EXP_RATIO_MAX_COUNT];
    xmedia_isp_asymmetry_curve_attr   asymmetry_curve[XMEDIA_ISP_EXP_RATIO_MAX_COUNT];
    xmedia_isp_asymmetry_curve_attr   asymmetry_max_curve[XMEDIA_ISP_EXP_RATIO_MAX_COUNT];
    xmedia_u16                        tm_value_user[XMEDIA_ISP_EXP_RATIO_MAX_COUNT][XMEDIA_ISP_DRC_TM_VALUE_NUM];
    xmedia_u8                         tm_value_max_user[XMEDIA_ISP_EXP_RATIO_MAX_COUNT][XMEDIA_ISP_DRC_TM_VALUE_NUM];
} xmedia_isp_drc_auto_attr;

typedef enum {
    XMEDIA_ISP_DRC_TM_TYPE_ASYMMETRY = 0,
    XMEDIA_ISP_DRC_TM_TYPE_USER_DEFINE,
    XMEDIA_ISP_DRC_TM_TYPE_MAX
} xmedia_isp_drc_tm_type;

typedef struct {
    xmedia_bool                 drc_en;
    xmedia_bool                 ltmc_en;
    xmedia_bool                 dering_en;
    xmedia_bool                 detail_enhance_en;
    xmedia_isp_drc_tm_type      tm_type;
    xmedia_video_operation_mode op_type;
    xmedia_isp_drc_manual_attr  manual_attr;
    xmedia_isp_drc_auto_attr    auto_attr;
} xmedia_isp_drc_attr;

#define XMEDIA_ISP_EXPANDER_POINT_NUM      257
#define XMEDIA_ISP_EXPANDER_KNEE_POINT_NUM (XMEDIA_ISP_EXPANDER_POINT_NUM - 1)

typedef struct {
    xmedia_bool        enable;
    xmedia_u8          bit_depth_in;
    xmedia_u8          bit_depth_out;
    xmedia_u16         knee_point_num;
    xmedia_video_point knee_point_coord[XMEDIA_ISP_EXPANDER_KNEE_POINT_NUM];
} xmedia_isp_expander_attr;

#define XMEDIA_ISP_CT_MAX_COUNT 16

typedef struct {
    xmedia_bool enable;
    xmedia_u8   iso_num;
    xmedia_u32  iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16  mesh_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16  ct_num;
    xmedia_u16  ct_level[XMEDIA_ISP_CT_MAX_COUNT];
    xmedia_u16  blend_ratio[XMEDIA_ISP_CT_MAX_COUNT];
} xmedia_isp_mesh_shading_attr;

#define XMEDIA_ISP_LSC_GRID_POINTS (33 * 33)

typedef struct {
    xmedia_u16 rgain[XMEDIA_ISP_LSC_GRID_POINTS];
    xmedia_u16 grgain[XMEDIA_ISP_LSC_GRID_POINTS];
    xmedia_u16 gbgain[XMEDIA_ISP_LSC_GRID_POINTS];
    xmedia_u16 bgain[XMEDIA_ISP_LSC_GRID_POINTS];
} xmedia_isp_shading_gain_lut;

#define XMEDIA_ISP_LSC_GRID_COL     33
#define XMEDIA_ISP_LSC_GRID_ROW     33
#define XMEDIA_ISP_LSC_GAIN_LUT_NUM 2

typedef struct {
    xmedia_u8                   mesh_scale;
    xmedia_u16                  xgrid_width[(XMEDIA_ISP_LSC_GRID_COL - 1) / 2];
    xmedia_u16                  ygrid_width[(XMEDIA_ISP_LSC_GRID_ROW - 1) / 2];
    xmedia_isp_shading_gain_lut lsc_gain_lut[XMEDIA_ISP_LSC_GAIN_LUT_NUM];
} xmedia_isp_mesh_shading_lut_attr;

typedef enum {
    XMEDIA_ISP_STATIC_DPC_TYPE_BRIGHT = 0,
    XMEDIA_ISP_STATIC_DPC_TYPE_DARK,
    XMEDIA_ISP_STATIC_DPC_TYPE_MAX
} xmedia_isp_static_dpc_type;

typedef enum {
    XMEDIA_ISP_DPC_STATUS_INIT = 0,
    XMEDIA_ISP_DPC_STATUS_PROCESS,
    XMEDIA_ISP_DPC_STATUS_SUCCESS,
    XMEDIA_ISP_DPC_STATUS_TIMEOUT,
    XMEDIA_ISP_DPC_STATUS_MAX
} xmedia_isp_dpc_status;

#define XMEDIA_ISP_STATIC_DPC_COUNT_MAX 4096

typedef struct {
    xmedia_bool                enable_detect;
    xmedia_isp_static_dpc_type static_dpc_type;
    xmedia_u8                  start_thd;
    xmedia_u16                 count_max;
    xmedia_u16                 count_min;
    xmedia_u16                 time_limit;
    xmedia_u32                 table[XMEDIA_ISP_STATIC_DPC_COUNT_MAX];
    xmedia_u8                  finish_thd;
    xmedia_u16                 count;
    xmedia_isp_dpc_status      status;
} xmedia_isp_dpc_static_calibrate;

typedef struct {
    xmedia_bool enable;
    xmedia_bool pd_enable; // correct pd pixel enable
    xmedia_u16  bright_count;
    xmedia_u16  dark_count;
    xmedia_u32  bright_table[XMEDIA_ISP_STATIC_DPC_COUNT_MAX];
    xmedia_u32  dark_table[XMEDIA_ISP_STATIC_DPC_COUNT_MAX];
    xmedia_bool show;
} xmedia_isp_dpc_static_attr;

typedef struct {
    xmedia_u16 str;
    xmedia_u16 thd;
    xmedia_u16 slope;
    xmedia_u16 bright_thd;
    xmedia_u8  bright_bias;
    xmedia_u16 dark_thd;
    xmedia_u8  dark_bias;
} xmedia_isp_dpc_dynamic_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 slope[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 bright_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  bright_bias[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 dark_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  dark_bias[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_dpc_dynamic_auto_attr;

typedef struct {
    xmedia_bool                        enable;
    xmedia_bool                        sup_twinkle_en;
    xmedia_s8                          soft_thd;
    xmedia_u8                          soft_slope;
    xmedia_video_operation_mode        op_type;
    xmedia_isp_dpc_dynamic_manual_attr manual_attr;
    xmedia_isp_dpc_dynamic_auto_attr   auto_attr;
} xmedia_isp_dpc_dynamic_attr;

#define XMEDIA_ISP_PD_BLOCK_MAX_NUM 128

typedef struct {
    xmedia_u16 pd_start_row;
    xmedia_u16 pd_start_col;
    xmedia_u16 pd_zone_height;
    xmedia_u16 pd_zone_width;
    xmedia_u16 pd_block_width;
    xmedia_u16 pd_block_height;
    xmedia_u16 pd_block_pixel_num;
    xmedia_u8  pd_block_row[XMEDIA_ISP_PD_BLOCK_MAX_NUM];
    xmedia_u8  pd_block_col[XMEDIA_ISP_PD_BLOCK_MAX_NUM];
} xmedia_isp_pd_struct_info;

typedef struct {
    xmedia_u8  slope;
    xmedia_u16 threshold;
    xmedia_u16 np_offset;
} xmedia_isp_cr_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  slope[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 threshold[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 np_offset[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_cr_auto_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_video_operation_mode op_type;
    xmedia_isp_cr_manual_attr   manual_attr;
    xmedia_isp_cr_auto_attr     auto_attr;
} xmedia_isp_cr_attr;

#define XMEDIA_ISP_DEHAZE_LUT_NUM    256
#define XMEDIA_ISP_DEHAZE_MAP_LENGTH 17
#define XMEDIA_ISP_DAIN_MAP_NUM      384

typedef struct {
    xmedia_u8  strength;
    xmedia_u16 bmap_curve[XMEDIA_ISP_DEHAZE_MAP_LENGTH];
    xmedia_u16 dmap_curve[XMEDIA_ISP_DEHAZE_MAP_LENGTH];
} xmedia_isp_dehaze_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  strength[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 bmap_curve[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_DEHAZE_MAP_LENGTH];
    xmedia_u16 dmap_curve[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_DEHAZE_MAP_LENGTH];
} xmedia_isp_dehaze_auto_attr;

typedef struct {
    xmedia_bool                   enable;
    xmedia_bool                   userlut_enable;
    xmedia_u8                     dehaze_lut[XMEDIA_ISP_DEHAZE_LUT_NUM];
    xmedia_video_operation_mode   op_type;
    xmedia_isp_dehaze_manual_attr manual_attr;
    xmedia_isp_dehaze_auto_attr   auto_attr;
    xmedia_u16                    temporal_incr_coef;
    xmedia_u16                    temporal_decr_coef;
} xmedia_isp_dehaze_attr;

typedef struct {
    xmedia_u8  mf_de_str;
    xmedia_u8  mf_de_osh_str;
    xmedia_u8  mf_de_ush_str;
    xmedia_u8  hf_de_str;
    xmedia_u8  hf_smooth_range;
    xmedia_u16 detail_smooth_str;
    xmedia_u8  detail_mf_de_bias;
    xmedia_u8  detail_hf_de_bias;
} xmedia_isp_demosaic_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  mf_de_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  mf_de_osh_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  mf_de_ush_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  hf_de_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  hf_smooth_range[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 detail_smooth_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  detail_mf_de_bias[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  detail_hf_de_bias[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_demosaic_auto_attr;

typedef struct {
    xmedia_bool                     enable;
    xmedia_video_operation_mode     op_type;
    xmedia_isp_demosaic_auto_attr   auto_attr;
    xmedia_isp_demosaic_manual_attr manual_attr;
} xmedia_isp_demosaic_attr;

typedef struct {
    xmedia_u16 black_level[XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_blc_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 black_level[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_blc_auto_attr;

typedef struct {
    xmedia_video_operation_mode op_type;
    xmedia_isp_blc_manual_attr  manual_attr;
    xmedia_isp_blc_auto_attr    auto_attr;
} xmedia_isp_blc_attr;

typedef struct {
    xmedia_u32              frame_num;
    xmedia_u32              offset;
    xmedia_video_frame_info fpn_frame;
} xmedia_isp_fpn_calibrate_attr;

typedef struct {
    xmedia_bool             enable;
    xmedia_video_frame_info fpn_frm_info;
} xmedia_isp_fpn_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_u16  ver_coord;
    xmedia_u16  hor_coord;
    xmedia_s16  param_red_a;
    xmedia_s16  param_red_b;
    xmedia_s16  param_red_c;
    xmedia_s16  param_blue_a;
    xmedia_s16  param_blue_b;
    xmedia_s16  param_blue_c;
    xmedia_u8   ver_norm_shift;
    xmedia_u8   ver_norm_factor;
    xmedia_u8   hor_norm_shift;
    xmedia_u8   hor_norm_factor;
    xmedia_u16  cor_var_thd;
} xmedia_isp_gcac_attr;

typedef struct {
    xmedia_s16 purple_upper_limit;
    xmedia_s16 purple_lower_limit;
    xmedia_u8  edge_thd;
    xmedia_u8  de_purple_cr_str;
    xmedia_u8  de_purple_cb_str;
    xmedia_u8  luma_thd_high;
    xmedia_u8  luma_thd_low;
    xmedia_u8  luma_str_high;
    xmedia_u8  luma_str_low;
} xmedia_isp_lcac_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_s16 purple_upper_limit[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_s16 purple_lower_limit[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  edge_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  de_purple_cr_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  de_purple_cb_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  luma_str_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  luma_str_low[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_lcac_auto_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_video_operation_mode op_type;
    xmedia_isp_lcac_auto_attr   auto_attr;
    xmedia_isp_lcac_manual_attr manual_attr;
} xmedia_isp_lcac_attr;

typedef struct {
    xmedia_u8 edge_thd;  // 去伪彩阈值
    xmedia_u8 edge_gain; // 去伪彩边缘增益
    xmedia_u8 fcr_str;   // 去伪彩强度
    xmedia_u8 sat_low;   // 去伪彩饱和度低阈值
    xmedia_u8 sat_high;  // 去伪彩饱和度高阈值
} xmedia_isp_anti_false_color_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  edge_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  edge_gain[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  fcr_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  sat_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  sat_high[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_anti_false_color_auto_attr;

typedef struct {
    xmedia_bool                             enable;
    xmedia_video_operation_mode             op_type;
    xmedia_isp_anti_false_color_auto_attr   auto_attr;
    xmedia_isp_anti_false_color_manual_attr manual_attr;
} xmedia_isp_anti_false_color_attr;

typedef struct {
    xmedia_u8  md_thd_slope_gain;
    xmedia_u16 md_thd_hig_gain;
} xmedia_isp_wdr_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  md_thd_slope_gain[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 md_thd_hig_gain[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_wdr_auto_attr;

typedef enum {
    XMEDIA_ISP_MOT_MERGE_MODE_SHORT = 0,
    XMEDIA_ISP_MOT_MERGE_MODE_LONG,
    XMEDIA_ISP_MOT_MERGE_MODE_MAX
} xmedia_isp_wdr_mot_merge_mode;

typedef struct {
    xmedia_isp_wdr_mot_merge_mode mot_merge_mode;
    xmedia_bool                   mot_merge_short_ref_en;
    xmedia_bool                   short_expo_chk;
    xmedia_u16                    short_check_thd;
    xmedia_bool                   md_ref_flicker;
    xmedia_u8                     md_slope;
    xmedia_u8                     md_full_thd;
    xmedia_u8                     md_long_blend;
    xmedia_video_operation_mode   op_type;
    xmedia_isp_wdr_manual_attr    manual_attr;
    xmedia_isp_wdr_auto_attr      auto_attr;

    xmedia_bool short_to_long_en;
    xmedia_u16  short_to_long_short_thd;
    xmedia_u16  short_to_long_short_slope;
    xmedia_u16  short_to_long_long_thd;
    xmedia_u8   short_to_long_rb_shift_mode;
    xmedia_s16  short_to_long_red_shift;
    xmedia_s16  short_to_long_blue_shift;
    xmedia_u16  short_to_long_wb_gain_r;
    xmedia_u16  short_to_long_wb_gain_b;
    xmedia_u16  short_to_long_wb_gain_thd;
} xmedia_isp_wdr_mdt_attr;

typedef struct {
    xmedia_bool             dgn_ref_bypass;
    xmedia_bool             motion_comp;
    xmedia_u16              short_thd;
    xmedia_u16              long_thd;
    xmedia_u8               force_long;
    xmedia_bool             ce_force_long_en;
    xmedia_u16              ce_force_long_slope;
    xmedia_u16              ce_force_long_hig_thd;
    xmedia_isp_wdr_mdt_attr wdr_mdt;
} xmedia_isp_wdr_combine_attr;

typedef enum {
    XMEDIA_ISP_WDR_WBGAIN_POSITION_IN_DG1 = 0,
    XMEDIA_ISP_WDR_WBGAIN_POSITION_IN_WB,
    XMEDIA_ISP_WDR_WBGAIN_POSITION_MAX
} xmedia_isp_wdr_wbgain_position;

typedef struct {
    xmedia_bool                    enable;
    xmedia_isp_wdr_wbgain_position wdr_wbgain_position;
    xmedia_isp_wdr_combine_attr    wdr_combine;
} xmedia_isp_wdr_attr;

#define XMEDIA_ISP_LCE_CONTRAST_SLOPE_NUM  2
#define XMEDIA_ISP_LCE_FLAT_FILTER_SIZE    9
#define XMEDIA_ISP_LCE_MAX_CURVE_POINT_NUM 33
#define XMEDIA_ISP_LCE_CONTRAST_BWLE_NUM   2

typedef struct {
    xmedia_u8  contrast_str;
    xmedia_s8  contrast_blc_slope[XMEDIA_ISP_LCE_CONTRAST_SLOPE_NUM];
    xmedia_s8  contrast_wht_slope[XMEDIA_ISP_LCE_CONTRAST_SLOPE_NUM];
    xmedia_u8  blending_fmax;
    xmedia_u16 blending_sigma_div;
    xmedia_u8  blending_mean;
    xmedia_u8  blending_blc_str;
    xmedia_u8  blending_blc_value;
    xmedia_u8  blending_wht_str;
    xmedia_u8  blending_wht_value;
    xmedia_u16 black_luma_expand[XMEDIA_ISP_LCE_CONTRAST_BWLE_NUM];
    xmedia_u16 white_luma_expand[XMEDIA_ISP_LCE_CONTRAST_BWLE_NUM];
    xmedia_u16 flat_denoise_thd;
    xmedia_u16 flat_denoise_str;
    xmedia_u8  flat_filter_str[XMEDIA_ISP_LCE_FLAT_FILTER_SIZE];
    xmedia_u8  halo_str;
    xmedia_u8  temporal_incr_coef;
    xmedia_u8  temporal_decr_coef;
} xmedia_isp_lce_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  contrast_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_s8  contrast_blc_slope[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_LCE_CONTRAST_SLOPE_NUM];
    xmedia_s8  contrast_wht_slope[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_LCE_CONTRAST_SLOPE_NUM];
    xmedia_u8  blending_fmax[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blending_sigma_div[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blending_mean[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blending_blc_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blending_blc_value[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blending_wht_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blending_wht_value[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 black_luma_expand[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_LCE_CONTRAST_BWLE_NUM];
    xmedia_u16 white_luma_expand[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_LCE_CONTRAST_BWLE_NUM];
    xmedia_u16 flat_denoise_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 flat_denoise_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  flat_filter_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_LCE_FLAT_FILTER_SIZE];
    xmedia_u8  halo_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  temporal_incr_coef[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  temporal_decr_coef[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_lce_auto_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_video_operation_mode op_type;
    xmedia_bool                 input_di_mode_en;
    xmedia_bool                 output_di_mode_en;
    xmedia_isp_lce_manual_attr  manual_attr;
    xmedia_isp_lce_auto_attr    auto_attr;
} xmedia_isp_lce_attr;

typedef struct {
    xmedia_u16 blending_weight_curve[XMEDIA_ISP_LCE_MAX_CURVE_POINT_NUM];
} xmedia_isp_lce_info;

#define XMEDIA_ISP_CA_HSL_HUE_NUM   24
#define XMEDIA_ISP_CA_HSL_LIGHT_NUM 33
#define XMEDIA_ISP_CA_YUV_SAT_NUM   33

typedef struct {
    xmedia_u8                     sat_str;
    xmedia_u16                    sat_by_y[XMEDIA_ISP_CA_YUV_SAT_NUM];
    xmedia_u16                    sat_by_c[XMEDIA_ISP_CA_YUV_SAT_NUM];
} xmedia_isp_ca_yuv_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  sat_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 sat_by_y[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_CA_YUV_SAT_NUM];
    xmedia_u16 sat_by_c[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_CA_YUV_SAT_NUM];
} xmedia_isp_ca_yuv_auto_attr;



typedef struct {
    xmedia_bool                   enable;
    xmedia_video_operation_mode   op_type;
    xmedia_isp_ca_yuv_manual_attr manual_attr;
    xmedia_isp_ca_yuv_auto_attr   auto_attr;
} xmedia_isp_ca_attr;

typedef struct {
    xmedia_s32 x;
    xmedia_s32 y;
} xmedia_isp_rc_point;

typedef struct {
    xmedia_bool         enable;
    xmedia_isp_rc_point center_coor;
    xmedia_u32          radius;
} xmedia_isp_rc_attr;

typedef enum {
    XMEDIA_ISP_CSC_COLOR_GAMUT_BT601 = 0,
    XMEDIA_ISP_CSC_COLOR_GAMUT_BT709,
    XMEDIA_ISP_CSC_COLOR_GAMUT_BT2020,
    XMEDIA_ISP_CSC_COLOR_GAMUT_USER,
    XMEDIA_ISP_CSC_COLOR_GAMUT_MAX
} xmedia_isp_csc_color_gamut;

#define XMEDIA_ISP_CSC_INPUT_DC_NUM  3
#define XMEDIA_ISP_CSC_OUTPUT_DC_NUM 3
#define XMEDIA_ISP_CSC_MATRIX_SIZE   9

typedef struct {
    xmedia_s16 csc_idc[XMEDIA_ISP_CSC_INPUT_DC_NUM];
    xmedia_s16 csc_odc[XMEDIA_ISP_CSC_OUTPUT_DC_NUM];
    xmedia_s16 csc_coef[XMEDIA_ISP_CSC_MATRIX_SIZE];
} xmedia_isp_csc_matrix;

typedef struct {
    xmedia_bool                enable;
    xmedia_isp_csc_color_gamut color_gamut;
    xmedia_u8                  hue;
    xmedia_u8                  luma;
    xmedia_u8                  contrast;
    xmedia_u8                  saturation;
    xmedia_bool                limited_range_en;
    xmedia_bool                ext_csc_en;
    xmedia_bool                ct_mode_en;
    xmedia_isp_csc_matrix      csc_matrix;
} xmedia_isp_csc_attr;

#define XMEDIA_ISP_CLUT_LUT_LENGTH   (4913 * 3)
#define XMEDIA_ISP_CLUT_CT_MAX_COUNT 3

typedef struct {
    xmedia_u16 lut[XMEDIA_ISP_CLUT_LUT_LENGTH];
} xmedia_isp_clut_manual_attr;

typedef struct {
    xmedia_u8  ct_num;
    xmedia_u16 ct_level[XMEDIA_ISP_CLUT_CT_MAX_COUNT];
    xmedia_u16 lut[XMEDIA_ISP_CLUT_CT_MAX_COUNT][XMEDIA_ISP_CLUT_LUT_LENGTH];
} xmedia_isp_clut_auto_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_video_operation_mode op_type;
    xmedia_isp_clut_manual_attr manual_attr;
    xmedia_isp_clut_auto_attr   auto_attr;
} xmedia_isp_clut_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_u8   iso_num;
    xmedia_u32  iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16  radial_str[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_radial_shading_attr;

#define XMEDIA_ISP_RLSC_POINTS 129

typedef struct {
    xmedia_u16 wb_rgain;
    xmedia_u16 wb_grgain;
    xmedia_u16 wb_gbgain;
    xmedia_u16 wb_bgain;
    xmedia_u16 r_gain[XMEDIA_ISP_RLSC_POINTS];
    xmedia_u16 gr_gain[XMEDIA_ISP_RLSC_POINTS];
    xmedia_u16 gb_gain[XMEDIA_ISP_RLSC_POINTS];
    xmedia_u16 b_gain[XMEDIA_ISP_RLSC_POINTS];
} xmedia_isp_radial_shading_gain_lut;

#define XMEDIA_ISP_RLSC_GAIN_LUT_NUM 3

typedef struct {
    xmedia_video_operation_mode        light_mode;
    xmedia_u16                         blend_ratio;
    xmedia_u8                          light_type1;
    xmedia_u8                          light_type2;
    xmedia_u8                          radial_scale;
    xmedia_u16                         center_r_x;
    xmedia_u16                         center_r_y;
    xmedia_u16                         center_gr_x;
    xmedia_u16                         center_gr_y;
    xmedia_u16                         center_gb_x;
    xmedia_u16                         center_gb_y;
    xmedia_u16                         center_b_x;
    xmedia_u16                         center_b_y;
    xmedia_u16                         off_center_r;
    xmedia_u16                         off_center_gr;
    xmedia_u16                         off_center_gb;
    xmedia_u16                         off_center_b;
    xmedia_isp_radial_shading_gain_lut r_lsc_gain_lut[XMEDIA_ISP_RLSC_GAIN_LUT_NUM];
} xmedia_isp_radial_shading_lut_attr;

typedef enum {
    XMEDIA_ISP_IRBAYER_FORMAT_GRGBI = 0,
    XMEDIA_ISP_IRBAYER_FORMAT_RGBGI,
    XMEDIA_ISP_IRBAYER_FORMAT_GBGRI,
    XMEDIA_ISP_IRBAYER_FORMAT_BGRGI,
    XMEDIA_ISP_IRBAYER_FORMAT_IGRGB,
    XMEDIA_ISP_IRBAYER_FORMAT_IRGBG,
    XMEDIA_ISP_IRBAYER_FORMAT_IBGRG,
    XMEDIA_ISP_IRBAYER_FORMAT_IGBGR,
    XMEDIA_ISP_IRBAYER_FORMAT_MAX
} xmedia_isp_irbayer_format;

typedef enum {
    XMEDIA_ISP_IR_CVTMAT_MODE_NORMAL = 0,
    XMEDIA_ISP_IR_CVTMAT_MODE_MONO,
    XMEDIA_ISP_IR_CVTMAT_MODE_USER,
    XMEDIA_ISP_IR_CVTMAT_MODE_MAX
} xmedia_isp_ir_cvtmat_mode;

#define XMEDIA_ISP_EXP_CTRL_NUM 2
#define XMEDIA_ISP_CVTMAT_NUM   12

typedef struct {
    xmedia_bool               enable;
    xmedia_isp_irbayer_format in_pattern;
    xmedia_video_bayer_format out_pattern;
    xmedia_u16                exp_ctrl[XMEDIA_ISP_EXP_CTRL_NUM];
    xmedia_u16                gain[XMEDIA_ISP_EXP_CTRL_NUM];
    xmedia_isp_ir_cvtmat_mode ir_cvt_mat_mode;
    xmedia_s16                cvt_matrix[XMEDIA_ISP_CVTMAT_NUM];
} xmedia_isp_rgbir_attr;

typedef enum {
    XMEDIA_ISP_AE_SWITCH_AFTER_DGN2 = 0,
    XMEDIA_ISP_AE_SWITCH_AFTER_DGN3,
    XMEDIA_ISP_AE_SWITCH_AFTER_DRC,
    XMEDIA_ISP_AE_SWITCH_AFTER_MAX,
} xmedia_isp_ae_switch;

typedef enum {
    XMEDIA_ISP_AE_STAT_MODE_NORMAL = 0,
    XMEDIA_ISP_AE_STAT_MODE_ROOT,
    XMEDIA_ISP_AE_STAT_MODE_MAX,
} xmedia_isp_ae_stat_mode;

typedef enum {
    XMEDIA_ISP_AE_HIST_OFFSET_ONE = 0, // 从第一行/列开始统计
    XMEDIA_ISP_AE_HIST_OFFSET_TWO,     // 从第二行/列开始统计
    XMEDIA_ISP_AE_HIST_OFFSET_MAX,
} xmedia_isp_ae_hist_offset;

typedef enum {
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_PIXEL = 0,
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_2ND_PIXEL,
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_3RD_PIXEL,
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_4TH_PIXEL,
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_5TH_PIXEL,
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_8TH_PIXEL,
    XMEDIA_ISP_AE_HIST_SKIP_EVERY_9TH_PIXEL,
    XMEDIA_ISP_AE_HIST_SKIP_MAX,
} xmedia_isp_ae_hist_skip;

typedef struct {
    xmedia_isp_ae_hist_skip   hist_skip_x;
    xmedia_isp_ae_hist_skip   hist_skip_y;
    xmedia_isp_ae_hist_offset hist_offset_x;
    xmedia_isp_ae_hist_offset hist_offset_y;
} xmedia_isp_ae_hist_config;

typedef enum {
    XMEDIA_ISP_AE_SINGLE_COLOR_BIN_R = 0,
    XMEDIA_ISP_AE_SINGLE_COLOR_BIN_GR,
    XMEDIA_ISP_AE_SINGLE_COLOR_BIN_B,
    XMEDIA_ISP_AE_SINGLE_COLOR_BIN_GB,
    XMEDIA_ISP_AE_SINGLE_COLOR_BIN_MAX,
} xmedia_isp_ae_single_color_bin;

typedef enum {
    XMEDIA_ISP_AE_HIST_FOUR_MODE_EVERY_COLOR = 0,
    XMEDIA_ISP_AE_HIST_FOUR_MODE_SINGLE_COLOR,
    XMEDIA_ISP_AE_HIST_FOUR_MODE_FOUR_PLANE,
    XMEDIA_ISP_AE_HIST_FOUR_MODE_MAX,
} xmedia_isp_ae_hist_four_mode;

typedef enum {
    XMEDIA_ISP_AE_HIST_MODE_NO_WEIGHT = 1,
    XMEDIA_ISP_AE_HIST_MODE_WEIGHT,
    XMEDIA_ISP_AE_HIST_MODE_MAX,
} xmedia_isp_ae_hist_mode;

typedef struct {
    xmedia_bool enable;
    xmedia_u16  x;
    xmedia_u16  y;
    xmedia_u16  w;
    xmedia_u16  h;
} xmedia_isp_ae_crop;

typedef struct {
    xmedia_bool                    global_avg_en;
    xmedia_bool                    zone_avg_en;
    xmedia_bool                    hist_en;
    xmedia_isp_ae_switch           pos_switch;
    xmedia_isp_ae_hist_mode        hist_mode;
    xmedia_isp_ae_hist_four_mode   hist_four_mode;
    xmedia_isp_ae_hist_config      hist_config;
    xmedia_isp_ae_single_color_bin single_color_bin;
    xmedia_isp_ae_stat_mode        hist_root_mode;
    xmedia_isp_ae_stat_mode        aver_root_mode;
    xmedia_isp_ae_crop             crop;
    xmedia_u16                     h_zone_num;
    xmedia_u16                     v_zone_num;
    xmedia_u8                      weight[XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM];
} xmedia_isp_ae_stat_cfg;

typedef struct {
    xmedia_isp_ae_stat_cfg fe_cfg;
    xmedia_isp_ae_stat_cfg be_cfg;
} xmedia_isp_ae_statistics_cfg;

typedef enum {
    XMEDIA_ISP_AWB_SWITCH_AFTER_EXPANDER = 0,
    XMEDIA_ISP_AWB_SWITCH_AFTER_DG2,
    XMEDIA_ISP_AWB_SWITCH_AFTER_DRC,
    XMEDIA_ISP_AWB_SWITCH_AFTER_MAX,
} xmedia_isp_awb_switch;

typedef struct {
    xmedia_bool enable;
    xmedia_u16  x;
    xmedia_u16  y;
    xmedia_u16  w;
    xmedia_u16  h;
} xmedia_isp_awb_crop;

#define XMEDIA_ISP_AWB_PSEUDO_WEIGHT_NUM 4

typedef struct {
    xmedia_bool           global_avg_en;
    xmedia_bool           zone_avg_en;
    xmedia_isp_awb_switch pos_switch;
    xmedia_u16            h_zone_num;
    xmedia_u16            v_zone_num;
    xmedia_u16            white_level;
    xmedia_u16            black_level;
    xmedia_u16            cb_max;
    xmedia_u16            cb_min;
    xmedia_u16            cr_max;
    xmedia_u16            cr_min;
    xmedia_isp_awb_crop   crop;
    xmedia_u8             pseudo_wgt[XMEDIA_ISP_AWB_PSEUDO_WEIGHT_NUM];
} xmedia_isp_awb_statistics_cfg;

typedef enum {
    XMEDIA_ISP_AF_AFTER_DGN2 = 0,
    XMEDIA_ISP_AF_AFTER_DRC,
    XMEDIA_ISP_AF_AFTER_CSC,
    XMEDIA_ISP_AF_AFTER_MAX,
} xmedia_isp_af_switch;

typedef struct {
    xmedia_bool enable;
    xmedia_u16  x;
    xmedia_u16  y;
    xmedia_u16  w;
    xmedia_u16  h;
} xmedia_isp_af_crop;

#define XMEDIA_ISP_AF_TONE_CURVE_NUM 32

typedef struct {
    xmedia_u16 tone_curve[XMEDIA_ISP_AF_TONE_CURVE_NUM];
} xmedia_isp_af_raw_cfg;

typedef enum {
    XMEDIA_ISP_AF_FILTER_MODE_NORM = 0,
    XMEDIA_ISP_AF_FILTER_MODE_PEAK,
    XMEDIA_ISP_AF_FILTER_MODE_MAX,
} xmedia_isp_af_filter_mode;

#define XMEDIA_ISP_AF_GAIN_CURVE_NUM 16

typedef struct {
    xmedia_u8 gain_curve[XMEDIA_ISP_AF_GAIN_CURVE_NUM];
} xmedia_isp_af_ld;

typedef struct {
    xmedia_s16 coring_en;
    xmedia_s16 coring_th;
    xmedia_s16 coring_slp;
    xmedia_s16 coring_lmt;
} xmedia_isp_af_coring;

#define XMEDIA_ISP_AF_IIR_EN_NUM   3
#define XMEDIA_ISP_AF_IIR_GAIN_NUM 4

#define XMEDIA_ISP_AF_IIR_FORWARD_COEF_NUM  9
#define XMEDIA_ISP_AF_IIR_BACKWARD_COEF_NUM 6

typedef struct {
    xmedia_bool               iir_en[XMEDIA_ISP_AF_IIR_EN_NUM];
    xmedia_s16                bias_offset;
    xmedia_u16                min_thd;
    xmedia_s16                iir_gain[XMEDIA_ISP_AF_IIR_GAIN_NUM];
    xmedia_s16                forward_coeff[XMEDIA_ISP_AF_IIR_FORWARD_COEF_NUM];
    xmedia_s16                backward_coeff[XMEDIA_ISP_AF_IIR_BACKWARD_COEF_NUM];
    xmedia_isp_af_filter_mode filter_mode;
    xmedia_isp_af_coring      coring;
    xmedia_isp_af_ld          ld;
} xmedia_isp_af_h_param;

#define XMEDIA_ISP_AF_FILTER_WEIGHT_NUM 2

typedef struct {
    xmedia_bool           pdaf_stat_en;
    xmedia_bool           cdaf_stat_en;
    xmedia_isp_af_switch  pos_switch;
    xmedia_u16            h_zone_num;
    xmedia_u16            v_zone_num;
    xmedia_u16            h_size;
    xmedia_u16            v_size;
    xmedia_u16            hlt_thd;
    xmedia_u8             h_iir_wgt[XMEDIA_ISP_AF_FILTER_WEIGHT_NUM];
    xmedia_u8             v_fir_wgt[XMEDIA_ISP_AF_FILTER_WEIGHT_NUM];
    xmedia_s16            pre_filter_en;
    xmedia_isp_af_crop    crop;
    xmedia_isp_af_raw_cfg raw_cfg;
} xmedia_isp_af_cfg;

#define XMEDIA_ISP_AF_FIR_GAIN_NUM 3

typedef struct {
    xmedia_u16           min_thd;
    xmedia_s8            fir_gain[XMEDIA_ISP_AF_FIR_GAIN_NUM];
    xmedia_isp_af_coring coring;
    xmedia_isp_af_ld     ld;
} xmedia_isp_af_v_param;

typedef struct {
    xmedia_u16 shift_y;
    xmedia_u16 shift_fv1;
    xmedia_u16 shift_fv2;
    xmedia_u16 shift_hlt_cnt;
} xmedia_isp_fv_param;

typedef struct {
    xmedia_isp_af_cfg     config;
    xmedia_isp_af_h_param h_filter0;
    xmedia_isp_af_h_param h_filter1;
    xmedia_isp_af_v_param v_filter0;
    xmedia_isp_af_v_param v_filter1;
    xmedia_isp_fv_param   fv_param;
} xmedia_isp_af_statistics_cfg;

typedef struct {
    xmedia_isp_ae_statistics_cfg  ae_cfg;
    xmedia_isp_awb_statistics_cfg awb_cfg;
    xmedia_isp_af_statistics_cfg  af_cfg;
} xmedia_isp_statistics_cfg;

typedef struct {
    xmedia_u16 grid_y_pos[XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM + 1]; // AE 统计信息 Y 方向坐标值
    xmedia_u16 grid_x_pos[XMEDIA_ISP_AE_ZONE_COL_MAX_NUM + 1]; // AE 统计信息 X 方向坐标值
    xmedia_u8  status;                                         // AE 统计信息坐标更新状态
} xmedia_isp_ae_grid_info;

typedef struct {
    xmedia_u32 fe_zone_avg[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM]
                          [XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 fe_global_avg[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 fe_hist_value[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_HIST_NUM];

    xmedia_u32 be_zone_avg[XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM]
                          [XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 be_global_avg[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 be_hist_value[XMEDIA_ISP_AE_HIST_NUM];

    xmedia_u32 fe_hist_weight_pixel_num[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_u32 be_hist_weight_pixel_num;

    xmedia_isp_ae_grid_info fe_grid_info;
    xmedia_isp_ae_grid_info be_grid_info;
} xmedia_isp_ae_statistics;

#define XMEDIA_ISP_BLEND_MAX_NUM 4

typedef struct {
    // 按pipe号升序存储zone_avg
    xmedia_u32 fe_zone_avg[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM]
                          [XMEDIA_ISP_AE_ZONE_COL_MAX_NUM][XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 fe_global_avg[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 fe_hist_value[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_HIST_NUM];

    // 按pipe号升序存储zone_avg
    xmedia_u32 be_zone_avg[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM]
                          [XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 be_global_avg[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u32 be_hist_value[XMEDIA_ISP_AE_HIST_NUM];

    xmedia_u32 fe_hist_weight_pixel_num[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_u32 be_hist_weight_pixel_num;
} xmedia_isp_ae_blend_statistics;

typedef struct {
    xmedia_u16 grid_y_pos[XMEDIA_ISP_AWB_ZONE_ROW_MAX_NUM + 1];
    xmedia_u16 grid_x_pos[XMEDIA_ISP_AWB_ZONE_COL_MAX_NUM + 1];
    xmedia_u8  status;
} xmedia_isp_awb_grid_info;

typedef struct {
    xmedia_u32 global_r;
    xmedia_u32 global_g;
    xmedia_u32 global_b;
    xmedia_u32 global_valid_count; // 全局有效像素个数
    xmedia_u32 zone_avg_r[XMEDIA_ISP_AWB_ZONE_NUM];
    xmedia_u32 zone_avg_g[XMEDIA_ISP_AWB_ZONE_NUM];
    xmedia_u32 zone_avg_b[XMEDIA_ISP_AWB_ZONE_NUM];
    xmedia_u32 zone_valid_count[XMEDIA_ISP_AWB_ZONE_NUM]; // 分块有效像素个数

    xmedia_isp_awb_grid_info grid_info;
} xmedia_isp_awb_statistics;

#define XMEDIA_ISP_AWB_ZONE_BLEND_MAX (XMEDIA_ISP_AWB_ZONE_NUM * XMEDIA_ISP_BLEND_MAX_NUM)

typedef struct {
    xmedia_u32 zone_row[XMEDIA_ISP_BLEND_MAX_NUM];
    xmedia_u32 zone_col[XMEDIA_ISP_BLEND_MAX_NUM];
    xmedia_u32 zone_avg_r[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_AWB_ZONE_NUM];
    xmedia_u32 zone_avg_g[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_AWB_ZONE_NUM];
    xmedia_u32 zone_avg_b[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_AWB_ZONE_NUM];
    xmedia_u32 zone_valid_count[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_AWB_ZONE_NUM]; // 分块有效像素个数
} xmedia_isp_awb_blend_statistics;

typedef struct {
    xmedia_u32 fv1_value;
    xmedia_u32 fv2_value;
    xmedia_u32 pseduoy_value;
    xmedia_u32 hlt_cnt;
} xmedia_isp_af_zone;

typedef struct {
    xmedia_u16 grid_y_pos[XMEDIA_ISP_AF_ZONE_ROW_MAX_NUM + 1];
    xmedia_u16 grid_x_pos[XMEDIA_ISP_AF_ZONE_COL_MAX_NUM + 1];
    xmedia_u8  status;
} xmedia_isp_af_grid_info;

typedef struct {
    xmedia_isp_af_zone zone_metrics[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AF_ZONE_ROW_MAX_NUM]
                                   [XMEDIA_ISP_AF_ZONE_COL_MAX_NUM];
    xmedia_isp_af_grid_info grid_info;
} xmedia_isp_fe_af_statistics;

typedef struct {
    xmedia_isp_af_zone      zone_metrics[XMEDIA_ISP_AF_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AF_ZONE_COL_MAX_NUM];
    xmedia_isp_af_grid_info grid_info;
} xmedia_isp_be_af_statistics;

typedef struct {
    xmedia_isp_fe_af_statistics fe_stat;
    xmedia_isp_be_af_statistics be_stat;
} xmedia_isp_af_statistics;

typedef enum {
    XMEDIA_ISP_AE_ACCURACY_TYPE_DB = 0,
    XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR,
    XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE,
    XMEDIA_ISP_AE_ACCURACY_TYPE_MAX,
} xmedia_isp_ae_accuracy_type;

typedef struct {
    xmedia_isp_ae_accuracy_type accu_type;
    xmedia_float                accuracy;
    xmedia_float                offset;
} xmedia_isp_ae_accuracy;

typedef enum {
    XMEDIA_ISP_IRIS_F_NO_32_0 = 0,
    XMEDIA_ISP_IRIS_F_NO_22_0,
    XMEDIA_ISP_IRIS_F_NO_16_0,
    XMEDIA_ISP_IRIS_F_NO_11_0,
    XMEDIA_ISP_IRIS_F_NO_8_0,
    XMEDIA_ISP_IRIS_F_NO_5_6,
    XMEDIA_ISP_IRIS_F_NO_4_0,
    XMEDIA_ISP_IRIS_F_NO_2_8,
    XMEDIA_ISP_IRIS_F_NO_2_0,
    XMEDIA_ISP_IRIS_F_NO_1_4,
    XMEDIA_ISP_IRIS_F_NO_1_0,
    XMEDIA_ISP_IRIS_F_NO_MAX
} xmedia_isp_iris_f_no;

typedef struct {
    xmedia_u32           int_time;
    xmedia_u32           sys_gain;
    xmedia_isp_iris_f_no iris_f_no;
    xmedia_u32           iris_f_no_lin;
} xmedia_isp_ae_route_node;

#define XMEDIA_ISP_AE_ROUTE_MAX_NODES 16

typedef struct {
    xmedia_u32               total_num;
    xmedia_isp_ae_route_node route_node[XMEDIA_ISP_AE_ROUTE_MAX_NODES];
} xmedia_isp_ae_route;

typedef struct {
    xmedia_u32           int_time;
    xmedia_u32           again;
    xmedia_u32           dgain;
    xmedia_u32           ispgain;
    xmedia_isp_iris_f_no iris_f_no;
    xmedia_u32           iris_f_no_lin;
} xmedia_isp_ae_ext_route_node;

#define XMEDIA_ISP_AE_EXT_ROUTE_MAX_NODES 16

typedef struct {
    xmedia_u32                   total_num;
    xmedia_isp_ae_ext_route_node route_ex_node[XMEDIA_ISP_AE_EXT_ROUTE_MAX_NODES];
} xmedia_isp_ae_ext_route;

typedef enum {
    XMEDIA_ISP_IRIS_TYPE_DC = 0,
    XMEDIA_ISP_IRIS_TYPE_P,
    XMEDIA_ISP_IRIS_TYPE_MAX
} xmedia_isp_iris_type; // TODO: 该数据结构不该定义在此处,应该定义在iris的头文件里

#define XMEDIA_ISP_PIRIS_AI_MAX_STEP_FNO_NUM 1024

typedef struct {
    xmedia_bool step_f_no_table_change;
    xmedia_bool zero_is_max;
    xmedia_u16  total_step;
    xmedia_u16  step_count;

    xmedia_u16           step_fno_table[XMEDIA_ISP_PIRIS_AI_MAX_STEP_FNO_NUM];
    xmedia_isp_iris_f_no max_iris_f_no_target;
    xmedia_isp_iris_f_no min_iris_f_no_target;

    xmedia_bool f_no_ex_valid;
    xmedia_u32  max_iris_fno_target;
    xmedia_u32  min_iris_fno_target;
} xmedia_isp_piris_attr; // TODO: 该数据结构不该定义在此处,应该定义在iris的头文件里

typedef enum {
    XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR = 0,
    XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR  = 1,
    XMEDIA_ISP_AE_STRATEGY_MAX
} xmedia_isp_ae_strategy;

#define XMEDIA_ISP_AE_EXP_RATIO_NUM 3

typedef struct {
    xmedia_u8               ae_compensation;
    xmedia_u32              lines_per_500ms;
    xmedia_u32              flicker_freq;
    xmedia_float            fps;
    xmedia_u32              hmax_times;
    xmedia_u32              init_exposure; // gain * 0x40 * exp_time (unit: line)
    xmedia_u32              init_ae_speed;
    xmedia_u32              init_ae_tolerance;
    xmedia_u32              full_lines_std;
    xmedia_u32              full_lines_max;
    xmedia_u32              full_lines;
    xmedia_u32              max_int_time;
    xmedia_u32              min_int_time;
    xmedia_u32              max_int_time_target;
    xmedia_u32              min_int_time_target;
    xmedia_isp_ae_accuracy  int_time_accu;
    xmedia_u32              max_again;
    xmedia_u32              min_again;
    xmedia_u32              max_again_target;
    xmedia_u32              min_again_target;
    xmedia_isp_ae_accuracy  again_accu;
    xmedia_u32              max_dgain;
    xmedia_u32              min_dgain;
    xmedia_u32              max_dgain_target;
    xmedia_u32              min_dgain_target;
    xmedia_isp_ae_accuracy  dgain_accu;
    xmedia_u32              ispdgain_sep_thd;
    xmedia_u32              max_ispdgain_target;
    xmedia_u32              min_ispdgain_target;
    xmedia_u32              ispdgain_shift;
    xmedia_u32              max_int_time_step;
    xmedia_u32              lf_max_short_time;
    xmedia_u32              lf_min_exposure;
    xmedia_isp_ae_route     ae_route_attr;
    xmedia_bool             ae_route_ex_valid;
    xmedia_isp_ae_ext_route ae_ext_route_attr;
    xmedia_isp_ae_route     ae_route_sf_attr;
    xmedia_isp_ae_ext_route ae_ext_route_sf_attr;
    xmedia_u16              man_ratio_enable;
    xmedia_u32              ratio[XMEDIA_ISP_AE_EXP_RATIO_NUM];
    xmedia_isp_iris_type    iris_type;
    xmedia_isp_piris_attr   piris_attr;
    xmedia_isp_iris_f_no    max_iris_f_no;
    xmedia_isp_iris_f_no    min_iris_f_no;
    xmedia_isp_ae_strategy  ae_exp_mode;
    xmedia_u16              iso_cal_coef;
    xmedia_u8               ae_run_interval;
    xmedia_u32              exp_ratio_max;
    xmedia_u32              exp_ratio_min;
    xmedia_bool             diff_gain_support;
} xmedia_isp_ae_sensor_default;

typedef struct {
    xmedia_bool blend_en;
    xmedia_u32  blend_num;
} xmedia_isp_3a_blend_config;

typedef struct {
    xmedia_s32                 sensor_id;
    xmedia_u8                  wdr_mode;
    xmedia_u8                  hdr_mode;
    xmedia_u16                 balck_level[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_float               fps;
    xmedia_video_bayer_format  bayer_format;
    xmedia_isp_3a_blend_config blend_config;
    xmedia_s32                 reserve;
} xmedia_isp_ae_param;

typedef struct {
    xmedia_bool change;
    xmedia_bool thresh_update;
    xmedia_u16  zone_max_thresh;
    xmedia_u16  zone_min_thresh;

    xmedia_bool                    hist_adjust_update;
    xmedia_isp_ae_switch           pos_switch;
    xmedia_isp_ae_hist_mode        hist_mode;
    xmedia_isp_ae_hist_four_mode   hist_four_mode;
    xmedia_isp_ae_single_color_bin single_color_bin;

    xmedia_bool             mode_update;
    xmedia_isp_ae_stat_mode hist_root_mode;
    xmedia_isp_ae_stat_mode aver_root_mode;

    xmedia_bool weight_table_update;
    xmedia_u8   weight_table[XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM];
} xmedia_isp_ae_stat_attr;

typedef struct {
    xmedia_u32 int_time[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_u32 isp_dgain;
    xmedia_u32 again;
    xmedia_u32 dgain;
    xmedia_u32 iso;
    xmedia_u32 isp_dgain_short_frame;
    xmedia_u32 again_short_frame;
    xmedia_u32 dgain_short_frame;
    xmedia_u32 iso_short_frame;
    xmedia_u32 isp_dgain_sep_thd;
    xmedia_u8  ae_run_interval;

    xmedia_bool piris_valid;
    xmedia_s32  piris_pos;
    xmedia_u32  piris_gain;

    xmedia_isp_wdr_mode wdr_mode;
    xmedia_u32          max_sensor_again;
    xmedia_u32          wdr_gain[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_u32          hmax_times;
    xmedia_u32          vmax;

    xmedia_isp_ae_stat_attr stat_attr;
    // xmedia_isp_dcf_info     dcf_update_info; // xmedia_isp_dcf_info未定义，解决编译问题，先注释掉
} xmedia_isp_ae_result;

typedef struct {
    xmedia_u32 histogram_mem_array[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_HIST_NUM];
} xmedia_isp_fe_hist;

typedef struct {
    xmedia_u32 histogram_mem_array[XMEDIA_ISP_AE_HIST_NUM];
} xmedia_isp_be_hist;

typedef struct {
    xmedia_u32          fe_pixel_weight[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_isp_fe_hist *fe_histogram_mem_array;
} xmedia_isp_fe_ae_stat_hist;

typedef struct {
    xmedia_u32 global_avg[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_fe_ae_stat_global_avg;

typedef struct {
    xmedia_u32 zone_avg[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM]
                       [XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_fe_ae_stat_zone_avg;

typedef struct {
    xmedia_u32 zone_avg[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM]
                       [XMEDIA_ISP_AE_ZONE_COL_MAX_NUM][XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_fe_ae_blend_stat;

typedef struct {
    xmedia_u32          be_pixel_weight;
    xmedia_isp_be_hist *be_histogram_mem_array;
} xmedia_isp_be_ae_stat_hist;

typedef struct {
    xmedia_u32 global_avg[XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_be_ae_stat_global_avg;

typedef struct {
    xmedia_u32 zone_avg[XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM][XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_be_ae_stat_zone_avg;

typedef struct {
    xmedia_u32 zone_avg[XMEDIA_ISP_BLEND_MAX_NUM][XMEDIA_ISP_AE_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AE_ZONE_COL_MAX_NUM]
                       [XMEDIA_ISP_BAYER_PATTERN_NUM];
} xmedia_isp_be_ae_blend_stat;

typedef struct {
    xmedia_u64                        frame_cnt;
    xmedia_isp_smart_info             smart_info;
    xmedia_u16                        fe_ae_row_num;
    xmedia_u16                        fe_ae_col_num;
    xmedia_isp_fe_ae_stat_hist        fe_ae_stat_hist;
    xmedia_isp_fe_ae_stat_global_avg *fe_ae_stat_global_avg;
    xmedia_isp_fe_ae_stat_zone_avg   *fe_ae_stat_zone_avg;
    xmedia_isp_fe_ae_blend_stat      *fe_ae_blend_stat;

    xmedia_u16                        be_ae_row_num;
    xmedia_u16                        be_ae_col_num;
    xmedia_isp_be_ae_stat_hist        be_ae_stat_hist;
    xmedia_isp_be_ae_stat_global_avg *be_ae_stat_global_avg;
    xmedia_isp_be_ae_stat_zone_avg   *be_ae_stat_zone_avg;
    xmedia_isp_be_ae_blend_stat      *be_ae_blend_stat;
} xmedia_isp_ae_info;

#define XMEDIA_ISP_ALG_NAME_MAX_SIZE 20

typedef struct {
    xmedia_char alg_name[XMEDIA_ISP_ALG_NAME_MAX_SIZE];
    xmedia_u32  alg_id; // 数据流pipe使用的ae库ch id.
    xmedia_s32 (*pfn_ae_init)(xmedia_u32 channel, const xmedia_isp_ae_param *ae_param);
    xmedia_s32 (*pfn_ae_run)(xmedia_u32 channel, const xmedia_isp_ae_info *ae_info, xmedia_isp_ae_result *ae_result);
    xmedia_s32 (*pfn_ae_ctrl)(xmedia_u32 channel, xmedia_u32 cmd, xmedia_void *value);
    xmedia_s32 (*pfn_ae_exit)(xmedia_u32 channel);
} xmedia_isp_ae_register_info;

#define XMEDIA_ISP_AUTO_ISO_STRENGTH_NUM 16

typedef struct {
    xmedia_bool valid;
    xmedia_u8   saturation[XMEDIA_ISP_AUTO_ISO_STRENGTH_NUM];
} xmedia_isp_awb_agc_table;

#define XMEDIA_ISP_CCM_MATRIX_SIZE 9

typedef struct {
    // RW; Range:[2000,10000]; Format:16.0; the current color temperature
    xmedia_u16 ct;
    // RW; Range: [0x0, 0xFFFF]; Format:8.8; CCM matrixes for different color temperature
    xmedia_u16 ccm[XMEDIA_ISP_CCM_MATRIX_SIZE];
} xmedia_isp_awb_ccm_table;

#define XMEDIA_ISP_CCM_TAB_MAX_NUM 7

typedef struct {
    xmedia_u16               ccm_tab_num;
    xmedia_isp_awb_ccm_table ccm_tab[XMEDIA_ISP_CCM_TAB_MAX_NUM];
} xmedia_isp_awb_ccm;

#define XMEDIA_ISP_AWB_CURVE_PARA_NUM 6

typedef struct {
    xmedia_u16                  wb_ref_temp;
    xmedia_u16                  gain_offset[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_s32                  wb_para[XMEDIA_ISP_AWB_CURVE_PARA_NUM];
    xmedia_u16                  golden_rgain;
    xmedia_u16                  golden_bgain;
    xmedia_u16                  sample_rgain;
    xmedia_u16                  sample_bgain;
    xmedia_isp_awb_agc_table    agc_tbl;
    xmedia_isp_awb_ccm          ccm;
    xmedia_u16                  init_rgain;
    xmedia_u16                  init_ggain;
    xmedia_u16                  init_bgain;
    xmedia_u8                   awb_runinterval;
    xmedia_u16                  init_ccm[XMEDIA_ISP_CCM_MATRIX_SIZE];
    xmedia_video_operation_mode wb_op_type;
    xmedia_video_operation_mode ccm_op_type;
    xmedia_video_operation_mode sat_op_type;
} xmedia_isp_awb_sensor_default;

typedef struct {
    xmedia_s32 sensor_id;
    xmedia_u8  wdr_mode;
    xmedia_u8  awb_zone_row;
    xmedia_u8  awb_zone_col;
    xmedia_u8  awb_zone_bin;
    xmedia_u16 awb_width;
    xmedia_u16 awb_height;
    xmedia_u32 init_iso;

    xmedia_isp_3a_blend_config blend_config;
} xmedia_isp_awb_param;

typedef struct {
    xmedia_u32 global_r;
    xmedia_u32 global_g;
    xmedia_u32 global_b;
    xmedia_u32 global_valid_count; // 全局有效像素个数
} xmedia_isp_awb_stat_global;

typedef struct {
    xmedia_u32 zone_avg_r[XMEDIA_ISP_AWB_ZONE_NUM];
} xmedia_isp_awb_stat_local_avg_r;

typedef struct {
    xmedia_u32 zone_avg_g[XMEDIA_ISP_AWB_ZONE_NUM];
} xmedia_isp_awb_stat_local_avg_g;

typedef struct {
    xmedia_u32 zone_avg_b[XMEDIA_ISP_AWB_ZONE_NUM];
} xmedia_isp_awb_stat_local_avg_b;

typedef struct {
    xmedia_u32 zone_valid_count[XMEDIA_ISP_AWB_ZONE_NUM]; // 分块有效像素个数
} xmedia_isp_awb_stat_local_valid_count;

typedef xmedia_isp_awb_blend_statistics xmedia_isp_awb_blend_stat;

typedef struct {
    xmedia_u32                             frame_cnt;
    xmedia_isp_awb_stat_global             awb_stat_global;
    xmedia_isp_awb_stat_local_avg_r       *awb_stat_local_avg_r;
    xmedia_isp_awb_stat_local_avg_g       *awb_stat_local_avg_g;
    xmedia_isp_awb_stat_local_avg_b       *awb_stat_local_avg_b;
    xmedia_isp_awb_stat_local_valid_count *stat_local_valid_count;
    xmedia_isp_awb_blend_stat             *awb_blend_stat_local;

    xmedia_u8  awb_gain_switch;
    xmedia_u8  awb_stat_switch;
    xmedia_u32 wdr_wb_gain[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u16 awb_zone_row;
    xmedia_u16 awb_zone_col;
    xmedia_u16 awb_width;
    xmedia_u16 awb_height;
} xmedia_isp_awb_info;

typedef struct {
    xmedia_bool change;

    xmedia_bool black_white_update;
    xmedia_u16  metering_white_level_awb;
    xmedia_u16  metering_black_level_awb;

    xmedia_bool cbcr_update;
    xmedia_u16  metering_crref_max_awb;
    xmedia_u16  metering_cbref_max_awb;
    xmedia_u16  metering_crref_min_awb;
    xmedia_u16  metering_cbref_min_awb;

    xmedia_bool           pos_update;
    xmedia_isp_awb_switch pos_switch;
} xmedia_isp_awb_raw_stat_attr;

#define XMEDIA_ISP_COLOR_SECTOR_NUM 6

typedef struct {
    xmedia_u32                   white_balance_gain[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_s16                   color_matrix[XMEDIA_ISP_CCM_MATRIX_SIZE];
    xmedia_u32                   ct;
    xmedia_u8                    saturation;
    xmedia_bool                  ccm_en;
    xmedia_bool                  ccm_hue_en;
    xmedia_s16                   ccm_hue[XMEDIA_ISP_COLOR_SECTOR_NUM];
    xmedia_s16                   sector_matrix[XMEDIA_ISP_COLOR_SECTOR_NUM][XMEDIA_ISP_CCM_MATRIX_SIZE];
    xmedia_isp_awb_raw_stat_attr raw_stat_attr;
} xmedia_isp_awb_result;

typedef struct {
    xmedia_char alg_name[XMEDIA_ISP_ALG_NAME_MAX_SIZE];
    xmedia_u32  alg_id; // 数据流pipe使用的awb库ch id.
    xmedia_s32 (*pfn_awb_init)(xmedia_u32 channel, const xmedia_isp_awb_param *awb_param);
    xmedia_s32 (*pfn_awb_run)(xmedia_u32 channel, const xmedia_isp_awb_info *awb_info,
                              xmedia_isp_awb_result *awb_result);
    xmedia_s32 (*pfn_awb_ctrl)(xmedia_u32 channel, xmedia_u32 cmd, xmedia_void *value);
    xmedia_s32 (*pfn_awb_exit)(xmedia_u32 channel);
} xmedia_isp_awb_register_info;

typedef struct {
    xmedia_bool       pdaf_en;
    xmedia_bool       cdaf_en;
    xmedia_video_size image_size;
} xmedia_isp_af_param;

typedef struct {
    xmedia_isp_af_zone zone_matrix[XMEDIA_ISP_WDR_CHN_MAX_NUM][XMEDIA_ISP_AF_ZONE_ROW_MAX_NUM]
                                  [XMEDIA_ISP_AF_ZONE_COL_MAX_NUM];
} xmedia_isp_fe_af_zone_matrix;

typedef struct {
    xmedia_isp_af_zone zone_matrix[XMEDIA_ISP_AF_ZONE_ROW_MAX_NUM][XMEDIA_ISP_AF_ZONE_COL_MAX_NUM];
} xmedia_isp_be_af_zone_matrix;

typedef struct {
    xmedia_u64   stat_frame_cnt;
    xmedia_u32   width;
    xmedia_u32   height;
    xmedia_u32   bit_width;
    xmedia_void *pd_stat;
} xmedia_isp_af_pdaf_statistics;

typedef struct {
    xmedia_u64                          frame_cnt;
    xmedia_video_rect                   roi;
    xmedia_isp_af_pdaf_statistics       pdaf_stat;
    const xmedia_isp_fe_af_zone_matrix *fe_zone_matrix;
    const xmedia_isp_be_af_zone_matrix *be_zone_matrix;
} xmedia_isp_af_info;

typedef struct {
    xmedia_bool       pdaf_en;
    xmedia_bool       cdaf_en;
    xmedia_bool       roi_update;
    xmedia_video_rect roi;
} xmedia_isp_af_result;

typedef struct {
    xmedia_char alg_name[XMEDIA_ISP_ALG_NAME_MAX_SIZE];
    xmedia_u32  alg_id; // 数据流pipe使用的af库ch id.
    xmedia_s32 (*pfn_af_init)(xmedia_u32 channel, const xmedia_isp_af_param *af_param, xmedia_isp_af_result *af_result);
    xmedia_s32 (*pfn_af_run)(xmedia_u32 channel, const xmedia_isp_af_info *af_info, xmedia_isp_af_result *af_result);
    xmedia_s32 (*pfn_af_ctrl)(xmedia_u32 channel, xmedia_u32 cmd, xmedia_void *value);
    xmedia_s32 (*pfn_af_exit)(xmedia_u32 channel);
} xmedia_isp_af_register_info;

// slave mode sensor sync signal generate module
typedef struct {
    xmedia_u16 trig_dev;  // 触发信号设备配置通道
    xmedia_u16 trig_mode; // 工作模式, 0: 同步模式; 1: 相位模式
    xmedia_u8  hs_enable; // XHS 输出使能
    xmedia_u8  vs_enable; // XVS 输出使能
    xmedia_u8  hs_invert; // XHS 极性配置 1: 低电平有效; 0: 高电平有效
    xmedia_u8  vs_invert; // XVS 极性配置 1: 低电平有效; 0: 高电平有效
    xmedia_u32 hs_time;   // XHS 信号周期, 单位：us
    xmedia_u32 vs_time;   // XVS 信号周期, 单位：us
    xmedia_u32 hs_active; // XHS 有效电平宽度, 单位：us
    xmedia_u32 vs_active; // XVS 有效电平宽度, 单位：us
    xmedia_u32 hs_delay;  // XHS 相对 XVS 信号的延迟时间, 单位：us
    xmedia_u32 ph_offset; // 相位偏移
    xmedia_u32 delay_frame_num; // 触发信号延迟生效帧数
} xmedia_sensor_trig_attr;

typedef enum {
    XMEDIA_ISP_FMW_STATE_RUN = 0,
    XMEDIA_ISP_FMW_STATE_FREEZE,
    XMEDIA_ISP_FMW_STATE_BUTT
} xmedia_isp_fmw_state;

typedef struct {
    // RW;Range:[-4095,4095];Format:12.0; Black level differences between the offset value
    xmedia_s32 blc_offset[XMEDIA_ISP_BAYER_PATTERN_NUM];
    // RW;Range:[1,16]
    xmedia_u16 ct_num;
    // RW;Range:[0,65535]
    xmedia_u16 ct_level[XMEDIA_ISP_CT_MAX_COUNT];
    // RW;Range:[-1048575,1048575];Format:8.12; The gain difference slope
    xmedia_s32 wb_slope[XMEDIA_ISP_CT_MAX_COUNT][XMEDIA_ISP_BAYER_PATTERN_NUM];
    // RW;Range:[-1048575,1048575];Format:8.12; The gain difference offset
    xmedia_s32 wb_offset[XMEDIA_ISP_CT_MAX_COUNT][XMEDIA_ISP_BAYER_PATTERN_NUM];
    // RW;Range:[0x0,0xFFFF];Format:8.8; Color correction matrix variance ratio
    xmedia_u16 ccm[XMEDIA_ISP_CT_MAX_COUNT][XMEDIA_ISP_CCM_MATRIX_SIZE];
    // RW;Range:[-128,128];Format:8.0; luma differences
    xmedia_s16 luma_differ;
} xmedia_isp_diff_attr;

typedef enum {
    XMEDIA_ISP_WAIT_TYPE_FE_START = 0,
    XMEDIA_ISP_WAIT_TYPE_FE_END,
    XMEDIA_ISP_WAIT_TYPE_BE_END,
    XMEDIA_ISP_WAIT_TYPE_MAX
} xmedia_isp_wait_type;

// ISP debug information
typedef struct {
    xmedia_bool debug_en; // rw; 1:enable debug, 0:disable debug
    xmedia_u64  phyaddr;  // rw; phy address of debug info
    xmedia_u32  depth;    // rw; depth of debug info - the frame number
} xmedia_isp_debug_info;

/*
 * Defines the ISP WDR operating mode, Only used in LINE_WDR mode
 * 0: Normal WDR mode
 * 1: Long frame mode, only effective in LINE_WDR
 * when running in Long frame mode, module only output the long frame data
 */
typedef enum {
    XMEDIA_ISP_AE_WDR_MODE_NORMAL = 0,
    XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME,
    XMEDIA_ISP_AE_WDR_MODE_MAX
} xmedia_isp_ae_wdr_mode;

typedef enum {
    XMEDIA_ISP_DEBUG_MODE_NONE = 0,
    XMEDIA_ISP_DEBUG_MODE_DEMOSAIC_DIR,
    XMEDIA_ISP_DEBUG_MODE_DRC_DETAIL,
    XMEDIA_ISP_DEBUG_MODE_WDR_LONG_WEIGHT,
    XMEDIA_ISP_DEBUG_MODE_WDR_LONG,
    XMEDIA_ISP_DEBUG_MODE_WDR_SHORT,
    XMEDIA_ISP_DEBUG_MODE_STNR_MV,
    XMEDIA_ISP_DEBUG_MODE_SHARPEN_TEXTURE,
    XMEDIA_ISP_DEBUG_MODE_SHARPEN_EDGE,
    XMEDIA_ISP_DEBUG_MODE_SHARPEN_EDGE_WEIGHT,
    XMEDIA_ISP_DEBUG_MODE_SHARPEN_DIR_EDGE,
    XMEDIA_ISP_DEBUG_MODE_SHARPEN_DIR_MASK,
    XMEDIA_ISP_DEBUG_MODE_SHARPEN_REGION_DIVISION,
    XMEDIA_ISP_DEBUG_MODE_MAX,
} xmedia_isp_debug_mode;

// nr/sharpen算法参数
#define XMEDIA_ISP_SHARPEN_LUMA_NUM      33
#define XMEDIA_ISP_SHARPEN_GAIN_NUM      16
#define XMEDIA_ISP_SHARPEN_DERING_NUM    17
#define XMEDIA_ISP_SHARPEN_LUMA_GAIN_NUM 2

#define XMEDIA_ISP_SHARPEN_SKIN_NUM    2
#define XMEDIA_ISP_SHARPEN_RGB_NUM     3
#define XMEDIA_ISP_SHARPEN_VAR_THD_NUM 4
#define XMEDIA_ISP_SHARPEN_DIR_THD_NUM 4
#define XMEDIA_ISP_SHARPEN_VAR_STR_NUM 3
#define XMEDIA_ISP_SHARPEN_DIR_STR_NUM 3

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8 edge_luma_wgt[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_LUMA_NUM]; // 低亮至高亮时的边缘衰减系数
    xmedia_u8 tex_luma_wgt[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_LUMA_NUM]; // 低亮至高亮时的纹理衰减系数
    xmedia_u8  edge_freq[XMEDIA_ISP_ISO_MAX_COUNT];                                         // 边缘频段控制
    xmedia_u8  tex_freq[XMEDIA_ISP_ISO_MAX_COUNT];                                          // 纹理频段控制
    xmedia_u16 edge_str[XMEDIA_ISP_ISO_MAX_COUNT];                                          // 边缘整体锐化强度
    xmedia_u16 tex_str[XMEDIA_ISP_ISO_MAX_COUNT];                                           // 纹理整体锐化强度

    xmedia_s8  pos_edge_lut[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_GAIN_NUM];         // 白色强边锐化强度
    xmedia_s8  neg_edge_lut[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_GAIN_NUM];         // 黑色强边锐化强度
    xmedia_s8  pos_tex_lut[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_GAIN_NUM];          // 白色纹理锐化强度
    xmedia_s8  neg_tex_lut[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_GAIN_NUM];          // 黑色纹理锐化强度
    xmedia_u8  edge_dering_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_DERING_NUM];    // 边缘Dering强度
    xmedia_u8  tex_dering_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_DERING_NUM];     // 纹理Dering强度
    xmedia_u8  dir_dering_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_DERING_NUM]; // 带方向边缘Dering强度
    xmedia_u8  max_sharp_gain[XMEDIA_ISP_ISO_MAX_COUNT];                                // 锐化的阶段范围
    xmedia_u8  edge_coring[XMEDIA_ISP_ISO_MAX_COUNT];                                   // 边缘coring控制
    xmedia_u8  rgain[XMEDIA_ISP_ISO_MAX_COUNT];                 // 深红色区域的锐化增益控制
    xmedia_u8  ggain[XMEDIA_ISP_ISO_MAX_COUNT];                 // 绿色区域的锐化增益控制
    xmedia_u8  bgain[XMEDIA_ISP_ISO_MAX_COUNT];                 // 深蓝色区域的锐化增益控制
    xmedia_u8  skin_gain[XMEDIA_ISP_ISO_MAX_COUNT];             // 肤色区域的锐化增益控制
    xmedia_u8  global_overshoot_str[XMEDIA_ISP_ISO_MAX_COUNT];  // 全局白边抑制强度
    xmedia_u8  global_undershoot_str[XMEDIA_ISP_ISO_MAX_COUNT]; // 全局黑边抑制强度
    xmedia_u8  global_overshoot_thd[XMEDIA_ISP_ISO_MAX_COUNT];  // 全局白边阈值
    xmedia_u8  global_undershoot_thd[XMEDIA_ISP_ISO_MAX_COUNT]; // 全局黑边阈值
    xmedia_u16 region_division_sensitivity[XMEDIA_ISP_ISO_MAX_COUNT]; // 区域划分灵敏度，值小对弱纹理的划分精度高
    xmedia_u16 region_division_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_VAR_THD_NUM]; // 区域划分阈值 平坦、弱纹理、强纹理、强边缘
    xmedia_u8 region_division_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_VAR_STR_NUM]; // 不同区域的锐化强度
    xmedia_u8 dir_sensitivity[XMEDIA_ISP_ISO_MAX_COUNT];           // 方向划分灵敏度，值大划分精度高
    xmedia_u8 dir_edge_freq[XMEDIA_ISP_ISO_MAX_COUNT];             // 带方向边缘频段控制
    xmedia_u8 dir_edge_thd[XMEDIA_ISP_ISO_MAX_COUNT]
                          [XMEDIA_ISP_SHARPEN_DIR_THD_NUM]; // 带方向边缘划分阈值：无方向及纹理区、弱方向、强方向
    xmedia_u8 dir_edge_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_SHARPEN_DIR_STR_NUM]; // 带方向边缘的叠加锐化强度
    xmedia_u8 fusion_edge_max_edge[XMEDIA_ISP_ISO_MAX_COUNT]; // 融合模式下边缘区边缘层细节最大融合权重
    xmedia_u8 fusion_edge_max_flat[XMEDIA_ISP_ISO_MAX_COUNT]; // 融合模式下平坦区边缘层细节最大融合权重
    xmedia_u8 fusion_edge_thd[XMEDIA_ISP_ISO_MAX_COUNT];      // 融合模式下边缘区阈值
    xmedia_u8 fusion_flat_thd[XMEDIA_ISP_ISO_MAX_COUNT];      // 融合模式下平坦区阈值
} xmedia_isp_sharpen_auto_attr;

typedef struct {
    xmedia_u8  edge_luma_wgt[XMEDIA_ISP_SHARPEN_LUMA_NUM];        // 低亮至高亮时的边缘衰减系数
    xmedia_u8  tex_luma_wgt[XMEDIA_ISP_SHARPEN_LUMA_NUM];         // 低亮至高亮时的纹理衰减系数
    xmedia_u8  edge_freq;                                         // 边缘频段控制
    xmedia_u8  tex_freq;                                          // 纹理频段控制
    xmedia_u16 edge_str;                                          // 边缘整体锐化强度
    xmedia_u16 tex_str;                                           // 纹理整体锐化强度

    xmedia_s8  pos_edge_lut[XMEDIA_ISP_SHARPEN_GAIN_NUM];         // 白色强边锐化强度
    xmedia_s8  neg_edge_lut[XMEDIA_ISP_SHARPEN_GAIN_NUM];         // 黑色强边锐化强度
    xmedia_s8  pos_tex_lut[XMEDIA_ISP_SHARPEN_GAIN_NUM];          // 白色纹理锐化强度
    xmedia_s8  neg_tex_lut[XMEDIA_ISP_SHARPEN_GAIN_NUM];          // 黑色纹理锐化强度
    xmedia_u8  edge_dering_str[XMEDIA_ISP_SHARPEN_DERING_NUM];    // 边缘Dering强度
    xmedia_u8  tex_dering_str[XMEDIA_ISP_SHARPEN_DERING_NUM];     // 纹理Dering强度
    xmedia_u8  dir_dering_str[XMEDIA_ISP_SHARPEN_DERING_NUM];     // 带方向边缘Dering强度
    xmedia_u8  max_sharp_gain;                                    // 锐化的阶段范围
    xmedia_u8  edge_coring;                                       // 边缘coring控制
    xmedia_u8  rgain;                                             // 深红色区域的锐化增益控制
    xmedia_u8  ggain;                                             // 绿色区域的锐化增益控制
    xmedia_u8  bgain;                                             // 深蓝色区域的锐化增益控制
    xmedia_u8  skin_gain;                                         // 肤色区域的锐化增益控制
    xmedia_u8  global_overshoot_str;                              // 全局白边抑制强度
    xmedia_u8  global_undershoot_str;                             // 全局黑边抑制强度
    xmedia_u8  global_overshoot_thd;                              // 全局白边阈值
    xmedia_u8  global_undershoot_thd;                             // 全局黑边阈值
    xmedia_u16 region_division_sensitivity;                       // 区域划分灵敏度，值大划分精度高
    xmedia_u16 region_division_thd[XMEDIA_ISP_SHARPEN_VAR_THD_NUM]; // 区域划分阈值 平坦、弱纹理、强纹理、强边缘
    xmedia_u8 region_division_str[XMEDIA_ISP_SHARPEN_VAR_STR_NUM]; // 不同区域的锐化强度
    xmedia_u8 dir_edge_mode;                                // 方向模式：2、4、8方向
    xmedia_u8 dir_sensitivity;                              // 方向划分灵敏度，值大划分精度高
    xmedia_u8 dir_edge_freq;                                // 带方向边缘频段控制
    xmedia_u8 dir_edge_thd[XMEDIA_ISP_SHARPEN_DIR_THD_NUM]; // 带方向边缘划分阈值：无方向及纹理区、弱方向、强方向
    xmedia_u8 dir_edge_str[XMEDIA_ISP_SHARPEN_DIR_STR_NUM]; // 带方向边缘的叠加锐化强度
    xmedia_u8 fusion_edge_max_edge;                         // 融合模式下边缘区边缘层细节最大融合权重
    xmedia_u8 fusion_edge_max_flat;                         // 融合模式下平坦区边缘层细节最大融合权重
    xmedia_u8 fusion_edge_thd;                              // 融合模式下边缘区阈值
    xmedia_u8 fusion_flat_thd;                              // 融合模式下平坦区阈值
} xmedia_isp_sharpen_manual_attr;

typedef enum {
    XMEDIA_ISP_SHARPEN_YSH_COMBINE_MODE_FUSION = 0,
    XMEDIA_ISP_SHARPEN_YSH_COMBINE_MODE_FUSION_WITH_WEIGHT,
    XMEDIA_ISP_SHARPEN_YSH_COMBINE_MODE_MAX
} xmedia_isp_sharpen_ysh_combine_mode;

typedef struct {
    xmedia_bool                         enable; // sharpen增强功能使能
    xmedia_isp_sharpen_ysh_combine_mode detail_combine_mode;
    xmedia_s8                           skin_ub[XMEDIA_ISP_SHARPEN_SKIN_NUM];    // 肤色区域的u/v上界
    xmedia_s8                           skin_lb[XMEDIA_ISP_SHARPEN_SKIN_NUM];    // 肤色区域的u/v下界
    xmedia_u8                           skin_delta[XMEDIA_ISP_SHARPEN_SKIN_NUM]; // 肤色区域的u/v扩张范围
    xmedia_u8                           rgb_delta[XMEDIA_ISP_SHARPEN_RGB_NUM];   // RGB的U/V扩张范围
    xmedia_video_operation_mode         op_type;                                 // sharpen工作类型
    xmedia_isp_sharpen_manual_attr      manual_attr;                             // sharpen手动参数结构
    xmedia_isp_sharpen_auto_attr        auto_attr;                               // sharpen自动参数结构
} xmedia_isp_sharpen_attr;

// STNR (3DNR)
#define XMEDIA_ISP_3DNR_THD_MAX_COUNT  4
#define XMEDIA_ISP_3DNR_STR_MAX_COUNT  3

typedef struct {
    xmedia_u8  edge_sens;       // 边缘区敏感指数
    xmedia_u8  corner_sens;     // 角点区敏感指数
    xmedia_u8  struct_sens;     // 结构性敏感指数
    xmedia_u8  graininess_sens; // 颗粒性敏感指数
    xmedia_u8  v_denoise_str;   // 控制去噪强度
    xmedia_u8  s_denoise_str;   // 控制去噪强度
    xmedia_u16 min_denoise_str; // 去噪强度下限
    xmedia_u8  sigma;           // 第一种滤波参数sigma
    xmedia_u8  s_luma_thd_low;  // 7605V2
    xmedia_u8  s_luma_thd_high; // 7605V2
    xmedia_u8  s_luma_str_low;  // 7605V2
    xmedia_u8  s_luma_str_high; // 7605V2
    xmedia_u8  s_mot_thd_low;   // 7605V2
    xmedia_u8  s_mot_thd_high;  // 7605V2
    xmedia_u8  s_mot_str_low;   // 7605V2
    xmedia_u8  s_mot_str_high;  // 7605V2
    xmedia_u8  v_luma_thd_low;
    xmedia_u8  v_luma_thd_high;
    xmedia_u8  v_luma_str_low;
    xmedia_u8  v_luma_str_high;
    xmedia_u8  v_mot_thd_low;
    xmedia_u8  v_mot_thd_high;
    xmedia_u8  v_mot_str_low;
    xmedia_u8  v_mot_str_high;
    xmedia_u8  text_thd[XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u8  text_str[XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  luma_varscale_thd_low;
    xmedia_u8  luma_varscale_thd_high;
    xmedia_u16 luma_varscale_str_low;
    xmedia_u16 luma_varscale_str_high;
    xmedia_u16 var_scale;
    xmedia_u8  blend_var_thd_low;
    xmedia_u8  blend_var_thd_high;
    xmedia_u16 blend_var_low_wgt;
    xmedia_u16 blend_var_high_wgt;
    xmedia_u8  blend_luma_thd_low;
    xmedia_u8  blend_luma_thd_high;
    xmedia_u16 blend_luma_low_wgt;
    xmedia_u16 blend_luma_high_wgt;
    xmedia_u8  blend_mot_thd_low;
    xmedia_u8  blend_mot_thd_high;
    xmedia_u16 blend_mot_low_wgt;
    xmedia_u16 blend_mot_high_wgt;
    xmedia_u16 nr_addback_str;
    xmedia_u8  blend_nr_center_var_thd_low; // 根据纹理融合第一种滤波结果和中心值
    xmedia_u8  blend_nr_center_var_thd_high;
    xmedia_u16 blend_nr_center_var_low_wgt;
    xmedia_u16 blend_nr_center_var_high_wgt;
    xmedia_u8  addback_text_thd[XMEDIA_ISP_3DNR_THD_MAX_COUNT]; // 根据纹理阈值噪声回叠
    xmedia_u16 addback_text_str[XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  addback_luma_thd[XMEDIA_ISP_3DNR_THD_MAX_COUNT]; // 根据亮度阈值噪声回叠
    xmedia_u16 addback_luma_str[XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  addback_text_luma_blend_mot_thd_low; // 根据运动阈值融合前面根据亮度和纹理回叠的结果
    xmedia_u16 addback_text_luma_blend_mot_thd_high;
    xmedia_u8  addback_mot_thd[XMEDIA_ISP_3DNR_THD_MAX_COUNT]; // 根据运动阈值噪声回叠
    xmedia_u16 addback_mot_weak_str;
    xmedia_u16 addback_mot_strong_str;
} xmedia_isp_stnr_ynr_param_manual;

typedef struct {
    xmedia_u32 iso_num;                                   // 自动参数配置的参考数组
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];       // 参考的ISO数组
    xmedia_u8  edge_sens[XMEDIA_ISP_ISO_MAX_COUNT];       // 边缘区敏感指数
    xmedia_u8  corner_sens[XMEDIA_ISP_ISO_MAX_COUNT];     // 角点区敏感指数
    xmedia_u8  struct_sens[XMEDIA_ISP_ISO_MAX_COUNT];     // 结构性敏感指数
    xmedia_u8  graininess_sens[XMEDIA_ISP_ISO_MAX_COUNT]; // 颗粒性敏感指数
    xmedia_u8  v_denoise_str[XMEDIA_ISP_ISO_MAX_COUNT];   // 控制去噪强度
    xmedia_u8  s_denoise_str[XMEDIA_ISP_ISO_MAX_COUNT];   // 控制去噪强度
    xmedia_u16 min_denoise_str[XMEDIA_ISP_ISO_MAX_COUNT]; // 去噪强度下限
    xmedia_u8  sigma[XMEDIA_ISP_ISO_MAX_COUNT];           // 第一种滤波参数sigma
    xmedia_u8  s_luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];  // 7605V2
    xmedia_u8  s_luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT]; // 7605V2
    xmedia_u8  s_luma_str_low[XMEDIA_ISP_ISO_MAX_COUNT];  // 7605V2
    xmedia_u8  s_luma_str_high[XMEDIA_ISP_ISO_MAX_COUNT]; // 7605V2
    xmedia_u8  s_mot_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];   // 7605V2
    xmedia_u8  s_mot_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];  // 7605V2
    xmedia_u8  s_mot_str_low[XMEDIA_ISP_ISO_MAX_COUNT];   // 7605V2
    xmedia_u8  s_mot_str_high[XMEDIA_ISP_ISO_MAX_COUNT];  // 7605V2
    xmedia_u8  v_luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_luma_str_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_luma_str_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_mot_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_mot_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_mot_str_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  v_mot_str_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  text_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u8  text_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  luma_varscale_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  luma_varscale_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 luma_varscale_str_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 luma_varscale_str_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 var_scale[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_var_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_var_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_var_low_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_var_high_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_luma_low_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_luma_high_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_mot_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_mot_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_mot_low_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_mot_high_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 nr_addback_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_nr_center_var_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  blend_nr_center_var_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_nr_center_var_low_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 blend_nr_center_var_high_wgt[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  addback_text_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u16 addback_text_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  addback_luma_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u16 addback_luma_str[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  addback_text_luma_blend_mot_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 addback_text_luma_blend_mot_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  addback_mot_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u16 addback_mot_weak_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 addback_mot_strong_str[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_stnr_ynr_param_auto;

typedef struct {
    xmedia_u8  chroma_sens;
    xmedia_u8  ghost_coring_uv;
    xmedia_u8  ghost_coring;
    xmedia_u8  md_win_size;
    xmedia_u8  bgm_str;
    xmedia_u8  str;
    xmedia_u8  uv_str;
    xmedia_u16 md_thd_low;
    xmedia_u16 md_thd_high;
    xmedia_u8  luma_md_thd[XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u16 luma_md_scl[XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  sat_md_thd_low;
    xmedia_u8  sat_md_thd_high;
    xmedia_u16 satl_md_scl;
    xmedia_u16 sath_md_scl;
    xmedia_u8  border_thd;
    xmedia_u16 border_md_scl;
} xmedia_isp_stnr_tnr_param_manual;

typedef struct {
    xmedia_u32 iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  chroma_sens[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  ghost_coring_uv[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  ghost_coring[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  md_win_size[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  bgm_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  uv_str[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 md_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 md_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  luma_md_thd[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_THD_MAX_COUNT];
    xmedia_u16 luma_md_scl[XMEDIA_ISP_ISO_MAX_COUNT][XMEDIA_ISP_3DNR_STR_MAX_COUNT];
    xmedia_u8  sat_md_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  sat_md_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 satl_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 sath_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  border_thd[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 border_md_scl[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_stnr_tnr_param_auto;

typedef struct {
    xmedia_u8  struct_sens;          // 滤波结构性敏感指数
    xmedia_u8  edge_det_thd;         // 边缘检测阈值
    xmedia_u8  edge_det_robust;      // 边缘检测抗噪阈值
    xmedia_u8  de_dark_color_str;    // 去暗部色噪强度
    xmedia_u8  de_bg_color_spot_str; // 去背景色斑强度
    xmedia_u8  coarse_denoise_str;   // 粗调控制全局色域去噪强度
    xmedia_u8  fine_denoise_str;     // 细调控制全局色域去噪强度
    xmedia_u8  mot_str;              // 运动区域降色噪强度
    xmedia_u8  mot_denoise_thd_high; // 运动区域降色噪高阈值
    xmedia_u8  mot_denoise_thd_low;  // 运动区域降色噪低阈值
    xmedia_u8  mot_denoise_str_high; // 运动区域降色噪强度
    xmedia_u8  mot_denoise_str_low;  // 静止区域降色噪强度
    xmedia_u8  mot_uvnr_str;         // 运动区域降色噪强度
    xmedia_u16 mot_bld_str;          // 运动区域使用小图的最高权重 0~256
    xmedia_u8  mot_bld_thd_low;      // 运动区域融合小图的低阈值   only for 7206V1
    xmedia_u8  mot_bld_thd_high;     // 运动区域融合小图的高阈值   only for 7206V1
    xmedia_u8  de_purple_edge_thd;   // 去紫边阈值
    xmedia_u8  de_purple_edge_str;   // 去紫边强度
    xmedia_u8  purple_range;         // 紫色范围
    xmedia_u8  spec_color_degree;    // 特定色角度
    xmedia_u8  spec_color_str;       // 去特定色强度
    xmedia_u8  luma_thd_high;        // 亮度控制降色噪高阈值
    xmedia_u8  luma_thd_low;         // 亮度控制降色噪低阈值
    xmedia_u8  luma_str_high;        // 亮区控制降色噪强度
    xmedia_u8  luma_str_low;         // 暗区降色噪强度
} xmedia_isp_stnr_cnr_param_manual;

typedef struct {
    xmedia_u32 iso_num;                                        // 自动参数配置的参考数组
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];            // 参考的ISO数组
    xmedia_u8  struct_sens[XMEDIA_ISP_ISO_MAX_COUNT];          // 滤波结构性敏感指数
    xmedia_u8  edge_det_thd[XMEDIA_ISP_ISO_MAX_COUNT];         // 边缘检测阈值
    xmedia_u8  edge_det_robust[XMEDIA_ISP_ISO_MAX_COUNT];      // 边缘检测抗噪阈值
    xmedia_u8  de_dark_color_str[XMEDIA_ISP_ISO_MAX_COUNT];    // 去暗部色噪强度
    xmedia_u8  de_bg_color_spot_str[XMEDIA_ISP_ISO_MAX_COUNT]; // 去背景色斑强度            only for 7605V2
    xmedia_u8  coarse_denoise_str[XMEDIA_ISP_ISO_MAX_COUNT];   // 粗调控制全局色域去噪强度
    xmedia_u8  fine_denoise_str[XMEDIA_ISP_ISO_MAX_COUNT];     // 细调控制全局色域去噪强度
    xmedia_u8  mot_str[XMEDIA_ISP_ISO_MAX_COUNT];              // 运动区域降色噪强度        only for 7605V2
    xmedia_u8  mot_denoise_thd_high[XMEDIA_ISP_ISO_MAX_COUNT]; // 运动区域降色噪高阈值
    xmedia_u8  mot_denoise_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];  // 运动区域降色噪低阈值
    xmedia_u8  mot_denoise_str_high[XMEDIA_ISP_ISO_MAX_COUNT]; // 运动区域降色噪强度
    xmedia_u8  mot_denoise_str_low[XMEDIA_ISP_ISO_MAX_COUNT];  // 静止区域降色噪强度
    xmedia_u8  mot_uvnr_str[XMEDIA_ISP_ISO_MAX_COUNT];         // 运动区域小图滤波强度
    xmedia_u16 mot_bld_str[XMEDIA_ISP_ISO_MAX_COUNT];          // 运动区域使用小图的最高权重
    xmedia_u8  mot_bld_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];      // 运动区域融合小图的低阈值  only for 7206V1
    xmedia_u8  mot_bld_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];     // 运动区域融合小图的高阈值  only for 7206V1
    xmedia_u8  de_purple_edge_thd[XMEDIA_ISP_ISO_MAX_COUNT];   // 去紫边阈值
    xmedia_u8  de_purple_edge_str[XMEDIA_ISP_ISO_MAX_COUNT];   // 去紫边强度
    xmedia_u8  purple_range[XMEDIA_ISP_ISO_MAX_COUNT];         // 紫色范围
    xmedia_u8  spec_color_degree[XMEDIA_ISP_ISO_MAX_COUNT];    // 特定色角度
    xmedia_u8  spec_color_str[XMEDIA_ISP_ISO_MAX_COUNT];       // 去特定色强度
    xmedia_u8  luma_thd_high[XMEDIA_ISP_ISO_MAX_COUNT];        // 亮度控制降色噪高阈值
    xmedia_u8  luma_thd_low[XMEDIA_ISP_ISO_MAX_COUNT];         // 亮度控制降色噪低阈值
    xmedia_u8  luma_str_high[XMEDIA_ISP_ISO_MAX_COUNT];        // 亮区控制降色噪强度
    xmedia_u8  luma_str_low[XMEDIA_ISP_ISO_MAX_COUNT];         // 暗区降色噪强度
} xmedia_isp_stnr_cnr_param_auto;

typedef enum {
    XMEDIA_ISP_STNR_YNR_DENOISE_STR_DEFAULT = 0, // 去噪基础强度默认不参考运动区
    XMEDIA_ISP_STNR_YNR_DENOISE_STR_MOV     = 1, // 去噪基础强度参考运动区
    XMEDIA_ISP_STNR_YNR_DENOISE_STR_MAX
} xmedia_isp_stnr_ynr_noise_str_mode;

typedef enum {
    XMEDIA_ISP_STNR_YNR_GRAD_WIN_SML = 5, // 小窗口计算梯度
    XMEDIA_ISP_STNR_YNR_GRAD_WIN_MED = 7, // 中等窗口计算梯度
    XMEDIA_ISP_STNR_YNR_GRAD_WIN_LG  = 9, // 大窗口计算梯度
    XMEDIA_ISP_STNR_YNR_GRAD_WIN_MAX
} xmedia_isp_stnr_ynr_grad_win_size;

typedef enum {
    XMEDIA_ISP_STNR_YNR_VAR_MODE_DEFAULT = 0, // 默认不参考运动区算边缘
    XMEDIA_ISP_STNR_YNR_VAR_MODE_MOV     = 1, // 参考运动区算边缘
    XMEDIA_ISP_STNR_YNR_VAR_MODE_MAX
} xmedia_isp_stnr_ynr_var_mode;

typedef enum {
    XMEDIA_ISP_STNR_YNR_SELECT_CENTERLUMA_NR  = 0, // 经过滤波后的结果作为中心点像素
    XMEDIA_ISP_STNR_YNR_SELECT_CENTERLUMA_ORI = 1, // 原始像素作为中心点像素
    XMEDIA_ISP_STNR_YNR_SELECT_CENTERLUMA_MAX
} xmedia_isp_stnr_ynr_select_centerluma_mode;

typedef enum {
    XMEDIA_ISP_STNR_YNR_ADDBACK_SCL_DEFAULT = 0, // 噪声回叠强度不参考运动区
    XMEDIA_ISP_STNR_YNR_ADDBACK_SCL_MOV     = 1, // 噪声回叠强度参考运动区
    XMEDIA_ISP_STNR_YNR_ADDBACK_SCL_MAX
} xmedia_isp_stnr_ynr_addback_scl_mode;

typedef struct {
    xmedia_isp_stnr_ynr_noise_str_mode         denoise_str_mode; // 去噪基础强度是否参考运动区模式选择
    xmedia_isp_stnr_ynr_grad_win_size          grad_win_size;    // 计算梯度的窗口大小选择
    xmedia_isp_stnr_ynr_var_mode               var_mode;         // edge图是否参考运动区模式选择
    xmedia_isp_stnr_ynr_select_centerluma_mode select_centerluma_mode; // 中心点模式选择
    xmedia_bool                                nr_addback_en;          // 噪声回叠开关
    xmedia_isp_stnr_ynr_addback_scl_mode ynr_addback_scl_mode; // 噪声回叠强度是否参考运动区模式选择
    xmedia_video_operation_mode          op_type;              // ynr操作模式，0：自动模式；1：手动模式
    xmedia_isp_stnr_ynr_param_manual     manual_attr;          // ynr手动参数结构
    xmedia_isp_stnr_ynr_param_auto       auto_attr;            // ynr自动参数结构
} xmedia_isp_stnr_ynr_param;

typedef struct {
    xmedia_video_operation_mode      op_type;     // tnr操作模式，0：自动模式；1：手动模式
    xmedia_isp_stnr_tnr_param_manual manual_attr; // tnr手动参数结构
    xmedia_isp_stnr_tnr_param_auto   auto_attr;   // tnr自动参数结构
} xmedia_isp_stnr_tnr_param;

typedef struct {
    xmedia_video_operation_mode      op_type;     // cnr操作模式，0：自动模式；1：手动模式
    xmedia_isp_stnr_cnr_param_manual manual_attr; // cnr手动参数结构
    xmedia_isp_stnr_cnr_param_auto   auto_attr;   // cnr自动参数结构
} xmedia_isp_stnr_cnr_param;

typedef struct {
    xmedia_bool stnr_enable; // stnr总开关
    xmedia_u8   stnr_mode;   // 选择YNR/TNR/YSH顺序的模式

    xmedia_bool ynr_enable;    // 亮度域空域去噪使能
    xmedia_bool cnr_enable;    // 色度域空域去噪使能
    xmedia_bool tnr_enable;    // 时域去噪使能
    xmedia_bool tnr_mc_enable; // 时域运动补偿使能

    xmedia_isp_stnr_ynr_param ynr; // 亮度域空域去噪参数
    xmedia_isp_stnr_tnr_param tnr; // 时域去噪参数
    xmedia_isp_stnr_cnr_param cnr; // 色度域空域去噪参数
} xmedia_isp_stnr_attr;

typedef struct {
    xmedia_u16 pos_gain;
    xmedia_u16 gau_gain;
} xmedia_isp_ainr_manual_attr;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 pos_gain[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16 gau_gain[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_isp_ainr_auto_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_u32                  switch_thd_low;
    xmedia_u32                  switch_thd_high;
    xmedia_video_operation_mode op_type;
    xmedia_isp_ainr_manual_attr manual_attr;
    xmedia_isp_ainr_auto_attr   auto_attr;
} xmedia_isp_ainr_attr;

#define XMEDIA_SENSOR_DEV_MAX_NUM        XMEDIA_ISP_PIPE_MAX_NUM
#define XMEDIA_SENSOR_MAX_REGS_NUM       64
#define XMEDIA_SENSOR_VCID_MAX_NUM       4
#define XMEDIA_SENSOR_NAME_MAX_SIZE      128
#define XMEDIA_SENSOR_BAYER_CHN_NUM      4
#define XMEDIA_SENSOR_CCM_MATRIX_SIZE    9
#define XMEDIA_SENSOR_GROUP_MIN_PIPE_NUM 2
#define XMEDIA_SENSOR_GROUP_MAX_PIPE_NUM 4
#define XMEDIA_SENSOR_PDAF_LRPD_MAX_NUM  64
#define XMEDIA_SENSOR_PDAF_LINE_MAX_NUM  16
#define XMEDIA_SENSOR_PDAF_PATTERN_LEFT  0
#define XMEDIA_SENSOR_PDAF_PATTERN_RIGHT 1

typedef enum {
    XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP = 0,
    XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR,
    XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP,
    XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL,
    XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MAX,
} xmedia_sensor_mirror_flip_type;

typedef enum {
    XMEDIA_SENSOR_PDAF_TYPE_NONE,
    XMEDIA_SENSOR_PDAF_TYPE_1, // output pd value by sensor.
    XMEDIA_SENSOR_PDAF_TYPE_2, // output pd pixel by sensor
    XMEDIA_SENSOR_PDAF_TYPE_3, // extract pd pixel by soc from image.
    XMEDIA_SENSOR_PDAF_TYPE_MAX,
} xmedia_sensor_pdaf_type;

typedef enum {
    XMEDIA_SENSOR_PDAF_PATTERN_UNDEFINED,
    XMEDIA_SENSOR_PDAF_PATTERN_LINE,  // 以pd行为单位进行循环
    XMEDIA_SENSOR_PDAF_PATTERN_PIXEL, // 以pd点为单位进行循环
    XMEDIA_SENSOR_PDAF_PATTERN_MAX,
} xmedia_sensor_pdaf_pattern_type;

typedef struct {
    xmedia_sensor_pdaf_pattern_type type;
    xmedia_u32                      cycle;
    xmedia_u32                      pattern[XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MAX];
} xmedia_sensor_pdaf_pattern;

typedef enum {
    XMEDIA_SENSOR_PDAF_CORRECT_NONE,
    XMEDIA_SENSOR_PDAF_CORRECT_BY_SENSOR, // dpc by sensor
    XMEDIA_SENSOR_PDAF_CORRECT_BY_ISP,    // dpc by soc
    XMEDIA_SENSOR_PDAF_CORRECT_MAX,
} xmedia_sensor_pdaf_correction;

typedef struct {
    xmedia_u32       lrpd_num;
    xmedia_isp_point left_pd_point[XMEDIA_SENSOR_PDAF_LRPD_MAX_NUM];
    xmedia_isp_point right_pd_point[XMEDIA_SENSOR_PDAF_LRPD_MAX_NUM];
} xmedia_sensor_pdaf_point;

typedef struct {
    xmedia_sensor_pdaf_type       pd_type;
    xmedia_sensor_pdaf_correction pd_correction;
    xmedia_u8                     mipi_vcid;      // type1 & type2
    xmedia_u8                     mipi_data_type; // type1 & type2
    xmedia_u8                     pd_data_width;
    xmedia_video_rect             pd_whole_pixel_area;
    xmedia_video_size             pd_block_pixel_size;
    xmedia_video_size             pd_block_data_size;
    xmedia_sensor_pdaf_pattern    pd_block_pattern;
} xmedia_sensor_pdaf_feature;

typedef struct {
    const xmedia_sensor_pdaf_feature *pdaf_feature;
    const xmedia_sensor_pdaf_point   *pdaf_point;
} xmedia_sensor_pdaf_info;

/*
 * NORMAL: sensor常规工作模式，也是默认工作模式。
 * MASTER: sensor工作在master模式, 向外提供VSYNC等同步信号
 * SLAVE: sensor工作在slave模式, 受外部VSYNC信号触发曝光和数据输出
 */
typedef enum {
    XMEDIA_SENSOR_WORK_MODE_NORMAL = 0,
    XMEDIA_SENSOR_WORK_MODE_MASTER,
    XMEDIA_SENSOR_WORK_MODE_SLAVE,
    XMEDIA_SENSOR_WORK_MODE_MAX,
} xmedia_sensor_work_mode;

typedef enum{
    XMEDIA_SENSOR_TRIG_SOURCE_NONE = 0,
    XMEDIA_SENSOR_TRIG_SOURCE_MASTER,
    XMEDIA_SENSOR_TRIG_SOURCE_HOST,
    XMEDIA_SENSOR_TRIG_SOURCE_MAX,
}xmedia_sensor_trig_source;

typedef struct{
    xmedia_sensor_trig_source source;
    xmedia_u32                dev_id;
}xmedia_sensor_trig_signal;

typedef enum{
    XMEDIA_SENSOR_GROUP_TYPE_MASTER_SLAVE,
    XMEDIA_SENSOR_GROUP_TYPE_HOST_SLAVE,
    XMEDIA_SENSOR_GROUP_TYPE_MAX,
}xmedia_sensor_group_type;

typedef struct{
    xmedia_sensor_group_type type;
    xmedia_u32               pipe_num;
    xmedia_u32               pipe[XMEDIA_SENSOR_GROUP_MAX_PIPE_NUM];
}xmedia_sensor_group_config;

typedef struct {
    xmedia_u16 width;
    xmedia_u16 height;
    xmedia_video_wdr_mode  wdr_mode;
} xmedia_sensor_attr;

typedef enum {
    XMEDIA_SENSOR_CFG_POS_VACTIVE = 0,
    XMEDIA_SENSOR_CFG_POS_VBLANK,
    XMEDIA_SENSOR_CFG_POS_MAX
} xmedia_sensor_cfg_pos;

typedef struct {
    xmedia_bool update;
    xmedia_u8 delay_frame_num; // sensor 当前寄存器(从配置到)生效的帧数, 控制sensor内部寄存器同步
    xmedia_sensor_cfg_pos cfg_pos; // sensor 当前寄存器配置时间点, FS: 帧起始, FE：帧结束

    xmedia_u32 dev_addr;
    xmedia_u32 reg_addr;
    xmedia_u32 addr_byte_num;
    xmedia_u32 data;
    xmedia_u32 data_byte_num;
} xmedia_sensor_i2c_sync_reg;

typedef enum {
    XMEDIA_SENSOR_BUS_TYPE_I2C = 0,
    XMEDIA_SENSOR_BUS_TYPE_SPI,
    XMEDIA_SENSOR_BUS_TYPE_MAX,
} xmedia_sensor_bus_type;

// sensor communication bus
typedef struct {
    xmedia_sensor_bus_type type;

    union {
        xmedia_s8 i2c_dev;

        struct {
            xmedia_u8 spi_dev;
            xmedia_u8 spi_cs; // chip select: 一主多从模式时，CS为主芯片对从芯片的控制信号
        } spi_info;
    };
} xmedia_sensor_commbus;

typedef struct {
    xmedia_u32 clk_ch;
    xmedia_u32 rst_ch;
} xmedia_sensor_ctrl_sig;

typedef enum {
    XMEDIA_SENSOR_INIT_MODE_NORMAL = 0,
    XMEDIA_SENSOR_INIT_MODE_SKIP, // 跳过sensor寄存器初始化流程
    XMEDIA_SENSOR_INIT_MODE_MAX,
} xmedia_sensor_init_mode;

typedef struct {
    xmedia_sensor_init_mode init_mode;
    xmedia_sensor_ctrl_sig  ctrl_sig;
    xmedia_sensor_commbus   comm_bus;
} xmedia_sensor_config;

typedef enum {
    XMEDIA_SENSOR_CLOCK_FREQ_74_25MHZ  = 74250000,
    XMEDIA_SENSOR_CLOCK_FREQ_72MHZ     = 72000000,
    XMEDIA_SENSOR_CLOCK_FREQ_54MHZ     = 54000000,
    XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ = 37125000,
    XMEDIA_SENSOR_CLOCK_FREQ_36MHZ     = 36000000,
    XMEDIA_SENSOR_CLOCK_FREQ_27MHZ     = 27000000,
    XMEDIA_SENSOR_CLOCK_FREQ_24MHZ     = 24000000,
    XMEDIA_SENSOR_CLOCK_FREQ_12MHZ     = 12000000,
} xmedia_sensor_clock_freq;

typedef struct {
    xmedia_u32            width;
    xmedia_u32            height;
    xmedia_video_wdr_mode wdr_mode;

    xmedia_video_wdr_format  wdr_format;
    xmedia_float             max_fps;
    xmedia_sensor_clock_freq input_clock;

    xmedia_intf_type          output_intf;
    xmedia_video_pixel_format pixel_format;
    xmedia_video_data_width   bit_width;

    xmedia_video_bayer_format bayer_format[XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MAX];

    xmedia_u8  vcid[XMEDIA_SENSOR_VCID_MAX_NUM];
    xmedia_u32 bit_rate; // unit: Mbps
} xmedia_sensor_property;

typedef enum {
    XMEDIA_SENSOR_MIPI_LANES_1L = 1,
    XMEDIA_SENSOR_MIPI_LANES_2L = 2,
    XMEDIA_SENSOR_MIPI_LANES_4L = 4,
    XMEDIA_SENSOR_MIPI_LANES_8L = 8,
    XMEDIA_SENSOR_MIPI_LANES_MAX
} xmedia_sensor_mipi_lanes;

typedef struct {
    xmedia_bool update;
    xmedia_u8   delay_frm_num;
    xmedia_u8   int_pos;
    xmedia_u32  dev_addr;
    xmedia_u32  dev_addr_byte_num;
    xmedia_u32  reg_addr;
    xmedia_u32  reg_addr_byte_num;
    xmedia_u32  data;
    xmedia_u32  data_byte_num;
} xmedia_sensor_spi_sync_reg;

typedef struct {
    xmedia_sensor_commbus com_bus;
    xmedia_u8 cfg_to_valid_delay_max; // Sensor 寄存器最大延迟生效帧数 ，控制Sensor与ISP同步

    xmedia_u32 reg_num; // RW; Number of registers required when exposure results are written to the sensor

    union {
        xmedia_sensor_i2c_sync_reg i2c_data[XMEDIA_SENSOR_MAX_REGS_NUM];
        xmedia_sensor_spi_sync_reg spi_data[XMEDIA_SENSOR_MAX_REGS_NUM];
    };
} xmedia_sensor_regs_info;

#define XMEDIA_SENSOR_BAYER_CHN_R  0
#define XMEDIA_SENSOR_BAYER_CHN_GR 1
#define XMEDIA_SENSOR_BAYER_CHN_GB 2
#define XMEDIA_SENSOR_BAYER_CHN_B  3

typedef struct {
    xmedia_bool update;
    xmedia_u16  black_level[XMEDIA_SENSOR_BAYER_CHN_NUM];
} xmedia_sensor_black_level;

typedef struct {
    xmedia_bool ir_mode;
    xmedia_u32  exp_time;
    xmedia_u32  again;
    xmedia_u32  dgain;
    xmedia_u32  ispdgain;
    xmedia_u32  exposure;
    xmedia_u32  init_iso;
    xmedia_u32  piris_fno;
    xmedia_u16  wb_rgain;
    xmedia_u16  wb_ggain;
    xmedia_u16  wb_bgain;
    xmedia_u16  sample_rgain;
    xmedia_u16  sample_bgain;
    xmedia_u16  ccm[XMEDIA_SENSOR_CCM_MATRIX_SIZE];
} xmedia_sensor_init_param;

typedef struct {
    const xmedia_isp_ainr_attr               *ainr;
    const xmedia_isp_anti_false_color_attr   *anti_false_color;
    const xmedia_isp_blc_attr                *blc;
    const xmedia_isp_bnr_attr                *bnr;
    const xmedia_isp_clut_attr               *clut_attr;
    const xmedia_isp_cr_attr                 *crosstalk;
    const xmedia_isp_csc_attr                *csc;
    const xmedia_isp_dehaze_attr             *dehaze;
    const xmedia_isp_demosaic_attr           *demosaic;
    const xmedia_isp_dpc_dynamic_attr        *dpc_dynamic;
    const xmedia_isp_dpc_static_attr         *dpc_static;
    const xmedia_isp_drc_attr                *drc;
    const xmedia_isp_expander_attr           *expander;
    const xmedia_isp_fpn_attr                *fpn;
    const xmedia_isp_gamma_attr              *gamma;
    const xmedia_isp_gcac_attr               *gcac;
    const xmedia_isp_hlc_attr                *hlc;
    const xmedia_isp_lcac_attr               *lcac;
    const xmedia_isp_lce_attr                *lce;
    const xmedia_isp_mesh_shading_attr       *mlsc_attr;
    const xmedia_isp_mesh_shading_lut_attr   *mlsc_lut;
    const xmedia_isp_rc_attr                 *radial_crop;
    const xmedia_isp_rgbir_attr              *rgbir;
    const xmedia_isp_radial_shading_attr     *rlsc_attr;
    const xmedia_isp_radial_shading_lut_attr *rlsc_lut;
    const xmedia_isp_sharpen_attr            *sharpen;
    const xmedia_isp_stnr_attr               *stnr;
    const xmedia_isp_wdr_attr                *wdr;
    const xmedia_isp_ca_attr                 *ca;
} xmedia_sensor_isp_default;

#define XMEDIA_SENSOR_DELAY_FLAG          0xFFFFFFFF
#define XMEDIA_SENSOR_STANDBY_REG_MAX_NUM 16

typedef struct {
    xmedia_bool standby_supported;
    xmedia_u16  addr_byte_num;
    xmedia_u16  data_byte_num;
    xmedia_u32  standby_reg_num;
    xmedia_u32  standby_reg_addr[XMEDIA_SENSOR_STANDBY_REG_MAX_NUM];
    xmedia_u32  standby_reg_data[XMEDIA_SENSOR_STANDBY_REG_MAX_NUM];
    xmedia_u32  resume_reg_num;
    xmedia_u32  resume_reg_addr[XMEDIA_SENSOR_STANDBY_REG_MAX_NUM];
    xmedia_u32  resume_reg_data[XMEDIA_SENSOR_STANDBY_REG_MAX_NUM];
} xmedia_sensor_standby_info;

// reflect the hightest supported specifications
typedef struct {
    xmedia_u32   max_width;
    xmedia_u32   max_height;
    xmedia_float max_fps;

    xmedia_intf_type         output_intf;
    xmedia_sensor_clock_freq init_mclk;
    xmedia_video_wdr_mode    wdr_mode;
    xmedia_video_wdr_format  wdr_format;
    xmedia_video_data_width  bit_width;  // max bit width
    xmedia_u32               reset_time; // unit: us

    // slave mode supported by the driver
    xmedia_bool slave_mode_supported;

    // all mipi lanes supported by the driver
    xmedia_u32 mipi_lanes_supported; // BIT0: 1LANE; BIT1: 2LANE; BIT2: 4LANE; BIT3: 8LANE;

    xmedia_sensor_standby_info standby_info;
} xmedia_sensor_capability;

typedef struct {
    // called by ISP
    xmedia_s32 (*pfn_sensor_init)(xmedia_u32 dev, xmedia_sensor_init_mode init_mode);
    xmedia_s32 (*pfn_sensor_exit)(xmedia_u32 dev);
    xmedia_s32 (*pfn_sensor_set_attr)(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr);
    xmedia_s32 (*pfn_sensor_get_isp_default)(xmedia_u32 dev, xmedia_sensor_isp_default *isp_def);
    xmedia_s32 (*pfn_sensor_get_isp_black_level)(xmedia_u32 dev, xmedia_sensor_black_level *black_level);
    xmedia_s32 (*pfn_sensor_get_reg_info)(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info);
    xmedia_s32 (*pfn_sensor_get_bayer_pattern)(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern);
    xmedia_s32 (*pfn_sensor_get_capability)(xmedia_sensor_capability *capability);
    xmedia_s32 (*pfn_sensor_get_fps)(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps);
    xmedia_s32 (*pfn_sensor_get_hmax_vmax)(xmedia_u32 dev, xmedia_u32 *hmax, xmedia_u32*vmax);
    xmedia_s32 (*pfn_sensor_get_trig_attr)(xmedia_sensor_trig_attr *trig_attr);
    xmedia_s32 (*pfn_sensor_get_dev_addr)(xmedia_u32 dev, xmedia_u32 *slave_addr);
    xmedia_s32 (*pfn_sensor_update_reg_info)(xmedia_u32 dev, xmedia_sensor_regs_info *cur_info,
                                             xmedia_sensor_regs_info *pre_info);
    xmedia_s32 (*pfn_sensor_get_pdaf_info)(xmedia_u32 dev, xmedia_sensor_pdaf_info *pdaf_info);
    xmedia_s32 (*pfn_sensor_format_pdaf)(xmedia_u32 dev, xmedia_void *pd_buff, xmedia_void *tmp_buff,
                                         xmedia_video_size *size, xmedia_u8 bit_width);
    xmedia_s32 (*pfn_sensor_calc_image_size)(xmedia_u32 dev, const xmedia_video_size *pd_size,
                                             xmedia_video_size *image_size);

    // Called by user
    xmedia_s32 (*pfn_sensor_get_property)(xmedia_u32 dev, xmedia_sensor_property *property);
    xmedia_s32 (*pfn_sensor_set_work_mode)(xmedia_u32 dev, xmedia_sensor_work_mode work_mode);
    xmedia_s32 (*pfn_sensor_set_mipi_lanes)(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes);

    xmedia_s32 (*pfn_sensor_mirror)(xmedia_u32 dev, xmedia_bool mirror_en);
    xmedia_s32 (*pfn_sensor_flip)(xmedia_u32 dev, xmedia_bool flip_en);

    xmedia_s32 (*pfn_sensor_set_bus_info)(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info);
    xmedia_s32 (*pfn_sensor_set_dev_addr)(xmedia_u32 dev, xmedia_u32 slave_addr);

    xmedia_s32 (*pfn_sensor_start)(xmedia_u32 dev);
    xmedia_s32 (*pfn_sensor_stop)(xmedia_u32 dev);
    xmedia_s32 (*pfn_sensor_standby)(xmedia_u32 dev);
    xmedia_s32 (*pfn_sensor_resume)(xmedia_u32 dev);
    xmedia_s32 (*pfn_sensor_write_reg)(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
    xmedia_s32 (*pfn_sensor_read_reg)(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
    xmedia_s32 (*pfn_sensor_set_init_param)(xmedia_u32 dev, const xmedia_sensor_init_param *def_cfg);
    xmedia_s32 (*pfn_sensor_low_power)(xmedia_u32 dev, xmedia_bool enable);
} xmedia_sensor_register_isp_func;

typedef struct {
    // 曝光比索引按升序排列, 0: 次短帧比最短帧; XMEDIA_ISP_WDR_CHN_MAX_NUM - 1: 最长帧比次长帧
    xmedia_u32 ratio[XMEDIA_ISP_WDR_EXP_RATIO_MAX_NUM];
    // 索引按升序排列, 0: 最短帧; XMEDIA_ISP_WDR_CHN_MAX_NUM - 1: 最长帧;
    xmedia_u32 max_inttime[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    // 索引按升序排列, 0: 最短帧; XMEDIA_ISP_WDR_CHN_MAX_NUM - 1: 最长帧;
    xmedia_u32 min_inttime[XMEDIA_ISP_WDR_CHN_MAX_NUM];
} xmedia_sensor_ae_inttime_attr;

typedef struct {
    xmedia_s32 (*pfn_sensor_get_ae_default)(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft);
    xmedia_s32 (*pfn_sensor_set_fps)(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft);
    xmedia_s32 (*pfn_sensor_set_slow_framerate)(xmedia_u32 dev, xmedia_u32 full_lines,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft);

    xmedia_s32 (*pfn_sensor_update_inttime)(xmedia_u32 dev, xmedia_u32 int_time);

    xmedia_s32 (*pfn_sensor_calc_again)(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db);
    xmedia_s32 (*pfn_sensor_calc_dgain)(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db);
    xmedia_s32 (*pfn_sensor_update_gains)(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain);

    xmedia_s32 (*pfn_sensor_get_max_inttime)(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inittime_attr);
    xmedia_s32 (*pfn_sensor_set_ae_wdr_attr)(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode);
} xmedia_sensor_register_ae_func;

typedef struct {
    xmedia_s32 (*pfn_sensor_get_awb_default)(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft);
    xmedia_s32 (*pfn_sensor_set_color_temperature)(xmedia_u32 dev, xmedia_u32 ct);
} xmedia_sensor_register_awb_func;

typedef struct {
} xmedia_sensor_register_af_func;

typedef struct {
    xmedia_u32                     dev_id;
    xmedia_sensor_register_ae_func func;
} xmedia_sensor_register_ae_info;

typedef struct {
    xmedia_u32                      dev_id;
    xmedia_sensor_register_awb_func func;
} xmedia_sensor_register_awb_info;

typedef struct {
    xmedia_u32                     dev_id;
    xmedia_sensor_register_af_func func;
} xmedia_sensor_register_af_info;

typedef struct {
    xmedia_u32                      dev_id; // sensor dev id
    xmedia_char                     sensor_name[XMEDIA_SENSOR_NAME_MAX_SIZE];
    xmedia_sensor_register_isp_func isp_func;
    xmedia_sensor_register_awb_func awb_func;
    xmedia_sensor_register_ae_func  ae_func;
} xmedia_sensor_register_info;

#define XMEDIA_SENSOR_REGISTER_DRIVER(pipe, sensor)                                                                    \
    ({                                                                                                                 \
        extern xmedia_s32 sensor##_register(xmedia_u32);                                                               \
        sensor##_register(pipe);                                                                                       \
    })

#define XMEDIA_SENSOR_UNREGISTER_DRIVER(pipe, sensor)                                                                  \
    ({                                                                                                                 \
        extern xmedia_s32 sensor##_unregister(xmedia_u32);                                                             \
        sensor##_unregister(pipe);                                                                                     \
    })

#define XMEDIA_LENS_DEV_MAX_NUM   XMEDIA_ISP_PIPE_MAX_NUM
#define XMEDIA_LENS_NAME_MAX_SIZE 128

typedef struct {
    xmedia_s32 (*pfn_lens_init)(xmedia_u32 dev);
    xmedia_s32 (*pfn_lens_exit)(xmedia_u32 dev);
    xmedia_s32 (*pfn_lens_standby)(xmedia_u32 dev);
    xmedia_s32 (*pfn_lens_resume)(xmedia_u32 dev);
    xmedia_s32 (*pfn_lens_park)(xmedia_u32 dev);
    xmedia_s32 (*pfn_lens_set_ircut)(xmedia_u32 dev, xmedia_bool enable);
} xmedia_lens_register_isp_func;

typedef struct {
    xmedia_s32 (*pfn_lens_set_aperture)(xmedia_u32 dev, xmedia_u32 index);
    xmedia_s32 (*pfn_lens_set_shutter)(xmedia_u32 dev, xmedia_bool enable);
} xmedia_lens_register_ae_func;

typedef struct {
} xmedia_lens_register_awb_func;

typedef struct {
    xmedia_s32 (*pfn_lens_zoom_in)(xmedia_u32 dev, xmedia_u32 distance);
    xmedia_s32 (*pfn_lens_zoom_out)(xmedia_u32 dev, xmedia_u32 distance);
    xmedia_s32 (*pfn_lens_zoom_pi)(xmedia_u32 dev, xmedia_u32 pi); // 0: tele, 1:wide
    xmedia_s32 (*pfn_lens_zoom_status)(xmedia_u32 dev);
    xmedia_s32 (*pfn_lens_focus_near)(xmedia_u32 dev, xmedia_u32 distance);
    xmedia_s32 (*pfn_lens_focus_far)(xmedia_u32 dev, xmedia_u32 distance);
    xmedia_s32 (*pfn_lens_focus_pi)(xmedia_u32 dev, xmedia_u32 pi); // 0: near, 1:far
    xmedia_s32 (*pfn_lens_focus_status)(xmedia_u32 dev);
} xmedia_lens_register_af_func;

typedef struct {
    xmedia_u32                   dev_id;
    xmedia_lens_register_ae_func func;
} xmedia_lens_register_ae_info;

typedef struct {
    xmedia_u32                    dev_id;
    xmedia_lens_register_awb_func func;
} xmedia_lens_register_awb_info;

typedef struct {
    xmedia_u32                   dev_id;
    xmedia_lens_register_af_func func;
} xmedia_lens_register_af_info;

typedef struct {
    xmedia_u32                    dev_id;
    xmedia_char                   lens_name[XMEDIA_LENS_NAME_MAX_SIZE];
    xmedia_lens_register_isp_func isp_func;
    xmedia_lens_register_awb_func awb_func;
    xmedia_lens_register_ae_func  ae_func;
    xmedia_lens_register_af_func  af_func;
} xmedia_lens_register_info;

#define XMEDIA_LENS_REGISTER_DRIVER(pipe, lens)                                                                        \
    ({                                                                                                                 \
        extern xmedia_s32 lens##_register(xmedia_u32);                                                                 \
        lens##_register(pipe);                                                                                         \
    })

#define XMEDIA_LENS_UNREGISTER_DRIVER(pipe, lens)                                                                      \
    ({                                                                                                                 \
        extern xmedia_s32 lens##_unregister(xmedia_u32);                                                               \
        lens##_unregister(pipe);                                                                                       \
    })

/*
 * 函数功能: 初始化sensor
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      cfg  - 输入参数，sensor初始化配置参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_EXIST - sensor重复初始化
 */
xmedia_s32 xmedia_sensor_init(xmedia_u32 pipe, xmedia_sensor_config *cfg);

/*
 * 函数功能: 反初始化sensor
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 *      XMEDIA_ERRCODE_NOT_INIT - sensor未初始化
 */
xmedia_s32 xmedia_sensor_exit(xmedia_u32 pipe);
/*
 * 函数功能: 获取当前规格下sensor的属性
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      property - 输出参数，sensor当前规格下的属性配置
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 * 注意: 建议先调用xmedia_sensor_set_attr,然后再调用此接口获取属性
 */
xmedia_s32 xmedia_sensor_get_property(xmedia_u32 pipe, xmedia_sensor_property *property);

/*
 * 函数功能: 配置sensor工作模式
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      work_mode - 输入参数，sensor工作模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：默认sensor工作在主模式，因此主模式时为非必调接口, 从模式时为必调接口
 *      xmedia_sensor_init后xmedia_sensor_start前调用
 */
xmedia_s32 xmedia_sensor_set_work_mode(xmedia_u32 pipe, xmedia_sensor_work_mode mode);

/*
 * 函数功能: SLAVE模式下，配置Sensor与触发信号的绑定关系
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      signal - 输入参数，Sensor触发信号的类型和设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 * 注意：SLAVE从模式下, 为必调接口, xmedia_sensor_init后xmedia_sensor_start前调用
 */
xmedia_s32 xmedia_sensor_set_trig_signal(xmedia_u32 pipe, xmedia_sensor_trig_signal *signal);

/*
 * 函数功能: 配置sensor属性
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      sns_attr - 输入参数，sensor属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意： xmedia_sensor_init后，xmedia_sensor_start前调用
 */
xmedia_s32 xmedia_sensor_set_attr(xmedia_u32 pipe, const xmedia_sensor_attr *sns_attr);

/*
 * 函数功能: MIPI模式传输sensor数据时，配置MIPI Lane数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      mipi_lanes - 输入参数，sensor mipi lanes
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：xmedia_sensor_init后xmedia_sensor_start前调用
 */
xmedia_s32 xmedia_sensor_set_mipi_lanes(xmedia_u32 pipe, xmedia_sensor_mipi_lanes mipi_lanes);

/*
 * 函数功能: 配置sensor寻址地址
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      slave_addr - 输入参数，sensor 寻址地址
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口非必调接口，用于多路输入时，需一个I2C通过不同的寻址地址控制多个同型号sensor时调用;
 *      xmedia_sensor_init后xmedia_sensor_start前调用
 */
xmedia_s32 xmedia_sensor_set_dev_addr(xmedia_u32 pipe, xmedia_u32 slave_addr);

/*
 * 函数功能: 创建sensor功能组
 * 函数参数：
 *      group_cfg - 输入参数，group功能组的参数配置
 *      group - 输出参数，group功能组的handle
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 * 注意：该接口非必调接口，用于多路输入时，同类型功能的sensor创建功能组，以便于同步和同一操作;
 *      group的pipe_num数量不能低于XMEDIA_SENSOR_GROUP_MIN_PIPE_NUM。
 *      xmedia_sensor_init后xmedia_sensor_start前调用，如果是HOST_SLAVE/MASTER_SLAVE的功能组，
 *      需要先通过xmedia_sensor_set_work_mode、xmedia_sensor_set_trig_signal配置主从模式和触发信号。
 */
xmedia_s32 xmedia_sensor_create_group(xmedia_sensor_group_config *group_cfg, xmedia_handle *group);

/*
 * 函数功能: 销毁sensor功能组
 * 函数参数：
 *      group - 输入参数，group功能组的handle
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_INVALID_GRP_ID - 无效group handle
 * 注意：该接口非必调接口，如果创建了group功能组，需要功能组内每个sensor
 *       在xmedia_sensor_stop后xmedia_sensor_exit前调用本接口。
 */
xmedia_s32 xmedia_sensor_destroy_group(xmedia_handle group);

/*
 * 函数功能: 启动sensor
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：调用xmedia_sensor_start之前，需要先确保xmedia_isp_init已配置;
 *       sensor从模式下，还需确保xmedia_sensor_set_trig_signal已配置。
 *       如果需要使用sensor group功能，需要确认group已创建。
 */
xmedia_s32 xmedia_sensor_start(xmedia_u32 pipe);

/*
 * 函数功能: 停止sensor
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_INIT - 未初始化sensor
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口调用前，需确保xmedia_isp_init已配置
 */
xmedia_s32 xmedia_sensor_stop(xmedia_u32 pipe);

/*
 * 函数功能: 待机sensor
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口调用前，需确保xmedia_isp_init已配置
 */
xmedia_s32 xmedia_sensor_standby(xmedia_u32 pipe);

/*
 * 函数功能: 对standby状态下的sensor进行恢复操作
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口调用前，需确保xmedia_isp_init已配置
 */
xmedia_s32 xmedia_sensor_resume(xmedia_u32 pipe);

/*
 * 函数功能: 对standby状态下的sensor 配置低功耗模式
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      enable - 输入参数，sensor 低功耗使能开关, TRUE: 低功耗使能；FALSE:退出低功耗
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口调用前，需确保xmedia_sensor_standby已配置
 */
xmedia_s32 xmedia_sensor_low_power(xmedia_u32 pipe, xmedia_bool enable);

/*
 * 函数功能: 配置sensor镜像模式
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      enable - 输入参数，镜像模式使能开关，TRUE：镜像模式；FALSE：非镜像模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口调用前，需确保xmedia_sensor_start已配置
 */
xmedia_s32 xmedia_sensor_mirror(xmedia_u32 pipe, xmedia_bool enable);

/*
 * 函数功能: 配置sensor翻转模式
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      enable - 输入参数，翻转模式使能开关，TRUE：翻转模式；FALSE：非翻转模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口调用前，需确保xmedia_sensor_start已配置
 */
xmedia_s32 xmedia_sensor_flip(xmedia_u32 pipe, xmedia_bool enable);

/*
 * 函数功能: 写sensor寄存器操作
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      addr - 输入参数，待写寄存器的地址
 *      data - 输入参数，待写的寄存器的值
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_INIT - sensor未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 * 注意：该接口调用前，需确保xmedia_sensor_init已配置
 *
 */
xmedia_s32 xmedia_sensor_write_reg(xmedia_u32 pipe, xmedia_u32 addr, xmedia_u32 data);

/*
 * 函数功能: 读sensor寄存器操作
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      addr - 输入参数，待读的sensor寄存器地址
 *      data - 输出参数，sensor寄存器的值
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_INIT - sensor未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 * 注意：该接口调用前，需确保xmedia_sensor_init已配置
 */
xmedia_s32 xmedia_sensor_read_reg(xmedia_u32 pipe, xmedia_u32 addr, xmedia_u32 *data);

/*
 * 函数功能: 配置AE/AWB相关默认参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      init_param -  AE/AWB初始化参数结构体
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 * 注意：该接口一般用于快启，默认参数一般通过标定获取
 */
xmedia_s32 xmedia_sensor_set_init_param(xmedia_u32 pipe, const xmedia_sensor_init_param *init_param);

/*
 * 函数功能: 获取AE/AWB相关默认参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      init_param -  AE/AWB初始化参数结构体
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 * 注意：该接口一般用于快启，获取MCU端的收敛结果
 */
xmedia_s32 xmedia_isp_get_sensor_init_param(xmedia_u32 pipe, xmedia_sensor_init_param *init_param);

/*
 * 函数功能: 获取sensor的车规安全状态及信息
 * 函数参数：
 *      pipe   - 输入参数，pipe号
 *      status - 输出参数，当前的安全状态，其值可能是一种错误状态或几种错误状态的位或。
 *      info   - 输出参数，与异常状态相关的详细信息。
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入sensor dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 此函数不支持
 */
xmedia_s32 xmedia_sensor_get_safety_status(xmedia_u32 pipe, xmedia_u32 *status, xmedia_u32 *info);

/*
 * 函数功能: 初始化lens
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入lens dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - lens未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_EXIST - lens重复初始化
 */
xmedia_s32 xmedia_lens_init(xmedia_u32 pipe);

/*
 * 函数功能: 反初始化lens
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入lens dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - lens未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 *      XMEDIA_ERRCODE_NOT_INIT - lens未初始化
 */
xmedia_s32 xmedia_lens_exit(xmedia_u32 pipe);

/*
 * 函数功能: 待机lens
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入lens dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - lens未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 */
xmedia_s32 xmedia_lens_standby(xmedia_u32 pipe);

/*
 * 函数功能: 对standby状态下的lens进行恢复操作
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入lens dev无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - lens未注册
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的规格
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不被允许的操作
 */
xmedia_s32 xmedia_lens_resume(xmedia_u32 pipe);

/*
 * 函数功能: 初始化ISP
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      cfg - 输入参数，ISP 初始化配置信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_READY - ISP设备未打开
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 未注册sensor时, 访问或配置senosr相关接口或参数
 *      XMEDIA_ERRCODE_EXIST - 已初始化
 *      XMEDIA_ERRCODE_NOT_INIT - 未初始化
 */
xmedia_s32 xmedia_isp_init(xmedia_u32 pipe, xmedia_isp_config *cfg);

/*
 * 函数功能: 退出ISP
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - 未初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许, ISP停止前调用或重复退出
 */
xmedia_s32 xmedia_isp_exit(xmedia_u32 pipe);

/*
 * 函数功能: 获取ISP初始化参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      cfg - 输出参数，ISP 初始化配置信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - 目标不存在
 */
xmedia_s32 xmedia_isp_get_config(xmedia_u32 pipe, xmedia_isp_config *cfg);

/*
 * 函数功能: ISP 启动
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_NOT_INIT - 未初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 * 注意：启动前，需确保xmedia_isp_init已配置
 */
xmedia_s32 xmedia_isp_start(xmedia_u32 pipe);

/*
 * 函数功能: ISP 停止
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_stop(xmedia_u32 pipe);

/*
 * 函数功能: ISP Firmware 停止/启动 run
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      freeze_en - 输入参数，冻结使能
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - 目标不存在
 */
xmedia_s32 xmedia_isp_freeze(xmedia_u32 pipe, xmedia_bool freeze_en);

/*
 * 函数功能: 导出算法参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      module_info - 输出参数，算法模块信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_NOT_EXIST - 目标不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 *      XMEDIA_ERRCODE_NOT_INIT - 未初始化
 */
xmedia_s32 xmedia_isp_export(xmedia_u32 pipe, xmedia_isp_module_info module_info[XMEDIA_ISP_MODULE_MAX_NUM]);

/*
 * 函数功能: 导入算法参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      module_info - 输入参数，算法模块信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_NOT_EXIST - 目标不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 *      XMEDIA_ERRCODE_NOT_INIT - 未初始化
 */
xmedia_s32 xmedia_isp_import(xmedia_u32 pipe, const xmedia_isp_module_info module_info[XMEDIA_ISP_MODULE_MAX_NUM]);

/*
 * 函数功能: 配置图像属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      attr - 输入参数，图像属性信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - 目标不存在
 * 注意：动态切换sensor帧率，分辨率，模式时调用
 */
xmedia_s32 xmedia_isp_set_attr(xmedia_u32 pipe, const xmedia_isp_attr *attr); // dynamic switch

/*
 * 函数功能: 获取图像属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      attr - 输出参数，图像属性信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - 目标不存在
 */
xmedia_s32 xmedia_isp_get_attr(xmedia_u32 pipe, xmedia_isp_attr *attr);

/*
 * 函数功能: ISP 提供的 sensor 注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      info - 输入参数，Sensor注册信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_EXIST - sensor已注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_register_sensor(xmedia_u32 pipe, const xmedia_sensor_register_info *info);

/*
 * 函数功能: ISP 提供的 sensor 反注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dev_id - 输入参数，向ISP注册的sensor设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入设备号无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - sensor未注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_unregister_sensor(xmedia_u32 pipe, xmedia_u32 dev_id);

/*
 * 函数功能: ISP 提供的 lens 注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      info - 输入参数，lens注册信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_EXIST - lens已注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_register_lens(xmedia_u32 pipe, const xmedia_lens_register_info *info);

/*
 * 函数功能: ISP 提供的 lens 反注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dev_id - 输入参数，向ISP注册的lens设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入设备号无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST - lens未注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_unregister_lens(xmedia_u32 pipe, xmedia_u32 dev_id);

/*
 * 函数功能: ISP 提供的 AE 库注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ae_info - 输入参数，AE库注册信息
 *      sensor_func - 输出参数，sensor向AE注册的信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_EXIST - AE库已注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 * 注意：AE向ISP注册前，需确保Sensor已经向ISP注册
 */
xmedia_s32 xmedia_isp_register_ae_lib(xmedia_u32 pipe, const xmedia_isp_ae_register_info *ae_info,
                                      xmedia_sensor_register_ae_info *sensor_func);

/*
 * 函数功能: ISP 提供的 AE 库反注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      name - 输入参数，AE向ISP注册的库的名字
 *      id - 输入参数，AE向ISP注册的CHID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST- AE库未注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_unregister_ae_lib(xmedia_u32 pipe, xmedia_char name[XMEDIA_ISP_ALG_NAME_MAX_SIZE], xmedia_s32 id);

/*
 * 函数功能: ISP 提供的 AWB 库注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      awb_info - 输入参数，AWB向ISP注册的信息
 *      sensor_func - 输出参数，sensor向AWB注册的信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_EXIST - AWB库已注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 * 注意：AWB向ISP注册前，需确保Sensor已经向ISP注册
 */
xmedia_s32 xmedia_isp_register_awb_lib(xmedia_u32 pipe, const xmedia_isp_awb_register_info *awb_info,
                                       xmedia_sensor_register_awb_info *sensor_func);

/*
 * 函数功能: ISP 提供的 AWB 库反注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      name - 输入参数，AWB向ISP注册的库的名字
 *      id - 输入参数，AWB向ISP注册的CHID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST- AWB库未注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_unregister_awb_lib(xmedia_u32 pipe, xmedia_char name[XMEDIA_ISP_ALG_NAME_MAX_SIZE],
                                         xmedia_s32 id);

/*
 * 函数功能: ISP 提供的 AF 库注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      af_info - 输入参数，AF向ISP注册的信息
 *      sensor_func - 输出参数，sensor向AF注册的信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_EXIST - AF库已注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 * 注意：AF向ISP注册前，需确保Sensor已经向ISP注册
 *
 */
xmedia_s32 xmedia_isp_register_af_lib(xmedia_u32 pipe, const xmedia_isp_af_register_info *af_info,
                                      xmedia_lens_register_af_info *lens_func);

/*
 * 函数功能: ISP 提供的 AF 库反注册的回调接口
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      name - 输入参数，AF向ISP注册的库的名字
 *      id - 输入参数，AF向ISP注册的CHID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_EXIST- AF库未注册
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_isp_unregister_af_lib(xmedia_u32 pipe, xmedia_char name[XMEDIA_ISP_ALG_NAME_MAX_SIZE], xmedia_s32 id);

/*
 * 函数功能: 设置多路 ISP Pipe 差异属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      differ - 输入参数，多路ISP差异属性结构体指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 * 注意：该接口不是必调接口，用于多路ISP拼接模式对拼接效果要求更高时，可以通过标定工具
 *       标定出多路 ISP 的亮度、颜色等差异，通过此接口配置减少多路 ISP 输出图像的差异。
 *       该接口必须在 xmedia_isp_init 之后调用
 */
xmedia_s32 xmedia_isp_set_differ_attr(xmedia_u32 pipe, const xmedia_isp_diff_attr *differ);

/*
 * 函数功能: 获取多路 ISP Pipe 差异属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      differ - 输出参数，多路 ISP 差异属性结构体指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 * 注意：该接口不是必调接口，与 xmedia_isp_set_differ_attr 配套使用，可以获取对应配置的多路ISP差异参数
 */
xmedia_s32 xmedia_isp_get_differ_attr(xmedia_u32 pipe, xmedia_isp_diff_attr *differ);

/*
 * 函数功能: 获取ISP中断信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      wait_type - 输入参数，等待的中断信号类型
 *      millisec - 输入参数，超时时间，单位ms
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_READY - ISP设备未打开
 */
xmedia_s32 xmedia_isp_wait_state(xmedia_u32 pipe, xmedia_isp_wait_type wait_type, xmedia_u32 millisec);

/*
 * 函数功能: 设置 ISP 调试信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      isp_debug - 输入参数，调试配置信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_debug(xmedia_u32 pipe, const xmedia_isp_debug_info *isp_debug);

/*
 * 函数功能: 获取 ISP 调试信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      isp_debug - 输出参数，调试配置信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_debug(xmedia_u32 pipe, xmedia_isp_debug_info *isp_debug);

/*
 * 函数功能: 设置 ISP 控制参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      isp_ctrl_attr - 输入参数，控制信息结构体指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_ctrl_param(xmedia_u32 pipe, const xmedia_isp_ctrl_param *isp_ctrl_attr);

/*
 * 函数功能: 获取 ISP 控制参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      isp_ctrl_attr - 输出参数，控制信息结构体指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_ctrl_param(xmedia_u32 pipe, xmedia_isp_ctrl_param *isp_ctrl_attr);

/*
 * 函数功能: 向 ISP 模块传递智能信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      smart_info - 输入参数，智能信息结构体指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_smart_info(xmedia_u32 pipe, const xmedia_isp_smart_info *smart_info);

/*
 * 函数功能: 从 ISP 模块获取智能信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      smart_info - 输出参数，智能信息结构体指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_smart_info(xmedia_u32 pipe, xmedia_isp_smart_info *smart_info);

/*
 * 函数功能: 设置 ISP 3A 统计信息配置
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      stat_cfg - 输入参数，ISP 3A统计信息配置
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_statistics_config(xmedia_u32 pipe, const xmedia_isp_statistics_cfg *stat_cfg);

/*
 * 函数功能: 获取 ISP 3A 统计信息配置
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      stat_cfg - 输出参数，ISP 3A统计信息配置
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 */
xmedia_s32 xmedia_isp_get_statistics_config(xmedia_u32 pipe, xmedia_isp_statistics_cfg *stat_cfg);

/*
 * 函数功能: 获取 AE 统计信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ae_stat - 输出参数，AE统计信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_READY - ISP设备未打开
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *
 */
xmedia_s32 xmedia_isp_get_ae_statistics(xmedia_u32 pipe, xmedia_isp_ae_statistics *ae_stat);

/*
 * 函数功能: 获取 AE 融合统计信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ae_blend_stat - 输出参数，AE融合统计信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 * 注意：非必调接口，用于获取多路ISP融合后AE的统计信息
 */
xmedia_s32 xmedia_isp_get_ae_blend_statistics(xmedia_u32 pipe, xmedia_isp_ae_blend_statistics *ae_blend_stat);

/*
 * 函数功能: 获取 AWB 统计信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      awb_stat - 输出参数，AWB统计信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_READY - ISP设备未打开
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_isp_get_awb_statistics(xmedia_u32 pipe, xmedia_isp_awb_statistics *awb_stat);

/*
 * 函数功能: 获取 AWB融合统计信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      awb_blend_stat - 输出参数，AWB融合统计信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 * 注意：非必调接口，用于获取多路ISP融合后AWB的统计信息
 */

xmedia_s32 xmedia_isp_get_awb_blend_statistics(xmedia_u32 pipe, xmedia_isp_awb_blend_statistics *awb_blend_stat);

/*
 * 函数功能: 获取 AF统计信息
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      af_stat - 输出参数， AF统计信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_READY - ISP设备未打开
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_isp_get_af_statistics(xmedia_u32 pipe, xmedia_isp_af_statistics *af_stat);

/*
 * 函数功能: 设定算法调试属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      mode - 输入参数，算法调试属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 * 注意：各算法模块调试模式是互斥的，同一时间仅一个模块调试模式设置有效
 */
xmedia_s32 xmedia_isp_set_debug_mode(xmedia_u32 pipe, const xmedia_isp_debug_mode *mode);

/*
 * 函数功能: 获取算法调试属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      mode - 输出参数，算法调试属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_debug_mode(xmedia_u32 pipe, xmedia_isp_debug_mode *mode);

/*
 * 函数功能: 设置高光遮蔽的属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      hlc_attr - 输入参数，图像的高光遮蔽属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 */
xmedia_s32 xmedia_isp_set_hlc_attr(xmedia_u32 pipe, const xmedia_isp_hlc_attr *hlc_attr);

/*
 * 函数功能: 获取图像高光遮蔽的属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      hlc_attr - 输出参数，图像的高光遮蔽属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_hlc_attr(xmedia_u32 pipe, xmedia_isp_hlc_attr *hlc_attr);

/*
 * 函数功能: 设置GAMMA属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      gamma_attr - 输入参数，gamma属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *
 */
xmedia_s32 xmedia_isp_set_gamma_attr(xmedia_u32 pipe, const xmedia_isp_gamma_attr *gamma_attr);

/*
 * 函数功能: 获取gamma属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      gamma_attr - 输出参数，gamma属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_gamma_attr(xmedia_u32 pipe, xmedia_isp_gamma_attr *gamma_attr);

/*
 * 函数功能: 设置动态范围压缩参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      drc_attr - 输入参数，DRC属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_drc_attr(xmedia_u32 pipe, const xmedia_isp_drc_attr *drc_attr);

/*
 * 函数功能: 获取动态范围压缩参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      drc_attr - 输出参数，DRC属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_drc_attr(xmedia_u32 pipe, xmedia_isp_drc_attr *drc_attr);

/*
 * 函数功能: 设置mesh shading算法参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      shading_attr - 输入参数，Mesh Shading 算法参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_mesh_shading_attr(xmedia_u32 pipe, const xmedia_isp_mesh_shading_attr *shading_attr);

/*
 * 函数功能: 获取mesh shading算法参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      shading_attr - 输出参数，Mesh Shading 算法参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_mesh_shading_attr(xmedia_u32 pipe, xmedia_isp_mesh_shading_attr *shading_attr);

/*
 * 函数功能: 设置mesh shading增益表属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      shading_gain_lut_attr - 输入参数，Mesh Shading增益表属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_mesh_shading_gain_lut_attr(xmedia_u32                              pipe,
                                                     const xmedia_isp_mesh_shading_lut_attr *shading_gain_lut_attr);

/*
 * 函数功能: 获取mesh shading增益表属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      shading_gain_lut_attr - 输出参数，Mesh Shading增益表属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_mesh_shading_gain_lut_attr(xmedia_u32                        pipe,
                                                     xmedia_isp_mesh_shading_lut_attr *shading_gain_lut_attr);

/*
 * 函数功能: 设置静态坏点标定参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dpc_calibrate - 输入参数，静态坏点标定参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 * 注意：fpn标定, bpc标定, ainr, dis, pdaf功能互斥, 不能同时开启.
 *       不同芯片版本的差异详见文档:ISP开发参考
 */
xmedia_s32 xmedia_isp_set_dpc_calibrate(xmedia_u32 pipe, const xmedia_isp_dpc_static_calibrate *dpc_calibrate);

/*
 * 函数功能: 获取静态坏点标定参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dpc_calibrate - 输出参数，静态坏点标定参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_dpc_calibrate(xmedia_u32 pipe, xmedia_isp_dpc_static_calibrate *dpc_calibrate);

/*
 * 函数功能: 设置静态坏点属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dpc_static_attr - 输入参数，静态坏点标定参数属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_dpc_static_attr(xmedia_u32 pipe, const xmedia_isp_dpc_static_attr *dpc_static_attr);

/*
 * 函数功能: 获取静态坏点属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dpc_static_attr - 输出参数，静态坏点标定参数属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_dpc_static_attr(xmedia_u32 pipe, xmedia_isp_dpc_static_attr *dpc_static_attr);

/*
 * 函数功能: 设置动态坏点属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dpc_dynamic_attr - 输入参数，动态坏点标定参数属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_dpc_dynamic_attr(xmedia_u32 pipe, const xmedia_isp_dpc_dynamic_attr *dpc_dynamic_attr);

/*
 * 函数功能: 获取动态坏点属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dpc_dynamic_attr - 输出参数，动态坏点标定参数属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_dpc_dynamic_attr(xmedia_u32 pipe, xmedia_isp_dpc_dynamic_attr *dpc_dynamic_attr);

/*
 * 函数功能: 设置crosstalk 属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      cr_attr - 输入参数，crosstalk 属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_crosstalk_attr(xmedia_u32 pipe, const xmedia_isp_cr_attr *cr_attr);

/*
 * 函数功能: 获取crosstalk 属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      cr_attr - 输出参数，crosstalk 属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_crosstalk_attr(xmedia_u32 pipe, xmedia_isp_cr_attr *cr_attr);

/*
 * 函数功能: 设置BNR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      bnr_attr - 输入参数，BNR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_bnr_attr(xmedia_u32 pipe, const xmedia_isp_bnr_attr *bnr_attr);

/*
 * 函数功能: 获取BNR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      bnr_attr - 输出参数，BNR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_bnr_attr(xmedia_u32 pipe, xmedia_isp_bnr_attr *bnr_attr);

/*
 * 函数功能: 设置去雾属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dehaze_attr - 输入参数，去雾属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_dehaze_attr(xmedia_u32 pipe, const xmedia_isp_dehaze_attr *dehaze_attr);

/*
 * 函数功能: 获取去雾属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      dehaze_attr - 输出参数，去雾属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_dehaze_attr(xmedia_u32 pipe, xmedia_isp_dehaze_attr *dehaze_attr);

/*
 * 函数功能: 设置DMS属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      demosaic_attr - 输入参数，去雾属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_demosaic_attr(xmedia_u32 pipe, const xmedia_isp_demosaic_attr *demosaic_attr);

/*
 * 函数功能: 获取DMS属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      demosaic_attr - 输出参数，去雾属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_demosaic_attr(xmedia_u32 pipe, xmedia_isp_demosaic_attr *demosaic_attr);

/*
 * 函数功能: 设置黑电平属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      black_level - 输入参数，黑电平属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_black_level_attr(xmedia_u32 pipe, const xmedia_isp_blc_attr *black_level);

/*
 * 函数功能: 获取黑电平属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      black_level - 输出参数，黑电平属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_black_level_attr(xmedia_u32 pipe, xmedia_isp_blc_attr *black_level);

/*
 * 函数功能: 设定去FPN标定属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      calibrate_attr - 输入参数，去 FPN 标定属性指针
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 * 注意：fpn标定, bpc标定, ainr, dis, pdaf功能互斥, 不能同时开启.
 *       不同芯片版本的差异详见文档:ISP开发参考
 */
xmedia_s32 xmedia_isp_set_fpn_calibrate(xmedia_u32 pipe, const xmedia_isp_fpn_calibrate_attr *calibrate_attr);

/*
 * 函数功能: 设定去FPN属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      fpn_attr - 输入参数，FPN属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_fpn_attr(xmedia_u32 pipe, const xmedia_isp_fpn_attr *fpn_attr);

/*
 * 函数功能: 获取去FPN属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      fpn_attr 输出参数，FPN属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_fpn_attr(xmedia_u32 pipe, xmedia_isp_fpn_attr *fpn_attr);

/*
 * 函数功能: 设置横向色差校正参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      gcac_attr - 输入参数，横向色差校正参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_gcac_attr(xmedia_u32 pipe, const xmedia_isp_gcac_attr *gcac_attr);

/*
 * 函数功能: 获取横向色差校正参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      gcac_attr - 输出参数，横向色差校正参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_gcac_attr(xmedia_u32 pipe, xmedia_isp_gcac_attr *gcac_attr);

/*
 * 函数功能: 设置去除紫边参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      lcac_attr - 输入参数，去除紫边参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_lcac_attr(xmedia_u32 pipe, const xmedia_isp_lcac_attr *lcac_attr);

/*
 * 函数功能: 获取去除紫边参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      lcac_attr - 输出参数，去除紫边参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_lcac_attr(xmedia_u32 pipe, xmedia_isp_lcac_attr *lcac_attr);

/*
 * 函数功能: 设置去伪彩参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      anti_false_color_attr - 输入参数，去除紫边参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_anti_false_color_attr(xmedia_u32                              pipe,
                                                const xmedia_isp_anti_false_color_attr *anti_false_color_attr);

/*
 * 函数功能: 获取去伪彩参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      anti_false_color_attr - 输出参数，去除紫边参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_anti_false_color_attr(xmedia_u32                        pipe,
                                                xmedia_isp_anti_false_color_attr *anti_false_color_attr);

/*
 * 函数功能: 设置expander参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      expander_attr - 输入参数，expander参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_set_expander_attr(xmedia_u32 pipe, const xmedia_isp_expander_attr *expander_attr);

/*
 * 函数功能: 获取expander参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      expander_attr - 输出参数，expander参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_expander_attr(xmedia_u32 pipe, xmedia_isp_expander_attr *expander_attr);

/*
 * 函数功能: 设置WDR帧合成参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      wdr_attr - 输入参数，帧合成参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_wdr_attr(xmedia_u32 pipe, const xmedia_isp_wdr_attr *wdr_attr);

/*
 * 函数功能: 获取WDR帧合成参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      wdr_attr - 输出参数，帧合成参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_wdr_attr(xmedia_u32 pipe, xmedia_isp_wdr_attr *wdr_attr);

/*
 * 函数功能: 设置LCE属性参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      lce_attr - 输入参数，LCE属性参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_lce_attr(xmedia_u32 pipe, const xmedia_isp_lce_attr *lce_attr);

/*
 * 函数功能: 获取Chroma Adjust属性参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ca_attr 输出参数，Chroma Adjust属性参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_ca_attr(xmedia_u32 pipe, xmedia_isp_ca_attr *ca_attr);

/*
 * 函数功能: 设置Chroma Adjust属性参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ca_attr - 输入参数，Chroma Adjust属性参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_ca_attr(xmedia_u32 pipe, const xmedia_isp_ca_attr *ca_attr);

/*
 * 函数功能: 获取LCE属性参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      lce_attr 输出参数，LCE属性参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_lce_attr(xmedia_u32 pipe, xmedia_isp_lce_attr *lce_attr);

/*
 * 函数功能: 获取LCE信息参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      lce_info 输出参数，LCE信息参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_lce_info(xmedia_u32 pipe, xmedia_isp_lce_info *lce_info);

/*
 * 函数功能: 设置Radial Crop的参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      rc_attr 输入参数，Radial Crop的参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_set_rc_attr(xmedia_u32 pipe, const xmedia_isp_rc_attr *rc_attr);

/*
 * 函数功能: 获取Radial Crop的参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      rc_attr - 输出参数，Radial Crop的参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_get_rc_attr(xmedia_u32 pipe, xmedia_isp_rc_attr *rc_attr);

/*
 * 函数功能: 设置 CSC（色彩空间转换） 的参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      csc_attr - 输入参数，CSC的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_csc_attr(xmedia_u32 pipe, const xmedia_isp_csc_attr *csc_attr);

/*
 * 函数功能: 获取 CSC（色彩空间转换） 的参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      csc_attr - 输出参数，CSC的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_csc_attr(xmedia_u32 pipe, xmedia_isp_csc_attr *csc_attr);

/*
 * 函数功能: 设置  CLUT 的属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      clut_attr - 输入参数，CLUT 的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_clut_attr(xmedia_u32 pipe, const xmedia_isp_clut_attr *clut_attr);

/*
 * 函数功能: 获取  CLUT 的属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      clut_attr - 输出参数，CLUT 的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_clut_attr(xmedia_u32 pipe, xmedia_isp_clut_attr *clut_attr);

/*
 * 函数功能: 设定 Radial Shading 属性参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ra_shading_attr - 输入参数，Radial Shading 属性参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_radial_shading_attr(xmedia_u32 pipe, const xmedia_isp_radial_shading_attr *ra_shading_attr);

/*
 * 函数功能: 获取Radial Shading 属性参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ra_shading_attr - 输出参数，Radial Shading 属性参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_radial_shading_attr(xmedia_u32 pipe, xmedia_isp_radial_shading_attr *ra_shading_attr);

/*
 * 函数功能: 设置 Radial Shading 对应查找表参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ra_shading_lut_attr 输入参数，Radial Shading 对应查找表参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_set_radial_shading_lut(xmedia_u32                                pipe,
                                             const xmedia_isp_radial_shading_lut_attr *ra_shading_lut_attr);

/*
 * 函数功能: 获取 Radial Shading 对应查找表参数
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ra_shading_lut_attr - 输出参数，Radial Shading 对应查找表参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 */
xmedia_s32 xmedia_isp_get_radial_shading_lut(xmedia_u32 pipe, xmedia_isp_radial_shading_lut_attr *ra_shading_lut_attr);

/*
 * 函数功能: 设定RGBIR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      rgbir_attr - 输入参数，RGBIR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_set_rgbir_attr(xmedia_u32 pipe, const xmedia_isp_rgbir_attr *rgbir_attr);

/*
 * 函数功能: 获取RGBIR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      rgbir_attr - 输出参数，RGBIR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_get_rgbir_attr(xmedia_u32 pipe, xmedia_isp_rgbir_attr *rgbir_attr);

/*
 * 函数功能: 设定SHARPEN属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      sharpen_attr - 输入参数，SHARPEN属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_set_sharpen_attr(xmedia_u32 pipe, const xmedia_isp_sharpen_attr *sharpen_attr);

/*
 * 函数功能: 获取SHARPEN属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      sharpen_attr - 输出参数，SHARPEN属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_get_sharpen_attr(xmedia_u32 pipe, xmedia_isp_sharpen_attr *sharpen_attr);

/*
 * 函数功能: 设定STNR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      stnr_attr - 输入参数，STNR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_set_stnr_attr(xmedia_u32 pipe, const xmedia_isp_stnr_attr *stnr_attr);

/*
 * 函数功能: 获取STNR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      stnr_attr - 输出参数，STNR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_get_stnr_attr(xmedia_u32 pipe, xmedia_isp_stnr_attr *stnr_attr);

/*
 * 函数功能: 加载AI ISP模型
 * 函数参数：
 *      model - 输入参数，存储AI模型的buffer地址
 *      handle - 输出参数，AI模型加载成功后的句柄
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_BUFFER_FULL - 模型存储空间已满
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存分配失败
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 * 注意：该接口调用前，需确保xmedia_isp_init已调用。该接口不支持多进程调用。
 */
xmedia_s32 xmedia_isp_load_model(const xmedia_void *model, xmedia_handle *handle);

/*
 * 函数功能: 卸载AI ISP模型
 * 函数参数：
 *      handle - 输入参数，要卸载的AI模型句柄
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 模型未解除绑定，不允许卸载
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
  * 注意：该接口调用前，需确保该模型已解除所有绑定。该接口不支持多进程调用。
 */
xmedia_s32 xmedia_isp_unload_model(xmedia_handle handle);

/*
 * 函数功能: 绑定ISP和AI模型
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      handle - 输入参数，要绑定的AI模型句柄
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_BUFFER_FULL - 模型存储空间已满
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不允许绑定的情形
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 * 注意：该接口调用前，需确保xmedia_isp_init已调用。该接口不支持多进程调用。
 */
xmedia_s32 xmedia_isp_bind_model(xmedia_u32 pipe, xmedia_handle handle[XMEDIA_ISP_AI_MODEL_MAX_NUM]);

/*
 * 函数功能: 解除ISP和AI模型绑定
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      handle - 输入参数，要绑定的AI模型句柄
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 * 注意：该接口调用时，该模型相关的业务将被停止。该接口不支持多进程调用。
 */
xmedia_s32 xmedia_isp_unbind_model(xmedia_u32 pipe, xmedia_handle handle[XMEDIA_ISP_AI_MODEL_MAX_NUM]);

/*
 * 函数功能: 加载AINR模型
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      model - 输入参数，存储AINR模型的buffer地址
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 * 注意：该接口调用前，需确保xmedia_isp_init已配置
 */
xmedia_s32 xmedia_isp_load_ainr_model(xmedia_u32 pipe, const xmedia_void *model[XMEDIA_ISP_AINR_MODEL_MAX_NUM]);

/*
 * 函数功能: 卸载AINR模型
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_unload_ainr_model(xmedia_u32 pipe);

/*
 * 函数功能: 设定AINR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ainr_attr - 输入参数，AINR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 * 注意：fpn标定, bpc标定, ainr, dis, pdaf功能互斥, 不能同时开启.
 *       不同芯片版本的差异详见文档:ISP开发参考
 */
xmedia_s32 xmedia_isp_set_ainr_attr(xmedia_u32 pipe, const xmedia_isp_ainr_attr *ainr_attr);

/*
 * 函数功能: 获取AINR属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      ainr_attr - 输出参数，AINR属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_INIT - ISP未初始化
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 当前版本不支持该功能
 */
xmedia_s32 xmedia_isp_get_ainr_attr(xmedia_u32 pipe, xmedia_isp_ainr_attr *ainr_attr);

/*
 * 函数功能: 获取AINR状态属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      state - 输出参数，AINR 状态属性。TRUE: AINR已激活，FALSE: AINR未激活。
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_isp_get_ainr_state(xmedia_u32 pipe, xmedia_bool *state);

#ifdef __cplusplus
}
#endif

#endif // __XMEDIA_ISP_H__
