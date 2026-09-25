/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _XMEDIA_IVP_H_
#define _XMEDIA_IVP_H_

#include "type.h"
#include "errcode.h"
#include "common.h"
#include "comm_video.h"
#include "ivp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

xmedia_s32 xmedia_ivp_init(xmedia_void);

xmedia_s32 xmedia_ivp_deinit(xmedia_void);

xmedia_s32 xmedia_ivp_load_resource_from_memory(const ivp_mem_info *ivp_file_mem_info, xmedia_s32 *ivp_handle);

xmedia_s32 xmedia_ivp_unload_resource(xmedia_s32 ivp_handle);

xmedia_s32 xmedia_ivp_set_ctrl_attr(xmedia_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

xmedia_s32 xmedia_ivp_get_ctrl_attr(xmedia_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

xmedia_s32 xmedia_ivp_set_fd_ctrl_attr(xmedia_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

xmedia_s32 xmedia_ivp_get_fd_ctrl_attr(xmedia_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

xmedia_s32 xmedia_ivp_set_venc_low_bitrate(xmedia_s32 ivp_handle, xmedia_s32 venc_chn, xmedia_bool enable);

xmedia_s32 xmedia_ivp_get_venc_low_bitrate(xmedia_s32 ivp_handle, xmedia_s32 venc_chn, xmedia_bool *enable);

xmedia_s32 xmedia_ivp_set_advance_isp(xmedia_s32 ivp_handle, xmedia_s32 vi_pipe, xmedia_bool enable);

xmedia_s32 xmedia_ivp_get_advance_isp(xmedia_s32 ivp_handle, xmedia_s32 vi_pipe, xmedia_bool *enable);

xmedia_s32 xmedia_ivp_set_roi_attr(xmedia_s32 ivp_handle, const ivp_roi_attr *roi_attr);

xmedia_s32 xmedia_ivp_get_roi_attr(xmedia_s32 ivp_handle, ivp_roi_attr *roi_attr);

xmedia_s32 xmedia_ivp_set_roi_map(xmedia_s32 ivp_handle, const ivp_roi_map *roi_map);

xmedia_s32 xmedia_ivp_process(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, xmedia_bool *obj_alarm);

xmedia_s32 xmedia_ivp_process_ex(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, ivp_obj_array *obj_array);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

