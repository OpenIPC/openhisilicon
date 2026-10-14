#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/pwm.h>
#include "common.h"
#include "osal.h"
#include "xmedia_errcode.h"
#include "drv_dev_defines.h"
#include "drv_misc.h"

#ifdef SUPPORT_OSD_TIME
#include "drv_osd_draw_time.h"
#endif
#include "drv_export.h"
#include "drv_sensor.h"

static osal_dev_t *g_misc_dev = XMEDIA_NULL;

xmedia_s32 drv_misc_open(xmedia_void *private_data)
{
    return XMEDIA_SUCCESS;
}

xmedia_s32 drv_misc_close(xmedia_void *private_data)
{
    return XMEDIA_SUCCESS;
}

xmedia_s32 drv_misc_suspend(osal_dev_t *pdev)
{
    return XMEDIA_SUCCESS;
}

xmedia_s32 drv_misc_resume(osal_dev_t *pdev)
{
#ifdef SUPPORT_OSD_TIME
    osd_time_update_all();
#endif

    misc_sensor_resume();

    return 0;
}

misc_ioctl_func_item g_misc_ctl_func_item[] = {
    {XMEDIA_MISC_AOV_INIT, misc_ctl_aov_init},
    {XMEDIA_MISC_AOV_SET_SENSOR_MODE, misc_ctl_aov_set_sensor_mode},
#ifdef SUPPORT_OSD_TIME
    {XMEDIA_MISC_OSD_TIME_CREATE_CTRL, misc_ctl_osd_time_create},
    {XMEDIA_MISC_OSD_TIME_UPDATE_CTRL, misc_ctl_osd_time_update},
    {XMEDIA_MISC_OSD_TIME_UPDATE_TIMEZONE_CTRL, misc_ctl_osd_time_update_timezone},
    {XMEDIA_MISC_OSD_TIME_DESTROY_CTRL, misc_ctl_osd_time_destroy},
#endif

#ifdef SUPPORT_SENSOR_PWDN_MODE
    {XMEDIA_MISC_SENSOR_PWDN_STANDBY, misc_ctl_sensor_pwdn_standby},
    {XMEDIA_MISC_SENSOR_PWDN_RESUME, misc_ctl_sensor_pwdn_resume},
#endif
};

static xmedia_slong drv_misc_ioctl(xmedia_u32 cmd, xmedia_ulong arg, xmedia_void *private_data)
{
    xmedia_u32 i;

    for (i = 0; i < sizeof(g_misc_ctl_func_item)/sizeof(misc_ioctl_func_item); i++) {
        if (cmd == g_misc_ctl_func_item[i].cmd && g_misc_ctl_func_item[i].ptr_func != XMEDIA_NULL) {
            return g_misc_ctl_func_item[i].ptr_func(arg);
        }
    }

    MISC_TRACE(MODULE_DBG_ERR, "invalid cmd(0x%x)!\n", cmd);
    return XMEDIA_ERRCODE_NOT_PERMITTED;
}

#ifdef CONFIG_COMPAT
xmedia_slong drv_misc_compat_ioctl(xmedia_u32 cmd, xmedia_ulong arg, xmedia_void *private_data)
{
    return drv_misc_ioctl(cmd, arg, private_data);
}
#endif

static struct osal_fileops g_misc_fops = {
    .open = drv_misc_open,
    .unlocked_ioctl = drv_misc_ioctl,
    .release = drv_misc_close,
#ifdef CONFIG_COMPAT
    .compat_ioctl = drv_misc_compat_ioctl,
#endif
};

struct osal_pmops g_misc_drv_ops = {
    .pm_suspend = drv_misc_suspend,
    .pm_resume= drv_misc_resume,
};

xmedia_s32 drv_misc_modinit(xmedia_void)
{
    g_misc_dev = osal_createdev(UMAP_DEVNAME_MISC_BASE);
    if (g_misc_dev == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR,"pm create dev err!\n");
        goto out0;
    }
    g_misc_dev->fops = &g_misc_fops;
    g_misc_dev->minor = UMAP_MISC_MINOR_BASE;
    g_misc_dev->osal_pmops = &g_misc_drv_ops;

    if (osal_registerdevice(g_misc_dev) < 0) {
        MISC_TRACE(MODULE_DBG_ERR,"regist pm device err!\n");
        goto out1;
    }

    misc_sensor_init();

    osal_printk("load xm_misc.ko OK!\n");
    return XMEDIA_SUCCESS;

out1:
    osal_destroydev(g_misc_dev);
    g_misc_dev = XMEDIA_NULL;
out0:
    osal_printk("load xm_misc.ko err!\n");
    return XMEDIA_FAILURE;
}

xmedia_void drv_misc_modexit(xmedia_void)
{
    osal_deregisterdevice(g_misc_dev);
    osal_destroydev(g_misc_dev);
    g_misc_dev = XMEDIA_NULL;

    misc_sensor_exit();

    MISC_TRACE(MODULE_DBG_ERR,"unload xm_misc.ko OK!\n");
    return;
}

#ifdef MODULE

module_init(drv_misc_modinit);
module_exit(drv_misc_modexit);
MODULE_LICENSE("GPL");

#else

int __init misc_driver_init(void)
{
    return drv_misc_modinit();
}

#endif
