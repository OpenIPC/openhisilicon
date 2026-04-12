#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_ir_init(void);
extern void _3519v101_hi3519v101_ir_exit(void);

static int __init ir_mod_init(void) { return _3519v101_hi3519v101_ir_init(); }
static void __exit ir_mod_exit(void) { _3519v101_hi3519v101_ir_exit(); }
module_init(ir_mod_init);
module_exit(ir_mod_exit);
MODULE_LICENSE("GPL");
