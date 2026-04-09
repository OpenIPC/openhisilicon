/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_statistics.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_STATISTICS_H__
#define __ISP_STATISTICS_H__

#include "hi_comm_isp.h"
#include "mkp_isp.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* init the statistics buf */
hi_s32 isp_statistics_init(hi_vi_pipe vi_pipe);
/* exit the statistics buf */
hi_void isp_statistics_exit(hi_vi_pipe vi_pipe);
/* get a statistics buf from kernel */
hi_s32 isp_statistics_get_buf(hi_vi_pipe vi_pipe, hi_void **pp_stat);
/* release a statistics buf to kernel */
hi_s32 isp_statistics_put_buf(hi_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
