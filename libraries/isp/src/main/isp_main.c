/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_main.h"
#include "isp_ext_config.h"
#include "isp_defaults.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_S32 ISP_UpdateInfo(VI_PIPE ViPipe)
{
    GK_S32 i, s32Ret;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_CHECK_OPEN(ViPipe);

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (pstIspCtx->astAlgs[i].bUsed) {
            if (pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl != GK_NULL) {
                pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl(ViPipe, ISP_UPDATE_INFO_GET, &pstIspCtx->stDcfUpdateInfo);
            }
        }
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_UPDATE_INFO_SET, &pstIspCtx->stDcfUpdateInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set dcf updateinfo failed\n", ViPipe);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateFrameInfo(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_FRAME_INFO_SET, &pstIspCtx->stFrameInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set frameinfo failed\n", ViPipe);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateAttachInfo(VI_PIPE ViPipe)
{
    GK_S32 i;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    if (pstIspCtx->stAttachInfoCtrl.pstAttachInfo == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d the Isp attach Info hasn't init!\n", ViPipe);
        return GK_FAILURE;
    }

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (pstIspCtx->astAlgs[i].bUsed) {
            if (pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl != GK_NULL) {
                pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl(ViPipe, ISP_ATTACHINFO_GET, pstIspCtx->stAttachInfoCtrl.pstAttachInfo);
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateColorGamutinfo(VI_PIPE ViPipe)
{
    GK_S32 i;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    if (pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d the Isp colorGamut Info hasn't init!\n", ViPipe);
        return GK_FAILURE;
    }

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (pstIspCtx->astAlgs[i].bUsed) {
            if (pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl != GK_NULL) {
                pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl(ViPipe, ISP_COLORGAMUTINFO_GET,
                                                             pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo);
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ReadExtregs(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_U64 u64PhyAddrHigh;
    GK_U64 u64PhyAddrTemp;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddrHigh = (GK_U64)ext_system_sys_debuggh_addr_read(ViPipe);
    u64PhyAddrTemp = (GK_U64)ext_system_sys_debug_low_addr_read(ViPipe);
    u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

    pstIspCtx->bFreezeFw = ext_system_freeze_firmware_read(ViPipe);

    pstIspCtx->stIspDbg.bDebugEn = ext_system_sys_debug_enable_read(ViPipe);
    pstIspCtx->stIspDbg.u64PhyAddr = u64PhyAddrTemp;

    pstIspCtx->stIspDbg.u32Depth = ext_system_sys_debug_depth_read(ViPipe);
    pstIspCtx->stIspDbg.u32Size = ext_system_sys_debug_size_read(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateExtRegs(VI_PIPE ViPipe)
{
    return GK_SUCCESS;
}

GK_S32 ISP_SwitchImageMode(VI_PIPE ViPipe)
{
    GK_U8 u8WDRMode;
    GK_S32 s32Ret = GK_SUCCESS;
    GK_U32 u32Value = 0;
    GK_VOID *pValue = GK_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stIspImageMode;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstIspCtx->stSnsImageMode.u16Width  = ext_top_sensor_width_read(ViPipe);
    pstIspCtx->stSnsImageMode.u16Height = ext_top_sensor_height_read(ViPipe);
    u32Value = ext_system_fps_base_read(ViPipe);
    pValue = (GK_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps    = *(GK_FLOAT *)pValue;
    pstIspCtx->stSnsImageMode.u8SnsMode = ext_system_sensor_mode_read(ViPipe);

    pstIspCtx->stSysRect.s32X      = ext_system_corp_pos_x_read(ViPipe);
    pstIspCtx->stSysRect.s32Y      = ext_system_corp_pos_y_read(ViPipe);
    pstIspCtx->stSysRect.u32Width  = ext_sync_total_width_read(ViPipe);
    pstIspCtx->stSysRect.u32Height = ext_sync_total_height_read(ViPipe);

    stIspImageMode.u16Width  = ext_sync_total_width_read(ViPipe);
    stIspImageMode.u16Height = ext_sync_total_height_read(ViPipe);
    stIspImageMode.f32Fps    = pstIspCtx->stSnsImageMode.f32Fps;
    stIspImageMode.u8SnsMode = pstIspCtx->stSnsImageMode.u8SnsMode;

    ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->stHdrAttr);

    if (pstIspCtx->u8IspImageModeFlag == 0) {
        ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET,
                     (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

        memcpy(&pstIspCtx->stPreSnsImageMode, &pstIspCtx->stSnsImageMode, sizeof(ISP_SENSOR_IMAGE_MODE_S));

        pstIspCtx->u8IspImageModeFlag = 1;
    } else {
        if ((pstIspCtx->stSnsImageMode.u16Width  != pstIspCtx->stPreSnsImageMode.u16Width) ||
            (pstIspCtx->stSnsImageMode.u16Height != pstIspCtx->stPreSnsImageMode.u16Height) ||
            (pstIspCtx->stSnsImageMode.f32Fps    != pstIspCtx->stPreSnsImageMode.f32Fps) ||
            (pstIspCtx->stSnsImageMode.u8SnsMode != pstIspCtx->stPreSnsImageMode.u8SnsMode)) {
            s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->stBlockAttr);
            if (s32Ret != GK_SUCCESS) {
                ISP_TRACE(MODULE_DBG_ERR, "pipe:%d Update ISP Block Attr Failed !\n", ViPipe);
                return s32Ret;
            }

            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->stPipeSize);
            if (s32Ret != GK_SUCCESS) {
                ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get Pipe size failed\n", ViPipe);
                return s32Ret;
            }

            /* p2en info */
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->bIsp0P2En);
            if (s32Ret != GK_SUCCESS) {
                ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get p2en info failed\n", ViPipe);
                return s32Ret;
            }

            stSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
            stSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
            stSnsImageMode.f32Fps    = pstIspCtx->stSnsImageMode.f32Fps;
            stSnsImageMode.u8SnsMode = pstIspCtx->stSnsImageMode.u8SnsMode;

            memcpy(&pstIspCtx->stPreSnsImageMode, &pstIspCtx->stSnsImageMode, sizeof(ISP_SENSOR_IMAGE_MODE_S));

            s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);

            if (s32Ret == GK_SUCCESS) {  /* Need to init sensor */
                u8WDRMode = ext_system_sensor_wdr_mode_read(ViPipe);

                s32Ret = ISP_SensorSetWDRMode(ViPipe, u8WDRMode);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set sensor wdr mode failed\n", ViPipe);
                    return s32Ret;
                }

                pstIspCtx->stIspParaRec.bStitchSync = GK_FALSE;
                s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->stIspParaRec.bStitchSync);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set isp stitch sync failed!\n", ViPipe);
                }

                if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
                    ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
                }

                s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_RES_SWITCH_SET);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "pipe:%d Config ISP Res Switch Failed with ec %#x!\n", ViPipe, s32Ret);
                    return s32Ret;
                }

                s32Ret = ISP_RegCfgCtrl(ViPipe);
                if (s32Ret != GK_SUCCESS) {
                    return s32Ret;
                }

                s32Ret = ISP_GetBeLastBuf(ViPipe);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d Get be last bufs failed %x!\n", ViPipe, s32Ret);
                    return s32Ret;
                }

                ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_CHANGE_IMAGE_MODE_SET, (GK_VOID *)&stIspImageMode);
                ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET,
                             (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);
                ISP_SensorUpdateSnsReg(ViPipe);

                if (IS_FS_WDR_MODE(u8WDRMode)) {
                    s32Ret = ISP_WDRCfgSet(ViPipe);
                    if (s32Ret != GK_SUCCESS) {
                        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d ISP_WDRCfgSet ec %#x!\n", ViPipe, s32Ret);
                        return s32Ret;
                    }
                }

                s32Ret = ISP_SwitchRegSet(ViPipe);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "pipe:%d set reg config failed!\n", ViPipe);
                    return GK_FAILURE;
                }

                s32Ret = ISP_SensorSwitch(ViPipe);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "pipe:%d init sensor failed!\n", ViPipe);
                    return GK_FAILURE;
                }
            } else {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET, (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);
            }

            ext_top_res_switch_write(ViPipe, GK_TRUE);

            ISP_SensorUpdateDefault(ViPipe);
            ISP_SensorUpdateBlc(ViPipe);
            ISP_SensorGetDefault(ViPipe, &pstSnsDft);

            pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorID = pstSnsDft->stSensorMode.u32SensorID;
            pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorMode = pstSnsDft->stSensorMode.u8SensorMode;

            pstIspCtx->stFrameInfo.u32SensorID = pstSnsDft->stSensorMode.u32SensorID;
            pstIspCtx->stFrameInfo.u32SensorMode = pstSnsDft->stSensorMode.u8SensorMode;

            ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);

            memcpy(&pstIspCtx->stDngInfoCtrl.pstIspDng->stDngRawFormat, &pstSnsDft->stSensorMode.stDngRawFormat,
                   sizeof(DNG_RAW_FORMAT_S));

            if ((pstSnsRegsInfo->u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || (pstSnsRegsInfo->u8Cfg2ValidDelayMax < 1)) {
                ISP_TRACE(MODULE_DBG_ERR, "pipe:%d Delay of config to invalid is:0x%x\n", ViPipe, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
                pstSnsRegsInfo->u8Cfg2ValidDelayMax = 1;
            }

            pstIspCtx->stLinkage.u8Cfg2ValidDelayMax = pstSnsRegsInfo->u8Cfg2ValidDelayMax;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_WDRCfgSet(VI_PIPE ViPipe)
{
    GK_U8  i;
    GK_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
    ISP_REGCFG_S       *pstRegCfg = GK_NULL;
    ISP_WDR_CFG_S      stWDRCfg;

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
    ISP_CHECK_POINTER(pstRegCfg);

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);
    memset(&stWDRCfg, 0, sizeof(ISP_WDR_CFG_S));

    stWDRCfg.u8WDRMode = pstIspCtx->u8SnsWDRMode;

    for (i = 0; i < EXP_RATIO_NUM; i++) {
        stWDRCfg.au32ExpRatio[i] = CLIP3(pstSnsDft->stWdrSwitchAttr.au32ExpRatio[i], 0x40, 0xFFF);
    }

    memcpy(&stWDRCfg.stWdrRegCfg, &pstRegCfg->stRegCfg.stAlgRegCfg[0].stWdrRegCfg.stSyncRegCfg,
           sizeof(ISP_FSWDR_SYNC_CFG_S));

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WDR_CFG_SET, &stWDRCfg);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d Config WDR register Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SwitchWDR(VI_PIPE ViPipe)
{
    GK_S32 s32Ret = GK_SUCCESS;
    GK_U8  u8SensorWDRMode;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = GK_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ISP_SttAddrInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init stt address failed!\n", ViPipe);
        return s32Ret;
    }

    /* 0. block attr change */
    s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->stBlockAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Update ISP Block Attr Failed !\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ISP_RegCfgCtrl(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    /* 1. switch sensor to WDR mode */
    u8SensorWDRMode = pstIspCtx->u8SnsWDRMode;
    s32Ret = ISP_SensorSetWDRMode(ViPipe, u8SensorWDRMode);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set sensor WDR mode failed!\n", ViPipe);
        return GK_FAILURE;
    }

    /* 2. get last buffer, note: when sensor timing change, callback */
    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
    }

    s32Ret = ISP_GetBeLastBuf(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get be last bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    ISP_SensorUpdateDefault(ViPipe);
    ISP_SensorUpdateBlc(ViPipe);
    ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

    ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_BLC_SET, (GK_VOID *)&pstSnsBlackLevel->au16BlackLevel[1]);

    /* 3. init the common part of extern registers and real registers */
    ISP_ExtRegsInitialize(ViPipe);
    ISP_RegsInitialize(ViPipe);

    /* 5. notify algs to switch WDR mode */
    ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_WDR_MODE_SET, (GK_VOID *)&u8SensorWDRMode);

    s32Ret = ISP_WDRCfgSet(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] ISP_WDRCfgSet error \n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_MODE_SWITCH_SET);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d Config ISP Mode Switch Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_SwitchRegSet(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set reg config failed!\n", ViPipe);
        return GK_FAILURE;
    }

    /* 6. switch sensor to WDR mode */
    ISP_SensorUpdateSnsReg(ViPipe);
    s32Ret = ISP_SensorSwitch(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init sensor failed!\n", ViPipe);
        return GK_FAILURE;
    }

    /* 7. get new sensor default param */
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);

    pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorID = pstSnsDft->stSensorMode.u32SensorID;
    pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorMode = pstSnsDft->stSensorMode.u8SensorMode;

    ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);

    if ((pstSnsRegsInfo->u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || (pstSnsRegsInfo->u8Cfg2ValidDelayMax < 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Delay of config to invalid is:0x%x\n", ViPipe, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
        pstSnsRegsInfo->u8Cfg2ValidDelayMax = 1;
    }

    pstIspCtx->stLinkage.u8Cfg2ValidDelayMax = pstSnsRegsInfo->u8Cfg2ValidDelayMax;

    pstIspCtx->u8PreSnsWDRMode = pstIspCtx->u8SnsWDRMode;
    ext_top_wdr_switch_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

static GK_S32 ISP_SwitchModeProcess(VI_PIPE ViPipe)
{
    GK_S32  s32Ret   = GK_SUCCESS;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stIspImageMode;
    ISP_CTX_S              *pstIspCtx        = GK_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    stIspImageMode.u16Width  = ext_sync_total_width_read(ViPipe);
    stIspImageMode.u16Height = ext_sync_total_height_read(ViPipe);
    stIspImageMode.f32Fps    = pstIspCtx->stSnsImageMode.f32Fps;
    stIspImageMode.u8SnsMode = pstIspCtx->stSnsImageMode.u8SnsMode;

    /* 1. if switch wdr mode, should disable festt enable first */
    if (pstIspCtx->bChangeWdrMode == GK_TRUE) {
        s32Ret = ISP_ResetFeSttEn(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Reset FeStt enable failed\n", ViPipe);
            return s32Ret;
        }
    }

    /* 2.Get info: WDR, HDR, Stitch */
    /* WDR attribute */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &pstIspCtx->stWdrAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get WDR attr failed\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->stPipeSize);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get Pipe size failed\n", ViPipe);
        return s32Ret;
    }

    /* p2en info */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->bIsp0P2En);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get p2en info failed\n", ViPipe);
        return s32Ret;
    }

    if (pstIspCtx->bChangeWdrMode == GK_TRUE) {
        s32Ret = ISP_SttAddrInit(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init stt address failed!\n", ViPipe);
            return s32Ret;
        }
    }

    s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->stBlockAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Update ISP Block Attr Failed !\n", ViPipe);
        return s32Ret;
    }

    pstIspCtx->stIspParaRec.bStitchSync = GK_FALSE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->stIspParaRec.bStitchSync);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set isp stitch sync failed!\n", ViPipe);
    }

    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_RES_SWITCH_SET);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Config ISP Res Switch Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_RegCfgCtrl(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = ISP_GetBeLastBuf(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get be last bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    ISP_SensorUpdateDefault(ViPipe);
    ISP_SensorUpdateBlc(ViPipe);
    ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

    ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_BLC_SET, (GK_VOID *)&pstSnsBlackLevel->au16BlackLevel[1]);

    /* init the common part of extern registers and real registers */
    ISP_ExtRegsInitialize(ViPipe);

    if (pstIspCtx->bChangeWdrMode == GK_TRUE) {
        ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_WDR_MODE_SET, (GK_VOID *)&pstIspCtx->u8SnsWDRMode);
    }

    if ((pstIspCtx->bChangeIspRes == GK_TRUE) || (pstIspCtx->bChangeImageMode == GK_TRUE)) {
        ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_CHANGE_IMAGE_MODE_SET, (GK_VOID *)&stIspImageMode);
    }
    ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET,
                 (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

    s32Ret = ISP_WDRCfgSet(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] ISP_WDRCfgSet error \n", ViPipe);
        return s32Ret;
    }

    s32Ret = ISP_SwitchRegSet(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set reg config failed!\n", ViPipe);
        return GK_FAILURE;
    }

    ISP_SensorUpdateSnsReg(ViPipe);
    s32Ret = ISP_SensorSwitch(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init sensor failed!\n", ViPipe);
        return GK_FAILURE;
    }

    pstIspCtx->u8PreSnsWDRMode = pstIspCtx->u8SnsWDRMode;

    if (pstIspCtx->bChangeWdrMode == GK_TRUE) {
        ext_top_wdr_switch_write(ViPipe, GK_TRUE);
    }

    if ((pstIspCtx->bChangeIspRes == GK_TRUE) || (pstIspCtx->bChangeImageMode == GK_TRUE)) {
        ext_top_res_switch_write(ViPipe, GK_TRUE);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SwitchRes(VI_PIPE ViPipe)
{
    GK_U8  u8WDRMode;
    GK_S32 s32Ret = GK_SUCCESS;
    GK_U32 u32Value = 0;
    GK_VOID *pValue = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stIspImageMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8WDRMode = ext_system_sensor_wdr_mode_read(ViPipe);
    u32Value  = ext_system_fps_base_read(ViPipe);
    pValue    = (GK_VOID *)&u32Value;

    stIspImageMode.u16Width  = ext_sync_total_width_read(ViPipe);
    stIspImageMode.u16Height = ext_sync_total_height_read(ViPipe);
    stIspImageMode.f32Fps    = *(GK_FLOAT *)pValue;
    stIspImageMode.u8SnsMode = ext_system_sensor_mode_read(ViPipe);

    s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->stBlockAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Update ISP Block Attr Failed !\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->stPipeSize);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get Pipe size failed\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->bIsp0P2En);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get p2en info failed\n", ViPipe);
        return s32Ret;
    }

    pstIspCtx->stIspParaRec.bStitchSync = GK_FALSE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->stIspParaRec.bStitchSync);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set isp stitch sync failed!\n", ViPipe);
    }

    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_RES_SWITCH_SET);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Config ISP Res Switch Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_RegCfgCtrl(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = ISP_GetBeLastBuf(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get be last bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    ISP_SensorUpdateDefault(ViPipe);
    ISP_SensorUpdateBlc(ViPipe);

    ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_CHANGE_IMAGE_MODE_SET, (GK_VOID *)&stIspImageMode);
    ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET,
                 (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

    if (IS_FS_WDR_MODE(u8WDRMode)) {
        s32Ret = ISP_WDRCfgSet(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] ISP_WDRCfgSet ec %#x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    s32Ret = ISP_SwitchRegSet(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set reg config failed!\n", ViPipe);
        return GK_FAILURE;
    }

    ISP_SensorUpdateSnsReg(ViPipe);
    s32Ret = ISP_SensorSwitch(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d init sensor failed!\n", ViPipe);
        return GK_FAILURE;
    }

    ext_top_res_switch_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 ISP_SwitchMode(VI_PIPE ViPipe)
{
    GK_BOOL bHDREnable;
    GK_U8   u8WDRMode, u8HDRMode;
    GK_U32  u32Value = 0;
    GK_S32  s32Ret   = GK_SUCCESS;
    GK_VOID   *pValue    = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CMOS_DEFAULT_S  *pstSnsDft      = GK_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stSnsImageMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    u8WDRMode = ext_system_sensor_wdr_mode_read(ViPipe);
    u32Value  = ext_system_fps_base_read(ViPipe);
    pValue    = (GK_VOID *)&u32Value;

    pstIspCtx->stSnsImageMode.f32Fps    = *(GK_FLOAT *)pValue;
    pstIspCtx->stSnsImageMode.u16Width  = ext_top_sensor_width_read(ViPipe);
    pstIspCtx->stSnsImageMode.u16Height = ext_top_sensor_height_read(ViPipe);
    pstIspCtx->stSnsImageMode.u8SnsMode = ext_system_sensor_mode_read(ViPipe);
    pstIspCtx->stSysRect.s32X           = ext_system_corp_pos_x_read(ViPipe);
    pstIspCtx->stSysRect.s32Y           = ext_system_corp_pos_y_read(ViPipe);

    if (pstIspCtx->u8IspImageModeFlag == 0) {
        ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET,
                     (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

        memcpy(&pstIspCtx->stPreSnsImageMode, &pstIspCtx->stSnsImageMode, sizeof(ISP_SENSOR_IMAGE_MODE_S));

        pstIspCtx->u8IspImageModeFlag = 1;
    } else {
        if (pstIspCtx->u8SnsWDRMode != u8WDRMode) {
            pstIspCtx->bChangeWdrMode = GK_TRUE;
        }

        if ((pstIspCtx->stSnsImageMode.u16Width  != pstIspCtx->stPreSnsImageMode.u16Width ) ||
            (pstIspCtx->stSnsImageMode.u16Height != pstIspCtx->stPreSnsImageMode.u16Height) ||
            (pstIspCtx->stSnsImageMode.f32Fps    != pstIspCtx->stPreSnsImageMode.f32Fps) ||
            (pstIspCtx->stSnsImageMode.u8SnsMode != pstIspCtx->stPreSnsImageMode.u8SnsMode)) {
            pstIspCtx->bChangeImageMode = GK_TRUE;
        }

        if (pstIspCtx->stSysRect.u32Width  != ext_sync_total_width_read(ViPipe) ||
            pstIspCtx->stSysRect.u32Height != ext_sync_total_height_read(ViPipe)) {
            pstIspCtx->stSysRect.u32Width  = ext_sync_total_width_read(ViPipe);
            pstIspCtx->stSysRect.u32Height = ext_sync_total_height_read(ViPipe);

            pstIspCtx->bChangeIspRes = GK_TRUE;
        }

        if ((pstIspCtx->bChangeWdrMode == GK_TRUE) ||
            (pstIspCtx->bChangeImageMode == GK_TRUE) ||
            (pstIspCtx->bChangeIspRes == GK_TRUE)) {
            /* HDR attribute */
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->stHdrAttr);
            if (s32Ret != GK_SUCCESS) {
                ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get HDR attr failed\n", ViPipe);
                return s32Ret;
            }

            if (pstIspCtx->bChangeWdrMode == GK_TRUE) {
                bHDREnable = (pstIspCtx->stHdrAttr.enDynamicRange == DYNAMIC_RANGE_XDR) ? 1 : 0;
                u8HDRMode  = (((bHDREnable) & 0x1) << 0x6);
                u8HDRMode  = u8HDRMode | ext_system_sensor_wdr_mode_read(ViPipe);

                s32Ret = ISP_SensorSetWDRMode(ViPipe, u8HDRMode);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set sensor wdr mode failed\n", ViPipe);
                    return s32Ret;
                }
            }

            stSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
            stSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
            stSnsImageMode.f32Fps    = pstIspCtx->stSnsImageMode.f32Fps;
            stSnsImageMode.u8SnsMode = pstIspCtx->stSnsImageMode.u8SnsMode;

            memcpy(&pstIspCtx->stPreSnsImageMode, &pstIspCtx->stSnsImageMode, sizeof(ISP_SENSOR_IMAGE_MODE_S));

            s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);
            if (s32Ret == GK_SUCCESS) {  /* Need to init sensor */
                pstIspCtx->u8SnsWDRMode = u8WDRMode;
                pstIspCtx->u8Count      = 0;
                pstIspCtx->stSpecialOpt.bFeSttUpdate = GK_TRUE; /* used for fe statistics */

                s32Ret = ISP_SwitchModeProcess(ViPipe);
                if (s32Ret != GK_SUCCESS) {
                    ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Switch Mode failed!\n", ViPipe);
                    return s32Ret;
                }
            } else {
                if (pstIspCtx->bChangeIspRes == GK_TRUE) {
                    ISP_SwitchRes(ViPipe);
                }
                ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_AE_FPS_BASE_SET, (GK_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);
                ext_top_res_switch_write(ViPipe, GK_TRUE);
            }

            ISP_SensorGetDefault(ViPipe, &pstSnsDft);

            pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorID = pstSnsDft->stSensorMode.u32SensorID;
            pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorMode = pstSnsDft->stSensorMode.u8SensorMode;

            ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
            ISP_SwitchStateSet(ViPipe);

            memcpy(&pstIspCtx->stDngInfoCtrl.pstIspDng->stDngRawFormat, &pstSnsDft->stSensorMode.stDngRawFormat,
                   sizeof(DNG_RAW_FORMAT_S));

            if ((pstSnsRegsInfo->u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || (pstSnsRegsInfo->u8Cfg2ValidDelayMax < 1)) {
                ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Delay of config to invalid is:0x%x\n", ViPipe, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
                pstSnsRegsInfo->u8Cfg2ValidDelayMax = 1;
            }

            pstIspCtx->stLinkage.u8Cfg2ValidDelayMax = pstSnsRegsInfo->u8Cfg2ValidDelayMax;
        }

        pstIspCtx->bChangeWdrMode   = GK_FALSE;
        pstIspCtx->bChangeImageMode = GK_FALSE;
        pstIspCtx->bChangeIspRes    = GK_FALSE;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SwitchWDRMode(VI_PIPE ViPipe)
{
    GK_U8  u8WDRMode;
    GK_S32 s32Ret = GK_SUCCESS;
    GK_U32 u32WDRmode;
    GK_U32 u32Value = 0;
    GK_BOOL bHDREnable;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
    GK_VOID *pValue = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    u32WDRmode = ext_system_sensor_wdr_mode_read(ViPipe);

    /* swtich linear/WDR mode, width/height, fps  */
    if (u32WDRmode != pstIspCtx->u8SnsWDRMode) {
        pstIspCtx->u8SnsWDRMode = u32WDRmode;
        pstIspCtx->u8Count = 0;

        s32Ret = ISP_ResetFeSttEn(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Reset FeStt enable failed\n", ViPipe);
            return s32Ret;
        }

        /* Get info: PipeBindDev, RunningMode, WDR, HDR, Stitch */
        /* WDR attribute */
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &pstIspCtx->stWdrAttr);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get WDR attr failed\n", ViPipe);
            return s32Ret;
        }

        /* HDR attribute */
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->stHdrAttr);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get HDR attr failed\n", ViPipe);
            return s32Ret;
        }

        bHDREnable = (pstIspCtx->stHdrAttr.enDynamicRange == DYNAMIC_RANGE_XDR) ? 1 : 0;
        u8WDRMode = (((bHDREnable) & 0x1) << 0x6);
        u8WDRMode = u8WDRMode | ext_system_sensor_wdr_mode_read(ViPipe);

        s32Ret = ISP_SensorSetWDRMode(ViPipe, u8WDRMode);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set sensor wdr mode failed\n", ViPipe);
            return s32Ret;
        }

        stSnsImageMode.u16Width = ext_top_sensor_width_read(ViPipe);
        stSnsImageMode.u16Height = ext_top_sensor_height_read(ViPipe);
        u32Value = ext_system_fps_base_read(ViPipe);
        pValue = (GK_VOID *)&u32Value;
        stSnsImageMode.f32Fps = *(GK_FLOAT *)pValue;
        stSnsImageMode.u8SnsMode = ext_system_sensor_mode_read(ViPipe);
        s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);

        pstIspCtx->bMemInit = GK_TRUE;
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit);
        if (s32Ret != GK_SUCCESS) {
            pstIspCtx->bMemInit = GK_FALSE;
            VReg_ReleaseAll(ViPipe);

            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set Mem info failed!\n", ViPipe);
            return s32Ret;
        }

        pstIspCtx->u8SnsWDRMode              = u32WDRmode;
        pstIspCtx->stSpecialOpt.bFeSttUpdate = GK_TRUE; /* used for fe statistics */

        s32Ret = ISP_SwitchWDR(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Switch WDR failed!\n", ViPipe);
            return s32Ret;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_UpdatePosGet(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_S32 s32Ret;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_UPDATE_POS_GET, &pstIspCtx->stLinkage.u32UpdatePos);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d get update pos %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_U32 ISP_FrameCntGet(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    GK_S32 s32Ret;
    GK_U32 u32FrmCnt = 0;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_FRAME_CNT_GET, &u32FrmCnt);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe:%d get update pos %x!\n", ViPipe, s32Ret);
        return pstIspCtx->stLinkage.u32IsoDoneFrmcnt;
    }

    return u32FrmCnt;
}

GK_S32 ISP_SnapAttrGet(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_SNAP_ATTR_S stIspSnapAttr;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_WORKING_MODE_S stIspWorkMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SNAP_ATTR_GET, &stIspSnapAttr);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }
    memcpy(&pstIspCtx->stProParam, &stIspSnapAttr.stProParam, sizeof(ISP_PRO_PARAM_S));
    pstIspCtx->stLinkage.enSnapType = stIspSnapAttr.enSnapType;
    if ((stIspSnapAttr.s32PicturePipeId == stIspSnapAttr.s32PreviewPipeId) && (stIspSnapAttr.s32PreviewPipeId != -1)) {
        pstIspCtx->stLinkage.enSnapPipeMode = ISP_SNAP_PREVIEW_PICTURE;
    } else if (stIspSnapAttr.s32PicturePipeId == ViPipe) {
        pstIspCtx->stLinkage.enSnapPipeMode = ISP_SNAP_PICTURE;
    } else if (stIspSnapAttr.s32PreviewPipeId == ViPipe) {
        pstIspCtx->stLinkage.enSnapPipeMode = ISP_SNAP_PREVIEW;
    } else {
        pstIspCtx->stLinkage.enSnapPipeMode = ISP_SNAP_NONE;
    }
    pstIspCtx->stLinkage.bLoadCCM = stIspSnapAttr.bLoadCCM;
    pstIspCtx->stLinkage.s32PicturePipeId = stIspSnapAttr.s32PicturePipeId;
    pstIspCtx->stLinkage.s32PreviewPipeId = stIspSnapAttr.s32PreviewPipeId;

    if (stIspSnapAttr.s32PicturePipeId != -1) {
        s32Ret = ioctl(g_as32IspFd[stIspSnapAttr.s32PicturePipeId], ISP_WORK_MODE_GET, &stIspWorkMode);

        if (s32Ret) {
            ISP_TRACE(MODULE_DBG_ERR, "get isp work mode failed!\n");
            return s32Ret;
        }
        pstIspCtx->stLinkage.enPictureRunningMode = stIspWorkMode.enIspRunningMode;
    } else {
        pstIspCtx->stLinkage.enPictureRunningMode = ISP_MODE_RUNNING_BUTT;
    }

    if (stIspSnapAttr.s32PicturePipeId == stIspSnapAttr.s32PreviewPipeId) {
        pstIspCtx->stLinkage.enPreviewRunningMode = pstIspCtx->stLinkage.enPictureRunningMode;
    } else {
        if (stIspSnapAttr.s32PreviewPipeId != -1) {
            s32Ret = ioctl(g_as32IspFd[stIspSnapAttr.s32PreviewPipeId], ISP_WORK_MODE_GET, &stIspWorkMode);

            if (s32Ret) {
                ISP_TRACE(MODULE_DBG_ERR, "get isp work mode failed!\n");
                return s32Ret;
            }
            pstIspCtx->stLinkage.enPreviewRunningMode = stIspWorkMode.enIspRunningMode;
        } else {
            pstIspCtx->stLinkage.enPreviewRunningMode = ISP_MODE_RUNNING_BUTT;
        }
    }

    return GK_SUCCESS;
}

GK_U8 ISP_IsoIndexCal(VI_PIPE ViPipe, GK_U32 u32AeDoneFrmCnt)
{
    GK_U8 u8Index;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8Index = pstIspCtx->stLinkage.u8Cfg2ValidDelayMax;

    if (pstIspCtx->stLinkage.u32IsoDoneFrmcnt > u32AeDoneFrmCnt) {  // preview pipe the last frame dont run complete.
        u8Index = u8Index - (pstIspCtx->stLinkage.u32IsoDoneFrmcnt - u32AeDoneFrmCnt);
    } else if (pstIspCtx->stLinkage.u32IsoDoneFrmcnt < u32AeDoneFrmCnt) { // the preview pipe run first.
        u8Index = u8Index + ((u32AeDoneFrmCnt - pstIspCtx->stLinkage.u32IsoDoneFrmcnt) - 1);
    } else if (pstIspCtx->stLinkage.u32IsoDoneFrmcnt == u32AeDoneFrmCnt) { // the picture pipe run first.
        u8Index = u8Index - 1;
    }

    if (0 < pstIspCtx->stLinkage.u32UpdatePos) {
        u8Index = u8Index - 1;
    }

    u8Index = MIN2(u8Index, ISP_SYNC_ISO_BUF_MAX - 1);

    return u8Index;
}

GK_S32 ISP_SnapPreProcess(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CTX_S *pstPreviewIspCtx = GK_NULL;
    SNAP_TYPE_E enSnapType;
    ISP_SNAP_PIPE_MODE_E enSnapPipeMode;
    GK_U8 u8Index = 0;
    GK_U32 u32PrePipeFrmCnt = 0;
    GK_U8 i;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_SnapAttrGet(ViPipe);

    enSnapType = pstIspCtx->stLinkage.enSnapType;
    enSnapPipeMode = pstIspCtx->stLinkage.enSnapPipeMode;

    if (enSnapPipeMode == ISP_SNAP_NONE) {
        return GK_SUCCESS;
    }

    if ((pstIspCtx->stLinkage.s32PreviewPipeId > -1) && (pstIspCtx->stLinkage.s32PreviewPipeId < ISP_MAX_PIPE_NUM)) {
        ISP_GET_CTX(pstIspCtx->stLinkage.s32PreviewPipeId, pstPreviewIspCtx);
        u32PrePipeFrmCnt = pstPreviewIspCtx->stLinkage.u32IsoDoneFrmcnt;
    }

    if (((enSnapPipeMode == ISP_SNAP_PICTURE) || (enSnapPipeMode == ISP_SNAP_PREVIEW_PICTURE)) &&
        (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
         IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))) {
        ISP_SnapRegCfgGet(ViPipe, &pstIspCtx->stSnapIspInfo);

        if (pstIspCtx->stSnapIspInfo.enSnapState == SNAP_STATE_CFG) {
            pstIspCtx->stLinkage.bSnapState = GK_TRUE;
            pstIspCtx->stLinkage.u32Iso = pstIspCtx->stSnapIspInfo.stIspCfgInfo.u32ISO;
            pstIspCtx->stLinkage.u32IspDgain = pstIspCtx->stSnapIspInfo.stIspCfgInfo.u32IspDgain;
            pstIspCtx->stLinkage.u32ColorTemp = pstIspCtx->stSnapIspInfo.stIspCfgInfo.u32ColorTemperature;
            pstIspCtx->stLinkage.u8ProIndex = pstIspCtx->stSnapIspInfo.u32ProIndex;
        } else {
            pstIspCtx->stLinkage.bSnapState = GK_FALSE;
            pstIspCtx->stLinkage.u8ProIndex = 0;
        }
    }

    if ((enSnapPipeMode == ISP_SNAP_PREVIEW) || (enSnapPipeMode == ISP_SNAP_PREVIEW_PICTURE)) {
        if (enSnapType == SNAP_TYPE_PRO) {
            pstIspCtx->stLinkage.bProTrigger = ISP_ProTriggerGet(ViPipe);
            if (pstIspCtx->stLinkage.bProTrigger == GK_TRUE) {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, ISP_PROTRIGGER_SET, (GK_VOID *)&pstIspCtx->stProParam);
            }
        }

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            pstIspCtx->stLinkage.au8ProIndexBuf[i] = pstIspCtx->stLinkage.au8ProIndexBuf[i - 1];
        }
        if ((pstIspCtx->stLinkage.bProTrigger == GK_TRUE) ||
            ((pstIspCtx->u8ProFrmCnt > 0) && (pstIspCtx->u8ProFrmCnt < pstIspCtx->stProParam.u8ProFrameNum))) {
            pstIspCtx->stLinkage.au8ProIndexBuf[0] = pstIspCtx->u8ProFrmCnt + 1;
        } else {
            pstIspCtx->stLinkage.au8ProIndexBuf[0] = 0;
        }
        pstIspCtx->stLinkage.u8ProIndex = pstIspCtx->stLinkage.au8ProIndexBuf[pstIspCtx->stLinkage.u8Cfg2ValidDelayMax];
        if (0 < pstIspCtx->stLinkage.u32UpdatePos) {
            pstIspCtx->stLinkage.u8ProIndex = pstIspCtx->stLinkage.au8ProIndexBuf[pstIspCtx->stLinkage.u8Cfg2ValidDelayMax - 1];
        }
    }

    if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
        IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        if (enSnapPipeMode == ISP_SNAP_PICTURE) {
            u8Index = ISP_IsoIndexCal(ViPipe, u32PrePipeFrmCnt);
            if (pstPreviewIspCtx != GK_NULL) {
                pstIspCtx->stLinkage.u32Iso = pstPreviewIspCtx->stLinkage.au32SyncIsoBuf[u8Index];
                pstIspCtx->stLinkage.u8ProIndex = pstPreviewIspCtx->stLinkage.au8ProIndexBuf[u8Index];
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SaveSnapInfo(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CONFIG_INFO_S stSnapInfo;
    GK_U32 i;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    stSnapInfo.u32ColorTemperature = pstIspCtx->stLinkage.u32ColorTemp;
    stSnapInfo.u32ISO = pstIspCtx->stLinkage.au32SyncIsoBuf[0];
    stSnapInfo.u32IspDgain = pstIspCtx->stLinkage.u32IspDgain;
    stSnapInfo.u32ExposureTime = pstIspCtx->stLinkage.u32Inttime;
    stSnapInfo.au32WhiteBalanceGain[0] = (pstIspCtx->stLinkage.au32WhiteBalanceGain[0] + 128) >> 8;
    stSnapInfo.au32WhiteBalanceGain[1] = (pstIspCtx->stLinkage.au32WhiteBalanceGain[1] + 128) >> 8;
    stSnapInfo.au32WhiteBalanceGain[2] = (pstIspCtx->stLinkage.au32WhiteBalanceGain[2] + 128) >> 8;
    stSnapInfo.au32WhiteBalanceGain[3] = (pstIspCtx->stLinkage.au32WhiteBalanceGain[3] + 128) >> 8;

    for (i = 0; i < ISP_CAP_CCM_NUM; i++) {
        stSnapInfo.au16CapCCM[i] = pstIspCtx->stLinkage.au16CCM[i];
    }

    ISP_SnapRegCfgSet(ViPipe, &stSnapInfo);

    return GK_SUCCESS;
}

GK_S32 ISP_SetSmartInfo(VI_PIPE ViPipe, const ISP_SMART_INFO_S *pstSmartInfo)
{
    GK_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSmartInfo);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        ext_system_smart_enable_write(ViPipe, i, pstSmartInfo->stROI[i].bEnable);
        ext_system_smart_available_write(ViPipe, i, pstSmartInfo->stROI[i].bAvailable);
        ext_system_smart_luma_write(ViPipe, i, pstSmartInfo->stROI[i].u8Luma);
    }

    ext_system_smart_update_write(ViPipe, GK_TRUE);

    return GK_SUCCESS;
}

GK_S32 ISP_GetSmartInfo(VI_PIPE ViPipe, ISP_SMART_INFO_S *pstSmartInfo)
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

GK_S32 ISP_SetProCalcDone(VI_PIPE ViPipe)
{
    GK_S32 s32Ret = 0;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SET_PROCALCDONE);

    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "set isp pro calculate done failed!\n");
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_StitchSyncExit(VI_PIPE ViPipe)
{
    GK_U32  k;
    VI_PIPE ViPipeS;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    /* 1. check status */
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (( pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) && (pstIspCtx->stStitchAttr.bMainPipe != GK_TRUE )) {
        for (k = 0; k < pstIspCtx->stStitchAttr.u8StitchPipeNum; k++) {
            ViPipeS = pstIspCtx->stStitchAttr.as8StitchBindId[k];
            ISP_GET_CTX(ViPipeS, pstIspCtx);

            while ((pstIspCtx->stStitchAttr.bMainPipe == GK_TRUE) && (pstIspCtx->bMemInit != GK_FALSE )) {
                usleep(10);
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_StitchSyncRun(VI_PIPE ViPipe)
{
    GK_U8 k;
    VI_PIPE ViPipeId;
    ISP_CTX_S *pstIspCtx  = GK_NULL;
    ISP_CTX_S *pstIspCtxS = GK_NULL;

    /* 1. check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE ) {
        if (pstIspCtx->stStitchAttr.bMainPipe == GK_TRUE ) {
            for (k = 0; k < pstIspCtx->stStitchAttr.u8StitchPipeNum; k++) {
                ViPipeId = pstIspCtx->stStitchAttr.as8StitchBindId[k];
                ISP_GET_CTX(ViPipeId, pstIspCtxS);

                if (pstIspCtxS->bMemInit != GK_TRUE) {
                    return GK_FAILURE;
                }

                if (pstIspCtxS->stIspParaRec.bInit != GK_TRUE) {
                    return GK_FAILURE;
                }
            }
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_StitchIsoSync(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CTX_S *pstMainIspCtx = GK_NULL;
    GK_U8 u8Index;
    GK_U32 u32MainPipeFrmCnt;
    VI_PIPE MainPipe;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE ) {
        if (pstIspCtx->stStitchAttr.bMainPipe != GK_TRUE ) {
            MainPipe = pstIspCtx->stStitchAttr.as8StitchBindId[0];
            ISP_CHECK_PIPE(MainPipe);
            ISP_GET_CTX(ViPipe, pstMainIspCtx);

            u32MainPipeFrmCnt = pstMainIspCtx->stLinkage.u32IsoDoneFrmcnt;
            u8Index = ISP_IsoIndexCal(ViPipe, u32MainPipeFrmCnt);
            pstIspCtx->stLinkage.u32Iso = pstMainIspCtx->stLinkage.au32SyncIsoBuf[u8Index];
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_Run(VI_PIPE ViPipe)
{
    GK_S32 s32Ret = GK_SUCCESS;
    GK_VOID *pStat = GK_NULL;
    GK_VOID *pRegCfg = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ISP_StitchSyncRun(ViPipe);
    if (s32Ret != GK_SUCCESS ) {
        return GK_SUCCESS;
    }

    /*  get isp BeBuf info. */
    s32Ret = ISP_GetBeFreeBuf(ViPipe);
    if (s32Ret != GK_SUCCESS ) {
        return s32Ret;
    }

    /*  get statistics buf info. */
    s32Ret = ISP_StatisticsGetBuf(ViPipe, &pStat);
    if (s32Ret != GK_SUCCESS ) {
        if (pstIspCtx->u32FrameCnt != 0) {
            return s32Ret;
        }

        pstIspCtx->stLinkage.bStatReady = GK_FALSE;
    } else {
        pstIspCtx->stLinkage.bStatReady = GK_TRUE;
    }

    s32Ret = ISP_SetCfgBeBufState(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    /* get regcfg */
    ISP_GetRegCfgCtx(ViPipe, &pRegCfg);

    ISP_ReadExtregs(ViPipe);

    if (pstIspCtx->bFreezeFw) {
        ISP_RegCfgInfoSet(ViPipe);
        return GK_SUCCESS;
    }

    ISP_UpdatePosGet(ViPipe);

    ISP_SnapPreProcess(ViPipe);

    ISP_StitchIsoSync(ViPipe);

    pstIspCtx->u32FrameCnt++;

    ISP_DbgRunBgn(&pstIspCtx->stIspDbg, pstIspCtx->u32FrameCnt);

    ISP_AlgsRun(pstIspCtx->astAlgs, ViPipe, pStat, pRegCfg, 0);

    /* update info */
    if (pstIspCtx->stLinkage.enSnapPipeMode != ISP_SNAP_PICTURE) {
        ISP_UpdateInfo(ViPipe);

        ISP_UpdateFrameInfo(ViPipe);
    }

    ISP_ProcWrite(pstIspCtx->astAlgs, ViPipe);

    ISP_UpdateAttachInfo(ViPipe);

    ISP_UpdateColorGamutinfo(ViPipe);

    ISP_DbgRunEnd(&pstIspCtx->stIspDbg, pstIspCtx->u32FrameCnt);

    /* release statistics buf info. */
    if (pstIspCtx->stLinkage.bStatReady == GK_TRUE) {
        ISP_StatisticsPutBuf(ViPipe);
    }

    /* record the register config infomation to fhy and kernel,and be valid in next frame. */
    s32Ret = ISP_RegCfgInfoSet(ViPipe);
    if (s32Ret != GK_SUCCESS ) {
        return s32Ret;
    }

    if (((pstIspCtx->u8ProFrmCnt > 0) && (pstIspCtx->u8ProFrmCnt < pstIspCtx->stProParam.u8ProFrameNum + 4)) ||
        ( pstIspCtx->stLinkage.bProTrigger == GK_TRUE)) {
        pstIspCtx->u8ProFrmCnt++;
    } else {
        pstIspCtx->u8ProFrmCnt = 0;
    }

    if ((pstIspCtx->u32FrameCnt % DIV_0_TO_1(pstIspCtx->stLinkage.u8AERunInterval)  == 0) ||
        (pstIspCtx->u8ProFrmCnt > 0)) {
        if (!pstIspCtx->stLinkage.bDefectPixel) {
            ISP_SyncCfgSet(ViPipe);
        }
    }

    ISP_UpdateExtRegs(ViPipe);

    /* save capture info */
    ISP_SaveSnapInfo(ViPipe);

    ISP_UpdateDngImageDynamicInfo(ViPipe);

    /* pro snap mode, ae calculate done */
    if (pstIspCtx->u8ProFrmCnt == 1) {
        ISP_SetProCalcDone(ViPipe);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_Exit(VI_PIPE ViPipe)
{
    GK_U8  i;
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    const GK_U64 u64Handsignal = ISP_EXIT_HAND_SIGNAL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    /* Set handsignal */
    if (ioctl(g_as32IspFd[ViPipe], ISP_RUN_STATE_SET, &u64Handsignal)) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set run state failed!\n", ViPipe);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    memset(&pstIspCtx->stIspParaRec, 0, sizeof(ISP_PARA_REC_S));

    ext_top_wdr_cfg_write(ViPipe, pstIspCtx->stIspParaRec.bWDRCfg);
    ext_top_pub_attr_cfg_write(ViPipe, pstIspCtx->stIspParaRec.bPubCfg);

    if (pstIspCtx->bISPYuvMode == GK_FALSE) {
        s32Ret |= ISP_SensorExit(ViPipe);
    }

    ISP_AlgsExit(ViPipe, pstIspCtx->astAlgs);

    ISP_AlgsUnRegister(ViPipe);

    s32Ret |= ISP_RegCfgExit(ViPipe);
    s32Ret |= ISP_ProShpParamExit(ViPipe);
    s32Ret |= ISP_ProNrParamExit(ViPipe);
    s32Ret |= ISP_ProInfoExit(ViPipe);
    s32Ret |= ISP_DngInfoExit(ViPipe);
    s32Ret |= ISP_ColorGamutInfoExit(ViPipe);
    s32Ret |= ISP_AttachInfoExit(ViPipe);
    s32Ret |= ISP_FrameInfoExit(ViPipe);
    s32Ret |= ISP_UpdateInfoExit(ViPipe);
    s32Ret |= ISP_TransInfoExit(ViPipe);
    s32Ret |= ISP_ProcExit(ViPipe);
    s32Ret |= ISP_StatisticsExit(ViPipe);
    s32Ret |= ISP_SpecAwbBufExit(ViPipe);
    s32Ret |= ISP_ClutBufExit(ViPipe);
    s32Ret |= ISP_SttBufExit(ViPipe);
    s32Ret |= ISP_CfgBeBufExit(ViPipe);
    s32Ret |= ISP_LdciBufExit(ViPipe);
    s32Ret |= ISP_BlockExit(ViPipe);
    /* exit global variables */
    pstIspCtx->bMemInit                 = GK_FALSE;
    pstIspCtx->stIspParaRec.bInit       = GK_FALSE;
    pstIspCtx->stIspParaRec.bStitchSync = GK_FALSE;
    pstIspCtx->stLinkage.bRunOnce       = GK_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstIspCtx->pIspBeVirtAddr[i]  = GK_NULL;
        pstIspCtx->pViProcVirtAddr[i] = GK_NULL;
    }

    if (ioctl(g_as32IspFd[ViPipe], ISP_RESET_CTX)) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] reset ctx failed!\n", ViPipe);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    /* release vregs */
    VReg_Munmap(ISP_FE_REG_BASE(ViPipe), FE_REG_SIZE_ALIGN);
    VReg_Munmap(ISP_BE_REG_BASE(ViPipe), BE_REG_SIZE_ALIGN);
    VReg_Exit(ViPipe, ISP_VIR_REG_BASE(ViPipe), ISP_VREG_SIZE);
    VReg_ReleaseAll(ViPipe);

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
