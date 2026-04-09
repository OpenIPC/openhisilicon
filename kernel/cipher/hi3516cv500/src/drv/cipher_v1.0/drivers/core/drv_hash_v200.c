/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv hash v200.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_hash_v200.h"
#include "drv_hash.h"

#ifdef CHIP_HASH_VER_V200

/* ************************** Structure Definition *************************** */
/* hash in entry list size */
#define HASH_NODE_SIZE             4096

/* hash in entry list size */
#define HASH_NODE_LIST_SIZE        (HASH_NODE_SIZE * HASH_HARD_CHANNEL_CNT)

/* hash node dpth */
#define HASH_MAX_DEPTH             127

#define KLAD_KEY_USE_ERR           0x01
#define ALG_LEN_ERR                0x02
#define SMMU_PAGE_UNVLID           0x04
#define OUT_SMMU_PAGE_NOT_VALID    0x08
#define KLAD_KEY_WRITE_ERR         0x10

/* Define the flag of node */
typedef enum {
    HASH_CTRL_NONE             = 0x00, /* middle node */
    HASH_CTRL_HASH_IN_FIRST    = 0x01, /* first node */
    HASH_CTRL_HASH_IN_LAST     = 0x02, /* last node */
    HASH_CTRL_COUNT,
} HASH_CTRL_EN;

/* spacc digest in entry struct which is defined by hardware, you can't change it */
struct hash_entry_in {
    hi_u32    spacc_cmd: 2;     /* reserve */
    hi_u32    rev1: 6;          /* reserve */
    hi_u32    hash_ctrl: 6;     /* hash control flag. */
    hi_u32    rev2: 18;         /* reserve */
    hi_u32    hash_start_addr;  /* hash message address */
    hi_u32    hash_alg_length;  /* hash message length */
    hi_u32    word1;            /* reserve */
};

/* Define the context of hash */
typedef struct {
    hash_mode  hash_alg;            /* hash mode */
    struct hash_entry_in *entry_in; /* spacc digest in entry struct */
    hi_u32 id_in;                   /*!< current hash nodes to be used */
    hi_u32 done;                    /* calculation finish flag. */
    crypto_queue_head  queue;       /* quene list */
} hash_hard_context;

/* hash already initialize or not */
static hi_u32 g_hash_initialize = HI_FALSE;

/* dma memory of hash node list. */
static crypto_mem   g_hash_dma;

/* Channel of hash */
static channel_context g_hash_hard_channel[CRYPTO_HARD_CHANNEL_MAX];

/* ****************************** API Declaration **************************** */
static hi_void hash_print_last_node(hi_u32 chn_num)
{
    struct hash_entry_in *in = HI_NULL;
    hash_hard_context *ctx = HI_NULL;

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;

    /* get last in node info. */
    in = &ctx->entry_in[ctx->id_in];

    hi_log_error("chn %d, src addr 0x%x, size 0x%x\n", chn_num, in->hash_start_addr, in->hash_alg_length);
    crypto_unused(in);
}

static hi_s32 drv_hash_get_err_code(hi_u32 chn_num)
{
    hi_u32 code;

    /* check error code
     * bit0: klad_key_use_err
     * bit1: alg_len_err
     * bit2: smmu_page_unvlid
     * bit3: out_smmu_page_not_valid
     * bit4: klad_key_write_err
     */
    code = hash_read(CALC_ERR);
    if (code & KLAD_KEY_USE_ERR) {
        hi_log_error("hash error: klad_key_use_err, chn %d !!!\n", chn_num);
    }
    if (code & ALG_LEN_ERR) {
        hi_log_error("hash error: alg_len_err, chn %d !!!\n", chn_num);
    }
    if (code & SMMU_PAGE_UNVLID) {
        hi_log_error("hash error: smmu_page_unvlid, chn %d !!!\n", chn_num);
    }
    if (code & OUT_SMMU_PAGE_NOT_VALID) {
        hi_log_error("symc error: out_smmu_page_not_valid, chn %d !!!\n", chn_num);
    }
    if (code & KLAD_KEY_WRITE_ERR) {
        hi_log_error("symc error: klad_key_write_err, chn %d !!!\n", chn_num);
    }

    /* print the inout buffer address. */
    if (code) {
        hash_print_last_node(chn_num);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    return HI_SUCCESS;
}

#ifdef CRYPTO_OS_INT_SUPPORT
static hi_u32 hash_done_notify(hi_void)
{
    hash_int_raw    int_raw;
    hash_int_status int_st;
    hi_u32 chn_mask;

    int_st.u32 = hash_read(HASH_INT_STATUS);
    int_raw.u32 = 0;

    /* just process the valid channel. */
    int_st.bits.hash_chn_oram_int &= HASH_HARD_CHANNEL_MASK;
    chn_mask = int_st.bits.hash_chn_oram_int;
    int_raw.bits.hash_chn_oram_raw = int_st.bits.hash_chn_oram_int;

    hi_log_debug("int_st 0x%x, mask 0x%x\n", int_st.u32, chn_mask);

    /* Clean raw int. */
    hash_write(HASH_INT_RAW, int_raw.u32);

    return chn_mask;
}

static hi_u32 symc_done_test(hi_void)
{
    cipher_int_status status;

    status.u32 = symc_read(CIPHER_INT_STATUS);

    /* just process the valid channel. */
    status.bits.cipher_chn_obuf_int &= CIPHER_HARD_CHANNEL_MASK;

    return status.bits.cipher_chn_obuf_int; /* mask */
}

/* hash interrupt process function */
static irqreturn_t hash_interrupt_isr(hi_s32 irq, hi_void *devId)
{
    hi_u32 mask, i;
    hash_hard_context *ctx = HI_NULL;

    crypto_unused(irq);

    mask = hash_done_notify();

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((mask >> i) & 0x01) {
            ctx = (hash_hard_context *)g_hash_hard_channel[i].ctx;
            ctx->done = HI_TRUE;
            hi_log_debug("chn %d wake up\n", i);
            crypto_queue_wait_up(&ctx->queue);
        }
    }

    /* symc and hash use the sample interrupt number
     * so if symc has occur interrupt, we should return IRQ_NONE
     * to tell system continue to process the symc interrupt.
     */
    if (symc_done_test() != 0) {
        return IRQ_NONE;
    }

    return IRQ_HANDLED;
}

/* hash register interrupt process function */
static hi_s32 drv_hash_register_interrupt(hi_void)
{
    hi_s32 ret;
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    hi_u32 i;
    const char *name = HI_NULL;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return HI_SUCCESS;
    }

    /* request irq */
    ret = crypto_request_irq(int_num, hash_interrupt_isr, name);
    if (ret != HI_SUCCESS) {
        hi_log_error("Irq request failure, irq = %d", int_num);
        hi_log_print_err_code(HI_ERR_CIPHER_REGISTER_IRQ);
        return HI_ERR_CIPHER_REGISTER_IRQ;
    }

    /* initialize queue list. */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        crypto_queue_init(&((hash_hard_context *)g_hash_hard_channel[i].ctx)->queue);
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

/* hash unregister interrupt process function */
static hi_void drv_hash_unregister_interrupt(hi_void)
{
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    const char *name = HI_NULL;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return;
    }

    /* free irq */
    hi_log_debug("hash free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    hi_log_func_exit();
}

/* set interrupt */
static hi_void hash_set_interrupt(hi_void)
{
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    const char *name = HI_NULL;
    hash_int_en hash_int_en;
    hash_int_raw hash_int_raw;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        hash_int_en.u32 = hash_read(HASH_INT_EN);

        /* The top interrupt switch only can be enable/disable by secure CPU. */
        hash_int_en.bits.hash_int_en = 0;
        hash_int_en.bits.hash_sec_int_en = 0;
        hash_int_en.bits.hash_chn_oram_en &= ~HASH_HARD_CHANNEL_MASK;
        hash_write(HASH_INT_EN, hash_int_en.u32);
        hi_log_info("HASH_INT_EN: 0x%x\n", hash_int_en.u32);
    } else {
        hash_int_en.u32 = hash_read(HASH_INT_EN);

        /* The top interrupt switch only can be enable/disable by secure CPU. */
        hash_int_en.bits.hash_int_en = 1;
        hash_int_en.bits.hash_sec_int_en = 1;
        hash_int_en.bits.hash_chn_oram_en |= HASH_HARD_CHANNEL_MASK;
        hash_write(HASH_INT_EN, hash_int_en.u32);
        hi_log_info("HASH_INT_EN: 0x%x\n", hash_int_en.u32);
    }

    /* clear interception
     * the history of interception may trigge the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort.
     */
    hash_int_raw.u32 = symc_read(HASH_INT_RAW);
    hash_int_raw.bits.hash_chn_oram_raw &= HASH_HARD_CHANNEL_MASK; /* clear valid channel */
    symc_write(HASH_INT_RAW, hash_int_raw.u32);

    hi_log_func_exit();

    return;
}

static hi_s32 drv_hash_wait_irq(hi_u32 chnnel_id)
{
    hi_s32 ret;
    hi_u32 err_code;
    hash_hard_context *ctx = HI_NULL;

    ctx = (hash_hard_context *)g_hash_hard_channel[chnnel_id].ctx;
    if (ctx == HI_NULL) {
        hi_log_error("hash hard ctx is null.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_TIMEOUT);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    ret = crypto_queue_wait_timeout(ctx->queue, &ctx->done, CRYPTO_TIME_OUT);
    if ((ret <= 0) && (ret != -ERESTARTSYS)) {
        hi_log_print_func_err(crypto_queue_wait_timeout, ret);
        err_code = drv_hash_get_err_code(chnnel_id);
        if (err_code != HI_SUCCESS) {
            hi_log_print_func_err(drv_hash_get_err_code, err_code);
        }
        hi_log_print_err_code(HI_ERR_CIPHER_TIMEOUT);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    return HI_SUCCESS;
}

#endif

static hi_u32 hash_done_try(hi_u32 chn_num)
{
    hash_int_raw int_raw;
    hi_u32 chn_mask;

    int_raw.u32 = hash_read(HASH_INT_RAW);
    int_raw.bits.hash_chn_oram_raw &= 0x01 << chn_num;
    chn_mask = int_raw.bits.hash_chn_oram_raw;

    /* Clean raw int. */
    hash_write(HASH_INT_RAW, int_raw.u32);

    return chn_mask;
}

/* set hash entry */
static hi_void hash_set_entry(hi_u32 chn, compat_addr dma_addr, hi_void *cpu_addr)
{
    hash_hard_context *ctx = (hash_hard_context *)g_hash_hard_channel[chn].ctx;
    chann_hash_int_node_cfg hash_in_cfg;

    /* set total num and start addr for hash in node. */
    hash_in_cfg.u32 = hash_read(chann_hash_in_node_cfg(chn));
    hash_in_cfg.bits.hash_in_node_total_num = HASH_MAX_DEPTH;
    hash_write(chann_hash_in_node_cfg(chn), hash_in_cfg.u32);
    hash_write(chann_hash_in_node_start_addr(chn), addr_l32(dma_addr));
    hash_write(chann_hash_in_node_start_high(chn), addr_h32(dma_addr));
    hi_log_info("chann_hash_in_node_cfg[0x%x]: \t0x%x, PHY: 0x%x, VIA %p\n",
                chann_hash_in_node_cfg(chn), hash_in_cfg.u32, addr_l32(dma_addr), cpu_addr);

    ctx->entry_in = (struct hash_entry_in *)cpu_addr;
    ctx->id_in = hash_in_cfg.bits.hash_in_node_wptr;
}

/* set smmu */
static hi_void hash_smmu_bypass(hi_void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    hash_in_smmu_en hash_in_smmu_en;

    hash_in_smmu_en.u32 = hash_read(HASH_IN_SMMU_EN);
    hash_in_smmu_en.bits.hash_in_chan_rd_dat_smmu_en |= HASH_HARD_CHANNEL_MASK >> 1;
    hash_in_smmu_en.bits.hash_in_chan_rd_node_smmu_en &= ~(HASH_HARD_CHANNEL_MASK >> 1);
    hash_write(HASH_IN_SMMU_EN, hash_in_smmu_en.u32);
    hi_log_info("HASH_IN_SMMU_EN[0x%x]  : 0x%x\n", HASH_IN_SMMU_EN, hash_in_smmu_en.u32);
#endif
}

/* smmu set base address */
static hi_void drv_hash_smmu_base_addr(hi_void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    hi_u64 err_raddr = 0;
    hi_u64 err_waddr = 0;
    hi_u64 table_addr = 0;

    /* get table base addr from system api */
    smmu_get_table_addr(&err_raddr, &err_waddr, &table_addr);

    if (crypto_is_sec_cpu()) {
        /* smmu page secure table addr. */
        hash_write(NORM_SMMU_START_ADDR, (hi_u32)table_addr);
        hi_log_info("NORM_SMMU_START_ADDR[0x%x]  : 0x%x\n", NORM_SMMU_START_ADDR, (hi_u32)table_addr);
    } else {
        /* smmu page nonsecure table addr. */
        hash_write(SEC_SMMU_START_ADDR, (hi_u32)table_addr);
        hi_log_info("SEC_SMMU_START_ADDR[0x%x]  : 0x%x\n", SEC_SMMU_START_ADDR, (hi_u32)table_addr);
    }
#endif
}

/* set secure channel,
 * non-secure CPU can't change the value of SEC_CHN_CFG,
 * so non-secure CPU call this function will do nothing.
 */
static hi_void drv_hash_enable_secure(hi_void)
{
    sec_chn_cfg sec_chn_cfg;

    sec_chn_cfg.u32 = hash_read(SEC_CHN_CFG);
    sec_chn_cfg.bits.hash_sec_chn_cfg |= HASH_HARD_CHANNEL_MASK;
    hash_write(SEC_CHN_CFG, sec_chn_cfg.u32);
    hi_log_info("SEC_CHN_CFG[0x%x]: 0x%x\n", SEC_CHN_CFG, sec_chn_cfg.u32);
}

hi_void hash_enrty_init(crypto_mem mem)
{
    hi_u32 i;
    compat_addr mmz_addr;
    hi_void *cpu_addr = HI_NULL;

    hi_log_info("symc entry list configure\n");
    addr_u64(mmz_addr) = addr_u64(mem.mmz_addr);
    cpu_addr = mem.dma_virt;
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((HASH_HARD_CHANNEL_MASK >> i) & 0x01) { /* valid channel. */
            hash_set_entry(i, mmz_addr, cpu_addr);
            addr_u64(mmz_addr) += HASH_NODE_SIZE;   /* move to next channel */
            cpu_addr = (hi_u8 *)cpu_addr + HASH_NODE_SIZE; /* move to next channel */
        }
    }
    return;
}

static hi_s32 drv_hash_cfg_init(hi_void)
{
    hi_s32 ret;

    hi_log_info("alloc memory for nodes list\n");
    ret = hash_mem_create(&g_hash_dma, SEC_MMZ, "hash_node_list", HASH_NODE_LIST_SIZE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hash_mem_create, ret);
        return ret;
    }
    hi_log_info("HASH DMA buffer, MMU 0x%x, MMZ 0x%x, VIA %p, size 0x%x\n", addr_l32(g_hash_dma.dma_addr),
        addr_l32(g_hash_dma.mmz_addr), g_hash_dma.dma_virt, g_hash_dma.dma_size);

    hi_log_info("hash entry list configure\n");
    hash_enrty_init(g_hash_dma);

    hi_log_info("hash SMMU configure\n");
    hash_smmu_bypass();
    drv_hash_smmu_base_addr();

    hi_log_info("hash secure channel configure\n");
    drv_hash_enable_secure();
#ifdef CRYPTO_OS_INT_SUPPORT
    hi_log_info("hash interrupt configure\n");
    hash_set_interrupt();

    hi_log_info("hash register interrupt function\n");
    ret = drv_hash_register_interrupt();
    if (ret != HI_SUCCESS) {
        hi_s32 ret_error;

        hi_log_print_func_err(drv_hash_register_interrupt, ret);
        ret_error = hash_mem_destory(&g_hash_dma);
        if (ret_error != HI_SUCCESS) {
            hi_log_print_func_err(hash_mem_destory, ret_error);
        }
        return ret;
    }
#endif

    return HI_SUCCESS;
}

hi_s32 drv_hash_init(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    if (g_hash_initialize == HI_TRUE) {
        hi_log_func_exit();
        return HI_SUCCESS;
    }

    ret = crypto_channel_init(g_hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX, sizeof(hash_hard_context));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_channel_init, ret);
        return ret;
    }

    hi_log_info("enable hash\n");
    module_enable(CRYPTO_MODULE_ID_HASH);

    ret = drv_hash_cfg_init();
    if (ret != HI_SUCCESS) {
        hi_s32 ret_error;

        hi_log_print_func_err(crypto_channel_init, ret);
        module_disable(CRYPTO_MODULE_ID_HASH);
        ret_error = crypto_channel_deinit(g_hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX);
        if (ret_error != HI_SUCCESS) {
            hi_log_print_func_err(crypto_channel_deinit, ret_error);
        }
        return ret;
    }

    g_hash_initialize = HI_TRUE;
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_hash_deinit(hi_void)
{
    hi_s32 ret;
    hi_log_func_enter();

    if (g_hash_initialize == HI_FALSE) {
        hi_log_func_exit();
        return HI_SUCCESS;
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_hash_unregister_interrupt();
#endif

    ret = hash_mem_destory(&g_hash_dma);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hash_mem_destory, ret);
        return ret;
    }
    module_disable(CRYPTO_MODULE_ID_HASH);
    ret = crypto_channel_deinit(g_hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_channel_deinit, ret);
        return ret;
    }

    g_hash_initialize = HI_FALSE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_hash_resume(hi_void)
{
    hi_log_func_enter();

    hi_log_info("enable hash\n");
    module_enable(CRYPTO_MODULE_ID_HASH);

    hi_log_info("hash entry list configure\n");
    hash_enrty_init(g_hash_dma);

#ifdef CRYPTO_OS_INT_SUPPORT
    hi_log_info("hash interrupt configure\n");
    hash_set_interrupt();
#endif

    hi_log_info("hash SMMU configure\n");
    hash_smmu_bypass();
    drv_hash_smmu_base_addr();

    hi_log_info("hash secure channel configure\n");
    drv_hash_enable_secure();

    hi_log_func_exit();

    return;
}

hi_void drv_hash_suspend(hi_void)
{
    hi_log_func_enter();
    hi_log_func_exit();

    return;
}

static hi_s32 drv_hash_query_raw_interrupt(hi_u32 chnnel_id)
{
    hi_s32 i;

    /* interrupt unsupport, query the raw interrupt flag. */
    for (i = 0; i < CRYPTO_TIME_OUT; i++) {
        if (hash_done_try(chnnel_id)) {
            break;
        }

        if (i <= MS_TO_US) {
            crypto_udelay(1);  /* short waitting for 1000 us. */
        } else {
            crypto_msleep(1);  /* long waitting for 5000 ms. */
        }
    }

    if (i >= CRYPTO_TIME_OUT) {
        hi_u32 err_code;

        hi_log_error("hash wait done timeout, chn=%d\n", chnnel_id);

        err_code = drv_hash_get_err_code(chnnel_id);
        if (err_code != HI_SUCCESS) {
            hi_log_print_func_err(drv_hash_get_err_code, err_code);
        }

        hi_log_print_err_code(HI_ERR_CIPHER_TIMEOUT);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    return HI_SUCCESS;
}

/* wait hash ready */
static hi_s32 drv_hash_wait_ready(hi_u32 chn_num)
{
    hi_s32 ret;
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, HI_NULL);

#ifdef CRYPTO_OS_INT_SUPPORT
    /* interrupt support, wait irq. */
    if (int_valid) {
        ret = drv_hash_wait_irq(chn_num);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_hash_wait_irq, ret);
            return ret;
        }
    } else {
        ret = drv_hash_query_raw_interrupt(chn_num);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_hash_query_raw_interrupt, ret);
            return ret;
        }
    }
#else
    ret = drv_hash_query_raw_interrupt(chn_num);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_hash_query_raw_interrupt, ret);
        return ret;
    }
#endif

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_void hash_addbuf(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size)
{
    hash_hard_context *ctx = HI_NULL;
    hi_u32 id, size;
    hi_void *addr = HI_NULL;

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;

    /* clean in entry */
    id = ctx->id_in++;
    addr = &ctx->entry_in[id];
    size = sizeof(struct hash_entry_in);

    crypto_memset(addr, size, 0, size);

    /* set addr and length */
    ctx->entry_in[id].spacc_cmd = 0x00;
    ctx->entry_in[id].hash_start_addr = addr_l32(buf_phy);
    ctx->entry_in[id].word1 = addr_h32(buf_phy);
    ctx->entry_in[id].hash_alg_length = buf_size;
    ctx->entry_in[id].hash_ctrl = HASH_CTRL_HASH_IN_FIRST | HASH_CTRL_HASH_IN_LAST;
    ctx->id_in %= HASH_MAX_DEPTH;
    hi_log_info("add digest in buf: id %d, addr 0x%x, len 0x%x\n", id, addr_l32(buf_phy), buf_size);
}

hi_s32 drv_hash_cfg(hi_u32 chn_num, hash_mode mode, hi_u32 state[HASH_RESULT_MAX_SIZE_IN_WORD])
{
    hash_hard_context *ctx = HI_NULL;
    chann_hash_ctrl hash_ctrl;
    hi_u32 i = 0;

    hi_log_chk_param_return(g_hash_initialize != HI_TRUE);
    hi_log_chk_param_return(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;
    ctx->hash_alg = mode;

    /* Control */
    hash_ctrl.u32 = hash_read(chann_hash_ctrl(chn_num));
    hash_ctrl.bits.hash_chn_mode = 0;
    hash_ctrl.bits.hash_chn_agl_sel = mode;
    hash_write(chann_hash_ctrl(chn_num), hash_ctrl.u32);
    hi_log_info("CTRL: 0x%X\n", hash_ctrl.u32);

    /* Write last state. */
    for (i = 0; i < HASH_RESULT_MAX_SIZE_IN_WORD; i++) {
        hash_write(chann_hash_state_val_addr(chn_num), i);
        hash_write(chann_hash_state_val(chn_num), state[i]);
    }
    hi_log_info("state[0]: 0x%x\n", state[0]);

    return HI_SUCCESS;
}

hi_s32 drv_hash_start(hi_u32 chn_num, crypto_mem *mem, hi_u32 length)
{
    chann_hash_int_node_cfg in_node_cfg;
    hash_hard_context *ctx = HI_NULL;
    hi_u32 ptr;
    crypto_mem *hash_dma_ctx = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_hash_initialize != HI_TRUE);
    hi_log_chk_param_return(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;

    if (length == 0) {
        return HI_SUCCESS;
    }

    ctx->done = HI_FALSE;

    /* set message addr and length */
    hash_addbuf(chn_num, mem->dma_addr, length);

    /* configure in-node, only compute one nodes */
    in_node_cfg.u32 = hash_read(chann_hash_in_node_cfg(chn_num));
    ptr = in_node_cfg.bits.hash_in_node_wptr + 1;
    in_node_cfg.bits.hash_in_node_wptr = ptr % HASH_MAX_DEPTH;
    in_node_cfg.bits.hash_in_node_mpackage_int_level = 1;

    /* hash flush cache of hash mem and hash list buffer. */
    crypto_cpuc_flush_dcache_area(mem->dma_virt, length);
    hash_dma_ctx = &g_hash_dma;
    crypto_cpuc_flush_dcache_area(hash_dma_ctx->dma_virt, HASH_NODE_LIST_SIZE);

    /* Start */
    hash_write(chann_hash_in_node_cfg(chn_num), in_node_cfg.u32);
    hi_log_info("chann_hash_in_node_cfg: 0x%x\n", in_node_cfg.u32);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_hash_wait_done(hi_u32 chn_num, hi_u32 *state)
{
    hi_u32 i = 0;
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_chk_param_return(state == HI_NULL);
    hi_log_chk_param_return(g_hash_initialize != HI_TRUE);
    hi_log_chk_param_return(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ret = drv_hash_wait_ready(chn_num);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_hash_wait_ready, ret);
        return ret;
    }

    /* read hash result */
    for (i = 0; i < HASH_RESULT_MAX_SIZE_IN_WORD; i++) {
        hash_write(chann_hash_state_val_addr(chn_num), i);
        state[i] = hash_read(chann_hash_state_val(chn_num));
    }
    hi_log_debug("digest[0]: 0x%x\n", state[0]);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_hash_reset(hi_u32 chn_num)
{
    crypto_unused(chn_num);
}

hi_void drv_hash_get_capacity(hash_capacity *capacity)
{
    crypto_memset(capacity, sizeof(hash_capacity), 0,  sizeof(hash_capacity));

    capacity->sha1   = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha224 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha256 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha384 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha512 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm3    = CRYPTO_CAPACITY_SUPPORT;

    return;
}
#endif /* End of CHIP_HASH_VER_V200 */
