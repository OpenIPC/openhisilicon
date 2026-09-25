/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef IVP_H
#define IVP_H

#include "type.h"
#include "errcode.h"
#include "common.h"
#include "comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IVP_MAX_VENC_CHN_NUM 16
#define IVP_MAX_VIPIPE_NUM   2
#define IVP_MAX_CLASS        2
#define IVP_CLASS_NAME_LEN   32

typedef struct {
    xmedia_u64 physical_addr; /* RW;The physical address of the memory */
    xmedia_u64 virtual_addr;  /* RW;The virtual address of the memory */
    xmedia_u32 memory_size;   /* RW;The size_value of memory */
} ivp_mem_info;

typedef struct {
    xmedia_float threshold;
} ivp_ctrl_attr;

typedef struct {
    xmedia_s32 x;
    xmedia_s32 y;
    xmedia_u32 width;
    xmedia_u32 height;
} ivp_rect;

typedef struct {
    ivp_rect rect;
    xmedia_float quality;
} ivp_obj;

typedef struct {
    xmedia_char class_name[IVP_CLASS_NAME_LEN];
    xmedia_u32 rect_num;
    xmedia_u32 rect_capcity;
    ivp_obj *objs;
} ivp_obj_of_one_class;

typedef struct {
    xmedia_s32 class_num;
    ivp_obj_of_one_class obj_class[IVP_MAX_CLASS];
} ivp_obj_array;

typedef struct {
    xmedia_bool enable;
    xmedia_u32 threshold; /* RW;Range: [1,1024] */
} ivp_roi_attr;

typedef enum {
    IVP_ROI_MB_MODE_4X4,
    IVP_ROI_MB_MODE_8X8,
    IVP_ROI_MB_MODE_16X16,
    IVP_ROI_MB_MODE_BUTT
} ivp_roi_mb_mode;

typedef struct {
    ivp_roi_mb_mode roi_mb_mode;
    xmedia_u32 img_width;  /* equal to the width of processing frame */
    xmedia_u32 img_height; /* equal to the height of processing frame */
    xmedia_u8 *mb_map;     /* for 4x4 mb mode, alloc (DIV_UP(img_width,4))*(DIV_UP(img_height,4)) bytes */
} ivp_roi_map;

/*
 * Prototype: ivp_init
 * Description: Intelligent Video Process(IVP) initialization.
 * Parameters: xmedia_void.
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_init(xmedia_void);

/*
 * Prototype: ivp_deinit
 * Description: Intelligent Video Process(IVP) exit.
 * Parameters: xmedia_void.
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_deinit(xmedia_void);

/*
 * Prototype: ivp_load_resource_from_memory
 * Description: Load resource from memory.
 * Parameters: const ivp_mem_info *ivp_file_mem_info:Input
 *             xmedia_s32 *ivp_handle:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_load_resource_from_memory(const ivp_mem_info *ivp_file_mem_info, xmedia_s32 *ivp_handle);

/*
 * Prototype: ivp_unload_resource
 * Description: Unload resource.
 * Parameters: xmedia_s32 ivp_handle:Input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_unload_resource(xmedia_s32 ivp_handle);

/*
 * Prototype: ivp_set_ctrl_attr
 * Description: Set ctrl param,include SMD threshold.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             const ivp_ctrl_attr *_ivp_ctrl_attr:input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_set_ctrl_attr(xmedia_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

/*
 * Prototype: ivp_get_ctrl_attr
 * Description: Get ctrl param,include SMD threshold.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             ivp_ctrl_attr *_ivp_ctrl_attr:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_get_ctrl_attr(xmedia_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

/*
 * Prototype: ivp_set_fd_ctrl_attr
 * Description: Set FD ctrl param,include FD threshold. Only XM72020300 support.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             const ivp_ctrl_attr *_ivp_ctrl_attr:input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_set_fd_ctrl_attr(xmedia_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

/*
 * Prototype: ivp_get_fd_ctrl_attr
 * Description: Get FD ctrl param,include FD threshold. Only XM72020300 support.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             ivp_ctrl_attr *_ivp_ctrl_attr:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_get_fd_ctrl_attr(xmedia_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

/*
 * Prototype: ivp_set_venc_low_bitrate
 * Description: Enable or disable venc low bitrate.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             xmedia_s32 venc_chn:Input
 *             xmedia_bool enable:Input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_set_venc_low_bitrate(xmedia_s32 ivp_handle, xmedia_s32 venc_chn, xmedia_bool enable);

/*
 * Prototype: ivp_get_venc_low_bitrate
 * Description: Get status of venc low bitrate.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             xmedia_s32 venc_chn:Input
 *             xmedia_bool *enable:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_get_venc_low_bitrate(xmedia_s32 ivp_handle, xmedia_s32 venc_chn, xmedia_bool *enable);

/*
 * Prototype: ivp_set_advance_isp
 * Description: Enable or disable advance Isp Attr.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             xmedia_s32 vi_pipe:Input
 *             xmedia_bool enable:Input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_set_advance_isp(xmedia_s32 ivp_handle, xmedia_s32 vi_pipe, xmedia_bool enable);

/*
 * Prototype: ivp_get_advance_isp
 * Description: Get status of advance ISP attr.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             xmedia_s32 vi_pipe:Input
 *             xmedia_bool *enable:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_get_advance_isp(xmedia_s32 ivp_handle, xmedia_s32 vi_pipe, xmedia_bool *enable);

/*
 * Prototype: ivp_set_roi_attr
 * Description: Set ROI Attr.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             const ivp_roi_attr *roi_attr:Input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_set_roi_attr(xmedia_s32 ivp_handle, const ivp_roi_attr *roi_attr);

/*
 * Prototype: ivp_get_roi_attr
 * Description: Get ROI Attr.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             ivp_roi_attr *roi_attr:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_get_roi_attr(xmedia_s32 ivp_handle, ivp_roi_attr *roi_attr);

/*
 * Prototype: ivp_set_roi_map
 * Description: Set ROI Map.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             const ivp_roi_map *roi_map:Input
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_set_roi_map(xmedia_s32 ivp_handle, const ivp_roi_map *roi_map);

/*
 * Prototype: ivp_process
 * Description: Process.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             const VIDEO_FRAME_INFO_S *src_frame:Input
 *             xmedia_bool *obj_alarm:Output,alarm info
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_process(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, xmedia_bool *obj_alarm);

/*
 * Prototype: ivp_process_ex
 * Description: Process.
 * Parameters: xmedia_s32 ivp_handle:Input
 *             const VIDEO_FRAME_INFO_S *src_frame:Input
 *             ivp_obj_array *obj_array:Output
 * Return Value: XMEDIA_SUCCESS: Success;Error codes: Failure.
 */
xmedia_s32 ivp_process_ex(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, ivp_obj_array *obj_array);

#define IVP_MAX_PRIVATE_DATA_LEN 32

typedef enum {
    IVP_OBJ_PEDESTRAIN,
    IVP_OBJ_FACE,
    IVP_OBJ_BUTT
} ivp_obj_type;

typedef enum {
    IVP_FUNC_DETECT,
    IVP_FUNC_TRACK,
    IVP_FUNC_BUTT
} ivp_func_type;

typedef enum {
    IVP_CROP_NONE,
    IVP_CROP_NON_UNIFORM,
    IVP_CROP_UNIFORM,
    IVP_CROP_BUTT
} ivp_crop_mode;

typedef enum {
    IVP_CROP_ORIGINAL_SHAPE,
    IVP_CROP_SQUARE_BASED_ON_LONG_SIDE,
    IVP_CROP_SHAPE_BUTT
} ivp_crop_shape;

typedef enum {
    IVP_EDGE_FILL_DEFAULT,
    IVP_EDGE_FILL_CUSTOM,
    IVP_EDGE_FILL_BUTT
} ivp_crop_edge_fill_mode;

typedef struct {
    ivp_crop_edge_fill_mode fill_mode;
    xmedia_u8 color;
} ivp_crop_edge_fill_attr;

typedef struct {
    ivp_crop_mode crop_mode;
    PIXEL_FORMAT_E crop_format;
    xmedia_u32 uniform_width;
    xmedia_u32 uniform_height;
    xmedia_float expand_coef;
    ivp_crop_shape crop_shape;
    ivp_crop_edge_fill_attr fill_attr;
} ivp_crop_attr;

typedef struct {
    xmedia_u16 max_select_obj_num;
    ivp_crop_attr crop_attr;
} ivp_select_obj_attr;

typedef struct {
    xmedia_u16 max_track_obj_num;
} ivp_track_attr;

typedef struct {
    ivp_func_type func_type[IVP_OBJ_BUTT];
    xmedia_bool support_select_obj[IVP_OBJ_BUTT];
    ivp_track_attr track_attr[IVP_OBJ_BUTT];
    ivp_select_obj_attr select_obj_attr[IVP_OBJ_BUTT];
    xmedia_u32 img_width;
    xmedia_u32 img_height;
} ivp_ctx_attr;

typedef struct {
    ivp_rect rect;
    xmedia_float quality;
    xmedia_s64 id;
} ivp_det_track_obj;

typedef struct {
    xmedia_u32 obj_num;
    xmedia_u32 obj_capacity;
    ivp_det_track_obj *objs;
} ivp_det_track_obj_class;

typedef struct {
    ivp_det_track_obj_class obj_class[IVP_OBJ_BUTT];
} ivp_det_track_obj_array;

typedef struct {
    ivp_rect rect;
    xmedia_s64 id;
    xmedia_float probability;
    xmedia_float quality;
    xmedia_u32 area;
    xmedia_u64 pts;
    VIDEO_FRAME_INFO_S crop;
    xmedia_u8 private_data[IVP_MAX_PRIVATE_DATA_LEN];
} ivp_select_obj;

typedef struct {
    xmedia_u32 obj_num;
    xmedia_u32 obj_capacity;
    ivp_select_obj *objs;
} ivp_select_obj_class;

typedef struct {
    ivp_select_obj_class obj_class[IVP_OBJ_BUTT];
} ivp_select_obj_array;

typedef enum {
    IVP_GESTURE_PALM_TIGHT,
    IVP_GESTURE_THUMB_INDEX_L,
    IVP_GESTURE_THUMB_PINKY_Y,
    IVP_GESTURE_UNKNOW
} ivp_gesture_type;


typedef struct {
    ivp_gesture_type type;
    ivp_rect rect;
} ivp_gesture_obj;

typedef struct {
    xmedia_u32 obj_num;
    xmedia_u32 obj_capacity;
    ivp_gesture_obj *objs;
} ivp_gesture_obj_array;

xmedia_s32 ivp_create_track_ctx(xmedia_s32 ivp_handle, const ivp_ctx_attr *ctx_attr);

xmedia_s32 ivp_destroy_track_ctx(xmedia_s32 ivp_handle);

xmedia_s32 ivp_detect(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, ivp_det_track_obj_array *obj_array);

xmedia_s32 ivp_track(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *pre_frame, const VIDEO_FRAME_INFO_S *cur_frame,
    ivp_det_track_obj_array *obj_array);

xmedia_s32 ivp_track_detect(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *pre_frame, const VIDEO_FRAME_INFO_S *cur_frame,
    ivp_det_track_obj_array *obj_array);

xmedia_s32 ivp_get_select_objs(xmedia_s32 ivp_handle, ivp_select_obj_array *obj_array);

xmedia_s32 ivp_release_select_objs(xmedia_s32 ivp_handle, const ivp_select_obj_array *obj_array);

xmedia_s32 ivp_load_mutil_resource_from_memory(const ivp_mem_info ivp_file_mem_info[], xmedia_u32 res_num,
    xmedia_s32 *ivp_handle);

xmedia_s32 ivp_set_hgr_ctrl_attr(xmedia_s32 ivp_handle, const ivp_ctrl_attr *_ivp_ctrl_attr);

xmedia_s32 ivp_get_hgr_ctrl_attr(xmedia_s32 ivp_handle, ivp_ctrl_attr *_ivp_ctrl_attr);

xmedia_s32 ivp_hgr(xmedia_s32 ivp_handle, const VIDEO_FRAME_INFO_S *src_frame, ivp_gesture_obj_array *obj_array);

/* Error Code */
typedef enum {
    ERR_IVP_READ_FILE = 0x41,    /* IVP read file error */
    ERR_IVP_OPERATE_FILE = 0x42, /* IVP operate file error */
    ERR_IVP_PROCESS_ERR = 0x43,
    ERR_IVP_INIT_FAIL = 0x44, /* IVP init fail */
    ERR_IVP_EXIT_FAIL = 0x45, /* IVP exit fail */
    ERR_IVP_LOAD_RESOURCE_FAIL = 0x46,
    ERR_IVP_PTS_NOT_MATCH = 0x47,
    ERR_IVP_CAL_PRIVATE_DATA = 0x48,

    ERR_IVP_BUTT
} ivp_err_code;

#define ERR_CODE_IVP_NULL_PTR           DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_IVP_ILLEGAL_PARAM      DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_IVP_NOT_SURPPORT       DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_IVP_INIT_FAIL          DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_INIT_FAIL)
#define ERR_CODE_IVP_EXIT_FAIL          DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_EXIT_FAIL)
#define ERR_CODE_IVP_NOMEM              DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_IVP_EXIST              DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_IVP_UNEXIST            DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_IVP_READ_FILE          DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_READ_FILE)
#define ERR_CODE_IVP_OPERATE_FILE       DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_OPERATE_FILE)
#define ERR_CODE_IVP_PROCESS_ERR        DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_PROCESS_ERR)
#define ERR_CODE_IVP_LOAD_RESOURCE_FAIL DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_LOAD_RESOURCE_FAIL)
#define ERR_CODE_IVP_PTS_NOT_MATCH      DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_PTS_NOT_MATCH)
#define ERR_CODE_IVP_BUSY               DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_IVP_CAL_PRIVATE_DATA   DEFINE_ERR_CODE(MOD_ID_IVP, EN_ERR_LEVEL_ERROR, ERR_IVP_CAL_PRIVATE_DATA)

#ifdef __cplusplus
}
#endif

#endif /* IVP_H */

