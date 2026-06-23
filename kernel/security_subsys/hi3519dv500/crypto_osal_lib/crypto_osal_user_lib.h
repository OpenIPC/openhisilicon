/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_OSAL_USER_LIB_H
#define CRYPTO_OSAL_USER_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ot_type.h"

#define crypto_print            printf
#define crypto_log_err(fmt, ...)  crypto_log_fmt("ERROR", fmt, ##__VA_ARGS__)
#define crypto_log_fmt(LOG_LEVEL_LABEL, fmt, ...)   \
    crypto_print("[%s:%d]" LOG_LEVEL_LABEL ": " fmt, __func__, __LINE__, ##__VA_ARGS__)

#define crypto_malloc(x)          (((x) > 0) ? malloc((x)) : TD_NULL)
#define crypto_free(x)            {if (((x) != TD_NULL)) free((x));}

#define crypto_open             open
#define crypto_close            close
#define crypto_ioctl            ioctl
#define crypto_km_ioctl         ioctl
#define crypto_otp_ioctl        ioctl

typedef pthread_mutex_t                             crypto_mutex_t;
#define crypto_pthread_mutex_lock(lock)             pthread_mutex_lock(lock)
#define crypto_pthread_mutex_unlock(lock)           pthread_mutex_unlock(lock)
#define CRYPTO_PTHREAD_MUTEX_INITIALIZER            PTHREAD_MUTEX_INITIALIZER

#define CRYPTO_ERROR_ENV                            ERROR_ENV_LINUX

#endif