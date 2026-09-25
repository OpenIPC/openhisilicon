/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __COMM_RC_H__
#define __COMM_RC_H__

#include "defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef XMEDIA_U32 XMEDIA_FR32;

typedef enum VENC_RC_MODE_E {
    VENC_RC_MODE_H264CBR = 1,
    VENC_RC_MODE_H264VBR,
    VENC_RC_MODE_H264AVBR,
    VENC_RC_MODE_H264QVBR,
    VENC_RC_MODE_H264CVBR,
    VENC_RC_MODE_H264FIXQP,
    VENC_RC_MODE_H264QPMAP,

    VENC_RC_MODE_MJPEGCBR,
    VENC_RC_MODE_MJPEGVBR,
    VENC_RC_MODE_MJPEGFIXQP,

    VENC_RC_MODE_H265CBR,
    VENC_RC_MODE_H265VBR,
    VENC_RC_MODE_H265AVBR,
    VENC_RC_MODE_H265QVBR,
    VENC_RC_MODE_H265CVBR,
    VENC_RC_MODE_H265FIXQP,
    VENC_RC_MODE_H265QPMAP,

    VENC_RC_MODE_BUTT,
} VENC_RC_MODE_E;

typedef enum VENC_RC_QPMAP_MODE_E {
    VENC_RC_QPMAP_MODE_MEANQP = 0,
    VENC_RC_QPMAP_MODE_MINQP,
    VENC_RC_QPMAP_MODE_MAXQP,

    VENC_RC_QPMAP_MODE_BUTT,
} VENC_RC_QPMAP_MODE_E;

typedef struct VENC_H264_FIXQP_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32IQp;
    XMEDIA_U32      u32PQp;
    XMEDIA_U32      u32BQp;
} VENC_H264_FIXQP_S;

typedef struct VENC_H264_CBR_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32BitRate;
} VENC_H264_CBR_S;

typedef struct VENC_H264_VBR_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32MaxBitRate;
} VENC_H264_VBR_S;

typedef struct VENC_H264_CVBR_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32MaxBitRate;

    XMEDIA_U32      u32ShortTermStatTime;
    XMEDIA_U32      u32LongTermStatTime;
    XMEDIA_U32      u32LongTermMaxBitrate ;
    XMEDIA_U32      u32LongTermMinBitrate ;
} VENC_H264_CVBR_S;

typedef struct VENC_H264_AVBR_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32MaxBitRate;
} VENC_H264_AVBR_S;

typedef struct VENC_H264_QPMAP_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
} VENC_H264_QPMAP_S;

typedef struct VENC_H264_QVBR_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32TargetBitRate;
}VENC_H264_QVBR_S;

typedef struct VENC_H265_QPMAP_S {
    XMEDIA_U32      u32Gop;
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    VENC_RC_QPMAP_MODE_E enQpMapMode;
} VENC_H265_QPMAP_S;

typedef struct VENC_H264_CBR_S   VENC_H265_CBR_S;
typedef struct VENC_H264_VBR_S   VENC_H265_VBR_S;
typedef struct VENC_H264_AVBR_S  VENC_H265_AVBR_S;
typedef struct VENC_H264_FIXQP_S VENC_H265_FIXQP_S;
typedef struct VENC_H264_QVBR_S  VENC_H265_QVBR_S;
typedef struct VENC_H264_CVBR_S  VENC_H265_CVBR_S;

typedef struct VENC_MJPEG_FIXQP_S {
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate;
    XMEDIA_U32      u32Qfactor;
} VENC_MJPEG_FIXQP_S;

typedef struct VENC_MJPEG_CBR_S {
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate ;
    XMEDIA_U32      u32BitRate;
} VENC_MJPEG_CBR_S;

typedef struct VENC_MJPEG_VBR_S {
    XMEDIA_U32      u32StatTime;
    XMEDIA_U32      u32SrcFrameRate;
    XMEDIA_FR32     fr32DstFrameRate;
    XMEDIA_U32      u32MaxBitRate;
}VENC_MJPEG_VBR_S;

typedef struct VENC_RC_ATTR_S {
    VENC_RC_MODE_E enRcMode;
    union {
        VENC_H264_CBR_S    stH264Cbr;
        VENC_H264_VBR_S    stH264Vbr;
        VENC_H264_AVBR_S   stH264AVbr;
        VENC_H264_QVBR_S   stH264QVbr;
        VENC_H264_CVBR_S   stH264CVbr;
        VENC_H264_FIXQP_S  stH264FixQp;
        VENC_H264_QPMAP_S  stH264QpMap;

        VENC_MJPEG_CBR_S   stMjpegCbr;
        VENC_MJPEG_VBR_S   stMjpegVbr;
        VENC_MJPEG_FIXQP_S stMjpegFixQp;

        VENC_H265_CBR_S    stH265Cbr;
        VENC_H265_VBR_S    stH265Vbr;
        VENC_H265_AVBR_S   stH265AVbr;
        VENC_H265_QVBR_S   stH265QVbr;
        VENC_H265_CVBR_S   stH265CVbr;
        VENC_H265_FIXQP_S  stH265FixQp;
        VENC_H265_QPMAP_S  stH265QpMap;
    };
} VENC_RC_ATTR_S;

typedef enum RC_SUPERFRM_MODE_E {
    SUPERFRM_NONE=0,
    SUPERFRM_DISCARD,
    SUPERFRM_REENCODE,
    SUPERFRM_BUTT
} VENC_SUPERFRM_MODE_E;

typedef struct VENC_PARAM_H264_CBR_S {
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;
} VENC_PARAM_H264_CBR_S;

typedef struct VENC_PARAM_H264_VBR_S {
    XMEDIA_S32  s32ChangePos;
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;
} VENC_PARAM_H264_VBR_S;

typedef struct VENC_PARAM_H264_AVBR_S {
    XMEDIA_S32  s32ChangePos;
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;

    XMEDIA_S32  s32MinStillPercent;
    XMEDIA_U32  u32MaxStillQP;
    XMEDIA_U32  u32MinStillPSNR;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;
    XMEDIA_U32  u32MinQpDelta;
    XMEDIA_U32  u32MotionSensitivity;
} VENC_PARAM_H264_AVBR_S;

typedef struct VENC_PARAM_H264_QVBR_S {
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;

    XMEDIA_S32  s32BitPercentUL;
    XMEDIA_S32  s32BitPercentLL;
    XMEDIA_S32  s32PsnrFluctuateUL;
    XMEDIA_S32  s32PsnrFluctuateLL;
} VENC_PARAM_H264_QVBR_S;

typedef struct VENC_PARAM_H264_CVBR_S {
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;

    XMEDIA_U32  u32MinQpDelta;
    XMEDIA_U32  u32MaxQpDelta;

    XMEDIA_U32  u32ExtraBitPercent;
    XMEDIA_U32  u32LongTermStatTimeUnit;
} VENC_PARAM_H264_CVBR_S;

typedef struct VENC_PARAM_MJPEG_CBR_S {
    XMEDIA_U32  u32MaxQfactor;
    XMEDIA_U32  u32MinQfactor;
} VENC_PARAM_MJPEG_CBR_S;

typedef struct VENC_PARAM_MJPEG_VBR_S {
    XMEDIA_S32 s32ChangePos;
    XMEDIA_U32 u32MaxQfactor;
    XMEDIA_U32 u32MinQfactor;
} VENC_PARAM_MJPEG_VBR_S;

typedef struct VENC_PARAM_H265_CBR_S {
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;
    VENC_RC_QPMAP_MODE_E enQpMapMode;
} VENC_PARAM_H265_CBR_S;

typedef struct VENC_PARAM_H265_VBR_S {
    XMEDIA_S32  s32ChangePos;
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;

    XMEDIA_BOOL bQpMapEn;
    VENC_RC_QPMAP_MODE_E enQpMapMode;
} VENC_PARAM_H265_VBR_S;

typedef struct VENC_PARAM_H265_AVBR_S {
    XMEDIA_S32  s32ChangePos;
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;

    XMEDIA_S32  s32MinStillPercent;
    XMEDIA_U32  u32MaxStillQP;
    XMEDIA_U32  u32MinStillPSNR;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;

    XMEDIA_U32  u32MinQpDelta;
    XMEDIA_U32  u32MotionSensitivity;

    XMEDIA_BOOL bQpMapEn;
    VENC_RC_QPMAP_MODE_E enQpMapMode;
} VENC_PARAM_H265_AVBR_S;

typedef struct VENC_PARAM_H265_QVBR_S {
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;

    XMEDIA_BOOL bQpMapEn;
    VENC_RC_QPMAP_MODE_E enQpMapMode;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;

    XMEDIA_S32  s32BitPercentUL;
    XMEDIA_S32  s32BitPercentLL;
    XMEDIA_S32  s32PsnrFluctuateUL;
    XMEDIA_S32  s32PsnrFluctuateLL;
} VENC_PARAM_H265_QVBR_S;

typedef struct VENC_PARAM_H265_CVBR_S {
    XMEDIA_U32  u32MinIprop;
    XMEDIA_U32  u32MaxIprop;
    XMEDIA_S32  s32MaxReEncodeTimes;
    XMEDIA_BOOL bQpMapEn;
    VENC_RC_QPMAP_MODE_E enQpMapMode;

    XMEDIA_U32  u32MaxQp;
    XMEDIA_U32  u32MinQp;
    XMEDIA_U32  u32MaxIQp;
    XMEDIA_U32  u32MinIQp;

    XMEDIA_U32  u32MinQpDelta;
    XMEDIA_U32  u32MaxQpDelta;

    XMEDIA_U32  u32ExtraBitPercent;
    XMEDIA_U32  u32LongTermStatTimeUnit;
} VENC_PARAM_H265_CVBR_S;

typedef struct VENC_SCENE_CHANGE_DETECT_S {
    XMEDIA_BOOL bDetectSceneChange;
    XMEDIA_BOOL bAdaptiveInsertIDRFrame;
} VENC_SCENE_CHANGE_DETECT_S;

typedef struct VENC_RC_PARAM_S {
    XMEDIA_U32 u32ThrdI[RC_TEXTURE_THR_SIZE];
    XMEDIA_U32 u32ThrdP[RC_TEXTURE_THR_SIZE];
    XMEDIA_U32 u32ThrdB[RC_TEXTURE_THR_SIZE];
    XMEDIA_U32 u32DirectionThrd;
    XMEDIA_U32 u32RowQpDelta;
    XMEDIA_S32 s32FirstFrameStartQp;
    VENC_SCENE_CHANGE_DETECT_S stSceneChangeDetect;
    union {
        VENC_PARAM_H264_CBR_S     stParamH264Cbr;
        VENC_PARAM_H264_VBR_S     stParamH264Vbr;
        VENC_PARAM_H264_AVBR_S    stParamH264AVbr;
        VENC_PARAM_H264_QVBR_S    stParamH264QVbr;
        VENC_PARAM_H264_CVBR_S    stParamH264CVbr;
        VENC_PARAM_H265_CBR_S     stParamH265Cbr;
        VENC_PARAM_H265_VBR_S     stParamH265Vbr;
        VENC_PARAM_H265_AVBR_S    stParamH265AVbr;
        VENC_PARAM_H265_QVBR_S    stParamH265QVbr;
        VENC_PARAM_H265_CVBR_S    stParamH265CVbr;
        VENC_PARAM_MJPEG_CBR_S    stParamMjpegCbr;
        VENC_PARAM_MJPEG_VBR_S    stParamMjpegVbr;
    };
} VENC_RC_PARAM_S;

typedef enum VENC_FRAMELOST_MODE_E {
    FRMLOST_NORMAL=0,
    FRMLOST_PSKIP,
    FRMLOST_BUTT,
} VENC_FRAMELOST_MODE_E;

typedef struct VENC_FRAMELOST_S {
    XMEDIA_BOOL                bFrmLostOpen;
    XMEDIA_U32                 u32FrmLostBpsThr;
    VENC_FRAMELOST_MODE_E  enFrmLostMode;
    XMEDIA_U32                 u32EncFrmGaps;
} VENC_FRAMELOST_S;

typedef enum VENC_RC_PRIORITY_E {
    VENC_RC_PRIORITY_BITRATE_FIRST = 1,
    VENC_RC_PRIORITY_FRAMEBITS_FIRST,
    VENC_RC_PRIORITY_BUTT,
} VENC_RC_PRIORITY_E;

typedef struct VENC_SUPERFRAME_CFG_S {
    VENC_SUPERFRM_MODE_E  enSuperFrmMode;
    XMEDIA_U32                u32SuperIFrmBitsThr;
    XMEDIA_U32                u32SuperPFrmBitsThr;
    XMEDIA_U32                u32SuperBFrmBitsThr;
    VENC_RC_PRIORITY_E    enRcPriority;
} VENC_SUPERFRAME_CFG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
