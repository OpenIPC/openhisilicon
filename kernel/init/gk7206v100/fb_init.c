// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_fb.ko, relinked from
 * obj/gk7206v100/<set>/fb.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_fb_init(void);
extern void _gk7206_fb_exit(void);

static int __init fb_mod_init(void)
{
	return _gk7206_fb_init();
}

static void __exit fb_mod_exit(void)
{
	_gk7206_fb_exit();
}

module_init(fb_mod_init);
module_exit(fb_mod_exit);
/* The license xm_fb.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
