#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "xmedia_debug.h"
#include "xmedia_msensor.h"
#include "msensor_chip_ioctl.h"
#include "msensor_mng_ioctl.h"

#define MSENSOR_CHIP_NAME   "/dev/msensor_chip"
#define MSENSOR_MNG_NAME    "/dev/msensor_mng"

static xmedia_bool g_msensor_tgid_init = XMEDIA_FALSE;
static xmedia_s32 g_msensor_chip_fd[XMEDIA_MSENSOR_MAX_DEV_NUM] = { [0 ...(XMEDIA_MSENSOR_MAX_DEV_NUM - 1)] = -1 };
static xmedia_s32 g_msensor_mng_fd[XMEDIA_MSENSOR_MAX_DEV_NUM] = { [0 ...(XMEDIA_MSENSOR_MAX_DEV_NUM - 1)] = -1 };

static pthread_mutex_t g_msensor_mutex = PTHREAD_MUTEX_INITIALIZER;

#define MSENSOR_MUTEX_LOCK()                      \
    do {                                     \
        (void)pthread_mutex_lock(&g_msensor_mutex); \
    } while (0)

#define MSENSOR_MUTEX_UNLOCK()                      \
    do {                                       \
        (void)pthread_mutex_unlock(&g_msensor_mutex); \
    } while (0)

#define MSENSOR_TRACE(level, fmt, ...)                                                                                 \
    do {                                                                                                          \
        printf("[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define MSENSOR_CHECK_INIT_RETURN()               \
    do {                                     \
        MSENSOR_MUTEX_LOCK();                     \
        if (g_msensor_tgid_init != XMEDIA_TRUE) { \
            printf("msensor not init!\n");        \
            MSENSOR_MUTEX_UNLOCK();               \
            return XMEDIA_ERRCODE_NOT_INIT;  \
        }                                    \
        MSENSOR_MUTEX_UNLOCK();                   \
    } while (0)

static xmedia_s32 msensor_check_dev(xmedia_s32 dev)
{
    if ((dev >= XMEDIA_MSENSOR_MAX_DEV_NUM) || (dev < 0)) {
        MSENSOR_TRACE(MODULE_DBG_ERR, "dev id(%d) is invalid\r\n", dev);
        return XMEDIA_ERRCODE_INVALID_PIPE_ID;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_check_null_ptr(xmedia_void *ptr)
{
    if (ptr == XMEDIA_NULL) {
        MSENSOR_TRACE(MODULE_DBG_ERR, "null pointer\n");
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_check_chip_open(xmedia_s32 dev)
{
    MSENSOR_MUTEX_LOCK();

    if (g_msensor_chip_fd[dev] < 0) {
        xmedia_s32 arg = dev;
        g_msensor_chip_fd[dev] = open(MSENSOR_CHIP_NAME, O_RDONLY);
        if (g_msensor_chip_fd[dev] < 0) {
            MSENSOR_MUTEX_UNLOCK();
            MSENSOR_TRACE(MODULE_DBG_ERR, "open msensor dev %d err, ret %d\n", dev, g_msensor_chip_fd[dev]);
            return XMEDIA_ERRCODE_NOT_READY;
        }
        if (ioctl(g_msensor_chip_fd[dev], MSENSOR_CMD_CHIP_BIND_FLAG2FD, &arg)) {
            close(g_msensor_chip_fd[dev]);
            g_msensor_chip_fd[dev] = -1;
            MSENSOR_MUTEX_UNLOCK();
            MSENSOR_TRACE(MODULE_DBG_ERR, "msensor dev %d, ret %d\n", dev, g_msensor_chip_fd[dev]);
            return XMEDIA_ERRCODE_NOT_READY;
        }
    }
    MSENSOR_MUTEX_UNLOCK();

    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_check_mng_open(xmedia_s32 dev)
{
    MSENSOR_MUTEX_LOCK();

    if (g_msensor_mng_fd[dev] < 0) {
        xmedia_s32 arg = dev;
        g_msensor_mng_fd[dev] = open(MSENSOR_MNG_NAME, O_RDONLY);
        if (g_msensor_mng_fd[dev] < 0) {
            MSENSOR_MUTEX_UNLOCK();
            MSENSOR_TRACE(MODULE_DBG_ERR, "open msensor dev %d err, ret %d\n", dev, g_msensor_mng_fd[dev]);
            return XMEDIA_ERRCODE_NOT_READY;
        }
        if (ioctl(g_msensor_mng_fd[dev], MSENSOR_CMD_MNG_BIND_FLAG2FD, &arg)) {
            close(g_msensor_mng_fd[dev]);
            g_msensor_mng_fd[dev] = -1;
            MSENSOR_MUTEX_UNLOCK();
            MSENSOR_TRACE(MODULE_DBG_ERR, "msensor dev %d, ret %d\n", dev, g_msensor_mng_fd[dev]);
            return XMEDIA_ERRCODE_NOT_READY;
        }
    }
    MSENSOR_MUTEX_UNLOCK();

    return XMEDIA_SUCCESS;
}

#define MSENSOR_CHECK_NULL_PTR_RETURN(ptr)                             \
    do {                                                          \
        xmedia_s32 ret = msensor_check_null_ptr((xmedia_void *)(ptr)); \
        if (ret != XMEDIA_SUCCESS) {                              \
            return ret;                                           \
        }                                                         \
    } while (0)


#define MSENSOR_CHECK_DEV_ID_RETURN(dev)            \
    do {                                         \
        xmedia_s32 ret = msensor_check_dev(dev); \
        if (ret != XMEDIA_SUCCESS) {             \
            return ret;                          \
        }                                        \
    } while (0)

#define MSENSOR_CHECK_CHIP_OPEN_RETURN(dev)            \
    do {                                         \
        xmedia_s32 ret = msensor_check_chip_open(dev); \
        if (ret != XMEDIA_SUCCESS) {             \
            return ret;                          \
        }                                        \
    } while (0)

#define MSENSOR_CHECK_MNG_OPEN_RETURN(dev)            \
            do {                                         \
                xmedia_s32 ret = msensor_check_mng_open(dev); \
                if (ret != XMEDIA_SUCCESS) {             \
                    return ret;                          \
                }                                        \
            } while (0)

xmedia_s32 xmedia_msensor_init(xmedia_void)
{
    MSENSOR_MUTEX_LOCK();

    if (g_msensor_tgid_init == XMEDIA_TRUE) {
        MSENSOR_MUTEX_UNLOCK();
        return XMEDIA_SUCCESS;
    }

    g_msensor_tgid_init = XMEDIA_TRUE;

    MSENSOR_MUTEX_UNLOCK();
    return XMEDIA_SUCCESS;
}

xmedia_void xmedia_msensor_exit(xmedia_void)
{
    xmedia_s32 i;

    MSENSOR_MUTEX_LOCK();

    if (g_msensor_tgid_init == XMEDIA_FALSE) {
        MSENSOR_MUTEX_UNLOCK();
        return ;
    }

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        if (g_msensor_mng_fd[i] >= 0) {
            close(g_msensor_mng_fd[i]);
            g_msensor_mng_fd[i] = -1;
        }
    }

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        if (g_msensor_chip_fd[i] >= 0) {
            close(g_msensor_chip_fd[i]);
            g_msensor_chip_fd[i] = -1;
        }
    }

    g_msensor_tgid_init = XMEDIA_FALSE;

    MSENSOR_MUTEX_UNLOCK();
    return;
}

xmedia_s32 xmedia_msensor_create_dev(xmedia_s32 dev, const xmedia_msensor_param *param)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);
    MSENSOR_CHECK_NULL_PTR_RETURN(param);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);

    return ioctl(g_msensor_chip_fd[dev], MSENSOR_CMD_CREATE, param);
}

xmedia_s32 xmedia_msensor_destroy_dev(xmedia_s32 dev)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);

    return ioctl(g_msensor_chip_fd[dev], MSENSOR_CMD_DESTROY);
}

xmedia_s32 xmedia_msensor_get_param(xmedia_s32 dev, xmedia_msensor_param *param)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);
    MSENSOR_CHECK_NULL_PTR_RETURN(param);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);

    return ioctl(g_msensor_chip_fd[dev], MSENSOR_CMD_GET_PARAM, param);
}

xmedia_s32 xmedia_msensor_start_dev(xmedia_s32 dev)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);

    return ioctl(g_msensor_chip_fd[dev], MSENSOR_CMD_START);
}

xmedia_s32 xmedia_msensor_stop_dev(xmedia_s32 dev)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);

    return ioctl(g_msensor_chip_fd[dev], MSENSOR_CMD_STOP);
}

xmedia_s32 xmedia_msensor_add_user(xmedia_s32 dev, xmedia_s32 *user_id)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);
    MSENSOR_CHECK_MNG_OPEN_RETURN(dev);

    return ioctl(g_msensor_mng_fd[dev], MSENSOR_CMD_ADD_USER, user_id);
}

xmedia_s32 xmedia_msensor_delete_user(xmedia_s32 dev, xmedia_s32 user_id)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);
    MSENSOR_CHECK_MNG_OPEN_RETURN(dev);

    return ioctl(g_msensor_mng_fd[dev], MSENSOR_CMD_DELETE_USER, &user_id);
}

xmedia_s32 xmedia_msensor_get_data(xmedia_s32 dev, xmedia_msensor_data_info *data)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);
    MSENSOR_CHECK_NULL_PTR_RETURN(data);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);
    MSENSOR_CHECK_MNG_OPEN_RETURN(dev);

    return ioctl(g_msensor_mng_fd[dev], MSENSOR_CMD_GET_DATA, data);
}

xmedia_s32 xmedia_msensor_send_data(xmedia_s32 dev, const xmedia_msensor_data_info *data)
{
    MSENSOR_CHECK_DEV_ID_RETURN(dev);
    MSENSOR_CHECK_NULL_PTR_RETURN(data);

    MSENSOR_CHECK_INIT_RETURN();
    MSENSOR_CHECK_CHIP_OPEN_RETURN(dev);
    MSENSOR_CHECK_MNG_OPEN_RETURN(dev);

    return ioctl(g_msensor_mng_fd[dev], MSENSOR_CMD_SEND_DATA, data);
}

