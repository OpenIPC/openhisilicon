#include <linux/module.h>

extern int _cv200_hi3518e_ive_init(void);
extern void _cv200_hi3518e_ive_exit(void);



static int __init ive_mod_init(void)
{
	return _cv200_hi3518e_ive_init();
}

static void __exit ive_mod_exit(void)
{
	_cv200_hi3518e_ive_exit();
}

module_init(ive_mod_init);
module_exit(ive_mod_exit);
MODULE_LICENSE("Proprietary");
