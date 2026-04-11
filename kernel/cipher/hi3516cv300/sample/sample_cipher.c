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
#include <signal.h>

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_mmz_api.h"
#include "config.h"

#define PHY_ADDR_ENABLE

#define VIR_ADDR_ENABLE

#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)

#ifdef CIPHER_MULTICIPHER_SUPPORT
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


static HI_S32 Setconfiginfo(HI_HANDLE chnHandle, HI_UNF_CIPHER_KEY_SRC_E enKeySrc,
                     HI_UNF_CIPHER_ALG_E alg,
                     HI_UNF_CIPHER_WORK_MODE_E mode,
                     HI_UNF_CIPHER_KEY_LENGTH_E keyLen,
                     const HI_U8 u8KeyBuf[32], const HI_U8 u8IVBuf[16])
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = keyLen;
    CipherCtrl.enKeySrc = enKeySrc;
    if((mode == HI_UNF_CIPHER_WORK_MODE_CBC)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_CFB)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_OFB)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_CTR))
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    if ( HI_UNF_CIPHER_KEY_SRC_USER == enKeySrc )
    {
        memcpy(CipherCtrl.u32Key, u8KeyBuf, 32);
    }

    s32Ret = HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;
	}

    return HI_SUCCESS;
}

/* encrypt data using special chn*/
static HI_S32 CBC_AES128(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 16;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U8 aes_key[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
    HI_U8 aes_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
    HI_U8 aes_src[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
    HI_U8 aes_dst[16] = {0x76,0x49,0xAB,0xAC,0x81,0x19,0xB2,0x46,0xCE,0xE9,0x8E,0x9B,0x12,0xE9,0x19,0x7D};

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
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

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CBC,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CBC-AES-128-ORI:", aes_src, sizeof(aes_src));

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CBC-AES-128-ENC:", pOutputAddrVir, sizeof(aes_dst));

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_UNF_CIPHER_KEY_SRC_USER,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CBC,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CBC-AES-128-DEC:", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

static HI_S32 CFB_AES128(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 32;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U8 aes_key[16] = {"\x2B\x7E\x15\x16\x28\xAE\xD2\xA6\xAB\xF7\x15\x88\x09\xCF\x4F\x3C"};
    HI_U8 aes_IV[16]  = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"};
    HI_U8 aes_src[32] = {"\x6B\xC1\xBE\xE2\x2E\x40\x9F\x96\xE9\x3D\x7E\x11\x73\x93\x17\x2A\xAE\x2D\x8A\x57\x1E\x03\xAC\x9C\x9E\xB7\x6F\xAC\x45\xAF\x8E\x51"};
    HI_U8 aes_dst[32] = {"\x3B\x3F\xD9\x2E\xB7\x2D\xAD\x20\x33\x34\x49\xF8\xE8\x3C\xFB\x4A\xC8\xA6\x45\x37\xA0\xB3\xA9\x3F\xCD\xE3\xCD\xAD\x9F\x1C\xE5\x8B"};

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
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

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CFB,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                             aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CFB-AES-128-ORI:", aes_src, u32TestDataLen);

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CFB-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_UNF_CIPHER_KEY_SRC_USER,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CFB,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CFB-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

static HI_S32 CTR_AES128(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 32;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U8 aes_key[16] = {"\x7E\x24\x06\x78\x17\xFA\xE0\xD7\x43\xD6\xCE\x1F\x32\x53\x91\x63"};
    HI_U8 aes_IV[16]  = {"\x00\x6C\xB6\xDB\xC0\x54\x3B\x59\xDA\x48\xD9\x0B\x00\x00\x00\x01"};
    HI_U8 aes_src[32] = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"};
    HI_U8 aes_dst[32] = {"\x51\x04\xA1\x06\x16\x8A\x72\xD9\x79\x0D\x41\xEE\x8E\xDA\xD3\x88\xEB\x2E\x1E\xFC\x46\xDA\x57\xC8\xFC\xE6\x30\xDF\x91\x41\xBE\x28"};

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
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

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CTR,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                             aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CTR-AES-128-ORI:", aes_src, u32TestDataLen);

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CTR-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_UNF_CIPHER_KEY_SRC_USER,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CTR,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                     aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CTR-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

#ifdef CIPHER_CCM_GCM_SUPPORT
static HI_S32 SetconfiginfoEx(HI_HANDLE chnHandle, HI_UNF_CIPHER_KEY_SRC_E enKeySrc,
                     HI_UNF_CIPHER_ALG_E alg,
                     HI_UNF_CIPHER_WORK_MODE_E mode,
                     HI_UNF_CIPHER_KEY_LENGTH_E keyLen,
                     const HI_U8 u8KeyBuf[32], const HI_U8 u8IVBuf[16],
                     HI_UNF_CIPHER_CCM_INFO_S *pstCCM,
                     HI_UNF_CIPHER_GCM_INFO_S *pstGCM)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = keyLen;
    CipherCtrl.enKeySrc = enKeySrc;
    if((mode == HI_UNF_CIPHER_WORK_MODE_CBC)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_CFB)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_OFB)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_CTR)
       ||(mode == HI_UNF_CIPHER_WORK_MODE_GCM))
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    if ((mode == HI_UNF_CIPHER_WORK_MODE_CCM) && (pstCCM != HI_NULL))
    {
        memcpy(&CipherCtrl.unModeInfo.stCCM, pstCCM, sizeof(HI_UNF_CIPHER_CCM_INFO_S));
        CipherCtrl.stChangeFlags.bit1IV = 1;
    }
    if ((mode == HI_UNF_CIPHER_WORK_MODE_GCM) && (pstGCM != HI_NULL))
    {
        memcpy(&CipherCtrl.unModeInfo.stGCM, pstGCM, sizeof(HI_UNF_CIPHER_GCM_INFO_S));
    }

    if ( HI_UNF_CIPHER_KEY_SRC_USER == enKeySrc )
    {
        memcpy(CipherCtrl.u32Key, u8KeyBuf, 32);
    }

    s32Ret = HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;
	}

    return HI_SUCCESS;
}

// Klen  = 128, Tlen =32, Nlen  = 56,  Alen  = 64, and  Plen  = 32
// t = 4, q=8, p=4
HI_S32 CCM_AES128(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 4;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_CCM_INFO_S stCCM;
    HI_U8 aes_key[16] = {"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"};
    HI_U8 aes_n[13] = {"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c"};
    static HI_U8 aes_a[65536];
    HI_U8 aes_src[32] = {"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
                         "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"};
    HI_U8 aes_dst[32] = {"\x69\x91\x5d\xad\x1e\x84\xc6\x37\x6a\x68\xc2\x96\x7e\x4d\xab\x61"
                         "\x5a\xe0\xfd\x1f\xae\xc4\x4c\xc4\x84\x82\x85\x29\x46\x3c\xcf\x72"};
    HI_U8 aes_tag[14] = {"\xb4\xac\x6b\xec\x93\xe8\x59\x8e\x7f\x0d\xad\xbc\xea\x5b"};
    HI_U8 out_tag[14];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}
    u32InputAddrPhy = (HI_U32)HI_MMZ_New(32, 0, NULL, "CIPHER_BufIn");
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
        goto __CIPHER_EXIT__;
    }
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);
    u32OutPutAddrPhy = (HI_U32)HI_MMZ_New(32, 0, NULL, "CIPHER_BufOut");
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32Testcached);

    memset(&stCCM, 0, sizeof(HI_UNF_CIPHER_CCM_INFO_S));
    memcpy(&stCCM.u8Nonce, aes_n, sizeof(aes_n));
    stCCM.pu8Aad = aes_a;
    stCCM.u32ALen = 8;
    stCCM.u8NLen = 7;
    stCCM.u32MLen = 4;
    stCCM.u8TLen = 4;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_a, sizeof(aes_a));
	s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

/* *****************************************************************************
 * In the following example,  Klen  = 128, Tlen =112,  Nlen  = 104,
 * Alen  = 524288, and  Plen  = 256.
 ******************************************************************************/
static HI_S32 CCM_AES128_2(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 32;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U32 i;
    HI_UNF_CIPHER_CCM_INFO_S stCCM;
    HI_U8 aes_key[16] = {"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"};
    HI_U8 aes_n[13] = {"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c"};
    static HI_U8 aes_a[65536];
    HI_U8 aes_src[32] = {"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
                         "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"};
    HI_U8 aes_dst[32] = {"\x69\x91\x5d\xad\x1e\x84\xc6\x37\x6a\x68\xc2\x96\x7e\x4d\xab\x61"
                         "\x5a\xe0\xfd\x1f\xae\xc4\x4c\xc4\x84\x82\x85\x29\x46\x3c\xcf\x72"};
    HI_U8 aes_tag[14] = {"\xb4\xac\x6b\xec\x93\xe8\x59\x8e\x7f\x0d\xad\xbc\xea\x5b"};
    HI_U8 out_tag[14];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    for(i=0; i<65536; i++)
    {
        aes_a[i] = i;
    }

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
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

    memset(&stCCM, 0, sizeof(HI_UNF_CIPHER_CCM_INFO_S));
    memcpy(&stCCM.u8Nonce, aes_n, sizeof(aes_n));
    stCCM.pu8Aad = aes_a;
    stCCM.u32ALen = 65536;
    stCCM.u8NLen = 13;
    stCCM.u32MLen = u32TestDataLen;
    stCCM.u8TLen = 14;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
	s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

#if 0
static HI_S32 GCM_AES128(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 60;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_GCM_INFO_S stGCM;
    HI_U8 aes_key[32] = {"\xfe\xff\xe9\x92\x86\x65\x73\x1c\x6d\x6a\x8f\x94\x67\x30\x83\x08"};
    HI_U8 aes_iv[12] =  {"\xca\xfe\xba\xbe\xfa\xce\xdb\xad\xde\xca\xf8\x88"};
    HI_U8 aes_a[20] =   {"\xfe\xed\xfa\xce\xde\xad\xbe\xef\xfe\xed\xfa\xce\xde\xad\xbe\xef\xab\xad\xda\xd2"};
    HI_U8 aes_src[60] = {"\xd9\x31\x32\x25\xf8\x84\x06\xe5\xa5\x59\x09\xc5\xaf\xf5\x26\x9a"
                         "\x86\xa7\xa9\x53\x15\x34\xf7\xda\x2e\x4c\x30\x3d\x8a\x31\x8a\x72"
                         "\x1c\x3c\x0c\x95\x95\x68\x09\x53\x2f\xcf\x0e\x24\x49\xa6\xb5\x25"
                         "\xb1\x6a\xed\xf5\xaa\x0d\xe6\x57\xba\x63\x7b\x39"};
    HI_U8 aes_dst[60] = {"\x42\x83\x1e\xc2\x21\x77\x74\x24\x4b\x72\x21\xb7\x84\xd0\xd4\x9c"
                         "\xe3\xaa\x21\x2f\x2c\x02\xa4\xe0\x35\xc1\x7e\x23\x29\xac\xa1\x2e"
                         "\x21\xd5\x14\xb2\x54\x66\x93\x1c\x7d\x8f\x6a\x5a\xac\x84\xaa\x05"
                         "\x1b\xa3\x0b\x39\x6a\x0a\xac\x97\x3d\x58\xe0\x91"};
    HI_U8 aes_tag[16] = {"\x5b\xc9\x4f\xbc\x32\x21\xa5\xdb\x94\xfa\xe9\x5a\xe7\x12\x1a\x47"};
    HI_U8 out_tag[16];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}
    u32InputAddrPhy = (HI_U32)HI_MMZ_New(64, 0, NULL, "CIPHER_BufIn");
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
        goto __CIPHER_EXIT__;
    }
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);
    u32OutPutAddrPhy = (HI_U32)HI_MMZ_New(64, 0, NULL, "CIPHER_BufOut");
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32Testcached);

    memset(&stGCM, 0, sizeof(HI_UNF_CIPHER_GCM_INFO_S));
    stGCM.pu8Aad = aes_a;
    stGCM.u32ALen = 20;
    stGCM.u32MLen = 60;
    stGCM.u32IVLen = 12;
    u32TestDataLen = stGCM.u32MLen;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("GCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_a, sizeof(aes_a));
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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
#endif

static HI_S32 GCM_AES128_2(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 64;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_GCM_INFO_S stGCM;
    HI_U8 aes_key[32] = {"\xFE\xFF\xE9\x92\x86\x65\x73\x1C\x6D\x6A\x8F\x94\x67\x30\x83\x08"};
    HI_U8 aes_iv[12] =  {"\xCA\xFE\xBA\xBE\xFA\xCE\xDB\xAD\xDE\xCA\xF8\x88"};
    HI_U8 aes_a[64] =   {"\x3A\xD7\x7B\xB4\x0D\x7A\x36\x60\xA8\x9E\xCA\xF3\x24\x66\xEF\x97"
                         "\xF5\xD3\xD5\x85\x03\xB9\x69\x9D\xE7\x85\x89\x5A\x96\xFD\xBA\xAF"
                         "\x43\xB1\xCD\x7F\x59\x8E\xCE\x23\x88\x1B\x00\xE3\xED\x03\x06\x88"
                         "\x7B\x0C\x78\x5E\x27\xE8\xAD\x3F\x82\x23\x20\x71\x04\x72\x5D\xD4"};
    HI_U8 aes_src[64] = {"\xD9\x31\x32\x25\xF8\x84\x06\xE5\xA5\x59\x09\xC5\xAF\xF5\x26\x9A"
                         "\x86\xA7\xA9\x53\x15\x34\xF7\xDA\x2E\x4C\x30\x3D\x8A\x31\x8A\x72"
                         "\x1C\x3C\x0C\x95\x95\x68\x09\x53\x2F\xCF\x0E\x24\x49\xA6\xB5\x25"
                         "\xB1\x6A\xED\xF5\xAA\x0D\xE6\x57\xBA\x63\x7B\x39\x1A\xAF\xD2\x55"};
    HI_U8 aes_dst[64] = {"\x42\x83\x1E\xC2\x21\x77\x74\x24\x4B\x72\x21\xB7\x84\xD0\xD4\x9C"
                         "\xE3\xAA\x21\x2F\x2C\x02\xA4\xE0\x35\xC1\x7E\x23\x29\xAC\xA1\x2E"
                         "\x21\xD5\x14\xB2\x54\x66\x93\x1C\x7D\x8F\x6A\x5A\xAC\x84\xAA\x05"
                         "\x1B\xA3\x0B\x39\x6A\x0A\xAC\x97\x3D\x58\xE0\x91\x47\x3F\x59\x85"};
    HI_U8 aes_tag[16] = {"\x64\xC0\x23\x29\x04\xAF\x39\x8A\x5B\x67\xC1\x0B\x53\xA5\x02\x4D"};
    HI_U8 out_tag[16];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}
    u32InputAddrPhy = (HI_U32)HI_MMZ_New(64, 0, NULL, "CIPHER_BufIn");
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
        goto __CIPHER_EXIT__;
    }
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);
    u32OutPutAddrPhy = (HI_U32)HI_MMZ_New(64, 0, NULL, "CIPHER_BufOut");
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32Testcached);

    memset(&stGCM, 0, sizeof(HI_UNF_CIPHER_GCM_INFO_S));
    stGCM.pu8Aad = aes_a;
    stGCM.u32ALen = 64;
    stGCM.u32MLen = 64;
    stGCM.u32IVLen = 12;
    u32TestDataLen = stGCM.u32MLen;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("GCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_a, sizeof(aes_a));
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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
#endif //CIPHER_CCM_GCM_SUPPORT
#endif

#ifdef VIR_ADDR_ENABLE
#ifdef CIPHER_MULTICIPHER_SUPPORT
/* encrypt data using special chn*/
static HI_S32 CBC_AES128_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 16;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U8 aes_key[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
    HI_U8 aes_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
    HI_U8 aes_src[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
    HI_U8 aes_dst[16] = {0x76,0x49,0xAB,0xAC,0x81,0x19,0xB2,0x46,0xCE,0xE9,0x8E,0x9B,0x12,0xE9,0x19,0x7D};

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CBC,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CBC-AES-128-ORI:", aes_src, sizeof(aes_src));

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CBC-AES-128-ENC:", pOutputAddrVir, sizeof(aes_dst));

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_UNF_CIPHER_KEY_SRC_USER,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CBC,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CBC-AES-128-DEC:", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

__CIPHER_EXIT__:

    if (pInputAddrVir> 0)
    {
        free(pInputAddrVir);
        pInputAddrVir = HI_NULL;
    }
    if (pOutputAddrVir > 0)
    {
        free(pOutputAddrVir);
        pOutputAddrVir = HI_NULL;
    }

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();

    return s32Ret;
}

static HI_S32 CFB_AES128_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 32;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U8 aes_key[16] = {"\x2B\x7E\x15\x16\x28\xAE\xD2\xA6\xAB\xF7\x15\x88\x09\xCF\x4F\x3C"};
    HI_U8 aes_IV[16]  = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"};
    HI_U8 aes_src[32] = {"\x6B\xC1\xBE\xE2\x2E\x40\x9F\x96\xE9\x3D\x7E\x11\x73\x93\x17\x2A\xAE\x2D\x8A\x57\x1E\x03\xAC\x9C\x9E\xB7\x6F\xAC\x45\xAF\x8E\x51"};
    HI_U8 aes_dst[32] = {"\x3B\x3F\xD9\x2E\xB7\x2D\xAD\x20\x33\x34\x49\xF8\xE8\x3C\xFB\x4A\xC8\xA6\x45\x37\xA0\xB3\xA9\x3F\xCD\xE3\xCD\xAD\x9F\x1C\xE5\x8B"};

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CFB,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                             aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CFB-AES-128-ORI:", aes_src, u32TestDataLen);

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CFB-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_UNF_CIPHER_KEY_SRC_USER,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CFB,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CFB-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

static HI_S32 CTR_AES128_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 32;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U8 aes_key[16] = {"\x7E\x24\x06\x78\x17\xFA\xE0\xD7\x43\xD6\xCE\x1F\x32\x53\x91\x63"};
    HI_U8 aes_IV[16]  = {"\x00\x6C\xB6\xDB\xC0\x54\x3B\x59\xDA\x48\xD9\x0B\x00\x00\x00\x01"};
    HI_U8 aes_src[32] = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"};
    HI_U8 aes_dst[32] = {"\x51\x04\xA1\x06\x16\x8A\x72\xD9\x79\x0D\x41\xEE\x8E\xDA\xD3\x88\xEB\x2E\x1E\xFC\x46\xDA\x57\xC8\xFC\xE6\x30\xDF\x91\x41\xBE\x28"};

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CTR,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                             aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CTR-AES-128-ORI:", aes_src, u32TestDataLen);

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CTR-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_UNF_CIPHER_KEY_SRC_USER,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CTR,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                     aes_key, aes_IV);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CTR-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

#ifdef CIPHER_CCM_GCM_SUPPORT
// Klen  = 128, Tlen =32, Nlen  = 56,  Alen  = 64, and  Plen  = 32
// t = 4, q=8, p=4
HI_S32 CCM_AES128_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 4;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_CCM_INFO_S stCCM;
    HI_U8 aes_key[16] = {"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"};
    HI_U8 aes_n[7] = {"\x10\x11\x12\x13\x14\x15\x16"};
    HI_U8 aes_a[8] = {"\x00\x01\x02\x03\x04\x05\x06\x07"};
    HI_U8 aes_src[4] = {"\x20\x21\x22\x23"};
    HI_U8 aes_dst[4] = {"\x71\x62\x01\x5b"};
    HI_U8 aes_tag[4] = {"\x4d\xac\x25\x5d"};
    HI_U8 out_tag[4];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(32);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(32);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

    memset(&stCCM, 0, sizeof(HI_UNF_CIPHER_CCM_INFO_S));
    memcpy(&stCCM.u8Nonce, aes_n, sizeof(aes_n));
    stCCM.pu8Aad = aes_a;
    stCCM.u32ALen = 8;
    stCCM.u8NLen = 7;
    stCCM.u32MLen = 4;
    stCCM.u8TLen = 4;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_a, sizeof(aes_a));
	s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

/* *****************************************************************************
 * In the following example,  Klen  = 128, Tlen =112,  Nlen  = 104,
 * Alen  = 524288, and  Plen  = 256.
 ******************************************************************************/
static HI_S32 CCM_AES128_2_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 32;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_U32 i;
    HI_UNF_CIPHER_CCM_INFO_S stCCM;
    HI_U8 aes_key[16] = {"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"};
    HI_U8 aes_n[13] = {"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c"};
    static HI_U8 aes_a[65536];
    HI_U8 aes_src[32] = {"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
                         "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"};
    HI_U8 aes_dst[32] = {"\x69\x91\x5d\xad\x1e\x84\xc6\x37\x6a\x68\xc2\x96\x7e\x4d\xab\x61"
                         "\x5a\xe0\xfd\x1f\xae\xc4\x4c\xc4\x84\x82\x85\x29\x46\x3c\xcf\x72"};
    HI_U8 aes_tag[14] = {"\xb4\xac\x6b\xec\x93\xe8\x59\x8e\x7f\x0d\xad\xbc\xea\x5b"};
    HI_U8 out_tag[14];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    for(i=0; i<65536; i++)
    {
        aes_a[i] = i;
    }

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(u32TestDataLen);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

    memset(&stCCM, 0, sizeof(HI_UNF_CIPHER_CCM_INFO_S));
    memcpy(&stCCM.u8Nonce, aes_n, sizeof(aes_n));
    stCCM.pu8Aad = aes_a;
    stCCM.u32ALen = 65536;
    stCCM.u8NLen = 13;
    stCCM.u32MLen = u32TestDataLen;
    stCCM.u8TLen = 14;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("CCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
	s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, HI_NULL,
                            &stCCM, HI_NULL);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("CCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("CCM-AES-128-TAG", out_tag, stCCM.u8TLen);
    if ( 0 != memcmp(out_tag, aes_tag, stCCM.u8TLen) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }
    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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

static HI_S32 GCM_AES128_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 60;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_GCM_INFO_S stGCM;
    HI_U8 aes_key[32] = {"\xfe\xff\xe9\x92\x86\x65\x73\x1c\x6d\x6a\x8f\x94\x67\x30\x83\x08"};
    HI_U8 aes_iv[12] =  {"\xca\xfe\xba\xbe\xfa\xce\xdb\xad\xde\xca\xf8\x88"};
    HI_U8 aes_a[20] =   {"\xfe\xed\xfa\xce\xde\xad\xbe\xef\xfe\xed\xfa\xce\xde\xad\xbe\xef\xab\xad\xda\xd2"};
    HI_U8 aes_src[60] = {"\xd9\x31\x32\x25\xf8\x84\x06\xe5\xa5\x59\x09\xc5\xaf\xf5\x26\x9a"
                         "\x86\xa7\xa9\x53\x15\x34\xf7\xda\x2e\x4c\x30\x3d\x8a\x31\x8a\x72"
                         "\x1c\x3c\x0c\x95\x95\x68\x09\x53\x2f\xcf\x0e\x24\x49\xa6\xb5\x25"
                         "\xb1\x6a\xed\xf5\xaa\x0d\xe6\x57\xba\x63\x7b\x39"};
    HI_U8 aes_dst[60] = {"\x42\x83\x1e\xc2\x21\x77\x74\x24\x4b\x72\x21\xb7\x84\xd0\xd4\x9c"
                         "\xe3\xaa\x21\x2f\x2c\x02\xa4\xe0\x35\xc1\x7e\x23\x29\xac\xa1\x2e"
                         "\x21\xd5\x14\xb2\x54\x66\x93\x1c\x7d\x8f\x6a\x5a\xac\x84\xaa\x05"
                         "\x1b\xa3\x0b\x39\x6a\x0a\xac\x97\x3d\x58\xe0\x91"};
    HI_U8 aes_tag[16] = {"\x5b\xc9\x4f\xbc\x32\x21\xa5\xdb\x94\xfa\xe9\x5a\xe7\x12\x1a\x47"};
    HI_U8 out_tag[16];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(64);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(64);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

    memset(&stGCM, 0, sizeof(HI_UNF_CIPHER_GCM_INFO_S));
    stGCM.pu8Aad = aes_a;
    stGCM.u32ALen = 20;
    stGCM.u32MLen = 60;
    stGCM.u32IVLen = 12;
    u32TestDataLen = stGCM.u32MLen;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("GCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_a, sizeof(aes_a));
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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
static HI_S32 GCM_AES128_2_VIR(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 64;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8 *pInputAddrVir = HI_NULL;
    HI_U8 *pOutputAddrVir = HI_NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_GCM_INFO_S stGCM;
    HI_U8 aes_key[32] = {"\xFE\xFF\xE9\x92\x86\x65\x73\x1C\x6D\x6A\x8F\x94\x67\x30\x83\x08"};
    HI_U8 aes_iv[12] =  {"\xCA\xFE\xBA\xBE\xFA\xCE\xDB\xAD\xDE\xCA\xF8\x88"};
    HI_U8 aes_a[64] =   {"\x3A\xD7\x7B\xB4\x0D\x7A\x36\x60\xA8\x9E\xCA\xF3\x24\x66\xEF\x97"
                         "\xF5\xD3\xD5\x85\x03\xB9\x69\x9D\xE7\x85\x89\x5A\x96\xFD\xBA\xAF"
                         "\x43\xB1\xCD\x7F\x59\x8E\xCE\x23\x88\x1B\x00\xE3\xED\x03\x06\x88"
                         "\x7B\x0C\x78\x5E\x27\xE8\xAD\x3F\x82\x23\x20\x71\x04\x72\x5D\xD4"};
    HI_U8 aes_src[64] = {"\xD9\x31\x32\x25\xF8\x84\x06\xE5\xA5\x59\x09\xC5\xAF\xF5\x26\x9A"
                         "\x86\xA7\xA9\x53\x15\x34\xF7\xDA\x2E\x4C\x30\x3D\x8A\x31\x8A\x72"
                         "\x1C\x3C\x0C\x95\x95\x68\x09\x53\x2F\xCF\x0E\x24\x49\xA6\xB5\x25"
                         "\xB1\x6A\xED\xF5\xAA\x0D\xE6\x57\xBA\x63\x7B\x39\x1A\xAF\xD2\x55"};
    HI_U8 aes_dst[64] = {"\x42\x83\x1E\xC2\x21\x77\x74\x24\x4B\x72\x21\xB7\x84\xD0\xD4\x9C"
                         "\xE3\xAA\x21\x2F\x2C\x02\xA4\xE0\x35\xC1\x7E\x23\x29\xAC\xA1\x2E"
                         "\x21\xD5\x14\xB2\x54\x66\x93\x1C\x7D\x8F\x6A\x5A\xAC\x84\xAA\x05"
                         "\x1B\xA3\x0B\x39\x6A\x0A\xAC\x97\x3D\x58\xE0\x91\x47\x3F\x59\x85"};
    HI_U8 aes_tag[16] = {"\x64\xC0\x23\x29\x04\xAF\x39\x8A\x5B\x67\xC1\x0B\x53\xA5\x02\x4D"};
    HI_U8 out_tag[16];

    printf("\n--------------------------%s-----------------------\n", __FUNCTION__);

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return s32Ret;
	}
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid);
    if(HI_SUCCESS != s32Ret)
	{
		HI_UNF_CIPHER_DeInit();
		return s32Ret;
	}

    pInputAddrVir = (HI_U8*)malloc(64);
    if (pInputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pInputAddrVir failed\n");
        return HI_FAILURE;
    }

    pOutputAddrVir = (HI_U8*)malloc(64);
    if (pOutputAddrVir == NULL)
    {
        HI_ERR_CIPHER("malloc pOutputAddrVir failed\n");
        return HI_FAILURE;
    }

    memset(&stGCM, 0, sizeof(HI_UNF_CIPHER_GCM_INFO_S));
    stGCM.pu8Aad = aes_a;
    stGCM.u32ALen = 64;
    stGCM.u32MLen = 64;
    stGCM.u32IVLen = 12;
    u32TestDataLen = stGCM.u32MLen;

	/* For encrypt */
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, aes_src, u32TestDataLen);
    printBuffer("GCM-AES-128-ORI:", aes_src, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_EncryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-ENC:", pOutputAddrVir, u32TestDataLen);

    /* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_dst, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

   /* For decrypt */
    memcpy(pInputAddrVir, aes_a, sizeof(aes_a));
    s32Ret = SetconfiginfoEx(hTestchnid,
                            HI_UNF_CIPHER_KEY_SRC_USER,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_GCM,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key, aes_iv,
                            HI_NULL, &stGCM);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(pInputAddrVir, aes_dst, u32TestDataLen);
    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    s32Ret = HI_UNF_CIPHER_DecryptVir(hTestchnid, pInputAddrVir, pOutputAddrVir, u32TestDataLen);
    if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = HI_FAILURE;
		goto __CIPHER_EXIT__;
	}

    printBuffer("GCM-AES-128-DEC", pOutputAddrVir, u32TestDataLen);
	/* compare */
    if ( 0 != memcmp(pOutputAddrVir, aes_src, u32TestDataLen) )
    {
        HI_ERR_CIPHER("Memcmp failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_GetTag(hTestchnid, out_tag);
	if(HI_SUCCESS != s32Ret)
	{
		HI_ERR_CIPHER("Get tag failed.\n");
		goto __CIPHER_EXIT__;
	}
    printBuffer("GCM-AES-128-TAG", out_tag, 16);
    if ( 0 != memcmp(out_tag, aes_tag, 16) )
    {
        HI_ERR_CIPHER("Tag compare failed!\n");
        s32Ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    printf("\033[0;1;32m""\%s test pass !!!\n""\033[0m", __FUNCTION__);

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
#endif //CIPHER_CCM_GCM_SUPPORT
#endif
#endif

#ifdef CIPHER_MULTICIPHER_SUPPORT
int sample_cipher(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifdef PHY_ADDR_ENABLE
    s32Ret = CBC_AES128();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = CFB_AES128();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = CTR_AES128();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

#ifdef  CIPHER_CCM_GCM_SUPPORT
    //s32Ret = CCM_AES128();
    //if (s32Ret != HI_SUCCESS)
    //{
    //    return s32Ret;
    //}
    s32Ret = CCM_AES128_2();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    //s32Ret = GCM_AES128();
    //if (s32Ret != HI_SUCCESS)
    //{
    //    return s32Ret;
    //}
    s32Ret = GCM_AES128_2();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
#endif
#endif

#ifdef VIR_ADDR_ENABLE

    s32Ret = CBC_AES128_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = CFB_AES128_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = CTR_AES128_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

#ifdef  CIPHER_CCM_GCM_SUPPORT
    s32Ret = CCM_AES128_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = CCM_AES128_2_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = GCM_AES128_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = GCM_AES128_2_VIR();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
#endif

#endif

    return HI_SUCCESS;
}
#else
int sample_cipher()
{
    printf("cipher not support!!!\n");
    return HI_SUCCESS;
}
#endif

