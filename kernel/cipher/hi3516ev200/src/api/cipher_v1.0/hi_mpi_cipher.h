/*
 * Copyright (C), Hisilicon Technologies Co., Ltd. 2014-2019. All rights reserved.
 * Description   : hi_mpi_cipher.h
 * Author        :
 * Create        : 2014-01-23
 */

#ifndef __HI_MPI_CIPHER_H__
#define __HI_MPI_CIPHER_H__

#include "hi_unf_cipher.h"
#include "hi_common_cipher.h"
#include "hi_cipher_compat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

hi_s32 hi_mpi_cipher_init(hi_void);

hi_s32 hi_mpi_cipher_deinit(hi_void);

hi_s32 hi_mpi_cipher_create_handle(hi_handle *handle,
                                   const hi_cipher_atts *cipher_atts);

hi_s32 hi_mpi_cipher_destroy_handle(hi_handle handle);

hi_s32 hi_mpi_cipher_config_handle(hi_handle handle, const hi_cipher_ctrl *ctrl);

hi_s32 hi_mpi_cipher_config_handle_ex(hi_handle handle,
                                      const hi_cipher_ctrl_ex *ctrl_ex);

hi_s32 hi_mpi_cipher_encrypt(hi_handle handle,
                             hi_size_t src_phy_addr,
                             hi_size_t dest_phy_addr,
                             hi_u32 byte_length);

hi_s32 hi_mpi_cipher_decrypt(hi_handle handle,
                             hi_size_t src_phy_addr,
                             hi_size_t dest_phy_addr,
                             hi_u32 byte_length);

hi_s32 hi_mpi_cipher_encrypt_vir(hi_handle handle,
                                 const hi_u8 *src_data,
                                 hi_u8 *dest_data,
                                 hi_u32 byte_length);

hi_s32 hi_mpi_cipher_decrypt_vir(hi_handle handle,
                                 const hi_u8 *src_data,
                                 hi_u8 *dest_data,
                                 hi_u32 byte_length);

hi_s32 hi_mpi_cipher_encrypt_multi(hi_handle handle,
                                   const hi_cipher_data *data_pkg,
                                   hi_u32 data_pkg_num);

hi_s32 hi_mpi_cipher_decrypt_multi(hi_handle handle,
                                   const hi_cipher_data *data_pkg,
                                   hi_u32 data_pkg_num);

hi_s32 hi_mpi_cipher_get_handle_config(hi_handle handle,
                                       hi_cipher_ctrl *ctrl);

hi_s32 hi_mpi_cipher_klad_encrypt_key(hi_cipher_ca_type root_key,
                                      hi_cipher_klad_target target,
                                      const hi_u8 *clean_key,
                                      hi_u8 *ecnrypt_key, hi_u32 key_len);

hi_s32 hi_mpi_cipher_hash_init(const hi_cipher_hash_atts *hash_atts,
                               hi_handle *handle);

hi_s32 hi_mpi_cipher_hash_update(hi_handle handle,
                                 const hi_u8 *input_data,
                                 hi_u32 input_data_len);

hi_s32 hi_mpi_cipher_hash_final(hi_handle handle, hi_u8 *output_hash);

hi_s32 hi_mpi_cipher_get_tag(hi_handle handle, hi_u8 *tag, hi_u32 *tag_len);

hi_s32 hi_mpi_cipher_get_random_number(hi_u32 *random_number);

hi_s32 hi_mpi_cipher_get_multi_random_bytes(hi_u8 *random_byte, hi_u32 bytes);

hi_s32 hi_mpi_cipher_rsa_public_encrypt(const hi_cipher_rsa_pub_enc *rsa_enc,
                                        const hi_u8 *input, hi_u32 in_len,
                                        hi_u8 *output, hi_u32 *out_len);

hi_s32 hi_mpi_cipher_rsa_private_decrypt(const hi_cipher_rsa_pri_enc *rsa_dec,
                                         const hi_u8 *input, hi_u32 in_len,
                                         hi_u8 *output, hi_u32 *out_len);

hi_s32 hi_mpi_cipher_rsa_sign(const hi_cipher_rsa_sign *rsa_sign,
                              const hi_u8 *in_data, hi_u32 in_data_len,
                              const hi_u8 *hash_data,
                              hi_u8 *out_sign, hi_u32 *out_sign_len);

hi_s32 hi_mpi_cipher_rsa_verify(const hi_cipher_rsa_verify *rsa_verify,
                                const hi_u8 *in_data, hi_u32 in_data_len,
                                const hi_u8 *hash_data,
                                const hi_u8 *in_sign, hi_u32 in_sign_len);

hi_s32 hi_mpi_cipher_rsa_private_encrypt(const hi_cipher_rsa_pri_enc *rsa_enc,
                                         const hi_u8 *input, hi_u32 in_len,
                                         hi_u8 *output, hi_u32 *out_len);

hi_s32 hi_mpi_cipher_rsa_public_decrypt(const hi_cipher_rsa_pub_enc *rsa_dec,
                                        const hi_u8 *input, hi_u32 in_len,
                                        hi_u8 *output, hi_u32 *out_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __hi_mpi_cipher_h__ */
