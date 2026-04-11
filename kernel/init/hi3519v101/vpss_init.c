#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_vpss_init(void);
extern void _3519v101_hi3519v101_vpss_exit(void);

static int __init vpss_mod_init(void) { return _3519v101_hi3519v101_vpss_init(); }
static void __exit vpss_mod_exit(void) { _3519v101_hi3519v101_vpss_exit(); }
module_init(vpss_mod_init);
module_exit(vpss_mod_exit);
MODULE_LICENSE("GPL");
