/*
 * Sys Config Driver for HiSilicon BVT SoCs
 *
 * Copyright (c) 2016 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#define PIN_CTRL_NAME "pinctrl-single,pins"

unsigned int vi_vpss_online = 1;
module_param(vi_vpss_online, uint, S_IRUGO);

static const char node_name[3][10] = {
	"online", "offline", "flag"
};

struct reg_vals {     
	unsigned int reg_phy_addr;
	unsigned int value;
};   

static const struct of_device_id hibvt_pinctrl_of_match[] = {
	{ .compatible = "hisilicon,vi-vpss-online", .data = node_name[2] },
	{ .compatible = "hisilicon,pinctrl-ddr" },
	{ .compatible = "hisilicon,pinctrl-online", .data = node_name[0] },
	{ .compatible = "hisilicon,pinctrl-offline", .data = node_name[1] },
	//{ .compatible = "hisilicon,user_define_pinmux" },
	{  },
};

static int hibvt_sys_init(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct reg_vals *vals;
	const __be32 *mux;
	unsigned int size, rows, index = 0, found = 0;
	void __iomem *reg_vir_addr;	
	const struct of_device_id *id = 
						of_match_device(hibvt_pinctrl_of_match, &pdev->dev);
	const char* p_node_name = (id ? id->data : NULL);

	/*when vi-vpss mode is online, the offline compatible string 
	would be probe. The same as offline mode.*/
	if (p_node_name)
	{
		if (!strncmp(p_node_name, "flag", 10))
		{
			unsigned int flag;
			if (0 == of_property_read_u32(np, "vi_vpss_online", &flag))
			{
				vi_vpss_online = flag;
			}
			return 0;
		}

        if(((1 == vi_vpss_online) && !strncmp(p_node_name, "offline", 10))
			|| ((0 == vi_vpss_online) && !strncmp(p_node_name, "online", 10)))
        {
		    return 0;
        }
	}

	mux = of_get_property(np, PIN_CTRL_NAME, &size);  
	if ((!mux) || (size < sizeof(*mux) * 2)) {
		return -EINVAL;
	}

	size /= sizeof(*mux);	/* Number of elements in array */
	rows = size / 2;

	vals = (struct reg_vals *)kmalloc(sizeof(*vals) * rows, GFP_KERNEL);
	if (!vals)
		return -ENOMEM;

	while (index < size) {
		vals[found].reg_phy_addr = be32_to_cpup(mux + index++);
		vals[found].value = be32_to_cpup(mux + index++);

		reg_vir_addr = ioremap_nocache(vals[found].reg_phy_addr, sizeof(*mux));
        if(reg_vir_addr == NULL)
        {
            printk("ioremap_nocache error\n");
            continue;
        }

		*((volatile int *)(reg_vir_addr)) = vals[found].value;

		iounmap(reg_vir_addr);

		found++;
	}

	kfree(vals);

	return 0;
}

#ifdef CONFIG_PM
static int hibvt_pinctrl_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int hibvt_pinctrl_resume(struct platform_device *pdev)
{
	return hibvt_sys_init(pdev);
}
#endif

MODULE_DEVICE_TABLE(of, hibvt_pinctrl_of_match);
static int hibvt_pinctrl_probe(struct platform_device *pdev)
{
	return hibvt_sys_init(pdev);
}

static compat_platform_remove_ret hibvt_pinctrl_remove(struct platform_device *pdev)
{
	compat_platform_remove_return;
}

static struct platform_driver hibvt_pinctrl_driver = {
	.driver = {
		.name = "hibvt-pinctrl",
		.of_match_table = hibvt_pinctrl_of_match,
	},
	.probe = hibvt_pinctrl_probe,
	.remove	= hibvt_pinctrl_remove,
#ifdef CONFIG_PM
	.suspend        = hibvt_pinctrl_suspend,
	.resume         = hibvt_pinctrl_resume,
#endif
};
module_platform_driver(hibvt_pinctrl_driver);

MODULE_AUTHOR("HiMPP GRP");
MODULE_DESCRIPTION("HiSilicon BVT Sys Config Driver");
MODULE_LICENSE("GPL");
