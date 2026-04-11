/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/12/20
  Description   : 
  History       :
  1.Date        : 2010/12/20
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __MPI_ISP_H__
#define __MPI_ISP_H__

#include "hi_comm_isp.h"
#include "hi_comm_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


HI_S32 HI_MPI_ISP_FreezeFmw(HI_BOOL bFreeze);

HI_S32 HI_MPI_ISP_SetModuleControl(HI_U32 u32ModFlag);
HI_S32 HI_MPI_ISP_GetModuleControl(HI_U32 *pu32ModFlag);

HI_S32 HI_MPI_ISP_GetExtRegAddr(HI_U32 *u32Addr);


/* Exposure Settings */

HI_S32 HI_MPI_ISP_SetExposureType(ISP_OP_TYPE_E enExpType);
HI_S32 HI_MPI_ISP_GetExposureType(ISP_OP_TYPE_E *penExpType);

HI_S32 HI_MPI_ISP_SetAEAttr(const ISP_AE_ATTR_S *pstAEAttr);
HI_S32 HI_MPI_ISP_GetAEAttr(ISP_AE_ATTR_S *pstAEAttr);

HI_S32 HI_MPI_ISP_SetAEAttrEx(const ISP_AE_ATTR_EX_S *pstAEAttrEx);
HI_S32 HI_MPI_ISP_GetAEAttrEx(ISP_AE_ATTR_EX_S *pstAEAttrEx);

HI_S32  HI_MPI_ISP_SetExpStaInfo(ISP_EXP_STA_INFO_S *pstExpStatistic);
HI_S32  HI_MPI_ISP_GetExpStaInfo(ISP_EXP_STA_INFO_S *pstExpStatistic);

HI_S32 HI_MPI_ISP_SetMEAttr(const ISP_ME_ATTR_S *pstMEAttr);
HI_S32 HI_MPI_ISP_GetMEAttr(ISP_ME_ATTR_S *pstMEAttr);

HI_S32 HI_MPI_ISP_SetMEAttrEx(const ISP_ME_ATTR_EX_S *pstMEAttrEx);
HI_S32 HI_MPI_ISP_GetMEAttrEx(ISP_ME_ATTR_EX_S *pstMEAttrEx);

HI_S32 HI_MPI_ISP_SetSlowFrameRate(HI_U8 u8Value);
HI_S32 HI_MPI_ISP_GetSlowFrameRate(HI_U8 *pu8Value);

/* White Balance Settings */

HI_S32 HI_MPI_ISP_SetWBType(ISP_OP_TYPE_E enWBType);
HI_S32 HI_MPI_ISP_GetWBType(ISP_OP_TYPE_E *penWBType);

HI_S32 HI_MPI_ISP_SetAWBAttr(const ISP_AWB_ATTR_S *pstAWBAttr);
HI_S32 HI_MPI_ISP_GetAWBAttr(ISP_AWB_ATTR_S *pstAWBAttr);

HI_S32 HI_MPI_ISP_SetMWBAttr(const ISP_MWB_ATTR_S *pstMWBAttr);
HI_S32 HI_MPI_ISP_GetMWBAttr(ISP_MWB_ATTR_S *pstMWBAttr);

HI_S32 HI_MPI_ISP_SetColorTemp(const HI_U16 u16ColorTemp);     //not support yet
HI_S32 HI_MPI_ISP_GetColorTemp(HI_U16 *pu16ColorTemp);

HI_S32 HI_MPI_ISP_SetColorTone(ISP_COLORTONE_S *pstColorTone);
HI_S32 HI_MPI_ISP_GetColorTone(ISP_COLORTONE_S *pstColorTone);
HI_S32 HI_MPI_ISP_SetWBStaInfo(ISP_WB_STA_INFO_S *pstWBStatistic);
HI_S32 HI_MPI_ISP_GetWBStaInfo(ISP_WB_STA_INFO_S *pstWBStatistic);

HI_S32 HI_MPI_ISP_SetAWBAlgType(ISP_AWB_ALG_TYPE_E enALGType);
HI_S32 HI_MPI_ISP_GetAWBAlgType(ISP_AWB_ALG_TYPE_E *penALGType);

HI_S32 HI_MPI_ISP_SetAdvAWBAttr(ISP_ADV_AWB_ATTR_S *pstAdvAWBAttr);
HI_S32 HI_MPI_ISP_GetAdvAWBAttr(ISP_ADV_AWB_ATTR_S *pstAdvAWBAttr);

HI_S32 HI_MPI_ISP_SetLightSource(const ISP_AWB_ADD_LIGHTSOURCE_S *pstLightSource);
HI_S32 HI_MPI_ISP_GetLightSource(ISP_AWB_ADD_LIGHTSOURCE_S *pstLightSource);

/* Focus Settings */

HI_S32 HI_MPI_ISP_SetFocusType(ISP_OP_TYPE_E enFocusType);    //not support yet
HI_S32 HI_MPI_ISP_GetFocusType(ISP_OP_TYPE_E *penFocusType);  //not support yet

HI_S32 HI_MPI_ISP_SetAFAttr(const ISP_AF_ATTR_S *pstAFAttr);  //not support yet
HI_S32 HI_MPI_ISP_GetAFAttr(ISP_AF_ATTR_S *pstAFAttr);        //not support yet

HI_S32 HI_MPI_ISP_SetMFAttr(const ISP_MF_ATTR_S *pstMFAttr);  //not support yet
HI_S32 HI_MPI_ISP_GetMFAttr(ISP_MF_ATTR_S *pstMFAttr);        //not support yet

HI_S32 HI_MPI_ISP_ManualFocusMove(HI_S32 s32MoveSteps);       //not support yet

HI_S32 HI_MPI_ISP_SetFocusStaInfo(ISP_FOCUS_STA_INFO_S *pstFocusStatistic);
HI_S32 HI_MPI_ISP_GetFocusStaInfo(ISP_FOCUS_STA_INFO_S *pstFocusStatistic);


/* Iris Settings */

HI_S32 HI_MPI_ISP_SetIrisType(ISP_OP_TYPE_E enIrisType);//not support yet
HI_S32 HI_MPI_ISP_GetIrisType(ISP_OP_TYPE_E *penIrisType);    //not support yet

HI_S32 HI_MPI_ISP_SetAIAttr(const ISP_AI_ATTR_S *pstAIAttr);
HI_S32 HI_MPI_ISP_GetAIAttr(ISP_AI_ATTR_S *pstAIAttr);

HI_S32 HI_MPI_ISP_SetMIAttr(const ISP_MI_ATTR_S *pstMIAttr);  //not support yet
HI_S32 HI_MPI_ISP_GetMIAttr(ISP_MI_ATTR_S *pstMIAttr);        //not support yet


/* General Function Settings */

HI_S32 HI_MPI_ISP_SetDRCAttr(const ISP_DRC_ATTR_S *pstDRC);
HI_S32 HI_MPI_ISP_GetDRCAttr(ISP_DRC_ATTR_S *pstDRC);

HI_S32 HI_MPI_ISP_SetDefectPixelAttr(const ISP_DP_ATTR_S *pstDPAttr);
HI_S32 HI_MPI_ISP_GetDefectPixelAttr(ISP_DP_ATTR_S *pstDPAttr);

HI_S32 HI_MPI_ISP_SetDISAttr(const ISP_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_GetDISAttr(ISP_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_GetDISInfo(ISP_DIS_INFO_S *pstDISInfo);

HI_S32 HI_MPI_ISP_SetShadingAttr(const ISP_SHADING_ATTR_S *pstShadingAttr);
HI_S32 HI_MPI_ISP_GetShadingAttr(ISP_SHADING_ATTR_S *pstShadingAttr);

HI_S32 HI_MPI_ISP_SetShadingTable(const ISP_SHADINGTAB_S *pstShadingTab);
HI_S32 HI_MPI_ISP_GetShadingTable(ISP_SHADINGTAB_S *pstShadingTab);

HI_S32 HI_MPI_ISP_SetDenoiseAttr(const ISP_DENOISE_ATTR_S *pstDenoiseAttr);
HI_S32 HI_MPI_ISP_GetDenoiseAttr(ISP_DENOISE_ATTR_S *pstDenoiseAttr);

HI_S32 HI_MPI_ISP_SetGammaAttr(const ISP_GAMMA_ATTR_S* pstGammaAttr);
HI_S32 HI_MPI_ISP_GetGammaAttr(ISP_GAMMA_ATTR_S* pstGammaAttr);

HI_S32 HI_MPI_ISP_SetGammaTable(ISP_GAMMA_TABLE_S* pstGammaAttr);
HI_S32 HI_MPI_ISP_GetGammaTable(ISP_GAMMA_TABLE_S* pstGammaAttr);

HI_S32 HI_MPI_ISP_SetGammaFETable(ISP_GAMMA_TABLE_S* pstGammaAttr);
HI_S32 HI_MPI_ISP_GetGammaFETable(ISP_GAMMA_TABLE_S* pstGammaAttr);

HI_S32 HI_MPI_ISP_SetSharpenAttr(const ISP_SHARPEN_ATTR_S *pstSharpenAttr);
HI_S32 HI_MPI_ISP_GetSharpenAttr(ISP_SHARPEN_ATTR_S *pstSharpenAttr);

HI_S32 HI_MPI_ISP_SetAntiFlickerAttr(const ISP_ANTIFLICKER_S *pstAntiflicker);
HI_S32 HI_MPI_ISP_GetAntiFlickerAttr(ISP_ANTIFLICKER_S *pstAntiflicker);

HI_S32 HI_MPI_ISP_SetCCM(const ISP_COLORMATRIX_S *pstColorMatrix);
HI_S32 HI_MPI_ISP_GetCCM(ISP_COLORMATRIX_S *pstColorMatrix);

HI_S32 HI_MPI_ISP_SetSaturationAttr(const ISP_SATURATION_ATTR_S *pstSatAttr);
HI_S32 HI_MPI_ISP_GetSaturationAttr(ISP_SATURATION_ATTR_S *pstSatAttr);

HI_S32 HI_MPI_ISP_SetSaturation(HI_U32 u32Value);
HI_S32 HI_MPI_ISP_GetSaturation(HI_U32 *pu32Value);

HI_S32 HI_MPI_ISP_SetCfg(HI_U32 u32Addr, HI_U32 u32Value);
HI_S32 HI_MPI_ISP_GetCfg(HI_U32 u32Addr, HI_U32 *pu32Value);

HI_S32 HI_MPI_ISP_SetCrosstalkAttr(const ISP_CR_ATTR_S *pstCRAttr);
HI_S32 HI_MPI_ISP_GetCrosstalkAttr(ISP_CR_ATTR_S *pstCRAttr);

HI_S32 HI_MPI_ISP_SetNoiseProfileTable(ISP_NOISEPROFILE_TABLE_S *pstNoiseProfileTable);
HI_S32 HI_MPI_ISP_GetNoiseProfileTable(ISP_NOISEPROFILE_TABLE_S *pstNoiseProfileTable);
// TODO:
HI_S32 HI_MPI_ISP_SetAntiFogAttr(const ISP_ANTIFOG_S *pstAntiFog);
HI_S32 HI_MPI_ISP_GetAntiFogAttr(ISP_ANTIFOG_S *pstAntiFog);

/* ANTI_FALSECOLOR */
// TODO:
HI_S32 HI_MPI_ISP_SetAntiFalseColorAttr(const ISP_ANTI_FALSECOLOR_S *pstAntiFC);
HI_S32 HI_MPI_ISP_GetAntiFalseColorAttr(ISP_ANTI_FALSECOLOR_S *pstAntiFC);

HI_S32 HI_MPI_ISP_SetDemosaicAttr(const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr);
HI_S32 HI_MPI_ISP_GetDemosaicAttr(ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr);

HI_S32 HI_MPI_ISP_SetBlackLevelAttr(const ISP_BLACK_LEVEL_S *pstBlackLevel); 
HI_S32 HI_MPI_ISP_GetBlackLevelAttr(ISP_BLACK_LEVEL_S *pstBlackLevel); 


/* Sensor Relative Operation */

HI_S32 HI_MPI_ISP_SensorRegCallBack(SENSOR_EXP_FUNC_S *pstSensorExpFuncs);


/* Firmware Main Operation */

HI_S32 HI_MPI_ISP_SetInputTiming(const ISP_INPUT_TIMING_S *pstInputTiming);
HI_S32 HI_MPI_ISP_GetInputTiming(ISP_INPUT_TIMING_S *pstInputTiming);

HI_S32 HI_MPI_ISP_SetImageAttr(const ISP_IMAGE_ATTR_S *pstImageAttr);
HI_S32 HI_MPI_ISP_GetImageAttr(ISP_IMAGE_ATTR_S *pstImageAttr);

HI_S32 HI_MPI_ISP_Init(void);

HI_S32 HI_MPI_ISP_Run(void);

HI_S32 HI_MPI_ISP_Exit(void);

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(ISP_INNER_STATE_INFO_S *pstInnerStateInfo);
HI_S32 HI_MPI_ISP_QueryInnerStateInfoEx(ISP_INNER_STATE_INFO_EX_S *pstInnerStateInfoEx);

HI_S32 HI_MPI_ISP_SetDebug(const ISP_DEBUG_INFO_S *pstIspDebug);
HI_S32 HI_MPI_ISP_GetDebug(ISP_DEBUG_INFO_S *pstIspDebug);

HI_S32 HI_MPI_ISP_I2cWrite(ISP_I2C_DATA_S *pstI2cData);
HI_S32 HI_MPI_ISP_SSPWrite(const ISP_SSP_DATA_S *pstSspData);

HI_S32  HI_MPI_ISP_GetVDTimeOut(ISP_VD_INFO_S *pstIspVdInfo, HI_U32 u32MilliSec);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*__MPI_ISP_H__ */

