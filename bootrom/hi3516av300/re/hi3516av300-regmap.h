#ifndef BOOTROM_HI3516AV300_REGMAP_H
#define BOOTROM_HI3516AV300_REGMAP_H

#include "regmap-crg.h"
#include "regmap-gpio.h"
#include "regmap-mmc.h"
#include "regmap-sysctrl.h"
#include "regmap-timer.h"
#include "regmap-trng.h"
#include "regmap-uart.h"

#define BOOTROM_START   0x04000000
#define SDRAM_START     0x04010000
#define FLASH_START     0x14000000
#define DDR_START       0x80000000

#define STACK_START     (SDRAM_START + 0x500)

/*
 * SRAM scratch slots used by the bootrom. The reset trampoline at
 * 0x9c reads SDRAM_START's first word to find the next-stage entry;
 * the load paths populate the rest as they parse the boot image.
 */
#define SRAM_BOOT_TYPE_OFF      0x0000  /* uart0/emmc/sdio/usb tag */
#define SRAM_LOADED_ENTRY_OFF   0x0000  /* alias — same word */
#define SRAM_LOAD_TARGET_OFF    0x0500  /* image buffer base in SRAM */
#define SRAM_TIMER0_VALUE_OFF   0x0180  /* timer_reset_counter saves here */
#define SRAM_TIMER0_DELTA_OFF   0x0184
#define SRAM_INIT_RECORD_OFF    0x0004  /* 12-byte record copied from bootrom 0x7f44 */
#define SRAM_CHUNK_TABLE_OFF    0x0010  /* organize_memory_chunks descriptor base */
#define SRAM_UART_BASE_OFF      0x017c  /* SRAM holds the active UART base pointer */

#define RSA0_START      0x10080000
#define TRNG_START      0x10090000
#define USB_TRIM_START  0x100a0000   /* (OTP slot mirror at 0x50..0x6c) */
#define SPACC_START     0x100c0000   /* Synopsys SPACC accelerator (SHA+AES) */
#define SDIO0_START     0x100f0000
#define EMMC_START      0x10100000
#define IO_CTRL0_START  0x10ff0000
#define IO_CTRL1_START  0x111f0000
#define TIMER0_START    0x12000000
#define CRG_START       0x12010000
#define SYSCTRL_START   0x12020000
#define MISC_START      0x12030000
#define DDRC_START      0x12060000
#define UART0_START     0x120a0000
#define GPIO0_START     0x120d0000

#define BOOTROM_INIT_REC_SRC    (BOOTROM_START + 0x7f44)
#define MEDIA_OK_MAGIC          0x7a696a75u  /* "ziju" — bootloader ack */
#define IMAGE_HEADER_MAGIC      0x4253424du  /* "MBSB" little-endian */

#define CLEAR_REMAP_THUNK_DST   (SDRAM_START + 0x6500)

#endif
