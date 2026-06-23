/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef UAPI_COMMON_H
#define UAPI_COMMON_H

#include "crypto_platform.h"

int crypto_cipher_open(void);

void crypto_cipher_close(void);

int crypto_cipher_ioctl(int cmd, void *args);

int crypto_cipher_is_init(void);

#endif