#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_ai_init(void);
extern void _3519v101_hi3519v101_ai_exit(void);

static int __init ai_mod_init(void) { return _3519v101_hi3519v101_ai_init(); }
static void __exit ai_mod_exit(void) { _3519v101_hi3519v101_ai_exit(); }
module_init(ai_mod_init);
module_exit(ai_mod_exit);
MODULE_LICENSE("GPL");
