/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_main.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

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
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_UpdateInfo(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    
    ISP_CHECK_OPEN(IspDev);

    if (HI_NULL == pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo)
    {
        printf("the isp update info buf hasn't init!\n");
        return HI_FAILURE;
    }

    memcpy(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo+1, pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));

    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (pstIspCtx->astAlgs[i].bUsed)
        {
            if (HI_NULL != pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl)
            {
                pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl(IspDev, ISP_UPDATE_INFO_GET, pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo);
            }
        }
    }
       
    return HI_SUCCESS;
}

HI_S32 ISP_UpdateFrameInfo(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    
    ISP_CHECK_OPEN(IspDev);

    if (!pstIspCtx->stFrameInfoCtrl.bSupportFrame)
    {
        return HI_FAILURE;
    }
  
    if (HI_NULL == pstIspCtx->stFrameInfoCtrl.pstIspFrame)
    {
        printf("the Isp Frame Info hasn't init!\n");
        return HI_FAILURE;
    }

    memcpy(pstIspCtx->stFrameInfoCtrl.pstIspFrame+1, pstIspCtx->stFrameInfoCtrl.pstIspFrame, sizeof(FRAME_SUPPLEMENT_INFO_S));

    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (pstIspCtx->astAlgs[i].bUsed)
        {
            if (HI_NULL != pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl)
            {
                pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl(IspDev, ISP_FRAMEINFO_GET, pstIspCtx->stFrameInfoCtrl.pstIspFrame);
            }
        }
    }
       
    return HI_SUCCESS;
}

HI_S32 ISP_ReadExtregs(ISP_DEV IspDev)
{
    //HI_U32 u32Value = 0;
    //HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_U8 i;

    ISP_GET_CTX(IspDev, pstIspCtx);

    pstIspCtx->bFreezeFw = hi_ext_system_freeze_firmware_read(IspDev);
    
    pstIspCtx->stIspDbg.bDebugEn = hi_ext_system_sys_debug_enable_read(IspDev);
    pstIspCtx->stIspDbg.u32PhyAddr = hi_ext_system_sys_debug_addr_read(IspDev);
    pstIspCtx->stIspDbg.u32Depth = hi_ext_system_sys_debug_depth_read(IspDev);
    pstIspCtx->stIspDbg.u32Size= hi_ext_system_sys_debug_size_read(IspDev);
#if 0
    pstIspCtx->stSnsImageMode.u16Width = hi_isp_top_active_width_read(IspDev);
    pstIspCtx->stSnsImageMode.u16Height = hi_isp_top_active_height_read(IspDev);
    u32Value = hi_ext_system_fps_base_read(IspDev);
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps =  *(HI_FLOAT *)pValue;

    if((pstIspCtx->stSnsImageMode.u16Width == pstIspCtx->stPreSnsImageMode.u16Width )&&(pstIspCtx->stSnsImageMode.u16Height == pstIspCtx->stPreSnsImageMode.u16Height))
    {
        if (pstIspCtx->stPreSnsImageMode.f32Fps != pstIspCtx->stSnsImageMode.f32Fps)
        {
            pstIspCtx->stPreSnsImageMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;
            ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET,
                (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);
        }
    }
#endif    
    pstIspCtx->stProParam.enOpType = hi_ext_system_pro_type_read(IspDev);
    pstIspCtx->stProParam.u8ProFrameNum = hi_ext_system_pro_frame_num_read(IspDev);
    for (i=0; i<pstIspCtx->stProParam.u8ProFrameNum; i++)
    {
        pstIspCtx->stProParam.stAuto.au16ProExpStep[i] = hi_ext_system_pro_exp_step_table_read(IspDev, i);
        pstIspCtx->stProParam.stManual.au32ManExpTime[i] = hi_ext_system_pro_man_exp_time_table_read(IspDev, i);
        pstIspCtx->stProParam.stManual.au32ManSysgain[i] = hi_ext_system_pro_man_sysgain_table_read(IspDev, i);
    }
    pstIspCtx->stLinkage.enSnapType = hi_ext_system_snap_type_read(IspDev);
    pstIspCtx->stLinkage.enSnapPipeMode = hi_ext_system_snap_pipe_mode_read(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_UpdateExtRegs(ISP_DEV IspDev)
{
#if 0
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_U32 u32IspBindAttr;

    ISP_GET_CTX(IspDev, pstIspCtx);

    /* update Isp bind attr to ext regs */
    u32IspBindAttr = (pstIspCtx->stBindAttr.stAeLib.s32Id << 8) | pstIspCtx->stBindAttr.stAwbLib.s32Id;
    hi_ext_system_bind_attr_write(IspDev, u32IspBindAttr);
#endif
    return HI_SUCCESS;
}

HI_S32 ISP_SwitchImageMode(ISP_DEV IspDev)
{
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stSnsImageMode;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stIspImageMode;
    ISP_RES_SWITCH_MODE_S stResSwitchMode;
    HI_S32 s32Ret1 = 0, s32Ret2 = 0, s32Ret3 = 0;
    
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstIspCtx->stSnsImageMode.u16Width = hi_ext_top_sensor_width_read(IspDev);
    pstIspCtx->stSnsImageMode.u16Height = hi_ext_top_sensor_height_read(IspDev);
    u32Value = hi_ext_system_fps_base_read(IspDev);
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;
    stIspImageMode.u16Width = hi_isp_top_active_width_read(IspDev);
    stIspImageMode.u16Height = hi_isp_top_active_height_read(IspDev);
    stIspImageMode.f32Fps =  pstIspCtx->stSnsImageMode.f32Fps; 

    if (0 == pstIspCtx->u8IspImageModeFlag)
    {
        ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET,
            (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

        pstIspCtx->stPreSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
        pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
        pstIspCtx->stPreSnsImageMode.f32Fps  = pstIspCtx->stSnsImageMode.f32Fps;
        pstIspCtx->u8IspImageModeFlag = 1;
    }
    else
    {
        if((pstIspCtx->stSnsImageMode.u16Width != pstIspCtx->stPreSnsImageMode.u16Width )||(pstIspCtx->stSnsImageMode.u16Height != pstIspCtx->stPreSnsImageMode.u16Height)||(pstIspCtx->stSnsImageMode.f32Fps != pstIspCtx->stPreSnsImageMode.f32Fps))
        {
            stSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
            stSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
            stSnsImageMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;

            pstIspCtx->stPreSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
            pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
            pstIspCtx->stPreSnsImageMode.f32Fps  = pstIspCtx->stSnsImageMode.f32Fps;

            s32Ret1 = ISP_SensorSetImageMode(IspDev, &stSnsImageMode);
            if(HI_SUCCESS == s32Ret1)    /* need to init sensor */
            {
                stResSwitchMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;
                stResSwitchMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
                stResSwitchMode.u16Width = pstIspCtx->stSnsImageMode.u16Width;
                
                s32Ret3 = ioctl(g_as32IspFd[IspDev], ISP_RES_SWITCH_SET, &stResSwitchMode);
                if (HI_SUCCESS != s32Ret3)
                {
                    printf("Config ISP Res Switch Failed with ec %#x!\n", s32Ret3);
                    return s32Ret3;
                }   

                ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_CHANGE_IMAGE_MODE_SET, (HI_VOID *)&stIspImageMode);
				ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET, (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

				ISP_SensorUpdateSnsReg(IspDev);
                s32Ret2 = ISP_SensorSwitch(IspDev);
                if (HI_SUCCESS != s32Ret2)
                {
                    printf("ISP[%d] init sensor failed!\n", IspDev);
                    return HI_FAILURE;
                }
            }			
			else
			{	
				ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET, (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);
			}

            hi_ext_top_res_switch_write(IspDev, HI_TRUE);

            /* get new sensor default param */
            ISP_SensorUpdateDefault(IspDev);
            ISP_SensorUpdateBlc(IspDev);
            ISP_SensorGetDefault(IspDev, &pstSnsDft);
            pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorID = pstSnsDft->stSensorMode.u32SensorID;
            pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorMode = pstSnsDft->stSensorMode.u8SensorMode;
            memcpy(&pstIspCtx->stDngInfoCtrl.pstIspDng->stDngRawFormat, &pstSnsDft->stSensorMode.stDngRawFormat, sizeof(DNG_RAW_FORMAT_S));
        }
    }

    return HI_SUCCESS;
    

}


HI_S32 ISP_WDRCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_WDR_CFG_S stWDRCfg;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    memset(&stWDRCfg, 0, sizeof(ISP_WDR_CFG_S));

    stWDRCfg.u8WDRMode = pstIspCtx->u8SnsWDRMode;
    memcpy(&stWDRCfg.au32ExpRatio, &pstSnsDft->stWDRAttr.au32ExpRatio, sizeof(stWDRCfg.au32ExpRatio));

    if (IS_LINEAR_MODE(stWDRCfg.u8WDRMode) || IS_BUILT_IN_WDR_MODE(stWDRCfg.u8WDRMode))
    {
        hi_isp_interrupts_interrupt0_source_write(IspDev, 31);   /* ISP Output frame start interrupt */
        hi_isp_interrupts_interrupt1_source_write(IspDev, 16);   /* ISP Input frame end interrupt */
    }
    else if (IS_LINE_WDR_MODE(stWDRCfg.u8WDRMode))
    {
        hi_isp_interrupts_interrupt0_source_write(IspDev, 31);   /* ISP Output frame start interrupt */
        hi_isp_interrupts_interrupt1_source_write(IspDev, 16);   /* ISP Input frame end interrupt */
    }
    else
    {
        hi_isp_interrupts_interrupt0_source_write(IspDev, 31);   /* ISP Output frame start interrupt */
        hi_isp_interrupts_interrupt1_source_write(IspDev, 16);   /* ISP Input frame end interrupt */
    }
    
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_WDR_CFG_SET, &stWDRCfg);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Config WDR register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SwitchWDRMode(ISP_DEV IspDev)
{
    HI_U8   u8SensorWDRMode = 0;
    HI_S32  s32Ret = 0;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);

    u8SensorWDRMode = hi_ext_system_sensor_wdr_mode_read(IspDev);
    if (pstIspCtx->u8SnsWDRMode == u8SensorWDRMode)
    {
        return HI_SUCCESS;
    }
    
    pstIspCtx->u8SnsWDRMode = u8SensorWDRMode;
    
    /* 1. notify sensor to switch WDR mode */
    ISP_SensorSetWDRMode(IspDev, u8SensorWDRMode);
	
    /* 2. get new sensor default param */
    ISP_SensorUpdateDefault(IspDev);
    ISP_SensorUpdateBlc(IspDev);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorID = pstSnsDft->stSensorMode.u32SensorID;
    pstIspCtx->stFrameInfoCtrl.pstIspFrame->u32SensorMode = pstSnsDft->stSensorMode.u8SensorMode;
    memcpy(&pstIspCtx->stDngInfoCtrl.pstIspDng->stDngRawFormat, &pstSnsDft->stSensorMode.stDngRawFormat, sizeof(DNG_RAW_FORMAT_S));
    
    /* 3. init the common part of extern registers and real registers */
    ISP_ExtRegsInitialize(IspDev);
    ISP_RegsInitialize(IspDev);

    /* 4. notify isp kernel to switch WDR mode */
    ISP_WDRCfgSet(IspDev);

    /* 5. notify algs to switch WDR mode */
    ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_WDR_MODE_SET, (HI_VOID *)&u8SensorWDRMode);

    /* 6. switch sensor to WDR mode */
	ISP_SensorUpdateSnsReg(IspDev);
	s32Ret = ISP_SensorSwitch(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] init sensor failed!\n", IspDev);
        return HI_FAILURE;
    }

    pstIspCtx->u8PreSnsWDRMode = pstIspCtx->u8SnsWDRMode;
    hi_ext_top_wdr_switch_write(IspDev, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 ISP_UpdatePosGet(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;    
    HI_S32 s32Ret;
	
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_OPEN(IspDev);

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_UPDATE_POS_GET, &pstIspCtx->stLinkage.u32UpdatePos);
    if (s32Ret)
    {
        printf("get update pos %x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SnapPreProcess(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CONFIG_INFO_S stSnapInfo = {0};
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    
    if (SNAP_TYPE_NONE == pstIspCtx->stLinkage.enSnapType)
    {
        return HI_SUCCESS;
    }
    else if ((SNAP_TYPE_NORMAL == pstIspCtx->stLinkage.enSnapType) || (SNAP_TYPE_PRO == pstIspCtx->stLinkage.enSnapType) || (SNAP_TYPE_SYNC_PRO == pstIspCtx->stLinkage.enSnapType))
    {
        if (ISP_SNAP_NONE == pstIspCtx->stLinkage.enSnapPipeMode)
        {
            return HI_SUCCESS;
        }
        
        if ((ISP_SNAP_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode) || (ISP_SNAP_PREVIEW_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode))
        {
            ISP_CapRegCfgGet(IspDev, &stSnapInfo);

            if (SNAP_STATE_CFG == stSnapInfo.enCapState)
            {
                pstIspCtx->stLinkage.bCapState = HI_TRUE;
                pstIspCtx->stLinkage.u32Iso = stSnapInfo.u32Iso;
                pstIspCtx->stLinkage.u32IspDgain = stSnapInfo.u32IspDgain;
                pstIspCtx->stLinkage.u32ColorTemp = stSnapInfo.u32ColorTemp;
            }
            else
            {
                pstIspCtx->stLinkage.bCapState = HI_FALSE;
            }
        }
		    
		if ((ISP_SNAP_PREVIEW == pstIspCtx->stLinkage.enSnapPipeMode) || (ISP_SNAP_PREVIEW_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode))
		{
		    if ((SNAP_TYPE_PRO == pstIspCtx->stLinkage.enSnapType) || (SNAP_TYPE_SYNC_PRO == pstIspCtx->stLinkage.enSnapType))
		    {
			    pstIspCtx->stLinkage.bProTrigger = ISP_ProTriggerGet(IspDev);
	            if (HI_TRUE == pstIspCtx->stLinkage.bProTrigger)
	            {
	                ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_PROTRIGGER_SET, (HI_VOID *)&pstIspCtx->stProParam);
	            }
		    }
		}
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SaveSnapInfo(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CONFIG_INFO_S stSnapInfo;
    HI_U32 i;

    ISP_GET_CTX(IspDev, pstIspCtx);

    if (SNAP_TYPE_NONE == pstIspCtx->stLinkage.enSnapType)
    {
        return HI_SUCCESS;
    }
    else if ((SNAP_TYPE_NORMAL == pstIspCtx->stLinkage.enSnapType) || (SNAP_TYPE_PRO == pstIspCtx->stLinkage.enSnapType))
    {
        if ((ISP_SNAP_PREVIEW == pstIspCtx->stLinkage.enSnapPipeMode) || (ISP_SNAP_PREVIEW_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode))
        {
            stSnapInfo.u32ColorTemp = pstIspCtx->stLinkage.u32ColorTemp;
            stSnapInfo.u32Iso = pstIspCtx->stLinkage.u32Iso;
            stSnapInfo.u32IspDgain = pstIspCtx->stLinkage.u32IspDgain;
            stSnapInfo.u32ExposureTime = pstIspCtx->stLinkage.u32Inttime;
            stSnapInfo.au32WhiteBalanceGain[0] = pstIspCtx->stLinkage.au32WhiteBalanceGain[0];
            stSnapInfo.au32WhiteBalanceGain[1] = pstIspCtx->stLinkage.au32WhiteBalanceGain[1];
            stSnapInfo.au32WhiteBalanceGain[2] = pstIspCtx->stLinkage.au32WhiteBalanceGain[2];
            stSnapInfo.au32WhiteBalanceGain[3] = pstIspCtx->stLinkage.au32WhiteBalanceGain[3];
            for (i=0; i<9; i++)
            {
                stSnapInfo.au16CapCCM[i] = pstIspCtx->stLinkage.au16CCM[i];
            }
            
            ISP_CapRegCfgSet(IspDev, &stSnapInfo);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_Run(ISP_DEV IspDev)
{
    HI_U32 i;
    HI_S32 s32Ret;   
    HI_VOID *pStat = HI_NULL;
    HI_VOID *pRegCfg = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    /*  get statistics buf info. */
    switch (g_astIspCtx[0].enIspMode) 
    {
        default:
        case ISP_MODE_NORMAL:
            s32Ret = ISP_StatisticsGetBuf(IspDev, &pStat);
            if (s32Ret)
            {
                //ISP_TRACE(HI_DBG_WARN, "ISP[%d] Statistics Get Buf failed!\n", IspDev);
                return s32Ret;
            }            
        break;
        
        case ISP_MODE_STITCH:           
            if(IS_MAIN_CHANNAL(IspDev))
            {
                for(i = 0; i < ISP_MAX_DEV_NUM; i++)
                {
                    if(g_astIspCtx[i].stIspParaRec.bInit != HI_TRUE)
                    {
                        return HI_FAILURE; 
                    }
                    
                    s32Ret = ISP_StatisticsGetBuf((ISP_DEV)i, &g_astIspCtx[i].pStatis);
                    if (s32Ret)
                    {
                        //ISP_TRACE(HI_DBG_WARN, "ISP[%d] Statistics Get Buf failed!\n", i);
                        return s32Ret;
                    } 
                }

                s32Ret = ISP_CalcStatisticsStit(g_astIspCtx[0].pStatis, g_astIspCtx[1].pStatis, &pStat);
                if (s32Ret)
                {
                    ISP_TRACE(HI_DBG_WARN, "ISP[%d] Calc Statistics Stit failed!\n", i);
                    return s32Ret;
                }
            }
        break;
    }    

    /* get regcfg */
    s32Ret = ISP_RegCfgInit(IspDev, &pRegCfg);
    if (s32Ret)
    {
        return s32Ret;
    }

    ISP_ReadExtregs(IspDev);

    if (pstIspCtx->bFreezeFw)
    {
        ISP_RegCfgSet(IspDev);
        
        return HI_SUCCESS;
    }

    ISP_UpdatePosGet(IspDev);    

    ISP_SnapPreProcess(IspDev);
    
    pstIspCtx->u32FrameCnt++;

    ISP_DbgRunBgn(&pstIspCtx->stIspDbg, pstIspCtx->u32FrameCnt);

    ISP_AlgsRun(pstIspCtx->astAlgs, IspDev, pStat, pRegCfg, 0);

    ISP_ProcWrite(pstIspCtx->astAlgs, IspDev);

    /* update info */
    /* picture pipe dont update info when capture*/
    if (((HI_TRUE != pstIspCtx->stLinkage.bCapState) && (SNAP_TYPE_SYNC != pstIspCtx->stLinkage.enSnapType) && (SNAP_TYPE_SYNC_PRO != pstIspCtx->stLinkage.enSnapType)) 
        || (ISP_SNAP_PICTURE != pstIspCtx->stLinkage.enSnapPipeMode))
    {
        ISP_UpdateInfo(IspDev);
        ISP_UpdateFrameInfo(IspDev);
    }

    ISP_DbgRunEnd(&pstIspCtx->stIspDbg, pstIspCtx->u32FrameCnt);

    /* release statistics buf info. */
    ISP_StatisticsPutBuf(IspDev);

    /* record the register config infomation to kernel,and be valid in next frame. */
    ISP_RegCfgSet(IspDev);

    if (((pstIspCtx->u8ProFrmCnt > 0) && (pstIspCtx->u8ProFrmCnt < pstIspCtx->stProParam.u8ProFrameNum)) || (HI_TRUE == pstIspCtx->stLinkage.bProTrigger))
    {
        pstIspCtx->u8ProFrmCnt++;
    }
    else
    {
        pstIspCtx->u8ProFrmCnt = 0;
    }

    if ((0 == pstIspCtx->u32FrameCnt % DIV_0_TO_1(pstIspCtx->stLinkage.u8AERunInterval)) || (pstIspCtx->u8ProFrmCnt > 0))   //h00191408
    {
        if (!pstIspCtx->stLinkage.bDefectPixel)
        {
            ISP_SyncCfgSet(IspDev);
        }
    }

    ISP_UpdateExtRegs(IspDev);
    /* save capture info*/
    ISP_SaveSnapInfo(IspDev);
    ISP_UpdateDngImageDynamicInfo(IspDev);

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

