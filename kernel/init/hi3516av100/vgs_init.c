#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_vgs_init(void);
extern void _av100_hi3516a_vgs_exit(void);

static int __init vgs_mod_init(void) { return _av100_hi3516a_vgs_init(); }
static void __exit vgs_mod_exit(void) { _av100_hi3516a_vgs_exit(); }
module_init(vgs_mod_init);
module_exit(vgs_mod_exit);
MODULE_LICENSE("GPL");
