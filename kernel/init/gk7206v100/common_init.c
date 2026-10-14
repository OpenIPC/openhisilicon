// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_common.ko, relinked from
 * obj/gk7206v100/<set>/common.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_common_init(void);
extern void _gk7206_common_exit(void);

static int __init common_mod_init(void)
{
	return _gk7206_common_init();
}

static void __exit common_mod_exit(void)
{
	_gk7206_common_exit();
}

module_init(common_mod_init);
module_exit(common_mod_exit);
/* The license xm_common.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
