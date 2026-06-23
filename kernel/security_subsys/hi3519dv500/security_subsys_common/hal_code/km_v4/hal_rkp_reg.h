/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_RKP_REG_H
#define HAL_RKP_REG_H

#define RKP_LOCK_CPU_IDLE  0
#define RKP_LOCK_CPU_REE   1
#define RKP_LOCK_CPU_TEE   2
#define RKP_LOCK_CPU_PCPU  4
#define RKP_LOCK_CPU_AIDSP 5

/* Current CPU ID Status .
8'h35: AIDSP;
8'h6a: PCPU;
8'ha5: TEE;
8'haa: ACPU.
*/
#define PCPU_STAT     0x6a
#define AIDSP_STAT    0x35
#define TEE_STAT      0xa5
#define ACPU_STAT     0xaa

#define PBKDF2_ALG_HMAC_SHA1       1
#define PBKDF2_ALG_HMAC_SHA256     0
#define PBKDF2_ALG_HMAC_SHA384     3
#define PBKDF2_ALG_HMAC_SHA512     4
#define PBKDF2_ALG_HMAC_SM3        5

#define HMAC_SHA1_OUTPUT_LEN       20
#define HMAC_SHA256_OUTPUT_LEN     32
#define HMAC_SHA384_OUTPUT_LEN     48
#define HMAC_SHA512_OUTPUT_LEN     64
#define HMAC_SM3_OUTPUT_LEN        32

#define KDF_KEY_BLOCK_SIZE_512     64
#define KDF_KEY_BLOCK_SIZE_1024    128
#define KDF_KEY_CONFIG_LEN         32
#define KDF_SALT_CONFIG_LEN        32
#define KDF_VAL_CONFIG_LEN         16
#define KDF_ALG_TYPE               5
#define DRV_KDF_OTP_KEY_MRK1       0
#define DRV_KDF_OTP_KEY_USK        1
#define DRV_KDF_OTP_KEY_RUSK       2
#define DRV_KDF_DEOB_ALG_AES       0
#define DRV_KDF_DEOB_ALG_SM4       1

/*
 * rkp
 */
#define RKP_LOCK                (0x000)
#define RKP_CMD_CFG             (0x004)
#define KDF_ERROR               (0x008)
#define RKP_DEOB_CFG            (0x020)
#define DEOB_ERROR              (0x028)
#define RK_RDY                  (0x050)
#define RKP_USD_DIS             (0x054)
#define RKP_LOW_POWER           (0x058)
#define RKP_INIT                (0x05C)
#define SW_CFG                  (0x060)
#define RKP_RAW_INT             (0x010)
#define RKP_INT_ENABLE          (0x014)
#define RKP_INT                 (0x018)
#define RKP_PBKDF2_DATA(a)      (0x100 + 4 * (a)) /* a 0~31 */
#define RKP_PBKDF2_KEY(a)       (0x180 + 4 * (a)) /* a 0~31 */
#define RKP_PBKDF2_VAL(b)       (0x200 + 4 * (b)) /* b 0~16 */
#define RKP_USD(c)              (0x300 + 4 * (c)) /* c 0~8 */
#define RKP_SALT(e)             (0x340 + 4 * (e)) /* e 0~6 */
#define RKP_ONEWAY              (0x360)
#define RKP_ALARM               (0x400)

/* Define the union U_RKP_RKP_LOCK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    km_lock_status            : 3; /* [2..0]  */
        unsigned int    reserved                  : 29; /* [31..3]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_lock;

/* Define the union U_RKP_CMD_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    sw_calc_req               : 1; /* [0]  */
        unsigned int    pbkdf2_alg_sel_cfg        : 3; /* [3..1]  */
        unsigned int    pbkdf2_key_sel_cfg        : 5; /* [8..4]  */
        unsigned int    reserved                  : 5; /* [13..9]  */
        unsigned int    pbkdf2_key_len            : 2; /* [15..14]  */
        unsigned int    rkp_pbkdf_calc_time       : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_cmd_cfg;

/* Define the union rkp_oneway_ree */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pbkdf2_rdrk_oneway          : 1; /* [0]  */
        unsigned int    pbkdf2_abrk_oneway          : 1; /* [1]  */
        unsigned int    pbkdf2_odrk1_oneway         : 1; /* [2]  */
        unsigned int    reserved                    : 29; /* [31..3]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_oneway_ree;

/* Define the union rkp_deob_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    deob_update_req            : 1; /* [0]  */
        unsigned int    deob_update_sel            : 2; /* [2..1]  */
        unsigned int    deob_update_alg_sel        : 1; /* [3]  */
        unsigned int    reserved                  : 29; /* [31..4]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_deob_cfg;

/* Define the union rkp_int_enable */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    rkp_int_enable            : 1; /* [0]  */
        unsigned int    reserved                  : 31; /* [31..1]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_int_cfg;

/* Define the union rkp_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    rkp_int                   : 1; /* [0]  */
        unsigned int    reserved                  : 31; /* [31..1]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_int_status;

#endif