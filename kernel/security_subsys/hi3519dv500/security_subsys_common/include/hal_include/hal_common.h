/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_COMMON_H
#define HAL_COMMON_H

#include "crypto_type.h"

/************************************************** common check log start************************************/
#ifndef hal_crypto_check_param
#define hal_crypto_check_param(module, cond) do { \
    crypto_check_param(ERROR_LAYER_HAL, module, cond, ERROR_INVALID_PARAM); \
} while (0)
#endif

#ifndef hal_crypto_check_param_null
#define hal_crypto_check_param_null(module, _val) do { \
    crypto_check_param_null(ERROR_LAYER_HAL, module, _val); \
} while (0)
#endif

#ifndef hal_crypto_pke_check_param
#define hal_crypto_pke_check_param(cond) do { \
    hal_crypto_check_param(ERROR_MODULE_PKE, cond); \
} while (0)
#endif

#ifndef hal_crypto_pke_check_param_null
#define hal_crypto_pke_check_param_null(_val) do { \
    hal_crypto_check_param_null(ERROR_MODULE_PKE, _val); \
} while (0)
#endif
/************************************************** common check log end************************************/

typedef enum {
    CRYPTO_CPU_TYPE_SCPU,
    CRYPTO_CPU_TYPE_ACPU,
    CRYPTO_CPU_TYPE_HPPCPU,
    CRYPTO_CPU_TYPE_PCPU,
    CRYPTO_CPU_TYPE_AIDSP,
    CRYPTO_CPU_TYPE_INVALID
} crypto_cpu_type;

typedef enum {
    IN_NODE_TYPE_FIRST = 1 << 0,
    IN_NODE_TYPE_NORMAL = 1 << 1,
    IN_NODE_TYPE_LAST = 1 << 2,
    /* CCM. */
    IN_NODE_TYPE_CCM_N = 1 << 3,
    IN_NODE_TYPE_CCM_AAD = 1 << 4,
    IN_NODE_TYPE_CCM_P = 1 << 5,
    IN_NODE_TYPE_CCM_LAST = 1 << 6,
    /* GCM. */
    IN_NODE_TYPE_GCM_FIRST = 1 << 7,
    IN_NODE_TYPE_GCM_A = 1 << 8,
    IN_NODE_TYPE_GCM_P = 1 << 9,
    IN_NODE_TYPE_GCM_LEN = 1 << 10,
    IN_NODE_TYPE_GCM_GHASH = 1 << 11,
    IN_NODE_TYPE_GCM_IV = 1 << 12,
} in_node_type_e;

typedef td_bool (*drv_wait_condition_func)(const td_void *param);
typedef td_s32 (*crypto_wait_timeout_interruptible)(const td_void *wait, drv_wait_condition_func func,
    const td_void *param, const td_u32 timeout_ms);

#endif