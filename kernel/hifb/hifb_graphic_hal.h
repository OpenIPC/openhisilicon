/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef __HIFB_HAL_H__
#define __HIFB_HAL_H__

#include "hifb.h"
#include "hifb_reg.h"
#include "hifb_def.h"
#include "hifb_coef_org.h"
#include "hi_comm_vo.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_VOID HAL_VOU_Init(HI_VOID);
HI_VOID HAL_VOU_Exit(HI_VOID);
HI_VOID HAL_WriteReg(HI_U32* pAddress, HI_U32 Value);
HI_U32 HAL_ReadReg(HI_U32* pAddress);

/*****************************************************************************
 Prototype       : sys Relative
*****************************************************************************/
HI_VOID HAL_SYS_SetOutstanding(HI_VOID);
HI_VOID HAL_SYS_SetArbMode(HI_U32 bMode);
HI_VOID HAL_SYS_SetRdBusId(HI_U32 bMode);
HI_VOID HAL_SYS_VdpResetClk(HI_U32 sel);

/*****************************************************************************
 Prototype       : device Relative
*****************************************************************************/
HI_BOOL HAL_DISP_SetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL enIntf);
HI_BOOL HAL_DISP_GetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbIntfEn);
hi_void hifb_hal_soft_int_en(hi_bool soft_int_en);


HI_BOOL HAL_DISP_GetIntState(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbBottom);
HI_BOOL HAL_DISP_GetIntStateVblank(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbVblank);
HI_BOOL HAL_DISP_GetIntStateVbackBlank(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbVbackBlank);
HI_BOOL HAL_DISP_GetIntStateVcnt(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32* pu32Vcnt);
HI_BOOL HAL_DISP_SetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                             HAL_DISP_SYNCINFO_S* pstSyncInfo);
HI_BOOL HAL_DISP_SetIntfSyncInv(HAL_DISP_INTF_E enIntf, HAL_DISP_SYNCINV_S* pstInv);
HI_BOOL HAL_DISP_SetIntfMultiChnEn(HAL_DISP_INTF_E enIntf, HAL_MULTICHN_EN enMultiChnEn);
HI_BOOL HAL_DISP_SetIntfCtrl(HAL_DISP_INTF_E enIntf, HI_U32* pu32CtrlInfo);
HI_BOOL HAL_DISP_GetDispIoP(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbIop);
HI_BOOL HAL_DISP_SetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan, HAL_DISP_INTF_E enIntf);
HI_BOOL HAL_DISP_SetBt1120Sel(HAL_DISP_OUTPUTCHANNEL_E enChan);
HI_BOOL HAL_DISP_SetIntfClipEnable(HAL_DISP_INTF_E enIntf, HI_BOOL enClip);
HI_BOOL HAL_DISP_SetIntfClip(HAL_DISP_INTF_E enChan, HI_BOOL bClip, HAL_DISP_CLIP_S* pstClipData);
HI_BOOL HAL_DISP_SetIntvsyncTeMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bSlave);
HI_BOOL HAL_DISP_SetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 uFieldMode);
HI_BOOL HAL_DISP_GetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbFieldMode);
HI_BOOL HAL_DISP_SetVtThd(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 vtthd);

HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_ClrIntMask(HI_U32 u32MaskEn);
HI_U32  HAL_DISP_GetIntStatus(HI_U32 u32IntMsk);
HI_U32  HAL_DISP_GetOriIntStatus(HI_U32 u32IntStatus);
HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk);
HI_U32  HAL_DISP_GetRawIntStatus(HI_U32 u32IntMsk);
HI_U32  HAL_DISP_GetRawIntStatus1(HI_U32 u32IntMsk);

HI_BOOL HAL_DISP_SetClkGateEnable(HI_U32 u32Data);
HI_VOID HAL_DISP_SetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan);
HI_U32  HAL_DISP_GetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan);
HI_BOOL HAL_DISP_GetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,HAL_DISP_SYNCINFO_S *pstSyncInfo);
HI_BOOL HAL_DISP_GetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan,VO_INTF_TYPE_E *pbenIntfType);
HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT* pstRect);
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT* pstRect);
HI_BOOL HAL_VIDEO_SetMultiAreaLAddr  (HAL_DISP_LAYER_E enLayer, HI_U32 area_num,
                                      HI_UL u32LAddr, HI_U16 stride);
HI_BOOL HAL_VIDEO_SetLayerRimWidth(HAL_DISP_LAYER_E enLayer, HI_U32 u32RimWidth);
HI_BOOL HAL_VIDEO_SetLayerRimCol0(HAL_DISP_LAYER_E enLayer, VDP_BKG_S  *pstRimCol0);
HI_BOOL HAL_VIDEO_SetLayerRimCol1(HAL_DISP_LAYER_E enLayer, VDP_BKG_S  *pstRimCol1);
HI_BOOL HAL_VIDEO_SetMultiAreaCAddr  (HAL_DISP_LAYER_E enLayer, HI_U32 area_num,
                                      HI_UL u32CAddr, HI_U16 stride);
HI_BOOL HAL_VIDEO_GetMultiAreaLAddr  (HAL_DISP_LAYER_E enLayer, HI_U32 area_num,
                                      HI_U32 *pu32LAddrLow, HI_U32 *pu32LAddrHigh);
HI_BOOL HAL_VIDEO_GetMultiAreaCAddr  (HAL_DISP_LAYER_E enLayer, HI_U32 area_num,
                                      HI_U32 *pu32CAddrLow, HI_U32 *pu32CAddrHigh);


HI_BOOL HAL_VIDEO_SetMultiAreaVDcmpOffset(HAL_DISP_LAYER_E enLayer, HI_U32 area_num,
        HI_U64 u64LHOffset, HI_U64 u64CHOffset);

HI_BOOL HAL_VIDEO_SetMultiAreaVDcmpEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_VIDEO_SetMultiAreaReso(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,
                                              HI_U32 u32Width);
HI_BOOL HAL_VIDEO_SetMultiAreaRect(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,RECT_S *pstVideoAreaRect);
HI_BOOL HAL_VIDEO_GetMRGState(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32Mrg);
HI_BOOL HAL_VIDEO_CLrMRGState(HAL_DISP_LAYER_E enLayer);
HI_BOOL HAL_VIDEO_GetVDcmpLumaState(HAL_DISP_LAYER_E enLayer, HI_U32* pu32DcmpLumaStat);
HI_BOOL HAL_VIDEO_GetVDcmpChromaState(HAL_DISP_LAYER_E enLayer, HI_U32* pu32DcmpChromaStat);
HI_BOOL HAL_VIDEO_ClrVDcmpLumaState(HAL_DISP_LAYER_E enLayer);
HI_BOOL HAL_VIDEO_ClrVDcmpChromaState(HAL_DISP_LAYER_E enLayer);

HI_BOOL HAL_VIDEO_SetAllAreaDisable  (HAL_DISP_LAYER_E enLayer);

/* Video layer CVFIR relative hal functions */
HI_VOID HAL_VIDEO_CVFIR_SetOutHeight(HAL_DISP_LAYER_E enLayer, HI_U32 out_height);
HI_VOID HAL_VIDEO_CVFIR_SetOutFmt(HAL_DISP_LAYER_E enLayer, HI_U32 out_fmt);
HI_VOID HAL_VIDEO_CVFIR_SetOutPro(HAL_DISP_LAYER_E enLayer, HI_U32 out_pro);
HI_VOID HAL_VIDEO_CVFIR_SetVzmeCkGtEn(HAL_DISP_LAYER_E enLayer, HI_BOOL vzme_ck_gt_en);

HI_VOID HAL_VIDEO_CVFIR_SetCvfirEn(HAL_DISP_LAYER_E enLayer, HI_U32 cvfir_en);
HI_VOID HAL_VIDEO_CVFIR_SetCvmidEn(HAL_DISP_LAYER_E enLayer, HI_U32 cvmid_en);
HI_VOID HAL_VIDEO_CVFIR_SetCvfirMode(HAL_DISP_LAYER_E enLayer, HI_U32 cvfir_mode);
HI_VOID HAL_VIDEO_CVFIR_SetVratio(HAL_DISP_LAYER_E enLayer, HI_U32 vratio);

HI_VOID HAL_VIDEO_CVFIR_SetVChromaOffset(HAL_DISP_LAYER_E enLayer, HI_U32 vchroma_offset);
HI_VOID HAL_VIDEO_CVFIR_SetVbChromaOffset(HAL_DISP_LAYER_E enLayer, HI_U32 vbchroma_offset);

HI_VOID HAL_VIDEO_CVFIR_SetCoef(HAL_DISP_LAYER_E enLayer, CvfirCoef_S *pstCvfirCoef);

/* Video layer HFIR relative hal functions */
HI_VOID HAL_VIDEO_HFIR_SetCkGtEn(HAL_DISP_LAYER_E enLayer, HI_U32 ck_gt_en);
HI_VOID HAL_VIDEO_HFIR_SetMidEn(HAL_DISP_LAYER_E enLayer, HI_U32 mid_en);
HI_VOID HAL_VIDEO_HFIR_SetHfirMode(HAL_DISP_LAYER_E enLayer, HI_U32 hfir_mode);
HI_VOID HAL_VIDEO_HFIR_SetHfirEn(HAL_DISP_LAYER_E enLayer, HI_U32 hfir_en);
HI_VOID HAL_VIDEO_HFIR_SetCoef(HAL_DISP_LAYER_E enLayer, HfirCoef_S *pstHfirCoef);

/*****************************************************************************
 Prototype       : layer Relative
*****************************************************************************/
/* Video layer CSC relative hal functions. */
HI_VOID HAL_LAYER_CSC_SetEnable(HAL_DISP_LAYER_E enLayer, HI_BOOL csc_en);
HI_VOID HAL_LAYER_CSC_SetCkGtEn(HAL_DISP_LAYER_E enLayer, HI_BOOL ck_gt_en);
HI_VOID HAL_LAYER_CSC_SetCoef(HAL_DISP_LAYER_E enLayer, VDP_CSC_COEF_S *pstCscCoef);
HI_VOID HAL_LAYER_CSC_SetDcCoef(HAL_DISP_LAYER_E enLayer, VDP_CSC_DC_COEF_S *pstCscDcCoef);
HI_BOOL HAL_LAYER_SetLinkCtrl(HAL_DISP_LAYER_E enLayer, HI_U8 u8CbmId);

HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_LAYER_GetLayerEnable(HAL_DISP_LAYER_E enLayer, HI_U32* pu32Enable);
HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                  HAL_DISP_PIXEL_FORMAT_E enDataFmt);
HI_BOOL HAL_LAYER_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer, HI_U32* pu32Fmt);
HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S* pstCscCoef);
HI_BOOL HAL_LAYER_SetCscMode(HAL_DISP_LAYER_E enLayer, HI_BOOL bIsHCModeBy709);
HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn);

HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer, HIFB_RECT *  pstRect);
HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL HAL_LAYER_SetLayerGAlpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U8 u8Alpha0);
HI_BOOL HAL_LAYER_GetLayerGAlpha(HAL_DISP_LAYER_E enLayer, HI_U8* pu8Alpha0);
HI_BOOL HAL_LAYER_SetZmeFirEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetZmeMscEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetHfirOrder(HAL_DISP_LAYER_E enLayer, HI_U32 uHfirOrder);
HI_BOOL HAL_LAYER_SetZmeVerTap(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 uVerTap);
HI_BOOL HAL_LAYER_SetHorRatio(HAL_DISP_LAYER_E enLayer, HI_U32 uRatio);
HI_BOOL HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer);
HI_BOOL HAL_LAYER_SetLayerBgColor(HAL_DISP_LAYER_E enLayer, HAL_VIDEO_LAYER_BKCOLOR_S *stBgColor);
HI_BOOL HAL_DISP_SetLayerBoundaryColor(HAL_DISP_LAYER_E enLayer, HAL_DISP_BOUNDARY_COLOR_S *stBoundaryColor);
HI_BOOL HAL_DISP_SetLayerBoundaryWidth(HAL_DISP_LAYER_E enLayer, HI_U32 u32Width);
HI_BOOL HAL_DISP_SetChnBoundary(HAL_DISP_LAYER_E enLayer, HI_U32 u32AreaNum, const VO_CHN_BOUNDARY_S *pstChnBoundary);


/*****************************************************************************
 Prototype       : graphic layer Relative
*****************************************************************************/
HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr);
HI_BOOL HAL_GRAPHIC_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64* pu64GfxAddr);
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch);
HI_BOOL HAL_GRAPHIC_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32* pu32GfxStride);
HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                              HAL_GFX_BITEXTEND_E enMode);
HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U32 bAlphaEn, HI_U32 bArange,
                                 HI_U8 u8Alpha0, HI_U8 u8Alpha1);
HI_BOOL HAL_GRAPHIC_GetGfxPalpha(HAL_DISP_LAYER_E enLayer, HI_U32* pbAlphaEn,
                                 HI_U8* pu8Alpha0, HI_U8* pu8Alpha1);
HI_BOOL HAL_GRAPHIC_SetGfxPalphaRange(HAL_DISP_LAYER_E enLayer, HI_U32 bArange);

HI_BOOL HAL_GRAPHIC_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable);
HI_BOOL HAL_GRAPHIC_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut);
HI_BOOL HAL_GRAPHIC_SetColorKeyValue(HAL_DISP_LAYER_E enLayer,
                                     HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin);
HI_BOOL HAL_GRAPHIC_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk);

HI_BOOL HAL_GRAPHIC_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32* pbEnable);

//for compress
HI_BOOL HAL_GRAPHIC_SetGfxDcmpEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_GRAPHIC_GetGfxDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable);


HI_BOOL HAL_MDDRC_InitMDDRDetect(HI_VOID);
HI_VOID HAL_MDDRC_GetStatus(HI_U32 *u32Status);
HI_VOID HAL_MDDRC_ClearStatus(HI_U32 u32Status);
HI_VOID HAL_MDDRC_Init(HI_VOID);
HI_VOID HAL_MDDRC_Exit(HI_VOID);


/***************************************************************************************************
*  Begin : Parameter Address distribute
***************************************************************************************************/
HI_VOID HAL_PARA_SetParaAddrVhdChn06(HI_U64 para_addr_vhd_chn06);
HI_VOID HAL_PARA_SetParaUpVhdChn(HI_U32 u32ChnNum);
HI_VOID HAL_FDR_GFX_SetDcmpEn(HI_U32 u32Data, HI_U32 dcmp_en);
/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
HI_VOID HAL_G0_ZME_SetCkGtEn(HI_U32 ck_gt_en);
HI_VOID HAL_G0_ZME_SetOutWidth(HI_U32 out_width);
HI_VOID HAL_G0_ZME_SetHfirEn(HI_U32 hfir_en);
HI_VOID HAL_G0_ZME_SetAhfirMidEn(HI_U32 ahfir_mid_en);
HI_VOID HAL_G0_ZME_SetLhfirMidEn(HI_U32 lhfir_mid_en);
HI_VOID HAL_G0_ZME_SetChfirMidEn(HI_U32 chfir_mid_en);
HI_VOID HAL_G0_ZME_SetLhfirMode(HI_U32 lhfir_mode);
HI_VOID HAL_G0_ZME_SetAhfirMode(HI_U32 ahfir_mode);
HI_VOID HAL_G0_ZME_SetHfirOrder(HI_U32 hfir_order);
HI_VOID HAL_G0_ZME_SetHratio(HI_U32 hratio);
HI_VOID HAL_G0_ZME_SetLhfirOffset(HI_U32 lhfir_offset);
HI_VOID HAL_G0_ZME_SetChfirOffset(HI_U32 chfir_offset);
HI_VOID HAL_G0_ZME_SetOutPro(HI_U32 out_pro);
HI_VOID HAL_G0_ZME_SetOutHeight(HI_U32 out_height);
HI_VOID HAL_G0_ZME_SetVfirEn(HI_U32 vfir_en);
HI_VOID HAL_G0_ZME_SetAvfirMidEn(HI_U32 avfir_mid_en);
HI_VOID HAL_G0_ZME_SetLvfirMidEn(HI_U32 lvfir_mid_en);
HI_VOID HAL_G0_ZME_SetCvfirMidEn(HI_U32 cvfir_mid_en);
HI_VOID HAL_G0_ZME_SetLvfirMode(HI_U32 lvfir_mode);
HI_VOID HAL_G0_ZME_SetVafirMode(HI_U32 vafir_mode);
HI_VOID HAL_G0_ZME_SetVratio(HI_U32 vratio);
HI_VOID HAL_G0_ZME_SetVtpOffset(HI_U32 vtp_offset);
HI_VOID HAL_G0_ZME_SetVbtmOffset(HI_U32 vbtm_offset);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of __VOU_HAL_H__ */

