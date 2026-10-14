/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef XMEDIA_VIDEO_COMMON_H
#define XMEDIA_VIDEO_COMMON_H

#include "common.h"
#include "xmedia_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_VIDEO_ISP_WDR_EXP_RATIO_MAX_NUM 2
#define XMEDIA_VIDEO_PMF_COEF_NUM              9
#define XMEDIA_VIDEO_LDCV2_CALIBRATION_COEF_NUM  3

typedef enum {
    XMEDIA_VIDEO_OPERATION_MODE_AUTO = 0,
    XMEDIA_VIDEO_OPERATION_MODE_MANUAL,
    XMEDIA_VIDEO_OPERATION_MODE_MAX
} xmedia_video_operation_mode;

typedef enum {
    XMEDIA_VIDEO_ROTATION_0 = 0x0,
    XMEDIA_VIDEO_ROTATION_90,
    XMEDIA_VIDEO_ROTATION_180,
    XMEDIA_VIDEO_ROTATION_270,
    XMEDIA_VIDEO_ROTATION_MAX
} xmedia_video_rotation;

typedef struct {
    xmedia_s32 x;
    xmedia_s32 y;
} xmedia_video_point;

typedef struct {
    xmedia_u32 width;
    xmedia_u32 height;
} xmedia_video_size;

typedef struct {
    xmedia_s32 x;
    xmedia_s32 y;
    xmedia_u32 width;
    xmedia_u32 height;
} xmedia_video_rect;

typedef enum {
    XMEDIA_VIDEO_COORD_MODE_ABS = 0,  // 绝对坐标
    XMEDIA_VIDEO_COORD_MODE_RATIO,    // 相对坐标
    XMEDIA_VIDEO_COORD_MODE_MAX
} xmedia_video_coord_mode;

typedef struct {
    xmedia_bool enable;
    xmedia_video_coord_mode coord_mode;
    xmedia_video_rect rect;
} xmedia_video_crop_attr;

typedef struct {
    xmedia_s32 src_frm_rate;
    xmedia_s32 dst_frm_rate;
} xmedia_video_frame_rate_ctrl;

typedef struct {
    xmedia_bool enable;
    xmedia_u32 buf_line; // wrap所需行数。用于wrap场景计算，一般buf_line = height/2
    xmedia_u32 buf_size; // wrap buf大小
} xmedia_video_wrap_attr;

typedef enum {
    XMEDIA_VIDEO_WDR_MODE_NONE = 0x0,
    XMEDIA_VIDEO_WDR_MODE_BUILT_IN,
    XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME,
    XMEDIA_VIDEO_WDR_MODE_3TO1_LINE,
    XMEDIA_VIDEO_WDR_MODE_3TO1_FRAME,
    XMEDIA_VIDEO_WDR_MODE_4TO1_LINE,
    XMEDIA_VIDEO_WDR_MODE_4TO1_FRAME,
    XMEDIA_VIDEO_WDR_MODE_MAX,
} xmedia_video_wdr_mode;

typedef enum {
    XMEDIA_VIDEO_WDR_FMT_NONE = 0x0,
    XMEDIA_VIDEO_WDR_FMT_VC,
    XMEDIA_VIDEO_WDR_FMT_DOL,
    XMEDIA_VIDEO_WDR_FMT_BUILTIN,
    XMEDIA_VIDEO_WDR_FMT_MAX,
} xmedia_video_wdr_format;

typedef enum {
    XMEDIA_VIDEO_BAYER_FMT_RGGB = 0,
    XMEDIA_VIDEO_BAYER_FMT_GRBG,
    XMEDIA_VIDEO_BAYER_FMT_GBRG,
    XMEDIA_VIDEO_BAYER_FMT_BGGR,

    XMEDIA_VIDEO_BAYER_FMT_GRGBI,
    XMEDIA_VIDEO_BAYER_FMT_RGBGI,
    XMEDIA_VIDEO_BAYER_FMT_GBGRI,
    XMEDIA_VIDEO_BAYER_FMT_BGRGI,
    XMEDIA_VIDEO_BAYER_FMT_IGRGB,
    XMEDIA_VIDEO_BAYER_FMT_IRGBG,
    XMEDIA_VIDEO_BAYER_FMT_IBGRG,
    XMEDIA_VIDEO_BAYER_FMT_IGBGR,

    XMEDIA_VIDEO_BAYER_FMT_RCCB,
    XMEDIA_VIDEO_BAYER_FMT_RCCG,
    XMEDIA_VIDEO_BAYER_FMT_RCCC,
    XMEDIA_VIDEO_BAYER_FMT_CCCC,

    XMEDIA_VIDEO_BAYER_FMT_RYYC,
    XMEDIA_VIDEO_BAYER_FMT_RYYB,

    XMEDIA_VIDEO_BAYER_FMT_RGBIR_2X2,
    XMEDIA_VIDEO_BAYER_FMT_RGBIR_4X4,

    XMEDIA_VIDEO_BAYER_MAX
} xmedia_video_bayer_format;

typedef enum {
    /* Semi-Planner */
    XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420 = 0,
    XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420,
    XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_422,
    XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_422,
    XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_444,
    XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_444,

    /* Package */
    XMEDIA_VIDEO_PIXEL_FMT_YUYV_PACKAGE_422 = 0x40,
    XMEDIA_VIDEO_PIXEL_FMT_YVYU_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_UYVY_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_VYUY_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_YYUV_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_YYVU_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_UVYY_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_VUYY_PACKAGE_422,
    XMEDIA_VIDEO_PIXEL_FMT_VY1UY0_PACKAGE_422,

    /*yuv 400 */
    XMEDIA_VIDEO_PIXEL_FMT_YUV_400 = 0x90,

    /*RGB*/
    XMEDIA_VIDEO_PIXEL_FMT_ARGB_1555 = 0xC0,
    XMEDIA_VIDEO_PIXEL_FMT_ARGB_4444,
    XMEDIA_VIDEO_PIXEL_FMT_ARGB_8888,
    XMEDIA_VIDEO_PIXEL_FMT_ABGR_1555,
    XMEDIA_VIDEO_PIXEL_FMT_ABGR_4444,
    XMEDIA_VIDEO_PIXEL_FMT_ABGR_8888,
    XMEDIA_VIDEO_PIXEL_FMT_RGBA_1555,
    XMEDIA_VIDEO_PIXEL_FMT_RGBA_4444,
    XMEDIA_VIDEO_PIXEL_FMT_RGBA_8888,
    XMEDIA_VIDEO_PIXEL_FMT_BGRA_1555,
    XMEDIA_VIDEO_PIXEL_FMT_BGRA_4444,
    XMEDIA_VIDEO_PIXEL_FMT_BGRA_8888,

    XMEDIA_VIDEO_PIXEL_FMT_RGB_565,
    XMEDIA_VIDEO_PIXEL_FMT_RGB_888,
    XMEDIA_VIDEO_PIXEL_FMT_BGR_565,
    XMEDIA_VIDEO_PIXEL_FMT_BGR_888,

    /*CLUT*/
    XMEDIA_VIDEO_PIXEL_FMT_CLUT2 = 0x100,
    XMEDIA_VIDEO_PIXEL_FMT_CLUT4,
    XMEDIA_VIDEO_PIXEL_FMT_CLUT8,

    /*RAW*/
    XMEDIA_VIDEO_PIXEL_FMT_RAW = 0x110,

    XMEDIA_VIDEO_PIXEL_FMT_MAX = 0x120
} xmedia_video_pixel_format;

typedef enum {
    XMEDIA_VIDEO_FMT_LINEAR = 0x0,
    XMEDIA_VIDEO_FMT_TILE_16x4,
    XMEDIA_VIDEO_FMT_TILE_32x4,
    XMEDIA_VIDEO_FMT_TILE_64x4,
    XMEDIA_VIDEO_FMT_TILE_64x64,
    XMEDIA_VIDEO_FMT_MAX
} xmedia_video_format;

/*维度：
 *yuv、RGB、RAW
 *有损/无损,
 *行、帧、段紧凑、段非紧凑
 */
typedef enum {
    XMEDIA_VIDEO_COMPRESS_MODE_NONE = 0x0,
    XMEDIA_VIDEO_COMPRESS_MODE_LINE, // 3DNR 使用行压缩
    XMEDIA_VIDEO_COMPRESS_MODE_FRAME,
    XMEDIA_VIDEO_COMPRESS_MODE_SEG,
    XMEDIA_VIDEO_COMPRESS_MODE_TILE, // tile非紧凑压缩
    XMEDIA_VIDEO_COMPRESS_MODE_SLICE,
    XMEDIA_VIDEO_COMPRESS_MODE_TILE_COMPACT, // tile紧凑压缩

    XMEDIA_VIDEO_COMPRESS_MODE_MAX
} xmedia_video_compress_mode;

typedef enum {
    XMEDIA_VIDEO_COLOR_SPACE_YUV = 0x0,
    XMEDIA_VIDEO_COLOR_SPACE_RGB,
    XMEDIA_VIDEO_COLOR_SPACE_MAX
} xmedia_video_color_space;

typedef enum {
    XMEDIA_VIDEO_COLOR_GAMUT_BT601 = 0x0,
    XMEDIA_VIDEO_COLOR_GAMUT_BT709,
    XMEDIA_VIDEO_COLOR_GAMUT_BT2020,
    XMEDIA_VIDEO_COLOR_GAMUT_USER,
    XMEDIA_VIDEO_COLOR_GAMUT_MAX
} xmedia_video_color_gamut;

typedef enum {
    XMEDIA_VIDEO_COLOR_LIMITED_RANGE = 0x0,
    XMEDIA_VIDEO_COLOR_FULL_RANGE,
    XMEDIA_VIDEO_COLOR_RANGE_MAX
} xmedia_video_color_quantify_range;

typedef struct {
    xmedia_video_color_space color_space;
    xmedia_video_color_gamut color_gamut;
    xmedia_video_color_quantify_range quantify_range;
} xmedia_video_color_descript;

typedef enum {
    XMEDIA_VIDEO_DYNAMIC_RANGE_SDR = 0x0,
    XMEDIA_VIDEO_DYNAMIC_RANGE_HDR10,
    XMEDIA_VIDEO_DYNAMIC_RANGE_HLG,
    XMEDIA_VIDEO_DYNAMIC_RANGE_MAX
} xmedia_video_dynamic_range;

typedef enum {
    XMEDIA_VIDEO_DATA_WIDTH_8 = 0x0,
    XMEDIA_VIDEO_DATA_WIDTH_10,
    XMEDIA_VIDEO_DATA_WIDTH_12,
    XMEDIA_VIDEO_DATA_WIDTH_14,
    XMEDIA_VIDEO_DATA_WIDTH_16,
    XMEDIA_VIDEO_DATA_WIDTH_MAX
} xmedia_video_data_width;

typedef struct {
    xmedia_u32 iso;
    xmedia_u32 exposure_time;
    xmedia_u32 exposure_bias_value; // 曝光补偿
    xmedia_u8 exposure_program;
    xmedia_u8 exposure_mode; // 曝光模式：0-自动曝光，1-手动曝光
    xmedia_u32 exposure_ratio[VI_MAX_WDR_NUM - 1];
    xmedia_u32 isp_dgain;
    xmedia_u32 isp_nr_strength;
    xmedia_u32 sensor_id;
    xmedia_u32 sensor_mode;
    xmedia_u32 sensor_hmax;
    xmedia_u32 sensor_vmax;
    xmedia_u32 sensor_again;
    xmedia_u32 sensor_dgain;
    xmedia_u32 f_number;
    xmedia_u32 max_aperture_value; // 最大光圈值
    xmedia_u8 white_balance_mode; // 白平衡模式：0-自动白平衡，1-手动白平衡
    xmedia_u32 vc_num;
    xmedia_video_bayer_format bayer_format;
    xmedia_video_color_gamut color_gamut;
    xmedia_video_color_quantify_range color_quantify_range;
    xmedia_u64 private_data;
} xmedia_video_isp_frame_info;

typedef struct {
    xmedia_u16 top_offset;
    xmedia_u16 left_offset;
    xmedia_u16 bottom_offset;
    xmedia_u16 right_offset;
    xmedia_u32 max_luma;
    xmedia_u32 min_luma;
    xmedia_u64 private_data;
} xmedia_video_vdec_frame_info;

typedef struct {
    xmedia_u64 isp_info_phy_addr;
    xmedia_u64 lowdelay_info_phy_addr;
    xmedia_u64 vdec_info_phy_addr;

    xmedia_void *ATTRIBUTE isp_info_vir_addr;
    xmedia_void *ATTRIBUTE lowdelay_info_vir_addr;
    xmedia_void *ATTRIBUTE vdec_info_vir_addr;
} xmedia_video_private_info;

typedef struct {
    xmedia_u64 y_head_phy_addr;
    xmedia_u64 c_head_phy_addr;
    xmedia_u64 y_phy_addr;
    xmedia_u64 c_phy_addr;
    // xmedia_u64 y_buf_phy_addr; // luma buffer start phy addr
    // xmedia_u64 c_buf_phy_addr; // chroma buffer start phy addr
    // xmedia_u32 y_size; // luma buffer size
    // xmedia_u32 c_size; // chroma buffer size
} xmedia_video_frame_address;

typedef struct {
    xmedia_u32 y_head_stride;
    xmedia_u32 c_head_stride;
    xmedia_u32 y_stride;
    xmedia_u32 c_stride;
} xmedia_video_frame_stride;

typedef struct {
    xmedia_u32 index;

    xmedia_u32 width;
    xmedia_u32 height;
    xmedia_video_frame_address addr;
    xmedia_video_frame_stride stride;

    xmedia_video_data_width bit_width;
    xmedia_video_pixel_format pixel_fmt;
    xmedia_video_compress_mode compress_mode;
    xmedia_video_format video_fmt;

    xmedia_video_dynamic_range dynamic_range;
    xmedia_video_color_descript color_info;

    xmedia_u64 pts;
    xmedia_u32 time_ref;
    xmedia_u32 frame_flag;
    xmedia_video_private_info priv_info;
} xmedia_video_frame;

typedef struct {
    xmedia_video_frame frame;
    xmedia_u32 pool_id;
    xmedia_mod_id mod_id;
} xmedia_video_frame_info;

typedef enum {
    XMEDIA_VIDEO_LDC_V1 = 0,
    XMEDIA_VIDEO_LDC_V2,
    XMEDIA_VIDEO_LDC_MAX,
} xmedia_video_ldc_version;

typedef struct {
    xmedia_bool aspect; // 视野调整过程中是否保持幅型比
    xmedia_s32 x_ratio; // [0,100]X方向视野大小参数，aspect = 0时有效
    xmedia_s32 y_ratio; // [0,100]Y方向视野大小参数，aspect = 0时有效
    xmedia_s32 xy_ratio; // [0,100]视野大小参数，aspect = 1时有效
    xmedia_s32 center_xoffset; // [-500,500]图像中心点相对于物理中心点的水平偏移
    xmedia_s32 center_yoffset; // [-500,500]图像中心点相对于物理中心点的垂直偏移
    xmedia_s32 distortion_ratio; // [-300,500]校正强度，负数为枕型，正数为桶型
} xmedia_video_ldcv1_attr;

typedef struct {
    xmedia_s32 focal_len_x; // 水平方向镜头有效焦距
    xmedia_s32 focal_len_y; // 垂直方向镜头有效焦距
    xmedia_s32 coor_shift_x; // 光心X坐标
    xmedia_s32 coor_shift_y; // 光心Y坐标
    xmedia_s32 src_calibration_ratio[XMEDIA_VIDEO_LDCV2_CALIBRATION_COEF_NUM]; // 镜头畸变系数
} xmedia_video_ldcv2_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_video_ldc_version version;
    union {
        xmedia_video_ldcv1_attr ldcv1_attr;
        xmedia_video_ldcv2_attr ldcv2_attr;
    };
} xmedia_video_ldc_attr;

typedef enum {
    XMEDIA_VIDEO_STITCH_MODE_PLANE_PROJECTION = 0,      // 平面投影
    XMEDIA_VIDEO_STITCH_MODE_CYLINDER_PROJECTION = 1,   // 柱面投影
    XMEDIA_VIDEO_STITCH_MODE_MAX
} xmedia_video_stitch_mode;

typedef enum {
    XMEDIA_VIDEO_MCF_HIST_ADJ_MODE_NON = 0,             // 不做调整
    XMEDIA_VIDEO_MCF_HIST_ADJ_MODE_COLOR,               // 对彩色路图像做直方图调整
    XMEDIA_VIDEO_MCF_HIST_ADJ_MODE_MONO,                // 对黑白路图像做直方图调整
    XMEDIA_VIDEO_MCF_HIST_ADJ_MODE_MAX,
} xmedia_video_mcf_hist_adj_mode;

typedef enum {
    XMEDIA_VIDEO_VB_SOURCE_COMMON = 0,
    XMEDIA_VIDEO_VB_SOURCE_PRIVATE,
    XMEDIA_VIDEO_VB_SOURCE_USER,
    XMEDIA_VIDEO_VB_SOURCE_MAX
} xmedia_video_vb_source;

typedef struct {
    xmedia_bool enable;             // 拼接亮度同步使能
    xmedia_s32  vi_grp;             // 拼接业务对应的vi_grp号
    xmedia_u32  frame_interval;     // 拼接亮度同步时间间隔
    xmedia_u32  smooth_str;         // 垂直滤波窗口大小。取值范围[1,16]。该值越大，亮度矫正表垂直滤波窗口越大，过渡越平滑。
    xmedia_u32  shading_adjust_thr; // 增益可调节限制范围。取值范围[0,255]。
} xmedia_video_stitch_luma_sync_attr;

typedef enum {
    XMEDIA_VIDEO_SCALE_MODE_NORMAL = 0,
    XMEDIA_VIDEO_SCALE_MODE_BILINEAR,            // 双线性插值
    XMEDIA_VIDEO_SCALE_MODE_MAX
} xmedia_video_scale_mode;

#ifdef __cplusplus
}
#endif

#endif
