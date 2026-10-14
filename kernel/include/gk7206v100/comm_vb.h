/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef COMM_VB_H
#define COMM_VB_H

#include "xmedia_type.h"
#include "common.h"
#include "xmedia_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VB_MAX_POOLS                   512              // 每个进程中pool最大个数
#define VB_MAX_POOL_ID                 0xffff           // pool_id最大值

#define MODULE_TRACE_VB(level, fmt, ...) do { \
    MODULE_TRACE(level, MOD_ID_VB, "[Func]:%s [Line]:%d [Info]:" fmt, \
        __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define VB_SUPPLEMENT_ISPINFO_MASK     0x1
#define VB_SUPPLEMENT_VDEC_INFO_MASK   (1 << 1)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMM_VB_H */

