/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#if 0
int osal_platform_driver_register(struct platform_driver *drv)
{
    return __platform_driver_register(drv, THIS_MODULE);
}
void osal_platform_driver_unregister(struct platform_driver *drv)
{
    return platform_driver_unregister(drv);
}
struct resource *osal_platform_get_resource_byname(struct platform_device *dev, unsigned int type, const char *name)
{
    return platform_get_resource_byname(dev, type, name);
}

#else
int osal_platform_driver_register(void *drv)
{
    return __platform_driver_register((struct platform_driver *)drv, THIS_MODULE);
}
EXPORT_SYMBOL(osal_platform_driver_register);

void osal_platform_driver_unregister(void *drv)
{
    return platform_driver_unregister((struct platform_driver *)drv);
}
EXPORT_SYMBOL(osal_platform_driver_unregister);

void *osal_platform_get_resource_byname(void *dev, unsigned int type,
                                        const char *name)
{
    return (void *)platform_get_resource_byname((struct platform_device *)dev, type, name);
}
EXPORT_SYMBOL(osal_platform_get_resource_byname);

void *osal_platform_get_resource(void *dev, unsigned int type, unsigned int num)
{
    return (void *)platform_get_resource((struct platform_device *)dev, type, num);
}
EXPORT_SYMBOL(osal_platform_get_resource);

int osal_platform_get_irq(void *dev, unsigned int num)
{
    return platform_get_irq((struct platform_device *)dev, num);
}
EXPORT_SYMBOL(osal_platform_get_irq);

int osal_platform_get_irq_byname(void *dev, const char *name)
{
    return platform_get_irq_byname((struct platform_device *)dev, name);
}
EXPORT_SYMBOL(osal_platform_get_irq_byname);

#endif
