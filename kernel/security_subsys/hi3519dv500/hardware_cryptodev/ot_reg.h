/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_REG_H
#define OT_REG_H

#include <linux/module.h>
#include <crypto/skcipher.h>
#include <crypto/internal/hash.h>
#include <crypto/internal/aead.h>
#include <securec.h>
#include "mm_ext.h"
#include "kapi_symc.h"
#include "kapi_km.h"
#include "kapi_otp.h"
#include "kapi_hash.h"

#define AES_MIN_KEY_SIZE	16
#define AES_MAX_KEY_SIZE	32
#define AES_BLOCK_SIZE      16

#define AES_128_KEY_SIZE	16
#define AES_192_KEY_SIZE	24
#define AES_256_KEY_SIZE	32
#define GCM_IV_SIZE         12
#define MAX_TAG_LEN         16
#define MAX_AAD_LEN         (8 * 1024)

#define CIPHER_BUFF_LENGTH              (64 * 1024)

#define INVALID_HANDLE                  0xffffffff

/* debug flag */
extern int hard_crypto_debug;

#define cprint(format, ...)			\
    do {							\
        if (hard_crypto_debug)		\
            printk("ERRO hard crypto" "%s[%u] (%s:%u): " format "\n",	\
                    current->comm, current->pid,	\
                    __func__, __LINE__, ##__VA_ARGS__);				\
    } while (0)

#define cryptodev_chk_return(cond, err_ret, fmt, ...) do {      \
    if (cond) {                                         \
        cprint(fmt, ##__VA_ARGS__);                       \
        return err_ret;                                 \
    }                                                   \
} while (0)

#define cryptodev_chk_goto_with_ret(cond, label, err_ret, fmt, ...) do {           \
    if (cond) {                                                                 \
        cprint(fmt, ##__VA_ARGS__);                                                       \
        ret = err_ret;                                                          \
        goto label;                                                             \
    }                                                                           \
} while (0)

#ifdef crypto_tarce_debug
#define enter_func() printk("[thread id: %d ]enter func %s ========>\r\n", current->pid, __func__)
#define exit_func() printk("[thread id: %d ]exit func %s <=========\r\n", current->pid, __func__)
#else
#define enter_func()
#define exit_func()
#endif

typedef enum {
    OT_DRV_ENCRYPT = 0,
    OT_DRV_DECRYPT = 1
} crypt_flag;

typedef struct {
    td_u32 length[2];
    td_u32 state[CRYPTO_HASH_RESULT_SIZE_MAX_IN_WORD];
    td_u32 tail_len;
    crypto_hash_type hash_type;
    td_u8 tail[CRYPTO_HASH_BLOCK_SIZE_MAX];
} hardware_hash_clone_ctx;

struct hash_ctx {
    td_handle hash_handle;
    crypto_hash_attr hash_attr;
    unsigned int digest_lenth;
    td_bool is_init;
    hardware_hash_clone_ctx clone_ctx;
};

typedef struct {
    crypto_symc_work_mode work_mode;
    crypto_symc_alg cipher_alg;
    crypt_flag flag;
} cipher_param;

// crypto context which contains harden info
struct cipher_aes_ctx {
    unsigned int key_len;
    td_handle symc_handle;
    td_handle keyslot_handle;
    td_handle klad_handle;
    td_phys_addr_t mem_phys;
    td_void *mem_virt;
    td_phys_addr_t aad_mem_phys;
    td_void *aad_mem_virt;
    unsigned int cryptlen;
    crypto_symc_attr cipher_attr;
    km_keyslot_type keyslot_type;
    unsigned int is_init;
    unsigned int authsize;
    u8 key[AES_MAX_KEY_SIZE];
};

extern struct skcipher_alg aes_algs[2];
extern struct shash_alg shash_alges[3];
extern struct aead_alg aes_aead_algs[2];
extern osal_mutex hash_channel_lock;
extern osal_mutex aes_lock;

int inner_cipher_set_key(unsigned int keyslot_handle, const u8 *key, unsigned int keylen);

int inner_aes_crypt(struct skcipher_request *req, cipher_param *param);

int inner_aes_aead_crypt(struct aead_request *req, cipher_param *param);

static inline void crypto_flush_dcache(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    osal_dcache_region_wb(kvirt, phys_addr, length);
}

#endif