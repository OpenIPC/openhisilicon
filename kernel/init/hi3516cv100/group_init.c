#include <linux/module.h>
extern int _cv100_hi3518_group_init(void);
extern void _cv100_hi3518_group_exit(void);
static int __init group_mod_init(void) { return _cv100_hi3518_group_init(); }
static void __exit group_mod_exit(void) { _cv100_hi3518_group_exit(); }
module_init(group_mod_init);
module_exit(group_mod_exit);
MODULE_LICENSE("GPL");
