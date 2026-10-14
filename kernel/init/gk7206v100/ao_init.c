// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_ao.ko, relinked from
 * obj/gk7206v100/<set>/ao.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_ao_init(void);
extern void _gk7206_ao_exit(void);

static int __init ao_mod_init(void)
{
	return _gk7206_ao_init();
}

static void __exit ao_mod_exit(void)
{
	_gk7206_ao_exit();
}

module_init(ao_mod_init);
module_exit(ao_mod_exit);
/* The license xm_ao.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
