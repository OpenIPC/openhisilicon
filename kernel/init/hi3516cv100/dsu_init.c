#include <linux/module.h>
extern int _cv100_hi3518_dsu_init(void);
extern void _cv100_hi3518_dsu_exit(void);
static int __init dsu_mod_init(void) { return _cv100_hi3518_dsu_init(); }
static void __exit dsu_mod_exit(void) { _cv100_hi3518_dsu_exit(); }
module_init(dsu_mod_init);
module_exit(dsu_mod_exit);
MODULE_LICENSE("GPL");
