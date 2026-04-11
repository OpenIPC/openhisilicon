#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_ao_init(void);
extern void _3519v101_hi3519v101_ao_exit(void);

static int __init ao_mod_init(void) { return _3519v101_hi3519v101_ao_init(); }
static void __exit ao_mod_exit(void) { _3519v101_hi3519v101_ao_exit(); }
module_init(ao_mod_init);
module_exit(ao_mod_exit);
MODULE_LICENSE("GPL");
