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

ISP_SNS_STATE_S    *sp2305_get_ctx(VI_PIPE ViPipe);
ISP_SNS_COMMBUS_U  *sp2305_get_bus_Info(VI_PIPE ViPipe);

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
