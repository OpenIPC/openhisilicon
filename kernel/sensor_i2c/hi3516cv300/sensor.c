#include <linux/module.h>

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

/* OpenIPC's load_hisilicon passes these as insmod parameters. We accept them
 * (so insmod doesn't reject the module) but the actual bus configuration is
 * driven later by user-space via the ISP callback path: the SDK populates
 * g_stSensorDev[index]->stCtrlBus, then invokes sensor_dev_init() which
 * dispatches into i2c_drv_init / ssp_drv_init based on the per-sensor bus
 * type. So this module just needs to load cleanly with module_init returning
 * 0 — the vendor's combined hi3516cv300_sensor.ko did the same.
 */
/* load_hisilicon passes these as strings (e.g. sensor_bus_type="i2c",
 * sensor_pinmux_mode="i2c_mipi") and a numeric Hz for the clock. */
static char *sensor_bus_type      = "";
static int   sensor_clk_frequency = 0;
static char *sensor_pinmux_mode   = "";
module_param(sensor_bus_type,      charp, S_IRUGO);
MODULE_PARM_DESC(sensor_bus_type,      "Sensor control bus: i2c or ssp");
module_param(sensor_clk_frequency, int,   S_IRUGO);
MODULE_PARM_DESC(sensor_clk_frequency, "Sensor clock frequency in Hz");
module_param(sensor_pinmux_mode,   charp, S_IRUGO);
MODULE_PARM_DESC(sensor_pinmux_mode,   "Sensor pinmux mode (i2c_mipi/ssp_mipi/i2c_dc/ssp_dc)");

static int __init sensor_mod_init(void)
{
	osal_printk("load hi3516cv300_sensor.ko ... OK!\n");
	return 0;
}

static void __exit sensor_mod_exit(void)
{
	osal_printk("unload hi3516cv300_sensor.ko ... OK!\n");
}

module_init(sensor_mod_init);
module_exit(sensor_mod_exit);

/* The kernel's i2c_new_device / i2c_unregister_device are EXPORT_SYMBOL_GPL,
 * so this module must declare a GPL-compatible license or insmod fails with
 * "Unknown symbol" on those calls in i2c_drv_init / i2c_drv_exit.
 */
MODULE_LICENSE("GPL");
