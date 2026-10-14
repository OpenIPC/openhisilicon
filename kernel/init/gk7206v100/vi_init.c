// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_vi.ko, relinked from
 * obj/gk7206v100/<set>/vi.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_vi_init(void);
extern void _gk7206_vi_exit(void);

static int __init vi_mod_init(void)
{
	return _gk7206_vi_init();
}

static void __exit vi_mod_exit(void)
{
	_gk7206_vi_exit();
}

module_init(vi_mod_init);
module_exit(vi_mod_exit);
/* The license xm_vi.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
