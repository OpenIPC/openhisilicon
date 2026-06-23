/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_STATISTICS_H
#define ISP_STATISTICS_H

#include "ot_common_isp.h"
#include "mkp_isp.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* init the statistics buf */
td_s32 isp_statistics_init(ot_vi_pipe vi_pipe);
/* exit the statistics buf */
td_void isp_statistics_exit(ot_vi_pipe vi_pipe);
/* get a statistics buf from kernel */
td_s32 isp_statistics_get_buf(ot_vi_pipe vi_pipe, td_void **stat);
/* release a statistics buf to kernel */
td_s32 isp_statistics_put_buf(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
