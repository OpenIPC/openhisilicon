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

typedef struct hiISP_BUS_CALLBACK_S
{
    HI_S32  (*pfnISPWriteSensorData) (HI_VOID *pSensorData);
    HI_S32  (*pfnISPReadSensorData) (HI_VOID *pSensorData);
} ISP_BUS_CALLBACK_S;

typedef struct hiISP_PIRIS_CALLBACK_S
{
    HI_S32  (*pfn_piris_gpio_update)(HI_S32 *s32Pos);
} ISP_PIRIS_CALLBACK_S;

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
    HI_S32  (*pfnISPRegisterBusCallBack) (HI_S32 IspDev, ISP_BUS_CALLBACK_S *pstBusCb);
    HI_S32  (*pfnISPRegisterPirisCallBack) (HI_S32 IspDev, ISP_PIRIS_CALLBACK_S *pstPirisCb);
    HI_S32  (*pfnISP_GetDCFInfo)(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF);
    HI_S32  (*pfnISP_GetIspUpdateInfo)(ISP_DEV IspDev,ISP_UPDATE_INFO_S *pstIspUpdateInfo);
    HI_S32  (*pfnISPRegisterSyncTask)(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstNewNode);
    HI_S32  (*pfnISPUnRegisterSyncTask)(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstDelNode);    
} ISP_EXPORT_FUNC_S;

#define CKMODULE_ISP()\
        (!FUNC_ENTRY_NULL(HI_ID_ISP))
        
#define CKFN_ISP_RegisterBusCallBack()\
        (CKMODULE_ISP()&&(NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterBusCallBack))
#define CALL_ISP_RegisterBusCallBack(IspDev, pstBusCb)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterBusCallBack(IspDev, pstBusCb)

#define CKFN_ISP_RegisterPirisCallBack()\
        (CKMODULE_ISP()&&(NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterPirisCallBack))
#define CALL_ISP_RegisterPirisCallBack(IspDev,pstPirisCb)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterPirisCallBack(IspDev,pstPirisCb)

#define CKFN_ISP_GetDCFInfo()\
        (CKMODULE_ISP()&&(NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetDCFInfo))
#define CALL_ISP_GetDCFInfo(IspDev,pstIspDCF)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetDCFInfo(IspDev,pstIspDCF)

#define CKFN_ISP_GetIspUpdateInfo()\
        (CKMODULE_ISP()&&(NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetIspUpdateInfo))
#define CALL_ISP_GetIspUpdateInfo(IspDev,pstIspUpdateInfo)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISP_GetIspUpdateInfo(IspDev,pstIspUpdateInfo)

#define CKFN_ISP_RegisterSyncTask()\
        (CKMODULE_ISP()&&(NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterSyncTask))
#define CALL_ISP_RegisterSyncTask(IspDev, pstNode)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPRegisterSyncTask(IspDev, pstNode)

#define CKFN_ISP_UnRegisterSyncTask()\
        (CKMODULE_ISP()&&(NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPUnRegisterSyncTask))
#define CALL_ISP_UnRegisterSyncTask(IspDev, pstNode)\
        FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)->pfnISPUnRegisterSyncTask(IspDev, pstNode)

typedef struct hiSPI_MODULE_PARAMS_S
{
    HI_U32 u32BusNum;
    HI_U32 u32csn;
    HI_CHAR cSensor[64];
}SPI_MODULE_PARAMS_S;
   
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
