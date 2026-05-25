#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

/* See base_init.c for the >= 5.0 gate rationale. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)


extern void *g_stVouExpFunc;
COMPAT_REEXPORT_BLOB_SYMBOL(g_stVouExpFunc);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

extern int _3519v101_hi3519v101_vou_init(void);
extern void _3519v101_hi3519v101_vou_exit(void);

static int __init vou_mod_init(void) { return _3519v101_hi3519v101_vou_init(); }
static void __exit vou_mod_exit(void) { _3519v101_hi3519v101_vou_exit(); }
module_init(vou_mod_init);
module_exit(vou_mod_exit);
MODULE_LICENSE("GPL");
