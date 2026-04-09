/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv symc v200.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_symc_v200.h"
#include "drv_symc.h"

#ifdef CHIP_SYMC_VER_V200

#define KLAD_KEY_USE_ERR                0x01
#define ALG_LEN_ERR                     0x02
#define SMMU_PAGE_UNVLID                0x04
#define OUT_SMMU_PAGE_NOT_VALID         0x08
#define KLAD_KEY_WRITE_ERR              0x10

/* Length of pading buffer */
#define SYMC_PAD_BUFFER_LEN             128

/* Length of aes ccm/gcm key */
#define AES_CCM_GCM_KEY_LEN             16

/* Total Length of pading buffer */
#define SYMC_PAD_BUFFER_TOTAL_LEN       (SYMC_PAD_BUFFER_LEN * CIPHER_HARD_CHANNEL_CNT)

/* spacc symc int entry struct which is defined by hardware, you can't change it */
typedef struct {
    hi_u32     spacc_cmd: 2;            /* reserve */
    hi_u32     rev1: 6;                 /* reserve */
    hi_u32     sym_ctrl: 7;             /* symc control flag. */
    hi_u32     rev2: 1;                 /* reserve */
    hi_u32     gcm_iv_len: 4;           /* gcm iv length */
    hi_u32     rev3: 12;                /* reserve */
    hi_u32     sym_start_high;          /* syma start high addr */
    hi_u32     sym_start_addr;          /* syma start low addr */
    hi_u32     sym_alg_length;          /* syma data length */
    hi_u32     iv[AES_IV_SIZE / WORD_WIDTH];    /* symc IV */
} symc_entry_in;

/* spacc symc out entry struct which is defined by hardware, you can't change it */
typedef struct {
    hi_u32    rev1: 8;                  /* reserve */
    hi_u32    aes_ctrl: 4;              /* aes contrl */
    hi_u32    rev2: 20;                 /* reserve */
    hi_u32    sym_start_addr;           /* syma start high addr */
    hi_u32    sym_alg_length;           /* syma data length */
    hi_u32    hash_rslt_start_addr;     /* syma data length */
    hi_u32    tag[AEAD_TAG_SIZE_IN_WORD];  /* CCM/GCM tag */
} symc_entry_out;

/* Define the context of cipher */
typedef struct {
    hi_u32 open;                        /* open or close */
    symc_entry_in  *entry_in;           /* in node list */
    symc_entry_out *entry_out;          /* out node list */
    compat_addr    dma_entry;           /* dma addr of node */
    compat_addr    dma_pad;             /* dma addr of padding buffer, for CCM/GCM */
    hi_u8          *via_pad;            /* via addr of padding buffer, for CCM/GCM */
    hi_u32         offset_pad;          /* offset of padding buffer, for CCM/GCM */
    hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD];    /* iv data from user. */

    /* iv usage flag, should be HI_CIPHER_IV_CHG_ONE_PACK
     * or HI_CIPHER_IV_CHG_ALL_PACK.
     */
    hi_u32 iv_flag;                     /* iv flag */
    hi_u32 iv_len;                      /* iv length */
    symc_alg alg;                       /* The alg of Symmetric cipher */
    symc_mode mode;                     /* mode */
    hi_u32 id_in;                       /* current in nodes index */
    hi_u32 id_out;                      /* current out nodes index */
    hi_u32 cnt_in;                      /* total count in nodes to be computed */
    hi_u32 cnt_out;                     /* total count out nodes to be computed */
    hi_u32 done;                        /* calculation finish flag. */
    crypto_queue_head  queue;           /* quene list */
    callback_symc_isr callback;         /* isr callback functon */
    callback_symc_destory destory;      /* destory callback functon */
    hi_void *ctx;                       /* params for isr callback functon */
} symc_hard_context;

/* spacc symc_chn_who_used struct which is defined by hardware, you can't change it */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    non_sec_chn_who_used : 16   ; /* [15..0]  */
        hi_u32    sec_chn1_who_used     : 2   ; /* [17..16] */
        hi_u32    reserved              : 22  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    hi_u32        u32;
} symc_chn_who_used;

/* Channel of cipher */
static symc_hard_context g_hard_context[CRYPTO_HARD_CHANNEL_MAX];

/* dma memory of cipher node list. */
static crypto_mem   g_symc_dma;

/* symc already initialize or not */
static hi_u32 g_symc_initialize = HI_FALSE;

/* ****************************** API Code **************************** */
static hi_u32 drv_symc_done_try(hi_u32 chn_num)
{
    cipher_int_raw status;

    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    status.u32 = symc_read(CIPHER_INT_RAW);
    status.bits.cipher_chn_obuf_raw &= 0x01 << chn_num; /* check interception */

    /* clear interception. */
    symc_write(CIPHER_INT_RAW, status.u32);

    return (hi_u32)(status.bits.cipher_chn_obuf_raw ? HI_TRUE : HI_FALSE);
}

#ifdef CRYPTO_OS_INT_SUPPORT
static hi_u32 drv_symc_done_notify(hi_void)
{
    cipher_int_status status;
    cipher_int_raw    raw;

    status.u32 = symc_read(CIPHER_INT_STATUS);
    raw.u32 = 0;

    /* just process the valid channel. */
    status.bits.cipher_chn_obuf_int &= CIPHER_HARD_CHANNEL_MASK;
    raw.bits.cipher_chn_obuf_raw = status.bits.cipher_chn_obuf_int;

    /* clear interception. */
    symc_write(CIPHER_INT_RAW, raw.u32);

    return status.bits.cipher_chn_obuf_int; /* mask */
}

static hi_u32 drv_hash_done_test(hi_void)
{
    hash_int_status int_st;
    hi_u32 chn_mask;

    int_st.u32 = symc_read(HASH_INT_STATUS);

    /* just process the valid channel. */
    int_st.bits.hash_chn_oram_int &= HASH_HARD_CHANNEL_MASK;
    chn_mask = int_st.bits.hash_chn_oram_int;

    return chn_mask;
}

/* symc interrupt process function */
static irqreturn_t drv_symc_interrupt_isr(hi_s32 irq, hi_void *dev_id)
{
    hi_u32 mask, i;
    symc_hard_context *ctx = HI_NULL;
    irqreturn_t ret = IRQ_HANDLED;

    /* get channel context. */
    mask = drv_symc_done_notify();
    hi_log_debug("symc irq: %d, mask 0x%x\n", irq, mask);

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((mask >> i) & 0x01) {
            ctx = &g_hard_context[i];
            if ((ctx->callback) && (ctx->callback(ctx->ctx) == HI_FALSE)) {
                /* contiue to compute */
                hi_log_debug("contiue to compute chn %d\n", i);
                drv_symc_start(i);
            } else {
                /* finish */
                ctx->done = HI_TRUE;
                hi_log_debug("chn %d wake up\n", i);
                crypto_queue_wait_up(&ctx->queue);
            }
        }
    }

    /* symc and hash use the sample interrupt number
     * so if hash has occur interrupt, we should return IRQ_NONE
     * to tell system continue to process the hash interrupt.
     */
    if (drv_hash_done_test() != 0) {
        ret = IRQ_NONE;
    }

    return ret;
}

/* symc register interrupt process function */
static hi_s32 drv_symc_register_interrupt(hi_void)
{
    hi_s32 ret;
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    hi_u32 i;
    const char *name = HI_NULL;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return HI_SUCCESS;
    }

    /* request irq */
    hi_log_debug("symc request irq, num %u, name %s\n", int_num, name);
    ret = crypto_request_irq(int_num, drv_symc_interrupt_isr, name);
    if (ret != HI_SUCCESS) {
        hi_log_error("Irq request failure, irq = %u\n", int_num);
        hi_log_print_err_code(HI_ERR_CIPHER_REGISTER_IRQ);
        return ret;
    }

    /* initialize queue list. */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        crypto_queue_init(&g_hard_context[i].queue);
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

/* symc unregister interrupt process function */
static hi_void drv_symc_unregister_interrupt(hi_void)
{
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    const char *name = HI_NULL;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return;
    }

    /* free irq */
    hi_log_debug("symc free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    hi_log_func_exit();
}

/* set interrupt */
static hi_void drv_symc_set_interrupt(hi_void)
{
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    cipher_int_en cipher_int_en;
    cipher_int_raw raw;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, HI_NULL);

    if (int_valid) {
        /* Enable interrupt */
        cipher_int_en.u32 = symc_read(CIPHER_INT_EN);

        if (crypto_is_sec_cpu()) {
            cipher_int_en.bits.cipher_sec_int_en = 1;
        } else {
            cipher_int_en.bits.cipher_nsec_int_en = 1;
        }
        cipher_int_en.bits.cipher_chn_obuf_en |= CIPHER_HARD_CHANNEL_MASK;

        symc_write(CIPHER_INT_EN, cipher_int_en.u32);
        hi_log_info("CIPHER_INT_EN: 0x%x\n", cipher_int_en.u32);
    } else {
        /* Disable interrupt */
        cipher_int_en.u32 = symc_read(CIPHER_INT_EN);

        if (crypto_is_sec_cpu()) {
            cipher_int_en.bits.cipher_sec_int_en = 0;
        } else {
            cipher_int_en.bits.cipher_nsec_int_en = 0;
        }
        symc_write(CIPHER_INT_EN, cipher_int_en.u32);
        hi_log_info("CIPHER_INT_EN: 0x%x\n", cipher_int_en.u32);
    }

    /* clear interception
     * the history of interception may trigge the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort.
     */
    raw.u32 = symc_read(CIPHER_INT_RAW);
    raw.bits.cipher_chn_obuf_raw &= CIPHER_HARD_CHANNEL_MASK; /* clear valid channel */
    symc_write(CIPHER_INT_RAW, raw.u32);

    hi_log_func_exit();

    return;
}

static hi_s32 drv_symc_wait_irq(hi_u32 chn_num, symc_hard_context *ctx, hi_u32 timeout)
{
    hi_s32 ret;

    /* wait interrupt */
    ret = crypto_queue_wait_timeout(ctx->queue, &ctx->done, timeout);

    /* Disable SM4 independent from spacc */
    if (ctx->alg == SYMC_ALG_SM4) {
        module_disable(CRYPTO_MODULE_ID_SM4);
    }

    if ((ret <= 0x00) && (ret != -ERESTARTSYS)) {
        hi_log_error("wait done timeout, chn=%d\n", chn_num);
        hi_log_print_func_err(crypto_queue_wait_timeout, ret);
        drv_symc_get_err_code(chn_num);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    return HI_SUCCESS;
}

#endif

/* set reduceing power disspation */
static hi_void drv_symc_core_auto_cken_bypass(hi_void)
{
#ifdef CRYPTO_CORE_AUTO_CKEN_SUPPORT
    cal_crg_cfg core_auto_cken_bypass;

    hi_log_func_enter();

    core_auto_cken_bypass.u32 = symc_read(SPACC_CALC_CRG_CFG);

    core_auto_cken_bypass.bits.spacc_core_auto_cken_bypass = 0x0;
    core_auto_cken_bypass.bits.spacc_rft_mem_wr_clk_gt_en = 0x1;
    core_auto_cken_bypass.bits.spacc_rft_mem_rd_clk_gt_en = 0x1;
    core_auto_cken_bypass.bits.spacc_rfs_mem_clk_gt_en = 0x1;

    symc_write(SPACC_CALC_CRG_CFG, core_auto_cken_bypass.u32);

    hi_log_info("SPACC_CALC_CRG_CFG[0x%x]: 0x%x\n", SPACC_CALC_CRG_CFG, core_auto_cken_bypass.u32);

    hi_log_func_exit();
#endif
}

/* set symc entry */
static hi_s32 drv_symc_recover_entry(hi_u32 chn)
{
    chann_cipher_in_node_cfg cipher_in_cfg;
    chann_cipher_out_node_cfg cipher_out_cfg;
    symc_hard_context *ctx = &g_hard_context[chn];
    compat_addr out_addr;
    hi_u32 entry;

    hi_log_func_enter();

    /* set total num and start addr for cipher in node
     * On ree, the chn may be seized by tee,
     * so we must check it, that is check we can write the reg of chn or not.
     */
    symc_write(chann_cipher_in_node_start_addr(chn), addr_l32(ctx->dma_entry));
    symc_write(chann_cipher_in_node_start_high(chn), addr_h32(ctx->dma_entry));
    entry = symc_read(chann_cipher_in_node_start_addr(chn));
    if (entry != addr_l32(ctx->dma_entry)) {
        hi_log_info("the ree chn be seized by tee\n");
        hi_log_print_err_code(HI_ERR_CIPHER_UNAVAILABLE);
        return HI_ERR_CIPHER_UNAVAILABLE;
    }
    cipher_in_cfg.u32  = symc_read(chann_cipher_in_node_cfg(chn));
    ctx->id_in = cipher_in_cfg.bits.cipher_in_node_wptr;
    hi_log_info("symc chn %d recover, id in  0x%x, IN_NODE_START_ADDR  0x%x, VIA %p\n",
                chn, ctx->id_in, addr_l32(ctx->dma_entry), ctx->entry_in);

    /* set total num and start addr for cipher out node. */
    cipher_out_cfg.u32 = symc_read(chann_cipher_out_node_cfg(chn));
    addr_u64(out_addr) = addr_u64(ctx->dma_entry) + SYMC_NODE_SIZE;
    symc_write(chann_cipher_out_node_start_addr(chn), addr_l32(out_addr));
    symc_write(chann_cipher_out_node_start_high(chn), addr_h32(out_addr));
    ctx->id_out = cipher_out_cfg.bits.cipher_out_node_wptr;
    hi_log_info("symc chn %d recover, id out 0x%x, OUT_NODE_START_ADDR 0x%x, VIA %p\n",
                chn, ctx->id_out, addr_l32(ctx->dma_entry) + SYMC_NODE_SIZE, ctx->entry_out);

    hi_log_func_exit();
    return HI_SUCCESS;
}

/* set symc entry */
static hi_void drv_symc_set_entry(hi_u32 chn, compat_addr dma_addr, hi_void *cpu_addr)
{
    chann_cipher_in_node_cfg cipher_in_cfg;
    chann_cipher_out_node_cfg cipher_out_cfg;
    symc_hard_context *ctx = &g_hard_context[chn];

    hi_log_func_enter();

    /* set total num and start addr for cipher in node. */
    cipher_in_cfg.u32  = symc_read(chann_cipher_in_node_cfg(chn));
    cipher_in_cfg.bits.cipher_in_node_total_num = SYMC_MAX_LIST_NUM;
    symc_write(chann_cipher_in_node_cfg(chn), cipher_in_cfg.u32);
    hi_log_info("chann_cipher_in_node_cfg[0x%x]: \t0x%x, PHY: 0x%x, VIA %p\n",
                chann_cipher_in_node_cfg(chn), cipher_in_cfg.u32, addr_l32(dma_addr), cpu_addr);
    ctx->entry_in = (symc_entry_in *)cpu_addr;
    ctx->cnt_in = 0;
    addr_u64(dma_addr) += SYMC_NODE_SIZE;
    cpu_addr = (hi_u8 *)cpu_addr + SYMC_NODE_SIZE;

    /* set total num and start addr for cipher out node. */
    cipher_out_cfg.u32 = symc_read(chann_cipher_out_node_cfg(chn));
    cipher_out_cfg.bits.cipher_out_node_total_num = SYMC_MAX_LIST_NUM;
    symc_write(chann_cipher_out_node_cfg(chn), cipher_out_cfg.u32);
    hi_log_info("chann_cipher_out_node_cfg[0x%x]: \t0x%x, PHY: 0x%x, VIA %p\n",
                chann_cipher_out_node_cfg(chn), cipher_out_cfg.u32, addr_l32(dma_addr), cpu_addr);
    ctx->entry_out = (symc_entry_out *)cpu_addr;
    ctx->cnt_out = 0;
    addr_u64(dma_addr) += SYMC_NODE_SIZE;

    hi_log_func_exit();
}

/* set symc pad buffer */
static hi_void drv_symc_set_pad_buffer(hi_u32 chn, compat_addr dma_addr, hi_void *cpu_addr)
{
    symc_hard_context *ctx = &g_hard_context[chn];

    ctx->dma_pad = dma_addr;
    ctx->via_pad = cpu_addr;
    ctx->offset_pad = 0x00;
}

/* set smmu */
static hi_void drv_symc_smmu_bypass(hi_void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    cipher_in_smmu_en cipher_in_smmu_en;
    out_smmu_en out_smmu_en;

    hi_log_func_enter();

    cipher_in_smmu_en.u32 = symc_read(CIPHER_IN_SMMU_EN);
    out_smmu_en.u32 = symc_read(OUT_SMMU_EN);

    cipher_in_smmu_en.bits.cipher_in_chan_rd_dat_smmu_en  |= CIPHER_HARD_CHANNEL_MASK >> 1;
    cipher_in_smmu_en.bits.cipher_in_chan_rd_node_smmu_en &= ~(CIPHER_HARD_CHANNEL_MASK >> 1);

    out_smmu_en.bits.out_chan_wr_dat_smmu_en  |= CIPHER_HARD_CHANNEL_MASK >> 1;
    out_smmu_en.bits.out_chan_rd_node_smmu_en &= ~(CIPHER_HARD_CHANNEL_MASK >> 1);

    symc_write(CIPHER_IN_SMMU_EN, cipher_in_smmu_en.u32);
    symc_write(OUT_SMMU_EN, out_smmu_en.u32);

    hi_log_info("CIPHER_IN_SMMU_EN[0x%x]: 0x%x\n", CIPHER_IN_SMMU_EN, cipher_in_smmu_en.u32);
    hi_log_info("OUT_SMMU_EN[0x%x]      : 0x%x\n", OUT_SMMU_EN, out_smmu_en.u32);

    hi_log_func_exit();
#endif
}

/* smmu set base address */
static hi_void drv_symc_smmu_base_addr(hi_void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    hi_u64 err_raddr = 0;
    hi_u64 err_waddr = 0;
    hi_u64 table_addr = 0;

    hi_log_func_enter();

    /* get table base addr from system api */
    smmu_get_table_addr(&err_raddr, &err_waddr, &table_addr);

    if (crypto_is_sec_cpu()) {
        symc_write(SEC_SMMU_START_ADDR, (hi_u32)table_addr);
        hi_log_info("SEC_SMMU_START_ADDR[0x%x]  : 0x%x\n", SEC_SMMU_START_ADDR, (hi_u32)table_addr);
    } else {
        symc_write(NORM_SMMU_START_ADDR, (hi_u32)table_addr);
        hi_log_info("NORM_SMMU_START_ADDR[0x%x]  : 0x%x\n", NORM_SMMU_START_ADDR, (hi_u32)table_addr);
    }
#endif

    hi_log_func_exit();
    return;
}

/* set secure channel,
 * non-secure CPU can't change the value of SEC_CHN_CFG,
 * so non-secure CPU call this function will do nothing.
 */
static hi_void drv_symc_enable_secure(hi_u32 chn, hi_u32 enable)
{
    sec_chn_cfg sec_chn_cfg;

    hi_log_func_enter();

    /* The SEC_CHN_CFG only can be set by secure CPU. */
    sec_chn_cfg.u32 = symc_read(SEC_CHN_CFG);
    if (enable == HI_TRUE) {
        sec_chn_cfg.bits.cipher_sec_chn_cfg |= 0x01 << chn;
    } else {
        sec_chn_cfg.bits.cipher_sec_chn_cfg &= ~(0x01 << chn);
    }
    symc_write(SEC_CHN_CFG, sec_chn_cfg.u32);
    hi_log_info("SEC_CHN_CFG[0x%x]: 0x%x\n", SEC_CHN_CFG, sec_chn_cfg.u32);

    hi_log_func_exit();

    return;
}

static hi_void drv_symc_print_last_node(hi_u32 chn_num)
{
    symc_entry_in *in = HI_NULL;
    symc_entry_out *out = HI_NULL;
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = &g_hard_context[chn_num];

    /* get last in node info. */
    if (ctx->id_in == 0x00) {
        in = &ctx->entry_in[SYMC_NODE_LIST_SIZE];
    } else {
        in = &ctx->entry_in[ctx->id_in - 1];
    }

    /* get last out node info. */
    if (ctx->id_out == 0x00) {
        out = &ctx->entry_out[SYMC_NODE_LIST_SIZE];
    } else {
        out = &ctx->entry_out[ctx->id_out - 1];
    }

    hi_log_error("chn %d, src addr 0x%x, size 0x%x, dest addr 0x%x, size 0x%x\n",
                 chn_num, in->sym_start_addr, in->sym_alg_length,
                 out->sym_start_addr, out->sym_alg_length);
    crypto_unused(in);
    crypto_unused(out);

    hi_log_func_exit();
    return;
}

static hi_void drv_symc_print_status(hi_u32 chn_num)
{
    cipher_int_raw    raw;
    cipher_int_status status;
    cipher_int_en     enable;
    sec_chn_cfg       cfg;
    chann_cipher_in_node_cfg in_node_cfg;
    chann_cipher_out_node_cfg out_node_cfg;

    hi_log_func_enter();

    in_node_cfg.u32  = symc_read(chann_cipher_in_node_cfg(chn_num));
    out_node_cfg.u32 = symc_read(chann_cipher_out_node_cfg(chn_num));
    hi_log_error("chann_cipher_in_node_cfg        : 0x%x\n", in_node_cfg.u32);
    hi_log_error("chann_cipher_in_node_start_addr : 0x%x\n", symc_read(chann_cipher_in_node_start_addr(chn_num)));
    hi_log_error("chann_cipher_in_buf_rptr        : 0x%x\n", symc_read(chann_cipher_in_buf_rptr(chn_num)));
    hi_log_error("chann_cipher_out_node_cfg       : 0x%x\n", out_node_cfg.u32);
    hi_log_error("chann_cipher_out_node_start_addr: 0x%x\n", symc_read(chann_cipher_out_node_start_addr(chn_num)));
    hi_log_error("chann_cipher_out_buf_rptr       : 0x%x\n", symc_read(chann_cipher_out_buf_rptr(chn_num)));
    hi_log_error("chann_cipher_ctrl               : 0x%x\n", symc_read(chann_cipher_ctrl(chn_num)));

    raw.u32    = symc_read(CIPHER_INT_RAW);
    status.u32 = symc_read(CIPHER_INT_STATUS);
    enable.u32 = symc_read(CIPHER_INT_EN);
    cfg.u32    = symc_read(SEC_CHN_CFG);
    hi_log_error("\nsec_chn_cfg 0x%x, chn %d, nsec_int_en 0x%x, sec_int_en 0x%x, chn_obuf_en 0x%x, status 0x%x, "
                 "raw 0x%x\n",
                 (cfg.bits.cipher_sec_chn_cfg >> chn_num) & 0x01,
                 chn_num, enable.bits.cipher_nsec_int_en, enable.bits.cipher_sec_int_en,
                 (enable.bits.cipher_chn_obuf_en >> chn_num) & 0x01,
                 (status.bits.cipher_chn_obuf_int >> chn_num) & 0x01,
                 (raw.bits.cipher_chn_obuf_raw >> chn_num) & 0x01);

    hi_log_error("\nThe cause of time out may be:\n"
                 "\t1. SMMU address invalid\n"
                 "\t2. interrupt number or name incorrect\n"
                 "\t3. CPU type mismatching, request CPU and channel: %s\n",
                 crypto_is_sec_cpu() ? "secure" : "non-secure");

    /* ahi_void compile error when hi_log_error be defined to empty */
    crypto_unused(raw);
    crypto_unused(status);
    crypto_unused(enable);
    crypto_unused(cfg);
    crypto_unused(in_node_cfg);
    crypto_unused(out_node_cfg);

    hi_log_func_exit();

    return;
}

hi_void drv_symc_get_err_code(hi_u32 chn_num)
{
    hi_u32 code;

    hi_log_func_enter();

    /* check error code
     * bit0: klad_key_use_err
     * bit1: alg_len_err
     * bit2: smmu_page_unvlid
     * bit3: out_smmu_page_not_valid
     * bit4: klad_key_write_err
     */
    code = symc_read(CALC_ERR);
    if (code & KLAD_KEY_USE_ERR) {
        hi_log_error("symc error: klad_key_use_err, chn %d !!!\n", chn_num);
    }
    if (code & ALG_LEN_ERR) {
        hi_log_error("symc error: alg_len_err, chn %d !!!\n", chn_num);
    }
    if (code & SMMU_PAGE_UNVLID) {
        hi_log_error("symc error: smmu_page_unvlid, chn %d !!!\n", chn_num);
    }
    if (code & OUT_SMMU_PAGE_NOT_VALID) {
        hi_log_error("symc error: out_smmu_page_not_valid, chn %d !!!\n", chn_num);
    }
    if (code & KLAD_KEY_WRITE_ERR) {
        hi_log_error("symc error: klad_key_write_err, chn %d !!!\n", chn_num);
    }

    /* print the inout buffer address. */
    drv_symc_print_last_node(chn_num);
    drv_symc_print_status(chn_num);

    hi_log_func_exit();
}

static hi_void drv_symc_entry_init(crypto_mem  mem)
{
    hi_u32 i = 0;
    hi_void *cpu_addr = HI_NULL;
    compat_addr dma_pad;
    hi_u8    *via_pad = HI_NULL;

    hi_log_func_enter();

    /* set in node and out node dma buffer */
    hi_log_info("symc entry list configure\n");
    cpu_addr = mem.dma_virt;

    /* skip the in node and out node dma buffer */
    addr_u64(dma_pad) = addr_u64(mem.dma_addr) + SYMC_NODE_LIST_SIZE;
    via_pad = (hi_u8 *)mem.dma_virt + SYMC_NODE_LIST_SIZE;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((CIPHER_HARD_CHANNEL_MASK >> i) & 0x01) { /* valid channel. */
            /* in node and out node */
            drv_symc_set_entry(i, mem.mmz_addr, cpu_addr);
            addr_u64(g_hard_context[i].dma_entry) = addr_u64(mem.mmz_addr);
            addr_u64(mem.mmz_addr) += SYMC_NODE_SIZE * MUL_VAL_2; /* move to next channel */
            cpu_addr = (hi_u8 *)cpu_addr + SYMC_NODE_SIZE * MUL_VAL_2; /* move to next channel */

            /* padding */
            drv_symc_set_pad_buffer(i, dma_pad, via_pad);
            addr_u64(dma_pad) += SYMC_PAD_BUFFER_LEN;
            via_pad += SYMC_PAD_BUFFER_LEN;
        }
    }

    hi_log_func_exit();
    return;
}

static hi_s32 drv_symc_mem_init(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    /*
     * dma buffer struct
     * ((in_node || out_node) * chn_num) || (pad_buffer * chn_num)
     */
    hi_log_info("alloc memory for nodes list\n");
    ret = crypto_mem_create(&g_symc_dma, SEC_MMZ, "symc_node_list",
                            SYMC_NODE_LIST_SIZE + SYMC_PAD_BUFFER_TOTAL_LEN);
    if (ret != HI_SUCCESS) {
        hi_log_error("error, malloc ddr for symc nodes list failed\n");
        hi_log_print_func_err(crypto_mem_create, ret);
        return ret;
    }

    drv_symc_entry_init(g_symc_dma);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 drv_symc_mem_deinit(hi_void)
{
    hi_s32 ret;

    ret = crypto_mem_destory(&g_symc_dma);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_destory, ret);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 drv_symc_chn_resume(hi_u32 chn_num)
{
    hi_s32 ret;
    hi_u32 base = 0;

    hi_log_func_enter();

    if (crypto_is_sec_cpu()) {
        base = symc_read(SEC_SMMU_START_ADDR);
    } else {
        base = symc_read(NORM_SMMU_START_ADDR);
    }

    if (base == 0) {
        /* smmu base address is zero means hardware be unexpected reset */
        hi_log_warn("cipher module is not ready, try to resume it now...\n");
        ret = drv_symc_resume();
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_resume, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_alloc_chn(hi_u32 *chn_num)
{
    hi_s32 ret;
    hi_u32 i;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        /* check the valid channel */
        if (((CIPHER_HARD_CHANNEL_MASK & (0x1 << i)) == 0x00) || (g_hard_context[i].open == HI_TRUE)) {
            continue;
        }

        drv_symc_enable_secure(i, HI_TRUE);
        ret = drv_symc_recover_entry(i);
        if (ret != HI_SUCCESS) {
            continue;
        }

        /* alloc channel */
        g_hard_context[i].open = HI_TRUE;
        *chn_num = i;
        hi_log_info("alloc symc chn %d.\n", i);
        break;
    }

    if (i >= CRYPTO_HARD_CHANNEL_MAX) {
        hi_log_error("symc alloc channel failed\n");
        hi_log_print_err_code(HI_ERR_CIPHER_BUSY);
        return HI_ERR_CIPHER_BUSY;
    }

    /* hardware may be unexpected reset by other module or platform,
     * such as unexpected reset by fastboot after load tee image,
     * in this case, the hardware configuration will be reset,
     * here try to re-config the hardware.
     */
    ret = drv_symc_chn_resume(*chn_num);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_chn_resume, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_free_chn(hi_u32 chn_num)
{
    symc_chn_who_used used;

    hi_log_func_enter();

    if (g_hard_context[chn_num].open == HI_FALSE) {
        hi_log_func_exit();
        return;
    }

    /* clean tee and ree mask */
    if (crypto_is_sec_cpu() == HI_TRUE) {
        used.u32 = symc_read(CHN_WHO_USED_TEE);
        used.bits.non_sec_chn_who_used &= (~symc_chn_mask(chn_num));
        symc_write(CHN_WHO_USED_TEE, used.u32);
    } else {
        used.u32 = symc_read(CHN_WHO_USED_REE);
        used.bits.non_sec_chn_who_used &= (~symc_chn_mask(chn_num));
        symc_write(CHN_WHO_USED_REE, used.u32);
    }

    drv_symc_enable_secure(chn_num, HI_FALSE);
    if (g_hard_context[chn_num].destory != HI_NULL) {
        g_hard_context[chn_num].destory();
        g_hard_context[chn_num].destory = HI_NULL;
    }

    /* free channel. */
    g_hard_context[chn_num].open = HI_FALSE;

    hi_log_info("free symc chn %d.\n", chn_num);

    hi_log_func_exit();
    return;
}

#ifdef CRYPTO_SWITCH_CPU
hi_u32 drv_symc_is_secure(hi_void)
{
    sec_chn_cfg sec;
    sec_chn_cfg tmp;
    hi_u32 secure = HI_FALSE;

    hi_log_func_enter();

    hi_log_info("Change the secure type of the chn0 to get cpu type\n");
    module_enable(CRYPTO_MODULE_ID_SYMC);

    sec.u32 = symc_read(SEC_CHN_CFG);

    /* change the secure type of chn0 */
    sec.bits.cipher_sec_chn_cfg ^= 0x01;
    symc_write(SEC_CHN_CFG, sec.u32);

    /* read the secure type of chn0 */
    tmp.u32 = symc_read(SEC_CHN_CFG);

    if (tmp.bits.cipher_sec_chn_cfg == sec.bits.cipher_sec_chn_cfg) {
        /* The REG_SEC_CHN_CFG only can be set by secure CPU
         * can write the cfg, must be secure CPU
         */
        secure =  HI_TRUE;

        /* recovery the secure type of chn0 */
        sec.bits.cipher_sec_chn_cfg ^= 0x01;
        symc_write(SEC_CHN_CFG, sec.u32);
    }

    hi_log_info("secure type: 0x%x\n", secure);

    hi_log_func_exit();
    return secure;
}
#endif

hi_s32 drv_symc_init(hi_void)
{
    hi_s32 ret;
    hi_u32 i = 0;

    hi_log_func_enter();

    if (g_symc_initialize == HI_TRUE) {
        hi_log_func_exit();
        return HI_SUCCESS;
    }

    crypto_memset(&g_symc_dma, sizeof(g_symc_dma), 0, sizeof(g_symc_dma));
    crypto_memset(g_hard_context, sizeof(g_hard_context), 0, sizeof(g_hard_context));

    hi_log_info("enable symc\n");
    module_enable(CRYPTO_MODULE_ID_SYMC);

    module_disable(CRYPTO_MODULE_ID_SM4);

    ret = drv_symc_mem_init();
    if (ret != HI_SUCCESS) {
        goto __error;
    }
    hi_log_info("SYMC DMA buffer, MMU 0x%x, MMZ 0x%x, VIA %p, size 0x%x\n",
                addr_l32(g_symc_dma.dma_addr), addr_l32(g_symc_dma.mmz_addr),
                g_symc_dma.dma_virt, g_symc_dma.dma_size);

    hi_log_info("symc SMMU configure\n");
    drv_symc_smmu_bypass();
    drv_symc_smmu_base_addr();

    hi_log_info("symc core_auto_cken_bypass configure\n");
    drv_symc_core_auto_cken_bypass();

#ifdef CRYPTO_OS_INT_SUPPORT
    hi_log_info("symc interrupt configure\n");
    drv_symc_set_interrupt();

    hi_log_info("symc register interrupt function\n");
    ret = drv_symc_register_interrupt();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_register_interrupt, ret);
        drv_symc_mem_deinit();
        goto __error;
    }
#endif

    /*  set all channel as non-secure channel,
     *  may be set it to secure channel when alloc handle.
     */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        /* check the valid channel */
        if (CIPHER_HARD_CHANNEL_MASK & (0x1 << i)) {
            drv_symc_enable_secure(i, HI_FALSE);
        }
    }

    g_symc_initialize = HI_TRUE;

    hi_log_func_exit();
    return HI_SUCCESS;

__error:
    module_disable(CRYPTO_MODULE_ID_SYMC);

    return ret;
}

hi_s32 drv_symc_resume(hi_void)
{
    hi_u32 i;
    symc_chn_who_used used;
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_info("enable symc\n");
    module_enable(CRYPTO_MODULE_ID_SYMC);
    module_disable(CRYPTO_MODULE_ID_SM4);

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (g_hard_context[i].open) {
            if (crypto_is_sec_cpu() == HI_TRUE) {
                used.u32 = symc_read(CHN_WHO_USED_TEE);
                used.bits.non_sec_chn_who_used |= symc_chn_mask(i);
                symc_write(CHN_WHO_USED_TEE, used.u32);
            } else {
                used.u32 = symc_read(CHN_WHO_USED_REE);
                used.bits.non_sec_chn_who_used |= symc_chn_mask(i);
                symc_write(CHN_WHO_USED_REE, used.u32);
            }

            drv_symc_enable_secure(i, HI_TRUE);
            ret = drv_symc_recover_entry(i);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(drv_symc_recover_entry, ret);
                return ret;
            }
        }
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_symc_set_interrupt();
#endif

    drv_symc_entry_init(g_symc_dma);
    drv_symc_smmu_bypass();
    drv_symc_smmu_base_addr();

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_suspend(hi_void)
{
    return;
}

hi_s32 drv_symc_deinit(hi_void)
{
    hi_s32 ret;
    hi_u32 i;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);

    ret = drv_symc_mem_deinit();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_mem_deinit, ret);
        return ret;
    }
    module_disable(CRYPTO_MODULE_ID_SYMC);

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_symc_unregister_interrupt();
#endif

    /* free all channel */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (CIPHER_HARD_CHANNEL_MASK & (0x1 << i)) {
            drv_symc_free_chn(i);
        }
    }

    g_symc_initialize = HI_FALSE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_add_buf_usage(hi_u32 chn_num, hi_u32 in, symc_node_usage usage)
{
    symc_hard_context *ctx = HI_NULL;
    hi_u32 id = 0;

    ctx = &g_hard_context[chn_num];

    if (in == HI_TRUE) {
        /* get last node */
        id = (ctx->id_in == 0) ? SYMC_MAX_LIST_NUM - 1 : ctx->id_in - 1;

        ctx->entry_in[id].sym_ctrl = ctx->entry_in[id].sym_ctrl | (hi_u32)usage;
        hi_log_info("chn %d, add symc in ctrl: id %d, ctrl 0x%x\n", chn_num, id, ctx->entry_in[id].sym_ctrl);
    } else {
        /* get last node */
        id = (ctx->id_out == 0) ? SYMC_MAX_LIST_NUM - 1 : ctx->id_out - 1;

        ctx->entry_out[id].aes_ctrl = ctx->entry_out[id].aes_ctrl | (hi_u32)usage;
        hi_log_info("chn %d, add symc out ctrl: id %d, ctrl 0x%x\n", chn_num, id, ctx->entry_out[id].aes_ctrl);
    }

    return;
}

hi_s32 drv_symc_set_iv(hi_u32 chn_num, hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD], hi_u32 ivlen, hi_u32 flag)
{
    hi_u32 i;
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = &g_hard_context[chn_num];

    /* copy iv data into channel context. */
    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        ctx->iv[i] = iv[i];
    }
    ctx->iv_flag = flag;
    ctx->iv_len = ivlen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_get_iv(hi_u32 chn_num, hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD])
{
    hi_u32 i;

    hi_log_func_enter();

    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        iv[i] = symc_read(chann_cipher_ivout(chn_num) + i * WORD_WIDTH);
        hi_log_info("IV[%d]: 0x%x\n", i, iv[i]);
    }

    hi_log_func_exit();
    return;
}

hi_void drv_symc_set_key(hi_u32 chn_num, hi_u32 key[SYMC_KEY_MAX_SIZE_IN_WORD], hi_u32 key_len, hi_u32 odd)
{
    hi_u32 i;

    hi_log_func_enter();
    if (key_len < SYMC_KEY_MAX_SIZE_IN_WORD * WORD_WIDTH) {
        return;
    }

    /* Set key, odd key only valid for aes ecb/cbc/ofb/cfb/ctr. */
    symc_write(ODD_EVEN_KEY_SEL, odd);
    for (i = 0; i < SYMC_KEY_MAX_SIZE_IN_WORD; i++) {
        symc_write(cipher_key(chn_num) + i * WORD_WIDTH, key[i]);
    }

    hi_log_func_exit();
    return;
}

hi_void drv_symc_set_sm1_sk(hi_u32 chn_num, hi_u32 key[SYMC_SM1_SK_SIZE_IN_WORD], hi_u32 key_len)
{
    hi_u32 i;

    if (key_len < SYMC_SM1_SK_SIZE_IN_WORD * WORD_WIDTH) {
        return;
    }

    for (i = 0; i < SYMC_SM1_SK_SIZE_IN_WORD; i++) {
        symc_write(sm1_sk(chn_num) + i * WORD_WIDTH, key[i]);
    }
    return;
}

hi_s32 drv_symc_add_inbuf(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size, symc_node_usage usage)
{
    symc_hard_context *ctx = HI_NULL;
    hi_u32 id, size;
    hi_u32 i;
    hi_void *addr = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    id = ctx->id_in;
    addr = &ctx->entry_in[id];
    size = sizeof(symc_entry_in);
    crypto_memset(addr, size, 0, size);
    ctx->entry_in[id].spacc_cmd = 0x00;
    ctx->entry_in[id].sym_start_addr = addr_l32(buf_phy);
    ctx->entry_in[id].sym_start_high = addr_h32(buf_phy);
    ctx->entry_in[id].sym_alg_length = buf_size;
    ctx->entry_in[id].sym_ctrl =  usage;

    if (ctx->mode == SYMC_MODE_GCM) {
        ctx->entry_in[id].gcm_iv_len = ctx->iv_len - 1;
    }

    /* set IV to every node, but the hardware only update th IV
     * from node when first flag is 1
     */
    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        ctx->entry_in[id].iv[i] = ctx->iv[i];
        hi_log_debug("IV[%d]: 0x%x\n", i, ctx->iv[i]);
    }

    if (ctx->iv_flag == HI_CIPHER_IV_CHG_ONE_PACK) {
        /* update iv for first node */
        ctx->iv_flag = 0x00;

        /* don't update iv any more */
        ctx->entry_in[id].sym_ctrl |= SYMC_NODE_USAGE_FIRST;
    } else if (ctx->iv_flag == HI_CIPHER_IV_CHG_ALL_PACK) {
        /* update iv for all node */
        ctx->entry_in[id].sym_ctrl |= SYMC_NODE_USAGE_FIRST | SYMC_NODE_USAGE_LAST;
    }

    /* move to next node */
    ctx->id_in++;
    ctx->id_in %= SYMC_MAX_LIST_NUM;
    hi_log_info("chn %d, add symc in buf[%p]: id %d, addr 0x%x, len 0x%x, ctrl 0x%x\n",
                chn_num, &ctx->entry_in[id], id, addr_l32(buf_phy), buf_size, ctx->entry_in[id].sym_ctrl);

    /* total count of computed nodes add 1. */
    ctx->cnt_in++;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_add_outbuf(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size, symc_node_usage usage)
{
    symc_hard_context *ctx = HI_NULL;
    hi_u32 id, size;
    hi_void *addr = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    id = ctx->id_out;
    addr = &ctx->entry_out[id];
    size = sizeof(symc_entry_out);
    crypto_memset(addr, size, 0, size);
    ctx->entry_out[id].sym_start_addr = addr_l32(buf_phy);
    ctx->entry_out[id].tag[0] = addr_h32(buf_phy);
    ctx->entry_out[id].sym_alg_length = buf_size;
    ctx->entry_out[id].aes_ctrl =  usage;

    /* move to next node */
    ctx->id_out++;
    ctx->id_out %= SYMC_MAX_LIST_NUM;
    hi_log_info("chn %d, add symc out buf[%p]: id %d, addr 0x%x, len 0x%x, ctrl 0x%x\n",
                chn_num, &ctx->entry_out[id], id, addr_l32(buf_phy), buf_size, usage);

    /* total count of computed nodes add 1. */
    ctx->cnt_out++;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_aead_ccm_add_n(hi_u32 chn_num, hi_u8 *nonce, hi_u32 nonce_len)
{
    hi_s32 ret;
    symc_hard_context *ctx = HI_NULL;
    symc_node_usage usage;
    compat_addr dma_pad;
    hi_u8 *via_pad = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    hi_log_debug("PAD buffer, PHY: 0x%x, VIA %p\n", addr_l32(ctx->dma_pad), ctx->via_pad);

    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;

    usage = SYMC_NODE_USAGE_IN_CCM_N | SYMC_NODE_USAGE_LAST;
    crypto_memcpy(via_pad, SYMC_CCM_N_LEN, nonce, nonce_len);
    ctx->offset_pad += SYMC_CCM_N_LEN;
    ret = drv_symc_add_inbuf(chn_num, dma_pad, SYMC_CCM_N_LEN, usage);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_inbuf, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_u32 drv_aead_ccm_a_head(hi_u8 *buf, hi_u32 alen)
{
    hi_u32 idx = 0;

    /*
     * Formatting of the Associated Data in B1, the length of A denotes as a.
     *
     * The value a is encoded according to the following three cases:
     * If 0 < a < 2^16 - 2^8, then a  is encoded as a[0..15], i.e., two octets.
     * If 2^16 - 2^8 <= a < 2^32, then a  is encoded as 0xff || 0xfe || a[0..31], i.e., six octets.
     * If 2^32 <= a < 2^64, then  a is encoded as 0xff || 0xff || a[0..63], i.e., ten octets.
     * For example, if a=2^16, the encoding of a  is
     * 11111111 11111110 00000000 00000001 00000000 00000000.
     */
    if (alen < SYMC_CCM_A_SMALL_LEN) {
        buf[idx++] = (hi_u8)(alen >> SHIFT_8BITS);
        buf[idx++] = (hi_u8)(alen);
    } else {
        buf[idx++] = SYMC_AAD_PAD_VAL_FF;
        buf[idx++] = SYMC_AAD_PAD_VAL_FE;
        buf[idx++] = (hi_u8)(alen >> SHIFT_24BITS);
        buf[idx++] = (hi_u8)(alen >> SHIFT_16BITS);
        buf[idx++] = (hi_u8)(alen >> SHIFT_8BITS);
        buf[idx++] = (hi_u8)alen;
    }

    return idx;
}

hi_s32 drv_aead_ccm_add_a(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size)
{
    symc_hard_context *ctx = HI_NULL;
    compat_addr dma_pad;
    hi_u8 *via_pad = HI_NULL;
    hi_s32 ret;
    hi_u32 count;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);
    ctx = &g_hard_context[chn_num];
    hi_log_chk_param_return((ctx->offset_pad + SYMC_CCM_A_HEAD_LEN) >= SYMC_PAD_BUFFER_LEN);

    /* return success when alen is zero. */
    if (buf_size == 0x00) {
        return HI_SUCCESS;
    }

    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;
    crypto_memset(via_pad, AES_BLOCK_SIZE * MUL_VAL_2, 0, AES_BLOCK_SIZE * MUL_VAL_2);

    /* add ccm a head */
    count = drv_aead_ccm_a_head(via_pad, buf_size);
    ret = drv_symc_add_inbuf(chn_num, dma_pad, count, SYMC_NODE_USAGE_IN_CCM_A);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* move buffer addr */
    ctx->offset_pad += count;
    addr_u64(dma_pad) += count;
    via_pad += count;

    /*  add the phy of A into node list. */
    ret = drv_symc_add_inbuf(chn_num, buf_phy, buf_size, SYMC_NODE_USAGE_IN_CCM_A);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* if idx + Alen do not aligned with 16, padding 0 to the tail. */
    count = (buf_size + count) % AES_BLOCK_SIZE;
    if (count > 0) {
        /* add the padding phy of A into node list. */
        ret = drv_symc_add_inbuf(chn_num, dma_pad, AES_BLOCK_SIZE - count,
                                 SYMC_NODE_USAGE_IN_CCM_A);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_add_inbuf, ret);
            return ret;
        }
        ctx->offset_pad += AES_BLOCK_SIZE - count;
    }

    /* add ccm a last flag */
    drv_symc_add_buf_usage(chn_num, HI_TRUE, SYMC_NODE_USAGE_LAST);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_aead_gcm_add_a(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size)
{
    symc_hard_context *ctx = HI_NULL;
    compat_addr dma_pad;
    hi_u8 *via_pad = HI_NULL;
    hi_s32 ret;
    hi_u32 count = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);
    ctx = &g_hard_context[chn_num];
    hi_log_chk_param_return((ctx->offset_pad + AES_BLOCK_SIZE) >= SYMC_PAD_BUFFER_LEN);

    /* return success when alen is zero. */
    if (buf_size == 0x00) {
        hi_log_func_exit();
        return HI_SUCCESS;
    }

    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;
    crypto_memset(via_pad, AES_BLOCK_SIZE, 0, AES_BLOCK_SIZE);

    /* Add phy of A into node list. */
    ret = drv_symc_add_inbuf(chn_num, buf_phy, buf_size, SYMC_NODE_USAGE_IN_GCM_A);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* if Alen do not aligned with 16, padding 0 to the tail. */
    count = (buf_size + count) % AES_BLOCK_SIZE;
    if (count > 0) {
        /* add the padding phy of A into node list. */
        ret = drv_symc_add_inbuf(chn_num, dma_pad, AES_BLOCK_SIZE - count,
                                 SYMC_NODE_USAGE_IN_GCM_A);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_add_inbuf, ret);
            return ret;
        }
        ctx->offset_pad += AES_BLOCK_SIZE - count;
    }

    /* add gcm a last flag */
    drv_symc_add_buf_usage(chn_num, HI_TRUE, SYMC_NODE_USAGE_LAST);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_aead_gcm_add_clen(hi_u32 chn_num, hi_u8 *clen, hi_u32 clen_len)
{
    hi_s32 ret;
    symc_hard_context *ctx = HI_NULL;
    symc_node_usage usage;
    compat_addr dma_pad;
    hi_u8 *via_pad = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);
    ctx = &g_hard_context[chn_num];
    hi_log_chk_param_return((ctx->offset_pad + SYMC_CCM_N_LEN) >= SYMC_PAD_BUFFER_LEN);

    /* add Clen */
    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;

    usage = SYMC_NODE_USAGE_IN_GCM_LEN | SYMC_NODE_USAGE_LAST;

    crypto_memcpy(via_pad, SYMC_GCM_CLEN_LEN, clen, clen_len);
    ctx->offset_pad += SYMC_GCM_CLEN_LEN;

    ret = drv_symc_add_inbuf(chn_num, dma_pad, SYMC_GCM_CLEN_LEN, usage);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_add_inbuf, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_aead_get_tag(hi_u32 chn_num, hi_u32 *tag, hi_u32 tag_buf_len)
{
    symc_hard_context *ctx = HI_NULL;
    chann_cipher_out_node_cfg out_node_cfg;
    hi_u32 last;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    out_node_cfg.u32 = symc_read(chann_cipher_out_node_cfg(chn_num));
    last = out_node_cfg.bits.cipher_out_node_wptr;
    last = (last == 0) ? (SYMC_MAX_LIST_NUM - 1) : (last - 1);

    crypto_memcpy(tag, tag_buf_len, ctx->entry_out[last].tag, sizeof(ctx->entry_out[last].tag));

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_node_check(symc_alg alg, symc_mode mode, hi_u32 klen, hi_u32 block_size, symc_multi_pack *pack)
{
    hi_u32 i;
    hi_u32 total = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(block_size == 0);
    hi_log_chk_param_return(pack == HI_NULL);

    crypto_unused(pack->in);
    crypto_unused(pack->out);

    for (i = 0; i < pack->num; i++) {
        /* Used the odd key must accord with conditions as follows: */
        if ((hi_u32)pack->usage[i] & SYMC_NODE_USAGE_ODD_KEY) {
            /* 1. Only support aes ecb/cbc/cfb/ofb/ctr */
            if ((alg != SYMC_ALG_AES) ||
                ((mode != SYMC_MODE_ECB) && (mode != SYMC_MODE_CBC) && (mode != SYMC_MODE_CFB) &&
                 (mode != SYMC_MODE_OFB) && (mode != SYMC_MODE_CTR))) {
                hi_log_error("Odd key only support aes ecb/cbc/cfb/ofb/ctr.");
                hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
                return HI_ERR_CIPHER_INVALID_PARAM;
            }

            /* 2. Only support aes128 */
            hi_log_chk_param_return(klen != AES_CCM_GCM_KEY_LEN);

            /* 3. each node length must be a multiple of 64 */
            hi_log_chk_param_return((pack->len[i] % (AES_BLOCK_SIZE * WORD_WIDTH)) != 0);
        }

        /* each node length can't be zero, and check overflow. */
        hi_log_chk_param_return((pack->len[i] == 0) || (pack->len[i] > addr_l32(pack->in[i]) + pack->len[i]));
        total += pack->len[i];
    }

    if ((alg != SYMC_ALG_NULL_CIPHER) &&
        ((mode == SYMC_MODE_ECB) || (mode == SYMC_MODE_CBC) || (mode == SYMC_MODE_CFB) || (mode == SYMC_MODE_OFB))) {
        /* The length of data depend on alg and mode, which limit to hardware
         * for ecb/cbc/ofb/cfb, the total data length must aligned with block size.
         * for ctr/ccm/gcm, support any data length.
         */
        if (total % block_size != 0) {
            hi_log_error("PKG len must align with 0x%x.\n", block_size);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_LEN);
            return HI_ERR_CIPHER_INVALID_LEN;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_cfg(cryp_symc_context *ctx_cfg, hi_u32 decrypt, symc_klen klen)
{
    symc_hard_context *ctx = HI_NULL;
    chann_chipher_ctrl cipher_ctrl;

    hi_log_func_enter();
    hi_log_chk_param_return(ctx_cfg == HI_NULL);
    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(ctx_cfg->hard_chn >= CRYPTO_HARD_CHANNEL_MAX);
    hi_log_chk_param_return(ctx_cfg->alg >= SYMC_ALG_COUNT);
    hi_log_chk_param_return(ctx_cfg->mode > SYMC_MODE_GCM);
    hi_log_chk_param_return(klen >= SYMC_KEY_LEN_COUNT);

    ctx = &g_hard_context[ctx_cfg->hard_chn];

    /* record alg */
    ctx->alg = ctx_cfg->alg;
    crypto_unused(ctx_cfg->sm1_round);

    hi_log_info("symc configure, chn %d, alg %d, mode %d, dec %d, hard %d\n",
                ctx_cfg->hard_chn, ctx_cfg->alg, ctx_cfg->mode, decrypt, ctx_cfg->hard_key);

    cipher_ctrl.u32 = symc_read(chann_cipher_ctrl(ctx_cfg->hard_chn));
    cipher_ctrl.bits.sym_chn_sm1_round_num = ctx_cfg->sm1_round;
    cipher_ctrl.bits.sym_chn_key_sel = ctx_cfg->hard_key;
    cipher_ctrl.bits.sym_chn_key_length = klen;
    cipher_ctrl.bits.sym_chn_dat_width = ctx_cfg->width;
    cipher_ctrl.bits.sym_chn_decrypt = decrypt;
    cipher_ctrl.bits.sym_chn_alg_sel = ctx_cfg->alg;
    cipher_ctrl.bits.sym_chn_alg_mode = ctx_cfg->mode;
    ctx->mode = ctx_cfg->mode;
    symc_write(chann_cipher_ctrl(ctx_cfg->hard_chn), cipher_ctrl.u32);
    hi_log_info("chann_cipher_ctrl(%d): 0x%x\n", ctx_cfg->hard_chn, cipher_ctrl.u32);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_set_isr_callback(hi_u32 chn_num, callback_symc_isr callback, hi_void *ctx)
{
    symc_hard_context *hisi_ctx = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    hisi_ctx = &g_hard_context[chn_num];

    hisi_ctx->callback = callback;
    hisi_ctx->ctx = ctx;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_start(hi_u32 chn_num)
{
    symc_hard_context *ctx = HI_NULL;
    chann_cipher_in_node_cfg in_node_cfg;
    chann_cipher_out_node_cfg out_node_cfg;
    hi_u32 ptr;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    hi_log_info("symc start, chn %d\n", chn_num);

    ctx->done = HI_FALSE;

    /* configure out nodes. */
    out_node_cfg.u32 = symc_read(chann_cipher_out_node_cfg(chn_num));
    if (out_node_cfg.bits.cipher_out_node_wptr != out_node_cfg.bits.cipher_out_node_rptr) {
        hi_log_error("Error, chn %d is busy.\n", chn_num);
        hi_log_print_err_code(HI_ERR_CIPHER_BUSY);
        return HI_ERR_CIPHER_BUSY;
    }
    ptr = out_node_cfg.bits.cipher_out_node_wptr + ctx->cnt_out;
    out_node_cfg.bits.cipher_out_node_wptr = ptr % SYMC_MAX_LIST_NUM;
    out_node_cfg.bits.cipher_out_node_mpackage_int_level = ctx->cnt_out;
    symc_write(chann_cipher_out_node_cfg(chn_num), out_node_cfg.u32);
    hi_log_info("chann_cipher_out_node_cfg: 0x%x\n", out_node_cfg.u32);

    /* configure in nodes. */
    in_node_cfg.u32 = symc_read(chann_cipher_in_node_cfg(chn_num));
    if (in_node_cfg.bits.cipher_in_node_wptr != in_node_cfg.bits.cipher_in_node_rptr) {
        hi_log_error("Error, chn %d is busy.\n", chn_num);
        hi_log_print_err_code(HI_ERR_CIPHER_BUSY);
        return HI_ERR_CIPHER_BUSY;
    }
    ptr = in_node_cfg.bits.cipher_in_node_wptr + ctx->cnt_in;
    in_node_cfg.bits.cipher_in_node_wptr = ptr % SYMC_MAX_LIST_NUM;
    in_node_cfg.bits.cipher_in_node_mpackage_int_level = ctx->cnt_in;

    /* SM4 may be disable independent from spacc */
    if (ctx->alg == SYMC_ALG_SM4) {
        module_enable(CRYPTO_MODULE_ID_SM4);
    }

    /* start */
    symc_write(chann_cipher_in_node_cfg(chn_num), in_node_cfg.u32);
    hi_log_info("chann_cipher_in_node_cfg: 0x%x\n", in_node_cfg.u32);

    /* all the nodes are processed, retset the cnount to zero */
    ctx->cnt_in = 0;
    ctx->cnt_out = 0;
    ctx->offset_pad = 0;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 drv_symc_query_ram_interrupt(hi_u32 chn_num, symc_hard_context *ctx, hi_u32 timeout)
{
    hi_s32 i;

    /* interrupt unsupport, query the raw interrupt flag. */
    for (i = 0; i < timeout; i++) {
        if (drv_symc_done_try(chn_num)) {
            break;
        }
        if (i <= MS_TO_US) {
            crypto_udelay(1);  /* short waitting for 1000 us. */
        } else {
            crypto_msleep(1);  /* long waitting for 5000 ms. */
        }
    }

    /* Disable SM4 independent from spacc */
    if (ctx->alg == SYMC_ALG_SM4) {
        module_disable(CRYPTO_MODULE_ID_SM4);
    }

    if (timeout <= i) {
        hi_log_error("symc wait done timeout, chn=%d\n", chn_num);
        hi_log_print_err_code(HI_ERR_CIPHER_TIMEOUT);
        drv_symc_get_err_code(chn_num);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    return HI_SUCCESS;
}

hi_s32 drv_symc_wait_done(hi_u32 chn_num, hi_u32 timeout)
{
    hi_s32 ret;
    hi_u32 int_valid = 0;
    hi_u32 int_num = 0;
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(g_symc_initialize != HI_TRUE);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];
    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, HI_NULL);

#ifdef CRYPTO_OS_INT_SUPPORT
    /* interrupt support, wait irq. */
    if (int_valid) {
        ret = drv_symc_wait_irq(chn_num, ctx, timeout);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_wait_irq, ret);
            return ret;
        }
    } else {
        /* interrupt unsupport, query the raw interrupt flag. */
        ret = drv_symc_query_ram_interrupt(chn_num, ctx, timeout);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_query_ram_interrupt, ret);
            return ret;
        }
    }
#else
    /* interrupt unsupport, query the raw interrupt flag. */
    ret = drv_symc_query_ram_interrupt(chn_num, ctx, timeout);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_query_ram_interrupt, ret);
        return ret;
    }
#endif

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_set_destory_callbcak(hi_u32 chn_num, callback_symc_destory destory)
{
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = &g_hard_context[chn_num];
    ctx->destory = destory;

    hi_log_func_exit();
    return HI_SUCCESS;
}

/* ****** proc function begin ******* */
#if (1 == HI_PROC_SUPPORT)
static hi_void drv_symc_proc_status_set_open(symc_chn_status *status, hi_u32 chnnel_id)
{
    if (g_hard_context[chnnel_id].open == HI_TRUE) {
        status[chnnel_id].open = "open ";
    } else {
        status[chnnel_id].open = "close";
    }
}

static hi_void drv_symc_proc_status_set_alg(symc_chn_status *status, hi_u32 chnnel_id, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    switch (ctrl.bits.sym_chn_alg_sel) {
        case HI_CIPHER_ALG_DES:
            status[chnnel_id].alg = "DES ";
            break;
        case HI_CIPHER_ALG_3DES:
            status[chnnel_id].alg = "3DES";
            break;
        case HI_CIPHER_ALG_AES:
            status[chnnel_id].alg = "AES ";
            break;
        case HI_CIPHER_ALG_SM1:
            status[chnnel_id].alg = "SM1 ";
            break;
        case HI_CIPHER_ALG_SM4:
            status[chnnel_id].alg = "SM4 ";
            break;
        case HI_CIPHER_ALG_DMA:
            status[chnnel_id].alg = "DMA ";
            break;
        default:
            status[chnnel_id].alg = "BUTT";
            break;
    }
}

static hi_void drv_symc_proc_status_set_mode(symc_chn_status *status, hi_u32 chnnel_id, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    switch (ctrl.bits.sym_chn_alg_mode) {
        case HI_CIPHER_WORK_MODE_ECB:
            status[chnnel_id].mode = "ECB ";
            break;
        case HI_CIPHER_WORK_MODE_CBC:
            status[chnnel_id].mode = "CBC ";
            break;
        case HI_CIPHER_WORK_MODE_CFB:
            status[chnnel_id].mode = "CFB ";
            break;
        case HI_CIPHER_WORK_MODE_OFB:
            status[chnnel_id].mode = "OFB ";
            break;
        case HI_CIPHER_WORK_MODE_CTR:
            status[chnnel_id].mode = "CTR ";
            break;
        case HI_CIPHER_WORK_MODE_CCM:
            status[chnnel_id].mode = "CCM ";
            break;
        case HI_CIPHER_WORK_MODE_GCM:
            status[chnnel_id].mode = "GCM ";
            break;
        default:
            status[chnnel_id].mode = "BUTT";
            break;
    }
}

static hi_void drv_symc_proc_status_set_key_len(symc_chn_status *status, hi_u32 chnnel_id, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    if (ctrl.bits.sym_chn_alg_sel == HI_CIPHER_ALG_AES) {
        switch (ctrl.bits.sym_chn_key_length) {
            case HI_CIPHER_KEY_AES_128BIT:
                status[chnnel_id].klen = AES_KEY_128BIT;
                break;
            case HI_CIPHER_KEY_AES_192BIT:
                status[chnnel_id].klen = AES_KEY_192BIT;
                break;
            case HI_CIPHER_KEY_AES_256BIT:
                status[chnnel_id].klen = AES_KEY_256BIT;
                break;
            default:
                status[chnnel_id].klen = 0;
                break;
        }
    } else if (ctrl.bits.sym_chn_alg_sel == HI_CIPHER_ALG_DES) {
        status[chnnel_id].klen = DES_KEY_SIZE;
    } else if (ctrl.bits.sym_chn_alg_sel == HI_CIPHER_ALG_3DES) {
        switch (ctrl.bits.sym_chn_key_length) {
            case HI_CIPHER_KEY_DES_3KEY:
                status[chnnel_id].klen = TDES_KEY_192BIT;
                break;
            case HI_CIPHER_KEY_DES_2KEY:
                status[chnnel_id].klen = TDES_KEY_128BIT;
                break;
            default:
                status[chnnel_id].klen = 0;
                break;
        }
    } else if (ctrl.bits.sym_chn_alg_sel == HI_CIPHER_ALG_SM1) {
        status[chnnel_id].klen = SM1_AK_EK_SIZE + SM1_SK_SIZE;
    } else if (ctrl.bits.sym_chn_alg_sel == HI_CIPHER_ALG_SM4) {
        status[chnnel_id].klen = SM4_KEY_SIZE;
    } else {
        status[chnnel_id].klen = 0;
    }
}

static hi_void drv_symc_proc_status_set_key_sel(symc_chn_status *status, hi_u32 chnnel_id, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    if (ctrl.bits.sym_chn_key_sel) {
        status[chnnel_id].ksrc = "HW";
    } else {
        status[chnnel_id].ksrc = "SW";
    }
}

static hi_void drv_symc_proc_status_set_addr(symc_chn_status *status, hi_u32 chnnel_id)
{
    hi_u32 addr;

    /* get data in */
    if (chnnel_id != 0) {
        addr = chann_cipher_in_buf_rptr(chnnel_id);
        status[chnnel_id].inaddr = symc_read(addr);
    } else {
        status[0].inaddr = CHAN0_CIPHER_DIN;
    }

    /* get data out */
    if (chnnel_id != 0) {
        addr = chann_cipher_out_buf_rptr(chnnel_id);
        status[chnnel_id].outaddr = symc_read(addr);
    } else {
        status[0].outaddr = CHAN0_CIPHER_DOUT;
    }
}

hi_s32 drv_symc_proc_status(symc_chn_status *status)
{
    hi_u32 addr = 0;
    chann_chipher_ctrl ctrl;
    cipher_int_raw stIntRaw;
    cipher_int_en  stIntEn;
    chann_cipher_in_node_cfg stInNode;
    hi_u32 val;
    hi_u32 i, j;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        drv_symc_proc_status_set_open(status, i);

        /* get cipher ctrl */
        if (i != 0) {
            addr = chann_cipher_ctrl(i);
        } else {
            addr = CHAN0_CIPHER_CTRL;
        }

        ctrl.u32 = symc_read(addr);
        status[i].decrypt = ctrl.bits.sym_chn_decrypt;
        drv_symc_proc_status_set_alg(status, i, ctrl.u32);
        drv_symc_proc_status_set_mode(status, i, ctrl.u32);
        drv_symc_proc_status_set_key_len(status, i, ctrl.u32);
        drv_symc_proc_status_set_key_sel(status, i, ctrl.u32);
        drv_symc_proc_status_set_addr(status, i);

        for (j = 0; j < WORD_WIDTH; j++) {
            val = symc_read(chann_cipher_ivout(i) + j * WORD_WIDTH);
            hex2str(status[i].iv + j * BYTE_BITS, 2, (hi_u8)(val & MAX_LOW_8BITS)); /* 2byte */
            hex2str(status[i].iv + j * BYTE_BITS + 2, 2, (hi_u8)((val >> SHIFT_8BITS) & MAX_LOW_8BITS)); /* 2, 2byte */
            hex2str(status[i].iv + j * BYTE_BITS + 4, 2, (hi_u8)((val >> SHIFT_16BITS) & MAX_LOW_8BITS)); /* 4, 2byte */
            hex2str(status[i].iv + j * BYTE_BITS + 6, 2, (hi_u8)((val >> SHIFT_24BITS) & MAX_LOW_8BITS)); /* 6, 2byte */
        }

        /* get INT RAW status */
        stIntRaw.u32 = symc_read(CIPHER_INT_RAW);
        status[i].inraw = (stIntRaw.bits.cipher_chn_ibuf_raw >> i) & 0x1;
        status[i].outraw = (stIntRaw.bits.cipher_chn_obuf_raw >> i) & 0x1;

        /* get INT EN status */
        stIntEn.u32 = symc_read(CIPHER_INT_EN);
        status[i].intswitch = stIntEn.bits.cipher_nsec_int_en;
        status[i].inten = (stIntEn.bits.cipher_chn_ibuf_en >> i) & 0x1;
        status[i].outen = (stIntEn.bits.cipher_chn_obuf_en >> i) & 0x1;

        /* get INT_OINTCFG */
        addr = chann_cipher_in_node_cfg(i);
        stInNode.u32 = symc_read(addr);
        status[i].outintcnt = stInNode.bits.cipher_in_node_mpackage_int_level;
    }

    return HI_SUCCESS;
}
#endif  /* ****** proc function end ******* */

hi_void drv_symc_get_capacity(symc_capacity *capacity)
{
    hi_log_func_enter();

    /* the mode depend on alg, which limit to hardware
     * des/3des support ecb/cbc/cfb/ofb
     * aes support ecb/cbc/cfb/ofb/ctr/ccm/gcm
     * sm1 support ecb/cbc/cfb/ofb
     * sm4 support ecb/cbc/ctr
     */
    crypto_memset(capacity, sizeof(symc_capacity), 0,  sizeof(symc_capacity));

    /* AES */
    capacity->aes_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cfb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_ctr = CRYPTO_CAPACITY_SUPPORT;
#ifdef CHIP_AES_CCM_GCM_SUPPORT
    capacity->aes_ccm = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_gcm = CRYPTO_CAPACITY_SUPPORT;
#endif

    /* TDES */
    capacity->tdes_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_cfb = CRYPTO_CAPACITY_SUPPORT;

    /* DES */
    capacity->des_ecb  = CRYPTO_CAPACITY_SUPPORT;
    capacity->des_cbc  = CRYPTO_CAPACITY_SUPPORT;
    capacity->des_ofb  = CRYPTO_CAPACITY_SUPPORT;
    capacity->des_cfb  = CRYPTO_CAPACITY_SUPPORT;

    /* SM1 */
#ifdef CHIP_SYMC_SM1_SUPPORT
    capacity->sm1_ecb  = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm1_cbc  = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm1_ofb  = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm1_cfb  = CRYPTO_CAPACITY_SUPPORT;
#endif

    /* SM4 */
    capacity->sm4_ecb  = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm4_cbc  = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm4_ctr  = CRYPTO_CAPACITY_SUPPORT;

    /* DMA */
    capacity->dma = CRYPTO_CAPACITY_SUPPORT;
    return;
}

#endif /* End of CHIP_SYMC_VER_V200 */
