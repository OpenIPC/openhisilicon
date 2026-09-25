/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_API_CIPHER_H__
#define __XMEDIA_API_CIPHER_H__

#include "type.h"
#include "comm_cipher.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define XMEDIA_API_CIPHER_Open(XMEDIA_VOID) XMEDIA_API_CIPHER_Init(XMEDIA_VOID);
#define XMEDIA_API_CIPHER_Close(XMEDIA_VOID) XMEDIA_API_CIPHER_DeInit(XMEDIA_VOID);

XMEDIA_S32 XMEDIA_API_CIPHER_Init(XMEDIA_VOID);

XMEDIA_S32 XMEDIA_API_CIPHER_DeInit(XMEDIA_VOID);

XMEDIA_S32 XMEDIA_API_CIPHER_CreateHandle(XMEDIA_HANDLE* phCipher, const UNF_CIPHER_ATTS_S *pstCipherAttr);

XMEDIA_S32 XMEDIA_API_CIPHER_DestroyHandle(XMEDIA_HANDLE hCipher);

XMEDIA_S32 XMEDIA_API_CIPHER_ConfigHandle(XMEDIA_HANDLE hCipher, const UNF_CIPHER_CTRL_S* pstCtrl);

XMEDIA_S32 XMEDIA_API_CIPHER_ConfigHandleEx(XMEDIA_HANDLE hCipher, const UNF_CIPHER_CTRL_EX_S* pstExCtrl);

XMEDIA_S32 XMEDIA_API_CIPHER_Encrypt(XMEDIA_HANDLE hCipher, XMEDIA_SIZE_T szSrcPhyAddr, XMEDIA_SIZE_T szDestPhyAddr, XMEDIA_U32 u32ByteLength);

XMEDIA_S32 XMEDIA_API_CIPHER_Decrypt(XMEDIA_HANDLE hCipher, XMEDIA_SIZE_T szSrcPhyAddr, XMEDIA_SIZE_T szDestPhyAddr, XMEDIA_U32 u32ByteLength);

XMEDIA_S32 XMEDIA_API_CIPHER_EncryptVir(XMEDIA_HANDLE hCipher, const XMEDIA_U8 *pu8SrcData, XMEDIA_U8 *pu8DestData, XMEDIA_U32 u32ByteLength);

XMEDIA_S32 XMEDIA_API_CIPHER_DecryptVir(XMEDIA_HANDLE hCipher, const XMEDIA_U8 *pu8SrcData, XMEDIA_U8 *pu8DestData, XMEDIA_U32 u32ByteLength);

XMEDIA_S32 XMEDIA_API_CIPHER_EncryptMulti(XMEDIA_HANDLE hCipher, const UNF_CIPHER_DATA_S *pstDataPkg, XMEDIA_U32 u32DataPkgNum);

XMEDIA_S32 XMEDIA_API_CIPHER_GetHandleConfig(XMEDIA_HANDLE hCipher, UNF_CIPHER_CTRL_S* pstCtrl);

XMEDIA_S32 XMEDIA_API_CIPHER_DecryptMulti(XMEDIA_HANDLE hCipher, const UNF_CIPHER_DATA_S *pstDataPkg, XMEDIA_U32 u32DataPkgNum);

XMEDIA_S32 XMEDIA_API_CIPHER_KladEncryptKey(UNF_CIPHER_CA_TYPE_E enRootKey,
                                    UNF_CIPHER_KLAD_TARGET_E enTarget,
                                    const XMEDIA_U8 *pu8CleanKey, XMEDIA_U8* pu8EcnryptKey, XMEDIA_U32 u32KeyLen);

XMEDIA_S32 XMEDIA_API_CIPHER_GetRandomNumber(XMEDIA_U32 *pu32RandomNumber);

XMEDIA_S32 XMEDIA_API_CIPHER_HashInit(const UNF_CIPHER_HASH_ATTS_S *pstHashAttr, XMEDIA_HANDLE *pHashHandle);

XMEDIA_S32 XMEDIA_API_CIPHER_HashUpdate(XMEDIA_HANDLE hHashHandle, const XMEDIA_U8 *pu8InputData, XMEDIA_U32 u32InputDataLen);


XMEDIA_S32 XMEDIA_API_CIPHER_HashFinal(XMEDIA_HANDLE hHashHandle, XMEDIA_U8 *pu8OutputHash);

XMEDIA_S32 XMEDIA_API_CIPHER_RsaPublicEncrypt(const UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
                                  const XMEDIA_U8 *pu8Input, XMEDIA_U32 u32InLen,
                                  XMEDIA_U8 *pu8Output, XMEDIA_U32 *pu32OutLen);

XMEDIA_S32 XMEDIA_API_CIPHER_RsaPrivateDecrypt(const UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
                                    const XMEDIA_U8 *pu8Input, XMEDIA_U32 u32InLen,
                                    XMEDIA_U8 *pu8Output, XMEDIA_U32 *pu32OutLen);

XMEDIA_S32 XMEDIA_API_CIPHER_RsaPrivateEncrypt(const UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
                                    const XMEDIA_U8 *pu8Input, XMEDIA_U32 u32InLen,
                                    XMEDIA_U8 *pu8Output, XMEDIA_U32 *pu32OutLen);

XMEDIA_S32 XMEDIA_API_CIPHER_RsaPublicDecrypt(const UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
                               const XMEDIA_U8 *pu8Input, XMEDIA_U32 u32InLen,
                               XMEDIA_U8 *pu8Output, XMEDIA_U32 *pu32OutLen);

XMEDIA_S32 XMEDIA_API_CIPHER_RsaSign(const UNF_CIPHER_RSA_SIGN_S *pstRsaSign,
                             const XMEDIA_U8 *pu8InData, XMEDIA_U32 u32InDataLen,
                             const XMEDIA_U8 *pu8HashData,
                             XMEDIA_U8 *pu8OutSign, XMEDIA_U32 *pu32OutSignLen);

XMEDIA_S32 XMEDIA_API_CIPHER_RsaVerify(const UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
                               const XMEDIA_U8 *pu8InData, XMEDIA_U32 u32InDataLen,
                               const XMEDIA_U8 *pu8HashData,
                               const XMEDIA_U8 *pu8InSign, XMEDIA_U32 u32InSignLen);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

