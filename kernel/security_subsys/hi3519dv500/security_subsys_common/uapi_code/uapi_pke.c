/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_pke.h"
#include <securec.h>

#include "uapi_common.h"
#include "crypto_ioctl_cmd.h"
#include "crypto_osal_user_lib.h"

#define PKE_COMPAT_ERRNO(err_code)      UAPI_COMPAT_ERRNO(ERROR_MODULE_PKE, err_code)

#define pke_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, PKE_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

#define pke_init_chk() do {                         \
    if (crypto_cipher_is_init() == 0) {                 \
        return PKE_COMPAT_ERRNO(ERROR_NOT_INIT);    \
    }                                               \
} while (0)

td_s32 unify_uapi_cipher_pke_init(td_void)
{
    td_s32 ret = CRYPTO_SUCCESS;

    crypto_uapi_func_enter();

    ret = crypto_cipher_open();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_pke_deinit(td_void)
{
    td_s32 ret = CRYPTO_SUCCESS;

    crypto_uapi_func_enter();

    if (crypto_cipher_is_init() == 0) {
        return CRYPTO_SUCCESS;
    }

    crypto_cipher_close();

    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_pke_ecc_gen_key(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecc_gen_key_ctl_t ecc_gen_key_ctl;

    crypto_uapi_func_enter();

    pke_init_chk();

    pke_null_ptr_chk(output_priv_key);
    pke_null_ptr_chk(output_pub_key);

    (td_void)memset_s(&ecc_gen_key_ctl, sizeof(ecc_gen_key_ctl), 0, sizeof(ecc_gen_key_ctl));
    ecc_gen_key_ctl.curve_type = curve_type;
    ecc_gen_key_ctl.output_priv_key.data.p = output_priv_key->data;
    ecc_gen_key_ctl.output_priv_key.length = output_priv_key->length;
    ecc_gen_key_ctl.output_pub_key.x.p = output_pub_key->x;
    ecc_gen_key_ctl.output_pub_key.y.p = output_pub_key->y;
    ecc_gen_key_ctl.output_pub_key.length = output_pub_key->length;

    if (input_priv_key != TD_NULL) {
        ecc_gen_key_ctl.input_priv_key.data.p = input_priv_key->data;
        ecc_gen_key_ctl.input_priv_key.length = input_priv_key->length;
    }
    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_ECC_GEN_KEY, &ecc_gen_key_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
exit:
    (td_void)memset_s(&ecc_gen_key_ctl, sizeof(ecc_gen_key_ctl), 0, sizeof(ecc_gen_key_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_ecc_gen_ecdh_key(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecc_gen_ecdh_key_ctl_t gen_ecdh_key_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(input_pub_key);
    pke_null_ptr_chk(input_priv_key);
    pke_null_ptr_chk(output_shared_key);

    (td_void)memset_s(&gen_ecdh_key_ctl, sizeof(gen_ecdh_key_ctl), 0, sizeof(gen_ecdh_key_ctl));
    gen_ecdh_key_ctl.curve_type = curve_type;
    gen_ecdh_key_ctl.input_pub_key.x.p = input_pub_key->x;
    gen_ecdh_key_ctl.input_pub_key.y.p = input_pub_key->y;
    gen_ecdh_key_ctl.input_pub_key.length = input_pub_key->length;
    gen_ecdh_key_ctl.input_priv_key.data.p = input_priv_key->data;
    gen_ecdh_key_ctl.input_priv_key.length = input_priv_key->length;
    gen_ecdh_key_ctl.output_shared_key.data.p = output_shared_key->data;
    gen_ecdh_key_ctl.output_shared_key.length = output_shared_key->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_ECC_GEN_ECDH_KEY, &gen_ecdh_key_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&gen_ecdh_key_ctl, sizeof(gen_ecdh_key_ctl), 0, sizeof(gen_ecdh_key_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_ecdsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecdsa_sign_ctl_t ecdsa_sign_ctl;

    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(hash);
    pke_null_ptr_chk(sig);

    (td_void)memset_s(&ecdsa_sign_ctl, sizeof(ecdsa_sign_ctl), 0, sizeof(ecdsa_sign_ctl));
    ecdsa_sign_ctl.curve_type = curve_type;
    ecdsa_sign_ctl.priv_key.data.p = priv_key->data;
    ecdsa_sign_ctl.priv_key.length = priv_key->length;
    ecdsa_sign_ctl.hash.data.p = hash->data;
    ecdsa_sign_ctl.hash.length = hash->length;
    ecdsa_sign_ctl.sig.r.p = sig->r;
    ecdsa_sign_ctl.sig.s.p = sig->s;
    ecdsa_sign_ctl.sig.length = sig->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_ECDSA_SIGN, &ecdsa_sign_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&ecdsa_sign_ctl, sizeof(ecdsa_sign_ctl), 0, sizeof(ecdsa_sign_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_ecdsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecdsa_verify_ctl_t ecdsa_verify_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(hash);
    pke_null_ptr_chk(sig);

    (td_void)memset_s(&ecdsa_verify_ctl, sizeof(ecdsa_verify_ctl), 0, sizeof(ecdsa_verify_ctl));
    ecdsa_verify_ctl.curve_type = curve_type;
    ecdsa_verify_ctl.pub_key.x.p = pub_key->x;
    ecdsa_verify_ctl.pub_key.y.p = pub_key->y;
    ecdsa_verify_ctl.pub_key.length = pub_key->length;
    ecdsa_verify_ctl.hash.data.p = hash->data;
    ecdsa_verify_ctl.hash.length = hash->length;
    ecdsa_verify_ctl.sig.r.p = sig->r;
    ecdsa_verify_ctl.sig.s.p = sig->s;
    ecdsa_verify_ctl.sig.length = sig->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_ECDSA_VERIFY, &ecdsa_verify_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&ecdsa_verify_ctl, sizeof(ecdsa_verify_ctl), 0, sizeof(ecdsa_verify_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_eddsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_SUCCESS;
    pke_eddsa_sign_ctl_t eddsa_sign_ctl = {0};

    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(msg);
    pke_null_ptr_chk(sig);

    eddsa_sign_ctl.curve_type = curve_type;
    eddsa_sign_ctl.priv_key.data.p = priv_key->data;
    eddsa_sign_ctl.priv_key.length = priv_key->length;
    eddsa_sign_ctl.msg.data.p = msg->data;
    eddsa_sign_ctl.msg.length = msg->length;
    eddsa_sign_ctl.msg.buf_sec = msg->buf_sec;
    eddsa_sign_ctl.sig.r.p = sig->r;
    eddsa_sign_ctl.sig.s.p = sig->s;
    eddsa_sign_ctl.sig.length = sig->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_EDDSA_SIGN, &eddsa_sign_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&eddsa_sign_ctl, sizeof(eddsa_sign_ctl), 0, sizeof(eddsa_sign_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_eddsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_SUCCESS;
    pke_eddsa_verify_ctl_t eddsa_verify_ctl = {0};
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(msg);
    pke_null_ptr_chk(sig);

    eddsa_verify_ctl.curve_type = curve_type;
    eddsa_verify_ctl.pub_key.y.p = pub_key->y;
    eddsa_verify_ctl.pub_key.length = pub_key->length;
    eddsa_verify_ctl.msg.data.p = msg->data;
    eddsa_verify_ctl.msg.length = msg->length;
    eddsa_verify_ctl.msg.buf_sec = msg->buf_sec;
    eddsa_verify_ctl.sig.r.p = sig->r;
    eddsa_verify_ctl.sig.s.p = sig->s;
    eddsa_verify_ctl.sig.length = sig->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_EDDSA_VERIFY, &eddsa_verify_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&eddsa_verify_ctl, sizeof(eddsa_verify_ctl), 0, sizeof(eddsa_verify_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve)
{
    td_s32 ret = TD_SUCCESS;
    pke_check_dot_on_curve_ctl_t dot_on_curve_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(is_on_curve);

    (td_void)memset_s(&dot_on_curve_ctl, sizeof(dot_on_curve_ctl), 0, sizeof(dot_on_curve_ctl));
    dot_on_curve_ctl.curve_type = curve_type;
    dot_on_curve_ctl.pub_key.x.p = pub_key->x;
    dot_on_curve_ctl.pub_key.y.p = pub_key->y;
    dot_on_curve_ctl.pub_key.length = pub_key->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_CHECK_DOT_ON_CURVE, &dot_on_curve_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);
    *is_on_curve = dot_on_curve_ctl.is_on_curve;

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&dot_on_curve_ctl, sizeof(dot_on_curve_ctl), 0, sizeof(dot_on_curve_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash)
{
    td_s32 ret = TD_SUCCESS;
    pke_sm2_dsa_hash_ctl_t sm2_dsa_hash_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(sm2_id);
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(msg);
    pke_null_ptr_chk(hash);

    (td_void)memset_s(&sm2_dsa_hash_ctl, sizeof(sm2_dsa_hash_ctl), 0, sizeof(sm2_dsa_hash_ctl));
    sm2_dsa_hash_ctl.sm2_id.data.p = sm2_id->data;
    sm2_dsa_hash_ctl.sm2_id.length = sm2_id->length;
    sm2_dsa_hash_ctl.pub_key.x.p = pub_key->x;
    sm2_dsa_hash_ctl.pub_key.y.p = pub_key->y;
    sm2_dsa_hash_ctl.pub_key.length = pub_key->length;
    sm2_dsa_hash_ctl.msg.buf_sec = msg->buf_sec;
    sm2_dsa_hash_ctl.msg.data.p = msg->data;
    sm2_dsa_hash_ctl.msg.length = msg->length;
    sm2_dsa_hash_ctl.hash.data.p = hash->data;
    sm2_dsa_hash_ctl.hash.length = hash->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_SM2_DSA_HASH, &sm2_dsa_hash_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&sm2_dsa_hash_ctl, sizeof(sm2_dsa_hash_ctl), 0, sizeof(sm2_dsa_hash_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text)
{
    td_s32 ret = TD_SUCCESS;
    pke_sm2_public_encrypt_ctl_t sm2_public_encrypt_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(plain_text);
    pke_null_ptr_chk(cipher_text);

    (td_void)memset_s(&sm2_public_encrypt_ctl, sizeof(sm2_public_encrypt_ctl), 0, sizeof(sm2_public_encrypt_ctl));
    sm2_public_encrypt_ctl.pub_key.x.p = pub_key->x;
    sm2_public_encrypt_ctl.pub_key.y.p = pub_key->y;
    sm2_public_encrypt_ctl.pub_key.length = pub_key->length;
    sm2_public_encrypt_ctl.plain_text.data.p = plain_text->data;
    sm2_public_encrypt_ctl.plain_text.length = plain_text->length;
    sm2_public_encrypt_ctl.cipher_text.data.p = cipher_text->data;
    sm2_public_encrypt_ctl.cipher_text.length = cipher_text->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_SM2_PUBLIC_ENCRYPT, &sm2_public_encrypt_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);
    cipher_text->length = sm2_public_encrypt_ctl.cipher_text.length;
    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&sm2_public_encrypt_ctl, sizeof(sm2_public_encrypt_ctl), 0, sizeof(sm2_public_encrypt_ctl));
    return ret;
}


td_s32 unify_uapi_cipher_pke_sm2_private_decrypt(const drv_pke_data *priv_key, const drv_pke_data *cipher_text,
    drv_pke_data *plain_text)
{
    td_s32 ret = TD_SUCCESS;
    pke_sm2_private_decrypt_ctl_t sm2_private_decrypt_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(cipher_text);
    pke_null_ptr_chk(plain_text);

    (td_void)memset_s(&sm2_private_decrypt_ctl, sizeof(sm2_private_decrypt_ctl), 0, sizeof(sm2_private_decrypt_ctl));
    sm2_private_decrypt_ctl.priv_key.data.p = priv_key->data;
    sm2_private_decrypt_ctl.priv_key.length = priv_key->length;
    sm2_private_decrypt_ctl.cipher_text.data.p = cipher_text->data;
    sm2_private_decrypt_ctl.cipher_text.length = cipher_text->length;
    sm2_private_decrypt_ctl.plain_text.data.p = plain_text->data;
    sm2_private_decrypt_ctl.plain_text.length = plain_text->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_SM2_PRIVATE_DECRYPT, &sm2_private_decrypt_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);
    plain_text->length = sm2_private_decrypt_ctl.plain_text.length;
    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&sm2_private_decrypt_ctl, sizeof(sm2_private_decrypt_ctl), 0, sizeof(sm2_private_decrypt_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_mod(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c)
{
    td_s32 ret = TD_SUCCESS;
    pke_mod_ctl_t mod_ctr_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(a);
    pke_null_ptr_chk(p);
    pke_null_ptr_chk(c);

    (td_void)memset_s(&mod_ctr_ctl, sizeof(mod_ctr_ctl), 0, sizeof(mod_ctr_ctl));
    mod_ctr_ctl.a.data.p = a->data;
    mod_ctr_ctl.a.length = a->length;
    mod_ctr_ctl.p.data.p = p->data;
    mod_ctr_ctl.p.length = p->length;
    mod_ctr_ctl.c.data.p = c->data;
    mod_ctr_ctl.c.length = c->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_MOD, &mod_ctr_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    return ret;
}

td_s32 unify_uapi_cipher_pke_exp_mod(const drv_pke_data *n, const drv_pke_data *k,
    const drv_pke_data *in, const drv_pke_data *out)
{
    td_s32 ret = TD_SUCCESS;
    pke_exp_mod_ctl_t exp_mod_ctr_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(n);
    pke_null_ptr_chk(k);
    pke_null_ptr_chk(in);
    pke_null_ptr_chk(out);

    (td_void)memset_s(&exp_mod_ctr_ctl, sizeof(exp_mod_ctr_ctl), 0, sizeof(exp_mod_ctr_ctl));
    exp_mod_ctr_ctl.n.data.p = n->data;
    exp_mod_ctr_ctl.n.length = n->length;
    exp_mod_ctr_ctl.k.data.p = k->data;
    exp_mod_ctr_ctl.k.length = k->length;
    exp_mod_ctr_ctl.in.data.p = in->data;
    exp_mod_ctr_ctl.in.length = in->length;
    exp_mod_ctr_ctl.out.data.p = out->data;
    exp_mod_ctr_ctl.out.length = out->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_EXP_MOD, &exp_mod_ctr_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    return ret;
}

td_s32 unify_uapi_cipher_pke_rsa_sign(const drv_pke_rsa_priv_key *priv_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign)
{
    td_s32 ret = TD_SUCCESS;
    pke_rsa_sign_ctl_t rsa_sign_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(input_hash);
    pke_null_ptr_chk(sign);

    (td_void)memset_s(&rsa_sign_ctl, sizeof(rsa_sign_ctl), 0, sizeof(rsa_sign_ctl));
    rsa_sign_ctl.scheme = scheme;
    rsa_sign_ctl.priv_key.n.p = priv_key->n;
    rsa_sign_ctl.priv_key.e.p = priv_key->e;
    rsa_sign_ctl.priv_key.d.p = priv_key->d;
    rsa_sign_ctl.priv_key.p.p = priv_key->p;
    rsa_sign_ctl.priv_key.q.p = priv_key->q;
    rsa_sign_ctl.priv_key.dp.p = priv_key->dp;
    rsa_sign_ctl.priv_key.dq.p = priv_key->dq;
    rsa_sign_ctl.priv_key.qp.p = priv_key->qp;
    rsa_sign_ctl.priv_key.n_len = priv_key->n_len;
    rsa_sign_ctl.priv_key.e_len = priv_key->e_len;
    rsa_sign_ctl.priv_key.d_len = priv_key->d_len;
    rsa_sign_ctl.priv_key.p_len = priv_key->p_len;
    rsa_sign_ctl.priv_key.q_len = priv_key->q_len;
    rsa_sign_ctl.priv_key.dp_len = priv_key->dp_len;
    rsa_sign_ctl.priv_key.dq_len = priv_key->dq_len;
    rsa_sign_ctl.priv_key.qp_len = priv_key->qp_len;
    rsa_sign_ctl.scheme = scheme;
    rsa_sign_ctl.hash_type = hash_type;
    rsa_sign_ctl.input_hash.data.p = input_hash->data;
    rsa_sign_ctl.input_hash.length = input_hash->length;
    rsa_sign_ctl.sig.data.p = sign->data;
    rsa_sign_ctl.sig.length = sign->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_RSA_SIGN, &rsa_sign_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);
    sign->length = rsa_sign_ctl.sig.length;
    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&rsa_sign_ctl, sizeof(rsa_sign_ctl), 0, sizeof(rsa_sign_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_rsa_verify(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, drv_pke_data *input_hash, const drv_pke_data *sig)
{
    td_s32 ret = TD_SUCCESS;
    pke_rsa_verify_ctl_t rsa_verify_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(input_hash);
    pke_null_ptr_chk(sig);

    (td_void)memset_s(&rsa_verify_ctl, sizeof(rsa_verify_ctl), 0, sizeof(rsa_verify_ctl));
    rsa_verify_ctl.scheme = scheme;
    rsa_verify_ctl.hash_type = hash_type;
    rsa_verify_ctl.pub_key.e.p = pub_key->e;
    rsa_verify_ctl.pub_key.n.p = pub_key->n;
    rsa_verify_ctl.pub_key.len = pub_key->len;
    rsa_verify_ctl.input_hash.data.p = input_hash->data;
    rsa_verify_ctl.input_hash.length = input_hash->length;
    rsa_verify_ctl.sig.data.p = sig->data;
    rsa_verify_ctl.sig.length = sig->length;

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_RSA_VERIFY, &rsa_verify_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&rsa_verify_ctl, sizeof(rsa_verify_ctl), 0, sizeof(rsa_verify_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_rsa_public_encrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output)
{
    td_s32 ret = TD_SUCCESS;
    pke_rsa_pub_crypto_ctl_t rsa_pub_crypto_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(input);
    pke_null_ptr_chk(output);

    (td_void)memset_s(&rsa_pub_crypto_ctl, sizeof(rsa_pub_crypto_ctl), 0, sizeof(rsa_pub_crypto_ctl));
    rsa_pub_crypto_ctl.scheme = scheme;
    rsa_pub_crypto_ctl.hash_type = hash_type;
    rsa_pub_crypto_ctl.pub_key.e.p = pub_key->e;
    rsa_pub_crypto_ctl.pub_key.n.p = pub_key->n;
    rsa_pub_crypto_ctl.pub_key.len = pub_key->len;
    rsa_pub_crypto_ctl.input.data.p = input->data;
    rsa_pub_crypto_ctl.input.length = input->length;
    rsa_pub_crypto_ctl.output.data.p = output->data;
    rsa_pub_crypto_ctl.output.length = output->length;
    if (label != TD_NULL) {
        rsa_pub_crypto_ctl.label.data.p = label->data;
        rsa_pub_crypto_ctl.label.length = label->length;
    } else {
        rsa_pub_crypto_ctl.label.data.p = TD_NULL;
    }

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_RSA_PUBLIC_ENCRYPT, &rsa_pub_crypto_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);
    output->length = rsa_pub_crypto_ctl.output.length;
    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&rsa_pub_crypto_ctl, sizeof(rsa_pub_crypto_ctl), 0, sizeof(rsa_pub_crypto_ctl));
    return ret;
}

td_s32 unify_uapi_cipher_pke_rsa_private_decrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output)
{
    td_s32 ret = TD_SUCCESS;
    pke_rsa_priv_crypto_ctl_t rsa_priv_crypto_ctl;
    crypto_uapi_func_enter();
    pke_init_chk();

    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(input);
    pke_null_ptr_chk(output);

    (td_void)memset_s(&rsa_priv_crypto_ctl, sizeof(rsa_priv_crypto_ctl), 0, sizeof(rsa_priv_crypto_ctl));
    rsa_priv_crypto_ctl.scheme = scheme;
    rsa_priv_crypto_ctl.hash_type = hash_type;
    rsa_priv_crypto_ctl.priv_key.n.p = priv_key->n;
    rsa_priv_crypto_ctl.priv_key.e.p = priv_key->e;
    rsa_priv_crypto_ctl.priv_key.d.p = priv_key->d;
    rsa_priv_crypto_ctl.priv_key.p.p = priv_key->p;
    rsa_priv_crypto_ctl.priv_key.q.p = priv_key->q;
    rsa_priv_crypto_ctl.priv_key.dp.p = priv_key->dp;
    rsa_priv_crypto_ctl.priv_key.dq.p = priv_key->dq;
    rsa_priv_crypto_ctl.priv_key.qp.p = priv_key->qp;
    rsa_priv_crypto_ctl.priv_key.n_len = priv_key->n_len;
    rsa_priv_crypto_ctl.priv_key.e_len = priv_key->e_len;
    rsa_priv_crypto_ctl.priv_key.d_len = priv_key->d_len;
    rsa_priv_crypto_ctl.priv_key.p_len = priv_key->p_len;
    rsa_priv_crypto_ctl.priv_key.q_len = priv_key->q_len;
    rsa_priv_crypto_ctl.priv_key.dp_len = priv_key->dp_len;
    rsa_priv_crypto_ctl.priv_key.dq_len = priv_key->dq_len;
    rsa_priv_crypto_ctl.priv_key.qp_len = priv_key->qp_len;
    rsa_priv_crypto_ctl.input.data.p = input->data;
    rsa_priv_crypto_ctl.input.length = input->length;
    rsa_priv_crypto_ctl.output.data.p = output->data;
    rsa_priv_crypto_ctl.output.length = output->length;
    if (label != TD_NULL) {
        rsa_priv_crypto_ctl.label.data.p = label->data;
        rsa_priv_crypto_ctl.label.length = label->length;
    } else {
        rsa_priv_crypto_ctl.label.data.p = TD_NULL;
    }

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PKE_RSA_PRIVATE_DECRYPT, &rsa_priv_crypto_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_ioctl failed, ret is 0x%x\n", ret);
    output->length = rsa_priv_crypto_ctl.output.length;
    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&rsa_priv_crypto_ctl, sizeof(rsa_priv_crypto_ctl), 0, sizeof(rsa_priv_crypto_ctl));
    return ret;
}