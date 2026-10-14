/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __MSENSOR_CHIP_IOCTL_H__
#define __MSENSOR_CHIP_IOCTL_H__

#include "xmedia_msensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define MSENSOR_TYPE_CHIP 12

typedef enum {
    IOC_NR_MSENSOR_CHIP_BIND_FLAG2FD = 0,
    IOC_NR_MSENSOR_CHIP_CREATE,
    IOC_NR_MSENSOR_CHIP_DESTROY,
    IOC_NR_MSENSOR_CHIP_START,
    IOC_NR_MSENSOR_CHIP_STOP,
    IOC_NR_MSENSOR_CHIP_GET_PARAM,
    IOC_NR_MSENSOR_CHIP_MNG_INIT,
    IOC_NR_MSENSOR_CHIP_MAX
} ioc_nr_msensor_chip;

#define MSENSOR_CMD_CHIP_BIND_FLAG2FD   _IOW(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_BIND_FLAG2FD, xmedia_s32)
#define MSENSOR_CMD_START       _IO(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_START)
#define MSENSOR_CMD_STOP        _IO(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_STOP)
#define MSENSOR_CMD_CREATE      _IOW(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_CREATE, xmedia_msensor_param)
#define MSENSOR_CMD_DESTROY     _IO(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_DESTROY)
#define MSENSOR_CMD_GET_PARAM   _IOR(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_GET_PARAM, xmedia_msensor_param)
#define MSENSOR_CMD_MNG_INIT    _IOW(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_MNG_INIT, xmedia_msensor_param)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

