#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_rtc_init(void);
extern void _3519v101_hi3519v101_rtc_exit(void);

static int __init rtc_mod_init(void) { return _3519v101_hi3519v101_rtc_init(); }
static void __exit rtc_mod_exit(void) { _3519v101_hi3519v101_rtc_exit(); }
module_init(rtc_mod_init);
module_exit(rtc_mod_exit);
MODULE_LICENSE("GPL");
