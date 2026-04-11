/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :hi_drv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/

#ifndef __HI_DRV_CIPHER_H__
#define __HI_DRV_CIPHER_H__

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#define CIPHER_SOFT_CHAN_NUM      CIPHER_CHAN_NUM
#define CIPHER_INVALID_CHN        (0xffffffff)
#define HASH_BLOCK_SIZE           (64)

#ifdef CIPHER_MULTICIPHER_SUPPORT
typedef struct hiCIPHER_DATA_INFO_S
{
    HI_U32  u32src;
    HI_U32  u32dest;
    HI_U32  u32length;
    HI_BOOL bDecrypt;
    HI_U32  u32DataPkg[4];
}HI_DRV_CIPHER_DATA_INFO_S;

typedef struct hiCIPHER_TASK_S
{
    HI_DRV_CIPHER_DATA_INFO_S stData2Process;
    HI_U32 u32CallBackArg;
}HI_DRV_CIPHER_TASK_S;

typedef struct  hiCIPHER_HANDLE_S
{
    HI_HANDLE hCIHandle;
}CIPHER_HANDLE_S;

typedef struct  hiCIPHER_DATA_S
{
    HI_HANDLE CIHandle;
    HI_BOOL bIsPhyAddr;
    HI_U32 ScrPhyAddr;
    HI_U32 DestPhyAddr;
    HI_U8  *pu8Input;
    HI_U8  *pu8Output;
    HI_U32 u32DataLength;
}CIPHER_DATA_S;

typedef struct  hiCIPHER_MUTIL_EX_DATA_S
{
  HI_HANDLE hCipher;
  HI_UNF_CIPHER_DATA_S *pstPkg;
  HI_U32 u32PkgNum;
  HI_UNF_CIPHER_CTRL_S CIpstCtrl;
}CIPHER_MUTIL_EX_DATA_S;

typedef struct hiCIPHER_Config_CTRL
{
    HI_HANDLE CIHandle;
    HI_UNF_CIPHER_CTRL_S CIpstCtrl;
}CIPHER_Config_CTRL;

typedef struct hiCIPHER_TAG
{
    HI_HANDLE CIHandle;
    HI_U32    u32Tag[4];
    HI_U32    u32Len;
}CIPHER_TAG;

typedef struct  hiCIPHER_CBCMAC_DATA_S
{
    HI_U8 *pu8RefCbcMac;
    HI_U32 u32AppLen;
}CIPHER_CBCMAC_DATA_S;

#endif

#ifdef CIPHER_HASH_SUPPORT
typedef enum
{
    HI_CIPHER_HMAC_KEY_FROM_CA  = 0,
    HI_CIPHER_HMAC_KEY_FROM_CPU = 1,
    HI_CIPHER_HMAC_KEY_FROM_NONE = 0xff,
}CIPHER_HMAC_KEY_FROM_E;

typedef struct hiCipher_HASH_MMZ_BUFFER_S
{
    HI_U32 u32StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
}HASH_MMZ_BUFFER_S;

typedef struct hiCIPHER_HASH_DATA_S
{
    HI_UNF_CIPHER_HASH_TYPE_E enShaType;
    CIPHER_HMAC_KEY_FROM_E enHMACKeyFrom;
    HI_U32  u32ShaVal[8];
    HI_U32  u32DataPhy;
    HI_BOOL  bIsPhyAddr;
    HI_U8  *pu8Input;
    HI_U32  u32DataLen;
}CIPHER_HASH_DATA_S;

typedef struct hiCIPHER_HASH_INIT_S
{
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle;
}CIPHER_HASH_INIT_S;

typedef struct hiCIPHER_HASH_UPDATE_S
{
    HI_HANDLE HashHandle;
    HI_U8 *pu8InputData;
    HI_U32 u32InputDataLen;
}CIPHER_HASH_UPDATE_S;

typedef struct hiCIPHER_HASH_FINISH_S
{
    HI_HANDLE HashHandle;
    HI_U8 *pu8OutputHash;
}CIPHER_HASH_FINISH_S;

#endif

#ifdef CIPHER_RNG_SUPPORT
typedef struct
{
    HI_U32 u32TimeOutUs;
	HI_U32 u32RNG;
}CIPHER_RNG_S;
#endif

#ifdef CIPHER_RSA_SUPPORT
typedef struct
{
    HI_U8  *pu8Input;
    HI_U8  *pu8Output;
    HI_U32  u32DataLen;
    HI_U8  *pu8N;
	HI_U8  *pu8K;
    HI_U16 u16NLen;
    HI_U16 u16KLen;
    HI_UNF_CIPHER_KEY_SRC_E enKeySrc;
}CIPHER_RSA_DATA_S;

typedef struct
{
    HI_UNF_CIPHER_RSA_PRI_KEY_S stPriKey;
    HI_U32 u32NumBits;
    HI_U32 u32Exponent;
}CIPHER_RSA_KEY_S;
#endif

#ifdef CIPHER_KLAD_SUPPORT
typedef struct
{
    HI_UNF_CIPHER_KEY_SRC_E enRootKey;
    HI_UNF_CIPHER_KLAD_TARGET_E enTarget;
    HI_U32 u32CleanKey[4];
    HI_U32 u32EncryptKey[4];
}CIPHER_KLAD_KEY_S;
#endif

#ifdef CIPHER_MULTICIPHER_SUPPORT
HI_S32 HI_DRV_CIPHER_CreateHandle(CIPHER_HANDLE_S *pstCIHandle, HI_U32 file);
HI_S32 HI_DRV_CIPHER_ConfigChn(HI_HANDLE CIHandle, HI_UNF_CIPHER_CTRL_S *pConfig);
HI_S32 HI_DRV_CIPHER_GetHandleConfig(CIPHER_Config_CTRL *pstCipherConfig);
HI_S32 HI_DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn);
HI_S32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_EncryptVir(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_DecryptVir(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_EncryptMulti(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_DecryptMulti(CIPHER_DATA_S *pstCIData);
HI_S32 HI_DRV_CIPHER_EncryptMultiEx(CIPHER_MUTIL_EX_DATA_S *pstCIDataEx);
HI_S32 HI_DRV_CIPHER_DecryptMultiEx(CIPHER_MUTIL_EX_DATA_S *pstCIDataEx);
#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 HI_DRV_CIPHER_GetTag(CIPHER_TAG *pstTag);
#endif
#endif

#ifdef CIPHER_RNG_SUPPORT
HI_S32 HI_DRV_CIPHER_GetRandomNumber(CIPHER_RNG_S *pstRNG);
#endif

#ifdef CIPHER_HASH_SUPPORT
HI_S32 HI_DRV_CIPHER_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData);
HI_S32 HI_DRV_CIPHER_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData);
HI_S32 HI_DRV_CIPHER_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData);
HI_S32 HI_DRV_CIPHER_CalcHash(CIPHER_HASH_DATA_S *pCipherHashData);
#ifdef CIPHER_HASHVIR_SUPPORT
HI_S32 HI_DRV_CIPHER_CalcHashMMZInit(CIPHER_HASH_DATA_S *pCipherHashData);
#endif
HI_S32 CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle);
HI_S32 CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);
HI_S32 CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash);
#endif

#ifdef CIPHER_RSA_SUPPORT
HI_S32 HI_DRV_CIPHER_CalcRsa(CIPHER_RSA_DATA_S *pCipherRsaData);
#endif

#ifdef CIPHER_KLAD_SUPPORT
HI_S32 HI_DRV_CIPHER_KladEncryptKey(CIPHER_KLAD_KEY_S *pstKladKey);
#endif

HI_VOID HI_DRV_CIPHER_Suspend(HI_VOID);
HI_S32 HI_DRV_CIPHER_Resume(HI_VOID);
HI_S32 HI_DRV_CIPHER_SoftReset(HI_VOID);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* End of #ifndef __HI_DRV_CIPHER_H__*/

