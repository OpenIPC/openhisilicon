#include <linux/module.h>
extern int _cv100_hi3518_ai_init(void);
extern void _cv100_hi3518_ai_exit(void);
static int __init ai_mod_init(void) { return _cv100_hi3518_ai_init(); }
static void __exit ai_mod_exit(void) { _cv100_hi3518_ai_exit(); }
module_init(ai_mod_init);
module_exit(ai_mod_exit);
MODULE_LICENSE("GPL");
