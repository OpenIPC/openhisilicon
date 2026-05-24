#include <linux/module.h>
extern int _cv100_hi3518_vou_init(void);
extern void _cv100_hi3518_vou_exit(void);
static int __init vou_mod_init(void) { return _cv100_hi3518_vou_init(); }
static void __exit vou_mod_exit(void) { _cv100_hi3518_vou_exit(); }
module_init(vou_mod_init);
module_exit(vou_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hi3518_vou.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(VOU_GraphicsBindDev);		EXPORT_SYMBOL(VOU_GraphicsBindDev);
DECLARE_BLOB_FUNC(VOU_GraphicsCloseLayer);	EXPORT_SYMBOL(VOU_GraphicsCloseLayer);
DECLARE_BLOB_FUNC(VOU_GraphicsDeInit);		EXPORT_SYMBOL(VOU_GraphicsDeInit);
DECLARE_BLOB_FUNC(VOU_GraphicsEnableLayer);	EXPORT_SYMBOL(VOU_GraphicsEnableLayer);
DECLARE_BLOB_FUNC(VOU_GraphicsGetDevMode);	EXPORT_SYMBOL(VOU_GraphicsGetDevMode);
DECLARE_BLOB_FUNC(VOU_GraphicsGetDevStatus);	EXPORT_SYMBOL(VOU_GraphicsGetDevStatus);
DECLARE_BLOB_FUNC(VOU_GraphicsGetGfxAddr);	EXPORT_SYMBOL(VOU_GraphicsGetGfxAddr);
DECLARE_BLOB_FUNC(VOU_GraphicsGetGfxPreMult);	EXPORT_SYMBOL(VOU_GraphicsGetGfxPreMult);
DECLARE_BLOB_FUNC(VOU_GraphicsGetGfxStride);	EXPORT_SYMBOL(VOU_GraphicsGetGfxStride);
DECLARE_BLOB_FUNC(VOU_GraphicsGetIntfSize);	EXPORT_SYMBOL(VOU_GraphicsGetIntfSize);
DECLARE_BLOB_FUNC(VOU_GraphicsGetLayerDataFmt);	EXPORT_SYMBOL(VOU_GraphicsGetLayerDataFmt);
DECLARE_BLOB_FUNC(VOU_GraphicsGetLayerGalpha);	EXPORT_SYMBOL(VOU_GraphicsGetLayerGalpha);
DECLARE_BLOB_FUNC(VOU_GraphicsInit);			EXPORT_SYMBOL(VOU_GraphicsInit);
DECLARE_BLOB_FUNC(VOU_GraphicsIntProcess);	EXPORT_SYMBOL(VOU_GraphicsIntProcess);
DECLARE_BLOB_FUNC(VOU_GraphicsOpenLayer);		EXPORT_SYMBOL(VOU_GraphicsOpenLayer);
DECLARE_BLOB_FUNC(VOU_GraphicsSetColorKeyMask);	EXPORT_SYMBOL(VOU_GraphicsSetColorKeyMask);
DECLARE_BLOB_FUNC(VOU_GraphicsSetColorKeyValue);	EXPORT_SYMBOL(VOU_GraphicsSetColorKeyValue);
DECLARE_BLOB_FUNC(VOU_GraphicsSetCscCoef);	EXPORT_SYMBOL(VOU_GraphicsSetCscCoef);
DECLARE_BLOB_FUNC(VOU_GraphicsSetCscEn);		EXPORT_SYMBOL(VOU_GraphicsSetCscEn);
DECLARE_BLOB_FUNC(VOU_GraphicsSetDcmpInfo);	EXPORT_SYMBOL(VOU_GraphicsSetDcmpInfo);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxAddr);	EXPORT_SYMBOL(VOU_GraphicsSetGfxAddr);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxDcmpEnable);	EXPORT_SYMBOL(VOU_GraphicsSetGfxDcmpEnable);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxExt);		EXPORT_SYMBOL(VOU_GraphicsSetGfxExt);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxKeyEn);	EXPORT_SYMBOL(VOU_GraphicsSetGfxKeyEn);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxKeyMode);	EXPORT_SYMBOL(VOU_GraphicsSetGfxKeyMode);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxPalpha);	EXPORT_SYMBOL(VOU_GraphicsSetGfxPalpha);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxPreMult);	EXPORT_SYMBOL(VOU_GraphicsSetGfxPreMult);
DECLARE_BLOB_FUNC(VOU_GraphicsSetGfxStride);	EXPORT_SYMBOL(VOU_GraphicsSetGfxStride);
DECLARE_BLOB_FUNC(VOU_GraphicsSetLayerDataFmt);	EXPORT_SYMBOL(VOU_GraphicsSetLayerDataFmt);
DECLARE_BLOB_FUNC(VOU_GraphicsSetLayerGalpha);	EXPORT_SYMBOL(VOU_GraphicsSetLayerGalpha);
DECLARE_BLOB_FUNC(VOU_GraphicsSetLayerInRect);	EXPORT_SYMBOL(VOU_GraphicsSetLayerInRect);
DECLARE_BLOB_FUNC(VOU_GraphicsSetLayerOutRect);	EXPORT_SYMBOL(VOU_GraphicsSetLayerOutRect);
DECLARE_BLOB_FUNC(VOU_GraphicsSetRegUp);		EXPORT_SYMBOL(VOU_GraphicsSetRegUp);
DECLARE_BLOB_FUNC(VOU_GraphicsSetVoCallback);	EXPORT_SYMBOL(VOU_GraphicsSetVoCallback);
DECLARE_BLOB_FUNC(VOU_GraphicsUnBindDev);		EXPORT_SYMBOL(VOU_GraphicsUnBindDev);
