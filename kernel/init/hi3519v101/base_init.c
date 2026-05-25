/*
 * V3A base module init wrapper.
 *
 * The hi3519v101_base.o blob defines CMPI_, VB_, HI_LOG and g_ symbols
 * via legacy __ksymtab_ entries (8-byte struct kernel_symbol). Modern
 * (Linux 5 and newer) modpost ignores that legacy format, so consumer
 * modules (open_sys, open_isp, ...) would see them as undefined.
 *
 * Re-export the same symbols here via EXPORT_SYMBOL so modpost picks
 * them up via Module.symvers. Pattern mirrors the cv300 init wrapper.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/sysctl.h>
#include <linux/version.h>

/* Pre-5.0 kernels honour the blob's legacy 8-byte __ksymtab_* entries
 * directly; adding our own extern + EXPORT_SYMBOL declarations on top
 * regresses lite-target loading on hardware (blob's exports disappear
 * from /proc/kallsyms when paired with our extern in the same TU).
 * Gate every re-export to >= 5.0 so the 3.18 lite path stays a simple
 * forwarder, bit-for-bit equivalent to the pre-V3A-neo upstream. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)

extern int HI_LOG(int level, int mod_id, const char *fmt, ...);
COMPAT_REEXPORT_BLOB_SYMBOL(HI_LOG);

extern void *CMPI_MmzMalloc(char *mmz_name, char *buf_name, unsigned int size);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_MmzMalloc);
extern void CMPI_MmzFree(unsigned int phy_addr, void *vir_addr);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_MmzFree);
extern int CMPI_MmzMallocNocache(char *cpMmzName, char *pBufName,
				 unsigned int *pu32PhyAddr, void **ppVitAddr,
				 int s32Len);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_MmzMallocNocache);
extern int CMPI_MmzMallocCached(char *cpMmzName, char *pBufName,
				unsigned int *pu32PhyAddr, void **ppVitAddr,
				int s32Len);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_MmzMallocCached);

extern int CMPI_GetModuleName(int mod_id, char *name);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_GetModuleName);
extern int CMPI_GetModuleById(int mod_id);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_GetModuleById);
extern int CMPI_GetModuleFuncById(int mod_id);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_GetModuleFuncById);
extern int CMPI_StopModules(void);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_StopModules);
extern int CMPI_QueryModules(void *info);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_QueryModules);
extern int CMPI_ExitModules(void);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_ExitModules);
extern int CMPI_InitModules(void);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_InitModules);
extern int CMPI_RegisterModule(void *mod_info);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_RegisterModule);
extern int CMPI_UnRegisterModule(int mod_id);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_UnRegisterModule);

extern int CMPI_LogInit(void);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_LogInit);
extern void CMPI_LogExit(void);
COMPAT_REEXPORT_BLOB_SYMBOL(CMPI_LogExit);

extern int g_proc_all;
COMPAT_REEXPORT_BLOB_SYMBOL(g_proc_all);
extern int g_proc_enable;
COMPAT_REEXPORT_BLOB_SYMBOL(g_proc_enable);
extern int g_power_save_enable;
COMPAT_REEXPORT_BLOB_SYMBOL(g_power_save_enable);

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

extern int _3519v101_hi3519v101_base_init(void);
extern void _3519v101_hi3519v101_base_exit(void);

static int __init base_mod_init(void)
{
	return _3519v101_hi3519v101_base_init();
}

static void __exit base_mod_exit(void)
{
	_3519v101_hi3519v101_base_exit();
}

module_init(base_mod_init);
module_exit(base_mod_exit);
MODULE_LICENSE("GPL");
