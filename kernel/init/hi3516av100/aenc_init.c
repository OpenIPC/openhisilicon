#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_aenc_init(void);
extern void _av100_hi3516a_aenc_exit(void);

static int __init aenc_mod_init(void) { return _av100_hi3516a_aenc_init(); }
static void __exit aenc_mod_exit(void) { _av100_hi3516a_aenc_exit(); }
module_init(aenc_mod_init);
module_exit(aenc_mod_exit);
MODULE_LICENSE("GPL");
