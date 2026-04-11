/******************************************************************************
Copyright (C), 2011-2018, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2017/07/10
Last Modified :
Description   :	sample for rng
Function List :
History       :
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "config.h"

#ifdef  CONFIG_SUPPORT_CA_RELEASE
#define HI_ERR_RNG(format, arg...)
#define HI_DEBUG_RNG(format...)   printf(format)
#else
#define HI_ERR_RNG(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_DEBUG_RNG(format...)   printf(format)
#endif

#ifdef CIPHER_RNG_SUPPORT
int sample_rng(HI_VOID)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_S32 index = 0;
    HI_U32 randomNumber = 0;

    Ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_RNG("HI_UNF_CIPHER_Init ERROR!\n");
        return Ret;
    }

    for (index = 0; index < 10; index++)
    {
        Ret = HI_UNF_CIPHER_GetRandomNumber(&randomNumber);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_RNG("HI_UNF_CIPHER_GetRandomNumber ERROR!\n");
            return Ret;
        }

        HI_DEBUG_RNG("Random number: %08x\n", randomNumber);
    }

    Ret = HI_UNF_CIPHER_DeInit();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_RNG("CIPHER DeInit ERROR! Return Value = %d.\n", Ret);
        return Ret;
    }

    return Ret;
}
#else
int sample_rng(HI_VOID)
{
    printf("TRNG not support!!!\n");
    return HI_SUCCESS;
}
#endif
