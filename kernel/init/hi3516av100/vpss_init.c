#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_vpss_init(void);
extern void _av100_hi3516a_vpss_exit(void);

static int __init vpss_mod_init(void) { return _av100_hi3516a_vpss_init(); }
static void __exit vpss_mod_exit(void) { _av100_hi3516a_vpss_exit(); }
module_init(vpss_mod_init);
module_exit(vpss_mod_exit);
MODULE_LICENSE("GPL");
