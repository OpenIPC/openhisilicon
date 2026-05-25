#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

/* See base_init.c for the >= 5.0 gate rationale. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)


extern void *pAcodec;
COMPAT_REEXPORT_BLOB_SYMBOL(pAcodec);
extern void *pAioBase;
COMPAT_REEXPORT_BLOB_SYMBOL(pAioBase);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

extern int _3519v101_hi3519v101_aio_init(void);
extern void _3519v101_hi3519v101_aio_exit(void);

static int __init aio_mod_init(void) { return _3519v101_hi3519v101_aio_init(); }
static void __exit aio_mod_exit(void) { _3519v101_hi3519v101_aio_exit(); }
module_init(aio_mod_init);
module_exit(aio_mod_exit);
MODULE_LICENSE("GPL");
