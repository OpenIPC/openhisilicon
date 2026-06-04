/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_SYS_REG_H
#define HAL_MIPIRX_SYS_REG_H

// system control reg
#define REG_MIPI_WORK_MODE                          0x0034
#define CFG_MIPI1_WORK_MODE_M                       mipirx_bits(4, 5)
#define CFG_MIPI0_WORK_MODE_M                       mipirx_bits(0, 1)

#define MIPI_RX_REGS_ADDR                           0x073C0000

// mipirx pixel crg
#define REG_MIPI_RX_CLK_RST                         0x0
#define CFG_MIPI_HS3_PCTRL_M                        mipirx_bit(23)
#define CFG_MIPI_HS2_PCTRL_M                        mipirx_bit(22)
#define CFG_MIPI_HS1_PCTRL_M                        mipirx_bit(21)
#define CFG_MIPI_HS0_PCTRL_M                        mipirx_bit(20)
#define CFG_MIPI_BUS_CKEN_M                         mipirx_bit(5)
#define CFG_CIL_CKEN_M                              mipirx_bit(4)
#define CFG_CBAR_CKEN_M                             mipirx_bit(3)
#define CFG_CAL_CKEN_M                              mipirx_bit(2)
#define CFG_MIPI_BUS_SRST_REQ_M                     mipirx_bit(0)

#define REG_MIPI_RX_PIX_CLK_RST                     0x20
#define CFG_MIPI_PIX_CKEN_M                         mipirx_bit(4)
#define CFG_MIPI_PIX_CORE_SRST_REQ_M                mipirx_bit(0)

#define PORT_OFFSET 0x20
#endif