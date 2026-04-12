/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of strfunc.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "strfunc.h"
#include <linux/ctype.h>


static int atoul(char *str, unsigned int *pul_value);
static int atoulx(char *str, unsigned int *pul_value);

/*
 * Prototype    : str_to_number
 * Description  : convert string to unsigned integer
 */
int str_to_number(char *str, unsigned int *pul_value)
{
    /* check hex string */
    if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X')) {
        if (*(str + 2) == '\0') { /* pointer shift by 2 */
            return -1;
        } else {
            return atoulx(str + 2, pul_value); /* pointer shift by 2 */
        }
    } else {
        return atoul(str, pul_value);
    }
}

/*
 * Prototype    : atoul
 * Description  : convert decimal string to unsigned integer
 */
static int atoul(char *str, unsigned int *pul_value)
{
    unsigned int ul_result = 0;

    while (*str) {
        if (isdigit((int)*str)) {
            /*
             * max value: 0xFFFFFFFF(4294967295),
             * X * 10 + (*str)-48 <:= 4294967295
             * so, X := 429496729
             */
            if ((ul_result < 429496729) || ((ul_result == 429496729) && (*str < '6'))) { /* calc number 429496729 */
                ul_result = ul_result * 10 + (*str) - 48; /* function 10x + (str) - 48 */
            } else {
                *pul_value = ul_result;
                return -1;
            }
        } else {
            *pul_value = ul_result;
            return -1;
        }
        str++;
    }
    *pul_value = ul_result;
    return 0;
}

/*
 * Prototype    : atoulx
 * Description  : Convert HEX string to unsigned integer. Hex string without "0x", ex ABCDE
 */
#define asc2num(ch) ((ch) - '0')
#define hexasc2num(ch) ((ch) - 'A' + 10)

int  atoulx(char *str, unsigned int *pul_value)
{
    unsigned int ul_result = 0;
    unsigned char ch;

    while (*str) {
        ch = toupper(*str);
        if (isdigit(ch) || ((ch >= 'A') && (ch <= 'F'))) {
            if (ul_result < 0x10000000) {
                ul_result = (ul_result << 4) + ((ch <= '9') ? (asc2num(ch)) : (hexasc2num(ch))); /* shift by 4 */
            } else {
                *pul_value = ul_result;
                return -1;
            }
        } else {
            *pul_value = ul_result;
            return -1;
        }
        str++;
    }

    *pul_value = ul_result;
    return 0;
}
