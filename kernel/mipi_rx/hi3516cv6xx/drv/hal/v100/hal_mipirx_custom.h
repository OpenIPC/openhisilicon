/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_CUSTOM_H
#define HAL_MIPIRX_CUSTOM_H

#define MIPIRX_PORT_NUM_MAX 2
#define MIPIRX_PHY_NUM_MAX 1
#define MIPIRX_TOTAL_LANE_NUM_MAX 4
#define MIPIRX_ONE_PHY_LANE_NUM 4
#define MIPIRX_VC_NUM_MAX 2

#define MIPIRX_MIN_WIDTH 120
#define MIPIRX_MIN_HEIGHT 88
#define MIPIRX_MAX_WIDTH 4096
#define MIPIRX_MAX_HEIGHT 4096

#define MIPIRX_PHY_SKEW_LINK_VAL 0x00f0ff00
#define MIPIRX_PHY_LANE_SI_LINK_VAL 0xffffffff
#define MIPIRX_PHY_EQ_LINK_VAL 0x00444444
#define MIPIRX_PHY_CFG_LINK 0x0001ffff
#define MIPIRX_PHY_DESKEW_CAL_LINK_VAL 0x00f03fff
#define MIPIRX_CIL_FSM0_LINK_VAL 0x000d1d0c
#define MIPIRX_PHY_EXT_REG0_VAL 0x12492000
#define MIPIRX_PHY_EXT_REG1_VAL 0x04000000
#define MIPIRX_PHY_TEST_VAL 0x00000004
#define MIPIRX_PHY_TEST_EXT_VAL  0x00000040
#define MIPIRX_PHY_DSW_VAL 0x1f1f1f1f
#define MIPIRX_PHY_OFFNN_VAL 0x00ffffff
#define MIPIRX_PHY_TEMRL_VAL 0x00999999

#define MIPIRX_REG_BASE_FROM_DTSI 0
#define MIPIRX_GLOBAL_REG_BASE 0x173C0800
#define MIPIRX_PIXEL_CRG_REG_BASE 0x11018540
#define MIPIRX_MISC_REG_BASE 0x17950000
#define MIPIRX_CTRL0_REG_BASE 0x173C1000
#define MIPIRX_CTRL1_REG_BASE 0x173C2000
#define MIPIRX_SNS0_REG_BASE 0x11018440
#define MIPIRX_SNS1_REG_BASE 0x11018460
#define MIPIRX_PHY0_REG_BASE 0x173C0000

#define MIPIRX_FPGA_PHY_REG_BASE 0x11070000
#define MIPIRX_FPGA_UMAP7_REG_BASE 0x11042000
#define MIPIRX_FPGA_CTRL_REG_BASE 0x17940000
#define MIPIRX_FPGA_CRG_REG_BASE 0x11014480

#define REG_FPGA_SENSOR_CFG                         0x0004
#define CFG_SENSOR3_CLK_SEL_M                       mipirx_bits(17, 19)
#define CFG_SENSOR2_CLK_SEL_M                       mipirx_bits(14, 16)
#define CFG_SENSOR1_CLK_SEL_M                       mipirx_bits(11, 13)
#define CFG_SENSOR0_CLK_SEL_M                       mipirx_bits(8, 10)
#define CFG_SENSOR3_CTRL_SRST_REQ_M                 mipirx_bit(7)
#define CFG_SENSOR2_CTRL_SRST_REQ_M                 mipirx_bit(6)
#define CFG_SENSOR1_CTRL_SRST_REQ_M                 mipirx_bit(5)
#define CFG_SENSOR0_CTRL_SRST_REQ_M                 mipirx_bit(4)
#define CFG_SENSOR3_SRST_REQ_M                      mipirx_bit(3)
#define CFG_SENSOR2_SRST_REQ_M                      mipirx_bit(2)
#define CFG_SENSOR1_SRST_REQ_M                      mipirx_bit(1)
#define CFG_SENSOR0_SRST_REQ_M                      mipirx_bit(0)

#define REG_FPGA_MIPIRX_CFG                         0x0008
#define CFG_FPGA_CAL_CKEN_M                         mipirx_bit(5)
#define CFG_FPGA_CBAR_CKEN_M                        mipirx_bit(4)
#define CFG_FPGA_CIL_CKEN_M                         mipirx_bit(3)
#define CFG_FPGA_MIPI_BUS_CKEN_M                    mipirx_bit(2)
#define CFG_MIPI_PIX1_CKEN_M                        mipirx_bit(1)
#define CFG_MIPI_PIX0_CKEN_M                        mipirx_bit(0)


#endif