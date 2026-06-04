/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_comm.h"
#include "hal_mipirx_comm.h"
#include "hal_mipirx_sensor_reg.h"
#include "hal_mipirx_sensor.h"

td_void hal_mipirx_sensor_enable_clock(td_u32 port_id, td_bool enable)
{
    td_void *sns_addr = hal_mipirx_get_sns_addr(port_id);
    hal_mipirx_reg_writeb(sns_addr, REG_SENSOR_CLK_RST, CFG_SENSOR_CKEN_M, enable);
}

td_bool hal_mipirx_get_sensor_clk_stat(td_u32 port_id)
{
    td_void *sns_addr = hal_mipirx_get_sns_addr(port_id);
    return hal_mipirx_reg_readb(sns_addr, REG_SENSOR_CLK_RST, CFG_SENSOR_CKEN_M) == 1;
}

td_void hal_mipirx_sensor_reset_clock(td_u32 port_id, td_bool reset)
{
    td_void *sns_addr = hal_mipirx_get_sns_addr(port_id);
    hal_mipirx_reg_writeb(sns_addr, REG_SENSOR_CLK_RST, CFG_SENSOR_CTRL_SRST_REQ_M, reset);
    hal_mipirx_reg_writeb(sns_addr, REG_SENSOR_CLK_RST, CFG_SENSOR_SRST_REQ_M, reset);
}
