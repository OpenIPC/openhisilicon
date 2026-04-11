/******************************************************************************
Copyright (C), 2011-2018, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2017/07/10
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

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_mmz_api.h"
#include "config.h"

#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)

#ifdef CIPHER_EFUSE_SUPPORT
#ifdef CIPHER_KLAD_SUPPORT
static HI_U8 aes_128_cbc_key[16] = {0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00};
static HI_U8 aes_128_enc_key[16]  = {0xc1,0x1b,0x54,0x4a,0x12,0x9c,0x08,0xa5,0xcc,0xd3,0xeb,0xec,0x7a,0x3b,0x00,0x2b};
#endif
static HI_U8 aes_128_cbc_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static HI_U8 aes_128_src_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
static HI_U8 aes_128_dst_buf[16] = {0xb0,0x1b,0x77,0x09,0xe8,0xdc,0xf9,0xef,0x37,0x13,0x0b,0x13,0xda,0x11,0xbf,0x24};

static HI_S32 printBuffer(HI_CHAR *string, HI_U8 *pu8Input, HI_U32 u32Length)
{
    HI_U32 i = 0;

    if ( NULL != string )
    {
        printf("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) printf("\n");
        printf("0x%02x ", pu8Input[i]);
    }
    printf("\n");

    return HI_SUCCESS;
}


static HI_S32 Setconfiginfo(HI_HANDLE chnHandle, HI_UNF_CIPHER_KEY_SRC_E enKeySrc, HI_UNF_CIPHER_ALG_E alg, HI_UNF_CIPHER_WORK_MODE_E mode, HI_UNF_CIPHER_KEY_LENGTH_E keyLen,
                                                const HI_U8 u8KeyBuf[16], const HI_U8 u8IVBuf[16])
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = keyLen;
    CipherCtrl.enKeySrc = enKeySrc;
    if(CipherCtrl.enWorkMode != HI_UNF_CIPHER_WORK_MODE_ECB)
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    memcpy(CipherCtrl.u32Key, u8KeyBuf, 32);

    s32Ret = HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;
	}

    return HI_SUCCESS;
}

HI_S32 sample_cipher_efuse(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 16;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_KEY_SRC_E enKeySrc;
    HI_U8 u8Key[32];

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;
	}

#ifdef CIPHER_KLAD_SUPPORT
    enKeySrc = HI_UNF_CIPHER_KEY_SRC_KLAD_1;
    s32Ret = HI_UNF_CIPHER_KladEncryptKey(HI_UNF_CIPHER_KEY_SRC_EFUSE_1, HI_UNF_CIPHER_KLAD_TARGET_AES, aes_128_cbc_key, u8Key, 16);
    if(HI_SUCCESS != s32Ret)
	{
	    HI_ERR_CIPHER("Error: Klad Encrypt Key failed!\n");
		return HI_FAILURE;
	}
    if ( 0 != memcmp(u8Key, aes_128_enc_key, 16) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        printBuffer("clean key", aes_128_cbc_key, 16);
        printBuffer("encrypt key", u8Key, 16);
        return HI_FAILURE;
    }
#else
    enKeySrc = HI_UNF_CIPHER_KEY_SRC_EFUSE_1;
#endif

    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return HI_FAILURE;
	}

    u32InputAddrPhy = (HI_U32)HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufIn");
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
        goto __CIPHER_EXIT__;
    }
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);
    u32OutPutAddrPhy = (HI_U32)HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufOut");
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32Testcached);
    #if 0
	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            enKeySrc,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CBC,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            u8Key,
                            aes_128_cbc_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_128_src_buf, u32TestDataLen);
    printBuffer("clear text:", aes_128_src_buf, sizeof(aes_128_src_buf));

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("encrypted text:", pOutputAddrVir, sizeof(aes_128_dst_buf));

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_128_dst_buf, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
#endif
   /* For decrypt */
    memcpy(pInputAddrVir, aes_128_dst_buf, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    enKeySrc,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CBC,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    u8Key,
                                    aes_128_cbc_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    printBuffer("before decrypt:", aes_128_dst_buf, sizeof(aes_128_dst_buf));
    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("decrypted text:", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_128_src_buf, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

__CIPHER_EXIT__:
    if (u32InputAddrPhy> 0)
    {
    HI_MMZ_Unmap(u32InputAddrPhy);
    HI_MMZ_Delete(u32InputAddrPhy);
    }
    if (u32OutPutAddrPhy > 0)
    {
    HI_MMZ_Unmap(u32OutPutAddrPhy);
    HI_MMZ_Delete(u32OutPutAddrPhy);
    }

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();

    return s32Ret;
}
#else
int sample_cipher_efuse()
{
    printf("cipher efuse not support!!!\n");
    return HI_SUCCESS;
}
#endif

