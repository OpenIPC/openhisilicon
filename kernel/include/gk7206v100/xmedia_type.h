/*
 * Copyright (c). All rights reserved.
 */

#ifndef __XMEDIA_TYPE_H__
#define __XMEDIA_TYPE_H__

typedef unsigned char           xmedia_uchar;
typedef unsigned char           xmedia_u8;
typedef unsigned short          xmedia_u16;
typedef unsigned int            xmedia_u32;
typedef unsigned long long      xmedia_u64;
typedef unsigned long           xmedia_ulong;

typedef char                    xmedia_char;
typedef signed char             xmedia_s8;
typedef short                   xmedia_s16;
typedef int                     xmedia_s32;
typedef long long               xmedia_s64;
typedef long                    xmedia_slong;

typedef float                   xmedia_float;
typedef double                  xmedia_double;

typedef void                    xmedia_void;

typedef xmedia_u32              xmedia_handle;

typedef xmedia_ulong            xmedia_uintptr_t;
typedef xmedia_slong            xmedia_intptr_t;

typedef enum {
    XMEDIA_FALSE    = 0,
    XMEDIA_TRUE     = 1,
} xmedia_bool;

#define XMEDIA_NULL                 0L

#define XMEDIA_SUCCESS              0
#define XMEDIA_FAILURE              (-1)

#define XMEDIA_INVALID_HANDLE       (0xffffffff)

#define XMEDIA_UNUSED(x)            ((x) = (x))

#endif

