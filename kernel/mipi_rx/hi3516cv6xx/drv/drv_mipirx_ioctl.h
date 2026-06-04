/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef DRV_MIPIRX_IOCTL_H
#define DRV_MIPIRX_IOCTL_H

#include "drv_mipirx_osal.h"

#define MIPIRX_DEV_NAME "ot_mipi_rx"

td_s32 drv_mipirx_ioctl_init(td_void);
td_void drv_mipirx_ioctl_deinit(td_void);

#endif
