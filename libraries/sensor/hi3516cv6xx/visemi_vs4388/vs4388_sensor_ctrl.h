#ifndef VS4388_SENSOR_CTRL_H
#define VS4388_SENSOR_CTRL_H
#include "sensor_common.h"

/* 0x0100 is the stream control: 0x01 streams, as the init leaves it. */
#define VS4388_STREAM_ADDR 0x0100
#define VS4388_STREAM_ON   0x01
#define VS4388_STREAM_OFF  0x00

td_s32 vs4388_linear_1440p30_10bit_init(cis_info *cis);
#endif
