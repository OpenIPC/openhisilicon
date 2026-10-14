// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_vo.ko, relinked from
 * obj/gk7206v100/<set>/vo.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_vo_init(void);
extern void _gk7206_vo_exit(void);

static int __init vo_mod_init(void)
{
	return _gk7206_vo_init();
}

static void __exit vo_mod_exit(void)
{
	_gk7206_vo_exit();
}

module_init(vo_mod_init);
module_exit(vo_mod_exit);
/* The license xm_vo.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
