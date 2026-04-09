/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for mpi cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __MPI_CIPHER_C__
#define __MPI_CIPHER_C__

#include "sys_cipher.h"
#include "user_osal_lib.h"

crypto_mutex g_cipher_mutex = PTHREAD_MUTEX_INITIALIZER;

#define hi_cipher_lock()        (void)crypto_mutex_lock(&g_cipher_mutex)
#define hi_cipher_unlock()      (void)crypto_mutex_unlock(&g_cipher_mutex)

#define CIPHER_MAX_MULTIPAD_NUM 5000
#define CENC_SUBSAMPLE_MAX_NUM  100
#define ECDH_MAX_KEY_LEN        72
#define CIPHER_INIT_MAX_NUM     0x7FFFFFFF
#define MIN_TAG_LEN             4
#define MAX_TAG_LEN             16
#define SYMC_SM1_EK_AK_SIZE     32

typedef struct {
    hi_cipher_hash_type sha_type;
    const hi_u8 *in;
    hi_u32 in_len;
    hi_u8 *hash;
    hi_u32 *hash_len;
    hi_u32 hash_buf_len;
} mpi_hash_pack;

/* handle of cipher device */
hi_s32 g_cipher_dev_fd = -1;

/* flag of cipher device
 * indicate the status of device that open or close
 * <0: close, 0: open>0: multiple initialization
 */
static hi_s32 g_cipher_init_counter = -1;

/* check the device of cipher whether already opend or not */
#define chk_cipher_open_err_return() \
    do { \
        if (g_cipher_init_counter < 0) { \
            hi_log_error("cipher init counter %d\n", g_cipher_init_counter); \
            hi_log_print_err_code(HI_ERR_CIPHER_NOT_INIT); \
            return HI_ERR_CIPHER_NOT_INIT; \
        } \
    } while (0)

/*
 * Read E in public key from arry to U32,
 * so only use last byte0~byte3, others are zero
 */
#define cipher_get_pub_exponent(pub_e, pub_rsa) \
    do { \
        hi_u8 *_buf = (pub_rsa)->pub_key.e; \
        hi_u8 *_pub = (hi_u8 *)(pub_e); \
        hi_u32 _len = (pub_rsa)->pub_key.e_len; \
        hi_u32 _i; \
        for (_i = 0; _i < crypto_min(WORD_WIDTH, _len); _i++) { \
            _pub[WORD_WIDTH -_i - 1] = _buf[_len - _i - 1]; \
        } \
    } while (0)

/*
 * brief  Init the cipher device.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_init(hi_void)
{
    hi_unf_func_enter();

    hi_cipher_lock();

    if (g_cipher_init_counter >= CIPHER_INIT_MAX_NUM) {
        hi_cipher_unlock();

        hi_log_print_err_code(HI_ERR_CIPHER_OVERFLOW);
        return HI_ERR_CIPHER_OVERFLOW;
    }

    if (g_cipher_init_counter >= 0) {
        g_cipher_init_counter++;
        hi_cipher_unlock();

        hi_unf_func_exit();
        return HI_SUCCESS;
    }

    g_cipher_dev_fd = crypto_open("/dev/"UMAP_DEVNAME_CIPHER, O_RDWR, 0);
    if (g_cipher_dev_fd < 0) {
        hi_cipher_unlock();

        hi_log_print_func_err(crypto_open, HI_ERR_CIPHER_FAILED_INIT);
        return HI_ERR_CIPHER_FAILED_INIT;
    }

    g_cipher_init_counter = 0;

    hi_cipher_unlock();

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief  Deinit the cipher device.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_deinit(hi_void)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_cipher_lock();

    if (g_cipher_init_counter < 0) {
        hi_cipher_unlock();

        hi_unf_func_exit();
        return HI_SUCCESS;
    }

    if (g_cipher_init_counter > 0) {
        g_cipher_init_counter--;

        hi_cipher_unlock();

        hi_unf_func_exit();
        return HI_SUCCESS;
    }

    ret = crypto_close(g_cipher_dev_fd);
    if (ret != HI_SUCCESS) {
        hi_cipher_unlock();
        hi_log_print_func_err(crypto_close, ret);
        g_cipher_dev_fd = -1;
        return ret;
    }

    g_cipher_init_counter = -1;

    hi_cipher_unlock();

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Obtain a cipher handle for encryption and decryption.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_create_handle(hi_handle *handle, const hi_cipher_attr *cipher_attr)
{
    hi_s32 ret;
    hi_u32 id = 0;

    hi_unf_func_enter();

    hi_log_chk_param_return(handle == HI_NULL);
    hi_log_chk_param_return(cipher_attr == HI_NULL);
    hi_log_chk_param_return(cipher_attr->cipher_type >= HI_CIPHER_TYPE_BUTT);

    hi_dbg_print_u32(cipher_attr->cipher_type);

    chk_cipher_open_err_return();

    ret = sys_symc_create(&id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_create, ret);
        return ret;
    }

    *handle = (hi_handle)id;

    hi_unf_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cipher_get_rsa_sign_attr(hi_u32 in_scheme, hi_u32 *hash_len, hi_cipher_hash_type *sha_type)
{
    hi_cipher_rsa_sign_scheme scheme = (hi_cipher_rsa_sign_scheme)in_scheme;

    hi_unf_func_enter();

    switch (scheme) {
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1: {
            *hash_len = SHA1_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA1;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224:
            *hash_len = SHA224_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA224;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256: {
            *hash_len = SHA256_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA256;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384: {
            *hash_len = SHA384_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA384;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512:
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512: {
            *hash_len = SHA512_RESULT_SIZE;
            *sha_type = HI_CIPHER_HASH_TYPE_SHA512;
            break;
        }
        default: {
            hi_err_print_u32(scheme);
            hi_log_print_err_code(HI_ERR_CIPHER_UNAVAILABLE);
            return HI_ERR_CIPHER_UNAVAILABLE;
        }
    }

    hi_dbg_print_u32(*hash_len);
    hi_dbg_print_u32((hi_u32)(*sha_type));
    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Destroy the existing cipher handle.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_destroy_handle(hi_handle handle)
{
    hi_s32 ret;

    hi_unf_func_enter();

    chk_cipher_open_err_return();

    ret = sys_symc_destroy(handle);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_destroy, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

static hi_s32 mpi_symc_chk_cfg(hi_handle handle, const hi_cipher_ctrl *ctrl, hi_u32 *hard_key)
{
    hi_log_chk_param_return(ctrl == HI_NULL);
    hi_log_chk_param_return(handle == HI_INVALID_HANDLE);
    hi_log_chk_param_return(hard_key == HI_NULL);

    if ((ctrl->work_mode == HI_CIPHER_WORK_MODE_CCM) || (ctrl->work_mode == HI_CIPHER_WORK_MODE_GCM)) {
        hi_log_error("Invalid work mode ccm or gcm by config handle.\n");
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((ctrl->key_by_ca != HI_TRUE) && (ctrl->key_by_ca != HI_FALSE) && (ctrl->alg != HI_CIPHER_ALG_DMA)) {
        hi_log_error("Invalid key by ca, you should set HI_TRUE or HI_FALSE.\n");
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if (ctrl->key_by_ca == HI_TRUE) {
        if ((int)ctrl->ca_type >= (int)HI_CIPHER_KEY_SRC_BUTT ||
            (int)ctrl->ca_type < (int)HI_CIPHER_KEY_SRC_USER) {
            hi_log_error("Invalid ca type, key ca type is HI_TRUE.\n");
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
        *hard_key  = ((hi_u32)ctrl->ca_type & 0xFF) << BITS_IN_BYTE;
        *hard_key |= 0x01;
    }

    return HI_SUCCESS;
}

/*
 * brief Configures the cipher control information.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_cfg_handle(hi_handle handle, const hi_cipher_ctrl *ctrl)
{
    hi_s32 ret;
    sys_symc_ctrl sys_ctrl;

    hi_unf_func_enter();

    ret = memset_s(&sys_ctrl, sizeof(sys_ctrl), 0, sizeof(sys_ctrl));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = mpi_symc_chk_cfg(handle, ctrl, &sys_ctrl.hard_key);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_symc_chk_cfg, ret);
        return ret;
    }

    chk_cipher_open_err_return();

    if ((ctrl->alg == HI_CIPHER_ALG_3DES) || (ctrl->alg == HI_CIPHER_ALG_DES)) {
        sys_ctrl.iv_len = DES_IV_SIZE;
    } else {
        sys_ctrl.iv_len = AES_IV_SIZE;
    }

    sys_ctrl.id = (hi_u32)handle;
    sys_ctrl.alg = ctrl->alg;
    sys_ctrl.work_mode = ctrl->work_mode;
    sys_ctrl.bit_width = ctrl->bit_width;
    sys_ctrl.key_len = ctrl->key_len;
    sys_ctrl.fkey = (hi_u8 *)ctrl->key;
    sys_ctrl.skey = HI_NULL;
    sys_ctrl.klen = SYMC_KEY_SIZE;
    sys_ctrl.iv = (hi_u8 *)ctrl->iv;
    sys_ctrl.iv_usage = ctrl->chg_flags.bits_iv;

    ret = sys_symc_cfg(&sys_ctrl);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_cfg, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

static hi_s32 mpi_symc_chk_cfg_ex(const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_log_chk_param_return(ctrl_ex == HI_NULL);
    hi_log_chk_param_return((ctrl_ex->param == HI_NULL) && (ctrl_ex->alg != HI_CIPHER_ALG_DMA));

    if ((ctrl_ex->key_by_ca != HI_TRUE) && (ctrl_ex->key_by_ca != HI_FALSE) && (ctrl_ex->alg != HI_CIPHER_ALG_DMA)) {
        hi_log_error("Invalid bKeyByCA, you should set HI_TRUE or HI_FALSE.\n");
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_void mpi_symc_set_des_cfg(sys_symc_ctrl *sys_ctrl, const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_cipher_ctrl_des *des_ctrl = (hi_cipher_ctrl_des *)ctrl_ex->param;

    sys_ctrl->fkey = (hi_u8 *)des_ctrl->key;
    sys_ctrl->klen = sizeof(des_ctrl->key);
    sys_ctrl->iv = (hi_u8 *)des_ctrl->iv;
    sys_ctrl->iv_usage = des_ctrl->chg_flags.bits_iv;
    sys_ctrl->iv_len = DES_IV_SIZE;
    sys_ctrl->bit_width = des_ctrl->bit_width;
}

static hi_void mpi_symc_set_3des_cfg(sys_symc_ctrl *sys_ctrl, const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_cipher_ctrl_3des *tdes_ctrl = (hi_cipher_ctrl_3des *)ctrl_ex->param;

    sys_ctrl->fkey = (hi_u8 *)tdes_ctrl->key;
    sys_ctrl->klen = sizeof(tdes_ctrl->key);
    sys_ctrl->iv = (hi_u8 *)tdes_ctrl->iv;
    sys_ctrl->iv_usage = tdes_ctrl->chg_flags.bits_iv;
    sys_ctrl->key_len = tdes_ctrl->key_len;
    sys_ctrl->iv_len = DES_IV_SIZE;
    sys_ctrl->bit_width = tdes_ctrl->bit_width;
}

static hi_s32 mpi_symc_set_aes_cfg(sys_symc_ctrl *sys_ctrl, const hi_cipher_ctrl_ex *ctrl_ex)
{
    if ((ctrl_ex->work_mode == HI_CIPHER_WORK_MODE_CCM) || (ctrl_ex->work_mode == HI_CIPHER_WORK_MODE_GCM)) {
        hi_cipher_ctrl_aes_ccm_gcm *aes_ccm_gcm_ctrl = (hi_cipher_ctrl_aes_ccm_gcm *)ctrl_ex->param;

        sys_ctrl->fkey = (hi_u8 *)aes_ccm_gcm_ctrl->key;
        sys_ctrl->klen = sizeof(aes_ccm_gcm_ctrl->key);
        sys_ctrl->iv = (hi_u8 *)aes_ccm_gcm_ctrl->iv;
        sys_ctrl->iv_len = aes_ccm_gcm_ctrl->iv_len;

        if (sys_ctrl->iv_len > AES_IV_SIZE) {
            hi_log_error("para set CIPHER ccm/gcm iv is invalid, iv_len:0x%x.\n", sys_ctrl->iv_len);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }

        sys_ctrl->tag_len = aes_ccm_gcm_ctrl->tag_len;
        sys_ctrl->key_len = aes_ccm_gcm_ctrl->key_len;
        addr_u64(sys_ctrl->aad) = aes_ccm_gcm_ctrl->aad_phys_addr;
        sys_ctrl->aad_len = aes_ccm_gcm_ctrl->aad_len;
        sys_ctrl->iv_usage = HI_CIPHER_IV_CHG_ONE_PACK;
    } else {
        hi_cipher_ctrl_aes *aes_ctrl = (hi_cipher_ctrl_aes *)ctrl_ex->param;

        sys_ctrl->fkey = (hi_u8 *)aes_ctrl->even_key;
        sys_ctrl->skey = (hi_u8 *)aes_ctrl->odd_key;
        sys_ctrl->klen = sizeof(aes_ctrl->even_key);
        sys_ctrl->iv = (hi_u8 *)aes_ctrl->iv;
        sys_ctrl->iv_usage = aes_ctrl->chg_flags.bits_iv;
        sys_ctrl->key_len = aes_ctrl->key_len;
        sys_ctrl->bit_width = aes_ctrl->bit_width;
        sys_ctrl->iv_len = AES_IV_SIZE;
    }

    return HI_SUCCESS;
}

static hi_s32 mpi_symc_set_sm1_cfg(sys_symc_ctrl *sys_ctrl, const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_s32 ret;
    hi_cipher_ctrl_sm1 *sm1_ctrl = (hi_cipher_ctrl_sm1 *)ctrl_ex->param;
    hi_u8 sm1_key[SYMC_SM1_EK_AK_SIZE] = {0};

    ret = memcpy_s(sm1_key, sizeof(sm1_key), sm1_ctrl->ek, sizeof(sm1_ctrl->ek));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(memcpy_s, ret);
        return ret;
    }

    ret = memcpy_s(sm1_key + sizeof(sm1_ctrl->ek), sizeof(sm1_key) - sizeof(sm1_ctrl->ek), sm1_ctrl->ak,
        sizeof(sm1_ctrl->ak));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(memcpy_s, ret);
        crypto_zeroize(sm1_key, sizeof(sm1_key));
        return ret;
    }

    sys_ctrl->fkey = (hi_u8 *)sm1_key;
    sys_ctrl->skey = (hi_u8 *)sm1_ctrl->sk;
    sys_ctrl->klen = sizeof(sm1_ctrl->ek);
    sys_ctrl->iv = (hi_u8 *)sm1_ctrl->iv;
    sys_ctrl->iv_usage = sm1_ctrl->chg_flags.bits_iv;
    sys_ctrl->key_len = HI_CIPHER_KEY_DEFAULT;
    sys_ctrl->sm1_round_num = sm1_ctrl->sm1_round;
    sys_ctrl->bit_width = sm1_ctrl->bit_width;
    sys_ctrl->iv_len = AES_IV_SIZE;

    return HI_SUCCESS;
}

static hi_void mpi_symc_set_sm4_cfg(sys_symc_ctrl *sys_ctrl, const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_cipher_ctrl_sm4 *sm4_ctrl = (hi_cipher_ctrl_sm4 *)ctrl_ex->param;

    sys_ctrl->fkey = (hi_u8 *)sm4_ctrl->key;
    sys_ctrl->klen = sizeof(sm4_ctrl->key);
    sys_ctrl->iv = (hi_u8 *)sm4_ctrl->iv;
    sys_ctrl->iv_usage = sm4_ctrl->chg_flags.bits_iv;
    sys_ctrl->key_len = HI_CIPHER_KEY_DEFAULT;
    sys_ctrl->iv_len = AES_IV_SIZE;
}

static hi_s32 mpi_symc_set_cfg_ex(sys_symc_ctrl *sys_ctrl, hi_handle handle, const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_s32 ret;

    sys_ctrl->id = handle;
    sys_ctrl->key_len = HI_CIPHER_KEY_DEFAULT;
    sys_ctrl->bit_width = HI_CIPHER_BIT_WIDTH_128BIT;
    sys_ctrl->sm1_round_num = HI_CIPHER_SM1_ROUND_BUTT;
    sys_ctrl->alg = ctrl_ex->alg;
    sys_ctrl->work_mode = ctrl_ex->work_mode;
    sys_ctrl->hard_key = ctrl_ex->key_by_ca;

    /* ****************************************************************************
     * for AES, the pointer should point to hi_cipher_ctrl_aes;
     * for AES_CCM or AES_GCM, the pointer should point to hi_cipher_ctrl_aes_ccm_gcm;
     * for DES, the pointer should point to hi_cipher_ctrl_des;
     * for 3DES, the pointer should point to hi_cipher_ctrl_3des;
     * for SM1, the pointer should point to hi_cipher_ctrl_sm1;
     * for SM4, the pointer should point to hi_cipher_ctrl_sm4;
     */
    switch (ctrl_ex->alg) {
        case HI_CIPHER_ALG_DES: {
            mpi_symc_set_des_cfg(sys_ctrl, ctrl_ex);
            break;
        }
        case HI_CIPHER_ALG_3DES: {
            mpi_symc_set_3des_cfg(sys_ctrl, ctrl_ex);
            break;
        }
        case HI_CIPHER_ALG_AES: {
            ret = mpi_symc_set_aes_cfg(sys_ctrl, ctrl_ex);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(mpi_symc_set_aes_cfg, ret);
                return ret;
            }
            break;
        }
        case HI_CIPHER_ALG_SM1: {
            ret = mpi_symc_set_sm1_cfg(sys_ctrl, ctrl_ex);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(mpi_symc_set_sm1_cfg, ret);
                return ret;
            }
            break;
        }
        case HI_CIPHER_ALG_SM4: {
            mpi_symc_set_sm4_cfg(sys_ctrl, ctrl_ex);
            break;
        }
        case HI_CIPHER_ALG_DMA: {
            break;
        }
        default:
            hi_log_error("para set cipher alg is invalid.\n");
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

/*
 * brief Configures the cipher expand control information.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_cfg_handle_ex(hi_handle handle, const hi_cipher_ctrl_ex *ctrl_ex)
{
    hi_s32 ret;
    sys_symc_ctrl sys_ctrl;

    hi_unf_func_enter();

    chk_cipher_open_err_return();

    ret = mpi_symc_chk_cfg_ex(ctrl_ex);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_symc_chk_cfg_ex, ret);
        return ret;
    }

    ret = memset_s(&sys_ctrl, sizeof(sys_ctrl), 0, sizeof(sys_ctrl));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = mpi_symc_set_cfg_ex(&sys_ctrl, handle, ctrl_ex);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_symc_set_cfg_ex, ret);
        return ret;
    }

    ret = sys_symc_cfg(&sys_ctrl);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_cfg, ret);
        crypto_zeroize(&sys_ctrl, sizeof(sys_ctrl));
        return ret;
    }

    crypto_zeroize(&sys_ctrl, sizeof(sys_ctrl));
    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Performs encryption.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_encrypt(hi_handle handle, hi_size_t src_phys_addr, hi_size_t dst_phys_addr,
    hi_u32 byte_len)
{
    compat_addr in;
    compat_addr out;
    hi_s32 ret;

    hi_unf_func_enter();

    chk_cipher_open_err_return();

    addr_u64(in) = src_phys_addr;
    addr_u64(out) = dst_phys_addr;

    hi_dbg_print_u32(handle);
    hi_dbg_print_u64(src_phys_addr);
    hi_dbg_print_u64(dst_phys_addr);
    hi_dbg_print_u32(byte_len);

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_ENCRYPT);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_crypto, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Performs descryption.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_decrypt(hi_handle handle, hi_size_t src_phys_addr, hi_size_t dst_phys_addr,
    hi_u32 byte_len)
{
    compat_addr in;
    compat_addr out;
    hi_s32 ret;

    hi_unf_func_enter();

    hi_dbg_print_u32(handle);
    hi_dbg_print_u32(src_phys_addr);
    hi_dbg_print_u32(dst_phys_addr);
    hi_dbg_print_u32(byte_len);

    chk_cipher_open_err_return();

    addr_u64(in) = src_phys_addr;
    addr_u64(out) = dst_phys_addr;

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_DECRYPT);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_crypto, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Performs encryption.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_encrypt_virt(hi_handle handle, const hi_u8 *src_data, hi_u8 *dst_data,
    hi_u32 byte_len)
{
    compat_addr in;
    compat_addr out;
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(src_data == HI_NULL);
    hi_log_chk_param_return(dst_data == HI_NULL);

    chk_cipher_open_err_return();

    addr_via_const(in) = src_data;
    addr_via(out) = dst_data;

    hi_dbg_print_u32(handle);
    hi_dbg_print_u32(byte_len);

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_ENCRYPT_VIA);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_crypto, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Performs decryption.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_decrypt_virt(hi_handle handle, const hi_u8 *src_data, hi_u8 *dst_data,
    hi_u32 byte_len)
{
    compat_addr in;
    compat_addr out;
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(src_data == HI_NULL);
    hi_log_chk_param_return(dst_data == HI_NULL);

    chk_cipher_open_err_return();

    addr_via_const(in) = src_data;
    addr_via(out) = dst_data;

    hi_dbg_print_u32(handle);
    hi_dbg_print_u32(byte_len);

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_DECRYPT_VIA);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_crypto, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Encrypt multiple packaged data.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_encrypt_multi_pack(hi_handle handle, const hi_cipher_data *pack, hi_u32 pack_num)
{
    hi_s32 ret;

    hi_unf_func_enter();

    chk_cipher_open_err_return();

    hi_log_chk_param_return(pack == HI_NULL);
    hi_log_chk_param_return((pack->odd_key != HI_TRUE) && (pack->odd_key != HI_FALSE));
    hi_log_chk_param_return(pack_num == 0x00);
    hi_log_chk_param_return(pack_num >= CIPHER_MAX_MULTIPAD_NUM);

    hi_dbg_print_u32(handle);
    hi_dbg_print_u32(pack_num);

    ret = sys_symc_crypto_multi(handle, pack, pack_num, SYMC_OPERATION_ENCRYPT);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_crypto_multi, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

/*
 * brief Encrypt multiple packaged data.
 */
MPI_STATIC hi_s32 hi_mpi_cipher_decrypt_multi_pack(hi_handle handle, const hi_cipher_data *pack, hi_u32 pack_num)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(pack == HI_NULL);
    hi_log_chk_param_return((pack->odd_key != HI_TRUE) && (pack->odd_key != HI_FALSE));
    hi_log_chk_param_return(pack_num == 0x00);
    hi_log_chk_param_return(pack_num >= CIPHER_MAX_MULTIPAD_NUM);

    hi_dbg_print_u32(handle);
    hi_dbg_print_u32(pack_num);

    chk_cipher_open_err_return();

    ret = sys_symc_crypto_multi(handle, pack, pack_num, SYMC_OPERATION_DECRYPT);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_crypto_multi, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_get_tag(hi_handle handle, hi_u8 *tag, hi_u32 *tag_len)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(tag == HI_NULL);
    hi_log_chk_param_return(tag_len == HI_NULL);
    hi_log_chk_param_return(*tag_len != MAX_TAG_LEN);

    hi_dbg_print_u32(handle);

    chk_cipher_open_err_return();

    ret = sys_aead_get_tag(handle, tag, tag_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_aead_get_tag, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_get_handle_cfg(hi_handle handle, hi_cipher_ctrl *ctrl)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(ctrl == HI_NULL);

    chk_cipher_open_err_return();

    ret = sys_symc_get_cfg(handle, ctrl);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_get_cfg, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_klad_encrypt_key(hi_cipher_ca_type root_key, hi_cipher_klad_target target,
    const hi_u8 *clean_key, hi_u8 *encrypt_key, hi_u32 key_len)
{
    hi_s32 ret;
    hi_u32 i;

    hi_unf_func_enter();

    hi_log_chk_param_return(root_key >= HI_CIPHER_KEY_SRC_BUTT);
    hi_log_chk_param_return(target >= HI_CIPHER_KLAD_TARGET_BUTT);
    hi_log_chk_param_return(clean_key == HI_NULL);
    hi_log_chk_param_return(encrypt_key == HI_NULL);
    hi_log_chk_param_return((key_len % AES_BLOCK_SIZE) != 0);
    hi_log_chk_param_return(key_len == 0);

    chk_cipher_open_err_return();

    for (i = 0; i < key_len; i += AES_BLOCK_SIZE) {
        ret = sys_klad_encrypt_key(root_key, target, clean_key + i, encrypt_key + i, AES_BLOCK_SIZE);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(sys_klad_encrypt_key, ret);
            return ret;
        }
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_get_random_num(hi_u32 *random_num)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(random_num == HI_NULL);

    ret = sys_trng_get_random(random_num, 0);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_hash_init(const hi_cipher_hash_attr *hash_attr, hi_handle *hash_handle)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(hash_attr == HI_NULL);
    hi_log_chk_param_return(hash_handle == HI_NULL);

    hi_dbg_print_u32(hash_attr->sha_type);
    hi_dbg_print_u32(hash_attr->hmac_key_len);

    chk_cipher_open_err_return();

    ret = sys_hash_start(hash_handle, hash_attr->sha_type, hash_attr->hmac_key, hash_attr->hmac_key_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_hash_start, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_hash_update(hi_handle hash_handle, const hi_u8 *in_data, hi_u32 in_data_len)
{
    hi_s32 ret;

    hi_unf_func_enter();

    hi_log_chk_param_return(in_data == HI_NULL);

    hi_dbg_print_u32(hash_handle);
    hi_dbg_print_u32(in_data_len);

    chk_cipher_open_err_return();

    ret = sys_hash_update(hash_handle, in_data, in_data_len, HASH_CHUNCK_SRC_USER);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_hash_update, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_hash_final(hi_handle hash_handle, hi_u8 *out_hash)
{
    hi_s32 ret;
    hi_u32 hash_len = 0x00;

    hi_unf_func_enter();

    hi_log_chk_param_return(out_hash == HI_NULL);
    hi_log_chk_param_return(hash_handle == HI_INVALID_HANDLE);

    hi_dbg_print_u32(hash_handle);

    chk_cipher_open_err_return();

    ret = sys_hash_finish(hash_handle, out_hash, &hash_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_hash_finish, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

static hi_s32 mpi_chk_rsa_private_key(const hi_cipher_rsa_private_key *private_key)
{
    hi_log_func_enter();

    hi_log_chk_param_return(private_key == HI_NULL);
    hi_log_chk_param_return(private_key->n == HI_NULL);
    hi_log_chk_param_return(private_key->n_len < RSA_MIN_KEY_LEN);
    hi_log_chk_param_return(private_key->n_len > RSA_MAX_KEY_LEN);

    if (private_key->d == HI_NULL) {
        hi_log_chk_param_return(private_key->p == HI_NULL);
        hi_log_chk_param_return(private_key->q == HI_NULL);
        hi_log_chk_param_return(private_key->dp == HI_NULL);
        hi_log_chk_param_return(private_key->dq == HI_NULL);
        hi_log_chk_param_return(private_key->qp == HI_NULL);
        hi_log_chk_param_return(private_key->n_len / MUL_VAL_2 != private_key->p_len);
        hi_log_chk_param_return(private_key->n_len / MUL_VAL_2 != private_key->q_len);
        hi_log_chk_param_return(private_key->n_len / MUL_VAL_2 != private_key->dp_len);
        hi_log_chk_param_return(private_key->n_len / MUL_VAL_2 != private_key->dq_len);
        hi_log_chk_param_return(private_key->n_len / MUL_VAL_2 != private_key->qp_len);
    } else {
        hi_log_chk_param_return(private_key->n_len != private_key->d_len);
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_pub_encrypt(const hi_cipher_rsa_pub_encrypt *rsa_encrypt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len)
{
    hi_s32 ret;
    cryp_rsa_key key;
    hi_cipher_rsa_crypt crypt;

    hi_unf_func_enter();

    hi_log_chk_param_return(rsa_encrypt == HI_NULL);
    hi_log_chk_param_return(in == HI_NULL);
    hi_log_chk_param_return(out == HI_NULL);
    hi_log_chk_param_return(out_len == HI_NULL);
    hi_log_chk_param_return(rsa_encrypt->pub_key.n == HI_NULL);
    hi_log_chk_param_return(rsa_encrypt->pub_key.e == HI_NULL);
    hi_log_chk_param_return(rsa_encrypt->ca_type != HI_CIPHER_KEY_SRC_USER);
    hi_log_chk_param_return(rsa_encrypt->pub_key.n_len < RSA_MIN_KEY_LEN);
    hi_log_chk_param_return(rsa_encrypt->pub_key.n_len > RSA_MAX_KEY_LEN);
    hi_log_chk_param_return(rsa_encrypt->pub_key.n_len < rsa_encrypt->pub_key.e_len);

    hi_dbg_print_u32(rsa_encrypt->pub_key.n_len);
    hi_dbg_print_u32(rsa_encrypt->scheme);
    hi_dbg_print_u32(in_len);

    chk_cipher_open_err_return();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public = HI_TRUE;
    key.klen = rsa_encrypt->pub_key.n_len;
    key.n = rsa_encrypt->pub_key.n;
    cipher_get_pub_exponent(&key.e, rsa_encrypt);

    crypt.in = in;
    crypt.in_len = in_len;
    crypt.out = out;
    crypt.out_buf_len = key.klen;
    crypt.out_len = out_len;
    ret = sys_rsa_encrypt(&key, rsa_encrypt->scheme, &crypt);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_rsa_encrypt, ret);
        crypto_zeroize(&key, sizeof(key));
        return ret;
    }

    crypto_zeroize(&key, sizeof(key));
    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_private_decrypt(const hi_cipher_rsa_private_encrypt *rsa_decrypt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len)
{
    hi_s32 ret;
    cryp_rsa_key key;
    hi_cipher_rsa_crypt crypt;

    hi_unf_func_enter();

    hi_log_chk_param_return(rsa_decrypt == HI_NULL);
    hi_log_chk_param_return(rsa_decrypt->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = mpi_chk_rsa_private_key(&rsa_decrypt->private_key);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_chk_rsa_private_key, ret);
        return ret;
    }

    hi_log_chk_param_return(in == HI_NULL);
    hi_log_chk_param_return(out == HI_NULL);
    hi_log_chk_param_return(out_len == HI_NULL);

    hi_dbg_print_u32(rsa_decrypt->private_key.n_len);
    hi_dbg_print_u32(rsa_decrypt->scheme);
    hi_dbg_print_u32(in_len);

    chk_cipher_open_err_return();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_FALSE;
    key.klen    = rsa_decrypt->private_key.n_len;
    key.n       = rsa_decrypt->private_key.n;
    key.d       = rsa_decrypt->private_key.d;
    key.ca_type = rsa_decrypt->ca_type;
    key.p       = rsa_decrypt->private_key.p;
    key.q       = rsa_decrypt->private_key.q;
    key.dp      = rsa_decrypt->private_key.dp;
    key.dq      = rsa_decrypt->private_key.dq;
    key.qp      = rsa_decrypt->private_key.qp;

    crypt.in = in;
    crypt.in_len = in_len;
    crypt.out = out;
    crypt.out_buf_len = key.klen;
    crypt.out_len = out_len;
    ret = sys_rsa_decrypt(&key, rsa_decrypt->scheme, &crypt);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_rsa_decrypt, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_private_encrypt(const hi_cipher_rsa_private_encrypt *rsa_encrypt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len)
{
    hi_s32 ret;
    cryp_rsa_key key;
    hi_cipher_rsa_crypt crypt;

    hi_unf_func_enter();

    hi_log_chk_param_return(rsa_encrypt == HI_NULL);
    hi_log_chk_param_return(rsa_encrypt->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = mpi_chk_rsa_private_key(&rsa_encrypt->private_key);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_chk_rsa_private_key, ret);
        return ret;
    }

    hi_log_chk_param_return(in == HI_NULL);
    hi_log_chk_param_return(out == HI_NULL);
    hi_log_chk_param_return(out_len == HI_NULL);

    hi_dbg_print_u32(rsa_encrypt->private_key.n_len);
    hi_dbg_print_u32(rsa_encrypt->scheme);
    hi_dbg_print_u32(in_len);

    chk_cipher_open_err_return();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_FALSE;
    key.klen    = rsa_encrypt->private_key.n_len;
    key.n       = rsa_encrypt->private_key.n;
    key.d       = rsa_encrypt->private_key.d;
    key.ca_type = rsa_encrypt->ca_type;
    key.p       = rsa_encrypt->private_key.p;
    key.q       = rsa_encrypt->private_key.q;
    key.dp      = rsa_encrypt->private_key.dp;
    key.dq      = rsa_encrypt->private_key.dq;
    key.qp      = rsa_encrypt->private_key.qp;

    crypt.in = in;
    crypt.in_len = in_len;
    crypt.out = out;
    crypt.out_buf_len = key.klen;
    crypt.out_len = out_len;
    ret = sys_rsa_encrypt(&key, rsa_encrypt->scheme, &crypt);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_rsa_encrypt, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_pub_decrypt(const hi_cipher_rsa_pub_encrypt *rsa_decrypt, const hi_u8 *in,
    hi_u32 in_len, hi_u8 *out, hi_u32 *out_len)
{
    hi_s32 ret;
    cryp_rsa_key key;
    hi_cipher_rsa_crypt crypt;

    hi_unf_func_enter();

    hi_log_chk_param_return(rsa_decrypt == HI_NULL);
    hi_log_chk_param_return(in == HI_NULL);
    hi_log_chk_param_return(out == HI_NULL);
    hi_log_chk_param_return(out_len == HI_NULL);
    hi_log_chk_param_return(rsa_decrypt->ca_type != HI_CIPHER_KEY_SRC_USER);
    hi_log_chk_param_return(rsa_decrypt->pub_key.n == HI_NULL);
    hi_log_chk_param_return(rsa_decrypt->pub_key.e == HI_NULL);
    hi_log_chk_param_return(rsa_decrypt->pub_key.n_len < RSA_MIN_KEY_LEN);
    hi_log_chk_param_return(rsa_decrypt->pub_key.n_len > RSA_MAX_KEY_LEN);
    hi_log_chk_param_return(rsa_decrypt->pub_key.n_len < rsa_decrypt->pub_key.e_len);

    hi_dbg_print_u32(rsa_decrypt->scheme);
    hi_dbg_print_u32(in_len);
    hi_dbg_print_u32(rsa_decrypt->pub_key.n_len);

    chk_cipher_open_err_return();

    ret = memset_s(&key, sizeof(key), 0, sizeof(key));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    key.public  = HI_TRUE;
    key.klen    = rsa_decrypt->pub_key.n_len;
    key.n       = rsa_decrypt->pub_key.n;
    cipher_get_pub_exponent(&key.e, rsa_decrypt);

    crypt.in = in;
    crypt.in_len = in_len;
    crypt.out = out;
    crypt.out_buf_len = key.klen;
    crypt.out_len = out_len;
    ret = sys_rsa_decrypt(&key, rsa_decrypt->scheme, &crypt);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_rsa_decrypt, ret);
        crypto_zeroize(&key, sizeof(key));
        return ret;
    }

    crypto_zeroize(&key, sizeof(key));
    hi_unf_func_exit();
    return HI_SUCCESS;
}

static hi_s32 mpi_cipher_hash(const mpi_hash_pack *pack)
{
    hi_s32 ret;
    hi_handle hash_id;

    ret = sys_hash_start(&hash_id, pack->sha_type, HI_NULL, 0);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_hash_start, ret);
        return ret;
    }

    ret = sys_hash_update(hash_id, pack->in, pack->in_len, HASH_CHUNCK_SRC_USER);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_hash_update, ret);
        return ret;
    }

    ret = sys_hash_finish(hash_id, pack->hash, pack->hash_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_hash_finish, ret);
        return ret;
    }

    hi_dbg_print_u32(hash_len);
    return HI_SUCCESS;
}

static hi_void mpi_rsa_sign_set_key(sys_rsa_sign_verify *rsa_info, const hi_cipher_rsa_sign *rsa_sign)
{
    rsa_info->key.public  = HI_FALSE;
    rsa_info->key.klen    = rsa_sign->private_key.n_len;
    rsa_info->key.n       = rsa_sign->private_key.n;
    rsa_info->key.d       = rsa_sign->private_key.d;
    rsa_info->key.ca_type = rsa_sign->ca_type;
    rsa_info->key.p       = rsa_sign->private_key.p;
    rsa_info->key.q       = rsa_sign->private_key.q;
    rsa_info->key.dp      = rsa_sign->private_key.dp;
    rsa_info->key.dq      = rsa_sign->private_key.dq;
    rsa_info->key.qp      = rsa_sign->private_key.qp;
    rsa_info->scheme      = rsa_sign->scheme;
}

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_sign(const hi_cipher_rsa_sign *rsa_sign, const hi_u8 *in_data, hi_u32 in_data_len,
    const hi_u8 *hash_data, hi_u8 *out_sign, hi_u32 *out_sign_len)
{
    hi_s32 ret;
    hi_u8 hash[HASH_RESULT_MAX_SIZE] = {0};
    hi_cipher_hash_type sha_type = 0;
    sys_rsa_sign_verify rsa_info;

    hi_unf_func_enter();

    hi_log_chk_param_return(rsa_sign == HI_NULL);
    hi_log_chk_param_return(rsa_sign->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = mpi_chk_rsa_private_key(&rsa_sign->private_key);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_chk_rsa_private_key, ret);
        return ret;
    }

    hi_log_chk_param_return(out_sign == HI_NULL);
    hi_log_chk_param_return(out_sign_len == HI_NULL);
    hi_log_chk_param_return((in_data == HI_NULL) && (hash_data == HI_NULL));

    chk_cipher_open_err_return();

    ret = memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = cipher_get_rsa_sign_attr(rsa_sign->scheme, &rsa_info.hash_len, &sha_type);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cipher_get_rsa_sign_attr, ret);
        return ret;
    }

    /* hash value of context, if NULL, compute hash = Hash(in_data). */
    if (hash_data == HI_NULL) {
        mpi_hash_pack pack;

        pack.sha_type = sha_type;
        pack.in = in_data;
        pack.in_len = in_data_len;
        pack.hash = hash;
        pack.hash_buf_len = sizeof(hash);
        pack.hash_len = &rsa_info.hash_len;
        ret = mpi_cipher_hash(&pack);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mpi_cipher_hash, ret);
            return ret;
        }
        rsa_info.hash = pack.hash;
    } else {
        rsa_info.hash = (hi_u8 *)hash_data;
    }

    mpi_rsa_sign_set_key(&rsa_info, rsa_sign);

    ret = sys_rsa_sign_hash(&rsa_info, out_sign, out_sign_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_rsa_sign_hash, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

static hi_s32 mpi_cipher_rsa_chk_verify_param(const hi_cipher_rsa_verify *rsa_verify, const hi_u8 *in, hi_u32 in_len,
    const hi_u8 *hash, hi_u32 sign_len)
{
    hi_log_chk_param_return((in == HI_NULL) && (hash == HI_NULL));
    hi_log_chk_param_return(rsa_verify == HI_NULL);
    hi_log_chk_param_return(rsa_verify->pub_key.n == HI_NULL);
    hi_log_chk_param_return(rsa_verify->pub_key.e == HI_NULL);
    hi_log_chk_param_return(rsa_verify->pub_key.n_len < RSA_MIN_KEY_LEN);
    hi_log_chk_param_return(rsa_verify->pub_key.n_len > RSA_MAX_KEY_LEN);
    hi_log_chk_param_return(rsa_verify->pub_key.n_len < rsa_verify->pub_key.e_len);
    hi_log_chk_param_return(rsa_verify->pub_key.n_len != sign_len);

    hi_dbg_print_u32(rsa_verify->scheme);
    hi_dbg_print_u32(rsa_verify->pub_key.n_len);
    hi_dbg_print_u32(rsa_verify->scheme);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_cipher_rsa_verify(const hi_cipher_rsa_verify *rsa_verify, const hi_u8 *in_data,
    hi_u32 in_data_len, const hi_u8 *hash_data, const hi_u8 *in_sign, hi_u32 in_sign_len)
{
    hi_s32 ret;
    hi_u8 hash[HASH_RESULT_MAX_SIZE] = {0};
    hi_cipher_hash_type sha_type = 0;
    sys_rsa_sign_verify rsa_info;

    hi_unf_func_enter();
    hi_log_chk_param_return(in_sign == HI_NULL);
    ret = mpi_cipher_rsa_chk_verify_param(rsa_verify, in_data, in_data_len, hash_data, in_sign_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mpi_cipher_rsa_chk_verify_param, ret);
        return ret;
    }

    chk_cipher_open_err_return();
    ret = memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = cipher_get_rsa_sign_attr(rsa_verify->scheme, &rsa_info.hash_len, &sha_type);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cipher_get_rsa_sign_attr, ret);
        return ret;
    }

    /* hash value of context, if NULL, compute hash = Hash(in_data). */
    if (hash_data == HI_NULL) {
        mpi_hash_pack pack;

        pack.sha_type = sha_type;
        pack.in = in_data;
        pack.in_len = in_data_len;
        pack.hash = hash;
        pack.hash_buf_len = sizeof(hash);
        pack.hash_len = &rsa_info.hash_len;
        ret = mpi_cipher_hash(&pack);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mpi_cipher_hash, ret);
            return ret;
        }
        rsa_info.hash = pack.hash;
    } else {
        rsa_info.hash = (hi_u8 *)hash_data;
    }

    rsa_info.key.public = HI_TRUE;
    rsa_info.key.klen   = rsa_verify->pub_key.n_len;
    rsa_info.key.n      = rsa_verify->pub_key.n;
    rsa_info.scheme     = rsa_verify->scheme;
    cipher_get_pub_exponent(&rsa_info.key.e, rsa_verify);

    ret = sys_rsa_verify_hash(&rsa_info, in_sign, in_sign_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_rsa_verify_hash, ret);
        return ret;
    }

    hi_unf_func_exit();
    return HI_SUCCESS;
}

#endif
