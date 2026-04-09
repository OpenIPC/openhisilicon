/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_dcfinfo.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DCFINFO_H__
#define __ISP_DCFINFO_H__

#include "hi_comm_isp_adapt.h"
#include "hi_comm_video_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_isp_dcf_const_info  *isp_dcf_const_info;
    hi_isp_dcf_update_info *isp_update_info;
} isp_update_info_ctrl;

hi_s32 isp_update_info_init(hi_vi_pipe vi_pipe);
hi_void isp_update_info_exit(hi_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
