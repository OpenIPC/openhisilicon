/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_BLOCK_CONFIG_H__
#define __ISP_BLOCK_CONFIG_H__

#include "type.h"
#include "common.h"
#include "comm_video.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_RECT_S {
    GK_S32 s32X;
    GK_S32 s32Y;
    GK_U32 u32Width;
    GK_U32 u32Height;
} ISP_RECT_S;

typedef enum ISP_RUNNING_MODE_E {
    ISP_MODE_RUNNING_OFFLINE = 0,
    ISP_MODE_RUNNING_ONLINE,
    ISP_MODE_RUNNING_SIDEBYSIDE,
    ISP_MODE_RUNNING_STRIPING,

    ISP_MODE_RUNNING_BUTT,
} ISP_RUNNING_MODE_E;

typedef enum ISP_DATAINPUT_MODE_E {
    ISP_MODE_RAW = 0,
    ISP_MODE_BT1120_YUV,
    ISP_MODE_DATAINPUT_BUTT,
} ISP_DATAINPUT_MODE_E;

typedef struct ISP_WORKING_MODE_S {
    GK_U8                u8PreBlockNum;
    GK_U8                u8BlockNum;
    GK_U32               u32OverLap;
    SIZE_S               stFrameRect;  /* BE Full Frame  information */
    RECT_S               astBlockRect[ISP_STRIPING_MAX_NUM];
    ISP_RUNNING_MODE_E   enIspRunningMode;
    ISP_DATAINPUT_MODE_E enIspDataInputMode;
} ISP_WORKING_MODE_S;

typedef struct ISP_BLOCK_S {
    GK_U8   u8BlockNum;
    GK_U8   u8PreBlockNum;
    GK_U32  u32OverLap;
    SIZE_S  stFrameRect;  /* BE Full Frame  information */
    RECT_S  astBlockRect[ISP_STRIPING_MAX_NUM];
    ISP_RUNNING_MODE_E enIspRunningMode;
} ISP_BLOCK_ATTR_S;

GK_S32 ISP_BlockInit(VI_PIPE ViPipe, ISP_BLOCK_ATTR_S *pstBlock);
GK_S32 ISP_BlockExit(VI_PIPE ViPipe);
GK_S32 ISP_BlockUpdate(VI_PIPE ViPipe, ISP_BLOCK_ATTR_S *pstBlock);
GK_S32 ISP_GetWorkMode(VI_PIPE ViPipe, ISP_WORKING_MODE_S *pstIspWorkMode);
GK_U32 ISP_GetBlockRect(ISP_RECT_S *pstBlockRect, ISP_BLOCK_ATTR_S *pstBlock, GK_U8 u8BlockId);

#define ISP_CHECK_RUNNING_MODE(eIspRunningMode)                   \
    do {                                                          \
        if (eIspRunningMode >= ISP_MODE_RUNNING_BUTT) {           \
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Isp Running mode!\n"); \
            return ERR_CODE_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define ISP_CHECK_BLOCK_ID(Blk)                                   \
    do {                                                          \
        if (((Blk) >= ISP_STRIPING_MAX_NUM)) {                    \
            ISP_TRACE(MODULE_DBG_ERR, "Err isp block Id %d!\n", Blk); \
            return ERR_CODE_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define ISP_CHECK_BLOCK_NUM(Blk)                                   \
    do {                                                           \
        if (((Blk) > ISP_STRIPING_MAX_NUM) || ((Blk) < 1)) {       \
            ISP_TRACE(MODULE_DBG_ERR, "Err isp block Num %d!\n", Blk); \
            return ERR_CODE_ISP_ILLEGAL_PARAM;                       \
        }                                                          \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
