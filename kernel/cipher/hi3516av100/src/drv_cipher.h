/**
\file
\brief cipher osi
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2009-11-8
*/

#ifndef __DRV_CIPHER_H__
#define __DRV_CIPHER_H__

/* add include here */
#include "hi_unf_ecs.h"

#include "hal_cipher.h"
#include "hi_error_ecs.h"
#include "cipher_log.h"
#include "cipher_mmz.h"
//#include "common_mem.h"


#ifdef __cplusplus
extern "C" {
#endif
/***************************** Macro Definition ******************************/
#define CIPHER_DEFAULT_INT_NUM    1
#define CIPHER_SOFT_CHAN_NUM      CI_CHAN_NUM

#define CIPHER_INVALID_CHN        (0xffffffff)

typedef struct hiCIPHER_OSR_CHN_S
{
    HI_BOOL g_bSoftChnOpen; /* mark soft channel open or not*/
    HI_BOOL g_bDataDone;    /* mark the data done or not */
    wait_queue_head_t cipher_wait_queue; /* mutex method */
    struct file *pWichFile; /* which file need to operate */
}CIPHER_OSR_CHN_S;

/*************************** Structure Definition ****************************/

typedef struct hiCIPHER_TASK_S
{
    HI_UNF_CIPHER_INFO_S        stData2Process;
    HI_U32                      u32CallBackArg;
} CIPHER_TASK_S;
/** */
typedef HI_VOID (*funcCipherCallback)(HI_U32);

typedef struct hiCIPHER_SOFTCHAN_S
{
    HI_BOOL               bOpen;
    HI_U32                u32HardWareChn;

    HI_UNF_CIPHER_CTRL_S  stCtrl;

    HI_BOOL               bIVChange;
    HI_BOOL               bKeyChange;

    HI_BOOL               bDecrypt;       /* hi_false: encrypt */

    HI_U32                u32PirvateData;
    funcCipherCallback    pfnCallBack;
    CI_MMZ_BUF_S sMLastBlock;

} CIPHER_SOFTCHAN_S;

#define CIPHER_ID_OFFSET       (16)  
#define GET_CIPHER_SOFT_HANDLE (0x00FF)

/********************** Global Variable declaration **************************/
#if 0
#define CIPHER_CHECK_HANDLE(Handle,Ret)\
if (Handle >= CIPHER_SOFT_CHAN_NUM)\
{\
    HI_ERR_CIPHER("Handle(%d) is more than 8.\n",Handle);\
    Ret = HI_ERR_CIPHER_INVALID_HANDLE;\
    break;\
}\

#endif
#define CIPHER_CHECK_HANDLE(CIHandle,Ret)\
if ((CIHandle >> CIPHER_ID_OFFSET) != HI_ID_CIPHER)\
{\
    HI_ERR_CIPHER("CIHandle(%d) is not a valid cipher handle.\n",CIHandle);\
}\
if ((CIHandle & GET_CIPHER_SOFT_HANDLE) >= CIPHER_SOFT_CHAN_NUM)\
{\
    HI_ERR_CIPHER("SoftHandle(%d) is more than 8.\n",CIHandle & GET_CIPHER_SOFT_HANDLE);\
    Ret = HI_ERR_CIPHER_INVALID_HANDLE;\
    break;\
}


/******************************* API declaration *****************************/
extern HI_S32 DRV_Cipher_OpenChn(HI_U32 softChnId);
extern HI_S32 DRV_Cipher_CloseChn(HI_U32 softChnId);
extern HI_S32 DRV_Cipher_ConfigChn(HI_U32 softChnId,  HI_UNF_CIPHER_CTRL_S *pConfig, funcCipherCallback fnCallBack);
extern HI_S32 DRV_Cipher_CreatTask(HI_U32 softChnId, CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV);

extern HI_S32 DRV_Cipher_Init(HI_VOID);
extern HI_VOID DRV_Cipher_DeInit(HI_VOID);
extern HI_VOID  DRV_Cipher_Suspend(HI_VOID);
extern HI_VOID DRV_Cipher_Resume(HI_VOID);
HI_S32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData);

#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_H__ */

