/*
 * Copyright (C), Hisilicon Technologies Co., Ltd. 2005-2019. All rights reserved.
 * Description   : head file for hisi types adapt to kernel.
 * Author        : Hisilicon multimedia software group
 * Create        : 2005-04-23
 */

#ifndef __HI_TYPES_H__
#define __HI_TYPES_H__

#include <linux/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* -------------------------------------------------------------------------------------------------------------*
 * Defintion of basic data types. The data types are applicable to both the application layer and kernel codes. *
 * -------------------------------------------------------------------------------------------------------------*
 */
#define HI_VOID                 void

typedef unsigned char           hi_uchar;
typedef unsigned char           hi_u8;
typedef unsigned short          hi_u16;
typedef unsigned int            hi_u32;
typedef unsigned long           hi_ulong;

typedef char                    hi_char;
typedef signed char             hi_s8;
typedef short                   hi_s16;
typedef int                     hi_s32;
typedef long                    hi_slong;

typedef float                   hi_float;
typedef double                  hi_double;

typedef void                    hi_void;

typedef unsigned long           hi_size_t;
typedef unsigned long           hi_length_t;

typedef hi_u32                  hi_handle;

typedef HI_BOOL                 hi_bool;
typedef HI_UINTPTR_T            hi_uintptr_t;

#ifndef _M_IX86
typedef unsigned long long      hi_u64;
typedef long long               hi_s64;
#else
typedef __int64                 hi_u64;
typedef __int64                 hi_s64;
#endif

/* Constant Definition */
typedef enum {
    HI_FALSE    = 0,
    HI_TRUE     = 1,
} HI_BOOL;

#ifndef NULL
#define NULL                0L
#endif

#define HI_NULL             0L
#define HI_NULL_PTR         0L

#define HI_SUCCESS          0
#define HI_FAILURE          (-1)
#define HI_INVALID_HANDLE   0xffffffff

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TYPES_H__ */

