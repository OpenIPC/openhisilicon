/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_vreg.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/09
  Description   : 
  History       :
  1.Date        : 2013/01/09
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifdef __HuaweiLite__
#undef __KERNEL__
#endif
#ifdef __KERNEL__
#include "hi_osal.h"
//#include <asm/io.h>
//#include <mach/io.h>/* for IO_ADDRESS */
#else
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "mpi_isp.h"
#include "mpi_sys.h"
#endif

#include "hi_vreg.h"
#include "hi_comm_isp.h"
#include "hi_drv_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __KERNEL__
extern void  *reg_vi_isp_base_va[ISP_MAX_DEV_NUM];
#else
typedef struct hiHI_VREG_ADDR_S
{
    HI_U32  u32PhyAddr;
    HI_U32  u32VirtAddr;
} HI_VREG_ADDR_S;

typedef struct hiHI_VREG_S
{
    HI_VREG_ADDR_S stViIspRegAddr[ISP_MAX_DEV_NUM];
    HI_VREG_ADDR_S stIspVRegAddr[ISP_MAX_DEV_NUM];
    HI_VREG_ADDR_S astAeAddr[MAX_ALG_LIB_VREG_NUM];
    HI_VREG_ADDR_S astAwbAddr[MAX_ALG_LIB_VREG_NUM];
    HI_VREG_ADDR_S astAfAddr[MAX_ALG_LIB_VREG_NUM];    
} HI_VREG_S;

static HI_VREG_S g_stHiVreg = {{{0}}};

HI_S32 g_s32VregFd = -1;
static inline HI_S32 VREG_CHECK_OPEN(HI_VOID)
{
    if (g_s32VregFd < 0)
    {
        g_s32VregFd = open("/dev/isp_dev", O_RDONLY);
        if (g_s32VregFd < 0)
        {
            perror("open isp device error!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

#endif

#ifdef __KERNEL__
extern VREG_ARGS_S *VREG_DRV_Search(HI_U32 u32BaseAddr);

HI_U32 VReg_GetVirtAddr(HI_U32 u32BaseAddr)
{
    HI_U32 u32TmpBaseAddr = (u32BaseAddr & 0xffff0000);
    HI_U32 u32Base;
    VREG_ARGS_S *pstVreg;
    
    if ((u32TmpBaseAddr >= VI_ISP0_REG_BASE) && (u32TmpBaseAddr < (VI_ISP0_REG_BASE + VI_ISP_REG_SIZE)))
    {
        return (HI_U32)(reg_vi_isp_base_va[0]);
    }
    else if ((u32TmpBaseAddr >= VI_ISP1_REG_BASE) && (u32TmpBaseAddr < (VI_ISP1_REG_BASE + VI_ISP_REG_SIZE)))
    {
        return (HI_U32)(reg_vi_isp_base_va[1]);
    }
    else
    {
        u32Base = (ISP0_VREG_BASE == u32TmpBaseAddr) ? ISP0_VREG_BASE :
                  ((ISP1_VREG_BASE == u32TmpBaseAddr) ? ISP1_VREG_BASE : (u32BaseAddr & 0xfffff000));

        pstVreg = VREG_DRV_Search(u32Base);
        if (!pstVreg)
        {
            osal_printk("DBG[%s][%d] NULL PTR!\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }
    return pstVreg->u32VirtAddr;
}
#else
#define VREG_MUNMAP_VIRTADDR(u32VirtAddr, u32Size)\
do{\
    if (0 != (u32VirtAddr))\
    {\
        HI_MPI_SYS_Munmap((HI_VOID *)(u32VirtAddr), (u32Size));\
    }\
}while(0);

static inline HI_VREG_ADDR_S *VReg_Match(HI_U32 u32BaseAddr)
{
    HI_U32 u32TmpBaseAddr = (u32BaseAddr & 0xffff0000);
    
    if ((u32TmpBaseAddr >= VI_ISP0_REG_BASE) && (u32TmpBaseAddr < (VI_ISP0_REG_BASE + VI_ISP_REG_SIZE)))
    {
        return &g_stHiVreg.stViIspRegAddr[0];
    }
    else if ((u32TmpBaseAddr >= VI_ISP1_REG_BASE) && (u32TmpBaseAddr < (VI_ISP1_REG_BASE + VI_ISP_REG_SIZE)))
    {
        return &g_stHiVreg.stViIspRegAddr[1];
    }
    
    switch (u32TmpBaseAddr)
    {
        case ISP0_VREG_BASE :
            return &g_stHiVreg.stIspVRegAddr[0];
        case ISP1_VREG_BASE :
            return &g_stHiVreg.stIspVRegAddr[1];
        case AE_VREG_BASE :
            return &g_stHiVreg.astAeAddr[(u32BaseAddr >> 12) & 0xf];
        case AWB_VREG_BASE :
            return &g_stHiVreg.astAwbAddr[(u32BaseAddr >> 12) & 0xf];
        case AF_VREG_BASE :
            return &g_stHiVreg.astAfAddr[(u32BaseAddr >> 12) & 0xf];
        default :
            return HI_NULL;
    }
    return HI_NULL;
}

static inline HI_U32 VReg_BaseAlign(HI_U32 u32BaseAddr)
{
    switch (u32BaseAddr & 0xffff0000)
    {
        case VI_ISP0_REG_BASE : 
        case VI_ISP1_REG_BASE :     
        case ISP0_VREG_BASE :
        case ISP1_VREG_BASE :
            return (u32BaseAddr & 0xffff0000);
        case AE_VREG_BASE :
        case AWB_VREG_BASE :
        case AF_VREG_BASE :
            return (u32BaseAddr & 0xfffff000);
        default :
            return (u32BaseAddr & 0xffff0000);
    }
}

static inline HI_U32 VReg_SizeAlign(HI_U32 u32Size)
{
    return (ALG_LIB_VREG_SIZE * ((u32Size + ALG_LIB_VREG_SIZE - 1) / ALG_LIB_VREG_SIZE));
}

HI_S32 VReg_Init(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
    VREG_ARGS_S stVreg;
    
    if (u32BaseAddr != VReg_BaseAlign(u32BaseAddr))
    {
        return HI_FAILURE;
    }

    if (VREG_CHECK_OPEN())
    {
        return HI_FAILURE;
    }

    /* malloc vreg's phyaddr in kernel */
    stVreg.u32BaseAddr = VReg_BaseAlign(u32BaseAddr);
    stVreg.u32Size = VReg_SizeAlign(u32Size);
    if (ioctl(g_s32VregFd, VREG_DRV_INIT, &stVreg))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 VReg_Exit(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
    HI_VREG_ADDR_S *pstVReg = HI_NULL;
    VREG_ARGS_S stVreg;

    if (u32BaseAddr != VReg_BaseAlign(u32BaseAddr))
    {
        return HI_FAILURE;
    }
    
    /* check base */
    pstVReg = VReg_Match(VReg_BaseAlign(u32BaseAddr));
    if (HI_NULL == pstVReg)
    {
        return HI_FAILURE;
    }

    if (0 != pstVReg->u32VirtAddr)
    {
        /* munmap virtaddr */
        VREG_MUNMAP_VIRTADDR(pstVReg->u32VirtAddr, VReg_SizeAlign(u32Size));
        pstVReg->u32VirtAddr = 0;
        pstVReg->u32PhyAddr = 0;
    }

    if (VREG_CHECK_OPEN())
    {
        return HI_FAILURE;
    }
    /* release the buf in the kernel */
    stVreg.u32BaseAddr = VReg_BaseAlign(u32BaseAddr);
    if (ioctl(g_s32VregFd, VREG_DRV_EXIT, &stVreg))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 VReg_ReleaseAll(HI_VOID)
{
    VREG_ARGS_S stVreg;

    if (VREG_CHECK_OPEN())
    {
        return HI_FAILURE;
    }
    /* release all buf in the kernel */
    if (ioctl(g_s32VregFd, VREG_DRV_RELEASE_ALL, &stVreg))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_U32 VReg_GetVirtAddr(HI_U32 u32BaseAddr)
{
    HI_U32 u32Base, u32Size;
    HI_VREG_ADDR_S *pstVReg = HI_NULL;
    VREG_ARGS_S stVreg;
    HI_U32 u32TmpBaseAddr = (u32BaseAddr & 0xFFFF0000);
    
    if (VREG_CHECK_OPEN())
    {
        return 0;
    }    

    /* check base */
    pstVReg = VReg_Match(u32BaseAddr);
    if (HI_NULL == pstVReg)
    {
        return 0;
    }

    if (0 != pstVReg->u32VirtAddr)
    {
        return pstVReg->u32VirtAddr;
    }

    /* get phyaddr first */
    if ((u32TmpBaseAddr >= VI_ISP0_REG_BASE) && (u32TmpBaseAddr < (VI_ISP0_REG_BASE + VI_ISP_REG_SIZE)))
    {
        pstVReg->u32PhyAddr = VI_ISP0_REG_BASE;
        u32Size = VI_ISP_REG_SIZE;
    }
    else if ((u32TmpBaseAddr >= VI_ISP1_REG_BASE) && (u32TmpBaseAddr < (VI_ISP1_REG_BASE + VI_ISP_REG_SIZE)))
    {
        pstVReg->u32PhyAddr = VI_ISP1_REG_BASE;
        u32Size = VI_ISP_REG_SIZE;
    }
    else
    {
        u32Base = (ISP0_VREG_BASE == u32TmpBaseAddr) ? ISP0_VREG_BASE :
                  ((ISP1_VREG_BASE == u32TmpBaseAddr) ? ISP1_VREG_BASE : (u32BaseAddr & 0xfffff000));
        stVreg.u32BaseAddr = u32Base;
        if (ioctl(g_s32VregFd, VREG_DRV_GETADDR, &stVreg))
        {
            return 0;
        }
        pstVReg->u32PhyAddr = stVreg.u32PhyAddr;

        u32Size = (ISP0_VREG_BASE == u32TmpBaseAddr) ? ISP_VREG_SIZE :
                  ((ISP1_VREG_BASE == u32TmpBaseAddr) ? ISP_VREG_SIZE : ALG_LIB_VREG_SIZE);
    }
    
    /* map virtaddr */
    pstVReg->u32VirtAddr = (HI_U32)HI_MPI_SYS_Mmap(pstVReg->u32PhyAddr, u32Size);

    return pstVReg->u32VirtAddr;
}

HI_S32 VReg_Munmap(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
#if 0    
    HI_S32 i;

    for (i=0; i<ISP_MAX_DEV_NUM; i++)
    {
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.stViIspRegAddr[i].u32VirtAddr, VI_ISP_REG_SIZE);
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.stIspVRegAddr[i].u32VirtAddr, ISP_VREG_SIZE);
    }

    for (i=0; i<MAX_ALG_LIB_VREG_NUM; i++)
    {
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.astAeAddr[i].u32VirtAddr, ALG_LIB_VREG_SIZE);
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.astAwbAddr[i].u32VirtAddr, ALG_LIB_VREG_SIZE);
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.astAfAddr[i].u32VirtAddr, ALG_LIB_VREG_SIZE);
    }

    memset(&g_stHiVreg, 0, sizeof(HI_VREG_S));
    return;
#endif

    HI_VREG_ADDR_S *pstVReg = HI_NULL;

    if (u32BaseAddr != VReg_BaseAlign(u32BaseAddr))
    {
        return HI_FAILURE;
    }

    /* check base */
    pstVReg = VReg_Match(VReg_BaseAlign(u32BaseAddr));
    if (HI_NULL == pstVReg)
    {
        return HI_FAILURE;
    }

    if (0 != pstVReg->u32VirtAddr)
    {
        /* munmap virtaddr */
        VREG_MUNMAP_VIRTADDR(pstVReg->u32VirtAddr, VReg_SizeAlign(u32Size));
        pstVReg->u32VirtAddr = 0;
    }

    return HI_SUCCESS;
}
#endif


HI_U32 VReg_GetAddrOffset(HI_U32 u32Addr)
{
    if ((u32Addr >= VI_ISP0_REG_BASE) && (u32Addr < (VI_ISP0_REG_BASE + VI_ISP_REG_SIZE)))
    {
        return (u32Addr - VI_ISP0_REG_BASE);
    }
    else if ((u32Addr >= VI_ISP1_REG_BASE) && (u32Addr < (VI_ISP1_REG_BASE + VI_ISP_REG_SIZE)))
    {
        return (u32Addr - VI_ISP1_REG_BASE);
    }
    else if ((u32Addr >= ISP0_VREG_BASE) && (u32Addr < (ISP0_VREG_BASE + ISP_VREG_SIZE)))
    {
        return (u32Addr - ISP0_VREG_BASE);
    }
    else if ((u32Addr >= ISP1_VREG_BASE) && (u32Addr < (ISP1_VREG_BASE + ISP_VREG_SIZE)))
    {
        return (u32Addr - ISP1_VREG_BASE);
    }
    else
    {
        return (u32Addr & 0xFFF);
    }

}

#ifdef __KERNEL__
#define HIVREG_WRITE_REG32(b, addr) osal_writel(b, addr)
#define HIVREG_READ_REG32(addr) osal_readl(addr)
#else
#define HIVREG_WRITE_REG32(b, addr) *(addr) = b
#define HIVREG_READ_REG32(addr) *(addr)
#endif

/*--------------------------------------------------------------------------------------*/
HI_U32 IO_READ32(HI_U32 u32Addr)
{
    HI_U32 *pu32Addr, u32VirtAddrBase, u32Value;
    HI_U32 u32AddrOffset = 0;

    u32VirtAddrBase = VReg_GetVirtAddr(u32Addr);
    if (0 == u32VirtAddrBase)
    {
        return 0;
    }

    u32AddrOffset = VReg_GetAddrOffset(u32Addr);
    pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32AddrOffset & 0xFFFFFFFC));
    
    u32Value = HIVREG_READ_REG32(pu32Addr);
    
    return u32Value;
}

HI_S32 IO_WRITE32(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32 *pu32Addr, u32VirtAddrBase;
    HI_U32 u32AddrOffset = 0;

    u32VirtAddrBase = VReg_GetVirtAddr(u32Addr);
    if (0 == u32VirtAddrBase)
    {
        return 0;
    }

    u32AddrOffset = VReg_GetAddrOffset(u32Addr);
    pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32AddrOffset & 0xFFFFFFFC));

    HIVREG_WRITE_REG32(u32Value, pu32Addr);

    return HI_SUCCESS;
}


HI_U8 IO_READ8(HI_U32 u32Addr)
{
    HI_U32 u32Value;    

    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 0)    
    {
        u32Value = IO_READ32(u32Addr);       
        u32Value = (u32Value & 0x000000FF);   
    }      
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 1)
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 1)<<REG_ACCESS_WIDTH_1);    
        u32Value = (u32Value & 0x0000FF00) >> 8; 
    }
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 2)
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 2)<<REG_ACCESS_WIDTH_1);    
        u32Value = (u32Value & 0x00FF0000) >> 16; 
    }
    else 
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 3)<<REG_ACCESS_WIDTH_1);    
        u32Value = (u32Value & 0xFF000000) >> 24; 
    }

    return u32Value;
}

HI_S32 IO_WRITE8(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32  u32TmpAddr, u32Current;

    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 0)    
    {
        u32Current = IO_READ32(u32Addr);      
        IO_WRITE32(u32Addr, ( u32Value & 0x000000FF )|(u32Current & 0xFFFFFF00 )); 
    }  
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 1) 
    {
        u32TmpAddr = ((u32Addr >> REG_ACCESS_WIDTH_1)-1)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x000000FF ) << 8)|(u32Current & 0xFFFF00FF ));
    }
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 2)
    {
        u32TmpAddr = ((u32Addr >> REG_ACCESS_WIDTH_1)-2)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x000000FF) << 16)|(u32Current & 0xFF00FFFF ));
    }
    else
    {
        u32TmpAddr = ((u32Addr >> REG_ACCESS_WIDTH_1)-3)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x000000FF) << 24)|(u32Current & 0x00FFFFFF ));
    }

    return HI_SUCCESS;
}

HI_U16 IO_READ16(HI_U32 u32Addr)
{
    HI_U32  u32Value;

    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 2) == 0)    
    {    
        u32Value = IO_READ32( u32Addr );       
        u32Value = (u32Value & 0xFFFF);    
    }      
    else   
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 2)<<REG_ACCESS_WIDTH_1);    
        u32Value = ((u32Value & 0xFFFF0000) >> 16); 
    }
    
    return u32Value;
}

HI_S32 IO_WRITE16(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32  u32TmpAddr, u32Current;        
    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 2 )== 0)    
    {
        u32Current = IO_READ32(u32Addr);      
        IO_WRITE32(u32Addr, (u32Value & 0x0000FFFF )|(u32Current & 0xFFFF0000 )); 
    }  
    else  
    {
        u32TmpAddr = ((u32Addr >>REG_ACCESS_WIDTH_1)-2)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x0000FFFF ) << 16)|(u32Current & 0x0000FFFF ));
    }
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

