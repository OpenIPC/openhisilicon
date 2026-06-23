/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_common.h"
#include "drv_trng.h"
#include "crypto_drv_common.h"

/* crypto dump functions. */
#define MAX_DUMP_LENGHT     (256)
#define BYTES_IN_ONE_LINE   (16)
#ifndef KERN_CONT
#define KERN_CONT
#endif

static crypto_drv_func g_drv_func = {
    .malloc_coherent = TD_NULL,
    .free_coherent = TD_NULL,
    .get_phys_addr = TD_NULL
};

td_void __attribute__((weak)) *crypto_malloc_coherent(td_u32 size, td_char *buffer_name)
{
    crypto_mem_type mem_type = CRYPTO_MEM_TYPE_MMZ;
    if (g_drv_func.malloc_coherent == TD_NULL) {
        return TD_NULL;
    }
    if (crypto_smmu_support()) {
        mem_type = CRYPTO_MEM_TYPE_SMMU;
    }
    return g_drv_func.malloc_coherent(size, mem_type, buffer_name);
}

td_void __attribute__((weak)) *crypto_malloc_mmz(td_u32 size, td_char *buffer_name)
{
    if (g_drv_func.malloc_coherent == TD_NULL) {
        return TD_NULL;
    }
    return g_drv_func.malloc_coherent(size, CRYPTO_MEM_TYPE_MMZ, buffer_name);
}

td_void __attribute__((weak)) crypto_free_coherent(td_void *ptr)
{
    if (g_drv_func.free_coherent == TD_NULL) {
        return;
    }
    g_drv_func.free_coherent(ptr);
}

td_phys_addr_t __attribute__((weak)) crypto_get_phys_addr(td_void *ptr)
{
    if (g_drv_func.get_phys_addr == TD_NULL) {
        return 0;
    }
    return g_drv_func.get_phys_addr(ptr);
}

td_bool __attribute__((weak)) crypto_smmu_support(td_void)
{
    if (g_drv_func.get_smmu_table_addr != TD_NULL) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

td_s32 drv_cipher_register_func(const crypto_drv_func *drv_func_list)
{
    if (drv_func_list == TD_NULL) {
        return TD_FAILURE;
    }
    if (drv_func_list->malloc_coherent == TD_NULL ||
        drv_func_list->free_coherent == TD_NULL ||
        drv_func_list->get_phys_addr == TD_NULL) {
        return TD_FAILURE;
    }

    g_drv_func.malloc_coherent = drv_func_list->malloc_coherent;
    g_drv_func.free_coherent = drv_func_list->free_coherent;
    g_drv_func.get_phys_addr = drv_func_list->get_phys_addr;
    g_drv_func.get_smmu_table_addr = drv_func_list->get_smmu_table_addr;
    g_drv_func.get_cpu_type = drv_func_list->get_cpu_type;

    return TD_SUCCESS;
}

void __attribute__((weak)) crypto_cache_flush(uintptr_t base_addr, td_u32 size)
{
    crypto_unused(base_addr);
    crypto_unused(size);
}

void __attribute__((weak)) crypto_dcache_enable(void)
{
}

void __attribute__((weak)) crypto_dcache_disable(void)
{
}

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    td_bool is_support;
} ecc_alg_support_t;

static ecc_alg_support_t g_ecc_alg_support[] = {
    {DRV_PKE_ECC_TYPE_RFC5639_P256, TD_TRUE},
    {DRV_PKE_ECC_TYPE_RFC5639_P384, TD_TRUE},
    {DRV_PKE_ECC_TYPE_RFC5639_P512, TD_TRUE},
    {DRV_PKE_ECC_TYPE_FIPS_P256K, TD_FALSE},
    {DRV_PKE_ECC_TYPE_FIPS_P256R, TD_FALSE},
    {DRV_PKE_ECC_TYPE_FIPS_P384R, TD_FALSE},
    {DRV_PKE_ECC_TYPE_FIPS_P521R, TD_FALSE},
    {DRV_PKE_ECC_TYPE_RFC7748, TD_TRUE},
    {DRV_PKE_ECC_TYPE_RFC8032, TD_TRUE},
    {DRV_PKE_ECC_TYPE_SM2, TD_TRUE},
};

td_bool crypto_ecc_support(drv_pke_ecc_curve_type curve_type)
{
    td_u32 i;
    for (i = 0; i < crypto_array_size(g_ecc_alg_support); i++) {
        if (curve_type == g_ecc_alg_support[i].curve_type) {
            return g_ecc_alg_support[i].is_support;
        }
    }
    return TD_FALSE;
}

typedef struct {
    td_u32 klen;
    td_bool is_support;
} rsa_alg_keylen_support_t;

static rsa_alg_keylen_support_t g_rsa_alg_keylen_support[] = {
    {CRYPTO_RSA_2048_LEN, TD_FALSE},
    {CRYPTO_RSA_3072_LEN, TD_TRUE},
    {CRYPTO_RSA_4096_LEN, TD_TRUE},
};

typedef struct {
    drv_pke_rsa_scheme scheme;
    td_bool is_support;
} rsa_alg_scheme_support_t;

static rsa_alg_scheme_support_t g_rsa_alg_scheme_support[] = {
    {DRV_PKE_RSA_SCHEME_PKCS1_V15, TD_FALSE},
    {DRV_PKE_RSA_SCHEME_PKCS1_V21, TD_TRUE},
};

td_bool crypto_rsa_support(td_u32 klen, drv_pke_rsa_scheme scheme)
{
    td_u32 i;
    td_bool is_support = TD_FALSE;
    for (i = 0; i < crypto_array_size(g_rsa_alg_keylen_support); i++) {
        if (g_rsa_alg_keylen_support[i].klen == klen) {
            is_support = g_rsa_alg_keylen_support[i].is_support;
            break;
        }
    }
    for (i = 0; i < crypto_array_size(g_rsa_alg_scheme_support); i++) {
        if (g_rsa_alg_scheme_support[i].scheme == scheme) {
            is_support = (is_support && g_rsa_alg_scheme_support[i].is_support);
            break;
        }
    }
    return is_support;
}

typedef struct {
    crypto_hash_type hash_type;
    td_bool is_support;
} hash_alg_support_t;

static hash_alg_support_t g_hash_alg_support[] = {
    {CRYPTO_HASH_TYPE_SHA256, TD_TRUE},
    {CRYPTO_HASH_TYPE_SHA384, TD_TRUE},
    {CRYPTO_HASH_TYPE_SHA512, TD_TRUE},
    {CRYPTO_HASH_TYPE_SM3, TD_TRUE},
    {CRYPTO_HASH_TYPE_HMAC_SHA256, TD_TRUE},
    {CRYPTO_HASH_TYPE_HMAC_SHA384, TD_TRUE},
    {CRYPTO_HASH_TYPE_HMAC_SHA512, TD_TRUE},
    {CRYPTO_HASH_TYPE_HMAC_SM3, TD_TRUE},
};

td_bool crypto_hash_support(crypto_hash_type hash_type)
{
    td_u32 i;
    for (i = 0; i < crypto_array_size(g_hash_alg_support); i++) {
        if (hash_type == g_hash_alg_support[i].hash_type) {
            return g_hash_alg_support[i].is_support;
        }
    }
    return TD_FALSE;
}

typedef struct {
    crypto_symc_alg alg;
    td_bool is_support;
} symc_alg_support_t;

static symc_alg_support_t g_symc_alg_support[] = {
    {CRYPTO_SYMC_ALG_TDES, TD_FALSE},
    {CRYPTO_SYMC_ALG_AES, TD_TRUE},
    {CRYPTO_SYMC_ALG_SM4, TD_TRUE},
    {CRYPTO_SYMC_ALG_LEA, TD_FALSE},
    {CRYPTO_SYMC_ALG_DMA, TD_FALSE},
};

typedef struct {
    crypto_symc_work_mode mode;
    td_bool is_support;
} symc_alg_work_mode_support_t;

static symc_alg_work_mode_support_t g_symc_alg_work_mode_support[] = {
    {CRYPTO_SYMC_WORK_MODE_ECB, TD_FALSE},
    {CRYPTO_SYMC_WORK_MODE_CBC, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_CTR, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_OFB, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_CFB, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_CCM, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_GCM, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_CBC_MAC, TD_TRUE},
    {CRYPTO_SYMC_WORK_MODE_CMAC, TD_TRUE},
};

typedef struct {
    crypto_symc_key_length key_len;
    td_bool is_support;
} symc_alg_keylen_support_t;

static symc_alg_keylen_support_t g_symc_alg_keylen_support[] = {
    {CRYPTO_SYMC_KEY_64BIT, TD_FALSE},
    {CRYPTO_SYMC_KEY_128BIT, TD_TRUE},
    {CRYPTO_SYMC_KEY_192BIT, TD_TRUE},
    {CRYPTO_SYMC_KEY_256BIT, TD_TRUE},
};

typedef struct {
    crypto_symc_bit_width bit_width;
    td_bool is_support;
} symc_alg_bit_width_support_t;

static symc_alg_bit_width_support_t g_symc_alg_bit_width_support[] = {
    {CRYPTO_SYMC_BIT_WIDTH_1BIT, TD_TRUE},
    {CRYPTO_SYMC_BIT_WIDTH_8BIT, TD_TRUE},
    {CRYPTO_SYMC_BIT_WIDTH_64BIT, TD_FALSE},
    {CRYPTO_SYMC_BIT_WIDTH_128BIT, TD_TRUE},
};

td_bool crypto_symc_support(crypto_symc_alg alg, crypto_symc_work_mode mode, crypto_symc_key_length key_len,
    crypto_symc_bit_width bit_width)
{
    td_u32 i;
    td_bool is_support = TD_FALSE;
    for (i = 0; i < crypto_array_size(g_symc_alg_support); i++) {
        if (g_symc_alg_support[i].alg == alg) {
            is_support = g_symc_alg_support[i].is_support;
            break;
        }
    }
    for (i = 0; i < crypto_array_size(g_symc_alg_work_mode_support); i++) {
        if (g_symc_alg_work_mode_support[i].mode == mode) {
            is_support = (is_support && g_symc_alg_work_mode_support[i].is_support);
            break;
        }
    }
    for (i = 0; i < crypto_array_size(g_symc_alg_keylen_support); i++) {
        if (g_symc_alg_keylen_support[i].key_len == key_len) {
            is_support = (is_support && g_symc_alg_keylen_support[i].is_support);
            break;
        }
    }
    for (i = 0; i < crypto_array_size(g_symc_alg_bit_width_support); i++) {
        if (g_symc_alg_bit_width_support[i].bit_width == bit_width) {
            is_support = (is_support && g_symc_alg_bit_width_support[i].is_support);
            break;
        }
    }

    return is_support;
}

/* copy from/to user. */
#if !defined(CRYPTO_COPY_XXX_USER_DEF) && (CRYPTO_ERROR_ENV != ERROR_ENV_NOOS)
td_s32 crypto_copy_from_user(td_void *to, unsigned long to_len, const td_void *from, unsigned long from_len)
{
    if (from_len == 0) {
        return TD_SUCCESS;
    }

    if (to == TD_NULL) {
        crypto_log_err("to is NULL\n");
        return TD_FAILURE;
    }
    if (from == TD_NULL) {
        crypto_log_err("from is NULL\n");
        return TD_FAILURE;
    }
    if (to_len < from_len) {
        crypto_log_err("to_len is Less Than from_len!\n");
        return TD_FAILURE;
    }

    return osal_copy_from_user(to, from, from_len);
}
td_s32 crypto_copy_to_user(td_void  *to, unsigned long to_len, const td_void *from, unsigned long from_len)
{
    if (from_len == 0) {
        return TD_SUCCESS;
    }
    if (to == TD_NULL) {
        crypto_log_err("to is NULL\n");
        return TD_FAILURE;
    }
    if (from == TD_NULL) {
        crypto_log_err("from is NULL\n");
        return TD_FAILURE;
    }
    if (to_len < from_len) {
        crypto_log_err("to_len is Less Than from_len!\n");
        return TD_FAILURE;
    }

    return osal_copy_to_user(to, from, from_len);
}
#endif

#if defined(CONFIG_HEX_DATA_DUMP_ENABLE)
void crypto_dump_data(const char *name, const td_u8 *data, td_u32 data_len)
{
    td_u32 i;
    crypto_unused(name);
    crypto_unused(data);
    if (data == TD_NULL) {
        crypto_log_err("data is NULL\n");
        return;
    }

    crypto_print("%s:\n", name);
    data_len = data_len > MAX_DUMP_LENGHT ? MAX_DUMP_LENGHT : data_len;
    for (i = 0; i < data_len; i++) {
        if (i % BYTES_IN_ONE_LINE == 0) {
            crypto_print("%04x: ", i);
        }
        crypto_print(KERN_CONT"%02x ", data[i]);
        if ((i + 1) % BYTES_IN_ONE_LINE == 0) {
            crypto_print("\n");
        }
    }
    if (data_len % BYTES_IN_ONE_LINE != 0) {
        crypto_print("\n");
    }
}
#endif

td_u32 crypto_get_value_by_index(const crypto_table_item *table, td_u32 table_size,
    td_u32 index, td_u32 *value)
{
    const crypto_table_item *item = TD_NULL;
    td_u32 i;

    crypto_chk_return(table == TD_NULL, TD_FAILURE, "table is NULL\n");
    crypto_chk_return(value == TD_NULL, TD_FAILURE, "value is NULL\n");

    for (i = 0; i < table_size; i++) {
        item = &table[i];
        if (item->index == index) {
            *value = item->value;
            return TD_SUCCESS;
        }
    }
    crypto_log_err("Invalid Index!\n");
    return TD_FAILURE;
}

td_u32 crypto_get_index_by_value(const crypto_table_item *table, td_u32 table_size,
    td_u32 value, td_u32 *index)
{
    const crypto_table_item *item = TD_NULL;
    td_u32 i;

    crypto_chk_return(table == TD_NULL, TD_FAILURE, "table is NULL\n");
    crypto_chk_return(index == TD_NULL, TD_FAILURE, "index is NULL\n");

    for (i = 0; i < table_size; i++) {
        item = &table[i];
        if (item->value == value) {
            *index = item->index;
            return TD_SUCCESS;
        }
    }
    crypto_log_err("Invalid Value!\n");
    return TD_FAILURE;
}

td_s32 __attribute__((weak)) crypto_virt_xor_phys_copy_to_phys(td_phys_addr_t dst_phys_addr, const td_u8 *a_virt_addr,
    td_phys_addr_t b_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u32 i;
    td_u8 *b_virt_addr = TD_NULL;
    td_u8 *dst_virt_addr = TD_NULL;

    b_virt_addr = crypto_ioremap_nocache(b_phys_addr, length);
    crypto_chk_goto_with_ret(b_virt_addr == TD_NULL, exit, TD_FAILURE, "crypto_ioremap_nocache failed\n");

    dst_virt_addr = crypto_ioremap_nocache(dst_phys_addr, length);
    crypto_chk_goto_with_ret(dst_virt_addr == TD_NULL, exit, TD_FAILURE, "crypto_ioremap_nocache failed\n");

#if defined(CRYPTO_CTR_TRACE_ENABLE)
    crypto_dump_data("a_virt_addr", a_virt_addr, length);
    crypto_dump_data("b_virt_addr", b_virt_addr, length);
#endif

    for (i = 0; i < length; i++) {
        dst_virt_addr[i] = a_virt_addr[i] ^ b_virt_addr[i];
    }

#if defined(CRYPTO_CTR_TRACE_ENABLE)
    crypto_dump_data("dst_virt_addr", dst_virt_addr, length);
#endif
    ret = TD_SUCCESS;
exit:
    if (b_virt_addr != TD_NULL) {
        crypto_iounmap(b_virt_addr, length);
    }
    if (dst_virt_addr != TD_NULL) {
        crypto_iounmap(dst_virt_addr, length);
    }
    return ret;
}

td_s32 __attribute__((weak)) crypto_virt_copy_to_phys(td_phys_addr_t dst_phys_addr,
    const td_u8 *src_virt_addr, td_u32 length)
{
    td_s32 ret;
    td_u8 *dst_virt_addr = TD_NULL;

    dst_virt_addr = crypto_ioremap_nocache(dst_phys_addr, length);
    crypto_chk_goto_with_ret(dst_virt_addr == TD_NULL, exit, TD_FAILURE, "crypto_ioremap_nocache failed\n");

    ret = memcpy_s(dst_virt_addr, length, src_virt_addr, length);
    crypto_chk_goto_with_ret(ret != EOK, exit, TD_FAILURE, "memcpy_s failed\n");

    ret = TD_SUCCESS;
exit:
    if (dst_virt_addr != TD_NULL) {
        crypto_iounmap(dst_virt_addr, length);
    }
    return ret;
}


td_s32 __attribute__((weak)) crypto_phys_copy_to_virt(td_u8 *dst_virt_addr,
    td_phys_addr_t src_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u8 *src_virt_addr = TD_NULL;

    src_virt_addr = crypto_ioremap_nocache(src_phys_addr, length);
    crypto_chk_goto_with_ret(src_virt_addr == TD_NULL, exit, TD_FAILURE, "crypto_ioremap_nocache failed\n");

    ret = memcpy_s(dst_virt_addr, length, src_virt_addr, length);
    crypto_chk_goto_with_ret(ret != EOK, exit, TD_FAILURE, "memcpy_s failed\n");

    ret = TD_SUCCESS;
exit:
    if (src_virt_addr != TD_NULL) {
        crypto_iounmap(src_virt_addr, length);
    }
    return ret;
}

unsigned int crypto_align(unsigned int original_length, unsigned int aligned_length)
{
    if (aligned_length == 0) {
        return 0;
    }

    if ((original_length % aligned_length) != 0) {
        original_length += (aligned_length - original_length % aligned_length);
    }

    return original_length;
}