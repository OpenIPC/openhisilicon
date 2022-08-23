#include <linux/module.h>
#include <linux/kernel.h>
#include "type.h"

extern int VENC_ModInit(void);
extern void VENC_ModExit(void);

extern GK_U32 g_venc_buffer_cache;
extern GK_U32 g_frame_buf_recycle;
extern GK_U32 VencMaxChnNum;

module_param(VencMaxChnNum, uint, S_IRUGO);
EXPORT_SYMBOL(g_venc_buffer_cache);
EXPORT_SYMBOL(g_frame_buf_recycle);

static int __init venc_mod_init(void)
{
	VENC_ModInit();
	return 0;
}
static void __exit venc_mod_exit(void)
{
	VENC_ModExit();
}

module_init(venc_mod_init);
module_exit(venc_mod_exit);

MODULE_LICENSE("GPL");
