#include <linux/module.h>

extern int _cv200_hifb_init(void);
extern void _cv200_hifb_exit(void);

static int __init hifb_mod_init(void)
{
	return _cv200_hifb_init();
}

static void __exit hifb_mod_exit(void)
{
	_cv200_hifb_exit();
}

module_init(hifb_mod_init);
module_exit(hifb_mod_exit);
MODULE_LICENSE("Proprietary");
