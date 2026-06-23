/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_FRAMEINFO_H
#define ISP_FRAMEINFO_H

#include "ot_common_isp.h"
#include "ot_common_video.h"
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    ot_isp_frame_info *isp_frame;
} isp_frame_info_ctrl;

typedef struct {
    ot_isp_attach_info *attach_info;
} isp_attach_info_ctrl;

typedef struct {
    ot_isp_colorgammut_info *color_gamut_info;
} isp_gammut_info_ctrl;

td_s32 isp_trans_info_init(ot_vi_pipe vi_pipe, isp_trans_info_buf *trans_info);
td_void isp_trans_info_exit(ot_vi_pipe vi_pipe);

td_s32 isp_pro_info_init(ot_vi_pipe vi_pipe, const isp_pro_info_buf *pro_info);
td_void isp_pro_info_exit(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
