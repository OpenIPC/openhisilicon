/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "pin_mux.h"
#include "sys_cfg.h"

/* mux with sensor0 slave/mipirx0/sensor0/i2c4 */
static void vi_bt1120_mode_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0034, 0x1006); /* VI_CLK */
    sys_writel(iocfg2_base, 0x0038, 0x1006); /* VI_DATA7 */
    sys_writel(iocfg2_base, 0x003C, 0x1006); /* VI_DATA6 */
    sys_writel(iocfg2_base, 0x0040, 0x1005); /* VI_DATA8 */
    sys_writel(iocfg2_base, 0x0044, 0x1005); /* VI_DATA9 */
    sys_writel(iocfg2_base, 0x0048, 0x1206); /* VI_DATA10 */
    sys_writel(iocfg2_base, 0x004C, 0x1206); /* VI_DATA11 */
    sys_writel(iocfg2_base, 0x0050, 0x1206); /* VI_DATA14 */
    sys_writel(iocfg2_base, 0x0054, 0x1206); /* VI_DATA15 */
    sys_writel(iocfg2_base, 0x0058, 0x1206); /* VI_DATA12 */
    sys_writel(iocfg2_base, 0x005C, 0x1206); /* VI_DATA13 */
    sys_writel(iocfg2_base, 0x0060, 0x1206); /* VI_DATA4 */
    sys_writel(iocfg2_base, 0x0064, 0x1206); /* VI_DATA5 */
    sys_writel(iocfg2_base, 0x0068, 0x1206); /* VI_DATA0 */
    sys_writel(iocfg2_base, 0x006C, 0x1206); /* VI_DATA1 */
    sys_writel(iocfg2_base, 0x0070, 0x1206); /* VI_DATA2 */
    sys_writel(iocfg2_base, 0x0074, 0x1206); /* VI_DATA3 */
}

static void vi_bt656_mode_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();
    sys_writel(iocfg2_base, 0x0024, 0x1006); /* VI_DATA3 */
    sys_writel(iocfg2_base, 0x0028, 0x1006); /* VI_DATA1 */
    sys_writel(iocfg2_base, 0x002C, 0x1006); /* VI_DATA0 */
    sys_writel(iocfg2_base, 0x0030, 0x1006); /* VI_DATA2 */
    sys_writel(iocfg2_base, 0x0034, 0x1006); /* VI_CLK */
    sys_writel(iocfg2_base, 0x0038, 0x1006); /* VI_DATA7 */
    sys_writel(iocfg2_base, 0x003C, 0x1006); /* VI_DATA6 */
    sys_writel(iocfg2_base, 0x0040, 0x1006); /* VI_DATA4 */
    sys_writel(iocfg2_base, 0x0044, 0x1006); /* VI_DATA5 */
}

#if SENSOR_SLAVE_EN
static void sensor0_slave_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0038, 0x1541); /* SENSOR0_HS */
    sys_writel(iocfg2_base, 0x003C, 0x1541); /* SENSOR0_VS */
}

static void sensor1_slave_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x000C, 0x1741); /* SENSOR1_HS */
    sys_writel(iocfg3_base, 0x0010, 0x1741); /* SENSOR1_VS */
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

static void sensor0_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0044, 0x1571); /* SENSOR0_CLK */
    sys_writel(iocfg2_base, 0x0040, 0x1501); /* SENSOR0_RSTN */
}

static void sensor1_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x0004, 0x1371); /* SENSOR1_CLK */
    sys_writel(iocfg3_base, 0x0008, 0x1601); /* SENSOR1_RSTN */
}

/* mux with sensor0/sensor1 */
static void sensor2_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x001C, 0x1174); /* SENSOR2_CLK */
}

/* mux with  VI BT1120/spi1 */
static void sensor3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0024, 0x1172); /* SENSOR3_CLK */
}

static void sensor_pin_mux(int sensor_index)
{
    switch (sensor_index) {
        case PIN_MUX_SENSOR_0:
            sensor0_pin_mux();
            break;
        case PIN_MUX_SENSOR_1:
            sensor1_pin_mux();
            break;
        case PIN_MUX_SENSOR_2:
            sensor2_pin_mux();
            break;
        case PIN_MUX_SENSOR_3:
            sensor3_pin_mux();
            break;
        default:
            break;
    }
}

/* mux with ir_auto I2C0/I2C1/RGB/UART2/SPI2/I2S0 */
static void ir_auto_pin_mux(int ir_auto_en)
{
    void *iocfg_base = sys_config_get_reg_iocfg();
    if (ir_auto_en == 0) {
        return;
    }

    sys_writel(iocfg_base, 0x0000, 0x1300); /* GPIO2_0 */
    sys_writel(iocfg_base, 0x006C, 0x1100); /* GPIO5_1 */
}

/* mux with VO BT656/SPI3/BT1120/RGB/SPITFT */
static void vo_mipi_tx_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg_base, 0x0078, 0x1200); /* DSI_D3N */
    sys_writel(iocfg_base, 0x007C, 0x1200); /* DSI_D3P */
    sys_writel(iocfg_base, 0x0080, 0x1200); /* DSI_D1N */
    sys_writel(iocfg_base, 0x0084, 0x1200); /* DSI_D1P */
    sys_writel(iocfg_base, 0x0088, 0x1200); /* DSI_CKN */
    sys_writel(iocfg_base, 0x008C, 0x1200); /* DSI_CKP */
    sys_writel(iocfg_base, 0x0090, 0x1200); /* DSI_D0N */
    sys_writel(iocfg_base, 0x0094, 0x1200); /* DSI_D0P */
    sys_writel(iocfg_base, 0x0098, 0x1200); /* DSI_D2N */
    sys_writel(iocfg_base, 0x009C, 0x1200); /* DSI_D2P */
    sys_writel(iocfg2_base, 0x0008, 0x1203); /* VSYNC_TE_MIPITX */
}

/* mux with BT656/SPI3/MIPITX/RGB/SPITFT */
static void vo_bt_pin_mux(int vo_bt_mode)
{
    void *iocfg_base = sys_config_get_reg_iocfg();
    void *iocfg_base2 = sys_config_get_reg_iocfg2();

    sys_writel(iocfg_base2, 0x0010,    0x100); /* gpio10_3 pull up */

    sys_writel(iocfg_base, 0x0094,    0x1202); /* VO_BT1120_DATA0 */
    sys_writel(iocfg_base, 0x0090,    0x1202); /* VO_BT1120_DATA1 */
    sys_writel(iocfg_base, 0x0084,    0x1202); /* VO_BT1120_DATA2 */
    sys_writel(iocfg_base, 0x0080,    0x1202); /* VO_BT1120_DATA3 */
    sys_writel(iocfg_base, 0x008C,    0x1202); /* VO_BT1120_DATA4 */
    sys_writel(iocfg_base, 0x0088,    0x1202); /* VO_BT1120_DATA5 */
    sys_writel(iocfg_base, 0x0098,    0x1202); /* VO_BT1120_DATA6 */
    sys_writel(iocfg_base, 0x007C,    0x1202); /* VO_BT1120_DATA7 */

    if (vo_bt_mode == VO_BT656_MODE) {
        sys_writel(iocfg_base, 0x0078,    0x1202); /* VO_BT656_CLK */
        return;
    }
    sys_writel(iocfg_base, 0x00B0,    0x1132); /* VO_BT1120_CLK */
    sys_writel(iocfg_base, 0x00B4,    0x1122); /* VO_BT1120_DATA8 */
    sys_writel(iocfg_base, 0x00AC,    0x1122); /* VO_BT1120_DATA9 */
    sys_writel(iocfg_base, 0x00A4,    0x1122); /* VO_BT1120_DATA10 */
    sys_writel(iocfg_base, 0x00A0,    0x1122); /* VO_BT1120_DATA11 */
    sys_writel(iocfg_base, 0x00A8,    0x1122); /* VO_BT1120_DATA12 */
    sys_writel(iocfg_base, 0x0078,    0x1204); /* VO_BT1120_DATA13 */
    sys_writel(iocfg_base, 0x009C,    0x1202); /* VO_BT1120_DATA14 */
    sys_writel(iocfg_base, 0x0070,    0x1322); /* VO_BT1120_DATA15 */
}

static void vo_rgb_24bit_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x005C,    0x1214); /* RGB_CLK */
    sys_writel(iocfg_base, 0x006C,    0x1514); /* RGB_DE */
    sys_writel(iocfg_base, 0x0058,    0x1614); /* RGB_HS */
    sys_writel(iocfg_base, 0x0064,    0x1614); /* RGB_VS */
    sys_writel(iocfg_base, 0x0060,    0x1714); /* RGB_DATA0 */
    sys_writel(iocfg_base, 0x0068,    0x1714); /* RGB_DATA1 */
    sys_writel(iocfg_base, 0x0070,    0x1714); /* RGB_DATA2 */
    sys_writel(iocfg_base, 0x002C,    0x1514); /* RGB_DATA3 */
    sys_writel(iocfg_base, 0x0028,    0x1514); /* RGB_DATA4 */
    sys_writel(iocfg_base, 0x0040,    0x1514); /* RGB_DATA5 */

    sys_writel(iocfg_base, 0x003C,    0x1514); /* RGB_DATA6 */
    sys_writel(iocfg_base, 0x0044,    0x1514); /* RGB_DATA7 */

    sys_writel(iocfg_base, 0x0048,    0x1714); /* RGB_DATA8 */
    sys_writel(iocfg_base, 0x004C,    0x1514); /* RGB_DATA9 */
    sys_writel(iocfg_base, 0x0054,    0x1514); /* RGB_DATA10 */
    sys_writel(iocfg_base, 0x0050,    0x1514); /* RGB_DATA11 */
    sys_writel(iocfg_base, 0x0030,    0x1514); /* RGB_DATA12 */
    sys_writel(iocfg_base, 0x0034,    0x1514); /* RGB_DATA13 */
    sys_writel(iocfg_base, 0x0038,    0x1514); /* RGB_DATA14 */
    sys_writel(iocfg_base, 0x0010,    0x1214); /* RGB_DATA15 */

    sys_writel(iocfg_base, 0x0014,    0x1214); /* RGB_DATA16 */
    sys_writel(iocfg_base, 0x001C,    0x1214); /* RGB_DATA17 */

    sys_writel(iocfg_base, 0x0020,    0x1214); /* RGB_DATA18 */
    sys_writel(iocfg_base, 0x008C,    0x1204); /* RGB_DATA19 */
    sys_writel(iocfg_base, 0x0080,    0x1204); /* RGB_DATA20 */
    sys_writel(iocfg_base, 0x0084,    0x1204); /* RGB_DATA21 */
    sys_writel(iocfg_base, 0x0090,    0x1204); /* RGB_DATA22 */
    sys_writel(iocfg_base, 0x0094,    0x1204); /* RGB_DATA23 */
}

/* mux with VO I2C/BT1120/MIPI_TX */
static void vo_rgb_pin_mux(int vo_rgb_mode)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    if (vo_rgb_mode == VO_RGB_24BIT_MODE) {
        vo_rgb_24bit_pin_mux();
        return;
    }

    sys_writel(iocfg_base, 0x005C,    0x1214); /* RGB_CLK */
    sys_writel(iocfg_base, 0x006C,    0x1514); /* RGB_DE */
    sys_writel(iocfg_base, 0x0058,    0x1614); /* RGB_HS */
    sys_writel(iocfg_base, 0x0064,    0x1614); /* RGB_VS */
    sys_writel(iocfg_base, 0x0060,    0x1714); /* RGB_DATA0 */
    sys_writel(iocfg_base, 0x0068,    0x1714); /* RGB_DATA1 */
    sys_writel(iocfg_base, 0x0070,    0x1714); /* RGB_DATA2 */
    sys_writel(iocfg_base, 0x002C,    0x1514); /* RGB_DATA3 */
    sys_writel(iocfg_base, 0x0028,    0x1514); /* RGB_DATA4 */
    sys_writel(iocfg_base, 0x0040,    0x1514); /* RGB_DATA5 */

    if (vo_rgb_mode == VO_RGB_6BIT_MODE) {
        return;
    }

    sys_writel(iocfg_base, 0x003C,    0x1514); /* RGB_DATA6 */
    sys_writel(iocfg_base, 0x0044,    0x1514); /* RGB_DATA7 */

    if (vo_rgb_mode == VO_RGB_8BIT_MODE) {
        return;
    }

    sys_writel(iocfg_base, 0x0048,    0x1714); /* RGB_DATA8 */
    sys_writel(iocfg_base, 0x004C,    0x1514); /* RGB_DATA9 */
    sys_writel(iocfg_base, 0x0054,    0x1514); /* RGB_DATA10 */
    sys_writel(iocfg_base, 0x0050,    0x1514); /* RGB_DATA11 */
    sys_writel(iocfg_base, 0x0030,    0x1514); /* RGB_DATA12 */
    sys_writel(iocfg_base, 0x0034,    0x1514); /* RGB_DATA13 */
    sys_writel(iocfg_base, 0x0038,    0x1514); /* RGB_DATA14 */
    sys_writel(iocfg_base, 0x0010,    0x1214); /* RGB_DATA15 */

    if (vo_rgb_mode == VO_RGB_16BIT_MODE) {
        return;
    }

    sys_writel(iocfg_base, 0x0014,    0x1214); /* RGB_DATA16 */
    sys_writel(iocfg_base, 0x001C,    0x1214); /* RGB_DATA17 */
}

static void vo_pin_mux(int vo_mode_type)
{
    switch (vo_mode_type) {
        case VO_BT1120_MODE:
        case VO_BT656_MODE:
            vo_bt_pin_mux(vo_mode_type);
            break;
        case VO_MIPI_TX_MODE:
            vo_mipi_tx_pin_mux();
            break;
        case VO_RGB_6BIT_MODE:
        case VO_RGB_8BIT_MODE:
        case VO_RGB_16BIT_MODE:
        case VO_RGB_18BIT_MODE:
        case VO_RGB_24BIT_MODE:
            vo_rgb_pin_mux(vo_mode_type);
            break;
        default:
            break;
    }
}

static void i2c0_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x006C, 0x1321); /* I2C0_SDA */
    sys_writel(iocfg_base, 0x0070, 0x1321); /* I2C0_SCL */
}

/* mux with i2s0/GPIO2_1 */
static void i2c1_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x0000, 0x1305); /* I2C1_SDA */
    sys_writel(iocfg_base, 0x0004, 0x1105); /* I2C1_SCL */
}

/* mux with i2s0 */
static void i2c2_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x0008, 0x1105); /* I2C2_SDA */
    sys_writel(iocfg_base, 0x000c, 0x1105); /* I2C2_SCL */
}

/* mux with spi0 */
static void i2c3_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0028, 0x1742); /* I2C3_SDA */
    sys_writel(iocfg2_base, 0x002C, 0x1732); /* I2C3_SCL */
}

/* mux with spi0 */
static void i2c4_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0030, 0x1742); /* I2C4_SDA */
    sys_writel(iocfg2_base, 0x0034, 0x1742); /* I2C4_SCL */
}

/* mux with spi1 */
static void i2c5_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x0028, 0x1752); /* I2C5_SDA */
    sys_writel(iocfg3_base, 0x0024, 0x1752); /* I2C5_SCL */
}

/* mux with spi1 */
static void i2c6_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x0030, 0x1752); /* I2C6_SDA */
    sys_writel(iocfg3_base, 0x002C, 0x1752); /* I2C6_SCL */
}

static void i2c7_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0000, 0x1012); /* I2C7_SDA */
    sys_writel(iocfg2_base, 0x0004, 0x1012); /* I2C7_SCL */
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
        case PIN_MUX_I2C_3:
            i2c3_pin_mux();
            break;
        case PIN_MUX_I2C_4:
            i2c4_pin_mux();
            break;
        case PIN_MUX_I2C_5:
            i2c5_pin_mux();
            break;
        case PIN_MUX_I2C_6:
            i2c6_pin_mux();
            break;
        case PIN_MUX_I2C_7:
            i2c7_pin_mux();
            break;
        default:
            break;
    }
}

/* mux with vo I2C3/SENSOR1/SENSOR2/SENSOR3/VI */
static void spi0_pin_mux_1v8(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x34, 0x1151); /* SPI0_SCLK */
    sys_writel(iocfg2_base, 0x30, 0x1341); /* SPI0_SDO */
    sys_writel(iocfg2_base, 0x2C, 0x1341); /* SPI0_SDI */
    sys_writel(iocfg2_base, 0x28, 0x1331); /* SPI0_CSN0 */
    sys_writel(iocfg2_base, 0x24, 0x1331); /* SPI0_CSN1 */
}

static void spi0_pin_mux_3v3(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x34, 0x1131); /* SPI0_SCLK */
    sys_writel(iocfg2_base, 0x30, 0x1331); /* SPI0_SDO */
    sys_writel(iocfg2_base, 0x2C, 0x1331); /* SPI0_SDI */
    sys_writel(iocfg2_base, 0x28, 0x1321); /* SPI0_CSN0 */
    sys_writel(iocfg2_base, 0x24, 0x1321); /* SPI0_CSN1 */
}
/* mux with vo GPIO1/I2C5 */
static void spi1_pin_mux_1v8(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x24, 0x1331); /* SPI1_SCLK */
    sys_writel(iocfg3_base, 0x28, 0x1331); /* SPI1_SDO */
    sys_writel(iocfg3_base, 0x2C, 0x1331); /* SPI1_SDI */
    sys_writel(iocfg3_base, 0x30, 0x1331); /* SPI1_CSN */
}

static void spi1_pin_mux_3v3(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x24, 0x1331); /* SPI1_SCLK */
    sys_writel(iocfg3_base, 0x28, 0x1331); /* SPI1_SDO */
    sys_writel(iocfg3_base, 0x2C, 0x1131); /* SPI1_SDI */
    sys_writel(iocfg3_base, 0x30, 0x1331); /* SPI1_CSN */
}

/* mux with vo UART2/I2S0/I2C1 */
static void spi2_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x4, 0x1223); /* SPI2_SCLK */
    sys_writel(iocfg_base, 0xC, 0x1623); /* SPI2_SDO */
    sys_writel(iocfg_base, 0x8, 0x1623); /* SPI2_SDI */
    sys_writel(iocfg_base, 0x0, 0x1723); /* SPI2_CSN */
}

/* mux with I2S0/RGB_GROUP1 */
static void spi3_gourp1_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x28, 0x1322); /* SPI3_SDO */
    sys_writel(iocfg_base, 0x2C, 0x1322); /* SPI3_SDI */
    sys_writel(iocfg_base, 0x3C, 0x1122); /* SPI3_SCLK */
    sys_writel(iocfg_base, 0x40, 0x1322); /* SPI3_CSN0 */
    sys_writel(iocfg_base, 0x44, 0x1322); /* SPI3_CSN1 */
}

/* mux with jtag */
static void spi3_gourp2_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x64, 0x1123); /* SPI3_SDO */
    sys_writel(iocfg_base, 0x60, 0x1323); /* SPI3_SDI */
    sys_writel(iocfg_base, 0x5C, 0x1223); /* SPI3_SCLK */
    sys_writel(iocfg_base, 0x58, 0x1123); /* SPI3_CSN0 */
    sys_writel(iocfg_base, 0x68, 0x1323); /* SPI3_CSN1 */
}

/* mux with mipitx */
static void spi3_gourp3_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x98, 0x1203); /* SPI3_SDO */
    sys_writel(iocfg_base, 0x7C, 0x1203); /* SPI3_SDI */
    sys_writel(iocfg_base, 0x88, 0x1203); /* SPI3_SCLK */
    sys_writel(iocfg_base, 0x78, 0x1203); /* SPI3_CSN0 */
    sys_writel(iocfg_base, 0x9C, 0x1203); /* SPI3_CSN1 */
}

/* mux with spi1 */
static void spi_3wire_pin_mux(void)
{
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x24, 0x1335); /* SPI_3WIRE_CLK */
    sys_writel(iocfg3_base, 0x30, 0x1335); /* SPI_3WIRE_CSN */
    sys_writel(iocfg3_base, 0x28, 0x1335); /* SPI_3WIRE_DATA */
}

/* mux with rgmii */
static void spi_tft_gourp1_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x040, 0x1106); /* SPI_TFT_CSN  */
    sys_writel(iocfg_base, 0x044, 0x1106); /* SPI_TFT_DATA */
    sys_writel(iocfg_base, 0x03C, 0x1106); /* SPI_TFT_CLK  */
}

/* mux with jtag */
static void spi_tft_gourp2_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x058, 0x1206); /* SPI_TFT_CSN  */
    sys_writel(iocfg_base, 0x060, 0x1306); /* SPI_TFT_DATA */
    sys_writel(iocfg_base, 0x05C, 0x1206); /* SPI_TFT_CLK  */
}

/* mux with mipitx */
static void spi_tft_gourp3_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x078, 0x1206); /* SPI_TFT_CSN  */
    sys_writel(iocfg_base, 0x07C, 0x1206); /* SPI_TFT_DATA */
    sys_writel(iocfg_base, 0x088, 0x1206); /* SPI_TFT_CLK  */
}

static void spi_pin_mux(int spi_index)
{
    switch (spi_index) {
        case PIN_MUX_SPI_0_1V8:
            spi0_pin_mux_1v8();
            break;
        case PIN_MUX_SPI_0_3V3:
            spi0_pin_mux_3v3();
            break;
        case PIN_MUX_SPI_1_1V8:
            spi1_pin_mux_1v8();
            break;
        case PIN_MUX_SPI_1_3V3:
            spi1_pin_mux_3v3();
            break;
        case PIN_MUX_SPI_2:
            spi2_pin_mux();
            break;
        case PIN_MUX_SPI_3_GROUP1:
            spi3_gourp1_pin_mux();
            break;
        case PIN_MUX_SPI_3_GROUP2:
            spi3_gourp2_pin_mux();
            break;
        case PIN_MUX_SPI_3_GROUP3:
            spi3_gourp3_pin_mux();
            break;
        case PIN_MUX_SPI_3WIRES:
            spi_3wire_pin_mux();
            break;
        case PIN_MUX_SPI_TFT_GROUP1:
            spi_tft_gourp1_pin_mux();
            break;
        case PIN_MUX_SPI_TFT_GROUP2:
            spi_tft_gourp2_pin_mux();
            break;
        case PIN_MUX_SPI_TFT_GROUP3:
            spi_tft_gourp3_pin_mux();
            break;
        default:
            break;
    }
}

static void mipi0_rx_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0060, 0x1200); /* MIPI_RX0_CK1N */
    sys_writel(iocfg2_base, 0x0064, 0x1200); /* MIPI_RX0_CK1P */
    sys_writel(iocfg2_base, 0x0070, 0x1200); /* MIPI_RX0_D3N */
    sys_writel(iocfg2_base, 0x0074, 0x1200); /* MIPI_RX0_D3P */
    sys_writel(iocfg2_base, 0x0068, 0x1200); /* MIPI_RX0_D1N */
    sys_writel(iocfg2_base, 0x006C, 0x1200); /* MIPI_RX0_D1P */
    sys_writel(iocfg2_base, 0x0058, 0x1200); /* MIPI_RX0_CK0N */
    sys_writel(iocfg2_base, 0x005C, 0x1200); /* MIPI_RX0_CK0P */
    sys_writel(iocfg2_base, 0x0048, 0x1200); /* MIPI_RX0_D0N */
    sys_writel(iocfg2_base, 0x004C, 0x1200); /* MIPI_RX0_D0P */
    sys_writel(iocfg2_base, 0x0050, 0x1200); /* MIPI_RX0_D2N */
    sys_writel(iocfg2_base, 0x0054, 0x1200); /* MIPI_RX0_D2P */
}

/* mux with vi bt1120 */
static void mipi1_rx_pin_mux(void)
{
    void *iocfg2_base = sys_config_get_reg_iocfg2();

    sys_writel(iocfg2_base, 0x0090, 0x1200); /* MIPI_RX1_CK1N */
    sys_writel(iocfg2_base, 0x0094, 0x1200); /* MIPI_RX1_CK1P */
    sys_writel(iocfg2_base, 0x00A0, 0x1200); /* MIPI_RX1_D3N */
    sys_writel(iocfg2_base, 0x00A4, 0x1200); /* MIPI_RX1_D3P */
    sys_writel(iocfg2_base, 0x0098, 0x1200); /* MIPI_RX1_D1N */
    sys_writel(iocfg2_base, 0x009C, 0x1200); /* MIPI_RX1_D1P */
    sys_writel(iocfg2_base, 0x0088, 0x1200); /* MIPI_RX1_CK0N */
    sys_writel(iocfg2_base, 0x008C, 0x1200); /* MIPI_RX1_CK0P */
    sys_writel(iocfg2_base, 0x0078, 0x1200); /* MIPI_RX1_D0N */
    sys_writel(iocfg2_base, 0x007C, 0x1200); /* MIPI_RX1_D0P */
    sys_writel(iocfg2_base, 0x0080, 0x1200); /* MIPI_RX1_D2N */
    sys_writel(iocfg2_base, 0x0084, 0x1200); /* MIPI_RX1_D2P */
}

/* mux with mipirx0 */
static void thermo_sensor_pin_mux(int intf_type)
{
    void *iocfg_base = sys_config_get_reg_iocfg();
    void *iocfg2_base = sys_config_get_reg_iocfg2();
    void *iocfg3_base = sys_config_get_reg_iocfg3();

    sys_writel(iocfg3_base, 0x0030, 0x1752); /* iocfg_reg111-I2C6 SCL */
    sys_writel(iocfg3_base, 0x002C, 0x1752); /* iocfg_reg111-I2C6 SDA */

    sys_writel(iocfg2_base, 0x0024, 0x1507); /* iocfg_reg67-THERMO_PS    */
    sys_writel(iocfg2_base, 0x0028, 0x1107); /* iocfg_reg68-THERMO_DO6   */
    sys_writel(iocfg2_base, 0x002C, 0x1107); /* iocfg_reg69-THERMO_DO7   */
    sys_writel(iocfg2_base, 0x0030, 0x1507); /* iocfg_reg70-THERMO_HS    */
    sys_writel(iocfg2_base, 0x0034, 0x1707); /* iocfg_reg71-THERMO_CLK   */
    sys_writel(iocfg2_base, 0x0038, 0x1507); /* iocfg_reg72-THERMO_VS    */
    sys_writel(iocfg2_base, 0x003C, 0x1327); /* iocfg_reg73-THERMO_SD1   */
    sys_writel(iocfg2_base, 0x0040, 0x1327); /* iocfg_reg74-THERMO_SD0   */
    sys_writel(iocfg2_base, 0x0044, 0x1357); /* iocfg_reg75-THERMO_MCK   */
    sys_writel(iocfg2_base, 0x0060, 0x1207); /* iocfg_reg82-THERMO_DO3   */
    sys_writel(iocfg2_base, 0x0064, 0x1207); /* iocfg_reg83-THERMO_DO0   */
    sys_writel(iocfg2_base, 0x0068, 0x1207); /* iocfg_reg84-THERMO_DO5   */
    sys_writel(iocfg2_base, 0x006C, 0x1207); /* iocfg_reg85-THERMO_DO4   */
    sys_writel(iocfg2_base, 0x0070, 0x1207); /* iocfg_reg86-THERMO_DO2   */
    sys_writel(iocfg2_base, 0x0074, 0x1207); /* iocfg_reg87-THERMO_DO1   */
    sys_writel(iocfg3_base, 0x0000, 0x1607); /* iocfg_reg100-THERMO_RSTN */
    sys_writel(iocfg3_base, 0x000C, 0x1337); /* iocfg_reg103-THERMO_SD2  */
    sys_writel(iocfg3_base, 0x0010, 0x1337); /* iocfg_reg104-THERMO_SD3  */

    if (intf_type == VI_THERMO_MODE_T3) { /* t3 mux with audio */
        sys_writel(iocfg_base, 0x0000, 0x1300);  /* iocfg_reg0-GPIO2_0 */
        sys_writel(iocfg_base, 0x0004, 0x1200);  /* iocfg_reg1-GPIO2_1 */
        sys_writel(iocfg_base, 0x0008, 0x1200);  /* iocfg_reg2-GPIO2_2 */
        sys_writel(iocfg_base, 0x000C, 0x1200);  /* iocfg_reg3-GPIO2_3 */
    }
}

static void mipi_rx_pin_mux(int mipi_rx_type)
{
    switch (mipi_rx_type) {
        case MIPI_RX_0:
            mipi0_rx_pin_mux();
            break;
        case MIPI_RX_1:
            mipi1_rx_pin_mux();
            break;
        default:
            break;
    }
}

static void vi_pin_mux(int vi_intf_type)
{
    switch (vi_intf_type) {
        case VI_MIPI_RX_MODE:
            mipi_rx_pin_mux(MIPI_RX_0);
            mipi_rx_pin_mux(MIPI_RX_1);
            break;
        case VI_CMOS_BT1120_MODE:
            mipi_rx_pin_mux(MIPI_RX_1);
            vi_bt1120_mode_mux();
            break;
        case VI_CMOS_BT656_MODE:
            mipi_rx_pin_mux(MIPI_RX_1);
            vi_bt656_mode_mux();
            break;
        case VI_THERMO_MODE:
        case VI_THERMO_MODE_T3:
            thermo_sensor_pin_mux(vi_intf_type);
            mipi_rx_pin_mux(MIPI_RX_1);
            break;
        default:
            break;
    }
}

#if I2S0_EN
static void i2s0_pin_mux(int board_type)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    if (board_type == BOARD_SCK) {
        sys_writel(iocfg_base, 0x0058, 0x1602); /* I2S0_SD_RX */
        sys_writel(iocfg_base, 0x005C, 0x1202); /* I2S0_MCLK */
        sys_writel(iocfg_base, 0x0060, 0x1702); /* I2S0_WS */
        sys_writel(iocfg_base, 0x0064, 0x1602); /* I2S0_SD_TX */
        sys_writel(iocfg_base, 0x0068, 0x1702); /* I2S0_BCLK */
    } else if (board_type == BOARD_DMEBPRO) {
        sys_writel(iocfg_base, 0x0000, 0x1724); /* I2S0_WS */
        sys_writel(iocfg_base, 0x0004, 0x1224); /* I2S0_MCLK */
        sys_writel(iocfg_base, 0x0008, 0x1614); /* I2S0_SD_TX */
        sys_writel(iocfg_base, 0x000C, 0x1224); /* I2S0_BCLK */
        sys_writel(iocfg_base, 0x0020, 0x1603); /* I2S0_SD_RX */
    }
}
#endif

#if DMIC_EN
static void dmic_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();

    sys_writel(iocfg_base, 0x0058, 0x1607); /* DMIC_SD0 */
    sys_writel(iocfg_base, 0x005C, 0x1237); /* DMIC_MCLK */
    sys_writel(iocfg_base, 0x0060, 0x1707); /* DMIC_SD1 */
    sys_writel(iocfg_base, 0x0064, 0x1607); /* DMIC_SD2 */
    sys_writel(iocfg_base, 0x0068, 0x1707); /* DMIC_SD3 */
}
#endif

#if USB_EN
/* mux with VO_RESET(GPIO10_3) */
static void usb_pin_mux(void)
{
    void *iocfg_base2 = sys_config_get_reg_iocfg2();

    sys_writel(iocfg_base2, 0x0010, 0x1201); /* USB_VBUS */
}
#endif

static void amp_unmute_pin_mux(void)
{
    void *iocfg_base = sys_config_get_reg_iocfg();
    void *gpio_base = sys_config_get_reg_gpio();

    /* GPIO2_1 */
    sys_writel(iocfg_base, 0x0004, 0x1100);

    /* output high */
    sys_writel(gpio_base, 0x2400, 0x02);
    sys_writel(gpio_base, 0x2008, 0x02);
}
void pin_mux(void)
{
    int vo_intf_type = sys_config_get_vo_intf_type();
    int vi_intf_type = sys_config_get_vi_intf_type();
    int board_type = sys_config_get_board_type();
    int ir_auto_en = sys_config_get_ir_auto();

    sensor_pin_mux(PIN_MUX_SENSOR_0);
    sensor_pin_mux(PIN_MUX_SENSOR_1);
    sensor_pin_mux(PIN_MUX_SENSOR_2);
    sensor_pin_mux(PIN_MUX_SENSOR_3);
    ir_auto_pin_mux(ir_auto_en);
#if SENSOR_SLAVE_EN
    sensor_slave_pin_mux(PIN_MUX_SENSOR0_SLAVE);
    sensor_slave_pin_mux(PIN_MUX_SENSOR1_SLAVE);
#endif
    vo_pin_mux(vo_intf_type);
    i2c_pin_mux(PIN_MUX_I2C_3);
    i2c_pin_mux(PIN_MUX_I2C_4);
#if I2C_EX_EN
    i2c_pin_mux(PIN_MUX_I2C_5);
    i2c_pin_mux(PIN_MUX_I2C_6);
#endif
    i2c_pin_mux(PIN_MUX_I2C_7);
    spi_pin_mux(PIN_MUX_SPI_BUTT);

    vi_pin_mux(vi_intf_type);

    /* audio */
#if I2S0_EN
    i2s0_pin_mux(board_type);
#else
    (void)board_type;
#endif
#if DMIC_EN
    dmic_pin_mux();
#endif
#if USB_EN
    usb_pin_mux();
#endif
    if (vi_intf_type != VI_THERMO_MODE_T3) { /* mux with thermo_t3 */
        amp_unmute_pin_mux();
    }
}
