/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_MIPIRX_CTRL_REG_H
#define HAL_MIPIRX_CTRL_REG_H

#define REG_MIPI_INTERLACED                         0x0000
#define CFG_MIPI_CSI_INTERLACED_M                   mipirx_bit(0)

#define REG_MIPI_FRAME_NUM0                         0x0010
#define CFG_MIPI_FRAME_NUM_VC1_M                    mipirx_bits(16, 31)
#define CFG_MIPI_FRAME_NUM_VC0_M                    mipirx_bits(0, 15)

#define REG_MIPI_FRAME_NUM1                         0x0014
#define CFG_MIPI_FRAME_NUM_VC3_M                    mipirx_bits(16, 31)
#define CFG_MIPI_FRAME_NUM_VC2_M                    mipirx_bits(0, 15)

#define REG_MIPI_CRC_INTR_RAW                       0x0020
#define CFG_FRAME_S_E_NUM_MISMATCH_VC3_RAW_M        mipirx_bit(23)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC2_RAW_M        mipirx_bit(22)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC1_RAW_M        mipirx_bit(21)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC0_RAW_M        mipirx_bit(20)
#define CFG_FRAME_NUM_ERR_VC3_RAW_M                 mipirx_bit(19)
#define CFG_FRAME_NUM_ERR_VC2_RAW_M                 mipirx_bit(18)
#define CFG_FRAME_NUM_ERR_VC1_RAW_M                 mipirx_bit(17)
#define CFG_FRAME_NUM_ERR_VC0_RAW_M                 mipirx_bit(16)
#define CFG_ECC_ERR_MULT_RAW_M                      mipirx_bit(8)
#define CFG_ECC_ERR_VC3_RAW_M                       mipirx_bit(7)
#define CFG_ECC_ERR_VC2_RAW_M                       mipirx_bit(6)
#define CFG_ECC_ERR_VC1_RAW_M                       mipirx_bit(5)
#define CFG_ECC_ERR_VC0_RAW_M                       mipirx_bit(4)
#define CFG_CRC_ERR_VC3_RAW_M                       mipirx_bit(3)
#define CFG_CRC_ERR_VC2_RAW_M                       mipirx_bit(2)
#define CFG_CRC_ERR_VC1_RAW_M                       mipirx_bit(1)
#define CFG_CRC_ERR_VC0_RAW_M                       mipirx_bit(0)

#define REG_MIPI_CRC_INTR_ST                        0x0024
#define CFG_FRAME_S_E_NUM_MISMATCH_VC3_ST_M         mipirx_bit(23)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC2_ST_M         mipirx_bit(22)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC1_ST_M         mipirx_bit(21)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC0_ST_M         mipirx_bit(20)
#define CFG_FRAME_NUM_ERR_VC3_ST_M                  mipirx_bit(19)
#define CFG_FRAME_NUM_ERR_VC2_ST_M                  mipirx_bit(18)
#define CFG_FRAME_NUM_ERR_VC1_ST_M                  mipirx_bit(17)
#define CFG_FRAME_NUM_ERR_VC0_ST_M                  mipirx_bit(16)
#define CFG_ECC_ERR_MULT_ST_M                       mipirx_bit(8)
#define CFG_ECC_ERR_VC3_ST_M                        mipirx_bit(7)
#define CFG_ECC_ERR_VC2_ST_M                        mipirx_bit(6)
#define CFG_ECC_ERR_VC1_ST_M                        mipirx_bit(5)
#define CFG_ECC_ERR_VC0_ST_M                        mipirx_bit(4)
#define CFG_CRC_ERR_VC3_ST_M                        mipirx_bit(3)
#define CFG_CRC_ERR_VC2_ST_M                        mipirx_bit(2)
#define CFG_CRC_ERR_VC1_ST_M                        mipirx_bit(1)
#define CFG_CRC_ERR_VC0_ST_M                        mipirx_bit(0)

#define REG_MIPI_CRC_INTR_MSK                       0x0028
#define CFG_FRAME_S_E_NUM_MISMATCH_VC3_MSK_M        mipirx_bit(23)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC2_MSK_M        mipirx_bit(22)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC1_MSK_M        mipirx_bit(21)
#define CFG_FRAME_S_E_NUM_MISMATCH_VC0_MSK_M        mipirx_bit(20)
#define CFG_FRAME_NUM_ERR_VC3_MSK_M                 mipirx_bit(19)
#define CFG_FRAME_NUM_ERR_VC2_MSK_M                 mipirx_bit(18)
#define CFG_FRAME_NUM_ERR_VC1_MSK_M                 mipirx_bit(17)
#define CFG_FRAME_NUM_ERR_VC0_MSK_M                 mipirx_bit(16)
#define CFG_ECC_ERR_MULT_MSK_M                      mipirx_bit(8)
#define CFG_ECC_ERR_VC3_MSK_M                       mipirx_bit(7)
#define CFG_ECC_ERR_VC2_MSK_M                       mipirx_bit(6)
#define CFG_ECC_ERR_VC1_MSK_M                       mipirx_bit(5)
#define CFG_ECC_ERR_VC0_MSK_M                       mipirx_bit(4)
#define CFG_CRC_ERR_VC3_MSK_M                       mipirx_bit(3)
#define CFG_CRC_ERR_VC2_MSK_M                       mipirx_bit(2)
#define CFG_CRC_ERR_VC1_MSK_M                       mipirx_bit(1)
#define CFG_CRC_ERR_VC0_MSK_M                       mipirx_bit(0)

#define REG_MIPI_USERDEF_DT                         0x0100
#define CFG_USER_DEF3_DT_M                          mipirx_bits(12, 14)
#define CFG_USER_DEF2_DT_M                          mipirx_bits(8, 10)
#define CFG_USER_DEF1_DT_M                          mipirx_bits(4, 6)
#define CFG_USER_DEF0_DT_M                          mipirx_bits(0, 2)

#define REG_MIPI_USER_DEF                           0x0104
#define CFG_USER_DEF3_M                             mipirx_bits(24, 29)
#define CFG_USER_DEF2_M                             mipirx_bits(16, 21)
#define CFG_USER_DEF1_M                             mipirx_bits(8, 13)
#define CFG_USER_DEF0_M                             mipirx_bits(0, 5)

#define REG_MIPI_CTRL_MODE_HS                       0x0108
#define CFG_LANE0_LOCATION_M                        mipirx_bits(12, 14)
#define CFG_USER_DEF_EN_M                           mipirx_bit(8)
#define CFG_VC_MODE_M                               mipirx_bit(4)
#define CFG_HDR_MODE_M                              mipirx_bit(0)

#define REG_MIPI_VHEND_DELAY                        0x010C
#define CFG_HEND_DELAY_M                            mipirx_bits(16, 31)
#define CFG_VEND_DELAY_M                            mipirx_bits(0, 15)

#define REG_MIPI_DOL_ID_CODE0                       0x0200
#define CFG_ID_CODE_REG1_M                          mipirx_bits(16, 31)
#define CFG_ID_CODE_REG0_M                          mipirx_bits(0, 15)

#define REG_MIPI_DOL_ID_CODE1                       0x0204
#define CFG_ID_CODE_REG3_M                          mipirx_bits(16, 31)
#define CFG_ID_CODE_REG2_M                          mipirx_bits(0, 15)

#define REG_MIPI_DOL_ID_CODE2                       0x0208
#define CFG_ID_CODE_REG5_M                          mipirx_bits(16, 31)
#define CFG_ID_CODE_REG4_M                          mipirx_bits(0, 15)

#define REG_MIPI_CROP_START_CHN0                    0x0210
#define CFG_MIPI_START_Y_CHN0_M                     mipirx_bits(16, 31)
#define CFG_MIPI_START_X_CHN0_M                     mipirx_bits(0, 15)

#define REG_MIPI_CROP_START_CHN1                    0x0214
#define CFG_MIPI_START_Y_CHN1_M                     mipirx_bits(16, 31)
#define CFG_MIPI_START_X_CHN1_M                     mipirx_bits(0, 15)

#define REG_MIPI_CROP_START_CHN2                    0x0218
#define CFG_MIPI_START_Y_CHN2_M                     mipirx_bits(16, 31)
#define CFG_MIPI_START_X_CHN2_M                     mipirx_bits(0, 15)

#define REG_MIPI_CROP_START_CHN3                    0x021C
#define CFG_MIPI_START_Y_CHN3_M                     mipirx_bits(16, 31)
#define CFG_MIPI_START_X_CHN3_M                     mipirx_bits(0, 15)

#define REG_MIPI_IMGSIZE                            0x0224
#define CFG_MIPI_IMGHEIGHT_M                        mipirx_bits(16, 31)
#define CFG_MIPI_IMGWIDTH_M                         mipirx_bits(0, 15)

#define REG_MIPI_CTRL_MODE_PIXEL                    0x0230
#define CFG_MIPI_YUV_420_LEGACY_EN_M                mipirx_bit(20)
#define CFG_MIPI_YUV_420_NOLEGACY_EN_M              mipirx_bit(19)
#define CFG_MIPI_YUV_422_EN_M                       mipirx_bit(18)
#define CFG_MIPI_DOUBLE_YUV_EN_M                    mipirx_bit(17)
#define CFG_MIPI_DOUBLE_PIX_EN_M                    mipirx_bit(16)
#define CFG_STAGGER_FRM_NUM_M                       mipirx_bits(13, 14)
#define CFG_STAGGER_HDR_MODE_M                      mipirx_bit(12)
#define CFG_SYNC_CLEAR_EN_M                         mipirx_bit(11)
#define CFG_DATA_MERGE_MODE_M                       mipirx_bits(9, 10)
#define CFG_RX2MODE_M                               mipirx_bit(8)
#define CFG_MIPI_DOL_MODE_M                         mipirx_bit(4)
#define CFG_MEMORY_HOLD_EN_M                        mipirx_bit(3)
#define CFG_MIPI_WDR_NUM_M                          mipirx_bits(1, 2)
#define CFG_CROP_EN_M                               mipirx_bit(0)

#define REG_MIPI_DUMMY_PIX_REG                      0x0240
#define CFG_MIPI_DUMMY_PIX_REG_M                    mipirx_bits(0, 15)

#define REG_MIPI_IMGSIZE0_STATIS                    0x0250
#define CFG_IMGHEIGHT_STATIS_VC0_M                  mipirx_bits(16, 31)
#define CFG_IMGWIDTH_STATIS_VC0_M                   mipirx_bits(0, 15)

#define REG_MIPI_IMGSIZE1_STATIS                    0x0254
#define CFG_IMGHEIGHT_STATIS_VC1_M                  mipirx_bits(16, 31)
#define CFG_IMGWIDTH_STATIS_VC1_M                   mipirx_bits(0, 15)

#define REG_MIPI_IMGSIZE2_STATIS                    0x0258
#define CFG_IMGHEIGHT_STATIS_VC2_M                  mipirx_bits(16, 31)
#define CFG_IMGWIDTH_STATIS_VC2_M                   mipirx_bits(0, 15)

#define REG_MIPI_IMGSIZE3_STATIS                    0x025C
#define CFG_IMGHEIGHT_STATIS_VC3_M                  mipirx_bits(16, 31)
#define CFG_IMGWIDTH_STATIS_VC3_M                   mipirx_bits(0, 15)

#define REG_MIPI_CTRL_INT_RAW                       0x02F0
#define CFG_INT_DFIFO_RDERR_RAW_M                   mipirx_bit(17)
#define CFG_INT_CFIFO_RDERR_RAW_M                   mipirx_bit(16)
#define CFG_CMDFIFO_NOEMPTY_RAW_M                   mipirx_bit(9)
#define CFG_MEMORY_NOEMPTY_WRITE_RAW_M              mipirx_bit(8)
#define CFG_INT_VSYNC_RAW_M                         mipirx_bit(4)
#define CFG_INT_DFIFO_WRERR_RAW_M                   mipirx_bit(1)
#define CFG_INT_CFIFO_WRERR_RAW_M                   mipirx_bit(0)

#define REG_MIPI_CTRL_INT                           0x02F4
#define CFG_INT_DFIFO_RDERR_ST_M                    mipirx_bit(17)
#define CFG_INT_CFIFO_RDERR_ST_M                    mipirx_bit(16)
#define CFG_CMDFIFO_NOEMPTY_ST_M                    mipirx_bit(9)
#define CFG_MEMORY_NOEMPTY_WRITE_ST_M               mipirx_bit(8)
#define CFG_INT_VSYNC_ST_M                          mipirx_bit(4)
#define CFG_INT_DFIFO_WRERR_ST_M                    mipirx_bit(1)
#define CFG_INT_CFIFO_WRERR_ST_M                    mipirx_bit(0)

#define REG_MIPI_CTRL_INT_MSK                       0x02F8
#define CFG_INT_DFIFO_RDERR_MSK_M                   mipirx_bit(17)
#define CFG_INT_CFIFO_RDERR_MSK_M                   mipirx_bit(16)
#define CFG_CMDFIFO_NOEMPTY_MSK_M                   mipirx_bit(9)
#define CFG_MEMORY_NOEMPTY_WRITE_MSK_M              mipirx_bit(8)
#define CFG_INT_VSYNC_MSK_M                         mipirx_bit(4)
#define CFG_INT_DFIFO_WRERR_MSK_M                   mipirx_bit(1)
#define CFG_INT_CFIFO_WRERR_MSK_M                   mipirx_bit(0)

#define REG_LVDS_WDR                                0x0300
#define CFG_LVDS_WDR_ID_SHIFT_M                     mipirx_bits(12, 15)
#define CFG_LVDS_WDR_MODE_M                         mipirx_bits(8, 11)
#define CFG_LVDS_WDR_NUM_M                          mipirx_bits(4, 5)
#define CFG_LVDS_WDR_EN_M                           mipirx_bit(0)

#define REG_LVDS_DOLSCD_HBLK                        0x0304
#define CFG_DOL_HBLANK2_M                           mipirx_bits(16, 31)
#define CFG_DOL_HBLANK1_M                           mipirx_bits(0, 15)

#define REG_LVDS_CTRL                               0x0308
#define CFG_LVDS_SPLIT_MODE_M                       mipirx_bits(16, 18)
#define CFG_LVDS_CROP_EN_M                          mipirx_bit(12)
#define CFG_LVDS_CODE_BIG_ENDIAN_M                  mipirx_bit(9)
#define CFG_LVDS_PIX_BIG_ENDIAN_M                   mipirx_bit(8)
#define CFG_LVDS_RAW_TYPE_M                         mipirx_bits(4, 6)
#define CFG_LVDS_SYNC_MODE_M                        mipirx_bit(0)

#define REG_LVDS_IMGSIZE                            0x030C
#define CFG_LVDS_IMGHEIGHT_M                        mipirx_bits(16, 31)
#define CFG_LVDS_IMGWIDTH_LANE_M                    mipirx_bits(0, 15)

#define REG_LVDS_CROP_START0                        0x0310
#define CFG_LVDS_START_Y0_M                         mipirx_bits(16, 31)
#define CFG_LVDS_START_X0_LANE_M                    mipirx_bits(0, 15)

#define REG_LVDS_CROP_START1                        0x0314
#define CFG_LVDS_START_Y1_M                         mipirx_bits(16, 31)
#define CFG_LVDS_START_X1_LANE_M                    mipirx_bits(0, 15)

#define REG_LVDS_CROP_START2                        0x0318
#define CFG_LVDS_START_Y2_M                         mipirx_bits(16, 31)
#define CFG_LVDS_START_X2_LANE_M                    mipirx_bits(0, 15)

#define REG_LVDS_CROP_START3                        0x031C
#define CFG_LVDS_START_Y3_M                         mipirx_bits(16, 31)
#define CFG_LVDS_START_X3_LANE_M                    mipirx_bits(0, 15)

#define REG_LVDS_LANE_SOF_01                        0x0320
#define CFG_LANE_SOF_1_M                            mipirx_bits(16, 31)
#define CFG_LANE_SOF_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_SOF_23                        0x0324
#define CFG_LANE_SOF_3_M                            mipirx_bits(16, 31)
#define CFG_LANE_SOF_2_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_EOF_01                        0x0328
#define CFG_LANE_EOF_1_M                            mipirx_bits(16, 31)
#define CFG_LANE_EOF_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_EOF_23                        0x032C
#define CFG_LANE_EOF_3_M                            mipirx_bits(16, 31)
#define CFG_LANE_EOF_2_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_SOL_01                        0x0330
#define CFG_LANE_SOL_1_M                            mipirx_bits(16, 31)
#define CFG_LANE_SOL_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_SOL_23                        0x0334
#define CFG_LANE_SOL_3_M                            mipirx_bits(16, 31)
#define CFG_LANE_SOL_2_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_EOL_01                        0x0338
#define CFG_LANE_EOL_1_M                            mipirx_bits(16, 31)
#define CFG_LANE_EOL_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_EOL_23                        0x033C
#define CFG_LANE_EOL_3_M                            mipirx_bits(16, 31)
#define CFG_LANE_EOL_2_M                            mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_SOF_01                    0x0520
#define CFG_LANE_NXT_SOF_1_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_SOF_0_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_SOF_23                    0x0524
#define CFG_LANE_NXT_SOF_3_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_SOF_2_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_EOF_01                    0x0528
#define CFG_LANE_NXT_EOF_1_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_EOF_0_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_EOF_23                    0x052C
#define CFG_LANE_NXT_EOF_3_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_EOF_2_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_SOL_01                    0x0530
#define CFG_LANE_NXT_SOL_1_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_SOL_0_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_SOL_23                    0x0534
#define CFG_LANE_NXT_SOL_3_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_SOL_2_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_EOL_01                    0x0538
#define CFG_LANE_NXT_EOL_1_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_EOL_0_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_NXT_EOL_23                    0x053C
#define CFG_LANE_NXT_EOL_3_M                        mipirx_bits(16, 31)
#define CFG_LANE_NXT_EOL_2_M                        mipirx_bits(0, 15)

#define REG_LVDS_LI_WORD0                           0x0720
#define CFG_LI_WORD0_1_M                            mipirx_bits(16, 31)
#define CFG_LI_WORD0_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LI_WORD1                           0x0724
#define CFG_LI_WORD1_1_M                            mipirx_bits(16, 31)
#define CFG_LI_WORD1_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LI_WORD2                           0x0728
#define CFG_LI_WORD2_1_M                            mipirx_bits(16, 31)
#define CFG_LI_WORD2_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_LI_WORD3                           0x072C
#define CFG_LI_WORD3_1_M                            mipirx_bits(16, 31)
#define CFG_LI_WORD3_0_M                            mipirx_bits(0, 15)

#define REG_LVDS_SYNC_BORD0                         0x0730
#define CFG_SYNC_BORD0_LANE3_M                      mipirx_bits(12, 15)
#define CFG_SYNC_BORD0_LANE2_M                      mipirx_bits(8, 11)
#define CFG_SYNC_BORD0_LANE1_M                      mipirx_bits(4, 7)
#define CFG_SYNC_BORD0_LANE0_M                      mipirx_bits(0, 3)

#define REG_LVDS_SYNC_BORD1                         0x0734
#define CFG_SYNC_BORD1_LANE3_M                      mipirx_bits(12, 15)
#define CFG_SYNC_BORD1_LANE2_M                      mipirx_bits(8, 11)
#define CFG_SYNC_BORD1_LANE1_M                      mipirx_bits(4, 7)
#define CFG_SYNC_BORD1_LANE0_M                      mipirx_bits(0, 3)

#define REG_LVDS_SYNC_BORD2                         0x0738
#define CFG_SYNC_BORD2_LANE3_M                      mipirx_bits(12, 15)
#define CFG_SYNC_BORD2_LANE2_M                      mipirx_bits(8, 11)
#define CFG_SYNC_BORD2_LANE1_M                      mipirx_bits(4, 7)
#define CFG_SYNC_BORD2_LANE0_M                      mipirx_bits(0, 3)

#define REG_LVDS_SYNC_BORD3                         0x073C
#define CFG_SYNC_BORD3_LANE3_M                      mipirx_bits(12, 15)
#define CFG_SYNC_BORD3_LANE2_M                      mipirx_bits(8, 11)
#define CFG_SYNC_BORD3_LANE1_M                      mipirx_bits(4, 7)
#define CFG_SYNC_BORD3_LANE0_M                      mipirx_bits(0, 3)

#define REG_LVDS_LANE_IMGSIZE_STATIS                0x0740
#define CFG_LANE_IMGHEIGHT_M                        mipirx_bits(16, 31)
#define CFG_LANE_IMGWIDTH_M                         mipirx_bits(0, 15)

#define REG_LVDS_IMGSIZE0_STATIS                    0x0780
#define CFG_LVDS_IMGHEIGHT0_M                       mipirx_bits(16, 31)
#define CFG_LVDS_IMGWIDTH0_M                        mipirx_bits(0, 15)

#define REG_LVDS_IMGSIZE1_STATIS                    0x0784
#define CFG_LVDS_IMGHEIGHT1_M                       mipirx_bits(16, 31)
#define CFG_LVDS_IMGWIDTH1_M                        mipirx_bits(0, 15)

#define REG_LVDS_IMGSIZE2_STATIS                    0x0788
#define CFG_LVDS_IMGHEIGHT2_M                       mipirx_bits(16, 31)
#define CFG_LVDS_IMGWIDTH2_M                        mipirx_bits(0, 15)

#define REG_LVDS_IMGSIZE3_STATIS                    0x078C
#define CFG_LVDS_IMGHEIGHT3_M                       mipirx_bits(16, 31)
#define CFG_LVDS_IMGWIDTH3_M                        mipirx_bits(0, 15)

#define REG_LVDS_LANE_ORDERS0                       0x0790
#define CFG_LVDS_LANE3_ID_M                         mipirx_bits(12, 15)
#define CFG_LVDS_LANE2_ID_M                         mipirx_bits(8, 11)
#define CFG_LVDS_LANE1_ID_M                         mipirx_bits(4, 7)
#define CFG_LVDS_LANE0_ID_M                         mipirx_bits(0, 3)

#define REG_LVDS_LANE_ORDERS1                       0x0794
#define CFG_LVDS_LANE7_ID_M                         mipirx_bits(12, 15)
#define CFG_LVDS_LANE6_ID_M                         mipirx_bits(8, 11)
#define CFG_LVDS_LANE5_ID_M                         mipirx_bits(4, 7)
#define CFG_LVDS_LANE4_ID_M                         mipirx_bits(0, 3)

#define REG_LVDS_LANE_ORDERS2                       0x0798
#define CFG_LVDS_LANE11_ID_M                        mipirx_bits(12, 15)
#define CFG_LVDS_LANE10_ID_M                        mipirx_bits(8, 11)
#define CFG_LVDS_LANE9_ID_M                         mipirx_bits(4, 7)
#define CFG_LVDS_LANE8_ID_M                         mipirx_bits(0, 3)

#define REG_LVDS_LANE_ORDERS3                       0x079C
#define CFG_LVDS_LANE15_ID_M                        mipirx_bits(12, 15)
#define CFG_LVDS_LANE14_ID_M                        mipirx_bits(8, 11)
#define CFG_LVDS_LANE13_ID_M                        mipirx_bits(4, 7)
#define CFG_LVDS_LANE12_ID_M                        mipirx_bits(0, 3)

#define REG_LVDS_OUTPUT_PIX_NUM                     0x07A0
#define CFG_LVDS_DOUBLE_PIX_EN_M                    mipirx_bit(0)

#define REG_LVDS_F1_EN                              0x07A4
#define CFG_LVDS_F1_EN_M                            mipirx_bit(0)

#define REG_LVDS_CTRL_INT_RAW                       0x07F0
#define CFG_LVDS_VSYNC_RAW_M                        mipirx_bit(28)
#define CFG_LVDS_STATE_ERR_RAW_M                    mipirx_bit(24)
#define CFG_LINK3_RD_ERR_RAW_M                      mipirx_bit(23)
#define CFG_LINK2_RD_ERR_RAW_M                      mipirx_bit(22)
#define CFG_LINK1_RD_ERR_RAW_M                      mipirx_bit(21)
#define CFG_LINK0_RD_ERR_RAW_M                      mipirx_bit(20)
#define CFG_LINK3_WR_ERR_RAW_M                      mipirx_bit(19)
#define CFG_LINK2_WR_ERR_RAW_M                      mipirx_bit(18)
#define CFG_LINK1_WR_ERR_RAW_M                      mipirx_bit(17)
#define CFG_LINK0_WR_ERR_RAW_M                      mipirx_bit(16)
#define CFG_LANE15_SYNC_ERR_RAW_M                   mipirx_bit(15)
#define CFG_LANE14_SYNC_ERR_RAW_M                   mipirx_bit(14)
#define CFG_LANE13_SYNC_ERR_RAW_M                   mipirx_bit(13)
#define CFG_LANE12_SYNC_ERR_RAW_M                   mipirx_bit(12)
#define CFG_LANE11_SYNC_ERR_RAW_M                   mipirx_bit(11)
#define CFG_LANE10_SYNC_ERR_RAW_M                   mipirx_bit(10)
#define CFG_LANE9_SYNC_ERR_RAW_M                    mipirx_bit(9)
#define CFG_LANE8_SYNC_ERR_RAW_M                    mipirx_bit(8)
#define CFG_LANE7_SYNC_ERR_RAW_M                    mipirx_bit(7)
#define CFG_LANE6_SYNC_ERR_RAW_M                    mipirx_bit(6)
#define CFG_LANE5_SYNC_ERR_RAW_M                    mipirx_bit(5)
#define CFG_LANE4_SYNC_ERR_RAW_M                    mipirx_bit(4)
#define CFG_LANE3_SYNC_ERR_RAW_M                    mipirx_bit(3)
#define CFG_LANE2_SYNC_ERR_RAW_M                    mipirx_bit(2)
#define CFG_LANE1_SYNC_ERR_RAW_M                    mipirx_bit(1)
#define CFG_LANE0_SYNC_ERR_RAW_M                    mipirx_bit(0)

#define REG_LVDS_CTRL_INT                           0x07F4
#define CFG_LVDS_VSYNC_ST_M                         mipirx_bit(28)
#define CFG_LVDS_STATE_ERR_ST_M                     mipirx_bit(24)
#define CFG_LINK3_RD_ERR_ST_M                       mipirx_bit(23)
#define CFG_LINK2_RD_ERR_ST_M                       mipirx_bit(22)
#define CFG_LINK1_RD_ERR_ST_M                       mipirx_bit(21)
#define CFG_LINK0_RD_ERR_ST_M                       mipirx_bit(20)
#define CFG_LINK3_WR_ERR_ST_M                       mipirx_bit(19)
#define CFG_LINK2_WR_ERR_ST_M                       mipirx_bit(18)
#define CFG_LINK1_WR_ERR_ST_M                       mipirx_bit(17)
#define CFG_LINK0_WR_ERR_ST_M                       mipirx_bit(16)
#define CFG_LANE15_SYNC_ERR_ST_M                    mipirx_bit(15)
#define CFG_LANE14_SYNC_ERR_ST_M                    mipirx_bit(14)
#define CFG_LANE13_SYNC_ERR_ST_M                    mipirx_bit(13)
#define CFG_LANE12_SYNC_ERR_ST_M                    mipirx_bit(12)
#define CFG_LANE11_SYNC_ERR_ST_M                    mipirx_bit(11)
#define CFG_LANE10_SYNC_ERR_ST_M                    mipirx_bit(10)
#define CFG_LANE9_SYNC_ERR_ST_M                     mipirx_bit(9)
#define CFG_LANE8_SYNC_ERR_ST_M                     mipirx_bit(8)
#define CFG_LANE7_SYNC_ERR_ST_M                     mipirx_bit(7)
#define CFG_LANE6_SYNC_ERR_ST_M                     mipirx_bit(6)
#define CFG_LANE5_SYNC_ERR_ST_M                     mipirx_bit(5)
#define CFG_LANE4_SYNC_ERR_ST_M                     mipirx_bit(4)
#define CFG_LANE3_SYNC_ERR_ST_M                     mipirx_bit(3)
#define CFG_LANE2_SYNC_ERR_ST_M                     mipirx_bit(2)
#define CFG_LANE1_SYNC_ERR_ST_M                     mipirx_bit(1)
#define CFG_LANE0_SYNC_ERR_ST_M                     mipirx_bit(0)

#define REG_LVDS_CTRL_INT_MSK                       0x07F8
#define CFG_LVDS_VSYNC_MSK_M                        mipirx_bit(28)
#define CFG_LVDS_STATE_ERR_MSK_M                    mipirx_bit(24)
#define CFG_LINK3_RD_ERR_MSK_M                      mipirx_bit(23)
#define CFG_LINK2_RD_ERR_MSK_M                      mipirx_bit(22)
#define CFG_LINK1_RD_ERR_MSK_M                      mipirx_bit(21)
#define CFG_LINK0_RD_ERR_MSK_M                      mipirx_bit(20)
#define CFG_LINK3_WR_ERR_MSK_M                      mipirx_bit(19)
#define CFG_LINK2_WR_ERR_MSK_M                      mipirx_bit(18)
#define CFG_LINK1_WR_ERR_MSK_M                      mipirx_bit(17)
#define CFG_LINK0_WR_ERR_MSK_M                      mipirx_bit(16)
#define CFG_LANE15_SYNC_ERR_MSK_M                   mipirx_bit(15)
#define CFG_LANE14_SYNC_ERR_MSK_M                   mipirx_bit(14)
#define CFG_LANE13_SYNC_ERR_MSK_M                   mipirx_bit(13)
#define CFG_LANE12_SYNC_ERR_MSK_M                   mipirx_bit(12)
#define CFG_LANE11_SYNC_ERR_MSK_M                   mipirx_bit(11)
#define CFG_LANE10_SYNC_ERR_MSK_M                   mipirx_bit(10)
#define CFG_LANE9_SYNC_ERR_MSK_M                    mipirx_bit(9)
#define CFG_LANE8_SYNC_ERR_MSK_M                    mipirx_bit(8)
#define CFG_LANE7_SYNC_ERR_MSK_M                    mipirx_bit(7)
#define CFG_LANE6_SYNC_ERR_MSK_M                    mipirx_bit(6)
#define CFG_LANE5_SYNC_ERR_MSK_M                    mipirx_bit(5)
#define CFG_LANE4_SYNC_ERR_MSK_M                    mipirx_bit(4)
#define CFG_LANE3_SYNC_ERR_MSK_M                    mipirx_bit(3)
#define CFG_LANE2_SYNC_ERR_MSK_M                    mipirx_bit(2)
#define CFG_LANE1_SYNC_ERR_MSK_M                    mipirx_bit(1)
#define CFG_LANE0_SYNC_ERR_MSK_M                    mipirx_bit(0)

#define REG_LANE_ID0_CHN                            0x0800
#define CFG_LANE3_ID_M                              mipirx_bits(12, 15)
#define CFG_LANE2_ID_M                              mipirx_bits(8, 11)
#define CFG_LANE1_ID_M                              mipirx_bits(4, 7)
#define CFG_LANE0_ID_M                              mipirx_bits(0, 3)

#define REG_LANE_ID1_CHN                            0x0804
#define CFG_LANE7_ID_M                              mipirx_bits(12, 15)
#define CFG_LANE6_ID_M                              mipirx_bits(8, 11)
#define CFG_LANE5_ID_M                              mipirx_bits(4, 7)
#define CFG_LANE4_ID_M                              mipirx_bits(0, 3)

#define REG_LANE_ID2_CHN                            0x0808
#define CFG_LANE11_ID_M                             mipirx_bits(12, 15)
#define CFG_LANE10_ID_M                             mipirx_bits(8, 11)
#define CFG_LANE9_ID_M                              mipirx_bits(4, 7)
#define CFG_LANE8_ID_M                              mipirx_bits(0, 3)

#define REG_LANE_ID3_CHN                            0x080C
#define CFG_LANE15_ID_M                             mipirx_bits(12, 15)
#define CFG_LANE14_ID_M                             mipirx_bits(8, 11)
#define CFG_LANE13_ID_M                             mipirx_bits(4, 7)
#define CFG_LANE12_ID_M                             mipirx_bits(0, 3)

#define REG_ALIGN_INT_RAW                           0x08F0
#define CFG_ERR_LANE15_RAW_M                        mipirx_bit(16)
#define CFG_ERR_LANE14_RAW_M                        mipirx_bit(15)
#define CFG_ERR_LANE13_RAW_M                        mipirx_bit(14)
#define CFG_ERR_LANE12_RAW_M                        mipirx_bit(13)
#define CFG_ERR_LANE11_RAW_M                        mipirx_bit(12)
#define CFG_ERR_LANE10_RAW_M                        mipirx_bit(11)
#define CFG_ERR_LANE9_RAW_M                         mipirx_bit(10)
#define CFG_ERR_LANE8_RAW_M                         mipirx_bit(9)
#define CFG_ERR_LANE7_RAW_M                         mipirx_bit(8)
#define CFG_ERR_LANE6_RAW_M                         mipirx_bit(7)
#define CFG_ERR_LANE5_RAW_M                         mipirx_bit(6)
#define CFG_ERR_LANE4_RAW_M                         mipirx_bit(5)
#define CFG_ERR_LANE3_RAW_M                         mipirx_bit(4)
#define CFG_ERR_LANE2_RAW_M                         mipirx_bit(3)
#define CFG_ERR_LANE1_RAW_M                         mipirx_bit(2)
#define CFG_ERR_LANE0_RAW_M                         mipirx_bit(1)
#define CFG_ERR_FULL_RAW_M                          mipirx_bit(0)

#define REG_ALIGN_INT                               0x08F4
#define CFG_ERR_LANE15_ST_M                         mipirx_bit(16)
#define CFG_ERR_LANE14_ST_M                         mipirx_bit(15)
#define CFG_ERR_LANE13_ST_M                         mipirx_bit(14)
#define CFG_ERR_LANE12_ST_M                         mipirx_bit(13)
#define CFG_ERR_LANE11_ST_M                         mipirx_bit(12)
#define CFG_ERR_LANE10_ST_M                         mipirx_bit(11)
#define CFG_ERR_LANE9_ST_M                          mipirx_bit(10)
#define CFG_ERR_LANE8_ST_M                          mipirx_bit(9)
#define CFG_ERR_LANE7_ST_M                          mipirx_bit(8)
#define CFG_ERR_LANE6_ST_M                          mipirx_bit(7)
#define CFG_ERR_LANE5_ST_M                          mipirx_bit(6)
#define CFG_ERR_LANE4_ST_M                          mipirx_bit(5)
#define CFG_ERR_LANE3_ST_M                          mipirx_bit(4)
#define CFG_ERR_LANE2_ST_M                          mipirx_bit(3)
#define CFG_ERR_LANE1_ST_M                          mipirx_bit(2)
#define CFG_ERR_LANE0_ST_M                          mipirx_bit(1)
#define CFG_ERR_FULL_ST_M                           mipirx_bit(0)

#define REG_ALIGN_INT_MSK                           0x08F8
#define CFG_ERR_LANE15_MASK_M                       mipirx_bit(16)
#define CFG_ERR_LANE14_MASK_M                       mipirx_bit(15)
#define CFG_ERR_LANE13_MASK_M                       mipirx_bit(14)
#define CFG_ERR_LANE12_MASK_M                       mipirx_bit(13)
#define CFG_ERR_LANE11_MASK_M                       mipirx_bit(12)
#define CFG_ERR_LANE10_MASK_M                       mipirx_bit(11)
#define CFG_ERR_LANE9_MASK_M                        mipirx_bit(10)
#define CFG_ERR_LANE8_MASK_M                        mipirx_bit(9)
#define CFG_ERR_LANE7_MASK_M                        mipirx_bit(8)
#define CFG_ERR_LANE6_MASK_M                        mipirx_bit(7)
#define CFG_ERR_LANE5_MASK_M                        mipirx_bit(6)
#define CFG_ERR_LANE4_MASK_M                        mipirx_bit(5)
#define CFG_ERR_LANE3_MASK_M                        mipirx_bit(4)
#define CFG_ERR_LANE2_MASK_M                        mipirx_bit(3)
#define CFG_ERR_LANE1_MASK_M                        mipirx_bit(2)
#define CFG_ERR_LANE0_MASK_M                        mipirx_bit(1)
#define CFG_ERR_FULL_MASK_M                         mipirx_bit(0)

#define REG_CHN_INT_RAW                             0x0FF0
#define CFG_INT_DATA_ALIGN_RAW_M                    mipirx_bit(3)
#define CFG_INT_MIPI_CTRL_RAW_M                     mipirx_bit(2)
#define CFG_INT_MIPI_CSI_RAW_M                      mipirx_bit(1)
#define CFG_INT_LVDS_CTRL_RAW_M                     mipirx_bit(0)

#define REG_CHN_INT                                 0x0FF4
#define CFG_INT_DATA_ALIGN_ST_M                     mipirx_bit(3)
#define CFG_INT_MIPI_CTRL_ST_M                      mipirx_bit(2)
#define CFG_INT_MIPI_CSI_ST_M                       mipirx_bit(1)
#define CFG_INT_LVDS_CTRL_ST_M                      mipirx_bit(0)

#define REG_CHN_INT_MASK                            0x0FF8
#define CFG_INT_DATA_ALIGN_MASK_M                   mipirx_bit(3)
#define CFG_INT_MIPI_CTRL_MASK_M                    mipirx_bit(2)
#define CFG_INT_MIPI_CSI_MASK_M                     mipirx_bit(1)
#define CFG_INT_LVDS_CTRL_MASK_M                    mipirx_bit(0)

#endif
