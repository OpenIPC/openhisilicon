/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_noise_reduction.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "hi_math.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define NR_LUT_SIZE 128 
#define NR_NR_BITS 12 

#define RATIO_SHIFT 6

typedef struct hiISP_NOISE_REDUCTION_S
{
    /* Public */
    HI_U8  u8SnrThreshShort;    /*for short exposure*/
    HI_U8  u8SnrThreshLong;     /*for long exposure*/
    HI_U8  u8NpShortRatio;
    HI_U8  u8NpLongRatio;
    HI_U8  u8NoiseProfile;
    HI_U8  u8NpOffset;
    HI_U16 u16NpExpThresh;

    /* Private */
    HI_U32 u32StructSize;
    HI_U16 u16TnrIncrement;
    HI_U8  au8TnrLog[NR_LUT_SIZE];
    HI_U32 u32TnrDarkNl;
    HI_U32 u32TnrBrightNl;
    HI_U8  u8TnrStrengthTarget;
    HI_U8  u8SnrThreshTargetShort;
    HI_U8  u8SnrThreshTargetLong;
    HI_S16 s16TnrThreshMaster;
    HI_S16 s16SnrThreshMaster;

    HI_U16 u16BlackLevel;
    HI_U8  u8SnrThreshMax;
    HI_U32 u32Iso;
} ISP_NOISE_REDUCTION_S;

ISP_NOISE_REDUCTION_S g_astNrCtx[ISP_MAX_DEV_NUM] = {{0}};
#define NR_GET_CTX(dev, pstCtx)   pstCtx = &g_astNrCtx[dev]

#define HI_FW_NOISE_EXP_THRESH_HDR  0x4E62
#define HI_FW_NOISE_SHORT_RATIO_HDR 0x04
#define HI_FW_NOISE_LONG_RATIO_HDR  0x40

#define HI_FW_NOISE_EXP_THRESH_LIN  0xFFFF
#define HI_FW_NOISE_SHORT_RATIO_LIN 0x04
#define HI_FW_NOISE_LONG_RATIO_LIN  0x04

static HI_VOID NrExtRegsDefault(ISP_DEV IspDev)
{
    /* initial register of noise reduction */
    hi_ext_system_manual_nr_write(IspDev, HI_EXT_SYSTEM_MANUAL_NR_DEFAULT);
    hi_ext_system_nr_threshold_target_write(IspDev, HI_EXT_SYSTEM_NR_THRESHOLD_TARGET_DEFAULT);
    hi_ext_system_nr_threshold_long_target_write(IspDev, HI_EXT_SYSTEM_NR_THRESHOLD_LONG_TARGET_DEFAULT); 

    return;
}

static HI_VOID NrRegsDefault(ISP_DEV IspDev) 
{    
    /* Noise profile, weaker denoise */
    hi_isp_noise_profile_exp_thresh_write(IspDev, 0xffff); 
    hi_isp_noise_profile_short_ratio_write(IspDev, 4);
    hi_isp_noise_profile_long_ratio_write(IspDev, 4);

    /* nr parameters, enable nr by default */
    hi_isp_top_bypass_nr_write(IspDev, 0);
    hi_isp_nr_enable_write(IspDev, 1);
    
    return;
}

static HI_VOID NrExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U8 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stAgcTbl.bValid)
    {
        for (i = 0; i < 16; i++)
        {
            hi_ext_system_agc_table_snr_thresh_write(IspDev, i, pstSnsDft->stAgcTbl.au8SnrThresh[i]);
            
            hi_ext_system_agc_table_demosaic_np_offset_write(IspDev, i, pstSnsDft->stAgcTbl.au8DemosaicNpOffset[i]);
        }
    }

    return;
}

static HI_VOID NrRegsInitialize(ISP_DEV IspDev)
{    
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    /* What is the function of these register?? */
    hi_isp_nr_filter_select_write(IspDev, 1);
    hi_isp_nr_int_select_write(IspDev, 1);
    hi_isp_nr_int_config_write(IspDev, 2);

    if (IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_noise_profile_raw_frontend_exp_thresh_write(IspDev, HI_FW_NOISE_EXP_THRESH_HDR);
        hi_isp_noise_profile_raw_frontend_short_ratio_write(IspDev, HI_FW_NOISE_SHORT_RATIO_HDR);
        hi_isp_noise_profile_raw_frontend_long_ratio_write(IspDev, HI_FW_NOISE_LONG_RATIO_HDR);
        
        hi_isp_noise_profile_exp_thresh_write(IspDev, HI_FW_NOISE_EXP_THRESH_HDR); 
        hi_isp_noise_profile_short_ratio_write(IspDev, HI_FW_NOISE_SHORT_RATIO_HDR);
        hi_isp_noise_profile_long_ratio_write(IspDev, HI_FW_NOISE_LONG_RATIO_HDR);
    }
    else if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode) || IS_BUILT_IN_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_noise_profile_raw_frontend_exp_thresh_write(IspDev, HI_FW_NOISE_EXP_THRESH_LIN);
        hi_isp_noise_profile_raw_frontend_short_ratio_write(IspDev, HI_FW_NOISE_SHORT_RATIO_LIN);
        hi_isp_noise_profile_raw_frontend_long_ratio_write(IspDev, HI_FW_NOISE_LONG_RATIO_LIN);
        
        hi_isp_noise_profile_exp_thresh_write(IspDev, HI_FW_NOISE_EXP_THRESH_LIN); 
        hi_isp_noise_profile_short_ratio_write(IspDev, HI_FW_NOISE_SHORT_RATIO_LIN);
        hi_isp_noise_profile_long_ratio_write(IspDev, HI_FW_NOISE_LONG_RATIO_LIN);
    }
    
    return;
}

static HI_S32 NrReadExtregs(ISP_DEV IspDev)
{
    ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;
    
    NR_GET_CTX(IspDev, pstNr);

    pstNr->u8SnrThreshTargetShort = hi_ext_system_nr_threshold_target_read(IspDev);
    pstNr->u8SnrThreshTargetLong = hi_ext_system_nr_threshold_long_target_read(IspDev); 

    return 0;
}

HI_S32 NrInitialize(ISP_DEV IspDev, ISP_NOISE_REDUCTION_S *pstNr, ISP_CMOS_DEFAULT_S *pstSnsDft,
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel)
{    
    HI_S32 i = 0;
    HI_U8 au8NosieProfileTable[NR_LUT_SIZE] = {0};
    HI_U8 u8DemosaicWeightLut[NR_LUT_SIZE] = {0};

    pstNr->u32StructSize = sizeof(ISP_NOISE_REDUCTION_S);
    pstNr->u8SnrThreshTargetShort = 0x23;
    pstNr->u8SnrThreshTargetLong = 0x23;
    pstNr->s16TnrThreshMaster   = 0x30;
    pstNr->s16SnrThreshMaster   = 0x30;
    pstNr->u16NpExpThresh = HI_FW_NOISE_EXP_THRESH_LIN;
    pstNr->u8NpShortRatio = HI_FW_NOISE_SHORT_RATIO_LIN;
    pstNr->u8NpLongRatio = HI_FW_NOISE_LONG_RATIO_LIN;
    
    if (HI_TRUE == pstSnsDft->stNoiseTbl.bValid)
    {
        memcpy(au8NosieProfileTable, pstSnsDft->stNoiseTbl.au8NoiseProfileWeightLut, NR_LUT_SIZE);
        memcpy(u8DemosaicWeightLut, pstSnsDft->stNoiseTbl.au8DemosaicWeightLut, NR_LUT_SIZE);
        pstNr->u8NoiseProfile = 1;
    }
    else
    {
        pstNr->u8NoiseProfile = 0;
    }

    pstNr->u16BlackLevel = pstSnsBlackLevel->au16BlackLevel[0];

    if (1 == pstNr->u8NoiseProfile)
    {
        for (i=0; i<NR_LUT_SIZE; i++)
        {
            hi_isp_noise_profile_weight_lut_write(IspDev, i, au8NosieProfileTable[i]);
        }
 
        for (i=0; i<NR_LUT_SIZE; i++)
        {
            hi_isp_demosaic_weight_lut_write(IspDev, i, u8DemosaicWeightLut[i]);
        }
    }
    else
    {
        pstNr->u32TnrDarkNl = 2;
        pstNr->u32TnrBrightNl = 256;    

        pstNr->u16TnrIncrement = (pstNr->u32TnrBrightNl - pstNr->u32TnrDarkNl) * 2;

        for (i=0; i<NR_LUT_SIZE; i++)
        {
            pstNr->au8TnrLog[i] = log16((sqrt16(pstNr->u16TnrIncrement * i + pstNr->u32TnrDarkNl * 256))) - 64;

            hi_isp_noise_profile_weight_lut_write(IspDev, i, (pstNr->au8TnrLog[i] - pstNr->au8TnrLog[0]));
            hi_isp_demosaic_weight_lut_write(IspDev, i, (pstNr->au8TnrLog[i] - pstNr->au8TnrLog[0]));
        }
    }
    
    return 0;
}

static __inline HI_U8 NrIsValid(ISP_NOISE_REDUCTION_S *pstNr)
{
    return (pstNr->u32StructSize != sizeof(ISP_NOISE_REDUCTION_S));
}

static __inline HI_U8 NrOffsetCalculate(
    const HI_U8 u8Y2,
    const HI_U8 u8Y1,
    const HI_U32 u32X2,
    const HI_U32 u32X1,
    const HI_U32 u32Iso)    
{
    HI_U8 u8Offset =  (ABS((HI_S32)u8Y2 - u8Y1) * (u32Iso - u32X1)
        + (u32X2 - u32X1)/2)/(u32X2 - u32X1);
    return u8Offset;
}

HI_S32 NrThreshCalculate(ISP_DEV IspDev, ISP_NOISE_REDUCTION_S *pstNr,
    HI_U32 u32Iso, HI_U8 u8SnsWDRMode, const HI_U16 u16ExpRatio)
{
    HI_U8 i = 0;
    HI_U8 u8Y1 = 0;
    HI_U8 u8Y2 = 0;
    HI_U8 u8Y3 = 0;
    HI_U8 u8Y4 = 0;
    HI_U8 u8Offset = 0;
    HI_U8 u8NpOffset = 0; 
    HI_U32 u32ISO1 = 0;
    HI_U32 u32ISO2 = 0;

    HI_U32 au32IsoLut[16] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
    
    if (0 != NrIsValid(pstNr))
    {
        return HI_FAILURE;
    }

    if (u32Iso >= au32IsoLut[15])
    {
        u8Y1 = hi_ext_system_agc_table_snr_thresh_read(IspDev, 15);
        u8Y2 = u8Y1;
        u8Offset = 0;
        u8NpOffset = hi_ext_system_agc_table_demosaic_np_offset_read(IspDev, 15);        
    }
    else
    {
        for (i = 1; i < 16; i++)
        {
            if (u32Iso < au32IsoLut[i])
            {
                u32ISO1 = au32IsoLut[i-1];
                u32ISO2 = au32IsoLut[i]; 
                break;
            }
        }           
            
        u8Y1 = hi_ext_system_agc_table_snr_thresh_read(IspDev, i-1);
        u8Y2 = hi_ext_system_agc_table_snr_thresh_read(IspDev, i);
        u8Offset = NrOffsetCalculate(u8Y2,u8Y1,u32ISO2,u32ISO1,u32Iso);
        u8Y3 =  hi_ext_system_agc_table_demosaic_np_offset_read(IspDev, i-1);
        u8Y4 =  hi_ext_system_agc_table_demosaic_np_offset_read(IspDev, i);
        u8NpOffset = NrOffsetCalculate(u8Y4,u8Y3,u32ISO2,u32ISO1,u32Iso);
    }

    if (u8Y1 > u8Y2)
    {
        pstNr->u8SnrThreshShort = u8Y1 - u8Offset;
    }
    else
    {
        pstNr->u8SnrThreshShort = u8Y1 + u8Offset;
    }

    if (u8Y3 > u8Y4)
    {
        pstNr->u8NpOffset = u8Y3 - u8NpOffset;
    }
    else
    {
        pstNr->u8NpOffset = u8Y3 + u8NpOffset;
    }   
 
	return HI_SUCCESS;
}

#define GAMMAFE_MAX_VALUE (65536)
HI_U32 NrNpCalcGammafeOut(ISP_DEV IspDev, HI_U32 u32Value)
{
    HI_U32 u32Index = 0, u32IndexPoint = 0, u32weight0 = 0, u32weight1 = 0;
    HI_U32 u32GammafeOut0 = 0, u32GammafeOut1 = 0;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (!pstSnsDft->stGammafe.bValid)
    {
        return 0;
    }

    if (u32Value < pstSnsDft->stGammafe.au32Gammafe0[0])
    {
        return pstSnsDft->stGammafe.au32Gammafe1[0];
    }
    else if (u32Value >= pstSnsDft->stGammafe.au32Gammafe0[GAMMA_FE0_NODE_NUM - 1])
    {
        return pstSnsDft->stGammafe.au32Gammafe1[GAMMA_FE1_NODE_NUM - 1];
    }
    else
    {
        u32IndexPoint = (u32Value << 8) * (GAMMA_FE0_NODE_NUM - 1) / GAMMAFE_MAX_VALUE;
        u32Index = (u32IndexPoint >> 8);
        u32weight1 = (u32IndexPoint & 0xFF);
        u32weight0 = 0x100 - u32weight1;
        u32GammafeOut0 = u32weight0 * pstSnsDft->stGammafe.au32Gammafe0[u32Index] + u32weight1 * pstSnsDft->stGammafe.au32Gammafe0[u32Index+1];

        u32IndexPoint = u32GammafeOut0 * (GAMMA_FE1_NODE_NUM - 1) / GAMMAFE_MAX_VALUE;
        u32Index = (u32IndexPoint >> 8);
        u32weight1 = (u32IndexPoint & 0xFF);
        u32weight0 = 0x100 - u32weight1;
        u32GammafeOut1 = u32weight0 * pstSnsDft->stGammafe.au32Gammafe1[u32Index] + u32weight1 * pstSnsDft->stGammafe.au32Gammafe1[u32Index+1];

        return (u32GammafeOut1 >> 8);
    }

}


#define NP_LUT_IN        1,     2,     3,     4,     5,     6,     7,     8,     9,     10,    11,   12,    13,    14,    15,    16
//#define NP_A        0xDF61,0xA0DE,0x8481,0x732B,0x671B,0x5DEF,0x5698,0x5081,0x4B4B,0x46BD,0x42AD,0x3F00,0x3B9E,0x3875,0x357A,0x32A0
#define NP_B        0xF848,0xB2D5,0x937B,0x806D,0x732B,0x6941,0x6155,0x5AD1,0x554A,0x5081,0x4C46,0x487F,0x4510,0x41EC,0x3F00,0x3C46
#define EXP_RATIO_SHIFT 6

HI_S32 NrNPCal(ISP_DEV IspDev, ISP_NOISE_REDUCTION_S *pstNr, HI_U8 u8SnsWDRMode)
{
    HI_S64 S64NRThreshTmp = 0, S64NRThreshDiv = 0;
    HI_U32 u32GammaFeDlAlpha = 0;
    HI_U32 u32WDRLongThresh = 0, u32WDRExpRatio = 0, u32BlackLevel = 0;
    //HI_U32 u32WDRShortThresh = 0,
    
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    if (IS_FS_WDR_MODE(u8SnsWDRMode))
    {
        if(IS_2to1_WDR_MODE(u8SnsWDRMode))
        {
            u32WDRLongThresh = hi_isp_frame_stitch_short_long_thresh_read(IspDev);
            //u32WDRShortThresh = hi_isp_frame_stitch_short_short_thresh_read(IspDev);
            u32WDRExpRatio = hi_isp_frame_stitch_short_exposure_ratio_read(IspDev);
        }
        else if(IS_3to1_WDR_MODE(u8SnsWDRMode))
        {
            u32WDRLongThresh = hi_isp_frame_stitch_med_long_thresh_read(IspDev);
            //u32WDRShortThresh = hi_isp_frame_stitch_med_short_thresh_read(IspDev);
            u32WDRExpRatio = (HI_U64)hi_isp_frame_stitch_med_exposure_ratio_read(IspDev) * \
                hi_isp_frame_stitch_short_exposure_ratio_read(IspDev) >> RATIO_SHIFT;
        }
        else if(IS_4to1_WDR_MODE(u8SnsWDRMode))
        {
            u32WDRLongThresh = hi_isp_frame_stitch_long_long_thresh_read(IspDev);
            //u32WDRShortThresh = hi_isp_frame_stitch_long_short_thresh_read(IspDev);
            u32WDRExpRatio = (HI_U64)hi_isp_frame_stitch_long_exposure_ratio_read(IspDev) * \
                hi_isp_frame_stitch_med_exposure_ratio_read(IspDev) * hi_isp_frame_stitch_short_exposure_ratio_read(IspDev) >> (RATIO_SHIFT * 2);
        }
        else
        {}

        u32WDRExpRatio = CLIP3(u32WDRExpRatio, 0x40, 0xFFFFFF);
        u32BlackLevel = hi_isp_frame_stitch_black_level_out_read(IspDev);
        u32GammaFeDlAlpha = hi_isp_gamma_fe_alpha_read(IspDev);

        S64NRThreshTmp = ((HI_U64)u32WDRLongThresh << 8) * 0x40 / u32WDRExpRatio + u32BlackLevel;

        /* y = x * 2^32 / (alpha * 2^20 + (4096-alpha) * x) */
        S64NRThreshDiv = (((HI_U64)u32GammaFeDlAlpha << 20) + (4096 - u32GammaFeDlAlpha) * S64NRThreshTmp);
        S64NRThreshTmp = (S64NRThreshTmp << 32) / DIV_0_TO_1(S64NRThreshDiv);
        S64NRThreshTmp = S64NRThreshTmp >> 4;
        pstNr->u16NpExpThresh = MIN2(S64NRThreshTmp, 0xFFFF);

        pstNr->u8NpShortRatio = HI_FW_NOISE_SHORT_RATIO_HDR;
        pstNr->u8NpLongRatio = (HI_FW_NOISE_SHORT_RATIO_HDR * u32WDRExpRatio) >> RATIO_SHIFT;
        pstNr->u8SnrThreshLong = pstNr->u8SnrThreshShort;
    }
    else if (IS_LINEAR_MODE(u8SnsWDRMode)|| IS_BUILT_IN_WDR_MODE(u8SnsWDRMode))
    {
        pstNr->u8NpShortRatio = HI_FW_NOISE_SHORT_RATIO_LIN;
        pstNr->u8NpLongRatio = HI_FW_NOISE_LONG_RATIO_LIN;
        pstNr->u16NpExpThresh = HI_FW_NOISE_EXP_THRESH_LIN;
        pstNr->u8SnrThreshLong = pstNr->u8SnrThreshShort;
    }
    return HI_SUCCESS;
}

HI_S32 NrProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;

    NR_GET_CTX(IspDev, pstNr);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----NR INFO-------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s" "%8s"     "%8s"      "%8s"      "%8s\n",
        "En", "ManuEn", "ThreshS", "ThreshL", "Offset");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%8d"     "%8d"     "%8d"     "%8d\n",
        hi_isp_nr_enable_read(IspDev),
        hi_ext_system_manual_nr_read(IspDev),
        pstNr->u8SnrThreshShort,
        pstNr->u8SnrThreshLong,
        pstNr->u8NpOffset);

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}

HI_S32 ISP_NrInit(ISP_DEV IspDev)
{
    ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL;

    NR_GET_CTX(IspDev, pstNr);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);

    NrRegsDefault(IspDev);
    NrExtRegsDefault(IspDev);
    NrRegsInitialize(IspDev);
    NrExtRegsInitialize(IspDev);

    NrInitialize(IspDev, pstNr, pstSnsDft, pstSnsBlackLevel);
    pstNr->u32Iso = 100;

    return HI_SUCCESS;
}

HI_S32 NrGetFrameInfo(ISP_DEV IspDev, FRAME_SUPPLEMENT_INFO_S* pstIspFrame)
{
    ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;
    NR_GET_CTX(IspDev, pstNr);
 
    if (HI_NULL == pstIspFrame)
    {
        return HI_FAILURE;
    }

    pstIspFrame->u32IspNrStrength[0]       =  pstNr->u8SnrThreshShort;
    pstIspFrame->u32IspNrStrength[1]       =  pstNr->u8SnrThreshLong;
    pstIspFrame->u32IspNrStrength[2]       =  pstNr->u8SnrThreshLong;
    pstIspFrame->u32IspNrStrength[3]       =  pstNr->u8SnrThreshLong;

    return HI_SUCCESS;
}


HI_S32 ISP_NrRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;
    HI_U32 u32Iso;
    
    NR_GET_CTX(IspDev, pstNr);
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    u32Iso = pstNr->u32Iso;
    pstNr->u32Iso = pstIspCtx->stLinkage.u32Iso;

    /* calculate every two interrupts */
    if ((1 != pstIspCtx->u32FrameCnt % 2) && (HI_TRUE != pstIspCtx->stLinkage.bCapState) && (SNAP_TYPE_SYNC != pstIspCtx->stLinkage.enSnapType) && (SNAP_TYPE_SYNC_PRO != pstIspCtx->stLinkage.enSnapType))
    {
        return HI_SUCCESS;
    }

    if ((0 == pstIspCtx->stLinkage.u32UpdatePos) || (HI_TRUE != pstIspCtx->stLinkage.bCapState))
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u16NpExpThresh = pstNr->u16NpExpThresh;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8NpShortRatio = pstNr->u8NpShortRatio;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8NpLongRatio = pstNr->u8NpLongRatio;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8SinterThreshShort = pstNr->u8SnrThreshShort;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8SinterThreshLong = pstNr->u8SnrThreshLong;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8NpOffset = pstNr->u8NpOffset;
        ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1NrCfg = 1;
    }

    NrReadExtregs(IspDev);

    /* calc NR thresh */
    if (HI_TRUE == pstIspCtx->stLinkage.bCapState)
    {
        NrThreshCalculate(IspDev, pstNr, pstIspCtx->stLinkage.u32Iso,
            pstIspCtx->u8SnsWDRMode, 64);
    }
    else
    {
        NrThreshCalculate(IspDev, pstNr, u32Iso,
            pstIspCtx->u8SnsWDRMode, 64);
    }

    /* calc NP parameter */
    NrNPCal(IspDev, pstNr, pstIspCtx->u8SnsWDRMode);

    /* add manual nr by lt */
    if (1 == hi_ext_system_manual_nr_read(IspDev))
    {
        if (IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
        {
            pstNr->u8SnrThreshShort = pstNr->u8SnrThreshTargetShort;
            pstNr->u8SnrThreshLong = pstNr->u8SnrThreshTargetLong;
        }
        else
        {
            pstNr->u8SnrThreshShort = pstNr->u8SnrThreshTargetShort;
            pstNr->u8SnrThreshLong = pstNr->u8SnrThreshTargetShort;
        }
    }

    /*when snap frame, parameters should set current frame */
    if ((1 == pstIspCtx->stLinkage.u32UpdatePos) || (HI_TRUE == pstIspCtx->stLinkage.bCapState))
    {
    	((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u16NpExpThresh = pstNr->u16NpExpThresh;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8NpShortRatio = pstNr->u8NpShortRatio;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8NpLongRatio = pstNr->u8NpLongRatio;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8SinterThreshShort = pstNr->u8SnrThreshShort;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8SinterThreshLong = pstNr->u8SnrThreshLong;
        ((ISP_REG_CFG_S *)pRegCfg)->stNrRegCfg.u8NpOffset = pstNr->u8NpOffset;
        ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1NrCfg = 1;
    }
     
    return HI_SUCCESS;
}

HI_S32 ISP_NrCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_NrInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            NrProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        case ISP_FRAMEINFO_GET:
            NrGetFrameInfo(IspDev, (FRAME_SUPPLEMENT_INFO_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_NrExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterNr(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_NR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_NrInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_NrRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_NrCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_NrExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

