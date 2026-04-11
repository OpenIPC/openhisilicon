/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_drc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HISTOGRAM_BANDS 5

#define DRC_STRENGTH_MAXIMUM 255
#define DRC_AVE_FRAME_QUANTITY 8

typedef struct hiISP_DRC_HIST_S
{
    HI_S32  s32HistDark;
    HI_S32  s32HistDarkShift;
    HI_U16  u16ExpRatio;
    HI_U16  au16HistValue[HISTOGRAM_BANDS];
} ISP_DRC_HIST_S;

#define ASYMMETRY_LUT_SIZE 33
typedef struct hiISP_DRC_S
{
    /* Public */
    HI_BOOL bManDrc;
    HI_U32  u32Strength;
    HI_U32  u32StrengthTarget;

    HI_U8   u8Asymmetry;
    HI_U8   u8BrightEnhance;
    HI_U32  au32AsymmetryLUT[ASYMMETRY_LUT_SIZE];

    ISP_DRC_HIST_S stDrcHist;

    /* Private */
    HI_U32  u32SturctSize;
} ISP_DRC_S;

ISP_DRC_S g_astDrcCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DRC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDrcCtx[dev]


void GenerateAsymmetry(ISP_DRC_S *pstDrc)
{
    HI_S32 Asymmetry = pstDrc->u8Asymmetry;
    HI_S32 SecondPole = pstDrc->u8BrightEnhance;
    
    double x  = ((double)(Asymmetry)+1)/257 * 2 - 1; 
    int ai = (int)(0.5 + 255 * (1- 1/(1000*x*x*x) + x - ((x >= 0)*2))); 
    double as = fabs((double)(ai) / 255); 
    double dp = (double)(SecondPole) / 255; 
    int ii; 
    int y = 0;
    for (ii = 0 ; ii <33 ; ++ii) 
    { 
        if(ai >= 0) 
        { 
            x = (double)(ii) / 32; 
        } 
        else 
        { 
            x = (double)(32 - ii) / 32; 
        } 

        y = 0;
        if (dp != 0.0)
        {
            y   =   (int)((dp+(1-dp)*pow((fabs(1-dp-x)/dp),   3))   * (x*(as+1)/(as+x) ) * 65535.0 + 0.5); 
            y = y < 0 ? 0 : y > 65535 ? 65535 : y; 
            if (ai < 0) 
            { 
                y = 65535 - y; 
            } 
        }
        pstDrc->au32AsymmetryLUT[ii] = y; 
    }
}

static HI_VOID DrcUpdateAsyLUT(ISP_DRC_S *pstDrc)
{
    int i = 0;
    HI_U8 u8Asymmetry     = hi_ext_system_drc_asymmetry_read();
    HI_U8 u8BrightEnhance = hi_ext_system_drc_bright_enhance_read();

    if ((pstDrc->u8Asymmetry != u8Asymmetry) || 
        (pstDrc->u8BrightEnhance!= u8BrightEnhance))
    {
        pstDrc->u8Asymmetry = u8Asymmetry;
        pstDrc->u8BrightEnhance = u8BrightEnhance;
        
        GenerateAsymmetry(pstDrc);
        hi_isp_gamma_mcu_priority_write(1);
        for(i = 0; i < ASYMMETRY_LUT_SIZE; i++)
        {
            hi_isp_drc_asymmetry_lut_write(i, pstDrc->au32AsymmetryLUT[i]);
        }
        hi_isp_gamma_mcu_priority_write(0);        
    }

    return;
}

static HI_VOID DrcExtRegsDefault(HI_VOID)
{
    /* initial register of drc */
    hi_ext_system_manual_drc_write(HI_EXT_SYSTEM_MANUAL_DRC_DEFAULT);
    hi_ext_system_drc_manual_strength_write(HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DEFAULT);
    hi_ext_system_drc_auto_strength_write(HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DEFAULT);
    
    hi_ext_system_drc_asymmetry_write(HI_EXT_SYSTEM_DRC_ASYMMETRY_DEFAULT);
    hi_ext_system_drc_bright_enhance_write(HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DEFAULT);
    
    return;
}

static HI_VOID DrcRegsDefault(HI_VOID) 
{
   
    return;
}

static HI_VOID DrcExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    HI_U8 u8WDRMode;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (IS_LINEAR_MODE(u8WDRMode))
    {
        hi_ext_system_drc_auto_strength_write(0x80);
    }
    else if (IS_BUILT_IN_WDR_MODE(u8WDRMode))
    {                    
        hi_ext_system_drc_auto_strength_write(0xA0);
    }
    else if (IS_FS_WDR_MODE(u8WDRMode))
    {
        hi_ext_system_drc_auto_strength_write(0x80);
    }
    else
    {

    }

    /* use default drc curve when both of Asymmetry and BrightEnhance are 0 */
    if ((0 == pstSnsDft->stDrc.u32Asymmetry) && (0 == pstSnsDft->stDrc.u32BrightEnhance))
    {
        hi_ext_system_drc_asymmetry_write(HI_EXT_SYSTEM_DRC_ASYMMETRY_DEFAULT);
        hi_ext_system_drc_bright_enhance_write(HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DEFAULT);
    }
    else
    {
        hi_ext_system_drc_asymmetry_write(pstSnsDft->stDrc.u32Asymmetry);
        hi_ext_system_drc_bright_enhance_write(pstSnsDft->stDrc.u32BrightEnhance);
    }

    return;
}

static HI_VOID DrcRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    HI_U8 u8WDRMode;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (IS_LINEAR_MODE(u8WDRMode))
    {
        /* Disable drc */
        hi_isp_drc_enable_write(0);

        /* drc parameters */ 
        hi_isp_drc_strength_write(0xFF);
    }
    else if (IS_BUILT_IN_WDR_MODE(u8WDRMode))
    {                    
        hi_isp_drc_enable_write(1);
        /* drc parameters */ 
        hi_isp_drc_strength_write(0xFF);
    }
    else if (IS_FS_WDR_MODE(u8WDRMode))
    {
        hi_isp_drc_enable_write(1); 
        hi_isp_drc_strength_write(0xFF);
     }
     else
     {

     }
     
    hi_isp_drc_enable_write(pstSnsDft->stDrc.bEnable);
    
    /* drc parameters */
    hi_isp_drc_variance_space_write(pstSnsDft->stDrc.u32VarianceSpace);
    hi_isp_drc_variance_intensity_write(pstSnsDft->stDrc.u32VarianceIntensity);
    hi_isp_drc_slope_max_write(pstSnsDft->stDrc.u32SlopeMax);
    hi_isp_drc_slope_min_write(pstSnsDft->stDrc.u32SlopeMin);
    hi_isp_drc_white_level_write(pstSnsDft->stDrc.u32WhiteLevel);
    hi_isp_drc_black_level_write(pstSnsDft->stDrc.u32BlackLevel);
    
    return;
}

static HI_VOID DrcInitialize(ISP_DEV IspDev)
{
    ISP_DRC_S *pstDrc = HI_NULL;
    DRC_GET_CTX(IspDev, pstDrc);

    DrcUpdateAsyLUT(pstDrc);

    pstDrc->bManDrc = HI_FALSE;
    pstDrc->u32SturctSize = sizeof(ISP_DRC_S);
    pstDrc->u32Strength = 128;
    pstDrc->u32StrengthTarget = 128;

    pstDrc->stDrcHist.u16ExpRatio = 64;
    pstDrc->stDrcHist.s32HistDark = 0;
    pstDrc->stDrcHist.s32HistDarkShift = 0x100;
    
    return;
}

static HI_S32 DrcReadExtregs(ISP_DEV IspDev)
{
    ISP_DRC_S *pstDrc = HI_NULL;
    
    DRC_GET_CTX(IspDev, pstDrc);

    pstDrc->bManDrc = hi_ext_system_manual_drc_read();
    if (pstDrc->bManDrc)
    {
        pstDrc->u32StrengthTarget = hi_ext_system_drc_manual_strength_read();
    }
    else
    {
        pstDrc->u32StrengthTarget = hi_ext_system_drc_auto_strength_read();
    }
    
    return 0;
}

static __inline HI_U8 DrcIsValid(ISP_DRC_S *pstDrc)
{
    return (pstDrc->u32SturctSize != sizeof(ISP_DRC_S));
}

static HI_U32  DrcCalHist(ISP_DRC_HIST_S *pstHist,
    HI_U8 u8SnsWDRMode, ISP_STAT_S *pstStat, HI_U32 u32ExpRatio)
{
    HI_S32 i;
    HI_U32 u32Hacc = 0;

    /* read hist value */
    pstHist->au16HistValue[0] = pstStat->stAeStat1.au16MeteringHist[0];
    pstHist->au16HistValue[1] = pstStat->stAeStat1.au16MeteringHist[1];
    pstHist->au16HistValue[2] = 0;
    pstHist->au16HistValue[3] = pstStat->stAeStat1.au16MeteringHist[2];
    pstHist->au16HistValue[4] = pstStat->stAeStat1.au16MeteringHist[3];
    
    for(i=0; i<HISTOGRAM_BANDS; ++i)
    {
        u32Hacc += pstHist->au16HistValue[i];
    }

    if (u32Hacc > 0xFFFF)
    {
        u32Hacc = 0xFFFF;
    }
    pstHist->au16HistValue[2] = 0xFFFF - u32Hacc;

    /* calculate histdark */
    pstHist->s32HistDark = (pstHist->s32HistDarkShift * (pstHist->au16HistValue[0] * 2 +
        pstHist->au16HistValue[1]) + 0x2000)/(pstHist->au16HistValue[2] + 0x2000);

    if (IS_LINEAR_MODE(u8SnsWDRMode))
    {
        pstHist->u16ExpRatio = 64;
    }
    else if (IS_WDR_MODE(u8SnsWDRMode))
    {
        pstHist->u16ExpRatio = u32ExpRatio;
    }
    else
    {
        /* unknow u8SnsWDRMode */
    } 

    return HI_SUCCESS;
}

#define EXP_RATIO_INPUT     1,   2,    4,   8,  12,  16
#define DRC_STRENGTH_TARGET 136, 136, 150, 170, 220, 255
//#define DRC_STRENGTH_TARGET 0,  64,  138, 180, 220, 255
#define EXP_RATIO_SHIFT 6

#define ISO_INPUT                 1,   2,   4,   8,  16,  32,  64, 128
#define DRC_STRENGTH_MAX_TARGET   0,   10,  20,  40, 60,  80, 100, 120
//#define DRC_STRENGTH_MAX_TARGET   0,   0,  0,  0, 0, 0, 0, 0

HI_S32 DrcCalStrength(ISP_DRC_S *pstDrc, HI_U32 u32Iso, HI_U32 u32ExpRatio,
    HI_U8 u8SnsWDRMode, ISP_STAT_S *pstStat)
{   
    HI_U32 u32Strength = 0;
    HI_U32 u32StrengthMax = 0;
    HI_S32 s32StrengthGlimit = 0;

    HI_U32 au32ExpRatioLut[]   = {EXP_RATIO_INPUT};
    HI_U32 au32DrcStrengthLut[] = {DRC_STRENGTH_TARGET};
    HI_U32 au32ISOLut[]   = {ISO_INPUT};
    HI_U32 au32DrcStrengthMaxLut[] = {DRC_STRENGTH_MAX_TARGET};

    if(0 != DrcIsValid(pstDrc))
    {
        return HI_FAILURE;
    }

    if (IS_FS_WDR_MODE(u8SnsWDRMode) || IS_BUILT_IN_WDR_MODE(u8SnsWDRMode))
    {
       
        u32StrengthMax = transition(&au32ISOLut[0], &au32DrcStrengthMaxLut[0], sizeof(au32ISOLut) / sizeof(HI_U32), (u32Iso << 7)/100, 7);

        u32Strength  = transition(&au32ExpRatioLut[0], &au32DrcStrengthLut[0], sizeof(au32ExpRatioLut) / sizeof(HI_U32), u32ExpRatio, EXP_RATIO_SHIFT);

        u32Strength  = (u32Strength * pstDrc->u32StrengthTarget) >> 7;

        u32Strength = (u32Strength > u32StrengthMax) ? (u32Strength - u32StrengthMax) : 0;

        //pstDrc->u32Strength = CLIP3(u32Strength, 0, 255);
        pstDrc->u32Strength = MIN2(u32Strength, 255);
    }
    else
    {
        DrcCalHist(&pstDrc->stDrcHist, u8SnsWDRMode, pstStat, u32ExpRatio);
        u32Strength = (pstDrc->u32StrengthTarget * pstDrc->stDrcHist.s32HistDark) /
            DIV_0_TO_1(pstDrc->stDrcHist.s32HistDarkShift); 
        u32Strength = u32Strength * pstDrc->stDrcHist.u16ExpRatio / (64*8);
        if (u32Strength > pstDrc->u32Strength)
        {
            u32Strength = pstDrc->u32Strength + 1;
        }
        else if (u32Strength < pstDrc->u32Strength)
        {
            u32Strength = pstDrc->u32Strength - 1;
        }

        /* Limit Drc strength according to digial gain to keep system gain constant */
        s32StrengthGlimit = 261  - (u32Iso * 64 * 396 / 100 >> 12);       
        if (s32StrengthGlimit < 0)
        {
            s32StrengthGlimit = 0;
        }
        if (u32Strength > s32StrengthGlimit)
        {
            u32Strength = s32StrengthGlimit;
        }

        if (u32Strength > 255)
        {
            u32Strength = 255;
        }
        
        pstDrc->u32Strength = u32Strength;        
    }
    
    return HI_SUCCESS;
}

HI_S32 DrcProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_DRC_S *pstDrc = HI_NULL;

    DRC_GET_CTX(IspDev, pstDrc);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----DRC INFO------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s" "%8s"     "%8s"    "%8s\n",
        "En", "ManuEn", "DrcSt", "DrcStTg");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%8d"     "%8d"    "%8d\n",
        hi_isp_drc_enable_read(),
        pstDrc->bManDrc,
        pstDrc->u32Strength,
        pstDrc->u32StrengthTarget);

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}

HI_S32 ISP_DrcInit(ISP_DEV IspDev)
{
    DrcRegsDefault();
    DrcExtRegsDefault();
    DrcRegsInitialize(IspDev);
    DrcExtRegsInitialize(IspDev);

    DrcInitialize(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_DrcRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_DRC_S *pstDrc = HI_NULL;
    ISP_CTX_S *pstIspCtx   = HI_NULL;    
    
    DRC_GET_CTX(IspDev, pstDrc);
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

#if 0
    /* calculate every two interrupts */
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }
#endif

    DrcReadExtregs(IspDev);

    DrcUpdateAsyLUT(pstDrc);

    if (pstDrc->bManDrc)
    {
        pstDrc->u32Strength = pstDrc->u32StrengthTarget;
        ((ISP_REG_CFG_S *)pRegCfg)->stDrcRegCfg.u32Strength = pstDrc->u32Strength;
        ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DrcCfg = 1;
        return HI_SUCCESS;
    }

    if((IS_LINE_WDR_MODE(pstIspCtx->u8SnsWDRMode)) && (ISP_FSWDR_LONG_FRAME_MODE == pstIspCtx->stLinkage.enFSWDRMode))
    {
        if(pstDrc->u32Strength > pstDrc->u32StrengthTarget)
        {
            pstDrc->u32Strength -= 1;
        }
        else if(pstDrc->u32Strength < pstDrc->u32StrengthTarget)
        {
            pstDrc->u32Strength += 1;
        }
        else
        {
        }
        //pstDrc->u32Strength = CLIP3(pstDrc->u32Strength, 0, 255);
        pstDrc->u32Strength = MIN2(pstDrc->u32Strength, 255);
    }
    else
    {
        DrcCalStrength(pstDrc, pstIspCtx->stLinkage.u32Iso, pstIspCtx->stLinkage.u32ExpRatio,
            pstIspCtx->u8SnsWDRMode, (ISP_STAT_S *)pStatInfo);
    }

    ((ISP_REG_CFG_S *)pRegCfg)->stDrcRegCfg.u32Strength = pstDrc->u32Strength;
    ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DrcCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_DrcCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    HI_U8 u8Mode = *((HI_U8 *)pValue);
    
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :            
            ISP_DrcInit(IspDev);            
            if (IS_LINEAR_MODE(u8Mode))
            {
                /* Disable drc */
                hi_isp_drc_enable_write(0);

                /* drc parameters */ 
                hi_isp_drc_strength_write(0);
                hi_ext_system_drc_auto_strength_write(128);
            }
            else if (IS_BUILT_IN_WDR_MODE(u8Mode))
            {
                hi_isp_drc_enable_write(1);
                /* drc parameters */ 
                hi_isp_drc_strength_write(0);
                hi_ext_system_drc_auto_strength_write(128);
            }
            else if (IS_FS_WDR_MODE(u8Mode))
            {
                hi_isp_drc_enable_write(1); 
                hi_isp_drc_strength_write(0);
                hi_ext_system_drc_auto_strength_write(128);
            }
            else
            {

            }
            break;
        case ISP_PROC_WRITE:
            DrcProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_DrcExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDrc(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DRC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DrcInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DrcRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DrcCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DrcExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

