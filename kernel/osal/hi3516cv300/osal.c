#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <linux/version.h>

#include "hi_osal.h"
#include "hi_osal_init.h"

extern void osal_proc_init(void);
extern void osal_proc_exit(void);
extern int himedia_init(void);
extern void himedia_exit(void);
extern int media_mem_init(void);
extern void media_mem_exit(void);
extern int mem_check_module_param(void);
extern void osal_device_init(void);

static int __init osal_init(void){
    int ret;

   if (-1 == mem_check_module_param())
   {
      return -1;
   }

    osal_device_init();
    osal_proc_init();
    himedia_init();
    ret = media_mem_init();
    if (ret) {
        himedia_exit();
        osal_proc_exit();
        return ret;
    }
    osal_printk("hi_osal %s init success!\n", HI_OSAL_VERSION);
    return 0;
}

static void __exit osal_exit(void){
	osal_proc_exit();
	himedia_exit();
    media_mem_exit();
	osal_printk("hi_osal v1.0 exit!\n");
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,19,0)
module_init(osal_init);
module_exit(osal_exit);
#else
//module param
#define MMZ_SETUP_CMDLINE_LEN 	256
#define MMZ_ALLOCATOR_NAME_LEN	32

extern int anony;
extern char setup_zones[MMZ_SETUP_CMDLINE_LEN];
extern char setup_allocator[MMZ_ALLOCATOR_NAME_LEN];

static int osal_probe(struct platform_device *pdev)
{
	int ret;

	osal_platform_get_module_param(pdev, "anony", int, &anony);
	osal_platform_get_modparam_string(pdev, "setup_zones", MMZ_SETUP_CMDLINE_LEN, setup_zones);
	osal_platform_get_modparam_string(pdev, "setup_allocator", MMZ_ALLOCATOR_NAME_LEN, setup_allocator);

	ret = osal_init();
	if (ret)
	{
		return ret;
	}
	
    return 0;
}

static compat_platform_remove_ret osal_remove(struct platform_device *pdev)
{
	osal_exit();
	compat_platform_remove_return;
}

static const struct of_device_id osal_match[] = {
        { .compatible = "hisilicon,osal" },
        {},
};
MODULE_DEVICE_TABLE(of, osal_match);

static struct platform_driver osal_driver = {
        .probe          = osal_probe,
        .remove         = osal_remove,
        .driver         = {
                .name   = "osal",
                .of_match_table = osal_match,
        },
};

osal_module_platform_driver(osal_driver);
#endif

MODULE_AUTHOR("Hisilicon");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
