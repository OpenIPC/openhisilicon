/*
 * The V4 sensor drivers (libraries/sensor/hi3516ev200) are written against
 * Goke's naming; include/hicompat.h maps them onto HiSilicon's. This maps the
 * same names onto the XMedia SPC020 SDK (gk7205v500 family), whose sensor
 * interface is the V4 one rebranded, so one driver source serves all three.
 *
 * Force-included (-include) by libraries/Makefile for XMedia builds: the
 * drivers get the Goke types from include/type.h, often only indirectly
 * through the SDK headers, and the SDK headers include their own type.h
 * relative to themselves, so shadowing it on the include path cannot work.
 */
#ifndef XM_GK_COMPAT_H
#define XM_GK_COMPAT_H

#include "type.h"

typedef XMEDIA_UCHAR   GK_UCHAR;
typedef XMEDIA_U8      GK_U8;
typedef XMEDIA_U16     GK_U16;
typedef XMEDIA_U32     GK_U32;
typedef XMEDIA_U64     GK_U64;
typedef XMEDIA_CHAR    GK_CHAR;
typedef XMEDIA_S8      GK_S8;
typedef XMEDIA_S16     GK_S16;
typedef XMEDIA_S32     GK_S32;
typedef XMEDIA_S64     GK_S64;
typedef XMEDIA_FLOAT   GK_FLOAT;
typedef XMEDIA_DOUBLE  GK_DOUBLE;
typedef XMEDIA_BOOL    GK_BOOL;
#define GK_TRUE        XMEDIA_TRUE
#define GK_FALSE       XMEDIA_FALSE
#define GK_NULL        XMEDIA_NULL
#define GK_SUCCESS     XMEDIA_SUCCESS
#define GK_FAILURE     XMEDIA_FAILURE
#define GK_VOID        void

typedef unsigned char      gk_uchar;
typedef unsigned char      gk_u8;
typedef unsigned short     gk_u16;
typedef unsigned int       gk_u32;
typedef unsigned long long gk_u64;
typedef unsigned long      gk_ulong;
typedef char               gk_char;
typedef signed char        gk_s8;
typedef short              gk_s16;
typedef int                gk_s32;
typedef long long          gk_s64;
typedef long               gk_slong;
typedef float              gk_float;
typedef double             gk_double;
typedef void               gk_void;
typedef XMEDIA_BOOL        gk_bool;

#endif
