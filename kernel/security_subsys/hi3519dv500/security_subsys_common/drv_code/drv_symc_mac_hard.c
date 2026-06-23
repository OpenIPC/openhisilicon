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

#define CRYPTO_SYMC_CBC_MAC_BUFFER_LEN  (4 * 1024)

td_s32 drv_cipher_mac_start(td_handle *symc_handle, const crypto_symc_mac_attr *mac_attr)
{
    td_s32 ret;
    crypto_symc_attr symc_attr = {0};
    drv_symc_context_t *symc_ctx = TD_NULL;
    td_u32 chn_num;
    hal_symc_config_t symc_config = {0};
    td_u8 iv_zeros[CRYPTO_IV_LEN_IN_BYTES] = {0};
    crypto_drv_func_enter();

    symc_null_ptr_chk(symc_handle);
    symc_null_ptr_chk(mac_attr);

    crypto_chk_return(mac_attr->symc_alg != CRYPTO_SYMC_ALG_AES, SYMC_COMPAT_ERRNO(ERROR_UNSUPPORT),
        "MAC only support AES\n");

    ret = drv_cipher_symc_create(symc_handle, &symc_attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_symc_create failed\n");

    symc_ctx = inner_get_symc_ctx(*symc_handle);
    crypto_chk_goto_with_ret(symc_ctx == TD_NULL, error_symc_destroy,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "symc_handle is invalid\n");
    crypto_chk_goto_with_ret(symc_ctx->is_open == TD_FALSE, error_symc_destroy,
        SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    chn_num = *symc_handle;
    symc_config.symc_alg = mac_attr->symc_alg;
    // For Both CBC_MAC and CMAC, the first work_mdoe is CBC_MAC
    symc_config.work_mode = CRYPTO_SYMC_WORK_MODE_CBC_MAC;
    symc_config.symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT;
    symc_config.symc_key_length = mac_attr->symc_key_length;

    ret = hal_cipher_symc_config(chn_num, &symc_config);
    crypto_chk_goto(ret != TD_SUCCESS, error_symc_destroy, "hal_cipher_symc_config failed\n");

    ret = hal_cipher_symc_attach(chn_num, mac_attr->keyslot_chn);
    crypto_chk_goto(ret != TD_SUCCESS, error_symc_destroy, "hal_cipher_symc_config failed\n");

    ret = hal_cipher_symc_set_iv(chn_num, iv_zeros, sizeof(iv_zeros));
    crypto_chk_goto(ret != TD_SUCCESS, error_symc_destroy, "hal_cipher_symc_set_iv failed\n");

    symc_ctx->work_mode = mac_attr->work_mode;
    symc_ctx->symc_alg = mac_attr->symc_alg;
    symc_ctx->symc_key_length = mac_attr->symc_key_length;

    return ret;
error_symc_destroy:
    drv_cipher_symc_destroy(*symc_handle);
    crypto_drv_func_exit();
    return ret;
}

static td_s32 inner_mac_process(td_u32 chn_num, drv_symc_context_t *symc_ctx,
    td_phys_addr_t data_phys_addr, td_u32 length)
{
    td_s32 ret;

#if defined(CRYPTO_MAC_TRACE_ENABLE)
    crypto_dump_phys_addr("mac_process_data", data_phys_addr, length);
    crypto_dump_data("set_mac_iv", symc_ctx->iv, sizeof(symc_ctx->iv));
#endif
 
    ret = hal_cipher_symc_set_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed, ret is 0x%x\n", ret);

    ret = hal_cipher_symc_add_in_node(chn_num, data_phys_addr, length, IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_add_in_node failed\n");

    ret = hal_cipher_symc_start(chn_num);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_start failed\n");

    ret = hal_cipher_symc_wait_done(chn_num, TD_TRUE);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_done failed\n");

    ret = hal_cipher_symc_get_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_iv failed, ret is 0x%x\n", ret);

#if defined(CRYPTO_MAC_TRACE_ENABLE)
    crypto_dump_data("get_mac_iv", symc_ctx->iv, sizeof(symc_ctx->iv));
#endif
    return ret;
}


static td_s32 inner_mac_update(td_u32 chn_num, drv_symc_context_t *symc_ctx, const crypto_buf_attr *src_buf,
    td_u32 length)
{
    td_s32 ret = CRYPTO_SUCCESS;
    td_u32 processing_length = 0;
    td_u32 processed_length = 0;
    td_u32 left = length;

    /* process one tail. */
    processing_length = sizeof(symc_ctx->tail) - symc_ctx->tail_length;
    if (processing_length != 0) {
        ret = memcpy_s(symc_ctx->tail + symc_ctx->tail_length, processing_length,
            src_buf->virt_addr, processing_length);
        crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    }

    ret = memcpy_s(symc_ctx->dma_addr, symc_ctx->dma_size, symc_ctx->tail, sizeof(symc_ctx->tail));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    ret = inner_mac_process(chn_num, symc_ctx, crypto_get_phys_addr(symc_ctx->dma_addr), sizeof(symc_ctx->tail));
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_mac_process failed\n");

    processed_length += processing_length;
    left -= processing_length;
    symc_ctx->tail_length = 0;

    while (left > sizeof(symc_ctx->tail)) {
        if (left > symc_ctx->dma_size) {
            processing_length = symc_ctx->dma_size;
        } else if (left % CRYPTO_AES_BLOCK_SIZE_IN_BYTES == 0) {
            processing_length = left - CRYPTO_AES_BLOCK_SIZE_IN_BYTES;   // remain the last block.
        } else {
            processing_length = left - left % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
        }
        ret = memcpy_s(symc_ctx->dma_addr, symc_ctx->dma_size,
            (td_u8 *)src_buf->virt_addr + processed_length, processing_length);
        crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

        ret = inner_mac_process(chn_num, symc_ctx, crypto_get_phys_addr(symc_ctx->dma_addr), processing_length);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_mac_process failed, ret is 0x%x\n", ret);

        processed_length += processing_length;
        left -= processing_length;
    }

    symc_ctx->tail_length = left;
    if (left != 0) {
        ret = memcpy_s(symc_ctx->tail, sizeof(symc_ctx->tail), (td_u8 *)src_buf->virt_addr + processed_length, left);
        crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    }
    return CRYPTO_SUCCESS;
}

td_s32 drv_cipher_mac_update(td_handle symc_handle, const crypto_buf_attr *src_buf, td_u32 length)
{
    td_s32 ret;
    td_u32 chn_num;
    drv_symc_context_t *symc_ctx = TD_NULL;

    crypto_drv_func_enter();

    symc_null_ptr_chk(src_buf);
    crypto_chk_return(length == 0 || length > CRYPTO_SYMC_MAC_UPDATE_MAX_LEN,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "length is Invalid\n");

    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    chn_num = symc_handle;
    symc_ctx = inner_get_symc_ctx(symc_handle);
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "symc_handle is invalid\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    if ((symc_ctx->tail_length + length) <= sizeof(symc_ctx->tail)) {
        ret = memcpy_s(symc_ctx->tail + symc_ctx->tail_length, sizeof(symc_ctx->tail) - symc_ctx->tail_length,
            src_buf->virt_addr, length);
        crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
        symc_ctx->tail_length += length;
        crypto_drv_func_exit();
        return TD_SUCCESS;
    }

    ret = inner_mac_update(chn_num, symc_ctx, src_buf, length);
    crypto_chk_return(ret != CRYPTO_SUCCESS, ret, "inner_mac_update failed\n");

    crypto_drv_func_exit();
    return ret;
}

static td_s32 inner_cmac_finish(td_u32 chn_num, drv_symc_context_t *symc_ctx)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *result_mac = TD_NULL;
    td_u8 *tail = symc_ctx->dma_addr;
    hal_symc_config_t symc_config = {0};

    result_mac = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_cmac");
    crypto_chk_return(result_mac == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc_coherent failed\n");

    symc_config.symc_alg = symc_ctx->symc_alg;
    symc_config.work_mode = symc_ctx->work_mode;
    symc_config.symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT;
    symc_config.symc_key_length = symc_ctx->symc_key_length;

    ret = hal_cipher_symc_config(chn_num, &symc_config);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_config failed\n");

    ret = hal_cipher_symc_set_iv(chn_num, symc_ctx->iv, sizeof(symc_ctx->iv));
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_set_iv failed, ret is 0x%x\n", ret);

    ret = memcpy_s(tail, symc_ctx->dma_size, symc_ctx->tail, symc_ctx->tail_length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit_free, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(tail), symc_ctx->tail_length,
        IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

    ret = hal_cipher_symc_add_out_node(chn_num, crypto_get_phys_addr(result_mac), CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

    ret = hal_cipher_symc_start(chn_num);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_start failed\n");

    ret = hal_cipher_symc_wait_done(chn_num, TD_TRUE);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_wait_done failed\n");

exit_free:
    (td_void)memset_s(result_mac, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    crypto_free_coherent(result_mac);
    return ret;
}

td_s32 drv_cipher_mac_finish(td_handle symc_handle, td_u8 *mac, td_u32 *mac_length)
{
    td_s32 ret;
    td_s32 final_ret = TD_SUCCESS;
    drv_symc_context_t *symc_ctx = TD_NULL;
    td_u32 chn_num = 0;

    crypto_drv_func_enter();

    symc_null_ptr_chk(mac);
    symc_null_ptr_chk(mac_length);
    crypto_chk_return(*mac_length < CRYPTO_AES_BLOCK_SIZE_IN_BYTES, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "mac_length is not enough\n");
    ret = inner_symc_drv_handle_chk(symc_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    symc_ctx = inner_get_symc_ctx(symc_handle);
    crypto_chk_return(symc_ctx == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "symc_handle is invalid\n");
    crypto_chk_return(symc_ctx->is_open == TD_FALSE, SYMC_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed\n");

    chn_num = (td_u32)symc_handle;
    /* For CBC_MAC, the tail_length must be 16-Byte Aligned.. */
    if (symc_ctx->work_mode == CRYPTO_SYMC_WORK_MODE_CBC_MAC) {
        crypto_chk_return(symc_ctx->tail_length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES,
            SYMC_COMPAT_ERRNO(ERROR_SYMC_LEN_NOT_ALIGNED), "length must be 16-Byte Aligned for CBC_MAC\n");
        if (symc_ctx->tail_length != 0) {
            ret = memcpy_s(symc_ctx->dma_addr, symc_ctx->dma_size, symc_ctx->tail, symc_ctx->tail_length);
            crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

            ret = inner_mac_process(chn_num, symc_ctx, crypto_get_phys_addr(symc_ctx->dma_addr), symc_ctx->tail_length);
            crypto_chk_return(ret != TD_SUCCESS, ret, "inner_mac_process failed, ret is 0x%x\n", ret);
        }
    } else {    /* For CMAC */
        if (symc_ctx->tail_length == 0) {
            crypto_log_err("the last block size is zero for CMAC!\n");
            return SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        }
        ret = inner_cmac_finish(chn_num, symc_ctx);
        crypto_chk_return(ret != TD_SUCCESS, ret, "inner_cmac_finish failed, ret is 0x%x\n", ret);
    }

    ret = hal_cipher_symc_get_iv(chn_num, mac, *mac_length);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_get_iv failed, ret is 0x%x\n", ret);
        final_ret = ret;
    }
    *mac_length = CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
#if defined(CRYPTO_MAC_TRACE_ENABLE)
    crypto_dump_data("mac_result", mac, *mac_length);
#endif
    (td_void)drv_cipher_symc_destroy(symc_handle);

    crypto_drv_func_exit();
    return final_ret;
}