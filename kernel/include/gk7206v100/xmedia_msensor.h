/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_MSENSOR_H__
#define __XMEDIA_MSENSOR_H__

#include "xmedia_errcode.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define XMEDIA_MSENSOR_MAX_DEV_NUM  2

#define XMEDIA_MSENSOR_MAX_DATA_NUM 128
#define XMEDIA_MSENSOR_GRADIENT     (0x1 << 10)

#define XMEDIA_MSENSOR_TEMP_GYRO    0x1
#define XMEDIA_MSENSOR_TEMP_ACC     0x2
#define XMEDIA_MSENSOR_TEMP_ALL     0x3

#define XMEDIA_MSENSOR_DEVICE_GYRO  0x1
#define XMEDIA_MSENSOR_DEVICE_ACC   0x2
#define XMEDIA_MSENSOR_DEVICE_ALL   0x3

#define xmedia_unused(x)            ((xmedia_void)(x))

typedef struct {
    xmedia_u32 device_mask;
    xmedia_u32 temperature_mask;
} xmedia_msensor_attr;

typedef struct {
    xmedia_u64 phys_addr;
    xmedia_void *virt_addr;
    xmedia_u32 buf_len;
} xmedia_msensor_buf_attr;

typedef struct {
    xmedia_s32 x;
    xmedia_s32 y;
    xmedia_s32 z;
    xmedia_s32 temperature;
    xmedia_u64 pts;
} xmedia_msensor_sample_data;

typedef struct {
    xmedia_u32 data_num;
    xmedia_msensor_sample_data gyro_data[XMEDIA_MSENSOR_MAX_DATA_NUM];
} xmedia_msensor_gyro_buf;

typedef struct {
    xmedia_u32 data_num;
    xmedia_msensor_sample_data acc_data[XMEDIA_MSENSOR_MAX_DATA_NUM];
} xmedia_msensor_acc_buf;

typedef struct {
    xmedia_u32 odr;
    xmedia_u32 fsr;
    xmedia_u8  data_width;
    xmedia_s32 temperature_max;
    xmedia_s32 temperature_min;
} xmedia_msensor_gyro_config;

typedef struct {
    xmedia_u32 odr; // 速率
    xmedia_u32 fsr; // 量程
    xmedia_u8  data_width; // 数据位宽
    xmedia_s32 temperature_max;
    xmedia_s32 temperature_min;
} xmedia_msensor_acc_config;

typedef struct {
    xmedia_msensor_gyro_config gyro_config;
    xmedia_msensor_acc_config acc_config;
} xmedia_msensor_config;

typedef struct {
    xmedia_msensor_buf_attr buf_attr;
    xmedia_msensor_config config;
    xmedia_msensor_attr attr;
} xmedia_msensor_param;

typedef enum {
    XMEDIA_MSENSOR_DATA_TYPE_GYRO = 0,
    XMEDIA_MSENSOR_DATA_TYPE_ACC,
    XMEDIA_MSENSOR_DATA_TYPE_MAX
} xmedia_msensor_data_type;

typedef struct {
    xmedia_s32 *x_phys_addr;
    xmedia_s32 *y_phys_addr;
    xmedia_s32 *z_phys_addr;
    xmedia_s32 *temperature_phys_addr;
    xmedia_u64 *pts_phys_addr;
    xmedia_u32 num; /* number of valid data */
} xmedia_msensor_data_addr;

typedef struct {
    xmedia_s32 id;
    xmedia_msensor_data_type data_type;
    xmedia_msensor_data_addr data[2]; /* cyclic buffer 2 */
    xmedia_u64 begin_pts;
    xmedia_u64 end_pts;
    xmedia_s64 addr_offset;
} xmedia_msensor_data_info;

typedef struct {
    xmedia_msensor_attr attr;
    xmedia_msensor_gyro_buf gyro_buf;
    xmedia_msensor_acc_buf acc_buf;
} xmedia_msensor_data;

xmedia_s32 xmedia_msensor_init(xmedia_void);
xmedia_void xmedia_msensor_exit(xmedia_void);
xmedia_s32 xmedia_msensor_create_dev(xmedia_s32 dev, const xmedia_msensor_param *param);
xmedia_s32 xmedia_msensor_destroy_dev(xmedia_s32 dev);
xmedia_s32 xmedia_msensor_get_param(xmedia_s32 dev, xmedia_msensor_param *param);
xmedia_s32 xmedia_msensor_start_dev(xmedia_s32 dev);
xmedia_s32 xmedia_msensor_stop_dev(xmedia_s32 dev);
xmedia_s32 xmedia_msensor_add_user(xmedia_s32 dev, xmedia_s32 *user_id);
xmedia_s32 xmedia_msensor_delete_user(xmedia_s32 dev, xmedia_s32 user_id);
xmedia_s32 xmedia_msensor_get_data(xmedia_s32 dev, xmedia_msensor_data_info *data);
xmedia_s32 xmedia_msensor_send_data(xmedia_s32 dev, const xmedia_msensor_data_info *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

