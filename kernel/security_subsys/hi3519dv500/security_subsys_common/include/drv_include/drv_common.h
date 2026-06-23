/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DRV_COMMON_H
#define DRV_COMMON_H

#include "crypto_type.h"
#include "crypto_common_struct.h"
#include "hal_common.h"

/************************************************** common check log start************************************/
#ifndef drv_crypto_check_param
#define drv_crypto_check_param(module, cond) do { \
    crypto_check_param(ERROR_LAYER_DRV, module, cond, ERROR_INVALID_PARAM); \
} while (0)
#endif

#ifndef drv_crypto_check_param_null
#define drv_crypto_check_param_null(module, _val) do { \
    crypto_check_param_null(ERROR_LAYER_DRV, module, _val); \
} while (0)
#endif

#ifndef drv_crypto_pke_check_param
#define drv_crypto_pke_check_param(cond) do { \
    drv_crypto_check_param(ERROR_MODULE_PKE, cond); \
} while (0)
#endif

#ifndef drv_crypto_pke_check_param_null
#define drv_crypto_pke_check_param_null(_val) do { \
    drv_crypto_check_param_null(ERROR_MODULE_PKE, _val); \
} while (0)
#endif
/************************************************** common check log end************************************/

typedef enum {
    CRYPTO_MEM_TYPE_MMZ,
    CRYPTO_MEM_TYPE_SMMU,
} crypto_mem_type;

typedef void *(*func_malloc)(unsigned int size, crypto_mem_type mem_type, const char *buffer_name);
typedef void (*func_free)(void *ptr);
typedef td_phys_addr_t (*func_get_phys_addr)(void *ptr);
typedef void *(*func_get_virt_addr)(void *ptr);
typedef td_bool (*func_get_smmu_table_addr)(td_u64 *table, td_u64 *rdaddr, td_u64 *wraddr);
typedef void *(*func_register_coherent_mem)(void *virt_addr, td_phys_addr_t phys_addr, unsigned int size);
typedef void (*func_unregister_coherent_mem)(void *virt_addr);
typedef crypto_cpu_type (*func_get_cpu_type)(void);

typedef struct {
    func_malloc malloc_coherent;
    func_free free_coherent;
    func_get_phys_addr get_phys_addr;
    func_get_virt_addr get_virt_addr;
    func_get_smmu_table_addr get_smmu_table_addr;
    func_register_coherent_mem register_coherent_mem;
    func_unregister_coherent_mem unregister_coherent_mem;
    func_get_cpu_type get_cpu_type;
} crypto_drv_func;

td_s32 drv_cipher_register_func(const crypto_drv_func *drv_func_list);

td_s32 drv_cipher_hash_suspend(td_void);

td_s32 drv_cipher_hash_resume(td_void);

#endif