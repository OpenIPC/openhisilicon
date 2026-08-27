/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_cipher.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "hi_mpi_cipher.h"
#include "sys_cipher.h"
#include "user_osal_lib.h"

crypto_mutex  cipher_mutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_CIPHER_LOCK()        (void)crypto_mutex_lock(&cipher_mutex)
#define HI_CIPHER_UNLOCK()      (void)crypto_mutex_unlock(&cipher_mutex)

#define BYTE_BITS               (8)
#define CIPHER_MAX_MULTIPAD_NUM (5000)
#define CENC_SUBSAMPLE_MAX_NUM  (100)
#define ECDH_MAX_KEY_LEN        (72)
#define CIPHER_INIT_MAX_NUM     (0x7FFFFFFF)
#define MAX_TAG_LEN             (16)
#define SYMC_SM1_EK_AK_SIZE     (32)

/* handle of cipher device */
hi_s32 cipher_dev_fd = -1;

/* flag of cipher device
 * indicate the status of device that open or close
 * <0: close, 0: open>0: multiple initialization
 */
static hi_s32 cipher_init_counter = -1;

/* check the device of cipher whether already opend or not */
#define CHECK_CIPHER_OPEN()\
    do{\
        if (0 > cipher_init_counter)\
        {\
            HI_LOG_ERROR("cipher init counter %d\n", cipher_init_counter);\
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_NOT_INIT);\
            return HI_ERR_CIPHER_NOT_INIT;\
        }\
    }while(0)

/**
 * Read E in public key from arry to U32,
 * so only use last byte0~byte3, others are zero
 */
#define CIPHER_GET_PUB_EXPONENT(_e, _rsades) \
    do{\
        hi_u8 *_buf = (_rsades)->pub_key.e; \
        hi_u8 *_pub = (hi_u8*)(_e); \
        hi_u32 _len = (_rsades)->pub_key.e_len; \
        hi_u32 _i; \
        for (_i = 0; _i< MIN(WORD_WIDTH, _len); _i++) \
        {\
            _pub[WORD_WIDTH -_i - 1] = _buf[_len - _i - 1];\
        }\
    }while(0)

/**
 * \brief  Init the cipher device.
 */
hi_s32 hi_mpi_cipher_init(hi_void)
{
    HI_UNF_FUNC_ENTER();

    HI_CIPHER_LOCK();

    if (cipher_init_counter >= CIPHER_INIT_MAX_NUM) {
        HI_CIPHER_UNLOCK();

        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_OVERFLOW);
        return HI_ERR_CIPHER_OVERFLOW;
    }

    if (cipher_init_counter >= 0) {
        cipher_init_counter++;
        HI_CIPHER_UNLOCK();

        HI_UNF_FUNC_EXIT();
        return HI_SUCCESS;
    }

    cipher_dev_fd = crypto_open("/dev/"UMAP_DEVNAME_CIPHER, O_RDWR, 0);
    if (cipher_dev_fd < 0) {
        HI_CIPHER_UNLOCK();

        HI_LOG_PRINT_FUNC_ERR(crypto_open, HI_ERR_CIPHER_FAILED_INIT);
        return HI_ERR_CIPHER_FAILED_INIT;
    }

    cipher_init_counter = 0;

    HI_CIPHER_UNLOCK();

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief  Deinit the cipher device.
 */
hi_s32 hi_mpi_cipher_deinit(hi_void)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_CIPHER_LOCK();

    if (cipher_init_counter < 0) {
        HI_CIPHER_UNLOCK();

        HI_UNF_FUNC_EXIT();
        return HI_SUCCESS;
    }

    if (cipher_init_counter > 0) {
        cipher_init_counter--;

        HI_CIPHER_UNLOCK();

        HI_UNF_FUNC_EXIT();
        return HI_SUCCESS;
    }

    ret = crypto_close(cipher_dev_fd);
    if (ret != HI_SUCCESS) {
        HI_CIPHER_UNLOCK();

        HI_LOG_PRINT_FUNC_ERR(crypto_close, ret);
        return ret;
    }

    cipher_init_counter = -1;

    HI_CIPHER_UNLOCK();

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Obtain a cipher handle for encryption and decryption.
 */
hi_s32 hi_mpi_cipher_create_handle(hi_handle *handle,
                                   const hi_cipher_atts *cipher_atts)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 id = 0;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(handle == HI_NULL);
    HI_LOG_CHECK_PARAM(cipher_atts == HI_NULL);
    HI_LOG_CHECK_PARAM(cipher_atts->cipher_type >= HI_CIPHER_TYPE_BUTT);

    HI_DBG_PRINT_U32(cipher_atts->cipher_type);

    CHECK_CIPHER_OPEN();

    ret = sys_symc_create(&id);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_create, ret);
        return ret;
    }

    *handle = (hi_handle)id;

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 cipher_get_rsa_attr(hi_u32 scheme, hi_u32 *hash_len,
                                     hi_cipher_hash_type *sha_type)
{
    HI_UNF_FUNC_ENTER();

    switch (scheme) {
        case HI_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5: {
            *hash_len = 0;
            *sha_type = HI_CIPHER_HASH_TYPE_BUTT;
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1: {
            *hash_len = SHA1_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA1;
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA224:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224:
            *hash_len = SHA224_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA224;
            break;
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256: {
            *hash_len = SHA256_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA256;
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA384:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384: {
            *hash_len = SHA384_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA384;
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA512:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512: {
            *hash_len = SHA512_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA512;
            break;
        }
        default: {
            HI_ERR_PRINT_U32(scheme);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_UNAVAILABLE);
            return HI_ERR_CIPHER_UNAVAILABLE;
        }
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Destroy the existing cipher handle.
 */
hi_s32 hi_mpi_cipher_destroy_handle(hi_handle handle)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    CHECK_CIPHER_OPEN();

    ret = sys_symc_destroy(handle);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_destroy, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Configures the cipher control information.
 */
hi_s32 hi_mpi_cipher_config_handle(hi_handle handle, const hi_cipher_ctrl *ctrl)
{
    hi_u32 ivlen = AES_IV_SIZE;
    hi_u32 hard_key = 0;
    hi_u32 key_len = SYMC_KEY_SIZE;
    compat_addr aad;
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctrl == HI_NULL);
    HI_LOG_CHECK_PARAM(handle == HI_INVALID_HANDLE);

    if ((ctrl->work_mode == HI_CIPHER_WORK_MODE_CCM)
        || (ctrl->work_mode == HI_CIPHER_WORK_MODE_GCM)) {
        HI_LOG_ERROR("Invalid work mode ccm or gcm by config handle.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((ctrl->key_by_ca != HI_TRUE)
        && (ctrl->key_by_ca != HI_FALSE)
        && (ctrl->alg != HI_CIPHER_ALG_DMA)) {
        HI_LOG_ERROR("Invalid key by ca, you should set HI_TRUE or HI_FALSE.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (ctrl->key_by_ca == HI_TRUE) {
        if (ctrl->ca_type >= HI_CIPHER_KEY_SRC_BUTT) {
            HI_LOG_ERROR("Invalid ca type,key ca type is HI_TRUE.\n");
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
        hard_key  = ((hi_u32)ctrl->ca_type & 0xFF) << BITS_IN_BYTE;
        hard_key |= 0x01;
    }

    CHECK_CIPHER_OPEN();

    ADDR_U64(aad) = 0x00;

    if ((ctrl->alg == HI_CIPHER_ALG_3DES) || (ctrl->alg == HI_CIPHER_ALG_DES)) {
        ivlen = DES_IV_SIZE;
    }

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(ctrl->key_by_ca);
    HI_DBG_PRINT_U32(ctrl->alg);
    HI_DBG_PRINT_U32(ctrl->work_mode);
    HI_DBG_PRINT_U32(ctrl->bit_width);
    HI_DBG_PRINT_U32(ctrl->key_len);
    HI_DBG_PRINT_U32(ivlen);
    HI_DBG_PRINT_U32(ctrl->change_flags.bit1_iv);
    HI_DBG_PRINT_U64(ADDR_U64(aad));

    ret = sys_symc_config(handle, hard_key,
                          ctrl->alg, ctrl->work_mode,
                          ctrl->bit_width, ctrl->key_len, 0,
                          (hi_u8 *)ctrl->key, HI_NULL, key_len,
                          (hi_u8 *)ctrl->iv, ivlen, ctrl->change_flags.bit1_iv,
                          aad, 0, 0);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_config, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Configures the cipher expand control information.
 */
hi_s32 hi_mpi_cipher_config_handle_ex(hi_handle handle,
                                      const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_cipher_key_length key_len = HI_CIPHER_KEY_DEFAULT;
    hi_u8 *fkey = HI_NULL;
    hi_u8 *skey = HI_NULL;
    hi_u8 *iv = HI_NULL;
    hi_u32 usage = 0;
    compat_addr aad;
    hi_u32 a_len = 0;
    hi_u32 tag_len = 0;
    hi_u32 iv_len = 0;
    hi_u32 key_size = 0;
    hi_cipher_sm1_round sm1_round = HI_CIPHER_SM1_ROUND_BUTT;
    hi_cipher_bit_width bit_width = HI_CIPHER_BIT_WIDTH_128BIT;
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctrl_ex == HI_NULL);
    HI_LOG_CHECK_PARAM((ctrl_ex->param == HI_NULL) && (ctrl_ex->alg != HI_CIPHER_ALG_DMA));

    if ((ctrl_ex->key_by_ca != HI_TRUE)
        && (ctrl_ex->key_by_ca != HI_FALSE)
        && (ctrl_ex->alg != HI_CIPHER_ALG_DMA)) {
        HI_LOG_ERROR("Invalid bKeyByCA, you should set HI_TRUE or HI_FALSE.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    ADDR_U64(aad) = 0x00;

    /*****************************************************************************
     * for AES, the pointer should point to hi_cipher_ctrl_aes;
     * for AES_CCM or AES_GCM, the pointer should point to hi_cipher_ctrl_aes_ccm_gcm;
     * for DES, the pointer should point to hi_cipher_ctrl_des;
     * for 3DES, the pointer should point to hi_cipher_ctrl_3des;
     * for SM1, the pointer should point to hi_cipher_ctrl_sm1;
     * for SM4, the pointer should point to hi_cipher_ctrl_sm4;
    */
    switch (ctrl_ex->alg) {
        case HI_CIPHER_ALG_DES: {
            hi_cipher_ctrl_des *des_ctrl = (hi_cipher_ctrl_des *)ctrl_ex->param;

            fkey = (hi_u8 *)des_ctrl->key;
            key_size = sizeof(des_ctrl->key);
            iv = (hi_u8 *)des_ctrl->iv;
            usage = des_ctrl->change_flags.bit1_iv;
            iv_len = DES_IV_SIZE;
            bit_width = des_ctrl->bit_width;

            break;
        }
        case HI_CIPHER_ALG_3DES: {
            hi_cipher_ctrl_3des *tdes_ctrl = (hi_cipher_ctrl_3des *)ctrl_ex->param;

            fkey = (hi_u8 *)tdes_ctrl->key;
            key_size = sizeof(tdes_ctrl->key);
            iv = (hi_u8 *)tdes_ctrl->iv;
            usage = tdes_ctrl->change_flags.bit1_iv;
            key_len = tdes_ctrl->key_len;
            iv_len = DES_IV_SIZE;
            bit_width = tdes_ctrl->bit_width;
            break;
        }
        case HI_CIPHER_ALG_AES: {
            if ((ctrl_ex->work_mode == HI_CIPHER_WORK_MODE_CCM)
                || (ctrl_ex->work_mode == HI_CIPHER_WORK_MODE_GCM)) {
                hi_cipher_ctrl_aes_ccm_gcm *aes_ccm_gcm_ctrl
                    = (hi_cipher_ctrl_aes_ccm_gcm *)ctrl_ex->param;

                fkey = (hi_u8 *)aes_ccm_gcm_ctrl->key;
                key_size = sizeof(aes_ccm_gcm_ctrl->key);
                iv = (hi_u8 *)aes_ccm_gcm_ctrl->iv;
                iv_len = aes_ccm_gcm_ctrl->iv_len;

                if (iv_len > AES_IV_SIZE) {
                    HI_LOG_ERROR("para set CIPHER ccm/gcm iv is invalid, iv_len:0x%x.\n", iv_len);
                    HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
                    return HI_ERR_CIPHER_INVALID_PARA;
                }

                tag_len = aes_ccm_gcm_ctrl->tag_len;
                key_len = aes_ccm_gcm_ctrl->key_len;
                ADDR_U64(aad) = aes_ccm_gcm_ctrl->a_phy_addr;
                a_len = aes_ccm_gcm_ctrl->a_len;
                usage = CIPHER_IV_CHANGE_ONE_PKG;
            } else {
                hi_cipher_ctrl_aes *aes_ctrl = (hi_cipher_ctrl_aes *)ctrl_ex->param;

                fkey = (hi_u8 *)aes_ctrl->even_key;
                skey = (hi_u8 *)aes_ctrl->odd_key;
                key_size = sizeof(aes_ctrl->even_key);
                iv = (hi_u8 *)aes_ctrl->iv;
                usage = aes_ctrl->change_flags.bit1_iv;
                key_len = aes_ctrl->key_len;
                bit_width = aes_ctrl->bit_width;
                iv_len = AES_IV_SIZE;
            }
            break;
        }
        case HI_CIPHER_ALG_SM1: {
            hi_cipher_ctrl_sm1 *sm1_ctrl = (hi_cipher_ctrl_sm1 *)ctrl_ex->param;
            hi_u8 sm1_key[SYMC_SM1_EK_AK_SIZE] = {0};

            ret = memcpy_s(sm1_key, sizeof(sm1_key), sm1_ctrl->ek, sizeof(sm1_ctrl->ek));
            if (ret != HI_SUCCESS) {
                HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
                return ret;
            }

            ret = memcpy_s(sm1_key + sizeof(sm1_ctrl->ek), sizeof(sm1_key) - sizeof(sm1_ctrl->ek), sm1_ctrl->ak,
                sizeof(sm1_ctrl->ak));
            if (ret != HI_SUCCESS) {
                HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
                crypto_zeroize(sm1_key, sizeof(sm1_key));
                return ret;
            }

            fkey = (hi_u8 *)sm1_key;
            skey = (hi_u8 *)sm1_ctrl->sk;
            key_size = sizeof(sm1_ctrl->ek);
            iv = (hi_u8 *)sm1_ctrl->iv;
            usage = sm1_ctrl->change_flags.bit1_iv;
            key_len = HI_CIPHER_KEY_DEFAULT;
            sm1_round = sm1_ctrl->sm1_round;
            bit_width = sm1_ctrl->bit_width;
            iv_len = AES_IV_SIZE;
            break;
        }
        case HI_CIPHER_ALG_SM4: {
            hi_cipher_ctrl_sm4 *sm4_ctrl = (hi_cipher_ctrl_sm4 *)ctrl_ex->param;

            fkey = (hi_u8 *)sm4_ctrl->key;
            key_size = sizeof(sm4_ctrl->key);
            iv = (hi_u8 *)sm4_ctrl->iv;
            usage = sm4_ctrl->change_flags.bit1_iv;
            key_len = HI_CIPHER_KEY_DEFAULT;
            iv_len = AES_IV_SIZE;
            break;
        }
        case HI_CIPHER_ALG_DMA: {
            break;
        }
        default:
            HI_LOG_ERROR("para set CIPHER alg is invalid.\n");
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(ctrl_ex->key_by_ca);
    HI_DBG_PRINT_U32(ctrl_ex->alg);
    HI_DBG_PRINT_U32(ctrl_ex->work_mode);
    HI_DBG_PRINT_U32(bit_width);
    HI_DBG_PRINT_U32(key_len);
    HI_DBG_PRINT_U32(sm1_round);
    HI_DBG_PRINT_U32(key_size);
    HI_DBG_PRINT_U32(iv_len);
    HI_DBG_PRINT_U32(usage);
    HI_DBG_PRINT_U64(ADDR_U64(aad));
    HI_DBG_PRINT_U32(a_len);
    HI_DBG_PRINT_U32(tag_len);

    ret = sys_symc_config(handle, ctrl_ex->key_by_ca, ctrl_ex->alg,
                          ctrl_ex->work_mode, bit_width, key_len, sm1_round,
                          fkey, skey, key_size, iv, iv_len,
                          usage, aad, a_len, tag_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_config, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Performs encryption.
 */
hi_s32 hi_mpi_cipher_encrypt(hi_handle handle, hi_size_t src_phy_addr, hi_size_t dest_phy_addr, hi_u32 byte_length)
{
    compat_addr input;
    compat_addr output;
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    CHECK_CIPHER_OPEN();

    ADDR_U64(input) = src_phy_addr;
    ADDR_U64(output) = dest_phy_addr;

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U64(src_phy_addr);
    HI_DBG_PRINT_U64(dest_phy_addr);
    HI_DBG_PRINT_U32(byte_length);

    ret = sys_symc_crypto(handle, input, output, byte_length, SYMC_OPERATION_ENCRYPT);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_crypto, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Performs descryption.
 */
hi_s32 hi_mpi_cipher_decrypt(hi_handle handle, hi_size_t src_phy_addr,
                           hi_size_t dest_phy_addr, hi_u32 byte_length)
{
    compat_addr input;
    compat_addr output;
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(src_phy_addr);
    HI_DBG_PRINT_U32(dest_phy_addr);
    HI_DBG_PRINT_U32(byte_length);

    CHECK_CIPHER_OPEN();

    ADDR_U64(input) = src_phy_addr;
    ADDR_U64(output) = dest_phy_addr;

    ret = sys_symc_crypto(handle, input, output, byte_length, SYMC_OPERATION_DECRYPT);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_crypto, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Performs encryption.
 */
hi_s32 hi_mpi_cipher_encrypt_vir(hi_handle handle,
                                 const hi_u8 *src_data,
                                 hi_u8 *dest_data,
                                 hi_u32 byte_length)
{
    compat_addr input;
    compat_addr output;
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(src_data == HI_NULL);
    HI_LOG_CHECK_PARAM(dest_data == HI_NULL);

    CHECK_CIPHER_OPEN();

    ADDR_VIA_CONST(input) = src_data;
    ADDR_VIA(output) = dest_data;

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(byte_length);

    ret = sys_symc_crypto(handle, input, output, byte_length, SYMC_OPERATION_ENCRYPT_VIA);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_crypto, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Performs decryption.
 */
hi_s32 hi_mpi_cipher_decrypt_vir(hi_handle handle,
                                 const hi_u8 *src_data,
                                 hi_u8 *dest_data,
                                 hi_u32 byte_length)
{
    compat_addr input;
    compat_addr output;
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(src_data == HI_NULL);
    HI_LOG_CHECK_PARAM(dest_data == HI_NULL);

    CHECK_CIPHER_OPEN();

    ADDR_VIA_CONST(input) = src_data;
    ADDR_VIA(output) = dest_data;

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(byte_length);

    ret = sys_symc_crypto(handle, input, output, byte_length, SYMC_OPERATION_DECRYPT_VIA);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_crypto, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Encrypt multiple packaged data.
 */
hi_s32 hi_mpi_cipher_encrypt_multi(hi_handle handle,
                                   const hi_cipher_data *data_pkg,
                                   hi_u32 data_pkg_num)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    CHECK_CIPHER_OPEN();

    HI_LOG_CHECK_PARAM(data_pkg == HI_NULL);
    HI_LOG_CHECK_PARAM((data_pkg->odd_key != HI_TRUE) && (data_pkg->odd_key != HI_FALSE));
    HI_LOG_CHECK_PARAM(data_pkg_num == 0x00);
    HI_LOG_CHECK_PARAM(data_pkg_num >= CIPHER_MAX_MULTIPAD_NUM);

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(data_pkg_num);

    ret = sys_symc_crypto_multi(handle, data_pkg, data_pkg_num, SYMC_OPERATION_ENCRYPT);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_crypto_multi, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
 * \brief Encrypt multiple packaged data.
 */
hi_s32 hi_mpi_cipher_decrypt_multi(hi_handle handle,
                                   const hi_cipher_data *data_pkg,
                                   hi_u32 data_pkg_num)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(data_pkg == HI_NULL);
    HI_LOG_CHECK_PARAM((data_pkg->odd_key != HI_TRUE) && (data_pkg->odd_key != HI_FALSE));
    HI_LOG_CHECK_PARAM(data_pkg_num == 0x00);
    HI_LOG_CHECK_PARAM(data_pkg_num >= CIPHER_MAX_MULTIPAD_NUM);

    HI_DBG_PRINT_U32(handle);
    HI_DBG_PRINT_U32(data_pkg_num);

    CHECK_CIPHER_OPEN();

    ret = sys_symc_crypto_multi(handle, data_pkg, data_pkg_num, SYMC_OPERATION_DECRYPT);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_crypto_multi, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_get_tag(hi_handle handle, hi_u8 *tag, hi_u32 *tag_len)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(tag == HI_NULL);
    HI_LOG_CHECK_PARAM(tag_len == HI_NULL);
    HI_LOG_CHECK_PARAM(*tag_len != MAX_TAG_LEN);

    HI_DBG_PRINT_U32(handle);

    CHECK_CIPHER_OPEN();

    ret = sys_aead_get_tag(handle, tag, tag_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_aead_get_tag, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_get_handle_config(hi_handle handle, hi_cipher_ctrl *ctrl)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctrl == HI_NULL);

    CHECK_CIPHER_OPEN();

    ret = sys_symc_get_config(handle, ctrl);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_symc_get_config, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_klad_encrypt_key(hi_cipher_ca_type root_key,
                                      hi_cipher_klad_target target,
                                      const hi_u8 *clean_key,
                                      hi_u8 *encrypt_key,
                                      hi_u32 key_len)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 i;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(root_key >= HI_CIPHER_KEY_SRC_BUTT);
    HI_LOG_CHECK_PARAM(target >= HI_CIPHER_KLAD_TARGET_BUTT);
    HI_LOG_CHECK_PARAM(clean_key == HI_NULL);
    HI_LOG_CHECK_PARAM(HI_NULL == encrypt_key);
    HI_LOG_CHECK_PARAM((key_len % AES_BLOCK_SIZE) != 0);
    HI_LOG_CHECK_PARAM(key_len == 0);

    CHECK_CIPHER_OPEN();

    for (i = 0; i < key_len; i += AES_BLOCK_SIZE) {
        ret = sys_klad_encrypt_key(root_key, target, clean_key + i, encrypt_key + i);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(sys_klad_encrypt_key, ret);
            return ret;
        }
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_get_random_number(hi_u32 *random_number)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(random_number == HI_NULL);

    ret = sys_trng_get_random(random_number, 0);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_hash_init(const hi_cipher_hash_atts *hash_atts,
                               hi_handle *hash_handle)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(hash_atts == HI_NULL);
    HI_LOG_CHECK_PARAM(hash_handle == HI_NULL);

    HI_DBG_PRINT_U32(hash_atts->sha_type);
    HI_DBG_PRINT_U32(hash_atts->hmac_key_len);

    CHECK_CIPHER_OPEN();

    ret = sys_hash_start(hash_handle,
                         hash_atts->sha_type,
                         hash_atts->hmac_key,
                         hash_atts->hmac_key_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_hash_start, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_hash_update(hi_handle hash_handle,
                                 const hi_u8 *input_data,
                                 hi_u32 input_data_len)
{
    hi_s32 ret = HI_FAILURE;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(input_data == HI_NULL);

    HI_DBG_PRINT_U32(hash_handle);
    HI_DBG_PRINT_U32(input_data_len);

    CHECK_CIPHER_OPEN();

    ret = sys_hash_update(hash_handle, input_data, input_data_len, HASH_CHUNCK_SRC_USER);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_hash_update, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_hash_final(hi_handle hash_handle, hi_u8 *output_hash)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 hash_len = 0x00;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(output_hash == HI_NULL);
    HI_LOG_CHECK_PARAM(hash_handle == HI_INVALID_HANDLE);

    HI_DBG_PRINT_U32(hash_handle);

    CHECK_CIPHER_OPEN();

    ret = sys_hash_finish(hash_handle, output_hash, &hash_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_hash_finish, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 check_rsa_pri_key(const hi_cipher_rsa_pri_key *pri_key)
{
    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(pri_key == HI_NULL);
    HI_LOG_CHECK_PARAM(pri_key->n == HI_NULL);
    HI_LOG_CHECK_PARAM(pri_key->n_len < RSA_MIN_KEY_LEN);
    HI_LOG_CHECK_PARAM(pri_key->n_len > RSA_MAX_KEY_LEN);

    if (pri_key->d == HI_NULL) {
        HI_LOG_CHECK_PARAM(pri_key->p == HI_NULL);
        HI_LOG_CHECK_PARAM(pri_key->q == HI_NULL);
        HI_LOG_CHECK_PARAM(pri_key->dp == HI_NULL);
        HI_LOG_CHECK_PARAM(pri_key->dq == HI_NULL);
        HI_LOG_CHECK_PARAM(pri_key->qp == HI_NULL);
        HI_LOG_CHECK_PARAM(pri_key->n_len / 2 != pri_key->p_len);
        HI_LOG_CHECK_PARAM(pri_key->n_len / 2 != pri_key->q_len);
        HI_LOG_CHECK_PARAM(pri_key->n_len / 2 != pri_key->dp_len);
        HI_LOG_CHECK_PARAM(pri_key->n_len / 2 != pri_key->dq_len);
        HI_LOG_CHECK_PARAM(pri_key->n_len / 2 != pri_key->qp_len);
    } else {
        HI_LOG_CHECK_PARAM(pri_key->n_len != pri_key->d_len);
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_rsa_public_encrypt(const hi_cipher_rsa_pub_enc *rsa_enc,
                                        const hi_u8 *input, hi_u32 in_len,
                                        hi_u8 *output, hi_u32 *out_len)
{
    hi_s32 ret = HI_FAILURE;
    cryp_rsa_key key;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(rsa_enc == HI_NULL);
    HI_LOG_CHECK_PARAM(input == HI_NULL);
    HI_LOG_CHECK_PARAM(output == HI_NULL);
    HI_LOG_CHECK_PARAM(out_len == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_enc->pub_key.n == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_enc->pub_key.e == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_enc->ca_type != HI_CIPHER_KEY_SRC_USER);
    HI_LOG_CHECK_PARAM(rsa_enc->pub_key.n_len < RSA_MIN_KEY_LEN);
    HI_LOG_CHECK_PARAM(rsa_enc->pub_key.n_len > RSA_MAX_KEY_LEN);
    HI_LOG_CHECK_PARAM(rsa_enc->pub_key.n_len < rsa_enc->pub_key.e_len);

    HI_DBG_PRINT_U32(rsa_enc->pub_key.n_len);
    HI_DBG_PRINT_U32(rsa_enc->scheme);
    HI_DBG_PRINT_U32(in_len);

    CHECK_CIPHER_OPEN();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public = HI_TRUE;
    key.klen = rsa_enc->pub_key.n_len;
    key.n = rsa_enc->pub_key.n;
    CIPHER_GET_PUB_EXPONENT(&key.e, rsa_enc);

    ret = sys_rsa_encrypt(&key, rsa_enc->scheme,
                          input, in_len,
                          output, out_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_rsa_encrypt, ret);
        crypto_zeroize(&key, sizeof(key));
        return ret;
    }

    crypto_zeroize(&key, sizeof(key));
    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;

}

hi_s32 hi_mpi_cipher_rsa_private_decrypt(const hi_cipher_rsa_pri_enc *rsa_dec,
                                         const hi_u8 *input, hi_u32 in_len,
                                         hi_u8 *output, hi_u32 *out_len)
{
    hi_s32 ret = HI_FAILURE;
    cryp_rsa_key key;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(rsa_dec == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_dec->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = check_rsa_pri_key(&rsa_dec->pri_key);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(check_rsa_pri_key, ret);
        return ret;
    }

    HI_LOG_CHECK_PARAM(input == HI_NULL);
    HI_LOG_CHECK_PARAM(output == HI_NULL);
    HI_LOG_CHECK_PARAM(out_len == HI_NULL);

    HI_DBG_PRINT_U32(rsa_dec->pri_key.n_len);
    HI_DBG_PRINT_U32(rsa_dec->scheme);
    HI_DBG_PRINT_U32(in_len);

    CHECK_CIPHER_OPEN();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_FALSE;
    key.klen    = rsa_dec->pri_key.n_len;
    key.n       = rsa_dec->pri_key.n;
    key.d       = rsa_dec->pri_key.d;
    key.ca_type = rsa_dec->ca_type;
    key.p       = rsa_dec->pri_key.p;
    key.q       = rsa_dec->pri_key.q;
    key.dp      = rsa_dec->pri_key.dp;
    key.dq      = rsa_dec->pri_key.dq;
    key.qp      = rsa_dec->pri_key.qp;

    ret = sys_rsa_decrypt(&key,
                          rsa_dec->scheme,
                          input, in_len,
                          output, out_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_rsa_decrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_rsa_private_encrypt(const hi_cipher_rsa_pri_enc *rsa_enc,
                                         const hi_u8 *input, hi_u32 in_len,
                                         hi_u8 *output, hi_u32 *out_len)
{
    hi_s32 ret = HI_FAILURE;
    cryp_rsa_key key;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(rsa_enc == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_enc->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = check_rsa_pri_key(&rsa_enc->pri_key);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(check_rsa_pri_key, ret);
        return ret;
    }

    HI_LOG_CHECK_PARAM(input == HI_NULL);
    HI_LOG_CHECK_PARAM(output == HI_NULL);
    HI_LOG_CHECK_PARAM(out_len == HI_NULL);

    HI_DBG_PRINT_U32(rsa_enc->pri_key.n_len);
    HI_DBG_PRINT_U32(rsa_enc->scheme);
    HI_DBG_PRINT_U32(in_len);

    CHECK_CIPHER_OPEN();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_FALSE;
    key.klen    = rsa_enc->pri_key.n_len;
    key.n       = rsa_enc->pri_key.n;
    key.d       = rsa_enc->pri_key.d;
    key.ca_type  = rsa_enc->ca_type;
    key.p       = rsa_enc->pri_key.p;
    key.q       = rsa_enc->pri_key.q;
    key.dp       = rsa_enc->pri_key.dp;
    key.dq       = rsa_enc->pri_key.dq;
    key.qp       = rsa_enc->pri_key.qp;

    ret = sys_rsa_encrypt(&key,
                          rsa_enc->scheme,
                          input, in_len,
                          output, out_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_rsa_encrypt, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}


hi_s32 hi_mpi_cipher_rsa_public_decrypt(const hi_cipher_rsa_pub_enc *rsa_dec,
                                        const hi_u8 *input, hi_u32 in_len,
                                        hi_u8 *output, hi_u32 *out_len)
{
    hi_s32 ret = HI_FAILURE;
    cryp_rsa_key key;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(rsa_dec == HI_NULL);
    HI_LOG_CHECK_PARAM(input == HI_NULL);
    HI_LOG_CHECK_PARAM(output == HI_NULL);
    HI_LOG_CHECK_PARAM(out_len == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_dec->ca_type != HI_CIPHER_KEY_SRC_USER);
    HI_LOG_CHECK_PARAM(rsa_dec->pub_key.n == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_dec->pub_key.e == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_dec->pub_key.n_len < RSA_MIN_KEY_LEN);
    HI_LOG_CHECK_PARAM(rsa_dec->pub_key.n_len > RSA_MAX_KEY_LEN);
    HI_LOG_CHECK_PARAM(rsa_dec->pub_key.n_len < rsa_dec->pub_key.e_len);

    HI_DBG_PRINT_U32(rsa_dec->scheme);
    HI_DBG_PRINT_U32(in_len);
    HI_DBG_PRINT_U32(rsa_dec->pub_key.n_len);

    CHECK_CIPHER_OPEN();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_TRUE;
    key.klen    = rsa_dec->pub_key.n_len;
    key.n       = rsa_dec->pub_key.n;
    CIPHER_GET_PUB_EXPONENT(&key.e, rsa_dec);

    ret = sys_rsa_decrypt(&key, rsa_dec->scheme, input, in_len, output, out_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_rsa_decrypt, ret);
        crypto_zeroize(&key, sizeof(key));
        return ret;
    }

    crypto_zeroize(&key, sizeof(key));
    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 mpi_cipher_hash(const hi_cipher_hash_type sha_type,
                              const hi_u8 *in_data, hi_u32 in_data_len,
                              hi_u8 *hash_data, hi_u32 *hash_len)
{
    hi_s32 ret = HI_FAILURE;
    hi_handle hash_id;

    ret = sys_hash_start(&hash_id, sha_type, HI_NULL, 0);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_hash_start, ret);
        return ret;
    }

    ret = sys_hash_update(hash_id, in_data, in_data_len, HASH_CHUNCK_SRC_USER);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_hash_update, ret);
        return ret;
    }

    ret = sys_hash_finish(hash_id, hash_data, hash_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_hash_finish, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_rsa_sign(const hi_cipher_rsa_sign *rsa_sign,
                              const hi_u8 *in_data, hi_u32 in_data_len,
                              const hi_u8 *hash_data,
                              hi_u8 *out_sign, hi_u32 *out_sign_len)
{
    hi_s32 ret = HI_FAILURE;
    cryp_rsa_key key;
    hi_u8 hash[HASH_RESULT_MAX_SIZE] = {0};
    const hi_u8 *ptr = HI_NULL;
    hi_u32 hash_len = 0;
    hi_cipher_hash_type sha_type = 0;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(rsa_sign == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_sign->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = check_rsa_pri_key(&rsa_sign->pri_key);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(check_rsa_pri_key, ret);
        return ret;
    }

    HI_LOG_CHECK_PARAM(out_sign == HI_NULL);
    HI_LOG_CHECK_PARAM(out_sign_len == HI_NULL);
    HI_LOG_CHECK_PARAM((in_data == HI_NULL) && (hash_data == HI_NULL));

    HI_DBG_PRINT_U32(rsa_sign->scheme);
    HI_DBG_PRINT_U32(rsa_sign->pri_key.n_len);

    CHECK_CIPHER_OPEN();

    ret = cipher_get_rsa_attr(rsa_sign->scheme, &hash_len, &sha_type);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cipher_get_rsa_attr, ret);
        return ret;
    }

    HI_DBG_PRINT_U32(hash_len);
    HI_DBG_PRINT_U32(sha_type);

    /* hash value of context,if NULL, compute hash = Hash(in_data */
    if (hash_data == HI_NULL) {
        ret = mpi_cipher_hash(sha_type, in_data, in_data_len, hash, &hash_len);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(mpi_cipher_hash, ret);
            return ret;
        }
        ptr = hash;
    } else {
        ptr = hash_data;
    }

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_FALSE;
    key.klen    = rsa_sign->pri_key.n_len;
    key.n       = rsa_sign->pri_key.n;
    key.d       = rsa_sign->pri_key.d;
    key.ca_type  = rsa_sign->ca_type;
    key.p       = rsa_sign->pri_key.p;
    key.q       = rsa_sign->pri_key.q;
    key.dp      = rsa_sign->pri_key.dp;
    key.dq      = rsa_sign->pri_key.dq;
    key.qp      = rsa_sign->pri_key.qp;

    ret = sys_rsa_sign_hash(&key, rsa_sign->scheme, ptr, hash_len, out_sign, out_sign_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_rsa_sign_hash, ret);
        return ret;
    }

    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 hi_mpi_cipher_rsa_verify(const hi_cipher_rsa_verify *rsa_verify,
                                const hi_u8 *in_data, hi_u32 in_data_len,
                                const hi_u8 *hash_data,
                                const hi_u8 *in_sign, hi_u32 in_sign_len)
{
    hi_s32 ret = HI_FAILURE;
    cryp_rsa_key key;
    hi_u8 hash[HASH_RESULT_MAX_SIZE] = {0};
    hi_u32 hash_len = 0;
    const hi_u8 *ptr = HI_NULL;
    hi_cipher_hash_type sha_type = 0;

    HI_UNF_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(rsa_verify == HI_NULL);
    HI_LOG_CHECK_PARAM(in_sign == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_verify->pub_key.n == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_verify->pub_key.e == HI_NULL);
    HI_LOG_CHECK_PARAM(rsa_verify->pub_key.n_len < RSA_MIN_KEY_LEN);
    HI_LOG_CHECK_PARAM(rsa_verify->pub_key.n_len > RSA_MAX_KEY_LEN);
    HI_LOG_CHECK_PARAM((in_data == HI_NULL) && (hash_data == HI_NULL));
    HI_LOG_CHECK_PARAM(rsa_verify->pub_key.n_len < rsa_verify->pub_key.e_len);

    HI_DBG_PRINT_U32(rsa_verify->scheme);
    HI_DBG_PRINT_U32(rsa_verify->pub_key.n_len);
    HI_DBG_PRINT_U32(rsa_verify->scheme);

    CHECK_CIPHER_OPEN();

    ret = cipher_get_rsa_attr(rsa_verify->scheme, &hash_len, &sha_type);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cipher_get_rsa_attr, ret);
        return ret;
    }

    HI_DBG_PRINT_U32(sha_type);

    /* hash value of context,if NULL, compute hash = Hash(in_data */
    if (hash_data == HI_NULL) {
        ret = mpi_cipher_hash(sha_type, in_data, in_data_len, hash, &hash_len);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(mpi_cipher_hash, ret);
            return ret;
        }
        ptr = hash;
    } else {
        ptr = hash_data;
    }

    HI_DBG_PRINT_U32(hash_len);

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_TRUE;
    key.klen    = rsa_verify->pub_key.n_len;
    key.n       = rsa_verify->pub_key.n;
    CIPHER_GET_PUB_EXPONENT(&key.e, rsa_verify);

    ret = sys_rsa_verify_hash(&key, rsa_verify->scheme, ptr, hash_len, in_sign, in_sign_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(sys_rsa_verify_hash, ret);
        crypto_zeroize(&key, sizeof(key));
        return ret;
    }

    crypto_zeroize(&key, sizeof(key));
    HI_UNF_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== Compat Code end ====*/
