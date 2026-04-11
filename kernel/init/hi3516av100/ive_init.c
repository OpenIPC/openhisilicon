#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_ive_init(void);
extern void _av100_hi3516a_ive_exit(void);

static int __init ive_mod_init(void) { return _av100_hi3516a_ive_init(); }
static void __exit ive_mod_exit(void) { _av100_hi3516a_ive_exit(); }
module_init(ive_mod_init);
module_exit(ive_mod_exit);
MODULE_LICENSE("GPL");
