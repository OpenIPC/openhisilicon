#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_wdt_init(void);
extern void _3519v101_hi3519v101_wdt_exit(void);

static int __init wdt_mod_init(void) { return _3519v101_hi3519v101_wdt_init(); }
static void __exit wdt_mod_exit(void) { _3519v101_hi3519v101_wdt_exit(); }
module_init(wdt_mod_init);
module_exit(wdt_mod_exit);
MODULE_LICENSE("GPL");
