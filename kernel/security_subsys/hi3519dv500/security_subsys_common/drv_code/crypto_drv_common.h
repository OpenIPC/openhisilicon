/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_DRV_COMMON_H
#define CRYPTO_DRV_COMMON_H

#include "crypto_type.h"
#include "drv_common.h"
#include "crypto_osal_adapt.h"
#include "crypto_pke_struct.h"
#include "crypto_hash_struct.h"
#include "crypto_symc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_void *crypto_malloc_coherent(td_u32 size, td_char *buffer_name);

td_void *crypto_malloc_mmz(td_u32 size, td_char *buffer_name);

td_void crypto_free_coherent(td_void *ptr);

td_phys_addr_t crypto_get_phys_addr(td_void *ptr);

td_bool crypto_smmu_support(td_void);

td_void crypto_get_smmu_table_addr(td_u64 *table, td_u64 *rdaddr, td_u64 *wraddr);

crypto_cpu_type crypto_get_cpu_type(td_void);

void crypto_cache_flush(uintptr_t base_addr, td_u32 size);

void crypto_cache_inv(uintptr_t base_addr, td_u32 size);

void crypto_cache_all(void);

void crypto_dcache_enable(void);

void crypto_dcache_disable(void);

td_bool crypto_ecc_support(drv_pke_ecc_curve_type curve_type);

td_bool crypto_rsa_support(td_u32 klen, drv_pke_rsa_scheme scheme);

td_bool crypto_hash_support(crypto_hash_type hash_type);

td_bool crypto_symc_support(crypto_symc_alg alg, crypto_symc_work_mode mode, crypto_symc_key_length key_len,
    crypto_symc_bit_width bit_width);

typedef enum {
    CRYPTO_SM_ALG_SM2,
    CRYPTO_SM_ALG_SM3,
    CRYPTO_SM_ALG_SM4
} crypto_sm_alg;

td_bool crypto_sm_support(crypto_sm_alg alg);

typedef struct {
    const td_char *name;
    td_u32 offset;
} reg_item_t;

typedef td_u32 (*reg_read_func)(td_u32 offset);

typedef enum {
    TIMER_ID_0  = 0,
    TIMER_ID_1,
    TIMER_ID_2,
    TIMER_ID_3,
    TIMER_ID_4,
    TIMER_ID_5,
    TIMER_ID_6,
    TIMER_ID_7,
    TIMER_ID_8,
    TIMER_ID_9,
} crypto_timer_id;
#if defined(CONFIG_CRYPTO_PERF_STATISTICS)
td_void crypto_timer_start(td_u32 timer_id, const td_char *name);

td_u64 crypto_timer_end(td_u32 timer_id, const td_char *item_name);

td_void crypto_timer_print(td_u32 timer_id);

td_void crypto_timer_print_all(td_void);
#else
#define crypto_timer_start(...)
#define crypto_timer_end(...)
#define crypto_timer_print(...)
#define crypto_timer_print_all(...)
#endif

typedef struct {
    td_u32 index;
    td_u32 value;
} crypto_table_item;

td_u32 crypto_get_value_by_index(const crypto_table_item *table, td_u32 table_size,
    td_u32 index, td_u32 *value);
td_u32 crypto_get_index_by_value(const crypto_table_item *table, td_u32 table_size,
    td_u32 value, td_u32 *index);

td_s32 crypto_virt_xor_phys_copy_to_phys(td_phys_addr_t dst_phys_addr, const td_u8 *a_virt_addr,
    td_phys_addr_t b_phys_addr, td_u32 length);

td_s32 crypto_virt_copy_to_phys(td_phys_addr_t dst_phys_addr, const td_u8 *src_virt_addr, td_u32 length);

td_s32 crypto_phys_copy_to_virt(td_u8 *dst_virt_addr, td_phys_addr_t src_phys_addr, td_u32 length);

unsigned int crypto_align(unsigned int original_length, unsigned int aligned_length);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif