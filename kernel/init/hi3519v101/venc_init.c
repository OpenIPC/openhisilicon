#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_venc_init(void);
extern void _3519v101_hi3519v101_venc_exit(void);

static int __init venc_mod_init(void) { return _3519v101_hi3519v101_venc_init(); }
static void __exit venc_mod_exit(void) { _3519v101_hi3519v101_venc_exit(); }
module_init(venc_mod_init);
module_exit(venc_mod_exit);
MODULE_LICENSE("GPL");
