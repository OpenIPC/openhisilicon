/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_TX_DEF_H
#define MIPI_TX_DEF_H

typedef enum {
    MIPI_TX_DSI_RGB_16BIT          = 0x00,
    MIPI_TX_DSI_RGB_18BIT          = 0x03,
    MIPI_TX_DSI_RGB_18BIT_LOOSELY  = 0x04,
    MIPI_TX_DSI_RGB_24BIT          = 0x05,
    MIPI_TX_DSI_YCBCR422_20BIT     = 0x06,
    MIPI_TX_DSI_YCBCR422_16BIT     = 0x08,
    MIPI_TX_DSI_RGB_30BIT          = 0x09,
    MIPI_TX_DSI_RGB_36BIT          = 0x0A,
    MIPI_TX_DSI_YCBCR420_12BIT     = 0x0b,
    MIPI_TX_DSC24_COMPRESSED_DATA  = 0x0f,
    MIPI_TX_CSI_YUV422_8BIT        = 0x10,
    MIPI_TX_CSI_YUV422_10BIT       = 0x11,
    MIPI_TX_CSI_LEGACY_YUV420_8BIT = 0x12,
    MIPI_TX_CSI_YUV420_8BIT        = 0x13,
    MIPI_TX_CSI_YUV420_10BIT       = 0x14,
    MIPI_TX_CSI_RGB888             = 0x15,
    MIPI_TX_CSI_RGB666             = 0x16,
    MIPI_TX_CSI_RGB565             = 0x17,
    MIPI_TX_CSI_RGB555             = 0x18,
    MIPI_TX_CSI_RGB444             = 0x19,
    MIPI_TX_CSI_RAW8               = 0x1A,
    MIPI_TX_CSI_RAW10              = 0x1B,
    MIPI_TX_CSI_RAW12              = 0x1C,
    MIPI_TX_CSI_RAW14              = 0x1D,
    MIPI_TX_CSI_RAW16              = 0x1E,

    MIPI_TX_DATA_TYPE_BUTT,
} mipi_tx_data_type;

typedef enum {
    CMD_TRANS_DCS_LW_LOWPOWER_MODE               = (0x01L << 0), /* 0: the 0 bit */
    CMD_TRANS_DCS_SR_0P_LOWPOWER_MODE            = (0x01L << 1), /* 1: the 1st bit */
    CMD_TRANS_DCS_SW_1P_LOWPOWER_MODE            = (0x01L << 2), /* 2: the 2nd bit */
    CMD_TRANS_DCS_SW_0P_LOWPOWER_MODE            = (0x01L << 3), /* 3: the 3rd bit */
    CMD_TRANS_GEN_LW_LOWPOWER_MODE               = (0x01L << 4), /* 4: the 4th bit */
    CMD_TRANS_GEN_SR_2P_LOWPOWER_MODE            = (0x01L << 5), /* 5: the 5th bit */
    CMD_TRANS_GEN_SR_1P_LOWPOWER_MODE            = (0x01L << 6), /* 6: the 6th bit */
    CMD_TRANS_GEN_SR_0P_LOWPOWER_MODE            = (0x01L << 7), /* 7: the 7th bit */
    CMD_TRANS_GEN_SW_2P_LOWPOWER_MODE            = (0x01L << 8), /* 8: the 8th bit */
    CMD_TRANS_GEN_SW_1P_LOWPOWER_MODE            = (0x01L << 9), /* 9: the 9th bit */
    CMD_TRANS_GEN_SW_0P_LOWPOWER_MODE            = (0x01L << 10), /* 10: the 10th bit */
    CMD_TRANS_MAX_RD_PKT_SIZE_TRAN_LOWPOWER_MODE = (0x01L << 11), /* 11: the 11th bit */

    CMD_TRANS_BUTT,
} cmd_trans_mode;

typedef enum {
    VIDEO_MODE = 0,
    COMMAND_MODE = 1,

    CMD_VIDEO_BUTT,
} cmd_video_mode;

typedef enum {
    DCS_LONG_WRITE            = 0x39,
    DCS_SHORT_READ_0PARAM     = 0x06,
    DCS_SHORT_WRITE_1PARAM    = 0x15,
    DCS_SHORT_WRITE_0PARAM    = 0x05,
    GEN_LONG_WRITE            = 0x29,
    GEN_SHORT_READ_2PARAM     = 0x24,
    GEN_SHORT_READ_1PARAM     = 0x14,
    GEN_SHORT_READ_0PARAM     = 0x04,
    GEN_SHORT_WRITE_2PARAM    = 0x23,
    GEN_SHORT_WRITE_1PARAM    = 0x13,
    GEN_SHORT_WRITE_0PARAM    = 0x03,
    MAX_READ_PACKET_SIZE_TRAN = 0x37,
} mipi_tx_dcs_data_type;

#endif /* MIPI_TX_DEF_H */
