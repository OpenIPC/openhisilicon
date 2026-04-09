/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv ifep rsa v100.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef _DRV_RSA_V1_H_
#define _DRV_RSA_V1_H_

#include "drv_osal_lib.h"

/* ************************** Internal Structure Definition *************************** */
/* Define the offset of reg */
#define REG_SEC_RSA_BUSY_REG                          0x50
#define REG_SEC_RSA_MOD_REG                           0x54
#define REG_SEC_RSA_WSEC_REG                          0x58
#define REG_SEC_RSA_WDAT_REG                          0x5c
#define REG_SEC_RSA_RPKT_REG                          0x60
#define REG_SEC_RSA_RRSLT_REG                         0x64
#define REG_SEC_RSA_START_REG                         0x68
#define REG_SEC_RSA_ADDR_REG                          0x6C
#define REG_SEC_RSA_ERROR_REG                         0x70
#define REG_SEC_RSA_CRC16_REG                         0x74
#define REG_SEC_RSA_KEY_RANDOM_1                      0x7c
#define REG_SEC_RSA_INT_EN                            0x80
#define REG_SEC_RSA_INT_STATUS                        0x84
#define REG_SEC_RSA_INT_RAW                           0x88
#define REG_SEC_RSA_INT_ERR_CLR                       0x8c
#define REG_SEC_RSA_VERSION_ID                        0x90
#define REG_SEC_RSA_KEY_RANDOM_2                      0x94

#define RSA_INT_RAW_CLR                               1
#define RSA_INT_ERR_CLR                               1

/* Clear the RAM data of the stored result. */
#define RSA_CLR_RAM_STORED_RESULT                     1

/* Clear the RAM data of the stored message. */
#define RSA_CLR_RAM_STORED_MSG                        1

/* Clear the RAM data of the storage key. */
#define RSA_CLR_RAM_STORED_KEY                        1

/* Define the union sec_rsa_busy_reg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    sec_rsa_busy_reg  : 1   ; /* [0] */
        hi_u32    reserved_1        : 31  ; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_rsa_busy_reg;

/* Define the union sec_rsa_mod_reg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    sec_rsa_mod_sel     : 2  ; /* [1..0] */
        hi_u32    sec_rsa_key_width   : 2  ; /* [3..2] */
        hi_u32    sec_rsa_data0_clr   : 1  ; /* [4]  */
        hi_u32    sec_rsa_data1_clr   : 1  ; /* [5]  */
        hi_u32    sec_rsa_data2_clr   : 1  ; /* [6]  */
        hi_u32    reserved_1          : 25 ; /* [31..7] */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_rsa_mod_reg;

/* Define the union sec_rsa_start_reg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    sec_rsa_start_reg     : 4   ; /* [3..0] */
        hi_u32    reserved_1            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_rsa_start_reg;

/* Define the union sec_rsa_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    rsa_int_en            : 1   ; /* [0]  */
        hi_u32    reserved_0            : 30  ; /* [30..1] */
        hi_u32    int_en                : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_rsa_int_en;

/* Define the union sec_rsa_int_status */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    rsa_int_status        : 1   ; /* [0]  */
        hi_u32    reserved_0            : 31  ; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_rsa_int_status;

/* Define the union sec_rsa_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    rsa_int_raw           : 1   ; /* [0]  */
        hi_u32    reserved_0            : 31  ; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_rsa_int_raw;
#endif
