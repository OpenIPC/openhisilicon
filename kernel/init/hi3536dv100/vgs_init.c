// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor hi3536dv100_vgs.ko (Hi3536DV100_SDK_V2.0.2.0), relinked from
 * obj/hi3536dv100/vgs.o (see scripts/ko2obj.sh). The blob keeps its own
 * EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _hi3536dv100_vgs_init(void);
extern void _hi3536dv100_vgs_exit(void);

static int __init vgs_mod_init(void)
{
	return _hi3536dv100_vgs_init();
}

static void __exit vgs_mod_exit(void)
{
	_hi3536dv100_vgs_exit();
}

module_init(vgs_mod_init);
module_exit(vgs_mod_exit);
/* The license hi3536dv100_vgs.ko declares. */
MODULE_LICENSE("Proprietary");
