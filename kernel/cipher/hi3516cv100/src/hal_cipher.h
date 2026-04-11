/**
\file
\brief cipher hal
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2009-11-3
*/

#ifndef __HAL_CIPHER_H__
#define __HAL_CIPHER_H__

/* add include here */
//#include <asm/arch/hardware.h> /* for IO_ADDRESS */
//#include <./arch/arm/mach-x5hd/include/mach/hardware.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>


//#include "hi_cipher.h"
#include "hi_unf_ecs.h"
#include "cipher_reg.h"
#include "cipher_define.h"

#ifdef __cplusplus
extern "C" {
#endif
/***************************** Macro Definition ******************************/
#define  CIPHER_IRQ_NUMBER          13

#define  CI_CHAN_NUM                 8
#define  CI_PKGx1_CHAN               0
#define  CI_PKGxN_CHAN_MIN           1
#define  CI_PKGxN_CHAN_MAX           7


/*************************** Structure Definition ****************************/
/** */
typedef enum hiCIPHER_INT_TYPE_E
{
    CIPHER_INT_TYPE_IN_BUF  = 0x1,
    CIPHER_INT_TYPE_OUT_BUF = 0x2,
} CIPHER_INT_TYPE_E;

typedef enum hiCIPHER_BUF_TYPE_E
{
    CIPHER_BUF_TYPE_IN  = 0x1,
    CIPHER_BUF_TYPE_OUT = 0x2,
} CIPHER_BUF_TYPE_E;


extern HI_S32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum);
extern HI_S32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum);
extern HI_S32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum);
extern HI_S32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum);
extern HI_S32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum);
extern HI_S32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum);
extern HI_S32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num);
extern HI_S32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num);
extern HI_S32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num);
extern HI_S32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num);
extern HI_S32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num);
extern HI_S32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num);

extern HI_S32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value);
extern HI_S32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value);
extern HI_VOID HAL_Cipher_DisableAllInt(HI_VOID);
extern HI_S32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType);
extern HI_S32 HAL_Cipher_EnableInt(HI_U32 chnId, int intType);
extern HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState);
extern HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus);
extern HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState);
extern HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState);

extern HI_S32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_UNF_CIPHER_CTRL_S* pCtrl);

extern HI_S32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr);
extern HI_S32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl);
extern HI_S32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl);
extern HI_S32 HAL_Cipher_SetKey(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl);

extern HI_S32 HAL_Cipher_SetDataSinglePkg(HI_UNF_CIPHER_INFO_S * info);
extern HI_S32 HAL_Cipher_StartSinglePkg(HI_U32 chnId);
extern HI_S32 HAL_Cipher_ReadDataSinglePkg(HI_U32 *pData);

extern HI_S32 HAL_Cipher_WaitIdle(HI_VOID);
extern HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn);
extern HI_VOID HAL_Cipher_Reset(HI_VOID);

extern HI_VOID HAL_Cipher_Init(void);
extern HI_VOID HAL_Cipher_DeInit(void);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_CIPHER_H__ */


