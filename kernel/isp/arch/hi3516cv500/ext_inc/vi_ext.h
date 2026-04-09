/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: vi_ext.h
 * Author:
 * Create: 2016-10-08
 */

#ifndef __VI_EXT_H__
#define __VI_EXT_H__

#include "hi_type.h"
#include "hi_common_adapt.h"
#include "hi_comm_isp_adapt.h"
#include "hi_comm_vi_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_sys_adapt.h"
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
#include "hi_comm_pq_ai.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    hi_s32 (*pfn_vi_get_flash_status)(hi_void);
} vi_flash_callback;

typedef struct {
    hi_dynamic_range dynamic_range;
    hi_pixel_format format;
    hi_bool mirror_en;
} vi_pipe_hdr_attr;

typedef struct {
    hi_vi_dev vi_dev;
    hi_wdr_mode wdr_mode;
    hi_vi_dev_bind_pipe dev_bind_pipe;
    hi_bool mast_pipe;
} vi_pipe_wdr_attr;

/* Defines the ISP stitch attribute. */
typedef struct {
    hi_bool stitch_enable;
    hi_bool main_pipe;
    hi_u8 stitch_pipe_num;
    hi_s8 stitch_bind_id[VI_MAX_PIPE_NUM];
} vi_stitch_attr;

typedef struct {
    hi_void *sync_be_stt[VI_MAX_PIPE_NUM];
} isp_stitch_sync_be_stats;

typedef struct {
    hi_bool discard_frame;
    hi_u32 frame_flag;
} vi_snap_status;

typedef struct {
    hi_vi_vpss_mode_type mode;
    hi_s32 split_num;
    hi_u32 over_lap;
    hi_rect rect[VI_MAX_SPLIT_NODE_NUM];
    hi_rect wch_out_rect;
    hi_pixel_format pix_fmt;
} vi_pipe_split_attr;

typedef struct {
    hi_bool vi_proc_en[VIPROC_IP_NUM];
} vi_proc_module_param;

typedef enum {
    SNAP_STATE_NULL = 0,
    SNAP_STATE_CFG = 1,
    SNAP_STATE_BUTT
} snap_isp_state;

typedef struct {
    snap_isp_state snap_state;
    hi_u32 pro_index;
    hi_isp_config_info isp_cfg_info;
} isp_snap_info;

typedef hi_s32 fn_vi_register_flash_call_back(hi_s32 vi_dev, vi_flash_callback *flash_cb);
typedef hi_s32 fn_vi_get_pipe_wdr_attr(hi_vi_pipe vi_pipe, vi_pipe_wdr_attr *wdr_attr);
typedef hi_s32 fn_vi_get_pipe_hdr_attr(hi_vi_pipe vi_pipe, vi_pipe_hdr_attr *hdr_attr);
typedef hi_s32 fn_vi_get_be_stt_info(hi_vi_pipe vi_pipe, hi_u8 block, hi_void *be_stt);
typedef hi_s32 fn_vi_get_pipe_stitch_attr(hi_vi_pipe vi_pipe, vi_stitch_attr *stitch_attr);
typedef hi_s32 fn_vi_get_stitch_sync_be_stt_info(hi_vi_pipe vi_pipe, isp_stitch_sync_be_stats *stitch_sync_stats);
typedef hi_s32 fn_vi_update_vi_vpss_mode(hi_vi_vpss_mode *vi_vpss_mode);
typedef hi_s32 fn_vi_get_pipe_snap_status(hi_vi_pipe vi_pipe, vi_snap_status *snap_status);
typedef hi_s32 fn_vi_get_split_attr(hi_vi_pipe vi_pipe, vi_pipe_split_attr *split_attr);
typedef hi_s32 fn_vi_get_proc_module_param(vi_proc_module_param *proc_module_param);
typedef hi_s32 fn_vi_down_semaphore(hi_vi_pipe vi_pipe);
typedef hi_s32 fn_vi_up_semaphore(hi_vi_pipe vi_pipe);
typedef hi_bool fn_vi_is_pipe_existed(hi_vi_pipe vi_pipe);
typedef hi_s32 fn_vi_get_pipe_bind_dev_size(hi_vi_pipe vi_pipe, hi_size *dev_size);
typedef hi_s32 fn_vi_get_pipe_nrx_status(hi_vi_pipe vi_pipe, hi_bool *nrx_en);
typedef hi_s32 fn_vi_set_vpss_frame_interrupt_attr(hi_vi_pipe vi_pipe, const hi_frame_interrupt_attr *frame_int_attr);
typedef hi_s32 fn_vi_get_vi_frame_interrupt_attr(hi_vi_pipe vi_pipe, hi_frame_interrupt_attr *frame_int_attr);
typedef hi_s32 fn_vi_isp_clear_input_queue(hi_vi_pipe vi_pipe);
typedef hi_s32 fn_vi_get_isp_config(hi_vi_pipe vi_pipe, isp_snap_info *snap_info);
typedef hi_s32 fn_vi_update_bayer_format(hi_vi_pipe vi_pipe, hi_isp_bayer_format bayer_format);
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
typedef hi_s32 fn_vi_set_pq_3dnr_ai_attr(hi_vi_pipe vi_pipe, hi_pq_ai_noiseness_post_attr *pq_ai_attr);
typedef hi_s32 fn_vi_get_pq_3dnr_ai_attr(hi_vi_pipe vi_pipe, hi_pq_ai_noiseness_post_attr *pq_ai_attr);
#endif
typedef struct {
    fn_vi_get_stitch_sync_be_stt_info *pfn_vi_get_stitch_sync_be_stt_info;
    fn_vi_get_pipe_stitch_attr *pfn_vi_get_pipe_stitch_attr;
    fn_vi_get_pipe_wdr_attr *pfn_vi_get_pipe_wdr_attr;
    fn_vi_get_pipe_hdr_attr *pfn_vi_get_pipe_hdr_attr;
    fn_vi_get_be_stt_info *pfn_vi_get_be_stt_info;
    fn_vi_register_flash_call_back *pfn_vi_register_flash_call_back;
    fn_vi_update_vi_vpss_mode *pfn_vi_update_vi_vpss_mode;
    fn_vi_get_pipe_snap_status *pfn_vi_get_pipe_snap_status;
    fn_vi_get_split_attr *pfn_vi_get_split_attr;
    fn_vi_get_proc_module_param *pfn_vi_get_proc_module_param;
    fn_vi_down_semaphore *pfn_vi_down_semaphore;
    fn_vi_up_semaphore *pfn_vi_up_semaphore;
    fn_vi_is_pipe_existed *pfn_vi_is_pipe_existed;
    fn_vi_get_pipe_bind_dev_size *pfn_vi_get_pipe_bind_dev_size;
    fn_vi_get_pipe_nrx_status *pfn_vi_get_pipe_nrx_status;
    fn_vi_set_vpss_frame_interrupt_attr *pfn_vi_set_vpss_frame_interrupt_attr;
    fn_vi_get_vi_frame_interrupt_attr *pfn_vi_get_vi_frame_interrupt_attr;
    fn_vi_isp_clear_input_queue *pfn_vi_isp_clear_input_queue;
    fn_vi_get_isp_config *pfn_vi_get_isp_config;
    fn_vi_update_bayer_format *pfn_vi_update_bayer_format;
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
    fn_vi_set_pq_3dnr_ai_attr *pfn_vi_set_pq_3dnr_ai_attr;
    fn_vi_get_pq_3dnr_ai_attr *pfn_vi_get_pq_3dnr_ai_attr;
#endif
} vi_export_func;

#define ckfn_vi_isp_clear_input_queue() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_isp_clear_input_queue != HI_NULL)
#define call_vi_isp_clear_input_queue(vi_pipe) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_isp_clear_input_queue(vi_pipe)

#define ckfn_vi_get_vi_frame_interrupt_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_vi_frame_interrupt_attr != HI_NULL)
#define call_vi_get_vi_frame_interrupt_attr(vi_pipe, frame_int_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_vi_frame_interrupt_attr(vi_pipe, frame_int_attr)

#define ckfn_vi_set_vpss_frame_interrupt_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_set_vpss_frame_interrupt_attr != HI_NULL)
#define call_vi_set_vpss_frame_interrupt_attr(vi_pipe, frame_int_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_set_vpss_frame_interrupt_attr(vi_pipe, frame_int_attr)

#define ckfn_vi_update_vi_vpss_mode() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_update_vi_vpss_mode != HI_NULL)
#define call_vi_update_vi_vpss_mode(vi_vpss_mode) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_update_vi_vpss_mode(vi_vpss_mode)

#define ckfn_vi_get_pipe_stitch_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_stitch_attr != HI_NULL)
#define call_vi_get_pipe_stitch_attr(vi_pipe, stitch_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_stitch_attr(vi_pipe, stitch_attr)

#define ckfn_vi_get_stitch_sync_be_stt_info() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_stitch_sync_be_stt_info != HI_NULL)
#define call_vi_get_stitch_sync_be_stt_info(vi_pipe, stitch_sync_stats) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_stitch_sync_be_stt_info(vi_pipe, stitch_sync_stats)

#define ckfn_vi_get_pipe_wdr_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_wdr_attr != HI_NULL)
#define call_vi_get_pipe_wdr_attr(vi_pipe, wdr_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_wdr_attr(vi_pipe, wdr_attr)

#define ckfn_vi_get_pipe_hdr_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_hdr_attr != HI_NULL)
#define call_vi_get_pipe_hdr_attr(vi_pipe, hdr_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_hdr_attr(vi_pipe, hdr_attr)

#define ckfn_vi_get_be_stt_info() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_be_stt_info != HI_NULL)
#define call_vi_get_be_stt_info(vi_pipe, block, be_stt) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_be_stt_info(vi_pipe, block, be_stt)

#define ckfn_vi_get_snap_status() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_snap_status != HI_NULL)
#define call_vi_get_snap_status(vi_pipe, vi_snap_status) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_snap_status(vi_pipe, vi_snap_status)

#define ckfn_vi_get_split_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_split_attr != HI_NULL)
#define call_vi_get_split_attr(vi_pipe, split_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_split_attr(vi_pipe, split_attr)

#define ckfn_vi_get_proc_module_param() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_proc_module_param != HI_NULL)
#define call_vi_get_proc_module_param(proc_module_param) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_proc_module_param(proc_module_param)

#define ckfn_vi_down_semaphore() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_down_semaphore != HI_NULL)
#define call_vi_down_semaphore(vi_pipe) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_down_semaphore(vi_pipe)

#define ckfn_vi_up_semaphore() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_up_semaphore != HI_NULL)
#define call_vi_up_semaphore(vi_pipe) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_up_semaphore(vi_pipe)

#define ckfn_vi_is_pipe_existed() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_is_pipe_existed != HI_NULL)
#define call_vi_is_pipe_existed(vi_pipe) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_is_pipe_existed(vi_pipe)

#define ckfn_vi_get_pipe_bind_dev_size() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_bind_dev_size != HI_NULL)
#define call_vi_get_pipe_bind_dev_size(vi_pipe, dev_size) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_bind_dev_size(vi_pipe, dev_size)

#define ckfn_vi_get_pipe_nrx_status() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_nrx_status != HI_NULL)
#define call_vi_get_pipe_nrx_status(vi_pipe, nrx_en) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pipe_nrx_status(vi_pipe, nrx_en)

#define ckfn_vi_get_isp_config() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_isp_config != HI_NULL)
#define call_vi_get_isp_config(vi_pipe, snap_info) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_isp_config(vi_pipe, snap_info)

#define ckfn_vi_update_bayer_format() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_update_bayer_format != HI_NULL)
#define call_vi_update_bayer_format(vi_pipe, bayer_format) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_update_bayer_format(vi_pipe, bayer_format)
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
#define ckfn_vi_set_pq_3dnr_ai_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_set_pq_3dnr_ai_attr != HI_NULL)
#define call_vi_set_pq_3dnr_ai_att(vi_pipe, ai_3dnr_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_set_pq_3dnr_ai_attr(vi_pipe, ai_3dnr_attr)

#define ckfn_vi_get_pq_3dnr_ai_attr() \
    (func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pq_3dnr_ai_attr != HI_NULL)
#define call_vi_get_pq_3dnr_ai_att(vi_pipe, ai_3dnr_attr) \
    func_entry(vi_export_func, HI_ID_VI)->pfn_vi_get_pq_3dnr_ai_attr(vi_pipe, ai_3dnr_attr)
#endif

typedef struct {
    hi_video_frame_info video_frame;
    hi_isp_config_info isp_config_info;
    hi_isp_dcf_update_info dcf_update_info;
    hi_u64 start_time;
    hi_u64 end_time;
    hi_u64 vicap_start_time;
    hi_u64 left_end_time;
} vi_cap_pic_info;

typedef struct {
    vi_cap_pic_info pic_info;
    hi_bool buf_ok;
    hi_bool cap_ok;
    hi_bool send_ok;
    hi_bool lost_repeat_frame;
} vi_buf_info;

typedef enum {
    FRAME_NORMAL = 0x0,
    FRAME_LOWDELAY,
    FRAME_LOWDELAY_FIN,
} vi_frame_info;

typedef enum {
    VI_FRAME_SOURCE_NORMAL = 0x0,
    VI_FRAME_SOURCE_USERPIC,
    VI_FRAME_SOURCE_BUTT,
} vi_frame_source_type;

typedef struct {
    hi_video_frame_info video_frame;
    hi_video_frame_info single_y_frame;
    hi_u64 start_time;
    hi_u64 end_time;
    vi_frame_info frame_info;
    vi_frame_source_type frame_source;
} vi_pic_info;

typedef enum {
    FPN_MODE_NONE = 0x0,
    FPN_MODE_CORRECTION,
    FPN_MODE_CALIBRATE,
    FPN_MODE_BUTT
} vi_fpn_work_mode;

typedef struct {
    vi_fpn_work_mode fpn_work_mode;
    union {
        hi_isp_fpn_calibrate_attr calibrate_attr;
        hi_isp_fpn_attr correction_attr;
    };
} vi_fpn_attr;

hi_s32 mpi_vi_set_fpn_attr(hi_vi_pipe vi_pipe, vi_fpn_attr *fpn_attr);
hi_s32 mpi_vi_get_fpn_attr(hi_vi_pipe vi_pipe, vi_fpn_attr *fpn_attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

