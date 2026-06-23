/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_SYS_MEM_H
#define SS_MPI_SYS_MEM_H

#include "ot_type.h"
#include "ot_common_sys_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

td_void *ss_mpi_sys_mmap(td_phys_addr_t phys_addr, td_u32 size);

td_void *ss_mpi_sys_mmap_cached(td_phys_addr_t phys_addr, td_u32 size);

td_s32 ss_mpi_sys_munmap(const td_void *virt_addr, td_u32 size);

/* alloc mmz memory in user context */
td_s32 ss_mpi_sys_mmz_alloc(td_phys_addr_t *phys_addr, td_void **virt_addr,
    const td_char *mmb, const td_char *zone, td_u32 len);

/* alloc mmz memory with cache in user context */
td_s32 ss_mpi_sys_mmz_alloc_cached(td_phys_addr_t *phys_addr, td_void **virt_addr,
    const td_char *mmb, const td_char *zone, td_u32 len);

/* free mmz memory in user context */
td_s32 ss_mpi_sys_mmz_free(td_phys_addr_t phys_addr, const td_void *virt_addr);

/* flush cache */
td_s32 ss_mpi_sys_flush_cache(td_phys_addr_t phys_addr, td_void *virt_addr, td_u32 size);

/* get virtual meminfo according to virtual addr, should be in one process */
td_s32 ss_mpi_sys_get_virt_mem_info(const td_void *virt_addr, ot_sys_virt_mem_info *mem_info);

td_s32 ss_mpi_sys_mem_share(const td_void *mem_handle, td_s32 pid);
td_s32 ss_mpi_sys_mem_unshare(const td_void *mem_handle, td_s32 pid);
td_s32 ss_mpi_sys_mem_share_all(const td_void *mem_handle);
td_s32 ss_mpi_sys_mem_unshare_all(const td_void *mem_handle);

td_s32 ss_mpi_sys_get_mem_info_by_virt(const td_void *virt_addr, ot_sys_mem_info *mem_info);
td_s32 ss_mpi_sys_get_mem_info_by_phys(td_phys_addr_t phys_addr, ot_sys_mem_info *mem_info);
td_s32 ss_mpi_sys_get_mem_info_by_handle(const td_void *mem_handle, ot_sys_mem_info *mem_info);

#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_SYS_MEM_H */
