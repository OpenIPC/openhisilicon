/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef IMX335_SENSOR_CTRL_H
#define IMX335_SENSOR_CTRL_H

#include "sensor_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 imx335_init(cis_info *cis);
td_s32 imx335_get_standby_cfg(ot_isp_sns_regs_info *standby_cfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
