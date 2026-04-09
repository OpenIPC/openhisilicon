#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>


#include "hi_type.h"

extern int ao_module_init(void);
extern void ao_module_exit(void);

static int __init ao_mod_init(void){
    ao_module_init();
    return 0;
}
static void __exit ao_mod_exit(void){
    ao_module_exit();
}

module_init(ao_mod_init);
module_exit(ao_mod_exit);

MODULE_LICENSE("Proprietary");

