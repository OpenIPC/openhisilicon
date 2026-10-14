// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_vgs.ko, relinked from
 * obj/gk7206v100/<set>/vgs.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_vgs_init(void);
extern void _gk7206_vgs_exit(void);

static int __init vgs_mod_init(void)
{
	return _gk7206_vgs_init();
}

static void __exit vgs_mod_exit(void)
{
	_gk7206_vgs_exit();
}

module_init(vgs_mod_init);
module_exit(vgs_mod_exit);
/* The license xm_vgs.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
