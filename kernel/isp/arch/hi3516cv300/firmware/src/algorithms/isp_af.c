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
#include <math.h>
#include "isp_alg.h"
#include "hi_comm_isp.h"
#include "isp_config.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

ISP_FOCUS_STATISTICS_CFG_S  g_stFocusCfg =        
{   
    {1,  8,  8, 1920, 1080, 1, 0, {0, 0, 0, 0, 0}, 0, {0, 0, 0, 0, 0}, {1, 0x9bff}, 0xf0}, 
    {1, {1,  1,  1}, {188,  414, -330, 486, -461, 400, -328}, {7, 0, 3, 1}, {1, 0, 255, 0, 220, 8, 14}, {127, 12, 2047} },
    {0, {1,  1,  0}, {200,  200, -110, 461, -415,  0,  0},    {6, 0, 1, 0}, {0, 0, 0,   0, 0,   0, 0 }, {15,  12, 2047} },
    {{20,   16,  0, -16, -20}, {1, 0, 255, 0, 220, 8, 14}, {38,  12, 1800} },
    {{-12, -24,  0,  24,  12}, {1, 0, 255, 0, 220, 8, 14}, {15,  12, 2047} },
    {4, {0, 0}, {1, 1}, 0}
};

HI_S32 ISP_AfCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);


HI_S32 ISP_AfDefautParam(const ISP_FOCUS_STATISTICS_CFG_S *pstFocusCfg)
{
    HI_U8 u8Shift0, u8I;
    HI_S16 s16G0, s16G1, s16G2;
    HI_FLOAT f32Temp, f32Pl;
    HI_U32 u32Plg, u32Pls;
    const ISP_AF_H_PARAM_S *pstIIR;
    ISP_DEV IspDev = 0;

    if (!pstFocusCfg)
    {
        return HI_ERR_ISP_NULL_PTR;
    }

    /* AF STATISTICS CONIFG START */
    
    hi_vi_af_enable_write(pstFocusCfg->stConfig.bEnable);
    hi_vi_af_iir0_en0_write(pstFocusCfg->stHParam_IIR0.abIIREn[0]);
    hi_vi_af_iir0_en1_write(pstFocusCfg->stHParam_IIR0.abIIREn[1]);
    hi_vi_af_iir0_en2_write(pstFocusCfg->stHParam_IIR0.abIIREn[2]);
    hi_vi_af_iir1_en0_write(pstFocusCfg->stHParam_IIR1.abIIREn[0]);
    hi_vi_af_iir1_en1_write(pstFocusCfg->stHParam_IIR1.abIIREn[1]);
    hi_vi_af_iir1_en2_write(pstFocusCfg->stHParam_IIR1.abIIREn[2]);
    hi_vi_af_fvmode_peak_write(pstFocusCfg->stConfig.enPeakMode);
    hi_vi_af_fvmode_squ_write(pstFocusCfg->stConfig.enSquMode);
    hi_vi_af_hwnd_write(pstFocusCfg->stConfig.u16Hwnd);
    hi_vi_af_vwnd_write(pstFocusCfg->stConfig.u16Vwnd);
    hi_vi_af_iir0_gain0_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[0]);
    hi_vi_af_iir1_gain0_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[0]);
    hi_vi_af_iir0_gain1_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[1]);
    hi_vi_af_iir1_gain1_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[1]);
    hi_vi_af_iir0_gain2_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[2]);
    hi_vi_af_iir1_gain2_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[2]);
    hi_vi_af_iir0_gain3_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[3]);
    hi_vi_af_iir1_gain3_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[3]);
    hi_vi_af_iir0_gain4_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[4]);
    hi_vi_af_iir1_gain4_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[4]);
    hi_vi_af_iir0_gain5_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[5]);
    hi_vi_af_iir1_gain5_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[5]);
    hi_vi_af_iir0_gain6_write((HI_U32)pstFocusCfg->stHParam_IIR0.as16IIRGain[6]);
    hi_vi_af_iir1_gain6_write((HI_U32)pstFocusCfg->stHParam_IIR1.as16IIRGain[6]);
    hi_vi_af_iir0_shift0_write(pstFocusCfg->stHParam_IIR0.au16IIRShift[0]);
    hi_vi_af_iir0_shift1_write(pstFocusCfg->stHParam_IIR0.au16IIRShift[1]);
    hi_vi_af_iir0_shift2_write(pstFocusCfg->stHParam_IIR0.au16IIRShift[2]);
    hi_vi_af_iir0_shift3_write(pstFocusCfg->stHParam_IIR0.au16IIRShift[3]);
    hi_vi_af_iir1_shift0_write(pstFocusCfg->stHParam_IIR1.au16IIRShift[0]);
    hi_vi_af_iir1_shift1_write(pstFocusCfg->stHParam_IIR1.au16IIRShift[1]);
    hi_vi_af_iir1_shift2_write(pstFocusCfg->stHParam_IIR1.au16IIRShift[2]);
    hi_vi_af_iir1_shift3_write(pstFocusCfg->stHParam_IIR1.au16IIRShift[3]);
    hi_vi_af_fir0_h0_write((HI_U32)pstFocusCfg->stVParam_FIR0.as16FIRH[0]);
    hi_vi_af_fir1_h0_write((HI_U32)pstFocusCfg->stVParam_FIR1.as16FIRH[0]);
    hi_vi_af_fir0_h1_write((HI_U32)pstFocusCfg->stVParam_FIR0.as16FIRH[1]);
    hi_vi_af_fir1_h1_write((HI_U32)pstFocusCfg->stVParam_FIR1.as16FIRH[1]);
    hi_vi_af_fir0_h2_write((HI_U32)pstFocusCfg->stVParam_FIR0.as16FIRH[2]);
    hi_vi_af_fir1_h2_write((HI_U32)pstFocusCfg->stVParam_FIR1.as16FIRH[2]);
    hi_vi_af_fir0_h3_write((HI_U32)pstFocusCfg->stVParam_FIR0.as16FIRH[3]);
    hi_vi_af_fir1_h3_write((HI_U32)pstFocusCfg->stVParam_FIR1.as16FIRH[3]);
    hi_vi_af_fir0_h4_write((HI_U32)pstFocusCfg->stVParam_FIR0.as16FIRH[4]);
    hi_vi_af_fir1_h4_write((HI_U32)pstFocusCfg->stVParam_FIR1.as16FIRH[4]);

    /* AF crop */
    hi_vi_af_crop_en_write(pstFocusCfg->stConfig.stCrop.bEnable);
    if (pstFocusCfg->stConfig.stCrop.bEnable)
    {
        hi_vi_af_crop_posx_write(pstFocusCfg->stConfig.stCrop.u16X);
        hi_vi_af_crop_posy_write(pstFocusCfg->stConfig.stCrop.u16Y);
        hi_vi_af_crop_hsize_write(pstFocusCfg->stConfig.stCrop.u16W);
        hi_vi_af_crop_vsize_write(pstFocusCfg->stConfig.stCrop.u16H);
    }

    /* AF raw cfg */
    if (ISP_AF_STATISTICS_YUV == pstFocusCfg->stConfig.enStatisticsPos)
    {
        hi_vi_af_pos_write(HI_VI_AF_POS_BE);
        hi_vi_af_raw_mode_write(HI_VI_AF_RAW_MODE_YUV);
    }
    else if (ISP_AF_STATISTICS_RAW == pstFocusCfg->stConfig.enStatisticsPos)
    {
        hi_vi_af_pos_write(HI_VI_AF_POS_FE);
        hi_vi_af_raw_mode_write(HI_VI_AF_RAW_MODE_RAW);
        hi_isp_module_af_sel_write(IspDev, HI_ISP_MOUDLE_POS_AF_AFTER_DG);
    }
    else
    {
        hi_vi_af_pos_write(HI_VI_AF_POS_FE);
        hi_vi_af_raw_mode_write(HI_VI_AF_RAW_MODE_RAW);
        hi_isp_module_af_sel_write(IspDev, HI_ISP_MOUDLE_POS_AF_AFTER_DRC);
    }
    //hi_vi_af_pos_write(~pstFocusCfg->stConfig.enStatisticsPos);
    //hi_vi_af_raw_mode_write(pstFocusCfg->stConfig.enStatisticsPos);
    
    //if (ISP_AF_STATISTICS_RAW == pstFocusCfg->stConfig.enStatisticsPos)
    {
        hi_vi_af_sqrt_en_write(pstFocusCfg->stConfig.stRawCfg.bGammaEn);
        hi_vi_af_bayer_mode_write(pstFocusCfg->stConfig.stRawCfg.enPattern);
        hi_vi_af_offset_en_write(pstFocusCfg->stConfig.stRawCfg.bOffsetEn);
        //if (pstFocusCfg->stConfig.stRawCfg.bOffsetEn)
        {
            hi_vi_af_offset_gr_write(pstFocusCfg->stConfig.stRawCfg.u16GrOffset);
            hi_vi_af_offset_gb_write(pstFocusCfg->stConfig.stRawCfg.u16GbOffset);
        }         
    }

    /* AF pre median filter */
    hi_vi_af_median_en_write(pstFocusCfg->stConfig.stPreFltCfg.bEn);
    //if (pstFocusCfg->stConfig.stPreFltCfg.bEn)
    {    
        hi_vi_af_median_th_write(0xffff - pstFocusCfg->stConfig.stPreFltCfg.u16strength);
    }

    /* level depend gain */
    hi_vi_af_iir0_ldg_en_write(pstFocusCfg->stHParam_IIR0.stLd.bLdEn);
    //if (pstFocusCfg->stHParam_IIR0.stLd.bLdEn)
    {
        hi_vi_af_iir0_ldg_th_l_write  (pstFocusCfg->stHParam_IIR0.stLd.u16ThLow    );
        hi_vi_af_iir0_ldg_th_h_write  (pstFocusCfg->stHParam_IIR0.stLd.u16ThHigh   );
        hi_vi_af_iir0_ldg_slp_l_write (pstFocusCfg->stHParam_IIR0.stLd.u16SlpLow   );
        hi_vi_af_iir0_ldg_slp_h_write (pstFocusCfg->stHParam_IIR0.stLd.u16SlpHigh  );
        hi_vi_af_iir0_ldg_gain_l_write(pstFocusCfg->stHParam_IIR0.stLd.u16GainLow  );
        hi_vi_af_iir0_ldg_gain_h_write(pstFocusCfg->stHParam_IIR0.stLd.u16GainHigh );

    }
    
    hi_vi_af_iir1_ldg_en_write(pstFocusCfg->stHParam_IIR1.stLd.bLdEn);
    //if (pstFocusCfg->stHParam_IIR1.stLd.bLdEn)
    {
        hi_vi_af_iir1_ldg_th_l_write  (pstFocusCfg->stHParam_IIR1.stLd.u16ThLow    );
        hi_vi_af_iir1_ldg_th_h_write  (pstFocusCfg->stHParam_IIR1.stLd.u16ThHigh   );
        hi_vi_af_iir1_ldg_slp_l_write (pstFocusCfg->stHParam_IIR1.stLd.u16SlpLow   );
        hi_vi_af_iir1_ldg_slp_h_write (pstFocusCfg->stHParam_IIR1.stLd.u16SlpHigh  );
        hi_vi_af_iir1_ldg_gain_l_write(pstFocusCfg->stHParam_IIR1.stLd.u16GainLow  );
        hi_vi_af_iir1_ldg_gain_h_write(pstFocusCfg->stHParam_IIR1.stLd.u16GainHigh );
    }

    hi_vi_af_fir0_ldg_en_write(pstFocusCfg->stVParam_FIR0.stLd.bLdEn);
    //if (pstFocusCfg->stVParam_FIR0.stLd.bLdEn)
    {
        hi_vi_af_fir0_ldg_th_l_write  (pstFocusCfg->stVParam_FIR0.stLd.u16ThLow    );
        hi_vi_af_fir0_ldg_th_h_write  (pstFocusCfg->stVParam_FIR0.stLd.u16ThHigh   );
        hi_vi_af_fir0_ldg_slp_l_write (pstFocusCfg->stVParam_FIR0.stLd.u16SlpLow   );
        hi_vi_af_fir0_ldg_slp_h_write (pstFocusCfg->stVParam_FIR0.stLd.u16SlpHigh  );
        hi_vi_af_fir0_ldg_gain_l_write(pstFocusCfg->stVParam_FIR0.stLd.u16GainLow  );
        hi_vi_af_fir0_ldg_gain_h_write(pstFocusCfg->stVParam_FIR0.stLd.u16GainHigh );
    }

    hi_vi_af_fir1_ldg_en_write(pstFocusCfg->stVParam_FIR1.stLd.bLdEn);
    //if (pstFocusCfg->stVParam_FIR1.stLd.bLdEn)
    {
        hi_vi_af_fir1_ldg_th_l_write  (pstFocusCfg->stVParam_FIR1.stLd.u16ThLow    );
        hi_vi_af_fir1_ldg_th_h_write  (pstFocusCfg->stVParam_FIR1.stLd.u16ThHigh   );
        hi_vi_af_fir1_ldg_slp_l_write (pstFocusCfg->stVParam_FIR1.stLd.u16SlpLow   );
        hi_vi_af_fir1_ldg_slp_h_write (pstFocusCfg->stVParam_FIR1.stLd.u16SlpHigh  );
        hi_vi_af_fir1_ldg_gain_l_write(pstFocusCfg->stVParam_FIR1.stLd.u16GainLow  );
        hi_vi_af_fir1_ldg_gain_h_write(pstFocusCfg->stVParam_FIR1.stLd.u16GainHigh );
    }

    /* AF coring */
    hi_vi_af_iir0_cor_th_write   (pstFocusCfg->stHParam_IIR0.stCoring.u16Th   );
    hi_vi_af_iir0_cor_slp_write  (pstFocusCfg->stHParam_IIR0.stCoring.u16Slp  );
    hi_vi_af_iir0_cor_peak_write (pstFocusCfg->stHParam_IIR0.stCoring.u16Lmt  );

    hi_vi_af_iir1_cor_th_write   (pstFocusCfg->stHParam_IIR1.stCoring.u16Th   );
    hi_vi_af_iir1_cor_slp_write  (pstFocusCfg->stHParam_IIR1.stCoring.u16Slp  );
    hi_vi_af_iir1_cor_peak_write (pstFocusCfg->stHParam_IIR1.stCoring.u16Lmt  ); 
                                                                      
    hi_vi_af_fir0_cor_th_write   (pstFocusCfg->stVParam_FIR0.stCoring.u16Th   );
    hi_vi_af_fir0_cor_slp_write  (pstFocusCfg->stVParam_FIR0.stCoring.u16Slp  );
    hi_vi_af_fir0_cor_peak_write (pstFocusCfg->stVParam_FIR0.stCoring.u16Lmt  );
                                                                      
    hi_vi_af_fir1_cor_th_write   (pstFocusCfg->stVParam_FIR1.stCoring.u16Th   );
    hi_vi_af_fir1_cor_slp_write  (pstFocusCfg->stVParam_FIR1.stCoring.u16Slp  );
    hi_vi_af_fir1_cor_peak_write (pstFocusCfg->stVParam_FIR1.stCoring.u16Lmt  );

    /* high luma counter */
    hi_vi_af_highluma_th_write(pstFocusCfg->stConfig.u16HighLumaTh);

    /* AF output shift */
    hi_vi_af_acc_shift_h0_write(pstFocusCfg->stFVParam.au16AccShiftH[0]);
    hi_vi_af_acc_shift_h1_write(pstFocusCfg->stFVParam.au16AccShiftH[1]);
    hi_vi_af_acc_shift_v0_write(pstFocusCfg->stFVParam.au16AccShiftV[0]);
    hi_vi_af_acc_shift_v1_write(pstFocusCfg->stFVParam.au16AccShiftV[1]);
    hi_vi_af_acc_shift_y_write(pstFocusCfg->stFVParam.u16AccShiftY);
    hi_vi_af_cnt_shift_highluma_write(pstFocusCfg->stFVParam.u16HlCntShift);

    hi_vi_af_iir0_ds_en_write(pstFocusCfg->stHParam_IIR0.bNarrowBand);
    hi_vi_af_iir1_ds_en_write(pstFocusCfg->stHParam_IIR1.bNarrowBand);

    /* PLG and PLS */
    for ( u8I = 0 ; u8I < 2 ; u8I++ )
    {
        pstIIR = u8I? &(pstFocusCfg->stHParam_IIR1) : &(pstFocusCfg->stHParam_IIR0);
        
        u8Shift0 =  pstIIR->au16IIRShift[0];
        s16G0 = pstIIR->as16IIRGain[0];
        s16G1 = pstIIR->as16IIRGain[1];
        s16G2 = pstIIR->as16IIRGain[2];
        
        f32Pl = (512.f / (512-2*s16G1-s16G2) * s16G0) /  (1 << u8Shift0);
        f32Temp = f32Pl;
        f32Temp = MIN2(7-floor(log(f32Temp)/log(2)), 7);
        
        u32Pls = (HI_U32)f32Temp;
        u32Plg = (HI_U32)((f32Pl * (1 << u32Pls)) + 0.5);

        if (0 == u8I)
        {
            hi_vi_af_iir0_pls_write(u32Pls);
            hi_vi_af_iir0_plg_write(u32Plg);
        }
        else
        {
            hi_vi_af_iir1_pls_write(u32Pls);
            hi_vi_af_iir1_plg_write(u32Plg);        
        }
    }

    hi_vi_af_hsize_write(pstFocusCfg->stConfig.u16Hsize - 1);
    hi_vi_af_vsize_write(pstFocusCfg->stConfig.u16Vsize - 1); 

    return HI_SUCCESS;
}

HI_S32 ISP_AfInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AF_PARAM_S stAfParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    ISP_BAYER_FORMAT_E enCfa;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    /* some registers about AF module shound be initial */
    enCfa = hi_isp_yuv444_rggb_start_read(IspDev);
    hi_vi_af_bayer_mode_write(enCfa);

    hi_vi_af_lpf_en_write(HI_VI_AF_LPF_EN_DEFAULT);
    hi_vi_af_fir0_lpf_en_write(HI_VI_AF_FIR0_LPF_EN_DEFAULT);
    hi_vi_af_fir1_lpf_en_write(HI_VI_AF_FIR1_LPF_EN_DEFAULT);
    hi_vi_af_iir0_ds_en_write(HI_VI_AF_IIR0_DS_EN_DEFAULT);
    hi_vi_af_iir1_ds_en_write(HI_VI_AF_IIR1_DS_EN_DEFAULT);
    hi_vi_af_iir0_dilate_write(HI_VI_AF_IIR0_DILATE_DEFAULT);
    hi_vi_af_iir1_dilate_write(HI_VI_AF_IIR1_DILATE_DEFAULT);
    hi_vi_af_iir0_plg_write(HI_VI_AF_IIR0_PLG_DEFAULT);
    hi_vi_af_iir0_pls_write(HI_VI_AF_IIR0_PLS_DEFAULT);
    hi_vi_af_iir1_plg_write(HI_VI_AF_IIR1_PLG_DEFAULT);
    hi_vi_af_iir1_pls_write(HI_VI_AF_IIR1_PLS_DEFAULT);   
    
    g_stFocusCfg.stConfig.u16Hsize = hi_isp_crop_width_read(IspDev) + 1;
    g_stFocusCfg.stConfig.u16Vsize = hi_isp_crop_height_read(IspDev) + 1;
    
    ISP_AfDefautParam(&g_stFocusCfg);

    hi_vi_af_update_mode_write(HI_VI_AF_UPDATE_MODE_DEFAULT);
    
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

    stAfInfo.pstAfStat = &((ISP_STAT_S *)pStatInfo)->stAfStat;
    
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

