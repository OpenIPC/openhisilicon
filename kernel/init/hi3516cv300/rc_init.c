#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "hi_type.h"
#include "hi_defines.h"

extern HI_U32 ClrStatAfterSetBr;
module_param( ClrStatAfterSetBr, uint, S_IRUGO);

extern int RC_ModInit(void);
extern void RC_ModExit(void);


static int __init rc_mod_init(void){
	RC_ModInit();
	return 0;
}
static void __exit rc_mod_exit(void){
	RC_ModExit();
}

module_init(rc_mod_init);
module_exit(rc_mod_exit);

MODULE_LICENSE("Proprietary");
