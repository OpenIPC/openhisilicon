/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AF_H__
#define __XMEDIA_AF_H__

#include "xmedia_type.h"
#include "xmedia_video_common.h"
#include "xmedia_isp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_AF_SPC_GAIN_MAP_X_DIR_NUM 8
#define XMEDIA_AF_SPC_GAIN_MAP_Y_DIR_NUM 6
#define XMEDIA_AF_DCC_MAP_X_DIR_NUM      8
#define XMEDIA_AF_DCC_MAP_Y_DIR_NUM      6

typedef struct {
    xmedia_bool vertical_binning_en;
    xmedia_u32  vertical_binning_times; // [0, 32]
    xmedia_bool xcorr_smooth_en;
    xmedia_s32  xcorr_calc_upper_thd; // [-32, 32]
    xmedia_s32  xcorr_calc_lower_thd; // [-32, 32]
    xmedia_u32  luma_thd;             // [0, 128]
    xmedia_s32  phase_disparity_thd;  // [0, 32]
    xmedia_s32  dcc_slope_adj_coeff;  // [-1024, 1024]
    xmedia_s32  confidence_thd_low;   // [-1024, 1024]
    xmedia_s32  confidence_thd_high;  // [-1024, 1024]

    xmedia_bool spc_en;
    xmedia_u32  left_spc_gain[XMEDIA_AF_SPC_GAIN_MAP_Y_DIR_NUM][XMEDIA_AF_SPC_GAIN_MAP_X_DIR_NUM];  // [0, 1024]
    xmedia_u32  right_spc_gain[XMEDIA_AF_SPC_GAIN_MAP_Y_DIR_NUM][XMEDIA_AF_SPC_GAIN_MAP_X_DIR_NUM]; // [0, 1024]
    xmedia_s32  dcc_slope[XMEDIA_AF_DCC_MAP_Y_DIR_NUM][XMEDIA_AF_DCC_MAP_X_DIR_NUM];                // [-1024, 1024]
    xmedia_s32  dcc_offset[XMEDIA_AF_DCC_MAP_Y_DIR_NUM][XMEDIA_AF_DCC_MAP_X_DIR_NUM];               // [-1024, 1024]
} xmedia_af_pdaf_attr;

typedef struct {
} xmedia_af_cdaf_attr;

typedef enum {
    XMEDIA_AF_TYPE_CDAF,
    XMEDIA_AF_TYPE_PDAF,
    XMEDIA_AF_TYPE_HYBRID,
    XMEDIA_AF_TYPE_MAX,
} xmedia_af_type;

typedef struct {
    xmedia_bool         enable;
    xmedia_af_type      type;
    xmedia_video_rect   roi;
    xmedia_u8           run_interval;
    xmedia_af_pdaf_attr pdaf_attr;
    xmedia_af_cdaf_attr cdaf_attr;
} xmedia_af_attr;

typedef struct {
    xmedia_u64        frame_cnt;
    xmedia_video_rect roi; // 实际生效的roi区域

    xmedia_u32 position;         // 镜头绝对位置
    xmedia_s32 defocus;          // 镜头移动量 unit: dac
    xmedia_s32 confidence_level; // -1: CDAF only, 0: PDAF and CDAF, 1: PDAF only
    xmedia_s32 phase_disparity;  // unit: pixel
} xmedia_af_info;

/*
 * 函数功能: 注册af库到ISP
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_EXIST - af库已注册
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不允许的操作
 */
xmedia_s32 xmedia_af_register(xmedia_u32 pipe);

/*
 * 函数功能: 反注册af库
 * 函数参数：
 *      pipe - 输入参数，pipe号
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_NOT_EXIST - 未注册af库
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 不允许的操作
 */
xmedia_s32 xmedia_af_unregister(xmedia_u32 pipe);

/*
 * 函数功能: 设置af参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      af_attr - 输入参数，af属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_af_set_attr(xmedia_u32 pipe, const xmedia_af_attr *af_attr);

/*
 * 函数功能: 获取af参数
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      af_attr - 输出参数，af属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_af_get_attr(xmedia_u32 pipe, xmedia_af_attr *af_attr);

/*
 * 函数功能: 查询af信息
 * 函数参数：
 *      pipe - 输入参数，pipe号
 *      af_info - 输出参数，af信息
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 xmedia_af_query_info(xmedia_u32 pipe, xmedia_af_info *af_info);

#ifdef __cplusplus
}
#endif

#endif // __XMEDIA_API_AF_H__
