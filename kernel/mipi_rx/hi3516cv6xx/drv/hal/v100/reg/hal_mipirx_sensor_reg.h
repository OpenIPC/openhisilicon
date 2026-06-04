/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_CTRL_REG_H
#define HAL_MIPIRX_CTRL_REG_H

#define REG_SENSOR_CLK_RST                          0x0
#define CFG_SENSOR_CKSEL_M                          mipirx_bits(12, 15)
#define CFG_SENSOR_CKEN_M                           mipirx_bit(4)
#define CFG_SENSOR_CTRL_SRST_REQ_M                  mipirx_bit(1)
#define CFG_SENSOR_SRST_REQ_M                       mipirx_bit(0)

#endif