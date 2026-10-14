/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __MSENSOR_GPIO_H__
#define __MSENSOR_GPIO_H__

#include "xmedia_type.h"
#include <linux/gpio.h>


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#define gpio_num(gpio_chip_num, gpio_offset_num) ((gpio_chip_num) * 8 + (gpio_offset_num))

xmedia_s32 gpio_init(xmedia_void);
xmedia_s32 gpio_deinit(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

