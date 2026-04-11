#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_chnl_init(void);
extern void _av100_hi3516a_chnl_exit(void);

static int __init chnl_mod_init(void) { return _av100_hi3516a_chnl_init(); }
static void __exit chnl_mod_exit(void) { _av100_hi3516a_chnl_exit(); }
module_init(chnl_mod_init);
module_exit(chnl_mod_exit);
MODULE_LICENSE("GPL");
