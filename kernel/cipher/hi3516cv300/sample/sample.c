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
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <assert.h>
#include <signal.h>
#include "hi_type.h"

extern HI_S32 sample_cipher(HI_VOID);
extern HI_S32 sample_mutiltcipher(HI_VOID);
extern HI_S32 sample_cipher_efuse(HI_VOID);
extern HI_S32 sample_hash(HI_VOID);
extern HI_S32 sample_rng(HI_VOID);
extern HI_S32 sample_rsa_sign(HI_VOID);
extern HI_S32 sample_rsa_enc(HI_VOID);
extern void SAMPLE_Cipher_HandleSig(HI_S32 signo);

#ifdef __HuaweiLite__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 funcNumber = 0;
    char u8Cmd[32];

    while (1)
    {
        printf("\n##########----Run a cipher sample----###############\n");
        printf("    You can selset a cipher sample to run as fllow:\n"
    		   "    [0] CIPHER\n"
    		   "    [1] CIPHER-MUTILT\n"
    		   "    [2] CIPHER-EFUSE\n"
    		   "    [3] HASH\n"
    		   "    [4] RNG\n"
    		   "    [5] RSA-SIGN\n"
    		   "    [6] RSA-ENC\n"
    		   "    [q] EXIT\n");
        printf("Please enter the function number:\n");
        fgets(u8Cmd, sizeof(u8Cmd)-1, stdin);

        if (u8Cmd[0] == '\0')
        {
            printf("Error, function number invalid.\n");
            continue;
        }
        else if (u8Cmd[0] == 'q')
        {
            printf("Cipher sample exit, thank you for test, byebye ~^~\n");
            break;
        }
        else if ((u8Cmd[0] < '0') || (u8Cmd[0] > '6'))
        {
            printf("Error, function number invalid: %c.\n", u8Cmd[0]);
            continue;
        }

        funcNumber = strtol(u8Cmd,NULL,0);

    	if ( 0 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_cipher ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_cipher();
        }
        else if ( 1 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_mutiltcipher ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_mutiltcipher();
        }
        else if ( 2 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_cipher_efuse ~~~~~~~~~~~~~~~~~~~\n");
            printf("please burn the efuse key before run this sample\n");
            printf("for example: ./write_efuse_key_hi3516cv300.sh\n");
            s32Ret = sample_cipher_efuse();
        }
        else if ( 3 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_hash ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_hash();
        }
    	else if ( 4 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_rng ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_rng();
        }
    	else if ( 5 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_rsa_sign ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_rsa_sign();
        }
    	else if ( 6 == funcNumber )
        {
            printf("\n~~~~~~~~~~~~~~~~~~~~~ run sample_rsa_enc ~~~~~~~~~~~~~~~~~~~\n");
            s32Ret = sample_rsa_enc();
        }
        else
        {
            printf("Function Number %d do not exist!\n", funcNumber);
        }

        if(HI_SUCCESS == s32Ret)
        {
            printf("\033[0;1;32m""\n################# FUNCTION %d RUN SUCCESS #################\n\n""\033[0m", funcNumber);
        }
        else
        {
            printf("\033[0;1;31m""\n################# FUNCTION %d RUN FAILURE #################\n\n""\033[0m", funcNumber);
        }
        sleep(1);
    }
    return HI_SUCCESS;
}

