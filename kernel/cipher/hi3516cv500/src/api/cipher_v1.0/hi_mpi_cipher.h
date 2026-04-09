/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2014-2019. All rights reserved.
 * Description   : hi_mpi_cipher.h
 * Author        : Hisilicon multimedia software group
 * Create        : 2014-01-23
 */

#ifndef __HI_MPI_CIPHER_H__
#define __HI_MPI_CIPHER_H__

#include "hi_unf_cipher.h"
#include "hi_common_cipher.h"
#include "hi_cipher_compat.h"
#include "mpi_cipher.c"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

MPI_STATIC hi_s32 hi_mpi_cipher_init(hi_void);

MPI_STATIC hi_s32 hi_mpi_cipher_deinit(hi_void);

MPI_STATIC hi_s32 hi_mpi_cipher_create_handle(hi_handle *handle, const hi_cipher_attr *cipher_attr);

MPI_STATIC hi_s32 hi_mpi_cipher_destroy_handle(hi_handle handle);

MPI_STATIC hi_s32 hi_mpi_cipher_cfg_handle(hi_handle handle, const hi_cipher_ctrl *ctrl);

MPI_STATIC hi_s32 hi_mpi_cipher_cfg_handle_ex(hi_handle handle, const hi_cipher_ctrl_ex *ctrl_ex);

MPI_STATIC hi_s32 hi_mpi_cipher_encrypt(hi_handle handle, hi_size_t src_phy_addr, hi_size_t dest_phy_addr,
    hi_u32 byte_len);

MPI_STATIC hi_s32 hi_mpi_cipher_decrypt(hi_handle handle, hi_size_t src_phy_addr, hi_size_t dest_phy_addr,
    hi_u32 byte_len);

MPI_STATIC hi_s32 hi_mpi_cipher_encrypt_virt(hi_handle handle, const hi_u8 *src_data, hi_u8 *dest_data,
    hi_u32 byte_len);

MPI_STATIC hi_s32 hi_mpi_cipher_decrypt_virt(hi_handle handle, const hi_u8 *src_data, hi_u8 *dest_data,
    hi_u32 byte_len);

MPI_STATIC hi_s32 hi_mpi_cipher_encrypt_multi_pack(hi_handle handle, const hi_cipher_data *pack, hi_u32 pack_num);

MPI_STATIC hi_s32 hi_mpi_cipher_decrypt_multi_pack(hi_handle handle, const hi_cipher_data *pack, hi_u32 pack_num);

MPI_STATIC hi_s32 hi_mpi_cipher_get_handle_cfg(hi_handle handle, hi_cipher_ctrl *ctrl);

MPI_STATIC hi_s32 hi_mpi_cipher_klad_encrypt_key(hi_cipher_ca_type root_key, hi_cipher_klad_target target,
    const hi_u8 *clean_key, hi_u8 *ecnrypt_key, hi_u32 key_len);

MPI_STATIC hi_s32 hi_mpi_cipher_hash_init(const hi_cipher_hash_attr *hash_atts, hi_handle *handle);

MPI_STATIC hi_s32 hi_mpi_cipher_hash_update(hi_handle handle, const hi_u8 *input_data, hi_u32 input_data_len);

MPI_STATIC hi_s32 hi_mpi_cipher_hash_final(hi_handle handle, hi_u8 *output_hash);

MPI_STATIC hi_s32 hi_mpi_cipher_get_tag(hi_handle handle, hi_u8 *tag, hi_u32 *tag_len);

MPI_STATIC hi_s32 hi_mpi_cipher_get_random_num(hi_u32 *random_num);

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_pub_encrypt(const hi_cipher_rsa_pub_encrypt *rsa_encrpt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len);

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_private_decrypt(const hi_cipher_rsa_private_encrypt *rsa_decrpt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len);

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_sign(const hi_cipher_rsa_sign *rsa_sign, const hi_u8 *in_data, hi_u32 in_data_len,
    const hi_u8 *hash_data, hi_u8 *out_sign, hi_u32 *out_sign_len);

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_verify(const hi_cipher_rsa_verify *rsa_verify, const hi_u8 *in_data,
    hi_u32 in_data_len, const hi_u8 *hash_data, const hi_u8 *in_sign, hi_u32 in_sign_len);

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_private_encrypt(const hi_cipher_rsa_private_encrypt *rsa_encrpt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len);

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_pub_decrypt(const hi_cipher_rsa_pub_encrypt *rsa_decrpt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __hi_mpi_cipher_h__ */
