#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_pm_init(void);
extern void _3519v101_hi3519v101_pm_exit(void);

static int __init pm_mod_init(void) { return _3519v101_hi3519v101_pm_init(); }
static void __exit pm_mod_exit(void) { _3519v101_hi3519v101_pm_exit(); }
module_init(pm_mod_init);
module_exit(pm_mod_exit);
MODULE_LICENSE("GPL");
