#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "type.h"
#include "osal.h"
#include "common.h"

#define PHASE 32
#define COEFF_BUTT (PHASE / 2 + 1)

/*set default user mode helper path*/
#define PM_EVENT_HELPER_LEN 128
char g_pm_mpp_helper[PM_EVENT_HELPER_LEN] = "/root";
static struct ctl_table_header *ctl_head;

static struct ctl_table pm_mpp_ctl[] = { {
						 .procname = "mpp_notifier",
						 .data = &g_pm_mpp_helper,
						 .maxlen = PM_EVENT_HELPER_LEN,
						 .mode = 0644,
						 .proc_handler = proc_dostring,
					 },
					 {} };

static struct ctl_path pm_umh_root[] = { {
						 .procname = "kernel",
					 },
					 {} };

extern int SYS_ModInit(void);
extern void SYS_ModExit(void);

#if 0
static int __init sys_mod_init(void){
    SYS_ModInit();

    ctl_head = register_sysctl_paths(pm_umh_root, pm_mpp_ctl);

    return 0;
}
static void __exit sys_mod_exit(void){
    unregister_sysctl_table(ctl_head);

    SYS_ModExit();
}

module_init(sys_mod_init);
module_exit(sys_mod_exit);
#else

#include <linux/of_platform.h>

static int hi35xx_sys_probe(struct platform_device *pdev)
{
	if (GK_SUCCESS != SYS_ModInit()) {
		return GK_FAILURE;
	}

	ctl_head = register_sysctl_paths(pm_umh_root, pm_mpp_ctl);

	return 0;
}

static int hi35xx_sys_remove(struct platform_device *pdev)
{
	unregister_sysctl_table(ctl_head);

	SYS_ModExit();

	return 0;
}

static const struct of_device_id hi35xx_sys_match[] = {
	{ .compatible = "hisilicon,hisi-sys" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_sys_match);

static struct platform_driver hi35xx_sys_driver = {
        .probe          = hi35xx_sys_probe,
        .remove         = hi35xx_sys_remove,
        .driver         = {
        .name   = "hi35xx_sys",
        .of_match_table = hi35xx_sys_match,
        },
};

osal_module_platform_driver(hi35xx_sys_driver);
#endif

MODULE_LICENSE("GPL");
