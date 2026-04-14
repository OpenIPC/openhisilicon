#include <linux/module.h>
/* Blob has _cv100_ir_init/exit (mis-renamed during extraction) */
extern int _cv100_ir_init(void);
extern void _cv100_ir_exit(void);
static int __init wdt_mod_init(void) { return _cv100_ir_init(); }
static void __exit wdt_mod_exit(void) { _cv100_ir_exit(); }
module_init(wdt_mod_init);
module_exit(wdt_mod_exit);
MODULE_LICENSE("GPL");
