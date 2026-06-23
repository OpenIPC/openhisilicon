/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_COMM_H
#define GFBG_COMM_H

#include "ot_type.h"
#include "gfbg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define gfbg_unequal_eok_return(ret)                       \
    do {                                                   \
        if ((ret) != EOK) {                                \
            gfbg_error("secure function failure\n");       \
            return TD_FAILURE;                             \
        }                                                  \
    } while (0)

#define gfbg_unequal_eok_return_void(ret)                  \
    do {                                                   \
        if ((ret) != EOK) {                                \
            gfbg_error("secure function failure\n");       \
            return;                             \
        }                                                  \
    } while (0)

#define gfbg_unlock_unequal_eok_return(ret, lock, lock_flag)           \
    do {                                                               \
        if ((ret) != EOK) {                                            \
            gfbg_error("secure function failure\n");                   \
            osal_spin_unlock_irqrestore(lock, lock_flag);              \
            return TD_FAILURE;                                         \
        }                                                              \
    } while (0)

#define gfbg_unlock_unequal_eok_return_void(ret, lock, lock_flag)      \
    do {                                                               \
        if ((ret) != EOK) {                                            \
            gfbg_error("secure function failure\n");                   \
            osal_spin_unlock_irqrestore(lock, lock_flag);              \
            return;                                                    \
        }                                                              \
    } while (0)

typedef struct {
    td_bool is_losslessa;
    td_bool is_lossless;
    td_u32 width;
    ot_fb_color_format format;
} gfbg_stride_attr;

td_void gfbg_recalculate_stride(td_u32 *cmp_stride, td_u32 *uncmp_stride, const gfbg_stride_attr *attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_COMM_H */
