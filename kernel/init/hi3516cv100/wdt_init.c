#include <linux/module.h>
extern int _cv100_wdt_init(void);
extern void _cv100_wdt_exit(void);
static int __init wdt_mod_init(void) { return _cv100_wdt_init(); }
static void __exit wdt_mod_exit(void) { _cv100_wdt_exit(); }
module_init(wdt_mod_init);
module_exit(wdt_mod_exit);
MODULE_LICENSE("GPL");
