#include <linux/module.h>
extern int _cv100_hi3518_region_init(void);
extern void _cv100_hi3518_region_exit(void);
static int __init region_mod_init(void) { return _cv100_hi3518_region_init(); }
static void __exit region_mod_exit(void) { _cv100_hi3518_region_exit(); }
module_init(region_mod_init);
module_exit(region_mod_exit);
MODULE_LICENSE("GPL");
