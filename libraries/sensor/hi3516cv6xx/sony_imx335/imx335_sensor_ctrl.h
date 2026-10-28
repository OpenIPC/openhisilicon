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

/* Vertical flip (readout reversal).  Supported for the full and binning modes
 * only; crop/flex windows keep the upright readout and report it upstream. */
td_bool imx335_flip_supported(td_u8 img_mode);
td_bool imx335_get_flip(ot_vi_pipe vi_pipe);
td_void imx335_set_flip(ot_vi_pipe vi_pipe, td_bool flip);
td_s32 imx335_apply_orientation(cis_info *cis, td_u8 img_mode, td_bool flip, td_u32 area3_st_normal);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
