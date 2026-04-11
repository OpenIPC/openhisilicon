#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "hi_type.h"

extern int VENC_ModInit(void);
extern void VENC_ModExit(void);


extern HI_U32 VencBufferCache;
module_param( VencBufferCache, uint, S_IRUGO);

extern HI_U32 FrameBufRecycle;
module_param( FrameBufRecycle, uint, S_IRUGO);

extern HI_U32 QuickSchedule;
module_param( QuickSchedule, uint, S_IRUGO);

extern HI_U32 JpegClearStreamBuf;
module_param( JpegClearStreamBuf, uint, S_IRUGO);

extern HI_U32 OneBufferForJpegOSD;
module_param( OneBufferForJpegOSD, uint, S_IRUGO);

extern HI_U32 VencMaxChnNum;
module_param( VencMaxChnNum, uint, S_IRUGO);

EXPORT_SYMBOL(VencBufferCache);
EXPORT_SYMBOL(FrameBufRecycle);
EXPORT_SYMBOL(QuickSchedule);
EXPORT_SYMBOL(JpegClearStreamBuf);
EXPORT_SYMBOL(OneBufferForJpegOSD);
EXPORT_SYMBOL(VencMaxChnNum);

static int __init venc_mod_init(void){
	VENC_ModInit();
	return 0;
}
static void __exit venc_mod_exit(void){
	VENC_ModExit();
}

module_init(venc_mod_init);
module_exit(venc_mod_exit);

MODULE_LICENSE("Proprietary");




