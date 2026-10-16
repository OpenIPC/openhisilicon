/**
\file
\brief cipher osi
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
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


#ifdef __cplusplus
extern "C" {
#endif
/***************************** Macro Definition ******************************/
#define CIPHER_DEFAULT_INT_NUM    (1)
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

#define HI_HANDLE_MAKEHANDLE(mod, privatedata, chnid)  (HI_HANDLE)( (((mod)& 0xffff) << 16) | ((((privatedata)& 0xff) << 8) ) | (((chnid) & 0xff)) )

#define HI_HANDLE_GET_MODID(handle)     (((handle) >> 16) & 0xffff)
#define HI_HANDLE_GET_PriDATA(handle)   (((handle) >> 8) & 0xff)
#define HI_HANDLE_GET_CHNID(handle)     (((handle)) & 0xff)


/********************** Global Variable declaration **************************/

#define CIPHER_CHECK_HANDLE(Handle,Ret)\
    if ((HI_ID_CIPHER != HI_HANDLE_GET_MODID(Handle))\
        || (0 != HI_HANDLE_GET_PriDATA(Handle)))\
    {\
        HI_ERR_CIPHER("Handle(0x%x) is invalid.\n",Handle);\
        Ret = HI_ERR_CIPHER_INVALID_HANDLE;\
        break;\
    }\
    if (CIPHER_SOFT_CHAN_NUM <= HI_HANDLE_GET_CHNID(Handle))\
    {\
        HI_ERR_CIPHER("Soft chnId (%d) is more than 8.\n",Handle);\
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

