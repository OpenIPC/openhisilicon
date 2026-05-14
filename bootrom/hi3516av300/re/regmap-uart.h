#ifndef BOOTROM_HI3516AV300_REGMAP_UART_H
#define BOOTROM_HI3516AV300_REGMAP_UART_H

/* PrimeCell PL011. Only registers actually touched by the bootrom. */
#define UART_DR     0x0000
#define UART_RSR    0x0004
#define UART_FR     0x0018
#define UART_IBRD   0x0024
#define UART_FBRD   0x0028
#define UART_LCR_H  0x002c
#define UART_CR     0x0030

#define UART_FR_RXFE   (1u << 4)   /* RX FIFO empty */

#endif
