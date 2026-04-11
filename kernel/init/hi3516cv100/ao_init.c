#include <linux/module.h>
extern int _cv100_hi3518_ao_init(void);
extern void _cv100_hi3518_ao_exit(void);
static int __init ao_mod_init(void) { return _cv100_hi3518_ao_init(); }
static void __exit ao_mod_exit(void) { _cv100_hi3518_ao_exit(); }
module_init(ao_mod_init);
module_exit(ao_mod_exit);
MODULE_LICENSE("GPL");
