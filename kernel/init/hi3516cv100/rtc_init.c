#include <linux/module.h>
extern int _cv100_hi_rtc_init(void);
extern void _cv100_hi_rtc_exit(void);
static int __init rtc_mod_init(void) { return _cv100_hi_rtc_init(); }
static void __exit rtc_mod_exit(void) { _cv100_hi_rtc_exit(); }
module_init(rtc_mod_init);
module_exit(rtc_mod_exit);
MODULE_LICENSE("GPL");
