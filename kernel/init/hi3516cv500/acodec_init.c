#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "hi_type.h"

extern int acodec_init(void);
extern void acodec_exit(void);
extern unsigned int init_delay_time;
module_param(init_delay_time, uint, S_IRUGO);

static __init int acodec_mod_init(void){

    return acodec_init();
}
static __exit void acodec_mod_exit(void){
    acodec_exit();
}

module_init(acodec_mod_init);
module_exit(acodec_mod_exit);

MODULE_LICENSE("Proprietary");

