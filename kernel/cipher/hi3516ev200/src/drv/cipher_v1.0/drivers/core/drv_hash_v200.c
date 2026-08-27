/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_hash_v200.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_hash_v200.h"
#include "drv_hash.h"

#ifdef CHIP_HASH_VER_V200

/*************************** Structure Definition ****************************/
/** \addtogroup     hash */
/** @{ */  /** <!-- [hash] */

/*! hash in entry list size */
#define HASH_NODE_SIZE             (4096)

/*! hash in entry list size */
#define HASH_NODE_LIST_SIZE        (HASH_NODE_SIZE * HASH_HARD_CHANNEL_CNT)

/*! hash node dpth */
#define HASH_MAX_DEPTH                      (127)

#define KLAD_KEY_USE_ERR         0x01
#define ALG_LEN_ERR              0x02
#define SMMU_PAGE_UNVLID         0x04
#define OUT_SMMU_PAGE_NOT_VALID  0x08
#define KLAD_KEY_WRITE_ERR       0x10

/*! Define the flag of node */
typedef enum {
    HASH_CTRL_NONE             = 0x00, /*!<  middle node */
    HASH_CTRL_HASH_IN_FIRST    = 0x01, /*!<  first node */
    HASH_CTRL_HASH_IN_LAST     = 0x02, /*!<  last node */
    HASH_CTRL_COUNT,
} HASH_CTRL_EN;

/*! spacc digest in entry struct which is defined by hardware, you can't change it */
struct hash_entry_in {
    hi_u32    spacc_cmd: 2;     /*!<  reserve */
    hi_u32    rev1: 6;          /*!<  reserve */
    hi_u32    hash_ctrl: 6;     /*!<  hash control flag*/
    hi_u32    rev2: 18;         /*!<  reserve */
    hi_u32    hash_start_addr;  /*!<  hash message address */
    hi_u32    hash_alg_length;  /*!<  hash message length */
    hi_u32    word1;            /*!<  reserve */
};

/*! Define the context of hash */
typedef struct {
    hash_mode  hash_alg;   /*!<  hash mode */
    struct hash_entry_in *entry_in; /*! spacc digest in entry struct */
    hi_u32 id_in;                  /*!< current hash nodes to be used */
    hi_u32 done;                   /*!<  calculation finish flag*/
    crypto_queue_head  queue;   /*!<  quene list */
} hash_hard_context;

/*! hash already initialize or not */
static hi_u32 hash_initialize = HI_FALSE;

/*! dma memory of hash node list*/
static crypto_mem   hash_dma;

/*! Channel of hash */
static channel_context hash_hard_channel[CRYPTO_HARD_CHANNEL_MAX];

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */  /** <!--[hash]*/

#ifdef HI_CIPHER_DEBUG
extern int dump_mem(void);
#endif

#ifdef CRYPTO_OS_INT_SUPPORT
static hi_u32 hash_done_notify(void)
{
    hash_int_raw    int_raw;
    hash_int_status int_st;
    hi_u32 chn_mask = 0;

    int_st.u32 = HASH_READ(HASH_INT_STATUS);
    int_raw.u32 = 0;

    /*just process the valid channel*/
    int_st.bits.hash_chn_oram_int &= HASH_HARD_CHANNEL_MASK;
    chn_mask = int_st.bits.hash_chn_oram_int;
    int_raw.bits.hash_chn_oram_raw = int_st.bits.hash_chn_oram_int;

    HI_LOG_DEBUG("int_st 0x%x, mask 0x%x\n", int_st.u32, chn_mask);

    /*Clean raw int*/
    HASH_WRITE(HASH_INT_RAW, int_raw.u32);

    return chn_mask;
}

static hi_u32 symc_done_test(void)
{
    cipher_int_status status;

    status.u32 = SYMC_READ(CIPHER_INT_STATUS);

    /*just process the valid channel*/
    status.bits.cipher_chn_obuf_int &= CIPHER_HARD_CHANNEL_MASK;

    return status.bits.cipher_chn_obuf_int; /* mask */
}

/*! hash interrupt process function */
static irqreturn_t hash_interrupt_isr(hi_s32 irq, void *devId)
{
    hi_u32 mask = 0, i = 0;
    hash_hard_context *ctx = HI_NULL;
    irqreturn_t ret = IRQ_HANDLED;

    CRYPTO_UNUSED(irq);

    mask = hash_done_notify();

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((mask >> i) & 0x01) {
            ctx = (hash_hard_context *)hash_hard_channel[i].ctx;
            ctx->done = HI_TRUE;
            HI_LOG_DEBUG("chn %d wake up\n", i);
            crypto_queue_wait_up(&ctx->queue);
        }
    }

    /* symc and hash use the sample interrupt number
     * so if symc has occur interrupt, we should return IRQ_NONE
     * to tell system continue to process the symc interrupt.
     */
    if (0 != symc_done_test()) {
        ret = IRQ_NONE;
    }

    return ret;
}

/*! hash register interrupt process function */
static hi_s32 drv_hash_register_interrupt(void)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 int_valid = 0, int_num = 0;
    hi_u32 i;
    const char *name;

    HI_LOG_FUNC_ENTER();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return HI_SUCCESS;
    }

    /* request irq */
    ret = crypto_request_irq(int_num, hash_interrupt_isr, name);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("Irq request failure, irq = %d", int_num);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_REGISTER_IRQ);
        return HI_ERR_CIPHER_REGISTER_IRQ;
    }

    /* initialize queue list*/
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        crypto_queue_init(&((hash_hard_context *)hash_hard_channel[i].ctx)->queue);
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/*! hash unregister interrupt process function */
static void drv_hash_unregister_interrupt(void)
{
    hi_u32 int_valid = 0, int_num = 0;
    const char *name;

    HI_LOG_FUNC_ENTER();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return;
    }

    /* free irq */
    HI_LOG_DEBUG("hash free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    HI_LOG_FUNC_EXIT();

    return;
}

/*! set interrupt */
static void hash_set_interrupt(void)
{
    hi_u32 int_valid = 0, int_num = 0;
    const char *name;
    hash_int_en hash_int_en;
    hash_int_raw hash_int_raw;

    HI_LOG_FUNC_ENTER();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        hash_int_en.u32 = HASH_READ(HASH_INT_EN);

        /*The top interrupt switch only can be enable/disable by secure CPU*/
        hash_int_en.bits.hash_int_en = 0;
        hash_int_en.bits.hash_sec_int_en = 0;
        hash_int_en.bits.hash_chn_oram_en &= ~HASH_HARD_CHANNEL_MASK;
        HASH_WRITE(HASH_INT_EN, hash_int_en.u32);
        HI_LOG_INFO("HASH_INT_EN: 0x%x\n", hash_int_en.u32);
    } else {
        hash_int_en.u32 = HASH_READ(HASH_INT_EN);

        /*The top interrupt switch only can be enable/disable by secure CPU*/
        hash_int_en.bits.hash_int_en = 1;
        hash_int_en.bits.hash_sec_int_en = 1;
        hash_int_en.bits.hash_chn_oram_en |= HASH_HARD_CHANNEL_MASK;
        HASH_WRITE(HASH_INT_EN, hash_int_en.u32);
        HI_LOG_INFO("HASH_INT_EN: 0x%x\n", hash_int_en.u32);
    }

    /* clear interception
     * the history of interception may trigge the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort.
     */
    hash_int_raw.u32 = SYMC_READ(HASH_INT_RAW);
    hash_int_raw.bits.hash_chn_oram_raw &= HASH_HARD_CHANNEL_MASK; /* clear valid channel */
    SYMC_WRITE(HASH_INT_RAW, hash_int_raw.u32);

    HI_LOG_FUNC_EXIT();

    return;
}
#endif

static hi_u32 hash_done_try(hi_u32 chn_num)
{
    hash_int_raw    int_raw;
    hi_u32 chn_mask = 0;

    int_raw.u32 = HASH_READ(HASH_INT_RAW);
    int_raw.bits.hash_chn_oram_raw &= 0x01 << chn_num;
    chn_mask = int_raw.bits.hash_chn_oram_raw;

    /*Clean raw int*/
    HASH_WRITE(HASH_INT_RAW, int_raw.u32);

    return chn_mask;
}

/*! set hash entry */
static void hash_set_entry(hi_u32 chn, compat_addr dma_addr, void *cpu_addr)
{
    hash_hard_context *ctx = (hash_hard_context *)hash_hard_channel[chn].ctx;
    chann_hash_int_node_cfg hash_in_cfg;

    /*set total num and start addr for hash in node*/
    hash_in_cfg.u32 = HASH_READ(CHANn_HASH_IN_NODE_CFG(chn));
    hash_in_cfg.bits.hash_in_node_total_num = HASH_MAX_DEPTH;
    HASH_WRITE(CHANn_HASH_IN_NODE_CFG(chn), hash_in_cfg.u32);
    HASH_WRITE(CHANn_HASH_IN_NODE_START_ADDR(chn), ADDR_L32(dma_addr));
    HASH_WRITE(CHANN_HASH_IN_NODE_START_HIGH(chn), ADDR_H32(dma_addr));
    HI_LOG_INFO("CHANn_HASH_IN_NODE_CFG[0x%x]: \t0x%x, PHY: 0x%x, VIA %p\n",
                CHANn_HASH_IN_NODE_CFG(chn), hash_in_cfg.u32, ADDR_L32(dma_addr), cpu_addr);

    ctx->entry_in = (struct hash_entry_in *)cpu_addr;
    ctx->id_in = hash_in_cfg.bits.hash_in_node_wptr;

    return;
}

/*! set smmu */
static void hash_smmu_bypass(void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    hash_in_smmu_en hash_in_smmu_en;

    hash_in_smmu_en.u32 = HASH_READ(HASH_IN_SMMU_EN);
    hash_in_smmu_en.bits.hash_in_chan_rd_dat_smmu_en |= HASH_HARD_CHANNEL_MASK >> 1;
    hash_in_smmu_en.bits.hash_in_chan_rd_node_smmu_en &= ~(HASH_HARD_CHANNEL_MASK >> 1);
    HASH_WRITE(HASH_IN_SMMU_EN, hash_in_smmu_en.u32);
    HI_LOG_INFO("HASH_IN_SMMU_EN[0x%x]  : 0x%x\n", HASH_IN_SMMU_EN, hash_in_smmu_en.u32);
#endif
}

/*! smmu set base address */
static hi_s32 drv_hash_smmu_base_addr(void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    hi_u64 err_raddr = 0;
    hi_u64 err_waddr = 0;
    hi_u64 table_addr = 0;

    /* get table base addr from system api */
    smmu_get_table_addr(&err_raddr, &err_waddr, &table_addr);

    if (crypto_is_sec_cpu()) {
        /*smmu page secure table addr*/
        HASH_WRITE(NORM_SMMU_START_ADDR, (hi_u32)table_addr);
        HI_LOG_INFO("NORM_SMMU_START_ADDR[0x%x]  : 0x%x\n", NORM_SMMU_START_ADDR, (hi_u32)table_addr);
    } else {
        /*smmu page nonsecure table addr*/
        HASH_WRITE(SEC_SMMU_START_ADDR, (hi_u32)table_addr);
        HI_LOG_INFO("SEC_SMMU_START_ADDR[0x%x]  : 0x%x\n", SEC_SMMU_START_ADDR, (hi_u32)table_addr);
    }
#endif

    return HI_SUCCESS;
}

/*! set secure channel,
 *  non-secure CPU can't change the value of SEC_CHN_CFG,
 *  so non-secure CPU call this function will do nothing.
 */
static void drv_hash_enable_secure(void)
{
    sec_chn_cfg sec_chn_cfg;

    sec_chn_cfg.u32 = HASH_READ(SEC_CHN_CFG);
    sec_chn_cfg.bits.hash_sec_chn_cfg |= HASH_HARD_CHANNEL_MASK;
    HASH_WRITE(SEC_CHN_CFG, sec_chn_cfg.u32);
    HI_LOG_INFO("SEC_CHN_CFG[0x%x]: 0x%x\n", SEC_CHN_CFG, sec_chn_cfg.u32);
}

static void hash_print_last_node(hi_u32 chn_num)
{
    struct hash_entry_in *in = HI_NULL;
    hash_hard_context *ctx = HI_NULL;

    ctx = (hash_hard_context *)hash_hard_channel[chn_num].ctx;

    /* get last in node info*/
    in = &ctx->entry_in[ctx->id_in];

    HI_LOG_ERROR("chn %d, src addr 0x%x, size 0x%x\n",
                 chn_num, in->hash_start_addr , in->hash_alg_length);
    CRYPTO_UNUSED(in);
}

static hi_s32 drv_hash_get_err_code(hi_u32 chn_num)
{
    hi_u32 code = 0;

    /* check error code
     * bit0: klad_key_use_err
     * bit1: alg_len_err
     * bit2: smmu_page_unvlid
     * bit3: out_smmu_page_not_valid
     * bit4: klad_key_write_err
     */

    /*read error code*/
    code = HASH_READ(CALC_ERR);

    if (code & KLAD_KEY_USE_ERR) {
        HI_LOG_ERROR("hash error: klad_key_use_err, chn %d !!!\n", chn_num);
    }
    if (code & ALG_LEN_ERR) {
        HI_LOG_ERROR("hash error: alg_len_err, chn %d !!!\n", chn_num);
    }
    if (code & SMMU_PAGE_UNVLID) {
        HI_LOG_ERROR("hash error: smmu_page_unvlid, chn %d !!!\n", chn_num);
    }
    if (code & OUT_SMMU_PAGE_NOT_VALID) {
        HI_LOG_ERROR("symc error: out_smmu_page_not_valid, chn %d !!!\n", chn_num);
    }
    if (code & KLAD_KEY_WRITE_ERR) {
        HI_LOG_ERROR("symc error: klad_key_write_err, chn %d !!!\n", chn_num);
    }

    /*print the inout buffer address*/
    if (code) {
        hash_print_last_node(chn_num);
#ifdef HI_CIPHER_DEBUG
        tee_hal_backtraces();
        dump_mem();
#endif
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    return HI_SUCCESS;
}

void hash_enrty_init(crypto_mem mem)
{
    hi_u32 i;
    compat_addr mmz_addr;
    void *cpu_addr = HI_NULL;

    HI_LOG_INFO("symc entry list configure\n");
    ADDR_U64(mmz_addr) = ADDR_U64(mem.mmz_addr);
    cpu_addr = mem.dma_virt;
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((HASH_HARD_CHANNEL_MASK >> i) & 0x01) { /*valid channel*/
            hash_set_entry(i, mmz_addr, cpu_addr);
            ADDR_U64(mmz_addr) += HASH_NODE_SIZE; /* move to next channel */
            cpu_addr = (hi_u8 *)cpu_addr + HASH_NODE_SIZE; /* move to next channel */
        }
    }
    return;
}

hi_s32 drv_hash_init(void)
{
    hi_s32 ret = HI_FAILURE;
    hi_s32 ret_error = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    if (HI_TRUE == hash_initialize) {
        HI_LOG_FUNC_EXIT();
        return HI_SUCCESS;
    }

    ret = crypto_channel_init(hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX, sizeof(hash_hard_context));
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, hash channel list init failed\n");
        HI_LOG_PRINT_FUNC_ERR(crypto_channel_init, ret);
        return ret;
    }

    HI_LOG_INFO("enable hash\n");
    module_enable(CRYPTO_MODULE_ID_HASH);

    HI_LOG_INFO("alloc memory for nodes list\n");
    ret = hash_mem_create(&hash_dma, SEC_MMZ, "hash_node_list", HASH_NODE_LIST_SIZE);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, malloc ddr for hash nodes list failed\n");
        HI_LOG_PRINT_FUNC_ERR(hash_mem_create, ret);
        goto __error1;
    }
    HI_LOG_INFO("HASH DMA buffer, MMU 0x%x, MMZ 0x%x, VIA %p, size 0x%x\n",
                ADDR_L32(hash_dma.dma_addr), ADDR_L32(hash_dma.mmz_addr),
                hash_dma.dma_virt, hash_dma.dma_size);

    HI_LOG_INFO("hash entry list configure\n");
    hash_enrty_init(hash_dma);

    HI_LOG_INFO("hash SMMU configure\n");
    hash_smmu_bypass();
    drv_hash_smmu_base_addr();

    HI_LOG_INFO("hash secure channel configure\n");
    drv_hash_enable_secure();

#ifdef CRYPTO_OS_INT_SUPPORT
    HI_LOG_INFO("hash interrupt configure\n");
    hash_set_interrupt();

    HI_LOG_INFO("hash register interrupt function\n");
    ret = drv_hash_register_interrupt();
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, register interrupt failed\n");
        HI_LOG_PRINT_FUNC_ERR(drv_hash_register_interrupt, ret);
        ret_error = hash_mem_destory(&hash_dma);
        if (ret_error != HI_SUCCESS) {
            HI_LOG_ERROR("error, hash dma crypto mem destory failed.\n");
            HI_LOG_PRINT_FUNC_ERR(hash_mem_destory, ret_error);
        }
        goto __error1;
    }
#endif

    hash_initialize = HI_TRUE;
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;

__error1:
    module_disable(CRYPTO_MODULE_ID_HASH);
    ret_error = crypto_channel_deinit(hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX);
    if (ret_error != HI_SUCCESS) {
        HI_LOG_ERROR("error, crypto channel deinit failed.\n");
        HI_LOG_PRINT_FUNC_ERR(crypto_channel_deinit, ret_error);
    }

    return ret;
}

hi_s32 drv_hash_deinit(void)
{
    hi_s32 ret = HI_FAILURE;
    HI_LOG_FUNC_ENTER();

    if (HI_FALSE == hash_initialize) {
        HI_LOG_FUNC_EXIT();
        return HI_SUCCESS;
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_hash_unregister_interrupt();
#endif

    ret = hash_mem_destory(&hash_dma);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hash_mem_destory, ret);
        return ret;
    }
    module_disable(CRYPTO_MODULE_ID_HASH);
    ret = crypto_channel_deinit(hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(crypto_channel_deinit, ret);
        return ret;
    }

    hash_initialize = HI_FALSE;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

void drv_hash_resume(void)
{
    HI_LOG_FUNC_ENTER();

    HI_LOG_INFO("enable hash\n");
    module_enable(CRYPTO_MODULE_ID_HASH);

    HI_LOG_INFO("hash entry list configure\n");
    hash_enrty_init(hash_dma);

#ifdef CRYPTO_OS_INT_SUPPORT
    HI_LOG_INFO("hash interrupt configure\n");
    hash_set_interrupt();
#endif

    HI_LOG_INFO("hash SMMU configure\n");
    hash_smmu_bypass();
    drv_hash_smmu_base_addr();

    HI_LOG_INFO("hash secure channel configure\n");
    drv_hash_enable_secure();

    HI_LOG_FUNC_EXIT();

    return;
}

void drv_hash_suspend(void)
{
    HI_LOG_FUNC_ENTER();
    HI_LOG_FUNC_EXIT();

    return;
}

/* wait hash ready */
static hi_s32 drv_hash_wait_ready(hi_u32 chn_num)
{
    hi_u32 int_valid = 0, int_num = 0;
    hi_u32 errcode = 0;
    hi_u32 i;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, HI_NULL);

#ifdef CRYPTO_OS_INT_SUPPORT
    /* interrupt support, wait irq*/
    if (int_valid) {
        hash_hard_context *ctx = HI_NULL;

        ctx = (hash_hard_context *)hash_hard_channel[chn_num].ctx;

        ret = crypto_queue_wait_timeout(ctx->queue, &ctx->done, CRYPTO_TIME_OUT);
        if ((0x00 < ret) || (-ERESTARTSYS == ret)) {
            ret = HI_SUCCESS;
        } else {
            HI_LOG_ERROR("wait done timeout, chn=%d, ret %d\n", chn_num, ret);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_TIMEOUT);
            ret = HI_ERR_CIPHER_TIMEOUT;
        }
    } else /* interrupt unsupport, query the raw interrupt flag*/
#endif
    {
        for (i = 0; i < CRYPTO_TIME_OUT; i++) {
            if (hash_done_try(chn_num)) {
                break;
            }

            if (MS_TO_US >= i) {
                crypto_udelay(1);  /* short waitting for 1000 us */
            } else {
                crypto_msleep(1);  /* long waitting for 5000 ms*/
            }
        }
        if (CRYPTO_TIME_OUT <= i) {
            HI_LOG_ERROR("hash wait done timeout, chn=%d\n", chn_num);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_TIMEOUT);
            ret = HI_ERR_CIPHER_TIMEOUT;
        } else {
            ret = HI_SUCCESS;
        }
    }

    if (ret != HI_SUCCESS) {
        errcode = drv_hash_get_err_code(chn_num);
        HI_LOG_ERROR("hard error code: 0x%x\n", errcode);
        CRYPTO_UNUSED(errcode);
    }

    HI_LOG_FUNC_EXIT();
    return ret;
}

static void hash_addbuf(hi_u32 chn_num, compat_addr buf_phy, hi_u32 buf_size)
{
    hash_hard_context *ctx = HI_NULL;
    hi_u32 id = 0, size = 0;
    void *addr = HI_NULL;

    ctx = (hash_hard_context *)hash_hard_channel[chn_num].ctx;

    /* clean in entry */
    id = ctx->id_in++;
    addr = &ctx->entry_in[id];
    size = sizeof(struct hash_entry_in);

    crypto_memset(addr, size, 0, size);

    /* set addr and length */
    ctx->entry_in[id].spacc_cmd = 0x00;
    ctx->entry_in[id].hash_start_addr = ADDR_L32(buf_phy);
    ctx->entry_in[id].word1 = ADDR_H32(buf_phy);
    ctx->entry_in[id].hash_alg_length = buf_size;
    ctx->entry_in[id].hash_ctrl = HASH_CTRL_HASH_IN_FIRST | HASH_CTRL_HASH_IN_LAST;
    ctx->id_in %= HASH_MAX_DEPTH;
    HI_LOG_INFO("add digest in buf: id %d, addr 0x%x, len 0x%x\n",
                id, ADDR_L32(buf_phy), buf_size);

    return;
}

hi_s32 drv_hash_config(hi_u32 chn_num, hash_mode mode, hi_u32 state[HASH_RESULT_MAX_SIZE_IN_WORD])
{
    hash_hard_context *ctx = HI_NULL;
    chann_hash_ctrl hash_ctrl;
    hi_u32 i = 0;

    HI_LOG_CHECK_PARAM(hash_initialize != HI_TRUE);
    HI_LOG_CHECK_PARAM(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ctx = (hash_hard_context *)hash_hard_channel[chn_num].ctx;
    ctx->hash_alg = mode;

    /* Control */
    hash_ctrl.u32 = HASH_READ(CHANn_HASH_CTRL(chn_num));
    hash_ctrl.bits.hash_chn_mode = 0;
    hash_ctrl.bits.hash_chn_agl_sel = mode;
    HASH_WRITE(CHANn_HASH_CTRL(chn_num), hash_ctrl.u32);
    HI_LOG_INFO("CTRL: 0x%X\n", hash_ctrl.u32);

    /*Write last state*/
    for (i = 0; i < HASH_RESULT_MAX_SIZE_IN_WORD; i++) {
        HASH_WRITE(CHANn_HASH_STATE_VAL_ADDR(chn_num), i);
        HASH_WRITE(CHANn_HASH_STATE_VAL(chn_num), state[i]);
    }
    HI_LOG_INFO("state[0]: 0x%x\n", state[0]);

    return HI_SUCCESS;
}

hi_s32 drv_hash_start(hi_u32 chn_num, crypto_mem *mem, hi_u32 length)
{
    chann_hash_int_node_cfg in_node_cfg;
    hash_hard_context *ctx = HI_NULL;
    hi_u32 ptr = 0;
    crypto_mem *hash_dma_ctx = HI_NULL;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(hash_initialize != HI_TRUE);
    HI_LOG_CHECK_PARAM(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ctx = (hash_hard_context *)hash_hard_channel[chn_num].ctx;

    if (0 == length) {
        return HI_SUCCESS;
    }

    ctx->done = HI_FALSE;

    /* set message addr and length */
    hash_addbuf(chn_num, mem->dma_addr, length);

    /* configure in-node, only compute one nodes */
    in_node_cfg.u32 = HASH_READ(CHANn_HASH_IN_NODE_CFG(chn_num));
    ptr = in_node_cfg.bits.hash_in_node_wptr + 1;
    in_node_cfg.bits.hash_in_node_wptr = ptr % HASH_MAX_DEPTH;
    in_node_cfg.bits.hash_in_node_mpackage_int_level = 1;

    /* hash flush cache of hash mem and hash list buffer. */
    crypto_cpuc_flush_dcache_area(mem->dma_virt, length);
    hash_dma_ctx = &hash_dma;
    crypto_cpuc_flush_dcache_area(hash_dma_ctx->dma_virt, HASH_NODE_LIST_SIZE);

    /* Start */
    HASH_WRITE(CHANn_HASH_IN_NODE_CFG(chn_num), in_node_cfg.u32);
    HI_LOG_INFO("CHANn_HASH_IN_NODE_CFG: 0x%x\n", in_node_cfg.u32);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 drv_hash_wait_done(hi_u32 chn_num, hi_u32 *state)
{
    hi_u32 i = 0;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(hash_initialize != HI_TRUE);
    HI_LOG_CHECK_PARAM(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ret = drv_hash_wait_ready(chn_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(drv_hash_wait_ready, ret);
        return ret;
    }

    /* read hash result */
    for (i = 0; i < HASH_RESULT_MAX_SIZE_IN_WORD; i++) {
        HASH_WRITE(CHANn_HASH_STATE_VAL_ADDR(chn_num), i);
        state[i] = HASH_READ(CHANn_HASH_STATE_VAL(chn_num));
    }
    HI_LOG_DEBUG("digest[0]: 0x%x\n", state[0]);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

void drv_hash_reset(hi_u32 chn_num)
{
    CRYPTO_UNUSED(chn_num);
}

void drv_hash_get_capacity(hash_capacity *capacity)
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

/** @} */  /** <!-- ==== API declaration end ==== */

#endif //End of CHIP_HASH_VER_V200
