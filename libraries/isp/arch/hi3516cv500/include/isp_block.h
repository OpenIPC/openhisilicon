/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#ifndef __ISP_BLOCK_CONFIG_H__
#define __ISP_BLOCK_CONFIG_H__

#include "hi_type.h"
#include "common.h"
#include "comm_video.h"
#include "comm_isp.h"

/* Compat: hi_ types used in this header */
typedef SIZE_S hi_size;
typedef RECT_S hi_rect;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_s32 x;
    hi_s32 y;
    hi_u32 width;
    hi_u32 height;
} isp_rect;

typedef enum {
    ISP_MODE_RUNNING_OFFLINE = 0,
    ISP_MODE_RUNNING_ONLINE,
    ISP_MODE_RUNNING_SIDEBYSIDE,
    ISP_MODE_RUNNING_STRIPING,

    ISP_MODE_RUNNING_BUTT,
} isp_running_mode;

typedef enum {
    ISP_MODE_RAW = 0,
    ISP_MODE_BT1120_YUV,
    ISP_MODE_DATAINPUT_BUTT,
} isp_data_input_mode;

typedef struct {
    hi_u8                pre_block_num;
    hi_u8                block_num;
    hi_u32               over_lap;
    hi_size              frame_rect;  /* BE full frame  information */
    hi_rect              block_rect[ISP_STRIPING_MAX_NUM];
    isp_running_mode     running_mode;
    isp_data_input_mode  data_input_mode;
} isp_working_mode;

typedef struct {
    hi_u8   block_num;
    hi_u8   pre_block_num;
    hi_u32  over_lap;
    hi_size frame_rect;  /* BE full frame  information */
    hi_rect block_rect[ISP_STRIPING_MAX_NUM];
    isp_running_mode running_mode;
} isp_block_attr;

HI_S32 ISP_BlockInit(VI_PIPE ViPipe, isp_block_attr *pstBlock);
HI_S32 ISP_BlockExit(VI_PIPE ViPipe);
HI_S32 ISP_BlockUpdate(VI_PIPE ViPipe, isp_block_attr *pstBlock);
HI_S32 ISP_GetWorkMode(VI_PIPE ViPipe, isp_working_mode *pstIspWorkMode);
HI_U32 ISP_GetBlockRect(isp_rect *pstBlockRect, isp_block_attr *pstBlock, HI_U8 u8BlockId);

#define ISP_CHECK_RUNNING_MODE(eIspRunningMode)                   \
    do {                                                          \
        if (eIspRunningMode >= ISP_MODE_RUNNING_BUTT) {           \
            ISP_ERR_TRACE("Invalid Isp Running mode!\n"); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define ISP_CHECK_BLOCK_ID(Blk)                                   \
    do {                                                          \
        if (((Blk) >= ISP_STRIPING_MAX_NUM)) {                    \
            ISP_ERR_TRACE("Err isp block Id %d!\n", Blk); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define ISP_CHECK_BLOCK_NUM(Blk)                                   \
    do {                                                           \
        if (((Blk) > ISP_STRIPING_MAX_NUM) || ((Blk) < 1)) {       \
            ISP_ERR_TRACE("Err isp block Num %d!\n", Blk); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                       \
        }                                                          \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
