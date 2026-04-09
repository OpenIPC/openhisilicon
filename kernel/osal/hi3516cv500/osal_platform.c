/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal platform source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>

int osal_platform_get_modparam_uint(void *pdev, const char *name, unsigned int *value)
{
    struct device_node *np = ((struct platform_device*)pdev)->dev.of_node;
    unsigned int temp_value;

    if (of_property_read_u32(np, name, &temp_value)) {
        return -1;
    }

    *value = temp_value;
    return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_uint);

int osal_platform_get_modparam_int(void *pdev, const char *name, int *value)
{
    struct device_node *np = ((struct platform_device*)pdev)->dev.of_node;
    unsigned int temp_value;

    if (of_property_read_u32(np, name, &temp_value)) {
        return -1;
    }

    *value = (int)temp_value;
    return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_int);

int osal_platform_get_modparam_uchar(void *pdev, const char *name, unsigned char *value)
{
    struct device_node *np = ((struct platform_device*)pdev)->dev.of_node;
    unsigned char temp_value;

    if (of_property_read_u8(np, name, &temp_value)) {
        return -1;
    }

    *value = temp_value;
    return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_uchar);

int osal_platform_get_modparam_ushort(void *pdev, const char *name, unsigned short *value)
{
    struct device_node *np = ((struct platform_device*)pdev)->dev.of_node;
    unsigned short temp_value;

    if (of_property_read_u16(np, name, &temp_value)) {
        return -1;
    }

    *value = temp_value;
    return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_ushort);

int osal_platform_get_modparam_charp(void *pdev, const char *name, char *value)
{
    struct device_node *np = ((struct platform_device*)pdev)->dev.of_node;
    const char *string = NULL;

    if (of_property_read_string(np, name, &string)) {
        return -1;
    }

    if (value == NULL) {
        printk("%s: module parameter %s is null pointer!!\n", __func__, name);
        return -1;
    }

    strcpy(value, string);
    return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_charp);

int osal_platform_get_modparam_string(void *pdev, const char *name, unsigned int size, char *value)
{
    struct device_node *np = ((struct platform_device*)pdev)->dev.of_node;
    const char *string = NULL;

    if (of_property_read_string(np, name, &string)) {
        return -1;
    }

    strncpy(value, string, size);
    return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_string);

int osal_of_property_read_u32(const void *np, const char *propname, unsigned int *value)
{
    return of_property_read_u32((struct device_node*)np, propname, value);
}
EXPORT_SYMBOL(osal_of_property_read_u32);

int osal_platform_driver_register(void *drv)
{
    return __platform_driver_register((struct platform_driver*)drv, THIS_MODULE);
}
EXPORT_SYMBOL(osal_platform_driver_register);

void osal_platform_driver_unregister(void *drv)
{
    return platform_driver_unregister((struct platform_driver*)drv);
}
EXPORT_SYMBOL(osal_platform_driver_unregister);

void *osal_platform_get_resource_byname(void *dev, unsigned int type,
                                        const char *name)
{
    return (void*)platform_get_resource_byname((struct platform_device*)dev, type, name);
}
EXPORT_SYMBOL(osal_platform_get_resource_byname);

void *osal_platform_get_resource(void *dev, unsigned int type, unsigned int num)
{
    return (void*)platform_get_resource((struct platform_device*)dev, type, num);
}
EXPORT_SYMBOL(osal_platform_get_resource);

int osal_platform_get_irq(void *dev, unsigned int num)
{
    return platform_get_irq((struct platform_device*)dev, num);
}
EXPORT_SYMBOL(osal_platform_get_irq);

int osal_platform_get_irq_byname(void *dev, const char *name)
{
    return platform_get_irq_byname((struct platform_device*)dev, name);
}
EXPORT_SYMBOL(osal_platform_get_irq_byname);
