/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef COMMON_H
#define COMMON_H

#include "xmedia_type.h"
#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ATTRIBUTE __attribute__((aligned (ALIGN_NUM)))

#define COMPAT_POINTER(ptr, type)\
    do {\
        xmedia_ulong ulAddr   =  (xmedia_ulong)ptr;\
        xmedia_u32 u32Addr =  (xmedia_u32)ulAddr;\
        ptr = (type)(xmedia_ulong)u32Addr;\
    } while(0)

#define XMEDIA_INVALID_CHN (-1)
#define XMEDIA_INVALID_WAY (-1)
#define XMEDIA_INVALID_LAYER (-1)
#define XMEDIA_INVALID_DEV (-1)
#define XMEDIA_INVALID_VALUE (-1)
#define XMEDIA_INVALID_TYPE (-1)

#define CCM_MATRIX_SIZE             (9)
#define CCM_MATRIX_NUM              (7)

#define CLUT_R_NUM            (17)
#define CLUT_B_NUM            (17)
#define CLUT_G_NUM            (17)
#define ISP_CLUT_COEFACMCNT (4850)
#define CUBIC_POINT_MAX  (5)

typedef enum {
    MOD_ID_VB      = 0,
    MOD_ID_SYS     = 1,
    MOD_ID_LOG     = 2,

    MOD_ID_RGN     = 3,
    MOD_ID_CHNL    = 4,
    MOD_ID_VI      = 5,
    MOD_ID_VO      = 6,
    MOD_ID_DIS     = 7,
    MOD_ID_AVS     = 8,
    MOD_ID_VPSS    = 9,
    MOD_ID_ISP     = 10,
    MOD_ID_FB      = 11,
    MOD_ID_GDC     = 12,
    MOD_ID_TDE     = 13,
    MOD_ID_VGS     = 14,
    MOD_ID_MCF     = 15,
    MOD_ID_HDMI    = 16,

    MOD_ID_VDEC    = 17,
    MOD_ID_VENC    = 18,
    MOD_ID_VPU     = 19,
    MOD_ID_VALG    = 20,
    MOD_ID_RC      = 21,
    MOD_ID_JPEGE   = 22,
    MOD_ID_JPEGD   = 23,
    MOD_ID_H264E   = 24,
    MOD_ID_H264D   = 25,
    MOD_ID_H265E   = 26,

    MOD_ID_AIO     = 29,
    MOD_ID_AI      = 30,
    MOD_ID_AO      = 31,
    MOD_ID_AENC    = 32,
    MOD_ID_ADEC    = 33,
    MOD_ID_ACOMM   = 34,

    MOD_ID_NPU     = 35,
    MOD_ID_IVE     = 36,
    MOD_ID_USER    = 37,
    MOD_ID_PM      = 38,
    MOD_ID_GYRODIS = 39,
    MOD_ID_IR      = 40,
    MOD_ID_NPU_SVP = 41,
    MOD_ID_DDR_OST_QOS = 42,
    MOD_ID_CIPHER  = 43,
    MOD_ID_AE      = 45,
    MOD_ID_AWB     = 46,
    MOD_ID_AF      = 47,
    MOD_ID_MISC    = 48,

    MOD_ID_MAX
} xmedia_mod_id;

#define MPP_MOD_VB        "vb"
#define MPP_MOD_SYS       "sys"
#define MPP_MOD_LOG       "logmpp"

#define MPP_MOD_RGN       "rgn"
#define MPP_MOD_CHNL      "chnl"
#define MPP_MOD_VI        "vi"
#define MPP_MOD_VO        "vo"
#define MPP_MOD_DIS       "dis"
#define MPP_MOD_AVS       "avs"
#define MPP_MOD_VPSS      "vpss"
#define MPP_MOD_ISP       "isp"
#define MPP_MOD_FB        "fb"
#define MPP_MOD_GDC       "gdc"
#define MPP_MOD_TDE       "tde"
#define MPP_MOD_VGS       "vgs"
#define MPP_MOD_MCF       "mcf"
#define MPP_MOD_HDMI      "hdmi"

#define MPP_MOD_VENC      "venc"
#define MPP_MOD_VDEC      "vdec"
#define MPP_MOD_VPU       "vpu"
#define MPP_MOD_VALG      "valg"
#define MPP_MOD_RC        "rc"
#define MPP_MOD_JPEGE     "jpege"
#define MPP_MOD_JPEGD     "jpegd"
#define MPP_MOD_H264E     "h264e"
#define MPP_MOD_H264D     "h264d"
#define MPP_MOD_H265E     "h265e"

#define MPP_MOD_AIO       "aio"
#define MPP_MOD_AI        "ai"
#define MPP_MOD_AO        "ao"
#define MPP_MOD_AENC      "aenc"
#define MPP_MOD_ADEC      "adec"
#define MPP_MOD_ACOMM     "acomm"

#define MPP_MOD_NPU       "npu"
#define MPP_MOD_IVE       "ive"
#define MPP_MOD_MD        "md"

#define MPP_MOD_USR       "usr"
#define MPP_MOD_PM        "pm"
#define MPP_MOD_GYRODIS   "gyrodis"
#define MPP_MOD_IR        "ir"
#define MPP_MOD_NPU_SVP   "svp"
#define MPP_MOD_DDR_OST_QOS   "ddr_ost_qos"
#define MPP_MOD_CIPHER    "cipher"
#define MPP_MOD_OTP       "otp"
#define MPP_MOD_AE        "ae"
#define MPP_MOD_AWB       "awb"
#define MPP_MOD_AF        "af"
#define MPP_MOD_MISC      "misc"

typedef enum {
    PT_PCMU          = 0,
    PT_1016          = 1,
    PT_G721          = 2,
    PT_GSM           = 3,
    PT_G723          = 4,
    PT_DVI4_8K       = 5,
    PT_DVI4_16K      = 6,
    PT_LPC           = 7,
    PT_PCMA          = 8,
    PT_G722          = 9,
    PT_S16BE_STEREO  = 10,
    PT_S16BE_MONO    = 11,
    PT_QCELP         = 12,
    PT_CN            = 13,
    PT_MPEGAUDIO     = 14,
    PT_G728          = 15,
    PT_DVI4_3        = 16,
    PT_DVI4_4        = 17,
    PT_G729          = 18,
    PT_G711A         = 19,
    PT_G711U         = 20,
    PT_G726          = 21,
    PT_G729A         = 22,
    PT_LPCM          = 23,
    PT_CelB          = 25,
    PT_JPEG          = 26,
    PT_CUSM          = 27,
    PT_NV            = 28,
    PT_PICW          = 29,
    PT_CPV           = 30,
    PT_H261          = 31,
    PT_MPEGVIDEO     = 32,
    PT_MPEG2TS       = 33,
    PT_H263          = 34,
    PT_SPEG          = 35,
    PT_MPEG2VIDEO    = 36,
    PT_AAC           = 37,
    PT_WMA9STD       = 38,
    PT_HEAAC         = 39,
    PT_PCM_VOICE     = 40,
    PT_PCM_AUDIO     = 41,
    PT_MP3           = 43,
    PT_ADPCMA        = 49,
    PT_AEC           = 50,
    PT_X_LD          = 95,
    PT_H264          = 96,
    PT_D_GSM_HR      = 200,
    PT_D_GSM_EFR     = 201,
    PT_D_L8          = 202,
    PT_D_RED         = 203,
    PT_D_VDVI        = 204,
    PT_D_BT656       = 220,
    PT_D_H263_1998   = 221,
    PT_D_MP1S        = 222,
    PT_D_MP2P        = 223,
    PT_D_BMPEG       = 224,
    PT_MP4VIDEO      = 230,
    PT_MP4AUDIO      = 237,
    PT_VC1           = 238,
    PT_JVC_ASF       = 255,
    PT_D_AVI         = 256,
    PT_DIVX3         = 257,
    PT_AVS             = 258,
    PT_REAL8         = 259,
    PT_REAL9         = 260,
    PT_VP6             = 261,
    PT_VP6F             = 262,
    PT_VP6A             = 263,
    PT_SORENSON          = 264,
    PT_H265          = 265,
    PT_VP8             = 266,
    PT_MVC             = 267,
    PT_PNG           = 268,
    PT_MJPEG         = 270,
    PT_AMR           = 1001,
    PT_AMRWB         = 1003,
    PT_PRORES        = 1006,
    PT_OPUS          = 1007,
    PT_MAX
} xmedia_payload_type;

#ifdef __cplusplus
}
#endif

#endif

