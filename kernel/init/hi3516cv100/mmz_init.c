#include <linux/module.h>
extern int _cv100_mmz_init(void);
extern void _cv100_mmz_exit(void);
static int __init mmz_mod_init(void) { return _cv100_mmz_init(); }
static void __exit mmz_mod_exit(void) { _cv100_mmz_exit(); }
module_init(mmz_mod_init);
module_exit(mmz_mod_exit);
MODULE_LICENSE("GPL");
