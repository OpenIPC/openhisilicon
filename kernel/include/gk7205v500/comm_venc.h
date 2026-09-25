/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __COMM_VENC_H__
#define __COMM_VENC_H__

#include "type.h"
#include "common.h"
#include "errcode.h"
#include "comm_video.h"
#include "comm_rc.h"
#include "comm_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ERR_CODE_VENC_INVALID_CHNID DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_VENC_ILLEGAL_PARAM DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_VENC_EXIST         DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_VENC_UNEXIST       DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_VENC_NULL_PTR      DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_VENC_NOT_CONFIG    DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define ERR_CODE_VENC_NOT_SUPPORT   DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_VENC_NOT_PERM      DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_VENC_NOMEM         DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_VENC_NOBUF         DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_VENC_BUF_EMPTY     DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_VENC_BUF_FULL      DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_VENC_SYS_NOTREADY  DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_VENC_BUSY          DEFINE_ERR_CODE(MOD_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

typedef enum H264E_NALU_TYPE_E {
    H264E_NALU_BSLICE = 0,
    H264E_NALU_PSLICE = 1,
    H264E_NALU_ISLICE = 2,
    H264E_NALU_IDRSLICE = 5,
    H264E_NALU_SEI    = 6,
    H264E_NALU_SPS    = 7,
    H264E_NALU_PPS    = 8,
    H264E_NALU_BUTT
} H264E_NALU_TYPE_E;

typedef enum H265E_NALU_TYPE_E {
    H265E_NALU_BSLICE = 0,
    H265E_NALU_PSLICE = 1,
    H265E_NALU_ISLICE = 2,
    H265E_NALU_IDRSLICE = 19,
    H265E_NALU_VPS    = 32,
    H265E_NALU_SPS    = 33,
    H265E_NALU_PPS    = 34,
    H265E_NALU_SEI    = 39,

    H265E_NALU_BUTT
} H265E_NALU_TYPE_E;

typedef enum H264E_REFSLICE_TYPE_E {
    H264E_REFSLICE_FOR_1X = 1,
    H264E_REFSLICE_FOR_2X = 2,
    H264E_REFSLICE_FOR_4X = 5,
    H264E_REFSLICE_FOR_BUTT
} H264E_REFSLICE_TYPE_E;

typedef enum JPEGE_PACK_TYPE_E {
    JPEGE_PACK_ECS = 5,
    JPEGE_PACK_APP = 6,
    JPEGE_PACK_VDO = 7,
    JPEGE_PACK_PIC = 8,
    JPEGE_PACK_DCF = 9,
    JPEGE_PACK_DCF_PIC = 10,
    JPEGE_PACK_BUTT
} JPEGE_PACK_TYPE_E;

typedef enum PRORES_PACK_TYPE_E {
    PRORES_PACK_PIC = 1,
    PRORES_PACK_BUTT
} PRORES_PACK_TYPE_E;

typedef union VENC_DATA_TYPE_U {
    H264E_NALU_TYPE_E    enH264EType;
    JPEGE_PACK_TYPE_E    enJPEGEType;
    H265E_NALU_TYPE_E    enH265EType;
    PRORES_PACK_TYPE_E   enPRORESType;
} VENC_DATA_TYPE_U;

typedef struct VENC_PACK_INFO_S {
    VENC_DATA_TYPE_U  u32PackType;
    XMEDIA_U32 u32PackOffset;
    XMEDIA_U32 u32PackLength;
} VENC_PACK_INFO_S;

typedef struct VENC_PACK_S {
    XMEDIA_U64               u64PhyAddr;
    XMEDIA_U8                ATTRIBUTE* pu8Addr;
    XMEDIA_U32               ATTRIBUTE u32Len;

    XMEDIA_U64               u64PTS;
    XMEDIA_BOOL              bFrameEnd;

    VENC_DATA_TYPE_U     DataType;
    XMEDIA_U32               u32Offset;
    XMEDIA_U32               u32DataNum;
    VENC_PACK_INFO_S     stPackInfo[8];
} VENC_PACK_S;

typedef enum H264E_REF_TYPE_E {
    BASE_IDRSLICE = 0,
    BASE_PSLICE_REFTOIDR,
    BASE_PSLICE_REFBYBASE,
    BASE_PSLICE_REFBYENHANCE,
    ENHANCE_PSLICE_REFBYENHANCE,
    ENHANCE_PSLICE_NOTFORREF,
    ENHANCE_PSLICE_BUTT
} H264E_REF_TYPE_E;

typedef enum H264E_REF_TYPE_E H265E_REF_TYPE_E;

typedef struct VENC_STREAM_INFO_H264_S {
    XMEDIA_U32                 u32PicBytesNum;
    XMEDIA_U32                 u32Inter16x16MbNum;
    XMEDIA_U32                 u32Inter8x8MbNum;
    XMEDIA_U32                 u32Intra16MbNum;
    XMEDIA_U32                 u32Intra8MbNum;
    XMEDIA_U32                 u32Intra4MbNum;

    H264E_REF_TYPE_E       enRefType;
    XMEDIA_U32                 u32UpdateAttrCnt;
    XMEDIA_U32                 u32StartQp;
    XMEDIA_U32                 u32MeanQp;
    XMEDIA_BOOL                bPSkip;
} VENC_STREAM_INFO_H264_S;

typedef struct VENC_STREAM_INFO_H265_S {
    XMEDIA_U32                 u32PicBytesNum;
    XMEDIA_U32                 u32Inter64x64CuNum;
    XMEDIA_U32                 u32Inter32x32CuNum;
    XMEDIA_U32                 u32Inter16x16CuNum;
    XMEDIA_U32                 u32Inter8x8CuNum;
    XMEDIA_U32                 u32Intra32x32CuNum;
    XMEDIA_U32                 u32Intra16x16CuNum;
    XMEDIA_U32                 u32Intra8x8CuNum;
    XMEDIA_U32                 u32Intra4x4CuNum;

    H265E_REF_TYPE_E       enRefType;
    XMEDIA_U32                 u32UpdateAttrCnt;
    XMEDIA_U32                 u32StartQp;
    XMEDIA_U32                 u32MeanQp;
    XMEDIA_BOOL                bPSkip;
} VENC_STREAM_INFO_H265_S;

typedef struct VENC_SSE_INFO_S {
    XMEDIA_BOOL bSSEEn;
    XMEDIA_U32  u32SSEVal;
} VENC_SSE_INFO_S;

typedef struct VENC_STREAM_ADVANCE_INFO_H264_S {
    XMEDIA_U32             u32ResidualBitNum;
    XMEDIA_U32             u32HeadBitNum;
    XMEDIA_U32             u32MadiVal;
    XMEDIA_U32             u32MadpVal;
    XMEDIA_DOUBLE          dPSNRVal;
    XMEDIA_U32             u32MseLcuCnt;
    XMEDIA_U32             u32MseSum;
    VENC_SSE_INFO_S    stSSEInfo[8];
    XMEDIA_U32             u32QpHstgrm[VENC_QP_HISGRM_NUM];
    XMEDIA_U32             u32MoveScene16x16Num;
    XMEDIA_U32             u32MoveSceneBits;
} VENC_STREAM_ADVANCE_INFO_H264_S;

typedef struct VENC_STREAM_ADVANCE_INFO_JPEG_S {
} VENC_STREAM_ADVANCE_INFO_JPEG_S;

typedef struct VENC_STREAM_ADVANCE_INFO_PRORES_S {
} VENC_STREAM_ADVANCE_INFO_PRORES_S;

typedef struct VENC_STREAM_ADVANCE_INFO_H265_S {
    XMEDIA_U32             u32ResidualBitNum;
    XMEDIA_U32             u32HeadBitNum;
    XMEDIA_U32             u32MadiVal;
    XMEDIA_U32             u32MadpVal;
    XMEDIA_DOUBLE          dPSNRVal;
    XMEDIA_U32             u32MseLcuCnt;
    XMEDIA_U32             u32MseSum;
    VENC_SSE_INFO_S    stSSEInfo[8];
    XMEDIA_U32             u32QpHstgrm[VENC_QP_HISGRM_NUM];
    XMEDIA_U32             u32MoveScene32x32Num;
    XMEDIA_U32             u32MoveSceneBits;
} VENC_STREAM_ADVANCE_INFO_H265_S;

typedef struct VENC_STREAM_INFO_PRORES_S {
    XMEDIA_U32 u32PicBytesNum;
    XMEDIA_U32 u32UpdateAttrCnt;
} VENC_STREAM_INFO_PRORES_S;

typedef struct VENC_STREAM_INFO_JPEG_S {
    XMEDIA_U32 u32PicBytesNum;
    XMEDIA_U32 u32UpdateAttrCnt;
    XMEDIA_U32 u32Qfactor;
} VENC_STREAM_INFO_JPEG_S;

typedef struct VENC_STREAM_S {
    VENC_PACK_S ATTRIBUTE* pstPack;
    XMEDIA_U32      ATTRIBUTE u32PackCount;
    XMEDIA_U32      u32Seq;

    union {
        VENC_STREAM_INFO_H264_S   stH264Info;
        VENC_STREAM_INFO_JPEG_S   stJpegInfo;
        VENC_STREAM_INFO_H265_S   stH265Info;
        VENC_STREAM_INFO_PRORES_S stProresInfo;
    };

    union {
        VENC_STREAM_ADVANCE_INFO_H264_S   stAdvanceH264Info;
        VENC_STREAM_ADVANCE_INFO_JPEG_S   stAdvanceJpegInfo;
        VENC_STREAM_ADVANCE_INFO_H265_S   stAdvanceH265Info;
        VENC_STREAM_ADVANCE_INFO_PRORES_S stAdvanceProresInfo;
    };
} VENC_STREAM_S;

typedef struct VENC_STREAM_INFO_S {
    H265E_REF_TYPE_E enRefType;

    XMEDIA_U32  u32PicBytesNum;
    XMEDIA_U32  u32PicCnt;
    XMEDIA_U32  u32StartQp;
    XMEDIA_U32  u32MeanQp;
    XMEDIA_BOOL bPSkip;

    XMEDIA_U32  u32ResidualBitNum;
    XMEDIA_U32  u32HeadBitNum;
    XMEDIA_U32  u32MadiVal;
    XMEDIA_U32  u32MadpVal;
    XMEDIA_U32  u32MseSum;
    XMEDIA_U32  u32MseLcuCnt;
    XMEDIA_DOUBLE dPSNRVal;
} VENC_STREAM_INFO_S;

typedef struct VENC_MPF_CFG_S {
    XMEDIA_U8   u8LargeThumbNailNum;
    SIZE_S  astLargeThumbNailSize[2];
} VENC_MPF_CFG_S;

typedef enum VENC_PIC_RECEIVE_MODE_E {
    VENC_PIC_RECEIVE_SINGLE = 0,
    VENC_PIC_RECEIVE_MULTI,

    VENC_PIC_RECEIVE_BUTT
} VENC_PIC_RECEIVE_MODE_E;

typedef struct VENC_ATTR_JPEG_S {
    XMEDIA_BOOL                     bSupportDCF;
    VENC_MPF_CFG_S              stMPFCfg;
    VENC_PIC_RECEIVE_MODE_E  	enReceiveMode;
} VENC_ATTR_JPEG_S;

typedef struct VENC_ATTR_MJPEG_S {
} VENC_ATTR_MJPEG_S;

typedef struct VENC_ATTR_H264_S {
    XMEDIA_BOOL bRcnRefShareBuf;
} VENC_ATTR_H264_S;

typedef struct VENC_ATTR_H265_S {
    XMEDIA_BOOL bRcnRefShareBuf;
} VENC_ATTR_H265_S;

typedef enum PRORES_FRAMERATE {
    PRORES_FR_UNKNOWN = 0,
    PRORES_FR_23_976,
    PRORES_FR_24,
    PRORES_FR_25,
    PRORES_FR_29_97,
    PRORES_FR_30,
    PRORES_FR_50,
    PRORES_FR_59_94,
    PRORES_FR_60,
    PRORES_FR_100,
    PRORES_FR_119_88,
    PRORES_FR_120,
    PRORES_FR_BUTT
} PRORES_FRAMERATE;

typedef enum PRORES_ASPECT_RATIO {
    PRORES_ASPECT_RATIO_UNKNOWN = 0,
    PRORES_ASPECT_RATIO_SQUARE,
    PRORES_ASPECT_RATIO_4_3,
    PRORES_ASPECT_RATIO_16_9,
    PRORES_ASPECT_RATIO_BUTT
} PRORES_ASPECT_RATIO;

typedef struct VENC_ATTR_PRORES_S {
    XMEDIA_CHAR             cIdentifier[4];
    PRORES_FRAMERATE    enFrameRateCode;
    PRORES_ASPECT_RATIO enAspectRatio;
} VENC_ATTR_PRORES_S;

typedef struct VENC_ATTR_S {
    PAYLOAD_TYPE_E  enType;

    XMEDIA_U32  u32MaxPicWidth;
    XMEDIA_U32  u32MaxPicHeight;

    XMEDIA_U32  u32BufSize;
    XMEDIA_U32  u32Profile;
    XMEDIA_BOOL bByFrame;
    XMEDIA_U32  u32PicWidth;
    XMEDIA_U32  u32PicHeight;
    union {
        VENC_ATTR_H264_S stAttrH264e;
        VENC_ATTR_H265_S stAttrH265e;
        VENC_ATTR_MJPEG_S stAttrMjpege;
        VENC_ATTR_JPEG_S  stAttrJpege;
        VENC_ATTR_PRORES_S stAttrProres;
    };
} VENC_ATTR_S;

typedef enum VENC_GOP_MODE_E {
    VENC_GOPMODE_NORMALP    = 0,
    VENC_GOPMODE_DUALP      = 1,
    VENC_GOPMODE_SMARTP     = 2,
    VENC_GOPMODE_ADVSMARTP  = 3,
    VENC_GOPMODE_BIPREDB    = 4,
    VENC_GOPMODE_LOWDELAYB  = 5,

    VENC_GOPMODE_BUTT,
} VENC_GOP_MODE_E;

typedef struct VENC_GOP_NORMALP_S {
    XMEDIA_S32   s32IPQpDelta;
} VENC_GOP_NORMALP_S;

typedef struct VENC_GOP_DUALP_S {
    XMEDIA_U32 u32SPInterval;
    XMEDIA_S32 s32SPQpDelta;
    XMEDIA_S32 s32IPQpDelta;
} VENC_GOP_DUALP_S;

typedef struct VENC_GOP_SMARTP_S {
    XMEDIA_U32  u32BgInterval;
    XMEDIA_S32  s32BgQpDelta;
    XMEDIA_S32  s32ViQpDelta;
} VENC_GOP_SMARTP_S;

typedef struct VENC_GOP_ADVSMARTP_S {
    XMEDIA_U32  u32BgInterval;
    XMEDIA_S32  s32BgQpDelta;
    XMEDIA_S32  s32ViQpDelta;
} VENC_GOP_ADVSMARTP_S;

typedef struct VENC_GOP_BIPREDB_S {
    XMEDIA_U32 u32BFrmNum;
    XMEDIA_S32 s32BQpDelta;
    XMEDIA_S32 s32IPQpDelta;
} VENC_GOP_BIPREDB_S;

typedef struct VENC_GOP_ATTR_S {
    VENC_GOP_MODE_E enGopMode;
    union {
        VENC_GOP_NORMALP_S   stNormalP;
        VENC_GOP_DUALP_S     stDualP;
        VENC_GOP_SMARTP_S    stSmartP;
        VENC_GOP_ADVSMARTP_S stAdvSmartP;
        VENC_GOP_BIPREDB_S   stBipredB;
    };

} VENC_GOP_ATTR_S;

typedef struct VENC_CHN_ATTR_S {
    VENC_ATTR_S     stVencAttr;
    VENC_RC_ATTR_S  stRcAttr;
    VENC_GOP_ATTR_S stGopAttr;
} VENC_CHN_ATTR_S;

typedef struct VENC_RECV_PIC_PARAM_S {
    XMEDIA_S32 s32RecvPicNum;
} VENC_RECV_PIC_PARAM_S;

typedef struct VENC_CHN_STATUS_S {
    XMEDIA_U32 u32LeftPics;
    XMEDIA_U32 u32LeftStreamBytes;
    XMEDIA_U32 u32LeftStreamFrames;
    XMEDIA_U32 u32CurPacks;
    XMEDIA_U32 u32LeftRecvPics;
    XMEDIA_U32 u32LeftEncPics;
    XMEDIA_BOOL bJpegSnapEnd;
    VENC_STREAM_INFO_S stVencStrmInfo;
} VENC_CHN_STATUS_S;

typedef struct VENC_H264_SLICE_SPLIT_S {
    XMEDIA_BOOL bSplitEnable;
    XMEDIA_U32  u32MbLineNum;
} VENC_H264_SLICE_SPLIT_S;

typedef struct VENC_H264_INTRA_PRED_S {
    XMEDIA_U32     constrained_intra_pred_flag;
} VENC_H264_INTRA_PRED_S;

typedef struct VENC_H264_TRANS_S {
    XMEDIA_U32     u32IntraTransMode;
    XMEDIA_U32     u32InterTransMode;

    XMEDIA_BOOL    bScalingListValid;
    XMEDIA_U8      InterScalingList8X8[64];
    XMEDIA_U8      IntraScalingList8X8[64];

    XMEDIA_S32     chroma_qp_index_offset;
} VENC_H264_TRANS_S;

typedef struct VENC_H264_ENTROPY_S {
    XMEDIA_U32 u32EntropyEncModeI;
    XMEDIA_U32 u32EntropyEncModeP;
    XMEDIA_U32 u32EntropyEncModeB;
    XMEDIA_U32 cabac_init_idc;
} VENC_H264_ENTROPY_S;

typedef struct VENC_H264_POC_S {
    XMEDIA_U32 pic_order_cnt_type;

} VENC_H264_POC_S;

typedef struct VENC_H264_DBLK_S {
    XMEDIA_U32 disable_deblocking_filter_idc;
    XMEDIA_S32 slice_alpha_c0_offset_div2;
    XMEDIA_S32 slice_beta_offset_div2;
} VENC_H264_DBLK_S;

typedef struct VENC_H264_VUI_TIME_INFO_S {
    XMEDIA_U8  timing_info_present_flag;
    XMEDIA_U8  fixed_frame_rate_flag;
    XMEDIA_U32 num_units_in_tick;
    XMEDIA_U32 time_scale;
} VENC_VUI_H264_TIME_INFO_S;

typedef struct VENC_VUI_ASPECT_RATIO_S {
    XMEDIA_U8  aspect_ratio_info_present_flag;
    XMEDIA_U8  aspect_ratio_idc;
    XMEDIA_U8  overscan_info_present_flag;
    XMEDIA_U8  overscan_appropriate_flag;
    XMEDIA_U16 sar_width;
    XMEDIA_U16 sar_height ;
} VENC_VUI_ASPECT_RATIO_S;

typedef struct VENC_VUI_VIDEO_SIGNAL_S {
    XMEDIA_U8  video_signal_type_present_flag ;
    XMEDIA_U8  video_format ;
    XMEDIA_U8  video_full_range_flag;
    XMEDIA_U8  colour_description_present_flag ;
    XMEDIA_U8  colour_primaries ;
    XMEDIA_U8  transfer_characteristics;
    XMEDIA_U8  matrix_coefficients;
} VENC_VUI_VIDEO_SIGNAL_S;

typedef struct VENC_VUI_BITSTREAM_RESTRIC_S {
    XMEDIA_U8  bitstream_restriction_flag ;
} VENC_VUI_BITSTREAM_RESTRIC_S;

typedef struct VENC_H264_VUI_S {
    VENC_VUI_ASPECT_RATIO_S           stVuiAspectRatio;
    VENC_VUI_H264_TIME_INFO_S          stVuiTimeInfo;
    VENC_VUI_VIDEO_SIGNAL_S           stVuiVideoSignal;
    VENC_VUI_BITSTREAM_RESTRIC_S      stVuiBitstreamRestric;
} VENC_H264_VUI_S;

typedef struct VENC_VUI_H265_TIME_INFO_S {
    XMEDIA_U32 timing_info_present_flag;
    XMEDIA_U32 num_units_in_tick;
    XMEDIA_U32 time_scale;
    XMEDIA_U32 num_ticks_poc_diff_one_minus1;
} VENC_VUI_H265_TIME_INFO_S;

typedef struct VENC_H265_VUI_S {
    VENC_VUI_ASPECT_RATIO_S        stVuiAspectRatio;
    VENC_VUI_H265_TIME_INFO_S     stVuiTimeInfo;
    VENC_VUI_VIDEO_SIGNAL_S       stVuiVideoSignal;
    VENC_VUI_BITSTREAM_RESTRIC_S  stVuiBitstreamRestric;
} VENC_H265_VUI_S;

typedef struct VENC_JPEG_PARAM_S {
    XMEDIA_U32 u32Qfactor;
    XMEDIA_U8  u8YQt[64];
    XMEDIA_U8  u8CbQt[64];
    XMEDIA_U8  u8CrQt[64];
    XMEDIA_U32 u32MCUPerECS;
} VENC_JPEG_PARAM_S;

typedef struct VENC_MJPEG_PARAM_S {
    XMEDIA_U8 u8YQt[64];
    XMEDIA_U8 u8CbQt[64];
    XMEDIA_U8 u8CrQt[64];
    XMEDIA_U32 u32MCUPerECS;
} VENC_MJPEG_PARAM_S;

typedef struct VENC_PRORES_PARAM_S {
    XMEDIA_U8 u8LumaQt[64];
    XMEDIA_U8 u8ChromaQt[64];
    XMEDIA_CHAR encoder_identifier[4];
} VENC_PRORES_PARAM_S;

typedef struct VENC_ROI_ATTR_S {
    XMEDIA_U32  u32Index;
    XMEDIA_BOOL bEnable;
    XMEDIA_BOOL bAbsQp;
    XMEDIA_S32  s32Qp;
    RECT_S  stRect;
} VENC_ROI_ATTR_S;

typedef struct VENC_ROI_ATTR_EX_S {
    XMEDIA_U32  u32Index;
    XMEDIA_BOOL bEnable[3];
    XMEDIA_BOOL bAbsQp[3];
    XMEDIA_S32  s32Qp[3];
    RECT_S  stRect[3];
}VENC_ROI_ATTR_EX_S;

typedef struct VENC_ROIBG_FRAME_RATE_S {
    XMEDIA_S32 s32SrcFrmRate;
    XMEDIA_S32 s32DstFrmRate;
} VENC_ROIBG_FRAME_RATE_S;

typedef struct VENC_REF_PARAM_S {
    XMEDIA_U32       u32Base;
    XMEDIA_U32       u32Enhance;
    XMEDIA_BOOL      bEnablePred;
} VENC_REF_PARAM_S;

typedef enum VENC_JPEG_ENCODE_MODE_E {
    JPEG_ENCODE_ALL   = 0,
    JPEG_ENCODE_SNAP  = 1,
    JPEG_ENCODE_BUTT,
} VENC_JPEG_ENCODE_MODE_E;


typedef struct VENC_STREAM_BUF_INFO_S {
    XMEDIA_U64   u64PhyAddr[MAX_TILE_NUM];
    XMEDIA_VOID ATTRIBUTE* pUserAddr[MAX_TILE_NUM];
    XMEDIA_U64  ATTRIBUTE u64BufSize[MAX_TILE_NUM];
} VENC_STREAM_BUF_INFO_S;

typedef struct VENC_H265_SLICE_SPLIT_S {
    XMEDIA_BOOL bSplitEnable;
    XMEDIA_U32  u32LcuLineNum;
} VENC_H265_SLICE_SPLIT_S;

typedef struct VENC_H265_PU_S {
    XMEDIA_U32    constrained_intra_pred_flag;
    XMEDIA_U32    strong_intra_smoothing_enabled_flag;
} VENC_H265_PU_S;

typedef struct VENC_H265_TRANS_S {
    XMEDIA_S32  cb_qp_offset;
    XMEDIA_S32  cr_qp_offset;

    XMEDIA_BOOL bScalingListEnabled;

    XMEDIA_BOOL bScalingListTu4Valid;
    XMEDIA_U8   InterScalingList4X4[2][16];
    XMEDIA_U8   IntraScalingList4X4[2][16];

    XMEDIA_BOOL bScalingListTu8Valid;
    XMEDIA_U8   InterScalingList8X8[2][64];
    XMEDIA_U8   IntraScalingList8X8[2][64];

    XMEDIA_BOOL bScalingListTu16Valid;
    XMEDIA_U8   InterScalingList16X16[2][64];
    XMEDIA_U8   IntraScalingList16X16[2][64];

    XMEDIA_BOOL bScalingListTu32Valid;
    XMEDIA_U8   InterScalingList32X32[64];
    XMEDIA_U8   IntraScalingList32X32[64];

} VENC_H265_TRANS_S;

typedef struct VENC_H265_ENTROPY_S {
    XMEDIA_U32 cabac_init_flag;
} VENC_H265_ENTROPY_S;

typedef struct VENC_H265_DBLK_S {
    XMEDIA_U32 slice_deblocking_filter_disabled_flag;
    XMEDIA_S32 slice_beta_offset_div2;
    XMEDIA_S32 slice_tc_offset_div2;
} VENC_H265_DBLK_S;

typedef struct VENC_H265_SAO_S {
    XMEDIA_U32  slice_sao_luma_flag;
    XMEDIA_U32  slice_sao_chroma_flag;
} VENC_H265_SAO_S;

typedef enum VENC_INTRA_REFRESH_MODE_E {
    INTRA_REFRESH_ROW = 0,
    INTRA_REFRESH_COLUMN,
    INTRA_REFRESH_BUTT
} VENC_INTRA_REFRESH_MODE_E;


typedef struct VENC_INTRA_REFRESH_S {
    XMEDIA_BOOL                     bRefreshEnable;
    VENC_INTRA_REFRESH_MODE_E   enIntraRefreshMode;
    XMEDIA_U32                      u32RefreshNum;
    XMEDIA_U32                      u32ReqIQp;
} VENC_INTRA_REFRESH_S;

typedef enum VENC_MODTYPE_E {
    MODTYPE_VENC = 1,
    MODTYPE_H264E,
    MODTYPE_H265E,
    MODTYPE_JPEGE,
    MODTYPE_RC,
    MODTYPE_BUTT
} VENC_MODTYPE_E;

typedef struct VENC_MOD_H264E_S {
    XMEDIA_U32          u32OneStreamBuffer;
    XMEDIA_U32          u32H264eMiniBufMode;
    XMEDIA_U32          u32H264ePowerSaveEn;
    VB_SOURCE_E     enH264eVBSource;
    XMEDIA_BOOL         bQpHstgrmEn;
} VENC_MOD_H264E_S;

typedef struct VENC_MOD_H265E_S {
    XMEDIA_U32          u32OneStreamBuffer;
    XMEDIA_U32          u32H265eMiniBufMode;
    XMEDIA_U32          u32H265ePowerSaveEn;
    VB_SOURCE_E     enH265eVBSource;
    XMEDIA_BOOL         bQpHstgrmEn;
} VENC_MOD_H265E_S;

typedef struct VENC_MOD_JPEGE_S {
    XMEDIA_U32  u32OneStreamBuffer;
    XMEDIA_U32  u32JpegeMiniBufMode;
    XMEDIA_U32  u32JpegClearStreamBuf;
    XMEDIA_U32  u32JpegeDeringMode;
} VENC_MOD_JPEGE_S;

typedef struct VENC_MOD_RC_S {
    XMEDIA_U32  u32ClrStatAfterSetBr;
} VENC_MOD_RC_S;
typedef struct VENC_MOD_VENC_S {
    XMEDIA_U32 u32VencBufferCache;
    XMEDIA_U32 u32FrameBufRecycle;
} VENC_MOD_VENC_S;

typedef struct VENC_MODPARAM_S {
    VENC_MODTYPE_E enVencModType;
    union {
        VENC_MOD_VENC_S  stVencModParam;
        VENC_MOD_H264E_S stH264eModParam;
        VENC_MOD_H265E_S stH265eModParam;
        VENC_MOD_JPEGE_S stJpegeModParam;
        VENC_MOD_RC_S    stRcModParam;
    };
} VENC_PARAM_MOD_S;

typedef enum VENC_FRAME_TYPE_E {
    VENC_FRAME_TYPE_NONE = 1,
    VENC_FRAME_TYPE_IDR,
    VENC_FRAME_TYPE_BUTT
} VENC_FRAME_TYPE_E;

typedef struct USER_RC_INFO_S {
    XMEDIA_BOOL bQpMapValid;
    XMEDIA_BOOL bSkipWeightValid;
    XMEDIA_U32  u32BlkStartQp;
    XMEDIA_U64  u64QpMapPhyAddr;
    XMEDIA_U64  u64SkipWeightPhyAddr;
    VENC_FRAME_TYPE_E enFrameType;
} USER_RC_INFO_S;


typedef struct USER_FRAME_INFO_S {
    VIDEO_FRAME_INFO_S stUserFrame;
    USER_RC_INFO_S     stUserRcInfo;
} USER_FRAME_INFO_S;

typedef struct VENC_SSE_CFG_S {
    XMEDIA_U32  u32Index;
    XMEDIA_BOOL bEnable;
    RECT_S  stRect;
} VENC_SSE_CFG_S;


typedef struct VENC_CROP_INFO_S {
    XMEDIA_BOOL bEnable;
    RECT_S  stRect;
} VENC_CROP_INFO_S;

typedef struct VENC_FRAME_RATE_S {
    XMEDIA_S32 s32SrcFrmRate;
    XMEDIA_S32 s32DstFrmRate;
} VENC_FRAME_RATE_S;

typedef struct VENC_CHN_PARAM_S {
    XMEDIA_BOOL bColor2Grey;
    XMEDIA_U32  u32Priority;
    XMEDIA_U32  u32MaxStrmCnt;
    XMEDIA_U32  u32PollWakeUpFrmCnt;
    VENC_CROP_INFO_S    stCropCfg;
    VENC_FRAME_RATE_S   stFrameRate;
} VENC_CHN_PARAM_S;

typedef struct VENC_FOREGROUND_PROTECT_S {
    XMEDIA_BOOL bForegroundCuRcEn;
    XMEDIA_U32  u32ForegroundDirectionThresh;
    XMEDIA_U32  u32ForegroundThreshGain;
    XMEDIA_U32  u32ForegroundThreshOffset;
    XMEDIA_U32  u32ForegroundThreshP[RC_TEXTURE_THR_SIZE];
    XMEDIA_U32  u32ForegroundThreshB[RC_TEXTURE_THR_SIZE];
} VENC_FOREGROUND_PROTECT_S;

typedef enum VENC_SCENE_MODE_E {
    SCENE_0  = 0,
    SCENE_1  = 1,
    SCENE_2  = 2,
    SCENE_BUTT
} VENC_SCENE_MODE_E;


typedef struct VENC_DEBREATHEFFECT_S {
    XMEDIA_BOOL   bEnable;
    XMEDIA_S32    s32Strength0;
    XMEDIA_S32    s32Strength1;
} VENC_DEBREATHEFFECT_S;

typedef struct VENC_CU_PREDICTION_S {
     OPERATION_MODE_E enPredMode;

     XMEDIA_U32 u32Intra32Cost;
     XMEDIA_U32 u32Intra16Cost;
     XMEDIA_U32 u32Intra8Cost;
     XMEDIA_U32 u32Intra4Cost;

     XMEDIA_U32 u32Inter64Cost;
     XMEDIA_U32 u32Inter32Cost;
     XMEDIA_U32 u32Inter16Cost;
     XMEDIA_U32 u32Inter8Cost;
} VENC_CU_PREDICTION_S;

typedef struct VENC_SKIP_BIAS_S {
     XMEDIA_BOOL   bSkipBiasEn;
     XMEDIA_U32    u32SkipThreshGain;
     XMEDIA_U32    u32SkipThreshOffset;
     XMEDIA_U32    u32SkipBackgroundCost;
     XMEDIA_U32    u32SkipForegroundCost;
} VENC_SKIP_BIAS_S;

typedef struct VENC_HIERARCHICAL_QP_S {
    XMEDIA_BOOL     bHierarchicalQpEn;
	XMEDIA_S32      s32HierarchicalQpDelta[4];
    XMEDIA_S32      s32HierarchicalFrameNum[4];
} VENC_HIERARCHICAL_QP_S;

typedef struct VENC_CHN_POOL_S {
    VB_POOL hPicVbPool;
    VB_POOL hPicInfoVbPool;
} VENC_CHN_POOL_S;

typedef struct VENC_RC_ADVPARAM_S {
    XMEDIA_U32 u32ClearStatAfterSetAttr;
} VENC_RC_ADVPARAM_S;

typedef enum FG_REGION_TYPE_E {
    FG_REGION_PEOPLE = 0,
    FG_REGION_FACE,
    FG_REGION_BUTT
} FG_REGION_TYPE_E;

typedef struct {
    XMEDIA_U8 u8QpmapValueI;
    XMEDIA_U8 u8QpmapValueP;
    XMEDIA_U8 u8SkipmapValue;
} VENC_SVC_MAP_PARAM_S;

typedef struct {
    VENC_SVC_MAP_PARAM_S  stFgRegion[FG_REGION_BUTT];
    VENC_SVC_MAP_PARAM_S  stActivityRegion;
    VENC_SVC_MAP_PARAM_S  stBgRegion;
    XMEDIA_BOOL bFgProtectAdaptiveEn;
} VENC_SVC_PARAM_S;

#define VENC_MAX_SVC_RECT_NUM 32

typedef enum VENC_SVC_RECT_TYPE_E {
    SVC_RECT_TYPE_FACE,
    SVC_RECT_TYPE_PEOPLE,
    SVC_RECT_TYPE_BUTT
} VENC_SVC_RECT_TYPE_E;

typedef struct VENC_SVC_RECT_INFO_S {
    XMEDIA_U32 u32RectNum;
    SIZE_S stBaseResolution;
    RECT_S stRectAttr[VENC_MAX_SVC_RECT_NUM];
    VENC_SVC_RECT_TYPE_E enDetectType[VENC_MAX_SVC_RECT_NUM];
    XMEDIA_U64 u64Pts;
} VENC_SVC_RECT_INFO_S;

typedef struct {
    XMEDIA_U16 u16SceneComplexity;
    XMEDIA_U8  u8ObjectLevel;
} VENC_SVC_DETECT_RESULT_S;

typedef struct VENC_SEARCH_WINDOW_S
{
    OPERATION_MODE_E enMode;
    XMEDIA_U32  u32Hor;
    XMEDIA_U32  u32Ver;
}VENC_SEARCH_WINDOW_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
