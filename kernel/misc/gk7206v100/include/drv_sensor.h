#ifndef __DRV_SENSOR_H__
#define __DRV_SENSOR_H__

#include "xmedia_debug.h"
#include "xmedia_type.h"
#include "drv_type_ioctl.h"
#include <linux/gpio.h>
#include <linux/pwm.h>
#include "common.h"
#include "osal.h"
#include "xmedia_errcode.h"
#include "drv_dev_defines.h"
#include "drv_misc.h"
#include "drv_export.h"


#ifdef __cplusplus
extern "C" {
#endif

xmedia_s32 misc_sensor_init(xmedia_void);
xmedia_s32 misc_sensor_exit(xmedia_void);
xmedia_s32 misc_sensor_resume(xmedia_void);

xmedia_s32 misc_ctl_aov_init(xmedia_ulong arg);
xmedia_s32 misc_ctl_aov_set_sensor_mode(xmedia_ulong arg);
#ifdef SUPPORT_SENSOR_PWDN_MODE
xmedia_s32 misc_ctl_sensor_pwdn_standby(xmedia_ulong arg);
xmedia_s32 misc_ctl_sensor_pwdn_resume(xmedia_ulong arg);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __DRV_SENSOR_H__ */

