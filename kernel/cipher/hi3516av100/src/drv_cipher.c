//#include <asm/arch/hardware.h>
#include <asm/uaccess.h>
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
//#include <asm/semaphore.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <linux/interrupt.h>

#include "hi_type.h"
#include "cipher_define.h"
#include "priv_cipher.h"
#include "hal_cipher.h"
#include "drv_cipher.h"

#include "priv_cipher.h"
#include "cipher_mmz.h"
#include "cipher_log.h"

typedef struct hiCIPHER_IV_VALUE_S
{
    HI_U32    u32PhyAddr;
    HI_U32   *pu32VirAddr;
    //HI_U8   au8IVValue[CI_IV_SIZE];
} CIPHER_IV_VALUE_S;


/*
-----------------------------------------------------------
0 | input buf list Node(16Byte) | ...  * CI_MAX_LIST_NUM  | = 16*CI_MAX_LIST_NUM
-----------------------------------------------------------
  | output buf list Node(16Byte)| ...  * CI_MAX_LIST_NUM  |
-----------------------------------------------------------
  | IV (16Byte)                 | ...  * CI_MAX_LIST_NUM  |
-----------------------------------------------------------
... * 7 Channels


*/

#define CIPHER_CheckeSoftId(ChanId)   \
    do \
    { \
        if (ChanId >= CIPHER_SOFT_CHAN_NUM) \
        { \
            HI_ERR_CIPHER("chan %d is too large, max: %d\n", ChanId, CIPHER_SOFT_CHAN_NUM); \
            return HI_ERR_CIPHER_INVALID_PARA; \
        } \
        if (ChanId <= CI_PKGx1_CHAN) \
        { \
            HI_ERR_CIPHER("chan %d is too samll, min: %d\n", ChanId, CI_PKGxN_CHAN_MIN); \
            return HI_ERR_CIPHER_INVALID_PARA; \
        } \
        if (HI_FALSE == g_stCipherOsrChn[ChanId].g_bSoftChnOpen) \
        { \
            HI_ERR_CIPHER("chan %d is not open\n", ChanId); \
            return HI_ERR_CIPHER_INVALID_HANDLE; \
        } \
    } while (0)

#define CIPHER_CheckHandle(CIHandle)   \
    do \
    { \
        if ((CIHandle >> CIPHER_ID_OFFSET) != HI_ID_CIPHER)\
        {\
            HI_ERR_CIPHER("CIHandle(%d) is not a valid cipher handle.\n",CIHandle);\
            return HI_ERR_CIPHER_INVALID_HANDLE; \
        }\
        CIPHER_CheckeSoftId((CIHandle & GET_CIPHER_SOFT_HANDLE));\
    } while (0)

typedef struct hiCIPHER_PKGN_MNG_S
{
    HI_U32              u32TotalPkg;  /*  */
    HI_U32              u32CurrentPtr;
    HI_U32              u32BusyCnt;
    HI_U32              u32FreeCnt;
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
    CIPHER_IV_VALUE_S       astCipherIVValue[CI_MAX_LIST_NUM];
    HI_U32                  au32WitchSoftChn[CI_MAX_LIST_NUM];
    HI_U32                  au32CallBackArg[CI_MAX_LIST_NUM];

    CIPHER_DATA_MNG_U       unInData;
    CIPHER_DATA_MNG_U       unOutData;
} CIPHER_CHAN_S;



/********************** Global Variable declaration **************************/



typedef struct hiCIPHER_COMM_S
{
    CI_MMZ_BUF_S        stPhyBuf;
}CIPHER_COMM_S;

#ifdef TEST_CIPHER
extern HI_U32 g_u32CipherStartCase;
extern HI_U32 g_u32CipherEndCase;
extern HI_S32 CIPHER_Test_Run(HI_U32 beginNo, HI_U32 endNo);
#endif

int cipher_irq = -1;

static CIPHER_COMM_S g_stCipherComm;
static CIPHER_CHAN_S g_stCipherChans[CI_CHAN_NUM];
static CIPHER_SOFTCHAN_S g_stCipherSoftChans[CIPHER_SOFT_CHAN_NUM];
extern CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];
HI_S32 DRV_CipherInitHardWareChn(HI_U32 chnId )
{
    if (CI_PKGxN_CHAN_MAX < chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HAL_Cipher_SetInBufNum(chnId, CI_MAX_LIST_NUM);
    HAL_Cipher_SetInBufCnt(chnId, 0);

    HAL_Cipher_SetOutBufNum(chnId, CI_MAX_LIST_NUM);
    HAL_Cipher_SetOutBufCnt(chnId, CI_MAX_LIST_NUM);

    HAL_Cipher_SetAGEThreshold(chnId, CIPHER_INT_TYPE_OUT_BUF, 0);
    HAL_Cipher_SetAGEThreshold(chnId, CIPHER_INT_TYPE_IN_BUF, 0);

    HAL_Cipher_DisableInt(chnId, CIPHER_INT_TYPE_OUT_BUF | CIPHER_INT_TYPE_IN_BUF);

    return HI_SUCCESS;
}

HI_S32 DRV_CipherDeInitHardWareChn(HI_U32 chnId)
{
/*
    HAL_Cipher_SetInBufNum(CI_MAX_LIST_NUM);
    HAL_Cipher_SetInBufCnt(0);
    HAL_Cipher_SetInBufEmpty(CI_MAX_LIST_NUM);

    HAL_Cipher_SetOutBufNum(CI_MAX_LIST_NUM);
    HAL_Cipher_SetOutBufCnt(CI_MAX_LIST_NUM);
    HAL_Cipher_SetOutBufFull(0);
*/
    if (CI_PKGxN_CHAN_MAX < chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HAL_Cipher_DisableInt(chnId, CIPHER_INT_TYPE_OUT_BUF | CIPHER_INT_TYPE_IN_BUF);
    return HI_SUCCESS;
}
/*
set interrupt threshold level and enable it, and flag soft channel opened
*/
HI_S32 DRV_Cipher_OpenChn(HI_U32 softChnId)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;

    CIPHER_CheckeSoftId(softChnId);

    pSoftChan = &g_stCipherSoftChans[softChnId];
    pSoftChan->u32HardWareChn = softChnId;

    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];

    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_OUT_BUF, CIPHER_DEFAULT_INT_NUM);
    ret = HAL_Cipher_EnableInt(pChan->chnId, CIPHER_INT_TYPE_OUT_BUF);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Cipher_EnableInt failed!\n");
        return HI_FAILURE;
    }

    ret = CI_MmzAlloc("CIPHER_LAST_BLOCK", NULL, AES_BLOCK_SIZE*2, 0, &pSoftChan->sMLastBlock);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Error: new phyaddr for last block failed!\n");
        return HI_FAILURE;
    }

    pSoftChan->bOpen = HI_TRUE;
    return ret;
}

/*
flag soft channel closed
*/
HI_S32 DRV_Cipher_CloseChn(HI_U32 softChnId)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SOFTCHAN_S *pSoftChan;

    if (CI_PKGxN_CHAN_MAX < softChnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pSoftChan = &g_stCipherSoftChans[softChnId];

    pSoftChan->bOpen = HI_FALSE;

    CI_MmzFree(&pSoftChan->sMLastBlock);

    return ret;
}

static HI_S32 Check_Cipher_Config(HI_UNF_CIPHER_CTRL_S *pConfig)
{
    if (HI_NULL == pConfig)
    {
        HI_ERR_CIPHER("pConfig is null!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((HI_UNF_CIPHER_ALG_BUTT <= pConfig->enAlg)
       ||(HI_UNF_CIPHER_WORK_MODE_BUTT <= pConfig->enWorkMode) 
       ||(HI_UNF_CIPHER_BIT_WIDTH_128BIT < pConfig->enBitWidth)
       ||(HI_UNF_CIPHER_KEY_DES_2KEY < pConfig->enKeyLen))
    {
        HI_ERR_CIPHER("alg-%d, mode-%d, width-%d, keylen-%d, by_ca-%d\n",
            pConfig->enAlg, pConfig->enWorkMode, pConfig->enBitWidth,
            pConfig->enKeyLen, pConfig->bKeyByCA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((HI_UNF_CIPHER_ALG_3DES == pConfig->enAlg)
         && (HI_UNF_CIPHER_KEY_DES_2KEY != pConfig->enKeyLen)
         && (HI_UNF_CIPHER_KEY_DES_3KEY != pConfig->enKeyLen))
    {
         HI_ERR_CIPHER("3DES with invalid key len\n");
         return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((HI_UNF_CIPHER_ALG_3DES == pConfig->enAlg)
        && (HI_FALSE == pConfig->bKeyByCA))
    {
        if (0 == memcmp(&pConfig->u32Key[0], &pConfig->u32Key[2], 8))
        {
            HI_ERR_CIPHER("Invalid 3DES key, k1 = k2 \n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        if (HI_UNF_CIPHER_KEY_DES_3KEY == pConfig->enKeyLen)
        {
            if (0 == memcmp(&pConfig->u32Key[2], &pConfig->u32Key[4], 8))
            {
                HI_ERR_CIPHER("Invalid 3DES key, k2 = k3 \n");
                return HI_ERR_CIPHER_INVALID_PARA;
            }
        }
    }

    if (((HI_UNF_CIPHER_ALG_DES == pConfig->enAlg)
        || (HI_UNF_CIPHER_ALG_3DES == pConfig->enAlg))
        && ((HI_UNF_CIPHER_WORK_MODE_CFB == pConfig->enWorkMode)
        || (HI_UNF_CIPHER_WORK_MODE_OFB == pConfig->enWorkMode)))
    {
        if ((HI_UNF_CIPHER_BIT_WIDTH_1BIT != pConfig->enBitWidth)
            && (HI_UNF_CIPHER_BIT_WIDTH_8BIT != pConfig->enBitWidth)
            && (HI_UNF_CIPHER_BIT_WIDTH_64BIT != pConfig->enBitWidth)
            )
        {
            HI_ERR_CIPHER("DES/3DES with CFB/OFB just support 1/8/64 bit width.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    if((HI_UNF_CIPHER_ALG_AES == pConfig->enAlg)
        && (HI_UNF_CIPHER_KEY_AES_256BIT < pConfig->enKeyLen))
    {
         HI_ERR_CIPHER("AES with invalid key len\n");
         return HI_ERR_CIPHER_INVALID_PARA;
    }
    if (HI_UNF_CIPHER_ALG_AES == pConfig->enAlg)
    {
        if ((HI_UNF_CIPHER_WORK_MODE_CFB == pConfig->enWorkMode)
            && (HI_UNF_CIPHER_BIT_WIDTH_1BIT != pConfig->enBitWidth)
            && (HI_UNF_CIPHER_BIT_WIDTH_8BIT != pConfig->enBitWidth)
            && (HI_UNF_CIPHER_BIT_WIDTH_128BIT != pConfig->enBitWidth))
        {
            HI_ERR_CIPHER("AES with CFB just support 1/8/128 bit width.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((HI_UNF_CIPHER_WORK_MODE_OFB == pConfig->enWorkMode)
            && (HI_UNF_CIPHER_BIT_WIDTH_128BIT != pConfig->enBitWidth))
        {
            HI_ERR_CIPHER("AES with OFB just support 128 bit width.\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DRV_Cipher_ConfigChn(HI_U32 softChnId,  HI_UNF_CIPHER_CTRL_S *pConfig,
                            funcCipherCallback fnCallBack)
{
    HI_S32 ret = HI_FAILURE;
    HI_BOOL bDecrypt = HI_FALSE;
    HI_BOOL bIVSet = HI_FALSE;
    CIPHER_CHAN_S *pChan = HI_NULL;
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;

    CIPHER_CheckeSoftId(softChnId);

    ret = Check_Cipher_Config(pConfig);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Check_Cipher_Config failed.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pSoftChan = &g_stCipherSoftChans[softChnId];
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];
    pSoftChan->pfnCallBack = fnCallBack;
    HI_DEBUG_CIPHER("softChnID=%d",softChnId);

    ret = HAL_Cipher_Config(pChan->chnId, bDecrypt, bIVSet, pConfig);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Cipher_Config failed.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pSoftChan->bIVChange = HI_TRUE;
    pSoftChan->bKeyChange = HI_TRUE;

    memcpy(&(pSoftChan->stCtrl), pConfig, sizeof(HI_UNF_CIPHER_CTRL_S));

    return ret;
}

HI_S32 DRV_CipherStartSinglePkgChn(HI_U32 softChnId, HI_UNF_CIPHER_INFO_S *pBuf2Process)
{
    HI_U32 ret = HI_FAILURE;
    CIPHER_CHAN_S *pChan = HI_NULL;
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;

    CIPHER_CheckeSoftId(softChnId);

    pSoftChan = &g_stCipherSoftChans[softChnId];
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];

    //printk("[%s] softchannel ID = %d\n", __func__, softChnId);

    #if 0
    if ( HI_FALSE == HAL_Cipher_IsIdle(0) )
    {
        HI_DEBUG_CIPHER("start single packet failure as the channel is busy!\n");
        return HI_FAILURE;
    }
    #endif

    HAL_Cipher_Config(0, pBuf2Process->bDecrypt, HI_FALSE, &(pSoftChan->stCtrl));
    HAL_Cipher_SetInIV(0, &(pSoftChan->stCtrl));
    HAL_Cipher_SetKey(0, &(pSoftChan->stCtrl));
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

HI_S32 DRV_CipherStartMultiPkgChn(HI_U32 softChnId, HI_UNF_CIPHER_INFO_S *pBuf2Process, HI_U32 callBackArg)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 hardWareChn;
    HI_U32 BusyCnt = 0;
    HI_U32 currentPtr;
    CI_BUF_LIST_ENTRY_S *pInBuf;
    CI_BUF_LIST_ENTRY_S *pOutBuf;

    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;

    CIPHER_CheckeSoftId(softChnId);

    pSoftChan = &g_stCipherSoftChans[softChnId];
    hardWareChn = pSoftChan->u32HardWareChn;
    pChan = &g_stCipherChans[hardWareChn];


    HAL_Cipher_GetInBufCnt(hardWareChn, &BusyCnt);
    HI_DEBUG_CIPHER("HAL_Cipher_GetInBufCnt, BusyCnt=%d.\n", BusyCnt);

    pChan->unInData.stPkgNMng.u32BusyCnt = BusyCnt;
    currentPtr = pChan->unInData.stPkgNMng.u32CurrentPtr;

    pInBuf = pChan->pstInBuf + currentPtr;
    pOutBuf = pChan->pstOutBuf + currentPtr;

    if (BusyCnt < CI_MAX_LIST_NUM) /* */
    {
        /* set addr */
        pInBuf->u32DataAddr = pBuf2Process->u32src;
        pInBuf->U32DataLen = pBuf2Process->u32length;

        pOutBuf->u32DataAddr = pBuf2Process->u32dest;
        pOutBuf->U32DataLen = pBuf2Process->u32length;

        /* set IV */
        if (pSoftChan->bIVChange)
        {
            memcpy(pChan->astCipherIVValue[currentPtr].pu32VirAddr,
                pSoftChan->stCtrl.u32IV, CI_IV_SIZE);

            pInBuf->u32IVStartAddr
                = pChan->astCipherIVValue[currentPtr].u32PhyAddr;

            pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_IVSET_BIT);
        }
        else
        {
            pInBuf->u32Flags &= ~(1 << CI_BUF_LIST_FLAG_IVSET_BIT);
        }


        /* set Key */
        if (pSoftChan->bKeyChange)
        {
            HAL_Cipher_SetKey(hardWareChn, &(pSoftChan->stCtrl));
            pSoftChan->bKeyChange = HI_FALSE;
        }
        else
        {
            ;
        }

        /* just set each node to End_of_list */
        pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_EOL_BIT);
        pOutBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_EOL_BIT);

        ret = HAL_Cipher_Config(hardWareChn, pBuf2Process->bDecrypt, pSoftChan->bIVChange, &(pSoftChan->stCtrl));
        pSoftChan->bIVChange = HI_FALSE;

        pChan->au32WitchSoftChn[currentPtr] = softChnId;
        pChan->au32CallBackArg[currentPtr] = callBackArg;
        pSoftChan->u32PirvateData = callBackArg;

        currentPtr++;
        if (currentPtr >=  CI_MAX_LIST_NUM)
        {
            currentPtr = 0;
        }

        /* save list Node */
        pChan->unInData.stPkgNMng.u32CurrentPtr = currentPtr;
        pChan->unInData.stPkgNMng.u32TotalPkg++;
        pChan->unOutData.stPkgNMng.u32TotalPkg++;


        HAL_Cipher_GetOutBufCnt(hardWareChn, &BusyCnt);

        HAL_Cipher_SetInBufCnt(hardWareChn, 1); /* +1 */

        HI_INFO_CIPHER("%s %#x->%#x, LEN:%#x", pBuf2Process->bDecrypt ? "Dec" : "ENC",
                pBuf2Process->u32src, pBuf2Process->u32dest,
                pBuf2Process->u32length );
    }
    else
    {
        return HI_FAILURE;
    }

    return ret;
}

HI_S32 DRV_Cipher_CreatTask(HI_U32 softChnId, CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV)
{
    HI_S32 ret = HI_FAILURE;
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;

    CIPHER_CheckeSoftId(softChnId);

    pSoftChan = &g_stCipherSoftChans[softChnId];

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

    if (CI_PKGx1_CHAN == pSoftChan->u32HardWareChn)
    {
        ret = DRV_CipherStartSinglePkgChn(softChnId, &(pTask->stData2Process));

    }
    else
    {
        ret = DRV_CipherStartMultiPkgChn(softChnId, &(pTask->stData2Process), pTask->u32CallBackArg);
    }

    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("can't create task, ERR=%#x.\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}


static HI_S32 CIPHER_LengthCheck(HI_UNF_CIPHER_CTRL_S *pstCtrl, HI_U32 u32ByteLength)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32S = 0;
    HI_U32 u32B = 0;
    HI_U32 u32BlockSize = 0;

    u32S = u32ByteLength % AES_BLOCK_SIZE;
    if (u32S == 0)
    {
        return HI_SUCCESS;
    }

    switch (pstCtrl->enAlg)
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

    switch (pstCtrl->enBitWidth)
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
            HI_ERR_CIPHER("Error: BitWidth(0x%x)!\n", pstCtrl->enBitWidth);
            return HI_ERR_CIPHER_INVALID_PARA;
    }

    switch (pstCtrl->enWorkMode)
    {
        case HI_UNF_CIPHER_WORK_MODE_ECB:
        case HI_UNF_CIPHER_WORK_MODE_CBC:
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
        default:
            HI_ERR_CIPHER("Error: Alg invalid!\n");
            Ret = HI_ERR_CIPHER_INVALID_PARA;
    }

    return Ret;
}

HI_S32 DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData, HI_BOOL bIsDescrypt)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 softChnId = 0;
    CIPHER_TASK_S stCITask;

    if (0 == pstCIData->ByteLength)
    {
        return HI_SUCCESS;
    }

    memset(&stCITask, 0, sizeof(stCITask));
    CIPHER_CheckHandle(pstCIData->CIHandle);
    softChnId = pstCIData->CIHandle & GET_CIPHER_SOFT_HANDLE;

    stCITask.stData2Process.u32src = pstCIData->ScrPhyAddr;
    stCITask.stData2Process.u32dest = pstCIData->DestPhyAddr;
    stCITask.stData2Process.u32length = pstCIData->ByteLength;
    stCITask.stData2Process.bDecrypt = bIsDescrypt;
    stCITask.u32CallBackArg = softChnId;

    HI_INFO_CIPHER("Start to Encrypt, chnNum = %#x!\n", softChnId);

    g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;

    ret = DRV_Cipher_CreatTask(softChnId,&stCITask, NULL, NULL);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    if (0 == wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
                g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
    {
        HI_ERR_CIPHER("Encrypt time out! \n");
        return HI_FAILURE;
    }
    HI_INFO_CIPHER("Encrypt OK, chnNum = %#x!\n", softChnId);
    return HI_SUCCESS;

}

HI_S32 DRV_CIPHER_EncryptEx(CIPHER_DATA_S *pstCIData, HI_BOOL bIsDescrypt)
{
    HI_S32 Ret = HI_FAILURE;
    CIPHER_DATA_S CIdata;
    HI_U8 *u32OutPutAddrVir = HI_NULL;
    HI_U8 *u32InPutAddrVir = HI_NULL;
    HI_U32 u32S = 0;
    HI_U32 u32SoftChanId = 0;
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;
    CI_MMZ_BUF_S *psMLastBlock = HI_NULL;

    CIPHER_CheckHandle(pstCIData->CIHandle);
    u32SoftChanId = pstCIData->CIHandle & GET_CIPHER_SOFT_HANDLE;

    memset(&CIdata, 0, sizeof(CIdata));

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    psMLastBlock = &pSoftChan->sMLastBlock;

    Ret = CIPHER_LengthCheck(&(pSoftChan->stCtrl), pstCIData->ByteLength);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Error: CIPHER Length Check failed!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    u32S = pstCIData->ByteLength % AES_BLOCK_SIZE;
    if(0 < u32S)
    {
        u32OutPutAddrVir = (HI_U8 *)CI_MMZ_Map(pstCIData->DestPhyAddr, (pstCIData->ByteLength+3)&(~0x03));
        if (0 == u32OutPutAddrVir)
        {
            HI_ERR_CIPHER("Error: Get Viaaddr for outPut failed!\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        u32InPutAddrVir = (HI_U8 *)CI_MMZ_Map(pstCIData->ScrPhyAddr, (pstCIData->ByteLength+3)&(~0x03));
        if (0 == u32InPutAddrVir)
        {
            HI_ERR_CIPHER("Error: Get Viaaddr for inPut failed!\n");
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        memset((HI_U8*)psMLastBlock->u32StartVirAddr, 0, AES_BLOCK_SIZE*2);
        memcpy((HI_U8*)psMLastBlock->u32StartVirAddr, u32InPutAddrVir+pstCIData->ByteLength-u32S, u32S);
        pstCIData->ByteLength-=u32S;
    }

    Ret = DRV_CIPHER_Encrypt(pstCIData, bIsDescrypt);
    if((HI_SUCCESS == Ret) && (0 < u32S))
    {
        CIdata.ScrPhyAddr=psMLastBlock->u32StartPhyAddr;
        CIdata.DestPhyAddr=psMLastBlock->u32StartPhyAddr;
        CIdata.ByteLength=AES_BLOCK_SIZE;
        CIdata.CIHandle = pstCIData->CIHandle;
        Ret = DRV_CIPHER_Encrypt(&CIdata, bIsDescrypt);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }
        memcpy(u32OutPutAddrVir+pstCIData->ByteLength, (HI_U8*)psMLastBlock->u32StartVirAddr, u32S);
        pstCIData->ByteLength+=u32S;
    }

    if(HI_NULL != u32InPutAddrVir)
    {
        CI_MMZ_Unmap((HI_U32)u32InPutAddrVir);
    }

    if(HI_NULL != u32OutPutAddrVir)
    {
        CI_MMZ_Unmap((HI_U32)u32OutPutAddrVir);
    }

    return HI_SUCCESS;
}


HI_S32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData)
{
	HI_S32 ret = HI_FAILURE;

	ret = DRV_CIPHER_EncryptEx(pstCIData, HI_FALSE);

	return ret;
}

HI_S32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData)
{
	HI_S32 ret = HI_FAILURE;

	ret = DRV_CIPHER_EncryptEx(pstCIData, HI_TRUE);

	return ret;
}

HI_S32 DRV_CipherDataDoneSinglePkg(HI_U32 chnId)
{
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;

    CIPHER_CheckeSoftId(chnId);

    HI_DEBUG_CIPHER("Data DONE, hwChn:%d\n", chnId);
    pSoftChan = &g_stCipherSoftChans[chnId];

    if (pSoftChan->pfnCallBack)
    {
        pSoftChan->pfnCallBack(pSoftChan->u32PirvateData);
    }
    return HI_SUCCESS;
}

HI_S32 DRV_CipherDataDoneMultiPkg(HI_U32 chnId)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 currentPtr = 0;
    HI_U32 softChnId = 0;
    HI_U32 fullCnt = 0;
    HI_U32 i = 0, idx = 0;
    CIPHER_CHAN_S *pChan = HI_NULL;
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;

    CIPHER_CheckeSoftId(chnId);

    pChan = &g_stCipherChans[chnId];
    HI_DEBUG_CIPHER("Data DONE, hwChn:%d\n", chnId);

    currentPtr = pChan->unOutData.stPkgNMng.u32CurrentPtr;

    HI_DEBUG_CIPHER("Data DONE, hwChn:%u, currentPtr=%u\n", chnId, currentPtr);

    /* get the finished output data buffer count */
    ret = HAL_Cipher_GetOutBufFull(chnId, &fullCnt);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    idx = currentPtr;

    if(CI_MAX_LIST_NUM <= idx)
    {
        HI_ERR_CIPHER("idx error: idx=%u, chnId=%d \n", idx, chnId);
		return HI_FAILURE;
    }

    if (0 < fullCnt)
    {
        for (i = 0; i < fullCnt; i++)
        {
            softChnId = pChan->au32WitchSoftChn[idx];
            pChan->au32WitchSoftChn[idx] = CIPHER_INVALID_CHN;

            pSoftChan = &g_stCipherSoftChans[softChnId];
            HI_DEBUG_CIPHER("softChnId=%d, idx=%u\n", softChnId, idx);
            if (pSoftChan->pfnCallBack)
            {
                HI_DEBUG_CIPHER("CallBack function\n");
                pSoftChan->pfnCallBack(pSoftChan->u32PirvateData);
            }

            idx++;
            if (idx >= CI_MAX_LIST_NUM)
            {
                idx = 0;
            }
        }

        pChan->unOutData.stPkgNMng.u32CurrentPtr = idx;
        HAL_Cipher_SetInBufEmpty(chnId, fullCnt);  /* -  */
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
    HI_U32 i = 0;
    HI_U32 INTValue = 0;

    HAL_Cipher_GetIntState(&INTValue);
    HAL_Cipher_ClrIntState(INTValue);

    HI_DEBUG_CIPHER(" in the isr INTValue=%#x!\n", INTValue);

    if (INTValue >> 8 & 0x1) /* single pkg */
    {
        DRV_CipherDataDoneSinglePkg(0);
    }

    for(i = 1; i < CI_CHAN_NUM; i++)
    {
        if ((INTValue >> (i+8)) & 0x1)
        {
            DRV_CipherDataDoneMultiPkg(i);
        }
    }
    return IRQ_HANDLED;
}

HI_S32 DRV_Cipher_Init(HI_VOID)
{
    HI_U32 i = 0,j = 0, hwChnId = 0;
    HI_S32 ret = HI_FAILURE;
    HI_U32 bufSizeChn = 0; /* all the buffer list size, included data buffer size and IV buffer size */
    HI_U32 databufSizeChn = 0; /* max list number data buffer size */
    HI_U32 ivbufSizeChn = 0; /* all the list IV size */
    HI_U32 bufSizeTotal = 0; /* all the channel buffer size */
    CI_MMZ_BUF_S   cipherListBuf;
    CIPHER_CHAN_S *pChan = HI_NULL;

    memset(&g_stCipherComm, 0, sizeof(g_stCipherComm));
    memset(&g_stCipherChans, 0, sizeof(g_stCipherChans));
    memset(&g_stCipherSoftChans, 0, sizeof(g_stCipherSoftChans));
    memset(&cipherListBuf, 0, sizeof(CI_MMZ_BUF_S));

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

    databufSizeChn = sizeof(CI_BUF_LIST_ENTRY_S) * CI_MAX_LIST_NUM;
    ivbufSizeChn = CI_IV_SIZE * CI_MAX_LIST_NUM;
    bufSizeChn = (databufSizeChn * 2) + ivbufSizeChn;/* inBuf + outBuf + keyBuf */
    bufSizeTotal = bufSizeChn * (CI_PKGxN_CHAN_MAX - CI_PKGxN_CHAN_MIN + 1) ; /* only 7 channels need buf */

    HAL_Cipher_Init();
    HAL_Cipher_DisableAllInt();

    //printk("bufSizeTotal %d\n",bufSizeTotal);

    /* allocate 7 channels size */
    ret = CI_MmzAlloc("cipherCfg",NULL, bufSizeTotal, 0, &(cipherListBuf));
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Can NOT get mem for cipher, init failed, exit...\n");
        return HI_FAILURE;
    }
    else
    {
        memset((void*)(cipherListBuf.u32StartVirAddr), 0, cipherListBuf.u32Size);

        /* save the whole memory info, included physical address, virtual address and their size */
        memcpy(&(g_stCipherComm.stPhyBuf), &(cipherListBuf), sizeof(g_stCipherComm.stPhyBuf));
    }

    HI_DEBUG_CIPHER("TOTAL BUF: %#x/%#x", cipherListBuf.u32StartPhyAddr, cipherListBuf.u32StartVirAddr);

    /* assign hardware channel ID from 0 to 7 */
    for (i = 0; i <= CI_PKGxN_CHAN_MAX; i++)
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
/\                                        /\                                        /\
 |              IV buf                  |             IN buf                    |             OUT buf
startPhyAddr
==============================================================
|
|
==============================================================
/\                                        /\                                        /\
 |              IV buf                  |             IN buf                    |             OUT buf
 startVirAddr
*/
    for (i = 0; i < CI_PKGxN_CHAN_MAX; i++)
    {
        /* config channel from 1 to 7 */
        hwChnId = i+CI_PKGxN_CHAN_MIN;
        pChan = &g_stCipherChans[hwChnId];

        pChan->astCipherIVValue[0].u32PhyAddr
            = cipherListBuf.u32StartPhyAddr + (i * bufSizeChn);
        pChan->astCipherIVValue[0].pu32VirAddr
            = (HI_U32*)(cipherListBuf.u32StartVirAddr + (i * bufSizeChn));

        for (j = 1; j < CI_MAX_LIST_NUM; j++)
        {
            pChan->astCipherIVValue[j].u32PhyAddr
                = pChan->astCipherIVValue[0].u32PhyAddr + (CI_IV_SIZE * j);
            pChan->astCipherIVValue[j].pu32VirAddr
                = (HI_U32*)(((HI_U32)pChan->astCipherIVValue[0].pu32VirAddr) + (CI_IV_SIZE * j));
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
    for (i = 0; i < CI_PKGxN_CHAN_MAX; i++)
    {
        hwChnId = i+CI_PKGxN_CHAN_MIN;
        pChan = &g_stCipherChans[hwChnId];

        HI_DEBUG_CIPHER("Chn%02x, IV:%#x/%p In:%#x/%p, Out:%#x/%p.\n", i,
            pChan->astCipherIVValue[0].u32PhyAddr,
            pChan->astCipherIVValue[0].pu32VirAddr,
            pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn, pChan->pstInBuf,
            pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn + databufSizeChn, pChan->pstOutBuf );
    }

    HAL_Cipher_ClrIntState(0xffffffff);

    /* request irq */
    ret = request_irq(cipher_irq, DRV_Cipher_ISR, 0, "CI_IRQ", NULL);
    if(HI_SUCCESS != ret)
    {
        HAL_Cipher_DisableAllInt();
        HAL_Cipher_ClrIntState(0xffffffff);

        HI_ERR_CIPHER("Irq request failure, ret=%#x.", ret);
        CI_MmzFree(&(g_stCipherComm.stPhyBuf));
        return HI_FAILURE;
    }

#ifdef TEST_CIPHER
    i = 0;
    while (i++ < 10000)
    {
        printk("============i: %d==============\n", i);
        CIPHER_Test_Run(g_u32CipherStartCase, g_u32CipherEndCase);
    }
#endif

    return HI_SUCCESS;
}

HI_VOID DRV_Cipher_DeInit(HI_VOID)
{
    HI_U32 i = 0, hwChnId = 0;
//    HI_U32  REG_SYS_BASE_ADDR_VIRT = 0;

    HAL_Cipher_DisableAllInt();
    HAL_Cipher_ClrIntState(0xffffffff);

    for (i = 0; i < CI_PKGxN_CHAN_MAX; i++)
    {
        hwChnId = i+CI_PKGxN_CHAN_MIN;
        DRV_CipherDeInitHardWareChn(hwChnId);
    }

    /* free irq */
    free_irq(cipher_irq, NULL);

    CI_MmzFree(&(g_stCipherComm.stPhyBuf));

#if 0
    REG_SYS_BASE_ADDR_VIRT = (HI_U32)ioremap_nocache(REG_SYS_BASE_ADDR_PHY, 0x1000);
    *((volatile unsigned int *)(REG_SYS_BASE_ADDR_VIRT+0xb4)) = 0x00000103;
    msleep(100);
    *((volatile unsigned int *)(REG_SYS_BASE_ADDR_VIRT+0xb4)) = 0x00000100;
    iounmap((void *)REG_SYS_BASE_ADDR_VIRT);
#endif

    HAL_Cipher_DeInit();

    return;
}


/* support powerdown step 0 */
HI_VOID  DRV_Cipher_Suspend(HI_VOID)
{
    return;
}

/* support powerdown step 0,reset cipher module! */
HI_VOID DRV_Cipher_Resume(HI_VOID)
{
    HI_U32 i = 0;
    HI_S32 ret = HI_FAILURE;
    HI_U32 hwChnId = 0;
    HI_U32 ivbufSizeChn = 0;
    HI_U32 databufSizeChn = 0;
    CIPHER_CHAN_S *pChan = HI_NULL;

    databufSizeChn = sizeof(CI_BUF_LIST_ENTRY_S) * CI_MAX_LIST_NUM;
    ivbufSizeChn = CI_IV_SIZE * CI_MAX_LIST_NUM;

    HAL_Cipher_DisableAllInt();
    for (i = 0; i < CI_PKGxN_CHAN_MAX; i++)
    {
        hwChnId = i+CI_PKGxN_CHAN_MIN;
        pChan = &g_stCipherChans[hwChnId];

        pChan->pstInBuf = (CI_BUF_LIST_ENTRY_S*)((HI_U32)(pChan->astCipherIVValue[0].pu32VirAddr) + ivbufSizeChn);
        pChan->pstOutBuf = (CI_BUF_LIST_ENTRY_S*)((HI_U32)(pChan->pstInBuf) + databufSizeChn);

        HAL_Cipher_SetBufAddr(hwChnId, CIPHER_BUF_TYPE_IN, pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn);
        HAL_Cipher_SetBufAddr(hwChnId, CIPHER_BUF_TYPE_OUT, pChan->astCipherIVValue[0].u32PhyAddr + ivbufSizeChn + databufSizeChn);

        DRV_CipherInitHardWareChn(hwChnId);
    }

    HAL_Cipher_GetIntState(&ret);
    HAL_Cipher_DisableAllInt();


    HAL_Cipher_ClrIntState(0xffffffff);

    return;
}

EXPORT_SYMBOL(DRV_Cipher_OpenChn);
EXPORT_SYMBOL(DRV_Cipher_CloseChn);
EXPORT_SYMBOL(DRV_Cipher_ConfigChn);
EXPORT_SYMBOL(DRV_Cipher_CreatTask);

EXPORT_SYMBOL(HAL_Cipher_GetIntState);
EXPORT_SYMBOL(HAL_Cipher_GetIntEnState);


