/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_pke.h"
#include "drv_pke.h"
#include "crypto_hash_struct.h"
#include "crypto_drv_common.h"
#include "securec.h"
#include "drv_hash.h"
#include "drv_trng.h"
#include "drv_inner.h"
#define PKE_COMPAT_ERRNO(err_code)      DRV_COMPAT_ERRNO(ERROR_MODULE_PKE, err_code)

static td_u32 g_drv_pke_initialize = TD_FALSE;

static td_u8 g_ed_pd[] = {
    0x59, 0x04, 0x56, 0xb4, 0xe5, 0x3f, 0x8a, 0x4d, 0xcb, 0x27, 0x24, 0x0f, 0x78, 0x31, 0x0d, 0x20,
    0x21, 0x43, 0x0e, 0xef, 0x5f, 0x8c, 0x52, 0xe7, 0x01, 0xdb, 0x17, 0xfd, 0xbe, 0x8f, 0xd3, 0xf4,
};

td_s32 drv_cipher_pke_init(void)
{
    g_drv_pke_initialize = TD_TRUE;
    return hal_pke_init();
}

td_s32 drv_cipher_pke_deinit(void)
{
    g_drv_pke_initialize = TD_FALSE;
    return hal_pke_deinit();
}

td_s32 drv_cipher_pke_lock_secure(void)
{
    td_s32 ret = TD_FAILURE;
    crypto_chk_return(g_drv_pke_initialize != TD_TRUE, PKE_COMPAT_ERRNO(ERROR_NOT_INIT), "pke not init!\n");
    ret = hal_pke_lock();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_lock failed, ret is 0x%x\n", ret);
    hal_pke_secure(TD_TRUE);
    return TD_SUCCESS;
}

td_s32 drv_cipher_pke_unlock_secure(void)
{
    td_s32 ret = TD_FAILURE;
    ret = hal_pke_unlock();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_unlock failed, ret is 0x%x\n", ret);
    hal_pke_secure(TD_FALSE);
    return TD_SUCCESS;
}

td_s32 drv_cipher_pke_clean_ram(void)
{
    td_s32 ret = TD_FAILURE;

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_CLR_RAM, DRV_PKE_LEN_4096);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    // Redundant cleaning
    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_CLR_RAM, DRV_PKE_LEN_4096);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_exp_mod_param_chk(const drv_pke_data *n, const drv_pke_data *k, const drv_pke_data *in,
    const drv_pke_data *out)
{
    crypto_param_check(n == TD_NULL);
    crypto_param_check(n->data == TD_NULL);
    crypto_param_check(k == TD_NULL);
    crypto_param_check(k->data == TD_NULL);
    crypto_param_check(in == TD_NULL);
    crypto_param_check(in->data == TD_NULL);
    crypto_param_check(out == TD_NULL);
    crypto_param_check(out->data == TD_NULL);
    crypto_param_check(n->length != k->length);
    crypto_param_check(n->length != in->length);
    crypto_param_check(n->length > out->length);
    crypto_param_check(n->length != DRV_PKE_LEN_2048 && n->length != DRV_PKE_LEN_3072 &&
        n->length != DRV_PKE_LEN_4096);
    return TD_SUCCESS;
}

/* out = in ^ k mod n */
td_s32 drv_cipher_pke_exp_mod(const drv_pke_data *n, const drv_pke_data *k, const drv_pke_data *in,
    const drv_pke_data *out)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    pke_block random;
    td_u8 *key = TD_NULL;
    td_u32 key_buf_len = 0;

    ret = drv_cipher_pke_exp_mod_param_chk(n, k, in, out);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_exp_mod_param_chk failed, ret is 0x%x\n", ret);

    klen = n->length;

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    key_buf_len = klen * 2; // 2: for n & k
    key = crypto_malloc(key_buf_len);
    crypto_chk_return(key == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    ret = drv_cipher_trng_get_random(&random.word[0]);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);

    ret = drv_cipher_trng_get_random(&random.word[1]);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_EXP_MOD, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    /* key xor random */
    if (memcpy_s(key, klen, n->data, klen) != EOK || memcpy_s(key + klen, klen, k->data, klen) != EOK) {
        crypto_log_err("memcpy_s(key, klen, k, klen) != EOK\n");
        ret = PKE_COMPAT_ERRNO(ERROR_MEMCPY_S);
        goto exit_free;
    }

#if defined(CONFIG_CRYPTO_CHIP_SHAOLINSWORD) || defined(CONFIG_CRYPTO_CHIP_HI3751V811)
    hal_pke_set_key(key, key, klen + klen, &random, klen + klen);
#endif

    hal_pke_set_ram(PKE_NRAM, key, klen, klen);
    hal_pke_set_ram(PKE_KRAM, key + klen, klen, klen);
    hal_pke_set_ram(PKE_MRAM, in->data, klen, klen);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_RRAM, out->data, klen);
exit_free:
    if (key != TD_NULL) {
        (void)memset_s(key, key_buf_len, 0, key_buf_len);
        crypto_free(key);
    }
    return ret;
}

static td_s32 drv_cipher_pke_add_mod_param_chk(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    crypto_param_check(a == TD_NULL);
    crypto_param_check(a->data == TD_NULL);
    crypto_param_check(b == TD_NULL);
    crypto_param_check(b->data == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(c->data == TD_NULL);
    crypto_param_check(p->length == 0);
    crypto_param_check(p->length != b->length);
    crypto_param_check(p->length != c->length);
    return TD_SUCCESS;
}

/* c = a + b mod p */
td_s32 drv_cipher_pke_add_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 max_len = 0;
    ret = drv_cipher_pke_add_mod_param_chk(a, b, p, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_add_mod_param_chk failed, ret is 0x%x\n", ret);

    ret = hal_pke_align_len(crypto_max(p->length, a->length), &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_ADD_MOD, max_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_NRAM, a->data, a->length, max_len);
    hal_pke_set_ram(PKE_KRAM, b->data, b->length, max_len);
    hal_pke_set_ram(PKE_RRAM, p->data, p->length, max_len);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_MRAM, c->data, c->length);

    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_sub_mod_param_chk(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    crypto_param_check(a == TD_NULL);
    crypto_param_check(b == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(a->data == TD_NULL);
    crypto_param_check(b->data == TD_NULL);
    crypto_param_check(c->data == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(a->length == 0);
    crypto_param_check(a->length != b->length);
    crypto_param_check(a->length != p->length);
    crypto_param_check(a->length != c->length);
    return TD_SUCCESS;
}

/* c = (a - b) mod p */
td_s32 drv_cipher_pke_sub_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 max_len = 0;
    ret = drv_cipher_pke_sub_mod_param_chk(a, b, p, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_sub_mod_param_chk failed, ret is 0x%x\n", ret);

    klen = a->length;

    ret = hal_pke_align_len(klen, &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_SUB_MOD, max_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_NRAM, a->data, klen, max_len);
    hal_pke_set_ram(PKE_KRAM, b->data, klen, max_len);
    hal_pke_set_ram(PKE_RRAM, p->data, klen, max_len);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_MRAM, c->data, klen);
    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_mod_param_chk(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c)
{
    crypto_param_check(a == TD_NULL);
    crypto_param_check(a->data == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(c->data == TD_NULL);
    crypto_param_check(p->length == 0);
    crypto_param_check(p->length != c->length);
    return TD_SUCCESS;
}

/* c = a mod p */
td_s32 drv_cipher_pke_mod(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 max_len = 0;
    ret = drv_cipher_pke_mod_param_chk(a, p, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_mod_param_chk failed, ret is 0x%x\n", ret);

    ret = hal_pke_align_len(crypto_max(a->length, p->length), &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_MOD, max_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_MRAM, a->data, a->length, max_len);
    hal_pke_set_ram(PKE_KRAM, p->data, p->length, max_len);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_RRAM, c->data, c->length);

    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_mul_dot_param_chk(const drv_pke_ecc_curve *ecc, const drv_pke_data *k,
    const drv_pke_ecc_point *p, const drv_pke_ecc_point *r)
{
    crypto_param_check(ecc == TD_NULL);
    crypto_param_check(ecc->gx == TD_NULL);
    crypto_param_check(ecc->gy == TD_NULL);
    crypto_param_check(ecc->n == TD_NULL);
    crypto_param_check(ecc->b == TD_NULL);
    crypto_param_check(ecc->a == TD_NULL);
    crypto_param_check(ecc->p == TD_NULL);
    crypto_param_check(k == TD_NULL);
    crypto_param_check(k->data == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->x == TD_NULL);
    crypto_param_check(p->y == TD_NULL);
    crypto_param_check(r == TD_NULL);
    crypto_param_check(r->x == TD_NULL);
    crypto_param_check(ecc->ksize != k->length);
    crypto_param_check(ecc->ksize != p->length);
    crypto_param_check(ecc->ksize != r->length);
    crypto_param_check(ecc->ksize != DRV_PKE_LEN_256 && ecc->ksize != DRV_PKE_LEN_384 &&
        ecc->ksize != DRV_PKE_LEN_512 && ecc->ksize != DRV_PKE_LEN_521);
    return TD_SUCCESS;
}

/* *** R = k . P(x,y) *** */
td_s32 drv_cipher_pke_mul_dot(const drv_pke_ecc_curve *ecc, const drv_pke_data *k, const drv_pke_ecc_point *p,
    const drv_pke_ecc_point *r)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen;
    td_u32 max_len = 0;
    td_u8 *key = TD_NULL;
    td_u32 id;
    pke_block random;
    td_u32 ram_section_len = 0;
    td_u32 set_key_len = 0;
    ret = drv_cipher_pke_mul_dot_param_chk(ecc, k, p, r);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_mul_dot_param_chk failed, ret is 0x%x\n", ret);

    klen = ecc->ksize;

    ret = hal_pke_align_len(0, &ram_section_len, HAL_RAM_SECTION_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);
    ret = hal_pke_align_len(0, &set_key_len, HAL_SET_KEY_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);
    ret = hal_pke_align_len(klen, &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    key = crypto_malloc(set_key_len);
    if (key == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    ret = hal_pke_wait_free();
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_MUL_DOT, max_len);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    ret = drv_cipher_trng_get_random(&random.word[0]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);
    ret = drv_cipher_trng_get_random(&random.word[1]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);

    hal_pke_set_key(k->data, key, klen, &random, set_key_len);

    id = 0;
    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, p->x, klen, ram_section_len);
    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, p->y, klen, ram_section_len);
    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, ecc->gx, klen, ram_section_len);
    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, ecc->gy, klen, ram_section_len);

    id = 0;
    hal_pke_set_ram(PKE_KRAM, key, set_key_len, set_key_len);
    hal_pke_set_ram(PKE_KRAM + set_key_len + (id++) * ram_section_len, ecc->n, klen, ram_section_len);
    hal_pke_set_ram(PKE_KRAM + set_key_len + (id++) * ram_section_len, ecc->b, klen, ram_section_len);
    hal_pke_set_ram(PKE_KRAM + set_key_len + (id++) * ram_section_len, ecc->a, klen, ram_section_len);
    hal_pke_set_ram(PKE_KRAM + set_key_len + (id++) * ram_section_len, ecc->p, klen, ram_section_len);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_RRAM, r->x, klen);
    // r->y does not need to be provided in the ECDH
    if (r->y != TD_NULL) {
        hal_pke_get_ram(PKE_RRAM + ram_section_len, r->y, klen);
    }
exit__:
    if (key != TD_NULL) {
        (void)memset_s(key, set_key_len, 0, set_key_len);
        crypto_free(key);
    }
    return ret;
}

static td_s32 drv_cipher_pke_mul_mod_param_chk(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data  *p,
    const drv_pke_data  *c)
{
    crypto_param_check(a == TD_NULL);
    crypto_param_check(a->data == TD_NULL);
    crypto_param_check(b == TD_NULL);
    crypto_param_check(b->data == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(c->data == TD_NULL);
    crypto_param_check(a->length == 0);
    crypto_param_check(a->length != b->length);
    crypto_param_check(a->length != p->length);
    crypto_param_check(a->length != c->length);
    return TD_SUCCESS;
}

/* c = a * b mod p */
td_s32 drv_cipher_pke_mul_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data  *p,
    const drv_pke_data  *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 max_len = 0;

    ret = drv_cipher_pke_mul_mod_param_chk(a, b, p, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_mul_mod_param_chk failed, ret is 0x%x\n", ret);

    klen = a->length;

    ret = hal_pke_align_len(klen, &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);
    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);
    ret = hal_pke_set_mode(HAL_PKE_MODE_MUL_MOD, max_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_MRAM, a->data, klen, max_len);
    hal_pke_set_ram(PKE_KRAM, b->data, klen, max_len);
    hal_pke_set_ram(PKE_NRAM, p->data, klen, max_len);

    hal_pke_start();
    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);
    hal_pke_get_ram(PKE_RRAM, c->data, klen);
    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_inv_mod_param_chk(const drv_pke_data *a, const drv_pke_data *p, drv_pke_data *c)
{
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(a == TD_NULL);
    crypto_param_check(a->data == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(c->data == TD_NULL);
    crypto_param_check(a->length != p->length);
    crypto_param_check(a->length != c->length);
    crypto_param_check(a->length != DRV_PKE_LEN_256 && a->length != DRV_PKE_LEN_384 &&
        a->length != DRV_PKE_LEN_512 && a->length != DRV_PKE_LEN_521);
    return TD_SUCCESS;
}

/* c = a ^ -1 mod p */
td_s32 drv_cipher_pke_inv_mod(const drv_pke_data *a, const drv_pke_data *p, drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 max_len = 0;
    ret = drv_cipher_pke_inv_mod_param_chk(a, p, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_inv_mod_param_chk failed, ret is 0x%x\n", ret);

    klen = a->length;

    ret = hal_pke_align_len(klen, &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_MINV_MOD, max_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_MRAM, a->data, klen, max_len);
    hal_pke_set_ram(PKE_KRAM, p->data, klen, max_len);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_RRAM, c->data, klen);

    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_add_dot_param_chk(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *s,
    const drv_pke_ecc_point *r, const drv_pke_ecc_point *c)
{
    crypto_param_check(ecc->a == TD_NULL);
    crypto_param_check(ecc->p == TD_NULL);
    crypto_param_check(s == TD_NULL);
    crypto_param_check(s->x == TD_NULL);
    crypto_param_check(s->y == TD_NULL);
    crypto_param_check(r == TD_NULL);
    crypto_param_check(r->x == TD_NULL);
    crypto_param_check(r->y == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(c->x == TD_NULL);
    crypto_param_check(c->y == TD_NULL);
    crypto_param_check(ecc->ksize != c->length);
    crypto_param_check(ecc->ksize != s->length);
    crypto_param_check(ecc->ksize != r->length);
    crypto_param_check(ecc->ksize != DRV_PKE_LEN_256 && ecc->ksize != DRV_PKE_LEN_384
        && ecc->ksize != DRV_PKE_LEN_512 && ecc->ksize != DRV_PKE_LEN_521);
    return TD_SUCCESS;
}

/* C(x,y) = S(x,y) + R(x,y) */
td_s32 drv_cipher_pke_add_dot(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *s, const drv_pke_ecc_point *r,
    const drv_pke_ecc_point *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 max_len = 0;
    td_u32 id = 0;
    td_u32 ram_section_len = 0;
    td_u32 set_key_len = 0;
    ret = drv_cipher_pke_add_dot_param_chk(ecc, s, r, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_add_dot_param_chk failed, ret is 0x%x\n", ret);

    klen = ecc->ksize;

    ret = hal_pke_align_len(0, &ram_section_len, HAL_RAM_SECTION_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);
    ret = hal_pke_align_len(0, &set_key_len, HAL_SET_KEY_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);
    ret = hal_pke_align_len(klen, &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_ADD_DOT, max_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, s->x, klen, ram_section_len);
    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, s->y, klen, ram_section_len);
    hal_pke_set_ram(PKE_MRAM + (id++) * ram_section_len, r->x, klen, ram_section_len);
    hal_pke_set_ram(PKE_MRAM + id * ram_section_len, r->y, klen, ram_section_len);
    id = 2; // KRAM start form 2 ram_section
    hal_pke_set_ram(PKE_KRAM + set_key_len + (id++) * ram_section_len, ecc->a, klen, ram_section_len);
    hal_pke_set_ram(PKE_KRAM + set_key_len + (id++) * ram_section_len, ecc->p, klen, ram_section_len);
    hal_pke_start();
    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_RRAM, c->x, klen);
    hal_pke_get_ram(PKE_RRAM + ram_section_len, c->y, klen);

    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_mg_mul_dot_param_chk(const drv_pke_data *k, const drv_pke_data *U, const drv_pke_data *p,
    const drv_pke_data *a24, const drv_pke_data *rx)
{
    crypto_param_check(k == TD_NULL);
    crypto_param_check(k->data == TD_NULL);
    crypto_param_check(U == TD_NULL);
    crypto_param_check(U->data == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(a24 == TD_NULL);
    crypto_param_check(a24->data == TD_NULL);
    crypto_param_check(rx == TD_NULL);
    crypto_param_check(rx->data == TD_NULL);
    crypto_param_check(k->length != U->length);
    crypto_param_check(k->length != p->length);
    crypto_param_check(k->length != a24->length);
    crypto_param_check(k->length != rx->length);
    crypto_param_check(k->length != DRV_PKE_LEN_256);
    return TD_SUCCESS;
}

/* R = k * U(x) */
td_s32 drv_cipher_pke_mg_mul_dot(const drv_pke_data *k, const drv_pke_data *U, const drv_pke_data *p,
    const drv_pke_data *a24, const drv_pke_data *rx)
{
    td_s32 ret = TD_FAILURE;
    pke_block random;
    td_u32 klen = DRV_PKE_LEN_256;
    td_u8 key[DRV_PKE_LEN_256] = {0};

    ret = drv_cipher_pke_mg_mul_dot_param_chk(k, U, p, a24, rx);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_mg_mul_dot_param_chk failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_MG_MUL_DOT, klen);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    ret = drv_cipher_trng_get_random(&random.word[0]);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);

    hal_pke_set_key(k->data, key, klen, &random, klen);
    hal_pke_set_ram(PKE_MRAM, U->data, klen, klen);
    hal_pke_set_ram(PKE_MG_A24, a24->data, klen, klen);
    hal_pke_set_ram(PKE_KRAM, p->data, klen, klen);
    hal_pke_set_ram(PKE_MG_K, key, klen, klen);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);
    hal_pke_get_ram(PKE_RRAM, rx->data, klen);
    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_ed_mul_dot_param_chk(const drv_pke_data *k, const drv_pke_ecc_point *U,
    const drv_pke_data *p, const drv_pke_ecc_point *r)
{
    crypto_param_check(k == TD_NULL);
    crypto_param_check(k->data == TD_NULL);
    crypto_param_check(U == TD_NULL);
    crypto_param_check(U->x == TD_NULL);
    crypto_param_check(U->y == TD_NULL);
    crypto_param_check(p == TD_NULL);
    crypto_param_check(p->data == TD_NULL);
    crypto_param_check(r == TD_NULL);
    crypto_param_check(r->x == TD_NULL);
    crypto_param_check(r->y == TD_NULL);
    crypto_param_check(k->length != DRV_PKE_LEN_256);
    crypto_param_check(U->length != DRV_PKE_LEN_256);
    crypto_param_check(p->length != DRV_PKE_LEN_256);
    crypto_param_check(r->length != DRV_PKE_LEN_256);
    return TD_SUCCESS;
}

/* R = k * U(x,y) */
td_s32 drv_cipher_pke_ed_mul_dot(const drv_pke_data *k, const drv_pke_ecc_point *U, const drv_pke_data *p,
    const drv_pke_ecc_point *r)
{
    td_s32 ret = TD_FAILURE;
    pke_block random;
    td_u32 klen = DRV_PKE_LEN_256;
    td_u8 key[DRV_PKE_LEN_256] = {0};

    ret = drv_cipher_pke_ed_mul_dot_param_chk(k, U, p, r);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_ed_mul_dot_param_chk failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_ED_MUL_DOT, klen);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    ret = drv_cipher_trng_get_random(&random.word[0]);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);
    ret = drv_cipher_trng_get_random(&random.word[1]);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);

    hal_pke_set_key(k->data, key, klen, &random, klen);

    hal_pke_set_ram(PKE_ED_MRAM(0), U->x, klen, klen);
    hal_pke_set_ram(PKE_ED_MRAM(1), U->y, klen, klen);
    hal_pke_set_ram(PKE_ED_PD, g_ed_pd, klen, klen);
    hal_pke_set_ram(PKE_KRAM, p->data, klen, klen);
    hal_pke_set_ram(PKE_ED_K, key, klen, klen);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_ED_RRAM(0), r->x, klen);
    hal_pke_get_ram(PKE_ED_RRAM(1), r->y, klen);
    return TD_SUCCESS;
}

static td_s32 drv_cipher_pke_ed_add_dot_param_chk(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *s,
    const drv_pke_ecc_point *r, const drv_pke_ecc_point *c)
{
    crypto_param_check(ecc == TD_NULL);
    crypto_param_check(ecc->a == TD_NULL);
    crypto_param_check(ecc->p == TD_NULL);
    crypto_param_check(s == TD_NULL);
    crypto_param_check(s->x == TD_NULL);
    crypto_param_check(s->y == TD_NULL);
    crypto_param_check(r == TD_NULL);
    crypto_param_check(r->x == TD_NULL);
    crypto_param_check(r->y == TD_NULL);
    crypto_param_check(c == TD_NULL);
    crypto_param_check(c->x == TD_NULL);
    crypto_param_check(c->y == TD_NULL);
    crypto_param_check(ecc->ksize != DRV_PKE_LEN_256);
    return TD_SUCCESS;
}

/* brief: C = S(x,y) + R(x,y) for Ed25519 curve */
td_s32 drv_cipher_pke_ed_add_dot(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *s,
    const drv_pke_ecc_point *r, const drv_pke_ecc_point *c)
{
    td_s32 ret = TD_FAILURE;
    td_u32 ram_section_len = 0;
    td_u32 klen = DRV_PKE_LEN_256;
    ret = drv_cipher_pke_ed_add_dot_param_chk(ecc, s, r, c);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_ed_add_dot_param_chk failed, ret is 0x%x\n", ret);
    
    ret = hal_pke_align_len(0, &ram_section_len, HAL_RAM_SECTION_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);

    ret = hal_pke_wait_free();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_free failed, ret is 0x%x\n", ret);

    ret = hal_pke_set_mode(HAL_PKE_MODE_ED_ADD_DOT, klen);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_set_mode failed, ret is 0x%x\n", ret);

    hal_pke_set_ram(PKE_ED_MRAM(0), s->x, klen, ram_section_len); // 0: slot0
    hal_pke_set_ram(PKE_ED_MRAM(1), s->y, klen, ram_section_len); // 1: slot1
    hal_pke_set_ram(PKE_ED_NRAM(0), r->x, klen, ram_section_len); // 0: slot0
    hal_pke_set_ram(PKE_ED_NRAM(1), r->y, klen, ram_section_len); // 1: slot1
    hal_pke_set_ram(PKE_ED_NRAM(2), g_ed_pd, klen, ram_section_len); // 2: slot2
    hal_pke_set_ram(PKE_KRAM, ecc->p, klen, ram_section_len);

    hal_pke_start();

    ret = hal_pke_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_wait_done failed, ret is 0x%x\n", ret);

    hal_pke_get_ram(PKE_ED_RRAM(0), c->x, DRV_PKE_LEN_256); // 0: slot0
    hal_pke_get_ram(PKE_ED_RRAM(1), c->y, DRV_PKE_LEN_256); // 1: slot1

    return TD_SUCCESS;
}

static crypto_hash_type drv_pke_get_hash_type(drv_pke_hash_type hash_type)
{
    switch (hash_type) {
        case DRV_PKE_HASH_TYPE_SHA224:
            return CRYPTO_HASH_TYPE_SHA224;
        case DRV_PKE_HASH_TYPE_SHA256:
            return CRYPTO_HASH_TYPE_SHA256;
        case DRV_PKE_HASH_TYPE_SHA384:
            return CRYPTO_HASH_TYPE_SHA384;
        case DRV_PKE_HASH_TYPE_SHA512:
            return CRYPTO_HASH_TYPE_SHA512;
        case DRV_PKE_HASH_TYPE_SM3:
            return CRYPTO_HASH_TYPE_SM3;
        default:
            return CRYPTO_HASH_TYPE_INVALID;
    }
}

td_s32 drv_cipher_pke_calc_hash(const drv_pke_data* arr, td_u32 arr_len,
    const drv_pke_hash_type hash_type, drv_pke_data *hash)
{
    td_s32 ret = TD_FAILURE;
    td_handle h_handle = 0;
    crypto_buf_attr src_buf = {0};
    td_u32 i;
    crypto_hash_type hash_alg;
    crypto_hash_attr hash_attr;
    crypto_param_check(arr == TD_NULL);
    crypto_param_check(hash == TD_NULL);
    crypto_param_check(hash->data == TD_NULL);

    hash_alg = drv_pke_get_hash_type(hash_type);
    if (hash_alg == CRYPTO_HASH_TYPE_INVALID) {
        return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    hash_attr = (crypto_hash_attr){.hash_type = hash_alg};

    ret = drv_cipher_hash_start(&h_handle, &hash_attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_hash_start failed, ret is 0x%x\n", ret);

    for (i = 0;i < arr_len; i++) {
        src_buf.virt_addr = arr[i].data;
        ret = drv_cipher_hash_update(h_handle, &src_buf, arr[i].length);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_hash_update failed, ret is 0x%x\n", ret);
    }
    ret = drv_cipher_hash_finish(h_handle, hash->data, &(hash->length));
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_hash_finish failed, ret is 0x%x\n", ret);
    return TD_SUCCESS;
exit__:
    (void)drv_cipher_hash_destroy(h_handle);
    return ret;
}