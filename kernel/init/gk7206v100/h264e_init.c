// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_h264e.ko, relinked from
 * obj/gk7206v100/<set>/h264e.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_h264e_init(void);
extern void _gk7206_h264e_exit(void);

static int __init h264e_mod_init(void)
{
	return _gk7206_h264e_init();
}

static void __exit h264e_mod_exit(void)
{
	_gk7206_h264e_exit();
}

module_init(h264e_mod_init);
module_exit(h264e_mod_exit);
/* The license xm_h264e.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
