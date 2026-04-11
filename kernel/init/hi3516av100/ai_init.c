#include <linux/module.h>
#include <linux/kernel.h>

extern int _av100_hi3516a_ai_init(void);
extern void _av100_hi3516a_ai_exit(void);

#define DECLARE_BLOB_SYM(sym) extern char sym[]

DECLARE_BLOB_SYM(g_stAiChn);			EXPORT_SYMBOL(g_stAiChn);

static int __init ai_mod_init(void) { return _av100_hi3516a_ai_init(); }
static void __exit ai_mod_exit(void) { _av100_hi3516a_ai_exit(); }
module_init(ai_mod_init);
module_exit(ai_mod_exit);
MODULE_LICENSE("GPL");
