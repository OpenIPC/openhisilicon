#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_ao_init(void);
extern void _av100_hi3516a_ao_exit(void);

static int __init ao_mod_init(void) { return _av100_hi3516a_ao_init(); }
static void __exit ao_mod_exit(void) { _av100_hi3516a_ao_exit(); }
module_init(ao_mod_init);
module_exit(ao_mod_exit);
MODULE_LICENSE("GPL");
