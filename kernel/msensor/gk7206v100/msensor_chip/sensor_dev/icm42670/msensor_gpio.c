/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor_gpio.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "icm42670.h"

xmedia_s32 gpio_init(xmedia_void)
{
    xmedia_s32 ret;

    ret = gpio_request(gpio_num(INT_GPIO_CHIP, INT_GPIO_OFFSET), "icm42670_irq");
    if (ret != XMEDIA_SUCCESS) {
        print_info("irq GPIO request failed: %d", ret);
        return ret;
    }

    gpio_direction_input(gpio_num(INT_GPIO_CHIP, INT_GPIO_OFFSET));

    return XMEDIA_SUCCESS;
}

xmedia_s32 gpio_deinit(xmedia_void)
{
    gpio_free(gpio_num(INT_GPIO_CHIP, INT_GPIO_OFFSET));

    return XMEDIA_SUCCESS;
}

