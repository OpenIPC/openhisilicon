/*
 * The hash and RSA halves of the Cipher driver, absent.
 *
 * The hi3516cv500 counterpart of the same file in the hi3516ev200 tree, and
 * it is a separate file rather than a shared one because these two trees are
 * different SDK vintages: cv500's kapi_rsa_* take a cryp_rsa_crypt_data, its
 * kapi_hash_finish takes the output buffer's length, and its log macros are
 * lower case. Read kernel/cipher/Kbuild for why the trim exists at all -- the
 * short of it is that a kernel module gets no dead-code elimination, so the
 * vendor SDK's whole crypto stack shipped whether or not anything reached it,
 * and RSA plus hash were about half of open_cipher.ko.
 *
 * Nothing calls them. majestic is the only consumer of /dev/cipher on these
 * boards -- it goes straight to the ioctls, see its
 * include/majestic/hisi/cipher_abi.h -- and it uses the symmetric channel, the
 * key ladder and the TRNG. Not the hash, whose handle closes on finish so the
 * key is reprogrammed per packet, measured five times slower than mbedTLS. Not
 * RSA, which the recorder needs once per clip and userspace mbedTLS already
 * does. `nm` over this tree's own objects agrees with the ev200 one: the two
 * subsystems are reached through exactly the twelve symbols below, all of them
 * from kapi_dispatch.c, and nothing in symc, klad, the TRNG or the OTP path
 * touches them.
 *
 * init/deinit/release succeed, because crypto_entry() aborts the module load
 * on anything else and the symmetric path has no business failing over an
 * absent hash. Every real operation returns HI_ERR_CIPHER_UNSUPPORTED.
 *
 * THE DISPATCH TABLE IS UNTOUCHED, which is the point of stubbing here rather
 * than editing kapi_dispatch.c: crypto_ioctl() indexes g_dispatch_func[] by
 * the command's nr and then checks the row's cmd matches, and majestic derives
 * its command numbers from the same struct sizes the driver does, so a removed
 * row would land every later command on the wrong handler.
 *
 * The vendor sources stay in the tree untouched. Restoring either subsystem is
 * putting its objects back on hi3516cv500.kbuild's list and dropping this file
 * from it -- not a merge.
 */

#include "drv_osal_lib.h"
#include "cryp_rsa.h"

/* Spelled out rather than borrowed: this tree defines no CRYPTO_UNUSED. */
#define UNUSED(x) ((void)(x))

/* ---- hash: SHA-1/224/256/384/512, HMAC, SM3 ---- */

hi_s32 kapi_hash_init(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 kapi_hash_deinit(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 kapi_hash_release(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 kapi_hash_start(hi_u32 *id, hi_cipher_hash_type type,
                       hi_u8 *key, hi_u32 keylen)
{
    UNUSED(id);
    UNUSED(type);
    UNUSED(key);
    UNUSED(keylen);

    hi_log_error("error, hash is not built into this module\n");
    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_hash_update(hi_u32 id, hi_u8 *input, hi_u32 length,
                        hash_chunk_src src)
{
    UNUSED(id);
    UNUSED(input);
    UNUSED(length);
    UNUSED(src);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 hash_buf_len,
                        hi_u32 *hashlen)
{
    UNUSED(id);
    UNUSED(hash);
    UNUSED(hash_buf_len);
    UNUSED(hashlen);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

/* ---- RSA: the IFEP engine and the mbedTLS software fallback ---- */

hi_s32 kapi_rsa_init(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_deinit(hi_void)
{
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_encrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa)
{
    UNUSED(key);
    UNUSED(rsa);

    hi_log_error("error, rsa is not built into this module\n");
    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_rsa_decrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa)
{
    UNUSED(key);
    UNUSED(rsa);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_rsa_sign_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa)
{
    UNUSED(key);
    UNUSED(rsa);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_rsa_verify_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa)
{
    UNUSED(key);
    UNUSED(rsa);

    return HI_ERR_CIPHER_UNSUPPORTED;
}
