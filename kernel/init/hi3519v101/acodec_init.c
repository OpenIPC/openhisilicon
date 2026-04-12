#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi_acodec_init(void);
extern void _3519v101_hi_acodec_exit(void);

static int __init acodec_mod_init(void) { return _3519v101_hi_acodec_init(); }
static void __exit acodec_mod_exit(void) { _3519v101_hi_acodec_exit(); }
module_init(acodec_mod_init);
module_exit(acodec_mod_exit);
MODULE_LICENSE("GPL");
