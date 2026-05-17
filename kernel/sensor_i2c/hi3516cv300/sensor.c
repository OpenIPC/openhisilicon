#include <linux/module.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/consumer.h>

#include "hi_osal.h"

#include "sensor.h"
#include "isp_ext.h"
#include "i2c_drv.h"
#include "ssp_drv.h"

struct sensor_device *g_stSensorDev[SENSOR_NUM] = {0};

static int sensor_drv_init(struct sensor_device *sensor_dev)
{
	SENSOR_CTRL_BUS_S *pstCtrlBus;
	HI_S32 ret = 0;

	pstCtrlBus = &sensor_dev->stCtrlBus;
	switch(pstCtrlBus->enBusType)
	{
		case SENSOR_CTRL_BUS_I2C:
			ret = i2c_drv_init(pstCtrlBus->u32BusNum);
			break;
		case SENSOR_CTRL_BUS_SSP:
			ret = ssp_drv_init(pstCtrlBus->u32BusNum);
			break;
		default:
			break;
	}

	return ret;
}

static void sensor_drv_exit(struct sensor_device *sensor_dev)
{
	SENSOR_CTRL_BUS_S *pstCtrlBus;

	pstCtrlBus = &sensor_dev->stCtrlBus;
	switch(pstCtrlBus->enBusType)
	{
		case SENSOR_CTRL_BUS_I2C:
			i2c_drv_exit(pstCtrlBus->u32BusNum);
			break;
		case SENSOR_CTRL_BUS_SSP:
			ssp_drv_exit(pstCtrlBus->u32BusNum);
			break;
		default:
			break;
	}

	return;
}

static HI_S32 isp_register_callback(struct sensor_device *sensor_dev)
{
	ISP_BUS_CALLBACK_S stBusCb = {0};

	SENSOR_CTRL_BUS_S *pstCtrlBus;
	SENSOR_CTRL_OPS_S *pstCtrlOps;

	pstCtrlBus = &sensor_dev->stCtrlBus;
	pstCtrlOps = &sensor_dev->stCtrlOps;

	switch(pstCtrlBus->enBusType)
	{
		case SENSOR_CTRL_BUS_I2C:
			i2c_get_ops(pstCtrlBus->u32BusNum, pstCtrlOps);
			break;
		case SENSOR_CTRL_BUS_SSP:
			ssp_get_ops(pstCtrlBus->u32BusNum, pstCtrlOps);
			break;
		default:
			break;
	}

	stBusCb.pfnISPWriteSensorData = pstCtrlOps->write;
	stBusCb.pfnISPReadSensorData  = pstCtrlOps->read;
	if (CKFN_ISP_RegisterBusCallBack())
	{
		CALL_ISP_RegisterBusCallBack(sensor_dev->s32IspBindDev, &stBusCb);
	}
	else
	{
		osal_printk("register senor_callback to isp failed!\n");
		return -1;
	}

	return 0;
}

HI_S32 sensor_dev_init(HI_S32 s32SensorIndex)
{
	struct sensor_device *sensor_dev;
	HI_S32 ret;

	sensor_dev = g_stSensorDev[s32SensorIndex];
	if (NULL == sensor_dev)
	{
		osal_printk("%s(%d): sensor_dev is NULL.\n", __func__, __LINE__);
		return -1;
	}

	ret = sensor_drv_init(sensor_dev);
	if (ret)
	{
		return ret;
	}

	ret = isp_register_callback(sensor_dev);
	if (ret)
	{
		sensor_drv_exit(sensor_dev);
		return ret;
	}

    osal_printk("sensor dev init OK.\n");
	return 0;
}

HI_VOID sensor_dev_exit(HI_S32 s32SensorIndex)
{
	struct sensor_device *sensor_dev;

	sensor_dev = g_stSensorDev[s32SensorIndex];
	if (NULL == sensor_dev)
	{
		osal_printk("%s(%d): sensor_dev is NULL.\n", __func__, __LINE__);
		return;
	}

	sensor_dev->stCtrlOps.read = NULL;
	sensor_dev->stCtrlOps.write = NULL;

	sensor_drv_exit(sensor_dev);

    osal_printk("sensor dev exit OK.\n");
	g_stSensorDev[s32SensorIndex] = NULL;
}

/* load_hisilicon passes these as strings (e.g. sensor_bus_type="i2c",
 * sensor_pinmux_mode="i2c_mipi") and a numeric Hz for the clock. */
static char *sensor_bus_type      = "i2c";
static int   sensor_clk_frequency = 0;
static char *sensor_pinmux_mode   = "";
module_param(sensor_bus_type,      charp, S_IRUGO);
MODULE_PARM_DESC(sensor_bus_type,      "Sensor control bus: i2c or ssp");
module_param(sensor_clk_frequency, int,   S_IRUGO);
MODULE_PARM_DESC(sensor_clk_frequency, "Sensor clock frequency in Hz");
module_param(sensor_pinmux_mode,   charp, S_IRUGO);
MODULE_PARM_DESC(sensor_pinmux_mode,   "Sensor pinmux mode (i2c_mipi/ssp_mipi/i2c_dc/ssp_dc)");

/* The cv300 SoC routes the sensor MCLK and pinmux through devicetree-bound
 * resources: /media/sensor_device0 with `compatible = "hisilicon,hi35xx_sensor"`,
 * exposing a clock phandle and pinctrl states named "i2c_mipi", "ssp_mipi",
 * "i2c_dc", "ssp_dc", "sleep". The vendor's combined hi3516cv300_sensor.ko
 * registered a platform_driver matching that node; its probe ran of_clk_get +
 * clk_set_rate(sensor_clk_frequency) + clk_prepare_enable plus pinctrl_select
 * for sensor_pinmux_mode. Without that setup the sensor MCLK stays gated
 * (clk_summary shows `clk_sensor enable_cnt=0`), the IMX291 sees no clock
 * and NACKs every i2c write — the `hibvt-i2c wait idle abort, RIS:0x611`
 * flood — so MIPI never gets framed pixels, ISP stat buffers stay empty,
 * VENC times out. Replicate the vendor's wiring as a platform_driver here
 * so cv300 cameras stream again under the openhisilicon-built form. */

static struct clk *g_sensor_clk;
static struct pinctrl *g_pctrl;
static struct platform_device *g_pdev;

static int hi35xx_sensor_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct pinctrl_state *state;
	const char *pinmux_name;

	/* Cache for unwind on driver remove (module_exit isn't always reached
	 * before the device goes away during late shutdown). */
	g_pdev = pdev;

	g_sensor_clk = of_clk_get(np, 0);
	if (IS_ERR(g_sensor_clk)) {
		dev_err(dev, "of_clk_get failed (%ld)\n", PTR_ERR(g_sensor_clk));
		g_sensor_clk = NULL;
	} else {
		if (sensor_clk_frequency > 0)
			clk_set_rate(g_sensor_clk, sensor_clk_frequency);
		clk_prepare_enable(g_sensor_clk);
	}

	g_pctrl = devm_pinctrl_get(dev);
	if (IS_ERR(g_pctrl)) {
		dev_warn(dev, "devm_pinctrl_get failed (%ld)\n", PTR_ERR(g_pctrl));
		g_pctrl = NULL;
	} else {
		pinmux_name = (sensor_pinmux_mode && *sensor_pinmux_mode)
			? sensor_pinmux_mode : "i2c_mipi";
		state = pinctrl_lookup_state(g_pctrl, pinmux_name);
		if (IS_ERR(state)) {
			dev_warn(dev, "pinctrl state '%s' not found (%ld)\n",
				 pinmux_name, PTR_ERR(state));
		} else {
			pinctrl_select_state(g_pctrl, state);
		}
	}

	return 0;
}

static compat_platform_remove_ret hi35xx_sensor_remove(struct platform_device *pdev)
{
	if (g_sensor_clk) {
		clk_disable_unprepare(g_sensor_clk);
		clk_put(g_sensor_clk);
		g_sensor_clk = NULL;
	}
	g_pctrl = NULL;
	g_pdev = NULL;
	compat_platform_remove_return;
}

static const struct of_device_id hi35xx_sensor_match[] = {
	{ .compatible = "hisilicon,hi35xx_sensor" },
	{}
};
MODULE_DEVICE_TABLE(of, hi35xx_sensor_match);

static struct platform_driver hi35xx_sensor_driver = {
	.driver = {
		.name = "hi35xx_sensor",
		.of_match_table = hi35xx_sensor_match,
	},
	.probe  = hi35xx_sensor_probe,
	.remove = hi35xx_sensor_remove,
};

/* On insmod we (1) register the platform_driver so the sensor clock + pinmux
 * gets wired up via DT, then (2) bring the i2c bus driver up and register the
 * ISP read/write callback. Order matters: i2c_drv_init's i2c_new_device call
 * needs the bus to actually be alive (i.e. clock enabled, pinmux selected),
 * otherwise the very first transaction NACKs and the sensor stays unreachable.
 */
static int __init sensor_mod_init(void)
{
	HI_S32 ret;
	ISP_BUS_CALLBACK_S stBusCb = {0};
	SENSOR_CTRL_OPS_S stCtrlOps = {0};

	ret = platform_driver_register(&hi35xx_sensor_driver);
	if (ret) {
		osal_printk("hi3516cv300_sensor: platform_driver_register failed (%d)\n", ret);
		return ret;
	}

	if (sensor_bus_type && 0 == strcmp(sensor_bus_type, "ssp")) {
		ret = ssp_drv_init(0);
		if (ret) {
			osal_printk("hi3516cv300_sensor: ssp_drv_init failed (%d)\n", ret);
			platform_driver_unregister(&hi35xx_sensor_driver);
			return ret;
		}
		ssp_get_ops(0, &stCtrlOps);
	} else {
		ret = i2c_drv_init(0);
		if (ret) {
			osal_printk("hi3516cv300_sensor: i2c_drv_init failed (%d)\n", ret);
			platform_driver_unregister(&hi35xx_sensor_driver);
			return ret;
		}
		i2c_get_ops(0, &stCtrlOps);
	}

	stBusCb.pfnISPWriteSensorData = stCtrlOps.write;
	stBusCb.pfnISPReadSensorData  = stCtrlOps.read;
	if (CKFN_ISP_RegisterBusCallBack()) {
		CALL_ISP_RegisterBusCallBack(0, &stBusCb);
	}

	osal_printk("load hi3516cv300_sensor.ko ... OK!\n");
	return 0;
}

static void __exit sensor_mod_exit(void)
{
	if (sensor_bus_type && 0 == strcmp(sensor_bus_type, "ssp")) {
		ssp_drv_exit(0);
	} else {
		i2c_drv_exit(0);
	}
	platform_driver_unregister(&hi35xx_sensor_driver);
	osal_printk("unload hi3516cv300_sensor.ko ... OK!\n");
}

module_init(sensor_mod_init);
module_exit(sensor_mod_exit);

/* The kernel's i2c_new_device / i2c_unregister_device are EXPORT_SYMBOL_GPL,
 * so this module must declare a GPL-compatible license or insmod fails with
 * "Unknown symbol" on those calls in i2c_drv_init / i2c_drv_exit.
 */
MODULE_LICENSE("GPL");
