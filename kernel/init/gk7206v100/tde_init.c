// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_tde.ko, relinked from
 * obj/gk7206v100/<set>/tde.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_tde_init(void);
extern void _gk7206_tde_exit(void);

static int __init tde_mod_init(void)
{
	return _gk7206_tde_init();
}

static void __exit tde_mod_exit(void)
{
	_gk7206_tde_exit();
}

module_init(tde_mod_init);
module_exit(tde_mod_exit);
/* The license xm_tde.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
