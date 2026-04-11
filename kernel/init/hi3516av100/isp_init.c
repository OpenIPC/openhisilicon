#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_isp_init(void);
extern void _av100_hi3516a_isp_exit(void);

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)
#define DECLARE_BLOB_SYM(sym) extern char sym[]

DECLARE_BLOB_SYM(g_stIspExpFunc);		EXPORT_SYMBOL(g_stIspExpFunc);
DECLARE_BLOB_SYM(g_stIspPirisExpFunc);		EXPORT_SYMBOL(g_stIspPirisExpFunc);
DECLARE_BLOB_FUNC(hi_isp_sync_task_register);	EXPORT_SYMBOL(hi_isp_sync_task_register);
DECLARE_BLOB_FUNC(hi_isp_sync_task_unregister);	EXPORT_SYMBOL(hi_isp_sync_task_unregister);
DECLARE_BLOB_FUNC(ISP_GetDCFInfo);		EXPORT_SYMBOL(ISP_GetDCFInfo);

static int __init isp_mod_init(void) { return _av100_hi3516a_isp_init(); }
static void __exit isp_mod_exit(void) { _av100_hi3516a_isp_exit(); }
module_init(isp_mod_init);
module_exit(isp_mod_exit);
MODULE_LICENSE("GPL");
