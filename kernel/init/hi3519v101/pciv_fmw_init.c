#include <linux/module.h>
#include <linux/kernel.h>

extern int PCIV_FirmWareCreate(void *p);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareCreate);
extern int PCIV_FirmWareDestroy(int id);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareDestroy);
extern int PCIV_FirmWareFree(int id);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareFree);
extern int PCIV_FirmWareFreeChnBuffer(int id, int chn);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareFreeChnBuffer);
extern int PCIV_FirmWareGetPreProcCfg(int id, void *cfg);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareGetPreProcCfg);
extern int PCIV_FirmWareMalloc(int id, unsigned int size);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareMalloc);
extern int PCIV_FirmWareMallocChnBuffer(int id, int chn, unsigned int size);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareMallocChnBuffer);
extern int PCIV_FirmWareRecvPicAndSend(int id, void *pic);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareRecvPicAndSend);
extern int PCIV_FirmWareRegisterFunc(int id, void *func);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareRegisterFunc);
extern int PCIV_FirmWareSetAttr(int id, void *attr);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareSetAttr);
extern int PCIV_FirmWareSetPreProcCfg(int id, void *cfg);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareSetPreProcCfg);
extern int PCIV_FirmWareSrcPicFree(int id, void *pic);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareSrcPicFree);
extern int PCIV_FirmWareStart(int id);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareStart);
extern int PCIV_FirmWareStop(int id);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareStop);
extern int PCIV_FirmWareWindowVbCreate(int id, void *attr);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareWindowVbCreate);
extern int PCIV_FirmWareWindowVbDestroy(int id);
COMPAT_REEXPORT_BLOB_SYMBOL(PCIV_FirmWareWindowVbDestroy);

extern int _3519v101_hi3519v101_pciv_fmw_init(void);
extern void _3519v101_hi3519v101_pciv_fmw_exit(void);

static int __init pciv_fmw_mod_init(void) { return _3519v101_hi3519v101_pciv_fmw_init(); }
static void __exit pciv_fmw_mod_exit(void) { _3519v101_hi3519v101_pciv_fmw_exit(); }
module_init(pciv_fmw_mod_init);
module_exit(pciv_fmw_mod_exit);
MODULE_LICENSE("GPL");
