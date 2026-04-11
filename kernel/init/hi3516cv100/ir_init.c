#include <linux/module.h>
extern int _cv100_ir_init(void);
extern void _cv100_ir_exit(void);
static int __init ir_mod_init(void) { return _cv100_ir_init(); }
static void __exit ir_mod_exit(void) { _cv100_ir_exit(); }
module_init(ir_mod_init);
module_exit(ir_mod_exit);
MODULE_LICENSE("GPL");
