/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/17
  Description   : 
  History       :
  1.Date        : 2013/07/17
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_EXT_H__
#define __ISP_EXT_H__

#include "hi_type.h"
#include "hi_comm_isp.h"
#include "mod_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum hiISP_BUS_TYPE_E
{
    ISP_BUS_TYPE_I2C = 0,
    ISP_BUS_TYPE_SSP,

    ISP_BUS_TYPE_BUTT,
} ISP_BUS_TYPE_E;

typedef struct hiISP_PRO_CTRL_S
{
    HI_U8  u8Vcnum;
    HI_U32 u32ExpTime;
    HI_U32 u32SysGain;
    ISP_PRO_PARAM_S stProParam;
}ISP_PRO_CTRL_S;

typedef struct hiISP_BUS_CALLBACK_S
{
    HI_S32  (*pfnISPWriteI2CData) (HI_U8 u8I2cDev, HI_U8 u8DevAddr, HI_U32 u32RegAddr,
        HI_U32 u32RegAddrByteNum, HI_U32 u32Data, HI_U32 u32DataByteNum);
    HI_S32  (*pfnISPWriteSSPData) (HI_U32 u32SpiDev, HI_U32 u32SpiCsn,
		HI_U32 u32DevAddr, HI_U32 u32DevAddrByteNum,
        HI_U32 u32RegAddr, HI_U32 u32RegAddrByteNum, HI_U32 u32Data, HI_U32 u32DataByteNum);
} ISP_BUS_CALLBACK_S;

typedef struct hiISP_PIRIS_CALLBACK_S
{
    HI_S32  (*pfn_piris_gpio_update)(HI_S32 IspDev, HI_S32 *s32Pos);
} ISP_PIRIS_CALLBACK_S;

typedef struct hiISP_SNAP_CALLBACK_S
{
    HI_S32  (*pfnSnapIspInfoUpdate)(HI_S32 IspDev, ISP_CONFIG_INFO_S *pstSnapIspInfo);
} ISP_SNAP_CALLBACK_S;

//-- AF --------------------------------------------------------------------------------------
#define DRV_AF_ZONE_ROW                 (15)
#define DRV_AF_ZONE_COLUMN              (17)
typedef struct hiISP_DRV_FOCUS_ZONE_S
{
    HI_U16  u16v1;
    HI_U16  u16h1;
    HI_U16  u16v2;
    HI_U16  u16h2;
    HI_U16  u16y;
    HI_U16  u16HlCnt;
} ISP_DRV_FOCUS_ZONE_S;
typedef struct hiISP_DRV_FOCUS_STATISTICS_S
{
    ISP_DRV_FOCUS_ZONE_S stZoneMetrics[DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN]; /*RO, The zoned measure of contrast*/
} ISP_DRV_FOCUS_STATISTICS_S;

// isp sync task    
typedef enum hiISP_SYNC_TSK_METHOD_E
{
    ISP_SYNC_TSK_METHOD_HW_IRQ = 0,
    ISP_SYNC_TSK_METHOD_TSKLET,         /* not support */
    ISP_SYNC_TSK_METHOD_WORKQUE,
    
    ISP_SYNC_TSK_METHOD_BUTT
    
} ISP_SYNC_TSK_METHOD_E;

typedef struct hiISP_SYNC_TASK_NODE_S
{
    ISP_SYNC_TSK_METHOD_E enMethod;
    HI_S32 ( *pfnIspSyncTskCallBack ) (HI_U64 u64Data);
    HI_U64 u64Data;
    const char *pszId;
    struct osal_list_head list;

    ISP_DRV_FOCUS_STATISTICS_S     *pstFocusStat;
} ISP_SYNC_TASK_NODE_S;   

extern HI_S32 hi_isp_sync_task_register(ISP_DEV dev, ISP_SYNC_TASK_NODE_S * pstNewNode);
extern HI_S32 hi_isp_sync_task_unregister(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstDelNode); 


typedef struct hiISP_EXPORT_FUNC_S
{
    HI_S32  (*pfnISPRegisterBusCallBack) (HI_S32 IspDev, ISP_BUS_TYPE_E enType, ISP_BUS_CALLBACK_S *pstBusCb);
    HI_S32  (*pfnISPRegisterPirisCallBack) (HI_S32 IspDev, ISP_PIRIS_CALLBACK_S *pstPirisCb);
    HI_S32  (*pfnISPRegisterSnapCallBack) (HI_S32 IspDev, ISP_SNAP_CALLBACK_S *pstSnapCb);
    HI_S32  (*pfnISP_GetDCFInfo)(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF);
    HI_S32  (*pfnISP_SetDCFInfo)(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF);
    HI_S32  (*pfnISP_GetIspUpdateInfo)(ISP_DEV IspDev,ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo);
    HI_S32  (*pfnISP_SetIspUpdateInfo)(ISP_DEV IspDev,ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo);
    HI_S32  (*pfnISP_GetFrameInfo)(ISP_DEV IspDev,FRAME_SUPPLEMENT_INFO_S *pstIspFrame);
    HI_S32  (*pfnISP_SetFrameInfo)(ISP_DEV IspDev,FRAME_SUPPLEMENT_INFO_S *pstIspFrame);    
    HI_S32  (*pfnISP_SaveCaptureConfig)(ISP_DEV IspDev, ISP_CONFIG_INFO_S *pstCapInfo);
    HI_S32  (*pfnISP_LoadCaptureConfig)(ISP_DEV IspDev, ISP_CONFIG_INFO_S *pstCapInfo);
    HI_S32  (*pfnISP_SetProEnable)(ISP_DEV IspDev);
    HI_BOOL (*pfnISP_GetProCtrl)(ISP_DEV IspDev, ISP_PRO_CTRL_S *pstProCtrl);
    HI_S32  (*pfnISP_SetProCtrl)(ISP_DEV IspDev, HI_BOOL bProEnd);
	HI_S32  (*pfnISP_PrimStatisticsRead)(ISP_DEV IspDev, HI_VOID *pstPrimStat);
    HI_S32  (*pfnISP_GetDngImageDynamicInfo)(ISP_DEV IspDev, DNG_IMAGE_DYNAMIC_INFO_S *pstDngImageDynamicInfo);
    HI_S32  (*pfnISPRegisterSyncTask)(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstNewNode);
    HI_S32  (*pfnISPUnRegisterSyncTask)(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstDelNode);    
} ISP_EXPORT_FUNC_S;

#define CKFN_ISP_RegisterBusCallBack()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterBusCallBack)
#define CALL_ISP_RegisterBusCallBack(IspDev,enType,pstBusCb)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterBusCallBack(IspDev,enType,pstBusCb)


#define CKFN_ISP_RegisterPirisCallBack()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterPirisCallBack)
#define CALL_ISP_RegisterPirisCallBack(IspDev,pstPirisCb)\
       FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterPirisCallBack(IspDev,pstPirisCb)

#define CKFN_ISP_RegisterSnapCallBack()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterSnapCallBack)
#define CALL_ISP_RegisterSnapCallBack(IspDev,pstSnapCb)\
       FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterSnapCallBack(IspDev,pstSnapCb)

#define CKFN_ISP_GetDCFInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetDCFInfo)
#define CALL_ISP_GetDCFInfo(IspDev,pstIspDCF)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetDCFInfo(IspDev,pstIspDCF)

#define CKFN_ISP_GetIspUpdateInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetIspUpdateInfo)
#define CALL_ISP_GetIspUpdateInfo(IspDev,pstIspUpdateInfo)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetIspUpdateInfo(IspDev,pstIspUpdateInfo)

#define CKFN_ISP_SetIspUpdateInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetIspUpdateInfo)
#define CALL_ISP_SetIspUpdateInfo(IspDev,pstIspUpdateInfo)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetIspUpdateInfo(IspDev,pstIspUpdateInfo)

#define CKFN_ISP_GetFrameInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetFrameInfo)
#define CALL_ISP_GetFrameInfo(IspDev,pstIspFrame)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetFrameInfo(IspDev,pstIspFrame)

#define CKFN_ISP_SetFrameInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetFrameInfo)
#define CALL_ISP_SetFrameInfo(IspDev,pstIspFrame)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetFrameInfo(IspDev,pstIspFrame)

#define CKFN_ISP_SetDCFInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetDCFInfo)
#define CALL_ISP_SetDCFInfo(IspDev,pstIspDCF)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetDCFInfo(IspDev,pstIspDCF)

#define CKFN_ISP_SaveCaptureConfig()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SaveCaptureConfig)
#define CALL_ISP_SaveCaptureConfig(IspDev,pstCapInfo)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SaveCaptureConfig(IspDev,pstCapInfo)


#define CKFN_ISP_LoadCaptureConfig()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_LoadCaptureConfig)
#define CALL_ISP_LoadCaptureConfig(IspDev,pstCapInfo)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_LoadCaptureConfig(IspDev,pstCapInfo)


#define CKFN_ISP_SetProEnable()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetProEnable)
#define CALL_ISP_SetProEnable(IspDev)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetProEnable(IspDev)


#define CKFN_ISP_GetProCtrl()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetProCtrl)
#define CALL_ISP_GetProCtrl(IspDev,pstProCtrl)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetProCtrl(IspDev,pstProCtrl)

#define CKFN_ISP_SetProCtrl()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetProCtrl)
#define CALL_ISP_SetProCtrl(IspDev,bProEnd)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_SetProCtrl(IspDev,bProEnd)
        

#define CKFN_ISP_PrimStatisticsRead()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_PrimStatisticsRead)
#define CALL_ISP_PrimStatisticsRead(IspDev, pstPrimStat)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_PrimStatisticsRead(IspDev, pstPrimStat)

#define CKFN_ISP_GetDngImageDynamicInfo()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetDngImageDynamicInfo)
#define CALL_ISP_GetDngImageDynamicInfo(IspDev, pstDngImageDynamicInfo)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetDngImageDynamicInfo(IspDev, pstDngImageDynamicInfo)


#define CKFN_ISP_RegisterSyncTask()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterSyncTask)
#define CALL_ISP_RegisterSyncTask(IspDev, pstNode)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterSyncTask(IspDev, pstNode)

#define CKFN_ISP_UnRegisterSyncTask()\
        (NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPUnRegisterSyncTask)
#define CALL_ISP_UnRegisterSyncTask(IspDev, pstNode)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPUnRegisterSyncTask(IspDev, pstNode)


   
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
