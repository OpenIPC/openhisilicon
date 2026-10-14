/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef COMM_SYS_H
#define COMM_SYS_H

#include "xmedia_type.h"
#include "xmedia_errcode.h"
#include "xmedia_debug.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

#define MODULE_TRACE_SYS(level, fmt, ...) do { \
    MODULE_TRACE(level, MOD_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMM_SYS_H */

