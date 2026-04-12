#include <linux/module.h>
extern int _3519v101_hi3519v101_viu_init(void);
extern void _3519v101_hi3519v101_viu_exit(void);
static int __init viu_mod_init(void) { return _3519v101_hi3519v101_viu_init(); }
static void __exit viu_mod_exit(void) { _3519v101_hi3519v101_viu_exit(); }
module_init(viu_mod_init);
module_exit(viu_mod_exit);
MODULE_LICENSE("GPL");
