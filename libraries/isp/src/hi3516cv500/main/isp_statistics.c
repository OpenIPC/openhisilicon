/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_statistics.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_statistics.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "mpi_sys.h"
#include "isp_ext_config.h"
#include "isp_main.h"

typedef struct {
    hi_u64  stat_phyaddr;
    hi_void *stat_virt_addr;

    hi_bool read;
    isp_stat_info stat_info;
} isp_sta_ctx;

isp_sta_ctx g_stat_ctx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define statistics_get_ctx(dev, ctx) ctx = &g_stat_ctx[dev]

hi_s32 isp_statistics_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_sta_ctx *stat = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    statistics_get_ctx(vi_pipe, stat);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_STAT_BUF_INIT, &stat->stat_phyaddr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init statistics bufs failed!\n", vi_pipe);
        return ret;
    }

    stat->read = HI_FALSE;
    stat->stat_virt_addr = HI_MPI_SYS_MmapCache(stat->stat_phyaddr,
                                                sizeof(isp_stat) * MAX_ISP_STAT_BUF_NUM);

    if (stat->stat_virt_addr == HI_NULL) {
        isp_err_trace("ISP[%d] mmap statistics bufs failed!\n", vi_pipe);

        ret = ioctl(isp_get_fd(vi_pipe), ISP_STAT_BUF_EXIT);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] exit statistics bufs failed!\n", vi_pipe);
            return ret;
        }

        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

hi_void isp_statistics_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_sta_ctx *stat = HI_NULL;

    statistics_get_ctx(vi_pipe, stat);

    if (stat->stat_virt_addr != HI_NULL) {
        HI_MPI_SYS_Munmap(stat->stat_virt_addr, sizeof(isp_stat) * MAX_ISP_STAT_BUF_NUM);
        stat->stat_virt_addr = HI_NULL;
        stat->stat_info.phy_addr  = 0;
        stat->stat_info.virt_addr = HI_NULL;
        stat->read                = HI_FALSE;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_STAT_BUF_EXIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] exit statistics bufs failed!\n", vi_pipe);
        return;
    }
}

hi_s32 isp_statistics_get_buf(hi_vi_pipe vi_pipe, hi_void **stat)
{
    hi_s32 ret;
    isp_sta_ctx *stat_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat);

    statistics_get_ctx(vi_pipe, stat_ctx);

    if (!stat_ctx->read) {
        ret = ioctl(isp_get_fd(vi_pipe), ISP_STAT_BUF_GET, &stat_ctx->stat_info);
        if (ret) {
            return ret;
        }

        if (stat_ctx->stat_virt_addr != HI_NULL) {
            stat_ctx->stat_info.virt_addr = (hi_void *)((hi_u8 *)stat_ctx->stat_virt_addr +
                                                        (stat_ctx->stat_info.phy_addr - stat_ctx->stat_phyaddr));
        } else {
            stat_ctx->stat_info.virt_addr = HI_NULL;
        }

        stat_ctx->read = HI_TRUE;
    }

    if (stat_ctx->stat_info.virt_addr == HI_NULL) {
        return HI_FAILURE;
    }

    *stat = stat_ctx->stat_info.virt_addr;

    return HI_SUCCESS;
}

hi_s32 isp_statistics_put_buf(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;
    isp_sta_ctx *stat = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    statistics_get_ctx(vi_pipe, stat);

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat->stat_info.stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit; /* left shift 32bits */

    if (stat->read) {
        ret = ioctl(isp_get_fd(vi_pipe), ISP_STAT_BUF_PUT, &stat->stat_info);
        if (ret) {
            isp_err_trace("release ISP[%d] stat buf failed with ec %#x!\n", vi_pipe, ret);
        }
        stat->stat_info.virt_addr = HI_NULL;
        stat->read = HI_FALSE;
    }

    return HI_SUCCESS;
}

