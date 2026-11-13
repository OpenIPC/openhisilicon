/* IMX678 (Sony) CV610 bring-up: the table in imx678_cfg.h. */
#ifndef IMX678_SENSOR_CTRL_H
#define IMX678_SENSOR_CTRL_H
#include "sensor_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/* Internal regulator stabilization between standby cancel and master start */
#define IMX678_REGULATOR_WAIT_MS 24

td_s32 imx678_linear_4k30_12bit_init(cis_info *cis);
td_s32 imx678_standby(cis_info *cis);
td_s32 imx678_restart(cis_info *cis);
/* Orientation: stored per pipe, written now if the sensor is up, and again by
 * every bring-up. */
td_s32 imx678_set_orientation(cis_info *cis, td_bool mirror, td_bool flip);
td_void imx678_clear_orientation(ot_vi_pipe vi_pipe);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
