#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_pciv_init(void);
extern void _3519v101_hi3519v101_pciv_exit(void);

static int __init pciv_mod_init(void) { return _3519v101_hi3519v101_pciv_init(); }
static void __exit pciv_mod_exit(void) { _3519v101_hi3519v101_pciv_exit(); }
module_init(pciv_mod_init);
module_exit(pciv_mod_exit);
MODULE_LICENSE("GPL");
