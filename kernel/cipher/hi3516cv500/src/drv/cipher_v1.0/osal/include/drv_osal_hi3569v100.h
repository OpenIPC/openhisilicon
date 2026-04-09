/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv osal hi3559av100 configuration for cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __DRV_OSAL_HI3569V100_H__
#define __DRV_OSAL_HI3569V100_H__

/* liteos use non-secure CPU */
#ifdef __HuaweiLite__
/* the total cipher hard channel which we can used. */
#define CIPHER_HARD_CHANNEL_CNT         0x07

/* mask which cipher channel we can used, bit0 means channel 0. */
#define CIPHER_HARD_CHANNEL_MASK        0xFE

/* the total hash hard channel which we can used. */
#define HASH_HARD_CHANNEL_CNT           0x01

/* mask which cipher channel we can used, bit0 means channel 0. */
#define HASH_HARD_CHANNEL_MASK          0x02
#define HASH_HARD_CHANNEL               0x01

/* the total cipher hard key channel which we can used. */
#define CIPHER_HARD_KEY_CHANNEL_CNT     0x04

/* mask which cipher hard key channel we can used, bit0 means channel 0. */
#define CIPHER_HARD_KEY_CHANNEL_MASK    0xF0

/* support OTP load key. */
#define OTP_SUPPORT

/* support interrupt. */
#define CRYPTO_OS_INT_SUPPORT

/* the hardware version */
#define CHIP_SYMC_VER_V200
#define CHIP_HASH_VER_V200
#define CHIP_TRNG_VER_V200
#define CHIP_IFEP_RSA_VER_V100

/* moudle unsupport, we need set the table. */
#define BASE_TABLE_NULL { \
.reset_valid = 0,  \
.clk_valid = 0, \
.phy_valid = 0, \
.crg_valid = 0, \
.ver_valid = 0, \
.int_valid = 0, \
}

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_CIPHER { \
.name = "cipher",  \
.reset_valid = 1,  \
.clk_valid = 1, \
.phy_valid = 1, \
.crg_valid = 1, \
.ver_valid = 1, \
.int_valid = 1, \
.int_num = 62, \
.reset_bit = 0, \
.clk_bit = 1, \
.version_reg = 0x308, \
.version_val = 0x20170609, \
.reg_addr_phy = 0x10200000, \
.reg_addr_size = 0x4000,    \
.crg_addr_phy = 0x1201016C, \
}

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_HASH { \
.name = "cipher",  \
.reset_valid = 0,  \
.clk_valid = 0, \
.phy_valid = 1, \
.crg_valid = 0, \
.ver_valid = 1, \
.int_valid = 1, \
.int_num = 62, \
.reset_bit = 0, \
.clk_bit = 1, \
.version_reg = 0x308, \
.version_val = 0x20170609, \
.reg_addr_phy = 0x10200000, \
.reg_addr_size = 0x4000, \
.crg_addr_phy = 0x1201016C, \
}

/* define initial value of struct sys_arch_boot_dts for HASH. */
#define HARD_INFO_TRNG { \
.name = "trng",  \
.reset_valid = 1,  \
.clk_valid = 1, \
.phy_valid = 1, \
.crg_valid = 1, \
.ver_valid = 0, \
.int_valid = 0, \
.reset_bit = 12, \
.clk_bit = 13, \
.reg_addr_phy = 0x10230200,  \
.reg_addr_size = 0x100,   \
.crg_addr_phy = 0x12010194, \
}

/* define initial value of struct sys_arch_boot_dts for IFEP RSA. */
#define HARD_INFO_IFEP_RSA { \
.name = "rsa",  \
.reset_valid = 1,  \
.clk_valid = 1, \
.phy_valid = 1, \
.crg_valid = 1, \
.ver_valid = 1, \
.int_valid = 1, \
.reg_addr_phy = 0x10220000,  \
.reg_addr_size = 0x1000, \
.crg_addr_phy = 0x12010194, \
.reset_bit = 14, \
.clk_bit = 15, \
.int_num = 136, \
.version_reg = 0x90, \
.version_val = 0x20160907, \
}

#define KLAD_REG_BASE_ADDR_PHY          0x10210000
#define OTP_REG_BASE_ADDR_PHY           0x10240000
#define KLAD_CRG_ADDR_PHY               0x12010194
#define REG_SYS_OTP_CLK_ADDR_PHY        0x12010194

#define OTP_CRG_CLOCK_BIT               (0x01 << 7)
#define OTP_CRG_RESET_BIT               (0x01 << 6)

#define KLAD_CRG_CLOCK_BIT              (0x01 << 11)
#define KLAD_CRG_RESET_BIT              (0x01 << 10)

/* the hardware capacity */
#define CHIP_AES_CCM_GCM_SUPPORT

/* support smmu.
 * CRYPTO_SMMU_SUPPORT
 *
 * RSA rand mask.
 * RSA_RAND_MASK
 *
 * secure cpu.
 * CRYPTO_SEC_CPU
 *
 * support chip sm2.
 * CHIP_SM2_VER_V100
 *
 * supoort odd key.
 * CHIP_SYMC_ODD_KEY_SUPPORT
 *
 * supoort SM1
 * CHIP_SYMC_SM1_SUPPORT
 *
 * support des
 * CHIP_DES_SUPPORT
 *
 * support 3des
 * CHIP_3DES_SUPPORT
 *
 * support sha1
 * CHIP_SHA1_SUPPORT
 *
 * support sha224
 * CHIP_SHA224_SUPPORT
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
#define HARD_INFO_SMMU                BASE_TABLE_NULL
#define HARD_INFO_SIC_RSA             BASE_TABLE_NULL
#define HARD_INFO_CIPHER_KEY          BASE_TABLE_NULL
#define HARD_INFO_SM4                 BASE_TABLE_NULL
#define HARD_INFO_SM2                 BASE_TABLE_NULL
#else
/* the total cipher hard channel which we can used. */
#define CIPHER_HARD_CHANNEL_CNT         0x07

/* mask which cipher channel we can used, bit0 means channel 0. */
#define CIPHER_HARD_CHANNEL_MASK        0xFE

/* the total hash hard channel which we can used. */
#define HASH_HARD_CHANNEL_CNT           0x01

/* mask which cipher channel we can used, bit0 means channel 0. */
#define HASH_HARD_CHANNEL_MASK          0x02
#define HASH_HARD_CHANNEL               0x01

/* the total cipher hard key channel which we can used. */
#define CIPHER_HARD_KEY_CHANNEL_CNT     0x04

/* mask which cipher hard key channel we can used, bit0 means channel 0. */
#define CIPHER_HARD_KEY_CHANNEL_MASK    0xF0

/* support OTP load key */
#define OTP_SUPPORT

/* support read IRQ number from DTS */
#define IRQ_DTS_SUPPORT

/* support interrupt. */
#define CRYPTO_OS_INT_SUPPORT

/* the hardware version */
#define CHIP_SYMC_VER_V200
#define CHIP_HASH_VER_V200
#define CHIP_TRNG_VER_V200
#define CHIP_IFEP_RSA_VER_V100

/* moudle unsupport, we need set the table. */
#define BASE_TABLE_NULL { \
.reset_valid = 0,  \
.clk_valid = 0, \
.phy_valid = 0, \
.crg_valid = 0, \
.ver_valid = 0, \
.int_valid = 0, \
}

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_CIPHER { \
.name = "cipher",  \
.reset_valid = 1,  \
.clk_valid = 1, \
.phy_valid = 1, \
.crg_valid = 1, \
.ver_valid = 1, \
.int_valid = 1, \
.int_num = 62, \
.reset_bit = 0, \
.clk_bit = 1, \
.version_reg = 0x308, \
.version_val = 0x20170609, \
.reg_addr_phy = 0x10200000, \
.reg_addr_size = 0x4000,    \
.crg_addr_phy = 0x1201016C, \
}

/* define initial value of struct sys_arch_boot_dts for cipher. */
#define HARD_INFO_HASH { \
.name = "hash",  \
.reset_valid = 0,  \
.clk_valid = 0, \
.phy_valid = 1, \
.crg_valid = 0, \
.ver_valid = 1, \
.int_valid = 1, \
.int_num = 62, \
.reset_bit = 0, \
.clk_bit = 1, \
.version_reg = 0x308, \
.version_val = 0x20170609, \
.reg_addr_phy = 0x10200000, \
.reg_addr_size = 0x4000, \
.crg_addr_phy = 0x1201016C, \
}

/* define initial value of struct sys_arch_boot_dts for HASH. */
#define HARD_INFO_TRNG { \
.name = "trng",  \
.reset_valid = 1,  \
.clk_valid = 1, \
.phy_valid = 1, \
.crg_valid = 1, \
.ver_valid = 0, \
.int_valid = 0, \
.reset_bit = 12, \
.clk_bit = 13, \
.reg_addr_phy = 0x10230200,  \
.reg_addr_size = 0x100,   \
.crg_addr_phy = 0x12010194, \
}

/* define initial value of struct sys_arch_boot_dts for IFEP RSA. */
#define HARD_INFO_IFEP_RSA { \
.name = "rsa",  \
.reset_valid = 1,  \
.clk_valid = 1, \
.phy_valid = 1, \
.crg_valid = 1, \
.ver_valid = 1, \
.int_valid = 1, \
.reg_addr_phy = 0x10220000,  \
.reg_addr_size = 0x1000, \
.crg_addr_phy = 0x12010194, \
.reset_bit = 14, \
.clk_bit = 15, \
.int_num = 136, \
.version_reg = 0x90, \
.version_val = 0x20160907, \
}

#define KLAD_REG_BASE_ADDR_PHY          0x10210000
#define OTP_REG_BASE_ADDR_PHY           0x10240000
#define KLAD_CRG_ADDR_PHY               0x12010194
#define REG_SYS_OTP_CLK_ADDR_PHY        0x12010194

#define OTP_CRG_CLOCK_BIT               (0x01 << 7)
#define OTP_CRG_RESET_BIT               (0x01 << 6)

#define KLAD_CRG_CLOCK_BIT              (0x01 << 11)
#define KLAD_CRG_RESET_BIT              (0x01 << 10)

/* the hardware capacity */
#define CHIP_AES_CCM_GCM_SUPPORT

/* support smmu.
 * CRYPTO_SMMU_SUPPORT
 *
 * RSA rand mask.
 * RSA_RAND_MASK
 *
 * secure cpu.
 * CRYPTO_SEC_CPU
 *
 * support chip sm2.
 * CHIP_SM2_VER_V100
 *
 * supoort odd key.
 * CHIP_SYMC_ODD_KEY_SUPPORT
 *
 * supoort SM1
 * CHIP_SYMC_SM1_SUPPORT
 *
 * support sha1
 * CHIP_SHA1_SUPPORT
 *
 * support des
 * CHIP_DES_SUPPORT
 *
 * support 3des
 * CHIP_3DES_SUPPORT
 *
 * support sha224
 * CHIP_SHA224_SUPPORT
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
#define HARD_INFO_SMMU                BASE_TABLE_NULL
#define HARD_INFO_SIC_RSA             BASE_TABLE_NULL
#define HARD_INFO_CIPHER_KEY          BASE_TABLE_NULL
#define HARD_INFO_SM4                 BASE_TABLE_NULL
#define HARD_INFO_SM2                 BASE_TABLE_NULL
#endif

#endif /* __DRV_OSAL_HI3569V100_H__ */
