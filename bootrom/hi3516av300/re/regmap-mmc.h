#ifndef BOOTROM_HI3516AV300_REGMAP_MMC_H
#define BOOTROM_HI3516AV300_REGMAP_MMC_H

/*
 * Synopsys DesignWare MMC controller layout, used by both EMMC_START
 * and SDIO0_START. Only the registers actually touched by the bootrom
 * are defined here.
 */
#define MMC_CTRL        0x0000
#define MMC_PWREN       0x0004
#define MMC_CMD         0x002c
#define MMC_RINTSTS     0x0044
#define MMC_STATUS      0x0048
#define MMC_CARD_RSTN   0x0078
#define MMC_DATA        0x0200

#endif
