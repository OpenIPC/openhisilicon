/*
 * V3A sys module init wrapper.
 *
 * Re-exports legacy __ksymtab_ globals defined by hi3519v101_sys.o
 * (vi_vpss_online, en_online_delayint, mem_total, reg_*_base_va).
 * Consumer modules (open_vi, open_vpss, ...) reference these directly.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/types.h>

extern int vi_vpss_online;
module_param(vi_vpss_online, int, 0444);
COMPAT_REEXPORT_BLOB_SYMBOL(vi_vpss_online);

extern int en_online_delayint;
module_param(en_online_delayint, int, 0444);
COMPAT_REEXPORT_BLOB_SYMBOL(en_online_delayint);

extern unsigned int mem_total;
module_param(mem_total, uint, 0444);
COMPAT_REEXPORT_BLOB_SYMBOL(mem_total);

extern void *reg_crg_base_va;
COMPAT_REEXPORT_BLOB_SYMBOL(reg_crg_base_va);
extern void *reg_sys_base_va;
COMPAT_REEXPORT_BLOB_SYMBOL(reg_sys_base_va);
extern void *reg_ddr0_base_va;
COMPAT_REEXPORT_BLOB_SYMBOL(reg_ddr0_base_va);
extern void *reg_misc_base_va;
COMPAT_REEXPORT_BLOB_SYMBOL(reg_misc_base_va);

extern int _3519v101_hi3519v101_sys_init(void);
extern void _3519v101_hi3519v101_sys_exit(void);

static int __init sys_mod_init(void)
{
	return _3519v101_hi3519v101_sys_init();
}

static void __exit sys_mod_exit(void)
{
	_3519v101_hi3519v101_sys_exit();
}

module_init(sys_mod_init);
module_exit(sys_mod_exit);
MODULE_LICENSE("GPL");
