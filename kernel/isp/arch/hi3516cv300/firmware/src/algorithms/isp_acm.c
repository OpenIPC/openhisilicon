/******************************************************************************

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_acm.c
  Version       : Initial Draft
  Author        : y00246723
  Created       : 2014/04/24
  Last Modified :
  Description   : acm alg
  Function List :
              
  History       :
  1.Date        : 2014/04/24
    Author      : y00246723
    Modification: Created file

  2.Date         : 2014/08/06
    Author       : y00246723
    Modification : Move memory alloc and free to KERNEL

  3.Date         : 2014/08/06
    Author       : y00246723
    Modification : Check parameter in USER 

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"

#include "hi_comm_vi.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#include "mkp_vi.h"
#include "mkp_isp.h"
#include "isp_proc.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
 HI_S32 ISP_AcmInit(ISP_DEV IspDev);

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/
 
 typedef struct hiISP_ACM_ALG_CTX_S
{
    HI_BOOL bAlgInit;
    ISP_ACM_BUFFER_S stAcmMem;    
} ISP_ACM_ALG_CTX_S;

 
ISP_ACM_ALG_CTX_S g_astAcmCtx[ISP_MAX_DEV_NUM] = {{0}};
#define ACM_GET_CTX(dev, pstCtx)   pstCtx = &g_astAcmCtx[dev]



#define ACM_DBG_LVL HI_DGB_ERR

static inline HI_S32 ACM_CheckAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{

    if((pstAcmAttr->bEnable & 0xFFFFFFFE) != 0 )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error bEnable:%d\n",pstAcmAttr->bEnable);
        return HI_FAILURE;
    }

    if((pstAcmAttr->bDemoEnable & 0xFFFFFFFE) != 0 )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error bDemoEnable:%d\n",pstAcmAttr->bDemoEnable);
        return HI_FAILURE;
    }

    if((pstAcmAttr->u32Stretch & 0xFFFFFFFE) != 0)
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32Stretch:%d\n",pstAcmAttr->u32Stretch);
        return HI_FAILURE;
    }

    if((pstAcmAttr->u32ClipRange  & 0xFFFFFFFE) != 0   )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32ClipRange:%d\n",pstAcmAttr->u32ClipRange);
        return HI_FAILURE;
    }

    if((pstAcmAttr->u32AcmClipOrWrap  & 0xFFFFFFFE) != 0)
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32AcmClipOrWrap:%d\n",pstAcmAttr->u32AcmClipOrWrap);
        return HI_FAILURE;
    }

    if( pstAcmAttr->u32CbcrThr >255     )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32CbcrThr:%d\n",pstAcmAttr->u32CbcrThr);
        return HI_FAILURE;
    }

    if( pstAcmAttr->u32GainLuma > 512    )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32GainLuma:%d\n",pstAcmAttr->u32GainLuma);
        return HI_FAILURE;
    }

    if( pstAcmAttr->u32GainHue > 512     )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32GainHue:%d\n",pstAcmAttr->u32GainHue);
        return HI_FAILURE;
    }

    if( pstAcmAttr->u32GainSat > 512     )
    {
        ISP_TRACE(HI_DBG_ERR, "Error acm attribute error u32GainSat:%d\n",pstAcmAttr->u32GainSat);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_GetAttr
 Description  : Get ACM attribute
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{
    HI_S32 s32Ret;
    ISP_CHECK_OPEN(0);    
    s32Ret = ioctl(g_as32IspFd[0], ISP_ACM_ATTR_GET, pstAcmAttr);
    return s32Ret;
}

/*****************************************************************************
 Prototype    : ISP_ACM_SetAttr
 Description  : Set ACM attribute
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{
    HI_S32 s32Ret;
    ISP_CHECK_OPEN(0);    

    s32Ret = ACM_CheckAttr(pstAcmAttr);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_DEF_ERR(HI_ID_ISP,EN_ERR_LEVEL_ERROR,EN_ERR_ILLEGAL_PARAM);
    }
    s32Ret = ioctl(g_as32IspFd[0], ISP_ACM_ATTR_SET, pstAcmAttr);
    return s32Ret;
}

/*****************************************************************************
 Prototype    : ISP_ACM_GetCoeff
 Description  : Get ACM Coeff
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_GetCoeff(ISP_ACM_LUT_S *pstAcmLUT)
{
    HI_S32 s32Ret;
	
	ISP_ACM_BUFFER_S stAcmBuffer;

    ISP_CHECK_OPEN(0);    

    s32Ret = ioctl(g_as32IspFd[0], ISP_ACM_LUT_PHY_ADDR_GET, &stAcmBuffer);

    stAcmBuffer.pVirAddr = HI_MPI_SYS_Mmap(stAcmBuffer.u32PhyAddr, stAcmBuffer.u32AcmSize);
	if(NULL == stAcmBuffer.pVirAddr)
	{
	    printf("HI_MPI_SYS_Mmap null\n");
        return 0;
	}
 
    memcpy(pstAcmLUT, stAcmBuffer.pVirAddr, sizeof(ISP_ACM_LUT_S));

	HI_MPI_SYS_Munmap(stAcmBuffer.pVirAddr, stAcmBuffer.u32AcmSize);
	
    return s32Ret;
}

/*****************************************************************************
 Prototype    : ISP_ACM_SetCoeff
 Description  : Set ACM coeff
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_SetCoeff(ISP_ACM_LUT_S *pstAcmLUT)
{
    HI_S32 s32Ret;
	ISP_ACM_BUFFER_S stAcmBuffer;

	ISP_CHECK_OPEN(0);

	s32Ret = ioctl(g_as32IspFd[0], ISP_ACM_LUT_PHY_ADDR_GET, &stAcmBuffer);
    stAcmBuffer.pVirAddr = HI_MPI_SYS_Mmap(stAcmBuffer.u32PhyAddr, stAcmBuffer.u32AcmSize);
    if(NULL == stAcmBuffer.pVirAddr)
	{
	    printf("HI_MPI_SYS_Mmap null\n");
        return 0;
	}
 	
    memcpy(stAcmBuffer.pVirAddr, pstAcmLUT, sizeof(ISP_ACM_LUT_S));

    s32Ret = ioctl(g_as32IspFd[0], ISP_ACM_COEF_SET, &stAcmBuffer);

	HI_MPI_SYS_Munmap(stAcmBuffer.pVirAddr, stAcmBuffer.u32AcmSize);


    return s32Ret;
}

/********************* INTERFACE****************************/

/*****************************************************************************
 Prototype    : ISP_AcmInit
 Description  : ACM ALG initialize
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_AcmInit(ISP_DEV IspDev)
{   
    HI_S32 s32Ret;
    HI_U32 u32w, u32H;

    ISP_ACM_ALG_CTX_S *pstAcm = HI_NULL;

    ACM_GET_CTX(IspDev, pstAcm);
    ISP_CHECK_OPEN(IspDev);   

    if (pstAcm->bAlgInit)
    {
       return HI_SUCCESS; 
    }
    s32Ret = ioctl(g_as32IspFd[0], ISP_ACM_LUT_PHY_ADDR_GET, &pstAcm->stAcmMem); 
    pstAcm->stAcmMem.pVirAddr = HI_MPI_SYS_Mmap(pstAcm->stAcmMem.u32PhyAddr, pstAcm->stAcmMem.u32AcmSize);
	if(NULL == pstAcm->stAcmMem.pVirAddr)
	{
	    printf("HI_MPI_SYS_Mmap null\n");
        return 0;
	}
    
    if (pstAcm->stAcmMem.pVirAddr && !s32Ret)
    {
        pstAcm->bAlgInit= HI_TRUE;

        // update ACM width and height

	    u32w  = hi_ext_system_bas_outwidth_read();
	    u32H = hi_ext_system_bas_outheight_read();
		hi_isp_acm_width_write(IspDev, u32w - 1);
		hi_isp_acm_height_write(IspDev, u32H - 1);
        hi_isp_acm_dbg_pos_write(IspDev, u32w >> 1);
	
        return HI_SUCCESS;
    } 
	
	
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_AcmRun
 Description  : ACM interface RUN
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_AcmRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    return HI_SUCCESS;
}

HI_S32 AcmProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_ACM_ATTR_S stAcmAttr;
    ISP_ACM_ATTR_S* pstAcmAttr = &stAcmAttr;
 
    ISP_ACM_GetAttr(pstAcmAttr);

    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----ACM INFO------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s" "%8s"      "%16s"     "%8s"   "%8s"   "%16s"      "%16s"       "%16s"      "%16s\n",
        "En", "DemoEn",  "Stretch", "Clip", "Wrap", "CbcrThr", "GainLuma", "GainHue", "GainSta");

    if(hi_isp_acm_enable_read(IspDev))
    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d"  "%8d"      "%16d"   "%8d"   "%8d"     "%16d"   "%16d"   "%16d"     "%16d",
        hi_isp_acm_enable_read(IspDev),
        hi_isp_acm_dbg_enable_read(IspDev),
        hi_isp_acm_stretch_read(IspDev),
        hi_isp_acm_cliprange_read(IspDev),
        hi_isp_acm_cliporwrap_read(IspDev),
        hi_isp_acm_cbcrthr_read(IspDev),
        hi_isp_acm_luma_gain_read(IspDev),
        hi_isp_acm_hue_gain_read(IspDev),
        hi_isp_acm_sat_gain_read(IspDev)
        );
    }
    else
    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%8d"    "%16d"     "%8d"   "%8d"   "%16d"      "%16d"       "%16d"      "%16d",
        hi_isp_acm_enable_read(IspDev),
        pstAcmAttr->bDemoEnable     , 
        pstAcmAttr->u32Stretch      ,
        pstAcmAttr->u32ClipRange    ,
        pstAcmAttr->u32AcmClipOrWrap,
        pstAcmAttr->u32CbcrThr      ,
        pstAcmAttr->u32GainLuma     ,
        pstAcmAttr->u32GainHue      ,
        pstAcmAttr->u32GainSat      
        );
    }

    pstProc->u32WriteLen += 1;

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : AcmImageResWrite
 Description  : acm image res write
 Input        : ISP_DEV IspDev                        
                ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/10
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
static __inline HI_S32 AcmImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
    // update Acm width and height
    
    hi_isp_acm_width_write(IspDev, pstRes->u16Width - 1);
	hi_isp_acm_height_write(IspDev, pstRes->u16Height - 1);
    hi_isp_acm_dbg_pos_write(IspDev, pstRes->u16Width >> 1);

    return HI_SUCCESS;
}


/*****************************************************************************
 Prototype    : ISP_AcmCtrl
 Description  : ACM interface Ctrl
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_AcmCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_PROC_WRITE:
            AcmProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;

		case ISP_CHANGE_IMAGE_MODE_SET:   
			AcmImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);  
			break;  
			
        default :
            break;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_AcmExit
 Description  : ACM interface Exit
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_AcmExit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;

    ISP_ACM_ALG_CTX_S *pstAcm = HI_NULL;

    ACM_GET_CTX(IspDev, pstAcm); 

    if ( pstAcm->bAlgInit )
    {
       s32Ret = HI_MPI_SYS_Munmap(pstAcm->stAcmMem.pVirAddr, pstAcm->stAcmMem.u32AcmSize);
       pstAcm->bAlgInit= HI_FALSE;
    }
    else
    {
        return HI_FAILURE;
    }

    return s32Ret;

}

/*****************************************************************************
 Prototype    : ISP_AlgRegisterAcm
 Description  : ACM alg register
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_AlgRegisterAcm(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_ACM;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AcmInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AcmRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AcmCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AcmExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

