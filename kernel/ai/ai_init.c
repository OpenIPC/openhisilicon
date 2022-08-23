#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "type.h"
#include "defines.h"

extern int AI_ModInit(void);
extern void AI_ModExit(void);

extern GK_S32 AI_DRV_IsVqeEnable(GK_S32 s32ChnId, GK_BOOL *pbEnable);

static int __init ai_mod_init(void)
{
	AI_ModInit();
	return 0;
}
static void __exit ai_mod_exit(void)
{
	AI_ModExit();
}

module_init(ai_mod_init);
module_exit(ai_mod_exit);

EXPORT_SYMBOL(AI_DRV_IsVqeEnable);

MODULE_LICENSE("GPL");
