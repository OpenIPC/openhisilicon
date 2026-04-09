/* Shim: redirect hi_type.h to openhisilicon type.h + hi_ aliases */
#ifndef __HI_TYPE_SHIM_H__
#define __HI_TYPE_SHIM_H__

#include "../../include/type.h"

/* HiSilicon SDK type aliases -> openhisilicon GK_ types */
typedef GK_U8 hi_u8;
typedef GK_U16 hi_u16;
typedef GK_U32 hi_u32;
typedef GK_U64 hi_u64;
typedef GK_S8 hi_s8;
typedef GK_S16 hi_s16;
typedef GK_S32 hi_s32;
typedef GK_S64 hi_s64;
typedef GK_CHAR hi_char;
typedef GK_UCHAR hi_uchar;
typedef GK_FLOAT hi_float;
typedef GK_DOUBLE hi_double;
typedef GK_UL hi_ulong;
typedef GK_SL hi_slong;
typedef GK_SIZE_T hi_size_t;
typedef GK_PHYS_ADDR_T hi_phys_addr_t;
typedef GK_VOID hi_void;
typedef GK_BOOL hi_bool;

/* Upper-case HI_ variants used by some SDK code */
typedef GK_U8 HI_U8;
typedef GK_U16 HI_U16;
typedef GK_U32 HI_U32;
typedef GK_U64 HI_U64;
typedef GK_S8 HI_S8;
typedef GK_S16 HI_S16;
typedef GK_S32 HI_S32;
typedef GK_S64 HI_S64;
typedef GK_CHAR HI_CHAR;
typedef GK_UCHAR HI_UCHAR;
typedef GK_FLOAT HI_FLOAT;
typedef GK_DOUBLE HI_DOUBLE;
typedef GK_UL HI_UL;
typedef GK_SL HI_SL;
typedef GK_SIZE_T HI_SIZE_T;
typedef GK_VOID HI_VOID;
typedef GK_BOOL HI_BOOL;

#define HI_NULL GK_NULL
#define HI_SUCCESS GK_SUCCESS
#define HI_FAILURE GK_FAILURE
#define HI_TRUE GK_TRUE
#define HI_FALSE GK_FALSE

#endif
