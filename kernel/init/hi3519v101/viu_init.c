/*
 * V3A vi/viu module init wrapper.
 *
 * Re-exports legacy __ksymtab_ entries from hi3519v101_viu.o that
 * consumer modules (vpss, isp) reference at link time.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>

/* See base_init.c for the >= 5.0 gate rationale. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)


extern int VIU_DRV_GetChnIntStatus(int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_GetChnIntStatus);
extern void VIU_DRV_ClearChnIntStatus(int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_ClearChnIntStatus);
extern void VIU_DRV_ClearChnIntMask(int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_ClearChnIntMask);
extern int VIU_DRV_GetGlobalChnIntMask(void);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_GetGlobalChnIntMask);
extern int VIU_DRV_GetGlobalIntIndicator(void);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_GetGlobalIntIndicator);
extern void *VIU_DRV_GetPrivateData(int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_GetPrivateData);
extern int VIU_DRV_IsFrameFlashed(int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(VIU_DRV_IsFrameFlashed);

extern void *g_psViuAllReg;
COMPAT_REEXPORT_BLOB_SYMBOL(g_psViuAllReg);
extern void *g_stViuExpFunc;
COMPAT_REEXPORT_BLOB_SYMBOL(g_stViuExpFunc);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

extern int _3519v101_hi3519v101_viu_init(void);
extern void _3519v101_hi3519v101_viu_exit(void);

static int __init viu_mod_init(void)
{
	return _3519v101_hi3519v101_viu_init();
}

static void __exit viu_mod_exit(void)
{
	_3519v101_hi3519v101_viu_exit();
}

module_init(viu_mod_init);
module_exit(viu_mod_exit);
MODULE_LICENSE("GPL");
