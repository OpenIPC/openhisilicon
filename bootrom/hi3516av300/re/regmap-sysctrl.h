#ifndef BOOTROM_HI3516AV300_REGMAP_SYSCTRL_H
#define BOOTROM_HI3516AV300_REGMAP_SYSCTRL_H

#define SC_CTRL          0x0000
#define SC_SYSRES        0x0004
#define PERISTAT         0x0030
#define SYSSTAT          0x008c

/*
 * SYSCTRL boot-state slots used by the bootrom's load paths.
 * cv500 calls these UNK_13C..UNK_154; we name by observed usage.
 * The slots survive reset (SYSCTRL retention block), so they pass
 * state across the bootrom-to-payload handover.
 */
#define SC_BOOT_TARGET   0x0130  /* DDR target address (0x81000000) */
#define SC_DOWN_TAG      0x013c  /* "DOWN" magic — download mode active */
#define SC_BOOT_ACK      0x0140  /* "ziju" magic — media-load success */
#define SC_UNK_144       0x0144
#define SC_LOAD_TAG      0x0148  /* "DOWN" magic — alt-path duplicate */
#define SC_UNK_14C       0x014c
#define SC_PAYLOAD_DONE  0x0150  /* "EMMC" magic — payload returned cleanly */
#define SC_RECOVERY      0x0154  /* recovery / fastboot mode active */

#endif
