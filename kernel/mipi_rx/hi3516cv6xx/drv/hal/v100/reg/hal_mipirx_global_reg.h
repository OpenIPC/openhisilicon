/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_GLOBAL_REG_H
#define HAL_MIPIRX_GLOBAL_REG_H

#define REG_HS_MODE_SELECT                          0x0000
#define CFG_HS_MODE_M                               mipirx_bits(0, 3)

#define REG_D_P_SWAP                                0x0004
#define CFG_PHY3_CK2_SWAP_M                         mipirx_bit(29)
#define CFG_PHY3_C2_SWAP_M                          mipirx_bit(28)
#define CFG_PHY3_D3_SWAP_M                          mipirx_bit(27)
#define CFG_PHY3_D2_SWAP_M                          mipirx_bit(26)
#define CFG_PHY3_D1_SWAP_M                          mipirx_bit(25)
#define CFG_PHY3_D0_SWAP_M                          mipirx_bit(24)
#define CFG_PHY2_CK2_SWAP_M                         mipirx_bit(21)
#define CFG_PHY2_C2_SWAP_M                          mipirx_bit(20)
#define CFG_PHY2_D3_SWAP_M                          mipirx_bit(19)
#define CFG_PHY2_D2_SWAP_M                          mipirx_bit(18)
#define CFG_PHY2_D1_SWAP_M                          mipirx_bit(17)
#define CFG_PHY2_D0_SWAP_M                          mipirx_bit(16)
#define CFG_PHY1_CK2_SWAP_M                         mipirx_bit(13)
#define CFG_PHY1_C2_SWAP_M                          mipirx_bit(12)
#define CFG_PHY1_D3_SWAP_M                          mipirx_bit(11)
#define CFG_PHY1_D2_SWAP_M                          mipirx_bit(10)
#define CFG_PHY1_D1_SWAP_M                          mipirx_bit(9)
#define CFG_PHY1_D0_SWAP_M                          mipirx_bit(8)
#define CFG_PHY0_CK2_SWAP_M                         mipirx_bit(5)
#define CFG_PHY0_C2_SWAP_M                          mipirx_bit(4)
#define CFG_PHY0_D3_SWAP_M                          mipirx_bit(3)
#define CFG_PHY0_D2_SWAP_M                          mipirx_bit(2)
#define CFG_PHY0_D1_SWAP_M                          mipirx_bit(1)
#define CFG_PHY0_D0_SWAP_M                          mipirx_bit(0)

#define REG_PHY_EN                                  0x0008
#define CFG_PHY3_EN_M                               mipirx_bit(3)
#define CFG_PHY2_EN_M                               mipirx_bit(2)
#define CFG_PHY1_EN_M                               mipirx_bit(1)
#define CFG_PHY0_EN_M                               mipirx_bit(0)

#define REG_LANE_EN                                 0x000C
#define CFG_LANE15_EN_M                             mipirx_bit(15)
#define CFG_LANE14_EN_M                             mipirx_bit(14)
#define CFG_LANE13_EN_M                             mipirx_bit(13)
#define CFG_LANE12_EN_M                             mipirx_bit(12)
#define CFG_LANE11_EN_M                             mipirx_bit(11)
#define CFG_LANE10_EN_M                             mipirx_bit(10)
#define CFG_LANE9_EN_M                              mipirx_bit(9)
#define CFG_LANE8_EN_M                              mipirx_bit(8)
#define CFG_LANE7_EN_M                              mipirx_bit(7)
#define CFG_LANE6_EN_M                              mipirx_bit(6)
#define CFG_LANE5_EN_M                              mipirx_bit(5)
#define CFG_LANE4_EN_M                              mipirx_bit(4)
#define CFG_LANE3_EN_M                              mipirx_bit(3)
#define CFG_LANE2_EN_M                              mipirx_bit(2)
#define CFG_LANE1_EN_M                              mipirx_bit(1)
#define CFG_LANE0_EN_M                              mipirx_bit(0)

#define REG_PHY_CIL_CTRL                            0x0010
#define CFG_CIL7_RST_REQ_M                          mipirx_bit(15)
#define CFG_CIL6_RST_REQ_M                          mipirx_bit(14)
#define CFG_CIL5_RST_REQ_M                          mipirx_bit(13)
#define CFG_CIL4_RST_REQ_M                          mipirx_bit(12)
#define CFG_CIL3_RST_REQ_M                          mipirx_bit(11)
#define CFG_CIL2_RST_REQ_M                          mipirx_bit(10)
#define CFG_CIL1_RST_REQ_M                          mipirx_bit(9)
#define CFG_CIL0_RST_REQ_M                          mipirx_bit(8)
#define CFG_PHYCIL3_CKEN_M                          mipirx_bit(3)
#define CFG_PHYCIL2_CKEN_M                          mipirx_bit(2)
#define CFG_PHYCIL1_CKEN_M                          mipirx_bit(1)
#define CFG_PHYCIL0_CKEN_M                          mipirx_bit(0)

#define REG_PHY_LP_SELECT                           0x0014
#define CFG_PIX6_CK_SEL_M                           mipirx_bit(5)
#define CFG_PIX2_CK_SEL_M                           mipirx_bit(4)
#define CFG_PHY3_LP_CK_SEL_M                        mipirx_bit(1)
#define CFG_PHY1_LP_CK_SEL_M                        mipirx_bit(0)

#define REG_PHYCFG_MODE                             0x0018
#define CFG_PHYCIL3_1_CFG_MODE_M                    mipirx_bits(28, 30)
#define CFG_PHYCIL3_CFG_MODE_SEL_M                  mipirx_bit(27)
#define CFG_PHYCIL3_0_CFG_MODE_M                    mipirx_bits(24, 26)
#define CFG_PHYCIL2_1_CFG_MODE_M                    mipirx_bits(20, 22)
#define CFG_PHYCIL2_CFG_MODE_SEL_M                  mipirx_bit(19)
#define CFG_PHYCIL2_0_CFG_MODE_M                    mipirx_bits(16, 18)
#define CFG_PHYCIL1_1_CFG_MODE_M                    mipirx_bits(12, 34)
#define CFG_PHYCIL1_CFG_MODE_SEL_M                  mipirx_bit(11)
#define CFG_PHYCIL1_0_CFG_MODE_M                    mipirx_bits(8, 10)
#define CFG_PHYCIL0_1_CFG_MODE_M                    mipirx_bits(4, 6)
#define CFG_PHYCIL0_CFG_MODE_SEL_M                  mipirx_bit(3)
#define CFG_PHYCIL0_0_CFG_MODE_M                    mipirx_bits(0, 2)

#define REG_PHYCFG_EN                               0x001C
#define CFG_PHYCIL3_CFG_EN_M                        mipirx_bit(3)
#define CFG_PHYCIL2_CFG_EN_M                        mipirx_bit(2)
#define CFG_PHYCIL1_CFG_EN_M                        mipirx_bit(1)
#define CFG_PHYCIL0_CFG_EN_M                        mipirx_bit(0)

#define REG_CHN0_MEM_CTRL                           0x0020
#define CFG_CHN0_MEM_RMEB_M                         mipirx_bit(17)
#define CFG_CHN0_MEM_TEST1B_M                       mipirx_bit(16)
#define CFG_CHN0_MEM_RMB_M                          mipirx_bits(12, 15)
#define CFG_CHN0_MEM_RMEA_M                         mipirx_bit(9)
#define CFG_CHN0_MEM_TEST1A_M                       mipirx_bit(8)
#define CFG_CHN0_MEM_RMA_M                          mipirx_bits(4, 7)
#define CFG_CHN0_MEM_CK_GT_M                        mipirx_bit(0)

#define REG_CHN0_CLR_EN                             0x0024
#define CFG_CHN0_CLR_EN_ALIGN_M                     mipirx_bit(1)
#define CFG_CHN0_CLR_EN_LVDS_M                      mipirx_bit(0)

#define REG_CHN1_MEM_CTRL                           0x0028
#define CFG_CHN1_MEM_RMEB_M                         mipirx_bit(17)
#define CFG_CHN1_MEM_TEST1B_M                       mipirx_bit(16)
#define CFG_CHN1_MEM_RMB_M                          mipirx_bits(12, 15)
#define CFG_CHN1_MEM_RMEA_M                         mipirx_bit(9)
#define CFG_CHN1_MEM_TEST1A_M                       mipirx_bit(8)
#define CFG_CHN1_MEM_RMA_M                          mipirx_bits(4, 7)
#define CFG_CHN1_MEM_CK_GT_M                        mipirx_bit(0)

#define REG_CHN1_CLR_EN                             0x002C
#define CFG_CHN1_CLR_EN_ALIGN_M                     mipirx_bit(1)
#define CFG_CHN1_CLR_EN_LVDS_M                      mipirx_bit(0)

#define REG_CHN2_MEM_CTRL                           0x0030
#define CFG_CHN2_MEM_RMEB_M                         mipirx_bit(17)
#define CFG_CHN2_MEM_TEST1B_M                       mipirx_bit(16)
#define CFG_CHN2_MEM_RMB_M                          mipirx_bits(12, 15)
#define CFG_CHN2_MEM_RMEA_M                         mipirx_bit(9)
#define CFG_CHN2_MEM_TEST1A_M                       mipirx_bit(8)
#define CFG_CHN2_MEM_RMA_M                          mipirx_bits(4, 7)
#define CFG_CHN2_MEM_CK_GT_M                        mipirx_bit(0)

#define REG_CHN2_CLR_EN                             0x0034
#define CFG_CHN2_CLR_EN_ALIGN_M                     mipirx_bit(1)
#define CFG_CHN2_CLR_EN_LVDS_M                      mipirx_bit(0)

#define REG_CHN3_MEM_CTRL                           0x0038
#define CFG_CHN3_MEM_RMEB_M                         mipirx_bit(17)
#define CFG_CHN3_MEM_TEST1B_M                       mipirx_bit(16)
#define CFG_CHN3_MEM_RMB_M                          mipirx_bits(12, 15)
#define CFG_CHN3_MEM_RMEA_M                         mipirx_bit(9)
#define CFG_CHN3_MEM_TEST1A_M                       mipirx_bit(8)
#define CFG_CHN3_MEM_RMA_M                          mipirx_bits(4, 7)
#define CFG_CHN3_MEM_CK_GT_M                        mipirx_bit(0)

#define REG_CHN3_CLR_EN                             0x003C
#define CFG_CHN3_CLR_EN_ALIGN_M                     mipirx_bit(1)
#define CFG_CHN3_CLR_EN_LVDS_M                      mipirx_bit(0)

#define REG_MIPI_TEST_PHY                           0x0080
#define CFG_TEST_PHY_SRST_REQ_M                     mipirx_bit(4)
#define CFG_TEST_PHY_EN_M                           mipirx_bit(0)

#define REG_COLORBAR_CTRL_LINK0                     0x0100
#define CFG_COLORBAR_WIDTH_M                        mipirx_bits(4, 13)
#define CFG_COLORBAR_LINE_INIT_M                    mipirx_bit(3)
#define CFG_COLORBAR_PATTERN_M                      mipirx_bit(2)
#define CFG_COLORBAR_MODE_M                         mipirx_bit(1)
#define CFG_COLORBAR_EN_M                           mipirx_bit(0)

#define REG_COLORBAR_INIT_LINK0                     0x0104
#define CFG_COLORBAR_DATA_INC_M                     mipirx_bits(16, 27)
#define CFG_COLORBAR_DATA_INIT_M                    mipirx_bits(0, 11)

#define REG_COLORBAR_V_BLK_LINK0                    0x0108
#define CFG_COLORBAR_VERTICAL_BLK_M                 mipirx_bits(0, 19)

#define REG_COLORBAR_H_BLK_LINK0                    0x010C
#define CFG_COLORBAR_LINE_BLK_M                     mipirx_bits(0, 19)

#define REG_COLORBAR_IMG_SIZE_LINK0                 0x0110
#define CFG_COLORBAR_IMGHEIGHT_M                    mipirx_bits(16, 31)
#define CFG_COLORBAR_IMGWIDTH_M                     mipirx_bits(0, 15)

#define REG_COLORBAR_SYNC_CODE0_LINK0               0x0114
#define CFG_COLORBAR_EOF_M                          mipirx_bits(16, 27)
#define CFG_COLORBAR_SOF_M                          mipirx_bits(0, 11)

#define REG_COLORBAR_SYNC_CODE1_LINK0               0x0118
#define CFG_COLORBAR_EOL_M                          mipirx_bits(16, 27)
#define CFG_COLORBAR_SOL_M                          mipirx_bits(0, 11)

#define REG_PHY_CAL_INT                             0x0120
#define CFG_PHY_CAL_INIT1_M                         mipirx_bit(1)
#define CFG_PHY_CAL_INIT0_M                         mipirx_bit(0)

#define REG_PHY0_TEST_OUT                           0x0200
#define CFG_TEST_PHY0_DONE_M                        mipirx_bits(8, 11)
#define CFG_TEST_PHY0_COMPARE_M                     mipirx_bits(4, 7)
#define CFG_TEST_PHY0_RESAULT_M                     mipirx_bits(0, 3)

#define REG_PHY1_TEST_OUT                           0x0204
#define CFG_TEST_PHY1_DONE_M                        mipirx_bits(8, 11)
#define CFG_TEST_PHY1_COMPARE_M                     mipirx_bits(4, 7)
#define CFG_TEST_PHY1_RESAULT_M                     mipirx_bits(0, 3)

#define REG_PHY2_TEST_OUT                           0x0208
#define CFG_TEST_PHY2_DONE_M                        mipirx_bits(8, 11)
#define CFG_TEST_PHY2_COMPARE_M                     mipirx_bits(4, 7)
#define CFG_TEST_PHY2_RESAULT_M                     mipirx_bits(0, 3)

#define REG_PHY3_TEST_OUT                           0x020C
#define CFG_TEST_PHY3_DONE_M                        mipirx_bits(8, 11)
#define CFG_TEST_PHY3_COMPARE_M                     mipirx_bits(4, 7)
#define CFG_TEST_PHY3_RESAULT_M                     mipirx_bits(0, 3)

#define REG_LPDT_PH_ADDR                            0x0300
#define CFG_LPDT_PACKET_HEADER_M                    mipirx_bits(0, 31)

#define REG_LPDT_CHECK_RES_ADDR                     0x0304
#define CFG_LPDT_ECC_ERR_M                          mipirx_bit(1)
#define CFG_LPDT_CRC_ERR_M                          mipirx_bit(0)

#define REG_MIPI_INT_RAW                            0x07F0
#define CFG_INT_CHN7_RAW_M                          mipirx_bit(11)
#define CFG_INT_CHN6_RAW_M                          mipirx_bit(10)
#define CFG_INT_CHN5_RAW_M                          mipirx_bit(9)
#define CFG_INT_CHN4_RAW_M                          mipirx_bit(8)
#define CFG_INT_CHN3_RAW_M                          mipirx_bit(7)
#define CFG_INT_CHN2_RAW_M                          mipirx_bit(6)
#define CFG_INT_CHN1_RAW_M                          mipirx_bit(5)
#define CFG_INT_CHN0_RAW_M                          mipirx_bit(4)
#define CFG_INT_PHYCIL3_RAW_M                       mipirx_bit(3)
#define CFG_INT_PHYCIL2_RAW_M                       mipirx_bit(2)
#define CFG_INT_PHYCIL1_RAW_M                       mipirx_bit(1)
#define CFG_INT_PHYCIL0_RAW_M                       mipirx_bit(0)

#define REG_MIPI_INT_ST                             0x07F4
#define CFG_INT_CHN7_ST_M                           mipirx_bit(11)
#define CFG_INT_CHN6_ST_M                           mipirx_bit(10)
#define CFG_INT_CHN5_ST_M                           mipirx_bit(9)
#define CFG_INT_CHN4_ST_M                           mipirx_bit(8)
#define CFG_INT_CHN3_ST_M                           mipirx_bit(7)
#define CFG_INT_CHN2_ST_M                           mipirx_bit(6)
#define CFG_INT_CHN1_ST_M                           mipirx_bit(5)
#define CFG_INT_CHN0_ST_M                           mipirx_bit(4)
#define CFG_INT_PHYCIL3_ST_M                        mipirx_bit(3)
#define CFG_INT_PHYCIL2_ST_M                        mipirx_bit(2)
#define CFG_INT_PHYCIL1_ST_M                        mipirx_bit(1)
#define CFG_INT_PHYCIL0_ST_M                        mipirx_bit(0)

#define REG_MIPI_INT_MSK                            0x07F8
#define CFG_INT_CHN7_MASK_M                         mipirx_bit(11)
#define CFG_INT_CHN6_MASK_M                         mipirx_bit(10)
#define CFG_INT_CHN5_MASK_M                         mipirx_bit(9)
#define CFG_INT_CHN4_MASK_M                         mipirx_bit(8)
#define CFG_INT_CHN3_MASK_M                         mipirx_bit(7)
#define CFG_INT_CHN2_MASK_M                         mipirx_bit(6)
#define CFG_INT_CHN1_MASK_M                         mipirx_bit(5)
#define CFG_INT_CHN0_MASK_M                         mipirx_bit(4)
#define CFG_INT_PHYCIL3_MASK_M                      mipirx_bit(3)
#define CFG_INT_PHYCIL2_MASK_M                      mipirx_bit(2)
#define CFG_INT_PHYCIL1_MASK_M                      mipirx_bit(1)
#define CFG_INT_PHYCIL0_MASK_M                      mipirx_bit(0)

#endif
