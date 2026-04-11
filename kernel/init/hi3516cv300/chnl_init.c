#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "hi_type.h"

extern int CHNL_ModInit(void);
extern void CHNL_ModExit(void);

static int __init chnl_mod_init(void)
{
	CHNL_ModInit();
	return 0;
}
static void __exit chnl_mod_exit(void)
{
	
	CHNL_ModExit();
	
}

module_init(chnl_mod_init);
module_exit(chnl_mod_exit);

MODULE_LICENSE("Proprietary");
