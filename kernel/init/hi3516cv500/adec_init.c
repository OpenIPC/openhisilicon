#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "hi_type.h"

extern int adec_module_init(void);
extern void adec_module_exit(void);

static int __init adec_mod_init(void){
    adec_module_init();
    return 0;
}
static void __exit adec_mod_exit(void){
    adec_module_exit();
}

module_init(adec_mod_init);
module_exit(adec_mod_exit);

MODULE_LICENSE("Proprietary");

