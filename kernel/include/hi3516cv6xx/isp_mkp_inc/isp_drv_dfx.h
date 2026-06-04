/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DRV_DFX_H
#define ISP_DRV_DFX_H

#include "isp.h"

td_void isp_dfx_global_enable_init(td_void);

td_void isp_dfx_ctx_init(ot_vi_pipe vi_pipe);
td_void isp_dfx_ctx_deinit(ot_vi_pipe vi_pipe);

td_void isp_dfx_sns_sync_cfg_show(const isp_drv_ctx *drv_ctx, td_u8 cur_node_idx, td_u8 stay_cnt, td_u32 update_pos);
td_void isp_dfx_sns_sync_verify_show(const isp_drv_ctx *drv_ctx, isp_stat_info *stat_info);
#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_void isp_drv_dfx_check_kol_int_num(ot_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, td_u32 wch_int_status,
    isp_interrupt_sch *int_sch);
#endif
#endif
