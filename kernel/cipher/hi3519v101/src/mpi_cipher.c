/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "hi_mpi_cipher.h"
#include "hi_error_mpi.h"
#include "drv_cipher_reg.h"
#include "hi_mmz_api.h"
#include "config.h"

#define HI_PRINT printf
#define HI_FATAL_CIPHER(format, arg...)    HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_ERR_CIPHER(format, arg...)      HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)     //HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)

#define CIPHER_MAX_LIST_NUM    (128)
#define RSA_MIN_KEY_LEN        (128)
#define RSA_MAX_KEY_LEN        (512)
#define U32_MAX_NUM            (0xFFFFFFFF)

HI_S32 g_CipherDevFd = -1;
HI_S32 g_CipherInitCounter = -1;
static pthread_mutex_t   g_CipherMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_CIPHER_LOCK()       (void)pthread_mutex_lock(&g_CipherMutex);
#define HI_CIPHER_UNLOCK()     (void)pthread_mutex_unlock(&g_CipherMutex);

#define CHECK_CIPHER_OPEN()\
do{\
    HI_CIPHER_LOCK();\
    if (g_CipherDevFd< 0 || g_CipherInitCounter < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        HI_CIPHER_UNLOCK();\
        return HI_ERR_CIPHER_NOT_INIT;\
    }\
    HI_CIPHER_UNLOCK();\
}while(0)

#define RSA_SIGN          1
#define ASN1_HASH_SHA1    "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14"
#define ASN1_HASH_SHA256  "\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20"
static const HI_S8 EMPTY_L_SHA1[]   = "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09";
static const HI_S8 EMPTY_L_SHA256[] = "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55";

#ifdef CIPHER_HASH_SUPPORT

static pthread_mutex_t   g_HashMutex = PTHREAD_MUTEX_INITIALIZER;
#define HI_HASH_LOCK()           (void)pthread_mutex_lock(&g_HashMutex);
#define HI_HASH_UNLOCK()         (void)pthread_mutex_unlock(&g_HashMutex);

#define HASH_BLOCK_SIZE          (64)
#define HASH_PAD_MAX_LEN         (64)
#define HASH1_SIGNATURE_SIZE     (20)
#define HASH256_SIGNATURE_SIZE   (32)
#define HASH_MMZ_BUF_LEN         (1*1024*1024)        //1M
#define HASH_MMZ_TAIL_LEN        (8*1024)             //8K
#define HASH_CHANNAL_MAX_NUM     (8)

typedef struct hiCI_MMZ_BUF_S
{
    HI_U32 u32StartPhyAddr;
    HI_U8* pu8StartVirAddr;
    HI_U32 u32Size;
}MMZ_BUFFER_S;

//static HASH_INFO_S g_stCipherHashData[HASH_CHANNAL_MAX_NUM];
static MMZ_BUFFER_S s_stMMZBuffer;
//HI_U32 g_u32HashBaseBufferLen = HASH_MMZ_BUF_LEN;


static void _memset(void *s, int c, size_t n)
{
    int i;
    char *p = s;
    if ((HI_NULL == s) && (0 != n))
    {
        HI_ERR_CIPHER("_memset failed, dest point is null.\n");
        return;
    }

    for(i=0; i<n; i++)
        p[i] = c;

    return;
}

static HI_VOID Cipher_HashMmzDeInit(HI_VOID);

static HI_S32 Cipher_HashMmzInit(HI_VOID)
{
    return HI_SUCCESS;
}

static HI_VOID Cipher_HashMmzDeInit(HI_VOID)
{
    return ;
}
#endif

HI_S32 HI_MPI_CIPHER_Init(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_CIPHER_LOCK();

    if (g_CipherInitCounter > 0)
    {
        g_CipherInitCounter++;
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    g_CipherDevFd = open("/dev/"UMAP_DEVNAME_CIPHER, O_RDWR, 0);
    if (g_CipherDevFd < 0)
    {
        HI_FATAL_CIPHER("Open CIPHER err.\n");
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_FAILED_INIT;
    }

#ifdef CIPHER_HASH_SUPPORT
    _memset(&s_stMMZBuffer, 0, sizeof(s_stMMZBuffer));
    ret = Cipher_HashMmzInit();
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Hash mmz buffer initial failed!\n");
        g_CipherInitCounter = -1;
        close(g_CipherDevFd);
        HI_CIPHER_UNLOCK();
        return HI_FAILURE;
    }
#endif
    g_CipherInitCounter = 1;

    HI_CIPHER_UNLOCK();

    return ret;
}

HI_S32 HI_MPI_CIPHER_DeInit(HI_VOID)
{
    HI_S32 Ret = HI_FAILURE;

    HI_CIPHER_LOCK();

    if(0 >= g_CipherInitCounter)
    {
        HI_CIPHER_UNLOCK();
		return HI_SUCCESS;
    }

    Ret = close(g_CipherDevFd);

    if(HI_SUCCESS != Ret)
    {
        HI_FATAL_CIPHER("Close cipher err.\n");
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_NOT_INIT;
    }

#ifdef CIPHER_HASH_SUPPORT
    Cipher_HashMmzDeInit();
    _memset(&s_stMMZBuffer, 0, sizeof(s_stMMZBuffer));

#endif
    g_CipherDevFd = -1;
    g_CipherInitCounter = -1;

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}

#ifdef CIPHER_MULTICIPHER_SUPPORT
HI_S32 HI_MPI_CIPHER_CreateHandle(HI_HANDLE* phCipher)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_HANDLE_S stCIHandle;

    if (  NULL == phCipher )
    {
        HI_ERR_CIPHER("Invalid param, null pointer!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    _memset(&stCIHandle, 0, sizeof(stCIHandle));

    Ret=ioctl(g_CipherDevFd, CMD_CIPHER_CREATEHANDLE, &stCIHandle);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *phCipher = stCIHandle.hCIHandle;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    HI_S32 Ret = HI_FAILURE;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd, CMD_CIPHER_DESTROYHANDLE, &hCipher);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_Config_CTRL configdata;

    CHECK_CIPHER_OPEN();

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    _memset(&configdata, 0, sizeof(configdata));

    memcpy(&configdata.CIpstCtrl, pstCtrl, sizeof(HI_UNF_CIPHER_CTRL_S));
    configdata.CIHandle=hCipher;

    if(configdata.CIpstCtrl.enWorkMode >= HI_UNF_CIPHER_WORK_MODE_BUTT)
    {
        HI_ERR_CIPHER("para set CIPHER wokemode is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    Ret=ioctl(g_CipherDevFd, CMD_CIPHER_CONFIGHANDLE, &configdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_DATA_S CIdata;

    CHECK_CIPHER_OPEN();

    if ( u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(&CIdata, 0, sizeof(CIdata));

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.u32DataLength=u32ByteLength;
    CIdata.CIHandle=hCipher;
    CIdata.bIsPhyAddr = HI_TRUE;

    Ret=ioctl(g_CipherDevFd, CMD_CIPHER_ENCRYPT, &CIdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_DATA_S CIdata;

    CHECK_CIPHER_OPEN();

    if (u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(&CIdata, 0, sizeof(CIdata));
    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.u32DataLength = u32ByteLength;
    CIdata.CIHandle=hCipher;
    CIdata.bIsPhyAddr = HI_TRUE;

    Ret=ioctl(g_CipherDevFd,CMD_CIPHER_DECRYPT, &CIdata);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_EncryptVir(HI_HANDLE hCipher, HI_U8 *pu8SrcData, HI_U8 *pu8DestData, HI_U32 u32ByteLength)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_DATA_S CIdata;

    CHECK_CIPHER_OPEN();

    if ( u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(&CIdata, 0, sizeof(CIdata));

    CIdata.bIsPhyAddr = HI_FALSE;
    CIdata.pu8Input=pu8SrcData;
    CIdata.pu8Output=pu8DestData;
    CIdata.u32DataLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    Ret=ioctl(g_CipherDevFd, CMD_CIPHER_ENCRYPT, &CIdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_DecryptVir(HI_HANDLE hCipher, HI_U8 *pu8SrcData, HI_U8 *pu8DestData, HI_U32 u32ByteLength)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_DATA_S CIdata;

    CHECK_CIPHER_OPEN();

    if (u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(&CIdata, 0, sizeof(CIdata));

    CIdata.bIsPhyAddr = HI_FALSE;
    CIdata.pu8Input=pu8SrcData;
    CIdata.pu8Output=pu8DestData;
    CIdata.u32DataLength = u32ByteLength;
    CIdata.CIHandle=hCipher;
    CIdata.bIsPhyAddr = HI_FALSE;

    Ret=ioctl(g_CipherDevFd,CMD_CIPHER_DECRYPT, &CIdata);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_EncryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 chnid = 0;
    HI_U32 i = 0;
    CIPHER_DATA_S CIdata;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = HI_NULL;

    CHECK_CIPHER_OPEN();

    if (pstDataPkg == HI_NULL)
    {
        HI_ERR_CIPHER("invalid point.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(0 == u32DataPkgNum)
    {
        HI_ERR_CIPHER("invalid u32DataPkgNum is zero\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(CIPHER_MAX_LIST_NUM < u32DataPkgNum)
    {
        HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\n",u32DataPkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    chnid=hCipher&0x00ff;
    if ( 0 == chnid )
    {
        HI_ERR_CIPHER("invalid chnid 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if (pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        if ((pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Invalid src phy addr or dest phy addr.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    _memset(&CIdata, 0, sizeof(CIdata));

    CIdata.ScrPhyAddr=(HI_U32)pstDataPkg;
    CIdata.DestPhyAddr= 0;
    CIdata.u32DataLength=u32DataPkgNum;
    CIdata.CIHandle=hCipher;

    Ret=ioctl(g_CipherDevFd,CMD_CIPHER_ENCRYPTMULTI, &CIdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_DecryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 chnid = 0;
    HI_U32 i = 0;
    CIPHER_DATA_S CIdata;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = HI_NULL;

    CHECK_CIPHER_OPEN();

    if (pstDataPkg == HI_NULL)
    {
        HI_ERR_CIPHER("invalid point.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(0 == u32DataPkgNum)
    {
        HI_ERR_CIPHER("invalid u32DataPkgNum is zero\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(CIPHER_MAX_LIST_NUM < u32DataPkgNum)
    {
        HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\n",u32DataPkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    chnid=hCipher&0x00ff;
    if ( 0 == chnid )
    {
        HI_ERR_CIPHER("invalid chnid 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if (pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        if ((pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Invalid src phy addr or dest phy addr.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    _memset(&CIdata, 0, sizeof(CIdata));

    CIdata.ScrPhyAddr=(HI_U32)pstDataPkg;
    CIdata.DestPhyAddr= 0;
    CIdata.u32DataLength=u32DataPkgNum;
    CIdata.CIHandle=hCipher;

    Ret = ioctl(g_CipherDevFd,CMD_CIPHER_DECRYPTMULTI, &CIdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_EncryptMultiEx(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 i = 0;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = HI_NULL;
    CIPHER_MUTIL_EX_DATA_S stMutliEx;

    CHECK_CIPHER_OPEN();

    if ((pstCtrl == HI_NULL) || (pstDataPkg == HI_NULL))
    {
        HI_ERR_CIPHER("pstCtrl invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(0 == u32DataPkgNum)
    {
        HI_ERR_CIPHER("invalid u32DataPkgNum is zero\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(CIPHER_MAX_LIST_NUM < u32DataPkgNum)
    {
        HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\n",u32DataPkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if (pPkgTmp->u32ByteLength < 8 || pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        if ((pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Invalid src phy addr or dest phy addr.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    _memset(&stMutliEx, 0, sizeof(stMutliEx));

    stMutliEx.hCipher = hCipher;
    stMutliEx.CIpstCtrl = *pstCtrl;
    stMutliEx.pstPkg = pstDataPkg;
    stMutliEx.u32PkgNum = u32DataPkgNum;

    Ret=ioctl(g_CipherDevFd, HI_CIPHER_ENCRYPTMULTI_EX, &stMutliEx);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_MPI_CIPHER_DecryptMultiEx(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 i = 0;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = HI_NULL;
    CIPHER_MUTIL_EX_DATA_S stMutliEx;

    CHECK_CIPHER_OPEN();

    if ((pstCtrl == HI_NULL) || (pstDataPkg == HI_NULL))
    {
        HI_ERR_CIPHER("pstCtrl invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(0 == u32DataPkgNum)
    {
        HI_ERR_CIPHER("invalid u32DataPkgNum is zero\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(CIPHER_MAX_LIST_NUM < u32DataPkgNum)
    {
        HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\n",u32DataPkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if (pPkgTmp->u32ByteLength < 8 || pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        if ((pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Invalid src phy addr or dest phy addr.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    _memset(&stMutliEx, 0, sizeof(stMutliEx));

    stMutliEx.hCipher = hCipher;
    stMutliEx.CIpstCtrl = *pstCtrl;
    stMutliEx.pstPkg = pstDataPkg;
    stMutliEx.u32PkgNum = u32DataPkgNum;

    Ret=ioctl(g_CipherDevFd, HI_CIPHER_DECRYPTMULTI_EX, &stMutliEx);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;

}

#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 HI_MPI_CIPHER_GetTag(HI_HANDLE hCipherHandle,  HI_U8 *pstTag)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_TAG stTag;

    CHECK_CIPHER_OPEN();

    if (pstTag == HI_NULL)
    {
        HI_ERR_CIPHER("invalid para.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    _memset(&stTag, 0, sizeof(stTag));

    stTag.CIHandle = hCipherHandle;
    Ret = ioctl(g_CipherDevFd,CMD_CIPHER_GETTAG, &stTag);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pstTag, stTag.u32Tag, stTag.u32Len);

    return HI_SUCCESS;

}
#endif

HI_S32 HI_MPI_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_Config_CTRL  configdata;

    CHECK_CIPHER_OPEN();

    if (pstCtrl == HI_NULL)
    {
        HI_ERR_CIPHER("invalid para.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    _memset(&configdata, 0, sizeof(configdata));
    configdata.CIHandle = hCipherHandle;

    Ret = ioctl(g_CipherDevFd, CMD_CIPHER_GETHANDLECONFIG, &configdata);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    memcpy(pstCtrl, &configdata.CIpstCtrl, sizeof(configdata.CIpstCtrl));

    return HI_SUCCESS;
}

#ifdef CIPHER_KLAD_SUPPORT
HI_S32 HI_MPI_CIPHER_KladEncryptKey(HI_UNF_CIPHER_KEY_SRC_E enRootKey,
                                    HI_UNF_CIPHER_KLAD_TARGET_E enTarget,
                                    HI_U8 *pu8CleanKey, HI_U8* pu8EcnryptKey,
                                    HI_U32 u32KeyLen)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_KLAD_KEY_S stKlad;
    HI_U32 i = 0;

    if ((pu8CleanKey == HI_NULL) || (pu8EcnryptKey == HI_NULL))
    {
        HI_ERR_CIPHER("invalid para.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((u32KeyLen == 0) || (u32KeyLen % 16 != 0))
    {
        HI_ERR_CIPHER("invalid key len 0x%x.\n", u32KeyLen);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((enRootKey >= HI_UNF_CIPHER_KEY_SRC_BUTT) || (enTarget >= HI_UNF_CIPHER_KLAD_TARGET_BUTT))
    {
        HI_ERR_CIPHER("invalid para.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(&stKlad, 0, sizeof(stKlad));

    stKlad.enRootKey = enRootKey;
    stKlad.enTarget = enTarget;

    for(i=0; i<u32KeyLen/16; i++)
    {
        memcpy(stKlad.u32CleanKey, pu8CleanKey + i*16, 16);
        Ret=ioctl(g_CipherDevFd, CMD_CIPHER_KLAD_KEY, &stKlad);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
        memcpy(pu8EcnryptKey + i*16, stKlad.u32EncryptKey, 16);
    }

    return HI_SUCCESS;
}
#endif

#endif

#ifdef CIPHER_HASH_SUPPORT
HI_S32 HI_MPI_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_INIT_S stHashInit;

    if((NULL == pstHashAttr) || (NULL == pHashHandle))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_HASH_LOCK();

    _memset(&stHashInit, 0, sizeof(CIPHER_HASH_INIT_S));
    memcpy(&stHashInit.stHashAttr, pstHashAttr, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASHINIT, &stHashInit);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, CMD_CIPHER_CALCHASHINIT failed!\n");
        HI_HASH_UNLOCK();
        return ret;
    }

    *pHashHandle = stHashInit.HashHandle;

    HI_HASH_UNLOCK();

    return ret;
}

HI_S32 HI_MPI_CIPHER_HashUpdate(HI_HANDLE CIHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_UPDATE_S stHashUpdate;
    CHECK_CIPHER_OPEN();

    if(NULL == pu8InputData)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_HASH_LOCK();

    _memset(&stHashUpdate, 0, sizeof(CIPHER_HASH_UPDATE_S));

    stHashUpdate.HashHandle = CIHashHandle;
    stHashUpdate.pu8InputData = pu8InputData;
    stHashUpdate.u32InputDataLen = u32InputDataLen;

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASHUPDATE, &stHashUpdate);
    if(HI_SUCCESS != ret)
    {
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    HI_HASH_UNLOCK();

    return  HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_HashFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8OutputHash)
{
    CIPHER_HASH_FINISH_S stHashFinish;
    HI_S32 ret = HI_FAILURE;

    CHECK_CIPHER_OPEN();

    if(NULL == pu8OutputHash)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_HASH_LOCK();

    _memset(&stHashFinish, 0, sizeof(CIPHER_HASH_FINISH_S));

    stHashFinish.HashHandle = CIHashHandle;
    stHashFinish.pu8OutputHash = pu8OutputHash;

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASHFINAL, &stHashFinish);
    if(HI_SUCCESS != ret)
    {
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    HI_HASH_UNLOCK();

    return HI_SUCCESS;

}

HI_S32 HI_MPI_CIPHER_Hash(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                          HI_U32 u32DataPhyAddr, HI_U32 u32ByteLength,
                          HI_U8 *pu8OutputHash)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 u32ShaLen = 0;

    if((NULL == pstHashAttr) || (NULL == pu8OutputHash))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    HI_HASH_LOCK();

    if (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType)
    {
        u32ShaLen = 20;
    }
    else if (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType)
    {
        u32ShaLen = 32;
    }
    else
    {
        HI_ERR_CIPHER("Error, Invalid sha type 0x%x !\n", pstHashAttr->eShaType);
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    _memset(&stHashData, 0, sizeof(stHashData));

    stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_NONE;

    stHashData.enShaType = pstHashAttr->eShaType;
    stHashData.u32DataPhy = u32DataPhyAddr;
    stHashData.u32DataLen = u32ByteLength;
    stHashData.bIsPhyAddr = HI_TRUE;

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASH, &stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash final failed!\n");
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    memcpy(pu8OutputHash, stHashData.u32ShaVal, u32ShaLen);

    HI_HASH_UNLOCK();

    return ret;
}

#ifdef CIPHER_HASHVIR_SUPPORT
HI_S32 HI_MPI_CIPHER_HashVir(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                             HI_U8 *pu8InputData, HI_U32 u32ByteLength,
                             HI_U8 *pu8OutputHash)

{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 u32ShaLen = 0;

    if((NULL == pstHashAttr) || (NULL == pu8OutputHash) || (NULL == pu8InputData))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    HI_HASH_LOCK();

    if (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType)
    {
        u32ShaLen = 20;
    }
    else if (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType)
    {
        u32ShaLen = 32;
    }
    else
    {
        HI_ERR_CIPHER("Error, Invalid sha type 0x%x !\n", pstHashAttr->eShaType);
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    _memset(&stHashData, 0, sizeof(stHashData));

    stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_NONE;

    stHashData.enShaType = pstHashAttr->eShaType;
    stHashData.pu8Input = pu8InputData;
    stHashData.u32DataLen = u32ByteLength;
    stHashData.bIsPhyAddr = HI_FALSE;

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASH, &stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash final failed!\n");
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }
    memcpy(pu8OutputHash, stHashData.u32ShaVal, u32ShaLen);

    HI_HASH_UNLOCK();

    return ret;
}
#endif
#endif

#ifdef CIPHER_RNG_SUPPORT
HI_S32 HI_MPI_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber, HI_U32 u32TimeOutUs)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_RNG_S stRNG;

    if (NULL == pu32RandomNumber)
    {
        HI_ERR_CIPHER("Input pointer is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_OPEN();

    _memset(&stRNG, 0, sizeof(stRNG));

    stRNG.u32TimeOutUs = u32TimeOutUs;
    Ret = ioctl(g_CipherDevFd, CMD_CIPHER_GETRANDOMNUMBER, &stRNG);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Get random number failed, ret = %x\n", Ret);
        return Ret;
    }

    *pu32RandomNumber = stRNG.u32RNG;

    return HI_SUCCESS;
}
#endif

#ifdef CIPHER_RSA_SUPPORT
static HI_S32 RSA_GetAttr(HI_U32 u32SchEme,
                          HI_U16 u16NLen,
                          HI_U32 *pu32HLen,
                          HI_U32 *pu32KeyLen,
                          HI_UNF_CIPHER_HASH_TYPE_E *pstenHashType)
{
    if ((pu32HLen == HI_NULL) || (pu32KeyLen == HI_NULL) || (pstenHashType == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    *pu32HLen = 0;
    *pu32KeyLen = 0;
    *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_BUTT;

    if (RSA_MIN_KEY_LEN <= u16NLen && RSA_MAX_KEY_LEN >= u16NLen)
    {
        *pu32KeyLen = u16NLen;
    }
    else
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\n", u16NLen);
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    switch(u32SchEme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        *pu32HLen = 0;
        *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_BUTT;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
        *pu32HLen = 20;
        *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        *pu32HLen = 32;
        *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
        break;
    default:
        HI_ERR_CIPHER("RSA scheme (0x%x) is invalid.\n", u32SchEme);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 RSA_PKCS1_MGF1(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                             HI_U8  *pu8Seed,
                             HI_U32 u32Seedlen,
                             HI_U8 *pu8Mask, HI_U32 u32MaskLen)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 i,j, u32Outlen = 0;
    HI_U8 u8Cnt[4] = {0};
    HI_U8 u8Md[32] = {0};
    HI_U32 u32MdLen;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle = 0;

    if ((pu8Seed == HI_NULL) || (pu8Mask == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    _memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    /*PKCS#1 V2.1 only use sha1 function, Others allow for future expansion*/
    stHashAttr.eShaType = enHashType;
    if( enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA1)
    {
        u32MdLen = 20;
    }
    else if( enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA256)
    {
        u32MdLen = 32;
    }
    else
    {
        HI_ERR_CIPHER("hash type is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; u32Outlen < u32MaskLen; i++)
    {
        u8Cnt[0] = (HI_U8)((i >> 24) & 0xFF);
        u8Cnt[1] = (HI_U8)((i >> 16) & 0xFF);
        u8Cnt[2] = (HI_U8)((i >> 8)) & 0xFF;
        u8Cnt[3] = (HI_U8)(i & 0xFF);

        Ret = HI_MPI_CIPHER_HashInit(&stHashAttr, &HashHandle);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
        Ret = HI_MPI_CIPHER_HashUpdate(HashHandle, pu8Seed, u32Seedlen);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
        Ret = HI_MPI_CIPHER_HashUpdate(HashHandle, u8Cnt, 4);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
        Ret = HI_MPI_CIPHER_HashFinal(HashHandle, u8Md);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
        for(j = 0; (j < u32MdLen) && (u32Outlen < u32MaskLen); j++)
        {
            pu8Mask[u32Outlen++] ^= u8Md[j];
        }
    }

    return HI_SUCCESS;
}

static HI_S32 RSA_GetRandomNumber(HI_U8 *pu8Rand, HI_U32 u32Size)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 i = 0;
    HI_U32 u32Rand = 0;

    _memset(pu8Rand, 0, u32Size);
    for(i=0; i<u32Size; i+=4)
    {
        ret = HI_MPI_CIPHER_GetRandomNumber(&u32Rand, -1);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
            return ret;
        }
        pu8Rand[i+3] = (HI_U8)(u32Rand >> 24)& 0xFF;
        pu8Rand[i+2] = (HI_U8)(u32Rand >> 16)& 0xFF;
        pu8Rand[i+1] = (HI_U8)(u32Rand >> 8)& 0xFF;
        pu8Rand[i+0] = (HI_U8)(u32Rand) & 0xFF;
    }

    /*non-zero random octet string*/
    for(i=0; i<u32Size; i++)
    {
        if (pu8Rand[i] == 0x00)
        {
            ret = HI_MPI_CIPHER_GetRandomNumber(&u32Rand, -1);
            if (ret != HI_SUCCESS)
            {
                HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
                return ret;
            }
            pu8Rand[i] = (HI_U8)(u32Rand) & 0xFF;
            i = 0;
        }
    }

    return HI_SUCCESS;
}

static HI_U32 HI_MPI_CIPHER_GetBitNum(HI_U8 *pu8BigNum, HI_U32 u32NumLen)
{
    static const HI_S8 u8Bits[16] = {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};
    HI_U32 i = 0;
    HI_U32 u32Num = 0;

    for(i=0; i<u32NumLen; i++)
    {
        u32Num = u8Bits[(pu8BigNum[i] & 0xF0) >> 4] ;
        if(u32Num > 0)
        {
            return (u32NumLen - i - 1) * 8 + u32Num + 4;
        }
        u32Num = u8Bits[pu8BigNum[i] & 0xF] ;
        if(u32Num > 0)
        {
            return (u32NumLen - i - 1) * 8 + u32Num;
        }
    }
    return  0;
}

/*PKCS #1: EME-OAEP encoding*/
/*************************************************************
                    +----------+---------+--+-------+
               DB = |  lHash   |    PS   |01|   M   |
                    +----------+---------+--+-------+
                                   |
         +----------+              V
         |   seed   |--> MGF ---> xor
         +----------+              |
               |                   |
      +--+     V                   |
      |00|    xor <----- MGF <-----|
      +--+     |                   |
        |      |                   |
        V      V                   V
      +--+----------+----------------------------+
EM =  |00|maskedSeed|          maskedDB          |
      +--+----------+----------------------------+
       1     hlen              k - hlen- 1

so: PS_LEN = k - hlen - 1 -  (hlen  + mlen + 1) = k - 2hlen - mlen - 2 > 0
so: mlen < k - 2hlen - 2
*************************************************************/
static HI_S32 RSA_padding_add_PKCS1_OAEP(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                  HI_U32 u32HLen,
                                  HI_U32 u32KeyLen,
                                  HI_U8 *pu8Input,
                                  HI_U32 u32InLen,
                                  HI_U8 *pu8Output,
                                  HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32DBLen = 0;
    HI_U8 *pu8DB = HI_NULL;
    HI_U8 *pu8Seed  = HI_NULL;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;

    CHECK_CIPHER_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /*In the v2.1 of PKCS #1, L is the empty string;    */
    /*other uses outside the scope of rsa specifications*/
    if( enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA256)
    {
        pu8LHASH = EMPTY_L_SHA256;
    }

    if ((u32InLen > u32KeyLen - 2 * u32HLen - 2) || (u32KeyLen < 2 * u32HLen + 2))
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0;
    pu8Output[0] = 0;
    pu8Seed = pu8Output + 1;
    pu8DB = pu8Output + u32HLen + 1;
    u32DBLen = u32KeyLen - u32HLen -1;

    memcpy(pu8DB, pu8LHASH, u32HLen);                                  /*set lHash*/
    _memset(&pu8DB[u32HLen], 0, u32DBLen - u32InLen - u32HLen -1);     /*set PS with 0x00*/
    pu8DB[u32DBLen - u32InLen - 1] = 0x01;                             /*set 0x01 after PS*/
    memcpy(&pu8DB[u32DBLen - u32InLen], pu8Input, u32InLen);           /*set M*/
    ret = RSA_GetRandomNumber(pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
        return ret;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return ret;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8DB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return ret;
    }
    *pu32OutLen = u32KeyLen;

    return HI_SUCCESS;
}

/*PKCS #1: RSAES-PKCS1-V1_5-ENCRYPT*/
/*************************************************
EM = 0x00 || 0x02 || PS || 0x00 || M

PS_LEN > 8, mlen < u32KeyLen - 11
*************************************************/
static HI_S32 RSA_padding_add_PKCS1_V15( HI_U32 u32KeyLen,
                                 HI_U8 *pu8Input,
                                 HI_U32 u32InLen,
                                 HI_U8 *pu8Output,
                                 HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32Index = 0;

    CHECK_CIPHER_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen > u32KeyLen - 11)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0;
    pu8Output[u32Index++] = 0x00;
    pu8Output[u32Index++] = 0x02;
    ret = RSA_GetRandomNumber(&pu8Output[u32Index], u32KeyLen - u32InLen -3);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
        return ret;
    }

    u32Index+=u32KeyLen - u32InLen -3;
    pu8Output[u32Index++] = 0x00;
    memcpy(&pu8Output[u32Index], pu8Input, u32InLen);

    *pu32OutLen = u32KeyLen;

    return HI_SUCCESS;
}

/*PKCS #1: block type 0,1,2 message padding*/
/*************************************************
EB = 00 || BT || PS || 00 || D

PS_LEN >= 8, mlen < u32KeyLen - 11
*************************************************/
static HI_S32 RSA_padding_add_PKCS1_type(HI_U32 u32KeyLen,
                               HI_U8  u8BT,
                               HI_U8 *pu8Input,
                               HI_U32 u32InLen,
                               HI_U8 *pu8Output,
                               HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32PLen = 0;
    HI_U8 *pu8EB = HI_NULL;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen > u32KeyLen - 11)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen =0;
    pu8EB = pu8Output;

    *(pu8EB++) = 0;
    *(pu8EB++) = u8BT;                 /* Private Key BT (Block Type) */

    /* pad out with 0xff data */
    u32PLen = u32KeyLen - 3 - u32InLen;
    switch(u8BT)
    {
    case 0x00:
        _memset(pu8EB, 0x00, u32PLen);
        break;
    case 0x01:
        _memset(pu8EB, 0xFF, u32PLen);
        break;
    case 0x02:
        ret = RSA_GetRandomNumber(pu8EB, u32PLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
            return ret;
        }
        break;
    default:
        HI_ERR_CIPHER("BT(0x%x) is invalid.\n", u8BT);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pu8EB += u32PLen;
    *(pu8EB++) = 0x00;
    memcpy(pu8EB, pu8Input, u32InLen);

    *pu32OutLen = u32KeyLen;

    return HI_SUCCESS;
}

/*PKCS #1: RSAES-PKCS1-V1_5-Signature*/
/*********************************************************
EM = 0x00 || 0x01 || PS || 0x00 || T

T ::= SEQUENCE {
          digestAlgorithm AlgorithmIdentifier,
          digest OCTET STRING
      }
The first field identifies the hash function and the second
contains the hash value
**********************************************************/
static HI_S32 RSA_padding_add_EMSA_PKCS1_v15(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                        HI_U32 u32HLen,
                                        HI_U32 u32KeyLen,
                                        HI_U8 *pu8Input,
                                        HI_U32 u32InLen,
                                        HI_U8 *pu8Output,
                                        HI_U32 *pu32OutLen)
{
    HI_U32 u32PadLen= 0;
    HI_U8  *p = HI_NULL;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32HLen != u32InLen)
    {
        HI_ERR_CIPHER("Error, the u32InLen must be 0x%x\n", u32HLen);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = u32KeyLen;

     p = pu8Output;
    *p++ = 0;
    *p++ = RSA_SIGN;
    switch(enHashType)
    {
    case HI_UNF_CIPHER_HASH_TYPE_SHA1:
        u32PadLen = u32KeyLen - 3 - 35;
        _memset(p, 0xFF, u32PadLen);
        p += u32PadLen;
        *p++ = 0;
        memcpy(p, ASN1_HASH_SHA1, 15);
        memcpy(p + 15, pu8Input, u32InLen);
        break;
    case HI_UNF_CIPHER_HASH_TYPE_SHA256:
        u32PadLen = u32KeyLen - 3 - 51;
        _memset(p, 0xFF, u32PadLen);
        p += u32PadLen;
        *p++ = 0;
        memcpy( p, ASN1_HASH_SHA256, 19);
        memcpy( p + 19, pu8Input, u32InLen);
        break;
    default:
        HI_ERR_CIPHER("RSA unsuporrt hash type: 0x%x.\n", enHashType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

/******************************************************************
                                  +-----------+
                                  |     M     |
                                  +-----------+
                                        |
                                        V
                                       Hash
                                        |
                                        V
                         +--------+----------+----------+
                   M' =  |Padding1|  mHash   |   salt   |
                         +--------+----------+----------+
                                        |
            +--------+----------+       V
     DB =   |Padding2|maskedseed|      Hash
            +--------+----------+       |
                    |                    |
                    V                    |      +--+
                    xor <----- MGF <----|      |bc|
                    |                    |      +--+
                    |                    |      |
                    V                    V      V
            +-------------------+----- -------+--+
    EM =    |    maskedDB       |  maskedseed |bc|
            +-------------------+-------------+--+
 ******************************************************************/
static HI_S32 RSA_padding_add_PKCS1_PSS(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                        HI_U32 u32HLen,
                                        HI_U32 u32EmBit,
                                        HI_U8 *pu8Input,
                                        HI_U32 u32InLen,
                                        HI_U8 *pu8Output,
                                        HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32SLen = 0;
    HI_U8 *pu8M = HI_NULL;
    HI_U8 u8Salt[32] = {0};
    HI_U8 *pu8MaskedDB = HI_NULL;
    HI_U8 *pu8Maskedseed = HI_NULL;
    HI_U32 u32Index = 0;
    HI_U32 u32KeyLen= 0;
    HI_U32 u32MSBits = 0;
    HI_HANDLE HashHandle = 0;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("zero point,pointer is NULL\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    u32SLen = u32HLen;
    u32KeyLen = (u32EmBit + 7)/8;
    u32MSBits = (u32EmBit - 1) & 0x07;

    *pu32OutLen = u32KeyLen;

    if (u32KeyLen < (u32HLen + u32SLen + 2))
    {
        HI_ERR_CIPHER("message too long\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    if (u32MSBits == 0)
    {
        *pu8Output++ = 0;
        u32KeyLen--;
    }

    pu8MaskedDB = pu8Output;
    pu8Maskedseed = pu8Output + u32KeyLen - u32HLen -1;

    /* Generate a random octet string salt of length sLen */
    ret = RSA_GetRandomNumber(u8Salt, u32SLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("GET_GetRandomNumber failed\n");
        return ret;
    }

    pu8M = (HI_U8*)malloc(u32SLen + u32HLen + 8);
    if (pu8M == HI_NULL)
    {
        HI_ERR_CIPHER("malloc failed\n");
        return HI_FAILURE;
    }

    /*M' = (0x)00 00 00 00 00 00 00 00 || mHash || salt*/
    u32Index = 0;
    _memset(pu8M, 0x00, 8);
    u32Index+=8;
    memcpy(&pu8M[u32Index], pu8Input, u32InLen);
    u32Index+=u32InLen;
    memcpy(&pu8M[u32Index], u8Salt, u32SLen);
    u32Index+=u32SLen;
    stHashAttr.eShaType = enHashType;
    stHashAttr.pu8HMACKey = HI_NULL;
    stHashAttr.u32HMACKeyLen = 0;
    ret = HI_MPI_CIPHER_HashInit(&stHashAttr, &HashHandle);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashInit failed\n");
        free(pu8M);
        pu8M = HI_NULL;
        return ret;
    }
    ret = HI_MPI_CIPHER_HashUpdate(HashHandle, pu8M, u32Index);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashUpdate failed\n");
        free(pu8M);
        pu8M = HI_NULL;
        return ret;
    }
    ret = HI_MPI_CIPHER_HashFinal(HashHandle, pu8Maskedseed);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashFinal failed\n");
        free(pu8M);
        pu8M = HI_NULL;
        return ret;
    }
    free(pu8M);
    pu8M = HI_NULL;

    /*maskedDB = DB xor dbMask, DB = PS || 0x01 || salt*/
    u32Index = 0;
    _memset(&pu8MaskedDB[u32Index], 0x00, u32KeyLen - u32SLen -u32HLen - 2);
    u32Index+=u32KeyLen - u32SLen -u32HLen - 2;
    pu8MaskedDB[u32Index++] = 0x01;
    memcpy(&pu8MaskedDB[u32Index], u8Salt, u32SLen);
    ret = RSA_PKCS1_MGF1(enHashType, pu8Maskedseed, u32HLen, pu8MaskedDB, u32KeyLen - u32HLen -1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return ret;
    }

    pu8Output[u32KeyLen - 1] = 0xBC;

    if (u32MSBits)
    {
        pu8Output[0] &= 0xFF >> (8 - u32MSBits);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_RsaPublicEnc(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
                                  HI_U8 *pu8Input, HI_U32 u32InLen,
                                  HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U32 u32HLen = 0;
    HI_U32 u32KeyLen = 0;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_U8  u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];

    CHECK_CIPHER_OPEN();

    if ((pstRsaEnc == HI_NULL) || (pu8Input == HI_NULL)
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    ret = RSA_GetAttr(pstRsaEnc->enScheme, pstRsaEnc->stPubKey.u16NLen,
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if (u32InLen > u32KeyLen)
    {
        HI_ERR_CIPHER("invalid inlen.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(u8EM, 0, sizeof(u8EM));

    switch(pstRsaEnc->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        /*if u32InLen < u32KeyLen, padding 0 before input data*/
        *pu32OutLen = u32KeyLen;
        memcpy(u8EM+(u32KeyLen - u32InLen), pu8Input, u32InLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        HI_ERR_CIPHER("For a public key encryption operation, the block type shall be 02.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        u8BT = (HI_U8)(pstRsaEnc->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_add_PKCS1_type(u32KeyLen, u8BT, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        ret = RSA_padding_add_PKCS1_OAEP(enHashType, u32HLen, u32KeyLen, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        ret = RSA_padding_add_PKCS1_V15(u32KeyLen, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    default:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }

    if(*pu32OutLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32OutLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaEnc->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaEnc->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPubKey.u16ELen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8Output;
    stRsaData.u32DataLen = u32KeyLen;
#ifdef CIPHER_KLAD_SUPPORT
    stRsaData.enKeySrc = HI_UNF_CIPHER_KEY_SRC_USER;
#endif
    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);

    return ret;
}

static HI_S32 RSA_padding_check_PKCS1_OAEP(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                  HI_U32 u32HLen,
                                  HI_U32 u32KeyLen,
                                  HI_U8 *pu8Input,
                                  HI_U32 u32InLen,
                                  HI_U8 *pu8Output,
                                  HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 i = 0;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;
    HI_U8 *pu8Seed = HI_NULL;
    HI_U8 *pu8DB = HI_NULL;
    HI_U8 *pu8MaskedDB = HI_NULL;

    CHECK_CIPHER_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA256)
    {
        pu8LHASH = EMPTY_L_SHA256;
    }

    if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (u32KeyLen < 2 * u32HLen + 2)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8Input[0] != 0x00)
    {
        HI_ERR_CIPHER("EM[0] != 0.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    *pu32OutLen = 0;
    pu8MaskedDB= pu8Input + u32HLen + 1;
    pu8Seed = pu8Input + 1;
    pu8DB = pu8Input + u32HLen + 1;
    ret = RSA_PKCS1_MGF1(enHashType, pu8MaskedDB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return ret;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return ret;
    }
    if(memcmp(pu8DB, pu8LHASH, u32HLen) != 0)
    {
        HI_ERR_CIPHER("LHASH error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    for(i=u32HLen; i < u32KeyLen - u32HLen - 1; i++)
    {
        if((pu8DB[i] == 0x01))
        {
            memcpy(pu8Output, pu8DB+i+1, u32KeyLen - u32HLen - i - 2);
            *pu32OutLen = u32KeyLen - u32HLen - i - 2;
            break;
        }
    }
    if (i >= u32KeyLen - u32HLen - 1)
    {
        HI_ERR_CIPHER("PS error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    return HI_SUCCESS;
}

static HI_S32 RSA_padding_check_PKCS1_V15(HI_U32 u32KeyLen,
                                          HI_U8 *pu8Input,
                                          HI_U32 u32InLen,
                                          HI_U8 *pu8Output,
                                          HI_U32 *pu32OutLen)
{
    HI_U32 u32Index = 0;

    CHECK_CIPHER_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL)|| (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((u32InLen != u32KeyLen) || (u32KeyLen < 11))
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8Input[u32Index] != 0x00)
    {
        HI_ERR_CIPHER("EM[0] != 0x00.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    u32Index++;
    if(pu8Input[u32Index] != 0x02)
    {
        HI_ERR_CIPHER("EM[1] != 0x02.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    u32Index++;
    for( ; u32Index < u32KeyLen; u32Index++)
    {
        if( (u32Index >= 10) //The length of PS is large than 8 octets
            && (pu8Input[u32Index] == 0x00))
        {
            memcpy(pu8Output, &pu8Input[u32Index+1], u32KeyLen - 1 - u32Index);
            *pu32OutLen = u32KeyLen - 1 - u32Index;
            break;
        }
    }
    if (u32Index >= u32KeyLen)
    {
        HI_ERR_CIPHER("PS error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    return HI_SUCCESS;
}

static HI_S32 RSA_padding_check_PKCS1_type(HI_U32 u32KeyLen,
                               HI_U8  u8BT,
                               HI_U8 *pu8Input,
                               HI_U32 u32InLen,
                               HI_U8 *pu8Output,
                               HI_U32 *pu32OutLen)
{
    HI_U8 *pu8EB = HI_NULL;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL)|| (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0x00;
    pu8EB = pu8Input;
    if(*pu8EB != 0x00)
    {
        HI_ERR_CIPHER("EB[0] != 0x00.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    pu8EB++;
    if(*pu8EB != u8BT)
    {
        HI_ERR_CIPHER("EB[1] != BT(0x%x).\n", u8BT);
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    pu8EB++;
    switch(u8BT)
    {
    case 0x00:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if( (*pu8EB == 0x00) && (*(pu8EB+1) != 0))
            {
                break;
            }
        }
        break;
    case 0x01:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if(*pu8EB == 0xFF)
            {
                continue;
            }
            else if (*pu8EB == 0x00)
            {
                break;
            }
            else
            {
                pu8EB = pu8Input + u32InLen - 1;
                break;
            }
        }
        break;
    case 0x02:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if(*pu8EB == 0x00)
            {
                break;
            }
        }
        break;
    default:
        HI_ERR_CIPHER("BT(0x%x) is invalid.\n", u8BT);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8EB >= (pu8Input + u32InLen - 1))
    {
        HI_ERR_CIPHER("PS Error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    pu8EB++;
    *pu32OutLen = pu8Input + u32KeyLen - pu8EB;
    memcpy(pu8Output, pu8EB, *pu32OutLen);

    return HI_SUCCESS;
}

static HI_S32 RSA_padding_check_EMSA_PKCS1_v15(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                        HI_U32 u32HLen,
                                        HI_U32 u32KeyLen,
                                        HI_U8 *pu8Input,
                                        HI_U32 u32InLen,
                                        HI_U8 *pu8Output,
                                        HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32Len = 0;
    HI_U8  *p = HI_NULL;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32KeyLen != u32InLen)
    {
        HI_ERR_CIPHER("Error, the u32InLen must be 0x%x\n", u32KeyLen);
        return ret;
    }

    *pu32OutLen = u32HLen;

    CHECK_CIPHER_OPEN();

    /*EM = 01 || PS || 00 || T*/
    p = pu8Input;

    if( *p++ != 0 || *p++ != RSA_SIGN )
    {
        HI_ERR_CIPHER("RSA EM head error\n");
        return HI_FAILURE;
    }

    while( *p != 0 )
    {
        if( p >= pu8Input + u32KeyLen - 1 || *p != 0xFF )
        {
            HI_ERR_CIPHER("RSA PS error\n");
            return HI_FAILURE;
        }
        p++;
    }
    p++; //skip 0x00

    u32Len = u32KeyLen - (HI_U32)( p - pu8Input);
    ret = HI_FAILURE;
    switch(enHashType)
    {
    case HI_UNF_CIPHER_HASH_TYPE_SHA1:
        if (u32Len != 35)
        {
            HI_ERR_CIPHER("RSA T len error: %d\n", u32Len);
            break;
        }
        if(memcmp(p, ASN1_HASH_SHA1, 15) == 0)
        {
            memcpy(pu8Output, p + 15, u32HLen);
            ret = HI_SUCCESS;
        }
        break;
    case HI_UNF_CIPHER_HASH_TYPE_SHA256:
        if (u32Len != 51)
        {
            HI_ERR_CIPHER("RSA T len error: %d\n", u32Len);
            break;
        }
        if(memcmp(p, ASN1_HASH_SHA256, 19) == 0)
        {
            memcpy(pu8Output, p + 19, u32HLen);
            ret = HI_SUCCESS;
        }
        break;
    default:
            HI_ERR_CIPHER("RSA unsuporrt hash type: 0x%x.\n", enHashType);
    }

    return ret;
}

static HI_S32 RSA_padding_check_PKCS1_PSS(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                          HI_U32 u32HLen,
                                          HI_U32 u32EmBit,
                                          HI_U8 *pu8Input,
                                          HI_U32 u32InLen,
                                          HI_U8 *pu8MHash)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32SLen = 0;
    HI_U8 *pu8M = HI_NULL;
    HI_U8 u8Salt[32] = {0};
    HI_U8 u8H[32] = {0};
    HI_U8 *pu8MaskedDB = HI_NULL;
    HI_U8 *pu8Maskedseed = HI_NULL;
    HI_U32 u32Index = 0;
    HI_U32 u32KeyLen = 0;
    HI_U32 u32MSBits = 0;
    HI_HANDLE HashHandle = 0;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

    if ((pu8Input == NULL) || (pu8MHash == NULL))
    {
        HI_ERR_CIPHER("zero point\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    u32SLen = u32HLen;
    u32KeyLen = (u32EmBit + 7)/8;
    u32MSBits = (u32EmBit - 1) & 0x07;

    if (u32KeyLen < (u32HLen + u32SLen + 2))
    {
        HI_ERR_CIPHER("message too long\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("inconsistent, u32InLen != 0xBC\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (pu8Input[0] & (0xFF << u32MSBits))
    {
        HI_ERR_CIPHER("inconsistent, EM[0] invalid\n");
        return HI_FAILURE;
    }

    _memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    if (u32MSBits == 0)
    {
        pu8Input++;
        u32KeyLen--;
    }

    pu8MaskedDB = pu8Input;
    pu8Maskedseed = pu8Input + u32KeyLen - u32HLen -1;

    if (pu8Input[u32KeyLen - 1] != 0xBC)
    {
        HI_ERR_CIPHER("inconsistent, EM[u32KeyLen - 1] != 0xBC\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*maskedDB = DB xor dbMask, DB = PS || 0x01 || salt*/
    ret = RSA_PKCS1_MGF1(enHashType, pu8Maskedseed, u32HLen, pu8MaskedDB, u32KeyLen - u32HLen -1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return ret;
    }
    if (u32MSBits)
    {
        pu8MaskedDB[0] &= 0xFF >> (8 - u32MSBits);
    }
    for (u32Index=0; u32Index<u32KeyLen - u32SLen -u32HLen - 2; u32Index++)
    {
        if (pu8MaskedDB[u32Index] != 0x00)
        {
            HI_ERR_CIPHER("inconsistent, PS != 0x00 in DB\n");
            return HI_FAILURE;
        }
    }
    if (pu8MaskedDB[u32Index++] != 0x01)
    {
        HI_ERR_CIPHER("inconsistent, can't find 0x01 in DB\n");
        return HI_FAILURE;
    }
    memcpy(u8Salt, &pu8MaskedDB[u32Index], u32SLen);

    pu8M = (HI_U8*)malloc(u32SLen + u32HLen + 8);
    if (pu8M == NULL)
    {
        HI_ERR_CIPHER("malloc failed\n");
        return HI_FAILURE;
    }

    /*M' = (0x)00 00 00 00 00 00 00 00 || mHash || salt*/
    u32Index = 0;
    _memset(pu8M, 0x00, 8);
    u32Index+=8;
    memcpy(&pu8M[u32Index], pu8MHash, u32HLen);
    u32Index+=u32HLen;
    memcpy(&pu8M[u32Index], u8Salt, u32SLen);
    u32Index+=u32SLen;
    stHashAttr.eShaType = enHashType;
    ret = HI_MPI_CIPHER_HashInit(&stHashAttr, &HashHandle);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashInit failed\n");
        free(pu8M);
        pu8M = HI_NULL;
        return ret;
    }
    ret = HI_MPI_CIPHER_HashUpdate(HashHandle, pu8M, u32Index);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashUpdate failed\n");
        free(pu8M);
        pu8M = HI_NULL;
        return ret;
    }
    ret = HI_MPI_CIPHER_HashFinal(HashHandle, u8H);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashFinal failed\n");
        free(pu8M);
        pu8M = HI_NULL;
        return ret;
    }
    free(pu8M);
    pu8M = HI_NULL;

    ret = memcmp(u8H, pu8Maskedseed, u32HLen);
    if (ret != 0)
    {
        HI_ERR_CIPHER("consistent, hash compare failed\n");
        ret = HI_FAILURE;
    }

    return ret;
}

HI_S32 HI_MPI_CIPHER_RsaPrivateDec(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
                                   HI_U8 *pu8Input, HI_U32 u32InLen,
                                   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U8 u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;

    CHECK_CIPHER_OPEN();
    if ((pstRsaDec == HI_NULL) || (pu8Input == HI_NULL)
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ret = RSA_GetAttr(pstRsaDec->enScheme, pstRsaDec->stPriKey.u16NLen,
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaDec->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaDec->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPriKey.u16DLen;
    stRsaData.pu8Input = pu8Input;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;
#ifdef CIPHER_KLAD_SUPPORT
    stRsaData.enKeySrc = pstRsaDec->enKeySrc;
#endif
    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("CMD_CIPHER_CALCRSA failed, ret = %x\n", ret);
        return HI_FAILURE;
    }

    switch(pstRsaDec->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        *pu32OutLen = u32InLen;
        memcpy(pu8Output, u8EM, u32KeyLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaDec->enScheme);
        HI_ERR_CIPHER("For a private key decryption operation, the block type shall be 02.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        u8BT = (HI_U8)(pstRsaDec->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_check_PKCS1_type(u32KeyLen, u8BT, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        ret = RSA_padding_check_PKCS1_OAEP(enHashType, u32HLen, u32KeyLen, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        ret = RSA_padding_check_PKCS1_V15(u32KeyLen, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    default:
        HI_ERR_CIPHER("RSA scheme error, scheme = 0x%x.\n", pstRsaDec->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }

    return ret;
}

HI_S32 HI_MPI_CIPHER_RsaSign(HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign,
                             HI_U8 *pu8InData, HI_U32 u32InDataLen,
                             HI_U8 *pu8HashData,
                             HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen)
{
    HI_U32 u32KeyLen = 0;
    HI_U32 u32HLen = 0;
    HI_U32 u32EmBit = 0;
    HI_U8  u8Hash[32] = {0};
    HI_U8  u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    HI_S32 ret = HI_FAILURE;
    CIPHER_RSA_DATA_S stRsaData;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle = 0;
    HI_U8 *pHash = HI_NULL;

    if (pstRsaSign == HI_NULL)
    {
        HI_ERR_CIPHER("pstRsaSign is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((pstRsaSign == HI_NULL)|| (pu8OutSign == HI_NULL)||(pu32OutSignLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((pu8InData == HI_NULL) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((pstRsaSign->stPriKey.pu8N == HI_NULL) || (pstRsaSign->stPriKey.pu8D == HI_NULL))
    {
        HI_ERR_CIPHER("key is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((u32InDataLen == 0) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    _memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    ret = RSA_GetAttr(pstRsaSign->enScheme, pstRsaSign->stPriKey.u16NLen,
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    /*hash is NULl, need to calc by self*/
    if (pu8HashData == HI_NULL)
    {
        stHashAttr.eShaType = enHashType;
        ret = HI_MPI_CIPHER_HashInit(&stHashAttr, &HashHandle);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashInit error\n");
            return ret;
        }
        ret = HI_MPI_CIPHER_HashUpdate(HashHandle, pu8InData, u32InDataLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        ret = HI_MPI_CIPHER_HashFinal(HashHandle, u8Hash);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        pHash = u8Hash;
    }
    else
    {
        pHash = pu8HashData;
    }

    _memset(u8EM, 0, sizeof(u8EM));
    switch(pstRsaSign->enScheme)
    {
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        ret = RSA_padding_add_EMSA_PKCS1_v15(enHashType, u32HLen, u32KeyLen,
            pHash, u32HLen, u8EM, pu32OutSignLen);
        break;
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        u32EmBit = HI_MPI_CIPHER_GetBitNum(pstRsaSign->stPriKey.pu8N, u32KeyLen);
        ret = RSA_padding_add_PKCS1_PSS(enHashType, u32HLen, u32EmBit,
            pHash, u32HLen, u8EM, pu32OutSignLen);
        break;
    default:
        HI_ERR_CIPHER("invalid scheme; 0x%x\n", pstRsaSign->enScheme);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA sign padding error\n");
        return ret;
    }

    if(*pu32OutSignLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InSigntLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaSign->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaSign->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaSign->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaSign->stPriKey.u16DLen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8OutSign;
    stRsaData.u32DataLen = u32KeyLen;
#ifdef CIPHER_KLAD_SUPPORT
    stRsaData.enKeySrc = pstRsaSign->enKeySrc;
#endif

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);

    return ret;
}

HI_S32 HI_MPI_CIPHER_RsaPrivateEnc(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
                                    HI_U8 *pu8Input, HI_U32 u32InLen,
                                   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U32 u32HLen = 0;
    HI_U32 u32KeyLen = 0;
    HI_U8  u8BT = 0;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_U8  u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];

    CHECK_CIPHER_OPEN();

    if ((pstRsaEnc == HI_NULL) || (pu8Input == HI_NULL)
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    ret = RSA_GetAttr(pstRsaEnc->enScheme, pstRsaEnc->stPriKey.u16NLen,
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if (u32InLen > u32KeyLen)
    {
        HI_ERR_CIPHER("invalid inlen.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    _memset(u8EM, 0, sizeof(u8EM));

    switch(pstRsaEnc->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        /*if u32InLen < u32KeyLen, padding 0 before input data*/
        *pu32OutLen = u32KeyLen;
        memcpy(u8EM+(u32KeyLen - u32InLen), pu8Input, u32InLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        u8BT = (HI_U8)(pstRsaEnc->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_add_PKCS1_type(u32KeyLen, u8BT, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        HI_ERR_CIPHER("For a private- key encryption operation, the block type shall be 00 or 01.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    default:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }

    if(*pu32OutLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32OutLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaEnc->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaEnc->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPriKey.u16DLen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8Output;
    stRsaData.u32DataLen = u32KeyLen;
#ifdef CIPHER_KLAD_SUPPORT
    stRsaData.enKeySrc = pstRsaEnc->enKeySrc;
#endif
    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);

    return ret;

}

HI_S32 HI_MPI_CIPHER_RsaVerify(HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
                               HI_U8 *pu8InData, HI_U32 u32InDataLen,
                               HI_U8 *pu8HashData,
                               HI_U8 *pu8InSign, HI_U32 u32InSignLen)
{
    HI_U32 ret = HI_FAILURE;
    HI_U32 u32KeyLen = 0;
    HI_U8  u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN] = {0};
    CIPHER_RSA_DATA_S stRsaData;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle = 0;
    HI_U8  u8Hash[32] = {0};
    HI_U8  u8SignHash[32] = {0};
    HI_U32 u32HLen = 0;
    HI_U32 u32EmBit = 0;
    HI_U8  *pHash = HI_NULL;

    if (pstRsaVerify == HI_NULL)
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    if ((pstRsaVerify->stPubKey.pu8N == HI_NULL) || (pstRsaVerify->stPubKey.pu8E == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ret = RSA_GetAttr(pstRsaVerify->enScheme, pstRsaVerify->stPubKey.u16NLen,
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(u32InSignLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InSigntLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_OPEN();

    _memset(&stHashAttr, 0, sizeof(stHashAttr));
    _memset(&stRsaData, 0, sizeof(stRsaData));

    stRsaData.pu8N = pstRsaVerify->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaVerify->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaVerify->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaVerify->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8InSign;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;
#ifdef CIPHER_KLAD_SUPPORT
    stRsaData.enKeySrc = HI_UNF_CIPHER_KEY_SRC_USER;
#endif
    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA verify dec error, ret=%d\n", ret);
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    if (pu8HashData == HI_NULL)
    {
        stHashAttr.eShaType = enHashType;
        ret = HI_MPI_CIPHER_HashInit(&stHashAttr, &HashHandle);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashInit error\n");
            return ret;
        }
        ret = HI_MPI_CIPHER_HashUpdate(HashHandle, pu8InData, u32InDataLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        ret = HI_MPI_CIPHER_HashFinal(HashHandle, u8Hash);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        pHash = u8Hash;
    }
    else
    {
        pHash = pu8HashData;
    }

    switch(pstRsaVerify->enScheme)
    {
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        ret = RSA_padding_check_EMSA_PKCS1_v15(enHashType, u32HLen, u32KeyLen, u8EM,
              u32InSignLen, u8SignHash, &u32HLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("RSA_padding_add_RSASSA_PKCS1_v15 error\n");
            return ret;
        }
        if (memcmp(pHash, u8SignHash, u32HLen) != 0)
        {
            HI_ERR_CIPHER("RSA verify, hash error\n");
            ret = HI_ERR_CIPHER_FAILED_DECRYPT;
        }
        break;
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        u32EmBit = HI_MPI_CIPHER_GetBitNum(pstRsaVerify->stPubKey.pu8N, u32KeyLen);
        ret = RSA_padding_check_PKCS1_PSS(enHashType, u32HLen, u32EmBit,
               u8EM, u32InSignLen, pHash);
        break;
    default:
        HI_ERR_CIPHER("invalid scheme; 0x%x\n", pstRsaVerify->enScheme);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return ret;
}

HI_S32 HI_MPI_CIPHER_RsaPublicDec(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
                               HI_U8 *pu8Input, HI_U32 u32InLen,
                               HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U8 u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN] = {0};
    HI_U32 u32HLen = 0;
    HI_U32 u32KeyLen = 0;
    HI_U8  u8BT = 0;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;

    CHECK_CIPHER_OPEN();

    if ((pstRsaDec == HI_NULL) || (pu8Input == HI_NULL)
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ret = RSA_GetAttr(pstRsaDec->enScheme, pstRsaDec->stPubKey.u16NLen,
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    _memset(&stRsaData, 0, sizeof(stRsaData));

    stRsaData.pu8N = pstRsaDec->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaDec->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8Input;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;
#ifdef CIPHER_KLAD_SUPPORT
    stRsaData.enKeySrc = HI_UNF_CIPHER_KEY_SRC_USER;
#endif
    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("CMD_CIPHER_CALCRSA failed, ret = %x\n", ret);
        return HI_FAILURE;
    }

    switch(pstRsaDec->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        *pu32OutLen = u32InLen;
        memcpy(pu8Output, u8EM, u32KeyLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        u8BT = (HI_U8)(pstRsaDec->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_check_PKCS1_type(u32KeyLen, u8BT, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaDec->enScheme);
        HI_ERR_CIPHER("For a public key decryption operation, the block type shall be 00 or 01.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    default:
        HI_ERR_CIPHER("RSA scheme error, scheme = 0x%x.\n", pstRsaDec->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }

    return ret;
}

#endif
