#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_aio_init(void);
extern void _av100_hi3516a_aio_exit(void);

static int __init aio_mod_init(void) { return _av100_hi3516a_aio_init(); }
static void __exit aio_mod_exit(void) { _av100_hi3516a_aio_exit(); }
module_init(aio_mod_init);
module_exit(aio_mod_exit);
MODULE_LICENSE("GPL");
