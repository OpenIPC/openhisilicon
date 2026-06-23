/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_DISPATCH_H
#define CRYPTO_DISPATCH_H

#include "crypto_type.h"

#include "crypto_osal_adapt.h"

crypto_ioctl_cmd *crypto_get_ioctl_cmd_list(td_void);

td_u32 crypto_get_ioctl_cmd_cnt(td_void);

#endif