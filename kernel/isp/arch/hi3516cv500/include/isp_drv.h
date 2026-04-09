/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_drv.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DRV_H__
#define __ISP_DRV_H__

#include "hi_comm_isp.h"
#include "sys_ext.h"

#define ISP_FE_IRQ_NR 88

#define isp_bitmask(bit)  ((1 << (bit)) - 1)
hi_s32 isp_drv_set_online_stt_addr(hi_vi_pipe vi_pipe, hi_u64 phy_base_addr);
#endif /* __ISP_DRV_H__ */
