#include <linux/module.h>
extern int _cv100_hi3518_jpege_init(void);
extern void _cv100_hi3518_jpege_exit(void);
static int __init jpege_mod_init(void) { return _cv100_hi3518_jpege_init(); }
static void __exit jpege_mod_exit(void) { _cv100_hi3518_jpege_exit(); }
module_init(jpege_mod_init);
module_exit(jpege_mod_exit);
MODULE_LICENSE("GPL");
