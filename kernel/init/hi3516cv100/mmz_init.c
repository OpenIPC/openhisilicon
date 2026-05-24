#include <linux/module.h>
extern int _cv100_mmz_init(void);
extern void _cv100_mmz_exit(void);
static int __init mmz_mod_init(void) { return _cv100_mmz_init(); }
static void __exit mmz_mod_exit(void) { _cv100_mmz_exit(); }
module_init(mmz_mod_init);
module_exit(mmz_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from mmz.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(delete_mmb);				EXPORT_SYMBOL(delete_mmb);
DECLARE_BLOB_FUNC(flush_mmb_byaddr);			EXPORT_SYMBOL(flush_mmb_byaddr);
DECLARE_BLOB_FUNC(hil_mmb_alloc);				EXPORT_SYMBOL(hil_mmb_alloc);
DECLARE_BLOB_FUNC(hil_mmb_alloc_v2);			EXPORT_SYMBOL(hil_mmb_alloc_v2);
DECLARE_BLOB_FUNC(hil_mmb_flush_dcache_byaddr);	EXPORT_SYMBOL(hil_mmb_flush_dcache_byaddr);
DECLARE_BLOB_FUNC(hil_mmb_free);				EXPORT_SYMBOL(hil_mmb_free);
DECLARE_BLOB_FUNC(hil_mmb_getby_kvirt);		EXPORT_SYMBOL(hil_mmb_getby_kvirt);
DECLARE_BLOB_FUNC(hil_mmb_getby_phys);		EXPORT_SYMBOL(hil_mmb_getby_phys);
DECLARE_BLOB_FUNC(hil_mmb_getby_phys_2);		EXPORT_SYMBOL(hil_mmb_getby_phys_2);
DECLARE_BLOB_FUNC(hil_mmb_map2kern);			EXPORT_SYMBOL(hil_mmb_map2kern);
DECLARE_BLOB_FUNC(hil_mmb_map2kern_cached);	EXPORT_SYMBOL(hil_mmb_map2kern_cached);
DECLARE_BLOB_FUNC(hil_mmb_unmap);				EXPORT_SYMBOL(hil_mmb_unmap);
DECLARE_BLOB_FUNC(hil_mmz_create);			EXPORT_SYMBOL(hil_mmz_create);
DECLARE_BLOB_FUNC(hil_mmz_destroy);			EXPORT_SYMBOL(hil_mmz_destroy);
DECLARE_BLOB_FUNC(new_mmb);					EXPORT_SYMBOL(new_mmb);
DECLARE_BLOB_FUNC(remap_mmb);					EXPORT_SYMBOL(remap_mmb);
DECLARE_BLOB_FUNC(remap_mmb_cached);			EXPORT_SYMBOL(remap_mmb_cached);
DECLARE_BLOB_FUNC(unmap_mmb);					EXPORT_SYMBOL(unmap_mmb);
