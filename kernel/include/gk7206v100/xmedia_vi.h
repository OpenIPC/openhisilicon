/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_VI_H__
#define __XMEDIA_VI_H__

#include "xmedia_errcode.h"
#include "xmedia_intf_common.h"
#include "xmedia_video_common.h"
#include "xmedia_sys.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define VI_INVALID_FRAME_RATE         (-1)
#define VI_MAX_PIN_NUM                12

#define VI_DIS_MAX_BUF_NUM            10
#define VI_OFFLINE_DIS_MIN_BUF_NUM    3
#define VI_ONLINE_DIS_MIN_BUF_NUM     4

#define VI_DIS_MAX_CROP_RATIO         98
#define VI_DIS_MIN_CROP_RATIO         50
#define VI_DIS_MAX_MOVING_SUB_LEVEL   6
#define VI_DIS_MIN_MOVING_SUB_LEVEL   0
#define VI_DIS_MAX_ROLLING_SHUTTER    2048
#define VI_DIS_MIN_ROLLING_SHUTTER    0
#define VI_DIS_MAX_SHIFT_LIMIT        2048
#define VI_DIS_MIN_SHIFT_LIMIT        0
#define VI_DIS_MAX_MOTION_COEF        100
#define VI_DIS_MIN_MOTION_COEF        0

#define XMEDIA_VI_FISHEYE_MAX_RGN_NUM 4

#define XMEDIA_VI_MAX_SELECT_CHN_NUM  (VI_MAX_PIPE_NUM * VI_MAX_CHN_NUM)

#define VI_MIPI_DATA_LANE_MAX_NUM     4

/*
 * 输入接口与 VI 引脚的连接关系
 * 引脚连接顺序一一对应：无需配置
 * 引脚连接顺序错误：
 *     1、enable = true
 *     2、连接顺序错误的引脚：vi_pin[n] = m，vi引脚n接收DC/BT输出的第m bit数据
 *     3、连接顺序正确的引脚：配置默认值，vi_pin[n] = n
 */
typedef struct {
    xmedia_bool enable;
    xmedia_s16 vi_pin[VI_MAX_PIN_NUM];
} xmedia_vi_data_connection;

// BT.601、DC时序输入数据同步信号的极性
typedef enum {
    XMEDIA_VI_SYNC_PULSE_POSITIVE = 0, // 同步脉冲为正脉冲
    XMEDIA_VI_SYNC_PULSE_NEGATIVE, // 同步脉冲为负脉冲
    XMEDIA_VI_VSYNC_NEG_MAX
} xmedia_vi_sync_polarity;

// DC属性
typedef struct {
    xmedia_intf_dc_data_type data_type;
    xmedia_vi_data_connection data_connect;
    xmedia_vi_sync_polarity vsync;
    xmedia_vi_sync_polarity hsync;
} xmedia_vi_dc_attr;

typedef enum {
    XMEDIA_VI_MIPI_LANE_DIVIDE_MODE_1_LANE = 0,
    XMEDIA_VI_MIPI_LANE_DIVIDE_MODE_2_LANE,
    XMEDIA_VI_MIPI_LANE_DIVIDE_MODE_4_LANE,
    XMEDIA_VI_MIPI_LANE_DIVIDE_MODE_MAX
} xmedia_vi_mipi_lane_divide_mode;

typedef struct {
    xmedia_bool enable;
    xmedia_u32 lane_cfg[VI_MIPI_DATA_LANE_MAX_NUM];
} xmedia_vi_mipi_lane_config;

typedef struct {
    xmedia_bool enable;
    xmedia_bool data_pn_swap[VI_MIPI_DATA_LANE_MAX_NUM];
    xmedia_bool clk_pn_swap;
} xmedia_vi_mipi_pn_config;

typedef struct {
    xmedia_vi_mipi_lane_config lane_config;
    xmedia_vi_mipi_pn_config pn_config;
} xmedia_vi_mipi_data_connection;

// mipi属性
typedef struct {
    xmedia_intf_mipi_csi_data_type input_data_type; // 输入图像的数据格式
    xmedia_vi_mipi_lane_divide_mode lane_mode;
    xmedia_video_wdr_format wdr_format; // WDR 模式
    xmedia_vi_mipi_data_connection mipi_connection;
} xmedia_vi_mipi_attr;

// 复合工作模式
typedef enum {
    XMEDIA_VI_MULTIPLEX_MODE_1 = 0,
    XMEDIA_VI_MULTIPLEX_MODE_2,
    XMEDIA_VI_MULTIPLEX_MODE_MAX
} xmedia_vi_multiplex_mode;

// VI设备接收的时钟类型
typedef enum {
    XMEDIA_VI_CLK_EDGE_SINGLE = 0, // 时钟单沿模式
    XMEDIA_VI_CLK_EDGE_DOUBLE, // 时钟双沿模式
    XMEDIA_VI_CLK_EDGE_MAX
} xmedia_vi_clk_edge;

// 输入时序的水平消隐信息
typedef struct {
    xmedia_u32 hsync_hfb; // 水平前消隐区宽度
    xmedia_u32 hsync_act; // 水平有效区宽度
} xmedia_vi_hsync_timing_blank;

// VI设备接收BT.601时序的同步信息
typedef struct {
    xmedia_vi_sync_polarity vsync;
    xmedia_vi_sync_polarity hsync;
    xmedia_vi_hsync_timing_blank timing_blank;
} xmedia_vi_sync_cfg;

// bt.601属性
typedef struct {
    xmedia_intf_bt601_bt656_data_seq data_seq;
    xmedia_vi_sync_cfg sync_cfg;
    xmedia_vi_data_connection data_connect;
} xmedia_vi_bt601_attr;

// bt.656属性
typedef struct {
    xmedia_vi_clk_edge clk_egde;
    xmedia_intf_bt601_bt656_data_seq data_seq;
    xmedia_vi_multiplex_mode multiplex_mode;
    xmedia_vi_data_connection data_connect;
} xmedia_vi_bt656_attr;

// bt.1120属性
typedef struct {
    xmedia_intf_bt1120_data_seq data_seq;
    xmedia_vi_multiplex_mode multiplex_mode;
    xmedia_vi_data_connection data_connect;
} xmedia_vi_bt1120_attr;

typedef enum {
    XMEDIA_VI_SPI_MODE_SPRD,
    XMEDIA_VI_SPI_MODE_MTK,
    XMEDIA_VI_SPI_MODE_MAX
} xmedia_vi_spi_mode;

typedef enum {
    XMEDIA_VI_SPI_CPOL_MODE_LOW, // 低电平代表空闲
    XMEDIA_VI_SPI_CPOL_MODE_HIGH, // 高电平代表空闲
    XMEDIA_VI_SPI_CPOL_MODE_MAX
} xmedia_vi_spi_cpol_mode;

typedef enum {
    XMEDIA_VI_SPI_CPHA_MODE_FIRST, // 数据采样在时钟的第一个边沿
    XMEDIA_VI_SPI_CPHA_MODE_SECOND, // 数据采样在时钟的第二个边沿
    XMEDIA_VI_SPI_CPHA_MODE_MAX
} xmedia_vi_spi_cpha_mode;

typedef enum {
    XMEDIA_VI_SPI_DATA_TYPE_RAW_8BIT,
    XMEDIA_VI_SPI_DATA_TYPE_YUV422,
    XMEDIA_VI_SPI_DATA_TYPE_MAX
} xmedia_vi_spi_data_type;

typedef enum {
    XMEDIA_VI_SPI_DATA_WIRE_MODE_1_WIRE,
    XMEDIA_VI_SPI_DATA_WIRE_MODE_2_WIRE,
    XMEDIA_VI_SPI_DATA_WIRE_MODE_4_WIRE,
    XMEDIA_VI_SPI_DATA_WIRE_MODE_MAX
} xmedia_vi_spi_data_wire_mode;

typedef enum {
    XMEDIA_VI_SPI_DATA_ENDIAN_MSB,
    XMEDIA_VI_SPI_DATA_ENDIAN_LSB,
    XMEDIA_VI_SPI_DATA_ENDIAN_MAX
} xmedia_vi_spi_data_endian;

typedef struct {
    xmedia_vi_spi_mode spi_mode;
    xmedia_vi_spi_cpol_mode cpol_mode; // 时钟极性
    xmedia_vi_spi_cpha_mode cpha_mode; // 时钟相位
    xmedia_vi_spi_data_type data_type;
    xmedia_intf_yuv_data_seq data_seq;
    xmedia_vi_spi_data_wire_mode data_wire_mode;
    xmedia_vi_spi_data_endian data_endian;
    xmedia_vi_data_connection data_connect;
} xmedia_vi_spi_attr;

// VI设备配置信息
typedef struct {
    xmedia_intf_type intf_type;
    union {
        xmedia_vi_dc_attr dc_attr;
        xmedia_vi_mipi_attr mipi_attr;
        xmedia_vi_bt601_attr bt601_attr;
        xmedia_vi_bt656_attr bt656_attr;
        xmedia_vi_bt1120_attr bt1120_attr;
        xmedia_vi_spi_attr spi_attr;
    };
} xmedia_vi_dev_config;

// mipi phy配置
typedef struct {
    xmedia_u32 mipi_rate; // 单位：Mbps
} xmedia_vi_dev_mipi_phy_config;

// 绑定dev的物理pipe序号
typedef struct {
    xmedia_u32 pipe_num;
    xmedia_s32 pipe_id[VI_MAX_PHY_PIPE_NUM];
} xmedia_vi_dev_bind_pipe_attr;

typedef enum {
    XMEDIA_VI_PIPE_BYPASS_NONE = 0,
    XMEDIA_VI_PIPE_BYPASS_BE,
    XMEDIA_VI_PIPE_BYPASS_MAX
} xmedia_vi_pipe_bypass_mode;

typedef struct {
    xmedia_bool bnr_en; // 3DNR使能开关,使能后为3dnr分配所需内存
    xmedia_video_compress_mode compress_mode; // 参考帧压缩模式
} xmedia_vi_bnr_attr;

typedef struct {
    xmedia_bool stnr_en; // yuv域nr,使能决定是否分配参考帧内存
    xmedia_video_compress_mode compress_mode; // 参考帧压缩模式
} xmedia_vi_stnr_attr;

typedef struct {
    xmedia_vi_pipe_bypass_mode pipe_bypass_mode;
    xmedia_bool isp_bypass;
    xmedia_bool rgbir_en;
    xmedia_bool gdc_en;
    xmedia_vi_bnr_attr bnr_attr;
    xmedia_vi_stnr_attr stnr_attr;
    xmedia_video_size in_size;
    xmedia_video_pixel_format pix_fmt;
    xmedia_video_compress_mode compress_mode;
    xmedia_video_data_width bit_width;
    xmedia_video_frame_rate_ctrl frm_rate_ctrl;
} xmedia_vi_pipe_config;

typedef enum {
    XMEDIA_VI_GRP_TYPE_WDR = 0,
    XMEDIA_VI_GRP_TYPE_STITCH,
    XMEDIA_VI_GRP_TYPE_MAX
} xmedia_vi_grp_type;

typedef struct {
    xmedia_video_wdr_mode wdr_mode;
    xmedia_u32 cache_line;
    xmedia_s32 pipe_id[VI_MAX_WDR_NUM];
} xmedia_vi_wdr_grp_config;

typedef struct {
    xmedia_video_point offset;
    xmedia_s64 pmf_coef[XMEDIA_VIDEO_PMF_COEF_NUM];
    xmedia_video_size out_size; // GDC输出图像的大小
} xmedia_vi_stitch_correction_attr;

typedef struct {
    xmedia_video_stitch_mode mode;
    xmedia_vi_stitch_correction_attr stitch_correction_attr[VI_MAX_STITCH_NUM];
} xmedia_vi_stitch_attr;

typedef struct {
    xmedia_u32 pipe_num;
    xmedia_s32 pipe_id[VI_MAX_STITCH_NUM];
    xmedia_vi_stitch_attr vi_stitch_attr;
} xmedia_vi_stitch_grp_config;

// VI合成组配置
typedef struct {
    xmedia_vi_grp_type grp_type;
    union {
        xmedia_vi_wdr_grp_config wdr_grp_config;
        xmedia_vi_stitch_grp_config stitch_grp_config;
    };
} xmedia_vi_grp_config;

typedef struct {
    xmedia_bool enable;
    xmedia_u32 depth; // dump图像帧的队列深度,范围[0, 8]
} xmedia_vi_frame_dump_attr;

typedef enum {
    XMEDIA_VI_PIPE_FRAME_SOURCE_DEV = 0,
    XMEDIA_VI_PIPE_FRAME_SOURCE_USER,
    XMEDIA_VI_PIPE_FRAME_SOURCE_MAX
} xmedia_vi_pipe_frame_source;

typedef struct {
    xmedia_s32 milli_sec;
    xmedia_video_wdr_mode wdr_mode;
    xmedia_video_frame_info frame_info[VI_MAX_WDR_FRAME_NUM];
} xmedia_vi_send_frame_info;

typedef struct {
    xmedia_bool enable;
    xmedia_u32 frame_rate;
    xmedia_u32 interrupt_cnt; // 中断次数
    xmedia_u32 lost_frame_cnt; // 丢帧次数
    xmedia_u32 vb_fail_cnt; // 申请vb失败次数
    xmedia_video_size out_size; // 输出图像大小
} xmedia_vi_pipe_status;

// todo 暂时定义解决编译问题
typedef struct {
    xmedia_bool enable;
    xmedia_u32 line_cnt; // 低延时行号
    xmedia_bool one_buf_en; // 是否使用one buffer
} xmedia_vi_lowdelay_attr;

typedef struct {
    xmedia_video_size out_size;
    xmedia_video_pixel_format pix_fmt;
    xmedia_video_dynamic_range dynamic_range;
    xmedia_video_format video_fmt;
    xmedia_video_compress_mode compress_mode;
    xmedia_bool mirror_en; // 开启gdc功能后，不支持动态更改mirror、flip
    xmedia_bool flip_en;
    xmedia_u32 depth;
    xmedia_video_frame_rate_ctrl frm_rate_ctrl;
} xmedia_vi_chn_config;

typedef struct {
    xmedia_s32 bind_chn;
    xmedia_video_size out_size;
    xmedia_u32 depth;
    xmedia_video_frame_rate_ctrl frm_rate_ctrl;
} xmedia_vi_ext_chn_config;

typedef enum {
    XMEDIA_VI_DIS_MODE_4_DOF_GME = 0, // 四轴防抖
    XMEDIA_VI_DIS_MODE_6_DOF_GME, // 六轴防抖
    XMEDIA_VI_DIS_MODE_MAX,
} xmedia_vi_dis_mode;

typedef enum {
    XMEDIA_VI_DIS_MOTION_LEVEL_LOW = 0, // 低级别运动，镜头小幅度运动
    XMEDIA_VI_DIS_MOTION_LEVEL_NORMAL, // 正常级别运动，镜头正常幅度运动
    XMEDIA_VI_DIS_MOTION_LEVEL_HIGH, // 高级别运动，镜头大幅度运动
    XMEDIA_VI_DIS_MOTION_LEVEL_MAX
} xmedia_vi_dis_motion_level;

// dis配置信息
typedef struct {
    xmedia_vi_dis_mode mode; // 防抖算法
    xmedia_vi_dis_motion_level motion_level; // camera的运动级别
    /*
     *DIS 用于缓存图像的 buf 数目，在 DIS 输出帧率偶尔出现丢帧时，可以增加缓存 buf
     *数。vi离线取值范围[3,10], vi在线取值范围[4,10]。
     */
    xmedia_u32 buf_num;
    xmedia_u32 crop_ratio; // 输出图像的裁剪比例
    /*
     * scale: Crop 后的输出图像是否进行放大。当前 DIS 提供 Crop 后的输出图像是否进行放大操作选择。如果用户不想采用在
     * DIS 放大功能，可以选择在后端的 VPSS 进行放大操作。scale 为
     * false的情况下输出图像宽高等于输入宽高乘以crop_ratio，输出宽高是 2 对齐的。
     */
    xmedia_bool scale;
} xmedia_vi_dis_config;

// dis属性
typedef struct {
    xmedia_bool enable;
    xmedia_u32 hor_limit; // 水平偏移限制
    xmedia_u32 ver_limit; // 垂直偏移限制
    xmedia_bool still_crop; // 关闭 DIS 防抖效果，但图像依旧保持裁剪比例输出
    xmedia_bool smooth_en; // 平滑过渡使能
    xmedia_u32 motion_stable_coef; // 防抖强度衰减系数 取值范围[0, 100]
} xmedia_vi_dis_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_s64 pmf_coef[XMEDIA_VIDEO_PMF_COEF_NUM];
    xmedia_video_size out_size;
} xmedia_vi_fov_correction_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_u32 spread_coef;
} xmedia_vi_spread_attr;

typedef enum {
    XMEDIA_VI_FISHEYE_MOUNT_MODE_DESKTOP = 0, // 地装
    XMEDIA_VI_FISHEYE_MOUNT_MODE_CEILING, // 顶装
    XMEDIA_VI_FISHEYE_MOUNT_MODE_WALL, // 壁装
    XMEDIA_VI_FISHEYE_MOUNT_MODE_MAX
} xmedia_vi_fisheye_mount_mode;

typedef enum {
    XMEDIA_VI_FISHEYE_VIEW_MODE_NORM = 0, // normal校正模式
    XMEDIA_VI_FISHEYE_VIEW_MODE_NO_TRANS, // 不做校正，输出原图
    XMEDIA_VI_FISHEYE_VIEW_MODE_MAX
} xmedia_vi_fisheye_view_mode;

typedef enum {
    XMEDIA_VI_FISHEYE_RGN_MODE_PAN_TILT = 0, // pan_tilt模式
    XMEDIA_VI_FISHEYE_RGN_MODE_X_Y, // xy模式
    XMEDIA_VI_FISHEYE_RGN_MODE_MAX
} xmedia_vi_fisheye_rgn_mode;

typedef struct {
    xmedia_u32 x; // 该校正区域中心点的水平坐标
    xmedia_u32 y; // 该校正区域中心点的垂直坐标
} xmedia_vi_fisheye_xy_attr;

typedef struct {
    xmedia_u32 pan; // 该校正区域PTZ参数的pan值,取值范围[90, 270]
    xmedia_u32 tilt; // 该校正区域PTZ参数的tilt值,取值范围[90, 270]
} xmedia_vi_fisheye_pan_tilt_attr;

typedef struct {
    xmedia_vi_fisheye_view_mode view_mode;
    xmedia_u32 out_radius; // 校正区域的可视半径,取值范围[100, 2048]
    union {
        xmedia_vi_fisheye_xy_attr xy_attr;
        xmedia_vi_fisheye_pan_tilt_attr pan_tilt_attr;
    };
    xmedia_u32 hor_zoom; // 该校正区域PTZ参数的水平Zoom值，取值范围[1, 4095]
    xmedia_video_rect out_rect; // 该校正区域的输出区域, 不能超出整个鱼眼矫正输出图的范围, 且宽高需满足鱼眼输出图像规格限制
} xmedia_vi_fisheye_rgn_attr;

typedef struct {
    xmedia_bool bg_color_en; // 是否在输出图像打上背景色
    xmedia_u32 bg_color; // 背景色的颜色。RGB888格式，取值范围[0,0xFFFFFF]
    xmedia_s32 hor_offset; // 镜头中心点相对于sensor中心点的水平偏移，取值范围[-511, 511], 单位像素
    xmedia_s32 ver_offset; // 镜头中心点相对于sensor中心点的垂直偏移，取值范围[-511, 511], 单位像素
    xmedia_u32 trapezoid_coef; // 梯形校正强度系数。用于壁装时的俯仰角校正，取值范围[0,32]
    xmedia_vi_fisheye_mount_mode mount_mode;
    xmedia_vi_fisheye_rgn_mode rgn_mode;
    xmedia_bool black_check_en; // 是否开启黑边检测
    xmedia_u32 rgn_num; // 一副图像的校正区域数目，取值范围[1, 4]
    xmedia_vi_fisheye_rgn_attr fisheye_rgn_attr[XMEDIA_VI_FISHEYE_MAX_RGN_NUM]; // 每个校正区域各自的属性配置
} xmedia_vi_fisheye_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_vi_fisheye_attr fisheye_attr;
    xmedia_video_size out_size; // 鱼眼矫正后输出图像的大小
} xmedia_vi_fisheye_correction_attr;

/*
 *  函数功能：VI初始化
 *  函数参数：无
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_READY                     - sys未初始化
 */
xmedia_s32 xmedia_vi_init(xmedia_void);

/*
 *  函数功能：VI退出
 *  函数参数：无
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 */
xmedia_s32 xmedia_vi_exit(xmedia_void);

/*
 *  函数功能：设置VI设备配置信息
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *      dev_config                                   - 输入参数，VI设备配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_DISABLE                   - 设备未禁用
 *  注意：
 *      调用前需要保证VI设备处于禁用状态
 */
xmedia_s32 xmedia_vi_set_dev_config(xmedia_s32 dev, const xmedia_vi_dev_config *dev_config);

/*
 *  函数功能：获取VI设备配置信息
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *      dev_config                                   - 输出参数，VI设备配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_CONFIG                    - 设备配置信息未配置
 *  注意：
 *      需要先设置VI设备配置信息
 */
xmedia_s32 xmedia_vi_get_dev_config(xmedia_s32 dev, xmedia_vi_dev_config *dev_config);

/*
 *  函数功能：使能vi设备快启
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_NOT_DISABLE                   - 设备未禁用
 *  注意：
 *      调用前需要保证VI设备处于禁用状态
 */
xmedia_s32 xmedia_vi_enable_dev_quick_start(xmedia_s32 dev);

/*
 *  函数功能：设置VI MIPI PHY手动配置信息
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *      mipi_phy_config                              - 输入参数，VI设备mipi phy配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_DISABLE                   - 设备未禁用
 *  注意：
 *      调用前需要保证VI设备处于禁用状态
 */
xmedia_s32 xmedia_vi_set_dev_mipi_phy_config(xmedia_s32 dev,
                                                      const xmedia_vi_dev_mipi_phy_config *mipi_phy_config);

/*
 *  函数功能：启用VI设备
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_NOT_CONFIG                    - 设备配置信息未配置
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 使能的设备配置与已使能设备冲突
 *  注意：
 *      启用前必须已经设置设备配置信息
 */
xmedia_s32 xmedia_vi_enable_dev(xmedia_s32 dev);

/*
 *  函数功能：禁用VI设备
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_EXIST                         - 绑定的pipe未销毁
 *  注意：
 *      需先销毁所有与该 VI 设备绑定的物理 PIPE 后，再禁用 VI 设备
 */
xmedia_s32 xmedia_vi_disable_dev(xmedia_s32 dev);

/*
 *  函数功能：绑定DEV和物理PIPE
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *      pipe                                         - 输入参数，设备绑定的pipe
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_ENABLE                    - 设备或通道未启用
 *      XMEDIA_ERRCODE_BINDED                        - pipe已绑定
 *      XMEDIA_ERRCODE_EXIST                         - pipe已创建
 *  注意：
 *      1、必须先使能 VI 设备后才能绑定物理 PIPE
 *      2、不支持动态绑定
 *      3、不支持绑定已经创建的 PIPE
 */
xmedia_s32 xmedia_vi_set_dev_bind_pipe(xmedia_s32 dev, xmedia_s32 pipe);

/*
 *  函数功能：解绑DEV和物理PIPE
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *      pipe                                         - 输入参数，设备解绑的pipe
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_BIND                      - pipe未绑定
 *      XMEDIA_ERRCODE_EXIST                         - pipe未销毁
 *  注意：
 *      必须先销毁物理 PIPE 再解绑定
 */
xmedia_s32 xmedia_vi_set_dev_unbind_pipe(xmedia_s32 dev, xmedia_s32 pipe);

/*
 *  函数功能：获取与DEV绑定的PIPE
 *  函数参数：
 *      dev                                          - 输入参数，VI设备号
 *      bind_pipe_attr                               - 输出参数，绑定dev的物理pipe序号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_DEV_ID                - 无效设备号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_ENABLE                    - 设备或通道未启用
 *      XMEDIA_ERRCODE_NOT_BIND                      - pipe未绑定
 *  注意：
        必须先绑定PIPE
 */
xmedia_s32 xmedia_vi_get_pipe_by_dev(xmedia_s32 dev, xmedia_vi_dev_bind_pipe_attr *bind_pipe_attr);

/*
 *  函数功能：设置VI合成组的配置信息
 *  函数参数：
 *      grp                                          - 输入参数，合成组的组号
 *      grp_config                                   - 输入参数，合成组的配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_GRP_ID                - 无效组号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - grp配置不支持
 *      XMEDIA_ERRCODE_EXIST                         - pipe已存在
 *      XMEDIA_ERRCODE_NOT_BIND                      - pipe未绑定
 *  注意：
 *      1、需要在创建pipe前调用
 *      2、设置wdr合成组，同一组中的pipe id不能重复
 *      3、设置wdr合成组，同一组中的pipe id必须是与同一个dev绑定
 *      4、设置mcf合成组，同一组中的pipe id必须是与不同的dev绑定
 *      5、设置stitch合成组，同一组中的pipe id必须是与不同的dev绑定
 */
xmedia_s32 xmedia_vi_set_grp_config(xmedia_s32 grp, const xmedia_vi_grp_config *grp_config);

/*
 *  函数功能：获取VI合成组的配置信息
 *  函数参数：
 *      grp                                          - 输入参数，合成组的组号
 *      grp_config                                   - 输出参数，合成组的配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_GRP_ID                - 无效组号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_CONFIG                    - 合成组配置信息未配置
 *  注意：
 *      需要先设置合成组的配置信息
 */
xmedia_s32 xmedia_vi_get_grp_config(xmedia_s32 grp, xmedia_vi_grp_config *grp_config);

/*
 *  函数功能：创建一个VI PIPE
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pipe_config                                  - 输入参数，pipe的配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_BIND                      - pipe未绑定
 *      XMEDIA_ERRCODE_EXIST                         - pipe已存在
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY             - 内存不足
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *  注意：
 *      不支持重复创建
 */
xmedia_s32 xmedia_vi_create_pipe(xmedia_s32 pipe, const xmedia_vi_pipe_config *pipe_config);

/*
 *  函数功能：销毁一个VI PIPE
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *  注意：
 *      需要先禁用PIPE
 */
xmedia_s32 xmedia_vi_destroy_pipe(xmedia_s32 pipe);

/*
 *  函数功能：获取VI PIPE的配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pipe_config                                  - 输出参数，pipe的配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_config(xmedia_s32 pipe, xmedia_vi_pipe_config *pipe_config);

/*
 *  函数功能：启用VI PIPE
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_start_pipe(xmedia_s32 pipe);

/*
 *  函数功能：禁用VI PIPE
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_stop_pipe(xmedia_s32 pipe);

/*
 *  函数功能：设置VI PIPE的压缩模式
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      compress_mode                                - 输入参数，压缩模式
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 */
xmedia_s32 xmedia_vi_set_pipe_compress_mode(xmedia_s32 pipe, const xmedia_video_compress_mode compress_mode);

/*
 *  函数功能：获取VI PIPE的压缩模式
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      compress_mode                                - 输入参数，压缩模式
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 */
xmedia_s32 xmedia_vi_get_pipe_compress_mode(xmedia_s32 pipe, xmedia_video_compress_mode *compress_mode);

/*
 *  函数功能：设置VI PIPE的帧率控制信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pipe_frm_rate                                - 输入参数，帧率控制信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe的帧率控制只有离线才有效
 *      2、目标帧率必须小于或等于原始帧率
 */
xmedia_s32 xmedia_vi_set_pipe_frame_rate(xmedia_s32 pipe, const xmedia_video_frame_rate_ctrl *pipe_frm_rate);

/*
 *  函数功能：获取VI PIPE的帧率控制信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pipe_frm_rate                                - 输出参数，帧率控制信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_frame_rate(xmedia_s32 pipe, xmedia_video_frame_rate_ctrl *pipe_frm_rate);

/*
 *  函数功能：设置用户图片
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      user_pic_info                                - 输入参数，用户图片帧信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、只支持VI离线模式
 *      2、只支持YUV图像模式
 *      3、此功能与pipe灌图功能互斥
 *      4、配置的用户图片的宽高必须与 VI PIPE 大小相一致
 */
xmedia_s32 xmedia_vi_set_user_pic(xmedia_s32 pipe, const xmedia_video_frame_info *user_pic_info);

/*
 *  函数功能：启用VI PIPE用户图片
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 *  注意：
 *      只支持VI离线模式
 */
xmedia_s32 xmedia_vi_enable_user_pic(xmedia_s32 pipe);

/*
 *  函数功能：禁用VI PIPE用户图片
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 *  注意：
 *      只支持VI离线模式
 */
xmedia_s32 xmedia_vi_disable_user_pic(xmedia_s32 pipe);

/*
 *  函数功能：设置 VI 物理 PIPE 的裁剪功能属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      crop_attr                                    - 输入参数，裁剪属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_pipe_crop_attr(xmedia_s32 pipe, const xmedia_video_crop_attr *crop_attr);

/*
 *  函数功能：获取 VI 物理 PIPE 的裁剪功能属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      crop_attr                                    - 输出参数，裁剪属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_crop_attr(xmedia_s32 pipe, xmedia_video_crop_attr *crop_attr);

/*
 *  函数功能：设置 VI 物理 PIPE dump 图像帧属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      dump_attr                                    - 输入参数，VI 物理 PIPE dump 的属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_pipe_frame_dump_attr(xmedia_s32 pipe, const xmedia_vi_frame_dump_attr *dump_attr);

/*
 *  函数功能：获取 VI 物理 PIPE dump 图像帧属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      dump_attr                                    - 输出参数，VI 物理 PIPE dump 的属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_frame_dump_attr(xmedia_s32 pipe, xmedia_vi_frame_dump_attr *dump_attr);

/*
 *  函数功能：获取 VI PIPE 图像帧
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      frame_info                                   - 输出参数，帧信息
 *      milli_sec                                    - 输入参数，超时参数
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      ERESTARTSYS                                  - 被其他信号中断
 *      XMEDIA_ERRCODE_BUFFER_EMPTY                  - buffer队列为空
 *      XMEDIA_ERRCODE_NOT_READY                     - 系统控制配置信息未配置
 *  注意：
 *      1、pipe必须已创建
 *      2、需要先设置dump属性，否则获取不到帧数据
 */
xmedia_s32 xmedia_vi_acquire_pipe_frame(xmedia_s32 pipe, xmedia_video_frame_info *frame_info, xmedia_s32 milli_sec);

/*
 *  函数功能：释放 VI PIPE 图像帧
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      frame_info                                   - 输入参数，帧信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *  注意：
 *      1、本接口调用次数需与 xmedia_vi_acquire_pipe_frame 匹配使用
 *      2、用户必须保证 frame_info 结构中的信息与获取时一致
 */
xmedia_s32 xmedia_vi_release_pipe_frame(xmedia_s32 pipe, const xmedia_video_frame_info *frame_info);

/*
 *  函数功能：设置 VI PIPE 数据的来源
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      frame_source                                 - 输入参数，pipe的数据来源
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_pipe_frame_source(xmedia_s32 pipe, const xmedia_vi_pipe_frame_source frame_source);

/*
 *  函数功能：获取 VI PIPE 数据的来源
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      frame_source                                 - 输出参数，pipe的数据来源
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_frame_source(xmedia_s32 pipe, xmedia_vi_pipe_frame_source *frame_source);

/*
 *  函数功能：通过 VI PIPE 发送YUV/RAW数据
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      send_frame_info                              - 输入参数，帧信息
 *      milli_sec                                    - 超时参数
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      ERESTARTSYS                                  - 被其他信号中断
 *      XMEDIA_ERRCODE_BUFFER_FULL                   - buffer队列已满
 *      XMEDIA_ERRCODE_NOT_READY                     - 系统控制配置信息未配置
 *  注意：
 *      1、pipe必须已创建且启动
 *      2、灌数据前，需要设置pipe的数据来源于用户。
 *      3、wdr模式灌raw时，传入的数据必须是长短帧的顺序，保证长短曝光帧的顺序正确。
 *      4、送raw时，pipe号必须为wdr融合的主pipe号。
 *      5、送下来的帧数据的宽高、像素格式等必须与pipe配置信息保持一致。
 *      6、只支持VI离线模式。
 */
xmedia_s32 xmedia_vi_send_pipe_frame(xmedia_s32 pipe, const xmedia_vi_send_frame_info *send_frame_info);

/*
 *  函数功能：设置 VI PIPE 低延时属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      lowdelay_attr                                - 输入参数，低延时功能属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、只支持VI离线
 */
xmedia_s32 xmedia_vi_set_pipe_low_delay_attr(xmedia_s32 pipe, const xmedia_vi_lowdelay_attr *lowdelay_attr);

/*
 *  函数功能：获取 VI PIPE 低延时属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      lowdelay_attr                                - 输出参数，低延时功能属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、只支持VI离线
 */
xmedia_s32 xmedia_vi_get_pipe_low_delay_attr(xmedia_s32 pipe, xmedia_vi_lowdelay_attr *lowdelay_attr);

/*
 *  函数功能：查询 VI PIPE 状态
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pipe_status                                  - 输出参数，pipe状态信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_query_pipe_status(xmedia_s32 pipe, xmedia_vi_pipe_status *pipe_status);

/*
 *  函数功能：设置 VI 物理 PIPE 对接前端 sensor 或者 AD 的 VC 号
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      vc_number                                    - 输入参数，VC号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *  注意：
 *      必须在pipe创建后，使能之前调用
 */
xmedia_s32 xmedia_vi_set_pipe_vc_number(xmedia_s32 pipe, const xmedia_u32 vc_number);

/*
 *  函数功能：获取 VI 物理 PIPE 对接前端 sensor 或者 AD 的 VC 号
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      vc_number                                    - 输出参数，VC号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_vc_number(xmedia_s32 pipe, xmedia_u32 *vc_number);

/*
 *  函数功能：设置 VI 物理 PIPE 输出镜像/翻转
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      mirror                                       - 输入参数，mirror使能
 *      flip                                         - 输入参数，flip使能
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_pipe_mirror_flip(xmedia_s32 pipe, xmedia_bool mirror, xmedia_bool flip);

/*
 *  函数功能：获取 VI 物理 PIPE 输出镜像/翻转开关状态
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      mirror                                       - 输出参数，mirror使能
 *      flip                                         - 输出参数，flip使能
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_mirror_flip(xmedia_s32 pipe, xmedia_bool *mirror, xmedia_bool *flip);

/*
 *  函数功能：设置VI通道配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      chn_config                                   - 输入参数，通道配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_DISABLE                   - 通道未禁用
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_chn_config(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_vi_chn_config *chn_config);

/*
 *  函数功能：获取VI通道配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      chn_config                                   - 输出参数，通道配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_CONFIG                    - 设备或通道配置信息未配置
 *  注意：
 *      1、pipe必须已创建
 *      2、需要先设置通道配置信息，才可获取
 */
xmedia_s32 xmedia_vi_get_chn_config(xmedia_s32 pipe, xmedia_s32 chn, xmedia_vi_chn_config *chn_config);

/*
 *  函数功能：设置VI CHN输出的压缩模式
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      compress_mode                                - 输入参数，压缩模式
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_chn_compress_mode(xmedia_s32 pipe, xmedia_s32 chn,
                                           const xmedia_video_compress_mode compress_mode);

/*
 *  函数功能：获取VI CHN输出的压缩模式
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      compress_mode                                - 输出参数，压缩模式
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_compress_mode(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_compress_mode *compress_mode);

/*
 *  函数功能：设置VI CHN的帧率控制信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      pipe_frm_rate                                - 输入参数，帧率控制信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      1、pipe必须已创建
 *      2、目标帧率必须小于或等于原始帧率
 */
xmedia_s32 xmedia_vi_set_chn_frame_rate(xmedia_s32 pipe, xmedia_s32 chn,
                                        const xmedia_video_frame_rate_ctrl *chn_frm_rate);

/*
 *  函数功能：获取VI CHN的帧率控制信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      pipe_frm_rate                                - 输出参数，帧率控制信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_frame_rate(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_frame_rate_ctrl *chn_frm_rate);

/*
 *  函数功能：设置用户获取通道图像的队列深度
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      depth                                        - 输入参数，用户获取图像的队列深度
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_chn_depth(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_u32 depth);

/*
 *  函数功能：获取用户获取通道图像的队列深度
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      depth                                        - 输出参数，用户获取图像的队列深度
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_depth(xmedia_s32 pipe, xmedia_s32 chn, xmedia_u32 *depth);

/*
 *  函数功能：设置VI通道裁剪功能属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      crop_attr                                    - 输入参数，裁剪属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、物理通道开启 Dis/Ldc/Spread/Rotation/Fisheye
 * 后，为防止这些功能的输入图像大小发生变化，要求不能再设置裁剪属性。
 *      3、通道开启低延时时，裁剪的高度不能小于低延时行号。
 */
xmedia_s32 xmedia_vi_set_chn_crop_attr(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_video_crop_attr *crop_attr);

/*
 *  函数功能：获取VI通道裁剪功能属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      crop_attr                                    - 输出参数，裁剪属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_crop_attr(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_crop_attr *crop_attr);

/*
 *  函数功能：设置VI镜头畸变校正属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      ldc_attr                                     - 输入参数，ldc属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、必须在设置通道配置信息后才能设置
 *      3、设置ldc后，不支持crop、rotation、mirror、flip
 *      4、设置ldc后，不支持fisheye、视场角矫正、stitch correction
 */
xmedia_s32 xmedia_vi_set_chn_ldc_attr(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_video_ldc_attr *ldc_attr);

/*
 *  函数功能：获取VI镜头畸变校正属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      ldc_attr                                     - 输出参数，ldc属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_ldc_attr(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_ldc_attr *ldc_attr);

/*
 *  函数功能：设置 VI 通道展宽属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      spread_attr                                  - 输入参数，展宽属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、必须在设置通道配置信息后才能设置
 *      3、设置spread后，不支持crop、rotation、mirror、flip
 *      4、设置spread后、不支持fisheye、视场角矫正、stitch correction
 */
xmedia_s32 xmedia_vi_set_chn_spread_attr(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_vi_spread_attr *spread_attr);

/*
 *  函数功能：获取 VI 通道展宽属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      spread_attr                                  - 输出参数，展宽属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_spread_attr(xmedia_s32 pipe, xmedia_s32 chn, xmedia_vi_spread_attr *spread_attr);

/*
 *  函数功能：设置 VI 通道低延时属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      lowdelay_attr                                - 输入参数，低延时功能属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、当通道设置 chn_crop 时，低延时行号应小于通道 crop 的高度
 */
xmedia_s32 xmedia_vi_set_chn_low_delay_attr(xmedia_s32 pipe, xmedia_s32 chn,
                                            const xmedia_vi_lowdelay_attr *lowdelay_attr);

/*
 *  函数功能：获取 VI 通道低延时属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      lowdelay_attr                                - 输出参数，低延时功能属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_low_delay_attr(xmedia_s32 pipe, xmedia_s32 chn, xmedia_vi_lowdelay_attr *lowdelay_attr);

/*
 *  函数功能：设置 VI 通道卷绕功能属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      wrap_attr                                    - 输入参数，卷绕功能属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_READY                     - 系统控制配置信息未配置
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_chn_wrap_attr(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_video_wrap_attr *wrap_attr);

/*
 *  函数功能：获取 VI 通道卷绕功能属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      wrap_attr                                    - 输出参数，卷绕功能属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_wrap_attr(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_wrap_attr *wrap_attr);

/*
 *  函数功能：设置 VI 通道的 DIS 配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      dis_config                                   - 输入参数，dis配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_chn_dis_config(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_vi_dis_config *dis_config);

/*
 *  函数功能：获取 VI 通道的 DIS 配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      dis_config                                   - 输出参数，dis配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_dis_config(xmedia_s32 pipe, xmedia_s32 chn, xmedia_vi_dis_config *dis_config);

/*
 *  函数功能：设置 VI 通道的 DIS 属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      dis_attr                                     - 输入参数，dis属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、必须先使能 VI 通道和设置 DIS 配置信息后，才能设置 DIS 属性。
 *      3、设置dis后，不支持crop、rotation、mirror、flip
 *      4、设置dis后，不支持fisheye、视场角矫正、stitch correction
 */
xmedia_s32 xmedia_vi_set_chn_dis_attr(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_vi_dis_attr *dis_attr);

/*
 *  函数功能：获取 VI 通道的 DIS 属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      dis_attr                                     - 输出参数，dis属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_dis_attr(xmedia_s32 pipe, xmedia_s32 chn, xmedia_vi_dis_attr *dis_attr);

/*
 *  函数功能：设置 VI 通道的视场角矫正属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      fov_correction_attr                          - 输入参数，视场角矫正属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、设置视场角矫正后，不支持crop、rotation、mirror、flip
 *      3、设置视场角矫正后，不支持fisheye、stitch correction、ldc、spread、dis
 */
xmedia_s32 xmedia_vi_set_chn_fov_correction_attr(xmedia_s32 pipe, xmedia_s32 chn,
                                                 const xmedia_vi_fov_correction_attr *fov_correction_attr);

/*
 *  函数功能：获取 VI 通道的视场角矫正属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      fov_correction_attr                          - 输出参数，视场角矫正属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_fov_correction_attr(xmedia_s32 pipe, xmedia_s32 chn,
                                                 xmedia_vi_fov_correction_attr *fov_correction_attr);

/*
 *  函数功能：设置 VI 通道对应的鱼眼属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      correction_attr                              - 输入参数，鱼眼属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、设置fisheye后，不支持crop、rotation、mirror、flip
 *      3、设置fisheye后，不支持视场角矫正、stitch correction、ldc、dis、spread
 */
xmedia_s32 xmedia_vi_set_chn_fisheye_attr(xmedia_s32 pipe, xmedia_s32 chn,
                                          const xmedia_vi_fisheye_correction_attr *fisheye_correction_attr);

/*
 *  函数功能：获取 VI 通道对应的鱼眼属性
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      correction_attr                              - 输出参数，鱼眼属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_fisheye_attr(xmedia_s32 pipe, xmedia_s32 chn,
                                          xmedia_vi_fisheye_correction_attr *fisheye_correction_attr);

/*
 *  函数功能：根据鱼眼校正输出图像坐标点查找源图像坐标点。
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      region_index                                 - 输入参数，区域号
 *      dst_point                                    - 输入参数，目标坐标
 *      src_point                                    - 输出参数，源坐标
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、必须先设置鱼眼属性
 */
xmedia_s32 xmedia_vi_fisheye_pos_query_dst_to_src(xmedia_s32 pipe, xmedia_s32 chn, xmedia_u32 region_index,
                                                  const xmedia_video_point *dst_point, xmedia_video_point *src_point);

/*
 *  函数功能：设置 VI 通道对应的拼接属性
 *  函数参数：
 *      grp                                          - 输入参数，组号
 *      vi_stitch_attr                               - 输入参数，拼接属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 组内无效pipe号
 *      XMEDIA_ERRCODE_INVALID_GRP_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、设置stitch后，不支持crop、rotation、mirror、flip
 *      3、设置stitch后，不支持视场角矫正、fisheye、dis、spread
 */
xmedia_s32 xmedia_vi_set_chn_stitch_attr(xmedia_s32 grp, const xmedia_vi_stitch_attr *vi_stitch_attr);

/*
 *  函数功能：获取 VI 通道对应的拼接属性
 *  函数参数：
 *      grp                                          - 输入参数，组号
 *      vi_stitch_attr                               - 输入参数，拼接属性
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 组内无效pipe号
 *      XMEDIA_ERRCODE_INVALID_GRP_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_stitch_attr(xmedia_s32 grp, xmedia_vi_stitch_attr *vi_stitch_attr);

/*
 *  函数功能：启用 VI 通道
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY             - 分配内存失败
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT                   - 操作不支持
 *  注意：
 *      1、pipe必须已创建
 *      2、必须先设置通道配置信息
 *      3、VPSS在线模式下，启用VI通道不生效，返回成功
 */
xmedia_s32 xmedia_vi_enable_chn(xmedia_s32 pipe, xmedia_s32 chn);

/*
 *  函数功能：禁用 VI 通道
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      1、pipe必须已创建
 *      2、VPSS在线模式下，禁用VI通道不生效，返回成功
 */
xmedia_s32 xmedia_vi_disable_chn(xmedia_s32 pipe, xmedia_s32 chn);

/*
 *  函数功能：从 VI 通道获取采集的图像
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      frame_info                                   - 输出参数，帧信息
 *      milli_sec                                    - 输入参数，超时参数
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *      ERESTARTSYS                                  - 被其他信号中断
 *      XMEDIA_ERRCODE_BUFFER_EMPTY                  - buffer队列为空
 *      XMEDIA_ERRCODE_NOT_READY                     - 系统控制配置信息未配置
 *  注意：
 *      1、pipe必须已创建
 *      2、此接口需在通道已启用，且通道队列深度不为 0，才能获取到图像
 *      3、支持多次获取后再释放，但建议获取和释放接口配对使用
 */
xmedia_s32 xmedia_vi_acquire_chn_frame(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_frame_info *frame_info,
                                       xmedia_s32 milli_sec);

/*
 *  函数功能：释放一帧从 VI 通道获取的图像
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      frame_info                                   - 输入参数，帧信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED                 - 操作不允许
 *  注意：
 *      1、本接口调用次数需与 xmedia_vi_acquire_chn_frame 匹配使用
 *      2、用户必须保证 frame_info 结构中的信息与获取时一致
 */
xmedia_s32 xmedia_vi_release_chn_frame(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_video_frame_info *frame_info);

/*
 *  函数功能：监测输出通道队列是否有图像
 *  函数参数：
 *      watch_chn                                    - 输入参数，被监测的通道
 *      watch_chn_num                                - 输入参数，被监测的通道数量
 *      valid_chn                                    - 输出参数，有数据的通道
 *      valid_chn_num                                - 输出参数，有数据的通道数量
 *      milli_sec                                    - 输入参数，超时参数
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_READY                     - 未加载驱动/未初始化
 *      XMEDIA_ERRCODE_NULL_PTR                      - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 非法参数
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      ERESTARTSYS                                  - 被其他信号中断
 *      XMEDIA_ERRCODE_TIMEOUT                       - 超时
 *      XMEDIA_FAILURE                               - 失败
 */
xmedia_s32 xmedia_vi_select_chn_frame(const xmedia_chn_info *watch_chn, xmedia_u32 watch_chn_num,
                                      xmedia_chn_info *valid_chn, xmedia_u32 *valid_chn_num, xmedia_s32 milli_sec);

/*
 *  函数功能：设置VI扩展通道配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，扩展通道号
 *      chn_config                                   - 输入参数，扩展通道配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效扩展通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_DISABLE                   - chn已启动
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_set_ext_chn_config(xmedia_s32 pipe, xmedia_s32 chn, const xmedia_vi_ext_chn_config *chn_config);

/*
 *  函数功能：获取VI扩展通道配置信息
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，扩展通道号
 *      chn_config                                   - 输出参数，扩展通道配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效扩展通道号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *      XMEDIA_ERRCODE_NOT_CONFIG                    - 扩展通道配置信息未配置
 *  注意：
 *      1、pipe必须已创建
 *      2、需要先设置通道配置信息，才可获取
 */
xmedia_s32 xmedia_vi_get_ext_chn_config(xmedia_s32 pipe, xmedia_s32 chn, xmedia_vi_ext_chn_config *chn_config);

/*
 *  函数功能：pipe绑定user vb池
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pool_id                                      - 输入参数，指定vb池子id
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_attach_pipe_vb_pool(xmedia_s32 pipe, xmedia_u32 pool_id);

/*
 *  函数功能：pipe解绑user vb池
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_detach_pipe_vb_pool(xmedia_s32 pipe);

/*
 *  函数功能：chn绑定user vb池
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *      pool_id                                      - 输入参数，指定vb池子id
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_attach_chn_vb_pool(xmedia_s32 pipe, xmedia_s32 chn, xmedia_u32 pool_id);

/*
 *  函数功能：chn解绑user vb池
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，通道号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_detach_chn_vb_pool(xmedia_s32 pipe, xmedia_s32 chn);

/*
 *  函数功能：ainr输出绑定user vb池
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      pool_id                                      - 输入参数，指定vb池子id
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_attach_ainr_vb_pool(xmedia_s32 pipe, xmedia_u32 pool_id);

/*
 *  函数功能：ainr输出解绑user vb池
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      XMEDIA_SUCCESS                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_NOT_EXIST                     - pipe不存在
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_detach_ainr_vb_pool(xmedia_s32 pipe);

/*
 *  函数功能：获取pipe对应的文件描述符fd
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *  返回值：
 *      对应文件描述符fd                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_PARAM                 - 无效参数
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_pipe_fd(xmedia_s32 pipe);

/*
 *  函数功能：获取chn对应的文件描述符fd
 *  函数参数：
 *      pipe                                         - 输入参数，pipe号
 *      chn                                          - 输入参数，chn号
 *  返回值：
 *      对应文件描述符fd                               - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT                      - vi未初始化
 *      XMEDIA_ERRCODE_INVALID_PIPE_ID               - 无效pipe号
 *      XMEDIA_ERRCODE_INVALID_CHN_ID                - 无效通道号
 *  注意：
 *      pipe必须已创建
 */
xmedia_s32 xmedia_vi_get_chn_fd(xmedia_s32 pipe, xmedia_s32 chn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
