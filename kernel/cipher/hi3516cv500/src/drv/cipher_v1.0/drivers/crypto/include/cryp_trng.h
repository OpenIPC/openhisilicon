/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for cipher cryp trng.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __CRYP_TRNG_H_
#define __CRYP_TRNG_H_

#include "drv_osal_lib.h"
#include "drv_trng.h"

/*
 * brief get rand number.
 * param[out]  randnum rand number.
 * param[in]   timeout time out.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 cryp_trng_get_random(hi_u32 *randnum, hi_u32 timeout);
#endif
