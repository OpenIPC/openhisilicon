#ifndef GC4653_SENSOR_CTRL_H
#define GC4653_SENSOR_CTRL_H
#include "sensor_common.h"

/* 0x0100 is the stream control: 0x09 streams, as the init leaves it. */
#define GC4653_STREAM_ADDR 0x0100
#define GC4653_STREAM_ON   0x09
#define GC4653_STREAM_OFF  0x00

td_s32 gc4653_linear_4m30_10bit_init(cis_info *cis);
#endif
