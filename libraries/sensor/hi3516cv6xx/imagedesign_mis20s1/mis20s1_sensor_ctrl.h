#ifndef MIS20S1_SENSOR_CTRL_H
#define MIS20S1_SENSOR_CTRL_H
#include "sensor_common.h"

/*
 * The vendor driver's stream control: 0x300c streams at 0x01; standby is
 * 0x300c 0 then 0x3006 0x02, and restart 0x3006 0, 1 ms, 0x300c 0x01.
 */
#define MIS20S1_STREAM_ADDR 0x300c
#define MIS20S1_STREAM_ON   0x01
#define MIS20S1_STREAM_OFF  0x00
#define MIS20S1_POWER_ADDR  0x3006
#define MIS20S1_POWER_ON    0x00
#define MIS20S1_POWER_STBY  0x02
/* mirror (bit 0) and flip (bit 1), written as ot_isp_sns_mirrorflip_type */
#define MIS20S1_MIRROR_FLIP_ADDR 0x3007

td_s32 mis20s1_linear_1080p30_12bit_init(cis_info *cis);
#endif
