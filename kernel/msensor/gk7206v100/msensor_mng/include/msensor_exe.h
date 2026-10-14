/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __MSENSOR_EXE_H__
#define __MSENSOR_EXE_H__

#include "xmedia_debug.h"
#include "xmedia_type.h"
#include "xmedia_msensor.h"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

/* name MAG INTERFACE */
#define C_BMI160_BYTE_COUNT                 2
#define BMI160_SLEEP_STATE                  0x00
#define BMI160_WAKEUP_INTR                  0x00
#define BMI160_SLEEP_TRIGGER                0x04
#define BMI160_WAKEUP_TRIGGER               0x02
#define BMI160_ENABLE_FIFO_WM               0x02
#define BMI160_MAG_INTERFACE_OFF_PRIMARY_ON 0x00
#define BMI160_MAG_INTERFACE_ON_PRIMARY_ON  0x02

#define BMI160_MODE_SWITCHING_DELAY         30

#define msensor_return_if_null_ptr(ptr) \
    do {                                \
        if ((ptr) == XMEDIA_NULL) {     \
            printk("input null ptr\n"); \
            return XMEDIA_FAILURE;      \
        }                               \
    } while (0)


typedef struct {
    xmedia_u32 cmd;
    xmedia_s32 (*func)(xmedia_uintptr_t arg);
} msensor_mng_info;

/* for msensor_mng proc */
#define MNG_MAX_LEN 10
typedef struct {
    xmedia_char gyro_name[MNG_MAX_LEN];
    xmedia_char accel_name[MNG_MAX_LEN];
    xmedia_char mag_name[MNG_MAX_LEN];

    xmedia_u64 buf_addr[XMEDIA_MSENSOR_DATA_TYPE_MAX];
    xmedia_u32 buf_size[XMEDIA_MSENSOR_DATA_TYPE_MAX];
    xmedia_u32 buf_overflow[XMEDIA_MSENSOR_DATA_TYPE_MAX];
    xmedia_u32 buf_data_unmatch[XMEDIA_MSENSOR_DATA_TYPE_MAX];
    xmedia_s32 buf_overflow_id[XMEDIA_MSENSOR_DATA_TYPE_MAX];
    xmedia_s32 buf_data_unmatch_id[XMEDIA_MSENSOR_DATA_TYPE_MAX];
} msensor_mng_proc_info;

msensor_mng_proc_info *msensor_mng_get_proc_info(xmedia_s32 dev);

xmedia_s32 msensor_mng_proc_info_init(xmedia_s32 dev);

xmedia_s32 msensor_mng_buf_init(xmedia_s32 dev,
    const xmedia_msensor_attr *motion_attr,
    const xmedia_msensor_buf_attr *msensor_buf_attr,
    const xmedia_msensor_config *msensor_config);

xmedia_s32 msensor_mng_buf_exit(xmedia_s32 dev);
xmedia_s32 msensor_mng_write_data_to_buf(xmedia_s32 dev, xmedia_msensor_data *msensor_data);

xmedia_s32 msensor_mng_write_data_2_buf(xmedia_s32 dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

