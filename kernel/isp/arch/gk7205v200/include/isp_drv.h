/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_DRV_H__
#define __ISP_DRV_H__

#include "comm_isp.h"

#define ISP_FE_IRQ_NR 75

#define ISP_BITMASK(bit)  ((1 << (bit)) - 1)
GK_S32 ISP_DRV_SetOnlineSttAddr(VI_PIPE ViPipe, GK_U64 u64PhyBaseAddr);
#endif /* __ISP_DRV_H__ */
