/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_VPSS_H__
#define __COMM_VPSS_H__

#include "type.h"
#include "common.h"
#include "errcode.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VPSS_SHARPEN_GAIN_NUM        (32)
#define VPSS_AUTO_ISO_STRENGTH_NUM   (16)
#define VPSS_YUV_SHPLUMA_NUM         (32)
#define VPSS_3DNR_MAX_AUTO_PARAM_NUM (16)

#define ERR_CODE_VPSS_NULL_PTR         DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_VPSS_NOTREADY         DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_VPSS_INVALID_DEVID    DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define ERR_CODE_VPSS_INVALID_CHNID    DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_VPSS_EXIST            DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_VPSS_UNEXIST          DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_VPSS_NOT_SUPPORT      DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_VPSS_NOT_PERM         DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_VPSS_NOMEM            DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_VPSS_NOBUF            DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_VPSS_SIZE_NOT_ENOUGH  DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)
#define ERR_CODE_VPSS_ILLEGAL_PARAM    DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_VPSS_BUSY             DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_VPSS_BUF_EMPTY        DEFINE_ERR_CODE(MOD_ID_VPSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)

#define VPSS_INVALID_FRMRATE         -1
#define VPSS_CHN0                    0
#define VPSS_CHN1                    1
#define VPSS_CHN2                    2
#define VPSS_CHN3                    3
#define VPSS_INVALID_CHN             -1

typedef enum VPSS_NR_TYPE_E {
    VPSS_NR_TYPE_VIDEO         = 0,
    VPSS_NR_TYPE_SNAP          = 1,
    VPSS_NR_TYPE_VIDEO_SPATIAL = 2,
    VPSS_NR_TYPE_VIDEO_ENHANCE = 3,
    VPSS_NR_TYPE_BUTT
} VPSS_NR_TYPE_E;

typedef enum NR_MOTION_MODE_E {
    NR_MOTION_MODE_NORMAL          = 0,
    NR_MOTION_MODE_COMPENSATE      = 1,
    NR_MOTION_MODE_BUTT
} NR_MOTION_MODE_E;

typedef struct VPSS_NR_ATTR_S {
    VPSS_NR_TYPE_E      enNrType;
    COMPRESS_MODE_E     enCompressMode;
    NR_MOTION_MODE_E    enNrMotionMode;
} VPSS_NR_ATTR_S;

typedef struct VPSS_GRP_ATTR_S {
    GK_U32                     u32MaxW;
    GK_U32                     u32MaxH;
    PIXEL_FORMAT_E             enPixelFormat;
    DYNAMIC_RANGE_E            enDynamicRange;
    FRAME_RATE_CTRL_S          stFrameRate;
    GK_BOOL                    bNrEn;
    VPSS_NR_ATTR_S             stNrAttr;
} VPSS_GRP_ATTR_S;

typedef enum VPSS_CHN_MODE_E {
    VPSS_CHN_MODE_USER  = 0,
    VPSS_CHN_MODE_AUTO  = 1

} VPSS_CHN_MODE_E;

typedef struct VPSS_CHN_ATTR_S {
    VPSS_CHN_MODE_E     enChnMode;
    GK_U32              u32Width;
    GK_U32              u32Height;
    VIDEO_FORMAT_E      enVideoFormat;
    PIXEL_FORMAT_E      enPixelFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    COMPRESS_MODE_E     enCompressMode;
    FRAME_RATE_CTRL_S   stFrameRate;
    GK_BOOL             bMirror;
    GK_BOOL             bFlip;
    GK_U32              u32Depth;
    ASPECT_RATIO_S      stAspectRatio;
} VPSS_CHN_ATTR_S;

typedef enum VPSS_CROP_COORDINATE_E {
    VPSS_CROP_RATIO_COOR = 0,
    VPSS_CROP_ABS_COOR
} VPSS_CROP_COORDINATE_E;

typedef struct VPSS_CROP_INFO_S {
    GK_BOOL                 bEnable;
    VPSS_CROP_COORDINATE_E  enCropCoordinate;
    RECT_S                  stCropRect;
} VPSS_CROP_INFO_S;

typedef struct VPSS_LDC_ATTR_S {
    GK_BOOL     bEnable;
    LDC_ATTR_S  stAttr;
} VPSS_LDC_ATTR_S;

typedef struct VPSS_LDCV3_ATTR_S {
    GK_BOOL     bEnable;
    LDCV3_ATTR_S  stAttr;
} VPSS_LDCV3_ATTR_S;

typedef struct VPSS_ROTATION_EX_ATTR_S {
    GK_BOOL       bEnable;
    ROTATION_EX_S stRotationEx;
} VPSS_ROTATION_EX_ATTR_S;

typedef struct VPSS_LOW_DELAY_INFO_S {
    GK_BOOL bEnable;
    GK_U32 u32LineCnt;
} VPSS_LOW_DELAY_INFO_S;

typedef struct VPSS_EXT_CHN_ATTR_S {
    VPSS_CHN           s32BindChn;
    GK_U32             u32Width;
    GK_U32             u32Height;
    VIDEO_FORMAT_E     enVideoFormat;
    PIXEL_FORMAT_E     enPixelFormat;
    DYNAMIC_RANGE_E    enDynamicRange;
    COMPRESS_MODE_E    enCompressMode;
    GK_U32             u32Depth;
    FRAME_RATE_CTRL_S  stFrameRate;
} VPSS_EXT_CHN_ATTR_S;

typedef struct VPSS_GRP_SHARPEN_MANUAL_ATTR_S {
    GK_U16 au16TextureStr[VPSS_SHARPEN_GAIN_NUM];
    GK_U16 au16EdgeStr[VPSS_SHARPEN_GAIN_NUM];
    GK_U16 u16TextureFreq;
    GK_U16 u16EdgeFreq;
    GK_U8  u8OverShoot;
    GK_U8  u8UnderShoot;
    GK_U8  u8ShootSupStr;
    GK_U8  u8DetailCtrl;
} VPSS_GRP_SHARPEN_MANUAL_ATTR_S;

typedef struct VPSS_GRP_SHARPEN_AUTO_ATTR_S {
    GK_U16 au16TextureStr[VPSS_SHARPEN_GAIN_NUM][VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16EdgeStr[VPSS_SHARPEN_GAIN_NUM][VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16TextureFreq[VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U16 au16EdgeFreq[VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8OverShoot[VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8UnderShoot[VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8ShootSupStr[VPSS_AUTO_ISO_STRENGTH_NUM];
    GK_U8  au8DetailCtrl[VPSS_AUTO_ISO_STRENGTH_NUM];
} VPSS_GRP_SHARPEN_AUTO_ATTR_S;

typedef struct VPSS_GRP_SHARPEN_ATTR_S {
    GK_BOOL                         bEnable;
    OPERATION_MODE_E                enOpType;
    GK_U8                           au8LumaWgt[VPSS_YUV_SHPLUMA_NUM];
    VPSS_GRP_SHARPEN_MANUAL_ATTR_S  stSharpenManualAttr;
    VPSS_GRP_SHARPEN_AUTO_ATTR_S    stSharpenAutoAttr;
} VPSS_GRP_SHARPEN_ATTR_S;

typedef struct {
    GK_U8  IES0, IES1, IES2, IES3;
    GK_U16 IEDZ : 10, _rb_ : 6;
} tV1IEy;

typedef struct {
    GK_U8  SPN6 : 3, SFR  : 5;
    GK_U8  SBN6 : 3, PBR6 : 5;
    GK_U8  SFS2, SFT2, SBR2;
    GK_U8  SFS4, SFT4, SBR4;

    GK_U16 STH1 : 9,  SFN1 : 3, SFN0  : 3, NRyEn : 1;
    GK_U16 STH2 : 9,  SFN2 : 3, BWSF4 : 1, kMode : 3;
    GK_U16 STH3 : 9,  SFN3 : 3, tEdge : 2, TriTh : 1, _rb_  : 1;
} tV1SFy;

typedef struct {
    GK_U16 MADZ : 10, MAI0 : 2, MAI1 : 2,  MAI2 : 2;
    GK_U8  MADK,      MABR;
    GK_U16 MATH : 10, MATE : 4, MATW : 2;
    GK_U8  MASW :  4, MABW : 3, MAXN : 1, _rB_;
} tV1MDy;

typedef struct {
    GK_U16 TFS : 4,  TDZ : 10, TDX : 2;
    GK_U8  TFR[5],   TSS : 4,  TSI : 1, _rb_ : 2;
    GK_U16 SDZ : 10, STR : 5,  bRef : 1;
} tV1TFy;

typedef struct {
    GK_U8  SFC, _rb_ : 2, TFC : 6;
    GK_U16 CSFS : 10,    CSFR : 6;
    GK_U16 CTFR : 11,    CTFS : 4,    CIIR : 1;
} tV1NRc;

typedef struct {
    tV1IEy IEy[2];
    tV1SFy SFy[4];
    tV1MDy MDy[2];
    tV1TFy TFy[2];
    tV1NRc NRc;

    GK_U16 SBSk2[32], SDSk2[32];
    GK_U16 SBSk3[32], SDSk3[32];
} VPSS_NRX_V1_S;

typedef struct VPSS_NRX_PARAM_MANUAL_V1_S {
    VPSS_NRX_V1_S stNRXParam;
} VPSS_NRX_PARAM_MANUAL_V1_S;

typedef struct VPSS_NRX_PARAM_AUTO_V1_S {
    GK_U32 u32ParamNum;
    GK_U32 *pau32ISO;
    VPSS_NRX_V1_S *pastNRXParam;
} VPSS_NRX_PARAM_AUTO_V1_S;

typedef struct VPSS_NRX_PARAM_V1_S {
    OPERATION_MODE_E           enOptMode;
    VPSS_NRX_PARAM_MANUAL_V1_S stNRXManual;
    VPSS_NRX_PARAM_AUTO_V1_S   stNRXAuto;
} VPSS_NRX_PARAM_V1_S;

typedef struct {
    GK_U8  IES0, IES1, IES2, IES3;
    GK_U16 IEDZ : 10, _rb_ : 6;
} tV2_VPSS_IEy;

typedef struct {
    GK_U8  SPN6 : 3, SFR  : 5;
    GK_U8  SBN6 : 3, PBR6 : 5;

    GK_U16 SRT0 : 5, SRT1 : 5, JMODE : 3,  DeIdx : 3;
    GK_U8  DeRate, SFR6[3];

    GK_U8  SFS1,  SFT1,  SBR1;
    GK_U8  SFS2,  SFT2,  SBR2;
    GK_U8  SFS4,  SFT4,  SBR4;

    GK_U16 STH1 : 9, SFN1 : 3, SFN0  : 3, NRyEn : 1;
    GK_U16 STH2 : 9, SFN2 : 3, BWSF4 : 1, kMode : 3;
    GK_U16 STH3 : 9, SFN3 : 3, TriTh : 1, _rb0_ : 3;

    GK_U16 SBSk[32], SDSk[32];
} tV2_VPSS_SFy;

typedef struct {
    GK_U16 MADZ0   : 9,  MAI00    : 2,  MAI01  : 2,  MAI02    : 2,  biPath  : 1;
    GK_U16 MADZ1   : 9,  MAI10    : 2,  MAI11  : 2,  MAI12    : 2,  _rb0_ : 1;
    GK_U8  MABR0, MABR1;

    GK_U16 MATH0   : 10,  MATE0   : 4,  MATW   : 2;
    GK_U16 MATH1   : 10,  MATE1   : 4,  _rb1_  : 2;


    GK_U8  MASW    :  4,  _rb2_   : 4;
    GK_U8  MABW0   :  4,  MABW1   : 4;
} tV2_VPSS_MDy;

typedef struct {
    GK_U16 TFS0  :  4, TDZ0   : 10, TDX0    : 2;
    GK_U16 TFS1  :  4, TDZ1   : 10, TDX1    : 2;

    GK_U16 SDZ0  : 10, STR0   : 5,  DZMode0 : 1;
    GK_U16 SDZ1  : 10, STR1   : 5,  DZMode1 : 1;

    GK_U8  TFR0[6],    TSS0   : 4,  TSI0    : 4;
    GK_U8  TFR1[6],    TSS1   : 4,  TSI1    : 4;

    GK_U8  RFI   : 3,  tEdge  : 2,  bRef    : 1,  _rb_ : 2;
} tV2_VPSS_TFy;

typedef struct {
    GK_U16 advMATH : 1, RFDZ  : 9,    _rb_ : 6;
    GK_U8  RFUI    : 3, RFSLP : 5;
} tV2_VPSS_RFs;

typedef struct {
    tV2_VPSS_IEy  IEy;
    tV2_VPSS_SFy  SFy;
    GK_U8 NRcEn : 1, _rb_ : 7;
} tV2_VPSS_NRc;

typedef struct {
    GK_U8  SFC;
    GK_U16 CTFS : 4, TFC : 6, _rb_ : 6;

} tV2_VPSS_pNRc;

typedef struct {
    tV2_VPSS_IEy  IEy[3];
    tV2_VPSS_SFy  SFy[3];
    tV2_VPSS_MDy  MDy[2];
    tV2_VPSS_RFs  RFs;
    tV2_VPSS_TFy  TFy[2];
    tV2_VPSS_pNRc pNRc;
    tV2_VPSS_NRc  NRc;
} VPSS_NRX_V2_S;

typedef struct VPSS_NRX_PARAM_MANUAL_V2_S {
    VPSS_NRX_V2_S stNRXParam;
} VPSS_NRX_PARAM_MANUAL_V2_S;

typedef struct VPSS_NRX_PARAM_AUTO_V2_S {
    GK_U32 u32ParamNum;
    GK_U32 *pau32ISO;
    VPSS_NRX_V2_S *pastNRXParam;
} VPSS_NRX_PARAM_AUTO_V2_S;

typedef struct VPSS_NRX_PARAM_V2_S {
    OPERATION_MODE_E           enOptMode;
    VPSS_NRX_PARAM_MANUAL_V2_S stNRXManual;
    VPSS_NRX_PARAM_AUTO_V2_S   stNRXAuto;
} VPSS_NRX_PARAM_V2_S;

typedef struct {
    GK_U8  IES0, IES1, IES2, IES3;
    GK_U16  IEDZ : 10, IEEn : 1, _rb_ : 5;
} tV3_VPSS_IEy;

typedef struct {
    GK_U8  SPN6 : 3, SFR  : 5;

    GK_U8  SBN6 : 3, PBR6 : 5;

    GK_U16  SRT0 : 5, SRT1 : 5, JMODE : 3, DeIdx : 3;

    GK_U8  SFR6[4], SBR6[2], DeRate;

    GK_U8  SFS1,  SFT1,  SBR1;
    GK_U8  SFS2,  SFT2,  SBR2;
    GK_U8  SFS4,  SFT4,  SBR4;

    GK_U16  STH1 : 9,  SFN1 : 3, SFN0  : 3, NRyEn   : 1;
    GK_U16  STHd1 : 9, _rb0_ : 7;
    GK_U16  STH2 : 9,  SFN2 : 3, kMode : 3, _rb1_   : 1;
    GK_U16  STHd2 : 9, _rb2_ : 7;
    GK_U16  SBSk[32], SDSk[32];
} tV3_VPSS_SFy;

typedef struct {
    GK_U16  TFS0 : 4,   TDZ0 : 10,  TDX0    : 2;
    GK_U16  TFS1 : 4,   TDZ1 : 10,  TDX1    : 2;

    GK_U16  SDZ0 : 10,  STR0 : 5,   DZMode0 : 1;
    GK_U16  SDZ1 : 10,  STR1 : 5,   DZMode1 : 1;

    GK_U8  TSS0 : 4,   TSI0 : 4,  TFR0[6];
    GK_U8  TSS1 : 4,   TSI1 : 4,  TFR1[6];

    GK_U8  TFRS : 4,   TED  : 2,   bRef    : 1,  _rb_ : 1;
} tV3_VPSS_TFy;

typedef struct {
    GK_U16  MADZ0   : 9,   MAI00 : 2,  MAI01  : 2, MAI02 : 2, _rb0_ : 1;
    GK_U16  MADZ1   : 9,   MAI10 : 2,  MAI11  : 2, MAI12 : 2, _rb1_ : 1;
    GK_U8  MABR0, MABR1;

    GK_U16  MATH0   : 10,  MATE0 : 4,  MATW   : 2;
    GK_U16  MATHd0  : 10,  _rb2_ : 6;
    GK_U16  MATH1   : 10,  _rb3_ : 6;
    GK_U16  MATHd1  : 10,  _rb4_ : 6;
    GK_U8  MASW    :  4,  MATE1 : 4;
    GK_U8  MABW0   :  4,  MABW1 : 4;

    GK_U16  AdvMATH : 1,   AdvTH : 12, _rb5_  : 3;
} tV3_VPSS_MDy;

typedef struct {
    GK_U8  SFC, TFC : 6, _rb0_ : 2;
    GK_U8  TRC, TPC : 6, _rb1_ : 2;

    GK_BOOL MODE;
    GK_U8  PRESFC : 6, _rb2_ : 2;
} tV3_VPSS_NRc;

typedef struct {
    tV3_VPSS_IEy  IEy[5];
    tV3_VPSS_SFy  SFy[5];
    tV3_VPSS_MDy  MDy[2];
    tV3_VPSS_TFy  TFy[3];
    tV3_VPSS_NRc  NRc;
} VPSS_NRX_V3_S;

typedef struct VPSS_NRX_PARAM_MANUAL_V3_S {
    VPSS_NRX_V3_S stNRXParam;
} VPSS_NRX_PARAM_MANUAL_V3_S;

typedef struct VPSS_NRX_PARAM_AUTO_V3_S {
    GK_U32 u32ParamNum;
    GK_U32 *pau32ISO;
    VPSS_NRX_V3_S *pastNRXParam;
} VPSS_NRX_PARAM_AUTO_V3_S;

typedef struct VPSS_NRX_PARAM_V3_S {
    OPERATION_MODE_E           enOptMode;
    VPSS_NRX_PARAM_MANUAL_V3_S stNRXManual;
    VPSS_NRX_PARAM_AUTO_V3_S   stNRXAuto;
} VPSS_NRX_PARAM_V3_S;

typedef enum VPSS_NR_VER_E {
    VPSS_NR_V1 = 1,
    VPSS_NR_V2 = 2,
    VPSS_NR_V3 = 3,
    VPSS_NR_V4 = 4,
    VPSS_NR_BUTT
} VPSS_NR_VER_E;

typedef struct VPSS_GRP_NRX_PARAM_S {
    VPSS_NR_VER_E enNRVer;
    union {
        VPSS_NRX_PARAM_V1_S stNRXParam_V1;
        VPSS_NRX_PARAM_V2_S stNRXParam_V2;
        VPSS_NRX_PARAM_V3_S stNRXParam_V3;
    };

} VPSS_GRP_NRX_PARAM_S;

typedef struct VPSS_CHN_BUF_WRAP_S {
    GK_BOOL bEnable;
    GK_U32  u32BufLine;
    GK_U32  u32WrapBufferSize;
} VPSS_CHN_BUF_WRAP_S;

typedef struct VPSS_PARAM_MOD_S {
    GK_BOOL bOneBufForLowDelay;
    GK_U32  u32VpssVbSource;
    GK_U32  u32VpssSplitNodeNum;
    GK_BOOL bHdrSupport;
    GK_BOOL bNrQuickStart;
} VPSS_MOD_PARAM_S;

typedef enum VPSS_CHN_PROC_MODE_E {
    VPSS_CHN_PROC_MODE_VIDEO = 0,
    VPSS_CHN_PROC_MODE_SNAP = 1,
    VPSS_CHN_PROC_MODE_BUTT
} VPSS_CHN_PROC_MODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif


