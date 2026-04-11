#include <linux/module.h>

extern int _cv200_hi3518e_jpege_init(void);
extern void _cv200_hi3518e_jpege_exit(void);



static int __init jpege_mod_init(void)
{
	return _cv200_hi3518e_jpege_init();
}

static void __exit jpege_mod_exit(void)
{
	_cv200_hi3518e_jpege_exit();
}

module_init(jpege_mod_init);
module_exit(jpege_mod_exit);
MODULE_LICENSE("Proprietary");
