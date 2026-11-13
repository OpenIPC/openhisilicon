/* SC850SL (SmartSens) CV610 bring-up -- tables from sc850sl_cfg.h. */
#ifndef SC850SL_SENSOR_CTRL_H
#define SC850SL_SENSOR_CTRL_H

#include "sensor_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sc850sl_linear_8m30_12bit_init(cis_info *cis);
td_s32 sc850sl_vc_wdr_2t1_8m30_10bit_init(cis_info *cis);
/* Orientation: stored per pipe, written now if the sensor is up, and again by
 * every bring-up. */
td_s32 sc850sl_set_orientation(cis_info *cis, td_bool mirror, td_bool flip);
td_void sc850sl_clear_orientation(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
