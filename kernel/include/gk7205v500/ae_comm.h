/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __ISP_AE_COMM_H__
#define __ISP_AE_COMM_H__

#include "type.h"
#include "comm_isp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ISP_AE_LIB_NAME "ae_lib"

typedef enum AE_CTRL_CMD_E {
    AE_DEBUG_ATTR_SET,
    AE_DEBUG_ATTR_GET,

    AE_CTRL_BUTT,
} AE_CTRL_CMD_E;

typedef struct AE_DBG_ATTR_S {
    XMEDIA_U32  u32MaxIntTime;
    XMEDIA_U32  u32MinIntTime;
    XMEDIA_U32  u32MaxAgain;
    XMEDIA_U32  u32MinAgain;
    XMEDIA_U32  u32MaxDgain;
    XMEDIA_U32  u32MinDgain;
    XMEDIA_U32  u32MaxIspDgain;
    XMEDIA_U32  u32MinIspDgain;
    XMEDIA_U32  u32MaxSysGain;
    XMEDIA_U32  u32MinSysGain;
    XMEDIA_U32  u32Compensation;
    XMEDIA_U32  u32EVBias;
    XMEDIA_BOOL bManualExposureEn;
    XMEDIA_BOOL bManualTimeEn;
    XMEDIA_BOOL bManualAgainEn;
    XMEDIA_BOOL bManualDgainEn;
    XMEDIA_BOOL bManualIspDgainEn;
    XMEDIA_U32  u32ManualExposureLines;
    XMEDIA_U32  u32ManualAgain;
    XMEDIA_U32  u32ManualDgain;
    XMEDIA_U32  u32ManualIspDgain;
    XMEDIA_U32  au32AeWeights[AE_ZONE_ROW *AE_ZONE_COLUMN];
} AE_DBG_ATTR_S;

typedef struct AE_DBG_STATUS_S {
    XMEDIA_U32  u32FrmNumBgn;
    XMEDIA_U32  u32FullLines;
    XMEDIA_U32  u32IntTime;
    XMEDIA_U32  u32ShortIntTime;
    XMEDIA_U32  u32MedIntTime;
    XMEDIA_U32  u32LongIntTime;
    XMEDIA_U32  u32Again;
    XMEDIA_U32  u32Dgain;
    XMEDIA_U32  u32IspDgain;
    XMEDIA_U32  u32IrisFNOLin;
    XMEDIA_U64  u64Exposure;
    XMEDIA_U32  u32Increment;
    XMEDIA_S32  s32HistError;
    XMEDIA_S32  s32HistOriAverage;
    XMEDIA_S32  s32LumaOffset;
    XMEDIA_U32  u32Iso;
    XMEDIA_U32  u32ExpRatio;
    XMEDIA_U32  u32OverExpRatio;
    XMEDIA_U32  u32OverExpRatioFilter;
} AE_DBG_STATUS_S;

typedef enum AE_ACCURACY_E {
    AE_ACCURACY_DB = 0,
    AE_ACCURACY_LINEAR,
    AE_ACCURACY_TABLE,

    AE_ACCURACY_BUTT,
} AE_ACCURACY_E;

typedef struct AE_ACCURACY_S {
    AE_ACCURACY_E enAccuType;
    float   f32Accuracy;
    float   f32Offset;
} AE_ACCURACY_S;

typedef struct AE_SENSOR_DEFAULT_S {
    XMEDIA_U8   au8HistThresh[HIST_THRESH_NUM];
    XMEDIA_U8   u8AeCompensation;

    XMEDIA_U32  u32LinesPer500ms;
    XMEDIA_U32  u32FlickerFreq;
    XMEDIA_FLOAT f32Fps;
    XMEDIA_U32  u32HmaxTimes;
    XMEDIA_U32  u32InitExposure;
    XMEDIA_U32  u32InitAESpeed;
    XMEDIA_U32  u32InitAETolerance;

    XMEDIA_U32  u32FullLinesStd;
    XMEDIA_U32  u32FullLinesMax;
    XMEDIA_U32  u32FullLines;
    XMEDIA_U32  u32MaxIntTime;
    XMEDIA_U32  u32MinIntTime;
    XMEDIA_U32  u32MaxIntTimeTarget;
    XMEDIA_U32  u32MinIntTimeTarget;
    AE_ACCURACY_S stIntTimeAccu;

    XMEDIA_U32  u32MaxAgain;
    XMEDIA_U32  u32MinAgain;
    XMEDIA_U32  u32MaxAgainTarget;
    XMEDIA_U32  u32MinAgainTarget;
    AE_ACCURACY_S stAgainAccu;

    XMEDIA_U32  u32MaxDgain;
    XMEDIA_U32  u32MinDgain;
    XMEDIA_U32  u32MaxDgainTarget;
    XMEDIA_U32  u32MinDgainTarget;
    AE_ACCURACY_S stDgainAccu;

    XMEDIA_U32  u32MaxISPDgainTarget;
    XMEDIA_U32  u32MinISPDgainTarget;
    XMEDIA_U32  u32ISPDgainShift;

    XMEDIA_U32  u32MaxIntTimeStep;
    XMEDIA_U32  u32LFMaxShortTime;
    XMEDIA_U32  u32LFMinExposure;

    ISP_AE_ROUTE_S stAERouteAttr;
    XMEDIA_BOOL bAERouteExValid;
    ISP_AE_ROUTE_EX_S stAERouteAttrEx;

    ISP_AE_ROUTE_S stAERouteSFAttr;
    ISP_AE_ROUTE_EX_S stAERouteSFAttrEx;

    XMEDIA_U16 u16ManRatioEnable;
    XMEDIA_U32 au32Ratio[EXP_RATIO_NUM];

    ISP_IRIS_TYPE_E  enIrisType;
    ISP_PIRIS_ATTR_S stPirisAttr;
    ISP_IRIS_F_NO_E  enMaxIrisFNO;
    ISP_IRIS_F_NO_E  enMinIrisFNO;

    ISP_AE_STRATEGY_E enAeExpMode;

    XMEDIA_U16 u16ISOCalCoef;
    XMEDIA_U8  u8AERunInterval;
    XMEDIA_U32 u32ExpRatioMax;
    XMEDIA_U32 u32ExpRatioMin;
    XMEDIA_BOOL bDiffGainSupport;
} AE_SENSOR_DEFAULT_S;

typedef struct AE_FSWDR_ATTR_S {
    ISP_FSWDR_MODE_E enFSWDRMode;
} AE_FSWDR_ATTR_S;

typedef struct AE_SENSOR_EXP_FUNC_S {
    XMEDIA_S32 (*pfn_cmos_get_ae_default)(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft);

    XMEDIA_VOID (*pfn_cmos_fps_set)(VI_PIPE ViPipe, XMEDIA_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft);
    XMEDIA_VOID (*pfn_cmos_slow_framerate_set)(VI_PIPE ViPipe, XMEDIA_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft);

    XMEDIA_VOID (*pfn_cmos_inttime_update)(VI_PIPE ViPipe, XMEDIA_U32 u32IntTime);
    XMEDIA_VOID (*pfn_cmos_gains_update)(VI_PIPE ViPipe, XMEDIA_U32 u32Again, XMEDIA_U32 u32Dgain);

    XMEDIA_VOID (*pfn_cmos_again_calc_table)(VI_PIPE ViPipe, XMEDIA_U32 *pu32AgainLin, XMEDIA_U32 *pu32AgainDb);
    XMEDIA_VOID (*pfn_cmos_dgain_calc_table)(VI_PIPE ViPipe, XMEDIA_U32 *pu32DgainLin, XMEDIA_U32 *pu32DgainDb);

    XMEDIA_VOID (*pfn_cmos_get_inttime_max)(VI_PIPE ViPipe, XMEDIA_U16 u16ManRatioEnable, XMEDIA_U32 *au32Ratio, XMEDIA_U32 *au32IntTimeMax, XMEDIA_U32 *au32IntTimeMin, XMEDIA_U32 *pu32LFMaxIntTime);

    XMEDIA_VOID (*pfn_cmos_ae_fswdr_attr_set)(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr);

} AE_SENSOR_EXP_FUNC_S;

typedef struct AE_SENSOR_REGISTER_S {
    AE_SENSOR_EXP_FUNC_S stSnsExp;
} AE_SENSOR_REGISTER_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
