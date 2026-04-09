/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * hi_ivp.h — public API for libivp_neo, the open-source replacement
 * for vendor libivp.so.
 *
 * ABI matches vendor hi_ivp.h (HiSilicon SDK V1.0.1.2) — struct layouts,
 * enum values, and function signatures are preserved so test binaries
 * can drop in libivp_neo without recompilation changes.
 *
 * NOTE: on Hi3516EV200/EV300 the XNN hardware that underpins IVP object
 * detection does not actually execute Conv inference (verified against
 * vendor's open_ive.ko — see project_ive_neo_phase4 memory). Both vendor
 * libivp.so and libivp_neo therefore observe the same behavior: the
 * pipeline runs without errors but produces zero detections on this SoC.
 */
#ifndef __HI_IVP_H__
#define __HI_IVP_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HI_IVP_MAX_VENC_CHN_NUM 16
#define HI_IVP_MAX_VIPIPE_NUM   2
#define HI_IVP_MAX_CLASS        2
#define HI_IVP_CLASS_NAME_LEN   32

typedef struct {
    hi_u64 physical_addr;
    hi_u64 virtual_addr;
    hi_u32 memory_size;
} hi_ivp_mem_info;

typedef struct {
    hi_float threshold;
} hi_ivp_ctrl_attr;

typedef struct {
    hi_s32 x;
    hi_s32 y;
    hi_u32 width;
    hi_u32 height;
} hi_ivp_rect;

typedef struct {
    hi_ivp_rect rect;
    hi_float    quality;
} hi_ivp_obj;

typedef struct {
    hi_char      class_name[HI_IVP_CLASS_NAME_LEN];
    hi_u32       rect_num;
    hi_u32       rect_capcity;
    hi_ivp_obj  *objs;
} hi_ivp_obj_of_one_class;

typedef struct {
    hi_s32                   class_num;
    hi_ivp_obj_of_one_class  obj_class[HI_IVP_MAX_CLASS];
} hi_ivp_obj_array;

typedef struct {
    hi_bool enable;
    hi_u32  threshold;
} hi_ivp_roi_attr;

typedef enum {
    HI_IVP_ROI_MB_MODE_4X4,
    HI_IVP_ROI_MB_MODE_8X8,
    HI_IVP_ROI_MB_MODE_16X16,
    HI_IVP_ROI_MB_MODE_BUTT
} hi_ivp_roi_mb_mode;

typedef struct {
    hi_ivp_roi_mb_mode  roi_mb_mode;
    hi_u32              img_width;
    hi_u32              img_height;
    hi_u8              *mb_map;
} hi_ivp_roi_map;

/* --- Public API --- */

hi_s32 hi_ivp_init(hi_void);
hi_s32 hi_ivp_deinit(hi_void);

hi_s32 hi_ivp_load_resource_from_memory(const hi_ivp_mem_info *ivp_file_mem_info,
                                        hi_s32 *ivp_handle);
hi_s32 hi_ivp_unload_resource(hi_s32 ivp_handle);

hi_s32 hi_ivp_set_ctrl_attr(hi_s32 ivp_handle, const hi_ivp_ctrl_attr *ivp_ctrl_attr);
hi_s32 hi_ivp_get_ctrl_attr(hi_s32 ivp_handle, hi_ivp_ctrl_attr *ivp_ctrl_attr);

hi_s32 hi_ivp_set_fd_ctrl_attr(hi_s32 ivp_handle, const hi_ivp_ctrl_attr *ivp_ctrl_attr);
hi_s32 hi_ivp_get_fd_ctrl_attr(hi_s32 ivp_handle, hi_ivp_ctrl_attr *ivp_ctrl_attr);

hi_s32 hi_ivp_set_venc_low_bitrate(hi_s32 ivp_handle, hi_s32 venc_chn, hi_bool enable);
hi_s32 hi_ivp_get_venc_low_bitrate(hi_s32 ivp_handle, hi_s32 venc_chn, hi_bool *enable);

hi_s32 hi_ivp_set_advance_isp(hi_s32 ivp_handle, hi_s32 vi_pipe, hi_bool enable);
hi_s32 hi_ivp_get_advance_isp(hi_s32 ivp_handle, hi_s32 vi_pipe, hi_bool *enable);

hi_s32 hi_ivp_set_roi_attr(hi_s32 ivp_handle, const hi_ivp_roi_attr *roi_attr);
hi_s32 hi_ivp_get_roi_attr(hi_s32 ivp_handle, hi_ivp_roi_attr *roi_attr);
hi_s32 hi_ivp_set_roi_map(hi_s32 ivp_handle, const hi_ivp_roi_map *roi_map);

hi_s32 hi_ivp_process(hi_s32 ivp_handle,
                      const VIDEO_FRAME_INFO_S *src_frame,
                      hi_bool *obj_alarm);

hi_s32 hi_ivp_process_ex(hi_s32 ivp_handle,
                         const VIDEO_FRAME_INFO_S *src_frame,
                         hi_ivp_obj_array *obj_array);

/* --- Error codes --- */

#define HI_ERR_IVP_NULL_PTR       HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define HI_ERR_IVP_ILLEGAL_PARAM  HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define HI_ERR_IVP_NOT_SURPPORT   HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define HI_ERR_IVP_NOMEM          HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define HI_ERR_IVP_EXIST          HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define HI_ERR_IVP_UNEXIST        HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define HI_ERR_IVP_BUSY           HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define HI_ERR_IVP_INIT_FAIL      HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, 0x44)
#define HI_ERR_IVP_EXIT_FAIL      HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, 0x45)
#define HI_ERR_IVP_LOAD_FAIL      HI_DEF_ERR(HI_ID_VALG, EN_ERR_LEVEL_ERROR, 0x46)

#ifdef __cplusplus
}
#endif

#endif /* __HI_IVP_H__ */
