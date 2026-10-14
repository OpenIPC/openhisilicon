// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_h265e.ko, relinked from
 * obj/gk7206v100/<set>/h265e.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_h265e_init(void);
extern void _gk7206_h265e_exit(void);

static int __init h265e_mod_init(void)
{
	return _gk7206_h265e_init();
}

static void __exit h265e_mod_exit(void)
{
	_gk7206_h265e_exit();
}

module_init(h265e_mod_init);
module_exit(h265e_mod_exit);
/* The license xm_h265e.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
