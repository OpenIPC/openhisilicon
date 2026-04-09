/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv trng v200.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef _DRV_TRNG_V200_H_
#define _DRV_TRNG_V200_H_

#include "drv_osal_lib.h"

/* ************************** Internal Structure Definition *************************** */
/* Define the offset of reg */
#define  HISEC_COM_TRNG_CTRL            0x00
#define  HISEC_COM_TRNG_FIFO_DATA       0x04
#define  HISEC_COM_TRNG_DATA_ST         0x08

/* Define the union hisec_com_trng_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32   osc_sel            :  2;   /* [1..0]  */
        hi_u32   cleardata          :  1;   /* [2]  */
        hi_u32   drbg_enable        :  1;   /* [3]  */
        hi_u32   pre_process_enable :  1;   /* [4]  */
        hi_u32   drop_enable        :  1;   /* [5]  */
        hi_u32   mix_enable         :  1;   /* [6]  */
        hi_u32   post_process_enable:  1;   /* [7]  */
        hi_u32   post_process_depth :  8;   /* [15..8]  */
        hi_u32   trng_sel           :  1;   /* [16]  */
        hi_u32   reserved0          :  2;   /* [18..17]  */
        hi_u32   alarm_clr          :  1;   /* [19]  */
        hi_u32   reserved1          :  12;  /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hisec_com_trng_ctrl;

/* Define the union hisec_com_trng_data_st */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    low_osc_st0        :    1; /* [0]  */
        hi_u32    low_osc_st1        :    1; /* [1]  */
        hi_u32    low_ro_st0         :    1; /* [2]  */
        hi_u32    low_ro_st1         :    1; /* [3]  */
        hi_u32    reserved0          :    4; /* [7..4]  */
        hi_u32    trng_fifo_data_cnt :    6; /* [13..8]  */
        hi_u32    reserved1          :    18; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hisec_com_trng_data_st;
#endif /* End of _DRV_TRNG_V200_H_ */
