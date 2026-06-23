/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DNGINFO_H
#define ISP_DNGINFO_H

#include "ot_common_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    ot_isp_dng_image_static_info *isp_dng;
} isp_dng_info_ctrl;

typedef struct {
    td_double a_color_matrix[OT_ISP_CCM_MATRIX_NUM];
    td_double d50_color_matrix[OT_ISP_CCM_MATRIX_NUM];
    td_double a_forward_matrix[OT_ISP_CCM_MATRIX_NUM];
    td_double d50_forward_matrix[OT_ISP_CCM_MATRIX_NUM];
} dng_matrix;

td_s32 isp_update_dng_image_dynamic_info(ot_vi_pipe vi_pipe);

td_s32 isp_dng_info_init(ot_vi_pipe vi_pipe);
td_void isp_dng_info_exit(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
