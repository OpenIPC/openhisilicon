/******************************************************************************

  Copyright (C), 2015-2025, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_cac.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Description   : 
  History       :
  1.Date        : 
    Author      :  q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_CA_CSC_DC_LEN (3)
#define HI_ISP_CA_CSC_COEF_LEN (9)
#define HI_ISP_CA_CSC_TYPE_DEFAULT (0)

static HI_S32 g_as32CAIsoLut  [ISP_AUTO_ISO_STRENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};


typedef struct
{
	HI_S32 s32Csc_coef00;
	HI_S32 s32Csc_coef01;
	HI_S32 s32Csc_coef02;
	HI_S32 s32Csc_coef10;
	HI_S32 s32Csc_coef11;
	HI_S32 s32Csc_coef12;
	HI_S32 s32Csc_coef20;
	HI_S32 s32Csc_coef21;
	HI_S32 s32Csc_coef22;
			  
	HI_S32 s32Csc_in_dc0;
	HI_S32 s32Csc_in_dc1;
	HI_S32 s32Csc_in_dc2;
			  
	HI_S32 s32Csc_out_dc0;
	HI_S32 s32Csc_out_dc1;
	HI_S32 s32Csc_out_dc2;

}CA_Coef_S;

typedef enum hiHI_ISP_CA_CS_E
{
    CA_CS_BT_709 = 0,
    CA_CS_BT_601 = 1,
    CA_CS_BUTT
} HI_ISP_CA_CS_E;

typedef struct tagHI_ISP_CA_CSC_TABLE_S
{
	HI_S32 s32CSCIdc[HI_ISP_CA_CSC_DC_LEN];   
	HI_S32 s32CSCOdc[HI_ISP_CA_CSC_DC_LEN];
	HI_S32 s32CSCCoef[HI_ISP_CA_CSC_COEF_LEN];
} HI_ISP_CA_CSC_TABLE_S;

static HI_ISP_CA_CSC_TABLE_S  g_stCSCTable_RGB_to_HDYCbCr =
{
	{0,0,0},
	{0,512,512},
	{213, 715, 72,-117, -394, 511,511, -464, -47},
};//range[0,255]  X1000

static HI_ISP_CA_CSC_TABLE_S  g_stCSCTable_RGB_to_SDYCbCr =
{
	{0,0,0},
	{0,512,512},
	{299,587,114,-172,-339,511,511,-428,-83},
};//range[0,255]  X1000

static HI_ISP_CA_CSC_TABLE_S  g_stCSCTable_HDYCbCr_to_RGB =
{
	{0,-512,-512},
	{0,0,0},
	{1000, 0, 1575,1000, -187, -468,1000, 1856, 0},
};//range[0,255]  X1000

static HI_ISP_CA_CSC_TABLE_S  g_stCSCTable_SDYCbCr_to_RGB =
{
	{0,-512,-512},
	{0,0,0},
	{1000,0,1402,1000,-344,-714,1000,1772,0},
};//range[0,255]  X1000

typedef struct hiISP_CA_S
{
	HI_U16 u16LumaThdHigh;
	HI_U16 u16LumaRatioHigh;
	HI_S16 s16SaturationRatio;
	HI_U16 au16YRatioLut[HI_ISP_CA_YRATIO_LUT_LENGTH];	
	HI_S32 aus32CAIsoRatio[ISP_AUTO_ISO_STRENGTH_NUM];//16
}ISP_CA_S;

ISP_CA_S g_astCaCtx[ISP_MAX_DEV_NUM] = {{0}};

#define CA_GET_CTX(dev, pstCtx)   pstCtx = &g_astCaCtx[dev]

static HI_VOID CaExtRegsDefault(HI_VOID)
{
	hi_ext_system_ca_luma_thd_high_write(HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT);
	hi_ext_system_ca_saturation_ratio_write(HI_ISP_EXT_CA_SATURATION_RATIO_DEFAULT);	
	return;
}

static HI_VOID CaRegsDefault(ISP_DEV IspDev) 
{	
	ISP_CTX_S *pstIspCtx = HI_NULL;    
	HI_U8 u8WDRMode;
	ISP_GET_CTX(IspDev, pstIspCtx);
    u8WDRMode = pstIspCtx->u8SnsWDRMode;
	
	hi_isp_ca_cfg_en_write(IspDev, HI_TRUE);
	hi_isp_ca_llhcproc_en_write(IspDev, HI_TRUE);
	hi_isp_ca_skinproc_en_write(IspDev, HI_TRUE);
	hi_isp_ca_colorproc_en_write(IspDev, HI_FALSE);
	if (IS_LINEAR_MODE(u8WDRMode))
	{  
		hi_isp_ca_satadj_en_write(IspDev,HI_TRUE);	
	}
	else{
		hi_isp_ca_satadj_en_write(IspDev,HI_FALSE);
	}
	hi_isp_ca_lumaratio_low_write(IspDev,HI_ISP_CA_LUMA_RATIO_LOW_DEFAULT);

	hi_isp_ca_lumath_low_write(IspDev, HI_ISP_CA_LUMA_THD_LOW_DEFAULT);
	//hi_isp_ca_lumath_high_write(IspDev, HI_ISP_CA_LUMA_THD_HIGH_DEFAULT);
	hi_isp_ca_darkchromath_low_write(IspDev, HI_ISP_CA_DARKCHROMA_THD_LOW_DEFAULT);
	hi_isp_ca_darkchromath_high_write(IspDev, HI_ISP_CA_DARKCHROMA_THD_HIGH_DEFAULT);
	hi_isp_ca_sdarkchromath_low_write(IspDev, HI_ISP_CA_SDARKCHROMA_THD_LOW_DEFAULT);
	hi_isp_ca_sdarkchromath_high_write(IspDev, HI_ISP_CA_SDARKCHROMA_THD_HIGH_DEFAULT);
	

	hi_isp_ca_skinlluma_umin_write(IspDev, HI_ISP_CA_SKINLOWLUAM_UMIN_DEFAULT);
	hi_isp_ca_skinlluma_umax_write(IspDev, HI_ISP_CA_SKINLOWLUAM_UMAX_DEFAULT);
	hi_isp_ca_skinlluma_uymin_write(IspDev, HI_ISP_CA_SKINLOWLUAM_UYMIN_DEFAULT);
	hi_isp_ca_skinlluma_uymax_write(IspDev, HI_ISP_CA_SKINLOWLUAM_UYMAX_DEFAULT);

	hi_isp_ca_skinhluma_umin_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_UMIN_DEFAULT);
	hi_isp_ca_skinhluma_umax_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_UMAX_DEFAULT);
	hi_isp_ca_skinhluma_uymin_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_UYMIN_DEFAULT);
	hi_isp_ca_skinhluma_uymax_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_UYMAX_DEFAULT);

	hi_isp_ca_skinlluma_vmin_write(IspDev, HI_ISP_CA_SKINLOWLUAM_VMIN_DEFAULT);
	hi_isp_ca_skinlluma_vmax_write(IspDev, HI_ISP_CA_SKINLOWLUAM_VMAX_DEFAULT);
	hi_isp_ca_skinlluma_vymin_write(IspDev, HI_ISP_CA_SKINLOWLUAM_VYMIN_DEFAULT);
	hi_isp_ca_skinlluma_vymax_write(IspDev, HI_ISP_CA_SKINLOWLUAM_VYMAX_DEFAULT);

	hi_isp_ca_skinhluma_vmin_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_VMIN_DEFAULT);
	hi_isp_ca_skinhluma_vmax_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_VMAX_DEFAULT);
	hi_isp_ca_skinhluma_vymin_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_VYMIN_DEFAULT);
	hi_isp_ca_skinhluma_vymax_write(IspDev, HI_ISP_CA_SKINHIGHLUAM_VYMAX_DEFAULT);

	hi_isp_ca_colorlluma_umin_write(IspDev, HI_ISP_CA_COLORLOWLUAM_UMIN_DEFAULT);
	hi_isp_ca_colorlluma_umax_write(IspDev, HI_ISP_CA_COLORLOWLUAM_UMAX_DEFAULT);
	hi_isp_ca_colorlluma_uymin_write(IspDev, HI_ISP_CA_COLORLOWLUAM_UYMIN_DEFAULT);
	hi_isp_ca_colorlluma_uymax_write(IspDev, HI_ISP_CA_COLORLOWLUAM_UYMAX_DEFAULT);

	hi_isp_ca_colorhluma_umin_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_UMIN_DEFAULT);
	hi_isp_ca_colorhluma_umax_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_UMAX_DEFAULT);
	hi_isp_ca_colorhluma_uymin_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_UYMIN_DEFAULT);
	hi_isp_ca_colorhluma_uymax_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_UYMAX_DEFAULT);

	hi_isp_ca_colorlluma_vmin_write(IspDev, HI_ISP_CA_COLORLOWLUAM_VMIN_DEFAULT);
	hi_isp_ca_colorlluma_vmax_write(IspDev, HI_ISP_CA_COLORLOWLUAM_VMAX_DEFAULT);
	hi_isp_ca_colorlluma_vymin_write(IspDev, HI_ISP_CA_COLORLOWLUAM_VYMIN_DEFAULT);
	hi_isp_ca_colorlluma_vymax_write(IspDev, HI_ISP_CA_COLORLOWLUAM_VYMAX_DEFAULT);

	hi_isp_ca_colorhluma_vmin_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_VMIN_DEFAULT);
	hi_isp_ca_colorhluma_vmax_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_VMAX_DEFAULT);
	hi_isp_ca_colorhluma_vymin_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_VYMIN_DEFAULT);
	hi_isp_ca_colorhluma_vymax_write(IspDev, HI_ISP_CA_COLORHIGHLUAM_VYMAX_DEFAULT);

	hi_isp_ca_skin_uvdiff_write(IspDev, HI_ISP_CA_SKINUVDIFF_DEFAULT);
	hi_isp_ca_skinratioth_low_write(IspDev, HI_ISP_CA_SKINRATIOTHD_LOW_DEFAULT);
	hi_isp_ca_skinratioth_mid_write(IspDev, HI_ISP_CA_SKINRATIOTHD_MID_DEFAULT);
	hi_isp_ca_skinratioth_high_write(IspDev, HI_ISP_CA_SKINRATIOTHD_HIGH_DEFAULT);

	hi_isp_ca_color_uvdiff_write(IspDev, HI_ISP_CA_COLORUVDIFF_DEFAULT);
	hi_isp_ca_colorratioth_low_write(IspDev, HI_ISP_CA_COLORRATIOTHD_LOW_DEFAULT);
	hi_isp_ca_colorratioth_mid_write(IspDev, HI_ISP_CA_COLORRATIOTHD_MID_DEFAULT);
	hi_isp_ca_colorratioth_high_write(IspDev, HI_ISP_CA_COLORRATIOTHD_HIGH_DEFAULT);
	
	return;
}

static HI_VOID CaExtRegsInitialize(ISP_DEV IspDev)
{	
	HI_U8 i;
	HI_U32 u32Width,u32Height;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	hi_isp_ca_cfg_en_write(IspDev,pstSnsDft->stCa.bEnable);

	
	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();
	hi_isp_ca_width_write(IspDev,u32Width - 1);
	hi_isp_ca_height_write(IspDev,u32Height - 1);

	for(i=0;i<HI_ISP_CA_YRATIO_LUT_LENGTH;i++)
	{
		hi_ext_system_ca_y_ratio_lut_write(i, pstSnsDft->stCa.au16YRatioLut[i]);
	}

	for(i=0;i<ISP_AUTO_ISO_STRENGTH_NUM;i++)
	{
		hi_ext_system_ca_iso_ratio_lut_write(i, pstSnsDft->stCa.as32ISORatio[i]);
	}
	
    return;
}

static HI_VOID GetCscTable(HI_ISP_CA_CS_E enCsc, HI_ISP_CA_CSC_TABLE_S **pstYuv2Rgb, HI_ISP_CA_CSC_TABLE_S **pstRgb2Yuv)
{
	switch (enCsc)
	{
		case CA_CS_BT_709:
			*pstYuv2Rgb = &g_stCSCTable_HDYCbCr_to_RGB;
			*pstRgb2Yuv = &g_stCSCTable_RGB_to_HDYCbCr;
			break;
		case CA_CS_BT_601:
			*pstYuv2Rgb = &g_stCSCTable_SDYCbCr_to_RGB;
			*pstRgb2Yuv = &g_stCSCTable_RGB_to_SDYCbCr;	
			break;
		default:
			*pstYuv2Rgb = HI_NULL;
			*pstRgb2Yuv = HI_NULL;			
			break;
	}
}

static HI_VOID SetColorSpaceConvertParamsDef(ISP_DEV IspDev, HI_ISP_CA_CS_E enCsc)
{	
	HI_ISP_CA_CSC_TABLE_S *pstYuv2Rgb, *pstRgb2Yuv;

	GetCscTable(enCsc, &pstYuv2Rgb, &pstRgb2Yuv);

	if (HI_NULL == pstYuv2Rgb || HI_NULL == pstRgb2Yuv)
	{
		printf("Unable to handle null point in ca moudle!\n");
		return;
	}
	
	hi_isp_ca_yuv2rgb_coef00_write(IspDev, pstYuv2Rgb->s32CSCCoef[0]*1024/1000);
	hi_isp_ca_yuv2rgb_coef01_write(IspDev, pstYuv2Rgb->s32CSCCoef[1]*1024/1000);
	hi_isp_ca_yuv2rgb_coef02_write(IspDev, pstYuv2Rgb->s32CSCCoef[2]*1024/1000);

	hi_isp_ca_yuv2rgb_coef10_write(IspDev, pstYuv2Rgb->s32CSCCoef[3]*1024/1000);
	hi_isp_ca_yuv2rgb_coef11_write(IspDev, pstYuv2Rgb->s32CSCCoef[4]*1024/1000);
	hi_isp_ca_yuv2rgb_coef12_write(IspDev, pstYuv2Rgb->s32CSCCoef[5]*1024/1000);

	hi_isp_ca_yuv2rgb_coef20_write(IspDev, pstYuv2Rgb->s32CSCCoef[6]*1024/1000);
	hi_isp_ca_yuv2rgb_coef21_write(IspDev, pstYuv2Rgb->s32CSCCoef[7]*1024/1000);
	hi_isp_ca_yuv2rgb_coef22_write(IspDev, pstYuv2Rgb->s32CSCCoef[8]*1024/1000);

	hi_isp_ca_yuv2rgb_indc0_write(IspDev, pstYuv2Rgb->s32CSCIdc[0]);
	hi_isp_ca_yuv2rgb_indc1_write(IspDev, pstYuv2Rgb->s32CSCIdc[1]);
	hi_isp_ca_yuv2rgb_indc2_write(IspDev, pstYuv2Rgb->s32CSCIdc[2]);

	hi_isp_ca_yuv2rgb_outdc0_write(IspDev, pstYuv2Rgb->s32CSCOdc[0]);
	hi_isp_ca_yuv2rgb_outdc1_write(IspDev, pstYuv2Rgb->s32CSCOdc[1]);
	hi_isp_ca_yuv2rgb_outdc2_write(IspDev, pstYuv2Rgb->s32CSCOdc[2]);
	
	hi_isp_ca_rgb2yuv_coef00_write(IspDev, pstRgb2Yuv->s32CSCCoef[0]*1024/1000);
	hi_isp_ca_rgb2yuv_coef01_write(IspDev, pstRgb2Yuv->s32CSCCoef[1]*1024/1000);
	hi_isp_ca_rgb2yuv_coef02_write(IspDev, pstRgb2Yuv->s32CSCCoef[2]*1024/1000);

	hi_isp_ca_rgb2yuv_coef10_write(IspDev, pstRgb2Yuv->s32CSCCoef[3]*1024/1000);
	hi_isp_ca_rgb2yuv_coef11_write(IspDev, pstRgb2Yuv->s32CSCCoef[4]*1024/1000);
	hi_isp_ca_rgb2yuv_coef12_write(IspDev, pstRgb2Yuv->s32CSCCoef[5]*1024/1000);

	hi_isp_ca_rgb2yuv_coef20_write(IspDev, pstRgb2Yuv->s32CSCCoef[6]*1024/1000);
	hi_isp_ca_rgb2yuv_coef21_write(IspDev, pstRgb2Yuv->s32CSCCoef[7]*1024/1000);
	hi_isp_ca_rgb2yuv_coef22_write(IspDev, pstRgb2Yuv->s32CSCCoef[8]*1024/1000);

	hi_isp_ca_rgb2yuv_indc0_write(IspDev, pstRgb2Yuv->s32CSCIdc[0]);
	hi_isp_ca_rgb2yuv_indc1_write(IspDev, pstRgb2Yuv->s32CSCIdc[1]);
	hi_isp_ca_rgb2yuv_indc2_write(IspDev, pstRgb2Yuv->s32CSCIdc[2]);

	hi_isp_ca_rgb2yuv_outdc0_write(IspDev, pstRgb2Yuv->s32CSCOdc[0]);
	hi_isp_ca_rgb2yuv_outdc1_write(IspDev, pstRgb2Yuv->s32CSCOdc[1]);
	hi_isp_ca_rgb2yuv_outdc2_write(IspDev, pstRgb2Yuv->s32CSCOdc[2]);
	
}

static HI_VOID CaRegsInitialize(ISP_DEV IspDev)
{
	ISP_CA_S *pstCA=HI_NULL;		
	HI_U16 u16Index, u16Width, u16Height;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	
	CA_GET_CTX(IspDev,pstCA);

	u16Width  = hi_ext_sync_total_width_read();
	u16Height = hi_ext_sync_total_height_read();

	hi_isp_ca_width_write(IspDev, u16Width-1);
	hi_isp_ca_height_write(IspDev, u16Height-1);

	SetColorSpaceConvertParamsDef(IspDev, HI_ISP_CA_CSC_TYPE_DEFAULT);

	//u16HighLumaRatio
	pstCA->u16LumaThdHigh = hi_ext_system_ca_luma_thd_high_read();
	hi_isp_ca_lumath_high_write(IspDev, pstCA->u16LumaThdHigh);
	
	u16Index = (pstCA->u16LumaThdHigh>>3);
	u16Index = (u16Index>=HI_ISP_CA_YRATIO_LUT_LENGTH)?(HI_ISP_CA_YRATIO_LUT_LENGTH-1):u16Index;
	
	hi_isp_ca_lumaratio_high_write(IspDev, pstSnsDft->stCa.au16YRatioLut[u16Index]);
		
    return;
}

static HI_S32 CaReadExtregs(ISP_DEV IspDev)
{   			
	HI_U8 i;
	ISP_CA_S *pstCA=HI_NULL;
	
	CA_GET_CTX(IspDev,pstCA);
	
	for(i=0;i<HI_ISP_CA_YRATIO_LUT_LENGTH;i++)
	{
		pstCA->au16YRatioLut[i]=hi_ext_system_ca_y_ratio_lut_read(i);
	}

	for(i=0;i<ISP_AUTO_ISO_STRENGTH_NUM;i++)
	{
		pstCA->aus32CAIsoRatio[i] = hi_ext_system_ca_iso_ratio_lut_read(i);
	}

	pstCA->u16LumaThdHigh = hi_ext_system_ca_luma_thd_high_read();
	pstCA->s16SaturationRatio = (HI_S16)hi_ext_system_ca_saturation_ratio_read();
	
	return 0;
}

static HI_S32 ISP_CaInit(ISP_DEV IspDev)
{
    CaRegsDefault(IspDev);
    CaExtRegsDefault();
    CaRegsInitialize(IspDev);
    CaExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

static HI_S32 CaGetValueFromLut(HI_S32 x, HI_S32 *pLutX, HI_S32 *pLutY, HI_S32 length)
{
	HI_S32 n = 0;

	if(x <= pLutX[0]) return pLutY[0];

	for(n = 1; n < length; n++) 
	{
		if(x <= pLutX[n]) 
		{
			return (pLutY[n-1] + (pLutY[n] - pLutY[n - 1]) * (x - pLutX[n - 1]) / DIV_0_TO_1(pLutX[n] - pLutX[n - 1]));
		}
	}

	return pLutY[length - 1];
}

static HI_VOID ISP_CA_FW(HI_S32 s32Iso,ISP_CA_S *pstCA, ISP_CA_REG_CFG_S *pstCaHwCfg)
{
	HI_U8 i;
	HI_U16 u16Index;
	HI_S32 s32IsoRatio;
	
	for(i=0;i<HI_ISP_CA_YRATIO_LUT_LENGTH;i++)
	{
		pstCaHwCfg->au16YRatioLUT[i] = CLIP3(pstCA->au16YRatioLut[i] *pstCA->s16SaturationRatio/1000,0,2047);
		//pstCaHwCfg->au16YRatioLUT[i] = pstCA->au16YRatioLut[i];
	}

	u16Index = (pstCA->u16LumaThdHigh>>3);
	u16Index = (u16Index>=HI_ISP_CA_YRATIO_LUT_LENGTH)?(HI_ISP_CA_YRATIO_LUT_LENGTH-1):u16Index;

	pstCaHwCfg->u16LumaThdHigh   = pstCA->u16LumaThdHigh;
	pstCaHwCfg->u16HighLumaRatio = pstCA->au16YRatioLut[u16Index];
	
	s32IsoRatio = CaGetValueFromLut(s32Iso, g_as32CAIsoLut, pstCA->aus32CAIsoRatio, ISP_AUTO_ISO_STRENGTH_NUM);
	
	pstCaHwCfg->s16ISORatio = CLIP3(s32IsoRatio,0,2047);
}

static HI_S32 ISP_CaRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CTX_S *pstIspCtx;
	ISP_CA_S *pstCA=HI_NULL;
	ISP_REG_CFG_S *pstReg;
	
	CA_GET_CTX(IspDev,pstCA);
	ISP_GET_CTX(IspDev, pstIspCtx);

	/* calculate every two interrupts */	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }
	
	CaReadExtregs(IspDev);

	pstReg = (ISP_REG_CFG_S *)pRegCfg;
	ISP_CA_FW((HI_S32)pstIspCtx->stLinkage.u32Iso, pstCA, &pstReg->stCaRegCfg);

	pstReg->unKey.bit1CaCfg = 1;
	
    return HI_SUCCESS;
}



static __inline HI_S32 CAImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{

	hi_isp_ca_width_write(IspDev, pstRes->u16Width-1);
	hi_isp_ca_height_write(IspDev,  pstRes->u16Height-1);

    return HI_SUCCESS;
}


static HI_S32 ISP_CaCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{	
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
			ISP_CaInit(IspDev);
            break;
        case ISP_PROC_WRITE:
			break;
        case ISP_CHANGE_IMAGE_MODE_SET:
			CAImageResWrite(IspDev,(ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

static HI_S32 ISP_CaExit(ISP_DEV IspDev)
{
	hi_isp_ca_cfg_en_write(IspDev, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterCa(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_CA;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CaInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CaRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CaCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CaExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


