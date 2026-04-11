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
    Author      : 
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

#ifndef MAX2
#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN2
#define MIN2(a, b)          ((a) < (b) ? (a) : (b))
#endif

#ifndef CLIP3
#define CLIP3(x,min,max)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#endif

#define SHARPEN_LINE_THD1	(0)
#define SHARPEN_LINE_THD2	(255)

typedef struct hiISP_SHARPEN_S
{
    HI_U8   u8GeStrengthTarget;    
    /* Sharpening Bayer */
    HI_BOOL bShpAlgSel;
    HI_U8   u8SharpenEnabled;
    HI_U8   u8ManualSharpenEnabled;
    HI_U8   u8SharpenDTarget;
    HI_U8   u8SharpenUdTarget;
    HI_U16  u16LumThreshTarget;
    HI_U8   u8SharpenHFTarget;  
    HI_U8   u8SharpenMFTarget;  
    HI_U8   u8SharpenLFTarget;  
    HI_U8   u8SadAmplifierTarget;

    HI_U8   au8GeStrength[ISP_AUTO_STENGTH_NUM];
    HI_U8   au8SharpenD[ISP_AUTO_STENGTH_NUM];
    HI_U8   au8SharpenUd[ISP_AUTO_STENGTH_NUM];
    HI_U16  au16LumThresh[ISP_AUTO_STENGTH_NUM];    
    HI_U8   au8SharpenHF[ISP_AUTO_STENGTH_NUM];  
    HI_U8   au8SharpenMF[ISP_AUTO_STENGTH_NUM];  
    HI_U8   au8SharpenLF[ISP_AUTO_STENGTH_NUM];  
    HI_U8   au8SadAmplifier[ISP_AUTO_STENGTH_NUM];

    /* Sharpening Yuv */
    //HI_U8   u8SharpenYuvEnabled;
    HI_U8   u8ManualSharpenYuvEnabled;
    HI_BOOL bEnLowLumaShootTarget;    
    HI_U8   u8TextureStTarget; 		
    HI_U8   u8EdgeStTarget;  		    
    HI_U8   u8OverShootTarget;		    
    HI_U8   u8UnderShootTarget;
    HI_U8   u8TextureThdTarget;    		
    HI_U8   u8EdgeThdTarget;  		    
    HI_U8   u8JagCtrlTarget;		    
    HI_U8   u8SaltCtrlTarget;
    HI_U8   u8PepperCtrlTarget;		
    HI_U8   u8DetailCtrlTarget;		
    HI_U32  u32LumaThdTarget;                              
    HI_U32  u32LumaWgtTarget;
    HI_U16  u16SharpThdDeltaTarget;
    HI_U16  u16EdgeThdDeltaTarget;

    HI_BOOL abEnLowLumaShoot[ISP_AUTO_STENGTH_NUM];
    HI_U8   au8TextureSt[ISP_AUTO_STENGTH_NUM];  		
    HI_U8   au8EdgeSt[ISP_AUTO_STENGTH_NUM];  		    
    HI_U8   au8OverShoot[ISP_AUTO_STENGTH_NUM];		    
    HI_U8   au8UnderShoot[ISP_AUTO_STENGTH_NUM];		
    HI_U8   au8TextureThd[ISP_AUTO_STENGTH_NUM];		
    HI_U8   au8EdgeThd[ISP_AUTO_STENGTH_NUM];	 		
    HI_U8   au8JagCtrl[ISP_AUTO_STENGTH_NUM];		 	
    HI_U8   au8SaltCtrl[ISP_AUTO_STENGTH_NUM];		    
    HI_U8   au8PepperCtrl[ISP_AUTO_STENGTH_NUM];		
    HI_U8   au8DetailCtrl[ISP_AUTO_STENGTH_NUM];		

    HI_U32  au32LumaThd[ISP_AUTO_STENGTH_NUM];                              
    HI_U32  au32LumaWgt[ISP_AUTO_STENGTH_NUM]; 

    HI_U32  u32Iso;
} ISP_SHARPEN_S;

ISP_SHARPEN_S g_astSharpenCtx[ISP_MAX_DEV_NUM] = {{0}};
#define SHARPEN_GET_CTX(dev, pstCtx)   pstCtx = &g_astSharpenCtx[dev]


static HI_VOID SharpenExtRegsDefault(ISP_DEV IspDev)
{
    /* initial register of bayer sharpen */
    hi_ext_system_sharpening_enable_write(IspDev, HI_EXT_SYSTEM_SHARPENING_ENABLE_DEFAULT);
    hi_ext_system_manual_sharpening_write(IspDev, HI_EXT_SYSTEM_MANUAL_SHARPENING_DEFAULT);
    hi_ext_system_sharpening_alg_select_write(IspDev, HI_EXT_SYSTEM_SHARPENING_ALG_SELECT_DEFAULT);
    hi_ext_system_sharpening_d_target_write(IspDev, HI_EXT_SYSTEM_SHARPENING_TARGET_DEFAULT);
    hi_ext_system_sharpening_ud_target_write(IspDev, HI_EXT_SYSTEM_SHARPENING_UD_TARGET_DEFAULT);
    hi_ext_system_demosaic_lum_thresh_target_write(IspDev, HI_EXT_SYSTEM_DEMOSAIC_LUM_THRESH_TARGET_DEFAULT);
    hi_ext_system_sharpening_hf_target_write(IspDev, HI_EXT_SYSTEM_SHARPENING_HF_TARGET_DEFAULT);
    hi_ext_system_sharpening_mf_target_write(IspDev, HI_EXT_SYSTEM_SHARPENING_MF_TARGET_DEFAULT);
    hi_ext_system_sharpening_lf_target_write(IspDev, HI_EXT_SYSTEM_SHARPENING_LF_TARGET_DEFAULT);
    hi_ext_system_sharpening_sad_amplifier_write(IspDev, HI_EXT_SYSTEM_SHARPENING_SAD_AMPLIFIER_DEFAULT);

    /* initial register of yuv sharpen */
    //hi_ext_system_yuv_sharpening_enable_write(IspDev, HI_EXT_SYSTEM_YUV_SHARPENING_ENABLE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_TextureSt_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_TEXTUREST_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_EdgeSt_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_EDGEST_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_OverShoot_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_OVERSHOOT_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_UnderShoot_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_UNDERSHOOT_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_ENLOWLUMASHOOT_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_TextureThd_write(IspDev, HI_EXT_SYSTEM_MANUAL_SHARPENING_TEXTURETHD_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_EdgeThd_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_EDGETHD_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_JagCtrl_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_JAGCTRL_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_SaltCtrl_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_SALTCTRL_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_PepperCtrl_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_PEPPERCTRL_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_DetailCtrl_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_DETAILCTRL_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_LumaThd_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_LUMATHD_WRITE_DEFAULT);
    hi_ext_system_manual_yuv_sharpening_LumaWgt_write(IspDev, HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_LUMAWGT_WRITE_DEFAULT);

    return;
}

static HI_VOID SharpenRegsDefault(ISP_DEV IspDev) 
{
    // update sharpen width and height
    HI_U32 u32w, u32H;

    u32w = hi_isp_top_active_width_read(IspDev);
    u32H = hi_isp_top_active_height_read(IspDev);
    
    hi_isp_sharpen_width_write(IspDev, u32w - 1);
    hi_isp_sharpen_height_write(IspDev, u32H - 1);
  
    return;    
}

static HI_VOID SharpenExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U8 i,j;
    HI_U32 u32LumaThd;
    HI_U32 u32LumaWgt;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    if (pstSnsDft->stAgcTbl.bValid)
    {
        for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
        {
            hi_ext_system_agc_table_ge_strength_write(IspDev, i, pstSnsDft->stAgcTbl.au8GeStrength[i]);
        }
    }
    
    if (pstSnsDft->stBayerSharpen.bValid)
    {
        hi_ext_system_sharpening_alg_select_write(IspDev, pstSnsDft->stBayerSharpen.bShpAlgSel);
            
        for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
        {
            hi_ext_system_agc_table_sharp_alt_d_write(IspDev, i, pstSnsDft->stBayerSharpen.au8SharpenAltD[i]);
            hi_ext_system_agc_table_sharp_alt_ud_write(IspDev, i, pstSnsDft->stBayerSharpen.au8SharpenAltUd[i]);
            hi_ext_system_agc_table_demosaic_lum_thresh_write(IspDev, i, pstSnsDft->stBayerSharpen.au8DemosaicLumThresh[i]);
            hi_ext_system_agc_table_sharping_hf_write(IspDev, i, pstSnsDft->stBayerSharpen.au8SharpenHF[i]);
            hi_ext_system_agc_table_sharping_mf_write(IspDev, i, pstSnsDft->stBayerSharpen.au8SharpenMF[i]);
            hi_ext_system_agc_table_sharping_lf_write(IspDev, i, pstSnsDft->stBayerSharpen.au8SharpenLF[i]);
            hi_ext_system_agc_table_sharping_sad_amplifier_write(IspDev, i, pstSnsDft->stBayerSharpen.au8SadAmplifier[i]);
        }
    } 
    
    if(pstSnsDft->stYuvSharpen.bValid) 
    {
        for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
        {
            hi_ext_system_yuv_sharpening_TextureSt_write(IspDev, i, pstSnsDft->stYuvSharpen.au8TextureSt[i]);
            hi_ext_system_yuv_sharpening_EdgeSt_write(IspDev, i, pstSnsDft->stYuvSharpen.au8EdgeSt[i]);
            hi_ext_system_yuv_sharpening_OverShoot_write(IspDev, i, pstSnsDft->stYuvSharpen.au8OverShoot[i]);     
            hi_ext_system_yuv_sharpening_UnderShoot_write(IspDev, i, pstSnsDft->stYuvSharpen.au8UnderShoot[i]);
            hi_ext_system_yuv_sharpening_EnLowLumaShoot_write(IspDev, i, pstSnsDft->stYuvSharpen.abEnLowLumaShoot[i]);
            hi_ext_system_yuv_sharpening_TextureThd_write(IspDev, i, pstSnsDft->stYuvSharpen.au8TextureThd[i]);
            hi_ext_system_yuv_sharpening_EdgeThd_write(IspDev, i, pstSnsDft->stYuvSharpen.au8EdgeThd[i]);
            hi_ext_system_yuv_sharpening_JagCtrl_write(IspDev, i, pstSnsDft->stYuvSharpen.au8JagCtrl[i]);
            hi_ext_system_yuv_sharpening_SaltCtrl_write(IspDev, i, pstSnsDft->stYuvSharpen.au8SaltCtrl[i]);
            hi_ext_system_yuv_sharpening_PepperCtrl_write(IspDev, i, pstSnsDft->stYuvSharpen.au8PepperCtrl[i]);
            hi_ext_system_yuv_sharpening_DetailCtrl_write(IspDev, i, pstSnsDft->stYuvSharpen.au8DetailCtrl[i]);
            
            u32LumaThd = 0;
            u32LumaWgt = 0;
            for (j = 0; j < 4; j++)
            {
                u32LumaThd |= (pstSnsDft->stYuvSharpen.au8LumaThd[j][i] << j*8);
                u32LumaWgt |= (pstSnsDft->stYuvSharpen.au8LumaWgt[j][i] << j*8);
            }
            
            hi_ext_system_yuv_sharpening_LumaThd_write(IspDev, i, u32LumaThd);
            hi_ext_system_yuv_sharpening_LumaWgt_write(IspDev, i, u32LumaWgt);
            
        }
    }
    
  return;
}

static HI_VOID SharpenRegsInitialize(ISP_DEV IspDev)
{   
    //ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    //SHARPEN_GET_CTX(IspDev, pstSharpen);
  
	hi_isp_sharpen_cfg_enable_write(IspDev, 1);
	hi_isp_sharpen_enpixsel_write(IspDev, 1); 
    hi_isp_sharpen_update_mode_write(IspDev, 1);
    hi_isp_demosaic_min_d_strength_write(IspDev, HI_ISP_DEMOSAIC_MIN_D_STRENGTH_DEFAULT);
    hi_isp_demosaic_min_ud_strength_write(IspDev, HI_ISP_DEMOSAIC_MIN_UD_STRENGTH_DEFAULT);
        
    return;
}

static HI_S32 SharpenReadExtregs(ISP_DEV IspDev)
{
    HI_U32 i;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);
    
    pstSharpen->u8SharpenEnabled = hi_ext_system_sharpening_enable_read(IspDev);
    pstSharpen->u8ManualSharpenEnabled = hi_ext_system_manual_sharpening_read(IspDev); 
    pstSharpen->bShpAlgSel = hi_ext_system_sharpening_alg_select_read(IspDev);
    pstSharpen->u8SharpenDTarget = hi_ext_system_sharpening_d_target_read(IspDev);
    pstSharpen->u8SharpenUdTarget = hi_ext_system_sharpening_ud_target_read(IspDev);
    pstSharpen->u16LumThreshTarget = hi_ext_system_demosaic_lum_thresh_target_read(IspDev);
    pstSharpen->u8SharpenHFTarget = hi_ext_system_sharpening_hf_target_read(IspDev);  
    pstSharpen->u8SharpenMFTarget = hi_ext_system_sharpening_mf_target_read(IspDev);  
    pstSharpen->u8SharpenLFTarget = hi_ext_system_sharpening_lf_target_read(IspDev);  
    pstSharpen->u8SadAmplifierTarget = hi_ext_system_sharpening_sad_amplifier_read(IspDev);    

    //pstSharpen->u8SharpenYuvEnabled = hi_ext_system_yuv_sharpening_enable_read(IspDev);
    pstSharpen->u8ManualSharpenYuvEnabled = hi_ext_system_manual_yuv_sharpening_read(IspDev);
    pstSharpen->bEnLowLumaShootTarget = hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_read(IspDev);
    pstSharpen->u8TextureStTarget = hi_ext_system_manual_yuv_sharpening_TextureSt_read(IspDev);
    pstSharpen->u8EdgeStTarget = hi_ext_system_manual_yuv_sharpening_EdgeSt_read(IspDev);
    pstSharpen->u8OverShootTarget = hi_ext_system_manual_yuv_sharpening_OverShoot_read(IspDev);		    
    pstSharpen->u8UnderShootTarget = hi_ext_system_manual_yuv_sharpening_UnderShoot_read(IspDev); 
    pstSharpen->u8TextureThdTarget = hi_ext_system_manual_yuv_sharpening_TextureThd_read(IspDev);    		
    pstSharpen->u8EdgeThdTarget = hi_ext_system_manual_yuv_sharpening_EdgeThd_read(IspDev);  		    
    pstSharpen->u8JagCtrlTarget = hi_ext_system_manual_yuv_sharpening_JagCtrl_read(IspDev);		    
    pstSharpen->u8SaltCtrlTarget = hi_ext_system_manual_yuv_sharpening_SaltCtrl_read(IspDev);
	pstSharpen->u8PepperCtrlTarget = hi_ext_system_manual_yuv_sharpening_PepperCtrl_read(IspDev);		
	pstSharpen->u8DetailCtrlTarget = hi_ext_system_manual_yuv_sharpening_DetailCtrl_read(IspDev);		
	pstSharpen->u32LumaThdTarget = hi_ext_system_manual_yuv_sharpening_LumaThd_read(IspDev);                              
	pstSharpen->u32LumaWgtTarget = hi_ext_system_manual_yuv_sharpening_LumaWgt_read(IspDev); 

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        /* Ge */
        pstSharpen->au8GeStrength[i] = hi_ext_system_agc_table_ge_strength_read(IspDev, i);
        
        pstSharpen->au16LumThresh[i] = hi_ext_system_agc_table_demosaic_lum_thresh_read(IspDev, i);
        pstSharpen->au8SharpenD[i] = hi_ext_system_agc_table_sharp_alt_d_read(IspDev, i);
        pstSharpen->au8SharpenUd[i] = hi_ext_system_agc_table_sharp_alt_ud_read(IspDev, i);
        pstSharpen->au8GeStrength[i] = hi_ext_system_agc_table_ge_strength_read(IspDev, i);

        
        pstSharpen->au8SharpenHF[i] = hi_ext_system_agc_table_sharping_hf_read(IspDev, i);  
        pstSharpen->au8SharpenMF[i] = hi_ext_system_agc_table_sharping_mf_read(IspDev, i);  
        pstSharpen->au8SharpenLF[i] = hi_ext_system_agc_table_sharping_lf_read(IspDev, i);  
        pstSharpen->au8SadAmplifier[i] = hi_ext_system_agc_table_sharping_sad_amplifier_read(IspDev, i);

        pstSharpen->abEnLowLumaShoot[i] = hi_ext_system_yuv_sharpening_EnLowLumaShoot_read(IspDev, i);
        pstSharpen->au8TextureSt[i] = hi_ext_system_yuv_sharpening_TextureSt_read(IspDev, i);  	
        pstSharpen->au8EdgeSt[i] = hi_ext_system_yuv_sharpenng_EdgeSt_read(IspDev, i);  		
        pstSharpen->au8OverShoot[i] = hi_ext_system_yuv_sharpening_OverShoot_read(IspDev, i);		
        pstSharpen->au8UnderShoot[i] = hi_ext_system_yuv_sharpening_UnderShoot_read(IspDev, i);	
        pstSharpen->au8TextureThd[i] = hi_ext_system_yuv_sharpening_TextureThd_read(IspDev, i);	
        pstSharpen->au8EdgeThd[i] = hi_ext_system_yuv_sharpening_EdgeThd_read(IspDev, i);	 	
        pstSharpen->au8JagCtrl[i] = hi_ext_system_yuv_sharpening_JagCtrl_read(IspDev, i);		
        pstSharpen->au8SaltCtrl[i] = hi_ext_system_yuv_sharpening_SaltCtrl_read(IspDev, i);		
        pstSharpen->au8PepperCtrl[i] = hi_ext_system_yuv_sharpening_PepperCtrl_read(IspDev, i);	
        pstSharpen->au8DetailCtrl[i] = hi_ext_system_yuv_sharpening_DetailCtrl_read(IspDev, i);	
        pstSharpen->au32LumaThd[i] = hi_ext_system_yuv_sharpening_LumaThd_read(IspDev, i);
        pstSharpen->au32LumaWgt[i] = hi_ext_system_yuv_sharpening_LumaWgt_read(IspDev, i);                       
    }    

    return 0;
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
        "%8s" "%8s"     "%8s"  "%8s"   "%8s"    "%8s"   "%10s"      "%10s"      "%8s"     "%8s\n",
        "En", "ManuEn", "SpD", "SpUd", "LumTh", "GeSt", "SpTextSt", "SpEdgeSt", "OShoot", "UShoot");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%8d"     "%8d"  "%8d"   "%8d"    "%8d"   "%10d"   "%10d"   "%8d"   "%8d\n",
        pstSharpen->u8SharpenEnabled,
        pstSharpen->u8ManualSharpenEnabled,
        pstSharpen->u8SharpenDTarget,
        pstSharpen->u8SharpenUdTarget,
        pstSharpen->u16LumThreshTarget,
        pstSharpen->u8GeStrengthTarget,
        pstSharpen->u8TextureStTarget,
        pstSharpen->u8EdgeStTarget,
        pstSharpen->u8OverShootTarget,
        pstSharpen->u8UnderShootTarget);

    pstProc->u32WriteLen += 1;
  
    return HI_SUCCESS;
}

static __inline HI_S32 SharpenImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
    // update sharpen width and height
    
    hi_isp_sharpen_width_write(IspDev, pstRes->u16Width - 1);
    hi_isp_sharpen_height_write(IspDev, pstRes->u16Height - 1);

    return HI_SUCCESS;
}


HI_S32 ISP_SharpenInit(ISP_DEV IspDev)
{
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    SHARPEN_GET_CTX(IspDev, pstSharpen);
    
    SharpenRegsDefault(IspDev);
    SharpenExtRegsDefault(IspDev);
    SharpenRegsInitialize(IspDev);
    SharpenExtRegsInitialize(IspDev);

    pstSharpen->bShpAlgSel = HI_EXT_SYSTEM_SHARPENING_ALG_SELECT_DEFAULT;
    pstSharpen->u8SharpenEnabled = HI_EXT_SYSTEM_SHARPENING_ENABLE_DEFAULT;
    pstSharpen->u8ManualSharpenEnabled = HI_EXT_SYSTEM_MANUAL_SHARPENING_DEFAULT;    
    pstSharpen->u8SharpenDTarget = HI_EXT_SYSTEM_SHARPENING_TARGET_DEFAULT;
    pstSharpen->u8SharpenUdTarget = HI_EXT_SYSTEM_SHARPENING_UD_TARGET_DEFAULT;
    pstSharpen->u16LumThreshTarget = HI_EXT_SYSTEM_DEMOSAIC_LUM_THRESH_TARGET_DEFAULT;
    pstSharpen->u8SharpenHFTarget = HI_EXT_SYSTEM_SHARPENING_HF_TARGET_DEFAULT;  
    pstSharpen->u8SharpenMFTarget = HI_EXT_SYSTEM_SHARPENING_MF_TARGET_DEFAULT;  
    pstSharpen->u8SharpenLFTarget = HI_EXT_SYSTEM_SHARPENING_LF_TARGET_DEFAULT;  
    pstSharpen->u8SadAmplifierTarget = HI_EXT_SYSTEM_SHARPENING_SAD_AMPLIFIER_DEFAULT;
   
    //pstSharpen->u8SharpenYuvEnabled = HI_EXT_SYSTEM_YUV_SHARPENING_ENABLE_DEFAULT;
    pstSharpen->u8ManualSharpenYuvEnabled = HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_WRITE_DEFAULT;    
    pstSharpen->u8TextureStTarget = HI_EXT_SYSTEM_MANUAL_SHARPENING_DEFAULT; 		
    pstSharpen->u8EdgeStTarget = HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_EDGEST_WRITE_DEFAULT;  		    
    pstSharpen->u8OverShootTarget = HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_OVERSHOOT_WRITE_DEFAULT;		    
    pstSharpen->u8UnderShootTarget = HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_UNDERSHOOT_WRITE_DEFAULT;

    pstSharpen->u32Iso = 100;
    
    return HI_SUCCESS;
}

static __inline HI_S32 LinearInter(HI_S32 s32ISO, HI_S32 s32x0, HI_S32 s32y0, HI_S32 s32x1, HI_S32 s32y1)
{
    HI_S32 res;

    if ( s32ISO <= s32x0 ) return s32y0;
    if ( s32ISO >= s32x1 ) return s32y1;

    res = (s32y1 - s32y0) * (s32ISO - s32x0) / (s32x1 - s32x0) + s32y0;
    return res;
}

static HI_S32 CalcMulCoef(HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1, HI_S32 shift)
{
    HI_S32 res;
    if ( x0 == x1 )
    {
        res = 0;
    }
    else
    {
        res = ( (y1 - y0) << shift ) / ( x1 - x0);
    }
    return res;
}

HI_S32 ISP_Sharpen_GetRegCfg(ISP_DEV IspDev, HI_U32 s32ISO)
{
	HI_U32 i,j;
	HI_S32 index1, index2;
    HI_U8 u8WDRMode;
    HI_U8 lumaTargetTmp[4], lumaIdx1Tmp[4], lumaIdx2Tmp[4];
    
    const HI_S32 SharpenLutIso[ISP_AUTO_STENGTH_NUM] ={100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
    const HI_U16 SharpThdDelta[ISP_AUTO_STENGTH_NUM]={50,60,80,150,200,250,300,350,400,500,600,700,700,700,700,700};
    const HI_U16 EdgeThdDelta[ISP_AUTO_STENGTH_NUM]={30,50,80,100,150,200,250,300,350,400,450,500,500,500,500,500};   
    const HI_U16 SharpThdDeltaWDR[ISP_AUTO_STENGTH_NUM]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    const HI_U16 EdgeThdDeltaWDR[ISP_AUTO_STENGTH_NUM]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
   
    ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_SHARPEN_S *pstSharpenPara = HI_NULL;
	SHARPEN_GET_CTX(IspDev, pstSharpenPara);
    ISP_GET_CTX(IspDev, pstIspCtx);
    
    u8WDRMode = pstIspCtx->u8SnsWDRMode;
#if 0    
	if (HI_NULL == pstSharpenPara)
	{
		return HI_FAILURE;
	}
#endif
	index1 = ISP_AUTO_STENGTH_NUM - 1;
    
    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        if (s32ISO <= SharpenLutIso[i])
        {
            index1 = i;
            break;
        }
    }
	
    index2 = MAX2(index1 - 1, 0);

    pstSharpenPara->u8GeStrengthTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8GeStrength[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8GeStrength[index1]);

    /* Yuv Sharpen Auto */
	pstSharpenPara->bEnLowLumaShootTarget = pstSharpenPara->abEnLowLumaShoot[index1];
	 
	pstSharpenPara->u8TextureStTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8TextureSt[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8TextureSt[index1]);

    pstSharpenPara->u8EdgeStTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8EdgeSt[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8EdgeSt[index1]);

    pstSharpenPara->u8OverShootTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8OverShoot[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8OverShoot[index1]);

    pstSharpenPara->u8UnderShootTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8UnderShoot[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8UnderShoot[index1]);

    pstSharpenPara->u8TextureThdTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8TextureThd[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8TextureThd[index1]);
    
    pstSharpenPara->u8EdgeThdTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8EdgeThd[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8EdgeThd[index1]);     

    pstSharpenPara->u8JagCtrlTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8JagCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8JagCtrl[index1]);
     
    pstSharpenPara->u8SaltCtrlTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SaltCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SaltCtrl[index1]);  

    pstSharpenPara->u8PepperCtrlTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8PepperCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8PepperCtrl[index1]);
      
    pstSharpenPara->u8DetailCtrlTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8DetailCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8DetailCtrl[index1]); 
    
    pstSharpenPara->u32LumaThdTarget = 0;
    pstSharpenPara->u32LumaWgtTarget = 0;
    for (j = 0; j < 4; j++)
    {
        lumaIdx1Tmp[j] = (HI_U8) ((pstSharpenPara->au32LumaThd[index1] >> (8*j)) & 0xFF);
        lumaIdx2Tmp[j] = (HI_U8) ((pstSharpenPara->au32LumaThd[index2] >> (8*j)) & 0xFF);
        lumaTargetTmp[j] = LinearInter(s32ISO, \
                                        SharpenLutIso[index2], lumaIdx2Tmp[j], \
                                        SharpenLutIso[index1], lumaIdx1Tmp[j]);  
        pstSharpenPara->u32LumaThdTarget |=  lumaTargetTmp[j] << (8*j);

        lumaIdx1Tmp[j] = (HI_U8) ((pstSharpenPara->au32LumaWgt[index1] >> (8*j)) & 0xFF);
        lumaIdx2Tmp[j] = (HI_U8) ((pstSharpenPara->au32LumaWgt[index2] >> (8*j)) & 0xFF);
        lumaTargetTmp[j] = LinearInter(s32ISO, \
                                        SharpenLutIso[index2], lumaIdx2Tmp[j], \
                                        SharpenLutIso[index1], lumaIdx1Tmp[j]);  
        pstSharpenPara->u32LumaWgtTarget |=  lumaTargetTmp[j] << (8*j);
    }   

    if (!u8WDRMode)
    {
        pstSharpenPara->u16SharpThdDeltaTarget = LinearInter(s32ISO,
                                            SharpenLutIso[index2], SharpThdDelta[index2],
                                            SharpenLutIso[index1], SharpThdDelta[index1]); 


        pstSharpenPara->u16EdgeThdDeltaTarget = LinearInter(s32ISO,
                                            SharpenLutIso[index2], EdgeThdDelta[index2],
                                            SharpenLutIso[index1], EdgeThdDelta[index1]); 
    }
    else
    {
        pstSharpenPara->u16SharpThdDeltaTarget = LinearInter(s32ISO,
                                            SharpenLutIso[index2], SharpThdDeltaWDR[index2],
                                            SharpenLutIso[index1], SharpThdDeltaWDR[index1]); 
        
        
        pstSharpenPara->u16EdgeThdDeltaTarget = LinearInter(s32ISO,
                                            SharpenLutIso[index2], EdgeThdDeltaWDR[index2],
                                            SharpenLutIso[index1], EdgeThdDeltaWDR[index1]); 
    }
    
    /* Bayer Sharpen Auto */
    pstSharpenPara->u8SharpenDTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenD[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenD[index1]); 
    
    pstSharpenPara->u8SharpenUdTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenUd[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenUd[index1]); 
    
    pstSharpenPara->u16LumThreshTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au16LumThresh[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au16LumThresh[index1]);
    
    pstSharpenPara->u8SharpenHFTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenHF[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenHF[index1]);    

    pstSharpenPara->u8SharpenMFTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenMF[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenMF[index1]);
    
    pstSharpenPara->u8SharpenLFTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenLF[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenLF[index1]);

    pstSharpenPara->u8SadAmplifierTarget = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SadAmplifier[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SadAmplifier[index1]);     

	return HI_SUCCESS;

}

HI_S32 ISP_SharpenRegCfg(ISP_DEV IspDev, HI_VOID *pRegCfg)
{
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    SHARPEN_GET_CTX(IspDev, pstSharpen);
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
    HI_U8  i;
    HI_S32 bpCoef[3][3] = {{-16, -16, -16},
                           {-16,  28,  28},
                           {-16,  28,  32}};

    /* Ge strength */
    pstRegCfg->stGeRegCfg.u8GeStrength = pstSharpen->u8GeStrengthTarget;
    pstRegCfg->unKey.bit1GeCfg = 1;  

    /* Fixed Logic Bpfilter coef */
    pstRegCfg->stSharpenRegCfg.G3[0][0] = bpCoef[0][0];
    pstRegCfg->stSharpenRegCfg.G3[0][1] = bpCoef[0][1];
    pstRegCfg->stSharpenRegCfg.G3[0][2] = bpCoef[0][2];
    pstRegCfg->stSharpenRegCfg.G3[1][0] = bpCoef[1][0];
    pstRegCfg->stSharpenRegCfg.G3[1][1] = bpCoef[1][1];
    pstRegCfg->stSharpenRegCfg.G3[1][2] = bpCoef[1][2];
    pstRegCfg->stSharpenRegCfg.G3[2][0] = bpCoef[2][0];
    pstRegCfg->stSharpenRegCfg.G3[2][1] = bpCoef[2][1];
    pstRegCfg->stSharpenRegCfg.G3[2][2] = bpCoef[2][2];

    pstRegCfg->stSharpenRegCfg.u8LineThd1 = SHARPEN_LINE_THD1;
    pstRegCfg->stSharpenRegCfg.u8LineThd2 = SHARPEN_LINE_THD2;

    /* Variable Logic coef */
    pstRegCfg->stSharpenRegCfg.enPixSel = !(pstSharpen->bEnLowLumaShootTarget);
    pstRegCfg->stSharpenRegCfg.u16SharpAmt = (pstSharpen->u8TextureStTarget << 4);
    pstRegCfg->stSharpenRegCfg.u16EdgeAmt = (pstSharpen->u8EdgeStTarget << 4);
    pstRegCfg->stSharpenRegCfg.u16SharpThd1 = (pstSharpen->u8TextureThdTarget << 2) + pstSharpen->u16SharpThdDeltaTarget;
    pstRegCfg->stSharpenRegCfg.u16SharpThd2 = (pstSharpen->u8TextureThdTarget << 2);
    pstRegCfg->stSharpenRegCfg.u16EdgeThd1 = (pstSharpen->u8EdgeThdTarget << 2) + pstSharpen->u16EdgeThdDeltaTarget;
    pstRegCfg->stSharpenRegCfg.u16EdgeThd2 = (pstSharpen->u8EdgeThdTarget << 2);

    //pstRegCfg->stSharpenRegCfg.u16SharpThd1 = CLIP3(pstRegCfg->stSharpenRegCfg.u16SharpThd1, 0, 1023);
    //pstRegCfg->stSharpenRegCfg.u16EdgeThd1 = CLIP3(pstRegCfg->stSharpenRegCfg.u16EdgeThd1, 0, 1023);
    pstRegCfg->stSharpenRegCfg.u16SharpThd1 = MIN2(pstRegCfg->stSharpenRegCfg.u16SharpThd1, 1023);
    pstRegCfg->stSharpenRegCfg.u16EdgeThd1 = MIN2(pstRegCfg->stSharpenRegCfg.u16EdgeThd1, 1023);

	/* Calc sharpThd1MulCoef */
    pstRegCfg->stSharpenRegCfg.u16SharpThd1MulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16SharpThd2, 0, pstRegCfg->stSharpenRegCfg.u16SharpThd1, 1, 12);

    /* Calc edgeThd1MulCoef */
    pstRegCfg->stSharpenRegCfg.u16EdgeThd1MulCoef  = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16EdgeThd2, 0,pstRegCfg->stSharpenRegCfg.u16EdgeThd1, 1, 12);
    pstRegCfg->stSharpenRegCfg.u8OvershootAmt = pstSharpen->u8OverShootTarget;
    pstRegCfg->stSharpenRegCfg.u8UndershootAmt = pstSharpen->u8UnderShootTarget;

    /* hi3519 new add */
    pstRegCfg->stSharpenRegCfg.bEnJagCtrl = (pstSharpen->u8JagCtrlTarget > 0) ? 1 : 0;
    pstRegCfg->stSharpenRegCfg.u16VarJagThd1 = 800;
    pstRegCfg->stSharpenRegCfg.u16VarJagThd2 = 900;
    pstRegCfg->stSharpenRegCfg.u16EdgeJagAmt = (pstRegCfg->stSharpenRegCfg.u16EdgeAmt)/(pstSharpen->u8JagCtrlTarget + 1);    
    pstRegCfg->stSharpenRegCfg.s16EdgeJagMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16VarJagThd1, pstRegCfg->stSharpenRegCfg.u16EdgeAmt, 
		                                                       pstRegCfg->stSharpenRegCfg.u16VarJagThd2, pstRegCfg->stSharpenRegCfg.u16EdgeJagAmt, 0);
    pstRegCfg->stSharpenRegCfg.u8OshtJagAmt = pstRegCfg->stSharpenRegCfg.u8OvershootAmt;//CLIP3((pstRegCfg->stSharpenRegCfg.u8OvershootAmt)*(pstSharpen->u8JagCtrlTarget + 1), 0, 255);
    pstRegCfg->stSharpenRegCfg.u8UshtJagAmt = pstRegCfg->stSharpenRegCfg.u8UndershootAmt;//CLIP3((pstRegCfg->stSharpenRegCfg.u8UndershootAmt)*(pstSharpen->u8JagCtrlTarget + 1), 0, 255);
    pstRegCfg->stSharpenRegCfg.s16OshtJagMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16VarJagThd1, pstRegCfg->stSharpenRegCfg.u8OvershootAmt, 
		                                                       pstRegCfg->stSharpenRegCfg.u16VarJagThd2, pstRegCfg->stSharpenRegCfg.u8OshtJagAmt, 4);
    pstRegCfg->stSharpenRegCfg.s16UshtJagMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16VarJagThd1, pstRegCfg->stSharpenRegCfg.u8UndershootAmt, 
		                                                       pstRegCfg->stSharpenRegCfg.u16VarJagThd2, pstRegCfg->stSharpenRegCfg.u8UshtJagAmt, 4);
    pstRegCfg->stSharpenRegCfg.u8ShtMaxShft = 0;
    pstRegCfg->stSharpenRegCfg.u8ShtMinShft = 0;

    /* control sharpen based on luma */
    pstRegCfg->stSharpenRegCfg.bEnLumaCtrl = 1;
    for (i = 0; i < 4; i++)
    {
        pstRegCfg->stSharpenRegCfg.u8LumaTh[i] = (HI_U8)(((pstSharpen->u32LumaThdTarget)&(0xFF << (8*i))) >> (8*i));
        pstRegCfg->stSharpenRegCfg.u8LumaWgt[i] = (HI_U8)(((pstSharpen->u32LumaWgtTarget)&(0xFF << (8*i))) >> (8*i));
    }
    for (i = 0; i < 3; i++ )
    {
        pstRegCfg->stSharpenRegCfg.s16LumaMul[i] = CalcMulCoef( pstRegCfg->stSharpenRegCfg.u8LumaTh[i], pstRegCfg->stSharpenRegCfg.u8LumaWgt[i], 
                                                  pstRegCfg->stSharpenRegCfg.u8LumaTh[i+1], pstRegCfg->stSharpenRegCfg.u8LumaWgt[i+1], 4);
    }
    
    /* control shoot based on var */
    pstRegCfg->stSharpenRegCfg.bEnShtCtrlVarMod = 1;
    pstRegCfg->stSharpenRegCfg.u16OShtVarTh[0]= 200;
    pstRegCfg->stSharpenRegCfg.u16OShtVarTh[1]= 500;
    pstRegCfg->stSharpenRegCfg.u16UShtVarTh[0]= 200;
    pstRegCfg->stSharpenRegCfg.u16UShtVarTh[1]= 500;
    pstRegCfg->stSharpenRegCfg.u8OShtVarAmt = CLIP3((pstRegCfg->stSharpenRegCfg.u8OvershootAmt + (pstSharpen->u8DetailCtrlTarget << 1) - 255), 0, 255);
    pstRegCfg->stSharpenRegCfg.u8UShtVarAmt = CLIP3((pstRegCfg->stSharpenRegCfg.u8UndershootAmt + (pstSharpen->u8DetailCtrlTarget << 1) - 255), 0, 255);  
    pstRegCfg->stSharpenRegCfg.s16OShtVarMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16OShtVarTh[0], pstRegCfg->stSharpenRegCfg.u8OShtVarAmt, 
                                            pstRegCfg->stSharpenRegCfg.u16OShtVarTh[1], pstRegCfg->stSharpenRegCfg.u8OvershootAmt, 4);
    pstRegCfg->stSharpenRegCfg.s16UShtVarMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16UShtVarTh[0], pstRegCfg->stSharpenRegCfg.u8UShtVarAmt, 
                                            pstRegCfg->stSharpenRegCfg.u16UShtVarTh[1], pstRegCfg->stSharpenRegCfg.u8UndershootAmt, 4);

    /* control shoot based on luma */
    pstRegCfg->stSharpenRegCfg.bEnShtCtrlLumaMod = 1;
    pstRegCfg->stSharpenRegCfg.u8OShtLumaTh[0] = pstRegCfg->stSharpenRegCfg.u8LumaTh[0];
    pstRegCfg->stSharpenRegCfg.u8OShtLumaTh[1] = pstRegCfg->stSharpenRegCfg.u8LumaTh[1];
    pstRegCfg->stSharpenRegCfg.u8UShtLumaTh[0] = pstRegCfg->stSharpenRegCfg.u8LumaTh[2];
    pstRegCfg->stSharpenRegCfg.u8UShtLumaTh[1] = pstRegCfg->stSharpenRegCfg.u8LumaTh[3];
    pstRegCfg->stSharpenRegCfg.u8OShtLumaWgt[0]= 255 - (pstSharpen->u8SaltCtrlTarget);
    pstRegCfg->stSharpenRegCfg.u8UShtLumaWgt[0]= 255 - (pstSharpen->u8PepperCtrlTarget);
    pstRegCfg->stSharpenRegCfg.u8OShtLumaWgt[1]= 255;
    pstRegCfg->stSharpenRegCfg.u8UShtLumaWgt[1]= 255;
    pstRegCfg->stSharpenRegCfg.s16OShtLumaMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8OShtLumaTh[0], pstRegCfg->stSharpenRegCfg.u8OShtLumaWgt[0], 
                                             pstRegCfg->stSharpenRegCfg.u8OShtLumaTh[1], pstRegCfg->stSharpenRegCfg.u8OShtLumaWgt[1], 4);
    pstRegCfg->stSharpenRegCfg.s16UShtLumaMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8UShtLumaTh[0], pstRegCfg->stSharpenRegCfg.u8UShtLumaWgt[0], 
                                             pstRegCfg->stSharpenRegCfg.u8UShtLumaTh[1], pstRegCfg->stSharpenRegCfg.u8UShtLumaWgt[1], 4);
   
    pstRegCfg->stSharpenRegCfg.u8Limit = 0;    
    pstRegCfg->unKey.bit1YuvShpCfg = 1;

    pstRegCfg->stSharpenRegCfg.bShpAlgSel = pstSharpen->bShpAlgSel;
    pstRegCfg->stSharpenRegCfg.u8SharpAltD = pstSharpen->u8SharpenDTarget;
    pstRegCfg->stSharpenRegCfg.u8SharpAltUd = pstSharpen->u8SharpenUdTarget;
    pstRegCfg->stSharpenRegCfg.u16LumThresh = pstSharpen->u16LumThreshTarget;
    pstRegCfg->stSharpenRegCfg.u8SharpenHF = pstSharpen->u8SharpenHFTarget;
    pstRegCfg->stSharpenRegCfg.u8SharpenMF = pstSharpen->u8SharpenMFTarget;
    pstRegCfg->stSharpenRegCfg.u8SharpenLF = pstSharpen->u8SharpenLFTarget;
    pstRegCfg->stSharpenRegCfg.u8SadAmplifier = pstSharpen->u8SadAmplifierTarget;
    pstRegCfg->unKey.bit1BayerShpCfg = 1;

    if (!pstSharpen->u8SharpenEnabled)
    {
        pstSharpen->u8SharpenDTarget = 0;
        pstSharpen->u8SharpenUdTarget = 0;
        pstSharpen->u16LumThreshTarget = 0;
        pstSharpen->u8SharpenHFTarget = 0;  
        pstSharpen->u8SharpenMFTarget = 0;  
        pstSharpen->u8SharpenLFTarget = 0;  
        
        pstRegCfg->stSharpenRegCfg.u8SharpAltD = 0;
        pstRegCfg->stSharpenRegCfg.u8SharpAltUd = 0;
        pstRegCfg->stSharpenRegCfg.u16LumThresh = 0;
        pstRegCfg->stSharpenRegCfg.u8SharpenHF = 0;
        pstRegCfg->stSharpenRegCfg.u8SharpenMF = 0;
        pstRegCfg->stSharpenRegCfg.u8SharpenLF = 0;

        pstRegCfg->unKey.bit1BayerShpCfg = 1;
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
    
}
    


HI_S32 ISP_SharpenRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{

    HI_U32 u32Iso = 0;    
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;

    SHARPEN_GET_CTX(IspDev, pstSharpen);
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* sharpen strength linkage with the iso calculated by ae */
    u32Iso = pstSharpen->u32Iso;
    pstSharpen->u32Iso = pstIspCtx->stLinkage.u32Iso;

    /* calculate every two interrupts other than capture state*/
    if ((1 != pstIspCtx->u32FrameCnt % 2) && (HI_TRUE != pstIspCtx->stLinkage.bCapState) && (SNAP_TYPE_SYNC != pstIspCtx->stLinkage.enSnapType) && (SNAP_TYPE_SYNC_PRO != pstIspCtx->stLinkage.enSnapType))
    {
        return HI_SUCCESS;
    }

    if ((0 == pstIspCtx->stLinkage.u32UpdatePos) || (HI_TRUE != pstIspCtx->stLinkage.bCapState))
    {
        ISP_SharpenRegCfg(IspDev, pRegCfg);
    }

    SharpenReadExtregs(IspDev);    

    if (HI_TRUE == pstIspCtx->stLinkage.bCapState)
    {
        ISP_Sharpen_GetRegCfg(IspDev, pstIspCtx->stLinkage.u32Iso);
    }
    else
    {
        ISP_Sharpen_GetRegCfg(IspDev, u32Iso);
    }

    if (pstSharpen->u8ManualSharpenYuvEnabled)
    {
        pstSharpen->bEnLowLumaShootTarget = hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_read(IspDev);
        pstSharpen->u8TextureStTarget = hi_ext_system_manual_yuv_sharpening_TextureSt_read(IspDev);
        pstSharpen->u8EdgeStTarget = hi_ext_system_manual_yuv_sharpening_EdgeSt_read(IspDev);
        pstSharpen->u8OverShootTarget = hi_ext_system_manual_yuv_sharpening_OverShoot_read(IspDev);           
        pstSharpen->u8UnderShootTarget = hi_ext_system_manual_yuv_sharpening_UnderShoot_read(IspDev); 
        pstSharpen->u8TextureThdTarget = hi_ext_system_manual_yuv_sharpening_TextureThd_read(IspDev);         
        pstSharpen->u8EdgeThdTarget = hi_ext_system_manual_yuv_sharpening_EdgeThd_read(IspDev);           
        pstSharpen->u8JagCtrlTarget = hi_ext_system_manual_yuv_sharpening_JagCtrl_read(IspDev);           
        pstSharpen->u8SaltCtrlTarget = hi_ext_system_manual_yuv_sharpening_SaltCtrl_read(IspDev);
        pstSharpen->u8PepperCtrlTarget = hi_ext_system_manual_yuv_sharpening_PepperCtrl_read(IspDev);     
        pstSharpen->u8DetailCtrlTarget = hi_ext_system_manual_yuv_sharpening_DetailCtrl_read(IspDev);     
        pstSharpen->u32LumaThdTarget = hi_ext_system_manual_yuv_sharpening_LumaThd_read(IspDev);                              
        pstSharpen->u32LumaWgtTarget = hi_ext_system_manual_yuv_sharpening_LumaWgt_read(IspDev);
    }  

    /* Bayer sharpen reg cfg */
    if (pstSharpen->u8ManualSharpenEnabled)
    {     
        pstSharpen->bShpAlgSel = hi_ext_system_sharpening_alg_select_read(IspDev);
        pstSharpen->u8SharpenDTarget = hi_ext_system_sharpening_d_target_read(IspDev);
        pstSharpen->u8SharpenUdTarget = hi_ext_system_sharpening_ud_target_read(IspDev);
        pstSharpen->u16LumThreshTarget = hi_ext_system_demosaic_lum_thresh_target_read(IspDev);
        pstSharpen->u8SharpenHFTarget = hi_ext_system_sharpening_hf_target_read(IspDev);  
        pstSharpen->u8SharpenMFTarget = hi_ext_system_sharpening_mf_target_read(IspDev);  
        pstSharpen->u8SharpenLFTarget = hi_ext_system_sharpening_lf_target_read(IspDev);  
        pstSharpen->u8SadAmplifierTarget = hi_ext_system_sharpening_sad_amplifier_read(IspDev);
    }

    /*when snap frame, parameters should set current frame */
    if ((1 == pstIspCtx->stLinkage.u32UpdatePos) || (HI_TRUE == pstIspCtx->stLinkage.bCapState))
    {
        ISP_SharpenRegCfg(IspDev, pRegCfg);
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
        //case ISP_CHANGE_IMAGE_MODE_SET:   
        //    SharpenImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);  
         //   break;
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

