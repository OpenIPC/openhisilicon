#include <linux/module.h>
extern int _cv100_hiuser_init(void);
extern void _cv100_hiuser_exit(void);
static int __init hiuser_mod_init(void) { return _cv100_hiuser_init(); }
static void __exit hiuser_mod_exit(void) { _cv100_hiuser_exit(); }
module_init(hiuser_mod_init);
module_exit(hiuser_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hiuser.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(hil_mmb_Invalid_Cache_L1L2_byaddr);	EXPORT_SYMBOL(hil_mmb_Invalid_Cache_L1L2_byaddr);
DECLARE_BLOB_FUNC(hi_sched_setscheduler);		EXPORT_SYMBOL(hi_sched_setscheduler);
DECLARE_BLOB_FUNC(hi_user_notify_vdec_event);	EXPORT_SYMBOL(hi_user_notify_vdec_event);
