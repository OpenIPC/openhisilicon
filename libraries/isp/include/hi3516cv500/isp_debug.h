/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_debug.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DEBUG_H__
#define __ISP_DEBUG_H__

#include <stdio.h>
#include "hi_type.h"
#include "hi_errno.h"
#include "hi_comm_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_bool debug_en;
    hi_u64  phy_addr;
    hi_u32  depth;

    hi_u32  size;
    hi_isp_dbg_attr   *dbg_attr;
    hi_isp_dbg_status *dbg_status;
} isp_dbg_ctrl;

hi_s32 isp_dbg_set(hi_vi_pipe vi_pipe, const hi_isp_debug_info *dbg_info);
hi_s32 isp_dbg_get(hi_vi_pipe vi_pipe, hi_isp_debug_info *dbg_info);
hi_s32 isp_dbg_run_bgn(isp_dbg_ctrl *dbg, hi_u32 frm_cnt);
hi_s32 isp_dbg_run_end(isp_dbg_ctrl *dbg, hi_u32 frm_cnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __HISI_ISP_DEBUG_H__ */
