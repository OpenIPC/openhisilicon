#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "hi_type.h"
#include "hi_defines.h"


extern int rc_module_init(void);
extern void rc_module_exit(void);

static int __init rc_mod_init(void)
{
    rc_module_init();
    return 0;
}

static void __exit rc_mod_exit(void)
{
    rc_module_exit();
}

module_init(rc_mod_init);
module_exit(rc_mod_exit);

MODULE_LICENSE("Proprietary");

