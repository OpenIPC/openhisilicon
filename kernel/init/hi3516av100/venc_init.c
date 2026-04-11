#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_venc_init(void);
extern void _av100_hi3516a_venc_exit(void);

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

DECLARE_BLOB_FUNC(VencBufferCache);		EXPORT_SYMBOL(VencBufferCache);

static int __init venc_mod_init(void) { return _av100_hi3516a_venc_init(); }
static void __exit venc_mod_exit(void) { _av100_hi3516a_venc_exit(); }
module_init(venc_mod_init);
module_exit(venc_mod_exit);
MODULE_LICENSE("GPL");
