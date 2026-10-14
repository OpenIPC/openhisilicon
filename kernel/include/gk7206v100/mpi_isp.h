/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __MPI_ISP_H__
#define __MPI_ISP_H__

#include "xmedia_type.h"
#include "xmedia_video_common.h"

#define ISP_OVERLAP_AREA_PRECISION 32
#define ISP_OVERLAP_REGION         2

typedef enum {
    MPI_ISP_EVENT_SET_BNR_REF_STATE = 0,
    MPI_ISP_EVENT_SET_STNR_REF_STATE,
    MPI_ISP_EVENT_MAX
} mpi_isp_event;

/*
 * 函数功能: 设置mlsc拼接亮度同步属性
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      src_position - 输入参数，左右图源图像坐标组
 *      overlap_width - 输入参数，重叠区域宽度
 *      sync_attr - 输入参数，拼接亮度同步属性
 *      area_flag - 输入参数，重叠区域标志
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 mpi_isp_set_mlsc_stitch_luma_attr(
    xmedia_u32 pipe, const xmedia_video_point src_position[ISP_OVERLAP_REGION][ISP_OVERLAP_AREA_PRECISION],
    xmedia_u32 overlap_width, const xmedia_video_stitch_luma_sync_attr *sync_attr, xmedia_u8 area_flag);

/*
 * 函数功能: 模块间的事件通知
 * 函数参数：
 *      pipe - 输入参数，ISP 设备号
 *      event - 输入参数，事件类型
 *      param - 输入参数，事件参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 输入pipe无效
 *      XMEDIA_ERRCODE_INVALID_PARAM - 输入参数无效
 */
xmedia_s32 mpi_isp_event_notifier(xmedia_u32 pipe, mpi_isp_event event, xmedia_void *param);

#endif // __MPI_ISP_H__
