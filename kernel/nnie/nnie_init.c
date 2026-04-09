/*
 * NNIE (Neural Network Inference Engine) module init - hi3516cv500 only
 *
 * Adapted from HiSilicon vendor SDK
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "../../include/type.h"
#include "../../include/defines.h"
#include "../compat/compat.h"
#include "../compat/kernel_compat.h"
#include "../osal/include/osal.h"

extern GK_U8 g_nnie_power_save_enable;
extern GK_U16 g_nnie_task_buf_num;

extern int svp_nnie_module_init(void);
extern void svp_nnie_module_exit(void);

module_param_named(nnie_save_power, g_nnie_power_save_enable, byte, S_IRUGO);
module_param_named(nnie_max_tskbuf_num, g_nnie_task_buf_num, ushort, S_IRUGO);

#define SVP_NNIE_DEV_NAME_LENGTH 8
#define SVP_NNIE_DEV_NUM 1

extern void *g_nnie_reg[SVP_NNIE_DEV_NUM];
extern GK_U32 g_nnie_irq[SVP_NNIE_DEV_NUM];
extern GK_U32 g_svp_nnie_gdc_irq[GDC_IP_NUM];

static int hi35xx_svp_nnie_probe(struct platform_device *pf_dev)
{
	GK_U32 i;
	GK_S32 irq = 0;
	GK_CHAR nnie_dev_name[SVP_NNIE_DEV_NAME_LENGTH] = { '\0' };
	GK_CHAR gdc_dev_name[SVP_NNIE_DEV_NAME_LENGTH] = "gdc";
	struct resource *mem;

	for (i = 0; i < SVP_NNIE_DEV_NUM; i++) {
		snprintf(nnie_dev_name, SVP_NNIE_DEV_NAME_LENGTH, "nnie%d", i);
		mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM,
							nnie_dev_name);
		g_nnie_reg[i] = devm_ioremap_resource(&pf_dev->dev, mem);
		if (IS_ERR(g_nnie_reg[i]))
			return PTR_ERR(g_nnie_reg[i]);

		irq = osal_platform_get_irq_byname(pf_dev, nnie_dev_name);
		if (irq <= 0) {
			dev_err(&pf_dev->dev, "can't find nnie%d IRQ\n", i);
			return -1;
		}
		g_nnie_irq[i] = (GK_U32)irq;
	}

	for (i = 0; i < GDC_IP_NUM; i++) {
		g_svp_nnie_gdc_irq[i] =
			osal_platform_get_irq_byname(pf_dev, gdc_dev_name);
		if (g_svp_nnie_gdc_irq[i] <= 0) {
			dev_err(&pf_dev->dev, "cannot find gdc%d IRQ\n", i);
			return -1;
		}
	}

	return svp_nnie_module_init();
}

static COMPAT_REMOVE_RETURN hi35xx_svp_nnie_remove(struct platform_device *pf_dev)
{
	GK_U32 i;

	svp_nnie_module_exit();
	for (i = 0; i < SVP_NNIE_DEV_NUM; i++)
		g_nnie_reg[i] = GK_NULL;

	COMPAT_REMOVE_RETURN_VALUE;
}

static const struct of_device_id hi35xx_svp_nnie_match[] = {
	{ .compatible = "hisilicon,hisi-nnie" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_svp_nnie_match);

static struct platform_driver hi35xx_svp_nnie_driver = {
	.probe = hi35xx_svp_nnie_probe,
	.remove = hi35xx_svp_nnie_remove,
	.driver = {
		.name = "hi35xx_nnie",
		.of_match_table = hi35xx_svp_nnie_match,
	},
};

osal_module_platform_driver(hi35xx_svp_nnie_driver);

MODULE_LICENSE("GPL");
