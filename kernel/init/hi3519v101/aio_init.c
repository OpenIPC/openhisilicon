#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_aio_init(void);
extern void _3519v101_hi3519v101_aio_exit(void);

static int __init aio_mod_init(void) { return _3519v101_hi3519v101_aio_init(); }
static void __exit aio_mod_exit(void) { _3519v101_hi3519v101_aio_exit(); }
module_init(aio_mod_init);
module_exit(aio_mod_exit);
MODULE_LICENSE("GPL");
