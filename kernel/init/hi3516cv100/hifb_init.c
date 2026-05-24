#include <linux/module.h>
extern int _cv100_hifb_init(void);
extern void _cv100_hifb_exit(void);
static int __init hifb_mod_init(void) { return _cv100_hifb_init(); }
static void __exit hifb_mod_exit(void) { _cv100_hifb_exit(); }
module_init(hifb_mod_init);
module_exit(hifb_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hifb.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(HIFB_DRV_GetOps);			EXPORT_SYMBOL(HIFB_DRV_GetOps);
