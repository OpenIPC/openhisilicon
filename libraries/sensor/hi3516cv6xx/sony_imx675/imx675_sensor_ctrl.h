/* IMX675 (Sony) CV610 bring-up — tables from imx675_cfg.h. */
#ifndef IMX675_SENSOR_CTRL_H
#define IMX675_SENSOR_CTRL_H
#include "sensor_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
td_s32 imx675_linear_5m30_10bit_init(cis_info *cis);
td_s32 imx675_linear_4m30_10bit_init(cis_info *cis);
/* Orientation: stored per pipe, written now if the sensor is up, and again by
 * every bring-up. */
td_s32 imx675_set_orientation(cis_info *cis, td_bool mirror, td_bool flip);
td_void imx675_clear_orientation(ot_vi_pipe vi_pipe);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
