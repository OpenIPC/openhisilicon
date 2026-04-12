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
#include "hi_type.h"
#include "hi_drv_cipher.h"
#include "drv_hash.h"
#include "hi_type.h"
#include "drv_cipher_define.h"
#include "drv_cipher_ioctl.h"
#include "hal_cipher.h"
#include "drv_cipher.h"
#include "config.h"
#include "drv_osal_lib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#ifdef CIPHER_HASH_SUPPORT

/* Set the defualt timeout value for hash calculating (5000 ms)*/
#define HASH_MAX_DURATION (5000)
#define HASH_PADDING_LEGNTH     8
#define HASH_MMZ_BUF_LEN	(1*1024*1024)		//1M
#define HASH_MAX_RESULT     (32)

#ifdef __HuaweiLite__
crypto_mutex_t g_HashMutexKernel;
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
DECLARE_MUTEX(g_HashMutexKernel);
#else
DEFINE_SEMAPHORE(g_HashMutexKernel);
#endif
#endif

typedef enum
{
    HASH_READY,
    REC_READY,
    DMA_READY,
}HASH_WAIT_TYPE;

#define HASH_BLOCK_SIZE          (64)
#define HASH_PAD_MAX_LEN         (64)
#define HASH1_SIGNATURE_SIZE     (20)
#define HASH256_SIGNATURE_SIZE   (32)
//#define HASH_MMZ_BUF_LEN         (1*1024*1024)        //1M
//#define HASH_MMZ_TAIL_LEN        (8*1024)             //8K
#define HASH_CHANNAL_MAX_NUM     (8)
#define U32_MAX_NUM            (0xFFFFFFFF)

extern CIPHER_COMM_S g_stCipherComm;

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

/*! \the source of hash message */
typedef enum hiHASH_CHUNK_SRC_E
{
    HASH_CHUNCK_SRC_LOCAL, /*!<  Local buffer, e.g. Kernel  */
    HASH_CHUNCK_SRC_USER,  /*!<  User buffer, use copy_from_user to read data */
}HASH_CHUNK_SRC_E;

static HASH_INFO_S g_stCipherHashData[HASH_CHANNAL_MAX_NUM];
HI_U32 g_u32HashBaseBufferLen = HASH_MMZ_BUF_LEN;

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
    if (HI_TRUE != g_stCipherHashData[HI_HANDLE_GET_CHNID(CIHandle)].bIsUsed) \
    { \
        HI_ERR_CIPHER("chan %d is not open\n", HI_HANDLE_GET_CHNID(CIHandle)); \
        return HI_ERR_CIPHER_INVALID_HANDLE; \
    } \
} while (0)


HI_S32 HAL_Cipher_HashSoftReset(HI_VOID)
{
    HI_U32 CipherCrgValue;
    HI_U32 pvirt;

    if (REG_CRG_CLK_PHY_ADDR_SHA == 0x00)
    {
        return HI_SUCCESS;
    }

    pvirt = (HI_U32)crypto_ioremap_nocache(REG_CRG_CLK_PHY_ADDR_SHA, 0x100);

    HAL_CIPHER_ReadReg(pvirt, &CipherCrgValue);
    HAL_SET_BIT(CipherCrgValue, CRG_RST_BIT_SHA); /* reset */
    HAL_SET_BIT(CipherCrgValue, CRG_CLK_BIT_SHA); /* set the bit 3, clock opened */
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);

    /* clock select and cancel reset 0x30100*/
    HAL_CLEAR_BIT(CipherCrgValue, CRG_RST_BIT_SHA); /* cancel reset */
    HAL_SET_BIT(CipherCrgValue, CRG_CLK_BIT_SHA);   /* set the bit 1, clock opened */
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);

    crypto_iounmap((void*)pvirt);

    return HI_SUCCESS;
}

inline HI_S32 HASH_WaitReady(HASH_WAIT_TYPE enType, HI_BOOL bFast)
{
    CIPHER_SHA_STATUS_U unCipherSHAstatus;
    HI_U32 u32TimeOut = 0;
    HI_SIZE_T ulStartTime = 0;
    HI_SIZE_T ulLastTime = 0;
    HI_SIZE_T ulDuraTime = 0;

    cipher_flush_cache_all();

    /* wait for hash_rdy */
    while(1)
    {
        unCipherSHAstatus.u32 = 0;
        (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAstatus.u32);
//        HI_PRINT("unCipherSHAstatus 0x%x\n", unCipherSHAstatus.u32);
        if(HASH_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.hash_rdy)
            {
                break;
            }
        }
        else if (REC_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.rec_rdy)
            {
                break;
            }
        }
        else
        {
            HI_ERR_CIPHER("Error! Invalid wait type!\n");
            return HI_FAILURE;
        }

        if (bFast)
        {
            u32TimeOut++;
            if (u32TimeOut >= 0x100000)
            {
                HI_ERR_CIPHER("Error! Hash time out (0x%x)!\n", u32TimeOut);
                return HI_FAILURE;
            }

            //No OP instruction
            //__asm__ __volatile__("": : :"memory");
        }
        else
        {
            if(ulStartTime == 0)
            {
                ulStartTime = crypto_get_tickcount();
            }

            ulLastTime = crypto_get_tickcount();
            ulDuraTime = ulLastTime - ulStartTime;
    		crypto_msleep(1);
            if (ulDuraTime >= HASH_MAX_DURATION )
            {
                HI_ERR_CIPHER("Error! Hash time out!\n");
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

#ifdef CIPHER_MHASH_SUPPORT
static HI_U32 HAL_Hash_Small2Large(HI_U32 u32SamllVal)
{
    HI_U32 u32LargeVal = 0;

    u32LargeVal  = (u32SamllVal >> 24) & 0xFF;
    u32LargeVal |= ((u32SamllVal >> 16) & 0xFF) << 8;
    u32LargeVal |= ((u32SamllVal >> 8) & 0xFF) << 16;
    u32LargeVal |= ((u32SamllVal) & 0xFF) << 24;

    return u32LargeVal;
}
#endif

HI_S32 HAL_Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
#ifdef CIPHER_MHASH_SUPPORT
    HI_U32 i = 0;
    HI_U32 u32RegAddr = 0;
    HI_U32 u32ShaVal = 0;
#endif
    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return ret;
    }

#if 0
    /* set hmac-sha key */
    if( ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        && (HI_CIPHER_HMAC_KEY_FROM_CPU == pCipherHashData->enHMACKeyFrom) )
    {
        for( i = 0; i < CIPHER_HMAC_KEY_LEN; i = i + 4)
        {
            u32WriteData = (pCipherHashData->pu8HMACKey[3+i] << 24) |
                           (pCipherHashData->pu8HMACKey[2+i] << 16) |
                           (pCipherHashData->pu8HMACKey[1+i] << 8)  |
                           (pCipherHashData->pu8HMACKey[0+i]);
            (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_MCU_KEY0 + i, u32WriteData);
        }
    }
#endif

    /* write total len low and high */
#ifdef CIPHER_MHASH_SUPPORT
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pCipherHashData->u32DataLen);
#else
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, 0xFFFFFFFF);
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 0;
    if( HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType )
    {
		/* unCipherSHACtrl.bits.hardkey_hmac_flag = 1; */
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        return HI_FAILURE;
    }

    unCipherSHACtrl.bits.small_end_en = 1;
#ifdef CIPHER_MHASH_SUPPORT
    unCipherSHACtrl.bits.sha_init_update_en = 1;
    u32RegAddr = CIPHER_HASH_REG_INIT1_UPDATE;
    for(i=0; i<8; i++)
    {
        u32ShaVal = HAL_Hash_Small2Large(pCipherHashData->u32ShaVal[i]);
        (HI_VOID)HAL_CIPHER_WriteReg(u32RegAddr+i*4, u32ShaVal);
 //        HI_PRINT ("IV[%d] 0x%x\n", i, u32ShaVal);
    }
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);

    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

	if ((pCipherHashData->u32DataLen > HASH_MMZ_BUF_LEN)  || (pCipherHashData->enShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT))
	{
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
	}

    ret= HASH_WaitReady(REC_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return ret;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_START_ADDR, pCipherHashData->u32DataPhy);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_LEN, pCipherHashData->u32DataLen);

    ret  = HASH_WaitReady(REC_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;
    HI_U32 u32RegAddr = 0;
    HI_U32 i = 0;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

    /* wait for rec_ready instead of hash_ready */
    ret= HASH_WaitReady(REC_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAStatus.u32);

    u32RegAddr = CIPHER_HASH_REG_SHA_OUT1;
    if( (0x00 == unCipherSHAStatus.bits.error_state) && (0x00 == unCipherSHAStatus.bits.len_err))
    {
        if( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType))
        {

            for(i = 0; i < 5; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr+i*4, &pCipherHashData->u32ShaVal[i]);
            }
        }
        else if( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
              || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        {
            for(i = 0; i < 8; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr+i*4, &pCipherHashData->u32ShaVal[i]);
    		}
        }
        else
        {
            HI_ERR_CIPHER("Invalid hash type : %d!\n", pCipherHashData->enShaType);
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\n", unCipherSHAStatus.bits.error_state);
        HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_U32 HashMsgPadding(HI_U8 *pu8Msg, HI_U32 u32ByteLen, HI_U32 u32TotalLen)
{
    HI_U32 u32Tmp = 0;
    HI_U32 u32PaddingLen = 0;

    if( NULL == pu8Msg )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        return HI_FAILURE;
    }

    u32Tmp = u32TotalLen % 64;
    /* 56 = 64 - 8, 120 = 56 + 64 */
    u32PaddingLen = (u32Tmp < 56) ? (56 - u32Tmp) : (120 - u32Tmp);
    /* add 8 bytes fix data length */
    u32PaddingLen += 8;

    /* Format(binary): {data|1000...00| fix_data_len(bits)} */
    pu8Msg[u32ByteLen++] = 0x80;
    crypto_memset(&pu8Msg[u32ByteLen], 0, u32PaddingLen - 1 - 8);

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

static HI_S32 Cipher_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 u32SoftId = 0;
    HASH_INFO_S *pstHashInfo = HI_NULL;
    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(crypto_mutex_lock_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("crypto_mutex_lock_interruptible failed!\n");
        return HI_FAILURE;
    }

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
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_FAILURE;
    }

    pstHashInfo = &g_stCipherHashData[u32SoftId];
    crypto_memset(pstHashInfo, 0, sizeof(HASH_INFO_S));
    crypto_memset(&stHashData, 0, sizeof(CIPHER_HASH_DATA_S));
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

    ret = HI_DRV_CIPHER_CalcHashInit(&stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, HI_DRV_CIPHER_CalcHashInit failed!\n");
        crypto_mutex_unlock(&g_HashMutexKernel);
        return ret;
    }

    pstHashInfo->stMMZBuffer = g_stCipherComm.stTempPhyBuf;

    pstHashInfo->bIsUsed = HI_TRUE;
    pstHashInfo->hHandle = u32SoftId;

    *pHashHandle = HI_HANDLE_MAKEHANDLE(HI_ID_HASH, 0, u32SoftId);

    crypto_mutex_unlock(&g_HashMutexKernel);

    return HI_SUCCESS;
}

HI_S32 Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

#ifndef CIPHER_MHASH_SUPPORT
    ret = HAL_Cipher_CalcHashInit(pCipherHashData);
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Cipher hash init failed!\n");
        return HI_FAILURE;
    }
#endif

    return ret;
}

static HI_S32 HashUpdate_Copy_InputData(HI_U8 *dest, HI_U8 *src, HI_U32 u32SrcLen, HASH_CHUNK_SRC_E srcFrom)
{
    HI_S32 ret = HI_FAILURE;

    if ((HI_NULL == dest) || (HI_NULL == src))
    {
        HI_ERR_CIPHER("Hash update with invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (HASH_CHUNCK_SRC_LOCAL == srcFrom)
    {
        crypto_memcpy(dest, src, u32SrcLen);
    }
    else if (HASH_CHUNCK_SRC_USER == srcFrom)
    {
        ret = copy_from_user(dest, src, u32SrcLen);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Hash update copy input data from user failed!\n");
            return ret;
        }
    }
    else
    {
        HI_ERR_CIPHER("Hash update with invalid input data!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 Cipher_HashUpdate(HI_HANDLE CIHashHandle, HI_U8 *pu8InputData,
                                HI_U32 u32InputDataLen, HASH_CHUNK_SRC_E srcFrom)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32Tmp = 0;
    HI_U32 u32TotalSize = 0;
    HI_U32 u32CopySize = 0;
    HI_U32 u32BufInUsedLen = 0;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 i = 0;
    HI_U32 u32SoftHash = 0x0;
    HASH_INFO_S *pstHashInfo = HI_NULL;

    Hash_CheckHandle(CIHashHandle);
    u32SoftHash = HI_HANDLE_GET_CHNID(CIHashHandle);
    pstHashInfo = &g_stCipherHashData[u32SoftHash];

    if (NULL == pu8InputData)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(crypto_mutex_lock_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("crypto_mutex_lock_interruptible failed!\n");
        return HI_FAILURE;
    }

    if (0 == u32InputDataLen)
    {
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_SUCCESS;
    }

    if ((pstHashInfo->u32TotalDataLen + u32InputDataLen < pstHashInfo->u32TotalDataLen)
        || (pstHashInfo->u8LastBlockSize + u32InputDataLen < pstHashInfo->u8LastBlockSize))
    {
        HI_ERR_CIPHER("Invalid u32InputDataLen:0x%x!\n",u32InputDataLen);
        pstHashInfo->bIsUsed = HI_FALSE;
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pstHashInfo->stMMZBuffer.u32Size = 0;
    pstHashInfo->u32TotalDataLen += u32InputDataLen;
    u32TotalSize = pstHashInfo->u8LastBlockSize + u32InputDataLen;
    if( u32TotalSize < HASH_BLOCK_SIZE)
    {
        ret = HashUpdate_Copy_InputData(pstHashInfo->u8LastBlock+pstHashInfo->u8LastBlockSize,
            pu8InputData, u32InputDataLen, srcFrom);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Hash update copy input datafailed!\n");
            pstHashInfo->bIsUsed = HI_FALSE;
            crypto_mutex_unlock(&g_HashMutexKernel);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        pstHashInfo->u8LastBlockSize+=u32InputDataLen;
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_SUCCESS;
    }
    else
    {
        crypto_memcpy(pstHashInfo->stMMZBuffer.pu8StartVirAddr, pstHashInfo->u8LastBlock, pstHashInfo->u8LastBlockSize);
        pstHashInfo->stMMZBuffer.u32Size = pstHashInfo->u8LastBlockSize;

        /* save tail data */
        crypto_memset(pstHashInfo->u8LastBlock, 0, HASH_BLOCK_SIZE);
        pstHashInfo->u8LastBlockSize = (u32InputDataLen + pstHashInfo->stMMZBuffer.u32Size) % HASH_BLOCK_SIZE;
        ret = HashUpdate_Copy_InputData(pstHashInfo->u8LastBlock, pu8InputData + (u32InputDataLen - pstHashInfo->u8LastBlockSize),
            pstHashInfo->u8LastBlockSize, srcFrom);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Hash update copy input datafailed!\n");
            pstHashInfo->bIsUsed = HI_FALSE;
            crypto_mutex_unlock(&g_HashMutexKernel);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

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
        ret = HashUpdate_Copy_InputData((HI_U8 *)pstHashInfo->stMMZBuffer.pu8StartVirAddr + pstHashInfo->stMMZBuffer.u32Size,
               pu8InputData + u32BufInUsedLen, u32CopySize, srcFrom);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Hash update copy input datafailed!\n");
            pstHashInfo->bIsUsed = HI_FALSE;
            crypto_mutex_unlock(&g_HashMutexKernel);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        pstHashInfo->stMMZBuffer.u32Size += u32CopySize;
        u32BufInUsedLen+=u32CopySize;
        stHashData.enShaType = pstHashInfo->enShaType;
        stHashData.u32DataLen = pstHashInfo->stMMZBuffer.u32Size;
        stHashData.u32DataPhy = pstHashInfo->stMMZBuffer.u32StartPhyAddr;
        crypto_memcpy(stHashData.u32ShaVal, pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
        pstHashInfo->stMMZBuffer.u32Size = 0;

        stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_NONE;

        ret = HI_DRV_CIPHER_CalcHashUpdate(&stHashData);
        if(ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("Error, ioctl for hash update failed!\n");
            pstHashInfo->bIsUsed = HI_FALSE;
            crypto_mutex_unlock(&g_HashMutexKernel);
            return ret;
        }
        crypto_memcpy(pstHashInfo->u32ShaVal, stHashData.u32ShaVal, pstHashInfo->u32ShaLen);
    }
    if((u32BufInUsedLen + pstHashInfo->u8LastBlockSize) != u32InputDataLen)
    {
        HI_ERR_CIPHER("Error, Invalid data size: 0x%x!\n", u32BufInUsedLen);
        pstHashInfo->bIsUsed = HI_FALSE;
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_FAILURE;
    }

	crypto_mutex_unlock(&g_HashMutexKernel);

    return HI_SUCCESS;
}

static HI_S32 Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

#ifdef CIPHER_MHASH_SUPPORT
    ret = HAL_Cipher_CalcHashInit(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Hash_Init failed! ret = 0x%08x\n", ret);
        return ret;
    }
    ret = HAL_Cipher_CalcHashUpdate(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Hash_Update failed! ret = 0x%08x\n", ret);
        return ret;
    }
    ret = HAL_Cipher_CalcHashFinal(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Hash_Final failed! ret = 0x%08x\n", ret);
        return ret;
    }
#else
    ret = HAL_Cipher_CalcHashUpdate(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher hash update failed!\n");
        return ret;
    }
#endif
    return HI_SUCCESS;

}

static HI_S32 Cipher_HashFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8OutputHash, HI_U32 *pu32OutputHashLen)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_HASH_DATA_S stHashData;
    HI_U32 u32Tmp = 0;
    HI_U32 u32SoftHash = 0x0;
    HASH_INFO_S *pstHashInfo = HI_NULL;

    Hash_CheckHandle(CIHashHandle);
    u32SoftHash = HI_HANDLE_GET_CHNID(CIHashHandle);
    pstHashInfo = &g_stCipherHashData[u32SoftHash];

    if ((NULL == pu8OutputHash) || (NULL == pu32OutputHashLen))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");

        return HI_FAILURE;
    }

    if(crypto_mutex_lock_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("crypto_mutex_lock_interruptible failed!\n");
        return HI_FAILURE;
    }

    crypto_memcpy(pstHashInfo->stMMZBuffer.pu8StartVirAddr, pstHashInfo->u8LastBlock, pstHashInfo->u8LastBlockSize);
    pstHashInfo->stMMZBuffer.u32Size = pstHashInfo->u8LastBlockSize;
    crypto_memset(pstHashInfo->u8LastBlock, 0, HASH_BLOCK_SIZE);
    pstHashInfo->u8LastBlockSize = 0;

    u32Tmp = HashMsgPadding(pstHashInfo->stMMZBuffer.pu8StartVirAddr, pstHashInfo->stMMZBuffer.u32Size, pstHashInfo->u32TotalDataLen);

    //HI_PRINT_HEX("SHA", (HI_U8*)pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
    //HI_PRINT_HEX("DATA", pstHashInfo->stMMZBuffer.pu8StartVirAddr, u32Tmp);

    stHashData.enShaType = pstHashInfo->enShaType;
    stHashData.u32DataLen = u32Tmp;
    stHashData.u32DataPhy = pstHashInfo->stMMZBuffer.u32StartPhyAddr;
    crypto_memcpy(stHashData.u32ShaVal, pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);

    stHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_NONE;

    ret = HI_DRV_CIPHER_CalcHashUpdate(&stHashData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash final failed!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        crypto_mutex_unlock(&g_HashMutexKernel);
        return ret;
    }

    crypto_memcpy(pu8OutputHash, stHashData.u32ShaVal, pstHashInfo->u32ShaLen);
    *pu32OutputHashLen = pstHashInfo->u32ShaLen;

    pstHashInfo->bIsUsed = HI_FALSE;

	crypto_mutex_unlock(&g_HashMutexKernel);

    return HI_SUCCESS;
}

HI_S32 Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    ret = Cipher_CalcHashUpdate(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher hash final failed!\n");
        return ret;
    }

#ifndef CIPHER_MHASH_SUPPORT
    ret = HAL_Cipher_CalcHashFinal(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher hash final failed!\n");
        return ret;
    }
#endif

    return ret;
}

static HI_S32 Cipher_HmacKeyInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_U8 au8Hmackey[HASH_BLOCK_SIZE])
{
    HI_S32 ret = HI_FAILURE;
    HI_HANDLE hHash = 0;
    HI_U32 u32HmackeyLen = 0;

    if((pstHashAttr == NULL)||(pstHashAttr->pu8HMACKey == NULL)||(pstHashAttr->u32HMACKeyLen == 0))
    {
        HI_ERR_CIPHER("Invalid parameters.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*key length is less than 64bytes, copy directly*/
    if(pstHashAttr->u32HMACKeyLen <= HASH_BLOCK_SIZE)
    {
        ret = copy_from_user(au8Hmackey, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Copy hmac key from user failed.\n");
            return ret;
        }
        return HI_SUCCESS;
    }

    /*key length more than 64bytes, calcute the hash result*/
    ret = Cipher_HashInit(pstHashAttr, &hHash);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashInit failure, ret=%d\n", ret);
        return ret;
    }

    Hash_CheckHandle(hHash);
    ret = Cipher_HashUpdate(hHash, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen, HASH_CHUNCK_SRC_USER);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashUpdate failure, ret=0x%x\n", ret);
        return ret;
    }

    ret = Cipher_HashFinal(hHash,au8Hmackey, &u32HmackeyLen);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashFinal failure, ret=%d\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}

static HI_S32 Cipher_HmacInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_FAILURE;
    HI_U8 key_pad[HASH_BLOCK_SIZE];
    HASH_INFO_S *pstHashInfo = HI_NULL;
    HI_U32 i = 0;
    HI_U32 softHashHandle = 0x0;

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT)
     || (NULL == pstHashAttr->pu8HMACKey)
     || (0 == pstHashAttr->u32HMACKeyLen))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* Init hmac key */
    crypto_memset(key_pad, 0, sizeof(key_pad));
    ret = Cipher_HmacKeyInit(pstHashAttr, key_pad);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Hmac key initial failed!\n");
        return ret;
    }

    /* hash i_key_pad and message start */
    ret = Cipher_HashInit(pstHashAttr, pHashHandle);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("hash i_key_pad and message start failed!\n");
        return ret;
    }

    Hash_CheckHandle(*pHashHandle);
    softHashHandle = HI_HANDLE_GET_CHNID(*pHashHandle);
    pstHashInfo = (HASH_INFO_S *)&g_stCipherHashData[softHashHandle];
    crypto_memcpy(pstHashInfo->u8Mac, key_pad, HASH_BLOCK_SIZE);

    /* generate i_key_pad */
    for(i=0; i < HASH_BLOCK_SIZE; i++)
    {
        key_pad[i] ^= 0x36;
    }

    /* hash i_key_pad update */
    ret = Cipher_HashUpdate(*pHashHandle, key_pad, HASH_BLOCK_SIZE, HASH_CHUNCK_SRC_LOCAL);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("hash i_key_pad and message update failed!\n");
        return ret;
    }

    return HI_SUCCESS;
}

static HI_S32 Cipher_HmacUpdate(HI_HANDLE hHashHandle,
                                HI_U8 *pu8InputData, HI_U32 u32InputDataLen, HASH_CHUNK_SRC_E srcFrom)
{
    HI_S32 ret = HI_FAILURE;

    ret = Cipher_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen, srcFrom);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("hmac message update failed!\n");
        return ret;
    }

    return HI_SUCCESS;
}

static HI_S32 Cipher_HmacFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8Output, HI_U32 *pu32OutputHashLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hHash = HI_INVALID_HANDLE;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_U8 hash_sum_1[HASH256_SIGNATURE_SIZE] = {0};
    HI_U32 u32HashSuLen = 0;
    HI_U8 key_pad[HASH_BLOCK_SIZE] = {0};
    HI_U32 i;
    HI_U32 u32SoftHash = 0x0;
    HASH_INFO_S *pstHashInfo = HI_NULL;

    if ((NULL == pu8Output) || (NULL == pu32OutputHashLen))
    {
        HI_ERR_CIPHER("Invalid param,pu8Output is null, hmac final failed!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    Hash_CheckHandle(CIHashHandle);
    u32SoftHash = HI_HANDLE_GET_CHNID(CIHashHandle);
    pstHashInfo = &g_stCipherHashData[u32SoftHash];

    crypto_memset(&stHashAttr, 0, sizeof(stHashAttr));
    stHashAttr.eShaType = pstHashInfo->enShaType;
    crypto_memcpy(key_pad, pstHashInfo->u8Mac, HASH_BLOCK_SIZE);

    /* hash i_key_pad+message finished */
    ret = Cipher_HashFinal(CIHashHandle, hash_sum_1, &u32HashSuLen);
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
    ret = Cipher_HashInit(&stHashAttr, &hHash);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Init o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    Hash_CheckHandle(hHash);

    /* hash o_key_pad */
    ret = Cipher_HashUpdate(hHash, key_pad, HASH_BLOCK_SIZE, HASH_CHUNCK_SRC_LOCAL);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update o_key_pad failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash hash_sum_1 */
    if ( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == stHashAttr.eShaType )
    {
        ret = Cipher_HashUpdate(hHash, (HI_U8 *)hash_sum_1, HASH1_SIGNATURE_SIZE, HASH_CHUNCK_SRC_LOCAL);
    }
    else
    {
        ret = Cipher_HashUpdate(hHash, (HI_U8 *)hash_sum_1, HASH256_SIGNATURE_SIZE, HASH_CHUNCK_SRC_LOCAL);
    }

    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash o_key_pad+hash_sum_1 finished */
    ret = Cipher_HashFinal(hHash, pu8Output, pu32OutputHashLen);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_CIPHER_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;

    ret = Cipher_CalcHashInit(pCipherHashData);

    return ret;
}

#ifdef CIPHER_HASHVIR_SUPPORT
HI_S32 HI_DRV_CIPHER_CalcHashMMZInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;
    if((NULL == pCipherHashData) || (NULL == pCipherHashData->pu8Input))
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    pCipherHashData->u32DataPhy = g_stCipherComm.stTempCalcHashBuf.u32StartPhyAddr;
    if((0 == pCipherHashData->u32DataPhy)
        || (NULL == g_stCipherComm.stTempCalcHashBuf.pu8StartVirAddr))
    {
        HI_ERR_CIPHER("Error, Get kzalloc buf failed!\n");
        return HI_FAILURE;
    }

    if (pCipherHashData->u32DataLen > g_stCipherComm.stTempCalcHashBuf.u32Size)
    {
        HI_ERR_CIPHER("Error, input data length is too large!\n");
        return HI_FAILURE;
    }

    ret = copy_from_user(g_stCipherComm.stTempCalcHashBuf.pu8StartVirAddr,pCipherHashData->pu8Input, pCipherHashData->u32DataLen);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Calc Hash copy input data from user failed!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}
#endif

HI_S32 HI_DRV_CIPHER_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = Cipher_CalcHashUpdate(pCipherHashData);

    return s32Ret;
}

HI_S32 HI_DRV_CIPHER_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = Cipher_CalcHashFinal(pCipherHashData);

    return s32Ret;
}

HI_S32 Cipher_CalcHash(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 i;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }
    if( ((pCipherHashData->u32DataLen % 64) != 0) || (pCipherHashData->u32DataLen == 0))
    {
        HI_ERR_CIPHER("Error, Invalid input data len 0x%x!\n", pCipherHashData->u32DataLen);
        return HI_FAILURE;
    }
    if ((pCipherHashData->u32DataPhy % 4) != 0)
    {
        HI_ERR_CIPHER("Error, Invalid input data phy addr 0x%x!\n", pCipherHashData->u32DataPhy);
        return HI_FAILURE;
    }
    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY, HI_TRUE);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* write total len low and high */
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pCipherHashData->u32DataLen);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 0;
    if( HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType )
    {
		/* unCipherSHACtrl.bits.hardkey_hmac_flag = 1; */
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }
    unCipherSHACtrl.bits.small_end_en = 1;
    unCipherSHACtrl.bits.sha_init_update_en = 0;

    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);

    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);

    /*Update*/
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_START_ADDR, pCipherHashData->u32DataPhy);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_LEN, pCipherHashData->u32DataLen);

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY, HI_TRUE);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAStatus.u32);

    if( (0x00 == unCipherSHAStatus.bits.error_state) && (0x00 == unCipherSHAStatus.bits.len_err))
    {
        if( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType))
        {

            for(i = 0; i < 5; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_SHA_OUT1+i*4, &pCipherHashData->u32ShaVal[i]);
            }
        }
        else if( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
              || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        {
            for(i = 0; i < 8; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_SHA_OUT1+i*4, &pCipherHashData->u32ShaVal[i]);
    		}
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\n", unCipherSHAStatus.bits.error_state);
        HI_ERR_CIPHER("Error! SHA Status Reg: len_err = %d!\n", unCipherSHAStatus.bits.len_err);
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_CIPHER_CalcHash(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_FAILURE;
    if (NULL == pCipherHashData)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(crypto_mutex_lock_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("crypto_mutex_lock_interruptible failed!\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == pCipherHashData->bIsPhyAddr)
    {
        HI_INFO_CIPHER("Calc hash with phy addr.\n");
    }
#ifdef CIPHER_HASHVIR_SUPPORT
    else if (HI_FALSE == pCipherHashData->bIsPhyAddr)
    {
        ret = HI_DRV_CIPHER_CalcHashMMZInit(pCipherHashData);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Calc Hash mmz init failed!\n");
            crypto_mutex_unlock(&g_HashMutexKernel);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
#endif
    else
    {
        HI_ERR_CIPHER("Cala hash with invalid hash addr!\n");
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    ret = Cipher_CalcHash(pCipherHashData);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher calc hash failed!\n");
        crypto_mutex_unlock(&g_HashMutexKernel);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    crypto_mutex_unlock(&g_HashMutexKernel);
	return HI_SUCCESS;
}

HI_S32 CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType) )
    {
        return Cipher_HashInit(pstHashAttr, pHashHandle);
    }
    else if ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
    {
        return Cipher_HmacInit(pstHashAttr, pHashHandle);
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    return HI_FAILURE;
}

HI_S32 CIPHER_HashUpdate(HI_HANDLE CIHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HASH_INFO_S *pstHashInfo = HI_NULL;
    HI_U32 u32SoftHash = 0x0;

    if (NULL == pu8InputData)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    Hash_CheckHandle(CIHashHandle);
    u32SoftHash = HI_HANDLE_GET_CHNID(CIHashHandle);
    pstHashInfo = (HASH_INFO_S*)&g_stCipherHashData[u32SoftHash];

    if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashInfo->enShaType ) )
    {
        return Cipher_HashUpdate(CIHashHandle, pu8InputData, u32InputDataLen, HASH_CHUNCK_SRC_USER);
    }
    else if ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType))
    {
        return Cipher_HmacUpdate(CIHashHandle, pu8InputData, u32InputDataLen, HASH_CHUNCK_SRC_USER);
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_FAILURE;
}

HI_S32 CIPHER_HashFinal(HI_HANDLE CIHashHandle, HI_U8 *pu8OutputHash)
{
    HASH_INFO_S *pstHashInfo = HI_NULL;
    HI_U32 u32SoftHash = 0x0;
    HI_U8 u8OutputHash[HASH_MAX_RESULT] = {0};
    HI_U32 u32OutputHashLen = 0;
    HI_S32 ret = HI_FAILURE;

    if (NULL == pu8OutputHash)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    Hash_CheckHandle(CIHashHandle);
    u32SoftHash = HI_HANDLE_GET_CHNID(CIHashHandle);

    pstHashInfo = (HASH_INFO_S*)&g_stCipherHashData[u32SoftHash];
    if ((HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashInfo->enShaType) )
    {
        ret = Cipher_HashFinal(CIHashHandle, u8OutputHash, &u32OutputHashLen);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Cipher hash final failed!\n");
            return ret;
        }
    }
    else if ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType))
    {
        ret = Cipher_HmacFinal(CIHashHandle, u8OutputHash, &u32OutputHashLen);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Cipher hmac final failed!\n");
            return ret;
        }
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    ret = copy_to_user(pu8OutputHash, u8OutputHash, u32OutputHashLen);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Hash final output copy to user failed!\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_VOID HASH_DRV_ModInit(HI_VOID)
{
    crypto_mutex_init(&g_HashMutexKernel);
}

HI_VOID HASH_DRV_ModDeInit(HI_VOID)
{
    crypto_memset(&g_HashMutexKernel, 0, sizeof(g_HashMutexKernel));
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
