#include <linux/module.h>
extern int _cv100_hi3518_rc_init(void);
extern void _cv100_hi3518_rc_exit(void);
static int __init rc_mod_init(void) { return _cv100_hi3518_rc_init(); }
static void __exit rc_mod_exit(void) { _cv100_hi3518_rc_exit(); }
module_init(rc_mod_init);
module_exit(rc_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hi3518_rc.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(dump);						EXPORT_SYMBOL(dump);
DECLARE_BLOB_FUNC(kfile_close);				EXPORT_SYMBOL(kfile_close);
DECLARE_BLOB_FUNC(kfile_open);				EXPORT_SYMBOL(kfile_open);
DECLARE_BLOB_FUNC(kfile_read);				EXPORT_SYMBOL(kfile_read);
DECLARE_BLOB_FUNC(kfile_seek);				EXPORT_SYMBOL(kfile_seek);
DECLARE_BLOB_FUNC(kfile_tell);				EXPORT_SYMBOL(kfile_tell);
DECLARE_BLOB_FUNC(kfile_write);				EXPORT_SYMBOL(kfile_write);
DECLARE_BLOB_FUNC(ksem_down);					EXPORT_SYMBOL(ksem_down);
DECLARE_BLOB_FUNC(ksem_init);					EXPORT_SYMBOL(ksem_init);
DECLARE_BLOB_FUNC(ksem_up);					EXPORT_SYMBOL(ksem_up);
