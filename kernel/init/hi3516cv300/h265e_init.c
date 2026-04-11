#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "hi_type.h"

extern int H265E_ModInit(void);
extern void H265E_ModExit(void);

extern HI_U32  H265eOneStreamBuffer;
module_param( H265eOneStreamBuffer, uint, S_IRUGO);

extern HI_U32  H265eVBSource;
module_param(H265eVBSource, uint, S_IRUGO);

extern HI_U32  H265eMiniBufMode;
module_param( H265eMiniBufMode, uint, S_IRUGO);

extern HI_U32  H265ePowerSaveEn;
module_param( H265ePowerSaveEn, uint, S_IRUGO);

static int __init h265e_mod_init(void){

	return H265E_ModInit();
	//return 0;
}
static void __exit h265e_mod_exit(void){
	H265E_ModExit();
}

module_init(h265e_mod_init);
module_exit(h265e_mod_exit);

MODULE_LICENSE("Proprietary");
