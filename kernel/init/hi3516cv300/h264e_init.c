#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "hi_type.h"

extern int H264E_ModInit(void);
extern void H264E_ModExit(void);

extern HI_U32  H264eOneStreamBuffer;
module_param( H264eOneStreamBuffer, uint, S_IRUGO);

extern HI_U32  H264eVBSource;
module_param(H264eVBSource, uint, S_IRUGO);

extern HI_U32  H264eRcnEqualRef;
module_param(H264eRcnEqualRef, uint, S_IRUGO);

extern HI_U32  H264eMiniBufMode;
module_param( H264eMiniBufMode, uint, S_IRUGO);

extern HI_U32  H264ePowerSaveEn;
module_param( H264ePowerSaveEn, uint, S_IRUGO);

static int __init h264e_mod_init(void)
{
	return H264E_ModInit();
}
static void __exit h264e_mod_exit(void){
	H264E_ModExit();
}

module_init(h264e_mod_init);
module_exit(h264e_mod_exit);

MODULE_LICENSE("Proprietary");
