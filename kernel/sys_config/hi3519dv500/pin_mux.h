/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef PIN_MUX_H
#define PIN_MUX_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SENSOR_SLAVE_EN 1
#define I2S0_EN 0
#define DMIC_EN 0

#define USB_EN  0

/* emmc 8bit mode conflict with i2c5, 6 */
#define I2C_EX_EN 0

#define VO_INTF_NAME_LEN 16

typedef enum {
    PIN_MUX_SENSOR_0 = 0,
    PIN_MUX_SENSOR_1 = 1,
    PIN_MUX_SENSOR_2 = 2,
    PIN_MUX_SENSOR_3 = 3,
} pin_mux_sensor;

typedef enum {
    PIN_MUX_SENSOR0_SLAVE = 0,
    PIN_MUX_SENSOR1_SLAVE = 1,
} pin_mux_sensor_slave;

typedef enum {
    PIN_MUX_I2C_0 = 0,
    PIN_MUX_I2C_1 = 1,
    PIN_MUX_I2C_2 = 2,
    PIN_MUX_I2C_3 = 3,
    PIN_MUX_I2C_4 = 4,
    PIN_MUX_I2C_5 = 5,
    PIN_MUX_I2C_6 = 6,
    PIN_MUX_I2C_7 = 7,
} pin_mux_i2c;

typedef enum {
    PIN_MUX_SPI_0_1V8 = 0,
    PIN_MUX_SPI_0_3V3 = 1,
    PIN_MUX_SPI_1_1V8 = 2,
    PIN_MUX_SPI_1_3V3 = 3,
    PIN_MUX_SPI_2 = 4,
    PIN_MUX_SPI_3_GROUP1 = 5,
    PIN_MUX_SPI_3_GROUP2 = 6,
    PIN_MUX_SPI_3_GROUP3 = 7,
    PIN_MUX_SPI_3WIRES = 8,
    PIN_MUX_SPI_TFT_GROUP1 = 9,
    PIN_MUX_SPI_TFT_GROUP2 = 10,
    PIN_MUX_SPI_TFT_GROUP3 = 11,
    PIN_MUX_SPI_BUTT
} pin_mux_spi;

typedef enum {
    MIPI_RX_0 = 0,
    MIPI_RX_1 = 1,
} mipi_rx_index;

typedef enum {
    VO_BT1120_MODE = 0,
    VO_BT656_MODE = 1,
    VO_RGB_6BIT_MODE = 2,
    VO_RGB_8BIT_MODE = 3,
    VO_RGB_16BIT_MODE = 4,
    VO_RGB_18BIT_MODE = 5,
    VO_RGB_24BIT_MODE = 6,
    VO_MIPI_TX_MODE = 7,
} vo_mode_index;

typedef enum {
    VI_MIPI_RX_MODE = 0,
    VI_CMOS_BT1120_MODE = 1,
    VI_CMOS_BT656_MODE = 2,
    VI_THERMO_MODE = 3,
    VI_THERMO_MODE_T3 = 4
} vi_mode_index;

typedef enum {
    MIPI_TX_DRIVE_CAP_LEVEL0 = 0,
    MIPI_TX_DRIVE_CAP_LEVEL1,
    MIPI_TX_DRIVE_CAP_LEVEL2,
    MIPI_TX_DRIVE_CAP_LEVEL3,
} mipi_tx_drive_cap;

typedef enum {
    PIN_MUX_UART_0 = 0,
    PIN_MUX_UART_1 = 1,
    PIN_MUX_UART_2 = 2,
    PIN_MUX_UART_3 = 3,
    PIN_MUX_UART_4 = 4,
} pin_mux_uart;


#define MIPI_TX_DRIVE_CAP_PHY_REG 0x66

#define MIPI_TX_DRIVE_CAP_LEVEL0_VALUE 0x00
#define MIPI_TX_DRIVE_CAP_LEVEL1_VALUE 0x10
#define MIPI_TX_DRIVE_CAP_LEVEL2_VALUE 0x18
#define MIPI_TX_DRIVE_CAP_LEVEL3_VALUE 0x1C

void pin_mux(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* PIN_MUX_H */
