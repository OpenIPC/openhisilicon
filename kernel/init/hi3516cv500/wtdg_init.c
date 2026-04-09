/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2006-2019. All rights reserved.
 * Description: wtdg_init.c
 * Author:
 * Create: 2006-12-16
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "hi_type.h"
#include "watchdog.h"

#define OSDRV_MODULE_VERSION_STRING "HISI_wtdg @HiMPP"

extern int default_margin;
extern int nodeamon;
extern volatile void *g_wtdg_reg_base;

module_param(default_margin, int, 0);
MODULE_PARM_DESC(default_margin,
    "Watchdog default_margin in seconds. (0<default_margin<80, default=" __MODULE_STRING(HIDOG_TIMER_MARGIN) ")");

module_param(nodeamon, int, 0);
MODULE_PARM_DESC(nodeamon,
    "By default, a kernel deamon feed watchdog when idle, set 'nodeamon=1' to disable this. (default=0)");

static int hi_wdg_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_wtdg_reg_base = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void*)g_wtdg_reg_base)) {
        return PTR_ERR((void*)g_wtdg_reg_base);
    }

    return watchdog_init();
}

static int hi_wdg_remove(struct platform_device *pdev)
{
    watchdog_exit();
    return 0;
}

static const struct of_device_id g_hi_wdg_match[] = {
    { .compatible = "hisilicon,hi_wdg" },
	{ .compatible = "hisilicon,hi-wdg" },
    {},
};

static struct platform_driver g_hi_wdg_driver = {
    .probe  = hi_wdg_probe,
    .remove = hi_wdg_remove,
    .driver =  { .name = "hi_wdg",
                .of_match_table = g_hi_wdg_match,
               },
};

module_platform_driver(g_hi_wdg_driver);
MODULE_LICENSE("GPL");

/*
 * Export symbol
 */
MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_DESCRIPTION("Mipi Driver");
MODULE_VERSION("HI_VERSION=" OSDRV_MODULE_VERSION_STRING);

