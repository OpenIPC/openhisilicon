#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include "hi_type.h"
#include "hi_osal.h"

extern int venc_module_init(void);
extern void venc_module_exit(void);
extern HI_U32 VencMaxChnNum;
module_param( VencMaxChnNum, uint, S_IRUGO);


static int hi35xx_venc_probe(struct platform_device *pdev)
{
    osal_platform_get_module_param(pdev, "VencMaxChnNum", int, &VencMaxChnNum);
    venc_module_init();
    return 0;
}


static int hi35xx_venc_remove(struct platform_device *pdev)
{
    venc_module_exit();
    return 0;
}

static const struct of_device_id hi35xx_venc_match[] = {
        { .compatible = "hisilicon,hisi-venc" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_venc_match);

static struct platform_driver hi35xx_venc_driver = {
        .probe          = hi35xx_venc_probe,
        .remove         = hi35xx_venc_remove,
        .driver         = {
                .name   = "hi35xx_venc",
                .of_match_table = hi35xx_venc_match,
        }
};

osal_module_platform_driver(hi35xx_venc_driver);


MODULE_LICENSE("Proprietary");




