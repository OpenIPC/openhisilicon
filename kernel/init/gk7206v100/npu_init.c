// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_npu.ko, relinked from
 * obj/gk7206v100/<set>/npu.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_npu_init(void);
extern void _gk7206_npu_exit(void);

static int __init npu_mod_init(void)
{
	return _gk7206_npu_init();
}

static void __exit npu_mod_exit(void)
{
	_gk7206_npu_exit();
}

module_init(npu_mod_init);
module_exit(npu_mod_exit);
/* The license xm_npu.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
