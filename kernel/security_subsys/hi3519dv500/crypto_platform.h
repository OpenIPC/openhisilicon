/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_PLATFORM_H
#define CRYPTO_PLATFORM_H

#define SOC_ID_CIPHER           0x6c

/* Register Base Addr. */
#define SEC_SUBSYS_BASE_ADDR    0x10100000

#define SPACC_REG_BASE_ADDR     (SEC_SUBSYS_BASE_ADDR + 0xF0000)
#define SPACC_REG_SIZE          (0x10000)

#define TRNG_REG_BASE_ADDR      (SEC_SUBSYS_BASE_ADDR + 0xEE000)
#define TRNG_REG_SIZE           (0x1000)

#define PKE_REG_BASE_ADDR       (SEC_SUBSYS_BASE_ADDR + 0xEC000)
#define PKE_REG_SIZE            (0x2000)

#define CA_MISC_REG_BASE_ADDR   (SEC_SUBSYS_BASE_ADDR + 0xE8000)
#define CA_MISC_REG_SIZE        (0x1000)

#define KM_REG_BASE_ADDR        (SEC_SUBSYS_BASE_ADDR + 0xEA000)
#define KM_REG_SIZE             (0x2000)

#define OTPC_BASE_ADDR          (SEC_SUBSYS_BASE_ADDR + 0xE0000)
#define OTPC_ADDR_SIZE          (0x2000)

/* symc hard channel num. */
#define CRYPTO_SYMC_HARD_CHANNEL_MAX    3
#define CRYPTO_SYMC_HARD_CHANNEL_MASK   (0xFFFE)

/* hash hard channel num. */
#define CRYPTO_HASH_HARD_CHN_CNT        3
#define CRYPTO_HASH_HARD_CHN_MASK       (0xFFFE)

#define CRYPTO_DRV_AAD_SIZE             (4 * 1024)
#define CRYPTO_HASH_DRV_BUFFER_SIZE     (128 * 1024)

/* whether to enable the DFA function */
#define CRYPTO_DRV_DFA_ENABLE

#endif