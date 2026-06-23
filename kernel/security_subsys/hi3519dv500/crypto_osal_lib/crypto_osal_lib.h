/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_OSAL_LIB_H
#define CRYPTO_OSAL_LIB_H

#include "ot_osal.h"
#include "crypto_type.h"
#include "crypto_platform.h"
#include "crypto_common_struct.h"

#include <securec.h>

unsigned long crypto_osal_get_phys_addr(const crypto_buf_attr *buf);
#define crypto_osal_put_phys_addr(...)
#define crypto_osal_mem_handle_get(fd, module_id)   1
#define crypto_osal_mem_handle_put(...)

#define crypto_getpid()                         osal_get_current_tgid()
#define crypto_gettid()                         osal_get_current_tid()

/* Register Read&Write. */
typedef enum {
    REG_REGION_SPACC = 0x0,
    REG_REGION_TRNG,
    REG_REGION_PKE,
    REG_REGION_CA_MISC,
    REG_REGION_KM,
    REG_REGION_OTPC,
    REG_REGION_NUM
} reg_region_e;

td_s32 crypto_get_random(td_u32 *randnum);

td_s32 crypto_register_reg_map_region(reg_region_e region);

void crypto_unregister_reg_map_region(reg_region_e region);

td_u32 crypto_ex_reg_read(reg_region_e region, td_u32 offset);

td_void crypto_ex_reg_write(reg_region_e region, td_u32 offset, td_u32 value);

#define spacc_reg_read(offset)          crypto_ex_reg_read(REG_REGION_SPACC, offset)
#define spacc_reg_write(offset, value)  crypto_ex_reg_write(REG_REGION_SPACC, offset, value)

#define trng_reg_read(offset)           crypto_ex_reg_read(REG_REGION_TRNG, offset)
#define trng_reg_write(offset, value)   crypto_ex_reg_write(REG_REGION_TRNG, offset, value)

#define pke_reg_read(offset)            crypto_ex_reg_read(REG_REGION_PKE, offset)
#define pke_reg_write(offset, value)    crypto_ex_reg_write(REG_REGION_PKE, offset, value)

#define km_reg_read(offset)             crypto_ex_reg_read(REG_REGION_KM, offset)
#define km_reg_write(offset, value)     crypto_ex_reg_write(REG_REGION_KM, offset, value)

#define otpc_reg_read(offset)              crypto_ex_reg_read(REG_REGION_OTPC, offset)
#define otpc_reg_write(offset, value)      crypto_ex_reg_write(REG_REGION_OTPC, offset, value)

#define ca_misc_reg_read(offset)             crypto_ex_reg_read(REG_REGION_CA_MISC, offset)
#define ca_misc_reg_write(offset, value)     crypto_ex_reg_write(REG_REGION_CA_MISC, offset, value)

#define CRYPTO_EXPORT_SYMBOL        EXPORT_SYMBOL

typedef enum {
    CRYPTO_OSAL_MMZ_TYPE,
    CRYPTO_OSAL_NSSMMU_TYPE,
    CRYPTO_OSAL_ERROR_TYPE,
} crypto_osal_mem_type;

crypto_osal_mem_type crypto_osal_get_mem_type(td_void *mem_handle);

#define crypto_osal_mem_phys(mem_handle)                        osal_mem_phys(mem_handle)
#define crypto_osal_mem_nssmmu_map(mem_handle)                  osal_mem_nssmmu_map(mem_handle, 0)
#define crypto_osal_mem_nssmmu_unmap(mem_handle, smmu_addr)     osal_mem_nssmmu_unmap(mem_handle, smmu_addr, 0)

#define crypto_memory_barrier() do { osal_mb(); osal_isb(); osal_dsb(); } while (0)

#define CRYPTO_ERROR_ENV                            ERROR_ENV_LINUX

#endif