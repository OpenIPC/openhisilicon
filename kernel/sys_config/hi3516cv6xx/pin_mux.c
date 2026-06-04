/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "pin_mux.h"
#include <asm/barrier.h>

#include "sys_cfg.h"

/* mux with sensor0 slave/mipirx0/sensor0/i2c4 */
static void vi_bt1120_mode_mux(int board_type)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    if (board_type == BOARD_DMEB_BGA) {
        sys_writel(iocfg2_base, 0x0030, 0x1631); /* VI_DATA9 */
        sys_writel(iocfg2_base, 0x0034, 0x1631); /* VI_DATA8 */
        sys_writel(iocfg2_base, 0x0038, 0x1631); /* VI_DATA6 */
        sys_writel(iocfg2_base, 0x003c, 0x1631); /* VI_DATA7 */
        sys_writel(iocfg2_base, 0x0040, 0x16f1); /* VI_DATA2 */
        sys_writel(iocfg2_base, 0x0044, 0x1631); /* VI_DATA4 */
        sys_writel(iocfg2_base, 0x0048, 0x1631); /* VI_DATA3 */
        sys_writel(iocfg2_base, 0x004c, 0x16f1); /* VI_DATA1 */
        sys_writel(iocfg2_base, 0x0050, 0x1631); /* VI_DATA5 */
        sys_writel(iocfg2_base, 0x0054, 0x1631); /* VI_DATA0 */
        sys_writel(iocfg2_base, 0x0058, 0x1631); /* VI_CLK */
        sys_writel(iocfg2_base, 0x005c, 0x1631); /* VI_DATA10 */
        sys_writel(iocfg2_base, 0x0064, 0x1631); /* VI_DATA11 */
        sys_writel(iocfg2_base, 0x0068, 0x1631); /* VI_DATA15 */
        sys_writel(iocfg2_base, 0x006c, 0x1631); /* VI_DATA14 */
        sys_writel(iocfg2_base, 0x0070, 0x1631); /* VI_DATA12 */
        sys_writel(iocfg2_base, 0x0078, 0x1631); /* VI_DATA13 */
    } else {
        sys_writel(iocfg2_base, 0x0000, 0x1632); /* VI_DATA8 */
        sys_writel(iocfg2_base, 0x0004, 0x1632); /* VI_DATA9 */
        sys_writel(iocfg2_base, 0x0008, 0x1632); /* VI_DATA10 */
        sys_writel(iocfg2_base, 0x000c, 0x1632); /* VI_DATA11 */
        sys_writel(iocfg2_base, 0x0010, 0x1632); /* VI_DATA12 */
        sys_writel(iocfg2_base, 0x0014, 0x1632); /* VI_DATA13 */
        sys_writel(iocfg2_base, 0x0018, 0x1632); /* VI_DATA0 */
        sys_writel(iocfg2_base, 0x001c, 0x1632); /* VI_DATA1 */
        sys_writel(iocfg2_base, 0x0020, 0x1632); /* VI_DATA2 */
        sys_writel(iocfg2_base, 0x0024, 0x1632); /* VI_DATA3 */
        sys_writel(iocfg2_base, 0x0028, 0x1632); /* VI_DATA4 */
        sys_writel(iocfg2_base, 0x002c, 0x1632); /* VI_DATA5 */
        sys_writel(iocfg2_base, 0x0088, 0x1631); /* VI_DATA6 */
        sys_writel(iocfg2_base, 0x008c, 0x1611); /* VI_DATA7 */
        sys_writel(iocfg2_base, 0x0090, 0x1671); /* VI_CLK */
        sys_writel(iocfg2_base, 0x0094, 0x1631); /* VI_DATA14 */
        sys_writel(iocfg2_base, 0x0098, 0x1631); /* VI_DATA15 */
    }
}

static void vi_bt656_mode_mux(int board_type)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    if (board_type == BOARD_DMEB_BGA) {
        sys_writel(iocfg2_base, 0x0054, 0x1631); /* VI_DATA0 */
        sys_writel(iocfg2_base, 0x004c, 0x1631); /* VI_DATA1 */
        sys_writel(iocfg2_base, 0x0040, 0x16f1); /* VI_DATA2 */
        sys_writel(iocfg2_base, 0x0048, 0x1631); /* VI_DATA3 */
        sys_writel(iocfg2_base, 0x0044, 0x1631); /* VI_DATA4 */
        sys_writel(iocfg2_base, 0x0050, 0x1631); /* VI_DATA5 */
        sys_writel(iocfg2_base, 0x0038, 0x1631); /* VI_DATA6 */
        sys_writel(iocfg2_base, 0x003c, 0x1631); /* VI_DATA7 */
        sys_writel(iocfg2_base, 0x0058, 0x1631); /* VI_CLK */
    } else {
        sys_writel(iocfg2_base, 0x0018, 0x1632); /* VI_DATA0 */
        sys_writel(iocfg2_base, 0x001c, 0x1632); /* VI_DATA1 */
        sys_writel(iocfg2_base, 0x0020, 0x1632); /* VI_DATA2 */
        sys_writel(iocfg2_base, 0x0024, 0x1632); /* VI_DATA3 */
        sys_writel(iocfg2_base, 0x0028, 0x1632); /* VI_DATA4 */
        sys_writel(iocfg2_base, 0x002c, 0x1632); /* VI_DATA5 */
        sys_writel(iocfg2_base, 0x0088, 0x1631); /* VI_DATA6 */
        sys_writel(iocfg2_base, 0x008c, 0x1611); /* VI_DATA7 */
        sys_writel(iocfg2_base, 0x0090, 0x1671); /* VI_CLK */
    }
}

#if SENSOR_SLAVE_EN
/* Share the same pin with i2c1 slave pin */
static void sensor0_slave_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0090, 0x1547); /* SENSOR0_HS */
    sys_writel(iocfg2_base, 0x0094, 0x1547); /* SENSOR0_VS */
}

/* Share the same pin with i2c0 slave pin */
static void sensor1_slave_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0098, 0x1547); /* SENSOR1_HS */
    sys_writel(iocfg2_base, 0x009C, 0x1547); /* SENSOR1_VS */
}

static void sensor_slave_pin_mux(int sensor_index)
{
    switch (sensor_index) {
        case PIN_MUX_SENSOR0_SLAVE:
            sensor0_slave_pin_mux();
            break;
        case PIN_MUX_SENSOR1_SLAVE:
            sensor1_slave_pin_mux();
            break;
        default:
            break;
    }
}
#endif

static void sensor0_pin_mux(int board_type)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    if (board_type == BOARD_DMEB_BGA || board_type == BOARD_KOLB) {
        sys_writel(iocfg2_base, 0x0040, 0x1212); /* SENSOR0_CLK */
        sys_writel(iocfg2_base, 0x0050, 0x1137); /* SENSOR0_RSTN */
    } else {
        sys_writel(iocfg2_base, 0x0084, 0x1211); /* SENSOR0_CLK */
        sys_writel(iocfg2_base, 0x008C, 0x1115); /* SENSOR0_RSTN */
    }
}

/* bt1120 and bt656 rely on sensor 0 */
static void sensor0_bt_pin_mux(int board_type)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    if (board_type == BOARD_DMEB_BGA) {
        sys_writel(iocfg2_base, 0x0084, 0x1211); /* SENSOR0_CLK */
        sys_writel(iocfg2_base, 0x008c, 0x1115); /* SENSOR0_RSTN */
    } else {
        sys_writel(iocfg2_base, 0x0084, 0x1211); /* SENSOR0_CLK */
        sys_writel(iocfg2_base, 0x0080, 0x11f7); /* SENSOR0_RSTN */
    }
}

static void sensor1_pin_mux(int board_type)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    if (board_type == BOARD_DMEB_BGA || board_type == BOARD_KOLB) {
        sys_writel(iocfg2_base, 0x004c, 0x1212); /* SENSOR0_CLK */
        sys_writel(iocfg2_base, 0x0054, 0x1137); /* SENSOR0_RSTN */
    } else {
        sys_writel(iocfg2_base, 0x0080, 0x1211); /* SENSOR1_CLK */
        sys_writel(iocfg2_base, 0x0088, 0x1135); /* SENSOR1_RSTN */
    }
}

static void sensor_pin_mux(int sensor_index, int board_type)
{
    switch (sensor_index) {
        case PIN_MUX_SENSOR_0:
            sensor0_pin_mux(board_type);
            break;
        case PIN_MUX_SENSOR_1:
            sensor1_pin_mux(board_type);
            break;
        case PIN_MUX_SENSOR_0_BT:
            sensor0_bt_pin_mux(board_type);
            break;
        default:
            break;
    }
}

static inline void set_phy_reg_isb(void)
{
    isb();
#ifdef CONFIG_64BIT
    dsb(sy);
#else
    dsb();
#endif
#ifdef CONFIG_64BIT
    dmb(sy);
#else
    dmb();
#endif
}

/* Share the same pin with sensor1 slave pin */
static void i2c0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0098, 0x1135); /* I2C0_SDA */
    sys_writel(iocfg2_base, 0x009C, 0x1135); /* I2C0_SCL */
}

/* Share the same pin with sensor0 slave pin */
static void i2c1_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0090, 0x1175); /* I2C1_SDA */
    sys_writel(iocfg2_base, 0x0094, 0x1135); /* I2C1_SCL */
}

static void i2c2_pin_mux(void)
{
#ifndef OT_FPGA
    void *iocfg_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg_base, 0x0050, 0x1133); /* I2C2_SDA */
    sys_writel(iocfg_base, 0x0054, 0x1133); /* I2C2_SCL */
#endif
}

static void i2c0_bt_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x0034, 0x1133); /* I2C0_SDA */
    sys_writel(iocfg3_base, 0x0030, 0x1133); /* I2C0_SCL */
}

static void i2c_pin_mux(int i2c_index)
{
    switch (i2c_index) {
        case PIN_MUX_I2C_0:
            i2c0_pin_mux();
            break;
        case PIN_MUX_I2C_1:
            i2c1_pin_mux();
            break;
        case PIN_MUX_I2C_2:
            i2c2_pin_mux();
            break;
        case PIN_MUX_I2C_0_BT:
            i2c0_bt_pin_mux();
            break;
        default:
            break;
    }
}

static void mipi0_rx_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0000, 0x1430); /* MIPI_RX0_CK1N */
    sys_writel(iocfg2_base, 0x0004, 0x1430); /* MIPI_RX0_CK1P */
    sys_writel(iocfg2_base, 0x0008, 0x1430); /* MIPI_RX0_D3N */
    sys_writel(iocfg2_base, 0x000C, 0x1430); /* MIPI_RX0_D3P */
    sys_writel(iocfg2_base, 0x0010, 0x1430); /* MIPI_RX0_D1N */
    sys_writel(iocfg2_base, 0x0014, 0x1430); /* MIPI_RX0_D1P */
    sys_writel(iocfg2_base, 0x0018, 0x1430); /* MIPI_RX0_CK0N */
    sys_writel(iocfg2_base, 0x001C, 0x1430); /* MIPI_RX0_CK0P */
    sys_writel(iocfg2_base, 0x0020, 0x1430); /* MIPI_RX0_D0N */
    sys_writel(iocfg2_base, 0x0024, 0x1430); /* MIPI_RX0_D0P */
    sys_writel(iocfg2_base, 0x0028, 0x1430); /* MIPI_RX0_D2N */
    sys_writel(iocfg2_base, 0x002C, 0x1430); /* MIPI_RX0_D2P */
}

static void mipi_rx_pin_mux(int mipi_rx_type)
{
    switch (mipi_rx_type) {
        case MIPI_RX_0:
            mipi0_rx_pin_mux();
            break;
        default:
            break;
    }
}

/* mux with ir_auto SDIO1/I2S/UART1/SPI1/PWM1/SENSOR1/JTAG */

static void ir_auto_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();
    int board_type = sys_config_get_board_type();
    if (board_type == BOARD_DMEB_BGA) {
        sys_writel(iocfg3_base, 0x0048, 0x1100); /* GPIO1_1 */
        sys_writel(iocfg3_base, 0x004c, 0x1100); /* GPIO7_6 */
        sys_writel(iocfg3_base, 0x0038, 0x1100); /* GPIO7_7 */
        sys_writel(iocfg3_base, 0x003c, 0x1100); /* GPIO1_0 */
    } else {
        sys_writel(iocfg3_base, 0x0038, 0x1100); /* GPIO7_7 */
        sys_writel(iocfg3_base, 0x003c, 0x1100); /* GPIO1_0 */
        sys_writel(iocfg3_base, 0x0040, 0x1100); /* GPIO1_2 */
        sys_writel(iocfg3_base, 0x0044, 0x1202); /* GPIO1_3 */
    }
}

static void vi_pin_mux(int vi_intf_type, int board_type)
{
    switch (vi_intf_type) {
        case VI_MIPI_RX_MODE:
            i2c_pin_mux(PIN_MUX_I2C_0);
            i2c_pin_mux(PIN_MUX_I2C_1);
            i2c_pin_mux(PIN_MUX_I2C_2);
            mipi_rx_pin_mux(MIPI_RX_0);
            break;
        case VI_CMOS_BT1120_MODE:
            sensor_pin_mux(PIN_MUX_SENSOR_0_BT, board_type);
            i2c_pin_mux(PIN_MUX_I2C_0_BT);
            vi_bt1120_mode_mux(board_type);
            break;
        case VI_CMOS_BT656_MODE:
            sensor_pin_mux(PIN_MUX_SENSOR_0_BT, board_type);
            i2c_pin_mux(PIN_MUX_I2C_0_BT);
            vi_bt656_mode_mux(board_type);
            break;
        default:
            break;
    }
}

#if I2S0_EN
static void i2s0_pin_mux(int board_type)
{
    void *iocfg_base = sys_config_get_reg_iocfg3();

    if (board_type == BOARD_SCK) {
        sys_writel(iocfg_base, 0x0048, 0x1503); /* I2S0_SD_RX */
        sys_writel(iocfg_base, 0x0044, 0x1663); /* I2S0_WS */
        sys_writel(iocfg_base, 0x0040, 0x1523); /* I2S0_BCLK */
        sys_writel(iocfg_base, 0x003c, 0x1523); /* I2S0_SD_TX */
        sys_writel(iocfg_base, 0x0038, 0x1523); /* I2S0_MCLK */
    }
}
#endif

static void uart2_pin_mux(int board_type)
{
    void *iocfg_base = sys_config_get_reg_iocfg3();

    if (board_type == BOARD_KOLB) {
        sys_writel(iocfg_base, 0x0010, 0x1102); /* UART2_TXD */
        sys_writel(iocfg_base, 0x0014, 0x1102); /* UART2_RXD */
    }
}

static void amp_unmute_pin_mux(void)
{
#ifndef OT_FPGA
    void *iocfg_base = sys_config_get_reg_iocfg3();
    void *gpio_base = sys_config_get_reg_gpio();

    /* GPIO1_4 */
    sys_writel(iocfg_base, 0x0004, 0x1200);

    /* output high */
    sys_writel(gpio_base, 0x1400, 0x10);
    sys_writel(gpio_base, 0x1040, 0x10);
#endif
}

void pin_mux(void)
{
    int vi_intf_type = sys_config_get_vi_intf_type();
    void *reg_misc_base = sys_config_get_reg_misc();
    int board_type = sys_config_get_board_type();
    bool ir_auto_en = sys_config_get_ir_auto();

    sensor_pin_mux(PIN_MUX_SENSOR_0, board_type);
    sensor_pin_mux(PIN_MUX_SENSOR_1, board_type);
    if (ir_auto_en == 1) {
        ir_auto_pin_mux();
    }
#if SENSOR_SLAVE_EN
    sensor_slave_pin_mux(PIN_MUX_SENSOR0_SLAVE);
    sensor_slave_pin_mux(PIN_MUX_SENSOR1_SLAVE);
#endif

    vi_pin_mux(vi_intf_type, board_type);

    /* audio */
#if I2S0_EN
    i2s0_pin_mux(BOARD_SCK);
#endif
    uart2_pin_mux(board_type);

    amp_unmute_pin_mux();

    sys_writel(reg_misc_base, 0x170, 0x0);  /* VGS & GZIP */
    sys_writel(reg_misc_base, 0x174, 0x0);  /* ddrt MUX */
}
