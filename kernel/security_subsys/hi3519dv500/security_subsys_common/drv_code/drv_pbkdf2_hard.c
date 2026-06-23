/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_hash.h"
#include "drv_inner.h"
#include "drv_trng.h"

#include "hal_hash.h"

#include "crypto_drv_common.h"

#define PBKDF2_COMPAT_ERRNO(err_code)     DRV_COMPAT_ERRNO(ERROR_MODULE_HASH, err_code)
#define pbkdf2_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, PBKDF2_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")
#define CRYPTO_U8_TO_U32_BIT_SHIFT(data, i) \
    ((td_u32)(data)[(i)*4]  | ((td_u32)(data)[(i)*4+1] << 8) | ((td_u32)(data)[(i)*4+2] << 16) | \
    ((td_u32)(data)[(i)*4+3]<< 24))

/* Current CPU ID Status .
8'h35: AIDSP;
8'h6a: PCPU;
8'ha5: TEE;
8'haa: ACPU.
*/
#define PCPU_STAT     0x6a
#define AIDSP_STAT    0x35
#define TEE_STAT      0xa5
#define ACPU_STAT     0xaa

/* KDF */

#define KM_CPU_PCPU  4
#define KM_CPU_AIDSP 5
#define KM_CPU_TEE   2
#define KM_CPU_REE   1
#define KM_CPU_IDLE  0

#define PBKDF2_ALG_HMAC_SHA1       1
#define PBKDF2_ALG_HMAC_SHA256     0
#define PBKDF2_ALG_HMAC_SHA384     3
#define PBKDF2_ALG_HMAC_SHA512     4
#define PBKDF2_ALG_HMAC_SM3        5

#define HMAC_SHA1_OUTPUT_LEN       20
#define HMAC_SHA256_OUTPUT_LEN     32
#define HMAC_SHA384_OUTPUT_LEN     48
#define HMAC_SHA512_OUTPUT_LEN     64
#define HMAC_SM3_OUTPUT_LEN        32

#define KDF_KEY_BLOCK_SIZE_512     64
#define KDF_KEY_BLOCK_SIZE_1024    128
#define KDF_KEY_CONFIG_LEN         32
#define KDF_SALT_CONFIG_LEN        32
#define KDF_VAL_CONFIG_LEN         16
#define KDF_ALG_TYPE               5
#define DRV_KDF_OTP_KEY_MRK1       0
#define DRV_KDF_OTP_KEY_USK        1
#define DRV_KDF_OTP_KEY_RUSK       2

/*
    pbkdf2_key_config
*/
#define KDF_SW_GEN              3

/*
 * rkp
 */
#define RKP_LOCK                (0x000)
#define RKP_CMD_CFG             (0x004)
#define KDF_ERROR               (0x008)
#define RKP_DEOB_CFG            (0x020)
#define DEOB_ERROR              (0x028)
#define RK_RDY                  (0x050)
#define RKP_USD_DIS             (0x054)
#define RKP_LOW_POWER           (0x058)
#define RKP_INIT                (0x05C)
#define SW_CFG                  (0x060)
#define RKP_RAW_INT             (0x010)
#define RKP_INT_ENABLE          (0x014)
#define RKP_INT                 (0x018)
#define RKP_PBKDF2_DATA(a)      (0x100 + 4 * (a)) /* a 0~31 */
#define RKP_PBKDF2_KEY(a)       (0x180 + 4 * (a)) /* a 0~31 */
#define RKP_PBKDF2_VAL(b)       (0x200 + 4 * (b)) /* b 0~16 */
#define RKP_USD(c)              (0x300 + 4 * (c)) /* c 0~8 */

/* salt len */
#define SALT_RESERVED_LEN_512   21
#define SALT_RESERVED_LEN_256   13

/* Define the union U_RKP_RKP_LOCK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    km_lock_status            : 3; /* [2..0]  */
        unsigned int    reserved                  : 29; /* [31..3]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_lock;

/* Define the union U_RKP_CMD_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    sw_calc_req               : 1; /* [0]  */
        unsigned int    pbkdf2_alg_sel_cfg        : 3; /* [3..1]  */
        unsigned int    pbkdf2_key_sel_cfg        : 5; /* [8..4]  */
        unsigned int    reserved                  : 7; /* [15..9]  */
        unsigned int    rkp_pbkdf_calc_time       : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} rkp_cmd_cfg;

static td_s32 priv_wait_rkp_to_be_lock(void)
{
    rkp_lock lock_val = {0};
    td_u32 rkp_config_val = KM_CPU_IDLE;
    crypto_cpu_type cpu = crypto_get_cpu_type();
    switch (cpu) {
        case CRYPTO_CPU_TYPE_SCPU:
            rkp_config_val = KM_CPU_TEE;
            break;
        case CRYPTO_CPU_TYPE_ACPU:
            rkp_config_val = KM_CPU_REE;
            break;
        default:
            return PBKDF2_COMPAT_ERRNO(ERROR_UNEXPECTED);
    }

    km_reg_write(RKP_LOCK, rkp_config_val);
    lock_val.u32 = km_reg_read(RKP_LOCK);
    if (lock_val.bits.km_lock_status != rkp_config_val) {
        return PBKDF2_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }

    return TD_SUCCESS;
}

#define RKP_TIME_OUT                1000000
static td_s32 priv_rkp_lock(td_void)
{
    td_u32 i = 0;
    td_s32 ret;

    for (i = 0; i < RKP_TIME_OUT; i++) {
        ret = priv_wait_rkp_to_be_lock();
        if (ret == TD_SUCCESS) {
            break;
        }
        crypto_udelay(1);
    }

    if (i >= RKP_TIME_OUT) {
        crypto_log_err("priv_rkp_lock busy.\n");
        return PBKDF2_COMPAT_ERRNO(ERROR_RKP_LOCK_TIMEOUT);
    }

    return TD_SUCCESS;
}

static td_void priv_rkp_unlock(void)
{
    km_reg_write(RKP_LOCK, KM_CPU_IDLE);
}

static td_s32 priv_kdf_param_salt_padding(td_u8 *salt_pad, td_u32 pad_len, td_u32 block_len,
    const crypto_kdf_pbkdf2_param *param, td_u32 cnt_bit)
{
    td_u32 i = 0;
    td_u32 j = 0;
    td_u32 salt_pad_total_len = 0;
    td_u32 random_val = 0;
    td_s32 ret;
    crypto_unused(pad_len);
    /* Formatting of the PBKDF2 salt Padding
     * The PBKDF2 salt Padding formatted as shown below:
     *                        big endian
     *         salt length || 32 bit(i) || 1 bit(1) || 2 word tail length
     *
     * sm3:
     * sha1:
     * sha256:   salt_len  ||   32 bit  ||   8 bit  ||  64 bit
     *
     * sha384:
     * sha512:   salt_len  ||   32 bit  ||   8 bit  ||  128 bit
     */
    ret = drv_cipher_trng_get_random(&random_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed\n");

    for (i = 0, j = random_val % param->slen; i < param->slen; i++) {
        salt_pad[j] = param->salt[j]; /* copy salt */
        j++;
        j %= param->slen;
    }

    salt_pad[i + 3] = cnt_bit & 0xff;         /* 3: need fill 32bit i, big endian padding */
    salt_pad[i + 2] = (cnt_bit >> 8) & 0xff;  /* 2/8: need fill 32bit i, big endian padding */
    salt_pad[i + 1] = (cnt_bit >> 16) & 0xff; /* 16: need fill 32bit i, big endian padding */
    salt_pad[i] = (cnt_bit >> 24) & 0xff;     /* 24: need fill 32bit i, big endian padding */
    salt_pad[i + 4] = 0x80;                   /* 4: message data fill 1 */

    if (block_len == KDF_KEY_BLOCK_SIZE_512) {
        /* need to connect i_key_pad length, the i_key_pad length is block size */
        salt_pad_total_len = KDF_KEY_BLOCK_SIZE_512 * CRYPTO_BITS_IN_BYTE;
    } else {
        /* need to connect i_key_pad length, the i_key_pad length is block size */
        salt_pad_total_len = KDF_KEY_BLOCK_SIZE_1024 * CRYPTO_BITS_IN_BYTE;
    }
    salt_pad_total_len += 32 + param->slen * 8;          /* length: 32 bit i + slen * 8 */

    salt_pad[block_len - 1] = salt_pad_total_len & 0xff;
    salt_pad[block_len - 2] = (salt_pad_total_len >> 8) & 0xff;  /* 2: 2bit 8: right move 8 */
    salt_pad[block_len - 3] = (salt_pad_total_len >> 16) & 0xff; /* 3: 3bit 16: right move 16 */
    salt_pad[block_len - 4] = (salt_pad_total_len >> 24) & 0xff; /* 4: 4bit 24: right move 24 */

    return TD_SUCCESS;
}


static td_s32 priv_kdf_config_val(crypto_hash_type hash_type)
{
    td_u32 i = 0;
    td_s32 ret = TD_SUCCESS;
    td_u32 val_len = 0;
    td_u32 state_iv[64];

    ret = drv_hash_get_state_iv(hash_type, &val_len, state_iv, sizeof(state_iv));
    if (ret != TD_SUCCESS) {
        return PBKDF2_COMPAT_ERRNO(ERROR_UNEXPECTED);
    }

    val_len /= CRYPTO_WORD_WIDTH;

    for (i = 0; i < val_len; i++) {
        km_reg_write(RKP_PBKDF2_VAL(i), state_iv[i]);
    }

    return TD_SUCCESS;
}

static td_s32 priv_kdf_config_data(const td_u8 *pswd_pad, td_u32 pswd_len, const td_u8 *salt_pad, td_u32 salt_len)
{
    td_u32 ret;
    td_u32 i = 0;
    td_u32 j = 0;
    td_u32 random_val = 0;
    td_u32 x = sizeof(td_u32) / sizeof(td_u8);
    td_u32 pswd_tmp = 0;
    td_u32 salt_tmp = 0;

    if (pswd_len != KDF_KEY_CONFIG_LEN * x ||
        salt_len != KDF_KEY_CONFIG_LEN * x) {
        return PBKDF2_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    ret = drv_cipher_trng_get_random(&random_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed\n");

    for (i = 0, j = random_val % KDF_KEY_CONFIG_LEN; i < KDF_KEY_CONFIG_LEN; i++) {
        pswd_tmp = CRYPTO_U8_TO_U32_BIT_SHIFT(pswd_pad, j);
        km_reg_write(RKP_PBKDF2_KEY(j), pswd_tmp);
        j++;
        j %= KDF_KEY_CONFIG_LEN;
    }

    ret = drv_cipher_trng_get_random(&random_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed\n");

    for (i = 0, j = random_val % KDF_SALT_CONFIG_LEN; i < KDF_SALT_CONFIG_LEN; i++) {
        salt_tmp = CRYPTO_U8_TO_U32_BIT_SHIFT(salt_pad, j);
        km_reg_write(RKP_PBKDF2_DATA(j), salt_tmp);
        j++;
        j %= KDF_SALT_CONFIG_LEN;
    }

    return TD_SUCCESS;
}

static td_void priv_kdf_sw_config_start(td_u32 alg_cfg, td_u16 count)
{
    rkp_cmd_cfg cfgval = {0};

    cfgval.u32 = km_reg_read(RKP_CMD_CFG);
    cfgval.bits.rkp_pbkdf_calc_time = count;
    cfgval.bits.pbkdf2_alg_sel_cfg = alg_cfg;
    cfgval.bits.pbkdf2_key_sel_cfg = KDF_SW_GEN;
    cfgval.bits.sw_calc_req = 0x1;         /* start calculation */
    km_reg_write(RKP_CMD_CFG, cfgval.u32);
}

static td_s32 priv_rkp_wait_idle(void)
{
    rkp_cmd_cfg cfg_val = {0};
    td_s32 ret;
    td_u32 i = 0;

    for (i = 0; i < RKP_TIME_OUT; ++i) {
        cfg_val.u32 = km_reg_read(RKP_CMD_CFG);
        if (cfg_val.bits.sw_calc_req == 0x0) {
            km_reg_write(RKP_RAW_INT, 0x1);
            break;
        }
        crypto_udelay(1);
    }
    if (i < RKP_TIME_OUT) {
        ret = TD_SUCCESS;
    } else {
        ret = PBKDF2_COMPAT_ERRNO(ERROR_RKP_CALC_TIMEOUT);
    }

    return ret;
}

static td_void priv_rkp_chk_err(void)
{
    td_s32 kdf_err;

    kdf_err = km_reg_read(KDF_ERROR);
    if (kdf_err != 0) {
        crypto_log_err("KDF Error: 0x%x\n", kdf_err);
    }
}

static td_s32 priv_kdf_calculation(const crypto_kdf_pbkdf2_param *param, td_u32 alg_cfg, td_u32 block_size,
    td_u32 cnt_bit)
{
    td_u8 kdf_passwd_pad[KDF_KEY_BLOCK_SIZE_1024];
    td_u8 kdf_salt_pad[KDF_KEY_BLOCK_SIZE_1024];

    td_s32 ret;

    (td_void)memset_s(kdf_passwd_pad, KDF_KEY_BLOCK_SIZE_1024, 0, KDF_KEY_BLOCK_SIZE_1024);
    (td_void)memset_s(kdf_salt_pad, KDF_KEY_BLOCK_SIZE_1024, 0, KDF_KEY_BLOCK_SIZE_1024);

    /* password padding */
    if (param->plen != 0) {
        ret = memcpy_s(kdf_passwd_pad, KDF_KEY_BLOCK_SIZE_1024, param->password, param->plen);
        crypto_chk_return(ret != EOK, ret, "memcpy_s failed\n");
    }

    /* salt padding */
    ret = priv_kdf_param_salt_padding(kdf_salt_pad, KDF_KEY_BLOCK_SIZE_1024, block_size, param, cnt_bit);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "priv_kdf_param_salt_padding failed\n");

    ret = priv_kdf_config_val(param->hash_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "priv_kdf_config_val failed\n");

    ret = priv_kdf_config_data(kdf_passwd_pad, KDF_KEY_BLOCK_SIZE_1024, kdf_salt_pad, KDF_KEY_BLOCK_SIZE_1024);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "priv_kdf_config_data failed\n");

    priv_kdf_sw_config_start(alg_cfg, param->count);
    ret = priv_rkp_wait_idle();
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "priv_rkp_wait_idle failed\n");

    priv_rkp_chk_err();

exit_clean:
    (td_void)memset_s(kdf_passwd_pad, KDF_KEY_BLOCK_SIZE_1024, 0, KDF_KEY_BLOCK_SIZE_1024);
    (td_void)memset_s(kdf_salt_pad, KDF_KEY_BLOCK_SIZE_1024, 0, KDF_KEY_BLOCK_SIZE_1024);
    return ret;
}

static td_void priv_kdf_clear_reg_key(td_void)
{
    td_u32 i = 0;
    td_u32 clear_key_val = 0;

    (td_void)drv_cipher_trng_get_random(&clear_key_val);

    for (i = 0; i < KDF_KEY_CONFIG_LEN; i++) {
        km_reg_write(RKP_PBKDF2_KEY(i), clear_key_val);
    }

    for (i = 0; i < KDF_SALT_CONFIG_LEN; i++) {
        km_reg_write(RKP_PBKDF2_DATA(i), clear_key_val);
    }

    for (i = 0; i < KDF_VAL_CONFIG_LEN; i++) {
        km_reg_write(RKP_PBKDF2_VAL(i), clear_key_val);
    }
}

#define PBKDF2_ALG_HMAC_SHA256     0
#define PBKDF2_ALG_HMAC_SHA384     3
#define PBKDF2_ALG_HMAC_SHA512     4
#define PBKDF2_ALG_HMAC_SM3        5

static td_s32 priv_get_alg_cfg(crypto_hash_type hash_type, td_u32 *alg_cfg, td_u32 *block_size, td_u32 *out_key_len)
{
    *block_size = CRYPTO_HASH_GET_BLOCK_SIZE(hash_type) / CRYPTO_BITS_IN_BYTE;
    *out_key_len = CRYPTO_HASH_GET_RESULT_SIZE(hash_type) / CRYPTO_BITS_IN_BYTE;
    switch (hash_type) {
        case CRYPTO_HASH_TYPE_HMAC_SHA256:
            *alg_cfg = PBKDF2_ALG_HMAC_SHA256;
            break;
        case CRYPTO_HASH_TYPE_HMAC_SHA384:
            *alg_cfg = PBKDF2_ALG_HMAC_SHA384;
            break;
        case CRYPTO_HASH_TYPE_HMAC_SHA512:
            *alg_cfg = PBKDF2_ALG_HMAC_SHA512;
            break;
        case CRYPTO_HASH_TYPE_HMAC_SM3:
            *alg_cfg = PBKDF2_ALG_HMAC_SM3;
            break;
        default:
            return PBKDF2_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    return TD_SUCCESS;
}

static td_s32 priv_kdf_output_key_remain(td_u8 *out, const td_u32 out_len, td_u32 out_key_len, td_u32 cnt_count,
    td_u32 remain_num)
{
    td_s32 ret;
    td_u32 i = 0;
    td_u32 readval[KDF_VAL_CONFIG_LEN];
    for (i = 0; i < out_key_len / CRYPTO_WORD_WIDTH; i++) {
        readval[i] = km_reg_read(RKP_PBKDF2_VAL(i));
    }

    ret = memcpy_s(out + cnt_count * out_key_len, out_len - cnt_count * out_key_len, readval, remain_num);
    crypto_chk_return(ret != EOK, PBKDF2_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return TD_SUCCESS;
}

static td_s32 priv_kdf_output_key(td_u8 *out, const td_u32 out_len, td_u32 out_key_len, td_u32 cnt_count)
{
    td_u32 i = 0;
    td_u32 j = 0;
    td_u32 readval[KDF_VAL_CONFIG_LEN];
    td_u32 random_val = 0;
    td_u32 ret;
    
    ret = drv_cipher_trng_get_random(&random_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed\n");

    for (i = 0, j = random_val % (out_key_len / CRYPTO_WORD_WIDTH); i < out_key_len / CRYPTO_WORD_WIDTH; i++) {
        readval[j] = km_reg_read(RKP_PBKDF2_VAL(j));
        j++;
        j %= (out_key_len / CRYPTO_WORD_WIDTH);
    }

    ret = memcpy_s(out + cnt_count * out_key_len, out_len - cnt_count * out_key_len, readval, out_key_len);
    crypto_chk_return(ret != EOK, PBKDF2_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return TD_SUCCESS;
}

td_s32 drv_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len)
{
    td_s32 ret;
    td_u32 alg_cfg = 0;
    td_u32 block_size = 0;
    td_u32 out_key_len = 0;
    td_u32 cal_count = 0;
    td_u32 remain_num = 0;
    td_u32 i = 0;

    pbkdf2_null_ptr_chk(param);
    pbkdf2_null_ptr_chk(out);
    if (param->plen != 0) {
        pbkdf2_null_ptr_chk(param->password);
    }
    if (param->slen != 0) {
        pbkdf2_null_ptr_chk(param->salt);
    }

    if (param->hash_type == CRYPTO_HASH_TYPE_HMAC_SM3) {
        crypto_chk_return(crypto_sm_support(CRYPTO_SM_ALG_SM3) == TD_FALSE, PBKDF2_COMPAT_ERRNO(ERROR_UNSUPPORT),
            "SM3 alg is unsupport\n");
    }

    ret = priv_get_alg_cfg(param->hash_type, &alg_cfg, &block_size, &out_key_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_get_alg_cfg failed\n");
    crypto_chk_return(param->plen > block_size, PBKDF2_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "plen should not be Bigger Than Block Size!\n");
    crypto_chk_return(out_key_len == 0, PBKDF2_COMPAT_ERRNO(ERROR_INVALID_PARAM), "out_key_len is Invalid!\n");

    /* Formatting of the PBKDF2 salt Padding
     * The PBKDF2 salt Padding formatted as shown below:
     *                        big endian
     *         salt length || 32 bit(i) || 1 bit(1) || 2 word tail length
     *
     * sm3:
     * sha1:
     * sha256:   salt_len  ||   4 byte  ||   1 byte  ||  8 byte
     * so salt_len smaller block_size - (4 + 1 + 8 = 13)
     * sha384:
     * sha512:   salt_len  ||   4 byte  ||   1 byte  ||  16 byte
     * so salt_len smaller block_size - (4 + 1 + 16 = 21)
     */
    if (block_size == KDF_KEY_BLOCK_SIZE_512) {
        crypto_chk_return(param->slen > block_size - SALT_RESERVED_LEN_256, PBKDF2_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "Invalid Salt Len!\n");   /* 13: byte */
    } else {
        crypto_chk_return(param->slen > block_size - SALT_RESERVED_LEN_512, PBKDF2_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "Invalid Salt Len!\n");   /* 21: byte */
    }

    remain_num = out_len % out_key_len;      /* residual processing */
    if (remain_num != 0) {
        cal_count = (out_len / out_key_len) + 1;  /* if remain_num is not 0, need to calculate one more block. */
    } else {
        cal_count = out_len / out_key_len;
    }
    ret = priv_rkp_lock();
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_rkp_lock failed\n");

    for (i = 0; i < cal_count; i++) {
        ret = priv_kdf_calculation(param, alg_cfg, block_size, i + 1);
        crypto_chk_goto(ret != TD_SUCCESS, exit_rkp_unlock, "priv_kdf_calculation failed\n");
        if (remain_num != 0) {
            if (i == cal_count - 1) { /* calculation the last conut */
                ret = priv_kdf_output_key_remain(out, out_len, out_key_len, i, remain_num);
                crypto_chk_goto(ret != TD_SUCCESS, exit_rkp_unlock, "priv_kdf_output_key_remain failed\n");
            } else {
                ret = priv_kdf_output_key(out, out_len, out_key_len, i);
                crypto_chk_goto(ret != TD_SUCCESS, exit_rkp_unlock, "priv_kdf_output_key failed\n");
            }
        } else {
            ret = priv_kdf_output_key(out, out_len, out_key_len, i);
            crypto_chk_goto(ret != TD_SUCCESS, exit_rkp_unlock, "priv_kdf_output_key failed\n");
        }
    }

    /* clear key */
    priv_kdf_clear_reg_key();

exit_rkp_unlock:
    priv_rkp_unlock();     /* unlock rkp */
    return ret;
}