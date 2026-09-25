/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_COMM_3A_H__
#define __ISP_COMM_3A_H__

#include "common.h"
#include "comm_isp.h"
#include "comm_sns.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IS_LINEAR_MODE(mode)      ((mode) == WDR_MODE_NONE)
#define IS_WDR_MODE(mode)         ((!IS_LINEAR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_HDR_MODE(mode)         (((mode) > DYNAMIC_RANGE_SDR10) && ((mode) < DYNAMIC_RANGE_BUTT))
#define IS_BUILT_IN_WDR_MODE(mode) ((mode) == WDR_MODE_BUILT_IN)
#define IS_FS_WDR_MODE(mode)      ((!IS_LINEAR_MODE(mode))&& (!IS_BUILT_IN_WDR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_2to1_WDR_MODE(mode)    (((mode) == WDR_MODE_2To1_FRAME) || ((mode) == WDR_MODE_2To1_FRAME_FULL_RATE) || ((mode) == WDR_MODE_2To1_LINE) || ((mode) == WDR_MODE_QUDRA))
#define IS_3to1_WDR_MODE(mode)    (((mode) == WDR_MODE_3To1_FRAME) || ((mode) == WDR_MODE_3To1_FRAME_FULL_RATE) || ((mode) == WDR_MODE_3To1_LINE))
#define IS_4to1_WDR_MODE(mode)    (((mode) == WDR_MODE_4To1_FRAME) || ((mode) == WDR_MODE_4To1_FRAME_FULL_RATE) || ((mode) == WDR_MODE_4To1_LINE))
#define IS_FULL_WDR_MODE(mode)    (((mode) == WDR_MODE_2To1_FRAME_FULL_RATE) || ((mode) == WDR_MODE_3To1_FRAME_FULL_RATE) || ((mode) == WDR_MODE_4To1_FRAME_FULL_RATE))
#define IS_HALF_WDR_MODE(mode)    (((mode) == WDR_MODE_2To1_FRAME) || ((mode) == WDR_MODE_3To1_FRAME) || ((mode) == WDR_MODE_4To1_FRAME))
#define IS_LINE_WDR_MODE(mode)    (((mode) == WDR_MODE_2To1_LINE) || ((mode) == WDR_MODE_3To1_LINE) || ((mode) == WDR_MODE_4To1_LINE) || ((mode) == WDR_MODE_QUDRA))

#define IS_STITCH_MAIN_PIPE(ViPipe,MainPipe) ((ViPipe) == (MainPipe))
#define IS_OFFLINE_MODE(mode)    ((mode) == ISP_MODE_RUNNING_OFFLINE)
#define IS_ONLINE_MODE(mode)     ((mode) == ISP_MODE_RUNNING_ONLINE)
#define IS_SIDEBYSIDE_MODE(mode) ((mode) == ISP_MODE_RUNNING_SIDEBYSIDE)
#define IS_STRIPING_MODE(mode)   ((mode) == ISP_MODE_RUNNING_STRIPING)

#define MAX_REGISTER_ALG_LIB_NUM 2

typedef enum ISP_ALG_MOD_E {
    ISP_ALG_AE = 0,
    ISP_ALG_AF,
    ISP_ALG_AWB,
    ISP_ALG_BLC,
    ISP_ALG_FELOGLUT,
    ISP_ALG_LOGLUT,
    ISP_ALG_DP,
    ISP_ALG_PREGAMMA,
    ISP_ALG_DRC,
    ISP_ALG_DEMOSAIC,
    ISP_ALG_ANTIFALSECOLOR,
    ISP_ALG_GAMMA,
    ISP_ALG_GE,
    ISP_ALG_SHARPEN,
    ISP_ALG_EDGEAMRK,
    ISP_ALG_FrameWDR,
    ISP_ALG_FPN,
    ISP_ALG_DEHAZE,
    ISP_ALG_ACM,
    ISP_ALG_LCAC,
    ISP_ALG_GCAC,
    ISP_ALG_CSC,
    ISP_ALG_EXPANDER,
    ISP_ALG_MCDS,
    ISP_ALG_LSC,
    ISP_ALG_RLSC,
    ISP_ALG_FeLSC,
    ISP_ALG_RC,
    ISP_ALG_RGBIR,
    ISP_ALG_HRS,
    ISP_ALG_DG,
    ISP_ALG_BAYERNR,
    ISP_ALG_FLICKER,
    ISP_ALG_SPLIT,
    ISP_ALG_LDCI,
    ISP_ALG_CA,
    ISP_ALG_CLUT,
    ISP_ALG_DETAIL,
    ISP_ALG_BUTT,
} ISP_ALG_MOD_E;

typedef enum ISP_CTRL_CMD_E {
    ISP_WDR_MODE_SET = 8000,
    ISP_PROC_WRITE,

    ISP_AE_FPS_BASE_SET,
    ISP_AE_BLC_SET,
    ISP_AE_RC_SET,
    ISP_AE_BAYER_FORMAT_SET,
    ISP_AE_INIT_INFO_GET,

    ISP_AWB_ISO_SET,
    ISP_CHANGE_IMAGE_MODE_SET,
    ISP_UPDATE_INFO_GET,
    ISP_FRAMEINFO_GET,
    ISP_ATTACHINFO_GET,
    ISP_COLORGAMUTINFO_GET,
    ISP_AWB_INTTIME_SET,
    ISP_BAS_MODE_SET,
    ISP_PROTRIGGER_SET,
    ISP_AWB_PIRIS_SET,
    ISP_AWB_SNAP_MODE_SET,
    ISP_AWB_ZONE_ROW_SET,
    ISP_AWB_ZONE_COL_SET,
    ISP_AWB_ZONE_BIN_SET,
    ISP_CTRL_CMD_BUTT,
} ISP_CTRL_CMD_E;

typedef struct ISP_CTRL_PROC_WRITE_S {
    XMEDIA_CHAR *pcProcBuff;
    XMEDIA_U32   u32BuffLen;
    XMEDIA_U32   u32WriteLen;
} ISP_CTRL_PROC_WRITE_S;


typedef struct ISP_STITCH_ATTR_S {
    XMEDIA_BOOL bStitchEnable;
    XMEDIA_BOOL bMainPipe;
    XMEDIA_U8   u8StitchPipeNum;
    XMEDIA_S8   as8StitchBindId[VI_MAX_PIPE_NUM];
} ISP_STITCH_ATTR_S;
typedef struct ISP_AE_PARAM_S {
    SENSOR_ID SensorId;
    XMEDIA_U8  u8WDRMode;
    XMEDIA_U8  u8HDRMode;
    XMEDIA_U16 u16BlackLevel;
    XMEDIA_FLOAT f32Fps;
    ISP_BAYER_FORMAT_E enBayer;
    ISP_STITCH_ATTR_S stStitchAttr;

    XMEDIA_S32 s32Rsv;
} ISP_AE_PARAM_S;

typedef struct ISP_FE_AE_STAT_1_S {
    XMEDIA_U32  u32PixelCount[ISP_CHN_MAX_NUM];
    XMEDIA_U32  u32PixelWeight[ISP_CHN_MAX_NUM];
    XMEDIA_U32  au32HistogramMemArray[ISP_CHN_MAX_NUM][1024];
} ISP_FE_AE_STAT_1_S;

typedef struct ISP_FE_AE_STAT_2_S {
    XMEDIA_U16  u16GlobalAvgR[ISP_CHN_MAX_NUM];
    XMEDIA_U16  u16GlobalAvgGr[ISP_CHN_MAX_NUM];
    XMEDIA_U16  u16GlobalAvgGb[ISP_CHN_MAX_NUM];
    XMEDIA_U16  u16GlobalAvgB[ISP_CHN_MAX_NUM];
} ISP_FE_AE_STAT_2_S;

typedef struct ISP_FE_AE_STAT_3_S {
    XMEDIA_U16  au16ZoneAvg[ISP_CHN_MAX_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN][4];
} ISP_FE_AE_STAT_3_S;

typedef struct ISP_FE_AE_STITCH_STAT_3_S {
    XMEDIA_U16  au16ZoneAvg[VI_MAX_PIPE_NUM][ISP_CHN_MAX_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN][4];
} ISP_FE_AE_STITCH_STAT_3_S;

typedef struct ISP_BE_AE_STAT_1_S {
    XMEDIA_U32  u32PixelCount;
    XMEDIA_U32  u32PixelWeight;
    XMEDIA_U32  au32HistogramMemArray[1024];
} ISP_BE_AE_STAT_1_S;

typedef struct ISP_BE_AE_STAT_2_S {
    XMEDIA_U16  u16GlobalAvgR;
    XMEDIA_U16  u16GlobalAvgGr;
    XMEDIA_U16  u16GlobalAvgGb;
    XMEDIA_U16  u16GlobalAvgB;
} ISP_BE_AE_STAT_2_S;

typedef struct ISP_BE_AE_STAT_3_S {
    XMEDIA_U16  au16ZoneAvg[AE_ZONE_ROW][AE_ZONE_COLUMN][4];
} ISP_BE_AE_STAT_3_S;

typedef struct ISP_BE_AE_STITCH_STAT_3_S {
    XMEDIA_U16  au16ZoneAvg[VI_MAX_PIPE_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN][4];
} ISP_BE_AE_STITCH_STAT_3_S;

typedef struct ISP_AE_INFO_S {
    XMEDIA_U32  u32FrameCnt;
    ISP_SMART_INFO_S stSmartInfo;

    ISP_FE_AE_STAT_1_S *pstFEAeStat1;
    ISP_FE_AE_STAT_2_S *pstFEAeStat2;
    ISP_FE_AE_STAT_3_S *pstFEAeStat3;
    ISP_FE_AE_STITCH_STAT_3_S *pstFEAeStiStat;
    ISP_BE_AE_STAT_1_S *pstBEAeStat1;
    ISP_BE_AE_STAT_2_S *pstBEAeStat2;
    ISP_BE_AE_STAT_3_S *pstBEAeStat3;
    ISP_BE_AE_STITCH_STAT_3_S *pstBEAeStiStat;
} ISP_AE_INFO_S;


typedef struct ISP_AE_STAT_ATTR_S {
    XMEDIA_BOOL bChange;

    XMEDIA_BOOL bHistAdjust;
    XMEDIA_U8 u8AEBESel;
    XMEDIA_U8 u8FourPlaneMode;
    XMEDIA_U8 u8HistOffsetX;
    XMEDIA_U8 u8HistOffsetY;
    XMEDIA_U8 u8HistSkipX;
    XMEDIA_U8 u8HistSkipY;

    XMEDIA_BOOL bModeUpdate;
    XMEDIA_U8 u8HistMode;
    XMEDIA_U8 u8AverMode;
    XMEDIA_U8 u8MaxGainMode;

    XMEDIA_BOOL bWightTableUpdate;
    XMEDIA_U8 au8WeightTable[VI_MAX_PIPE_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN];
} ISP_AE_STAT_ATTR_S;

#define AE_INT_TIME_NUM          (4)
typedef struct ISP_AE_RESULT_S {
    XMEDIA_U32  u32IntTime[AE_INT_TIME_NUM];
    XMEDIA_U32  u32IspDgain;
    XMEDIA_U32  u32Again;
    XMEDIA_U32  u32Dgain;
    XMEDIA_U32  u32Iso;
    XMEDIA_U32  u32IspDgainSF;
    XMEDIA_U32  u32AgainSF;
    XMEDIA_U32  u32DgainSF;
    XMEDIA_U32  u32IsoSF;
    XMEDIA_U8   u8AERunInterval;

    XMEDIA_BOOL bPirisValid;
    XMEDIA_S32  s32PirisPos;
    XMEDIA_U32  u32PirisGain;

    ISP_FSWDR_MODE_E enFSWDRMode;
    XMEDIA_U32  au32WDRGain[4];
    XMEDIA_U32  u32HmaxTimes;
    XMEDIA_U32  u32Vmax;

    ISP_AE_STAT_ATTR_S stStatAttr;
    ISP_DCF_UPDATE_INFO_S stUpdateInfo;
} ISP_AE_RESULT_S;

typedef struct ISP_AE_INIT_INFO_S {
    XMEDIA_U32 u32IspDgain;
    XMEDIA_U32 u32Iso;
} ISP_AE_INIT_INFO_S;

typedef struct ISP_AE_EXP_FUNC_S {
    XMEDIA_S32 (*pfn_ae_init)(XMEDIA_S32 s32Handle, const ISP_AE_PARAM_S *pstAeParam);
    XMEDIA_S32 (*pfn_ae_run)(XMEDIA_S32 s32Handle,
                         const ISP_AE_INFO_S *pstAeInfo,
                         ISP_AE_RESULT_S *pstAeResult,
                         XMEDIA_S32 s32Rsv
                        );
    XMEDIA_S32 (*pfn_ae_ctrl)(XMEDIA_S32 s32Handle, XMEDIA_U32 u32Cmd, XMEDIA_VOID *pValue);
    XMEDIA_S32 (*pfn_ae_exit)(XMEDIA_S32 s32Handle);
} ISP_AE_EXP_FUNC_S;

typedef struct ISP_AE_REGISTER_S {
    ISP_AE_EXP_FUNC_S stAeExpFunc;
} ISP_AE_REGISTER_S;

typedef enum AWB_CTRL_CMD_E {
    AWB_CCM_CONFIG_SET,
    AWB_CCM_CONFIG_GET,

    AWB_DEBUG_ATTR_SET,
    AWB_DEBUG_ATTR_GET,

    AWB_CTRL_BUTT,
} AWB_CTRL_CMD_E;

typedef struct AWB_CCM_CONFIG_S {
    XMEDIA_BOOL bAWBBypassEn;
    XMEDIA_BOOL bManualSatEn;
    XMEDIA_BOOL bManualTempEn;

    XMEDIA_U32  u32ManualSatValue;
    XMEDIA_U32  u32ManualTempValue;
    XMEDIA_U16  u16CCMSpeed;

    XMEDIA_U16  au16HighCCM[9];
    XMEDIA_U16  au16LowCCM[9];
    XMEDIA_U16  u16HighColorTemp;
    XMEDIA_U16  u16LowColorTemp;
} AWB_CCM_CONFIG_S;

typedef struct ISP_AWB_PARAM_S {
    SENSOR_ID SensorId;
    XMEDIA_U8 u8WDRMode;
    XMEDIA_U8 u8AWBZoneRow;
    XMEDIA_U8 u8AWBZoneCol;
    XMEDIA_U8 u8AWBZoneBin;
    ISP_STITCH_ATTR_S stStitchAttr;
    XMEDIA_U16 u16AWBWidth;
    XMEDIA_U16 u16AWBHeight;
    XMEDIA_U32 u32InitIso;
    XMEDIA_S8 s8Rsv;
} ISP_AWB_PARAM_S;

typedef struct ISP_AWB_STAT_1_S {
    XMEDIA_U16  u16MeteringAwbAvgR;
    XMEDIA_U16  u16MeteringAwbAvgG;
    XMEDIA_U16  u16MeteringAwbAvgB;
    XMEDIA_U16  u16MeteringAwbCountAll;
} ISP_AWB_STAT_1_S;

typedef struct ISP_AWB_STAT_2_S {
    XMEDIA_U16  au16MeteringMemArrayAvgR[AWB_ZONE_NUM];
    XMEDIA_U16  au16MeteringMemArrayAvgG[AWB_ZONE_NUM];
    XMEDIA_U16  au16MeteringMemArrayAvgB[AWB_ZONE_NUM];
    XMEDIA_U16  au16MeteringMemArrayCountAll[AWB_ZONE_NUM];

} ISP_AWB_STAT_2_S;

typedef struct ISP_AWB_STAT_STITCH_S {
    XMEDIA_U16  u16ZoneCol;
    XMEDIA_U16  u16ZoneRow;
    XMEDIA_U16  u16ZoneBin;
    XMEDIA_U16  au16MeteringMemArrayAvgR[AWB_ZONE_STITCH_MAX];
    XMEDIA_U16  au16MeteringMemArrayAvgG[AWB_ZONE_STITCH_MAX];
    XMEDIA_U16  au16MeteringMemArrayAvgB[AWB_ZONE_STITCH_MAX];
    XMEDIA_U16  au16MeteringMemArrayCountAll[AWB_ZONE_STITCH_MAX];

} ISP_AWB_STAT_STITCH_S;

typedef struct ISP_AWB_STAT_RESULT_S {
    XMEDIA_U16 *pau16ZoneAvgR;
    XMEDIA_U16 *pau16ZoneAvgG;
    XMEDIA_U16 *pau16ZoneAvgB;
    XMEDIA_U16 *pau16ZoneCount;
} ISP_AWB_STAT_RESULT_S;

typedef struct ISP_AWB_INFO_S {
    XMEDIA_U32  u32FrameCnt;

    ISP_AWB_STAT_1_S *pstAwbStat1;
    ISP_AWB_STAT_RESULT_S stAwbStat2;
    XMEDIA_U8 u8AwbGainSwitch;
    XMEDIA_U8 u8AwbStatSwitch;
    XMEDIA_U32 au32WDRWBGain[ISP_BAYER_CHN_NUM];
} ISP_AWB_INFO_S;

typedef struct ISP_AWB_RAW_STAT_ATTR_S {
    XMEDIA_BOOL bStatCfgUpdate;

    XMEDIA_U16  u16MeteringWhiteLevelAwb;
    XMEDIA_U16  u16MeteringBlackLevelAwb;
    XMEDIA_U16  u16MeteringCrRefMaxAwb;
    XMEDIA_U16  u16MeteringCbRefMaxAwb;
    XMEDIA_U16  u16MeteringCrRefMinAwb;
    XMEDIA_U16  u16MeteringCbRefMinAwb;
} ISP_AWB_RAW_STAT_ATTR_S;

typedef struct ISP_AWB_RESULT_S {
    XMEDIA_U32  au32WhiteBalanceGain[ISP_BAYER_CHN_NUM];
    XMEDIA_U16  au16ColorMatrix[CCM_MATRIX_SIZE];
    XMEDIA_U32  u32ColorTemp;
    XMEDIA_U8   u8Saturation;
    ISP_AWB_RAW_STAT_ATTR_S stRawStatAttr;
} ISP_AWB_RESULT_S;

typedef struct ISP_AWB_EXP_FUNC_S {
    XMEDIA_S32 (*pfn_awb_init)(XMEDIA_S32 s32Handle, const ISP_AWB_PARAM_S *pstAwbParam, ISP_AWB_RESULT_S *pstAwbResult);
    XMEDIA_S32 (*pfn_awb_run)(XMEDIA_S32 s32Handle,
                          const ISP_AWB_INFO_S *pstAwbInfo,
                          ISP_AWB_RESULT_S *pstAwbResult,
                          XMEDIA_S32 s32Rsv);
    XMEDIA_S32 (*pfn_awb_ctrl)(XMEDIA_S32 s32Handle, XMEDIA_U32 u32Cmd, XMEDIA_VOID *pValue);
    XMEDIA_S32 (*pfn_awb_exit)(XMEDIA_S32 s32Handle);
} ISP_AWB_EXP_FUNC_S;

typedef struct ISP_AWB_REGISTER_S {
    ISP_AWB_EXP_FUNC_S stAwbExpFunc;
} ISP_AWB_REGISTER_S;

typedef struct ISP_AF_ZONE_S {
    XMEDIA_U16  u16v1;
    XMEDIA_U16  u16h1;
    XMEDIA_U16  u16v2;
    XMEDIA_U16  u16h2;
    XMEDIA_U16  u16y;
    XMEDIA_U16  u16HlCnt;
} ISP_AF_ZONE_S;

typedef struct ISP_FE_AF_STAT_S {
    ISP_AF_ZONE_S stZoneMetrics[WDR_CHN_MAX][AF_ZONE_ROW][AF_ZONE_COLUMN];
} ISP_FE_AF_STAT_S;

typedef struct ISP_BE_AF_STAT_S {
    ISP_AF_ZONE_S stZoneMetrics[AF_ZONE_ROW][AF_ZONE_COLUMN];
} ISP_BE_AF_STAT_S;

#define ALG_LIB_NAME_SIZE_MAX    (20)
typedef struct ALG_LIB_S {
    XMEDIA_S32  s32Id;
    XMEDIA_CHAR acLibName[ALG_LIB_NAME_SIZE_MAX];
} ALG_LIB_S;

typedef struct ISP_BIND_ATTR_S {
    SENSOR_ID   SensorId;
    ALG_LIB_S   stAeLib;
    ALG_LIB_S   stAfLib;
    ALG_LIB_S   stAwbLib;
} ISP_BIND_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
