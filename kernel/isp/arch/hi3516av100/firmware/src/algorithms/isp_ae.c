/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ae.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/05
  Description   : 
  History       :
  1.Date        : 2013/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID AeRegsDefault(HI_VOID)
{
    /* still need to set ae default value */    
    /* xuhuanhai 2011/6/16 added for init the ae weighting table . */
    HI_U8 i,j;
    HI_U8 u8Weighttable[15][17]=
        {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1},
         {1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1},
         {1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},};
    for (i=0; i<15; i++)
    {
        for (j=0; j<17; j++)
        {
            hi_isp_zones_aexp_weight_write(i, j, u8Weighttable[i][j]);
        }
    }
    return;
}
HI_S32 ISP_AeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AeInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AE_PARAM_S stAeParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    AeRegsDefault();
    
    stAeParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    stAeParam.u8WDRMode = pstIspCtx->u8SnsWDRMode;
    stAeParam.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;

    /* init all registered ae libs */
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_init)
            {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_init(
                    pstLib->stAlgLib.s32Id, &stAeParam);
            }
        }
    }
   
    return HI_SUCCESS;
}

HI_S32 ISP_AeRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i, j, s32Ret = HI_FAILURE;
    ISP_AE_INFO_S       stAeInfo    = {0};
    ISP_AE_RESULT_S     stAeResult  = {{0}};
    ISP_CTX_S          *pstIspCtx   = HI_NULL;
    ISP_LIB_NODE_S     *pstLib      = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAeLibInfo.astLibs[pstIspCtx->stAeLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    stAeInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAeInfo.pstAeStat1 = &((ISP_STAT_S *)pStatInfo)->stAeStat1;
    stAeInfo.pstAeStat2 = &((ISP_STAT_S *)pStatInfo)->stAeStat2;
    stAeInfo.pstAeStat3 = &((ISP_STAT_S *)pStatInfo)->stAeStat3;
    stAeInfo.pstAeStat4 = &((ISP_STAT_S *)pStatInfo)->stAeStat4; 
    stAeInfo.pstAeStat5 = &((ISP_STAT_S *)pStatInfo)->stAeStat5;

    if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_run)
    {
        s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_run(
            pstLib->stAlgLib.s32Id, &stAeInfo, &stAeResult, 0);
        if (HI_SUCCESS != s32Ret)
        {
            printf("WARNING!! run ae lib err 0x%x!\n", s32Ret);
        }
    }
    
    if (stAeResult.stStatAttr.bChange)
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg1.au8WeightTable[i][j] =
                    stAeResult.stStatAttr.au8WeightTable[i][j];
            }
        }

        for (i = 0; i < 4; i++)
        {
            ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg1.au8MeteringHistThresh[i] =
                stAeResult.stStatAttr.au8MeteringHistThresh[i];
        }

        ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1AeCfg1 = 1;
    }    
    
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[0] = stAeResult.u32IntTime[0];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[1] = stAeResult.u32IntTime[1];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[2] = stAeResult.u32IntTime[2];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[3] = stAeResult.u32IntTime[3];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IspDgain = stAeResult.u32IspDgain;
    ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1AeCfg2 = 1;

    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.bPirisValid = stAeResult.bPirisValid;
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.s32PirisPos = stAeResult.s32PirisPos;
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.enFSWDRMode = stAeResult.enFSWDRMode;

    /* be careful avoid overflow */
    if(stAeResult.bPirisValid == HI_TRUE)
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u64Exposure = (HI_U64)stAeResult.u32IntTime[0] * stAeResult.u32Iso * stAeResult.u32PirisGain;
    }
    else
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u64Exposure = (HI_U64)stAeResult.u32IntTime[0] * stAeResult.u32Iso;
    }
	
    pstIspCtx->stLinkage.u32IspDgain = stAeResult.u32IspDgain;
    pstIspCtx->stLinkage.u32IspDgainShift = 8;
    pstIspCtx->stLinkage.u32Iso = stAeResult.u32Iso;
    pstIspCtx->stLinkage.u32SensorIso = ((HI_U64)stAeResult.u32Iso << 8) / DIV_0_TO_1(stAeResult.u32IspDgain);  // IspDgain is 8bit precision
    pstIspCtx->stLinkage.u32SensorIso = (pstIspCtx->stLinkage.u32SensorIso < 100) ? 100 : pstIspCtx->stLinkage.u32SensorIso;
    pstIspCtx->stLinkage.u32ExpRatio = (stAeResult.u32IntTime[1] << 6) / DIV_0_TO_1(stAeResult.u32IntTime[0]);  // WDR exposure ratio is 6bit precision
    pstIspCtx->stLinkage.u32Inttime = stAeResult.u32IntTime[0];
    if (HI_TRUE == stAeResult.bPirisValid)
    {
        pstIspCtx->stLinkage.u32PirisGain = stAeResult.u32PirisGain;
    }
    else
    {
        pstIspCtx->stLinkage.u32PirisGain = 0;
    }
    pstIspCtx->stLinkage.u8AERunInterval = stAeResult.u8AERunInterval;
    pstIspCtx->stLinkage.enFSWDRMode = stAeResult.enFSWDRMode;

    return s32Ret;
}

HI_S32 ISP_AeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{    
    HI_S32  i, s32Ret = HI_FAILURE;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl)
            {
                s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(
                    pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    }

    return s32Ret;
}

HI_S32 ISP_AeExit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_exit)
            {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_exit(
                    pstLib->stAlgLib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterAe(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AeExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

