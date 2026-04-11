#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_jpege_init(void);
extern void _av100_hi3516a_jpege_exit(void);

static int __init jpege_mod_init(void) { return _av100_hi3516a_jpege_init(); }
static void __exit jpege_mod_exit(void) { _av100_hi3516a_jpege_exit(); }
module_init(jpege_mod_init);
module_exit(jpege_mod_exit);
MODULE_LICENSE("GPL");
