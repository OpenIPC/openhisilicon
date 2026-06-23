/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_common.h"

#include "crypto_ioctl_cmd.h"
#include "crypto_osal_user_lib.h"

static crypto_mutex_t g_crypto_mutex = CRYPTO_PTHREAD_MUTEX_INITIALIZER;

#define crypto_common_lock()   (void)crypto_pthread_mutex_lock(&g_crypto_mutex)
#define crypto_common_unlock() (void)crypto_pthread_mutex_unlock(&g_crypto_mutex)

static td_u32 g_init_counter = 0;
#define CRYPTO_INIT_MAX_NUM 0xffffffff

static td_s32 g_dev_fd = -1;

int crypto_cipher_open(void)
{
    int ret = TD_FAILURE;
    crypto_common_lock();
    if (g_init_counter >= CRYPTO_INIT_MAX_NUM) {
        ret = ERROR_COUNT_OVERFLOW;
        goto exit_unlock;
    }

    if (g_init_counter > 0) {
        g_init_counter++;
        ret = TD_SUCCESS;
        goto exit_unlock;
    }

    g_dev_fd = crypto_open("/dev/" CRYPTO_CIPHER_DEV_NAME, O_RDWR, 0);
    if (g_dev_fd < 0) {
        ret = ERROR_DEV_OPEN_FAILED;
        goto exit_unlock;
    }
    g_init_counter++;

    ret = TD_SUCCESS;
exit_unlock:
    crypto_common_unlock();
    return ret;
}

void crypto_cipher_close(void)
{
    crypto_common_lock();

    if (g_init_counter > 0) {
        g_init_counter--;
    }
    if (g_init_counter == 0) {
        crypto_close(g_dev_fd);
        g_dev_fd = -1;
    }
    crypto_common_unlock();
}

int crypto_cipher_ioctl(int cmd, void *args)
{
    return crypto_ioctl(g_dev_fd, cmd, args);
}

int crypto_cipher_is_init(void)
{
    if (g_init_counter == 0) {
        return 0;
    }
    return 1;
}