#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <linux/clk.h>

#include "hi_common.h"
#include "hi_osal_init.h"

#define VIU_DEV_NAME_LENGTH 10
#define ISP_DEV_NAME_LENGTH 10
#define VIU_CLK_NAME_LENGTH 30
#define ISP_VIU_DIV_LENGTH 20

#define DEFAULT_ISP_VI_DIV 2  //clcok freq div

#define VIU_CLK_DEFAULT_FREQ 198000000  //198M

extern int detect_err_frame;
module_param(detect_err_frame, int, S_IRUGO);
extern int drop_err_frame;
module_param(drop_err_frame, uint, S_IRUGO);
extern int discard_int;
module_param(discard_int, uint, S_IRUGO);
extern int csc_tv_en;
module_param(csc_tv_en, int, S_IRUGO);
MODULE_PARM_DESC(csc_tv_en, "1: csc coefficient matrix[16~235].");
extern int csc_ct_mode;
module_param(csc_ct_mode, int, S_IRUGO);
MODULE_PARM_DESC(csc_ct_mode, "1: apply new csc contrast adjust mode.");
extern int ext_csc_en ;
module_param(ext_csc_en , int, S_IRUGO);
MODULE_PARM_DESC(ext_csc_en , "0: csc luminance [0~100], 1:[-128~128].");
extern int yuv_skip;
module_param(yuv_skip, int, S_IRUGO);
MODULE_PARM_DESC(yuv_skip, "0: yuv skip disable, default; 1: yuv skip enable.");
extern int delay_line;
module_param(delay_line, int, S_IRUGO);


unsigned int viu_clk_frequency[VIU_MAX_DEV_NUM] = {0};
module_param_array(viu_clk_frequency, uint, NULL, S_IRUGO);

unsigned int isp_div[VIU_MAX_DEV_NUM] = {0};
module_param_array(isp_div, uint, NULL, S_IRUGO);

char * input_mode[VIU_MAX_DEV_NUM] = {[0 ... (VIU_MAX_DEV_NUM-1)]="default"};
module_param_array(input_mode, charp, NULL, S_IRUGO);

extern int VIU_ModInit(void);
extern void VIU_ModExit(void);
extern  HI_BOOL VIU_DRV_IsFrameFlashed(VI_CHN ViChn);
EXPORT_SYMBOL(VIU_DRV_IsFrameFlashed);
extern HI_S32 VIU_DRV_GetPrivateData(VI_DEV ViDev, VIDEO_FRAME_INFO_S *pstViFrmInfo);
EXPORT_SYMBOL(VIU_DRV_GetPrivateData);

extern HI_U32 VIU_DRV_GetGlobalIntIndicator(HI_S32 s32ViDev);
EXPORT_SYMBOL(VIU_DRV_GetGlobalIntIndicator);

extern HI_U32 VIU_DRV_GetChnIntStatus(HI_S32 s32PhyChn);
EXPORT_SYMBOL(VIU_DRV_GetChnIntStatus);

extern HI_U32 VIU_DRV_GetGlobalChnIntMask(HI_S32 s32PhyChn);
EXPORT_SYMBOL(VIU_DRV_GetGlobalChnIntMask);

extern HI_VOID VIU_DRV_ClearChnIntStatus(HI_S32 s32PhyChn, HI_U32 u32ClrInt);
EXPORT_SYMBOL(VIU_DRV_ClearChnIntStatus);

extern HI_VOID VIU_DRV_ClearChnIntMask(HI_S32 s32PhyChn);
EXPORT_SYMBOL(VIU_DRV_ClearChnIntMask);


extern void  *g_psViuAllReg[VIU_MAX_DEV_NUM];
EXPORT_SYMBOL(g_psViuAllReg);

extern unsigned int viu_irq[VIU_MAX_DEV_NUM];

struct drv_data {
	struct clk *viu_clk[VIU_MAX_DEV_NUM];
	struct clk *isp_clk[VIU_MAX_DEV_NUM];
	HI_U32 viu_init_clk_freq[VIU_MAX_DEV_NUM];
	HI_U32 isp_init_clk_freq[VIU_MAX_DEV_NUM];
	HI_U32 viu_clk_freq[VIU_MAX_DEV_NUM];
	HI_U32 div[VIU_MAX_DEV_NUM];
	struct pinctrl *p;
	struct pinctrl_state *state;
};

static int viu_hw_init(struct platform_device *pdev)
{
	HI_S32 i;
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	for(i = 0; i < VIU_MAX_DEV_NUM; i++)
	{
		osal_clk_prepare_enable(drv_priv_data->viu_clk[i]);
		osal_clk_prepare_enable(drv_priv_data->isp_clk[i]);

		clk_freq = drv_priv_data->viu_clk_freq[i];
		osal_clk_set_rate(drv_priv_data->viu_clk[i], clk_freq);
		clk_freq = (drv_priv_data->viu_clk_freq[i])/(drv_priv_data->div[i]);
		osal_clk_set_rate(drv_priv_data->isp_clk[i], clk_freq);
	}

	return 0;
}

static void viu_hw_exit(struct platform_device *pdev)
{
	HI_S32 i;
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);
	for(i = 0; i < VIU_MAX_DEV_NUM; i++)
	{
		osal_clk_disable_unprepare(drv_priv_data->viu_clk[i]);
		osal_clk_disable_unprepare(drv_priv_data->isp_clk[i]);
	}
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_viu_suspend(struct platform_device *pdev, pm_message_t state)
{
	HI_S32 i;
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);
	for(i = 0; i < VIU_MAX_DEV_NUM; i++)
	{
		clk_freq = drv_priv_data->viu_init_clk_freq[i];
		osal_clk_set_rate(drv_priv_data->viu_clk[i], clk_freq);
		clk_freq = drv_priv_data->isp_init_clk_freq[i];
		osal_clk_set_rate(drv_priv_data->isp_clk[i], clk_freq);
	}

	viu_hw_exit(pdev);
	osal_pinctrl_get_select(&pdev->dev, "sleep");
	return 0;
}

static int hi35xx_viu_resume(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);
	if (drv_priv_data->p && drv_priv_data->state)
	{
		osal_pinctrl_select_state(drv_priv_data->p, drv_priv_data->state);
	}
	else
	{
		osal_pinctrl_pm_select_default_state(&pdev->dev);
	}

	viu_hw_init(pdev);
	return 0;
}
#endif

static int hi35xx_viu_probe(struct platform_device *pdev)
{
	HI_U32 i, ret;
	HI_CHAR ViuDevName[VIU_DEV_NAME_LENGTH] = {'\0'};
	HI_CHAR IspDevName[ISP_DEV_NAME_LENGTH] = {'\0'};
	HI_CHAR ViuClkName[VIU_CLK_NAME_LENGTH] = {'\0'};
	HI_CHAR IspDivName[ISP_VIU_DIV_LENGTH] = {'\0'};
    struct resource *mem;
	struct drv_data *drv_priv_data;
	struct device_node *np = pdev->dev.of_node;

	drv_priv_data = osal_devm_kzalloc(&pdev->dev, sizeof(*drv_priv_data));
	if (!drv_priv_data) {
		dev_err(&pdev->dev, "could not allocate viu driver private data.\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, drv_priv_data);

	for(i = 0; i < VIU_MAX_DEV_NUM; i++)
	{
		snprintf(ViuDevName, VIU_DEV_NAME_LENGTH, "viu%d", i);
		mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, ViuDevName);
    	g_psViuAllReg[i] = devm_ioremap_resource(&pdev->dev, mem);
    	if (IS_ERR(g_psViuAllReg[i]))
            return PTR_ERR(g_psViuAllReg[i]);

        viu_irq[i] = osal_platform_get_irq_byname(pdev, ViuDevName);
        if (viu_irq[i] <= 0)
		{
			dev_err(&pdev->dev, "cannot find viu IRQ\n");
		}

		snprintf(ViuClkName, VIU_CLK_NAME_LENGTH, "viu%d,clock-frequency", i);
		ret = osal_of_property_read_u32(np, ViuClkName, &drv_priv_data->viu_clk_freq[i]);
		if (ret)
		{
			drv_priv_data->viu_clk_freq[i] = viu_clk_frequency[i];
		}

		drv_priv_data->viu_clk[i] = of_clk_get_by_name(np, ViuDevName);
        if (IS_ERR(drv_priv_data->viu_clk[i]))
        {
           dev_err(&pdev->dev, "viu%d cannot get clock\n", i);
           return -ENOENT;
        }

		snprintf(IspDivName, ISP_VIU_DIV_LENGTH, "viu,isp-div-%d", i);
		if (osal_of_property_read_u32(np, IspDivName, &drv_priv_data->div[i]))
		{
			drv_priv_data->div[i] = isp_div[i];
		}

		if (0 == drv_priv_data->div[i] || drv_priv_data->div[i] > 2)
		{
			printk("the value(%d) of %s is invalid.\n", drv_priv_data->div[i], IspDivName);
			drv_priv_data->div[i] = DEFAULT_ISP_VI_DIV;
		}

		snprintf(IspDevName, ISP_DEV_NAME_LENGTH, "isp%d", i);
		drv_priv_data->isp_clk[i] = of_clk_get_by_name(np, IspDevName);
        if (IS_ERR(drv_priv_data->isp_clk[i]))
        {
            dev_err(&pdev->dev, "isp%d cannot get clock\n", i);
            return -ENOENT;
        }

		drv_priv_data->viu_init_clk_freq[i] = osal_clk_get_rate(drv_priv_data->viu_clk[i]);
		drv_priv_data->isp_init_clk_freq[i] = osal_clk_get_rate(drv_priv_data->isp_clk[i]);
		//printk("viu init clk: %d, isp init clk: %d.\n", drv_priv_data->viu_init_clk_freq[i], drv_priv_data->isp_init_clk_freq[i]);


		if (!strncmp(input_mode[i], "bt1120", 7))
		{
			struct pinctrl *p;
			struct pinctrl_state *state;

			p = (struct pinctrl *)osal_devm_pinctrl_get(&pdev->dev);
			if (IS_ERR(p)) {
				dev_err(&pdev->dev, "no pinctrl handle\n");
				return -ENOENT;
			}

			state = (struct pinctrl_state *)osal_pinctrl_lookup_state(p, "bt1120");
			if (IS_ERR(state)) {
				dev_err(&pdev->dev, "no bt1120 pinctrl state\n");
				osal_devm_pinctrl_put(p);
				return -ENOENT;
			}

			ret = osal_pinctrl_select_state(p, state);
			if (ret) {
				dev_err(&pdev->dev, "failed to activate bt1120 pinctrl state\n");
				osal_devm_pinctrl_put(p);
				return -ENOENT;
			}

			drv_priv_data->p = p;
			drv_priv_data->state = state;
		}

	}


	//get module param
    osal_platform_get_module_param(pdev, "detect_err_frame", int, &detect_err_frame);
    osal_platform_get_module_param(pdev, "drop_err_frame", uint, &drop_err_frame);
    osal_platform_get_module_param(pdev, "discard_int", uint, &discard_int);
    osal_platform_get_module_param(pdev, "csc_tv_en", int, &csc_tv_en);
    osal_platform_get_module_param(pdev, "csc_ct_mode", int, &csc_ct_mode);
    osal_platform_get_module_param(pdev, "ext_csc_en", int, &ext_csc_en);
    osal_platform_get_module_param(pdev, "yuv_skip", int, &yuv_skip);
    osal_platform_get_module_param(pdev, "delay_line", int, &delay_line);

	viu_hw_init(pdev);

    VIU_ModInit();

    return 0;
}

static int hi35xx_viu_remove(struct platform_device *pdev)
{
    VIU_ModExit();
	viu_hw_exit(pdev);

    return 0;
}

static const struct of_device_id hi35xx_viu_match[] = {
		{ .compatible = "hisilicon,hi35xx_viu" },
		{},
};
MODULE_DEVICE_TABLE(of, hi35xx_viu_match);

static struct platform_driver hi35xx_viu_driver = {
		.probe          = hi35xx_viu_probe,
		.remove         = hi35xx_viu_remove,
		.driver         = {
				.name   = "hi35xx_viu",
				.of_match_table = hi35xx_viu_match,
		},

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend        = hi35xx_viu_suspend,
		.resume         = hi35xx_viu_resume,
#endif
};

osal_module_platform_driver(hi35xx_viu_driver);

MODULE_LICENSE("Proprietary");
