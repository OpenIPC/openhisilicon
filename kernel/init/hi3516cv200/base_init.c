/*
 * Init wrapper for hi3518e_base blob module (hi3516cv200 / V2 family).
 *
 * The blob is a pre-built .o with init_module/cleanup_module renamed to
 * _cv200_hi3518e_base_init/_cv200_hi3518e_base_exit via objcopy.
 * This wrapper provides module_init/module_exit and re-exports symbols
 * that other modules depend on.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>

/* Blob entry points (renamed from init_module/cleanup_module via objcopy) */
extern int _cv200_hi3518e_base_init(void);
extern void _cv200_hi3518e_base_exit(void);

/*
 * Opaque extern declarations for blob-internal symbols.
 * The actual types live in the blob; we only need declarations
 * so EXPORT_SYMBOL's typeof() resolves.
 */
#define DECLARE_BLOB_SYM(sym) extern char sym[]
#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* CMPI module management */
DECLARE_BLOB_FUNC(CMPI_RegisterModule);	EXPORT_SYMBOL(CMPI_RegisterModule);
DECLARE_BLOB_FUNC(CMPI_UnRegisterModule);	EXPORT_SYMBOL(CMPI_UnRegisterModule);
DECLARE_BLOB_FUNC(CMPI_GetModuleName);		EXPORT_SYMBOL(CMPI_GetModuleName);
DECLARE_BLOB_FUNC(CMPI_GetModuleById);		EXPORT_SYMBOL(CMPI_GetModuleById);
DECLARE_BLOB_FUNC(CMPI_GetModuleFuncById);	EXPORT_SYMBOL(CMPI_GetModuleFuncById);
DECLARE_BLOB_FUNC(CMPI_StopModules);		EXPORT_SYMBOL(CMPI_StopModules);
DECLARE_BLOB_FUNC(CMPI_QueryModules);		EXPORT_SYMBOL(CMPI_QueryModules);
DECLARE_BLOB_FUNC(CMPI_ExitModules);		EXPORT_SYMBOL(CMPI_ExitModules);
DECLARE_BLOB_FUNC(CMPI_InitModules);		EXPORT_SYMBOL(CMPI_InitModules);

/* CMPI memory */
DECLARE_BLOB_FUNC(CMPI_MmzMalloc);		EXPORT_SYMBOL(CMPI_MmzMalloc);
DECLARE_BLOB_FUNC(CMPI_MmzFree);		EXPORT_SYMBOL(CMPI_MmzFree);
DECLARE_BLOB_FUNC(CMPI_MmzMallocNocache);	EXPORT_SYMBOL(CMPI_MmzMallocNocache);
DECLARE_BLOB_FUNC(CMPI_MmzMallocCached);	EXPORT_SYMBOL(CMPI_MmzMallocCached);
DECLARE_BLOB_FUNC(CMPI_UserCopy);		EXPORT_SYMBOL(CMPI_UserCopy);

/* CMPI proc / device */
DECLARE_BLOB_FUNC(CMPI_CreateProc);		EXPORT_SYMBOL(CMPI_CreateProc);
DECLARE_BLOB_FUNC(CMPI_RemoveProc);		EXPORT_SYMBOL(CMPI_RemoveProc);
DECLARE_BLOB_FUNC(CMPI_RegisterDevice);	EXPORT_SYMBOL(CMPI_RegisterDevice);
DECLARE_BLOB_FUNC(CMPI_UnRegisterDevice);	EXPORT_SYMBOL(CMPI_UnRegisterDevice);
DECLARE_BLOB_FUNC(CMPI_LogInit);		EXPORT_SYMBOL(CMPI_LogInit);
DECLARE_BLOB_FUNC(CMPI_LogExit);		EXPORT_SYMBOL(CMPI_LogExit);

/* Logging */
extern int HI_LOG(int level, int mod_id, const char *fmt, ...);
EXPORT_SYMBOL(HI_LOG);
DECLARE_BLOB_FUNC(HI_ChkLogLevel);		EXPORT_SYMBOL(HI_ChkLogLevel);
extern int g_proc_enable;			EXPORT_SYMBOL(g_proc_enable);
DECLARE_BLOB_SYM(g_proc_all);			EXPORT_SYMBOL(g_proc_all);
DECLARE_BLOB_SYM(g_power_save_enable);		EXPORT_SYMBOL(g_power_save_enable);

/* Video buffer */
DECLARE_BLOB_FUNC(VB_Init);			EXPORT_SYMBOL(VB_Init);
DECLARE_BLOB_FUNC(VB_Exit);			EXPORT_SYMBOL(VB_Exit);
DECLARE_BLOB_FUNC(VB_CreatePool);		EXPORT_SYMBOL(VB_CreatePool);
DECLARE_BLOB_FUNC(VB_CreatePoolExt);		EXPORT_SYMBOL(VB_CreatePoolExt);
DECLARE_BLOB_FUNC(VB_DestroyPool);		EXPORT_SYMBOL(VB_DestroyPool);
DECLARE_BLOB_FUNC(VB_GetBlkByPoolId);		EXPORT_SYMBOL(VB_GetBlkByPoolId);
DECLARE_BLOB_FUNC(VB_GetBlkBySize);		EXPORT_SYMBOL(VB_GetBlkBySize);
DECLARE_BLOB_FUNC(VB_GetBlkBySizeAndModule);	EXPORT_SYMBOL(VB_GetBlkBySizeAndModule);
DECLARE_BLOB_FUNC(VB_PutBlk);			EXPORT_SYMBOL(VB_PutBlk);
DECLARE_BLOB_FUNC(VB_Phy2Handle);		EXPORT_SYMBOL(VB_Phy2Handle);
DECLARE_BLOB_FUNC(VB_Handle2Phys);		EXPORT_SYMBOL(VB_Handle2Phys);
DECLARE_BLOB_FUNC(VB_Handle2Kern);		EXPORT_SYMBOL(VB_Handle2Kern);
DECLARE_BLOB_FUNC(VB_Handle2PoolId);		EXPORT_SYMBOL(VB_Handle2PoolId);
DECLARE_BLOB_FUNC(VB_Handle2BlkId);		EXPORT_SYMBOL(VB_Handle2BlkId);
DECLARE_BLOB_FUNC(VB_Handle2BlkSize);		EXPORT_SYMBOL(VB_Handle2BlkSize);
DECLARE_BLOB_FUNC(VB_Handle2Supplement);	EXPORT_SYMBOL(VB_Handle2Supplement);
DECLARE_BLOB_FUNC(VB_InquirePoolUserCnt);	EXPORT_SYMBOL(VB_InquirePoolUserCnt);
DECLARE_BLOB_FUNC(VB_InquirePool);		EXPORT_SYMBOL(VB_InquirePool);
DECLARE_BLOB_FUNC(VB_InquireUserCnt);		EXPORT_SYMBOL(VB_InquireUserCnt);
DECLARE_BLOB_FUNC(VB_InquireOneUserCnt);	EXPORT_SYMBOL(VB_InquireOneUserCnt);
DECLARE_BLOB_FUNC(VB_InquireBlkCnt);		EXPORT_SYMBOL(VB_InquireBlkCnt);
DECLARE_BLOB_FUNC(VB_InquirePoolBusyBlkCnt);	EXPORT_SYMBOL(VB_InquirePoolBusyBlkCnt);
DECLARE_BLOB_FUNC(VB_UserAdd);			EXPORT_SYMBOL(VB_UserAdd);
DECLARE_BLOB_FUNC(VB_UserSub);			EXPORT_SYMBOL(VB_UserSub);
DECLARE_BLOB_FUNC(VB_IsBlkValid);		EXPORT_SYMBOL(VB_IsBlkValid);
DECLARE_BLOB_FUNC(VB_IsSupplementSupport);	EXPORT_SYMBOL(VB_IsSupplementSupport);
DECLARE_BLOB_FUNC(VB_GetPoolInfo);		EXPORT_SYMBOL(VB_GetPoolInfo);
DECLARE_BLOB_FUNC(VB_CopySupplement);		EXPORT_SYMBOL(VB_CopySupplement);
DECLARE_BLOB_FUNC(VB_AddBlkToPool);		EXPORT_SYMBOL(VB_AddBlkToPool);
DECLARE_BLOB_FUNC(VbGetSupplementConf);		EXPORT_SYMBOL(VbGetSupplementConf);

unsigned int vb_force_exit;
EXPORT_SYMBOL(vb_force_exit);
module_param(vb_force_exit, uint, S_IRUGO);

/******* create node  /proc/sys/dev/debug/proc_message_enable ************/
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

static int __init base_mod_init(void)
{
	int ret;

	ret = _cv200_hi3518e_base_init();
	if (ret)
		return ret;

	comm_eproc_tbl_head = register_sysctl_table(comm_parent_tbl);
	if (!comm_eproc_tbl_head) {
		_cv200_hi3518e_base_exit();
		return -ENOMEM;
	}
	return 0;
}

static void __exit base_mod_exit(void)
{
	if (comm_eproc_tbl_head)
		unregister_sysctl_table(comm_eproc_tbl_head);
	_cv200_hi3518e_base_exit();
}

module_init(base_mod_init);
module_exit(base_mod_exit);
MODULE_LICENSE("Proprietary");
