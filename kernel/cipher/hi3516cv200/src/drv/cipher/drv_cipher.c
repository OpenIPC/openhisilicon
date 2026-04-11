/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <asm/setup.h>
#include <asm/barrier.h>    /* mb() */
#include <asm/uaccess.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/param.h>
#include <linux/delay.h>

#include <linux/init.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/random.h>
#include <linux/signal.h>
#include <linux/seq_file.h>

#include "hi_type.h"
#include "drv_cipher_define.h"
#include "drv_cipher_ioctl.h"
#include "hal_cipher.h"
#include "drv_cipher.h"
#include "drv_hash.h"
#include "hal_efuse.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
DECLARE_MUTEX(g_CipherMutexKernel);
DECLARE_MUTEX(g_RsaMutexKernel);
#else
DEFINE_SEMAPHORE(g_CipherMutexKernel);
DEFINE_SEMAPHORE(g_RsaMutexKernel);
#endif

#define CI_BUF_LIST_SetIVFlag(u32Flags)
#define CI_BUF_LIST_SetEndFlag(u32Flags)

#define AES_BLOCK_SIZE        (16)
#define CIPHER_TEMP_MMZ_SIZE  (AES_BLOCK_SIZE*32)//512
#define CIPHER_MXA_PKG_SIZE   (0x100000 - 16)
#ifndef ALIGN
#define ALIGN(x,a)            (((x)+(a)-1)&~(a-1))
#endif
#define ALIGN16(val)          ALIGN(val, 16)
#define CIPHER_TIME_OUT       10000

typedef struct hiCIPHER_IV_VALUE_S
{
    HI_U32    u32PhyAddr;
    HI_U32   *pu32VirAddr;
    //HI_U8   au8IVValue[CI_IV_SIZE];
} CIPHER_IV_VALUE_S;


/*
-----------------------------------------------------------
0 | input buf list Node(16Byte) | ...  * CIPHER_MAX_LIST_NUM  | = 16*CIPHER_MAX_LIST_NUM
-----------------------------------------------------------
  | output buf list Node(16Byte)| ...  * CIPHER_MAX_LIST_NUM  |
-----------------------------------------------------------
  | IV (16Byte)                 | ...  * CIPHER_MAX_LIST_NUM  |
-----------------------------------------------------------
... * 7 Channels


*/

typedef struct hiCIPHER_PKGN_MNG_S
{
    HI_U32              u32TotalPkg;  /*  */
    HI_U32              u32Head;
    HI_U32              u32Tail;
} CIPHER_PKGN_MNG_S;

typedef struct hiCIPHER_PKG1_MNG_S
{
    HI_U32              au32Data[4];
} CIPHER_PKG1_MNG_S;

typedef union hiCIPHER_DATA_MNG_U
{
    CIPHER_PKGN_MNG_S  stPkgNMng;
    CIPHER_PKG1_MNG_S  stPkg1Mng;
}CIPHER_DATA_MNG_U;

typedef struct hiCIPHER_CHAN_S
{
    HI_U32                  chnId;
    CI_BUF_LIST_ENTRY_S     *pstInBuf;
    CI_BUF_LIST_ENTRY_S     *pstOutBuf;
    CIPHER_IV_VALUE_S       astCipherIVValue[CIPHER_MAX_LIST_NUM];
    HI_U32                  au32WitchSoftChn[CIPHER_MAX_LIST_NUM];
    HI_U32                  au32CallBackArg[CIPHER_MAX_LIST_NUM];
    HI_BOOL                 bNeedCallback[CIPHER_MAX_LIST_NUM];
    CIPHER_DATA_MNG_U       unInData;
    CIPHER_DATA_MNG_U       unOutData;
} CIPHER_CHAN_S;

typedef struct hiCIPHER_CCM_STATUS_S
{
    HI_U32  u32S0[4];
    HI_U32  u32CTRm[4];
    HI_U32  u32Yi[4];
    HI_U32 u32Plen;
}CIPHER_CCM_STATUS_S;

typedef struct hiCIPHER_GCM_STATUS_S
{
    HI_U32 u32Plen;
}CIPHER_GCM_STATUS_S;

typedef struct hiCIPHER_SOFTCHAN_S
{
    HI_BOOL               bOpen;
    HI_U32                u32HardWareChn;

    HI_UNF_CIPHER_CTRL_S  stCtrl;

    HI_BOOL               bIVChange;
    HI_BOOL               bKeyChange;
    HI_U32                u32LastPkg;     /* save which pkg's IV we should use for next pkg */
    HI_BOOL               bDecrypt;       /* hi_false: encrypt */

    HI_U32                u32PrivateData;
    funcCipherCallback    pfnCallBack;
    MMZ_BUFFER_S stMmzTemp;
    CIPHER_CCM_STATUS_S stCcmStatus;
    CIPHER_GCM_STATUS_S stGcmStatus;
} CIPHER_SOFTCHAN_S;

/********************** Global Variable declaration **************************/
extern HI_U32 g_u32CipherStartCase;
extern HI_U32 g_u32CipherEndCase;

CIPHER_COMM_S g_stCipherComm;
CIPHER_CHAN_S g_stCipherChans[CIPHER_CHAN_NUM];
CIPHER_SOFTCHAN_S g_stCipherSoftChans[CIPHER_SOFT_CHAN_NUM];
CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];

int cipher_irq = -1;

#define CIPHER_CheckHandle(CIHandle)   \
do \
{ \
    if((HI_ID_CIPHER != HI_HANDLE_GET_MODID(CIHandle)) \
        || (0 != HI_HANDLE_GET_PriDATA(CIHandle))) \
    { \
        HI_ERR_CIPHER("Invalid ChanId!\n"); \
        up(&g_CipherMutexKernel); \
        return HI_ERR_CIPHER_INVALID_PARA; \
    } \
    if (HI_HANDLE_GET_CHNID(CIHandle) >= CIPHER_SOFT_CHAN_NUM) \
    { \
        HI_ERR_CIPHER("chan %d is too large, max: %d\n", HI_HANDLE_GET_CHNID(CIHandle), CIPHER_SOFT_CHAN_NUM); \
        up(&g_CipherMutexKernel);   \
        return HI_ERR_CIPHER_INVALID_PARA; \
    } \
    if (HI_FALSE == g_stCipherOsrChn[HI_HANDLE_GET_CHNID(CIHandle)].g_bSoftChnOpen) \
    { \
        HI_ERR_CIPHER("chan %d is not open\n", HI_HANDLE_GET_CHNID(CIHandle)); \
        up(&g_CipherMutexKernel);    \
        return HI_ERR_CIPHER_INVALID_HANDLE; \
    } \
} while (0)

HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg)
{
    HI_INFO_CIPHER("arg=%#x.\n", arg);

    g_stCipherOsrChn[arg].g_bDataDone = HI_TRUE;
    wake_up_interruptible(&(g_stCipherOsrChn[arg].cipher_wait_queue));

    return ;
}

HI_S32 DRV_CIPHER_ReadReg(HI_U32 addr, HI_U32 *pVal)
{
    if ( NULL == pVal )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    (HI_VOID)HAL_CIPHER_ReadReg(addr, pVal);

    return HI_SUCCESS;;
}

HI_S32 DRV_CIPHER_WriteReg(HI_U32 addr, HI_U32 Val)
{
    (HI_VOID)HAL_CIPHER_WriteReg(addr, Val);
    return HI_SUCCESS;
}

HI_S32 DRV_CipherInitHardWareChn(HI_U32 chnId )
{
    HI_U32        i;
    CIPHER_CHAN_S *pChan;

    pChan = &g_stCipherChans[chnId];

    HAL_Cipher_SetInBufNum(chnId, CIPHER_MAX_LIST_NUM);
    HAL_Cipher_SetInBufCnt(chnId, 0);

    HAL_Cipher_SetOutBufNum(chnId, CIPHER_MAX_LIST_NUM);
    HAL_Cipher_SetOutBufCnt(chnId, CIPHER_MAX_LIST_NUM);

    HAL_Cipher_SetAGEThreshold(chnId, CIPHER_INT_TYPE_OUT_BUF, 0);
    HAL_Cipher_SetAGEThreshold(chnId, CIPHER_INT_TYPE_IN_BUF, 0);

    HAL_Cipher_DisableInt(chnId, CIPHER_INT_TYPE_OUT_BUF | CIPHER_INT_TYPE_IN_BUF);

    for (i = 0; i < CIPHER_MAX_LIST_NUM; i++)
    {
        ;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CipherDeInitHardWareChn(HI_U32 chnId)
{
    HAL_Cipher_DisableInt(chnId, CIPHER_INT_TYPE_OUT_BUF | CIPHER_INT_TYPE_IN_BUF);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_CIPHER_GetRandomNumber(CIPHER_RNG_S *pstRNG)
{
	HI_S32 ret = HI_FAILURE;

    if(NULL == pstRNG)
    {
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_FAILURE;
    }

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    ret = HAL_Cipher_GetRandomNumber(pstRNG);

	up(&g_CipherMutexKernel);
	return ret;
}

/*
set interrupt threshold level and enable it, and flag soft channel opened
*/
HI_S32 DRV_CIPHER_OpenChn(HI_U32 softChnId)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;

    pSoftChan = &g_stCipherSoftChans[softChnId];
    pSoftChan->u32HardWareChn = softChnId;

    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];

    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_OUT_BUF, CIPHER_DEFAULT_INT_NUM);
    ret = HAL_Cipher_EnableInt(pChan->chnId, CIPHER_INT_TYPE_OUT_BUF);
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_TRUE;

    pSoftChan->bOpen = HI_TRUE;

    return ret;
}

HI_S32 DRV_CIPHER_CloseChn(HI_U32 softChnId)
{
    g_stCipherSoftChans[softChnId].bOpen = HI_FALSE;

    return HI_SUCCESS;
}

#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 DRV_CIPHER_CCM_Init(HI_U32 u32SoftChanId)
{
    HI_S32 Ret;
    MMZ_BUFFER_S *pstMmzTemp;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;
    HI_UNF_CIPHER_CCM_INFO_S *pstCCM;
    HI_U32 u32Index = 0;
    HI_U8 *pu8Buf = 0;
    CIPHER_SOFTCHAN_S *pSoftChan;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstMmzTemp = &pSoftChan->stMmzTemp;
    pstCtrl = &pSoftChan->stCtrl;
    pu8Buf = (HI_U8*)pstMmzTemp->pu8StartVirAddr;
    pstCCM = &pstCtrl->unModeInfo.stCCM;

    if ((pstCCM->u8NLen < 7) || (pstCCM->u8NLen > 13))
    {
        HI_ERR_CIPHER("Invalid Nlen: 0x%x!\n", pstCCM->u8NLen);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(&pSoftChan->stCcmStatus, 0, sizeof(CIPHER_CCM_STATUS_S));
    pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    memset(pstCtrl->u32IV, 0x00, CI_IV_SIZE);
    memset(pu8Buf, 0, pstMmzTemp->u32Size);
    pu8Buf[u32Index]  = (pstCCM->u32ALen > 0 ? 1 : 0) << 6;//Adata
    pu8Buf[u32Index] |= ((pstCCM->u8TLen - 2)/2) << 3;// ( t -2)/2
    pu8Buf[u32Index] |= ((15 - pstCCM->u8NLen) - 1);// q-1, n+q=15
    u32Index++;
    memcpy(&pu8Buf[u32Index], pstCCM->u8Nonce, pstCCM->u8NLen);
    u32Index+=pstCCM->u8NLen;
    if(u32Index <= 12)
    {
        u32Index = 12; //in fact, mlen <= 2^32, so skip to the last 4 bytes fo Q
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen >> 24);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen >> 16);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen >> 8);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen);
    }
    else if ((u32Index == 13) && (pstCCM->u32MLen <= 0xFFFFFF))
    {
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen >> 16);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen >> 8);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen);
    }
    else if ((u32Index == 14) && (pstCCM->u32MLen <= 0xFFFF))
    {
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen >> 8);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32MLen);
    }
    else
    {
        HI_ERR_CIPHER("Invalid Mlen: 0x%x, q: 0x%x!\n", pstCCM->u32MLen, 16 - u32Index);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
//    HI_PRINT_HEX ("B0", pu8Buf, 16);
    pSoftChan->bIVChange = HI_TRUE;
    Ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstMmzTemp->u32StartPhyAddr,
        pstMmzTemp->u32StartPhyAddr, AES_BLOCK_SIZE, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Cipher encrypt failed!\n");
        return Ret;
    }
    HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
    memcpy((HI_U8*)pSoftChan->stCcmStatus.u32Yi, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
//    HI_PRINT_HEX ("Y0", (HI_U8*)pSoftChan->stCcmStatus.u32Yi, 16);

    pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CCM;
    pu8Buf = (HI_U8*)pstCtrl->u32IV;
    memset(pu8Buf, 0x00, CI_IV_SIZE);
    *pu8Buf = (15 - pstCCM->u8NLen) - 1;
    memcpy(pu8Buf+1, pstCCM->u8Nonce, pstCCM->u8NLen);
    pSoftChan->bIVChange = HI_TRUE;
    memset((void*)pstMmzTemp->pu8StartVirAddr, 0, AES_BLOCK_SIZE);
    Ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstMmzTemp->u32StartPhyAddr,
        pstMmzTemp->u32StartPhyAddr, AES_BLOCK_SIZE, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Cipher encrypt failed!\n");
        return Ret;
    }
    memcpy((HI_U8*)pSoftChan->stCcmStatus.u32S0, (HI_U8*)pstMmzTemp->pu8StartVirAddr, AES_BLOCK_SIZE);
    HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
    memcpy((HI_U8*)pSoftChan->stCcmStatus.u32CTRm, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
    pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CCM;

    return HI_SUCCESS;

}

HI_S32 DRV_CIPHER_GCM_Init(HI_U32 u32SoftChanId)
{
    HI_S32 Ret;
    CIPHER_SOFTCHAN_S *pSoftChan;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstCtrl = &pSoftChan->stCtrl;
    memset(&pSoftChan->stGcmStatus, 0, sizeof(CIPHER_GCM_STATUS_S));

    if ((pstCtrl->unModeInfo.stGCM.u32IVLen < 1)||(pstCtrl->unModeInfo.stGCM.u32IVLen > 16))
    {
         HI_ERR_CIPHER("Invalid IVlen: 0x%x, must not large than 16!\n", pstCtrl->unModeInfo.stGCM.u32IVLen);
         return HI_ERR_CIPHER_INVALID_PARA;
    }
    if ((pstCtrl->unModeInfo.stGCM.u32MLen + pstCtrl->unModeInfo.stGCM.u32ALen) == 0)
    {
         HI_ERR_CIPHER("Invalid u32MLen + u32ALen: 0, must be large than 0!\n");
         return HI_ERR_CIPHER_INVALID_PARA;
    }

    Ret = HAL_Cipher_SetLen (u32SoftChanId, pstCtrl);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = HAL_Cipher_CleanTagVld(u32SoftChanId, pstCtrl);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return Ret;
}

HI_S32 DRV_CIPHER_CCM_Aad(HI_U32 u32SoftChanId)
{
    HI_S32 Ret;
    MMZ_BUFFER_S *pstMmzTemp;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;
    HI_UNF_CIPHER_CCM_INFO_S *pstCCM;
    HI_U32 u32Index = 0;
    HI_U32 u32CopySize = 0;
    HI_U32 u32TotalCopySize = 0;
    HI_U8 *pu8Buf = 0;
    CIPHER_SOFTCHAN_S *pSoftChan;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstMmzTemp = &pSoftChan->stMmzTemp;
    pstCtrl = &pSoftChan->stCtrl;
    pstCCM = &pstCtrl->unModeInfo.stCCM;
    pu8Buf = (HI_U8*)pstMmzTemp->pu8StartVirAddr;

    if (pstCCM->u32ALen == 0)
    {
        return HI_SUCCESS;
    }

    if (pstCCM->pu8Aad + pstCCM->u32ALen < pstCCM->u32ALen)
    {
        HI_ERR_CIPHER("Invalid A addr or A length!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)pSoftChan->stCcmStatus.u32Yi, CI_IV_SIZE);
    memset((HI_U8*)pstMmzTemp->pu8StartVirAddr, 0, pstMmzTemp->u32Size);
    pSoftChan->bIVChange = HI_TRUE;
    u32Index = 0;

    if (pstCCM->u32ALen < (0x10000 - 0x100))
    {
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32ALen >> 8);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32ALen);
    }
    else
    {
        pu8Buf[u32Index++] = 0xFF;
        pu8Buf[u32Index++] = 0xFE;
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32ALen >> 24);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32ALen >> 16);
        pu8Buf[u32Index++] = (HI_U8)(pstCCM->u32ALen >> 8);
        pu8Buf[u32Index++] = (HI_U8)pstCCM->u32ALen;
    }
    if (pstCCM->u32ALen > (pstMmzTemp->u32Size - u32Index))
    {
        u32CopySize = pstMmzTemp->u32Size - u32Index;
    }
    else
    {
        u32CopySize = pstCCM->u32ALen;
    }
    if(copy_from_user(&pu8Buf[u32Index], pstCCM->pu8Aad, u32CopySize))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }
    u32Index += u32CopySize;
    u32TotalCopySize = u32CopySize;
//    HI_PRINT_HEX ("B(A)", (HI_U8*)stAadMmzBuf.pu8StartVirAddr, ALIGN16(u32Index));
    Ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstMmzTemp->u32StartPhyAddr,
        pstMmzTemp->u32StartPhyAddr, ALIGN16(u32Index), HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Cipher encrypt failed!\n");
        return Ret;
    }

    while(u32TotalCopySize < pstCCM->u32ALen)
    {
        if ((pstCCM->u32ALen - u32TotalCopySize) > pstMmzTemp->u32Size)
        {
            u32CopySize = pstMmzTemp->u32Size;
        }
        else
        {
            u32CopySize = pstCCM->u32ALen - u32TotalCopySize;
            memset(pu8Buf, 0, pstMmzTemp->u32Size);
        }
        if(copy_from_user(pu8Buf, pstCCM->pu8Aad+u32TotalCopySize, u32CopySize))
        {
            HI_ERR_CIPHER("copy data from user fail!\n");
            return HI_FAILURE;
        }
        Ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstMmzTemp->u32StartPhyAddr,
            pstMmzTemp->u32StartPhyAddr, ALIGN16(u32CopySize), HI_FALSE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
        u32TotalCopySize += u32CopySize;
    }

    HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
    memcpy((HI_U8*)pSoftChan->stCcmStatus.u32Yi, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
//    HI_PRINT_HEX ("Y(A)", (HI_U8*)pSoftChan->stCcmStatus.u32Yi, CI_IV_SIZE);

    pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CCM;

    return HI_SUCCESS;
}

HI_S32 DRV_CIPHER_GCM_Aad(HI_U32 u32SoftChanId)
{
    HI_S32 Ret;
    HI_U32 fullCnt;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;
    CIPHER_SOFTCHAN_S *pSoftChan;
    CIPHER_CHAN_S *pChan = NULL;
    HI_UNF_CIPHER_GCM_INFO_S *pstGCM;
    MMZ_BUFFER_S *pstMmzTemp;
    HI_U32 u32CopySize = 0;
    HI_U32 u32TotalCopySize = 0;
    HI_U8 *pu8Buf = 0;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstMmzTemp = &pSoftChan->stMmzTemp;
    pstCtrl = &pSoftChan->stCtrl;
    pstGCM = &pstCtrl->unModeInfo.stGCM;
    pu8Buf = (HI_U8*)pstMmzTemp->pu8StartVirAddr;

    if (pstCtrl->unModeInfo.stGCM.u32ALen == 0)
    {
        return HI_SUCCESS;
    }

    Ret = HAL_Cipher_CleanAadEnd(u32SoftChanId, pstCtrl);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    if (pstGCM->pu8Aad + pstGCM->u32ALen < pstGCM->u32ALen)
    {
        HI_ERR_CIPHER("Invalid A addr or A lenth!\n");
        return Ret;
    }

    while(u32TotalCopySize < pstGCM->u32ALen)
    {
        if ((pstGCM->u32ALen - u32TotalCopySize) > pstMmzTemp->u32Size)
        {
            u32CopySize = pstMmzTemp->u32Size;
        }
        else
        {
            u32CopySize = pstGCM->u32ALen - u32TotalCopySize;
            memset(pu8Buf, 0, pstMmzTemp->u32Size);
        }
        if(copy_from_user(pu8Buf, pstGCM->pu8Aad+u32TotalCopySize, u32CopySize))
        {
            HI_ERR_CIPHER("copy data from user fail!\n");
            return HI_FAILURE;
        }
        Ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstMmzTemp->u32StartPhyAddr,
            HI_NULL, ALIGN16(u32CopySize), HI_FALSE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
        u32TotalCopySize += u32CopySize;
    }

    /*compute aad without Output data, interrupt did not occur*/
    Ret = HAL_Cipher_WaitAadEnd(u32SoftChanId, pstCtrl);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    /*process input queue*/
    pChan = &g_stCipherChans[u32SoftChanId];
    Ret = HAL_Cipher_GetInBufEmpty(u32SoftChanId, &fullCnt);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    HAL_Cipher_SetInBufEmpty(u32SoftChanId, fullCnt);
    pChan->unInData.stPkgNMng.u32Tail++;
    pChan->unInData.stPkgNMng.u32Tail %= CIPHER_MAX_LIST_NUM;

    return HI_SUCCESS;

}
#endif

HI_S32 DRV_CIPHER_ConfigChn(HI_U32 softChnId,  HI_UNF_CIPHER_CTRL_S *pConfig)
{
    HI_S32 ret = HI_FAILURE;
    HI_BOOL bDecrypt = HI_FALSE;
    HI_U32 hardWareChn;
    HI_BOOL bIVSet;
    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;

    if (HI_NULL == pConfig)
    {
    	HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    pSoftChan = &g_stCipherSoftChans[softChnId];
    hardWareChn = pSoftChan->u32HardWareChn;
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];
    pSoftChan->pfnCallBack = DRV_CIPHER_UserCommCallBack;
    bIVSet = (pConfig->stChangeFlags.bit1IV & 0x1) ? HI_TRUE : HI_FALSE;

    ret = HAL_Cipher_Config(pChan->chnId, bDecrypt, bIVSet, pConfig);

    pSoftChan->bIVChange = bIVSet;
    pSoftChan->bKeyChange = HI_TRUE;

    memcpy(&(pSoftChan->stCtrl), pConfig, sizeof(HI_UNF_CIPHER_CTRL_S));

    HAL_Cipher_GetSrcLstRaddr(softChnId, &pChan->unInData.stPkgNMng.u32Head);
    HAL_Cipher_GetDestLstRaddr(softChnId, &pChan->unOutData.stPkgNMng.u32Head);

    /* set Key */
    if (pSoftChan->bKeyChange)
    {
        ret = HAL_Cipher_SetKey(hardWareChn, &(pSoftChan->stCtrl));
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        pSoftChan->bKeyChange = HI_FALSE;
    }
    else
    {
        ;
    }

    switch(pSoftChan->stCtrl.enWorkMode)
    {
        case HI_UNF_CIPHER_WORK_MODE_ECB:
        case HI_UNF_CIPHER_WORK_MODE_CBC:
        case HI_UNF_CIPHER_WORK_MODE_CFB:
        case HI_UNF_CIPHER_WORK_MODE_OFB:
        case HI_UNF_CIPHER_WORK_MODE_CTR:
            break;
#ifdef CIPHER_CCM_GCM_SUPPORT
        case HI_UNF_CIPHER_WORK_MODE_CCM:
            ret = DRV_CIPHER_CCM_Init(softChnId);
            break;
        case HI_UNF_CIPHER_WORK_MODE_GCM:
            ret = DRV_CIPHER_GCM_Init(softChnId);
            break;
#endif
        default:
            HI_ERR_CIPHER("Cipher mode 0x%x unsupport!\n", pSoftChan->stCtrl.enWorkMode);
            return HI_FAILURE;
    }

    return ret;
}

HI_S32 HI_DRV_CIPHER_ConfigChn(HI_HANDLE CIHandle,  HI_UNF_CIPHER_CTRL_S *pConfig)
{
	HI_S32 ret = HI_FAILURE;
    HI_U32 u32IVSet = 0;
    HI_U32 softChnId = 0;

    if (HI_NULL == pConfig)
    {
    	HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

	if(down_interruptible(&g_CipherMutexKernel))
	{
		HI_ERR_CIPHER("down_interruptible failed!\n");
		return HI_FAILURE;
	}

    CIPHER_CheckHandle(CIHandle);
    softChnId = HI_HANDLE_GET_CHNID(CIHandle);

    u32IVSet = pConfig->stChangeFlags.bit1IV;
	ret = DRV_CIPHER_ConfigChn(softChnId, pConfig);
    if (ret != HI_SUCCESS)
    {
        up(&g_CipherMutexKernel);
        return ret;
    }

    switch(pConfig->enWorkMode)
    {
#ifdef CIPHER_CCM_GCM_SUPPORT
    case HI_UNF_CIPHER_WORK_MODE_CCM:
        ret = DRV_CIPHER_CCM_Aad(softChnId);
        break;
    case HI_UNF_CIPHER_WORK_MODE_GCM:
        if (u32IVSet)
        {
            ret = DRV_CIPHER_GCM_Aad(softChnId);
        }
        break;
#endif
    default:
        break;
    }

	up(&g_CipherMutexKernel);

	return ret;
}

/*
*/
HI_S32 DRV_CipherStartSinglePkgChn(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process)
{
    HI_U32 ret;
    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;
    pSoftChan = &g_stCipherSoftChans[softChnId];
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];
    HAL_Cipher_Config(0, pBuf2Process->bDecrypt, pSoftChan->bIVChange, &(pSoftChan->stCtrl));

    if (pSoftChan->bIVChange)
    {
        ret = HAL_Cipher_SetInIV(0, &(pSoftChan->stCtrl));
        if (HI_SUCCESS != ret)
        {
            return ret;
        }
        pSoftChan->bIVChange = HI_FALSE;
    }
    if (pSoftChan->bKeyChange)
    {
        ret = HAL_Cipher_SetKey(0, &(pSoftChan->stCtrl));
        if (HI_SUCCESS != ret)
        {
            return ret;
        }
        pSoftChan->bKeyChange = HI_FALSE;
    }

    HAL_Cipher_SetDataSinglePkg(pBuf2Process);
    HAL_Cipher_StartSinglePkg(pChan->chnId);
    ret = HAL_Cipher_WaitIdle();
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }
    HAL_Cipher_ReadDataSinglePkg(pBuf2Process->u32DataPkg);
    return HI_SUCCESS;
}

HI_S32 DRV_CipherStartMultiPkgChn(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 hardWareChn;
    HI_U32 BusyCnt = 0;
    HI_U32 currentInPtr;
    HI_U32 currentOutPtr;
    HI_U32 i;
    HI_U32 u32OutCnt = 0;
    HI_U32 u32InCnt = 0;
    CI_BUF_LIST_ENTRY_S *pInBuf;
    CI_BUF_LIST_ENTRY_S *pOutBuf;

    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;

    pSoftChan = &g_stCipherSoftChans[softChnId];
    hardWareChn = pSoftChan->u32HardWareChn;
    pChan = &g_stCipherChans[hardWareChn];

    HAL_Cipher_GetInBufCnt(hardWareChn, &BusyCnt);

    currentInPtr = pChan->unInData.stPkgNMng.u32Head;
    currentOutPtr = pChan->unOutData.stPkgNMng.u32Head;

    if ((pBuf2Process == HI_NULL) || (pkgNum== 0))
    {
         HI_ERR_CIPHER("Invalid param\n");
         return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (BusyCnt + pkgNum > CIPHER_MAX_LIST_NUM)
    {
         HI_ERR_CIPHER("pkg want to do: %u, free pkg num:%u.\n", pkgNum, CIPHER_MAX_LIST_NUM - BusyCnt);
         return HI_ERR_CIPHER_BUSY;
    }

    /* set Key */
    if (pSoftChan->bKeyChange)
    {
        ret = HAL_Cipher_SetKey(hardWareChn, &(pSoftChan->stCtrl));
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        pSoftChan->bKeyChange = HI_FALSE;
    }

    ret = HAL_Cipher_Config(hardWareChn, pBuf2Process[0].bDecrypt, pSoftChan->bIVChange, &(pSoftChan->stCtrl));
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    for (i = 0; i < pkgNum; i++)
    {
        /*Config IN buf*/
        if (pBuf2Process[i].u32src > 0)
        {
 //         HI_PRINT("input:%d/%d: 0x%x, currentInPtr: %d\n", i, pkgNum, pBuf2Process[i].u32length, currentInPtr);
            pInBuf = pChan->pstInBuf + currentInPtr;
            pInBuf->u32DataAddr = pBuf2Process[i].u32src;
            pInBuf->U32DataLen = pBuf2Process[i].u32length;
            pInBuf->u32Flags = 0;

            if (pSoftChan->bIVChange)
            {
                memcpy(pChan->astCipherIVValue[currentInPtr].pu32VirAddr, pSoftChan->stCtrl.u32IV, CI_IV_SIZE);
                mb();
                pInBuf->u32IVStartAddr = pChan->astCipherIVValue[currentInPtr].u32PhyAddr;
                pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_IVSET_BIT);
            }

            if (isMultiIV)
            {
                pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_EOL_BIT);
            }
            else
            {
                pSoftChan->bIVChange = HI_FALSE;
            }

            u32InCnt++;
            currentInPtr = (currentInPtr + 1) % CIPHER_MAX_LIST_NUM;
            pChan->unInData.stPkgNMng.u32TotalPkg++;
        }

        /*Config OUT buf*/
        if (pBuf2Process[i].u32dest > 0)
        {
  //        HI_PRINT("output:%d/%d: 0x%x, currentOutPtr: %d\n", i, pkgNum, pBuf2Process[i].u32length, currentOutPtr);
            pOutBuf = pChan->pstOutBuf + currentOutPtr;
            pOutBuf->u32DataAddr = pBuf2Process[i].u32dest;
            pOutBuf->U32DataLen = pBuf2Process[i].u32length;
            pChan->au32WitchSoftChn[currentOutPtr] = softChnId;
            pChan->au32CallBackArg[currentOutPtr] = callBackArg;
            pSoftChan->u32PrivateData = callBackArg;
            pChan->bNeedCallback[currentOutPtr] = ((i + 1) == pkgNum ? HI_TRUE : HI_FALSE);
            pOutBuf->u32Flags = 0;

            pOutBuf->u32Flags &= ~(1 << CI_BUF_LIST_FLAG_EOL_BIT);
            pOutBuf->u32Flags |= (isMultiIV << CI_BUF_LIST_FLAG_EOL_BIT);

            currentOutPtr = (currentOutPtr + 1) % CIPHER_MAX_LIST_NUM;
            pChan->unOutData.stPkgNMng.u32TotalPkg++;
            u32OutCnt++;
        }

        HI_INFO_CIPHER("%s %#x->%#x, LEN:%#x\n", pBuf2Process[i].bDecrypt ? "Dec" : "ENC",
            pBuf2Process[i].u32src, pBuf2Process[i].u32dest,pBuf2Process[i].u32length );
    }

    pSoftChan->bIVChange = HI_FALSE;

    if (u32OutCnt > 0)
    {
        HAL_Cipher_SetIntThreshold(hardWareChn, CIPHER_INT_TYPE_OUT_BUF, u32OutCnt);
    }
    if (u32InCnt > 0)
    {
        HAL_Cipher_SetInBufCnt(hardWareChn, u32InCnt); /* +1 */
    }

    /* save list Node */
    pChan->unInData.stPkgNMng.u32Head = currentInPtr;
    pChan->unOutData.stPkgNMng.u32Head = currentOutPtr;
    pSoftChan->bIVChange = HI_FALSE;

    return ret;
}

HI_S32 DRV_CIPHER_CreatMultiPkgTask(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg)
{
    return DRV_CipherStartMultiPkgChn(softChnId, pBuf2Process, pkgNum, isMultiIV, callBackArg);
}

/*
*/
HI_S32 DRV_CIPHER_CreatTask(HI_U32 softChnId, HI_DRV_CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV)
{
    HI_S32 ret;
    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;

    pSoftChan = &g_stCipherSoftChans[softChnId];
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];

    if (pKey)
    {
        pSoftChan->bKeyChange = HI_TRUE;
        memcpy(pSoftChan->stCtrl.u32Key, pKey, CI_KEY_SIZE);
    }

    if (pIV)
    {
        pSoftChan->bIVChange = HI_TRUE;
        memcpy(pSoftChan->stCtrl.u32IV, pIV, CI_IV_SIZE);
    }

    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_OUT_BUF, 1);

    if (CIPHER_PKGx1_CHAN == pSoftChan->u32HardWareChn)
    {
        ret = DRV_CipherStartSinglePkgChn(softChnId, &(pTask->stData2Process));
    }
    else
    {
        ret = DRV_CipherStartMultiPkgChn(softChnId, &(pTask->stData2Process), 1, HI_TRUE, pTask->u32CallBackArg);
    }
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("can't create task, ERR=%#x.\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CipherDataDoneMultiPkg(HI_U32 chnId)
{
    HI_S32 ret;
    HI_U32 currentInPtr = 0;
    HI_U32 currentOutPtr = 0;
    HI_U32 softChnId = 0;
    HI_U32 fullCnt = 0;
    HI_U32 i;
    CIPHER_CHAN_S *pChan = NULL;
    CIPHER_SOFTCHAN_S *pSoftChan = NULL;
    CI_BUF_LIST_ENTRY_S *pInBuf = NULL;
    CI_BUF_LIST_ENTRY_S *pOutBuf = NULL;

    pChan = &g_stCipherChans[chnId];
    HI_DEBUG_CIPHER("Data DONE, hwChn:%d\n", chnId);

    ret = HAL_Cipher_GetInBufEmpty(chnId, &fullCnt);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    HAL_Cipher_SetInBufEmpty(chnId, fullCnt);  /* -  */
    currentInPtr = pChan->unInData.stPkgNMng.u32Tail;
    pChan->unInData.stPkgNMng.u32Tail =  (currentInPtr + fullCnt) % CIPHER_MAX_LIST_NUM;
    HI_DEBUG_CIPHER("Data DONE, In Cnt:%d\n", fullCnt);

    /* get the finished output data buffer count */
    ret = HAL_Cipher_GetOutBufFull(chnId, &fullCnt);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    currentOutPtr = pChan->unOutData.stPkgNMng.u32Tail;

    if(currentOutPtr >= CIPHER_MAX_LIST_NUM)
    {
        HI_ERR_CIPHER("idx error: idx=%u, chnId=%d \n", currentOutPtr, chnId);
        return HI_FAILURE;
    }

    HI_DEBUG_CIPHER("fullCnt:%d\n", fullCnt);
    if (fullCnt > 0) /* have list entry */
    {
        for (i = 0; i < fullCnt; i++)
        {
            softChnId = pChan->au32WitchSoftChn[currentOutPtr];
            pChan->au32WitchSoftChn[currentOutPtr] = CIPHER_INVALID_CHN;

            pSoftChan = &g_stCipherSoftChans[softChnId];
            pSoftChan->u32LastPkg = currentOutPtr;
            HI_INFO_CIPHER("softChnId=%d, %d-idx=%u, needCallback:%d\n", softChnId, fullCnt, currentOutPtr, pChan->bNeedCallback[currentOutPtr]);
            if (pSoftChan->pfnCallBack && pChan->bNeedCallback[currentOutPtr])
            {
                HI_DEBUG_CIPHER("CallBack function\n");
                pSoftChan->pfnCallBack(pSoftChan->u32PrivateData);
            }

            pInBuf = pChan->pstInBuf + currentOutPtr;
            pInBuf->u32Flags = 0;
            pOutBuf = pChan->pstOutBuf + currentOutPtr;
            pOutBuf->u32Flags = 0;

            currentOutPtr = (currentOutPtr + 1) % CIPHER_MAX_LIST_NUM;
        }

        pChan->unOutData.stPkgNMng.u32Tail = currentOutPtr;
        HAL_Cipher_SetOutBufFull(chnId, fullCnt);  /* -  */
        HAL_Cipher_SetOutBufCnt(chnId, fullCnt);   /* +  */
    }
    else
    {
        HI_U32 regValue = 0xabcd;

        HI_ERR_CIPHER("Data done, but fullCnt=0, chn%d\n", chnId);

        HAL_Cipher_GetIntState(&regValue);
        HI_ERR_CIPHER("INTSt:%#x\n", regValue);

        HAL_Cipher_GetIntEnState(&regValue);
        HI_ERR_CIPHER("INTEnSt:%#x\n", regValue);

        HAL_Cipher_GetRawIntState(&regValue);
        HI_ERR_CIPHER("INTRawSt:%#x\n", regValue);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* interrupt routine, callback */
irqreturn_t DRV_Cipher_ISR(HI_S32 irq, HI_VOID *devId)
{
    HI_U32 i;
    HI_U32 INTValue = 0;

    HAL_Cipher_GetIntState(&INTValue);
    HAL_Cipher_ClrIntState(INTValue);

    HI_INFO_CIPHER(" in the isr INTValue=%#x!\n", INTValue);

    for(i = 1; i < CIPHER_CHAN_NUM; i++)
    {
        if ((INTValue >> (i+8)) & 0x1)
        {
            DRV_CipherDataDoneMultiPkg(i);
        }
    }
//    HAL_Cipher_ClrIntState();
    return IRQ_HANDLED;
}


HI_S32 DRV_CIPHER_Init(HI_VOID)
{
    HI_U32 i,j, hwChnId;
    HI_S32 ret;
    HI_U32 bufSizeChn = 0; /* all the buffer list size, included data buffer size and IV buffer size */
    HI_U32 databufSizeChn = 0; /* max list number data buffer size */
    HI_U32 ivbufSizeChn = 0; /* all the list IV size */
    HI_U32 bufSizeTotal = 0; /* all the channel buffer size */
    MMZ_BUFFER_S   cipherListBuf;
    CIPHER_CHAN_S *pChan;

    memset(&g_stCipherComm, 0, sizeof(g_stCipherComm));
    memset(&g_stCipherChans, 0, sizeof(g_stCipherChans));
    memset(&g_stCipherSoftChans, 0, sizeof(g_stCipherSoftChans));

    ret = HI_DRV_MMZ_AllocAndMap("CIPHER_TEMP", NULL, 4096, 0, &g_stCipherComm.stTempPhyBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Error: new phyaddr for last block failed!\n");
        return HI_FAILURE;
    }
    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
        g_stCipherOsrChn[i].g_bDataDone = HI_FALSE;
        g_stCipherOsrChn[i].pWichFile = NULL;
        init_waitqueue_head(&(g_stCipherOsrChn[i].cipher_wait_queue));
        memcpy(&g_stCipherSoftChans[i].stMmzTemp, &g_stCipherComm.stTempPhyBuf, sizeof(MMZ_BUFFER_S));
    }

/*
==========================channel-1=============================
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |inBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |outBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |keyBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
=============================================================
...
...
...
==========================channel-7=============================
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |inBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |outBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |keyBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
=============================================================
*/

    databufSizeChn = sizeof(CI_BUF_LIST_ENTRY_S) * CIPHER_MAX_LIST_NUM;
    ivbufSizeChn = CI_IV_SIZE * CIPHER_MAX_LIST_NUM;
    bufSizeChn = (databufSizeChn * 2) + ivbufSizeChn;/* inBuf + outBuf + keyBuf */
    bufSizeTotal = bufSizeChn * (CIPHER_PKGxN_CHAN_MAX - CIPHER_PKGxN_CHAN_MIN + 1) ; /* only 7 channels need buf */

    HAL_Cipher_Init();
    HAL_Cipher_DisableAllInt();

    /* allocate 7 channels size */
    ret = HI_DRV_MMZ_AllocAndMap("CIPHER_ChnBuf",NULL, bufSizeTotal, 0, &(cipherListBuf));
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Can NOT get mem for cipher, init failed, exit...\n");
        HI_DRV_MMZ_UnmapAndRelease(&g_stCipherComm.stTempPhyBuf);
        return HI_FAILURE;
    }
    else
    {
        memset((void*)(cipherListBuf.pu8StartVirAddr), 0, cipherListBuf.u32Size);
        memcpy(&(g_stCipherComm.stChainedListPhyBuf), &(cipherListBuf), sizeof(g_stCipherComm.stChainedListPhyBuf));
    }

    HI_DEBUG_CIPHER("TOTAL BUF: %#x/%p", cipherListBuf.u32StartPhyAddr, cipherListBuf.pu8StartVirAddr);

    /* assign hardware channel ID from 0 to 7 */
    for (i = 0; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        pChan = &g_stCipherChans[i];
        pChan->chnId = i;
    }

/*
channel layout
==============================================================
|
|
==============================================================
/\                                     /\                                      /\
 |              IV buf                  |             IN buf                    |             OUT buf
startPhyAddr
==============================================================
|
|
==============================================================
/\                                     /\                                      /\
 |              IV buf                  |             IN buf                    |             OUT buf
 startVirAddr
*/
    for (i = 0; i < CIPHER_PKGxN_CHAN_MAX; i++)
    {
        /* config channel from 1 to 7 */
        hwChnId = i + CIPHER_PKGxN_CHAN_MIN;
        pChan = &g_stCipherChans[hwChnId];

        pChan->astCipherIVValue[0].u32PhyAddr
            = cipherListBuf.u32StartPhyAddr + (i * bufSizeChn);
        pChan->astCipherIVValue[0].pu32VirAddr
            = (HI_U32*)(cipherListBuf.pu8StartVirAddr + (i * bufSizeChn));

        for (j = 1; j < CIPHER_MAX_LIST_NUM; j++)
        {
            pChan->astCipherIVValue[j].u32PhyAddr
                = pChan->astCipherIVValue[0].u32PhyAddr + (CI_IV_SIZE * j);
            pChan->astCipherIVValue[j].pu32VirAddr
                = (HI_U32*)(((HI_U32)pChan->astCipherIVValue[0].pu32VirAddr) + (CI_IV_SIZE * j));

            pChan->bNeedCallback[j] = HI_FALSE;
        }

        pChan->pstInBuf = (CI_BUF_LIST_ENTRY_S*)((HI_U32)(pChan->astCipherIVValue[0].pu32VirAddr) + ivbufSizeChn);
        pChan->pstOutBuf = (CI_BUF_LIST_ENTRY_S*)((HI_U32)(pChan->pstInBuf) + databufSizeChn);

        HAL_Cipher_SetBufAddr(hwChnId, CIPHER_BUF_TYPE_IN,
            pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn);
        HAL_Cipher_SetBufAddr(hwChnId, CIPHER_BUF_TYPE_OUT,
            pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn + databufSizeChn);

        DRV_CipherInitHardWareChn(hwChnId);
    }

    /* debug info */
    for (i = 0; i < CIPHER_PKGxN_CHAN_MAX; i++)
    {
        hwChnId = i+CIPHER_PKGxN_CHAN_MIN;
        pChan = &g_stCipherChans[hwChnId];

        HI_INFO_CIPHER("Chn%02x, IV:%#x/%p In:%#x/%p, Out:%#x/%p.\n", i,
            pChan->astCipherIVValue[0].u32PhyAddr,
            pChan->astCipherIVValue[0].pu32VirAddr,
            pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn, pChan->pstInBuf,
            pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn + databufSizeChn, pChan->pstOutBuf );
    }

    HAL_Cipher_ClrIntState(0xffffffff);

    /* request irq */
	ret = request_irq(cipher_irq, DRV_Cipher_ISR, 0, "hi_cipher_irq", NULL);
    if(HI_SUCCESS != ret)
    {
        HAL_Cipher_DisableAllInt();
        HAL_Cipher_ClrIntState(0xffffffff);

        HI_ERR_CIPHER("Irq request failure, irq =0x%x.", CIPHER_IRQ_NUMBER);
        HI_DRV_MMZ_UnmapAndRelease(&(g_stCipherComm.stChainedListPhyBuf));
        HI_DRV_MMZ_UnmapAndRelease(&g_stCipherComm.stTempPhyBuf);
        return HI_FAILURE;
    }

#if 0
    HAL_Cipher_EnableAllSecChn();
#endif

    return HI_SUCCESS;
}

HI_VOID DRV_CIPHER_DeInit(HI_VOID)
{
    HI_U32 i, hwChnId;

    HAL_Cipher_DisableAllInt();
    HAL_Cipher_ClrIntState(0xffffffff);

    for (i = 0; i < CIPHER_PKGxN_CHAN_MAX; i++)
    {
        hwChnId = i+CIPHER_PKGxN_CHAN_MIN;
        DRV_CipherDeInitHardWareChn(hwChnId);
    }

    /* free irq */
    free_irq(cipher_irq, NULL);

    HI_DRV_MMZ_UnmapAndRelease(&g_stCipherComm.stChainedListPhyBuf);
    HI_DRV_MMZ_UnmapAndRelease(&g_stCipherComm.stTempPhyBuf);

    HAL_Cipher_DeInit();

    return;
}

HI_VOID HI_DRV_CIPHER_Suspend(HI_VOID)
{
    DRV_CIPHER_DeInit();

    return;
}

HI_S32 HI_DRV_CIPHER_Resume(HI_VOID)
{
    return DRV_CIPHER_Init();
}

#ifdef CFG_HI_CIPHER_GETHANDLECONFIG_SUPPORT
HI_S32 DRV_CIPHER_GetHandleConfig(CIPHER_Config_CTRL *pstCipherConfig)
{
    CIPHER_SOFTCHAN_S *pSoftChan = NULL;
    HI_U32 u32SoftChanId = 0;

    if(pstCipherConfig == NULL)
    {
        HI_ERR_CIPHER("Error! NULL pointer!\n");
        return HI_FAILURE;
    }

    CIPHER_CheckHandle(pstCipherConfig->CIHandle);
    u32SoftChanId = HI_HANDLE_GET_CHNID(pstCipherConfig->CIHandle);

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    memcpy(&pstCipherConfig->CIpstCtrl, &(pSoftChan->stCtrl), sizeof(HI_UNF_CIPHER_CTRL_S));
    return HI_SUCCESS;
}

HI_S32 HI_DRV_CIPHER_GetHandleConfig(CIPHER_Config_CTRL *pstCipherConfig)
{
    HI_S32 ret;
    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    ret = DRV_CIPHER_GetHandleConfig(pstCipherConfig);

	up(&g_CipherMutexKernel);
    return ret;
}
#endif

HI_S32 DRV_CIPHER_CreateHandle(CIPHER_HANDLE_S *pstCIHandle, HI_VOID *file)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 i = 0;
    HI_HANDLE hCipherchn = 0;
    HI_U32 softChnId = 0;

    if ( NULL == pstCIHandle)
    {
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_FAILURE;
    }

    for(i = CIPHER_PKGxN_CHAN_MIN; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        if (0 == g_stCipherOsrChn[i].g_bSoftChnOpen)
        {
            break;
        }
    }

    if (i >= CIPHER_SOFT_CHAN_NUM)
    {
        ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
        HI_ERR_CIPHER("No more cipher chan left.\n");
        return ret;
    }
    else
    {
        g_stCipherOsrChn[i].pstDataPkg = HI_VMALLOC(HI_ID_CIPHER, sizeof(HI_UNF_CIPHER_DATA_S) * CIPHER_MAX_LIST_NUM);
        if (NULL == g_stCipherOsrChn[i].pstDataPkg)
        {
            ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
            HI_ERR_CIPHER("can NOT malloc memory for cipher.\n");
            return HI_FAILURE;
        }

        softChnId = i;
        g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_TRUE;
    }

    ret = DRV_CIPHER_OpenChn(softChnId);
    if (HI_SUCCESS != ret)
    {
        HI_VFREE(HI_ID_CIPHER, g_stCipherOsrChn[i].pstDataPkg);
        g_stCipherOsrChn[i].pstDataPkg = NULL;
        return HI_FAILURE;
    }

    g_stCipherOsrChn[i].pWichFile = file;

    hCipherchn = HI_HANDLE_MAKEHANDLE(HI_ID_CIPHER, 0, softChnId);
    pstCIHandle->hCIHandle = hCipherchn;

    return HI_SUCCESS;
}


HI_S32 HI_DRV_CIPHER_CreateHandle(CIPHER_HANDLE_S *pstCIHandle, HI_U32 file)
{
	HI_S32 ret = HI_FAILURE;
    if (HI_NULL == pstCIHandle)
    {
    	HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

	ret = DRV_CIPHER_CreateHandle(pstCIHandle, (struct file*)file);

	up(&g_CipherMutexKernel);

	return ret;
}

HI_S32 DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn)
{
    HI_U32 softChnId = 0;
	HI_S32 ret = HI_FAILURE;

    if((HI_ID_CIPHER != HI_HANDLE_GET_MODID(hCipherchn))
        || (0 != HI_HANDLE_GET_PriDATA(hCipherchn)))
    {
        HI_ERR_CIPHER("Invalid ChanId!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    softChnId = HI_HANDLE_GET_CHNID(hCipherchn);
    if (softChnId >= CIPHER_SOFT_CHAN_NUM)
    {
        HI_ERR_CIPHER("chan %d is too large, max: %d\n", softChnId, CIPHER_SOFT_CHAN_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
    {
        return HI_SUCCESS;
    }

    if (g_stCipherOsrChn[softChnId].pstDataPkg)
    {
        HI_VFREE(HI_ID_CIPHER, g_stCipherOsrChn[softChnId].pstDataPkg);
        g_stCipherOsrChn[softChnId].pstDataPkg = NULL;
    }

    g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_FALSE;
    g_stCipherOsrChn[softChnId].pWichFile = NULL;

    ret = DRV_CIPHER_CloseChn(softChnId);

	return ret;
}


HI_S32 HI_DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn)
{
	HI_S32 ret = HI_FAILURE;

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

	ret = DRV_CIPHER_DestroyHandle(hCipherchn);

	up(&g_CipherMutexKernel);

	return ret;
}

HI_S32 DRV_CIPHER_Encrypt(HI_U32 softChnId,
                          HI_U32 ScrPhyAddr,
                          HI_U32 DestPhyAddr,
                          HI_U32 u32DataLength,
                          HI_BOOL bIsDescrypt)
{
    return DRV_CIPHER_EncryptEx(softChnId, ScrPhyAddr, DestPhyAddr, u32DataLength, bIsDescrypt, HI_NULL);
}

static HI_S32 CIPHER_LengthCheck(HI_UNF_CIPHER_CTRL_S *pstCtrl, HI_U32 u32ByteLength)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32S;
    HI_U32 u32B;
    HI_U32 u32BlockSize;

    if ( u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    switch(pstCtrl->enAlg)
    {
    case HI_UNF_CIPHER_ALG_DES:
    case HI_UNF_CIPHER_ALG_3DES:
        u32BlockSize = 8;
        break;
    case HI_UNF_CIPHER_ALG_AES:
        u32BlockSize = 16;
        break;
    default:
        HI_ERR_CIPHER("Error: alg(0x%X)!\n", pstCtrl->enAlg);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    u32S = u32ByteLength % u32BlockSize;
    if(u32S == 0)
    {
        return HI_SUCCESS;
    }

    switch(pstCtrl->enBitWidth)
    {
    case HI_UNF_CIPHER_BIT_WIDTH_1BIT:
        u32B = 1;
        break;
    case HI_UNF_CIPHER_BIT_WIDTH_8BIT:
        u32B = 1;
        break;
    case HI_UNF_CIPHER_BIT_WIDTH_64BIT:
        u32B = 8;
        break;
    case HI_UNF_CIPHER_BIT_WIDTH_128BIT:
        u32B = 16;
        break;
    default:
        HI_ERR_CIPHER("Error: BitWidth(0x%X)!\n", pstCtrl->enBitWidth);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    switch(pstCtrl->enWorkMode)
    {
    case HI_UNF_CIPHER_WORK_MODE_ECB:
    case HI_UNF_CIPHER_WORK_MODE_CBC:
    case HI_UNF_CIPHER_WORK_MODE_CBC_CTS:
         if (u32S != 0)
         {
            HI_ERR_CIPHER("Error: Length not align at %d!\n", u32B);
            Ret =  HI_ERR_CIPHER_INVALID_PARA;
         }
         break;
    case HI_UNF_CIPHER_WORK_MODE_CFB:
    case HI_UNF_CIPHER_WORK_MODE_OFB:
         if ((u32ByteLength % u32B) != 0)
         {
            HI_ERR_CIPHER("Error: Length not align at %d!\n", u32B);
            Ret =  HI_ERR_CIPHER_INVALID_PARA;
         }
         break;
    case HI_UNF_CIPHER_WORK_MODE_CTR:
         break;
#ifdef CIPHER_CCM_GCM_SUPPORT
    case HI_UNF_CIPHER_WORK_MODE_CCM:
         break;
    case HI_UNF_CIPHER_WORK_MODE_GCM:
         break;
#endif
    default:
        HI_ERR_CIPHER("Error: Alg invalid!\n");
        Ret = HI_ERR_CIPHER_INVALID_PARA;
    }

    return Ret;
}

HI_S32 DRV_CIPHER_EncryptEx(HI_U32 u32SoftChanId,
                        HI_U32 ScrPhyAddr,
                        HI_U32 DestPhyAddr,
                        HI_U32 u32DataLength,
                        HI_BOOL bIsDescrypt,
                        HI_U8  *pLastBlock)
{
    HI_S32 Ret;
    MMZ_BUFFER_S *pstMmzTemp;
    HI_U8 *pu8Buf = 0;
    CIPHER_SOFTCHAN_S *pSoftChan;
    HI_U32 u32PkgNum = 0;
    HI_U32 u32Offset = 0;
    HI_U32 u32Copysize = 0;
    MMZ_BUFFER_S stSrcMmzBuf = {0};
    MMZ_BUFFER_S stDestMmzBuf = {0};
    HI_DRV_CIPHER_TASK_S stCITask;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];

    pstMmzTemp = &pSoftChan->stMmzTemp;

    if (u32DataLength == 0)
    {
        return HI_SUCCESS;
    }

    stSrcMmzBuf.u32Size = u32DataLength;
    stSrcMmzBuf.u32StartPhyAddr = ScrPhyAddr;
    HI_DRV_MMZ_Map(&stSrcMmzBuf);
    if ( stSrcMmzBuf.pu8StartVirAddr == 0)
    {
        HI_ERR_CIPHER("DRV SRC MMZ MAP(0x%x) ERROR, sizse = 0x%x!\n", ScrPhyAddr, u32DataLength);
        return HI_FAILURE;
    }
    if (DestPhyAddr != 0)
    {
        stDestMmzBuf.u32Size = u32DataLength;
        stDestMmzBuf.u32StartPhyAddr = DestPhyAddr;
        HI_DRV_MMZ_Map(&stDestMmzBuf);
        if ( stDestMmzBuf.pu8StartVirAddr == 0)
        {
            HI_ERR_CIPHER("DRV Dest MMZ MAP(0x%x) ERROR, , sizse = 0x%x!\n", DestPhyAddr, u32DataLength);
            HI_DRV_MMZ_Unmap(&stSrcMmzBuf);
            return HI_FAILURE;;
        }
    }

    if ( 0 != u32SoftChanId )
    {
        HI_DRV_CIPHER_DATA_INFO_S Buf2Process[2];

        u32Copysize = u32DataLength & (~0x0F);
        if (u32Copysize > 0)
        {
            Buf2Process[u32PkgNum].u32src = ScrPhyAddr;
            Buf2Process[u32PkgNum].u32dest = DestPhyAddr;
            Buf2Process[u32PkgNum].u32length = u32Copysize;
            Buf2Process[u32PkgNum].bDecrypt = bIsDescrypt;
//          HI_PRINT_HEX ("in", (HI_U8*)stSrcMmzBuf.pu8StartVirAddr, u32Copysize);
            u32PkgNum++;
        }
        u32Copysize = u32DataLength & 0x0F;
        if (u32Copysize > 0)
        {
            pu8Buf = (HI_U8*)(stSrcMmzBuf.pu8StartVirAddr + u32DataLength - u32Copysize);
            if (pLastBlock != HI_NULL)
            {
                memcpy((HI_U8*)pstMmzTemp->pu8StartVirAddr, pLastBlock, AES_BLOCK_SIZE);
            }
            else
            {
                memset((HI_U8*)pstMmzTemp->pu8StartVirAddr, 0, AES_BLOCK_SIZE);
            }
            memcpy((HI_U8*)pstMmzTemp->pu8StartVirAddr, pu8Buf, u32Copysize);
            Buf2Process[u32PkgNum].u32src = pstMmzTemp->u32StartPhyAddr;
            Buf2Process[u32PkgNum].u32dest = (DestPhyAddr == 0 ? 0 : pstMmzTemp->u32StartPhyAddr);
            Buf2Process[u32PkgNum].u32length = AES_BLOCK_SIZE;//16 - u32Copysize;
            Buf2Process[u32PkgNum].bDecrypt = bIsDescrypt;
            u32PkgNum++;
 //         HI_PRINT_HEX ("in", (HI_U8*)pstMmzTemp->pu8StartVirAddr, AES_BLOCK_SIZE);
        }

        g_stCipherOsrChn[u32SoftChanId].g_bDataDone = HI_FALSE;
        Ret = DRV_CIPHER_CreatMultiPkgTask(u32SoftChanId, Buf2Process, u32PkgNum, HI_FALSE, u32SoftChanId);
        if (stDestMmzBuf.pu8StartVirAddr != 0)
        {
			Ret = wait_event_interruptible_timeout(g_stCipherOsrChn[u32SoftChanId].cipher_wait_queue,
                        g_stCipherOsrChn[u32SoftChanId].g_bDataDone != HI_FALSE, CIPHER_TIME_OUT);
            if (0 == Ret)
            {
                HI_ERR_CIPHER("DRV_CIPHER_CreatMultiPkgTask() - Encrypt time out! \n");
                HI_DRV_MMZ_Unmap(&stDestMmzBuf);
                HI_DRV_MMZ_Unmap(&stSrcMmzBuf);
                return HI_FAILURE;
            }
            if (u32Copysize > 0)
            {
                pu8Buf = (HI_U8*)(stDestMmzBuf.pu8StartVirAddr + u32DataLength - u32Copysize);
                memcpy (pu8Buf, (HI_U8*)pstMmzTemp->pu8StartVirAddr, u32Copysize);
            }
        }
    }
    else
    {
		while(u32Offset < u32DataLength)
		{
            if ((u32DataLength - u32Offset) > 16)
            {
                u32Copysize = 16;
            }
            else
            {
                u32Copysize =u32DataLength - u32Offset;
                if (pLastBlock != HI_NULL)
                {
                    memcpy((HI_U8 *)stCITask.stData2Process.u32DataPkg, pLastBlock, AES_BLOCK_SIZE);
                }
                else
                {
                    memset((HI_U8 *)stCITask.stData2Process.u32DataPkg, 0, AES_BLOCK_SIZE);
                }
            }
            pu8Buf = (HI_U8*)(stSrcMmzBuf.pu8StartVirAddr + u32Offset);
		    memcpy((HI_U8 *)stCITask.stData2Process.u32DataPkg, pu8Buf, u32Copysize);
		    stCITask.stData2Process.u32length = 16;
		    stCITask.stData2Process.bDecrypt = bIsDescrypt;
		    stCITask.u32CallBackArg = u32SoftChanId;
//          HI_PRINT_HEX ("in", (HI_U8 *)(stCITask.stData2Process.u32DataPkg), 16);
		    Ret = DRV_CIPHER_CreatTask(u32SoftChanId, &stCITask, NULL, NULL);
		    if (HI_SUCCESS != Ret)
		    {
	            HI_ERR_CIPHER("Cipher create task failed!\n");
                HI_DRV_MMZ_Unmap(&stDestMmzBuf);
                HI_DRV_MMZ_Unmap(&stSrcMmzBuf);
	            return HI_FAILURE;
		    }
            if (stDestMmzBuf.pu8StartVirAddr != 0)
            {
                pu8Buf = (HI_U8*)(stDestMmzBuf.pu8StartVirAddr + u32Offset);
                memcpy (pu8Buf, (HI_U8 *)stCITask.stData2Process.u32DataPkg, u32Copysize);
//                HI_PRINT_HEX ("out", (HI_U8 *)(stCITask.stData2Process.u32DataPkg), 16);
            }
		    u32Offset += 16;
		}
    }

    if(stSrcMmzBuf.pu8StartVirAddr != 0)
    {
        HI_DRV_MMZ_Unmap(&stSrcMmzBuf);
    }
    if(stDestMmzBuf.pu8StartVirAddr != 0)
    {
        HI_DRV_MMZ_Unmap(&stDestMmzBuf);
    }

    return HI_SUCCESS;
}

#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 DRV_CIPHER_CCM(HI_U32 u32SoftChanId, HI_U32 ScrPhyAddr,
                      HI_U32 DestPhyAddr, HI_U32 u32DataLength, HI_BOOL bIsDescrypt)
{
    HI_S32 Ret;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;
    HI_UNF_CIPHER_CCM_INFO_S *pstCCM;
    CIPHER_CCM_STATUS_S *pstCcmStatus;
    CIPHER_SOFTCHAN_S *pSoftChan;
    HI_U32 u32IV[4];

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];

    pstCtrl = &pSoftChan->stCtrl;
    pstCCM = &pstCtrl->unModeInfo.stCCM;
    pstCcmStatus = &pSoftChan->stCcmStatus;

    if((u32DataLength+pstCcmStatus->u32Plen) > pstCCM->u32MLen)
    {
        HI_ERR_CIPHER("Cipher process failed, datasize overflow!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (bIsDescrypt)
    {
        /*CTR Start, diphertext descrypt*/
        pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CCM;
        memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)(pstCcmStatus->u32CTRm), CI_IV_SIZE); //Recover CTR IV CTRM
        pSoftChan->bIVChange = HI_TRUE;
        Ret = DRV_CIPHER_Encrypt(u32SoftChanId, ScrPhyAddr, DestPhyAddr, u32DataLength, HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
        HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
        memcpy((HI_U8*)pstCcmStatus->u32CTRm, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
//      HI_PRINT_HEX ("CTRM", (HI_U8*)pstCcmStatus->u32CTRm, 16);

        /*AES-CBC Start, calculate MAC of plaintext*/
        pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
        memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)(pstCcmStatus->u32Yi), CI_IV_SIZE);
        memcpy((HI_U8*)u32IV, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);//Recover CBC IV Yi
        pSoftChan->bIVChange = HI_TRUE;
        Ret = DRV_CIPHER_Encrypt(u32SoftChanId, DestPhyAddr, DestPhyAddr, u32DataLength, HI_FALSE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
        HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
        memcpy((HI_U8*)pstCcmStatus->u32Yi, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);

        memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)u32IV, CI_IV_SIZE);
        pSoftChan->bIVChange = HI_TRUE;
        Ret = DRV_CIPHER_EncryptEx(u32SoftChanId, DestPhyAddr, DestPhyAddr,
            u32DataLength, HI_TRUE, (HI_U8*)pstCcmStatus->u32Yi);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
    }
    else //Encrypt
    {
        /*AES-CBC Start, calculate MAC of plaintext*/
        pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
        memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)(pstCcmStatus->u32Yi), CI_IV_SIZE);
        memcpy((HI_U8*)u32IV, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
        pSoftChan->bIVChange = HI_TRUE;
        Ret = DRV_CIPHER_Encrypt(u32SoftChanId, ScrPhyAddr, DestPhyAddr, u32DataLength, HI_FALSE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
        HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
        memcpy((HI_U8*)pstCcmStatus->u32Yi, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
//        HI_PRINT_HEX ("Y", (HI_U8*)pstCcmStatus->u32Yi, 16);

        memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)u32IV, CI_IV_SIZE);
        pSoftChan->bIVChange = HI_TRUE;
        Ret = DRV_CIPHER_EncryptEx(u32SoftChanId, DestPhyAddr, DestPhyAddr,
                u32DataLength, HI_TRUE, (HI_U8*)pstCcmStatus->u32Yi);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }

        /*CTR Start, plaintext encrypt*/
        pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CCM;
        memcpy((HI_U8*)pstCtrl->u32IV, (HI_U8*)(pstCcmStatus->u32CTRm), CI_IV_SIZE);
        pSoftChan->bIVChange = HI_TRUE;
        Ret = DRV_CIPHER_Encrypt(u32SoftChanId, ScrPhyAddr, DestPhyAddr, u32DataLength, HI_FALSE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
        HAL_Cipher_GetOutIV(pSoftChan->u32HardWareChn, pstCtrl);
        memcpy((HI_U8*)pstCcmStatus->u32CTRm, (HI_U8*)pstCtrl->u32IV, CI_IV_SIZE);
 //     HI_PRINT_HEX ("CTRM", (HI_U8*)pstCcmStatus->u32CTRm, 16);
     }

    pstCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_CCM;
    pstCcmStatus->u32Plen += u32DataLength;

    return Ret;

}

HI_S32 DRV_CIPHER_GCM(HI_U32 u32SoftChanId, HI_U32 ScrPhyAddr,
                      HI_U32 DestPhyAddr, HI_U32 u32DataLength, HI_BOOL bIsDescrypt)
{
    HI_S32 ret;
    CIPHER_SOFTCHAN_S *pSoftChan;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstCtrl = &pSoftChan->stCtrl;

    if ((pSoftChan->stGcmStatus.u32Plen + u32DataLength) <= pstCtrl->unModeInfo.stGCM.u32MLen)
    {
        ret = DRV_CIPHER_Encrypt(u32SoftChanId, ScrPhyAddr, DestPhyAddr, u32DataLength, bIsDescrypt);
        pSoftChan->stGcmStatus.u32Plen+=u32DataLength;
    }
    else
    {
        HI_ERR_CIPHER("Error: CIPHER Length Check failed, Mlen: 0x%x, datalength: 0x%x!\n",
            pstCtrl->unModeInfo.stGCM.u32MLen, u32DataLength);
        ret = HI_ERR_CIPHER_INVALID_PARA;
    }
    return ret;

}

HI_S32 DRV_CIPHER_GetTag(HI_U32 u32SoftChanId, HI_U32 *pu32Tag, HI_U32 *pu32Len)
{
	HI_S32 ret = HI_SUCCESS;
    CIPHER_SOFTCHAN_S *pSoftChan;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;
    HI_U32 i;

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstCtrl = &pSoftChan->stCtrl;

    ret = HAL_RSA_WaitTagVld(u32SoftChanId, pstCtrl);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    switch(pstCtrl->enWorkMode)
    {
    case HI_UNF_CIPHER_WORK_MODE_CCM:
        if (pSoftChan->stCcmStatus.u32Plen != pstCtrl->unModeInfo.stCCM.u32MLen)
        {
            HI_ERR_CIPHER ("Can't get TAG before complete the compute, already compute: 0x%x, total: 0x%x\n",
                pSoftChan->stCcmStatus.u32Plen, pstCtrl->unModeInfo.stCCM.u32MLen);
            ret = HI_ERR_CIPHER_INVALID_PARA;
            break;
        }
        for(i=0; i<4; i++)
        {
            pu32Tag[i] = pSoftChan->stCcmStatus.u32Yi[i] ^ pSoftChan->stCcmStatus.u32S0[i];
        }
        memset((HI_U8*)pu32Tag + pstCtrl->unModeInfo.stCCM.u8TLen, 0, AES_BLOCK_SIZE - pstCtrl->unModeInfo.stCCM.u8TLen);
        *pu32Len = pstCtrl->unModeInfo.stCCM.u8TLen;
        break;
    case HI_UNF_CIPHER_WORK_MODE_GCM:
        if (pSoftChan->stGcmStatus.u32Plen != pstCtrl->unModeInfo.stGCM.u32MLen)
        {
            HI_ERR_CIPHER ("Can't get TAG before complete the compute, already compute: 0x%x, total: 0x%x\n",
                pSoftChan->stGcmStatus.u32Plen, pstCtrl->unModeInfo.stGCM.u32MLen);
            ret = HI_ERR_CIPHER_INVALID_PARA;
            break;
        }
        ret = HAL_Cipher_GetTag(u32SoftChanId, pstCtrl, pu32Tag);
        *pu32Len = 16;
        break;
    default:
        HI_ERR_CIPHER ("Can't get TAG with mode 0x%x\n", pstCtrl->enWorkMode);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

	return ret;
}

HI_S32 HI_DRV_CIPHER_GetTag(CIPHER_TAG *pstTag)
{
	HI_S32 ret = HI_FAILURE;
    HI_U32 u32SoftChanId = 0;

    if (HI_NULL == pstTag)
    {
		HI_ERR_CIPHER("Invalid point!\n");
		return HI_ERR_CIPHER_INVALID_POINT;
    }

	if(down_interruptible(&g_CipherMutexKernel))
	{
		HI_ERR_CIPHER("down_interruptible failed!\n");
		return HI_FAILURE;
	}

    CIPHER_CheckHandle(pstTag->CIHandle);
    u32SoftChanId = HI_HANDLE_GET_CHNID(pstTag->CIHandle);

	ret = DRV_CIPHER_GetTag(u32SoftChanId, pstTag->u32Tag, &pstTag->u32Len);

	up(&g_CipherMutexKernel);

	return ret;
}
#endif

HI_S32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData)
{
	HI_S32 ret = HI_FAILURE;
    HI_U32 u32SoftChanId = 0;
	CIPHER_SOFTCHAN_S *pSoftChan;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl = HI_NULL;
    HI_U32 u32Offset = 0;
    HI_U32 u32TempLen = 0;

    if (HI_NULL == pstCIData)
    {
    	HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (0 == pstCIData->u32DataLength)
    {
        return HI_SUCCESS;
    }

    if ((pstCIData->ScrPhyAddr + pstCIData->u32DataLength < pstCIData->u32DataLength)
        || (pstCIData->DestPhyAddr + pstCIData->u32DataLength < pstCIData->u32DataLength))
    {
    	HI_ERR_CIPHER("Invalid src phy addr,dest phy addr or data Length!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if(down_interruptible(&g_CipherMutexKernel))
	{
		HI_ERR_CIPHER("down_interruptible failed!\n");
		return HI_FAILURE;
	}

    CIPHER_CheckHandle(pstCIData->CIHandle);
    u32SoftChanId = HI_HANDLE_GET_CHNID(pstCIData->CIHandle);

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstCtrl = &pSoftChan->stCtrl;

    ret = CIPHER_LengthCheck(pstCtrl, pstCIData->u32DataLength);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error: CIPHER Length Check failed!\n");
        up(&g_CipherMutexKernel);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    while(u32Offset < pstCIData->u32DataLength)
    {
        if((pstCIData->u32DataLength - u32Offset) > CIPHER_MXA_PKG_SIZE)
        {
            u32TempLen = CIPHER_MXA_PKG_SIZE;
        }
        else
        {
            u32TempLen = pstCIData->u32DataLength - u32Offset;
        }

        switch(pstCtrl->enWorkMode)
        {
#ifdef CIPHER_CCM_GCM_SUPPORT
        case HI_UNF_CIPHER_WORK_MODE_CCM:
            ret = DRV_CIPHER_CCM(u32SoftChanId, pstCIData->ScrPhyAddr,
                pstCIData->DestPhyAddr, pstCIData->u32DataLength, HI_FALSE);
            break;
        case HI_UNF_CIPHER_WORK_MODE_GCM:
            ret = DRV_CIPHER_GCM(u32SoftChanId, pstCIData->ScrPhyAddr,
                pstCIData->DestPhyAddr, pstCIData->u32DataLength, HI_FALSE);
            break;
#endif
        default:
            ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstCIData->ScrPhyAddr,
                pstCIData->DestPhyAddr, pstCIData->u32DataLength, HI_FALSE);
            break;
        }
        if(ret != HI_SUCCESS)
        {
            up(&g_CipherMutexKernel);
            HI_ERR_CIPHER("Cipher_Decrypt failure, ret=0x%x\n", ret);
            return HI_FAILURE;
        }
        u32Offset += u32TempLen;
    }
	up(&g_CipherMutexKernel);

	return ret;
}

HI_S32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData)
{
	HI_S32 ret = HI_SUCCESS;
    HI_U32 u32SoftChanId = 0;
	CIPHER_SOFTCHAN_S *pSoftChan;
    HI_UNF_CIPHER_CTRL_S  *pstCtrl;
    HI_U32 u32Offset = 0;
    HI_U32 u32TempLen = 0;

    if (HI_NULL == pstCIData)
    {
    	HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (0 == pstCIData->u32DataLength)
    {
        return HI_SUCCESS;
    }

    if ((pstCIData->ScrPhyAddr + pstCIData->u32DataLength < pstCIData->u32DataLength)
        || (pstCIData->DestPhyAddr + pstCIData->u32DataLength < pstCIData->u32DataLength))
    {
    	HI_ERR_CIPHER("Invalid src phy addr,dest phy addr or data Length!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if(down_interruptible(&g_CipherMutexKernel))
	{
		HI_ERR_CIPHER("down_interruptible failed!\n");
		return HI_FAILURE;
	}

    CIPHER_CheckHandle(pstCIData->CIHandle);
    u32SoftChanId = HI_HANDLE_GET_CHNID(pstCIData->CIHandle);

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    pstCtrl = &pSoftChan->stCtrl;

    ret = CIPHER_LengthCheck(pstCtrl, pstCIData->u32DataLength);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Error: CIPHER Length Check failed!\n");
        up(&g_CipherMutexKernel);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    while(u32Offset < pstCIData->u32DataLength)
    {
        if((pstCIData->u32DataLength - u32Offset) > CIPHER_MXA_PKG_SIZE)
        {
            u32TempLen = CIPHER_MXA_PKG_SIZE;
        }
        else
        {
            u32TempLen = pstCIData->u32DataLength - u32Offset;
        }

        switch(pstCtrl->enWorkMode)
        {
#ifdef CIPHER_CCM_GCM_SUPPORT
        case HI_UNF_CIPHER_WORK_MODE_CCM:
            ret = DRV_CIPHER_CCM(u32SoftChanId, pstCIData->ScrPhyAddr,
                pstCIData->DestPhyAddr, pstCIData->u32DataLength, HI_TRUE);
            break;
        case HI_UNF_CIPHER_WORK_MODE_GCM:
            ret = DRV_CIPHER_GCM(u32SoftChanId, pstCIData->ScrPhyAddr,
                pstCIData->DestPhyAddr, pstCIData->u32DataLength, HI_TRUE);
            break;
#endif
        default:
            ret = DRV_CIPHER_Encrypt(u32SoftChanId, pstCIData->ScrPhyAddr,
                pstCIData->DestPhyAddr, pstCIData->u32DataLength, HI_TRUE);
            break;
        }
        if(ret != HI_SUCCESS)
        {
            up(&g_CipherMutexKernel);
            HI_ERR_CIPHER("Cipher_Decrypt failure, ret=0x%x\n", ret);
            return HI_FAILURE;
        }
        u32Offset += u32TempLen;
    }
	up(&g_CipherMutexKernel);

	return ret;
}

HI_S32 DRV_CIPHER_EncryptMulti(CIPHER_DATA_S *pstCIData)
{
    HI_U32 i = 0;
    HI_U32 softChnId = 0;
    static HI_DRV_CIPHER_DATA_INFO_S  tmpData[CIPHER_MAX_LIST_NUM];
    HI_UNF_CIPHER_DATA_S *pTmp = NULL;
    HI_U32 pkgNum = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 raw = 0, count = 0;

    if(NULL == pstCIData)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CIPHER_CheckHandle(pstCIData->CIHandle);
    softChnId = HI_HANDLE_GET_CHNID(pstCIData->CIHandle);

    if ((g_stCipherSoftChans[softChnId].stCtrl.enWorkMode == HI_UNF_CIPHER_WORK_MODE_CCM)
        || (g_stCipherSoftChans[softChnId].stCtrl.enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM))
    {
        HI_ERR_CIPHER("Error: Multi cipher unsuport CCM/GCM mode.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pkgNum = pstCIData->u32DataLength;
    if ((pkgNum > CIPHER_MAX_LIST_NUM) || (pkgNum == 0))
    {
        HI_ERR_CIPHER("Error: Invalid pkg num(%d), must < %d and > 0.\n",pkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (pstCIData->ScrPhyAddr + pkgNum * sizeof(HI_UNF_CIPHER_DATA_S) < pstCIData->ScrPhyAddr)
    {
        HI_ERR_CIPHER("Invalid src phy addr!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (copy_from_user(g_stCipherOsrChn[softChnId].pstDataPkg, (void*)pstCIData->ScrPhyAddr,
                       pkgNum * sizeof(HI_UNF_CIPHER_DATA_S)))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < pstCIData->u32DataLength; i++)
    {
        pTmp = g_stCipherOsrChn[softChnId].pstDataPkg + i;
        if ((pTmp->u32SrcPhyAddr + pTmp->u32ByteLength < pTmp->u32ByteLength)
            || (pTmp->u32DestPhyAddr + pTmp->u32ByteLength < pTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Invalid phy addr or data lenth!\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        tmpData[i].bDecrypt = HI_FALSE;
        tmpData[i].u32src = pTmp->u32SrcPhyAddr;
        tmpData[i].u32dest = pTmp->u32DestPhyAddr;
        tmpData[i].u32length = pTmp->u32ByteLength;
    }

    HI_INFO_CIPHER("Start to DecryptMultiPkg, chnNum = %#x, pkgNum=%d!\n", softChnId, pkgNum);

#if 0
    g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;

    ret = DRV_CIPHER_CreatMultiPkgTask(softChnId, tmpData, pkgNum, HI_TRUE, softChnId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher create multi task failed!\n");
        return ret;
    }

    if (0== wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
                g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, CIPHER_TIME_OUT))
    {
        HI_ERR_CIPHER("Decrypt time out \n");
        return HI_FAILURE;
    }
#else

    HAL_Cipher_DisableInt(softChnId, CIPHER_INT_TYPE_OUT_BUF);

    ret =  DRV_CIPHER_CreatMultiPkgTask(softChnId, tmpData, pkgNum, HI_TRUE, softChnId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher create multi task failed!\n");
        return ret;
    }

    while((((raw >>(softChnId + 8)) & 0x01) == 0x00) && (count++ < 0x10000000))
    {
        HAL_Cipher_GetRawIntState(&raw);
    }

    if (count < 0x10000000)
    {
        HAL_Cipher_ClrIntState(1 << (softChnId + 8));
        DRV_CipherDataDoneMultiPkg(softChnId);
    }
    else
    {
        HI_ERR_CIPHER ("Wait raw int timeout, raw = 0x%x\n", raw);
        ret = HI_FAILURE;
    }

    HAL_Cipher_EnableInt(softChnId, CIPHER_INT_TYPE_OUT_BUF);
#endif
    HI_INFO_CIPHER("Decrypt OK, chnNum = %#x!\n", softChnId);

    return HI_SUCCESS;
}

HI_S32 DRV_CIPHER_DecryptMulti(CIPHER_DATA_S *pstCIData)
{
    HI_U32 i;
    HI_U32 softChnId = 0;
    static HI_DRV_CIPHER_DATA_INFO_S  tmpData[CIPHER_MAX_LIST_NUM];
    HI_UNF_CIPHER_DATA_S *pTmp = NULL;
    HI_U32 pkgNum = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_S32 raw = 0, count = 0;

    if(NULL == pstCIData)
    {
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIPHER_CheckHandle(pstCIData->CIHandle);
    softChnId = HI_HANDLE_GET_CHNID(pstCIData->CIHandle);

    if ((g_stCipherSoftChans[softChnId].stCtrl.enWorkMode == HI_UNF_CIPHER_WORK_MODE_CCM)
        || (g_stCipherSoftChans[softChnId].stCtrl.enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM))
    {
        HI_ERR_CIPHER("Error: Multi cipher unsuport CCM/GCM mode.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pkgNum = pstCIData->u32DataLength;
    if ((pkgNum > CIPHER_MAX_LIST_NUM) || (pkgNum == 0))
    {
        HI_ERR_CIPHER("Error: Invalid pkg num(%d), must < %d and > 0.\n",pkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (pstCIData->ScrPhyAddr + pkgNum * sizeof(HI_UNF_CIPHER_DATA_S) < pstCIData->ScrPhyAddr)
    {
        HI_ERR_CIPHER("Invalid src phy addr!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (copy_from_user(g_stCipherOsrChn[softChnId].pstDataPkg, (void*)pstCIData->ScrPhyAddr,
                       pkgNum * sizeof(HI_UNF_CIPHER_DATA_S)))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < pstCIData->u32DataLength; i++)
    {
        pTmp = g_stCipherOsrChn[softChnId].pstDataPkg + i;
        if ((pTmp->u32SrcPhyAddr + pTmp->u32ByteLength < pTmp->u32ByteLength)
            || (pTmp->u32DestPhyAddr + pTmp->u32ByteLength < pTmp->u32ByteLength))
        {
            HI_ERR_CIPHER("Invalid phy addr or data lenth!\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        tmpData[i].bDecrypt = HI_TRUE;
        tmpData[i].u32src = pTmp->u32SrcPhyAddr;
        tmpData[i].u32dest = pTmp->u32DestPhyAddr;
        tmpData[i].u32length = pTmp->u32ByteLength;
    }

    HI_INFO_CIPHER("Start to DecryptMultiPkg, chnNum = %#x, pkgNum=%d!\n", softChnId, pkgNum);

#if 0
    g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;

    ret = DRV_CIPHER_CreatMultiPkgTask(softChnId, tmpData, pkgNum, HI_TRUE, softChnId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher create multi task failed!\n");
        return ret;
    }

    if (0== wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
                g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, CIPHER_TIME_OUT))
    {
        HI_ERR_CIPHER("Decrypt time out \n");
        return HI_FAILURE;
    }
#else

   HAL_Cipher_DisableInt(softChnId, CIPHER_INT_TYPE_OUT_BUF);

    ret = DRV_CIPHER_CreatMultiPkgTask(softChnId, tmpData, pkgNum, HI_TRUE, softChnId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher create multi task failed!\n");
        return ret;
    }

    while((((raw >>(softChnId + 8)) & 0x01) == 0x00) && (count++ < 0x10000000))
    {
        HAL_Cipher_GetRawIntState(&raw);
    }

    if (count < 0x10000000)
    {
        HAL_Cipher_ClrIntState(1 << (softChnId + 8));
        DRV_CipherDataDoneMultiPkg(softChnId);
    }
    else
    {
        HI_ERR_CIPHER ("Wait raw int timeout, raw = 0x%x\n", raw);
        ret = HI_FAILURE;
    }

    HAL_Cipher_EnableInt(softChnId, CIPHER_INT_TYPE_OUT_BUF);
#endif

    HI_INFO_CIPHER("Decrypt OK, chnNum = %#x!\n", softChnId);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_CIPHER_EncryptMultiEx(CIPHER_MUTIL_EX_DATA_S *pstCIDataEx)
{
    HI_U32 softChnId = 0;
    CIPHER_DATA_S stCIData;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstCIDataEx)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    CIPHER_CheckHandle(pstCIDataEx->hCipher);
    softChnId = HI_HANDLE_GET_CHNID(pstCIDataEx->hCipher);

    s32Ret = DRV_CIPHER_ConfigChn(softChnId, &pstCIDataEx->CIpstCtrl);
    if (HI_SUCCESS != s32Ret)
    {
        up(&g_CipherMutexKernel);
        return s32Ret;
    }

    memset(&stCIData, 0, sizeof(stCIData));
    stCIData.CIHandle = pstCIDataEx->hCipher;
    stCIData.ScrPhyAddr = (HI_U32)pstCIDataEx->pstPkg;
    stCIData.u32DataLength = pstCIDataEx->u32PkgNum;
    s32Ret = DRV_CIPHER_EncryptMulti(&stCIData);

    up(&g_CipherMutexKernel);

    return s32Ret;

}

HI_S32 HI_DRV_CIPHER_DecryptMultiEx(CIPHER_MUTIL_EX_DATA_S *pstCIDataEx)
{
    HI_U32 softChnId = 0;
    CIPHER_DATA_S stCIData;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstCIDataEx)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    CIPHER_CheckHandle(pstCIDataEx->hCipher);
    softChnId = HI_HANDLE_GET_CHNID(pstCIDataEx->hCipher);

    s32Ret = DRV_CIPHER_ConfigChn(softChnId, &pstCIDataEx->CIpstCtrl);
    if (HI_SUCCESS != s32Ret)
    {
        up(&g_CipherMutexKernel);
        return s32Ret;
    }

    memset(&stCIData, 0, sizeof(stCIData));
    stCIData.CIHandle = pstCIDataEx->hCipher;
    stCIData.ScrPhyAddr = (HI_U32)pstCIDataEx->pstPkg;
    stCIData.u32DataLength = pstCIDataEx->u32PkgNum;
    s32Ret = DRV_CIPHER_DecryptMulti(&stCIData);

    up(&g_CipherMutexKernel);

    return s32Ret;

}

HI_S32 HI_DRV_CIPHER_EncryptMulti(CIPHER_DATA_S *pstCIData)
{
    HI_S32 s32Ret = HI_FAILURE;

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    s32Ret = DRV_CIPHER_EncryptMulti(pstCIData);

    up(&g_CipherMutexKernel);

    return s32Ret;
}

HI_S32 HI_DRV_CIPHER_DecryptMulti(CIPHER_DATA_S *pstCIData)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(down_interruptible(&g_CipherMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    s32Ret = DRV_CIPHER_DecryptMulti(pstCIData);

    up(&g_CipherMutexKernel);

    return s32Ret;
}

HI_S32 DRV_CIPHER_Open(struct inode * inode, struct file * file)
{
    return HI_SUCCESS;
}

HI_S32 DRV_CIPHER_Release(struct inode * inode, struct file * file)
{
    HI_U32 i;

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        if (g_stCipherOsrChn[i].pWichFile == file)
        {
            DRV_CIPHER_CloseChn(i);
            g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
            g_stCipherOsrChn[i].pWichFile = NULL;
            if (g_stCipherOsrChn[i].pstDataPkg)
            {
                HI_VFREE(HI_ID_CIPHER, g_stCipherOsrChn[i].pstDataPkg);
                g_stCipherOsrChn[i].pstDataPkg = NULL;
            }
        }
    }

    return 0;
}

HI_S32 HI_DRV_CIPHER_SoftReset()
{
    HI_S32 ret = HI_FAILURE;

    (HI_VOID)HI_DRV_CIPHER_Suspend();

    ret = HI_DRV_CIPHER_Resume();
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Cipher Soft Reset failed in cipher resume!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CIPHER_ProcGetStatus(CIPHER_CHN_STATUS_S *pstCipherStatus)
{
    HI_U32 i = 0;

    if (HI_NULL == pstCipherStatus)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    for (i = 0; i < 8; i++)
    {
        if (HI_TRUE == g_stCipherSoftChans[i].bOpen)
        {
            pstCipherStatus[i].ps8Openstatus = "open ";
        }
        else
        {
            pstCipherStatus[i].ps8Openstatus = "close";
        }
    }
    return HAL_CIPHER_ProcGetStatus(pstCipherStatus);
}

HI_S32 DRV_CIPHER_CheckRsaData(HI_U8 *N, HI_U8 *E, HI_U8 *MC, HI_U32 u32Len)
{
    HI_U32 i;

    /*MC > 0*/
    for(i=0; i<u32Len; i++)
    {
        if(MC[i] > 0)
        {
            break;
        }
    }
    if(i>=u32Len)
    {
        HI_ERR_CIPHER("RSA M/C is zero, error!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*MC < N*/
    for(i=0; i<u32Len; i++)
    {
        if(MC[i] < N[i])
        {
            break;
        }
    }
    if(i>=u32Len)
    {
        HI_ERR_CIPHER("RSA M/C is larger than N, error!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*E > 1*/
    for(i=0; i<u32Len; i++)
    {
        if(E[i] > 0)
        {
            break;
        }
    }
    if(i>=u32Len)
    {
        HI_ERR_CIPHER("RSA D/E is zero, error!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

//    HI_PRINT_HEX("N", N, u32Len);
//    HI_PRINT_HEX("K", E, u32Len);

    return HI_SUCCESS;
}

HI_S32 DRV_CIPHER_ClearRsaRam(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    ret = HAL_RSA_WaitFree();
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\n");
        return HI_FAILURE;
    }
    HAL_RSA_ClearRam();
    HAL_RSA_Start();
    ret = HAL_RSA_WaitFree();
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CIPHER_CalcRsa_ex(CIPHER_RSA_DATA_S *pCipherRsaData)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32ErrorCode;
    HI_U32 u32KeyLen;
    CIPHER_RSA_KEY_WIDTH_E enKeyWidth;

    if ((pCipherRsaData == HI_NULL)
        ||(pCipherRsaData->pu8Input == HI_NULL)
        ||(pCipherRsaData->pu8Output== HI_NULL)
        || (pCipherRsaData->pu8N == HI_NULL)
        || (pCipherRsaData->pu8K == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

	if ((pCipherRsaData->u16NLen == 0) || (pCipherRsaData->u16KLen == 0))
    {
        HI_ERR_CIPHER("RSA K size is zero.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pCipherRsaData->u32DataLen != pCipherRsaData->u16NLen)
    {
        HI_ERR_CIPHER("Error, DataLen != u16NLen!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (pCipherRsaData->u16NLen <= 128)
    {
        u32KeyLen = 128;
        enKeyWidth = CIPHER_RSA_KEY_WIDTH_1K;
    }
    else if (pCipherRsaData->u16NLen <= 256)
    {
        u32KeyLen = 256;
        enKeyWidth = CIPHER_RSA_KEY_WIDTH_2K;
    }
    else if (pCipherRsaData->u16NLen <= 512)
    {
        u32KeyLen = 512;
        enKeyWidth = CIPHER_RSA_KEY_WIDTH_4K;
    }
    else
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\n", pCipherRsaData->u16NLen);
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ret = DRV_CIPHER_CheckRsaData(pCipherRsaData->pu8N, pCipherRsaData->pu8K, pCipherRsaData->pu8Input, u32KeyLen);
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA data invalid!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    ret = DRV_CIPHER_ClearRsaRam();
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA clear ram error!\n");
        return HI_FAILURE;
    }

    /*Config Mode*/
    HAL_RSA_ConfigMode(enKeyWidth);

    /*Write N,E,M*/
    HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_MODULE, pCipherRsaData->pu8N, pCipherRsaData->u16NLen, u32KeyLen);
    HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_KEY, pCipherRsaData->pu8K, pCipherRsaData->u16NLen, u32KeyLen);
    HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_CONTEXT, pCipherRsaData->pu8Input, pCipherRsaData->u16NLen, u32KeyLen);

//   HI_PRINT_HEX("M_IN", pCipherRsaData->pu8Input, u32KeyLen);

    /*Sart*/
    HAL_RSA_Start();
    ret = HAL_RSA_WaitFree();
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\n");
        return HI_FAILURE;
    }
    u32ErrorCode = HAL_RSA_GetErrorCode();
    if( 0 != u32ErrorCode )
    {
        HI_ERR_CIPHER("RSA is err: chipset error code: 0x%x!\n", u32ErrorCode);
        return HI_FAILURE;
    }

    /*Get result*/
    HAL_RSA_ReadData(pCipherRsaData->pu8Output, pCipherRsaData->u16NLen, u32KeyLen);

//    HI_PRINT_HEX("M_OUT", pCipherRsaData->pu8Output, u32KeyLen);

    return HI_SUCCESS;

}

HI_S32 DRV_CIPHER_CalcRsa(CIPHER_RSA_DATA_S *pCipherRsaData)
{
    static HI_U8  N[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    static HI_U8  K[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    static HI_U8  M[HI_UNF_CIPHER_MAX_RSA_KEY_LEN];
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32KeyLen = 0;
    CIPHER_RSA_DATA_S stCipherRsaData;
    HI_U8 *p = HI_NULL;

    if(pCipherRsaData == HI_NULL)
    {
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((pCipherRsaData->pu8Input == HI_NULL) ||(pCipherRsaData->pu8Output== HI_NULL)
        || (pCipherRsaData->pu8N == HI_NULL) || (pCipherRsaData->pu8K == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        HI_ERR_CIPHER("pu8Input:0x%p, pu8Output:0x%p, pu8N:0x%p, pu8K:0x%p\n", pCipherRsaData->pu8Input, pCipherRsaData->pu8Output, pCipherRsaData->pu8N,pCipherRsaData->pu8K);
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(pCipherRsaData->u32DataLen != pCipherRsaData->u16NLen)
    {
        HI_ERR_CIPHER("Error, DataLen != u16NLen!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pCipherRsaData->u16KLen > pCipherRsaData->u16NLen)
    {
        HI_ERR_CIPHER("Error, KLen > u16NLen!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(N, 0, sizeof(N));
    memset(K, 0, sizeof(K));
    memset(M, 0, sizeof(M));

    /*Only support the key width of 1024,2048 and 4096*/
    if (pCipherRsaData->u16NLen == 128)
    {
        u32KeyLen = 128;
    }
    else if (pCipherRsaData->u16NLen == 256)
    {
        u32KeyLen = 256;
    }
    else if (pCipherRsaData->u16NLen == 512)
    {
        u32KeyLen = 512;
    }
    else
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\n", pCipherRsaData->u16NLen);
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /*if dataLen < u32KeyLen, padding 0 before data*/
    p = N + (u32KeyLen - pCipherRsaData->u16NLen);
    if (copy_from_user(p, pCipherRsaData->pu8N, pCipherRsaData->u16NLen))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }
    p = K + (u32KeyLen - pCipherRsaData->u16KLen);
    if (copy_from_user(p, pCipherRsaData->pu8K, pCipherRsaData->u16KLen))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }
    p = M + (u32KeyLen - pCipherRsaData->u32DataLen);
    if (copy_from_user(p, pCipherRsaData->pu8Input, pCipherRsaData->u32DataLen))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }

    stCipherRsaData.pu8N = N;
    stCipherRsaData.pu8K = K;
    stCipherRsaData.pu8Input = M;
    stCipherRsaData.u16NLen = u32KeyLen;
    stCipherRsaData.u16KLen = u32KeyLen;
    stCipherRsaData.u32DataLen = u32KeyLen;
    stCipherRsaData.pu8Output = M;

	ret = DRV_CIPHER_CalcRsa_ex(&stCipherRsaData);
    if( HI_SUCCESS != ret )
    {
        return HI_FAILURE;
    }

    if (copy_to_user(pCipherRsaData->pu8Output, M+(u32KeyLen - pCipherRsaData->u16NLen),
            pCipherRsaData->u16NLen))
    {
        HI_ERR_CIPHER("copy data to user fail!\n");
        return HI_FAILURE;
    }

	return ret;
}


HI_S32 HI_DRV_CIPHER_CalcRsa(CIPHER_RSA_DATA_S *pCipherRsaData)
{
    HI_S32 ret = HI_SUCCESS;

    if(pCipherRsaData == HI_NULL)
    {
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if(down_interruptible(&g_RsaMutexKernel))
	{
		HI_ERR_CIPHER("down_interruptible failed!\n");
		return HI_FAILURE;
	}

	ret = DRV_CIPHER_CalcRsa(pCipherRsaData);

	up(&g_RsaMutexKernel);

	return ret;
}

EXPORT_SYMBOL(HI_DRV_CIPHER_CreateHandle);
EXPORT_SYMBOL(HI_DRV_CIPHER_ConfigChn);
EXPORT_SYMBOL(HI_DRV_CIPHER_DestroyHandle);
EXPORT_SYMBOL(HI_DRV_CIPHER_Encrypt);
EXPORT_SYMBOL(HI_DRV_CIPHER_Decrypt);
EXPORT_SYMBOL(HI_DRV_CIPHER_EncryptMulti);
EXPORT_SYMBOL(HI_DRV_CIPHER_DecryptMulti);
#ifdef CFG_HI_CIPHER_GETHANDLECONFIG_SUPPORT
EXPORT_SYMBOL(HI_DRV_CIPHER_GetHandleConfig);
#endif
EXPORT_SYMBOL(HI_DRV_CIPHER_SoftReset);
EXPORT_SYMBOL(HI_DRV_CIPHER_Resume);
EXPORT_SYMBOL(HI_DRV_CIPHER_Suspend);
EXPORT_SYMBOL(HI_DRV_CIPHER_CalcRsa);


