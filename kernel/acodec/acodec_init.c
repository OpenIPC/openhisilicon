#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "type.h"

#include "../compat/compat.h"

#ifdef hi3516cv500
extern int acodec_mod_init(void);
extern void acodec_mod_exit(void);
#else
extern int acodec_init(void);
extern void acodec_exit(void);
#endif
extern unsigned int init_delay_time;
module_param(init_delay_time, uint, S_IRUGO);

static __init int acodec_module_init(void)
{
#ifdef hi3516cv500
	return acodec_mod_init();
#else
	return acodec_init();
#endif
}
static __exit void acodec_module_exit(void)
{
#ifdef hi3516cv500
	acodec_mod_exit();
#else
	acodec_exit();
#endif
}

module_init(acodec_module_init);
module_exit(acodec_module_exit);

MODULE_LICENSE("GPL");
