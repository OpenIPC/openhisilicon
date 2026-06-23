/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_IOCTL_H
#define CRYPTO_IOCTL_H

#define CRYPTO_IOC_NA           0U
#define CRYPTO_IOC_W            1U
#define CRYPTO_IOC_R            2U
#define CRYPTO_IOC_RW           3U

/* Ioctl definitions */
#define CRYPTO_IOC(dir, type, nr, size) (((dir) << 30) | ((size) << 16) | ((type) << 8) | ((nr) << 0))

#define CRYPTO_IOC_DIR(cmd)       (((cmd) >> 30) & 0x03)
#define CRYPTO_IOC_TYPE(cmd)      (((cmd) >> 8) & 0xFF)
#define CRYPTO_IOC_NR(cmd)        (((cmd) >> 0) & 0xFF)
#define CRYPTO_IOC_SIZE(cmd)      (((cmd) >> 16) & 0x3FFF)

#endif