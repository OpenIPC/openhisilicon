/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef XMEDIA_LOG_H
#define XMEDIA_LOG_H

#include "xmedia_type.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  函数功能：初始化日志模块
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED - 打开节点失败
 */
xmedia_s32 xmedia_log_init(xmedia_void);

/*
 *  函数功能：去初始化日志模块
 *  函数参数：
 *      无
 *  返回值：
 *      无
 */
xmedia_void xmedia_log_exit(xmedia_void);

/*
 *  函数功能：设置模块内核态日志等级
 *  函数参数：
 *      mod_id   -  输入参数，目标模块ID
 *      level    -  输入参数，内核态日志等级
 *      is_all_mod - 输入参数，是否同时设置所有模块的内核态日志等级
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      1. 若is_all_mod为XMEDIA_TRUE，表示修改所有模块的内核态日志等级
 *      2. 需要先调用xmedia_log_init
 */
xmedia_s32 xmedia_log_set_level_cfg(xmedia_mod_id mod_id, xmedia_u32 level, xmedia_bool is_all_mod);

/*
 *  函数功能：获取目标模块内核态日志等级
 *  函数参数：
 *      mod_id   -  输入参数，目标模块ID
 *      level    -  输出参数，内核态日志等级
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      需要先调用xmedia_log_init
 */
xmedia_s32 xmedia_log_get_level_cfg(xmedia_mod_id mod_id, xmedia_u32 *level);

/*
 *  函数功能：设置读取日志时是否一直等待日志信息
 *  函数参数：
 *      wait_flag  -  输入参数，等待控制位
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      需要先调用xmedia_log_init
 */
xmedia_s32 xmedia_log_set_wait_flag(xmedia_bool wait_flag);

/*
 *  函数功能：读取日志
 *  函数参数：
 *      buf  -  输出参数，保存读出日志的buffer
 *      size -  输入参数，读取日志的大小
 *  返回值：
 *      小于 0       - 函数执行失败
 *      大于或等于 0 - 实际读取日志的大小
 *  注意：
 *      需要先调用xmedia_log_init
 */
xmedia_s32 xmedia_log_read(xmedia_char *buf, xmedia_u32 size);

#ifdef __cplusplus
}
#endif

#endif

