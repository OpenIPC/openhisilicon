/*
 * DIS (Digital Image Stabilization) module init - hi3516cv500 only
 *
 * Adapted from HiSilicon vendor SDK
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include "../compat/compat.h"
#include "../compat/kernel_compat.h"

extern int dis_module_init(void);
extern void dis_module_exit(void);

static int __init dis_mod_init(void)
{
	return dis_module_init();
}

static void __exit dis_mod_exit(void)
{
	dis_module_exit();
}

module_init(dis_mod_init);
module_exit(dis_mod_exit);

MODULE_LICENSE("GPL");
