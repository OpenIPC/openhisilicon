/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * hi_comm_ive.h — common IVE type/enum/struct definitions.
 *
 * Matches vendor HiSilicon libive.so ABI (verified against IDA decomp at
 * ~/libive/mpi_ive.o.c and against the kernel driver ive_neo.c which
 * dispatches ioctls from these structs).
 *
 * Critical struct sizes (verified):
 *   IVE_IMAGE_S     = 72 bytes  (au64PhyAddr[3]+au64VirAddr[3]+au32Stride[3]+w+h+type)
 *   IVE_MEM_INFO_S  = 24 bytes  (u64PhyAddr + u64VirAddr + u32Size + 4B pad)
 *   IVE_DATA_S      = 32 bytes
 */
#ifndef __HI_COMM_IVE_H__
#define __HI_COMM_IVE_H__

#include "hi_type.h"
#include "hi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- IVE handle + query-timeout errcode --- */

typedef HI_S32 IVE_HANDLE;

#define HI_ERR_IVE_SYS_TIMEOUT    HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, 0x40)
#define HI_ERR_IVE_QUERY_TIMEOUT  HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, 0x41)
#define HI_ERR_IVE_OPEN_FILE      HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, 0x42)
#define HI_ERR_IVE_READ_FILE      HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, 0x43)
#define HI_ERR_IVE_WRITE_FILE     HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, 0x44)
#define HI_ERR_IVE_BUS_ERR        HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, 0x45)

#define HI_ERR_IVE_INVALID_DEVID  HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define HI_ERR_IVE_INVALID_CHNID  HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define HI_ERR_IVE_ILLEGAL_PARAM  HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define HI_ERR_IVE_EXIST          HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define HI_ERR_IVE_UNEXIST        HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define HI_ERR_IVE_NULL_PTR       HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define HI_ERR_IVE_NOT_CONFIG     HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define HI_ERR_IVE_NOT_SUPPORT    HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define HI_ERR_IVE_NOT_PERM       HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define HI_ERR_IVE_NOMEM          HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define HI_ERR_IVE_NOBUF          HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define HI_ERR_IVE_BUF_EMPTY      HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define HI_ERR_IVE_BUF_FULL       HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define HI_ERR_IVE_NOTREADY       HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define HI_ERR_IVE_BADADDR        HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR)
#define HI_ERR_IVE_BUSY           HI_DEF_ERR(HI_ID_IVE, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

/* --- Image / memory descriptor structs --- */

typedef enum hiIVE_IMAGE_TYPE_E {
    IVE_IMAGE_TYPE_U8C1          = 0x0,
    IVE_IMAGE_TYPE_S8C1          = 0x1,
    IVE_IMAGE_TYPE_YUV420SP      = 0x2,
    IVE_IMAGE_TYPE_YUV422SP      = 0x3,
    IVE_IMAGE_TYPE_YUV420P       = 0x4,
    IVE_IMAGE_TYPE_YUV422P       = 0x5,
    IVE_IMAGE_TYPE_S8C2_PACKAGE  = 0x6,
    IVE_IMAGE_TYPE_S8C2_PLANAR   = 0x7,
    IVE_IMAGE_TYPE_S16C1         = 0x8,
    IVE_IMAGE_TYPE_U16C1         = 0x9,
    IVE_IMAGE_TYPE_U8C3_PACKAGE  = 0xa,
    IVE_IMAGE_TYPE_U8C3_PLANAR   = 0xb,
    IVE_IMAGE_TYPE_S32C1         = 0xc,
    IVE_IMAGE_TYPE_U32C1         = 0xd,
    IVE_IMAGE_TYPE_S64C1         = 0xe,
    IVE_IMAGE_TYPE_U64C1         = 0xf,
    IVE_IMAGE_TYPE_BUTT,
} IVE_IMAGE_TYPE_E;

typedef struct hiIVE_IMAGE_S {
    HI_U64           au64PhyAddr[3];
    HI_U64           au64VirAddr[3];
    HI_U32           au32Stride[3];
    HI_U32           u32Width;
    HI_U32           u32Height;
    IVE_IMAGE_TYPE_E enType;
} IVE_IMAGE_S;

typedef IVE_IMAGE_S IVE_SRC_IMAGE_S;
typedef IVE_IMAGE_S IVE_DST_IMAGE_S;

typedef struct hiIVE_MEM_INFO_S {
    HI_U64 u64PhyAddr;
    HI_U64 u64VirAddr;
    HI_U32 u32Size;
} IVE_MEM_INFO_S;

typedef IVE_MEM_INFO_S IVE_SRC_MEM_INFO_S;
typedef IVE_MEM_INFO_S IVE_DST_MEM_INFO_S;

typedef struct hiIVE_DATA_S {
    HI_U64 u64PhyAddr;
    HI_U64 u64VirAddr;
    HI_U32 u32Stride;
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32Reserved;
} IVE_DATA_S;

typedef IVE_DATA_S IVE_SRC_DATA_S;
typedef IVE_DATA_S IVE_DST_DATA_S;

typedef union hiIVE_8BIT_U {
    HI_S8 s8Val;
    HI_U8 u8Val;
} IVE_8BIT_U;

typedef struct hiIVE_POINT_U16_S {
    HI_U16 u16X;
    HI_U16 u16Y;
} IVE_POINT_U16_S;

typedef struct hiIVE_POINT_S16_S {
    HI_S16 s16X;
    HI_S16 s16Y;
} IVE_POINT_S16_S;

typedef struct hiIVE_POINT_S25Q7_S {
    HI_S25Q7 s25q7X;
    HI_S25Q7 s25q7Y;
} IVE_POINT_S25Q7_S;

typedef struct hiIVE_POINT_U14Q2_S {
    HI_U14Q2 u14q2X;
    HI_U14Q2 u14q2Y;
} IVE_POINT_U14Q2_S;

typedef struct hiIVE_RECT_U16_S {
    HI_U16 u16X;
    HI_U16 u16Y;
    HI_U16 u16Width;
    HI_U16 u16Height;
} IVE_RECT_U16_S;

typedef struct hiIVE_RECT_U32_S {
    HI_U32 u32X;
    HI_U32 u32Y;
    HI_U32 u32Width;
    HI_U32 u32Height;
} IVE_RECT_U32_S;

typedef struct hiIVE_LOOK_UP_TABLE_S {
    IVE_MEM_INFO_S stTable;
    HI_U16 u16ElemNum;
    HI_U8  u8TabInPreci;
    HI_U8  u8TabOutNorm;
    HI_S32 s32TabInLower;
    HI_S32 s32TabInUpper;
} IVE_LOOK_UP_TABLE_S;

/* --- Blob descriptor (for CCL output, CNN input, etc.) --- */

typedef enum hiIVE_BLOB_TYPE_E {
    IVE_BLOB_TYPE_S32       = 0x0,
    IVE_BLOB_TYPE_U8        = 0x1,
    IVE_BLOB_TYPE_YVU420SP  = 0x2,
    IVE_BLOB_TYPE_YVU422SP  = 0x3,
    IVE_BLOB_TYPE_VEC_S32   = 0x4,
    IVE_BLOB_TYPE_SEQ_S32   = 0x5,
    IVE_BLOB_TYPE_U16       = 0x6,
    IVE_BLOB_TYPE_S8        = 0x7,
    IVE_BLOB_TYPE_F32       = 0x8,
    IVE_BLOB_TYPE_BUTT,
} IVE_BLOB_TYPE_E;

typedef struct hiIVE_BLOB_S {
    IVE_BLOB_TYPE_E enType;
    HI_U32          u32Stride;
    HI_U64          u64VirAddr;
    HI_U64          u64PhyAddr;
    HI_U32          u32Num;
    union {
        struct {
            HI_U32 u32Width;
            HI_U32 u32Height;
            HI_U32 u32Chn;
        } stWhc;
        struct {
            HI_U32 u32Dim;
            HI_U64 u64VirAddrStep;
        } stSeq;
    } unShape;
} IVE_BLOB_S;

typedef IVE_BLOB_S IVE_SRC_BLOB_S;
typedef IVE_BLOB_S IVE_DST_BLOB_S;

#ifdef __cplusplus
}
#endif

#endif /* __HI_COMM_IVE_H__ */
