#include <linux/module.h>

extern int _cv200_acodec_init(void);
extern void _cv200_acodec_exit(void);



static int __init acodec_mod_init(void)
{
	return _cv200_acodec_init();
}

static void __exit acodec_mod_exit(void)
{
	_cv200_acodec_exit();
}

module_init(acodec_mod_init);
module_exit(acodec_mod_exit);
MODULE_LICENSE("Proprietary");
