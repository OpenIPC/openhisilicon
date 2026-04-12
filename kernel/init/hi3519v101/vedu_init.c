#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_vedu_init(void);
extern void _3519v101_hi3519v101_vedu_exit(void);

static int __init vedu_mod_init(void) { return _3519v101_hi3519v101_vedu_init(); }
static void __exit vedu_mod_exit(void) { _3519v101_hi3519v101_vedu_exit(); }
module_init(vedu_mod_init);
module_exit(vedu_mod_exit);
MODULE_LICENSE("GPL");
