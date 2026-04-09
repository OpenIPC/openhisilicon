#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "hi_type.h"

extern int aenc_module_init(void);
extern void aenc_module_exit(void);


static int __init aenc_mod_init(void){
    aenc_module_init();
    return 0;
}
static void __exit aenc_mod_exit(void){
    aenc_module_exit();
}

module_init(aenc_mod_init);
module_exit(aenc_mod_exit);


MODULE_LICENSE("Proprietary");

