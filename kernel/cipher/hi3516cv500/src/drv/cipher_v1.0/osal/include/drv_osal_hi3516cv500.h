/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv osal hi3516cv500 configuration of cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __DRV_OSAL_HI3516CV500_H__
#define __DRV_OSAL_HI3516CV500_H__

/* the total cipher hard channel which we can used. */
#define CIPHER_HARD_CHANNEL_CNT         0x07
/* mask which cipher channel we can used, bit0 means channel 0. */
#define CIPHER_HARD_CHANNEL_MASK        0xFE

/* the total hash hard channel which we can used. */
#define HASH_HARD_CHANNEL_CNT           0x01

/* liteos resource config */
#ifdef __HuaweiLite__
/* mask which cipher channel we can used, bit0 means channel 0. */
#define HASH_HARD_CHANNEL_MASK          0x04
#define HASH_HARD_CHANNEL               0x02
#else
/* mask which cipher channel we can used, bit0 means channel 0. */
#define HASH_HARD_CHANNEL_MASK          0x02
#define HASH_HARD_CHANNEL               0x01
#endif

/* the total cipher hard key channel which we can used. */
#define CIPHER_HARD_KEY_CHANNEL_CNT     0x04

/* mask which cipher hard key channel we can used, bit0 means channel 0. */
#define CIPHER_HARD_KEY_CHANNEL_MASK    0xF0

/* support read IRQ number from DTS */
#define IRQ_DTS_SUPPORT

/* support OTP load key */
#define OTP_SUPPORT

/* support reducing power dissipation. */
#define CRYPTO_CORE_AUTO_CKEN_SUPPORT

#ifndef __HuaweiLite__
/* support interrupt. */
#define CRYPTO_OS_INT_SUPPORT
#endif

/* the hardware version */
#define CHIP_SYMC_VER_V200
#define CHIP_HASH_VER_V200
#define CHIP_TRNG_VER_V200
#define CHIP_IFEP_RSA_VER_V100

/*
 * SMP version linux is sec config
 * moudle unsupport, we need set the table.
 */
#define BASE_TABLE_NULL { \
.reset_valid = 0,  \
.clk_valid = 0, \
.phy_valid = 0, \
.crg_valid = 0, \
.ver_valid = 0, \
.int_valid = 0, \
}

/* support des */
#define CHIP_DES_SUPPORT

/* support 3des */
#define CHIP_3DES_SUPPORT

/* support sha1 */
#define CHIP_SHA1_SUPPORT

/* support sha224 */
#define CHIP_SHA224_SUPPORT

/* supoort trng version v200.
 * CHIP_TRNG_VER_V200
 *
 * supoort odd key.
 * CHIP_SYMC_ODD_KEY_SUPPORT
 *
 * supoort SM1.
 * CHIP_SYMC_SM1_SUPPORT
 *
 * the hardware capacity.
 * CHIP_AES_CCM_GCM_SUPPORT
 *
 * supoort sm2 ver v100.
 * CHIP_SM2_VER_V100
 *
 * RSA rand mask.
 * RSA_RAND_MASK
 *
 * support smmu.
 * CRYPTO_SMMU_SUPPORT
 *
 * support secure cpu
 * CRYPTO_SEC_CPU
 *
 * support switch cpu.
 * CRYPTO_SWITCH_CPU
 *
 * the software capacity.
 * SOFT_AES_SUPPORT
 * SOFT_TDES_SUPPORT
 * SOFT_AES_CCM_GCM_SUPPORT
 * SOFT_SHA1_SUPPORT
 * SOFT_SHA256_SUPPORT
 * SOFT_SHA512_SUPPORT
 * SOFT_SM2_SUPPORT
 * SOFT_SM3_SUPPORT
 * SOFT_ECC_SUPPORT
 * SOFT_AES_CTS_SUPPORT
 */
#if defined(ARCH_TYPE_amp) && !defined(AMP_NONSECURE_VERSION) && !defined(__HuaweiLite__)

/* linux of AMP use non-secure config. */
#define HARD_INFO_TRNG                BASE_TABLE_NULL

#define HARD_INFO_CIPHER {  \
.name = "nonsec_cipher",    \
.reset_valid = 1,           \
.clk_valid = 1,             \
.phy_valid = 1,             \
.crg_valid = 1,             \
.ver_valid = 1,             \
.int_valid = 1,             \
.int_num = 104,             \
.reset_bit = 8,             \
.clk_bit = 9,               \
.version_reg = 0x308,       \
.version_val = 0x2018121,   \
.reg_addr_phy = 0x100C0000, \
.reg_addr_size = 0x4000,    \
.crg_addr_phy = 0x120101A0, \
}

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_HASH {    \
.name = "nonsec_hash",      \
.reset_valid = 0,           \
.clk_valid = 0,             \
.phy_valid = 1,             \
.crg_valid = 0,             \
.ver_valid = 1,             \
.int_valid = 1,             \
.int_num = 104,             \
.reset_bit = 8,             \
.clk_bit = 9,               \
.version_reg = 0x308,       \
.version_val = 0x2018121,   \
.reg_addr_phy = 0x100C0000, \
.reg_addr_size = 0x4000,    \
.crg_addr_phy = 0x120101A0, \
}

/* define initial value of struct sys_arch_boot_dts for IFEP RSA. */
#define HARD_INFO_IFEP_RSA { \
.name = "nonsec_rsa",       \
.reset_valid = 1,           \
.clk_valid = 1,             \
.phy_valid = 1,             \
.crg_valid = 1,             \
.ver_valid = 1,             \
.int_valid = 0,             \
.reg_addr_phy = 0x100D0000, \
.reg_addr_size = 0x1000,    \
.crg_addr_phy = 0x120101A0, \
.reset_bit = 6,             \
.clk_bit = 7,               \
.version_reg = 0x90,        \
.version_val = 0,           \
}
#else
/* SMP, liteos of secure AMP, linux of non-secure AMP use secure config. */
#define CRYPTO_SEC_CPU

#define CHIP_TRNG_VER_V200

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_CIPHER {  \
.name = "cipher",           \
.reset_valid = 1,           \
.clk_valid = 1,             \
.phy_valid = 1,             \
.crg_valid = 1,             \
.ver_valid = 1,             \
.int_valid = 1,             \
.int_num = 103,             \
.reset_bit = 8,             \
.clk_bit = 9,               \
.version_reg = 0x308,       \
.version_val = 0x2018121,   \
.reg_addr_phy = 0x100C0000, \
.reg_addr_size = 0x4000,    \
.crg_addr_phy = 0x120101A0, \
}

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_HASH {    \
.name = "hash",             \
.reset_valid = 0,           \
.clk_valid = 0,             \
.phy_valid = 1,             \
.crg_valid = 0,             \
.ver_valid = 1,             \
.int_valid = 1,             \
.int_num = 103,             \
.reset_bit = 8,             \
.clk_bit = 9,               \
.version_reg = 0x308,       \
.version_val = 0x2018121,   \
.reg_addr_phy = 0x100C0000, \
.reg_addr_size = 0x4000,    \
.crg_addr_phy = 0x120101A0, \
}

/* define sec rsa1 for SMP VERSION. */
#define HARD_INFO_IFEP_RSA { \
.name = "rsa0",             \
.reset_valid = 1,           \
.clk_valid = 1,             \
.phy_valid = 1,             \
.crg_valid = 1,             \
.ver_valid = 1,             \
.int_valid = 0,             \
.reg_addr_phy = 0x10080000, \
.reg_addr_size = 0x1000,    \
.crg_addr_phy = 0x120101A0, \
.reset_bit = 4,             \
.clk_bit = 5,               \
.version_reg = 0x90,        \
.version_val = 0,           \
}

/* define initial value of struct sys_arch_boot_dts for HASH. */
#define HARD_INFO_TRNG {    \
.name = "trng",             \
.reset_valid = 1,           \
.clk_valid = 1,             \
.phy_valid = 1,             \
.crg_valid = 1,             \
.ver_valid = 0,             \
.int_valid = 0,             \
.reset_bit = 2,             \
.clk_bit = 3,               \
.reg_addr_phy = 0x10090200, \
.reg_addr_size = 0x100,     \
.crg_addr_phy = 0x120101A0, \
}
#endif

#define KLAD_REG_BASE_ADDR_PHY          0x10070000
#define OTP_REG_BASE_ADDR_PHY           0x100B0000
#define KLAD_CRG_ADDR_PHY               0x120101A0
#define REG_SYS_OTP_CLK_ADDR_PHY        0x120101BC

#define OTP_CRG_CLOCK_BIT               (0x01 << 1)

#define KLAD_CRG_CLOCK_BIT              (0x01 << 1)
#define KLAD_CRG_RESET_BIT              (0x01 << 0)

#define HARD_INFO_SMMU                BASE_TABLE_NULL
#define HARD_INFO_SIC_RSA             BASE_TABLE_NULL
#define HARD_INFO_CIPHER_KEY          BASE_TABLE_NULL
#define HARD_INFO_SM4                 BASE_TABLE_NULL
#define HARD_INFO_SM2                 BASE_TABLE_NULL

#define NSEC_HARD_INFO_CIPHER              BASE_TABLE_NULL
#define NSEC_HARD_INFO_HASH                BASE_TABLE_NULL
#define NSEC_HARD_INFO_IFEP_RSA            BASE_TABLE_NULL
#define NSEC_HARD_INFO_SMMU                BASE_TABLE_NULL
#define NSEC_HARD_INFO_SIC_RSA             BASE_TABLE_NULL
#define NSEC_HARD_INFO_CIPHER_KEY          BASE_TABLE_NULL
#define NSEC_HARD_INFO_SM4                 BASE_TABLE_NULL
#define NSEC_HARD_INFO_SM2                 BASE_TABLE_NULL
#define NSEC_HARD_INFO_TRNG                BASE_TABLE_NULL

#endif /* __DRV_OSAL_HI3516CV500_H__ */
