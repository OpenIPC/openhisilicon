// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_vpss.ko, relinked from
 * obj/gk7206v100/<set>/vpss.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_vpss_init(void);
extern void _gk7206_vpss_exit(void);

static int __init vpss_mod_init(void)
{
	return _gk7206_vpss_init();
}

static void __exit vpss_mod_exit(void)
{
	_gk7206_vpss_exit();
}

module_init(vpss_mod_init);
module_exit(vpss_mod_exit);
/* The license xm_vpss.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
