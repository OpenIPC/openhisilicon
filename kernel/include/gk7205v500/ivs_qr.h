/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _IVS_QR_H_
#define _IVS_QR_H_

#include "common_qr.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

xmedia_s32 ivs_qr_init(xmedia_void);

xmedia_s32 ivs_qr_get_buf_size(xmedia_u32 max_img_width, xmedia_u32 max_img_height, xmedia_u32 *size);

xmedia_s32 ivs_qr_proc(VIDEO_FRAME_INFO_S *src_frame, qr_mem_info *buf, qr_info *qr_info);

xmedia_s32 ivs_qr_deinit(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
