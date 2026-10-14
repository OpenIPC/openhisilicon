/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_VGS_H__
#define __XMEDIA_VGS_H__

#include "xmedia_errcode.h"
#include "xmedia_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define XMEDIA_VGS_CLUT_LEN 16
#define XMEDIA_VGS_POINT_NUM 4

typedef struct {
    xmedia_video_frame_info img_in;
    xmedia_video_frame_info img_out;
} xmedia_vgs_frame_info;

typedef struct {
    xmedia_video_point start_point;
    xmedia_video_point end_point;
    xmedia_u32 thick;
    xmedia_u32 color; // 颜色值固定按照ARGB8888解析,alpha固定0xff,RGB用户可配,范围[0x0, 0x00ffffff]
} xmedia_vgs_line_attr;

typedef enum {
    XMEDIA_VGS_COVER_TYPE_RECT = 0,
    XMEDIA_VGS_COVER_TYPE_QUAD_RANGLE,
    XMEDIA_VGS_COVER_TYPE_MAX
} xmedia_vgs_cover_type;

typedef struct {
    xmedia_bool is_solid;
    xmedia_u32 thick;
    xmedia_video_point points[XMEDIA_VGS_POINT_NUM];
} xmedia_vgs_quadrangle_cover;

typedef struct {
    xmedia_vgs_cover_type cover_type;
    union {
        xmedia_video_rect dst_rect;
        xmedia_vgs_quadrangle_cover quadrangle;
    };
    xmedia_u32 color;
} xmedia_vgs_cover_attr;

typedef enum {
    XMEDIA_VGS_OSD_REVERT_MODE_RGB = 0,
    XMEDIA_VGS_OSD_REVERT_MODE_ALPHA,
    XMEDIA_VGS_OSD_REVERT_MODE_BOTH,
    XMEDIA_VGS_OSD_REVERT_MODE_MAX
} xmedia_vgs_osd_revert_mode;

typedef struct {
    xmedia_video_rect osd_rect;
    xmedia_video_pixel_format pixel_fmt;
    xmedia_u64 phys_addr;
    xmedia_u32 stride;
    xmedia_u8 alpha0; // 仅用于位图是ARGB1555场景,alpha位0对应的8位有效值
    xmedia_u8 alpha1; // 仅用于位图是ARGB1555场景,alpha位1对应的8位有效值
    xmedia_bool is_osd_revert;
    xmedia_vgs_osd_revert_mode revert_mode;
    xmedia_u32 clut[XMEDIA_VGS_CLUT_LEN];
} xmedia_vgs_osd_attr;

typedef struct {
    xmedia_video_size dst_img_size;
    xmedia_video_point out_block_offset;
} xmedia_vgs_block_info;

/*
 * 函数功能: 初始化vgs模块
 * 函数参数：
 *     NA
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_READY - 驱动未加载
 */
xmedia_s32 xmedia_vgs_init(xmedia_void);

/*
 * 函数功能: 去初始化vgs模块
 * 函数参数：
 *     NA
 * 返回值：
 *     NA
 */
xmedia_void xmedia_vgs_exit(xmedia_void);

/*
 * 函数功能: 创建job
 * 函数参数：
 *     handle - 输出参数, 对应job的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 */
xmedia_s32 xmedia_vgs_create_job(xmedia_s32 *handle);

/*
 * 函数功能: 添加缩放task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入、输出帧信息
 *     scale_mode - 输入参数， 缩放模式
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_scale(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    xmedia_video_scale_mode scale_mode);

/*
 * 函数功能: 添加画线task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入、输出帧信息
 *     line - 输入参数, 线条属性信息
 *     num - 输入参数, 线条个数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_line(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    const xmedia_vgs_line_attr *line, xmedia_u32 num);

/*
 * 函数功能: 添加遮挡task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入、输出帧信息
 *     cover - 输入参数, 遮挡属性信息
 *     num - 输入参数, 遮挡个数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_cover(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    const xmedia_vgs_cover_attr *cover, xmedia_u32 num);

/*
 * 函数功能: 添加叠加位图task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入、输出帧信息
 *     osd - 输入参数, 位图属性信息
 *     num - 输入参数, 位图个数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_osd(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    const xmedia_vgs_osd_attr *osd, xmedia_u32 num);

/*
 * 函数功能: 添加旋转task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入、输出帧信息
 *     rotation - 输入参数, 旋转角度
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_rotation(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    xmedia_video_rotation rotation);

/*
 * 函数功能: 添加亮度统计task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入、输出帧信息
 *     luma_rect - 输入参数, 亮度统计区域
 *     num - 输入参数, 区域个数，针对luma_rect和luma_data同时生效
 *     luma_data - 输出参数, 亮度统计结果
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_luma(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    const xmedia_video_rect *luma_rect, xmedia_u32 num, xmedia_u64 *luma_data);

/*
 * 函数功能: 添加block scale task到指定的job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 *     frame_info - 输入参数, 输入原图(整图)、输出分块帧信息
 *     block_info - 输入参数, 分块信息
 *     scale_mode - 输入参数, 缩放模式
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的job不可再添加任务)
 */
xmedia_s32 xmedia_vgs_add_task_block_scale(xmedia_s32 handle, const xmedia_vgs_frame_info *frame_info,
    const xmedia_vgs_block_info *block_info, xmedia_video_scale_mode scale_mode);

/*
 * 函数功能: 提交job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_vgs_submit_job(xmedia_s32 handle);

/*
 * 函数功能: 取消job
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 * 注意：异步提交, 接口返回不代表硬件已经完成，可使用xmedia_vgs_wait_job同步
 */
xmedia_s32 xmedia_vgs_cancel_job(xmedia_s32 handle);

/*
 * 函数功能: 等待job完成
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_vgs_create_job得到的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - vgs系统没有准备好
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_TIMEOUT - 等待超时
 */
xmedia_s32 xmedia_vgs_wait_job(xmedia_s32 handle, xmedia_u32 timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMEDIA_VGS_H__ */
