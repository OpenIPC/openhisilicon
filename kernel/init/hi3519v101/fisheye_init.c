#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_fisheye_init(void);
extern void _3519v101_hi3519v101_fisheye_exit(void);

static int __init fisheye_mod_init(void) { return _3519v101_hi3519v101_fisheye_init(); }
static void __exit fisheye_mod_exit(void) { _3519v101_hi3519v101_fisheye_exit(); }
module_init(fisheye_mod_init);
module_exit(fisheye_mod_exit);
MODULE_LICENSE("GPL");
