/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_VO_H__
#define __XMEDIA_VO_H__

#include "xmedia_type.h"
#include "xmedia_video_common.h"
#include "xmedia_errcode.h"
#include "xmedia_intf_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef xmedia_s32 xmedia_vo_chn;

typedef enum {
    XMEDIA_VO_DEV_0 = 0,
    XMEDIA_VO_DEV_1,
    XMEDIA_VO_DEV_VIRT_0,
    XMEDIA_VO_DEV_VIRT_1,
    XMEDIA_VO_DEV_MAX
}xmedia_vo_dev;

typedef enum {
    XMEDIA_VO_LAYER_V0 = 0,
    XMEDIA_VO_LAYER_V1,
    XMEDIA_VO_LAYER_VIRT_V0,
    XMEDIA_VO_LAYER_VIRT_V1,
    XMEDIA_VO_LAYER_MAX
}xmedia_vo_layer;

//定义输出接口时序
typedef enum {
    XMEDIA_VO_INTF_SYNC_PAL = 0,
    XMEDIA_VO_INTF_SYNC_NTSC,

    XMEDIA_VO_INTF_SYNC_1080P24,
    XMEDIA_VO_INTF_SYNC_1080P25,
    XMEDIA_VO_INTF_SYNC_1080P30,

    XMEDIA_VO_INTF_SYNC_720P50,
    XMEDIA_VO_INTF_SYNC_720P60,
    XMEDIA_VO_INTF_SYNC_1080I50,
    XMEDIA_VO_INTF_SYNC_1080I60,
    XMEDIA_VO_INTF_SYNC_1080P50,
    XMEDIA_VO_INTF_SYNC_1080P60,

    XMEDIA_VO_INTF_SYNC_576P50 = 11,
    XMEDIA_VO_INTF_SYNC_480P60,

    XMEDIA_VO_INTF_SYNC_800x600_60 = 13,
    XMEDIA_VO_INTF_SYNC_1024x768_60,
    XMEDIA_VO_INTF_SYNC_1280x1024_60,
    XMEDIA_VO_INTF_SYNC_1366x768_60,
    XMEDIA_VO_INTF_SYNC_1440x900_60,
    XMEDIA_VO_INTF_SYNC_1280x800_60,
    XMEDIA_VO_INTF_SYNC_1600x1200_60,
    XMEDIA_VO_INTF_SYNC_1680x1050_60,
    XMEDIA_VO_INTF_SYNC_1920x1200_60,
    XMEDIA_VO_INTF_SYNC_640x480_60,
    XMEDIA_VO_INTF_SYNC_960H_PAL,
    XMEDIA_VO_INTF_SYNC_960H_NTSC,
    XMEDIA_VO_INTF_SYNC_1920x2160_30,
    XMEDIA_VO_INTF_SYNC_2560x1440_30,
    XMEDIA_VO_INTF_SYNC_2560x1440_60,
    XMEDIA_VO_INTF_SYNC_2560x1600_60,

    XMEDIA_VO_INTF_SYNC_3840x2160_24 = 29,
    XMEDIA_VO_INTF_SYNC_3840x2160_25,
    XMEDIA_VO_INTF_SYNC_3840x2160_30,
    XMEDIA_VO_INTF_SYNC_3840x2160_50,
    XMEDIA_VO_INTF_SYNC_3840x2160_60,
    XMEDIA_VO_INTF_SYNC_4096x2160_24,
    XMEDIA_VO_INTF_SYNC_4096x2160_25,
    XMEDIA_VO_INTF_SYNC_4096x2160_30,
    XMEDIA_VO_INTF_SYNC_4096x2160_50,
    XMEDIA_VO_INTF_SYNC_4096x2160_60,

    XMEDIA_VO_INTF_SYNC_320x240_60 = 39,
    XMEDIA_VO_INTF_SYNC_320x240_50,
    XMEDIA_VO_INTF_SYNC_240x320_50,
    XMEDIA_VO_INTF_SYNC_240x320_60,
    XMEDIA_VO_INTF_SYNC_800x600_50,
    XMEDIA_VO_INTF_SYNC_800x480_50,
    XMEDIA_VO_INTF_SYNC_720x1280_60,
    XMEDIA_VO_INTF_SYNC_1080x1920_60,
    XMEDIA_VO_INTF_SYNC_7680x4320_30,
    XMEDIA_VO_INTF_SYNC_USER = 48,

    XMEDIA_VO_INTF_SYNC_MAX
}xmedia_vo_intf_sync;

//定义设备模式
typedef enum{
    XMEDIA_VO_DEV_MODE_VIDEO_GRAPHIC = 0,
    XMEDIA_VO_DEV_MODE_VIDEO,
    XMEDIA_VO_DEV_MODE_GRAPHIC,
    XMEDIA_VO_DEV_MODE_MAX
}xmedia_vo_dev_mode;

//定义lcd的数据传输模式
typedef enum {
    XMEDIA_VO_LCD_DATA_MODE_SERIAL = 0,
    XMEDIA_VO_LCD_DATA_MODE_PARA,
    XMEDIA_VO_LCD_DATA_MODE_MAX
} xmedia_vo_lcd_data_mode;

//定义视频分割模式
typedef enum {
    XMEDIA_VO_PARTITION_MODE_SINGLE = 0, //单区域模式，用软件将多张图片进行拼接显示在硬件的一个区域上
    XMEDIA_VO_PARTITION_MODE_MULTI,      //多区域模式，每个区域对应硬件的一个区域
    XMEDIA_VO_PARTITION_MODE_MAX,
}xmedia_vo_partition_mode;

//定义局部放大类型
typedef enum {
    XMEDIA_VO_ZOOM_IN_RECT = 0,
    XMEDIA_VO_ZOOM_IN_RATIO,
    XMEDIA_VO_ZOOM_IN_MAX
} xmedia_vo_zoom_in_type;

//定义视频低延时模式
typedef enum {
    XMEDIA_VO_LOW_DELAY_NONE = 0,   //普通模式
    XMEDIA_VO_LOW_DELAY_NORMAL,     //软件低延时
    XMEDIA_VO_LOW_DELAY_MAX
} xmedia_vo_low_delay_mode;

//定义用户同步时序
typedef struct {
    xmedia_u16 vact;  //垂直有效区
    xmedia_u16 vbb;   //垂直消隐后肩
    xmedia_u16 vfb;   //垂直消隐前肩

    xmedia_u16 hact;  //水平有效区
    xmedia_u16 hbb;   //水平消隐后肩
    xmedia_u16 hfb;   //水平消隐前肩
    xmedia_u16 hmid;  //底场同步水平有效区域

    xmedia_u16 bvact; //底场垂直有效区
    xmedia_u16 bvbb;  //底场垂直消隐后肩
    xmedia_u16 bvfb;  //底场垂直消隐前肩

    xmedia_u16 hpw;   //水平脉冲宽度
    xmedia_u16 vpw;   //垂直脉冲宽度

    xmedia_bool idv;  //数据有效信号的极性
    xmedia_bool ihs;  //水平有效信号的极性
    xmedia_bool ivs;  //垂直有效信号的极性

    xmedia_bool synm; //同步模式
    xmedia_bool iop;  //隔行或者逐行显示(0表示隔行，1表示逐行)
    xmedia_u8 intfb;  //隔行输出的位宽
} xmedia_vo_user_sync_timing;

//定义用户同步信息
typedef struct {
    xmedia_vo_user_sync_timing timing_info;   //输出接口的时序信息
    xmedia_u32 frame_rate;                    //用户同步输出帧率
    xmedia_bool clk_reverse_en;               //时钟是否翻转
} xmedia_vo_user_sync_config;

//定义BT656的属性
typedef struct {
    xmedia_intf_bt_data_type data_type;
    xmedia_intf_bt601_bt656_data_seq data_seq;
} xmedia_vo_bt656_attr;

//定义BT1120的属性
typedef struct {
    xmedia_intf_bt_data_type data_type;
    xmedia_intf_bt1120_data_seq data_seq;
} xmedia_vo_bt1120_attr;

//定义LCD的属性
typedef struct {
    xmedia_intf_lcd_data_type data_type;
    xmedia_vo_lcd_data_mode data_mode;
    xmedia_intf_lcd_data_seq data_seq;
} xmedia_vo_lcd_attr;

typedef enum {
    XMEDIA_VO_MIPI_LANE_DATA0 = 0,
    XMEDIA_VO_MIPI_LANE_DATA1,
    XMEDIA_VO_MIPI_LANE_CLK,
    XMEDIA_VO_MIPI_LANE_DATA2,
    XMEDIA_VO_MIPI_LANE_DATA3,
    XMEDIA_VO_MIPI_LANE_MAX
}xmedia_vo_mipi_lane;

typedef enum {
    XMEDIA_VO_MIPI_LANE_DEFAULT = 0,
    XMEDIA_VO_MIPI_LANE_NUM1,
    XMEDIA_VO_MIPI_LANE_NUM2,
    XMEDIA_VO_MIPI_LANE_NUM3,
    XMEDIA_VO_MIPI_LANE_NUM4,
    XMEDIA_VO_MIPI_LANE_NUM_MAX
}xmedia_vo_mipi_lane_num;

//定义MIPI的属性
typedef struct {
    xmedia_intf_mipi_dsi_data_type data_type;
    xmedia_bool lane_pn_swap[XMEDIA_VO_MIPI_LANE_MAX];
    xmedia_vo_mipi_lane lane_sel[XMEDIA_VO_MIPI_LANE_MAX];
    xmedia_s32 mipi_htotal_adjust;
    xmedia_s32 mipi_vsync_adjust;
    xmedia_vo_mipi_lane_num mipi_lane_num;
} xmedia_vo_mipi_attr;

typedef enum {
    XMEDIA_VO_LVDS_LANE_DATA0 = 0,
    XMEDIA_VO_LVDS_LANE_DATA1,
    XMEDIA_VO_LVDS_LANE_DATA2,
    XMEDIA_VO_LVDS_LANE_DATA3,
    XMEDIA_VO_LVDS_LANE_CLK,
    XMEDIA_VO_LVDS_LANE_MAX
}xmedia_vo_lvds_lane;

//定义LVDS的属性
typedef struct {
    xmedia_intf_lvds_data_type data_type;
    xmedia_intf_lvds_format format;
    xmedia_vo_lvds_lane lane_sel[XMEDIA_VO_LVDS_LANE_MAX];
    xmedia_bool lane_pn_swap[XMEDIA_VO_LVDS_LANE_MAX];
} xmedia_vo_lvds_attr;

//定义mcu的数据传输模式
typedef enum {
    XMEDIA_VO_MCU_DATA_TYPE_8BIT_16BPP_RGB565_8080I = 0,
    XMEDIA_VO_MCU_DATA_TYPE_8BIT_18BPP_RGB666_8080I,
    XMEDIA_VO_MCU_DATA_TYPE_8BIT_16BPP_RGB565_8080II,
    XMEDIA_VO_MCU_DATA_TYPE_8BIT_18BPP_RGB666_8080II,

    XMEDIA_VO_MCU_DATA_TYPE_9BIT_16BPP_RGB565_8080I,
    XMEDIA_VO_MCU_DATA_TYPE_9BIT_18BPP_RGB666_8080I_OPTION1,
    XMEDIA_VO_MCU_DATA_TYPE_9BIT_18BPP_RGB666_8080I_OPTION2,
    XMEDIA_VO_MCU_DATA_TYPE_9BIT_16BPP_RGB565_8080II,
    XMEDIA_VO_MCU_DATA_TYPE_9BIT_18BPP_RGB666_8080II_OPTION1,
    XMEDIA_VO_MCU_DATA_TYPE_9BIT_18BPP_RGB666_8080II_OPTION2,

    XMEDIA_VO_MCU_DATA_TYPE_16BIT_16BPP_RGB565_8080I,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080I_OPTION1,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080I_OPTION2,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080I_OPTION3,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080I_OPTION4,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080I_OPTION5,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_16BPP_RGB565_8080II,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080II_OPTION1,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080II_OPTION2,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080II_OPTION3,
    XMEDIA_VO_MCU_DATA_TYPE_16BIT_18BPP_RGB666_8080II_OPTION4,

    XMEDIA_VO_MCU_DATA_TYPE_18BIT_16BPP_RGB565_8080I,
    XMEDIA_VO_MCU_DATA_TYPE_18BIT_18BPP_RGB666_8080I,
    XMEDIA_VO_MCU_DATA_TYPE_18BIT_16BPP_RGB565_8080II,
    XMEDIA_VO_MCU_DATA_TYPE_18BIT_18BPP_RGB666_8080II,

    XMEDIA_VO_MCU_DATA_TYPE_3LINE_RGB565_OPTION1,
    XMEDIA_VO_MCU_DATA_TYPE_3LINE_RGB666_OPTION1,
    XMEDIA_VO_MCU_DATA_TYPE_3LINE_RGB565_OPTION2,
    XMEDIA_VO_MCU_DATA_TYPE_3LINE_RGB666_OPTION2,
    XMEDIA_VO_MCU_DATA_TYPE_4LINE_RGB565,
    XMEDIA_VO_MCU_DATA_TYPE_4LINE_RGB666,
    XMEDIA_VO_MCU_DATA_TYPE_MAX
} xmedia_intf_mcu_data_type;

//定义MCU的属性
typedef struct {
    xmedia_bool te_mode_en;
    xmedia_intf_mcu_data_type data_type;
} xmedia_vo_mcu_attr;

//定义输出接口信息
typedef struct {
    xmedia_intf_type intf_type;
    union {
        xmedia_vo_bt656_attr bt656_attr;
        xmedia_vo_bt1120_attr bt1120_attr;
        xmedia_vo_lcd_attr lcd_attr;
        xmedia_vo_mipi_attr mipi_attr;
        xmedia_vo_lvds_attr lvds_attr;
        xmedia_vo_mcu_attr mcu_attr;
    };
} xmedia_vo_intf_config;

//定义设备属性
typedef struct {
    xmedia_u32 bg_color;                     //设备的背景色，RGB格式
    xmedia_vo_intf_config intf_config;       //输出接口属性
    xmedia_vo_intf_sync intf_sync;           //输出接口的时序
    xmedia_vo_user_sync_config user_sync_config; //用户同步属性
} xmedia_vo_dev_config;

//定义设备的csc属性
typedef struct {
    xmedia_vo_dev_mode mode;                  //设备模式
    xmedia_video_color_descript color_info;   //颜色信息，包含输出色域空间，色域范围和量化范围
    xmedia_u32 luma;                          //亮度范围为[0, 100]，默认50
    xmedia_u32 contrast;                      //对比度度范围为[0, 100]，默认50
    xmedia_u32 hue;                           //色度范围为[0, 100]，默认50
    xmedia_u32 saturation;                    //饱和度范围为[0, 100]，默认50
} xmedia_vo_dev_csc;

typedef struct {
    xmedia_u8 gamma_table[VO_GAMMA_MAP_SIZE];
} xmedia_vo_gamma_table;

//定义设备的gamma属性
typedef struct {
    xmedia_bool gamma_en;                     //gamma使能状态
    xmedia_vo_gamma_table gamma_table;        //gamma表
} xmedia_vo_dev_gamma;

//定义视频层输出属性
typedef struct {
    xmedia_video_size img_size;               //视频层的画布尺寸
    xmedia_video_pixel_format pix_format;     //视频层的像素格式
    xmedia_video_data_width bit_width;;       //视频层数据位宽
    xmedia_u32 disp_buf_len;                  //显示buf的长度
    xmedia_vo_partition_mode  part_mode;      //视频层的分割模式
} xmedia_vo_layer_config;

//定义通道属性
typedef struct {
    xmedia_u32 zorder;            //通道顺序
    xmedia_video_rect out_rect;   //输出通道的分辨率和位置
    xmedia_u32 bg_color;          //输出通道区域背景色
    xmedia_video_rect region_rect;//输出通道的区域分辨率和位置
} xmedia_vo_chn_attr;

//定义通道局部放大属性
typedef struct {
    xmedia_u32 x_ratio; //范围为[0，1000]; x_ratio = x * 1000 / W, x是局部放大的起始位置，W是通道帧的宽度
    xmedia_u32 y_ratio; //范围为[0，1000]; y_ratio = y * 1000 / H, y是局部放大的起始位置，H是通道帧的高度
    xmedia_u32 w_ratio; //范围为[0，1000]; w_ratio = x * 1000 / W, w是局部放大的宽度，W是通道帧的宽度
    xmedia_u32 h_ratio; //范围为[0，1000]; h_ratio = h * 1000 / H, h是局部放大的高度，H是通道帧的高度
} xmedia_vo_zoom_ratio;

typedef struct {
    xmedia_vo_zoom_in_type zoom_type;    //选择局部放大类型
    union {
        xmedia_video_rect zoom_rect;     //按窗口局部放大
        xmedia_vo_zoom_ratio zoom_ratio; //按比例局部放大
    };
} xmedia_vo_zoom_attr;

//定义边框属性
typedef struct{
    xmedia_u32 width;
    xmedia_u32 color;
}xmedia_vo_border;

typedef struct {
    xmedia_bool border_en;
    xmedia_vo_border border;
} xmedia_vo_border_attr;

//定义镜像属性
typedef struct {
    xmedia_bool mirror_en;
    xmedia_bool flip_en;
} xmedia_vo_mirror_attr;


/*
 * 函数功能: 初始化vo设备
 * 函数参数：无
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_OPEN_FAILED - 打开设备节点失败
 */
xmedia_s32 xmedia_vo_init(xmedia_void);

/*
 * 函数功能: 去初始化vo设备
 * 函数参数：无
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_OPEN_FAILED - 关闭设备节点失败
 */
xmedia_s32 xmedia_vo_exit(xmedia_void);

/*
 * 函数功能: 设置设备属性
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 *     dev_config - 输出参数, 设备属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID- 无效的设备ID
 *     XMEDIA_ERRCODE_NULL_PTR      - 参数空指针
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 设备已被使能无法设置参数
 *     XMEDIA_ERRCODE_INVALID_PARAM - 同步时序参数错误
 *     XMEDIA_ERRCODE_NOT_SUPPORT   - 设备接口复用异常用 或者 接口类型参数检查错误
 * 注意：
 *     必须在设备未使能之前调用
 */
xmedia_s32 xmedia_vo_set_dev_config(xmedia_vo_dev dev, const xmedia_vo_dev_config *dev_config);

/*
 * 函数功能: 获取设备属性
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NULL_PTR      - 参数空指针
 */
xmedia_s32 xmedia_vo_get_dev_config(xmedia_vo_dev dev, xmedia_vo_dev_config *dev_config);

/*
 * 函数功能: 设备使能
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NOT_READY      - 设备没有配置
 *     XMEDIA_ERRCODE_NOT_PERMITTED  - 设备已经使能，不允许重复使能
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - 设备crg时钟设置失败
 *     XMEDIA_ERRCODE_NOT_READY      - 中断使能或注册失败
 * 注意：
 *     必须在设备配置后使用
 */
xmedia_s32 xmedia_vo_enable_dev(xmedia_vo_dev dev);

/*
 * 函数功能: 设备禁止
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NOT_READY - 设备没有使能
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 视频层未禁止
 *     XMEDIA_ERRCODE_BUSY - 中断注销失败
 *     ERR_CODE_VO_WAIT_INTERRUPTED - 等待信号被中断
 * 注意：
 *     必须在视频层都关闭之后才能关闭设备
 */
xmedia_s32 xmedia_vo_disable_dev(xmedia_vo_dev dev);

/*
 * 函数功能: 设置设备的CSC信息
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 *     dev_csc - 输入参数, 设备颜色空间指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NULL_PTR      - 参数空指针
 *     XMEDIA_ERRCODE_INVALID_PARAM - 检查csc参数失败
 *     XMEDIA_ERRCODE_NOT_SUPPORT   - 检查csc参数或接口属性不合法
 */
xmedia_s32 xmedia_vo_set_dev_csc(xmedia_vo_dev dev, const xmedia_vo_dev_csc *dev_csc);

/*
 * 函数功能: 获取设备的CSC信息
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 *     dev_csc - 输出参数, 设备颜色空间指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_dev_csc(xmedia_vo_dev dev, xmedia_vo_dev_csc *dev_csc);

/*
 * 函数功能: 设置设备的GAMMA信息
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 *     dev_gamma - 输入参数, 设备GAMMA指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NULL_PTR      - 参数空指针
 *     XMEDIA_ERRCODE_NOT_SUPPORT   - 检查gamma参数或接口属性不合法
 */
xmedia_s32 xmedia_vo_set_dev_gamma(xmedia_vo_dev dev, const xmedia_vo_dev_gamma *dev_gamma);

/*
 * 函数功能: 获取设备的GAMMA信息
 * 函数参数：
 *     dev - 输入参数, 设备ID号
 *     dev_csc - 输出参数, 设备GAMMA指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_dev_gamma(xmedia_vo_dev dev, xmedia_vo_dev_gamma *dev_gamma);


/*
 * 函数功能: 设置视频层属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     layer_config - 输入参数, 视频层属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 视频层对应的设备ID无效
 *     XMEDIA_ERRCODE_NOT_READY      - 视频层对应的设备未使能
 *     XMEDIA_ERRCODE_NOT_PERMITTED  - 视频层已使能
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 配置参数不合法
 * 注意：
 *     必须在设备使能之后，视频层使能之前调用
 */
xmedia_s32 xmedia_vo_set_layer_config(xmedia_vo_layer layer, const xmedia_vo_layer_config *layer_config);

/*
 * 函数功能: 获取视频层属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     layer_config - 输出参数, 视频层属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_layer_config(xmedia_vo_layer layer, xmedia_vo_layer_config *layer_config);

/*
 * 函数功能: 视频层使能
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NOT_READY      - 设备未使能 或者 layer层参数未配置
 *     XMEDIA_ERRCODE_NOT_PERMITTED  - layer层已使能
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - buf list初始化失败 或者 vb申请失败
 * 注意：
 *     必须在设备使能 且 视频属性配置之后调用
 */
xmedia_s32 xmedia_vo_enable_layer(xmedia_vo_layer layer);

/*
 * 函数功能: 视频层禁止
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NOT_PERMITTED  - 层上还有通道未关闭
 *     XMEDIA_ERRCODE_BUSY           - 还有vgs任务未完成
 *     XMEDIA_ERRCODE_TIMEOUT        - 获取并发标志超时
 * 注意：
 *     必须在通道关闭下使用
 */
xmedia_s32 xmedia_vo_disable_layer(xmedia_vo_layer layer);

/*
 * 函数功能: 获取视频层帧
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     milli_sec - 输入参数, 等待时间
 *     frame_info - 输出参数, 帧信息结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 超时参数无效
 *     XMEDIA_ERRCODE_NOT_READY      - 层未使能
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - 多通道模式 或者 vgs bypass模式 当前模式不支持获取视频层帧
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - vb占用累加失败
 *     XMEDIA_ERRCODE_TIMEOUT        - 等待视频层帧超时
 *     XMEDIA_ERRCODE_BUSY           - 等待视频层帧出错
 * 注意：
 *     必须在视频层使能之后才能调用
 */
xmedia_s32 xmedia_vo_acquire_layer_frame(xmedia_vo_layer layer, xmedia_video_frame_info *frame_info,
    xmedia_s32 milli_sec);

/*
 * 函数功能: 释放视频层帧
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     frame_info - 输入参数, 帧信息结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_PERMITTED  - 未调用获取获取视频层帧接口
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - 模式不支持释放视频层帧
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 无效的帧
 *     XMEDIA_ERRCODE_BUSY           - vb帧计数累减失败
 * 注意：
 *     不允许释放重复帧
 */
xmedia_s32 xmedia_vo_release_layer_frame(xmedia_vo_layer layer, const xmedia_video_frame_info *frame_info);

/*
 * 函数功能: 设置视频层播放显容忍度
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     toleration - 输入参数 容忍度
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NOT_READY     -设备未使能
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 */
xmedia_s32 xmedia_vo_set_layer_toleration(xmedia_vo_layer layer, xmedia_u32 toleration);

/*
 * 函数功能: 获取视频播放层容忍度
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     toleration - 输出参数，保存容忍度指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 */
xmedia_s32 xmedia_vo_get_layer_toleration(xmedia_vo_layer layer, xmedia_u32 *toleration);

/*
 * 函数功能: 视频层上通道属性设置批处理开始
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 */
xmedia_s32 xmedia_vo_begin_batch_layer(xmedia_vo_layer layer);


/*
 * 函数功能: 视频层上通道属性设置批处理结束
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 */
xmedia_s32 xmedia_vo_end_batch_layer(xmedia_vo_layer layer);

/*
 * 函数功能: 设置通视频层转属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     rota - 输入参数, 旋转属性
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - MULTI或者disp_buf_len为0 场景则不支持
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 */
xmedia_s32 xmedia_vo_set_layer_rotation(xmedia_vo_layer layer, xmedia_video_rotation rota);

/*
 * 函数功能: 获取视频层旋转属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     rota - 输出参数, 旋转属性结构体指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_layer_rotation(xmedia_vo_layer layer, xmedia_video_rotation *rota);

/*
 * 函数功能: 设置通道属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道号
 *     chn_attr - 输入参数, 通道属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY      - 视频层未使能
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数检查不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - 通道已使能情况下 多通道模式 不允许动态改变通道输出size
 * 注意：
 *     必须在视频层配置之后调用
 */
xmedia_s32 xmedia_vo_set_chn_attr(xmedia_vo_layer layer, xmedia_vo_chn chn, const xmedia_vo_chn_attr *chn_attr);

/*
 * 函数功能: 获取通道属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     chn_attr - 输出参数, 通道属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 */
xmedia_s32 xmedia_vo_get_chn_attr(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_vo_chn_attr *chn_attr);

/*
 * 函数功能: 通道使能
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY      - 视频层未使能 或者 通道未配置
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 通道队列初始化失败
 * 注意：
 *    必须在视频层使能和通道属性配置后调用
 */
xmedia_s32 xmedia_vo_enable_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);

/*
 * 函数功能: 通道禁止
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_BUSY           - 去初始化队列 或者 清vb失败
 */
xmedia_s32 xmedia_vo_disable_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);

/*
 * 函数功能: 通道显示
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY        - 视频层未使能
 * 注意：
 *     必须在视频层使能之后调用
 */
xmedia_s32 xmedia_vo_show_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);

/*
 * 函数功能: 通道隐藏
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 */
xmedia_s32 xmedia_vo_hide_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);

/*
 * 函数功能: 设置通道帧率
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     frame_rate N*X[-64*X,+64*X] X为dev帧率 N为[-64,+64]任意整数
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY - 通道没有配置
 *     XMEDIA_ERRCODE_INVALID_PARAM - 帧率不合法
 * 注意：
 *     必须在通道配置之后调用
 */
xmedia_s32 xmedia_vo_set_chn_frame_rate(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_s32 frame_rate);

/*
 * 函数功能: 获取通道帧率
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     frame_rate 获取用户设置的通道帧率
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_chn_frame_rate(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_s32 *frame_rate);

/*
 * 函数功能: 获取通道帧
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     milli_sec - 输入参数, 等待时间
 *     frame_info - 输出参数, 帧信息结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_READY      - 通道未使能
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 超时参数无效
 *     XMEDIA_ERRCODE_TIMEOUT        - 并发标识获取失败 或者 等待通道帧超时
 *     XMEDIA_ERRCODE_BUSY           - 等待通道帧失败
 * 注意：
 *     必须在通道使能之后调用
 */
xmedia_s32 xmedia_vo_acquire_chn_frame(xmedia_vo_layer layer, xmedia_vo_chn chn,
    xmedia_video_frame_info *frame_info, xmedia_s32 milli_sec);

/*
 * 函数功能: 释放通道帧
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     frame_info - 输入参数, 帧信息结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_PERMITTED  - 未调用获取通道帧接口
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 无效的帧
 *     XMEDIA_ERRCODE_BUSY           - vb帧计数失败
 * 注意：
 *     不允许释放重复帧
 */
xmedia_s32 xmedia_vo_release_chn_frame(xmedia_vo_layer layer, xmedia_vo_chn chn,
    const xmedia_video_frame_info *frame_info);

/*
 * 函数功能: 发送帧给通道
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     frame_info - 输入参数, 帧信息结构指针
 *     milli_sec - 输入参数, 等待时间
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY      - 通道未使能
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 送下来的帧无效 或者 超时参数无效
 *     XMEDIA_ERRCODE_TIMEOUT        - 等待送帧水线正常超时
 *     XMEDIA_ERRCODE_BUSY           - 等待送帧水线出错 或者 vb计数失败
 *     XMEDIA_FAILURE                - 送帧入队失败
 * 注意：
 *     必须在通道使能之后才能调用
 */
xmedia_s32 xmedia_vo_send_chn_frame(xmedia_vo_layer layer, xmedia_vo_chn chn,
    xmedia_video_frame_info *frame_info, xmedia_s32 milli_sec);

/*
 * 函数功能: 清除通道缓存
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     is_clear_all - 输入参数, 是否清除通道所有的buf
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 *     XMEDIA_ERRCODE_NOT_READY      - 通道未使能
 *     XMEDIA_ERRCODE_BUSY           - 还有vgs任务未完成
 *     XMEDIA_ERRCODE_TIMEOUT        - 获取并发标识失败
 *     XMEDIA_ERRCODE_BUSY           - 清buf list或者vb失败
 */
xmedia_s32 xmedia_vo_clear_chn_buf(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_bool is_clear_all);

/*
 * 函数功能: 设置通道局部放大属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     zoom_attr - 输入参数, 局部放大属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_READY      - 视频层未使能
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - MULTI或者disp_buf_len为0 场景则不支持
 * 注意：
 *     必须在视频层配置之后使用
 */
xmedia_s32 xmedia_vo_set_chn_zoom_in(xmedia_vo_layer layer, xmedia_vo_chn chn,
    const xmedia_vo_zoom_attr *zoom_attr);

/*
 * 函数功能: 获取通道局部放大属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     zoom_attr - 输出参数, 局部放大属性结构指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_chn_zoom_in(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_vo_zoom_attr *zoom_attr);

/*
 * 函数功能: 设置通道边框属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     border_attr - 输入参数, 边框属性结构体指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - MULTI或者disp_buf_len为0 场景则不支持
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 */
xmedia_s32 xmedia_vo_set_chn_border(xmedia_vo_layer layer, xmedia_vo_chn chn,
    const xmedia_vo_border_attr *border_attr);

/*
 * 函数功能: 获取通道边框属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     border_attr - 输出参数, 边框属性结构体指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_chn_border(xmedia_vo_layer layer, xmedia_vo_chn chn,
    xmedia_vo_border_attr *border_attr);

/*
 * 函数功能: 设置通道旋转属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     rota - 输入参数, 旋转属性
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - MULTI或者disp_buf_len为0 场景则不支持
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 */
xmedia_s32 xmedia_vo_set_chn_rotation(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_video_rotation rota);

/*
 * 函数功能: 获取通道旋转属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     rota - 输出参数, 旋转属性结构体指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_chn_rotation(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_video_rotation *rota);

/*
 * 函数功能: 设置通道镜像属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     mirror_attr - 输入参数, 镜像属性
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT    - MULTI或者disp_buf_len为0 场景则不支持
 */
xmedia_s32 xmedia_vo_set_chn_mirror(xmedia_vo_layer layer, xmedia_vo_chn chn,
    const xmedia_vo_mirror_attr *mirror_attr);

/*
 * 函数功能: 获取通道镜像属性
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     mirror_attr - 输出参数, 镜像属性结构体指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_chn_mirror(xmedia_vo_layer layer, xmedia_vo_chn chn,
    xmedia_vo_mirror_attr *mirror_attr);

/*
 * 函数功能: 设置通道低延时模式
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     low_delay_mode - 输入参数, 低延时模式
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_INVALID_PARAM  - 参数不合法
 */
xmedia_s32 xmedia_vo_set_chn_low_delay(xmedia_vo_layer layer, xmedia_vo_chn chn,
    xmedia_vo_low_delay_mode low_delay_mode);

/*
 * 函数功能: 获取通道低延时模式
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     low_delay_mode - 输出参数, 低延时模式结构体指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NULL_PTR       - 参数空指针
 */
xmedia_s32 xmedia_vo_get_chn_low_delay(xmedia_vo_layer layer, xmedia_vo_chn chn,
    xmedia_vo_low_delay_mode *low_delay_mode);

/*
 * 函数功能: 暂停通道
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY        - 通道未使能
 */
xmedia_s32 xmedia_vo_pause_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);

/*
 * 函数功能: 唤醒通道
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY        - 通道未使能
 */
xmedia_s32 xmedia_vo_resume_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);


/*
 * 函数功能: 步进通道
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY        - 通道未使能
 */
xmedia_s32 xmedia_vo_step_chn(xmedia_vo_layer layer, xmedia_vo_chn chn);

/*
 * 函数功能: 设置通道显示缓存门限
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     threshold - 输入参数 显示门限
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY        - 视频层未使能
 */
xmedia_s32 xmedia_vo_set_chn_threshold(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_u32 threshold);


/*
 * 函数功能: 获取通道显示缓存门限
 * 函数参数：
 *     layer - 输入参数, 视频层ID号
 *     chn - 输入参数, 通道ID号
 *     threshold - 输出参数，保存显示门限的指针
 * 返回值：
 *     XMEDIA_SUCCESS - 成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的视频层ID
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道ID无效
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建通道失败
 *     XMEDIA_ERRCODE_NOT_READY        - 视频层未使能

 */
xmedia_s32 xmedia_vo_get_chn_threshold(xmedia_vo_layer layer, xmedia_vo_chn chn, xmedia_u32 *threshold);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
