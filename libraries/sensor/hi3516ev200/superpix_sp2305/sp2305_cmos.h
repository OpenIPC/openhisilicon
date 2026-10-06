/*
 * Copyright (c) GK.  All rights reserved.
 */

#ifndef __SP2305_CMOS_H_
#define __SP2305_CMOS_H_

#include "comm_sns.h"
#include "sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SP2305_I2C_ADDR   0x78 /* I2C Address of Sp2305 */
#define SP2305_ADDR_BYTE  1
#define SP2305_DATA_BYTE  1
#define SP2305_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = sp2305_get_ctx(dev))

/*
 * Orientation.  SP2305 is the same die as OV2735, whose datasheet puts mirror
 * in P1:0x3F bit0 and flip in bit1: the ISP_SNS_MIRRORFLIP_TYPE_E value.
 *
 * Both move the Bayer phase, and the sensor cannot move it back cleanly.  Its
 * window starts are in 2-pixel columns, and P2:0x5E br_first (and
 * auto_first_en, which drives it) gets B/R first by swapping pixel pairs: the
 * colour comes out right and every edge turns into a sawtooth.  So the sensor
 * reads out raw, with br_first set to what the readout already starts with,
 * and the ISP is told the order that results.  Mapped on hi3516ev300 against
 * a Siemens star for #237.
 */
#define SP2305_MIRROR_FLIP_REG(mf)  ((GK_U8)(mf) & 0x03)
#define SP2305_BR_FIRST_REG(mf)     \
    ((((mf) == ISP_SNS_NORMAL) || ((mf) == ISP_SNS_MIRROR_FLIP)) ? 0x05 : 0x04)

ISP_SNS_STATE_S    *sp2305_get_ctx(VI_PIPE ViPipe);
ISP_SNS_COMMBUS_U  *sp2305_get_bus_Info(VI_PIPE ViPipe);
ISP_SNS_MIRRORFLIP_TYPE_E sp2305_get_mirror_flip(VI_PIPE ViPipe);

void sp2305_init(VI_PIPE ViPipe);
void sp2305_exit(VI_PIPE ViPipe);
void sp2305_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E sns_mirror_flip);
void sp2305_standby(VI_PIPE ViPipe);
void sp2305_restart(VI_PIPE ViPipe);
int  sp2305_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data);
int  sp2305_read_register(VI_PIPE ViPipe, GK_S32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __SP2305_CMOS_H_ */
