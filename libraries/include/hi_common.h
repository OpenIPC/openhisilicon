/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * hi_common.h — common HiSilicon SDK definitions used by IVE.
 * Minimal self-contained version for libive_neo.
 */
#ifndef __HI_COMMON_H__
#define __HI_COMMON_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Module IDs — only IVE is needed by our code; others are retained
 * as enum slots so values of other modules (if referenced) don't clash.
 */
typedef enum hiMOD_ID_E {
    HI_ID_CMPI     = 0,
    HI_ID_VB       = 1,
    HI_ID_SYS      = 2,
    HI_ID_RGN      = 3,
    HI_ID_CHNL     = 4,
    HI_ID_VDEC     = 5,
    HI_ID_AVS      = 6,
    HI_ID_VPSS     = 7,
    HI_ID_VENC     = 8,
    HI_ID_SVP      = 9,
    HI_ID_H264E    = 10,
    HI_ID_JPEGE    = 11,
    HI_ID_MPEG4E   = 12,
    HI_ID_H265E    = 13,
    HI_ID_JPEGD    = 14,
    HI_ID_VO       = 15,
    HI_ID_VI       = 16,
    HI_ID_DIS      = 17,
    HI_ID_VALG     = 18,
    HI_ID_RC       = 19,
    HI_ID_AIO      = 20,
    HI_ID_AI       = 21,
    HI_ID_AO       = 22,
    HI_ID_AENC     = 23,
    HI_ID_ADEC     = 24,
    HI_ID_VPU      = 25,
    HI_ID_PCIV     = 26,
    HI_ID_FMD      = 27,
    HI_ID_HDMI     = 28,
    HI_ID_VOIE     = 29,
    HI_ID_TDE      = 30,
    HI_ID_HDR      = 31,
    HI_ID_PRORES   = 32,
    HI_ID_VFMW     = 33,
    HI_ID_IVE      = 34,
    HI_ID_USER     = 35,
    HI_ID_DCCM     = 36,
    HI_ID_DCCS     = 37,
    HI_ID_GMINFO   = 38,
    HI_ID_BUTT,
} MOD_ID_E;

/* Error-level encoding (matches HiSilicon SDK convention) */
typedef enum {
    EN_ERR_LEVEL_DEBUG = 0,
    EN_ERR_LEVEL_INFO,
    EN_ERR_LEVEL_NOTICE,
    EN_ERR_LEVEL_WARNING,
    EN_ERR_LEVEL_ERROR,
    EN_ERR_LEVEL_CRIT,
    EN_ERR_LEVEL_ALERT,
    EN_ERR_LEVEL_FATAL,
    EN_ERR_LEVEL_BUTT
} ERR_LEVEL_E;

#define HI_ERR_APPID        (0x80000000L + 0x20000000L)

#define HI_DEF_ERR(module, level, errid) \
    ((HI_S32)(HI_ERR_APPID | ((module) << 16) | ((level) << 13) | (errid)))

/* Common low-level error codes (bits 0..12 of error word) */
#define EN_ERR_INVALID_DEVID    1
#define EN_ERR_INVALID_CHNID    2
#define EN_ERR_ILLEGAL_PARAM    3
#define EN_ERR_EXIST            4
#define EN_ERR_UNEXIST          5
#define EN_ERR_NULL_PTR         6
#define EN_ERR_NOT_CONFIG       7
#define EN_ERR_NOT_SUPPORT      8
#define EN_ERR_NOT_PERM         9
#define EN_ERR_NOMEM            12
#define EN_ERR_NOBUF            13
#define EN_ERR_BUF_EMPTY        14
#define EN_ERR_BUF_FULL         15
#define EN_ERR_SYS_NOTREADY     16
#define EN_ERR_BADADDR          17
#define EN_ERR_BUSY             18

#ifdef __cplusplus
}
#endif

#endif /* __HI_COMMON_H__ */
