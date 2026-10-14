// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_region.ko, relinked from
 * obj/gk7206v100/<set>/region.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_region_init(void);
extern void _gk7206_region_exit(void);

static int __init region_mod_init(void)
{
	return _gk7206_region_init();
}

static void __exit region_mod_exit(void)
{
	_gk7206_region_exit();
}

module_init(region_mod_init);
module_exit(region_mod_exit);
/* The license xm_region.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
