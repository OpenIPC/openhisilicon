/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __MKP_NPU_H__
#define __MKP_NPU_H__

#include "math_fun.h"
#include "xmedia_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_TRACE(level, fmt, ...) \
    do { \
        MODULE_TRACE(level, MOD_ID_NPU_SVP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __MKP_NPU_H__ */
