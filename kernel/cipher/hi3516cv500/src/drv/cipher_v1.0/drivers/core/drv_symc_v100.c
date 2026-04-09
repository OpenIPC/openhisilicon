/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv symc v100.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_symc_v100.h"
#include "drv_symc.h"

#ifdef CHIP_SYMC_VER_V100

#define MAX_NODE_SIZE                (0x100000 - 16)

/* ************************** Internal Structure Definition *************************** */
/* Define the struct of node */
typedef struct {
    hi_u32 phy_addr;  /* physics address of buffer */
    hi_u32 flags;     /* flag */
    hi_u32 length;    /* length of buffer */
    hi_u32 iv_addr;   /* physics address of iv */
} symc_entry;

/* Define the struct of iv in node */
typedef struct {
    hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD];  /* iv data */
} symc_entry_iv;

/* Define the context of cipher */
typedef struct {
    symc_entry *entry_in;     /* in node list */
    symc_entry *entry_out;    /* out node list */
    symc_entry_iv *entry_iv;  /* buffer to store the IV */
    hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD];    /* iv data from user. */

    /* iv usage flag, should be HI_CIPHER_IV_CHG_ONE_PACK
     * or HI_CIPHER_IV_CHG_ALL_PACK.
     */
    hi_u32 iv_flag;
    hi_u32 id_in;                       /* current in nodes index */
    hi_u32 id_out;                      /* current out nodes index */
    hi_u32 cnt;                         /* total count nodes to be computed */
    hi_u32 done;                        /* calculation finish flag. */
    crypto_queue_head  queue;           /* quene list */
    callback_symc_isr callback;         /* isr callback functon */
    callback_symc_destory destory;      /* destory callback functon */
    hi_void *ctx;                       /* params for isr callback functon */
} symc_hard_context;

/* Channel of cipher */
static channel_context g_symc_hard_channel[CRYPTO_HARD_CHANNEL_MAX];

/* dma memory of cipher node list. */
static crypto_mem   g_symc_dma;

/* symc already initialize or not */
static hi_u32 g_symc_initialize = HI_FALSE;

/* ****************************** API Code **************************** */
static hi_u32 drv_symc_done_try(hi_u32 chn_num)
{
    int_raw status;
    symc_hard_context *ctx = HI_NULL;
    hi_u32 process = 0;

    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    status.u32 = symc_read(REG_INT_RAW);
    status.bits.chn_obuf_raw &= 0x01 << chn_num; /* check interception */

    /* clear interception. */
    symc_write(REG_INT_RAW, status.u32);

    if (status.bits.chn_obuf_raw) {
        ctx = crypto_channel_get_context(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, chn_num);
        if (ctx == HI_NULL) {
            hi_log_error("crypto channel get context failed, ctx is null!\n");
            return HI_ERR_CIPHER_INVALID_POINT;
        }

        process = symc_read(reg_chann_ofull_cnt(chn_num));
        symc_write(reg_chann_iempty_cnt(chn_num), process);
        symc_write(reg_chann_ofull_cnt(chn_num),  process);
        ctx->cnt -= process;
    }

    return status.bits.chn_obuf_raw ? HI_TRUE : HI_FALSE;
}

#ifdef CRYPTO_OS_INT_SUPPORT
static hi_u32 drv_symc_done_notify(hi_void)
{
    symc_hard_context *ctx = HI_NULL;
    int_raw status;
    hi_u32 process = 0;
    hi_u32 i;

    status.u32 = symc_read(REG_INT_RAW);

    /* must clear interception before any operation */
    symc_write(REG_INT_RAW, status.u32);

    /* just process the valid channel. */
    status.bits.chn_obuf_raw &= CIPHER_HARD_CHANNEL_MASK;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((status.bits.chn_obuf_raw >> i) & 0x01) {
            /* received interception, tell hardware that
             * we already process the output node
             */
            ctx = crypto_channel_get_context(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, i);
            if (ctx == HI_NULL) {
                hi_log_error("crypto channel get context failed, ctx is null!\n");
                return HI_ERR_CIPHER_INVALID_POINT;
            }

            process = symc_read(reg_chann_ofull_cnt(i));
            symc_write(reg_chann_iempty_cnt(i), process);
            symc_write(reg_chann_ofull_cnt(i),  process);
            ctx->cnt -= process;
        }
    }

    return status.bits.chn_obuf_raw; /* mask */
}

/* symc interrupt process function */
static irqreturn_t drv_symc_interrupt_isr(hi_s32 irq, hi_void *dev_id)
{
    hi_u32 mask, i;
    symc_hard_context *ctx = HI_NULL;

    hi_log_debug("symc irq: %d\n", irq);

    /* get channel context. */
    mask = drv_symc_done_notify();

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((mask >> i) & 0x01) {
            ctx = crypto_channel_get_context(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, i);
            if (ctx == HI_NULL) {
                hi_log_error("Ctx get context failed.\n");
                return IRQ_NONE;
            }

            /* make sure all the loaded nodes are finished before start next compute. */
            if (ctx->cnt != 0) {
                continue;
            }

            /* continue to load other nodes */
            if ((ctx->callback) && (ctx->callback(ctx->ctx) == HI_FALSE)) {
                hi_log_debug("contiue to compute chn %d\n", i);
                drv_symc_start(i);
            } else { /* finished, no more nodes need to be load */
                ctx->done = HI_TRUE;
                hi_log_debug("chn %d wake up\n", i);
                crypto_queue_wait_up(&ctx->queue);
            }
        }
    }

    return IRQ_HANDLED;
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
    hi_log_debug("symc request irq, num %d, name %s\n", int_num, name);
    ret = crypto_request_irq(int_num, drv_symc_interrupt_isr, name);
    if (ret != HI_SUCCESS) {
        hi_log_error("Irq request failure, ret=%d, irq = %d", ret, int_num);
        hi_log_print_err_code(HI_ERR_CIPHER_REGISTER_IRQ);
        return ret;
    }

    /* initialize queue list. */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        crypto_queue_init(&((symc_hard_context *)g_symc_hard_channel[i].ctx)->queue);
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
    int_en  int_en;
    int_raw int_raw;

    hi_log_func_enter();

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, HI_NULL);

    if (int_valid == HI_FALSE) {
        int_en.u32 = symc_read(REG_INT_EN);

        /* The top interrupt switch only can be enable/disable by secure CPU. */
        int_en.bits.sec_int_en = 0;
        int_en.bits.int_en = 0;
        int_en.bits.chn_obuf_en &= ~CIPHER_HARD_CHANNEL_MASK;
        symc_write(REG_INT_EN, int_en.u32);
        hi_log_info("REG_INT_EN 0x%x\n", int_en.u32);
    } else {
        int_en.u32 = symc_read(REG_INT_EN);

        /* The top interrupt switch only can be enable/disable by secure CPU. */
        int_en.bits.sec_int_en = 1;
        int_en.bits.int_en = 1;
        int_en.bits.chn_obuf_en = CIPHER_HARD_CHANNEL_MASK;
        symc_write(REG_INT_EN, int_en.u32);
        hi_log_info("REG_INT_EN 0x%x\n", int_en.u32);
    }

    /* clear interception
     * the history of interception may trigge the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort.
     */
    int_raw.u32 = symc_read(REG_INT_RAW);
    int_raw.bits.chn_obuf_raw &= CIPHER_HARD_CHANNEL_MASK; /* clear valid channel. */
    symc_write(REG_INT_RAW, int_raw.u32);

    hi_log_func_exit();
}

static hi_s32 drv_symc_wait_irq(hi_u32 chn_num, hi_u32 timeout)
{
    hi_s32 ret;
    symc_hard_context *ctx = HI_NULL;

    ctx = crypto_channel_get_context(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, chn_num);
    if (ctx == HI_NULL) {
        hi_log_error("crypto channel get context failed, ctx is null, chn=%d\n", chn_num);
        hi_log_print_func_err(crypto_channel_get_context, HI_ERR_CIPHER_INVALID_POINT);
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /* wait interrupt */
    ret = crypto_queue_wait_timeout(ctx->queue, &ctx->done, timeout);
    if ((ret <= 0) && (ret != -ERESTARTSYS)) {
        hi_log_error("wait done timeout, chn=%d\n", chn_num);
        hi_log_print_func_err(crypto_queue_wait_timeout, HI_ERR_CIPHER_TIMEOUT);
        drv_symc_get_err_code(chn_num);
        return HI_ERR_CIPHER_TIMEOUT;
    }

    return HI_SUCCESS;
}

#endif

/* set symc entry */
static hi_void drv_symc_set_entry(hi_u32 chn, hi_u32 dma_addr, hi_u32 mmz_addr, hi_void *cpu_addr)
{
    hi_u32 i;
    symc_hard_context *ctx = (symc_hard_context *)g_symc_hard_channel[chn].ctx;

    hi_log_func_enter();

    /* set first in node addr. */
    hi_log_info("chn %d, entry in  dma addr 0x%x, mmz addr 0x%x, cpu addr 0x%p\n",
                chn, dma_addr, mmz_addr, cpu_addr);
    symc_write(reg_chann_src_lst_saddr(chn), mmz_addr);     /* node list must be mmz. */
    ctx->entry_in = cpu_addr;
    dma_addr += ENTRY_NODE_SIZE * SYMC_MAX_LIST_NUM;
    mmz_addr += ENTRY_NODE_SIZE * SYMC_MAX_LIST_NUM;
    cpu_addr = (hi_u8 *)cpu_addr + ENTRY_NODE_SIZE * SYMC_MAX_LIST_NUM;

    /* set first out node addr. */
    hi_log_info("chn %d, entry out dma addr 0x%x, mmz addr 0x%x, cpu addr 0x%p\n",
                chn, dma_addr, mmz_addr, cpu_addr);
    symc_write(reg_chann_dest_lst_saddr(chn), mmz_addr);    /* node list must be mmz. */
    ctx->entry_out = cpu_addr;
    dma_addr += ENTRY_NODE_SIZE * SYMC_MAX_LIST_NUM;
    mmz_addr += ENTRY_NODE_SIZE * SYMC_MAX_LIST_NUM;
    cpu_addr = (hi_u8 *)cpu_addr + ENTRY_NODE_SIZE * SYMC_MAX_LIST_NUM;

    /* set iv addr of in node. */
    hi_log_info("chn %d,        IV dma addr 0x%x, cpu addr 0x%p\n", chn, dma_addr, cpu_addr);
    for (i = 0; i < SYMC_MAX_LIST_NUM; i++) {
        /* set iv dma addr to each in node, IV dma addr may be mmz or mmu. */
        ctx->entry_in[i].iv_addr = dma_addr;
        dma_addr += SYMC_IV_MAX_SIZE;
    }
    ctx->entry_iv = cpu_addr;
    cpu_addr = (hi_u8 *)cpu_addr + SYMC_IV_MAX_SIZE * SYMC_MAX_LIST_NUM;

    /* the symc module may be running at setup period,
     * Here, we must continue running immediately after the last node.
     */
    ctx->id_in = symc_read(reg_chann_src_lst_raddr(chn));
    ctx->id_out = symc_read(reg_chann_dest_lst_raddr(chn));
    ctx->cnt = 0x00;
    hi_log_info("chn %d, id in %d, out %d\n\n", chn, ctx->id_in, ctx->id_out);

    /* set total node list Num */
    hi_log_debug("set total node list Num\n");
    symc_write(reg_chann_ibuf_num(chn), SYMC_MAX_LIST_NUM);
    symc_write(reg_chann_obuf_num(chn), SYMC_MAX_LIST_NUM);

    hi_log_func_exit();

    return;
}

#ifdef CRYPTO_SMMU_SUPPORT

/* set smmu */
static hi_void drv_symc_smmu_bypass(hi_void)
{
    chann_rd_dat_addr_smmu_bypass rd_mmu;
    chann_wr_dat_addr_smmu_bypass wr_mmu;

    hi_log_func_enter();

    rd_mmu.u32 = symc_read(CHANn_RD_DAT_ADDR_SMMU_BYPASS);
    wr_mmu.u32 = symc_read(CHANn_WR_DAT_ADDR_SMMU_BYPASS);

    /* bypass: 1-use mmz, 0-use mmu. */
    rd_mmu.bits.chann_rd_dat_addr_smmu_bypass &= ~(CIPHER_HARD_CHANNEL_MASK >> 1);
    wr_mmu.bits.chann_wr_dat_addr_smmu_bypass &= ~(CIPHER_HARD_CHANNEL_MASK >> 1);

    symc_write(CHANn_RD_DAT_ADDR_SMMU_BYPASS, rd_mmu.u32);
    symc_write(CHANn_WR_DAT_ADDR_SMMU_BYPASS, wr_mmu.u32);

    hi_log_info("CHANn_RD_DAT_ADDR_SMMU_BYPASS 0x%x\n", rd_mmu.u32);
    hi_log_info("CHANn_WR_DAT_ADDR_SMMU_BYPASS 0x%x\n", wr_mmu.u32);

    hi_log_func_exit();

    return;
}

#ifdef CRYPTO_OS_INT_SUPPORT
/* smmu int enable */
static hi_void drv_symc_smmint_enable(hi_void)
{
    smmu_scr scr;
    smmu_int int_en;

    hi_log_func_enter();

    /* enable smmu interception and disable bypass. */
    scr.u32 = smmu_read(REG_MMU_GLOBAL_CTR_ADDR);
    scr.bits.glb_bypass = 0;
    scr.bits.int_en = 1;
    smmu_write(REG_MMU_GLOBAL_CTR_ADDR, scr.u32);
    hi_log_info("REG_MMU_GLOBAL_CTR_ADDR 0x%x\n", scr.u32);

    /* enable interception. */
    if (crypto_is_sec_cpu()) {
        int_en.u32 = smmu_read(REG_MMU_INTMAS_S);
        int_en.bits.ints_tlbmiss = 0;
        int_en.bits.ints_ptw_trans = 0;
        int_en.bits.ints_tlbinvalid = 0;
        smmu_write(REG_MMU_INTMAS_S, int_en.u32);
        hi_log_info("REG_MMU_INTMAS_S 0x%x\n", int_en.u32);
    } else {
        int_en.u32 = smmu_read(REG_MMU_INTMASK_NS);
        int_en.bits.ints_tlbmiss = 0;
        int_en.bits.ints_ptw_trans = 0;
        int_en.bits.ints_tlbinvalid = 0;
        smmu_write(REG_MMU_INTMASK_NS, int_en.u32);
        hi_log_info("REG_MMU_INTMASK_NS 0x%x\n", int_en.u32);
    }

    hi_log_func_exit();

    return;
}
#endif

/* smmu int disable */
static hi_void drv_symc_smmu_int_disable(hi_void)
{
    smmu_scr scr;
    smmu_int int_en;

    hi_log_func_enter();

    /* enable smmu interception and disable bypass. */
    scr.u32 = smmu_read(REG_MMU_GLOBAL_CTR_ADDR);
    scr.bits.glb_bypass = 0;
    scr.bits.int_en = 0;
    smmu_write(REG_MMU_GLOBAL_CTR_ADDR, scr.u32);
    hi_log_info("REG_MMU_GLOBAL_CTR_ADDR 0x%x\n", scr.u32);

    /* disable secure interception */
    int_en.u32 = smmu_read(REG_MMU_INTMAS_S);
    int_en.bits.ints_tlbmiss = 1;
    int_en.bits.ints_ptw_trans = 1;
    int_en.bits.ints_tlbinvalid = 1;
    smmu_write(REG_MMU_INTMAS_S, int_en.u32);
    hi_log_info("REG_MMU_INTMAS_S 0x%x\n", int_en.u32);

    /* disable non-secure interception */
    int_en.u32 = smmu_read(REG_MMU_INTMASK_NS);
    int_en.bits.ints_tlbmiss = 1;
    int_en.bits.ints_ptw_trans = 1;
    int_en.bits.ints_tlbinvalid = 1;
    smmu_write(REG_MMU_INTMASK_NS, int_en.u32);
    hi_log_info("REG_MMU_INTMASK_NS 0x%x\n", int_en.u32);

    hi_log_func_exit();

    return;
}

/* smmu set base address */
static hi_void drv_symc_smmu_base_addr(hi_void)
{
    hi_u64 err_raddr = 0;
    hi_u64 err_waddr = 0;
    hi_u64 table_addr = 0;

    hi_log_func_enter();

    /* get table base addr from system api */
    smmu_get_table_addr(&err_raddr, &err_waddr, &table_addr);

    if (crypto_is_sec_cpu()) {
#ifdef CHIP_SMMU_VER_V200
        smmu_write(REG_MMU_ERR_RDADDR_H_S, 0x00); /* default error read high addr. */
        smmu_write(REG_MMU_ERR_WRADDR_H_S, 0x00); /* default error write high addr. */
        smmu_write(REG_MMU_SCB_TTBR_H,     0x00); /* smmu page table high addr. */
#endif
        smmu_write(REG_MMU_ERR_RDADDR_S, (hi_u32)err_raddr); /* default error read addr. */
        smmu_write(REG_MMU_ERR_WRADDR_S, (hi_u32)err_waddr); /* default error write addr. */
        smmu_write(REG_MMU_SCB_TTBR, (hi_u32)table_addr);    /* smmu page table addr. */
    } else {
#ifdef CHIP_SMMU_VER_V200
        smmu_write(REG_MMU_ERR_RDADDR_H_NS, 0x00);  /* default error read high addr. */
        smmu_write(REG_MMU_ERR_WRADDR_H_NS, 0x00);  /* default error write high addr. */
        smmu_write(REG_MMU_CB_TTBR_H,       0x00);  /* smmu page table high addr. */
#endif
        smmu_write(REG_MMU_ERR_RDADDR_NS, (hi_u32)err_raddr);   /* default error read addr. */
        smmu_write(REG_MMU_ERR_WRADDR_NS, (hi_u32)err_waddr);   /* default error write addr. */
        smmu_write(REG_MMU_CB_TTBR, (hi_u32)table_addr);        /* smmu page table addr. */
    }

    hi_log_func_exit();
}
#endif

/* set secure channel,
 * non-secure CPU can't change the value of SEC_CHN_CFG,
 * so non-secure CPU call this function will do nothing.
 */
static hi_void drv_symc_enable_secure(hi_void)
{
    sec_chn_cfg sec;

    hi_log_func_enter();

    /* The REG_SEC_CHN_CFG only can be set by secure CPU. */
    sec.u32 = symc_read(REG_SEC_CHN_CFG);
    sec.bits.sec_chn_cfg = CIPHER_HARD_CHANNEL_MASK;
    symc_write(REG_SEC_CHN_CFG, sec.u32);
    hi_log_info("REG_SEC_CHN_CFG 0x%x\n", sec.u32);

    hi_log_func_exit();
}

static hi_void drv_symc_print_last_node(hi_u32 chn_num)
{
    symc_entry *in = HI_NULL;
    symc_entry *out = HI_NULL;
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = (symc_hard_context *)g_symc_hard_channel[chn_num].ctx;

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
                 chn_num, in->phy_addr, in->length, out->phy_addr, out->length);

    hi_log_func_exit();
}

static hi_void drv_symc_print_status(hi_u32 chn_num)
{
    int_raw    raw;
    int_status status;
    int_en     enable;
    sec_chn_cfg cfg;

    hi_log_func_enter();

    hi_log_error("reg_chann_src_lst_saddr         : 0x%x\n", symc_read(reg_chann_src_lst_saddr(chn_num)));
    hi_log_error("reg_chann_src_lst_raddr         : 0x%x\n", symc_read(reg_chann_src_lst_raddr(chn_num)));
    hi_log_error("reg_chann_ibuf_num              : 0x%x\n", symc_read(reg_chann_ibuf_num(chn_num)));
    hi_log_error("chnn_src_addr                  : 0x%x\n", symc_read(chnn_src_addr(chn_num)));
    hi_log_error("reg_chann_dest_lst_saddr        : 0x%x\n", symc_read(reg_chann_dest_lst_saddr(chn_num)));
    hi_log_error("reg_chann_dest_lst_raddr        : 0x%x\n", symc_read(reg_chann_dest_lst_raddr(chn_num)));
    hi_log_error("reg_chann_obuf_num              : 0x%x\n", symc_read(reg_chann_obuf_num(chn_num)));
    hi_log_error("reg_chann_dest_addr             : 0x%x\n", symc_read(reg_chann_dest_addr(chn_num)));
    hi_log_error("chann_cipher_ctrl               : 0x%x\n", symc_read(reg_chann_cipher_ctrl(chn_num)));

    raw.u32    = symc_read(REG_INT_RAW);
    status.u32 = symc_read(REG_INT_STATUS);
    enable.u32 = symc_read(REG_INT_EN);
    cfg.u32    = symc_read(REG_SEC_CHN_CFG);

    hi_log_error("\nsec_chn_cfg 0x%x, chn %u, nsec_int_en 0x%x, sec_int_en 0x%x, chn_obuf_en 0x%x, status 0x%x, "
                 "raw 0x%x\n",
                 (cfg.bits.sec_chn_cfg >> chn_num) & 0x01,
                 chn_num, enable.bits.int_en, enable.bits.sec_int_en,
                 (enable.bits.chn_obuf_en >> chn_num) & 0x01,
                 (status.bits.chn_obuf_int >> chn_num) & 0x01,
                 (raw.bits.chn_obuf_raw >> chn_num) & 0x01);

    hi_log_error("\nThe cause of time out may be:\n"
                 "\t1. SMMU address invalid\n"
                 "\t2. interrupt number or name incorrect\n"
                 "\t3. CPU type mismatching, request CPU and channel: %s\n",
                 crypto_is_sec_cpu() ? "secure" : "non-secure");

    /* a hi_void compile error when hi_log_error be defined to empty */
    crypto_unused(raw);
    crypto_unused(status);
    crypto_unused(enable);
    crypto_unused(cfg);

    hi_log_func_exit();
}

hi_void drv_symc_get_err_code(hi_u32 chn_num)
{
    hi_log_func_enter();

#ifdef CRYPTO_SMMU_SUPPORT
    {
        /* check error code
         * bit0: TLB miss
         * bit1: PTW transaciont error
         * bit2: TLB read invalid
         * bit3: TLB Write invalid
         */
        hi_u32 code = 0;

        /* read error code. */
        if (crypto_is_sec_cpu()) {
            code = smmu_read(REG_MMU_INTRAW_S);
            smmu_write(REG_MMU_INTRAW_S, code);
        } else {
            code = smmu_read(REG_MMU_INTRAW_NS);
            smmu_write(REG_MMU_INTRAW_NS, code);
        }
        hi_log_info("symc done, hardware error code 0x%x\n", code);

        if ((code >> SHIFT_3BITS) & 0x01) {
            hi_log_error("MMU Error, there is a TLB Write invalid generated during the translation process.\n");
        }
        if ((code >> SHIFT_2BITS) & 0x01) {
            hi_log_error("MMU Error, there is a TLB read invalid generated during the translation process.\n");
        }
        if ((code >> SHIFT_1BITS) & 0x01) {
            hi_log_error("MMU Error, the PTW transaciont receive an error response.\n");
        }
        if ((code) & 0x01) {
            hi_log_error("MMU Error, there is a TLB miss generated during the translation process.\n");
        }
    }
#endif

    /* print the inout buffer address. */
    drv_symc_print_last_node(chn_num);
    drv_symc_print_status(chn_num);

    hi_log_func_exit();
}

hi_void drv_symc_enrty_init(crypto_mem mem)
{
    hi_u32 i;
    hi_u32 dma_addr;
    hi_u32 mmz_addr;
    hi_void *cpu_addr = HI_NULL;

    hi_log_info("symc entry list configure\n");
    dma_addr = addr_l32(mem.dma_addr);
    mmz_addr = addr_l32(mem.mmz_addr);
    cpu_addr = mem.dma_virt;
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((CIPHER_HARD_CHANNEL_MASK >> i) & 0x01) { /* valid channel. */
            drv_symc_set_entry(i, dma_addr, mmz_addr, cpu_addr);
            dma_addr += CHN_LIST_SIZE; /* move to next channel. */
            mmz_addr += CHN_LIST_SIZE; /* move to next channel. */
            cpu_addr = (hi_u8 *)cpu_addr + CHN_LIST_SIZE; /* move to next channel. */
        }
    }
}

/* encrypt data using special chn. */
hi_s32 drv_cipher_aes_test(hi_void)
{
    hi_s32 ret;
    crypto_mem mem;
    cryp_symc_context ctx;
    hi_u32 chn, last, first, i;
    hi_u32 key[SYMC_KEY_MAX_SIZE_IN_WORD] = {0};
    const char *gold_enc = "\x66\xE9\x4B\xD4\xEF\x8A\x2C\x3B\x88\x4C\xFA\x59\xCA\x34\x2B\x2E";

    ret = crypto_mem_create(&mem, SEC_MMZ, "symc_data", AES_BLOCK_SIZE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_create, ret);
        return ret;
    }

    /* read last used channel. */
    last = symc_read(REG_HL_APP_LEN) & 0xFF;

    /* read the max valid channel. */
    for (i = CRYPTO_HARD_CHANNEL_MAX - 1; i > 0; i--) {
        if (CIPHER_HARD_CHANNEL_MASK & (0x1 << i)) {
            first = i;
            break;
        }
    }

    /* select a channel dfferent with last used channel. */
    if (last == first) {
        chn = first - 1;
    } else {
        chn = first;
    }
    hi_log_info("drv cipher aes test, first %u, last chn %u, use chn %u\n", first, last, chn);

    g_symc_hard_channel[chn].open = HI_TRUE;
    drv_symc_set_key(chn, key, sizeof(key), HI_FALSE);

    crypto_memset(&ctx, sizeof(ctx), 0, sizeof(ctx));
    ctx.hard_chn = chn;
    ctx.alg = SYMC_ALG_AES;
    ctx.mode = SYMC_MODE_ECB;
    ctx.width = SYMC_DAT_WIDTH_128;
    ctx.sm1_round = 0;
    ctx.hard_key = HI_FALSE;
    crypto_chk_err_exit(drv_symc_cfg(&ctx, HI_FALSE, SYMC_KEY_DEFAULT));

    crypto_memset(mem.dma_virt, AES_BLOCK_SIZE, 0, AES_BLOCK_SIZE);

    crypto_chk_err_exit(drv_symc_add_inbuf(chn, mem.dma_addr, AES_BLOCK_SIZE, SYMC_NODE_USAGE_NORMAL));
    crypto_chk_err_exit(drv_symc_add_outbuf(chn, mem.dma_addr, AES_BLOCK_SIZE, SYMC_NODE_USAGE_NORMAL));

    /* start running */
    crypto_chk_err_exit(drv_symc_start(chn));

    /* wait done */
    crypto_chk_err_exit(drv_symc_wait_done(chn, CRYPTO_TIME_OUT));

    if (memcmp(mem.dma_virt, gold_enc, AES_BLOCK_SIZE) != 0) {
        hi_log_print_err_code(HI_ERR_CIPHER_FAILED_INIT);
        ret = HI_ERR_CIPHER_FAILED_INIT;
        goto exit__;
    }

exit__:
    crypto_mem_destory(&mem);
    g_symc_hard_channel[chn].open = HI_FALSE;
    return ret;
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

    sec.u32 = symc_read(REG_SEC_CHN_CFG);

    /* change the secure type of chn0 */
    sec.bits.sec_chn_cfg ^= 0x01;
    symc_write(REG_SEC_CHN_CFG, sec.u32);

    /* read the secure type of chn0 */
    tmp.u32 = symc_read(REG_SEC_CHN_CFG);

    if (tmp.bits.sec_chn_cfg == sec.bits.sec_chn_cfg) {
        /* The REG_SEC_CHN_CFG only can be set by secure CPU
         * can write the cfg, must be secure CPU
         */
        secure =  HI_TRUE;

        /* recovery the secure type of chn0 */
        sec.bits.sec_chn_cfg ^= 0x01;
        symc_write(REG_SEC_CHN_CFG, sec.u32);
    }

    hi_log_info("secure type: 0x%x\n", secure);

    hi_log_func_exit();
    return secure;
}
#endif

static hi_s32 drv_symc_cfg_init_value(hi_void)
{
    hi_s32 ret;

    crypto_memset(&g_symc_dma, sizeof(g_symc_dma), 0, sizeof(g_symc_dma));
    hi_log_info("disable symc\n");
    module_disable(CRYPTO_MODULE_ID_SYMC);

    hi_log_info("enable symc\n");
    module_enable(CRYPTO_MODULE_ID_SYMC);

    hi_log_info("enable smmu\n");
    module_enable(CRYPTO_MODULE_ID_SMMU);

    hi_log_info("alloc memory for nodes list\n");
    ret = crypto_mem_create(&g_symc_dma, SEC_MMZ, "symc_node_list", SYMC_NODE_LIST_SIZE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_create, ret);
        module_disable(CRYPTO_MODULE_ID_SMMU);
        module_disable(CRYPTO_MODULE_ID_SYMC);
        return ret;
    }

    hi_log_info("symc secure channel configure\n");
    drv_symc_enable_secure();

    hi_log_info("symc entry list configure\n");
    drv_symc_enrty_init(g_symc_dma);

#ifdef CRYPTO_SMMU_SUPPORT
    hi_log_info("symc SMMU configure\n");
    drv_symc_smmu_int_disable();
    drv_symc_smmu_bypass();
    drv_symc_smmu_base_addr();
#endif

#ifdef CRYPTO_OS_INT_SUPPORT

#ifdef CRYPTO_SMMU_SUPPORT
    drv_symc_smmint_enable();
#endif

    hi_log_info("symc interrupt configure\n");
    drv_symc_set_interrupt();

    hi_log_info("symc register interrupt function\n");
    ret = drv_symc_register_interrupt();
    if (ret != HI_SUCCESS) {
        hi_log_error("error, register interrupt failed\n");
        hi_log_print_func_err(drv_symc_register_interrupt, ret);
        crypto_mem_destory(&g_symc_dma);
        module_disable(CRYPTO_MODULE_ID_SMMU);
        module_disable(CRYPTO_MODULE_ID_SYMC);
        return ret;
    }
#endif

    return HI_SUCCESS;
}

hi_s32 drv_symc_init(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    if (g_symc_initialize == HI_TRUE) {
        return HI_SUCCESS;
    }

    ret = crypto_channel_init(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, sizeof(symc_hard_context));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_channel_init, ret);
        return ret;
    }

    ret = drv_symc_cfg_init_value();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_cfg_init_value, ret);
        (hi_void)crypto_channel_deinit(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX);
        return ret;
    }

    /* because can't reset hardware under tee environment
     * the low channel(1 or 2) may be used by boot or loader,
     * DRV_CIPHER_Init() will change the address of first node list,
     * this may occur a hardware error to analyse address,
     * so here need to start anther channel to computing once,
     * it will force the hardware re-read the address of first node list
     */
    ret = drv_cipher_aes_test();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_cipher_aes_test, ret);
    }

    g_symc_initialize = HI_TRUE;
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_deinit(hi_void)
{
    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);

    crypto_mem_destory(&g_symc_dma);

    module_disable(CRYPTO_MODULE_ID_SYMC);
    module_disable(CRYPTO_MODULE_ID_SMMU);

    crypto_channel_deinit(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX);

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_symc_unregister_interrupt();
#endif

    g_symc_initialize = HI_FALSE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_resume(hi_void)
{
    hi_log_func_enter();

    hi_log_info("enable symc\n");
    module_enable(CRYPTO_MODULE_ID_SYMC);
    module_enable(CRYPTO_MODULE_ID_SMMU);
    module_disable(CRYPTO_MODULE_ID_SM4);

    /* must be called before set register of
     * - interrupt
     * - channel
     */
    drv_symc_enable_secure();

#ifdef CRYPTO_SMMU_SUPPORT
    drv_symc_smmu_int_disable();
    drv_symc_smmu_bypass();
    drv_symc_smmu_base_addr();
#endif

#ifdef CRYPTO_OS_INT_SUPPORT
#ifdef CRYPTO_SMMU_SUPPORT
    drv_symc_smmint_enable();
#endif
    drv_symc_set_interrupt();
#endif

    drv_symc_enrty_init(g_symc_dma);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_suspend(hi_void)
{
    hi_log_func_enter();

    module_disable(CRYPTO_MODULE_ID_SYMC);
    module_disable(CRYPTO_MODULE_ID_SMMU);

    hi_log_func_exit();
    return ;
}

hi_s32 drv_symc_alloc_chn(hi_u32 *chn_num)
{
    hi_s32 ret;
    hi_u32 list;
    hi_u32 chn = 0;
    hi_u32 resume = HI_FALSE;

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);

    ret = crypto_channel_alloc(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX,
                               CIPHER_HARD_CHANNEL_MASK, &chn);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_channel_alloc, ret);
        return ret;
    }

    /* hardware may be unexpected reset by other module or platform,
     * such as unexpected reset by fastboot after load tee image,
     * in this case, the hardware configuration will be reset,
     * here try to re-config the hardware.
     */
    list = symc_read(reg_chann_src_lst_saddr(chn));

#ifdef CRYPTO_SMMU_SUPPORT
    {
        hi_u32 base = 0;

        if (crypto_is_sec_cpu()) {
            base = smmu_read(REG_MMU_SCB_TTBR);
        } else {
            base = smmu_read(REG_MMU_CB_TTBR);
        }
        if ((list == 0) || (base == 0)) {
            resume = HI_TRUE;
        }
    }
#else
    if (list == 0) {
        resume = HI_TRUE;
    }
#endif

    if (resume == HI_TRUE) {
        /* smmu base address or node list address is zero
         * means hardware be unexpected reset
         */
        hi_log_warn("cipher module is not ready, try to resume it now...\n");
        ret = drv_symc_resume();
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_resume, ret);
            return ret;
        }
    }

    *chn_num = chn;
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_free_chn(hi_u32 chn_num)
{
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = crypto_channel_get_context(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, chn_num);
    if (ctx->destory != HI_NULL) {
        ctx->destory();
        ctx->destory = HI_NULL;
    }

    crypto_channel_free(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, chn_num);
    hi_log_func_exit();
}

hi_s32 drv_symc_set_iv(hi_u32 chn_num, hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD], hi_u32 ivlen, hi_u32 flag)
{
    hi_u32 i;
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    crypto_unused(ivlen);

    ctx = crypto_channel_get_context(g_symc_hard_channel,
                                     CRYPTO_HARD_CHANNEL_MAX, chn_num);
    if (ctx == HI_NULL) {
        hi_log_error("crypto channel get context failed, ctx is null!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /* copy iv data into channel context. */
    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        ctx->iv[i] = iv[i];
        hi_log_info("IV[%d] 0x%x\n", i, iv[i]);
    }
    ctx->iv_flag = flag;

    hi_log_info("iv_flag 0x%x\n", flag);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void drv_symc_get_iv(hi_u32 chn_num, hi_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD])
{
    hi_u32 i;

    hi_log_func_enter();

    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        iv[i] = symc_read(reg_chan_cipher_ivout(chn_num) + i * WORD_WIDTH);
    }

    hi_log_func_exit();
}

hi_void drv_symc_set_key(hi_u32 chn_num, hi_u32 key[SYMC_KEY_MAX_SIZE_IN_WORD], hi_u32 key_len, hi_u32 odd)
{
    hi_u32 i;

    hi_log_func_enter();

    if (key_len < SYMC_KEY_MAX_SIZE_IN_WORD * WORD_WIDTH) {
        return;
    }

    /* unsupport odd key */
    if (odd) {
        return;
    }

    for (i = 0; i < SYMC_KEY_MAX_SIZE_IN_WORD; i++) {
        symc_write(reg_cipher_key(chn_num) + i * WORD_WIDTH, key[i]);
    }

    hi_log_func_exit();
}

hi_void drv_symc_set_sm1_sk(hi_u32 chn_num, hi_u32 key[SYMC_SM1_SK_SIZE_IN_WORD], hi_u32 key_len)
{
    crypto_unused(chn_num);
    crypto_unused(key);
    crypto_unused(key_len);
}

static hi_void drv_symc_add_iv_to_node(symc_hard_context *ctx, hi_u32 *iv_flg, hi_u8 *iv, hi_u32 iv_len, hi_u32 *flgs)
{
    hi_u32 *node_iv = (hi_u32 *)iv;

    crypto_unused(iv_len);

    /* first node must set iv except ecb mode. */
    if (ctx->iv_flag == HI_CIPHER_IV_CHG_ONE_PACK) {
        *iv_flg = HI_TRUE;

        /* don't set iv any more. */
        ctx->iv_flag = 0;
    } else if (ctx->iv_flag == HI_CIPHER_IV_CHG_ALL_PACK) {
        *iv_flg = HI_TRUE;
    }

    /* set iv to node */
    if (node_iv != HI_NULL) {
        *flgs |= SYMC_BUF_LIST_FLAG_IVSET;
        node_iv[WORD_IDX_0] = ctx->iv[WORD_IDX_0];
        node_iv[WORD_IDX_1] = ctx->iv[WORD_IDX_1];
        node_iv[WORD_IDX_2] = ctx->iv[WORD_IDX_2];
        node_iv[WORD_IDX_3] = ctx->iv[WORD_IDX_3];
        hi_log_debug("iv[0] 0x%x\n", node_iv[WORD_IDX_0]);
        hi_log_debug("iv[1] 0x%x\n", node_iv[WORD_IDX_1]);
        hi_log_debug("iv[2] 0x%x\n", node_iv[WORD_IDX_2]);
        hi_log_debug("iv[3] 0x%x\n", node_iv[WORD_IDX_3]);
    }
}

hi_s32 drv_symc_add_inbuf(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size, symc_node_usage usage)
{
    symc_entry *entry = HI_NULL;
    hi_u32 *node_iv = HI_NULL;
    symc_hard_context *ctx = HI_NULL;
    hi_u32 iv_flag = HI_FALSE;
    hi_u32 length;

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    hi_log_chk_param_return(buf_size == 0);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = crypto_channel_get_context(g_symc_hard_channel, CRYPTO_HARD_CHANNEL_MAX, chn_num);
    hi_log_chk_param_return(ctx == HI_NULL);
    hi_log_chk_param_return(ctx->cnt >= SYMC_MAX_LIST_NUM);
    hi_log_chk_param_return(ctx->id_in >= SYMC_MAX_LIST_NUM);
    hi_log_chk_param_return(ctx->id_out >= SYMC_MAX_LIST_NUM);

    /* split the buf to mutlit node, as the max length of one node is 1M-16. */
    while (buf_size > 0) {
        if (ctx->cnt > SYMC_MAX_LIST_NUM) {
            hi_log_error("node count %d overflow, max %d\n",  ctx->cnt, SYMC_MAX_LIST_NUM);
            hi_log_print_err_code(HI_ERR_CIPHER_OVERFLOW);
            return HI_ERR_CIPHER_OVERFLOW;
        }

        if (buf_size > MAX_NODE_SIZE) {
            length = MAX_NODE_SIZE;
        } else {
            length = buf_size;
        }
        entry = &ctx->entry_in[ctx->id_in];
        node_iv = ctx->entry_iv[ctx->id_in].iv;
        iv_flag = HI_FALSE;

        /* set dma addr and size, only support 32bit ddr. */
        entry->phy_addr = addr_l32(buf_phy);
        entry->length = length;

        /* set flag. */
        entry->flags = SYMC_BUF_LIST_FLAG_DUMM | SYMC_BUF_LIST_FLAG_EOL;

        hi_log_info("add inbuf, chn %d, id %d, addr 0x%x, length 0x%x, iv_flag %d\n",
                    chn_num, ctx->id_in, entry->phy_addr, entry->length, ctx->iv_flag);

        drv_symc_add_iv_to_node(ctx, &iv_flag, (hi_u8 *)node_iv, sizeof(ctx->iv), &entry->flags);

        /* move to next node, reset to zero if overflow. */
        ctx->id_in++;
        if (ctx->id_in == SYMC_MAX_LIST_NUM) {
            ctx->id_in = 0;
        }

        /* total count of computed nodes add 1. */
        ctx->cnt++;

        addr_l32(buf_phy) += length;
        buf_size -= length;
    }
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_add_outbuf(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size, symc_node_usage usage)
{
    symc_entry *entry = HI_NULL;
    symc_hard_context *ctx = HI_NULL;
    hi_u32 length;

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);
    hi_log_chk_param_return(buf_size == 0);

    crypto_unused(usage);

    ctx = crypto_channel_get_context(g_symc_hard_channel,
                                     CRYPTO_HARD_CHANNEL_MAX, chn_num);
    if (ctx == HI_NULL) {
        hi_log_error("crypto channel get context failed, ctx is null!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /* split the buf to mutlit node, as the max length of one node is 1M-16. */
    while (buf_size > 0) {
        if (buf_size > MAX_NODE_SIZE) {
            length = MAX_NODE_SIZE;
        } else {
            length = buf_size;
        }
        entry = &ctx->entry_out[ctx->id_out];

        /* set dma addr and size, only support 32bit ddr. */
        entry->phy_addr = addr_l32(buf_phy);
        entry->length = length;

        hi_log_info("add outbuf, chn %d, id %d, addr 0x%x, length 0x%x\n",
                    chn_num, ctx->id_out, entry->phy_addr, entry->length);

        /* set flag. */
        entry->flags = SYMC_BUF_LIST_FLAG_DUMM | SYMC_BUF_LIST_FLAG_EOL;

        /* move to next node, reset to zero if overflow. */
        ctx->id_out++;
        if (ctx->id_out == SYMC_MAX_LIST_NUM) {
            ctx->id_out = 0;
        }

        addr_l32(buf_phy) += length;
        buf_size -= length;
    }
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_aead_ccm_add_n(hi_u32 chn_num, hi_u8 *nonce, hi_u32 nonce_len)
{
    hi_log_chk_init_err_return(g_symc_initialize);
    crypto_unused(chn_num);
    crypto_unused(nonce);
    crypto_unused(nonce_len);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 drv_aead_ccm_add_a(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size)
{
    hi_log_chk_init_err_return(g_symc_initialize);
    crypto_unused(chn_num);
    crypto_unused(buf_phy);
    crypto_unused(buf_size);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 drv_aead_gcm_add_a(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size)
{
    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    crypto_unused(chn_num);
    crypto_unused(buf_phy);
    crypto_unused(buf_size);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 drv_aead_get_tag(hi_u32 chn_num, hi_u32 *tag, hi_u32 tag_buf_len)
{
    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    crypto_unused(chn_num);
    crypto_unused(tag);
    crypto_unused(tag_buf_len);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 drv_aead_gcm_add_clen(hi_u32 chn_num, hi_u8 *clen, hi_u32 clen_len)
{
    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    crypto_unused(chn_num);
    crypto_unused(clen);
    crypto_unused(clen_len);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_void drv_symc_add_buf_usage(hi_u32 chn_num, hi_u32 in, symc_node_usage usage)
{
    crypto_unused(chn_num);
    crypto_unused(in);
    crypto_unused(usage);
}

hi_s32 drv_symc_node_check(symc_alg alg, symc_mode mode, hi_u32 klen, hi_u32 block_size, symc_multi_pack *pack)
{
    hi_u32 i, tail;

    hi_log_func_enter();

    hi_log_chk_param_return(block_size == 0);
    hi_log_chk_param_return(pack == 0);

    hi_log_chk_init_err_return(g_symc_initialize);
    crypto_unused(alg);
    crypto_unused(mode);
    crypto_unused(klen);
    crypto_unused(pack->in);
    crypto_unused(pack->out);
    crypto_unused(pack->usage);

    for (i = 0; i < pack->num; i++) {
        hi_log_debug("node %d, length 0x%x\n", i, pack->len[i]);
        if ((pack->len[i] % block_size) != 0) {
            /* The last block of CTR mode don't need align with block size,
             * but the hardware require the data size must align with block size,
             * as the MMZ/MMU size is align with page size(4K) by system,
             * here we can increase the last block size to align with block size.
             */
            if ((mode != SYMC_MODE_CTR) || (i != pack->num - 1)) {
                hi_log_error("error, node length must be multiple of block size %d.\n", block_size);
                hi_log_print_err_code(HI_ERR_CIPHER_INVALID_LEN);
                return HI_ERR_CIPHER_INVALID_LEN;
            }

            tail = pack->len[i] % block_size;
            if (tail != 0) {
                pack->len[i] += block_size - tail;
            }
        }

        /* each node length can't be zero. */
        if (pack->len[i] == 0) {
            hi_log_error("PKG len must large than 0.\n");
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_LEN);
            return HI_ERR_CIPHER_INVALID_LEN;
        }

        /* check overflow */
        if (pack->len[i] > addr_l32(pack->in[i]) + pack->len[i]) {
            hi_log_error("PKG len overflow.\n");
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_LEN);
            return HI_ERR_CIPHER_INVALID_LEN;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_cfg(cryp_symc_context *ctx, hi_u32 decrypt, symc_klen klen)
{
    chann_chipher_ctrl ctrl;

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    hi_log_chk_param_return(ctx->hard_chn >= CRYPTO_HARD_CHANNEL_MAX);
    hi_log_chk_param_return(ctx->alg > SYMC_ALG_AES);
    hi_log_chk_param_return(ctx->mode > SYMC_MODE_CTR);
    hi_log_chk_param_return(klen >= SYMC_KEY_LEN_COUNT);

    crypto_unused(ctx->sm1_round);

    ctrl.u32 = symc_read(reg_chann_cipher_ctrl(ctx->hard_chn));

    ctrl.bits.decrypt = decrypt;
    ctrl.bits.mode = ctx->mode;
    ctrl.bits.alg_sel = ctx->alg;
    ctrl.bits.width = ctx->width;
    ctrl.bits.key_length = klen;
    ctrl.bits.key_sel = ctx->hard_key;
    ctrl.bits.key_adder = ctx->hard_chn;
    ctrl.bits.weight = 1;

    symc_write(reg_chann_cipher_ctrl(ctx->hard_chn), ctrl.u32);
    hi_log_info("reg_chann_cipher_ctrl[%d] 0x%x\n", ctx->hard_chn, ctrl.u32);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_set_isr_callback(hi_u32 chn_num, callback_symc_isr callback, hi_void *ctx)
{
    symc_hard_context *hisi_ctx = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    hisi_ctx = crypto_channel_get_context(g_symc_hard_channel,
                                          CRYPTO_HARD_CHANNEL_MAX, chn_num);
    if (hisi_ctx == HI_NULL) {
        hi_log_error("crypto channel get context failed, hisi_ctx is null!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    hisi_ctx->callback = callback;
    hisi_ctx->ctx = ctx;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 drv_symc_start(hi_u32 chn_num)
{
    symc_hard_context *ctx = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);
    ctx = crypto_channel_get_context(g_symc_hard_channel,
                                     CRYPTO_HARD_CHANNEL_MAX, chn_num);
    hi_log_chk_param_return(ctx->cnt < 0x01);

    hi_log_info("symc start, chn %d, cnt %d\n", chn_num, ctx->cnt);

    ctx->done = HI_FALSE;

    /* read last used channel. */
    symc_write(REG_HL_APP_LEN, chn_num);

    /* start work, the count of nodes to be computed is ctx->cnt. */
    symc_write(reg_chann_int_ocntcfg(chn_num), ctx->cnt);
    symc_write(reg_chann_obuf_cnt(chn_num), ctx->cnt);
    symc_write(reg_chann_ibuf_cnt(chn_num), ctx->cnt);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 drv_symc_query_raw_interrupt(hi_u32 chn_num, hi_u32 timeout)
{
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
    if (timeout <= i) {
        hi_log_error("symc wait done timeout, chn=%d\n", chn_num);
        hi_log_print_func_err(crypto_queue_wait_timeout, HI_ERR_CIPHER_TIMEOUT);
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

    hi_log_func_enter();

    hi_log_chk_init_err_return(g_symc_initialize);
    hi_log_chk_param_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, HI_NULL);

    hi_log_info("symc wait done, chn %d\n", chn_num);

#ifdef CRYPTO_OS_INT_SUPPORT
    if (int_valid) {
        /* interrupt support, wait irq. */
        ret = drv_symc_wait_irq(chn_num, timeout);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_wait_irq, ret);
            return ret;
        }
    } else {
        /* interrupt unsupport, query the raw interrupt flag. */
        ret = drv_symc_query_raw_interrupt(chn_num, timeout);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_symc_query_raw_interrupt, ret);
            return ret;
        }
    }
#else
    /* interrupt unsupport, query the raw interrupt flag. */
    ret = drv_symc_query_raw_interrupt(chn_num, timeout);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_symc_query_raw_interrupt, ret);
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

    ctx = crypto_channel_get_context(g_symc_hard_channel,
                                     CRYPTO_HARD_CHANNEL_MAX, chn_num);
    if (ctx == HI_NULL) {
        hi_log_error("crypto channel get context failed, ctx is null!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ctx->destory = destory;

    hi_log_func_exit();
    return HI_SUCCESS;
}

/* ****** proc function begin ******* */
#if (1 == HI_PROC_SUPPORT)
static hi_void drv_symc_proc_alg(symc_chn_status *status, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    switch (ctrl.bits.alg_sel) {
        case HI_CIPHER_ALG_DES: {
            status->alg = "DES ";
            break;
        }
        case HI_CIPHER_ALG_3DES: {
            status->alg = "3DES";
            break;
        }
        case HI_CIPHER_ALG_AES: {
            status->alg = "AES ";
            break;
        }
        default: {
            status->alg = "BUTT";
            break;
        }
    }
}

static hi_void drv_symc_proc_mode(symc_chn_status *status, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    switch (ctrl.bits.mode) {
        case HI_CIPHER_WORK_MODE_ECB:
            status->mode = "ECB ";
            break;
        case HI_CIPHER_WORK_MODE_CBC:
            status->mode = "CBC ";
            break;
        case HI_CIPHER_WORK_MODE_CFB:
            status->mode = "CFB ";
            break;
        case HI_CIPHER_WORK_MODE_OFB:
            status->mode = "OFB ";
            break;
        case HI_CIPHER_WORK_MODE_CTR:
            status->mode = "CTR ";
            break;
        case HI_CIPHER_WORK_MODE_CCM:
            status->mode = "CCM ";
            break;
        case HI_CIPHER_WORK_MODE_GCM:
            status->mode = "GCM ";
            break;
        default:
            status->mode = "BUTT";
            break;
    }
}

static hi_void drv_symc_proc_klen(symc_chn_status *status, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    if (ctrl.bits.alg_sel == HI_CIPHER_ALG_AES) {
        switch (ctrl.bits.key_length) {
            case HI_CIPHER_KEY_AES_128BIT: {
                status->klen = AES_KEY_128BIT;
                break;
            }
            case HI_CIPHER_KEY_AES_192BIT: {
                status->klen = AES_KEY_192BIT;
                break;
            }
            case HI_CIPHER_KEY_AES_256BIT: {
                status->klen = AES_KEY_256BIT;
                break;
            }
            default: {
                status->klen = 0;
                break;
            }
        }
    } else if (ctrl.bits.alg_sel == HI_CIPHER_ALG_DES) {
        status->klen = DES_KEY_SIZE;
    } else if (ctrl.bits.alg_sel == HI_CIPHER_ALG_3DES) {
        switch (ctrl.bits.key_length) {
            case HI_CIPHER_KEY_DES_3KEY: {
                status->klen = TDES_KEY_192BIT;
                break;
            }
            case HI_CIPHER_KEY_DES_2KEY: {
                status->klen = TDES_KEY_128BIT;
                break;
            }
            default: {
                status->klen = 0;
                break;
            }
        }
    } else {
        status->klen = 0;
    }
}

static hi_void drv_symc_proc_key_src(symc_chn_status *status, hi_u32 ctrl_info)
{
    chann_chipher_ctrl ctrl;

    ctrl.u32 = ctrl_info;
    if (ctrl.bits.key_sel) {
        status->ksrc = "HW";
    } else {
        status->ksrc = "SW";
    }
}

hi_void drv_symc_proc_ctrl(symc_chn_status *status, chann_chipher_ctrl ctrl)
{
    status->decrypt = ctrl.bits.decrypt;
    drv_symc_proc_alg(status, ctrl.u32);
    drv_symc_proc_mode(status, ctrl.u32);
    drv_symc_proc_klen(status, ctrl.u32);
    drv_symc_proc_key_src(status, ctrl.u32);
    return;
}

hi_s32 drv_symc_proc_status(symc_chn_status *status)
{
    hi_u32 addr, val, i, j;
    chann_chipher_ctrl ctrl;
    int_raw stIntRaw;
    int_en stIntEn;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (g_symc_hard_channel[i].open == HI_TRUE) {
            status[i].open = "open ";
        } else {
            status[i].open = "close";
        }

        /* get cipher ctrl. */
        if (i != 0) {
            addr = reg_chann_cipher_ctrl(i);
        } else {
            addr = REG_CHAN0_CIPHER_CTRL;
        }
        ctrl.u32 = symc_read(addr);
        drv_symc_proc_ctrl(&status[i], ctrl);

        /* get data in */
        if (i != 0) {
            addr = reg_chann_src_lst_saddr(i);
            status[i].inaddr = symc_read(addr);
        } else {
            status[i].inaddr = REG_CHAN0_CIPHER_DIN;
        }

        /* get data out */
        if (i != 0) {
            addr = reg_chann_dest_lst_saddr(i);
            status[i].outaddr = symc_read(addr);
        } else {
            status[0].outaddr = REG_CHAN0_CIPHER_DOUT;
        }

        for (j = 0; j < WORD_WIDTH; j++) {
            val = symc_read(reg_chan_cipher_ivout(i) + j * WORD_WIDTH);
            hex2str(status[i].iv + j * BYTE_BITS, 2, (hi_u8)(val & MAX_LOW_8BITS)); /* 2bytes */
            hex2str(status[i].iv + j * BYTE_BITS + 2, 2, (hi_u8)((val >> SHIFT_8BITS) & MAX_LOW_8BITS)); /* 2, 2byte */
            hex2str(status[i].iv + j * BYTE_BITS + 4, 2, (hi_u8)((val >> SHIFT_16BITS) & MAX_LOW_8BITS)); /* 4, 2byte */
            hex2str(status[i].iv + j * BYTE_BITS + 6, 2, (hi_u8)((val >> SHIFT_24BITS) & MAX_LOW_8BITS)); /* 6, 2byte */
        }

        /* get INT RAW status */
        stIntRaw.u32 = symc_read(REG_INT_RAW);
        status[i].inraw = (stIntRaw.bits.chn_ibuf_raw >> i) & 0x1;
        status[i].outraw = (stIntRaw.bits.chn_obuf_raw >> i) & 0x1;

        /* get INT EN status */
        stIntEn.u32 = symc_read(REG_INT_EN);
        status[i].intswitch = stIntEn.bits.sec_int_en;
        status[i].inten = (stIntEn.bits.chn_ibuf_en >> i) & 0x1;
        status[i].outen = (stIntEn.bits.chn_obuf_en >> i) & 0x1;

        /* get INT_OINTCFG */
        addr = reg_chann_int_ocntcfg(i);
        status[i].outintcnt = symc_read(addr);
    }

    return HI_SUCCESS;
}
#endif  /* ****** proc function end ******* */

hi_void drv_symc_get_capacity(symc_capacity *capacity)
{
    hi_log_func_enter();

    crypto_memset(capacity, sizeof(symc_capacity), 0,  sizeof(symc_capacity));

    /* AES */
    capacity->aes_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cfb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_ctr = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cts = CRYPTO_CAPACITY_NONSUPPORT;

    /* TDES */
    capacity->tdes_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_cfb = CRYPTO_CAPACITY_SUPPORT;

    /* DES */
    capacity->des_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->des_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->des_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->des_cfb = CRYPTO_CAPACITY_SUPPORT;

    hi_log_func_exit();
}

#endif /* End of CHIP_SYMC_VER_V100 */
