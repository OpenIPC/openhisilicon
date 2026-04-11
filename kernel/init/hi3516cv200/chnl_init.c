#include <linux/module.h>

extern int _cv200_hi3518e_chnl_init(void);
extern void _cv200_hi3518e_chnl_exit(void);



static int __init chnl_mod_init(void)
{
	return _cv200_hi3518e_chnl_init();
}

static void __exit chnl_mod_exit(void)
{
	_cv200_hi3518e_chnl_exit();
}

module_init(chnl_mod_init);
module_exit(chnl_mod_exit);
MODULE_LICENSE("Proprietary");
