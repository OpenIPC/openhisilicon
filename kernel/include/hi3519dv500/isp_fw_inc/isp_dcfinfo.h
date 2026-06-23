/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DCFINFO_H
#define ISP_DCFINFO_H

#include "ot_common_isp.h"
#include "ot_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    ot_isp_dcf_const_info  *isp_dcf_const_info;
    ot_isp_dcf_update_info *isp_update_info;
} isp_update_info_ctrl;

td_s32 isp_update_info_init(ot_vi_pipe vi_pipe);
td_void isp_update_info_exit(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
