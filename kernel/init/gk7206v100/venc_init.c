// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_venc.ko, relinked from
 * obj/gk7206v100/<set>/venc.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_venc_init(void);
extern void _gk7206_venc_exit(void);

static int __init venc_mod_init(void)
{
	return _gk7206_venc_init();
}

static void __exit venc_mod_exit(void)
{
	_gk7206_venc_exit();
}

module_init(venc_mod_init);
module_exit(venc_mod_exit);
/* The license xm_venc.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
