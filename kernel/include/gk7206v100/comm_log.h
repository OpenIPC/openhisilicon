/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef COMM_LOG_H
#define COMM_LOG_H

#include "xmedia_type.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  函数功能：用户态日志管理初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                       - 函数执行成功
 *      XMEDIA_FAILURE                       - 函数执行失败
 */
xmedia_s32 usr_log_init(xmedia_void);


/*
 *  函数功能：用户态日志管理去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      无
 */
xmedia_void usr_log_exit(xmedia_void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMM_LOG_H */

