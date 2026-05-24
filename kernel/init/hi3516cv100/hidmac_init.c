#include <linux/module.h>
extern int _cv100_hidmac_init(void);
extern void _cv100_hidmac_exit(void);
static int __init hidmac_mod_init(void) { return _cv100_hidmac_init(); }
static void __exit hidmac_mod_exit(void) { _cv100_hidmac_exit(); }
module_init(hidmac_mod_init);
module_exit(hidmac_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hidmac.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(allocate_dmalli_space);		EXPORT_SYMBOL(allocate_dmalli_space);
DECLARE_BLOB_FUNC(dmac_buildllim2m);			EXPORT_SYMBOL(dmac_buildllim2m);
DECLARE_BLOB_FUNC(dmac_buildllim2p);			EXPORT_SYMBOL(dmac_buildllim2p);
DECLARE_BLOB_FUNC(dmac_channel_allocate);		EXPORT_SYMBOL(dmac_channel_allocate);
DECLARE_BLOB_FUNC(dmac_channelclose);			EXPORT_SYMBOL(dmac_channelclose);
DECLARE_BLOB_FUNC(dmac_channel_free);			EXPORT_SYMBOL(dmac_channel_free);
DECLARE_BLOB_FUNC(dmac_channelstart);			EXPORT_SYMBOL(dmac_channelstart);
DECLARE_BLOB_FUNC(dmac_register_isr);			EXPORT_SYMBOL(dmac_register_isr);
DECLARE_BLOB_FUNC(dmac_start_llim2m);			EXPORT_SYMBOL(dmac_start_llim2m);
DECLARE_BLOB_FUNC(dmac_start_llim2p);			EXPORT_SYMBOL(dmac_start_llim2p);
DECLARE_BLOB_FUNC(dmac_start_m2m);			EXPORT_SYMBOL(dmac_start_m2m);
DECLARE_BLOB_FUNC(dmac_start_m2p);			EXPORT_SYMBOL(dmac_start_m2p);
DECLARE_BLOB_FUNC(dmac_wait);					EXPORT_SYMBOL(dmac_wait);
DECLARE_BLOB_FUNC(free_dmalli_space);			EXPORT_SYMBOL(free_dmalli_space);
