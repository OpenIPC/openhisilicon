/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __DRV_EXPORT_H__
#define __DRV_EXPORT_H__

#include "xmedia_region.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//rgn
xmedia_s32 export_rgn_init(xmedia_void);
xmedia_void export_rgn_exit(xmedia_void);
xmedia_s32 export_rgn_create(xmedia_u32 handle, const xmedia_rgn_attr *rgn_attr);
xmedia_s32 export_rgn_destroy(xmedia_u32 handle);
xmedia_s32 export_rgn_get_attr(xmedia_u32 handle, xmedia_rgn_attr *rgn_attr);
xmedia_s32 export_rgn_set_attr(xmedia_u32 handle, const xmedia_rgn_attr *rgn_attr);
xmedia_s32 export_rgn_attach_to_chn(xmedia_u32 handle, const xmedia_chn_info *chn, const xmedia_rgn_chn_attr *chn_attr);
xmedia_s32 export_rgn_detach_from_chn(xmedia_u32 handle, const xmedia_chn_info *chn);
xmedia_s32 export_rgn_get_display_attr(xmedia_u32 handle, const xmedia_chn_info *chn, xmedia_rgn_chn_attr *chn_attr);
xmedia_s32 export_rgn_set_display_attr(xmedia_u32 handle, const xmedia_chn_info *chn, const xmedia_rgn_chn_attr *chn_attr);
xmedia_s32 export_rgn_get_canvas_info(xmedia_u32 handle, xmedia_rgn_canvas_info *canvas_info);
xmedia_s32 export_rgn_update_canvas (xmedia_u32 handle);
xmedia_s32 export_rgn_batch_begin(xmedia_u32 *grp, xmedia_u32 handle_num, const xmedia_u32 handle[]);
xmedia_s32 export_rgn_batch_end(xmedia_u32 grp);
//isp
xmedia_s32 export_isp_suspend_sensor(xmedia_u32 pipe);
xmedia_s32 export_isp_resume_sensor(xmedia_u32 pipe);
//vi
xmedia_void export_vi_register_misc_aov_callback(xmedia_s32 (*misc_aov_callback)(xmedia_s32 pipe));


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_EXPORT_H__ */

