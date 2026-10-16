/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_unf_ecs_type.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2008/6/5
Last Modified :
Description   : ECS API
Function List :
History       :
 ******************************************************************************/
#ifndef __HI_UNF_ECS_TYPE_H__
#define __HI_UNF_ECS_TYPE_H__

#include "priv_cipher.h"
#include "hi_error_ecs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

HI_S32 HI_UNF_CIPHER_Open(HI_VOID);

HI_S32 HI_UNF_CIPHER_Close(HI_VOID);

HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE* phCipher);

HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher);

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl);

HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);
/** @} */  /** <!-- ==== API declaration end ==== */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ECS_TYPE_H__ */

