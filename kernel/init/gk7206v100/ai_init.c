// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_ai.ko, relinked from
 * obj/gk7206v100/<set>/ai.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_ai_init(void);
extern void _gk7206_ai_exit(void);

static int __init ai_mod_init(void)
{
	return _gk7206_ai_init();
}

static void __exit ai_mod_exit(void)
{
	_gk7206_ai_exit();
}

module_init(ai_mod_init);
module_exit(ai_mod_exit);
/* The license xm_ai.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
