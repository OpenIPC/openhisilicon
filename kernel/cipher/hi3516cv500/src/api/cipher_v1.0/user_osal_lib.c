/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for user osal lib.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "user_osal_lib.h"

/* *********************** Internal Structure Definition ********************* */
#ifndef __HuaweiLite__
void hex2str(char buf[MUL_VAL_2], hi_u32 buf_len, hi_u8 val)
{
    hi_u8 high, low;

    if (buf_len != MUL_VAL_2) {
        return;
    }

    high = (val >> SHIFT_4BITS) & MAX_LOW_4BITS;
    low =  val & MAX_LOW_4BITS;

    if (high <= 9) {    /* 0 ~ 9. */
        buf[WORD_IDX_0] = high + '0';
    } else {
        buf[WORD_IDX_0] = (high - 0x0A) + 'A';  /* A ~ F. */
    }

    if (low <= 9) {     /* 0 ~ 9. */
        buf[WORD_IDX_1] = low + '0';
    } else {
        buf[WORD_IDX_1] = (low - 0x0A) + 'A';   /* A ~ F. */
    }
}


/* Implementation that should never be optimized out by the compiler. */
hi_void crypto_zeroize(hi_void *buf, hi_u32 len)
{
    volatile unsigned char *p = (unsigned char *)buf;

    if (buf == HI_NULL) {
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
    hi_s8 buf[WORD_WIDTH] = {0};

    if (string == HI_NULL) {
        HI_PRINT("string is null!\n");
        return;
    }

    if (name != HI_NULL) {
        HI_PRINT("[%s-%p]:\n", name, string);
    }
    for (i = 0; i < size; i++) {
        hex2str(buf, 2, string[i]); /* 2 bytes */
        HI_PRINT("%c%c ", buf[0], buf[1]);
        if (((i + 1) % PRINT_HEX_BLOCK_LEN) == 0) {
            HI_PRINT("\n");
        }
    }
    if ((i % PRINT_HEX_BLOCK_LEN) != 0) {
        HI_PRINT("\n");
    }
#endif
}

#ifdef CIPHER_DEBUG_TEST_SUPPORT
void crypto_print_hex(const hi_char *name, hi_u8 *str, hi_u32 len)
{
    hi_u32 i;
    hi_u8 *tmp_str = (hi_u8 *)(str);

    if (name == HI_NULL) {
        hi_log_error("name is null.\n");
        return;
    }

    if (str == HI_NULL) {
        hi_log_error("str is null.\n");
        return;
    }

    if (len == 0) {
        hi_log_error("len is 0.\n");
        return;
    }

    HI_PRINT("[%s]:\n", (name));
    for (i = 0 ; i < (len); i++) {
        if ((i % PRINT_HEX_BLOCK_LEN == 0) && (i != 0)) {
            HI_PRINT("\n");
        }

        HI_PRINT("\\x%02x", *((tmp_str) + i));
    }
    HI_PRINT("\n");
}
#endif
