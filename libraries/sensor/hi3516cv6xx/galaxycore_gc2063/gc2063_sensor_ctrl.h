/* GC2063 (GalaxyCore) CV610 bring-up -- table from gc2063_cfg.h. */
#ifndef GC2063_SENSOR_CTRL_H
#define GC2063_SENSOR_CTRL_H
#include "sensor_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
td_s32 gc2063_linear_2m30_10bit_init(cis_info *cis);
/* Orientation: stored per pipe, written now if the sensor is up, and again by
 * every bring-up. */
td_s32 gc2063_set_orientation(cis_info *cis, td_bool mirror, td_bool flip);
td_void gc2063_clear_orientation(ot_vi_pipe vi_pipe);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
