/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_TX_HAL_H
#define MIPI_TX_HAL_H

#include "securec.h"
#include "ot_mipi_tx.h"
#include "mipi_tx_reg.h"

#define MIPI_TX_BITS_PER_BYTE 8
#define MIPI_TX_INTERRUPTS_ENABLE 0

#define MIPI_TX_BYTES_PER_WORD 4
#define MIPI_TX_CMD_BYTE1 0
#define MIPI_TX_CMD_BYTE2 1
#define MIPI_TX_CMD_BYTE3 2
#define MIPI_TX_CMD_BYTE4 3

#define MIPI_TX_REGS_ADDR   0x10270000
#define MIPI_TX_REGS_SIZE   0x2000

#define MIPI_TX_PHY_REGS_ADDR   0x10272000
#define MIPI_TX_PHY_REGS_SIZE   0x2000

#define MIPI_TX_IRQ         83

#define MIPI_TX_REF_CLK     192

#ifdef OT_FPGA

/* crg */
#define MIPI_TX_CRG         0x11018140

/* phy addr */
#define PLL_SET_0X14           0x14
#define PLL_SET_0X15           0x15
#define PLL_SET_0X16           0x16
#define PLL_SET_0X17           0x17
#define PLL_SET_0X1E           0x1E
#define PLL_SET_0X12           0x12
#define PLL_SET_0X13           0x13

#define DATA0_TPRE_DELAY    0x30
#define DATA1_TPRE_DELAY    0x40
#define DATA2_TPRE_DELAY    0x50
#define DATA3_TPRE_DELAY    0x60

/* clk */
#define CLK_POST_DELAY      0x21
#define CLK_TLPX            0x22
#define CLK_TCLK_PREPARE    0x23
#define CLK_TCLK_ZERO       0x24
#define CLK_TCLK_TRAIL      0x25

/* data0 */
#define DATA0_TLPX          0x32
#define DATA0_THS_PREPARE   0x33
#define DATA0_THS_ZERO      0x34
#define DATA0_THS_TRAIL     0x35

/* data1 */
#define DATA1_TLPX          0x42
#define DATA1_THS_PREPARE   0x43
#define DATA1_THS_ZERO      0x44
#define DATA1_THS_TRAIL     0x45

/* data2 */
#define DATA2_TLPX          0x52
#define DATA2_THS_PREPARE   0x53
#define DATA2_THS_ZERO      0x54
#define DATA2_THS_TRAIL     0x55

/* data3 */
#define DATA3_TLPX          0x62
#define DATA3_THS_PREPARE   0x63
#define DATA3_THS_ZERO      0x64
#define DATA3_THS_TRAIL     0x65

#define DATA0_TPRE_DELAY_VALUE 0x6
#define DATA1_TPRE_DELAY_VALUE 0x6
#define DATA2_TPRE_DELAY_VALUE 0x6
#define DATA3_TPRE_DELAY_VALUE 0x6

#define CLK_POST_DELAY_VALUE   0x0
#define CLK_TLPX_VALUE         0x4
#define CLK_TCLK_PREPARE_VALUE 0x3
#define CLK_TCLK_ZERO_VALUE    0xc
#define CLK_TCLK_TRAIL_VALUE   0x4

#define DATA0_TLPX_VALUE        0x4
#define DATA0_THS_PREPARE_VALUE 0x3
#define DATA0_THS_ZERO_VALUE    0x6
#define DATA0_THS_TRAIL_VALUE   0x4

#define DATA1_TLPX_VALUE        0x4
#define DATA1_THS_PREPARE_VALUE 0x3
#define DATA1_THS_ZERO_VALUE    0x6
#define DATA1_THS_TRAIL_VALUE   0x4

#define DATA2_TLPX_VALUE        0x4
#define DATA2_THS_PREPARE_VALUE 0x3
#define DATA2_THS_ZERO_VALUE    0x6
#define DATA2_THS_TRAIL_VALUE   0x4

#define DATA3_TLPX_VALUE        0x4
#define DATA3_THS_PREPARE_VALUE 0x3
#define DATA3_THS_ZERO_VALUE    0x6
#define DATA3_THS_TRAIL_VALUE   0x4

#else

/* crg */
#define MIPI_TX_CRG         0x11018140

/* timing parameter */
#define TLPX                60
#define TCLK_PREPARE        60
#define TCLK_ZERO           285
#define TCLK_TRAIL          60
#define TCLK_POST           95
#define TPRE_DELAY          10

#endif /* end of #ifdef OT_FPGA */

#define MIPI_TX_CSI_DATA_TYPE_FRAME_START 0x00
#define MIPI_TX_CSI_DATA_TYPE_FRAME_END   0x01
#define MIPI_TX_CSI_DATA_TYPE_LINE_START  0x02
#define MIPI_TX_CSI_DATA_TYPE_LINE_END    0x03

#define MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_NORMAL 0x18
#define MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_LEGACY 0x1A
#define MIPI_TX_CSI_DATA_TYPE_YUV422_8BIT        0x1E
#define MIPI_TX_CSI_DATA_TYPE_RBG_888            0x24
#define MIPI_TX_CSI_DATA_TYPE_RAW_8BIT           0x2A
#define MIPI_TX_CSI_DATA_TYPE_RAW_10BIT          0x2B
#define MIPI_TX_CSI_DATA_TYPE_RAW_12BIT          0x2C
#define MIPI_TX_CSI_DATA_TYPE_RAW_14BIT          0x2D
#define MIPI_TX_CSI_DATA_TYPE_RAW_16BIT          0x2E

#define MIPI_TX_READ_TIMEOUT_CNT 1000
#define MIPI_TX_MAX_RESET_PHY_TIMES 10

#define PREPARE_COMPENSATE    10
#define ROUNDUP_VALUE     7999
#define INNER_PEROID    8000   /* 8 * 1000 ,1000 is 1us = 1000ns, 8 is division ratio */

#define mipi_tx_align_up(x, a) ((((x) + ((a) - 1)) / (a)) * (a))
#define mipi_tx_ceil(x, a)     (((x) + (a)-1) / (a))

/*
 * global operation timing parameters
 */
typedef struct {
    unsigned char data_tpre_delay;
    unsigned char clk_post_delay;
    unsigned char clk_tlpx;
    unsigned char clk_tclk_prepare;
    unsigned char clk_tclk_zero;
    unsigned char clk_tclk_trail;
    unsigned char data_tlpx;
    unsigned char data_ths_prepare;
    unsigned char data_ths_zero;
    unsigned char data_ths_trail;
} mipi_tx_phy_timing_parameters;

typedef struct {
    unsigned int vall_det;
    unsigned int vact_det;
    unsigned int hall_det;
    unsigned int hact_det;
    unsigned int hbp_det;
    unsigned int hsa_det;
    unsigned int vsa_det;
} mipi_tx_dev_phy_t;

typedef enum {
    HIGH_SPEED_MODE = 0,
    LOW_POWER_MODE = 1,
} command_trans_mode;

__inline static unsigned char get_low_byte(unsigned short short_data)
{
    return (unsigned char)(short_data & 0xff);
}

__inline static unsigned char get_high_byte(unsigned short short_data)
{
    return (unsigned char)((short_data & 0xff00) >> MIPI_TX_BITS_PER_BYTE);
}

void mipi_tx_drv_set_irq_num(unsigned int irq_num);
void mipi_tx_drv_set_regs(const mipi_tx_regs_type_t *regs);

void mipi_tx_drv_set_phy_cfg(void);
void mipi_tx_drv_get_dev_status(mipi_tx_dev_phy_t *mipi_tx_phy_ctx);
void mipi_tx_drv_set_controller_cfg(const combo_dev_cfg_t *dev_cfg);

int mipi_tx_drv_set_cmd_info(const cmd_info_t *cmd_info);

int mipi_tx_drv_get_cmd_info(const get_cmd_info_t *get_cmd_info);
void mipi_tx_drv_enable_input(const out_mode_t output_mode);
void mipi_tx_drv_set_pn_swap(const combo_dev_pn_swap_t *pn_swap);
void mipi_tx_drv_get_pn_swap(combo_dev_pn_swap_t *pn_swap);
void mipi_tx_drv_reset_pn_swap(void);
void mipi_tx_drv_disable_input(void);
void mipi_tx_drv_set_ctrl_clk(unsigned int enable);
void mipi_tx_drv_phy_power_down(void);

int mipi_tx_drv_init(int smooth);
void mipi_tx_drv_exit(void);

#endif /* end of #ifndef MIPI_TX_HAL_H */
