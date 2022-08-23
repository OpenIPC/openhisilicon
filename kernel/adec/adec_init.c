#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "type.h"

extern int ADEC_ModInit(void);
extern void ADEC_ModExit(void);

static int __init adec_mod_init(void)
{
	ADEC_ModInit();
	return 0;
}
static void __exit adec_mod_exit(void)
{
	ADEC_ModExit();
}

module_init(adec_mod_init);
module_exit(adec_mod_exit);

MODULE_LICENSE("GPL");
