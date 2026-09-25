/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __ISP_STATISTICS_H__
#define __ISP_STATISTICS_H__

#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* init the statistics buf */
XMEDIA_S32 ISP_StatisticsInit(VI_PIPE ViPipe);
/* exit the statistics buf */
XMEDIA_S32 ISP_StatisticsExit(VI_PIPE ViPipe);
/* get a statistics buf from kernel */
XMEDIA_S32 ISP_StatisticsGetBuf(VI_PIPE ViPipe, XMEDIA_VOID **ppStat);
/* release a statistics buf to kernel */
XMEDIA_S32 ISP_StatisticsPutBuf(VI_PIPE ViPipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
