// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor hi3536dv100_hdmi.ko (Hi3536DV100_SDK_V2.0.2.0), relinked from
 * obj/hi3536dv100/hdmi.o (see scripts/ko2obj.sh). The blob keeps its own
 * EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _hi3536dv100_hdmi_init(void);
extern void _hi3536dv100_hdmi_exit(void);

static int __init hdmi_mod_init(void)
{
	return _hi3536dv100_hdmi_init();
}

static void __exit hdmi_mod_exit(void)
{
	_hi3536dv100_hdmi_exit();
}

module_init(hdmi_mod_init);
module_exit(hdmi_mod_exit);
/* The license hi3536dv100_hdmi.ko declares. */
MODULE_LICENSE("Proprietary");
