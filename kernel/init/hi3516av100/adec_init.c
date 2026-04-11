#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_adec_init(void);
extern void _av100_hi3516a_adec_exit(void);

static int __init adec_mod_init(void) { return _av100_hi3516a_adec_init(); }
static void __exit adec_mod_exit(void) { _av100_hi3516a_adec_exit(); }
module_init(adec_mod_init);
module_exit(adec_mod_exit);
MODULE_LICENSE("GPL");
