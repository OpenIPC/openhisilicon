/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DFX_H
#define ISP_DFX_H

#include "isp.h"

td_void isp_dfx_global_enable_init(td_void);

td_void isp_dfx_ctx_init(ot_vi_pipe vi_pipe);
td_void isp_dfx_ctx_deinit(ot_vi_pipe vi_pipe);

td_void isp_dfx_sns_sync_cfg_show(const isp_drv_ctx *drv_ctx, td_u8 cur_node_idx, td_u8 stay_cnt, td_u32 update_pos);
td_void isp_dfx_sns_sync_verify_show(const isp_drv_ctx *drv_ctx, isp_stat_info *stat_info);

#endif
