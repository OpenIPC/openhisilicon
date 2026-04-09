/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * hi_type.h — basic HiSilicon scalar/bool/void typedefs.
 *
 * Part of libive_neo, the open-source replacement for vendor libive.so.
 * Provides the HI_* primitive type aliases that test code and IVE API
 * structs expect. Self-contained — no dependency on the openhisilicon
 * gk_* header tree.
 */
#ifndef __HI_TYPE_H__
#define __HI_TYPE_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t             HI_U8;
typedef uint16_t            HI_U16;
typedef uint32_t            HI_U32;
typedef uint64_t            HI_U64;

typedef int8_t              HI_S8;
typedef int16_t             HI_S16;
typedef int32_t             HI_S32;
typedef int64_t             HI_S64;

typedef unsigned long       HI_UL;
typedef long                HI_SL;

typedef char                HI_CHAR;
typedef unsigned char       HI_UCHAR;

typedef float               HI_FLOAT;
typedef double              HI_DOUBLE;

typedef void                HI_VOID;

typedef enum {
    HI_FALSE = 0,
    HI_TRUE  = 1,
} HI_BOOL;

#define HI_NULL             0L
#define HI_SUCCESS          0
#define HI_FAILURE          (-1)

#ifndef NULL
#define NULL                ((void *)0)
#endif

/* Fixed-point types used by IVE ctrl structs */
typedef unsigned char  HI_U0Q8;
typedef unsigned char  HI_U1Q7;
typedef unsigned char  HI_U5Q3;
typedef unsigned char  HI_U3Q5;

typedef unsigned short HI_U0Q16;
typedef unsigned short HI_U4Q12;
typedef unsigned short HI_U6Q10;
typedef unsigned short HI_U8Q8;
typedef unsigned short HI_U9Q7;
typedef unsigned short HI_U12Q4;
typedef unsigned short HI_U14Q2;
typedef unsigned short HI_U5Q11;
typedef unsigned short HI_U1Q15;
typedef unsigned short HI_U2Q14;
typedef unsigned short HI_UFP16;

typedef short          HI_S9Q7;
typedef short          HI_S14Q2;
typedef short          HI_S1Q15;

typedef unsigned int   HI_U22Q10;
typedef unsigned int   HI_U25Q7;
typedef unsigned int   HI_U21Q11;
typedef unsigned int   HI_U14Q18;
typedef unsigned int   HI_U8Q24;
typedef unsigned int   HI_U4Q28;

typedef int            HI_S25Q7;
typedef int            HI_S16Q16;
typedef int            HI_S14Q18;
typedef int            HI_S20Q12;
typedef int            HI_S24Q8;

typedef unsigned short HI_U8Q4F4;

typedef HI_S32         HI_HANDLE;

/* Lowercase type aliases matching vendor hi_type.h naming convention
 * (used by hi_ivp.h / hi_comm_video.h public API). */
typedef HI_U8    hi_u8;
typedef HI_U16   hi_u16;
typedef HI_U32   hi_u32;
typedef HI_U64   hi_u64;
typedef HI_S8    hi_s8;
typedef HI_S16   hi_s16;
typedef HI_S32   hi_s32;
typedef HI_S64   hi_s64;
typedef HI_UL    hi_ul;
typedef HI_CHAR  hi_char;
typedef HI_UCHAR hi_uchar;
typedef HI_FLOAT hi_float;
typedef HI_VOID  hi_void;
typedef HI_BOOL  hi_bool;

#define HI_TRUE_LC  1
#define HI_FALSE_LC 0

#ifdef __cplusplus
}
#endif

#endif /* __HI_TYPE_H__ */
