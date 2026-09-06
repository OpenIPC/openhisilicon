/*
 * The hash and RSA halves of the Cipher driver, absent.
 *
 * WHY. The vendor tree is a whole crypto stack, and a kernel module is linked
 * with `ld -r`: every object named in the Kbuild ships, reachable or not,
 * because there is no --gc-sections and nothing to prune a .ko. Of the
 * 110 KB of code and data in open_cipher.ko, 39 KB was RSA -- the IFEP
 * hardware driver plus a whole software mbedTLS bignum/RSA/ASN.1 fallback
 * compiled into the kernel -- and 14 KB was the hash engine, SHA-1 through
 * SHA-512 with HMAC. Half the module, for two subsystems no OpenIPC
 * userspace calls.
 *
 * Nothing calls them, and that is measured rather than assumed. majestic is
 * the only consumer of /dev/cipher on these boards -- it talks to the ioctls
 * directly, see its include/majestic/hisi/cipher_abi.h -- and it uses the
 * symmetric channel, the key ladder and the TRNG. Not the hash: the engine's
 * HMAC-SHA1 closes its handle on finish and so reprograms the key for every
 * packet, which measured 192.4 us against mbedTLS's 37.4, five times worse.
 * Not RSA either: the recorder's public-key mode wraps a clip key once per
 * clip, and mbedTLS does that in userspace.
 *
 * WHAT THIS FILE IS. The twelve entry points those two subsystems export to
 * kapi_dispatch.c, and the only twelve -- `nm` says nothing else in the
 * module reaches into them. init/deinit/release succeed, because
 * crypto_entry() aborts the whole module load on anything else and the
 * symmetric path has no business failing over an absent hash. Every actual
 * operation returns HI_ERR_CIPHER_UNSUPPORTED, which is what a caller of
 * HI_UNF_CIPHER_HashInit() would have to handle anyway on a part whose
 * engine is missing.
 *
 * THE DISPATCH TABLE IS UNTOUCHED, and that is the point of stubbing here
 * rather than editing kapi_dispatch.c. crypto_ioctl() indexes dispatch_func[]
 * by the command's nr and then checks the row's cmd matches, so removing the
 * hash and RSA rows would shift every command after them onto the wrong
 * handler -- and majestic derives its command numbers from the same struct
 * sizes the driver does. The rows stay, the handlers stay, and what changes
 * is only what they call.
 *
 * The vendor sources are still in the tree, unmodified. Restoring either
 * subsystem is putting its objects back in the Kbuild and dropping this file
 * from the list -- not a merge.
 */

#include "drv_osal_lib.h"
#include "cryp_rsa.h"

/* Spelled out rather than borrowed from drv_cipher_kapi.h's CRYPTO_UNUSED,
 * which is ((x) = (x)) and so needs a modifiable lvalue -- and which the
 * hi3516cv500 tree, where this file would go next, does not define. */
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

    HI_LOG_ERROR("error, hash is not built into this module\n");
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

hi_s32 kapi_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 *hashlen)
{
    UNUSED(id);
    UNUSED(hash);
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

hi_s32 kapi_rsa_encrypt(cryp_rsa_key *key, hi_cipher_rsa_enc_scheme scheme,
                        hi_u8 *in, hi_u32 inlen, hi_u8 *out, hi_u32 *outlen)
{
    UNUSED(key);
    UNUSED(scheme);
    UNUSED(in);
    UNUSED(inlen);
    UNUSED(out);
    UNUSED(outlen);

    HI_LOG_ERROR("error, rsa is not built into this module\n");
    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_rsa_decrypt(cryp_rsa_key *key, hi_cipher_rsa_enc_scheme scheme,
                        hi_u8 *in, hi_u32 inlen, hi_u8 *out, hi_u32 *outlen)
{
    UNUSED(key);
    UNUSED(scheme);
    UNUSED(in);
    UNUSED(inlen);
    UNUSED(out);
    UNUSED(outlen);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_rsa_sign_hash(cryp_rsa_key *key, hi_cipher_rsa_sign_scheme scheme,
                          hi_u8 *hash, hi_u32 hlen,
                          hi_u8 *sign, hi_u32 *signlen)
{
    UNUSED(key);
    UNUSED(scheme);
    UNUSED(hash);
    UNUSED(hlen);
    UNUSED(sign);
    UNUSED(signlen);

    return HI_ERR_CIPHER_UNSUPPORTED;
}

hi_s32 kapi_rsa_verify_hash(cryp_rsa_key *key, hi_cipher_rsa_sign_scheme scheme,
                            hi_u8 *hash, hi_u32 hlen,
                            hi_u8 *sign, hi_u32 signlen)
{
    UNUSED(key);
    UNUSED(scheme);
    UNUSED(hash);
    UNUSED(hlen);
    UNUSED(sign);
    UNUSED(signlen);

    return HI_ERR_CIPHER_UNSUPPORTED;
}
