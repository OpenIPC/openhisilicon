#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_rc_init(void);
extern void _av100_hi3516a_rc_exit(void);

static int __init rc_mod_init(void) { return _av100_hi3516a_rc_init(); }
static void __exit rc_mod_exit(void) { _av100_hi3516a_rc_exit(); }
module_init(rc_mod_init);
module_exit(rc_mod_exit);
MODULE_LICENSE("GPL");
