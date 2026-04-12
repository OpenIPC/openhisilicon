#include <linux/module.h>
extern int _3519v101_hi3519v101_tde_init(void);
extern void _3519v101_hi3519v101_tde_exit(void);
static int __init tde_mod_init(void) { return _3519v101_hi3519v101_tde_init(); }
static void __exit tde_mod_exit(void) { _3519v101_hi3519v101_tde_exit(); }
module_init(tde_mod_init);
module_exit(tde_mod_exit);
MODULE_LICENSE("GPL");
