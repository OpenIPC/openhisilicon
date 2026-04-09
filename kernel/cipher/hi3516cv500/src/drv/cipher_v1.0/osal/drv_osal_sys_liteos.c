/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv osal sys liteos adapt for cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"

/* under TEE, we only can malloc secure mmz at system steup,
 * then map the mmz to Smmu, but the smmu can't map to cpu address,
 * so we must save the cpu address in a static table when malloc and map mmz.
 * when call crypto_mem_map, we try to query the table to get cpu address firstly,
 * if can't get cpu address from the table, then call system api to map it.
 */
#define CRYPTO_MEM_MAP_TABLE_DEPTH      32

typedef struct {
    hi_u32      valid;
    compat_addr dma;
    void        *via;
} crypto_mem_map_table;

static crypto_mem_map_table g_loacl_map_table[CRYPTO_MEM_MAP_TABLE_DEPTH];

/* ***************************************************************
 *                       mmz/mmu api                             *
 * ***************************************************************
 *
 * brief allocate and map a mmz or smmu memory
 * we can't allocate smmu directly during TEE boot period.
 * in addition, the buffer of cipher node list must be mmz.
 * so here we have to allocate a mmz memory then map to smmu if necessary.
 */
hi_s32  crypto_mmz_malloc_nocache(hi_char *mmz_name, hi_char *buf_name,
                                  hi_u64 *phy_addr, hi_void **vir_addr,
                                  hi_ulong length)
{
    hil_mmb_t *pmmb = NULL;

    pmmb = hil_mmb_alloc(buf_name, length, 0, 0, mmz_name);
    if (pmmb == NULL) {
        hi_log_print_func_err(hil_mmb_alloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    /* The buffer alloced by mmz is 4k align. */
    *phy_addr = hil_mmb_phys(pmmb);
    if (*phy_addr == 0) {
        hil_mmb_free(pmmb);
        pmmb = NULL;
        hi_log_print_func_err(hil_mmb_phys, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    *vir_addr = hil_mmb_map2kern(pmmb);
    if (*vir_addr == NULL) {
        hil_mmb_free(pmmb);
        pmmb = NULL;
        hi_log_print_func_err(hil_mmb_map2kern, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    return HI_SUCCESS;
}

hi_void crypto_mmz_free(hi_u64 phy_addr, hi_void *vir_addr)
{
    if (vir_addr != NULL) {
        hil_mmb_t *pmmb = hil_mmb_getby_kvirt(vir_addr);

        if (pmmb != NULL) {
            hil_mmb_unmap(pmmb);
            pmmb = NULL;
        }
    }

    if (phy_addr != 0) {
        hil_mmb_freeby_phys(phy_addr);
    }
}

static hi_s32 crypto_mem_alloc_remap(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size)
{
    hi_s32 ret;
    hi_u32 i;
    crypto_memset(mem, sizeof(crypto_mem), 0, sizeof(crypto_mem));

    hi_log_debug("mem_alloc_remap()- name %s, size 0x%x\n", name, size);

    ret = crypto_mmz_malloc_nocache(NULL, (char *)name, &addr_u64(mem->mmz_addr), (void **)&mem->dma_virt, size);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mmz_malloc_nocache, ret);
        return ret;
    }
    addr_u64(mem->dma_addr) = addr_u64(mem->mmz_addr);
    mem->dma_size = size;

    hi_log_debug("MMZ/MMU malloc, MMZ 0x%x, MMZ/MMU 0x%x, VIA 0x%p, SIZE 0x%x\n",
                 addr_u64(mem->mmz_addr), addr_u64(mem->dma_addr), mem->dma_virt, size);

    mem->user_buf = HI_NULL;

    /* save the map info */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if (g_loacl_map_table[i].valid == HI_FALSE) {
            addr_u64(g_loacl_map_table[i].dma) = addr_u64(mem->dma_addr);
            g_loacl_map_table[i].via = mem->dma_virt;
            g_loacl_map_table[i].valid = HI_TRUE;
            hi_log_debug("map local map %d, dam 0x%x, via 0x%p\n", i, addr_u64(mem->dma_addr), mem->dma_virt);
            break;
        }
    }

    return HI_SUCCESS;
}

/* brief release and unmap a mmz or smmu memory. */
static hi_s32 crypto_mem_release_unmap(crypto_mem *mem)
{
    hi_u32 i;
    crypto_mmz_free(addr_u64(mem->mmz_addr), mem->dma_virt);

    /* remove the map info */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if (g_loacl_map_table[i].valid && addr_u64(g_loacl_map_table[i].dma) == addr_u64(mem->dma_addr)) {
            addr_u64(g_loacl_map_table[i].dma) = 0x00;
            g_loacl_map_table[i].via = HI_NULL;
            g_loacl_map_table[i].valid = HI_FALSE;
            hi_log_debug("unmap local map %d, dam 0x%x, via 0x%p\n", i, addr_u64(mem->dma_addr), mem->dma_virt);
            break;
        }
    }
    crypto_memset(mem, sizeof(crypto_mem), 0, sizeof(crypto_mem));

    return HI_SUCCESS;
}

/* brief map a mmz or smmu memory. */
static hi_s32 crypto_mem_map(crypto_mem *mem)
{
    hi_u32 i;

    hi_log_debug("crypto_mem_map()- dma 0x%x, size 0x%x\n",
                 addr_u64(mem->dma_addr), mem->dma_size);

    /* try to query the table to get cpu address firstly,
     * if can't get cpu address from the table, then call system api to map it.
     */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if (g_loacl_map_table[i].valid && addr_u64(g_loacl_map_table[i].dma) == addr_u64(mem->dma_addr)) {
            mem->dma_virt = g_loacl_map_table[i].via;
            hi_log_debug("local map %d, dam 0x%x, via 0x%p\n", i, addr_u64(mem->dma_addr), mem->dma_virt);
            return HI_SUCCESS;
        }
    }

    mem->dma_virt = (hi_u8 *)crypto_osal_ioremap_nocache(addr_u64(mem->dma_addr), mem->dma_size);
    if (mem->dma_virt == HI_NULL) {
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    hi_log_info("crypto_mem_map()- via 0x%p\n", mem->dma_virt);
    return HI_SUCCESS;
}

/* brief unmap a mmz or smmu memory. */
static hi_s32 crypto_mem_unmap(crypto_mem *mem)
{
    hi_u32 i;

    hi_log_debug("crypto_mem_unmap()- dma 0x%x, size 0x%x\n", addr_u64(mem->dma_addr), mem->dma_size);

    /* try to query the table to ummap cpu address firstly,
     * if can't get cpu address from the table, then call system api to unmap it.
     */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if (g_loacl_map_table[i].valid && addr_u64(g_loacl_map_table[i].dma) == addr_u64(mem->dma_addr)) {
            /* this api can't unmap the dma within the map table */
            hi_log_debug("local unmap %d, dam 0x%x, via 0x%p\n", i, addr_u64(mem->dma_addr), mem->dma_virt);
            return HI_SUCCESS;
        }
    }

    crypto_osal_iounmap(mem->dma_virt);
    return HI_SUCCESS;
}

void crypto_mem_init(void)
{
    crypto_memset(&g_loacl_map_table, sizeof(g_loacl_map_table), 0, sizeof(g_loacl_map_table));
}

void crypto_mem_deinit(void)
{
}

void crypto_cpuc_flush_dcache_area(hi_void *kvir, hi_u32 length)
{
}

hi_s32 crypto_mem_create(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size)
{
    hi_log_chk_param_return(mem == HI_NULL);

    return crypto_mem_alloc_remap(mem, type, name, size);
}

hi_s32 crypto_mem_destory(crypto_mem *mem)
{
    hi_log_chk_param_return(mem == HI_NULL);

    return crypto_mem_release_unmap(mem);
}

hi_s32 hash_mem_create(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size)
{
    hi_log_chk_param_return(mem == HI_NULL);

    return crypto_mem_alloc_remap(mem, type, name, size);
}

hi_s32 hash_mem_destory(crypto_mem *mem)
{
    hi_log_chk_param_return(mem == HI_NULL);

    return crypto_mem_release_unmap(mem);
}

hi_s32 crypto_mem_open(crypto_mem *mem, compat_addr dma_addr, hi_u32 dma_size)
{
    hi_log_chk_param_return(mem == HI_NULL);

    mem->dma_addr = dma_addr;
    mem->dma_size = dma_size;

    return crypto_mem_map(mem);
}

hi_s32 crypto_mem_close(crypto_mem *mem)
{
    hi_log_chk_param_return(mem == HI_NULL);

    return crypto_mem_unmap(mem);
}

hi_s32 crypto_mem_attach(crypto_mem *mem, hi_void *buffer)
{
    hi_log_chk_param_return(mem == HI_NULL);

    mem->user_buf = buffer;

    return HI_SUCCESS;
}

hi_s32 crypto_mem_flush(crypto_mem *mem, hi_u32 dma2user, hi_u32 offset, hi_u32 data_size)
{
    hi_log_chk_param_return(mem == HI_NULL);
    hi_log_chk_param_return(mem->dma_virt == HI_NULL);
    hi_log_chk_param_return(mem->user_buf == HI_NULL);
    hi_log_chk_param_return(data_size + offset < data_size);
    hi_log_chk_param_return(data_size + offset > mem->dma_size);

    if (dma2user) {
        crypto_memcpy((hi_u8 *)mem->user_buf + offset, data_size, (hi_u8 *)mem->dma_virt + offset, data_size);
    } else {
        crypto_memcpy((hi_u8 *)mem->dma_virt + offset, data_size, (hi_u8 *)mem->user_buf + offset, data_size);
    }

    return HI_SUCCESS;
}

hi_s32 crypto_mem_phys(crypto_mem *mem, compat_addr *dma_addr)
{
    hi_log_chk_param_return(mem == HI_NULL);

    dma_addr->phy = addr_u64(mem->dma_addr);

    return HI_SUCCESS;
}

void *crypto_mem_virt(crypto_mem *mem)
{
    if (mem == HI_NULL) {
        return HI_NULL;
    }

    return mem->dma_virt;
}

hi_s32 crypto_copy_from_user(hi_void *to, const hi_void *from, unsigned long n)
{
    if (n == 0) {
        return HI_SUCCESS;
    }

    hi_log_chk_param_return(to == HI_NULL);
    hi_log_chk_param_return(from == HI_NULL);

    return osal_copy_from_user(to, from, n);
}

hi_s32 crypto_copy_to_user(hi_void *to, const hi_void *from, unsigned long n)
{
    if (n == 0) {
        return HI_SUCCESS;
    }

    hi_log_chk_param_return(to == HI_NULL);
    hi_log_chk_param_return(from == HI_NULL);

    return osal_copy_to_user(to, from, n);
}

hi_u32 crypto_is_sec_cpu(void)
{
    return module_get_secure();
}

void smmu_get_table_addr(hi_u64 *rdaddr, hi_u64 *wraddr, hi_u64 *table)
{
#ifdef CRYPTO_SMMU_SUPPORT
    hi_u32 smmu_e_raddr, smmu_e_waddr, mmu_pgtbl;
    HI_DRV_SMMU_GetPageTableAddr(&mmu_pgtbl, &smmu_e_raddr, &smmu_e_waddr);

    *rdaddr = smmu_e_raddr;
    *wraddr = smmu_e_waddr;
    *table = mmu_pgtbl;
#else
    *rdaddr = 0x00;
    *wraddr = 0x00;
    *table  = 0x00;
#endif
}

hi_s32 crypto_waitdone_callback(hi_void *param)
{
    hi_u32 *pbDone = param;

    return  *pbDone != HI_FALSE;
}

hi_s32 cipher_check_mmz_phy_addr(hi_u64 phy_addr, hi_u64 length)
{
#ifndef CIPHER_BUILDIN
    hil_mmb_t *mmb = HI_NULL;
    unsigned long mmb_offset = 0;

    /* Check wether the start address is within the MMZ range of the current system. */
    mmb = hil_mmb_getby_phys_2(phy_addr, &mmb_offset);
    if (mmb != NULL) {
        /* Check wether the end address is within the MMZ range of the current system */
        mmb = hil_mmb_getby_phys_2(phy_addr + length - 1, &mmb_offset);
        if (mmb == NULL) {
            hi_log_print_func_err(hil_mmb_getby_phys_2, HI_ERR_CIPHER_INVALID_ADDR);
            return HI_ERR_CIPHER_INVALID_ADDR;
        }
    } else { /* Whether the starting address is within the MMZ range of other systems */
        if (hil_map_mmz_check_phys(phy_addr, length)) {
            hi_log_print_func_err(hil_map_mmz_check_phys, HI_ERR_CIPHER_INVALID_ADDR);
            return HI_ERR_CIPHER_INVALID_ADDR;
        }
    }
#else

    /* check physical addr is ram region. */
    if (pfn_valid(phy_addr >> PAGE_SHIFT) || pfn_valid(length + (phy_addr >> PAGE_SHIFT))) {
#if defined(CONFIG_CMA) && defined(CONFIG_ARCH_HISI_BVT)
        if (is_hicma_address(phy_addr, length)) {
            return HI_SUCCESS;
        } else {
            hi_log_print_func_err(is_hicma_address, HI_ERR_CIPHER_INVALID_ADDR);
            return HI_ERR_CIPHER_INVALID_ADDR;
        }
#endif
        hi_log_error("physical addr is ram region.\n");
        return HI_ERR_CIPHER_INVALID_ADDR;
    } else {
        return HI_SUCCESS;
    }
#endif

    return HI_SUCCESS;
}
