#include <linux/module.h>

extern int _cv200_hi3518e_tde_init(void);
extern void _cv200_hi3518e_tde_exit(void);

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

DECLARE_BLOB_FUNC(tde_init_module_k);		EXPORT_SYMBOL(tde_init_module_k);
DECLARE_BLOB_FUNC(tde_cleanup_module_k);	EXPORT_SYMBOL(tde_cleanup_module_k);
DECLARE_BLOB_FUNC(tde_open);			EXPORT_SYMBOL(tde_open);
DECLARE_BLOB_FUNC(tde_release);			EXPORT_SYMBOL(tde_release);
DECLARE_BLOB_FUNC(tde_ioctl);			EXPORT_SYMBOL(tde_ioctl);
DECLARE_BLOB_FUNC(tde_read_proc);		EXPORT_SYMBOL(tde_read_proc);
DECLARE_BLOB_FUNC(tde_write_proc);		EXPORT_SYMBOL(tde_write_proc);
DECLARE_BLOB_FUNC(tde_osr_enableirq);		EXPORT_SYMBOL(tde_osr_enableirq);
DECLARE_BLOB_FUNC(tde_osr_disableirq);		EXPORT_SYMBOL(tde_osr_disableirq);
DECLARE_BLOB_FUNC(TdeCalScaleRect);		EXPORT_SYMBOL(TdeCalScaleRect);
DECLARE_BLOB_FUNC(TdeOsiOpen);			EXPORT_SYMBOL(TdeOsiOpen);
DECLARE_BLOB_FUNC(TdeOsiClose);			EXPORT_SYMBOL(TdeOsiClose);
DECLARE_BLOB_FUNC(TdeOsiBeginJob);		EXPORT_SYMBOL(TdeOsiBeginJob);
DECLARE_BLOB_FUNC(TdeOsiEndJob);		EXPORT_SYMBOL(TdeOsiEndJob);
DECLARE_BLOB_FUNC(TdeOsiCancelJob);		EXPORT_SYMBOL(TdeOsiCancelJob);
DECLARE_BLOB_FUNC(TdeOsiWaitForDone);		EXPORT_SYMBOL(TdeOsiWaitForDone);
DECLARE_BLOB_FUNC(TdeOsiWaitAllDone);		EXPORT_SYMBOL(TdeOsiWaitAllDone);
DECLARE_BLOB_FUNC(TdeOsiReset);			EXPORT_SYMBOL(TdeOsiReset);
DECLARE_BLOB_FUNC(TdeOsiQuickCopy);		EXPORT_SYMBOL(TdeOsiQuickCopy);
DECLARE_BLOB_FUNC(TdeOsiQuickFill);		EXPORT_SYMBOL(TdeOsiQuickFill);
DECLARE_BLOB_FUNC(TdeOsiQuickResize);		EXPORT_SYMBOL(TdeOsiQuickResize);
DECLARE_BLOB_FUNC(TdeOsiQuickFlicker);		EXPORT_SYMBOL(TdeOsiQuickFlicker);
DECLARE_BLOB_FUNC(TdeOsiBlit);			EXPORT_SYMBOL(TdeOsiBlit);
DECLARE_BLOB_FUNC(TdeOsiMbBlit);		EXPORT_SYMBOL(TdeOsiMbBlit);
DECLARE_BLOB_FUNC(TdeOsiSolidDraw);		EXPORT_SYMBOL(TdeOsiSolidDraw);
DECLARE_BLOB_FUNC(TdeOsiBitmapMaskRop);	EXPORT_SYMBOL(TdeOsiBitmapMaskRop);
DECLARE_BLOB_FUNC(TdeOsiBitmapMaskBlend);	EXPORT_SYMBOL(TdeOsiBitmapMaskBlend);
DECLARE_BLOB_FUNC(TdeOsiPatternFill);		EXPORT_SYMBOL(TdeOsiPatternFill);
DECLARE_BLOB_FUNC(TdeOsiEnableRegionDeflicker);	EXPORT_SYMBOL(TdeOsiEnableRegionDeflicker);
DECLARE_BLOB_FUNC(TdeOsiSetDeflickerLevel);	EXPORT_SYMBOL(TdeOsiSetDeflickerLevel);
DECLARE_BLOB_FUNC(TdeOsiGetDeflickerLevel);	EXPORT_SYMBOL(TdeOsiGetDeflickerLevel);
DECLARE_BLOB_FUNC(TdeOsiSetAlphaThresholdValue); EXPORT_SYMBOL(TdeOsiSetAlphaThresholdValue);
DECLARE_BLOB_FUNC(TdeOsiGetAlphaThresholdValue); EXPORT_SYMBOL(TdeOsiGetAlphaThresholdValue);
DECLARE_BLOB_FUNC(TdeOsiSetAlphaThresholdState); EXPORT_SYMBOL(TdeOsiSetAlphaThresholdState);

static int __init tde_mod_init(void)
{
	return _cv200_hi3518e_tde_init();
}

static void __exit tde_mod_exit(void)
{
	_cv200_hi3518e_tde_exit();
}

module_init(tde_mod_init);
module_exit(tde_mod_exit);
MODULE_LICENSE("Proprietary");
