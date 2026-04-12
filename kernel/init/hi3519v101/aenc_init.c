#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_aenc_init(void);
extern void _3519v101_hi3519v101_aenc_exit(void);

static int __init aenc_mod_init(void) { return _3519v101_hi3519v101_aenc_init(); }
static void __exit aenc_mod_exit(void) { _3519v101_hi3519v101_aenc_exit(); }
module_init(aenc_mod_init);
module_exit(aenc_mod_exit);
MODULE_LICENSE("GPL");
