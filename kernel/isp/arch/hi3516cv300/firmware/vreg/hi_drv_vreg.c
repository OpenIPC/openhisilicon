/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_vreg.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/09
  Description   : 
  History       :
  1.Date        : 2013/01/09
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifdef __KERNEL__
#include "hi_osal.h"

#include "mm_ext.h"
#include "hi_drv_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static VREG_ARGS_S g_stVreg[VREG_MAX_NUM] = {{0}};
static inline VREG_ARGS_S *VREG_DRV_Search(HI_U32 u32BaseAddr)
{
    HI_S32  i;
    for (i=0; i<VREG_MAX_NUM; i++)
    {
        if ((0 != g_stVreg[i].u32PhyAddr)
            && (u32BaseAddr == g_stVreg[i].u32BaseAddr))
        {            
            return &g_stVreg[i];
        }
    }

    return NULL;
}

HI_S32 VREG_DRV_Init(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
    HI_S32  i;
    HI_CHAR acName[16] = {0};
    VREG_ARGS_S *pstVreg = NULL;

    /* check param */
    if (0 == u32Size)
    {
        ISP_TRACE(HI_DBG_ERR, "The vreg's size is 0!\n");
        return HI_FAILURE;
    }

    pstVreg = VREG_DRV_Search(u32BaseAddr);
    if (NULL != pstVreg)
    {        
        ISP_TRACE(HI_DBG_ERR, "The vreg of u32BaseAddr 0x%x has registerd!\n", u32BaseAddr);
        return HI_FAILURE;
    }

    /* search pos */
    for (i=0; i<VREG_MAX_NUM; i++)
    {
        if (0 == g_stVreg[i].u32PhyAddr)
        {
            pstVreg = &g_stVreg[i];
            break;
        }
    }

    if (NULL == pstVreg)
    {
        ISP_TRACE(HI_DBG_ERR, "The vreg is too many, can't register!\n");
        return HI_FAILURE;
    }

    /* malloc memory */
    osal_snprintf(acName, sizeof(acName), "VirtReg%d", i);
    
    pstVreg->u32PhyAddr = CMPI_MmzMalloc(NULL, acName, u32Size);
    if (0 == pstVreg->u32PhyAddr)
    {
        return HI_FAILURE;
    }
    pstVreg->u32BaseAddr = u32BaseAddr;
    
    return HI_SUCCESS;
}

HI_S32 VREG_DRV_Exit(HI_U32 u32BaseAddr)
{
    VREG_ARGS_S *pstVreg = NULL;

    pstVreg = VREG_DRV_Search(u32BaseAddr);
    if (NULL == pstVreg)
    {
        ISP_TRACE(HI_DBG_ERR, "The vreg of u32BaseAddr 0x%x has not registerd!\n", u32BaseAddr);
        return HI_FAILURE;
    }
    
    CMPI_MmzFree(pstVreg->u32PhyAddr, NULL);
    pstVreg->u32Size= 0;
    pstVreg->u32BaseAddr = 0;
    pstVreg->u32PhyAddr = 0;

    return HI_SUCCESS;
}

HI_S32 VREG_DRV_GetAddr(HI_U32 u32BaseAddr, HI_U32 *pu32PhyAddr)
{
    VREG_ARGS_S *pstVreg = NULL;

    pstVreg = VREG_DRV_Search(u32BaseAddr);
    if (NULL == pstVreg)
    {
        ISP_TRACE(HI_DBG_ERR, "The vreg of u32BaseAddr 0x%x has not registerd!\n", u32BaseAddr);
        return HI_FAILURE;
    }

    *pu32PhyAddr = pstVreg->u32PhyAddr;
    
    return HI_SUCCESS;
}

HI_VOID VREG_DRV_ReleaseAll(HI_VOID)
{
    HI_S32  i;

    for (i=0; i<VREG_MAX_NUM; i++)
    {
        if (0 != g_stVreg[i].u32PhyAddr)
        {
            CMPI_MmzFree(g_stVreg[i].u32PhyAddr, NULL);
            g_stVreg[i].u32PhyAddr = 0;
        }
    }

    return;
}

long VREG_DRV_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    unsigned int   *argp = (unsigned int   *)arg;
    HI_S32  s32Ret;

    switch (cmd)
    {        
        /* malloc memory for vregs, and record information in kernel. */
        case VREG_DRV_INIT :
        {
            VREG_ARGS_S stVreg;

            osal_memcpy(&stVreg, argp, sizeof(VREG_ARGS_S));
            
            s32Ret = VREG_DRV_Init(stVreg.u32BaseAddr, stVreg.u32Size);
            return s32Ret;
        }
        /* free the memory of vregs, and clean information in kernel. */
        case VREG_DRV_EXIT :
        {
            VREG_ARGS_S stVreg;
            
            osal_memcpy(&stVreg, argp, sizeof(VREG_ARGS_S));
            
            s32Ret = VREG_DRV_Exit(stVreg.u32BaseAddr);
            return s32Ret;
        }
        /* get the mapping relation between vreg addr and physical addr. */
        case VREG_DRV_GETADDR :
        {
            VREG_ARGS_S stVreg;
            
            osal_memcpy(&stVreg, argp, sizeof(VREG_ARGS_S));
            
            s32Ret = VREG_DRV_GetAddr(stVreg.u32BaseAddr, &stVreg.u32PhyAddr);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
            
            osal_memcpy(argp, &stVreg, sizeof(VREG_ARGS_S));
            
            return 0;
        }
        default:
        {

            return HI_FAILURE;
        }
    }

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

