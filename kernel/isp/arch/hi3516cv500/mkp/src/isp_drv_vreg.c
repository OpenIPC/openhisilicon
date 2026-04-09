/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_drv_vreg.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_drv_vreg.h"
#include "hi_osal.h"
#include "mm_ext.h"
#include "isp_vreg.h"

static drv_vreg_args g_vreg[ISP_MAX_PIPE_NUM][VREG_MAX_NUM] = {0};
drv_vreg_args *vreg_drv_search(hi_vi_pipe vi_pipe, hi_u64 base_addr)
{
    hi_s32  i;

    for (i = 0; i < VREG_MAX_NUM; i++) {
        if ((g_vreg[vi_pipe][i].phy_addr != 0) &&
            (base_addr == g_vreg[vi_pipe][i].base_addr)) {
            return &g_vreg[vi_pipe][i];
        }
    }

    return HI_NULL;
}

drv_vreg_args *vreg_drv_query(hi_u64 base_addr)
{
    hi_s32  i, j;

    for (j = 0; j < ISP_MAX_PIPE_NUM; j++) {
        for (i = 0; i < VREG_MAX_NUM; i++) {
            if ((g_vreg[j][i].phy_addr != 0) &&
                (base_addr == g_vreg[j][i].base_addr)) {
                return &g_vreg[j][i];
            }
        }
    }

    return HI_NULL;
}

hi_s32 vreg_drv_init(hi_vi_pipe vi_pipe, hi_u64 base_addr, hi_u64 size)
{
    hi_s32 ret;
    hi_s32  i;
    hi_u64 phy_addr;
    hi_u8 *vir_addr = HI_NULL;
    hi_char name[MAX_VREG_NAME_LEN] = {0};
    drv_vreg_args *vreg_args = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    /* check param */
    if (size == 0) {
        isp_err_trace("The vreg's size is 0!\n");
        return HI_FAILURE;
    }

    vreg_args = vreg_drv_search(vi_pipe, base_addr);
    if (vreg_args != HI_NULL) {
        isp_err_trace("The vreg of base_addr 0x%llx has registerd!\n", base_addr);
        return HI_FAILURE;
    }

    /* search pos */
    for (i = 0; i < VREG_MAX_NUM; i++) {
        if (g_vreg[vi_pipe][i].phy_addr == 0) {
            vreg_args = &g_vreg[vi_pipe][i];
            break;
        }
    }

    if (vreg_args == HI_NULL) {
        isp_err_trace("The vreg is too many, can't register!\n");
        return HI_FAILURE;
    }

    /* Mmz malloc memory */
    osal_snprintf(name, sizeof(name), "ISP[%d].Vreg[%d]", vi_pipe, i);
    ret = cmpi_mmz_malloc_nocache(HI_NULL, name, &phy_addr, (hi_void **)&vir_addr, size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc virt regs buf err\n");
        return HI_FAILURE;
    }

    osal_memset(vir_addr, 0, size);

    vreg_args->phy_addr  = phy_addr;
    vreg_args->virt_addr   = (hi_void *)vir_addr;
    vreg_args->base_addr = base_addr;

    return HI_SUCCESS;
}

hi_s32 vreg_drv_exit(hi_vi_pipe vi_pipe, hi_u64 base_addr)
{
    drv_vreg_args *vreg_args = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    vreg_args = vreg_drv_search(vi_pipe, base_addr);
    if (vreg_args == HI_NULL) {
        isp_warn_trace("The vreg of base_addr 0x%llx has not registerd!\n", base_addr);
        return HI_FAILURE;
    }

    if (vreg_args->phy_addr != 0) {
        cmpi_mmz_free(vreg_args->phy_addr, vreg_args->virt_addr);
        vreg_args->phy_addr  = 0;
        vreg_args->size     = 0;
        vreg_args->base_addr = 0;
        vreg_args->virt_addr   = HI_NULL;
    }

    return HI_SUCCESS;
}

hi_s32 vreg_drv_get_addr(hi_vi_pipe vi_pipe, hi_u64 base_addr, hi_u64 *phy_addr)
{
    drv_vreg_args *vreg_args = HI_NULL;

    vreg_args = vreg_drv_query(base_addr);
    if (vreg_args == HI_NULL) {
        isp_warn_trace("The vreg of base_addr 0x%llx has not registerd!\n", base_addr);
        return HI_FAILURE;
    }

    *phy_addr = vreg_args->phy_addr;

    return HI_SUCCESS;
}

hi_s32 vreg_drv_release_all(hi_vi_pipe vi_pipe)
{
    hi_s32  i;

    isp_check_pipe_return(vi_pipe);

    for (i = 0; i < VREG_MAX_NUM; i++) {
        if (g_vreg[vi_pipe][i].phy_addr != 0) {
            cmpi_mmz_free(g_vreg[vi_pipe][i].phy_addr, g_vreg[vi_pipe][i].virt_addr);
            g_vreg[vi_pipe][i].phy_addr  = 0;
            g_vreg[vi_pipe][i].base_addr = 0;
            g_vreg[vi_pipe][i].size     = 0;
            g_vreg[vi_pipe][i].virt_addr   = HI_NULL;
        }
    }

    return HI_SUCCESS;
}

long vreg_drv_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    hi_vi_pipe vi_pipe;
    unsigned int *argp = (unsigned int *)(hi_uintptr_t)arg;
    drv_vreg_args *vreg_args = HI_NULL;
    vi_pipe = isp_get_dev(private_data);

    switch (cmd) {
        case VREG_DRV_FD:
            isp_check_pointer_return(arg);
            *((hi_u32 *)(private_data)) = *(hi_u32 *)argp;

            return HI_SUCCESS;

        /* malloc memory for vregs, and record information in kernel. */
        case VREG_DRV_INIT:
            isp_check_pointer_return(argp);
            vreg_args = (drv_vreg_args *)argp;

            return vreg_drv_init(vi_pipe, vreg_args->base_addr, vreg_args->size);

        /* free the memory of vregs, and clean information in kernel. */
        case VREG_DRV_EXIT:
            isp_check_pointer_return(argp);
            vreg_args = (drv_vreg_args *)argp;

            return vreg_drv_exit(vi_pipe, vreg_args->base_addr);

        /* free the memory of vregs, and clean information in kernel. */
        case VREG_DRV_RELEASE_ALL:
            isp_check_pointer_return(argp);
            vreg_args = (drv_vreg_args *)argp;

            if ((vreg_args->size == ISP_VREG_SIZE) &&
                (vreg_args->base_addr == ISP_VREG_BASE)) {
                vreg_drv_release_all(vi_pipe);
            }

            return HI_SUCCESS;

        /* get the mapping relation between vreg addr and physical addr. */
        case VREG_DRV_GETADDR:
            isp_check_pointer_return(argp);
            vreg_args = (drv_vreg_args *)argp;

            return vreg_drv_get_addr(vi_pipe, vreg_args->base_addr, &vreg_args->phy_addr);

        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

