/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include "gfbg_init.h"
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/of_platform.h>
#include "ot_type.h"
#include "ot_osal.h"
#ifndef MODULE
#include "securec.h"

char g_layer_mmz_names[GFBG_MAX_LAYER_NUM][OT_MAX_MMZ_NAME_LEN] = { TD_NULL };
#else
char *g_layer_mmz_names[GFBG_MAX_LAYER_NUM] = { [0 ... GFBG_MAX_LAYER_NUM - 1] = TD_NULL };
#endif
char g_tmp_video[128] = "gfbg:vram0_size:8100"; /* 128 The length of the array */

#ifndef MODULE
osal_setup_str_param(video, g_tmp_video);
static int __init parse_kern_str_layer_mmz_names(char *line)
{
    const td_char *delim = ",";
    td_char *token = TD_NULL;
    td_char *cur = line;
    td_u32 i = 0;

    while ((token = osal_strsep(&cur, delim)) && (i < GFBG_MAX_LAYER_NUM)) {
        if (strncpy_s(g_layer_mmz_names[i], OT_MAX_MMZ_NAME_LEN, token, OT_MAX_MMZ_NAME_LEN - 1) != EOK) {
            osal_printk("parse param g_layer_mmz_names failed\n");
            return -1;
        }
        i++;
    }
    return 0;
}
__setup("g_layer_mmz_names=", parse_kern_str_layer_mmz_names);
#else
module_param_string(video, g_tmp_video, 128, 0);

module_param_array(g_layer_mmz_names, charp, TD_NULL, S_IRUGO);
MODULE_PARM_DESC(g_layer_mmz_names, "The mmz names for the graphics layers.");
#endif

char *gfbg_get_layer_mmz_names(td_u32 layer_id)
{
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_NULL;
    }
    return g_layer_mmz_names[layer_id];
}

#define GFBG_INT_NAME_LENGTH 10

static int ot_gfbg_probe(struct platform_device *pdev)
{
    td_char gfbg_int_name[GFBG_INT_NAME_LENGTH] = "gfbg";
    int *fifb_irq = (int *)(fb_get_gfbg_irq());
    set_video_name(g_tmp_video);
    *fifb_irq = osal_platform_get_irq_byname(pdev, gfbg_int_name);
    if (*fifb_irq <= 0) {
        dev_err(&pdev->dev, "cannot find gfbg IRQ\n");
        return TD_FAILURE;
    }
    if (gfbg_init() != TD_SUCCESS) {
        osal_printk("gfbg_init FAILURE!\n");
        return TD_FAILURE;
    }
    return 0;
}

static int ot_gfbg_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    gfbg_cleanup();
    return 0;
}

static const struct of_device_id ot_gfbg_match[] = {
    { .compatible = "vendor,gfbg" },
    {},
};
MODULE_DEVICE_TABLE(of, ot_gfbg_match);

static struct platform_driver ot_gfbg_driver = {
    .probe = ot_gfbg_probe,
    .remove = ot_gfbg_remove,
    .driver = {
        .name   = "ot_gfbg",
        .of_match_table = ot_gfbg_match,
    },
};

osal_module_platform_driver(ot_gfbg_driver);
MODULE_LICENSE("GPL");

