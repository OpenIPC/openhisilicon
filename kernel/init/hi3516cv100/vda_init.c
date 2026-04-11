#include <linux/module.h>
extern int _cv100_hi3518_vda_init(void);
extern void _cv100_hi3518_vda_exit(void);
static int __init vda_mod_init(void) { return _cv100_hi3518_vda_init(); }
static void __exit vda_mod_exit(void) { _cv100_hi3518_vda_exit(); }
module_init(vda_mod_init);
module_exit(vda_mod_exit);
MODULE_LICENSE("GPL");
