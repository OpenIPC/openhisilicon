#include <linux/module.h>
extern int _cv100_hi3518_venc_init(void);
extern void _cv100_hi3518_venc_exit(void);
#define DECLARE_BLOB_FUNC(sym) extern int sym(void)
DECLARE_BLOB_FUNC(VencQuery);		EXPORT_SYMBOL(VencQuery);
DECLARE_BLOB_FUNC(VencReleaseStreamEx);	EXPORT_SYMBOL(VencReleaseStreamEx);
DECLARE_BLOB_FUNC(VencGetStreamEx);	EXPORT_SYMBOL(VencGetStreamEx);
extern char VencBufferCache[];		EXPORT_SYMBOL(VencBufferCache);
static int __init venc_mod_init(void) { return _cv100_hi3518_venc_init(); }
static void __exit venc_mod_exit(void) { _cv100_hi3518_venc_exit(); }
module_init(venc_mod_init);
module_exit(venc_mod_exit);
MODULE_LICENSE("GPL");
