#include <linux/module.h>
extern int _3519v101_hi3519v101_sys_init(void);
extern void _3519v101_hi3519v101_sys_exit(void);
static int __init sys_mod_init(void) { return _3519v101_hi3519v101_sys_init(); }
static void __exit sys_mod_exit(void) { _3519v101_hi3519v101_sys_exit(); }
module_init(sys_mod_init);
module_exit(sys_mod_exit);
MODULE_LICENSE("GPL");
