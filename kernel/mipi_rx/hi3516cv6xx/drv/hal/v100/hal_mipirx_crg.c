/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_comm.h"
#include "hal_mipirx_comm.h"
#include "hal_mipirx_sys_reg.h"
#include "hal_mipirx_crg.h"

td_void hal_mipirx_crg_enable_sys_clock(td_bool enable)
{
    td_u32 *pixel_crg_addr = hal_mipirx_get_pixel_crg_addr();
    if (enable) {
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_CAL_CKEN_M, 1);
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_CBAR_CKEN_M, 1);
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_CIL_CKEN_M, 1);
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_MIPI_BUS_CKEN_M, 1);
        // reset and unreset mipi_bus_srst_req
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_MIPI_BUS_SRST_REQ_M, 1);
        osal_udelay(10); /* delay 10us */
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_MIPI_BUS_SRST_REQ_M, 0);
    } else {
        hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_CLK_RST, CFG_MIPI_BUS_SRST_REQ_M, 1);
    }
}

td_void hal_mipirx_crg_enable_pix_clock(td_u8 port_id, td_bool enable)
{
    td_u32 *pixel_crg_addr = hal_mipirx_get_pixel_crg_addr();
    hal_mipirx_reg_writeb(pixel_crg_addr, REG_MIPI_RX_PIX_CLK_RST + port_id * PORT_OFFSET, CFG_MIPI_PIX_CKEN_M, enable);
}

td_bool hal_mipirx_crg_get_pix_clock_stat(td_u8 port_id)
{
    td_u32 *pixel_crg_addr = hal_mipirx_get_pixel_crg_addr();
    return hal_mipirx_reg_readb(pixel_crg_addr, REG_MIPI_RX_PIX_CLK_RST + port_id * PORT_OFFSET, CFG_MIPI_PIX_CKEN_M) ==
           1;
}

td_void hal_mipirx_crg_reset_pix_clock(td_u8 port_id, td_bool reset)
{
    td_u32 *pixel_crg_addr = hal_mipirx_get_pixel_crg_addr();
    hal_mipirx_reg_writeb(pixel_crg_addr,
        REG_MIPI_RX_PIX_CLK_RST + port_id * PORT_OFFSET, CFG_MIPI_PIX_CORE_SRST_REQ_M, reset);
}
