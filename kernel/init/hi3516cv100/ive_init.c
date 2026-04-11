#include <linux/module.h>
extern int _cv100_hi3518_ive_init(void);
extern void _cv100_hi3518_ive_exit(void);
static int __init ive_mod_init(void) { return _cv100_hi3518_ive_init(); }
static void __exit ive_mod_exit(void) { _cv100_hi3518_ive_exit(); }
module_init(ive_mod_init);
module_exit(ive_mod_exit);
MODULE_LICENSE("GPL");
