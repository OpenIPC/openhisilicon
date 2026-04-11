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
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//#define HISTOGRAM_BANDS 5

#define DRC_STRENGTH_MAXIMUM 0x3FF

#define EXP_RATIO_INPUT     1,   2,    4,   8,  12,  16,   32,  64,  128,  256, 512
//#define DRC_STRENGTH_TARGET 100, 256, 384, 448, 512, 768, 896, 1023, 1023, 1023, 1023
#define DRC_STRENGTH_TARGET 100, 256, 384, 540, 660, 768, 896, 1023, 1023, 1023, 1023

#define EXP_RATIO_SHIFT 6

#define ISO_INPUT                 1,    2,   4,   8,   16,  32,  64, 128
//#define DRC_STRENGTH_MAX_TARGET   20,   40,  60,  80,  120, 160, 200, 324
#define DRC_STRENGTH_MAX_TARGET   20,   40,  60,  80,  100, 130, 200, 324


typedef struct hiISP_DRC_HIST_S
{
    HI_S32  s32HistDark;
    HI_S32  s32HistDarkShift;
    HI_U16  u16ExpRatio;
    HI_U16  au16HistValue[HIST_5_NUM];
} ISP_DRC_HIST_S;

typedef struct hiISP_DRC_S
{
    /* Public */
    HI_BOOL bManDrc;
	HI_BOOL bUpdateLut;
	HI_BOOL bUserTmEnable;

    HI_U32  u32Strength;
    HI_U32  u32StrengthTarget;
    HI_U64  u64DRCStrFilter;

    HI_U8   u8Asymmetry;
    HI_U8   u8BrightEnhance;
    HI_U32  au32AsymmetryLUT[DRC_TM_NODE_NUM];

    ISP_DRC_HIST_S stDrcHist;

    /* Private */
    HI_U32  u32SturctSize;
} ISP_DRC_S;

ISP_DRC_S g_astDrcCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DRC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDrcCtx[dev]

#define ISP_DRC_DATA_MAX    ((1<<20) - 1)

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
    for (ii = 0 ; ii <DRC_TM_NODE_NUM ; ++ii)
    {
        if(ai >= 0)
        {
            x = (double)(ii) / (DRC_TM_NODE_NUM - 1);
        }
        else
        {
            x = (double)((DRC_TM_NODE_NUM - 1) - ii) / (DRC_TM_NODE_NUM - 1);
        }

        y = 0;
        if (dp != 0.0)
        {
            y   =   (int)((dp+(1-dp)*pow((fabs(1-dp-x)/dp),   3))   * (x*(as+1)/(as+x) ) * ISP_DRC_DATA_MAX + 0.5);
            y = y < 0 ? 0 : y > ISP_DRC_DATA_MAX ? ISP_DRC_DATA_MAX : y;
            if (ai < 0)
            {
                y = ISP_DRC_DATA_MAX - y;
            }
        }
        pstDrc->au32AsymmetryLUT[ii] = y;
    }
}

static HI_VOID DrcUpdateAsyLUT(ISP_DEV IspDev, ISP_DRC_S *pstDrc)
{
    int i = 0;
    HI_U8 u8Asymmetry     = hi_ext_system_drc_asymmetry_read(IspDev);
    HI_U8 u8BrightEnhance = hi_ext_system_drc_bright_enhance_read(IspDev);
    //HI_U32 au32Lut[] = {0, 224720, 346557, 424670, 484695, 532840, 572584, 606097, 634841, 659833, 681794, 701298, 718738, 734467, 748731, 761763, 773707, 784731, 794931, 804420, 813292, 821612, 829436, 836828, 843828, 850500, 856852, 862932, 868772, 874405, 879837, 885109, 890229, 895229, 900101, 904893, 909597, 914237, 918829, 923365, 927877, 932373, 936861, 941342, 945830, 950342, 954878, 959446, 964062, 968718, 973438, 978222, 983062, 987990, 992998, 998086, 1003278, 1008566, 1013959, 1019463, 1025079, 1030815, 1036687, 1042687, 1048575};

    //if ((pstDrc->u8Asymmetry != u8Asymmetry) || (pstDrc->u8BrightEnhance!= u8BrightEnhance))
    {
        pstDrc->u8Asymmetry = u8Asymmetry;
        pstDrc->u8BrightEnhance = u8BrightEnhance;

        GenerateAsymmetry(pstDrc);
        hi_isp_drc_mcu_priority_write(IspDev, 1);
        for(i = 0; i < DRC_TM_NODE_NUM; i++)
        {
            hi_isp_drc_asymmetry_lut_write(IspDev, i, pstDrc->au32AsymmetryLUT[i]);
        }

        hi_isp_drc_mcu_priority_write(IspDev, 0);
    }

    return;
}

static HI_VOID DrcUpdateUserDefineLUT(ISP_DEV IspDev, ISP_DRC_S *pstDrc)
{
    int i = 0;

    hi_isp_drc_mcu_priority_write(IspDev, 1);
    for(i = 0; i < DRC_TM_NODE_NUM; i++)
    {
        hi_isp_drc_asymmetry_lut_write(IspDev, i, hi_ext_system_drc_tm_lut_read(IspDev, i));
    }
    hi_isp_drc_mcu_priority_write(IspDev, 0);

    return;
}

static HI_VOID DrcExtRegsDefault(ISP_DEV IspDev)
{
    /* initial register of drc */
    hi_ext_system_manual_drc_write(IspDev, HI_EXT_SYSTEM_MANUAL_DRC_DEFAULT);
    hi_ext_system_drc_manual_strength_write(IspDev, HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DEFAULT);
    hi_ext_system_drc_auto_strength_write(IspDev, HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DEFAULT);

    hi_ext_system_drc_asymmetry_write(IspDev, HI_EXT_SYSTEM_DRC_ASYMMETRY_DEFAULT);
    hi_ext_system_drc_bright_enhance_write(IspDev, HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DEFAULT);

    hi_ext_system_drc_user_tm_enable_write(IspDev, HI_EXT_SYSTEM_DRC_USER_TM_ENABLE_DEFAULT);
    hi_ext_system_drc_user_tm_update_write(IspDev, HI_EXT_SYSTEM_DRC_USER_TM_UPDATE_DEFAULT);

    return;
}

static HI_VOID DrcRegsDefault(ISP_DEV IspDev)
{

    return;
}

static HI_VOID DrcExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U32 i = 0;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_U32 au32Lut[] = {0, 224720, 346557, 424670, 484695, 532840, 572584, 606097, 634841, 659833, 681794, 701298, 718738, 734467, 748731, 761763, 773707, 784731, 794931, 804420, 813292, 821612, 829436, 836828, 843828, 850500, 856852, 862932, 868772, 874405, 879837, 885109, 890229, 895229, 900101, 904893, 909597, 914237, 918829, 923365, 927877, 932373, 936861, 941342, 945830, 950342, 954878, 959446, 964062, 968718, 973438, 978222, 983062, 987990, 992998, 998086, 1003278, 1008566, 1013959, 1019463, 1025079, 1030815, 1036687, 1042687, 1048575};

    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    hi_ext_system_manual_drc_write(IspDev, pstSnsDft->stDrc.bOpType);
    hi_ext_system_drc_manual_strength_write(IspDev, pstSnsDft->stDrc.u32ManualStrength);

    if (pstSnsDft->stDrc.u32AutoStrength)
    {
        hi_ext_system_drc_auto_strength_write(IspDev, pstSnsDft->stDrc.u32AutoStrength);
    }
    else
    {
        hi_ext_system_drc_auto_strength_write(IspDev, 0x200);
    }

    /* use default drc curve when both of Asymmetry and BrightEnhance are 0 */
    if ((0 == pstSnsDft->stDrc.u32Asymmetry) && (0 == pstSnsDft->stDrc.u32BrightEnhance))
    {
        hi_ext_system_drc_asymmetry_write(IspDev, HI_EXT_SYSTEM_DRC_ASYMMETRY_DEFAULT);
        hi_ext_system_drc_bright_enhance_write(IspDev, HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DEFAULT);
    }
    else
    {
        hi_ext_system_drc_asymmetry_write(IspDev, pstSnsDft->stDrc.u32Asymmetry);
        hi_ext_system_drc_bright_enhance_write(IspDev, pstSnsDft->stDrc.u32BrightEnhance);
    }

	for (i = 0; i < DRC_TM_NODE_NUM; i++)
	{
		hi_ext_system_drc_tm_lut_write(IspDev, i, au32Lut[i]);
	}

    return;
}

static HI_VOID DrcRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_U32 u32ExpRatio;
    HI_U32 u32DRCStr;
    HI_U32 u32DRCStrTarget;
    HI_U32 au32ExpRatioLut[]   = {EXP_RATIO_INPUT};
    HI_U32 au32DrcStrengthLut[] = {DRC_STRENGTH_TARGET};

    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stDrc.bOpType) /* Manual */
    {
        u32DRCStrTarget = pstSnsDft->stDrc.u32ManualStrength;
        u32DRCStr = u32DRCStrTarget;
    }
    else /* Auto */
    {
        u32DRCStrTarget = pstSnsDft->stDrc.u32AutoStrength;
        u32DRCStrTarget = u32DRCStrTarget ? u32DRCStrTarget : 0x200;

        u32ExpRatio = (HI_U64)pstSnsDft->stWDRAttr.au32ExpRatio[0] * pstSnsDft->stWDRAttr.au32ExpRatio[1] * pstSnsDft->stWDRAttr.au32ExpRatio[2] >> 12;
        if(u32ExpRatio == 0)
        {
            u32DRCStr = u32DRCStrTarget;
        }
        else
        {
            u32DRCStr = transition(&au32ExpRatioLut[0], &au32DrcStrengthLut[0], sizeof(au32ExpRatioLut) / sizeof(HI_U32), u32ExpRatio, EXP_RATIO_SHIFT);
            u32DRCStr  = (u32DRCStr * u32DRCStrTarget) >> 9;
        }
    }

    u32DRCStr = MIN2(u32DRCStr, DRC_STRENGTH_MAX);
    hi_isp_drc_strength_inroi_write(IspDev, u32DRCStr);
    hi_isp_drc_strength_outroi_write(IspDev, u32DRCStr);

    /* drc parameters in cmos.c */
    hi_isp_drc_enable_write(IspDev, pstSnsDft->stDrc.bEnable);
    hi_isp_drc_variance_space_write(IspDev, pstSnsDft->stDrc.u32VarianceSpace);
    hi_isp_drc_variance_intensity_write(IspDev, pstSnsDft->stDrc.u32VarianceIntensity);
    hi_isp_drc_slope_max_write(IspDev, pstSnsDft->stDrc.u32SlopeMax);
    hi_isp_drc_slope_min_write(IspDev, pstSnsDft->stDrc.u32SlopeMin);
    hi_isp_drc_white_level_write(IspDev, pstSnsDft->stDrc.u32WhiteLevel);
    hi_isp_drc_black_level_write(IspDev, pstSnsDft->stDrc.u32BlackLevel);

    hi_isp_drc_filter_mux_write(IspDev, pstSnsDft->stDrc.bFilterMux);
    hi_isp_drc_svariance_write(IspDev, pstSnsDft->stDrc.u32Svariance);
    hi_isp_drc_bright_pr_write(IspDev, pstSnsDft->stDrc.u32BrightPr);
    hi_isp_drc_contrast_write(IspDev, pstSnsDft->stDrc.u32Contrast);
    hi_isp_drc_dark_enh_write(IspDev, pstSnsDft->stDrc.u32DarkEnhance);

    return;
}

static HI_VOID DrcInitialize(ISP_DEV IspDev)
{
    ISP_DRC_S *pstDrc = HI_NULL;
    DRC_GET_CTX(IspDev, pstDrc);

    DrcUpdateAsyLUT(IspDev, pstDrc);

    pstDrc->bManDrc = HI_FALSE;
    pstDrc->u32SturctSize = sizeof(ISP_DRC_S);
    pstDrc->u32Strength = 0x200;
    pstDrc->u32StrengthTarget = 0x200;

    pstDrc->stDrcHist.u16ExpRatio = 64;
    pstDrc->stDrcHist.s32HistDark = 0;
    pstDrc->stDrcHist.s32HistDarkShift = 0x100;

    return;
}

static HI_S32 DrcReadExtregs(ISP_DEV IspDev)
{
    ISP_DRC_S *pstDrc = HI_NULL;

    DRC_GET_CTX(IspDev, pstDrc);

    pstDrc->bUserTmEnable = hi_ext_system_drc_user_tm_enable_read(IspDev);
    pstDrc->bUpdateLut = hi_ext_system_drc_user_tm_update_read(IspDev);

    pstDrc->bManDrc = hi_ext_system_manual_drc_read(IspDev);
    if (pstDrc->bManDrc)
    {
        pstDrc->u32StrengthTarget = hi_ext_system_drc_manual_strength_read(IspDev);
    }
    else
    {
        pstDrc->u32StrengthTarget = hi_ext_system_drc_auto_strength_read(IspDev);
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

    /* read hist value */
    for(i=0; i<HIST_5_NUM; ++i)
    {
        pstHist->au16HistValue[i] = pstStat->stAeStat1.au16MeteringHist[i];
    }

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
    HI_U32 u32Alpha = 0, u32Beta = 0;

    if(0 != DrcIsValid(pstDrc))
    {
        return HI_FAILURE;
    }

    if (IS_FS_WDR_MODE(u8SnsWDRMode) || IS_BUILT_IN_WDR_MODE(u8SnsWDRMode))
    {
        u32StrengthMax = transition(&au32ISOLut[0], &au32DrcStrengthMaxLut[0], sizeof(au32ISOLut) / sizeof(HI_U32), (u32Iso << 7)/100, 7);

        u32Strength  = transition(&au32ExpRatioLut[0], &au32DrcStrengthLut[0], sizeof(au32ExpRatioLut) / sizeof(HI_U32), u32ExpRatio, EXP_RATIO_SHIFT);

        /* here right shift 9 bits because the max DRC strength is 1023 */
        u32Strength  = (u32Strength * pstDrc->u32StrengthTarget) >> 9;

        u32Strength = (u32Strength > u32StrengthMax) ? (u32Strength - u32StrengthMax) : 0;

        pstDrc->u32Strength = MIN2(u32Strength, DRC_STRENGTH_MAX);
        pstDrc->u64DRCStrFilter = (HI_U64)pstDrc->u32Strength << 12;
    }
    else
    {
        DrcCalHist(&pstDrc->stDrcHist, u8SnsWDRMode, pstStat, u32ExpRatio);
        u32Strength = (pstDrc->u32StrengthTarget * pstDrc->stDrcHist.s32HistDark) /
            DIV_0_TO_1(pstDrc->stDrcHist.s32HistDarkShift);
        u32Strength = u32Strength * pstDrc->stDrcHist.u16ExpRatio / (64*8);

        /* Filtering in time */
        u32Alpha = 0x40;
        u32Beta = (HI_U32)0x1000 - u32Alpha;
        pstDrc->u64DRCStrFilter = (u32Alpha * u32Strength) + ((u32Beta * pstDrc->u64DRCStrFilter + (1<<11)) >> 12);
        u32Strength = (pstDrc->u64DRCStrFilter + (1<<11)) >> 12;

        /* Limit Drc strength according to digial gain to keep system gain constant */
        s32StrengthGlimit = 1047  - (u32Iso * 64 * 396 / 100 >> 10);
        if (s32StrengthGlimit < 0)
        {
            s32StrengthGlimit = 0;
        }
        if (u32Strength > s32StrengthGlimit)
        {
            u32Strength = s32StrengthGlimit;
        }

        if (u32Strength > DRC_STRENGTH_MAX)
        {
            u32Strength = DRC_STRENGTH_MAX;
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
        hi_isp_drc_enable_read(IspDev),
        pstDrc->bManDrc,
        pstDrc->u32Strength,
        pstDrc->u32StrengthTarget);

    pstProc->u32WriteLen += 1;

    return HI_SUCCESS;
}

HI_S32 ISP_DrcInit(ISP_DEV IspDev)
{
    DrcRegsDefault(IspDev);
    DrcExtRegsDefault(IspDev);
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
    HI_U32 u32DRCStrTmp = 0;

    DRC_GET_CTX(IspDev, pstDrc);
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    DrcReadExtregs(IspDev);

    if (pstDrc->bUserTmEnable)
    {
        if (pstDrc->bUpdateLut)
        {
            DrcUpdateUserDefineLUT(IspDev, pstDrc);
            hi_ext_system_drc_user_tm_update_write(IspDev, HI_FALSE);
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        if (pstDrc->bUpdateLut)
        {
            DrcUpdateAsyLUT(IspDev, pstDrc);
            hi_ext_system_drc_user_tm_update_write(IspDev, HI_FALSE);
        }
        else
        {
            /* do nothing */
        }
    }

    if (pstDrc->bManDrc)
    {
        pstDrc->u32Strength = pstDrc->u32StrengthTarget;
        ((ISP_REG_CFG_S *)pRegCfg)->stDrcRegCfg.u32Strength = pstDrc->u32Strength;
        ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DrcCfg = 1;
        pstDrc->u64DRCStrFilter = (HI_U64)pstDrc->u32Strength << 12;
        return HI_SUCCESS;
    }

    if((IS_LINE_WDR_MODE(pstIspCtx->u8SnsWDRMode)) && (ISP_FSWDR_LONG_FRAME_MODE == pstIspCtx->stLinkage.enFSWDRMode))
    {
        /* Filtering in time */
        u32DRCStrTmp = pstDrc->u32StrengthTarget;
        HI_U32 u32Alpha = 0x40;
        HI_U32 u32Beta = (HI_U32)0x1000 - u32Alpha;
        pstDrc->u64DRCStrFilter = (u32Alpha * u32DRCStrTmp) + ((u32Beta * pstDrc->u64DRCStrFilter + (1<<11)) >> 12);
        u32DRCStrTmp = (pstDrc->u64DRCStrFilter + (1<<11)) >> 12;
        pstDrc->u32Strength = MIN2(u32DRCStrTmp, DRC_STRENGTH_MAX);
    }
    else
    {
	    if (ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
	    {
	        if(IS_MAIN_CHANNAL(IspDev))
	        {
	            DrcCalStrength(pstDrc, pstIspCtx->stLinkage.u32Iso, pstIspCtx->stLinkage.u32ExpRatio,
	                pstIspCtx->u8SnsWDRMode, (ISP_STAT_S *)pStatInfo);
	        }
	        else
	        {
            	pstDrc->u32Strength = g_astDrcCtx[0].u32Strength;
                pstDrc->u64DRCStrFilter = (HI_U64)pstDrc->u32Strength << 12;
	        }
	    }
	    else
	    {
	        DrcCalStrength(pstDrc, pstIspCtx->stLinkage.u32Iso, pstIspCtx->stLinkage.u32ExpRatio,
	            pstIspCtx->u8SnsWDRMode, (ISP_STAT_S *)pStatInfo);
	    }
    }

    ((ISP_REG_CFG_S *)pRegCfg)->stDrcRegCfg.u32Strength = pstDrc->u32Strength;
    ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DrcCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_DrcCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    //HI_U8 u8Mode = *((HI_U8 *)pValue);

    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_DrcInit(IspDev);
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

