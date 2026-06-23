/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_VO_DEV_H
#define SS_MPI_VO_DEV_H

#include "ot_common_vo_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_vo_enable(ot_vo_dev dev);
td_s32 ss_mpi_vo_disable(ot_vo_dev dev);
td_s32 ss_mpi_vo_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr);
td_s32 ss_mpi_vo_get_pub_attr(ot_vo_dev dev, ot_vo_pub_attr *pub_attr);
td_s32 ss_mpi_vo_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param);
td_s32 ss_mpi_vo_get_dev_param(ot_vo_dev dev, ot_vo_dev_param *dev_param);
td_s32 ss_mpi_vo_set_dev_frame_rate(ot_vo_dev dev, td_u32 frame_rate);
td_s32 ss_mpi_vo_get_dev_frame_rate(ot_vo_dev dev, td_u32 *frame_rate);
td_s32 ss_mpi_vo_set_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info);
td_s32 ss_mpi_vo_get_user_sync_info(ot_vo_dev dev, ot_vo_user_sync_info *sync_info);
td_s32 ss_mpi_vo_set_mod_param(const ot_vo_mod_param *mod_param);
td_s32 ss_mpi_vo_get_mod_param(ot_vo_mod_param *mod_param);
td_s32 ss_mpi_vo_set_vtth(ot_vo_dev dev, td_u32 vtth);
td_s32 ss_mpi_vo_get_vtth(ot_vo_dev dev, td_u32 *vtth);
td_s32 ss_mpi_vo_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr);
td_s32 ss_mpi_vo_get_less_buf_attr(ot_vo_dev dev, ot_vo_less_buf_attr *less_buf_attr);
td_s32 ss_mpi_vo_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr);
td_s32 ss_mpi_vo_get_user_notify_attr(ot_vo_dev dev, ot_vo_user_notify_attr *user_notify_attr);

td_s32 ss_mpi_vo_query_intf_status(ot_vo_intf_type intf_type, ot_vo_intf_status *status);
td_s32 ss_mpi_vo_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param);
td_s32 ss_mpi_vo_get_vga_param(ot_vo_dev dev, ot_vo_vga_param *vga_param);
td_s32 ss_mpi_vo_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param);
td_s32 ss_mpi_vo_get_hdmi_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi_param);
td_s32 ss_mpi_vo_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param);
td_s32 ss_mpi_vo_get_hdmi1_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi1_param);
td_s32 ss_mpi_vo_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param);
td_s32 ss_mpi_vo_get_rgb_param(ot_vo_dev dev, ot_vo_rgb_param *rgb_param);
td_s32 ss_mpi_vo_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param);
td_s32 ss_mpi_vo_get_bt_param(ot_vo_dev dev, ot_vo_bt_param *bt_param);
td_s32 ss_mpi_vo_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param);
td_s32 ss_mpi_vo_get_mipi_param(ot_vo_dev dev, ot_vo_mipi_param *mipi_param);

td_s32 ss_mpi_vo_set_layer_priority(ot_vo_layer layer, td_u32 priority);
td_s32 ss_mpi_vo_get_layer_priority(ot_vo_layer layer, td_u32 *priority);
td_s32 ss_mpi_vo_bind_layer(ot_vo_layer layer, ot_vo_dev dev);
td_s32 ss_mpi_vo_unbind_layer(ot_vo_layer layer, ot_vo_dev dev);

#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_VO_DEV_H */
