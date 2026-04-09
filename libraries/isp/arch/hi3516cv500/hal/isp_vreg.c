/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_vreg.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_drv_vreg.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "mpi_sys.h"
#include "isp_vreg.h"
#include "hi_comm_isp_adapt.h"


typedef struct {
    hi_u64  size;
    hi_u64  base_addr;
    hi_u64  phy_addr;
    hi_u64  virt_addr;
} vreg_args;

typedef struct {
    hi_u64  phy_addr;
    hi_void  *virt_addr;
} hi_vreg_addr;

typedef struct {
    hi_vreg_addr slave_time_addr;
    hi_vreg_addr slave_reg_addr[CAP_SLAVE_MAX_NUM];
    hi_vreg_addr isp_fe_reg_addr[ISP_MAX_PIPE_NUM];
    hi_vreg_addr isp_be_reg_addr[ISP_MAX_BE_NUM];
    hi_vreg_addr isp_hdr_reg_addr[ISP_MAX_BE_NUM];
    hi_vreg_addr viproc_reg_addr[ISP_MAX_BE_NUM];
    hi_vreg_addr isp_vreg_addr[ISP_MAX_PIPE_NUM];
    hi_vreg_addr ae_vreg_addr[MAX_ALG_LIB_VREG_NUM];
    hi_vreg_addr awb_vreg_addr[MAX_ALG_LIB_VREG_NUM];
    hi_vreg_addr af_vreg_addr[MAX_ALG_LIB_VREG_NUM];
} hi_vreg;

static hi_vreg g_hi_vreg = {{ 0 }};

hi_s32 g_vreg_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

static hi_s32 vreg_check_open(hi_vi_pipe vi_pipe)
{
    if (g_vreg_fd[vi_pipe] <= 0) {
        g_vreg_fd[vi_pipe] = open("/dev/isp_dev", O_RDONLY, S_IRUSR);
        if (g_vreg_fd[vi_pipe] < 0) {
            perror("open isp device error!\n");
            return HI_FAILURE;
        }
        if (ioctl(g_vreg_fd[vi_pipe], VREG_DRV_FD, &vi_pipe)) {
            close(g_vreg_fd[vi_pipe]);
            g_vreg_fd[vi_pipe] = -1;
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static inline hi_s32 vreg_check0_open(hi_void)
{
    if (g_vreg_fd[0] <= 0) {
        g_vreg_fd[0] = open("/dev/isp_dev", O_RDONLY, S_IRUSR);
        if (g_vreg_fd[0] < 0) {
            perror("open isp device error!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 g_s_mem_dev = -1;
#define isp_mem_dev_open if (g_s_mem_dev <= 0)\
    {\
        g_s_mem_dev = open("/dev/mem", O_RDWR|O_SYNC,S_IRUSR | S_IRUSR);\
        if (g_s_mem_dev < 0) {\
            perror("Open dev/mem error");\
            return HI_NULL;\
        }\
    }\

static hi_void *vreg_io_mmap(hi_u64 phy_addr, hi_u32 size)
{
    hi_u32 diff;
    hi_u64 page_phy;
    hi_u8 *page_addr = HI_NULL;
    hi_ulong  page_size;

    isp_mem_dev_open;

    /*
     * PageSize will be 0 when u32size is 0 and diff is 0,
     * and then mmap will be error (error: Invalid argument)
     */
    if (!size) {
        isp_err_trace("size can't be 0.\n");
        return HI_NULL;
    }

    /* The mmap address should align with page */
    page_phy = phy_addr & 0xfffffffffffff000ULL;
    diff     = phy_addr - page_phy;

    /* The mmap size shuld be mutliples of 1024 */
    page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    page_addr = mmap((void *)0, page_size, PROT_READ | PROT_WRITE,
                     MAP_SHARED, g_s_mem_dev, page_phy);
    if (page_addr == MAP_FAILED) {
        perror("mmap error");
        return HI_NULL;
    }

    return (hi_void *)(page_addr + diff);
}

static inline hi_bool vreg_check_slave_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(base_addr, SLAVE_REG_BASE, cap_slave_reg_base(CAP_SLAVE_MAX_NUM));
}

static inline hi_bool vreg_check_slave_mode_time_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(base_addr, SLAVE_MODE_REG_BASE, SLAVE_MODE_REG_BASE);
}

static inline hi_bool vreg_check_isp_fe_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(get_isp_reg_base(base_addr), FE_REG_BASE, isp_fe_reg_base(ISP_MAX_PIPE_NUM));
}

static inline hi_bool vreg_check_isp_be_base(hi_u32 base_addr)
{
    return (hi_bool)(isp_check_reg_base(get_isp_reg_base(base_addr), BE_REG_BASE, isp_be_reg_base(ISP_MAX_BE_NUM)) &&
                     ((get_isp_reg_base(base_addr)) == (isp_be_reg_base(isp_get_be_id(base_addr)))));
}

static inline hi_bool vreg_check_viproc_base(hi_u32 base_addr)
{
    return (hi_bool)(isp_check_reg_base(get_viproc_reg_base(base_addr), VIPROC_REG_BASE,
                                        isp_viproc_reg_base(ISP_MAX_BE_NUM)) &&
                     ((get_viproc_reg_base(base_addr)) == (isp_viproc_reg_base(isp_get_viproc_id(base_addr)))));
}

static inline hi_bool vreg_check_isp_vreg_base_ex(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base_ex(get_isp_vreg_base(base_addr), ISP_VREG_BASE,
                                          isp_vir_reg_base(ISP_MAX_PIPE_NUM));
}

static inline hi_bool vreg_check_isp_vreg_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(get_isp_vreg_base(base_addr), ISP_VREG_BASE, isp_vir_reg_base(ISP_MAX_PIPE_NUM));
}

static inline hi_bool vreg_check_ae_vreg_base_ex(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base_ex(get_3a_vreg_base(base_addr), AE_VREG_BASE, ae_lib_vreg_base(ALG_LIB_MAX_NUM));
}

static inline hi_bool vreg_check_ae_vreg_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(get_3a_vreg_base(base_addr), AE_VREG_BASE, ae_lib_vreg_base(ALG_LIB_MAX_NUM));
}

static inline hi_bool vreg_check_awb_vreg_base_ex(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base_ex(get_3a_vreg_base(base_addr), AWB_VREG_BASE,
                                          awb_lib_vreg_base(ALG_LIB_MAX_NUM));
}

static inline hi_bool vreg_check_awb_vreg_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(get_3a_vreg_base(base_addr), AWB_VREG_BASE, awb_lib_vreg_base(ALG_LIB_MAX_NUM));
}

static inline hi_bool vreg_check_af_vreg_base(hi_u32 base_addr)
{
    return (hi_bool)isp_check_reg_base(get_3a_vreg_base(base_addr), AF_VREG_BASE, af_lib_vreg_base(ALG_LIB_MAX_NUM));
}

static inline hi_bool vreg_check_3a_vreg_base(hi_u32 base_addr)
{
    return (hi_bool)(vreg_check_ae_vreg_base(get_3a_vreg_base(base_addr)) || \
                     vreg_check_awb_vreg_base(get_3a_vreg_base(base_addr)) || \
                     vreg_check_af_vreg_base(get_3a_vreg_base(base_addr)));
}

#define vreg_munmap_virtaddr(virt_addr, size)\
    do{\
        if ((virt_addr) != HI_NULL) {\
            HI_MPI_SYS_Munmap((virt_addr), (size));\
        }\
    }while (0);

static  hi_vreg_addr *vreg_match(hi_u32 base_addr)
{
    if (vreg_check_slave_mode_time_base(base_addr)) {
        return &g_hi_vreg.slave_time_addr;
    }

    if (vreg_check_slave_base(base_addr)) {
        return &g_hi_vreg.slave_reg_addr[get_slave_id(base_addr)];
    }

    if (vreg_check_isp_fe_base(base_addr)) {
        return &g_hi_vreg.isp_fe_reg_addr[isp_get_fe_id(base_addr)];
    }

    if (vreg_check_isp_be_base(base_addr)) {
        return &g_hi_vreg.isp_be_reg_addr[isp_get_be_id(base_addr)];
    }

    if (vreg_check_viproc_base(base_addr)) {
        return &g_hi_vreg.viproc_reg_addr[isp_get_viproc_id(base_addr)];
    }

    if (vreg_check_isp_vreg_base(base_addr)) {
        return &g_hi_vreg.isp_vreg_addr[isp_get_vreg_id(base_addr)];
    }

    if (vreg_check_ae_vreg_base(base_addr)) {
        return &g_hi_vreg.ae_vreg_addr[isp_get_ae_id(base_addr)];
    }

    if (vreg_check_awb_vreg_base(base_addr)) {
        return &g_hi_vreg.awb_vreg_addr[isp_get_awb_id(base_addr)];
    }

    if (vreg_check_af_vreg_base(base_addr)) {
        return &g_hi_vreg.af_vreg_addr[isp_get_af_id(base_addr)];
    }

    return HI_NULL;
}

static inline hi_u32 vreg_base_align(hi_u32 base_addr)
{
    if (vreg_check_ae_vreg_base(base_addr)) {
        return (base_addr & 0xFFFFE000);
    } else if (vreg_check_awb_vreg_base(base_addr) || vreg_check_af_vreg_base(base_addr)) {
        return (base_addr & 0xFFFFF000);
    } else {
        return (base_addr & 0xFFFE0000);
    }
}

static inline hi_u32 vreg_size_align(hi_u32 size)
{
    return (ALG_LIB_VREG_SIZE * ((size + ALG_LIB_VREG_SIZE - 1) / ALG_LIB_VREG_SIZE));
}

hi_s32 vreg_init(hi_vi_pipe vi_pipe, hi_u32 base_addr, hi_u32 size)
{
    vreg_args vreg = { 0 };

    isp_check_pipe_return(vi_pipe);

    if (vreg_check_open(vi_pipe)) {
        return HI_FAILURE;
    }

    if (base_addr != vreg_base_align(base_addr)) {
        return HI_FAILURE;
    }

    /* malloc vreg's phyaddr in kernel */
    vreg.base_addr = (hi_u64)vreg_base_align(base_addr);
    vreg.size = (hi_u64)vreg_size_align(size);
    if (ioctl(g_vreg_fd[vi_pipe], VREG_DRV_INIT, &vreg)) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 vreg_exit(hi_vi_pipe vi_pipe, hi_u32 base_addr, hi_u32 size)
{
    hi_vreg_addr *v_reg = HI_NULL;
    vreg_args vreg;

    isp_check_pipe_return(vi_pipe);

    if (vreg_check_open(vi_pipe)) {
        return HI_FAILURE;
    }

    if (base_addr != vreg_base_align(base_addr)) {
        return HI_FAILURE;
    }

    /* check base */
    v_reg = vreg_match(vreg_base_align(base_addr));
    if (v_reg == HI_NULL) {
        return HI_FAILURE;
    }

    if (v_reg->virt_addr != HI_NULL) {
        /* munmap virtaddr */
        vreg_munmap_virtaddr(v_reg->virt_addr, vreg_size_align(size));
        v_reg->virt_addr  = HI_NULL;
        v_reg->phy_addr = 0;
    }

    /* release the buf in the kernel */
    vreg.base_addr = (hi_u64)vreg_base_align(base_addr);
    if (ioctl(g_vreg_fd[vi_pipe], VREG_DRV_EXIT, &vreg)) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 vreg_release_all(hi_vi_pipe vi_pipe)
{
    vreg_args vreg = { 0 };

    isp_check_pipe_return(vi_pipe);

    if (vreg_check_open(vi_pipe)) {
        return HI_FAILURE;
    }

    vreg.base_addr = ISP_VREG_BASE;
    vreg.size      = ISP_VREG_SIZE;

    /* release all buf in the kernel */
    if (ioctl(g_vreg_fd[vi_pipe], VREG_DRV_RELEASE_ALL, &vreg)) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void *vreg_get_virt_addr_base(hi_u32 base_addr)
{
    hi_u32 base, size;
    hi_vreg_addr *v_reg = HI_NULL;
    vreg_args vreg;

    /* check base */
    v_reg = vreg_match(base_addr);
    if (v_reg == HI_NULL) {
        return v_reg;
    }

    if (v_reg->virt_addr != HI_NULL) {
        return v_reg->virt_addr;
    }

    /* get phyaddr first */
    if (vreg_check_slave_mode_time_base(base_addr)) {
        v_reg->phy_addr = SLAVE_MODE_REG_BASE;
        size = SLAVE_MODE_TIME_ALIGN;
        v_reg->virt_addr = vreg_io_mmap(v_reg->phy_addr, size);
    } else if (vreg_check_slave_base(base_addr)) {
        v_reg->phy_addr = cap_slave_reg_base(get_slave_id(base_addr));
        size = SLAVE_MODE_ALIGN;
        v_reg->virt_addr = vreg_io_mmap(v_reg->phy_addr, size);
    } else if (vreg_check_isp_fe_base(base_addr)) {
        v_reg->phy_addr = isp_fe_reg_base(isp_get_fe_id(base_addr));
        size = FE_REG_SIZE_ALIGN;
        v_reg->virt_addr = vreg_io_mmap(v_reg->phy_addr, size);
    } else if (vreg_check_isp_be_base(base_addr)) {
        v_reg->phy_addr = isp_be_reg_base(isp_get_be_id(base_addr));
        size = BE_REG_SIZE_ALIGN;
        v_reg->virt_addr = vreg_io_mmap(v_reg->phy_addr, size);
    } else if (vreg_check_viproc_base(base_addr)) {
        v_reg->phy_addr = isp_viproc_reg_base(isp_get_viproc_id(base_addr));
        size = VIPROC_REG_SIZE;
        v_reg->virt_addr = vreg_io_mmap(v_reg->phy_addr, size);
    } else {
        if (vreg_check0_open()) {
            return HI_NULL;
        }

        if (vreg_check_isp_vreg_base(base_addr)) {
            base = get_isp_vreg_base(base_addr);
            size = ISP_VREG_SIZE;
        } else if (vreg_check_ae_vreg_base(base_addr)) {
            base = get_ae_id_vreg_base(base_addr);
            size = AE_VREG_SIZE;
        } else if (vreg_check_awb_vreg_base(base_addr)) {
            base = get_awb_id_vreg_base(base_addr);
            size = ALG_LIB_VREG_SIZE;
        } else {
            base = get_af_id_vreg_base(base_addr);
            size = ALG_LIB_VREG_SIZE;
        }

        vreg.base_addr = (hi_u64)base;
        if (ioctl(g_vreg_fd[0], VREG_DRV_GETADDR, &vreg)) {
            return HI_NULL;
        }
        v_reg->phy_addr = vreg.phy_addr;

        /* Mmap virtaddr */
        v_reg->virt_addr = HI_MPI_SYS_Mmap(v_reg->phy_addr, size);
    }

    return v_reg->virt_addr;
}

hi_s32 vreg_munmap(hi_u32 base_addr, hi_u32 size)
{
    hi_vreg_addr *v_reg = HI_NULL;

    if (base_addr != vreg_base_align(base_addr)) {
        return HI_FAILURE;
    }

    /* check base */
    v_reg = vreg_match(vreg_base_align(base_addr));
    if (v_reg == HI_NULL) {
        return HI_FAILURE;
    }

    if (v_reg->virt_addr != HI_NULL) {
        /* munmap virtaddr */
        vreg_munmap_virtaddr(v_reg->virt_addr, vreg_size_align(size));
        v_reg->virt_addr = HI_NULL;
    }

    return HI_SUCCESS;
}

static hi_u32 get_slave_addr_offset(hi_u32 base_addr)
{
    hi_u8 addr_id;

    addr_id = get_slave_id(base_addr);

    switch (addr_id) {
        case 0:                                                              /* case index 0 */
            return ((base_addr - cap_slave_reg_base(addr_id)) & 0xFF);
        case 1:                                                              /* case index 1 */
            return ((base_addr - cap_slave_reg_base(addr_id)) & 0xFF);
        default:
            return 0;
    }
}

#define hivreg_write_reg32(b, addr) *(addr) = b
#define hivreg_read_reg32(addr) *(addr)

hi_void *vreg_get_virt_addr(hi_u32 base_addr)
{
    hi_void *virt_addr = HI_NULL;

    virt_addr = vreg_get_virt_addr_base(base_addr);
    if (virt_addr == HI_NULL) {
        return virt_addr;
    }

    if ((vreg_check_isp_fe_base(base_addr)) ||
        (vreg_check_isp_be_base(base_addr))) {
        return ((hi_u8 *)virt_addr + (base_addr & 0x3FFFF));
    } else if ((vreg_check_isp_vreg_base(base_addr))) {
        return ((hi_u8 *)virt_addr + (base_addr & 0x1FFFF));
    } else if (vreg_check_ae_vreg_base(base_addr)) {
        return ((hi_u8 *)virt_addr + (base_addr & 0x1FFF));
    } else if (vreg_check_awb_vreg_base(base_addr)) {
        return ((hi_u8 *)virt_addr + (base_addr & 0xFFF));
    } else if (vreg_check_af_vreg_base(base_addr)) {
        return ((hi_u8 *)virt_addr + (base_addr & 0xFFF));
    } else if (vreg_check_slave_mode_time_base(base_addr)) {
        return ((hi_u8 *)virt_addr + (base_addr - SLAVE_MODE_REG_BASE));
    } else if (vreg_check_slave_base(base_addr)) {
        return ((hi_u8 *)virt_addr + get_slave_addr_offset(base_addr));
    } else if (vreg_check_viproc_base(base_addr)) {
        return ((hi_u8 *)virt_addr + (base_addr & 0xFFF));
    } else {
        return virt_addr;
    }
}

hi_void *vreg_get_virt_addr_ex(hi_u32 base_addr)
{
    hi_void *virt_addr = HI_NULL;

    virt_addr = vreg_get_virt_addr_base(base_addr);
    if (virt_addr == HI_NULL) {
        return virt_addr;
    }

    if ((vreg_check_isp_fe_base(base_addr)) ||
        (vreg_check_isp_be_base(base_addr))) {
        return HI_NULL;
    } else if ((vreg_check_isp_vreg_base_ex(base_addr))) {
        return ((hi_u8 *)virt_addr + (base_addr & 0x1FFFF));
    } else if ((vreg_check_ae_vreg_base_ex(base_addr))) {
        return ((hi_u8 *)virt_addr + (base_addr & 0x1FFF));
    } else if ((vreg_check_awb_vreg_base_ex(base_addr))) {
        return ((hi_u8 *)virt_addr + (base_addr & 0xFFF));
    } else if (vreg_check_slave_base(base_addr)) {
        return HI_NULL;
    } else if (vreg_check_viproc_base(base_addr)) {
        return HI_NULL;
    } else {
        return HI_NULL;
    }
}

hi_u32 io_read32(hi_u32 addr)
{
    hi_void *virt_addr = HI_NULL;
    hi_u32  *addr_tmp  = HI_NULL;
    hi_u32  value;

    virt_addr = vreg_get_virt_addr(addr);
    if (virt_addr == HI_NULL) {
        return 0;
    }

    addr_tmp = (hi_u32 *)(hi_uintptr_t)((hi_uintptr_t)virt_addr & IO_MASK_BITXX);
    value = hivreg_read_reg32(addr_tmp);

    return value;
}

hi_s32 io_read32_ex(hi_u32 addr, hi_u32 *value)
{
    hi_void *virt_addr = HI_NULL;
    hi_u32  *addr_tmp = HI_NULL;
    hi_u32  value_tmp;
    isp_check_pointer_return(value);
    virt_addr = vreg_get_virt_addr(addr);
    if (virt_addr == HI_NULL) {
        return HI_FAILURE;
    }
    addr_tmp  = (hi_u32 *)(hi_uintptr_t)((hi_uintptr_t)virt_addr & IO_MASK_BITXX);
    value_tmp = hivreg_read_reg32(addr_tmp);
    *value = value_tmp;
    return HI_SUCCESS;
}

hi_s32 io_write32(hi_u32 addr, hi_u32 value)
{
    hi_void *virt_addr = HI_NULL;
    hi_u32  *addr_tmp = HI_NULL;

    virt_addr = vreg_get_virt_addr(addr);
    if (virt_addr == HI_NULL) {
        return 0;
    }

    addr_tmp = (hi_u32 *)(hi_uintptr_t)((hi_uintptr_t)virt_addr & IO_MASK_BITXX);
    hivreg_write_reg32(value, addr_tmp);

    return HI_SUCCESS;
}

hi_s32 io_write32_ex(hi_u32 addr, hi_u32 value)
{
    hi_void *virt_addr = HI_NULL;
    hi_u32  *addr_tmp = HI_NULL;

    virt_addr = vreg_get_virt_addr_ex(addr);
    if (virt_addr == HI_NULL) {
        return HI_FAILURE;
    }

    addr_tmp = (hi_u32 *)(hi_uintptr_t)((hi_uintptr_t)virt_addr & IO_MASK_BITXX);
    hivreg_write_reg32(value, addr_tmp);

    return HI_SUCCESS;
}

hi_u8 io_read8(hi_u32 addr)
{
    hi_u32 value;

    value = io_read32(addr & IO_MASK_BIT32);

    return (value >> get_shift_bit(addr)) & 0xFF;
}

hi_s32 io_write8(hi_u32 addr, hi_u32 value)
{
    hi_u32  current;
    hi_u32  current_mask;
    hi_u32  value_tmp;
    hi_s32  ret;

    current_mask = ~(0xFF << get_shift_bit(addr));
    value_tmp    = (value & 0xFF) << get_shift_bit(addr);
    current = io_read32(addr & IO_MASK_BIT32);
    ret = io_write32((addr & IO_MASK_BIT32), value_tmp | (current & current_mask));

    return ret;
}

hi_u16 io_read16(hi_u32 addr)
{
    hi_u32  value;

    value = io_read32(addr & IO_MASK_BIT32);

    return (value >> get_shift_bit(addr & 0xFFFFFFFE)) & 0xFFFF;
}

hi_s32 io_write16(hi_u32 addr, hi_u32 value)
{
    hi_u32  current;
    hi_u32  current_mask;
    hi_u32  value_tmp;
    hi_s32  ret;

    current_mask = ~(0xFFFF << get_shift_bit(addr & 0xFFFFFFFE));
    value_tmp    = (value & 0xFFFF) << get_shift_bit(addr & 0xFFFFFFFE);
    current = io_read32(addr & IO_MASK_BIT32);
    ret = io_write32((addr & IO_MASK_BIT32), value_tmp | (current & current_mask));

    return ret;
}

