/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2012/07/10
Last Modified :
Description   :    sample for cipher
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

#include "hi_types.h"
#include "hi_unf_cipher.h"
#include "hi_mmz_api.h"

#define CIPHER_KLAD_SUPPORT

#define HI_ERR_CIPHER(format, arg...)     printf( "\033[0;1;31m" format "\033[0m", ## arg)
#define HI_INFO_CIPHER(format, arg...)    printf( "\033[0;1;32m" format "\033[0m", ## arg)
#define TEST_END_PASS()                   HI_INFO_CIPHER("****************** %s test PASS !!! ******************\n", __FUNCTION__)
#define TEST_END_FAIL()                   HI_ERR_CIPHER("****************** %s test FAIL !!! ******************\n", __FUNCTION__)
#define TEST_RESULT_PRINT()               { if (ret) TEST_END_FAIL(); else TEST_END_PASS();}

#if defined(CHIP_TYPE_hi3516cv500) || defined(CHIP_TYPE_hi3516ev200)
/* 32byte otp key */
static HI_U8 aes_128_cbc_key[16] = {0x00,0xe9,0x85,0xde,0x41,0x67,0x6f,0x00,0x96,0x94,0x41,0x54,0x02,0x7d,0xe7,0xc5};
static HI_U8 aes_128_enc_key[16] = {0x50,0xDB,0x86,0xC5,0x92,0xC5,0x2F,0x0C,0x43,0x6C,0xCA,0x6F,0x2F,0xFE,0xCA,0xF5};
static HI_U8 aes_128_cbc_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static HI_U8 aes_128_src_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
static HI_U8 aes_128_dst_buf[16] = {0xF0,0XB7,0X6A,0X0C,0X16,0X06,0XD4,0XDE,0X79,0XCB,0X5C,0X66,0XD3,0X51,0X0C,0XAB};

#if 0
/* 32byte otp key,32byte encrypt key */
static HI_U8 aes_128_cbc_key[32] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
                                    0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
static HI_U8 aes_128_enc_key[32] = {0x86,0x3a,0xe9,0x1f,0xd4,0xcf,0x51,0x13,0x2d,0xc3,0xe4,0xed,0xf0,0x9d,0xbc,0x6e,
                                    0x86,0x3a,0xe9,0x1f,0xd4,0xcf,0x51,0x13,0x2d,0xc3,0xe4,0xed,0xf0,0x9d,0xbc,0x6e};
static HI_U8 aes_128_cbc_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static HI_U8 aes_128_src_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
static HI_U8 aes_128_dst_buf[16] = {0xe0,0xfa,0xab,0x42,0xd5,0xa5,0x5c,0x18,0xb0,0x13,0x1f,0x47,0x6c,0xa5,0xd0,0x0a};

/*32byte otp key,24byte encrypt key */
static HI_U8 aes_128_cbc_key[32] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
                                    0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
static HI_U8 aes_128_enc_key[32] = {0x86,0x3a,0xe9,0x1f,0xd4,0xcf,0x51,0x13,0x2d,0xc3,0xe4,0xed,0xf0,0x9d,0xbc,0x6e,
                                    0x86,0x3a,0xe9,0x1f,0xd4,0xcf,0x51,0x13,0x2d,0xc3,0xe4,0xed,0xf0,0x9d,0xbc,0x6e};
static HI_U8 aes_128_cbc_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static HI_U8 aes_128_src_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
static HI_U8 aes_128_dst_buf[16] = {0x69,0x25,0x8f,0xc2,0x8d,0x58,0x8b,0x84,0x67,0x87,0x76,0x02,0x2f,0x9c,0x1c,0x91};
#endif

#else
/* 16byte otp key */
static HI_U8 aes_128_cbc_key[16] = {0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00};
static HI_U8 aes_128_enc_key[16]  = {0xc1,0x1b,0x54,0x4a,0x12,0x9c,0x08,0xa5,0xcc,0xd3,0xeb,0xec,0x7a,0x3b,0x00,0x2b};
static HI_U8 aes_128_cbc_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static HI_U8 aes_128_src_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
static HI_U8 aes_128_dst_buf[16] = {0xb0,0x1b,0x77,0x09,0xe8,0xdc,0xf9,0xef,0x37,0x13,0x0b,0x13,0xda,0x11,0xbf,0x24};
#endif

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

static HI_S32 Setconfiginfo(HI_HANDLE chnHandle, HI_UNF_CIPHER_CA_TYPE_E enKeySrc, HI_UNF_CIPHER_ALG_E alg, HI_UNF_CIPHER_WORK_MODE_E mode, HI_UNF_CIPHER_KEY_LENGTH_E keyLen,
                                                const HI_U8 u8KeyBuf[32], const HI_U8 u8IVBuf[16])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = keyLen;
    CipherCtrl.enCaType = enKeySrc;
    CipherCtrl.bKeyByCA = HI_TRUE;

    if(CipherCtrl.enWorkMode != HI_UNF_CIPHER_WORK_MODE_ECB)
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    if(keyLen == HI_UNF_CIPHER_KEY_AES_128BIT)
    {
        memcpy(CipherCtrl.u32Key, u8KeyBuf, 16);
    }else
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
//HI_S32 main(int argc, char* argv[])
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
    HI_UNF_CIPHER_CA_TYPE_E enKeySrc;
    HI_U8 u8Key[32];
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    memset(u8Key, 0x0, 32);

    enKeySrc = HI_UNF_CIPHER_KEY_SRC_KLAD_1;

#ifdef CIPHER_KLAD_SUPPORT
    s32Ret = HI_UNF_CIPHER_KladEncryptKey(HI_UNF_CIPHER_KEY_SRC_KLAD_1, HI_UNF_CIPHER_KLAD_TARGET_AES, aes_128_cbc_key, u8Key, 16);
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
#endif

    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_COPY_AVOID;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
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
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32TestDataLen, u32Testcached);
    u32OutPutAddrPhy = (HI_U32)HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufOut");
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32TestDataLen, u32Testcached);
    if (0 == pOutputAddrVir)
    {
        HI_ERR_CIPHER("Errror: Get Viraddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }

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

    TEST_END_PASS();

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
