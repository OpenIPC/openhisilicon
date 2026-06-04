/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SC4336P_SENSOR_CTRL_H
#define SC4336P_SENSOR_CTRL_H

#include "sensor_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sc4336p_linear_4m30_10bit_init(cis_info *cis);
td_s32 sc4336p_get_standby_cfg(ot_isp_sns_regs_info *standby_cfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
