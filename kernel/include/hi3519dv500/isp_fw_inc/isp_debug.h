/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DEBUG_H
#define ISP_DEBUG_H

#include <stdio.h>
#include "ot_type.h"
#include "ot_errno.h"
#include "ot_common_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    td_bool debug_en;
    td_u64  phy_addr;
    td_u32  depth;

    td_u32  size;
    ot_isp_debug_attr   *dbg_attr;
    ot_isp_debug_status *dbg_status;
} isp_dbg_ctrl;

td_s32 isp_dbg_reg_set(ot_vi_pipe vi_pipe, const ot_isp_debug_info *dbg_info);
td_s32 isp_dbg_reg_get(ot_vi_pipe vi_pipe, ot_isp_debug_info *dbg_info);

td_s32 isp_blc_dbg_set(ot_vi_pipe vi_pipe, const ot_isp_debug_info *dbg_info);
td_s32 isp_blc_dbg_get(ot_vi_pipe vi_pipe, ot_isp_debug_info *dbg_info);
td_s32 isp_blc_dbg_run_bgn(isp_dbg_ctrl *dbg, td_u32 frm_cnt);
td_s32 isp_blc_dbg_run_end(ot_vi_pipe vi_pipe, isp_dbg_ctrl *dbg, td_u32 frm_cnt);

td_s32 isp_ae_dbg_set(ot_vi_pipe vi_pipe, ot_isp_debug_info *dbg_info);
td_s32 isp_awb_dbg_set(ot_vi_pipe vi_pipe, ot_isp_debug_info *dbg_info);
td_s32 isp_dehaze_dbg_set(ot_vi_pipe vi_pipe, ot_isp_debug_info *dbg_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif

