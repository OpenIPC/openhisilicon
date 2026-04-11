#include <linux/module.h>

extern int _cv200_hi3518e_aio_init(void);
extern void _cv200_hi3518e_aio_exit(void);



static int __init aio_mod_init(void)
{
	return _cv200_hi3518e_aio_init();
}

static void __exit aio_mod_exit(void)
{
	_cv200_hi3518e_aio_exit();
}

module_init(aio_mod_init);
module_exit(aio_mod_exit);
MODULE_LICENSE("Proprietary");
