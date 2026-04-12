/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_fpn.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/24
  Description   : 
  History       :
  1.Date        : 2013/07/24
    Author      : z00186359
    Modification: Created file

******************************************************************************/

#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"


#include "hi_comm_vi.h"
#include "mpi_vi.h"
#include "mkp_vi.h"
#include "isp_proc.h"
#include <sys/ioctl.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 g_s32ViFd[2] = {-1,-1};
HI_S32 g_s32Iso = 0;
HI_S32 u32IspIntCnt = 3;

#define FPN_CHN_NUM     4
        
#define CHECK_VI_DEV_OPEN(ViDev)\
do{\
    if (g_s32ViFd[ViDev] <= 0)\
    {\
        g_s32ViFd[ViDev] = HI_MPI_VI_GetDevFd(ViDev);\
        if (g_s32ViFd[ViDev] < 0)\
        {\
            printf("open chn %d err, ret:%d \n", 0, g_s32ViFd[ViDev]);\
            return HI_ERR_VI_SYS_NOTREADY;\
        }\
    }\
}while(0)


HI_S32  VI_SetCalibrateAttr(VI_DEV ViDev, VI_FPN_ATTR_S *pstFPNAttr)
{
    CHECK_VI_DEV_OPEN(ViDev);

    return ioctl(g_s32ViFd[ViDev], VIU_FPN_ATTR_SET_CTRL, pstFPNAttr);
}

HI_S32  VI_GetCorrectionAttr(VI_DEV ViDev, VI_FPN_ATTR_S *pstFPNAttr)
{
    CHECK_VI_DEV_OPEN(ViDev);

    return ioctl(g_s32ViFd[ViDev], VIU_FPN_ATTR_GET_CTRL, pstFPNAttr);
}

HI_S32 ISP_CheckFpnConfig(ISP_FPN_TYPE_E enFpnType, 
                          const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo)
{
    COMPRESS_MODE_E    enCompressMode;
    PIXEL_FORMAT_E  enPixelFormat;
    
    if (0 == pstFPNFrmInfo->u32Iso)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid fpn ISO(%d)!\n", pstFPNFrmInfo->u32Iso);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstFPNFrmInfo->u32Offset > 0xFFF)
    {
       ISP_TRACE(HI_DBG_ERR, "Invalid fpn offset, can't be larger than 0xFFF!\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
        
    enCompressMode = pstFPNFrmInfo->stFpnFrame.stVFrame.enCompressMode;
    enPixelFormat  = pstFPNFrmInfo->stFpnFrame.stVFrame.enPixelFormat;

    if ((enFpnType != ISP_FPN_TYPE_FRAME) && (enFpnType != ISP_FPN_TYPE_LINE))
    {
       ISP_TRACE(HI_DBG_ERR, "Invalid fpn Mode, should be in {line, frame} mode!\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((enCompressMode != COMPRESS_MODE_NONE) && (enCompressMode != COMPRESS_MODE_LINE))
    {
        ISP_TRACE(HI_DBG_ERR, "fpn compress can only be {none(%d), line(%d)} mode!\n", COMPRESS_MODE_NONE, COMPRESS_MODE_LINE);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (enFpnType == ISP_FPN_TYPE_LINE)
    {
        /* 16bit only in line mode */
        if (PIXEL_FORMAT_RGB_BAYER != enPixelFormat)
        {
            ISP_TRACE(HI_DBG_ERR, "only support 16bit fpn in line mode!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (COMPRESS_MODE_NONE != enCompressMode)
        {
            ISP_TRACE(HI_DBG_ERR, "fpn compress is not supported in line mode!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    else
    {
        if ((PIXEL_FORMAT_RGB_BAYER != enPixelFormat)
            && (PIXEL_FORMAT_RGB_BAYER_8BPP != enPixelFormat)
            && (PIXEL_FORMAT_RGB_BAYER_10BPP != enPixelFormat)
            && (PIXEL_FORMAT_RGB_BAYER_12BPP != enPixelFormat))
        {
            ISP_TRACE(HI_DBG_ERR, "enPixelFormat invalid(%d), only support Raw8,Raw10,Raw12,Raw16.\n", enPixelFormat);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        
        if ((COMPRESS_MODE_NONE != enCompressMode)
            && (PIXEL_FORMAT_RGB_BAYER == enPixelFormat))
        {
            ISP_TRACE(HI_DBG_ERR, "fpn compress is not supported in 16bit frame mode!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CheckCalibrateAttr(VI_DEV ViDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_FPN_TYPE_E enFpnType;
    const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo;

    enFpnType = pstCalibrate->enFpnType;
    pstFPNFrmInfo = &pstCalibrate->stFpnCaliFrame;
    s32Ret = ISP_CheckFpnConfig(enFpnType, pstFPNFrmInfo);
    return s32Ret;
}

HI_S32 ISP_CheckCorrectionAttr(VI_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Strength;
    ISP_FPN_TYPE_E enFpnType;
    
    if (pstCorrection->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR,"Invalid fpn op type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstCorrection->bEnable != HI_TRUE)
        &&(pstCorrection->bEnable != HI_FALSE))
    {
       ISP_TRACE(HI_DBG_ERR,"Invalid fpn enable, must be {true, false}!\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }    

    u32Strength   = pstCorrection->stManual.u32Strength;
    enFpnType     = pstCorrection->enFpnType;
    
    if (u32Strength > 1023)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid fpn strength, over 1023!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_CheckFpnConfig(enFpnType, &pstCorrection->stFpnFrmInfo);
    
    /* to do param check */
    return s32Ret;
}

HI_S32  ISP_SetCalibrateAttr(VI_DEV ViDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate)
{
    VI_FPN_ATTR_S stFpnAttr;
    HI_S32 s32Ret = HI_SUCCESS;    
    //ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_DEV IspDev = (ISP_DEV)ViDev;

    //ISP_GET_CTX(0, pstIspCtx);   
    //pstCalibrate->stFpnCaliFrame.u32Iso = pstIspCtx->stLinkage.u32SensorIso;
    pstCalibrate->stFpnCaliFrame.u32Iso = hi_ext_system_sensor_iso_read(IspDev);
    
    s32Ret = ISP_CheckCalibrateAttr(ViDev, pstCalibrate);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    stFpnAttr.enFpnWorkMode = FPN_MODE_CALIBRATE;
    stFpnAttr.enFpnType     = pstCalibrate->enFpnType;    
   
    stFpnAttr.stCalibrate.u32Threshold = pstCalibrate->u32Threshold;
    stFpnAttr.stCalibrate.u32FrameNum  = pstCalibrate->u32FrameNum;

    memcpy(&stFpnAttr.stCalibrate.stFpnCaliFrame,
           &pstCalibrate->stFpnCaliFrame, 
           sizeof(ISP_FPN_FRAME_INFO_S));

    /* get calibrate result */
    s32Ret = VI_SetCalibrateAttr(ViDev, &stFpnAttr);

    /* get value from temp variable */
    memcpy(&pstCalibrate->stFpnCaliFrame, 
           &stFpnAttr.stCalibrate.stFpnCaliFrame,
           sizeof(ISP_FPN_FRAME_INFO_S));
    
    return s32Ret;
}

HI_U32 ISP_FPN_CLIP(HI_U32 u32Gain,HI_U32 u32Min, HI_U32 u32Max)
{
    if(u32Gain < u32Min)
    {
        return u32Min;
    }
    else if(u32Gain >u32Max)
    {
        return u32Max;
    }
    else
    {
        return u32Gain;
    }
}

inline HI_U32 ISP_Fpn_GetStrength(HI_U32 u32Iso, HI_S32 s32K1, HI_S32 s32K2, HI_S32 s32K3, HI_U32 u32CalibrateIso)
{
    HI_U32 u32Strength = 256;
    HI_U32 u32In;

    u32In = 256 * u32Iso / DIV_0_TO_1(u32CalibrateIso);    
    //s32In = 256 * s32Iso /6400;
    //u32Strength = (s32K1 + s32K2*u32In + s32K3*u32In*u32In) >> 8;
    u32Strength = u32In;    
    return u32Strength;
}

HI_S32 ISP_SetCorrectionAttr(VI_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_FPN_ATTR_S stFpnAttr;
    //ISP_CTX_S *pstIspCtx = HI_NULL;
    const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo = HI_NULL;
    HI_U32 u32Iso;
    VI_FPN_TYPE_E  enFpnType;
    HI_U32 u32Gain =0;
    ISP_DEV IspDev = (ISP_DEV)ViDev;
   
    s32Ret = ISP_CheckCorrectionAttr(ViDev, pstCorrection);    
    if (HI_SUCCESS != s32Ret)
    {        
        return s32Ret;
    }
    
    isp_ext_system_manual_fpn_opmode_write(IspDev, pstCorrection->enOpType);

    /* save manual strength */
    isp_ext_system_manual_fpn_strength_write(IspDev, pstCorrection->stManual.u32Strength);

    enFpnType     = pstCorrection->enFpnType;
    pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;

    isp_ext_system_manual_FPN_ISO_write(IspDev, pstCorrection->stFpnFrmInfo.u32Iso);

    if (OP_TYPE_AUTO == pstCorrection->enOpType)
    {
        u32Iso = hi_ext_system_sensor_iso_read(IspDev);
        isp_ext_system_manual_fpn_update_write(IspDev, 0x1);

        /* k2=256 in auto mode */
        u32Gain = ISP_Fpn_GetStrength(u32Iso, 0, 256, 0, pstCorrection->stFpnFrmInfo.u32Iso);
    }
    else
    {
        u32Gain = pstCorrection->stManual.u32Strength;
    }
    
    u32Gain = ISP_FPN_CLIP(u32Gain, 0, 1023);
    stFpnAttr.enFpnWorkMode            = FPN_MODE_CORRECTION;
    stFpnAttr.enFpnType                = enFpnType;   
    stFpnAttr.stCorrection.bEnable     = pstCorrection->bEnable;
    stFpnAttr.stCorrection.u32Gain     = u32Gain;
    stFpnAttr.stCorrection.enFpnOpType = pstCorrection->enOpType;  
   
    memcpy(&stFpnAttr.stCorrection.stFpnFrmInfo, pstFPNFrmInfo, 
           sizeof(ISP_FPN_FRAME_INFO_S));    
    
    s32Ret = VI_SetCalibrateAttr(ViDev, &stFpnAttr);
    if(HI_SUCCESS == s32Ret)
    {
        /* set cfg flag true when success */
        isp_ext_system_manual_fpn_CorrCfg_write(IspDev, HI_TRUE);
    }
    else
    {
        /* set cfg flag flag when success */
        isp_ext_system_manual_fpn_CorrCfg_write(IspDev, HI_FALSE);
    }
    
    return s32Ret;
}

HI_S32 ISP_GetCorrectionAttr(VI_DEV ViDev, ISP_FPN_ATTR_S *pstCorrection)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_FPN_ATTR_S stTempViFpnAttr = {0};
    ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo;
    ISP_DEV IspDev = (ISP_DEV)ViDev;

    if(0 == isp_ext_system_manual_fpn_CorrCfg_read(IspDev))
    {
       return HI_ERR_ISP_ATTR_NOT_CFG;
    }
    
    s32Ret =  VI_GetCorrectionAttr(ViDev, &stTempViFpnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstCorrection->bEnable   = stTempViFpnAttr.stCorrection.bEnable;
    pstCorrection->enOpType  = stTempViFpnAttr.stCorrection.enFpnOpType;
    pstCorrection->enFpnType = stTempViFpnAttr.enFpnType;
    
    pstCorrection->stManual.u32Strength = isp_ext_system_manual_fpn_strength_read(IspDev);
    /* auto.strength is equal to fpn gain, high 16 bits: gain */
    pstCorrection->stAuto.u32Strength = (hi_vi_fpn_read_gain_offset(IspDev, 0) >> 16);

    /* fpn frame info copy */
    pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;    
    memcpy(pstFPNFrmInfo, &stTempViFpnAttr.stCorrection.stFpnFrmInfo, sizeof(ISP_FPN_FRAME_INFO_S));

    return HI_SUCCESS;
}

static HI_VOID FPNExtRegsDefault(ISP_DEV IspDev)
{
    hi_ext_system_sensor_iso_write(IspDev, HI_EXT_SYSTEM_SENSOR_ISO_DEFAULT);
    return;
}

static HI_VOID FPNRegsDefault(ISP_DEV IspDev) 
{
    g_s32Iso = 0;  
    isp_ext_system_manual_fpn_CorrCfg_write(IspDev, HI_FALSE);
    isp_ext_system_manual_fpn_strength_write(IspDev, 0);
    isp_ext_system_manual_fpn_opmode_write(IspDev, 0xf);
    isp_ext_system_manual_fpn_update_write(IspDev, 0);
    return;
}

static HI_VOID FPNExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID FPNRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_S32 FPNReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 FPNUpdateExtRegs(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);

    hi_ext_system_sensor_iso_write(IspDev, pstIspCtx->stLinkage.u32SensorIso);
    
    return 0;
}

HI_VOID IspSetStrength(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;    
    HI_U32 u32Iso, u32Gain, Offfset, i;
    HI_U32 u32CalibrateIso;
    HI_U8  s8FpnOpMode;    
    HI_U32 u32GainOffset[FPN_CHN_NUM];
    
    ISP_GET_CTX(IspDev, pstIspCtx);   
  
    /* calculate every 120 interrupts */
    #if 0
    if (0 == pstIspCtx->u32FrameCnt % 120)
    {
        printf("\nisp auto set fpn's strength and offset!\n");
    }
    #endif
    
    if ((0 != pstIspCtx->u32FrameCnt % u32IspIntCnt) && (HI_TRUE != pstIspCtx->stLinkage.bCapState))
    {
        return ;
    }
    
    s8FpnOpMode = isp_ext_system_manual_fpn_opmode_read(IspDev);    
    if(OP_TYPE_MANUAL == s8FpnOpMode)        
    {
        return ;
    }   
    
    u32Iso = hi_ext_system_sensor_iso_read(IspDev);
    if ((hi_vi_fpn_en_read(IspDev) && g_s32Iso != u32Iso)
        || isp_ext_system_manual_fpn_update_read(IspDev))
    {
        if(isp_ext_system_manual_fpn_update_read(IspDev))
        {
            isp_ext_system_manual_fpn_update_write(IspDev, 0);
        }

        /* Auto calculate gain value according to iso */
        u32CalibrateIso = isp_ext_system_manual_FPN_ISO_read(IspDev);        
        u32Gain = ISP_Fpn_GetStrength(u32Iso, 0 , 255, 0, u32CalibrateIso);        
        u32Gain = ISP_FPN_CLIP(u32Gain, 0,1023);
        
        for (i = 0; i < FPN_CHN_NUM; i++)
        {
            u32GainOffset[i] = hi_vi_fpn_read_gain_offset(IspDev, i);
            Offfset          = u32GainOffset[i] & 0xFFF;
            u32GainOffset[i] = (u32Gain << 16) + Offfset; 
            hi_vi_fpn_write_gain_offset(IspDev, i, u32GainOffset[i]);             
        }  

        //printf("\ns32Isos =%d,32Strength= %d\n",s32Iso,u32Gain);           
        g_s32Iso = u32Iso;                  
    } 
    else
    {
        return ;

    }   
}

HI_S32 ISP_FPNInit(ISP_DEV IspDev)
{
    FPNRegsDefault(IspDev);
    FPNExtRegsDefault(IspDev);
    FPNRegsInitialize(IspDev);
    FPNExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_FPNRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    FPNReadExtregs(IspDev);
    FPNUpdateExtRegs(IspDev);
    IspSetStrength(IspDev);    
    
    return HI_SUCCESS;
}

HI_S32 FpnProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    HI_U32 u32GainOffset;
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----FPN INFO------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s" "%7s"     "%9s"       "%8s",
        "En", "OpType", "Strength", "Offset\n");

    u32GainOffset = hi_vi_fpn_read_gain_offset(IspDev, 0);
    
    if(hi_vi_fpn_en_read(IspDev))
    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%4d"  "%9d"  "%8d\n",
        hi_vi_fpn_en_read(IspDev),
        isp_ext_system_manual_fpn_opmode_read(IspDev),
        u32GainOffset >> 16,
        u32GainOffset & 0xffff);
    }
    else
    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%4s"  "%9s"  "%8s\n",
        hi_vi_fpn_en_read(IspDev),
        "--",
        "--",
        "--");
    }

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}

HI_S32 ISP_FPNCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_PROC_WRITE:
            FpnProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
            
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_FPNExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterFPN(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_FPN;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_FPNInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_FPNRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_FPNCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_FPNExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

