#include <linux/module.h>
extern int _cv100_hidmac_init(void);
extern void _cv100_hidmac_exit(void);
static int __init hidmac_mod_init(void) { return _cv100_hidmac_init(); }
static void __exit hidmac_mod_exit(void) { _cv100_hidmac_exit(); }
module_init(hidmac_mod_init);
module_exit(hidmac_mod_exit);
MODULE_LICENSE("GPL");
