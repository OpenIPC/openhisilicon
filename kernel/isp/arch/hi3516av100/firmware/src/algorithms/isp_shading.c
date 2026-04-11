/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_shading.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/24
  Description   : 
  History       :
  1.Date        : 2013/04/24
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_shading_mem_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID ShadingExtRegsDefault(HI_VOID)
{    
    hi_ext_system_shading_table_node_number_write(HI_EXT_SYSTEM_SHADING_TABLE_NODE_NUMBER_DEFAULT);
    return;
}

static HI_VOID ShadingRegsDefault(HI_VOID) 
{
    return;
}

static HI_VOID ShadingExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stShading.bValid)
    {
        hi_ext_system_shading_table_node_number_write(pstSnsDft->stShading.u16TblNodeNum);
    }
    return;
}

static HI_VOID ShadingRegsInitialize(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);   
    
    if (pstSnsDft->stShading.bValid)
    {
        pstSnsDft->stShading.u16TblNodeNum =
            (pstSnsDft->stShading.u16TblNodeNum < CMOS_SHADING_TABLE_NODE_NUMBER_MAX) ?
            pstSnsDft->stShading.u16TblNodeNum : CMOS_SHADING_TABLE_NODE_NUMBER_MAX;

        hi_isp_radial_shading_centerr_x_write(pstSnsDft->stShading.u16RCenterX);
        hi_isp_radial_shading_centerr_y_write(pstSnsDft->stShading.u16RCenterY);
        hi_isp_radial_shading_centerg_x_write(pstSnsDft->stShading.u16GCenterX);
        hi_isp_radial_shading_centerg_y_write(pstSnsDft->stShading.u16GCenterY);
        hi_isp_radial_shading_centerb_x_write(pstSnsDft->stShading.u16BCenterX);
        hi_isp_radial_shading_centerb_y_write(pstSnsDft->stShading.u16BCenterY);

        hi_isp_radial_shading_mcu_priority_write(1);
        for(i = 0; i < pstSnsDft->stShading.u16TblNodeNum; i++)
        {
            hi_radial_shading_mem_luts_shading_lutr_write(i, pstSnsDft->stShading.au16RShadingTbl[i]);
            hi_radial_shading_mem_luts_shading_lutg_write(i, pstSnsDft->stShading.au16GShadingTbl[i]);
            hi_radial_shading_mem_luts_shading_lutb_write(i, pstSnsDft->stShading.au16BShadingTbl[i]);
        }
        hi_isp_radial_shading_mcu_priority_write(0);

        hi_isp_radial_shading_off_center_multr_write(pstSnsDft->stShading.u16ROffCenter);
        hi_isp_radial_shading_off_center_multg_write(pstSnsDft->stShading.u16GOffCenter);
        hi_isp_radial_shading_off_center_multb_write(pstSnsDft->stShading.u16BOffCenter);

        hi_isp_radial_shading_enable_write(1);
    }
    
    return;
}

static HI_S32 ShadingReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 ShadingProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----SHADING INFO--------------------------------------------------------------\n");
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s\n",
        "En");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d\n",
        hi_isp_radial_shading_enable_read());

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_ShadingInit(ISP_DEV IspDev)
{
    ShadingRegsDefault();
    ShadingExtRegsDefault();
    ShadingRegsInitialize(IspDev);
    ShadingExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_ShadingRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ShadingReadExtregs(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_ShadingCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_ShadingInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            ShadingProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_ShadingExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterShading(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SHADING;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_ShadingInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_ShadingRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_ShadingCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_ShadingExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


