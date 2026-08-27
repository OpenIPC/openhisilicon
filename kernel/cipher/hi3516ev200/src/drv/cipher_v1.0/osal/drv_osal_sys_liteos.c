/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_osal_sys_liteos.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"

/************************* Internal Structure Definition *********************/
/** \addtogroup      base type*/
/** @{*/  /** <!-- [base]*/

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

static crypto_mem_map_table loacl_map_table[CRYPTO_MEM_MAP_TABLE_DEPTH];

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      base*/
/** @{*/  /** <!--[base]*/

/*****************************************************************
 *                       mmz/mmu api                             *
 *****************************************************************/

/*brief allocate and map a mmz or smmu memory
* we can't allocate smmu directly during TEE boot period.
* in addition, the buffer of cipher node list must be mmz.
* so here we have to allocate a mmz memory then map to smmu if necessary.
*/
hi_s32  crypto_mmz_malloc_nocache(hi_char* mmz_name, hi_char* buf_name,
                                  hi_u64* phy_addr, hi_void** vir_addr,
                                  hi_ulong length)
{
    hil_mmb_t *pmmb = NULL;

    pmmb = hil_mmb_alloc(buf_name, length, 0, 0, mmz_name);
    if (pmmb == NULL) {
        HI_LOG_PRINT_FUNC_ERR(hil_mmb_alloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    /* The buffer alloced by mmz is 4k align. */
    *phy_addr = hil_mmb_phys(pmmb);
    if (*phy_addr == 0) {
        hil_mmb_free(pmmb);
        pmmb = NULL;
        HI_LOG_PRINT_FUNC_ERR(hil_mmb_phys, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    *vir_addr = hil_mmb_map2kern(pmmb);
    if (*vir_addr == NULL) {
        hil_mmb_free(pmmb);
        pmmb = NULL;
        HI_LOG_PRINT_FUNC_ERR(hil_mmb_map2kern, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    return HI_SUCCESS;
}

hi_void crypto_mmz_free(hi_u64 phy_addr, hi_void* vir_addr)
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

static hi_s32 crypto_mem_alloc_remap(crypto_mem *mem, hi_u32 type, char const *name, hi_u32 size)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 i;
    crypto_memset(mem, sizeof(crypto_mem), 0, sizeof(crypto_mem));

    HI_LOG_DEBUG("mem_alloc_remap()- name %s, size 0x%x\n", name, size);

    ret = crypto_mmz_malloc_nocache(NULL, (char *)name, &ADDR_U64(mem->mmz_addr), (void **)&mem->dma_virt, size);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(crypto_mmz_malloc_nocache, ret);
        return ret;
    }
    ADDR_U64(mem->dma_addr) = ADDR_U64(mem->mmz_addr);
    mem->dma_size = size;

    HI_LOG_DEBUG("MMZ/MMU malloc, MMZ 0x%x, MMZ/MMU 0x%x, VIA 0x%p, SIZE 0x%x\n",
                 ADDR_U64(mem->mmz_addr), ADDR_U64(mem->dma_addr), mem->dma_virt, size);

    mem->user_buf = HI_NULL;

    /* save the map info */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if (loacl_map_table[i].valid == HI_FALSE) {
            ADDR_U64(loacl_map_table[i].dma) = ADDR_U64(mem->dma_addr);
            loacl_map_table[i].via = mem->dma_virt;
            loacl_map_table[i].valid = HI_TRUE;
            HI_LOG_DEBUG("map local map %d, dam 0x%x, via 0x%p\n",
                         i, ADDR_U64(mem->dma_addr), mem->dma_virt);
            break;
        }
    }

    return HI_SUCCESS;
}

/*brief release and unmap a mmz or smmu memory */
static hi_s32 crypto_mem_release_unmap(crypto_mem *mem)
{
    hi_u32 i;
    crypto_mmz_free(ADDR_U64(mem->mmz_addr), mem->dma_virt);

    /* remove the map info */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if ( loacl_map_table[i].valid &&
             ADDR_U64(loacl_map_table[i].dma) == ADDR_U64(mem->dma_addr)) {
            ADDR_U64(loacl_map_table[i].dma) = 0x00;
            loacl_map_table[i].via = HI_NULL;
            loacl_map_table[i].valid = HI_FALSE;
            HI_LOG_DEBUG("unmap local map %d, dam 0x%x, via 0x%p\n",
                         i, ADDR_U64(mem->dma_addr), mem->dma_virt);
            break;
        }
    }
    crypto_memset(mem, sizeof(crypto_mem), 0, sizeof(crypto_mem));

    return HI_SUCCESS;
}

/*brief map a mmz or smmu memory */
static hi_s32 crypto_mem_map(crypto_mem *mem)
{
    hi_u32 i;

    HI_LOG_DEBUG("crypto_mem_map()- dma 0x%x, size 0x%x\n",
                 ADDR_U64(mem->dma_addr), mem->dma_size);

    /* try to query the table to get cpu address firstly,
     * if can't get cpu address from the table, then call system api to map it.
     */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if ( loacl_map_table[i].valid &&
             ADDR_U64(loacl_map_table[i].dma) == ADDR_U64(mem->dma_addr)) {
            mem->dma_virt = loacl_map_table[i].via;
            HI_LOG_DEBUG("local map %d, dam 0x%x, via 0x%p\n",
                         i, ADDR_U64(mem->dma_addr), mem->dma_virt);
            return HI_SUCCESS;
        }
    }

    mem->dma_virt = (hi_u8 *)crypto_osal_ioremap_nocache(ADDR_U64(mem->dma_addr), mem->dma_size);
    if (mem->dma_virt == HI_NULL) {
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    HI_LOG_INFO("crypto_mem_map()- via 0x%p\n", mem->dma_virt);

    return HI_SUCCESS;

}

/*brief unmap a mmz or smmu memory */
static hi_s32 crypto_mem_unmap(crypto_mem *mem)
{
    hi_u32 i;

    HI_LOG_DEBUG("crypto_mem_unmap()- dma 0x%x, size 0x%x\n",
                 ADDR_U64(mem->dma_addr), mem->dma_size);

    /* try to query the table to ummap cpu address firstly,
     * if can't get cpu address from the table, then call system api to unmap it.
     */
    for (i = 0; i < CRYPTO_MEM_MAP_TABLE_DEPTH; i++) {
        if ( loacl_map_table[i].valid &&
             ADDR_U64(loacl_map_table[i].dma) == ADDR_U64(mem->dma_addr)) {
            /* this api can't unmap the dma within the map table */
            HI_LOG_DEBUG("local unmap %d, dam 0x%x, via 0x%p\n",
                         i, ADDR_U64(mem->dma_addr), mem->dma_virt);
            return HI_SUCCESS;
        }
    }

    crypto_osal_iounmap(mem->dma_virt);

    return HI_SUCCESS;
}

void crypto_mem_init(void)
{
    crypto_memset(&loacl_map_table, sizeof(loacl_map_table), 0, sizeof(loacl_map_table));
}

void crypto_mem_deinit(void)
{

}

void crypto_cpuc_flush_dcache_area(void *kvir, hi_u32 length)
{

}

hi_s32 crypto_mem_create(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    return crypto_mem_alloc_remap(mem, type, name, size);
}

hi_s32 crypto_mem_destory(crypto_mem *mem)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    return crypto_mem_release_unmap(mem);
}

hi_s32 hash_mem_create(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    return crypto_mem_alloc_remap(mem, type, name, size);
}

hi_s32 hash_mem_destory(crypto_mem *mem)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    return crypto_mem_release_unmap(mem);
}

hi_s32 crypto_mem_open(crypto_mem *mem, compat_addr dma_addr, hi_u32 dma_size)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    mem->dma_addr = dma_addr;
    mem->dma_size = dma_size;

    return crypto_mem_map(mem);
}

hi_s32 crypto_mem_close(crypto_mem *mem)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    return crypto_mem_unmap(mem);;
}

hi_s32 crypto_mem_attach(crypto_mem *mem, void *buffer)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    mem->user_buf = buffer;

    return HI_SUCCESS;
}

hi_s32 crypto_mem_flush(crypto_mem *mem, hi_u32 dma2user, hi_u32 offset, hi_u32 data_size)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);
    HI_LOG_CHECK_PARAM(mem->dma_virt == HI_NULL);
    HI_LOG_CHECK_PARAM(mem->user_buf == HI_NULL);
    HI_LOG_CHECK_PARAM(data_size + offset < data_size);
    HI_LOG_CHECK_PARAM(data_size + offset > mem->dma_size);

    if (dma2user) {
        crypto_memcpy((hi_u8 *)mem->user_buf + offset, data_size,
                      (hi_u8 *)mem->dma_virt + offset, data_size);
    } else {
        crypto_memcpy((hi_u8 *)mem->dma_virt + offset, data_size,
                      (hi_u8 *)mem->user_buf + offset, data_size);
    }

    return HI_SUCCESS;
}

hi_s32 crypto_mem_phys(crypto_mem *mem, compat_addr *dma_addr)
{
    HI_LOG_CHECK_PARAM(mem == HI_NULL);

    dma_addr->phy = ADDR_U64(mem->dma_addr);

    return HI_SUCCESS;
}

void *crypto_mem_virt(crypto_mem *mem)
{
    if (mem == HI_NULL) {
        return HI_NULL;
    }

    return mem->dma_virt;
}

hi_s32 crypto_copy_from_user(void *to, const void  *from, unsigned long n)
{
    if (n == 0) {
        return HI_SUCCESS;
    }

    HI_LOG_CHECK_PARAM(to == HI_NULL);
    HI_LOG_CHECK_PARAM(from == HI_NULL);

    return osal_copy_from_user(to, from, n);
}

hi_s32 crypto_copy_to_user(void *to, const void  *from, unsigned long n)
{
    if (n == 0) {
        return HI_SUCCESS;
    }

    HI_LOG_CHECK_PARAM(to == HI_NULL);
    HI_LOG_CHECK_PARAM(from == HI_NULL);

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

hi_s32 crypto_waitdone_callback(void *param)
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
            HI_LOG_PRINT_FUNC_ERR(hil_mmb_getby_phys_2, HI_FAILURE);
            return HI_FAILURE;
        }
    } else { /* Whether the starting address is within the MMZ range of other systems */
        if (hil_map_mmz_check_phys(phy_addr, length)) {
            HI_LOG_PRINT_FUNC_ERR(hil_map_mmz_check_phys, HI_FAILURE);
            return HI_FAILURE;
        }
    }
#else

    /*check physical addr is ram region*/
    if (pfn_valid(phy_addr >> PAGE_SHIFT) || pfn_valid(length + (phy_addr >> PAGE_SHIFT))) {
#if defined(CONFIG_CMA) && defined(CONFIG_ARCH_HISI_BVT)
        if (is_hicma_address(phy_addr, length)) {
            return HI_SUCCESS;
        } else {
            HI_LOG_PRINT_FUNC_ERR(is_hicma_address, HI_FAILURE);
            return HI_FAILURE;
        }
#endif
        HI_LOG_ERROR("physical addr is ram region.\n");
        return HI_FAILURE;
    } else {
        return HI_SUCCESS;
    }
#endif

    return HI_SUCCESS;
}
/** @}*/  /** <!-- ==== API Code end ====*/
