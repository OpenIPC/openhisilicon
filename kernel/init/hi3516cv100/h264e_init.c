#include <linux/module.h>
extern int _cv100_hi3518_h264e_init(void);
extern void _cv100_hi3518_h264e_exit(void);
static int __init h264e_mod_init(void) { return _cv100_hi3518_h264e_init(); }
static void __exit h264e_mod_exit(void) { _cv100_hi3518_h264e_exit(); }
module_init(h264e_mod_init);
module_exit(h264e_mod_exit);
MODULE_LICENSE("GPL");
