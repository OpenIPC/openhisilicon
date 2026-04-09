/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_block.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_BLOCK_CONFIG_H__
#define __ISP_BLOCK_CONFIG_H__

#include "hi_type.h"
#include "hi_common_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIN_SPLIT_NUM  2

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

hi_s32 isp_block_init(hi_vi_pipe vi_pipe, isp_block_attr *block);
hi_void isp_block_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_block_update(hi_vi_pipe vi_pipe, isp_block_attr *block);
hi_s32 isp_get_work_mode(hi_vi_pipe vi_pipe, isp_working_mode *isp_work_mode);
hi_u32 isp_get_block_rect(isp_rect *block_rect, isp_block_attr *block, hi_u8 block_id);

#define isp_check_running_mode_return(isp_running_mode)                   \
    do {                                                          \
        if ((isp_running_mode) >= ISP_MODE_RUNNING_BUTT) {           \
            isp_err_trace("Invalid isp running mode!\n"); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define isp_check_block_id_return(blk)                                   \
    do {                                                          \
        if (((blk) >= ISP_STRIPING_MAX_NUM)) {                    \
            isp_err_trace("Err isp block id %d!\n", blk); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define isp_check_block_num_return(blk)                                   \
    do {                                                           \
        if (((blk) > ISP_STRIPING_MAX_NUM) || ((blk) < 1)) {       \
            isp_err_trace("Err isp block num %d!\n", blk); \
            return HI_ERR_ISP_ILLEGAL_PARAM;                       \
        }                                                          \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
