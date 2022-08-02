/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>

#include "ot_mpi_sys.h"
#include "ot_mpi_isp.h"
#include "mkp_isp.h"
#include "comm_isp.h"
#include "comm_3a.h"
#include "ae_comm.h"
#include "awb_comm.h"
#include "comm_vi.h"
#include "isp_inner.h"
#include "isp_ext_config.h"
#include "isp_debug.h"
#include "isp_main.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include "isp_vreg.h"
#include "math_fun.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*****************************************************************************
 Prototype       : isp pub configure
 Description     : need I/F cowork:
                    OB area, need to configure window, and I/F font edge;
                    others configure I/F as back edge;
 Input           : None
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/14
    Author       :
    Modification : Created function

*****************************************************************************/
GK_S32 MPI_ISP_SetPubAttr(VI_PIPE ViPipe, const ISP_PUB_ATTR_S *pstPubAttr)
{
    GK_S32 s32Ret;
    GK_VOID   *pValue    = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if ((pstPubAttr->stWndRect.u32Width < RES_WIDTH_MIN) ||
        (pstPubAttr->stWndRect.u32Width > RES_WIDTH_MAX(ViPipe)) ||
        (pstPubAttr->stWndRect.u32Width % 4) != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image Width:%d!\n", pstPubAttr->stWndRect.u32Width);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stWndRect.u32Height < RES_HEIGHT_MIN) ||
        (pstPubAttr->stWndRect.u32Height > RES_HEIGHT_MAX(ViPipe)) ||
        (pstPubAttr->stWndRect.u32Height % 4) != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image Height:%d!\n", pstPubAttr->stWndRect.u32Height);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stSnsSize.u32Width < RES_WIDTH_MIN) ||
        (pstPubAttr->stSnsSize.u32Width > SENSOR_RES_WIDTH_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Sensor Image Width:%d!\n", pstPubAttr->stSnsSize.u32Width);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stSnsSize.u32Height < RES_HEIGHT_MIN) ||
        (pstPubAttr->stSnsSize.u32Height > SENSOR_RES_HEIGHT_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Sensor Image Height:%d!\n", pstPubAttr->stSnsSize.u32Height);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stWndRect.s32X < 0) || (pstPubAttr->stWndRect.s32X > RES_WIDTH_MAX(ViPipe) - RES_WIDTH_MIN)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image X:%d!\n", pstPubAttr->stWndRect.s32X);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stWndRect.s32Y < 0) || (pstPubAttr->stWndRect.s32Y > RES_HEIGHT_MAX(ViPipe) - RES_HEIGHT_MIN)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image Y:%d!\n", pstPubAttr->stWndRect.s32Y);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->f32FrameRate <= 0.0) || (pstPubAttr->f32FrameRate > FRAME_RATE_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid FrameRate:%f!\n", pstPubAttr->f32FrameRate);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstPubAttr->enBayer >= BAYER_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Bayer Pattern:%d!\n", pstPubAttr->enBayer);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstPubAttr->enWDRMode >= WDR_MODE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid WDR mode %d!\n", pstPubAttr->enWDRMode);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PUB_ATTR_INFO, pstPubAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Set ISP PUB attr failed\n", ViPipe);
        return s32Ret;
    }

    /* Set WDR Mode */
    ext_top_wdr_switch_write(ViPipe, GK_FALSE);

    pstIspCtx->stIspParaRec.bWDRCfg = GK_TRUE;
    ext_top_wdr_cfg_write(ViPipe, pstIspCtx->stIspParaRec.bWDRCfg);

    if ((GK_U8)pstPubAttr->enWDRMode == ext_system_sensor_wdr_mode_read(ViPipe)) {
        ext_top_wdr_switch_write(ViPipe, GK_TRUE);
    } else {
        ext_system_sensor_wdr_mode_write(ViPipe, (GK_U8)pstPubAttr->enWDRMode);
    }

    /* Set Othes cfgs */
    ext_top_res_switch_write(ViPipe, GK_FALSE);

    ext_system_corp_pos_x_write(ViPipe, pstPubAttr->stWndRect.s32X);
    ext_system_corp_pos_y_write(ViPipe, pstPubAttr->stWndRect.s32Y);

    ext_sync_total_width_write(ViPipe, pstPubAttr->stWndRect.u32Width);
    ext_sync_total_height_write(ViPipe, pstPubAttr->stWndRect.u32Height);

    ext_top_sensor_width_write(ViPipe, pstPubAttr->stSnsSize.u32Width);
    ext_top_sensor_height_write(ViPipe, pstPubAttr->stSnsSize.u32Height);

    ext_system_rggb_cfg_write(ViPipe, (GK_U8)pstPubAttr->enBayer);

    pValue = (GK_VOID *)(&pstPubAttr->f32FrameRate);
    ext_system_fps_base_write(ViPipe, *(GK_U32 *)pValue);
    ext_system_sensor_mode_write(ViPipe, pstPubAttr->u8SnsMode);

    pstIspCtx->stIspParaRec.bPubCfg = GK_TRUE;
    ext_top_pub_attr_cfg_write(ViPipe, pstIspCtx->stIspParaRec.bPubCfg);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetPubAttr(VI_PIPE ViPipe, ISP_PUB_ATTR_S *pstPubAttr)
{
    GK_U8 u8Bayer;
    GK_U8 u8WDRmode;
    GK_U32 u32Value = 0;
    GK_VOID *pValue = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u8Bayer = ext_system_rggb_cfg_read(ViPipe);
    pstPubAttr->enBayer = (u8Bayer >= BAYER_BUTT) ? BAYER_BUTT : u8Bayer;

    u8WDRmode = ext_system_sensor_wdr_mode_read(ViPipe);
    pstPubAttr->enWDRMode = (u8WDRmode >= WDR_MODE_BUTT) ? WDR_MODE_BUTT : u8WDRmode;

    u32Value = ext_system_fps_base_read(ViPipe);
    pValue   = (GK_VOID *)&u32Value;
    pstPubAttr->f32FrameRate = *(GK_FLOAT *)pValue;

    pstPubAttr->u8SnsMode = ext_system_sensor_mode_read(ViPipe);

    pstPubAttr->stWndRect.s32X      = ext_system_corp_pos_x_read(ViPipe);
    pstPubAttr->stWndRect.s32Y      = ext_system_corp_pos_y_read(ViPipe);
    pstPubAttr->stWndRect.u32Width  = ext_sync_total_width_read(ViPipe);
    pstPubAttr->stWndRect.u32Height = ext_sync_total_height_read(ViPipe);
    pstPubAttr->stSnsSize.u32Width  = ext_top_sensor_width_read(ViPipe);
    pstPubAttr->stSnsSize.u32Height = ext_top_sensor_height_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetPipeDifferAttr(VI_PIPE ViPipe, const ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer)
{
    return ISP_SetPipeDifferAttr(ViPipe, pstPipeDiffer);
}

GK_S32 MPI_ISP_GetPipeDifferAttr(VI_PIPE ViPipe, ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer)
{
    return ISP_GetPipeDifferAttr(ViPipe, pstPipeDiffer);
}

GK_S32 MPI_ISP_SetModuleControl(VI_PIPE ViPipe, const ISP_MODULE_CTRL_U *punModCtrl)
{
    GK_S32 s32Ret = GK_SUCCESS;
    GK_U32 u32ChnSelCur = 0;
    GK_U32 u32ChnSelPre = 0;
    GK_U64 u64Key = 0;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u64Key = punModCtrl->u64Key;

    ext_system_isp_bit_bypass_write(ViPipe, u64Key);

    ext_system_isp_dgain_enable_write(ViPipe, !((u64Key >> 0) & 0x1));
    ext_system_antifalsecolor_enable_write(ViPipe, !((u64Key >> 1) & 0x1));
    ext_system_ge_enable_write(ViPipe, !((u64Key >> 2) & 0x1));
    ext_system_dpc_dynamic_cor_enable_write(ViPipe, !((u64Key >> 3) & 0x1));
    ext_system_bayernr_enable_write(ViPipe, !((u64Key >> 4) & 0x1));
    ext_system_dehaze_enable_write(ViPipe, !((u64Key >> 5) & 0x1));
    ext_system_awb_gain_enable_write(ViPipe, !((u64Key >> 6) & 0x1));
    ext_system_isp_mesh_shading_enable_write(ViPipe, !((u64Key >> 7) & 0x1));
    ext_system_drc_enable_write(ViPipe, !((u64Key >> 8) & 0x1));
    ext_system_demosaic_enable_write(ViPipe, !((u64Key >> 9) & 0x1));
    ext_system_cc_enable_write(ViPipe, !((u64Key >> 10) & 0x1));
    ext_system_gamma_en_write(ViPipe, !((u64Key >> 11) & 0x1));
    ext_system_wdr_en_write(ViPipe, !((u64Key >> 12) & 0x1));
    ext_system_ca_en_write(ViPipe, !((u64Key >> 13) & 0x1));
    ext_system_csc_enable_write(ViPipe, !((u64Key >> 14) & 0x1));
    ext_system_rc_en_write(ViPipe, !((u64Key >> 15) & 0x1));
    ext_system_manual_isp_sharpen_en_write(ViPipe, !((u64Key >> 16) & 0x1));
    ext_system_localCAC_enable_write(ViPipe, !((u64Key >> 17) & 0x1));
    ext_system_GlobalCAC_enable_write(ViPipe, !((u64Key >> 18) & 0x1));
    ext_system_top_channel_select_write(ViPipe, (u64Key >> 19) & 0x3);
    ext_system_ldci_enable_write(ViPipe, !((u64Key >> 21) & 0x1));
    ext_system_pregamma_en_write(ViPipe, !((u64Key >> 22) & 0x1));
    ext_system_isp_radial_shading_enable_write(ViPipe, !((u64Key >> 23) & 0x1));
    ext_system_ae_fe_en_write(ViPipe, !((u64Key >> 24) & 0x1));
    ext_system_ae_be_en_write(ViPipe, !((u64Key >> 25) & 0x1));
    ext_system_mg_en_write(ViPipe, !((u64Key >> 26) & 0x1));
    ext_system_detail_enable_write(ViPipe, !((u64Key >> 27) & 0x1));
    ext_system_af_enable_write(ViPipe, ~((u64Key >> 28) & 0x3));
    ext_system_awb_sta_enable_write(ViPipe, !((u64Key >> 30) & 0x1));
    ext_system_clut_en_write(ViPipe, !((u64Key >> 31) & 0x1));
    ext_system_manual_isp_hlc_en_write(ViPipe, !((u64Key >> 32) & 0x1));
    ext_system_manual_isp_edgemark_en_write(ViPipe, !((u64Key >> 33) & 0x1));
    ext_system_rgbir_enable_write(ViPipe, !((u64Key >> 34) & 0x1));
    u32ChnSelCur = punModCtrl->bit2ChnSelect;
    u32ChnSelPre = ext_system_top_channel_select_pre_read(ViPipe);

    if (u32ChnSelPre != u32ChnSelCur) {
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_CHN_SELECT_CFG, &u32ChnSelCur);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "Set isp[%d] chn select failed with ec %#x!\n", ViPipe, s32Ret);
            return s32Ret;
        }

        ext_system_top_channel_select_pre_write(ViPipe, u32ChnSelCur);
    }

    ext_af_set_flag_write(ViPipe, ISP_EXT_AF_SET_FLAG_ENABLE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetModuleControl(VI_PIPE ViPipe, ISP_MODULE_CTRL_U *punModCtrl)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    punModCtrl->bitBypassISPDGain      = !(ext_system_isp_dgain_enable_read(ViPipe));
    punModCtrl->bitBypassAntiFC        = !(ext_system_antifalsecolor_enable_read(ViPipe));
    punModCtrl->bitBypassCrosstalkR    = !(ext_system_ge_enable_read(ViPipe));
    punModCtrl->bitBypassDPC           = !(ext_system_dpc_dynamic_cor_enable_read(ViPipe));
    punModCtrl->bitBypassNR            = !(ext_system_bayernr_enable_read(ViPipe));
    punModCtrl->bitBypassDehaze        = !(ext_system_dehaze_enable_read(ViPipe));
    punModCtrl->bitBypassWBGain        = !(ext_system_awb_gain_enable_read(ViPipe));
    punModCtrl->bitBypassMeshShading   = !(ext_system_isp_mesh_shading_enable_read(ViPipe));
    punModCtrl->bitBypassDRC           = !(ext_system_drc_enable_read(ViPipe));
    punModCtrl->bitBypassDemosaic      = !(ext_system_demosaic_enable_read(ViPipe));
    punModCtrl->bitBypassColorMatrix   = !(ext_system_cc_enable_read(ViPipe));
    punModCtrl->bitBypassGamma         = !(ext_system_gamma_en_read(ViPipe));
    punModCtrl->bitBypassFSWDR         = !(ext_system_wdr_en_read(ViPipe));
    punModCtrl->bitBypassCA            = !(ext_system_ca_en_read(ViPipe));
    punModCtrl->bitBypassCsConv        = !(ext_system_csc_enable_read(ViPipe));
    punModCtrl->bitBypassRadialCrop    = !(ext_system_rc_en_read(ViPipe));
    punModCtrl->bitBypassSharpen       = !(ext_system_manual_isp_sharpen_en_read(ViPipe));
    punModCtrl->bitBypassLCAC          = !(ext_system_localCAC_enable_read(ViPipe));
    punModCtrl->bitBypassGCAC          = !(ext_system_GlobalCAC_enable_read(ViPipe));
    punModCtrl->bit2ChnSelect          = ext_system_top_channel_select_read(ViPipe);
    punModCtrl->bitBypassLdci          = !(ext_system_ldci_enable_read(ViPipe));
    punModCtrl->bitBypassPreGamma      = !(ext_system_pregamma_en_read(ViPipe));
    punModCtrl->bitBypassRadialShading = !(ext_system_isp_radial_shading_enable_read(ViPipe));
    punModCtrl->bitBypassAEStatFE      = !(ext_system_ae_fe_en_read(ViPipe));
    punModCtrl->bitBypassAEStatBE      = !(ext_system_ae_be_en_read(ViPipe));
    punModCtrl->bitBypassMGStat        = !(ext_system_mg_en_read(ViPipe));
    punModCtrl->bitBypassDE            = !(ext_system_detail_enable_read(ViPipe));
    punModCtrl->bitBypassAFStatFE      = !(ext_system_af_enable_read(ViPipe) & 0x1);
    punModCtrl->bitBypassAFStatBE      = !((ext_system_af_enable_read(ViPipe) >> 1) & 0x1);
    punModCtrl->bitBypassAWBStat       = !(ext_system_awb_sta_enable_read(ViPipe));
    punModCtrl->bitBypassCLUT          = !(ext_system_clut_en_read(ViPipe));
    punModCtrl->bitBypassHLC           = !(ext_system_manual_isp_hlc_en_read(ViPipe));
    punModCtrl->bitBypassEdgeMark      = !(ext_system_manual_isp_edgemark_en_read(ViPipe));
    punModCtrl->bitBypassRGBIR         = !(ext_system_rgbir_enable_read(ViPipe));

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetFMWState(VI_PIPE ViPipe, const ISP_FMW_STATE_E enState)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (enState >= ISP_FMW_STATE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid firmware state %d!\n", enState);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_freeze_firmware_write(ViPipe, (GK_U8)enState);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetFMWState(VI_PIPE ViPipe, ISP_FMW_STATE_E *penState)
{
    GK_U8 u8FMWState;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(penState);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u8FMWState = ext_system_freeze_firmware_read(ViPipe);
    *penState  = (u8FMWState >= ISP_FMW_STATE_BUTT) ? ISP_FMW_STATE_BUTT : u8FMWState;

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetLDCIAttr(VI_PIPE ViPipe, const ISP_LDCI_ATTR_S *pstLDCIAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstLDCIAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_BOOL(pstLDCIAttr->bEnable);

    if (pstLDCIAttr->u8GaussLPFSigma < 0x1) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8GaussLPFSigma!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_ldci_enable_write(ViPipe, pstLDCIAttr->bEnable);
    ext_system_ldci_gaussLPFSigma_write(ViPipe, pstLDCIAttr->u8GaussLPFSigma);
    ext_system_ldci_manu_mode_write(ViPipe, pstLDCIAttr->enOpType);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Wgt > ISP_LDCI_HEPOSWGT_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid astHeWgt.stHePosWgt.u8Wgt[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Sigma < 0x1) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid astHeWgt.stHePosWgt.u8Sigma[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Wgt > ISP_LDCI_HENEGWGT_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid astHeWgt.stHeNegWgt.u8Wgt[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Sigma < 0x1) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid astHeWgt.stHeNegWgt.u8Sigma[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstLDCIAttr->stAuto.au16BlcCtrl[i] > 0x1ff) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16BlcCtrl[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ldci_hePosWgt_write(ViPipe, i, pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Wgt);
        ext_system_ldci_hePosSigma_write(ViPipe, i, pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Sigma);
        ext_system_ldci_hePosMean_write(ViPipe, i, pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Mean);
        ext_system_ldci_heNegWgt_write(ViPipe, i, pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Wgt);
        ext_system_ldci_heNegSigma_write(ViPipe, i, pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Sigma);
        ext_system_ldci_heNegMean_write(ViPipe, i, pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Mean);
        ext_system_ldci_blcCtrl_write(ViPipe, i, pstLDCIAttr->stAuto.au16BlcCtrl[i]);
    }

    if (pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Wgt > ISP_LDCI_HEPOSWGT_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stHeWgt.stHePosWgt.u8Wgt!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Sigma < 0x1) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stHeWgt.stHePosWgt.u8Sigma!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Wgt > ISP_LDCI_HENEGWGT_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stHeWgt.stHeNegWgt.u8Wgt!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Sigma < 0x1) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stHeWgt.stHeNegWgt.u8Sigma!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->stManual.u16BlcCtrl > 0x1ff) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16BlcCtrl!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->u16TprIncrCoef > 0x100) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16TprIncrCoef!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLDCIAttr->u16TprDecrCoef > 0x100) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16TprDecrCoef!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_ldci_TprIncrCoef_write(ViPipe, pstLDCIAttr->u16TprIncrCoef);
    ext_system_ldci_TprDecrCoef_write(ViPipe, pstLDCIAttr->u16TprDecrCoef);
    ext_system_ldci_manu_hePosWgt_write(ViPipe, pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Wgt);
    ext_system_ldci_manu_hePosSigma_write(ViPipe, pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Sigma);
    ext_system_ldci_manu_hePosMean_write(ViPipe, pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Mean);
    ext_system_ldci_manu_heNegWgt_write(ViPipe, pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Wgt);
    ext_system_ldci_manu_heNegSigma_write(ViPipe, pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Sigma);
    ext_system_ldci_manu_heNegMean_write(ViPipe, pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Mean);
    ext_system_ldci_manu_blcCtrl_write(ViPipe, pstLDCIAttr->stManual.u16BlcCtrl);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetLDCIAttr(VI_PIPE ViPipe, ISP_LDCI_ATTR_S *pstLDCIAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstLDCIAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstLDCIAttr->bEnable         = ext_system_ldci_enable_read(ViPipe);
    pstLDCIAttr->u8GaussLPFSigma = ext_system_ldci_gaussLPFSigma_read(ViPipe);
    pstLDCIAttr->enOpType        = (ISP_OP_TYPE_E)ext_system_ldci_manu_mode_read(ViPipe);
    pstLDCIAttr->u16TprIncrCoef  = ext_system_ldci_TprIncrCoef_read(ViPipe);
    pstLDCIAttr->u16TprDecrCoef  = ext_system_ldci_TprDecrCoef_read(ViPipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Wgt   = ext_system_ldci_hePosWgt_read(ViPipe, i);
        pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Sigma = ext_system_ldci_hePosSigma_read(ViPipe, i);
        pstLDCIAttr->stAuto.astHeWgt[i].stHePosWgt.u8Mean  = ext_system_ldci_hePosMean_read(ViPipe, i);
        pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Wgt   = ext_system_ldci_heNegWgt_read(ViPipe, i);
        pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Sigma = ext_system_ldci_heNegSigma_read(ViPipe, i);
        pstLDCIAttr->stAuto.astHeWgt[i].stHeNegWgt.u8Mean  = ext_system_ldci_heNegMean_read(ViPipe, i);
        pstLDCIAttr->stAuto.au16BlcCtrl[i] = ext_system_ldci_blcCtrl_read(ViPipe, i);
    }

    pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Wgt   = ext_system_ldci_manu_hePosWgt_read(ViPipe);
    pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Sigma = ext_system_ldci_manu_hePosSigma_read(ViPipe);
    pstLDCIAttr->stManual.stHeWgt.stHePosWgt.u8Mean  = ext_system_ldci_manu_hePosMean_read(ViPipe);
    pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Wgt   = ext_system_ldci_manu_heNegWgt_read(ViPipe);
    pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Sigma = ext_system_ldci_manu_heNegSigma_read(ViPipe);
    pstLDCIAttr->stManual.stHeWgt.stHeNegWgt.u8Mean  = ext_system_ldci_manu_heNegMean_read(ViPipe);
    pstLDCIAttr->stManual.u16BlcCtrl = ext_system_ldci_manu_blcCtrl_read(ViPipe);

    return GK_SUCCESS;
}

/* General Function Settings */
GK_S32 MPI_ISP_SetDRCAttr(VI_PIPE ViPipe, const ISP_DRC_ATTR_S *pstDRC)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_BOOL(pstDRC->bEnable);
    if (pstDRC->u8DpDetectRangeRatio > 0x1F) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DpDetectRangeRatio!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8DpDetectThrSlo > 0x1F) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DpDetectThrSlo!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstDRC->stAsymmetryCurve.u8Asymmetry > 0x1E) || (pstDRC->stAsymmetryCurve.u8Asymmetry < 0x1)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8Asymmetry!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstDRC->stAsymmetryCurve.u8SecondPole > 0xD2) || (pstDRC->stAsymmetryCurve.u8SecondPole < 0x96)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8SecondPole!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstDRC->stAsymmetryCurve.u8Stretch > 0x3C) || (pstDRC->stAsymmetryCurve.u8Stretch < 0x1E)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8Stretch!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->stAsymmetryCurve.u8Compress > 0xc8) || (pstDRC->stAsymmetryCurve.u8Compress < 0x64)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8Compress!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstDRC->enCurveSelect >= DRC_CURVE_BUTT)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enCurveSelect!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->stAuto.u16Strength > ISP_DRC_STRENGTH_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.u16Strength!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->stAuto.u16StrengthMax > ISP_DRC_STRENGTH_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.u16StrengthMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->stAuto.u16StrengthMin > ISP_DRC_STRENGTH_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.u16StrengthMin!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->stAuto.u16StrengthMax < pstDRC->stAuto.u16StrengthMin) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.u16StrengthMin and stAuto.u16StrengthMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->stManual.u16Strength > ISP_DRC_STRENGTH_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u16Strength!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8LocalMixingDarkMax > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8LocalMixingDarkMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8LocalMixingDarkMin > 0x40) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8LocalMixingDarkMin!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->s8LocalMixingDarkSlo > 7 || pstDRC->s8LocalMixingDarkSlo < -7) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s8LocalMixingDarkSlo!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8LocalMixingBrightMax > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8LocalMixingBrightMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8LocalMixingBrightMin > 0x40) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8LocalMixingBrightMin!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->s8LocalMixingBrightSlo > 7 || pstDRC->s8LocalMixingBrightSlo < -7) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s8LocalMixingBrightSlo!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8BrightGainLmt > 0xF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8BrightGainLmt!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8BrightGainLmtStep > 0xF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8BrightGainLmtStep!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8DarkGainLmtY > 0x85) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DarkGainLmtY!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8DarkGainLmtC > 0x85) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DarkGainLmtC!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8PDStrength > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8PDStrength!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8DetailDarkStr > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DetailDarkStr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8DetailBrightStr > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DetailBrightStr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8DetailDarkStep > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DetailDarkStep!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8DetailBrightStep > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DetailBrightStep!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8FltScaleCoarse > 0xf) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8FltScaleCoarse!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8FltScaleFine > 0xf) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8FltScaleFine!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8ContrastControl > 0xF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8ContrastControl!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8SpatialFltCoef > ISP_DRC_SPA_FLT_COEF_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8SpatialFilterCoef!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8RangeFltCoef > 0xA) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8RangeFltCoef!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8RangeAdaMax > 0x8) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8RangeAdaMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8GradRevMax > 0x40) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8GradRevMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8GradRevThr > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8GradRevThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->s8DetailAdjustFactor > 15 || pstDRC->s8DetailAdjustFactor < -15) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s8DetailAdjustFactor!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_drc_manual_mode_write(ViPipe, pstDRC->enOpType);
    ext_system_drc_auto_strength_write(ViPipe, pstDRC->stAuto.u16Strength);
    ext_system_drc_auto_strength_max_write(ViPipe, pstDRC->stAuto.u16StrengthMax);
    ext_system_drc_auto_strength_min_write(ViPipe, pstDRC->stAuto.u16StrengthMin);
    ext_system_drc_manual_strength_write(ViPipe, pstDRC->stManual.u16Strength);

    ext_system_drc_enable_write(ViPipe, pstDRC->bEnable);
    ext_system_drc_asymmetry_write(ViPipe, pstDRC->stAsymmetryCurve.u8Asymmetry);
    ext_system_drc_secondpole_write(ViPipe, pstDRC->stAsymmetryCurve.u8SecondPole);
    ext_system_drc_stretch_write(ViPipe, pstDRC->stAsymmetryCurve.u8Stretch);
    ext_system_drc_compress_write(ViPipe, pstDRC->stAsymmetryCurve.u8Compress);

    ext_system_drc_mixing_dark_max_write(ViPipe, pstDRC->u8LocalMixingDarkMax);
    ext_system_drc_mixing_dark_min_write(ViPipe, pstDRC->u8LocalMixingDarkMin);
    ext_system_drc_mixing_dark_thr_write(ViPipe, pstDRC->u8LocalMixingDarkThr);
    ext_system_drc_mixing_dark_slo_write(ViPipe, pstDRC->s8LocalMixingDarkSlo);
    ext_system_drc_mixing_bright_max_write(ViPipe, pstDRC->u8LocalMixingBrightMax);
    ext_system_drc_mixing_bright_min_write(ViPipe, pstDRC->u8LocalMixingBrightMin);
    ext_system_drc_mixing_bright_thr_write(ViPipe, pstDRC->u8LocalMixingBrightThr);
    ext_system_drc_mixing_bright_slo_write(ViPipe, pstDRC->s8LocalMixingBrightSlo);

    ext_system_drc_gain_clip_knee_write(ViPipe, pstDRC->u8BrightGainLmt);
    ext_system_drc_gain_clip_step_write(ViPipe, pstDRC->u8BrightGainLmtStep);
    ext_system_drc_dark_gain_lmt_y_write(ViPipe, pstDRC->u8DarkGainLmtY);
    ext_system_drc_dark_gain_lmt_c_write(ViPipe, pstDRC->u8DarkGainLmtC);
    ext_system_drc_purplegh_thr_write(ViPipe, pstDRC->u8PDStrength);

    // ShpExp and ShpLog are both bound to ContrastControl
    ext_system_drc_shp_log_write(ViPipe, pstDRC->u8ContrastControl);
    ext_system_drc_shp_exp_write(ViPipe, pstDRC->u8ContrastControl);

    ext_system_drc_flt_scale_coarse_write(ViPipe, pstDRC->u8FltScaleCoarse);
    ext_system_drc_flt_scale_fine_write(ViPipe, pstDRC->u8FltScaleFine);

    ext_system_drc_flt_rng_ada_max_write(ViPipe, pstDRC->u8RangeAdaMax);

    ext_system_drc_flt_spa_fine_write(ViPipe, pstDRC->u8SpatialFltCoef);
    ext_system_drc_flt_rng_fine_write(ViPipe, pstDRC->u8RangeFltCoef);
    ext_system_drc_grad_max_write(ViPipe, pstDRC->u8GradRevMax);
    ext_system_drc_grad_thr_write(ViPipe, pstDRC->u8GradRevThr);
    ext_system_drc_detail_sub_factor_write(ViPipe, pstDRC->s8DetailAdjustFactor);

    ext_system_drc_detail_dark_max_write(ViPipe, pstDRC->u8DetailDarkStr);
    ext_system_drc_detail_bright_min_write(ViPipe, pstDRC->u8DetailBrightStr);
    ext_system_drc_detail_darkstep_write(ViPipe, pstDRC->u8DetailDarkStep);
    ext_system_drc_detail_brightstep_write(ViPipe, pstDRC->u8DetailBrightStep);

    for (i = 0; i < ISP_DRC_TM_NODE_NUM; i++) {
        ext_system_drc_tm_lut_write(ViPipe, i, pstDRC->au16ToneMappingValue[i]);
    }

    for (i = 0; i < ISP_DRC_CC_NODE_NUM; i++) {
        if ((pstDRC->au16ColorCorrectionLut[i] > 0x400)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16ColorCorrectionLut!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_drc_colorcc_lut_write(ViPipe, i, pstDRC->au16ColorCorrectionLut[i]);
    }

    ext_system_drc_curve_sel_write(ViPipe, pstDRC->enCurveSelect);

    for (i = 0; i < ISP_DRC_CUBIC_POINT_NUM; i++) {
        if ((pstDRC->astCubicPoint[i].u16X > 0x3E8)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stCubicPoint[%d].u16X!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if ((pstDRC->astCubicPoint[i].u16Y > 0x3E8)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stCubicPoint[%d].u16Y!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if ((pstDRC->astCubicPoint[i].u16Slope > 0x2710)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stCubicPoint[%d].u16Slope!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    ext_system_drc_cubic_ltmx0_write(ViPipe, pstDRC->astCubicPoint[0].u16X);
    ext_system_drc_cubic_ltmy0_write(ViPipe, pstDRC->astCubicPoint[0].u16Y);
    ext_system_drc_cubic_slp0_write(ViPipe, pstDRC->astCubicPoint[0].u16Slope);

    ext_system_drc_cubic_ltmx1_write(ViPipe, pstDRC->astCubicPoint[1].u16X);
    ext_system_drc_cubic_ltmy1_write(ViPipe, pstDRC->astCubicPoint[1].u16Y);
    ext_system_drc_cubic_slp1_write(ViPipe, pstDRC->astCubicPoint[1].u16Slope);

    ext_system_drc_cubic_ltmx2_write(ViPipe, pstDRC->astCubicPoint[2].u16X);
    ext_system_drc_cubic_ltmy2_write(ViPipe, pstDRC->astCubicPoint[2].u16Y);
    ext_system_drc_cubic_slp2_write(ViPipe, pstDRC->astCubicPoint[2].u16Slope);

    ext_system_drc_cubic_ltmx3_write(ViPipe, pstDRC->astCubicPoint[3].u16X);
    ext_system_drc_cubic_ltmy3_write(ViPipe, pstDRC->astCubicPoint[3].u16Y);
    ext_system_drc_cubic_slp3_write(ViPipe, pstDRC->astCubicPoint[3].u16Slope);

    ext_system_drc_cubic_ltmx4_write(ViPipe, pstDRC->astCubicPoint[4].u16X);
    ext_system_drc_cubic_ltmy4_write(ViPipe, pstDRC->astCubicPoint[4].u16Y);
    ext_system_drc_cubic_slp4_write(ViPipe, pstDRC->astCubicPoint[4].u16Slope);

    ext_system_drc_coef_update_en_write(ViPipe, GK_TRUE);
    ext_system_drc_manual_mode_write(ViPipe, pstDRC->enOpType);

    ext_system_drc_dp_detect_rng_ratio_write(ViPipe, pstDRC->u8DpDetectRangeRatio);
    ext_system_drc_dp_detect_thr_slo_write(ViPipe, pstDRC->u8DpDetectThrSlo);
    ext_system_drc_dp_detect_thr_min_write(ViPipe, pstDRC->u16DpDetectThrMin);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDRCAttr(VI_PIPE ViPipe, ISP_DRC_ATTR_S *pstDRC)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDRC->enOpType              = (ISP_OP_TYPE_E)ext_system_drc_manual_mode_read(ViPipe);
    pstDRC->stAuto.u16Strength    = ext_system_drc_auto_strength_read(ViPipe);
    pstDRC->stAuto.u16StrengthMax = ext_system_drc_auto_strength_max_read(ViPipe);
    pstDRC->stAuto.u16StrengthMin = ext_system_drc_auto_strength_min_read(ViPipe);
    pstDRC->stManual.u16Strength  = ext_system_drc_manual_strength_read(ViPipe);
    pstDRC->bEnable               = ext_system_drc_enable_read(ViPipe);

    pstDRC->stAsymmetryCurve.u8Asymmetry  = ext_system_drc_asymmetry_read(ViPipe);
    pstDRC->stAsymmetryCurve.u8SecondPole = ext_system_drc_secondpole_read(ViPipe);
    pstDRC->stAsymmetryCurve.u8Stretch    = ext_system_drc_stretch_read(ViPipe);
    pstDRC->stAsymmetryCurve.u8Compress   = ext_system_drc_compress_read(ViPipe);

    pstDRC->u8LocalMixingDarkMax   = ext_system_drc_mixing_dark_max_read(ViPipe);
    pstDRC->u8LocalMixingDarkMin   = ext_system_drc_mixing_dark_min_read(ViPipe);
    pstDRC->u8LocalMixingDarkThr   = ext_system_drc_mixing_dark_thr_read(ViPipe);
    pstDRC->s8LocalMixingDarkSlo   = ext_system_drc_mixing_dark_slo_read(ViPipe);
    pstDRC->u8LocalMixingBrightMax = ext_system_drc_mixing_bright_max_read(ViPipe);
    pstDRC->u8LocalMixingBrightMin = ext_system_drc_mixing_bright_min_read(ViPipe);
    pstDRC->u8LocalMixingBrightThr = ext_system_drc_mixing_bright_thr_read(ViPipe);
    pstDRC->s8LocalMixingBrightSlo = ext_system_drc_mixing_bright_slo_read(ViPipe);

    pstDRC->u8BrightGainLmt     = ext_system_drc_gain_clip_knee_read(ViPipe);
    pstDRC->u8BrightGainLmtStep = ext_system_drc_gain_clip_step_read(ViPipe);
    pstDRC->u8DarkGainLmtY      = ext_system_drc_dark_gain_lmt_y_read(ViPipe);
    pstDRC->u8DarkGainLmtC      = ext_system_drc_dark_gain_lmt_c_read(ViPipe);
    pstDRC->u8PDStrength        = ext_system_drc_purplegh_thr_read(ViPipe);

    pstDRC->u8FltScaleCoarse = ext_system_drc_flt_scale_coarse_read(ViPipe);
    pstDRC->u8FltScaleFine   = ext_system_drc_flt_scale_fine_read(ViPipe);

    pstDRC->u8ContrastControl    = ext_system_drc_shp_log_read(ViPipe);
    pstDRC->s8DetailAdjustFactor = ext_system_drc_detail_sub_factor_read(ViPipe);

    pstDRC->u8SpatialFltCoef = ext_system_drc_flt_spa_fine_read(ViPipe);
    pstDRC->u8RangeFltCoef   = ext_system_drc_flt_rng_fine_read(ViPipe);
    pstDRC->u8RangeAdaMax    = ext_system_drc_flt_rng_ada_max_read(ViPipe);

    pstDRC->u8GradRevMax = ext_system_drc_grad_max_read(ViPipe);
    pstDRC->u8GradRevThr = ext_system_drc_grad_thr_read(ViPipe);

    pstDRC->u8DetailDarkStr    = ext_system_drc_detail_dark_max_read(ViPipe);
    pstDRC->u8DetailBrightStr  = ext_system_drc_detail_bright_min_read(ViPipe);
    pstDRC->u8DetailDarkStep   = ext_system_drc_detail_darkstep_read(ViPipe);
    pstDRC->u8DetailBrightStep = ext_system_drc_detail_brightstep_read(ViPipe);

    for (i = 0; i < ISP_DRC_CC_NODE_NUM; i++) {
        pstDRC->au16ColorCorrectionLut[i] = ext_system_drc_colorcc_lut_read(ViPipe, i);
    }

    for (i = 0; i < ISP_DRC_TM_NODE_NUM; i++) {
        pstDRC->au16ToneMappingValue[i] = ext_system_drc_tm_lut_read(ViPipe, i);
    }

    pstDRC->enCurveSelect = ext_system_drc_curve_sel_read(ViPipe);

    pstDRC->astCubicPoint[0].u16X = ext_system_drc_cubic_ltmx0_read(ViPipe);
    pstDRC->astCubicPoint[1].u16X = ext_system_drc_cubic_ltmx1_read(ViPipe);
    pstDRC->astCubicPoint[2].u16X = ext_system_drc_cubic_ltmx2_read(ViPipe);
    pstDRC->astCubicPoint[3].u16X = ext_system_drc_cubic_ltmx3_read(ViPipe);
    pstDRC->astCubicPoint[4].u16X = ext_system_drc_cubic_ltmx4_read(ViPipe);

    pstDRC->astCubicPoint[0].u16Y = ext_system_drc_cubic_ltmy0_read(ViPipe);
    pstDRC->astCubicPoint[1].u16Y = ext_system_drc_cubic_ltmy1_read(ViPipe);
    pstDRC->astCubicPoint[2].u16Y = ext_system_drc_cubic_ltmy2_read(ViPipe);
    pstDRC->astCubicPoint[3].u16Y = ext_system_drc_cubic_ltmy3_read(ViPipe);
    pstDRC->astCubicPoint[4].u16Y = ext_system_drc_cubic_ltmy4_read(ViPipe);

    pstDRC->astCubicPoint[0].u16Slope = ext_system_drc_cubic_slp0_read(ViPipe);
    pstDRC->astCubicPoint[1].u16Slope = ext_system_drc_cubic_slp1_read(ViPipe);
    pstDRC->astCubicPoint[2].u16Slope = ext_system_drc_cubic_slp2_read(ViPipe);
    pstDRC->astCubicPoint[3].u16Slope = ext_system_drc_cubic_slp3_read(ViPipe);
    pstDRC->astCubicPoint[4].u16Slope = ext_system_drc_cubic_slp4_read(ViPipe);

    pstDRC->u8DpDetectRangeRatio = ext_system_drc_dp_detect_rng_ratio_read(ViPipe);
    pstDRC->u8DpDetectThrSlo   = ext_system_drc_dp_detect_thr_slo_read(ViPipe);
    pstDRC->u16DpDetectThrMin = ext_system_drc_dp_detect_thr_min_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetDehazeAttr(VI_PIPE ViPipe, const ISP_DEHAZE_ATTR_S *pstDehazeAttr)
{
    GK_S32 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDehazeAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_BOOL(pstDehazeAttr->bEnable);
    ISP_CHECK_BOOL(pstDehazeAttr->bUserLutEnable);

    if (pstDehazeAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid OpType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDehazeAttr->u16TmprfltDecrCoef > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16TmprfltDecrCoef\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDehazeAttr->u16TmprfltIncrCoef > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16TmprfltIncrCoef\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_user_dehaze_lut_enable_write(ViPipe, pstDehazeAttr->bUserLutEnable);

    for (i = 0; i < 256; i++) {
        ext_system_dehaze_lut_write(ViPipe, i, pstDehazeAttr->au8DehazeLut[i]);
    }

    if (pstDehazeAttr->bUserLutEnable) {
        // 1:update the defog lut,FW will change it to 0 when the lut updating is finished.
        ext_system_user_dehaze_lut_update_write(ViPipe, GK_TRUE);
    }

    ext_system_dehaze_enable_write(ViPipe, pstDehazeAttr->bEnable);
    ext_system_dehaze_manu_mode_write(ViPipe, pstDehazeAttr->enOpType);
    ext_system_manual_dehaze_strength_write(ViPipe, pstDehazeAttr->stManual.u8strength);
    ext_system_manual_dehaze_autostrength_write(ViPipe, pstDehazeAttr->stAuto.u8strength);
    ext_system_dehaze_tfic_write(ViPipe, pstDehazeAttr->u16TmprfltIncrCoef);
    ext_system_dehaze_tfdc_write(ViPipe, pstDehazeAttr->u16TmprfltDecrCoef);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDehazeAttr(VI_PIPE ViPipe, ISP_DEHAZE_ATTR_S *pstDehazeAttr)
{
    GK_S32 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDehazeAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDehazeAttr->bEnable             = ext_system_dehaze_enable_read(ViPipe);
    pstDehazeAttr->enOpType            = (ISP_OP_TYPE_E)ext_system_dehaze_manu_mode_read(ViPipe);
    pstDehazeAttr->stManual.u8strength = ext_system_manual_dehaze_strength_read(ViPipe);
    pstDehazeAttr->stAuto.u8strength   = ext_system_manual_dehaze_autostrength_read(ViPipe);
    pstDehazeAttr->bUserLutEnable      = ext_system_user_dehaze_lut_enable_read(ViPipe);

    for (i = 0; i < 256; i++) {
        pstDehazeAttr->au8DehazeLut[i] = ext_system_dehaze_lut_read(ViPipe, i);
    }

    pstDehazeAttr->u16TmprfltIncrCoef = ext_system_dehaze_tfic_read(ViPipe);
    pstDehazeAttr->u16TmprfltDecrCoef = ext_system_dehaze_tfdc_read(ViPipe);

    return GK_SUCCESS;
}

extern GK_S32 MPI_VI_SetIspDISAttr(VI_PIPE ViPipe, GK_BOOL bDisEnable);
extern GK_S32 MPI_VI_GetIspDISAttr(VI_PIPE ViPipe, GK_BOOL *pbDisEnable);

GK_S32 MPI_ISP_SetDISAttr(VI_PIPE ViPipe, const ISP_DIS_ATTR_S *pstDISAttr)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDISAttr);
    ISP_CHECK_BOOL(pstDISAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return MPI_VI_SetIspDISAttr(ViPipe, pstDISAttr->bEnable);
}

GK_S32 MPI_ISP_GetDISAttr(VI_PIPE ViPipe, ISP_DIS_ATTR_S *pstDISAttr)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDISAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return MPI_VI_GetIspDISAttr(ViPipe, &pstDISAttr->bEnable);
}

GK_S32 MPI_ISP_SetFSWDRAttr(VI_PIPE ViPipe, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    GK_U8 j;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_BOOL(pstFSWDRAttr->stWDRCombine.bMotionComp);
    ISP_CHECK_BOOL(pstFSWDRAttr->stWDRCombine.stWDRMdt.bMDRefFlicker);
    ISP_CHECK_BOOL(pstFSWDRAttr->stWDRCombine.bForceLong);
    ISP_CHECK_BOOL(pstFSWDRAttr->stBnr.bShortFrameNR);
    ISP_CHECK_BOOL(pstFSWDRAttr->stWDRCombine.stWDRMdt.bShortExpoChk);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstFSWDRAttr->enWDRMergeMode >= MERGE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enWDRMergeMode !\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stBnr.enBnrMode >= BNR_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enBnrMode !\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.u16ShortThr > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ShortThr !\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->stWDRCombine.u16LongThr > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16LongThr !\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->stWDRCombine.u16LongThr > pstFSWDRAttr->stWDRCombine.u16ShortThr) {
        ISP_TRACE(MODULE_DBG_ERR, "u16LongThresh should NOT be larger than u16ShortThresh !\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.stWDRMdt.enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.stWDRMdt.stManual.u8MdThrLowGain >
        pstFSWDRAttr->stWDRCombine.stWDRMdt.stManual.u8MdThrHigGain) {
        ISP_TRACE(MODULE_DBG_ERR, "u8MdThrLowGain should NOT be larger than u8MdThrHigGain!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->stWDRCombine.u16ForceLongHigThr > 0XFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ForceLongHigThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.u16ForceLongLowThr > pstFSWDRAttr->stWDRCombine.u16ForceLongHigThr) {
        ISP_TRACE(MODULE_DBG_ERR, "u16ForceLongLowThr should NOT be larger than u16ForceLongHigThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stBnr.u8ShortFrameNRStr > 0X3F) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8Mot2SigCwgtHigh!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stBnr.u8FusionBnrStr > 0X3F) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8Mot2SigCwgtHigh!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (j = 0; j < 4; j++) {
        if (pstFSWDRAttr->stFusion.au16FusionThr[j] > 0x3FFF) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16FusionThr!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) {
        if (pstFSWDRAttr->stWDRCombine.stWDRMdt.stAuto.au8MdThrLowGain[j] >
            pstFSWDRAttr->stWDRCombine.stWDRMdt.stAuto.au8MdThrHigGain[j]) {
            ISP_TRACE(MODULE_DBG_ERR, "au8MdThrLowGain[%d] should NOT be larger than au8MdThrHigGain[%d]!\n", j, j);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_wdr_auto_mdthr_low_gain_write(ViPipe, j, pstFSWDRAttr->stWDRCombine.stWDRMdt.stAuto.au8MdThrLowGain[j]);
        ext_system_wdr_auto_mdthrg_gain_write(ViPipe, j, pstFSWDRAttr->stWDRCombine.stWDRMdt.stAuto.au8MdThrHigGain[j]);
    }

    if (pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtFullThd > 254) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8MdtFullThd!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtStillThd > 254) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8MdtStillThd!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtLongBlend > 254) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8MdtLongBlend!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stBnr.u8FullMdtSigWgt > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8FullMdtSigWgt!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stBnr.u8FullMdtSigGWgt > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8FullMdtSigGWgt!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stBnr.u8FullMdtSigRBWgt > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8FullMdtSigRBWgt!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->stWDRCombine.stWDRMdt.u16ShortCheckThd > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ShortCheckThd!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->enWDRWbgainPosition >= WDR_WBGAIN_POSITION_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enWDRWbgainPosition %d!\n", pstFSWDRAttr->enWDRWbgainPosition);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_wdr_wbgain_position_write(ViPipe, pstFSWDRAttr->enWDRWbgainPosition);

    ext_system_fusion_mode_write(ViPipe, pstFSWDRAttr->enWDRMergeMode);
    ext_system_mdt_en_write(ViPipe, pstFSWDRAttr->stWDRCombine.bMotionComp);
    ext_system_wdr_longthr_write(ViPipe, pstFSWDRAttr->stWDRCombine.u16LongThr);
    ext_system_wdr_shortthr_write(ViPipe, pstFSWDRAttr->stWDRCombine.u16ShortThr);

    ext_system_wdr_manual_mode_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.enOpType);
    ext_system_wdr_mdref_flicker_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.bMDRefFlicker);
    ext_system_wdr_manual_mdthr_low_gain_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.stManual.u8MdThrLowGain);
    ext_system_wdr_manual_mdthrg_gain_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.stManual.u8MdThrHigGain);

    for (j = 0; j < 4; j++) {
        ext_system_fusion_thr_write(ViPipe, j, pstFSWDRAttr->stFusion.au16FusionThr[j]);
    }

    ext_system_bnr_mode_write(ViPipe, pstFSWDRAttr->stBnr.enBnrMode);
    ext_system_wdr_coef_update_en_write(ViPipe, GK_TRUE);

    ext_system_wdr_forcelong_en_write(ViPipe, pstFSWDRAttr->stWDRCombine.bForceLong);
    ext_system_wdr_forcelonggh_thd_write(ViPipe, pstFSWDRAttr->stWDRCombine.u16ForceLongHigThr);
    ext_system_wdr_forcelong_low_thd_write(ViPipe, pstFSWDRAttr->stWDRCombine.u16ForceLongLowThr);

    ext_system_wdr_sfnr_en_write(ViPipe, pstFSWDRAttr->stBnr.bShortFrameNR);
    ext_system_wdr_shortframe_nrstr_write(ViPipe, pstFSWDRAttr->stBnr.u8ShortFrameNRStr);

    ext_system_wdr_fusionbnrstr_write(ViPipe, pstFSWDRAttr->stBnr.u8FusionBnrStr);
    ext_system_wdr_shortexpo_chk_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.bShortExpoChk);

    ext_system_wdr_shortcheck_thd_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.u16ShortCheckThd);
    ext_system_wdr_mdt_full_thr_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtFullThd);
    ext_system_wdr_mdt_still_thr_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtStillThd);
    ext_system_wdr_mdt_long_blend_write(ViPipe, pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtLongBlend);

    for (j = 0; j < NoiseSet_EleNum; j++) {
        ext_system_wdr_floorset_write(ViPipe, j, pstFSWDRAttr->stBnr.au8NoiseFloor[j]);
    }

    ext_system_wdr_mot2sig_gwgtgh_write(ViPipe, pstFSWDRAttr->stBnr.u8FullMdtSigGWgt);
    ext_system_wdr_mot2sig_cwgtgh_write(ViPipe, pstFSWDRAttr->stBnr.u8FullMdtSigRBWgt);
    ext_system_wdr_full_mot_sigma_weight_write(ViPipe, pstFSWDRAttr->stBnr.u8FullMdtSigWgt);
    ext_system_wdr_g_sigma_gain1_write(ViPipe, pstFSWDRAttr->stBnr.au8GsigmaGain[0]);
    ext_system_wdr_g_sigma_gain2_write(ViPipe, pstFSWDRAttr->stBnr.au8GsigmaGain[1]);
    ext_system_wdr_g_sigma_gain3_write(ViPipe, pstFSWDRAttr->stBnr.au8GsigmaGain[2]);
    ext_system_wdr_c_sigma_gain1_write(ViPipe, pstFSWDRAttr->stBnr.au8RBsigmaGain[0]);
    ext_system_wdr_c_sigma_gain2_write(ViPipe, pstFSWDRAttr->stBnr.au8RBsigmaGain[1]);
    ext_system_wdr_c_sigma_gain3_write(ViPipe, pstFSWDRAttr->stBnr.au8RBsigmaGain[2]);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetFSWDRAttr(VI_PIPE ViPipe, ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    GK_U8 j;
    GK_U8 u8Tmp;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstFSWDRAttr->enWDRMergeMode           = ext_system_fusion_mode_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.bMotionComp = ext_system_mdt_en_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.u16LongThr  = ext_system_wdr_longthr_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.u16ShortThr = ext_system_wdr_shortthr_read(ViPipe);

    pstFSWDRAttr->stWDRCombine.stWDRMdt.enOpType      = ext_system_wdr_manual_mode_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.stWDRMdt.bMDRefFlicker = ext_system_wdr_mdref_flicker_read(ViPipe);

    pstFSWDRAttr->stWDRCombine.stWDRMdt.stManual.u8MdThrLowGain = ext_system_wdr_manual_mdthr_low_gain_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.stWDRMdt.stManual.u8MdThrHigGain = ext_system_wdr_manual_mdthrg_gain_read(ViPipe);

    for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) {
        pstFSWDRAttr->stWDRCombine.stWDRMdt.stAuto.au8MdThrLowGain[j] = ext_system_wdr_auto_mdthr_low_gain_read(ViPipe, j);
        pstFSWDRAttr->stWDRCombine.stWDRMdt.stAuto.au8MdThrHigGain[j] = ext_system_wdr_auto_mdthrg_gain_read(ViPipe, j);
    }
    for (j = 0; j < 4; j++) {
        pstFSWDRAttr->stFusion.au16FusionThr[j] = ext_system_fusion_thr_read(ViPipe, j);
    }
    /*************************WDR DGAIN CONIFG ***************************/
    u8Tmp = ext_system_wdr_wbgain_position_read(ViPipe);
    if (u8Tmp == ISP_EXT_SYSTEM_WDR_WBGAIN_IN_DG1) {
        pstFSWDRAttr->enWDRWbgainPosition = WDR_WBGAIN_IN_DG1;
    } else if (u8Tmp == ISP_EXT_SYSTEM_WDR_WBGAIN_IN_WB) {
        pstFSWDRAttr->enWDRWbgainPosition = WDR_WBGAIN_IN_WB;
    } else {
        pstFSWDRAttr->enWDRWbgainPosition = WDR_WBGAIN_POSITION_BUTT;
    }
    pstFSWDRAttr->stBnr.enBnrMode                 = ext_system_bnr_mode_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.bForceLong         = ext_system_wdr_forcelong_en_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.u16ForceLongHigThr = ext_system_wdr_forcelonggh_thd_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.u16ForceLongLowThr = ext_system_wdr_forcelong_low_thd_read(ViPipe);
    pstFSWDRAttr->stBnr.bShortFrameNR             = ext_system_wdr_sfnr_en_read(ViPipe);
    pstFSWDRAttr->stBnr.u8ShortFrameNRStr         = ext_system_wdr_shortframe_nrstr_read(ViPipe);

    pstFSWDRAttr->stBnr.u8FusionBnrStr                = ext_system_wdr_fusionbnrstr_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.stWDRMdt.bShortExpoChk = ext_system_wdr_shortexpo_chk_read(ViPipe);

    pstFSWDRAttr->stWDRCombine.stWDRMdt.u16ShortCheckThd = ext_system_wdr_shortcheck_thd_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtFullThd     = ext_system_wdr_mdt_full_thr_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtStillThd    = ext_system_wdr_mdt_still_thr_read(ViPipe);
    pstFSWDRAttr->stWDRCombine.stWDRMdt.u8MdtLongBlend   = ext_system_wdr_mdt_long_blend_read(ViPipe);

    for (j = 0; j < NoiseSet_EleNum; j++) {
        pstFSWDRAttr->stBnr.au8NoiseFloor[j] = ext_system_wdr_floorset_read(ViPipe, j);
    }

    pstFSWDRAttr->stBnr.u8FullMdtSigGWgt  = ext_system_wdr_mot2sig_gwgtgh_read(ViPipe);
    pstFSWDRAttr->stBnr.u8FullMdtSigRBWgt = ext_system_wdr_mot2sig_cwgtgh_read(ViPipe);
    pstFSWDRAttr->stBnr.u8FullMdtSigWgt   = ext_system_wdr_full_mot_sigma_weight_read(ViPipe);
    pstFSWDRAttr->stBnr.au8GsigmaGain[0]  = ext_system_wdr_g_sigma_gain1_read(ViPipe);
    pstFSWDRAttr->stBnr.au8GsigmaGain[1]  = ext_system_wdr_g_sigma_gain2_read(ViPipe);
    pstFSWDRAttr->stBnr.au8GsigmaGain[2]  = ext_system_wdr_g_sigma_gain3_read(ViPipe);
    pstFSWDRAttr->stBnr.au8RBsigmaGain[0] = ext_system_wdr_c_sigma_gain1_read(ViPipe);
    pstFSWDRAttr->stBnr.au8RBsigmaGain[1] = ext_system_wdr_c_sigma_gain2_read(ViPipe);
    pstFSWDRAttr->stBnr.au8RBsigmaGain[2] = ext_system_wdr_c_sigma_gain3_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetFocusStatistics(VI_PIPE ViPipe, ISP_AF_STATISTICS_S *pstAfStat)
{
#ifdef CONFIG_ISP_AF_SUPPORT
    GK_U8 u8Col, u8Row, u8WdrChn;
    GK_S32 i, j;
    VI_PIPE_WDR_ATTR_S stWdrAttr;

    ISP_STATISTICS_CTRL_U unStatKey;
    ISP_STAT_INFO_S stActStatInfo;
    ISP_STAT_S *pstIspActStat = GK_NULL;
    GK_S32 s32Ret;
    GK_U32 u32KeyLowbit, u32KeyHighbit;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstAfStat);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u32KeyLowbit  = ext_system_statistics_ctrl_lowbit_read(ViPipe);
    u32KeyHighbit = ext_system_statistics_ctrlghbit_read(ViPipe);
    unStatKey.u64Key = ((GK_U64)u32KeyHighbit << 32) + u32KeyLowbit;

    u8Col = MIN2(ext_af_window_hnum_read(ViPipe), AF_ZONE_COLUMN);
    u8Row = MIN2(ext_af_window_vnum_read(ViPipe), AF_ZONE_ROW);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_ACT_GET, &stActStatInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Active Stat Buffer Err\n");
        return ERR_CODE_ISP_NO_INT;
    }

    stActStatInfo.pVirtAddr = MPI_SYS_MmapCache(stActStatInfo.u64PhyAddr, sizeof(ISP_STAT_S));
    if (stActStatInfo.pVirtAddr == GK_NULL) {
        return ERR_CODE_ISP_NULL_PTR;
    }

    pstIspActStat = (ISP_STAT_S *)stActStatInfo.pVirtAddr;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &stWdrAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get WDR attr failed\n", ViPipe);
        MPI_SYS_Munmap(stActStatInfo.pVirtAddr, sizeof(ISP_STAT_S));
        return s32Ret;
    }

    u8WdrChn = MIN2(stWdrAttr.stDevBindPipe.u32Num, WDR_CHN_MAX);

    /* AF FE Stat */
    if (unStatKey.bit1FEAfStat && stWdrAttr.bMastPipe) {
        ISP_GetFeFocusStatistics(ViPipe, &pstAfStat->stFEAFStat, pstIspActStat, u8WdrChn);
    }

    /* BE */
    if (unStatKey.bit1BEAfStat) {
        for (i = 0; i < u8Row; i++) {
            for (j = 0; j < u8Col; j++) {
                pstAfStat->stBEAFStat.stZoneMetrics[i][j].u16v1 = pstIspActStat->stBEAfStat.stZoneMetrics[i][j].u16v1;
                pstAfStat->stBEAFStat.stZoneMetrics[i][j].u16h1 = pstIspActStat->stBEAfStat.stZoneMetrics[i][j].u16h1;
                pstAfStat->stBEAFStat.stZoneMetrics[i][j].u16v2 = pstIspActStat->stBEAfStat.stZoneMetrics[i][j].u16v2;
                pstAfStat->stBEAFStat.stZoneMetrics[i][j].u16h2 = pstIspActStat->stBEAfStat.stZoneMetrics[i][j].u16h2;
                pstAfStat->stBEAFStat.stZoneMetrics[i][j].u16y  = pstIspActStat->stBEAfStat.stZoneMetrics[i][j].u16y;
                pstAfStat->stBEAFStat.stZoneMetrics[i][j].u16HlCnt = pstIspActStat->stBEAfStat.stZoneMetrics[i][j].u16HlCnt;
            }
        }
    }

    MPI_SYS_Munmap((GK_VOID *)stActStatInfo.pVirtAddr, sizeof(ISP_STAT_S));

    ISP_GetAFGridInfo(ViPipe, &pstAfStat->stFEAFGridInfo, &pstAfStat->stBEAFGridInfo);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetAEStatistics(VI_PIPE ViPipe, ISP_AE_STATISTICS_S *pstAeStat)
{
    GK_S32 i, j, k;
    GK_U32 u32PipeNum;
    GK_S32 s32Ret;
    GK_U32 u32KeyLowbit, u32KeyHighbit;

    VI_PIPE_WDR_ATTR_S stWdrAttr;
    ISP_STATISTICS_CTRL_U unStatKey;
    ISP_STAT_INFO_S stIspStatInfo;
    ISP_STAT_S *pstIspStat;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstAeStat);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_ACT_GET, &stIspStatInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Active Stat Buffer Err\n");
        return ERR_CODE_ISP_NOMEM;
    }

    stIspStatInfo.pVirtAddr = MPI_SYS_MmapCache(stIspStatInfo.u64PhyAddr, sizeof(ISP_STAT_S));
    if (stIspStatInfo.pVirtAddr == GK_NULL) {
        return ERR_CODE_ISP_NULL_PTR;
    }
    pstIspStat = (ISP_STAT_S *)stIspStatInfo.pVirtAddr;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &stWdrAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get WDR attr failed\n", ViPipe);
        MPI_SYS_Munmap(stIspStatInfo.pVirtAddr, sizeof(ISP_STAT_S));
        return s32Ret;
    }

    u32KeyLowbit  = ext_system_statistics_ctrl_lowbit_read(ViPipe);
    u32KeyHighbit = ext_system_statistics_ctrlghbit_read(ViPipe);
    unStatKey.u64Key = ((GK_U64)u32KeyHighbit << 32) + u32KeyLowbit;

    /* AE FE Stat */
    u32PipeNum = MIN2(stWdrAttr.stDevBindPipe.u32Num, ISP_CHN_MAX_NUM);
    k = 0;
    if (unStatKey.bit1FEAeGloStat && stWdrAttr.bMastPipe) {
        for (; k < u32PipeNum; k++) {
            for (i = 0; i < HIST_NUM; i++) {
                pstAeStat->au32FEHist1024Value[k][i] = pstIspStat->stFEAeStat1.au32HistogramMemArray[k][i];
            }

            pstAeStat->au16FEGlobalAvg[k][0] = pstIspStat->stFEAeStat2.u16GlobalAvgR[k];
            pstAeStat->au16FEGlobalAvg[k][1] = pstIspStat->stFEAeStat2.u16GlobalAvgGr[k];
            pstAeStat->au16FEGlobalAvg[k][2] = pstIspStat->stFEAeStat2.u16GlobalAvgGb[k];
            pstAeStat->au16FEGlobalAvg[k][3] = pstIspStat->stFEAeStat2.u16GlobalAvgB[k];
        }

        for (; k < ISP_CHN_MAX_NUM; k++) {
            for (i = 0; i < HIST_NUM; i++) {
                pstAeStat->au32FEHist1024Value[k][i] = 0;
            }

            pstAeStat->au16FEGlobalAvg[k][0] = 0;
            pstAeStat->au16FEGlobalAvg[k][1] = 0;
            pstAeStat->au16FEGlobalAvg[k][2] = 0;
            pstAeStat->au16FEGlobalAvg[k][3] = 0;
        }
    }

    k = 0;
    if (unStatKey.bit1FEAeLocStat && stWdrAttr.bMastPipe) {
        for (; k < u32PipeNum; k++) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    pstAeStat->au16FEZoneAvg[k][i][j][0] = pstIspStat->stFEAeStat3.au16ZoneAvg[k][i][j][0]; /* R */
                    pstAeStat->au16FEZoneAvg[k][i][j][1] = pstIspStat->stFEAeStat3.au16ZoneAvg[k][i][j][1]; /* Gr */
                    pstAeStat->au16FEZoneAvg[k][i][j][2] = pstIspStat->stFEAeStat3.au16ZoneAvg[k][i][j][2]; /* Gb */
                    pstAeStat->au16FEZoneAvg[k][i][j][3] = pstIspStat->stFEAeStat3.au16ZoneAvg[k][i][j][3]; /* B */
                }
            }
        }

        for (; k < ISP_CHN_MAX_NUM; k++) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    pstAeStat->au16FEZoneAvg[k][i][j][0] = 0; /* R */
                    pstAeStat->au16FEZoneAvg[k][i][j][1] = 0; /* Gr */
                    pstAeStat->au16FEZoneAvg[k][i][j][2] = 0; /* Gb */
                    pstAeStat->au16FEZoneAvg[k][i][j][3] = 0; /* B */
                }
            }
        }
    }

    /* AE BE Stat */
    if (unStatKey.bit1BEAeGloStat) {
        for (i = 0; i < HIST_NUM; i++) {
            pstAeStat->au32BEHist1024Value[i] = pstIspStat->stBEAeStat1.au32HistogramMemArray[i];
        }

        pstAeStat->au16BEGlobalAvg[0] = pstIspStat->stBEAeStat2.u16GlobalAvgR;
        pstAeStat->au16BEGlobalAvg[1] = pstIspStat->stBEAeStat2.u16GlobalAvgGr;
        pstAeStat->au16BEGlobalAvg[2] = pstIspStat->stBEAeStat2.u16GlobalAvgGb;
        pstAeStat->au16BEGlobalAvg[3] = pstIspStat->stBEAeStat2.u16GlobalAvgB;
    }

    if (unStatKey.bit1BEAeLocStat) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                pstAeStat->au16BEZoneAvg[i][j][0] = pstIspStat->stBEAeStat3.au16ZoneAvg[i][j][0]; /* R */
                pstAeStat->au16BEZoneAvg[i][j][1] = pstIspStat->stBEAeStat3.au16ZoneAvg[i][j][1]; /* Gr */
                pstAeStat->au16BEZoneAvg[i][j][2] = pstIspStat->stBEAeStat3.au16ZoneAvg[i][j][2]; /* Gb */
                pstAeStat->au16BEZoneAvg[i][j][3] = pstIspStat->stBEAeStat3.au16ZoneAvg[i][j][3]; /* B */
            }
        }
    }

    MPI_SYS_Munmap((GK_VOID *)pstIspStat, sizeof(ISP_STAT_S));
    ISP_GetAEGridInfo(ViPipe, &pstAeStat->stFEGridInfo, &pstAeStat->stBEGridInfo);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetAEStitchStatistics(VI_PIPE ViPipe, ISP_AE_STITCH_STATISTICS_S *pstAeStitchStat)
{
    return ISP_GetAEStitchStatistics(ViPipe, pstAeStitchStat);
}

GK_S32 MPI_ISP_GetMGStatistics(VI_PIPE ViPipe, ISP_MG_STATISTICS_S *pstMgStat)
{
    GK_S32 i, j;
    GK_S32 s32Ret;
    GK_U32 u32KeyLowbit, u32KeyHighbit;

    ISP_STATISTICS_CTRL_U unStatKey;
    ISP_STAT_INFO_S stIspStatInfo;
    ISP_STAT_S *pstIspStat;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstMgStat);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_ACT_GET, &stIspStatInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Active Stat Buffer Err\n");
        return ERR_CODE_ISP_NOMEM;
    }

    stIspStatInfo.pVirtAddr = MPI_SYS_MmapCache(stIspStatInfo.u64PhyAddr, sizeof(ISP_STAT_S));
    if (stIspStatInfo.pVirtAddr == GK_NULL) {
        return ERR_CODE_ISP_NULL_PTR;
    }
    pstIspStat = (ISP_STAT_S *)stIspStatInfo.pVirtAddr;

    u32KeyLowbit  = ext_system_statistics_ctrl_lowbit_read(ViPipe);
    u32KeyHighbit = ext_system_statistics_ctrlghbit_read(ViPipe);
    unStatKey.u64Key = ((GK_U64)u32KeyHighbit << 32) + u32KeyLowbit;

    /* AE FE Stat */
    if (unStatKey.bit1MgStat) {
        for (i = 0; i < MG_ZONE_ROW; i++) {
            for (j = 0; j < MG_ZONE_COLUMN; j++) {
                pstMgStat->au16ZoneAvg[i][j][0] = pstIspStat->stMgStat.au16ZoneAvg[i][j][0]; /* R */
                pstMgStat->au16ZoneAvg[i][j][1] = pstIspStat->stMgStat.au16ZoneAvg[i][j][1]; /* Gr */
                pstMgStat->au16ZoneAvg[i][j][2] = pstIspStat->stMgStat.au16ZoneAvg[i][j][2]; /* Gb */
                pstMgStat->au16ZoneAvg[i][j][3] = pstIspStat->stMgStat.au16ZoneAvg[i][j][3]; /* B */
            }
        }
    }

    MPI_SYS_Munmap((GK_VOID *)pstIspStat, sizeof(ISP_STAT_S));
    ISP_GetMGGridInfo(ViPipe, &pstMgStat->stGridInfo);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetWBStitchStatistics(VI_PIPE ViPipe, ISP_WB_STITCH_STATISTICS_S *pstStitchWBStat)
{
    return ISP_GetWBStitchStatistics(ViPipe, pstStitchWBStat);
}

GK_S32 MPI_ISP_GetWBStatistics(VI_PIPE ViPipe, ISP_WB_STATISTICS_S *pstWBStat)
{
    GK_S32 i;
    ISP_STATISTICS_CTRL_U unStatKey;
    ISP_STAT_INFO_S stActStatInfo;
    ISP_STAT_S *pstIspActStat;
    GK_S32 s32Ret;
    GK_U32 u32KeyLowbit, u32KeyHighbit;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstWBStat);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u32KeyLowbit  = ext_system_statistics_ctrl_lowbit_read(ViPipe);
    u32KeyHighbit = ext_system_statistics_ctrlghbit_read(ViPipe);
    unStatKey.u64Key = ((GK_U64)u32KeyHighbit << 32) + u32KeyLowbit;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_ACT_GET, &stActStatInfo);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Active Stat Buffer Err\n");
        return ERR_CODE_ISP_NOMEM;
    }

    stActStatInfo.pVirtAddr = MPI_SYS_MmapCache(stActStatInfo.u64PhyAddr, sizeof(ISP_STAT_S));

    if (stActStatInfo.pVirtAddr == GK_NULL) {
        return ERR_CODE_ISP_NULL_PTR;
    }

    pstIspActStat = (ISP_STAT_S *)stActStatInfo.pVirtAddr;

    if (unStatKey.bit1AwbStat1) {
        pstWBStat->u16GlobalR = pstIspActStat->stAwbStat1.u16MeteringAwbAvgR;
        pstWBStat->u16GlobalG = pstIspActStat->stAwbStat1.u16MeteringAwbAvgG;
        pstWBStat->u16GlobalB = pstIspActStat->stAwbStat1.u16MeteringAwbAvgB;
        pstWBStat->u16CountAll = pstIspActStat->stAwbStat1.u16MeteringAwbCountAll;
    }

    if (unStatKey.bit1AwbStat2) {
        for (i = 0; i < AWB_ZONE_NUM; i++) {
            pstWBStat->au16ZoneAvgR[i] = pstIspActStat->stAwbStat2.au16MeteringMemArrayAvgR[i];
            pstWBStat->au16ZoneAvgG[i] = pstIspActStat->stAwbStat2.au16MeteringMemArrayAvgG[i];
            pstWBStat->au16ZoneAvgB[i] = pstIspActStat->stAwbStat2.au16MeteringMemArrayAvgB[i];
            pstWBStat->au16ZoneCountAll[i] = pstIspActStat->stAwbStat2.au16MeteringMemArrayCountAll[i];
        }
    }

    MPI_SYS_Munmap(stActStatInfo.pVirtAddr, sizeof(ISP_STAT_S));

    ISP_GetWBGridInfo(ViPipe, &pstWBStat->stGridInfo);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetStatisticsConfig(VI_PIPE ViPipe, const ISP_STATISTICS_CFG_S *pstStatCfg)
{
    GK_U8  u8MaxAwbCol, u8MaxAwbRow;
    GK_U16 u16Width, u16Height;
    GK_S32 i, j;
    GK_U32 u32KeyLowbit, u32KeyHighbit;
    ISP_WORKING_MODE_S stIspWorkMode;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode) != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Work Mode error!\n");
        return GK_FAILURE;
    }

    u16Width  = ext_system_be_total_width_read(ViPipe);
    u16Height = ext_system_be_total_height_read(ViPipe);

    if (stIspWorkMode.enIspDataInputMode == ISP_MODE_RAW) {
        // AWB
        if (pstStatCfg->stWBCfg.enAWBSwitch >= ISP_AWB_SWITCH_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid WB enAWBSwitch %d !\n", pstStatCfg->stWBCfg.enAWBSwitch);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.u16CrMax > 0xFFF) {
            ISP_TRACE(MODULE_DBG_ERR, "Max value WB u16CrMax is 0xFFF!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.u16CbMax > 0xFFF) {
            ISP_TRACE(MODULE_DBG_ERR, "Max value of WB u16CbMax is 0xFFF!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.u16BlackLevel > pstStatCfg->stWBCfg.u16WhiteLevel) {
            ISP_TRACE(MODULE_DBG_ERR, "WB BlackLevel should not larger than WhiteLevel!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stWBCfg.u16CrMin) > (pstStatCfg->stWBCfg.u16CrMax)) {
            ISP_TRACE(MODULE_DBG_ERR, "WB CrMin should not larger than CrMax!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.u16CbMin > pstStatCfg->stWBCfg.u16CbMax) {
            ISP_TRACE(MODULE_DBG_ERR, "WB CbMin should not larger than CbMax!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        u8MaxAwbCol = MIN2(AWB_ZONE_ORIG_COLUMN, u16Width / AWB_MIN_WIDTH);
        u8MaxAwbRow = MIN2(AWB_ZONE_ORIG_ROW, u16Height / AWB_MIN_HEIGHT);

        if ((pstStatCfg->stWBCfg.u16ZoneRow < 1) || (pstStatCfg->stWBCfg.u16ZoneRow > u8MaxAwbRow)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ZoneRow! Value range:[1, %d]!\n", u8MaxAwbRow);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stWBCfg.u16ZoneCol < stIspWorkMode.u8BlockNum) || (pstStatCfg->stWBCfg.u16ZoneCol > u8MaxAwbCol)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ZoneCol! Value range:[%d, %d]!\n", stIspWorkMode.u8BlockNum, u8MaxAwbCol);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.u16ZoneBin == 0x0) {
            ISP_TRACE(MODULE_DBG_ERR, "The value of WB u16ZoneBin should be larger than 0x0!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.u16ZoneBin > AWB_ZONE_BIN) {
            ISP_TRACE(MODULE_DBG_ERR, "Max value of WB u16ZoneBin is %d!\n", AWB_ZONE_BIN);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        for (i = 1; i < 4; i++) {
            if (pstStatCfg->stWBCfg.au16HistBinThresh[i] < pstStatCfg->stWBCfg.au16HistBinThresh[i - 1]) {
                ISP_TRACE(MODULE_DBG_ERR, "Max value of WB au16HistBinThresh[%d] should not be smaller than au16HistBinThresh[%d]!\n",
                          i, i - 1);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }

        if (pstStatCfg->stWBCfg.enAWBSwitch == ISP_AWB_AFTER_DG) {
            ext_system_awb_switch_write(ViPipe, ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_DG);
        } else if (pstStatCfg->stWBCfg.enAWBSwitch == ISP_AWB_AFTER_DRC) {
            ext_system_awb_switch_write(ViPipe, ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC);
        } else if (pstStatCfg->stWBCfg.enAWBSwitch == ISP_AWB_AFTER_Expander) {
            ext_system_awb_switch_write(ViPipe, ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER);
        } else {
            ISP_TRACE(MODULE_DBG_ERR, "not support!");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ISP_CHECK_BOOL(pstStatCfg->stWBCfg.stCrop.bEnable);

        if (pstStatCfg->stWBCfg.stCrop.u16W < pstStatCfg->stWBCfg.u16ZoneCol * AWB_MIN_WIDTH) {
            ISP_TRACE(MODULE_DBG_ERR, "u16W should NOT be less than %d(u16ZoneCol * 60)!\n",
                      pstStatCfg->stWBCfg.u16ZoneCol * AWB_MIN_WIDTH);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stWBCfg.stCrop.u16H < pstStatCfg->stWBCfg.u16ZoneRow * AWB_MIN_HEIGHT) {
            ISP_TRACE(MODULE_DBG_ERR, "u16H should NOT be less than %d(u16ZoneRow *14)!\n",
                      pstStatCfg->stWBCfg.u16ZoneRow * AWB_MIN_HEIGHT);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stWBCfg.stCrop.u16X + pstStatCfg->stWBCfg.stCrop.u16W) > u16Width) {
            ISP_TRACE(MODULE_DBG_ERR, "u16X + u16W should NOT be larger than %d!\n", u16Width);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stWBCfg.stCrop.u16Y + pstStatCfg->stWBCfg.stCrop.u16H) > u16Height) {
            ISP_TRACE(MODULE_DBG_ERR, "u16Y + u16H should NOT be larger than %d!\n", u16Height);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_awb_white_level_write(ViPipe, pstStatCfg->stWBCfg.u16WhiteLevel);
        ext_system_awb_black_level_write(ViPipe, pstStatCfg->stWBCfg.u16BlackLevel);
        ext_system_awb_cr_ref_max_write(ViPipe, pstStatCfg->stWBCfg.u16CrMax);
        ext_system_awb_cr_ref_min_write(ViPipe, pstStatCfg->stWBCfg.u16CrMin);
        ext_system_awb_cb_ref_max_write(ViPipe, pstStatCfg->stWBCfg.u16CbMax);
        ext_system_awb_cb_ref_min_write(ViPipe, pstStatCfg->stWBCfg.u16CbMin);

        ext_system_awb_vnum_write(ViPipe, pstStatCfg->stWBCfg.u16ZoneRow);
        ext_system_awb_hnum_write(ViPipe, pstStatCfg->stWBCfg.u16ZoneCol);
        ext_system_awb_zone_bin_write(ViPipe, pstStatCfg->stWBCfg.u16ZoneBin);
        ext_system_awbst_bin_thresh0_write(ViPipe, pstStatCfg->stWBCfg.au16HistBinThresh[0]);
        ext_system_awbst_bin_thresh1_write(ViPipe, pstStatCfg->stWBCfg.au16HistBinThresh[1]);
        ext_system_awbst_bin_thresh2_write(ViPipe, pstStatCfg->stWBCfg.au16HistBinThresh[2]);
        ext_system_awbst_bin_thresh3_write(ViPipe, pstStatCfg->stWBCfg.au16HistBinThresh[3]);

        ext_system_awb_crop_en_write(ViPipe, pstStatCfg->stWBCfg.stCrop.bEnable);
        ext_system_awb_crop_x_write(ViPipe, pstStatCfg->stWBCfg.stCrop.u16X);
        ext_system_awb_crop_y_write(ViPipe, pstStatCfg->stWBCfg.stCrop.u16Y);
        ext_system_awb_crop_height_write(ViPipe, pstStatCfg->stWBCfg.stCrop.u16H);
        ext_system_awb_crop_width_write(ViPipe, pstStatCfg->stWBCfg.stCrop.u16W);

        ext_system_wb_statistics_mpi_update_en_write(ViPipe, GK_TRUE);

        // AE
        if (pstStatCfg->stAECfg.enAESwitch >= ISP_AE_SWITCH_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid AE Switch %d!\n", pstStatCfg->stAECfg.enAESwitch);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.stHistConfig.enHistSkipX >= ISP_AE_HIST_SKIP_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "u8HistSkipX should not be larger than 6\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.stHistConfig.enHistSkipY >= ISP_AE_HIST_SKIP_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "u8HistSkipY should not be larger than 6\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.stHistConfig.enHistOffsetX >= ISP_AE_HIST_OFFSET_X_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "u8HistOffsetX should not be larger than 1\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.stHistConfig.enHistOffsetY >= ISP_AE_HIST_OFFSET_Y_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "u8HistOffsetY should not larger than 1\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.enFourPlaneMode >= ISP_AE_FOUR_PLANE_MODE_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "enFourPlaneMode should not be larger than 1\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stAECfg.enFourPlaneMode == ISP_AE_FOUR_PLANE_MODE_DISABLE) &&
            (pstStatCfg->stAECfg.stHistConfig.enHistSkipX == ISP_AE_HIST_SKIP_EVERY_PIXEL)) {
            ISP_TRACE(MODULE_DBG_ERR, "u8HistSkipX should not be 0 when not in FourPlaneMode\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.enHistMode >= ISP_AE_MODE_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "enHistMode should not be larger than 1\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.enAverMode >= ISP_AE_MODE_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "enAverMode should not be larger than 1\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.enMaxGainMode >= ISP_AE_MODE_BUTT) {
            ISP_TRACE(MODULE_DBG_ERR, "enMaxGainMode should not be larger than 1\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                if (pstStatCfg->stAECfg.au8Weight[i][j] > 0xF) {
                    ISP_TRACE(MODULE_DBG_ERR, "au8Weight[%d][%d] should not be larger than 0xF!\n", i, j);
                    return ERR_CODE_ISP_ILLEGAL_PARAM;
                }
            }
        }

        if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_DG) {
            ext_system_ae_be_sel_write(ViPipe, ISP_TOP_AE_SELECT_AFTER_DG);
        } else if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_WB) {
            ext_system_ae_be_sel_write(ViPipe, ISP_TOP_AE_SELECT_AFTER_WB);
        } else if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_DRC) {
            ext_system_ae_be_sel_write(ViPipe, ISP_TOP_AE_SELECT_AFTER_DRC);
        } else {
            ISP_TRACE(MODULE_DBG_ERR, "ViPipe:%d Not Support this!\n", ViPipe);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ISP_CHECK_BOOL(pstStatCfg->stAECfg.stCrop.bEnable);

        if (pstStatCfg->stAECfg.stCrop.u16W < AE_MIN_WIDTH) {
            ISP_TRACE(MODULE_DBG_ERR, "u16CropWidth should be lager than 256!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stAECfg.stCrop.u16H < AE_MIN_HEIGHT) {
            ISP_TRACE(MODULE_DBG_ERR, "u16CropHeight should be lager than 120!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stAECfg.stCrop.u16X + pstStatCfg->stAECfg.stCrop.u16W) > u16Width) {
            ISP_TRACE(MODULE_DBG_ERR, "u16X + u16W should not be lager than %d!\n", u16Width);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stAECfg.stCrop.u16Y + pstStatCfg->stAECfg.stCrop.u16H) > u16Height) {
            ISP_TRACE(MODULE_DBG_ERR, "u16Y + u16H should not be lager than %d!\n", u16Height);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ae_fourplanemode_write(ViPipe, pstStatCfg->stAECfg.enFourPlaneMode);
        ext_system_aest_skip_x_write(ViPipe, pstStatCfg->stAECfg.stHistConfig.enHistSkipX);
        ext_system_aest_skip_y_write(ViPipe, pstStatCfg->stAECfg.stHistConfig.enHistSkipY);
        ext_system_aest_offset_x_write(ViPipe, pstStatCfg->stAECfg.stHistConfig.enHistOffsetX);
        ext_system_aest_offset_y_write(ViPipe, pstStatCfg->stAECfg.stHistConfig.enHistOffsetY);
        ext_system_aestmode_write(ViPipe, pstStatCfg->stAECfg.enHistMode);
        ext_system_ae_avermode_write(ViPipe, pstStatCfg->stAECfg.enAverMode);
        ext_system_ae_maxgainmode_write(ViPipe, pstStatCfg->stAECfg.enMaxGainMode);

        ext_system_ae_crop_en_write(ViPipe, pstStatCfg->stAECfg.stCrop.bEnable);
        ext_system_ae_crop_x_write(ViPipe, pstStatCfg->stAECfg.stCrop.u16X);
        ext_system_ae_crop_y_write(ViPipe, pstStatCfg->stAECfg.stCrop.u16Y);
        ext_system_ae_crop_height_write(ViPipe, pstStatCfg->stAECfg.stCrop.u16H);
        ext_system_ae_crop_width_write(ViPipe, pstStatCfg->stAECfg.stCrop.u16W);

        /* set 15*17 weight table */
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                ext_system_ae_weight_table_write(ViPipe, (i * AE_ZONE_COLUMN + j), pstStatCfg->stAECfg.au8Weight[i][j]);
            }
        }
    }

#ifdef CONFIG_ISP_AF_SUPPORT
    GK_U8 u8Shift0, u8I;
    GK_S16 s16G0, s16G1, s16G2;
    GK_FLOAT f32Temp, f32Pl;
    GK_U32 u32Plg, u32Pls;
    const ISP_AF_H_PARAM_S *pstIIR;

    /* AF paramters check */
    /* confg */
    if (pstStatCfg->stFocusCfg.stConfig.u16Hwnd < stIspWorkMode.u8BlockNum) {
        ISP_TRACE(MODULE_DBG_ERR, "The value of AF u16Hwnd should be >= u8BlockNum:%d!\n", stIspWorkMode.u8BlockNum);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.bEnable,         0x1,            "Invalid AF bEnable!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Hwnd,               1,        17, "Invalid AF u16Hwnd!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Vwnd,               1,        15, "Invalid AF u16Vwnd!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Hsize,            256,  u16Width, "Invalid AF u16Hsize!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Vsize, RES_HEIGHT_MIN, u16Height, "Invalid AF u16Vsize!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enPeakMode,      0x1,            "Invalid AF enPeakMode!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enSquMode,       0x1,            "Invalid AF enSquMode!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable,  0x1,            "Invalid AF stCrop.bEnable!\n");

    if (pstStatCfg->stFocusCfg.stConfig.stCrop.u16W < 256) {
        ISP_TRACE(MODULE_DBG_ERR, "u16W should be larger than 256!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stConfig.stCrop.u16H < 120) {
        ISP_TRACE(MODULE_DBG_ERR, "u16H should be larger than 256!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.stCrop.u16X + pstStatCfg->stFocusCfg.stConfig.stCrop.u16W) > u16Width) {
        ISP_TRACE(MODULE_DBG_ERR, "u16X + u16W should NOT be larger than %d!\n", u16Width);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y + pstStatCfg->stFocusCfg.stConfig.stCrop.u16H) > u16Height) {
        ISP_TRACE(MODULE_DBG_ERR, "u16Y + u16H should NOT be larger than %d!\n", u16Height);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stConfig.stCrop.u16W % 8 != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Crop u16W:%d!\n", pstStatCfg->stFocusCfg.stConfig.stCrop.u16W);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stConfig.stCrop.u16H % 2 != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Crop u16H:%d!\n", pstStatCfg->stFocusCfg.stConfig.stCrop.u16H);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enStatisticsPos,                       0x2, "Invalid AF enStatisticsPos!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.GammaValue,    AF_GAMMA_VALUE_MAX, "Invalid AF stRawCfg.GammaValue!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.GammaGainLimit,               0x5, "Invalid AF stRawCfg.GammaGainLimit!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern,                    0x3, "Invalid AF stRawCfg.enPattern!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn,                       0x1, "Invalid AF stPreFltCfg.bEn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength,            0xFFFF, "Invalid AF stPreFltCfg.u16strength!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh,                        0xFF, "Invalid AF u16HighLumaTh!\n");

    /* IIR0 */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand,           0x1,               "Invalid AF stHParam_IIR0.bNarrowBand!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn,     GK_BOOL, 3,    0x1,    "Invalid AF stHParam_IIR0.abIIREn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.u8IIRShift,           0x3F,               "Invalid AF stHParam_IIR0.u8IIRDelay!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0],              0,     255,      "Invalid AF stHParam_IIR0.as16IIRGain[0]\n!");
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(&pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1],  GK_S16, 6, -511, 511, "Invalid AF stHParam_IIR0.as16IIRGain!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift, GK_U16, 4,  0x7,      "Invalid AF stHParam_IIR0.au16IIRShift!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn,            0x1,               "Invalid AF stHParam_IIR0.stLd.bLdEn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow,        0xFF,               "Invalid AF stHParam_IIR0.stLd.u16ThLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow,      0xFF,               "Invalid AF stHParam_IIR0.stLd.u16GainLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow,        0xF,               "Invalid AF stHParam_IIR0.stLd.u16SlpLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh,       0xFF,               "Invalid AF stHParam_IIR0.stLd.u16ThHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh,     0xFF,               "Invalid AF stHParam_IIR0.stLd.u16GainHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh,       0xF,               "Invalid AF stHParam_IIR0.stLd.u16SlpHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th,      0x7FF,               "Invalid AF stHParam_IIR0.stCoring.u16Th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp,       0xF,               "Invalid AF stHParam_IIR0.stCoring.u16Slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt,     0x7FF,               "Invalid AF stHParam_IIR0.stCoring.u16Lmt!\n");
    /* IIR1 */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand,           0x1,               "Invalid AF stHParam_IIR1.bNarrowBand!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn,     GK_BOOL, 3,  0x1,      "Invalid AF stHParam_IIR1.abIIREn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.u8IIRShift,           0x3F,               "Invalid AF stHParam_IIR1.u8IIRDelay\n!");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0],              0,    0xFF,      "Invalid AF stHParam_IIR1.as16IIRGain[0]!\n");
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(&pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1],  GK_S16, 6, -511, 511, "Invalid AF stHParam_IIR1.as16IIRGain!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift, GK_U16, 4,  0x7,      "Invalid AF stHParam_IIR1.au16IIRShift!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn,            0x1,               "Invalid AF stHParam_IIR1.stLd.bLdEn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow,        0xFF,               "Invalid AF stHParam_IIR1.stLd.u16ThLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow,      0xFF,               "Invalid AF stHParam_IIR1.stLd.u16GainLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow,        0xF,               "Invalid AF stHParam_IIR1.stLd.u16SlpLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh,       0xFF,               "Invalid AF stHParam_IIR1.stLd.u16ThHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh,     0xFF,               "Invalid AF stHParam_IIR1.stLd.u16GainHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh,       0xF,               "Invalid AF stHParam_IIR1.stLd.u16SlpHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th,      0x7FF,               "Invalid AF stHParam_IIR1.stCoring.u16Th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp,       0xF,               "Invalid AF stHParam_IIR1.stCoring.u16Slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt,     0x7FF,               "Invalid AF stHParam_IIR1.stCoring.u16Lmt!\n");
    /* FIR0 */
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH,      GK_S16, 5, -31,  31,   "Invalid AF stVParam_FIR0.as16FIRH!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn,         0x1,                "Invalid AF stVParam_FIR0.stLd.bLdEn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow,     0xFF,                "Invalid AF stVParam_FIR0.stLd.u16ThLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow,   0xFF,                "Invalid AF stVParam_FIR0.stLd.u16GainLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow,     0xF,                "Invalid AF stVParam_FIR0.stLd.u16SlpLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh,    0xFF,                "Invalid AF stVParam_FIR0.stLd.u16ThHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh,  0xFF,                "Invalid AF stVParam_FIR0.stLd.u16GainHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh,    0xF,                "Invalid AF stVParam_FIR0.stLd.u16SlpHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th,   0x7FF,                "Invalid AF stVParam_FIR0.stCoring.u16Th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp,    0xF,                "Invalid AF stVParam_FIR0.stCoring.u16Slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt,  0x7FF,                "Invalid AF stVParam_FIR0.stCoring.u16Lmt!\n");
    /* FIR1 */
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH,     GK_S16, 5, -31,  31,  "Invalid AF stVParam_FIR1.as16FIRH!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn,        0x1,               "Invalid AF stVParam_FIR1.stLd.bLdEn!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow,    0xFF,               "Invalid AF stVParam_FIR1.stLd.u16ThLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow,  0xFF,               "Invalid AF stVParam_FIR1.stLd.u16GainLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow,    0xF,               "Invalid AF stVParam_FIR1.stLd.u16SlpLow!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh,   0xFF,               "Invalid AF stVParam_FIR1.stLd.u16ThHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh, 0xFF,               "Invalid AF stVParam_FIR1.stLd.u16GainHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh,   0xF,               "Invalid AF stVParam_FIR1.stLd.u16SlpHigh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th,  0x7FF,               "Invalid AF stVParam_FIR1.stCoring.u16Th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp,   0xF,               "Invalid AF stVParam_FIR1.stCoring.u16Slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt, 0x7FF,               "Invalid AF stVParam_FIR1.stCoring.u16Lmt!\n");
    /* FVPARAM */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY,          0xF,                  "Invalid AF stFVParam.u16AccShiftY!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH, GK_U16,       2,   0xF, "Invalid AF stFVParam.au16AccShiftH!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV, GK_U16,       2,   0xF, "Invalid AF stFVParam.au16AccShiftH!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift,         0xF,                  "Invalid AF stFVParam.u16HlCntShift!\n");

    /* AF STATISTICS CONIFG */
    if (pstStatCfg->stFocusCfg.stConfig.bEnable) {
        ext_system_af_enable_write(ViPipe, 0x3);
    } else {
        ext_system_af_enable_write(ViPipe, 0x0);
    }
    ext_af_iir0_enable0_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0]);
    ext_af_iir0_enable1_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1]);
    ext_af_iir0_enable2_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2]);
    ext_af_iir1_enable0_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0]);
    ext_af_iir1_enable1_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1]);
    ext_af_iir1_enable2_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2]);
    ext_af_iir0_shift_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.u8IIRShift);
    ext_af_iir1_shift_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.u8IIRShift);
    ext_af_peakmode_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.enPeakMode);
    ext_af_squmode_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.enSquMode);
    ext_af_window_hnum_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.u16Hwnd);
    ext_af_window_vnum_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.u16Vwnd);
    ext_af_iir_gain0_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
    ext_af_iir_gain0_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
    ext_af_iir_gain1_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1]);
    ext_af_iir_gain1_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1]);
    ext_af_iir_gain2_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2]);
    ext_af_iir_gain2_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2]);
    ext_af_iir_gain3_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3]);
    ext_af_iir_gain3_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3]);
    ext_af_iir_gain4_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4]);
    ext_af_iir_gain4_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4]);
    ext_af_iir_gain5_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5]);
    ext_af_iir_gain5_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5]);
    ext_af_iir_gain6_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6]);
    ext_af_iir_gain6_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6]);
    ext_af_iir0_shift_group0_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0]);
    ext_af_iir1_shift_group0_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1]);
    ext_af_iir2_shift_group0_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2]);
    ext_af_iir3_shift_group0_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3]);
    ext_af_iir0_shift_group1_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0]);
    ext_af_iir1_shift_group1_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1]);
    ext_af_iir2_shift_group1_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2]);
    ext_af_iir3_shift_group1_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3]);
    ext_af_fir_h_gain0_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0]);
    ext_af_fir_h_gain0_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0]);
    ext_af_fir_h_gain1_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1]);
    ext_af_fir_h_gain1_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1]);
    ext_af_fir_h_gain2_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2]);
    ext_af_fir_h_gain2_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2]);
    ext_af_fir_h_gain3_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3]);
    ext_af_fir_h_gain3_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3]);
    ext_af_fir_h_gain4_group0_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4]);
    ext_af_fir_h_gain4_group1_write(ViPipe, (GK_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4]);

    /* ds */
    ext_af_iir0_ds_enable_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand);
    ext_af_iir1_ds_enable_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand);

    /* PLG and PLS */
    for (u8I = 0; u8I < 2; u8I++) {
        pstIIR = u8I ? &(pstStatCfg->stFocusCfg.stHParam_IIR1) : &(pstStatCfg->stFocusCfg.stHParam_IIR0);

        u8Shift0 = pstIIR->au16IIRShift[0];
        s16G0 = pstIIR->as16IIRGain[0];
        s16G1 = pstIIR->as16IIRGain[1];
        s16G2 = pstIIR->as16IIRGain[2];

        f32Pl = (512.f / DIV_0_TO_1(512 - 2 * s16G1 - s16G2) * s16G0) / (1 << u8Shift0);
        f32Temp = f32Pl;
        f32Temp = MIN2(7 - floor(log(f32Temp) / log(2)), 7);

        u32Pls = (GK_U32)f32Temp;
        u32Plg = (GK_U32)((f32Pl * (1 << u32Pls)) + 0.5);

        if (u8I == 0) {
            ext_af_iir_pls_group0_write(ViPipe, u32Pls);
            ext_af_iir_plg_group0_write(ViPipe, u32Plg);
        } else {
            ext_af_iir_pls_group1_write(ViPipe, u32Pls);
            ext_af_iir_plg_group1_write(ViPipe, u32Plg);
        }
    }

    /* AF crop */
    ext_af_crop_enable_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable);
    ext_af_crop_pos_x_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stCrop.u16X);
    ext_af_crop_pos_y_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y);
    ext_af_crop_hsize_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stCrop.u16W);
    ext_af_crop_vsize_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stCrop.u16H);

    /* AF raw cfg */
    ext_af_pos_sel_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.enStatisticsPos);
    ext_af_rawmode_write(ViPipe, ~(((pstStatCfg->stFocusCfg.stConfig.enStatisticsPos) >> 0x1) & 0x1));
    ext_af_gain_limit_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stRawCfg.GammaGainLimit);
    ext_af_gamma_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stRawCfg.GammaValue);
    ext_af_bayermode_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern);

    /* AF pre median filter */
    ext_af_mean_enable_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn);
    ext_af_mean_thres_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength);

    /* level depend gain */
    ext_af_iir0_ldg_enable_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn);

    ext_af_iir_thre0_low_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow);
    ext_af_iir_thre0gh_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh);
    ext_af_iir_slope0_low_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow);
    ext_af_iir_slope0gh_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh);
    ext_af_iir_gain0_low_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow);
    ext_af_iir_gain0gh_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh);

    ext_af_iir1_ldg_enable_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn);

    ext_af_iir_thre1_low_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow);
    ext_af_iir_thre1gh_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh);
    ext_af_iir_slope1_low_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow);
    ext_af_iir_slope1gh_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh);
    ext_af_iir_gain1_low_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow);
    ext_af_iir_gain1gh_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh);

    ext_af_fir0_ldg_enable_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn);

    ext_af_fir_thre0_low_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow);
    ext_af_fir_thre0gh_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh);
    ext_af_fir_slope0_low_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow);
    ext_af_fir_slope0gh_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh);
    ext_af_fir_gain0_low_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow);
    ext_af_fir_gain0gh_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh);

    ext_af_fir1_ldg_enable_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn);

    ext_af_fir_thre1_low_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow);
    ext_af_fir_thre1gh_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh);
    ext_af_fir_slope1_low_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow);
    ext_af_fir_slope1gh_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh);
    ext_af_fir_gain1_low_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow);
    ext_af_fir_gain1gh_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh);

    /* AF coring */
    ext_af_iir_thre0_coring_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th);
    ext_af_iir_slope0_coring_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp);
    ext_af_iir_peak0_coring_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt);

    ext_af_iir_thre1_coring_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th);
    ext_af_iir_slope1_coring_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp);
    ext_af_iir_peak1_coring_write(ViPipe, pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt);

    ext_af_fir_thre0_coring_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th);
    ext_af_fir_slope0_coring_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp);
    ext_af_fir_peak0_coring_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt);

    ext_af_fir_thre1_coring_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th);
    ext_af_fir_slope1_coring_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp);
    ext_af_fir_peak1_coring_write(ViPipe, pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt);

    /* high luma counter */
    ext_afligh_thre_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh);

    /* AF output shift */
    ext_af_acc_shift0_h_write(ViPipe, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0]);
    ext_af_acc_shift1_h_write(ViPipe, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1]);
    ext_af_acc_shift0_v_write(ViPipe, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0]);
    ext_af_acc_shift1_v_write(ViPipe, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1]);
    ext_af_acc_shift_y_write(ViPipe, pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY);
    ext_af_shift_count_y_write(ViPipe, pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift);

    ext_af_input_hsize_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.u16Hsize);
    ext_af_input_vsize_write(ViPipe, pstStatCfg->stFocusCfg.stConfig.u16Vsize);

    ext_af_set_flag_write(ViPipe, ISP_EXT_AF_SET_FLAG_ENABLE);
#endif

    u32KeyLowbit = (GK_U32)(pstStatCfg->unKey.u64Key & 0xFFFFFFFF);
    u32KeyHighbit = (GK_U32)((pstStatCfg->unKey.u64Key & 0xFFFFFFFF00000000) >> 32);
    ext_system_statistics_ctrl_lowbit_write(ViPipe, u32KeyLowbit);
    ext_system_statistics_ctrlghbit_write(ViPipe, u32KeyHighbit);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetStatisticsConfig(VI_PIPE ViPipe, ISP_STATISTICS_CFG_S *pstStatCfg)
{
    GK_S32 i, j;
    GK_U8  u8Tmp;
    GK_U32 u32KeyLowbit, u32KeyHighbit;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u32KeyLowbit  = ext_system_statistics_ctrl_lowbit_read(ViPipe);
    u32KeyHighbit = ext_system_statistics_ctrlghbit_read(ViPipe);
    pstStatCfg->unKey.u64Key = ((GK_U64)u32KeyHighbit << 32) + u32KeyLowbit;

    /*************************AWB STATISTICS CONIFG ***************************/
    u8Tmp = ext_system_awb_switch_read(ViPipe);
    if (u8Tmp == ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_DG) {
        pstStatCfg->stWBCfg.enAWBSwitch = ISP_AWB_AFTER_DG;
    } else if (u8Tmp == ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER) {
        pstStatCfg->stWBCfg.enAWBSwitch = ISP_AWB_AFTER_Expander;
    } else if (u8Tmp == ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC) {
        pstStatCfg->stWBCfg.enAWBSwitch = ISP_AWB_AFTER_DRC;
    } else {
        pstStatCfg->stWBCfg.enAWBSwitch = ISP_AWB_SWITCH_BUTT;
    }

    pstStatCfg->stWBCfg.u16ZoneRow = ext_system_awb_vnum_read(ViPipe);
    pstStatCfg->stWBCfg.u16ZoneCol = ext_system_awb_hnum_read(ViPipe);
    pstStatCfg->stWBCfg.u16ZoneBin = ext_system_awb_zone_bin_read(ViPipe);
    pstStatCfg->stWBCfg.au16HistBinThresh[0] = ext_system_awbst_bin_thresh0_read(ViPipe);
    pstStatCfg->stWBCfg.au16HistBinThresh[1] = ext_system_awbst_bin_thresh1_read(ViPipe);
    pstStatCfg->stWBCfg.au16HistBinThresh[2] = ext_system_awbst_bin_thresh2_read(ViPipe);
    pstStatCfg->stWBCfg.au16HistBinThresh[3] = ext_system_awbst_bin_thresh3_read(ViPipe);

    pstStatCfg->stWBCfg.u16WhiteLevel = ext_system_awb_white_level_read(ViPipe);
    pstStatCfg->stWBCfg.u16BlackLevel = ext_system_awb_black_level_read(ViPipe);
    pstStatCfg->stWBCfg.u16CrMax = ext_system_awb_cr_ref_max_read(ViPipe);
    pstStatCfg->stWBCfg.u16CrMin = ext_system_awb_cr_ref_min_read(ViPipe);
    pstStatCfg->stWBCfg.u16CbMax = ext_system_awb_cb_ref_max_read(ViPipe);
    pstStatCfg->stWBCfg.u16CbMin = ext_system_awb_cb_ref_min_read(ViPipe);
    pstStatCfg->stWBCfg.stCrop.bEnable = ext_system_awb_crop_en_read(ViPipe);
    pstStatCfg->stWBCfg.stCrop.u16X = ext_system_awb_crop_x_read(ViPipe);
    pstStatCfg->stWBCfg.stCrop.u16Y = ext_system_awb_crop_y_read(ViPipe);
    pstStatCfg->stWBCfg.stCrop.u16H = ext_system_awb_crop_height_read(ViPipe);
    pstStatCfg->stWBCfg.stCrop.u16W = ext_system_awb_crop_width_read(ViPipe);

    /*************************AE STATISTICS CONIFG ***************************/
    u8Tmp = ext_system_ae_be_sel_read(ViPipe);
    if (u8Tmp == ISP_TOP_AE_SELECT_AFTER_DG) {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_DG;
    } else if (u8Tmp == ISP_TOP_AE_SELECT_AFTER_WB) {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_WB;
    } else if (u8Tmp == ISP_TOP_AE_SELECT_AFTER_DRC) {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_DRC;
    } else {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_SWITCH_BUTT;
    }

    u8Tmp = ext_system_ae_fourplanemode_read(ViPipe);
    if (u8Tmp == ISP_AE_FOUR_PLANE_MODE_DISABLE) {
        pstStatCfg->stAECfg.enFourPlaneMode = ISP_AE_FOUR_PLANE_MODE_DISABLE;
    } else if (u8Tmp == ISP_AE_FOUR_PLANE_MODE_ENABLE) {
        pstStatCfg->stAECfg.enFourPlaneMode = ISP_AE_FOUR_PLANE_MODE_ENABLE;
    } else {
        pstStatCfg->stAECfg.enFourPlaneMode = ISP_AE_FOUR_PLANE_MODE_BUTT;
    }

    pstStatCfg->stAECfg.stHistConfig.enHistSkipX = ext_system_aest_skip_x_read(ViPipe);
    pstStatCfg->stAECfg.stHistConfig.enHistSkipY = ext_system_aest_skip_y_read(ViPipe);
    pstStatCfg->stAECfg.stHistConfig.enHistOffsetX = ext_system_aest_offset_x_read(ViPipe);
    pstStatCfg->stAECfg.stHistConfig.enHistOffsetY = ext_system_aest_offset_y_read(ViPipe);
    pstStatCfg->stAECfg.enHistMode = ext_system_aestmode_read(ViPipe);
    pstStatCfg->stAECfg.enAverMode = ext_system_ae_avermode_read(ViPipe);
    pstStatCfg->stAECfg.enMaxGainMode = ext_system_ae_maxgainmode_read(ViPipe);

    pstStatCfg->stAECfg.stCrop.bEnable = ext_system_ae_crop_en_read(ViPipe);
    pstStatCfg->stAECfg.stCrop.u16X = ext_system_ae_crop_x_read(ViPipe);
    pstStatCfg->stAECfg.stCrop.u16Y = ext_system_ae_crop_y_read(ViPipe);
    pstStatCfg->stAECfg.stCrop.u16H = ext_system_ae_crop_height_read(ViPipe);
    pstStatCfg->stAECfg.stCrop.u16W = ext_system_ae_crop_width_read(ViPipe);

    /* set 15*17 weight table */
    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            pstStatCfg->stAECfg.au8Weight[i][j] = ext_system_ae_weight_table_read(ViPipe, (i * AE_ZONE_COLUMN + j));
        }
    }

#ifdef CONFIG_ISP_AF_SUPPORT
    /* AF FE STATISTICS CONIFG START */
    pstStatCfg->stFocusCfg.stConfig.bEnable = (ext_system_af_enable_read(ViPipe) != 0x0) ? 1 : 0;
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0] = ext_af_iir0_enable0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1] = ext_af_iir0_enable1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2] = ext_af_iir0_enable2_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0] = ext_af_iir1_enable0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1] = ext_af_iir1_enable1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2] = ext_af_iir1_enable2_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.u8IIRShift = ext_af_iir0_shift_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.u8IIRShift = ext_af_iir1_shift_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.enPeakMode = ext_af_peakmode_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.enSquMode = ext_af_squmode_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.u16Hwnd = ext_af_window_hnum_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.u16Vwnd = ext_af_window_vnum_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] = ext_af_iir_gain0_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] = ext_af_iir_gain0_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1] = ext_af_iir_gain1_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1] = ext_af_iir_gain1_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2] = ext_af_iir_gain2_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2] = ext_af_iir_gain2_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3] = ext_af_iir_gain3_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3] = ext_af_iir_gain3_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4] = ext_af_iir_gain4_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4] = ext_af_iir_gain4_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5] = ext_af_iir_gain5_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5] = ext_af_iir_gain5_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6] = ext_af_iir_gain6_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6] = ext_af_iir_gain6_group1_read(ViPipe);

    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0] = ext_af_iir0_shift_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1] = ext_af_iir1_shift_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2] = ext_af_iir2_shift_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3] = ext_af_iir3_shift_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0] = ext_af_iir0_shift_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1] = ext_af_iir1_shift_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2] = ext_af_iir2_shift_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3] = ext_af_iir3_shift_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0] = ext_af_fir_h_gain0_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0] = ext_af_fir_h_gain0_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1] = ext_af_fir_h_gain1_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1] = ext_af_fir_h_gain1_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2] = ext_af_fir_h_gain2_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2] = ext_af_fir_h_gain2_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3] = ext_af_fir_h_gain3_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3] = ext_af_fir_h_gain3_group1_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4] = ext_af_fir_h_gain4_group0_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4] = ext_af_fir_h_gain4_group1_read(ViPipe);

    /* ds */
    pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand = ext_af_iir0_ds_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand = ext_af_iir1_ds_enable_read(ViPipe);

    /* AF crop */
    pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable = ext_af_crop_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stCrop.u16X = ext_af_crop_pos_x_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y = ext_af_crop_pos_y_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stCrop.u16W = ext_af_crop_hsize_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stCrop.u16H = ext_af_crop_vsize_read(ViPipe);

    /* AF raw cfg */
    pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ext_af_pos_sel_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stRawCfg.GammaGainLimit = ext_af_gain_limit_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stRawCfg.GammaValue = ext_af_gamma_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern = ext_af_bayermode_read(ViPipe);

    /* AF pre median filter */
    pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn = ext_af_mean_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength = ext_af_mean_thres_read(ViPipe);

    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn       = ext_af_iir0_ldg_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow    = ext_af_iir_thre0_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh   = ext_af_iir_thre0gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow   = ext_af_iir_slope0_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh  = ext_af_iir_slope0gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow  = ext_af_iir_gain0_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh = ext_af_iir_gain0gh_read(ViPipe);

    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn       = ext_af_iir1_ldg_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow    = ext_af_iir_thre1_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh   = ext_af_iir_thre1gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow   = ext_af_iir_slope1_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh  = ext_af_iir_slope1gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow  = ext_af_iir_gain1_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh = ext_af_iir_gain1gh_read(ViPipe);

    /* level depend gain */
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn       = ext_af_fir0_ldg_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow    = ext_af_fir_thre0_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh   = ext_af_fir_thre0gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow   = ext_af_fir_slope0_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh  = ext_af_fir_slope0gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow  = ext_af_fir_gain0_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh = ext_af_fir_gain0gh_read(ViPipe);

    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn       = ext_af_fir1_ldg_enable_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow    = ext_af_fir_thre1_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh   = ext_af_fir_thre1gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow   = ext_af_fir_slope1_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh  = ext_af_fir_slope1gh_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow  = ext_af_fir_gain1_low_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh = ext_af_fir_gain1gh_read(ViPipe);

    /* AF coring */
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th  = ext_af_iir_thre0_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp = ext_af_iir_slope0_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt = ext_af_iir_peak0_coring_read(ViPipe);

    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th  = ext_af_iir_thre1_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp = ext_af_iir_slope1_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt = ext_af_iir_peak1_coring_read(ViPipe);

    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th  = ext_af_fir_thre0_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp = ext_af_fir_slope0_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt = ext_af_fir_peak0_coring_read(ViPipe);

    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th  = ext_af_fir_thre1_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp = ext_af_fir_slope1_coring_read(ViPipe);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt = ext_af_fir_peak1_coring_read(ViPipe);

    /* high luma counter */
    pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh = ext_afligh_thre_read(ViPipe);

    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0] = ext_af_acc_shift0_h_read(ViPipe);
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1] = ext_af_acc_shift1_h_read(ViPipe);
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0] = ext_af_acc_shift0_v_read(ViPipe);
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1] = ext_af_acc_shift1_v_read(ViPipe);
    pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY = ext_af_acc_shift_y_read(ViPipe);
    pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift = ext_af_shift_count_y_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.u16Hsize = ext_af_input_hsize_read(ViPipe);
    pstStatCfg->stFocusCfg.stConfig.u16Vsize = ext_af_input_vsize_read(ViPipe);
#endif

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetGammaAttr(VI_PIPE ViPipe, const ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    GK_U32 i = 0;
    GK_U32 u32WDRMode;

    const GK_U16 *pu16GammaLut;
    static GK_U16 au16Gamma033[GAMMA_NODE_NUM] = {
        0, 21, 42, 64, 85, 106, 128, 149, 171, 192, 214, 235, 257, 279, 300, 322, 344, 366, 388, 409, 431, 453, 475, 496, 518, 540, 562, 583, 605, 627, 648, 670,
        691, 712, 734, 755, 776, 797, 818, 839, 860, 881, 902, 922, 943, 963, 984, 1004, 1024, 1044, 1064, 1084, 1104, 1124, 1144, 1164, 1182, 1198, 1213, 1227, 1241, 1256, 1270, 1285,
        1299, 1313, 1328, 1342, 1356, 1370, 1384, 1398, 1412, 1426, 1440, 1453, 1467, 1481, 1494, 1508, 1521, 1535, 1548, 1562, 1575, 1588, 1602, 1615, 1628, 1641, 1654, 1667, 1680, 1693, 1706, 1719,
        1732, 1745, 1758, 1771, 1784, 1797, 1810, 1823, 1836, 1849, 1862, 1875, 1888, 1901, 1914, 1926, 1939, 1952, 1965, 1978, 1991, 2004, 2017, 2030, 2043, 2056, 2069, 2082, 2095, 2109, 2123, 2137,
        2148, 2157, 2164, 2171, 2178, 2185, 2193, 2200, 2208, 2216, 2223, 2231, 2239, 2247, 2255, 2262, 2270, 2278, 2286, 2293, 2301, 2309, 2316, 2324, 2332, 2340, 2348, 2356, 2364, 2372, 2380, 2388,
        2396, 2404, 2412, 2419, 2427, 2435, 2443, 2451, 2459, 2467, 2475, 2483, 2491, 2499, 2507, 2515, 2523, 2531, 2539, 2546, 2554, 2562, 2570, 2578, 2586, 2594, 2602, 2609, 2617, 2625, 2633, 2640,
        2648, 2656, 2664, 2671, 2679, 2687, 2695, 2702, 2710, 2718, 2725, 2733, 2740, 2748, 2755, 2763, 2770, 2778, 2785, 2793, 2800, 2807, 2815, 2822, 2829, 2836, 2844, 2851, 2858, 2865, 2872, 2879,
        2886, 2893, 2900, 2906, 2913, 2920, 2927, 2934, 2941, 2948, 2954, 2961, 2967, 2974, 2980, 2987, 2993, 2999, 3006, 3012, 3018, 3024, 3030, 3036, 3042, 3048, 3054, 3059, 3065, 3071, 3077, 3082,
        3088, 3094, 3099, 3105, 3110, 3115, 3121, 3126, 3131, 3136, 3142, 3147, 3152, 3157, 3162, 3167, 3172, 3177, 3182, 3187, 3192, 3197, 3202, 3206, 3211, 3216, 3220, 3225, 3229, 3234, 3238, 3243,
        3247, 3252, 3256, 3261, 3265, 3269, 3274, 3278, 3282, 3286, 3291, 3295, 3299, 3303, 3307, 3311, 3315, 3319, 3323, 3327, 3331, 3335, 3339, 3342, 3346, 3350, 3354, 3357, 3361, 3365, 3368, 3371,
        3375, 3379, 3383, 3386, 3390, 3394, 3397, 3401, 3404, 3407, 3411, 3414, 3417, 3420, 3424, 3427, 3430, 3433, 3437, 3440, 3443, 3446, 3450, 3453, 3456, 3459, 3462, 3465, 3468, 3471, 3474, 3477,
        3480, 3483, 3486, 3489, 3492, 3495, 3498, 3501, 3504, 3507, 3510, 3512, 3515, 3518, 3521, 3523, 3526, 3529, 3532, 3534, 3537, 3540, 3543, 3545, 3548, 3551, 3554, 3556, 3559, 3562, 3564, 3567,
        3569, 3572, 3574, 3577, 3579, 3582, 3584, 3587, 3589, 3591, 3594, 3596, 3598, 3600, 3603, 3605, 3607, 3609, 3612, 3614, 3616, 3618, 3620, 3622, 3624, 3626, 3628, 3629, 3631, 3633, 3635, 3637,
        3639, 3641, 3643, 3644, 3646, 3648, 3649, 3650, 3652, 3654, 3656, 3657, 3659, 3661, 3662, 3664, 3665, 3667, 3668, 3670, 3671, 3672, 3674, 3675, 3676, 3677, 3678, 3680, 3681, 3682, 3684, 3686,
        3687, 3688, 3690, 3691, 3692, 3693, 3694, 3695, 3696, 3697, 3698, 3700, 3701, 3702, 3704, 3705, 3706, 3707, 3708, 3709, 3710, 3711, 3713, 3714, 3715, 3716, 3717, 3718, 3719, 3720, 3721, 3722,
        3723, 3724, 3726, 3727, 3728, 3729, 3730, 3731, 3732, 3733, 3734, 3735, 3736, 3737, 3739, 3740, 3741, 3742, 3743, 3744, 3745, 3746, 3748, 3749, 3750, 3751, 3752, 3753, 3754, 3755, 3756, 3758,
        3759, 3760, 3762, 3763, 3764, 3765, 3767, 3768, 3769, 3770, 3771, 3772, 3773, 3774, 3776, 3777, 3778, 3779, 3780, 3781, 3782, 3783, 3785, 3786, 3787, 3788, 3789, 3790, 3791, 3792, 3793, 3794,
        3795, 3796, 3797, 3798, 3799, 3800, 3801, 3802, 3803, 3804, 3805, 3806, 3807, 3808, 3809, 3810, 3811, 3812, 3813, 3814, 3815, 3816, 3817, 3818, 3819, 3820, 3821, 3822, 3823, 3824, 3825, 3825,
        3826, 3827, 3828, 3829, 3830, 3831, 3832, 3833, 3834, 3835, 3836, 3836, 3837, 3838, 3839, 3840, 3841, 3842, 3843, 3843, 3844, 3845, 3845, 3846, 3847, 3848, 3849, 3850, 3851, 3852, 3853, 3853,
        3854, 3855, 3855, 3856, 3857, 3858, 3859, 3860, 3861, 3862, 3863, 3863, 3864, 3865, 3866, 3866, 3867, 3868, 3868, 3869, 3870, 3871, 3872, 3873, 3874, 3875, 3876, 3876, 3877, 3878, 3879, 3879,
        3880, 3881, 3882, 3882, 3883, 3884, 3885, 3885, 3886, 3887, 3888, 3888, 3889, 3890, 3891, 3891, 3892, 3893, 3894, 3894, 3895, 3896, 3897, 3897, 3898, 3899, 3899, 3899, 3900, 3901, 3901, 3902,
        3903, 3904, 3905, 3905, 3906, 3907, 3907, 3907, 3908, 3909, 3910, 3910, 3911, 3912, 3912, 3912, 3913, 3914, 3915, 3915, 3916, 3917, 3917, 3917, 3918, 3919, 3920, 3920, 3921, 3922, 3922, 3923,
        3923, 3923, 3924, 3924, 3925, 3926, 3927, 3927, 3928, 3929, 3929, 3930, 3930, 3930, 3931, 3931, 3932, 3933, 3934, 3934, 3935, 3936, 3936, 3937, 3937, 3937, 3938, 3938, 3939, 3940, 3941, 3941,
        3942, 3943, 3943, 3944, 3944, 3944, 3945, 3945, 3946, 3947, 3948, 3948, 3949, 3950, 3950, 3950, 3951, 3952, 3953, 3953, 3954, 3955, 3955, 3955, 3956, 3957, 3958, 3958, 3959, 3960, 3960, 3960,
        3961, 3962, 3963, 3963, 3964, 3965, 3965, 3965, 3966, 3967, 3968, 3968, 3969, 3970, 3970, 3970, 3971, 3972, 3973, 3973, 3974, 3975, 3975, 3975, 3976, 3977, 3977, 3978, 3979, 3980, 3981, 3981,
        3982, 3983, 3983, 3983, 3984, 3985, 3985, 3986, 3987, 3988, 3989, 3989, 3990, 3991, 3991, 3991, 3992, 3993, 3994, 3994, 3995, 3996, 3996, 3996, 3997, 3998, 3998, 3999, 4000, 4001, 4002, 4002,
        4003, 4004, 4004, 4004, 4005, 4006, 4007, 4007, 4008, 4009, 4009, 4009, 4010, 4011, 4012, 4012, 4013, 4014, 4014, 4014, 4015, 4016, 4017, 4017, 4018, 4019, 4019, 4019, 4020, 4021, 4022, 4022,
        4023, 4024, 4024, 4024, 4025, 4026, 4027, 4027, 4028, 4029, 4029, 4030, 4030, 4030, 4031, 4031, 4032, 4033, 4034, 4034, 4035, 4036, 4036, 4037, 4037, 4038, 4038, 4039, 4039, 4040, 4040, 4041,
        4041, 4042, 4042, 4043, 4043, 4044, 4044, 4045, 4045, 4046, 4046, 4047, 4047, 4048, 4048, 4049, 4049, 4050, 4050, 4051, 4051, 4052, 4052, 4053, 4053, 4054, 4054, 4055, 4055, 4055, 4056, 4056,
        4056, 4056, 4057, 4057, 4058, 4059, 4059, 4060, 4060, 4060, 4061, 4061, 4061, 4061, 4062, 4062, 4063, 4064, 4064, 4065, 4065, 4065, 4066, 4066, 4066, 4066, 4067, 4067, 4068, 4069, 4069, 4070,
        4070, 4070, 4071, 4071, 4071, 4071, 4072, 4073, 4073, 4073, 4074, 4074, 4074, 4074, 4075, 4076, 4076, 4076, 4077, 4077, 4077, 4077, 4078, 4078, 4079, 4080, 4080, 4081, 4081, 4081, 4082, 4082,
        4082, 4082, 4083, 4084, 4084, 4084, 4085, 4085, 4085, 4085, 4086, 4087, 4087, 4087, 4088, 4088, 4088, 4088, 4089, 4089, 4090, 4091, 4091, 4092, 4092, 4092, 4093, 4093, 4093, 4093, 4094, 4094,
        4095

    };

    static GK_U16 au16GammaDef[GAMMA_NODE_NUM] = {
        0,   30,  60,  90,  120, 145, 170, 195, 220, 243, 265, 288, 310, 330, 350, 370, 390, 410, 430, 450, 470, 488, 505, 523, 540, 558, 575, 593, 610, 625, 640, 655, 670, 685, 700,
        715, 730, 744, 758, 772, 786, 800, 814, 828, 842, 855, 868, 881, 894, 907, 919, 932, 944, 957, 969, 982, 994, 1008,    1022,    1036,    1050,   1062,    1073,    1085,
        1096,    1107,    1117,    1128,    1138,    1148,    1158,    1168,    1178,    1188,    1198,    1208,    1218,    1227,    1236,    1245,    1254,    1261,
        1267,    1274,    1280,    1289,    1297,    1306,    1314,    1322,    1330,    1338,    1346,    1354,    1362,    1370,    1378,    1386,    1393,    1401,
        1408,    1416,    1423,    1431,    1438,    1445,    1453,    1460,    1467,    1474,    1480,    1487,    1493,    1500,    1506,    1513,    1519,    1525,
        1531,    1537,    1543,    1549,    1556,    1562,    1568,    1574,    1580,    1586,    1592,    1598,    1604,    1609,    1615,    1621,    1627,    1632,
        1638,    1644,    1650,    1655,    1661,    1667,    1672,    1678,    1683,    1689,    1694,    1700,    1705,    1710,    1716,    1721,    1726,    1732,
        1737,    1743,    1748,    1753,    1759,    1764,    1769,    1774,    1779,    1784,    1789,    1794,    1800,    1805,    1810,    1815,    1820,    1825,
        1830,    1835,    1840,    1844,    1849,    1854,    1859,    1864,    1869,    1874,    1879,    1883,    1888,    1893,    1898,    1902,    1907,    1912,
        1917,    1921,    1926,    1931,    1936,    1940,    1945,    1950,    1954,    1959,    1963,    1968,    1972,    1977,    1981,    1986,    1990,    1995,
        1999,    2004,    2008,    2013,    2017,    2021,    2026,    2030,    2034,    2039,    2043,    2048,    2052,    2056,    2061,    2065,    2069,    2073,
        2078,    2082,    2086,    2090,    2094,    2098,    2102,    2106,    2111,    2115,    2119,    2123,    2128,    2132,    2136,    2140,    2144,    2148,
        2152,    2156,    2160,    2164,    2168,    2172,    2176,    2180,    2184,    2188,    2192,    2196,    2200,    2204,    2208,    2212,    2216,    2220,
        2224,    2227,    2231,    2235,    2239,    2243,    2247,    2251,    2255,    2258,    2262,    2266,    2270,    2273,    2277,    2281,    2285,    2288,
        2292,    2296,    2300,    2303,    2307,    2311,    2315,    2318,    2322,    2326,    2330,    2333,    2337,    2341,    2344,    2348,    2351,    2355,
        2359,    2362,    2366,    2370,    2373,    2377,    2380,    2384,    2387,    2391,    2394,    2398,    2401,    2405,    2408,    2412,    2415,    2419,
        2422,    2426,    2429,    2433,    2436,    2440,    2443,    2447,    2450,    2454,    2457,    2461,    2464,    2467,    2471,    2474,    2477,    2481,
        2484,    2488,    2491,    2494,    2498,    2501,    2504,    2508,    2511,    2515,    2518,    2521,    2525,    2528,    2531,    2534,    2538,    2541,
        2544,    2547,    2551,    2554,    2557,    2560,    2564,    2567,    2570,    2573,    2577,    2580,    2583,    2586,    2590,    2593,    2596,    2599,
        2603,    2606,    2609,    2612,    2615,    2618,    2621,    2624,    2628,    2631,    2634,    2637,    2640,    2643,    2646,    2649,    2653,    2656,
        2659,    2662,    2665,    2668,    2671,    2674,    2677,    2680,    2683,    2686,    2690,    2693,    2696,    2699,    2702,    2705,    2708,    2711,
        2714,    2717,    2720,    2723,    2726,    2729,    2732,    2735,    2738,    2741,    2744,    2747,    2750,    2753,    2756,    2759,    2762,    2764,
        2767,    2770,    2773,    2776,    2779,    2782,    2785,    2788,    2791,    2794,    2797,    2799,    2802,    2805,    2808,    2811,    2814,    2817,
        2820,    2822,    2825,    2828,    2831,    2834,    2837,    2840,    2843,    2845,    2848,    2851,    2854,    2856,    2859,    2862,    2865,    2868,
        2871,    2874,    2877,    2879,    2882,    2885,    2888,    2890,    2893,    2896,    2899,    2901,    2904,    2907,    2910,    2912,    2915,    2918,
        2921,    2923,    2926,    2929,    2932,    2934,    2937,    2940,    2943,    2945,    2948,    2951,    2954,    2956,    2959,    2962,    2964,    2967,
        2969,    2972,    2975,    2977,    2980,    2983,    2986,    2988,    2991,    2994,    2996,    2999,    3001,    3004,    3007,    3009,    3012,    3015,
        3018,    3020,    3023,    3026,    3028,    3031,    3033,    3036,    3038,    3041,    3043,    3046,    3049,    3051,    3054,    3057,    3059,    3062,
        3064,    3067,    3069,    3072,    3074,    3077,    3080,    3082,    3085,    3088,    3090,    3093,    3095,    3098,    3100,    3103,    3105,    3108,
        3110,    3113,    3115,    3118,    3120,    3123,    3125,    3128,    3130,    3133,    3135,    3138,    3140,    3143,    3145,    3148,    3150,    3153,
        3155,    3158,    3160,    3163,    3165,    3168,    3170,    3173,    3175,    3178,    3180,    3183,    3185,    3187,    3190,    3192,    3194,    3197,
        3199,    3202,    3204,    3207,    3209,    3212,    3214,    3217,    3219,    3222,    3224,    3226,    3229,    3231,    3233,    3236,    3238,    3241,
        3243,    3245,    3248,    3250,    3252,    3255,    3257,    3260,    3262,    3264,    3267,    3269,    3271,    3274,    3276,    3279,    3281,    3283,
        3286,    3288,    3290,    3293,    3295,    3298,    3300,    3302,    3305,    3307,    3309,    3311,    3314,    3316,    3318,    3320,    3323,    3325,
        3327,    3330,    3332,    3335,    3337,    3339,    3342,    3344,    3346,    3348,    3351,    3353,    3355,    3357,    3360,    3362,    3364,    3366,
        3369,    3371,    3373,    3375,    3378,    3380,    3382,    3384,    3387,    3389,    3391,    3393,    3396,    3398,    3400,    3402,    3405,    3407,
        3409,    3411,    3414,    3416,    3418,    3420,    3423,    3425,    3427,    3429,    3432,    3434,    3436,    3438,    3441,    3443,    3445,    3447,
        3450,    3452,    3454,    3456,    3459,    3461,    3463,    3465,    3467,    3469,    3471,    3473,    3476,    3478,    3480,    3482,    3485,    3487,
        3489,    3491,    3494,    3496,    3498,    3500,    3502,    3504,    3506,    3508,    3511,    3513,    3515,    3517,    3519,    3521,    3523,    3525,
        3528,    3530,    3532,    3534,    3536,    3538,    3540,    3542,    3545,    3547,    3549,    3551,    3553,    3555,    3557,    3559,    3562,    3564,
        3566,    3568,    3570,    3572,    3574,    3576,    3579,    3581,    3583,    3585,    3587,    3589,    3591,    3593,    3596,    3598,    3600,    3602,
        3604,    3606,    3608,    3610,    3612,    3614,    3616,    3618,    3620,    3622,    3624,    3626,    3629,    3631,    3633,    3635,    3637,    3639,
        3641,    3643,    3645,    3647,    3649,    3651,    3653,    3655,    3657,    3659,    3661,    3663,    3665,    3667,    3670,    3672,    3674,    3676,
        3678,    3680,    3682,    3684,    3686,    3688,    3690,    3692,    3694,    3696,    3698,    3700,    3702,    3704,    3706,    3708,    3710,    3712,
        3714,    3716,    3718,    3720,    3722,    3724,    3726,    3728,    3730,    3732,    3734,    3736,    3738,    3740,    3742,    3744,    3746,    3748,
        3750,    3752,    3754,    3756,    3758,    3760,    3762,    3764,    3766,    3767,    3769,    3771,    3773,    3775,    3777,    3779,    3781,    3783,
        3785,    3787,    3789,    3791,    3793,    3795,    3797,    3799,    3801,    3803,    3805,    3806,    3808,    3810,    3812,    3814,    3816,    3818,
        3820,    3822,    3824,    3826,    3828,    3830,    3832,    3834,    3836,    3837,    3839,    3841,    3843,    3845,    3847,    3849,    3851,    3853,
        3855,    3857,    3859,    3860,    3862,    3864,    3866,    3868,    3870,    3872,    3874,    3875,    3877,    3879,    3881,    3883,    3885,    3887,
        3889,    3890,    3892,    3894,    3896,    3898,    3900,    3902,    3904,    3905,    3907,    3909,    3911,    3913,    3915,    3917,    3919,    3920,
        3922,    3924,    3926,    3928,    3930,    3932,    3934,    3935,    3937,    3939,    3941,    3943,    3945,    3947,    3949,    3950,    3952,    3954,
        3956,    3957,    3959,    3961,    3963,    3965,    3967,    3969,    3971,    3972,    3974,    3976,    3978,    3979,    3981,    3983,    3985,    3987,
        3989,    3991,    3993,    3994,    3996,    3998,    4000,    4001,    4003,    4005,    4007,    4008,    4010,    4012,    4014,    4016,    4018,    4020,
        4022,    4023,    4025,    4027,    4029,    4030,    4032,    4034,    4036,    4037,    4039,    4041,    4043,    4044,    4046,    4048,    4050,    4052,
        4054,    4056,    4058,    4059,    4061,    4063,    4065,    4066,    4068,    4070,    4072,    4073,    4075,    4077,    4079,    4080,    4082,    4084,
        4086,    4087,    4089,    4091,    4092,    4094,    4095

    };
    static GK_U16 au16GammasRGB[GAMMA_NODE_NUM] = {
        0,   18,  36,  54,  72,  90,  108, 126, 144, 162, 180, 198, 216, 234, 252, 270, 288, 306, 324, 343, 360, 377, 394, 410, 426, 441, 456, 471, 486, 500, 514, 527,
        541, 554, 567, 580, 592, 605, 617, 629, 641, 652, 664, 675, 686, 698, 709, 719, 730, 741, 751, 761, 772, 782, 792, 802, 812, 821, 831, 841, 850, 859, 869, 878,
        887, 896, 905, 914, 923, 931, 940, 949, 957, 966, 974, 983, 991, 999, 1007, 1015, 1024, 1032, 1039, 1047, 1055, 1063, 1071, 1078, 1086, 1094, 1101, 1109, 1116, 1124, 1131, 1138,
        1146, 1153, 1160, 1167, 1174, 1182, 1189, 1196, 1203, 1210, 1216, 1223, 1230, 1237, 1244, 1250, 1257, 1264, 1270, 1277, 1284, 1290, 1297, 1303, 1310, 1316, 1322, 1329, 1335, 1341, 1348, 1354,
        1360, 1366, 1372, 1379, 1385, 1391, 1397, 1403, 1409, 1415, 1421, 1427, 1433, 1439, 1444, 1450, 1456, 1462, 1468, 1474, 1479, 1485, 1491, 1496, 1502, 1508, 1513, 1519, 1524, 1530, 1536, 1541,
        1547, 1552, 1557, 1563, 1568, 1574, 1579, 1585, 1590, 1595, 1601, 1606, 1611, 1616, 1622, 1627, 1632, 1637, 1642, 1648, 1653, 1658, 1663, 1668, 1673, 1678, 1683, 1688, 1693, 1698, 1703, 1708,
        1713, 1718, 1723, 1728, 1733, 1738, 1743, 1748, 1753, 1758, 1762, 1767, 1772, 1777, 1782, 1786, 1791, 1796, 1801, 1805, 1810, 1815, 1819, 1824, 1829, 1833, 1838, 1843, 1847, 1852, 1857, 1861,
        1866, 1870, 1875, 1879, 1884, 1888, 1893, 1897, 1902, 1906, 1911, 1915, 1920, 1924, 1928, 1933, 1937, 1942, 1946, 1950, 1955, 1959, 1963, 1968, 1972, 1976, 1981, 1985, 1989, 1994, 1998, 2002,
        2006, 2011, 2015, 2019, 2023, 2027, 2032, 2036, 2040, 2044, 2048, 2052, 2057, 2061, 2065, 2069, 2073, 2077, 2081, 2085, 2089, 2093, 2097, 2102, 2106, 2110, 2114, 2118, 2122, 2126, 2130, 2134,
        2138, 2142, 2146, 2149, 2153, 2157, 2161, 2165, 2169, 2173, 2177, 2181, 2185, 2189, 2192, 2196, 2200, 2204, 2208, 2212, 2216, 2219, 2223, 2227, 2231, 2235, 2238, 2242, 2246, 2250, 2254, 2257,
        2261, 2265, 2269, 2272, 2276, 2280, 2283, 2287, 2291, 2295, 2298, 2302, 2306, 2309, 2313, 2317, 2320, 2324, 2328, 2331, 2335, 2338, 2342, 2346, 2349, 2353, 2356, 2360, 2364, 2367, 2371, 2374,
        2378, 2381, 2385, 2389, 2392, 2396, 2399, 2403, 2406, 2410, 2413, 2417, 2420, 2424, 2427, 2431, 2434, 2438, 2441, 2445, 2448, 2451, 2455, 2458, 2462, 2465, 2469, 2472, 2475, 2479, 2482, 2486,
        2489, 2492, 2496, 2499, 2503, 2506, 2509, 2513, 2516, 2519, 2523, 2526, 2529, 2533, 2536, 2539, 2543, 2546, 2549, 2553, 2556, 2559, 2563, 2566, 2569, 2572, 2576, 2579, 2582, 2585, 2589, 2592,
        2595, 2598, 2602, 2605, 2608, 2611, 2615, 2618, 2621, 2624, 2627, 2631, 2634, 2637, 2640, 2643, 2647, 2650, 2653, 2656, 2659, 2662, 2666, 2669, 2672, 2675, 2678, 2681, 2684, 2688, 2691, 2694,
        2697, 2700, 2703, 2706, 2709, 2712, 2716, 2719, 2722, 2725, 2728, 2731, 2734, 2737, 2740, 2743, 2746, 2749, 2752, 2755, 2759, 2762, 2765, 2768, 2771, 2774, 2777, 2780, 2783, 2786, 2789, 2792,
        2795, 2798, 2801, 2804, 2807, 2810, 2813, 2816, 2819, 2822, 2825, 2828, 2831, 2833, 2836, 2839, 2842, 2845, 2848, 2851, 2854, 2857, 2860, 2863, 2866, 2869, 2872, 2875, 2877, 2880, 2883, 2886,
        2889, 2892, 2895, 2898, 2901, 2904, 2906, 2909, 2912, 2915, 2918, 2921, 2924, 2926, 2929, 2932, 2935, 2938, 2941, 2944, 2946, 2949, 2952, 2955, 2958, 2961, 2963, 2966, 2969, 2972, 2975, 2977,
        2980, 2983, 2986, 2989, 2991, 2994, 2997, 3000, 3003, 3005, 3008, 3011, 3014, 3016, 3019, 3022, 3025, 3027, 3030, 3033, 3036, 3038, 3041, 3044, 3047, 3049, 3052, 3055, 3058, 3060, 3063, 3066,
        3068, 3071, 3074, 3077, 3079, 3082, 3085, 3087, 3090, 3093, 3095, 3098, 3101, 3103, 3106, 3109, 3112, 3114, 3117, 3120, 3122, 3125, 3128, 3130, 3133, 3135, 3138, 3141, 3143, 3146, 3149, 3151,
        3154, 3157, 3159, 3162, 3164, 3167, 3170, 3172, 3175, 3178, 3180, 3183, 3185, 3188, 3191, 3193, 3196, 3198, 3201, 3204, 3206, 3209, 3211, 3214, 3217, 3219, 3222, 3224, 3227, 3229, 3232, 3235,
        3237, 3240, 3242, 3245, 3247, 3250, 3252, 3255, 3257, 3260, 3263, 3265, 3268, 3270, 3273, 3275, 3278, 3280, 3283, 3285, 3288, 3290, 3293, 3295, 3298, 3300, 3303, 3305, 3308, 3310, 3313, 3315,
        3318, 3320, 3323, 3325, 3328, 3330, 3333, 3335, 3338, 3340, 3343, 3345, 3348, 3350, 3353, 3355, 3358, 3360, 3362, 3365, 3367, 3370, 3372, 3375, 3377, 3380, 3382, 3385, 3387, 3389, 3392, 3394,
        3397, 3399, 3402, 3404, 3406, 3409, 3411, 3414, 3416, 3418, 3421, 3423, 3426, 3428, 3431, 3433, 3435, 3438, 3440, 3443, 3445, 3447, 3450, 3452, 3454, 3457, 3459, 3462, 3464, 3466, 3469, 3471,
        3474, 3476, 3478, 3481, 3483, 3485, 3488, 3490, 3492, 3495, 3497, 3500, 3502, 3504, 3507, 3509, 3511, 3514, 3516, 3518, 3521, 3523, 3525, 3528, 3530, 3532, 3535, 3537, 3539, 3542, 3544, 3546,
        3549, 3551, 3553, 3555, 3558, 3560, 3562, 3565, 3567, 3569, 3572, 3574, 3576, 3579, 3581, 3583, 3585, 3588, 3590, 3592, 3595, 3597, 3599, 3601, 3604, 3606, 3608, 3610, 3613, 3615, 3617, 3620,
        3622, 3624, 3626, 3629, 3631, 3633, 3635, 3638, 3640, 3642, 3644, 3647, 3649, 3651, 3653, 3656, 3658, 3660, 3662, 3665, 3667, 3669, 3671, 3674, 3676, 3678, 3680, 3682, 3685, 3687, 3689, 3691,
        3694, 3696, 3698, 3700, 3702, 3705, 3707, 3709, 3711, 3713, 3716, 3718, 3720, 3722, 3724, 3727, 3729, 3731, 3733, 3735, 3738, 3740, 3742, 3744, 3746, 3749, 3751, 3753, 3755, 3757, 3759, 3762,
        3764, 3766, 3768, 3770, 3772, 3775, 3777, 3779, 3781, 3783, 3785, 3788, 3790, 3792, 3794, 3796, 3798, 3800, 3803, 3805, 3807, 3809, 3811, 3813, 3815, 3818, 3820, 3822, 3824, 3826, 3828, 3830,
        3833, 3835, 3837, 3839, 3841, 3843, 3845, 3847, 3850, 3852, 3854, 3856, 3858, 3860, 3862, 3864, 3866, 3869, 3871, 3873, 3875, 3877, 3879, 3881, 3883, 3885, 3887, 3890, 3892, 3894, 3896, 3898,
        3900, 3902, 3904, 3906, 3908, 3910, 3912, 3915, 3917, 3919, 3921, 3923, 3925, 3927, 3929, 3931, 3933, 3935, 3937, 3939, 3942, 3944, 3946, 3948, 3950, 3952, 3954, 3956, 3958, 3960, 3962, 3964,
        3966, 3968, 3970, 3972, 3974, 3976, 3978, 3980, 3983, 3985, 3987, 3989, 3991, 3993, 3995, 3997, 3999, 4001, 4003, 4005, 4007, 4009, 4011, 4013, 4015, 4017, 4019, 4021, 4023, 4025, 4027, 4029,
        4031, 4033, 4035, 4037, 4039, 4041, 4043, 4045, 4047, 4049, 4051, 4053, 4055, 4057, 4059, 4061, 4063, 4065, 4067, 4069, 4071, 4073, 4075, 4077, 4079, 4081, 4083, 4085, 4087, 4089, 4091, 4093,
        4095
    };

    static GK_U16 au16GammaDefWDR[GAMMA_NODE_NUM] = {
        0,      3,      6,      8,      11,     14,     17,     20,     23,     26,     29,     32,     35,     38,     41,     44,     47,     50,     53,     56,     59,     62,     65,     68,
        71,     74,     77,     79,     82,     85,     88,     91,     94,     97,     100,    103,    106,    109,    112,    114,    117,    120,    123,    126,    129,    132,    135,    138,
        141,    144,    147,    149,    152,    155,    158,    161,    164,    167,    170,    172,    175,    178,    181,    184,    187,    190,    193,    195,    198,    201,    204,    207,
        210,    213,    216,    218,    221,    224,    227,    230,    233,    236,    239,    241,    244,    247,    250,    253,    256,    259,    262,    264,    267,    270,    273,    276,
        279,    282,    285,    287,    290,    293,    296,    299,    302,    305,    308,    310,    313,    316,    319,    322,    325,    328,    331,    333,    336,    339,    342,    345,
        348,    351,    354,    356,    359,    362,    365,    368,    371,    374,    377,    380,    383,    386,    389,    391,    394,    397,    400,    403,    406,    409,    412,    415,
        418,    421,    424,    426,    429,    432,    435,    438,    441,    444,    447,    450,    453,    456,    459,    462,    465,    468,    471,    474,    477,    480,    483,    486,
        489,    492,    495,    498,    501,    504,    507,    510,    513,    516,    519,    522,    525,    528,    531,    534,    537,    540,    543,    546,    549,    552,    555,    558,
        561,    564,    568,    571,    574,    577,    580,    583,    586,    589,    592,    595,    598,    601,    605,    608,    611,    614,    617,    620,    623,    626,    630,    633,
        636,    639,    643,    646,    649,    652,    655,    658,    661,    664,    668,    671,    674,    677,    681,    684,    687,    690,    694,    697,    700,    703,    707,    710,
        713,    716,    720,    723,    726,    730,    733,    737,    740,    743,    747,    750,    753,    757,    760,    764,    767,    770,    774,    777,    780,    784,    787,    791,
        794,    797,    801,    804,    807,    811,    814,    818,    821,    825,    828,    832,    835,    838,    842,    845,    848,    852,    855,    859,    862,    866,     869,   873,
        876,    880,    883,    887,    890,    894,    897,    901,    904,    908,    911,    915,    918,    922,    925,    929,    932,    936,    939,    943,    946,    950,    954,    957,
        961,    965,    968,    972,    975,    979,    982,    986,    989,    993,    997,    1000,   1004,   1008,   1011,   1015,   1018,   1022,   1026,   1029,   1033,   1037,   1040,   1044,
        1047,   1051,   1055,   1058,   1062,   1066,   1069,   1073,   1076,   1080,   1084,   1087,   1091,   1095,   1099,   1102,   1106,   1110,   1113,   1117,   1120,   1124,   1128,   1131,
        1135,   1139,   1143,   1146,   1150,   1154,   1158,   1161,   1165,   1169,   1173,   1176,   1180,   1184,   1188,   1191,   1195,   1199,   1203,   1206,   1210,   1214,   1218,   1221,
        1225,   1229,   1233,   1236,   1240,   1244,   1248,   1251,   1255,   1259,   1263,   1266,   1270,   1274,   1278,   1281,   1285,   1289,   1293,   1297,   1301,   1305,   1309,   1312,
        1316,   1320,   1324,   1327,   1331,   1335,   1339,   1343,   1347,   1351,   1355,   1358,   1362,   1366,   1370,   1373,   1377,   1381,   1385,   1389,   1393,   1397,   1401,   1404,
        1408,   1412,   1416,   1420,   1424,   1428,   1432,   1435,   1439,   1443,   1447,   1451,   1455,   1459,   1463,   1467,   1471,   1475,   1479,   1482,   1486,   1490,   1494,   1498,
        1502,   1506,   1510,   1514,   1518,   1522,   1526,   1529,   1533,   1537,   1541,   1545,   1549,   1553,   1557,   1561,   1565,   1569,   1573,   1577,   1581,   1585,   1589,   1593,
        1597,   1601,   1605,   1609,   1613,   1617,   1621,   1624,   1628,   1632,   1636,   1640,   1644,   1648,   1652,   1656,   1660,   1664,   1668,   1672,   1676,   1680,   1684,   1688,
        1692,   1696,   1700,   1704,   1708,   1712,   1717,   1721,   1725,   1729,   1733,   1737,   1741,   1745,   1749,   1753,   1757,   1761,   1765,   1769,   1773,   1777,   1781,   1785,
        1789,   1793,   1797,   1801,   1805,   1809,   1813,   1817,   1821,   1825,   1830,   1834,   1838,   1842,   1846,   1850,   1854,   1858,   1862,   1866,   1870,   1874,   1879,   1883,
        1887,   1891,   1895,   1899,   1903,   1907,   1911,   1915,   1919,   1923,   1928,   1932,   1936,   1940,   1944,   1948,   1952,   1956,   1961,   1965,   1969,   1973,   1977,   1981,
        1985,   1989,   1994,   1998,   2002,   2006,   2010,   2014,   2018,   2022,   2027,   2031,   2035,   2039,   2044,   2048,   2052,   2056,   2060,   2064,   2068,   2072,   2077,   2081,
        2085,   2089,   2094,   2098,   2102,   2106,   2111,   2115,   2119,   2123,   2128,   2132,   2136,   2140,   2144,   2148,   2152,   2156,   2161,   2165,   2169,   2173,   2178,   2182,
        2186,   2190,   2195,   2199,   2203,   2207,   2212,   2216,   2220,   2224,   2229,   2233,   2237,   2241,   2246,   2250,   2254,   2259,   2263,   2268,   2272,   2276,   2281,   2285,
        2289,   2293,   2298,   2302,   2306,   2310,   2315,   2319,   2323,   2327,   2332,   2336,   2340,   2345,   2349,   2354,   2358,   2362,   2367,   2371,   2375,   2380,   2384,   2389,
        2393,   2397,   2402,   2406,   2410,   2415,   2419,   2424,   2428,   2432,   2437,   2441,   2445,   2450,   2454,   2459,   2463,   2467,   2472,   2476,   2480,   2485,   2489,   2494,
        2498,   2503,   2507,   2512,   2516,   2520,   2525,   2529,   2533,   2538,   2542,   2547,   2551,   2556,   2560,   2565,   2569,   2574,   2578,   2583,   2587,   2592,   2596,   2601,
        2605,   2610,   2614,   2619,   2623,   2628,   2632,   2637,   2641,   2646,   2650,   2655,   2659,   2664,   2668,   2673,   2677,   2682,   2686,   2691,   2695,   2700,   2704,   2709,
        2713,   2718,   2723,   2727,   2732,   2737,   2741,   2746,   2750,   2755,   2759,   2764,   2768,   2773,   2778,   2782,   2787,   2792,   2796,   2801,   2805,   2810,   2815,   2819,
        2824,   2829,   2833,   2838,   2842,   2847,   2852,   2856,   2861,   2866,   2870,   2875,   2879,   2884,   2889,   2893,   2898,   2903,   2908,   2912,   2917,   2922,   2927,   2931,
        2936,   2941,   2946,   2950,   2955,   2960,   2965,   2969,   2974,   2979,   2984,   2988,   2993,   2998,   3003,   3008,   3013,   3018,   3023,   3027,   3032,   3037,   3042,   3046,
        3051,   3056,   3061,   3066,   3071,   3076,   3081,   3085,   3090,   3095,   3100,   3105,   3110,   3115,   3120,   3124,   3129,   3134,   3139,   3144,   3149,   3154,   3159,   3163,
        3168,   3173,   3178,   3183,   3188,   3193,   3198,   3203,   3208,   3213,   3218,   3223,   3228,   3233,   3238,   3243,   3248,   3253,   3258,   3263,   3268,   3273,   3278,   3283,
        3288,   3293,   3298,   3303,   3308,   3313,   3318,   3323,   3328,   3333,   3338,   3343,   3348,   3353,   3358,   3363,   3368,   3373,   3378,   3383,   3388,   3393,   3398,   3403,
        3408,   3413,   3418,   3423,   3428,   3433,   3438,   3443,   3448,   3453,   3458,   3463,   3468,   3473,   3479,   3484,   3489,   3494,   3499,   3504,   3509,   3514,   3519,   3524,
        3529,   3534,   3539,   3544,   3549,   3554,   3560,   3565,   3570,   3575,   3580,   3585,   3590,   3595,   3600,   3605,   3610,   3615,   3621,   3626,   3631,   3636,   3641,   3646,
        3651,   3656,   3661,   3666,   3671,   3676,   3682,   3687,   3692,   3697,   3702,   3707,   3712,   3717,   3722,   3727,   3732,   3737,   3742,   3747,   3752,   3757,   3763,   3768,
        3773,   3778,   3783,   3788,   3793,   3798,   3803,   3808,   3813,   3818,   3824,   3829,   3834,   3839,   3844,   3849,   3854,   3859,   3864,   3869,   3874,   3879,   3884,   3889,
        3894,   3899,   3904,   3909,   3914,   3919,   3924,   3929,   3934,   3939,   3945,   3950,   3955,   3960,   3965,   3970,   3975,   3980,   3985,   3990,   3995,   4000,   4005,   4010,
        4015,   4020,   4025,   4030,   4035,   4040,   4045,   4050,   4055,   4060,   4065,   4070,   4075,   4080,   4085,   4090,   4095

    };
    // HDR mode is only supported in GK7608V200
    static GK_U16 au16GammaHDR[GAMMA_NODE_NUM] = {
        0,   42,   67,     87,     105,    122,    138,    152,    167,    180,    193,    205,    218,    229,    241,    252,    263,    274,    284,    295,    305,    315,    325,    334,   344,
        353,   363,   372,    381,    390,    398,    407,    416,    424,    433,    441,    449,    458,    466,    474,    482,    490,    498,    505,    513,    521,    528,   536,   543,   551,
        558,   566,   573,    580,    587,    594,    602,    609,    616,    623,    630,    636,    643,    650,    657,    664,    670,    677,    684,    690,    697,    704,   710,   717,   723,
        729,   736,   742,    749,    755,    761,    767,    774,    780,    786,    792,    798,    805,    811,    817,    823,    829,    835,    841,    847,    853,    859,   864,   870,   876,
        882,   888,   894,    899,    905,    911,    917,    922,    928,    934,    939,    945,    950,    956,    962,    967,    973,    978,    984,    989,    995,    1000,   1006,   1011,   1017,
        1022,   1027,   1033,   1038,   1043,   1049,   1054,   1059,   1065,   1070,   1075,   1080,   1086,   1091,   1096,   1101,   1106,   1112,   1117,   1122,   1127,   1132,   1137,   1142,   1148,
        1153,   1158,   1163,   1168,   1173,   1178,   1183,   1188,   1193,   1198,   1203,   1208,   1213,   1218,   1223,   1227,   1232,   1237,   1242,   1247,   1252,   1257,   1262,   1266,   1271,
        1276,   1281,   1286,   1290,   1295,   1300,   1305,   1309,   1314,   1319,   1324,   1328,   1333,   1338,   1343,   1347,   1352,   1357,   1361,   1366,   1370,   1375,   1380,   1384,   1389,
        1394,   1398,   1403,   1407,   1412,   1416,   1421,   1426,   1430,   1435,   1439,   1444,   1448,   1453,   1457,   1462,   1466,   1471,   1475,   1480,   1484,   1489,   1493,   1497,   1502,
        1506,   1511,   1515,   1519,   1524,   1528,   1533,   1537,   1541,   1546,   1550,   1554,   1559,   1563,   1567,   1572,   1576,   1580,   1585,   1589,   1593,   1598,   1602,   1606,   1610,
        1615,   1619,   1623,   1627,   1632,   1636,   1640,   1644,   1649,   1653,   1657,   1661,   1665,   1670,   1674,   1678,   1682,   1686,   1691,   1695,   1699,   1703,   1707,   1711,   1715,
        1720,   1724,   1728,   1732,   1736,   1740,   1744,   1748,   1752,   1756,   1761,   1765,   1769,   1773,   1777,   1781,   1785,   1789,   1793,   1797,   1801,   1805,   1809,   1813,   1817,
        1821,   1825,   1829,   1833,   1837,   1841,   1845,   1849,   1853,   1857,   1861,   1865,   1869,   1873,   1877,   1881,   1885,   1889,   1893,   1897,   1900,   1904,   1908,   1912,   1916,
        1920,   1924,   1928,   1932,   1936,   1939,   1943,   1947,   1951,   1955,   1959,   1963,   1966,   1970,   1974,   1978,   1982,   1986,   1990,   1993,   1997,   2001,   2005,   2009,   2012,
        2016,   2020,   2024,   2028,   2031,   2035,   2039,   2043,   2047,   2050,   2054,   2058,   2062,   2065,   2069,   2073,   2077,   2080,   2084,   2088,   2092,   2095,   2099,   2103,   2106,
        2110,   2114,   2118,   2121,   2125,   2129,   2132,   2136,   2140,   2143,   2147,   2151,   2154,   2158,   2162,   2166,   2169,   2173,   2176,   2180,   2184,   2187,   2191,   2195,   2198,
        2202,   2206,   2209,   2213,   2216,   2220,   2224,   2227,   2231,   2235,   2238,   2242,   2245,   2249,   2253,   2256,   2260,   2263,   2267,   2270,   2274,   2278,   2281,   2285,   2288,
        2292,   2295,   2299,   2303,   2306,   2310,   2313,   2317,   2320,   2324,   2327,   2331,   2334,   2338,   2341,   2345,   2348,   2352,   2355,   2359,   2363,   2366,   2370,   2373,   2377,
        2380,   2383,   2387,   2390,   2394,   2397,   2401,   2404,   2408,   2411,   2415,   2418,   2422,   2425,   2429,   2432,   2436,   2439,   2442,   2446,   2449,   2453,   2456,   2460,   2463,
        2466,   2470,   2473,   2477,   2480,   2484,   2487,   2490,   2494,   2497,   2501,   2504,   2507,   2511,   2514,   2518,   2521,   2524,   2528,   2531,   2535,   2538,   2541,   2545,   2548,
        2551,   2555,   2558,   2561,   2565,   2568,   2572,   2575,   2578,   2582,   2585,   2588,   2592,   2595,   2598,   2602,   2605,   2608,   2612,   2615,   2618,   2622,   2625,   2628,   2632,
        2635,   2638,   2642,   2645,   2648,   2651,   2655,   2658,   2661,   2665,   2668,   2671,   2674,   2678,   2681,   2684,   2688,   2691,   2694,   2697,   2701,   2704,   2707,   2711,   2714,
        2717,   2720,   2724,   2727,   2730,   2733,   2737,   2740,   2743,   2746,   2750,   2753,   2756,   2759,   2762,   2766,   2769,   2772,   2775,   2779,   2782,   2785,   2788,   2792,   2795,
        2798,   2801,   2804,   2808,   2811,   2814,   2817,   2820,   2824,   2827,   2830,   2833,   2836,   2840,   2843,   2846,   2849,   2852,   2855,   2859,   2862,   2865,   2868,   2871,   2874,
        2878,   2881,   2884,   2887,   2890,   2893,   2897,   2900,   2903,   2906,   2909,   2912,   2915,   2919,   2922,   2925,   2928,   2931,   2934,   2937,   2941,   2944,   2947,   2950,   2953,
        2956,   2959,   2962,   2966,   2969,   2972,   2975,   2978,   2981,   2984,   2987,   2990,   2994,   2997,   3000,   3003,   3006,   3009,   3012,   3015,   3018,   3021,   3024,   3028,   3031,
        3034,   3037,   3040,   3043,   3046,   3049,   3052,   3055,   3058,   3061,   3064,   3068,   3071,   3074,   3077,   3080,   3083,   3086,   3089,   3092,   3095,   3098,   3101,   3104,   3107,
        3110,   3113,   3116,   3119,   3122,   3125,   3128,   3132,   3135,   3138,   3141,   3144,   3147,   3150,   3153,   3156,   3159,   3162,   3165,   3168,   3171,   3174,   3177,   3180,   3183,
        3186,   3189,   3192,   3195,   3198,   3201,   3204,   3207,   3210,   3213,   3216,   3219,   3222,   3225,   3228,   3231,   3234,   3237,   3240,   3243,   3246,   3249,   3252,   3255,   3257,
        3260,   3263,   3266,   3269,   3272,   3275,   3278,   3281,   3284,   3287,   3290,   3293,   3296,   3299,   3302,   3305,   3308,   3311,   3314,   3317,   3320,   3322,   3325,   3328,   3331,
        3334,   3337,   3340,   3343,   3346,   3349,   3352,   3355,   3358,   3361,   3364,   3366,   3369,   3372,   3375,   3378,   3381,   3384,   3387,   3390,   3393,   3396,   3398,   3401,   3404,
        3407,   3410,   3413,   3416,   3419,   3422,   3425,   3427,   3430,   3433,   3436,   3439,   3442,   3445,   3448,   3451,   3453,   3456,   3459,   3462,   3465,   3468,   3471,   3474,   3476,
        3479,   3482,   3485,   3488,   3491,   3494,   3497,   3499,   3502,   3505,   3508,   3511,   3514,   3517,   3519,   3522,   3525,   3528,   3531,   3534,   3536,   3539,   3542,   3545,   3548,
        3551,   3554,   3556,   3559,   3562,   3565,   3568,   3571,   3573,   3576,   3579,   3582,   3585,   3588,   3590,   3593,   3596,   3599,   3602,   3604,   3607,   3610,   3613,   3616,   3619,
        3621,   3624,   3627,   3630,   3633,   3635,   3638,   3641,   3644,   3647,   3649,   3652,   3655,   3658,   3661,   3663,   3666,   3669,   3672,   3675,   3677,   3680,   3683,   3686,   3689,
        3691,   3694,   3697,   3700,   3702,   3705,   3708,   3711,   3714,   3716,   3719,   3722,   3725,   3727,   3730,   3733,   3736,   3738,   3741,   3744,   3747,   3750,   3752,   3755,   3758,
        3761,   3763,   3766,   3769,   3772,   3774,   3777,   3780,   3783,   3785,   3788,   3791,   3794,   3796,   3799,   3802,   3805,   3807,   3810,   3813,   3816,   3818,   3821,   3824,   3826,
        3829,   3832,   3835,   3837,   3840,   3843,   3846,   3848,   3851,   3854,   3856,   3859,   3862,   3865,   3867,   3870,   3873,   3876,   3878,   3881,   3884,   3886,   3889,   3892,   3894,
        3897,   3900,   3903,   3905,   3908,   3911,   3913,   3916,   3919,   3922,   3924,   3927,   3930,   3932,   3935,   3938,   3940,   3943,   3946,   3948,   3951,   3954,   3957,   3959,   3962,
        3965,   3967,   3970,   3973,   3975,   3978,   3981,   3983,   3986,   3989,   3991,   3994,   3997,   3999,   4002,   4005,   4007,   4010,   4013,   4015,   4018,   4021,   4023,   4026,   4029,
        4031,   4034,   4037,   4039,   4042,   4045,   4047,   4050,   4053,   4055,   4058,   4061,   4063,   4066,   4069,   4071,   4074,   4077,   4079,   4082,   4084,   4087,   4090,   4092,   4095

    };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_BOOL(pstGammaAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u32WDRMode = ext_system_sensor_wdr_mode_read(ViPipe);

    switch (pstGammaAttr->enCurveType) {
        case ISP_GAMMA_CURVE_DEFAULT:
            if (u32WDRMode == 0) {
                pu16GammaLut = au16GammaDef;
            } else {
                pu16GammaLut = au16GammaDefWDR;
            }
            break;
        case ISP_GAMMA_CURVE_SRGB:
            if (u32WDRMode == 0) {
                pu16GammaLut = au16GammasRGB;
            } else {
                pu16GammaLut = au16Gamma033;
            }
            break;
        case ISP_GAMMA_CURVE_HDR:
            ISP_TRACE(MODULE_DBG_WARN, "ISP_GAMMA_CURVE_HDR is only used for GK7608V200!\n");
            pu16GammaLut = au16GammaHDR;
            break;
        case ISP_GAMMA_CURVE_USER_DEFINE:
            pu16GammaLut = pstGammaAttr->u16Table;
            break;
        default:
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  Gamma Curve Type %d!\n", pstGammaAttr->enCurveType);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < GAMMA_NODE_NUM; i++) {
        if (pu16GammaLut[i] > 0xFFF) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Gamma Table[%d] %d!\n", i, pu16GammaLut[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_gamma_lut_write(ViPipe, i, pu16GammaLut[i]);
    }

    ext_system_gamma_lut_update_write(ViPipe, GK_TRUE);
    ext_system_gamma_en_write(ViPipe, pstGammaAttr->bEnable);
    ext_system_gamma_curve_type_write(ViPipe, pstGammaAttr->enCurveType);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetGammaAttr(VI_PIPE ViPipe, ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    GK_U32 i = 0;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstGammaAttr->bEnable = ext_system_gamma_en_read(ViPipe);
    pstGammaAttr->enCurveType = ext_system_gamma_curve_type_read(ViPipe);

    for (i = 0; i < GAMMA_NODE_NUM; i++) {
        pstGammaAttr->u16Table[i] = ext_system_gamma_lut_read(ViPipe, i);
    }

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetPreGammaAttr(VI_PIPE ViPipe, const ISP_PREGAMMA_ATTR_S *pstPreGammaAttr)
{
#ifdef CONFIG_ISP_PREGAMMA_SUPPORT
    GK_U32 i = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPreGammaAttr);
    ISP_CHECK_BOOL(pstPreGammaAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        if (pstPreGammaAttr->au32Table[i] > ISP_PREGAMMA_LUT_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Table[%d] %d!\n", i, pstPreGammaAttr->au32Table[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_pregamma_lut_write(ViPipe, i, pstPreGammaAttr->au32Table[i]);
    }

    ext_system_pregamma_lut_update_write(ViPipe, GK_TRUE);
    ext_system_pregamma_en_write(ViPipe, pstPreGammaAttr->bEnable);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetPreGammaAttr(VI_PIPE ViPipe, ISP_PREGAMMA_ATTR_S *pstPreGammaAttr)
{
#ifdef CONFIG_ISP_PREGAMMA_SUPPORT
    GK_U32 i = 0;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPreGammaAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstPreGammaAttr->bEnable = ext_system_pregamma_en_read(ViPipe);

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        pstPreGammaAttr->au32Table[i] = ext_system_pregamma_lut_read(ViPipe, i);
    }

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetPreLogLUTAttr(VI_PIPE ViPipe, const ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr)
{
    return ISP_SetPreLogLUTAttr(ViPipe, pstPreLogLUTAttr);
}

GK_S32 MPI_ISP_GetPreLogLUTAttr(VI_PIPE ViPipe, ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr)
{
    return ISP_GetPreLogLUTAttr(ViPipe, pstPreLogLUTAttr);
}

GK_S32 MPI_ISP_SetLogLUTAttr(VI_PIPE ViPipe, const ISP_LOGLUT_ATTR_S *pstLogLUTAttr)
{
    return ISP_SetLogLUTAttr(ViPipe, pstLogLUTAttr);
}

GK_S32 MPI_ISP_GetLogLUTAttr(VI_PIPE ViPipe, ISP_LOGLUT_ATTR_S *pstLogLUTAttr)
{
    return ISP_GetLogLUTAttr(ViPipe, pstLogLUTAttr);
}

GK_S32 MPI_ISP_SetCSCAttr(VI_PIPE ViPipe, const ISP_CSC_ATTR_S *pstCSCAttr)
{
    GK_U8 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCSCAttr);
    ISP_CHECK_BOOL(pstCSCAttr->bEnable);
    ISP_CHECK_BOOL(pstCSCAttr->bLimitedRangeEn);
    ISP_CHECK_BOOL(pstCSCAttr->bCtModeEn);
    ISP_CHECK_BOOL(pstCSCAttr->bExtCscEn);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if ((pstCSCAttr->u8Luma > 100) || (pstCSCAttr->u8Contr > 100) ||
        (pstCSCAttr->u8Satu > 100) || (pstCSCAttr->u8Hue > 100)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid csc parameter!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstCSCAttr->enColorGamut >= COLOR_GAMUT_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid color gamut!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstCSCAttr->enColorGamut == COLOR_GAMUT_BT2020) {
        ISP_TRACE(MODULE_DBG_WARN, "BT.2020 gamut is only used in GK7608V200!\n");
    }

    ext_system_csc_enable_write(ViPipe, pstCSCAttr->bEnable);
    ext_system_csc_gamuttype_write(ViPipe, pstCSCAttr->enColorGamut);
    ext_system_csc_luma_write(ViPipe, pstCSCAttr->u8Luma);
    ext_system_csc_contrast_write(ViPipe, pstCSCAttr->u8Contr);
    ext_system_csc_hue_write(ViPipe, pstCSCAttr->u8Hue);
    ext_system_csc_sat_write(ViPipe, pstCSCAttr->u8Satu);
    ext_system_csc_limitrange_en_write(ViPipe, pstCSCAttr->bLimitedRangeEn);
    ext_system_csc_ext_en_write(ViPipe, pstCSCAttr->bExtCscEn);
    ext_system_csc_ctmode_en_write(ViPipe, pstCSCAttr->bCtModeEn);

    for (i = 0; i < 3; i++) {
        if ((pstCSCAttr->stCscMagtrx.as16CSCIdc[i] > 1023) || (pstCSCAttr->stCscMagtrx.as16CSCIdc[i] < -1024)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid as16CSCIdc[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if ((pstCSCAttr->stCscMagtrx.as16CSCOdc[i] > 1023) || (pstCSCAttr->stCscMagtrx.as16CSCOdc[i] < -1024)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid as16CSCOdc[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_csc_dcin_write(ViPipe, i,  pstCSCAttr->stCscMagtrx.as16CSCIdc[i]);
        ext_system_csc_dcout_write(ViPipe, i, pstCSCAttr->stCscMagtrx.as16CSCOdc[i]);
    }

    for (i = 0; i < 9; i++) {
        ext_system_csc_coef_write(ViPipe, i, pstCSCAttr->stCscMagtrx.as16CSCCoef[i]);
    }

    ext_system_csc_attr_update_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetCSCAttr(VI_PIPE ViPipe, ISP_CSC_ATTR_S *pstCSCAttr)
{
    GK_U8 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCSCAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstCSCAttr->bEnable         = ext_system_csc_enable_read(ViPipe);
    pstCSCAttr->enColorGamut    = ext_system_csc_gamuttype_read(ViPipe);
    pstCSCAttr->u8Hue           = ext_system_csc_hue_read(ViPipe);
    pstCSCAttr->u8Luma          = ext_system_csc_luma_read(ViPipe);
    pstCSCAttr->u8Contr         = ext_system_csc_contrast_read(ViPipe);
    pstCSCAttr->u8Satu          = ext_system_csc_sat_read(ViPipe);
    pstCSCAttr->bLimitedRangeEn = ext_system_csc_limitrange_en_read(ViPipe);
    pstCSCAttr->bExtCscEn       = ext_system_csc_ext_en_read(ViPipe);
    pstCSCAttr->bCtModeEn       = ext_system_csc_ctmode_en_read(ViPipe);

    for (i = 0; i < 3; i++) {
        pstCSCAttr->stCscMagtrx.as16CSCIdc[i] = ext_system_csc_dcin_read(ViPipe, i);
        pstCSCAttr->stCscMagtrx.as16CSCOdc[i] = ext_system_csc_dcout_read(ViPipe, i);
    }

    for (i = 0; i < 9; i++) {
        pstCSCAttr->stCscMagtrx.as16CSCCoef[i] = ext_system_csc_coef_read(ViPipe, i);
    }

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetDPCalibrate(VI_PIPE ViPipe, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    GK_U16 u16StaticCountMax = STATIC_DP_COUNT_NORMAL;
    ISP_WORKING_MODE_S stIspWorkMode;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnableDetect);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode) != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Work Mode error!\n");
        return GK_FAILURE;
    }

    if (((stIspWorkMode.enIspRunningMode == ISP_MODE_RUNNING_OFFLINE) ||
         (stIspWorkMode.enIspRunningMode == ISP_MODE_RUNNING_STRIPING)) &&
        (pstDPCalibrate->bEnableDetect == GK_TRUE) &&
        (ISP_SUPPORT_OFFLINE_DPC_CALIBRATION) != 1) {
        ISP_TRACE(MODULE_DBG_ERR, "Only support dpc calibration under online mode!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    u16StaticCountMax = u16StaticCountMax * stIspWorkMode.u8BlockNum;

    u16StaticCountMax = MIN2(u16StaticCountMax, STATIC_DP_COUNT_MAX);

    if (pstDPCalibrate->u16CountMax > u16StaticCountMax) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16CountMax!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16CountMin > pstDPCalibrate->u16CountMax) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16CountMin!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16TimeLimit > 0x640) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16TimeLimit!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDPCalibrate->enStaticDPType >= ISP_STATIC_DP_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enStaticDPType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDPCalibrate->u8StartThresh == 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8StartThresh!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_dpc_static_calib_enable_write(ViPipe, pstDPCalibrate->bEnableDetect);

    ext_system_dpc_static_defect_type_write(ViPipe, pstDPCalibrate->enStaticDPType);
    ext_system_dpc_start_thresh_write(ViPipe, pstDPCalibrate->u8StartThresh);
    ext_system_dpc_count_max_write(ViPipe, pstDPCalibrate->u16CountMax);
    ext_system_dpc_count_min_write(ViPipe, pstDPCalibrate->u16CountMin);
    ext_system_dpc_trigger_time_write(ViPipe, pstDPCalibrate->u16TimeLimit);
    ext_system_dpc_trigger_status_write(ViPipe, ISP_STATE_INIT);

    return GK_SUCCESS;
#else
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetDPCalibrate(VI_PIPE ViPipe, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    GK_U16 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDPCalibrate->bEnableDetect   = ext_system_dpc_static_calib_enable_read(ViPipe);

    pstDPCalibrate->enStaticDPType  = ext_system_dpc_static_defect_type_read(ViPipe);
    pstDPCalibrate->u8StartThresh   = ext_system_dpc_start_thresh_read(ViPipe);
    pstDPCalibrate->u16CountMax     = ext_system_dpc_count_max_read(ViPipe);
    pstDPCalibrate->u16CountMin     = ext_system_dpc_count_min_read(ViPipe);
    pstDPCalibrate->u16TimeLimit    = ext_system_dpc_trigger_time_read(ViPipe);

    pstDPCalibrate->enStatus        = ext_system_dpc_trigger_status_read(ViPipe);
    pstDPCalibrate->u8FinishThresh  = ext_system_dpc_finish_thresh_read(ViPipe);
    pstDPCalibrate->u16Count        = ext_system_dpc_bpt_calib_number_read(ViPipe);

    if (pstDPCalibrate->enStatus == ISP_STATE_INIT) {
        for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
            pstDPCalibrate->au32Table[i] = 0;
        }
    } else {
        for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
            if (i < pstDPCalibrate->u16Count) {
                pstDPCalibrate->au32Table[i] = ext_system_dpc_calib_bpt_read(ViPipe, i);
            } else {
                pstDPCalibrate->au32Table[i] = 0;
            }
        }
    }
    return GK_SUCCESS;
#else
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetDPStaticAttr(VI_PIPE ViPipe, const ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    GK_U16 i = 0, j = 0, m = 0, u16CountIn = 0;
    GK_S32 s32Ret = GK_SUCCESS;
    GK_U16 u16StaticCountMax = STATIC_DP_COUNT_NORMAL;
    GK_U32 *pu32DefectPixelTable = GK_NULL;
    ISP_WORKING_MODE_S stIspWorkMode;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDPStaticAttr);
    ISP_CHECK_BOOL(pstDPStaticAttr->bEnable);
    ISP_CHECK_BOOL(pstDPStaticAttr->bShow);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode)) {
        ISP_TRACE(MODULE_DBG_ERR, "Get Work Mode error!\n");
        return GK_FAILURE;
    }

    if ((stIspWorkMode.enIspRunningMode == ISP_MODE_RUNNING_SIDEBYSIDE)) {
        u16StaticCountMax = u16StaticCountMax * ISP_SBS_BLOCK_NUM;
    } else if (stIspWorkMode.enIspRunningMode == ISP_MODE_RUNNING_STRIPING) {
        if (ISP_SUPPORT_OFFLINE_DPC_CALIBRATION == 1) {
            u16StaticCountMax = u16StaticCountMax * stIspWorkMode.u8BlockNum;
        } else {
            u16StaticCountMax = u16StaticCountMax * ISP_SBS_BLOCK_NUM;
        }
    }

    if (pstDPStaticAttr->u16BrightCount > u16StaticCountMax) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16BrightCount!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDPStaticAttr->u16DarkCount > u16StaticCountMax) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16DarkCount!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < u16StaticCountMax; i++) {
        if (pstDPStaticAttr->au32BrightTable[i] > 0x1FFF1FFF || pstDPStaticAttr->au32DarkTable[i] > 0x1FFF1FFF) {
            ISP_TRACE(MODULE_DBG_ERR, "u32DarkTable and au32BrightTable should be less than 0x1FFF1FFF!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    pu32DefectPixelTable = (GK_U32 *)ISP_MALLOC(sizeof(GK_U32) * u16StaticCountMax);

    if (pu32DefectPixelTable == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "Malloc defectPixelTable Memory failure!\n");
        return ERR_CODE_ISP_NOMEM;
    }

    i = 0;
    /* merging dark talbe and bright table */
    while ((i < pstDPStaticAttr->u16BrightCount) && (j < pstDPStaticAttr->u16DarkCount)) {
        if (m >= u16StaticCountMax) {
            ISP_TRACE(MODULE_DBG_ERR, "The size of merging DP table(BrightTable and DarkTable) is larger than %d!\n",
                      u16StaticCountMax);
            s32Ret = ERR_CODE_ISP_ILLEGAL_PARAM;
            goto EXIT_SET_DP_ATTR;
        }
        if (pstDPStaticAttr->au32BrightTable[i] > pstDPStaticAttr->au32DarkTable[j]) {
            pu32DefectPixelTable[m++] = pstDPStaticAttr->au32DarkTable[j++];
        } else if (pstDPStaticAttr->au32BrightTable[i] < pstDPStaticAttr->au32DarkTable[j]) {
            pu32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i++];
        } else {
            pu32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i];
            i++;
            j++;
        }
    }

    if (i >= pstDPStaticAttr->u16BrightCount) {
        while (j < pstDPStaticAttr->u16DarkCount) {
            if (m >= u16StaticCountMax) {
                ISP_TRACE(MODULE_DBG_ERR, "The size of merging DP table(BrightTable and DarkTable) is larger than %d!\n",
                          u16StaticCountMax);
                s32Ret = ERR_CODE_ISP_ILLEGAL_PARAM;
                goto EXIT_SET_DP_ATTR;
            }
            pu32DefectPixelTable[m++] = pstDPStaticAttr->au32DarkTable[j++];
        }
    }
    if (j >= pstDPStaticAttr->u16DarkCount) {
        while (i < pstDPStaticAttr->u16BrightCount) {
            if (m >= u16StaticCountMax) {
                ISP_TRACE(MODULE_DBG_ERR, "The size of merging DP table(BrightTable and DarkTable) is larger than %d!\n",
                          u16StaticCountMax);
                s32Ret = ERR_CODE_ISP_ILLEGAL_PARAM;
                goto EXIT_SET_DP_ATTR;
            }
            pu32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i++];
        }
    }

    u16CountIn = m;

    ext_system_dpc_static_cor_enable_write(ViPipe, pstDPStaticAttr->bEnable);
    ext_system_dpc_static_dp_show_write(ViPipe, pstDPStaticAttr->bShow);
    ext_system_dpc_bpt_cor_number_write(ViPipe, u16CountIn);

    for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
        if (i < u16CountIn) {
            ext_system_dpc_cor_bpt_write(ViPipe, i, pu32DefectPixelTable[i]);
        } else {
            ext_system_dpc_cor_bpt_write(ViPipe, i, 0);
        }
    }
    ext_system_dpc_static_attr_update_write(ViPipe, GK_TRUE);

EXIT_SET_DP_ATTR:

    ISP_FREE(pu32DefectPixelTable);

    return s32Ret;
#else
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetDPStaticAttr(VI_PIPE ViPipe, ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    GK_U32 i = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDPStaticAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDPStaticAttr->bEnable        = ext_system_dpc_static_cor_enable_read(ViPipe);
    pstDPStaticAttr->bShow          = ext_system_dpc_static_dp_show_read(ViPipe);
    pstDPStaticAttr->u16BrightCount = ext_system_dpc_bpt_cor_number_read(ViPipe);
    pstDPStaticAttr->u16DarkCount   = 0;

    for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
        if (i < pstDPStaticAttr->u16BrightCount) {
            pstDPStaticAttr->au32BrightTable[i] = ext_system_dpc_cor_bpt_read(ViPipe, i);
        } else {
            pstDPStaticAttr->au32BrightTable[i] = 0;
        }
        pstDPStaticAttr->au32DarkTable[i] = 0;
    }
    return GK_SUCCESS;
#else
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetDPDynamicAttr(VI_PIPE ViPipe, const ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
    GK_U8 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDPDynamicAttr);
    ISP_CHECK_BOOL(pstDPDynamicAttr->bEnable);
    ISP_CHECK_BOOL(pstDPDynamicAttr->bSupTwinkleEn);
    ISP_CHECK_OPEN(ViPipe);

    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstDPDynamicAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_dpc_dynamic_cor_enable_write(ViPipe, pstDPDynamicAttr->bEnable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstDPDynamicAttr->stAuto.au16Strength[i] > 255) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16Strength[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstDPDynamicAttr->stAuto.au16BlendRatio[i] > 0x80) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16BlendRatio[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_dpc_dynamic_strength_table_write(ViPipe, i, pstDPDynamicAttr->stAuto.au16Strength[i]);
        ext_system_dpc_dynamic_blend_ratio_table_write(ViPipe, i, pstDPDynamicAttr->stAuto.au16BlendRatio[i]);
    }

    if (pstDPDynamicAttr->stManual.u16Strength > 255) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Strength!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDPDynamicAttr->stManual.u16BlendRatio > 0x80) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid BlendRatio!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstDPDynamicAttr->s8SoftThr > 0x80 || pstDPDynamicAttr->s8SoftThr < 0x0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s8SoftThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_dpc_dynamic_manual_enable_write(ViPipe, pstDPDynamicAttr->enOpType);
    ext_system_dpc_dynamic_strength_write(ViPipe, pstDPDynamicAttr->stManual.u16Strength);
    ext_system_dpc_dynamic_blend_ratio_write(ViPipe, pstDPDynamicAttr->stManual.u16BlendRatio);

    ext_system_dpc_suppress_twinkle_enable_write(ViPipe, pstDPDynamicAttr->bSupTwinkleEn);
    ext_system_dpc_suppress_twinkle_thr_write(ViPipe, pstDPDynamicAttr->s8SoftThr);
    ext_system_dpc_suppress_twinkle_slope_write(ViPipe, pstDPDynamicAttr->u8SoftSlope);
    ext_system_dpc_dynamic_attr_update_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDPDynamicAttr(VI_PIPE ViPipe, ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDPDynamicAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDPDynamicAttr->bEnable       = ext_system_dpc_dynamic_cor_enable_read(ViPipe);
    pstDPDynamicAttr->enOpType      = (ISP_OP_TYPE_E)ext_system_dpc_dynamic_manual_enable_read(ViPipe);
    pstDPDynamicAttr->bSupTwinkleEn = ext_system_dpc_suppress_twinkle_enable_read(ViPipe);
    pstDPDynamicAttr->s8SoftThr     = ext_system_dpc_suppress_twinkle_thr_read(ViPipe);
    pstDPDynamicAttr->u8SoftSlope   = ext_system_dpc_suppress_twinkle_slope_read(ViPipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstDPDynamicAttr->stAuto.au16Strength[i]   = ext_system_dpc_dynamic_strength_table_read(ViPipe, i);
        pstDPDynamicAttr->stAuto.au16BlendRatio[i] = ext_system_dpc_dynamic_blend_ratio_table_read(ViPipe, i);
    }

    pstDPDynamicAttr->stManual.u16Strength   = ext_system_dpc_dynamic_strength_read(ViPipe);
    pstDPDynamicAttr->stManual.u16BlendRatio = ext_system_dpc_dynamic_blend_ratio_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetMeshShadingAttr(VI_PIPE ViPipe, const ISP_SHADING_ATTR_S *pstShadingAttr)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_BOOL(pstShadingAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ext_system_isp_mesh_shading_enable_write(ViPipe, pstShadingAttr->bEnable);
    if (pstShadingAttr->u16MeshStr > ISP_LSC_MESHSTR_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16MeshStr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_isp_mesh_shading_mesh_strength_write(ViPipe, pstShadingAttr->u16MeshStr);

    if (pstShadingAttr->u16BlendRatio > 256) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16BlendRatio!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_isp_mesh_shading_blendratio_write(ViPipe, pstShadingAttr->u16BlendRatio);

    ext_system_isp_mesh_shading_attr_updata_write(ViPipe, GK_TRUE);
    ext_system_isp_mesh_shading_fe_attr_updata_write(ViPipe, GK_TRUE);
    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetMeshShadingAttr(VI_PIPE ViPipe, ISP_SHADING_ATTR_S *pstShadingAttr)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstShadingAttr->bEnable       = ext_system_isp_mesh_shading_enable_read(ViPipe);
    pstShadingAttr->u16MeshStr    = ext_system_isp_mesh_shading_mesh_strength_read(ViPipe);
    pstShadingAttr->u16BlendRatio = ext_system_isp_mesh_shading_blendratio_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetMeshShadingGainLutAttr(VI_PIPE ViPipe, const ISP_SHADING_LUT_ATTR_S *pstShadingLutAttr)
{
    GK_U16 i;
    GK_U32 u32Width, u32Height;
    GK_U32 u32XSum = 0;
    GK_U32 u32YSum = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstShadingLutAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstShadingLutAttr->u8MeshScale > 7) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8MeshScale!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_isp_mesh_shading_mesh_scale_write(ViPipe, pstShadingLutAttr->u8MeshScale);

    for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
        if ((pstShadingLutAttr->astLscGainLut[0].au16RGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[0].au16GrGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[0].au16GbGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[0].au16BGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[1].au16RGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[1].au16GrGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[1].au16GbGain[i] > 1023) ||
            (pstShadingLutAttr->astLscGainLut[1].au16BGain[i] > 1023)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Gain!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        ext_system_isp_mesh_shading_r_gain0_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[0].au16RGain[i]);
        ext_system_isp_mesh_shading_gr_gain0_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[0].au16GrGain[i]);
        ext_system_isp_mesh_shading_gb_gain0_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[0].au16GbGain[i]);
        ext_system_isp_mesh_shading_b_gain0_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[0].au16BGain[i]);

        ext_system_isp_mesh_shading_r_gain1_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[1].au16RGain[i]);
        ext_system_isp_mesh_shading_gr_gain1_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[1].au16GrGain[i]);
        ext_system_isp_mesh_shading_gb_gain1_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[1].au16GbGain[i]);
        ext_system_isp_mesh_shading_b_gain1_write(ViPipe, i, pstShadingLutAttr->astLscGainLut[1].au16BGain[i]);
    }

    for (i = 0; i < ISP_RLSC_POINTS; i++) {
        ext_system_isp_bnr_shading_r_gain_write(ViPipe, i, pstShadingLutAttr->stBNRLscGainLut.au16RGain[i]);
        ext_system_isp_bnr_shading_gr_gain_write(ViPipe, i, pstShadingLutAttr->stBNRLscGainLut.au16GrGain[i]);
        ext_system_isp_bnr_shading_gb_gain_write(ViPipe, i, pstShadingLutAttr->stBNRLscGainLut.au16GbGain[i]);
        ext_system_isp_bnr_shading_b_gain_write(ViPipe, i, pstShadingLutAttr->stBNRLscGainLut.au16BGain[i]);
    }
    u32Width  = ext_system_be_total_width_read(ViPipe);
    u32Height = ext_system_be_total_height_read(ViPipe);

    for (i = 0; i < (ISP_LSC_GRID_COL - 1) / 2; i++) {
        u32XSum += pstShadingLutAttr->au16XGridWidth[i];
    }

    for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
        u32YSum += pstShadingLutAttr->au16YGridWidth[i];
    }

    if ((u32XSum != (u32Width / 4)) || (u32YSum != (u32Height / 4))) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid block size u32XSum = %d, u32YSum = %d!\n", u32XSum, u32YSum);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < (ISP_LSC_GRID_COL - 1) / 2; i++) {
        if ((pstShadingLutAttr->au16XGridWidth[i] > ISP_LSC_XGRID_WIDTH_MAX(u32Width)) ||
            (pstShadingLutAttr->au16XGridWidth[i] < 4)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au32XGridWidth[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_isp_mesh_shading_xgrid_write(ViPipe, i, pstShadingLutAttr->au16XGridWidth[i]);
    }

    for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
        if ((pstShadingLutAttr->au16YGridWidth[i] > ISP_LSC_YGRID_WIDTH_MAX(u32Height)) ||
            (pstShadingLutAttr->au16YGridWidth[i] < 4)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au32YGridWidth[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_isp_mesh_shading_ygrid_write(ViPipe, i, pstShadingLutAttr->au16YGridWidth[i]);
    }

    ext_system_isp_mesh_shading_lut_attr_updata_write(ViPipe, GK_TRUE);
    ext_system_isp_mesh_shading_fe_lut_attr_updata_write(ViPipe, GK_TRUE);
    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetMeshShadingGainLutAttr(VI_PIPE ViPipe, ISP_SHADING_LUT_ATTR_S *pstShadingLutAttr)
{
    GK_U16 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstShadingLutAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstShadingLutAttr->u8MeshScale = ext_system_isp_mesh_shading_mesh_scale_read(ViPipe);

    for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
        pstShadingLutAttr->astLscGainLut[0].au16RGain[i]  = ext_system_isp_mesh_shading_r_gain0_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[0].au16GrGain[i] = ext_system_isp_mesh_shading_gr_gain0_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[0].au16GbGain[i] = ext_system_isp_mesh_shading_gb_gain0_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[0].au16BGain[i]  = ext_system_isp_mesh_shading_b_gain0_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[1].au16RGain[i]  = ext_system_isp_mesh_shading_r_gain1_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[1].au16GrGain[i] = ext_system_isp_mesh_shading_gr_gain1_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[1].au16GbGain[i] = ext_system_isp_mesh_shading_gb_gain1_read(ViPipe, i);
        pstShadingLutAttr->astLscGainLut[1].au16BGain[i]  = ext_system_isp_mesh_shading_b_gain1_read(ViPipe, i);
    }

    for (i = 0; i < (ISP_LSC_GRID_COL - 1) / 2; i++) {
        pstShadingLutAttr->au16XGridWidth[i] = ext_system_isp_mesh_shading_xgrid_read(ViPipe, i);
    }

    for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
        pstShadingLutAttr->au16YGridWidth[i] = ext_system_isp_mesh_shading_ygrid_read(ViPipe, i);
    }

    for (i = 0; i < ISP_RLSC_POINTS; i++) {
        pstShadingLutAttr->stBNRLscGainLut.au16RGain[i]  = ext_system_isp_bnr_shading_r_gain_read(ViPipe, i);
        pstShadingLutAttr->stBNRLscGainLut.au16GrGain[i] = ext_system_isp_bnr_shading_gr_gain_read(ViPipe, i);
        pstShadingLutAttr->stBNRLscGainLut.au16GbGain[i] = ext_system_isp_bnr_shading_gb_gain_read(ViPipe, i);
        pstShadingLutAttr->stBNRLscGainLut.au16BGain[i]  = ext_system_isp_bnr_shading_b_gain_read(ViPipe, i);
    }

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetRadialShadingAttr(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr)
{
    return ISP_SetRadialShadingAttr(ViPipe, pstRaShadingAttr);
}

GK_S32 MPI_ISP_GetRadialShadingAttr(VI_PIPE ViPipe, ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr)
{
    return ISP_GetRadialShadingAttr(ViPipe, pstRaShadingAttr);
}

GK_S32 MPI_ISP_SetRadialShadingLUT(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr)
{
    return ISP_SetRadialShadingLUT(ViPipe, pstRaShadingLutAttr);
}

GK_S32 MPI_ISP_GetRadialShadingLUT(VI_PIPE ViPipe, ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr)
{
    return ISP_GetRadialShadingLUT(ViPipe, pstRaShadingLutAttr);
}

GK_S32 MPI_ISP_GetLightboxGain(VI_PIPE ViPipe, ISP_AWB_Calibration_Gain_S *pstAWBCalibrationGain)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstAWBCalibrationGain);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return ISP_GetLightboxGain(ViPipe, pstAWBCalibrationGain);
}
GK_S32 MPI_ISP_MeshShadingCalibration(VI_PIPE ViPipe, GK_U16 *pu16SrcRaw, ISP_MLSC_CALIBRATION_CFG_S *pstMLSCCaliCfg,
                                         ISP_MESH_SHADING_TABLE_S *pstMLSCTable)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu16SrcRaw);
    ISP_CHECK_POINTER(pstMLSCCaliCfg);
    ISP_CHECK_POINTER(pstMLSCTable);

    if (pstMLSCCaliCfg->u16ImgWidth % 4 != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ImgWidth:%d, Width must be a muliple of 4!\n", pstMLSCCaliCfg->u16ImgWidth);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16ImgHeight % 4 != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16ImgHeight:%d, Height must be a muliple of 4!\n", pstMLSCCaliCfg->u16ImgHeight);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16DstImgWidth % 4 != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16DstImgWidth:%d, Width must be a muliple of 4!\n", pstMLSCCaliCfg->u16DstImgWidth);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16DstImgHeight % 4 != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16DstImgHeight:%d, Height must be a muliple of 4!\n", pstMLSCCaliCfg->u16DstImgHeight);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstMLSCCaliCfg->u16OffsetX % 2 != 0) || (pstMLSCCaliCfg->u16OffsetY % 2) != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16OffsetX:%d/u16OffsetY:%d, Bayer Pattern of the cropped image does not match to the input image!\n",
                  pstMLSCCaliCfg->u16OffsetX, pstMLSCCaliCfg->u16OffsetY);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16DstImgWidth + pstMLSCCaliCfg->u16OffsetX > pstMLSCCaliCfg->u16ImgWidth) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16DstImgWidth:%d/u16OffsetX:%d, u16DstImgWidth + u16OffsetX > u16ImgWidth!\n",
                  pstMLSCCaliCfg->u16DstImgWidth, pstMLSCCaliCfg->u16OffsetX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16DstImgHeight + pstMLSCCaliCfg->u16OffsetY > pstMLSCCaliCfg->u16ImgHeight) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16DstImgHeight:%d/u16OffsetY:%d, u16DstImgHeight + u16OffsetY > u16ImgHeight!\n",
                  pstMLSCCaliCfg->u16DstImgHeight, pstMLSCCaliCfg->u16OffsetY);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u32MeshScale > 7) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u32MeshScale:%d, MeshScale must be smaller than 8!\n", pstMLSCCaliCfg->u32MeshScale);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstMLSCCaliCfg->enRawBit >= BAYER_RAWBIT_BUTT) || (pstMLSCCaliCfg->enRawBit < BAYER_RAWBIT_8BIT) ||
        ((pstMLSCCaliCfg->enRawBit % 2) == 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enRawBit type:%d!\n", pstMLSCCaliCfg->enRawBit);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->enBayer >= BAYER_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enBayer type:%d!\n", pstMLSCCaliCfg->enBayer);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16BLCOffsetB > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Blc(Chn B) offset:%d!\n", pstMLSCCaliCfg->u16BLCOffsetB);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16BLCOffsetGb > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Blc(Chn Gb) offset:%d!\n", pstMLSCCaliCfg->u16BLCOffsetGb);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16BLCOffsetGr > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Blc(Chn Gr) offset:%d!\n", pstMLSCCaliCfg->u16BLCOffsetGr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstMLSCCaliCfg->u16BLCOffsetR > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Blc(Chn R) offset:%d!\n", pstMLSCCaliCfg->u16BLCOffsetR);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    return ISP_MeshShadingCalibration(pu16SrcRaw, pstMLSCCaliCfg, pstMLSCTable);
}

GK_S32 MPI_ISP_SetLocalCacAttr(VI_PIPE ViPipe, const ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstLocalCacAttr);
    ISP_CHECK_BOOL(pstLocalCacAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstLocalCacAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLocalCacAttr->u16VarThr > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16VarThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstLocalCacAttr->u16PurpleDetRange > 410) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8PurpleDetRange!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
        if (pstLocalCacAttr->stAuto.au8DePurpleCrStr[i] > 8) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8DePurpleCrStr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstLocalCacAttr->stAuto.au8DePurpleCbStr[i] > 8) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8DePurpleCbStr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_localCAC_auto_cb_str_table_write(ViPipe, i, pstLocalCacAttr->stAuto.au8DePurpleCbStr[i]);
        ext_system_localCAC_auto_cr_str_table_write(ViPipe, i, pstLocalCacAttr->stAuto.au8DePurpleCrStr[i]);
    }

    for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
        if (pstLocalCacAttr->u16RDetThr[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid u16RDetThr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstLocalCacAttr->u16GDetThr[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid u16GDetThr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstLocalCacAttr->u16BDetThr[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid u16BDetThr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstLocalCacAttr->u16LumaDetThr[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid u16LumaDetThr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstLocalCacAttr->s16CbCrRatio[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid s16CbCrRatio[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_localCAC_r_thd_table_write(ViPipe, i, pstLocalCacAttr->u16RDetThr[i]);
        ext_system_localCAC_g_thd_table_write(ViPipe, i, pstLocalCacAttr->u16GDetThr[i]);
        ext_system_localCAC_b_thd_table_write(ViPipe, i, pstLocalCacAttr->u16BDetThr[i]);
        ext_system_localCAC_luma_thd_table_write(ViPipe, i, pstLocalCacAttr->u16LumaDetThr[i]);
        ext_system_localCAC_cbcr_ratio_table_write(ViPipe, i, pstLocalCacAttr->s16CbCrRatio[i]);

    }

    if (pstLocalCacAttr->stManual.u8DePurpleCrStr > 8) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DePurpleCrStr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstLocalCacAttr->stManual.u8DePurpleCbStr > 8) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DePurpleCrStr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_localCAC_manual_mode_enable_write(ViPipe, pstLocalCacAttr->enOpType);
    ext_system_localCAC_manual_cb_str_write(ViPipe, pstLocalCacAttr->stManual.u8DePurpleCbStr);
    ext_system_localCAC_manual_cr_str_write(ViPipe, pstLocalCacAttr->stManual.u8DePurpleCrStr);

    ext_system_localCAC_enable_write(ViPipe, pstLocalCacAttr->bEnable);

    ext_system_localCAC_purple_var_thr_write(ViPipe, pstLocalCacAttr->u16VarThr);
    ext_system_localCAC_purple_det_range_write(ViPipe, pstLocalCacAttr->u16PurpleDetRange);

    ext_system_LocalCAC_coef_update_en_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetLocalCacAttr(VI_PIPE ViPipe, ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstLocalCacAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstLocalCacAttr->bEnable            = ext_system_localCAC_enable_read(ViPipe);
    pstLocalCacAttr->u16PurpleDetRange  = ext_system_localCAC_purple_det_range_read(ViPipe);
    pstLocalCacAttr->u16VarThr          = ext_system_localCAC_purple_var_thr_read(ViPipe);
    pstLocalCacAttr->enOpType           = ext_system_localCAC_manual_mode_enable_read(ViPipe);
    pstLocalCacAttr->stManual.u8DePurpleCbStr = ext_system_localCAC_manual_cb_str_read(ViPipe);
    pstLocalCacAttr->stManual.u8DePurpleCrStr = ext_system_localCAC_manual_cr_str_read(ViPipe);

    for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
        pstLocalCacAttr->stAuto.au8DePurpleCbStr[i] = ext_system_localCAC_auto_cb_str_table_read(ViPipe, i);
        pstLocalCacAttr->stAuto.au8DePurpleCrStr[i] = ext_system_localCAC_auto_cr_str_table_read(ViPipe, i);
    }

    for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
        pstLocalCacAttr->u16RDetThr[i]    = ext_system_localCAC_r_thd_table_read(ViPipe, i);
        pstLocalCacAttr->u16GDetThr[i]    = ext_system_localCAC_g_thd_table_read(ViPipe, i);
        pstLocalCacAttr->u16BDetThr[i]    = ext_system_localCAC_b_thd_table_read(ViPipe, i);
        pstLocalCacAttr->u16LumaDetThr[i] = ext_system_localCAC_luma_thd_table_read(ViPipe, i);
        pstLocalCacAttr->s16CbCrRatio[i]  = ext_system_localCAC_cbcr_ratio_table_read(ViPipe, i);
    }

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetGlobalCacAttr(VI_PIPE ViPipe, const ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr)
{
#ifdef CONFIG_ISP_GCAC_SUPPORT
    GK_U16 u16Width = 0, u16Height = 0;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstGlobalCacAttr);
    ISP_CHECK_BOOL(pstGlobalCacAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u16Width  = ext_system_be_total_width_read(ViPipe);
    u16Height = ext_system_be_total_height_read(ViPipe);

    if (pstGlobalCacAttr->u16HorCoordinate >= u16Width) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16HorCoordinate!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstGlobalCacAttr->u16VerCoordinate >= u16Height) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16VerCoordinate!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstGlobalCacAttr->s16ParamRedA > 255) || (pstGlobalCacAttr->s16ParamRedA < -256)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s16ParamRedA!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstGlobalCacAttr->s16ParamRedB > 255) || (pstGlobalCacAttr->s16ParamRedB < -256)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s16ParamRedB!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstGlobalCacAttr->s16ParamRedC > 255) || (pstGlobalCacAttr->s16ParamRedC < -256)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s16ParamRedC!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstGlobalCacAttr->s16ParamBlueA > 255) || (pstGlobalCacAttr->s16ParamBlueA < -256)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s16ParamBlueA!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstGlobalCacAttr->s16ParamBlueB > 255) || (pstGlobalCacAttr->s16ParamBlueB < -256)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s16ParamBlueB!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstGlobalCacAttr->s16ParamBlueC > 255) || (pstGlobalCacAttr->s16ParamBlueC < -256)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid s16ParamBlueC!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstGlobalCacAttr->u8VerNormShift > 7) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8VerNormShift!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstGlobalCacAttr->u8VerNormFactor > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8VerNormFactor!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstGlobalCacAttr->u8HorNormShift > 7) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8HorNormShift!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstGlobalCacAttr->u8HorNormFactor > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8HorNormFactor!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstGlobalCacAttr->u16CorVarThr > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16CorVarThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_GlobalCAC_enable_write(ViPipe, pstGlobalCacAttr->bEnable);
    ext_system_GlobalCAC_coor_center_hor_write(ViPipe, pstGlobalCacAttr->u16HorCoordinate);
    ext_system_GlobalCAC_coor_center_ver_write(ViPipe, pstGlobalCacAttr->u16VerCoordinate);
    ext_system_GlobalCAC_param_red_A_write(ViPipe, pstGlobalCacAttr->s16ParamRedA);
    ext_system_GlobalCAC_param_red_B_write(ViPipe, pstGlobalCacAttr->s16ParamRedB);
    ext_system_GlobalCAC_param_red_C_write(ViPipe, pstGlobalCacAttr->s16ParamRedC);
    ext_system_GlobalCAC_param_blue_A_write(ViPipe, pstGlobalCacAttr->s16ParamBlueA);
    ext_system_GlobalCAC_param_blue_B_write(ViPipe, pstGlobalCacAttr->s16ParamBlueB);
    ext_system_GlobalCAC_param_blue_C_write(ViPipe, pstGlobalCacAttr->s16ParamBlueC);

    ext_system_GlobalCAC_y_ns_norm_write(ViPipe, pstGlobalCacAttr->u8VerNormShift);
    ext_system_GlobalCAC_y_nf_norm_write(ViPipe, pstGlobalCacAttr->u8VerNormFactor);
    ext_system_GlobalCAC_x_ns_norm_write(ViPipe, pstGlobalCacAttr->u8HorNormShift);
    ext_system_GlobalCAC_x_nf_norm_write(ViPipe, pstGlobalCacAttr->u8HorNormFactor);

    ext_system_GlobalCAC_cor_thr_write(ViPipe, pstGlobalCacAttr->u16CorVarThr);
    ext_system_GlobalCAC_coef_update_en_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}
GK_S32 MPI_ISP_GetGlobalCacAttr(VI_PIPE ViPipe, ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr)
{
#ifdef CONFIG_ISP_GCAC_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstGlobalCacAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstGlobalCacAttr->bEnable           = ext_system_GlobalCAC_enable_read(ViPipe);
    pstGlobalCacAttr->u16HorCoordinate  = ext_system_GlobalCAC_coor_center_hor_read(ViPipe);
    pstGlobalCacAttr->u16VerCoordinate  = ext_system_GlobalCAC_coor_center_ver_read(ViPipe);
    pstGlobalCacAttr->s16ParamRedA      = ext_system_GlobalCAC_param_red_A_read(ViPipe);
    pstGlobalCacAttr->s16ParamRedB      = ext_system_GlobalCAC_param_red_B_read(ViPipe);
    pstGlobalCacAttr->s16ParamRedC      = ext_system_GlobalCAC_param_red_C_read(ViPipe);
    pstGlobalCacAttr->s16ParamBlueA     = ext_system_GlobalCAC_param_blue_A_read(ViPipe);
    pstGlobalCacAttr->s16ParamBlueB     = ext_system_GlobalCAC_param_blue_B_read(ViPipe);
    pstGlobalCacAttr->s16ParamBlueC     = ext_system_GlobalCAC_param_blue_C_read(ViPipe);
    pstGlobalCacAttr->u8VerNormShift    = ext_system_GlobalCAC_y_ns_norm_read(ViPipe);
    pstGlobalCacAttr->u8VerNormFactor   = ext_system_GlobalCAC_y_nf_norm_read(ViPipe);
    pstGlobalCacAttr->u8HorNormShift    = ext_system_GlobalCAC_x_ns_norm_read(ViPipe);
    pstGlobalCacAttr->u8HorNormFactor   = ext_system_GlobalCAC_x_nf_norm_read(ViPipe);
    pstGlobalCacAttr->u16CorVarThr      = ext_system_GlobalCAC_cor_thr_read(ViPipe);
    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetRcAttr(VI_PIPE ViPipe, const ISP_RC_ATTR_S *pstRcAttr)
{
    return ISP_SetRcAttr(ViPipe, pstRcAttr);
}

GK_S32 MPI_ISP_GetRcAttr(VI_PIPE ViPipe, ISP_RC_ATTR_S *pstRcAttr)
{
    return ISP_GetRcAttr(ViPipe, pstRcAttr);
}

GK_S32 MPI_ISP_SetNRAttr(VI_PIPE ViPipe, const ISP_NR_ATTR_S *pstNRAttr)
{
    GK_U8 i, j;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_BOOL(pstNRAttr->bEnable);
    ISP_CHECK_BOOL(pstNRAttr->bLowPowerEnable);
    ISP_CHECK_BOOL(pstNRAttr->bNrLscEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstNRAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid NR type %d!\n", pstNRAttr->enOpType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_bayernr_enable_write(ViPipe, pstNRAttr->bEnable);
    ext_system_bayernr_manual_mode_write(ViPipe, pstNRAttr->enOpType);
    ext_system_bayernr_low_power_enable_write(ViPipe, pstNRAttr->bLowPowerEnable);
    ext_system_bayernr_lsc_enable_write(ViPipe, pstNRAttr->bNrLscEnable);

    ext_system_bayernr_lsc_nr_ratio_write(ViPipe, pstNRAttr->u8NrLscRatio);

    if (pstNRAttr->u8BnrLscMaxGain > 0xbf) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8BnrLscMaxGain %d!\n", pstNRAttr->u8BnrLscMaxGain);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_bayernr_lsc_max_gain_write(ViPipe, pstNRAttr->u8BnrLscMaxGain);

    if (pstNRAttr->u16BnrLscCmpStrength > 0x100) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16BnrLscCmpStrength %d!\n", pstNRAttr->u16BnrLscCmpStrength);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_bayernr_lsc_cmp_strength_write(ViPipe, pstNRAttr->u16BnrLscCmpStrength);

    for (i = 0; i < ISP_BAYERNR_LUT_LENGTH; i++) {
        if (pstNRAttr->au16CoringRatio[i] > 0x3ff) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16CoringRatio %d!\n", pstNRAttr->au16CoringRatio[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        ext_system_bayernr_coring_ratio_write(ViPipe, i, pstNRAttr->au16CoringRatio[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            if (pstNRAttr->stAuto.au8ChromaStr[j][i] > 3) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid au8ChromaStr[%d][%d] %d!\n", j, i, pstNRAttr->stAuto.au8ChromaStr[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }

            if (pstNRAttr->stAuto.au16CoarseStr[j][i] > 0x360) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid au16CoarseStr[%d][%d] %d!\n", j, i, pstNRAttr->stAuto.au16CoarseStr[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }

        if (pstNRAttr->stAuto.au8FineStr[i] > 128) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8FineStr %d!\n", pstNRAttr->stAuto.au8FineStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNRAttr->stAuto.au16CoringWgt[i] > 3200) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16CoringWgt %d!\n", pstNRAttr->stAuto.au16CoringWgt[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_bayernr_auto_chroma_strength_r_write(ViPipe, i, pstNRAttr->stAuto.au8ChromaStr[0][i]);
        ext_system_bayernr_auto_chroma_strength_gr_write(ViPipe, i, pstNRAttr->stAuto.au8ChromaStr[1][i]);
        ext_system_bayernr_auto_chroma_strength_gb_write(ViPipe, i, pstNRAttr->stAuto.au8ChromaStr[2][i]);
        ext_system_bayernr_auto_chroma_strength_b_write(ViPipe, i, pstNRAttr->stAuto.au8ChromaStr[3][i]);
        ext_system_bayernr_auto_coarse_strength_r_write(ViPipe, i, pstNRAttr->stAuto.au16CoarseStr[0][i]);
        ext_system_bayernr_auto_coarse_strength_gr_write(ViPipe, i, pstNRAttr->stAuto.au16CoarseStr[1][i]);
        ext_system_bayernr_auto_coarse_strength_gb_write(ViPipe, i, pstNRAttr->stAuto.au16CoarseStr[2][i]);
        ext_system_bayernr_auto_coarse_strength_b_write(ViPipe, i, pstNRAttr->stAuto.au16CoarseStr[3][i]);
        ext_system_bayernr_auto_fine_strength_write(ViPipe, i, pstNRAttr->stAuto.au8FineStr[i]);
        ext_system_bayernr_auto_coring_weight_write(ViPipe, i, pstNRAttr->stAuto.au16CoringWgt[i]);
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        if (pstNRAttr->stManual.au8ChromaStr[j] > 3) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8ChromaStr[%d] %d!\n", j, pstNRAttr->stManual.au8ChromaStr[j]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstNRAttr->stManual.au16CoarseStr[j] > 0x360) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16CoarseStr[%d] %d!\n", j, pstNRAttr->stManual.au16CoarseStr[j]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_bayernr_manual_chroma_strength_write(ViPipe, j, pstNRAttr->stManual.au8ChromaStr[j]);
        ext_system_bayernr_manual_coarse_strength_write(ViPipe, j, pstNRAttr->stManual.au16CoarseStr[j]);
    }

    if (pstNRAttr->stManual.u8FineStr > 128) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8FineStr %d!\n", pstNRAttr->stManual.u8FineStr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstNRAttr->stManual.u16CoringWgt > 3200) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16CoringWgt %d!\n", pstNRAttr->stManual.u16CoringWgt);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_bayernr_manual_fine_strength_write(ViPipe, pstNRAttr->stManual.u8FineStr);
    ext_system_bayernr_manual_coring_weight_write(ViPipe, pstNRAttr->stManual.u16CoringWgt);

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        if (pstNRAttr->stWdr.au8WDRFrameStr[i] > 80) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8WDRFrameStr[%d] %d!\n", i, pstNRAttr->stWdr.au8WDRFrameStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        ext_system_bayernr_wdr_frame_strength_write(ViPipe, i, pstNRAttr->stWdr.au8WDRFrameStr[i]);
    }
    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        if (pstNRAttr->stWdr.au8FusionFrameStr[i] > 80) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8FusionFrameStr[%d] %d!\n", i, pstNRAttr->stWdr.au8FusionFrameStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        ext_system_bayernr_fusion_frame_strength_write(ViPipe, i, pstNRAttr->stWdr.au8FusionFrameStr[i]);
    }

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetNRAttr(VI_PIPE ViPipe, ISP_NR_ATTR_S *pstNRAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstNRAttr->bEnable              = ext_system_bayernr_enable_read(ViPipe);
    pstNRAttr->enOpType             = ext_system_bayernr_manual_mode_read(ViPipe);
    pstNRAttr->bLowPowerEnable      = ext_system_bayernr_low_power_enable_read(ViPipe);
    pstNRAttr->bNrLscEnable         = ext_system_bayernr_lsc_enable_read(ViPipe);
    pstNRAttr->u8NrLscRatio         = ext_system_bayernr_lsc_nr_ratio_read(ViPipe);
    pstNRAttr->u8BnrLscMaxGain      = ext_system_bayernr_lsc_max_gain_read(ViPipe);
    pstNRAttr->u16BnrLscCmpStrength = ext_system_bayernr_lsc_cmp_strength_read(ViPipe);

    for (i = 0; i < ISP_BAYERNR_LUT_LENGTH; i++) {
        pstNRAttr->au16CoringRatio[i] = ext_system_bayernr_coring_ratio_read(ViPipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstNRAttr->stAuto.au8ChromaStr[0][i]  = ext_system_bayernr_auto_chroma_strength_r_read(ViPipe, i);
        pstNRAttr->stAuto.au8ChromaStr[1][i]  = ext_system_bayernr_auto_chroma_strength_gr_read(ViPipe, i);
        pstNRAttr->stAuto.au8ChromaStr[2][i]  = ext_system_bayernr_auto_chroma_strength_gb_read(ViPipe, i);
        pstNRAttr->stAuto.au8ChromaStr[3][i]  = ext_system_bayernr_auto_chroma_strength_b_read(ViPipe, i);
        pstNRAttr->stAuto.au16CoarseStr[0][i] = ext_system_bayernr_auto_coarse_strength_r_read(ViPipe, i);
        pstNRAttr->stAuto.au16CoarseStr[1][i] = ext_system_bayernr_auto_coarse_strength_gr_read(ViPipe, i);
        pstNRAttr->stAuto.au16CoarseStr[2][i] = ext_system_bayernr_auto_coarse_strength_gb_read(ViPipe, i);
        pstNRAttr->stAuto.au16CoarseStr[3][i] = ext_system_bayernr_auto_coarse_strength_b_read(ViPipe, i);
        pstNRAttr->stAuto.au8FineStr[i]       = ext_system_bayernr_auto_fine_strength_read(ViPipe, i);
        pstNRAttr->stAuto.au16CoringWgt[i]    = ext_system_bayernr_auto_coring_weight_read(ViPipe, i);
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstNRAttr->stManual.au8ChromaStr[i]  = ext_system_bayernr_manual_chroma_strength_read(ViPipe, i);
        pstNRAttr->stManual.au16CoarseStr[i] = ext_system_bayernr_manual_coarse_strength_read(ViPipe, i);
    }

    pstNRAttr->stManual.u8FineStr        = ext_system_bayernr_manual_fine_strength_read(ViPipe);
    pstNRAttr->stManual.u16CoringWgt     = ext_system_bayernr_manual_coring_weight_read(ViPipe);

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        pstNRAttr->stWdr.au8WDRFrameStr[i]     =  ext_system_bayernr_wdr_frame_strength_read(ViPipe, i);
        pstNRAttr->stWdr.au8FusionFrameStr[i]  =  ext_system_bayernr_fusion_frame_strength_read(ViPipe, i);
    }
    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetDEAttr(VI_PIPE ViPipe, const ISP_DE_ATTR_S *pstDEAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDEAttr);
    ISP_CHECK_BOOL(pstDEAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstDEAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid DE type %d!\n", pstDEAttr->enOpType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_detail_manual_mode_write(ViPipe, pstDEAttr->enOpType);

    ext_system_detail_enable_write(ViPipe, pstDEAttr->bEnable);

    for (i = 0; i < ISP_DE_LUMA_GAIN_LUT_N; i++) {
        if (pstDEAttr->au16LumaGainLut[i] > 0x100) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16LumaGainLut[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        ext_system_detail_luma_gain_lut_write(ViPipe, i, pstDEAttr->au16LumaGainLut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstDEAttr->stAuto.au16GlobalGain[i] > 0x100) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16GlobalGain[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_detail_auto_global_gain_write(ViPipe, i, pstDEAttr->stAuto.au16GlobalGain[i]);

        if (pstDEAttr->stAuto.au16GainLF[i] > 0x20) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16GainLF[%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_detail_auto_gain_lf_write(ViPipe, i, pstDEAttr->stAuto.au16GainLF[i]);

        if (pstDEAttr->stAuto.au16GainHF[i] > 0x20) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16GainHF[%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_detail_auto_gain_hf_write(ViPipe, i, pstDEAttr->stAuto.au16GainHF[i]);
    }

    // de manual mode
    if (pstDEAttr->stManual.u16GlobalGain > 0x100) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16GlobalGain!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_detail_manual_global_gain_write(ViPipe, pstDEAttr->stManual.u16GlobalGain);

    if (pstDEAttr->stManual.u16GainLF > 0x20) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16GainLF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_detail_manual_gain_lf_write(ViPipe, pstDEAttr->stManual.u16GainLF);

    if (pstDEAttr->stManual.u16GainHF > 0x20) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16GainHF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_detail_manual_gain_hf_write(ViPipe, pstDEAttr->stManual.u16GainHF);

    ext_system_detail_attr_update_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDEAttr(VI_PIPE ViPipe, ISP_DE_ATTR_S *pstDEAttr)
{
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDEAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDEAttr->bEnable  = ext_system_detail_enable_read(ViPipe);
    pstDEAttr->enOpType = ext_system_detail_manual_mode_read(ViPipe);

    for (i = 0; i < ISP_DE_LUMA_GAIN_LUT_N; i++) {
        pstDEAttr->au16LumaGainLut[i] = ext_system_detail_luma_gain_lut_read(ViPipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstDEAttr->stAuto.au16GlobalGain[i] = ext_system_detail_auto_global_gain_read(ViPipe, i);
        pstDEAttr->stAuto.au16GainLF[i]     = ext_system_detail_auto_gain_lf_read(ViPipe, i);
        pstDEAttr->stAuto.au16GainHF[i]     = ext_system_detail_auto_gain_hf_read(ViPipe, i);
    }

    pstDEAttr->stManual.u16GlobalGain = ext_system_detail_manual_global_gain_read(ViPipe);
    pstDEAttr->stManual.u16GainLF     = ext_system_detail_manual_gain_lf_read(ViPipe);
    pstDEAttr->stManual.u16GainHF     = ext_system_detail_manual_gain_hf_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetRGBIRAttr(VI_PIPE ViPipe, const ISP_RGBIR_ATTR_S *pstRGBIRAttr)
{
    return ISP_SetRGBIRAttr(ViPipe, pstRGBIRAttr);
}

GK_S32 MPI_ISP_GetRGBIRAttr(VI_PIPE ViPipe, ISP_RGBIR_ATTR_S *pstRGBIRAttr)
{
    return ISP_GetRGBIRAttr(ViPipe, pstRGBIRAttr);
}
GK_S32 MPI_ISP_SetColorToneAttr(VI_PIPE ViPipe, const ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCTAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if ((pstCTAttr->u16RedCastGain < 0x100) || (pstCTAttr->u16RedCastGain > 0x180)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16RedCastGain! Should in range of [0x100, 0x180]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstCTAttr->u16GreenCastGain < 0x100) || (pstCTAttr->u16GreenCastGain > 0x180)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16GreenCastGain! Should in range of [0x100, 0x180]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstCTAttr->u16BlueCastGain < 0x100) || (pstCTAttr->u16BlueCastGain > 0x180)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16BlueCastGain! Should in range of [0x100, 0x180]\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_cc_colortone_rgain_write(ViPipe, pstCTAttr->u16RedCastGain);
    ext_system_cc_colortone_ggain_write(ViPipe, pstCTAttr->u16GreenCastGain);
    ext_system_cc_colortone_bgain_write(ViPipe, pstCTAttr->u16BlueCastGain);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetColorToneAttr(VI_PIPE ViPipe, ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCTAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstCTAttr->u16RedCastGain   = ext_system_cc_colortone_rgain_read(ViPipe);
    pstCTAttr->u16GreenCastGain = ext_system_cc_colortone_ggain_read(ViPipe);
    pstCTAttr->u16BlueCastGain  = ext_system_cc_colortone_bgain_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetIspSharpenAttr(VI_PIPE ViPipe, const ISP_SHARPEN_ATTR_S *pstIspShpAttr)
{
    GK_U8 i, j;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspShpAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_BOOL(pstIspShpAttr->bEnable);
    if (pstIspShpAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Sharpen Type %d!\n", pstIspShpAttr->enOpType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_manual_isp_sharpen_en_write(ViPipe, pstIspShpAttr->bEnable);
    ext_system_isp_sharpen_manu_mode_write(ViPipe, pstIspShpAttr->enOpType);
    ext_system_manual_Isp_sharpen_SkinUmax_write(ViPipe, pstIspShpAttr->u8SkinUmax);
    ext_system_manual_Isp_sharpen_SkinVmax_write(ViPipe, pstIspShpAttr->u8SkinVmax);
    ext_system_manual_Isp_sharpen_SkinUmin_write(ViPipe, pstIspShpAttr->u8SkinUmin);
    ext_system_manual_Isp_sharpen_SkinVmin_write(ViPipe, pstIspShpAttr->u8SkinVmin);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (pstIspShpAttr->stAuto.au16TextureStr[j][i] > 4095) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au16TextureStr:%d! Value range:[0, 4095]\n",
                          pstIspShpAttr->stAuto.au16TextureStr[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
            if (pstIspShpAttr->stAuto.au16EdgeStr[j][i] > 4095) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au16EdgeStr:%d! Value range:[0, 4095]\n",
                          pstIspShpAttr->stAuto.au16EdgeStr[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            if (pstIspShpAttr->stAuto.au8LumaWgt[j][i] > ISP_SHARPEN_LUMAWGT_MAX) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8LumaWgt:%d! Value range:[0, %d]\n", pstIspShpAttr->stAuto.au8LumaWgt[j][i],
                          ISP_SHARPEN_LUMAWGT_MAX);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }
        if (pstIspShpAttr->stAuto.au16TextureFreq[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au16TextureFreq:%d! Value range:[0, 4095]\n",
                      pstIspShpAttr->stAuto.au16TextureFreq[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstIspShpAttr->stAuto.au16EdgeFreq[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au16EdgeFreq:%d! Value range:[0, 4095]\n", pstIspShpAttr->stAuto.au16EdgeFreq[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstIspShpAttr->stAuto.au8OverShoot[i] > 127) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8OverShoot:%d! Value range:[0, 127]\n", pstIspShpAttr->stAuto.au8OverShoot[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstIspShpAttr->stAuto.au8UnderShoot[i] > 127) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8UnderShoot:%d! Value range:[0, 127]\n",
                      pstIspShpAttr->stAuto.au8UnderShoot[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8EdgeFiltStr[i] > 63) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8EdgeFiltStr:%d! Value range:[0, 63]\n",
                      pstIspShpAttr->stAuto.au8EdgeFiltStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8EdgeFiltMaxCap[i] > ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8EdgeFiltMaxCap:%d! Value range:[0, %d]\n", pstIspShpAttr->stAuto.au8EdgeFiltMaxCap[i],
                      ISP_SHARPEN_EdgeFiltMaxCap_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8RGain[i] > ISP_SHARPEN_RGAIN_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8RGain:%d! Value range:[0, %d]\n", pstIspShpAttr->stAuto.au8RGain[i],
                      ISP_SHARPEN_RGAIN_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8GGain[i] > ISP_SHARPEN_GGAIN_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8GGain:%d! Value range:[0, %d]\n", pstIspShpAttr->stAuto.au8GGain[i],
                      ISP_SHARPEN_GGAIN_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8BGain[i] > ISP_SHARPEN_BGAIN_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8BGain:%d! Value range:0, %d]\n", pstIspShpAttr->stAuto.au8BGain[i],
                      ISP_SHARPEN_BGAIN_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8SkinGain[i] > 31) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8SkinGain:%d! Value range:[0, 31]\n", pstIspShpAttr->stAuto.au8SkinGain[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au8ShootSupAdj[i] > 15) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8ShootSupAdj:%d! Value range:[0, 15]\n",
                      pstIspShpAttr->stAuto.au8ShootSupAdj[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstIspShpAttr->stAuto.au16MaxSharpGain[i] > 0x7FF) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au16MaxSharpGain:%d! Value range:[0, 0x7FF]\n",
                      pstIspShpAttr->stAuto.au16MaxSharpGain[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstIspShpAttr->stAuto.au8WeakDetailGain[i] > 0x7F) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stAuto.au8WeakDetailGain:%d! Value range:[0, 0x7F]\n",
                      pstIspShpAttr->stAuto.au8WeakDetailGain[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            ext_system_Isp_sharpen_TextureStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, pstIspShpAttr->stAuto.au16TextureStr[j][i]);
            ext_system_Isp_sharpen_EdgeStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, pstIspShpAttr->stAuto.au16EdgeStr[j][i]);
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            ext_system_Isp_sharpen_LumaWgt_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, pstIspShpAttr->stAuto.au8LumaWgt[j][i]);
        }

        ext_system_Isp_sharpen_TextureFreq_write(ViPipe, i, pstIspShpAttr->stAuto.au16TextureFreq[i]);
        ext_system_Isp_sharpen_EdgeFreq_write(ViPipe, i, pstIspShpAttr->stAuto.au16EdgeFreq[i]);
        ext_system_Isp_sharpen_OverShoot_write(ViPipe, i, pstIspShpAttr->stAuto.au8OverShoot[i]);
        ext_system_Isp_sharpen_UnderShoot_write(ViPipe, i, pstIspShpAttr->stAuto.au8UnderShoot[i]);
        ext_system_Isp_sharpen_shootSupStr_write(ViPipe, i, pstIspShpAttr->stAuto.au8ShootSupStr[i]);
        ext_system_Isp_sharpen_detailctrl_write(ViPipe, i, pstIspShpAttr->stAuto.au8DetailCtrl[i]);
        ext_system_Isp_sharpen_EdgeFiltStr_write(ViPipe, i, pstIspShpAttr->stAuto.au8EdgeFiltStr[i]);
        ext_system_Isp_sharpen_EdgeFiltMaxCap_write(ViPipe, i, pstIspShpAttr->stAuto.au8EdgeFiltMaxCap[i]);
        ext_system_Isp_sharpen_RGain_write(ViPipe, i, pstIspShpAttr->stAuto.au8RGain[i]);
        ext_system_Isp_sharpen_GGain_write(ViPipe, i, pstIspShpAttr->stAuto.au8GGain[i]);
        ext_system_Isp_sharpen_BGain_write(ViPipe, i, pstIspShpAttr->stAuto.au8BGain[i]);
        ext_system_Isp_sharpen_SkinGain_write(ViPipe, i, pstIspShpAttr->stAuto.au8SkinGain[i]);
        ext_system_Isp_sharpen_ShootSupAdj_write(ViPipe, i, pstIspShpAttr->stAuto.au8ShootSupAdj[i]);
        ext_system_Isp_sharpen_detailctrlThr_write(ViPipe, i, pstIspShpAttr->stAuto.au8DetailCtrlThr[i]);
        ext_system_Isp_sharpen_MaxSharpGain_write(ViPipe, i, pstIspShpAttr->stAuto.au16MaxSharpGain[i]);
        ext_system_Isp_sharpen_WeakDetailGain_write(ViPipe, i, pstIspShpAttr->stAuto.au8WeakDetailGain[i]);
    }
    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        if (pstIspShpAttr->stManual.au16TextureStr[j] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.au16TextureStr:%d! Value range:[0, 4095]\n",
                      pstIspShpAttr->stManual.au16TextureStr[j]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstIspShpAttr->stManual.au16EdgeStr[j] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.au16EdgeStr:%d! Value range:[0, 4095]\n",
                      pstIspShpAttr->stManual.au16EdgeStr[j]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        if (pstIspShpAttr->stManual.au8LumaWgt[j] > ISP_SHARPEN_LUMAWGT_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.au8LumaWgt:%d! Value range:[0, %d]\n", pstIspShpAttr->stManual.au8LumaWgt[j],
                      ISP_SHARPEN_LUMAWGT_MAX);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }
    if (pstIspShpAttr->stManual.u16TextureFreq > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u16TextureFreq:%d! Value range:[0, 4095]\n",
                  pstIspShpAttr->stManual.u16TextureFreq);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u16EdgeFreq > 4095) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u16EdgeFreq:%d! Value range:[0, 4095]\n", pstIspShpAttr->stManual.u16EdgeFreq);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8OverShoot > 127) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8OverShoot:%d! Value range:[0, 127]\n", pstIspShpAttr->stManual.u8OverShoot);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8UnderShoot > 127) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8UnderShoot:%d! Value range:[0, 127]\n", pstIspShpAttr->stManual.u8UnderShoot);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8EdgeFiltStr > 63) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8EdgeFiltStr:%d! Value range:[0, 63]\n",
                  pstIspShpAttr->stManual.u8EdgeFiltStr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8EdgeFiltMaxCap > ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8EdgeFiltMaxCap:%d! Value range:[0, %d]\n", pstIspShpAttr->stManual.u8EdgeFiltMaxCap,
                  ISP_SHARPEN_EdgeFiltMaxCap_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8RGain > ISP_SHARPEN_RGAIN_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8RGain:%d! Value range:[0, %d]\n", pstIspShpAttr->stManual.u8RGain,
                  ISP_SHARPEN_RGAIN_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8GGain > ISP_SHARPEN_GGAIN_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8GGain:%d! Value range:[0, %d]\n", pstIspShpAttr->stManual.u8GGain,
                  ISP_SHARPEN_GGAIN_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8BGain > ISP_SHARPEN_BGAIN_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8BGain:%d! Value range:[0, %d]\n", pstIspShpAttr->stManual.u8BGain,
                  ISP_SHARPEN_BGAIN_MAX);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8SkinGain > 31) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8SkinGain:%d! Value range:[0, 31]\n", pstIspShpAttr->stManual.u8SkinGain);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u8ShootSupAdj > 15) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8ShootSupAdj:%d! Value range:[0, 15]\n",
                  pstIspShpAttr->stManual.u8ShootSupAdj);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstIspShpAttr->stManual.u16MaxSharpGain > 0x7FF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u16MaxSharpGain:%d! Value range:[0, 0x7FF]\n",
                  pstIspShpAttr->stManual.u16MaxSharpGain);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstIspShpAttr->stManual.u8WeakDetailGain > 0x7F) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid stManual.u8WeakDetailGain:%d! Value range:[0, 0x7F]\n",
                  pstIspShpAttr->stManual.u8WeakDetailGain);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        ext_system_manual_Isp_sharpen_TextureStr_write(ViPipe, j, pstIspShpAttr->stManual.au16TextureStr[j]);
        ext_system_manual_Isp_sharpen_EdgeStr_write(ViPipe, j, pstIspShpAttr->stManual.au16EdgeStr[j]);
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        ext_system_manual_Isp_sharpen_LumaWgt_write(ViPipe, j, pstIspShpAttr->stManual.au8LumaWgt[j]);
    }
    ext_system_manual_Isp_sharpen_TextureFreq_write(ViPipe, pstIspShpAttr->stManual.u16TextureFreq);
    ext_system_manual_Isp_sharpen_EdgeFreq_write(ViPipe, pstIspShpAttr->stManual.u16EdgeFreq);
    ext_system_manual_Isp_sharpen_OverShoot_write(ViPipe, pstIspShpAttr->stManual.u8OverShoot);
    ext_system_manual_Isp_sharpen_UnderShoot_write(ViPipe, pstIspShpAttr->stManual.u8UnderShoot);
    ext_system_manual_Isp_sharpen_shootSupStr_write(ViPipe, pstIspShpAttr->stManual.u8ShootSupStr);
    ext_system_manual_Isp_sharpen_detailctrl_write(ViPipe, pstIspShpAttr->stManual.u8DetailCtrl);
    ext_system_manual_Isp_sharpen_EdgeFiltStr_write(ViPipe, pstIspShpAttr->stManual.u8EdgeFiltStr);
    ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(ViPipe, pstIspShpAttr->stManual.u8EdgeFiltMaxCap);
    ext_system_manual_Isp_sharpen_RGain_write(ViPipe, pstIspShpAttr->stManual.u8RGain);
    ext_system_manual_Isp_sharpen_GGain_write(ViPipe, pstIspShpAttr->stManual.u8GGain);
    ext_system_manual_Isp_sharpen_BGain_write(ViPipe, pstIspShpAttr->stManual.u8BGain);
    ext_system_manual_Isp_sharpen_SkinGain_write(ViPipe, pstIspShpAttr->stManual.u8SkinGain);
    ext_system_manual_Isp_sharpen_ShootSupAdj_write(ViPipe, pstIspShpAttr->stManual.u8ShootSupAdj);
    ext_system_manual_Isp_sharpen_MaxSharpGain_write(ViPipe, pstIspShpAttr->stManual.u16MaxSharpGain);
    ext_system_manual_Isp_sharpen_detailctrlThr_write(ViPipe, pstIspShpAttr->stManual.u8DetailCtrlThr);
    ext_system_manual_Isp_sharpen_WeakDetailGain_write(ViPipe, pstIspShpAttr->stManual.u8WeakDetailGain);

    ext_system_sharpen_mpi_update_en_write(ViPipe, GK_TRUE);
    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetIspSharpenAttr(VI_PIPE ViPipe, ISP_SHARPEN_ATTR_S *pstIspShpAttr)
{
    GK_U8 i, j;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspShpAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstIspShpAttr->bEnable = ext_system_manual_isp_sharpen_en_read(ViPipe);
    pstIspShpAttr->enOpType = (ISP_OP_TYPE_E)ext_system_isp_sharpen_manu_mode_read(ViPipe);

    pstIspShpAttr->u8SkinUmax = ext_system_manual_Isp_sharpen_SkinUmax_read(ViPipe);
    pstIspShpAttr->u8SkinVmax = ext_system_manual_Isp_sharpen_SkinVmax_read(ViPipe);
    pstIspShpAttr->u8SkinUmin = ext_system_manual_Isp_sharpen_SkinUmin_read(ViPipe);
    pstIspShpAttr->u8SkinVmin = ext_system_manual_Isp_sharpen_SkinVmin_read(ViPipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            pstIspShpAttr->stAuto.au16TextureStr[j][i] = ext_system_Isp_sharpen_TextureStr_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
            pstIspShpAttr->stAuto.au16EdgeStr[j][i]    = ext_system_Isp_sharpen_EdgeStr_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            pstIspShpAttr->stAuto.au8LumaWgt[j][i]     = ext_system_Isp_sharpen_LumaWgt_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
        }
        pstIspShpAttr->stAuto.au16TextureFreq[i]  = ext_system_Isp_sharpen_TextureFreq_read(ViPipe, i);
        pstIspShpAttr->stAuto.au16EdgeFreq[i]     = ext_system_Isp_sharpen_EdgeFreq_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8OverShoot[i]     = ext_system_Isp_sharpen_OverShoot_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8UnderShoot[i]    = ext_system_Isp_sharpen_UnderShoot_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8ShootSupStr[i]   = ext_system_Isp_sharpen_shootSupStr_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8DetailCtrl[i]    = ext_system_Isp_sharpen_detailctrl_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8EdgeFiltStr[i]   = ext_system_Isp_sharpen_EdgeFiltStr_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8EdgeFiltMaxCap[i] = ext_system_Isp_sharpen_EdgeFiltMaxCap_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8RGain[i]         = ext_system_Isp_sharpen_RGain_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8GGain[i]         = ext_system_Isp_sharpen_GGain_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8BGain[i]         = ext_system_Isp_sharpen_BGain_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8SkinGain[i]      = ext_system_Isp_sharpen_SkinGain_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8ShootSupAdj[i]   = ext_system_Isp_sharpen_ShootSupAdj_read(ViPipe, i);
        pstIspShpAttr->stAuto.au16MaxSharpGain[i] = ext_system_Isp_sharpen_MaxSharpGain_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8DetailCtrlThr[i] = ext_system_Isp_sharpen_detailctrlThr_read(ViPipe, i);
        pstIspShpAttr->stAuto.au8WeakDetailGain[i] = ext_system_Isp_sharpen_WeakDetailGain_read(ViPipe, i);
    }

    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        pstIspShpAttr->stManual.au16TextureStr[j] = ext_system_manual_Isp_sharpen_TextureStr_read(ViPipe, j);
        pstIspShpAttr->stManual.au16EdgeStr[j]    = ext_system_manual_Isp_sharpen_EdgeStr_read(ViPipe, j);
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        pstIspShpAttr->stManual.au8LumaWgt[j] = ext_system_manual_Isp_sharpen_LumaWgt_read(ViPipe, j);
    }
    pstIspShpAttr->stManual.u16TextureFreq = ext_system_manual_Isp_sharpen_TextureFreq_read(ViPipe);
    pstIspShpAttr->stManual.u16EdgeFreq    = ext_system_manual_Isp_sharpen_EdgeFreq_read(ViPipe);
    pstIspShpAttr->stManual.u8OverShoot    = ext_system_manual_Isp_sharpen_OverShoot_read(ViPipe);
    pstIspShpAttr->stManual.u8UnderShoot   = ext_system_manual_Isp_sharpen_UnderShoot_read(ViPipe);
    pstIspShpAttr->stManual.u8ShootSupStr  = ext_system_manual_Isp_sharpen_shootSupStr_read(ViPipe);
    pstIspShpAttr->stManual.u8DetailCtrl   = ext_system_manual_Isp_sharpen_detailctrl_read(ViPipe);
    pstIspShpAttr->stManual.u8EdgeFiltStr  = ext_system_manual_Isp_sharpen_EdgeFiltStr_read(ViPipe);
    pstIspShpAttr->stManual.u8EdgeFiltMaxCap = ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_read(ViPipe);
    pstIspShpAttr->stManual.u8RGain        = ext_system_manual_Isp_sharpen_RGain_read(ViPipe);
    pstIspShpAttr->stManual.u8GGain        = ext_system_manual_Isp_sharpen_GGain_read(ViPipe);
    pstIspShpAttr->stManual.u8BGain        = ext_system_manual_Isp_sharpen_BGain_read(ViPipe);
    pstIspShpAttr->stManual.u8SkinGain     = ext_system_manual_Isp_sharpen_SkinGain_read(ViPipe);
    pstIspShpAttr->stManual.u8ShootSupAdj   = ext_system_manual_Isp_sharpen_ShootSupAdj_read(ViPipe);
    pstIspShpAttr->stManual.u16MaxSharpGain = ext_system_manual_Isp_sharpen_MaxSharpGain_read(ViPipe);
    pstIspShpAttr->stManual.u8DetailCtrlThr = ext_system_manual_Isp_sharpen_detailctrlThr_read(ViPipe);
    pstIspShpAttr->stManual.u8WeakDetailGain = ext_system_manual_Isp_sharpen_WeakDetailGain_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetIspEdgeMarkAttr(VI_PIPE ViPipe, const ISP_EDGEMARK_ATTR_S *pstIspEdgeMarkAttr)
{
#ifdef CONFIG_ISP_EDGEMARK_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspEdgeMarkAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_BOOL(pstIspEdgeMarkAttr->bEnable);

    if (pstIspEdgeMarkAttr->u32Color > 0xFFFFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u32Color:%d! Value range:[0, 0xFFFFFF]\n", pstIspEdgeMarkAttr->u32Color);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_manual_isp_edgemark_en_write(ViPipe, pstIspEdgeMarkAttr->bEnable);
    ext_system_manual_isp_edgemark_color_write(ViPipe, pstIspEdgeMarkAttr->u32Color);
    ext_system_manual_isp_edgemark_threshold_write(ViPipe, pstIspEdgeMarkAttr->u8Threshold);

    ext_system_edgemark_mpi_update_en_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetIspEdgeMarkAttr(VI_PIPE ViPipe, ISP_EDGEMARK_ATTR_S *pstIspEdgeMarkAttr)
{
#ifdef CONFIG_ISP_EDGEMARK_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspEdgeMarkAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstIspEdgeMarkAttr->bEnable     = ext_system_manual_isp_edgemark_en_read(ViPipe);
    pstIspEdgeMarkAttr->u32Color    = ext_system_manual_isp_edgemark_color_read(ViPipe);
    pstIspEdgeMarkAttr->u8Threshold = ext_system_manual_isp_edgemark_threshold_read(ViPipe);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

// High Light Constraint
GK_S32 MPI_ISP_SetIspHlcAttr(VI_PIPE ViPipe, const ISP_HLC_ATTR_S *pstIspHlcAttr)
{
#ifdef CONFIG_ISP_HLC_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspHlcAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_BOOL(pstIspHlcAttr->bEnable);

    ext_system_manual_isp_hlc_en_write(ViPipe, pstIspHlcAttr->bEnable);
    ext_system_manual_isp_hlc_lumathr_write(ViPipe, pstIspHlcAttr->u8LumaThr);
    ext_system_manual_isp_hlc_lumatarget_write(ViPipe, pstIspHlcAttr->u8LumaTarget);

    ext_system_hlc_mpi_update_en_write(ViPipe, GK_TRUE);
    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetIspHlcAttr(VI_PIPE ViPipe, ISP_HLC_ATTR_S *pstIspHlcAttr)
{
#ifdef CONFIG_ISP_HLC_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspHlcAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstIspHlcAttr->bEnable      = ext_system_manual_isp_hlc_en_read(ViPipe);
    pstIspHlcAttr->u8LumaThr    = ext_system_manual_isp_hlc_lumathr_read(ViPipe);
    pstIspHlcAttr->u8LumaTarget = ext_system_manual_isp_hlc_lumatarget_read(ViPipe);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

/* Crosstalk Removal Strength */
GK_S32 MPI_ISP_SetCrosstalkAttr(VI_PIPE ViPipe, const ISP_CR_ATTR_S *pstCRAttr)
{
#ifdef CONFIG_ISP_CR_SUPPORT
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_BOOL(pstCRAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstCRAttr->u8Slope > ISP_CR_SLOPE_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid slope %d!\n", pstCRAttr->u8Slope);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstCRAttr->u8SensiSlope > ISP_CR_SLOPE_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Sensitivity %d!\n", pstCRAttr->u8SensiSlope);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstCRAttr->u16SensiThr > ISP_CR_THR_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid SensiThreshold %d!\n", pstCRAttr->u16SensiThr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_ge_enable_write(ViPipe, pstCRAttr->bEnable);
    ext_system_ge_slope_write(ViPipe, pstCRAttr->u8Slope);
    ext_system_ge_sensitivity_write(ViPipe, pstCRAttr->u8SensiSlope);
    ext_system_ge_sensithreshold_write(ViPipe, pstCRAttr->u16SensiThr);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCRAttr->au16Strength[i] > 0x100) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Strength %d!\n", pstCRAttr->au16Strength[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ge_strength_write(ViPipe, i, pstCRAttr->au16Strength[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCRAttr->au16NpOffset[i] > ISP_CR_NPOFFSET_MAX || pstCRAttr->au16NpOffset[i] < ISP_CR_NPOFFSET_MIN) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid NpOffset %d!\n", pstCRAttr->au16NpOffset[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ge_npoffset_write(ViPipe, i, pstCRAttr->au16NpOffset[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCRAttr->au16Threshold[i] > ISP_CR_THR_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Threshold %d!\n", pstCRAttr->au16Threshold[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ge_threshold_write(ViPipe, i, pstCRAttr->au16Threshold[i]);
    }

    ext_system_ge_coef_update_en_write(ViPipe, GK_TRUE);
    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetCrosstalkAttr(VI_PIPE ViPipe, ISP_CR_ATTR_S *pstCRAttr)
{
#ifdef CONFIG_ISP_CR_SUPPORT
    GK_U8 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstCRAttr->bEnable      = ext_system_ge_enable_read(ViPipe);
    pstCRAttr->u8Slope      = ext_system_ge_slope_read(ViPipe);
    pstCRAttr->u8SensiSlope = ext_system_ge_sensitivity_read(ViPipe);
    pstCRAttr->u16SensiThr  = ext_system_ge_sensithreshold_read(ViPipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstCRAttr->au16Strength[i]  = ext_system_ge_strength_read(ViPipe, i);
        pstCRAttr->au16NpOffset[i]  = ext_system_ge_npoffset_read(ViPipe, i);
        pstCRAttr->au16Threshold[i] = ext_system_ge_threshold_read(ViPipe, i);
    }

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetAntiFalseColorAttr(VI_PIPE ViPipe, const ISP_ANTIFALSECOLOR_ATTR_S *pstAntiFalseColor)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstAntiFalseColor);
    ISP_CHECK_BOOL(pstAntiFalseColor->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstAntiFalseColor->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType %d!\n", pstAntiFalseColor->enOpType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_antifalsecolor_enable_write(ViPipe, pstAntiFalseColor->bEnable);
    ext_system_antifalsecolor_manual_mode_write(ViPipe, pstAntiFalseColor->enOpType);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i] > 0x20) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8AntiFalseColorThreshold %d!\n",
                      pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i] > 0x1f) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8AntiFalseColorStrength %d!\n",
                      pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        ext_system_antifalsecolor_auto_threshold_write(ViPipe, i, pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i]);
        ext_system_antifalsecolor_auto_strenght_write(ViPipe, i, pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i]);
    }

    if (pstAntiFalseColor->stManual.u8AntiFalseColorThreshold > 0x20) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8AntiFalseColorThreshold %d!\n", pstAntiFalseColor->stManual.u8AntiFalseColorThreshold);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstAntiFalseColor->stManual.u8AntiFalseColorStrength > 0x1f) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8AntiFalseColorStrength %d!\n", pstAntiFalseColor->stManual.u8AntiFalseColorStrength);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    ext_system_antifalsecolor_manual_threshold_write(ViPipe, pstAntiFalseColor->stManual.u8AntiFalseColorThreshold);
    ext_system_antifalsecolor_manual_strenght_write(ViPipe, pstAntiFalseColor->stManual.u8AntiFalseColorStrength);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetAntiFalseColorAttr(VI_PIPE ViPipe, ISP_ANTIFALSECOLOR_ATTR_S *pstAntiFalseColor)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstAntiFalseColor);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstAntiFalseColor->bEnable  = ext_system_antifalsecolor_enable_read(ViPipe);
    pstAntiFalseColor->enOpType = ext_system_antifalsecolor_manual_mode_read(ViPipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i] = ext_system_antifalsecolor_auto_threshold_read(ViPipe, i);
        pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i]  = ext_system_antifalsecolor_auto_strenght_read(ViPipe, i);
    }

    pstAntiFalseColor->stManual.u8AntiFalseColorThreshold = ext_system_antifalsecolor_manual_threshold_read(ViPipe);
    pstAntiFalseColor->stManual.u8AntiFalseColorStrength  = ext_system_antifalsecolor_manual_strenght_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetDemosaicAttr(VI_PIPE ViPipe, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_BOOL(pstDemosaicAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstDemosaicAttr->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType %d!\n", pstDemosaicAttr->enOpType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_demosaic_enable_write(ViPipe, pstDemosaicAttr->bEnable);
    ext_system_demosaic_manual_mode_write(ViPipe, pstDemosaicAttr->enOpType);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstDemosaicAttr->stAuto.au8NonDirMFDetailEhcStr[i] > ISP_DEMOSAIC_NONDIR_MFDETALEHC_STR_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8NonDirMFDetailEhcStr %d!\n", pstDemosaicAttr->stAuto.au8NonDirMFDetailEhcStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstDemosaicAttr->stAuto.au8NonDirHFDetailEhcStr[i] > 0x10) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8NonDirHFDetailEhcStr %d!\n", pstDemosaicAttr->stAuto.au8NonDirHFDetailEhcStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if ((pstDemosaicAttr->stAuto.au8DetailSmoothRange[i] > ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MAX) ||
            (pstDemosaicAttr->stAuto.au8DetailSmoothRange[i] < ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MIN)) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8DetailSmoothRange %d!\n", pstDemosaicAttr->stAuto.au8DetailSmoothRange[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstDemosaicAttr->stAuto.au16DetailSmoothStr[i] > 0x100) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16DetailSmoothStr %d!\n", pstDemosaicAttr->stAuto.au16DetailSmoothStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
		if (pstDemosaicAttr->stAuto.au8ColorNoiseThdY[i] > 15) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8ColorNoiseThdY %d!\n", pstDemosaicAttr->stAuto.au8ColorNoiseThdY[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
	    if (pstDemosaicAttr->stAuto.au8ColorNoiseStrY[i] > 63) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8ColorNoiseStrY %d!\n", pstDemosaicAttr->stAuto.au8ColorNoiseStrY[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
		if (pstDemosaicAttr->stAuto.au8ColorNoiseStrF[i] > 0xF) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8ColorNoiseStrF %d!\n", pstDemosaicAttr->stAuto.au8ColorNoiseStrF[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_demosaic_auto_nondirection_strength_write(ViPipe, i, pstDemosaicAttr->stAuto.au8NonDirStr[i]);
        ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write(ViPipe, i, pstDemosaicAttr->stAuto.au8NonDirMFDetailEhcStr[i]);
        ext_system_demosaic_auto_nondirectiongfreq_detailenhance_strength_write(ViPipe, i, pstDemosaicAttr->stAuto.au8NonDirHFDetailEhcStr[i]);
        ext_system_demosaic_auto_detail_smooth_range_write(ViPipe, i, pstDemosaicAttr->stAuto.au8DetailSmoothRange[i]);
        ext_system_demosaic_auto_detail_smooth_strength_write(ViPipe, i, pstDemosaicAttr->stAuto.au16DetailSmoothStr[i]);
        ext_system_demosaic_auto_desat_dark_range_write(ViPipe, i, pstDemosaicAttr->stAuto.au8ColorNoiseThdY[i]);
        ext_system_demosaic_auto_desat_dark_strength_write(ViPipe, i, pstDemosaicAttr->stAuto.au8ColorNoiseStrY[i]);
		ext_system_demosaic_auto_colornoise_thdf_write(ViPipe, i, pstDemosaicAttr->stAuto.au8ColorNoiseThdF[i]);
        ext_system_demosaic_auto_colornoise_strf_write(ViPipe, i, pstDemosaicAttr->stAuto.au8ColorNoiseStrF[i]);
    }

    if (pstDemosaicAttr->stManual.u8NonDirMFDetailEhcStr > ISP_DEMOSAIC_NONDIR_MFDETALEHC_STR_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8NonDirMFDetailEhcStr %d!\n", pstDemosaicAttr->stManual.u8NonDirMFDetailEhcStr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->stManual.u8NonDirHFDetailEhcStr > 0x10) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8NonDirHFDetailEhcStr %d!\n", pstDemosaicAttr->stManual.u8NonDirHFDetailEhcStr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if ((pstDemosaicAttr->stManual.u8DetailSmoothRange > ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MAX) ||
        (pstDemosaicAttr->stManual.u8DetailSmoothRange < ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MIN)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8DetailSmoothRange %d!\n", pstDemosaicAttr->stManual.u8DetailSmoothRange);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->stManual.u16DetailSmoothStr > 0x100) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u16DetailSmoothStr %d!\n", pstDemosaicAttr->stManual.u16DetailSmoothStr);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->stManual.u8ColorNoiseThdY > 15) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8ColorNoiseThdY %d!\n", pstDemosaicAttr->stManual.u8ColorNoiseThdY);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->stManual.u8ColorNoiseStrY > 63) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8ColorNoiseStrY %d!\n", pstDemosaicAttr->stManual.u8ColorNoiseStrY);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
	if (pstDemosaicAttr->stManual.u8ColorNoiseStrF > 0xF) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u8ColorNoiseStrF %d!\n", pstDemosaicAttr->stManual.u8ColorNoiseStrF);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_demosaic_manual_nondirection_strength_write(ViPipe, pstDemosaicAttr->stManual.u8NonDirStr);
    ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write(ViPipe, pstDemosaicAttr->stManual.u8NonDirMFDetailEhcStr);
    ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_write(ViPipe, pstDemosaicAttr->stManual.u8NonDirHFDetailEhcStr);
    ext_system_demosaic_manual_detail_smooth_range_write(ViPipe, pstDemosaicAttr->stManual.u8DetailSmoothRange);
    ext_system_demosaic_manual_detail_smooth_strength_write(ViPipe, pstDemosaicAttr->stManual.u16DetailSmoothStr);
	ext_system_demosaic_manual_desat_dark_range_write(ViPipe, pstDemosaicAttr->stManual.u8ColorNoiseThdY);
	ext_system_demosaic_manual_desat_dark_strength_write(ViPipe, pstDemosaicAttr->stManual.u8ColorNoiseStrY);
    ext_system_demosaic_manual_colornoise_thdf_write(ViPipe, pstDemosaicAttr->stManual.u8ColorNoiseThdF);
    ext_system_demosaic_manual_colornoise_strf_write(ViPipe, pstDemosaicAttr->stManual.u8ColorNoiseStrF);
    ext_system_demosaic_attr_update_en_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDemosaicAttr(VI_PIPE ViPipe, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDemosaicAttr->bEnable  = ext_system_demosaic_enable_read(ViPipe);
    pstDemosaicAttr->enOpType = ext_system_demosaic_manual_mode_read(ViPipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstDemosaicAttr->stAuto.au8NonDirStr[i]            = ext_system_demosaic_auto_nondirection_strength_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au8NonDirMFDetailEhcStr[i] = ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au8NonDirHFDetailEhcStr[i] = ext_system_demosaic_auto_nondirectiongfreq_detailenhance_strength_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au8DetailSmoothRange[i]    = ext_system_demosaic_auto_detail_smooth_range_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au16DetailSmoothStr[i]     = ext_system_demosaic_auto_detail_smooth_strength_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au8ColorNoiseThdY[i]       = ext_system_demosaic_auto_desat_dark_range_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au8ColorNoiseStrY[i]       = ext_system_demosaic_auto_desat_dark_strength_read(ViPipe, i);
		pstDemosaicAttr->stAuto.au8ColorNoiseThdF[i]       = ext_system_demosaic_auto_colornoise_thdf_read(ViPipe, i);
        pstDemosaicAttr->stAuto.au8ColorNoiseStrF[i]       = ext_system_demosaic_auto_colornoise_strf_read(ViPipe, i);
    }

    pstDemosaicAttr->stManual.u8NonDirStr            = ext_system_demosaic_manual_nondirection_strength_read(ViPipe);
    pstDemosaicAttr->stManual.u8NonDirMFDetailEhcStr = ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_read(ViPipe);
    pstDemosaicAttr->stManual.u8NonDirHFDetailEhcStr = ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_read(ViPipe);
    pstDemosaicAttr->stManual.u8DetailSmoothRange    = ext_system_demosaic_manual_detail_smooth_range_read(ViPipe);
    pstDemosaicAttr->stManual.u16DetailSmoothStr     = ext_system_demosaic_manual_detail_smooth_strength_read(ViPipe);
    pstDemosaicAttr->stManual.u8ColorNoiseThdY       = ext_system_demosaic_manual_desat_dark_range_read(ViPipe);
    pstDemosaicAttr->stManual.u8ColorNoiseStrY       = ext_system_demosaic_manual_desat_dark_strength_read(ViPipe);
	pstDemosaicAttr->stManual.u8ColorNoiseThdF       = ext_system_demosaic_manual_colornoise_thdf_read(ViPipe);
    pstDemosaicAttr->stManual.u8ColorNoiseStrF       = ext_system_demosaic_manual_colornoise_strf_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetCAAttr(VI_PIPE ViPipe, const ISP_CA_ATTR_S *pstCAAttr)
{
#ifdef CONFIG_ISP_CA_SUPPORT
    GK_U16 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCAAttr);
    ISP_CHECK_BOOL(pstCAAttr->bEnable);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ext_system_ca_en_write(ViPipe, pstCAAttr->bEnable);

    if (pstCAAttr->eCaCpEn >= ISP_CA_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid eCaCpEn!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_ca_cp_en_write(ViPipe, pstCAAttr->eCaCpEn);

    for (i = 0; i < ISP_CA_YRATIO_LUT_LENGTH; i++) {
        ext_system_ca_cp_lut_write(ViPipe, i, ((pstCAAttr->stCP.au8CPLutY[i] << 16) + (pstCAAttr->stCP.au8CPLutU[i] << 8) + (pstCAAttr->stCP.au8CPLutV[i])));

        if (pstCAAttr->stCA.au32YRatioLut[i] > 2047) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au32YRatioLut[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ca_y_ratio_lut_write(ViPipe, i, pstCAAttr->stCA.au32YRatioLut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCAAttr->stCA.as32ISORatio[i] > 2047) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid as32ISORatio[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        ext_system_ca_iso_ratio_lut_write(ViPipe, i, pstCAAttr->stCA.as32ISORatio[i]);
    }

    ext_system_ca_coef_update_en_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetCAAttr(VI_PIPE ViPipe, ISP_CA_ATTR_S *pstCAAttr)
{
#ifdef CONFIG_ISP_CA_SUPPORT
    GK_U16 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCAAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstCAAttr->bEnable = ext_system_ca_en_read(ViPipe);
    pstCAAttr->eCaCpEn = ext_system_ca_cp_en_read(ViPipe);

    for (i = 0; i < ISP_CA_YRATIO_LUT_LENGTH; i++) {
        pstCAAttr->stCP.au8CPLutY[i] = (ext_system_ca_cp_lut_read(ViPipe, i) >> 16) & 0x000000FF;
        pstCAAttr->stCP.au8CPLutU[i] = (ext_system_ca_cp_lut_read(ViPipe, i) >> 8) & 0x000000FF;
        pstCAAttr->stCP.au8CPLutV[i] = ext_system_ca_cp_lut_read(ViPipe, i) & 0x000000FF;
    }

    for (i = 0; i < ISP_CA_YRATIO_LUT_LENGTH; i++) {
        pstCAAttr->stCA.au32YRatioLut[i] = ext_system_ca_y_ratio_lut_read(ViPipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstCAAttr->stCA.as32ISORatio[i] = ext_system_ca_iso_ratio_lut_read(ViPipe, i);
    }

    return GK_SUCCESS;
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetClutCoeff(VI_PIPE ViPipe, const ISP_CLUT_LUT_S *pstClutLUT)
{
    return ISP_SetClutCoeff(ViPipe, pstClutLUT);
}
GK_S32 MPI_ISP_GetClutCoeff(VI_PIPE ViPipe, ISP_CLUT_LUT_S *pstClutLUT)
{
    return ISP_GetClutCoeff(ViPipe, pstClutLUT);
}

GK_S32 MPI_ISP_SetClutAttr(VI_PIPE ViPipe, const ISP_CLUT_ATTR_S *pstClutAttr)
{
    return ISP_SetClutAttr(ViPipe, pstClutAttr);
}
GK_S32 MPI_ISP_GetClutAttr(VI_PIPE ViPipe, ISP_CLUT_ATTR_S *pstClutAttr)
{
    return ISP_GetClutAttr(ViPipe, pstClutAttr);
}

GK_S32 MPI_ISP_SetBlackLevelAttr(VI_PIPE ViPipe, const ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    GK_S32 i = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstBlackLevel->enOpType >= OP_TYPE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid enOpType %d!\n", pstBlackLevel->enOpType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_black_level_manual_mode_write(ViPipe, pstBlackLevel->enOpType);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        if (pstBlackLevel->au16BlackLevel[i] > 0xFFF) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid BlackLevel[%d]:%d!\n", i, pstBlackLevel->au16BlackLevel[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    ext_system_black_level_00_write(ViPipe, pstBlackLevel->au16BlackLevel[0]);
    ext_system_black_level_01_write(ViPipe, pstBlackLevel->au16BlackLevel[1]);
    ext_system_black_level_10_write(ViPipe, pstBlackLevel->au16BlackLevel[2]);
    ext_system_black_level_11_write(ViPipe, pstBlackLevel->au16BlackLevel[3]);
    ext_system_black_level_change_write(ViPipe, (GK_U8)GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetBlackLevelAttr(VI_PIPE ViPipe, ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstBlackLevel->enOpType = ext_system_black_level_manual_mode_read(ViPipe);
    pstBlackLevel->au16BlackLevel[0] = ext_system_black_level_00_read(ViPipe);
    pstBlackLevel->au16BlackLevel[1] = ext_system_black_level_01_read(ViPipe);
    pstBlackLevel->au16BlackLevel[2] = ext_system_black_level_10_read(ViPipe);
    pstBlackLevel->au16BlackLevel[3] = ext_system_black_level_11_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_FPNCalibrate(VI_PIPE ViPipe, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr)
{
#ifdef CONFIG_VI_FPN_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstCalibrateAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return ISP_SetCalibrateAttr(ViPipe, pstCalibrateAttr);
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_SetFPNAttr(VI_PIPE ViPipe, const ISP_FPN_ATTR_S *pstFPNAttr)
{
#ifdef CONFIG_VI_FPN_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);
    ISP_CHECK_BOOL(pstFPNAttr->bEnable);

    return ISP_SetCorrectionAttr(ViPipe, pstFPNAttr);
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetFPNAttr(VI_PIPE ViPipe, ISP_FPN_ATTR_S *pstFPNAttr)
{
#ifdef CONFIG_VI_FPN_SUPPORT
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return ISP_GetCorrectionAttr(ViPipe, pstFPNAttr);
#else
    ISP_TRACE(MODULE_DBG_ERR, "Not support this interface!\n");
    return ERR_CODE_ISP_NOT_SUPPORT;
#endif
}

GK_S32 MPI_ISP_GetISPRegAttr(VI_PIPE ViPipe, ISP_REG_ATTR_S *pstIspRegAttr)
{
    GK_U32 u32IspBindAttr;
    ALG_LIB_S stAeLib;
    ALG_LIB_S stAwbLib;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspRegAttr);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u32IspBindAttr = ext_system_bind_attr_read(ViPipe);
    stAeLib.s32Id  = (u32IspBindAttr >> 8) & 0xFF;
    stAwbLib.s32Id = u32IspBindAttr & 0xFF;

    MUTEX_LOCK(pstIspCtx->stLock);

    pstIspRegAttr->pIspExtRegAddr   = VReg_GetVirtAddrBase(ISP_VIR_REG_BASE(ViPipe));
    pstIspRegAttr->u32IspExtRegSize = ISP_VREG_SIZE;
    pstIspRegAttr->pAeExtRegAddr    = VReg_GetVirtAddrBase(AE_LIB_VREG_BASE(stAeLib.s32Id));
    pstIspRegAttr->u32AeExtRegSize  = AE_VREG_SIZE;
    pstIspRegAttr->pAwbExtRegAddr   = VReg_GetVirtAddrBase(AWB_LIB_VREG_BASE(stAwbLib.s32Id));
    pstIspRegAttr->u32AwbExtRegSize = ALG_LIB_VREG_SIZE_BIN;
    MUTEX_UNLOCK(pstIspCtx->stLock);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetDebug(VI_PIPE ViPipe, const ISP_DEBUG_INFO_S *pstIspDebug)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_BOOL(pstIspDebug->bDebugEn);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return ISP_DbgSet(ViPipe, pstIspDebug);
}

GK_S32 MPI_ISP_GetDebug(VI_PIPE ViPipe, ISP_DEBUG_INFO_S *pstIspDebug)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    return ISP_DbgGet(ViPipe, pstIspDebug);
}

GK_S32 MPI_ISP_SetCtrlParam(VI_PIPE ViPipe, const ISP_CTRL_PARAM_S *pstIspCtrlParam)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspCtrlParam);

    ISP_CHECK_OPEN(ViPipe);

    return ioctl(g_as32IspFd[ViPipe], ISP_SET_CTRL_PARAM, pstIspCtrlParam);
}

GK_S32 MPI_ISP_GetCtrlParam(VI_PIPE ViPipe, ISP_CTRL_PARAM_S *pstIspCtrlParam)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspCtrlParam);
    ISP_CHECK_OPEN(ViPipe);

    return ioctl(g_as32IspFd[ViPipe], ISP_GET_CTRL_PARAM, pstIspCtrlParam);
}

GK_S32 MPI_ISP_QueryInnerStateInfo(VI_PIPE ViPipe, ISP_INNER_STATE_INFO_S *pstInnerStateInfo)
{
    int i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstInnerStateInfo);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstInnerStateInfo->u8OverShoot    = ext_system_actual_sharpen_overshootAmt_read(ViPipe);
    pstInnerStateInfo->u8UnderShoot   = ext_system_actual_sharpen_undershootAmt_read(ViPipe);
    pstInnerStateInfo->u8ShootSupStr  = ext_system_actual_sharpen_shootSupSt_read(ViPipe);
    pstInnerStateInfo->u16TextureFreq = ext_system_actual_sharpen_texture_frequence_read(ViPipe);
    pstInnerStateInfo->u16EdgeFreq    = ext_system_actual_sharpen_edge_frequence_read(ViPipe);
    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        pstInnerStateInfo->au16EdgeStr[i]    = ext_system_actual_sharpen_edge_str_read(ViPipe, i);
        pstInnerStateInfo->au16TextureStr[i] = ext_system_actual_sharpen_texture_str_read(ViPipe, i);
    }

    pstInnerStateInfo->u8NrLscRatio = ext_system_bayernr_actual_nr_lsc_ratio_read(ViPipe);
    pstInnerStateInfo->u16CoringWgt = ext_system_bayernr_actual_coring_weight_read(ViPipe);
    pstInnerStateInfo->u8FineStr    = ext_system_bayernr_actual_fine_strength_read(ViPipe);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstInnerStateInfo->au16CoarseStr[i] = ext_system_bayernr_actual_coarse_strength_read(ViPipe, i);
        pstInnerStateInfo->au8ChromaStr[i]  = ext_system_bayernr_actual_chroma_strength_read(ViPipe, i);
    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        pstInnerStateInfo->au8WDRFrameStr[i] = ext_system_bayernr_actual_wdr_frame_strength_read(ViPipe, i);
    }

    for (i = 0; i < 3; i++) {
        pstInnerStateInfo->u32WDRExpRatioActual[i] = ext_system_actual_wdr_exposure_ratio_read(ViPipe, i);
    }

    pstInnerStateInfo->u16DrcStrengthActual    = ext_system_drc_actual_strength_read(ViPipe);
    pstInnerStateInfo->u16DeHazeStrengthActual = ext_system_dehaze_actual_strength_read(ViPipe);
    pstInnerStateInfo->bWDRSwitchFinish        = ext_top_wdr_switch_read(ViPipe);
    pstInnerStateInfo->bResSwitchFinish        = ext_top_res_switch_read(ViPipe);

    pstInnerStateInfo->au16BLActual[0] = ext_system_black_level_query_00_read(ViPipe);
    pstInnerStateInfo->au16BLActual[1] = ext_system_black_level_query_01_read(ViPipe);
    pstInnerStateInfo->au16BLActual[2] = ext_system_black_level_query_10_read(ViPipe);
    pstInnerStateInfo->au16BLActual[3] = ext_system_black_level_query_11_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDngImageStaticInfo(VI_PIPE ViPipe, DNG_IMAGE_STATIC_INFO_S *pstDngImageStaticInfo)
{
    GK_BOOL bValidDngImageStaticInfo;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDngImageStaticInfo);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    bValidDngImageStaticInfo = ext_system_dng_static_info_valid_read(ViPipe);

    if (bValidDngImageStaticInfo == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_ERR, "DngImageStaticInfo have not been set in xxx_Cmos.x!\n");
        return GK_SUCCESS;
    }

    if (ioctl(g_as32IspFd[ViPipe], ISP_DNG_INFO_GET, pstDngImageStaticInfo) != GK_SUCCESS) {
        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetDngColorParam(VI_PIPE ViPipe, const ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDngColorParam);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    if (pstDngColorParam->stWbGain1.u16Bgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain1.u16Bgain can't bigger than 0xFFF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain1.u16Ggain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain1.u16Ggain can't bigger than 0xFFF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain1.u16Rgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain1.u16Rgain can't bigger than 0xFFF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain2.u16Bgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain2.u16Bgain can't bigger than 0xFFF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain2.u16Ggain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain2.u16Ggain can't bigger than 0xFFF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain2.u16Rgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain2.u16Rgain can't bigger than 0xFFF!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    ext_system_dnggh_wb_gain_r_write(ViPipe, pstDngColorParam->stWbGain1.u16Rgain);
    ext_system_dnggh_wb_gain_g_write(ViPipe, pstDngColorParam->stWbGain1.u16Ggain);
    ext_system_dnggh_wb_gain_b_write(ViPipe, pstDngColorParam->stWbGain1.u16Bgain);
    ext_system_dng_low_wb_gain_r_write(ViPipe, pstDngColorParam->stWbGain2.u16Rgain);
    ext_system_dng_low_wb_gain_g_write(ViPipe, pstDngColorParam->stWbGain2.u16Ggain);
    ext_system_dng_low_wb_gain_b_write(ViPipe, pstDngColorParam->stWbGain2.u16Bgain);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetDngColorParam(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDngColorParam);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    pstDngColorParam->stWbGain1.u16Rgain = ext_system_dnggh_wb_gain_r_read(ViPipe);
    pstDngColorParam->stWbGain1.u16Ggain = ext_system_dnggh_wb_gain_g_read(ViPipe);
    pstDngColorParam->stWbGain1.u16Bgain = ext_system_dnggh_wb_gain_b_read(ViPipe);

    pstDngColorParam->stWbGain2.u16Rgain = ext_system_dng_low_wb_gain_r_read(ViPipe);
    pstDngColorParam->stWbGain2.u16Ggain = ext_system_dng_low_wb_gain_g_read(ViPipe);
    pstDngColorParam->stWbGain2.u16Bgain = ext_system_dng_low_wb_gain_b_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_SetSmartInfo(VI_PIPE ViPipe, const ISP_SMART_INFO_S *pstSmartInfo)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSmartInfo);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        ISP_CHECK_BOOL(pstSmartInfo->stROI[i].bEnable);
        ISP_CHECK_BOOL(pstSmartInfo->stROI[i].bAvailable);
        ext_system_smart_enable_write(ViPipe, i, pstSmartInfo->stROI[i].bEnable);
        ext_system_smart_available_write(ViPipe, i, pstSmartInfo->stROI[i].bAvailable);
        ext_system_smart_luma_write(ViPipe, i, pstSmartInfo->stROI[i].u8Luma);
    }
    ext_system_smart_update_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 MPI_ISP_GetSmartInfo(VI_PIPE ViPipe, ISP_SMART_INFO_S *pstSmartInfo)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSmartInfo);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        pstSmartInfo->stROI[i].bEnable    = ext_system_smart_enable_read(ViPipe, i);
        pstSmartInfo->stROI[i].bAvailable = ext_system_smart_available_read(ViPipe, i);
        pstSmartInfo->stROI[i].u8Luma     = ext_system_smart_luma_read(ViPipe, i);
    }

    return GK_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
