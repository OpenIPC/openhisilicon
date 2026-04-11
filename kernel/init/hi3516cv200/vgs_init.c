#include <linux/module.h>

extern int _cv200_hi3518e_vgs_init(void);
extern void _cv200_hi3518e_vgs_exit(void);



static int __init vgs_mod_init(void)
{
	return _cv200_hi3518e_vgs_init();
}

static void __exit vgs_mod_exit(void)
{
	_cv200_hi3518e_vgs_exit();
}

module_init(vgs_mod_init);
module_exit(vgs_mod_exit);
MODULE_LICENSE("Proprietary");
