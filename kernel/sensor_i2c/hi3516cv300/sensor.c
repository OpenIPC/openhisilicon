#include <linux/module.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/err.h>

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

/* clk_sensor / clk_mipi handles obtained at module_init. The cv300 SoC's
 * sensor MCLK output is gated by the kernel CCF; until clk_prepare_enable
 * is called the sensor sees no clock and does not respond to i2c — the
 * symptom on real hardware is `hibvt-i2c wait idle abort` floods at the
 * point libsns_<sensor>.so writes sensor config. The vendor's combined
 * hi3516cv300_sensor.ko enabled these clocks at module_init from the
 * sensor_clk_frequency= insmod param; openhisilicon's split form lost
 * this setup. */
static struct clk *g_sensor_clk;
static struct clk *g_mipi_clk;

static void sensor_clk_enable(void)
{
	g_sensor_clk = clk_get(NULL, "clk_sensor");
	if (IS_ERR(g_sensor_clk)) {
		osal_printk("hi3516cv300_sensor: clk_get(clk_sensor) failed (%ld)\n", PTR_ERR(g_sensor_clk));
		g_sensor_clk = NULL;
	} else {
		if (sensor_clk_frequency > 0) {
			clk_set_rate(g_sensor_clk, sensor_clk_frequency);
		}
		clk_prepare_enable(g_sensor_clk);
	}

	g_mipi_clk = clk_get(NULL, "clk_mipi");
	if (IS_ERR(g_mipi_clk)) {
		osal_printk("hi3516cv300_sensor: clk_get(clk_mipi) failed (%ld)\n", PTR_ERR(g_mipi_clk));
		g_mipi_clk = NULL;
	} else {
		clk_prepare_enable(g_mipi_clk);
	}
}

static void sensor_clk_disable(void)
{
	if (g_mipi_clk) {
		clk_disable_unprepare(g_mipi_clk);
		clk_put(g_mipi_clk);
		g_mipi_clk = NULL;
	}
	if (g_sensor_clk) {
		clk_disable_unprepare(g_sensor_clk);
		clk_put(g_sensor_clk);
		g_sensor_clk = NULL;
	}
}

/* Mirror what cv200/hi3519v101 do at module_init: enable the SoC sensor and
 * mipi clocks (vendor's combined hi3516cv300_sensor.ko did this from the
 * sensor_clk_frequency insmod param), bring up the kernel-side i2c bus
 * driver (i2c_new_device on bus 0), and register the read/write callback
 * with ISP so it can issue sensor register accesses through us. Without
 * any of this, the SDK's ISP_QueryExposureInfo / VENC chain never gets
 * stat data — frames don't land in MIPI/VI/ISP because the sensor side
 * wasn't ever programmed at the kernel layer.
 */
static int __init sensor_mod_init(void)
{
	HI_S32 ret;
	ISP_BUS_CALLBACK_S stBusCb = {0};
	SENSOR_CTRL_OPS_S stCtrlOps = {0};

	sensor_clk_enable();

	if (sensor_bus_type && 0 == strcmp(sensor_bus_type, "ssp")) {
		ret = ssp_drv_init(0);
		if (ret) {
			osal_printk("hi3516cv300_sensor: ssp_drv_init failed (%d)\n", ret);
			sensor_clk_disable();
			return ret;
		}
		ssp_get_ops(0, &stCtrlOps);
	} else {
		ret = i2c_drv_init(0);
		if (ret) {
			osal_printk("hi3516cv300_sensor: i2c_drv_init failed (%d)\n", ret);
			sensor_clk_disable();
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
	sensor_clk_disable();
	osal_printk("unload hi3516cv300_sensor.ko ... OK!\n");
}

module_init(sensor_mod_init);
module_exit(sensor_mod_exit);

/* The kernel's i2c_new_device / i2c_unregister_device are EXPORT_SYMBOL_GPL,
 * so this module must declare a GPL-compatible license or insmod fails with
 * "Unknown symbol" on those calls in i2c_drv_init / i2c_drv_exit.
 */
MODULE_LICENSE("GPL");
