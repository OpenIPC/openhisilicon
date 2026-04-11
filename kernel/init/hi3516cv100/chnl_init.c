#include <linux/module.h>
extern int _cv100_hi3518_chnl_init(void);
extern void _cv100_hi3518_chnl_exit(void);
static int __init chnl_mod_init(void) { return _cv100_hi3518_chnl_init(); }
static void __exit chnl_mod_exit(void) { _cv100_hi3518_chnl_exit(); }
module_init(chnl_mod_init);
module_exit(chnl_mod_exit);
MODULE_LICENSE("GPL");
