#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hifb_init(void);
extern void _3519v101_hifb_exit(void);

static int __init hifb_mod_init(void) { return _3519v101_hifb_init(); }
static void __exit hifb_mod_exit(void) { _3519v101_hifb_exit(); }
module_init(hifb_mod_init);
module_exit(hifb_mod_exit);
MODULE_LICENSE("GPL");
