#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_region_init(void);
extern void _3519v101_hi3519v101_region_exit(void);

static int __init region_mod_init(void) { return _3519v101_hi3519v101_region_init(); }
static void __exit region_mod_exit(void) { _3519v101_hi3519v101_region_exit(); }
module_init(region_mod_init);
module_exit(region_mod_exit);
MODULE_LICENSE("GPL");
