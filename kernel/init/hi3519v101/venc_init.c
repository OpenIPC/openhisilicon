#include <linux/module.h>
#include <linux/kernel.h>

extern int VencMaxChnNum;
COMPAT_REEXPORT_BLOB_SYMBOL(VencMaxChnNum);
extern int VencBufferCache;
COMPAT_REEXPORT_BLOB_SYMBOL(VencBufferCache);
extern void FrameBufRecycle(void *ctx);
COMPAT_REEXPORT_BLOB_SYMBOL(FrameBufRecycle);
extern void JpegClearStreamBuf(int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(JpegClearStreamBuf);
extern void *OneBufferForJpegOSD;
COMPAT_REEXPORT_BLOB_SYMBOL(OneBufferForJpegOSD);
extern void QuickSchedule(void);
COMPAT_REEXPORT_BLOB_SYMBOL(QuickSchedule);

extern int _3519v101_hi3519v101_venc_init(void);
extern void _3519v101_hi3519v101_venc_exit(void);

static int __init venc_mod_init(void) { return _3519v101_hi3519v101_venc_init(); }
static void __exit venc_mod_exit(void) { _3519v101_hi3519v101_venc_exit(); }
module_init(venc_mod_init);
module_exit(venc_mod_exit);
MODULE_LICENSE("GPL");
