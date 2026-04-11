#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_pm_init(void);
extern void _av100_hi3516a_pm_exit(void);

static int __init pm_mod_init(void) { return _av100_hi3516a_pm_init(); }
static void __exit pm_mod_exit(void) { _av100_hi3516a_pm_exit(); }
module_init(pm_mod_init);
module_exit(pm_mod_exit);
MODULE_LICENSE("GPL");
