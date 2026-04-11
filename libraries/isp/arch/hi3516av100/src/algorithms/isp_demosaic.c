/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_demosaic.c
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
#include "isp_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


typedef struct hiISP_DEMOSAIC_S
{
    HI_U8   u8VhSlope;
    HI_U8   u8AaSlope;
    HI_U8   u8VaSlope;
    HI_U8   u8UuSlope;
    HI_U8   u8SatSlope;
    HI_U8   u8AcSlope;
    HI_U8   u8FcSlope;
    HI_U16  u16VhThresh;
    HI_U16  u16AaThresh;
    HI_U16  u16VaThresh;
    HI_U16  u16UuThresh;
    HI_U16  u16SatThresh;
    HI_U16  u16AcThresh;
} ISP_DEMOSAIC_S;

ISP_DEMOSAIC_S g_astDemosaicCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DEMOSAIC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDemosaicCtx[dev]


static HI_VOID DemosaicExtRegsDefault(HI_VOID)
{
    return;
}

static HI_VOID DemosaicRegsDefault(HI_VOID) 
{
    hi_isp_demosaic_vh_slope_write(0xf5);
    hi_isp_demosaic_vh_thresh_write(0x30);
    hi_isp_demosaic_va_thresh_write(0x52);
    hi_isp_demosaic_aa_thresh_write(0x5b);
    hi_isp_demosaic_uu_thresh_write(0x40);
    hi_isp_demosaic_uu_slope_write(0xc0);
    hi_isp_demosaic_aa_slope_write(0x98);
    hi_isp_demosaic_va_slope_write(0xe6);
    hi_isp_demosaic_fc_slope_write(0x80); 
    hi_isp_demosaic_ac_offset_write(0xbc0);
    
    return;
}

static HI_VOID DemosaicExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID DemosaicRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    if (HI_TRUE == pstSnsDft->stDemosaic.bValid)
    {
        hi_isp_demosaic_vh_slope_write(pstSnsDft->stDemosaic.u8VhSlope);
        hi_isp_demosaic_aa_slope_write(pstSnsDft->stDemosaic.u8AaSlope);
        hi_isp_demosaic_va_slope_write(pstSnsDft->stDemosaic.u8VaSlope);
        hi_isp_demosaic_uu_slope_write(pstSnsDft->stDemosaic.u8UuSlope);
        hi_isp_demosaic_sat_slope_write(pstSnsDft->stDemosaic.u8SatSlope);
        hi_isp_demosaic_ac_slope_write(pstSnsDft->stDemosaic.u8AcSlope);

        hi_isp_demosaic_vh_thresh_write(pstSnsDft->stDemosaic.u16VhThresh);
        hi_isp_demosaic_aa_thresh_write(pstSnsDft->stDemosaic.u16AaThresh);
        hi_isp_demosaic_va_thresh_write(pstSnsDft->stDemosaic.u16VaThresh);
        hi_isp_demosaic_uu_thresh_write(pstSnsDft->stDemosaic.u16UuThresh);
        hi_isp_demosaic_sat_thresh_write(pstSnsDft->stDemosaic.u16SatThresh);
        hi_isp_demosaic_ac_thresh_write(pstSnsDft->stDemosaic.u16AcThresh);

        hi_isp_demosaic_fc_slope_write(pstSnsDft->stDemosaic.u8FcSlope);
    }
   
    return;
}

static HI_S32 DemosaicReadExtregs(ISP_DEV IspDev)
{    
    ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

    pstDemosaic->u8VhSlope = hi_isp_demosaic_vh_slope_read();
    pstDemosaic->u8AaSlope = hi_isp_demosaic_aa_slope_read();
    pstDemosaic->u8VaSlope = hi_isp_demosaic_va_slope_read();
    pstDemosaic->u8UuSlope = hi_isp_demosaic_uu_slope_read();
    pstDemosaic->u8SatSlope = hi_isp_demosaic_sat_slope_read();
    pstDemosaic->u8AcSlope = hi_isp_demosaic_ac_slope_read();
    pstDemosaic->u8FcSlope = hi_isp_demosaic_fc_slope_read();
    
    pstDemosaic->u16VhThresh = hi_isp_demosaic_vh_thresh_read();
    pstDemosaic->u16AaThresh = hi_isp_demosaic_aa_thresh_read();
    pstDemosaic->u16VaThresh = hi_isp_demosaic_va_thresh_read();
    pstDemosaic->u16UuThresh = hi_isp_demosaic_uu_thresh_read();
    pstDemosaic->u16SatThresh = hi_isp_demosaic_sat_thresh_read();
    pstDemosaic->u16AcThresh = hi_isp_demosaic_ac_thresh_read();
    
    return 0;
}

HI_S32 DemosaicProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----DEMOSAIC INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8s"      "%8s"   "%8s"      "%8s"   "%8s"      "%8s"   "%8s"      "%8s\n",
            "VhSlope", "VhTh", "AaSlope", "AaTh", "VaSlope", "VaTh", "UuSlope", "UuTh");
    
            ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "    0x%2x" "   0x%3x" "    0x%2x" "   0x%3x" "    0x%2x" "   0x%3x" "    0x%2x" "   0x%3x\n\n",
            pstDemosaic->u8VhSlope,
            pstDemosaic->u16VhThresh,
            pstDemosaic->u8AaSlope,
            pstDemosaic->u16AaThresh,
            pstDemosaic->u8VaSlope,
            pstDemosaic->u16VaThresh,
            pstDemosaic->u8UuSlope,
            pstDemosaic->u16UuThresh
            );
    
            ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8s"       "%8s"    "%8s"      "%8s"   "%8s\n",
            "SatSlope", "SatTh", "AcSlope", "AcTh", "FcSlope");
    
            ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "    0x%2x" "   0x%3x" "    0x%2x" "   0x%3x" "    0x%2x\n",
            pstDemosaic->u8SatSlope,
            pstDemosaic->u16SatThresh,
            pstDemosaic->u8AcSlope,
            pstDemosaic->u16AcThresh, 
            pstDemosaic->u8FcSlope
            );

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_DemosaicInit(ISP_DEV IspDev)
{
    DemosaicRegsDefault();
    DemosaicExtRegsDefault();
    DemosaicRegsInitialize(IspDev);
    DemosaicExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    DemosaicReadExtregs(IspDev);
    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_DemosaicInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            DemosaicProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDemosaic(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DEMOSAIC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DemosaicInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DemosaicRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DemosaicCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DemosaicExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


