// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_jpege.ko, relinked from
 * obj/gk7206v100/<set>/jpege.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_jpege_init(void);
extern void _gk7206_jpege_exit(void);

static int __init jpege_mod_init(void)
{
	return _gk7206_jpege_init();
}

static void __exit jpege_mod_exit(void)
{
	_gk7206_jpege_exit();
}

module_init(jpege_mod_init);
module_exit(jpege_mod_exit);
/* The license xm_jpege.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
