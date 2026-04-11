#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_viu_init(void);
extern void _av100_hi3516a_viu_exit(void);

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)
#define DECLARE_BLOB_SYM(sym) extern char sym[]

DECLARE_BLOB_SYM(g_stViuExpFunc);		EXPORT_SYMBOL(g_stViuExpFunc);
DECLARE_BLOB_FUNC(VIU_DRV_GetPrivateData);	EXPORT_SYMBOL(VIU_DRV_GetPrivateData);
DECLARE_BLOB_FUNC(VIU_DRV_IsFrameFlashed);	EXPORT_SYMBOL(VIU_DRV_IsFrameFlashed);

static int __init viu_mod_init(void) { return _av100_hi3516a_viu_init(); }
static void __exit viu_mod_exit(void) { _av100_hi3516a_viu_exit(); }
module_init(viu_mod_init);
module_exit(viu_mod_exit);
MODULE_LICENSE("GPL");
