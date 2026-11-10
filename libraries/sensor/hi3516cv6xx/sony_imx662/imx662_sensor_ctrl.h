/* IMX662 (Sony) CV610 bring-up — tables from imx662_cfg.h. */
#ifndef IMX662_SENSOR_CTRL_H
#define IMX662_SENSOR_CTRL_H
#include "sensor_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
td_s32 imx662_linear_2m30_12bit_init(cis_info *cis);
td_s32 imx662_linear_2m60_12bit_init(cis_info *cis);
td_s32 imx662_linear_2m90_10bit_init(cis_info *cis);
/* Orientation: stored per pipe, written now if the sensor is up, and again by
 * every bring-up. */
td_s32 imx662_set_orientation(cis_info *cis, td_bool mirror, td_bool flip);
td_void imx662_clear_orientation(ot_vi_pipe vi_pipe);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
