#include <linux/module.h>
extern int _cv100_hi3518_vpss_init(void);
extern void _cv100_hi3518_vpss_exit(void);
static int __init vpss_mod_init(void) { return _cv100_hi3518_vpss_init(); }
static void __exit vpss_mod_exit(void) { _cv100_hi3518_vpss_exit(); }
module_init(vpss_mod_init);
module_exit(vpss_mod_exit);
MODULE_LICENSE("GPL");
