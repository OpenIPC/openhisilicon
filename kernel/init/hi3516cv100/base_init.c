#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>

extern int _cv100_hi3518_base_init(void);
extern void _cv100_hi3518_base_exit(void);

#define DECLARE_BLOB_SYM(sym) extern char sym[]
#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

DECLARE_BLOB_FUNC(CMPI_RegisterMod);		EXPORT_SYMBOL(CMPI_RegisterMod);
DECLARE_BLOB_FUNC(CMPI_UnRegisterMod);		EXPORT_SYMBOL(CMPI_UnRegisterMod);
DECLARE_BLOB_FUNC(CMPI_StopModules);		EXPORT_SYMBOL(CMPI_StopModules);
DECLARE_BLOB_FUNC(CMPI_QueryModules);		EXPORT_SYMBOL(CMPI_QueryModules);
DECLARE_BLOB_FUNC(CMPI_ExitModules);		EXPORT_SYMBOL(CMPI_ExitModules);
DECLARE_BLOB_FUNC(CMPI_InitModules);		EXPORT_SYMBOL(CMPI_InitModules);
DECLARE_BLOB_FUNC(CMPI_MmzMalloc);		EXPORT_SYMBOL(CMPI_MmzMalloc);
DECLARE_BLOB_FUNC(CMPI_MmzFree);		EXPORT_SYMBOL(CMPI_MmzFree);
DECLARE_BLOB_FUNC(CMPI_UserCopy);		EXPORT_SYMBOL(CMPI_UserCopy);
DECLARE_BLOB_FUNC(CMPI_CreateProc);		EXPORT_SYMBOL(CMPI_CreateProc);
DECLARE_BLOB_FUNC(CMPI_RemoveProc);		EXPORT_SYMBOL(CMPI_RemoveProc);
DECLARE_BLOB_FUNC(CMPI_RegisterDevice);	EXPORT_SYMBOL(CMPI_RegisterDevice);
DECLARE_BLOB_FUNC(CMPI_UnRegisterDevice);	EXPORT_SYMBOL(CMPI_UnRegisterDevice);
DECLARE_BLOB_FUNC(CMPI_DevInit);		EXPORT_SYMBOL(CMPI_DevInit);
DECLARE_BLOB_FUNC(CMPI_DevExit);		EXPORT_SYMBOL(CMPI_DevExit);
DECLARE_BLOB_FUNC(CMPI_LogInit);		EXPORT_SYMBOL(CMPI_LogInit);
DECLARE_BLOB_FUNC(CMPI_LogExit);		EXPORT_SYMBOL(CMPI_LogExit);
extern int HI_LOG(int level, int mod_id, const char *fmt, ...);
EXPORT_SYMBOL(HI_LOG);
DECLARE_BLOB_FUNC(HI_ChkLogLevel);		EXPORT_SYMBOL(HI_ChkLogLevel);
extern int g_proc_enable;			EXPORT_SYMBOL(g_proc_enable);
DECLARE_BLOB_SYM(g_aModName);			EXPORT_SYMBOL(g_aModName);
DECLARE_BLOB_SYM(g_astModules);			EXPORT_SYMBOL(g_astModules);
DECLARE_BLOB_FUNC(VB_Init);			EXPORT_SYMBOL(VB_Init);
DECLARE_BLOB_FUNC(VB_Exit);			EXPORT_SYMBOL(VB_Exit);
DECLARE_BLOB_FUNC(VB_CreatePool);		EXPORT_SYMBOL(VB_CreatePool);
DECLARE_BLOB_FUNC(VB_CreatePoolExt);		EXPORT_SYMBOL(VB_CreatePoolExt);
DECLARE_BLOB_FUNC(VB_DestroyPool);		EXPORT_SYMBOL(VB_DestroyPool);
DECLARE_BLOB_FUNC(VB_GetBlkByPoolId);		EXPORT_SYMBOL(VB_GetBlkByPoolId);
DECLARE_BLOB_FUNC(VB_GetBlkBySize);		EXPORT_SYMBOL(VB_GetBlkBySize);
DECLARE_BLOB_FUNC(VB_PutBlk);			EXPORT_SYMBOL(VB_PutBlk);
DECLARE_BLOB_FUNC(VB_Phy2Handle);		EXPORT_SYMBOL(VB_Phy2Handle);
DECLARE_BLOB_FUNC(VB_Handle2Phys);		EXPORT_SYMBOL(VB_Handle2Phys);
DECLARE_BLOB_FUNC(VB_Handle2Kern);		EXPORT_SYMBOL(VB_Handle2Kern);
DECLARE_BLOB_FUNC(VB_Handle2PoolId);		EXPORT_SYMBOL(VB_Handle2PoolId);
DECLARE_BLOB_FUNC(VB_Handle2BlkId);		EXPORT_SYMBOL(VB_Handle2BlkId);
DECLARE_BLOB_FUNC(VB_Handle2BlkSize);		EXPORT_SYMBOL(VB_Handle2BlkSize);
DECLARE_BLOB_FUNC(VB_InquirePoolUserCnt);	EXPORT_SYMBOL(VB_InquirePoolUserCnt);
DECLARE_BLOB_FUNC(VB_InquirePool);		EXPORT_SYMBOL(VB_InquirePool);
DECLARE_BLOB_FUNC(VB_InquireUserCnt);		EXPORT_SYMBOL(VB_InquireUserCnt);
DECLARE_BLOB_FUNC(VB_InquireOneUserCnt);	EXPORT_SYMBOL(VB_InquireOneUserCnt);
DECLARE_BLOB_FUNC(VB_InquireBlkCnt);		EXPORT_SYMBOL(VB_InquireBlkCnt);
DECLARE_BLOB_FUNC(VB_InquirePoolBusyBlkCnt);	EXPORT_SYMBOL(VB_InquirePoolBusyBlkCnt);
DECLARE_BLOB_FUNC(VB_UserAdd);			EXPORT_SYMBOL(VB_UserAdd);
DECLARE_BLOB_FUNC(VB_UserSub);			EXPORT_SYMBOL(VB_UserSub);
DECLARE_BLOB_FUNC(VB_IsBlkValid);		EXPORT_SYMBOL(VB_IsBlkValid);
DECLARE_BLOB_FUNC(VB_GetPoolInfo);		EXPORT_SYMBOL(VB_GetPoolInfo);
DECLARE_BLOB_FUNC(VB_AddBlkToPool);		EXPORT_SYMBOL(VB_AddBlkToPool);

extern unsigned int vb_force_exit;
module_param(vb_force_exit, uint, S_IRUGO);

static int __init base_mod_init(void)
{
	return _cv100_hi3518_base_init();
}
static void __exit base_mod_exit(void)
{
	_cv100_hi3518_base_exit();
}
module_init(base_mod_init);
module_exit(base_mod_exit);
MODULE_LICENSE("GPL");
