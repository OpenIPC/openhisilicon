/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_symc.h"
#include "drv_inner.h"
#include "hal_symc.h"
#include "crypto_drv_common.h"

#define SYMC_COMPAT_ERRNO(err_code)     DRV_COMPAT_ERRNO(ERROR_MODULE_SYMC, err_code)
#define symc_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

#define CRYPTO_DRV_CCM_DMA_SIZE     (CRYPTO_AES_BLOCK_SIZE_IN_BYTES * 3)    /* For N + ccm_header + ccm_padding. */
#define CRYPTO_DRV_GCM_DMA_SIZE     (CRYPTO_AES_BLOCK_SIZE_IN_BYTES * 2)    /* For aad_padding + clen. */
#define CRYPTO_DRV_DMA_SIZE         (4 * 1024)
#define CRYPTO_DRV_DMA_TOTAL_SIZE   (CRYPTO_DRV_DMA_SIZE * CRYPTO_SYMC_HARD_CHANNEL_MAX)

static td_u8 *g_drv_dma_buf = TD_NULL;

static drv_symc_context_t g_drv_symc_ctx_list[CRYPTO_SYMC_HARD_CHANNEL_MAX];

td_s32 inner_symc_drv_handle_chk(td_handle symc_handle)
{
    td_u32 chn_num = (td_u32)symc_handle;
    if (chn_num >= CRYPTO_SYMC_HARD_CHANNEL_MAX) {
        return SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE);
    }
    if (((1 << chn_num) & CRYPTO_SYMC_HARD_CHANNEL_MASK) == 0) {
        return SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE);
    }
    return TD_SUCCESS;
}

drv_symc_context_t *inner_get_symc_ctx(td_handle symc_handle)
{
    td_s32 ret;
    td_u32 chn_num = (td_u32)symc_handle;
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return TD_NULL;
    }
    return &g_drv_symc_ctx_list[chn_num];
}

td_s32 drv_cipher_symc_init(td_void)
{
    td_s32 ret;
    crypto_drv_func_enter();
    ret = hal_cipher_symc_init();
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_init failed, ret is 0x%x\n", ret);
        return ret;
    }
    g_drv_dma_buf = crypto_malloc_coherent(CRYPTO_DRV_DMA_TOTAL_SIZE, "cipher_symc_drv");
    if (g_drv_dma_buf == TD_NULL) {
        hal_cipher_symc_deinit();
        return SYMC_COMPAT_ERRNO(ERROR_MALLOC);
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_deinit(td_void)
{
    crypto_drv_func_enter();
    if (g_drv_dma_buf != TD_NULL) {
        (td_void)memset_s(g_drv_dma_buf, CRYPTO_DRV_DMA_TOTAL_SIZE, 0, CRYPTO_DRV_DMA_TOTAL_SIZE);
        crypto_free_coherent(g_drv_dma_buf);
        g_drv_dma_buf = TD_NULL;
    }
    hal_cipher_symc_deinit();
    crypto_drv_func_exit();
    return TD_SUCCESS;
}

td_s32 drv_cipher_symc_create(td_handle *symc_handle, const crypto_symc_attr *symc_attr)
{
    td_u32 i;
    td_s32 ret = CRYPTO_SUCCESS;
    drv_symc_context_t *symc_ctx = TD_NULL;
    crypto_drv_func_enter();

    symc_null_ptr_chk(symc_handle);
    symc_null_ptr_chk(symc_attr);

    for (i = 1; i < CRYPTO_SYMC_HARD_CHANNEL_MAX; i++) {
        symc_ctx = &g_drv_symc_ctx_list[i];
        if (symc_ctx->is_open == TD_TRUE) {
            continue;
        }
        ret = hal_cipher_symc_lock_chn(i);
        if (ret == CRYPTO_SUCCESS) {
            break;
        }
    }
    if (i >= CRYPTO_SYMC_HARD_CHANNEL_MAX) {
        crypto_log_err("All SYMC Channels are busy!\n");
        return SYMC_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }
    symc_ctx = &g_drv_symc_ctx_list[i];
    (td_void)memset_s(symc_ctx, sizeof(drv_symc_context_t), 0, sizeof(drv_symc_context_t));

    /*
     * For CBC_MAC, store data copy;
     * For CCM, store N + ccm_header + ccm_padding
     * For GCM, store aad_padding + clen
     */
    symc_ctx->dma_addr = g_drv_dma_buf + i * CRYPTO_DRV_DMA_SIZE;
    symc_ctx->dma_size = CRYPTO_DRV_DMA_SIZE;
    symc_ctx->chn_num = i;
    symc_ctx->is_open = TD_TRUE;
    *symc_handle = i;

    crypto_drv_func_exit();
    return CRYPTO_SUCCESS;
}

td_s32 drv_cipher_symc_destroy(td_handle symc_handle)
{
    td_s32 ret;
    drv_symc_context_t *symc_ctx = TD_NULL;
    td_u32 chn_num = (td_u32)symc_handle;
    crypto_drv_func_enter();

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        crypto_log_err("Invalid Handle\n");
        return ret;
    }
    symc_ctx = &g_drv_symc_ctx_list[chn_num];

#if defined(CONFIG_CRYPTO_SOFT_CENC_SUPPORT)
    cenc_ddr_release();
    cenc_ddr_unmap_input(chn_num);
#endif
    (td_void)memset_s(symc_ctx, sizeof(drv_symc_context_t), 0, sizeof(drv_symc_context_t));
    ret = hal_cipher_symc_unlock_chn(chn_num);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_unlock_chn failed, ret is 0x%x\n", ret);
    }

    crypto_drv_func_exit();
    return TD_SUCCESS;
}

static td_s32 inner_format_ccm_iv(const td_u8 *iv, td_u32 iv_length, td_u8 *ccm_iv, td_u32 *ccm_iv_length)
{
    td_s32 ret;
    ccm_iv[0] = CRYPTO_AES_CCM_NQ_LEN - iv_length;
    ret = memcpy_s(&ccm_iv[1], *ccm_iv_length - 1, iv, iv_length);
    if (ret != EOK) {
        crypto_log_err("memcpy_s failed\n");
        return TD_FAILURE;
    }
    *ccm_iv_length = iv_length + 1;
    return TD_SUCCESS;
}

static td_void inner_format_ccm_header(td_u32 aad_len, td_u8 *ccm_header, td_u32 *ccm_header_len)
{
    td_u32 idx = 0;
    ccm_header[idx++] = (td_u8)(aad_len >> 8);  /* 8: bits 15-8 */
    ccm_header[idx++] = (td_u8)(aad_len);
    *ccm_header_len = idx;
}

static td_s32 priv_symc_cfg_ex_param_check(const crypto_symc_ctrl_t *symc_ctrl)
{
    crypto_symc_work_mode mode = symc_ctrl->work_mode;
    crypto_symc_config_aes_ccm_gcm *ccm_gcm_param = (crypto_symc_config_aes_ccm_gcm *)symc_ctrl->param;
    td_u32 iv_length = symc_ctrl->iv_length;
    td_u32 tag_len;
    td_u32 data_len;
    td_u32 diff = 0;

    crypto_chk_return(ccm_gcm_param == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "param is NULL\n");
    tag_len = ccm_gcm_param->tag_len;
    data_len = ccm_gcm_param->data_len;
    /* Check for CCM. */
    if (mode == CRYPTO_SYMC_WORK_MODE_CCM && symc_ctrl->iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_START) {
        /* IV lenght for CCM, which should be 7~13. */
        crypto_chk_return(iv_length > 13 || iv_length < 7,      // 7: min iv_len for ccm, 13: max iv_len for ccm.
            SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "iv_len must be 7~13 for CCM\n");
        /* Tag length for CCM should be 4,6,8,10,12,14,16. */
        crypto_chk_return(
            tag_len % 2 != 0 || tag_len < 4 || tag_len > 16,  // 4: min tag_len, 16: max tag_len, 2: judge if even.
            SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "tag_len must be 4,6,8,10,12,14,16 for CCM\n");
        diff = CRYPTO_IV_LEN_IN_BYTES - iv_length - 1;
        if (diff == 2) {    // 2: 2 * 8 bits to represent data_len.
            crypto_chk_return(data_len > 0xFFFF, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), // 0xFFFF: 2^16 - 1
                "data_len is too long\n");
        }
        if (diff == 3) {    // 3: 3 * 8 bits to represent data_len.
            crypto_chk_return(data_len > 0xFFFFFF, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), // 0xFFFFFF: 2^24 - 1
                "data_len is too long\n");
        }
    }

    /* Check for GCM. */
    if (mode == CRYPTO_SYMC_WORK_MODE_GCM && symc_ctrl->iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_START) {
        /* IV lenght for GCM, which should be 1~16. */
        crypto_chk_return(iv_length == 0 || iv_length > CRYPTO_IV_LEN_IN_BYTES, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "iv_len must be 1~16 for GCM.\n");
        /* Tag length for GCM should be 1~16.. */
        crypto_chk_return(tag_len == 0 || tag_len > CRYPTO_AES_MAX_TAG_SIZE, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "tag_len must be 1~16 for GCM.\n");
    }
    return TD_SUCCESS;
}

td_s32 inner_symc_cfg_param_check(const crypto_symc_ctrl_t *symc_ctrl)
{
    td_bool is_support = TD_FALSE;
    crypto_symc_alg symc_alg = symc_ctrl->symc_alg;
    crypto_symc_work_mode mode = symc_ctrl->work_mode;
    crypto_symc_key_length key_length = symc_ctrl->symc_key_length;
    crypto_symc_bit_width bit_width = symc_ctrl->symc_bit_width;
    crypto_symc_iv_change_type iv_change_flag = symc_ctrl->iv_change_flag;
    td_u32 iv_length = symc_ctrl->iv_length;

    /* Check Common Params. */
    crypto_chk_return(symc_alg != CRYPTO_SYMC_ALG_AES && symc_alg != CRYPTO_SYMC_ALG_SM4,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "symc_alg is invalid\n");
    crypto_chk_return(mode > CRYPTO_SYMC_WORK_MODE_GCM, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "mode is invalid\n");
    crypto_chk_return(key_length < CRYPTO_SYMC_KEY_128BIT || key_length > CRYPTO_SYMC_KEY_256BIT,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "key_length is invalid\n");
    crypto_chk_return(bit_width >= CRYPTO_SYMC_BIT_WIDTH_MAX,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "bit_width is invalid\n");
    crypto_chk_return(iv_change_flag >= CRYPTO_SYMC_IV_CHANGE_MAX, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "iv_change_flag is invalid\n");

    /* Check Mode. && Check key_length. */
    if (symc_alg == CRYPTO_SYMC_ALG_SM4) {
        /* SM4 only support ECB/CBC/CTR. */
        crypto_chk_return(mode != CRYPTO_SYMC_WORK_MODE_ECB && mode != CRYPTO_SYMC_WORK_MODE_CBC &&
            mode != CRYPTO_SYMC_WORK_MODE_CTR, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT), "sm4 unsupport this mode\n");
        /* SM4's keylength must be 128. */
        crypto_chk_return(key_length != CRYPTO_SYMC_KEY_128BIT, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT),
            "sm4's key_len only support 128-bit\n");
    }

    /* Check bit_width. */
    if (mode == CRYPTO_SYMC_WORK_MODE_CFB) {
        crypto_chk_return(bit_width == CRYPTO_SYMC_BIT_WIDTH_64BIT, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT),
            "CFB's bit-width don't support 64-bit\n");
    } else {
        crypto_chk_return(bit_width != CRYPTO_SYMC_BIT_WIDTH_128BIT, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT),
            "bit-width only support 128-bit\n");
    }

    /* Check iv_change_flag. */

    /* Check iv_length. */
    if (mode != CRYPTO_SYMC_WORK_MODE_CCM && mode != CRYPTO_SYMC_WORK_MODE_GCM && mode != CRYPTO_SYMC_WORK_MODE_ECB) {
        crypto_chk_return(iv_length != CRYPTO_IV_LEN_IN_BYTES, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "iv_length is invalid\n");
    }

    if (mode == CRYPTO_SYMC_WORK_MODE_CCM || mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        return priv_symc_cfg_ex_param_check(symc_ctrl);
    }

    is_support = crypto_symc_support(symc_ctrl->symc_alg, symc_ctrl->work_mode,
        symc_ctrl->symc_key_length, symc_ctrl->symc_bit_width);
    crypto_chk_return(is_support == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT), "alg is unsupport\n");
    if (symc_ctrl->symc_alg == CRYPTO_SYMC_ALG_SM4) {
        is_support = crypto_sm_support(CRYPTO_SM_ALG_SM4);
    }
    crypto_chk_return(is_support == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT), "alg is unsupport\n");

    return TD_SUCCESS;
}

static td_s32 inner_drv_set_config_ex(drv_symc_context_t *drv_symc_ctx, const crypto_symc_ctrl_t *symc_ctrl,
    hal_symc_config_t *hal_symc_config)
{
    td_s32 ret;
    crypto_symc_config_aes_ccm_gcm *ccm_gcm_config = TD_NULL;

    if (symc_ctrl->iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_UPDATE ||
        symc_ctrl->iv_change_flag == CRYPTO_SYMC_GCM_IV_CHANGE_UPDATE) {
        drv_symc_ctx->iv_change_flag = symc_ctrl->iv_change_flag;
        drv_symc_ctx->is_config = TD_TRUE;
        return TD_SUCCESS;
    }

    /* CCM. */
    ccm_gcm_config = (crypto_symc_config_aes_ccm_gcm *)symc_ctrl->param;
    crypto_chk_return(ccm_gcm_config->aad_len > CRYPTO_SYMC_AAD_MAX_SIZE, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "aad_len is too long\n");
    drv_symc_ctx->aad_len = ccm_gcm_config->aad_len;
    drv_symc_ctx->data_len = ccm_gcm_config->data_len;
    drv_symc_ctx->processed_len = 0;
    if (ccm_gcm_config->aad_len != 0) {
        symc_null_ptr_chk(ccm_gcm_config->aad_buf.virt_addr);
        drv_symc_ctx->aad_phys = ccm_gcm_config->aad_buf.phys_addr;
        drv_symc_ctx->aad_virt = ccm_gcm_config->aad_buf.virt_addr;
    }

    ret = memcpy_s(&drv_symc_ctx->param.ccm_config, sizeof(crypto_symc_config_aes_ccm_gcm),
        symc_ctrl->param, sizeof(crypto_symc_config_aes_ccm_gcm));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed, ret is 0x%x\n", ret);

    hal_symc_config->gcm_iv_ptr = symc_ctrl->iv;
    hal_symc_config->gcm_iv_len = symc_ctrl->iv_length;

    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM
        && drv_symc_ctx->iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_START) {
        drv_symc_ctx->iv_length = sizeof(drv_symc_ctx->iv);
        ret = inner_format_ccm_iv(symc_ctrl->iv, symc_ctrl->iv_length, drv_symc_ctx->iv, &(drv_symc_ctx->iv_length));
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_format_ccm_iv failed, ret is 0x%x\n", ret);
    }
    return TD_SUCCESS;
}

td_s32 drv_cipher_symc_set_config(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret;
    td_u32 chn_num = symc_handle;
    hal_symc_config_t hal_symc_config = {0};
    drv_symc_context_t *drv_symc_ctx = TD_NULL;
    crypto_drv_func_enter();
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    symc_null_ptr_chk(symc_ctrl);
    ret = inner_symc_cfg_param_check(symc_ctrl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_symc_cfg_param_check failed, ret is 0x%x\n", ret);

    drv_symc_ctx = &g_drv_symc_ctx_list[chn_num];
    crypto_chk_return(drv_symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    drv_symc_ctx->work_mode = symc_ctrl->work_mode;
    drv_symc_ctx->iv_change_flag = symc_ctrl->iv_change_flag;
    (td_void)memset_s(drv_symc_ctx->iv, sizeof(drv_symc_ctx->iv), 0, sizeof(drv_symc_ctx->iv));
    ret = memcpy_s(drv_symc_ctx->iv, sizeof(drv_symc_ctx->iv), symc_ctrl->iv, symc_ctrl->iv_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM || symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        ret = inner_drv_set_config_ex(drv_symc_ctx, symc_ctrl, &hal_symc_config);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_set_config_ex failed, ret is 0x%x\n", ret);
    }

    ret = hal_cipher_symc_set_iv(chn_num, drv_symc_ctx->iv, sizeof(drv_symc_ctx->iv));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed, ret is 0x%x\n", ret);

    hal_symc_config.symc_alg = symc_ctrl->symc_alg;
    hal_symc_config.work_mode = symc_ctrl->work_mode;
    hal_symc_config.symc_bit_width = symc_ctrl->symc_bit_width;
    hal_symc_config.symc_key_length = symc_ctrl->symc_key_length;
    hal_symc_config.iv_change_flag = symc_ctrl->iv_change_flag;
    ret = hal_cipher_symc_config(chn_num, &hal_symc_config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_config failed, ret is 0x%x\n", ret);

#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    drv_symc_ctx->ctr_offset = 0;
#endif
    drv_symc_ctx->is_config = TD_TRUE;
    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_get_module_info(crypto_symc_module_info *module_info)
{
    return hal_cipher_symc_get_module_info(module_info);
}

td_s32 drv_cipher_symc_get_proc_info(td_handle symc_handle, crypto_symc_proc_info *proc_symc_info)
{
    td_s32 ret;
    td_u32 chn_num;
    drv_symc_context_t *drv_symc_ctx = TD_NULL;

    crypto_drv_func_enter();
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    symc_null_ptr_chk(proc_symc_info);

    chn_num = symc_handle;
    drv_symc_ctx = &g_drv_symc_ctx_list[chn_num];
    proc_symc_info->chn_id = chn_num;
    proc_symc_info->open = drv_symc_ctx->is_open;

    if (drv_symc_ctx->is_open == TD_FALSE) {
        return SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED);
    }

    if (drv_symc_ctx->is_config == TD_FALSE) {
        return SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG);
    }

    ret = hal_cipher_symc_get_proc_info(chn_num, proc_symc_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_proc_info, ret is 0x%x\n", ret);

    ret = hal_cipher_symc_get_iv(chn_num, proc_symc_info->iv, sizeof(proc_symc_info->iv));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_iv failed, ret is 0x%x\n", ret);

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_get_config(td_handle symc_handle, crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret;
    td_u32 chn_num;
    hal_symc_config_t hal_symc_config = {0};
    drv_symc_context_t *drv_symc_ctx = TD_NULL;

    crypto_drv_func_enter();
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    symc_null_ptr_chk(symc_ctrl);

    chn_num = symc_handle;
    drv_symc_ctx = &g_drv_symc_ctx_list[chn_num];
    crypto_chk_return(drv_symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(drv_symc_ctx->is_config == TD_FALSE,
        SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG), "call set_config first\n");

    ret = hal_cipher_symc_get_config(chn_num, &hal_symc_config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_unlock_chn failed, ret is 0x%x\n", ret);

    symc_ctrl->symc_alg = hal_symc_config.symc_alg;
    symc_ctrl->work_mode = hal_symc_config.work_mode;
    symc_ctrl->symc_bit_width = hal_symc_config.symc_bit_width;
    symc_ctrl->symc_key_length = hal_symc_config.symc_key_length;
    symc_ctrl->iv_length = CRYPTO_IV_LEN_IN_BYTES;
    symc_ctrl->iv_change_flag = hal_symc_config.iv_change_flag;

    ret = memcpy_s(symc_ctrl->iv, symc_ctrl->iv_length, drv_symc_ctx->iv, sizeof(drv_symc_ctx->iv));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed, ret is 0x%x\n", ret);

    /* CCM. */
    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM || symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        crypto_chk_return(symc_ctrl->param == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "Param is NULL!\n");
        ret = memcpy_s(symc_ctrl->param, sizeof(crypto_symc_config_aes_ccm_gcm),
            &drv_symc_ctx->param.ccm_config, sizeof(crypto_symc_config_aes_ccm_gcm));
        crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed, ret is 0x%x\n", ret);
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_attach(td_handle symc_handle, td_handle keyslot_handle)
{
    td_s32 ret;
    td_u32 symc_chn_num = symc_handle;
    td_u32 keyslot_chn_num = keyslot_handle;
    drv_symc_context_t *drv_symc_ctx = TD_NULL;

    crypto_drv_func_enter();
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    drv_symc_ctx = &g_drv_symc_ctx_list[symc_chn_num];
    crypto_chk_return(drv_symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(drv_symc_ctx->is_create_keyslot == TD_TRUE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED),
        "has been create\n");

    ret = hal_cipher_symc_attach(symc_chn_num, keyslot_chn_num);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_attach failed, ret is 0x%x\n", ret);
        return ret;
    }

    drv_symc_ctx->keyslot_handle = keyslot_handle;
    drv_symc_ctx->is_attached = TD_TRUE;

    crypto_drv_func_exit();
    return ret;
}

static td_s32 inner_drv_cipher_symc_normal_process(td_u32 chn_num, td_phys_addr_t src_phys_addr,
    td_phys_addr_t dst_phys_addr, td_u32 length)
{
    td_s32 ret;
    in_node_type_e node_type = IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;

#if defined(CRYPTO_NORMAL_TRACE_ENABLE)
    crypto_dump_phys_addr("normal_process_in_buf", src_phys_addr, length);
#endif
    ret = hal_cipher_symc_add_in_node(chn_num, src_phys_addr, length, node_type);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_add_in_node failed, ret is 0x%x\n", ret);
        return ret;
    }

    ret = hal_cipher_symc_add_out_node(chn_num, dst_phys_addr, length);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_add_out_node failed, ret is 0x%x\n", ret);
        return ret;
    }

    return ret;
}

static td_s32 inner_symc_ccm_process_N(td_u8 *N, td_u32 N_len, const drv_symc_context_t *drv_symc_ctx, td_u32 length)
{
    td_s32 ret;
    td_u32 ccm_n_node_type = 0;
    const crypto_symc_config_aes_ccm_gcm *ccm_config = &drv_symc_ctx->param.ccm_config;
    td_u32 data_len = ccm_config->data_len;
    td_u32 chn_num = drv_symc_ctx->chn_num;
    td_u32 idx = 0;
    td_u32 q;

    /* CCM ADD N. */
    (td_void)memset_s(N, N_len, 0, N_len);
    /* First byte of N. */
    N[idx] = (ccm_config->aad_len > 0 ? 1 : 0) << CRYPTO_BIT_6;
    N[idx] |= ((ccm_config->tag_len - 2) / 2) << CRYPTO_BIT_3;  /* 2: ccm require */
    N[idx] |= (CRYPTO_AES_BLOCK_SIZE_IN_BYTES - 1 - drv_symc_ctx->iv_length);
    idx++;
    /* copy ccm_iv[1..] to N[1..] */
    ret = memcpy_s(&N[idx], N_len - idx, &(drv_symc_ctx->iv[1]), drv_symc_ctx->iv_length - 1);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    idx += drv_symc_ctx->iv_length - 1;

    q = N_len - idx;
    if (q >= CRYPTO_SYMC_CCM_Q_LEN_4B) {
        idx = CRYPTO_AES_BLOCK_SIZE_IN_BYTES - CRYPTO_SYMC_CCM_Q_LEN_4B;
        N[idx++] = (td_u8)(data_len >> 24);     /* 24: bits 31-24 */
        N[idx++] = (td_u8)(data_len >> 16);     /* 16: bits 23-16 */
        N[idx++] = (td_u8)(data_len >> 8);      /* 8: bits 15-8 */
        N[idx++] = (td_u8)(data_len);
    } else if (q == CRYPTO_SYMC_CCM_Q_LEN_3B) {
        N[idx++] = (td_u8)(data_len >> 16);     /* 16: bits 23-16 */
        N[idx++] = (td_u8)(data_len >> 8);      /* 8: bits 15-8 */
        N[idx++] = (td_u8)(data_len);
    } else if (q == CRYPTO_SYMC_CCM_Q_LEN_2B) {
        N[idx++] = (td_u8)(data_len >> 8);      /* 8: bits 15-8 */
        N[idx++] = (td_u8)(data_len);
    } else {
        crypto_log_err("Invalid iv_len\n");
        return SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    ccm_n_node_type = IN_NODE_TYPE_CCM_N | IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;
    if ((length == 0) && (ccm_config->aad_len == 0)) {
        ccm_n_node_type |= IN_NODE_TYPE_CCM_LAST;
    }

#ifdef CRYPTO_CCM_TRACE_ENABLE
    crypto_dump_data("N", N, N_len);
#endif
    ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(N), N_len, ccm_n_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed\n");

    return ret;
}

#define AAD_BUF_SIZE            (4096)
static td_s32 inner_symc_ccm_process_aad(const drv_symc_context_t *drv_symc_ctx, td_u32 length)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 chn_num = drv_symc_ctx->chn_num;
    td_u32 ccm_header_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    td_u32 aad_len = drv_symc_ctx->aad_len;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_length = AAD_BUF_SIZE;
    in_node_type_e node_type = IN_NODE_TYPE_CCM_AAD | IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;
    td_u32 left = 0;
    td_u32 processed_length = 0;
    td_u32 processing_length = 0;

    if (aad_len == 0) {
        return TD_SUCCESS;
    }

    buffer = crypto_malloc_coherent(buffer_length, "cipher_aad_body");
    crypto_chk_return(buffer == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc_coherent failed\n");

    (td_void)memset_s(buffer, buffer_length, 0, buffer_length);

    /* AAD Header. */
    inner_format_ccm_header(aad_len, buffer, &ccm_header_len);
    ret = memcpy_s(buffer + ccm_header_len, buffer_length - ccm_header_len, drv_symc_ctx->aad_virt,
        crypto_min(aad_len, buffer_length - ccm_header_len));
    crypto_chk_goto_with_ret(ret != EOK, exit_free, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    left = ccm_header_len + aad_len;
    left = crypto_align(left, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    processing_length = crypto_min(left, buffer_length);

    ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(buffer), processing_length, node_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

    left -= processing_length;
    processed_length += (processing_length - ccm_header_len);

    while (left > 0) {
        processing_length = crypto_min(left, buffer_length);
        ret = memcpy_s(buffer, buffer_length, drv_symc_ctx->aad_virt + processed_length,
            crypto_min(processing_length, aad_len - processed_length));
        crypto_chk_goto_with_ret(ret != EOK, exit_free, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

        ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(buffer), processing_length, node_type);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

        left -= processing_length;
        processed_length += processing_length;

        (td_void)memset_s(buffer, buffer_length, 0, buffer_length);
    }

exit_free:
    (td_void)memset_s(buffer, buffer_length, 0, buffer_length);
    crypto_free_coherent(buffer);
    return ret;
}

static td_s32 inner_drv_cipher_symc_ccm_process(drv_symc_context_t *symc_ctx, td_phys_addr_t src_phys_addr,
    td_phys_addr_t dst_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u32 chn_num = symc_ctx->chn_num;
    td_u32 ccm_p_node_type = 0;
    td_u8 *N = symc_ctx->dma_addr;
    td_u32 N_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES;

    crypto_chk_return(symc_ctx->dma_addr == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_UNEXPECTED), "unexpected error\n");

    if (symc_ctx->iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_START) {
        /* CCM ADD N. */
        crypto_log_dbg("ccm start process aad N\n");
        ret = inner_symc_ccm_process_N(N, N_len, symc_ctx, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_symc_ccm_process_N failed\n");

        /* CCM ADD AAD. */
        crypto_log_dbg("ccm start process aad, length is %d\n", symc_ctx->aad_len);
        ret = inner_symc_ccm_process_aad(symc_ctx, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_symc_ccm_process_aad failed\n");

        symc_ctx->iv_change_flag = CRYPTO_SYMC_CCM_IV_CHANGE_UPDATE;
    }

    /* CCM ADD P. */
    if (length != 0) {
        crypto_log_dbg("ccm start process P\n");
        ccm_p_node_type = IN_NODE_TYPE_CCM_P | IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;
        if (symc_ctx->processed_len + length == symc_ctx->data_len) {
            ccm_p_node_type |= IN_NODE_TYPE_CCM_LAST;
        }
        ret = hal_cipher_symc_add_in_node(chn_num, src_phys_addr, length, ccm_p_node_type);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed\n");

        ret = hal_cipher_symc_add_out_node(chn_num, dst_phys_addr, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_out_node failed, ret is 0x%x\n", ret);
    }
    symc_ctx->processed_len += length;
    return ret;
}

static td_s32 inner_drv_symc_gcm_process_aad(td_u8 *aad_padding, td_u32 aad_padding_len,
    const drv_symc_context_t *drv_symc_ctx)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 chn_num = drv_symc_ctx->chn_num;
    td_u32 gcm_a_node_type = 0;
    const crypto_symc_config_aes_ccm_gcm *gcm_config = &drv_symc_ctx->param.ccm_config;

    /* GCM ADD AAD. */
    if (gcm_config->aad_len != 0) {
        aad_padding_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES - gcm_config->aad_len % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
        gcm_a_node_type = IN_NODE_TYPE_GCM_A | IN_NODE_TYPE_FIRST | IN_NODE_TYPE_GCM_FIRST;
        if (aad_padding_len == CRYPTO_AES_BLOCK_SIZE_IN_BYTES) {
            gcm_a_node_type |= IN_NODE_TYPE_LAST;
        }
#if defined(CRYPTO_GCM_TRACE_ENABLE)
        crypto_dump_phys_addr("ccm aad", drv_symc_ctx->aad_phys, drv_symc_ctx->aad_len);
#endif
        ret = hal_cipher_symc_add_in_node(chn_num, drv_symc_ctx->aad_phys, drv_symc_ctx->aad_len,
            gcm_a_node_type);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed, ret is 0x%x\n", ret);

        if (aad_padding_len != CRYPTO_AES_BLOCK_SIZE_IN_BYTES) {
            /* padding must be zeros. */
            (td_void)memset_s(aad_padding, aad_padding_len, 0, aad_padding_len);
#if defined(CRYPTO_GCM_TRACE_ENABLE)
        crypto_dump_data("ccm aad_padding", aad_padding, aad_padding_len);
#endif
            ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(aad_padding),
                aad_padding_len, IN_NODE_TYPE_GCM_A | IN_NODE_TYPE_LAST);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed, ret is 0x%x\n", ret);
        }
    }
    return ret;
}

static td_s32 inner_drv_symc_gcm_process_P(drv_symc_context_t *drv_symc_ctx,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 length)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 gcm_p_node_type = 0;
    td_u32 chn_num = drv_symc_ctx->chn_num;
    const crypto_symc_config_aes_ccm_gcm *gcm_config = &drv_symc_ctx->param.ccm_config;

    /* GCM ADD P. */
    if (length != 0) {
        gcm_p_node_type = IN_NODE_TYPE_GCM_P | IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;
        if (gcm_config->aad_len == 0) {
            gcm_p_node_type |= IN_NODE_TYPE_GCM_FIRST;
        }
        ret = hal_cipher_symc_add_in_node(chn_num, src_phys_addr, length, gcm_p_node_type);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed, ret is 0x%x\n", ret);

        ret = hal_cipher_symc_add_out_node(chn_num, dst_phys_addr, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_out_node failed, ret is 0x%x\n", ret);
    }
    drv_symc_ctx->processed_len += length;
    return ret;
}

static td_s32 inner_drv_symc_gcm_process_clen(td_u8 *clen_buf, td_u32 clen_buf_len,
    const drv_symc_context_t *drv_symc_ctx, td_u32 length)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 aad_len_in_bit = 0;
    td_u32 data_len_in_bit = 0;
    td_u32 gcm_len_node_type = 0;
    const crypto_symc_config_aes_ccm_gcm *gcm_config = &drv_symc_ctx->param.ccm_config;

    /* GCM ADD LEN. */
    aad_len_in_bit = crypto_cpu_to_be32(gcm_config->aad_len * CRYPTO_BITS_IN_BYTE);
    data_len_in_bit = crypto_cpu_to_be32(length * CRYPTO_BITS_IN_BYTE);
    (td_void)memset_s(clen_buf, clen_buf_len, 0x00, clen_buf_len);
    (td_void)memcpy_s(clen_buf + 8 - sizeof(td_u32),        /* 8: bit15~8 for aad_len. */
        sizeof(td_u32), &aad_len_in_bit, sizeof(td_u32));
    (td_void)memcpy_s(clen_buf + 16 - sizeof(td_u32), sizeof(td_u32),   /* 16: bit7~0 for data_len. */
        &data_len_in_bit, sizeof(td_u32));

    gcm_len_node_type = IN_NODE_TYPE_GCM_LEN | IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;
    if (gcm_config->aad_len == 0 && length == 0) {
        gcm_len_node_type |= IN_NODE_TYPE_GCM_FIRST;
    }
#if defined(CRYPTO_GCM_TRACE_ENABLE)
    crypto_dump_data("gcm_clen_buf", clen_buf, clen_buf_len);
#endif
    ret = hal_cipher_symc_add_in_node(drv_symc_ctx->chn_num, crypto_get_phys_addr(clen_buf),
        clen_buf_len, gcm_len_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed, ret is 0x%x\n", ret);

    return ret;
}

static td_s32 inner_drv_cipher_symc_gcm_process(drv_symc_context_t *drv_symc_ctx, td_phys_addr_t src_phys_addr,
    td_phys_addr_t dst_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u8 *aad_padding = drv_symc_ctx->dma_addr;
    td_u32 aad_padding_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES;

    crypto_chk_return(drv_symc_ctx->dma_addr == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_UNEXPECTED), "unexpected error\n");
    if (drv_symc_ctx->processed_len == 0) {
        /* GCM ADD AAD. */
        crypto_log_dbg("gcm start process aad\n");
        ret = inner_drv_symc_gcm_process_aad(aad_padding, aad_padding_len, drv_symc_ctx);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_gcm_process_aad failed, ret is 0x%x\n", ret);
    }
    /* GCM ADD P. */
    crypto_log_dbg("gcm start process p\n");
    ret = inner_drv_symc_gcm_process_P(drv_symc_ctx, src_phys_addr, dst_phys_addr, length);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_gcm_process_P failed, ret is 0x%x\n", ret);

    return ret;
}

static td_s32 inner_drv_cipher_wait_func(td_u32 chn_num, drv_symc_context_t *symc_ctx)
{
    td_s32 ret;

    ret = hal_cipher_symc_start(chn_num);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_start failed\n");

    ret = hal_cipher_symc_wait_done(chn_num, TD_TRUE);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_done failed\n");

    ret = hal_cipher_symc_get_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_iv failed\n");

    return ret;
}

#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
td_s32 inner_drv_symc_set_ctr_block(td_handle symc_handle, const td_u8 *block, td_u32 block_size, td_u32 ctr_offset)
{
    td_s32 ret;
    drv_symc_context_t *symc_ctx = TD_NULL;

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_null_ptr_chk(block);
    crypto_chk_return(block_size != CRYPTO_AES_BLOCK_SIZE_IN_BYTES, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "block_size is invalid\n");

    symc_ctx = &g_drv_symc_ctx_list[symc_handle];

    ret = memcpy_s(symc_ctx->ctr_last_block, sizeof(symc_ctx->ctr_last_block), block, block_size);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    symc_ctx->ctr_offset = ctr_offset;
    return TD_SUCCESS;
}

td_s32 inner_drv_symc_get_ctr_block(td_handle symc_handle, td_u8 *block, td_u32 block_size, td_u32 *ctr_offset)
{
    td_s32 ret;
    drv_symc_context_t *symc_ctx = TD_NULL;

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_null_ptr_chk(block);
    symc_null_ptr_chk(ctr_offset);
    crypto_chk_return(block_size != CRYPTO_AES_BLOCK_SIZE_IN_BYTES, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "block_size is invalid\n");

    symc_ctx = &g_drv_symc_ctx_list[symc_handle];

    ret = memcpy_s(block, block_size, symc_ctx->ctr_last_block, sizeof(symc_ctx->ctr_last_block));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    *ctr_offset = symc_ctx->ctr_offset;
    return TD_SUCCESS;
}

static td_s32 inner_drv_cipher_ctr_process_one_zero_block(td_u32 chn_num, drv_symc_context_t *symc_ctx)
{
    td_u32 ret;
    td_u8 *zero_block = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_ctr_zero");
    crypto_chk_return(zero_block == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc_coherent failed\n");

    (td_void)memset_s(zero_block, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
#if defined(CRYPTO_CTR_TRACE_ENABLE)
    crypto_dump_data("zero_block_before_iv", symc_ctx->iv, sizeof(symc_ctx->iv));
#endif
    ret = hal_cipher_symc_set_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_set_iv failed\n");

    ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(zero_block),
        CRYPTO_AES_BLOCK_SIZE_IN_BYTES, IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

    ret = hal_cipher_symc_add_out_node(chn_num, crypto_get_phys_addr(zero_block), CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

    ret = inner_drv_cipher_wait_func(chn_num, symc_ctx);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "inner_drv_cipher_wait_func failed\n");

    ret = memcpy_s(symc_ctx->ctr_last_block, sizeof(symc_ctx->ctr_last_block),
        zero_block, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    crypto_chk_goto_with_ret(ret != EOK, exit_free, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
#if defined(CRYPTO_CTR_TRACE_ENABLE)
    crypto_dump_data("zero_block_after_iv", symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_dump_data("zero_block_after", zero_block, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
#endif
exit_free:
    crypto_free_coherent(zero_block);
    return ret;
}
#endif

static td_s32 inner_drv_cipher_symc_crypto_for_ctr(td_u32 chn_num, drv_symc_context_t *symc_ctx,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u32 processing_length = length;
#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    if (symc_ctx->ctr_offset != 0) {
        symc_ctx->ctr_used = crypto_min(CRYPTO_AES_BLOCK_SIZE_IN_BYTES - symc_ctx->ctr_offset, length);
        ret = crypto_virt_xor_phys_copy_to_phys(dst_phys_addr, symc_ctx->ctr_last_block + symc_ctx->ctr_offset,
            src_phys_addr, symc_ctx->ctr_used);
        crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_virt_xor_copy_to_phys failed\n");
        src_phys_addr += symc_ctx->ctr_used;
        dst_phys_addr += symc_ctx->ctr_used;
        crypto_log_dbg("ctr skip %d bytes\n", symc_ctx->ctr_used);
        if (length == symc_ctx->ctr_used) {
            symc_ctx->ctr_offset = (symc_ctx->ctr_offset + symc_ctx->ctr_used) % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
            return TD_SUCCESS;
        }
        processing_length -= symc_ctx->ctr_used;
    }

    symc_ctx->ctr_offset = processing_length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    processing_length -= symc_ctx->ctr_offset;
#endif
    ret = hal_cipher_symc_set_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed\n");

    if (processing_length != 0) {
        ret = inner_drv_cipher_symc_normal_process(chn_num, src_phys_addr, dst_phys_addr, processing_length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_symc_normal_process failed\n");

        ret = inner_drv_cipher_wait_func(chn_num, symc_ctx);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_wait_func failed\n");
    }

#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    if (symc_ctx->ctr_offset != 0) {
        ret = inner_drv_cipher_ctr_process_one_zero_block(chn_num, symc_ctx);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_ctr_process_one_zero_block failed\n");

        crypto_log_dbg("ctr xor %d bytes, processing_length is %d, ctr_used is %d\n",
            symc_ctx->ctr_offset, processing_length, symc_ctx->ctr_used);
        ret = crypto_virt_xor_phys_copy_to_phys(dst_phys_addr + processing_length,
            symc_ctx->ctr_last_block, src_phys_addr + processing_length, symc_ctx->ctr_offset);
        crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_virt_xor_copy_to_phys failed\n");
    }
#endif
    return ret;
}

static td_s32 inner_drv_cipher_symc_crypto_for_others(td_u32 chn_num, drv_symc_context_t *symc_ctx,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 length)
{
    td_s32 ret;
    if (symc_ctx->work_mode == CRYPTO_SYMC_WORK_MODE_CCM) {
        ret = inner_drv_cipher_symc_ccm_process(symc_ctx, src_phys_addr, dst_phys_addr, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_symc_ccm_process failed\n");
    } else if (symc_ctx->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        ret = inner_drv_cipher_symc_gcm_process(symc_ctx, src_phys_addr, dst_phys_addr, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_symc_gcm_process failed\n");
    } else {
        ret = hal_cipher_symc_set_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed\n");

        ret = inner_drv_cipher_symc_normal_process(chn_num, src_phys_addr, dst_phys_addr, length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_symc_normal_process failed\n");
    }

    ret = inner_drv_cipher_wait_func(chn_num, symc_ctx);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_cipher_wait_func failed\n");

    return ret;
}

#if defined(CRYPTO_SYMC_ADDR_NOT_ALIGN_SUPPORT)
#define SYMC_BUFFER_SIZE        4096
static td_s32 inner_drv_cipher_symc_crypto_process_unalign_addr(td_u32 chn_num, drv_symc_context_t *symc_ctx,
    const crypto_buf_attr *src_buf, const crypto_buf_attr *dst_buf, td_u32 length)
{
    td_s32 ret;
    td_u32 left = length;
    td_u32 processed_length = 0;
    td_u32 processing_length = 0;
    td_u8 *buffer = TD_NULL;
    td_phys_addr_t buffer_phys_addr = 0;

    buffer = crypto_malloc_coherent(SYMC_BUFFER_SIZE, "cipher_unalign");
    crypto_chk_return(buffer == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc_coherent failed\n");

    buffer_phys_addr = crypto_get_phys_addr(buffer);
    while (left > 0) {
        processing_length = crypto_min(left, SYMC_BUFFER_SIZE);
        crypto_log_dbg("left is %d, processing_length is %d, processed_length is %d\n",
            left, processing_length, processed_length);
        ret = crypto_phys_copy_to_virt(buffer, src_buf->phys_addr + processed_length, processing_length);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "crypto_phys_copy_to_virt failed\n");

        if (symc_ctx->work_mode == CRYPTO_SYMC_WORK_MODE_CTR) {
            ret = inner_drv_cipher_symc_crypto_for_ctr(chn_num, symc_ctx, buffer_phys_addr,
                buffer_phys_addr, processing_length);
            crypto_chk_goto(ret != TD_SUCCESS, exit_free, "inner_drv_cipher_symc_crypto_for_ctr failed\n");
        } else {
            ret = inner_drv_cipher_symc_crypto_for_others(chn_num, symc_ctx,
                buffer_phys_addr, buffer_phys_addr, processing_length);
            crypto_chk_goto(ret != TD_SUCCESS, exit_free, "inner_drv_cipher_symc_crypto_for_others failed\n");
        }
        ret = crypto_virt_copy_to_phys(dst_buf->phys_addr + processed_length, buffer, processing_length);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "crypto_phys_copy_to_virt failed\n");

        left -= processing_length;
        processed_length += processing_length;
    }

exit_free:
    (td_void)memset_s(buffer, SYMC_BUFFER_SIZE, 0, SYMC_BUFFER_SIZE);
    crypto_free_coherent(buffer);
    return ret;
}
#endif

static td_s32 inner_drv_cipher_symc_crypto(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length, td_bool is_decrypt)
{
    td_s32 ret;
    hal_symc_config_t hal_symc_config = {0};
    drv_symc_context_t *symc_ctx = TD_NULL;
    td_u32 chn_num = (td_u32)symc_handle;

    crypto_drv_func_enter();

    ret = inner_drv_symc_crypto_chk(symc_handle, src_buf, dst_buf, length);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_crypto_chk failed\n");

    symc_ctx = &g_drv_symc_ctx_list[chn_num];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_config == TD_FALSE,
        SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG), "call set_config first\n");
    crypto_chk_return(symc_ctx->is_attached == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_ATTACHED), "call attach first\n");

    (td_void)hal_cipher_set_chn_secure(chn_num, dst_buf->buf_sec == CRYPTO_BUF_SECURE,
        src_buf->buf_sec == CRYPTO_BUF_SECURE);

    ret = hal_cipher_symc_get_config(chn_num, &hal_symc_config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_config failed\n");

    hal_symc_config.is_decrypt = is_decrypt;
    ret = hal_cipher_symc_config(chn_num, &hal_symc_config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_config failed\n");

    if ((crypto_addr_align_check(src_buf->phys_addr) == TD_TRUE) &&
        (crypto_addr_align_check(dst_buf->phys_addr) == TD_TRUE)) {
        if (symc_ctx->work_mode == CRYPTO_SYMC_WORK_MODE_CTR) {
            ret = inner_drv_cipher_symc_crypto_for_ctr(chn_num, symc_ctx, src_buf->phys_addr,
                dst_buf->phys_addr, length);
        } else {
            ret = inner_drv_cipher_symc_crypto_for_others(chn_num, symc_ctx,
                src_buf->phys_addr, dst_buf->phys_addr, length);
        }
    } else {
#if defined(CRYPTO_SYMC_ADDR_NOT_ALIGN_SUPPORT)
        ret = inner_drv_cipher_symc_crypto_process_unalign_addr(chn_num, symc_ctx, src_buf, dst_buf, length);
#else
        ret = SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT);
        crypto_log_err("unsupport unalign address\n");
#endif
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_encrypt(td_handle symc_handle, const crypto_buf_attr *src_buf, const crypto_buf_attr *dst_buf,
    td_u32 length)
{
    td_s32 ret;
    crypto_drv_func_enter();

    ret = inner_drv_cipher_symc_crypto(symc_handle, src_buf, dst_buf, length, TD_FALSE);
    if (ret != TD_SUCCESS) {
        crypto_log_err("inner_drv_cipher_symc_crypto failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_decrypt(td_handle symc_handle, const crypto_buf_attr *src_buf, const crypto_buf_attr *dst_buf,
    td_u32 length)
{
    td_s32 ret;
    crypto_drv_func_enter();

    ret = inner_drv_cipher_symc_crypto(symc_handle, src_buf, dst_buf, length, TD_TRUE);
    if (ret != TD_SUCCESS) {
        crypto_log_err("inner_drv_cipher_symc_crypto failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_drv_func_exit();
    return ret;
}

static td_s32 priv_drv_symc_add_node(td_u32 chn_num, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length, in_node_type_e node_type)
{
    td_s32 ret;

    ret = hal_cipher_symc_add_in_node(chn_num, src_buf->phys_addr, length, node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed, ret is 0x%x\n", ret);

    ret = hal_cipher_symc_add_out_node(chn_num, dst_buf->phys_addr, length);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_out_node failed, ret is 0x%x\n", ret);

    return ret;
}

static td_s32 priv_drv_symc_crypto_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num, td_u32 crypto_type)
{
    td_s32 ret;
    td_u32 i;
    hal_symc_config_t hal_symc_config = {0};
    td_u32 chn_num = symc_handle;
    in_node_type_e node_type = IN_NODE_TYPE_NORMAL;
    drv_symc_context_t *symc_ctx = TD_NULL;

    crypto_drv_func_enter();

    symc_null_ptr_chk(symc_ctrl);
    symc_null_ptr_chk(src_buf_pack);
    symc_null_ptr_chk(dst_buf_pack);

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_ctx = &g_drv_symc_ctx_list[chn_num];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    hal_symc_config.symc_alg = symc_ctrl->symc_alg;
    hal_symc_config.work_mode = symc_ctrl->work_mode;
    hal_symc_config.symc_bit_width = symc_ctrl->symc_bit_width;
    hal_symc_config.symc_key_length = symc_ctrl->symc_key_length;
    hal_symc_config.is_decrypt = (crypto_type == CRYPTO_TYPE_ENCRYPT) ? TD_FALSE : TD_TRUE;

    ret = hal_cipher_symc_config(chn_num, &hal_symc_config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_config failed, ret is 0x%x\n", ret);

    if (symc_ctrl->iv_change_flag == CRYPTO_SYMC_IV_CHANGE_ONE_PKG) {
        for (i = 0; i < pack_num; i++, node_type = IN_NODE_TYPE_NORMAL) {
            ret = hal_cipher_symc_set_iv(chn_num, symc_ctrl->iv, symc_ctrl->iv_length);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed, ret is 0x%x\n", ret);
            node_type = IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST;

            ret = priv_drv_symc_add_node(chn_num, &src_buf_pack[i].buf_attr, &dst_buf_pack[i].buf_attr,
                src_buf_pack[i].length, node_type);
            crypto_chk_return(ret != TD_SUCCESS, ret, "priv_drv_symc_add_node failed, ret is 0x%x\n", ret);

            ret = hal_cipher_symc_start(chn_num);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_start failed, ret is 0x%x\n", ret);

            ret = hal_cipher_symc_wait_done(chn_num, TD_TRUE);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_done failed, ret is 0x%x\n", ret);
            ret = hal_cipher_symc_get_iv(chn_num, (td_u8 *)symc_ctrl->iv, symc_ctrl->iv_length);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_iv failed, ret is 0x%x\n", ret);
        }
    } else if (symc_ctrl->iv_change_flag == CRYPTO_SYMC_IV_CHANGE_ALL_PKG) {
        for (i = 0; i < pack_num; i++) {
            ret = hal_cipher_symc_set_iv(chn_num, symc_ctrl->iv, symc_ctrl->iv_length);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed, ret is 0x%x\n", ret);

            ret = priv_drv_symc_add_node(chn_num, &src_buf_pack[i].buf_attr, &dst_buf_pack[i].buf_attr,
                src_buf_pack[i].length, IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
            crypto_chk_return(ret != TD_SUCCESS, ret, "priv_drv_symc_add_node failed, ret is 0x%x\n", ret);

            ret = hal_cipher_symc_start(chn_num);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_start failed, ret is 0x%x\n", ret);

            ret = hal_cipher_symc_wait_done(chn_num, TD_TRUE);
            crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_done failed, ret is 0x%x\n", ret);
        }
    } else {
        crypto_log_err("Invalid iv_change_flag!\n");
        return TD_FAILURE;
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_encrypt_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_s32 ret;
    crypto_drv_func_enter();

    ret = priv_drv_symc_crypto_multi(symc_handle, symc_ctrl, src_buf_pack, dst_buf_pack, pack_num, CRYPTO_TYPE_ENCRYPT);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_drv_symc_crypto_multi for Encrypt failed\n");

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_decrypt_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_s32 ret;
    crypto_drv_func_enter();

    ret = priv_drv_symc_crypto_multi(symc_handle, symc_ctrl, src_buf_pack, dst_buf_pack, pack_num, CRYPTO_TYPE_DECRYPT);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_drv_symc_crypto_multi for Encrypt failed\n");

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_symc_get_tag(td_handle symc_handle, td_u8 *tag, td_u32 tag_length)
{
    td_s32 ret;
    td_u32 chn_num;
    td_u8 *clen_buf;
    td_u32 clen_buf_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    drv_symc_context_t *symc_ctx = TD_NULL;
    crypto_drv_func_enter();
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    symc_null_ptr_chk(tag);
    crypto_chk_return(tag_length == 0 || tag_length > CRYPTO_AES_MAX_TAG_SIZE,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "tag_length is invalid\n");
    chn_num = symc_handle;
    symc_ctx = &g_drv_symc_ctx_list[chn_num];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_config == TD_FALSE,
        SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG), "call set_config first\n");
    crypto_chk_return(symc_ctx->is_attached == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_ATTACHED), "call attach first\n");

    /* if work mode equals GCM, process clen. */
    if (symc_ctx->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        crypto_log_dbg("gcm start process clen\n");
        clen_buf = symc_ctx->dma_addr + CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
        ret = inner_drv_symc_gcm_process_clen(clen_buf, clen_buf_len, symc_ctx, symc_ctx->processed_len);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_gcm_process_clen failed, ret is 0x%x\n", ret);
    }

    ret = hal_cipher_symc_get_tag(chn_num, tag, tag_length);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_get_tag failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 inner_drv_get_mac_ctx(td_handle symc_handle, crypto_symc_mac_ctx *mac_ctx)
{
    td_s32 ret;
    drv_symc_context_t *symc_ctx = TD_NULL;

    symc_null_ptr_chk(mac_ctx);
    symc_ctx = inner_get_symc_ctx(symc_handle);
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "symc_handle is invalid\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    ret = memcpy_s(mac_ctx->tail, sizeof(mac_ctx->tail), symc_ctx->tail, symc_ctx->tail_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    mac_ctx->tail_length = symc_ctx->tail_length;

    ret = memcpy_s(mac_ctx->mac, sizeof(mac_ctx->mac), symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return ret;
}

td_s32 inner_drv_set_mac_ctx(td_handle symc_handle, const crypto_symc_mac_ctx *mac_ctx)
{
    td_s32 ret;
    drv_symc_context_t *symc_ctx = TD_NULL;

    symc_null_ptr_chk(mac_ctx);
    symc_ctx = inner_get_symc_ctx(symc_handle);
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "symc_handle is invalid\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    ret = memcpy_s(symc_ctx->tail, sizeof(symc_ctx->tail), mac_ctx->tail, mac_ctx->tail_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    symc_ctx->tail_length = mac_ctx->tail_length;

    ret = memcpy_s(symc_ctx->iv, sizeof(symc_ctx->iv), mac_ctx->mac, sizeof(mac_ctx->mac));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return ret;
}

td_s32 inner_drv_symc_crypto_chk(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length)
{
    td_s32 ret;
    drv_symc_context_t *symc_ctx = TD_NULL;
    crypto_symc_work_mode mode;

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    symc_ctx = &g_drv_symc_ctx_list[symc_handle];

    symc_null_ptr_chk(src_buf);
    symc_null_ptr_chk(dst_buf);
    crypto_chk_return(src_buf->phys_addr == 0, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "src_buf's phys_addr is invalid\n");
    crypto_chk_return(dst_buf->phys_addr == 0, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "dst_buf's phys_addr is invalid\n");

    crypto_chk_return(length == 0, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "length is zero!\n");
    mode = symc_ctx->work_mode;
    if (mode == CRYPTO_SYMC_WORK_MODE_CBC || mode == CRYPTO_SYMC_WORK_MODE_ECB) {
        crypto_chk_return((length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES) != 0, SYMC_COMPAT_ERRNO(ERROR_SYMC_LEN_NOT_ALIGNED),
            "length must be aligned to 16-Byte\n");
    }
    return TD_SUCCESS;
}

td_s32 inner_drv_symc_ex_restore(td_handle symc_handle, const drv_symc_ex_context_t *symc_ex_ctx)
{
    td_s32 ret;
    td_u32 chn_num = symc_handle;
    hal_symc_config_t config = {0};
    drv_symc_context_t *symc_ctx = TD_NULL;

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_null_ptr_chk(symc_ex_ctx);
    symc_null_ptr_chk(symc_ex_ctx->iv0);
    symc_null_ptr_chk(symc_ex_ctx->iv_mac);
    crypto_chk_return(symc_ex_ctx->iv0_length != sizeof(config.iv0), SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "iv0_length is invalid\n");
    crypto_chk_return(symc_ex_ctx->iv_mac_length != sizeof(config.iv_mac), SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "iv_mac_length is invalid\n");

    symc_ctx = &g_drv_symc_ctx_list[symc_handle];

    ret = hal_cipher_symc_get_config(chn_num, &config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_config failed\n");

    ret = memcpy_s(config.iv0, sizeof(config.iv0), symc_ex_ctx->iv0, symc_ex_ctx->iv0_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    ret = memcpy_s(config.iv_mac, sizeof(config.iv_mac), symc_ex_ctx->iv_mac, symc_ex_ctx->iv_mac_length);
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    ret = hal_cipher_symc_config(chn_num, &config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_config failed\n");

    symc_ctx->param.ccm_config.aad_len = symc_ex_ctx->aad_len;
    symc_ctx->data_len = symc_ex_ctx->data_length;
    symc_ctx->processed_len = symc_ex_ctx->processed_length;

    return TD_SUCCESS;
}

td_s32 inner_drv_symc_get_iv0(td_handle symc_handle, td_u8 *iv0, td_u32 iv0_length)
{
    td_s32 ret;
    td_u32 chn_num = symc_handle;
    hal_symc_config_t config = {0};

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_null_ptr_chk(iv0);
    crypto_chk_return(iv0_length != sizeof(config.iv0), SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "iv0_length is invalid\n");

    ret = hal_cipher_symc_get_config(chn_num, &config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_config failed\n");

    ret = memcpy_s(iv0, iv0_length, config.iv0, sizeof(config.iv0));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return TD_SUCCESS;
}

td_s32 inner_drv_symc_get_iv_mac(td_handle symc_handle, td_u8 *iv_mac, td_u32 iv_mac_length)
{
    td_s32 ret;
    td_u32 chn_num = symc_handle;
    hal_symc_config_t config = {0};

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_null_ptr_chk(iv_mac);
    crypto_chk_return(iv_mac_length != sizeof(config.iv_mac), SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "iv_mac_length is invalid\n");

    ret = hal_cipher_symc_get_config(chn_num, &config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_config failed\n");

    ret = memcpy_s(iv_mac, iv_mac_length, config.iv_mac, sizeof(config.iv_mac));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return TD_SUCCESS;
}