/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sharpen.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include "isp_config.h"
#include "isp_sharpen_mem_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_SHARPEN_S
{
    /* Sharpening inputs */
    HI_U8   u8SharpenEnabled;
    HI_U8   u8ManualSharpenEnabled;
    HI_U8   u8SharpenDTarget;
    HI_U8   u8SharpenUdTarget;
    HI_U8   u8RGBSharpenTarget;
    HI_U8   u8GeStrength;
    HI_U16  u16LumThresh;
    HI_U8   u8LutCoring;
    HI_U8   u8LutStrength;
    HI_U8   u8LutMagnitude;

    /* Public */
    HI_U8   u8SharpenMin;
} ISP_SHARPEN_S;

ISP_SHARPEN_S g_astSharpenCtx[ISP_MAX_DEV_NUM] = {{0}};
#define SHARPEN_GET_CTX(dev, pstCtx)   pstCtx = &g_astSharpenCtx[dev]

/* RGBsharpening updata for LUT */
static HI_VOID SharpenFunUpdate(ISP_DEV IspDev)
{
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);

    HI_S32 s32Core = hi_ext_system_sharpening_lut_coring_read();
    HI_S32 s32Strength = hi_ext_system_sharpening_lut_strength_read();
    HI_S32 s32Magnitude = hi_ext_system_sharpening_lut_magnitude_read();
    
    if ((pstSharpen->u8LutCoring != s32Core) || (pstSharpen->u8LutStrength != s32Strength) || (pstSharpen->u8LutMagnitude != s32Magnitude))
    {
        HI_S32 i;
        HI_S32 s32Lut;
        HI_S32 s32LutMax = 0;
        HI_S32 s32Lut_t[256 / 2];
        HI_S32 s32Index = 0;
        for(i = 0; i < 256; i+=2)    {
            s32Lut = 512 * 128/math_exp2(-(-i * s32Magnitude + (s32Core + 256)*((HI_S32) log2_int_to_fixed(i, 8, 0) - 8 * 256) / 256), 8, 0);
            s32Lut_t[s32Index] = s32Lut;
            if (s32LutMax < s32Lut)
                s32LutMax = s32Lut;

            s32Index = s32Index + 1;
        }
        s32Index = 0;
        HI_S32 s32Index_first;
        HI_S32 s32Index_second;
        HI_S32 s32Coring;
        if (hi_isp_sharpen_coring_read() == 0)    {
            s32Index_first = 256 + 128;
            s32Index_second = 256 + 127;
            s32Coring = 1;
        }
        else    {
            s32Index_first = 128;
            s32Index_second = 127;
            s32Coring = 0;
        }
        for(i = 0; i < 256; i+=2)    {
            s32Lut = s32Lut_t[s32Index];
            s32Lut = s32Lut * s32Strength / s32LutMax;
            if (s32Lut > 127)
                s32Lut = 127;

            s32Lut_t[s32Index] = s32Lut;
            /* Update LUT for ISP */
            hi_sharpen_mem_array_data_write(s32Index_first + s32Index, 128 + s32Lut);  // Coring = 0
            hi_sharpen_mem_array_data_write(s32Index_second - s32Index, 128 - s32Lut); // Coring = 0
            s32Index = s32Index + 1;
        }
        hi_isp_sharpen_coring_write(s32Coring);
        pstSharpen->u8LutCoring = s32Core;
        pstSharpen->u8LutStrength = s32Strength;
        pstSharpen->u8LutMagnitude = s32Magnitude;
    }
    
}

static HI_VOID SharpenExtRegsDefault(HI_VOID)
{
    /* initial register of sharpen */
    hi_ext_system_sharpening_enable_write(HI_EXT_SYSTEM_SHARPENING_ENABLE_DEFAULT);
    hi_ext_system_manual_sharpening_write(HI_EXT_SYSTEM_MANUAL_SHARPENING_DEFAULT);
    hi_ext_system_sharpening_target_write(HI_EXT_SYSTEM_SHARPENING_TARGET_DEFAULT);
    hi_ext_system_RGBsharpening_target_write(HI_EXT_SYSTEM_RGBSHARPENING_TARGET_DEFAULT);
    hi_ext_system_sharpening_ud_target_write(HI_EXT_SYSTEM_SHARPENING_UD_TARGET_DEFAULT);
    hi_ext_system_sharpening_lut_coring_write(HI_EXT_SYSTEM_SHARPENING_LUT_CORING_DEFAULT);
    hi_ext_system_sharpening_lut_strength_write(HI_EXT_SYSTEM_SHARPENING_LUT_STRENGTH_DEFAULT);
    hi_ext_system_sharpening_lut_magnitude_write(HI_EXT_SYSTEM_SHARPENING_LUT_MAGNITUDE_DEFAULT);

    return;
}

static HI_VOID SharpenRegsDefault(HI_VOID) 
{
    return;
}

static HI_VOID SharpenExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U8 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    if (pstSnsDft->stAgcTbl.bValid)
    {
        for (i = 0; i < 16; i++)
        {
            hi_ext_system_agc_table_sharp_alt_d_write(i, pstSnsDft->stAgcTbl.au8SharpenAltD[i]);
            hi_ext_system_agc_table_sharp_alt_ud_write(i, pstSnsDft->stAgcTbl.au8SharpenAltUd[i]);
            hi_ext_system_agc_table_demosaic_lum_thresh_write(i, pstSnsDft->stAgcTbl.au8DemosaicLumThresh[i]);
            hi_ext_system_agc_table_ge_strength_write(i, pstSnsDft->stAgcTbl.au8GeStrength[i]);
            hi_ext_system_agc_table_RGBsharp_strength_write(i, pstSnsDft->stAgcTbl.au8SharpenRGB[i]);
        }
    }
    
    if(pstSnsDft->stRgbSharpen.bValid) 
    {
        hi_ext_system_sharpening_lut_coring_write(pstSnsDft->stRgbSharpen.u8LutCore);
        hi_ext_system_sharpening_lut_strength_write(pstSnsDft->stRgbSharpen.u8LutStrength);     
        hi_ext_system_sharpening_lut_magnitude_write(pstSnsDft->stRgbSharpen.u8LutMagnitude);
    }
  return;
}

static HI_VOID SharpenRegsInitialize(ISP_DEV IspDev)
{   
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);

    pstSharpen->u8LutCoring = 0;
    pstSharpen->u8LutStrength = 0;
    pstSharpen->u8LutMagnitude = 0;

    hi_isp_sharpen_enable_write(1);

    return;
}

static HI_S32 SharpenReadExtregs(ISP_DEV IspDev)
{
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);
    
    pstSharpen->u8SharpenEnabled = hi_ext_system_sharpening_enable_read();
    pstSharpen->u8ManualSharpenEnabled = hi_ext_system_manual_sharpening_read();
    pstSharpen->u8SharpenDTarget = hi_ext_system_sharpening_target_read();
    pstSharpen->u8SharpenUdTarget = hi_ext_system_sharpening_ud_target_read();
    pstSharpen->u8RGBSharpenTarget = hi_ext_system_RGBsharpening_target_read();

    return 0;
}

static __inline HI_U8 SharpenOffsetCalculate(
    const HI_U8 u8Y2,
    const HI_U8 u8Y1,
    const HI_U32 u32X2,
    const HI_U32 u32X1,
    const HI_U32 u32Iso
    )    
{
    HI_U8 u8Offset =  (ABS((HI_S32)u8Y2 - u8Y1)*(u32Iso - u32X1) +
        (u32X2 - u32X1)/2)/ DIV_0_TO_1(u32X2 - u32X1);
    
    return u8Offset;
}

HI_S32 SharpenProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----SHARPEN INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s" "%8s"     "%8s"  "%8s"   "%8s"    "%8s"    "%8s\n",
        "En", "ManuEn", "SpD", "SpUd", "SpRGB", "LumTh", "GeSt");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%8d"     "%8d"  "%8d"   "%8d"    "%8d"    "%8d\n",
        pstSharpen->u8SharpenEnabled,
        pstSharpen->u8ManualSharpenEnabled,
        pstSharpen->u8SharpenDTarget,
        pstSharpen->u8SharpenUdTarget,
        pstSharpen->u8RGBSharpenTarget,
        pstSharpen->u16LumThresh,
        pstSharpen->u8GeStrength);

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_SharpenInit(ISP_DEV IspDev)
{
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);
    
    SharpenRegsDefault();
    SharpenExtRegsDefault();
    SharpenRegsInitialize(IspDev);
    SharpenExtRegsInitialize(IspDev);

    pstSharpen->u8SharpenEnabled = 1;
    pstSharpen->u8SharpenDTarget = HI_EXT_SYSTEM_SHARPENING_TARGET_DEFAULT;
    pstSharpen->u8SharpenUdTarget = HI_EXT_SYSTEM_SHARPENING_UD_TARGET_DEFAULT;
    pstSharpen->u8RGBSharpenTarget = HI_EXT_SYSTEM_RGBSHARPENING_TARGET_DEFAULT;
    pstSharpen->u8ManualSharpenEnabled = 0;
    pstSharpen->u16LumThresh = 96;
    
    return HI_SUCCESS;
}

HI_S32 ISP_SharpenRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    HI_U8  i;
    HI_U8  u8Y1 = 0;
    HI_U8  u8Y2 = 0;
    HI_U8  u8Offsetd = 0;
    HI_U8  u8Y3 = 0;
    HI_U8  u8Y4 = 0;
    HI_U8  u8OffsetUd = 0;
    HI_U8  u8Y5 = 0;
    HI_U8  u8Y6 = 0;
    HI_U8  u8Offset = 0;
    HI_U32 u32Iso = 0;
    HI_U32 u32ISO1 = 0;
    HI_U32 u32ISO2 = 0;

    HI_U32 au32IsoLut[16] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};

    SHARPEN_GET_CTX(IspDev, pstSharpen);
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

    SharpenReadExtregs(IspDev);

    SharpenFunUpdate(IspDev);
    
    if (!pstSharpen->u8SharpenEnabled)
    {
        pstSharpen->u8SharpenDTarget = 0;
        pstSharpen->u8SharpenUdTarget = 0;
        pstSharpen->u8RGBSharpenTarget = 0;
        
        pstRegCfg->stSharpenRegCfg.u8SharpAltD = 0;
        pstRegCfg->stSharpenRegCfg.u8SharpAltUd = 0;
        pstRegCfg->stSharpenRegCfg.u8RGBsharpshrength = 0;

        pstRegCfg->unKey.bit1SharpenCfg = 1;
        return HI_SUCCESS;
    }

    /* sharpen strength linkage with the iso calculated by ae */
    u32Iso = pstIspCtx->stLinkage.u32Iso;

    {  
        if (u32Iso >= au32IsoLut[15])
        {           
            u8Y1 = hi_ext_system_agc_table_sharp_alt_d_read(15);
            u8Y2 = u8Y1;
            u8Offsetd = 0;            
                
            u8Y3 = hi_ext_system_agc_table_sharp_alt_ud_read(15);
            u8Y4 = u8Y3;
            u8OffsetUd= 0;
            
            u8Y5 = hi_ext_system_agc_table_RGBsharp_strength_read(15);
            u8Y6 = u8Y5;
            u8Offset = 0;
            
            pstSharpen->u16LumThresh = hi_ext_system_agc_table_demosaic_lum_thresh_read(15);
            pstSharpen->u8GeStrength = hi_ext_system_agc_table_ge_strength_read(15);
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

            u8Y1 = hi_ext_system_agc_table_sharp_alt_d_read(i-1);
            u8Y2 = hi_ext_system_agc_table_sharp_alt_d_read(i);
            u8Offsetd = SharpenOffsetCalculate(u8Y2,u8Y1,u32ISO2,u32ISO1,u32Iso);            
                
            u8Y3 = hi_ext_system_agc_table_sharp_alt_ud_read(i-1);
            u8Y4 = hi_ext_system_agc_table_sharp_alt_ud_read(i);
            u8OffsetUd= SharpenOffsetCalculate(u8Y4,u8Y3,u32ISO2,u32ISO1,u32Iso);
            
            u8Y5 = hi_ext_system_agc_table_RGBsharp_strength_read(i-1);
            u8Y6 = hi_ext_system_agc_table_RGBsharp_strength_read(i);
            u8Offset = SharpenOffsetCalculate(u8Y6,u8Y5,u32ISO2,u32ISO1,u32Iso);
            
            pstSharpen->u16LumThresh = hi_ext_system_agc_table_demosaic_lum_thresh_read(i-1);
            pstSharpen->u8GeStrength = hi_ext_system_agc_table_ge_strength_read(i-1);
        }

        if (u8Y1 > u8Y2)
        {
            pstSharpen->u8SharpenDTarget = u8Y1 - u8Offsetd;
        }
        else
        {
            pstSharpen->u8SharpenDTarget = u8Y1 + u8Offsetd;
        }        

        if (u8Y3 > u8Y4)
        {
            pstSharpen->u8SharpenUdTarget = u8Y3 - u8OffsetUd;
        }
        else
        {
            pstSharpen->u8SharpenUdTarget = u8Y3 + u8OffsetUd;
        }

        if (u8Y5 > u8Y6)
        {
            pstSharpen->u8RGBSharpenTarget = u8Y5 - u8Offset;
        }
        else
        {
            pstSharpen->u8RGBSharpenTarget = u8Y5 + u8Offset;
        }

        
        pstRegCfg->stSharpenRegCfg.u8SharpAltD = pstSharpen->u8SharpenDTarget;
        pstRegCfg->stSharpenRegCfg.u8SharpAltUd = pstSharpen->u8SharpenUdTarget;
        pstRegCfg->stSharpenRegCfg.u8RGBsharpshrength = pstSharpen->u8RGBSharpenTarget;
        pstRegCfg->stSharpenRegCfg.u16LumThresh = pstSharpen->u16LumThresh;
        pstRegCfg->unKey.bit1SharpenCfg = 1;
        
        pstRegCfg->stGeRegCfg.u8GeStrength = pstSharpen->u8GeStrength;
        pstRegCfg->unKey.bit1GeCfg = 1;
    }    

    if (pstSharpen->u8ManualSharpenEnabled)
    {
        pstSharpen->u8SharpenDTarget = hi_ext_system_sharpening_target_read();
        pstSharpen->u8SharpenUdTarget = hi_ext_system_sharpening_ud_target_read();
        pstSharpen->u8RGBSharpenTarget = hi_ext_system_RGBsharpening_target_read();
        
        pstRegCfg->stSharpenRegCfg.u8SharpAltD = pstSharpen->u8SharpenDTarget;
        pstRegCfg->stSharpenRegCfg.u8SharpAltUd = pstSharpen->u8SharpenUdTarget;
        pstRegCfg->stSharpenRegCfg.u8RGBsharpshrength = pstSharpen->u8RGBSharpenTarget;
        pstRegCfg->unKey.bit1SharpenCfg = 1;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SharpenCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_SharpenInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            SharpenProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_SharpenExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterSharpen(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SHARPEN;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_SharpenInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_SharpenRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_SharpenCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_SharpenExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

