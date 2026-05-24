#include <linux/module.h>
extern int _cv100_hi3518_sio_init(void);
extern void _cv100_hi3518_sio_exit(void);
static int __init sio_mod_init(void) { return _cv100_hi3518_sio_init(); }
static void __exit sio_mod_exit(void) { _cv100_hi3518_sio_exit(); }
module_init(sio_mod_init);
module_exit(sio_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hi3518_sio.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(AiCheckCodecNoise);			EXPORT_SYMBOL(AiCheckCodecNoise);
DECLARE_BLOB_FUNC(AiGetDMAChn);				EXPORT_SYMBOL(AiGetDMAChn);
DECLARE_BLOB_FUNC(AoGetDevId);				EXPORT_SYMBOL(AoGetDevId);
DECLARE_BLOB_FUNC(AoGetDMAChn);				EXPORT_SYMBOL(AoGetDMAChn);
DECLARE_BLOB_FUNC(SIO_GetIntStatus);			EXPORT_SYMBOL(SIO_GetIntStatus);
