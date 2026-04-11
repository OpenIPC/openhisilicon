#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_h264e_init(void);
extern void _3519v101_hi3519v101_h264e_exit(void);

static int __init h264e_mod_init(void) { return _3519v101_hi3519v101_h264e_init(); }
static void __exit h264e_mod_exit(void) { _3519v101_hi3519v101_h264e_exit(); }
module_init(h264e_mod_init);
module_exit(h264e_mod_exit);
MODULE_LICENSE("GPL");
