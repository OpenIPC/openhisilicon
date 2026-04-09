/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_ext.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_EXT_H__
#define __ISP_EXT_H__

#include "hi_type.h"
#include "hi_comm_isp_adapt.h"
#include "mod_ext.h"
#include "mkp_isp.h"
#include "hi_osal.h"
#include "isp_list.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_snap_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum {
    ISP_BUS_TYPE_I2C = 0,
    ISP_BUS_TYPE_SSP,

    ISP_BUS_TYPE_BUTT,
} isp_bus_type;

typedef struct {
    hi_u8  vc_num;
} isp_pro_ctrl;

typedef struct {
    hi_s32 (*pfn_isp_write_i2c_data)(hi_u8 i2c_dev, hi_u8 dev_addr, hi_u32 reg_addr,
                                     hi_u32 reg_addr_byte_num, hi_u32 data, hi_u32 data_byte_num);
    hi_s32 (*pfn_isp_write_ssp_data)(hi_u32 spi_dev, hi_u32 spi_csn,
                                     hi_u32 dev_addr, hi_u32 dev_addr_byte_num,
                                     hi_u32 reg_addr, hi_u32 reg_addr_byte_num, hi_u32 data, hi_u32 data_byte_num);
} isp_bus_callback;

typedef struct {
    hi_s32 (*pfn_piris_gpio_update)(hi_s32 vi_pipe, const hi_s32 *pos);
} isp_piris_callback;

typedef struct {
    hi_s32 (*pfn_isp_be_sta_read)(hi_s32 vi_pipe, isp_be_stat *be_stat);
    hi_s32 (*pfn_isp_be_cfg_read)(hi_s32 vi_pipe, isp_reg_cfg *reg_cfg_info);
} isp_vibus_callback;

/* AF */
#define DRV_AF_ZONE_ROW                15
#define DRV_AF_ZONE_COLUMN             17
#define DRV_WDR_CHN_MAX                4

typedef struct hiISP_DRV_FOCUS_ZONE_S {
    HI_U16  u16v1;
    HI_U16  u16h1;
    HI_U16  u16v2;
    HI_U16  u16h2;
    HI_U16  u16y;
    HI_U16  u16HlCnt;
} ISP_DRV_FOCUS_ZONE_S;

typedef struct hiISP_DRV_FE_FOCUS_STATISTICS_S {
    /* RO, The zoned measure of contrast */
    ISP_DRV_FOCUS_ZONE_S stZoneMetrics[DRV_WDR_CHN_MAX][DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN];
} ISP_DRV_FE_FOCUS_STATISTICS_S;

typedef struct hiISP_DRV_BE_FOCUS_STATISTICS_S {
    ISP_DRV_FOCUS_ZONE_S stZoneMetrics[DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN]; /* RO, The zoned measure of contrast */
} ISP_DRV_BE_FOCUS_STATISTICS_S;

typedef struct hiISP_DRV_AF_STATISTICS_S {
    ISP_DRV_FE_FOCUS_STATISTICS_S      stFEAFStat;
    ISP_DRV_BE_FOCUS_STATISTICS_S      stBEAFStat;
} ISP_DRV_AF_STATISTICS_S;

/* ---------------------------------------------------------------------------------------- */
/* isp sync task */
typedef enum hiISP_SYNC_TSK_METHOD_E {
    ISP_SYNC_TSK_METHOD_HW_IRQ = 0,
    ISP_SYNC_TSK_METHOD_WORKQUE,

    ISP_SYNC_TSK_METHOD_BUTT
} ISP_SYNC_TSK_METHOD_E;

typedef struct hiISP_SYNC_TASK_NODE_S {
    ISP_SYNC_TSK_METHOD_E enMethod;
    HI_S32 (*pfnIspSyncTskCallBack) (HI_U64 u64Data);
    HI_U64 u64Data;
    const char *pszId;
    struct osal_list_head list;
    ISP_DRV_AF_STATISTICS_S     *pstFocusStat;
} ISP_SYNC_TASK_NODE_S;

typedef struct {
    hi_s32 (*pfn_isp_register_bus_callback) (hi_s32 vi_pipe, isp_bus_type type, isp_bus_callback *bus_cb);
    hi_s32 (*pfn_isp_register_piris_callback) (hi_s32 vi_pipe, isp_piris_callback *piris_cb);
    hi_s32 (*pfn_isp_register_vi_bus_callback) (hi_s32 vi_pipe, isp_vibus_callback *vi_bus_cb);
    hi_s32 (*pfn_isp_get_dcf_info)(hi_vi_pipe vi_pipe, hi_isp_dcf_info *isp_dcf);
    hi_s32 (*pfn_isp_get_frame_info)(hi_vi_pipe vi_pipe, hi_isp_frame_info *isp_frame);
    hi_s32 (*pfn_isp_get_attach_info)(hi_vi_pipe vi_pipe, hi_isp_attach_info *isp_attach_info);
    hi_s32 (*pfn_isp_get_color_gamut_info)(hi_vi_pipe vi_pipe, hi_isp_colorgammut_info *isp_color_gamut_info);
    hi_s32 (*pfn_isp_get_dng_image_dynamic_info)(hi_vi_pipe vi_pipe, hi_dng_image_dynamic_info *dng_image_dynamic_info);
    hi_s32 (*pfn_isp_save_snap_config)(hi_vi_pipe vi_pipe, hi_isp_config_info *snap_info);
    hi_s32 (*pfn_isp_set_pro_enable)(hi_vi_pipe vi_pipe);
    hi_s32 (*pfn_isp_drv_get_ready_be_buf)(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
    hi_s32 (*pfn_isp_drv_put_free_be_buf)(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
    hi_s32 (*pfn_isp_drv_hold_busy_be_buf)(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
    hi_s32 (*pfn_isp_drv_get_be_sync_para)(hi_vi_pipe vi_pipe, isp_be_sync_para *be_sync_para);
    hi_bool (*pfn_isp_get_pro_ctrl)(hi_vi_pipe vi_pipe, isp_pro_ctrl *pro_ctrl);
    hi_s32 (*pfn_isp_set_snap_attr)(hi_vi_pipe vi_pipe, isp_snap_attr *snap_attr);
    hi_s32 (*pfn_isp_set_pro_nr_param)(hi_vi_pipe vi_pipe, const hi_isp_pro_bnr_param *pro_nr_param);
    hi_s32 (*pfn_isp_set_pro_shp_param)(hi_vi_pipe vi_pipe, const hi_isp_pro_sharpen_param *pro_shp_param);
    hi_s32 (*pfn_isp_get_pro_nr_param)(hi_vi_pipe vi_pipe, hi_isp_pro_bnr_param *pro_nr_param);
    hi_s32 (*pfn_isp_get_pro_shp_param)(hi_vi_pipe vi_pipe, hi_isp_pro_sharpen_param *pro_shp_param);
    hi_s32 (*pfn_isp_drv_be_end_int_proc)(hi_vi_pipe vi_pipe);
    hi_s32 (*pfn_isp_register_sync_task)(hi_vi_pipe vi_pipe, ISP_SYNC_TASK_NODE_S *new_node);
    hi_s32 (*pfn_isp_unregister_sync_task)(hi_vi_pipe vi_pipe, ISP_SYNC_TASK_NODE_S *del_node);
    hi_s32 (*pfn_isp_int_bottom_half)(hi_s32 irq, void *id);
    hi_s32 (*pfn_isp_isr)(hi_s32 irq, void *id);
    hi_s32 (*pfn_isp_get_pub_attr)(hi_vi_pipe vi_pipe, hi_isp_pub_attr *pub_attr);
    hi_s32 (*pfn_isp_drv_get_rgbir_format)(hi_vi_pipe vi_pipe, hi_isp_ir_bayer_format *rgbir_format);
    hi_s32 (*pfn_isp_drv_set_online_statistics_stt_addr)(hi_vi_pipe vi_pipe, hi_u32 viproc_irq_status);
    hi_s32 (*pfn_isp_get_pq_ai_info)(hi_vi_pipe vi_pipe, pq_ai_attr *pq_info);
} isp_export_func;

#define ckfn_isp() \
    (func_entry(isp_export_func, HI_ID_ISP) != HI_NULL)

#define ckfn_isp_register_bus_callback()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_bus_callback != HI_NULL)
#define call_isp_register_bus_callback(vi_pipe, type, bus_cb)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_bus_callback(vi_pipe, type, bus_cb)

#define ckfn_isp_register_piris_callback()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_piris_callback != HI_NULL)
#define call_isp_register_piris_callback(vi_pipe, piris_cb)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_piris_callback(vi_pipe, piris_cb)

#define ckfn_isp_register_vi_bus_callback()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_vi_bus_callback != HI_NULL)
#define call_isp_register_vi_bus_callback(vi_pipe, vi_bus_cb)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_vi_bus_callback(vi_pipe, vi_bus_cb)

#define ckfn_isp_get_dcf_info()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_dcf_info != HI_NULL)
#define call_isp_get_dcf_info(vi_pipe, isp_dcf)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_dcf_info(vi_pipe, isp_dcf)

#define ckfn_isp_get_frame_info()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_frame_info != HI_NULL)
#define call_isp_get_frame_info(vi_pipe, isp_frame)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_frame_info(vi_pipe, isp_frame)

#define ckfn_isp_get_attach_info()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_attach_info != HI_NULL)
#define call_isp_get_attach_info(vi_pipe, isp_attach_info)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_attach_info(vi_pipe, isp_attach_info)

#define ckfn_isp_get_color_gamut_info()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_color_gamut_info != HI_NULL)
#define call_isp_get_color_gamut_info(vi_pipe, isp_color_gamut_info)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_color_gamut_info(vi_pipe, isp_color_gamut_info)

#define ckfn_isp_get_dng_image_dynamic_info()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_dng_image_dynamic_info != HI_NULL)
#define call_isp_get_dng_image_dynamic_info(vi_pipe, dng_image_dynamic_info)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_dng_image_dynamic_info(vi_pipe, dng_image_dynamic_info)

#define ckfn_isp_save_snap_config()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_save_snap_config != HI_NULL)
#define call_isp_save_snap_config(vi_pipe, snap_info)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_save_snap_config(vi_pipe, snap_info)

#define ckfn_isp_set_pro_enable()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_pro_enable != HI_NULL)
#define call_isp_set_pro_enable(vi_pipe)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_pro_enable(vi_pipe)

#define ckfn_isp_drv_get_ready_be_buf()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_get_ready_be_buf != HI_NULL)
#define call_isp_drv_get_ready_be_buf(vi_pipe, be_cfg_buf)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_get_ready_be_buf(vi_pipe, be_cfg_buf)

#define ckfn_isp_drv_put_free_be_buf()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_put_free_be_buf != HI_NULL)
#define call_isp_drv_put_free_be_buf(vi_pipe, be_cfg_buf)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_put_free_be_buf(vi_pipe, be_cfg_buf)

#define ckfn_isp_drv_hold_busy_be_buf()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_hold_busy_be_buf != HI_NULL)
#define call_isp_drv_hold_busy_be_buf(vi_pipe, be_cfg_buf)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_hold_busy_be_buf(vi_pipe, be_cfg_buf)

#define ckfn_isp_drv_get_be_sync_para()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_get_be_sync_para != HI_NULL)
#define call_isp_drv_get_be_sync_para(vi_pipe, be_sync_para)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_get_be_sync_para(vi_pipe, be_sync_para)

#define ckfn_isp_get_pro_ctrl()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pro_ctrl != HI_NULL)
#define call_isp_get_pro_ctrl(vi_pipe, pro_ctrl)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pro_ctrl(vi_pipe, pro_ctrl)

#define ckfn_isp_set_snap_attr()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_snap_attr != HI_NULL)
#define call_isp_set_snap_attr(vi_pipe, snap_attr)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_snap_attr(vi_pipe, snap_attr)

#define ckfn_isp_set_pro_nr_param()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_pro_nr_param != HI_NULL)
#define call_isp_set_pro_nr_param(vi_pipe, pro_nr_param)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_pro_nr_param(vi_pipe, pro_nr_param)
#define ckfn_isp_set_pro_shp_param()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_pro_shp_param != HI_NULL)
#define call_isp_set_pro_shp_param(vi_pipe, pro_shp_param)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_set_pro_shp_param(vi_pipe, pro_shp_param)
#define ckfn_isp_get_pro_nr_param()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pro_nr_param != HI_NULL)
#define call_isp_get_pro_nr_param(vi_pipe, pro_nr_param)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pro_nr_param(vi_pipe, pro_nr_param)
#define ckfn_isp_get_pro_shp_param()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pro_shp_param != HI_NULL)
#define call_isp_get_pro_shp_param(vi_pipe, pro_shp_param)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pro_shp_param(vi_pipe, pro_shp_param)
#define ckfn_isp_drv_be_end_int_proc()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_be_end_int_proc != HI_NULL)
#define call_isp_drv_be_end_int_proc(vi_pipe)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_be_end_int_proc(vi_pipe)

#define ckfn_isp_get_pub_attr()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pub_attr != HI_NULL)
#define call_isp_get_pub_attr(vi_pipe, pub_attr)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pub_attr(vi_pipe, pub_attr)

#define CKFN_ISP_RegisterSyncTask()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_sync_task != HI_NULL)
#define CALL_ISP_RegisterSyncTask(vi_pipe, node)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_register_sync_task(vi_pipe, node)

#define CKFN_ISP_UnRegisterSyncTask()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_unregister_sync_task != HI_NULL)
#define CALL_ISP_UnRegisterSyncTask(vi_pipe, node)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_unregister_sync_task(vi_pipe, node)

#define ckfn_isp_int_bottom_half()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_int_bottom_half != HI_NULL)
#define call_isp_int_bottom_half(irq, id)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_int_bottom_half(irq, id)

#define ckfn_isp_isr()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_isr != HI_NULL)
#define call_isp_isr(irq, id)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_isr(irq, id)

#define ckfn_isp_get_rgbir_format()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_get_rgbir_format != HI_NULL)
#define call_isp_get_rgbir_format(vi_pipe, rgbir_format)                                \
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_get_rgbir_format(vi_pipe, rgbir_format)


#define ckfn_isp_drv_set_online_statistics_stt_addr() \
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_set_online_statistics_stt_addr != HI_NULL)
#define call_isp_drv_set_online_statistics_stt_addr(vi_pipe, proc_irq_status)                                \
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_drv_set_online_statistics_stt_addr(vi_pipe, proc_irq_status)

#define ckfn_isp_get_pq_ai_info()\
    (func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pq_ai_info != HI_NULL)
#define call_isp_get_pq_ai_info(vi_pipe, ai_info)\
    func_entry(isp_export_func, HI_ID_ISP)->pfn_isp_get_pq_ai_info(vi_pipe, ai_info)

#define HI_ISP_SYNC_TASK_ID_MAX_LENGTH 64
hi_s32 hi_isp_sync_task_register(VI_PIPE vi_pipe, ISP_SYNC_TASK_NODE_S *new_node);
hi_s32 hi_isp_sync_task_unregister(VI_PIPE vi_pipe, ISP_SYNC_TASK_NODE_S *del_node);

hi_s32 isp_set_smart_info(hi_vi_pipe vi_pipe, hi_isp_smart_info *smart_info);
hi_s32 isp_get_smart_info(hi_vi_pipe vi_pipe, hi_isp_smart_info *smart_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
