/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * ive_fd.c — /dev/ive file-descriptor management for libive_neo.
 *
 * Mirrors vendor libive.so's ive_check_fd_open() pattern: a single
 * cached fd, opened on first use under a pthread mutex.
 */
#include "ive_internal.h"

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

int g_ive_fd = -1;
static pthread_mutex_t g_ive_fd_lock = PTHREAD_MUTEX_INITIALIZER;

HI_S32 ive_open_fd(void)
{
    HI_S32 ret = HI_SUCCESS;

    pthread_mutex_lock(&g_ive_fd_lock);
    if (g_ive_fd < 0) {
        g_ive_fd = open(IVE_DEV_PATH, O_RDWR);
        if (g_ive_fd < 0) {
            IVE_LOG("ive_open_fd", "open(%s) failed: %s",
                    IVE_DEV_PATH, strerror(errno));
            ret = HI_FAILURE;
        }
    }
    pthread_mutex_unlock(&g_ive_fd_lock);
    return ret;
}

/* Library destructor: close the cached fd cleanly when libive_neo unloads.
 * Not strictly required (the kernel will release on process exit) but
 * matches vendor behavior. */
__attribute__((destructor))
static void ive_fini(void)
{
    pthread_mutex_lock(&g_ive_fd_lock);
    if (g_ive_fd >= 0) {
        close(g_ive_fd);
        g_ive_fd = -1;
    }
    pthread_mutex_unlock(&g_ive_fd_lock);
}
