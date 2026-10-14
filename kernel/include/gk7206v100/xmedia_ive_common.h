/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _COMM_IVE_H_
#define _COMM_IVE_H_

#include "common.h"
#include "xmedia_type.h"
#include "xmedia_errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define XMEDIA_IVE_HIST_NUM          256
#define XMEDIA_IVE_MAP_NUM           256
#define XMEDIA_IVE_MAX_REGION_NUM    254
#define XMEDIA_IVE_ST_MAX_CORNER_NUM 500
#define XMEDIA_IVE_INVALID_HANDLE       (-1)

typedef xmedia_s32 xmedia_ive_handle;

/* -----------------------------------------------*
 * The fixed-point data type, will be used to    *
 * represent float data in hardware calculations. linux code style.*
 * ----------------------------------------------- */
/* --u8bit---------------------------------------- */
typedef xmedia_u8 xmedia_u0q8;
typedef xmedia_u8 xmedia_u1q7;
typedef xmedia_u8 xmedia_u5q3;
typedef xmedia_u8 xmedia_u3q5;

/* --u16bit--------------------------------------- */
typedef xmedia_u16 xmedia_u0q16;
typedef xmedia_u16 xmedia_u4q12;
typedef xmedia_u16 xmedia_u6q10;
typedef xmedia_u16 xmedia_u8q8;
typedef xmedia_u16 xmedia_u9q7;
typedef xmedia_u16 xmedia_u12q4;
typedef xmedia_u16 xmedia_u14q2;
typedef xmedia_u16 xmedia_u5q11;
typedef xmedia_u16 xmedia_u1q15;
typedef xmedia_u16 xmedia_u2q14;
typedef xmedia_u16 xmedia_ufp16;
/* 8bits unsigned integer,4bits decimal fraction,4bits flag_bits */
typedef xmedia_u16 xmedia_u8q4f4;

/* --s16bit--------------------------------------- */
typedef xmedia_s16 xmedia_s9q7;
typedef xmedia_s16 xmedia_s14q2;
typedef xmedia_s16 xmedia_s1q15;

/* --u32bit--------------------------------------- */
typedef xmedia_u32 xmedia_u22q10;
typedef xmedia_u32 xmedia_u25q7;
typedef xmedia_u32 xmedia_u21q11;
typedef xmedia_u32 xmedia_u14q18;
typedef xmedia_u32 xmedia_u8q24;
typedef xmedia_u32 xmedia_u4q28;

/* --s32bit--------------------------------------- */
typedef xmedia_s32 xmedia_s25q7;
typedef xmedia_s32 xmedia_s16q16;
typedef xmedia_s32 xmedia_s14q18;
typedef xmedia_s32 xmedia_s20q12;
typedef xmedia_s32 xmedia_s24q8;


typedef enum xmedia_ive_image_type_e {
    XMEDIA_IVE_IMAGE_TYPE_U8C1 = 0x0,
    XMEDIA_IVE_IMAGE_TYPE_S8C1 = 0x1,

    XMEDIA_IVE_IMAGE_TYPE_YUV420SP = 0x2,
    XMEDIA_IVE_IMAGE_TYPE_YUV422SP = 0x3,
    XMEDIA_IVE_IMAGE_TYPE_YUV420P = 0x4,
    XMEDIA_IVE_IMAGE_TYPE_YUV422P = 0x5,

    XMEDIA_IVE_IMAGE_TYPE_S8C2_PACKAGE = 0x6,
    XMEDIA_IVE_IMAGE_TYPE_S8C2_PLANAR = 0x7,

    XMEDIA_IVE_IMAGE_TYPE_S16C1 = 0x8,
    XMEDIA_IVE_IMAGE_TYPE_U16C1 = 0x9,

    XMEDIA_IVE_IMAGE_TYPE_U8C3_PACKAGE = 0xa,
    XMEDIA_IVE_IMAGE_TYPE_U8C3_PLANAR = 0xb,

    XMEDIA_IVE_IMAGE_TYPE_S32C1 = 0xc,
    XMEDIA_IVE_IMAGE_TYPE_U32C1 = 0xd,

    XMEDIA_IVE_IMAGE_TYPE_S64C1 = 0xe,
    XMEDIA_IVE_IMAGE_TYPE_U64C1 = 0xf,

    XMEDIA_IVE_IMAGE_TYPE_BUTT

} xmedia_ive_image_type_e;

typedef struct xmedia_ive_image_s {
    xmedia_u64 au64phyaddr[3];
    xmedia_u64 au64viraddr[3];
    xmedia_u32 au32stride[3];
    xmedia_u32 u32width;
    xmedia_u32 u32height;
    xmedia_ive_image_type_e en_type;
} xmedia_ive_image_s;

typedef xmedia_ive_image_s xmedia_ive_src_image_s;
typedef xmedia_ive_image_s xmedia_ive_dst_image_s;


typedef struct xmedia_ive_mem_info_s {
    xmedia_u64 u64phyaddr;
    xmedia_u64 u64viraddr;
    xmedia_u32 u32size;
} xmedia_ive_mem_info_s;
typedef xmedia_ive_mem_info_s xmedia_ive_src_mem_info_s;
typedef xmedia_ive_mem_info_s xmedia_ive_dst_mem_info_s;

typedef struct xmedia_ive_data_s {
    xmedia_u64 u64phyaddr;
    xmedia_u64 u64viraddr;

    xmedia_u32 u32stride;
    xmedia_u32 u32width;
    xmedia_u32 u32height;

    xmedia_u32 u32reserved;
} xmedia_ive_data_s;
typedef xmedia_ive_data_s xmedia_ive_src_data_s;
typedef xmedia_ive_data_s xmedia_ive_dst_data_s;

typedef union xmedia_ive_8bit_u {
    xmedia_s8 s8val;
    xmedia_u8 u8val;
} xmedia_ive_8bit_u;

typedef struct xmedia_ive_point_u16_s {
    xmedia_u16 u16x;
    xmedia_u16 u16y;
} xmedia_ive_point_u16_s;

typedef struct xmedia_ive_point_s16_s {
    xmedia_u16 s16x;
    xmedia_u16 s16y;
} xmedia_ive_point_s16_s;

typedef struct xmedia_ive_point_s25q7_s {
    xmedia_s25q7 s25q7x;
    xmedia_s25q7 s25q7y;
} xmedia_ive_point_s25q7_s;

typedef struct xmedia_ive_point_u14q2_s {
    xmedia_u14q2 u14q2x;
    xmedia_u14q2 u14q2y;
} xmedia_ive_point_u14q2_s;

typedef struct xmedia_ive_rect_u16_s {
    xmedia_u16 u16x;
    xmedia_u16 u16y;
    xmedia_u16 u16width;
    xmedia_u16 u16height;
} xmedia_ive_rect_u16_s;

typedef struct xmedia_ive_rect_u32_s {
    xmedia_u32 u32x;
    xmedia_u32 u32y;
    xmedia_u32 u32width;
    xmedia_u32 u32height;
} xmedia_ive_rect_u32_s;

typedef struct xmedia_ive_rect_s24q8_s {
    xmedia_s24q8 s24q8X;
    xmedia_s24q8 s24q8Y;
    xmedia_u32 u32width;
    xmedia_u32 u32height;
} xmedia_ive_rect_s24q8_s;

typedef struct xmedia_ive_look_up_table_s {
    xmedia_ive_mem_info_s sttable;
    xmedia_u16 u16elemnum;

    xmedia_u8 u8tabinpreci;
    xmedia_u8 u8taboutnorm;

    xmedia_s32 s32tabinlower;
    xmedia_s32 s32tabinupper;
} xmedia_ive_look_up_table_s;

typedef enum xmedia_ive_blob_type_e {
    XMEDIA_IVE_BLOB_TYPE_S32 = 0x0,

    XMEDIA_IVE_BLOB_TYPE_U8 = 0x1,

    XMEDIA_IVE_BLOB_TYPE_YVU420SP = 0x2,
    XMEDIA_IVE_BLOB_TYPE_YVU422SP = 0x3,

    XMEDIA_IVE_BLOB_TYPE_VEC_S32 = 0x4,

    XMEDIA_IVE_BLOB_TYPE_SEQ_S32 = 0x5,

    XMEDIA_IVE_BLOB_TYPE_U16 = 0x6,

    XMEDIA_IVE_BLOB_TYPE_S8 = 0x7,

    XMEDIA_IVE_BLOB_TYPE_F32 = 0x8,

    XMEDIA_IVE_BLOB_TYPE_BUTT
} xmedia_ive_blob_type_e;


typedef struct xmedia_ive_blob_s {
    xmedia_ive_blob_type_e en_type;
    xmedia_u32 u32stride;

    xmedia_u64 u64viraddr;
    xmedia_u64 u64phyaddr;

    xmedia_u32 u32_num;
    union {
        struct {
            xmedia_u32 u32width;
            xmedia_u32 u32height;
            xmedia_u32 u32chn;
        } stWhc;
        struct {
            xmedia_u32 u32dim;
            xmedia_u64 u64viraddrstep;
        } stseq;
    } unshape;

} xmedia_ive_blob_s;

typedef xmedia_ive_blob_s xmedia_ive_src_blob_s;
typedef xmedia_ive_blob_s xmedia_ive_dst_blob_s;

typedef enum xmedia_en_ive_err_code_e {
    XMEDIA_IVE_ERR_SYS_TIMEOUT = 0x40,
    XMEDIA_IVE_ERR_QUERY_TIMEOUT = 0x41,
    XMEDIA_IVE_ERR_OPEN_FILE = 0x42,
    XMEDIA_IVE_ERR_READ_FILE = 0x43,
    XMEDIA_IVE_ERR_WRITE_FILE = 0x44,
    XMEDIA_IVE_ERR_BUS_ERR = 0x45,

    XMEDIA_IVE_ERR_BUTT
} xmedia_en_ive_err_code_e;

typedef enum xmedia_en_fd_err_code_e {
    XMEDIA_IVE_ERR_FD_SYS_TIMEOUT = 0x40,
    XMEDIA_IVE_ERR_FD_CFG = 0x41,
    XMEDIA_IVE_ERR_FD_FACE_NUM_OVER = 0x42,
    XMEDIA_IVE_ERR_FD_OPEN_FILE = 0x43,
    XMEDIA_IVE_ERR_FD_READ_FILE = 0x44,
    XMEDIA_IVE_ERR_FD_WRITE_FILE = 0x45,

    XMEDIA_IVE_ERR_FD_BUTT
} xmedia_en_fd_err_code_e;

/************************************************IVE error code ***********************************/
/* Invalid device ID */
#define ERR_CODE_IVE_INVALID_DEVID XMEDIA_ERRCODE_INVALID_DEV_ID
/* Invalid channel ID */
#define ERR_CODE_IVE_INVALID_CHNID XMEDIA_ERRCODE_INVALID_CHN_ID
/* At least one parameter is illegal. For example, an illegal enumeration value exists. */
#define ERR_CODE_IVE_ILLEGAL_PARAM XMEDIA_ERRCODE_INVALID_PARAM
/* The channel exists. */
#define ERR_CODE_IVE_EXIST XMEDIA_ERRCODE_EXIST
/* The UN exists. */
#define ERR_CODE_IVE_UNEXIST XMEDIA_ERRCODE_NOT_EXIST
/* A null point is used. */
#define ERR_CODE_IVE_NULL_PTR XMEDIA_ERRCODE_NULL_PTR
/* Try to enable or initialize the system, device, or channel before configuring attributes. */
#define ERR_CODE_IVE_NOT_CONFIG XMEDIA_ERRCODE_NOT_CONFIG
/* The operation is not supported currently. */
#define ERR_CODE_IVE_NOT_SURPPORT XMEDIA_ERRCODE_NOT_SUPPORT
/* The operation, changing static attributes for example, is not permitted. */
#define ERR_CODE_IVE_NOT_PERM XMEDIA_ERRCODE_NOT_PERMITTED
/* A failure caused by the malloc memory occurs. */
#define ERR_CODE_IVE_NOMEM XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY
/* A failure caused by the malloc buffer occurs. */
#define ERR_CODE_IVE_NOBUF XMEDIA_ERRCODE_NO_BUFFER_FREE
/* The buffer is empty. */
#define ERR_CODE_IVE_BUF_EMPTY XMEDIA_ERRCODE_BUFFER_EMPTY
/* No buffer is provided for storing new data. */
#define ERR_CODE_IVE_BUF_FULL XMEDIA_ERRCODE_BUFFER_FULL
/* The system is not ready because it may be not initialized or loaded.
 * The error code is returned when a device file fails to be opened. */
#define ERR_CODE_IVE_NOTREADY XMEDIA_ERRCODE_NOT_READY
/* The source address or target address is incorrect during the operations such as calling
copy_from_user or copy_to_user. */
#define ERR_CODE_IVE_BADADDR XMEDIA_ERRCODE_BAD_ADDRESS
/* The resource is busy during the operations such as destroying a VENC channel
without deregistering it. */
#define ERR_CODE_IVE_BUSY XMEDIA_ERRCODE_BUSY
/* IVE process timeout: 0xA01D8040 */
#define ERR_CODE_IVE_SYS_TIMEOUT XMEDIA_ERRCODE_TIMEOUT
/* IVE query timeout: 0xA01D8041 */
#define ERR_CODE_IVE_QUERY_TIMEOUT XMEDIA_ERRCODE_TIMEOUT
/* IVE open file error: 0xA01D8042 */
#define ERR_CODE_IVE_OPEN_FILE XMEDIA_ERRCODE_OPEN_FAILED
/* IVE read file error: 0xA01D8043 */
#define ERR_CODE_IVE_READ_FILE XMEDIA_ERRCODE_COPY_DATA_ERROR
/* IVE read file error: 0xA01D8044 */
#define ERR_CODE_IVE_WRITE_FILE XMEDIA_ERRCODE_COPY_DATA_ERROR
/* IVE Bus error: 0xA01D8045 */
#define ERR_CODE_IVE_BUS_ERR XMEDIA_ERRCODE_NOT_READY

/************************************************FD error code ***********************************/
/* Invalid device ID */
#define ERR_CODE_FD_INVALID_DEVID XMEDIA_ERRCODE_INVALID_DEV_ID
/* Invalid channel ID */
#define ERR_CODE_FD_INVALID_CHNID XMEDIA_ERRCODE_INVALID_CHN_ID
/* At least one parameter is illegal. For example, an illegal enumeration value exists. */
#define ERR_CODE_FD_ILLEGAL_PARAM XMEDIA_ERRCODE_INVALID_PARAM
/* The channel exists. */
#define ERR_CODE_FD_EXIST XMEDIA_ERRCODE_EXIST
/* The UN exists. */
#define ERR_CODE_FD_UNEXIST XMEDIA_ERRCODE_NOT_EXIST
/* A null point is used. */
#define ERR_CODE_FD_NULL_PTR XMEDIA_ERRCODE_NULL_PTR
/* Try to enable or initialize the system, device, or channel before configuring attributes. */
#define ERR_CODE_FD_NOT_CONFIG XMEDIA_ERRCODE_NOT_CONFIG
/* The operation is not supported currently. */
#define ERR_CODE_FD_NOT_SURPPORT XMEDIA_ERRCODE_NOT_SUPPORT
/* The operation, changing static attributes for example, is not permitted. */
#define ERR_CODE_FD_NOT_PERM XMEDIA_ERRCODE_NOT_PERMITTED
/* A failure caused by the malloc memory occurs. */
#define ERR_CODE_FD_NOMEM XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY
/* A failure caused by the malloc buffer occurs. */
#define ERR_CODE_FD_NOBUF XMEDIA_ERRCODE_NO_BUFFER_FREE
/* The buffer is empty. */
#define ERR_CODE_FD_BUF_EMPTY XMEDIA_ERRCODE_BUFFER_EMPTY
/* No buffer is provided for storing new data. */
#define ERR_CODE_FD_BUF_FULL XMEDIA_ERRCODE_BUFFER_FULL
/* The system is not ready because it may be not initialized or loaded.
 * The error code is returned when a device file fails to be opened. */
#define ERR_CODE_FD_NOTREADY XMEDIA_ERRCODE_NOT_READY
/* The source address or target address is incorrect during the operations such as calling
copy_from_user or copy_to_user. */
#define ERR_CODE_FD_BADADDR XMEDIA_ERRCODE_BAD_ADDRESS
/* The resource is busy during the operations such as destroying a VENC channel
without deregistering it. */
#define ERR_CODE_FD_BUSY XMEDIA_ERRCODE_BUSY
/* FD process timeout*/
#define ERR_CODE_FD_SYS_TIMEOUT XMEDIA_ERRCODE_TIMEOUT
/* FD configuration error*/
#define ERR_CODE_FD_CFG XMEDIA_ERRCODE_INVALID_PARAM
/* FD candidate face number over*/
#define ERR_CODE_FD_FACE_NUM_OVER XMEDIA_ERRCODE_INVALID_PARAM
/* FD open file error*/
#define ERR_CODE_FD_OPEN_FILE XMEDIA_ERRCODE_OPEN_FAILED
/* FD read file error*/
#define ERR_CODE_FD_READ_FILE XMEDIA_ERRCODE_COPY_DATA_ERROR
/* FD read file error*/
#define ERR_CODE_FD_WRITE_FILE XMEDIA_ERRCODE_COPY_DATA_ERROR

/************************************************ODT error code ***********************************/
/* ODT Invalid channel ID*/
#define ERR_CODE_ODT_INVALID_CHNID XMEDIA_ERRCODE_INVALID_CHN_ID
/* ODT exist*/
#define ERR_CODE_ODT_EXIST XMEDIA_ERRCODE_EXIST
/* ODT unexist*/
#define ERR_CODE_ODT_UNEXIST XMEDIA_ERRCODE_NOT_EXIST
/* ODT The operation, changing static attributes for example, is not permitted*/
#define ERR_CODE_ODT_NOT_PERM XMEDIA_ERRCODE_NOT_PERMITTED
/* ODT the system is not ready because it may be not initialized*/
#define ERR_CODE_ODT_NOTREADY XMEDIA_ERRCODE_NOT_READY
/* ODT busy*/
#define ERR_CODE_ODT_BUSY XMEDIA_ERRCODE_BUSY


typedef enum xmedia_ive_resize_csc_letterbox_op_mode_e {
    XMEDIA_IVE_RCL_OP_MODE_CSC_ONLY = 0,
    XMEDIA_IVE_RCL_OP_MODE_RESIZE_LETTERBOX_ONLY = 1,
    XMEDIA_IVE_RCL_OP_MODE_RESIZE_CSC_LETTERBOX = 2,
} xmedia_ive_resize_csc_letterbox_op_mode_e;

typedef enum xmedia_ive_resize_csc_letterbox_csc_mode_e {
    XMEDIA_IVE_RCL_CSC_MODE_VIDEO_BT601_YUV2RGB =
        0x0, /* CSC: YUV2RGB, video transfer mode, RGB value range [16, 235] */
    XMEDIA_IVE_RCL_CSC_MODE_VIDEO_BT709_YUV2RGB =
        0x1, /* CSC: YUV2RGB, video transfer mode, RGB value range [16, 235] */
    XMEDIA_IVE_RCL_CSC_MODE_PIC_BT601_YUV2RGB =
        0x2, /* CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255] */
    XMEDIA_IVE_RCL_CSC_MODE_PIC_BT709_YUV2RGB =
        0x3, /* CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255] */
} xmedia_ive_resize_csc_letterbox_csc_mode_e;

typedef struct xmedia_ive_resize_csc_letterbox_ctrl_s {
    xmedia_ive_resize_csc_letterbox_op_mode_e  op_mode;
    xmedia_ive_resize_csc_letterbox_csc_mode_e csc_mode;
    xmedia_u16 u16_top_padding_len;
    xmedia_u16 u16_bottom_padding_len;
    xmedia_u16 u16_left_padding_len;
    xmedia_u16 u16_right_padding_len;
    xmedia_u8  u8_letterbox_value;
} xmedia_ive_resize_csc_letterbox_ctrl_s;

typedef enum xmedia_ive_dma_mode_e {
    XMEDIA_IVE_DMA_MODE_DIRECT_COPY = 0x0,
    XMEDIA_IVE_DMA_MODE_INTERVAL_COPY = 0x1,
    XMEDIA_IVE_DMA_MODE_SET_3BYTE = 0x2,
    XMEDIA_IVE_DMA_MODE_SET_8BYTE = 0x3,
    XMEDIA_IVE_DMA_MODE_BUTT
} xmedia_ive_dma_mode_e;


typedef struct xmedia_ive_dma_ctrl_s {
    xmedia_ive_dma_mode_e en_mode;
    xmedia_u64     u64_val;
    xmedia_u8      u8_hor_seg_size;
    xmedia_u8      u8_elem_size;
    xmedia_u16     u16_ver_seg_rows;
} xmedia_ive_dma_ctrl_s;


typedef struct xmedia_ive_filter_ctrl_s {
    xmedia_s8 as8_mask[25];
    xmedia_u8 u8_norm;
} xmedia_ive_filter_ctrl_s;


typedef enum xmedia_ive_csc_mode_e {
    XMEDIA_IVE_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x0,
    XMEDIA_IVE_CSC_MODE_VIDEO_BT709_YUV2RGB = 0x1,
    XMEDIA_IVE_CSC_MODE_PIC_BT601_YUV2RGB = 0x2,
    XMEDIA_IVE_CSC_MODE_PIC_BT709_YUV2RGB = 0x3,

    XMEDIA_IVE_CSC_MODE_PIC_BT601_YUV2HSV = 0x4,
    XMEDIA_IVE_CSC_MODE_PIC_BT709_YUV2HSV = 0x5,

    XMEDIA_IVE_CSC_MODE_PIC_BT601_YUV2LAB = 0x6,
    XMEDIA_IVE_CSC_MODE_PIC_BT709_YUV2LAB = 0x7,

    XMEDIA_IVE_CSC_MODE_VIDEO_BT601_RGB2YUV = 0x8,
    XMEDIA_IVE_CSC_MODE_VIDEO_BT709_RGB2YUV = 0x9,
    XMEDIA_IVE_CSC_MODE_PIC_BT601_RGB2YUV = 0xa,
    XMEDIA_IVE_CSC_MODE_PIC_BT709_RGB2YUV = 0xb,

    XMEDIA_IVE_CSC_MODE_BUTT
} xmedia_ive_csc_mode_e;


typedef struct xmedia_ive_csc_ctrl_s {
    xmedia_ive_csc_mode_e en_mode;
} xmedia_ive_csc_ctrl_s;


typedef struct xmedia_ive_filter_and_csc_ctrl_s {
    xmedia_ive_csc_mode_e en_mode;
    xmedia_s8      as8_mask[25];
    xmedia_u8      u8_norm;
} xmedia_ive_filter_and_csc_ctrl_s;

typedef enum xmedia_ive_sobel_out_ctrl_e {
    XMEDIA_IVE_SOBEL_OUT_CTRL_BOTH = 0x0,
    XMEDIA_IVE_SOBEL_OUT_CTRL_HOR = 0x1,
    XMEDIA_IVE_SOBEL_OUT_CTRL_VER = 0x2,
    XMEDIA_IVE_SOBEL_OUT_CTRL_BUTT
} xmedia_ive_sobel_out_ctrl_e;


typedef struct xmedia_ive_sobel_ctrl_s {
    xmedia_ive_sobel_out_ctrl_e en_out_ctrl;
    xmedia_s8 as8_mask[25];
} xmedia_ive_sobel_ctrl_s;


typedef enum xmedia_ive_mag_and_ang_out_ctrl_e {
    XMEDIA_IVE_MAG_AND_ANG_OUT_CTRL_MAG = 0x0,
    XMEDIA_IVE_MAG_AND_ANG_OUT_CTRL_MAG_AND_ANG = 0x1,
    XMEDIA_IVE_MAG_AND_ANG_OUT_CTRL_BUTT
} xmedia_ive_mag_and_ang_out_ctrl_e;

typedef struct xmedia_ive_mag_and_ang_ctrl_s {
    xmedia_ive_mag_and_ang_out_ctrl_e en_out_ctrl;
    xmedia_u16 u16_thr;
    xmedia_s8  as8_mask[25];
} xmedia_ive_mag_and_ang_ctrl_s;

typedef struct xmedia_ive_dilate_ctrl_s {
    xmedia_u8 au8_mask[25];
} xmedia_ive_dilate_ctrl_s;

typedef struct xmedia_ive_erode_ctrl_s {
    xmedia_u8 au8_mask[25];
} xmedia_ive_erode_ctrl_s;


typedef enum xmedia_ive_thresh_mode_e {
    XMEDIA_IVE_THRESH_MODE_BINARY = 0x0,
    XMEDIA_IVE_THRESH_MODE_TRUNC = 0x1,
    XMEDIA_IVE_THRESH_MODE_TO_MINVAL = 0x2,

    XMEDIA_IVE_THRESH_MODE_MIN_MID_MAX = 0x3,
    XMEDIA_IVE_THRESH_MODE_ORI_MID_MAX = 0x4,
    XMEDIA_IVE_THRESH_MODE_MIN_MID_ORI = 0x5,
    XMEDIA_IVE_THRESH_MODE_MIN_ORI_MAX = 0x6,
    XMEDIA_IVE_THRESH_MODE_ORI_MID_ORI = 0x7,

    XMEDIA_IVE_THRESH_MODE_BUTT
} xmedia_ive_thresh_mode_e;


typedef struct xmedia_ive_thresh_ctrl_s {
    xmedia_ive_thresh_mode_e en_mode;
    xmedia_u8 u8_low_thr;
    xmedia_u8 u8_high_thr;
    xmedia_u8 u8_min_val;
    xmedia_u8 u8_mid_val;
    xmedia_u8 u8_max_val;
} xmedia_ive_thresh_ctrl_s;

typedef enum xmedia_ive_sub_mode_e {
    XMEDIA_IVE_SUB_MODE_ABS = 0x0,
    XMEDIA_IVE_SUB_MODE_SHIFT = 0x1,
    XMEDIA_IVE_SUB_MODE_BUTT
} xmedia_ive_sub_mode_e;

typedef struct xmedia_ive_sub_ctrl_s {
    xmedia_ive_sub_mode_e en_mode;
} xmedia_ive_sub_ctrl_s;


typedef enum xmedia_ive_integ_out_ctrl_e {
    XMEDIA_IVE_INTEG_OUT_CTRL_COMBINE = 0x0,
    XMEDIA_IVE_INTEG_OUT_CTRL_SUM = 0x1,
    XMEDIA_IVE_INTEG_OUT_CTRL_SQSUM = 0x2,
    XMEDIA_IVE_INTEG_OUT_CTRL_BUTT
} xmedia_ive_integ_out_ctrl_e;


typedef struct xmedia_ive_integ_ctrl_s {
    xmedia_ive_integ_out_ctrl_e en_out_ctrl;
} xmedia_ive_integ_ctrl_s;

typedef enum xmedia_ive_thresh_s16_mode_e {
    XMEDIA_IVE_THRESH_S16_MODE_S16_TO_S8_MIN_MID_MAX = 0x0,
    XMEDIA_IVE_THRESH_S16_MODE_S16_TO_S8_MIN_ORI_MAX = 0x1,
    XMEDIA_IVE_THRESH_S16_MODE_S16_TO_U8_MIN_MID_MAX = 0x2,
    XMEDIA_IVE_THRESH_S16_MODE_S16_TO_U8_MIN_ORI_MAX = 0x3,

    XMEDIA_IVE_THRESH_S16_MODE_BUTT
} xmedia_ive_thresh_s16_mode_e;

typedef struct xmedia_ive_thresh_s16_ctrl_s {
    xmedia_ive_thresh_s16_mode_e en_mode;
    xmedia_s16 s16_low_thr;
    xmedia_s16 s16_high_thr;
    xmedia_ive_8bit_u un8_min_val;
    xmedia_ive_8bit_u un8_mid_val;
    xmedia_ive_8bit_u un8_max_val;
} xmedia_ive_thresh_s16_ctrl_s;

typedef enum xmedia_ive_thresh_u16_mode_e {
    XMEDIA_IVE_THRESH_U16_MODE_U16_TO_U8_MIN_MID_MAX = 0x0,
    XMEDIA_IVE_THRESH_U16_MODE_U16_TO_U8_MIN_ORI_MAX = 0x1,

    XMEDIA_IVE_THRESH_U16_MODE_BUTT
} xmedia_ive_thresh_u16_mode_e;

typedef struct xmedia_ive_thresh_u16_ctrl_s {
    xmedia_ive_thresh_u16_mode_e en_mode;
    xmedia_u16 u16_low_thr;
    xmedia_u16 u16_high_thr;
    xmedia_u8  u8_min_val;
    xmedia_u8  u8_mid_val;
    xmedia_u8  u8_max_val;
} xmedia_ive_thresh_u16_ctrl_s;

typedef enum xmedia_ive_16bit_to_8bit_mode_e {
    XMEDIA_IVE_16BIT_TO_8BIT_MODE_S16_TO_S8 = 0x0,
    XMEDIA_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS = 0x1,
    XMEDIA_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS = 0x2,
    XMEDIA_IVE_16BIT_TO_8BIT_MODE_U16_TO_U8 = 0x3,

    XMEDIA_IVE_16BIT_TO_8BIT_MODE_BUTT
} xmedia_ive_16bit_to_8bit_mode_e;

typedef struct xmedia_ive_16bit_to_8bit_ctrl_s {
    xmedia_ive_16bit_to_8bit_mode_e en_mode;
    xmedia_u16 u16_denominator;
    xmedia_u8  u8_numerator;
    xmedia_s8  s8_bias;
} xmedia_ive_16bit_to_8bit_ctrl_s;

typedef enum xmedia_ive_ord_stat_filter_mode_e {
    XMEDIA_IVE_ORD_STAT_FILTER_MODE_MEDIAN = 0x0,
    XMEDIA_IVE_ORD_STAT_FILTER_MODE_MAX = 0x1,
    XMEDIA_IVE_ORD_STAT_FILTER_MODE_MIN = 0x2,

    XMEDIA_IVE_ORD_STAT_FILTER_MODE_BUTT
} xmedia_ive_ord_stat_filter_mode_e;

typedef struct xmedia_ive_ord_stat_filter_ctrl_s {
    xmedia_ive_ord_stat_filter_mode_e en_mode;

} xmedia_ive_ord_stat_filter_ctrl_s;

typedef enum xmedia_ive_map_mode_e {
    XMEDIA_IVE_MAP_MODE_U8 = 0x0,
    XMEDIA_IVE_MAP_MODE_S16 = 0x1,
    XMEDIA_IVE_MAP_MODE_U16 = 0x2,

    XMEDIA_IVE_MAP_MODE_BUTT
} xmedia_ive_map_mode_e;
typedef struct xmedia_ive_map_ctrl_s {
    xmedia_ive_map_mode_e en_mode;
} xmedia_ive_map_ctrl_s;
typedef struct xmedia_ive_map_u8bit_lut_mem_s {
    xmedia_u8  au8_map[XMEDIA_IVE_MAP_NUM];
} xmedia_ive_map_u8bit_lut_mem_s;
typedef struct xmedia_ive_map_u16bit_lut_mem_s {
    xmedia_u16 au16_map[XMEDIA_IVE_MAP_NUM];
} xmedia_ive_map_u16bit_lut_mem_s;
typedef struct xmedia_ive_map_s16bit_lut_mem_s {
    xmedia_s16 as16_map[XMEDIA_IVE_MAP_NUM];
} xmedia_ive_map_s16bit_lut_mem_s;
typedef struct xmedia_ive_equalize_hist_ctrl_mem_s {
    xmedia_u32 au32_hist[XMEDIA_IVE_HIST_NUM];
    xmedia_u8  au8_map[XMEDIA_IVE_MAP_NUM];
} xmedia_ive_equalize_hist_ctrl_mem_s;


typedef struct xmedia_ive_equalize_hist_ctrl_s {
    xmedia_ive_mem_info_s stmem;
} xmedia_ive_equalize_hist_ctrl_s;


typedef struct xmedia_ive_add_ctrl_s {
    xmedia_u0q16 u0q16x;
    xmedia_u0q16 u0q16y;
} xmedia_ive_add_ctrl_s;

typedef enum xmedia_ive_acc_mode_e {
    XMEDIA_IVE_ACC_MODE_NO_USE_MASK = 0x0,
    XMEDIA_IVE_ACC_MODE_USE_MASK = 0x1,

    XMEDIA_IVE_ACC_MODE_BUTT
} xmedia_ive_acc_mode_e;
typedef struct xmedia_ive_acc_ctrl_s {
    xmedia_ive_acc_mode_e mode;
    xmedia_u16 mask[4];
} xmedia_ive_acc_ctrl_s;

typedef struct xmedia_ive_ncc_dst_mem_s {
    xmedia_u64 u64_numerator;
    xmedia_u64 u64_quad_sum1;
    xmedia_u64 u64_quad_sum2;
    xmedia_u8  u8_reserved[8];
} xmedia_ive_ncc_dst_mem_s;

typedef struct xmedia_ive_region_s {
    xmedia_u32 u32_area;
    xmedia_u16 u16_left;
    xmedia_u16 u16_right;
    xmedia_u16 u16_top;
    xmedia_u16 u16_bottom;
} xmedia_ive_region_s;

typedef struct xmedia_ive_ccblob_s {
    xmedia_u16   u16_cur_area_thr;
    xmedia_s8    s8_label_status;
    xmedia_u8    u8_region_num;
    xmedia_ive_region_s ast_region[XMEDIA_IVE_MAX_REGION_NUM];
} xmedia_ive_ccblob_s;

typedef enum xmedia_ive_ccl_mode_e {
    XMEDIA_IVE_CCL_MODE_4C = 0x0,
    XMEDIA_IVE_CCL_MODE_8C = 0x1,

    XMEDIA_IVE_CCL_MODE_BUTT
} xmedia_ive_ccl_mode_e;
typedef struct xmedia_ive_ccl_ctrl_s {
    xmedia_ive_ccl_mode_e en_mode;
    xmedia_u16     u16_init_area_thr;
    xmedia_u16     u16_step;
} xmedia_ive_ccl_ctrl_s;

typedef struct xmedia_ive_gmm_ctrl_s {
    xmedia_u22q10 u22q10_noise_var;
    xmedia_u22q10 u22q10_max_var;
    xmedia_u22q10 u22q10_min_var;
    xmedia_u0q16  u0q16_learn_rate;
    xmedia_u0q16  u0q16_bg_ratio;
    xmedia_u8q8   u8q8_var_thr;
    xmedia_u0q16  u0q16_init_weight;
    xmedia_u8     u8_model_num;
} xmedia_ive_gmm_ctrl_s;

typedef enum xmedia_ive_gmm2_sns_factor_mode_e {
    XMEDIA_IVE_GMM2_SNS_FACTOR_MODE_GLB = 0x0,
    XMEDIA_IVE_GMM2_SNS_FACTOR_MODE_PIX = 0x1,

    XMEDIA_IVE_GMM2_SNS_FACTOR_MODE_BUTT
} xmedia_ive_gmm2_sns_factor_mode_e;

typedef enum xmedia_ive_gmm2_life_update_factor_mode_e {
    XMEDIA_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLB = 0x0,
    XMEDIA_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIX = 0x1,

    XMEDIA_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_BUTT
} xmedia_ive_gmm2_life_update_factor_mode_e;

typedef struct xmedia_ive_gmm2_ctrl_s {
    xmedia_ive_gmm2_sns_factor_mode_e en_sns_factor_mode;
    xmedia_ive_gmm2_life_update_factor_mode_e en_life_update_factor_mode;
    xmedia_u16 u16_glb_life_update_factor;
    xmedia_u16 u16_life_thr;
    xmedia_u16 u16_freq_init_val;
    xmedia_u16 u16_freq_redu_factor;
    xmedia_u16 u16_freq_add_factor;
    xmedia_u16 u16_freq_thr;
    xmedia_u16 u16_var_rate;
    xmedia_u9q7 u9q7_max_var;
    xmedia_u9q7 u9q7_min_var;
    xmedia_u8 u8_glb_sns_factor;
    xmedia_u8 u8_model_num;
} xmedia_ive_gmm2_ctrl_s;

typedef struct xmedia_ive_canny_hys_edge_ctrl_s {
    xmedia_ive_mem_info_s stmem;
    xmedia_u16 u16_low_thr;
    xmedia_u16 u16_high_thr;
    xmedia_s8  as8_mask[25];
} xmedia_ive_canny_hys_edge_ctrl_s;

typedef struct xmedia_ive_canny_stack_size_s {
    xmedia_u32 u32StackSize;
    xmedia_u8 u8_reserved[12];
} xmedia_ive_canny_stack_size_s;

typedef enum xmedia_ive_lbp_cmp_mode_e {
    XMEDIA_IVE_LBP_CMP_MODE_NORMAL = 0x0,
    XMEDIA_IVE_LBP_CMP_MODE_ABS = 0x1,

    XMEDIA_IVE_LBP_CMP_MODE_BUTT
} xmedia_ive_lbp_cmp_mode_e;

typedef struct xmedia_ive_lbp_ctrl_s {
    xmedia_ive_lbp_cmp_mode_e en_mode;
    xmedia_ive_8bit_u un8_bit_thr;
} xmedia_ive_lbp_ctrl_s;

typedef enum xmedia_ive_norm_grad_out_ctrl_e {
    XMEDIA_IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER = 0x0,
    XMEDIA_IVE_NORM_GRAD_OUT_CTRL_HOR = 0x1,
    XMEDIA_IVE_NORM_GRAD_OUT_CTRL_VER = 0x2,
    XMEDIA_IVE_NORM_GRAD_OUT_CTRL_COMBINE = 0x3,

    XMEDIA_IVE_NORM_GRAD_OUT_CTRL_BUTT
} xmedia_ive_norm_grad_out_ctrl_e;

typedef struct xmedia_ive_norm_grad_ctrl_s {
    xmedia_ive_norm_grad_out_ctrl_e en_out_ctrl;
    xmedia_s8 as8_mask[25];
    xmedia_u8 u8_norm;
} xmedia_ive_norm_grad_ctrl_s;

typedef enum xmedia_ive_lk_optical_flow_pyr_out_mode_e {
    XMEDIA_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE = 0,
    XMEDIA_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS = 1,
    XMEDIA_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH = 2,

    XMEDIA_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT
} xmedia_ive_lk_optical_flow_pyr_out_mode_e;

typedef struct xmedia_ive_lk_optical_flow_pyr_ctrl_s {
    xmedia_ive_lk_optical_flow_pyr_out_mode_e en_out_mode;
    xmedia_bool b_use_init_flow;
    xmedia_u16  u16_pts_num;
    xmedia_u8   u8_max_level;
    xmedia_u0q8 u0q8_min_eigthr;
    xmedia_u8   u8_iter_cnt;
    xmedia_u0q8 u0q8_eps;
} xmedia_ive_lk_optical_flow_pyr_ctrl_s;

typedef struct xmedia_ive_st_max_eig_s {
    xmedia_u16 u16_max_eig;
    xmedia_u8 u8_reserved[14];
} xmedia_ive_st_max_eig_s;

typedef struct xmedia_ive_st_candi_corner_ctrl_s {
    xmedia_ive_mem_info_s stmem;
    xmedia_u0q8 u0q8_quality_level;
} xmedia_ive_st_candi_corner_ctrl_s;

typedef struct xmedia_ive_st_corner_info_s {
    xmedia_u16 u16_corner_num;
    xmedia_ive_point_u16_s ast_corner[XMEDIA_IVE_ST_MAX_CORNER_NUM];
} xmedia_ive_st_corner_info_s;

typedef struct xmedia_ive_st_corner_ctrl_s {
    xmedia_u16 u16_max_corner_num;
    xmedia_u16 u16_min_dist;
} xmedia_ive_st_corner_ctrl_s;

typedef enum xmedia_ive_grad_fg_mode_e {
    XMEDIA_IVE_GRAD_FG_MODE_USE_CUR_GRAD = 0x0,
    XMEDIA_IVE_GRAD_FG_MODE_FIND_MIN_GRAD = 0x1,

    XMEDIA_IVE_GRAD_FG_MODE_BUTT
} xmedia_ive_grad_fg_mode_e;

typedef struct xmedia_ive_grad_fg_ctrl_s {
    xmedia_ive_grad_fg_mode_e en_mode;
    xmedia_u16 u16_ed_w_factor;
    xmedia_u8 u8_crl_coef_thr;
    xmedia_u8 u8_mag_crl_thr;
    xmedia_u8 u8_min_mag_diff;
    xmedia_u8 u8_noise_val;
    xmedia_u8 u8_ed_w_dark;
} xmedia_ive_grad_fg_ctrl_s;

typedef struct xmedia_ive_candi_bg_pix_s {
    xmedia_u8q4f4 u8q4f4_mean;
    xmedia_u16 u16_start_time;
    xmedia_u16 u16_sum_access_time;
    xmedia_u16 u16_short_keep_time;
    xmedia_u8 u8_chg_cond;
    xmedia_u8 u8_poten_bg_life;
} xmedia_ive_candi_bg_pix_s;

typedef struct xmedia_ive_work_bg_pix_s {
    xmedia_u8q4f4 u8q4f4_mean;
    xmedia_u16 u16_acc_time;
    xmedia_u8 u8_pre_gray;
    xmedia_u5q3 u5q3_diff_thr;
    xmedia_u8 u8_acc_flag;
    xmedia_u8 u8_bg_gray[3];
} xmedia_ive_work_bg_pix_s;

typedef struct xmedia_ive_bg_life_s {
    xmedia_u8 u8_work_bg_life[3];
    xmedia_u8 u8_candi_bg_life;
} xmedia_ive_bg_life_s;

typedef struct xmedia_ive_bg_model_pix_s {
    xmedia_ive_work_bg_pix_s st_work_bg_pixel;
    xmedia_ive_candi_bg_pix_s st_candi_pixel;
    xmedia_ive_bg_life_s st_bg_life;
} xmedia_ive_bg_model_pix_s;

typedef struct xmedia_ive_fg_stat_data_s {
    xmedia_u32 u32_pix_num;
    xmedia_u32 u32_sum_lum;
    xmedia_u8  u8_reserved[8];
} xmedia_ive_fg_stat_data_s;

typedef struct xmedia_ive_bg_stat_data_s {
    xmedia_u32 u32_pix_num;
    xmedia_u32 u32_sum_lum;
    xmedia_u8 u8_reserved[8];
} xmedia_ive_bg_stat_data_s;

typedef struct xmedia_ive_match_bg_model_ctrl_s {
    xmedia_u32 u32_cur_frm_num;
    xmedia_u32 u32_pre_frm_num;
    xmedia_u16 u16_time_thr;

    xmedia_u8 u8_diff_thr_crl_coef;
    xmedia_u8 u8_diff_max_thr;
    xmedia_u8 u8_diff_min_thr;
    xmedia_u8 u8_diff_thr_inc;
    xmedia_u8 u8_fast_learn_rate;
    xmedia_u8 u8_det_chg_region;
} xmedia_ive_match_bg_model_ctrl_s;

typedef struct xmedia_ive_update_bg_model_ctrl_s {
    xmedia_u32 u32_cur_frm_num;
    xmedia_u32 u32_pre_chk_time;
    xmedia_u32 u32_frm_chk_period;

    xmedia_u32 u32_init_min_time;
    xmedia_u32 u32_sty_bg_min_blend_time;
    xmedia_u32 u32_sty_bg_max_blend_time;
    xmedia_u32 u32_dyn_bg_min_blend_time;
    xmedia_u32 u32_static_det_min_time;
    xmedia_u16 u16_fg_max_fade_time;
    xmedia_u16 u16_bg_max_fade_time;

    xmedia_u8 u8_sty_bg_acc_time_rate_thr;
    xmedia_u8 u8_chg_bg_acc_time_rate_thr;
    xmedia_u8 u8_dyn_bg_acc_time_thr;
    xmedia_u8 u8_dyn_bg_depth;
    xmedia_u8 u8_bg_eff_sta_rate_thr;

    xmedia_u8 u8_acce_bg_learn;
    xmedia_u8 u8_det_chg_region;
} xmedia_ive_update_bg_model_ctrl_s;

typedef enum xmedia_ive_ann_mlp_activ_func_e {
    XMEDIA_IVE_ANN_MLP_ACTIV_FUNC_IDENTITY = 0x0,
    XMEDIA_IVE_ANN_MLP_ACTIV_FUNC_SIGMOID_SYM = 0x1,
    XMEDIA_IVE_ANN_MLP_ACTIV_FUNC_GAUSSIAN = 0x2,

    XMEDIA_IVE_ANN_MLP_ACTIV_FUNC_BUTT
} xmedia_ive_ann_mlp_activ_func_e;

typedef enum xmedia_ive_ann_mlp_accurate_e {
    XMEDIA_IVE_ANN_MLP_ACCURATE_SRC16_WGT16 = 0x0,
    XMEDIA_IVE_ANN_MLP_ACCURATE_SRC14_WGT20 = 0x1,

    XMEDIA_IVE_ANN_MLP_ACCURATE_BUTT
} xmedia_ive_ann_mlp_accurate_e;

typedef struct xmedia_ive_ann_mlp_model_s {
    xmedia_ive_ann_mlp_activ_func_e en_activ_func;
    xmedia_ive_ann_mlp_accurate_e en_accurate;
    xmedia_ive_mem_info_s st_weight;
    xmedia_u32 u32_total_weight_size;

    xmedia_u16 au16_layer_count[8];
    xmedia_u16 u16_max_count;
    xmedia_u8 u8_layer_num;
    xmedia_u8 u8_reserved;
} xmedia_ive_ann_mlp_model_s;

typedef enum xmedia_ive_svm_type_e {
    XMEDIA_IVE_SVM_TYPE_C_SVC = 0x0,
    XMEDIA_IVE_SVM_TYPE_NU_SVC = 0x1,

    XMEDIA_IVE_SVM_TYPE_BUTT
} xmedia_ive_svm_type_e;

typedef enum xmedia_ive_svm_kernel_type_e {
    XMEDIA_IVE_SVM_KERNEL_TYPE_LINEAR = 0x0,
    XMEDIA_IVE_SVM_KERNEL_TYPE_POLY = 0x1,
    XMEDIA_IVE_SVM_KERNEL_TYPE_RBF = 0x2,
    XMEDIA_IVE_SVM_KERNEL_TYPE_SIGMOID = 0x3,

    XMEDIA_IVE_SVM_KERNEL_TYPE_BUTT
} xmedia_ive_svm_kernel_type_e;

typedef struct xmedia_ive_svm_model_s {
    xmedia_ive_svm_type_e en_type;
    xmedia_ive_svm_kernel_type_e en_kernel_type;

    xmedia_ive_mem_info_s st_sv;
    xmedia_ive_mem_info_s st_df;
    xmedia_u32 u32_total_df_size;

    xmedia_u16 u16_feature_dim;
    xmedia_u16 u16_sv_total;
    xmedia_u8 u8_class_count;
} xmedia_ive_svm_model_s;

typedef enum xmedia_ive_sad_mode_e {
    XMEDIA_IVE_SAD_MODE_MB_4X4 = 0x0,
    XMEDIA_IVE_SAD_MODE_MB_8X8 = 0x1,
    XMEDIA_IVE_SAD_MODE_MB_16X16 = 0x2,

    XMEDIA_IVE_SAD_MODE_BUTT
} xmedia_ive_sad_mode_e;
typedef enum xmedia_ive_sad_out_ctrl_e {
    XMEDIA_IVE_SAD_OUT_CTRL_16BIT_BOTH = 0x0,
    XMEDIA_IVE_SAD_OUT_CTRL_8BIT_BOTH = 0x1,
    XMEDIA_IVE_SAD_OUT_CTRL_16BIT_SAD = 0x2,
    XMEDIA_IVE_SAD_OUT_CTRL_8BIT_SAD = 0x3,
    XMEDIA_IVE_SAD_OUT_CTRL_THRESH = 0x4,

    XMEDIA_IVE_SAD_OUT_CTRL_BUTT
} xmedia_ive_sad_out_ctrl_e;
typedef struct xmedia_ive_sad_ctrl_s {
    xmedia_ive_sad_mode_e en_mode;
    xmedia_ive_sad_out_ctrl_e en_out_ctrl;
    xmedia_u16 u16_thr;
    xmedia_u8 u8_min_val;
    xmedia_u8 u8_max_val;
} xmedia_ive_sad_ctrl_s;

typedef enum xmedia_ive_resize_mode_e {
    XMEDIA_IVE_RESIZE_MODE_LINEAR = 0x0,
    XMEDIA_IVE_RESIZE_MODE_AREA = 0x1,

    XMEDIA_IVE_RESIZE_MODE_BUTT
} xmedia_ive_resize_mode_e;

typedef struct xmedia_ive_resize_ctrl_s {
    xmedia_ive_resize_mode_e en_mode;
    xmedia_ive_mem_info_s stmem;
    xmedia_u16 u16_num;
} xmedia_ive_resize_ctrl_s;

typedef enum xmedia_ive_cnn_activ_func_e {
    XMEDIA_IVE_CNN_ACTIV_FUNC_NONE = 0x0,
    XMEDIA_IVE_CNN_ACTIV_FUNC_RELU = 0x1,
    XMEDIA_IVE_CNN_ACTIV_FUNC_SIGMOID = 0x2,

    XMEDIA_IVE_CNN_ACTIV_FUNC_BUTT
} xmedia_ive_cnn_activ_func_e;

typedef enum xmedia_ive_cnn_pooling_e {
    XMEDIA_IVE_CNN_POOLING_NONE = 0x0,
    XMEDIA_IVE_CNN_POOLING_MAX = 0x1,
    XMEDIA_IVE_CNN_POOLING_AVG = 0x2,

    XMEDIA_IVE_CNN_POOLING_BUTT
} xmedia_ive_cnn_pooling_e;

typedef struct xmedia_ive_cnn_conv_pooling_s {
    xmedia_ive_cnn_activ_func_e en_activ_func;
    xmedia_ive_cnn_pooling_e en_pooling;

    xmedia_u8 u8_feature_map_num;
    xmedia_u8 u8_kernel_size;
    xmedia_u8 u8_conv_step;

    xmedia_u8 u8_pool_size;
    xmedia_u8 u8_pool_step;
    xmedia_u8 u8_reserved[3];

} xmedia_ive_cnn_conv_pooling_s;

typedef struct xmedia_ive_cnn_full_connect_s {
    xmedia_u16 au16_layer_cnt[8];
    xmedia_u16 u16_max_cnt;
    xmedia_u8  u8_layer_num;
    xmedia_u8  u8_reserved;
} xmedia_ive_cnn_full_connect_s;

typedef struct xmedia_ive_cnn_model_s {
    xmedia_ive_cnn_conv_pooling_s ast_conv_pool[8];
    xmedia_ive_cnn_full_connect_s st_full_connect;

    xmedia_ive_mem_info_s st_conv_kernel_bias;
    xmedia_u32 u32_conv_kernel_bias_size;

    xmedia_ive_mem_info_s st_fcl_wgt_bias;
    xmedia_u32 u32_fcl_wgt_bias_size;

    xmedia_u32 u32_total_mem_size;

    xmedia_ive_image_type_e en_type;
    xmedia_u32 u32width;
    xmedia_u32 u32height;

    xmedia_u16 u16_class_count;
    xmedia_u8 u8_conv_pool_layer_num;
    xmedia_u8 u8_reserved;
} xmedia_ive_cnn_model_s;

typedef struct xmedia_ive_cnn_ctrl_s {
    xmedia_ive_mem_info_s stmem;
    xmedia_u32 u32_num;
} xmedia_ive_cnn_ctrl_s;

typedef struct xmedia_ive_cnn_result_s {
    xmedia_s32 s32_class_idx;
    xmedia_s32 s32_confidence;
} xmedia_ive_cnn_result_s;

typedef enum xmedia_ive_persp_trans_alg_mode_e {
    XMEDIA_IVE_PERSP_TRANS_ALG_MODE_NR_SIM = 0x0,
    XMEDIA_IVE_PERSP_TRANS_ALG_MODE_SIM = 0x1,
    XMEDIA_IVE_PERSP_TRANS_ALG_MODE_AFFINE = 0x2,

    XMEDIA_IVE_PERSP_TRANS_ALG_MODE_BUTT
} xmedia_ive_persp_trans_alg_mode_e;

typedef struct xmedia_ive_persp_trans_point_pair_s {
    xmedia_ive_point_u14q2_s st_src_point;
    xmedia_ive_point_u14q2_s st_dst_point;
} xmedia_ive_persp_trans_point_pair_s;

typedef enum xmedia_ive_persp_trans_csc_mode_e {
    XMEDIA_IVE_PERSP_TRANS_CSC_MODE_NONE = 0x0,
    XMEDIA_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x1,
    XMEDIA_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT709_YUV2RGB = 0x2,
    XMEDIA_IVE_PERSP_TRANS_CSC_MODE_PIC_BT601_YUV2RGB = 0x3,
    XMEDIA_IVE_PERSP_TRANS_CSC_MODE_PIC_BT709_YUV2RGB = 0x4,

    XMEDIA_IVE_PERSP_TRANS_CSC_MODE_BUTT
} xmedia_ive_persp_trans_csc_mode_e;

typedef struct xmedia_ive_persp_trans_ctrl_s {
    xmedia_ive_persp_trans_alg_mode_e en_alg_mode;
    xmedia_ive_persp_trans_csc_mode_e en_csc_mode;
    xmedia_u16 u16_roi_num;
    xmedia_u16 u16_point_pair_num;
} xmedia_ive_persp_trans_ctrl_s;

typedef struct xmedia_ive_roi_info_s {
    xmedia_ive_rect_s24q8_s st_roi;
    xmedia_u32 u32_roi_id;
} xmedia_ive_roi_info_s;

typedef struct xmedia_ive_kcf_pro_ctrl_s {
    xmedia_ive_csc_mode_e en_csc_mode;
    xmedia_ive_mem_info_s st_tmp_buf;
    xmedia_u1q15 u1q15_inter_factor;
    xmedia_u0q16 u0q16_lamda;
    xmedia_u4q12 u4q12_trancalfa;
    xmedia_u0q8 u0q8_sigma;
    xmedia_u8 u8_resp_thr;
} xmedia_ive_kcf_pro_ctrl_s;

typedef struct xmedia_ive_kcf_obj_s {
    xmedia_ive_roi_info_s st_roi_info;
    xmedia_ive_mem_info_s st_cos_win_x;
    xmedia_ive_mem_info_s st_cos_win_y;
    xmedia_ive_mem_info_s st_gauss_peak;
    xmedia_ive_mem_info_s st_hog_feature;
    xmedia_ive_mem_info_s st_alpha;
    xmedia_ive_mem_info_s st_dst;
    xmedia_u3q5 u3q5_padding;
    xmedia_u8 au8_reserved[3];
} xmedia_ive_kcf_obj_s;

typedef struct xmedia_ive_list_head_s {
    struct xmedia_ive_list_head_s *pst_next, *pst_prev;
} xmedia_ive_list_head_s;

typedef struct xmedia_ive_kcf_obj_node_s {
    xmedia_ive_list_head_s st_list;
    xmedia_ive_kcf_obj_s st_kcf_obj;
} xmedia_ive_kcf_obj_node_s;

typedef enum xmedia_ive_kcf_list_state_e {
    IVE_KCF_LIST_STATE_CREATE = 0x1,
    IVE_KCF_LIST_STATE_DESTORY = 0x2,
    IVE_KCF_LIST_STATE_BUTT
} xmedia_ive_kcf_list_state_e;

typedef struct xmedia_ive_kcf_obj_list_s {
    xmedia_ive_kcf_obj_node_s *pst_obj_node_buf;
    xmedia_ive_list_head_s st_free_obj_list;
    xmedia_ive_list_head_s st_train_obj_list;
    xmedia_ive_list_head_s st_track_obj_list;

    xmedia_u32 u32_free_obj_num;
    xmedia_u32 u32_train_obj_num;
    xmedia_u32 u32_track_obj_num;
    xmedia_u32 u32_max_obj_num;
    xmedia_ive_kcf_list_state_e en_list_state;
    xmedia_u8 *pu8_tmp_buf;
} xmedia_ive_kcf_obj_list_s;

typedef struct xmedia_ive_kcf_bbox_s {
    xmedia_ive_kcf_obj_node_s *pst_node;
    xmedia_s32 s32_response;

    xmedia_ive_roi_info_s st_roi_info;
    xmedia_bool b_track_ok;
    xmedia_bool b_roi_refresh;
} xmedia_ive_kcf_bbox_s;

typedef struct xmedia_ive_kcf_bbox_ctrl_s {
    xmedia_u32 u32_max_bbox_num;
    xmedia_s32 s32_resp_thr;
} xmedia_ive_kcf_bbox_ctrl_s;

typedef enum xmedia_ive_hog_mode_e {
    XMEDIA_IVE_HOG_MODE_VERTICAL_TANGENT_PLANE = 0x1,
    XMEDIA_IVE_HOG_MODE_HORIZONTAL_TANGENT_PLANE = 0x2,
    XMEDIA_IVE_HOG_MODE_BUTT
} xmedia_ive_hog_mode_e;

typedef struct xmedia_ive_hog_ctrl_s {
    xmedia_ive_csc_mode_e en_csc_mode;
    xmedia_ive_hog_mode_e en_hog_mode;
    xmedia_u32 u32_roi_num;
    xmedia_u4q12 u4q12_trancalfa;
    xmedia_u8 au8_rsv[2];
} xmedia_ive_hog_ctrl_s;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

