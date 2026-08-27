/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_trng_v100.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef _DRV_HDCP_V100_H_
#define _DRV_HDCP_V100_H_

#include "drv_osal_lib.h"

/*************************** Internal Structure Definition ****************************/
/** \addtogroup      cipher drivers*/
/** @{*/  /** <!-- [cipher]*/

/*! \Define the offset of reg */
#define RNG_CTRL                   (0x0000)
#define RNG_FIFO_DATA              (0x0004)
#define RNG_STAT                   (0x0008)

#define TRNG_TIMEOUT               (0x10000000)

/* Define the union rng_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    osc_sel               : 2   ; /* [1..0]  */
        hi_u32    cleardata             : 1   ; /* [2]  */
        hi_u32    mix_en                : 1   ; /* [3]  */
        hi_u32    filter_enable         : 1   ; /* [4]  */
        hi_u32    drop_enable           : 1   ; /* [5]  */
        hi_u32    reserved0             : 1   ; /* [6]  */
        hi_u32    post_process_enable   : 1   ; /* [7]  */
        hi_u32    post_process_depth    : 8   ; /* [15..8]  */
        hi_u32    reserved1             : 13  ; /* [28..16]  */
        hi_u32    low_osc_st0           : 1   ; /* [29]  */
        hi_u32    low_osc_st1           : 1   ; /* [30]  */
        hi_u32    reserved2             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} rng_ctrl;

/* Define the union rng_stat */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    rng_data_count        : 3   ; /* [2..0]  */
        hi_u32    reserved_0            : 28  ; /* [30..2]  */
        hi_u32    rn_dat_rdy            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} rng_stat;

/** @}*/  /** <!-- ==== Structure Definition end ====*/
#endif
