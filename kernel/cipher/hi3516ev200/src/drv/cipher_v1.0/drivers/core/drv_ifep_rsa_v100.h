/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_ifep_rsa_v100.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef _DRV_RSA_V1_H_
#define _DRV_RSA_V1_H_

#include "drv_osal_lib.h"

/*************************** Internal Structure Definition ****************************/
/** \addtogroup      rsa drivers*/
/** @{*/  /** <!-- [rsa]*/

/*! \Define the offset of reg */
#define REG_SEC_RSA_BUSY_REG                          (0x50)
#define REG_SEC_RSA_MOD_REG                           (0x54)
#define REG_SEC_RSA_WSEC_REG                          (0x58)
#define REG_SEC_RSA_WDAT_REG                          (0x5c)
#define REG_SEC_RSA_RPKT_REG                          (0x60)
#define REG_SEC_RSA_RRSLT_REG                         (0x64)
#define REG_SEC_RSA_START_REG                         (0x68)
#define REG_SEC_RSA_ADDR_REG                          (0x6C)
#define REG_SEC_RSA_ERROR_REG                         (0x70)
#define REG_SEC_RSA_CRC16_REG                         (0x74)
#define REG_SEC_RSA_KEY_RANDOM_1                      (0x7c)
#define REG_SEC_RSA_INT_EN                            (0x80)
#define REG_SEC_RSA_INT_STATUS                        (0x84)
#define REG_SEC_RSA_INT_RAW                           (0x88)
#define REG_SEC_RSA_INT_ERR_CLR                       (0x8c)
#define REG_SEC_RSA_VERSION_ID                        (0x90)
#define REG_SEC_RSA_KEY_RANDOM_2                      (0x94)

/*! \Define the union sec_rsa_busy_reg */
typedef union {
    /*! \Define the struct bits */
    struct {
        hi_u32    sec_rsa_busy_reg  : 1   ; /* [0]  */
        hi_u32    reserved_1        : 31  ; /* [31..1]  */
    } bits;

    /*! \Define an unsigned member */
    hi_u32    u32;

} sec_rsa_busy_reg;

/*! \Define the union sec_rsa_mod_reg */
typedef union {
    /*! \Define the struct bits */
    struct {
        hi_u32    sec_rsa_mod_sel     : 2  ; /* [1..0]  */
        hi_u32    sec_rsa_key_width   : 2  ; /* [3..2]  */
        hi_u32    sec_rsa_data0_clr   : 1  ; /* [4]  */
        hi_u32    sec_rsa_data1_clr   : 1  ; /* [5]  */
        hi_u32    sec_rsa_data2_clr   : 1  ; /* [6]  */
        hi_u32    reserved_1          : 25 ; /* [31..7]  */
    } bits;

    /*! \Define an unsigned member */
    hi_u32    u32;

} sec_rsa_mod_reg;

/*! \Define the union sec_rsa_start_reg */
typedef union {
    /*! \Define the struct bits */
    struct {
        hi_u32    sec_rsa_start_reg     : 4   ; /* [3..0]  */
        hi_u32    reserved_1            : 28  ; /* [31..4]  */
    } bits;

    /*! \Define an unsigned member */
    hi_u32    u32;

} sec_rsa_start_reg;

/* Define the union sec_rsa_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    rsa_int_en            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 30  ; /* [30..1]  */
        unsigned int    int_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} sec_rsa_int_en;

/* Define the union sec_rsa_int_status */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    rsa_int_status        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} sec_rsa_int_status;

/* Define the union sec_rsa_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    rsa_int_raw           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} sec_rsa_int_raw;

/** @}*/  /** <!-- ==== Structure Definition end ====*/
#endif
