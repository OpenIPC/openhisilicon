#ifndef BOOTROM_HI3516AV300_REGMAP_GPIO_H
#define BOOTROM_HI3516AV300_REGMAP_GPIO_H

/*
 * PL061 PrimeCell layout. Offsets 0x000-0x3fc are the masked DATA
 * register: address bits [9:2] select which pins of the byte-wide
 * port participate. GPIO_DATA_BIT(n) yields the offset that masks
 * down to pin n only.
 */
#define GPIO_DATA_BIT(n)  ((1u << (n)) << 2)
#define GPIO_DIR          0x0400

#endif
