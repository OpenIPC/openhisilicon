// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor xm_aio.ko, relinked from
 * obj/gk7206v100/<set>/aio.o (see scripts/ko2obj.sh). The blob keeps its
 * own EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _gk7206_aio_init(void);
extern void _gk7206_aio_exit(void);

static int __init aio_mod_init(void)
{
	return _gk7206_aio_init();
}

static void __exit aio_mod_exit(void)
{
	_gk7206_aio_exit();
}

module_init(aio_mod_init);
module_exit(aio_mod_exit);
/* The license xm_aio.ko declares; it imports GPL-only symbols. */
MODULE_LICENSE("GPL");
