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

#ifndef __DRV_CIPHER_H__
#define __DRV_CIPHER_H__

/* add include here */
#include "hal_cipher.h"
#include "drv_cipher_ioctl.h"
#include "drv_osal_lib.h"
#include "drv_cipher_log.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct hiCIPHER_COMM_S
{
    MMZ_BUFFER_S        stChainedListPhyBuf;
    MMZ_BUFFER_S        stTempPhyBuf;
#ifdef CIPHER_HASHVIR_SUPPORT
    MMZ_BUFFER_S        stTempCalcHashBuf;
#endif
    MMZ_BUFFER_S        stOutPhyBuf;
} CIPHER_COMM_S;

typedef struct hiCIPHER_OSR_CHN_S
{
    HI_BOOL g_bSoftChnOpen;                 /* mark soft channel open or not*/
    HI_BOOL g_bDataDone;                    /* mark the data done or not */
    crypto_wait_t cipher_wait_queue;       /* mutex method */
    HI_VOID *pWichFile;                    /* which file need to operate */

    HI_UNF_CIPHER_DATA_S *pstDataPkg;
    HI_U32                u32DataPkgNum;
}CIPHER_OSR_CHN_S;

typedef HI_VOID (*funcCipherCallback)(HI_U32);

/***************************** Macro Definition ******************************/
#define CIPHER_DEFAULT_INT_NUM    1
#define CIPHER_SOFT_CHAN_NUM      CIPHER_CHAN_NUM
#define CIPHER_INVALID_CHN        (0xffffffff)

HI_S32 DRV_CIPHER_ReadReg(HI_U32 addr, HI_U32 *pVal);
HI_S32 DRV_CIPHER_WriteReg(HI_U32 addr, HI_U32 Val);
HI_S32 DRV_CIPHER_Init(HI_VOID);
HI_VOID DRV_CIPHER_DeInit(HI_VOID);

#ifdef __HuaweiLite__
HI_S32 DRV_CIPHER_Open(HI_VOID *private_data);
HI_S32 DRV_CIPHER_Release(HI_VOID *private_data);
#else
HI_S32 DRV_CIPHER_Open(struct inode * inode, struct file * file);
HI_S32 DRV_CIPHER_Release(struct inode * inode, struct file * file);
#endif

HI_S32 DRV_CIPHER_OpenChn(HI_U32 softChnId);
HI_S32 DRV_CIPHER_CloseChn(HI_U32 softChnId);
HI_S32 DRV_CIPHER_CreatTask(HI_U32 softChnId, HI_DRV_CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV);
HI_S32 DRV_CIPHER_CreatMultiPkgTask(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process,
                                    HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg);
HI_S32 DRV_CIPHER_ProcGetStatus(CIPHER_CHN_STATUS_S *pstCipherStatus);
HI_S32 DRV_CIPHER_CreateHandle(CIPHER_HANDLE_S * pstCIHandle, HI_VOID *file);
HI_S32 DRV_CIPHER_ConfigChn(HI_U32 softChnId,HI_UNF_CIPHER_CTRL_S * pConfig);
HI_S32 DRV_CIPHER_GetHandleConfig(CIPHER_Config_CTRL *pstCipherConfig);
HI_S32 DRV_CIPHER_Encrypt(HI_U32 softChnId, HI_U32 ScrPhyAddr,HI_U32 DestPhyAddr,
                          HI_U32 u32DataLength, HI_BOOL bIsDescrypt);
HI_S32 DRV_CIPHER_EncryptEx(HI_U32 u32SoftChanId, HI_U32 ScrPhyAddr,HI_U32 DestPhyAddr,
                            HI_U32 u32DataLength, HI_BOOL bIsDescrypt, HI_U8 *pLastBlock);

#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 DRV_CIPHER_CCM_Aad(HI_U32 u32SoftChanId);
HI_S32 DRV_CIPHER_GCM_Aad(HI_U32 u32SoftChanId);
HI_S32 DRV_CIPHER_CCM(HI_U32 u32SoftChanId, HI_U32 ScrPhyAddr,
                      HI_U32 DestPhyAddr, HI_U32 u32DataLength, HI_BOOL bIsDescrypt);
HI_S32 DRV_CIPHER_GCM(HI_U32 u32SoftChanId, HI_U32 ScrPhyAddr,
                      HI_U32 DestPhyAddr, HI_U32 u32DataLength, HI_BOOL bIsDescrypt);
HI_S32 DRV_CIPHER_GetTag(HI_U32 u32SoftChanId, HI_U32 *pu32Tag, HI_U32 *pu32Len);
#endif

HI_S32 DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn);

#ifdef CIPHER_KLAD_SUPPORT
HI_S32 DRV_Cipher_KladLoadKey(HI_U32 chnId,
                              HI_UNF_CIPHER_KEY_SRC_E enRootKey,
                              HI_UNF_CIPHER_KLAD_TARGET_E enTarget,
                              HI_U8 *pu8DataIn,
                              HI_U32 u32KeyLen);

HI_S32 DRV_Cipher_KladEncryptKey(CIPHER_KLAD_KEY_S *pstKladKey);
#endif

#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_H__ */
