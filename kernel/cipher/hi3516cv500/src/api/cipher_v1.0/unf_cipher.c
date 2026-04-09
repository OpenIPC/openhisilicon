/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description   : drivers for unf cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2018-10-20
 */
#include "hi_unf_cipher.h"
#include "hi_mpi_cipher.h"

HI_S32 HI_UNF_CIPHER_Init(HI_VOID)
{
    return hi_mpi_cipher_init();
}

HI_S32 HI_UNF_CIPHER_DeInit(HI_VOID)
{
    return hi_mpi_cipher_deinit();
}

HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE *phCipher, const HI_UNF_CIPHER_ATTS_S *pstCipherAttr)
{
    return hi_mpi_cipher_create_handle(phCipher, (hi_cipher_attr *)pstCipherAttr);
}

HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    return hi_mpi_cipher_destroy_handle(hCipher);
}

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, const HI_UNF_CIPHER_CTRL_S *pstCtrl)
{
    return hi_mpi_cipher_cfg_handle(hCipher, (hi_cipher_ctrl *)pstCtrl);
}

HI_S32 HI_UNF_CIPHER_ConfigHandleEx(HI_HANDLE hCipher, const HI_UNF_CIPHER_CTRL_EX_S *pstExCtrl)
{
    return hi_mpi_cipher_cfg_handle_ex(hCipher, (hi_cipher_ctrl_ex *)pstExCtrl);
}

HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_SIZE_T u32SrcPhyAddr, HI_SIZE_T u32DestPhyAddr, HI_U32 u32ByteLength)
{
    return hi_mpi_cipher_encrypt(hCipher, (hi_phys_addr_t)u32SrcPhyAddr, (hi_phys_addr_t)u32DestPhyAddr, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_SIZE_T u32SrcPhyAddr, HI_SIZE_T u32DestPhyAddr, HI_U32 u32ByteLength)
{
    return hi_mpi_cipher_decrypt(hCipher, (hi_phys_addr_t)u32SrcPhyAddr, (hi_phys_addr_t)u32DestPhyAddr, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_EncryptVir(HI_HANDLE hCipher, const HI_U8 *pu8SrcData, HI_U8 *pu8DestData, HI_U32 u32ByteLength)
{
    return hi_mpi_cipher_encrypt_virt(hCipher, pu8SrcData, pu8DestData, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_DecryptVir(HI_HANDLE hCipher, const HI_U8 *pu8SrcData, HI_U8 *pu8DestData, HI_U32 u32ByteLength)
{
    return hi_mpi_cipher_decrypt_virt(hCipher, pu8SrcData, pu8DestData, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_EncryptMulti(HI_HANDLE hCipher, const HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    return hi_mpi_cipher_encrypt_multi_pack(hCipher, (hi_cipher_data *)pstDataPkg, u32DataPkgNum);
}

HI_S32 HI_UNF_CIPHER_DecryptMulti(HI_HANDLE hCipher, const HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    return hi_mpi_cipher_decrypt_multi_pack(hCipher, (hi_cipher_data *)pstDataPkg, u32DataPkgNum);
}

HI_S32 HI_UNF_CIPHER_GetTag(HI_HANDLE hCipher, HI_U8 *pu8Tag, HI_U32 *pu32TagLen)
{
    return hi_mpi_cipher_get_tag(hCipher, pu8Tag, pu32TagLen);
}

HI_S32 HI_UNF_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle, HI_UNF_CIPHER_CTRL_S *pstCtrl)
{
    return hi_mpi_cipher_get_handle_cfg(hCipherHandle, (hi_cipher_ctrl *)pstCtrl);
}

HI_S32 HI_UNF_CIPHER_KladEncryptKey(HI_UNF_CIPHER_CA_TYPE_E enRootKey, HI_UNF_CIPHER_KLAD_TARGET_E enTarget,
    const HI_U8 *pu8CleanKey, HI_U8 *pu8EcnryptKey, HI_U32 u32KeyLen)
{
    return hi_mpi_cipher_klad_encrypt_key((hi_cipher_ca_type)enRootKey, (hi_cipher_klad_target)enTarget, pu8CleanKey,
        pu8EcnryptKey, u32KeyLen);
}

HI_S32 HI_UNF_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber)
{
    return hi_mpi_cipher_get_random_num(pu32RandomNumber);
}

HI_S32 HI_UNF_CIPHER_HashInit(const HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    return hi_mpi_cipher_hash_init((hi_cipher_hash_attr *)pstHashAttr, pHashHandle);
}

HI_S32 HI_UNF_CIPHER_HashUpdate(HI_HANDLE hHashHandle, const HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    return hi_mpi_cipher_hash_update(hHashHandle, pu8InputData, u32InputDataLen);
}

HI_S32 HI_UNF_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash)
{
    return hi_mpi_cipher_hash_final(hHashHandle, pu8OutputHash);
}

HI_S32 HI_UNF_CIPHER_RsaPublicEncrypt(const HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
    const HI_U8 *pu8Input, HI_U32 u32InLen, HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return hi_mpi_cipher_rsa_pub_encrypt((hi_cipher_rsa_pub_encrypt *)pstRsaEnc, pu8Input, u32InLen,
        pu8Output, pu32OutLen);
}

HI_S32 HI_UNF_CIPHER_RsaPrivateDecrypt(const HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
    const HI_U8 *pu8Input, HI_U32 u32InLen, HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return hi_mpi_cipher_rsa_private_decrypt((hi_cipher_rsa_private_encrypt *)pstRsaDec, pu8Input, u32InLen,
        pu8Output, pu32OutLen);
}

HI_S32 HI_UNF_CIPHER_RsaPrivateEncrypt(const HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
    const HI_U8 *pu8Input, HI_U32 u32InLen, HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return hi_mpi_cipher_rsa_private_encrypt((hi_cipher_rsa_private_encrypt *)pstRsaEnc, pu8Input, u32InLen,
        pu8Output, pu32OutLen);
}


HI_S32 HI_UNF_CIPHER_RsaPublicDecrypt(const HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
    const HI_U8 *pu8Input, HI_U32 u32InLen, HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    return hi_mpi_cipher_rsa_pub_decrypt((hi_cipher_rsa_pub_encrypt *)pstRsaDec,
        pu8Input, u32InLen, pu8Output, pu32OutLen);
}

HI_S32 HI_UNF_CIPHER_RsaSign(const HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign, const HI_U8 *pu8InData, HI_U32 u32InDataLen,
    const HI_U8 *pu8HashData, HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen)
{
    return hi_mpi_cipher_rsa_sign((hi_cipher_rsa_sign *)pstRsaSign, pu8InData, u32InDataLen, pu8HashData,
        pu8OutSign, pu32OutSignLen);
}

HI_S32 HI_UNF_CIPHER_RsaVerify(const HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
    const HI_U8 *pu8InData, HI_U32 u32InDataLen, const HI_U8 *pu8HashData, const HI_U8 *pu8InSign, HI_U32 u32InSignLen)
{
    return hi_mpi_cipher_rsa_verify((hi_cipher_rsa_verify *)pstRsaVerify, pu8InData, u32InDataLen, pu8HashData,
        pu8InSign, u32InSignLen);
}
