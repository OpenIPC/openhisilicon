#include <linux/module.h>
extern int _cv100_hi3518_isp_init(void);
extern void _cv100_hi3518_isp_exit(void);
static int __init isp_mod_init(void) { return _cv100_hi3518_isp_init(); }
static void __exit isp_mod_exit(void) { _cv100_hi3518_isp_exit(); }
module_init(isp_mod_init);
module_exit(isp_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hi3518_isp.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(g_stIspExpFunc);			EXPORT_SYMBOL(g_stIspExpFunc);
