/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2012/07/10
Last Modified :
Description   :	sample for cipher
Function List :
History       :
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_types.h"

extern HI_S32 sample_cipher(HI_VOID);
extern HI_S32 sample_cipher_via(HI_VOID);
extern HI_S32 sample_mutiltcipher(HI_VOID);
extern HI_S32 sample_cipher_efuse(HI_VOID);
extern HI_S32 sample_hash(HI_VOID);
extern HI_S32 sample_rng(HI_VOID);
extern HI_S32 sample_rsa_sign(HI_VOID);
extern HI_S32 sample_rsa_enc(HI_VOID);

#ifdef __HuaweiLite__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 funcNumber = 0;

    if(argc < 2)
    {
        printf("\n##########----Run a cipher sample:for example, ./sample 1----###############\n");
        printf("    You can selset a cipher sample to run as fllow:\n"
                   "    [1] CIPHER\n"
                   "    [2] CIPHER-MUTILT\n"
                   "    [3] CIPHER-EFUSE\n"
                   "    [4] HASH\n"
                   "    [5] RNG\n"
                   "    [6] RSA-SIGN\n"
                   "    [7] RSA-ENC\n"
                   "    [8] CIPHER-VIA\n");

        return HI_FAILURE;
    }

    funcNumber = strtol(argv[1], NULL, 0);
    switch (funcNumber)
    {
        case 1:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_cipher ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_cipher();
            break;
        case 2:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_mutiltcipher ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_mutiltcipher();
            break;
        case 3:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_cipher_efuse ~~~~~~~~~~~~~~~~~~~\n");
            printf("please burn the efuse key before run this sample\n");
            printf("for example: ./write_efuse_key_hi3516cv300.sh\n");
            s32Ret = sample_cipher_efuse();
            break;
        case 4:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_hash ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_hash();
            break;
        case 5:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_rng ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_rng();
            break;
        case 6:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_rsa_sign ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_rsa_sign();
            break;
        case 7:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_rsa_enc ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_rsa_enc();
            break;
        case 8:
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_cipher_via ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_cipher_via();
            break;
        default:
            printf("Function Number %d do not exist!\n", funcNumber);
            s32Ret = HI_FAILURE;
            break;
    }

    if(HI_SUCCESS == s32Ret)
    {
        printf("\n################# FUNCTION %d RUN SUCCESS #################\n", funcNumber);
    }
    else if(HI_FAILURE == s32Ret)
    {
        printf("\n################# FUNCTION %d RUN FAILURE #################\n", funcNumber);
    }

    return s32Ret;
}

