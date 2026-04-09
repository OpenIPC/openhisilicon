/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_vreg.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __HI_VREG_H__
#define __HI_VREG_H__

#include "hi_type.h"
#include "hi_comm_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Vreg is a blok of memory alloced to simulate the regs.
 * We try to differentiate vregs by baseaddr.  Each vreg
 * block's size should at least 4k bytes. */
/* ISP FE */
/* -----------------------------------*
 * | 0x1132_0000 |  0x1134_0000 | ... |*
 * |-------------|--------------|-----|*
 * |   FE0_REG   |    FE1_REG   | ... |*
 * ------------------------------------ */
/* ISP VREG */
/* -------------------------------*
 * | 0x10_0000 |  0x12_0000 | ... |*
 * |-----------|------------|-----|*
 * | ISP0_VREG |  ISP1_VREG | ... |*
 * -------------------------------- */
/* AE/AWB/AF */
/* ------------------------------------------------------------------*
 * |  0x60_0000 |  0x60_1000 | ... |  0x70_0000  | ... |  0x80_0000 |*
 * |------------|------------|-----|-------------|-----|------------|*
 * |  AWB0_VREG  |  AWB1_VREG  | ... |  AE0_VREG  | ... |  AF0_VREG  |*
 * ------------------------------------------------------------------ */
#define CHECK_ISP_REG_MASK      0xFFFF0000
#define CHECK_ISP_VREG_MASK     0xFFFE0000
#define CHECK_3A_VREG_MASK      0xFFFF0000
#define CHECK_AE_ID_VREG_MASK   0xFFFFE000
#define CHECK_AWB_ID_VREG_MASK  0xFFFFF000
#define CHECK_AF_ID_VREG_MASK   0xFFFFF000

#define VREG_SIZE_ALIGN         0x1000
#define VI_CAP_REG_SIZE         0x10000
#define VI_ISP_FE_REG_SIZE      0x10000
#define VI_ISP_BE_REG_SIZE      0x10000

#define CAP_SLAVE_MAX_NUM       1
#define SLAVE_MODE_ALIGN        0x40
#define SLAVE_REG_BASE          0x11300100
#define vicap_slave_vstime(dev) (0x108 + (dev) * SLAVE_MODE_ALIGN)
#define cap_slave_reg_base(id)  (SLAVE_REG_BASE + SLAVE_MODE_ALIGN * (id))

#define SLAVE_MODE_TIME_NUM     1
#define SLAVE_MODE_TIME_ALIGN   0x10
#define SLAVE_MODE_REG_BASE     0x113001F8

#define CAP_REG_SIZE_ALIGN      0x10000
#define CAP_REG_BASE            0x11300000

#define VICAP_CH_REG_SIZE_ALIGN 0x1000
#define VICAP_CH_REG_BASE       0x11310000
#define isp_vicap_ch_reg_base(id) (VICAP_CH_REG_BASE + VICAP_CH_REG_SIZE_ALIGN * (id))

#define FE_REG_SIZE_ALIGN       0x20000
#define FE_REG_BASE             0x11320000
#define isp_fe_reg_base(id)     (FE_REG_BASE + FE_REG_SIZE_ALIGN * (id))

#define HDR_REG_SIZE_ALIGN       0x20000
#define HDR_REG_BASE             0x04b20000
#define isp_hdr_reg_base(id)     (HDR_REG_BASE + HDR_REG_SIZE_ALIGN * (id))

#define VIPROC_OFFLINE_OFFSET   0xb230
#define VIPROC_REG_SIZE         0x1000
#define VIPROC_REG_SIZE_ALIGN   0x40000
#define VIPROC_REG_BASE         0x11000000
#define isp_viproc_reg_base(id) (VIPROC_REG_BASE + VIPROC_REG_SIZE_ALIGN * (id))

#define BE_REG_SIZE_ALIGN       0x40000
#define BE_REG_BASE             0x11020000
#define BE_VI_ISP_OFFSET        0x10000
#define isp_be_reg_base(id)     (BE_REG_BASE + BE_REG_SIZE_ALIGN * (id))

#define ISP_VREG_SIZE           (VREG_SIZE_ALIGN << 5) /* 0x20000, 128K */
#define ISP_VREG_BASE           0x100000
#define isp_vir_reg_base(id)    (ISP_VREG_BASE + ISP_VREG_SIZE * (id))

#define ALG_LIB_MAX_NUM         16

#define AE_VREG_BASE            0x700000
#define AWB_VREG_BASE           0x600000
#define AF_VREG_BASE            0x800000
#define AE_VREG_SIZE            (VREG_SIZE_ALIGN << 1) /* 0x2000, 2K */
#define ae_lib_vreg_base(id)    (AE_VREG_BASE + AE_VREG_SIZE * (id))
#define awb_lib_vreg_base(id)   (AWB_VREG_BASE + VREG_SIZE_ALIGN * (id))
#define af_lib_vreg_base(id)    (AF_VREG_BASE + VREG_SIZE_ALIGN * (id))
#define ALG_LIB_VREG_SIZE       (VREG_SIZE_ALIGN)
#define MAX_ALG_LIB_VREG_NUM    (1 << 4)

#define HISI_AE_LIB_EXTREG_ID_0 0
#define HISI_AE_LIB_EXTREG_ID_1 1

#define HISI_AWB_LIB_EXTREG_ID_0 0
#define HISI_AWB_LIB_EXTREG_ID_1 1

#define ISP_AE_HIST_ADDR_OFFSET       0x0
#define ISP_AE_AVER_R_GR_ADDR_OFFSET  0x1000
#define ISP_AE_AVER_GB_B_ADDR_OFFSET  0x1400
#define ISP_AF_STAT_ADDR_OFFSET       0x1800
#define ISP_AWB_STAT_ADDR_OFFSET      0x2800
#define ISP_LA_AVER_ADDR_OFFSET       0x4800
#define ISP_DEHAZE_MIN_ADDR_OFFSET    0x4c00
#define ISP_DEHAZE_MAX_ADDR_OFFSET    0x5400
#define ISP_LDCI_LPF_MAP_ADDR_OFFSET  0x6400
#define ISP_FLICK_GMEAN_ADDR_OFFSET   0x7400

#define ISP_CLUT_LUT_WSTT_OFFSET      0x0
#define ISP_DEHAZE_LUT_WSTT_OFFSET    0x5700
#define ISP_BNR_RLSC_LUT_WSTT_OFFSET  0x6b00
#define ISP_GAMMA_LUT_WSTT_OFFSET     0x7000
#define ISP_SHARPEN_LUT_WSTT_OFFSET   0x7500
#define ISP_DPC_LUT_WSTT_OFFSET       0x7900
#define ISP_LSC_LUT_WSTT_OFFSET       0x9900
#define ISP_CA_LUT_WSTT_OFFSET        0xde00
#define ISP_LDCI_LUT_WSTT_OFFSET      0xe000
#define ISP_PREGAMMA_LUT_WSTT_OFFSET  0xe500

hi_s32 vreg_init(hi_vi_pipe vi_pipe, hi_u32 base_addr, hi_u32 size);
hi_s32 vreg_exit(hi_vi_pipe vi_pipe, hi_u32 base_addr, hi_u32 size);
hi_s32 vreg_release_all(hi_vi_pipe vi_pipe);
hi_void *vreg_get_virt_addr_base(hi_u32 base_addr);
hi_s32 vreg_munmap(hi_u32 base_addr, hi_u32 size);
hi_u32 io_read32(hi_u32 addr);
hi_s32 io_write32(hi_u32 addr, hi_u32 value);
hi_s32 io_write32_ex(hi_u32 addr, hi_u32 value);
hi_s32 io_read32_ex(hi_u32 addr, hi_u32 *value);
hi_u16 io_read16(hi_u32 addr);
hi_s32 io_write16(hi_u32 addr, hi_u32 value);
hi_u8  io_read8(hi_u32 addr);
hi_s32 io_write8(hi_u32 addr, hi_u32 value);

#define get_isp_reg_base(base_addr)   ((base_addr) & CHECK_ISP_REG_MASK)
#define get_viproc_reg_base(base_addr)   ((base_addr) & CHECK_ISP_REG_MASK)

#define get_isp_vreg_base(base_addr)  ((base_addr) & CHECK_ISP_VREG_MASK)
#define get_3a_vreg_base(base_addr)   ((base_addr) & CHECK_3A_VREG_MASK)
#define isp_check_reg_base(base_addr, min, max) (((base_addr) >= (min)) && ((base_addr) <= (max)))
#define isp_check_reg_base_ex(base_addr, min, max) (((base_addr) >= (min)) && ((base_addr) < (max)))

#define get_ae_id_vreg_base(base_addr)   ((base_addr) & CHECK_AE_ID_VREG_MASK)
#define get_awb_id_vreg_base(base_addr)  ((base_addr) & CHECK_AWB_ID_VREG_MASK)
#define get_af_id_vreg_base(base_addr)   ((base_addr) & CHECK_AF_ID_VREG_MASK)

#define get_slave_id(base_addr)       (((base_addr) - SLAVE_REG_BASE) >> 6)
#define isp_get_fe_id(base_addr)      ((get_isp_reg_base(base_addr) - FE_REG_BASE) >> 17)
#define isp_get_be_id(base_addr)      ((get_isp_reg_base(base_addr) - BE_REG_BASE) >> 18)
#define isp_get_viproc_id(base_addr)  ((get_viproc_reg_base(base_addr) - VIPROC_REG_BASE) >> 18)

#define isp_get_hdr_id(base_addr)     ((get_isp_reg_base(base_addr) - HDR_REG_BASE)>> 18)

/* Addr : BaseAddr + Offset * (id) */
#define isp_get_vreg_id(base_addr)    ((((base_addr) - ISP_VREG_BASE) >> 17) & 0xF)
#define isp_get_ae_id(base_addr)      ((((base_addr) - AE_VREG_BASE) >> 13) & 0xF)
#define isp_get_awb_id(base_addr)     ((((base_addr) - AWB_VREG_BASE) >> 12) & 0xF)
#define isp_get_af_id(base_addr)      ((((base_addr) - AF_VREG_BASE) >> 12) & 0xF)

#define get_shift_bit(addr)          (((addr) & 0x3) << 3)
#define IO_MASK_BIT32                   0xFFFFFFFC
#define IO_MASK_BITXX                   (~((hi_ulong)0x3))

/* Dynamic bus access functions, 4 byte align access */
#define __io_calc_address_dynamic(base)    (base)
#define iord_32direct(base)             io_read32(__io_calc_address_dynamic(base))
#define iord_16direct(base)             io_read16(__io_calc_address_dynamic(base))
#define iord_8direct(base)              io_read8(__io_calc_address_dynamic(base))

#define iowr_32direct(base, data)       io_write32(__io_calc_address_dynamic(base), (data))
#define iowr_16direct(base, data)       io_write16(__io_calc_address_dynamic(base), (data))
#define iowr_8direct(base, data)        io_write8(__io_calc_address_dynamic(base), (data))

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
