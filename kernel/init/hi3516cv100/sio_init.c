#include <linux/module.h>
extern int _cv100_hi3518_sio_init(void);
extern void _cv100_hi3518_sio_exit(void);
static int __init sio_mod_init(void) { return _cv100_hi3518_sio_init(); }
static void __exit sio_mod_exit(void) { _cv100_hi3518_sio_exit(); }
module_init(sio_mod_init);
module_exit(sio_mod_exit);
MODULE_LICENSE("GPL");
