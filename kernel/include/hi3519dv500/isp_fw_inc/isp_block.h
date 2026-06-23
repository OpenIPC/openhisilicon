/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_BLOCK_CONFIG_H
#define ISP_BLOCK_CONFIG_H

#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_isp.h"
#include "ot_common_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIN_SPLIT_NUM  2
#define ISP_NORMAL_BLOCK_NUM 1

typedef struct {
    td_s32 x;
    td_s32 y;
    td_u32 width;
    td_u32 height;
} isp_rect;

typedef enum {
    ISP_MODE_RUNNING_OFFLINE = 0,
    ISP_MODE_RUNNING_ONLINE,
    ISP_MODE_RUNNING_SIDEBYSIDE,
    ISP_MODE_RUNNING_STRIPING,
    ISP_MODE_RUNNING_PRE_ONLINE_POST_OFFLINE,

    ISP_MODE_RUNNING_BUTT,
} isp_running_mode;

typedef enum {
    ISP_MODE_RAW = 0,
    ISP_MODE_BT1120_YUV,
    ISP_MODE_DATAINPUT_BUTT,
} isp_data_input_mode;

typedef struct {
    td_bool             is_ia_nr_enable;
    td_u8                pre_block_num;
    td_u8                block_num;
    td_u8                block_dev;
    td_u32               over_lap;
    ot_size              frame_rect;  /* BE full frame  information */
    ot_rect              block_rect[OT_ISP_STRIPING_MAX_NUM];
    isp_running_mode     running_mode;
    ot_vi_aiisp_mode     aiisp_mode;
    isp_data_input_mode  data_input_mode;
} isp_working_mode;

typedef struct {
    td_u8   block_num;
    td_u8   pre_block_num;
    td_u32  over_lap;
    ot_size frame_rect;  /* BE full frame  information */
    ot_rect block_rect[OT_ISP_STRIPING_MAX_NUM];
    isp_running_mode running_mode;
} isp_block_attr;

td_s32 isp_block_init(ot_vi_pipe vi_pipe, isp_block_attr *block);
td_void isp_block_exit(ot_vi_pipe vi_pipe);
td_s32 isp_block_update(ot_vi_pipe vi_pipe, isp_block_attr *block);
td_u32 isp_get_block_rect(isp_rect *block_rect, isp_block_attr *block, td_u8 block_id);

#define isp_check_running_mode_return(isp_running_mode)                   \
    do {                                                          \
        if ((isp_running_mode) >= ISP_MODE_RUNNING_BUTT) {           \
            isp_err_trace("Invalid isp running mode!\n"); \
            return OT_ERR_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define isp_check_block_id_return(blk)                                   \
    do {                                                          \
        if (((blk) >= OT_ISP_STRIPING_MAX_NUM)) {                    \
            isp_err_trace("Err isp block id %u!\n", blk); \
            return OT_ERR_ISP_ILLEGAL_PARAM;                      \
        }                                                         \
    } while (0)

#define isp_check_block_num_return(blk)                                   \
    do {                                                           \
        if (((blk) > OT_ISP_STRIPING_MAX_NUM) || ((blk) < 1)) {       \
            isp_err_trace("Err isp block num %u!\n", blk); \
            return OT_ERR_ISP_ILLEGAL_PARAM;                       \
        }                                                          \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
