/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * ive_internal.h — shared internal declarations for libive_neo.
 */
#ifndef __LIBIVE_NEO_INTERNAL_H__
#define __LIBIVE_NEO_INTERNAL_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_ive.h"

#include <stdio.h>
#include <string.h>

#define IVE_DEV_PATH    "/dev/ive"

extern int g_ive_fd;

/* Open /dev/ive (lazy + thread-safe). Returns HI_SUCCESS or HI_FAILURE. */
HI_S32 ive_open_fd(void);

/* Tag-style logger — emits to stderr in the same format as vendor. */
#define IVE_LOG(fn, fmt, ...)                                            \
    do {                                                                 \
        fprintf(stderr, "[Func]:%s [Line]:%d [Info]:", (fn), __LINE__);  \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__);                        \
    } while (0)

/* Quick NULL check that logs and returns illegal-param. */
#define IVE_CHECK_NULL(fn, ptr, name)                                    \
    do {                                                                 \
        if ((ptr) == NULL) {                                             \
            IVE_LOG(fn, "%s can't be NULL", (name));                     \
            return HI_ERR_IVE_NULL_PTR;                                  \
        }                                                                \
    } while (0)

/* Validate src image: non-NULL, non-zero w/h, valid stride. */
static inline HI_S32 ive_check_image(const IVE_IMAGE_S *img)
{
    if (img == NULL)
        return HI_ERR_IVE_NULL_PTR;
    if (img->u32Width == 0 || img->u32Height == 0)
        return HI_ERR_IVE_ILLEGAL_PARAM;
    if (img->au32Stride[0] == 0)
        return HI_ERR_IVE_ILLEGAL_PARAM;
    if (img->au64PhyAddr[0] == 0)
        return HI_ERR_IVE_BADADDR;
    return HI_SUCCESS;
}

#endif /* __LIBIVE_NEO_INTERNAL_H__ */
