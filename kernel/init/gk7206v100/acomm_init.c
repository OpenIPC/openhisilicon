// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_acomm.ko, relinked from
 * obj/gk7206v100/<set>/acomm.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_acomm_init(void);
extern void _gk7206_acomm_exit(void);

static int __init acomm_mod_init(void)
{
	return _gk7206_acomm_init();
}

static void __exit acomm_mod_exit(void)
{
	_gk7206_acomm_exit();
}

module_init(acomm_mod_init);
module_exit(acomm_mod_exit);
/* The license xm_acomm.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
