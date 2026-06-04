/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef PIN_MUX_H
#define PIN_MUX_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SENSOR_SLAVE_EN 0
#define I2S0_EN 0

typedef enum {
    PIN_MUX_SENSOR_0    = 0,
    PIN_MUX_SENSOR_1    = 1,
    PIN_MUX_SENSOR_0_BT = 2,
} pin_mux_sensor;

typedef enum {
    PIN_MUX_SENSOR0_SLAVE = 0,
    PIN_MUX_SENSOR1_SLAVE = 1,
} pin_mux_sensor_slave;

typedef enum {
    PIN_MUX_I2C_0    = 0,
    PIN_MUX_I2C_1    = 1,
    PIN_MUX_I2C_2    = 2,
    PIN_MUX_I2C_0_BT = 3,
} pin_mux_i2c;

typedef enum {
    MIPI_RX_0 = 0,
} mipi_rx_index;

typedef enum {
    VI_MIPI_RX_MODE = 0,
    VI_CMOS_BT1120_MODE = 1,
    VI_CMOS_BT656_MODE = 2,
} vi_mode_index;

typedef enum {
    PIN_MUX_UART_0 = 0,
    PIN_MUX_UART_1 = 1,
    PIN_MUX_UART_2 = 2,
} pin_mux_uart;

void pin_mux(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* PIN_MUX_H */
