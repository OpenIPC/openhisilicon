/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2011-2019. All rights reserved.
* Description: mem common api
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __MM_EXT_H__
#define __MM_EXT_H__

#include "hi_osal.h"
#include "hi_math.h"
#include "hi_common.h"
#include "osal_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern hi_u64 cmpi_mmz_malloc(const hi_char *mmz_name, const hi_char *buf_name, hi_ulong ul_size);
extern hi_void cmpi_mmz_free(hi_u64 phy_addr, hi_void *vir_addr);
extern hi_s32 cmpi_mmz_malloc_nocache(const hi_char *cp_mmz_name, const hi_char *buf_name,
                                    hi_u64 *phy_addr, hi_void **pp_vir_addr, hi_ulong ul_len);
extern hi_s32 cmpi_mmz_malloc_cached(const hi_char *cp_mmz_name, const hi_char *buf_name,
                                   hi_u64 *phy_addr, hi_void **pp_vir_addr, hi_ulong ul_len);


hi_void *cmpi_remap_cached(hi_u64 phy_addr, hi_ulong ul_size);
hi_void *cmpi_remap_nocache(hi_u64 phy_addr, hi_ulong ul_size);
hi_void cmpi_unmap(hi_void *virt_addr);


static inline hi_s32 cmpi_check_mmz_phy_addr(hi_u64 phy_addr, hi_u64 len)
{
    /* if address in mmz of current system */
    if (hil_is_phys_in_mmz(phy_addr, len)) {
        /* if in other system */
        if (hil_map_mmz_check_phys(phy_addr, len)) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static inline hi_bool read_user_linear_space_valid(hi_u8 *addr_start, hi_u32 len)
{
    hi_u8 check;
    hi_u8 *addr_end = HI_NULL;

    if (len == 0) {
        return HI_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_READ, addr_start, len)) {
        return HI_FALSE;
    }

    addr_end = addr_start + len - 1;
    if (osal_copy_from_user(&check, addr_end, 1)) {
        return HI_FALSE;
    }

    return HI_TRUE;
}

static inline hi_bool write_user_linear_space_valid(hi_u8 *addr_start, hi_u32 len)
{
    hi_u8 check = 0;
    hi_u8 *addr_end = HI_NULL;

    if (len == 0) {
        return HI_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_WRITE, addr_start, len)) {
        return HI_FALSE;
    }

    addr_end = addr_start + len - 1;
    if (osal_copy_to_user(addr_end, &check, 1)) {
        return HI_FALSE;
    }

    return HI_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
