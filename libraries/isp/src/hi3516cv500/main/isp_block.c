/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_block.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_block.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include "mkp_isp.h"
#include "hi_isp_debug.h"
#include "isp_main.h"
#include "isp_ext_config.h"

hi_s32 isp_get_work_mode(hi_vi_pipe vi_pipe, isp_working_mode *isp_work_mode)
{
    return HI_SUCCESS;
}

static hi_s32 isp_get_block_hw_attr(hi_vi_pipe vi_pipe, isp_block_attr *block)
{
    hi_s32 ret, isp_fd;
    isp_block_attr blk_attr = { 0 };

    isp_fd = isp_get_fd(vi_pipe);

    ret = ioctl(isp_fd, ISP_WORK_MODE_INIT, &blk_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ret:%d!\n", ret);
        return ret;
    }

    isp_check_running_mode_return(blk_attr.running_mode);
    isp_check_block_num_return(blk_attr.block_num);

    if (((blk_attr.running_mode == ISP_MODE_RUNNING_OFFLINE) ||
         (blk_attr.running_mode == ISP_MODE_RUNNING_ONLINE)) &&
        (blk_attr.block_num != ISP_NORMAL_BLOCK_NUM)) {
        isp_err_trace("vi_pipe :%d,when isp_running_mode = %d,block_num should be equal to %d!\n",
                      vi_pipe, blk_attr.running_mode, ISP_NORMAL_BLOCK_NUM);
        return HI_FAILURE;
    } else if ((blk_attr.running_mode == ISP_MODE_RUNNING_SIDEBYSIDE) &&
               (blk_attr.block_num != ISP_SBS_BLOCK_NUM)) {
        isp_err_trace("vi_pipe :%d,when isp_running_mode = %d,block_num should be equal to %d!\n",
                      vi_pipe, blk_attr.running_mode, ISP_SBS_BLOCK_NUM);
        return HI_FAILURE;
    } else if ((blk_attr.running_mode == ISP_MODE_RUNNING_STRIPING) && (blk_attr.block_num < MIN_SPLIT_NUM)) {
        isp_err_trace("vi_pipe :%d,when isp_running_mode = %d,block_num should not be less than %d!\n",
                      vi_pipe, blk_attr.running_mode, MIN_SPLIT_NUM);
        return HI_FAILURE;
    }

    block->running_mode = blk_attr.running_mode;
    block->block_num    = blk_attr.block_num;
    block->over_lap     = blk_attr.over_lap;

    block->frame_rect.width  = blk_attr.frame_rect.width;
    block->frame_rect.height = blk_attr.frame_rect.height;

    memcpy_s(block->block_rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM,
             blk_attr.block_rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM);

    hi_ext_system_be_total_width_write(vi_pipe,  block->frame_rect.width);
    hi_ext_system_be_total_height_write(vi_pipe, block->frame_rect.height);

    return HI_SUCCESS;
}

hi_s32 isp_block_init(hi_vi_pipe vi_pipe, isp_block_attr *block)
{
    hi_s32 ret;
    hi_s32 isp_fd = isp_get_fd(vi_pipe);

    ret = isp_get_block_hw_attr(vi_pipe, block);
    if (ret != HI_SUCCESS) {
        isp_err_trace("get isp block HW attr failed!\n");
        return ret;
    }

    block->pre_block_num = block->block_num;

    ret = ioctl(isp_fd, ISP_PRE_BLK_NUM_UPDATE, &block->pre_block_num);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d]:update pre block num failed\n", vi_pipe);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_block_update(hi_vi_pipe vi_pipe, isp_block_attr *block)
{
    hi_s32 ret;

    ret = isp_get_block_hw_attr(vi_pipe, block);
    if (ret != HI_SUCCESS) {
        isp_err_trace("get isp block HW attr failed!\n");
        return ret;
    }

    return HI_SUCCESS;
}

hi_void isp_block_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_s32 isp_fd = isp_get_fd(vi_pipe);

    ret = ioctl(isp_fd, ISP_WORK_MODE_EXIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp block exit failed!\n");
        return;
    }
}

hi_u32 isp_get_block_rect(isp_rect *block_rect, isp_block_attr *block, hi_u8 block_id)
{
    isp_check_block_id_return(block_id);

    if (block->block_num == 1) {
        block_rect->x      = 0;
        block_rect->y      = 0;
        block_rect->width  = block->frame_rect.width;
        block_rect->height = block->frame_rect.height;

        return HI_SUCCESS;
    }

    block_rect->x      = block->block_rect[block_id].x;
    block_rect->y      = block->block_rect[block_id].y;
    block_rect->width  = block->block_rect[block_id].width;
    block_rect->height = block->block_rect[block_id].height;

    return HI_SUCCESS;
}

