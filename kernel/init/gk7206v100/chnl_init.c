// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_chnl.ko, relinked from
 * obj/gk7206v100/<set>/chnl.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_chnl_init(void);
extern void _gk7206_chnl_exit(void);

static int __init chnl_mod_init(void)
{
	return _gk7206_chnl_init();
}

static void __exit chnl_mod_exit(void)
{
	_gk7206_chnl_exit();
}

module_init(chnl_mod_init);
module_exit(chnl_mod_exit);
/* The license xm_chnl.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
