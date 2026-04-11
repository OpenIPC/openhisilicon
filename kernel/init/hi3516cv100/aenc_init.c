#include <linux/module.h>
extern int _cv100_hi3518_aenc_init(void);
extern void _cv100_hi3518_aenc_exit(void);
static int __init aenc_mod_init(void) { return _cv100_hi3518_aenc_init(); }
static void __exit aenc_mod_exit(void) { _cv100_hi3518_aenc_exit(); }
module_init(aenc_mod_init);
module_exit(aenc_mod_exit);
MODULE_LICENSE("GPL");
