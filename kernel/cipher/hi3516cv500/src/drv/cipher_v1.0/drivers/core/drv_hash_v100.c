/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv hash v100.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_hash_v100.h"
#include "drv_hash.h"

#ifdef CHIP_HASH_VER_V100

/* ************************** Structure Definition *************************** */
/* hash 256 result size in word */
#define HASH256_RESULT_SIZE_IN_WORD   8

/* Define the rec read bit mask */
#define HASH_READ_MASK_REC            0x08

/* Define the dma read bit mask */
#define HASH_READ_MASK_DMA            0x02

/* Define the hash read bit mask */
#define HASH_READ_MASK_HASH           0x01

/* hash is busy ot not */
#ifdef MHASH_NONSUPPORT
static hi_u32 g_hash_busy = HI_FALSE;
#endif

/* ****************************** API Declaration **************************** */
hi_s32 drv_hash_init(void)
{
    hi_log_func_enter();

    module_disable(CRYPTO_MODULE_ID_HASH);
    module_enable(CRYPTO_MODULE_ID_HASH);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_hash_deinit(void)
{
    hi_log_func_enter();
    hi_log_func_exit();
    return HI_SUCCESS;
}

void drv_hash_resume(void)
{
    hi_log_func_enter();

    module_enable(CRYPTO_MODULE_ID_HASH);

    hi_log_func_exit();
    return;
}

void drv_hash_suspend(void)
{
    hi_log_func_enter();

    module_disable(CRYPTO_MODULE_ID_HASH);

    hi_log_func_exit();
    return;
}

/* wait hash ready */
static hi_s32 drv_hash_wait_ready(hi_u32 bitmask)
{
    hi_u32 i;
    hash_status ready;

    hi_log_func_enter();

    /* wait ready */
    for (i = 0; i < CRYPTO_TIME_OUT; i++) {
        ready.u32 = hash_read(REG_STATUS);
        if (ready.u32 & bitmask) {
            break;
        }
        if (i <= MS_TO_US) {
            crypto_udelay(1);  /* short waitting for 1000 us */
        } else {
            crypto_msleep(1);  /* long waitting for 5000 ms. */
        }
    }
    if (i >= CRYPTO_TIME_OUT) {
        hi_log_error("error, hash wait done timeout\n");
        hi_log_print_err_code(HI_ERR_CIPHER_TIMEOUT);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 drv_hash_cfg_hash_ctrl(hash_mode mode)
{
    hash_ctrl ctrl;

    ctrl.u32 = hash_read(REG_CTRL);

    /* only support sha1 and sha256 */
    if (mode == HASH_MODE_SHA1) {
        ctrl.bits.sha_sel = 0x00; /* SHA1 */
    } else if ((mode == HASH_MODE_SHA256) || (mode == HASH_MODE_SHA224)) {
        ctrl.bits.sha_sel = 0x01; /* SHA256 */
    } else {
        hi_log_error("error, nonsupport hash mode %d\n", mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* control configure */
    ctrl.bits.read_ctrl = 0;            /* 0:read by DMA,   1: read by CPU */
    ctrl.bits.hmac_flag = 0;            /* 0:raw hash,      1: hmac */
    ctrl.bits.hardkey_sel = 0;          /* 0:cpu key,       1: hard key. */
    ctrl.bits.small_end_en = 1;         /* 0:big,           1: little */
    ctrl.bits.sha_init_update_en = 1;   /* initial hash value from CPU */

    hash_write(REG_CTRL, ctrl.u32);
    hi_log_info("REG_CTRL: 0x%x\n", ctrl.u32);
    return HI_SUCCESS;
}

#ifdef MHASH_NONSUPPORT
/*
 * MHASH_NONSUPPORT is a very old version of the logic, only the old version of the chip is supported. It requires the
 * initial length of the HASH calculation to know the total length of the HASH data. In fact, in order to support
 * multi-segment HASH calculations, it is impossible to know the total length of the HASH data at this time.
 * Therefore, the maximum length value of 0xFFFFFFFF is configured in the HASH initialization node, and a HASH soft
 * reset is performed every time until all HASH data and padding data are calculated, and then take out the HASH
 * calculation result, and reset HASH logic again.
 */
static hi_void drv_hash_mhash_non_support_sha_start(hi_void)
{
    sha_start start;

    /* logic don't support config init value of hash
     * here must set it to max vaule
     * then reset hash after compute finished
     */
    hash_write(REG_TOTALLEN_LOW, 0xFFFFFFFF);
    hash_write(REG_TOTALLEN_HIGH, 0x00);

    /* ready to working,
     * the hardware still be idle until write the reg of REG_DMA_LEN
     * so must start the hardware before write DMA addr and length.
     */
    start.u32 = hash_read(REG_START);
    start.bits.sha_start = 0x01;
    hash_write(REG_START, start.u32);
    hi_log_info("REG_START: 0x%x\n", start.u32);
}
#endif

static hi_void drv_hash_cfg_initial_value(hi_u32 *state, hi_u32 state_len_in_word)
{
    hi_u32 i;

    /* write hash initial value */
    for (i = 0; i < state_len_in_word; i++) {
        hash_write(REG_INIT1_UPDATE + i * WORD_WIDTH, crypto_cpu_to_be32(state[i]));
        hi_log_debug("Set hash: 0x%x\n", state[i]);
    }
}

hi_s32 drv_hash_cfg(hi_u32 chn_num, hash_mode mode, hi_u32 state[HASH_RESULT_MAX_SIZE_IN_WORD])
{
    hash_ctrl ctrl;
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_chk_param_return(state == HI_NULL);

    crypto_unused(chn_num);

#ifdef MHASH_NONSUPPORT
    if (g_hash_busy == HI_TRUE) {
        /* the hash already starting, here just return success */
        hi_log_func_exit();
        return HI_SUCCESS;
    }
    g_hash_busy = HI_TRUE;
#endif

    /* wait ready */
    ret = drv_hash_wait_ready(HASH_READ_MASK_HASH);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_hash_wait_ready, ret);
        return ret;
    }

    ret = drv_hash_cfg_hash_ctrl(mode);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_hash_cfg_hash_ctrl, ret);
        return ret;
    }

#ifdef MHASH_NONSUPPORT
    drv_hash_mhash_non_support_sha_start();
    g_hash_busy = HI_TRUE;
#else
    drv_hash_cfg_initial_value(state, HASH_RESULT_MAX_SIZE_IN_WORD);
#endif
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_hash_start(hi_u32 chn_num, crypto_mem *mem, hi_u32 length)
{
    hi_log_func_enter();

    /* buffer addr and size must align with word */
    hi_log_chk_param_return((length & 0x03) != 0);

    hi_log_debug("MMZ phy 0x%x, length 0x%x\n", addr_l32(mem->mmz_addr), length);

    crypto_unused(chn_num);

#ifndef MHASH_NONSUPPORT
    {
        sha_start start;

        /* set hash message total length.
         * in nature here we don't know the total length of message,
         * here just set the total length to buf_size each hash calculation.
         * then we can release the hash hardware after each hash calculation.
         */
        hash_write(REG_TOTALLEN_LOW, length);

        /* max length of msg is 2^32, the high 32bit of length always be zero */
        hash_write(REG_TOTALLEN_HIGH, 0x00);

        /* ready to working,
         * the hardware still be idle until write the reg of REG_DMA_LEN
         * so must start the hardware before write DMA addr and length.
         */
        start.u32 = hash_read(REG_START);
        start.bits.sha_start = 0x01;
        hash_write(REG_START, start.u32);
        hi_log_info("REG_START: 0x%x\n", start.u32);
    }
#endif

    /* DMA address and length, at once write the reg of REG_DMA_LEN,
     * the hardware start working immediately.
     */
    hash_write(REG_DMA_START_ADDR, addr_l32(mem->mmz_addr));
    hash_write(REG_DMA_LEN, length);
    hi_log_info("Hash PHY: 0x%x, size 0x%x\n", addr_l32(mem->mmz_addr), length);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_hash_wait_done(hi_u32 chn_num, hi_u32 *state)
{
    hi_u32 i;
    hi_s32 ret;
    hash_status ready;

    hi_log_func_enter();

    hi_log_chk_param_return(state == HI_NULL);

    crypto_unused(chn_num);

    /* wait ready */
    ret = drv_hash_wait_ready(HASH_READ_MASK_REC);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_hash_wait_ready, ret);
        return ret;
    }

    /* check error code */
    ready.u32 = hash_read(REG_STATUS);
    if (ready.bits.len_err) {
        hi_log_error("error, hash len err, chn=%d", chn_num);
        return HI_ERR_CIPHER_OVERFLOW;
    }
    hi_log_info("Status: 0x%x\n", ready.u32);

    /* read hash result */
    for (i = 0; i < HASH256_RESULT_SIZE_IN_WORD; i++) {
        state[i] = hash_read(REG_SHA_OUT1 + i * WORD_WIDTH);
        hi_log_info("state[%d] 0x%x\n", i, state[i]);
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

void drv_hash_reset(hi_u32 chn_num)
{
    crypto_unused(chn_num);
#ifdef MHASH_NONSUPPORT

    hi_log_func_enter();

    module_disable(CRYPTO_MODULE_ID_HASH);
    module_enable(CRYPTO_MODULE_ID_HASH);
    g_hash_busy = HI_FALSE;

    hi_log_info("Hash reset ...\n");

    hi_log_func_exit();
#endif
}

void drv_hash_get_capacity(hash_capacity *capacity)
{
    crypto_memset(capacity, sizeof(hash_capacity), 0,  sizeof(hash_capacity));

    capacity->sha1   = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha256 = CRYPTO_CAPACITY_SUPPORT;

#ifndef MHASH_NONSUPPORT
    capacity->sha224 = CRYPTO_CAPACITY_SUPPORT;
#endif
}

#endif /* End of CHIP_HASH_VER_V100 */
