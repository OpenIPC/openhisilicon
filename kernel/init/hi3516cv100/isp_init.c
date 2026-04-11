#include <linux/module.h>
extern int _cv100_hi3518_isp_init(void);
extern void _cv100_hi3518_isp_exit(void);
static int __init isp_mod_init(void) { return _cv100_hi3518_isp_init(); }
static void __exit isp_mod_exit(void) { _cv100_hi3518_isp_exit(); }
module_init(isp_mod_init);
module_exit(isp_mod_exit);
MODULE_LICENSE("GPL");
