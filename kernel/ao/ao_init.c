#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "type.h"

extern int AO_ModInit(void);
extern void AO_ModExit(void);

static int __init ao_mod_init(void)
{
	AO_ModInit();
	return 0;
}
static void __exit ao_mod_exit(void)
{
	AO_ModExit();
}

module_init(ao_mod_init);
module_exit(ao_mod_exit);

MODULE_LICENSE("GPL");
