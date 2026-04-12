#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_photo_init(void);
extern void _3519v101_hi3519v101_photo_exit(void);

static int __init photo_mod_init(void) { return _3519v101_hi3519v101_photo_init(); }
static void __exit photo_mod_exit(void) { _3519v101_hi3519v101_photo_exit(); }
module_init(photo_mod_init);
module_exit(photo_mod_exit);
MODULE_LICENSE("GPL");
