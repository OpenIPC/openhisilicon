/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : kapi_trng.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "cryp_trng.h"

/** @}*/  /** <!-- ==== Structure Definition end ====*/


/******************************* API Code *****************************/
/** \addtogroup      trng */
/** @{*/  /** <!-- [kapi]*/

hi_s32 kapi_trng_get_random(hi_u32 *randnum, hi_u32 timeout)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(randnum == HI_NULL);

    ret = cryp_trng_get_random(randnum, timeout);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== Structure Definition end ====*/
