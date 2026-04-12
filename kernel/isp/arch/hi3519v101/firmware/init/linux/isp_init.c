#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_osal.h"

extern HI_U32 pwm_num;
module_param(pwm_num, uint, S_IRUGO);
extern HI_U32 proc_param;
module_param(proc_param, uint, S_IRUGO);
extern HI_U32 update_pos;
module_param(update_pos, uint, S_IRUGO);
extern HI_U32 int_timeout;
module_param(int_timeout, uint, S_IRUGO);
extern HI_BOOL int_bottomhalf;
module_param(int_bottomhalf, uint, S_IRUGO);
extern HI_U32 stat_intvl;
module_param(stat_intvl, uint, S_IRUGO);

extern int ISP_ModInit(void);
extern void ISP_ModExit(void);

#if 0
static int __init isp_mod_init(void){
	ISP_ModInit();
	return 0;
}
static void __exit isp_mod_exit(void){
	ISP_ModExit();
}

module_init(isp_mod_init);
module_exit(isp_mod_exit);

MODULE_LICENSE("Proprietary");
#else
#include <linux/of_platform.h>

extern unsigned int isp_irq[VIU_MAX_DEV_NUM];

static int hi35xx_isp_probe(struct platform_device *pdev)
{
	HI_S32 i = 0;
	HI_CHAR IspDevName[10] = {'\0'};
	
	for(i = 0; i < VIU_MAX_DEV_NUM; i++)
	{
        snprintf(IspDevName, sizeof(IspDevName), "isp%d", i);
		
	    isp_irq[i] = osal_platform_get_irq_byname (pdev, IspDevName);
	    if (isp_irq[i] <= 0) {
	            dev_err(&pdev->dev, "cannot find isp IRQ\n");
	    }
	}

	//printk("++++++++++ isp_irq[0] = %d isp_irq[1] = %d\n",
	//	isp_irq[0],isp_irq[1]);

    ISP_ModInit();
    
    return 0;
}

static int hi35xx_isp_remove(struct platform_device *pdev)
{
    //printk("<%s> is called\n",__FUNCTION__);
    ISP_ModExit();
    return 0;
}


static const struct of_device_id hi35xx_isp_match[] = {
        { .compatible = "hisilicon,hi35xx_isp" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_isp_match);

static struct platform_driver hi35xx_isp_driver = {
        .probe          = hi35xx_isp_probe,
        .remove         = hi35xx_isp_remove,
        .driver         = {
                .name   = "hi35xx_isp",
                .of_match_table = hi35xx_isp_match,
        },
};

osal_module_platform_driver(hi35xx_isp_driver);

MODULE_LICENSE("Proprietary");
#endif

