/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_dnginfo.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DNGINFO_H__
#define __ISP_DNGINFO_H__

#include "hi_comm_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_isp_dng_image_static_info *isp_dng;
} isp_dng_info_ctrl;

typedef struct {
    hi_double a_color_matrix[CCM_MATRIX_NUM];
    hi_double d50_color_matrix[CCM_MATRIX_NUM];
    hi_double a_forward_matrix[CCM_MATRIX_NUM];
    hi_double d50_forward_matrix[CCM_MATRIX_NUM];
} dng_matrix;

hi_s32 isp_update_dng_image_dynamic_info(hi_vi_pipe vi_pipe);

hi_s32 isp_dng_info_init(hi_vi_pipe vi_pipe);
hi_void isp_dng_info_exit(hi_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
