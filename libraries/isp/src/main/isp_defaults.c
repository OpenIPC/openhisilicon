/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_defaults.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_VOID ISP_RegsDefault(VI_PIPE ViPipe)
{
    return;
}

GK_VOID ISP_RegsInitialize(VI_PIPE ViPipe)
{
    return;
}

/* The ext regs is a block of memory, not real register, need a default value. */
GK_VOID ISP_ExtRegsDefault(VI_PIPE ViPipe)
{
    GK_U32 i;

    ext_system_freeze_firmware_write(ViPipe, ISP_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT);

    ext_system_sys_debug_enable_write(ViPipe, ISP_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT);
    ext_system_sys_debuggh_addr_write(ViPipe, ISP_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DEFAULT);
    ext_system_sys_debug_low_addr_write(ViPipe, ISP_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DEFAULT);
    ext_system_sys_debug_size_write(ViPipe, ISP_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT);
    ext_system_sys_debug_depth_write(ViPipe, ISP_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT);
    ext_system_statistics_ctrl_lowbit_write(ViPipe, ISP_EXT_SYSTEM_STATISTICS_LOWBIT_DEFAULT);
    ext_system_statistics_ctrlghbit_write(ViPipe, ISP_EXT_SYSTEM_STATISTICS_HIGHBIT_DEFAULT);
    ext_system_be_free_buffergh_addr_write(ViPipe, ISP_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DEFAULT);
    ext_system_be_free_buffer_low_addr_write(ViPipe, ISP_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DEFAULT);
    ext_system_be_lut_stt_buffergh_addr_write(ViPipe, ISP_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DEFAULT);
    ext_system_be_lut_stt_buffer_low_addr_write(ViPipe, ISP_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DEFAULT);
    ext_system_ldci_read_stt_buffer_low_addr_write(ViPipe, ISP_EXT_SYSTEM_LDCI_READ_STT_BUF_LOW_ADDR_DEFAULT);
    ext_system_ldci_read_stt_buffergh_addr_write(ViPipe, ISP_EXT_SYSTEM_LDCI_READ_STT_BUF_HIGH_ADDR_DEFAULT);
    ext_system_top_channel_select_write(ViPipe, ISP_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DEFAULT);
    ext_system_top_channel_select_pre_write(ViPipe, ISP_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DEFAULT);

    /* isp pipe different configs for stitch mode */
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        ext_system_isp_pipe_diff_offset_write(ViPipe, i, ISP_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT);
        ext_system_isp_pipe_diff_gain_write(ViPipe, i, ISP_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT);
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        if (i % 4 == 0) {
            ext_system_isp_pipe_diff_ccm_write(ViPipe, i, ISP_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT);
        } else {
            ext_system_isp_pipe_diff_ccm_write(ViPipe, i, 0);
        }
    }

    return;
}

GK_VOID ISP_ExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_U32 u32IspBindAttr = 0;
    ISP_CMOS_SENSOR_MAX_RESOLUTION_S *pstSnsMaxResolution = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_SensorGetMaxResolution(ViPipe, &pstSnsMaxResolution);
    ext_system_sensor_max_resolution_width_write(ViPipe, pstSnsMaxResolution->u32MaxWidth);
    ext_system_sensor_max_resolution_height_write(ViPipe, pstSnsMaxResolution->u32MaxHeight);

    /* update Isp bind attr to ext regs */
    u32IspBindAttr = SignedLeftShift(pstIspCtx->stBindAttr.stAeLib.s32Id, 8) + pstIspCtx->stBindAttr.stAwbLib.s32Id;

    ext_system_bind_attr_write(ViPipe, u32IspBindAttr);

    ext_system_block_num_write(ViPipe, pstIspCtx->stBlockAttr.u8BlockNum);

    return;
}

GK_VOID ISP_YUVExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_U32 u32IspBindAttr = 0;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* update Isp bind attr to ext regs */
    u32IspBindAttr = (pstIspCtx->stBindAttr.stAeLib.s32Id << 8) | pstIspCtx->stBindAttr.stAwbLib.s32Id;
    ext_system_bind_attr_write(ViPipe, u32IspBindAttr);

    ext_system_block_num_write(ViPipe, pstIspCtx->stBlockAttr.u8BlockNum);

    return;
}

static GK_VOID ISP_ProShpParamInitialize(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_U8 i, j, k;
    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspCtx->stProShpParamCtrl.pstProShpParam->bEnable = GK_FALSE;
    pstIspCtx->stProShpParamCtrl.pstProShpParam->u32ParamNum = 3;
    for (k = 0; k < PRO_MAX_FRAME_NUM; k++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au16TextureStr[j][i] = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT;
                pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au16EdgeStr[j][i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT;
            }

            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8LumaWgt[j][i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT;
            }

            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au16TextureFreq[i] = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au16EdgeFreq[i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8OverShoot[i]    = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8UnderShoot[i]   = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8ShootSupStr[i]  = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8DetailCtrl[i]   = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8EdgeFiltStr[i]  = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8RGain[i]        = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8BGain[i]        = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT;
            pstIspCtx->stProShpParamCtrl.pstProShpParam->astShpAttr[k].au8SkinGain[i]     = ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT;
        }
    }
}

static GK_VOID ISP_ProNrParamInitialize(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_U8 i, j, k;
    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspCtx->stProNrParamCtrl.pstProNrParam->bEnable = GK_FALSE;
    pstIspCtx->stProNrParamCtrl.pstProNrParam->u32ParamNum = 3;
    for (k = 0; k < PRO_MAX_FRAME_NUM; k++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstIspCtx->stProNrParamCtrl.pstProNrParam->astNrAttr[k].au8FineStr[i]       = ISP_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT;
            pstIspCtx->stProNrParamCtrl.pstProNrParam->astNrAttr[k].au16CoringWgt[i]    = ISP_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT;

            for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
                pstIspCtx->stProNrParamCtrl.pstProNrParam->astNrAttr[k].au8ChromaStr[j][i]  = ISP_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
                pstIspCtx->stProNrParamCtrl.pstProNrParam->astNrAttr[k].au16CoarseStr[j][i] = ISP_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
            }
        }
    }
}

GK_VOID ISP_AlgKeyInit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstIspCtx->unAlgKey.u64Key = 0xFFFFFFFFFFFFFFFF;
}

GK_VOID ISP_GlobalInitialize(VI_PIPE ViPipe)
{
    GK_U8  i;
    GK_U32 u32Value = 0;
    GK_VOID   *pValue    = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstIspCtx->stSnsImageMode.u16Width  = ext_top_sensor_width_read(ViPipe);
    pstIspCtx->stSnsImageMode.u16Height = ext_top_sensor_height_read(ViPipe);
    u32Value = ext_system_fps_base_read(ViPipe);
    pValue = (GK_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps = *(GK_FLOAT *)pValue;

    pstIspCtx->stPreSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
    pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
    pstIspCtx->stPreSnsImageMode.f32Fps    = pstIspCtx->stSnsImageMode.f32Fps;

    pstIspCtx->bChangeWdrMode   =  GK_FALSE;
    pstIspCtx->bChangeImageMode =  GK_FALSE;
    pstIspCtx->bChangeIspRes    =  GK_FALSE;
    pstIspCtx->u8Count          = 0;
    pstIspCtx->u8SnsWDRMode     = ext_system_sensor_wdr_mode_read(ViPipe);
    pstIspCtx->u8PreSnsWDRMode  = pstIspCtx->u8SnsWDRMode;

    pstIspCtx->u32FrameCnt       = 0;
    pstIspCtx->u8IspImageModeFlag = 0;

    pstIspCtx->stLinkage.bDefectPixel = GK_FALSE;
    pstIspCtx->bFreezeFw = GK_FALSE;

    pstIspCtx->stLinkage.bSnapState  = GK_FALSE;
    pstIspCtx->stLinkage.bProTrigger = GK_FALSE;
    pstIspCtx->stLinkage.bStatReady  = GK_FALSE;
    pstIspCtx->stLinkage.bRunOnce    = GK_FALSE;
    pstIspCtx->u8ProFrmCnt           = 0;
    pstIspCtx->stLinkage.u32IsoDoneFrmcnt    = 0;
    pstIspCtx->stLinkage.u8ProIndex          = 0;
    pstIspCtx->stLinkage.u8Cfg2ValidDelayMax = 2;
    for (i = 0; i < ISP_SYNC_ISO_BUF_MAX; i++) {
        pstIspCtx->stLinkage.au32SyncIsoBuf[i] = 100;
        pstIspCtx->stLinkage.au8ProIndexBuf[i] = 0;
    }
    for (i = 0; i < ISP_MAX_BE_NUM; i++) {
        pstIspCtx->stSpecialOpt.abBeOnSttUpdate[i] = GK_TRUE;
    }

    memset(&pstIspCtx->stPreDngCcm, 0, sizeof(ISP_DNG_CCM_S));
    memset(&pstIspCtx->stPreDngColorParam, 0, sizeof(ISP_DNG_COLORPARAM_S));
    memset(&pstIspCtx->stDcfUpdateInfo, 0, sizeof(ISP_DCF_UPDATE_INFO_S));
    memset(&pstIspCtx->stFrameInfo, 0, sizeof(ISP_FRAME_INFO_S));

    pstIspCtx->stSpecialOpt.bFeSttUpdate = GK_TRUE;
    ISP_ProShpParamInitialize(ViPipe);
    ISP_ProNrParamInitialize(ViPipe);
    ISP_AlgKeyInit(ViPipe);

    return;
}

GK_VOID ISP_DngExtRegsInitialize(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    if (pstDngColorParam->stWbGain1.u16Bgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain1.u16Bgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Bgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain1.u16Ggain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain1.u16Ggain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Ggain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain1.u16Rgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain1.u16Rgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Rgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Bgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain2.u16Bgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Bgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Ggain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain2.u16Ggain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Ggain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Rgain > 0xFFF) {
        ISP_TRACE(MODULE_DBG_ERR, "stWbGain2.u16Rgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Rgain = 0xFFF;
    }

    ext_system_dnggh_wb_gain_r_write(ViPipe, pstDngColorParam->stWbGain1.u16Rgain);
    ext_system_dnggh_wb_gain_g_write(ViPipe, pstDngColorParam->stWbGain1.u16Ggain);
    ext_system_dnggh_wb_gain_b_write(ViPipe, pstDngColorParam->stWbGain1.u16Bgain);
    ext_system_dng_low_wb_gain_r_write(ViPipe, pstDngColorParam->stWbGain2.u16Rgain);
    ext_system_dng_low_wb_gain_g_write(ViPipe, pstDngColorParam->stWbGain2.u16Ggain);
    ext_system_dng_low_wb_gain_b_write(ViPipe, pstDngColorParam->stWbGain2.u16Bgain);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
