/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef XMEDIA_DEBUG_H
#define XMEDIA_DEBUG_H

#include "xmedia_type.h"
#include "common.h"

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif

/*
 *  函数功能：检查调试信息打印等级，若打印等级值大于模块的打印等级值，不进行打印；反之，打印
 *  函数参数：
 *      level   - 输入参数，调试信息打印等级
 *      mod_id  - 输入参数，模块id
 *  返回值：
 *      XMEDIA_FAILURE                       - 函数执行失败或不满足打印条件
 *      XMEDIA_SUCCESS                       - 允许打印调试信息
 */
xmedia_s32 usr_log_check_level(xmedia_s32 level, xmedia_mod_id mod_id);


#define _EX__FILE_LINE(fxx,lxx) "[File]:"fxx"\n[Line]:"#lxx"\n[Info]:"
#define EX__FILE_LINE(fxx,lxx) _EX__FILE_LINE(fxx,lxx)
#define __FILE_LINE__ EX__FILE_LINE(__FILE__, __LINE__)

#define MODULE_DBG_EMERG      0
#define MODULE_DBG_ALERT      1
#define MODULE_DBG_CRIT       2
#define MODULE_DBG_ERR        3
#define MODULE_DBG_WARN       4
#define MODULE_DBG_NOTICE     5
#define MODULE_DBG_INFO       6
#define MODULE_DBG_DEBUG      7

#define MODULE_USR_DEFAULT_DBG      MODULE_DBG_ERR       // 用户态日志管理初始化失败时各模块的打印等级

#ifndef __KERNEL__

#define XMEDIA_PRINT printf

#ifdef CONFIG_LOG_TRACE_SUPPORT
    #define XMEDIA_ASSERT(expr) do { \
        if (!(expr)) { \
            printf("\nASSERT at:\n" \
                   "  >Function : %s\n" \
                   "  >Line No. : %d\n" \
                   "  >Condition: %s\n", \
                   __FUNCTION__, __LINE__, #expr); \
            _exit(-1); \
        } \
    } while (0)

    #define MODULE_TRACE(level, mod_id, fmt...) do { \
        if (usr_log_check_level(level, mod_id) == XMEDIA_SUCCESS) { \
            fprintf(stderr, ##fmt); \
        } \
    } while (0) \

#else
    #define XMEDIA_ASSERT(expr) do { \
        if (!(expr)) { \
        } \
    } while (0)

    /* for clear compile warning */
    #define MODULE_TRACE(level, mod_id, fmt...) do { \
        if (0) { \
            fprintf(stderr,##fmt); \
        } \
    } while (0)

#endif

#else
#include "drv_log.h"
#include "osal.h"

#define XMEDIA_PRINT osal_printk

#ifdef CONFIG_LOG_TRACE_SUPPORT
#define XMEDIA_ASSERT(expr) do { \
    if (!(expr)) { \
        osal_panic("\nASSERT at:\n" \
              "  >Function : %s\n" \
              "  >Line No. : %d\n" \
              "  >Condition: %s\n", \
              __FUNCTION__, __LINE__, #expr); \
    } \
} while (0)

#define MODULE_TRACE(level, mod_id, fmt...) do { \
    drv_log_write(level,mod_id,fmt); \
} while (0)

#else
#define XMEDIA_ASSERT(expr) do { \
    if (!(expr)) { \
    } \
} while (0)

/* for clear compile warning */
#define MODULE_TRACE(level, mod_id, fmt...) do { \
     if (0) { \
        drv_log_write(level,mod_id,fmt); \
     }  \
} while (0)
#endif

#endif

#define XMEDIA_PRINT_BLOCK(data_block, length) \
{ \
    xmedia_u32 ii = 0; \
    xmedia_u8* vir_addr = (xmedia_u8*)(data_block); \
    if (vir_addr != XMEDIA_NULL) { \
        XMEDIA_PRINT("\n[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, #data_block); \
        for (ii = 0; ii < (length);) { \
            XMEDIA_PRINT(" %02X", *vir_addr); \
            vir_addr++; \
            ii++; \
            if(0 == (ii % 16))XMEDIA_PRINT("\n"); \
        } \
        XMEDIA_PRINT("\n\n");\
    } else { \
        XMEDIA_PRINT("\n[Func]:%s [Line]:%d [Info]:pointer(%s) is null!\n", __FUNCTION__, __LINE__, #data_block); \
    } \
}

#ifdef __cplusplus
}
#endif

#endif

