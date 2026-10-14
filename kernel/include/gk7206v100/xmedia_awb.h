/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __XMEDIA_API_AWB_H__
#define __XMEDIA_API_AWB_H__

#include "xmedia_type.h"
#include "xmedia_isp.h"

#ifdef __cplusplus
extern "C" {
#endif // End of #ifdef __cplusplus

typedef struct {
    xmedia_u16 red_cast_gain;   // RW; Range: [0x100, 0x180]; Format:4.8; R channel gain after CCM
    xmedia_u16 green_cast_gain; // RW; Range: [0x100, 0x180]; Format:4.8; G channel gain after CCM
    xmedia_u16 blue_cast_gain;  // RW; Range: [0x100, 0x180]; Format:4.8; B channel gain after CCM
} xmedia_awb_color_tone_attr;

typedef enum {
    XMEDIA_AWB_INDOOR_MODE = 0,
    XMEDIA_AWB_OUTDOOR_MODE,
    XMEDIA_AWB_INDORR_OUTDOOR_MAX
} xmedia_awb_indoor_outdoor_status;

typedef struct {
    xmedia_u16 rgain;      // R; Range: [0x0, 0xFFF]; Format:8.8;AWB result of R color channel
    xmedia_u16 grgain;     // R; Range: [0x0, 0xFFF]; Format:8.8; AWB result of Gr color channel
    xmedia_u16 gbgain;     // R; Range: [0x0, 0xFFF]; Format:8.8; AWB result of Gb color channel
    xmedia_u16 bgain;      // R; Range: [0x0, 0xFFF]; Format:8.8; AWB result of B color channel
    xmedia_u16 saturation; // R; Range: [0x0, 0xFF];Format:8.0;Current saturation
    xmedia_u16 ct;         // R; Range: [0x0, 0xFFFF];Format:16.0;Detect color temperature, maybe out of color
                           // cemeprature range
    xmedia_u16 ccm[XMEDIA_ISP_CCM_MATRIX_SIZE]; // R; Range: [0x0, 0xFFFF];Format:16.0;Current color correction matrix

    xmedia_u16 ls0_ct;   // R; Range: [0x0, 0xFFFF];Format:16.0;color tempearture of primary light source
    xmedia_u16 ls1_ct;   // R; Range: [0x0, 0xFFFF];Format:16.0;color tempearture of secondary light source
    xmedia_u16 ls0_area; // R; Range: [0x0, 0xFF];Format:8.0;area of primary light source
    xmedia_u16 ls1_area; // R; Range: [0x0, 0xFF];Format:8.0;area of secondary light source

    xmedia_u8  multi_degree; // R; Range:[0x0, 0xFF];0 means uniform light source, larger value means multi light source
    xmedia_u16 active_shift; // R; Range;[0x0,0xFF]
    xmedia_u32 first_stable_time; // R, Range: [0x0, 0xFFFFFFFF];Format:32.0;AWB first stable frame number
    xmedia_awb_indoor_outdoor_status inout_status; // R; indoor or outdoor status
    xmedia_s16                       bv;           // R; Range;[-32768, 32767]; Bv value
} xmedia_awb_info;

typedef struct {
    xmedia_u8 saturation;
} xmedia_awb_saturation_manual;

typedef struct {
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  sat[XMEDIA_ISP_ISO_MAX_COUNT];
} xmedia_awb_saturation_auto;

typedef struct {
    xmedia_video_operation_mode  op_type;
    xmedia_awb_saturation_manual manual_attr;
    xmedia_awb_saturation_auto   auto_attr;
} xmedia_awb_saturation_attr;

typedef struct {
    xmedia_bool sat_en;
    xmedia_u16  ccm[XMEDIA_ISP_CCM_MATRIX_SIZE];
} xmedia_awb_ccm_manual;

typedef struct {
    xmedia_u16 ct;
    xmedia_u16 ccm[XMEDIA_ISP_CCM_MATRIX_SIZE];
} xmedia_awb_ccm_param;

typedef struct {
    xmedia_bool          iso_act_en;
    xmedia_bool          temp_act_en;
    xmedia_u16           ccm_tab_num;
    xmedia_awb_ccm_param ccm_tab[XMEDIA_ISP_CCM_TAB_MAX_NUM];
} xmedia_awb_ccm_auto;

typedef struct {
    xmedia_bool                 ccm_en;
    xmedia_video_operation_mode op_type;
    xmedia_awb_ccm_manual       manual_attr;
    xmedia_awb_ccm_auto         auto_attr;
} xmedia_awb_ccm_attr;

typedef struct {
    xmedia_u16 hue_shift[XMEDIA_ISP_COLOR_SECTOR_NUM];
    xmedia_u16 sat_shift[XMEDIA_ISP_COLOR_SECTOR_NUM];
} xmedia_color_sector_manual;

typedef struct {
    xmedia_u8  ct_num;
    xmedia_u32 ct_level[XMEDIA_ISP_CCM_TAB_MAX_NUM];
    xmedia_u16 hue_shift[XMEDIA_ISP_CCM_TAB_MAX_NUM][XMEDIA_ISP_COLOR_SECTOR_NUM];
    xmedia_u16 sat_shift[XMEDIA_ISP_CCM_TAB_MAX_NUM][XMEDIA_ISP_COLOR_SECTOR_NUM];
} xmedia_color_sector_auto;

typedef struct {
    xmedia_bool                 sector_en;
    xmedia_video_operation_mode op_type;
    xmedia_color_sector_manual  manual_attr;
    xmedia_color_sector_auto    auto_attr;
} xmedia_color_sector_attr;

typedef enum {
    XMEDIA_AWB_MULTI_LS_SAT = 0,
    XMEDIA_AWB_MULTI_LS_CCM,
    XMEDIA_AWB_MULTI_LS_MAX
} xmedia_awb_multi_ls_type;

typedef struct {
    xmedia_bool                      enable;
    xmedia_video_operation_mode      op_type;
    xmedia_awb_indoor_outdoor_status outdoorstatus;
    xmedia_u32                       out_thresh;
    xmedia_u16                       low_start;
    xmedia_u16                       low_stop;
    xmedia_u16                       high_start;
    xmedia_u16                       high_stop;
    xmedia_bool                      green_enhanceen;
    xmedia_u8                        out_shift_limit;
} xmedia_awb_in_out_attr;

typedef struct {
    xmedia_u16 white_rgain;
    xmedia_u16 white_bgain;
    xmedia_u16 exp_quant;
    xmedia_u16 light_status;
    xmedia_u16 radius;
} xmedia_awb_extra_lightsource_info;

#define XMEDIA_ISP_AWB_LS_NUM       4
#define XMEDIA_ISP_AWB_MULTI_CT_NUM 8

typedef struct {
    xmedia_u8  tolerance; // RW; Range:[0x0, 0xFF]; Format:8.0; AWB adjust tolerance
    xmedia_u8  iso_num;
    xmedia_u32 iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u8  zone_radius[XMEDIA_ISP_ISO_MAX_COUNT]; // RW; Range:[0x0, 0xFF]; Format:8.0; radius of AWB blocks
    xmedia_u16 curve_l_limit;  // RW; Range:[0x0, 0x100]; Format:9.0; Left limit of AWB Curve, recomend for indoor
                               // 0xE0,outdoor 0xE0
    xmedia_u16 curve_r_limit;  // RW; Range:[0x100, 0xFFF]; Format:12.0; Right Limit of AWB Curve,recomend for indoor
                               // 0x130, outdoor 0x120
    xmedia_bool extra_lighten; // RW; Range:[0, 1]; Format:1.0; Enable special light source function
    xmedia_awb_extra_lightsource_info light_info[XMEDIA_ISP_AWB_LS_NUM];
    xmedia_awb_in_out_attr            in_or_out;
    xmedia_bool                       multilight_source_en; // RW; Range:[0, 1]; Format:1.0;
    xmedia_awb_multi_ls_type          multi_ls_type;        // Saturation or CCM Tunning
    xmedia_u16                        multi_ls_scaler;      // RW; Range:[0x0, 0x100]; Format:12.0;
    xmedia_u16 multi_ct_bin[XMEDIA_ISP_AWB_MULTI_CT_NUM];   // RW; Range:[0, 0XFFFF]; Format:16.0;AWB Support divide the
    xmedia_u16 multi_ct_wt[XMEDIA_ISP_AWB_MULTI_CT_NUM];    // RW; Range:[0x0, 0x400];Weight for different color
                                                            // temperature, same value  of 8 means CT weight does't work
    xmedia_bool fine_tun_en;       // RW; Range:[0x0, 0x1]; Format:1.0;If enabled, skin color scene will be optimized
    xmedia_u8   fine_tun_strength; // RW; Range:[0x0, 0xFF]; Format:8.0;larger value means better performance of skin
                                   // color scene, but will increase error probability in low color temperature scene
} xmedia_awb_scene_adj_attr;

typedef struct {
    xmedia_bool                 enable;
    xmedia_video_operation_mode op_type;

    // RW; Range:[0x0, 0xFFF]; Format:4.8; in Manual Mode, user define the Max Rgain of High Color Temperature
    xmedia_u16 high_rg_limit;
    // RW; Range:[0x0, 0xFFF]; Format:4.8; in Manual Mode, user define the Min Bgain of High Color Temperature
    xmedia_u16 high_bg_limit;
    // RW; Range:[0x0, 0xFFF]; Format:4.8; limited range:[0x0, u16HighRgLimit)
    xmedia_u16 low_rg_limit;
    // RW; Range:[0, 0xFFF]; Format:4.8; limited Range:(u16HighBgLimit, 0xFFF]
    xmedia_u16 low_bg_limit;

} xmedia_awb_ct_limit_attr;

typedef struct {
    // RW; Range:[0, 1]; Format:1.0;If enabled, statistic parameter cr, cb will change according to iso
    xmedia_bool enable;
    xmedia_u8   iso_num;
    xmedia_u32  iso_level[XMEDIA_ISP_ISO_MAX_COUNT];
    xmedia_u16  crmax[XMEDIA_ISP_ISO_MAX_COUNT]; // RW; Range:[0x0, 0xFFF]; au16CrMax[i] >= au16CrMin[i]
    xmedia_u16  crmin[XMEDIA_ISP_ISO_MAX_COUNT]; // RW; Range:[0x0, 0xFFF]
    xmedia_u16  cbmax[XMEDIA_ISP_ISO_MAX_COUNT]; // RW; Range:[0x0, 0xFFF]; au16CbMax[i] >= au16CbMin[i]
    xmedia_u16  cbmin[XMEDIA_ISP_ISO_MAX_COUNT]; // RW; Range:[0x0, 0xFFF]
} xmedia_awb_cbcr_track_attr;

#define XMEDIA_AWB_LUM_HIST_NUM 6

// Defines the parameters of the luminance histogram statistics for white balance
typedef struct {
    xmedia_bool enable; // RW; Range:[0, 1]; Format:1.0; If enabled, zone weight to awb is combined with zone luma
    xmedia_video_operation_mode op_type; // In auto mode, the weight distribution follows Gaussian distribution
    // RW; Range:[0x0, 0xFF]; Format:8.0; In manual mode, user define luma thresh, thresh[0] = 0, thresh[5] = 0xFF,
    // thresh[i] >=  thresh[i-1]
    xmedia_u8 hist_thresh[XMEDIA_AWB_LUM_HIST_NUM];
    // RW; Range:[0x0, 0xFFFF]; Format:16.0; user can define luma weight in both manual and auto mode.
    xmedia_u16 hist_wt[XMEDIA_AWB_LUM_HIST_NUM];
} xmedia_awb_luma_hist_attr;

typedef enum {
    XMEDIA_AWB_ALG_NORMAL = 0,
    XMEDIA_AWB_ALG_AI,
    XMEDIA_AWB_ALG_MAX
} xmedia_awb_alg;

typedef enum {
    XMEDIA_AWB_NORMAL_ALG_LOWCOST = 0,
    XMEDIA_AWB_NORMAL_ALG_ADVANCE,
    XMEDIA_AWB_NORMAL_ALG_MAX
} xmedia_awb_normal_alg_type;

#define XMEDIA_AWB_BAYER_CHN_NUM    4
#define XMEDIA_AWB_ZONE_ORIG_ROW    32
#define XMEDIA_AWB_ZONE_ORIG_COLUMN 32

typedef struct {
    xmedia_bool                enable;
    xmedia_u16                 ref_ct;
    xmedia_u16                 static_wb[XMEDIA_AWB_BAYER_CHN_NUM];
    xmedia_s32                 curve_para[XMEDIA_ISP_AWB_CURVE_PARA_NUM];
    xmedia_awb_normal_alg_type alg_type;
    xmedia_u8                  rg_str;
    xmedia_u8                  bg_str;
    xmedia_u16                 speed;
    xmedia_u16                 zone_sel;
    xmedia_u16                 high_ct;
    xmedia_u16                 low_ct;
    xmedia_awb_ct_limit_attr   ct_limit;
    xmedia_bool                shift_limit_en;
    xmedia_u8                  shift_limit;
    xmedia_bool                gain_norm_en;
    xmedia_bool                natural_cast_en;
    xmedia_awb_cbcr_track_attr cbcr_track;
    xmedia_awb_luma_hist_attr  luma_hist;
    xmedia_bool                awb_zone_wt_en;
    xmedia_u8                  zone_wt[XMEDIA_AWB_ZONE_ORIG_ROW * XMEDIA_AWB_ZONE_ORIG_COLUMN];
} xmedia_awb_auto_attr;

typedef struct {
    xmedia_u16 rgain;
    xmedia_u16 grgain;
    xmedia_u16 gbgain;
    xmedia_u16 bgain;
} xmedia_awb_manual_attr;

typedef struct {
    xmedia_bool                 bypass;          // RW; Range: [0, 1];  Format:1.0; If enabled, awb will be bypassed
    xmedia_u8                   awb_runinterval; // RW; Range: [0x1, 0xFF];  Format:8.0; set the AWB run interval
    xmedia_video_operation_mode op_type;
    xmedia_awb_alg              alg_type;
    xmedia_awb_manual_attr      manual_attr;
    xmedia_awb_auto_attr        auto_attr;
} xmedia_awb_attr;

typedef struct {
    xmedia_u16 raw_white_level;
    xmedia_u16 raw_black_level;
    xmedia_u16 raw_cr_max;
    xmedia_u16 raw_cr_min;
    xmedia_u16 raw_cb_max;
    xmedia_u16 raw_cb_min;

    xmedia_u16 wdr_mode;
    xmedia_u16 enable;
    xmedia_u16 manual_en;
    xmedia_u16 zone;
    xmedia_u16 high_temp;
    xmedia_u16 low_temp;
    xmedia_u16 ref_temp;
    xmedia_u16 rgain_base;
    xmedia_u16 ggain_base;
    xmedia_u16 bgain_base;
    xmedia_s32 p1;
    xmedia_s32 p2;
    xmedia_s32 q;
    xmedia_s32 a;
    xmedia_s32 c;

    xmedia_u16 man_sat_en;
    xmedia_u16 sat_target;

    xmedia_u16 hist_en;
    xmedia_u16 multi_en;
    xmedia_u16 outdoor_en;
} xmedia_awb_dbg_attr;

typedef struct {
    xmedia_u16 count_all;
    xmedia_u16 raw_r_avg;
    xmedia_u16 raw_g_avg;
    xmedia_u16 raw_b_avg;
    xmedia_u16 tk;
    xmedia_u16 weight;
    xmedia_s16 shift;
} xmedia_awb_zone_dbg;

typedef struct {
    xmedia_u32          frm_num_bgn;
    xmedia_u16          global_countall;
    xmedia_u16          global_countmin;
    xmedia_u16          global_countmax;
    xmedia_u16          global_ravg;
    xmedia_u16          global_gavg;
    xmedia_u16          global_bavg;
    xmedia_u16          tk;
    xmedia_u16          rgain;
    xmedia_u16          ggain;
    xmedia_u16          bgain;
    xmedia_u16          ccm[XMEDIA_ISP_CCM_MATRIX_SIZE];
    xmedia_awb_zone_dbg zone_debug[XMEDIA_AWB_ZONE_ORIG_ROW * XMEDIA_AWB_ZONE_ORIG_COLUMN];

    xmedia_u32 frm_num_end;
} xmedia_awb_dbg_status;

/*
 * 函数功能: 注册awb库
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_EXIST - awb重复注册
 *      XMEDIA_ERRCODE_NULL_PTR - 空指针
 */
xmedia_s32 xmedia_awb_register(xmedia_u32 pipe);

/*
 * 函数功能: 注销awb库
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 */
xmedia_s32 xmedia_awb_unregister(xmedia_u32 pipe);

/*
 * 函数功能: 设置awb属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      awb_attr - awb属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_set_attr(xmedia_u32 pipe, const xmedia_awb_attr *awb_attr);

/*
 * 函数功能: 获取awb属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      awb_attr - awb属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_get_attr(xmedia_u32 pipe, xmedia_awb_attr *awb_attr);

/*
 * 函数功能: 设置awb场景调整属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      awb_scene_adj_attr - 部分场景调整属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_set_scene_adj_attr(xmedia_u32 pipe, const xmedia_awb_scene_adj_attr *awb_scene_adj_attr);

/*
 * 函数功能: 获取awb场景调整属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      awb_scene_adj_attr - 部分场景调整属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_get_scene_adj_attr(xmedia_u32 pipe, xmedia_awb_scene_adj_attr *awb_scene_adj_attr);

/*
 * 函数功能: 设置ccm属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      ccm_attr - ccm属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_set_ccm_attr(xmedia_u32 pipe, const xmedia_awb_ccm_attr *ccm_attr);

/*
 * 函数功能: 获取ccm属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      ccm_attr - ccm属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_get_ccm_attr(xmedia_u32 pipe, xmedia_awb_ccm_attr *ccm_attr);

/*
 * 函数功能: 设置ccm 色调和饱和度调整属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      color_sector_attr - ccm 色调和饱和度调整属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_set_color_sector_attr(xmedia_u32 pipe, const xmedia_color_sector_attr *color_sector_attr);

/*
 * 函数功能: 获取ccm 色调和饱和度调整参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      color_sector_attr - ccm 色调和饱和度调整属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_get_color_sector_attr(xmedia_u32 pipe, xmedia_color_sector_attr *color_sector_attr);

/*
 * 函数功能: 设置饱和度属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      sat_attr - 饱和度属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_set_saturation_attr(xmedia_u32 pipe, const xmedia_awb_saturation_attr *sat_attr);

/*
 * 函数功能: 获取饱和度属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      sat_attr - 饱和度属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_get_saturation_attr(xmedia_u32 pipe, xmedia_awb_saturation_attr *sat_attr);

/*
 * 函数功能: 更新awb info
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      wb_info - awb info
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_query_info(xmedia_u32 pipe, xmedia_awb_info *wb_info);

/*
 * 函数功能: 设置color tone属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      ct_attr - color tone属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_set_color_tone_attr(xmedia_u32 pipe, const xmedia_awb_color_tone_attr *ct_attr);

/*
 * 函数功能: 获取color tone属性参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      ct_attr - color tone属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe参数无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入指针参数无效
 *      XMEDIA_ERRCODE_NULL_PTR - 获取vreg失败
 */
xmedia_s32 xmedia_awb_get_color_tone_attr(xmedia_u32 pipe, xmedia_awb_color_tone_attr *ct_attr);

/*
 * 函数功能: 根据色温值计算awbgain
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      awb_attr - awb属性
 *      color_temp - 色温
 *      shift - 偏移量
 *      awb_gain - awb gain
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 */
xmedia_s32 xmedia_awb_cal_gain_by_temp(xmedia_u32 pipe, const xmedia_awb_attr *awb_attr, xmedia_u16 color_temp,
                                       xmedia_s16 shift, xmedia_u16 *awb_gain);

/*
 * 函数功能: 设置awb库debug功能, 打印信息输出到内存
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      debug_info - 用户分配的debug信息输出buffer信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 */
xmedia_s32 xmedia_awb_set_debug_info(xmedia_u32 pipe, xmedia_isp_debug_info *debug_info);

/*
 * 函数功能: 设置awb库debug打印等级, 打印信息输出到串口
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      debug_level - AWB库debug打印等级 (1: ERROR  2: WARNING  3: NOTICE  4: INFO  5: DEBUG)
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入参数无效
 */
xmedia_s32 xmedia_awb_set_debug_level(xmedia_u32 pipe, xmedia_u32 debug_level);

#ifdef __cplusplus
}
#endif // End of #ifdef __cplusplus

#endif
