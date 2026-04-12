#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_adec_init(void);
extern void _3519v101_hi3519v101_adec_exit(void);

static int __init adec_mod_init(void) { return _3519v101_hi3519v101_adec_init(); }
static void __exit adec_mod_exit(void) { _3519v101_hi3519v101_adec_exit(); }
module_init(adec_mod_init);
module_exit(adec_mod_exit);
MODULE_LICENSE("GPL");
