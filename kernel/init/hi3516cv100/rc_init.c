#include <linux/module.h>
extern int _cv100_hi3518_rc_init(void);
extern void _cv100_hi3518_rc_exit(void);
static int __init rc_mod_init(void) { return _cv100_hi3518_rc_init(); }
static void __exit rc_mod_exit(void) { _cv100_hi3518_rc_exit(); }
module_init(rc_mod_init);
module_exit(rc_mod_exit);
MODULE_LICENSE("GPL");
