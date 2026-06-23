/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef ISP_EXT_H
#define ISP_EXT_H

#include "ot_type.h"
#include "ot_common_isp.h"
#include "ot_common_vi.h"
#include "mod_ext.h"
#include "mkp_isp.h"
#include "ot_osal.h"
#include "isp_list.h"
#include "ot_common_video.h"

#ifndef __LITEOS__
#define ISP_WRITE_I2C_THROUGH_MUL_REG
#define ISP_WRITE_I2C_MAX_REG_NUM        60
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum {
    ISP_BE_FORMAT_YUV420,
    ISP_BE_FORMAT_YUV422,
    ISP_BE_FORMAT_RAW,
    ISP_BE_FORMAT_BUTT
} isp_be_format;

typedef enum {
    ISP_BUS_TYPE_I2C = 0,
    ISP_BUS_TYPE_SSP,

    ISP_BUS_TYPE_BUTT,
} isp_bus_type;

typedef struct {
    td_u8  vc_num;
} isp_pro_ctrl;

typedef struct {
#ifndef ISP_WRITE_I2C_THROUGH_MUL_REG
    td_s32 (*pfn_isp_write_i2c_data)(td_u8 i2c_dev, td_u8 dev_addr, td_u32 reg_addr,
                                     td_u32 reg_addr_byte_num, td_u32 data, td_u32 data_byte_num);
#else
    td_s32 (*pfn_isp_write_i2c_data)(td_u8 i2c_dev, td_u8 dev_addr, td_s8 *data_buf, td_u8 data_buf_len, td_u8 data_len,
                                     td_u32 reg_addr_byte_num, td_u32 data_byte_num);
#endif
    td_s32 (*pfn_isp_write_ssp_data)(td_u32 spi_dev, td_u32 spi_csn,
                                     td_u32 dev_addr, td_u32 dev_addr_byte_num,
                                     td_u32 reg_addr, td_u32 reg_addr_byte_num, td_u32 data, td_u32 data_byte_num);
} isp_bus_callback;

typedef struct {
    td_s32 (*pfn_piris_gpio_update)(td_s32 vi_pipe, const td_s32 *pos);
} isp_piris_callback;

/* AF */
#define DRV_AF_ZONE_ROW                15
#define DRV_AF_ZONE_COLUMN             17
#define DRV_WDR_CHN_MAX                4

typedef struct {
    td_u16  v1;
    td_u16  h1;
    td_u16  v2;
    td_u16  h2;
    td_u16  y;
    td_u16  hl_cnt;
} ot_isp_drv_focus_zone;

typedef struct {
    /* RO, The zoned measure of contrast */
    ot_isp_drv_focus_zone zone_metrics[DRV_WDR_CHN_MAX][DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN];
} ot_isp_drv_fe_focus_statistics;

typedef struct {
    ot_isp_drv_focus_zone zone_metrics[DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN]; /* RO, The zoned measure of contrast */
} ot_isp_drv_be_focus_statistics;

typedef struct {
    ot_isp_drv_fe_focus_statistics      fe_af_stat;
    ot_isp_drv_be_focus_statistics      be_af_stat;
} ot_isp_drv_af_statistics;

/* ---------------------------------------------------------------------------------------- */
/* isp sync task */
typedef enum {
    ISP_SYNC_TSK_METHOD_HW_IRQ = 0,
    ISP_SYNC_TSK_METHOD_WORKQUE,

    ISP_SYNC_TSK_METHOD_BUTT
} ot_isp_sync_tsk_method;

typedef struct {
    ot_isp_sync_tsk_method method;
    td_s32 (*isp_sync_tsk_callback)(td_u64 data);
    td_u64 data;
    const char *sz_id;
    struct osal_list_head list;
    ot_isp_drv_af_statistics *focus_stat;
    ot_isp_ae_stats *ae_stat;
} ot_isp_sync_task_node;

/* offline mode isp sync para */
typedef struct {
    td_bool correct_en;
    td_u16  offset;
    td_u32  isp_fpn_frame_calibrate_shift;
    td_u32  isp_fpn_out_shift;
    td_u32  isp_fpn_in_shift;
    td_u32  isp_fpn_shift;
    td_u32  isp_fpn_max;
} isp_fpn_cor_reg_cfg;

typedef struct {
    td_bool aibnr_en;
    td_bool blend_en;
    td_bool bnr_bypass;
    td_bool dgain_bypass;
} isp_aibnr_cfg;

typedef enum {
    ISP_AIISP_PENDING_STAT_IDLE = 0,
    ISP_AIISP_PENDING_STAT_ENABLE,
    ISP_AIISP_PENDING_STAT_DISABLE,
    ISP_AIISP_PENDING_STAT_BUTT
} isp_aiisp_pending_stat;

/* AIDRC */
typedef enum {
    ISP_AIDRC_MODE_NORMAL = 0x0,
    ISP_AIDRC_MODE_ADVANCED,
    ISP_AIDRC_MODE_BUTT
} isp_aidrc_mode;

typedef struct {
    td_bool aidrc_en;
    isp_aidrc_mode mode;
    isp_aiisp_pending_stat pending_stat;
    td_u32 pending_idx;
} isp_aidrc_cfg;

typedef enum {
    ISP_ALG_IN_PRE = 0,
    ISP_ALG_IN_POST = 1,
    ISP_ALG_BUTT
} isp_alg_in_pre_post;

typedef struct {
    union {
        struct {
            td_u32  bit_ae     :  1; /* [0]  */
            td_u32  bit_awb    :  1; /* [1]  */
            td_u32  bit_af     :  1; /* [2]  */
            td_u32  bit_la     :  1; /* [3]  */
            td_u32  bit_drc    :  1; /* [4]  */
            td_u32  bit_rsv  :  27; /* [5:31]  */
        } bits;
        td_u32 pos;
    } alg_pos;
    td_u32 stat_valid; // isp_be_stat_valid stat_valid;
    u_isp_viproc_ispbe_ctrl0 be_ctrl0;
    u_isp_viproc_ispbe_ctrl1 be_ctrl1;
} isp_ai_alg_cfg;

typedef struct {
    ot_vi_aiisp_mode aiisp_mode;
    td_u32 be_mask;
    td_u32 split_index;
    isp_be_wo_cfg_buf *be_cfg_buf_pre;
    isp_be_wo_cfg_buf *be_cfg_buf_post;
} isp_vi_ai_work_param;
#ifdef CONFIG_OT_ISP_DETAIL_STATS_SUPPORT
typedef struct {
    ot_isp_detail_stats_cfg stats_cfg;
    size_t stt_offset;
    size_t stt_size;
} isp_vi_detail_stats_cfg;
#endif

typedef struct {
    td_s32 (*pfn_isp_register_bus_callback) (td_s32 vi_pipe, isp_bus_type type, isp_bus_callback *bus_cb);
    td_s32 (*pfn_isp_register_piris_callback) (td_s32 vi_pipe, isp_piris_callback *piris_cb);
    td_s32 (*pfn_isp_get_dcf_info)(ot_vi_pipe vi_pipe, ot_isp_dcf_info *isp_dcf);
    td_s32 (*pfn_isp_get_frame_info)(ot_vi_pipe vi_pipe, ot_isp_frame_info *isp_frame);
    td_s32 (*pfn_isp_get_attach_info)(ot_vi_pipe vi_pipe, ot_isp_attach_info *isp_attach_info);
    td_s32 (*pfn_isp_get_color_gamut_info)(ot_vi_pipe vi_pipe, ot_isp_colorgammut_info *isp_color_gamut_info);
    td_s32 (*pfn_isp_get_dng_image_dynamic_info)(ot_vi_pipe vi_pipe, ot_dng_image_dynamic_info *dng_image_dynamic_info);

    td_s32 (*pfn_isp_drv_get_be_offline_addr_info)(ot_vi_pipe vi_pipe, isp_be_offline_addr_info *be_offline_addr);
    td_s32 (*pfn_isp_drv_get_ready_be_buf)(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
    td_s32 (*pfn_isp_drv_put_free_be_buf)(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
    td_s32 (*pfn_isp_drv_hold_busy_be_buf)(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
    td_s32 (*pfn_isp_drv_get_be_sync_para)(ot_vi_pipe vi_pipe, const ot_video_frame_info *frame_info,
        isp_be_sync_para *be_sync_para);
    td_s32 (*pfn_isp_drv_set_be_sync_para)(ot_vi_pipe vi_pipe, void *be_node, isp_be_sync_para *be_sync_para,
        isp_aibnr_cfg *ainr_cfg);
    td_s32 (*pfn_isp_drv_set_be_format)(ot_vi_pipe vi_pipe, void *be_node, isp_be_format be_format);
#ifdef CONFIG_OT_SNAP_SUPPORT
    td_s32 (*pfn_isp_set_pro_enable)(ot_vi_pipe vi_pipe);
    td_s32 (*pfn_isp_set_snap_attr)(ot_vi_pipe vi_pipe, const isp_snap_attr *snap_attr);
#endif
    td_s32 (*pfn_isp_drv_be_end_int_proc)(ot_vi_pipe vi_pipe);
    td_s32 (*pfn_isp_register_sync_task)(ot_vi_pipe vi_pipe, ot_isp_sync_task_node *new_node);
    td_s32 (*pfn_isp_unregister_sync_task)(ot_vi_pipe vi_pipe, ot_isp_sync_task_node *del_node);
    td_s32 (*pfn_isp_int_bottom_half)(td_s32 irq, void *id);
    td_s32 (*pfn_isp_isr)(td_s32 irq, void *id, td_u32 vicap_int_status);
    td_s32 (*pfn_isp_get_pub_attr)(ot_vi_pipe vi_pipe, ot_isp_pub_attr *pub_attr);
    td_s32 (*pfn_isp_drv_get_fpn_sum)(ot_vi_pipe vi_pipe, td_u64 *fpn_sum, td_phys_addr_t phy_addr);
    td_s32 (*pfn_isp_drv_set_online_statistics_stt_addr)(ot_vi_pipe vi_pipe, td_u32 viproc_irq_status);
    td_s32 (*pfn_isp_drv_set_online_reg_be)(ot_vi_pipe vi_pipe, td_u64 viproc_irq_status);
    td_s32 (*pfn_isp_drv_switch_be_offline_stt_info)(ot_vi_pipe vi_pipe, td_u32 viproc_id,
                                                    td_u32 stat_valid); /* viproc frame end */
    td_s32 (*pfn_isp_drv_get_be_offline_stt_addr)(ot_vi_pipe vi_pipe, td_u32 viproc_id, td_u64 pts,
                                                  isp_be_offline_stt_buf *be_offline_stt_addr); /* set isp stt addr */
    td_bool (*pfn_isp_drv_is_stitch_sync_lost_frame)(ot_vi_pipe vi_pipe, td_u32 viproc_id);
    td_bool (*pfn_isp_drv_get_wbf_en)(ot_vi_pipe vi_pipe);
#ifdef CONFIG_OT_AIBNR_SUPPORT
    td_s32 (*pfn_isp_drv_update_aibnr_be_cfg)(ot_vi_pipe vi_pipe, td_void *be_node, isp_aibnr_cfg *ainr_cfg);
#endif
#ifdef CONFIG_OT_AIDRC_SUPPORT
    td_s32 (*pfn_isp_drv_update_aidrc_be_cfg)(ot_vi_pipe vi_pipe, td_void *be_node, isp_aidrc_cfg *cfg);
#endif
    td_s32 (*pfn_isp_update_pts)(ot_vi_pipe vi_pipe, td_u64 pts);
    td_s32 (*pfn_isp_update_fe_pts)(ot_vi_pipe vi_pipe, td_u64 pts);
    td_s32 (*pfn_isp_drv_vi_set_frame_info)(ot_vi_pipe vi_pipe, ot_isp_frame_info *isp_frame);
    td_s32 (*pfn_isp_drv_get_sync_id)(ot_vi_pipe vi_pipe, const ot_video_frame_info *frame_info, td_bool current_frm,
                                      td_bool before_sns_cfg);
    td_s32 (*pfn_isp_drv_set_aibnr_fpn_cor_cfg)(ot_vi_pipe vi_pipe, isp_fpn_cor_reg_cfg *fpn_cor_cfg);
    td_s32 (*pfn_isp_drv_get_ai_be_cfg)(ot_vi_pipe vi_pipe, isp_vi_ai_work_param *param, isp_ai_alg_cfg *alg_cfg);
    td_s32 (*pfn_isp_drv_set_vicap_int_mask)(ot_vi_pipe vi_pipe, td_bool int0_mask_en);
#ifdef CONFIG_OT_ISP_DETAIL_STATS_SUPPORT
    td_s32 (*pfn_isp_drv_get_detail_stats_cfg)(ot_vi_pipe vi_pipe, isp_vi_detail_stats_cfg *cfg);
    td_s32 (*pfn_isp_drv_get_detail_stats_alg_ctrl)(ot_vi_pipe vi_pipe, u_isp_viproc_ispbe_ctrl0 *be_ctrl0,
        u_isp_viproc_ispbe_ctrl1 *be_ctrl1);
#endif
} isp_export_func;

#define ckfn_isp() \
    (func_entry(isp_export_func, OT_ID_ISP) != TD_NULL)

#define ckfn_isp_register_bus_callback() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_register_bus_callback != TD_NULL)
#define call_isp_register_bus_callback(vi_pipe, type, bus_cb) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_register_bus_callback(vi_pipe, type, bus_cb)

#define ckfn_isp_register_piris_callback() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_register_piris_callback != TD_NULL)
#define call_isp_register_piris_callback(vi_pipe, piris_cb) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_register_piris_callback(vi_pipe, piris_cb)


#define ckfn_isp_get_dcf_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_dcf_info != TD_NULL)
#define call_isp_get_dcf_info(vi_pipe, isp_dcf) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_dcf_info(vi_pipe, isp_dcf)

#define ckfn_isp_get_frame_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_frame_info != TD_NULL)
#define call_isp_get_frame_info(vi_pipe, isp_frame) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_frame_info(vi_pipe, isp_frame)

#define ckfn_isp_get_attach_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_attach_info != TD_NULL)
#define call_isp_get_attach_info(vi_pipe, isp_attach_info) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_attach_info(vi_pipe, isp_attach_info)

#define ckfn_isp_get_color_gamut_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_color_gamut_info != TD_NULL)
#define call_isp_get_color_gamut_info(vi_pipe, isp_color_gamut_info) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_color_gamut_info(vi_pipe, isp_color_gamut_info)

#define ckfn_isp_get_dng_image_dynamic_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_dng_image_dynamic_info != TD_NULL)
#define call_isp_get_dng_image_dynamic_info(vi_pipe, dng_image_dynamic_info) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_dng_image_dynamic_info(vi_pipe, dng_image_dynamic_info)

#define ckfn_isp_set_pro_enable() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_pro_enable != TD_NULL)
#define call_isp_set_pro_enable(vi_pipe) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_pro_enable(vi_pipe)

#define ckfn_isp_drv_get_be_offline_addr_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_be_offline_addr_info != TD_NULL)
#define call_isp_drv_get_be_offline_addr_info(vi_pipe, be_offline_addr) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_be_offline_addr_info(vi_pipe, be_offline_addr)

#define ckfn_isp_drv_get_ready_be_buf() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_ready_be_buf != TD_NULL)
#define call_isp_drv_get_ready_be_buf(vi_pipe, be_cfg_buf) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_ready_be_buf(vi_pipe, be_cfg_buf)

#define ckfn_isp_drv_put_free_be_buf() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_put_free_be_buf != TD_NULL)
#define call_isp_drv_put_free_be_buf(vi_pipe, be_cfg_buf) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_put_free_be_buf(vi_pipe, be_cfg_buf)

#define ckfn_isp_drv_hold_busy_be_buf() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_hold_busy_be_buf != TD_NULL)
#define call_isp_drv_hold_busy_be_buf(vi_pipe, be_cfg_buf) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_hold_busy_be_buf(vi_pipe, be_cfg_buf)

#define ckfn_isp_drv_get_be_sync_para() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_be_sync_para != TD_NULL)
#define call_isp_drv_get_be_sync_para(vi_pipe, frame_info, be_sync_para) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_be_sync_para(vi_pipe, frame_info, be_sync_para)

#define ckfn_isp_drv_set_be_sync_para() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_be_sync_para != TD_NULL)
#define call_isp_drv_set_be_sync_para(vi_pipe, be_node, be_sync_para, aibnr_cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_be_sync_para(vi_pipe, be_node, be_sync_para, aibnr_cfg)

#define ckfn_isp_drv_set_be_format() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_be_format != TD_NULL)
#define call_isp_drv_set_be_format(vi_pipe, be_node, be_format) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_be_format(vi_pipe, be_node, be_format)

#define ckfn_isp_set_snap_attr() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_snap_attr != TD_NULL)
#define call_isp_set_snap_attr(vi_pipe, snap_attr) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_snap_attr(vi_pipe, snap_attr)

#define ckfn_isp_set_pro_nr_param() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_pro_nr_param != TD_NULL)
#define call_isp_set_pro_nr_param(vi_pipe, pro_nr_param) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_pro_nr_param(vi_pipe, pro_nr_param)
#define ckfn_isp_set_pro_shp_param() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_pro_shp_param != TD_NULL)
#define call_isp_set_pro_shp_param(vi_pipe, pro_shp_param) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_set_pro_shp_param(vi_pipe, pro_shp_param)
#define ckfn_isp_get_pro_nr_param() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_pro_nr_param != TD_NULL)
#define call_isp_get_pro_nr_param(vi_pipe, pro_nr_param) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_pro_nr_param(vi_pipe, pro_nr_param)
#define ckfn_isp_get_pro_shp_param() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_pro_shp_param != TD_NULL)
#define call_isp_get_pro_shp_param(vi_pipe, pro_shp_param) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_pro_shp_param(vi_pipe, pro_shp_param)
#define ckfn_isp_drv_be_end_int_proc() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_be_end_int_proc != TD_NULL)
#define call_isp_drv_be_end_int_proc(vi_pipe) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_be_end_int_proc(vi_pipe)

#define ckfn_isp_get_pub_attr() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_pub_attr != TD_NULL)
#define call_isp_get_pub_attr(vi_pipe, pub_attr) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_get_pub_attr(vi_pipe, pub_attr)

#define ckfn_isp_register_sync_task() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_register_sync_task != TD_NULL)
#define call_isp_register_sync_task(vi_pipe, node) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_register_sync_task(vi_pipe, node)

#define ckfn_isp_unregister_sync_task() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_unregister_sync_task != TD_NULL)
#define call_isp_unregister_sync_task(vi_pipe, node) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_unregister_sync_task(vi_pipe, node)

#define ckfn_isp_int_bottom_half() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_int_bottom_half != TD_NULL)
#define call_isp_int_bottom_half(irq, id) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_int_bottom_half(irq, id)

#define ckfn_isp_isr() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_isr != TD_NULL)
#define call_isp_isr(irq, id, vicap_int_status) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_isr(irq, id, vicap_int_status)

#define ckfn_isp_get_fpn_sum() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_fpn_sum != TD_NULL)
#define call_isp_get_fpn_sum(vi_pipe, fpn_sum, phys_addr)               \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_fpn_sum(vi_pipe, fpn_sum, phys_addr)

#define ckfn_isp_drv_set_online_statistics_stt_addr() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_online_statistics_stt_addr != TD_NULL)
#define call_isp_drv_set_online_statistics_stt_addr(vi_pipe, proc_irq_status) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_online_statistics_stt_addr(vi_pipe, proc_irq_status)

#define ckfn_isp_drv_set_online_reg_be() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_online_reg_be != TD_NULL)
#define call_isp_drv_set_online_reg_be(vi_pipe, proc_irq_status) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_online_reg_be(vi_pipe, proc_irq_status)

#define ckfn_isp_drv_switch_be_stt_info() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_switch_be_offline_stt_info != TD_NULL)
#define call_isp_drv_switch_be_stt_info(vi_pipe, viproc_id, stat_valid) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_switch_be_offline_stt_info(vi_pipe, viproc_id, stat_valid)

#define ckfn_isp_drv_get_be_offline_stt_addr() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_be_offline_stt_addr != TD_NULL)
#define call_isp_drv_get_be_offline_stt_addr(vi_pipe, viproc_id, pts, stt_addr) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_be_offline_stt_addr(vi_pipe, viproc_id, pts, stt_addr)

#define ckfn_isp_drv_is_stitch_sync_lost_frame() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_is_stitch_sync_lost_frame != TD_NULL)
#define call_isp_drv_is_stitch_sync_lost_frame(vi_pipe, viproc_id) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_is_stitch_sync_lost_frame(vi_pipe, viproc_id)

#define ckfn_isp_drv_get_wbf_en() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_wbf_en != TD_NULL)
#define call_isp_drv_get_wbf_en(vi_pipe) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_wbf_en(vi_pipe)
#ifdef CONFIG_OT_AIBNR_SUPPORT
#define ckfn_isp_drv_update_aibnr_be_cfg() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_update_aibnr_be_cfg != TD_NULL)
#define call_isp_drv_update_aibnr_be_cfg(vi_pipe, be_node, hnr_cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_update_aibnr_be_cfg(vi_pipe, be_node, hnr_cfg)
#endif
#ifdef CONFIG_OT_AIDRC_SUPPORT
#define ckfn_isp_drv_update_aidrc_be_cfg() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_update_aidrc_be_cfg != TD_NULL)
#define call_isp_drv_update_aidrc_be_cfg(vi_pipe, be_node, cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_update_aidrc_be_cfg(vi_pipe, be_node, cfg)
#endif

#define ckfn_isp_drv_set_aibnr_fpn_cor_cfg() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_aibnr_fpn_cor_cfg != TD_NULL)
#define call_isp_drv_set_aibnr_fpn_cor_cfg(vi_pipe, fpn_cor_cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_aibnr_fpn_cor_cfg(vi_pipe, fpn_cor_cfg)

#define OT_ISP_SYNC_TASK_ID_MAX_LENGTH 64

#define ckfn_isp_update_frame_pts() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_update_pts != TD_NULL)
#define call_isp_update_frame_pts(vi_pipe, pts) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_update_pts(vi_pipe, pts)

#define ckfn_isp_update_frame_fe_pts() \
        (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_update_fe_pts != TD_NULL)
#define call_isp_update_frame_fe_pts(vi_pipe, pts) \
        func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_update_fe_pts(vi_pipe, pts)
#define ckfn_isp_drv_vi_set_frame_info() \
        (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_vi_set_frame_info != TD_NULL)
#define call_isp_drv_vi_set_frame_info(vi_pipe, isp_frame) \
        func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_vi_set_frame_info(vi_pipe, isp_frame)


#define ckfn_isp_drv_get_sync_id() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_sync_id != TD_NULL)
#define call_isp_drv_get_sync_id(vi_pipe, frame_info, is_current_frm, before_sns_cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_sync_id(vi_pipe, frame_info, is_current_frm, before_sns_cfg)

#define ckfn_isp_drv_get_ai_be_cfg_pos() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_ai_be_cfg != TD_NULL)
#define call_isp_drv_get_ai_be_cfg_pos(vi_pipe, work_mode, alg_ctrl_cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_ai_be_cfg(vi_pipe, work_mode, alg_ctrl_cfg)

#define ckfn_isp_drv_set_vicap_int_mask() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_vicap_int_mask != TD_NULL)
#define call_isp_drv_set_vicap_int_mask(vi_pipe, int0_mask_en) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_set_vicap_int_mask(vi_pipe, int0_mask_en)
#ifdef CONFIG_OT_ISP_DETAIL_STATS_SUPPORT
#define ckfn_isp_drv_get_detail_stats_cfg() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_detail_stats_cfg != TD_NULL)
#define call_isp_drv_get_detail_stats_cfg(vi_pipe, detail_stats_cfg) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_detail_stats_cfg(vi_pipe, detail_stats_cfg)

#define ckfn_isp_drv_get_detail_stats_alg_ctrl() \
    (func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_detail_stats_alg_ctrl != TD_NULL)
#define call_isp_drv_get_detail_stats_alg_ctrl(vi_pipe, be_ctrl0, be_ctrl1) \
    func_entry(isp_export_func, OT_ID_ISP)->pfn_isp_drv_get_detail_stats_alg_ctrl(vi_pipe, be_ctrl0, be_ctrl1)
#endif
td_s32 ot_isp_sync_task_register(ot_vi_pipe vi_pipe, ot_isp_sync_task_node *new_node);
td_s32 ot_isp_sync_task_unregister(ot_vi_pipe vi_pipe, ot_isp_sync_task_node *del_node);

td_s32 isp_set_smart_info(ot_vi_pipe vi_pipe, const ot_isp_smart_info *smart_info);
td_s32 isp_get_smart_info(ot_vi_pipe vi_pipe, ot_isp_smart_info *smart_info);

td_void *isp_get_export_func(td_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
