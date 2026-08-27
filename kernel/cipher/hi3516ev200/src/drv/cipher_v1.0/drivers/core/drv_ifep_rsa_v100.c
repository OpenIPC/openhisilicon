/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_ifep_rsa_v100.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_ifep_rsa_v100.h"
#include "drv_srsa.h"
#include "drv_trng.h"
#include "drv_klad.h"

#ifdef CHIP_IFEP_RSA_VER_V100

/*************************** Structure Definition ****************************/
/** \addtogroup     rsa */
/** @{ */  /** <!-- [rsa] */


/*! Define the time out */
#define RSA_TIME_OUT             5000

/* rsa support rand mask or not */
#define RSA_SUB_VER_RAND_MASK    (0x20160907)
#define RSA_SUB_VER_NORMAL       (0x00)

/* crc 16 */
#define CRC16_TABLE_SIZE          256
#define U16_MSB                   0x8000
#define BYTE_MASK                 0xFF
#define CRC16_POLYNOMIAL          0x1021
#define BLOCK_BYTES               0x08

/* rsa work mode */
#define RSA_MODE_EXP              0x00
#define RSA_MODE_CLEAR_RAM        0x02

/*! rsa already initialize or not */
static hi_u32 rsa_initialize = HI_FALSE;

/*! Define the context of rsa */
typedef struct {
    hi_u32 rsa_sub_ver;
    hi_u32 done;                   /*!<  calculation finish flag*/
    crypto_queue_head  queue;   /*!<  quene list */
} rsa_hard_context;

static rsa_hard_context rsainfo;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      rsa */
/** @{ */  /** <!--[rsa]*/

#ifdef RSA_RAND_MASK

static u16 crc_table[CRC16_TABLE_SIZE];

static void drv_rsa_crc16_init(void)
{
    u16 remainder;
    u16 n, m;
    u16  *local_crc_table = crc_table;

    for (n = 0; n < CRC16_TABLE_SIZE; n ++) {
        remainder = (HI_U16)n << BITS_IN_BYTE;
        for (m = BITS_IN_BYTE; m > 0; m --) {
            if (remainder & U16_MSB) {
                remainder = (remainder << 1) ^ CRC16_POLYNOMIAL;
            } else {
                remainder = (remainder << 1);
            }
        }
        *(local_crc_table + n) = remainder;
    }
}

static u16 drv_rsa_crc16_block(u16 crc, hi_u8 block[BLOCK_BYTES], hi_u8 rand_number[BLOCK_BYTES])
{
    hi_u8 i, j;
    hi_u8 val;

    for (i = 0; i < BLOCK_BYTES / WORD_WIDTH; i++) {
        for (j = 0; j < WORD_WIDTH; j++) {
            val = block[i * 4 + 3 - j] ^ rand_number[i * 4 + 3 - j];
            crc = (crc << BITS_IN_BYTE) ^ crc_table[((crc >> BITS_IN_BYTE) ^ val) & BYTE_MASK];
        }
    }
    return crc;
}

static u16 drv_rsa_key_crc(hi_u8 *n, hi_u8 *k, hi_u32 klen, hi_u32 randnum[2])
{
    hi_u32 i;
    u16 crc = 0;

    for (i = 0; i < klen; i += BLOCK_BYTES) {
        crc = drv_rsa_crc16_block(crc, n + i, (hi_u8 *)randnum);
    }
    for (i = 0; i < klen; i += BLOCK_BYTES) {
        crc = drv_rsa_crc16_block(crc, k + i, (hi_u8 *)randnum);
    }
    return crc;
}
#endif

#ifdef CRYPTO_OS_INT_SUPPORT

/*! set interrupt */
static void rsa_set_interrupt(void)
{
    hi_u32 int_valid = 0, int_num = 0;
    sec_rsa_int_en int_en;

    module_get_attr(CRYPTO_MODULE_ID_IFEP_RSA, &int_valid, &int_num, HI_NULL);
    if (int_valid == HI_FALSE) {
        return;
    }
    if (rsainfo.rsa_sub_ver != RSA_SUB_VER_RAND_MASK) {
        return;
    }

    int_en.u32 = IFEP_RSA_READ(REG_SEC_RSA_INT_EN);

    /*The top interrupt switch only can be enable/disable by secure CPU*/
    int_en.bits.rsa_int_en = 1;
    int_en.bits.int_en = 1;
    IFEP_RSA_WRITE(REG_SEC_RSA_INT_EN, int_en.u32);
    HI_LOG_INFO("RSA_INT_EN: 0x%x\n", int_en.u32);

    return;
}

static hi_u32 drv_rsa_done_notify(void)
{
    sec_rsa_int_status int_st;
    sec_rsa_int_raw int_raw;

    int_st.u32 = IFEP_RSA_READ(REG_SEC_RSA_INT_STATUS);
    int_raw.u32 = 0x00;

    HI_LOG_DEBUG("REG_SEC_RSA_INT_STATUS: 0x%x\n", int_st.u32);

    /*Clean raw int*/
    int_raw.bits.rsa_int_raw = 1;
    IFEP_RSA_WRITE(REG_SEC_RSA_INT_RAW, int_raw.u32);

    return int_st.bits.rsa_int_status;
}

/*! rsa interrupt process function */
static irqreturn_t drv_rsa_interrupt_isr(hi_s32 irq, void *devId)
{
    CRYPTO_UNUSED(irq);

    drv_rsa_done_notify();

    rsainfo.done = HI_TRUE;
    HI_LOG_DEBUG("rsa wake up\n");
    crypto_queue_wait_up(&rsainfo.queue);

    return IRQ_HANDLED;
}

/*! rsa register interrupt process function */
static hi_s32 drv_rsa_register_interrupt(void)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 int_valid = 0, int_num = 0;
    const char *name;

    HI_LOG_FUNC_ENTER();

    module_get_attr(CRYPTO_MODULE_ID_IFEP_RSA, &int_valid, &int_num, &name);
    if (int_valid == HI_FALSE) {
        return HI_SUCCESS;
    }
    if (rsainfo.rsa_sub_ver != RSA_SUB_VER_RAND_MASK) {
        return HI_SUCCESS;
    }

    /* request irq */
    ret = crypto_request_irq(int_num, drv_rsa_interrupt_isr, name);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("Irq request failure, irq = %d", int_num);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_REGISTER_IRQ);
        return ret;
    }

    /* initialize queue list*/
    crypto_queue_init(&rsainfo.queue);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/*! rsa unregister interrupt process function */
static void drv_rsa_unregister_interrupt(void)
{
    hi_u32 int_valid = 0, int_num = 0;
    const char *name = HI_NULL;

    HI_LOG_FUNC_ENTER();

    module_get_attr(CRYPTO_MODULE_ID_IFEP_RSA, &int_valid, &int_num, &name);

    if (int_valid == HI_FALSE) {
        return;
    }

    if (rsainfo.rsa_sub_ver != RSA_SUB_VER_RAND_MASK) {
        return;
    }

    /* free irq */
    HI_LOG_INFO("rsa free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    HI_LOG_FUNC_EXIT();

    return;
}
#endif

hi_s32 drv_rsa_init(void)
{
    HI_LOG_FUNC_ENTER();

    HI_LOG_INFO("enable rsa\n");

    if (rsa_initialize == HI_TRUE) {
        return HI_SUCCESS;
    }

#ifdef RSA_RAND_MASK
    drv_rsa_crc16_init();
#endif

    module_enable(CRYPTO_MODULE_ID_IFEP_RSA);

    /* RSA request the TRNG must valid */
    module_enable(CRYPTO_MODULE_ID_TRNG);

    rsainfo.rsa_sub_ver = IFEP_RSA_READ(REG_SEC_RSA_VERSION_ID);
    HI_LOG_INFO("rsa version 0x%x\n", rsainfo.rsa_sub_ver);
    module_disable(CRYPTO_MODULE_ID_IFEP_RSA);

#ifdef CRYPTO_OS_INT_SUPPORT
    {
        hi_s32 ret = HI_FAILURE;

        HI_LOG_INFO("rsa register interrupt function\n");
        ret = drv_rsa_register_interrupt();
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("error, register interrupt failed\n");
            HI_LOG_PRINT_FUNC_ERR(drv_rsa_register_interrupt, ret);
            return ret;
        }
    }
#endif

    rsa_initialize = HI_TRUE;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 drv_rsa_deinit(void)
{
    HI_LOG_FUNC_ENTER();

    if (rsa_initialize == HI_FALSE) {
        return HI_SUCCESS;
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_rsa_unregister_interrupt();
#endif

    rsa_initialize = HI_FALSE;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static void drv_rsa_resume(void)
{
    HI_LOG_FUNC_ENTER();

    module_enable(CRYPTO_MODULE_ID_IFEP_RSA);

#ifdef CRYPTO_OS_INT_SUPPORT
    HI_LOG_INFO("RSA interrupt configure\n");
    rsa_set_interrupt();
#endif

    HI_LOG_FUNC_EXIT();
    return;
}

static void drv_rsa_suspend(void)
{
    HI_LOG_FUNC_ENTER();

    module_disable(CRYPTO_MODULE_ID_IFEP_RSA);

    HI_LOG_FUNC_EXIT();
    return;
}

static void drv_rsa_set_width(rsa_key_width width)
{
    sec_rsa_mod_reg ctrl;

    ctrl.u32 = 0x00;
    ctrl.bits.sec_rsa_mod_sel = RSA_MODE_EXP;
    ctrl.bits.sec_rsa_key_width = width;
    IFEP_RSA_WRITE(REG_SEC_RSA_MOD_REG, ctrl.u32);
    HI_LOG_INFO("REG_SEC_RSA_MOD_REG 0x%x\n", ctrl.u32);
    return;
}

static hi_s32 drv_rsa_set_key(hi_u32 ca_type, hi_u8 *n, hi_u8 *d, hi_u32 klen, hi_u32 random[2])
{
    hi_u32 i = 0, id = 0;
    hi_u32 val = 0x00;
    hi_s32 ret = HI_FAILURE;

    /**
     * The even word shell XOR with even random[0]
     * The odd word shell XOR with odd random[1]
     * The random may be zero.
     * Must set N before set E.
     * The E must padding with zero.
     */

    /* Set N */
    for (i = 0; i < klen; i += WORD_WIDTH) {
        crypto_memcpy(&val, sizeof(hi_u32), n + i, WORD_WIDTH);
        val ^= random[id];
        IFEP_RSA_WRITE(REG_SEC_RSA_WSEC_REG, val);

        /* switch between even and odd */
        id ^= 0x01;
    }

    /* Set D */
    if (ca_type != HI_CIPHER_KEY_SRC_USER) {
        ret = drv_cipher_klad_load_key(0, ca_type, HI_CIPHER_KLAD_TARGET_RSA, d, klen);
        if ( ret != HI_SUCCESS ) {
            HI_LOG_ERROR("drv_cipher_klad_load_key, error!\n");
            return ret;
        }
    } else {
        for (i = 0; i < klen; i += WORD_WIDTH) {
            crypto_memcpy(&val, sizeof(hi_u32), d + i, WORD_WIDTH);
            val ^= random[id];
            IFEP_RSA_WRITE(REG_SEC_RSA_WSEC_REG, val);

            /* switch between even and odd */
            id ^= 0x01;
        }
    }
    return HI_SUCCESS;
}

static void drv_rsa_set_input(hi_u8 *in, hi_u32 klen)
{
    hi_u32 i = 0;
    hi_u32 val = 0x00;

    for (i = 0; i < klen; i += WORD_WIDTH) {
        crypto_memcpy(&val, sizeof(hi_u32), in + i, WORD_WIDTH);
        IFEP_RSA_WRITE(REG_SEC_RSA_WDAT_REG, val);
    }
    return;
}

static void drv_rsa_get_output(hi_u8 *out, hi_u32 klen)
{
    hi_u32 i = 0;
    hi_u32 val = 0x00;

    for (i = 0; i < klen; i += 4) {
        val = IFEP_RSA_READ(REG_SEC_RSA_RRSLT_REG);
        crypto_memcpy(out + i, sizeof(hi_u32), &val, 4);
    }
    return;
}

static hi_u32 drv_rsa_get_klen(rsa_key_width width)
{
    hi_u32 klen = 0x00;

    /* nonsupport rsa 3072, can compute it as 4096 */

    switch (width) {
        case RSA_KEY_WIDTH_1024: {
            klen = RSA_KEY_LEN_1024;
            break;
        }
        case RSA_KEY_WIDTH_2048: {
            klen = RSA_KEY_LEN_2048;
            break;
        }
        case RSA_KEY_WIDTH_4096: {
            klen = RSA_KEY_LEN_4096;
            break;
        }
        default: {
            HI_LOG_ERROR("error, nonsupport RSA width %d\n", width);
            klen = 0;
            break;
        }
    }

    return klen;
}

static void drv_rsa_start(void)
{
    sec_rsa_start_reg start;

    HI_LOG_FUNC_ENTER();

    rsainfo.done = HI_FALSE;

    start.u32 = 0x00;

    if (rsainfo.rsa_sub_ver == RSA_SUB_VER_RAND_MASK) {
        start.bits.sec_rsa_start_reg = 0x05;
    } else {
        start.bits.sec_rsa_start_reg = 0x01;
    }

    IFEP_RSA_WRITE(REG_SEC_RSA_START_REG, start.u32);
    HI_LOG_INFO("REG_SEC_RSA_START_REG 0x%x\n", start.u32);

    HI_LOG_FUNC_EXIT();

    return;
}

static hi_s32 drv_rsa_wait_done(void)
{
    hi_u32 int_valid = 0, int_num = 0;
    hi_u32 i;
    sec_rsa_busy_reg ready;
    const char *name = HI_NULL;

    HI_LOG_FUNC_ENTER();

    module_get_attr(CRYPTO_MODULE_ID_IFEP_RSA, &int_valid, &int_num, &name);

#ifdef CRYPTO_OS_INT_SUPPORT
    /* interrupt support, wait irq*/
    if ((rsainfo.rsa_sub_ver == RSA_SUB_VER_RAND_MASK) && (int_valid == HI_TRUE)) {
        hi_s32 ret = HI_FAILURE;

        /* wait interrupt */
        ret = crypto_queue_wait_timeout(rsainfo.queue, &rsainfo.done, RSA_TIME_OUT);
        if ((ret <= 0x00) && (ret != -ERESTARTSYS)) {
            HI_LOG_ERROR("wait done timeout\n");
            HI_LOG_PRINT_FUNC_ERR(crypto_queue_wait_timeout, ret);
            return HI_ERR_CIPHER_TIMEOUT;
        }
    } else
#endif
    {
        /* wait ready */
        for (i = 0; i < RSA_TIME_OUT; i++) {
            ready.u32 = IFEP_RSA_READ(REG_SEC_RSA_BUSY_REG);
            if (!ready.bits.sec_rsa_busy_reg) {
                break;
            }
            crypto_msleep(1);
        }

        if (i >= RSA_TIME_OUT) {
            HI_LOG_ERROR("error, rsa wait free timeout\n");
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_TIMEOUT);
            return HI_ERR_CIPHER_TIMEOUT;
        }
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static void drv_rsa_randnum(hi_u8 *n, hi_u8 *k, hi_u32 klen, hi_u32 random[2])
{
    HI_LOG_FUNC_ENTER();

#ifdef RSA_RAND_MASK
    if (rsainfo.rsa_sub_ver == RSA_SUB_VER_RAND_MASK) {
        u16 crc16 = 0;

        random[0] = get_rand();
        random[1] = get_rand();

        crc16 = drv_rsa_key_crc(n, k, klen, random);
        HI_LOG_INFO("random 0x%x 0x%x, CRC16: 0x%x\n", random[0], random[1], crc16);
        IFEP_RSA_WRITE(REG_SEC_RSA_KEY_RANDOM_1, random[0]);
        IFEP_RSA_WRITE(REG_SEC_RSA_KEY_RANDOM_2, random[1]);
        IFEP_RSA_WRITE(REG_SEC_RSA_CRC16_REG, crc16);
    } else
#endif
    {
        random[0] = 0x00;
        random[1] = 0x00;
    }

    HI_LOG_FUNC_EXIT();

    return;
}

static hi_s32 drv_rsa_clean_ram(void)
{
    hi_s32 ret = HI_FAILURE;
    sec_rsa_mod_reg ctrl;

    ctrl.u32 = IFEP_RSA_READ(REG_SEC_RSA_MOD_REG);
    ctrl.bits.sec_rsa_mod_sel = RSA_MODE_CLEAR_RAM;
    ctrl.bits.sec_rsa_data0_clr = 1;
    ctrl.bits.sec_rsa_data1_clr = 1;
    ctrl.bits.sec_rsa_data2_clr = 1;
    IFEP_RSA_WRITE(REG_SEC_RSA_MOD_REG, ctrl.u32);

    /* start */
    drv_rsa_start();

    /* wait done */
    ret = drv_rsa_wait_done();
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(drv_rsa_wait_done, ret);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 drv_rsa_error_code(void)
{
    hi_u32 code = 0;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    if (rsainfo.rsa_sub_ver == RSA_SUB_VER_RAND_MASK) {
        code = IFEP_RSA_READ(REG_SEC_RSA_ERROR_REG);
    }

    if (code == 0x00) {
        ret = HI_SUCCESS;
    } else {
        HI_LOG_ERROR("rsa error code: 0x%x\n", code);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_HARD_STATUS);
        return HI_ERR_CIPHER_HARD_STATUS;
    }

    HI_LOG_FUNC_EXIT();

    return ret;
}

hi_s32 drv_ifep_rsa_exp_mod(hi_u32 ca_type, hi_u8 *n, hi_u8 *k, hi_u8 *in, hi_u8 *out, rsa_key_width width)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 klen = 0;
    hi_u32 random[2] = {0, 0};

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_INITED(rsa_initialize);

    klen = drv_rsa_get_klen(width);
    if (klen == 0) {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    drv_rsa_resume();

    /* set rsa width */
    drv_rsa_set_width(width);

    /* config randnum */
    drv_rsa_randnum(n, k, klen, random);

    /* set rsa key */
    ret = drv_rsa_set_key(ca_type, n, k, klen, random);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(drv_rsa_set_key, ret);
        ret = HI_ERR_CIPHER_ILLEGAL_KEY;
        goto exit;
    }

    /* set input data */
    drv_rsa_set_input(in, klen);

    /* start */
    drv_rsa_start();

    /* wait done */
    ret = drv_rsa_wait_done();
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(drv_rsa_wait_done, ret);
        ret = HI_ERR_CIPHER_TIMEOUT;
        goto exit;
    }

    /* get input data */
    drv_rsa_get_output(out, klen);

    ret = drv_rsa_error_code();
exit:
    /* clean key and data */
    (void)drv_rsa_clean_ram();
    drv_rsa_suspend();

    HI_LOG_FUNC_EXIT();

    return ret;
}

void drv_ifep_rsa_get_capacity(rsa_capacity *capacity)
{
    crypto_memset(capacity, sizeof(rsa_capacity), 0,  sizeof(rsa_capacity));

    capacity->rsa = CRYPTO_CAPACITY_SUPPORT;

    return;
}

/** @} */  /** <!-- ==== API declaration end ==== */

#endif //End of CHIP_RSA_VER_V100
