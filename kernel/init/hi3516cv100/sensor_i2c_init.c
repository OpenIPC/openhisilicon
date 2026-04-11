#include <linux/module.h>
extern int _cv100_hi_i2c_init(void);
extern void _cv100_hi_i2c_exit(void);
static int __init sensor_i2c_mod_init(void) { return _cv100_hi_i2c_init(); }
static void __exit sensor_i2c_mod_exit(void) { _cv100_hi_i2c_exit(); }
module_init(sensor_i2c_mod_init);
module_exit(sensor_i2c_mod_exit);
MODULE_LICENSE("GPL");
