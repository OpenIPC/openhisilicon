/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DISPATCH_KM_H
#define DISPATCH_KM_H

#include "crypto_osal_lib.h"
#include "crypto_osal_adapt.h"
#include "ioctl_km.h"

crypto_ioctl_cmd *get_km_func_list(td_void);

td_u32 get_km_func_num(td_void);

#endif