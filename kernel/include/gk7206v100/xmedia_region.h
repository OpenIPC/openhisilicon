/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __XMEDIA_REGION_H__
#define __XMEDIA_REGION_H__

#include "xmedia_sys.h"
#include "xmedia_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// 定义颜色查表长度
#define RGN_CLUT_NUM        16

// 定义任意四边形坐标点个数
#define RGN_QUAD_POINT_NUM  4

// 定义区域类型
typedef enum {
    XMEDIA_RGN_TYPE_OVERLAY = 0,
    XMEDIA_RGN_TYPE_OVERLAYEX,
    XMEDIA_RGN_TYPE_COVER,
    XMEDIA_RGN_TYPE_COVEREX,
    XMEDIA_RGN_TYPE_MOSAIC,
    XMEDIA_RGN_TYPE_MOSAICEX,
    XMEDIA_RGN_TYPE_CORNER_RECT,
    XMEDIA_RGN_TYPE_CORNER_RECTEX,
    XMEDIA_RGN_TYPE_MAX
}xmedia_rgn_type;

// 定义overlay区域属性结构体
typedef struct  {
    xmedia_u32                  bg_color;     // 背景色
    xmedia_u32                  canvas_num;   // 内存个数
    xmedia_video_size           size;         // 区域大小
    xmedia_video_pixel_format   pixel_format; // 像素格式
} xmedia_rgn_overlay_attr;

// 定义overlayex区域属性结构体
typedef struct  {
    xmedia_u32                  bg_color;     // 背景色
    xmedia_u32                  canvas_num;   // 内存个数
    xmedia_video_size           size;         // 区域大小
    xmedia_video_pixel_format   pixel_format; // 像素格式
} xmedia_rgn_overlayex_attr;

// 定义区域属性联合体
typedef union {
    xmedia_rgn_overlay_attr    overlay;   // overlay属性
    xmedia_rgn_overlayex_attr  overlayex; // overlayex属性
} xmedia_rgn_type_attr;

// 定义区域属性结构体
typedef struct {
    xmedia_rgn_type         type; // 区域类型
    xmedia_rgn_type_attr    attr; // 区域属性
} xmedia_rgn_attr;

// 定义mosaic/mosaicex 块大小
typedef enum  {
    XMEDIA_RGN_MOSAIC_BLK_SIZE_8,
    XMEDIA_RGN_MOSAIC_BLK_SIZE_16,
    XMEDIA_RGN_MOSAIC_BLK_SIZE_32,
    XMEDIA_RGN_MOSAIC_BLK_SIZE_MAX
} xmedia_rgn_mosaic_blk_size;

// 定义mosaic属性结构体
typedef struct {
    xmedia_u32                   layer;    // 区域层次
    xmedia_video_rect            rect;     // 位置宽高
    xmedia_rgn_mosaic_blk_size   blk_size; // 块大小
} xmedia_rgn_mosaic_chn_attr;

// 定义mosaicex属性结构体
typedef struct {
    xmedia_u32                   layer;    // 区域层次
    xmedia_video_rect            rect;     // 位置宽高
    xmedia_rgn_mosaic_blk_size   blk_size; // 块大小
} xmedia_rgn_mosaicex_chn_attr;

// 定义cover/coverex 类型
typedef enum {
    XMEDIA_RGN_COVER_TYPE_RECT = 0,  // 矩形
    XMEDIA_RGN_COVER_TYPE_QUAD,      // 任意四边形
    XMEDIA_RGN_COVER_TYPE_MAX
} xmedia_rgn_cover_type;

// 定义任意四边形属性结构体
typedef struct {
    xmedia_bool          is_solid;                 // 实心虚心标志位
    xmedia_u32           thick;                    // 厚度
    xmedia_video_point   point[RGN_QUAD_POINT_NUM];// 坐标点位置
} xmedia_rgn_quad;

// 定义cover通道属性结构体
typedef struct {
    xmedia_rgn_cover_type      cover_type; // 类型
    union {
        xmedia_video_rect      rect;       // 位置宽高
        xmedia_rgn_quad        quad;       // 任意四边形属性
    };
    xmedia_u32                 color;      // 颜色
    xmedia_u32                 layer;      // 区域层次
} xmedia_rgn_cover_chn_attr;

// 定义coverex通道属性结构体
typedef struct {
    xmedia_rgn_cover_type      coverex_type;// 类型
    union {
        xmedia_video_rect      rect;        // 位置宽高
        xmedia_rgn_quad        quad;        // 任意四边形属性
    };
    xmedia_u32                 color;       // 颜色
    xmedia_u32                 layer;       // 区域层次
} xmedia_rgn_coverex_chn_attr;

// 定义qp保护
typedef struct {
    xmedia_bool        disable;    // 关闭 osd与roi重叠区域 的osd qp保护
    xmedia_bool        is_abs_qp;  // 是否绝对qp
    xmedia_s32         qp_val;     // qp值
} xmedia_rgn_overlay_qp_info;

// 定义 venc 图像类型
typedef enum {
    XMEDIA_RGN_ATTACH_JPEG_MAIN = 0,
    XMEDIA_RGN_ATTACH_JPEG_MPF0,
    XMEDIA_RGN_ATTACH_JPEG_MPF1,
    XMEDIA_RGN_ATTACH_JPEG_MAX
} xmedia_rgn_attach_dst;

// 定义反色模式
typedef enum {
    XMEDIA_RGN_OSD_INVERT_COLOR_MODE_RGB = 0,
    XMEDIA_RGN_OSD_INVERT_COLOR_MODE_ALPHA,
    XMEDIA_RGN_OSD_INVERT_COLOR_MODE_BOTH,
    XMEDIA_RGN_OSD_INVERT_COLOR_MODE_MAX
} xmedia_rgn_osd_invert_color_mode;

// 定义反色属性
typedef struct {
    xmedia_bool                      enable;     // 是否开启反色
    xmedia_rgn_osd_invert_color_mode mode;       // 反色模式
} xmedia_rgn_osd_invert_color_attr;

// 定义overlay通道属性结构体
typedef struct {
    xmedia_u8             alpha0;                  // 仅用于位图是ARGB1555场景,alpha位0对应的8位有效值
    xmedia_u8             alpha1;                  // 仅用于位图是ARGB1555场景,alpha位1对应的8位有效值
    xmedia_u32            layer;                   // 区域层次
    xmedia_u32            clut[RGN_CLUT_NUM];      // 颜色查找表
    xmedia_video_point    point;                   // 坐标点
    xmedia_rgn_attach_dst dst;                     // venc 图像类型
    xmedia_rgn_overlay_qp_info  qp_info;           // qp保护
    xmedia_rgn_osd_invert_color_attr invert_color; // 反色属性
} xmedia_rgn_overlay_chn_attr;

// 定义overlayex通道属性结构体
typedef struct {
    xmedia_u8            alpha0;                   // 仅用于位图是ARGB1555场景,alpha位0对应的8位有效值
    xmedia_u8            alpha1;                   // 仅用于位图是ARGB1555场景,alpha位1对应的8位有效值
    xmedia_u32           layer;                    // 区域层次
    xmedia_u32           clut[RGN_CLUT_NUM];       // 颜色查找表
    xmedia_video_point   point;                    // 坐标点
    xmedia_rgn_osd_invert_color_attr invert_color; // 反色属性
} xmedia_rgn_overlayex_chn_attr;

// 定义corner rect通道属性结构体
typedef struct {
    xmedia_u32            thick;   // 线宽
    xmedia_u32            color;   // 颜色
    xmedia_u32            layer;   // 区域层次
    xmedia_u32            hor_len; // 水平线长
    xmedia_u32            ver_len; // 垂直线长
    xmedia_video_rect     rect;    // 位置宽高
} xmedia_rgn_corner_rect_chn_attr;

// 定义corner rectex通道属性结构体
typedef struct {
    xmedia_u32            thick;   // 线宽
    xmedia_u32            color;   // 颜色
    xmedia_u32            layer;   // 区域层次
    xmedia_u32            hor_len; // 水平线长
    xmedia_u32            ver_len; // 垂直线长
    xmedia_video_rect     rect;    // 位置宽高
} xmedia_rgn_corner_rectex_chn_attr;

// 定义区域通道属性联合体
typedef union {
    xmedia_rgn_overlay_chn_attr        overlay_chn;       // overlay区域
    xmedia_rgn_overlayex_chn_attr      overlayex_chn;     // overlayex区域
    xmedia_rgn_cover_chn_attr          cover_chn;         // cover区域
    xmedia_rgn_coverex_chn_attr        coverex_chn;       // coverex区域
    xmedia_rgn_mosaic_chn_attr         mosaic_chn;        // mosaic区域
    xmedia_rgn_mosaicex_chn_attr       mosaicex_chn;      // mosaicex区域
    xmedia_rgn_corner_rect_chn_attr    corner_rect_chn;   // corner区域
    xmedia_rgn_corner_rectex_chn_attr  corner_rectex_chn; // cornerex区域
} xmedia_rgn_type_chn_attr;

// 定义区域通道属性
typedef struct  {
    xmedia_bool              is_show;  // 是否显示
    xmedia_rgn_type          type;     // 区域类型
    xmedia_rgn_type_chn_attr attr;     // 区域属性
} xmedia_rgn_chn_attr;


// 定义区域画布信息
typedef struct {
    xmedia_u64                  phy_addr;     // 画布物理地址
    xmedia_void *ATTRIBUTE      virt_addr;    // 画布虚拟地址
    xmedia_u32                  stride;       // 画布跨距
    xmedia_video_size           size;         // 画布大小
    xmedia_video_pixel_format   pixel_format; // 像素格式
} xmedia_rgn_canvas_info;

/*
 * 函数功能: 初始化region模块
 * 函数参数：
 *     NA
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_READY - 驱动未加载
 */
xmedia_s32 xmedia_rgn_init(xmedia_void);

/*
 * 函数功能: 去初始化region模块
 * 函数参数：
 *     NA
 * 返回值：
 *     NA
 */
xmedia_void xmedia_rgn_exit(xmedia_void);

/*
 * 函数功能: 创建一个区域
 * 函数参数：
 *       handle - 输入参数，区域号
 *       rgn_attr - 输入参数，区域属性指针
 * 返回值：
 *       XMEDIA_SUCCESS - 区域创建成功
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_EXIST - 重复创建已存在的区域
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NO_BUFFER_FREE - 分配内存失败
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_create(xmedia_u32 handle, const xmedia_rgn_attr *rgn_attr);

/*
 * 函数功能: 销毁一个区域
 * 函数参数：
 *       handle - 输入参数，区域号
 *
 * 返回值：
 *       XMEDIA_SUCCESS - 区域销毁成功
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_EXIST -  区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_destroy(xmedia_u32 handle);

/*
 * 函数功能: 获取区域属性
 * 函数参数：
 *       handle - 输入参数，区域号
 *       rgn_attr - 输出参数，区域属性指针
 * 返回值：
 *       XMEDIA_SUCCESS - 获取区域属性成功
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_get_attr(xmedia_u32 handle, xmedia_rgn_attr *rgn_attr);

/*
 * 函数功能: 设置区域属性
 * 函数参数：
 *       handle - 输入参数，区域号
 *       rgn_attr - 输入参数，区域属性指针
 * 返回值：
 *       XMEDIA_SUCCESS - 设置区域属性成功
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_BUFFER_FULL - 缓冲区数据满
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_set_attr(xmedia_u32 handle, const xmedia_rgn_attr *rgn_attr);

/*
 * 函数功能: 将区域叠加到通道上
 * 函数参数：
 *       handle - 输入参数，区域号
 *       chn - 输入参数，通道结构体指针
 *       chn_attr - 输入参数，区域通道属性指针
 * 返回值：
 *       XMEDIA_SUCCESS - 区域成功叠加到区域
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备号
 *       XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *       XMEDIA_ERRCODE_NO_BUFFER_FREE - 分配内存失败
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_attach_to_chn(xmedia_u32 handle, const xmedia_chn_info *chn, const xmedia_rgn_chn_attr *chn_attr);

/*
 * 函数功能: 将区域从通道中撤离
 * 函数参数：
 *       handle - 输入参数，区域号
 *       chn - 输入参数，通道结构体指针
 * 返回值：
 *       XMEDIA_SUCCESS - 区域成功从通道中撤离
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备号
 *       XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_detach_from_chn(xmedia_u32 handle, const xmedia_chn_info *chn);

/*
 * 函数功能: 获取区域通道属性
 * 函数参数：
 *       handle - 输入参数，区域号
 *       chn - 输入参数，通道结构体指针
 *       chn_attr - 输出参数，区域通道属性指针
 * 返回值：
 *       XMEDIA_SUCCESS - 成功获取区域通道属性
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备号
 *       XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_get_display_attr(xmedia_u32 handle, const xmedia_chn_info *chn, xmedia_rgn_chn_attr *chn_attr);

/*
 * 函数功能: 设置区域通道属性
 * 函数参数：
 *       handle - 输入参数，区域号
 *       chn - 输入参数，通道结构体指针
 *       chn_attr - 输入参数，区域通道属性指针
 * 返回值：
 *       XMEDIA_SUCCESS - 成功设置区域通道属性
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备号
 *       XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_set_display_attr(xmedia_u32 handle, const xmedia_chn_info *chn, const xmedia_rgn_chn_attr *chn_attr);

/*
 * 函数功能: 获取区域画布信息
 * 函数参数：
 *       handle - 输入参数，区域号
 *       canvas_info --输出参数，画布信息
 * 返回值：
 *       XMEDIA_SUCCESS - 成功获取区域画布信息
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_BUFFER_FULL - 缓冲区数据满
 *       XMEDIA_ERRCODE_NO_BUFFER_FREE - 分配内存失败
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_get_canvas_info(xmedia_u32 handle, xmedia_rgn_canvas_info *canvas_info);

/*
 * 函数功能: 获取区域画布信息
 * 函数参数：
 *       handle - 输入参数，区域号
 * 返回值：
 *       XMEDIA_SUCCESS - 成功获取区域画布信息
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_update_canvas (xmedia_u32 handle);

/*
 * 函数功能: 开启批处理
 * 函数参数：
 *       grp - 输入参数，批处理组号
 *       handle_num - 输入参数，一组批处理中，区域个数
 *       handle[] - 输入参数，区域句柄号
 * 返回值：
 *       XMEDIA_SUCCESS - 成功获取区域画布信息
 *       XMEDIA_ERRCODE_NULL_PTR - 函数参数中有空指针
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_batch_begin(xmedia_u32 *grp, xmedia_u32 handle_num, const xmedia_u32 handle[]);

/*
 * 函数功能: 结束批处理
 * 函数参数：
 *       grp - 输入参数，组号
 * 返回值：
 *       XMEDIA_SUCCESS - 成功获取区域画布信息
 *       XMEDIA_ERRCODE_INVALID_PARAM - 参数超出合法范围
 *       XMEDIA_ERRCODE_NOT_SUPPORT - 不支持的参数或功能
 *       XMEDIA_ERRCODE_NOT_EXIST - 区域不存在
 *       XMEDIA_ERRCODE_NOT_PERMITTED - 该操作不允许
 *       XMEDIA_ERRCODE_NOT_READY - 系统没有初始化或没有加载相应模块的ko
 */
xmedia_s32 xmedia_rgn_batch_end(xmedia_u32 grp);

#ifdef __cplusplus
}
#endif

#endif /* __XMEDIA_REGION_H__ */

