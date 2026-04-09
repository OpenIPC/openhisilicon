/* hdmi_init.h - declarations for HDMI blob entry points */
#ifndef __HDMI_INIT_H__
#define __HDMI_INIT_H__

#include <linux/module.h>
#include <linux/of_platform.h>
#include "hi_type.h"
#include "hi_osal.h"

extern int hdmi_drv_mod_init(void);
extern void hdmi_drv_mod_exit(void);
extern void hdmi_set_reg(hi_char *reg);

#endif
