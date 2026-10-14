// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_cipher_drv.ko, relinked from
 * obj/gk7206v100/<set>/cipher_drv.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_cipher_drv_init(void);
extern void _gk7206_cipher_drv_exit(void);

static int __init cipher_drv_mod_init(void)
{
	return _gk7206_cipher_drv_init();
}

static void __exit cipher_drv_mod_exit(void)
{
	_gk7206_cipher_drv_exit();
}

module_init(cipher_drv_mod_init);
module_exit(cipher_drv_mod_exit);
/* The license xm_cipher_drv.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
