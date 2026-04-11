/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_af.c
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
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

ISP_FOCUS_STATISTICS_CFG_S  g_stFocusCfg =        
{
    {1, 8, 8, 1920, 1080, 1, 0}, 
    {{0, 1, 1}, {188,  0,   0,    414, -165, 400, -164}, {7, 0, 3, 2}, 127},
    {{0, 1, 0}, {200,  0,   0,    200,   -55,  0,   0},   {6, 0, 1, 0}, 31},
    {{-6,  12, 22, 12, -6}, 511}, 
    {{-16, 21, 0, -21, 16}, 10},
    {0, {0, 0}, {3, 2}}
};

HI_S32 ISP_AfDefautParam()
{

    hi_vi_af_enable_write(g_stFocusCfg.stConfig.bEnable);
    hi_vi_af_iir0_en0_write(g_stFocusCfg.stHParam_IIR0.abIIREn[0]);
    hi_vi_af_iir0_en1_write(g_stFocusCfg.stHParam_IIR0.abIIREn[1]);
    hi_vi_af_iir0_en2_write(g_stFocusCfg.stHParam_IIR0.abIIREn[2]);
    hi_vi_af_iir1_en0_write(g_stFocusCfg.stHParam_IIR1.abIIREn[0]);
    hi_vi_af_iir1_en1_write(g_stFocusCfg.stHParam_IIR1.abIIREn[1]);
    hi_vi_af_iir1_en2_write(g_stFocusCfg.stHParam_IIR1.abIIREn[2]);
    hi_vi_af_fvmode_peak_write(g_stFocusCfg.stConfig.enPeakMode);
    hi_vi_af_fvmode_squ_write(g_stFocusCfg.stConfig.enSquMode);
    hi_vi_af_hwnd_write(g_stFocusCfg.stConfig.u16Hwnd);
    hi_vi_af_vwnd_write(g_stFocusCfg.stConfig.u16Vwnd);
    hi_vi_af_iir0_gain0_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
    hi_vi_af_iir1_gain0_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
    hi_vi_af_iir0_gain1_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[1]);
    hi_vi_af_iir1_gain1_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[1]);
    hi_vi_af_iir0_gain2_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[2]);
    hi_vi_af_iir1_gain2_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[2]);
    hi_vi_af_iir0_gain3_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[3]);
    hi_vi_af_iir1_gain3_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[3]);
    hi_vi_af_iir0_gain4_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[4]);
    hi_vi_af_iir1_gain4_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[4]);
    hi_vi_af_iir0_gain5_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[5]);
    hi_vi_af_iir1_gain5_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[5]);
    hi_vi_af_iir0_gain6_write((HI_U32)g_stFocusCfg.stHParam_IIR0.as16IIRGain[6]);
    hi_vi_af_iir1_gain6_write((HI_U32)g_stFocusCfg.stHParam_IIR1.as16IIRGain[6]);
    hi_vi_af_iir0_shift0_write(g_stFocusCfg.stHParam_IIR0.au16IIRShift[0]);
    hi_vi_af_iir0_shift1_write(g_stFocusCfg.stHParam_IIR0.au16IIRShift[1]);
    hi_vi_af_iir0_shift2_write(g_stFocusCfg.stHParam_IIR0.au16IIRShift[2]);
    hi_vi_af_iir0_shift3_write(g_stFocusCfg.stHParam_IIR0.au16IIRShift[3]);
    hi_vi_af_iir1_shift0_write(g_stFocusCfg.stHParam_IIR1.au16IIRShift[0]);
    hi_vi_af_iir1_shift1_write(g_stFocusCfg.stHParam_IIR1.au16IIRShift[1]);
    hi_vi_af_iir1_shift2_write(g_stFocusCfg.stHParam_IIR1.au16IIRShift[2]);
    hi_vi_af_iir1_shift3_write(g_stFocusCfg.stHParam_IIR1.au16IIRShift[3]);
    hi_vi_af_fir0_h0_write((HI_U32)g_stFocusCfg.stVParam_FIR0.as16FIRH[0]);
    hi_vi_af_fir1_h0_write((HI_U32)g_stFocusCfg.stVParam_FIR1.as16FIRH[0]);
    hi_vi_af_fir0_h1_write((HI_U32)g_stFocusCfg.stVParam_FIR0.as16FIRH[1]);
    hi_vi_af_fir1_h1_write((HI_U32)g_stFocusCfg.stVParam_FIR1.as16FIRH[1]);
    hi_vi_af_fir0_h2_write((HI_U32)g_stFocusCfg.stVParam_FIR0.as16FIRH[2]);
    hi_vi_af_fir1_h2_write((HI_U32)g_stFocusCfg.stVParam_FIR1.as16FIRH[2]);
    hi_vi_af_fir0_h3_write((HI_U32)g_stFocusCfg.stVParam_FIR0.as16FIRH[3]);
    hi_vi_af_fir1_h3_write((HI_U32)g_stFocusCfg.stVParam_FIR1.as16FIRH[3]);
    hi_vi_af_fir0_h4_write((HI_U32)g_stFocusCfg.stVParam_FIR0.as16FIRH[4]);
    hi_vi_af_fir1_h4_write((HI_U32)g_stFocusCfg.stVParam_FIR1.as16FIRH[4]);
    hi_vi_af_iir0_thd_x_write(g_stFocusCfg.stHParam_IIR0.u16IIRThd);
    hi_vi_af_iir1_thd_x_write(g_stFocusCfg.stHParam_IIR1.u16IIRThd);
    hi_vi_af_fir0_thd_y_write(g_stFocusCfg.stVParam_FIR0.u16FIRThd);
    hi_vi_af_fir1_thd_y_write(g_stFocusCfg.stVParam_FIR1.u16FIRThd);
    hi_vi_af_acc_shift_h0_write(g_stFocusCfg.stFVParam.au16AccShiftH[0]);
    hi_vi_af_acc_shift_h1_write(g_stFocusCfg.stFVParam.au16AccShiftH[1]);
    hi_vi_af_acc_shift_v0_write(g_stFocusCfg.stFVParam.au16AccShiftV[0]);
    hi_vi_af_acc_shift_v1_write(g_stFocusCfg.stFVParam.au16AccShiftV[1]);
    hi_vi_af_acc_shift_y_write(g_stFocusCfg.stFVParam.u16AccShiftY);
    hi_vi_af_hsize_write(hi_isp_input_port_hc_size0_read() - 1);
    hi_vi_af_vsize_write(hi_isp_input_port_vc_size_read() - 1);

    return HI_SUCCESS;    
}

HI_S32 ISP_AfCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AfInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AF_PARAM_S stAfParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    ISP_AfDefautParam();
    
    stAfParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAfLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAfLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_init)
            {
                pstLib->stAfRegsiter.stAfExpFunc.pfn_af_init(
                    pstLib->stAlgLib.s32Id, &stAfParam);
            }
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_AfRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 s32Ret = HI_FAILURE;
    ISP_AF_INFO_S       stAfInfo = {0};
    ISP_AF_RESULT_S     stAfResult = {0};
    ISP_CTX_S          *pstIspCtx   = HI_NULL;
    ISP_LIB_NODE_S     *pstLib      = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAfLibInfo.astLibs[pstIspCtx->stAfLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    stAfInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAfInfo.stAfStat = &((ISP_STAT_S *)pStatInfo)->stAfStat;
    
    if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_run)
    {
        s32Ret = pstLib->stAfRegsiter.stAfExpFunc.pfn_af_run(
            pstLib->stAlgLib.s32Id, &stAfInfo, &stAfResult, 0);
        if (HI_SUCCESS != s32Ret)
        {
            printf("WARNING!! run af lib err 0x%x!\n", s32Ret);
        }
    }

    return s32Ret;
}

HI_S32 ISP_AfCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{    
    HI_S32  i, s32Ret = HI_FAILURE;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAfLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAfLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_ctrl)
            {
                s32Ret = pstLib->stAfRegsiter.stAfExpFunc.pfn_af_ctrl(
                    pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    }

    return s32Ret;
}

HI_S32 ISP_AfExit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAfLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAfLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_exit)
            {
                pstLib->stAfRegsiter.stAfExpFunc.pfn_af_exit(
                    pstLib->stAlgLib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterAf(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AF;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AfInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AfRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AfCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AfExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

