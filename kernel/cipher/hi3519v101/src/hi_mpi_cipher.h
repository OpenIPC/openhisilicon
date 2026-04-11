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
#ifndef __HI_MPI_CIPHER_H__
#define __HI_MPI_CIPHER_H__

#include "drv_cipher_ioctl.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32 HI_MPI_CIPHER_Init(HI_VOID);
HI_S32 HI_MPI_CIPHER_DeInit(HI_VOID);

#ifdef CIPHER_MULTICIPHER_SUPPORT
HI_S32 HI_MPI_CIPHER_CreateHandle(HI_HANDLE* phCipher);
HI_S32 HI_MPI_CIPHER_DestroyHandle(HI_HANDLE hCipher);
HI_S32 HI_MPI_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl);
HI_S32 HI_MPI_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);
HI_S32 HI_MPI_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);
HI_S32 HI_MPI_CIPHER_EncryptVir(HI_HANDLE hCipher, HI_U8 *pu8SrcData, HI_U8 *pu8DestData, HI_U32 u32ByteLength);
HI_S32 HI_MPI_CIPHER_DecryptVir(HI_HANDLE hCipher, HI_U8 *pu8SrcData, HI_U8 *pu8DestData, HI_U32 u32ByteLength);
HI_S32 HI_MPI_CIPHER_EncryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);
HI_S32 HI_MPI_CIPHER_DecryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);
#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 HI_MPI_CIPHER_GetTag(HI_HANDLE hCipherHandle,  HI_U8 *pstTag);
#endif
HI_S32 HI_MPI_CIPHER_EncryptMultiEx(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);
HI_S32 HI_MPI_CIPHER_DecryptMultiEx(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);
HI_S32 HI_MPI_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle, HI_UNF_CIPHER_CTRL_S* pstCtrl);
#endif


#ifdef CIPHER_KLAD_SUPPORT
HI_S32 HI_MPI_CIPHER_KladEncryptKey(HI_UNF_CIPHER_KEY_SRC_E enRootKey,
                                    HI_UNF_CIPHER_KLAD_TARGET_E enTarget,
                                    HI_U8 *pu8CleanKey, HI_U8* pu8EcnryptKey,
                                    HI_U32 u32KeyLen);
#endif

#ifdef CIPHER_RNG_SUPPORT
HI_S32 HI_MPI_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber, HI_U32 u32TimeOutUs);
#endif

#ifdef CIPHER_HASH_SUPPORT
HI_S32 HI_MPI_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle);
HI_S32 HI_MPI_CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);
HI_S32 HI_MPI_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash);

HI_S32 HI_MPI_CIPHER_Hash(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                          HI_U32 u32DataPhyAddr, HI_U32 u32ByteLength,
                          HI_U8 *pu8OutputHash);
#ifdef CIPHER_HASHVIR_SUPPORT
HI_S32 HI_MPI_CIPHER_HashVir(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                             HI_U8 *pu8InputData, HI_U32 u32ByteLength,
                             HI_U8 *pu8OutputHash);
#endif
#endif

#ifdef CIPHER_RSA_SUPPORT
HI_S32 HI_MPI_CIPHER_RsaPublicEnc(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
								  HI_U8 *pu8Input, HI_U32 u32InLen,
								  HI_U8 *pu8Output, HI_U32 *pu32OutLen);

HI_S32 HI_MPI_CIPHER_RsaPrivateDec(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
								   HI_U8 *pu8Input, HI_U32 u32InLen,
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen);

HI_S32 HI_MPI_CIPHER_RsaSign(HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign,
							 HI_U8 *pu8InData, HI_U32 u32InDataLen,
							 HI_U8 *pu8HashData,
							 HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen);

HI_S32 HI_MPI_CIPHER_RsaVerify(HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
							   HI_U8 *pu8InData, HI_U32 u32InDataLen,
							   HI_U8 *pu8HashData,
							   HI_U8 *pu8InSign, HI_U32 u32InSignLen);

HI_S32 HI_MPI_CIPHER_RsaPrivateEnc(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsEnc,
								   HI_U8 *pu8Input, HI_U32 u32InLen,
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen);

HI_S32 HI_MPI_CIPHER_RsaPublicDec(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
							   HI_U8 *pu8Input, HI_U32 u32InLen,
							   HI_U8 *pu8Output, HI_U32 *pu32OutLen);

HI_S32 HI_MPI_CIPHER_RsaGenKey(HI_U32 u32NumBits, HI_U32 u32Exponent, HI_UNF_CIPHER_RSA_PRI_KEY_S *pstRsaPriKey);
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_MPI_CIPHER_H__ */
