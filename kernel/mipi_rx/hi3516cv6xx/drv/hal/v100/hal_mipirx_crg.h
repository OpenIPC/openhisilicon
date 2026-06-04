/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_CRG_H
#define HAL_MIPIRX_CRG_H

#include "drv_mipirx_comm.h"
#include "drv_mipirx_osal.h"

td_void hal_mipirx_crg_enable_sys_clock(td_bool enable);
td_void hal_mipirx_crg_enable_pix_clock(td_u8 port_id, td_bool enable);
td_bool hal_mipirx_crg_get_pix_clock_stat(td_u8 port_id);
td_void hal_mipirx_crg_reset_pix_clock(td_u8 port_id, td_bool reset);

#endif
