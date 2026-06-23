/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_symc.h"
#include "hal_spacc_reg.h"

#include "crypto_drv_common.h"

#ifndef crypto_memory_barrier
#define crypto_memory_barrier()
#endif

/* current cpu */
#define SPACC_CPU_CUR ((crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) ? SPACC_CPU_TEE : SPACC_CPU_REE)

/* ! \Numbers of nodes list */
#define CRYPTO_SYMC_MAX_LIST_NUM            2

#define CRYPTO_SYMC_IN_NODE_SIZE            (sizeof(crypto_symc_entry_in) * CRYPTO_SYMC_MAX_LIST_NUM)
#define CRYPTO_SYMC_OUT_NODE_SIZE           (sizeof(crypto_symc_entry_out) * CRYPTO_SYMC_MAX_LIST_NUM)
#define CRYPTO_SYMC_NODE_SIZE               (CRYPTO_SYMC_IN_NODE_SIZE + CRYPTO_SYMC_OUT_NODE_SIZE)
#define CRYPTO_SYMC_NODE_LIST_SIZE          (CRYPTO_SYMC_NODE_SIZE * CRYPTO_SYMC_HARD_CHANNEL_MAX)
#define CRYPTO_SYMC_WAIT_TIMEOUT            (6000000)

#define CRYPTO_SYMC_DFA_ENABLE_VAL          0x1
#define CRYPTO_SYMC_DFA_DISABLE_VAL         0xa

#define CACHE_LINE_SIZE                     32
#define SYMC_COMPAT_ERRNO(err_code)         HAL_COMPAT_ERRNO(ERROR_MODULE_SYMC, err_code)

/* ! spacc symc int entry struct which is defined by hardware, you can't change it */
typedef struct {
    td_u32 sym_first_node : 1;         /* !<  Indicates whether the node is the first node */
    td_u32 sym_last_node : 1;          /* !<  Indicates whether the node is the last node */
    td_u32 rev1 : 7;                   /* !<  reserve */
    td_u32 odd_even : 1;               /* !<  Indicates whether the key is an odd key or even key */
    td_u32 rev2 : 22;                  /* !<  reserve */
    td_u32 sym_alg_length;             /* !<  symc data length */
    td_u32 sym_start_addr;             /* !<  symc start low addr */
    td_u32 sym_start_high;             /* !<  symc start high addr */
    td_u32 iv[CRYPTO_AES_IV_SIZE_IN_WORD]; /* !<  symc IV */
} crypto_symc_entry_in;

/* ! spacc symc out entry struct which is defined by hardware, you can't change it */
typedef struct {
    td_u32 rev1;           /* !<  reserve */
    td_u32 sym_alg_length; /* !<  syma data length */
    td_u32 sym_start_addr; /* !<  syma start low addr */
    td_u32 sym_start_high; /* !<  syma start high addr */
} crypto_symc_entry_out;

typedef struct {
    crypto_symc_entry_in *entry_in;
    crypto_symc_entry_out *entry_out;
    td_u32 idx_in;
    td_u32 idx_out;
    td_u32 cnt_in;
    td_u32 cnt_out;
    td_void *wait;
    crypto_wait_timeout_interruptible wait_func;
    td_bool done;
    td_bool is_wait;
    td_u32 timeout_ms;
    td_u8 iv_ctr[CRYPTO_AES_IV_SIZE];
    td_u8 gcm_iv[CRYPTO_AES_IV_SIZE];
    td_phys_addr_t gcm_len_addr;
    hal_symc_config_t symc_config;
} crypto_symc_hard_context;

#if defined(CRYPTO_CONFIG_ROMBOOT_ENV)
static td_u8 g_symc_node_buffer[CRYPTO_SYMC_NODE_LIST_SIZE];
#endif

static crypto_symc_hard_context g_symc_hard_context[CRYPTO_SYMC_HARD_CHANNEL_MAX];
static td_s32 hal_cipher_symc_ccm_mac_update(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type);
static td_s32 hal_cipher_symc_gcm_mac_update(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type);

#define CRYPTO_SYMC_CLEAR_TIMEOUT   6000

static td_s32 hal_cipher_symc_out_node_done_try(td_u32 chn_num);

#if defined(CONFIG_SYMC_TIMEOUT_DEBUG)
static td_s32 hal_cipher_symc_debug(td_void)
{
    crypto_print("CRYPTO_SYMC_IN_NODE_SIZE is 0x%x\n", CRYPTO_SYMC_IN_NODE_SIZE);
    crypto_print("CRYPTO_SYMC_OUT_NODE_SIZE is 0x%x\n", CRYPTO_SYMC_OUT_NODE_SIZE);
    crypto_print("CRYPTO_SYMC_NODE_SIZE is 0x%x\n", CRYPTO_SYMC_NODE_SIZE);
    crypto_print("CRYPTO_SYMC_NODE_LIST_SIZE is 0x%x\n", CRYPTO_SYMC_NODE_LIST_SIZE);
    crypto_print("SPACC_IE is 0x%x\n", spacc_reg_read(SPACC_IE));
    crypto_print("SPACC_SYMC_CHN_LOCK is 0x%x\n", spacc_reg_read(SPACC_SYM_CHN_LOCK));
    crypto_print("OUT_SYM_CHAN_RAW_LAST_NODE_INT is 0x%x\n", spacc_reg_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT));
    crypto_print("OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN is 0x%x\n", spacc_reg_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN));
    crypto_print("OUT_SYM_CHAN_RAW_LEVEL_INT is 0x%x\n", spacc_reg_read(OUT_SYM_CHAN_RAW_LEVEL_INT));
    crypto_print("SYM_CHANN_RAW_INT is 0x%x\n", spacc_reg_read(SYM_CHANN_RAW_INT));
    crypto_print("SYM_CHANN_RAW_INT_EN is 0x%x\n", spacc_reg_read(SYM_CHANN_RAW_INT_EN));
    crypto_print("TEE_SYM_CALC_CTRL_CHECK_ERR is 0x%x\n", spacc_reg_read(TEE_SYM_CALC_CTRL_CHECK_ERR));
    crypto_print("REE_SYM_CALC_CTRL_CHECK_ERR is 0x%x\n", spacc_reg_read(REE_SYM_CALC_CTRL_CHECK_ERR));
    crypto_print("TEE_SYM_CALC_CTRL_CHECK_ERR_STATUS is 0x%x\n", spacc_reg_read(TEE_SYM_CALC_CTRL_CHECK_ERR_STATUS));
    crypto_print("REE_SYM_CALC_CTRL_CHECK_ERR_STATUS is 0x%x\n", spacc_reg_read(REE_SYM_CALC_CTRL_CHECK_ERR_STATUS));

    return TD_SUCCESS;
}

static td_void hal_cipher_symc_print_entry_in(const crypto_symc_entry_in *entry_in)
{
    crypto_unused(entry_in);
    crypto_print("The Details of Entry In:\n");
    crypto_print("sym_first_node is 0x%x\n", entry_in->sym_first_node);
    crypto_print("sym_last_node is 0x%x\n", entry_in->sym_last_node);
    crypto_print("odd_even is 0x%x\n", entry_in->odd_even);
    crypto_print("sym_alg_length is 0x%x\n", entry_in->sym_alg_length);
    crypto_print("sym_start_addr is 0x%x\n", entry_in->sym_start_addr);
    crypto_print("sym_start_high is 0x%x\n", entry_in->sym_start_high);
    crypto_dump_data("iv", (td_u8 *)entry_in->iv, sizeof(entry_in->iv));
}

static td_void hal_cipher_symc_print_entry_out(const crypto_symc_entry_out *entry_in)
{
    crypto_unused(entry_in);
    crypto_print("The Details of Entry Out:\n");
    crypto_print("sym_alg_length is 0x%x\n", entry_in->sym_alg_length);
    crypto_print("sym_start_addr is 0x%x\n", entry_in->sym_start_addr);
    crypto_print("sym_start_high is 0x%x\n", entry_in->sym_start_high);
}

static td_s32 hal_cipher_symc_debug_chn(td_u32 chn_num)
{
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_symc_entry_in *entry_in = TD_NULL;
    crypto_symc_entry_out *entry_out = TD_NULL;
    td_u32 i;
    td_u8 iv[CRYPTO_IV_LEN_IN_BYTES] = {0};

    hard_ctx = &g_symc_hard_context[chn_num];
    entry_in = hard_ctx->entry_in;
    entry_out = hard_ctx->entry_out;

    crypto_print("The Status of SYMC CHN %u:\n", chn_num);
    crypto_print("IN_SYM_CHN_NODE_START_ADDR_L is 0x%x\n", spacc_reg_read(IN_SYM_CHN_NODE_START_ADDR_L(chn_num)));
    crypto_print("IN_SYM_CHN_NODE_START_ADDR_H is 0x%x\n", spacc_reg_read(IN_SYM_CHN_NODE_START_ADDR_H(chn_num)));
    crypto_print("IN_SYM_CHN_NODE_LENGTH is 0x%x\n", spacc_reg_read(IN_SYM_CHN_NODE_LENGTH(chn_num)));

    crypto_print("OUT_SYM_CHN_NODE_START_ADDR_L is 0x%x\n", spacc_reg_read(OUT_SYM_CHN_NODE_START_ADDR_L(chn_num)));
    crypto_print("OUT_SYM_CHN_NODE_START_ADDR_H is 0x%x\n", spacc_reg_read(OUT_SYM_CHN_NODE_START_ADDR_H(chn_num)));
    crypto_print("OUT_SYM_CHN_NODE_LENGTH is 0x%x\n", spacc_reg_read(OUT_SYM_CHN_NODE_LENGTH(chn_num)));

    crypto_print("IN_SYM_CHN_CTRL is 0x%x\n", spacc_reg_read(IN_SYM_CHN_CTRL(chn_num)));
    crypto_print("IN_SYM_OUT_CTRL is 0x%x\n", spacc_reg_read(IN_SYM_OUT_CTRL(chn_num)));
    crypto_print("IN_SYM_CHN_KEY_CTRL is 0x%x\n", spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num)));

    crypto_print("OUT_SYM_CHN_NODE_WR_POINT is 0x%x\n", spacc_reg_read(OUT_SYM_CHN_NODE_WR_POINT(chn_num)));
    crypto_print("OUT_SYM_CHN_NODE_RD_POINT is 0x%x\n", spacc_reg_read(OUT_SYM_CHN_NODE_RD_POINT(chn_num)));

    crypto_print("IN_SYM_CHN_NODE_WR_POINT is 0x%x\n", spacc_reg_read(IN_SYM_CHN_NODE_WR_POINT(chn_num)));
    crypto_print("IN_SYM_CHN_NODE_RD_POINT is 0x%x\n", spacc_reg_read(IN_SYM_CHN_NODE_RD_POINT(chn_num)));

    for (i = 0; i < CRYPTO_SYMC_MAX_LIST_NUM; i++) {
        crypto_print("IDX_IN %u:\n", i);
        hal_cipher_symc_print_entry_in(&entry_in[i]);
    }
    for (i = 0; i < CRYPTO_SYMC_MAX_LIST_NUM; i++) {
        crypto_print("IDX_OUT %u:\n", i);
        hal_cipher_symc_print_entry_out(&entry_out[i]);
    }

    hal_cipher_symc_get_iv(chn_num, iv, sizeof(iv));
    crypto_dump_data("Current IV", iv, sizeof(iv));

    return TD_SUCCESS;
}
#else
#define hal_cipher_symc_debug(...)
#define hal_cipher_symc_debug_chn(...)
#endif

static void inner_symc_irq_enable(td_u8 chn_num, td_bool enable)
{
    td_u32 reg_val;

    reg_val = spacc_reg_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN);
    if (enable == TD_TRUE) {
        reg_val |= (1 << chn_num);
    } else {
        reg_val &= ~(1 << chn_num);
    }
    spacc_reg_write(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN, reg_val);
}

static void inner_symc_int_raw_enable(td_u8 chn_num, td_bool enable)
{
    td_u32 reg_val;

    reg_val = spacc_reg_read(SYM_CHANN_RAW_INT_EN);
    if (enable == TD_TRUE) {
        reg_val |= (1 << chn_num);
    } else {
        reg_val &= ~(1 << chn_num);
    }
    spacc_reg_write(SYM_CHANN_RAW_INT_EN, reg_val);
}

static td_s32 hal_cipher_symc_clear_channel(td_u32 chn_num)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 i;
    td_u32 chn_mask;
    spacc_sym_chn_clear_req symc_chn_clear = {0};
    spacc_int_raw_sym_clear_finish symc_chn_clear_finish = {0};

    chn_mask = 0x1 << chn_num;
    symc_chn_clear.u32 = spacc_reg_read(SPACC_SYM_CHN_CLEAR_REQ);
    symc_chn_clear.bits.sym_chn_clear_req |= chn_mask;
    spacc_reg_write(SPACC_SYM_CHN_CLEAR_REQ, symc_chn_clear.u32);

    for (i = 0; i < CRYPTO_SYMC_CLEAR_TIMEOUT; i++) {
        symc_chn_clear_finish.u32 = spacc_reg_read(SPACC_INT_RAW_SYM_CLEAR_FINISH);
        symc_chn_clear_finish.bits.int_raw_sym_clear_finish &= chn_mask;
        if (symc_chn_clear_finish.bits.int_raw_sym_clear_finish != 0) {
            spacc_reg_write(SPACC_INT_RAW_SYM_CLEAR_FINISH, symc_chn_clear.u32);
            break;
        }
        if (i <= MS_TO_US) {
            crypto_udelay(1); /* short waitting for 1000 us */
        } else {
            crypto_msleep(1); /* long waitting for 5000 ms */
        }
    }
    if (i >= CRYPTO_SYMC_CLEAR_TIMEOUT) {
        crypto_log_err("SYMC CHN %u Clear Channel Timeout\n", chn_num);
        return TD_FAILURE;
    }
    inner_symc_int_raw_enable(chn_num, TD_FALSE);

    return ret;
}

static td_void hal_cipher_symc_dfa_config(td_u32 chn_num, td_bool enable)
{
    symc_chann_dfa_en dfa_en = { 0 };

    dfa_en.u32 = spacc_reg_read(CHANN_CIPHER_DFA_EN(chn_num));
    if (enable == TD_TRUE) {
        dfa_en.bits.chann_dfa_en = CRYPTO_SYMC_DFA_ENABLE_VAL;
    } else {
        dfa_en.bits.chann_dfa_en = CRYPTO_SYMC_DFA_DISABLE_VAL;
    }
    spacc_reg_write(CHANN_CIPHER_DFA_EN(chn_num), dfa_en.u32);
}

static td_void hal_cipher_symc_set_entry_node(td_u32 chn_num, crypto_symc_hard_context *hard_ctx)
{
    in_sym_chn_node_length symc_in_node_length;
    out_sym_chn_node_length symc_out_node_length;
    td_ulong entry_in_phy = (td_ulong)crypto_get_phys_addr((td_void *)hard_ctx->entry_in);
    td_ulong entry_out_phy = (td_ulong)crypto_get_phys_addr((td_void *)hard_ctx->entry_out);

    /* set start addr for cipher in node and node length. */
    spacc_reg_write(IN_SYM_CHN_NODE_START_ADDR_L(chn_num), entry_in_phy);
    spacc_reg_write(IN_SYM_CHN_NODE_START_ADDR_H(chn_num), 0);

    symc_in_node_length.u32 = spacc_reg_read(IN_SYM_CHN_NODE_LENGTH(chn_num));
    symc_in_node_length.bits.sym_chn_node_length = CRYPTO_SYMC_MAX_LIST_NUM;
    spacc_reg_write(IN_SYM_CHN_NODE_LENGTH(chn_num), symc_in_node_length.u32);

    /* set start addr for cipher out node and node length. */
    spacc_reg_write(OUT_SYM_CHN_NODE_START_ADDR_L(chn_num), entry_out_phy);
    spacc_reg_write(OUT_SYM_CHN_NODE_START_ADDR_H(chn_num), 0);

    symc_out_node_length.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_LENGTH(chn_num));
    symc_out_node_length.bits.sym_chn_node_length = CRYPTO_SYMC_MAX_LIST_NUM;
    spacc_reg_write(OUT_SYM_CHN_NODE_LENGTH(chn_num), symc_out_node_length.u32);

    hard_ctx->idx_in = 0;
    hard_ctx->idx_out = 0;
    hard_ctx->cnt_in = 0;
    hard_ctx->cnt_out = 0;
    (td_void)memset_s(hard_ctx->entry_in, CRYPTO_SYMC_IN_NODE_SIZE, 0, CRYPTO_SYMC_IN_NODE_SIZE);
    (td_void)memset_s(hard_ctx->entry_out, CRYPTO_SYMC_OUT_NODE_SIZE, 0, CRYPTO_SYMC_OUT_NODE_SIZE);
}

static td_s32 hal_cipher_symc_wait_noout_done(td_u32 chn_num)
{
    td_u32 i = 0;
    td_s32 ret;
    crypto_hal_func_enter();

    for (i = 0; i < CRYPTO_SYMC_WAIT_TIMEOUT; i++) {
        ret = hal_cipher_symc_out_node_done_try(chn_num);
        if (ret == TD_SUCCESS) {
            break;
        }
        crypto_udelay(1);
    }

    if (i >= CRYPTO_SYMC_WAIT_TIMEOUT) {
        ret = SYMC_COMPAT_ERRNO(ERROR_HASH_CALC_TIMEOUT);
    }
    if (ret == SYMC_COMPAT_ERRNO(ERROR_HASH_CALC_TIMEOUT)) {
        crypto_log_err("symc wait done timeout, chn is %u\n", chn_num);
        hal_cipher_symc_debug();
        hal_cipher_symc_debug_chn(chn_num);
        return ret;
    }

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

static td_s32 priv_set_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len, in_node_type_e type)
{
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_symc_entry_in *entry_in = TD_NULL;

#if defined(CONFIG_CRYPTO_CHIP_HI3892WV100)
    crypto_chk_return((data_phys_addr % CRYPTO_WORD_WIDTH) != 0, SYMC_COMPAT_ERRNO(ERROR_SYMC_LEN_NOT_ALIGNED),
        "phys_addr should align to %u-Byte\n", CRYPTO_WORD_WIDTH);
#endif
    hard_ctx = &g_symc_hard_context[chn_num];
    entry_in = &hard_ctx->entry_in[hard_ctx->idx_in];

    entry_in->sym_first_node = (type & IN_NODE_TYPE_FIRST) ? 0x1 : 0;
    entry_in->sym_last_node = (type & IN_NODE_TYPE_LAST) ? 0x1 : 0;
    entry_in->sym_alg_length = data_len;
    entry_in->sym_start_addr = data_phys_addr;
    entry_in->sym_start_high = 0;
    return TD_SUCCESS;
}

static td_void inner_spacc_interrupt_enable(td_bool enable)
{
    spacc_ie cfg_val = { 0 };
    cfg_val.u32 = spacc_reg_read(SPACC_IE);
    if (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) {
        cfg_val.bits.spacc_ie_tee = enable;
    } else {
        cfg_val.bits.spacc_ie_ree = enable;
    }

    spacc_reg_write(SPACC_IE, cfg_val.u32);
}

td_s32 hal_cipher_symc_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 i;
    td_u8 *entry_buffer = TD_NULL;
    crypto_symc_hard_context *hard_ctx = TD_NULL;

    crypto_hal_func_enter();
    /* Node Buffer must be MMZ. */
#if defined(CRYPTO_CONFIG_ROMBOOT_ENV)
    entry_buffer = g_symc_node_buffer;
#else
    entry_buffer = crypto_malloc_mmz(CRYPTO_SYMC_NODE_LIST_SIZE, "cipher_symc_node");
    if (entry_buffer == TD_NULL) {
        crypto_log_err("crypto_malloc_mmz failed\n");
        return TD_FAILURE;
    }
#endif
    (td_void)memset_s(g_symc_hard_context, sizeof(g_symc_hard_context), 0, sizeof(g_symc_hard_context));
    for (i = 0; i < CRYPTO_SYMC_HARD_CHANNEL_MAX; i++) {
        hard_ctx = &g_symc_hard_context[i];
        hard_ctx->entry_in = (void *)(entry_buffer + i * CRYPTO_SYMC_NODE_SIZE);
        hard_ctx->entry_out = (void *)(entry_buffer + i * CRYPTO_SYMC_NODE_SIZE +
            CRYPTO_SYMC_IN_NODE_SIZE);
    }

    inner_spacc_interrupt_enable(TD_FALSE);

#if defined(CRYPTO_ERROR_ENV)
    if (CRYPTO_ERROR_ENV != ERROR_ENV_NOOS) {
        /* release all previously locked channels */
        for (i = 1; i < CRYPTO_SYMC_HARD_CHANNEL_MAX; i++) {
            (td_void)hal_cipher_symc_unlock_chn(i);
        }
    }
#endif

    crypto_hal_func_exit();
    return ret;
}

td_s32 hal_cipher_symc_deinit(td_void)
{
    td_u8 *entry_buffer = TD_NULL;
    td_u32 i;
    crypto_hal_func_enter();

    entry_buffer = (td_u8 *)g_symc_hard_context[0].entry_in;
    (td_void)memset_s(entry_buffer, CRYPTO_SYMC_NODE_LIST_SIZE, 0, CRYPTO_SYMC_NODE_LIST_SIZE);
#if !defined(CRYPTO_CONFIG_ROMBOOT_ENV)
    crypto_free_coherent(entry_buffer);
#endif
    (td_void)memset_s(g_symc_hard_context, sizeof(g_symc_hard_context), 0, sizeof(g_symc_hard_context));
    for (i = 0; i < CRYPTO_SYMC_HARD_CHANNEL_MAX; i++) {
        hal_cipher_symc_unlock_chn(i);
    }
    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_lock_chn(td_u32 chn_num)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 used;
    td_u32 chnn_who_used = 0;
    spacc_cpu_mask cpu_mask = SPACC_CPU_IDLE;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    in_sym_chn_ctrl in_ctrl;
    crypto_hal_func_enter();

    cpu_mask = (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) ? SPACC_CPU_TEE : SPACC_CPU_REE;

    used = spacc_reg_read(SPACC_SYM_CHN_LOCK);
    chnn_who_used = CHN_WHO_USED_GET(used, chn_num);
    if (chnn_who_used != SPACC_CPU_IDLE) {
        return TD_FAILURE;
    }
    CHN_WHO_USED_SET(used, chn_num, cpu_mask);
    spacc_reg_write(SPACC_SYM_CHN_LOCK, used);

    /* Read Back. */
    used = spacc_reg_read(SPACC_SYM_CHN_LOCK);
    chnn_who_used = CHN_WHO_USED_GET(used, chn_num);
    crypto_chk_return(chnn_who_used != cpu_mask, TD_FAILURE, "Lock SYMC CHN %u Failed\n", chn_num);

    ret = hal_cipher_symc_clear_channel(chn_num);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_clear_channel failed\n");
        hal_cipher_symc_unlock_chn(chn_num);
        return ret;
    }
    hard_ctx = &g_symc_hard_context[chn_num];
    hal_cipher_symc_set_entry_node(chn_num, hard_ctx);

    /* enable channel. */
    in_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_CTRL(chn_num));
    in_ctrl.bits.sym_chn_en = 1;
    if (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) {
        in_ctrl.bits.sym_chn_ss = SYMC_CFG_SECURE;
        in_ctrl.bits.sym_chn_ds = SYMC_CFG_SECURE;
    } else {
        in_ctrl.bits.sym_chn_ss = SYMC_CFG_NON_SECURE;
        in_ctrl.bits.sym_chn_ds = SYMC_CFG_NON_SECURE;
    }
    spacc_reg_write(IN_SYM_CHN_CTRL(chn_num), in_ctrl.u32);

    hal_cipher_symc_done_try(chn_num);
    hal_cipher_symc_out_node_done_try(chn_num);

#if defined(CRYPTO_DRV_DFA_ENABLE)
    /* DFA Enable */
    hal_cipher_symc_dfa_config(chn_num, TD_TRUE);
#else
    /* DFA Disable */
    hal_cipher_symc_dfa_config(chn_num, TD_FALSE);
#endif

    crypto_hal_func_exit();
    return ret;
}

td_s32 hal_cipher_symc_unlock_chn(td_u32 chn_num)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 used;
    td_u32 chnn_who_used = 0;
    spacc_cpu_mask cpu_mask = SPACC_CPU_IDLE;
    crypto_hal_func_enter();

    if (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) {
        cpu_mask = SPACC_CPU_TEE;
    } else {
        cpu_mask = SPACC_CPU_REE;
    }

    used = spacc_reg_read(SPACC_SYM_CHN_LOCK);
    chnn_who_used = CHN_WHO_USED_GET(used, chn_num);
    if (chnn_who_used != cpu_mask) {
        return TD_SUCCESS;
    }

    ret = hal_cipher_symc_clear_channel(chn_num);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_symc_clear_channel failed\n");
    }

    used = spacc_reg_read(SPACC_SYM_CHN_LOCK);
    CHN_WHO_USED_CLR(used, chn_num);
    spacc_reg_write(SPACC_SYM_CHN_LOCK, used);

    crypto_hal_func_exit();
    return ret;
}

td_s32 hal_cipher_symc_attach(td_u32 symc_chn_num, td_u32 keyslot_chn_num)
{
    in_sym_chn_key_ctrl in_key_ctrl;
    crypto_hal_func_enter();

    in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(symc_chn_num));
    /* Keyslot CHN Num. */
    in_key_ctrl.bits.sym_key_chn_id = keyslot_chn_num;
    spacc_reg_write(IN_SYM_CHN_KEY_CTRL(symc_chn_num), in_key_ctrl.u32);

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_set_iv(td_u32 chn_num, const td_u8 *iv, td_u32 iv_len)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_symc_entry_in *entry_in = TD_NULL;
    td_u32 idx;
    crypto_hal_func_enter();

#if defined(CRYPTO_SYMC_HAL_DEBUG_ENABLE)
    crypto_dump_data("iv", iv, iv_len);
#endif

    hard_ctx = &g_symc_hard_context[chn_num];
    idx = hard_ctx->idx_in;
    entry_in = &hard_ctx->entry_in[idx];

    ret = memcpy_s(entry_in->iv, sizeof(entry_in->iv), iv, iv_len);
    if (ret != TD_SUCCESS) {
        crypto_log_err("memcpy_s failed, ret is 0x%x\n", ret);
        return ret;
    }

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_get_iv(td_u32 chn_num, td_u8 *iv, td_u32 iv_len)
{
    td_u32 i;
    td_u32 iv_get[CRYPTO_AES_IV_SIZE_IN_WORD] = {0};
    crypto_hal_func_enter();
    (td_void)iv_len;

    for (i = 0; i < CRYPTO_AES_IV_SIZE_IN_WORD; i++) {
        iv_get[i] = spacc_reg_read(CHANN_CIPHER_IVOUT(chn_num) + i * CRYPTO_WORD_WIDTH);
    }

    (td_void)memcpy_s(iv, CRYPTO_IV_LEN_IN_BYTES, iv_get, CRYPTO_IV_LEN_IN_BYTES);

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

/* 1. Key Length. */
#define SYMC_KEY_64BIT_VAL      0
#define SYMC_KEY_128BIT_VAL     1
#define SYMC_KEY_192BIT_VAL     2
#define SYMC_KEY_256BIT_VAL     3

static crypto_table_item g_symc_key_length_table[] = {
    {
        .index = CRYPTO_SYMC_KEY_64BIT, .value = SYMC_KEY_64BIT_VAL
    },
    {
        .index = CRYPTO_SYMC_KEY_128BIT, .value = SYMC_KEY_128BIT_VAL
    },
#if defined(CRYPTO_IOT_V200)
    {
        .index = CRYPTO_SYMC_KEY_192BIT, .value = SYMC_KEY_192BIT_VAL
    },
#endif
    {
        .index = CRYPTO_SYMC_KEY_256BIT, .value = SYMC_KEY_256BIT_VAL
    },
};

/* 2. ALg Mode. */
#define SYMC_ALG_MODE_ECB_VAL           0x1
#define SYMC_ALG_MODE_CBC_VAL           0x3
#define SYMC_ALG_MODE_CTR_VAL           0x6
#define SYMC_ALG_MODE_OFB_VAL           0x7
#define SYMC_ALG_MODE_CFB_VAL           0x8
#define SYMC_ALG_MODE_GCTR_VAL          0xb
#define SYMC_ALG_MODE_CMAC_VAL          0xc
#define SYMC_ALG_MODE_CBC_NOOUT_VAL     0xd
#define SYMC_ALG_MODE_GCTR_NOOUT_VAL    0xe

static crypto_table_item g_symc_alg_mode_table[] = {
    {
        .index = CRYPTO_SYMC_WORK_MODE_ECB, .value = SYMC_ALG_MODE_ECB_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_CBC, .value = SYMC_ALG_MODE_CBC_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_CTR, .value = SYMC_ALG_MODE_CTR_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_OFB, .value = SYMC_ALG_MODE_OFB_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_CFB, .value = SYMC_ALG_MODE_CFB_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_CCM, .value = SYMC_ALG_MODE_CTR_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_GCM, .value = SYMC_ALG_MODE_GCTR_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_CBC_MAC, .value = SYMC_ALG_MODE_CBC_NOOUT_VAL
    },
    {
        .index = CRYPTO_SYMC_WORK_MODE_CMAC, .value = SYMC_ALG_MODE_CMAC_VAL
    }
};

/* 3. Alg. */
#define SYMC_ALG_AES_VAL        0x2
#define SYMC_ALG_SM4_VAL        0x5
#define SYMC_ALG_GHASH_VAL      0x6

static crypto_table_item g_symc_alg_table[] = {
    {
        .index = CRYPTO_SYMC_ALG_AES, .value = SYMC_ALG_AES_VAL
    },
    {
        .index = CRYPTO_SYMC_ALG_SM4, .value = SYMC_ALG_SM4_VAL
    },
};

/* 4. Bit Width. */
#define SYMC_ALG_BIT_WIDTH_128BIT   0
#define SYMC_ALG_BIT_WIDTH_64BIT    1
#define SYMC_ALG_BIT_WIDTH_8BIT     1

static crypto_table_item g_symc_bit_width_table[] = {
    {
        .index = CRYPTO_SYMC_BIT_WIDTH_128BIT, .value = SYMC_ALG_BIT_WIDTH_128BIT
    },
    {
        .index = CRYPTO_SYMC_BIT_WIDTH_64BIT, .value = SYMC_ALG_BIT_WIDTH_64BIT
    },
    {
        .index = CRYPTO_SYMC_BIT_WIDTH_8BIT, .value = SYMC_ALG_BIT_WIDTH_8BIT
    },
};

static td_s32 hal_cipher_symc_ccm_get_s0_and_iv_ctr(td_u32 chn_num, crypto_symc_hard_context *hard_ctx)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *head = TD_NULL;

    if (hard_ctx->symc_config.iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_START) {
        (td_void)memset_s(hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr), 0, sizeof(hard_ctx->iv_ctr));
        (td_void)memset_s(hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac),
            0, sizeof(hard_ctx->symc_config.iv_mac));

        head = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_ccm_head");
        crypto_chk_return(head == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");
        (td_void)memset_s(head, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);

        ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(head), CRYPTO_AES_BLOCK_SIZE_IN_BYTES,
            IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

        ret = hal_cipher_symc_add_out_node(chn_num, crypto_get_phys_addr(head), CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_out_node failed\n");

        ret = hal_cipher_symc_start(chn_num);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_start failed\n");

        ret = hal_cipher_symc_wait_done(chn_num, TD_FALSE);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_wait_done failed\n");

        ret = memcpy_s(hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0),
            head, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
        crypto_chk_goto(ret != EOK, exit_free, "memcpy_s failed\n");

        hal_cipher_symc_get_iv(chn_num, hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));

        hard_ctx->symc_config.iv_change_flag = CRYPTO_SYMC_CCM_IV_CHANGE_UPDATE;
#if defined(CRYPTO_CCM_TRACE_ENABLE)
        crypto_dump_data("s0", hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0));
        crypto_dump_data("iv_ctr", hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));
#endif
    }
exit_free:
    if (head != TD_NULL) {
        (td_void)memset_s(head, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
        crypto_free_coherent(head);
    }
    return ret;
}

static td_s32 gcm_inc(const td_u8 *input, td_u32 input_len, td_u8 *output, td_u32 output_len)
{
    td_u32 i = 0;

    if (input_len != output_len) {
        crypto_log_err("input_len must be equal to output_len for gcm_inc\n");
        return TD_FAILURE;
    }

    for (i = 0; i < input_len; i++) {
        output[i] = input[i];
    }

    for (i = input_len; i > input_len - CRYPTO_WORD_WIDTH; i--) {
        if (++(output[i - 1]) != 0) {
            break;
        }
    }
    return TD_SUCCESS;
}

static td_s32 hal_gcm_get_j0_by_iv(td_u32 chn_num, crypto_symc_hard_context *hard_ctx,
    const td_u8 *gcm_iv_ptr, td_u32 gcm_iv_len)
{
    td_s32 ret;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES * 2; /* 2: Store iv and Padding. */
    td_u32 iv_bits_len = 0;

    buffer = crypto_malloc_coherent(buffer_len, "cipher_gcm_head");
    crypto_chk_return(buffer == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");
    /* padding must be zeros. */
    (td_void)memset_s(buffer, buffer_len, 0, buffer_len);

    ret = memcpy_s(buffer, buffer_len, gcm_iv_ptr, gcm_iv_len);
    crypto_chk_goto(ret != EOK, exit_free, "memcpy_s failed\n");

    // Set iv total bits to last 4 bytes.
    iv_bits_len = gcm_iv_len * CRYPTO_BITS_IN_BYTE;
    iv_bits_len = crypto_cpu_to_be32(iv_bits_len);
    ret = memcpy_s(buffer + buffer_len - CRYPTO_WORD_WIDTH, CRYPTO_WORD_WIDTH,
        (td_u8 *)(&iv_bits_len), CRYPTO_WORD_WIDTH);
    crypto_chk_goto(ret != EOK, exit_free, "memcpy_s failed\n");

    ret = hal_cipher_symc_gcm_mac_update(chn_num, crypto_get_phys_addr(buffer), buffer_len,
        IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST | IN_NODE_TYPE_GCM_IV);
    crypto_chk_goto(ret != EOK, exit_free, "hal_cipher_symc_gcm_mac_update failed\n");

    // get J0
    ret = hal_cipher_symc_get_iv(chn_num, hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0));
    crypto_chk_goto(ret != EOK, exit_free, "hal_cipher_symc_get_iv failed\n");

exit_free:
    (td_void)memset_s(buffer, buffer_len, 0, buffer_len);
    crypto_free_coherent(buffer);
    return ret;
}

static td_s32 hal_cipher_symc_gcm_get_j0(td_u32 chn_num, crypto_symc_hard_context *hard_ctx)
{
    td_s32 ret;
    td_u32 i = 0;
    const td_u8 *gcm_iv_ptr = hard_ctx->symc_config.gcm_iv_ptr;
    td_u32 iv_len = hard_ctx->symc_config.gcm_iv_len;

    crypto_chk_return(gcm_iv_ptr == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "gcm_iv_ptr is NULL\n");
    /* Before Calculate j0, iv_ghash must be clear. */
    (td_void)memset_s(hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac), 0,
        sizeof(hard_ctx->symc_config.iv_mac));
    (td_void)memset_s(hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0),
        0, sizeof(hard_ctx->symc_config.iv0));
    /* If the IV length is 12 bytes, J0 = IV || 00000000 00000000 00000000 00000001 */
    if (iv_len == CRYPTO_GCM_SPECIAL_IV_BYTES) { // for 96 bits, J0 = IV || 00000001
        for (i = 0; i < iv_len; i++) {
            hard_ctx->symc_config.iv0[i] = gcm_iv_ptr[i];
        }
        hard_ctx->symc_config.iv0[CRYPTO_AES_IV_SIZE - 1] = 0x01; // j0[15] = 0x01
    } else { /* If the IV length is not 12 bytes, J0 = GHASH{IV || [00..00] || len(IV)} */
        ret = hal_gcm_get_j0_by_iv(chn_num, hard_ctx, gcm_iv_ptr, iv_len);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_gcm_get_j0_by_iv failed\n");
    }

    /* get the initial vertor by j0. */
    ret = gcm_inc(hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0),
        hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));
    crypto_chk_return(ret != TD_SUCCESS, ret, "gcm_inc failed\n");

#ifdef CRYPTO_GCM_TRACE_ENABLE
    crypto_dump_data("gcm_j0", hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0));
    crypto_dump_data("gcm_iv", hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));
#endif

    return ret;
}

static td_s32 inner_symc_config_key_ctrl(td_u32 chn_num, const hal_symc_config_t *symc_config)
{
    volatile td_s32 ret = TD_FAILURE;
    td_u32 reg_value = 0;
    in_sym_chn_key_ctrl in_key_ctrl = {0};
    td_bool is_decrypt = symc_config->is_decrypt;
    crypto_symc_alg symc_alg = symc_config->symc_alg;
    crypto_symc_work_mode work_mode = symc_config->work_mode;
    crypto_symc_key_length symc_key_length = symc_config->symc_key_length;
    crypto_symc_bit_width symc_bit_width = symc_config->symc_bit_width;

    in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));

    /* alg_decrypt. */
    in_key_ctrl.bits.sym_alg_decrypt = is_decrypt;

    /* alg_sel. */
    ret = crypto_get_value_by_index(g_symc_alg_table, crypto_array_size(g_symc_alg_table),
        symc_alg, &reg_value);
    crypto_chk_return(ret != TD_SUCCESS, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "Invalid Alg!\n");
    in_key_ctrl.bits.sym_alg_sel = reg_value;

    /* alg_mode. */
    ret = crypto_get_value_by_index(g_symc_alg_mode_table, crypto_array_size(g_symc_alg_mode_table),
        work_mode, &reg_value);
    crypto_chk_return(ret != TD_SUCCESS, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "Invalid Alg_Mode!\n");
    in_key_ctrl.bits.sym_alg_mode = reg_value;

    /* alg_key_len. */
    ret = crypto_get_value_by_index(g_symc_key_length_table, crypto_array_size(g_symc_key_length_table),
        symc_key_length, &reg_value);
    crypto_chk_return(ret != TD_SUCCESS, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "Invalid key_len!\n");
    in_key_ctrl.bits.sym_alg_key_len = reg_value;

    /* alg_data_width. */
    ret = crypto_get_value_by_index(g_symc_bit_width_table, crypto_array_size(g_symc_bit_width_table),
        symc_bit_width, &reg_value);
    crypto_chk_return(ret != TD_SUCCESS, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "Invalid alg_data_width!\n");
    in_key_ctrl.bits.sym_alg_data_width = reg_value;

    spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_config(td_u32 chn_num, const hal_symc_config_t *symc_config)
{
    td_s32 ret;
    in_sym_out_ctrl cipher_dma_ctrl = { 0 };
    crypto_symc_alg symc_alg = symc_config->symc_alg;
    crypto_symc_work_mode work_mode = symc_config->work_mode;
    crypto_symc_hard_context *hard_ctx = TD_NULL;

    crypto_hal_func_enter();

    /* dma enable. */
    cipher_dma_ctrl.u32 = spacc_reg_read(IN_SYM_OUT_CTRL(chn_num));
    if (symc_alg == CRYPTO_SYMC_ALG_DMA) {
        cipher_dma_ctrl.bits.sym_dma_copy = TD_TRUE;
        spacc_reg_write(IN_SYM_OUT_CTRL(chn_num), cipher_dma_ctrl.u32);
        return TD_SUCCESS;
    }
    cipher_dma_ctrl.bits.sym_dma_copy = TD_FALSE;
    spacc_reg_write(IN_SYM_OUT_CTRL(chn_num), cipher_dma_ctrl.u32);

    ret = inner_symc_config_key_ctrl(chn_num, symc_config);
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_symc_config_key_ctrl failed\n");

    hard_ctx = &g_symc_hard_context[chn_num];

    (td_void)memcpy_s(&(hard_ctx->symc_config), sizeof(hal_symc_config_t), symc_config, sizeof(hal_symc_config_t));

    /* For ccm, you should calculate s0 and iv_ctr first. */
    if (work_mode == CRYPTO_SYMC_WORK_MODE_CCM && symc_config->iv_change_flag == CRYPTO_SYMC_CCM_IV_CHANGE_START) {
        crypto_log_dbg("ccm calculate s0 and iv_ctr\n");
        hal_cipher_symc_ccm_get_s0_and_iv_ctr(chn_num, hard_ctx);
        hard_ctx->symc_config.iv_change_flag = CRYPTO_SYMC_CCM_IV_CHANGE_UPDATE;
    }
    /* for gcm, you should calculate j0 first. */
    /* you need get j0 only once. */
    if (work_mode == CRYPTO_SYMC_WORK_MODE_GCM && symc_config->iv_change_flag == CRYPTO_SYMC_GCM_IV_CHANGE_START) {
        crypto_log_dbg("gcm calculate j0.\n");
        ret = hal_cipher_symc_gcm_get_j0(chn_num, hard_ctx);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_gcm_get_j0 failed\n");
        hard_ctx->symc_config.iv_change_flag = CRYPTO_SYMC_GCM_IV_CHANGE_UPDATE;
    }

    crypto_hal_func_exit();

    return ret;
}

td_s32 hal_cipher_symc_get_module_info(crypto_symc_module_info *module_info)
{
    spacc_ie st_int_en;
 
    crypto_hal_func_enter();
    crypto_chk_return(module_info == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "module_info is NULL\n");
 
    /* module info. */
    module_info->sec_cpu = (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) ? 1 : 0;
    st_int_en.u32 = spacc_reg_read(SPACC_IE);
    module_info->int_en = (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) ? \
        st_int_en.bits.spacc_ie_tee : st_int_en.bits.spacc_ie_ree;
    module_info->err_code = spacc_reg_read(SYM_CALC_CTRL_CHECK_ERR(SPACC_CPU_CUR));
 
    crypto_hal_func_exit();
    return TD_SUCCESS;
}
 
td_s32 hal_cipher_symc_get_proc_info(td_u32 chn_num, crypto_symc_proc_info *proc_symc_info)
{
    td_s32 ret;
    td_u32 index;
 
    in_sym_chn_key_ctrl in_key_ctrl;
 
    out_sym_chan_raw_int out_raw_int;
    out_sym_chan_raw_int_en out_sym_int_en;
    out_sym_chn_status out_sym_int_status;
 
    in_sym_chn_node_wr_point in_node_wr_ptr;
    in_sym_chn_node_rd_point in_node_rd_ptr;
    in_sym_chn_node_length in_node_depth;
 
    out_sym_chn_node_wr_point out_node_wr_ptr;
    out_sym_chn_node_rd_point out_node_rd_ptr;
    out_sym_chn_node_length out_node_depth;
 
    crypto_hal_func_enter();
 
    crypto_chk_return(proc_symc_info == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "proc_symc_info is NULL\n");
 
    /* Get Key Ctrl. */
    in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
    proc_symc_info->is_decrypt = in_key_ctrl.bits.sym_alg_decrypt;
 
    /* Alg. */
    ret = crypto_get_index_by_value(g_symc_alg_table, crypto_array_size(g_symc_alg_table),
        in_key_ctrl.bits.sym_alg_sel, &index);
    if (ret != TD_SUCCESS) {
        return SYMC_COMPAT_ERRNO(ERROR_INVALID_REGISTER_VALUE);
    }
    proc_symc_info->alg = index;
 
    /* Alg Mode. */
    ret = crypto_get_index_by_value(g_symc_alg_mode_table, crypto_array_size(g_symc_alg_mode_table),
        in_key_ctrl.bits.sym_alg_mode, &index);
    if (ret != TD_SUCCESS) {
        return SYMC_COMPAT_ERRNO(ERROR_INVALID_REGISTER_VALUE);
    }
    proc_symc_info->mode = index;
 
    /* Key Length. */
    ret = crypto_get_index_by_value(g_symc_key_length_table, crypto_array_size(g_symc_key_length_table),
        in_key_ctrl.bits.sym_alg_key_len, &index);
    if (ret != TD_SUCCESS) {
        crypto_log_err("Invalid Key Length in Register!\n");
        return SYMC_COMPAT_ERRNO(ERROR_INVALID_REGISTER_VALUE);
    }
    proc_symc_info->key_len = index;
 
    /* Key Source. */
    proc_symc_info->key_source = in_key_ctrl.bits.sym_key_chn_id;
 
    /* int_raw. */
    out_raw_int.u32 = spacc_reg_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    proc_symc_info->int_raw = (out_raw_int.bits.out_sym_chan_raw_int >> chn_num) & 0x1;
 
    /* int_en. */
    out_sym_int_en.u32 = spacc_reg_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN);
    proc_symc_info->int_en = (out_sym_int_en.bits.out_sym_chan_int_en >> chn_num) & 0x1;
 
    /* int status. */
    out_sym_int_status.u32 = spacc_reg_read(OUT_SYM_CHAN_LAST_NODE_INT);
    proc_symc_info->int_status = (out_sym_int_status.bits.out_sym_chn_int_status >> chn_num) & 0x1;
 
    /* is_secure. */
    proc_symc_info->is_secure = (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) ? 1 : 0;
 
    /* in node: head(r/w/d) */
    in_node_wr_ptr.u32 = spacc_reg_read(IN_SYM_CHN_NODE_WR_POINT(chn_num));
    in_node_rd_ptr.u32 = spacc_reg_read(IN_SYM_CHN_NODE_RD_POINT(chn_num));
    in_node_depth.u32 = spacc_reg_read(IN_SYM_CHN_NODE_LENGTH(chn_num));
    proc_symc_info->in_node_head = spacc_reg_read(IN_SYM_CHN_NODE_START_ADDR_L(chn_num));
    proc_symc_info->in_node_rptr = in_node_rd_ptr.bits.sym_chn_node_rd_point;
    proc_symc_info->in_node_wptr = in_node_wr_ptr.bits.sym_chn_node_wr_point;
    proc_symc_info->in_node_depth = in_node_depth.bits.sym_chn_node_length;
 
    /* out node: head(r/w/d) */
    out_node_wr_ptr.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_WR_POINT(chn_num));
    out_node_rd_ptr.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_RD_POINT(chn_num));
    out_node_depth.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_LENGTH(chn_num));
    proc_symc_info->out_node_head = spacc_reg_read(OUT_SYM_CHN_NODE_START_ADDR_L(chn_num));
    proc_symc_info->out_node_rptr = out_node_rd_ptr.bits.sym_chn_node_rd_point;
    proc_symc_info->out_node_wptr = out_node_wr_ptr.bits.sym_chn_node_wr_point;
    proc_symc_info->out_node_depth = out_node_depth.bits.sym_chn_node_length;
 
    crypto_hal_func_exit();
 
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_get_config(td_u32 chn_num, hal_symc_config_t *symc_config)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    ret = memcpy_s(symc_config, sizeof(hal_symc_config_t), &hard_ctx->symc_config, sizeof(hal_symc_config_t));
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed\n");

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_start(td_u32 chn_num)
{
    in_sym_chn_node_wr_point in_node_wr_ptr;
    out_sym_chn_node_wr_point out_node_wr_ptr;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    td_u32 ptr;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    /* if wait_func registered, enable interrupt */
    if (hard_ctx->wait_func != TD_NULL) {
        hard_ctx->done = TD_FALSE;
        hard_ctx->is_wait = TD_TRUE;
        inner_symc_irq_enable(chn_num, TD_TRUE);
    } else {
        hard_ctx->is_wait = TD_FALSE;
        inner_symc_irq_enable(chn_num, TD_FALSE);
    }

    /* configure out nodes. */
    out_node_wr_ptr.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_WR_POINT(chn_num));
    ptr = out_node_wr_ptr.bits.sym_chn_node_wr_point + hard_ctx->cnt_out;
    out_node_wr_ptr.bits.sym_chn_node_wr_point = ptr % CRYPTO_SYMC_MAX_LIST_NUM;

    /* make sure all the above explicit memory accesses and instructions are completed
     * before start the hardware.
     */
    crypto_memory_barrier();

    spacc_reg_write(OUT_SYM_CHN_NODE_WR_POINT(chn_num), out_node_wr_ptr.u32);

    /* configure in nodes. */
    in_node_wr_ptr.u32 = spacc_reg_read(IN_SYM_CHN_NODE_WR_POINT(chn_num));

    ptr = in_node_wr_ptr.bits.sym_chn_node_wr_point + hard_ctx->cnt_in;
    in_node_wr_ptr.bits.sym_chn_node_wr_point = ptr % CRYPTO_SYMC_MAX_LIST_NUM;

    /* make sure all the above explicit memory accesses and instructions are completed
     * before start the hardware.
     */
    crypto_memory_barrier();

    crypto_dcache_disable();

    spacc_reg_write(IN_SYM_CHN_NODE_WR_POINT(chn_num), in_node_wr_ptr.u32);

    hard_ctx->cnt_in = 0;
    hard_ctx->cnt_out = 0;

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_done_try(td_u32 chn_num)
{
    out_sym_chan_raw_int last_raw;

    td_u32 mask;

    last_raw.u32 = spacc_reg_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    last_raw.bits.out_sym_chan_raw_int &= (0x01 << chn_num);
    spacc_reg_write(OUT_SYM_CHAN_RAW_LAST_NODE_INT, last_raw.u32);

    mask = last_raw.bits.out_sym_chan_raw_int;
    if (mask == 0) {
        return CRYPTO_FAILURE;
    }

    return CRYPTO_SUCCESS;
}

#if defined(CRYPTO_OS_INT_SUPPORT)
td_s32 hal_cipher_symc_done_notify(td_u32 chn_num)
{
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];
    hard_ctx->done = TD_TRUE;

    crypto_hal_func_exit();
    return TD_SUCCESS;
}
#endif

static td_s32 hal_cipher_symc_out_node_done_try(td_u32 chn_num)
{
    sym_chann_raw_int raw_int;
    td_u32 mask;
    crypto_hal_func_enter();

    raw_int.u32 = spacc_reg_read(SYM_CHANN_RAW_INT);
    raw_int.bits.sym_chann_raw_int &= (0x01 << chn_num);
    spacc_reg_write(SYM_CHANN_RAW_INT, raw_int.u32);

    mask = raw_int.bits.sym_chann_raw_int;
    if (mask == 0) {
        return TD_FAILURE;
    }

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

static td_bool hal_symc_condition(const td_void *param)
{
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    td_u32 chn_num = *(td_u32 *)param;

    hard_ctx = &g_symc_hard_context[chn_num];
    if (hard_ctx->done == TD_TRUE) {
        hard_ctx->done = TD_FALSE;
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

/*
 * Notice: CCM process don't care about multiple encrypt operations, so don't support ccm update.
 */
static td_s32 hal_symc_process_ccm_p_pre(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len)
{
    td_s32 ret;
    td_u8 *ccm_p_padding = TD_NULL;
    td_u32 ccm_p_padding_len = 0;
    in_node_type_e ccm_p_type = IN_NODE_TYPE_NORMAL;

    ccm_p_padding = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_ccm_p_p");
    crypto_chk_return(ccm_p_padding == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");
    /* ccm_p_padding must be zeros. */
    (td_void)memset_s(ccm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);

    if (data_len % CRYPTO_AES_BLOCK_SIZE_IN_BYTES != 0) {
        ccm_p_padding_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES - data_len % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    }
    /* If padding_len == 0, then as the last node. */
    ccm_p_type |= (ccm_p_padding_len == 0 ? IN_NODE_TYPE_LAST : 0);
    ret = hal_cipher_symc_ccm_mac_update(chn_num, data_phys_addr, data_len, IN_NODE_TYPE_FIRST | ccm_p_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_ccm_mac_update failed\n");

    /* If paddding_len != 0, then ccm_p_padding as the last node. */
    if (ccm_p_padding_len != 0) {
        ret = hal_cipher_symc_ccm_mac_update(chn_num, crypto_get_phys_addr(ccm_p_padding), ccm_p_padding_len,
            IN_NODE_TYPE_LAST);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_ccm_mac_update failed\n");
    }

exit_free:
    (td_void)memset_s(ccm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    crypto_free_coherent(ccm_p_padding);
    return ret;
}

static td_s32 hal_symc_process_ccm_p_final(td_u32 chn_num)
{
    td_s32 ret;
    td_u32 pre_idx_out = 0;
    crypto_symc_entry_out *pre_entry_out = TD_NULL;
    td_phys_addr_t pre_phys_addr = 0;
    td_u8 *ccm_p_padding = TD_NULL;
    td_u32 ccm_p_padding_len = 0;
    td_u32 pre_length = 0;
    in_node_type_e ccm_p_type = IN_NODE_TYPE_NORMAL;
    
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    hard_ctx = &g_symc_hard_context[chn_num];

    ccm_p_padding = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_ccm_p_f");
    crypto_chk_return(ccm_p_padding == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");
    /* ccm_p_padding must be zeros. */
    (td_void)memset_s(ccm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);

    /* Get the previous node, beacuse it contains the plain_text's addr&length. */
    pre_idx_out = (hard_ctx->idx_out + CRYPTO_SYMC_MAX_LIST_NUM - 1) % CRYPTO_SYMC_MAX_LIST_NUM;
    pre_entry_out = &hard_ctx->entry_out[pre_idx_out];
    pre_phys_addr = pre_entry_out->sym_start_addr;
    pre_length = pre_entry_out->sym_alg_length;

    if (pre_length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES != 0) {
        ccm_p_padding_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES - pre_length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    }

    /* If padding_len == 0, then as the last node. */
    ccm_p_type |= (ccm_p_padding_len == 0 ? IN_NODE_TYPE_LAST : 0);
    ret = hal_cipher_symc_ccm_mac_update(chn_num, pre_phys_addr, pre_length, IN_NODE_TYPE_FIRST | ccm_p_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_ccm_mac_update failed\n");

    /* If paddding_len != 0, then ccm_p_padding as the last node. */
    if (ccm_p_padding_len != 0) {
        ret = hal_cipher_symc_ccm_mac_update(chn_num, crypto_get_phys_addr(ccm_p_padding),
            ccm_p_padding_len, IN_NODE_TYPE_LAST);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_ccm_mac_update failed\n");
    }

exit_free:
    if (ccm_p_padding != TD_NULL) {
        (td_void)memset_s(ccm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
        crypto_free_coherent(ccm_p_padding);
    }
    return TD_SUCCESS;
}

static td_s32 hal_symc_process_gcm_p_pre(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len)
{
    td_s32 ret;
    td_u8 *gcm_p_padding = TD_NULL;
    td_u32 gcm_p_padding_len = 0;
    in_node_type_e gcm_p_type = IN_NODE_TYPE_NORMAL;

    gcm_p_padding = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_gcm_p_p");
    crypto_chk_return(gcm_p_padding == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");
    /* gcm_p_padding must be zeros. */
    (td_void)memset_s(gcm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);

    if (data_len % CRYPTO_AES_BLOCK_SIZE_IN_BYTES != 0) {
        gcm_p_padding_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES - data_len % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    }
    /* If padding_len == 0, then as the last node. */
    gcm_p_type |= (gcm_p_padding_len == 0 ? IN_NODE_TYPE_LAST : 0);
    ret = hal_cipher_symc_gcm_mac_update(chn_num, data_phys_addr, data_len, IN_NODE_TYPE_FIRST | gcm_p_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_gcm_mac_update failed\n");

    /* If paddding_len != 0, then gcm_p_padding as the last node. */
    if (gcm_p_padding_len != 0) {
        ret = hal_cipher_symc_gcm_mac_update(chn_num, crypto_get_phys_addr(gcm_p_padding), gcm_p_padding_len,
            IN_NODE_TYPE_LAST);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_gcm_mac_update failed\n");
    }

exit_free:
    (td_void)memset_s(gcm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
    crypto_free_coherent(gcm_p_padding);
    return ret;
}

static td_s32 hal_symc_process_gcm_p_final(td_u32 chn_num)
{
    td_s32 ret;
    td_u32 pre_idx_out = 0;
    crypto_symc_entry_out *pre_entry_out = TD_NULL;
    td_phys_addr_t pre_phys_addr = 0;
    td_u8 *gcm_p_padding = TD_NULL;
    td_u32 gcm_p_padding_len = 0;
    td_u32 pre_length = 0;
    in_node_type_e gcm_p_type = IN_NODE_TYPE_NORMAL;
    
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    hard_ctx = &g_symc_hard_context[chn_num];

    gcm_p_padding = crypto_malloc_coherent(CRYPTO_AES_BLOCK_SIZE_IN_BYTES, "cipher_gcm_p_f");
    crypto_chk_return(gcm_p_padding == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");
    /* gcm_p_padding must be zeros. */
    (td_void)memset_s(gcm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);

    /* Get the previous node, beacuse it contains the cipher_text's addr&length. */
    pre_idx_out = (hard_ctx->idx_out + CRYPTO_SYMC_MAX_LIST_NUM - 1) % CRYPTO_SYMC_MAX_LIST_NUM;
    pre_entry_out = &hard_ctx->entry_out[pre_idx_out];
    pre_phys_addr = pre_entry_out->sym_start_addr;
    pre_length = pre_entry_out->sym_alg_length;

    if (pre_length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES != 0) {
        gcm_p_padding_len = CRYPTO_AES_BLOCK_SIZE_IN_BYTES - pre_length % CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    }

    /* If padding_len == 0, then as the last node. */
    gcm_p_type |= (gcm_p_padding_len == 0 ? IN_NODE_TYPE_LAST : 0);
    ret = hal_cipher_symc_gcm_mac_update(chn_num, pre_phys_addr, pre_length, IN_NODE_TYPE_FIRST | gcm_p_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_gcm_mac_update failed\n");

    /* If paddding_len != 0, then gcm_p_padding as the last node. */
    if (gcm_p_padding_len != 0) {
        ret = hal_cipher_symc_gcm_mac_update(chn_num, crypto_get_phys_addr(gcm_p_padding),
            gcm_p_padding_len, IN_NODE_TYPE_LAST);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_gcm_mac_update failed\n");
    }

exit_free:
    if (gcm_p_padding != TD_NULL) {
        (td_void)memset_s(gcm_p_padding, CRYPTO_AES_BLOCK_SIZE_IN_BYTES, 0, CRYPTO_AES_BLOCK_SIZE_IN_BYTES);
        crypto_free_coherent(gcm_p_padding);
    }
    return TD_SUCCESS;
}

static td_s32 hal_wait_in_node_done(td_u32 chn_num)
{
    td_u32 i;
    td_u32 data_len = 0;
    in_sym_chn_node_wr_point in_node_wr_ptr = {0};
    in_sym_chn_node_rd_point in_node_rd_ptr = {0};

    for (i = 0; i < CRYPTO_SYMC_WAIT_TIMEOUT; i++) {
        in_node_wr_ptr.u32 = spacc_reg_read(IN_SYM_CHN_NODE_WR_POINT(chn_num));
        in_node_rd_ptr.u32 = spacc_reg_read(IN_SYM_CHN_NODE_RD_POINT(chn_num));
        data_len = spacc_reg_read(DBG_IN_SYM_CHN_DATA_LEN(chn_num));
        if ((in_node_rd_ptr.bits.sym_chn_node_rd_point == in_node_wr_ptr.bits.sym_chn_node_wr_point) &&
            (data_len == 0)) {
            break;
        }
        crypto_udelay(1);
    }

    if (i >= CRYPTO_SYMC_WAIT_TIMEOUT) {
        crypto_log_err("hal_wait_in_node_done Timeout!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 hal_wait_out_node_done(td_u32 chn_num)
{
    td_u32 i;

    out_sym_chn_node_wr_point out_node_wr_ptr = {0};
    out_sym_chn_node_rd_point out_node_rd_ptr = {0};

    for (i = 0; i < CRYPTO_SYMC_WAIT_TIMEOUT; i++) {
        out_node_wr_ptr.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_WR_POINT(chn_num));
        out_node_rd_ptr.u32 = spacc_reg_read(OUT_SYM_CHN_NODE_RD_POINT(chn_num));
        if (out_node_rd_ptr.bits.sym_chn_node_rd_point == out_node_wr_ptr.bits.sym_chn_node_wr_point) {
            break;
        }
        crypto_udelay(1);
    }

    if (i >= CRYPTO_SYMC_WAIT_TIMEOUT) {
        crypto_log_err("hal_wait_out_node_done Timeout!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 symc_wait_done(td_u32 chn_num, td_bool is_wait, crypto_symc_hard_context *hard_ctx)
{
    td_s32 ret = CRYPTO_SUCCESS;
    td_u32 i = 0;
    (void)(is_wait);
    if ((hard_ctx->is_wait == TD_TRUE) && (hard_ctx->wait_func != TD_NULL)) {
        ret = hard_ctx->wait_func(hard_ctx->wait, hal_symc_condition, (td_void *)(&chn_num), hard_ctx->timeout_ms);
        if (ret <= 0) {
            crypto_log_err("wait_func Timeout!\n");
            ret = SYMC_COMPAT_ERRNO(ERROR_SYMC_CALC_TIMEOUT);
        }
    } else {
        for (i = 0; i < CRYPTO_SYMC_WAIT_TIMEOUT; i++) {
            ret = hal_cipher_symc_done_try(chn_num);
            if (ret == TD_SUCCESS) {
                break;
            }
            crypto_udelay(1);
        }
        if (i >= CRYPTO_SYMC_WAIT_TIMEOUT) {
            ret = SYMC_COMPAT_ERRNO(ERROR_SYMC_CALC_TIMEOUT);
        }
    }
    if (ret == SYMC_COMPAT_ERRNO(ERROR_SYMC_CALC_TIMEOUT)) {
        hal_cipher_symc_debug();
        hal_cipher_symc_debug_chn(chn_num);
    } else {
        ret = CRYPTO_SUCCESS;
    }

    return ret;
}

td_s32 hal_cipher_symc_wait_done(td_u32 chn_num, td_bool is_wait)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    if (hard_ctx->symc_config.work_mode == CRYPTO_SYMC_WORK_MODE_CBC_MAC) {
        ret = hal_cipher_symc_wait_noout_done(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_noout_done failed\n");
        return ret;
    }

    ret = symc_wait_done(chn_num, is_wait, hard_ctx);
    inner_symc_irq_enable(chn_num, TD_FALSE);
    if (ret == SYMC_COMPAT_ERRNO(ERROR_HASH_CALC_TIMEOUT)) {
        crypto_log_err("symc wait done timeout, chn is %u\n", chn_num);
        hal_cipher_symc_debug();
        hal_cipher_symc_debug_chn(chn_num);
        return ret;
    }

    hal_cipher_symc_get_iv(chn_num, hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));

    /* For CCM Decrypt, you should mac_update plain_text after decrypt. */
    if (hard_ctx->symc_config.work_mode == CRYPTO_SYMC_WORK_MODE_CCM && hard_ctx->symc_config.is_decrypt == TD_TRUE) {
        ret = hal_symc_process_ccm_p_final(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_symc_process_ccm_p_final failed\n");
    }

    /* For GCM Encrypt, you should mac_update cipher_text after encrypt. */
    if (hard_ctx->symc_config.work_mode == CRYPTO_SYMC_WORK_MODE_GCM && hard_ctx->symc_config.is_decrypt == TD_FALSE) {
        ret = hal_symc_process_gcm_p_final(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_symc_process_gcm_p_final failed\n");
    }

    crypto_dcache_enable();

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

static td_s32 hal_cipher_symc_ccm_mac_update(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    in_sym_chn_key_ctrl in_key_ctrl = {0};
    crypto_symc_hard_context *hard_ctx = TD_NULL;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    ret = priv_set_in_node(chn_num, data_phys_addr, data_len, in_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_set_in_node failed\n");

    /* To mac_update, you should choose MODE_CBC_NOOUT. */
    in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
    in_key_ctrl.bits.sym_alg_mode = SYMC_ALG_MODE_CBC_NOOUT_VAL;
    in_key_ctrl.bits.sym_alg_decrypt = TD_FALSE;
    spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);

#if defined(CRYPTO_CCM_TRACE_ENABLE)
    crypto_dump_data("iv_mac before update", hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
#endif
    /* You should set iv_mac before mac_update. */
    ret = hal_cipher_symc_set_iv(chn_num, hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed\n");

    hard_ctx->cnt_in++;
    hard_ctx->idx_in++;
    hard_ctx->idx_in %= CRYPTO_SYMC_MAX_LIST_NUM;

    hal_cipher_symc_start(chn_num);

    /**
     * For last node, you need to wait until the interrupt(SYM_CHANN_RAW_INT) occurs;
     * Others, you only need to wait until in_node_wr_ptr == in_node_rd_ptr.
     *
     **/
    if (in_node_type & IN_NODE_TYPE_LAST) {
        ret = hal_cipher_symc_wait_noout_done(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_noout_done failed\n");
    } else {
        ret = hal_wait_in_node_done(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_wait_in_node_done failed\n");
    }

    /* You should update iv_mac after mac_update. */
    ret = hal_cipher_symc_get_iv(chn_num, hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_iv failed\n");

#if defined(CRYPTO_CCM_TRACE_ENABLE)
    crypto_dump_phys_addr("ccm update data", data_phys_addr, data_len);
    crypto_dump_data("ccm iv_mac update", hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
#endif
    return TD_SUCCESS;
}

static td_s32 hal_cipher_symc_gcm_mac_update(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    in_sym_chn_key_ctrl in_key_ctrl = {0};
    crypto_symc_hard_context *hard_ctx = TD_NULL;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    ret = priv_set_in_node(chn_num, data_phys_addr, data_len, in_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_set_in_node failed\n");

    /* To gcm_mac_update, you should choose ALG_GHASH. */
    in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
    in_key_ctrl.bits.sym_alg_sel = SYMC_ALG_GHASH_VAL;
    /* symc_alg_mode must be 0 for GHASH, otherwise the calculation will timeout. */
    in_key_ctrl.bits.sym_alg_mode = 0;
    in_key_ctrl.bits.sym_alg_decrypt = TD_FALSE;
    spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);

    /**
    * You should set iv before mac_update.
    * For GCM_HASH, the iv is gcm_j0; otherwise the iv is iv_ghash.
    **/
    if (in_node_type & IN_NODE_TYPE_GCM_GHASH) {
        ret = hal_cipher_symc_set_iv(chn_num, hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0));
    } else {
        ret = hal_cipher_symc_set_iv(chn_num, hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
    }
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_set_iv failed\n");

    hard_ctx->cnt_in++;
    hard_ctx->idx_in++;
    hard_ctx->idx_in %= CRYPTO_SYMC_MAX_LIST_NUM;

    hal_cipher_symc_start(chn_num);

    /**
     * For last node, you need to wait until the interrupt(SYM_CHANN_RAW_INT) occurs;
     * Others, you only need to wait until in_node_wr_ptr == in_node_rd_ptr.
     *
     **/
    if (in_node_type & IN_NODE_TYPE_LAST) {
        ret = hal_cipher_symc_wait_noout_done(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_wait_noout_done failed\n");
    } else {
        ret = hal_wait_in_node_done(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_wait_in_node_done failed\n");
    }

#ifdef CRYPTO_GCM_TRACE_ENABLE
    crypto_dump_data("iv_ghash", hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
#endif

    /* You should update iv_ghash after mac_update. For GCM_IV, no need to update iv_ghash. */
    if (!(in_node_type & IN_NODE_TYPE_GCM_IV)) {
        ret = hal_cipher_symc_get_iv(chn_num, hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_get_iv failed\n");
    }
#ifdef CRYPTO_GCM_TRACE_ENABLE
    crypto_dump_data("iv_ghash_next", hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
#endif
    return TD_SUCCESS;
}

static td_s32 add_normal_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    ret = priv_set_in_node(chn_num, data_phys_addr, data_len, in_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_set_in_node failed\n");

    hard_ctx->idx_in++;
    hard_ctx->cnt_in++;
    hard_ctx->idx_in %= CRYPTO_SYMC_MAX_LIST_NUM;

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

static td_s32 add_ccm_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    in_sym_chn_key_ctrl in_key_ctrl;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    /* For CCM_N and CCM_AAD, you should mac_update them. */
    if ((in_node_type & IN_NODE_TYPE_CCM_N) || (in_node_type & IN_NODE_TYPE_CCM_AAD)) {
        ret = hal_cipher_symc_ccm_mac_update(chn_num, data_phys_addr, data_len, in_node_type);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_ccm_mac_update failed\n");
        return TD_SUCCESS;
    }

    /* For CCM Encrypt, you should mac_update plain_text before encrypt. */
    if ((in_node_type & IN_NODE_TYPE_CCM_P) && (hard_ctx->symc_config.is_decrypt == TD_FALSE)) {
        hal_symc_process_ccm_p_pre(chn_num, data_phys_addr, data_len);
    }

    /* For ccm, the work_mode is CTR and the iv is iv_ctr. */
    if (in_node_type & IN_NODE_TYPE_CCM_P) {
        in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
        in_key_ctrl.bits.sym_alg_mode = SYMC_ALG_MODE_CTR_VAL;
        spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);
        hal_cipher_symc_set_iv(chn_num, hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));
    }

    ret = priv_set_in_node(chn_num, data_phys_addr, data_len, in_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_set_in_node failed\n");

    hard_ctx->idx_in++;
    hard_ctx->cnt_in++;
    hard_ctx->idx_in %= CRYPTO_SYMC_MAX_LIST_NUM;

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

static td_s32 add_gcm_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    in_sym_chn_key_ctrl in_key_ctrl;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    /* For GCM_AAD, you should mac_update it. */
    if (in_node_type & IN_NODE_TYPE_GCM_A) {
        ret = hal_cipher_symc_gcm_mac_update(chn_num, data_phys_addr, data_len, in_node_type);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_gcm_mac_update failed\n");
        return TD_SUCCESS;
    }
    /* For GCM_LEN, you should store it and mac_update it when get_tag. */
    if (in_node_type & IN_NODE_TYPE_GCM_LEN) {
        hard_ctx->gcm_len_addr = data_phys_addr;
        return TD_SUCCESS;
    }

    /* For GCM Decrypt, you should mac_update plain_text before decrypt. */
    if ((in_node_type & IN_NODE_TYPE_GCM_P) && (hard_ctx->symc_config.is_decrypt == TD_TRUE)) {
        hal_symc_process_gcm_p_pre(chn_num, data_phys_addr, data_len);
    }

    /* For GCM_GHASH, the work_mode is MODE_GCTR_NOOUT and the iv is j0. */
    if (in_node_type & IN_NODE_TYPE_GCM_GHASH) {
        in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
        in_key_ctrl.bits.sym_alg_sel = SYMC_ALG_AES_VAL;
        in_key_ctrl.bits.sym_alg_mode = SYMC_ALG_MODE_GCTR_NOOUT_VAL;
        spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);
        hal_cipher_symc_set_iv(chn_num, hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0));
    }

    /* For gcm, the work_mode is GCTR and the iv is gcm_iv. */
    if (in_node_type & IN_NODE_TYPE_GCM_P) {
        in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
        in_key_ctrl.bits.sym_alg_sel = SYMC_ALG_AES_VAL;
        in_key_ctrl.bits.sym_alg_mode = SYMC_ALG_MODE_GCTR_VAL;
        spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);
        hal_cipher_symc_set_iv(chn_num, hard_ctx->iv_ctr, sizeof(hard_ctx->iv_ctr));
    }
    ret = priv_set_in_node(chn_num, data_phys_addr, data_len, in_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_set_in_node failed\n");

    hard_ctx->idx_in++;
    hard_ctx->cnt_in++;
    hard_ctx->idx_in %= CRYPTO_SYMC_MAX_LIST_NUM;

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

static td_s32 add_cbc_mac_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    in_sym_chn_key_ctrl in_key_ctrl;

    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];

    in_key_ctrl.u32 = spacc_reg_read(IN_SYM_CHN_KEY_CTRL(chn_num));
    in_key_ctrl.bits.sym_alg_sel = SYMC_ALG_AES_VAL;
    in_key_ctrl.bits.sym_alg_mode = SYMC_ALG_MODE_CBC_NOOUT_VAL;
    spacc_reg_write(IN_SYM_CHN_KEY_CTRL(chn_num), in_key_ctrl.u32);

    ret = priv_set_in_node(chn_num, data_phys_addr, data_len, in_node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_set_in_node failed\n");

    hard_ctx->idx_in++;
    hard_ctx->cnt_in++;
    hard_ctx->idx_in %= CRYPTO_SYMC_MAX_LIST_NUM;

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_add_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;

    crypto_hal_func_enter();
    crypto_chk_return(data_phys_addr == 0, SYMC_COMPAT_ERRNO(ERROR_INVALID_PHYS_ADDR), "data_phys_addr is invalid\n");

    hard_ctx = &g_symc_hard_context[chn_num];
    switch (hard_ctx->symc_config.work_mode) {
        case CRYPTO_SYMC_WORK_MODE_CCM:
            ret = add_ccm_in_node(chn_num, data_phys_addr, data_len, in_node_type);
            break;
        case CRYPTO_SYMC_WORK_MODE_GCM:
            ret = add_gcm_in_node(chn_num, data_phys_addr, data_len, in_node_type);
            break;
        case CRYPTO_SYMC_WORK_MODE_CBC_MAC:
            ret = add_cbc_mac_in_node(chn_num, data_phys_addr, data_len, in_node_type);
            break;
        default:
            ret = add_normal_in_node(chn_num, data_phys_addr, data_len, in_node_type);
            break;
    }

    crypto_hal_func_exit();
    return ret;
}

td_s32 hal_cipher_symc_add_out_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len)
{
    td_s32 ret;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_symc_entry_out *entry_out = TD_NULL;
    td_u32 idx_in;
    td_u32 idx_out;
    crypto_hal_func_enter();
    crypto_chk_return(data_phys_addr == 0, SYMC_COMPAT_ERRNO(ERROR_INVALID_PHYS_ADDR), "data_phys_addr is invalid\n");

    hard_ctx = &g_symc_hard_context[chn_num];
    idx_out = hard_ctx->idx_out;
    if (hard_ctx->entry_out == TD_NULL) {
        crypto_log_err("hard_ctx->entry_out is NULL\n");
        return TD_FAILURE;
    }
    entry_out = &hard_ctx->entry_out[idx_out];

    entry_out->sym_alg_length = data_len;
    entry_out->sym_start_addr = data_phys_addr;
    entry_out->sym_start_high = 0;

    hard_ctx->idx_out++;
    hard_ctx->cnt_out++;
    hard_ctx->idx_out %= CRYPTO_SYMC_MAX_LIST_NUM;

    idx_in = (hard_ctx->idx_in + CRYPTO_SYMC_MAX_LIST_NUM - 1) % CRYPTO_SYMC_MAX_LIST_NUM;

    if (hard_ctx->entry_in[idx_in].sym_last_node == 0 &&
        hard_ctx->symc_config.work_mode != CRYPTO_SYMC_WORK_MODE_CCM &&
        hard_ctx->symc_config.work_mode != CRYPTO_SYMC_WORK_MODE_GCM) {
        ret = hal_cipher_symc_start(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_start failed\n");

        ret = hal_wait_out_node_done(chn_num);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_wait_out_node_done failed\n");
    }

    crypto_hal_func_exit();
    return TD_SUCCESS;
}

td_s32 hal_cipher_symc_get_tag(td_u32 chn_num, td_u8 *tag, td_u32 tag_len)
{
    td_s32 ret;
    td_u32 i;
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    td_u8 *iv_ghash = TD_NULL;

    hard_ctx = &g_symc_hard_context[chn_num];
    /* For CCM, the tag is MSB(s0^iv_mac). */
    if (hard_ctx->symc_config.work_mode == CRYPTO_SYMC_WORK_MODE_CCM) {
#if defined(CRYPTO_CCM_TRACE_ENABLE)
        crypto_dump_data("get tag s0", hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0));
        crypto_dump_data("get tag iv_mac", hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
#endif
        for (i = 0; i < tag_len; i++) {
            tag[i] = hard_ctx->symc_config.iv0[i] ^ hard_ctx->symc_config.iv_mac[i];
        }
    } else if (hard_ctx->symc_config.work_mode == CRYPTO_SYMC_WORK_MODE_GCM) {
        if (hard_ctx->gcm_len_addr == 0) {
            crypto_log_err("hard_ctx->gcm_len_addr is zero\n");
            return SYMC_COMPAT_ERRNO(ERROR_UNEXPECTED);
        }
#if defined(CRYPTO_GCM_TRACE_ENABLE)
        crypto_log_dbg("gcm_len_addr is 0x%llx\n", hard_ctx->gcm_len_addr);
        crypto_dump_phys_addr("get tag gcm_len", hard_ctx->gcm_len_addr, CRYPTO_AES_IV_SIZE);
#endif
        ret = hal_cipher_symc_gcm_mac_update(chn_num, hard_ctx->gcm_len_addr, CRYPTO_AES_IV_SIZE,
            IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_symc_gcm_mac_update failed\n");

        iv_ghash = crypto_malloc_coherent(sizeof(hard_ctx->symc_config.iv_mac), "cipher_get_tag");
        crypto_chk_return(iv_ghash == TD_NULL, TD_FAILURE, "crypto_malloc_coherent failed\n");

        ret = memcpy_s(iv_ghash, sizeof(hard_ctx->symc_config.iv_mac),
            hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac));
        crypto_chk_goto(ret != EOK, exit_free, "memcpy_s failed\n");

        ret = hal_cipher_symc_add_in_node(chn_num, crypto_get_phys_addr(iv_ghash), sizeof(hard_ctx->symc_config.iv_mac),
            IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST | IN_NODE_TYPE_GCM_GHASH);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_add_in_node failed\n");

        ret = hal_cipher_symc_start(chn_num);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_start failed\n");

        ret = hal_cipher_symc_wait_noout_done(chn_num);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_wait_noout_done failed\n");

        ret = hal_cipher_symc_get_iv(chn_num, tag, tag_len);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_cipher_symc_get_iv failed\n");
#if defined(CRYPTO_CCM_TRACE_ENABLE) || defined(CRYPTO_GCM_TRACE_ENABLE)
        crypto_dump_data("get tag", tag, tag_len);
#endif
    }

    /* Clear iv_ghash. */
    (td_void)memset_s(hard_ctx->symc_config.iv_mac, sizeof(hard_ctx->symc_config.iv_mac),
        0, sizeof(hard_ctx->symc_config.iv_mac));
    (td_void)memset_s(hard_ctx->symc_config.iv0, sizeof(hard_ctx->symc_config.iv0),
        0, sizeof(hard_ctx->symc_config.iv0));
exit_free:
    if (iv_ghash != TD_NULL) {
        (td_void)memset_s(iv_ghash, sizeof(hard_ctx->symc_config.iv_mac), 0, sizeof(hard_ctx->symc_config.iv_mac));
        crypto_free_coherent(iv_ghash);
    }

    return TD_SUCCESS;
}

#if defined(CRYPTO_OS_INT_SUPPORT)
td_s32 hal_cipher_symc_register_wait_func(td_u32 chn_num, td_void *wait,
    crypto_wait_timeout_interruptible wait_func, td_u32 timeout_ms)
{
    crypto_symc_hard_context *hard_ctx = TD_NULL;
    crypto_hal_func_enter();

    hard_ctx = &g_symc_hard_context[chn_num];
    hard_ctx->wait = wait;
    hard_ctx->wait_func = wait_func;
    hard_ctx->timeout_ms = timeout_ms;

    if (wait_func != TD_NULL) {
        inner_spacc_interrupt_enable(TD_TRUE);
    }
    crypto_hal_func_exit();
    return TD_SUCCESS;
}
#endif

td_void hal_cipher_set_chn_secure(td_u32 chn_num, td_bool dest_sec, td_bool src_sec)
{
    crypto_unused(chn_num);
    crypto_unused(dest_sec);
    crypto_unused(src_sec);

    return;
}