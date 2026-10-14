/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef MOTIONSENSOR_EXT_H
#define MOTIONSENSOR_EXT_H

#include "common.h"
#include "xmedia_debug.h"
#include "xmedia_msensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    xmedia_s32  (*pfn_get_config_from_chip) (xmedia_s32 dev, xmedia_msensor_param *param);
    xmedia_s32  (*pfn_write_data_to_buf) (xmedia_s32 dev);
} msensor_mng_callback;


/* debug print format  */

#define MSENSOR_ERR_TRACE(fmt, ...)                                                                         \
    printk(KERN_ERR  "" fmt, \
                 ##__VA_ARGS__)

#define msensor_warn_trace(fmt, ...)                                                                         \
    printk(KERN_WARN  "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, \
                 ##__VA_ARGS__)

#define msensor_notice_trace(fmt, ...)                                                                         \
    printk(KERN_NOTICE  "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, \
                 ##__VA_ARGS__)

#define msensor_info_trace(fmt, ...)                                                                         \
    printk(KERN_INFO  "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, \
                 ##__VA_ARGS__)

#define msensor_debug_trace(fmt, ...)                                                                         \
    printk(KERN_DEBUG  "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, \
                 ##__VA_ARGS__)

xmedia_s32 msensor_mng_register_call_back(xmedia_s32 dev, const msensor_mng_callback *callback);
xmedia_void msensor_mng_unregister_call_back(xmedia_s32 dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

