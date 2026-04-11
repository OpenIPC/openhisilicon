#include <linux/module.h>

extern int _cv200_hi3518e_ai_init(void);
extern void _cv200_hi3518e_ai_exit(void);

extern char g_stAiChn[];	EXPORT_SYMBOL(g_stAiChn);

static int __init ai_mod_init(void)
{
	return _cv200_hi3518e_ai_init();
}

static void __exit ai_mod_exit(void)
{
	_cv200_hi3518e_ai_exit();
}

module_init(ai_mod_init);
module_exit(ai_mod_exit);
MODULE_LICENSE("Proprietary");
