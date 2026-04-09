/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * hi_comm_video.h — minimal VIDEO_FRAME_INFO_S + related enums.
 *
 * Only the subset needed by test-ivp.c and by libivp_neo's internal
 * handling of the frame descriptor. The full vendor header is several
 * thousand lines; we only carry what the test actually references.
 */
#ifndef __HI_COMM_VIDEO_H__
#define __HI_COMM_VIDEO_H__

#include "hi_type.h"
#include "hi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- Enums --- */

typedef enum hiVIDEO_FIELD_E {
    VIDEO_FIELD_TOP         = 0x01,
    VIDEO_FIELD_BOTTOM      = 0x02,
    VIDEO_FIELD_INTERLACED  = 0x03,
    VIDEO_FIELD_FRAME       = 0x04,
    VIDEO_FIELD_BUTT
} VIDEO_FIELD_E;

typedef enum hiPIXEL_FORMAT_E {
    PIXEL_FORMAT_RGB_444 = 0,
    PIXEL_FORMAT_RGB_888,
    PIXEL_FORMAT_BGR_888,
    PIXEL_FORMAT_YVU_PLANAR_420  = 0x17,
    PIXEL_FORMAT_YVU_SEMIPLANAR_420 = 0x18,
    PIXEL_FORMAT_YVU_SEMIPLANAR_422 = 0x19,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420 = 0x1a,
    PIXEL_FORMAT_BUTT
} PIXEL_FORMAT_E;

typedef enum hiVIDEO_FORMAT_E {
    VIDEO_FORMAT_LINEAR = 0,
    VIDEO_FORMAT_TILE_64x16,
    VIDEO_FORMAT_TILE_16x8,
    VIDEO_FORMAT_BUTT
} VIDEO_FORMAT_E;

typedef enum hiCOMPRESS_MODE_E {
    COMPRESS_MODE_NONE  = 0,
    COMPRESS_MODE_SEG,
    COMPRESS_MODE_TILE,
    COMPRESS_MODE_LINE,
    COMPRESS_MODE_FRAME,
    COMPRESS_MODE_BUTT
} COMPRESS_MODE_E;

typedef enum hiDYNAMIC_RANGE_E {
    DYNAMIC_RANGE_SDR8  = 0,
    DYNAMIC_RANGE_SDR10,
    DYNAMIC_RANGE_HDR10,
    DYNAMIC_RANGE_HLG,
    DYNAMIC_RANGE_SLF,
    DYNAMIC_RANGE_XDR,
    DYNAMIC_RANGE_BUTT
} DYNAMIC_RANGE_E;

typedef enum hiCOLOR_GAMUT_E {
    COLOR_GAMUT_BT601  = 0,
    COLOR_GAMUT_BT709,
    COLOR_GAMUT_BT2020,
    COLOR_GAMUT_USER,
    COLOR_GAMUT_BUTT
} COLOR_GAMUT_E;

/* --- Video supplement struct (opaque — only size matters) --- */

typedef struct hiVIDEO_SUPPLEMENT_S {
    HI_U32 u32JpegDCFPhyAddr;
    HI_U32 u32IspInfoPhyAddr;
    HI_U32 u32LowDelayPhyAddr;
    HI_U32 u32Reserved[16];
} VIDEO_SUPPLEMENT_S;

/* --- The frame struct. Layout matches vendor SDK exactly. --- */

typedef struct hiVIDEO_FRAME_S {
    HI_U32              u32Width;
    HI_U32              u32Height;
    VIDEO_FIELD_E       enField;
    PIXEL_FORMAT_E      enPixelFormat;
    VIDEO_FORMAT_E      enVideoFormat;
    COMPRESS_MODE_E     enCompressMode;
    DYNAMIC_RANGE_E     enDynamicRange;
    COLOR_GAMUT_E       enColorGamut;

    HI_U32              u32HeaderStride[3];
    HI_U32              u32Stride[3];
    HI_U32              u32ExtStride[3];

    HI_U64              u64HeaderPhyAddr[3];
    HI_U64              u64HeaderVirAddr[3];
    HI_U64              u64PhyAddr[3];
    HI_U64              u64VirAddr[3];
    HI_U64              u64ExtPhyAddr[3];
    HI_U64              u64ExtVirAddr[3];

    HI_S16              s16OffsetTop;
    HI_S16              s16OffsetBottom;
    HI_S16              s16OffsetLeft;
    HI_S16              s16OffsetRight;

    HI_U32              u32MaxLuminance;
    HI_U32              u32MinLuminance;

    HI_U32              u32TimeRef;
    HI_U64              u64PTS;

    HI_U64              u64PrivateData;
    HI_U32              u32FrameFlag;
    VIDEO_SUPPLEMENT_S  stSupplement;
} VIDEO_FRAME_S;

typedef struct hiVIDEO_FRAME_INFO_S {
    VIDEO_FRAME_S stVFrame;
    HI_U32        u32PoolId;
    MOD_ID_E      enModId;
} VIDEO_FRAME_INFO_S;

#ifdef __cplusplus
}
#endif

#endif /* __HI_COMM_VIDEO_H__ */
