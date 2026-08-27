/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : user_osal_lib.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "user_osal_lib.h"

/************************ Internal Structure Definition **********************/
/** \addtogroup      lib */
/** @{*/  /** <!-- [osal]*/

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      osal lib */
/** @{*/  /** <!-- [osal]*/

#ifndef __HuaweiLite__
void hex2str(char buf[2], hi_u8 val)
{
    hi_u8 high, low;

    high = (val >> 4) & 0x0F;
    low =  val & 0x0F;

    if (high <= 9) {
        buf[0] = high + '0';
    } else {
        buf[0] = (high - 0x0A) + 'A';
    }

    if (low <= 9) {
        buf[1] = low + '0';
    } else {
        buf[1] = (low - 0x0A) + 'A';
    }

}


/* Implementation that should never be optimized out by the compiler */
hi_void crypto_zeroize( hi_void *buf, hi_u32 len )
{
    volatile unsigned char *p = (unsigned char *)buf;

    if (HI_NULL == buf) {
        return;
    }

    while (len--) {
        *p++ = 0;
    }
}
#endif

hi_void print_string(const hi_s8 *name, hi_u8 *string, hi_u32 size)
{
#ifdef HI_CIPHER_TEST_SUPPORT
    hi_u32 i;
    hi_s8 buf[4] = {0};

    if (name != HI_NULL) {
        HI_PRINT("[%s-%p]:\n", name, string);
    }
    for (i = 0; i < size; i++) {
        hex2str(buf, string[i]);
        HI_PRINT("%c%c ", buf[0], buf[1]);
        if (((i + 1) % 16) == 0)
        { HI_PRINT("\n"); }
    }
    if (( i % 16) != 0) {
        HI_PRINT("\n");
    }
#endif
}

/** @}*/  /** <!-- ==== API Code end ====*/
