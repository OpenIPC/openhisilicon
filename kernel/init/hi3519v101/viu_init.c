#include <linux/module.h>
#include <linux/kernel.h>

extern int _3519v101_hi3519v101_viu_init(void);
extern void _3519v101_hi3519v101_viu_exit(void);

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)
#define DECLARE_BLOB_SYM(sym) extern char sym[]

DECLARE_BLOB_SYM(g_stViuExpFunc);		EXPORT_SYMBOL(g_stViuExpFunc);
DECLARE_BLOB_FUNC(VIU_DRV_GetPrivateData);	EXPORT_SYMBOL(VIU_DRV_GetPrivateData);
DECLARE_BLOB_FUNC(VIU_DRV_IsFrameFlashed);	EXPORT_SYMBOL(VIU_DRV_IsFrameFlashed);
DECLARE_BLOB_FUNC(VIU_DRV_GetGlobalIntIndicator);	EXPORT_SYMBOL(VIU_DRV_GetGlobalIntIndicator);
DECLARE_BLOB_FUNC(VIU_DRV_GetChnIntStatus);	EXPORT_SYMBOL(VIU_DRV_GetChnIntStatus);
DECLARE_BLOB_FUNC(VIU_DRV_GetGlobalChnIntMask);	EXPORT_SYMBOL(VIU_DRV_GetGlobalChnIntMask);
DECLARE_BLOB_FUNC(VIU_DRV_ClearChnIntStatus);	EXPORT_SYMBOL(VIU_DRV_ClearChnIntStatus);
DECLARE_BLOB_FUNC(VIU_DRV_ClearChnIntMask);	EXPORT_SYMBOL(VIU_DRV_ClearChnIntMask);
DECLARE_BLOB_SYM(g_psViuAllReg);		EXPORT_SYMBOL(g_psViuAllReg);
extern unsigned int viu_irq[];			EXPORT_SYMBOL(viu_irq);

static int __init viu_mod_init(void) { return _3519v101_hi3519v101_viu_init(); }
static void __exit viu_mod_exit(void) { _3519v101_hi3519v101_viu_exit(); }
module_init(viu_mod_init);
module_exit(viu_mod_exit);
MODULE_LICENSE("GPL");
