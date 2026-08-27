/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : cryp_trng.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "drv_trng.h"
#include "cryp_trng.h"

/********************** Internal Structure Definition ************************/
/** \addtogroup      trng */
/** @{*/  /** <!-- [trng]*/

/* the max continuous bits of randnum is allowed */
#define CONTINUOUS_BITS_ALLOWD              0x08

/* times try to read rang  */
#define RANG_READ_TRY_TIME                  0x40

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      trng drivers*/
/** @{*/  /** <!-- [trng]*/

#ifdef CHIP_TRNG_SUPPORT
static hi_s32 cryp_trng_check(hi_u32 randnum)
{
#ifdef CIPHER_CHECK_RNG_BY_BYTE
    static hi_u32 lastrand = 0;
    hi_u8 *byte = HI_NULL;
    hi_u32 i;

    /* compare with last rand number */
    if (randnum == lastrand) {
        return HI_FAILURE;
    }

    /* update last randnum */
    lastrand = randnum;
    byte = (hi_u8 *)&randnum;

    /* continuous 8 bits0 or bit1 is prohibited */
    for (i = 0; i < 4; i++) {
        /* compare with 0x00 and 0xff */
        if ((byte[i] == 0x00) || (byte[i] == 0xff)) {
            return HI_FAILURE;
        }
    }
#else
    /* continuous 32 bits0 or bit1 is prohibited */
    if ((randnum == 0x00000000) || (randnum == 0xffffffff)) {
        return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
    }
#endif
    return HI_SUCCESS;
}

hi_s32 cryp_trng_get_random(hi_u32 *randnum, hi_u32 timeout)
{
    hi_u32 i = 0;
    hi_s32 ret = HI_FAILURE;
    trng_capacity capacity;

    HI_LOG_FUNC_ENTER();

    drv_trng_get_capacity(&capacity);
    if (!capacity.trng) {
        HI_LOG_ERROR("error, trng nonsupport\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_UNSUPPORTED);
        return HI_ERR_CIPHER_UNSUPPORTED;
    }

    for (i = 0; i < RANG_READ_TRY_TIME; i++) {
        ret = drv_trng_randnum(randnum, timeout);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        ret = cryp_trng_check(*randnum);
        if (ret == HI_SUCCESS) {
            break;
        }
    }

    if (i >= RANG_READ_TRY_TIME) {
        HI_LOG_ERROR("error, trng randnum check failed\n");
        return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

#else
#include <linux/random.h>

hi_s32 cryp_trng_get_random(hi_u32 *randnum, hi_u32 timeout)
{
    HI_LOG_FUNC_ENTER();

    get_random_bytes((hi_u8 *)randnum, WORD_WIDTH);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}
#endif

/** @}*/  /** <!-- ==== API Code end ====*/
