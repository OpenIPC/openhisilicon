// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor hi3536dv100_vou.ko (Hi3536DV100_SDK_V2.0.2.0), relinked from
 * obj/hi3536dv100/vou.o (see scripts/ko2obj.sh). The blob keeps its own
 * EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _hi3536dv100_vou_init(void);
extern void _hi3536dv100_vou_exit(void);

static int __init vou_mod_init(void)
{
	return _hi3536dv100_vou_init();
}

static void __exit vou_mod_exit(void)
{
	_hi3536dv100_vou_exit();
}

module_init(vou_mod_init);
module_exit(vou_mod_exit);
/* The license hi3536dv100_vou.ko declares. */
MODULE_LICENSE("Proprietary");
