/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_VPSS_H__
#define __XMEDIA_VPSS_H__

#include "xmedia_errcode.h"
#include "xmedia_sys.h"
#include "xmedia_isp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define XMEDIA_VPSS_ICHN0  0
#define XMEDIA_VPSS_ICHN1  1

#define XMEDIA_VPSS_OCHN0  0
#define XMEDIA_VPSS_OCHN1  1
#define XMEDIA_VPSS_OCHN2  2
#define XMEDIA_VPSS_OCHN3  3

#define XMEDIA_VPSS_OVERLAP_POINT_TOP_LEFT     0
#define XMEDIA_VPSS_OVERLAP_POINT_TOP_RIGHT    1
#define XMEDIA_VPSS_OVERLAP_POINT_BOTTOM_LEFT  2
#define XMEDIA_VPSS_OVERLAP_POINT_BOTTOM_RIGHT 3
#define XMEDIA_VPSS_OVERLAP_POINT_NUM          4

#define XMEDIA_VPSS_INVALID_CHN  (-1)

#define XMEDIA_VPSS_MAX_SELECT_OCHN_NUM  (VPSS_MAX_PIPE_NUM * VPSS_MAX_OCHN_NUM)

#define XMEDIA_VPSS_MAX_PIPE_NUM_IN_GRP  2 // 拼接组最大的pipe个数

typedef enum {
    XMEDIA_VPSS_STITCH_BLEND_MODE_LINEAR = 0, //直线模式
    XMEDIA_VPSS_STITCH_BLEND_MODE_CURVE,      //曲线模式
    XMEDIA_VPSS_STITCH_BLEND_MAX
} xmedia_vpss_stitch_blend_mode;

typedef struct {
    xmedia_bool  stitch_blend_en; //拼接使能开关
    xmedia_bool  sync_en;         //同步使能开关
    xmedia_u32   stitch_num;      //拼接路数
    xmedia_vpss_stitch_blend_mode mode;//拼接融合模式
    /*
     * 拼接后大小：
     * 宽度范围：[VPSS_MIN_STITCH_OUT_WIDTH, VPSS_MAX_STITCH_OUT_WIDTH]
     * 高度范围：[VPSS_MIN_STITCH_OUT_HEIGHT, VPSS_MAX_STITCH_OUT_HEIGHT]
     */
    xmedia_video_size out_size;
    //描述拼接右图和左图重叠区域的点, 四个的顺序为上左、上右、下左、下右.
    xmedia_video_point overlap_point[VPSS_MAX_STITCH_NUM - 1][XMEDIA_VPSS_OVERLAP_POINT_NUM];
} xmedia_vpss_stitch_blend_config;

typedef struct {
    /*
     * 拼接后大小：
     * 宽度范围：[VPSS_MIN_STITCH_OUT_WIDTH, VPSS_MAX_STITCH_OUT_WIDTH]
     * 高度范围：[VPSS_MIN_STITCH_OUT_HEIGHT, VPSS_MAX_STITCH_OUT_HEIGHT]
     */
    xmedia_video_size out_size;
    //描述拼接右图和左图重叠区域的点, 四个的顺序为上左、上右、下左、下右.
    xmedia_video_point overlap_point[VPSS_MAX_STITCH_NUM - 1][XMEDIA_VPSS_OVERLAP_POINT_NUM];
} xmedia_vpss_stitch_size_attr;

typedef struct {
    /*
     * 融合区域占重叠区域的百分比。
     * 取值范围：[0, 100]。0表示不融合，100表示重叠区域全部融合。默认值为10
     */
    xmedia_u32 fusion_percent[VPSS_MAX_STITCH_NUM - 1];
    /*
     * 融合区域中线位置百分比。
     * 取值范围：[0, 100]。0表示在重合区的最左边，100表示重合区的最右边。默认值为50
     */
    xmedia_u32 fusion_mid_pos_percent[VPSS_MAX_STITCH_NUM - 1];
} xmedia_vpss_stitch_blend_attr;

typedef struct {
    xmedia_bool                nr_en;         //3DNR使能开关
    xmedia_video_compress_mode compress_mode; //参考帧压缩模式
} xmedia_vpss_nr_attr;

typedef struct {
    xmedia_u32                      max_w;            //输入图像宽度
    xmedia_u32                      max_h;            //输入图像高度
    xmedia_video_pixel_format       pixel_fmt;        //输入图像像素格式
    xmedia_video_dynamic_range      dync_range;       //输入图像动态范围
    xmedia_video_frame_rate_ctrl    frm_rate_ctrl;    //输入帧率控制
    xmedia_vpss_nr_attr             nr_attr;          //3DNR属性
    xmedia_vpss_stitch_blend_config stitch_blend_cfg; //拼接融合属性
} xmedia_vpss_pipe_config;

typedef enum {
    XMEDIA_VPSS_OCHN_MODE_USER = 0,            // 用户模式
    XMEDIA_VPSS_OCHN_MODE_AUTO,                // 自动模式
    XMEDIA_VPSS_OCHN_MODE_MAX
} xmedia_vpss_ochn_mode;

typedef struct {
    xmedia_vpss_ochn_mode        mode;         //通道工作模式
    xmedia_video_scale_mode      scale_mode;   //缩放模式
    xmedia_u32                   width;        //目标图像的宽度.
    xmedia_u32                   height;       //目标图像的高度.
    xmedia_video_format          video_fmt;    //目标图像的视频格式.
    xmedia_video_pixel_format    pix_fmt;      //目标图像的像素格式.
    xmedia_video_dynamic_range   dync_range;   //目标图像的动态范围.
    xmedia_video_compress_mode   cmp_mode;     //输出的压缩模式
    xmedia_video_frame_rate_ctrl frm_rate_ctrl;//帧率控制信息.
    xmedia_bool                  mirror_en;    //镜像使能.
    xmedia_bool                  flip_en;      //翻转使能.
    xmedia_u32                   depth;        //用户获取通道图像的队列长度.
} xmedia_vpss_ochn_config;

typedef struct {
    xmedia_vpss_ochn_mode        mode;         //通道工作模式
    xmedia_video_scale_mode      scale_mode;   //缩放模式
    xmedia_u32                   width;        //目标图像的宽度.
    xmedia_u32                   height;       //目标图像的高度.
    xmedia_video_format          video_fmt;    //目标图像的视频格式.
    xmedia_video_pixel_format    pix_fmt;      //目标图像的像素格式.
    xmedia_video_dynamic_range   dync_range;   //目标图像的动态范围.
    xmedia_video_compress_mode   cmp_mode;     //输出的压缩模式
    xmedia_video_frame_rate_ctrl frm_rate_ctrl;//帧率控制信息.
    xmedia_bool                  mirror_en;    //镜像使能.
    xmedia_bool                  flip_en;      //翻转使能.
    xmedia_u32                   depth;        //用户获取通道图像的队列长度.
} xmedia_vpss_ochn_attr;

typedef struct {
    xmedia_s32                   bind_ochn;     //绑定的源物理通道.
    xmedia_video_scale_mode      scale_mode;    //缩放模式
    xmedia_u32                   width;         //目标图像的宽度.
    xmedia_u32                   height;        //目标图像的高度.
    xmedia_video_format          video_fmt;     //目标图像的视频格式.
    xmedia_video_pixel_format    pix_fmt;       //目标图像的像素格式.
    xmedia_video_dynamic_range   dync_range;    //目标图像的动态范围.
    xmedia_video_compress_mode   cmp_mode;      //输出的压缩模式.
    xmedia_u32                   depth;         //用户获取通道图像的队列长度.
    xmedia_video_frame_rate_ctrl frm_rate_ctrl; //帧率控制信息.
} xmedia_vpss_ext_ochn_config;

typedef struct {
    xmedia_s32                   bind_ochn;     //绑定的源物理通道.
    xmedia_video_scale_mode      scale_mode;    //缩放模式
    xmedia_u32                   width;         //目标图像的宽度.
    xmedia_u32                   height;        //目标图像的高度.
    xmedia_video_format          video_fmt;     //目标图像的视频格式.
    xmedia_video_pixel_format    pix_fmt;       //目标图像的像素格式.
    xmedia_video_dynamic_range   dync_range;    //目标图像的动态范围.
    xmedia_video_compress_mode   cmp_mode;      //输出的压缩模式.
    xmedia_u32                   depth;         //用户获取通道图像的队列长度.
    xmedia_video_frame_rate_ctrl frm_rate_ctrl; //帧率控制信息.
} xmedia_vpss_ext_ochn_attr;

typedef struct {
    xmedia_bool enable;
    xmedia_u32  line_cnt;   //低延时行号
    xmedia_bool one_buf_en; //是否使用one buffer
} xmedia_vpss_low_delay_attr;

typedef struct {
    xmedia_video_frame_info frame_info[VPSS_MAX_STITCH_NUM];    // 拼接图像
} xmedia_vpss_stitch_calibration_frame_info;

typedef struct {
    xmedia_video_stitch_mode mode;                          // 标定模式
    xmedia_u32 stitch_num;                                  // 拼接路数
    xmedia_video_point ajust_point[VPSS_MAX_STITCH_NUM - 1];    // 图像位置微调参数
} xmedia_vpss_stitch_calibration_config;

typedef struct {
    xmedia_video_size out_size[VPSS_MAX_STITCH_NUM];                                            // GDC输出图像大小
    xmedia_s64 pmf_coef[VPSS_MAX_STITCH_NUM][XMEDIA_VIDEO_PMF_COEF_NUM];                        // GDC矫正矩阵
    xmedia_video_point offset[VPSS_MAX_STITCH_NUM];                                             // GDC坐标偏移
    xmedia_video_point overlap_point[VPSS_MAX_STITCH_NUM - 1][XMEDIA_VPSS_OVERLAP_POINT_NUM];   // 拼接重叠区域大小
    xmedia_video_size stitch_out_size;                                                      // 拼接后输出大小
    xmedia_video_rect effective_rect;                                                       // 拼接后图像有效区域
} xmedia_vpss_stitch_calibration_result;

typedef struct {
    xmedia_u32 num;                          // 区域数量
    xmedia_video_rect *ATTRIBUTE rect;       // 区域坐标、宽高参数
} xmedia_vpss_rect_attr;

typedef enum {
    XMEDIA_VPSS_WRAP_POS_PHY_OCHN = 0, // 物理通道的卷绕
    XMEDIA_VPSS_WRAP_POS_EXT_OCHN,     // 扩展通道的卷绕
    XMEDIA_VPSS_WRAP_POS_MAX
} xmedia_vpss_wrap_pos;

typedef struct {
    xmedia_bool enable;            // 卷绕buf复用使能开关
    xmedia_u32 buf_size;           // 卷绕buf大小
    xmedia_vpss_wrap_pos wrap_pos; // 卷绕的位置
} xmedia_vpss_wrap_buf_share_config;

typedef enum {
    XMEDIA_VPSS_OUTPUT_MODE_INDEPENDENT = 0,  // 不做非融合拼接
    XMEDIA_VPSS_OUTPUT_MODE_H_SPLICING,       // 水平非融合拼接
    XMEDIA_VPSS_OUTPUT_MODE_V_SPLICING,       // 垂直非融合拼接
    XMEDIA_VPSS_OUTPUT_MODE_MAX
} xmedia_vpss_output_mode;

typedef struct {
    xmedia_u32 pipe_num;                                         // 非融合拼接的pipe数
    xmedia_s32 pipe_id[XMEDIA_VPSS_MAX_PIPE_NUM_IN_GRP];         // 非融合拼接的pipe id
    xmedia_s32 master_pipe_id;                                   // 非融合拼接的主pipe id
    xmedia_vpss_output_mode output_mode[VPSS_MAX_PHY_OCHN_NUM];  // 非融合拼接的通道拼接模式
} xmedia_vpss_splicing_grp_config;

/*
 * 函数功能: 初始化vpss模块
 * 函数参数：无
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 */
xmedia_s32 xmedia_vpss_init(xmedia_void);

/*
 * 函数功能: 去初始化vpss模块
 * 函数参数：无
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 */
xmedia_s32 xmedia_vpss_exit(xmedia_void);

/*
 * 函数功能: 创建pipe
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     pipe_cfg - 输入参数, pipe配置
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_EXIST - 重复创建pipe
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 */
xmedia_s32 xmedia_vpss_create_pipe(xmedia_s32 pipe, const xmedia_vpss_pipe_config *pipe_cfg);

/*
 * 函数功能: 销毁pipe
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_PERMITTED - pipe未禁用
 * 注意：
 *     必须在先禁用再销毁
 */
xmedia_s32 xmedia_vpss_destroy_pipe(xmedia_s32 pipe);

/*
 * 函数功能: 启用pipe
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_start_pipe(xmedia_s32 pipe);

/*
 * 函数功能: 停用pipe
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_stop_pipe(xmedia_s32 pipe);

/*
 * 函数功能: 复位pipe
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_reset_pipe(xmedia_s32 pipe);

/*
 *  函数功能：设置pipe帧率控制属性
 *  函数参数：
 *      pipe          - 输入参数，pipe id号
 *      frm_rate_ctrl - 输入参数，pipe的帧率控制属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 非法参数
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *  注意：
 *     1、pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_frc(xmedia_s32 pipe, const xmedia_video_frame_rate_ctrl *frm_rate_ctrl);

/*
 *  函数功能：获取pipe帧率控制属性
 *  函数参数：
 *      pipe          - 输入参数，pipe id号
 *      frm_rate_ctrl - 输出参数，pipe的帧率控制属性
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 非法参数
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *  注意：
 *     1、pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_frc(xmedia_s32 pipe, xmedia_video_frame_rate_ctrl *frm_rate_ctrl);

/*
 * 函数功能: 设置pipe裁剪属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     crop_attr - 输入参数, 裁剪属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_crop(xmedia_s32 pipe, const xmedia_video_crop_attr *crop_attr);

/*
 * 函数功能: 获取pipe裁剪属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     crop_attr - 输出参数, 裁剪属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_crop(xmedia_s32 pipe, xmedia_video_crop_attr *crop_attr);

/*
 * 函数功能: 设置pipe stnr属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     stnr_attr - 输入参数, stnr属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_stnr_attr(xmedia_s32 pipe, const xmedia_isp_stnr_attr *stnr_attr);

/*
 * 函数功能: 获取pipe stnr属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     stnr_attr - 输出参数, stnr属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_stnr_attr(xmedia_s32 pipe, xmedia_isp_stnr_attr *stnr_attr);

/*
 * 函数功能: 设置pipe sharpen属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     sharpen_attr - 输入参数, sharpen属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_sharpen_attr(xmedia_s32 pipe, const xmedia_isp_sharpen_attr *sharpen_attr);

/*
 * 函数功能: 获取pipe sharpen属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     sharpen_attr - 输出参数, sharpen属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_sharpen_attr(xmedia_s32 pipe, xmedia_isp_sharpen_attr *sharpen_attr);

/*
 * 函数功能: 设置pipe anti false color属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     afc_attr - 输入参数, afc 属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_anti_false_color_attr(xmedia_s32 pipe,
    const xmedia_isp_anti_false_color_attr *afc_attr);

/*
 * 函数功能: 获取pipe anti false color属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     afc_attr - 输出参数, afc属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_anti_false_color_attr(xmedia_s32 pipe, xmedia_isp_anti_false_color_attr *afc_attr);

/*
 * 函数功能: 设置pipe 融合拼接的重叠区属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     stitch_size_attr - 输入参数, 拼接重叠区属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_stitch_size_attr(xmedia_s32 pipe,
    const xmedia_vpss_stitch_size_attr *stitch_size_attr);

/*
 * 函数功能: 获取pipe 融合拼接的重叠区属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     stitch_size_attr - 输出参数, 拼接重叠区属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_stitch_size_attr(xmedia_s32 pipe, xmedia_vpss_stitch_size_attr *stitch_size_attr);

/*
 * 函数功能: 设置pipe 融合拼接的融合属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     stitch_blend_attr - 输入参数, 拼接融合属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_pipe_stitch_blend_attr(xmedia_s32 pipe,
    const xmedia_vpss_stitch_blend_attr *stitch_blend_attr);

/*
 * 函数功能: 获取pipe 融合拼接的融合属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     stitch_blend_attr - 输出参数, 拼接融合属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_pipe_stitch_blend_attr(xmedia_s32 pipe, xmedia_vpss_stitch_blend_attr *stitch_blend_attr);

/*
 * 函数功能: 设置融合拼接的亮度同步属性
 * 函数参数：
 *     pipe - 输入参数，pipe id号
 *     luma_sync_attr - 输入参数，亮度同步属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 */
xmedia_s32 xmedia_vpss_set_pipe_stitch_luma_sync_attr(xmedia_s32 pipe,
    const xmedia_video_stitch_luma_sync_attr *luma_sync_attr);

/*
 * 函数功能: 设置融合拼接的亮度同步属性
 * 函数参数：
 *     pipe - 输入参数，pipe id号
 *     luma_sync_attr - 输出参数，亮度同步属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 */
xmedia_s32 xmedia_vpss_get_pipe_stitch_luma_sync_attr(xmedia_s32 pipe,
    xmedia_video_stitch_luma_sync_attr *luma_sync_attr);

/*
 * 函数功能: 设置输入通道固定角度旋转属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ichn - 输入参数,     ichn id号
 *     rotation - 输入参数, 旋转属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ichn_rotation(xmedia_s32 pipe, xmedia_s32 ichn, xmedia_video_rotation rotation);

/*
 * 函数功能: 获取输入通道固定角度旋转属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ichn - 输入参数,     ichn id号
 *     rotation - 输出参数, 旋转属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ichn_rotation(xmedia_s32 pipe, xmedia_s32 ichn, xmedia_video_rotation *rotation);

/*
 * 函数功能: 发送图像到输入通道
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ichn - 输入参数,     ichn id号
 *     video_frame - 输入参数, 帧信息
 *     milli_sec - 输入参数, 超时参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_BUSY - pipe正在忙
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_send_ichn_frame(xmedia_s32 pipe, xmedia_s32 ichn,
    const xmedia_video_frame_info *video_frame, xmedia_s32 milli_sec);

/*
 * 函数功能: 获取pipe输入帧
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ichn - 输入参数,     ichn id号
 *     video_frame - 输出参数, 帧信息
 *     milli_sec - 输入参数, 超时参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - pipe队列为空
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_acquire_ichn_frame(xmedia_s32 pipe, xmedia_s32 ichn,
    xmedia_video_frame_info *video_frame, xmedia_s32 milli_sec);

/*
 * 函数功能: 释放pipe输入帧
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ichn - 输入参数,     ichn id号
 *     video_frame - 输入参数, 帧信息
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     此接口需与xmedia_vpss_acquire_ichn_frame配对使用
 */
xmedia_s32 xmedia_vpss_release_ichn_frame(xmedia_s32 pipe, xmedia_s32 ichn,
    const xmedia_video_frame_info *video_frame);

/*
 * 函数功能: 设置输出通道配置
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     ochn_cfg - 输入参数, 输出通道配置
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ochn_config(xmedia_s32 pipe, xmedia_s32 ochn, const xmedia_vpss_ochn_config *ochn_cfg);

/*
 * 函数功能: 设置输出通道属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     ochn_attr - 输入参数, 输出通道属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ochn_attr(xmedia_s32 pipe, xmedia_s32 ochn, const xmedia_vpss_ochn_attr *ochn_attr);

/*
 * 函数功能: 获取输出通道属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     ochn_attr - 输出参数, 输出通道属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ochn_attr(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_vpss_ochn_attr *ochn_attr);

/*
 * 函数功能: 开启输出通道
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_enable_ochn(xmedia_s32 pipe, xmedia_s32 ochn);

/*
 * 函数功能: 关闭输出通道
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_disable_ochn(xmedia_s32 pipe, xmedia_s32 ochn);

/*
 * 函数功能: 设置输出通道缩放前的裁剪属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     crop_info - 输入参数, 裁剪属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ochn_pre_crop(xmedia_s32 pipe, xmedia_s32 ochn, const xmedia_video_crop_attr *crop_info);

/*
 * 函数功能: 获取输出通道缩放前的裁剪属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     crop_info - 输出参数, 裁剪属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ochn_pre_crop(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_video_crop_attr *crop_info);

/*
 * 函数功能: 设置输出通道缩放后的裁剪属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     crop_info - 输入参数, 裁剪属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ochn_post_crop(xmedia_s32 pipe, xmedia_s32 ochn, const xmedia_video_crop_attr *crop_info);

/*
 * 函数功能: 获取输出通道缩放后的裁剪属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     crop_info - 输出参数, 裁剪属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ochn_post_crop(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_video_crop_attr *crop_info);

/*
 * 函数功能: 设置输出通道固定角度旋转属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     rotation - 输入参数, 旋转属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建，输出通道属性必须已设置
 */
xmedia_s32 xmedia_vpss_set_ochn_rotation(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_video_rotation rotation);

/*
 * 函数功能: 获取输出通道固定角度旋转属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     rotation - 输出参数, 旋转属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建，输出通道属性必须已设置
 */
xmedia_s32 xmedia_vpss_get_ochn_rotation(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_video_rotation *rotation);

/*
 * 函数功能: 获取输出通道图像
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     video_frame - 输出参数, 帧信息
 *     milli_sec - 输入参数, 超时参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 输出通道队列为空
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_acquire_ochn_frame(xmedia_s32 pipe, xmedia_s32 ochn,
    xmedia_video_frame_info *video_frame, xmedia_s32 milli_sec);

/*
 * 函数功能: 释放从输出通道获取的图像
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     video_frame - 输出参数, 帧信息
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 * 注意：
 *     此接口需与xmedia_vpss_acquire_ochn_frame配对使用
 */
xmedia_s32 xmedia_vpss_release_ochn_frame(xmedia_s32 pipe, xmedia_s32 ochn,
    const xmedia_video_frame_info *video_frame);

/*
 * 函数功能: 获取输出通道亮度和
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     rect_attr - 输入参数, 统计区域属性
 *     luma_data - 输出参数, 统计结果
 *     milli_sec - 输入参数, 超时参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 *     此接口是使用VPSS硬件处理，仅通道0支持
 */
xmedia_s32 xmedia_vpss_get_ochn_rgn_luma(xmedia_s32 pipe, xmedia_s32 ochn,
    const xmedia_vpss_rect_attr *rect_attr, xmedia_u64 *luma_data, xmedia_s32 milli_sec);

/*
 * 函数功能: 获取输出通道亮度和
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     rect_attr - 输入参数, 统计区域属性
 *     luma_data - 输出参数, 统计结果
 *     milli_sec - 输入参数, 超时参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 *     此接口是使用VGS硬件处理，通道0~3支持
 */
xmedia_s32 xmedia_vpss_get_ochn_rgn_luma_ex(xmedia_s32 pipe, xmedia_s32 ochn,
        const xmedia_vpss_rect_attr *rect_attr, xmedia_u64 *luma_data, xmedia_s32 milli_sec);

/*
 * 函数功能: 设置输出通道低延时属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     low_delay_info - 输入参数, 低延时属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 不支持低延时
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ochn_low_delay_attr(xmedia_s32 pipe, xmedia_s32 ochn,
    const xmedia_vpss_low_delay_attr *low_delay_info);

/*
 * 函数功能: 获取输出通道低延时属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     low_delay_info - 输出参数, 低延时属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ichn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 不支持低延时
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ochn_low_delay_attr(xmedia_s32 pipe, xmedia_s32 ochn,
    xmedia_vpss_low_delay_attr *low_delay_info);

/*
 * 函数功能: 设置输出通道卷绕属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     wrap_info - 输入参数, 卷绕属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 不支持卷绕
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 输出通道未禁用
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ochn_wrap_attr(xmedia_s32 pipe, xmedia_s32 ochn, const xmedia_video_wrap_attr *wrap_info);

/*
 * 函数功能: 获取输出通道卷绕属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     wrap_info - 输出参数,卷绕属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 不支持低延时
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ochn_wrap_attr(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_video_wrap_attr *wrap_info);

/*
 * 函数功能: 设置扩展通道配置
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     ext_ochn_cfg - 输入参数, 扩展输出通道配置
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ext_ochn_config(xmedia_s32 pipe, xmedia_s32 ochn,
    const xmedia_vpss_ext_ochn_config *ext_ochn_cfg);

/*
 * 函数功能: 设置扩展通道属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     ext_ochn_attr - 输入参数, 扩展通道属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_set_ext_ochn_attr(xmedia_s32 pipe, xmedia_s32 ochn,
    const xmedia_vpss_ext_ochn_attr *ext_ochn_attr);

/*
 * 函数功能: 获取扩展通道属性
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     ext_ochn_attr - 输出参数, 扩展通道属性
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_get_ext_ochn_attr(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_vpss_ext_ochn_attr *ext_ochn_attr);

/*
 *  函数功能：监测输出通道队列是否有图像
 *  函数参数：
 *      watch_ochn       - 输入参数，被监测的通道
 *      watch_ochn_num   - 输入参数，被监测的通道数量
 *      valid_ochn       - 输出参数，有数据的通道
 *      valid_ochn_num   - 输出参数，有数据的通道数量
 *      milli_sec        - 输入参数，超时参数
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_READY        - 未加载驱动/未初始化
 *      XMEDIA_ERRCODE_NULL_PTR         - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM    - 非法参数
 */
xmedia_s32 xmedia_vpss_select_ochn_frame(const xmedia_chn_info *watch_ochn, xmedia_u32 watch_ochn_num,
    xmedia_chn_info *valid_ochn, xmedia_u32 *valid_ochn_num, xmedia_s32 milli_sec);

/*
 * 函数功能: 拼接标定
 * 函数参数：
 *     frame_info - 输入参数，帧信息
 *     config     - 输入参数，标定参数
 *     result     - 输出参数，标定结果
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 */
xmedia_s32 xmedia_vpss_stitch_calibration(const xmedia_vpss_stitch_calibration_frame_info *frame_info,
    const xmedia_vpss_stitch_calibration_config *config, xmedia_vpss_stitch_calibration_result *result);

/*
 * 函数功能: 设置卷绕buf复用配置
 * 函数参数：
 *     wrap_buf_share_cfg - 输入参数，卷绕buf复用配置
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - 未初始化VPSS
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 */
xmedia_s32 xmedia_vpss_set_wrap_buf_share_config(const xmedia_vpss_wrap_buf_share_config *wrap_buf_share_cfg);

/*
 * 函数功能: 设置非融合拼接组的配置
 * 函数参数：
 *     grp              - 输入参数, grp id号
 *     splicing_grp_cfg - 输入参数，非融合拼接组配置
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_GRP_ID - 无效的grp id
 *     XMEDIA_ERRCODE_NOT_INIT - 未初始化VPSS
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 */
xmedia_s32 xmedia_vpss_set_splicing_grp_config(xmedia_s32 grp, const xmedia_vpss_splicing_grp_config *splicing_grp_cfg);

/*
 * 函数功能: 绑定输出通道user vb池
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 *     pool_id - 输入参数, vb池id
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_attach_ochn_vb_pool(xmedia_s32 pipe, xmedia_s32 ochn, xmedia_u32 pool_id);

/*
 * 函数功能: 解除绑定输出通道user vb池
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NOT_EXIST - pipe不存在
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 输入参数非法
 * 注意：
 *     pipe必须已创建
 */
xmedia_s32 xmedia_vpss_detach_ochn_vb_pool(xmedia_s32 pipe, xmedia_s32 ochn);

/*
 * 函数功能: 获取输出通道fd
 * 函数参数：
 *     pipe - 输入参数, pipe id号
 *     ochn - 输入参数, ochn id号
 * 返回值：
 *     正数值 - 有效返回值
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的pipe id
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的ochn id
 *     XMEDIA_ERRCODE_NOT_INIT - 未初始化VPSS
 *     XMEDIA_ERRCODE_NOT_READY - vpss未准备好
 */
xmedia_s32 xmedia_vpss_get_ochn_fd(xmedia_s32 pipe, xmedia_s32 ochn);

#ifdef __cplusplus
}
#endif

#endif /* __XMEDIA_VPSS_H__ */
