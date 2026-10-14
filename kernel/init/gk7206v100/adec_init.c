// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_adec.ko, relinked from
 * obj/gk7206v100/<set>/adec.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_adec_init(void);
extern void _gk7206_adec_exit(void);

static int __init adec_mod_init(void)
{
	return _gk7206_adec_init();
}

static void __exit adec_mod_exit(void)
{
	_gk7206_adec_exit();
}

module_init(adec_mod_init);
module_exit(adec_mod_exit);
/* The license xm_adec.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
