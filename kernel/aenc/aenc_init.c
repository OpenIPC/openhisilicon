#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "type.h"

extern int AENC_ModInit(void);
extern void AENC_ModExit(void);

static int __init aenc_mod_init(void)
{
	AENC_ModInit();
	return 0;
}
static void __exit aenc_mod_exit(void)
{
	AENC_ModExit();
}

module_init(aenc_mod_init);
module_exit(aenc_mod_exit);

MODULE_LICENSE("GPL");
