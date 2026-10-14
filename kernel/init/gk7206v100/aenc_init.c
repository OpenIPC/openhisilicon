// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_aenc.ko, relinked from
 * obj/gk7206v100/<set>/aenc.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_aenc_init(void);
extern void _gk7206_aenc_exit(void);

static int __init aenc_mod_init(void)
{
	return _gk7206_aenc_init();
}

static void __exit aenc_mod_exit(void)
{
	_gk7206_aenc_exit();
}

module_init(aenc_mod_init);
module_exit(aenc_mod_exit);
/* The license xm_aenc.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
