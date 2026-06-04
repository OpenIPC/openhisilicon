/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_PHY_REG_H
#define HAL_MIPIRX_PHY_REG_H

#define REG_PHY_MODE_LINK                           0x0000
#define CFG_PHY_RG_MIPI_MODE1_M                     mipirx_bit(25)
#define CFG_PHY_RG_MIPI_MODE0_M                     mipirx_bit(24)
#define CFG_PHY_RG_EN_2L2L_M                        mipirx_bit(20)
#define CFG_PHY_RG_FACLK2_EN_M                      mipirx_bit(17)
#define CFG_PHY_RG_FACLK_EN_M                       mipirx_bit(16)
#define CFG_PHY_RG_EN_LP1_M                         mipirx_bit(13)
#define CFG_PHY_RG_EN_LP0_M                         mipirx_bit(12)
#define CFG_PHY_RG_EN_CMOS_M                        mipirx_bit(8)
#define CFG_PHY_RG_EN_CLK1_M                        mipirx_bit(5)
#define CFG_PHY_RG_EN_CLK0_M                        mipirx_bit(4)
#define CFG_PHY_RG_EN_D_M                           mipirx_bits(0, 3)

#define REG_PHY_SKEW_LINK                           0x0004
#define CFG_PHY_D3_SKEW_M                           mipirx_bits(20, 22)
#define CFG_PHY_D2_SKEW_M                           mipirx_bits(16, 18)
#define CFG_PHY_D1_SKEW_M                           mipirx_bits(12, 14)
#define CFG_PHY_D0_SKEW_M                           mipirx_bits(8, 10)
#define CFG_PHY_CLK1_SKEW_M                         mipirx_bits(4, 6)
#define CFG_PHY_CLK0_SKEW_M                         mipirx_bits(0, 2)

#define REG_PHY_LANE_SI_LINK                        0x0008
#define CFG_PHY_RG_LANE3_SI_M                       mipirx_bits(24, 31)
#define CFG_PHY_RG_LANE2_SI_M                       mipirx_bits(16, 23)
#define CFG_PHY_RG_LANE1_SI_M                       mipirx_bits(8, 15)
#define CFG_PHY_RG_LANE0_SI_M                       mipirx_bits(0, 7)

#define REG_PHY_EN_LINK                             0x000C
#define CFG_PHY_CLK2_TERM_EN_M                      mipirx_bit(13)
#define CFG_PHY_CLK_TERM_EN_M                       mipirx_bit(12)
#define CFG_PHY_D3_TERM_EN_M                        mipirx_bit(11)
#define CFG_PHY_D2_TERM_EN_M                        mipirx_bit(10)
#define CFG_PHY_D1_TERM_EN_M                        mipirx_bit(9)
#define CFG_PHY_D0_TERM_EN_M                        mipirx_bit(8)
#define CFG_PHY_DA_D_VALID_INV_M                    mipirx_bits(4, 7)
#define CFG_PHY_DA_D3_VALID_M                       mipirx_bit(3)
#define CFG_PHY_DA_D2_VALID_M                       mipirx_bit(2)
#define CFG_PHY_DA_D1_VALID_M                       mipirx_bit(1)
#define CFG_PHY_DA_D0_VALID_M                       mipirx_bit(0)

#define REG_PHY_EQ_LINK                             0x0010
#define CFG_PHY_RG_EQ_LANE3_M                       mipirx_bits(20, 23)
#define CFG_PHY_RG_EQ_LANE2_M                       mipirx_bits(16, 19)
#define CFG_PHY_RG_EQ_LANE1_M                       mipirx_bits(12, 15)
#define CFG_PHY_RG_EQ_LANE0_M                       mipirx_bits(8, 11)
#define CFG_PHY_RG_EQ_CLK2_M                        mipirx_bits(4, 7)
#define CFG_PHY_RG_EQ_CLK_M                         mipirx_bits(0, 3)

#define REG_PHY_CFG_LINK                            0x0014
#define CFG_PHY_RG_SA_EN_M                          mipirx_bit(16)
#define CFG_PHY_RG_CFG_TEST_M                       mipirx_bits(12, 15)
#define CFG_PHY_DA_IDLB_M                           mipirx_bits(8, 11)
#define CFG_PHY_DA_ULPB_M                           mipirx_bits(4, 7)
#define CFG_PHY_DA_IDLB_CLK1_M                      mipirx_bit(3)
#define CFG_PHY_DA_IDLB_CLK0_M                      mipirx_bit(2)
#define CFG_PHY_DA_ULPB_CK1_M                       mipirx_bit(1)
#define CFG_PHY_DA_ULPB_CK0_M                       mipirx_bit(0)

#define REG_PHY_DATA_LINK                           0x0018
#define CFG_PHY_DATA3_MIPI_M                        mipirx_bits(24, 31)
#define CFG_PHY_DATA2_MIPI_M                        mipirx_bits(16, 23)
#define CFG_PHY_DATA1_MIPI_M                        mipirx_bits(8, 15)
#define CFG_PHY_DATA0_MIPI_M                        mipirx_bits(0, 7)

#define REG_PHY_PH_MIPI_LINK                        0x001C
#define CFG_PHY_PH3_MIPI_M                          mipirx_bits(24, 31)
#define CFG_PHY_PH2_MIPI_M                          mipirx_bits(16, 23)
#define CFG_PHY_PH1_MIPI_M                          mipirx_bits(8, 15)
#define CFG_PHY_PH0_MIPI_M                          mipirx_bits(0, 7)

#define REG_PHY_DATA_MIPI_LINK                      0x0020
#define CFG_PHY_DATA3_MIPI_HS_M                     mipirx_bits(24, 31)
#define CFG_PHY_DATA2_MIPI_HS_M                     mipirx_bits(16, 23)
#define CFG_PHY_DATA1_MIPI_HS_M                     mipirx_bits(8, 15)
#define CFG_PHY_DATA0_MIPI_HS_M                     mipirx_bits(0, 7)

#define REG_PHY_SYNC_DCT_LINK                       0x0024
#define CFG_CIL_SYNC_DET_MODE_M                     mipirx_bit(25)
#define CFG_CIL_SPLIT_SEL_M                         mipirx_bit(24)
#define CFG_CIL_CODE_BIG_ENDIAN1_M                  mipirx_bit(20)
#define CFG_CIL_SPLIT_MODE1_M                       mipirx_bits(16, 18)
#define CFG_CIL_RAW_TYPE1_M                         mipirx_bits(12, 14)
#define CFG_CIL_CODE_BIG_ENDIAN0_M                  mipirx_bit(8)
#define CFG_CIL_SPLIT_MODE0_M                       mipirx_bits(4, 6)
#define CFG_CIL_RAW_TYPE0_M                         mipirx_bits(0, 2)

#define REG_PHY_SYNC_SOF0_LINK                      0x0030
#define CFG_CIL_SOF1_WORD4_0_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOF0_WORD4_0_M                      mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOF1_LINK                      0x0034
#define CFG_CIL_SOF1_WORD4_1_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOF0_WORD4_1_M                      mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOF2_LINK                      0x0038
#define CFG_CIL_SOF1_WORD4_2_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOF0_WORD4_2_M                      mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOF3_LINK                      0x003C
#define CFG_CIL_SOF1_WORD4_3_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOF0_WORD4_3_M                      mipirx_bits(0, 15)

#define REG_PHY_OUT_VALID_LINK                      0x0040
#define CFG_CIL_MIPI_OUT_VALID_M                    mipirx_bits(4, 7)
#define CFG_CIL_LVDS_OUT_VALID_M                    mipirx_bits(3, 0)

#define REG_PHY_DATA_LVDS_LINK                      0x0044
#define CFG_PHY_DATA3_LVDS_HS_M                     mipirx_bits(24, 31)
#define CFG_PHY_DATA2_LVDS_HS_M                     mipirx_bits(16, 23)
#define CFG_PHY_DATA1_LVDS_HS_M                     mipirx_bits(8, 15)
#define CFG_PHY_DATA0_LVDS_HS_M                     mipirx_bits(0, 7)

#define REG_PHY_DESKEW_CAL_LINK                     0x0050
#define CFG_DESKEW_CAL_8B1_M                        mipirx_bits(28, 31)
#define CFG_DESKEW_CAL_SOT_BYPASS_M                 mipirx_bits(24, 27)
#define CFG_DESKEW_CAL_RSTN_EN_M                    mipirx_bits(20, 23)
#define CFG_DESKEW_CAL_EN_M                         mipirx_bits(16, 19)
#define CFG_DESKEW_CAL_CYCLE_M                      mipirx_bits(0, 15)

#define REG_PHY_PIX_PUM_LINK                        0x0054
#define CFG_PHY_LANE13_PIX_NUM_M                    mipirx_bits(16, 31)
#define CFG_PHY_LANE02_PIX_NUM_M                    mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOL0_LINK                      0x0060
#define CFG_CIL_SOL1_WORD4_0_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOL0_WORD4_0_M                      mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOL1_LINK                      0x0064
#define CFG_CIL_SOL1_WORD4_1_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOL0_WORD4_1_M                      mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOL2_LINK                      0x0068
#define CFG_CIL_SOL1_WORD4_2_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOL0_WORD4_2_M                      mipirx_bits(0, 15)

#define REG_PHY_SYNC_SOL3_LINK                      0x006C
#define CFG_CIL_SOL1_WORD4_3_M                      mipirx_bits(16, 31)
#define CFG_CIL_SOL0_WORD4_3_M                      mipirx_bits(0, 15)

#define REG_PHY_RG_TOP_CFG_LINK                     0x00D0
#define CFG_CIL_RG_TOP_CFG1_M                       mipirx_bits(0, 15)

#define REG_CIL_TIMEOUT_LINK                        0x0100
#define CFG_CIL_TIMEOUT_EN_D_M                      mipirx_bit(31)
#define CFG_CIL_TIMEOUT_EN_CK_M                     mipirx_bit(30)
#define CFG_CIL_TIMEOUT_CYC_M                       mipirx_bits(0, 23)

#define REG_CIL_FSM0_LINK                           0x0104
#define CFG_CIL_CYC_CLK_HS_TRAIL_M                  mipirx_bits(24, 29)
#define CFG_CIL_CYC_CLK_HS_EXIT_M                   mipirx_bits(16, 21)
#define CFG_CIL_CYC_CLK_HS0_M                       mipirx_bits(8, 13)
#define CFG_CIL_CYC_CLK_LP00_M                      mipirx_bits(0, 5)

#define REG_CIL_FSM_ST0_LINK                        0x0108
#define CFG_CIL_NXT_ST_D3_M                         mipirx_bits(28, 31)
#define CFG_CIL_CUR_ST_D3_M                         mipirx_bits(24, 27)
#define CFG_CIL_NXT_ST_D2_M                         mipirx_bits(20, 23)
#define CFG_CIL_CUR_ST_D2_M                         mipirx_bits(16, 19)
#define CFG_CIL_NXT_ST_D1_M                         mipirx_bits(12, 15)
#define CFG_CIL_CUR_ST_D1_M                         mipirx_bits(8, 11)
#define CFG_CIL_NXT_ST_D0_M                         mipirx_bits(4, 7)
#define CFG_CIL_CUR_ST_D0_M                         mipirx_bits(0, 3)

#define REG_CIL_FSM_ST1_LINK                        0x010C
#define CFG_CIL_NXT_ST_CK2_M                        mipirx_bits(12, 15)
#define CFG_CIL_CUR_ST_CK2_M                        mipirx_bits(8, 11)
#define CFG_CIL_NXT_ST_CK_M                         mipirx_bits(4, 7)
#define CFG_CIL_CUR_ST_CK_M                         mipirx_bits(0, 3)

#define REG_PHY_ST0_LINK                            0x0110
#define CFG_PHYST_RG_MIPI_MODE2_M                   mipirx_bit(25)
#define CFG_PHYST_RG_MIPI_MODE_M                    mipirx_bit(24)
#define CFG_PHYST_RG_EN_2L2L_M                      mipirx_bit(20)
#define CFG_PHYST_RG_FACLK1_EN_M                    mipirx_bit(17)
#define CFG_PHYST_RG_FACLK0_EN_M                    mipirx_bit(16)
#define CFG_PHYST_RG_EN_LP1_M                       mipirx_bit(13)
#define CFG_PHYST_RG_EN_LP0_M                       mipirx_bit(12)
#define CFG_PHYST_RG_EN_CMOS_M                      mipirx_bit(8)
#define CFG_PHYST_RG_EN_CLK2_M                      mipirx_bit(5)
#define CFG_PHYST_RG_EN_CLK_M                       mipirx_bit(4)
#define CFG_PHYST_RG_EN_D_M                         mipirx_bits(0, 3)

#define REG_PHY_ST1_LINK                            0x0114
#define CFG_PHYST_AD_DIRB_D_M                       mipirx_bits(24, 27)
#define CFG_PHYST_RG_AH_DS_M                        mipirx_bits(20, 23)
#define CFG_PHYST_RG_ADV12_M                        mipirx_bits(16, 19)
#define CFG_PHYST_RG_AADH_DS_M                      mipirx_bits(12, 15)
#define CFG_PHYST_RG_DIRBI_M                        mipirx_bits(8, 11)
#define CFG_PHYST_RG_DESKEW_M                       mipirx_bits(4, 7)
#define CFG_PHYST_DA_EN_DESKEW_M                    mipirx_bits(0, 3)

#define REG_PHY_ST2_LINK                            0x0118
#define CFG_PHYST_RG_LANE3_SI_M                     mipirx_bits(12, 15)
#define CFG_PHYST_RG_LANE2_SI_M                     mipirx_bits(8, 11)
#define CFG_PHYST_RG_LANE1_SI_M                     mipirx_bits(4, 7)
#define CFG_PHYST_RG_LANE0_SI_M                     mipirx_bits(0, 3)

#define REG_PHY_ST3_LINK                            0x011C
#define CFG_PHYST_CLK2_TERM_EN_M                    mipirx_bit(13)
#define CFG_PHYST_CLK_TERM_EN_M                     mipirx_bit(12)
#define CFG_PHYST_D3_TERM_EN_M                      mipirx_bit(11)
#define CFG_PHYST_D2_TERM_EN_M                      mipirx_bit(10)
#define CFG_PHYST_D1_TERM_EN_M                      mipirx_bit(9)
#define CFG_PHYST_D0_TERM_EN_M                      mipirx_bit(8)
#define CFG_PHYST_DA_D_VALID_INV_M                  mipirx_bits(4, 7)
#define CFG_PHYST_DA_D3_VALID_M                     mipirx_bit(3)
#define CFG_PHYST_DA_D2_VALID_M                     mipirx_bit(2)
#define CFG_PHYST_DA_D1_VALID_M                     mipirx_bit(1)
#define CFG_PHYST_DA_D0_VALID_M                     mipirx_bit(0)

#define REG_PHY_ST4_LINK                            0x0120
#define CFG_PHYST_RG_EQ_LANE3_M                     mipirx_bits(20, 23)
#define CFG_PHYST_RG_EQ_LANE2_M                     mipirx_bits(16, 19)
#define CFG_PHYST_RG_EQ_LANE1_M                     mipirx_bits(12, 15)
#define CFG_PHYST_RG_EQ_LANE0_M                     mipirx_bits(8, 11)
#define CFG_PHYST_RG_EQ_CLK2_M                      mipirx_bits(4, 7)
#define CFG_PHYST_RG_EQ_CLK_M                       mipirx_bits(0, 3)

#define REG_PHY_ST5_LINK                            0x0124
#define CFG_PHYST_RG_SA_EN_M                        mipirx_bit(16)
#define CFG_PHYST_RG_TEST_M                         mipirx_bits(12, 15)
#define CFG_PHYST_DA_IDLB_M                         mipirx_bits(8, 11)
#define CFG_PHYST_DA_ULPB_M                         mipirx_bits(4, 7)
#define CFG_PHYST_DA_IDLB_CLK2_M                    mipirx_bit(3)
#define CFG_PHYST_DA_IDLB_CLK_M                     mipirx_bit(2)
#define CFG_PHYST_DA_ULPB_CK2_M                     mipirx_bit(1)
#define CFG_PHYST_DA_ULPB_CK_M                      mipirx_bit(0)

#define REG_PHY_ST_SO_LINK                          0x0128
#define CFG_PHY_AD_SO_D3_M                          mipirx_bits(24, 31)
#define CFG_PHY_AD_SO_D2_M                          mipirx_bits(16, 23)
#define CFG_PHY_AD_SO_D1_M                          mipirx_bits(8, 15)
#define CFG_PHY_AD_SO_D0_M                          mipirx_bits(0, 7)

#define REG_CIL_FSM1_LINK                           0x012C
#define CFG_CIL_CYC_DATA_HS_TRAIL_M                 mipirx_bits(24, 29)
#define CFG_CIL_CYC_DATA_HS_EXIT_M                  mipirx_bits(16, 21)
#define CFG_CIL_CYC_DATA_HS0_M                      mipirx_bits(8, 13)
#define CFG_CIL_CYC_DATA_LP00_M                     mipirx_bits(0, 5)

#define REG_SKEW_CAL_LEPGTH0_LINK                   0x0130
#define CFG_SKEW_CAL_LENGTH1_M                      mipirx_bits(16, 31)
#define CFG_SKEW_CAL_LENGTH0_M                      mipirx_bits(0, 15)

#define REG_SKEW_CAL_LEPGTH1_LINK                   0x0134
#define CFG_SKEW_CAL_LENGTH1_M                      mipirx_bits(16, 31)
#define CFG_SKEW_CAL_LENGTH0_M                      mipirx_bits(0, 15)

#define REG_FREQ_MEASURE                            0x0150
#define CFG_HS_CNT_M                                mipirx_bits(16, 31)
#define CFG_CIL_CNT_M                               mipirx_bits(0, 15)

#define REG_PHY_EXT_REG0                            0x0160
#define CFG_PHY_EXT_RG_EQ_CLK2_M                    mipirx_bits(27, 29)
#define CFG_PHY_EXT_RG_EQ_CLK_M                     mipirx_bits(24, 26)
#define CFG_PHY_EXT_RG_EQ_LANE3_M                   mipirx_bits(21, 23)
#define CFG_PHY_EXT_RG_EQ_LANE2_M                   mipirx_bits(18, 20)
#define CFG_PHY_EXT_RG_EQ_LANE1_M                   mipirx_bits(15, 17)
#define CFG_PHY_EXT_RG_EQ_LANE0_M                   mipirx_bits(12, 14)
#define CFG_PHY_EXT_RG_LANE3_SI_M                   mipirx_bits(10, 11)
#define CFG_PHY_EXT_RG_LANE2_SI_M                   mipirx_bits(8, 9)
#define CFG_PHY_EXT_RG_LANE1_SI_M                   mipirx_bits(6, 7)
#define CFG_PHY_EXT_RG_LANE0_SI_M                   mipirx_bits(4, 5)
#define CFG_RG_CLK_MODECK_M                         mipirx_bit(3)
#define CFG_RG_CMP_MOD_M                            mipirx_bit(2)
#define CFG_DA_KOFS_DPHY_EN_M                       mipirx_bit(1)
#define CFG_RG_EN_IDAC_CAL_OV_M                     mipirx_bit(0)

#define REG_PHY_EXT_REG1                            0x0164
#define CFG_RG_BG_TEST_MIPI_M                       mipirx_bits(22, 29)
#define CFG_RG_AD12_M                               mipirx_bits(18, 21)
#define CFG_RG_IDAC_CAL_IN_M                        mipirx_bits(4, 9)

#define REG_PHY_TEST                                0x0168
#define CFG_PHY_RG_TEST_M                           mipirx_bits(16, 27)
#define CFG_PHY_RG_TEST_TOP_M                       mipirx_bits(0, 15)

#define REG_PHY_DSW                                 0x016C
#define CFG_PHY_RG_LANE3_DSW_M                      mipirx_bits(24, 29)
#define CFG_PHY_RG_LANE2_DSW_M                      mipirx_bits(16, 21)
#define CFG_PHY_RG_LANE1_DSW_M                      mipirx_bits(8, 13)
#define CFG_PHY_RG_LANE0_DSW_M                      mipirx_bits(0, 5)

#define REG_PHY_OFFNN                               0x0170
#define CFG_PHY_DA_CLK2_OFFNN_M                     mipirx_bits(20, 23)
#define CFG_PHY_DA_CLK_OFFNN_M                      mipirx_bits(16, 19)
#define CFG_PHY_DA_LANE3_OFFNN_M                    mipirx_bits(12, 15)
#define CFG_PHY_DA_LANE2_OFFNN_M                    mipirx_bits(8, 11)
#define CFG_PHY_DA_LANE1_OFFNN_M                    mipirx_bits(4, 7)
#define CFG_PHY_DA_LANE0_OFFNN_M                    mipirx_bits(0, 3)

#define REG_PHY_OFFNP                               0x0174
#define CFG_PHY_DA_CLK2_OFFNP_M                     mipirx_bits(20, 23)
#define CFG_PHY_DA_CLK_OFFNP_M                      mipirx_bits(16, 19)
#define CFG_PHY_DA_LANE3_OFFNP_M                    mipirx_bits(12, 15)
#define CFG_PHY_DA_LANE2_OFFNP_M                    mipirx_bits(8, 11)
#define CFG_PHY_DA_LANE1_OFFNP_M                    mipirx_bits(4, 7)
#define CFG_PHY_DA_LANE0_OFFNP_M                    mipirx_bits(0, 3)

#define REG_PHY_BIST_CUR                            0x0178
#define CFG_PHY_RG_SEL_BIST_CUR_CLK2_M              mipirx_bits(20, 23)
#define CFG_PHY_RG_SEL_BIST_CUR_CLK_M               mipirx_bits(16, 19)
#define CFG_PHY_RG_SEL_BIST_CUR_LANE3_M             mipirx_bits(12, 15)
#define CFG_PHY_RG_SEL_BIST_CUR_LANE2_M             mipirx_bits(8, 11)
#define CFG_PHY_RG_SEL_BIST_CUR_LANE1_M             mipirx_bits(4, 7)
#define CFG_PHY_RG_SEL_BIST_CUR_LANE0_M             mipirx_bits(0, 3)

#define REG_PHY_BIST_SOR                            0x017C
#define CFG_PHY_RG_SEL_BIST_SOR_CLK2_M              mipirx_bits(20, 23)
#define CFG_PHY_RG_SEL_BIST_SOR_CLK_M               mipirx_bits(16, 19)
#define CFG_PHY_RG_SEL_BIST_SOR_LANE3_M             mipirx_bits(12, 15)
#define CFG_PHY_RG_SEL_BIST_SOR_LANE2_M             mipirx_bits(8, 11)
#define CFG_PHY_RG_SEL_BIST_SOR_LANE1_M             mipirx_bits(4, 7)
#define CFG_PHY_RG_SEL_BIST_SOR_LANE0_M             mipirx_bits(0, 3)

#define REG_PHY_TEST_HS1                            0x0180
#define CFG_PHY_RG_LANE3_TEST_HS_M                  mipirx_bits(21, 27)
#define CFG_PHY_RG_LANE2_TEST_HS_M                  mipirx_bits(14, 20)
#define CFG_PHY_RG_LANE1_TEST_HS_M                  mipirx_bits(7, 13)
#define CFG_PHY_RG_LANE0_TEST_HS_M                  mipirx_bits(0, 6)

#define REG_PHY_TEST_HS2                            0x0184
#define CFG_PHY_RG_CLK2_TEST_HS_M                   mipirx_bits(7, 13)
#define CFG_PHY_RG_CLK_TEST_HS_M                    mipirx_bits(0, 6)

#define REG_PHY_TEMRL                               0x0188
#define CFG_PHY_RG_TERML_CLK2_M                     mipirx_bits(20, 23)
#define CFG_PHY_RG_TERML_CLK_M                      mipirx_bits(16, 19)
#define CFG_PHY_RG_TERML_LANE3_M                    mipirx_bits(12, 15)
#define CFG_PHY_RG_TERML_LANE2_M                    mipirx_bits(8, 11)
#define CFG_PHY_RG_TERML_LANE1_M                    mipirx_bits(4, 7)
#define CFG_PHY_RG_TERML_LANE0_M                    mipirx_bits(0, 3)

#define REG_PHY_DELAY_MEASURE                       0x018C
#define CFG_PHY_RG_DELAY_SEL_M                      mipirx_bits(1, 2)
#define CFG_PHY_RG_EN_DELAY_M                       mipirx_bit(0)

#define REG_PHY_OFSOUT                              0x0190
#define CFG_PHY_AD_CAL_END_DPHY_M                   mipirx_bits(6, 11)

#define REG_PHY_DELAY_CODE                          0x0194
#define CFG_PHY_AD_DELAY_CODE_M                     mipirx_bits(0, 7)

#define REG_PHY_EXT_REG2                            0x0198
#define CFG_PHY_RG_EN_BIST_M                        mipirx_bits(0, 5)

#define REG_MIPI_CIL_INT_RAW_LINK                   0x01F0
#define CFG_SOT_DETC_D3_RAW_M                       mipirx_bit(19)
#define CFG_SOT_DETC_D2_RAW_M                       mipirx_bit(18)
#define CFG_SOT_DETC_D1_RAW_M                       mipirx_bit(17)
#define CFG_SOT_DETC_D0_RAW_M                       mipirx_bit(16)
#define CFG_ERR_TIMEOUT_CK_RAW_M                    mipirx_bit(12)
#define CFG_ERR_TIMEOUT_D3_RAW_M                    mipirx_bit(11)
#define CFG_ERR_TIMEOUT_D2_RAW_M                    mipirx_bit(10)
#define CFG_ERR_TIMEOUT_D1_RAW_M                    mipirx_bit(9)
#define CFG_ERR_TIMEOUT_D0_RAW_M                    mipirx_bit(8)
#define CFG_ERR_ESCAPE_CK_RAW_M                     mipirx_bit(4)
#define CFG_ERR_ESCAPE_D3_RAW_M                     mipirx_bit(3)
#define CFG_ERR_ESCAPE_D2_RAW_M                     mipirx_bit(2)
#define CFG_ERR_ESCAPE_D1_RAW_M                     mipirx_bit(1)
#define CFG_ERR_ESCAPE_D0_RAW_M                     mipirx_bit(0)

#define REG_MIPI_CIL_INT_LINK                       0x01F4
#define CFG_SOT_DETC_D3_ST_M                        mipirx_bit(19)
#define CFG_SOT_DETC_D2_ST_M                        mipirx_bit(18)
#define CFG_SOT_DETC_D1_ST_M                        mipirx_bit(17)
#define CFG_SOT_DETC_D0_ST_M                        mipirx_bit(16)
#define CFG_ERR_TIMEOUT_CK_ST_M                     mipirx_bit(12)
#define CFG_ERR_TIMEOUT_D3_ST_M                     mipirx_bit(11)
#define CFG_ERR_TIMEOUT_D2_ST_M                     mipirx_bit(10)
#define CFG_ERR_TIMEOUT_D1_ST_M                     mipirx_bit(9)
#define CFG_ERR_TIMEOUT_D0_ST_M                     mipirx_bit(8)
#define CFG_ERR_ESCAPE_CK_ST_M                      mipirx_bit(4)
#define CFG_ERR_ESCAPE_D3_ST_M                      mipirx_bit(3)
#define CFG_ERR_ESCAPE_D2_ST_M                      mipirx_bit(2)
#define CFG_ERR_ESCAPE_D1_ST_M                      mipirx_bit(1)
#define CFG_ERR_ESCAPE_D0_ST_M                      mipirx_bit(0)

#define REG_MIPI_CIL_INT_MSK_LINK                   0x01F8
#define CFG_SOT_DETC_D3_MSK_M                       mipirx_bit(19)
#define CFG_SOT_DETC_D2_MSK_M                       mipirx_bit(18)
#define CFG_SOT_DETC_D1_MSK_M                       mipirx_bit(17)
#define CFG_SOT_DETC_D0_MSK_M                       mipirx_bit(16)
#define CFG_ERR_TIMEOUT_CK_MSK_M                    mipirx_bit(12)
#define CFG_ERR_TIMEOUT_D3_MSK_M                    mipirx_bit(11)
#define CFG_ERR_TIMEOUT_D2_MSK_M                    mipirx_bit(10)
#define CFG_ERR_TIMEOUT_D1_MSK_M                    mipirx_bit(9)
#define CFG_ERR_TIMEOUT_D0_MSK_M                    mipirx_bit(8)
#define CFG_ERR_ESCAPE_CK_MSK_M                     mipirx_bit(4)
#define CFG_ERR_ESCAPE_D3_MSK_M                     mipirx_bit(3)
#define CFG_ERR_ESCAPE_D2_MSK_M                     mipirx_bit(2)
#define CFG_ERR_ESCAPE_D1_MSK_M                     mipirx_bit(1)
#define CFG_ERR_ESCAPE_D0_MSK_M                     mipirx_bit(0)

#endif
