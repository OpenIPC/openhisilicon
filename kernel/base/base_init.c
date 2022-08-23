#include <linux/module.h>

#include "type.h"
#include "common.h"
#include "mod_ext.h"

#define EXPORT_ALIAS(sym, alias)                                            \
	extern typeof(sym) sym;                                             \
	__CRC_SYMBOL(sym, "")                                               \
	static const char __kstrtab_##alias[]                               \
		__attribute__((section("__ksymtab_strings"), aligned(1))) = \
			VMLINUX_SYMBOL_STR(alias);                          \
	static const struct kernel_symbol __ksymtab_##alias __used          \
		__attribute__((section("___ksymtab+" #alias), used)) = {    \
			(unsigned long)&sym, __kstrtab_##alias              \
		}

extern int HI_LOG(GK_S32 s32Level, MOD_ID_E enModId, const char *fmt, ...);
EXPORT_SYMBOL(HI_LOG);
EXPORT_ALIAS(HI_LOG, LOG);

extern GK_U32 CMPI_MmzMalloc(GK_CHAR *pMmzName, GK_CHAR *pBufName,
			     GK_UL ulSize);
EXPORT_SYMBOL(CMPI_MmzMalloc);
extern GK_VOID CMPI_MmzFree(GK_U64 u64PhyAddr, GK_VOID *pVirtAddr);
EXPORT_SYMBOL(CMPI_MmzFree);
extern GK_S32 CMPI_MmzMallocNocache(GK_CHAR *cpMmzName, GK_CHAR *pBufName,
				    GK_U32 *pu32PhyAddr, GK_VOID **ppVitAddr,
				    GK_UL ulLen);
EXPORT_SYMBOL(CMPI_MmzMallocNocache);
extern GK_S32 CMPI_MmzMallocCached(GK_CHAR *cpMmzName, GK_CHAR *pBufName,
				   GK_U32 *pu32PhyAddr, GK_VOID **ppVitAddr,
				   GK_UL ulLen);
EXPORT_SYMBOL(CMPI_MmzMallocCached);

extern GK_VOID *CMPI_Remap_Cached(GK_U64 u64PhyAddr, GK_UL ulSize);
EXPORT_SYMBOL(CMPI_Remap_Cached);

extern GK_VOID *CMPI_Remap_Nocache(GK_U64 u64PhyAddr, GK_UL ulSize);
EXPORT_SYMBOL(CMPI_Remap_Nocache);

extern GK_VOID CMPI_Unmap(GK_VOID *pVirtAddr);
EXPORT_SYMBOL(CMPI_Unmap);

extern GK_U32 vb_force_exit;
module_param(vb_force_exit, uint, S_IRUGO);

/*************************MOD********************/
EXPORT_SYMBOL(CMPI_GetModuleName);
EXPORT_SYMBOL(CMPI_GetModuleById);
EXPORT_SYMBOL(CMPI_GetModuleFuncById);
EXPORT_SYMBOL(CMPI_StopModules);
EXPORT_SYMBOL(CMPI_QueryModules);
EXPORT_SYMBOL(CMPI_ExitModules);
EXPORT_SYMBOL(CMPI_InitModules);
EXPORT_SYMBOL(CMPI_RegisterModule);
EXPORT_SYMBOL(CMPI_UnRegisterModule);

/******* create node  /proc/sys/dev/debug/proc_message_enable ************/
extern GK_S32 g_proc_enable;
EXPORT_SYMBOL(g_proc_enable);

static struct ctl_table comm_eproc_table[] = {
	{ .procname = "proc_message_enable",
	  .data = &g_proc_enable,
	  .maxlen = sizeof(g_proc_enable),
	  .mode = 0644,
	  .proc_handler = proc_dointvec },
	{}
};

static struct ctl_table comm_dir_table[] = {
	{ .procname = "debug", .mode = 0555, .child = comm_eproc_table },
	{}
};

static struct ctl_table comm_parent_tbl[] = {
	{ .procname = "dev", .mode = 0555, .child = comm_dir_table },
	{}
};

static struct ctl_table_header *comm_eproc_tbl_head;

int __init COMM_init_proc_ctrl(void)
{
	comm_eproc_tbl_head = register_sysctl_table(comm_parent_tbl);
	if (!comm_eproc_tbl_head)
		return -ENOMEM;
	return 0;
}

void COMM_exit_proc_ctrl(void)
{
	unregister_sysctl_table(comm_eproc_tbl_head);
}

extern int COMM_Init(void);
extern void COMM_Exit(void);

static int __init base_mod_init(void)
{
	COMM_Init();

	/* init proc switch */
	if (GK_SUCCESS != COMM_init_proc_ctrl()) {
		COMM_Exit();
		return -1;
	}

	return 0;
}
static void __exit base_mod_exit(void)
{
	COMM_exit_proc_ctrl();
	COMM_Exit();
}

module_init(base_mod_init);
module_exit(base_mod_exit);

MODULE_LICENSE("GPL");
