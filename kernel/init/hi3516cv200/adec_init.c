#include <linux/module.h>

extern int _cv200_hi3518e_adec_init(void);
extern void _cv200_hi3518e_adec_exit(void);



static int __init adec_mod_init(void)
{
	return _cv200_hi3518e_adec_init();
}

static void __exit adec_mod_exit(void)
{
	_cv200_hi3518e_adec_exit();
}

module_init(adec_mod_init);
module_exit(adec_mod_exit);
MODULE_LICENSE("Proprietary");
