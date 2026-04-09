/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_proc.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_PROC_H__
#define __ISP_PROC_H__

#include "hi_common.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_s32 isp_proc_init(hi_vi_pipe vi_pipe);
hi_s32 isp_proc_write(const isp_alg_node *algs, hi_vi_pipe vi_pipe);
hi_void isp_proc_exit(hi_vi_pipe vi_pipe);

#define isp_proc_printf(proc, len, fmt...)                                              \
    do {                                                                                \
        snprintf_s((proc)->proc_buff, (proc)->buff_len, (proc)->buff_len, ##fmt); \
        (proc)->write_len = strlen((proc)->proc_buff);                               \
        (proc)->proc_buff = &((proc)->proc_buff[(proc)->write_len]);                \
        (proc)->buff_len -= (proc)->write_len;                                      \
        len += (proc)->write_len;                                                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
