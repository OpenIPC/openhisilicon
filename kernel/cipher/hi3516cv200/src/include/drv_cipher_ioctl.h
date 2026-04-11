/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __DRV_CIPHER_IOCTL_H__
#define __DRV_CIPHER_IOCTL_H__

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_drv_cipher.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


typedef struct hiCIPHER_PROC_ITEM_S
{
    HI_U32    u32Resv;
}CIPHER_PROC_ITEM_S;

typedef struct
{
	HI_U32 u32ChnId;
	HI_CHAR *ps8Openstatus;
	HI_CHAR *ps8Alg;
	HI_CHAR *ps8Mode;
	HI_U32 u32KeyLen;
	HI_CHAR *ps8KeyFrom;
	HI_BOOL bIsDecrypt;
	HI_U32 u32DataInSize;
	HI_U32 u32DataInAddr;
	HI_U32 u32DataOutSize;
	HI_U32 u32DataOutAddr;
	HI_BOOL bInINTAllEn;
	HI_BOOL bInINTEn;
	HI_BOOL bInINTRaw;
	HI_BOOL bOutINTEn;
	HI_BOOL bOutINTRaw;
	HI_U32 u32OutINTCount;		//CHANn_INT_OCNTCFG
	HI_U32 au32IV[4];
	HI_U8 au8DataIn[16];
	HI_U8 au8DataOut[16];
}CIPHER_CHN_STATUS_S;

#define UMAP_DEVNAME_CIPHER "cipher"
#define CIPHER_PROC_NAME    "driver/hi_cipher"

/* Ioctl definitions */
#define HI_ID_CIPHER 100
#define HI_ID_HASH   101

#define    CMD_CIPHER_CREATEHANDLE           _IOWR(HI_ID_CIPHER, 0x1, CIPHER_HANDLE_S)
#define    CMD_CIPHER_DESTROYHANDLE          _IOW(HI_ID_CIPHER,  0x2, HI_U32)
#define    CMD_CIPHER_CONFIGHANDLE           _IOW(HI_ID_CIPHER,  0x3, CIPHER_Config_CTRL)
#define    CMD_CIPHER_ENCRYPT                _IOW(HI_ID_CIPHER,  0x4, CIPHER_DATA_S)
#define    CMD_CIPHER_DECRYPT                _IOW(HI_ID_CIPHER,  0x5, CIPHER_DATA_S)
#define    CMD_CIPHER_DECRYPTMULTI           _IOW(HI_ID_CIPHER,  0x6, CIPHER_DATA_S)
#define    CMD_CIPHER_ENCRYPTMULTI           _IOW(HI_ID_CIPHER,  0x7, CIPHER_DATA_S)
#define    CMD_CIPHER_GETRANDOMNUMBER        _IOWR(HI_ID_CIPHER,  0x8, CIPHER_RNG_S)
#define    CMD_CIPHER_GETHANDLECONFIG        _IOWR(HI_ID_CIPHER, 0x9, CIPHER_Config_CTRL)
#define    CMD_CIPHER_CALCHASHINIT           _IOWR(HI_ID_CIPHER, 0xa, CIPHER_HASH_DATA_S)
#define    CMD_CIPHER_CALCHASHUPDATE         _IOWR(HI_ID_CIPHER, 0xb, CIPHER_HASH_DATA_S)
#define    CMD_CIPHER_CALCHASHFINAL          _IOWR(HI_ID_CIPHER, 0xc, CIPHER_HASH_DATA_S)
#define    CMD_CIPHER_CALCRSA                _IOWR(HI_ID_CIPHER, 0xd, CIPHER_RSA_DATA_S)
#define    HI_CIPHER_DECRYPTMULTI_EX         _IOW(HI_ID_CIPHER,  0xe, CIPHER_MUTIL_EX_DATA_S)
#define    HI_CIPHER_ENCRYPTMULTI_EX         _IOW(HI_ID_CIPHER,  0xf, CIPHER_MUTIL_EX_DATA_S)
#define    CMD_CIPHER_CALCHASH               _IOWR(HI_ID_CIPHER, 0x10, CIPHER_HASH_DATA_S)
#define    CMD_CIPHER_GETTAG                 _IOWR(HI_ID_CIPHER, 0x11, CIPHER_TAG)
#define    CMD_CIPHER_WRITEOTPKEY            _IOW(HI_ID_CIPHER,  0x12, CIPHER_OTP_KEY_S)




typedef union
{   
    CIPHER_HANDLE_S      stCIHandle;
    HI_U32               u32;
    CIPHER_DATA_S        stCIData;
    CIPHER_Config_CTRL   stCIConfig;
    CIPHER_HASH_DATA_S   stCIHashData;
    CIPHER_CBCMAC_DATA_S stCICbcmacData;
    CIPHER_RSA_DATA_S    stCIRsaData;
    CIPHER_RSA_KEY_S     stCIPHERRsaKey;
    CIPHER_MUTIL_EX_DATA_S stMutliEx;
    CIPHER_TAG           stCIPHERTag;
}CMD_CIPHER_PARAM_U;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __DRV_CIPHER_IOCTL_H__*/
