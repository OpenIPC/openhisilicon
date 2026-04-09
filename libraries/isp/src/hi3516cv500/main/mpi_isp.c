/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mpi_isp.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "mpi_isp.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>

#include "mpi_sys.h"
#include "mpi_isp_adapt.h"
#include "mkp_isp.h"
#include "hi_comm_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "isp_inner.h"

HI_S32 HI_MPI_ISP_SetPubAttr(VI_PIPE ViPipe, const ISP_PUB_ATTR_S *pstPubAttr)
{
    MPI_ASSERT(sizeof(ISP_PUB_ATTR_S) == sizeof(hi_isp_pub_attr));
    return hi_mpi_isp_set_pub_attr(ViPipe, (hi_isp_pub_attr *)pstPubAttr);
}

HI_S32 HI_MPI_ISP_GetPubAttr(VI_PIPE ViPipe, ISP_PUB_ATTR_S *pstPubAttr)
{
    MPI_ASSERT(sizeof(ISP_PUB_ATTR_S) == sizeof(hi_isp_pub_attr));
    return hi_mpi_isp_get_pub_attr(ViPipe, (hi_isp_pub_attr *)pstPubAttr);
}

HI_S32 HI_MPI_ISP_SetPipeDifferAttr(VI_PIPE ViPipe, const ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer)
{
    MPI_ASSERT(sizeof(ISP_PIPE_DIFF_ATTR_S) == sizeof(hi_isp_pipe_diff_attr));
    return hi_mpi_isp_set_pipe_differ_attr(ViPipe, (hi_isp_pipe_diff_attr *)pstPipeDiffer);
}

HI_S32 HI_MPI_ISP_GetPipeDifferAttr(VI_PIPE ViPipe, ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer)
{
    MPI_ASSERT(sizeof(ISP_PIPE_DIFF_ATTR_S) == sizeof(hi_isp_pipe_diff_attr));
    return hi_mpi_isp_get_pipe_differ_attr(ViPipe, (hi_isp_pipe_diff_attr *)pstPipeDiffer);
}

HI_S32 HI_MPI_ISP_SetModuleControl(VI_PIPE ViPipe, const ISP_MODULE_CTRL_U *punModCtrl)
{
    MPI_ASSERT(sizeof(ISP_MODULE_CTRL_U) == sizeof(hi_isp_module_ctrl));
    return hi_mpi_isp_set_module_control(ViPipe, (hi_isp_module_ctrl *)punModCtrl);
}

HI_S32 HI_MPI_ISP_GetModuleControl(VI_PIPE ViPipe, ISP_MODULE_CTRL_U *punModCtrl)
{
    MPI_ASSERT(sizeof(ISP_MODULE_CTRL_U) == sizeof(hi_isp_module_ctrl));
    return hi_mpi_isp_get_module_control(ViPipe, (hi_isp_module_ctrl *)punModCtrl);
}

HI_S32 HI_MPI_ISP_SetRawPos(VI_PIPE ViPipe, const  ISP_RAW_POS *rawPos)
{
    MPI_ASSERT(sizeof(ISP_RAW_POS) == sizeof(hi_isp_raw_pos_attr));
    return hi_mpi_isp_set_raw_pos(ViPipe, (hi_isp_raw_pos_attr *)rawPos);
}

HI_S32 HI_MPI_ISP_GetRawPos(VI_PIPE ViPipe,   ISP_RAW_POS *rawPos)
{
    MPI_ASSERT(sizeof(ISP_RAW_POS) == sizeof(hi_isp_raw_pos_attr));
    return hi_mpi_isp_get_raw_pos(ViPipe, (hi_isp_raw_pos_attr *)rawPos);
}

HI_S32 HI_MPI_ISP_SetFMWState(VI_PIPE ViPipe, const ISP_FMW_STATE_E enState)
{
    MPI_ASSERT(sizeof(ISP_FMW_STATE_E) == sizeof(hi_isp_fmw_state));
    return hi_mpi_isp_set_fmw_state(ViPipe, (hi_isp_fmw_state)enState);
}

HI_S32 HI_MPI_ISP_GetFMWState(VI_PIPE ViPipe, ISP_FMW_STATE_E *penState)
{
    MPI_ASSERT(sizeof(ISP_FMW_STATE_E) == sizeof(hi_isp_fmw_state));
    return hi_mpi_isp_get_fmw_state(ViPipe, (hi_isp_fmw_state *)penState);
}

HI_S32 HI_MPI_ISP_SetLDCIAttr(VI_PIPE ViPipe, const ISP_LDCI_ATTR_S *pstLDCIAttr)
{
    MPI_ASSERT(sizeof(ISP_LDCI_ATTR_S) == sizeof(hi_isp_ldci_attr));
    return hi_mpi_isp_set_ldci_attr(ViPipe, (hi_isp_ldci_attr *)pstLDCIAttr);
}

HI_S32 HI_MPI_ISP_GetLDCIAttr(VI_PIPE ViPipe, ISP_LDCI_ATTR_S *pstLDCIAttr)
{
    MPI_ASSERT(sizeof(ISP_LDCI_ATTR_S) == sizeof(hi_isp_ldci_attr));
    return hi_mpi_isp_get_ldci_attr(ViPipe, (hi_isp_ldci_attr *)pstLDCIAttr);
}

/* General Function Settings */
HI_S32 HI_MPI_ISP_SetDRCAttr(VI_PIPE ViPipe, const ISP_DRC_ATTR_S *pstDRC)
{
    MPI_ASSERT(sizeof(ISP_DRC_ATTR_S) == sizeof(hi_isp_drc_attr));
    return hi_mpi_isp_set_drc_attr(ViPipe, (hi_isp_drc_attr *)pstDRC);
}

HI_S32 HI_MPI_ISP_GetDRCAttr(VI_PIPE ViPipe, ISP_DRC_ATTR_S *pstDRC)
{
    MPI_ASSERT(sizeof(ISP_DRC_ATTR_S) == sizeof(hi_isp_drc_attr));
    return hi_mpi_isp_get_drc_attr(ViPipe, (hi_isp_drc_attr *)pstDRC);
}

HI_S32 HI_MPI_ISP_SetDehazeAttr(VI_PIPE ViPipe, const ISP_DEHAZE_ATTR_S *pstDehazeAttr)
{
    MPI_ASSERT(sizeof(ISP_DEHAZE_ATTR_S) == sizeof(hi_isp_dehaze_attr));
    return hi_mpi_isp_set_dehaze_attr(ViPipe, (hi_isp_dehaze_attr *)pstDehazeAttr);
}

HI_S32 HI_MPI_ISP_GetDehazeAttr(VI_PIPE ViPipe, ISP_DEHAZE_ATTR_S *pstDehazeAttr)
{
    MPI_ASSERT(sizeof(ISP_DEHAZE_ATTR_S) == sizeof(hi_isp_dehaze_attr));
    return hi_mpi_isp_get_dehaze_attr(ViPipe, (hi_isp_dehaze_attr *)pstDehazeAttr);
}

HI_S32 HI_MPI_ISP_SetDISAttr(VI_PIPE ViPipe, const ISP_DIS_ATTR_S *pstDISAttr)
{
    MPI_ASSERT(sizeof(ISP_DIS_ATTR_S) == sizeof(hi_isp_dis_attr));
    return hi_mpi_isp_set_dis_attr(ViPipe, (hi_isp_dis_attr *)pstDISAttr);
}

HI_S32 HI_MPI_ISP_GetDISAttr(VI_PIPE ViPipe, ISP_DIS_ATTR_S *pstDISAttr)
{
    MPI_ASSERT(sizeof(ISP_DIS_ATTR_S) == sizeof(hi_isp_dis_attr));
    return hi_mpi_isp_get_dis_attr(ViPipe, (hi_isp_dis_attr *)pstDISAttr);
}

HI_S32 HI_MPI_ISP_SetFSWDRAttr(VI_PIPE ViPipe, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    MPI_ASSERT(sizeof(ISP_WDR_FS_ATTR_S) == sizeof(hi_isp_wdr_fs_attr));
    return hi_mpi_isp_set_fswdr_attr(ViPipe, (hi_isp_wdr_fs_attr *)pstFSWDRAttr);
}

HI_S32 HI_MPI_ISP_GetFSWDRAttr(VI_PIPE ViPipe, ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    MPI_ASSERT(sizeof(ISP_WDR_FS_ATTR_S) == sizeof(hi_isp_wdr_fs_attr));
    return hi_mpi_isp_get_fswdr_attr(ViPipe, (hi_isp_wdr_fs_attr *)pstFSWDRAttr);
}

HI_S32 HI_MPI_ISP_GetFocusStatistics(VI_PIPE ViPipe, ISP_AF_STATISTICS_S *pstAfStat)
{
    MPI_ASSERT(sizeof(ISP_AF_STATISTICS_S) == sizeof(hi_isp_af_statistics));
    return hi_mpi_isp_get_focus_statistics(ViPipe, (hi_isp_af_statistics *)pstAfStat);
}

HI_S32 HI_MPI_ISP_GetAEStatistics(VI_PIPE ViPipe, ISP_AE_STATISTICS_S *pstAeStat)
{
    MPI_ASSERT(sizeof(ISP_AE_STATISTICS_S) == sizeof(hi_isp_ae_statistics));
    return hi_mpi_isp_get_ae_statistics(ViPipe, (hi_isp_ae_statistics *)pstAeStat);
}

HI_S32 HI_MPI_ISP_GetAEStitchStatistics(VI_PIPE ViPipe, ISP_AE_STITCH_STATISTICS_S *pstAeStitchStat)
{
    MPI_ASSERT(sizeof(ISP_AE_STITCH_STATISTICS_S) == sizeof(hi_isp_ae_stitch_statistics));
    return hi_mpi_isp_get_ae_stitch_statistics(ViPipe, (hi_isp_ae_stitch_statistics *)pstAeStitchStat);
}

HI_S32 HI_MPI_ISP_GetMGStatistics(VI_PIPE ViPipe, ISP_MG_STATISTICS_S *pstMgStat)
{
    MPI_ASSERT(sizeof(ISP_MG_STATISTICS_S) == sizeof(hi_isp_mg_statistics));
    return hi_mpi_isp_get_mg_statistics(ViPipe, (hi_isp_mg_statistics *)pstMgStat);
}

HI_S32 HI_MPI_ISP_GetWBStitchStatistics(VI_PIPE ViPipe, ISP_WB_STITCH_STATISTICS_S *pstStitchWBStat)
{
    MPI_ASSERT(sizeof(ISP_WB_STITCH_STATISTICS_S) == sizeof(hi_isp_wb_stitch_statistics));
    return hi_mpi_isp_get_wb_stitch_statistics(ViPipe, (hi_isp_wb_stitch_statistics *)pstStitchWBStat);
}

HI_S32 HI_MPI_ISP_GetWBStatistics(VI_PIPE ViPipe, ISP_WB_STATISTICS_S *pstWBStat)
{
    MPI_ASSERT(sizeof(ISP_WB_STATISTICS_S) == sizeof(hi_isp_wb_statistics));
    return hi_mpi_isp_get_wb_statistics(ViPipe, (hi_isp_wb_statistics *)pstWBStat);
}

HI_S32 HI_MPI_ISP_SetStatisticsConfig(VI_PIPE ViPipe, const ISP_STATISTICS_CFG_S *pstStatCfg)
{
    MPI_ASSERT(sizeof(ISP_STATISTICS_CFG_S) == sizeof(hi_isp_statistics_cfg));
    return hi_mpi_isp_set_statistics_config(ViPipe, (hi_isp_statistics_cfg *)pstStatCfg);
}

HI_S32 HI_MPI_ISP_GetStatisticsConfig(VI_PIPE ViPipe, ISP_STATISTICS_CFG_S *pstStatCfg)
{
    MPI_ASSERT(sizeof(ISP_STATISTICS_CFG_S) == sizeof(hi_isp_statistics_cfg));
    return hi_mpi_isp_get_statistics_config(ViPipe, (hi_isp_statistics_cfg *)pstStatCfg);
}

HI_S32 HI_MPI_ISP_SetGammaAttr(VI_PIPE ViPipe, const ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    MPI_ASSERT(sizeof(ISP_GAMMA_ATTR_S) == sizeof(hi_isp_gamma_attr));
    return hi_mpi_isp_set_gamma_attr(ViPipe, (hi_isp_gamma_attr *)pstGammaAttr);
}

HI_S32 HI_MPI_ISP_GetGammaAttr(VI_PIPE ViPipe, ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    MPI_ASSERT(sizeof(ISP_GAMMA_ATTR_S) == sizeof(hi_isp_gamma_attr));
    return hi_mpi_isp_get_gamma_attr(ViPipe, (hi_isp_gamma_attr *)pstGammaAttr);
}

HI_S32 HI_MPI_ISP_SetPreGammaAttr(VI_PIPE ViPipe, const ISP_PREGAMMA_ATTR_S *pstPreGammaAttr)
{
    MPI_ASSERT(sizeof(ISP_PREGAMMA_ATTR_S) == sizeof(hi_isp_pregamma_attr));
    return hi_mpi_isp_set_pre_gamma_attr(ViPipe, (hi_isp_pregamma_attr *)pstPreGammaAttr);
}

HI_S32 HI_MPI_ISP_GetPreGammaAttr(VI_PIPE ViPipe, ISP_PREGAMMA_ATTR_S *pstPreGammaAttr)
{
    MPI_ASSERT(sizeof(ISP_PREGAMMA_ATTR_S) == sizeof(hi_isp_pregamma_attr));
    return hi_mpi_isp_get_pre_gamma_attr(ViPipe, (hi_isp_pregamma_attr *)pstPreGammaAttr);
}

HI_S32 HI_MPI_ISP_SetPreLogLUTAttr(VI_PIPE ViPipe, const ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr)
{
    MPI_ASSERT(sizeof(ISP_PRELOGLUT_ATTR_S) == sizeof(hi_isp_preloglut_attr));
    return hi_mpi_isp_set_pre_log_lut_attr(ViPipe, (hi_isp_preloglut_attr *)pstPreLogLUTAttr);
}

HI_S32 HI_MPI_ISP_GetPreLogLUTAttr(VI_PIPE ViPipe, ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr)
{
    MPI_ASSERT(sizeof(ISP_PRELOGLUT_ATTR_S) == sizeof(hi_isp_preloglut_attr));
    return hi_mpi_isp_get_pre_log_lut_attr(ViPipe, (hi_isp_preloglut_attr *)pstPreLogLUTAttr);
}

HI_S32 HI_MPI_ISP_SetLogLUTAttr(VI_PIPE ViPipe, const ISP_LOGLUT_ATTR_S *pstLogLUTAttr)
{
    MPI_ASSERT(sizeof(ISP_LOGLUT_ATTR_S) == sizeof(hi_isp_loglut_attr));
    return hi_mpi_isp_set_log_lut_attr(ViPipe, (hi_isp_loglut_attr *)pstLogLUTAttr);
}

HI_S32 HI_MPI_ISP_GetLogLUTAttr(VI_PIPE ViPipe, ISP_LOGLUT_ATTR_S *pstLogLUTAttr)
{
    MPI_ASSERT(sizeof(ISP_LOGLUT_ATTR_S) == sizeof(hi_isp_loglut_attr));
    return hi_mpi_isp_get_log_lut_attr(ViPipe, (hi_isp_loglut_attr *)pstLogLUTAttr);
}

HI_S32 HI_MPI_ISP_SetCSCAttr(VI_PIPE ViPipe, const ISP_CSC_ATTR_S *pstCSCAttr)
{
    MPI_ASSERT(sizeof(ISP_CSC_ATTR_S) == sizeof(hi_isp_csc_attr));
    return hi_mpi_isp_set_csc_attr(ViPipe, (hi_isp_csc_attr *)pstCSCAttr);
}

HI_S32 HI_MPI_ISP_GetCSCAttr(VI_PIPE ViPipe, ISP_CSC_ATTR_S *pstCSCAttr)
{
    MPI_ASSERT(sizeof(ISP_CSC_ATTR_S) == sizeof(hi_isp_csc_attr));
    return hi_mpi_isp_get_csc_attr(ViPipe, (hi_isp_csc_attr *)pstCSCAttr);
}

HI_S32 HI_MPI_ISP_SetDPCalibrate(VI_PIPE ViPipe, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    MPI_ASSERT(sizeof(ISP_DP_STATIC_CALIBRATE_S) == sizeof(hi_isp_dp_static_calibrate));
    return hi_mpi_isp_set_dp_calibrate(ViPipe, (hi_isp_dp_static_calibrate *)pstDPCalibrate);
}

HI_S32 HI_MPI_ISP_GetDPCalibrate(VI_PIPE ViPipe, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    MPI_ASSERT(sizeof(ISP_DP_STATIC_CALIBRATE_S) == sizeof(hi_isp_dp_static_calibrate));
    return hi_mpi_isp_get_dp_calibrate(ViPipe, (hi_isp_dp_static_calibrate *)pstDPCalibrate);
}

HI_S32 HI_MPI_ISP_SetDPStaticAttr(VI_PIPE ViPipe, const ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
    MPI_ASSERT(sizeof(ISP_DP_STATIC_ATTR_S) == sizeof(hi_isp_dp_static_attr));
    return hi_mpi_isp_set_dp_static_attr(ViPipe, (hi_isp_dp_static_attr *)pstDPStaticAttr);
}

HI_S32 HI_MPI_ISP_GetDPStaticAttr(VI_PIPE ViPipe, ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
    MPI_ASSERT(sizeof(ISP_DP_STATIC_ATTR_S) == sizeof(hi_isp_dp_static_attr));
    return hi_mpi_isp_get_dp_static_attr(ViPipe, (hi_isp_dp_static_attr *)pstDPStaticAttr);
}

HI_S32 HI_MPI_ISP_SetDPDynamicAttr(VI_PIPE ViPipe, const ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
    MPI_ASSERT(sizeof(ISP_DP_DYNAMIC_ATTR_S) == sizeof(hi_isp_dp_dynamic_attr));
    return hi_mpi_isp_set_dp_dynamic_attr(ViPipe, (hi_isp_dp_dynamic_attr *)pstDPDynamicAttr);
}

HI_S32 HI_MPI_ISP_GetDPDynamicAttr(VI_PIPE ViPipe, ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
    MPI_ASSERT(sizeof(ISP_DP_DYNAMIC_ATTR_S) == sizeof(hi_isp_dp_dynamic_attr));
    return hi_mpi_isp_get_dp_dynamic_attr(ViPipe, (hi_isp_dp_dynamic_attr *)pstDPDynamicAttr);
}

HI_S32 HI_MPI_ISP_SetMeshShadingAttr(VI_PIPE ViPipe, const ISP_SHADING_ATTR_S *pstShadingAttr)
{
    MPI_ASSERT(sizeof(ISP_SHADING_ATTR_S) == sizeof(hi_isp_shading_attr));
    return hi_mpi_isp_set_mesh_shading_attr(ViPipe, (hi_isp_shading_attr *)pstShadingAttr);
}

HI_S32 HI_MPI_ISP_GetMeshShadingAttr(VI_PIPE ViPipe, ISP_SHADING_ATTR_S *pstShadingAttr)
{
    MPI_ASSERT(sizeof(ISP_SHADING_ATTR_S) == sizeof(hi_isp_shading_attr));
    return hi_mpi_isp_get_mesh_shading_attr(ViPipe, (hi_isp_shading_attr *)pstShadingAttr);
}

HI_S32 HI_MPI_ISP_SetMeshShadingGainLutAttr(VI_PIPE ViPipe, const ISP_SHADING_LUT_ATTR_S *pstShadingLutAttr)
{
    MPI_ASSERT(sizeof(ISP_SHADING_LUT_ATTR_S) == sizeof(hi_isp_shading_lut_attr));
    return hi_mpi_isp_set_mesh_shading_gain_lut_attr(ViPipe, (hi_isp_shading_lut_attr *)pstShadingLutAttr);
}

HI_S32 HI_MPI_ISP_GetMeshShadingGainLutAttr(VI_PIPE ViPipe, ISP_SHADING_LUT_ATTR_S *pstShadingLutAttr)
{
    MPI_ASSERT(sizeof(ISP_SHADING_LUT_ATTR_S) == sizeof(hi_isp_shading_lut_attr));
    return hi_mpi_isp_get_mesh_shading_gain_lut_attr(ViPipe, (hi_isp_shading_lut_attr *)pstShadingLutAttr);
}

HI_S32 HI_MPI_ISP_SetAutoColorShadingAttr(VI_PIPE ViPipe, const ISP_ACS_ATTR_S *pstACSAttr)
{
    MPI_ASSERT(sizeof(ISP_ACS_ATTR_S) == sizeof(hi_isp_acs_attr));
    return hi_mpi_isp_set_auto_color_shading_attr(ViPipe, (hi_isp_acs_attr *)pstACSAttr);
}

HI_S32 HI_MPI_ISP_GetAutoColorShadingAttr(VI_PIPE ViPipe, ISP_ACS_ATTR_S *pstACSAttr)
{
    MPI_ASSERT(sizeof(ISP_ACS_ATTR_S) == sizeof(hi_isp_acs_attr));
    return hi_mpi_isp_get_auto_color_shading_attr(ViPipe, (hi_isp_acs_attr *)pstACSAttr);
}

HI_S32 HI_MPI_ISP_SetRadialShadingAttr(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr)
{
    MPI_ASSERT(sizeof(ISP_RADIAL_SHADING_ATTR_S) == sizeof(hi_isp_radial_shading_attr));
    return hi_mpi_isp_set_radial_shading_attr(ViPipe, (hi_isp_radial_shading_attr *)pstRaShadingAttr);
}

HI_S32 HI_MPI_ISP_GetRadialShadingAttr(VI_PIPE ViPipe, ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr)
{
    MPI_ASSERT(sizeof(ISP_RADIAL_SHADING_ATTR_S) == sizeof(hi_isp_radial_shading_attr));
    return hi_mpi_isp_get_radial_shading_attr(ViPipe, (hi_isp_radial_shading_attr *)pstRaShadingAttr);
}

HI_S32 HI_MPI_ISP_SetRadialShadingLUT(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr)
{
    MPI_ASSERT(sizeof(ISP_RADIAL_SHADING_LUT_ATTR_S) == sizeof(hi_isp_radial_shading_lut_attr));
    return hi_mpi_isp_set_radial_shading_lut(ViPipe, (hi_isp_radial_shading_lut_attr *)pstRaShadingLutAttr);
}

HI_S32 HI_MPI_ISP_GetRadialShadingLUT(VI_PIPE ViPipe, ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr)
{
    MPI_ASSERT(sizeof(ISP_RADIAL_SHADING_LUT_ATTR_S) == sizeof(hi_isp_radial_shading_lut_attr));
    return hi_mpi_isp_get_radial_shading_lut(ViPipe, (hi_isp_radial_shading_lut_attr *)pstRaShadingLutAttr);
}

HI_S32 HI_MPI_ISP_GetLightboxGain(VI_PIPE ViPipe, ISP_AWB_Calibration_Gain_S *pstAWBCalibrationGain)
{
    MPI_ASSERT(sizeof(ISP_AWB_Calibration_Gain_S) == sizeof(hi_isp_awb_calibration_gain));
    return hi_mpi_isp_get_lightbox_gain(ViPipe, (hi_isp_awb_calibration_gain *)pstAWBCalibrationGain);
}

HI_S32 HI_MPI_ISP_MeshShadingCalibration(VI_PIPE ViPipe, HI_U16 *pu16SrcRaw,
                                         ISP_MLSC_CALIBRATION_CFG_S *pstMLSCCaliCfg,
                                         ISP_MESH_SHADING_TABLE_S *pstMLSCTable)
{
    MPI_ASSERT(sizeof(ISP_MLSC_CALIBRATION_CFG_S) == sizeof(hi_isp_mlsc_calibration_cfg));
    MPI_ASSERT(sizeof(ISP_MESH_SHADING_TABLE_S) == sizeof(hi_isp_mesh_shading_table));
    return hi_mpi_isp_mesh_shading_calibration(ViPipe, pu16SrcRaw, (hi_isp_mlsc_calibration_cfg *)pstMLSCCaliCfg,
                                               (hi_isp_mesh_shading_table *)pstMLSCTable);
}

HI_S32 HI_MPI_ISP_RadialShadingCalibration(VI_PIPE ViPipe, HI_U16 *pu16SrcRaw,
    ISP_RLSC_CALIBRATION_CFG_S *pstRLSCCaliCfg, ISP_RADIAL_SHADING_TABLE_S *pstRLSCTable)
{
    MPI_ASSERT(sizeof(ISP_RLSC_CALIBRATION_CFG_S) == sizeof(hi_isp_rlsc_calibration_cfg));
    MPI_ASSERT(sizeof(ISP_RADIAL_SHADING_TABLE_S) == sizeof(hi_isp_radial_shading_table));
    return hi_mpi_isp_radial_shading_calibration(ViPipe, pu16SrcRaw, (hi_isp_rlsc_calibration_cfg *)pstRLSCCaliCfg,
        (hi_isp_radial_shading_table *)pstRLSCTable);
}

HI_S32 HI_MPI_ISP_SetLocalCacAttr(VI_PIPE ViPipe, const ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr)
{
    MPI_ASSERT(sizeof(ISP_LOCAL_CAC_ATTR_S) == sizeof(hi_isp_local_cac_attr));
    return hi_mpi_isp_set_local_cac_attr(ViPipe, (hi_isp_local_cac_attr *)pstLocalCacAttr);
}

HI_S32 HI_MPI_ISP_GetLocalCacAttr(VI_PIPE ViPipe, ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr)
{
    MPI_ASSERT(sizeof(ISP_LOCAL_CAC_ATTR_S) == sizeof(hi_isp_local_cac_attr));
    return hi_mpi_isp_get_local_cac_attr(ViPipe, (hi_isp_local_cac_attr *)pstLocalCacAttr);
}

HI_S32 HI_MPI_ISP_SetGlobalCacAttr(VI_PIPE ViPipe, const ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr)
{
    MPI_ASSERT(sizeof(ISP_GLOBAL_CAC_ATTR_S) == sizeof(hi_isp_global_cac_attr));
    return hi_mpi_isp_set_global_cac_attr(ViPipe, (hi_isp_global_cac_attr *)pstGlobalCacAttr);
}

HI_S32 HI_MPI_ISP_GetGlobalCacAttr(VI_PIPE ViPipe, ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr)
{
    MPI_ASSERT(sizeof(ISP_GLOBAL_CAC_ATTR_S) == sizeof(hi_isp_global_cac_attr));
    return hi_mpi_isp_get_global_cac_attr(ViPipe, (hi_isp_global_cac_attr *)pstGlobalCacAttr);
}

HI_S32 HI_MPI_ISP_SetRcAttr(VI_PIPE ViPipe, const ISP_RC_ATTR_S *pstRcAttr)
{
    MPI_ASSERT(sizeof(ISP_RC_ATTR_S) == sizeof(hi_isp_rc_attr));
    return hi_mpi_isp_set_rc_attr(ViPipe, (hi_isp_rc_attr *)pstRcAttr);
}

HI_S32 HI_MPI_ISP_GetRcAttr(VI_PIPE ViPipe, ISP_RC_ATTR_S *pstRcAttr)
{
    MPI_ASSERT(sizeof(ISP_RC_ATTR_S) == sizeof(hi_isp_rc_attr));
    return hi_mpi_isp_get_rc_attr(ViPipe, (hi_isp_rc_attr *)pstRcAttr);
}

HI_S32 HI_MPI_ISP_SetNRAttr(VI_PIPE ViPipe, const ISP_NR_ATTR_S *pstNRAttr)
{
    MPI_ASSERT(sizeof(ISP_NR_ATTR_S) == sizeof(hi_isp_nr_attr));
    return hi_mpi_isp_set_nr_attr(ViPipe, (hi_isp_nr_attr *)pstNRAttr);
}

HI_S32 HI_MPI_ISP_GetNRAttr(VI_PIPE ViPipe, ISP_NR_ATTR_S *pstNRAttr)
{
    MPI_ASSERT(sizeof(ISP_NR_ATTR_S) == sizeof(hi_isp_nr_attr));
    return hi_mpi_isp_get_nr_attr(ViPipe, (hi_isp_nr_attr *)pstNRAttr);
}

HI_S32 HI_MPI_ISP_SetDEAttr(VI_PIPE ViPipe, const ISP_DE_ATTR_S *pstDEAttr)
{
    MPI_ASSERT(sizeof(ISP_DE_ATTR_S) == sizeof(hi_isp_de_attr));
    return hi_mpi_isp_set_de_attr(ViPipe, (hi_isp_de_attr *)pstDEAttr);
}

HI_S32 HI_MPI_ISP_GetDEAttr(VI_PIPE ViPipe, ISP_DE_ATTR_S *pstDEAttr)
{
    MPI_ASSERT(sizeof(ISP_DE_ATTR_S) == sizeof(hi_isp_de_attr));
    return hi_mpi_isp_get_de_attr(ViPipe, (hi_isp_de_attr *)pstDEAttr);
}

HI_S32 HI_MPI_ISP_SetRGBIRAttr(VI_PIPE ViPipe, const ISP_RGBIR_ATTR_S *pstRGBIRAttr)
{
    MPI_ASSERT(sizeof(ISP_RGBIR_ATTR_S) == sizeof(hi_isp_rgbir_attr));
    return hi_mpi_isp_set_rgbir_attr(ViPipe, (hi_isp_rgbir_attr *)pstRGBIRAttr);
}

HI_S32 HI_MPI_ISP_GetRGBIRAttr(VI_PIPE ViPipe, ISP_RGBIR_ATTR_S *pstRGBIRAttr)
{
    MPI_ASSERT(sizeof(ISP_RGBIR_ATTR_S) == sizeof(hi_isp_rgbir_attr));
    return hi_mpi_isp_get_rgbir_attr(ViPipe, (hi_isp_rgbir_attr *)pstRGBIRAttr);
}
HI_S32 HI_MPI_ISP_SetColorToneAttr(VI_PIPE ViPipe, const ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    MPI_ASSERT(sizeof(ISP_COLOR_TONE_ATTR_S) == sizeof(hi_isp_color_tone_attr));
    return hi_mpi_isp_set_color_tone_attr(ViPipe, (hi_isp_color_tone_attr *)pstCTAttr);
}

HI_S32 HI_MPI_ISP_GetColorToneAttr(VI_PIPE ViPipe, ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    MPI_ASSERT(sizeof(ISP_COLOR_TONE_ATTR_S) == sizeof(hi_isp_color_tone_attr));
    return hi_mpi_isp_get_color_tone_attr(ViPipe, (hi_isp_color_tone_attr *)pstCTAttr);
}

HI_S32 HI_MPI_ISP_SetIspSharpenAttr(VI_PIPE ViPipe, const ISP_SHARPEN_ATTR_S *pstIspShpAttr)
{
    MPI_ASSERT(sizeof(ISP_SHARPEN_ATTR_S) == sizeof(hi_isp_sharpen_attr));
    return hi_mpi_isp_set_sharpen_attr(ViPipe, (hi_isp_sharpen_attr *)pstIspShpAttr);
}

HI_S32 HI_MPI_ISP_GetIspSharpenAttr(VI_PIPE ViPipe, ISP_SHARPEN_ATTR_S *pstIspShpAttr)
{
    MPI_ASSERT(sizeof(ISP_SHARPEN_ATTR_S) == sizeof(hi_isp_sharpen_attr));
    return hi_mpi_isp_get_sharpen_attr(ViPipe, (hi_isp_sharpen_attr *)pstIspShpAttr);
}

HI_S32 HI_MPI_ISP_SetIspEdgeMarkAttr(VI_PIPE ViPipe, const ISP_EDGEMARK_ATTR_S *pstIspEdgeMarkAttr)
{
    MPI_ASSERT(sizeof(ISP_EDGEMARK_ATTR_S) == sizeof(hi_isp_edge_mark_attr));
    return hi_mpi_isp_set_edge_mark_attr(ViPipe, (hi_isp_edge_mark_attr *)pstIspEdgeMarkAttr);
}

HI_S32 HI_MPI_ISP_GetIspEdgeMarkAttr(VI_PIPE ViPipe, ISP_EDGEMARK_ATTR_S *pstIspEdgeMarkAttr)
{
    MPI_ASSERT(sizeof(ISP_EDGEMARK_ATTR_S) == sizeof(hi_isp_edge_mark_attr));
    return hi_mpi_isp_get_edge_mark_attr(ViPipe, (hi_isp_edge_mark_attr *)pstIspEdgeMarkAttr);
}

/* High Light Constraint */
HI_S32 HI_MPI_ISP_SetIspHlcAttr(VI_PIPE ViPipe, const ISP_HLC_ATTR_S *pstIspHlcAttr)
{
    MPI_ASSERT(sizeof(ISP_HLC_ATTR_S) == sizeof(hi_isp_hlc_attr));
    return hi_mpi_isp_set_hlc_attr(ViPipe, (hi_isp_hlc_attr *)pstIspHlcAttr);
}

HI_S32 HI_MPI_ISP_GetIspHlcAttr(VI_PIPE ViPipe, ISP_HLC_ATTR_S *pstIspHlcAttr)
{
    MPI_ASSERT(sizeof(ISP_HLC_ATTR_S) == sizeof(hi_isp_hlc_attr));
    return hi_mpi_isp_get_hlc_attr(ViPipe, (hi_isp_hlc_attr *)pstIspHlcAttr);
}

/* Crosstalk Removal Strength */
HI_S32 HI_MPI_ISP_SetCrosstalkAttr(VI_PIPE ViPipe, const ISP_CR_ATTR_S *pstCRAttr)
{
    MPI_ASSERT(sizeof(ISP_CR_ATTR_S) == sizeof(hi_isp_cr_attr));
    return hi_mpi_isp_set_crosstalk_attr(ViPipe, (hi_isp_cr_attr *)pstCRAttr);
}

HI_S32 HI_MPI_ISP_GetCrosstalkAttr(VI_PIPE ViPipe, ISP_CR_ATTR_S *pstCRAttr)
{
    MPI_ASSERT(sizeof(ISP_CR_ATTR_S) == sizeof(hi_isp_cr_attr));
    return hi_mpi_isp_get_crosstalk_attr(ViPipe, (hi_isp_cr_attr *)pstCRAttr);
}

HI_S32 HI_MPI_ISP_SetAntiFalseColorAttr(VI_PIPE ViPipe, const ISP_ANTIFALSECOLOR_ATTR_S *pstAntiFalseColor)
{
    MPI_ASSERT(sizeof(ISP_ANTIFALSECOLOR_ATTR_S) == sizeof(hi_isp_antifalsecolor_attr));
    return hi_mpi_isp_set_anti_false_color_attr(ViPipe, (hi_isp_antifalsecolor_attr *)pstAntiFalseColor);
}

HI_S32 HI_MPI_ISP_GetAntiFalseColorAttr(VI_PIPE ViPipe, ISP_ANTIFALSECOLOR_ATTR_S *pstAntiFalseColor)
{
    MPI_ASSERT(sizeof(ISP_ANTIFALSECOLOR_ATTR_S) == sizeof(hi_isp_antifalsecolor_attr));
    return hi_mpi_isp_get_anti_false_color_attr(ViPipe, (hi_isp_antifalsecolor_attr *)pstAntiFalseColor);
}

HI_S32 HI_MPI_ISP_SetDemosaicAttr(VI_PIPE ViPipe, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    MPI_ASSERT(sizeof(ISP_DEMOSAIC_ATTR_S) == sizeof(hi_isp_demosaic_attr));
    return hi_mpi_isp_set_demosaic_attr(ViPipe, (hi_isp_demosaic_attr *)pstDemosaicAttr);
}

HI_S32 HI_MPI_ISP_GetDemosaicAttr(VI_PIPE ViPipe, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    MPI_ASSERT(sizeof(ISP_DEMOSAIC_ATTR_S) == sizeof(hi_isp_demosaic_attr));
    return hi_mpi_isp_get_demosaic_attr(ViPipe, (hi_isp_demosaic_attr *)pstDemosaicAttr);
}

HI_S32 HI_MPI_ISP_SetCAAttr(VI_PIPE ViPipe, const ISP_CA_ATTR_S *pstCAAttr)
{
    MPI_ASSERT(sizeof(ISP_CA_ATTR_S) == sizeof(hi_isp_ca_attr));
    return hi_mpi_isp_set_ca_attr(ViPipe, (hi_isp_ca_attr *)pstCAAttr);
}

HI_S32 HI_MPI_ISP_GetCAAttr(VI_PIPE ViPipe, ISP_CA_ATTR_S *pstCAAttr)
{
    MPI_ASSERT(sizeof(ISP_CA_ATTR_S) == sizeof(hi_isp_ca_attr));
    return hi_mpi_isp_get_ca_attr(ViPipe, (hi_isp_ca_attr *)pstCAAttr);
}

HI_S32 HI_MPI_ISP_SetClutCoeff(VI_PIPE ViPipe, const ISP_CLUT_LUT_S *pstClutLUT)
{
    MPI_ASSERT(sizeof(ISP_CLUT_LUT_S) == sizeof(hi_isp_clut_lut));
    return hi_mpi_isp_set_clut_coeff(ViPipe, (hi_isp_clut_lut *)pstClutLUT);
}

HI_S32 HI_MPI_ISP_GetClutCoeff(VI_PIPE ViPipe, ISP_CLUT_LUT_S *pstClutLUT)
{
    MPI_ASSERT(sizeof(ISP_CLUT_LUT_S) == sizeof(hi_isp_clut_lut));
    return hi_mpi_isp_get_clut_coeff(ViPipe, (hi_isp_clut_lut *)pstClutLUT);
}

HI_S32 HI_MPI_ISP_SetClutAttr(VI_PIPE ViPipe, const ISP_CLUT_ATTR_S *pstClutAttr)
{
    MPI_ASSERT(sizeof(ISP_CLUT_ATTR_S) == sizeof(hi_isp_clut_attr));
    return hi_mpi_isp_set_clut_attr(ViPipe, (hi_isp_clut_attr *)pstClutAttr);
}

HI_S32 HI_MPI_ISP_GetClutAttr(VI_PIPE ViPipe, ISP_CLUT_ATTR_S *pstClutAttr)
{
    MPI_ASSERT(sizeof(ISP_CLUT_ATTR_S) == sizeof(hi_isp_clut_attr));
    return hi_mpi_isp_get_clut_attr(ViPipe, (hi_isp_clut_attr *)pstClutAttr);
}

HI_S32 HI_MPI_ISP_SetBlackLevelAttr(VI_PIPE ViPipe, const ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    MPI_ASSERT(sizeof(ISP_BLACK_LEVEL_S) == sizeof(hi_isp_black_level));
    return hi_mpi_isp_set_black_level_attr(ViPipe, (hi_isp_black_level *)pstBlackLevel);
}

HI_S32 HI_MPI_ISP_GetBlackLevelAttr(VI_PIPE ViPipe, ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    MPI_ASSERT(sizeof(ISP_BLACK_LEVEL_S) == sizeof(hi_isp_black_level));
    return hi_mpi_isp_get_black_level_attr(ViPipe, (hi_isp_black_level *)pstBlackLevel);
}

HI_S32 HI_MPI_ISP_FPNCalibrate(VI_PIPE ViPipe, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr)
{
#ifdef CONFIG_HI_VI_FPN_SUPPORT
    MPI_ASSERT(sizeof(ISP_FPN_CALIBRATE_ATTR_S) == sizeof(hi_isp_fpn_calibrate_attr));
    return hi_mpi_isp_fpn_calibrate(ViPipe, (hi_isp_fpn_calibrate_attr *)pstCalibrateAttr);
#else
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

HI_S32 HI_MPI_ISP_SetFPNAttr(VI_PIPE ViPipe, const ISP_FPN_ATTR_S *pstFPNAttr)
{
#ifdef CONFIG_HI_VI_FPN_SUPPORT
    MPI_ASSERT(sizeof(ISP_FPN_ATTR_S) == sizeof(hi_isp_fpn_attr));
    return hi_mpi_isp_set_fpn_attr(ViPipe, (hi_isp_fpn_attr *)pstFPNAttr);
#else
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

HI_S32 HI_MPI_ISP_GetFPNAttr(VI_PIPE ViPipe, ISP_FPN_ATTR_S *pstFPNAttr)
{
#ifdef CONFIG_HI_VI_FPN_SUPPORT
    MPI_ASSERT(sizeof(ISP_FPN_ATTR_S) == sizeof(hi_isp_fpn_attr));
    return hi_mpi_isp_get_fpn_attr(ViPipe, (hi_isp_fpn_attr *)pstFPNAttr);
#else
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

HI_S32 HI_MPI_ISP_GetISPRegAttr(VI_PIPE ViPipe, ISP_REG_ATTR_S *pstIspRegAttr)
{
    MPI_ASSERT(sizeof(ISP_REG_ATTR_S) == sizeof(hi_isp_reg_attr));
    return hi_mpi_isp_get_isp_reg_attr(ViPipe, (hi_isp_reg_attr *)pstIspRegAttr);
}

HI_S32 HI_MPI_ISP_SetDebug(VI_PIPE ViPipe, const ISP_DEBUG_INFO_S *pstIspDebug)
{
    MPI_ASSERT(sizeof(ISP_DEBUG_INFO_S) == sizeof(hi_isp_debug_info));
    return hi_mpi_isp_set_debug(ViPipe, (hi_isp_debug_info *)pstIspDebug);
}

HI_S32 HI_MPI_ISP_GetDebug(VI_PIPE ViPipe, ISP_DEBUG_INFO_S *pstIspDebug)
{
    MPI_ASSERT(sizeof(ISP_DEBUG_INFO_S) == sizeof(hi_isp_debug_info));
    return hi_mpi_isp_get_debug(ViPipe, (hi_isp_debug_info *)pstIspDebug);
}

HI_S32 HI_MPI_ISP_SetCtrlParam(VI_PIPE ViPipe, const ISP_CTRL_PARAM_S *pstIspCtrlParam)
{
    MPI_ASSERT(sizeof(ISP_CTRL_PARAM_S) == sizeof(hi_isp_ctrl_param));
    return hi_mpi_isp_set_ctrl_param(ViPipe, (hi_isp_ctrl_param *)pstIspCtrlParam);
}

HI_S32 HI_MPI_ISP_GetCtrlParam(VI_PIPE ViPipe, ISP_CTRL_PARAM_S *pstIspCtrlParam)
{
    MPI_ASSERT(sizeof(ISP_CTRL_PARAM_S) == sizeof(hi_isp_ctrl_param));
    return hi_mpi_isp_get_ctrl_param(ViPipe, (hi_isp_ctrl_param *)pstIspCtrlParam);
}

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(VI_PIPE ViPipe, ISP_INNER_STATE_INFO_S *pstInnerStateInfo)
{
    MPI_ASSERT(sizeof(ISP_INNER_STATE_INFO_S) == sizeof(hi_isp_inner_state_info));
    return hi_mpi_isp_query_inner_state_info(ViPipe, (hi_isp_inner_state_info *)pstInnerStateInfo);
}

HI_S32 HI_MPI_ISP_GetDngImageStaticInfo(VI_PIPE ViPipe, DNG_IMAGE_STATIC_INFO_S *pstDngImageStaticInfo)
{
    MPI_ASSERT(sizeof(DNG_IMAGE_STATIC_INFO_S) == sizeof(hi_isp_dng_image_static_info));
    return hi_mpi_isp_get_dng_image_static_info(ViPipe, (hi_isp_dng_image_static_info *)pstDngImageStaticInfo);
}

HI_S32 HI_MPI_ISP_SetDngColorParam(VI_PIPE ViPipe, const ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    MPI_ASSERT(sizeof(ISP_DNG_COLORPARAM_S) == sizeof(hi_isp_dng_color_param));
    return hi_mpi_isp_set_dng_color_param(ViPipe, (hi_isp_dng_color_param *)pstDngColorParam);
}

HI_S32 HI_MPI_ISP_GetDngColorParam(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    MPI_ASSERT(sizeof(ISP_DNG_COLORPARAM_S) == sizeof(hi_isp_dng_color_param));
    return hi_mpi_isp_get_dng_color_param(ViPipe, (hi_isp_dng_color_param *)pstDngColorParam);
}

HI_S32 HI_MPI_ISP_IrAutoRunOnce(VI_PIPE ViPipe, ISP_IR_AUTO_ATTR_S *pstIrAttr)
{
    MPI_ASSERT(sizeof(ISP_IR_AUTO_ATTR_S) == sizeof(hi_isp_ir_auto_attr));
    return hi_mpi_isp_ir_auto_run_once(ViPipe, (hi_isp_ir_auto_attr *)pstIrAttr);
}

HI_S32 HI_MPI_ISP_SetSmartInfo(VI_PIPE ViPipe, const ISP_SMART_INFO_S *pstSmartInfo)
{
    MPI_ASSERT(sizeof(ISP_SMART_INFO_S) == sizeof(hi_isp_smart_info));
    return hi_mpi_isp_set_smart_info(ViPipe, (hi_isp_smart_info *)pstSmartInfo);
}

HI_S32 HI_MPI_ISP_GetSmartInfo(VI_PIPE ViPipe, ISP_SMART_INFO_S *pstSmartInfo)
{
    MPI_ASSERT(sizeof(ISP_SMART_INFO_S) == sizeof(hi_isp_smart_info));
    return hi_mpi_isp_get_smart_info(ViPipe, (hi_isp_smart_info *)pstSmartInfo);
}

HI_S32 HI_MPI_ISP_SetTunnelInfo(VI_PIPE ViPipe, const ISP_TUNNEL_INFO_S *pstTunnelInfo)
{
    MPI_ASSERT(sizeof(ISP_TUNNEL_INFO_S) == sizeof(hi_isp_tunnel_info));
    return hi_mpi_isp_set_tunnel_info(ViPipe, (hi_isp_tunnel_info *)pstTunnelInfo);
}
HI_S32 HI_MPI_ISP_GetTunnelInfo(VI_PIPE ViPipe, ISP_TUNNEL_INFO_S *pstTunnelInfo)
{
    MPI_ASSERT(sizeof(ISP_TUNNEL_INFO_S) == sizeof(hi_isp_tunnel_info));
    return hi_mpi_isp_get_tunnel_info(ViPipe, (hi_isp_tunnel_info *)pstTunnelInfo);
}
HI_S32 HI_MPI_ISP_CalcFlickerType(VI_PIPE ViPipe, ISP_CALCFLICKER_INPUT_S *pstInputParam,
                                  ISP_CALCFLICKER_OUTPUT_S *pstOutputParam, VIDEO_FRAME_INFO_S stFrame[],
                                  HI_U32 u32ArraySize)
{
    MPI_ASSERT(sizeof(ISP_CALCFLICKER_INPUT_S) == sizeof(hi_isp_calc_flicker_input));
    MPI_ASSERT(sizeof(ISP_CALCFLICKER_OUTPUT_S) == sizeof(hi_isp_calc_flicker_output));
    MPI_ASSERT(sizeof(VIDEO_FRAME_INFO_S) == sizeof(hi_video_frame_info));
    return isp_calc_flicker_type(ViPipe, (hi_isp_calc_flicker_input *)pstInputParam,
                                 (hi_isp_calc_flicker_output *)pstOutputParam,
                                 (hi_video_frame_info *)stFrame, u32ArraySize);
}

