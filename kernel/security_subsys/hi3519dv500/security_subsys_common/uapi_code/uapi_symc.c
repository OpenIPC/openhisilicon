/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_symc.h"

#include <securec.h>

#include "uapi_common.h"
#include "crypto_ioctl_cmd.h"
#include "crypto_osal_user_lib.h"

#define SYMC_COMPAT_ERRNO(err_code)     UAPI_COMPAT_ERRNO(ERROR_MODULE_SYMC, err_code)

#define CRYPTO_HASH_MAX_PROCESS_LEN         (4 * 1024)

#define crypto_symc_init_chk() do {                 \
    if (crypto_cipher_is_init() == 0) {                 \
        return SYMC_COMPAT_ERRNO(ERROR_NOT_INIT);   \
    }                                               \
} while (0)

td_s32 unify_uapi_cipher_symc_init(td_void)
{
    td_s32 ret = CRYPTO_SUCCESS;

    crypto_uapi_func_enter();

    ret = crypto_cipher_open();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_INIT, TD_NULL);
    if (ret != CRYPTO_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        crypto_cipher_close();
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_deinit(td_void)
{
    td_s32 ret = CRYPTO_SUCCESS;

    crypto_uapi_func_enter();

    if (crypto_cipher_is_init() == 0) {
        return CRYPTO_SUCCESS;
    }

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_DEINIT, TD_NULL);
    if (ret != CRYPTO_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
    }

    crypto_cipher_close();

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_create(td_handle *symc_handle, const crypto_symc_attr *symc_attr)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_create_t symc_create;

    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(symc_handle == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "symc_handle is NULL\n");
    crypto_chk_return(symc_attr == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "symc_attr is NULL\n");

    (td_void)memset_s(&symc_create, sizeof(symc_create), 0, sizeof(symc_create));
    (td_void)memcpy_s(&symc_create.symc_attr, sizeof(crypto_symc_attr), symc_attr, sizeof(crypto_symc_attr));

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_CREATE, &symc_create);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    *symc_handle = symc_create.symc_handle;

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_destroy(td_handle symc_handle)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_destroy_t symc_destroy;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    (td_void)memset_s(&symc_destroy, sizeof(symc_destroy), 0, sizeof(symc_destroy));

    symc_destroy.symc_handle = symc_handle;
    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_DESTROY, &symc_destroy);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_set_config(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_config_t symc_config;
    crypto_symc_config_aes_ccm_gcm *ccm_gcm_config = TD_NULL;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(symc_ctrl == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "symc_ctrl is NULL\n");

    (td_void)memset_s(&symc_config, sizeof(symc_config), 0, sizeof(symc_config));

    symc_config.symc_handle = symc_handle;
    symc_config.symc_alg = symc_ctrl->symc_alg;
    symc_config.work_mode = symc_ctrl->work_mode;
    symc_config.symc_key_length = symc_ctrl->symc_key_length;
    symc_config.symc_bit_width = symc_ctrl->symc_bit_width;
    symc_config.iv_change_flag = symc_ctrl->iv_change_flag;
    ret = memcpy_s(symc_config.iv, sizeof(symc_config.iv), symc_ctrl->iv, symc_ctrl->iv_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    symc_config.iv_length = symc_ctrl->iv_length;

    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM || symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        crypto_chk_return(symc_ctrl->param == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "param is NULL\n");
        ccm_gcm_config = (crypto_symc_config_aes_ccm_gcm *)symc_ctrl->param;
        symc_config.aad_len = ccm_gcm_config->aad_len;
        symc_config.data_len = ccm_gcm_config->data_len;
        symc_config.tag_len = ccm_gcm_config->tag_len;
        symc_config.aad_mem_handle = ccm_gcm_config->aad_buf.uapi_mem_handle;
        symc_config.aad_phys_addr = ccm_gcm_config->aad_buf.phys_addr;
        symc_config.aad.p = ccm_gcm_config->aad_buf.virt_addr;
    }

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_SET_CONFIG, &symc_config);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_get_config(td_handle symc_handle, crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_config_t symc_config;
    crypto_symc_config_aes_ccm_gcm *ccm_gcm_config = TD_NULL;
    crypto_uapi_func_enter();

    crypto_symc_init_chk();
    crypto_chk_return(symc_ctrl == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "symc_ctrl is NULL\n");
    (td_void)memset_s(&symc_config, sizeof(symc_config), 0, sizeof(symc_config));
    ccm_gcm_config = (crypto_symc_config_aes_ccm_gcm *)symc_ctrl->param;
    if (ccm_gcm_config != TD_NULL) {
        symc_config.aad.p = ccm_gcm_config->aad_buf.virt_addr;
    }
    symc_config.symc_handle = symc_handle;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_GET_CONFIG, &symc_config);
    if (ret != TD_SUCCESS) {
        printf("%s\n", __func__);
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    symc_ctrl->symc_alg = symc_config.symc_alg;
    symc_ctrl->work_mode = symc_config.work_mode;
    symc_ctrl->symc_key_length = symc_config.symc_key_length;
    symc_ctrl->symc_bit_width = symc_config.symc_bit_width;
    symc_ctrl->iv_change_flag = symc_config.iv_change_flag;

    symc_ctrl->iv_length = symc_config.iv_length;
    ret = memcpy_s(symc_ctrl->iv, sizeof(symc_ctrl->iv), symc_config.iv, sizeof(symc_config.iv));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM || symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        crypto_chk_return(ccm_gcm_config == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "param is NULL\n");
        ccm_gcm_config->aad_len = symc_config.aad_len;
        ccm_gcm_config->data_len = symc_config.data_len;
        ccm_gcm_config->tag_len = symc_config.tag_len;
        ccm_gcm_config->aad_buf.uapi_mem_handle = symc_config.aad_mem_handle;
        ccm_gcm_config->aad_buf.phys_addr = symc_config.aad_phys_addr;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_attach(td_handle symc_handle, td_handle keyslot_handle)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_attach_t symc_attach;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    (td_void)memset_s(&symc_attach, sizeof(symc_attach), 0, sizeof(symc_attach));
    symc_attach.symc_handle = symc_handle;
    symc_attach.keyslot_handle = keyslot_handle;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_ATTACH, &symc_attach);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_encrypt(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_crypto_t symc_crypto;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(src_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "src_buf is NULL\n");
    crypto_chk_return(dst_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "dst_buf is NULL\n");

    (td_void)memset_s(&symc_crypto, sizeof(symc_crypto), 0, sizeof(symc_crypto));
    symc_crypto.symc_handle = symc_handle;
    symc_crypto.src_mem_handle = src_buf->uapi_mem_handle;
    symc_crypto.src_phys_addr = src_buf->phys_addr;
    symc_crypto.src_buf_sec = src_buf->buf_sec;
    symc_crypto.src_addr_offset = src_buf->addr_offset;
    symc_crypto.dst_mem_handle = dst_buf->uapi_mem_handle;
    symc_crypto.dst_phys_addr = dst_buf->phys_addr;
    symc_crypto.dst_buf_sec = dst_buf->buf_sec;
    symc_crypto.dst_addr_offset = dst_buf->addr_offset;
    symc_crypto.length = length;
    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_ENCRYPT, &symc_crypto);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_decrypt(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_crypto_t symc_crypto;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(src_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "src_buf is NULL\n");
    crypto_chk_return(dst_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "dst_buf is NULL\n");

    (td_void)memset_s(&symc_crypto, sizeof(symc_crypto), 0, sizeof(symc_crypto));
    symc_crypto.symc_handle = symc_handle;
    symc_crypto.src_mem_handle = src_buf->uapi_mem_handle;
    symc_crypto.src_phys_addr = src_buf->phys_addr;
    symc_crypto.src_buf_sec = src_buf->buf_sec;
    symc_crypto.src_addr_offset = src_buf->addr_offset;
    symc_crypto.dst_mem_handle = dst_buf->uapi_mem_handle;
    symc_crypto.dst_phys_addr = dst_buf->phys_addr;
    symc_crypto.dst_buf_sec = dst_buf->buf_sec;
    symc_crypto.dst_addr_offset = dst_buf->addr_offset;
    symc_crypto.length = length;
    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_DECRYPT, &symc_crypto);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_get_tag(td_handle symc_handle, td_u8 *tag, td_u32 tag_length)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_get_tag_t symc_get_tag;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(tag == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "tag is NULL\n");

    (td_void)memset_s(&symc_get_tag, sizeof(symc_get_tag), 0, sizeof(symc_get_tag));
    symc_get_tag.symc_handle = symc_handle;
    symc_get_tag.tag_length = tag_length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_GET_TAG, &symc_get_tag);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    ret = memcpy_s(tag, tag_length, symc_get_tag.tag, symc_get_tag.tag_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_mac_start(td_handle *symc_handle, const crypto_symc_mac_attr *mac_attr)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_mac_start_t mac_start;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(symc_handle == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "symc_handle is NULL\n");
    crypto_chk_return(mac_attr == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "mac_attr is NULL\n");

    (td_void)memset_s(&mac_start, sizeof(mac_start), 0, sizeof(mac_start));
    (td_void)memcpy_s(&mac_start.mac_attr, sizeof(crypto_symc_mac_attr), mac_attr, sizeof(crypto_symc_mac_attr));
    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_MAC_START, &mac_start);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    *symc_handle = mac_start.symc_handle;
    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_mac_update(td_handle symc_handle, const crypto_buf_attr *src_buf, td_u32 length)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_mac_update_t mac_update;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(src_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "src_buf is NULL\n");

    (td_void)memset_s(&mac_update, sizeof(mac_update), 0, sizeof(mac_update));
    mac_update.symc_handle = symc_handle;
    mac_update.src_buf.p = src_buf->virt_addr;
    mac_update.length = length;
    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_MAC_UPDATE, &mac_update);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_mac_finish(td_handle symc_handle, td_u8 *mac, td_u32 *mac_length)
{
    td_s32 ret = CRYPTO_SUCCESS;
    symc_mac_finish_t mac_finish;
    crypto_uapi_func_enter();
    crypto_symc_init_chk();

    crypto_chk_return(mac == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "mac is NULL\n");
    crypto_chk_return(mac_length == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "mac_length is NULL\n");

    (td_void)memset_s(&mac_finish, sizeof(mac_finish), 0, sizeof(mac_finish));
    mac_finish.symc_handle = symc_handle;
    ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_MAC_FINISH, &mac_finish);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        return ret;
    }
    crypto_chk_return(*mac_length < mac_finish.mac_length, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "mac_length is not enough\n");
    ret = memcpy_s(mac, *mac_length, mac_finish.mac, mac_finish.mac_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    *mac_length = mac_finish.mac_length;
    crypto_uapi_func_exit();
    return ret;
}

static td_s32 priv_uapi_symc_config_ctrl(const crypto_symc_ctrl_t *symc_ctrl, crypto_symc_multi_t *symc_multi)
{
    td_s32 ret;

    symc_multi->symc_alg = symc_ctrl->symc_alg;
    symc_multi->work_mode = symc_ctrl->work_mode;
    symc_multi->symc_key_length = symc_ctrl->symc_key_length;
    symc_multi->key_parity = symc_ctrl->key_parity;
    symc_multi->symc_bit_width = symc_ctrl->symc_bit_width;
    symc_multi->iv_change_flag = symc_ctrl->iv_change_flag;
    symc_multi->iv_length = symc_ctrl->iv_length;
    ret = memcpy_s(symc_multi->iv, sizeof(symc_multi->iv), symc_ctrl->iv, symc_ctrl->iv_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return ret;
}

typedef struct {
    td_handle symc_handle;
    td_u32 pack_num;
    td_u32 crypto_type;
} priv_multi_param;

static td_s32 priv_uapi_symc_crypto_multi(const priv_multi_param *multi_param, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack)
{
    td_u32 i;
    td_s32 ret = CRYPTO_SUCCESS;
    crypto_symc_multi_t symc_multi;
    crypto_mem_pack *buffer = TD_NULL;
    crypto_mem_pack *src_mem_handle = TD_NULL;
    crypto_mem_pack *dst_mem_handle = TD_NULL;
    td_u32 pack_size = sizeof(crypto_mem_pack) * multi_param->pack_num;
    crypto_symc_init_chk();

    crypto_chk_return(symc_ctrl == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "symc_ctrl is NULL\n");
    crypto_chk_return(src_buf_pack == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "src_buf_pack is NULL\n");
    crypto_chk_return(dst_buf_pack == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "dst_buf_pack is NULL\n");
    crypto_chk_return(multi_param->pack_num > CRYPTO_SYMC_MULTI_PACK_MAX_SIZE,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "pack_num is too large\n");

    (td_void)memset_s(&symc_multi, sizeof(crypto_symc_multi_t), 0, sizeof(crypto_symc_multi_t));
    symc_multi.symc_handle = multi_param->symc_handle;

    ret = priv_uapi_symc_config_ctrl(symc_ctrl, &symc_multi);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_uapi_symc_config_ctrl failed\n");

    buffer = crypto_malloc(pack_size * 2);  /* 2: for Both src and dst. */
    crypto_chk_return(buffer == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    src_mem_handle = buffer;
    dst_mem_handle = buffer + multi_param->pack_num;
    for (i = 0; i < multi_param->pack_num; i++) {
        src_mem_handle[i].uapi_mem_handle = src_buf_pack[i].buf_attr.uapi_mem_handle;
        src_mem_handle[i].length = src_buf_pack[i].length;
        src_mem_handle[i].phys_addr = src_buf_pack[i].buf_attr.phys_addr;
        src_mem_handle[i].buf_sec = src_buf_pack[i].buf_attr.buf_sec;
        dst_mem_handle[i].uapi_mem_handle = dst_buf_pack[i].buf_attr.uapi_mem_handle;
        dst_mem_handle[i].length = dst_buf_pack[i].length;
        dst_mem_handle[i].phys_addr = dst_buf_pack[i].buf_attr.phys_addr;
        dst_mem_handle[i].buf_sec = dst_buf_pack[i].buf_attr.buf_sec;
    }
    symc_multi.src_pack_addr.p = (td_void *)src_mem_handle;
    symc_multi.dst_pack_addr.p = (td_void *)dst_mem_handle;
    symc_multi.pack_num = multi_param->pack_num;
    switch (multi_param->crypto_type) {
        case CRYPTO_TYPE_ENCRYPT:
            ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_ENCRYPT_MULTI, &symc_multi);
            break;
        case CRYPTO_TYPE_DECRYPT:
            ret = crypto_cipher_ioctl(CRYPTO_CMD_SYMC_DECRYPT_MULTI, &symc_multi);
            break;
        default:
            printf("Invalid Crypto Type!\n");
            ret = SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    (td_void)memset_s(buffer, pack_size * 2, 0, pack_size * 2); /* 2: for Both src and dst. */
    crypto_free(buffer);
    return ret;
}

td_s32 unify_uapi_cipher_symc_encrypt_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_s32 ret = CRYPTO_SUCCESS;
    priv_multi_param multi_param = {
        .symc_handle = symc_handle,
        .pack_num = pack_num,
        .crypto_type = CRYPTO_TYPE_ENCRYPT,
    };
    crypto_uapi_func_enter();
    
    ret = priv_uapi_symc_crypto_multi(&multi_param, symc_ctrl, src_buf_pack, dst_buf_pack);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_uapi_symc_crypto_multi for Encrypt failed\n");
    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_symc_decrypt_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_s32 ret = CRYPTO_SUCCESS;
    priv_multi_param multi_param = {
        .symc_handle = symc_handle,
        .pack_num = pack_num,
        .crypto_type = CRYPTO_TYPE_DECRYPT,
    };
    crypto_uapi_func_enter();

    ret = priv_uapi_symc_crypto_multi(&multi_param, symc_ctrl, src_buf_pack, dst_buf_pack);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_uapi_symc_crypto_multi for Decrypt failed\n");
    crypto_uapi_func_exit();
    return ret;
}
