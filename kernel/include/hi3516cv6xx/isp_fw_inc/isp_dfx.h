/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DFX_H
#define ISP_DFX_H

#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_void isp_dfx_get_lock_time(isp_usr_ctx *isp_ctx);
td_void isp_dfx_get_unlock_time(isp_usr_ctx *isp_ctx);
td_void isp_dfx_get_run_begin_time(isp_usr_ctx *isp_ctx);
td_void isp_dfx_get_run_end_time(isp_usr_ctx *isp_ctx);
td_void isp_dfx_update_frame_edge_info(isp_usr_ctx *isp_ctx, isp_frame_edge_info *frame_edge_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
