#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "hi_type.h"
#include "hi_defines.h"

extern int ai_module_init(void);
extern void ai_module_exit(void);

static int __init ai_mod_init(void){
    ai_module_init();
    return 0;
}
static void __exit ai_mod_exit(void){
    ai_module_exit();
}

module_init(ai_mod_init);
module_exit(ai_mod_exit);

MODULE_LICENSE("Proprietary");

