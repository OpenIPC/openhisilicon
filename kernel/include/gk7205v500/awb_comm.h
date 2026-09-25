/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __ISP_AWB_COMM_H__
#define __ISP_AWB_COMM_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ISP_AWB_LIB_NAME "awb_lib"

typedef struct AWB_DBG_ATTR_S {
    XMEDIA_U16 u16WhiteLevel;
    XMEDIA_U16 u16BlackLevel;
    XMEDIA_U16 u16CrMax;
    XMEDIA_U16 u16CrMin;
    XMEDIA_U16 u16CbMax;
    XMEDIA_U16 u16CbMin;
    XMEDIA_U16 u16CrHigh;
    XMEDIA_U16 u16CrLow;
    XMEDIA_U16 u16CbHigh;
    XMEDIA_U16 u16CbLow;
    XMEDIA_U16 u16RawWhiteLevel;
    XMEDIA_U16 u16RawBlackLevel;
    XMEDIA_U16 u16RawCrMax;
    XMEDIA_U16 u16RawCrMin;
    XMEDIA_U16 u16RawCbMax;
    XMEDIA_U16 u16RawCbMin;
    XMEDIA_U16 u16RawCrHigh;
    XMEDIA_U16 u16RawCrLow;
    XMEDIA_U16 u16RawCbHigh;
    XMEDIA_U16 u16RawCbLow;

    XMEDIA_U16 u16WDRMode;
    XMEDIA_U16 u16Enable;
    XMEDIA_U16 u16ManualEnable;
    XMEDIA_U16 u16Zone;
    XMEDIA_U16 u16HighTemp;
    XMEDIA_U16 u16LowTemp;
    XMEDIA_U16 u16RefTemp;
    XMEDIA_U16 u16RgainBase;
    XMEDIA_U16 u16GgainBase;
    XMEDIA_U16 u16BgainBase;
    XMEDIA_S32 s32p1;
    XMEDIA_S32 s32p2;
    XMEDIA_S32 s32q;
    XMEDIA_S32 s32a;
    XMEDIA_S32 s32c;

    XMEDIA_U16 u16ManSatEnable;
    XMEDIA_U16 u16SatTarget;
} AWB_DBG_ATTR_S;

typedef struct AWB_ZONE_DBG_S {
    XMEDIA_U16 u16Sum;
    XMEDIA_U16 u16Rg;
    XMEDIA_U16 u16Bg;
    XMEDIA_U16 u16CountAll;
    XMEDIA_U16 u16CountMin;
    XMEDIA_U16 u16CountMax;
    XMEDIA_U16 u16RawRAvg;
    XMEDIA_U16 u16RawGAvg;
    XMEDIA_U16 u16RawBAvg;
    XMEDIA_U16 u16TK;
    XMEDIA_U16 u16Weight;
    XMEDIA_S16 s16Shift;
} AWB_ZONE_DBG_S;

typedef struct AWB_DBG_STATUS_S {
    XMEDIA_U32 u32FrmNumBgn;
    XMEDIA_U32 u32GlobalSum;
    XMEDIA_U16 u16GlobalRgSta;
    XMEDIA_U16 u16GlobalBgSta;
    XMEDIA_U16 u16GlobalCountAll;
    XMEDIA_U16 u16GlobalCountMin;
    XMEDIA_U16 u16GlobalCountMax;
    XMEDIA_U16 u16GlobalRAvg;
    XMEDIA_U16 u16GlobalGAvg;
    XMEDIA_U16 u16GlobalBAvg;
    XMEDIA_U16 u16TK;
    XMEDIA_U16 u16Rgain;
    XMEDIA_U16 u16Ggain;
    XMEDIA_U16 u16Bgain;
    XMEDIA_U16 au16CCM[CCM_MATRIX_SIZE];

    XMEDIA_U32 au32HistInfo[256];
    AWB_ZONE_DBG_S astZoneDebug[AWB_ZONE_NUM];

    XMEDIA_U32 u32FrmNumEnd;
} AWB_DBG_STATUS_S;


typedef struct AWB_CCM_TAB_S {
    XMEDIA_U16 u16ColorTemp;
    XMEDIA_U16 au16CCM[CCM_MATRIX_SIZE];
} AWB_CCM_TAB_S;

typedef struct AWB_CCM_S {
    XMEDIA_U16  u16CCMTabNum;
    AWB_CCM_TAB_S astCCMTab[CCM_MATRIX_NUM];
} AWB_CCM_S;

typedef struct AWB_AGC_TABLE_S {
    XMEDIA_BOOL bValid;

    XMEDIA_U8   au8Saturation[ISP_AUTO_ISO_STRENGTH_NUM];
} AWB_AGC_TABLE_S;

typedef struct AWB_SENSOR_DEFAULT_S {
    XMEDIA_U16  u16WbRefTemp;
    XMEDIA_U16  au16GainOffset[ISP_BAYER_CHN_NUM];
    XMEDIA_S32  as32WbPara[AWB_CURVE_PARA_NUM];

    XMEDIA_U16  u16GoldenRgain;
    XMEDIA_U16  u16GoldenBgain;
    XMEDIA_U16  u16SampleRgain;
    XMEDIA_U16  u16SampleBgain;
    AWB_AGC_TABLE_S stAgcTbl;
    AWB_CCM_S stCcm;
    XMEDIA_U16    u16InitRgain;
    XMEDIA_U16    u16InitGgain;
    XMEDIA_U16    u16InitBgain;
    XMEDIA_U8     u8AWBRunInterval;
    XMEDIA_U16    au16InitCCM[CCM_MATRIX_SIZE];
} AWB_SENSOR_DEFAULT_S;

typedef struct AWB_SPEC_SENSOR_DEFAULT_S {
    ISP_SPECAWB_ATTR_S  stSpecAwbAttrs;
    ISP_SPECAWB_CAA_CONTROl_S stCaaControl;
} AWB_SPEC_SENSOR_DEFAULT_S;

typedef struct AWB_SENSOR_EXP_FUNC_S {
    XMEDIA_S32 (*pfn_cmos_get_awb_default)(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft);
    XMEDIA_S32 (*pfn_cmos_get_awb_spec_default)(VI_PIPE ViPipe, AWB_SPEC_SENSOR_DEFAULT_S *pstAwbSpecSnsDft);
} AWB_SENSOR_EXP_FUNC_S;

typedef struct AWB_SENSOR_REGISTER_S {
    AWB_SENSOR_EXP_FUNC_S stSnsExp;
} AWB_SENSOR_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
