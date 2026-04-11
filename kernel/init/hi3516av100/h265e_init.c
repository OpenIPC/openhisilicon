#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_h265e_init(void);
extern void _av100_hi3516a_h265e_exit(void);

static int __init h265e_mod_init(void) { return _av100_hi3516a_h265e_init(); }
static void __exit h265e_mod_exit(void) { _av100_hi3516a_h265e_exit(); }
module_init(h265e_mod_init);
module_exit(h265e_mod_exit);
MODULE_LICENSE("GPL");
