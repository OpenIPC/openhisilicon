#include <linux/module.h>

extern int _cv200_hi3518e_ao_init(void);
extern void _cv200_hi3518e_ao_exit(void);



static int __init ao_mod_init(void)
{
	return _cv200_hi3518e_ao_init();
}

static void __exit ao_mod_exit(void)
{
	_cv200_hi3518e_ao_exit();
}

module_init(ao_mod_init);
module_exit(ao_mod_exit);
MODULE_LICENSE("Proprietary");
