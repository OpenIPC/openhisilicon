/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __TYPE_H__
#define __TYPE_H__

#ifdef __KERNEL__

#include <linux/types.h>
#else

#include <stdint.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


typedef unsigned char           XMEDIA_UCHAR;
typedef unsigned char           XMEDIA_U8;
typedef unsigned short          XMEDIA_U16;
typedef unsigned int            XMEDIA_U32;
typedef unsigned long           XMEDIA_UL;
typedef uintptr_t               XMEDIA_UINTPTR_T;

typedef char                    XMEDIA_CHAR;
typedef signed char             XMEDIA_S8;
typedef short                   XMEDIA_S16;
typedef int                     XMEDIA_S32;
typedef long                    XMEDIA_SL;

typedef float                   XMEDIA_FLOAT;
typedef double                  XMEDIA_DOUBLE;

#ifndef _M_IX86
    typedef unsigned long long  XMEDIA_U64;
    typedef long long           XMEDIA_S64;
#else
    typedef unsigned __int64    XMEDIA_U64;
    typedef __int64             XMEDIA_S64;
#endif

typedef unsigned long           XMEDIA_SIZE_T;
typedef unsigned long           XMEDIA_LENGTH_T;
typedef long int                XMEDIA_PHYS_ADDR_T;

typedef unsigned int            XMEDIA_HANDLE;

typedef enum {
    XMEDIA_FALSE = 0,
    XMEDIA_TRUE  = 1,
} XMEDIA_BOOL;

#ifndef NULL
    #define NULL                0L
#endif

#ifndef FALSE
    #define FALSE   0
#endif

#ifndef TRUE
    #define TRUE    1
#endif

#define XMEDIA_NULL                 0L
#define XMEDIA_SUCCESS              0
#define XMEDIA_FAILURE              (-1)

#define XMEDIA_VOID                 void

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

typedef unsigned long           xmedia_size_t;
typedef unsigned long           xmedia_length_t;

typedef xmedia_u32                  xmedia_handle;

typedef XMEDIA_BOOL                 xmedia_bool;
typedef XMEDIA_UINTPTR_T            xmedia_uintptr_t;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

