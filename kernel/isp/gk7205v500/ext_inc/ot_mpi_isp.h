/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __MPI_ISP_H__
#define __MPI_ISP_H__

#include "comm_isp.h"
#include "comm_sns.h"
#include "comm_3a.h"
#include "comm_video.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Firmware Main Operation */
XMEDIA_S32 MPI_ISP_Init(VI_PIPE ViPipe);
XMEDIA_S32 MPI_ISP_MemInit(VI_PIPE ViPipe);
XMEDIA_S32 MPI_ISP_Run(VI_PIPE ViPipe);
XMEDIA_S32 MPI_ISP_RunOnce(VI_PIPE ViPipe);
XMEDIA_S32 MPI_ISP_Exit(VI_PIPE ViPipe);

XMEDIA_S32 MPI_ISP_SensorRegCallBack(VI_PIPE ViPipe, ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo, ISP_SENSOR_REGISTER_S *pstRegister);
XMEDIA_S32 MPI_ISP_SensorUnRegCallBack(VI_PIPE ViPipe, SENSOR_ID SensorId);

/* if have registered multy libs, set bind attr to appoint the active lib. */
XMEDIA_S32 MPI_ISP_SetBindAttr(VI_PIPE ViPipe, const ISP_BIND_ATTR_S *pstBindAttr);
XMEDIA_S32 MPI_ISP_GetBindAttr(VI_PIPE ViPipe, ISP_BIND_ATTR_S *pstBindAttr);
XMEDIA_S32 MPI_ISP_AELibRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                   ISP_AE_REGISTER_S *pstRegister);
XMEDIA_S32 MPI_ISP_AWBLibRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
                                    ISP_AWB_REGISTER_S *pstRegister);
XMEDIA_S32 MPI_ISP_AELibUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib);
XMEDIA_S32 MPI_ISP_AWBLibUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib);

XMEDIA_S32 MPI_ISP_GetLightboxGain(VI_PIPE ViPipe, ISP_AWB_Calibration_Gain_S *pstAWBCalibrationGain);
XMEDIA_S32 MPI_ISP_SetPubAttr(VI_PIPE ViPipe, const ISP_PUB_ATTR_S *pstPubAttr);
XMEDIA_S32 MPI_ISP_GetPubAttr(VI_PIPE ViPipe, ISP_PUB_ATTR_S *pstPubAttr);

XMEDIA_S32 MPI_ISP_SetPipeDifferAttr(VI_PIPE ViPipe, const ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer);
XMEDIA_S32 MPI_ISP_GetPipeDifferAttr(VI_PIPE ViPipe, ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer);

XMEDIA_S32 MPI_ISP_GetVDTimeOut(VI_PIPE ViPipe, ISP_VD_TYPE_E enIspVDType, XMEDIA_U32 u32MilliSec);
XMEDIA_S32 MPI_ISP_QueryInnerStateInfo(VI_PIPE ViPipe, ISP_INNER_STATE_INFO_S *pstInnerStateInfo);

XMEDIA_S32 MPI_ISP_GetAEStatistics(VI_PIPE ViPipe, ISP_AE_STATISTICS_S *pstAeStat);
XMEDIA_S32 MPI_ISP_GetAEStitchStatistics(VI_PIPE ViPipe, ISP_AE_STITCH_STATISTICS_S *pstStitchStat);
XMEDIA_S32 MPI_ISP_GetMGStatistics(VI_PIPE ViPipe, ISP_MG_STATISTICS_S *pstMgStat);
XMEDIA_S32 MPI_ISP_GetWBStatistics(VI_PIPE ViPipe, ISP_WB_STATISTICS_S *pstWBStat);
XMEDIA_S32 MPI_ISP_GetWBStitchStatistics(VI_PIPE ViPipe, ISP_WB_STITCH_STATISTICS_S *pstStitchWBStat);
XMEDIA_S32 MPI_ISP_GetFocusStatistics(VI_PIPE ViPipe, ISP_AF_STATISTICS_S *pstAfStat);

XMEDIA_S32 MPI_ISP_SetStatisticsConfig(VI_PIPE ViPipe, const ISP_STATISTICS_CFG_S *pstStatCfg);
XMEDIA_S32 MPI_ISP_GetStatisticsConfig(VI_PIPE ViPipe, ISP_STATISTICS_CFG_S *pstStatCfg);

XMEDIA_S32 MPI_ISP_GetISPRegAttr(VI_PIPE ViPipe, ISP_REG_ATTR_S *pstIspRegAttr);

XMEDIA_S32 MPI_ISP_SetFMWState(VI_PIPE ViPipe, const ISP_FMW_STATE_E enState);
XMEDIA_S32 MPI_ISP_GetFMWState(VI_PIPE ViPipe, ISP_FMW_STATE_E *penState);

XMEDIA_S32 MPI_ISP_SetDebug(VI_PIPE ViPipe, const ISP_DEBUG_INFO_S *pstIspDebug);
XMEDIA_S32 MPI_ISP_GetDebug(VI_PIPE ViPipe, ISP_DEBUG_INFO_S *pstIspDebug);

XMEDIA_S32 MPI_ISP_SetModParam(const ISP_MOD_PARAM_S *pstModParam);
XMEDIA_S32 MPI_ISP_GetModParam(ISP_MOD_PARAM_S *pstModParam);

XMEDIA_S32 MPI_ISP_SetCtrlParam(VI_PIPE ViPipe, const ISP_CTRL_PARAM_S *pstIspCtrlParam);
XMEDIA_S32 MPI_ISP_GetCtrlParam(VI_PIPE ViPipe, ISP_CTRL_PARAM_S *pstIspCtrlParam);

XMEDIA_S32 MPI_ISP_SetFSWDRAttr(VI_PIPE ViPipe, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr);
XMEDIA_S32 MPI_ISP_GetFSWDRAttr(VI_PIPE ViPipe, ISP_WDR_FS_ATTR_S *pstFSWDRAttr);

XMEDIA_S32 MPI_ISP_SetDRCAttr(VI_PIPE ViPipe, const ISP_DRC_ATTR_S *pstDRC);
XMEDIA_S32 MPI_ISP_GetDRCAttr(VI_PIPE ViPipe, ISP_DRC_ATTR_S *pstDRC);

XMEDIA_S32 MPI_ISP_SetDehazeAttr(VI_PIPE ViPipe, const ISP_DEHAZE_ATTR_S *pstDehazeAttr);
XMEDIA_S32 MPI_ISP_GetDehazeAttr(VI_PIPE ViPipe, ISP_DEHAZE_ATTR_S *pstDehazeAttr);

XMEDIA_S32 MPI_ISP_SetLDCIAttr(VI_PIPE ViPipe, const ISP_LDCI_ATTR_S *pstLDCIAttr);
XMEDIA_S32 MPI_ISP_GetLDCIAttr(VI_PIPE ViPipe, ISP_LDCI_ATTR_S *pstLDCIAttr);

XMEDIA_S32 MPI_ISP_SetSnsSlaveAttr(SLAVE_DEV SlaveDev, const ISP_SLAVE_SNS_SYNC_S *pstSnsSync);
XMEDIA_S32 MPI_ISP_GetSnsSlaveAttr(SLAVE_DEV SlaveDev, ISP_SLAVE_SNS_SYNC_S *pstSnsSync);

XMEDIA_S32 MPI_ISP_SetModuleControl(VI_PIPE ViPipe, const ISP_MODULE_CTRL_U *punModCtrl);
XMEDIA_S32 MPI_ISP_GetModuleControl(VI_PIPE ViPipe, ISP_MODULE_CTRL_U *punModCtrl);

XMEDIA_S32 MPI_ISP_SetDPCalibrate(VI_PIPE ViPipe, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate);
XMEDIA_S32 MPI_ISP_GetDPCalibrate(VI_PIPE ViPipe, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate);

XMEDIA_S32 MPI_ISP_SetDPStaticAttr(VI_PIPE ViPipe, const ISP_DP_STATIC_ATTR_S *pstDPStaticAttr);
XMEDIA_S32 MPI_ISP_GetDPStaticAttr(VI_PIPE ViPipe,  ISP_DP_STATIC_ATTR_S *pstDPStaticAttr);

XMEDIA_S32 MPI_ISP_SetDPDynamicAttr(VI_PIPE ViPipe, const ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr);
XMEDIA_S32 MPI_ISP_GetDPDynamicAttr(VI_PIPE ViPipe, ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr);

XMEDIA_S32 MPI_ISP_SetDISAttr(VI_PIPE ViPipe, const ISP_DIS_ATTR_S *pstDISAttr);
XMEDIA_S32 MPI_ISP_GetDISAttr(VI_PIPE ViPipe, ISP_DIS_ATTR_S *pstDISAttr);

XMEDIA_S32 MPI_ISP_SetMeshShadingAttr(VI_PIPE ViPipe, const ISP_SHADING_ATTR_S *pstShadingAttr);
XMEDIA_S32 MPI_ISP_GetMeshShadingAttr(VI_PIPE ViPipe, ISP_SHADING_ATTR_S *pstShadingAttr);

XMEDIA_S32 MPI_ISP_SetMeshShadingGainLutAttr(VI_PIPE ViPipe, const ISP_SHADING_LUT_ATTR_S *pstShadingGainLutAttr);
XMEDIA_S32 MPI_ISP_GetMeshShadingGainLutAttr(VI_PIPE ViPipe, ISP_SHADING_LUT_ATTR_S *pstShadingGainLutAttr);

XMEDIA_S32 MPI_ISP_SetRadialShadingAttr(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr);
XMEDIA_S32 MPI_ISP_GetRadialShadingAttr(VI_PIPE ViPipe, ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr);

XMEDIA_S32 MPI_ISP_SetRadialShadingLUT(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr);
XMEDIA_S32 MPI_ISP_GetRadialShadingLUT(VI_PIPE ViPipe, ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr);

XMEDIA_S32 MPI_ISP_MeshShadingCalibration(VI_PIPE ViPipe, XMEDIA_U16 *pu16SrcRaw, ISP_MLSC_CALIBRATION_CFG_S *pstMLSCCaliCfg, ISP_MESH_SHADING_TABLE_S *pstMLSCTable);

XMEDIA_S32 MPI_ISP_SetNRAttr(VI_PIPE ViPipe, const ISP_NR_ATTR_S *pstNRAttr);
XMEDIA_S32 MPI_ISP_GetNRAttr(VI_PIPE ViPipe, ISP_NR_ATTR_S *pstNRAttr);

XMEDIA_S32 MPI_ISP_SetDEAttr(VI_PIPE ViPipe, const ISP_DE_ATTR_S *pstDEAttr);
XMEDIA_S32 MPI_ISP_GetDEAttr(VI_PIPE ViPipe, ISP_DE_ATTR_S *pstDEAttr);

XMEDIA_S32 MPI_ISP_SetColorToneAttr(VI_PIPE ViPipe, const ISP_COLOR_TONE_ATTR_S *pstCTAttr);
XMEDIA_S32 MPI_ISP_GetColorToneAttr(VI_PIPE ViPipe, ISP_COLOR_TONE_ATTR_S *pstCTAttr);

XMEDIA_S32 MPI_ISP_SetGammaAttr(VI_PIPE ViPipe, const ISP_GAMMA_ATTR_S *pstGammaAttr);
XMEDIA_S32 MPI_ISP_GetGammaAttr(VI_PIPE ViPipe, ISP_GAMMA_ATTR_S *pstGammaAttr);

XMEDIA_S32 MPI_ISP_SetPreGammaAttr(VI_PIPE ViPipe, const ISP_PREGAMMA_ATTR_S *pstPreGammaAttr);
XMEDIA_S32 MPI_ISP_GetPreGammaAttr(VI_PIPE ViPipe, ISP_PREGAMMA_ATTR_S *pstPreGammaAttr);

XMEDIA_S32 MPI_ISP_SetPreLogLUTAttr(VI_PIPE ViPipe, const ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr);
XMEDIA_S32 MPI_ISP_GetPreLogLUTAttr(VI_PIPE ViPipe, ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr);

XMEDIA_S32 MPI_ISP_SetLogLUTAttr(VI_PIPE ViPipe, const ISP_LOGLUT_ATTR_S *pstLogLUTAttr);
XMEDIA_S32 MPI_ISP_GetLogLUTAttr(VI_PIPE ViPipe, ISP_LOGLUT_ATTR_S *pstLogLUTAttr);

XMEDIA_S32 MPI_ISP_SetLocalCacAttr(VI_PIPE ViPipe, const ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr);
XMEDIA_S32 MPI_ISP_GetLocalCacAttr(VI_PIPE ViPipe, ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr);

XMEDIA_S32 MPI_ISP_SetGlobalCacAttr(VI_PIPE ViPipe, const ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr);
XMEDIA_S32 MPI_ISP_GetGlobalCacAttr(VI_PIPE ViPipe, ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr);

XMEDIA_S32 MPI_ISP_SetRcAttr(VI_PIPE ViPipe, const ISP_RC_ATTR_S *pstRcAttr);
XMEDIA_S32 MPI_ISP_GetRcAttr(VI_PIPE ViPipe, ISP_RC_ATTR_S *pstRcAttr);

XMEDIA_S32 MPI_ISP_SetIspSharpenAttr(VI_PIPE ViPipe, const ISP_SHARPEN_ATTR_S *pstIspShpAttr);
XMEDIA_S32 MPI_ISP_GetIspSharpenAttr(VI_PIPE ViPipe, ISP_SHARPEN_ATTR_S *pstIspShpAttr);

XMEDIA_S32 MPI_ISP_SetIspEdgeMarkAttr(VI_PIPE ViPipe, const ISP_EDGEMARK_ATTR_S *pstIspEdgeMarkAttr);
XMEDIA_S32 MPI_ISP_GetIspEdgeMarkAttr(VI_PIPE ViPipe, ISP_EDGEMARK_ATTR_S *pstIspEdgeMarkAttr);

XMEDIA_S32 MPI_ISP_SetIspHlcAttr(VI_PIPE ViPipe, const ISP_HLC_ATTR_S *pstIspHlcAttr);
XMEDIA_S32 MPI_ISP_GetIspHlcAttr(VI_PIPE ViPipe, ISP_HLC_ATTR_S *pstIspHlcAttr);

XMEDIA_S32 MPI_ISP_SetCrosstalkAttr(VI_PIPE ViPipe, const ISP_CR_ATTR_S *pstCRAttr);
XMEDIA_S32 MPI_ISP_GetCrosstalkAttr(VI_PIPE ViPipe, ISP_CR_ATTR_S *pstCRAttr);

XMEDIA_S32 MPI_ISP_SetCAAttr(VI_PIPE ViPipe, const ISP_CA_ATTR_S *pstCAAttr);
XMEDIA_S32 MPI_ISP_GetCAAttr(VI_PIPE ViPipe, ISP_CA_ATTR_S *pstCAAttr);

XMEDIA_S32 MPI_ISP_SetAntiFalseColorAttr(VI_PIPE ViPipe, const ISP_ANTIFALSECOLOR_ATTR_S *pstAntiFalseColor);
XMEDIA_S32 MPI_ISP_GetAntiFalseColorAttr(VI_PIPE ViPipe, ISP_ANTIFALSECOLOR_ATTR_S *pstAntiFalseColor);

XMEDIA_S32 MPI_ISP_SetDemosaicAttr(VI_PIPE ViPipe, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr);
XMEDIA_S32 MPI_ISP_GetDemosaicAttr(VI_PIPE ViPipe, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr);

XMEDIA_S32 MPI_ISP_SetBlackLevelAttr(VI_PIPE ViPipe, const ISP_BLACK_LEVEL_S *pstBlackLevel);
XMEDIA_S32 MPI_ISP_GetBlackLevelAttr(VI_PIPE ViPipe, ISP_BLACK_LEVEL_S *pstBlackLevel);

XMEDIA_S32 MPI_ISP_SetClutAttr(VI_PIPE ViPipe, const ISP_CLUT_ATTR_S *pstClutAttr);
XMEDIA_S32 MPI_ISP_GetClutAttr(VI_PIPE ViPipe, ISP_CLUT_ATTR_S *pstClutAttr);

XMEDIA_S32 MPI_ISP_SetClutCoeff(VI_PIPE ViPipe, const ISP_CLUT_LUT_S *pstClutLUT);
XMEDIA_S32 MPI_ISP_GetClutCoeff(VI_PIPE ViPipe, ISP_CLUT_LUT_S *pstClutLUT);

XMEDIA_S32 MPI_ISP_SetDCFInfo(VI_PIPE ViPipe, const ISP_DCF_INFO_S *pstIspDCF);
XMEDIA_S32 MPI_ISP_GetDCFInfo(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF);

XMEDIA_S32 MPI_ISP_SetCSCAttr(VI_PIPE ViPipe, const ISP_CSC_ATTR_S *pstCSCAttr);
XMEDIA_S32 MPI_ISP_GetCSCAttr(VI_PIPE ViPipe, ISP_CSC_ATTR_S *pstCSCFAttr);

XMEDIA_S32 MPI_ISP_SetRGBIRAttr(VI_PIPE ViPipe, const ISP_RGBIR_ATTR_S *pstRGBIRAttr);
XMEDIA_S32 MPI_ISP_GetRGBIRAttr(VI_PIPE ViPipe, ISP_RGBIR_ATTR_S *pstRGBIRAttr);
XMEDIA_S32 MPI_ISP_SetRegister(VI_PIPE ViPipe, XMEDIA_U32 u32Addr, XMEDIA_U32 u32Value);
XMEDIA_S32 MPI_ISP_GetRegister(VI_PIPE ViPipe, XMEDIA_U32 u32Addr, XMEDIA_U32 *pu32Value);

/* block, return fpn frame when this is function is called. */
XMEDIA_S32 MPI_ISP_FPNCalibrate(VI_PIPE ViPipe, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr);
XMEDIA_S32 MPI_ISP_SetFPNAttr(VI_PIPE ViPipe, const ISP_FPN_ATTR_S *pstFPNAttr);
XMEDIA_S32 MPI_ISP_GetFPNAttr(VI_PIPE ViPipe, ISP_FPN_ATTR_S *pstFPNAttr);

XMEDIA_S32 MPI_ISP_GetDngImageStaticInfo(VI_PIPE ViPipe, DNG_IMAGE_STATIC_INFO_S *pstDngImageStaticInfo);
XMEDIA_S32 MPI_ISP_SetDngColorParam(VI_PIPE ViPipe, const ISP_DNG_COLORPARAM_S *pstDngColorParam);
XMEDIA_S32 MPI_ISP_GetDngColorParam(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam);
XMEDIA_S32 MPI_ISP_IrAutoRunOnce(VI_PIPE ViPipe, ISP_IR_AUTO_ATTR_S *pstIrAttr);

XMEDIA_S32 MPI_ISP_SetSmartInfo(VI_PIPE ViPipe, const ISP_SMART_INFO_S *pstSmartInfo);
XMEDIA_S32 MPI_ISP_GetSmartInfo(VI_PIPE ViPipe, ISP_SMART_INFO_S *pstSmartInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __MPI_ISP_H__ */
