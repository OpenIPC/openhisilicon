#include <linux/module.h>
extern int _3519v101_hi3519v101_base_init(void);
extern void _3519v101_hi3519v101_base_exit(void);
static int __init base_mod_init(void) { return _3519v101_hi3519v101_base_init(); }
static void __exit base_mod_exit(void) { _3519v101_hi3519v101_base_exit(); }
module_init(base_mod_init);
module_exit(base_mod_exit);
MODULE_LICENSE("GPL");
