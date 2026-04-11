#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>


extern unsigned int bCanvasNumValid;
module_param(bCanvasNumValid, uint, S_IRUGO);

extern int RGN_ModInit(void);
extern void RGN_ModExit(void);


static int __init rgn_mod_init(void)
{
	RGN_ModInit();
	return 0;
}
static void __exit rgn_mod_exit(void)
{
	RGN_ModExit();
}

module_init(rgn_mod_init);
module_exit(rgn_mod_exit);

MODULE_LICENSE("Proprietary");
