#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_isp_init(void);
extern void _3519v101_hi3519v101_isp_exit(void);

static int __init isp_mod_init(void) { return _3519v101_hi3519v101_isp_init(); }
static void __exit isp_mod_exit(void) { _3519v101_hi3519v101_isp_exit(); }
module_init(isp_mod_init);
module_exit(isp_mod_exit);
MODULE_LICENSE("GPL");
