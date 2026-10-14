/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __MSENSOR_BUF_H__
#define __MSENSOR_BUF_H__

#include "msensor_ext.h"
#include "osal.h"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#define BUF_BLOCK_NUM 6
#define MAX_USER_NUM  10

/*
 * min msensor gap between reader pointer and write pointer,
 * to prevent new data overlap with reading/processing data
 */
#define WR_GAP 100

typedef struct {
    xmedia_void *start_addr; /* start address */
    xmedia_void *write_ptr; /* write pointer */
} msensor_buf_info;

typedef enum {
    DATA_X,
    DATA_Y,
    DATA_Z,
    DATA_TEMP,
    DATA_PTS,
    DATA_BUTT
} msensor_buf_data_type;

typedef struct {
    xmedia_void *read_ptr[XMEDIA_MSENSOR_DATA_TYPE_MAX][DATA_BUTT];
    xmedia_s32 reverd3[4]; /* array 4 */
} msensor_buf_user_context;

typedef struct {
    xmedia_u32 user_cnt;
    osal_spinlock_t mng_lock;
    osal_spinlock_t read_lock[MAX_USER_NUM];
    msensor_buf_user_context *user_ctx[MAX_USER_NUM];
    osal_mutex_t mng_mutex;
} msensor_buf_user_mng;

msensor_buf_info **msensor_buf_get_info(xmedia_s32 dev);
osal_spinlock_t *msensor_buf_get_lock(xmedia_s32 dev);

xmedia_s32 msensor_buf_lock_init(xmedia_void);
xmedia_void msensor_buf_lock_exit(xmedia_void);
xmedia_s32 msensor_buf_init(xmedia_s32 dev,  const xmedia_msensor_buf_attr *buf_attr,
    xmedia_u32 gyro_freq, xmedia_u32 accel_freq, xmedia_u32 mag_freq);
xmedia_s32 msensor_buf_exit(xmedia_s32 dev);
xmedia_s32 msensor_buf_write_data(xmedia_s32 dev, xmedia_msensor_data_type data_type,
    const xmedia_msensor_sample_data *sample_data);
xmedia_s32 msensor_buf_get_data(xmedia_s32 dev, xmedia_msensor_data_info *msensor_data);
xmedia_s32 msensor_buf_release_data(xmedia_s32 dev, xmedia_msensor_data_info *msensor_data_info);
xmedia_s32 msensor_buf_add_user(xmedia_s32 dev, xmedia_s32 *id);
xmedia_s32 msensor_buf_delete_user(xmedia_s32 dev, const xmedia_s32 *id);
xmedia_bool msensor_buf_get_status(xmedia_s32 dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

