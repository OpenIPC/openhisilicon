/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __SYS_EXT_H__
#define __SYS_EXT_H__

#include "type.h"
#include "mod_ext.h"
#include "comm_video.h"
#include "comm_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SYS_DDR_MAXNUM 16

#define SYS_SEND_DATA_BLOCK_MASK 0x1
#define SYS_SEND_DATA_BLOCK_OFFSET 0
#define SYS_SEND_DATA_NOBLOCK \
	((0 << SYS_SEND_DATA_BLOCK_OFFSET) & SYS_SEND_DATA_BLOCK_MASK)
#define SYS_SEND_DATA_BLOCK \
	((1 << SYS_SEND_DATA_BLOCK_OFFSET) & SYS_SEND_DATA_BLOCK_MASK)

#define SYS_SEND_DATA_DELAY_MASK 0x6
#define SYS_SEND_DATA_DELAY_OFFSET 1
#define SYS_SEND_DATA_FINISH \
	((0 << SYS_SEND_DATA_DELAY_OFFSET) & SYS_SEND_DATA_DELAY_MASK)
#define SYS_SEND_DATA_LOWDELAY \
	((1 << SYS_SEND_DATA_DELAY_OFFSET) & SYS_SEND_DATA_DELAY_MASK)
#define SYS_SEND_DATA_LOWDELAY_FINISH \
	((2 << SYS_SEND_DATA_DELAY_OFFSET) & SYS_SEND_DATA_DELAY_MASK)

typedef struct MPP_SYS_DDR_NAME_S {
	GK_U32 u32Num;
	GK_VOID *apMmzName[SYS_DDR_MAXNUM];
} MPP_SYS_DDR_NAME_S;

typedef enum MPP_DATA_TYPE_E {
	MPP_DATA_VI_FRAME,
	MPP_DATA_VOU_FRAME,
	MPP_DATA_VDEC_FRAME,
	MPP_DATA_VIDEO_FRAME,
	MPP_DATA_VOU_WBC_FRAME,
	MPP_DATA_AUDIO_FRAME,
	MPP_DATA_VPSS_FRAME,
	MPP_DATA_DPU_RECT_FRAME,
	MPP_DATA_AVS_FRAME,
	MPP_DATA_BUTT
} MPP_DATA_TYPE_E;

typedef struct BIND_SENDER_INFO_S {
	MOD_ID_E enModId;
	GK_U32 u32MaxDevCnt;
	GK_U32 u32MaxChnCnt;
	GK_S32(*pGiveBindCallBack)
	(GK_S32 s32DevId, GK_S32 s32ChnId, MPP_BIND_DEST_S *pstBindSend);
} BIND_SENDER_INFO_S;

typedef struct BIND_RECEIVER_INFO_S {
	MOD_ID_E enModId;
	GK_U32 u32MaxDevCnt;
	GK_U32 u32MaxChnCnt;
	GK_S32(*pCallBack)
	(GK_S32 s32DevId, GK_S32 s32ChnId, GK_BOOL bBlock,
	 MPP_DATA_TYPE_E enDataType, GK_VOID *pvData);
	GK_S32(*pResetCallBack)
	(GK_S32 s32DevId, GK_S32 s32ChnId, GK_VOID *pvData);
	GK_BOOL bSupportDelayData;
} BIND_RECEIVER_INFO_S;

typedef struct VB_BASE_INFO_S {
	PIXEL_FORMAT_E enPixelFormat;
	GK_U32 u32Width;
	GK_U32 u32Height;
	GK_U32 u32BufLine;
	GK_U32 u32Align; /* 0:auto */
	DYNAMIC_RANGE_E enDynamicRange;
	COMPRESS_MODE_E enCompressMode;
	VIDEO_FORMAT_E enVideoFormat;
	GK_BOOL b3DNRBuffer;
} VB_BASE_INFO_S;

typedef enum SYS_FUNC_E {
	SYS_VO_BT1120_CLK_SEL,
	SYS_VO_HDDAC_CLK_SEL,
	SYS_VO_HDDATE_CLK_SEL,
	SYS_VO_HDMI_CLK_SEL,
	SYS_VO_GET_HD_CLK_SEL,
	SYS_VO_BT1120_CLK_EN,
	SYS_VO_MIPI_TX_CLK_EN,
	SYS_VO_MIPI_CLK_EN,
	SYS_VO_HDMI_TX_CLK_EN,
	SYS_VO_MIPI_CLK_SEL,
	SYS_VO_MIPI_CHN_SEL,
	SYS_VO_PLL_FRAC_SET,
	SYS_VO_PLL_POSTDIV1_SET,
	SYS_VO_PLL_POSTDIV2_SET,
	SYS_VO_PLL_REFDIV_SET,
	SYS_VO_PLL_FBDIV_SET,
	SYS_VO_SPLL_FRAC_SET,
	SYS_VO_SPLL_POSTDIV1_SET,
	SYS_VO_SPLL_POSTDIV2_SET,
	SYS_VO_SPLL_REFDIV_SET,
	SYS_VO_SPLL_FBDIV_SET,

	SYS_AI_SAMPLECLK_DIVSEL,
	SYS_AI_BITCKL_DIVSEL,
	SYS_AI_SYSCKL_SEL,
	SYS_AI_MASTER,
	SYS_AI_RESET_SEL,
	SYS_AI_CLK_EN,

	SYS_AO_SAMPLECLK_DIVSEL,
	SYS_AO_BITCKL_DIVSEL,
	SYS_AO_SYSCKL_SEL,
	SYS_AO_MASTER,
	SYS_AO_RESET_SEL,
	SYS_AO_CLK_EN,

	SYS_AIO_RESET_SEL,
	SYS_AIO_CLK_EN,
	SYS_AIO_CLK_SEL,

	SYS_AIO_RX0SD_SEL,
	SYS_AIO_HDMISD_SEL,
	SYS_AIO_PADSD_SEL,
	SYS_AIO_CODECSD_SEL,
	SYS_AIO_HDMICLK_SEL,
	SYS_AIO_PADCLK_SEL,
	SYS_AIO_CODECCLK_SEL,

	SYS_ACODEC_RESET_SEL,

	SYS_VI_BUS_RESET_SEL,
	SYS_VI_BUS_CLK_EN,
	SYS_VI_PPC_RESET_SEL,
	SYS_VI_PPC_CLK_EN,
	SYS_VI_PORT_RESET_SEL,
	SYS_VI_PORT_CLK_EN,
	SYS_VI_PPC_PORT_CLK_EN,
	SYS_VI_PPC_CHN_CLK_EN,
	SYS_VI_DEV_RESET_SEL,
	SYS_VI_DEV_CLK_EN,
	SYS_VI_PROC_RESET_SEL,
	SYS_VI_PROC_CLK_EN,
	SYS_VI_PROC_PWR_EN,
	SYS_VI_PROC_PWR_ISO,

	SYS_VI_ISP_CFG_RESET_SEL,
	SYS_VI_ISP_CORE_RESET_SEL,
	SYS_VI_ISP_CLK_EN,
	SYS_VI_CAP_GET_ONLINE_FLAG,
	SYS_VI_CAP_PROC_ONLINE_SEL,
	SYS_VI_PROC_VPSS_ONLINE_SEL,
	SYS_VI_DEV_BIND_MIPI_SEL,
	SYS_VI_DEV_BIND_SLVS_SEL,
	SYS_VI_DEV_INPUT_SEL,
	SYS_VI_DEV_GET_PORT_CLK,
	SYS_VI_DEV_SET_PORT_CLK,
	SYS_VI_DEV_SET_PT_PPC_CLK,
	SYS_VI_DEV_GET_ISP_CLK,
	SYS_VI_PROC_GET_CLK,
	SYS_ISPFE_SET_CLK,
	SYS_ISPFE_GET_CLK,
	SYS_SENSOR_CLK_OUT_SEL,

	SYS_VOU_BUS_CLK_EN,
	SYS_VOU_BUS_RESET_SEL,
	SYS_VOU_SD_RESET_SEL,
	SYS_VOU_HD_RESET_SEL,
	SYS_VOU_HD_DATE_RESET_SEL,
	SYS_VOU_HD_CLK_EN,
	SYS_VOU_DEV_CLK_EN,
	SYS_VOU_HD_OUT_CLK_EN,
	SYS_VOU_HD_CLKOUT_PHASIC_REVERSE_EN,
	SYS_VOU_DAC_CLK_PHASIC_REVERSE_EN,
	SYS_VOU_CFG_CLK_EN,
	SYS_VOU_CORE_CLK_EN,
	SYS_VOU_PPC_CLK_EN,
	SYS_VOU_APB_CLK_EN,
	SYS_VOU_OUT_CLK_EN,
	SYS_VOU_OUT_CLK_SEL,
	SYS_VOU_HD_CLK_SEL,
	SYS_VOU_HD0_PPC_SEL,
	SYS_VOU_HD1_PPC_SEL,
	SYS_VOU_HD1_DIV_MOD,
	SYS_VOU_HD0_DIV_MOD,
	SYS_VO_HDMI_CLK_EN,
	SYS_VOU_SD_DATE_CLK_EN,
	SYS_VOU_HD_DATE_CLK_EN,
	SYS_VOU_SD_DAC_EN,
	SYS_VOU_SD_DAC_PWR_EN,
	SYS_VOU_SD_DAC_DETECT_EN,
	SYS_VOU_HD_DAC_EN,
	SYS_VOU_DEV_MODE_SEL,
	SYS_VO_CLK_SEL,
	SYS_VO_SD_CLK_DIV,
	SYS_VOU_WORK_EN,
	SYS_VOU_LCD_CLK_SEL,
	SYS_VOU_HD_LCD_CLK_SEL,
	SYS_VOU_LCD_CLK_EN,
	SYS_LCD_MCLK_DIV,
	SYS_LCD_DATA_MODE,

	SYS_HDMI_RESET_SEL,
	SYS_HDMI_PIXEL_CLK_EN,
	SYS_HDMI_BUS_CLK_EN,

	SYS_VO_LowPowerCTRL,

	SYS_VEDU_RESET_SEL,
	SYS_VEDU_CLK_EN,
	SYS_VEDU_CLK_SEL,
	SYS_VEDU_SED_RESET_SEL,
	SYS_VEDU_SED_CLK_EN,
	SYS_VEDU_PWR_EN,
	SYS_VEDU_PWR_ISO,
	SYS_VEDU_PWR_STATE,

	SYS_VPSS_CLK_SEL,
	SYS_VPSS_RESET_SEL,
	SYS_VPSS_CLK_EN,
	SYS_VPSS_GET_CLK_SEL,
	SYS_VPSS_SET_CLK_SEL,
	SYS_AVS_RESET_SEL,
	SYS_AVS_CLK_EN,
	SYS_AVS_PWR_EN,
	SYS_AVS_PWR_ISO,
	SYS_AVS_PWR_STATE,

	SYS_TDE_RESET_SEL,
	SYS_TDE_CLK_EN,

	SYS_JPGE_RESET_SEL,
	SYS_JPGE_CLK_EN,
	SYS_JPGE_SEL_CLK,

	SYS_JPGD_CLOCK_SEL,
	SYS_JPGD_RESET_SEL,
	SYS_JPGD_CLK_EN,

	SYS_MD_RESET_SEL,
	SYS_MD_CLK_EN,

	SYS_IVE_RESET_SEL,
	SYS_IVE_CLK_EN,

	SYS_FD_RESET_SEL,
	SYS_FD_CLK_EN,

	SYS_DSP_CLK_SET,
	SYS_DSP0_CORE_RESET_SEL,
	SYS_DSP0_CFG_RESET_SEL,
	SYS_DSP0_DBG_RESET_SEL,
	SYS_DSP0_CLK_EN,
	SYS_DSP0_POWER_EN,
	SYS_DSP0_POWER_STATE,
	SYS_DSP1_CORE_RESET_SEL,
	SYS_DSP1_CFG_RESET_SEL,
	SYS_DSP1_DBG_RESET_SEL,
	SYS_DSP1_CLK_EN,
	SYS_DSP1_POWER_EN,
	SYS_DSP1_POWER_STATE,
	SYS_DSP2_CORE_RESET_SEL,
	SYS_DSP2_CFG_RESET_SEL,
	SYS_DSP2_DBG_RESET_SEL,
	SYS_DSP2_CLK_EN,
	SYS_DSP2_POWER_EN,
	SYS_DSP2_POWER_STATE,
	SYS_DSP3_CORE_RESET_SEL,
	SYS_DSP3_CFG_RESET_SEL,
	SYS_DSP3_DBG_RESET_SEL,
	SYS_DSP3_CLK_EN,
	SYS_DSP3_POWER_EN,
	SYS_DSP3_POWER_STATE,

	SYS_EV_CNN_RESET_SEL,
	SYS_EV_CNN_CFG_RESET_SEL,
	SYS_EV_CNN_CLK_EN,

	SYS_WK_CNN_RESET_SEL,
	SYS_WK_CNN_CLK_EN,
	SYS_WK_CNN_GET_CLK_STATE,
	SYS_WK_CNN_CLK_SET,
	SYS_WK_CNN_CLK_GET,
	SYS_WK_CNN_PWR_EN,
	SYS_WK_CNN_PWR_ISO,
	SYS_WK_CNN_PWR_STATE,
	SYS_WK_CNN1_RESET_SEL,
	SYS_WK_CNN1_CLK_EN,
	SYS_WK_CNN1_CLK_SET,
	SYS_WK_CNN1_CLK_GET,
	SYS_WK_CNN1_PWR_EN,
	SYS_WK_CNN1_PWR_ISO,
	SYS_WK_CNN1_PWR_STATE,
	SYS_DPU_CLK_SET,
	SYS_DPU_RESET_SEL,
	SYS_DPU_CLK_EN,

	SYS_PCIE_RESET_SEL,
	SYS_PCIE_CLK_EN,

	SYS_CIPHER_RESET_SEL,
	SYS_CIPHER_CLK_EN,

	SYS_VGS_RESET_SEL,
	SYS_VGS_CLK_EN,
	SYS_VGS_GET_CLK_SEL,
	SYS_VGS_SET_CLK_SEL,

	SYS_GDC_RESET_SEL,
	SYS_GDC_CLK_EN,
	SYS_GDC_GET_CLK_STATE,
	SYS_GDC_NNIE_RAM_USE,
	SYS_GDC_GET_CLKSEL,
	SYS_GDC_SET_CLKSEL,

	SYS_GDC_VGS_PWR_EN,
	SYS_GDC_VGS_PWR_ISO,
	SYS_GDC_VGS_PWR_STATE,

	SYS_VPSS_PWR_EN,
	SYS_VPSS_PWR_ISO,
	SYS_VPSS_PWR_STATE,

	SYS_DIS_RESET_SEL,
	SYS_DIS_CLK_EN,

	SYS_GDC_NNIE_MUTEX_SEL,
	SYS_VENC_NNIE_MUTEX_SEL,
	SYS_NNIE_GET_MUTEX_STATE,
	SYS_NNIE_GDC_GET_MUTEX_STATE,

	SYS_GET_SYS,
	SYS_GET_SCT,
	SYS_BUTT,
} SYS_FUNC_E;

#ifdef CONFIG_TZASC_SUPPORT
typedef enum SYS_SECURITY_VERSION_E {
	NON_TEE_VERSION = 0,
	TEE_VERSION = 1,
} SYS_SECURITY_VERSION_E;
#endif

typedef struct SYS_EXPORT_FUNC_S {
	GK_U64(*pfnSYSGetTimeStamp)
	(GK_VOID);
	GK_U64(*pfnSYSGetLocalTimeStamp)
	(GK_VOID);
	GK_VOID(*pfnSYSSyncTimeStamp)
	(GK_U64 u64Base, GK_BOOL bInit);
	GK_U32 (*pfnSysGetChipVersion)(GK_VOID);
	GK_S32(*pfnSysGetStride)
	(GK_U32 u32Width, GK_U32 *pu32Stride);
	GK_S32(*pfnSysDrvIoCtrl)
	(MPP_CHN_S *pstMppChn, SYS_FUNC_E enFuncId, GK_VOID *pIoArgs);

	GK_S32(*pfnSysRegisterSender)
	(BIND_SENDER_INFO_S *pstInfo);
	GK_S32(*pfnSysUnRegisterSender)
	(MOD_ID_E enModId);
	GK_S32(*pfnSysRegisterReceiver)
	(BIND_RECEIVER_INFO_S *pstInfo);
	GK_S32(*pfnSysUnRegisterReceiver)
	(MOD_ID_E enModId);
	GK_S32(*pfnSysSendData)
	(MOD_ID_E enModId, GK_S32 s32DevId, GK_S32 s32ChnId, GK_U32 u32Flag,
	 MPP_DATA_TYPE_E enDataType, GK_VOID *pvData);

	GK_S32(*pfnSysResetData)
	(MOD_ID_E enModId, GK_S32 s32DevId, GK_S32 s32ChnId, GK_VOID *pPrivate);

	GK_S32(*pfnGetBindbySrc)
	(MPP_CHN_S *pstSrcChn, MPP_BIND_DEST_S *pstBindSrc);
	GK_S32 (*pfnGetBindNumbySrc)(MPP_CHN_S *pstSrcChn, GK_U32 *pu32BindNum);
	GK_S32 (*pfnGetBindbyDest)(MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn);
	GK_S32 (*pfnGetMmzName)(MPP_CHN_S *pstChn, GK_VOID **ppMmzName);
	GK_S32(*pfnGetMemDdr)
	(MPP_CHN_S *pstChn, MPP_SYS_DDR_NAME_S *pstMemDdr);
	GK_S32 (*pfnGetSpinRec)(GK_U32 *u32SpinRec);

	GK_U32(*pfnSysVRegRead)
	(GK_U32 u32Addr, GK_U32 u32Bytes);
	GK_S32(*pfnSysVRegWrite)
	(GK_U32 u32Addr, GK_U32 u32Value, GK_U32 u32Bytes);
	GK_U32(*pfnSysGetVRegAddr)
	(GK_VOID);
	GK_U32(*pfnSysGetScaleCoef)
	(MOD_ID_E enModId, GK_VOID *pstScaleCoefOpt, GK_VOID *pvRate,
	 GK_VOID *pvCfg);
	GK_U64(*pfnSYSGetSchedClock)
	(GK_VOID);
	GK_S32(*pfnSYSGetFullChipId)
	(GK_U32 *pu32ChipId);

	GK_U32(*pfnSYSGetChipId)
	(GK_VOID);
	GK_U64(*pfnSYSGetEfuseAddr)
	(GK_VOID);

	GK_S32 (*pfnSYSGetTimeZone)(GK_S32 *ps32TimeZone);
	GK_S32 (*pfnSYSGetNowString)(GK_U8 *pu8NowTime);
	GPS_INFO_S *(*pfnSYSGetGPSInfo)(GK_VOID);

	GK_VOID(*pfnSYSGetCmpCfg)
	(GK_VOID *pstVFrame, GK_VOID *pstYCmpCfg, GK_VOID *pstCCmpCfg,
	 void *pstCmpModeExParam);

	GK_VOID(*pfnSYSGetVBCfg)
	(GK_VOID *pstVBBaseInfo, GK_VOID *pstVBCalConfig);
	GK_VOID(*pfnSYSGetVdecBufferCfg)
	(PAYLOAD_TYPE_E enType, GK_VOID *pstVBBaseInfo,
	 GK_VOID *pstVBCalConfig);

	GK_VOID(*pfnSYSGetCmpSegCfg)
	(GK_VOID *pstSysCmpIn, GK_VOID *pstYCmpCfg, GK_VOID *pstCCmpCfg);
	GK_VOID(*pfnSYSGetCmpBayerCfg)
	(GK_VOID *pstSysCmpIn, GK_VOID *pstBayerCmpCfg,
	 GK_VOID *pstBayerDcmpCfg);
	GK_VOID(*pfnSYSGetCmp3DNRCfg)
	(GK_VOID *pstSysCmpIn, GK_VOID *pstYCmpCfg, GK_VOID *pstCCmpCfg);
	GK_VOID(*pfnSYSGetCmpTILE16x8Cfg)
	(GK_VOID *pstSysCmpIn, GK_VOID *pstCmpOut);
	GK_VOID(*pfnSYSGetCmpMUV1Cfg)
	(GK_VOID *pstSysCmpIn, GK_VOID *pstCmpOut);
	GK_VOID(*pfnSYSGetDcmpMUV1Cfg)
	(GK_VOID *pstSysCmpIn, GK_VOID *pstCmpOut);
	GK_VOID (*pfnSYSGetCmpTDECfg)(GK_VOID *pstSysCmpIn, GK_VOID *pstCmpOut);

#ifdef CONFIG_TZASC_SUPPORT
	GK_S32 (*pfnSYSGetSecurityVersion)(SYS_SECURITY_VERSION_E *version);
#endif
} SYS_EXPORT_FUNC_S;

#define CKFN_SYS_ENTRY() CHECK_FUNC_ENTRY(MOD_ID_SYS)

#define CKFN_SYS_GetSchedClock() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetSchedClock)
#define CALL_SYS_GetSchedClock() \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetSchedClock()

#define CKFN_SYS_GetTimeStamp() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetTimeStamp)
#define CALL_SYS_GetTimeStamp() \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetTimeStamp()

#define CKFN_SYS_GetLocalTimeStamp() \
	(NULL !=                     \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetLocalTimeStamp)
#define CALL_SYS_GetLocalTimeStamp() \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetLocalTimeStamp()

#define CKFN_SYS_SyncTimeStamp() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSSyncTimeStamp)
#define CALL_SYS_SyncTimeStamp(u64Base, bInit)    \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSYSSyncTimeStamp(u64Base, bInit)

#define CKFN_SYS_GetStride() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysGetStride)
#define CALL_SYS_GetStride(u32Width, pu32Stride)  \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSysGetStride(u32Width, pu32Stride)

#define CKFN_SYS_GetChipVersion() \
	(NULL !=                  \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysGetChipVersion)
#define CALL_SYS_GetChipVersion() \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysGetChipVersion()

#define CKFN_SYS_DrvIoCtrl() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysDrvIoCtrl)
#define CALL_SYS_DrvIoCtrl(pstMppChn, enFuncId, pIoArgs) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)        \
		->pfnSysDrvIoCtrl(pstMppChn, enFuncId, pIoArgs)

#define CKFN_SYS_RegisterSender() \
	(NULL !=                  \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysRegisterSender)
#define CALL_SYS_RegisterSender(pstInfo) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysRegisterSender(pstInfo)

#define CKFN_SYS_UnRegisterSender() \
	(NULL !=                    \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysUnRegisterSender)
#define CALL_SYS_UnRegisterSender(enModId)        \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSysUnRegisterSender(enModId)

#define CKFN_SYS_RegisterReceiver() \
	(NULL !=                    \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysRegisterReceiver)
#define CALL_SYS_RegisterReceiver(pstInfo)        \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSysRegisterReceiver(pstInfo)

#define CKFN_SYS_UnRegisterReceiver() \
	(NULL !=                      \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysUnRegisterReceiver)
#define CALL_SYS_UnRegisterReceiver(enModId)      \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSysUnRegisterReceiver(enModId)

#define CKFN_SYS_SendData() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysSendData)
#define CALL_SYS_SendData(enModId, s32DevId, s32ChnId, u32Flag, enDataType, \
			  pvData)                                           \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                           \
		->pfnSysSendData(enModId, s32DevId, s32ChnId, u32Flag,      \
				 enDataType, pvData)

#define CKFN_SYS_ResetData() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysResetData)
#define CALL_SYS_ResetData(enModId, s32DevId, s32ChnId, pPrivate) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                 \
		->pfnSysResetData(enModId, s32DevId, s32ChnId, pPrivate)

#define CKFN_SYS_GetBindbySrc() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnGetBindbySrc)
#define CALL_SYS_GetBindbySrc(pstSrcChn, pstBindDest) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)     \
		->pfnGetBindbySrc(pstSrcChn, pstBindDest)

#define CKFN_SYS_GetBindNumbySrc() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnGetBindNumbySrc)
#define CALL_SYS_GetBindNumbySrc(pstSrcChn, pu32BindNum) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)        \
		->pfnGetBindNumbySrc(pstSrcChn, pu32BindNum)

#define CKFN_SYS_GetBindbyDest() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnGetBindbyDest)
#define CALL_SYS_GetBindbyDest(pstDestChn, pstSrcChn) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)     \
		->pfnGetBindbyDest(pstDestChn, pstSrcChn)

#define CKFN_SYS_GetMmzName() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnGetMmzName)
#define CALL_SYS_GetMmzName(pstSrcChn, ppMmzName) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnGetMmzName(pstSrcChn, ppMmzName)

#define CKFN_SYS_GetMemDdr() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnGetMemDdr)
#define CALL_SYS_GetMemDdr(pstSrcChn, pstMemDdr)  \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnGetMemDdr(pstSrcChn, pstMemDdr)

#define CKFN_SYS_GetScaleCoef() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSysGetScaleCoef)
#define CALL_SYS_GetScaleCoef(enModId, pstScaleCoefOpt, pvRate, pvCfg) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                      \
		->pfnSysGetScaleCoef(enModId, pstScaleCoefOpt, pvRate, pvCfg)

#define CKFN_SYS_GetFullChipId() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetFullChipId)
#define CALL_SYS_GetFullChipId(pu32ChipId)        \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSYSGetFullChipId(pu32ChipId)

#define CKFN_SYS_GetChipId() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetChipId)
#define CALL_SYS_GetChipId() \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetChipId()

#define CKFN_SYS_GetEfuseAddr() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetEfuseAddr)
#define CALL_SYS_GetEfuseAddr() \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetEfuseAddr()

#define CKFN_SYS_GetTimeZone() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetTimeZone)
#define CALL_SYS_GetTimeZone(ps32TimeZone)        \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSYSGetTimeZone(ps32TimeZone)

#define CKFN_SYS_GetGPSInfo() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetGPSInfo)
#define CALL_SYS_GetGPSInfo(pstGPSInfo) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetGPSInfo()

#define CKFN_SYS_GetNowString() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetNowString)
#define CALL_SYS_GetNowString(pu8NowTime)         \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS) \
		->pfnSYSGetNowString(pu8NowTime)

#define CKFN_SYS_GetCmpCfg() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmpCfg)
#define CALL_SYS_GetCmpCfg(pstVideoFrame, pstYCmpCfg, pstCCmpCfg,        \
			   pstCmpModeExParam)                            \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                        \
		->pfnSYSGetCmpCfg(pstVideoFrame, pstYCmpCfg, pstCCmpCfg, \
				  pstCmpModeExParam)

#define CKFN_SYS_GetVBCfg() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetVBCfg)
#define CALL_SYS_GetVBCfg(pstVBBaseInfo, pstVBCalConfig) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)        \
		->pfnSYSGetVBCfg(pstVBBaseInfo, pstVBCalConfig)

#define CKFN_SYS_GetVdecBufferCfg() \
	(NULL !=                    \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetVdecBufferCfg)
#define CALL_SYS_GetVdecBufferCfg(enType, pstVBBaseInfo, pstVBCalConfig) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                        \
		->pfnSYSGetVdecBufferCfg(enType, pstVBBaseInfo,          \
					 pstVBCalConfig)

#define CKFN_SYS_GetCmpSegCfg() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmpSegCfg)
#define CALL_SYS_GetCmpSegCfg(pstSysCmpIn, pstYCmpCfg, pstCCmpCfg) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                  \
		->pfnSYSGetCmpSegCfg(pstSysCmpIn, pstYCmpCfg, pstCCmpCfg)

#define CKFN_SYS_GetCmpBayerCfg() \
	(NULL !=                  \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmpBayerCfg)
#define CALL_SYS_GetCmpBayerCfg(pstSysCmpIn, pstBayerCmpCfg, pstBayerDcmpCfg) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                             \
		->pfnSYSGetCmpBayerCfg(pstSysCmpIn, pstBayerCmpCfg,           \
				       pstBayerDcmpCfg)

#define CKFN_SYS_GetCmp3DNRCfg() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmp3DNRCfg)
#define CALL_SYS_GetCmp3DNRCfg(pstSysCmpIn, pstYCmpCfg, pstCCmpCfg) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)                   \
		->pfnSYSGetCmp3DNRCfg(pstSysCmpIn, pstYCmpCfg, pstCCmpCfg)

#define CKFN_SYS_GetCmpTILE16x8Cfg() \
	(NULL !=                     \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmpTILE16x8Cfg)
#define CALL_SYS_GetCmpTILE16x8Cfg(pstSysCmpIn, pstCmpOut) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)          \
		->pfnSYSGetCmpTILE16x8Cfg(pstSysCmpIn, pstCmpOut)

#define CKFN_SYS_GetCmpMUV1Cfg() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmpMUV1Cfg)
#define CALL_SYS_GetCmpMUV1Cfg(pstSysCmpIn, pstCmpOut) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)      \
		->pfnSYSGetCmpMUV1Cfg(pstSysCmpIn, pstCmpOut)

#define CKFN_SYS_GetDcmpMUV1Cfg() \
	(NULL !=                  \
	 FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetDcmpMUV1Cfg)
#define CALL_SYS_GetDcmpMUV1Cfg(pstSysCmpIn, pstDcmpOut) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)        \
		->pfnSYSGetDcmpMUV1Cfg(pstSysCmpIn, pstDcmpOut)

#define CKFN_SYS_GetCmpTDECfg() \
	(NULL != FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)->pfnSYSGetCmpTDECfg)
#define CALL_SYS_GetCmpTDECfg(pstSysCmpIn, pstCmpOut) \
	FUNC_ENTRY(SYS_EXPORT_FUNC_S, MOD_ID_SYS)     \
		->pfnSYSGetCmpTDECfg(pstSysCmpIn, pstCmpOut)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SYS_EXT_H__ */
