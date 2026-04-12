#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_pciv_fmw_init(void);
extern void _3519v101_hi3519v101_pciv_fmw_exit(void);

static int __init pciv_fmw_mod_init(void) { return _3519v101_hi3519v101_pciv_fmw_init(); }
static void __exit pciv_fmw_mod_exit(void) { _3519v101_hi3519v101_pciv_fmw_exit(); }
module_init(pciv_fmw_mod_init);
module_exit(pciv_fmw_mod_exit);
MODULE_LICENSE("GPL");
