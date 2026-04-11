/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : priv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/01/23
  Description   :
  History       :
  1.Date        : 2010/05/11
    Author      :
    Modification: Created file

******************************************************************************/

#ifndef __PRIV_CIPHER_H__
#define __PRIV_CIPHER_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef HI_U32 HI_HANDLE;

typedef enum hiHI_UNF_CIPHER_WORK_MODE_E
{
    HI_UNF_CIPHER_WORK_MODE_ECB = 0x0,
    HI_UNF_CIPHER_WORK_MODE_CBC = 0x1,
    HI_UNF_CIPHER_WORK_MODE_CFB = 0x2,
    HI_UNF_CIPHER_WORK_MODE_OFB = 0x3,
    HI_UNF_CIPHER_WORK_MODE_CTR = 0x4,
    HI_UNF_CIPHER_WORK_MODE_BUTT = 0x5    

}HI_UNF_CIPHER_WORK_MODE_E;

typedef enum hiHI_UNF_CIPHER_ALG_E
{
    HI_UNF_CIPHER_ALG_DES  = 0x0,
    HI_UNF_CIPHER_ALG_3DES = 0x1,
    HI_UNF_CIPHER_ALG_AES  = 0x2,
    HI_UNF_CIPHER_ALG_BUTT = 0x3   
}HI_UNF_CIPHER_ALG_E;

typedef enum hiHI_UNF_CIPHER_KEY_LENGTH_E
{
    HI_UNF_CIPHER_KEY_AES_128BIT = 0x0,
    HI_UNF_CIPHER_KEY_AES_192BIT = 0x1,
    HI_UNF_CIPHER_KEY_AES_256BIT = 0x2,
    HI_UNF_CIPHER_KEY_DES_3KEY = 0x2,
    HI_UNF_CIPHER_KEY_DES_2KEY = 0x3,
}HI_UNF_CIPHER_KEY_LENGTH_E;

typedef enum hiHI_UNF_CIPHER_BIT_WIDTH_E
{
    HI_UNF_CIPHER_BIT_WIDTH_64BIT  = 0x0,
    HI_UNF_CIPHER_BIT_WIDTH_8BIT   = 0x1,
    HI_UNF_CIPHER_BIT_WIDTH_1BIT   = 0x2,
    HI_UNF_CIPHER_BIT_WIDTH_128BIT = 0x3,
}HI_UNF_CIPHER_BIT_WIDTH_E;

typedef struct hiHI_UNF_CIPHER_CTRL_S
{
    HI_U32                     u32Key[8];    
    HI_U32                     u32IV[4];     
    HI_BOOL                    bKeyByCA;     
    HI_UNF_CIPHER_ALG_E        enAlg;       
    HI_UNF_CIPHER_BIT_WIDTH_E  enBitWidth;
    HI_UNF_CIPHER_WORK_MODE_E  enWorkMode;
    HI_UNF_CIPHER_KEY_LENGTH_E enKeyLen;
} HI_UNF_CIPHER_CTRL_S;

/* device parameter */
typedef struct  hiCIPHER_DATA_S
{
  HI_HANDLE CIHandle;
  HI_U32 ScrPhyAddr;
  HI_U32 DestPhyAddr;
  HI_U32 ByteLength;
}CIPHER_DATA_S;

typedef struct hiCIPHER_Config_CTRL
{
  HI_HANDLE CIHandle;
  HI_UNF_CIPHER_CTRL_S CIpstCtrl;
}CIPHER_Config_CTRL;


typedef struct hiCIPHER_PROC_ITEM_S
{
    HI_U32    u32Resv;

}CIPHER_PROC_ITEM_S;


/** @} */  /** <!-- ==== Structure Definition end ==== */


/* Ioctl definitions */
#define HI_ID_CIPHER 100

#define    HI_CIPHER_CreateHandle    _IOWR(HI_ID_CIPHER,0x1,HI_U32)
#define    HI_CIPHER_DestroyHandle   _IOW(HI_ID_CIPHER,0x2,HI_U32)
#define    HI_CIPHER_ConfigHandle    _IOW(HI_ID_CIPHER,0x3,HI_U32)
#define    HI_CIPHER_Encrypt         _IOW(HI_ID_CIPHER,0x4,HI_U32)
#define    HI_CIPHER_Decrypt         _IOW(HI_ID_CIPHER,0x5,HI_U32)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __PRIV_CIPHER_H__ */










