/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for cipher cryp symc.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "cryp_symc.h"
#include "drv_osal_lib.h"
#include "drv_symc.h"
#include "ext_alg.h"

/* ************************** Internal Structure Definition ****************** */
/* AES set IV for first package */
#define SYMC_IV_USAGE_ONE_PKG           1

/* AES set IV for first package */
#define SYMC_IV_USAGE_ALL_PKG           2

/* SYMC interrupt level (1UL) */
#define SYMC_INT_LEVEL                  (SYMC_MAX_LIST_NUM - 15)

/* Length of SYMC ccm q  */
#define SYMC_CCM_Q_LEN_2B               2
#define SYMC_CCM_Q_LEN_3B               3
#define SYMC_CCM_Q_LEN_4B               4

/* Length of SYMC ccm P  */
#define SYMC_CCM_P_LEN_2B               0xFFFF
#define SYMC_CCM_P_LEN_3B               0xFFFFFF

/* length range of aead */
#define AES_CCM_MIN_IV_LEN              7
#define AES_CCM_MAX_IV_LEN              13
#define AES_CCM_NQ_LEN                  14
#define AES_CCM_MIN_TAG_LEN             4
#define AES_CCM_MAX_TAG_LEN             16
#define AES_GCM_MIN_IV_LEN              1
#define AES_GCM_MAX_IV_LEN              16
#define AES_GCM_MIN_TAG_LEN             1
#define AES_GCM_MAX_TAG_LEN             16
#define AAD_EXIST                       1
#define AAD_NOT_EXIST                   0
#define N_AND_Q_VAL                     15

/* Multi nodes added status, finished or finished */
#define SYMC_NODES_ADD_FINISHED         0x0a0a0a0a
#define SYMC_NODES_ADD_NOTFINISHED      0x05050505

/* The max tab size of symc function */
#define SYMC_FUNC_TAB_SIZE              (SYMC_ALG_COUNT * SYMC_MODE_COUNT)

/* symc function list */
static symc_func g_symc_descriptor[SYMC_FUNC_TAB_SIZE];

/* symc context */
static cryp_symc_context g_symc_context[CRYPTO_HARD_CHANNEL_MAX];

/* symc function register */
static hi_void cryp_register_all_symc(hi_void);

#ifdef CHIP_AES_CCM_GCM_SUPPORT
static hi_u32 cyp_aead_gcm_clen(hi_u8 *buf, hi_u32 buf_len, hi_u32 alen, hi_u32 enclen);
#endif

hi_s32 cryp_symc_init(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    crypto_memset(g_symc_descriptor, sizeof(g_symc_descriptor), 0, sizeof(g_symc_descriptor));

    ret = drv_symc_init();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_init, ret);
        return ret;
    }

    cryp_register_all_symc();

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void cryp_symc_deinit(hi_void)
{
    hi_log_func_enter();

    drv_symc_deinit();

    hi_log_func_exit();
    return;
}

hi_s32 cryp_symc_alloc_chn(hi_u32 *hard_chn)
{
    hi_s32 ret;
    hi_u32 key[SYMC_KEY_MAX_SIZE_IN_WORD] = {0, 1, 2, 3, 4, 5, 6, 7};
    hi_u32 sm1_key[SYMC_SM1_SK_SIZE_IN_WORD] = {0, 1, 2, 3};

    hi_log_func_enter();

    /* allocate a aes hard key channel */
    ret = drv_symc_alloc_chn(hard_chn);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_alloc_chn, ret);
        return ret;
    }

    /* Set a fake key to clear the true key. */
    drv_symc_set_key(*hard_chn, key, sizeof(key), HI_FALSE);
    drv_symc_set_sm1_sk(*hard_chn, sm1_key, sizeof(sm1_key));

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void cryp_symc_free_chn(hi_u32 hard_chn)
{
    hi_log_func_enter();

    drv_symc_free_chn(hard_chn);

    hi_log_func_exit();
}

static hi_void *cryp_symc_create(hi_u32 hard_chn)
{
    cryp_symc_context *hisi_ctx = HI_NULL;

    hi_log_func_enter();

    hisi_ctx = &g_symc_context[hard_chn];
    crypto_memset(hisi_ctx, sizeof(cryp_symc_context), 0, sizeof(cryp_symc_context));
    hisi_ctx->hard_key = HI_FALSE;
    hisi_ctx->hard_chn = hard_chn;

    hi_log_func_exit();
    return hisi_ctx;
}

static hi_void cryp_symc_setkey(hi_void *ctx, const hi_u8 *key, hi_u32 keylen, hi_u32 odd)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    if (hisi_ctx == HI_NULL) {
        hi_log_error("Invalid point.\n");
        return;
    }

    if (odd) {
        crypto_memcpy(hisi_ctx->odd_key, SYMC_KEY_SIZE, key, keylen);
    } else {
        crypto_memcpy(hisi_ctx->even_key, SYMC_KEY_SIZE, key, keylen);
    }
    hisi_ctx->klen = keylen;

    hi_log_func_exit();
    return;
}

static hi_void cryp_symc_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    if (hisi_ctx == HI_NULL) {
        hi_log_error("Invalid point.\n");
        return;
    }

    if (iv == HI_NULL) {
        if (ivlen != 0) {
            hi_log_error("Invalid iv len(%u), iv is null.\n", ivlen);
        }
        return;
    }

    crypto_memcpy(hisi_ctx->iv, AES_IV_SIZE, iv, ivlen);
    hisi_ctx->iv_usage = usage;
    hisi_ctx->ivlen = ivlen;

    hi_log_func_exit();
    return;
}

static hi_void cryp_symc_getiv(hi_void *ctx, hi_u8 *iv, hi_u32 *ivlen)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    if (hisi_ctx == HI_NULL) {
        hi_log_error("Invalid point.\n");
        return;
    }
    crypto_memcpy(iv, AES_IV_SIZE, hisi_ctx->iv, hisi_ctx->ivlen);
    *ivlen = hisi_ctx->ivlen;

    hi_log_func_exit();
    return;
}

static hi_void cryp_symc_setmode(hi_void *ctx, symc_alg alg, symc_mode mode, symc_width width)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    if (hisi_ctx == HI_NULL) {
        hi_log_error("Invalid point.\n");
        return;
    }
    hisi_ctx->mode = mode;
    hisi_ctx->alg = alg;
    hisi_ctx->width = width;

    hi_log_func_exit();
    return;
}

static hi_void cryp_3des2dma_setmode(hi_void *ctx, symc_alg alg, symc_mode mode, symc_width width)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    if (hisi_ctx == HI_NULL) {
        hi_log_error("Invalid point.\n");
        return;
    }

    crypto_unused(alg);
    crypto_unused(mode);
    crypto_unused(width);

    hisi_ctx->mode = SYMC_MODE_ECB;
    hisi_ctx->alg = SYMC_ALG_TDES;
    hisi_ctx->width = SYMC_DAT_WIDTH_64;

    hi_log_func_exit();
    return;
}

static hi_s32 cryp_symc_sm1_setsk(hi_void *ctx, const hi_u8 *key)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();
    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    crypto_memcpy(hisi_ctx->sk, sizeof(hisi_ctx->sk), key, SYMC_SM1_SK_SIZE);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_symc_sm1_setround(hi_void *ctx, hi_u32 round)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();
    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    hisi_ctx->sm1_round = round;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 symc_add_buf(cryp_symc_context *ctx, symc_node_usage out_uasge)
{
    hi_s32 ret;
    hi_u32 cur = ctx->cur_nodes;

    hi_log_func_enter();

    /* Add P in. */
    ret = drv_symc_add_inbuf(ctx->hard_chn,
                             ctx->input_list[cur],
                             ctx->length_list[cur],
                             ctx->usage_list[cur]);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* Add P out, only need the last flag. */
    ret = drv_symc_add_outbuf(ctx->hard_chn,
                              ctx->output_list[cur],
                              ctx->length_list[cur],
                              out_uasge);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_outbuf, ret);
        return ret;
    }

    hi_log_func_exit();

    return ret;
}

static symc_node_usage symc_get_out_usage(symc_mode mode, hi_u32 cur_node, hi_u32 total_node)
{
    symc_node_usage usage = SYMC_NODE_USAGE_NORMAL;

    if ((mode != SYMC_MODE_GCM) && ((cur_node + 1) == total_node)) {
        usage = SYMC_NODE_USAGE_LAST;
    }

    return usage;
}

static hi_s32 symc_add_next_node(cryp_symc_context *ctx, hi_u32 *total, symc_node_usage usage)
{
    hi_s32 ret;
    hi_u32 cur, total_len;

    total_len = *total;
    total_len %= ctx->block_size;
    /* Compute the tail length. */
    if (total_len > 0) {
        total_len = ctx->block_size - total_len;
    }

    /* if the total length don't aligned with block size, split joint the follow nodes. */
    while ((total_len > 0) && (ctx->cur_nodes < ctx->total_nodes)) {
        cur = ctx->cur_nodes;

        /* The next node large than tail size, just split it to 2 nodes. */
        if (ctx->length_list[cur] > total_len) {
            /* Add P in. */
            ret = drv_symc_add_inbuf(ctx->hard_chn, ctx->input_list[cur], total_len, ctx->usage_list[cur]);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(drv_symc_add_inbuf, ret);
                return ret;
            }

            /* Add P out. */
            usage = symc_get_out_usage(ctx->mode, cur, ctx->total_nodes);
            ret = drv_symc_add_outbuf(ctx->hard_chn, ctx->output_list[cur], total_len, usage);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(drv_symc_add_outbuf, ret);
                return ret;
            }

            /* Let next node skip the tail size. */
            addr_u64(ctx->input_list[cur]) += total_len;
            addr_u64(ctx->output_list[cur]) += total_len;
            ctx->length_list[cur] -= total_len;
            total_len = 0;
        } else {
            /* The next node less than tail size, add it to nodes list. */
            usage = symc_get_out_usage(ctx->mode, cur, ctx->total_nodes);
            ret = symc_add_buf(ctx, usage);    /* Add one node. */
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(symc_add_buf, ret);
                return ret;
            }

            /* re-compute the tail size. */
            total_len -= ctx->length_list[cur];

            /* Process next node. */
            ctx->cur_nodes++;
        }
    }

    *total = total_len;
    return HI_SUCCESS;
}

static hi_s32 symc_add_buf_list(hi_void *ctx)
{
    hi_s32 ret;
    hi_u32 i, nodes, cur, total_len;
    cryp_symc_context *hisi_ctx = ctx;
    symc_node_usage usage;

    hi_log_func_enter();

    /* compute finished. */
    if (hisi_ctx->cur_nodes == hisi_ctx->total_nodes) {
        hi_log_func_exit();
        return SYMC_NODES_ADD_FINISHED;
    }

    /* compute not finished.
     * select the minimum numbers of nodes to calculate.
     */
    nodes = crypto_min(SYMC_INT_LEVEL, hisi_ctx->total_nodes - hisi_ctx->cur_nodes);
    total_len = 0;

    for (i = 0; i < nodes; i++) {
        cur = hisi_ctx->cur_nodes;
        usage = symc_get_out_usage(hisi_ctx->mode, cur, hisi_ctx->total_nodes);

        /* Add one node. */
        ret = symc_add_buf(hisi_ctx, usage);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(symc_add_buf, ret);
            return ret;
        }

        total_len += hisi_ctx->length_list[cur];
        hisi_ctx->cur_nodes++;
    }

    /* For each compute, the total length of valid nodes list
     * must aligned with block size, otherwise can't recv interrupt,
     * which limit to hardware devising.
     */
    ret = symc_add_next_node(hisi_ctx, &total_len, usage);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(symc_add_next_node, ret);
        return ret;
    }

#ifdef CHIP_AES_CCM_GCM_SUPPORT
    /* gcm add nodes finished ? */
    if ((hisi_ctx->mode == SYMC_MODE_GCM) && (hisi_ctx->cur_nodes == hisi_ctx->total_nodes)) {
        hi_u8 clen[AES_BLOCK_SIZE];
        compat_addr addr_null;

        /* At the and of GCM, must add a empty node to nodes list,
         * limit to hardware devising
         */
        crypto_memset(&addr_null, sizeof(addr_null), 0, sizeof(addr_null));
        drv_symc_add_outbuf(hisi_ctx->hard_chn, addr_null, 0x00, SYMC_NODE_USAGE_LAST);
        /* Format the length fields of C and add to nodes list. */
        cyp_aead_gcm_clen(clen, sizeof(clen), hisi_ctx->alen, hisi_ctx->enclen);
        drv_aead_gcm_add_clen(hisi_ctx->hard_chn, clen, sizeof(clen));
    }
#endif

    hi_log_func_exit();
    return SYMC_NODES_ADD_NOTFINISHED;
}

static symc_klen cryp_symc_key_type(symc_alg alg, hi_u32 klen)
{
    symc_klen type;

    if ((alg == SYMC_ALG_AES) && (klen == AES_KEY_192BIT)) {
        type = SYMC_KEY_AES_192BIT;
    } else if ((alg == SYMC_ALG_AES) && (klen == AES_KEY_256BIT)) {
        type = SYMC_KEY_AES_256BIT;
    } else if ((alg == SYMC_ALG_TDES) && (klen == TDES_KEY_128BIT)) {
        type = SYMC_KEY_TDES_2KEY;
    } else {
        type = SYMC_KEY_DEFAULT;
    }

    return type;
}

static hi_s32 cryp_symc_config(hi_void *ctx, hi_u32 decrypt)
{
    cryp_symc_context *hisi_ctx = ctx;
    symc_klen type;
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_info("symc configure, chn %u, alg %d, mode %d, dec %u, klen %u, hard %u, iv len %u, iv usage %u\n",
                hisi_ctx->hard_chn, hisi_ctx->alg, hisi_ctx->mode,
                decrypt, hisi_ctx->klen, hisi_ctx->hard_key,
                hisi_ctx->ivlen, hisi_ctx->iv_usage);

    type = cryp_symc_key_type(hisi_ctx->alg, hisi_ctx->klen);

    /* configure */
    ret = drv_symc_cfg(hisi_ctx, decrypt, type);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_cfg, ret);
        return ret;
    }

    if (hisi_ctx->hard_key != HI_TRUE) {
        /* set odd key */
        drv_symc_set_key(hisi_ctx->hard_chn, hisi_ctx->odd_key, sizeof(hisi_ctx->odd_key), HI_TRUE);

        /* set even key */
        drv_symc_set_key(hisi_ctx->hard_chn, hisi_ctx->even_key, sizeof(hisi_ctx->even_key), HI_FALSE);
    }

    if (hisi_ctx->alg == SYMC_ALG_SM1) {
        drv_symc_set_sm1_sk(hisi_ctx->hard_chn, hisi_ctx->sk, sizeof(hisi_ctx->sk));
    }

    /* set iv */
    ret = drv_symc_set_iv(hisi_ctx->hard_chn, hisi_ctx->iv, hisi_ctx->ivlen, hisi_ctx->iv_usage);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_set_iv, ret);
        return ret;
    }

    /* first node must set iv except ecb mode. */
    if (hisi_ctx->iv_usage == HI_CIPHER_IV_CHG_ONE_PACK) {
        /* don't set iv any more. */
        hisi_ctx->iv_usage = 0;
    }

    if (hisi_ctx->alg == SYMC_ALG_DES) {
        hisi_ctx->block_size = DES_BLOCK_SIZE;
    } else if (hisi_ctx->alg == SYMC_ALG_TDES) {
        hisi_ctx->block_size = DES_BLOCK_SIZE;
    } else {
        hisi_ctx->block_size = AES_BLOCK_SIZE;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_symc_wait_done(hi_void *ctx, hi_u32 timeout)
{
    hi_s32 ret;
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    /* wait done */
    ret = drv_symc_wait_done(hisi_ctx->hard_chn, timeout);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_wait_done, ret);
        return ret;
    }

    drv_symc_get_iv(hisi_ctx->hard_chn, hisi_ctx->iv);

    if ((hisi_ctx->mode == SYMC_MODE_CCM) || (hisi_ctx->mode == SYMC_MODE_GCM)) {
        ret = drv_aead_get_tag(hisi_ctx->hard_chn, hisi_ctx->tag, sizeof(hisi_ctx->tag));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_aead_get_tag, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_symc_crypto_init(cryp_symc_context *hisi_ctx, hi_u32 operation, symc_multi_pack *pack,
    symc_node_usage usage)
{
    hi_u32 i = 0;
    hi_s32 ret;

    hi_log_func_enter();
    hi_log_chk_param_return(pack == HI_NULL);

    /* length of pkage can't be zero */
    hisi_ctx->enclen = 0;
    if (pack->num == 0x01) {
        hisi_ctx->enclen += pack->len[i];
        pack->usage[i] = (hi_u32)pack->usage[i] | (hi_u32)usage;
    } else {
        for (i = 0; i < pack->num; i++) {
            if (pack->len[i] == 0x00) {
                hi_log_print_err_code(HI_ERR_CIPHER_INVALID_LEN);
                return HI_ERR_CIPHER_INVALID_LEN;
            }

            hi_log_chk_param_return(hisi_ctx->enclen + pack->len[i] < hisi_ctx->enclen);

            hisi_ctx->enclen += pack->len[i];
            pack->usage[i] = (hi_u32)pack->usage[i] | (hi_u32)usage;
        }
    }

    /* configuration parameter */
    ret = cryp_symc_config(hisi_ctx, operation);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_config, ret);
        return ret;
    }

    hisi_ctx->input_list = pack->in;
    hisi_ctx->output_list = pack->out;
    hisi_ctx->length_list = pack->len;
    hisi_ctx->usage_list = pack->usage;
    hisi_ctx->total_nodes = pack->num;
    hisi_ctx->cur_nodes = 0;

    /* set isr callback function */
    ret = drv_symc_set_isr_callback(hisi_ctx->hard_chn, HI_NULL, HI_NULL);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_set_isr_callback, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_symc_crypto_process(cryp_symc_context *hisi_ctx, hi_u32 wait)
{
    hi_s32 ret;

    hi_log_func_enter();

    if (wait == HI_TRUE) {
        while (symc_add_buf_list(hisi_ctx) == SYMC_NODES_ADD_NOTFINISHED) {
            /* start running */
            drv_symc_start(hisi_ctx->hard_chn);

            /* wait done */
            ret = drv_symc_wait_done(hisi_ctx->hard_chn, CRYPTO_TIME_OUT);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(drv_symc_wait_done, ret);
                return ret;
            }
        }
    } else {
        /* add buf list once */
        ret = symc_add_buf_list(hisi_ctx);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(symc_add_buf_list, ret);
            return ret;
        }

        /* set isr callback function */
        ret = drv_symc_set_isr_callback(hisi_ctx->hard_chn, symc_add_buf_list, hisi_ctx);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_set_isr_callback, ret);
            return ret;
        }

        /* start running */
        ret = drv_symc_start(hisi_ctx->hard_chn);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_start, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_symc_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 wait)
{
    hi_s32 ret;
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();
    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    if (hisi_ctx->alg == SYMC_ALG_NULL_CIPHER) {
        /* set last flag for each node when DMA copy */
        hisi_ctx->iv_usage = HI_CIPHER_IV_CHG_ALL_PACK;
    }

    ret = cryp_symc_crypto_init(hisi_ctx, operation, pack, SYMC_NODE_USAGE_NORMAL);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_crypto_init, ret);
        return ret;
    }
    pack->usage[pack->num - 1] = (hi_u32)pack->usage[pack->num - 1] | SYMC_NODE_USAGE_LAST;

    /* tdes used as dma */
    if (hisi_ctx->tdes2dma == HI_TRUE) {
        if ((pack->num != 0x01) && (pack->len[0] < DES_BLOCK_SIZE)) {
            hi_log_error("Invalid 3des dma for pkg num (0x%x) or data lenth (0x%x).\n", pack->num, pack->len[0]);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        /* check the length of nodes list */
        ret = drv_symc_node_check(hisi_ctx->alg, hisi_ctx->mode, hisi_ctx->klen, hisi_ctx->block_size, pack);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_node_check, ret);
            return ret;
        }
    }

    ret = cryp_symc_crypto_process(hisi_ctx, wait);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_crypto_process, ret);
        return ret;
    }

    drv_symc_get_iv(hisi_ctx->hard_chn, hisi_ctx->iv);

    hi_log_func_exit();
    return HI_SUCCESS;
}

#ifdef CHIP_AES_CCM_GCM_SUPPORT
static hi_s32 cryp_aead_ccm_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    hi_u8 ccm_iv[AES_IV_SIZE] = {0};

    /* The octet lengths of N are denoted  n,
     * The octet length of the binary represen tation of the
     * octet length of the payload denoted q,
     * n is an element of {7, 8, 9, 10, 11, 12, 13}
     * descript: n + q = 15
     * here the string of N  is pConfig->iv, and n is pConfig->ivLen.
     */
    if ((ivlen < AES_CCM_MIN_IV_LEN) || (ivlen > AES_CCM_MAX_IV_LEN)) {
        hi_log_error("Invalid ccm iv len, ivlen = 0x%x.\n", ivlen);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* Formatting of the Counter Blocks(IV for CTR)
     *
     * According to the CCM spec, the counter is equivalent to
     * a formatting of the counter index i into a complete data block.
     * The counter blocks Ctri are formatted as shown below:
     * | Octet number:  0    1 ... 15-q    16-q ... 15
     * | Contents:     Flags     N             [i]
     * Within each block Ctri, the N is get from pConfig->iv, n + q = 15,
     * so the q equal to 15 - pConfig->ivLen.
     * the [i] is the block conut start with 0,
     * In the Flags field, Bits 0, 1, and 2 contain the encoding of q - 1,
     * others bits shall be set to 0.
     * so the first byte of IV shall be q -1, that is 15 - pConfig->ivLen - 1
     */
    ccm_iv[0] = AES_CCM_NQ_LEN - ivlen; /* descript: IV[0] = q - 1 = 15 - n -1. */
    crypto_memcpy(&ccm_iv[1], sizeof(ccm_iv) - 1, iv, ivlen);
    ivlen += 1;

    cryp_symc_setiv(ctx, ccm_iv, ivlen, usage);

    return HI_SUCCESS;
}

static hi_s32 cryp_aead_gcm_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    if ((ivlen < AES_GCM_MIN_IV_LEN) || (ivlen > AES_GCM_MAX_IV_LEN)) {
        hi_log_error("Invalid gcm iv len, ivlen = 0x%x.\n", ivlen);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    cryp_symc_setiv(ctx, iv, ivlen, usage);

    return HI_SUCCESS;
}

static hi_s32 cyp_aead_ccm_n(cryp_symc_context *ctx, hi_u8 *buf, hi_u32 buf_len)
{
    hi_u32 idx = 0;
    hi_u32 q;
    hi_u8 *iv = (hi_u8 *)ctx->iv;

    hi_log_func_enter();

    /* Format B0
     * The leading octet of the first block of the formatting, B0,
     * contains four flags for control information: two single bits,
     * called  Reserved  and  Adata, and two strings of three bits,
     * to encode the values t and q.  The encoding of t is [(t -2)/2],
     * and the encoding of q is [ q-1].
     * The ordering of the flags with in the octet is given:
     *  _____________________________________________________
     * |Bit number  7     |   6   | 5  4  3     |  2  1  0   |
     * |Contents  Reserved  Adata   [(t -2)/2] |  [q-1]     |
     *  -----------------------------------------------------
     * The remaining 15 octets of the first block of  the formatting are
     * devoted to the nonce and the binary representation of
     * the message length in  q octets, as given:
     *  _____________________________________________
     * |Octet number  0   | 1 ... 15-q | 16-q ... 15 |
     * |Contents    Flags |      N     |      Q      |
     *  ---------------------------------------------
     */
    crypto_memset(buf, buf_len, 0, buf_len);
    buf[idx]  = (ctx->alen > 0 ? AAD_EXIST : AAD_NOT_EXIST) << SHIFT_6BITS;  /* descript: Adata exists or not. */
    buf[idx] |= ((ctx->tlen - CRYPTO_NUM_2) / MUL_VAL_2) << SHIFT_3BITS;     /* descript: (t -2) / 2 */
    buf[idx] |= (N_AND_Q_VAL - ctx->ivlen);                                  /* descript: q-1, n+q=15 */
    idx++;

    /* copy N, skip Flags in byte0. */
    crypto_memcpy(&buf[idx], AES_BLOCK_SIZE - idx, &iv[1], ctx->ivlen - 1);
    idx += ctx->ivlen - 1;

    q = AES_BLOCK_SIZE - idx;

    if (q >= SYMC_CCM_Q_LEN_4B) {
        /* max payload len of 2^32, jump to the location of last word */
        idx = AES_BLOCK_SIZE - SYMC_CCM_Q_LEN_4B;

        buf[idx++] = (hi_u8)(ctx->enclen >> SHIFT_24BITS);
        buf[idx++] = (hi_u8)(ctx->enclen >> SHIFT_16BITS);
        buf[idx++] = (hi_u8)(ctx->enclen >> SHIFT_8BITS);
        buf[idx++] = (hi_u8)(ctx->enclen);
    } else if ((q == SYMC_CCM_Q_LEN_3B) && (ctx->enclen <= SYMC_CCM_P_LEN_3B)) {
        /* max payload len of 2^24. */
        buf[idx++] = (hi_u8)(ctx->enclen >> SHIFT_16BITS);
        buf[idx++] = (hi_u8)(ctx->enclen >> SHIFT_8BITS);
        buf[idx++] = (hi_u8)(ctx->enclen);
    } else if ((q == SYMC_CCM_Q_LEN_2B) && (ctx->enclen <= SYMC_CCM_P_LEN_2B)) {
        /* max payload len of 2^16. */
        buf[idx++] = (hi_u8)(ctx->enclen >> SHIFT_8BITS);
        buf[idx++] = (hi_u8)(ctx->enclen);
    } else {
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_u32 cyp_aead_gcm_clen(hi_u8 *buf, hi_u32 buf_len, hi_u32 alen, hi_u32 enclen)
{
    hi_u32 idx = 0;

    hi_log_func_enter();
    crypto_unused(buf_len);

    /* Format len(C), 16 byets, coding in bits.
     * Byet0~7:  bits number of Add
     * Byet8~15: bits number of P
     */
    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = (hi_u8)((alen >> SHIFT_29BITS) & MAX_LOW_3BITS);
    buf[idx++] = (hi_u8)((alen >> SHIFT_21BITS) & MAX_LOW_8BITS);
    buf[idx++] = (hi_u8)((alen >> SHIFT_13BITS) & MAX_LOW_8BITS);
    buf[idx++] = (hi_u8)((alen >> SHIFT_5BITS) & MAX_LOW_8BITS);
    buf[idx++] = (hi_u8)((alen << SHIFT_3BITS) & MAX_LOW_8BITS);

    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = (hi_u8)((enclen >> SHIFT_29BITS) & MAX_LOW_3BITS);
    buf[idx++] = (hi_u8)((enclen >> SHIFT_21BITS) & MAX_LOW_8BITS);
    buf[idx++] = (hi_u8)((enclen >> SHIFT_13BITS) & MAX_LOW_8BITS);
    buf[idx++] = (hi_u8)((enclen >> SHIFT_5BITS) & MAX_LOW_8BITS);
    buf[idx++] = (hi_u8)((enclen << SHIFT_3BITS) & MAX_LOW_8BITS);

    return idx;
}

static hi_s32 cryp_aead_ccm_set_aad(hi_void *ctx, compat_addr aad, hi_u32 alen, hi_u32 tlen)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    /* the parameter t denotes the octet length of T(tag)
     * t is an element of  { 4, 6, 8, 10, 12, 14, 16}
     * here t is pConfig->u32TagLen
     */
    if ((tlen & 0x01) || (tlen < AES_CCM_MIN_TAG_LEN) || (tlen > AES_CCM_MAX_TAG_LEN)) {
        hi_log_error("Invalid tag len, tlen = 0x%x.\n", tlen);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    hisi_ctx->aad = aad;
    hisi_ctx->alen = alen;
    hisi_ctx->tlen = tlen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_aead_gcm_set_aad(hi_void *ctx, compat_addr aad, hi_u32 alen, hi_u32 tlen)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    if ((tlen < AES_GCM_MIN_TAG_LEN)
        || (tlen > AES_GCM_MAX_TAG_LEN)) {
        hi_log_error("Invalid tag len, tlen = 0x%x.\n", tlen);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    hisi_ctx->aad = aad;
    hisi_ctx->alen = alen;
    hisi_ctx->tlen = tlen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_aead_crypto_zero(cryp_symc_context *hisi_ctx, hi_u32 wait)
{
    hi_s32 ret;

    hi_log_func_enter();

    /* start running */
    ret = drv_symc_start(hisi_ctx->hard_chn);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_start, ret);
        return ret;
    }

    /* wait done */
    if (wait == HI_TRUE) {
        ret = drv_symc_wait_done(hisi_ctx->hard_chn, CRYPTO_TIME_OUT);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_wait_done, ret);
            return ret;
        }

        ret = drv_aead_get_tag(hisi_ctx->hard_chn, hisi_ctx->tag, sizeof(hisi_ctx->tag));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_aead_get_tag, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_ccm_add_p(cryp_symc_context *hisi_ctx, symc_multi_pack *pack, hi_u32 wait)
{
    hi_s32 ret;

    if (hisi_ctx->enclen == 0) {
        compat_addr addr_null;
        crypto_memset(&addr_null, sizeof(addr_null), 0, sizeof(addr_null));

        /* Set CCM last flag. */
        drv_symc_add_buf_usage(hisi_ctx->hard_chn, HI_TRUE, SYMC_NODE_USAGE_CCM_LAST);

        /* If P is HI_NULL, must add a empty node into node list, limit to hardware devising */
        ret = drv_symc_add_outbuf(hisi_ctx->hard_chn, addr_null, 0x00, SYMC_NODE_USAGE_LAST);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_add_outbuf, ret);
            return ret;
        }

        ret = cryp_aead_crypto_zero(hisi_ctx, wait);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_aead_crypto_zero, ret);
            return ret;
        }
    } else {
        /* for CCM, must set P last and ccm last flag */
        pack->usage[pack->num - 1] = (hi_u32)pack->usage[pack->num - 1] |
            (SYMC_NODE_USAGE_CCM_LAST | SYMC_NODE_USAGE_LAST);
        ret = cryp_symc_crypto_process(hisi_ctx, wait);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_symc_crypto_process, ret);
            return ret;
        }

        ret = drv_aead_get_tag(hisi_ctx->hard_chn, hisi_ctx->tag, sizeof(hisi_ctx->tag));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_aead_get_tag, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 cryp_aead_ccm_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 wait)
{
    hi_s32 ret;
    hi_u8 n[AES_BLOCK_SIZE] = {0};
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    ret = cryp_symc_crypto_init(hisi_ctx, operation, pack, SYMC_NODE_USAGE_IN_CCM_P);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_crypto_init, ret);
        return ret;
    }

    /*
     * NIST Special Publication 800-38C
     * The data that CCM protects consists of a message, i.e., a bit string,
     * called the payload, denoted P, of bit length denoted Plen,
     * and a bit string, called the associated data, denoted A.
     * The associated data is optional, i.e., A may be the empty string.
     * CCM provides assurance of the confidentiality of P and assurance of
     * the authenticity of the origin of both A and P;
     * confidentiality is not provided for A.
     * Compute N.
     */
    ret  = cyp_aead_ccm_n(hisi_ctx, n, sizeof(n));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cyp_aead_ccm_n, ret);
        return ret;
    }

    ret = drv_aead_ccm_add_n(hisi_ctx->hard_chn, n, sizeof(n));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_aead_ccm_add_n, ret);
        return ret;
    }

    /* Compute A */
    ret = drv_aead_ccm_add_a(hisi_ctx->hard_chn, hisi_ctx->aad, hisi_ctx->alen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_aead_ccm_add_a, ret);
        return ret;
    }

    /* Compute A */
    ret = cryp_ccm_add_p(hisi_ctx, pack, wait);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_ccm_add_p, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_aead_gcm_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 wait)
{
    hi_s32 ret;
    cryp_symc_context *hisi_ctx = ctx;
    hi_u8 clen[AES_BLOCK_SIZE] = {0};

    hi_log_func_enter();

    ret = cryp_symc_crypto_init(hisi_ctx, operation, pack, SYMC_NODE_USAGE_IN_GCM_P);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_crypto_init, ret);
        return ret;
    }

    /*
     * NIST Special Publication 800-38D
     * A || P || Clen.
     * Compute A.
     */
    ret = drv_aead_gcm_add_a(hisi_ctx->hard_chn, hisi_ctx->aad, hisi_ctx->alen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_aead_gcm_add_a, ret);
        return ret;
    }

    if (hisi_ctx->enclen == 0) {
        compat_addr addr_null;
        crypto_memset(&addr_null, sizeof(addr_null), 0, sizeof(addr_null));
        /*
         * At the and of GCM, must add a empty node to nodes list,
         * limit to hardware devising.
         */
        ret = drv_symc_add_outbuf(hisi_ctx->hard_chn, addr_null, 0x00, SYMC_NODE_USAGE_LAST);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_add_outbuf, ret);
            return ret;
        }

        /* Format the length fields of C and add to nodes list. */
        cyp_aead_gcm_clen(clen, sizeof(clen), hisi_ctx->alen, 0x00);
        ret = drv_aead_gcm_add_clen(hisi_ctx->hard_chn, clen, sizeof(clen));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_aead_gcm_add_clen, ret);
            return ret;
        }

        ret = cryp_aead_crypto_zero(hisi_ctx, wait);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_aead_crypto_zero, ret);
            return ret;
        }
    } else {
        /* for GCM, must set P last and gcm last flag. */
        pack->usage[pack->num - BOUND_VAL_1] = (hi_u32)pack->usage[pack->num - BOUND_VAL_1] | SYMC_NODE_USAGE_LAST;
        ret = cryp_symc_crypto_process(hisi_ctx, wait);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_symc_crypto_process, ret);
            return ret;
        }

        ret = drv_aead_get_tag(hisi_ctx->hard_chn, hisi_ctx->tag, sizeof(hisi_ctx->tag));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_aead_get_tag, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_aead_get_tag(hi_void *ctx, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(*taglen < hisi_ctx->tlen);

    hi_log_debug("tag buffer len %d, tag len %d\n", *taglen,  hisi_ctx->tlen);

    *taglen = hisi_ctx->tlen;

    crypto_memcpy(tag, AEAD_TAG_SIZE, hisi_ctx->tag, hisi_ctx->tlen);

    hi_log_func_exit();
    return HI_SUCCESS;
}
#endif

static hi_s32 cryp_register_symc(symc_func *func)
{
    hi_u32 i = 0;

    hi_log_func_enter();

    /* check availability */
    if ((func->create == HI_NULL) || (func->crypto == HI_NULL)) {
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* is it already registered? */
    for (i = 0; i < SYMC_FUNC_TAB_SIZE; i++) {
        if ((g_symc_descriptor[i].valid) && (g_symc_descriptor[i].alg == func->alg) &&
            (g_symc_descriptor[i].mode == func->mode)) {
            hi_log_func_exit();
            return HI_SUCCESS;
        }
    }

    /* find a blank spot */
    for (i = 0; i < SYMC_FUNC_TAB_SIZE; i++) {
        if (g_symc_descriptor[i].valid == 0) {
            crypto_memcpy(&g_symc_descriptor[i], sizeof(symc_func), func, sizeof(symc_func));
            g_symc_descriptor[i].valid = HI_TRUE;
            hi_log_debug("g_symc_descriptor[%d], alg %d, mode %d\n", i,
                g_symc_descriptor[i].alg, g_symc_descriptor[i].mode);

            hi_log_func_exit();
            return HI_SUCCESS;
        }
    }

    /* Can't find a blank spot */
    hi_log_print_err_code(HI_ERR_CIPHER_OVERFLOW);
    return HI_ERR_CIPHER_OVERFLOW;
}

static symc_func *cryp_get_symc(hi_u32 alg, hi_u32 mode)
{
    hi_u32 i = 0;
    symc_func *template = HI_NULL;

    hi_log_func_enter();

    /* find the valid function */
    for (i = 0; i < SYMC_FUNC_TAB_SIZE; i++) {
        hi_log_debug("g_symc_descriptor[%d] valid %d, alg %d, mode %d \n",
                     i, g_symc_descriptor[i].valid, g_symc_descriptor[i].alg, g_symc_descriptor[i].mode);

        if (g_symc_descriptor[i].valid) {
            if (g_symc_descriptor[i].alg == alg
                && g_symc_descriptor[i].mode == mode) {
                template = &g_symc_descriptor[i];
                break;
            }
        }
    }

    hi_log_func_exit();
    return template;
}

/*
 * Defined the default template of Symmetric cipher function,
 * the function can be replaced by other engine
 */
static hi_s32 cryp_aes_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_u32 klen;
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    /* set the key length depend on alg
     * des/3des support 2key and 3key
     * aes support 128, 192, and 256
     * sm1 support ak/ek/sk
     * sm4 support 128
     */
    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    hisi_ctx->tdes2dma = HI_FALSE;

    switch (*hisi_klen) {
        case HI_CIPHER_KEY_AES_128BIT: {
            klen = AES_KEY_128BIT;
            break;
        }
        case HI_CIPHER_KEY_AES_192BIT: {
            klen = AES_KEY_192BIT;
            break;
        }
        case HI_CIPHER_KEY_AES_256BIT: {
            klen = AES_KEY_256BIT;
            break;
        }
        default: {
            hi_log_error("Invalid aes key len: 0x%x\n", *hisi_klen);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    }
    hi_log_info("key len %d, type %d\n", klen, hisi_klen);

    *hisi_klen = klen;

    if (fkey == HI_NULL) {
        hisi_ctx->hard_key = HI_TRUE;
        hisi_ctx->klen = klen;
        hi_log_func_exit();
        return HI_SUCCESS;
    }

    cryp_symc_setkey(ctx, fkey, klen, HI_FALSE);

    if (skey != HI_NULL) {
        cryp_symc_setkey(ctx, skey, klen, HI_TRUE);
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_tdes_chk_cpu_key(const hi_u8 *fkey, const hi_u32 *hisi_klen, hi_u32 *invalid, hi_u32 *klen)
{
    /* check k1 != k2. */
    if (memcmp(&fkey[0], &fkey[DES_BLOCK_SIZE], DES_BLOCK_SIZE) == 0) {
        *invalid = HI_TRUE;
    }

    switch (*hisi_klen) {
        case HI_CIPHER_KEY_DES_2KEY: {
            *klen = TDES_KEY_128BIT;
            break;
        }
        case HI_CIPHER_KEY_DES_3KEY: {
            *klen = TDES_KEY_192BIT;

            /* check k2 != k3. */
            if (memcmp(&fkey[DES_BLOCK_SIZE], &fkey[DES_BLOCK_SIZE * MUL_VAL_2], DES_BLOCK_SIZE) == 0) {
                *invalid = HI_TRUE;
            }
            break;
        }
        default: {
            hi_log_error("3des with invalid keylen, keylen = 0x%x.\n", *hisi_klen);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 cryp_tdes_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_s32 ret;
    hi_u32 klen = 0;
    cryp_symc_context *hisi_ctx = ctx;
    symc_capacity capacity;
    hi_u32 invalid = HI_FALSE;

    hi_log_func_enter();

    /* set the key length depend on alg
     * des/3des support 2key and 3key
     * aes support 128, 192, and 256
     * sm1 support ak/ek/sk
     * sm4 support 128
     */
    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    crypto_unused(skey);

    hisi_ctx->tdes2dma = HI_FALSE;

    if (fkey == HI_NULL) {
        /* 3des use hard key. */
        if (*hisi_klen != HI_CIPHER_KEY_DES_2KEY) {
            hi_log_error("error, tdes hard key must be 2key.\n");
            hi_log_print_err_code(HI_ERR_CIPHER_ILLEGAL_KEY);
            return HI_ERR_CIPHER_ILLEGAL_KEY;
        }

        hisi_ctx->hard_key = HI_TRUE;
        hisi_ctx->klen = TDES_KEY_128BIT;
        *hisi_klen = TDES_KEY_128BIT;

        hi_log_func_exit();
        return HI_SUCCESS;
    }

    /* get symc capacity */
    drv_symc_get_capacity(&capacity);
    ret = cryp_tdes_chk_cpu_key(fkey, (const hi_u32 *)hisi_klen, &invalid, &klen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_sm1_setsk, ret);
        return ret;
    }

    if (invalid == HI_TRUE) {
        if (capacity.dma == CRYPTO_CAPACITY_SUPPORT) {
            hi_log_error("3des with invalid key.\n");
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_ILLEGAL_KEY;
        }

        /* if don't support dma, the tdes with invalid key can be used as dma. */
        hisi_ctx->tdes2dma = HI_TRUE;
    }

    cryp_symc_setkey(ctx, fkey, klen, HI_FALSE);

    *hisi_klen = klen;
    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_des_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    crypto_unused(skey);

    hisi_ctx->tdes2dma = HI_FALSE;

    if (fkey == HI_NULL) {
        hi_log_error("error, des nonsupport hard key.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_ILLEGAL_KEY);
        return HI_ERR_CIPHER_ILLEGAL_KEY;
    }

    cryp_symc_setkey(ctx, fkey, DES_KEY_SIZE, HI_FALSE);

    *hisi_klen = DES_KEY_SIZE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_3des2dma_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_u8 key[TDES_KEY_128BIT] = {0};
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();
    hi_log_chk_param_return(hisi_ctx == HI_NULL);

    crypto_unused(fkey);
    crypto_unused(skey);

    cryp_symc_setkey(ctx, key, TDES_KEY_128BIT, HI_FALSE);

    *hisi_klen = TDES_KEY_128BIT;
    hisi_ctx->tdes2dma = HI_TRUE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_sm1_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_s32 ret;
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();
    hi_log_chk_param_return(hisi_klen == HI_NULL);
    hi_log_chk_param_return(*hisi_klen == HI_CIPHER_KEY_DEFAULT);
    hi_log_chk_param_return(hisi_ctx == HI_NULL);
    crypto_unused(skey);

    hisi_ctx->tdes2dma = HI_FALSE;

    if (fkey == HI_NULL) {
        hisi_ctx->hard_key = HI_TRUE;
        hi_log_func_exit();
        return HI_SUCCESS;
    }

    cryp_symc_setkey(ctx, fkey, SM1_AK_EK_SIZE, HI_FALSE);

    /* sm1 support ak/ek/sk */
    ret = cryp_symc_sm1_setsk(ctx, skey);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_sm1_setsk, ret);
        return ret;
    }

    *hisi_klen = SM1_AK_EK_SIZE;
    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_sm4_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    cryp_symc_context *hisi_ctx = ctx;

    hi_log_func_enter();
    hi_log_chk_param_return(hisi_klen == HI_NULL);
    hi_log_chk_param_return(*hisi_klen != HI_CIPHER_KEY_DEFAULT);
    hi_log_chk_param_return(hisi_ctx == HI_NULL);
    crypto_unused(skey);

    hisi_ctx->tdes2dma = HI_FALSE;

    if (fkey == HI_NULL) {
        hisi_ctx->hard_key = HI_TRUE;

        hi_log_func_exit();
        return HI_SUCCESS;
    }

    /*  sm4 support 128 */
    cryp_symc_setkey(ctx, fkey, SM4_KEY_SIZE, HI_FALSE);

    *hisi_klen = SM4_KEY_SIZE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 cryp_symc_setiv_default(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    hi_log_func_enter();

    if (iv == HI_NULL) {
        return HI_SUCCESS;
    }

    if (ivlen > AES_IV_SIZE) {
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    cryp_symc_setiv(ctx, iv, ivlen, usage);

    hi_log_func_exit();
    return HI_SUCCESS;
}

/* Default As AES */
static hi_void cryp_register_symc_default(symc_func *func, symc_alg alg, symc_mode mode)
{
    crypto_memset(func, sizeof(symc_func), 0, sizeof(symc_func));

    func->mode = mode;
    func->alg = alg;
    func->create = cryp_symc_create;
    func->setiv = cryp_symc_setiv_default;
    func->getiv = cryp_symc_getiv;
    func->crypto = cryp_symc_crypto;
    func->setmode = cryp_symc_setmode;
    func->setkey = cryp_aes_setkey;
    func->waitdone = cryp_symc_wait_done;
    return;
}

static hi_void cryp_register_symc_aes(hi_u32 capacity, symc_mode mode)
{
    symc_func func;
    hi_s32 ret;

    crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#ifdef SOFT_AES_SUPPORT
    } else {
        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        func.create = ext_mbedtls_symc_create;
        func.destroy = ext_mbedtls_symc_destory;
        func.setiv = ext_mbedtls_symc_setiv;
        func.getiv = ext_mbedtls_symc_getiv;
        func.setkey = ext_mbedtls_symc_setkey;
        func.setmode = ext_mbedtls_symc_setmode;
        func.crypto = ext_mbedtls_symc_crypto;
        func.waitdone = HI_NULL;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    }
    return;
}

static hi_void cryp_register_symc_dma(hi_u32 dma_capacity, hi_u32 tdes_capacity)
{
    symc_func func;
    hi_s32 ret;

    crypto_memset(&func, sizeof(func), 0, sizeof(func));

    if (dma_capacity == CRYPTO_CAPACITY_SUPPORT) {
        func.mode = SYMC_MODE_ECB;
        func.alg = SYMC_ALG_NULL_CIPHER;
        func.create = cryp_symc_create;
        func.setmode = cryp_symc_setmode;
        func.crypto = cryp_symc_crypto;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
    } else if (tdes_capacity == CRYPTO_CAPACITY_SUPPORT) {
        func.mode = SYMC_MODE_ECB;
        func.alg = SYMC_ALG_NULL_CIPHER;
        func.create = cryp_symc_create;
        func.setmode = cryp_3des2dma_setmode;
        func.setkey = cryp_3des2dma_setkey;
        func.crypto = cryp_symc_crypto;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
    }
    return;
}

static hi_void cryp_register_symc_aes_cts(hi_u32 capacity, symc_mode mode)
{
#ifdef SOFT_AES_CTS_SUPPORT
    hi_s32 ret;

    hi_log_debug("CTS crypto capacity %d, mode %d\n", capacity, mode);

    if (capacity != CRYPTO_CAPACITY_SUPPORT) {
        symc_func func;

        crypto_memset(&func, sizeof(func), 0, sizeof(func));

        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        func.crypto = cryp_aes_cbc_cts_crypto;
        func.waitdone = HI_NULL;
        hi_log_debug("CTS crypto 0x%p, mode %d\n", func.crypto, mode);
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
    }
#endif
    return;
}

static hi_void cryp_register_aead_ccm(hi_u32 capacity, symc_mode mode)
{
    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
#ifdef CHIP_AES_CCM_GCM_SUPPORT
        symc_func func;
        hi_s32 ret;

        crypto_memset(&func, sizeof(func), 0, sizeof(func));

        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        func.setadd = cryp_aead_ccm_set_aad;
        func.gettag = cryp_aead_get_tag;
        func.crypto = cryp_aead_ccm_crypto;
        func.setiv = cryp_aead_ccm_setiv;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    } else {
#ifdef SOFT_AES_CCM_GCM_SUPPORT
        symc_func func;
        hi_s32 ret;

        crypto_memset(&func, sizeof(func), 0, sizeof(func));

        func.mode = mode;
        func.alg = SYMC_ALG_AES;
        func.create = ext_mbedtls_aead_create;
        func.destroy = ext_mbedtls_aead_destory;
        func.setiv = ext_mbedtls_aead_setiv;
        func.getiv = HI_NULL;
        func.crypto = ext_mbedtls_aead_ccm_crypto;
        func.setmode = HI_NULL;
        func.setkey = ext_mbedtls_aead_setkey;
        func.setadd = ext_mbedtls_aead_set_aad;
        func.gettag = ext_mbedtls_aead_get_tag;
        func.waitdone = HI_NULL;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    }

    return;
}

static hi_void cryp_register_aead_gcm(hi_u32 capacity, symc_mode mode)
{
    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
#ifdef  CHIP_AES_CCM_GCM_SUPPORT
        symc_func func;
        hi_s32 ret;

        crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        func.setadd = cryp_aead_gcm_set_aad;
        func.gettag = cryp_aead_get_tag;
        func.crypto = cryp_aead_gcm_crypto;
        func.setiv = cryp_aead_gcm_setiv;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    } else {
#ifdef SOFT_AES_CCM_GCM_SUPPORT
        symc_func func;
        hi_s32 ret;

        crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

        func.mode = mode;
        func.alg = SYMC_ALG_AES;
        func.create = ext_mbedtls_aead_create;
        func.destroy = ext_mbedtls_aead_destory;
        func.setiv = ext_mbedtls_aead_setiv;
        func.getiv = HI_NULL;
        func.crypto = ext_mbedtls_aead_gcm_crypto;
        func.setmode = HI_NULL;
        func.setkey = ext_mbedtls_aead_setkey;
        func.setadd = ext_mbedtls_aead_set_aad;
        func.gettag = ext_mbedtls_aead_get_tag;
        func.waitdone = HI_NULL;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    }

    return;
}

static hi_void cryp_register_symc_tdes(hi_u32 capacity, symc_mode mode)
{
    symc_func func;
    hi_s32 ret;

    crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        cryp_register_symc_default(&func, SYMC_ALG_TDES, mode);
        func.setkey = cryp_tdes_setkey;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#ifdef SOFT_TDES_SUPPORT
    } else {
        cryp_register_symc_default(&func, SYMC_ALG_TDES, mode);
        func.create = ext_mbedtls_symc_create;
        func.destroy = ext_mbedtls_symc_destory;
        func.setiv = ext_mbedtls_symc_setiv;
        func.getiv = ext_mbedtls_symc_getiv;
        func.setkey = ext_mbedtls_symc_setkey;
        func.setmode = ext_mbedtls_symc_setmode;
        func.crypto = ext_mbedtls_symc_crypto;
        func.waitdone = HI_NULL;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    }
    return;
}

static hi_void cryp_register_symc_des(hi_u32 capacity, symc_mode mode)
{
    symc_func func;
    hi_s32 ret;

    crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        cryp_register_symc_default(&func, SYMC_ALG_DES, mode);
        func.setkey = cryp_des_setkey;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#ifdef SOFT_TDES_SUPPORT
    } else {
        cryp_register_symc_default(&func, SYMC_ALG_DES, mode);
        func.create = ext_mbedtls_symc_create;
        func.destroy = ext_mbedtls_symc_destory;
        func.setiv = ext_mbedtls_symc_setiv;
        func.getiv = ext_mbedtls_symc_getiv;
        func.setkey = ext_mbedtls_symc_setkey;
        func.setmode = ext_mbedtls_symc_setmode;
        func.crypto = ext_mbedtls_symc_crypto;
        func.waitdone = HI_NULL;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    }
    return;
}

static hi_void cryp_register_symc_sm1(hi_u32 capacity, symc_mode mode)
{
    symc_func func;
    hi_s32 ret;

    crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        cryp_register_symc_default(&func, SYMC_ALG_SM1, mode);
        func.setround = cryp_symc_sm1_setround;
        func.setkey = cryp_sm1_setkey;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
    }
    return;
}

static hi_void cryp_register_symc_sm4(hi_u32 capacity, symc_mode mode)
{
    symc_func func;
    hi_s32 ret;

    crypto_memset(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        cryp_register_symc_default(&func, SYMC_ALG_SM4, mode);
        func.setkey = cryp_sm4_setkey;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#ifdef SOFT_SM4_SUPPORT
    } else {
        cryp_register_symc_default(&func, SYMC_ALG_SM4, mode);
        func.create = ext_sm4_create;
        func.destroy = ext_sm4_destory;
        func.setiv = ext_sm4_setiv;
        func.getiv = ext_sm4_getiv;
        func.setkey = ext_sm4_setkey;
        func.setmode = ext_sm4_setmode;
        func.crypto = ext_sm4_crypto;
        ret = cryp_register_symc(&func);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_register_symc, ret);
            return;
        }
#endif
    }
    return;
}

/* symc function register */
static hi_void cryp_register_all_symc(hi_void)
{
    symc_capacity capacity;

    hi_log_func_enter();

    crypto_memset(&capacity, sizeof(capacity), 0, sizeof(capacity));

    /* get symc capacity */
    drv_symc_get_capacity(&capacity);

    /*
     * register the symc function if supported.
     * AES.
     */
    cryp_register_symc_aes(capacity.aes_ecb, SYMC_MODE_ECB);
    cryp_register_symc_aes(capacity.aes_cbc, SYMC_MODE_CBC);
    cryp_register_symc_aes(capacity.aes_cfb, SYMC_MODE_CFB);
    cryp_register_symc_aes(capacity.aes_ofb, SYMC_MODE_OFB);
    cryp_register_symc_aes(capacity.aes_ctr, SYMC_MODE_CTR);
    cryp_register_symc_dma(capacity.dma, capacity.tdes_ecb);
    cryp_register_symc_aes_cts(capacity.aes_cts, SYMC_MODE_CTS);

    /* AEAD */
    cryp_register_aead_ccm(capacity.aes_ccm, SYMC_MODE_CCM);
    cryp_register_aead_gcm(capacity.aes_gcm, SYMC_MODE_GCM);

    /* TDES */
    cryp_register_symc_tdes(capacity.tdes_ecb, SYMC_MODE_ECB);
    cryp_register_symc_tdes(capacity.tdes_cbc, SYMC_MODE_CBC);
    cryp_register_symc_tdes(capacity.tdes_cfb, SYMC_MODE_CFB);
    cryp_register_symc_tdes(capacity.tdes_ofb, SYMC_MODE_OFB);
    cryp_register_symc_tdes(capacity.tdes_ctr, SYMC_MODE_CTR);

    /* DES */
    cryp_register_symc_des(capacity.des_ecb, SYMC_MODE_ECB);
    cryp_register_symc_des(capacity.des_cbc, SYMC_MODE_CBC);
    cryp_register_symc_des(capacity.des_cfb, SYMC_MODE_CFB);
    cryp_register_symc_des(capacity.des_ofb, SYMC_MODE_OFB);
    cryp_register_symc_des(capacity.des_ctr, SYMC_MODE_CTR);

    /* SM1 */
    cryp_register_symc_sm1(capacity.sm1_ecb, SYMC_MODE_ECB);
    cryp_register_symc_sm1(capacity.sm1_cbc, SYMC_MODE_CBC);
    cryp_register_symc_sm1(capacity.sm1_cfb, SYMC_MODE_CFB);
    cryp_register_symc_sm1(capacity.sm1_ofb, SYMC_MODE_OFB);
    cryp_register_symc_sm1(capacity.sm1_ctr, SYMC_MODE_CTR);

    /* SM4 */
    cryp_register_symc_sm4(capacity.sm4_ecb, SYMC_MODE_ECB);
    cryp_register_symc_sm4(capacity.sm4_cbc, SYMC_MODE_CBC);
    cryp_register_symc_sm4(capacity.sm4_cfb, SYMC_MODE_CFB);
    cryp_register_symc_sm4(capacity.sm4_ofb, SYMC_MODE_OFB);
    cryp_register_symc_sm4(capacity.sm4_ctr, SYMC_MODE_CTR);

    hi_log_func_exit();

    return;
}

static hi_s32 cryp_get_symc_alg(hi_cipher_alg alg, symc_alg *cryp_alg)
{
    symc_alg local_alg;

    switch (alg) {
        case HI_CIPHER_ALG_DES:
            local_alg = SYMC_ALG_DES;
            break;
        case HI_CIPHER_ALG_3DES:
            local_alg = SYMC_ALG_TDES;
            break;
        case HI_CIPHER_ALG_AES:
            local_alg = SYMC_ALG_AES;
            break;
        case HI_CIPHER_ALG_SM1:
            local_alg = SYMC_ALG_SM1;
            break;
        case HI_CIPHER_ALG_SM4:
            local_alg = SYMC_ALG_SM4;
            break;
        case HI_CIPHER_ALG_DMA:
            local_alg = SYMC_ALG_NULL_CIPHER;
            break;
        default:
            hi_log_error("Invalid alg, alg = 0x%x.\n", alg);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    *cryp_alg = local_alg;
    return HI_SUCCESS;
}

static hi_s32 cryp_get_symc_mode(hi_cipher_work_mode mode, symc_mode *cryp_mode)
{
    symc_mode local_mode;
    switch (mode) {
        case HI_CIPHER_WORK_MODE_ECB:
            local_mode = SYMC_MODE_ECB;
            break;
        case HI_CIPHER_WORK_MODE_CBC:
            local_mode = SYMC_MODE_CBC;
            break;
        case HI_CIPHER_WORK_MODE_CFB:
            local_mode = SYMC_MODE_CFB;
            break;
        case HI_CIPHER_WORK_MODE_OFB:
            local_mode = SYMC_MODE_OFB;
            break;
        case HI_CIPHER_WORK_MODE_CTR:
            local_mode = SYMC_MODE_CTR;
            break;
        case HI_CIPHER_WORK_MODE_CCM:
            local_mode = SYMC_MODE_CCM;
            break;
        case HI_CIPHER_WORK_MODE_GCM:
            local_mode = SYMC_MODE_GCM;
            break;
        default:
            hi_log_error("Invalid mode, mode = 0x%x.\n", mode);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    *cryp_mode = local_mode;
    return HI_SUCCESS;
}

symc_func *cryp_get_symc_op(hi_cipher_alg alg, hi_cipher_work_mode mode)
{
    hi_s32 ret;
    symc_mode cryp_mode = 0;
    symc_func *func = HI_NULL;
    symc_alg cryp_alg = 0;
    hi_cipher_work_mode local_mode;

    hi_log_func_enter();

    ret = cryp_get_symc_alg(alg, &cryp_alg);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_get_symc_alg, ret);
        return HI_NULL;
    }

    if (alg == HI_CIPHER_ALG_DMA) {
        local_mode = HI_CIPHER_WORK_MODE_ECB;
    } else {
        local_mode = mode;
    }

    ret = cryp_get_symc_mode(local_mode, &cryp_mode);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_get_symc_mode, ret);
        return HI_NULL;
    }

    func = cryp_get_symc(cryp_alg, cryp_mode);
    if (func == HI_NULL) {
        hi_log_print_func_err(cryp_get_symc, HI_ERR_CIPHER_INVALID_POINT);
        return HI_NULL;
    }

    hi_log_func_exit();
    return func;
}
