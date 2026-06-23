/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "kapi_symc.h"
#include "kapi_inner.h"
#include "drv_symc.h"
#include "drv_inner.h"
#include "crypto_drv_common.h"

#define CRYPTO_KAPI_DMA_SIZE_ONE_PROCESS    (CRYPTO_DRV_AAD_SIZE * CRYPTO_SYMC_VIRT_CHN_NUM)
#define CRYPTO_KAPI_TOTAL_DMA_SIZE          (CRYPTO_KAPI_DMA_SIZE_ONE_PROCESS * MAX_PROCESS_NUM)
#define CRYPTO_SYMC_INIT_MAX_NUM            0xffffffff

static td_u8 *g_kapi_dma_addr = TD_NULL;

typedef struct {
    td_bool is_open;
    td_bool is_long_term;
    td_bool is_mac;
    td_bool is_multi_pack;
    td_u32 tid;
    td_handle drv_symc_handle;
    crypto_symc_attr symc_attr;
    td_handle keyslot_handle;
    td_bool is_attached;
    td_bool is_set_config;
    td_u8 *aad_buf;
    td_u32 aad_buf_size;
    crypto_symc_ctrl_t symc_ctrl;
    crypto_symc_config_aes_ccm_gcm ccm_gcm_config;
    crypto_symc_mac_attr mac_attr;
    crypto_symc_mac_ctx mac_ctx;
    /* The length should be processed. */
    td_u32 data_length;
    /* The length has been processd. */
    td_u32 processed_length;
    /* iv_mac for ccm, iv_ghash for gcm. */
    td_u8 iv_mac[16];
    /* s0 for ccm, j0 for gcm. */
    td_u8 iv0[16];
#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    td_u32 ctr_offset;
    td_u8 ctr_last_block[CRYPTO_AES_BLOCK_SIZE_IN_BYTES];
#endif
} crypto_kapi_symc_ctx;

typedef struct {
    crypto_owner owner;
    crypto_kapi_symc_ctx symc_ctx_list[CRYPTO_SYMC_VIRT_CHN_NUM];
    crypto_mutex symc_ctx_mutex[CRYPTO_SYMC_VIRT_CHN_NUM];
    td_u32 ctx_num;
    td_bool is_used;
    td_u32 init_counter;
    td_u8 *dma_addr;
} crypto_kapi_symc_process;

static crypto_kapi_symc_process g_kapi_symc_channel[MAX_PROCESS_NUM];

static crypto_mutex g_symc_mutex;

#define kapi_symc_lock() do {                   \
    crypto_mutex_lock(&g_symc_mutex);           \
} while (0)
#define kapi_symc_unlock() do {                 \
    crypto_mutex_unlock(&g_symc_mutex);         \
} while (0)

#define SYMC_COMPAT_ERRNO(err_code)     KAPI_COMPAT_ERRNO(ERROR_MODULE_SYMC, err_code)
#define symc_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

static td_s32 priv_symc_handle_check(td_handle kapi_symc_handle)
{
    crypto_chk_return(kapi_get_module_id(kapi_symc_handle) != KAPI_SYMC_MODULE_ID,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "Invalid Symc Handle! 0x%x\n", kapi_symc_handle);
    crypto_chk_return(kapi_get_ctx_idx(kapi_symc_handle) >= CRYPTO_SYMC_VIRT_CHN_NUM,  \
        SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "Invalid Symc Handle! 0x%x\n", kapi_symc_handle);
    return TD_SUCCESS;
}

static crypto_kapi_symc_process *priv_get_current_symc_channel(td_void)
{
    td_u32 i;
    crypto_owner owner;
    if (crypto_get_owner(&owner) != CRYPTO_SUCCESS) {
        return TD_NULL;
    }
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (memcmp(&owner, &g_kapi_symc_channel[i].owner, sizeof(crypto_owner)) == 0 &&
            g_kapi_symc_channel[i].is_used == TD_TRUE) {
            return &g_kapi_symc_channel[i];
        }
    }
    return TD_NULL;
}

static crypto_kapi_symc_ctx *priv_occupy_symc_soft_chn(crypto_kapi_symc_process *symc_channel, td_u32 *idx)
{
    td_u32 i;
    td_u32 tid = crypto_gettid();
    crypto_kapi_symc_ctx *symc_ctx_list = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;

    kapi_symc_lock();

    symc_ctx_list = symc_channel->symc_ctx_list;
    for (i = 0; i < CRYPTO_SYMC_VIRT_CHN_NUM; i++) {
        if (symc_ctx_list[i].is_open == TD_FALSE) {
            break;
        }
    }
    if (i >= CRYPTO_SYMC_VIRT_CHN_NUM) {
        crypto_log_err("All Symc Channels are busy!\n");
        goto exit_unlock;
    }
    symc_ctx = &symc_channel->symc_ctx_list[i];
    (td_void)memset_s(symc_ctx, sizeof(crypto_kapi_symc_ctx), 0, sizeof(crypto_kapi_symc_ctx));
    symc_ctx->is_open = TD_TRUE;
    symc_ctx->tid = tid;

    *idx = i;

exit_unlock:
    kapi_symc_unlock();
    return symc_ctx;
}

static td_void priv_release_symc_soft_chn(crypto_kapi_symc_ctx *symc_ctx)
{
    kapi_symc_lock();
    (td_void)memset_s(symc_ctx, sizeof(crypto_kapi_symc_ctx), 0, sizeof(crypto_kapi_symc_ctx));
    kapi_symc_unlock();
}

static td_bool priv_check_is_init(crypto_owner *owner)
{
    td_u32 i;
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (memcmp(owner, &g_kapi_symc_channel[i].owner, sizeof(crypto_owner)) == 0) {
            return TD_TRUE;
        }
    }
    return TD_FALSE;
}

static td_s32 priv_process_symc_init(td_void)
{
    td_u32 i;
    td_u32 ret = TD_SUCCESS;
    crypto_owner owner;
    crypto_kapi_symc_process *symc_channel = TD_NULL;

    crypto_kapi_func_enter();
    kapi_symc_lock();
    ret = crypto_get_owner(&owner);
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_unlock, SYMC_COMPAT_ERRNO(ERROR_GET_OWNER),
        "crypto_get_owner failed\n");
    if (priv_check_is_init(&owner) == TD_TRUE) {
        symc_channel = priv_get_current_symc_channel();
        if (symc_channel->init_counter >= CRYPTO_SYMC_INIT_MAX_NUM) {
            ret = SYMC_COMPAT_ERRNO(ERROR_COUNT_OVERFLOW);
        } else {
            ret = TD_SUCCESS;
            ++(symc_channel->init_counter);
        }
        goto exit_unlock;
    }
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (g_kapi_symc_channel[i].is_used == TD_FALSE) {
            break;
        }
    }
    if (i >= MAX_PROCESS_NUM) {
        crypto_log_err("Process Num is More Than %u\n", MAX_PROCESS_NUM);
        ret = SYMC_COMPAT_ERRNO(ERROR_MAX_PROCESS);
        goto exit_unlock;
    }
    symc_channel = &g_kapi_symc_channel[i];
    (td_void)memset_s(symc_channel, sizeof(crypto_kapi_symc_process), 0, sizeof(crypto_kapi_symc_process));
    /* Alloc dma memory. */
    symc_channel->dma_addr = g_kapi_dma_addr + i * CRYPTO_KAPI_DMA_SIZE_ONE_PROCESS;
    for (i = 0; i < CRYPTO_SYMC_VIRT_CHN_NUM; ++i) {
        ret = crypto_mutex_init(&symc_channel->symc_ctx_mutex[i]);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit_destroy, SYMC_COMPAT_ERRNO(ERROR_MUTEX_INIT),
            "symc ctx mutex init failed at chn: %u\n", i);
    }
    (td_void)memcpy_s(&symc_channel->owner, sizeof(crypto_owner), &owner, sizeof(crypto_owner));
    symc_channel->is_used = TD_TRUE;
    symc_channel->ctx_num = CRYPTO_SYMC_VIRT_CHN_NUM;
    ++(symc_channel->init_counter);
    (td_void)memset_s(symc_channel->symc_ctx_list, sizeof(crypto_kapi_symc_ctx) * CRYPTO_SYMC_VIRT_CHN_NUM,
        0, sizeof(crypto_kapi_symc_ctx) * CRYPTO_SYMC_VIRT_CHN_NUM);
    kapi_symc_unlock();

    crypto_kapi_func_exit();
    return ret;

exit_destroy:
    for (i = 0; i < CRYPTO_SYMC_VIRT_CHN_NUM; ++i) {
        crypto_mutex_destroy(&symc_channel->symc_ctx_mutex[i]);
    }
exit_unlock:
    kapi_symc_unlock();
    crypto_kapi_func_exit();
    return ret;
}

static td_s32 priv_drv_lock_create(crypto_kapi_symc_ctx *symc_ctx, const crypto_symc_attr *symc_attr)
{
    td_s32 ret = TD_SUCCESS;
    kapi_symc_lock();
    ret = drv_cipher_symc_create(&symc_ctx->drv_symc_handle, symc_attr);
    kapi_symc_unlock();
    return ret;
}

static td_void priv_drv_lock_destroy(td_handle drv_symc_handle)
{
    kapi_symc_lock();
    (td_void)drv_cipher_symc_destroy(drv_symc_handle);
    kapi_symc_unlock();
}

static td_s32 priv_drv_lock_mac_start(td_handle *kapi_symc_handle, const crypto_symc_mac_attr *mac_attr)
{
    td_s32 ret = TD_SUCCESS;
    kapi_symc_lock();
    ret = drv_cipher_mac_start(kapi_symc_handle, mac_attr);
    kapi_symc_unlock();
    return ret;
}

static td_s32 priv_drv_lock_mac_finish(td_handle kapi_symc_handle, td_u8 *mac, td_u32 *mac_length)
{
    td_s32 ret;
    kapi_symc_lock();
    ret = drv_cipher_mac_finish(kapi_symc_handle, mac, mac_length);
    kapi_symc_unlock();
    return ret;
}

static td_void priv_process_release(crypto_kapi_symc_process *symc_channel)
{
    td_u32 i;
    for (i = 0; i < CRYPTO_SYMC_VIRT_CHN_NUM; ++i) {
        crypto_mutex_destroy(&symc_channel->symc_ctx_mutex[i]);
        if (symc_channel->symc_ctx_list[i].is_open == TD_TRUE &&  \
            symc_channel->symc_ctx_list[i].is_long_term == TD_TRUE) {
            (td_void)drv_cipher_symc_destroy(symc_channel->symc_ctx_list[i].drv_symc_handle);
        }
    }
    (td_void)memset_s(symc_channel, sizeof(crypto_kapi_symc_process), 0, sizeof(crypto_kapi_symc_process));
}

static td_void priv_process_symc_deinit(td_void)
{
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_func_enter();
    kapi_symc_lock();
    symc_channel = priv_get_current_symc_channel();
    if (symc_channel == TD_NULL) {
        kapi_symc_unlock();
        return;
    }
    if (symc_channel->init_counter > 1) {
        --(symc_channel->init_counter);
        kapi_symc_unlock();
        return;
    }
    priv_process_release(symc_channel);
    kapi_symc_unlock();
    crypto_kapi_func_exit();
}

td_void kapi_cipher_symc_process_release(td_void)
{
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_func_enter();
    kapi_symc_lock();
    symc_channel = priv_get_current_symc_channel();
    if (symc_channel == TD_NULL) {
        kapi_symc_unlock();
        return;
    }
    priv_process_release(symc_channel);
    kapi_symc_unlock();
    crypto_kapi_func_exit();
}

td_s32 kapi_cipher_symc_env_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();
    ret = drv_cipher_symc_init();
    if (ret != TD_SUCCESS) {
        crypto_log_err("drv_cipher_symc_init failed\n");
        return ret;
    }

    (td_void)memset_s(&g_kapi_symc_channel, sizeof(g_kapi_symc_channel), 0, sizeof(g_kapi_symc_channel));
    ret = crypto_mutex_init(&g_symc_mutex);
    if (ret != TD_SUCCESS) {
        crypto_log_err("crypto_mutex_init failed\n");
        ret = SYMC_COMPAT_ERRNO(ERROR_MUTEX_INIT);
        goto error_symc_deinit;
    }

    g_kapi_dma_addr = crypto_malloc_coherent(CRYPTO_KAPI_TOTAL_DMA_SIZE, "cipher_symc_kapi");
    crypto_chk_goto_with_ret(g_kapi_dma_addr == TD_NULL, error_symc_deinit, SYMC_COMPAT_ERRNO(ERROR_MALLOC),
        "crypto_malloc_coherent failed\n");

    crypto_kapi_func_exit();
    return ret;

error_symc_deinit:
    drv_cipher_symc_deinit();
    return ret;
}

td_s32 kapi_cipher_symc_env_deinit(td_void)
{
    td_u32 i, j;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    crypto_kapi_func_enter();

    if (g_kapi_dma_addr != TD_NULL) {
        (td_void)memset_s(g_kapi_dma_addr, CRYPTO_KAPI_TOTAL_DMA_SIZE, 0, CRYPTO_KAPI_TOTAL_DMA_SIZE);
        crypto_free_coherent(g_kapi_dma_addr);
        g_kapi_dma_addr = TD_NULL;
    }

    /* release all the resource. */
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        symc_channel = &g_kapi_symc_channel[i];
        if (symc_channel->is_used == TD_FALSE) {
            continue;
        }
        for (j = 0; j < CRYPTO_SYMC_VIRT_CHN_NUM; j++) {
            symc_ctx = &symc_channel->symc_ctx_list[j];
            if (symc_ctx->is_open == TD_FALSE) {
                continue;
            }
            priv_release_symc_soft_chn(symc_ctx);
            symc_ctx->is_open = TD_FALSE;
        }
        symc_channel->is_used = TD_FALSE;
    }
    crypto_mutex_destroy(&g_symc_mutex);
    (td_void)memset_s(&g_kapi_symc_channel, sizeof(g_kapi_symc_channel), 0, sizeof(g_kapi_symc_channel));
    drv_cipher_symc_deinit();
    crypto_kapi_func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_cipher_symc_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    ret = priv_process_symc_init();
    if (ret != TD_SUCCESS) {
        crypto_log_err("symc priv_process_symc_init failed\n");
        return ret;
    }

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_init);

td_s32 kapi_cipher_symc_deinit(td_void)
{
    crypto_kapi_func_enter();
    priv_process_symc_deinit();
    crypto_kapi_func_exit();
    return TD_SUCCESS;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_deinit);

td_s32 kapi_cipher_symc_create(td_handle *kapi_symc_handle, const crypto_symc_attr *symc_attr)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;

    crypto_kapi_func_enter();
    symc_null_ptr_chk(kapi_symc_handle);
    symc_null_ptr_chk(symc_attr);

    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first!\n");

    symc_ctx = priv_occupy_symc_soft_chn(symc_channel, &idx);
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_CHN_BUSY), "all symc soft chns are busy\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_attr->is_long_term == TD_TRUE) {
        ret = priv_drv_lock_create(symc_ctx, symc_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_symc_ctx, "drv_cipher_symc_create failed, ret is 0x%x\n", ret);
    }
    ret = memcpy_s(&symc_ctx->symc_attr, sizeof(crypto_symc_attr), symc_attr, sizeof(crypto_symc_attr));
    crypto_chk_goto_with_ret(ret != EOK, error_unlock_symc_ctx, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    symc_ctx->is_long_term = symc_attr->is_long_term;

    symc_ctx->aad_buf = symc_channel->dma_addr + idx * CRYPTO_DRV_AAD_SIZE;
    symc_ctx->aad_buf_size = CRYPTO_DRV_AAD_SIZE;
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    *kapi_symc_handle = synthesize_kapi_handle(KAPI_SYMC_MODULE_ID, idx);
    crypto_kapi_func_exit();
    return ret;

error_unlock_symc_ctx:
    priv_release_symc_soft_chn(symc_ctx);
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_create);

td_s32 kapi_cipher_symc_destroy(td_handle kapi_symc_handle)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first!\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
    }
    priv_release_symc_soft_chn(symc_ctx);
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_destroy);

/* used for CCM&GCM. */
static td_s32 inner_symc_set_config_ex(crypto_kapi_symc_ctx *symc_ctx,
    const crypto_symc_config_aes_ccm_gcm *ccm_gcm_config)
{
    td_s32 ret = TD_FAILURE;
    symc_ctx->symc_ctrl.param = (void *)&symc_ctx->ccm_gcm_config;
    symc_ctx->data_length = ccm_gcm_config->data_len;
    (td_void)memcpy_s(&symc_ctx->ccm_gcm_config, sizeof(crypto_symc_config_aes_ccm_gcm),
        ccm_gcm_config, sizeof(crypto_symc_config_aes_ccm_gcm));
    if (ccm_gcm_config->aad_len != 0) {
        crypto_chk_return(ccm_gcm_config->aad_buf.virt_addr == TD_NULL,
            SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "aad_virt is NULL\n");
        crypto_chk_return(symc_ctx->aad_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_UNEXPECTED), "unexpected error\n");

        ret = memcpy_s(symc_ctx->aad_buf, symc_ctx->aad_buf_size,
            ccm_gcm_config->aad_buf.virt_addr, ccm_gcm_config->aad_len);
        crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

        symc_ctx->ccm_gcm_config.aad_buf.virt_addr = symc_ctx->aad_buf;
        symc_ctx->ccm_gcm_config.aad_buf.phys_addr = crypto_get_phys_addr(symc_ctx->aad_buf);
    }
    return TD_SUCCESS;
}

td_s32 kapi_cipher_symc_set_config(td_handle kapi_symc_handle, const crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    crypto_symc_config_aes_ccm_gcm *ccm_gcm_config = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    symc_null_ptr_chk(symc_ctrl);

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first!\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    (td_void)memset_s(&symc_ctx->ccm_gcm_config, sizeof(crypto_symc_config_aes_ccm_gcm), 0,
        sizeof(crypto_symc_config_aes_ccm_gcm));
    (td_void)memcpy_s(&symc_ctx->symc_ctrl, sizeof(crypto_symc_ctrl_t), symc_ctrl, sizeof(crypto_symc_ctrl_t));

    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM || symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        ccm_gcm_config = symc_ctrl->param;
        crypto_chk_goto_with_ret(ccm_gcm_config == TD_NULL, unlock_exit, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
            "ccm_gcm_config is NULL\n");

        ret = inner_symc_set_config_ex(symc_ctx, ccm_gcm_config);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "inner_symc_set_config_ex failed\n");
    }
    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_symc_set_config(symc_ctx->drv_symc_handle, &symc_ctx->symc_ctrl);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_symc_set_config failed\n");
    } else {
        ret = inner_symc_cfg_param_check(symc_ctrl);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "inner_symc_cfg_param_check failed\n");
    }

#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    symc_ctx->ctr_offset = 0;
#endif
    symc_ctx->is_set_config = TD_TRUE;

unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_set_config);

td_s32 kapi_cipher_symc_get_config(td_handle kapi_symc_handle, crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    symc_null_ptr_chk(symc_ctrl);

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_set_config == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG),
        "set config first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_symc_get_config(symc_ctx->drv_symc_handle, &symc_ctx->symc_ctrl);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_symc_get_config failed\n");
    }
    (td_void)memcpy_s(symc_ctrl, sizeof(crypto_symc_ctrl_t), &symc_ctx->symc_ctrl, sizeof(crypto_symc_ctrl_t));
    if (symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_CCM || symc_ctrl->work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        crypto_chk_goto_with_ret(symc_ctrl->param == TD_NULL, unlock_exit, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
            "symc_ctrl->param is null\n");
        (td_void)memcpy_s(symc_ctrl->param, sizeof(crypto_symc_config_aes_ccm_gcm),
            &symc_ctx->ccm_gcm_config, sizeof(crypto_symc_config_aes_ccm_gcm));
    }

unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_get_config);

td_s32 kapi_cipher_symc_attach(td_handle kapi_symc_handle, td_handle keyslot_handle)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);

    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_symc_attach(symc_ctx->drv_symc_handle, keyslot_handle);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_symc_attach failed, ret is 0x%x\n", ret);
    }
    symc_ctx->keyslot_handle = keyslot_handle;
    symc_ctx->is_attached = TD_TRUE;

unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_attach);

static td_s32 inner_kapi_cipher_symc_crypto(td_handle symc_handle,
    const crypto_buf_attr *src_buf, const crypto_buf_attr *dst_buf, td_u32 length, td_u32 crypto_type)
{
    td_s32 ret;
    crypto_buf_attr drv_src_buf = {0};
    crypto_buf_attr drv_dst_buf = {0};

    /* Transfer phys_addr from buf_attr. */
    drv_src_buf.phys_addr = crypto_osal_get_phys_addr(src_buf);
    crypto_chk_goto_with_ret(drv_src_buf.phys_addr == 0, exit_put_phys_addr, SYMC_COMPAT_ERRNO(ERROR_GET_PHYS_ADDR),
        "crypto_osal_get_phys_addr failed\n");
    drv_src_buf.buf_sec = src_buf->buf_sec;
    drv_dst_buf.phys_addr = crypto_osal_get_phys_addr(dst_buf);
    crypto_chk_goto_with_ret(drv_dst_buf.phys_addr == 0, exit_put_phys_addr, SYMC_COMPAT_ERRNO(ERROR_GET_PHYS_ADDR),
        "crypto_osal_get_phys_addr failed\n");
    drv_dst_buf.buf_sec = dst_buf->buf_sec;

    switch (crypto_type) {
        case CRYPTO_TYPE_ENCRYPT:
            ret = drv_cipher_symc_encrypt(symc_handle, &drv_src_buf, &drv_dst_buf, length);
            break;
        case CRYPTO_TYPE_DECRYPT:
            ret = drv_cipher_symc_decrypt(symc_handle, &drv_src_buf, &drv_dst_buf, length);
            break;
        default:
            crypto_log_err("Invalid Crypto Type!\n");
            ret = SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

exit_put_phys_addr:
    if (drv_src_buf.phys_addr != 0) {
        crypto_osal_put_phys_addr(src_buf, drv_src_buf.phys_addr);
    }
    if (drv_dst_buf.phys_addr != 0) {
        crypto_osal_put_phys_addr(dst_buf, drv_dst_buf.phys_addr);
    }
    return ret;
}

static td_s32 inner_kapi_symc_restore(crypto_kapi_symc_ctx *symc_ctx)
{
    td_s32 ret;
    drv_symc_ex_context_t symc_ex_ctx = {0};

    ret = drv_cipher_symc_attach(symc_ctx->drv_symc_handle, symc_ctx->keyslot_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_symc_attach failed, ret is 0x%x\n", ret);

    ret = drv_cipher_symc_set_config(symc_ctx->drv_symc_handle, &symc_ctx->symc_ctrl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_symc_set_config failed\n");

    if (symc_ctx->processed_length == 0) {
        ret = inner_drv_symc_get_iv0(symc_ctx->drv_symc_handle, symc_ctx->iv0, sizeof(symc_ctx->iv0));
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_get_iv0 failed\n");
    }

    symc_ex_ctx.iv0 = symc_ctx->iv0;
    symc_ex_ctx.iv0_length = sizeof(symc_ctx->iv0);
    symc_ex_ctx.iv_mac = symc_ctx->iv_mac;
    symc_ex_ctx.iv_mac_length = sizeof(symc_ctx->iv_mac);
    symc_ex_ctx.data_length = symc_ctx->data_length;
    symc_ex_ctx.processed_length = symc_ctx->processed_length;
    symc_ex_ctx.aad_len = symc_ctx->ccm_gcm_config.aad_len;

    ret = inner_drv_symc_ex_restore(symc_ctx->drv_symc_handle, &symc_ex_ctx);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_ex_restore failed\n");

#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    ret = inner_drv_symc_set_ctr_block(symc_ctx->drv_symc_handle, symc_ctx->ctr_last_block,
        sizeof(symc_ctx->ctr_last_block), symc_ctx->ctr_offset);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_get_ctr_block failed\n");
#endif
    return ret;
}

static td_s32 inner_kapi_symc_get_config(crypto_kapi_symc_ctx *symc_ctx)
{
    td_s32 ret;

    ret = inner_drv_symc_get_iv_mac(symc_ctx->drv_symc_handle, symc_ctx->iv_mac, sizeof(symc_ctx->iv_mac));
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_get_iv_mac failed\n");

    ret = drv_cipher_symc_get_config(symc_ctx->drv_symc_handle, &symc_ctx->symc_ctrl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_symc_get_config failed\n");

#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    ret = inner_drv_symc_get_ctr_block(symc_ctx->drv_symc_handle, symc_ctx->ctr_last_block,
        sizeof(symc_ctx->ctr_last_block), &symc_ctx->ctr_offset);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_drv_symc_get_ctr_block failed\n");
#endif
    return ret;
}

static td_s32 inner_kapi_symc_crypto_short_term(crypto_kapi_symc_ctx *symc_ctx,
    const crypto_buf_attr *src_buf, const crypto_buf_attr *dst_buf, td_u32 length, td_u32 crypto_type)
{
    td_s32 ret;

    ret = priv_drv_lock_create(symc_ctx, &symc_ctx->symc_attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_drv_lock_create failed, ret is 0x%x\n", ret);

    ret = inner_kapi_symc_restore(symc_ctx);
    crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit,
        "inner_kapi_symc_restore for crypto failed, ret is 0x%x\n", ret);

    ret = inner_kapi_cipher_symc_crypto(symc_ctx->drv_symc_handle, src_buf, dst_buf, length, crypto_type);
    crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit,
        "inner_kapi_cipher_symc_crypto for crypto failed, ret is 0x%x\n", ret);

    ret = inner_kapi_symc_get_config(symc_ctx);
    crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit,
        "inner_kapi_symc_get_config for crypto failed, ret is 0x%x\n", ret);

symc_destroy_exit:
    priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
    return ret;
}

static td_s32 inner_kapi_symc_get_tag_short_term(crypto_kapi_symc_ctx *symc_ctx, td_u8 *tag, td_u32 tag_len)
{
    td_s32 ret;

    ret = priv_drv_lock_create(symc_ctx, &symc_ctx->symc_attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_drv_lock_create failed, ret is 0x%x\n", ret);

    ret = inner_kapi_symc_restore(symc_ctx);
    crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit,
        "inner_kapi_symc_restore for crypto failed, ret is 0x%x\n", ret);

    crypto_log_dbg("processed_length is %d, data_len is %d\n", symc_ctx->processed_length, symc_ctx->data_length);
    ret = drv_cipher_symc_get_tag(symc_ctx->drv_symc_handle, tag, tag_len);
    crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit, "drv_cipher_symc_get_tag failed\n");

    ret = inner_kapi_symc_get_config(symc_ctx);
    crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit,
        "inner_kapi_symc_get_config for crypto failed, ret is 0x%x\n", ret);

    symc_ctx->processed_length = 0;

symc_destroy_exit:
    priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
    return ret;
}

static td_s32 kapi_cipher_symc_crypto(td_handle kapi_symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length, td_u32 crypto_type)
{
    td_s32 ret;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;

    /* Handle Check. */
    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_set_config == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG),
        "set_config first\n");
    crypto_chk_return(symc_ctx->is_attached == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_ATTACHED), "attach first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = inner_kapi_cipher_symc_crypto(symc_ctx->drv_symc_handle, src_buf, dst_buf, length, crypto_type);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit,
            "inner_kapi_cipher_symc_crypto for Encrypt failed, ret is 0x%x\n", ret);
    } else {
        ret = inner_kapi_symc_crypto_short_term(symc_ctx, src_buf, dst_buf, length, crypto_type);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "inner_kapi_symc_crypto_short_term failed, ret is 0x%x\n", ret);
    }
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);

    symc_ctx->processed_length += length;
    return ret;

unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    return ret;
}

td_s32 kapi_cipher_symc_encrypt(td_handle kapi_symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length)
{
    td_s32 ret;
    crypto_kapi_func_enter();

    /* Param Check. */
    symc_null_ptr_chk(src_buf);
    symc_null_ptr_chk(dst_buf);
    crypto_chk_return(crypto_data_buf_check(src_buf, length) == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_MEMORY_ACCESS),
        "src_buf access refused\n");
    crypto_chk_return(crypto_data_buf_check(dst_buf, length) == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_MEMORY_ACCESS),
        "dst_buf access refused\n");

    ret = kapi_cipher_symc_crypto(kapi_symc_handle, src_buf, dst_buf, length, CRYPTO_TYPE_ENCRYPT);

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_encrypt);

td_s32 kapi_cipher_symc_decrypt(td_handle kapi_symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length)
{
    td_s32 ret;
    crypto_kapi_func_enter();

    /* Param Check. */
    symc_null_ptr_chk(src_buf);
    symc_null_ptr_chk(dst_buf);
    crypto_chk_return(crypto_data_buf_check(src_buf, length) == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_MEMORY_ACCESS),
        "src_buf access refused\n");
    crypto_chk_return(crypto_data_buf_check(dst_buf, length) == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_MEMORY_ACCESS),
        "dst_buf access refused\n");

    ret = kapi_cipher_symc_crypto(kapi_symc_handle, src_buf, dst_buf, length, CRYPTO_TYPE_DECRYPT);

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_decrypt);

typedef struct {
    td_handle drv_symc_handle;
    td_u32 pack_num;
    td_u32 crypto_type;
} priv_multi_param_t;

static td_s32 priv_kapi_symc_crypto_multi(const priv_multi_param_t *multi_param, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack)
{
    td_s32 ret;
    td_u32 i;
    crypto_symc_pack *buffer = TD_NULL;
    crypto_symc_pack *drv_src_buf_pack = TD_NULL;
    crypto_symc_pack *drv_dst_buf_pack = TD_NULL;
    td_u32 pack_size = sizeof(crypto_symc_pack) * multi_param->pack_num;
    td_u32 buffer_size = pack_size * 2; /* 2: for Both src and dst. */

    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    (td_void)memset_s(buffer, buffer_size, 0, buffer_size);

    drv_src_buf_pack = buffer;
    drv_dst_buf_pack = buffer + multi_param->pack_num;

    for (i = 0; i < multi_param->pack_num; i++) {
        /* Check buf_attr access. */
        crypto_chk_goto_with_ret(crypto_data_buf_check(&src_buf_pack[i].buf_attr, src_buf_pack[i].length) == TD_FALSE,
            exit_put_phys_addr, SYMC_COMPAT_ERRNO(ERROR_MEMORY_ACCESS), "src_buf access refuse\n");
        crypto_chk_goto_with_ret(crypto_data_buf_check(&dst_buf_pack[i].buf_attr, dst_buf_pack[i].length) == TD_FALSE,
            exit_put_phys_addr, SYMC_COMPAT_ERRNO(ERROR_MEMORY_ACCESS), "dst_buf access refuse\n");
        /* Transfer phys_addr from buf_attr. */
        drv_src_buf_pack[i].buf_attr.phys_addr = crypto_osal_get_phys_addr(&src_buf_pack[i].buf_attr);
        crypto_chk_goto_with_ret(drv_src_buf_pack[i].buf_attr.phys_addr == 0, exit_put_phys_addr,
            SYMC_COMPAT_ERRNO(ERROR_GET_PHYS_ADDR), "crypto_osal_get_phys_addr failed\n");
        drv_src_buf_pack[i].length = src_buf_pack[i].length;

        drv_dst_buf_pack[i].buf_attr.phys_addr = crypto_osal_get_phys_addr(&dst_buf_pack[i].buf_attr);
        crypto_chk_goto_with_ret(drv_dst_buf_pack[i].buf_attr.phys_addr == 0, exit_put_phys_addr,
            SYMC_COMPAT_ERRNO(ERROR_GET_PHYS_ADDR), "crypto_osal_get_phys_addr failed\n");
        drv_dst_buf_pack[i].length = dst_buf_pack[i].length;
    }

    switch (multi_param->crypto_type) {
        case CRYPTO_TYPE_ENCRYPT:
            ret = drv_cipher_symc_encrypt_multi(multi_param->drv_symc_handle, symc_ctrl, drv_src_buf_pack,
                drv_dst_buf_pack, multi_param->pack_num);
            break;
        case CRYPTO_TYPE_DECRYPT:
            ret = drv_cipher_symc_decrypt_multi(multi_param->drv_symc_handle, symc_ctrl, drv_src_buf_pack,
                drv_dst_buf_pack, multi_param->pack_num);
            break;
        default:
            crypto_log_err("Invalid Crypto Type!\n");
            ret = SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

exit_put_phys_addr:
    for (i = 0; i < multi_param->pack_num; i++) {
        crypto_osal_put_phys_addr(&src_buf_pack[i].buf_attr, drv_src_buf_pack[i].buf_attr.phys_addr);
        crypto_osal_put_phys_addr(&dst_buf_pack[i].buf_attr, drv_dst_buf_pack[i].buf_attr.phys_addr);
    }
    (td_void)memset_s(buffer, buffer_size, 0, buffer_size);
    crypto_free(buffer);
    return ret;
}

td_s32 kapi_cipher_symc_encrypt_multi(td_handle kapi_symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_s32 ret;
    priv_multi_param_t multi_param = {
        .pack_num = pack_num,
        .crypto_type = CRYPTO_TYPE_ENCRYPT
    };
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    symc_null_ptr_chk(src_buf_pack);
    symc_null_ptr_chk(dst_buf_pack);
    crypto_chk_return(pack_num > CRYPTO_SYMC_MULTI_PACK_MAX_SIZE, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
        "pack_num is too large\n");

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "Invalid Process!\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_attached == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_ATTACHED), "attach first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        multi_param.drv_symc_handle = symc_ctx->drv_symc_handle;
        ret = priv_kapi_symc_crypto_multi(&multi_param, symc_ctrl, src_buf_pack, dst_buf_pack);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "priv_kapi_symc_crypto_multi for Encrypt failed\n");
    } else {
        ret = priv_drv_lock_create(symc_ctx, &symc_ctx->symc_attr);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_symc_create failed, ret is 0x%x\n", ret);

        ret = drv_cipher_symc_attach(symc_ctx->drv_symc_handle, symc_ctx->keyslot_handle);
        crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit, "drv_cipher_symc_attach failed, ret is 0x%x\n", ret);

        multi_param.drv_symc_handle = symc_ctx->drv_symc_handle;
        ret = priv_kapi_symc_crypto_multi(&multi_param, symc_ctrl, src_buf_pack, dst_buf_pack);
        crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit, "priv_kapi_symc_crypto_multi for Encrypt failed\n");

        priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
    }

    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;

symc_destroy_exit:
    priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_encrypt_multi);

td_s32 kapi_cipher_symc_decrypt_multi(td_handle kapi_symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_s32 ret;
    priv_multi_param_t multi_param = {
        .pack_num = pack_num,
        .crypto_type = CRYPTO_TYPE_DECRYPT
    };
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    symc_null_ptr_chk(src_buf_pack);
    symc_null_ptr_chk(dst_buf_pack);

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "Invalid Process!\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_attached == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_ATTACHED), "attach first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        multi_param.drv_symc_handle = symc_ctx->drv_symc_handle;
        ret = priv_kapi_symc_crypto_multi(&multi_param, symc_ctrl, src_buf_pack, dst_buf_pack);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "priv_kapi_symc_crypto_multi for Encrypt failed\n");
    } else {
        ret = priv_drv_lock_create(symc_ctx, &symc_ctx->symc_attr);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_symc_create failed, ret is 0x%x\n", ret);

        ret = drv_cipher_symc_attach(symc_ctx->drv_symc_handle, symc_ctx->keyslot_handle);
        crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit, "drv_cipher_symc_attach failed, ret is 0x%x\n", ret);

        multi_param.drv_symc_handle = symc_ctx->drv_symc_handle;
        ret = priv_kapi_symc_crypto_multi(&multi_param, symc_ctrl, src_buf_pack, dst_buf_pack);
        crypto_chk_goto(ret != TD_SUCCESS, symc_destroy_exit, "priv_kapi_symc_crypto_multi for Encrypt failed\n");

        priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
    }

    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;

symc_destroy_exit:
    priv_drv_lock_destroy(symc_ctx->drv_symc_handle);
unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_decrypt_multi);

td_s32 kapi_cipher_symc_get_tag(td_handle kapi_symc_handle, td_u8 *tag, td_u32 tag_length)
{
    td_s32 ret;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    symc_null_ptr_chk(tag);

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_set_config == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG),
        "set_config first\n");
    crypto_chk_return(symc_ctx->is_attached == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_ATTACHED), "attach first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_symc_get_tag(symc_ctx->drv_symc_handle, tag, tag_length);
        crypto_chk_goto(ret != TD_SUCCESS, exit_unlock_mutex, "drv_cipher_symc_get_tag failed\n");
    } else {
        ret = inner_kapi_symc_get_tag_short_term(symc_ctx, tag, tag_length);
        crypto_chk_goto(ret != TD_SUCCESS, exit_unlock_mutex, "inner_kapi_symc_get_tag_short_term failed\n");
    }

exit_unlock_mutex:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_symc_get_tag);

td_s32 kapi_cipher_mac_start(td_handle *kapi_symc_handle, const crypto_symc_mac_attr *mac_attr)
{
    td_s32 ret = CRYPTO_SUCCESS;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;

    crypto_kapi_func_enter();
    symc_null_ptr_chk(kapi_symc_handle);
    symc_null_ptr_chk(mac_attr);

    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    symc_ctx = priv_occupy_symc_soft_chn(symc_channel, &idx);
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_CHN_BUSY), "all symc soft chns are busy\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (mac_attr->is_long_term == TD_TRUE) {
        ret = priv_drv_lock_mac_start(&symc_ctx->drv_symc_handle, mac_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_symc_ctx, "drv_cipher_mac_start failed, ret is 0x%x\n", ret);
    }
    (td_void)memcpy_s(&symc_ctx->mac_attr, sizeof(crypto_symc_mac_attr), mac_attr, sizeof(crypto_symc_mac_attr));
    symc_ctx->is_long_term = mac_attr->is_long_term;
    symc_ctx->is_mac = TD_TRUE;
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    *kapi_symc_handle = synthesize_kapi_handle(KAPI_SYMC_MODULE_ID, idx);
    crypto_kapi_func_exit();
    return ret;

error_unlock_symc_ctx:
    priv_release_symc_soft_chn(symc_ctx);
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_mac_start);

td_s32 kapi_cipher_mac_update(td_handle kapi_symc_handle, const crypto_buf_attr *src_buf, td_u32 length)
{
    td_s32 ret;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    ret =  priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);
    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_CHN_BUSY), "all symc soft chns are busy\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_mac == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_MAC_START), "call mac_start first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_mac_update(symc_ctx->drv_symc_handle, src_buf, length);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_mac_update failed, ret is 0x%x\n", ret);
    } else {
        ret = priv_drv_lock_mac_start(&symc_ctx->drv_symc_handle, &symc_ctx->mac_attr);
        crypto_chk_goto(ret != TD_SUCCESS, unlock_exit, "drv_cipher_mac_start failed, ret is 0x%x\n", ret);

        ret = inner_drv_set_mac_ctx(symc_ctx->drv_symc_handle, &symc_ctx->mac_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, error_mac_finish, "inner_drv_get_mac_ctx failed, ret is 0x%x\n", ret);

        ret = drv_cipher_mac_update(symc_ctx->drv_symc_handle, src_buf, length);
        crypto_chk_goto(ret != TD_SUCCESS, error_mac_finish, "drv_cipher_mac_update failed, ret is 0x%x\n", ret);

        ret = inner_drv_get_mac_ctx(symc_ctx->drv_symc_handle, &symc_ctx->mac_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, error_mac_finish, "inner_drv_get_mac_ctx failed, ret is 0x%x\n", ret);

        (td_void)drv_cipher_symc_destroy(symc_ctx->drv_symc_handle);
    }

    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;

error_mac_finish:
    if (symc_ctx->is_long_term == TD_FALSE) {
        (td_void)drv_cipher_symc_destroy(symc_ctx->drv_symc_handle);
    }
unlock_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_mac_update);

td_s32 kapi_cipher_mac_finish(td_handle kapi_symc_handle, td_u8 *mac, td_u32 *mac_length)
{
    td_s32 ret;
    crypto_kapi_symc_process *symc_channel = TD_NULL;
    crypto_kapi_symc_ctx *symc_ctx = TD_NULL;
    td_u32 idx;
    crypto_kapi_func_enter();

    symc_null_ptr_chk(mac);
    symc_null_ptr_chk(mac_length);

    crypto_chk_return(*mac_length < CRYPTO_AES_BLOCK_SIZE_IN_BYTES, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "mac_length is not enough\n");

    ret = priv_symc_handle_check(kapi_symc_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_symc_handle_check failed, ret is 0x%x\n", ret);

    idx = kapi_get_ctx_idx(kapi_symc_handle);

    symc_channel = priv_get_current_symc_channel();
    crypto_chk_return(symc_channel == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first\n");

    symc_ctx = &symc_channel->symc_ctx_list[idx];
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_CHN_BUSY), "all symc soft chns are busy\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");
    crypto_chk_return(symc_ctx->is_mac == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_NOT_MAC_START), "call mac_start first\n");

    crypto_mutex_lock(&symc_channel->symc_ctx_mutex[idx]);

    if (symc_ctx->is_long_term == TD_TRUE) {
        ret = priv_drv_lock_mac_finish(symc_ctx->drv_symc_handle, mac, mac_length);
        crypto_chk_goto(ret != TD_SUCCESS, error_exit, "priv_drv_lock_mac_finish failed, ret is 0x%x\n", ret);
    } else {
        ret = priv_drv_lock_mac_start(&symc_ctx->drv_symc_handle, &symc_ctx->mac_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_exit, "drv_cipher_mac_start failed, ret is 0x%x\n", ret);
 
        ret = inner_drv_set_mac_ctx(symc_ctx->drv_symc_handle, &symc_ctx->mac_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, error_mac_destroy, "inner_drv_get_mac_ctx failed, ret is 0x%x\n", ret);
 
        (td_void)priv_drv_lock_mac_finish(symc_ctx->drv_symc_handle, mac, mac_length);
    }

    priv_release_symc_soft_chn(symc_ctx);
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;

error_mac_destroy:
    (td_void)drv_cipher_symc_destroy(symc_ctx->drv_symc_handle);
error_exit:
    crypto_mutex_unlock(&symc_channel->symc_ctx_mutex[idx]);
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_mac_finish);
