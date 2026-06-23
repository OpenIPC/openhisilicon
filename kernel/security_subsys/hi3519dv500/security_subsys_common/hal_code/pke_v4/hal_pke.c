/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_pke.h"
#include "hal_pke_reg.h"
#include "crypto_drv_common.h"
#include "securec.h"
#include "hal_common.h"

#define PKE_TIME_OUT     6000000
#define BYTE_BITS        0x08
#define BYTE_MASK        0xFF
#define U16_MSB          0x8000
#define CRC16_POLYNOMIAL 0x1021
#define BITS_PER_BYTE    8
#define BYTES_PER_WORD   4
#define PKE_COMPAT_ERRNO(err_code)      HAL_COMPAT_ERRNO(ERROR_MODULE_PKE, err_code)

typedef struct {
    const td_void *wait;
    crypto_wait_timeout_interruptible wait_func;
    td_u32 timeout_ms;
    td_bool wait_done;
} pke_wait_event_type;

static td_u32 g_pke_initialize = TD_FALSE;
static pke_wait_event_type g_pke_wait_event = {
    .wait = TD_NULL,
    .wait_func = TD_NULL,
    .timeout_ms = 0,
    .wait_done = TD_FALSE
};

typedef struct {
    hal_pke_len pke_len;
    td_u32 pke_data_len_reg;
} pke_len_reg_map;

// Define the union of pke module interrupt enable register
typedef union {
    // Define the struct bits
    struct {
        td_u32 pke_ie : 1;
        td_u32 reserved : 31;
    } bits;

    // Define an unsigned member
    td_u32 u32;
} pke_ie;

static const pke_len_reg_map g_pke_len_reg_table[] = {
    {HAL_PKE_LEN_256, 0},
    {HAL_PKE_LEN_384, 1},
    {HAL_PKE_LEN_512, 2},
    {HAL_PKE_LEN_521, 2},
    {HAL_PKE_LEN_2048, 3},
    {HAL_PKE_LEN_3072, 4},
    {HAL_PKE_LEN_4096, 5}
};

#if defined(CONFIG_PKE_TIMEOUT_DEBUG)
static td_void hal_cipher_pke_debug(td_void)
{
    crypto_log_dbg("PKE_BUSY is 0x%x\n", pke_reg_read(PKE_BUSY));
    crypto_log_dbg("PKE_WORK_MODE is 0x%x\n", pke_reg_read(PKE_WORK_MODE));
    crypto_log_dbg("PKE_INT_ENABLE is 0x%x\n", pke_reg_read(PKE_INT_ENABLE));
    crypto_log_dbg("PKE_LOCK_INT_ENABLE is 0x%x\n", pke_reg_read(PKE_LOCK_INT_ENABLE));
    crypto_log_dbg("PKE_INT_STATUS is 0x%x\n", pke_reg_read(PKE_INT_STATUS));
    crypto_log_dbg("PKE_INT_NOMASK_STATUS is 0x%x\n", pke_reg_read(PKE_INT_NOMASK_STATUS));
    crypto_log_dbg("PKE_NOISE_EN is 0x%x\n", pke_reg_read(PKE_NOISE_EN));
    crypto_log_dbg("PKE_LOCK_STATUS is 0x%x\n", pke_reg_read(PKE_LOCK_STATUS));
    crypto_log_dbg("PKE_INT_RAW is 0x%x\n", pke_reg_read(PKE_INT_RAW));
}
#else
#define hal_cipher_pke_debug(...)
#endif

td_s32 hal_pke_init(void)
{
    g_pke_initialize = TD_TRUE;
    return TD_SUCCESS;
}

td_s32 hal_pke_deinit(void)
{
    g_pke_initialize = TD_FALSE;
    return TD_SUCCESS;
}

static td_bool pke_wait_condition(const td_void *param __attribute__((unused)))
{
    if (g_pke_wait_event.wait_done == TD_TRUE) {
        g_pke_wait_event.wait_done = TD_FALSE;
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void inner_pke_lock_irq_enable(td_bool enable)
{
    pke_lock_int_enable pke_lock_en;

    if (g_pke_wait_event.wait_func == TD_NULL) {
        return;
    }
    pke_lock_en.u32 = pke_reg_read(PKE_LOCK_INT_ENABLE);
    pke_lock_en.bits.pke_lock_int_en = enable;
    pke_reg_write(PKE_LOCK_INT_ENABLE, pke_lock_en.u32);
}

static td_void inner_pke_irq_enable(td_bool enable)
{
    pke_int_enable pke_int_en;
    if (g_pke_wait_event.wait_func == TD_NULL) {
        return;
    }
    pke_int_en.u32 = pke_reg_read(PKE_INT_ENABLE);
    pke_int_en.bits.finish_int_enable = enable;
    pke_reg_write(PKE_INT_ENABLE, pke_int_en.u32);
}

td_s32 hal_pke_lock(void)
{
    pke_lock_ctrl lock_ctrl;
    pke_lock_status lock_status;
    td_u32 lock_type;
    td_s32 ret = TD_FAILURE;
    td_u32 i = 0;
    if (g_pke_initialize == TD_FALSE) {
        crypto_log_err("pke must be initialized!\n");
        return TD_FAILURE;
    }
    if (crypto_get_cpu_type() == CRYPTO_CPU_TYPE_SCPU) {
        lock_type = LOCK_STATUS_SCPU;
    } else {
        lock_type = LOCK_STATUS_ACPU;
    }

    inner_pke_lock_irq_enable(TD_TRUE);
    /* lock pke */
    lock_ctrl.u32 = pke_reg_read(PKE_LOCK_CTRL);
    lock_ctrl.bits.pke_lock_type = 0; /* lock command */
    lock_ctrl.bits.pke_lock = 1;
    pke_reg_write(PKE_LOCK_CTRL, lock_ctrl.u32);

    if (g_pke_wait_event.wait_func != TD_NULL) {
        g_pke_wait_event.wait_done = TD_FALSE;
        ret = g_pke_wait_event.wait_func(g_pke_wait_event.wait, pke_wait_condition, (td_void *)(uintptr_t)lock_type,
            g_pke_wait_event.timeout_ms);
        if (ret <= 0) {
            ret = PKE_COMPAT_ERRNO(ERROR_PKE_LOCK_TIMEOUT);
        }
    } else {
        for (i = 0; i < PKE_TIME_OUT; i++) {
            /* check lock result */
            lock_status.u32 = pke_reg_read(PKE_LOCK_STATUS);
            if (lock_status.bits.pke_lock_stat == lock_type) {
                break;
            }
            crypto_udelay(1); // 1 us is empirical value of register lock read
        }
        if (i >= PKE_TIME_OUT) {
            crypto_log_err("pke lock timeout\n");
            ret = PKE_COMPAT_ERRNO(ERROR_PKE_LOCK_TIMEOUT);
        }
    }
    if (ret == PKE_COMPAT_ERRNO(ERROR_PKE_LOCK_TIMEOUT)) {
        hal_cipher_pke_debug();
    } else {
        ret = CRYPTO_SUCCESS;
    }
    inner_pke_lock_irq_enable(TD_FALSE);

    inner_pke_irq_enable(TD_TRUE);

    return ret;
}

td_s32 hal_pke_unlock(void)
{
    pke_lock_ctrl lock_ctrl;
    pke_lock_status lock_status;

    inner_pke_irq_enable(TD_FALSE);

    /* unlock pke */
    lock_ctrl.u32 = pke_reg_read(PKE_LOCK_CTRL);
    lock_ctrl.bits.pke_lock_type = 1; /* unlock command */
    lock_ctrl.bits.pke_lock = 1;
    pke_reg_write(PKE_LOCK_CTRL, lock_ctrl.u32);

    /* check unlock result */
    lock_status.u32 = pke_reg_read(PKE_LOCK_STATUS);
    if ((lock_status.bits.pke_lock_stat != PKE_LOCK_UNLOCK) ||
        (lock_status.bits.pke_unlock_fail == 1)) {
        crypto_log_err("pke unlock timeout\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_void hal_pke_secure(td_bool enable)
{
    pke_noise_en noise;

    /* enable noise */
    noise.u32 = pke_reg_read(PKE_NOISE_EN);
    noise.bits.noise_en = 0;
    if (enable) {
        noise.bits.noise_en = 1;
    }
    pke_reg_write(PKE_NOISE_EN, noise.u32);
}

#if defined(CRYPTO_OS_INT_SUPPORT)
td_u32 hal_pke_done_try(void)
{
    pke_lock_int_dat lock_int_dat = {0};
    pke_int_dat int_dat = {0};

    lock_int_dat.u32 = pke_reg_read(PKE_INT_RAW);
    int_dat.u32 = pke_reg_read(PKE_INT_NOMASK_STATUS);

    if (lock_int_dat.bits.pke_lock_int_raw == 0x1) {
        lock_int_dat.bits.pke_lock_int_raw = 0x1;
        pke_reg_write(PKE_INT_RAW, lock_int_dat.u32);
        return 1;
    }

    if (int_dat.bits.finish_int_nomask == 0x5) {
        int_dat.bits.finish_int_nomask = 0x1;
        pke_reg_write(PKE_INT_NOMASK_STATUS, int_dat.u32);
        return 1;
    }

    return 0;
}

td_void hal_pke_done_notify(void)
{
    g_pke_wait_event.wait_done = TD_TRUE;
}
#endif

td_s32 hal_pke_wait_free(void)
{
    td_u32 i = 0;
    pke_busy busy;
    /* wait ready */
    for (i = 0; i < PKE_TIME_OUT; i++) {
        busy.u32 = pke_reg_read(PKE_BUSY);
        if (!busy.bits.pke_busy) {
            break;
        }
        crypto_udelay(1);
    }

    if (i >= PKE_TIME_OUT) {
        crypto_log_err("error, pke wait free timeout\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_WAIT_DONE_TIMEOUT);
    }
    return CRYPTO_SUCCESS;
}

static pke_mode mode_map(hal_pke_mode mode)
{
    switch (mode) {
        case HAL_PKE_MODE_CLR_RAM:
            return PKE_MODE_CLR_RAM ;
        case HAL_PKE_MODE_EXP_MOD:
            return PKE_MODE_EXP_MOD;
        case HAL_PKE_MODE_MUL_DOT:
            return PKE_MODE_MUL_DOT;
        case HAL_PKE_MODE_ADD_DOT:
            return PKE_MODE_ADD_DOT;
        case HAL_PKE_MODE_TIMES_DOT:
            return PKE_MODE_TIMES_DOT;
        case HAL_PKE_MODE_MINV_MOD:
            return PKE_MODE_MINV_MOD;
        case HAL_PKE_MODE_SUB_MOD:
            return PKE_MODE_SUB_MOD;
        case HAL_PKE_MODE_MUL_MOD:
            return PKE_MODE_MUL_MOD;
        case HAL_PKE_MODE_ADD_MOD:
            return PKE_MODE_ADD_MOD;
        case HAL_PKE_MODE_MOD:
            return PKE_MODE_MOD;
        case HAL_PKE_MODE_MUL:
            return PKE_MODE_MUL;
        case HAL_PKE_MODE_MG_MUL_DOT:
            return PKE_MODE_MG_MUL_DOT;
        case HAL_PKE_MODE_ED_MUL_DOT:
            return PKE_MODE_ED_MUL_DOT;
        case HAL_PKE_MODE_ED_ADD_DOT:
            return PKE_MODE_ED_ADD_DOT;
        default:
            return PKE_MODE_UNSUPPORTTED;
    }
}

td_s32 hal_pke_set_mode(hal_pke_mode mode, td_u32 len)
{
    td_u32 i = 0;
    pke_work_mode work;
    pke_mode mode_mapped = mode_map(mode);

    work.u32 = pke_reg_read(PKE_WORK_MODE);

    if (mode_mapped == PKE_MODE_UNSUPPORTTED) {
        crypto_log_err("This soc doesn't support this work mode!");
        return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    work.bits.mode_sel = mode_mapped;

    for (i = 0; i < sizeof(g_pke_len_reg_table) / sizeof(pke_len_reg_map); i++) {
        if (len == g_pke_len_reg_table[i].pke_len) {
            work.bits.data_len = g_pke_len_reg_table[i].pke_data_len_reg;
            break;
        }
    }

    pke_reg_write(PKE_WORK_MODE, work.u32);
    return TD_SUCCESS;
}

td_void hal_pke_set_ram(td_u32 addr, const td_u8 *ram, td_u32 data_len, td_u32 padded_len)
{
    td_u32 start, val, i, j;
    start = addr;

    /* Input the data which is aligned with 4 bytes;
       For little-endian system, on reading one word from ram to val, byte sequence should be adjusted as -
       in ram: Byte1 | Byte2 | Byte3 | Byte4 (addr low <--> addr high)
       to val: Byte4 | Byte3 | Byte2 | Byte1 (MSB <--> LSB)
    */
    for (i = data_len; i >= WORD_WIDTH; i -= WORD_WIDTH) {
        val = ram[i - 4] << 24;  /* i-4 index shift 24 bits */
        val |= ram[i - 3] << 16; /* i-3 index shift 16 bits */
        val |= ram[i - 2] << 8;  /* i-2 index shift 8 bits */
        val |= ram[i - 1];
        pke_reg_write(start, val);
        start += WORD_WIDTH;
    }

    /* Input the data which is not aligned with 4 bytes */
    if (i != 0) {
        val = 0;
        for (j = 0; j < i; j++) {
            val |= ram[j] << (8 * (i - 1 - j)); // 8 bit = 1 byte
        }
        pke_reg_write(start, val);
        start += WORD_WIDTH;
    }

    /* Pad the remaining part with 0x00 */
    for (; start < addr + padded_len; start += WORD_WIDTH) {
        val = 0x0;
        pke_reg_write(start, val);
    }
    return;
}

td_void hal_pke_start(void)
{
    pke_start start;

    start.u32 = 0x00;
    start.bits.pke_start = PKE_START_CODE;

    pke_reg_write(PKE_START, start.u32);

    return;
}

static td_s32 hal_pke_wait_free_int(void)
{
    td_s32 ret = TD_FAILURE;
    /* wait ready */
    if (g_pke_wait_event.wait_func != TD_NULL) {
        ret = g_pke_wait_event.wait_func(g_pke_wait_event.wait, pke_wait_condition, TD_NULL,
            g_pke_wait_event.timeout_ms);
        if (ret > 0) {
            return TD_SUCCESS;
        } else {
            hal_cipher_pke_debug();
            return PKE_COMPAT_ERRNO(ERROR_PKE_WAIT_DONE_TIMEOUT);
        }
    }

    return hal_pke_wait_free();
}

static td_s32 hal_pke_check_robust_warn(void)
{
    pke_robust_warn result;

    result.u32 = pke_reg_read(PKE_INT_NOMASK_STATUS);
    if (result.bits.alarm_int_nomask == PKE_INT_NOMASK_ALARM) {
        result.u32 = PKE_INT_NOMASK_CLR; // clear warn interrupt
        pke_reg_write(PKE_INT_NOMASK_STATUS, result.u32);
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

static td_s32 hal_pke_error_code(void)
{
    pke_failure_flag result;
    result.u32 = pke_reg_read(PKE_FAILURE_FLAG);
    return result.u32;
}

/* wait pke done */
td_s32 hal_pke_wait_done(void)
{
    td_s32 ret = TD_FAILURE;

    ret = hal_pke_wait_free_int();
    if (ret != TD_SUCCESS) {
        hal_cipher_pke_debug();
        return ret;
    }

    if (hal_pke_check_robust_warn() == TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = hal_pke_error_code();
    if (ret != 0) {
        return ret;
    }
    return TD_SUCCESS;
}

td_void hal_pke_get_ram(td_u32 addr, td_u8 *ram, td_u32 klen)
{
    td_u32 val, i;
    if (ram == TD_NULL) {
        return;
    }
    for (i = klen; i >= WORD_WIDTH; i -= WORD_WIDTH) {
        val = pke_reg_read(addr + i - WORD_WIDTH);
        ram[klen - i + 0] = (val >> 24) & 0xFF; /* byte 0: bit 31~24 */
        ram[klen - i + 1] = (val >> 16) & 0xFF; /* byte 1: bit 23~16 */
        ram[klen - i + 2] = (val >> 8) & 0xFF;  /* byte 2: bit 15~8  */
        ram[klen - i + 3] = (val) & 0xFF;       /* byte 3: bit 7~0   */
    }
    return;
}

static td_u16 crc_table(td_u8 byte)
{
    td_u16 n_remainder = (td_u16)byte << BYTE_BITS;
    td_u16 m = 0;
    for (m = BYTE_BITS; m > 0; m--) {
        if (n_remainder & U16_MSB) {
            n_remainder = (n_remainder << 1) ^ CRC16_POLYNOMIAL;
        } else {
            n_remainder = (n_remainder << 1);
        }
    }
    return n_remainder;
}

static td_u16 hal_pke_crc16(td_u16 initial, const td_u8 *input, td_u32 len)
{
    td_u16 crc = initial, idx;
    td_s32 i, j;

    for (i = len - 1; i >= 0; i--) {
        j = (i % BYTES_PER_WORD) * 2; // 2 * len
        idx = ((crc >> BITS_PER_BYTE) ^ (*(input + i + 3 - j))) & 0xFF; // 3: offset
        crc = crc_table(idx) ^ (crc << BITS_PER_BYTE);
    }

    return crc;
}


td_void hal_pke_set_key(const td_u8 *inkey, td_u8 *outkey, td_u32 klen, pke_block *random, td_u32 pad_len)
{
    td_u32 i = 0;
    td_u16 crc16 = 0;

    /* The padded data will be 0x0 */
    for (i = (pad_len - 1); i >= klen; i--) {
        td_u32 idx = (pad_len - 1 - i) & PKE_LEN_BLOCK_MASK;
        outkey[pad_len - 1 - i] = random->byte[idx];
    }

    /* key must be protected */
    for (i = 0; i < klen; i++) {
        outkey[pad_len - 1 - i] = inkey[klen - 1 - i] ^ random->byte[(pad_len - 1 - i) & PKE_LEN_BLOCK_MASK];
    }

    crc16 = hal_pke_crc16(0x0000, random->byte, 4); // 4 is len of cube in crc
    crc16 = hal_pke_crc16(crc16, outkey, pad_len);

    /* set Random */
    pke_reg_write(PKE_KEY_RANDOM, crypto_cpu_to_be32(random->word[0]));

    /* set crc16 */
    pke_reg_write(PKE_KEY_CRC, crc16);

    return;
}

td_s32 hal_pke_align_len(td_u32 a_len, td_u32 *a_len_aligned, diff_len_in_chips len)
{
    crypto_param_require(a_len_aligned != TD_NULL);
    if (len == HAL_ALIGN_LEN) {
        if (a_len <= HAL_PKE_LEN_256) {
            *a_len_aligned = HAL_PKE_LEN_256;
        } else if (a_len <= HAL_PKE_LEN_384) {
            *a_len_aligned = HAL_PKE_LEN_384;
        } else if (a_len <= HAL_PKE_LEN_521) {
            *a_len_aligned = HAL_PKE_LEN_521;
        } else if (a_len <= HAL_PKE_LEN_2048) {
            *a_len_aligned = HAL_PKE_LEN_2048;
        } else if (a_len <= HAL_PKE_LEN_3072) {
            *a_len_aligned = HAL_PKE_LEN_3072;
        } else if (a_len <= HAL_PKE_LEN_4096) {
            *a_len_aligned = HAL_PKE_LEN_4096;
        } else {
            return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        }
        return TD_SUCCESS;
    } else if (len == HAL_SET_KEY_LEN) {
        *a_len_aligned = SET_KEY_LEN;
    } else if (len == HAL_RAM_SECTION_LEN) {
        *a_len_aligned = RAM_SECTION_LEN;
    } else if (len == HAL_ECC_512_LEN) {
        *a_len_aligned = HAL_PKE_LEN_512;
    } else if (len == HAL_ECC_521_LEN) {
        *a_len_aligned = HAL_PKE_LEN_521;
    } else {
        return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    return TD_SUCCESS;
}

#if defined(CRYPTO_OS_INT_SUPPORT)
td_s32 hal_cipher_pke_register_wait_func(td_void *wait,
    crypto_wait_timeout_interruptible wait_func, td_u32 timeout_ms)
{
    crypto_hal_func_enter();

    g_pke_wait_event.wait = wait;
    g_pke_wait_event.wait_func = wait_func;
    g_pke_wait_event.timeout_ms = timeout_ms;
    g_pke_wait_event.wait_done = TD_FALSE;

    crypto_hal_func_exit();
    return CRYPTO_SUCCESS;
}
#endif