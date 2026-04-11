#include <linux/module.h>

extern int _cv200_hi3518e_venc_init(void);
extern void _cv200_hi3518e_venc_exit(void);

extern char VencBufferCache[];	EXPORT_SYMBOL(VencBufferCache);

static int __init venc_mod_init(void)
{
	return _cv200_hi3518e_venc_init();
}

static void __exit venc_mod_exit(void)
{
	_cv200_hi3518e_venc_exit();
}

module_init(venc_mod_init);
module_exit(venc_mod_exit);
MODULE_LICENSE("Proprietary");
