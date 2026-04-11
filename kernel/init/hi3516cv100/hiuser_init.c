#include <linux/module.h>
extern int _cv100_hiuser_init(void);
extern void _cv100_hiuser_exit(void);
static int __init hiuser_mod_init(void) { return _cv100_hiuser_init(); }
static void __exit hiuser_mod_exit(void) { _cv100_hiuser_exit(); }
module_init(hiuser_mod_init);
module_exit(hiuser_mod_exit);
MODULE_LICENSE("GPL");
