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

#include "hi_mpi_cipher.h"

HI_S32 HI_UNF_CIPHER_Init(HI_VOID)
{
    return HI_MPI_CIPHER_Init();
}

HI_S32 HI_UNF_CIPHER_DeInit(HI_VOID)
{
    return HI_MPI_CIPHER_DeInit();
}

#ifdef CFG_HI_CIPHER_MULTICIPHER_SUPPORT
HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE* phCipher)
{
    return HI_MPI_CIPHER_CreateHandle(phCipher);
}

HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    return HI_MPI_CIPHER_DestroyHandle(hCipher);
}

#ifdef CFG_HI_CIPHER_RNG_SUPPORT
HI_S32 HI_UNF_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber)
{
	return HI_MPI_CIPHER_GetRandomNumber(pu32RandomNumber, -1);
}
#endif

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    return HI_MPI_CIPHER_ConfigHandle(hCipher, pstCtrl);
}

HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    return HI_MPI_CIPHER_Encrypt(hCipher, u32SrcPhyAddr, u32DestPhyAddr, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    return HI_MPI_CIPHER_Decrypt(hCipher, u32SrcPhyAddr, u32DestPhyAddr, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_EncryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    return HI_MPI_CIPHER_EncryptMulti(hCipher, pstDataPkg, u32DataPkgNum);
}

HI_S32 HI_UNF_CIPHER_DecryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    return HI_MPI_CIPHER_DecryptMulti(hCipher, pstDataPkg, u32DataPkgNum);
}

HI_S32 HI_UNF_CIPHER_EncryptMultiEx(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    return HI_MPI_CIPHER_EncryptMultiEx(hCipher, pstCtrl, pstDataPkg, u32DataPkgNum);
}

HI_S32 HI_UNF_CIPHER_DecryptMultiEx(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    return HI_MPI_CIPHER_DecryptMultiEx(hCipher, pstCtrl, pstDataPkg, u32DataPkgNum);
}

#ifdef CFG_HI_CIPHER_CCM_GCM_SUPPORT
HI_S32 HI_UNF_CIPHER_GetTag(HI_HANDLE hCipher,  HI_U8 *pstTag)
{
    return HI_MPI_CIPHER_GetTag(hCipher, pstTag);
}
#endif

#ifdef CFG_HI_CIPHER_GETHANDLECONFIG_SUPPORT
HI_S32 HI_UNF_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    return HI_MPI_CIPHER_GetHandleConfig(hCipherHandle, pstCtrl);
}
#endif
#endif

#ifdef CFG_HI_CIPHER_HASH_SUPPORT
HI_S32 HI_UNF_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    return HI_MPI_CIPHER_HashInit(pstHashAttr, pHashHandle);
}

HI_S32 HI_UNF_CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    return HI_MPI_CIPHER_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen);
}

HI_S32 HI_UNF_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash)
{
    return HI_MPI_CIPHER_HashFinal(hHashHandle, pu8OutputHash);
}

HI_S32 HI_UNF_CIPHER_Hash(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                          HI_U32 u32DataPhyAddr, HI_U32 u32ByteLength,
                          HI_U8 *pu8OutputHash)
{
    return HI_MPI_CIPHER_Hash(pstHashAttr, u32DataPhyAddr, u32ByteLength, pu8OutputHash);
}

#endif

#ifdef CFG_HI_CIPHER_RSA_SUPPORT
HI_S32 HI_UNF_CIPHER_RsaPublicEnc(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
								  HI_U8 *pu8Input, HI_U32 u32InLen,
								  HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return HI_MPI_CIPHER_RsaPublicEnc(pstRsaEnc, pu8Input, u32InLen, pu8Output, pu32OutLen);
}

HI_S32 HI_UNF_CIPHER_RsaPrivateDec(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
								   HI_U8 *pu8Input, HI_U32 u32InLen,
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return HI_MPI_CIPHER_RsaPrivateDec(pstRsaDec, pu8Input, u32InLen, pu8Output, pu32OutLen);
}

HI_S32 HI_UNF_CIPHER_RsaSign(HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign,
							 HI_U8 *pu8InData, HI_U32 u32InDataLen,
							 HI_U8 *pu8HashData,
							 HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen)
{
    return HI_MPI_CIPHER_RsaSign(pstRsaSign, pu8InData, u32InDataLen, pu8HashData,
        pu8OutSign, pu32OutSignLen);
}

HI_S32 HI_UNF_CIPHER_RsaVerify(HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
							   HI_U8 *pu8InData, HI_U32 u32InDataLen,
							   HI_U8 *pu8HashData,
							   HI_U8 *pu8InSign, HI_U32 u32InSignLen)
{
    return HI_MPI_CIPHER_RsaVerify(pstRsaVerify, pu8InData, u32InDataLen, pu8HashData,
        pu8InSign, u32InSignLen);
}

HI_S32 HI_UNF_CIPHER_RsaPrivateEnc(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
							 	   HI_U8 *pu8Input, HI_U32 u32InLen,
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return HI_MPI_CIPHER_RsaPrivateEnc(pstRsaEnc, pu8Input, u32InLen, pu8Output, pu32OutLen);
}

HI_S32 HI_UNF_CIPHER_RsaPublicDec(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
							   HI_U8 *pu8Input, HI_U32 u32InLen,
							   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return HI_MPI_CIPHER_RsaPublicDec(pstRsaDec, pu8Input, u32InLen, pu8Output,pu32OutLen);
}

#endif


HI_S32 HI_UNF_CIPHER_WriteOTPKey(HI_U32 u32OptId, const HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    return HI_MPI_CIPHER_WriteOTPKey(u32OptId, pu8Key, u32KeyLen);
}





