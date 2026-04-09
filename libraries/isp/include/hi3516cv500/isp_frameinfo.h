/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_frameinfo.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_FRAMEINFO_H__
#define __ISP_FRAMEINFO_H__

#include "hi_comm_isp_adapt.h"
#include "hi_comm_video_adapt.h"
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_isp_frame_info *isp_frame;
} isp_frame_info_ctrl;

typedef struct {
    hi_isp_attach_info *attach_info;
} isp_attach_info_ctrl;

typedef struct {
    hi_isp_colorgammut_info *color_gamut_info;
} isp_gammut_info_ctrl;

typedef struct {
    isp_pro_nr_param *pro_nr_param;
} isp_pro_nr_param_ctrl;

typedef struct {
    isp_pro_shp_param *pro_shp_param;
} isp_pro_shp_param_ctrl;

hi_s32 isp_trans_info_init(hi_vi_pipe vi_pipe, isp_trans_info_buf *trans_info);
hi_void isp_trans_info_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_frame_info_init(hi_vi_pipe vi_pipe);
hi_void isp_frame_info_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_attach_info_init(hi_vi_pipe vi_pipe);
hi_void isp_attach_info_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_color_gamut_info_init(hi_vi_pipe vi_pipe);
hi_void isp_color_gamut_info_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_pro_info_init(hi_vi_pipe vi_pipe, isp_pro_info_buf *pro_info);
hi_void isp_pro_info_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_pro_nr_param_init(hi_vi_pipe vi_pipe);
hi_void isp_pro_nr_param_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_pro_shp_param_init(hi_vi_pipe vi_pipe);
hi_void isp_pro_shp_param_exit(hi_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
