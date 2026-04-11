#include <linux/module.h>
extern int _cv100_pwm_init(void);
extern void _cv100_pwm_exit(void);
static int __init pwm_mod_init(void) { return _cv100_pwm_init(); }
static void __exit pwm_mod_exit(void) { _cv100_pwm_exit(); }
module_init(pwm_mod_init);
module_exit(pwm_mod_exit);
MODULE_LICENSE("GPL");
