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

/* Orientation (0x304E horizontal mirror, 0x304F reverse readout).  The request is
 * kept apart from what a given image mode can apply: vertical flip is only
 * reachable on the full and binning modes, and crop/flex windows keep the
 * upright readout while the request survives them.  The request is cleared when
 * the caller unregisters (a new sensor session starts upright).
 *
 * imx335_apply_orientation() takes cis->mutex itself: the callback path and the
 * bring-up path must not program the readout at the same time.  Callers must not
 * hold that mutex (a non-recursive one) -- the init path uses the internal
 * locked variant for that reason.  The i2c fd is not required: before
 * cmos_isp_init() opens it the request is stored and applied at the next
 * bring-up. */
td_void imx335_get_orientation(ot_vi_pipe vi_pipe, td_bool *mirror, td_bool *flip);
td_void imx335_set_orientation(ot_vi_pipe vi_pipe, td_bool mirror, td_bool flip);
td_void imx335_clear_orientation(ot_vi_pipe vi_pipe);
td_s32 imx335_apply_orientation(cis_info *cis, td_u8 img_mode, td_bool mirror,
    td_bool flip, td_u32 area3_st_normal);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
