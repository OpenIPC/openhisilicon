/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef XMEDIA_SYS_H
#define XMEDIA_SYS_H

#include "xmedia_type.h"
#include "common.h"
#include "xmedia_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BIND_DEST_MAXNUM               64
#define UNIQUE_ID_NUM                  6
#define SWITCH_MAX_GRP_NUM             2
#define SWITCH_MAX_PIPE_NUM            2

typedef enum {
    XMEDIA_WORK_MODE_OFFLINE = 0,
    XMEDIA_WORK_MODE_ONLINE,
    XMEDIA_WORK_MODE_MAX,
} xmedia_work_mode;

typedef struct {
    xmedia_work_mode vicap_viproc_mode;
    xmedia_work_mode viproc_vpss_mode;
    xmedia_work_mode gdc_vpss_mode;
} xmedia_pipe_work_mode;

typedef struct {
    xmedia_pipe_work_mode pipe_mode[VI_MAX_PIPE_NUM];
} xmedia_sys_config;

typedef enum {
    XMEDIA_SYS_MULTI_PIPE_MODE_NORMAL = 0,
    XMEDIA_SYS_MULTI_PIPE_MODE_DUAL_SWITCH,
    XMEDIA_SYS_MULTI_PIPE_MODE_MAX
} xmedia_sys_multi_pipe_mode;

typedef enum {
    XMEDIA_SYS_SWITCH_SYNC_MODE_SOC_MASTER_SLAVE = 0,
    XMEDIA_SYS_SWITCH_SYNC_MODE_SOC_SLAVE,
    XMEDIA_SYS_SWITCH_SYNC_MODE_SENSOR_MASTER_SLAVE,
    XMEDIA_SYS_SWITCH_SYNC_MODE_MAX
} xmedia_sys_switch_sync_mode;

typedef struct {
    xmedia_sys_multi_pipe_mode multi_pipe_mode;
    xmedia_u32 pipe_num;
    xmedia_s32 master_pipe_id;
    xmedia_s32 pipe_id[SWITCH_MAX_PIPE_NUM];
    xmedia_bool pipe_sel[SWITCH_MAX_PIPE_NUM];
    xmedia_sys_switch_sync_mode sync_mode;
    xmedia_bool ext_switch_en;
} xmedia_sys_switch_config;

typedef struct {
    xmedia_mod_id mod_id;
    xmedia_s32 dev_id;
    xmedia_s32 chn_id;
} xmedia_chn_info;

typedef struct {
    xmedia_u32 num;
    xmedia_chn_info mpp_chn[BIND_DEST_MAXNUM];
} xmedia_sys_bind_dest;

typedef struct {
    xmedia_u32 id[UNIQUE_ID_NUM];
} xmedia_unique_id;

typedef enum {
    XMEDIA_SCALE_RANGE_0 = 0, /* scale range <   8/64 */
    XMEDIA_SCALE_RANGE_1, /* scale range >=  8/64 */
    XMEDIA_SCALE_RANGE_2, /* scale range >= 9/64 */
    XMEDIA_SCALE_RANGE_3, /* scale range >= 12/64 */
    XMEDIA_SCALE_RANGE_4, /* scale range >= 17/64 */
    XMEDIA_SCALE_RANGE_5, /* scale range >= 21/64 */
    XMEDIA_SCALE_RANGE_6, /* scale range >= 26/64 */
    XMEDIA_SCALE_RANGE_7, /* scale range >= 31/64 */
    XMEDIA_SCALE_RANGE_8, /* scale range >= 34/64 */
    XMEDIA_SCALE_RANGE_9, /* scale range >= 36/64 */
    XMEDIA_SCALE_RANGE_10, /* scale range >= 40/64 */
    XMEDIA_SCALE_RANGE_11, /* scale range >= 43/64 */
    XMEDIA_SCALE_RANGE_12, /* scale range >= 46/64 */
    XMEDIA_SCALE_RANGE_13, /* scale range >= 49/64 */
    XMEDIA_SCALE_RANGE_14, /* scale range >= 52/64 */
    XMEDIA_SCALE_RANGE_15, /* scale range >= 54/64 */
    XMEDIA_SCALE_RANGE_16, /* scale range >= 56/64 */
    XMEDIA_SCALE_RANGE_17, /* scale range >= 58/64 */
    XMEDIA_SCALE_RANGE_18, /* scale range >  1     */
    XMEDIA_SCALE_RANGE_MAX,
} xmedia_scale_range;

typedef struct {
    xmedia_scale_range horizontal;
    xmedia_scale_range vertical;
} xmedia_sys_scale_range;

typedef enum {
    XMEDIA_COEFF_LEVEL_0 = 0, /* coefficient level 0 */
    XMEDIA_COEFF_LEVEL_1, /* coefficient level 1 */
    XMEDIA_COEFF_LEVEL_2, /* coefficient level 2 */
    XMEDIA_COEFF_LEVEL_3, /* coefficient level 3 */
    XMEDIA_COEFF_LEVEL_4, /* coefficient level 4 */
    XMEDIA_COEFF_LEVEL_5, /* coefficient level 5 */
    XMEDIA_COEFF_LEVEL_6, /* coefficient level 6 */
    XMEDIA_COEFF_LEVEL_7, /* coefficient level 7 */
    XMEDIA_COEFF_LEVEL_8, /* coefficient level 8 */
    XMEDIA_COEFF_LEVEL_9, /* coefficient level 9 */
    XMEDIA_COEFF_LEVEL_10, /* coefficient level 10 */
    XMEDIA_COEFF_LEVEL_11, /* coefficient level 11 */
    XMEDIA_COEFF_LEVEL_12, /* coefficient level 12 */
    XMEDIA_COEFF_LEVEL_13, /* coefficient level 13 */
    XMEDIA_COEFF_LEVEL_14, /* coefficient level 14 */
    XMEDIA_COEFF_LEVEL_15, /* coefficient level 15 */
    XMEDIA_COEFF_LEVEL_16, /* coefficient level 16 */
    XMEDIA_COEFF_LEVEL_17, /* coefficient level 17 */
    XMEDIA_COEFF_LEVEL_18, /* coefficient level 18 */
    XMEDIA_COEFF_LEVEL_MAX,
} xmedia_coeff_level;

typedef struct {
    xmedia_coeff_level horizontal_lum; /* horizontal luminance   coefficient level */
    xmedia_coeff_level horizontal_chr; /* horizontal chrominance coefficient level */
    xmedia_coeff_level vertical_lum; /* vertical   luminance   coefficient level */
    xmedia_coeff_level vertical_chr; /* vertical   chrominance coefficient level */
} xmedia_scale_coef_level;

typedef struct {
    xmedia_char gps_latitude_ref; /* GPS LatitudeRef Indicates whether the latitude is north or south latitude,
                               * 'N'/'S', default 'N' */
    xmedia_u32 gps_latitude[3][2]; /* GPS Latitude is expressed as degrees, minutes and seconds, a typical format
                                * like "dd/1, mm/1, ss/1", default 0/0, 0/0, 0/0 */
    xmedia_char gps_longitude_ref; /* GPS LongitudeRef Indicates whether the longitude is east or west longitude,
                                * 'E'/'W', default 'E' */
    xmedia_u32 gps_longitude[3][2]; /* GPS Longitude is expressed as degrees, minutes and seconds, a typical format
                                 * like "dd/1, mm/1, ss/1", default 0/0, 0/0, 0/0 */
    xmedia_u8 gps_altitude_ref; /* GPS AltitudeRef Indicates the reference altitude used, 0 - above sea level,
                             * 1 - below sea level default 0 */
    xmedia_u32 gps_altitude[2]; /* GPS AltitudeRef Indicates the altitude based on the reference gps_altitude_ref,
                             * the reference unit is meters, default 0/0 */
} xmedia_gps_info;

typedef struct {
    xmedia_bool all_online;

    xmedia_u32 frame_rate; /* Input frame rate of VIPROC(linear/Frame mode WDR is the sensor frame rate,
                          * and the line mode WDR is the frame rate after synthesis) */
    xmedia_u32 full_lines_std; /* Total height of sensor, include of VBlank */

    xmedia_video_size large_stream_size; /* VENC Large Stream Size */
    xmedia_video_size small_stream_size; /* VENC Small Stream Size */

    xmedia_payload_type type; /* VENC encode type */

} xmedia_vpss_venc_wrap_param;

typedef enum {
    XMEDIA_SYS_COMPRESS_LEVEL_0 = 0,  //较低压缩率
    XMEDIA_SYS_COMPRESS_LEVEL_1,      //折中压缩率
    XMEDIA_SYS_COMPRESS_LEVEL_2,      //较高压缩率
    XMEDIA_SYS_COMPRESS_LEVEL_MAX
} xmedia_sys_compress_level;

/*
 *  函数功能：SYS模块初始化
 *  函数参数：
 *      sys_config - 输入参数，配置pipe工作模式
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_init(const xmedia_sys_config *sys_config);

/*
 *  函数功能：SYS模块去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_CLOSE_FAILED      - 关闭节点失败
 */
xmedia_s32 xmedia_sys_exit(xmedia_void);

/*
 *  函数功能：设置switch配置信息
 *  函数参数：
 *      grp_id - 输入参数，switch组号
 *      xmedia_sys_switch_config - 输入参数，switch配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_set_switch_config(xmedia_s32 grp_id, const xmedia_sys_switch_config *switch_config);

/*
 *  函数功能：获取switch配置信息
 *  函数参数：
 *      grp_id - 输入参数，switch组号
 *      xmedia_sys_switch_config - 输出参数，获取到的switch配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_switch_config(xmedia_s32 grp_id, xmedia_sys_switch_config *switch_config);

/*
 *  函数功能：数据源到数据接收者绑定
 *  函数参数：
 *      src_chn - 输入参数，数据源的描述信息
 *      dest_chn - 输入参数，数据接收者的描述信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请绑定相关内存失败
 */
xmedia_s32 xmedia_sys_bind(const xmedia_chn_info *src_chn, const xmedia_chn_info *dest_chn);

/*
 *  函数功能：数据源到数据接收者解除绑定
 *  函数参数：
 *      src_chn - 输入参数，数据源的描述信息
 *      dest_chn - 输入参数，数据接收者的描述信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_unbind(const xmedia_chn_info *src_chn, const xmedia_chn_info *dest_chn);

/*
 *  函数功能：获取目标通道上绑定的源通道的信息
 *  函数参数：
 *      dest_chn - 输入参数，数据接收者的描述信息
 *      src_chn - 输出参数，获取的对应数据源的描述信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_get_bind_by_dest(const xmedia_chn_info *dest_chn, xmedia_chn_info *src_chn);

/*
 *  函数功能：根据源获取绑定的目标
 *  函数参数：
 *      src_chn - 输入参数，数据源的描述信息
 *      bind_dest - 输出参数，获取的源对应所有接收者的描述信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_get_bind_by_src(const xmedia_chn_info *src_chn, xmedia_sys_bind_dest *bind_dest);

/*
 *  函数功能：获取当前芯片的 ID
 *  函数参数：
 *      chip_id - 输出参数，当前芯片的ID，来源OTP
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_chip_id(xmedia_u32 *chip_id);

/*
 *  函数功能：获取芯片唯一识别码
 *  函数参数：
 *      unique_id - 输出参数，芯片唯一识别码
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_unique_id(xmedia_unique_id *unique_id);

/*
 *  函数功能：获取当前芯片的客户识别码
 *  函数参数：
 *      custom_code - 输出参数，当前芯片的客户识别码
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_custom_code(xmedia_u32 *custom_code);

/*
 *  函数功能：获取系统的当前时间戳
 *  函数参数：
 *      cur_pts - 输出参数，系统的当前时间戳
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_cur_pts(xmedia_u64 *cur_pts);

/*
 *  函数功能：初始化系统的时间戳基准
 *  函数参数：
 *      pts_base - 输入参数，设置的时间戳基准
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_init_pts_base(xmedia_u64 pts_base);

/*
 *  函数功能：同步系统的时间戳
 *  函数参数：
 *      pts_base - 输入参数，设置的时间戳基准
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_sync_pts(xmedia_u64 pts_base);

/*
 *  函数功能：配置内存参数，设置模块设备通道使用的 MMZ 区域名称
 *  函数参数：
 *      chn_info - 输入参数，设备通道描述信息
 *      mmz_name - 输入参数，MMZ区域名称
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_set_mem_config(const xmedia_chn_info *chn_info, const xmedia_char *mmz_name);

/*
 *  函数功能：获取模块设备通道使用的 MMZ 区域名称
 *  函数参数：
 *      chn_info - 输入参数，设备通道描述信息
 *      mmz_name - 输出参数，返回的对应MMZ区域名称
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_get_mem_config(const xmedia_chn_info *chn_info, xmedia_char *mmz_name);

/*
 *  函数功能：设置 VPSS 模块和 VGS 模块的缩放系数等级
 *  函数参数：
 *      scale_range - 输入参数，缩放区域信息
 *      scale_coef_level - 输入参数，缩放等级信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_set_scale_coef_level(const xmedia_sys_scale_range *scale_range,
        const xmedia_scale_coef_level *scale_coef_level);

/*
 *  函数功能：获取 VPSS 和 VGS 缩放系数等级
 *  函数参数：
 *      scale_range - 输入参数，缩放区域信息
 *      scale_coef_level - 输出参数，缩放等级信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_get_scale_coef_level(const xmedia_sys_scale_range *scale_range,
    xmedia_scale_coef_level *scale_coef_level);

/*
 *  函数功能：设置时区信息
 *  函数参数：
 *      timezone - 输入参数，时区信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_set_timezone(xmedia_s32 timezone);

/*
 *  函数功能：获取时区信息
 *  函数参数：
 *      timezone - 输出参数，时区信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_timezone(xmedia_s32 *timezone);

/*
 *  函数功能：设置 GPS 信息
 *  函数参数：
 *      gps_info - 输入参数，GPS信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_sys_set_gps_info(const xmedia_gps_info *gps_info);

/*
 *  函数功能：获取 GPS 信息
 *  函数参数：
 *      gps_info - 输出参数，GPS信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_gps_info(xmedia_gps_info *gps_info);

/*
 *  函数功能：设置 Tuning 工具连接
 *  函数参数：
 *      connect - 输入参数，Tuning 工具连接控制状态
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_set_tuning_connect(xmedia_s32 connect);

/*
 *  函数功能：获取 Tuning 工具连接状态
 *  函数参数：
 *      connect - 输出参数，Tuning 工具连接控制状态
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_sys_get_tuning_connect(xmedia_s32 *connect);

/*
 *  函数功能：获取VPSS-VENC卷绕模式下的buffer像素行数
 *  函数参数：
 *      wrap_param - 输入参数，计算卷绕buffer像素行数的参数
 *      buf_line - 输出参数，卷绕buffer的像素行数
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - 不支持
 */
xmedia_s32 xmedia_sys_get_vpss_venc_wrap_buf_line(const xmedia_vpss_venc_wrap_param *wrap_param, xmedia_u32 *buf_line);

/*
 *  函数功能：设置系统压缩率等级
 *  函数参数：
 *      compress_level - 输入参数，系统压缩率等级
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - 函数调用时机错误，必须在确定vb block大小前调用
 *  注意：
 *      1、必须在确定vb block大小前调用，不调用则使用默认等级
 *      2、默认系统压缩率等级为XMEDIA_SYS_COMPRESS_LEVEL_0
 */
xmedia_s32 xmedia_sys_set_compress_level(xmedia_sys_compress_level compress_level);

/*
 *  函数功能：获取系统压缩率等级
 *  函数参数：
 *      compress_level - 输出参数，系统压缩率等级
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 */
xmedia_s32 xmedia_sys_get_compress_level(xmedia_sys_compress_level *compress_level);

#ifdef __cplusplus
}
#endif

#endif

