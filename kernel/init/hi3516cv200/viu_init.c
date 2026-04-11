#include <linux/module.h>

extern int _cv200_hi3518e_viu_init(void);
extern void _cv200_hi3518e_viu_exit(void);

#define DECLARE_BLOB_SYM(sym) extern char sym[]
#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

DECLARE_BLOB_SYM(g_stViuExpFunc);		EXPORT_SYMBOL(g_stViuExpFunc);
DECLARE_BLOB_FUNC(VIU_DRV_GetPrivateData);	EXPORT_SYMBOL(VIU_DRV_GetPrivateData);
DECLARE_BLOB_FUNC(VIU_DRV_IsFrameFlashed);	EXPORT_SYMBOL(VIU_DRV_IsFrameFlashed);

static int __init viu_mod_init(void)
{
	return _cv200_hi3518e_viu_init();
}

static void __exit viu_mod_exit(void)
{
	_cv200_hi3518e_viu_exit();
}

module_init(viu_mod_init);
module_exit(viu_mod_exit);
MODULE_LICENSE("Proprietary");
