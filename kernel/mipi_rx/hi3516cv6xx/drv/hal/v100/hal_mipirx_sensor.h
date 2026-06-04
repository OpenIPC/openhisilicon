/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef HAL_MIPIRX_SENSOR_H
#define HAL_MIPIRX_SENSOR_H

#include "drv_mipirx_comm.h"

td_void hal_mipirx_sensor_enable_clock(td_u32 port_id, td_bool enable);
td_bool hal_mipirx_get_sensor_clk_stat(td_u32 port_id);
td_void hal_mipirx_sensor_reset_clock(td_u32 port_id, td_bool reset);

#endif
