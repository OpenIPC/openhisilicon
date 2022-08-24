/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_VREG_H__
#define __ISP_VREG_H__

#include "type.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Vreg is a blok of memory alloced to simulate the regs.
 * We try to differentiate vregs by baseaddr.  Each vreg
 * block's size should at least 4k bytes.
 */
/* ISP FE */
/* -----------------------------------*
 * | 0x1102_0000 |  0x1104_0000 | ... |*
 * |-------------|--------------|-----|*
 * |   FE0_REG   |    FE1_REG   | ... |*
 * ------------------------------------ */

/* ISP VREG */
/* -------------------------------*
 * | 0x10_0000 |  0x11_0000 | ... |*
 * |-----------|------------|-----|*
 * | ISP0_VREG |  ISP1_VREG | ... |*
 * -------------------------------- */

/* AE/AWB/AF */
/* ------------------------------------------------------------------*
 * |  0x20_0000 |  0x20_1000 | ... |  0x30_0000  | ... |  0x40_0000 |*
 * |------------|------------|-----|-------------|-----|------------|*
 * |  AE0_VREG  |  AE1_VREG  | ... |  AWB0_VREG  | ... |  AF0_VREG  |*
 * ------------------------------------------------------------------ */
#define ISP_OFFSETOF(TYPE, MEMBER) ((GK_U32)(&((TYPE *)0)->MEMBER))

#define CHECK_ISP_REG_MASK      0xFFFF0000

#define CHECK_ISP_VREG_MASK     0xFFFF0000
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
#define SLAVE_REG_BASE          0x11000100

#define VICAP_SLAVE_VSTIME(dev) (0x108 + (dev) * SLAVE_MODE_ALIGN)
#define CAP_SLAVE_REG_BASE(id)  (SLAVE_REG_BASE + SLAVE_MODE_ALIGN * (id))

#define SLAVE_MODE_TIME_NUM     1
#define SLAVE_MODE_TIME_ALIGN   0x10
#define SLAVE_MODE_REG_BASE     0x110001f8

#define CAP_REG_SIZE_ALIGN      0x10000
#define CAP_REG_BASE            0x11000000

#define VICAP_CH_REG_SIZE_ALIGN 0x1000
#define VICAP_CH_REG_BASE       0x11010000
#define ISP_VICAP_CH_REG_BASE(id) (VICAP_CH_REG_BASE + VICAP_CH_REG_SIZE_ALIGN * (id))

#define FE_REG_SIZE_ALIGN       0x20000
#define FE_REG_BASE             0x11020000
#define ISP_FE_REG_BASE(id)     (FE_REG_BASE + FE_REG_SIZE_ALIGN * (id))

#define HDR_REG_SIZE_ALIGN       0x20000
#define HDR_REG_BASE             0x04b20000
#define ISP_HDR_REG_BASE(id)     (HDR_REG_BASE + HDR_REG_SIZE_ALIGN * (id))

#define VIPROC_OFFLINE_OFFSET   (ISP_OFFSETOF(S_ISPBE_REGS_TYPE,stViProcReg))
#define VIPROC_REG_SIZE         0x1000
#define VIPROC_REG_SIZE_ALIGN   0x40000
#define VIPROC_REG_BASE         0x11200000
#define ISP_VIPROC_REG_BASE(id) (VIPROC_REG_BASE + VIPROC_REG_SIZE_ALIGN * (id))


#define BE_REG_SIZE_ALIGN       0x40000
#define BE_REG_BASE             0x11220000
#define BE_VI_ISP_OFFSET        0x10000
#define ISP_BE_REG_BASE(id)     (BE_REG_BASE + BE_REG_SIZE_ALIGN * (id))

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
#define ISP_VREG_SIZE           (VREG_SIZE_ALIGN << 5) /* 0x20000 */
#else
#define ISP_VREG_SIZE           (VREG_SIZE_ALIGN << 4) /* 0x10000 */
#endif
#define ISP_VREG_BASE           0x100000
#define ISP_VIR_REG_BASE(id)    (ISP_VREG_BASE + ISP_VREG_SIZE * (id))

#define ALG_LIB_MAX_NUM         16

#define AE_VREG_BASE            0x700000
#define AWB_VREG_BASE           0x600000
#define AF_VREG_BASE            0x800000
#define AE_VREG_SIZE            (VREG_SIZE_ALIGN << 1) /* 0x2000, 2K */
#define AE_LIB_VREG_BASE(id)    (AE_VREG_BASE + AE_VREG_SIZE * (id))
#define AWB_LIB_VREG_BASE(id)   (AWB_VREG_BASE + VREG_SIZE_ALIGN * (id))
#define AF_LIB_VREG_BASE(id)    (AF_VREG_BASE + VREG_SIZE_ALIGN * (id))
#define ALG_LIB_VREG_SIZE       (VREG_SIZE_ALIGN)
#define MAX_ALG_LIB_VREG_NUM    (1 << 4)

#define ISP_AE_LIB_EXTREG_ID_0 0
#define ISP_AE_LIB_EXTREG_ID_1 1

#define ISP_AE_LIB_EXTREG_ID_0 0
#define ISP_AE_LIB_EXTREG_ID_1 1

#define ISP_AE_HIST_ADDR_OFFSET       (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_AE_HIST))
#define ISP_AE_AVER_R_GR_ADDR_OFFSET  (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_AE_AVER_R_GR))
#define ISP_AE_AVER_GB_B_ADDR_OFFSET  (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_AE_AVER_GB_B))
#define ISP_AF_STAT_ADDR_OFFSET       (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_AF_STAT))
#define ISP_AWB_STAT_ADDR_OFFSET      (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_AWB_STAT))
#define ISP_LA_AVER_ADDR_OFFSET       (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_LA_AVER))
#define ISP_DEHAZE_MIN_ADDR_OFFSET    (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_DEHAZE_MINSTAT))
#define ISP_DEHAZE_MAX_ADDR_OFFSET    (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_DEHAZE_MAXSTAT))
#define ISP_LDCI_LPF_MAP_ADDR_OFFSET  (ISP_OFFSETOF(S_ISP_ONLINE_STT_REGS_TYPE,ISP_LDCI_LPF_MAP_RSTT))

#define ISP_DEHAZE_LUT_WSTT_OFFSET    (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_DEHAZE_LUT_WSTT))
#define ISP_GAMMA_LUT_WSTT_OFFSET     (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_GAMMA_LUT_WSTT))
#define ISP_SHARPEN_LUT_WSTT_OFFSET   (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_SHARPEN_LUT_WSTT))
#define ISP_LSC_LUT_WSTT_OFFSET       (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_MLSC_LUT_WSTT))
#define ISP_CA_LUT_WSTT_OFFSET        (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_CA_LUT_WSTT))
#define ISP_LDCI_LUT_WSTT_OFFSET      (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_LDCI_LUT_WSTT))
#define ISP_BNR_LUT_WSTT_OFFSET       (ISP_OFFSETOF(S_ISP_LUT_WSTT_TYPE,ISP_BNR_LUT_WSTT))

GK_S32 VReg_Init(VI_PIPE ViPipe, GK_U32 u32BaseAddr, GK_U32 u32Size);
GK_S32 VReg_Exit(VI_PIPE ViPipe, GK_U32 u32BaseAddr, GK_U32 u32Size);
GK_S32 VReg_ReleaseAll(VI_PIPE ViPipe);
GK_VOID *VReg_GetVirtAddr(GK_U32 u32BaseAddr);

GK_VOID *VReg_GetVirtAddrBase(GK_U32 u32BaseAddr);
GK_S32 VReg_Munmap(GK_U32 u32BaseAddr, GK_U32 u32Size);
GK_U32 IO_READ32(GK_U32 u32Addr);
GK_S32 IO_WRITE32(GK_U32 u32Addr, GK_U32 u32Value);
GK_S32 IO_WRITE32_Ex(GK_U32 u32Addr, GK_U32 u32Value);
GK_S32 IO_READ32_Ex(GK_U32 u32Addr, GK_U32 *pu32Value);
GK_U16 IO_READ16(GK_U32 u32Addr);
GK_S32 IO_WRITE16(GK_U32 u32Addr, GK_U32 u32Value);
GK_U8  IO_READ8(GK_U32 u32Addr);
GK_S32 IO_WRITE8(GK_U32 u32Addr, GK_U32 u32Value);


#define GET_ISP_REG_BASE(u32BaseAddr)   (u32BaseAddr & CHECK_ISP_REG_MASK)
#define GET_VIPROC_REG_BASE(u32BaseAddr)   (u32BaseAddr & CHECK_ISP_REG_MASK)

#define GET_ISP_VREG_BASE(u32BaseAddr)  (u32BaseAddr & CHECK_ISP_VREG_MASK)
#define GET_3A_VREG_BASE(u32BaseAddr)   (u32BaseAddr & CHECK_3A_VREG_MASK)
#define ISP_CHECK_REG_BASE(u32BaseAddr, u32Min, u32Max) (((u32BaseAddr) >= (u32Min)) && ((u32BaseAddr) <= (u32Max)))

#define GET_AE_ID_VREG_BASE(u32BaseAddr)   (u32BaseAddr & CHECK_AE_ID_VREG_MASK)
#define GET_AWB_ID_VREG_BASE(u32BaseAddr)  (u32BaseAddr & CHECK_AWB_ID_VREG_MASK)
#define GET_AF_ID_VREG_BASE(u32BaseAddr)   (u32BaseAddr & CHECK_AF_ID_VREG_MASK)

#define GET_SLAVE_ID(u32BaseAddr)       ((u32BaseAddr - SLAVE_REG_BASE) >> 6)
#define ISP_GET_FE_ID(u32BaseAddr)      ((GET_ISP_REG_BASE(u32BaseAddr) - FE_REG_BASE) >> 17)
#define ISP_GET_BE_ID(u32BaseAddr)      ((GET_ISP_REG_BASE(u32BaseAddr) - BE_REG_BASE) >> 18)
#define ISP_GET_VIPROC_ID(u32BaseAddr)  ((GET_VIPROC_REG_BASE(u32BaseAddr) - VIPROC_REG_BASE) >> 18)

#define ISP_GET_HDR_ID(u32BaseAddr)     ((GET_ISP_REG_BASE(u32BaseAddr) - HDR_REG_BASE)>> 18)

/* Addr = BaseAddr + Offset * (id) */
#define ISP_GET_VREG_ID(u32BaseAddr)    (((u32BaseAddr - ISP_VREG_BASE) >> 16) & 0xF) // 64KB
#define ISP_GET_AE_ID(u32BaseAddr)      (((u32BaseAddr - AE_VREG_BASE ) >> 13) & 0xF)
#define ISP_GET_AWB_ID(u32BaseAddr)     (((u32BaseAddr - AWB_VREG_BASE) >> 12) & 0xF)
#define ISP_GET_AF_ID(u32BaseAddr)      (((u32BaseAddr - AF_VREG_BASE ) >> 12) & 0xF)

#define GET_SHIFT_BIT(u32Addr)          (((u32Addr) & 0x3) << 3)
#define IO_MASK_BIT32                   (0xFFFFFFFC)
#define IO_MASK_BITXX                   (~((GK_UL)0x3))

/* Dynamic bus access functions, 4 byte align access */
#define __IO_CALC_ADDRESS_DYNAMIC(BASE)    (BASE)
#define IORD_32DIRECT(BASE)             IO_READ32(__IO_CALC_ADDRESS_DYNAMIC(BASE))
#define IORD_16DIRECT(BASE)             IO_READ16(__IO_CALC_ADDRESS_DYNAMIC(BASE))
#define IORD_8DIRECT(BASE)              IO_READ8(__IO_CALC_ADDRESS_DYNAMIC(BASE))

#define IOWR_32DIRECT(BASE, DATA)       IO_WRITE32(__IO_CALC_ADDRESS_DYNAMIC(BASE), (DATA))
#define IOWR_16DIRECT(BASE, DATA)       IO_WRITE16(__IO_CALC_ADDRESS_DYNAMIC(BASE), (DATA))
#define IOWR_8DIRECT(BASE, DATA)        IO_WRITE8(__IO_CALC_ADDRESS_DYNAMIC(BASE), (DATA))

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
