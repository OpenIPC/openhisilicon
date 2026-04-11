#include <linux/module.h>
extern int _cv100_hi3518_sys_init(void);
extern void _cv100_hi3518_sys_exit(void);
extern char g_benchmark_list[];	EXPORT_SYMBOL(g_benchmark_list);
extern char g_u32FuncId[];	EXPORT_SYMBOL(g_u32FuncId);
static int __init sys_mod_init(void) { return _cv100_hi3518_sys_init(); }
static void __exit sys_mod_exit(void) { _cv100_hi3518_sys_exit(); }
module_init(sys_mod_init);
module_exit(sys_mod_exit);
MODULE_LICENSE("GPL");
