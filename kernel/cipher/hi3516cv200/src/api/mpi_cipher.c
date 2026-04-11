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
#include "drv_cipher_log.h"
#include "drv_cipher_reg.h"
#include "hi_mmz_api.h"

HI_S32 g_CipherDevFd = -1;
static HI_S32 g_CipherInitCounter = -1;
static pthread_mutex_t   g_CipherMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_CIPHER_LOCK()  	     (void)pthread_mutex_lock(&g_CipherMutex);
#define HI_CIPHER_UNLOCK()  	 (void)pthread_mutex_unlock(&g_CipherMutex);

#define CHECK_CIPHER_OPEN()\
do{\
    HI_CIPHER_LOCK();\
    if (g_CipherInitCounter < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        HI_CIPHER_UNLOCK();\
        return HI_ERR_CIPHER_NOT_INIT;\
    }\
    HI_CIPHER_UNLOCK();\
}while(0)

#define RSA_SIGN        1
#define ASN1_HASH_SHA1    "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14"
#define ASN1_HASH_SHA256  "\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20"
static const HI_S8 EMPTY_L_SHA1[]   = "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09";
static const HI_S8 EMPTY_L_SHA256[] = "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55";

#ifdef CFG_HI_CIPHER_HASH_SUPPORT

static pthread_mutex_t   g_HashMutex = PTHREAD_MUTEX_INITIALIZER;
#define HI_HASH_LOCK()  	 (void)pthread_mutex_lock(&g_HashMutex);
#define HI_HASH_UNLOCK()  	 (void)pthread_mutex_unlock(&g_HashMutex);

#define HASH_BLOCK_SIZE		        (64)
#define HASH_PAD_MAX_LEN	        (64)
#define HASH1_SIGNATURE_SIZE		(20)
#define HASH256_SIGNATURE_SIZE		(32)
#define HASH_MMZ_BUF_LEN	        (1*1024*1024)		//1M
#define HASH_MMZ_TAIL_LEN	        (8*1024)			//8K
#define HASH_CHANNAL_MAX_NUM        (8)
#define RSA_MAX_KEY_LEN             (512)
#define RSA_MIN_KEY_LEN             (128)
#define CIPHER_MAX_LIST_NUM         (128)

typedef struct hiCI_MMZ_BUF_S
{
    HI_U32 u32StartPhyAddr;
    HI_U8* pu8StartVirAddr;
    HI_U32 u32Size;
}MMZ_BUFFER_S;

typedef struct hiHASH_INFO_S
{
    HI_BOOL   bIsUsed;
    HI_HANDLE hHandle;
    HI_UNF_CIPHER_HASH_TYPE_E enShaType;
    HI_U32 u32TotalDataLen;
    HI_U32 u32ShaVal[8];
    HI_U32 u32ShaLen;
    HI_U8  u8LastBlock[HASH_BLOCK_SIZE];
    HI_U8  u8Mac[HASH_BLOCK_SIZE];
    HI_U32 u8LastBlockSize;
    MMZ_BUFFER_S stMMZBuffer;
}HASH_INFO_S;

static HASH_INFO_S g_stCipherHashData[HASH_CHANNAL_MAX_NUM];
static MMZ_BUFFER_S s_stMMZBuffer;
HI_U32 g_u32HashBaseBufferLen = HASH_MMZ_BUF_LEN;

static HI_VOID Cipher_HashMmzDeInit(HI_VOID);

#define Hash_CheckHandle(CIHandle)   \
do \
{ \
    if((HI_ID_HASH != HI_HANDLE_GET_MODID(CIHandle)) \
        || (0 != HI_HANDLE_GET_PriDATA(CIHandle))) \
    { \
        HI_ERR_CIPHER("Invalid ChanId!\n"); \
        return HI_ERR_CIPHER_INVALID_PARA; \
    } \
    if (HI_HANDLE_GET_CHNID(CIHandle) >= HASH_CHANNAL_MAX_NUM) \
    { \
        HI_ERR_CIPHER("chan %d is too large, max: %d\n", HI_HANDLE_GET_CHNID(CIHandle), HASH_CHANNAL_MAX_NUM); \
        return HI_ERR_CIPHER_INVALID_PARA; \
    } \
    if (HI_FALSE == g_stCipherHashData[HI_HANDLE_GET_CHNID(CIHandle)].bIsUsed) \
    { \
        HI_ERR_CIPHER("chan %d is not open\n", HI_HANDLE_GET_CHNID(CIHandle)); \
        return HI_ERR_CIPHER_INVALID_HANDLE; \
    } \
} while (0)

static HI_S32 Cipher_HashMmzInit(HI_VOID)
{
	HI_U32 u32Testcached = 0;
	HI_U32 u32HashBufLen = 0;

    if( s_stMMZBuffer.u32StartPhyAddr == 0)
    {
    	u32HashBufLen = g_u32HashBaseBufferLen + HASH_MMZ_TAIL_LEN;
        s_stMMZBuffer.u32StartPhyAddr = (HI_U32)HI_MMZ_New(u32HashBufLen, 0, NULL, "CIPHER_HashBuffer");
        if (0 == s_stMMZBuffer.u32StartPhyAddr)
        {
        	g_u32HashBaseBufferLen = g_u32HashBaseBufferLen / 2;
        	u32HashBufLen = g_u32HashBaseBufferLen + HASH_MMZ_TAIL_LEN;
    	    s_stMMZBuffer.u32StartPhyAddr = (HI_U32)HI_MMZ_New(u32HashBufLen, 0, NULL, "CIPHER_HashBuffer");
    	    if (0 == s_stMMZBuffer.u32StartPhyAddr)
    	    {
    	    	g_u32HashBaseBufferLen = g_u32HashBaseBufferLen / 2;
    	        u32HashBufLen = g_u32HashBaseBufferLen + HASH_MMZ_TAIL_LEN;
    		    s_stMMZBuffer.u32StartPhyAddr = (HI_U32)HI_MMZ_New(u32HashBufLen, 0, NULL, "CIPHER_HashBuffer");
    		    if (0 == s_stMMZBuffer.u32StartPhyAddr)
    		    {
    		        HI_ERR_CIPHER("Error: Get mmz buffer for hash failed!\n");
    		        return HI_FAILURE;
    		    }
    	    }
        }

        s_stMMZBuffer.pu8StartVirAddr = (HI_U8*)HI_MMZ_Map(s_stMMZBuffer.u32StartPhyAddr, u32Testcached);
    	if( 0 == (HI_VOID *)s_stMMZBuffer.pu8StartVirAddr )
    	{
            HI_ERR_CIPHER("Error: Map mmz buffer for hash failed!\n");
    		HI_MMZ_Delete(s_stMMZBuffer.u32StartPhyAddr);
    		memset(&s_stMMZBuffer, 0, sizeof(s_stMMZBuffer));
            return HI_FAILURE;
    	}
    }
	return HI_SUCCESS;
}

static HI_VOID Cipher_HashMmzDeInit(HI_VOID)
{
    if (s_stMMZBuffer.u32StartPhyAddr == 0)
    {
        return;
    }

    (HI_VOID)HI_MMZ_Unmap(s_stMMZBuffer.u32StartPhyAddr);
    (HI_VOID)HI_MMZ_Delete(s_stMMZBuffer.u32StartPhyAddr);
    memset(&s_stMMZBuffer, 0, sizeof(s_stMMZBuffer));
    s_stMMZBuffer.u32StartPhyAddr = 0;
}

#endif

HI_S32 HI_MPI_CIPHER_Init(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
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

#ifdef CFG_HI_CIPHER_HASH_SUPPORT
    memset(g_stCipherHashData, 0, sizeof(g_stCipherHashData));
    memset(&s_stMMZBuffer, 0, sizeof(s_stMMZBuffer));
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

    if(g_CipherInitCounter > 0)
    {
        g_CipherInitCounter--;
    }
	if(g_CipherInitCounter != 0)
	{
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_CipherDevFd);

    if(HI_SUCCESS != Ret)
    {
        HI_FATAL_CIPHER("Close cipher err.\n");
		g_CipherInitCounter++;
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_NOT_INIT;
    }
    g_CipherDevFd = -1;

#ifdef CFG_HI_CIPHER_HASH_SUPPORT
    Cipher_HashMmzDeInit();
    memset(&s_stMMZBuffer, 0, sizeof(s_stMMZBuffer));
#endif
	g_CipherInitCounter = -1;

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}

#ifdef CFG_HI_CIPHER_MULTICIPHER_SUPPORT
HI_S32 HI_MPI_CIPHER_CreateHandle(HI_HANDLE* phCipher)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_HANDLE_S stCIHandle = {0};

    if (  NULL == phCipher )
    {
        HI_ERR_CIPHER("Invalid param, null pointer!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_OPEN();

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

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memcpy(&configdata.CIpstCtrl, pstCtrl, sizeof(HI_UNF_CIPHER_CTRL_S));
    configdata.CIHandle=hCipher;

    if(configdata.CIpstCtrl.enWorkMode >= HI_UNF_CIPHER_WORK_MODE_BUTT)
    {
        HI_ERR_CIPHER("para set CIPHER wokemode is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

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

    if ((u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
        || (u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        || (u32SrcPhyAddr + u32ByteLength < u32ByteLength)
        || (u32DestPhyAddr + u32ByteLength < u32ByteLength))
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.u32DataLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

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

    if ((u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
        || (u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        || (u32SrcPhyAddr + u32ByteLength < u32ByteLength)
        || (u32DestPhyAddr + u32ByteLength < u32ByteLength))
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.u32DataLength = u32ByteLength;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

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
    HI_U32 i;
    CIPHER_DATA_S CIdata;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = NULL;

    if ((NULL == pstDataPkg) || (0 == u32DataPkgNum) || (CIPHER_MAX_LIST_NUM < u32DataPkgNum))
    {
        HI_ERR_CIPHER("Invalid data pkg.\n");
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
        if ((pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
            || (pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
            || (pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    CIdata.ScrPhyAddr=(HI_U32)pstDataPkg;
    CIdata.DestPhyAddr= 0;
    CIdata.u32DataLength=u32DataPkgNum;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

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
    HI_U32 i;
    CIPHER_DATA_S CIdata;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = NULL;

    if ((NULL == pstDataPkg) || (0 == u32DataPkgNum) || (CIPHER_MAX_LIST_NUM < u32DataPkgNum))
    {
        HI_ERR_CIPHER("Invalid data pkg.\n");
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
        if ((pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
            || (pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
            || (pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    CIdata.ScrPhyAddr=(HI_U32)pstDataPkg;
    CIdata.DestPhyAddr= 0;
    CIdata.u32DataLength=u32DataPkgNum;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

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
    HI_U32 i;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = NULL;
    CIPHER_MUTIL_EX_DATA_S  stMutliEx;

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("Invalid point.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((NULL == pstDataPkg) || (0 == u32DataPkgNum) || (CIPHER_MAX_LIST_NUM < u32DataPkgNum))
    {
        HI_ERR_CIPHER("Invalid data pkg.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if ((pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
            || (pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
            || (pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

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
    HI_U32 i;
    HI_UNF_CIPHER_DATA_S *pPkgTmp = NULL;
    CIPHER_MUTIL_EX_DATA_S  stMutliEx;

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("Invalid point.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((NULL == pstDataPkg) || (0 == u32DataPkgNum) || (CIPHER_MAX_LIST_NUM < u32DataPkgNum))
    {
        HI_ERR_CIPHER("Invalid data pkg.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if ((pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
            || (pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
            || (pPkgTmp->u32SrcPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength)
            || (pPkgTmp->u32DestPhyAddr + pPkgTmp->u32ByteLength < pPkgTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

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

#ifdef CFG_HI_CIPHER_CCM_GCM_SUPPORT
HI_S32 HI_MPI_CIPHER_GetTag(HI_HANDLE hCipherHandle,  HI_U8 *pstTag)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_TAG stTag;

    if (pstTag == HI_NULL)
    {
        HI_ERR_CIPHER("invalid para.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

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

#ifdef CFG_HI_CIPHER_GETHANDLECONFIG_SUPPORT
HI_S32 HI_MPI_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_Config_CTRL  configdata;

    if (pstCtrl == HI_NULL)
    {
        HI_ERR_CIPHER("invalid para.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_OPEN();

    configdata.CIHandle = hCipherHandle;
    memset(&configdata.CIpstCtrl, 0, sizeof(configdata.CIpstCtrl));

    Ret = ioctl(g_CipherDevFd, CMD_CIPHER_GETHANDLECONFIG, &configdata);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    memcpy(pstCtrl, &configdata.CIpstCtrl, sizeof(configdata.CIpstCtrl));

    return HI_SUCCESS;
}
#endif
#endif

#ifdef CFG_HI_CIPHER_HASH_SUPPORT
static HI_U32 HashMsgPadding(HI_U8 *pu8Msg, HI_U32 u32ByteLen, HI_U32 u32TotalLen)
{
    HI_U32 u32Tmp = 0;
    HI_U32 u32PaddingLen;

    if( NULL == pu8Msg )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        return -1;
    }

    u32Tmp = u32TotalLen % 64;
    /* 56 = 64 - 8, 120 = 56 + 64 */
    u32PaddingLen = (u32Tmp < 56) ? (56 - u32Tmp) : (120 - u32Tmp);
    /* add 8 bytes fix data length */
    u32PaddingLen += 8;

    /* Format(binary): {data|1000...00| fix_data_len(bits)} */
    pu8Msg[u32ByteLen++] = 0x80;
    memset(&pu8Msg[u32ByteLen], 0, u32PaddingLen - 1 - 8);
    u32ByteLen+=u32PaddingLen - 1 - 8;

    /* write 8 bytes fix data length */
    pu8Msg[u32ByteLen++] = 0x00;
    pu8Msg[u32ByteLen++] = 0x00;
    pu8Msg[u32ByteLen++] = 0x00;
    pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 29)&0x07);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 21)&0xff);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 13)&0xff);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 5)&0xff);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen << 3)&0xff);

    return u32ByteLen;
}

static HI_S32 CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 u32SoftId = 0;
    HASH_INFO_S *pstHashInfo = NULL;

    CHECK_CIPHER_OPEN();

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_HASH_LOCK();

    for(u32SoftId=0; u32SoftId<HASH_CHANNAL_MAX_NUM; u32SoftId++)
    {
        if (!g_stCipherHashData[u32SoftId].bIsUsed)
        {
            break;
        }
    }

    if(u32SoftId >= HASH_CHANNAL_MAX_NUM)
    {
        HI_ERR_CIPHER("Hash module is busy!\n");
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    pstHashInfo = &g_stCipherHashData[u32SoftId];
	memset(pstHashInfo, 0, sizeof(HASH_INFO_S));
    memset(&stHashData, 0, sizeof(CIPHER_HASH_DATA_S));
	pstHashInfo->stMMZBuffer = s_stMMZBuffer;
    pstHashInfo->enShaType = pstHashAttr->eShaType;

	if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType)
	  || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType ))
	{
	    stHashData.enShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
        pstHashInfo->u32ShaLen = HASH1_SIGNATURE_SIZE;
        pstHashInfo->u32ShaVal[0] = 0x01234567;
        pstHashInfo->u32ShaVal[1] = 0x89abcdef;
        pstHashInfo->u32ShaVal[2] = 0xfedcba98;
        pstHashInfo->u32ShaVal[3] = 0x76543210;
        pstHashInfo->u32ShaVal[4] = 0xf0e1d2c3;
	}
	else if ( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType)
		 || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
	{
	    stHashData.enShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
        pstHashInfo->u32ShaLen = HASH256_SIGNATURE_SIZE;
        pstHashInfo->u32ShaVal[0] = 0x67e6096a;
        pstHashInfo->u32ShaVal[1] = 0x85ae67bb;
        pstHashInfo->u32ShaVal[2] = 0x72f36e3c;
        pstHashInfo->u32ShaVal[3] = 0x3af54fa5;
        pstHashInfo->u32ShaVal[4] = 0x7f520e51;
        pstHashInfo->u32ShaVal[5] = 0x8c68059b;
        pstHashInfo->u32ShaVal[6] = 0xabd9831f;
        pstHashInfo->u32ShaVal[7] = 0x19cde05b;
	}
	else
	{
	    ;/* Invalid hash/hmac type, no need to be processed, it have been processed before */
	}
    stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASHINIT, &stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash initial failed!\n");
        HI_HASH_UNLOCK();
        return ret;
    }

    pstHashInfo->bIsUsed = HI_TRUE;
    *pHashHandle = HI_HANDLE_MAKEHANDLE(HI_ID_HASH, 0, u32SoftId);

     HI_HASH_UNLOCK();

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HashUpdate(HI_HANDLE CIHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_S32 ret = HI_FAILURE;
	HI_U32 u32Tmp = 0;
    HI_U32 u32TotalSize = 0;
    HI_U32 u32CopySize = 0;
    HI_U32 u32BufInUsedLen = 0;
    CIPHER_HASH_DATA_S stHashData;
	HI_U32 i = 0;
    HI_HANDLE hHashHandle = 0x0;

    Hash_CheckHandle(CIHashHandle);
    hHashHandle = HI_HANDLE_GET_CHNID(CIHashHandle);
    HASH_INFO_S *pstHashInfo = &g_stCipherHashData[hHashHandle];

    if(NULL == pu8InputData)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
		pstHashInfo->bIsUsed = HI_FALSE;
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if (0 == u32InputDataLen)
	{
		if( pstHashInfo->u32TotalDataLen > 0)
		{
			return HI_SUCCESS;
		}
		else
		{
			HI_HASH_LOCK();
            pstHashInfo->bIsUsed = HI_FALSE;
			HI_HASH_UNLOCK();
		    return HI_ERR_CIPHER_INVALID_PARA;
		}
	}

    CHECK_CIPHER_OPEN();

    HI_HASH_LOCK();

    if ((pstHashInfo->u32TotalDataLen + u32InputDataLen < u32InputDataLen)
        || (pstHashInfo->u8LastBlockSize + u32InputDataLen < u32InputDataLen))
    {
        HI_ERR_CIPHER("Invalid u32InputDataLen!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        HI_HASH_UNLOCK();
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pstHashInfo->stMMZBuffer.u32Size = 0;
	pstHashInfo->u32TotalDataLen += u32InputDataLen;
    u32TotalSize = pstHashInfo->u8LastBlockSize + u32InputDataLen;
    if( u32TotalSize < HASH_BLOCK_SIZE)
    {
        memcpy(pstHashInfo->u8LastBlock+pstHashInfo->u8LastBlockSize, pu8InputData, u32InputDataLen);
        pstHashInfo->u8LastBlockSize+=u32InputDataLen;
        HI_HASH_UNLOCK();
	    return HI_SUCCESS;
    }
    else
    {
        memcpy(pstHashInfo->stMMZBuffer.pu8StartVirAddr, pstHashInfo->u8LastBlock, pstHashInfo->u8LastBlockSize);
        pstHashInfo->stMMZBuffer.u32Size = pstHashInfo->u8LastBlockSize;

        /* save tail data */
        memset(pstHashInfo->u8LastBlock, 0, HASH_BLOCK_SIZE);
        pstHashInfo->u8LastBlockSize = (u32InputDataLen + pstHashInfo->stMMZBuffer.u32Size) % HASH_BLOCK_SIZE;
        memcpy(pstHashInfo->u8LastBlock, pu8InputData + (u32InputDataLen - pstHashInfo->u8LastBlockSize),
            pstHashInfo->u8LastBlockSize);
    }

    u32TotalSize  = u32InputDataLen - pstHashInfo->u8LastBlockSize;
    u32Tmp = (u32TotalSize + pstHashInfo->stMMZBuffer.u32Size + g_u32HashBaseBufferLen - 1) / g_u32HashBaseBufferLen;
    /* Send data down piece by piece */
    for ( i = 0 ; i < u32Tmp; i++ )
    {
        u32CopySize = g_u32HashBaseBufferLen - pstHashInfo->stMMZBuffer.u32Size;
        if (u32CopySize > (u32TotalSize - u32BufInUsedLen))
        {
            u32CopySize = u32TotalSize - u32BufInUsedLen;
        }
        memcpy((HI_U8 *)pstHashInfo->stMMZBuffer.pu8StartVirAddr + pstHashInfo->stMMZBuffer.u32Size,
		       pu8InputData + u32BufInUsedLen, u32CopySize);
	    pstHashInfo->stMMZBuffer.u32Size += u32CopySize;
        u32BufInUsedLen+=u32CopySize;

        memset(&stHashData, 0, sizeof(stHashData));
        stHashData.enShaType = pstHashInfo->enShaType;
		stHashData.u32DataLen = pstHashInfo->stMMZBuffer.u32Size;
        stHashData.u32DataPhy = pstHashInfo->stMMZBuffer.u32StartPhyAddr;
        stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
        memcpy(stHashData.u32ShaVal, pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
		pstHashInfo->stMMZBuffer.u32Size = 0;
		ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASHUPDATE, &stHashData);
	    if(ret != HI_SUCCESS)
	    {
	        HI_ERR_CIPHER("Error, ioctl for hash update failed!\n");
            pstHashInfo->bIsUsed = HI_FALSE;
            HI_HASH_UNLOCK();
	        return ret;
	    }
        memcpy(pstHashInfo->u32ShaVal, stHashData.u32ShaVal, pstHashInfo->u32ShaLen);
    }

    if((u32BufInUsedLen + pstHashInfo->u8LastBlockSize) != u32InputDataLen)
    {
        HI_ERR_CIPHER("Error, Invalid data size: 0x%x!\n", u32BufInUsedLen);
        pstHashInfo->bIsUsed = HI_FALSE;
        HI_HASH_UNLOCK();
        return HI_FAILURE;
    }

    HI_HASH_UNLOCK();

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HashFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8OutputHash)
{
    HI_S32 ret = HI_FAILURE;
    HASH_INFO_S *pstHashInfo = NULL;
    CIPHER_HASH_DATA_S stHashData;
	HI_U32 u32Tmp = 0;
    HI_HANDLE hHashHandle = 0x0;

    Hash_CheckHandle(CIHashHandle);
    hHashHandle = HI_HANDLE_GET_CHNID(CIHashHandle);

    pstHashInfo = &g_stCipherHashData[hHashHandle];

    if ( NULL == pu8OutputHash )
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        return HI_FAILURE;
    }

    CHECK_CIPHER_OPEN();
    HI_HASH_LOCK();

    memcpy(pstHashInfo->stMMZBuffer.pu8StartVirAddr, pstHashInfo->u8LastBlock, pstHashInfo->u8LastBlockSize);
    pstHashInfo->stMMZBuffer.u32Size = pstHashInfo->u8LastBlockSize;
    memset(pstHashInfo->u8LastBlock, 0, HASH_BLOCK_SIZE);
    pstHashInfo->u8LastBlockSize = 0;

	u32Tmp = HashMsgPadding(pstHashInfo->stMMZBuffer.pu8StartVirAddr, pstHashInfo->stMMZBuffer.u32Size, pstHashInfo->u32TotalDataLen);

//    HI_PRINT_HEX("SHA", (HI_U8*)pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
//    HI_PRINT_HEX("DATA", pstHashInfo->stMMZBuffer.pu8StartVirAddr, u32Tmp);

    memset(&stHashData, 0, sizeof(stHashData));
    stHashData.enShaType = pstHashInfo->enShaType;
    stHashData.u32DataLen = u32Tmp;
    stHashData.u32DataPhy = pstHashInfo->stMMZBuffer.u32StartPhyAddr;
    stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
    memcpy(stHashData.u32ShaVal, pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASHFINAL, &stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash final failed!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        HI_HASH_UNLOCK();
        return ret;
    }

    memcpy(pu8OutputHash, stHashData.u32ShaVal, pstHashInfo->u32ShaLen);

    pstHashInfo->bIsUsed = HI_FALSE;

    HI_HASH_UNLOCK();

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacKeyInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_U8 au8Hmackey[HASH_BLOCK_SIZE])
{
    HI_S32 ret = HI_FAILURE;
    HI_HANDLE hHash = 0;

    if((pstHashAttr == NULL)
        ||(pstHashAttr->pu8HMACKey == NULL)
        ||(pstHashAttr->u32HMACKeyLen == 0)
        || (au8Hmackey == NULL))
    {
        HI_ERR_CIPHER("Invalid parameters.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*key length is less than 64bytes, copy directly*/
    if(pstHashAttr->u32HMACKeyLen <= HASH_BLOCK_SIZE)
    {
        memcpy(au8Hmackey, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
        return HI_SUCCESS;
    }

    /*key length more than 64bytes, calcute the hash result*/
    ret = CIPHER_HashInit(pstHashAttr, &hHash);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashInit failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    ret = CIPHER_HashUpdate(hHash, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashUpdate failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    ret = CIPHER_HashFinal(hHash,au8Hmackey);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashFinal failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 key_pad[HASH_BLOCK_SIZE];
    HASH_INFO_S *pstHashInfo = NULL;
	HI_U32 i = 0;
    HI_HANDLE softHashHandle = 0x0;

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT)
     || (NULL == pstHashAttr->pu8HMACKey)
     || (0 == pstHashAttr->u32HMACKeyLen))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

	/* Init hmac key */
    memset(key_pad, 0, sizeof(key_pad));
    ret = CIPHER_HmacKeyInit(pstHashAttr, key_pad);
	if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("Hmac key initial failed!\n");
		return HI_FAILURE;
	}

	/* hash i_key_pad and message start */
	ret = CIPHER_HashInit(pstHashAttr, pHashHandle);
	if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("hash i_key_pad and message start failed!\n");
		return HI_FAILURE;
	}

    Hash_CheckHandle(*pHashHandle);
    softHashHandle = HI_HANDLE_GET_CHNID(*pHashHandle);

    pstHashInfo = (HASH_INFO_S *)&g_stCipherHashData[softHashHandle];
    memcpy(pstHashInfo->u8Mac, key_pad, HASH_BLOCK_SIZE);

	/* generate i_key_pad */
    for(i=0; i < HASH_BLOCK_SIZE; i++)
    {
        key_pad[i] ^= 0x36;
    }

	/* hash i_key_pad update */
	ret = CIPHER_HashUpdate(*pHashHandle, key_pad, HASH_BLOCK_SIZE);
	if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("hash i_key_pad and message update failed!\n");
		return HI_FAILURE;
	}

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacUpdate(HI_HANDLE CIHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_S32 ret = HI_FAILURE;

	ret = CIPHER_HashUpdate(CIHashHandle, pu8InputData, u32InputDataLen);
	if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("hmac message update failed!\n");
		return HI_FAILURE;
	}

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8Output)
{
    HI_S32 ret = HI_FAILURE;
    HASH_INFO_S *pstHashInfo;
	HI_HANDLE hHash = HI_INVALID_HANDLE;
	HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_U8 hash_sum_1[HASH256_SIGNATURE_SIZE];
    HI_U8 key_pad[HASH_BLOCK_SIZE];
    HI_U32 i;
    HI_HANDLE hHashHandle = 0x0;

    Hash_CheckHandle(CIHashHandle);
    hHashHandle = HI_HANDLE_GET_CHNID(CIHashHandle);

	if ( NULL == pu8Output )
	{
   	    HI_ERR_CIPHER("Invalid point!\n");
		return HI_ERR_CIPHER_INVALID_POINT;
	}

    pstHashInfo = &g_stCipherHashData[hHashHandle];

    memset(&stHashAttr, 0, sizeof(stHashAttr));
	stHashAttr.eShaType = pstHashInfo->enShaType;
    memcpy(key_pad, pstHashInfo->u8Mac, HASH_BLOCK_SIZE);

    /* hash i_key_pad+message finished */
    ret = CIPHER_HashFinal(CIHashHandle, hash_sum_1);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final i_key_pad+message failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* generate o_key_pad */
    for(i=0; i < HASH_BLOCK_SIZE; i++)
    {
        key_pad[i] ^= 0x5c;
    }

    /* hash o_key_pad+hash_sum_1 start */
    ret = CIPHER_HashInit(&stHashAttr, &hHash);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Init o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash o_key_pad */
    ret = CIPHER_HashUpdate(hHash, key_pad, HASH_BLOCK_SIZE);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update o_key_pad failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash hash_sum_1 */
	if ( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == stHashAttr.eShaType )
	{
	    ret = CIPHER_HashUpdate(hHash, (HI_U8 *)hash_sum_1, HASH1_SIGNATURE_SIZE);
	}
	else
	{
	    ret = CIPHER_HashUpdate(hHash, (HI_U8 *)hash_sum_1, HASH256_SIGNATURE_SIZE);
	}

    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash o_key_pad+hash_sum_1 finished */
    ret = CIPHER_HashFinal(hHash, pu8Output);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    if ((NULL == pstHashAttr) || (NULL == pHashHandle))
    {
   	    HI_ERR_CIPHER("Invalid point!\n");
		return HI_ERR_CIPHER_INVALID_POINT;
    }

	if ((HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType) )
	{
	    return CIPHER_HashInit(pstHashAttr, pHashHandle);
	}
    else if ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
    {
        return CIPHER_HmacInit(pstHashAttr, pHashHandle);
    }

    return HI_FAILURE;
}

HI_S32 HI_MPI_CIPHER_HashUpdate(HI_HANDLE CIHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_HANDLE hHashHandle = 0x0;

    Hash_CheckHandle(CIHashHandle);
    hHashHandle = HI_HANDLE_GET_CHNID(CIHashHandle);

    HASH_INFO_S *pstHashInfo = (HASH_INFO_S*)&g_stCipherHashData[hHashHandle];

    if (NULL == pu8InputData)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashInfo->enShaType ) )
	{
		return CIPHER_HashUpdate(CIHashHandle, pu8InputData, u32InputDataLen);
	}
    else if ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType))
    {
        return CIPHER_HmacUpdate(CIHashHandle, pu8InputData, u32InputDataLen);
    }

    return HI_FAILURE;
}

HI_S32 HI_MPI_CIPHER_HashFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8OutputHash)
{
    HI_HANDLE hHashHandle = 0x0;

    Hash_CheckHandle(CIHashHandle);
    hHashHandle = HI_HANDLE_GET_CHNID(CIHashHandle);
    HASH_INFO_S *pstHashInfo = (HASH_INFO_S*)&g_stCipherHashData[hHashHandle];

    if (NULL == pu8OutputHash)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if ((HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashInfo->enShaType) )
	{
		return CIPHER_HashFinal(CIHashHandle, pu8OutputHash);
	}
    else if ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType))
    {
        return CIPHER_HmacFinal(CIHashHandle, pu8OutputHash);
    }

    return HI_FAILURE;
}

HI_S32 HI_MPI_CIPHER_Hash(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                          HI_U32 u32DataPhyAddr, HI_U32 u32ByteLength,
                          HI_U8 *pu8OutputHash)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 u32ShaLen;

    if((NULL == pstHashAttr) || (NULL == pu8OutputHash))
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((0 == u32ByteLength)
        || (0 != u32ByteLength % 64)
        || (0 == u32DataPhyAddr)
        || (0 != u32DataPhyAddr % 4)
        || (u32DataPhyAddr + u32ByteLength < u32ByteLength))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_HASH_LOCK();

    stHashData.enShaType = pstHashAttr->eShaType;
    stHashData.u32DataPhy = u32DataPhyAddr;
    stHashData.u32DataLen = u32ByteLength;

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

    stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCHASH, &stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash final failed!\n");
        HI_HASH_UNLOCK();
        return ret;
    }

    memcpy(pu8OutputHash, stHashData.u32ShaVal, u32ShaLen);

    HI_HASH_UNLOCK();

    return ret;
}

#endif

#ifdef CFG_HI_CIPHER_RNG_SUPPORT
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

    stRNG.u32RNG = 0;
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

#ifdef CFG_HI_CIPHER_RSA_SUPPORT
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

    if ((u16NLen <= RSA_MAX_KEY_LEN) && (u16NLen >= RSA_MIN_KEY_LEN))
    {
        *pu32KeyLen = u16NLen;
    }
    else
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\n", u16NLen);
        return HI_ERR_CIPHER_INVALID_PARA;
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

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

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
        for(j=0; (j<u32MdLen) && (u32Outlen < u32MaskLen); j++)
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

    if (NULL == pu8Rand)
    {
        HI_ERR_CIPHER("Invalid point.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memset(pu8Rand, 0, u32Size);
    for(i=0; i<u32Size; i+=4)
    {
        ret = HI_MPI_CIPHER_GetRandomNumber(&u32Rand, -1);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
            return HI_FAILURE;
        }
        pu8Rand[i+3]   = (HI_U8)(u32Rand >> 24)& 0xFF;
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
                return HI_FAILURE;
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
	HI_U32 i;
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
	return HI_SUCCESS;
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
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32DBLen;
    HI_U8 *pu8DB;
    HI_U8 *pu8Seed;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;

    CHECK_CIPHER_OPEN();

    if ((pu8Input == HI_NULL)
        || (pu8Output == HI_NULL)
        || (pu32OutLen == HI_NULL))
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

    memcpy(pu8DB, pu8LHASH, u32HLen);                                 /*set lHash*/
    memset(&pu8DB[u32HLen], 0, u32DBLen - u32InLen - u32HLen -1);     /*set PS with 0x00*/
    pu8DB[u32DBLen - u32InLen - 1] = 0x01;                            /*set 0x01 after PS*/
    memcpy(&pu8DB[u32DBLen - u32InLen], pu8Input, u32InLen);          /*set M*/
    ret = RSA_GetRandomNumber(pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8DB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
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
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32Index = 0;

    CHECK_CIPHER_OPEN();

    if ((pu8Input == HI_NULL)
        || (pu8Output == HI_NULL)
        || (pu32OutLen == HI_NULL))
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
        return HI_FAILURE;
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
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32PLen;
    HI_U8 *pu8EB;

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
        memset(pu8EB, 0x00, u32PLen);
        break;
    case 0x01:
        memset(pu8EB, 0xFF, u32PLen);
        break;
    case 0x02:
        ret = RSA_GetRandomNumber(pu8EB, u32PLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
            return HI_FAILURE;
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
	HI_U32 u32PadLen = 0;
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
        memset(p, 0xFF, u32PadLen);
        p += u32PadLen;
        *p++ = 0;
        memcpy(p, ASN1_HASH_SHA1, 15);
        memcpy(p + 15, pu8Input, u32InLen);
        break;
    case HI_UNF_CIPHER_HASH_TYPE_SHA256:
        u32PadLen = u32KeyLen - 3 - 51;
        memset(p, 0xFF, u32PadLen);
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
					|					|
					V					|	  +--+
					xor <----- MGF <----|	  |bc|
					|					|	  +--+
					|					|      |
					V					V      V
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
	HI_U32 ret = HI_FAILURE;
	HI_U32 u32SLen = 0;
	HI_U8 *pu8M = HI_NULL;
	HI_U8 u8Salt[32] = {0};
	HI_U8 *pu8MaskedDB = HI_NULL;
	HI_U8 *pu8Maskedseed = HI_NULL;
	HI_U32 u32Index = 0;
	HI_U32 u32KeyLen = 0;
	HI_U32 u32MSBits = 0;
    HI_HANDLE HashHandle = 0;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

	if ((pu8Input == NULL) || (pu8Output == NULL) || (pu32OutLen == NULL))
	{
		HI_ERR_CIPHER("zero point\n");
		return HI_ERR_CIPHER_INVALID_POINT;
	}

	u32SLen = u32HLen;
    u32KeyLen = (u32EmBit + 7)/8;
	u32MSBits = (u32EmBit - 1) & 0x07;

    *pu32OutLen = u32KeyLen;

	if ((u32KeyLen < (u32HLen + u32SLen + 2)) || (u32HLen + u32SLen + 2 < u32SLen))
	{
		HI_ERR_CIPHER("message too long\n");
		return HI_ERR_CIPHER_INVALID_PARA;
	}

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

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
	memset(pu8M, 0x00, 8);
	u32Index+=8;
	memcpy(&pu8M[u32Index], pu8Input, u32InLen);
	u32Index+=u32InLen;
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
	memset(&pu8MaskedDB[u32Index], 0x00, u32KeyLen - u32SLen -u32HLen - 2);
	u32Index+=u32KeyLen - u32SLen -u32HLen - 2;
	pu8MaskedDB[u32Index++] = 0x01;
	memcpy(&pu8MaskedDB[u32Index], u8Salt, u32SLen);
	ret = RSA_PKCS1_MGF1(enHashType, pu8Maskedseed, u32HLen, pu8MaskedDB, u32KeyLen - u32HLen -1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
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

    if ((pstRsaEnc == HI_NULL)
        || (HI_NULL == pu8Input)
        || (HI_NULL == pu8Output)
        || (HI_NULL == pu32OutLen)
        || (HI_NULL == pstRsaEnc->stPubKey.pu8N)
        || (HI_NULL == pstRsaEnc->stPubKey.pu8E))
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
        HI_ERR_CIPHER("inlen is too large.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(u8EM, 0, sizeof(u8EM));

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

    memset(&stRsaData, 0, sizeof(stRsaData));
    stRsaData.pu8N = pstRsaEnc->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaEnc->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPubKey.u16ELen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8Output;
    stRsaData.u32DataLen = u32KeyLen;

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
    HI_U32 i;
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
        return HI_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    if(memcmp(pu8DB, pu8LHASH, u32HLen) != 0)
    {
        HI_ERR_CIPHER("LHASH error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    for(i=u32HLen; i < u32KeyLen - u32HLen - 1; i++)
    {
        if(pu8DB[i] == 0x01)
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

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
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

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
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
    HI_U32 ret = HI_SUCCESS;
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

static HI_U32 RSA_padding_check_PKCS1_PSS(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                          HI_U32 u32HLen,
                                          HI_U32 u32EmBit,
							              HI_U8 *pu8Input,
							              HI_U32 u32InLen,
							              HI_U8 *pu8MHash)
{
	HI_U32 ret = HI_FAILURE;
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

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

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
	memset(pu8M, 0x00, 8);
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
        pu8M = NULL;
        return ret;
    }
    ret = HI_MPI_CIPHER_HashUpdate(HashHandle, pu8M, u32Index);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashUpdate failed\n");
        free(pu8M);
        pu8M = NULL;
        return ret;
    }
    ret = HI_MPI_CIPHER_HashFinal(HashHandle, u8H);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("HashFinal failed\n");
        free(pu8M);
        pu8M = NULL;
        return ret;
    }
    free(pu8M);
    pu8M = NULL;

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
    HI_S32 ret = HI_FAILURE;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U8 u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    HI_U32 u32HLen = 0;
    HI_U32 u32KeyLen = 0;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;

    if ((HI_NULL == pstRsaDec)
        || (HI_NULL == pu8Input)
        || (HI_NULL == pu8Output)
        || (HI_NULL == pu32OutLen)
        || (HI_NULL == pstRsaDec->stPriKey.pu8N)
        || (HI_NULL == pstRsaDec->stPriKey.pu8D))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

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

    memset(&stRsaData, 0, sizeof (stRsaData));
    stRsaData.pu8N = pstRsaDec->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaDec->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPriKey.u16DLen;
    stRsaData.pu8Input = pu8Input;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;

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

    if ((pstRsaSign->stPriKey.pu8N == HI_NULL) || (pstRsaSign->stPriKey.pu8D == HI_NULL))
    {
        HI_ERR_CIPHER("key is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((pu8OutSign == HI_NULL) || (pu32OutSignLen == HI_NULL))
    {
        HI_ERR_CIPHER("out sign is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((pu8InData == HI_NULL) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("In data is null.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((u32InDataLen == 0) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

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

    memset(u8EM, 0, sizeof(u8EM));
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
        HI_ERR_CIPHER("RSA pading error\n");
        return ret;
    }

    if(*pu32OutSignLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InSigntLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memset(&stRsaData, 0, sizeof(stRsaData));
    stRsaData.pu8N = pstRsaSign->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaSign->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaSign->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaSign->stPriKey.u16DLen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8OutSign;
    stRsaData.u32DataLen = u32KeyLen;

//    HI_PRINT_HEX ("N", stRsaData.pu8N, stRsaData.u16NLen);
//    HI_PRINT_HEX ("K", stRsaData.pu8K, stRsaData.u16KLen);
//    HI_PRINT_HEX ("M", stRsaData.pu8Input, stRsaData.u32DataLen);

    ret = ioctl(g_CipherDevFd, CMD_CIPHER_CALCRSA, &stRsaData);

    return ret;
}

HI_S32 HI_MPI_CIPHER_RsaPrivateEnc(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
							 	   HI_U8 *pu8Input, HI_U32 u32InLen,
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_U8  u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];

    CHECK_CIPHER_OPEN();

    if ((HI_NULL == pstRsaEnc)
        || (HI_NULL == pu8Input)
        || (HI_NULL == pu8Output)
        || (HI_NULL == pu32OutLen)
        || (HI_NULL == pstRsaEnc->stPriKey.pu8N)
        || (HI_NULL == pstRsaEnc->stPriKey.pu8D))
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
        HI_ERR_CIPHER("inlen is too large.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(u8EM, 0, sizeof(u8EM));

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
    HI_U8  u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
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

    if (pu8InSign == HI_NULL)
    {
        HI_ERR_CIPHER("In sign is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((pu8InData == HI_NULL) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("In data is null.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((u32InDataLen == 0) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
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

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    memset(&stRsaData, 0, sizeof(stRsaData));
    stRsaData.pu8N = pstRsaVerify->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaVerify->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaVerify->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaVerify->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8InSign;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;

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
    HI_U8 u8EM[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    HI_U32 u32HLen = 0;
    HI_U32 u32KeyLen = 0;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;

    if ((pstRsaDec == HI_NULL)
        || (HI_NULL == pu8Input)
        || (HI_NULL == pu8Output)
        || (HI_NULL == pu32OutLen)
        || (HI_NULL == pstRsaDec->stPubKey.pu8N)
        || (HI_NULL == pstRsaDec->stPubKey.pu8E))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

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

    memset(&stRsaData, 0, sizeof(stRsaData));
    stRsaData.pu8N = pstRsaDec->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaDec->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8Input;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;

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


HI_S32 HI_MPI_CIPHER_WriteOTPKey(HI_U32 u32OptId, const HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CIPHER_OTP_KEY_S stOtpKey;

    if (NULL == pu8Key)
    {
        HI_ERR_CIPHER("para pu8Key is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32OptId > 3)
    {
        HI_ERR_CIPHER("The opt id must be {0,1,2,3}!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (u32KeyLen != 16)
    {
        HI_ERR_CIPHER("The length of key must be equal to 16!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    memset(&stOtpKey, 0, sizeof(stOtpKey));
    stOtpKey.u32OtpId  = u32OptId;
    stOtpKey.pu8Key    = (HI_U8*)pu8Key;
    stOtpKey.u32KeyLen = u32KeyLen;

    s32Ret = ioctl(g_CipherDevFd, CMD_CIPHER_WRITEOTPKEY, &stOtpKey);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return s32Ret;
}



