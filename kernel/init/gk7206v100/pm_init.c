// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_pm.ko, relinked from
 * obj/gk7206v100/<set>/pm.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_pm_init(void);
extern void _gk7206_pm_exit(void);

static int __init pm_mod_init(void)
{
	return _gk7206_pm_init();
}

static void __exit pm_mod_exit(void)
{
	_gk7206_pm_exit();
}

module_init(pm_mod_init);
module_exit(pm_mod_exit);
/* The license xm_pm.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
