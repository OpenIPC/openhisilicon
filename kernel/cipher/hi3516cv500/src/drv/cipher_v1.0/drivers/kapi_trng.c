/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for kapi trng.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_trng.h"

/* ****************************** API Code **************************** */
hi_s32 kapi_trng_get_random(hi_u32 *randnum, hi_u32 timeout)
{
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_chk_param_return(randnum == HI_NULL);

    ret = cryp_trng_get_random(randnum, timeout);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}
