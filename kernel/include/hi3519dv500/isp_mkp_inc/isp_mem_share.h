/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_MEM_SHARE_H
#define ISP_MEM_SHARE_H

#include "ot_type.h"
#include "ot_common.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 isp_drv_mem_share_owner(ot_vi_pipe vi_pipe);
td_s32 isp_drv_mem_share_pid(ot_vi_pipe vi_pipe, td_s32 pid);
td_s32 isp_drv_mem_unshare_pid(ot_vi_pipe vi_pipe, td_s32 pid);
td_s32 isp_drv_mem_share_all(ot_vi_pipe vi_pipe);
td_s32 isp_drv_mem_unshare_all(ot_vi_pipe vi_pipe);
td_s32 isp_drv_mem_verify_pid(ot_vi_pipe vi_pipe, td_s32 pid);
td_s32 isp_drv_mem_share_exit(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_MEM_SHARE_H */

