/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_TDE_H__
#define __XMEDIA_TDE_H__

#include "xmedia_errcode.h"
#include "xmedia_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define XMEDIA_COLOR_COMPONENT_NUM 3

typedef struct {
    xmedia_u64 phys_addr[XMEDIA_COLOR_COMPONENT_NUM]; // 0:Y或RGB; 1:UV; 2:U或V
    xmedia_u32 stride[XMEDIA_COLOR_COMPONENT_NUM]; // 0:Y或RGB; 1:UV; 2:U或V
    xmedia_u32 width;
    xmedia_u32 height;
    xmedia_video_pixel_format pixel_format;
    xmedia_bool is_alpha_ext_1555; // ARGB1555 alpha位扩展8位有效位场景,0:系统默认扩展,1:用户配置
    xmedia_u8 alpha0; // ARGB1555 alpha位扩展8位有效位场景,0对应的8位有效位
    xmedia_u8 alpha1; // ARGB1555 alpha位扩展8位有效位场景,1对应的8位有效位
    xmedia_u64 clut_phys_addr; // 256个argb8888颜色，总内存256*4字节
    xmedia_bool clut_reload;
} xmedia_tde_surface;

typedef struct {
    xmedia_tde_surface surface;
    xmedia_video_rect rect;
} xmedia_tde_surface_info;

typedef struct {
    xmedia_video_point start_point;
    xmedia_video_point end_point;
    xmedia_u32 thick;
    xmedia_u32 color; // 颜色值固定按照ARGB8888解析,可配范围[0x0, 0xffffffff]
} xmedia_tde_line_attr;

typedef enum {
    XMEDIA_TDE_BLEND_MODE_CLEAR = 0, // fs: 0.0     fd: 0.0
    XMEDIA_TDE_BLEND_MODE_SRC,       // fs: 1.0     fd: 0.0
    XMEDIA_TDE_BLEND_MODE_DST,       // fs: 0.0     fd: 1.0
    XMEDIA_TDE_BLEND_MODE_SRCOVER,   // fs: 1.0     fd: 1.0-sa
    XMEDIA_TDE_BLEND_MODE_DSTOVER,   // fs: 1.0-da  fd: 1.0
    XMEDIA_TDE_BLEND_MODE_SRCIN,     // fs: da      fd: 0.0
    XMEDIA_TDE_BLEND_MODE_DSTIN,     // fs: 0.0     fd: sa
    XMEDIA_TDE_BLEND_MODE_SRCOUT,    // fs: 1.0-da  fd: 0.0
    XMEDIA_TDE_BLEND_MODE_DSTOUT,    // fs: 0.0     fd: 1.0-sa
    XMEDIA_TDE_BLEND_MODE_SRCATOP,   // fs: da      fd: 1.0-sa
    XMEDIA_TDE_BLEND_MODE_DSTATOP,   // fs: 1.0-da  fd: sa
    XMEDIA_TDE_BLEND_MODE_XOR,       // fs: 1.0-da  fd: 1.0-sa
    XMEDIA_TDE_BLEND_MODE_DARKEN,    // alpha取1，color取min(s1, s2)
    XMEDIA_TDE_BLEND_MODE_LIGHTEN,   // alpha取1，color取max(s1, s2)
    XMEDIA_TDE_BLEND_MODE_MULTIPLY,  // As1 * As2/256, Cs1 * Cs2/256
    XMEDIA_TDE_BLEND_MODE_SCREEN,    // As1 + As2 - As1 * As2/256, Cs1 + Cs2 - Cs1 * Cs2/256
    XMEDIA_TDE_BLEND_MODE_MAX
} xmedia_tde_blend_mode;

typedef struct {
    xmedia_bool global_alpha_en;
    xmedia_bool pixel_alpha_en;
    xmedia_bool bg_premulti_en;
    xmedia_bool fg_premulti_en;
    xmedia_tde_blend_mode blend_mode;
} xmedia_tde_blend_attr;

typedef enum {
    XMEDIA_TDE_ROP_MODE_BLACKNESS = 0, // Blackness
    XMEDIA_TDE_ROP_MODE_NOTMERGEPEN,   // ~(S2 | S1)
    XMEDIA_TDE_ROP_MODE_MASKNOTPEN,    // ~S2&S1
    XMEDIA_TDE_ROP_MODE_NOTCOPYPEN,    // ~S2
    XMEDIA_TDE_ROP_MODE_MASKPENNOT,    // S2&~S1
    XMEDIA_TDE_ROP_MODE_NOT,           // ~S1
    XMEDIA_TDE_ROP_MODE_XORPEN,        // S2^S1
    XMEDIA_TDE_ROP_MODE_NOTMASKPEN,    // ~(S2 & S1)
    XMEDIA_TDE_ROP_MODE_MASKPEN,       // S2&S1
    XMEDIA_TDE_ROP_MODE_NOTXORPEN,     // ~(S2^S1)
    XMEDIA_TDE_ROP_MODE_NOP,           // S1
    XMEDIA_TDE_ROP_MODE_MERGENOTPEN,   // ~S2|S1
    XMEDIA_TDE_ROP_MODE_COPYPEN,       // S2
    XMEDIA_TDE_ROP_MODE_MERGEPENNOT,   // S2|~S1
    XMEDIA_TDE_ROP_MODE_MERGEPEN,      // S2|S1
    XMEDIA_TDE_ROP_MODE_WHITE,         // Whiteness
    XMEDIA_TDE_ROP_MODE_MAX
} xmedia_tde_rop_mode;

typedef struct {
    xmedia_tde_rop_mode alpha_rop;
    xmedia_tde_rop_mode color_rop;
} xmedia_tde_rop_attr;

typedef enum {
    XMEDIA_TDE_COLORKEY_MODE_FOREGROUND = 0,
    XMEDIA_TDE_COLORKEY_MODE_BACKGROUND,
    XMEDIA_TDE_COLORKEY_MODE_MAX
} xmedia_tde_colorkey_mode;

typedef struct {
    xmedia_u8 component_min;
    xmedia_u8 component_max;
    xmedia_u8 component_mask;
    xmedia_bool is_component_out;
    xmedia_bool is_component_ignore;
} xmedia_tde_colorkey_comp;

typedef union {
    struct {
        xmedia_tde_colorkey_comp alpha;
        xmedia_tde_colorkey_comp red;
        xmedia_tde_colorkey_comp green;
        xmedia_tde_colorkey_comp blue;
    } argb_color_key;
    struct {
        xmedia_tde_colorkey_comp alpha;
        xmedia_tde_colorkey_comp y;
        xmedia_tde_colorkey_comp cb;
        xmedia_tde_colorkey_comp cr;
    } yuv_color_key;
    struct {
        xmedia_tde_colorkey_comp alpha;
        xmedia_tde_colorkey_comp clut;
    } clut_color_key;
} xmedia_tde_colorkey_value;

typedef struct {
    xmedia_tde_colorkey_mode ck_mode;
    xmedia_tde_colorkey_value ck_value;
} xmedia_tde_colorkey_attr;

typedef enum {
    XMEDIA_TDE_CLIP_MODE_INSIDE = 0,
    XMEDIA_TDE_CLIP_MODE_OUTSIDE,
    XMEDIA_TDE_CLIP_MODE_MAX
} xmedia_tde_clip_mode;

typedef struct {
    xmedia_tde_clip_mode clip_mode;
    xmedia_video_rect clip_rect;
} xmedia_tde_clip_attr;

typedef enum {
    XMEDIA_TDE_OUT_ALPHA_MODE_NORMAL = 0,
    XMEDIA_TDE_OUT_ALPHA_MODE_FROM_BACKGROUND,
    XMEDIA_TDE_OUT_ALPHA_MODE_FROM_FOREGROUND,
    XMEDIA_TDE_OUT_ALPHA_MODE_FROM_GLOBALALPHA,
    XMEDIA_TDE_OUT_ALPHA_MODE_MAX
} xmedia_tde_out_alpha_mode;

typedef struct {
    xmedia_bool blend_en;
    xmedia_tde_blend_attr blend_attr;
    xmedia_bool rop_en;
    xmedia_tde_rop_attr rop_attr;
    xmedia_bool colorkey_en;
    xmedia_tde_colorkey_attr ck_attr;
    xmedia_bool clip_en;
    xmedia_tde_clip_attr clip_attr;
    xmedia_bool mirror_en;
    xmedia_bool flip_en;
    xmedia_tde_out_alpha_mode alpha_mode;
    xmedia_u8 global_alpha;
} xmedia_tde_operation_attr;

/*
 * 函数功能: 初始化tde模块
 * 函数参数：
 *     NA
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_READY - 驱动未加载
 */
xmedia_s32 xmedia_tde_init(xmedia_void);

/*
 * 函数功能: 去初始化tde模块
 * 函数参数：
 *     NA
 * 返回值：
 *     NA
 */
xmedia_void xmedia_tde_exit(xmedia_void);

/*
 * 函数功能: 创建一个工作组(job)
 * 函数参数：
 *     handle - 输出参数, 对应job的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的job资源
 */
xmedia_s32 xmedia_tde_create_job(xmedia_s32 *handle);

/*
 * 函数功能: 添加一个或多个画线任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     dst - 输入参数, 目标surface和目标rect
 *     line - 输入参数, 线条属性信息
 *     line_num - 输入参数, 线条个数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_draw_line(xmedia_s32 handle, const xmedia_tde_surface_info *dst,
    xmedia_tde_line_attr *line, xmedia_u32 line_num);

/*
 * 函数功能: 添加一个快速填充任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     dst - 输入参数, 目标surface和目标rect
 *     fill_color - 输入参数, 填充色, 固定按ARGB8888配
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_quick_fill(xmedia_s32 handle, const xmedia_tde_surface_info *dst,
    xmedia_u32 fill_color);

/*
 * 函数功能: 添加一个快速拷贝任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     src - 输入参数, 源surface和源rect
 *     dst - 输入参数, 目标surface和目标rect
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_quick_copy(xmedia_s32 handle, const xmedia_tde_surface_info *src,
    const xmedia_tde_surface_info *dst);

/*
 * 函数功能: 添加一个缩放任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     src - 输入参数, 源surface和源rect
 *     dst - 输入参数, 目标surface和目标rect
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_scale(xmedia_s32 handle, const xmedia_tde_surface_info *src,
    const xmedia_tde_surface_info *dst);

/*
 * 函数功能: 添加一个旋转任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     src - 输入参数, 源surface和源rect
 *     dst - 输入参数, 目标surface和目标rect
 *     rot - 输入参数, 旋转角度
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_rotation(xmedia_s32 handle, const xmedia_tde_surface_info *src,
    const xmedia_tde_surface_info *dst, xmedia_video_rotation rot);

/*
 * 函数功能: 添加一个单源搬移任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     src - 输入参数, 源surface和源rect
 *     dst - 输入参数, 目标surface和目标rect
 *     fill_color - 输入参数, 可以配ARGB8888格式的填充色当做前景与背景叠加输出到目标位图
 *     opt - 输入参数, 附加操作集合(alpha blending/rop/colorkey/clip/mirro/flip); opt可以为NULL
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_single_blit(xmedia_s32 handle, const xmedia_tde_surface_info *src,
    const xmedia_tde_surface_info *dst, xmedia_u32 fill_color, xmedia_tde_operation_attr *opt);

/*
 * 函数功能: 添加一个双源搬移任务(task)到指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 *     bg - 输入参数, 背景surface和背景rect
 *     fg - 输入参数, 前景surface和前景rect
 *     dst - 输入参数, 目标surface和目标rect
 *     opt - 输入参数, 附加操作集合(alpha blending/rop/colorkey/clip/mirro/flip);不能为NULL
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 没有空闲的task资源
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_SUPPORT - 参数配置超出能力范围，不支持
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许(如已经提交的工作组不可再添加任务)
 */
xmedia_s32 xmedia_tde_add_task_double_blit(xmedia_s32 handle, const xmedia_tde_surface_info *bg,
    const xmedia_tde_surface_info *fg, const xmedia_tde_surface_info *dst, xmedia_tde_operation_attr *opt);

/*
 * 函数功能: 提交指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 */
xmedia_s32 xmedia_tde_submit_job(xmedia_s32 handle);

/*
 * 函数功能: 取消指定的工作组(job)
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许
 * 注意：异步提交, 接口返回不代表硬件已经完成，可使用xmedia_vgs_wait_job同步
 */
xmedia_s32 xmedia_tde_cancel_job(xmedia_s32 handle);

/*
 * 函数功能: 等待指定的工作组(job)完成
 * 函数参数：
 *     handle - 输入参数, 对应xmedia_tde_create_job得到的有效句柄
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 *     XMEDIA_ERRCODE_TIMEOUT - 等待超时
 */
xmedia_s32 xmedia_tde_wait_job(xmedia_s32 handle, xmedia_u32 timeout);

/*
 * 函数功能: 指定alpha阈值
 * 函数参数：
 *     value - 输入参数, alpha阈值 0~255
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 */
xmedia_s32 xmedia_tde_set_alpha_threshold_value(xmedia_u8 value);

/*
 * 函数功能: 获取alpha阈值
 * 函数参数：
 *     value - 输出参数, alpha阈值 0~255
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - tde模块未初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数不合法
 */
xmedia_s32 xmedia_tde_get_alpha_threshold_value(xmedia_u8 *value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMEDIA_TDE_H__ */
