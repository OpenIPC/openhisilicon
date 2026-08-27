/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : unf_cipher.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "hi_mpi_cipher.h"
#include "user_osal_lib.h"

HI_S32 HI_UNF_CIPHER_Init()
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_init();
    if (ret != HI_SUCCESS)
    {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_init, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_DeInit(HI_VOID)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_deinit();
    if (ret != HI_SUCCESS)
    {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_deinit, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE *phCipher,
                                  const HI_UNF_CIPHER_ATTS_S *pstCipherAttr)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_create_handle(phCipher, (hi_cipher_atts *)pstCipherAttr);
    if (ret != HI_SUCCESS)
    {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_create_handle, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_destroy_handle(hCipher);
    if (ret != HI_SUCCESS)
    {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_destroy_handle, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher,
                                  const HI_UNF_CIPHER_CTRL_S *pstCtrl)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_config_handle(hCipher, (hi_cipher_ctrl *)pstCtrl);
    if (ret != HI_SUCCESS)
    {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_config_handle, ret);
        return ret;
    }
    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_ConfigHandleEx(HI_HANDLE hCipher,
                                    const HI_UNF_CIPHER_CTRL_EX_S *pstExCtrl)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_config_handle_ex(hCipher, (hi_cipher_ctrl_ex *)pstExCtrl);
    if (ret != HI_SUCCESS)
    {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_config_handle_ex, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher,
                             HI_SIZE_T u32SrcPhyAddr,
                             HI_SIZE_T u32DestPhyAddr,
                             HI_U32 u32ByteLength)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_encrypt(hCipher, u32SrcPhyAddr,
                                u32DestPhyAddr, u32ByteLength);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_encrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher,
                             HI_SIZE_T u32SrcPhyAddr,
                             HI_SIZE_T u32DestPhyAddr,
                             HI_U32 u32ByteLength)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_decrypt(hCipher, u32SrcPhyAddr,
                                u32DestPhyAddr, u32ByteLength);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_decrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_EncryptVir(HI_HANDLE hCipher,
                                const HI_U8 *pu8SrcData,
                                HI_U8 *pu8DestData,
                                HI_U32 u32ByteLength)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_encrypt_vir(hCipher, pu8SrcData, pu8DestData, u32ByteLength);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_encrypt_vir, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_DecryptVir(HI_HANDLE hCipher,
                                const HI_U8 *pu8SrcData,
                                HI_U8 *pu8DestData,
                                HI_U32 u32ByteLength)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_decrypt_vir(hCipher, pu8SrcData, pu8DestData, u32ByteLength);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_decrypt_vir, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_EncryptMulti(HI_HANDLE hCipher,
                                const HI_UNF_CIPHER_DATA_S *pstDataPkg,
                                HI_U32 u32DataPkgNum)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_encrypt_multi(hCipher,
                                      (hi_cipher_data *)pstDataPkg,
                                      u32DataPkgNum);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_encrypt_multi, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_DecryptMulti(HI_HANDLE hCipher,
                                  const HI_UNF_CIPHER_DATA_S *pstDataPkg,
                                  HI_U32 u32DataPkgNum)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_decrypt_multi(hCipher,
                                      (hi_cipher_data *)pstDataPkg,
                                      u32DataPkgNum);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_decrypt_multi, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_GetTag(HI_HANDLE hCipher, HI_U8 *pu8Tag, HI_U32 *pu32TagLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_get_tag(hCipher, pu8Tag, pu32TagLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_get_tag, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle,
                                     HI_UNF_CIPHER_CTRL_S *pstCtrl)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_get_handle_config(hCipherHandle, (hi_cipher_ctrl *)pstCtrl);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_get_handle_config, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_KladEncryptKey(HI_UNF_CIPHER_CA_TYPE_E enRootKey,
                                    HI_UNF_CIPHER_KLAD_TARGET_E enTarget,
                                    const HI_U8 *pu8CleanKey,
                                    HI_U8 *pu8EcnryptKey,
                                    HI_U32 u32KeyLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_klad_encrypt_key((hi_cipher_ca_type)enRootKey,
                                         (hi_cipher_klad_target)enTarget,
                                         pu8CleanKey,
                                         pu8EcnryptKey,
                                         u32KeyLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_klad_encrypt_key, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_get_random_number(pu32RandomNumber);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_get_random_number, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_HashInit(const HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr,
                              HI_HANDLE *pHashHandle)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_hash_init((hi_cipher_hash_atts *)pstHashAttr,
                                  pHashHandle);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_hash_init, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_HashUpdate(HI_HANDLE hHashHandle,
                                const HI_U8 *pu8InputData,
                                HI_U32 u32InputDataLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_hash_update(hHashHandle, pu8InputData, u32InputDataLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_hash_update, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_hash_final(hHashHandle, pu8OutputHash);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_hash_final, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaPublicEncrypt(const HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc,
                                      const HI_U8 *pu8Input, HI_U32 u32InLen,
                                      HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_rsa_public_encrypt((hi_cipher_rsa_pub_enc *)pstRsaEnc,
                                           pu8Input, u32InLen,
                                           pu8Output, pu32OutLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_rsa_public_encrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;

}

HI_S32 HI_UNF_CIPHER_RsaPrivateDecrypt(const HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,
                                       const HI_U8 *pu8Input, HI_U32 u32InLen,
                                       HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_rsa_private_decrypt((hi_cipher_rsa_pri_enc *)pstRsaDec,
                                            pu8Input, u32InLen,
                                            pu8Output, pu32OutLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_rsa_private_decrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaPrivateEncrypt(const HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc,
                                       const HI_U8 *pu8Input, HI_U32 u32InLen,
                                       HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_rsa_private_encrypt((hi_cipher_rsa_pri_enc *)pstRsaEnc,
                                            pu8Input, u32InLen,
                                            pu8Output, pu32OutLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_rsa_private_encrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}


HI_S32 HI_UNF_CIPHER_RsaPublicDecrypt(const HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
                                      const HI_U8 *pu8Input, HI_U32 u32InLen,
                                      HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_rsa_public_decrypt((hi_cipher_rsa_pub_enc *)pstRsaDec,
        pu8Input, u32InLen, pu8Output, pu32OutLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_rsa_public_decrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaSign(const HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign,
                             const HI_U8 *pu8InData, HI_U32 u32InDataLen,
                             const HI_U8 *pu8HashData,
                             HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_rsa_sign((hi_cipher_rsa_sign *)pstRsaSign,
                                 pu8InData, u32InDataLen,
                                 pu8HashData,
                                 pu8OutSign, pu32OutSignLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_rsa_sign, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaVerify(const HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
                               const HI_U8 *pu8InData, HI_U32 u32InDataLen,
                               const HI_U8 *pu8HashData,
                               const HI_U8 *pu8InSign, HI_U32 u32InSignLen)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    ret = hi_mpi_cipher_rsa_verify((hi_cipher_rsa_verify*)pstRsaVerify,
                                   pu8InData, u32InDataLen,
                                   pu8HashData,
                                   pu8InSign, u32InSignLen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hi_mpi_cipher_rsa_verify, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== Compat Code end ====*/
