/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_EXT_H__
#define __ISP_EXT_H__

#include "comm_isp.h"
#include "mod_ext.h"
#include "mkp_isp.h"
#include "osal.h"
#include "isp_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum ISP_BUS_TYPE_E {
	ISP_BUS_TYPE_I2C = 0,
	ISP_BUS_TYPE_SSP,

	ISP_BUS_TYPE_BUTT,
} ISP_BUS_TYPE_E;

typedef struct ISP_PRO_CTRL_S {
	GK_U8 u8Vcnum;
} ISP_PRO_CTRL_S;

typedef struct ISP_BUS_CALLBACK_S {
	GK_S32(*pfnISPWriteI2CData)
	(GK_U8 u8I2cDev, GK_U8 u8DevAddr, GK_U32 u32RegAddr,
	 GK_U32 u32RegAddrByteNum, GK_U32 u32Data, GK_U32 u32DataByteNum);
	GK_S32(*pfnISPWriteSSPData)
	(GK_U32 u32SpiDev, GK_U32 u32SpiCsn, GK_U32 u32DevAddr,
	 GK_U32 u32DevAddrByteNum, GK_U32 u32RegAddr, GK_U32 u32RegAddrByteNum,
	 GK_U32 u32Data, GK_U32 u32DataByteNum);
} ISP_BUS_CALLBACK_S;

typedef struct ISP_PIRIS_CALLBACK_S {
	GK_S32 (*pfn_piris_gpio_update)(GK_S32 ViPipe, const GK_S32 *s32Pos);
} ISP_PIRIS_CALLBACK_S;

typedef struct ISP_VIBUS_CALLBACK_S {
	GK_S32 (*pfnIspBeStaRead)(GK_S32 ViPipe, ISP_BE_STAT_S *pstBeStat);
	GK_S32 (*pfnIspBeCfgRead)(GK_S32 ViPipe, ISP_REG_CFG_S *pstRegCfgInfo);
} ISP_VIBUS_CALLBACK_S;

// -- AF --------------------------------------------------------------------------------------
#define DRV_AF_ZONE_ROW (15)
#define DRV_AF_ZONE_COLUMN (17)
#define DRV_WDR_CHN_MAX (4)

typedef struct ISP_DRV_FOCUS_ZONE_S {
	GK_U16 u16v1;
	GK_U16 u16h1;
	GK_U16 u16v2;
	GK_U16 u16h2;
	GK_U16 u16y;
	GK_U16 u16HlCnt;
} ISP_DRV_FOCUS_ZONE_S;

typedef struct ISP_DRV_FE_FOCUS_STATISTICS_S {
	ISP_DRV_FOCUS_ZONE_S stZoneMetrics
		[DRV_WDR_CHN_MAX][DRV_AF_ZONE_ROW]
		[DRV_AF_ZONE_COLUMN]; /* RO, The zoned measure of contrast */
} ISP_DRV_FE_FOCUS_STATISTICS_S;

typedef struct ISP_DRV_BE_FOCUS_STATISTICS_S {
	ISP_DRV_FOCUS_ZONE_S stZoneMetrics
		[DRV_AF_ZONE_ROW]
		[DRV_AF_ZONE_COLUMN]; /* RO, The zoned measure of contrast */
} ISP_DRV_BE_FOCUS_STATISTICS_S;

typedef struct ISP_DRV_AF_STATISTICS_S {
	ISP_DRV_FE_FOCUS_STATISTICS_S stFEAFStat;
	ISP_DRV_BE_FOCUS_STATISTICS_S stBEAFStat;
} ISP_DRV_AF_STATISTICS_S;

// ----------------------------------------------------------------------------------------
// isp sync task
typedef enum ISP_SYNC_TSK_METHOD_E {
	ISP_SYNC_TSK_METHOD_HW_IRQ = 0,
	ISP_SYNC_TSK_METHOD_WORKQUE,

	ISP_SYNC_TSK_METHOD_BUTT

} ISP_SYNC_TSK_METHOD_E;

typedef struct ISP_SYNC_TASK_NODE_S {
	ISP_SYNC_TSK_METHOD_E enMethod;
	GK_S32 (*pfnIspSyncTskCallBack)(GK_U64 u64Data);
	GK_U64 u64Data;
	const char *pszId;
	struct osal_list_head list;
	ISP_DRV_AF_STATISTICS_S *pstFocusStat;
} ISP_SYNC_TASK_NODE_S;

typedef struct ISP_EXPORT_FUNC_S {
	GK_S32(*pfnISPRegisterBusCallBack)
	(GK_S32 ViPipe, ISP_BUS_TYPE_E enType, ISP_BUS_CALLBACK_S *pstBusCb);
	GK_S32(*pfnISPRegisterPirisCallBack)
	(GK_S32 ViPipe, ISP_PIRIS_CALLBACK_S *pstPirisCb);
	GK_S32(*pfnISPRegisterViBusCallBack)
	(GK_S32 ViPipe, ISP_VIBUS_CALLBACK_S *pstViBusCb);
	GK_S32 (*pfnISP_GetDCFInfo)(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF);
	GK_S32 (*pfnISP_SetDCFInfo)(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF);
	GK_S32(*pfnISP_GetIspUpdateInfo)
	(VI_PIPE ViPipe, ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo);
	GK_S32(*pfnISP_SetIspUpdateInfo)
	(VI_PIPE ViPipe, ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo);
	GK_S32(*pfnISP_GetFrameInfo)
	(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspFrame);
	GK_S32(*pfnISP_SetFrameInfo)
	(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspFrame);
	GK_S32(*pfnISP_GetAttachInfo)
	(VI_PIPE ViPipe, ISP_ATTACH_INFO_S *pstIspAttachInfo);
	GK_S32(*pfnISP_GetColorGamutInfo)
	(VI_PIPE ViPipe, ISP_COLORGAMMUT_INFO_S *pstIspColorGamutInfo);
	GK_S32(*pfnISP_GetDngImageDynamicInfo)
	(VI_PIPE ViPipe, DNG_IMAGE_DYNAMIC_INFO_S *pstDngImageDynamicInfo);
	GK_S32(*pfnISP_SaveSnapConfig)
	(VI_PIPE ViPipe, ISP_CONFIG_INFO_S *pstSnapInfo);
	GK_S32 (*pfnISP_SetProEnable)(VI_PIPE ViPipe);
	GK_S32(*pfnISP_DRV_GetReadyBeBuf)
	(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf);
	GK_S32(*pfnISP_DRV_PutFreeBeBuf)
	(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf);
	GK_S32(*pfnISP_DRV_HoldBusyBeBuf)
	(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf);
	GK_S32(*pfnISP_DRV_GetBeSyncPara)
	(VI_PIPE ViPipe, ISP_BE_SYNC_PARA_S *pstBeSyncPara);
	GK_BOOL(*pfnISP_GetProCtrl)
	(VI_PIPE ViPipe, ISP_PRO_CTRL_S *pstProCtrl);
	GK_S32(*pfnISP_SetSnapAttr)
	(VI_PIPE ViPipe, ISP_SNAP_ATTR_S *pstSnapAttr);
	GK_S32(*pfnISP_SetProNrParam)
	(VI_PIPE ViPipe, const ISP_PRO_BNR_PARAM_S *pstProNrParam);
	GK_S32(*pfnISP_SetProShpParam)
	(VI_PIPE ViPipe, const ISP_PRO_SHARPEN_PARAM_S *pstProShpParam);
	GK_S32(*pfnISP_GetProNrParam)
	(VI_PIPE ViPipe, ISP_PRO_BNR_PARAM_S *pstProNrParam);
	GK_S32(*pfnISP_GetProShpParam)
	(VI_PIPE ViPipe, ISP_PRO_SHARPEN_PARAM_S *pstProShpParam);
	GK_S32 (*pfnISP_DRV_BeEndIntProc)(VI_PIPE ViPipe);
	GK_S32(*pfnISPRegisterSyncTask)
	(VI_PIPE ViPipe, ISP_SYNC_TASK_NODE_S *pstNewNode);
	GK_S32(*pfnISPUnRegisterSyncTask)
	(VI_PIPE ViPipe, ISP_SYNC_TASK_NODE_S *pstDelNode);
	int (*pfnISP_IntBottomHalf)(int irq, void *id);
	int (*pfnISP_ISR)(int irq, void *id);
	GK_S32 (*pfnISP_GetPubAttr)(VI_PIPE ViPipe, ISP_PUB_ATTR_S *pstPubAttr);
	GK_S32(*pfnISP_DRV_GetRgbirFormat)
	(VI_PIPE ViPipe, ISP_IRBAYER_FORMAT_E *pRgbirFormat);
	GK_S32(*pfnISP_DRV_SetOnLineStaticsSttAddr)
	(VI_PIPE ViPipe, GK_U32 u32ViprocIrqStatus);
} ISP_EXPORT_FUNC_S;

#define CKFN_ISP() (FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) != NULL)

#define CKFN_ISP_RegisterBusCallBack()             \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISPRegisterBusCallBack != NULL)
#define CALL_ISP_RegisterBusCallBack(ViPipe, enType, pstBusCb) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)              \
		->pfnISPRegisterBusCallBack(ViPipe, enType, pstBusCb)

#define CKFN_ISP_RegisterPirisCallBack()           \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISPRegisterPirisCallBack != NULL)
#define CALL_ISP_RegisterPirisCallBack(ViPipe, pstPirisCb) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)          \
		->pfnISPRegisterPirisCallBack(ViPipe, pstPirisCb)

#define CKFN_ISP_RegisterSnapCallBack()            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISPRegisterSnapCallBack != NULL)
#define CALL_ISP_RegisterSnapCallBack(ViPipe, pstSnapCb) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)        \
		->pfnISPRegisterSnapCallBack(ViPipe, pstSnapCb)

#define CKFN_ISP_RegisterViBusCallBack()           \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISPRegisterViBusCallBack != NULL)
#define CALL_ISP_RegisterViBusCallBack(ViPipe, pstViBusCb) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)          \
		->pfnISPRegisterViBusCallBack(ViPipe, pstViBusCb)

#define CKFN_ISP_GetDCFInfo() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetDCFInfo != NULL)
#define CALL_ISP_GetDCFInfo(ViPipe, pstIspDCF)    \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		->pfnISP_GetDCFInfo(ViPipe, pstIspDCF)

#define CKFN_ISP_GetIspUpdateInfo()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetIspUpdateInfo != \
	 NULL)
#define CALL_ISP_GetIspUpdateInfo(ViPipe, pstIspUpdateInfo) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)           \
		->pfnISP_GetIspUpdateInfo(ViPipe, pstIspUpdateInfo)

#define CKFN_ISP_SetIspUpdateInfo()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetIspUpdateInfo != \
	 NULL)
#define CALL_ISP_SetIspUpdateInfo(ViPipe, pstIspUpdateInfo) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)           \
		->pfnISP_SetIspUpdateInfo(ViPipe, pstIspUpdateInfo)

#define CKFN_ISP_GetFrameInfo() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetFrameInfo != NULL)
#define CALL_ISP_GetFrameInfo(ViPipe, pstIspFrame) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)  \
		->pfnISP_GetFrameInfo(ViPipe, pstIspFrame)

#define CKFN_ISP_SetFrameInfo() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetFrameInfo != NULL)
#define CALL_ISP_SetFrameInfo(ViPipe, pstIspFrame) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)  \
		->pfnISP_SetFrameInfo(ViPipe, pstIspFrame)

#define CKFN_ISP_GetAttachInfo()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetAttachInfo != \
	 NULL)
#define CALL_ISP_GetAttachInfo(ViPipe, pstIspAttachInfo) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)        \
		->pfnISP_GetAttachInfo(ViPipe, pstIspAttachInfo)

#define CKFN_ISP_GetColorGamutInfo()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetColorGamutInfo != \
	 NULL)
#define CALL_ISP_GetColorGamutInfo(ViPipe, pstIspColorGamutInfo) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)                \
		->pfnISP_GetColorGamutInfo(ViPipe, pstIspColorGamutInfo)

#define CKFN_ISP_SetDCFInfo() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetDCFInfo != NULL)
#define CALL_ISP_SetDCFInfo(ViPipe, pstIspDCF)    \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		->pfnISP_SetDCFInfo(ViPipe, pstIspDCF)

#define CKFN_ISP_GetDngImageDynamicInfo()          \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISP_GetDngImageDynamicInfo != NULL)
#define CALL_ISP_GetDngImageDynamicInfo(ViPipe, pstDngImageDynamicInfo) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)                       \
		->pfnISP_GetDngImageDynamicInfo(ViPipe,                 \
						pstDngImageDynamicInfo)

#define CKFN_ISP_SaveSnapConfig()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SaveSnapConfig != \
	 NULL)
#define CALL_ISP_SaveSnapConfig(ViPipe, pstSnapInfo) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)    \
		->pfnISP_SaveSnapConfig(ViPipe, pstSnapInfo)

#define CKFN_ISP_SetProEnable() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetProEnable != NULL)
#define CALL_ISP_SetProEnable(ViPipe) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetProEnable(ViPipe)

#define CKFN_ISP_DRV_GetReadyBeBuf()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_DRV_GetReadyBeBuf != \
	 NULL)
#define CALL_ISP_DRV_GetReadyBeBuf(ViPipe, pstBeCfgBuf) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)       \
		->pfnISP_DRV_GetReadyBeBuf(ViPipe, pstBeCfgBuf)

#define CKFN_ISP_DRV_PutFreeBeBuf()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_DRV_PutFreeBeBuf != \
	 NULL)
#define CALL_ISP_DRV_PutFreeBeBuf(ViPipe, pstBeCfgBuf) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)      \
		->pfnISP_DRV_PutFreeBeBuf(ViPipe, pstBeCfgBuf)

#define CKFN_ISP_DRV_HoldBusyBeBuf()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_DRV_HoldBusyBeBuf != \
	 NULL)
#define CALL_ISP_DRV_HoldBusyBeBuf(ViPipe, pstBeCfgBuf) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)       \
		->pfnISP_DRV_HoldBusyBeBuf(ViPipe, pstBeCfgBuf)

#define CKFN_ISP_DRV_GetBeSyncPara()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_DRV_GetBeSyncPara != \
	 NULL)
#define CALL_ISP_DRV_GetBeSyncPara(ViPipe, pstBeSyncPara) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)         \
		->pfnISP_DRV_GetBeSyncPara(ViPipe, pstBeSyncPara)

#define CKFN_ISP_GetProCtrl() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetProCtrl != NULL)
#define CALL_ISP_GetProCtrl(ViPipe, pstProCtrl)   \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		->pfnISP_GetProCtrl(ViPipe, pstProCtrl)

#define CKFN_ISP_SetSnapAttr() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetSnapAttr != NULL)
#define CALL_ISP_SetSnapAttr(ViPipe, pstSnapAttr) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		->pfnISP_SetSnapAttr(ViPipe, pstSnapAttr)

#define CKFN_ISP_SetProNrParam()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetProNrParam != \
	 NULL)
#define CALL_ISP_SetProNrParam(ViPipe, pstProNrParam) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)     \
		->pfnISP_SetProNrParam(ViPipe, pstProNrParam)
#define CKFN_ISP_SetProShpParam()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_SetProShpParam != \
	 NULL)
#define CALL_ISP_SetProShpParam(ViPipe, pstProShpParam) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)       \
		->pfnISP_SetProShpParam(ViPipe, pstProShpParam)
#define CKFN_ISP_GetProNrParam()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetProNrParam != \
	 NULL)
#define CALL_ISP_GetProNrParam(ViPipe, pstProNrParam) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)     \
		->pfnISP_GetProNrParam(ViPipe, pstProNrParam)
#define CKFN_ISP_GetProShpParam()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetProShpParam != \
	 NULL)
#define CALL_ISP_GetProShpParam(ViPipe, pstProShpParam) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)       \
		->pfnISP_GetProShpParam(ViPipe, pstProShpParam)
#define CKFN_ISP_DRV_BeEndIntProc()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_DRV_BeEndIntProc != \
	 NULL)
#define CALL_ISP_DRV_BeEndIntProc(ViPipe)         \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		->pfnISP_DRV_BeEndIntProc(ViPipe)

#define CKFN_ISP_GetPubAttr() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_GetPubAttr != NULL)
#define CALL_ISP_GetPubAttr(ViPipe, pstPubAttr)   \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		->pfnISP_GetPubAttr(ViPipe, pstPubAttr)

#define CKFN_ISP_RegisterSyncTask()                                           \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISPRegisterSyncTask != \
	 NULL)
#define CALL_ISP_RegisterSyncTask(ViPipe, pstNode) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)  \
		->pfnISPRegisterSyncTask(ViPipe, pstNode)

#define CKFN_ISP_UnRegisterSyncTask()                                           \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISPUnRegisterSyncTask != \
	 NULL)
#define CALL_ISP_UnRegisterSyncTask(ViPipe, pstNode) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)    \
		->pfnISPUnRegisterSyncTask(ViPipe, pstNode)

#define CKFN_ISP_IntBottomHalf()                                            \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_IntBottomHalf != \
	 NULL)
#define CALL_ISP_IntBottomHalf(irq, id) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_IntBottomHalf(irq, id)

#define CKFN_ISP_ISR() \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_ISR != NULL)
#define CALL_ISP_ISR(irq, id) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)->pfnISP_ISR(irq, id)

#define CKFN_ISP_GetRgbirFormat()                  \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISP_DRV_GetRgbirFormat != NULL)
#define CALL_ISP_GetRgbirFormat(ViPipe, pRgbirFormat) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)     \
		->pfnISP_DRV_GetRgbirFormat(ViPipe, pRgbirFormat)
#define CKFN_ISP_DRV_SetOnLineStaticsSttAddr()     \
	(FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) \
		 ->pfnISP_DRV_SetOnLineStaticsSttAddr != NULL)
#define CALL_ISP_DRV_SetOnLineStaticsSttAddr(ViPipe, u32ViProcIrqStatus) \
	FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP)                        \
		->pfnISP_DRV_SetOnLineStaticsSttAddr(ViPipe,             \
						     u32ViProcIrqStatus)

#define ISP_SYNC_TASK_ID_MAX_LENGTH 64
extern GK_S32 isp_sync_task_register(VI_PIPE ViPipe,
				     ISP_SYNC_TASK_NODE_S *pstNewNode);
extern GK_S32 isp_sync_task_unregister(VI_PIPE ViPipe,
				       ISP_SYNC_TASK_NODE_S *pstDelNode);

GK_S32 ISP_SetSmartInfo(VI_PIPE ViPipe, ISP_SMART_INFO_S *pstSmartInfo);
GK_S32 ISP_GetSmartInfo(VI_PIPE ViPipe, ISP_SMART_INFO_S *pstSmartInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
