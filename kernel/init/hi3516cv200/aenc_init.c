#include <linux/module.h>

extern int _cv200_hi3518e_aenc_init(void);
extern void _cv200_hi3518e_aenc_exit(void);



static int __init aenc_mod_init(void)
{
	return _cv200_hi3518e_aenc_init();
}

static void __exit aenc_mod_exit(void)
{
	_cv200_hi3518e_aenc_exit();
}

module_init(aenc_mod_init);
module_exit(aenc_mod_exit);
MODULE_LICENSE("Proprietary");
