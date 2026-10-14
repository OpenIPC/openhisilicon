// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_vedu.ko, relinked from
 * obj/gk7206v100/<set>/vedu.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_vedu_init(void);
extern void _gk7206_vedu_exit(void);

static int __init vedu_mod_init(void)
{
	return _gk7206_vedu_init();
}

static void __exit vedu_mod_exit(void)
{
	_gk7206_vedu_exit();
}

module_init(vedu_mod_init);
module_exit(vedu_mod_exit);
/* The license xm_vedu.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
