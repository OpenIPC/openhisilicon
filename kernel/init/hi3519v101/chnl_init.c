#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_chnl_init(void);
extern void _3519v101_hi3519v101_chnl_exit(void);

static int __init chnl_mod_init(void) { return _3519v101_hi3519v101_chnl_init(); }
static void __exit chnl_mod_exit(void) { _3519v101_hi3519v101_chnl_exit(); }
module_init(chnl_mod_init);
module_exit(chnl_mod_exit);
MODULE_LICENSE("GPL");
