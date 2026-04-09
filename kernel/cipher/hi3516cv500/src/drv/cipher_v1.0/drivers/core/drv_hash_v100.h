/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv hash v100.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef _DRV_HASH_V100_H_
#define _DRV_HASH_V100_H_

#include "drv_osal_lib.h"

/* Define the offset of reg */
#define  REG_TOTALLEN_LOW           0x00
#define  REG_TOTALLEN_HIGH          0x04
#define  REG_STATUS                 0x08
#define  REG_CTRL                   0x0C
#define  REG_START                  0x10
#define  REG_DMA_START_ADDR         0x14
#define  REG_DMA_LEN                0x18
#define  REG_DATA_IN                0x1C
#define  REG_REC_LEN1               0x20
#define  REG_REC_LEN2               0x24
#define  REG_SHA_OUT1               0x30
#define  REG_SHA_OUT2               0x34
#define  REG_SHA_OUT3               0x38
#define  REG_SHA_OUT4               0x3C
#define  REG_SHA_OUT5               0x40
#define  REG_SHA_OUT6               0x44
#define  REG_SHA_OUT7               0x48
#define  REG_SHA_OUT8               0x4C
#define  REG_MCU_KEY0               0x70
#define  REG_MCU_KEY1               0x74
#define  REG_MCU_KEY2               0x78
#define  REG_MCU_KEY3               0x7C
#define  REG_KL_KEY0                0x80
#define  REG_KL_KEY1                0x84
#define  REG_KL_KEY2                0x88
#define  REG_KL_KEY3                0x8C
#define  REG_INIT1_UPDATE           0x90

/* Define the union hash_status */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    hash_rdy        : 1   ; /* [0]  */
        hi_u32    dma_rdy         : 1   ; /* [1]  */
        hi_u32    msg_rdy         : 1   ; /* [2]  */
        hi_u32    rec_rdy         : 1   ; /* [3]  */
        hi_u32    error_state     : 2   ; /* [5..4]  */
        hi_u32    len_err         : 1   ; /* [6]  */
        hi_u32    reserved_1      : 2   ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hash_status;

/* Define the union hash_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    read_ctrl     : 1  ; /* [0]  */
        hi_u32    sha_sel       : 2  ; /* [2..1]  */
        hi_u32    hmac_flag     : 1  ; /* [3]  */
        hi_u32    hardkey_sel   : 1  ; /* [4]  */
        hi_u32    small_end_en  : 1  ; /* [5]  */
        hi_u32    sha_init_update_en  : 1  ; /* [7]  */
        hi_u32    reserved_1      : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hash_ctrl;

/* Define the union sha_start */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    sha_start     : 1   ; /* [0]  */
        hi_u32    reserved_1    : 30  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sha_start;
#endif /* End of _DRV_HASH_V100_H_ */
